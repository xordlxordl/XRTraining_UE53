// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
// Http
#include "http.h"
// Protocol
#include "Common/RtWebCommon.h"
#include "RtCore/Public/Common/RtProtocol.h"
// Data
#include "Common/RtRpcCommon.h"


#include "RtWebManager.generated.h"

RTGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogWebManager, Log, All);

DECLARE_MULTICAST_DELEGATE(FRecvHandleWeb);
DECLARE_MULTICAST_DELEGATE_OneParam(FRecvHandleResultOneParam, const FResponse_TrngSimpleResult&);
DECLARE_MULTICAST_DELEGATE_OneParam(FDownloadCompleteHandle, FString filename);

// Delegate renew
DECLARE_MULTICAST_DELEGATE_OneParam(FRecvHandleLoginCheck, int32 Result);
DECLARE_MULTICAST_DELEGATE_OneParam(FRecvHandleGetTID, int32 TID);
DECLARE_MULTICAST_DELEGATE_OneParam(FRecvHandleGetResult, const FResponse_TrngSimpleResult&);

class ARtPlayerController;
class URtConfig;

UENUM(BlueprintType)
enum class ERtWebFileType : uint8
{
	ENone = 0,
	ESCT,
	EMCT,
	EWCT,
	ETCT,
	EFBX,
	EPNG,
	EWAV,
	FBXANIM,
	EEFF,
	MAX_FILE,
};

USTRUCT()
struct FRtFileVersion
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString filename;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int32 filever = 0;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int32 filetype = 0;
};

/**
 * 
 */
UCLASS()
class RTGAME_API URtWebManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	bool bIsInitialized = false;
// Various
public:

	FHttpModule* Http;
	FHttpModule* DownloadHttp;

	FString WebUrl = "";
	FString ScenarioUrl = "";
	FString ReplayUploadUrl = "";
	FString ReplayDownloadUrl = "";
	FString WebLogUrl = "";

	FRecvHandleWeb Dele_RecvHandle;

	// delegate renew handle
	FRecvHandleLoginCheck Handle_LoginCheck;
	FRecvHandleGetTID Handle_GetTID;
	FRecvHandleGetResult Handle_GetResult;

	UPROPERTY(BlueprintReadOnly)
	TMap<FString, int32> oldVersionMap; // <filename, version>

	UPROPERTY(BlueprintReadOnly)
	TMap<FString, int32> downloadList;  // <filename, protocol>
	TArray<FString> downloadCheckList;

	// Replay
	TArray<class USoundWave*> LoadedSoundWaves;
	class URtReplayManager* repmgr;

private:
	FResponse_Login_Result ResponseResult_Login;
	FResponse_Data Response_Data;
	FResponse_Result Response_Result;

// Function
public:
    // Flow
	bool IsInitialized() { return bIsInitialized; }
	void Init_WebManager();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> SetRequestOptions(const FString& URL);
	FString                                       GetContentString(const TSharedRef<FJsonObject>& requestObj);
	TSharedPtr<FJsonObject>                       GetResultByDeserialize(FHttpResponsePtr Response, bool bWasSuccessful);
	
	void Login_LogWeb();
	UFUNCTION(BlueprintCallable)
	void CheckDownloadComplete(FString filename);
	UFUNCTION(BlueprintCallable)
	void VoiceDownComplete();
	
	// NewFlow
	// Control to Web Login
	void Login(const FString& InID, const FString& InPassword);
	void OnComplete_Login(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	// Get Init Data from Web
	void GetUserInfo();
	void OnComplete_GetUserInfo(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void GetDeviceInfo();
	void OnComplete_GetDeviceInfo(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	// Template Download
	void GetAllTemplateList();
	void OnComplete_GetAllTemplateList(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void DownLoadTemplate(FString URL, FString fileName, int32 protocol);
	void OnComplete_File(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	// Training Flow
	void GetTID(FString InScenario);      // When Training Start
	void OnComplete_GetTID(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void CheckTID(int32 TID);             // When Training End
	void OnComplete_CheckTID(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void GetResult(int32 TID, int32 UID); // When Click Trainee on TraineeListBlocks
	void OnComplete_GetResult(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	// Trainee Flow
	void EnterTrainee(int32 TID, int32 UID, int32 MID, FString Role);
	void OnComplete_EnterTrainee(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void SendLog(const ELogProtocol& protocol, int32 TID, int32 UID);
	void SendLogDistance(int32 TID, int32 UID, float distance);
	void SendReplayMotion(int32 TID, FString DataString);
	void OnComplete_SendLog(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	// Replay::RepFile = Voice , Motion
	void GetTraineeList(int32 TID);      // Trainee List of Selected Training by TID
	void OnComplete_GetTraineeList(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void GetMotionWeb(int32 TID, int32 UID); 
	void OnComplete_GetMotionWeb(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void GetVoiceList(int32 TID);
    void OnComplete_GetVoiceList(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	bool CheckErrorCode(const enErrorCode& errorcode);

	std::string GetBase64Decording(std::string encoded_string);
	bool IsBase64(uint8 c);
 	// Get 
	URtConfig* Get_Config();

	// Set
	void Set_ReplayManager(URtReplayManager* Inthis);

	// UnZip
	int32 UnZippingCounter;
	TArray<FString> UnZipTargetPathList;
	void UnZipping();



};
