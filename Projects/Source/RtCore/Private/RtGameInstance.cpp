// Fill out your copyright notice in the Description page of Project Settings.


#include "RtGameInstance.h"
#include "RtDefines.h"

#include "Misc/MessageDialog.h"
#include "Misc/OutputDeviceFile.h"

#include "Runtime/Online/HTTP/Public/Http.h"
#include "Interfaces/IHttpRequest.h"

#include "Kismet/KismetSystemLibrary.h"

void URtGameInstance::Init()
{
	Super::Init();

	FText text = FText::FromString(FString::Printf(TEXT("%s %s"), BUILD_TEMPORARY ? PROJECT_TEMPORARY : PROJECT_NAME, PROJECT_VERSION));
	UKismetSystemLibrary::SetWindowTitle(text);

	// dave
	if (GEngine != nullptr)
	{
		NotifyNetworkFailureDelegateHandle = GEngine->OnNetworkFailure().AddUObject(this,
			&URtGameInstance::HandleNetworkFailure);
	}

	//FCoreDelegates::OnExit.AddUObject(this, &URtGameInstance::HandleOnExit);
	//FCoreDelegates::OnPreExit.AddUObject(this, &URtGameInstance::HandleOnPreExit);
	//FCoreDelegates::ApplicationWillTerminateDelegate.AddUObject(this, &URtGameInstance::ApplicationWillTerminateDelegate);

//	FString baseDir;
//
//#if WITH_EDITOR
//	baseDir = FPaths::ProjectDir();
//#else
//	baseDir = FPaths::LaunchDir();
//#endif

	//FString LogFilename = baseDir + TEXT("RtGameInstanceLog.log");
	//FOutputDeviceFile* OutputDevice = new FOutputDeviceFile(*LogFilename);
	//GLog->AddOutputDevice(OutputDevice);
	//UE_LOG(RtGameInstanceLog, Log, TEXT("Custom log message"));

}

// dave
void URtGameInstance::Shutdown()
{
	if (GEngine != nullptr)
	{
		GEngine->OnNetworkFailure().Remove(NotifyNetworkFailureDelegateHandle);
	}
	OnNetworkFailure_Delegate.RemoveAll(this);
	Super::Shutdown();
}


// ������ ���� �޽����� ����ä��.
void URtGameInstance::BindWindowCloseRequestAction()
{
	UGameViewportClient* viewPortClient = GetGameViewportClient();
	if (viewPortClient)
	{
		viewPortClient->OnWindowCloseRequested().BindUFunction(this, "OnWindowCloseRequested");
	}
}

bool URtGameInstance::OnWindowCloseRequested()
{
	APP_LOG("OnWindowCloseRequested()~")
	//UE_LOG(RtGameInstanceLog, Log, TEXT("OnWindowCloseRequested()~"));

	OnWindowCloseReqMsg.Broadcast();

	//FName mctStringTable = "/Game/StringTables/MctStringTable_Kr.MctStringTable_Kr";
	//FString message = FText::FromStringTable(mctStringTable, TEXT("System_ExitWarningMessage")).ToString();
	//FText QuitMessage = FText::FromString(message);

	//// ����ڿ��� Ȯ���� �ޱ� ���� ���̾�α׸� �����մϴ�.
	//EAppReturnType::Type DialogResult = FMessageDialog::Open(EAppMsgType::YesNo, QuitMessage);

	//// ����ڰ� '��'�� �������� ��� ���Ḧ �����մϴ�.
	//if (DialogResult == EAppReturnType::Yes)
	//{
	//	return true;
	//}

	return false;
}

