 // Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtGameMode.h"
// Unreal Basic Framework �߰�
#include "GameFramework/GameSession.h"


// Framework 
#include "Framework/RtGameState.h"
#include "Framework/RtPlayerState.h"
#include "Framework/RtPlayerController.h"
#include "Framework/RtReplayPlayerController.h"
#include "Framework/RtGameHUD.h"
#include "Framework/RtSpectatorPawn.h"

// Manager 
#include "Manager/RtGameManager.h"
#include "Manager/RtWebManager.h"
#include "Manager/RtTrainingManager.h"
#include "Manager/RtReplayManager.h"
#include "Manager/RtConfig.h"

#include "Network/RtNetworkManager.h" 

// Data 
#include "Common/RtBlueprintAssetPath.h"
#include "RtCore/Public/Common/RtCommon.h"

// Level Prop 
#include "Level/RtTrainingReadyPoint.h"
#include "Level/RtLevelScriptActor.h"

#include "RtResourceModelSubsystem.h"


ARtGameMode::Dele_Player_Connected ARtGameMode::Player_Connected_Delegate;

ARtGameMode::ARtGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// ���� ������Ʈ ����, �÷��̾� ������Ʈ ���� 
	GameStateClass = ARtGameState::StaticClass();
	PlayerControllerClass = ARtPlayerController::StaticClass();
	PlayerStateClass = ARtPlayerState::StaticClass();

	// ��(Default Pawn) ����
	const TCHAR* pawnClass;
	pawnClass = RtBlueprintAsset::MainCharacter;

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(pawnClass);
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	HUDClass = ARtGameHUD::StaticClass();
	SpectatorClass = ARtSpectatorPawn::StaticClass();
	Match_State = RtMatchState::EnteringMap;
	//ReplaySpectatorPlayerControllerClass = ARtReplayPlayerController::StaticClass();

}

bool ARtGameMode::IsControllerPlayType(const int32 InPlayerId)
{
	if (auto* pc = Get_PlayerController(InPlayerId)) 
	{
		bool ref = (pc->Get_PlayType() == ERt_PlayType::E_Control);
		return ref;
	}
	return false;
}

ARtPlayerController* ARtGameMode::IsControllerId(int32 InId)
{
	if (auto* pc = Get_PlayerController(InId)) {
		auto play_type = pc->Get_PlayType();
		if (play_type == ERt_PlayType::E_Control) {
			return pc;
		}
		else {
			UE_LOG(LogRtServer, Error, TEXT("Invalid Call: Only the controller can initiate training."));
			//pc->Client_ErrorMessage(TEXT("Invalid Call: Only the controller can initiate training."));
			return nullptr;
		}
	}
	return nullptr;
}

void ARtGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	Net_Mode = GetNetMode();
	bAutoPlayType = GIsEditor;

	if (Net_Mode == NM_Standalone) {
	}
	else {
		Set_MatchState(RtMatchState::EnteringMap);

		auto* mgr = GetGameInstance()->GetSubsystem<URtGameManager>();
		mgr->Init_Game(MapName, Options, ErrorMessage, true);
	}
}

void ARtGameMode::InitGameState()
{
	Super::InitGameState();

	auto* gs = Cast<ARtGameState>(GameState);
	if (gs) {
		gs->Init_GameState();
	}
}

void ARtGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();


}

void ARtGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	++ConnectCount;

	if (Net_Mode == NM_Standalone) {
	}
	else {
		if (auto* mgr = GetGameInstance()->GetSubsystem<URtGameManager>()) {
			if (Pre_Login(Options, Address, UniqueId, ErrorMessage, mgr->Get_Current_GameMode()) == false) {
				FGameModeEvents::GameModePreLoginEvent.Broadcast(this, UniqueId, ErrorMessage);
				return;
			}
		}
	}
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

