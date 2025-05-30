// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/RtGameModeBase.h"

//Data 
#include "RtCore/Public/Common/RtCommon.h"
#include "Common/RtRpcCommon.h"
#include "RtGameMode.generated.h"

// Framework
class ARtPlayerController;
class ARtGameState;

// Manager
class URtTrainingManager;
class URtReplayManager;
// Level prop
class ARtTrainingReadyPoint;

/**
 * 
 */
UCLASS()
class RTGAME_API ARtGameMode : public ARtGameModeBase
{
	GENERATED_UCLASS_BODY()
	// GENERATED_BODY �� UCLASS �ٵ�� ��ü ���־�� �ʱ�ȭ ������Ʈ ���ڸ� ����� �����ڸ� ��� ����
// Delegate
public:
	//login delegate
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPostLogin, AGameModeBase* /*GameMode*/, AController* /*NewPlayer*/);
	FOnPostLogin OnPostLogin_Delegate;

	//Player Delegate
	DECLARE_MULTICAST_DELEGATE(Dele_Player_Connected);
	static Dele_Player_Connected Player_Connected_Delegate;

// ����
public:
	UPROPERTY(config)	int32 Progress_StartTrng_Time;
	UPROPERTY(config)	int32 Progress_PlayerReady_Time;
	UPROPERTY(config)	int32 Progress_RunningTrng_Time;
	UPROPERTY(config)	int32 Progress_PreResult_Time;


	TArray<ARtTrainingReadyPoint*> TrainingReadyPointArray;
    
private:
	ENetMode Net_Mode;
	int32 ConnectCount; // Number of Clients
	FDateTime Scenario_StartTime;// current senario start time
	FDateTime Scenario_EndTime;	// current senario end time

	static int32 Static_Count;//``login �Ǵ� Ŭ���̾�Ʈ �� count �̴�. 
	TArray<ARtPlayerController*> Trainee_Array;
	ARtPlayerController* Control_Controller;
	TArray<ARtPlayerController*> Trainer_Array;
	//FRtTrngResult TrngResult; //trainig result 

	// Login 
	//''�α��� ������ �߻��ϴ� �������� ������ ��� �����Ѵ�. ���ܻ�Ȳ ó���� �ʿ��ѵ�. �Ʒ��߿� �����ϴ� ���� ������ ���� ���¿� �����ӽ�Ű�� �۾��� ����Ѵ�. 
    //''���÷� SaveGame ���Ͽ� ���� �ϴ� ��� �۾��� �ʿ��ϴ�.
	TArray<FRtUserInfo_PreLogin> Game_PreLogin;
	TArray<FRtUserInfo_PreLogin> Others_PreLogin;
	TArray<FRtUserInfo_PostLogin> Game_PostLogin;
	TArray<FRtUserInfo_PostLogin> Others_PostLogin;

	TMap<FString, FRtUserInfo_PreLogin> Lobby_Login_Try_ClientList;
	TMap<FString, FRtUserInfo_PreLogin> Game_Login_Try_ClientList;

	// Boolean 
	bool bAutoPlayType = false;
	bool Pause_RemainTime = false;
	bool Reject_AllClient_Event = false; 	//''��� client ������ �������� ���

protected:
	FTimerHandle TimerHandle_DefaultTimer;


// �Լ�

// check Control Mode
private:
	bool IsControllerPlayType(const int32 InPlayerId);
	ARtPlayerController* IsControllerId(int32 InId); //  check controller id

public:
// Trainning Flow
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void InitGameState() override;
	virtual void PreInitializeComponents() override;
	
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void StartPlay() override;

// Start Trainning
	void Confirm_TraineeAassignment(int32 InId);//``����̽��� �Ʒ��� �Ҵ� �۾��� ������.
	void Confirm_TraineeAassignment(bool bSync);
	void Confirm_TraineeDeployment(int32 InId);

	// dave
	void Confirm_BackConfirmation(int32 InId);
	void RequestResetTraining(int32 InId);

// Custom Trainning Flow
	//``Pre login �� �߻� ������ �����Ѵ�. ���� �κ� ������, ���� �κ� ���� ����� ��� �� ���̴�. 
	bool Pre_Login(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage, ERt_GameMode InGameMode);
	//``Post login �� �߻� �����͸� �����Ѵ�. ���� �κ� ���� ����� ��� �� ���̴�. 
	void Post_Login(APlayerController* InPlayerController, ERt_GameMode InGameMode);

	//''Pre login �� �߻��� ������ ��´�.
	bool Get_PreLoginInfo(ARtPlayerController* InPlayerController, FRtUserInfo_PreLogin& OutPreLoginInfo);

	//``Prelogin ���� ��ϵ� PlayType �� ��µ�
	ERt_PlayType Get_Recored_PlayType_InPreloginInfo(ARtPlayerController* InPlayerController);

	// Change Information PlayerInfo and PlayerDeviceInfo
	void Change_PlayerInfo(const FRtDeviceInfo& InInfo);
	void Change_PlayersInfo(const TArray<FRtDeviceInfo>& InInfo);

	// Assign Trainee
	bool AssignTrainee_ToDevice(const FString& InDeviceName, const FRtTraineeInfo& InInfo, int32 InPlayerId);
	bool AssignRole_ToDevice(const FString& InDeviceName, const FRtRoleInfo& InInfo, int32 InPlayerId);

// Custom GetSet, not Unreal Engine's Function
	ARtGameState* Get_GameState();
	ARtPlayerController* Get_PlayerController(int32 InPlayerId);  // get player controll by player id 
	ARtPlayerController* Get_PlayerController();
	class URtGameManager* GetGameManager();
	TArray<ARtPlayerController*> GetTraineeArray() { return Trainee_Array; }
// Device
	void SC_DeviceInfo();
	void SC_DeviceInfo(int32 InId);

// Trainee
	void SC_TraineeInfo();
	void SC_TraineeInfo(int32 InId);

	ARtPlayerController* ExistDevice(APlayerController* NewPlayer);
	TArray<ARtPlayerController*> ExistDeviceAll(APlayerController* NewPlayer);

	void ResultMatch();

	//dave
	void ExitTraining();

private:
	void OnMatchStateSet();
	/** Transition from WaitingToStart to InProgress. You can call this manually, will also get called if ReadyToStartMatch returns true */
	void StartMatch();
	

	// ���ſ���
	//void DefaultTimer();

    bool Check_PreLoginIntegrity(const FRtUserInfo_PreLogin& InInfo); // ������ �������� ���� üũ�Ͽ� �����Ǻ�

// Training
	bool Add_New_Player(ARtPlayerController* InPlayerController);
	bool Remove_Player(ARtPlayerController* InPlayerController);


// Custom Get Set
	TArray<FRtDeviceInfo>& Get_DeviceArray();
	TArray<FRtTraineeInfo>& Get_TraineeArray();
	TArray<FRtDeviceInfo>& Get_DeviceArray_BeingTrained();

	URtTrainingManager* Get_TrngManager();
	URtReplayManager* Get_ReplayManager();

protected:
// Trainning Flow
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; 

// Match Flow
	void Handle_MatchIsWaitingToStart();
	void Handle_MatchHasStarted();

	void Handle_InProgress_PlayerReady_HasStarted();
	void Handle_InProgress_RunningTrng_HasStarted();
	void Handle_InProgress_PreResult_HasStarted();
	void Handle_InProgress_Result_HasStarted();
	
// Custom Get Set
// Get 

// Set
	/** Updates the match state and calls the appropriate transition functions */
	void Set_MatchState(FName NewState);

// Training
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT(""));



};