void URtGameInstance::UploadOneFile(FString URL, FString FilePath)
{
	// ���ε��� ���� ������
	TArray<uint8> UpFileRawData;
	FFileHelper::LoadFileToArray(UpFileRawData, *FilePath);

	FString fileName = FPaths::GetCleanFilename(FilePath);

	// htttp ������Ʈ ����
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httpRequest = FHttpModule::Get().CreateRequest();

	// ������Ʈ�� ���� webserver  �ּ�
	httpRequest->SetURL(URL);

	// post ���
	httpRequest->SetVerb(TEXT("POST"));

	// ������ �����ϴ� ������ �˷��ִ� boundary �÷���, ����� �����Ͽ� ������ �̸� �� �� �ֵ��� ��
	const FString boundary(TEXT("------------------------axcvv68d9we0asdg"));
	httpRequest->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data; boundary=") + boundary);
	httpRequest->SetHeader(TEXT("Accept"), TEXT("*/*"));
	httpRequest->SetHeader(TEXT("Expect"), TEXT("100-continue"));

	// boundary �÷��׸� ����� ���⼭���� ���� �������� �˷���
	const FString prefixBoundary(TEXT("--") + boundary + TEXT("\r\n"));
	const FString fileHeader(TEXT("Content-Disposition: form-data; name=\"fileToUpload\"; filename=") + fileName + TEXT("; Content-Type:application/octet-stream\r\n\r\n"));
	const FString suffixBoundary(TEXT("\r\n--") + boundary + TEXT("--\r\n"));
	// ���� ������ ������ �κп��� boundary �÷��׸� �������, ���� -- �� ����

	TArray<uint8> CombinedContent;
	CombinedContent.Append(FStringToUint8(prefixBoundary + fileHeader));	// �ٿ���� ǥ�� �� ���� ���� �Է�
	CombinedContent.Append(UpFileRawData);									// ���� ���ε�
	CombinedContent.Append(FStringToUint8(suffixBoundary));					// �б� ���� ǥ��

		// content body�� ���� ���� �߰�
	httpRequest->SetContent(CombinedContent);

	// ������Ʈ�� ����� �޾ƿ��� �Լ� ���ε�
	httpRequest->OnRequestProgress().BindUObject(this, &URtGameInstance::OnResponseUpLoadProgress);
	httpRequest->OnProcessRequestComplete().BindUObject(this, &URtGameInstance::OnResponseUpLoad);
	httpRequest->ProcessRequest();
}

bool URtGameInstance::UploadFiles(FString URL, TArray<FString> filePathList)
{
	if (URL.IsEmpty() || filePathList.Num() == 0)
	{
		return false;
	}

	for(auto data : filePathList)
	{
		UploadOneFile(URL, data);
	}

	return true;
}

// searchFileBasePath ���� 
bool URtGameInstance::UploadSearchFiles(FString URL, FString searchFileBasePath, FString extName)
{
	if (URL.IsEmpty() || searchFileBasePath.IsEmpty() || extName.IsEmpty())
	{
		return false;
	}

	// TCT ���� ����Ʈ ����.
	TArray<FString> fileList;
	IFileManager::Get().FindFiles(fileList, *FString(searchFileBasePath + "/*." + extName), true, false);
	if (fileList.Num() > 0)
	{
		for (auto& data : fileList) {
			data = FString::Printf(TEXT("%s/%s"), *searchFileBasePath, *data);
		}

		for (auto data : fileList)
		{
			UploadOneFile(URL, data);
		}

		return true;
	}

	return false;
}

TArray<uint8> URtGameInstance::FStringToUint8(const FString& InString)
{
	TArray<uint8> OutBytes;

	// Handle empty strings
	if (InString.Len() > 0)
	{
		FTCHARToUTF8 Converted(*InString); // Convert to UTF8
		OutBytes.Append(reinterpret_cast<const uint8*>(Converted.Get()), Converted.Length());
	}

	return OutBytes;
}

void URtGameInstance::OnResponseUpLoadProgress(FHttpRequestPtr Request, int32 SendByte, int32 RecvByte)
{
	TotalSize = Request->GetResponse()->GetContentLength();
	int send = SendByte;
	int tt = TotalSize;
}

void URtGameInstance::OnResponseUpLoad(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

	FString _Response = Response->GetContentAsString();
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Success   %s"), *_Response);

		SCREEN_LOG("Success   %s", *_Response);
	}

}


std::string URtGameInstance::GetBase64Decording(std::string encoded_string)
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