void ARtGameMode::PostLogin(APlayerController* NewPlayer)
{
	Post_Login_Sub(NewPlayer);

	if (IsServerMode()) {
		if (auto* pc = Cast<ARtPlayerController>(NewPlayer)) {
			if (NewPlayer->IsLocalController()) {
				if (auto* player_state = Cast<ARtPlayerState>(NewPlayer->PlayerState)) {
					player_state->Play_Type = ERt_PlayType::E_Server;
					// Controller = spectator 
					pc->Set_Spectator(true);
				}
			}
			else { // case of client
				if (auto* player_state = Cast<ARtPlayerState>(NewPlayer->PlayerState)) {
					if (auto* mgr = GetGameInstance()->GetSubsystem<URtGameManager>()) {
						Post_Login(NewPlayer, mgr->Get_Current_GameMode());

						FRtUserInfo_PreLogin out_info;
						if (Get_PreLoginInfo(pc, out_info))
						{
							// Deliver Play Type : Server.., Trainee
							player_state->Set_AddtionalInformation(out_info);

							if (player_state->Play_Type == ERt_PlayType::E_Control) {
								pc->Set_Spectator(true);
								UE_LOG(LogRtServer, Warning, TEXT("Logged-in Control!"));
							}
							else if (player_state->Play_Type == ERt_PlayType::E_Trainee) {
								pc->Set_Spectator(false); 
								UE_LOG(LogRtServer, Warning, TEXT("Logged-in Trainee!"));
							}
							else if (player_state->Play_Type == ERt_PlayType::E_Replay) {
								pc->Set_Spectator(true);
								UE_LOG(LogRtServer, Warning, TEXT("Logged-in Replay!"));
							}
							else {
								pc->Set_Spectator(false);

							}
						}
						else {
							UE_LOG(LogRtServer, Warning, TEXT("Invalid Play Type!"));
						}
					}
				}
				else {
					UE_LOG(LogRtServer, Log, TEXT("Login Failed!"));
				}
			}
		}

		OnPostLogin_Delegate.Broadcast(this, NewPlayer);
	}
	Super::PostLogin(NewPlayer);
}

void ARtGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	auto* game_manager = GetGameManager();
	if (game_manager && game_manager->Is_Play_Type(ERt_PlayType::E_Server)) {

		//Handling duplicate device logins
		auto exist_devices = ExistDeviceAll(NewPlayer);
		for (auto* it : exist_devices) {
			GameSession->KickPlayer(it, FText::FromString(TEXT("Multiplay Login")));
		}

		if (ARtPlayerController* player = Cast< ARtPlayerController>(NewPlayer)) {
			if (Add_New_Player(player)) {
				SC_DeviceInfo();
			}
		}
	}
}

void ARtGameMode::Logout(AController* Exiting)
{
	if (!Exiting)
	{
		Super::Logout(Exiting);
		return;
	}

	ARtPlayerController* PlayerController = Cast<ARtPlayerController>(Exiting);
	if (!PlayerController)
	{
		Super::Logout(Exiting);
		return;
	}

	if (IsServerMode() && Remove_Player(PlayerController))
	{
		SC_DeviceInfo();

		// dave - Release network resoueces	
		PlayerController->Client_Disconnect();
	}

	Super::Logout(Exiting);
}

