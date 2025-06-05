// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtGameState.h"

//Unreal Basic Framework
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

//Framework
#include "Framework/RtGameMode.h"
#include "Framework/RtPlayerState.h"

//Manager
#include "Manager/RtGameManager.h"
#include "Manager/RtWebManager.h"
#include "RtGame/Public/Manager/RtConfig.h"


//Data
#include "RtCore/Public/Common/RtCommon.h"

#include "Manager/RtGameScenarioController.h"

#include "Framework/RtPlayerController.h"

#include "RtBluePrintCommonUtil.h"
#include "RtDefines.h"

#include "Manager/RtGameSceneManager.h"
#include "Framework/RtNpcObjectActor.h"

DEFINE_LOG_CATEGORY(LogRtGameState);

ARtGameState::FAdd_PlayerState ARtGameState::Add_PlayerState;
ARtGameState::FRemove_PlayerState ARtGameState::Remove_PlayerState;
ARtGameState::FChanged_InProgress_State ARtGameState::Changed_InProgress_State_Delegate;
ARtGameState::FChanged_GameStateContent_Delegate ARtGameState::Changed_GameStateContent_Delegate;
ARtGameState::FChanged_GameScenarioInfo ARtGameState::Changed_GameScenarioInfo_Delegate;
ARtGameState::FChanged_Devicenfo ARtGameState::Changed_DeviceInfo_Delegate;
ARtGameState::FChanged_TraineeInfo ARtGameState::Changed_TraineeInfo_Delegate;
ARtGameState::FOnDeviceInfoChangedDelegate ARtGameState::OnDeviceInfoChanged;

//UFUNCTION() void HandleDeviceInfoChanged(int32 PlayerId, const FRtDeviceInfo& Info);

ARtGameState::ARtGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Match_State = RtMatchState::EnteringMap;
	Previous_Match_State = RtMatchState::EnteringMap;

	RemainTime = 0;
	bTimerPaused = false;
	bReplicates = true;
}

void ARtGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARtGameState, Match_State);
	DOREPLIFETIME(ARtGameState, Training_Score);
	DOREPLIFETIME(ARtGameState, Training_Index);
					   
	DOREPLIFETIME(ARtGameState, RemainTime);
	DOREPLIFETIME(ARtGameState, Scenario_Info);
	DOREPLIFETIME(ARtGameState, Device_Info);
	DOREPLIFETIME(ARtGameState, Trainee_Info);

	DOREPLIFETIME(ARtGameState, ScenarioVariableSync);
	DOREPLIFETIME(ARtGameState, NpcUpdatInfoArray);

	//FDoRepLifetimeParams SharedParams;
	//SharedParams.bIsPushBased = true;

	//DOREPLIFETIME_WITH_PARAMS_FAST(ARtGameState, TrngEvaluationRecord, SharedParams);
}


void ARtGameState::Init_GameState()
{
	Match_State = RtMatchState::EnteringMap;
	Previous_Match_State = RtMatchState::EnteringMap;

	Custom_MapName = URtConfig::Custom_MapName;
}

ARtPlayerState* ARtGameState::Find_DeviceUser(const FString& InDevice)
{
	for (auto it : PlayerArray) {
		if (auto* ps = Cast<ARtPlayerState>(it.Get())) {
			if (ps->Login_DeviceName.Equals(InDevice)) {
				return ps;
			}
		}
	}
	return nullptr;
}

ARtPlayerState* ARtGameState::Find_AssignedDeviceById(const int32 PlayerId)
{
	for (auto it : PlayerArray) {
		if (auto* ps = Cast<ARtPlayerState>(it.Get())) {
			if (ps->PlayerId == PlayerId) {
				return ps;
			}
		}
	}
	return nullptr;
}

ARtPlayerState* ARtGameState::Find_AssignedDevice(const FString& InTrainee)
{
	for (auto it : PlayerArray) {
		if (auto* ps = Cast<ARtPlayerState>(it.Get())) {
			if (ps->Login_PlayerName.Equals(InTrainee)) {
				return ps;
			}
		}
	}
	return nullptr;
}

void ARtGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	Add_PlayerState.Broadcast(PlayerState);
}

void ARtGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
	Remove_PlayerState.Broadcast(PlayerState);
}

const FRtScenarioInfo& ARtGameState::Get_Scenario_Info()
{
	return Scenario_Info;
}

void ARtGameState::Update_AllPlayerStates(TArray<FRtDeviceInfo>& InData)
{
	for (auto& it : InData) {
		TObjectPtr<APlayerState> found = Get_PlayerState_ByDeviceName(it.Name);
		if (auto* ps = Cast<ARtPlayerState>(found.Get())) {
			it.Assign_Trainee(ps->Get_PlayerInfo().PlayerInfo, ps->Get_PlayerID(), ps->Get_PlayerInfo().PlayerInfo.Traineeheight);
		}
	}
}

