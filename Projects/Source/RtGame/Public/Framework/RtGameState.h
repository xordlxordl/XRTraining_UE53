// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

// Data
#include "Common/RtRpcCommon.h"

#include "RtGameState.generated.h"

RTGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogRtGameState, Log, All);


class ARtPlayerState;

USTRUCT(BlueprintType)
struct FScenarioVariableFair
{
	GENERATED_BODY()

	UPROPERTY()
	FString Key;

	UPROPERTY()
	int32 Value;

	FScenarioVariableFair() = default;
};

USTRUCT(BlueprintType)
struct FScenarioNpcUpdateInfo
{
	GENERATED_BODY()

	FScenarioNpcUpdateInfo() = default;
	FScenarioNpcUpdateInfo(FString npcID, FVector2D npcPos) {
		NpcID = npcID;
		NpcPos = npcPos;
	}

	UPROPERTY()
	FString NpcID;

	UPROPERTY()
	FVector2D NpcPos;
};

/**
 * 
 */
UCLASS()
class RTGAME_API ARtGameState : public AGameStateBase
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(Transient, Replicated)	int32 RemainTime;
	UPROPERTY(Transient, Replicated)	int32 Training_Score;
	UPROPERTY(ReplicatedUsing = OnRep_Training_Index)    int32 Training_Index;


	UPROPERTY(Transient)	bool bTimerPaused = false;

	UPROPERTY(ReplicatedUsing = OnRep_OnUpdate_ScenarioVariables) FScenarioVariableFair ScenarioVariableSync;
	UPROPERTY(ReplicatedUsing = OnRep_OnUpdate_AllNpcPos) TArray<FScenarioNpcUpdateInfo> NpcUpdatInfoArray;

// Delegate
    DECLARE_MULTICAST_DELEGATE_OneParam(FAdd_PlayerState, APlayerState*);
    static	FAdd_PlayerState Add_PlayerState;

    DECLARE_MULTICAST_DELEGATE_OneParam(FRemove_PlayerState, APlayerState*);
	static  FRemove_PlayerState Remove_PlayerState;

	DECLARE_MULTICAST_DELEGATE_OneParam(FChanged_InProgress_State, ARtGameState*);
	static FChanged_InProgress_State Changed_InProgress_State_Delegate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FChanged_GameStateContent_Delegate, ARtGameState*);
	static FChanged_GameStateContent_Delegate Changed_GameStateContent_Delegate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FChanged_GameScenarioInfo, ARtGameState*);
	static FChanged_GameScenarioInfo Changed_GameScenarioInfo_Delegate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FChanged_Devicenfo, ARtGameState*);
	static FChanged_Devicenfo Changed_DeviceInfo_Delegate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FChanged_TraineeInfo, ARtGameState*);
	static FChanged_TraineeInfo Changed_TraineeInfo_Delegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDeviceInfoChangedDelegate, int32 PlayerId, const FRtDeviceInfo& InDeviceInfo);
	static FOnDeviceInfoChangedDelegate OnDeviceInfoChanged;

private:
	UPROPERTY(Transient) FString Custom_MapName;  // MapName from SCT or MCT JSON File


protected:
	FName Previous_Match_State;
	UPROPERTY(ReplicatedUsing = OnRep_Match_State)	FName Match_State;
	UPROPERTY(ReplicatedUsing = OnRep_Scenario_Info)	FRtScenarioInfo Scenario_Info;
	UPROPERTY(ReplicatedUsing = OnRep_Device_Info)  TArray<FRtDeviceInfo> Device_Info;
	UPROPERTY(ReplicatedUsing = OnRep_Player_Info) TArray<FRtTraineeInfo> Trainee_Info;


public:
	void Init_GameState();
	// Assign
	UFUNCTION(BlueprintCallable)
	ARtPlayerState* Find_DeviceUser(const FString& InDevice);
	UFUNCTION(BlueprintCallable)
	ARtPlayerState* Find_AssignedDevice(const FString& InTrainee);
	ARtPlayerState* Find_AssignedDeviceById(const int32 PlayerId);
	/** Add PlayerState to the PlayerArray */
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	/** Remove PlayerState from the PlayerArray. */
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

	void Update_AllPlayerStates(TArray<FRtDeviceInfo>& InData);

	// Custom Get Set
	// Get
	const FName& Get_Match_State() { return Match_State; }
	UFUNCTION(BlueprintCallable)
	const FRtScenarioInfo& Get_Scenario_Info();
	UFUNCTION(BlueprintCallable)
	const TArray<FRtDeviceInfo>& Get_Device_Info();
	TObjectPtr<APlayerState> Get_PlayerState_ByDeviceName(const FString& InDevice);
	// Set
	/** Updates the match state and calls the appropriate transition functions, only valid on server */
	void SetMatchState(FName NewState);
	UFUNCTION(BlueprintCallable)
	void Set_Scenario_Info(const FRtScenarioInfo& InInfo);
	void Set_Device_Info(const TArray<FRtDeviceInfo>& InInfo);

	/** Match state has changed */
	UFUNCTION()	virtual void OnRep_Match_State();
	UFUNCTION()	void OnRep_Scenario_Info();
	UFUNCTION() void OnRep_Device_Info();
	UFUNCTION() void OnRep_Player_Info();

	UFUNCTION() void OnRep_Training_Index();

	UFUNCTION() void OnRep_OnUpdate_ScenarioVariables();
	UFUNCTION() void OnRep_OnUpdate_AllNpcPos();

	UFUNCTION() void Set_OnUpdate_AllNpcPos();


	UFUNCTION() void HandleDeviceInfoChanged(int32 PlayerId, const FRtDeviceInfo& Info);

	void Bind();
	void UnBind();
	void UpdateDeviceInfo();

private:

// MatchState Code
	/** Called when the state transitions to WaitingToStart */
	void Handle_MatchIsWaitingToStart();

	/** Called when the state transitions to InProgress */
	void Handle_MatchHasStarted();

	/** Called when the map transitions to WaitingPostMatch */
	void Handle_MatchHasEnded();

	/** Called when the match transitions to LeavingMap */
	void Handle_LeavingMap();

	void Handle_PlayerReady_HasStarted();
	void Handle_RunningTrng_HasStarted();
	void Handle_PreResult_HasStarted();
	void Handle_Result_HasStarted();



};