void ARtGameMode::StartPlay()
{
	Super::StartPlay();

	UE_LOG(LogRtServer, Log, TEXT("Game has Started!"));
	
	if (auto* mgr = GetGameManager()) {
		//mgr->Check_TrainingRestartProcess(this);
	}
	
	if (Match_State == RtMatchState::EnteringMap)
	{
		Set_MatchState(RtMatchState::WaitingToStart);
	}
	
	// Check to see if we should immediately transfer to match start
	if (Match_State == RtMatchState::WaitingToStart && ReadyToStartMatch())
	{
		StartMatch();
	}
}
//'' �Ʒ��� ������ ��
void ARtGameMode::Confirm_TraineeAassignment(int32 InId)
{
	auto* pc = IsControllerId(InId);
	if (pc == nullptr) {
		UE_LOG(LogRtServer, Error, TEXT("The training controller didn't call."));
		return;
	}
	Confirm_TraineeAassignment(false);

	Change_PlayersInfo(Get_DeviceArray());
	SC_DeviceInfo();

	if (IsServerMode()) {
		pc->Client_Confirm_TraineeAassignment();
	}
		
	for (auto* trainee : Trainee_Array)
	{
	    //trainee->Get_GameState()->Set_Scenario_Info(Control_Array[0]->Get_GameState()->Get_Scenario_Info());
		trainee->Get_GameState()->Set_Scenario_Info(pc->Current_Scenario);
		trainee->Client_Confirm_TraineeAassignment();
	}

	Set_MatchState(RtMatchState::InProgress_PlayerReady); // run next progress

	ARtPlayerController* playerController = Get_PlayerController();
	if (playerController)
	{
		if (auto* player_state = playerController->Get_PlayerState()) {
			if (player_state->Play_Type == ERt_PlayType::E_Server) {
				if (auto* gameState = Get_GameState()) {
					FString sctFileName = gameState->Get_Scenario_Info().FileName;
					if (!sctFileName.IsEmpty())
					{
						playerController->LoadSct(); // TODO : 리슨서버 전용 LoadSct
					}
				}
			}
		}
	}

}

void ARtGameMode::Confirm_TraineeAassignment(bool bSync)
{
	// change status to ready
	for (auto& it : Get_DeviceArray()) {
		if (it.Active == true && it.IsValid_TraineeInformation()) {
			it.StatusInTraining = EStatusInTraining::Ready;
		}
		else {
			it.StatusInTraining = EStatusInTraining::Default;
		}
	}

	if (bSync)
		SC_DeviceInfo();// update device info.. after
}

void ARtGameMode::Confirm_TraineeDeployment(int32 InId)
{
	bool res = false;

	if (auto* trmgr = Get_TrngManager()) {
			if (auto* pc = Get_PlayerController(InId)) {
				auto play_type = pc->Get_PlayType();
				if (play_type == ERt_PlayType::E_Control) {

					// Server_StartTraining_Implementation 
					if (ARtGameState* gs = pc->Get_GameState()) {
						int32 tid = gs->Training_Index;
						// Save TID in Server
						if (ARtGameState* server_gs = Get_GameState()) {
							server_gs->Training_Index = tid;
						}

					}
					pc->Client_Confirm_TraineeDeployment();

				}
				else {
					UE_LOG(LogRtServer, Error, TEXT("Invalid Call: Only the controller can initiate training."));
				}
			}
			res = true;
	}

	if (res) {
		// State : Running Training
		Set_MatchState(RtMatchState::InProgress_RunningTrng);
	}
}

// dave
void ARtGameMode::Confirm_BackConfirmation(int32 InId)
{
	bool res = false;

	if (auto* trmgr = Get_TrngManager()) 
	{
		if (auto* pc = Get_PlayerController(InId)) 
		{
			auto play_type = pc->Get_PlayType();
			if (play_type == ERt_PlayType::E_Control) 
			{
				// Server_StartTraining_Implementation 
				if (ARtGameState* gs = pc->Get_GameState()) 
				{
					int32 tid = gs->Training_Index;
					// Save TID in Server
					if (ARtGameState* server_gs = Get_GameState()) 
					{
						server_gs->Training_Index = tid;
					}
				}
				pc->Client_Confirm_TraineeDeployment();

			}
			else {
				UE_LOG(LogRtServer, Error, TEXT("Invalid Call: Only the controller can initiate training. - Reset"));
			}
		}
		res = true;
	}

	if (res) {
		StartMatch();
	}
}


//dave
void ARtGameMode::RequestResetTraining(int32 InId)
{
	auto* pc = IsControllerId(InId);
	if (pc == nullptr) 
	{
		UE_LOG(LogRtServer, Error, TEXT("The training controller didn't call. - Reset"));
		return;
	}

	if (IsServerMode()) 
	{
		pc->Client_ResetTraining();
	}

	for (auto* trainee : Trainee_Array)
	{
		trainee->Client_ResetTraining();
	}

	Set_MatchState(RtMatchState::WaitingToStart);
}



