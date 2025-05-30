// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/RtWebManager.h"

#include "Kismet/GameplayStatics.h"
// Core
#include "RtCore/Public/RtJsonFunctionLib.h"

// Framework 
#include "Framework/RtPlayerController.h"
// Manager 
#include "Manager/RtConfig.h"
#include "Manager/RtGameManager.h"
#include "Manager/RtTrainingManager.h"
#include "Manager/RtReplayManager.h"
#include "Manager/RtReplayManagerActor.h"
#include "Manager/RtLoadingManager.h"
#include "Misc/Char.h"

// Network 
#include "Network/RtWorkerThread.h"
//Json, JsonUtilities
#include "Serialization/JsonSerializer.h" 
#include "JsonObjectConverter.h" 
// Unzip
//#include "ZipFileFunctionLibrary.h"

DEFINE_LOG_CATEGORY(LogWebManager);

void URtWebManager::Init_WebManager()
{
	bIsInitialized = true;

	// Init
	WebUrl = URtConfig::WebServerUrl;
	ScenarioUrl = URtConfig::WebScenarioUrl;
	WebLogUrl = URtConfig::WebLogUrl;
	ReplayUploadUrl = URtConfig::WebReplayUploadUrl;
	ReplayDownloadUrl = URtConfig::WebReplayDownloadUrl;


	GetDeviceInfo();
	GetUserInfo();
	GetAllTemplateList();
}

void URtWebManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> URtWebManager::SetRequestOptions(const FString& URL)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	Request->SetURL(URL);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));

	return Request;
}

FString URtWebManager::GetContentString(const TSharedRef<FJsonObject>& requestObj)
{
	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(requestObj, Writer);

	return RequestBody;
}

TSharedPtr<FJsonObject> URtWebManager::GetResultByDeserialize(FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (Response == nullptr || !bWasSuccessful)
		return nullptr;
	TSharedPtr<FJsonObject> ResultObject;


	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	// Try to convert string to json object. Output goes in RetJsonObject
	if (FJsonSerializer::Deserialize(jsonReader, JsonObject))
	{
		FString Result = JsonObject->GetStringField("result");
		TSharedRef<TJsonReader<>> resultReader = TJsonReaderFactory<>::Create(Result);
		if (FJsonSerializer::Deserialize(resultReader, ResultObject))
			return ResultObject;
	}

	return ResultObject;
}

void URtWebManager::Login(const FString& InID, const FString& InPassword)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UserRequest = SetRequestOptions(WebUrl);
	UserRequest->OnProcessRequestComplete().BindUObject(this, &URtWebManager::OnComplete_Login);

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("protocol", FString::FromInt((int32)ERtWebProtocolType::webLogin));
	RequestObj->SetStringField("account", InID);
	RequestObj->SetStringField("pass", InPassword);

	UserRequest->SetContentAsString(GetContentString(RequestObj));
	UserRequest->ProcessRequest();

	UE_LOG(LogWebManager, Warning, TEXT("Login Request Called"));
}

void URtWebManager::OnComplete_Login(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> ResultObject;
	ResultObject = GetResultByDeserialize(Response, bWasSuccessful);
	if (ResultObject.IsValid()) {

		FString Result = ResultObject->GetStringField("result");
		TSharedRef<TJsonReader<>> resultReader = TJsonReaderFactory<>::Create(Result);
		Handle_LoginCheck.Broadcast(FCString::Atoi(*Result));
	}
}

void URtWebManager::GetAllTemplateList()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UserRequest = SetRequestOptions(WebUrl);
	UserRequest->OnProcessRequestComplete().BindUObject(this, &URtWebManager::OnComplete_GetAllTemplateList);

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("protocol", FString::FromInt((int32)ERtWebProtocolType::webGetAllFileList));

	UserRequest->SetContentAsString(GetContentString(RequestObj));
	UserRequest->ProcessRequest();

	UE_LOG(LogWebManager, Warning, TEXT("Download All File Request Called"));
}

