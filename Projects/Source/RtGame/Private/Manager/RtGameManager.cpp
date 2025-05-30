// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/RtGameManager.h"

// Unreal Basic 
#include "Kismet/Gameplaystatics.h"
//#include "Engine/AssetManager.h"

//Default
#include "RtCore/Public/RtDefines.h"

// Session
#include "Common/RtSessionSystem.h"

// Framework
#include "Framework/RtPlayerController.h"
#include "Framework/RtPlayerState.h"
#include "Framework/RtGameState.h"

// Manager
#include "Manager/RtConfig.h"
#include "Manager/RtTrainingManager.h"
#include "Manager/RtAssetManager.h"
#include "Manager/RtTableManager.h"
#include "Manager/RtWebManager.h"
#include "Manager/RtReplayManager.h"

// Data
#include "RtCore/Public/Common/RtCommon.h"
#include "Common/RtRpcCommon.h"
#include "Common/RtModeSettingData.h"

// VR
#include "HeadMountedDisplayFunctionLibrary.h" // for GetDeviceWorldPose() WW_HMD
#include <RtResourceModelSubsystem.h>
#include <RtGameInstance.h>



DEFINE_LOG_CATEGORY(LogPlayerInfo);
DEFINE_LOG_CATEGORY(LogGameManager);

URtGameManager::URtGameManager() :UGameInstanceSubsystem(),
bTrainingRestartProcess(false)
{
}

void URtGameManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Collection.InitializeDependency(URtConfig::StaticClass());
	Collection.InitializeDependency(URtSessionSystem::StaticClass());
	Collection.InitializeDependency(URtWebManager::StaticClass());
	Collection.InitializeDependency(URtTableManager::StaticClass());

	Super::Initialize(Collection);

	//''load playsettings

	//'' init constant values
	Is_SoloPlay = false;      // Solo Play is TestMode
	
	/* Associated Save Data */
	SaveData_SlotName = "SaveData_Slot";
	SaveData_UserIndex = 0;

	Is_ListenServer = false; // Check Server Mode

	Check_Start_PlayMode();

	//Network
	URtSessionSystem* sub_system = GetGameInstance()->GetSubsystem< URtSessionSystem>();
	if (sub_system) {
		sub_system->OnCreateSessionCompleteEvent.AddDynamic(this, &URtGameManager::OnCreateSessionCompleted);
	}
	NickName = FString(TEXT("N"));
	
	// Load Saved Player Data from local profile data
	Player_SaveData = Load_SavedPlayerData();

	if (GIsEditor) {
		bInitialized = true;
	}
	else {
		URtAssetManager& AssetManager = URtAssetManager::Get();
		AssetManager.Preload(FStreamableDelegate::CreateUObject(this, &URtGameManager::HandleLoadCompleted));
	}

	if (IsXR()) {
	
	}
	else {
		URtConfig::bMoveUsingHmdController = false;
	}
	
	//''init training manager
	TrngManager = NewObject<URtTrainingManager>(this);
	
	// dave
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		URtGameInstance* RtGameInstance = Cast<URtGameInstance>(GameInstance);
		if (RtGameInstance)
		{
			RtGameInstance->OnNetworkFailure_Delegate.AddUObject(this, &URtGameManager::OnNetworkFailure);
		}
	}

	//For VR Start Trainee
	IConsoleManager::Get().RegisterConsoleCommand(TEXT("Rt.Menu.Trainee"),
		TEXT("[Device Name] [Listen Ip]"),
		FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args)
			{
				if (Args.Num() >= 2)
				{
					URtConfig::ListenUrl = FString::Printf(TEXT("192.168.0.%s"), *Args[1]);
					URtConfig::Play_Settings.Set_ServerId(URtConfig::ListenUrl);
				}

				FString deviceName = Args.Num() ? *Args[0] : URtConfig::Device_Name;

				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("Device Name : %s\nListen Ip : %s"), *deviceName, *URtConfig::ListenUrl));
				Start_Client(*deviceName, "");
			}));

	URtResourceModelSubsystem::InitReference(GetGameInstance());

}