bool ARtGameMode::Pre_Login(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage, ERt_GameMode InGameMode)
{
	//`` ������ ������ �ź��Ѵ�.
	if (Reject_AllClient_Event) {
		ErrorMessage = TEXT("Force Reject All Client");
		return false;
	}
	FString str_Options = FString::Printf(TEXT("Options : %s"), *Options);
	FString str_Address = FString::Printf(TEXT("Address : %s"), *Address);
	
	UE_LOG(LogRtServer, Log, TEXT("%s"), *str_Options);
	UE_LOG(LogRtServer, Log, TEXT("%s"), *str_Address);
	
	// Save Player Information.
	FRtUserInfo_PreLogin info;
	info.Set_Options(Options);
	info.Address = Address;
	info.UniqueId = UniqueId.ToString();
	info.ErrorMessage = ErrorMessage;
	info.TimeStr = FDateTime::Now().ToString();

	// This is Filling Code. We get needed data from rtsettings.ini 
    // This Step, we need Only Device data for matching Trainee Login
	//info.DeviceName = URtConfig::Device_Name;
	

	if (Check_PreLoginIntegrity(info) == false) {
		ErrorMessage = TEXT("Invalid Devcie Info");
		return false;
	}

	if (InGameMode == ERt_GameMode::E_Multiplay) {
		Game_PreLogin.Add(info);
	
		if (Game_Login_Try_ClientList.Contains(info.UniqueId) == false) {
			Game_Login_Try_ClientList.Add(info.UniqueId, info);
			UE_LOG(LogRtServer, Log, TEXT("New User: %s"), *info.UniqueId);
		}
		else {
			UE_LOG(LogRtServer, Log, TEXT("Exist User: %s"), *info.UniqueId);
		}
	}
	
	if (Lobby_Login_Try_ClientList.Contains(info.UniqueId) == false) {
		Lobby_Login_Try_ClientList.Add(info.UniqueId, info);
		UE_LOG(LogRtServer, Log, TEXT("New User]: %s"), *info.UniqueId);
	}
	else {
		UE_LOG(LogRtServer, Log, TEXT("Exist User]: %s"), *info.UniqueId);
	}
	return true;
}

void ARtGameMode::Post_Login(APlayerController* InPlayerController, ERt_GameMode InGameMode)
{
	FString unique_id = InPlayerController->PlayerState->GetUniqueId().ToString();
	int32 player_id = InPlayerController->PlayerState->GetPlayerId();
	FString player_name = InPlayerController->PlayerState->GetPlayerName();

	FString str_unique_id = FString::Printf(TEXT("UniqueID : %s"), *unique_id);
	FString str_player_id = FString::Printf(TEXT("PlayerID : %d"), player_id);
	FString str_player_name = FString::Printf(TEXT("PlayerName: : %s"), *player_name);

	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, str_unique_id);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, str_player_id);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, str_player_name);

	UE_LOG(LogRtServer, Log, TEXT("%s"), *str_unique_id);
	UE_LOG(LogRtServer, Log, TEXT("%s"), *str_player_id);
	UE_LOG(LogRtServer, Log, TEXT("%s"), *str_player_name);

	FRtUserInfo_PostLogin info;
	info.Unique_Id = unique_id;
	info.Player_Id = str_player_id;
	info.Player_Name = str_player_name;
	info.TimeStr = FDateTime::Now().ToString();

	if (InGameMode == ERt_GameMode::E_Multiplay) {
		Game_PostLogin.Add(info);
	}
	else {
		Others_PostLogin.Add(info);
	}
}

bool ARtGameMode::Get_PreLoginInfo(ARtPlayerController* InPlayerController, FRtUserInfo_PreLogin& OutPreLoginInfo)
{
	if (InPlayerController) {
		FString unique_id = InPlayerController->PlayerState->GetUniqueId().ToString();

		bool found = false;
		for (auto& it : Game_PreLogin) {
			if (it.UniqueId.Equals(unique_id)) {
				OutPreLoginInfo = it;
				return true;
			}
		}
	}
	return false;
}