void URtWebManager::OnComplete_GetAllTemplateList(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Response, bWasSuccessful]()
		{

			TSharedPtr<FJsonObject> ResultObject;
			ResultObject = GetResultByDeserialize(Response, bWasSuccessful);

			if (ResultObject.IsValid()) {

				TArray<FString> sctfileNames;
				// INIT
				TArray<FString> downloadfileList;
				FString SavedFolder_Path;
				bool bOutReadResult;
				bool bOutWriteResult;
				FString ErrMsg;

				// Make version Path
#if WITH_EDITOR
				SavedFolder_Path = FPaths::ProjectDir();
#else
				SavedFolder_Path = FPaths::LaunchDir();
#endif
				SavedFolder_Path = FPaths::Combine(SavedFolder_Path, TEXT("TrainingData/Version/version.json"));

				// 1. Load Json : version.json
				TSharedPtr<FJsonObject> readJsonObject = MakeShareable(new FJsonObject);
				readJsonObject = URtJsonFunctionLib::ReadJson(SavedFolder_Path, bOutReadResult, ErrMsg);

				if (bOutReadResult)
				{
					UE_LOG(LogWebManager, Warning, TEXT("Version.json already exist, get old version table"));
					// 2-1. Ready for compare with new and old
					TArray<TSharedPtr<FJsonValue>> oldListArray = readJsonObject->GetArrayField(TEXT("LIST"));
					for (int32 i = 0; i < oldListArray.Num(); i++)
					{
						TSharedPtr<FJsonValue> JsonValue = oldListArray[i];
						TSharedPtr<FJsonObject> JsonValueObject = JsonValue->AsObject();

						FString fileName = JsonValueObject->GetStringField(TEXT("filename"));
						int32 fileversion = JsonValueObject->GetIntegerField(TEXT("filever"));

						oldVersionMap.Add(fileName, fileversion);
					}
				}
				else
				{
					// 2-2. Ready for download all files 
					UE_LOG(LogWebManager, Warning, TEXT("Version.json not exist, download all files"));
				}

				// 3. Get Web json and write: version table
				TArray<TSharedPtr<FJsonValue>> newListArray = ResultObject->GetArrayField(TEXT("LIST"));
				TArray<TSharedPtr<FJsonValue>> newobjArray;

				if (newListArray.Num() > 0)
				{
					for (int32 i = 0; i < newListArray.Num(); i++)
					{
						TSharedPtr<FJsonValue> JsonValue = newListArray[i];
						TSharedPtr<FJsonObject> JsonValueObject = JsonValue->AsObject();

						FString fileName = JsonValueObject->GetStringField(TEXT("filename"));
						int32 version = JsonValueObject->GetIntegerField(TEXT("ver"));
						int32 type = JsonValueObject->GetIntegerField(TEXT("filetype"));

						FRtFileVersion verstruct;
						verstruct.filename = fileName;
						verstruct.filever = version;
						verstruct.filetype = type;

						// 3-0. Add sctfileNames for scenario Select
						if (type == (int32)ERtWebFileType::ESCT) sctfileNames.Add(fileName);

						// 3-1. if have version.json, write downloadList array of filenames
						if (bOutReadResult)
						{
							if (oldVersionMap.FindRef(fileName) == 0)
							{
								// haven't templete. need download
								downloadList.Add(fileName, type);
								downloadfileList.Add(fileName);
							}
							else
							{
								// have templete
								if (oldVersionMap.FindRef(fileName) == version)
								{
									// same version, pass this code
								}
								else
								{
									// wrong version. need download
									downloadList.Add(fileName, type);
									downloadfileList.Add(fileName);
								}
							}
						}

						TSharedPtr<FJsonObject> newJsonObject = FJsonObjectConverter::UStructToJsonObject(verstruct);
						newobjArray.Add(MakeShareable(new FJsonValueObject(newJsonObject)));
					}

					// 3-2. write new version.json after make download list
					TSharedPtr<FJsonObject> newJsonFile = MakeShareable(new FJsonObject);
					newJsonFile->SetArrayField(TEXT("LIST"), newobjArray);
					URtJsonFunctionLib::WriteJson(SavedFolder_Path, newJsonFile, bOutWriteResult, ErrMsg);
				}


				// 4. Check read result and download depends on
				if (bOutReadResult)
				{
					UE_LOG(LogWebManager, Warning, TEXT("Version.json already exist"));
					downloadCheckList.Append(downloadfileList);
				}
				else
				{
					for (int32 i = 0; i < newListArray.Num(); i++)
					{
						TSharedPtr<FJsonValue> JsonValue = newListArray[i];
						TSharedPtr<FJsonObject> JsonValueObject = JsonValue->AsObject();

						FString fileName = JsonValueObject->GetStringField(TEXT("filename"));
						int32 type = JsonValueObject->GetIntegerField(TEXT("filetype"));
						downloadList.Add(fileName, type);
						downloadfileList.Add(fileName);
					}
					downloadCheckList.Append(downloadfileList);
				}

				for (int32 i = 0; i < downloadfileList.Num(); i++)
				{
					FString str_downfilename = downloadfileList[i];
					TSharedPtr<FJsonValue> JsonValue = MakeShareable(new FJsonValueString(str_downfilename));
					TSharedPtr<FJsonObject> JsonValueObject = JsonValue->AsObject();

					int32 downloadfiletype = downloadList.FindRef(str_downfilename);
					// File Down Load Code
					DownLoadTemplate(ScenarioUrl, str_downfilename, downloadfiletype);
				}

				// Update Scenario Name Array
				if (auto* gi = GetGameInstance()) {
					if (auto* mgr = gi->GetSubsystem<URtGameManager>()) {
						if (mgr->TrngManager) {
							mgr->TrngManager->Update_ScenarioArray(sctfileNames);
						}
					}
				}
			}
	});
}

