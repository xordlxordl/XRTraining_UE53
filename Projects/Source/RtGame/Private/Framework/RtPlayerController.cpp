// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtPlayerController.h"

// Unreal Basic 
#include "Net/UnrealNetwork.h"
#include "kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameplayTagsModule.h"
#include "NativeGameplayTags.h"
#include "EngineUtils.h"
#include "Components/PoseableMeshComponent.h"

// Framework
#include "Framework/RtSpectatorPawn.h"
#include "Framework/RtPlayerState.h"
#include "Framework/RtGameHUD.h"
#include "Framework/RtGameMode.h"
#include "Framework/RtGameState.h"
#include "Framework/RtTraineePawn.h"
#include "Framework/RtReplayTrainee.h"
#include "Framework/RtwidgetActor.h"

// Manager 
#include "Manager/RtGameManager.h"
#include "Manager/RtWebManager.h"
#include "Manager/RtTrainingManager.h"
#include "Manager/RtConfig.h"
#include "Manager/RtGameScenarioController.h"
#include "Manager/RtGameSceneManager.h"
#include "Manager/RtReplayManager.h"
#include "Manager/RtLoadingManager.h"
#include "Manager/RtVoiceManager.h"
#include "Manager/RtReplayManager.h"
#include "Manager/RtReplayManagerActor.h"

// Widget
#include "UI/Rt_PrimaryLayout.h"

// Json
#include "Serialization/JsonSerializer.h" //Json
#include "JsonObjectConverter.h" // JsonUtilities

// Data
#include "Common/RtGameplayTags.h"
#include "Common/RtRpcCommon.h"
#include "RtCore/Public/Common/RtCommon.h"
#include "RtReadWriteFile.h"

// Network
#include "Network/RtNetworkManager.h"

// Voice
#include "Components/AudioComponent.h"
// voice session
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

// GlTF Plugin
#include "glTFRuntimeFBX/Public/glTFRuntimeFBXAssetActor.h"
#include <UI\Trainee\Rt_Trainee_Main.h>



ARtPlayerController::FTwoParam_Delegate ARtPlayerController::Add_Player;
ARtPlayerController::FTwoParam_Delegate ARtPlayerController::Remove_Player;

ARtPlayerController::ARtPlayerController()
{
	ControlMode_PlayerTick = true;

	ControllerModeCamera_Location = FVector(0, 0, 0);
	ReplayModeCamera_Location = FVector(0, 0, 3000);
	ControllerModeCamera_Rotator = FRotator(FQuat::MakeFromEuler(FVector(0, -90, 0)));
	ControllerModeCamera_Location_Offset = FVector(0, 0, 0);
	bReplicates = true;
}

bool ARtPlayerController::Get_DeviceInfo(FRtDeviceInfo& OutInfo)
{
	if (auto* ps = Get_PlayerState())
	{
		OutInfo = ps->Get_PlayerInfo();
		return true;
	}
	return false;
}

const TArray<FRtDeviceInfo>& ARtPlayerController::Get_Accessible_PlayerDevices()
{
	return Accessible_PlayerDevices;
}

const TArray<FRtTraineeInfo>& ARtPlayerController::Get_Accessible_Trainees()
{
	return Accessible_Trainees;
}

TWeakObjectPtr<URt_PrimaryLayout> ARtPlayerController::Get_Primary_Layout()
{
	return TWeakObjectPtr<URt_PrimaryLayout>();
}

ERt_PlayType ARtPlayerController::Get_PlayType()
{
	if (auto* playState = Get_PlayerState())
	{
		return playState->Play_Type;
	}
	return ERt_PlayType::E_None;
}




void ARtPlayerController::Set_MyLoginInfo(const FRtUserInfo_PreLogin& info)
{
	MyLoginInfo = info;
}

void ARtPlayerController::Set_Spectator(bool val)
{
	if (auto* ps = GetPlayerState<ARtPlayerState>())
	{
		if (val)
		{
			ps->SetIsOnlyASpectator(val);
			ChangeState(NAME_Spectating);
		}
		else
		{
			ps->SetIsOnlyASpectator(val);
			ChangeState(NAME_Playing);
		}
	}
}


void ARtPlayerController::Init_PlayType()
{
	if (PlayerState)
	{
		if (auto* ps = Get_PlayerState())
		{
			ps->CallOrRegister_DeviceInfoChanged(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::Handle_Changed_DeviceInfo));
			ENetMode NetMode = GetNetMode();

			if (IsLocalPlayerController())
			{
				auto netmode = GetNetMode();
				if (netmode == ENetMode::NM_Standalone)
				{
					// Test
					if (ps->Play_Type == ERt_PlayType::E_Replay)
					{
						// Top view
						APawn* pawn = GetSpectatorPawn();
						if (pawn)
						{
							if (auto* camera = Get_ReplayMode_Camera())
							{
								ReplayModeCamera = camera;
								SetViewTarget(camera);
							}
						}
					}
				}
				else
				{
					switch (ps->Play_Type)
					{

					case ERt_PlayType::E_Trainee:
						Change_CharacterIcon(FText::FromString(ps->Login_DeviceName));

						// Voice Test Code
						// StartTalking();


						break;
					case ERt_PlayType::E_Server:
					{
						PlayerCameraManager->SetActorLocationAndRotation(FVector(0, 1000, 0), FQuat::MakeFromEuler(FVector(0, -90, 0)));
						if (GetPawn())
						{
							GetPawn()->SetActorLocationAndRotation(FVector(0, 1000, 0), FQuat::MakeFromEuler(FVector(0, -90, 0)));
						}
					}break;
					case ERt_PlayType::E_Control:
					{
						// 
						Server_Send_DeviceInfo(Get_Id());
						Server_Send_TraineeInfo(Get_Id());
					}break;
					case ERt_PlayType::E_Single:
						Change_CharacterIcon(FText::FromString(ps->Login_DeviceName));
						break;
					}
				}
			}
		}
	}
}

void ARtPlayerController::Init_RePlayType()
{
	// Set Params
	int32 TID;
	int32 UID;
	// Loading Start
	if (auto* gi = GetGameInstance())
	{

		if (auto* rmgr = gi->GetSubsystem<URtReplayManager>())
		{
			rmgr->InitManager();
			TID = rmgr->ReplayInfo.SelectedTID;
			UID = rmgr->ReplayInfo.SelectedUID;
			// Download Code
			URtWebManager* webmgr = Get_WebManager();
			if (webmgr)
			{
				webmgr->GetVoiceList(TID);
			}
		}

		if (auto* lm = gi->GetSubsystem<URtLoadingManager>())
		{
			lm->LoadLevelWithLoadingScreen("Lobby_Map", this);
			UE_LOG(LogTemp, Warning, TEXT("LoadLevelWithLoadingScreen from RtPlayerController::Init_PlayType"));
		}
	}
}


void ARtPlayerController::Log_Message(ELogProtocol protocol)
{
	if (auto* gi = GetGameInstance())
	{
		URtWebManager* logsub = gi->GetSubsystem<URtWebManager>();
		if (logsub->IsInitialized())
		{
			ARtGameState* gs = Get_GameState();
			if (!gs) return;
			int32 TID = gs->Training_Index;
			int32 UID = 0;
			FRtDeviceInfo currinfo;
			if (Get_DeviceInfo(currinfo))
			{
				UID = currinfo.PlayerInfo.UID;
			}
			logsub->SendLog(protocol, TID, UID);
		}
	}
}