ERt_PlayType ARtGameMode::Get_Recored_PlayType_InPreloginInfo(ARtPlayerController* InPlayerController)
{
	FRtUserInfo_PreLogin out_info;
	if (Get_PreLoginInfo(InPlayerController, out_info)) {
		return out_info.PlayType;
	}
	return ERt_PlayType::E_None;
}

void ARtGameMode::Change_PlayerInfo(const FRtDeviceInfo& InInfo)
{
	if (auto* pc = Get_PlayerController(InInfo.PlayerId)) {
		if (auto* ps = pc->Get_PlayerState()) {
			ps->Set_PlayerInfo(InInfo);
		}
		else {
			UE_LOG(LogRtServer, Log, TEXT("Null Playstate"));
		}
	}
}

void ARtGameMode::Change_PlayersInfo(const TArray<FRtDeviceInfo>& InInfo)
{
	if (auto* mgr = GetGameManager()) {
		auto& device_array = mgr->TrngManager->Get_Device_StatusArray();
		for (int i = 0; i < device_array.Num(); ++i) {
			Change_PlayerInfo(device_array[i]);
		}
	}
}

bool ARtGameMode::AssignTrainee_ToDevice(const FString& InDeviceName, const FRtTraineeInfo& InInfo, int32 InPlayerId)
{
	auto* mgr = GetGameManager();
	auto* trainingmgr = mgr->TrngManager;
	bool res = false;
	if (IsControllerPlayType(InPlayerId) == false) {
		return res;
	}
	if (InDeviceName.IsEmpty()) {
		return res;
	}
	if (auto* found_deviceinfo = trainingmgr->AssignTrainee_ToDevice(InDeviceName, InInfo)) {
		if (auto* ps = Get_GameState()->Find_DeviceUser(InDeviceName)) {
			ps->Set_PlayerInfo(*found_deviceinfo);
		}
		res = true;
	}
	if (res) {
		SC_DeviceInfo(InPlayerId);
	}
	return res;
}


bool ARtGameMode::AssignRole_ToDevice(const FString& InDeviceName, const FRtRoleInfo& InInfo, int32 InPlayerId)
{
	auto* mgr = GetGameManager();
	auto* trainingmgr = mgr->TrngManager;
	bool res = false;


	if (IsControllerPlayType(InPlayerId) == false) {
		return res;
	}
	if (InDeviceName.IsEmpty()) {
		return res;
	}
	if (auto* found_deviceinfo = trainingmgr->AssignRole_ToDevice(InDeviceName, InInfo)) {
		if (auto* ps = Get_GameState()->Find_DeviceUser(InDeviceName)) {
			ps->Set_PlayerInfo(*found_deviceinfo);
		}
		res = true;
	}
	if (res) {
		SC_DeviceInfo(InPlayerId);
	}
	return res;
}

ARtGameState* ARtGameMode::Get_GameState()
{
	return Cast<ARtGameState>(GameState);
}

ARtPlayerController* ARtGameMode::Get_PlayerController(int32 InPlayerId)
{
	for (auto* pc : Trainee_Array) {
		if (pc) {
			if (auto* ps = pc->Get_PlayerState()) {
				if(ps)
				{
					if (ps->GetPlayerId() == InPlayerId) {
						return pc;
					}
				}
			}
		}
	}

	if(Control_Controller) {
			if (auto* ps = Control_Controller->Get_PlayerState()) {
				if(ps){
					if (ps->GetPlayerId() == InPlayerId) {
						return Control_Controller;
					}
				}
			}
	}

	return nullptr;
}