void URtGameManager::Deinitialize()
{
	URtSessionSystem* sub_system = GetGameInstance()->GetSubsystem< URtSessionSystem >();
	if (sub_system) {
		sub_system->OnCreateSessionCompleteEvent.RemoveDynamic(this, &URtGameManager::OnCreateSessionCompleted);
	}

	for (IConsoleObject* AuditCmd : AuditCmds)
	{
		IConsoleManager::Get().UnregisterConsoleObject(AuditCmd);
	}
	AuditCmds.Empty();

	// dave
	if (TrngManager)
	{
		TrngManager->RemoveFromRoot();
		TrngManager = nullptr;
	}

	Super::Deinitialize();
}

FDelegateHandle URtGameManager::CallAndRegister_OnInitialized(FOnInitialized_Delegate::FDelegate InDelegate)
{
	if (IsInitialzied()) {
		InDelegate.Execute();
	}
	return OnInitialized_Delegate.Add(InDelegate);
}

bool URtGameManager::IsInitialzied()
{
	UE_LOG(LogGameManager, Log, TEXT("%d"), (int)bInitialized);

	if (!bInitialized)
		return false;

	if (!ModeDefinition) {
		ModeDefinition = URtAssetManager::Get().Load_ModeSetting();
	}
	return ModeDefinition ? true : false;
}


void URtGameManager::Init_Game(const FString& MapName, const FString& Options, FString& ErrorMessage, bool b_gamemode)
{
	if (b_gamemode) {

		if (Current_GameMode == ERt_GameMode::E_None) {
			if (URtConfig::Solo_MapName.IsEmpty())  // dave
			{
				Current_GameMode = ERt_GameMode::E_Multiplay;
			}
			else 
			{
				if (MapName.Contains(URtConfig::Solo_MapName))
				{
					Current_GameMode = ERt_GameMode::E_Soloplay;
				}
				else
				{
					UE_LOG(LogGameManager, Warning, TEXT("* Solo Map is assigned. But don't match. Check out the spells or keep going with Multiplay mode."));

					// Set GameMode : Multi
					Current_GameMode = ERt_GameMode::E_Multiplay;
				}
			}
		}
	}
	else {
		if (MapName.Contains(URtConfig::Menu_MapName)) {

			Current_GameMode = ERt_GameMode::E_Menu;
		}
		else if (MapName.Contains(URtConfig::Lobby_MapName)) {

			Current_GameMode = ERt_GameMode::E_Lobby;
		}
		else {
			Current_GameMode = ERt_GameMode::E_None;
		}
	}
}


