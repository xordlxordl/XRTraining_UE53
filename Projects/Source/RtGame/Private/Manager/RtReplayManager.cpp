// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/RtReplayManager.h"
// UE
#include "Engine/GameInstance.h"
#include "Engine/DemoNetDriver.h"
#include "ReplaySubsystem.h"
#include <Kismet/GameplayStatics.h>
// Framework
#include "Framework/RtGameMode.h"
#include "Framework/RtPlayerController.h"
#include "Framework/RtReplayTrainee.h"

// Manager
#include "Manager/RtConfig.h" 
#include "Manager/RtReplayManagerActor.h"
#include "Manager/RtGameManager.h"
#include "Manager/RtLoadingManager.h"

//Json, JsonUtilities
#include "Serialization/JsonSerializer.h" 
#include "JsonObjectConverter.h" 
#include "RtCore/Public/RtJsonFunctionLib.h"

// Voice
#include "Sound/SoundWaveProcedural.h"
#include "Components/AudioComponent.h"

// RAI
#include "RuntimeAudioImporterLibrary.h"
#include "RuntimeAudioExporter.h"
#include "RuntimeAudioImporter.h"

// Network
#include "Common/RtWebCommon.h"
#include "Manager/RtWebManager.h"

#include "Camera/CameraActor.h"
#include "Common/RtGameplayTags.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"


DEFINE_LOG_CATEGORY(LogReplayManager);
DEFINE_LOG_CATEGORY(LogDemoNetDriver);


void URtReplayManager::InitManager()
{
	ReplayUrl_upload = URtConfig::WebReplayUploadUrl;
	ReplayUrl_download = URtConfig::WebReplayDownloadUrl;


	// Set BaseFolder Path
	FString BasePath;
#if WITH_EDITOR
	BasePath = FPaths::ProjectDir();
#else
	BasePath = FPaths::LaunchDir();
#endif

	int32 ReplayTID = ReplayInfo.SelectedTID;
	FString ReplaySecarioName = ReplayInfo.SelectedScenarioName;

	FString MakePath = FString::Printf(TEXT("TrainingData/RepFiles/%s/%d"), *ReplaySecarioName, ReplayTID);
	BasePath = FPaths::Combine(BasePath, MakePath);
	// Set BasePath for Load Voice Files
	ReplayInfo.SelectedScenarioFolderPath = BasePath;

	// Make Dir tree 
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	PlatformFile.CreateDirectoryTree(*BasePath);

	if (auto* gi = GetGameInstance()) {
		WebManager = gi->GetSubsystem<URtWebManager>();
		if (WebManager->IsInitialized()) {
			WebManager->Set_ReplayManager(this);
			WebManager->GetTraineeList(ReplayInfo.SelectedTID);
		}
	}

	PlayerReplayWavFullPathMap.Empty();
}

void URtReplayManager::ClearSCT()
{
	if (ARtReplayManagerActor* replayManagerActor = ARtReplayManagerActor::Get(GetWorld())) {
		replayManagerActor->InitReplay();
	}
}

TArray<uint8> URtReplayManager::FStringToUint8(const FString& InString)
{
	TArray<uint8> OutBytes;

	// Handle empty strings
	if (InString.Len() > 0) {
		FTCHARToUTF8 Converted(*InString); // Convert to UTF8
		OutBytes.Append(reinterpret_cast<const uint8*>(Converted.Get()), Converted.Length());
	}

	return OutBytes;
}

// Maybe UID == INDEX
ARtReplayTrainee* URtReplayManager::FindRepTrainee(int32 uid) 
{
	if (ARtReplayManagerActor* replayManagerActor = ARtReplayManagerActor::Get(GetWorld())) {
		ARtReplayTrainee* RepTrainee;
		RepTrainee = replayManagerActor->ReplayPawnMap.FindRef(uid);
		return RepTrainee;
	}
	return nullptr;
}

void URtReplayManager::ClearAllCameraCaptureTextureTarget()
{
	if (ARtReplayManagerActor* replayManagerActor = ARtReplayManagerActor::Get(GetWorld())) {
		for (const auto& item : replayManagerActor->ReplayPawnMap) {
			ARtReplayTrainee* RepTrainee = item.Value;
			if (RepTrainee) {
				RepTrainee->ClearScencCaptureTextureTarget();
			}
		}
	}
}