void ARtPlayerController::Server_StartTraining_Implementation(int32 TID)
{

	if (ARtGameState* gs = Get_GameState())
	{
		gs->Training_Index = TID;
	}

	FString Check;
}
void ARtPlayerController::Exx_Send_Log_Message()
{
	Log_Message(ELogProtocol::LOG_FIRE);
}

void ARtPlayerController::Exx_Send_Packet_Ready()
{
	if (auto* gi = GetGameInstance())
	{
		if (auto* netmgr = gi->GetSubsystem<URtNetworkManager>())
		{

			int UID = Get_PlayerState()->Get_PlayerInfo().PlayerInfo.UID;

			netmgr->SendPacket_Ready(UID, true);
		}
	}
}


void ARtPlayerController::StartTraining(int32 TID)
{
	if (ARtGameState* gs = Get_GameState())
	{
		// 4-1. Update TID all Trainee
		gs->Training_Index = TID;
	}
	// 5. Send to Web TID, UID, MID , FString (Role)
	if (ARtPlayerState* ps = Get_PlayerState())
	{

		if (IsLocalController())
		{
			FRtDeviceInfo device_info = ps->Get_PlayerInfo();
			int32 UID = device_info.TraineeUID;
			int32 MID = device_info.DeviceIndex;
			FString Role_str = device_info.PlayerRole;

			auto* webmgr = Get_WebManager();
			if (webmgr->IsInitialized())
			{
				webmgr->EnterTrainee(TID, UID, MID, Role_str);
			}
		}
	}



	APP_LOG("ARtPlayerController::StartTraining ~ ");

	if (ARtGameScenarioController* gsc = ARtGameScenarioController::Get(GetWorld()))
	{
		gsc->SetGamePlay(true);
	}

	FString Check;

	// Create Speech Recognizer Actor
	//if (auto* character = Cast<ARtTraineePawn>(GetPawn()))
	//{
	//	character->CreateSpeechRecognizer();
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("RtTraineePawn is null"));
	//}
}


bool ARtPlayerController::IsAlreadyAssignedTrainee(const FRtTraineeInfo& InInfo)
{
	if (auto* it = Accessible_PlayerDevices.FindByPredicate([InInfo](auto& element) { return element.TraineeUID == InInfo.UID; }))
	{
		return true;
	}
	return false;
}

bool ARtPlayerController::IsAlreadyAssignedRole(const FRtRoleInfo& InInfo)
{
	if (auto* it = Accessible_PlayerDevices.FindByPredicate([InInfo](auto& element) { return element.PlayerRole == InInfo.TraineeRole; }))
	{
		return true;
	}
	return false;
}


void ARtPlayerController::Server_Confirm_TraineeAassignment_Implementation()
{
	if (bHasBeenAssigned) return;

	if (auto gm = Get_GameMode())
	{
		gm->Confirm_TraineeAassignment(Get_Id());
		bHasBeenAssigned = true;
	}
}

void ARtPlayerController::Server_Confirm_TraineeDeployment_Implementation()
{
	if (auto gm = Get_GameMode())
	{
		gm->Confirm_TraineeDeployment(Get_Id());
	}
}

void ARtPlayerController::Server_AssignTrainee_ToDevice_Implementation(const FString& InDeviceName, const FRtTraineeInfo& InInfo)
{
	if (auto gm = Get_GameMode())
	{
		gm->AssignTrainee_ToDevice(InDeviceName, InInfo, Get_Id());
	}
}


void ARtPlayerController::Server_AssignRole_ToDevice_Implementation(const FString& InDeviceName, const FRtRoleInfo& InInfo)
{
	if (auto gm = Get_GameMode())
	{
		gm->AssignRole_ToDevice(InDeviceName, InInfo, Get_Id());
	}
}

void ARtPlayerController::Server_Set_Scenario_Info_Implementation(const FRtScenarioInfo& Info)
{
	Set_ScenarioInfo(Info);
	if (auto* gs = Get_GameState())
	{
		gs->Set_Scenario_Info(Info);
	}

#if WITH_EDITOR
	FString playType = TEXT("");
	if (ARtPlayerController* controller = Cast<ARtPlayerController>(GetWorld()->GetFirstPlayerController())) {
		ARtPlayerState* playerState = controller->Get_PlayerState();
		if (playerState) {
			playType = URtResourceModelSubsystem::GetEnumToString(TEXT("ERt_PlayType"), (int32)playerState->Play_Type);
		}
	}

	APP_LOG("ARtPlayerController::Server_Set_Scenario_Info_Implementation : %s", *playType);
#endif
}

void ARtPlayerController::Server_RequestRandomNum_Implementation(const FString& variableName, const int32& lastNum)
{
	if (lastNum < 1)
	{
		return;
	}

	if (auto* GameState = GetWorld()->GetGameState<ARtGameState>())
	{
		int32 randomNumber = FMath::RandRange(1, lastNum);

		// 서버에서 정한 랜덤 값을 모든 클라이언트에게 전달한다.
		GameState->ScenarioVariableSync.Key = variableName;
		GameState->ScenarioVariableSync.Value = randomNumber;

		if (ARtGameScenarioController* ScenarioController = ARtGameScenarioController::Get(GetWorld()))
		{
			// 자신(서버)은 직접 갱신해 줘야 한다.
			ScenarioController->ScenarioModel->SetVariableValue(variableName, randomNumber);
		}

#if WITH_EDITOR
		ENetMode netMode = GetNetMode();
		FString playType = netMode == ENetMode::NM_ListenServer ? TEXT("Server") : TEXT("Not Server");
		APP_LOG("2. NetworkTest (Client -> [Server]): Server_RequestRandomNum_Implementation => %s", *playType);
#endif

	}
}

void ARtPlayerController::Client_Confirm_TraineeAassignment_Implementation()
{
	// Loading Start
	if (auto* gi = GetGameInstance())
	{
		if (auto* lm = gi->GetSubsystem<URtLoadingManager>())
		{
			lm->LoadLevelWithLoadingScreen("Lobby_Map", this);
			UE_LOG(LogTemp, Warning, TEXT("LoadLevelWithLoadingScreen from RtPlayerController::Client_Confirm_TraineeAassignment_Implementation"));
		}
	}
	if (IsLocalPlayerController())
	{
		bool bLoadMap = false;

		if (auto* player_state = Get_PlayerState())
		{

			if (player_state->Play_Type == ERt_PlayType::E_Control)
			{

				if (auto* gs = Get_GameState())
				{
					FRtScenarioInfo si = gs->Get_Scenario_Info();
				}

				// Top view
				APawn* pawn = GetSpectatorPawn();
				if (pawn)
				{
					if (auto* camera = Get_ControllMode_ViewCamera())
					{
						ControllerModeCamera = camera;
						ControllerModeCamera_Location = ControllerModeCamera->GetActorLocation();
						SetViewTarget(camera);

						/*
						// dave - test 
						bAutoManageActiveCameraTarget = true;

						USceneCaptureComponent2D* CaptureComponent = ControllerModeCamera->FindComponentByClass<USceneCaptureComponent2D>();
						if (CaptureComponent)
						{
							CaptureComponent->bCaptureEveryFrame = true;
							CaptureComponent->bCaptureOnMovement = true;
							CaptureComponent->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
						}
						*/
					}

					if (auto* follow_camera = Get_ControllMode_FollowCamera())
					{
						ControllerModeFollowCamera = follow_camera;
					}
				}

				bLoadMap = true;

			}
			else if (player_state->Play_Type == ERt_PlayType::E_Server)
			{
			}
			else if (player_state->Play_Type == ERt_PlayType::E_Trainee)
			{
				// PacketSend
				if (auto* ps = Get_PlayerState())
				{
					FString RoleID = ps->Get_PlayerInfo().PlayerRoleID;
					if (auto* gi = GetGameInstance())
					{
						if (auto* netmgr = gi->GetSubsystem<URtNetworkManager>())
						{
							netmgr->SendPacket_Login(RoleID, enRole::ROLE_USER);
						}
					}
				}

				bLoadMap = true;
			}
		}

		if (bLoadMap)
		{
			LoadSct();
		}
	}
	Client_Confirm_TraineeAassignment_Delegate.Broadcast(this, 0);
}