void URtGameManager::Start_PlayMode()
{
	UE_LOG(LogGameManager, Log, TEXT("Call Start_PlayMode"));

	if (bRun_StartMode)
		return;
	bRun_StartMode = true;
	FString start_mode;
	auto* mgr = this;
	UWorld* World = GetWorld();
	check(mgr);
	check(World);
	FString current_map_name = World->GetMapName();
	bool ck_gamemap = false;

	switch (mgr->Get_Start_Play_Type())
	{
	case ERt_PlayType::E_Server:
	{
		if (ck_gamemap == false) {
			UE_LOG(LogGameManager, Log, TEXT("E_Server"));
			mgr->Start_Server();
			bRun_StartMode = true;
		}
	}
	break;
	case ERt_PlayType::E_Control:
	{
		if (ck_gamemap == false) {
			UE_LOG(LogGameManager, Log, TEXT("E_Control"));
			mgr->Start_ControlMode();
			bRun_StartMode = true;
		}
	}
	break;
	case ERt_PlayType::E_Trainee:
	{
		if (ck_gamemap == false) {
			UE_LOG(LogGameManager, Log, TEXT("E_Trainee"));
			FString device_name;
			FString device_id;
			bool ck_name = FParse::Value(FCommandLine::Get(), TEXT("DeviceName="), device_name);
			bool ck_id = FParse::Value(FCommandLine::Get(), TEXT("DeviceId="), device_id);
			mgr->Start_Client(device_name, device_id);
			bRun_StartMode = true;



		}
	}
	break;
	case ERt_PlayType::E_Replay:
	{
		if (ck_gamemap == false) {
			UE_LOG(LogGameManager, Log, TEXT("E_Replay"));
			FString replay_TID;
			FString replay_UID;
			FString replay_SCN;

			// 제거예정
			//bool ck_tid = FParse::Value(FCommandLine::Get(), TEXT("TID="), replay_TID);
			//bool ck_uid = FParse::Value(FCommandLine::Get(), TEXT("UID="), replay_UID);
			//bool ck_scn = FParse::Value(FCommandLine::Get(), TEXT("SNAME="), replay_SCN);

			FString commandLine = FCommandLine::Get();
			//FString low_commandLine = FCommandLine::Get();
			//FString low_commandLine = TEXT(" -interact:2080/2/");
			//UE_LOG(LogGameManager, Log, TEXT("%s"),*low_commandLine);
			//std::string de_commandLine_str = TCHAR_TO_UTF8(*low_commandLine);
			//std::string commandLine_str = GetBase64Decording(de_commandLine_str);
			//FString commandLine = UTF8_TO_TCHAR(commandLine_str.c_str());
			//UE_LOG(LogGameManager, Log, TEXT("%s"), *commandLine);

			//TArray<FString> result;
			//commandLine.ParseIntoArray(result, TEXT(":"), true);
			//UE_LOG(LogGameManager, Log, TEXT("result[1] = %s"), *result[1]);

			//TArray<FString> result2;
			//result[1].ParseIntoArray(result2, TEXT("/"), true);
			//UE_LOG(LogGameManager, Log, TEXT("result2[0] = %s"), *result2[0]);

			//replay_TID = result2[0];
			//replay_UID = result2[1];
			//replay_SCN = result2[2];

			replay_TID = TEXT("0");
			replay_UID = TEXT("0"); 
			replay_SCN = TEXT("None");

			mgr->Start_ReplayMode(replay_TID, replay_UID, replay_SCN);
			bRun_StartMode = true;
		}
	}
	break;
	case ERt_PlayType::E_Single:
	{
		if (ck_gamemap == false) {
			UE_LOG(LogGameManager, Log, TEXT("E_Single"));
			mgr->Start_SingleMode();
			bRun_StartMode = true;
		}
	}
	break;
	default:
		bRun_StartMode = true;
		UE_LOG(LogGameManager, Log, TEXT("E_None"));
		break;
	}
}



void URtGameManager::BPCall_StartMode()
{
	if (bInitialized) {
		//Start_Mode(Start_Play_Type);
		Start_PlayMode();
	}
}

void URtGameManager::HandleLoadCompleted()
{
	checkNoReentry();

	UAssetManager& AssetManager3 = UAssetManager::Get();
	FPrimaryAssetType AssetType3 = FPrimaryAssetType(URtModeSettingData::StaticClass()->GetFName());

	FPrimaryAssetId AssetId4 = FPrimaryAssetId(AssetType3, "DA_ModeDatas");

	UObject* AssetPtr3 = nullptr;
	AssetPtr3 = AssetManager3.GetPrimaryAssetObject(AssetId4);

	ModeDefinition = Cast<URtModeSettingData>(AssetPtr3);

	UE_LOG(LogGameManager, Log, TEXT("GameManager initialized"));

	OnInitialized_Delegate.Broadcast();

	//Initialize Manager
	bInitialized = true;
}

void URtGameManager::Host_Game()
{
	Create_Session();
}

void URtGameManager::Start_Server()
{
	if (!IsInitialzied()) {
		UE_LOG(LogGameManager, Log, TEXT("GameManager is not Initialized yet"));
		return;
	}
	Is_SoloPlay = false;
	Play_Type = ERt_PlayType::E_Server;
	UGameplayStatics::OpenLevel(this, FName(*URtConfig::Lobby_MapName));

	URtSessionSystem* ss = GetGameInstance()->GetSubsystem<URtSessionSystem>();
	if (ss) {
		bRun_StartMode = true;
		ss->CreateSession(URtConfig::NumPublicConnections, true);
	}

	auto* webManager = GetGameInstance()->GetSubsystem<URtWebManager>();
	if (webManager) {

		webManager->Init_WebManager();
	}
}