void URtWebManager::Login_LogWeb()
{
	UE_LOG(LogWebManager, Warning, TEXT("Create Request Called of Log Web"));
}

void URtWebManager::CheckDownloadComplete(FString filename)
{
	// remove value from list
	if (downloadCheckList.Num() > 0)
	{
		downloadCheckList.Remove(filename);
	}

	// check complete
	if (downloadCheckList.IsEmpty())
	{
		FString EfkZipFilesPath;

#if WITH_EDITOR
		EfkZipFilesPath = FPaths::ProjectDir();
#else
		EfkZipFilesPath = FPaths::LaunchDir();
#endif

		EfkZipFilesPath = FPaths::Combine(EfkZipFilesPath, TEXT("TrainingData/EffectFiles/"));
		IFileManager& FileManager = IFileManager::Get();
		UnZipTargetPathList.Empty();
		FileManager.FindFilesRecursive(UnZipTargetPathList, *EfkZipFilesPath, TEXT("*.zip"), true, false);

		UnZippingCounter = 0;
		UnZipping();
	}
}





void URtWebManager::DownLoadTemplate(FString URL, FString fileName, int32 protocol)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> DownLoadRequest = SetRequestOptions(URL);
	DownLoadRequest->OnProcessRequestComplete().BindUObject(this, &URtWebManager::OnComplete_File);

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("filename", fileName);
	FString fileTypeInfoStr = FString::FromInt(protocol);
	RequestObj->SetStringField("filetype", fileTypeInfoStr);

	DownLoadRequest->SetContentAsString(GetContentString(RequestObj));
	DownLoadRequest->ProcessRequest();
}

void URtWebManager::GetUserInfo()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UserRequest = SetRequestOptions(WebUrl);
	UserRequest->OnProcessRequestComplete().BindUObject(this, &URtWebManager::OnComplete_GetUserInfo);

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("protocol", FString::FromInt((int32)ERtWebProtocolType::webGetUserList));

	UserRequest->SetContentAsString(GetContentString(RequestObj));
	UserRequest->ProcessRequest();

	UE_LOG(LogWebManager, Warning, TEXT("GetUserInfo Request Called"));
}

void URtWebManager::OnComplete_GetUserInfo(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> ResultObject;
	ResultObject = GetResultByDeserialize(Response, bWasSuccessful);

	if (ResultObject.IsValid()) {
		TArray<FRtTraineeInfo> TraineeInfos;
		TArray<TSharedPtr<FJsonValue>> PlayerArray = ResultObject->GetArrayField(TEXT("LIST"));
		if (PlayerArray.Num() > 0)
		{
			for (int32 i = 0; i < PlayerArray.Num(); i++)
			{
				TSharedPtr<FJsonValue> JsonValue = PlayerArray[i];
				TSharedPtr<FJsonObject> JsonValueObject = JsonValue->AsObject();

				FString PlayerName = JsonValueObject->GetStringField(TEXT("name"));
				int32 PlayerID = JsonValueObject->GetNumberField(TEXT("UID")); // WEB INDEX 1 ~ Int32
				FString PlayerNumber = JsonValueObject->GetStringField(TEXT("num"));
				float height = JsonValueObject->GetNumberField(TEXT("height"));

				// Create Player Information Data
				FRtTraineeInfo newPlayer;
				newPlayer.TraineeName = PlayerName;
				newPlayer.UID = PlayerID;
				newPlayer.TraineeSN = PlayerNumber;
				newPlayer.Traineeheight = height;

				// Add to Array
				TraineeInfos.Add(newPlayer);

				UE_LOG(LogWebManager, Warning, TEXT("User name is %s"), *PlayerName);
				UE_LOG(LogWebManager, Warning, TEXT("User Number is %s"), *PlayerNumber);
				UE_LOG(LogWebManager, Warning, TEXT("User ID is %d"), PlayerID);
				UE_LOG(LogWebManager, Warning, TEXT("User Height is %d"), height);
			}
		}

		Response_Data.UserArray = TraineeInfos;
		if (auto* gi = GetGameInstance()) {
			if (auto* mgr = gi->GetSubsystem<URtGameManager>()) {
				if (mgr->TrngManager) {
					mgr->TrngManager->Update_TraineeArray(Response_Data.UserArray);
				}
			}
		}
	}
}