ARtPlayerController* ARtGameMode::Get_PlayerController()
{
	return Cast<ARtPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

URtGameManager* ARtGameMode::GetGameManager()
{
	if (auto* game_instacne = GetGameInstance())
		return game_instacne->GetSubsystem<URtGameManager>();

	return nullptr;
}

void ARtGameMode::SC_DeviceInfo()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It) {
		ARtPlayerController* pc = Cast<ARtPlayerController>(*It);
		if (pc != nullptr) {
			pc->Client_Update_DeviceInfo(Get_DeviceArray());
		}
	}
}
void ARtGameMode::SC_DeviceInfo(int32 InId)

{
	if (auto* player = Get_PlayerController(InId)) {
		player->Client_Update_DeviceInfo(Get_DeviceArray());
	}
}


void ARtGameMode::SC_TraineeInfo()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It) {
		ARtPlayerController* pc = Cast<ARtPlayerController>(*It);
		if (pc != nullptr) {
			pc->Client_Update_TraineeInfo(Get_TraineeArray());
		}
	}
}

void ARtGameMode::SC_TraineeInfo(int32 InId)
{
	if (auto* player = Get_PlayerController(InId)) {
		player->Client_Update_TraineeInfo(Get_TraineeArray());
	}
}

ARtPlayerController* ARtGameMode::ExistDevice(APlayerController* NewPlayer)
{
	if (ARtPlayerController* player = Cast< ARtPlayerController>(NewPlayer)) {
		FRtDeviceInfo NewPlayerInfo;
		if (player->Get_DeviceInfo(NewPlayerInfo)) {

			for (auto* temp_player : Trainee_Array) {
				if (temp_player) {
					if (auto* ps = temp_player->Get_PlayerState()) {
						FRtDeviceInfo OutInfo;
						if (player->Get_DeviceInfo(OutInfo)) {
							if (OutInfo.Name.Equals(NewPlayerInfo.Name)) {
								return temp_player;
							}
						}
					}
				}
			}
		}
	}
	return nullptr;
}

TArray<ARtPlayerController*> ARtGameMode::ExistDeviceAll(APlayerController* NewPlayer)
{
	TArray<ARtPlayerController*> res;
	if (ARtPlayerController* new_player = Cast< ARtPlayerController>(NewPlayer)) {
		if (auto* newplayer_ps = new_player->Get_PlayerState()) {
			auto& newplayer_device_name = newplayer_ps->Login_DeviceName;
			for (auto* temp_player : Trainee_Array) {
				if (temp_player) {
					if (auto* ps = temp_player->Get_PlayerState()) {
						if (ps->Get_PlayerInfo().Name.Equals(newplayer_device_name)) {
							res.Add(temp_player);
						}
					}
				}
			}
		}
	}
	return res;
}


void ARtGameMode::OnMatchStateSet()
{
	FGameModeEvents::OnGameModeMatchStateSetEvent().Broadcast(Match_State);
	// Call change callbacks
	if (Match_State == RtMatchState::WaitingToStart)
	{
		Handle_MatchIsWaitingToStart();
	}
	else if (Match_State == RtMatchState::InProgress_StartTrng)
	{
		//Handle_MatchHasStarted();
	}
	//else if (Match_State == RtMatchState::InProgress_PlayerReady)
	//{
	//	
	//}
	//else if (Match_State == RtMatchState::InProgress_RunningTrng)
	//{
	//	
	//}
	//else if (Match_State == RtMatchState::InProgress_PreResult)
	//{
	//	  // finish training  .. replay stop
	//}
	else if (Match_State == RtMatchState::InProgress_Result)
	{
		  // finish training
		Handle_InProgress_Result_HasStarted();
	}
	else if (Match_State == RtMatchState::WaitingPostMatch)
	{
		//	HandleMatchHasEnded();
	}
	else if (Match_State == RtMatchState::LeavingMap)
	{
		//	HandleLeavingMap();
	}
	else if (Match_State == RtMatchState::Aborted)
	{
		//	HandleMatchAborted();
	}
}