void URtGameManager::Start_Client(const FString& InDeviceName, const FString& InDeviceID)
{
	if (!IsInitialzied())
		return;

	//Play_Type = ERt_PlayType::E_Client;
	Play_Type = ERt_PlayType::E_Trainee;
	FString options = FString::Printf(TEXT("%s=%d?%s=%s?%s=%s?%s=%s?%s=%s?%s=%s"),
		*RtString_Option::PlayerType, Play_Type,
		*RtString_Option::PlayerName, L"Any",
		*RtString_Option::PlayerId, L"PlayId",
		*RtString_Option::PlayerPassword, L"PlayPassword",
		*RtString_Option::DeviceName, *InDeviceName,
		*RtString_Option::DeviceId, *InDeviceID
	);
	if (auto* world = GetWorld()) {
		bRun_StartMode = true;
		// Level Name
		UGameplayStatics::OpenLevel(GetWorld(), URtConfig::Get_ServerLevel(), true, options);
	    // PC IP 
	}

	auto* webManager = GetGameInstance()->GetSubsystem<URtWebManager>();
	if (webManager) {

		webManager->Init_WebManager();
	}
}



void URtGameManager::Start_ControlMode()
{
	if (auto* world = GetWorld()) {
		UGameplayStatics::SetEnableWorldRendering(world, false);
	}
	if (!IsInitialzied())
		return;

	Play_Type = ERt_PlayType::E_Control;
	//InPc->Set_PlayType(Play_Type);
	FString options = FString::Printf(TEXT("%s=%d?%s=%s?%s=%s?%s=%s"),
		*RtString_Option::PlayerType, Play_Type,
		*RtString_Option::PlayerName, L"Any",
		*RtString_Option::PlayerId, L"PlayId",
		*RtString_Option::PlayerPassword, L"PlayPassword"
	);
	bRun_StartMode = true;
	UGameplayStatics::OpenLevel(GetWorld(), URtConfig::Get_ServerLevel(), true, options);

	auto* webManager = GetGameInstance()->GetSubsystem<URtWebManager>();
	if (webManager) {

		webManager->Init_WebManager();
	}
}

void URtGameManager::Start_TrainingMode()
{
	if (!IsInitialzied())
		return;

	Play_Type = ERt_PlayType::E_Trainee;
	// Just Init Options , * we need code for fill this options
	FString options = FString::Printf(TEXT("%s=%d?%s=%s?%s=%s?%s=%s?%s=%s?%s=%s"),
		*RtString_Option::PlayerType, Play_Type,
		*RtString_Option::PlayerName, L"Any",
		*RtString_Option::PlayerId, L"PlayId",
		*RtString_Option::PlayerPassword, L"PlayPassword",
		*RtString_Option::DeviceName, L"DeviceName",
		*RtString_Option::DeviceId, L"DeviceID"
	);

	bRun_StartMode = true;
	UGameplayStatics::OpenLevel(GetWorld(), URtConfig::Get_ServerLevel(), true, options);

	auto* webManager = GetGameInstance()->GetSubsystem<URtWebManager>();
	if (webManager) {

		webManager->Init_WebManager();
	}
}


void URtGameManager::Start_ReplayMode(FString TID, FString UID ,FString SCN)
{
	if (!IsInitialzied())
		return;

	if (auto* gi = GetGameInstance()) {
		if (auto* configmgr = gi->GetSubsystem<URtConfig>()) {
			Play_Type = ERt_PlayType::E_Replay;
			// Just Init Options , * we need code for fill this options
			FString options = FString::Printf(TEXT("%s=%d"),
				*RtString_Option::PlayerType, Play_Type
			);
			bRun_StartMode = true;
			// Maybe TrainingMap
			UGameplayStatics::OpenLevel(GetWorld(), FName(*URtConfig::Replay_MapName), true, options);
		}

		if (auto* replay_mgr = gi->GetSubsystem<URtReplayManager>()) {
			// Set ReplayFileName
			replay_mgr->ReplayInfo.SelectedTID = FCString::Atoi(*TID);
			replay_mgr->ReplayInfo.SelectedUID = FCString::Atoi(*UID);
			replay_mgr->ReplayInfo.SelectedScenarioName = SCN;
		}
	}

	auto* webManager = GetGameInstance()->GetSubsystem<URtWebManager>();
	if (webManager) {

		webManager->Init_WebManager();
	}
}