void URtWebManager::GetDeviceInfo()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UserRequest = SetRequestOptions(WebUrl);
	UserRequest->OnProcessRequestComplete().BindUObject(this, &URtWebManager::OnComplete_GetDeviceInfo);

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("protocol", FString::FromInt((int32)ERtWebProtocolType::webGetMachineList));

	UserRequest->SetContentAsString(GetContentString(RequestObj));
	UserRequest->ProcessRequest();

	UE_LOG(LogWebManager, Warning, TEXT("GetDeviceInfo Request Called"));
}

void URtWebManager::OnComplete_GetDeviceInfo(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> ResultObject;
	ResultObject = GetResultByDeserialize(Response, bWasSuccessful);

	if (ResultObject.IsValid()) {
		TArray<FRtDeviceInfo> deviceInfos;
		TArray<TSharedPtr<FJsonValue>> DeviceArray = ResultObject->GetArrayField(TEXT("LIST"));
		if (DeviceArray.Num() > 0)
		{
			for (int32 i = 0; i < DeviceArray.Num(); i++)
			{
				TSharedPtr<FJsonValue> JsonValue = DeviceArray[i];
				TSharedPtr<FJsonObject> JsonValueObject = JsonValue->AsObject();

				FString DeviceName = JsonValueObject->GetStringField(TEXT("name"));
				FString DeviceIndex_str = JsonValueObject->GetStringField(TEXT("nID"));

				int32 DeviceIndex = FCString::Atoi(*DeviceIndex_str);

				// Create Device Information Data
				FRtDeviceInfo newDevice;
				newDevice.Name = DeviceName;
				newDevice.DeviceIndex = DeviceIndex; // Device UID

				// ADD to Array
				deviceInfos.Add(newDevice);
				UE_LOG(LogWebManager, Warning, TEXT("Device Set is %s"), *DeviceName);
			}
		}

		Response_Data.DeviceArray = deviceInfos;

		if (auto* gi = GetGameInstance()) {
			if (auto* mgr = gi->GetSubsystem<URtGameManager>()) {
				if (mgr->TrngManager) {
					mgr->TrngManager->Update_WebDeviceArray(Response_Data.DeviceArray);
				}
			}
		}
	}
}

void URtWebManager::GetMotionWeb(int32 TID, int32 UID)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UserRequest = SetRequestOptions(WebUrl);
	UserRequest->OnProcessRequestComplete().BindUObject(this, &URtWebManager::OnComplete_GetMotionWeb);

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("protocol", FString::FromInt((int32)ERtWebProtocolType::webGetMotionLog));
	RequestObj->SetStringField("TID", FString::FromInt(TID));
	RequestObj->SetStringField("UID", FString::FromInt(UID));

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);
	UserRequest->SetContentAsString(RequestBody);
	UserRequest->ProcessRequest();
}

void URtWebManager::OnComplete_GetMotionWeb(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> ResultObject;
	ResultObject = GetResultByDeserialize(Response, bWasSuccessful);

	if (ResultObject.IsValid()) {
		TArray<FString> DataResult_str;
		TArray<TSharedPtr<FJsonValue>> DataResult;
		DataResult = ResultObject->GetArrayField(TEXT("LIST"));
		if (DataResult.Num() > 0)
		{
			for (int i = 0; i < DataResult.Num(); i++)
			{
				TSharedPtr<FJsonValue> JsonValue = DataResult[i];
				TSharedPtr<FJsonObject> JsonValueObject = JsonValue->AsObject();

				FString motionstring = JsonValueObject->GetStringField(TEXT("str"));
				DataResult_str.Add(motionstring);

			}
		}

		auto* gi = GetGameInstance();
		if (gi) {
			repmgr = gi->GetSubsystem<URtReplayManager>();
			repmgr->Download_RepFile(DataResult_str);
		}
	}
}