void URtReplayManager::Upload_VoiceFile(FString TID, FString UID, FString FileName)
{
	FString VoiceFilePath;
#if WITH_EDITOR
	VoiceFilePath = FPaths::ProjectDir();
#else
	VoiceFilePath = FPaths::LaunchDir();
#endif

	FString DirectoryPath = FString::Printf(TEXT("TrainingData/RecordingData/%s/%s.WAV"), *TID , *FileName);
	VoiceFilePath = FPaths::Combine(VoiceFilePath, DirectoryPath);

	FPaths::MakeStandardFilename(VoiceFilePath);

	// Upload Data File
	TArray<uint8> UpFileRawData;
	FFileHelper::LoadFileToArray(UpFileRawData, *VoiceFilePath);

	// Create Http Request
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httpRequest = FHttpModule::Get().CreateRequest();

	// Address of php File that receives Request
	httpRequest->SetURL(ReplayUrl_upload);
	// post 
	httpRequest->SetVerb(TEXT("POST"));

	// boundary flag notice start point of File
	const FString boundary(TEXT("------------------------axcvv68d9we0asdg"));
	httpRequest->SetHeader(TEXT("TID"), TID);
	httpRequest->SetHeader(TEXT("UID"), UID);
	httpRequest->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data; boundary=") + boundary);
	httpRequest->SetHeader(TEXT("Accept"), TEXT("*/*"));
	httpRequest->SetHeader(TEXT("Expect"), TEXT("100-continue"));

	// By using boundary flag, inform to server the File's info start from here
	const FString prefixBoundary(TEXT("--") + boundary + TEXT("\r\n"));
	const FString fileHeader(TEXT("Content-Disposition: form-data; name=\"fileToUpload\"; filename=") + FileName + TEXT(".WAV") + TEXT("; Content-Type:application/octet-stream\r\n\r\n"));
	const FString suffixBoundary(TEXT("\r\n--") + boundary + TEXT("--\r\n"));
	// Use boundary flag to end point of File's info. add '--' at the end point.

	TArray<uint8> CombinedContent;
	CombinedContent.Append(FStringToUint8(prefixBoundary + fileHeader));
	CombinedContent.Append(UpFileRawData);
	CombinedContent.Append(suffixBoundary);

	// Add File info at content body
	httpRequest->SetContent(CombinedContent);

	// Function Bind 'OnResponse_Upload_File'
	httpRequest->OnProcessRequestComplete().BindUObject(this, &URtReplayManager::OnResponse_Upload_File);
	httpRequest->ProcessRequest();
}


void URtReplayManager::Download_VoiceFiles(int index)
{
	// Init idx
	FString idx = FString::FromInt(index);

	// 다운로드 페이지 호출
	FHttpModule* Http = &FHttpModule::Get();
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &URtReplayManager::OnResponse_Download_File);
	// This is the url on which to process the request
	Request->SetURL(ReplayUrl_download);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("i", idx);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);
	Request->SetContentAsString(RequestBody);
	Request->ProcessRequest();
}


void URtReplayManager::Download_RepFile(TArray<FString> ReplayDataString)
{
	FString SavedFolder_Path;
	bool bOutWriteResult;
	FString ErrMsg;

	SavedFolder_Path = ReplayInfo.SelectedScenarioFolderPath;

	MotionFilePath = FPaths::Combine(SavedFolder_Path, FString::Printf(TEXT("/Motion.json")));

	// Create File class of platform
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FPaths::MakeStandardFilename(MotionFilePath);

	TArray<TSharedPtr<FJsonValue>> newobjArray;

	for (int i = 0; i < ReplayDataString.Num(); i++) {

		TSharedPtr<FJsonObject> newJsonObject;
		if (FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(ReplayDataString[i]), newJsonObject)) {
			if (newJsonObject.IsValid()) {
				newobjArray.Add(MakeShareable(new FJsonValueObject(newJsonObject)));
			}
		}
	}

	TSharedPtr<FJsonObject> newJsonFile = MakeShareable(new FJsonObject);
	newJsonFile->SetArrayField(TEXT("LIST"), newobjArray);
	URtJsonFunctionLib::WriteJson(MotionFilePath, newJsonFile, bOutWriteResult, ErrMsg);

	if (bOutWriteResult) {
		LoadSCTDatas();
	}
}


