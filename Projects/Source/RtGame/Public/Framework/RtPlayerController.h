// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/RtPlayerControllerBase.h"
// Data
#include "Common/RtRpcCommon.h"
#include "Common/RtWebCommon.h"
#include "GameplayTagContainer.h"

// Voice Test
// UE
#include "OnlineSubsystemUtils.h"

#include "RtPlayerController.generated.h"



// Framework
class ARtPlayerState;
class ARtGameState;
class ARtGameHUD;
class ARtGameMode;
class ARtTraineePawn;
class ARtReplayTrainee;

// Manager
class URtGameManager;
class URtWebManager;
class ARtGameScenarioController;

// Widget
class URt_PageBase;
class URt_PrimaryLayout;

// Network
class URtNetworkManager;

/**
 *
 */
UCLASS()
class RTGAME_API ARtPlayerController : public APlayerController
{
	GENERATED_BODY()

	// Various
public:

	ARtPlayerController();

	FRtUserInfo_PreLogin MyLoginInfo;

	UPROPERTY()
	TArray<FRtDeviceInfo> Accessible_PlayerDevices;

	UPROPERTY()
	TArray<FRtTraineeInfo> Accessible_Trainees;


	UPROPERTY()
	FRtScenarioInfo Current_Scenario;

	// Scenario
	UPROPERTY()
	TArray<FString> FileList_sct;

	// DelegateHandle
	DECLARE_MULTICAST_DELEGATE_TwoParams(FUpdate_DeviceList, APlayerController*, const TArray<FRtDeviceInfo>&);
	FUpdate_DeviceList Update_DeviceList_Delegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FUpdate_TraineeList, APlayerController*, const TArray<FRtTraineeInfo>&);
	FUpdate_TraineeList Update_TraineeList_Delegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FUpdate_PlayerPreparing, APlayerController*, const TArray<FRtDeviceInfo>&);
	FUpdate_PlayerPreparing RscDele_Update_PlayerPreparing;

	// When Started Training
	DECLARE_MULTICAST_DELEGATE_TwoParams(FClient_Start_Training, APlayerController*, int32);  // Start Training
	FClient_Start_Training Client_Start_Training_Delegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FClient_Ready_For_Training, int32, bool);
	FClient_Ready_For_Training Client_Ready_Delegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FClient_Confirm_TraineeAassignment, APlayerController*, int32);
	FClient_Confirm_TraineeAassignment Client_Confirm_TraineeAassignment_Delegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FClient_Confirm_TraineeDeployment_Delegate, APlayerController*, int32);
	FClient_Confirm_TraineeDeployment_Delegate Client_Confirm_TraineeDeployment_Delegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChanged_DeviceInfo, const FRtDeviceInfo&, ARtPlayerState*);
	FOnChanged_DeviceInfo OnChanged_DeviceInfo_delegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChanged_TraineeInfo, const FRtTraineeInfo&, ARtPlayerState*);
	FOnChanged_TraineeInfo OnChanged_TraineeInfo_delegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FTwoParam_Delegate, APlayerController*, const int32 InPlayerId);
	static FTwoParam_Delegate Add_Player;
	static FTwoParam_Delegate Remove_Player;

	// Functions