bool URtGameInstance::IsBase64(uint8 c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

void URtGameInstance::DownloadOneFile(FString URL, FString fileName, ERtWebProtocolFileType fileTypeInfo)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httpRequest = FHttpModule::Get().CreateRequest();

//	TSharedRef< IHttpRequest > Request = Http->CreateRequest();

	//APP_LOG("URtGameInstance::DownloadOneFile %s", *fileName);
	
	httpRequest->OnProcessRequestComplete().BindUObject(this, &URtGameInstance::OnResponseDownLoad);

	httpRequest->SetURL(URL);

	httpRequest->SetVerb("POST");

	httpRequest->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("filename", fileName);
	FString fileTypeInfoStr = FString::FromInt((int32)fileTypeInfo);
	RequestObj->SetStringField("filetype", fileTypeInfoStr);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);
	httpRequest->SetContentAsString(RequestBody);
	httpRequest->ProcessRequest();
}

void URtGameInstance::DownloadFiles(FString URL_Download, FString URL_MainServer, FString projectBaseFilePath)
{
	ProjectBaseFilePath = projectBaseFilePath;
	
	for (int32 i = (int32)ERtWebProtocol::EWebGetSCTList; i < (int32)ERtWebProtocol::EWebGetAllFileList; i++)
	{
		RequestToWebServer(URL_MainServer, (ERtWebProtocol)i);
	}

	DownloadURL = URL_Download;

//	DownloadOneFile(URL_Download, TEXT("BigTree1.tct"), TEXT("TCT"));
		
}

//void URtGameInstance::OnResponseProgress(FHttpRequestPtr Request, int32 SendByte, int32 RecvByte)
//{
//	TotalSize = Request->GetResponse()->GetContentLength();
//	int tt = TotalSize;
//}

void URtGameInstance::OnResponseDownLoad(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString de_filename = Response.Get()->GetHeader("filename");
	std::string de_filename_str = TCHAR_TO_UTF8(*de_filename);
	std::string filename_str = GetBase64Decording(de_filename_str);
	FString filename = UTF8_TO_TCHAR(filename_str.c_str());
	FString filetype = Response.Get()->GetHeader("filetype");
	//FString filename = Response.Get()->GetHeader("filename");
	//FString filetype = Response.Get()->GetHeader("filetype");
	FString SubPath;

	//APP_LOG("URtGameInstance::OnResponseDownLoad %s", *filename);

	ERtWebProtocolFileType fileTypeEnum = (ERtWebProtocolFileType)FCString::Atoi(*filetype);
	switch (fileTypeEnum)
	{
		case ERtWebProtocolFileType::ESCT:
		case ERtWebProtocolFileType::EMCT:
		case ERtWebProtocolFileType::EWCT: SubPath = PROJECT_PATH_MAPFILES; break;
		case ERtWebProtocolFileType::ETCT: SubPath = PROJECT_PATH_TCTFILES; break;
		case ERtWebProtocolFileType::EFBX: SubPath = PROJECT_PATH_FBXFILES; break;
		case ERtWebProtocolFileType::EPNG: SubPath = PROJECT_PATH_PNGFILES; break;
		case ERtWebProtocolFileType::EWAV: SubPath = PROJECT_PATH_WAVFILES; break;
		case ERtWebProtocolFileType::EFBXANIM: SubPath = PROJECT_PATH_FBX_AINM_FILES; break;
		case ERtWebProtocolFileType::EEFK_ZIP: SubPath = PROJECT_PATH_EFFECT_FILES; break;
		default:
			return;
	}

	FString FileSavePath = ProjectBaseFilePath + SubPath + TEXT("/") + filename;
	FPaths::MakeStandardFilename(FileSavePath);

	// Save File.
	if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		if( Response->GetContentLength() > 0 )
		{
			// �ش� �÷����� �´� File Ŭ���� �����.
			IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

			IFileHandle* FileHandler = PlatformFile.OpenWrite(*FileSavePath);
			if (!FileHandler)
			{
				FFileHelper::SaveStringToFile(TEXT(""), *FileSavePath); // ������ ������ ���� ����� �ش�.
				FileHandler = PlatformFile.OpenWrite(*FileSavePath);
			}

			if (FileHandler)
			{
				// ������ ���� ����.
				FileHandler->Write(Response->GetContent().GetData(), Response->GetContentLength());
				FileHandler->Flush();		// ����.

				if (fileTypeEnum == ERtWebProtocolFileType::EEFK_ZIP) {
					EfkZipFileList.Remove(filename);

					if (EfkZipFileList.IsEmpty()) {
						CompleteEfkZipDownloadMessage.Broadcast();
					}
				}

				delete FileHandler;
				bWasSuccessful = true;
				return;
			}
		}
	}

	bWasSuccessful = false;
}