void URtReplayManager::OnResponse_Upload_File(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	//FString Response_String = Response->GetContentAsString();
	if (bWasSuccessful) {
		UE_LOG(LogReplayManager, Warning, TEXT("Success Upload"));
	}
	 
	//TSharedPtr<FJsonObject> JsonObject;
	//
	//if (FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Response_String), JsonObject)) {
	//	FString protocol = JsonObject->GetStringField(TEXT("protocol"));
	//	FString result = JsonObject->GetStringField(TEXT("result"));
	//
	//	if (FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(result), JsonObject) && JsonObject.IsValid()) {
	//		FString nid = JsonObject->GetStringField(TEXT("filename"));
	//		FString filename = JsonObject->GetStringField(TEXT("FileSize"));
	//
	//		if (auto* wm = GetGameInstance()->GetSubsystem<URtWebManager>()) {
	//			// 추후에 결과 데이터가 필요하면 여기서 관리하자.
	//		}
	//	}
	//}
}

void URtReplayManager::OnResponse_Download_File(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (Response == nullptr) return;
	if (!bWasSuccessful) return;

	FString de_filename = Response.Get()->GetHeader("filename");
	std::string de_filename_str = TCHAR_TO_UTF8(*de_filename);
	std::string filename_str = GetBase64Decording(de_filename_str);
	FString filename = UTF8_TO_TCHAR(filename_str.c_str());

	// Set Path
	FString VoiceFilePath;
#if WITH_EDITOR
	VoiceFilePath = FPaths::ProjectDir();
#else
	VoiceFilePath = FPaths::LaunchDir();
#endif
	
	int32 ReplayTID = ReplayInfo.SelectedTID;
	FString ReplaySecarioName = ReplayInfo.SelectedScenarioName;
	FString ReplaySavedFolder = ReplayInfo.SelectedScenarioFolderPath;

	VoiceFilePath = FString::Printf(TEXT("%s/%s"),*ReplaySavedFolder, *filename);
	
	//int32 index1, index2;
	//if (filename.FindChar(TEXT(','), index1) && 
	//	filename.FindChar(TEXT('.'), index2))
	//{
	//	int32 uid = FCString::Atoi(*filename.Mid(index1, index2 - index1));
	//	PlayerReplayWavFullPathMap[uid] = VoiceFilePath;
	//}
	
	FPaths::MakeStandardFilename(VoiceFilePath);
	
	// Save File
	if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode())) {
		// Create File class of platform
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		IFileHandle* FileHandler = PlatformFile.OpenWrite(*VoiceFilePath);
		if (!FileHandler)
		{
			FFileHelper::SaveStringToFile(TEXT(""), *VoiceFilePath); // 파일이 없으면 새로 만들어 준다.
			FileHandler = PlatformFile.OpenWrite(*VoiceFilePath);
		}

		if (FileHandler) {
			// Write new File 
			FileHandler->Write(Response->GetContent().GetData(), Response->GetContentLength());
			FileHandler->Flush();

			voiceListNum--;
	

			delete FileHandler;
			bWasSuccessful = true;
		}
		else {
			bWasSuccessful = false;
		}

		if (voiceListNum == 0) {
			// Fin Delegate
			voiceHandler.Broadcast();
			voiceHandler.Clear();
		}
	}
	else {
		bWasSuccessful = false;
	}
}

void URtReplayManager::SetWavFullPathMap(FString fullPath)
{
	TArray<FString> voiceList;
	FString searchPattern = fullPath / TEXT("*.wav");
	IFileManager::Get().FindFiles(voiceList, *searchPattern, true, false);
	for (auto& item : voiceList) {
		int32 index1, index2;
		if (item.FindChar(TEXT(','), index1) &&
			item.FindChar(TEXT('.'), index2))
		{
			index1++;
			int32 uid = FCString::Atoi(*item.Mid(index1, index2 - index1));
			if (uid > 0) {
				FString value = fullPath + TEXT("/") + item;
				PlayerReplayWavFullPathMap.Add(uid, value);
			}
		}
	}

	PlayedFilePath = FString::Printf(TEXT("%s/ReplayVoice.WAV"), *fullPath);
}