void URtWebManager::GetVoiceList(int32 TID)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UserRequest = FHttpModule::Get().CreateRequest();

	UserRequest->OnProcessRequestComplete().BindUObject(this, &URtWebManager::OnComplete_GetVoiceList);
	UserRequest->SetURL(WebUrl);
	UserRequest->SetVerb("POST");
	UserRequest->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();

	RequestObj->SetStringField("protocol", FString::FromInt((int32)ERtWebProtocolType::webGetVoiceList));
	RequestObj->SetStringField("TID", FString::FromInt(TID));

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);
	UserRequest->SetContentAsString(RequestBody);
	UserRequest->ProcessRequest();
}

void URtWebManager::OnComplete_GetVoiceList(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> ResultObject;
	ResultObject = GetResultByDeserialize(Response, bWasSuccessful);

	if (ResultObject.IsValid()) {
		if (!repmgr) return;
		TArray<TSharedPtr<FJsonValue>> Voicelist;
		Voicelist = ResultObject->GetArrayField(TEXT("LIST"));

		auto* gi = GetGameInstance();
		if (gi) {
			repmgr = gi->GetSubsystem<URtReplayManager>();
			repmgr->voiceListNum = Voicelist.Num();       // check for download finish
			repmgr->voiceHandler.AddUFunction(this, TEXT("VoiceDownComplete"));
			for (int i = 0; i < Voicelist.Num(); i++)
			{
				TSharedPtr<FJsonValue> JsonValue = Voicelist[i];
				TSharedPtr<FJsonObject> JsonValueObject = JsonValue->AsObject();

				int32 listindex = JsonValueObject->GetIntegerField(TEXT("i"));

				// RepManager Get => Download Method 
				repmgr->Download_VoiceFiles(listindex);
			}
		}
	}
}


void URtWebManager::OnComplete_File(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Response]()
		{
			FString SavedFolder_Path;

			FString de_filename = Response.Get()->GetHeader("filename");
			std::string de_filename_str = TCHAR_TO_UTF8(*de_filename);
			std::string filename_str = GetBase64Decording(de_filename_str);
			FString filename = UTF8_TO_TCHAR(filename_str.c_str());
			FString filetype = Response.Get()->GetHeader("filetype");

#if WITH_EDITOR
			SavedFolder_Path = FPaths::ProjectDir();
#else
			SavedFolder_Path = FPaths::LaunchDir();
#endif

			switch ((ERtWebFileType)FCString::Atoi(*filetype))
			{
			case ERtWebFileType::ESCT:    SavedFolder_Path = FPaths::Combine(SavedFolder_Path, TEXT("TrainingData/MapFiles/"));    break;
			case ERtWebFileType::EMCT:    SavedFolder_Path = FPaths::Combine(SavedFolder_Path, TEXT("TrainingData/MapFiles/"));    break;
			case ERtWebFileType::EWCT:    SavedFolder_Path = FPaths::Combine(SavedFolder_Path, TEXT("TrainingData/MapFiles/"));    break;
			case ERtWebFileType::ETCT:    SavedFolder_Path = FPaths::Combine(SavedFolder_Path, TEXT("TrainingData/TctFiles/"));    break;
			case ERtWebFileType::EFBX:    SavedFolder_Path = FPaths::Combine(SavedFolder_Path, TEXT("TrainingData/FbxFiles/"));    break;
			case ERtWebFileType::EPNG:    SavedFolder_Path = FPaths::Combine(SavedFolder_Path, TEXT("TrainingData/PngFiles/"));    break;
			case ERtWebFileType::EWAV:    SavedFolder_Path = FPaths::Combine(SavedFolder_Path, TEXT("TrainingData/WavFiles/"));    break;
			case ERtWebFileType::FBXANIM: SavedFolder_Path = FPaths::Combine(SavedFolder_Path, TEXT("TrainingData/FbxAnimFiles/"));    break;
			case ERtWebFileType::EEFF:    SavedFolder_Path = FPaths::Combine(SavedFolder_Path, TEXT("TrainingData/EffectFiles/"));    break;
			default:
				return;
			}

			// Make Folder
			FString NewSavedPath = SavedFolder_Path + filename;
			FPaths::MakeStandardFilename(NewSavedPath);


			if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
			{
				IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
				// Make Dir tree 
				PlatformFile.CreateDirectoryTree(*SavedFolder_Path);
				IFileHandle* FileHandler = PlatformFile.OpenWrite(*NewSavedPath);


				if (FileHandler)
				{
					FileHandler->Write(Response->GetContent().GetData(), Response->GetContentLength());
					FileHandler->Flush();

					delete FileHandler;
				}
				else
				{
					UE_LOG(LogWebManager, Warning, TEXT("FileHandler is invalid!!"));
				}
			}
			else
			{
				UE_LOG(LogWebManager, Warning, TEXT("Response is invalid!!"));
			}
	});
}