void ARtPlayerController::Client_Confirm_TraineeDeployment_Implementation()
{
	// Start Sign
	Client_Confirm_TraineeDeployment_Delegate.Broadcast(this, 0);
}

void ARtPlayerController::Server_Send_DeviceInfo_Implementation(int32 InId)
{
	if (InId == 0)
		return;

	if (auto* game_mode = Get_GameMode())
	{
		game_mode->SC_DeviceInfo(InId);
	}
}

void ARtPlayerController::Server_Send_TraineeInfo_Implementation(int32 InId)
{
	if (InId == 0)
		return;

	if (auto* game_mode = Get_GameMode())
	{
		game_mode->SC_TraineeInfo(InId);
	}


}




void ARtPlayerController::Client_Update_DeviceInfo_Implementation(const TArray<FRtDeviceInfo>& InInfo)
{
	Set_DeviceInfo(InInfo);
}

void ARtPlayerController::Client_Update_TraineeInfo_Implementation(const TArray<FRtTraineeInfo>& InInfo)
{
	Set_TraineeInfo(InInfo);
}

void ARtPlayerController::Client_Set_Scenario_Info_Implementation(const FRtScenarioInfo& Info)
{
	Set_ScenarioInfo(Info);
}

void ARtPlayerController::Set_DeviceInfo(const TArray<FRtDeviceInfo>& InInfo)
{
	Accessible_PlayerDevices = InInfo;
	Update_DeviceList_Delegate.Broadcast(this, InInfo);
}

void ARtPlayerController::Set_TraineeInfo(const TArray<FRtTraineeInfo>& InInfo)
{
	Accessible_Trainees = InInfo;
	Update_TraineeList_Delegate.Broadcast(this, InInfo);
}

void ARtPlayerController::Set_ScenarioInfo(const FRtScenarioInfo& InInfo)
{
	Current_Scenario = InInfo;
	if (auto* mgr = Get_GameManager())
	{
		if (auto* tmgr = mgr->TrngManager)
		{
			tmgr->Scenario_Info = InInfo;
			FRtRoleInfoList rolearray = tmgr->Whole_Role_Container.FindRef(InInfo.SCTFileName);
			TArray<FRtRoleInfo> info = rolearray.RoleList;
			tmgr->Update_RoleArray(info);

			FRtSuddenEvtList evtarray = tmgr->Whold_SEvt_Container.FindRef(InInfo.SCTFileName);
			TArray<FString> evts = evtarray.EvtList;
			tmgr->Update_SuddenEvtArray(evts);
		}
	}
}


int32 ARtPlayerController::Get_Id()
{
	if (auto* ps = Get_PlayerState())
	{
		return ps->GetPlayerId();
	}
	return 0;
}

URtGameManager* ARtPlayerController::Get_GameManager()
{
	if (auto* gi = GetGameInstance())
		return gi->GetSubsystem<URtGameManager>();

	return nullptr;
}

URtNetworkManager* ARtPlayerController::Get_NetManager()
{
	if (auto* gi = GetGameInstance())
		return gi->GetSubsystem<URtNetworkManager>();

	return nullptr;
}

URtWebManager* ARtPlayerController::Get_WebManager()
{
	if (auto* gi = GetGameInstance())
		return gi->GetSubsystem<URtWebManager>();
	return nullptr;
}

ARtPlayerState* ARtPlayerController::Get_PlayerState()
{
	return Cast<ARtPlayerState>(PlayerState);
}

ARtGameState* ARtPlayerController::Get_GameState()
{
	if (auto* world = GetWorld())
	{
		auto* gs = Cast<ARtGameState>(world->GetGameState());
		return gs;
	}
	return nullptr;
}

ARtGameHUD* ARtPlayerController::Get_HUD()
{
	auto* res = Cast<ARtGameHUD>(GetHUD());
	return res;
}

ARtGameMode* ARtPlayerController::Get_GameMode()
{
	UWorld* World = GetWorld();
	check(World);
	return Cast<ARtGameMode>(World->GetAuthGameMode());
}

void ARtPlayerController::HasStarted_MatchState(const FName& InState)
{
	// PrintOut on the EditorScreen
	//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Red, InState.ToString());
}

// dave
// TODO : release resources
void ARtPlayerController::HasExited_MatchState(const FName& InState)
{
	if (PlayerState) {
		if (IsLocalPlayerController()) 
		{
			/*
			ARtGameSceneManager* gameSceneManager = ARtGameSceneManager::Get(GetWorld());
			gameSceneManager->ClearReplay();
			gameSceneManager->DestroyAllSpawnedObjects();

			if (ARtGameScenarioController* ScenarioController = ARtGameScenarioController::Get(GetWorld()))
			{
				ScenarioController->UnloadSct();
				ScenarioController->Init(ERt_PlayType::E_None, TEXT(""));
			}

			if (ARtGameScenarioController* gameScenarioController = ARtGameScenarioController::Get(GetWorld()))
			{
				gameScenarioController->UnloadSct();
				gameScenarioController->Init(ERt_PlayType::E_None, TEXT(""));
			}

			if (ARtGameSceneManager* gameSceneManager = ARtGameSceneManager::Get(GetWorld()))
			{
				gameSceneManager->ClearReplay();
				gameSceneManager->DestroyAllSpawnedObjects();

			}

			if (ARtGameState* gameState = Get_GameState())
			{
				FRtScenarioInfo emptyInfo;
				gameState->Set_Scenario_Info(emptyInfo);
			}

			if (ARtPlayerState* playerState = Get_PlayerState())
			{
				playerState->Play_Type = ERt_PlayType::E_None;
			}
			*/
			if (auto* ps = Get_PlayerState()) 
			{
				switch (ps->Play_Type)
				{
				case ERt_PlayType::E_Trainee:
				{

				}
					break;
				case ERt_PlayType::E_Server:
				{
				}
					break;
				case ERt_PlayType::E_Control:
				{
					Open_Page("ControlSelectMap");
				}
					break;
				case ERt_PlayType::E_Single:
					break;
				}
			}

			GEngine->ForceGarbageCollection(true);
		}
	}
}