USoundWave* URtReplayManager::LoadSoundWave(const FString& FilePath)
{
	USoundWave* SoundWave = NewObject<USoundWave>(USoundWave::StaticClass());

	TArray<uint8> RawFile;
	bool ck = FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath);
	if (FFileHelper::LoadFileToArray(RawFile, *FilePath))
	{
		FWaveModInfo WaveInfo;
		if (WaveInfo.ReadWaveInfo(RawFile.GetData(), RawFile.Num()))
		{
			// Set WAV Info to SoundWave
			SoundWave->SetSampleRate(*WaveInfo.pSamplesPerSec);
			SoundWave->NumChannels = *WaveInfo.pChannels;
			SoundWave->Duration = static_cast<float>(*WaveInfo.pWaveDataSize) / *WaveInfo.pAvgBytesPerSec;
			SoundWave->RawPCMDataSize = *WaveInfo.pWaveDataSize;

			// Copy Audio Data
			SoundWave->RawPCMData = (uint8*)FMemory::Malloc(SoundWave->RawPCMDataSize);
			FMemory::Memcpy(SoundWave->RawPCMData, WaveInfo.SampleDataStart, SoundWave->RawPCMDataSize);

			// Set Additional Info
			SoundWave->SoundGroup = ESoundGroup::SOUNDGROUP_Default;
			SoundWave->NumChannels = *WaveInfo.pChannels;

			// Set Cue Point
			for (const FWaveCue& WaveCue : WaveInfo.WaveCues)
			{
				FSoundWaveCuePoint NewCuePoint;
				NewCuePoint.CuePointID = WaveCue.CuePointID;
				NewCuePoint.Label = WaveCue.Label.IsEmpty() ? FString::Printf(TEXT("Cue_%d"), WaveCue.CuePointID) : WaveCue.Label;
				NewCuePoint.FramePosition = WaveCue.Position;
				NewCuePoint.FrameLength = WaveCue.SampleLength;
				SoundWave->CuePoints.Add(NewCuePoint);
			}
			return SoundWave;
		}
	}
	return nullptr;
}

void URtReplayManager::LoadSCTDatas()
{
	if (ARtReplayManagerActor* replayManagerActor = ARtReplayManagerActor::Get(GetWorld())) {
		replayManagerActor->LoadSctReplay(ReplayInfo);
	}
}

USoundWave* URtReplayManager::MixSoundWaves(const TArray<USoundWave*>& SoundWaves)
{
	if (SoundWaves.Num() == 0) {
		UE_LOG(LogReplayManager, Warning, TEXT("Empty SoundWaves"));
		return nullptr;
	}

	int32 MaxLength = 0; // Longest Length of AudioFiles
	int32 SampleRate = 48000;
	int32 Channel = 2;

	for (const USoundWave* wave : SoundWaves) {
		MaxLength = FMath::Max(MaxLength, wave->RawPCMDataSize);
	}

	TArray<float> MixedBuffer;
	MixedBuffer.SetNumZeroed(MaxLength / sizeof(int16));

	for (const USoundWave* Wave : SoundWaves)
	{
		const int16* RawData = reinterpret_cast<const int16*>(Wave->RawPCMData);
		int32 NumSamples = Wave->RawPCMDataSize / sizeof(int16);

		for (int32 i = 0; i < NumSamples; ++i)
		{
			MixedBuffer[i] += static_cast<float>(RawData[i]) / 32768.0f;
		}
	}

	float MaxAmplitude = 0.0f;
	for (float& Sample : MixedBuffer)
	{
		MaxAmplitude = FMath::Max(MaxAmplitude, FMath::Abs(Sample));
	}

	if (MaxAmplitude > 1.0f)
	{
		for (float& Sample : MixedBuffer)
		{
			Sample /= MaxAmplitude;
		}
	}
	USoundWave* MixedSoundWave = NewObject<USoundWave>(USoundWave::StaticClass());
	MixedSoundWave->SetSampleRate(SampleRate);
	MixedSoundWave->NumChannels = Channel;
	MixedSoundWave->Duration = static_cast<float>(MaxLength) / (SampleRate * Channel * sizeof(int16));
	MixedSoundWave->RawPCMDataSize = MaxLength;

	// 믹싱된 데이터를 int16 형식으로 변환
	MixedSoundWave->RawPCMData = (uint8*)FMemory::Malloc(MaxLength);
	int16* DestData = reinterpret_cast<int16*>(MixedSoundWave->RawPCMData);
	for (int32 i = 0; i < MixedBuffer.Num(); ++i)
	{
		DestData[i] = static_cast<int16>(MixedBuffer[i] * 32767.0f);
	}

	// 믹싱된 데이터를 uint8 로 변환
	TArray<uint8> ImportedBuffer;
	ImportedBuffer.SetNum(MixedSoundWave->RawPCMDataSize);
	FMemory::Memcpy(ImportedBuffer.GetData(), MixedSoundWave->RawPCMData, MixedSoundWave->RawPCMDataSize);

	// Import Buffer
	URuntimeAudioImporterLibrary* Importer = URuntimeAudioImporterLibrary::CreateRuntimeAudioImporter();
	Importer->OnResult.AddDynamic(this, &URtReplayManager::Finish_Import);
	Importer->ImportAudioFromRAWBuffer(ImportedBuffer, ERuntimeRAWAudioFormat::Int16, SampleRate, Channel);
	

	return MixedSoundWave;
}