void URtWebManager::GetTID(FString InScenario)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UserRequest = SetRequestOptions(WebUrl);
	UserRequest->OnProcessRequestComplete().BindUObject(this, &URtWebManager::OnComplete_GetTID);

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("protocol", FString::FromInt((int32)ERtWebProtocolType::webStart));
	RequestObj->SetStringField("TName", InScenario);
	UserRequest->SetContentAsString(GetContentString(RequestObj));
	UserRequest->ProcessRequest();

	UE_LOG(LogWebManager, Warning, TEXT("GetTID Request Called :: Training Start "));
}

void URtWebManager::OnComplete_GetTID(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> ResultObject;
	ResultObject = GetResultByDeserialize(Response, bWasSuccessful);

	if (ResultObject.IsValid()) {
		// Recv TID from web server , 
		int32 TrainingID = ResultObject->GetIntegerField(TEXT("TID"));

		// 2. TID Recv and Broad Cast Send Packet Function
		Handle_GetTID.Broadcast(TrainingID);
		Handle_GetTID.Clear();
	}
}

void URtWebManager::CheckTID(int32 TID)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UserRequest = SetRequestOptions(WebUrl);
	UserRequest->OnProcessRequestComplete().BindUObject(this, &URtWebManager::OnComplete_CheckTID);

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("protocol", FString::FromInt((int32)ERtWebProtocolType::webFinish));
	RequestObj->SetStringField("TID", FString::FromInt(TID));

	UserRequest->SetContentAsString(GetContentString(RequestObj));
	UserRequest->ProcessRequest();

	UE_LOG(LogWebManager, Warning, TEXT("CheckTID Request Called"));
}

void URtWebManager::OnComplete_CheckTID(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> ResultObject;
	ResultObject = GetResultByDeserialize(Response, bWasSuccessful);

	if (ResultObject.IsValid()) {
		int32 result = ResultObject->GetIntegerField(TEXT("RESULT"));
		if (CheckErrorCode((enErrorCode)result)) {
			//  Result relative coding
		}
	}
}

void URtWebManager::GetResult(int32 TID, int32 UID)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UserRequest = SetRequestOptions(WebUrl);
	UserRequest->OnProcessRequestComplete().BindUObject(this, &URtWebManager::OnComplete_GetResult);

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("protocol", FString::FromInt((int32)ERtWebProtocolType::webTrainingDetail));
	RequestObj->SetStringField("TID", FString::FromInt(TID));
	RequestObj->SetStringField("UID", FString::FromInt(UID));

	UserRequest->SetContentAsString(GetContentString(RequestObj));
	UserRequest->ProcessRequest();

	UE_LOG(LogWebManager, Warning, TEXT("Result Request Called"));
}

void URtWebManager::OnComplete_GetResult(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> ResultObject;
	ResultObject = GetResultByDeserialize(Response, bWasSuccessful);

	if (ResultObject.IsValid()) {

		FResponse_TrngSimpleResult resultstruct;

		resultstruct.Name = ResultObject->GetStringField(TEXT("UName"));
		resultstruct.Role = ResultObject->GetStringField(TEXT("Job"));
		resultstruct.FireCnt = ResultObject->GetStringField(TEXT("FireCount"));
		resultstruct.HitCnt = ResultObject->GetStringField(TEXT("HitCount"));
		resultstruct.ChallengeCnt = ResultObject->GetStringField(TEXT("ChallengeCount"));
		resultstruct.SurrenderCnt = ResultObject->GetStringField(TEXT("SurrenderCount"));
		resultstruct.BindCnt = ResultObject->GetStringField(TEXT("BindCount"));
		resultstruct.CommandMoveCnt = ResultObject->GetStringField(TEXT("CommandMoveCount"));
		resultstruct.BreifCnt = ResultObject->GetStringField(TEXT("BreifCount"));
		resultstruct.CallCnt = ResultObject->GetStringField(TEXT("CallCount"));
		resultstruct.EODCnt = ResultObject->GetStringField(TEXT("EodCount"));
		resultstruct.BadFireCnt = ResultObject->GetStringField(TEXT("BadFireCount"));
		resultstruct.PlayTimeSec = ResultObject->GetStringField(TEXT("PlayTime"));

		resultstruct.Distance = ResultObject->GetStringField(TEXT("Distance"));

		// Execute
		Handle_GetResult.Broadcast(resultstruct);
	}
}