void ARtPlayerController::Change_CharacterIcon(const FText& InText)
{
	if (auto* character = Cast<ARtTraineePawn>(GetPawn()))
	{
		character->Change_IconName(InText);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Character is null"));
	}
}

void ARtPlayerController::Change_TraineeSelectionView(AActor* InActor)
{
	ControllerModeFollowCamera_Target = InActor;
}

bool ARtPlayerController::Change_TraineeSelectionView(const FString& InStr)
{
	if (auto* trainee = Find_Trainee(InStr))
	{
		Change_TraineeSelectionView(trainee);
		return true;
	}
	return false;
}

ARtTraineePawn* ARtPlayerController::Find_Trainee(const FString& InStr)
{
	if (UWorld* world = GetWorld())
	{
		for (TActorIterator<ARtTraineePawn> it(world); it; ++it)
		{
			ARtTraineePawn* traineepawn = *it;
			if (auto* ps = traineepawn->Get_PlayerState())
			{
				auto& device_info = ps->Get_PlayerInfo();
				if (device_info.Name.Equals(InStr, ESearchCase::IgnoreCase))
				{
					return traineepawn;
				}
			}
		}
	}
	return nullptr;
}

void ARtPlayerController::SetReplayTrainee(const int32 uid)
{
	if (auto* ReplayManager = Get_ReplayManager())
	{
		auto* ReplayTrainee = ReplayManager->FindRepTrainee(uid);
		if (ReplayTrainee)
		{
			ReplayManager->ClearTopViewCameraCaptureTextureTarget();
			ReplayManager->ClearAllCameraCaptureTextureTarget();
			ReplayTrainee->SetFirstPersonView();
			ReplayModeFollowCamera_Target = ReplayTrainee;
			//ReplayMode_PlayerTick = true;
		}
	}
}



URt_PageBase* ARtPlayerController::Open_Popup(const FName& InName)
{
	if (auto* hud = Get_HUD())
	{
		return hud->Open_Page(InName, true);
	}
	else
	{
		TWeakObjectPtr<URt_PrimaryLayout> primary_layout = Get_Primary_Layout();
		if (primary_layout.IsValid())
		{
			primary_layout.Get()->Open_Page(InName, true, this, nullptr);
		}
	}
	return nullptr;
}

URt_PageBase* ARtPlayerController::Open_Page(const FName& InName)
{
	if (auto* hud = Get_HUD())
	{
		return hud->Open_Page(InName, false);
	}
	else
	{
		TWeakObjectPtr<URt_PrimaryLayout> primary_layout = Get_Primary_Layout();
		if (primary_layout.IsValid())
		{
			primary_layout.Get()->Open_Page(InName, false, this, nullptr);
		}
	}
	return nullptr;
}

bool ARtPlayerController::Close_Popup(URt_PageBase* InPage)
{
	if (auto* hud = Get_HUD())
	{
		return hud->Close_Popup(InPage);
	}
	else
	{
		TWeakObjectPtr<URt_PrimaryLayout> primary_layout = Get_Primary_Layout();
		if (primary_layout.IsValid())
		{
			primary_layout.Get()->Close_Popup(InPage);
		}
	}
	return false;
}

FRtScenarioInfo ARtPlayerController::ReadScenarioFile(FString filename)
{
	// Make Path
	FString Saved_sctPath;
#if WITH_EDITOR
	Saved_sctPath = FPaths::ProjectDir();
#else
	Saved_sctPath = FPaths::LaunchDir();
#endif

	//Saved_sctPath = FPaths::ProjectContentDir();
	Saved_sctPath = FPaths::Combine(Saved_sctPath, TEXT("TrainingData/MapFiles/"));

	Saved_sctPath += filename;

	FPaths::MakeStandardFilename(Saved_sctPath);

	//
	FRtScenarioInfo newInfo;


	// ReadJSON
	//Check if the file exists
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*Saved_sctPath))
	{
		UE_LOG(LogTemp, Log, TEXT("Read String From File Failed - File doesn't exist %s"), *Saved_sctPath);
		return FRtScenarioInfo();;
	}

	FString RetString = "";

	// Try to read the file. Output goes in RetString
	if (!FFileHelper::LoadFileToString(RetString, *Saved_sctPath))
	{
		UE_LOG(LogTemp, Log, TEXT("Read String From File Failed - Was not able to read file. Is this a text file? - '%s'"), *Saved_sctPath);
		return FRtScenarioInfo();
	}

	UE_LOG(LogTemp, Log, TEXT("Read String From File Succeeded - '%s'"), *Saved_sctPath);

	TSharedPtr<FJsonObject> JsonObject;
	FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(RetString), JsonObject);
	if (nullptr != JsonObject)
	{
		UE_LOG(LogTemp, Log, TEXT("Read Json Succeeded - '%s'"), *Saved_sctPath);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Read Json Failed - Was not able to deserialize the json string. Is it the right format? - '%s'"), *Saved_sctPath);
		return FRtScenarioInfo();
	}


	// Move Data to R 

	newInfo.Name = JsonObject->GetStringField("scenarioName");
	newInfo.FileName = JsonObject->GetStringField("mctFileName");
	newInfo.PlayerNum = JsonObject->GetNumberField("playerCount");
	newInfo.Destination = JsonObject->GetStringField("scenarioGoal");
	newInfo.Summary = JsonObject->GetStringField("scenarioSummary");
	newInfo.TimeLimit = JsonObject->GetNumberField("trainingTime");
	newInfo.SCTFileName = filename;

	TArray<TSharedPtr<FJsonValue>> GroupList;
	GroupList = JsonObject->GetArrayField("playerGroupList");
	// Read SuddenEvent
	TArray<TSharedPtr<FJsonValue>> SuddenEvtList;
	SuddenEvtList = JsonObject->GetArrayField("suddenEventList");


	TArray<FRtRoleInfo> RoleArray;
	TArray<FString> SuddenEvtArray;
	for (int32 i = 0; i < GroupList.Num(); i++)
	{
		TSharedPtr<FJsonValue> ResultObject;
		TSharedPtr<FJsonObject> JsonValueObject = GroupList[i]->AsObject();

		TArray<TSharedPtr<FJsonValue>>  playerList = JsonValueObject->GetArrayField(TEXT("playerList"));
		FString playerType = JsonValueObject->GetStringField(TEXT("playerType"));
		FString playerGroupID = JsonValueObject->GetStringField(TEXT("playerGroupId"));
		if (playerType != "EPC") continue;


		if (playerList.Num() > 0)
		{

			for (int32 j = 0; j < playerList.Num(); j++)
			{
				TSharedPtr<FJsonValue> ListResultObject;
				TSharedPtr<FJsonObject> JsonValueObject_List = playerList[j]->AsObject();

				FString playerRoleID = JsonValueObject_List->GetStringField(TEXT("playerObjectId"));
				FString playerName = JsonValueObject_List->GetStringField(TEXT("playerName"));


				FRtRoleInfo newRole;
				newRole.TraineeRole = playerName;
				newRole.RoleID = playerRoleID;
				newRole.RID = j + 1;
				RoleArray.Add(newRole);
			}

		}

	}

	for (int32 j = 0; j < SuddenEvtList.Num(); j++)
	{
		TSharedPtr<FJsonValue> ResultObject;
		TSharedPtr<FJsonObject> JsonValueObject = SuddenEvtList[j]->AsObject();

		FString SuddenEvtID = JsonValueObject->GetStringField(TEXT("eventNameTId"));
		SuddenEvtArray.Add(SuddenEvtID);
	}

	FRtRoleInfoList roleList;
	roleList.RoleList = RoleArray;
	FRtSuddenEvtList evtList;
	evtList.EvtList = SuddenEvtArray;
	// Scenario Info Toss
	// Sudden Evt Toss
	if (auto* mgr = Get_GameManager())
	{
		if (auto* tmgr = mgr->TrngManager)
		{
			tmgr->Whole_Role_Container.Add(filename, roleList);
			tmgr->Whold_SEvt_Container.Add(filename, evtList);
		}
	}

	// 제거예정
		//FRtSuddenEvtList evtList;
		//evtList.EvtList = SuddenEvtArray;
		// Sudden Evt Toss
		//if (auto* mgr = Get_GameManager()) {
		//	if (auto* tmgr = mgr->TrngManager) {
		//		tmgr->Whold_SEvt_Container.Add(filename, evtList);
		//		// Update Sudden Evt
		//		tmgr->Update_SuddenEvtArray(SuddenEvtArray);
		//	}
		//}
		//Server_Set_Scenario_Info(newInfo);
		//if (auto* mgr = Get_GameManager()) {
		//	if (auto* tmgr = mgr->TrngManager) {
		//		tmgr->Scenario_Info = newInfo;
		//		tmgr->Update_RoleArray(RoleArray);
		//	}
		//}


	return newInfo;
}