void ARtGameMode::StartMatch()
{
	if (HasMatchStarted())
	{
		// Already started
		return;
	}

	//Let the game session override the StartMatch function, in case it wants to wait for arbitration
	if (GameSession->HandleStartMatchRequest())
	{
		return;
	}

	Set_MatchState(RtMatchState::InProgress_StartTrng);
}
void ARtGameMode::ResultMatch()
{
	Set_MatchState(RtMatchState::InProgress_Result);
}

// dave
void ARtGameMode::ExitTraining()
{
	Set_MatchState(RtMatchState::LeavingMap);
}


bool ARtGameMode::Check_PreLoginIntegrity(const FRtUserInfo_PreLogin& InInfo)
{
	auto* mgr = GetGameManager();
	auto* trainingmgr = mgr->TrngManager;

	if (InInfo.PlayType == ERt_PlayType::E_Control) {
		return true;
	}
	else if (InInfo.PlayType == ERt_PlayType::E_Trainee) {
		bool bCheck = false;
		if (trainingmgr->Device_StatusArray.IsEmpty()) {
			UE_LOG(LogPlayerInfo, Error, TEXT("VR Device List is Empty!"));
		}
		for (auto& Device : trainingmgr->Device_StatusArray) {
			if (InInfo.DeviceName.Equals(Device.Name)) {
				
				return true;
				break;
			}
		}
	}
	return false;
}

bool ARtGameMode::Add_New_Player(ARtPlayerController* InPlayerController)
{
	auto* mgr = GetGameManager();
	auto* trainingmgr = mgr->TrngManager;

	auto* player_state = InPlayerController->Get_PlayerState();
	check(player_state);

	auto play_type = player_state->Play_Type;
	// res 
	bool res = false;
	if (play_type == ERt_PlayType::E_Control)
	{
		// Control is Only One
		if (Control_Controller != nullptr) {
			GameSession->KickPlayer(Control_Controller, FText::FromString(TEXT("Duplecate Cotrol Login")));
		}
		else {
			Control_Controller = InPlayerController;
			res = true;
		}
	}
	else if (play_type == ERt_PlayType::E_Trainee)
	{
		if (Trainee_Array.Contains(InPlayerController) == false) {
			Trainee_Array.Add(InPlayerController);

			int32 player_id = player_state->GetPlayerId();
			check(player_id);

			if (trainingmgr->IsAssigned_ToDevice(player_id)) {
				UE_LOG(LogRtServer, Log, TEXT("Already Exist player id %d"), player_id);
			}
			if (auto* found_device_info = trainingmgr->Get_DeviceStatus(player_state->Login_DeviceName)) {
				found_device_info->PlayerId = player_id;
				found_device_info->Active = true;
				// Set Device Info Fisrt
				player_state->Set_PlayerInfo(*found_device_info);//!
				res = true;
			}
			else {
				UE_LOG(LogRtServer, Log, TEXT("Already Exist player id"));
			}
		}
		else {
			UE_LOG(LogRtServer, Log, TEXT("Warning: Add duplicate!"));
		}
	}
	else {
		UE_LOG(LogRtServer, Warning, TEXT("Add_New_Player - Invalid PlayType!"));
	}
	return res;
}

bool ARtGameMode::Remove_Player(ARtPlayerController* InPlayerController)
{
	auto* mgr = GetGameManager();
	if (!mgr) {
		return false; 
	}

	auto* trainingmgr = mgr->TrngManager;

	auto* player_state = InPlayerController->Get_PlayerState();
	check(player_state);

	auto play_type = player_state->Play_Type;

	bool res = false;
	if (play_type == ERt_PlayType::E_Trainee)
	{
		if (Trainee_Array.Contains(InPlayerController)) {
			Trainee_Array.Remove(InPlayerController);

			int32 player_id = player_state->GetPlayerId();
			check(player_id);

			if (auto* found_info = trainingmgr->Device_StatusArray.FindByPredicate([player_id](const FRtDeviceInfo& InInfo) { return InInfo.PlayerId == player_id; })) {
				found_info->Logout();
				res = true;
			}
			else {
				UE_LOG(LogRtServer, Log, TEXT("non-existent member"));
			}
		}
		else {
			UE_LOG(LogRtServer, Log, TEXT("Warning: player that does not exist!"));
		}

	}
	else if (play_type == ERt_PlayType::E_Control) {
		if (Control_Controller == InPlayerController) {
			Control_Controller = nullptr;
			res = true;
		}
		else {
			UE_LOG(LogRtServer, Log, TEXT("Warning: player that does not exist!"));
		}
	}
	else {
		UE_LOG(LogRtServer, Warning, TEXT("Remove_Player - Invalid PlayType!"));
	}
	return res;
}