public:
	// Main Flow
	void Init_PlayType();
	void Init_RePlayType();
	void Log_Message(ELogProtocol protocol);

	// TEST
	UFUNCTION(exec) void Exx_Send_Log_Message();
	UFUNCTION(exec) void Exx_Send_Packet_Ready();
	UFUNCTION() void StartTraining(int32 TID);


	// Check Data
	bool IsAlreadyAssignedTrainee(const FRtTraineeInfo& InInfo); // 이미 할당된 훈련자?
	bool IsAlreadyAssignedRole(const FRtRoleInfo& InInfo);

	// dave
	void OnLoadFinished();

	// Server
	UFUNCTION(Reliable, Server)
	void Server_Send_DeviceInfo(int32 InId);

	UFUNCTION(Reliable, Server)
	void Server_Send_TraineeInfo(int32 InId);

	UFUNCTION(Reliable, Server)
	void Server_Confirm_TraineeAassignment();

	UFUNCTION(Reliable, Server)
	void Server_Confirm_TraineeDeployment();

	UFUNCTION(Reliable, Server)
	void Server_AssignTrainee_ToDevice(const FString& InDeviceName, const FRtTraineeInfo& InInfo);

	UFUNCTION(Reliable, Server)
	void Server_AssignRole_ToDevice(const FString& InDeviceName, const FRtRoleInfo& InInfo);

	UFUNCTION(Reliable, Server)
	void Server_Set_Scenario_Info(const FRtScenarioInfo& Info);

	UFUNCTION(Reliable, Server)
	void Server_StartTraining(int32 TID);

	UFUNCTION(Reliable, Server) // 리슨 서버에서 실행 되는 함수.
	void Server_RequestRandomNum(const FString& variableName, const int32& lastNum);
	
	// dave
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestResetTraining();

	UFUNCTION(Reliable, Client)
	void Client_ResetTraining();

	// Client
	UFUNCTION(Reliable, Client)
	void Client_Update_DeviceInfo(const TArray<FRtDeviceInfo>& InInfo);

	UFUNCTION(Reliable, Client)
	void Client_Update_TraineeInfo(const TArray<FRtTraineeInfo>& InInfo);

	UFUNCTION(Reliable, Client)
	void Client_Confirm_TraineeAassignment();

	UFUNCTION(Reliable, Client)
	void Client_Confirm_TraineeDeployment();

	UFUNCTION(Reliable, Client)
	void Client_Set_Scenario_Info(const FRtScenarioInfo& Info);

	UFUNCTION(Client, Reliable)
	void Client_Disconnect();


	// Flag to ensure the RPC is Called only once
	bool bHasBeenAssigned = false;



	// Costom Get Set 
	// Get
	// Training
	int32 Get_Id();
	// Whole Struct
	URtGameManager* Get_GameManager();
	URtNetworkManager* Get_NetManager();
	URtWebManager* Get_WebManager();
	ARtPlayerState* Get_PlayerState();
	ARtGameState* Get_GameState();
	ARtGameHUD* Get_HUD();
	ARtGameMode* Get_GameMode();
	bool Get_DeviceInfo(FRtDeviceInfo& OutInfo);
	UFUNCTION(BlueprintCallable)
	const TArray<FRtDeviceInfo>& Get_Accessible_PlayerDevices();
	UFUNCTION(BlueprintCallable)
	const TArray<FRtTraineeInfo>& Get_Accessible_Trainees();
	virtual TWeakObjectPtr<URt_PrimaryLayout> Get_Primary_Layout();
	ERt_PlayType Get_PlayType();


	// Set
	void Set_MyLoginInfo(const FRtUserInfo_PreLogin& info);
	void Set_Spectator(bool val);
	void Set_DeviceInfo(const TArray<FRtDeviceInfo>& InInfo);
	void Set_TraineeInfo(const TArray<FRtTraineeInfo>& InInfo);
	void Set_ScenarioInfo(const FRtScenarioInfo& InInfo);

	void HasStarted_MatchState(const FName& InState);

	//dave
	void HasExited_MatchState(const FName& InState);
	void ExecuteTrainingReset();

	void MoveCameraByDelta(const FVector2D& MouseDelta);
	void ZoomCamera(float ScrollDelta);

	void Change_CharacterIcon(const FText& InText);

	// ViewControl
	void Change_TraineeSelectionView(AActor* InActor);
	bool Change_TraineeSelectionView(const FString& InStr);
	ARtTraineePawn* Find_Trainee(const FString& InStr);
	void SetReplayTrainee(const int32 uid);

	// UI Control
	UFUNCTION(BlueprintCallable)
	URt_PageBase* Open_Popup(const FName& InName);
	UFUNCTION(BlueprintCallable)
	URt_PageBase* Open_Page(const FName& InName);

	bool Close_Popup(URt_PageBase* InPage);

	UFUNCTION(BlueprintCallable)
	FRtScenarioInfo ReadScenarioFile(FString filename);

	// DelegateBindFunc
	void Handle_Changed_DeviceInfo();
	void CallBeforeBindingToPlayerState();
protected:
	virtual void BeginPlay() override;

	// dave
	virtual void Destroyed() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;
	virtual void PawnLeavingGame() override;

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	// Unreal Virtual Func
	virtual void OnRep_PlayerState() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void ReceivedPlayer() override;

public:
	// Control Mode CameraView
	void Update_ControlModeCamera(const FVector2D& InOffset2);

	TWeakObjectPtr<ACameraActor> ControllerModeFollowCamera;
	TWeakObjectPtr<ACameraActor> ControllerModeCamera;
	TWeakObjectPtr<ACameraActor> ReplayModeCamera;

	FVector ControllerModeCamera_Location;
	FVector ControllerModeCamera_Location_Offset;
	FRotator ControllerModeCamera_Rotator;
	FVector ReplayModeCamera_Location;

	TWeakObjectPtr<AActor> ControllerModeFollowCamera_Target; //follow camera target 이 설정된다.
	TWeakObjectPtr<AActor> ReplayModeFollowCamera_Target;
private:
	ACameraActor* Get_ControllMode_ViewCamera();
	ACameraActor* Get_ControllMode_FollowCamera();

	ACameraActor* Get_ReplayMode_Camera();

	bool ControlMode_PlayerTick = false;
	bool ReplayMode_PlayerTick = false;



public:
	void LoadSct();

public:
	UFUNCTION(Exec)
	bool Change_Role(int groupidx, int playeridx);

	///////////////////// replay 

public:
	class URtReplayManager* Get_ReplayManager();

};
