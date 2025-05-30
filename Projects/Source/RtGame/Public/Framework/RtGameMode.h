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
	// GENERATED_BODY 를 UCLASS 바디로 교체 해주어야 초기화 오브젝트 인자를 사용한 생성자를 사용 가능
// Delegate
public:
	//login delegate
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPostLogin, AGameModeBase* /*GameMode*/, AController* /*NewPlayer*/);
	FOnPostLogin OnPostLogin_Delegate;

	//Player Delegate
	DECLARE_MULTICAST_DELEGATE(Dele_Player_Connected);
	static Dele_Player_Connected Player_Connected_Delegate;

// 변수
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

	static int32 Static_Count;//``login 되는 클라이언트 수 count 이다. 
	TArray<ARtPlayerController*> Trainee_Array;
	ARtPlayerController* Control_Controller;
	TArray<ARtPlayerController*> Trainer_Array;
	//FRtTrngResult TrngResult; //trainig result 

	// Login 
	//''로그인 과정에 발생하는 정보들을 가능한 모두 저장한다. 예외상황 처리에 필요한데. 훈련중에 접속하는 경우와 서버가 꺼진 상태에 제접속시키는 작업에 사용한다. 
    //''수시로 SaveGame 파일에 저장 하는 기능 작업이 필요하다.
	TArray<FRtUserInfo_PreLogin> Game_PreLogin;
	TArray<FRtUserInfo_PreLogin> Others_PreLogin;
	TArray<FRtUserInfo_PostLogin> Game_PostLogin;
	TArray<FRtUserInfo_PostLogin> Others_PostLogin;

	TMap<FString, FRtUserInfo_PreLogin> Lobby_Login_Try_ClientList;
	TMap<FString, FRtUserInfo_PreLogin> Game_Login_Try_ClientList;

	// Boolean 
	bool bAutoPlayType = false;
	bool Pause_RemainTime = false;
	bool Reject_AllClient_Event = false; 	//''모든 client 접속을 막기위해 사용

protected:
	FTimerHandle TimerHandle_DefaultTimer;


// 함수

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
	void Confirm_TraineeAassignment(int32 InId);//``디바이스에 훈련자 할당 작업이 끝나다.
	void Confirm_TraineeAassignment(bool bSync);
	void Confirm_TraineeDeployment(int32 InId);

	// dave
	void Confirm_BackConfirmation(int32 InId);
	void RequestResetTraining(int32 InId);

// Custom Trainning Flow
	//``Pre login 시 발생 정보를 저장한다. 현재 로비가 없지만, 추후 로비 모드로 변경시 사용 할 것이다. 
	bool Pre_Login(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage, ERt_GameMode InGameMode);
	//``Post login 시 발생 데이터를 저장한다. 추후 로비 모드로 변경시 사용 할 것이다. 
	void Post_Login(APlayerController* InPlayerController, ERt_GameMode InGameMode);

	//''Pre login 시 발생한 정보를 얻는다.
	bool Get_PreLoginInfo(ARtPlayerController* InPlayerController, FRtUserInfo_PreLogin& OutPreLoginInfo);

	//``Prelogin 에서 기록된 PlayType 을 얻는디
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
	

	// 제거예정
	//void DefaultTimer();

    bool Check_PreLoginIntegrity(const FRtUserInfo_PreLogin& InInfo); // 접속한 유저정보 정보 체크하여 오류판별

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