void URtGameInstance::RequestToWebServer(FString URL, ERtWebProtocol protocal)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httpRequest = FHttpModule::Get().CreateRequest();
//	TSharedRef< IHttpRequest > Request = Http->CreateRequest();

	httpRequest->OnProcessRequestComplete().BindUObject(this, &URtGameInstance::OnResponseReceivedFromWebServer);

	//This is the url on which to process the request

//	Request->SetURL("http://192.168.0.64/ARTS/main.aspx");
	httpRequest->SetURL(URL);
	httpRequest->SetVerb("POST");
	httpRequest->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	FString protocalStr = FString::FromInt((int32)protocal);
	RequestObj->SetStringField("protocol", *protocalStr);
	// RequestObj->SetStringField( "UID", "100" );
	// RequestObj->SetStringField( "customint", "36" );

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);
	httpRequest->SetContentAsString(RequestBody);
	httpRequest->ProcessRequest();
}

void URtGameInstance::OnResponseReceivedFromWebServer(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		int32 protocol;
		if (JsonObject->TryGetNumberField(TEXT("protocol"), protocol))
		{
			APP_LOG("Protocol : %d", protocol);
		}

		ERtWebProtocolFileType downloadType = (ERtWebProtocolFileType)protocol;

		FString Result = JsonObject->GetStringField("result");

		TSharedPtr<FJsonObject> ResultJsonObject;
		if (FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Result), ResultJsonObject))
		{
			TArray<TSharedPtr<FJsonValue>> jvArray = ResultJsonObject->GetArrayField(TEXT("LIST"));

			if (jvArray.Num() > 0)
			{
				if (downloadType == ERtWebProtocolFileType::EEFK_ZIP) {
					EfkZipFileList.Empty();
					for (int32 i = 0; i < jvArray.Num(); i++) {
						TSharedPtr<FJsonValue> JsonValue = jvArray[i];
						TSharedPtr<FJsonObject> JsonValueObject = JsonValue->AsObject();
						FString fileName = JsonValueObject->GetStringField(TEXT("filename"));
						EfkZipFileList.Add(fileName);
					}
				}

				for (int32 i = 0; i < jvArray.Num(); i++)
				{
					TSharedPtr<FJsonValue> JsonValue = jvArray[i];
					TSharedPtr<FJsonObject> JsonValueObject = JsonValue->AsObject();

					FString fileName = JsonValueObject->GetStringField(TEXT("filename"));

					DownloadOneFile(DownloadURL, fileName, downloadType);

					APP_LOG("filename : %s", *fileName);
				}
			}
		}

	}

}

// dave 
void URtGameInstance::HandleNetworkFailure(UWorld* InWorld, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	FName NetDriverName = NetDriver ? NetDriver->NetDriverName : NAME_None;
	if (NetDriverName == NAME_GameNetDriver || NetDriverName == NAME_PendingNetDriver)
	{
		if (NetDriver)
		{
			switch (FailureType)
			{
			case ENetworkFailure::FailureReceived:
			{
				if (NetDriver->GetNetMode() == NM_Client) {}
				break;
			}
			case ENetworkFailure::PendingConnectionFailure:
			{
				if (NetDriver->GetNetMode() == NM_Client) {}
				break;
			}
			case ENetworkFailure::ConnectionLost:
			case ENetworkFailure::ConnectionTimeout:
			{
				break;
			}
			case ENetworkFailure::NetDriverAlreadyExists:
			case ENetworkFailure::NetDriverCreateFailure:
			case ENetworkFailure::OutdatedClient:
			case ENetworkFailure::OutdatedServer:
			default:
				break;
			}

			OnNetworkFailure_Delegate.Broadcast(FailureType);
		}
	}
}