void URtReplayManager::Finish_Conv()
{
	


}

void URtReplayManager::Finish_Import(class URuntimeAudioImporterLibrary* Importer, class UImportedSoundWave* ImportedSoundWav, enum ERuntimeImportStatus Status)
{
	UImportedSoundWave* testimportwave = ImportedSoundWav;
	ERuntimeImportStatus importresult = Status;

	URuntimeAudioExporter* RuntimeAudioExporter = NewObject<URuntimeAudioExporter>();
	if (RuntimeAudioExporter)
	{
		FOnAudioExportToFileResultNative result;
		result.BindUFunction(this, TEXT("Finish_Export"));
		FRuntimeAudioExportOverrideOptions overrideoption;
		FString baseDir;
		baseDir = ReplayInfo.SelectedScenarioFolderPath;

		//PlayedFilePath = FString::Printf(TEXT("%s/ReplayVoice.WAV"), *baseDir);
		SetWavFullPathMap(baseDir);

		FPaths::MakeStandardFilename(PlayedFilePath);

		RuntimeAudioExporter->ExportSoundWaveToFile(testimportwave, PlayedFilePath, ERuntimeAudioFormat::Wav, 100, overrideoption, result);
	}
}

void URtReplayManager::Finish_Export()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, TEXT("EXPORT DONE!!"));

	SetCompHandler.Broadcast(PlayedFilePath);
}

TArray<FString> URtReplayManager::ListAllWavFilesInFolder()
{
	TArray<FString> FoundFiles;
	TArray<FString> FileFullPathArray;
	// 폴더 내의 모든 .wav 파일 찾기
	FString FullPath = FPaths::ConvertRelativePathToFull(ReplayInfo.SelectedScenarioFolderPath);
	FString SearchPattern = FullPath / TEXT("*.wav");
	IFileManager::Get().FindFiles(FoundFiles, *SearchPattern, true, false);

	// 각 WAV 파일 로드
	for (const FString& FileName : FoundFiles)
	{
		FString FullFilePath = FullPath / FileName;
		FPaths::MakeStandardFilename(FullFilePath);

		FileFullPathArray.Add(FullFilePath);
	}

	return FileFullPathArray;
}



std::string URtReplayManager::GetBase64Decording(std::string encoded_string)
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

bool URtReplayManager::IsBase64(uint8 c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

void URtReplayManager::ClearAllTraineeCaptureTextureTarget()
{
	if (ARtReplayManagerActor* replayManagerActor = ARtReplayManagerActor::Get(GetWorld())) {

		for (const TPair<int32, ARtReplayTrainee*>& pair : replayManagerActor->ReplayPawnMap)
		{
			USceneCaptureComponent2D* sceneCaptureCom = Cast<USceneCaptureComponent2D>(
				pair.Value->GetComponentByClass(USceneCaptureComponent2D::StaticClass()));

			if (sceneCaptureCom) {
				sceneCaptureCom->TextureTarget = nullptr;
			}
		}
	}
}

void URtReplayManager::ClearTopViewCameraCaptureTextureTarget()
{
	ACameraActor* camera = nullptr;
	TArray<AActor*> OutActor;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraActor::StaticClass(), RtPlayTags::RtReplayMode_Actor_Camera, OutActor);
	for (auto* Actor : OutActor)
	{
		camera = Cast<ACameraActor>(Actor);
		if (camera) {

			USceneCaptureComponent2D* sceneCaptureCom = Cast<USceneCaptureComponent2D>(
				camera->GetComponentByClass(USceneCaptureComponent2D::StaticClass()));

			if (sceneCaptureCom) {
				sceneCaptureCom->TextureTarget = nullptr;
			}

			return;
		}
	}

}