void ARtPlayerController::Handle_Changed_DeviceInfo()
{
	if (auto* ps = Get_PlayerState())
	{
		OnChanged_DeviceInfo_delegate.Broadcast(ps->Get_PlayerInfo(), ps);
	}
}

void ARtPlayerController::CallBeforeBindingToPlayerState()
{
	Handle_Changed_DeviceInfo();
}

void ARtPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (Get_PlayType() == ERt_PlayType::E_Replay)
	{
		Init_RePlayType();
	}
}

void ARtPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (auto* player_state = Get_PlayerState())
	{
		if (player_state->Play_Type == ERt_PlayType::E_Control)
		{
			ARtGameState::Add_PlayerState.RemoveAll(this);
			ARtGameState::Remove_PlayerState.RemoveAll(this);
		}
	}
}

// dave - debug
void ARtPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

#if !UE_BUILD_SHIPPING
#if WITH_EDITOR
	if (auto* player_state = Get_PlayerState())
	{
		const UEnum* state = FindFirstObjectSafe<UEnum>(TEXT("ERt_PlayType"));
		
		if (IsValid(state)) 
		{
			FString str = state->GetDisplayNameTextByIndex(((int32)player_state->Play_Type)).ToString();
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("* Controller possessed : (%s)"), *str));
		}
	}
#endif
#endif
}

// dave - debug
void ARtPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

#if !UE_BUILD_SHIPPING
#if WITH_EDITOR
	if (auto* player_state = Get_PlayerState())
	{
		const UEnum* state = FindFirstObjectSafe<UEnum>(TEXT("ERt_PlayType"));
		if (IsValid(state))
		{
			FString str = state->GetDisplayNameTextByIndex(((int32)player_state->Play_Type)).ToString();
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("* Controller unpossessed : (%s)"), *str));
		}
	}
#endif
#endif
}

// dave - debug
void ARtPlayerController::PawnLeavingGame() 
{
	// dave 
#if !UE_BUILD_SHIPPING
#if WITH_EDITOR
	if (auto* player_state = Get_PlayerState())
	{
		const UEnum* state = FindFirstObjectSafe<UEnum>(TEXT("ERt_PlayType"));
		if (IsValid(state))
		{
			FString str = state->GetDisplayNameTextByIndex(((int32)player_state->Play_Type)).ToString();
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("* PawnLeavingGame : (%s)"), *str));
		}
	}
#endif
#endif
	Super::PawnLeavingGame();
}

void ARtPlayerController::Destroyed()
{
	Super::Destroyed();

	// dave 
#if !UE_BUILD_SHIPPING
#if WITH_EDITOR
	if (auto* player_state = Get_PlayerState())
	{
		const UEnum* state = FindFirstObjectSafe<UEnum>(TEXT("ERt_PlayType"));
		if (IsValid(state))
		{
			FString str = state->GetDisplayNameTextByIndex(((int32)player_state->Play_Type)).ToString();
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("* ARtPlayerController Destroyed : (%s)"), *str));
		}
	}
#endif
#endif
}

void ARtPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (PlayerState)
	{
		if (IsLocalController())
		{
			Init_PlayType();
			// Call and Register_OnPlayerStateSet
			if (auto* mgr = Get_GameManager())
			{

				if (auto ModeDefinition = mgr->ModeDefinition.Get())
				{
					auto play_type = mgr->Get_Play_Type();

					auto mode = ModeDefinition->Get_Mode(play_type);
				}
			}

		}
	}
}

void ARtPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (ControlMode_PlayerTick)
	{
		if (ControllerModeFollowCamera.Get() && ControllerModeFollowCamera_Target.Get())
		{
			if (auto* TraineePawn = Cast<ARtTraineePawn>(ControllerModeFollowCamera_Target))
			{
				if (USkeletalMeshSocket const* socket = TraineePawn->SkeletalMesh->GetSocketByName("eyeSocket"))
				{
					FTransform trans = TraineePawn->SkeletalMesh->GetSocketTransform("eyeSocket");
					ControllerModeFollowCamera.Get()->SetActorTransform(trans);
				}
			}
		}
	}

	if (ReplayMode_PlayerTick)
	{
		if (ReplayModeCamera.Get())
		{
			if (auto* ReplayPawn = Cast<ARtReplayTrainee>(ReplayModeFollowCamera_Target))
			{
				if (USkeletalMeshSocket const* socket = ReplayPawn->SkeletalMesh->GetSocketByName("eyeSocket"))
				{
					FTransform trans = ReplayPawn->SkeletalMesh->GetSocketTransform("eyeSocket");
					ReplayModeCamera.Get()->SetActorTransform(trans);
				}
			}
		}
	}

}

void ARtPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (IsLocalController())
	{
		if (PlayerState)
		{
			Init_PlayType();
		}

		//dave
		URtLoadingManager::Update_OnLoadingFinished_Delegate.RemoveAll(this); 
		URtLoadingManager::Update_OnLoadingFinished_Delegate.AddUObject(this, &ARtPlayerController::OnLoadFinished);
	}
}