TObjectPtr<APlayerState> ARtGameState::Get_PlayerState_ByDeviceName(const FString& InDevice)
{
	for (auto it : PlayerArray) {
		if (auto* ps = Cast<ARtPlayerState>(it.Get())) {
			if (ps->Login_DeviceName.Equals(InDevice)) {
				return ps;
			}
		}
	}
	return nullptr;
}

const TArray<FRtDeviceInfo>& ARtGameState::Get_Device_Info()
{
	return Device_Info;
}

void ARtGameState::SetMatchState(FName NewState)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogRtGameState, Log, TEXT("Match State Changed from %s to %s"), *Match_State.ToString(), *NewState.ToString());
		Match_State = NewState;
		// Call the onrep to make sure the callbacks happen
		OnRep_Match_State();
	}
}

void ARtGameState::Set_Scenario_Info(const FRtScenarioInfo& InInfo)
{
	Scenario_Info = InInfo;
}

void ARtGameState::Set_Device_Info(const TArray<FRtDeviceInfo>& InInfo)
{
	Device_Info = InInfo;
}



void ARtGameState::OnRep_Match_State()
{
	if (Match_State == RtMatchState::WaitingToStart || Previous_Match_State == RtMatchState::EnteringMap)
	{
		// Call MatchIsWaiting to start even if you join in progress at a later state
		Handle_MatchIsWaitingToStart();
	}
	else if (Match_State == RtMatchState::InProgress_StartTrng)
	{
		Handle_MatchHasStarted();
	}
	else if (Match_State == RtMatchState::InProgress_PlayerReady)
	{
		Handle_PlayerReady_HasStarted();
	}
	else if (Match_State == RtMatchState::InProgress_RunningTrng)
	{
		Handle_RunningTrng_HasStarted();
	}
	else if (Match_State == RtMatchState::InProgress_PreResult)
	{
		Handle_PreResult_HasStarted();
	}
	else if (Match_State == RtMatchState::InProgress_Result)
	{
		Handle_MatchHasEnded();
	}
	else if (Match_State == RtMatchState::LeavingMap)
	{
		Handle_LeavingMap();
	}
	
	Previous_Match_State = Match_State;

	//Changed_GameStateContent_Delegate.Broadcast(this); //!!!! 테스트 코드다. 무슨 코드인지 확인 필요
}

void ARtGameState::OnRep_Scenario_Info()
{
	Changed_GameScenarioInfo_Delegate.Broadcast(this);
}

void ARtGameState::OnRep_Device_Info()
{
	Changed_DeviceInfo_Delegate.Broadcast(this);
}

void ARtGameState::OnRep_Player_Info()
{
	Changed_TraineeInfo_Delegate.Broadcast(this);
}

void ARtGameState::OnRep_OnUpdate_ScenarioVariables()
{
	ARtGameScenarioController* ScenarioController = ARtGameScenarioController::Get(GetWorld());
	if (ScenarioController && ScenarioController->ScenarioModel.IsValid()) {
		ScenarioController->ScenarioModel->SetVariableValue(ScenarioVariableSync.Key, ScenarioVariableSync.Value);

		CUSTOM_FILE_APP_LOG_EX("App.log", "Var Name %s : %d", *ScenarioVariableSync.Key, ScenarioVariableSync.Value);

#if WITH_EDITOR
		FString playType = TEXT("");
		if (ARtPlayerController* controller = Cast<ARtPlayerController>(GetWorld()->GetFirstPlayerController())) {
			ARtPlayerState* playerState = controller->Get_PlayerState();
			if (playerState) {
				playType = URtResourceModelSubsystem::GetEnumToString(TEXT("ERt_PlayType"), (int32)playerState->Play_Type);
			}
		}

		APP_LOG("3. NetworkTest (Server -> [Client]): OnRep_OnUpdate_ScenarioVariables : %s ( %s, %d )", *playType,
			*ScenarioVariableSync.Key, ScenarioVariableSync.Value);
#endif
	}
}

// 서버로 부터 정기적으로 받는 NPC 위치정보들을 모두 갱신한다.
void ARtGameState::OnRep_OnUpdate_AllNpcPos()
{
	const TMap<FString, class ARtNpcObjectActor*>& npcMap = ARtGameSceneManager::Get(GetWorld())->GetRtNpcMap();
	for (auto updateInfo : NpcUpdatInfoArray) {
		auto npcObjectActor = npcMap.Find(updateInfo.NpcID);
		if (npcObjectActor) {
			FVector pos = (*npcObjectActor)->GetActorLocation();
			pos.X = updateInfo.NpcPos.X;
			pos.Y = updateInfo.NpcPos.Y;
			//			pos.Z = 50; // 테스트 용

			(*npcObjectActor)->SetActorLocation(pos);
		}
	}

#if WITH_EDITOR
	FString playType = TEXT("");
	if (ARtPlayerController* controller = Cast<ARtPlayerController>(GetWorld()->GetFirstPlayerController())) {
		ARtPlayerState* playerState = controller->Get_PlayerState();
		if (playerState) {
			playType = URtResourceModelSubsystem::GetEnumToString(TEXT("ERt_PlayType"), (int32)playerState->Play_Type);
		}
	}

	APP_LOG("NpcPosSync : OnRep_OnUpdate_AllNpcPos %s", *playType);
#endif

}