void URtGameManager::Start_SingleMode()
{
	Play_Type = ERt_PlayType::E_Single;
	
	FText text = FText::FromString(FString::Printf(TEXT("%s %s"), SINGLE_NAME, PROJECT_VERSION));
	UKismetSystemLibrary::SetWindowTitle(text);

	UGameplayStatics::OpenLevel(GetWorld(), FName(*URtConfig::Game_MapName));
}

void URtGameManager::Quit_Program()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void URtGameManager::OnCreateSessionCompleted(bool Successful)
{
	Is_ListenServer = true;  //`` listen server bool : set
	GetWorld()->ServerTravel(URtConfig::Game_MapName + TEXT("?listen"));
}

void URtGameManager::HasStarted_MatchState(const FName& InState)
{
	if (auto* pc = Get_LocalPlayerController())
	{
	    // Print Screen
		pc->HasStarted_MatchState(InState);
	}
}

// dave
void URtGameManager::HasExited_MatchState(const FName& InState)
{
	if (auto* pc = Get_LocalPlayerController())
	{
		pc->HasExited_MatchState(InState);
	}
}

ARtPlayerController* URtGameManager::Get_LocalPlayerController()
{
	auto* pc = Cast<ARtPlayerController>(GetGameInstance()->GetFirstLocalPlayerController());
	return pc;
}

URtTableManager* URtGameManager::Get_TableManager()
{
	return GetGameInstance()->GetSubsystem<URtTableManager>();
}



void URtGameManager::Save_PlayerData(const FRtPlayerSaveData& player_data)
{
	URtSaveGame* SaveGameInstance = Cast<URtSaveGame>(UGameplayStatics::CreateSaveGameObject(URtSaveGame::StaticClass()));
	SaveGameInstance->PlayerProfile = Player_SaveData;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveData_SlotName, SaveData_UserIndex);
}

FRtPlayerSaveData URtGameManager::Load_SavedPlayerData()
{
	return FRtPlayerSaveData();
}

bool URtGameManager::IsHeadMountedDisplayEnabled()
{
#if !UE_SERVER
	if (!GEngine || !GEngine->XRSystem)
		return false;
	return UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
#else
	return false;
#endif
}

bool URtGameManager::IsXR()
{
	return IsHeadMountedDisplayEnabled();
}

void URtGameManager::Create_Session()
{
	URtSessionSystem* sub_system = GetGameInstance()->GetSubsystem<URtSessionSystem>();
	if (sub_system) {
		sub_system->CreateSession(URtConfig::NumPublicConnections, true);
	}
}

void URtGameManager::Check_Start_PlayMode()
{
	UE_LOG(LogGameManager, Log, TEXT("Call Check_Start_PlayMode"));
	FString start_mode;
	UWorld* World = GetWorld();
	check(World);
	Start_Play_Type = ERt_PlayType::E_None;

	// temp testcode 제거예정
	FString temp;
	temp = FCommandLine::Get();
	UE_LOG(LogGameManager, Log, TEXT("%s"), *temp);

	if (bool ck = FParse::Value(FCommandLine::Get(), TEXT("StartMode="), start_mode)) {
		if (start_mode.IsEmpty() == false) {
			if (start_mode.Equals(TEXT("Server"))) {
				Start_Play_Type = ERt_PlayType::E_Server;
			}
			else if (start_mode.Equals(TEXT("Control"))) {
				Start_Play_Type = ERt_PlayType::E_Control;
			}
			else if (start_mode.Equals(TEXT("Trainee"))) {
				Start_Play_Type = ERt_PlayType::E_Trainee;
			}
			else if (start_mode.Equals(TEXT("Single"))) {
				Start_Play_Type = ERt_PlayType::E_Single;
			}
			else  /*(start_mode.Equals(TEXT("Replay"))) */{
				Start_Play_Type = ERt_PlayType::E_Replay;
			}
		
			UE_LOG(LogPlayerInfo, Log, TEXT("StartMode = %s"), *start_mode);
		}
		else {
			UE_LOG(LogPlayerInfo, Log, TEXT("StartMode = Empty"));
		}
	}
	else {
		// Fail is Replay
		Start_Play_Type = ERt_PlayType::E_Replay;
		UE_LOG(LogPlayerInfo, Log, TEXT("StartMode = Replay"));
	}
}