void ARtPlayerController::Update_ControlModeCamera(const FVector2D& InOffset2)
{
	if (Get_PlayType() == ERt_PlayType::E_Control)
	{
		if (ControllerModeCamera.Get())
		{
			if (auto component = ControllerModeCamera->GetCameraComponent())
			{
				int32 ViewportSizeX, ViewportSizeY;
				GetViewportSize(ViewportSizeX, ViewportSizeY);
				float scale = (float)ViewportSizeY / (float)ViewportSizeX;

				//ControllerModeCamera_Location_Offset.X = -InOffset2.X * component->OrthoWidth * 0.5f;
				//ControllerModeCamera_Location_Offset.Y = InOffset2.Y * component->OrthoWidth * 0.5f;

				ControllerModeCamera_Location_Offset.X = 0;
				ControllerModeCamera_Location_Offset.Y = (1.f - InOffset2.X) * (float)component->OrthoWidth * 0.5f; // basic 0.5
				ControllerModeCamera->SetActorLocation(ControllerModeCamera_Location + ControllerModeCamera_Location_Offset);
			}
		}
	}
}

ACameraActor* ARtPlayerController::Get_ControllMode_ViewCamera()
{
	ACameraActor* camera = nullptr;
	TArray<AActor*> OutActor;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraActor::StaticClass(), RtPlayTags::RtControlMode_Actor_TopCamera, OutActor);

	for (auto* Actor : OutActor)
	{
		camera = Cast<ACameraActor>(Actor);
		if (camera)
			return camera;
	}

	/*
	FActorSpawnParameters spawn_param;
	spawn_param.Name = FName(TEXT("Controller_TopCamera"));
	camera = GetWorld()->SpawnActor<ACameraActor>(spawn_param);
	camera->SetActorLocationAndRotation(ControllerModeCamera_Location, ControllerModeCamera_Rotator);
	camera->Tags.Add(RtPlayTags::RtControlMode_Actor_TopCamera);
	auto* camera_component = camera->GetCameraComponent();
	camera_component->ProjectionMode = ECameraProjectionMode::Orthographic;
	camera_component->OrthoWidth = 3000;
	*/

	// dave 
	FSoftClassPath path(TEXT("/Game/RS/Control/Camera/Control_TopCamera.Control_TopCamera_C"));
	UClass* ActorCloass = path.TryLoadClass<ACameraActor>();
	checkf(ActorCloass != nullptr, TEXT("expected non-null class"));

	if (IsValid(ActorCloass))
	{
		FVector SpawnPos = FVector(0, 0, 1500);
		FRotator SpawnRot = FRotator(-90.f, 0.f, -180.f); 
		//FTransform SpawnTransform(SpawnRot, SpawnPos);

		FActorSpawnParameters SpawnParams;
		SpawnParams.Name = FName("Controller_TopCamera");
	
		//SpawnParams.bDeferConstruction = true;
		//SpawnParams.bNoFail = true;
		//SpawnParams.Instigator = GetInstigator();
		//SpawnParams.Owner = this;  
		//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		camera = Cast<ACameraActor>(GetWorld()->SpawnActor(ActorCloass, &SpawnPos, &SpawnRot, SpawnParams));
		if (camera)
		{
			//camera->Tags.Add(RtPlayTags::RtControlMode_Actor_TopCamera);
			//camera->FinishSpawning(SpawnTransform);

			//auto* camera_component = camera->GetCameraComponent();
			//camera_component->ProjectionMode = ECameraProjectionMode::Orthographic;
			//camera_component->OrthoWidth = 3000;
			return camera;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Failed to create controllmode follow camera actor"));
		}
	}
	return nullptr;
}

ACameraActor* ARtPlayerController::Get_ControllMode_FollowCamera()
{
	ACameraActor* camera = nullptr;
	TArray<AActor*> OutActor;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraActor::StaticClass(), RtPlayTags::RtControlMode_Actor_FollowCamera, OutActor);

	for (auto* Actor : OutActor)
	{
		camera = Cast<ACameraActor>(Actor);
		if (camera)
			return camera;
	}

	FSoftClassPath path(RtBlueprintAsset::ControlMode_FollowCamera);
	UClass* actor_class = path.TryLoadClass<ACameraActor>();
	checkf(actor_class != nullptr, TEXT("expected non-null class"));

	if (actor_class)
	{
		FVector SpawnPos = FVector(0, 0, 100);
		camera = Cast<ACameraActor>(GetWorld()->SpawnActor(actor_class, &SpawnPos));
		if (camera)
		{
			return camera;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Failed to create controllmode follow camera actor"));
		}
	}
	return nullptr;
}

ACameraActor* ARtPlayerController::Get_ReplayMode_Camera()
{
	ACameraActor* camera = nullptr;
	TArray<AActor*> OutActor;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraActor::StaticClass(), RtPlayTags::RtReplayMode_Actor_Camera, OutActor);

	for (auto* Actor : OutActor)
	{
		camera = Cast<ACameraActor>(Actor);
		if (camera)
			return camera;
	}

	FActorSpawnParameters spawn_param;
	spawn_param.Name = FName(TEXT("Replay_Camera"));
	camera = GetWorld()->SpawnActor<ACameraActor>(spawn_param);
	camera->SetActorLocationAndRotation(ReplayModeCamera_Location, ControllerModeCamera_Rotator);
	camera->Tags.Add(RtPlayTags::RtReplayMode_Actor_Camera);
	auto* camera_component = camera->GetCameraComponent();
	camera_component->ProjectionMode = ECameraProjectionMode::Perspective;

	return camera;
}