void URtWebManager::EnterTrainee(int32 TID, int32 UID, int32 MID, FString Role)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UserRequest = SetRequestOptions(WebUrl);
	UserRequest->OnProcessRequestComplete().BindUObject(this, &URtWebManager::OnComplete_CheckTID);

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("protocol", FString::FromInt((int32)ERtWebProtocolType::webEnterUser));
	RequestObj->SetStringField("TID", FString::FromInt(TID));
	RequestObj->SetStringField("UID", FString::FromInt(UID));
	RequestObj->SetStringField("MID", FString::FromInt(MID));
	RequestObj->SetStringField("Job", Role);

	UserRequest->SetContentAsString(GetContentString(RequestObj));
	UserRequest->ProcessRequest();

	UE_LOG(LogWebManager, Warning, TEXT("EnterTrainee Request Called"));
}

void URtWebManager::OnComplete_EnterTrainee(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// TODO
}

void URtWebManager::SendLog(const ELogProtocol& protocol, int32 TID, int32 UID)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UserRequest = SetRequestOptions(WebLogUrl);
	UserRequest->OnProcessRequestComplete().BindUObject(this, &URtWebManager::OnComplete_SendLog);

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("protocol", FString::FromInt((int32)protocol));

	RequestObj->SetStringField("TID", FString::FromInt(TID));
	RequestObj->SetStringField("UID", FString::FromInt(UID));

	UserRequest->SetContentAsString(GetContentString(RequestObj));
	UserRequest->ProcessRequest();

	UE_LOG(LogWebManager, Warning, TEXT("SendLog Request Called"));
}

void URtWebManager::SendLogDistance(int32 TID, int32 UID, float distance)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UserRequest = SetRequestOptions(WebLogUrl);
	UserRequest->OnProcessRequestComplete().BindUObject(this, &URtWebManager::OnComplete_SendLog);

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("protocol", FString::FromInt((int32)ELogProtocol::LOG_DISTANCE));

	RequestObj->SetStringField("TID", FString::FromInt(TID));
	RequestObj->SetStringField("UID", FString::FromInt(UID));
	RequestObj->SetStringField("dist", FString::SanitizeFloat(distance));

	UserRequest->SetContentAsString(GetContentString(RequestObj));
	UserRequest->ProcessRequest();

	UE_LOG(LogWebManager, Warning, TEXT("SendLog Request Called"));
}

void URtWebManager::SendReplayMotion(int32 TID, FString DataString)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UserRequest = SetRequestOptions(WebLogUrl);
	UserRequest->OnProcessRequestComplete().BindUObject(this, &URtWebManager::OnComplete_SendLog);

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("protocol", FString::FromInt((int32)ELogProtocol::LOG_MOTION));
	RequestObj->SetStringField("TID", FString::FromInt(TID));
	RequestObj->SetStringField("motion", DataString);

	UserRequest->SetContentAsString(GetContentString(RequestObj));
	UserRequest->ProcessRequest();

	UE_LOG(LogWebManager, Warning, TEXT("ReplayMotion Request Called"));

}

void URtWebManager::OnComplete_SendLog(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// TODO
}

void URtWebManager::GetTraineeList(int32 TID)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UserRequest = SetRequestOptions(WebUrl);
	UserRequest->OnProcessRequestComplete().BindUObject(this, &URtWebManager::OnComplete_GetTraineeList);

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("protocol", FString::FromInt((int32)ERtWebProtocolType::webTrainingUserList));
	RequestObj->SetStringField("TID", FString::FromInt(TID));

	UserRequest->SetContentAsString(GetContentString(RequestObj));
	UserRequest->ProcessRequest();

	UE_LOG(LogWebManager, Warning, TEXT("GetTraineeList Request Called"));
}

void URtWebManager::OnComplete_GetTraineeList(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> ResultObject;
	ResultObject = GetResultByDeserialize(Response, bWasSuccessful);

	if (ResultObject.IsValid()) {

		if (!repmgr) return;
		TArray<TSharedPtr<FJsonValue>> UserListArray = ResultObject->GetArrayField(TEXT("LIST"));

		TArray<FString> usernameArr;
		TArray<FString> userroleArr;
		TArray<int32> userIDArr;

		for (TSharedPtr<FJsonValue> UserInfo : UserListArray) {
			TSharedPtr<FJsonObject> UserInfoObject = UserInfo->AsObject();

			FString username = UserInfoObject->GetStringField(TEXT("name"));
			FString userrole = UserInfoObject->GetStringField(TEXT("JOb"));
			int32 UID = UserInfoObject->GetIntegerField(TEXT("UID"));

			usernameArr.Add(username);
			userroleArr.Add(userrole);
			userIDArr.Add(UID);
		}
		repmgr->ReplayInfo.SetArray(userroleArr, usernameArr, userIDArr);
		repmgr->userHandler.Broadcast();
	}
}