TArray<FRtDeviceInfo>& ARtGameMode::Get_DeviceArray()
{
	if (auto* mgr = GetGameManager()) {
		if (auto* trainingmgr = mgr->TrngManager)
			return trainingmgr->Device_StatusArray;
	}
	return FRtDeviceInfo::DefualtArray;
}

TArray<FRtTraineeInfo>& ARtGameMode::Get_TraineeArray()
{
	if (auto* mgr = GetGameManager()) {
		if (auto* trainingmgr = mgr->TrngManager)
			return trainingmgr->Trainee_StatusArray;
	}
	return FRtTraineeInfo::DefualtArray;
}

TArray<FRtDeviceInfo>& ARtGameMode::Get_DeviceArray_BeingTrained()
{
	if (auto* mgr = GetGameManager()) {
		if (auto* trainingmgr = mgr->TrngManager)
			return trainingmgr->DeviceArray_BeingTrained;
	}
	return FRtDeviceInfo::DefualtArray;
}

URtTrainingManager* ARtGameMode::Get_TrngManager()
{
	if (auto* mgr = GetGameManager()) {
		return mgr->TrngManager;
	}
	return nullptr;
}

URtReplayManager* ARtGameMode::Get_ReplayManager()
{
	if (UGameInstance* gi = GetGameInstance()) {
		return gi->GetSubsystem<URtReplayManager>();
	}
	return nullptr;
}

void ARtGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (auto* mgr = GetGameManager()) {
		auto* training = mgr->TrngManager;
		if (training) {
			training->Initialize_Manager();
		}
	}
}

void ARtGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

FString ARtGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	FString res = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	if (auto* PlayerController = Cast<ARtPlayerController>(NewPlayerController)) {
		if (auto* PlayerState = PlayerController->Get_PlayerState()) {
			PlayerState->Set_LoginInfo(Options, NewPlayerController->IsLocalController());
		}
	}
	return res;
}

void ARtGameMode::Handle_MatchIsWaitingToStart()
{
	if (GameSession != nullptr)
	{
		GameSession->HandleMatchIsWaitingToStart();
	}
	// Calls begin play on actors, unless we're about to transition to match start
	if (!ReadyToStartMatch())
	{
		GetWorldSettings()->NotifyBeginPlay();
	}
}

// 예전 리플레이 파일, 제거예정
void ARtGameMode::Handle_InProgress_Result_HasStarted()
{
	if (auto* gi = GetGameInstance())
	{
		URtWebManager* logsub = gi->GetSubsystem<URtWebManager>();
		ARtGameState* gs = Get_GameState();
		if (!gs) return;
		int32 TID = gs->Training_Index;
		int32 UID = 0;    // server
		FRtDeviceInfo currinfo;
		if (URtReplayManager* replay_mgr = Get_ReplayManager()) {
			//replay_mgr->Upload_File(FString::FromInt(TID), FString::FromInt(UID));
		}	
	}
}


void ARtGameMode::Set_MatchState(FName NewState)
{
	if (Match_State == NewState)
	{
		return;
	}

	UE_LOG(LogGameMode, Display, TEXT("Match State Changed from %s to %s"), *Match_State.ToString(), *NewState.ToString());
	Match_State = NewState;

	OnMatchStateSet();

	ARtGameState* FullGameState = GetGameState<ARtGameState>();
	if (FullGameState)
	{
		FullGameState->SetMatchState(NewState);
	}
}