void ARtPlayerController::LoadSct()
{
	ARtGameScenarioController* gameScenarioController = ARtGameScenarioController::Get(GetWorld());
	if (gameScenarioController)
	{
		ARtGameSceneManager* gameSceneManager = ARtGameSceneManager::Get(GetWorld());
		if (gameSceneManager)
		{
			if (auto* gameState = Get_GameState())
			{
				if (auto* playerState = Get_PlayerState())
				{
					gameScenarioController->Init(playerState->Play_Type, gameState->Get_Scenario_Info().SCTFileName);

					URtResourceModelSubsystem::Get()->UpdateTrainingDataPath(playerState->Play_Type != ERt_PlayType::E_Single);

					if (playerState->Play_Type != ERt_PlayType::E_Single)
					{
						gameSceneManager->LoadSimpleProjectScenarioScene(
							CLIENT_PROJECT_DATA_PATH,
							CLIENT_PROJECT_SYSTEM_DATA_PATH,
							gameScenarioController->SctFileName,
							FRotator::ZeroRotator, FVector::ZeroVector
						);
					}
					else // SingleMode
					{
						FString projFilePath = URtResourceModelSubsystem::Get()->GetTrainingDataPath();
						FString sctFileName = URtResourceModelSubsystem::Get()->GetSingleSctFileName();

						projFilePath += TEXT("/");

						if (ARtPlayerController* controller = Cast<ARtPlayerController>(GetWorld()->GetFirstPlayerController()))
						{
							FRtDeviceInfo deviceInfo = controller->Get_PlayerState()->Get_PlayerInfo();
							deviceInfo.PlayerRoleID = URtResourceModelSubsystem::Get()->GetSingleRoleID();
							controller->Get_PlayerState()->Set_PlayerInfo(deviceInfo);
						}

						FString tctFilesPath = projFilePath + ARtSceneManagerBase::TctSubFolderName;
						FString systemPath;
						FString scenarioFilePath = projFilePath + ARtSceneManagerBase::MapSubFolderName + TEXT("/") + sctFileName;
						FRotator totalRotOffset = FRotator::ZeroRotator;
						FVector totalPosOffset = FVector::ZeroVector;

						gameSceneManager->LoadScenarioScene(tctFilesPath, systemPath, scenarioFilePath, false, totalRotOffset, totalPosOffset);
					}

					// 모든 파트에서 캐릭터 모델링 동기화.
					if (playerState->Play_Type == ERt_PlayType::E_Server ||
						playerState->Play_Type == ERt_PlayType::E_Control ||
						playerState->Play_Type == ERt_PlayType::E_Trainee ||
						playerState->Play_Type == ERt_PlayType::E_Single)
					{
						FRtDeviceInfo deviceInfo = playerState->Get_PlayerInfo();

						if (ARtGameScenarioController* gsc = ARtGameScenarioController::Get(GetWorld()))
						{
							if (playerState->Play_Type == ERt_PlayType::E_Trainee || // 내가 훈련자이면..
								playerState->Play_Type == ERt_PlayType::E_Single) 
							{
								gsc->SetCurrentPlayerID(playerState->Get_PlayerInfo().PlayerRoleID);
							}

							TArray<AActor*> foundActors;
							UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARtTraineePawn::StaticClass(), foundActors);

							for (auto& actor : foundActors)
							{
								ARtTraineePawn* pawn = Cast<ARtTraineePawn>(actor);
								if (pawn && pawn->SkeletalMesh)
								{
									ARtPlayerState* otherPlayerState = Cast<ARtPlayerState>(pawn->GetPlayerState());
									if (otherPlayerState)
									{
										FRtDeviceInfo otherDeviceInfo = otherPlayerState->Get_PlayerInfo();

										ARtGameSceneManager::Get(GetWorld())->AddTraineeMap(otherDeviceInfo.PlayerRoleID, pawn);

										USkeletalMesh* skeletalMesh = nullptr;
										FString modelPath = gsc->GetModelPathByObjectID(otherDeviceInfo.PlayerRoleID);
										if (!modelPath.IsEmpty())
										{
											// Load Lagacy SkeletalMesh Asset
											skeletalMesh = URtReadWriteFile::LoadSkeletalFromPath(modelPath);
										}
										if (nullptr != skeletalMesh)
										{
											// Load Lagacy SkeletalMesh Asset
											pawn->IsPoseableMeshPose = false;
											bool isBoneHidden = pawn->SkeletalMesh.Get()->IsBoneHidden(pawn->SkeletalMesh.Get()->GetBoneIndex(FName("neck_01")));
											pawn->SkeletalMesh.Get()->SetSkeletalMesh(skeletalMesh);
											pawn->SetVisibleHead(!isBoneHidden);

											if (playerState->Play_Type == ERt_PlayType::E_Trainee || playerState->Play_Type == ERt_PlayType::E_Single)
											{
												gsc->SetCurrentPlayerID(otherDeviceInfo.PlayerRoleID);
											}
										}
										else
										{
											// Load GlTF SkeletalMesh Asset;
											if (ARtObjectActor* objActor = gameSceneManager->GetPlayerMeshActor(otherDeviceInfo.PlayerRoleID))
											{
												AglTFRuntimeFBXAssetActor* glTFActor = objActor->CurrentGltfFBXAssetActor;
												if (glTFActor && !glTFActor->DiscoveredSkeletalMeshes.IsEmpty())
													skeletalMesh = glTFActor->DiscoveredSkeletalMeshes[0].Key->GetSkeletalMeshAsset();

												if (nullptr != skeletalMesh)
												{
													pawn->IsPoseableMeshPose = true;
													bool isBoneHidden = pawn->SkeletalMesh.Get()->IsBoneHidden(pawn->SkeletalMesh.Get()->GetBoneIndex(FName("neck_01")));

													//pawn->SkeletalMesh->SetSkeletalMeshAsset(skeletalMesh);

													if (pawn->PoseableMesh)
													{
														pawn->PoseableMesh.Get()->SetSkinnedAssetAndUpdate(skeletalMesh);
														pawn->SetVisibleHead(!isBoneHidden);

														//if(INDEX_NONE != pawn->PoseableMesh->GetBoneIndex(FName("neck_01")))
														//{
														//	if (isBoneHidden)
														//		pawn->PoseableMesh.Get()->HideBoneByName(FName("neck_01"), EPhysBodyOp::PBO_None);
														//	else
														//		pawn->PoseableMesh.Get()->UnHideBoneByName(FName("neck_01"));
														//}
													}

													//if (playerState->Play_Type == ERt_PlayType::E_Trainee || playerState->Play_Type == ERt_PlayType::E_Single)
													//{
													//	gsc->SetCurrentPlayerID(otherDeviceInfo.PlayerRoleID);
													//}
												}

												objActor->Destroy();
											}
										}

										pawn->SetMeshVisible(pawn->SkeletalMesh->IsVisible() || pawn->PoseableMesh->IsVisible());
									}
								}
							}
						}
					}

				}
			}
		}
	}
}

bool ARtPlayerController::Change_Role(int groupidx, int playeridx)
{
	ARtPlayerState* playerState = Get_PlayerState();
	ARtGameState* gameState = Get_GameState();
	ARtGameScenarioController* pController = ARtGameScenarioController::Get(this);

	if (NULL == pController || NULL == gameState || NULL == playerState || playerState->Play_Type != ERt_PlayType::E_Single)
		return false;

	if (!pController->ScenarioModel.IsValid())
		return false;

	TArray<FRtScenarioPlayerGroup> playerList = pController->ScenarioModel->SctData.PlayerGroupList;

	if (playerList.IsValidIndex(groupidx) && playerList[groupidx].PlayerList.IsValidIndex(playeridx))
	{
		FRtDeviceInfo deviceInfo = playerState->Get_PlayerInfo();
		FString newRole = playerList[groupidx].PlayerList[playeridx].PlayerObjectID;;
		UE_LOG(LogTemp, Log, TEXT("Change Role %s to %s"), *deviceInfo.PlayerRoleID, *newRole);
		deviceInfo.PlayerRoleID = newRole;
		playerState->Set_PlayerInfo(deviceInfo);

		ARtGameScenarioController* gsc = ARtGameScenarioController::Get(GetWorld());
		if (gsc)
		{
			gsc->SetCurrentPlayerID(newRole);
			
			if (gsc->CurrentSctEventCommand)
			{
				FRtScenarioEventCommand* findcommand = NULL;
				for (int i = 0; i < gsc->ScenarioModel->SctData.EventList.Num() ; ++i)
				{
					FString eventID = gsc->ScenarioModel->SctData.EventList[i].EventNameTID;
					if (gsc->CurrentSctEventCommand->EventID == eventID)
						break;

					FRtScenarioEventCommand* ecommand = gsc->ScenarioModel->EventCommandMap.Find(eventID);
					if (ecommand)
					{
						for (FRtSctFunctionCommand fcommand : ecommand->ProcessCommandList)
						{
							if (ERtSctFunCmd::ShowScreenText == fcommand.FunctionCommand && newRole == fcommand.Params[0].ParamString)
							{
								findcommand = ecommand;
								break;
							}
						}
					}
				}

				if (findcommand)
				{
					for (FRtSctFunctionCommand command : findcommand->ProcessCommandList)
					{
						if (ERtSctFunCmd::ShowScreenText == command.FunctionCommand)
						{
							ARtGameScenarioController::FDelegateCallCommand* found = gsc->CallCommandFunctionMap.Find(command.FunctionCommand);
							if (found)
							{
								bool retValue = false;
								found->ExecuteIfBound(command.Params, retValue);
							}
						}
					}
				}
			}
		}


		ARtTraineePawn* pPawn = Cast<ARtTraineePawn>(GetPawn());
		URt_Trainee_Main* pWidget = pPawn ? pPawn->GetMainWidget() : NULL;
		if (pWidget)
			pWidget->UpdateRoleText();


		return true;
	}

	return false;
}