// 서버에서 일정 간격으로 업데이트 신호를 쏴 준다.
void ARtGameState::Set_OnUpdate_AllNpcPos()
{
	NpcUpdatInfoArray.Empty();
	const TMap<FString, class ARtNpcObjectActor*>& npcMap = ARtGameSceneManager::Get(GetWorld())->GetRtNpcMap();
	for (auto& npc : npcMap) {
		FVector npcPos = npc.Value->GetActorLocation();
		NpcUpdatInfoArray.Add(FScenarioNpcUpdateInfo(npc.Key, FVector2D(npcPos.X, npcPos.Y)));
	}

#if WITH_EDITOR
	FString playType = TEXT("");
	if (ARtPlayerController* controller = Cast<ARtPlayerController>(GetWorld()->GetFirstPlayerController())) {
		ARtPlayerState* playerState = controller->Get_PlayerState();
		if (playerState) {
			playType = URtResourceModelSubsystem::GetEnumToString(TEXT("ERt_PlayType"), (int32)playerState->Play_Type);
		}
	}

	APP_LOG("NpcPosSync : Set_OnUpdate_AllNpcPos %s", *playType);
#endif

}

void ARtGameState::OnRep_Training_Index()
{
	UE_LOG(LogTemp, Log, TEXT("OnRep_TID"));
}


void ARtGameState::Handle_MatchIsWaitingToStart()
{
	if (GetLocalRole() != ROLE_Authority)
	{
		GetWorldSettings()->NotifyBeginPlay();
	}

	FCoreDelegates::GameStateClassChanged.Broadcast(GetClass()->GetName());
}

void ARtGameState::Handle_MatchHasStarted()
{
	// Start Training
	if (GetLocalRole() != ROLE_Authority)
	{
		// Server handles this in AGameMode::HandleMatchHasStarted
		GetWorldSettings()->NotifyMatchStarted();
	}
	else
	{
		// Now that match has started, act like the base class and set replicated flag
		bReplicatedHasBegunPlay = true;
	}
}

void ARtGameState::Handle_MatchHasEnded()
{
}

// dave
void ARtGameState::Handle_LeavingMap()
{
	if (auto* mgr = GetGameInstance()->GetSubsystem<URtGameManager>()) {
		mgr->HasExited_MatchState(RtMatchState::LeavingMap);
	}
}

void ARtGameState::Handle_PlayerReady_HasStarted()
{
	if (auto* mgr = GetGameInstance()->GetSubsystem<URtGameManager>()) {
		mgr->HasStarted_MatchState(RtMatchState::InProgress_PlayerReady);
	}

	auto* world = GetWorld();
	if (!world)
		return;

//	if (auto* levelscript = Cast<ARsGameLevelScriptActor>(world->GetLevelScriptActor())) {
//
//		levelscript->Init_SPointState();
//	}
}

void ARtGameState::Handle_RunningTrng_HasStarted() // start senario
{
	// Start Replay Record 
}

void ARtGameState::Handle_PreResult_HasStarted() // end senario
{
	// End Replay Record
}

void ARtGameState::Handle_Result_HasStarted()
{
}

// dave - Bind (DeviceInfoChange)
void ARtGameState::Bind()
{
	for (auto it : PlayerArray) {
		if (auto* ps = Cast<ARtPlayerState>(it.Get())) {
			ps->OnDeviceInfoChanged.AddDynamic(this, &ARtGameState::HandleDeviceInfoChanged);
		}
	}
}

// dave - UnBind (DeviceInfoChange)
void ARtGameState::UnBind()
{
	for (auto it : PlayerArray) {
		if (auto* ps = Cast<ARtPlayerState>(it.Get())) {
			ps->OnDeviceInfoChanged.RemoveDynamic(this, &ARtGameState::HandleDeviceInfoChanged);
		}
	}
}

// dave
void ARtGameState::HandleDeviceInfoChanged(int32 PlayerId, const FRtDeviceInfo& Info)
{
	UE_LOG(LogRtGameState, Log, TEXT(">>>>> HandleDeviceInfoChanged(PlayerId) : %d"), PlayerId);
	OnDeviceInfoChanged.Broadcast(PlayerId, Info);
}

// dave - Notify deivce infos of all players
void ARtGameState::UpdateDeviceInfo()
{
	for (auto it : PlayerArray) {
		if (auto* ps = Cast<ARtPlayerState>(it.Get())) {
			OnDeviceInfoChanged.Broadcast(it->PlayerId, ps->Get_PlayerInfo());
		}
	}
}