// dave
void URtGameManager::OnNetworkFailure(ENetworkFailure::Type FailureType)
{
	switch (FailureType)
	{
	case ENetworkFailure::FailureReceived:
	{
		break;
	}
	case ENetworkFailure::PendingConnectionFailure:
	{
		break;
	}
	case ENetworkFailure::ConnectionLost:
	case ENetworkFailure::ConnectionTimeout:
	{
		break;
	}
	case ENetworkFailure::NetDriverAlreadyExists:
	case ENetworkFailure::NetDriverCreateFailure:
	case ENetworkFailure::OutdatedClient:
	case ENetworkFailure::OutdatedServer:
	default:
		break;
	}
}


void FRtUserInfo_PreLogin::Set_Options(const FString& option_p)
{
	Options = option_p;

	FString temp = UGameplayStatics::ParseOption(option_p, RtString_Option::PlayerType);
	uint8 temp_interger = FCString::Atoi(*temp);
	
	if (temp_interger >= (uint8)(ERt_PlayType::E_Replay)) {
		PlayType = ERt_PlayType::E_None;
	}
	else {
		PlayType = (ERt_PlayType)temp_interger;
	}

	PlayerName = UGameplayStatics::ParseOption(option_p, RtString_Option::PlayerName);
	PlayerId = UGameplayStatics::ParseOption(option_p, RtString_Option::PlayerId);
	PlayerPassword = UGameplayStatics::ParseOption(option_p, RtString_Option::PlayerPassword);
	DeviceName = UGameplayStatics::ParseOption(option_p, RtString_Option::DeviceName);
	DeviceId = UGameplayStatics::ParseOption(option_p, RtString_Option::DeviceId);
}


std::string URtGameManager::GetBase64Decording(std::string encoded_string)
{
	const std::string base64_chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4] = { 0, }, char_array_3[3] = { 0, };

	std::string ret;

	while (in_len-- && (encoded_string[in_] != '=') && IsBase64(encoded_string[in_]))
	{
		char_array_4[i++] = encoded_string[in_];
		in_++;

		if (i == 4)
		{
			for (i = 0; i < 4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i]; i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 4; j++)
			char_array_4[j] = 0;

		for (j = 0; j < 4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++)
			ret += char_array_3[j];
	}

	return ret;
}

bool URtGameManager::IsBase64(uint8 c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}






#pragma  region InfoLogRegion
void FRtUserInfo_PreLogin::Print_Log()
{
	UE_LOG(LogPlayerInfo, Log, TEXT("Options: %s"), *Options);
	UE_LOG(LogPlayerInfo, Log, TEXT("Address: %s"), *Address);
	UE_LOG(LogPlayerInfo, Log, TEXT("UniqueId: %s"), *UniqueId);
	UE_LOG(LogPlayerInfo, Log, TEXT("ErrorMessage: %s"), *ErrorMessage);
	UE_LOG(LogPlayerInfo, Log, TEXT("TimeStr: %s"), *TimeStr);
}

void FRtUserInfo_PostLogin::Print_Log()
{
	UE_LOG(LogPlayerInfo, Log, TEXT("Unique_Id: %s"), *Unique_Id);
	UE_LOG(LogPlayerInfo, Log, TEXT("Player_Id: %s"), *Player_Id);
	UE_LOG(LogPlayerInfo, Log, TEXT("Player_Name: %s"), *Player_Name);
	UE_LOG(LogPlayerInfo, Log, TEXT("TimeStr: %s"), *TimeStr);
}

#pragma endregion