URtReplayManager* ARtPlayerController::Get_ReplayManager()
{
	if (UGameInstance* gi = GetGameInstance())
	{
		return gi->GetSubsystem<URtReplayManager>();
	}
	return nullptr;
}

// dave
void ARtPlayerController::Server_RequestResetTraining_Implementation()
{
	if (auto gm = Get_GameMode())
	{
		// reset flag
		bHasBeenAssigned = false;
		gm->RequestResetTraining(Get_Id());
		
		//!! gm->ExitTraining();   // TEST

	}
}

// dave
void ARtPlayerController::Client_ResetTraining_Implementation()
{
	// Reset each parameters
	if (auto* ps = Get_PlayerState())
	{
		switch (ps->Play_Type)
		{
		case ERt_PlayType::E_Trainee:
		{

		}
			break;
		case ERt_PlayType::E_Server:
		{
		}
			break;
		case ERt_PlayType::E_Control:
		{
			if (IsLocalController())
			{
				Open_Page("ControlSelectMap");
			}
		}
			break;
		case ERt_PlayType::E_Single:
		{

		}
			break;
		}

		GEngine->ForceGarbageCollection(true);
	}
}


bool ARtPlayerController::Server_RequestResetTraining_Validate()
{
	return true;
}

// dave - Test
void ARtPlayerController::ExecuteTrainingReset()
{
	// 1. GameState 초기화
	if (ARtGameState* GameState = Get_GameState())
	{
		// 시나리오 정보 초기화
		FRtScenarioInfo EmptyInfo;
		GameState->Set_Scenario_Info(EmptyInfo);

		// 훈련 인덱스 초기화
		GameState->Training_Index = 0;
	}

	// 2. PlayerState 초기화
	if (auto* ps = Get_PlayerState())
	{
		if (ps->Play_Type == ERt_PlayType::E_Control) 
		{
			Open_Page("ControlSelectMap");
		}
		// 플레이어 타입 초기화
		ps->Play_Type = ERt_PlayType::E_None;

		// 플레이어 정보 초기화
		FRtDeviceInfo EmptyDeviceInfo;
		ps->Set_PlayerInfo(EmptyDeviceInfo);
	}

	// 3. 게임 씬 매니저를 통한 리소스 정리
	if (ARtGameSceneManager* GameSceneManager = ARtGameSceneManager::Get(GetWorld()))
	{
		// 모든 오브젝트 언로드
		GameSceneManager->DestroyAllSpawnedObjects();

		// MCT 언로드
		GameSceneManager->UnloadMct();
	}

	// 4. 시나리오 컨트롤러 초기화
	if (ARtGameScenarioController* ScenarioController = ARtGameScenarioController::Get(GetWorld()))
	{
		ScenarioController->UnloadSct();
		ScenarioController->Init(ERt_PlayType::E_None, TEXT(""));
	}


	if (UGameInstance* gi = GetGameInstance()) {
		if (URtGameManager* gmgr = gi->GetSubsystem<URtGameManager>()) {
			if (URtTrainingManager* tmgr = gmgr->TrngManager)
				tmgr->SetControlFlow(EControlFlow::CONTROL_SETSCT);
		}
	}

	// 6. 로컬 플레이어인 경우 UI 전환
	if (IsLocalController())
	{
		Open_Page("ControlSelectMap");
	}

	// 7. 가비지 컬렉션 요청
	GEngine->ForceGarbageCollection(true);
	
}


// dave
void ARtPlayerController::OnLoadFinished()
{
	if (IsLocalController())
	{
		if (auto* player_state = Get_PlayerState())
		{
			if (player_state->Play_Type == ERt_PlayType::E_Control)
			{
				if (auto* character = Cast<ARtTraineePawn>(GetPawn()))
				{
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Character is null ( OnLoadFinished ) "));
				}
			}
			else if (player_state->Play_Type == ERt_PlayType::E_Trainee)
			{
				//Exx_Send_Packet_Ready();
			}
			else
			{
				if (auto* character = Cast<ARtTraineePawn>(GetPawn()))
				{
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Character is null ( OnLoadFinished )"));
				}
			}
		}
	}
}

void ARtPlayerController::Client_Disconnect_Implementation()
{
	if (URtNetworkManager* NetManager = GetGameInstance()->GetSubsystem<URtNetworkManager>())
	{
		NetManager->Disconnect();
	}
}

// dave - control view move
void ARtPlayerController::MoveCameraByDelta(const FVector2D& MouseDelta)
{
	if (Get_PlayType() == ERt_PlayType::E_Control && ControllerModeCamera.IsValid())
	{
		if (UCameraComponent* Cam = ControllerModeCamera->GetCameraComponent())
		{
			const float BaseSensitivity = 0.1f; 
			float MoveSpeed = Cam->OrthoWidth * BaseSensitivity;  
			FVector MoveVector(MouseDelta.Y * MoveSpeed, -MouseDelta.X * MoveSpeed, 0.0f);
			ControllerModeCamera->SetActorLocation(ControllerModeCamera->GetActorLocation() + MoveVector);
				
			UE_LOG(LogTemp, Warning, TEXT("Move: %s"), *MoveVector.ToString());
#if !UE_BUILD_SHIPPING
#if WITH_EDITOR
			FVector Loc = ControllerModeCamera->GetActorLocation();
			FString Msg = FString::Printf(TEXT("Camera Location : %s"), *Loc.ToString());
			GEngine->AddOnScreenDebugMessage(100, 10.0f, FColor::Yellow, Msg, true, FVector2D(2.0f, 2.0f));
#endif
#endif
		}
	}
}
// dave - control view zoom

void ARtPlayerController::ZoomCamera(float ScrollDelta)
{
	if (auto* player_state = Get_PlayerState())
	{
		if (player_state->Play_Type == ERt_PlayType::E_Control)
		{
			if (!ControllerModeCamera.IsValid()) return;

			if (UCameraComponent* Cam = ControllerModeCamera->GetCameraComponent())
			{
				const float ZoomSpeed = 50.0f;
				ControllerModeCamera->SetActorLocation(ControllerModeCamera->GetActorLocation() + FVector(0, 0, ScrollDelta * ZoomSpeed));

				UE_LOG(LogTemp, Warning, TEXT("Zoom Changed: = %.1f"), ScrollDelta);
#if !UE_BUILD_SHIPPING
#if WITH_EDITOR
				FVector Loc = ControllerModeCamera->GetActorLocation();
				FString Msg = FString::Printf(TEXT("Camera Location : %s"), *Loc.ToString());
				GEngine->AddOnScreenDebugMessage(100, 10.0f, FColor::Yellow, Msg, true, FVector2D(2.0f, 2.0f));
#endif
#endif
			}
		}
	}
}

