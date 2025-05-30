// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"



//Data 
#include "RtCore/Public/Common/RtCommon.h"
#include "Saved/RtSaveGame.h"
#include "Common/RtModeSettingData.h"
 
#include "RtGameManager.generated.h"

RTGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogPlayerInfo, Log, All);
RTGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogGameManager, Log, All);




// Framework
class ARtPlayerController;
class ARtPlayerState;
class URtTrainingManager;
class URtReplayManager;
class ARtGameState;

// Manager
class URtTableManager;

// Data
class URtModeSettingData;

// Delegate
DECLARE_MULTICAST_DELEGATE(FOnInitialized_Delegate)
/**
 * 
 */
UCLASS()
class RTGAME_API URtGameManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
// Various
public:

	FOnInitialized_Delegate OnInitialized_Delegate;

UPROPERTY(BlueprintReadWrite)
    URtTrainingManager* TrngManager;

	// Save
	FString SaveData_SlotName;
	int32 SaveData_UserIndex;

	// Solo : Test
	bool bRun_StartMode = false;

	TObjectPtr<URtModeSettingData> ModeDefinition;
private:
	// boolean
	bool Is_ListenServer = false;
	bool Is_SoloPlay;
	bool bInitialized = false;
	bool bTrainingRestartProcess; // restart status check

	FString NickName;
	ERt_GameMode Current_GameMode = ERt_GameMode::E_None;

	ERt_PlayType Play_Type = ERt_PlayType::E_None;
	ERt_PlayType Start_Play_Type = ERt_PlayType::E_None; // 최초 실행시 play type


	// Save
	struct FRtPlayerSaveData Player_SaveData;
// Fucntions
public:
	URtGameManager();

	TArray<IConsoleObject*> AuditCmds;

	// Main Flow
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	FDelegateHandle CallAndRegister_OnInitialized(FOnInitialized_Delegate::FDelegate InDelegate);
	UFUNCTION(BlueprintPure) bool IsInitialzied();

	void Init_Game(const FString& MapName, const FString& Options, FString& ErrorMessage, bool bGamemode);

	bool Is_Play_Type(ERt_PlayType InType) { return Play_Type == InType; }
	
	void Start_PlayMode();

	UFUNCTION(BlueprintCallable)
	void BPCall_StartMode();

	UFUNCTION()
	void HandleLoadCompleted();

	// Server
	UFUNCTION(BlueprintCallable)
	void Host_Game();  // listen server 를 시작한다.

	UFUNCTION(BlueprintCallable)
	void Start_Server();
	UFUNCTION(BlueprintCallable)
	void Start_Client(const FString& InDeviceName, const FString& InDeviceID);

	UFUNCTION(BlueprintCallable)
	void Start_ControlMode();

	UFUNCTION(BlueprintCallable)
	void Start_TrainingMode();

	UFUNCTION(BlueprintCallable)
	void Start_ReplayMode(FString TID, FString UID, FString SCN);
	
	UFUNCTION(BlueprintCallable)
	void Start_SingleMode();

	void Quit_Program();

	UFUNCTION()
		void OnCreateSessionCompleted(bool Successful);
	
	//MatchState
	void HasStarted_MatchState(const FName& InState);

	// dave
	void HasExited_MatchState(const FName& InState);
	

	// Costom GetSet
	ERt_GameMode Get_Current_GameMode() { return Current_GameMode; }
	ERt_PlayType Get_Play_Type() { return Play_Type; }
	ERt_PlayType Get_Start_Play_Type() { return Start_Play_Type; }
	ARtPlayerController* Get_LocalPlayerController();
    URtTableManager* Get_TableManager();
	URtReplayManager* Get_ReplayManager();


	// Save
	void Save_PlayerData(const FRtPlayerSaveData& player_data);
	FRtPlayerSaveData Load_SavedPlayerData();

	// XR
	static bool IsHeadMountedDisplayEnabled();
	static bool IsXR();
private:
	void Create_Session();// listen server 를 시작한다.

	void Check_Start_PlayMode(); // 최초 실행시 playtype 을 검사한다.








// Encoding
	std::string GetBase64Decording(std::string encoded_string);
	bool IsBase64(uint8 c);

	// dave
	void OnNetworkFailure(ENetworkFailure::Type FailureType);
};