void URtWebManager::VoiceDownComplete()
{
	// Voice execute 
	if (!repmgr) return;
	TArray<FString> VoiceFilePathList;


	if (auto* gi = GetGameInstance()) {

		repmgr = gi->GetSubsystem<URtReplayManager>();
		// List Up
		VoiceFilePathList = repmgr->ListAllWavFilesInFolder();

		LoadedSoundWaves.Empty();

		for (auto FilePath : VoiceFilePathList) {
			LoadedSoundWaves.Add(repmgr->LoadSoundWave(FilePath));
		}

		USoundWave* MixedSoundWave = repmgr->MixSoundWaves(LoadedSoundWaves);
		
		int32 TID = repmgr->ReplayInfo.SelectedTID;
		int32 UID = repmgr->ReplayInfo.SelectedUID;

		// Download Replay Data Code
		// GetMotionWeb(TID, UID); // 웹으로부터 받은 데이터는 빈 값이다.
	}
}

bool URtWebManager::CheckErrorCode(const enErrorCode& errorcode)
{
	switch ((int32)errorcode)
	{
	case 1:
		UE_LOG(LogWebManager, Log, TEXT("Success"));
		return true;
		break;
	case 0:
		UE_LOG(LogWebManager, Error, TEXT("Fail"));
		return false;
		break;
	case -1:
		UE_LOG(LogWebManager, Error, TEXT("Error : Duplecate account")); 
		return false;
		break;
	case -2:
		UE_LOG(LogWebManager, Error, TEXT("Error : No exist account"));
		return false;
		break;
	case -4:
		UE_LOG(LogWebManager, Error, TEXT("Error : Wrong password"));
		return false;
		break;
	case -5:
		UE_LOG(LogWebManager, Error, TEXT("Error : Duplecate Trainee Server Number"));
		return false;
		break;
	case -6:
		UE_LOG(LogWebManager, Error, TEXT("Error : Duplecate Trainee"));
		return false;
		break;
	case -7:
		UE_LOG(LogWebManager, Error, TEXT("Error : Duplecate Role"));
		return false;
		break;
	case -40:
		UE_LOG(LogWebManager, Error, TEXT("Error : No exist Training ID Number"));
		return false;
		break;
	case -41:
		UE_LOG(LogWebManager, Error, TEXT("Error : Same Result exist already"));
		return false;
		break;
	case -42:
		UE_LOG(LogWebManager, Error, TEXT("Error : No exist Trainee ID"));
		return false;
		break;
	case -43:
		UE_LOG(LogWebManager, Error, TEXT("Error : No exist Device ID"));
		return false;
		break;
	default:
		return true;
		break;
	}
}

std::string URtWebManager::GetBase64Decording(std::string encoded_string)
{
	const std::string base64_chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4] = { 0, }, char_array_3[3] = { 0, };

	std::string ret;

	while (in_len-- && (encoded_string[in_] != '=') && IsBase64(encoded_string[in_]))
	{
		char_array_4[i++] = encoded_string[in_];
		in_++;

		if (i == 4)
		{
			for (i = 0; i < 4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i]; i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 4; j++)
			char_array_4[j] = 0;

		for (j = 0; j < 4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++)
			ret += char_array_3[j];
	}

	return ret;
}

bool URtWebManager::IsBase64(uint8 c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}


URtConfig* URtWebManager::Get_Config()
{
	return GetGameInstance()->GetSubsystem<URtConfig>();
}

// 7zip 플러그인이 5.3버전에서 에러를 발생시켜서 임시 제거
void URtWebManager::UnZipping()
{
	//if (UnZippingCounter < UnZipTargetPathList.Num()) {
	//	FString unZipTargetPath = UnZipTargetPathList[UnZippingCounter++];
	//	UZipFileFunctionLibrary::UnzipWithLambda(unZipTargetPath,
	//		[&]() {
	//			UnZipping();
	//		},
	//		nullptr,
	//		EZipUtilityCompressionFormat::COMPRESSION_FORMAT_ZIP);
	//}
}


void URtWebManager::Set_ReplayManager(URtReplayManager* Inthis)
{
	repmgr = Inthis;
}
