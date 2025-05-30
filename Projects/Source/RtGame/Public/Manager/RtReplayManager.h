// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
// Manager
#include "Manager/RtWebManager.h"
// Protocol
#include "Common/RtWebCommon.h"


#include "RtReplayManager.generated.h"

class UReplaySubsystem;
class UAudioComponent;
class ARtReplayTrainee;

RTGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogReplayManager, Log, All);
RTGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogDemoNetDriver, Log, All);

DECLARE_MULTICAST_DELEGATE(FVoiceDownLoadDele);
DECLARE_MULTICAST_DELEGATE(FGetTrainingUserListDele);
DECLARE_MULTICAST_DELEGATE_OneParam(FSetAudioComp, const FString& );



/**
 * 
 */

USTRUCT(BlueprintType)
struct FRtReplayStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Replay) FString SelectedScenarioName;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Replay) FString SelectedScenarioFolderPath;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Replay) int32 SelectedTID;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Replay) int32 SelectedUID;

	TArray<int32> UserIDs;
	TArray<FString> TraineeRoles;
	TArray<FString> TraineeNames;

	FRtReplayStruct() {
		SelectedScenarioName = TEXT("");
		SelectedScenarioFolderPath = TEXT("");
		SelectedTID = 0;
	}

	void SetArray(const TArray<FString>& InArrayRole, const TArray<FString>& InArrayName , const TArray<int32>& InArrayUID) {
		TraineeNames = InArrayName;
		TraineeRoles = InArrayRole;
		UserIDs = InArrayUID;
	}

	TArray<FString> GetNames() { return TraineeNames; }
	TArray<FString> GetRoles() { return TraineeRoles; }
	TArray<int32>   GetUIDs()  { return UserIDs;      }
};

UCLASS()
class RTGAME_API URtReplayManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	FString ReplayUrl_upload;
	FString ReplayUrl_download;
	bool bIsReplay = false; // Voice 처럼 리플레이인지 아닌지 startmode 쪽에서 수정을 해야할 듯, 기본값은 false
	

public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere) FRtReplayStruct ReplayInfo;

	// check download fin
	int32 voiceListNum;

	void InitManager();

	void ClearSCT();

	// Conv
	TArray<uint8> FStringToUint8(const FString& InString);

	// Handler
	FVoiceDownLoadDele voiceHandler;
	FGetTrainingUserListDele userHandler;

	// Set for Replay
	UAudioComponent* MixedAudioComp;
	FSetAudioComp SetCompHandler;
	FString PlayedFilePath; // Voice
	FString MotionFilePath; // Motion Data 
	int32 MaxMotionLength;  // for Slider

	TMap<int32, FString> PlayerReplayWavFullPathMap; // UID, wav 파일 FullPath
 
	class URtWebManager* WebManager;
	URtWebManager* Get_WebManager() { return WebManager; }
	
	ARtReplayTrainee* FindRepTrainee(int32 uid);

	void ClearAllCameraCaptureTextureTarget();

	// Send
	/*
	* TID,UID 는 Request Header 용
	* FilePath 는 Voice Manager 에서 저장된 후 넘어올 것
	*/
	void Upload_VoiceFile(FString TID, FString UID, FString FileName);
	// Local Data Save 
	/*
	* 실행 자체를 webManager 에서 리스트를 받고 할 것
	*/
	void Download_VoiceFiles(int index);
	void Download_RepFile(TArray<FString> ReplayDataString);


	// Recv 
	void OnResponse_Upload_File(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnResponse_Download_File(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void SetWavFullPathMap(FString fullPath);


	// Wav Import
	class USoundWave* LoadSoundWave(const FString& FolderPath);
	UFUNCTION(BlueprintCallable)
	TArray<FString> ListAllWavFilesInFolder();

    // Load 
	void LoadSCTDatas();

	USoundWave* MixSoundWaves(const TArray<USoundWave*>& SoundWaves);


	// TEST Code
	UFUNCTION() void Finish_Conv();
	UFUNCTION() void Finish_Import(class URuntimeAudioImporterLibrary* Importer, class UImportedSoundWave* ImportedSoundWav, enum ERuntimeImportStatus Status);
	UFUNCTION() void Finish_Export();


	// encoding
	std::string GetBase64Decording(std::string encoded_string);
	bool IsBase64(uint8 c);

	void ClearAllTraineeCaptureTextureTarget();
	void ClearTopViewCameraCaptureTextureTarget();

};
