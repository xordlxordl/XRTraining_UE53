// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Http.h"
#include "RtGameInstance.generated.h"

enum class ERtWebProtocol: int32
{
	EWebErrorServerCheck = -5,
	EWebErrorBlock = -4,
	EWebError = -3,
	EWebErrorDup = -2,
	EWebErrorParse = -1,
	EWEB_PROTOCOL_START = 0,
	//-----------------------------------------------------

	EWebGetSCTList,
	EWebGetMCTList,
	EWebGetWCTList,
	EWebGetTCTList,
	EWebGetFBXList,
	EWebGetPNGList,
	EWebGetWAVList,
	EWebGetFBXANIMList,
	EWebGetEFKZipList,
	EWebGetAllFileList,

	//-----------------------------------------------------

	EWebLogin,

};

enum class ERtWebProtocolFileType : int32
{
	ENone = 0,
	ESCT,
	EMCT,
	EWCT,
	ETCT,
	EFBX,
	EPNG,
	EWAV,
	EFBXANIM,
	EEFK_ZIP,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWindowCloseRequestedMessage);


/**
 * 
 */
UCLASS()
class RTCORE_API URtGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init() override;
	virtual void Shutdown() override;

	UPROPERTY(BlueprintAssignable, Category = "URtGameInstance")
	FOnWindowCloseRequestedMessage OnWindowCloseReqMsg;
	
	DECLARE_EVENT(URtGameInstance, FCompleteEfkZipDownloadMessage);
	FCompleteEfkZipDownloadMessage CompleteEfkZipDownloadMessage;

	// dave
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnNetrokFailure, ENetworkFailure::Type);
	FOnNetrokFailure OnNetworkFailure_Delegate;

	TArray<FString> EfkZipFileList;

	FHttpModule* Http;

	int32 TotalSize;

	FString ProjectBaseFilePath;
	FString DownloadURL;
public:

	UFUNCTION(BlueprintCallable)
	void BindWindowCloseRequestAction();

	UFUNCTION(BlueprintCallable)
	bool OnWindowCloseRequested();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// 서버에 파일 업로드 기능 ////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	void UploadOneFile(FString URL, FString FilePath);

	bool UploadFiles(FString URL, TArray<FString> filePathList);

	bool UploadSearchFiles(FString URL, FString searchFileBasePath, FString extName);

	TArray<uint8> FStringToUint8(const FString& InString);

	void OnResponseUpLoadProgress(FHttpRequestPtr Request, int32 SendByte, int32 RecvByte);
	void OnResponseUpLoad(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// 서버 파일 다운로드 기능 ////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::string GetBase64Decording(std::string encoded_string);
	bool IsBase64(uint8 c);

	void DownloadOneFile(FString URL, FString fileName, ERtWebProtocolFileType typeInfo);

	void DownloadFiles(FString URL_Download, FString URL_MainServer, FString projectBaseFilePath);

	//void OnResponseProgress(FHttpRequestPtr Request, int32 SendByte, int32 RecvByte);
	void OnResponseDownLoad(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// 서버로 요청하기.
	void RequestToWebServer(FString URL, ERtWebProtocol protocal);
	// 서버의 응답받기.
	void OnResponseReceivedFromWebServer(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

protected:
	void HandleNetworkFailure(UWorld* InWorld, UNetDriver* InNetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

private:
	FDelegateHandle NotifyNetworkFailureDelegateHandle;
};
