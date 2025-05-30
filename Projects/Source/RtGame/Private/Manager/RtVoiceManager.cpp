// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/RtVoiceManager.h"
#include "Components/AudioComponent.h"
#include "Network/RtNetworkManager.h"
#include "Manager/RtReplayManager.h"

#include "OnlineSubsystemUtils.h"
#include "VoiceModule.h"
#include "Voice.h"
#include "Sound/SoundWaveProcedural.h"

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"


#include "RuntimeAudioImporterLibrary.h"
#include "RuntimeAudioExporter.h"
#include "AudioCaptureCore.h"

#include "Manager/RtConfig.h"
#include "Manager/RtVoiceRecognitionManager.h"

#if !UE_BUILD_SHIPPING
#define VOICE_BUFFER_CHECK(Buffer, Size) \
	check(Buffer.Num() >= (int32)(Size))
#else
#define VOICE_BUFFER_CHECK(Buffer, Size)
#endif


// Sets default values
ARtVoiceManager::ARtVoiceManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARtVoiceManager::BeginPlay()
{
	Super::BeginPlay();

	VoiceComp = nullptr;
	SoundStreaming = nullptr;
	VoiceCapture = nullptr;
	VoiceEncoder = nullptr;
	VoiceDecoder = nullptr;
	EncodeHint = UVOIPStatics::GetAudioEncodingHint();
	InputSampleRate = UVOIPStatics::GetVoiceSampleRate();
	OutputSampleRate = UVOIPStatics::GetVoiceSampleRate();
	NumInChannels = UVOIPStatics::GetVoiceNumChannels();
	NumOutChannels = UVOIPStatics::GetVoiceNumChannels();
	bLastWasPlaying = false;
	StarvedDataCount = 0;
	MaxRawCaptureDataSize = 0;
	MaxCompressedDataSize = 0;
	MaxUncompressedDataSize = 0;
	CurrentUncompressedDataQueueSize = 0;
	MaxUncompressedDataQueueSize = 0;
	MaxRemainderSize = 0;
	LastRemainderSize = 0;
	CachedSampleCount = 0;
	bZeroInput = false;        // 해당 부분은 제어해볼 필요 있음
	bUseDecompressed = true;
	bZeroOutput = false;
}

void ARtVoiceManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// dave
	if (CapturableSoundWave)
	{
		CapturableSoundWave->ReleaseMemory();
	}

	Super::EndPlay(EndPlayReason);

	Shutdown();
}


void ARtVoiceManager::Init()
{

	DeviceName = TEXT("");
	EncodeHint = EAudioEncodeHint::VoiceEncode_Audio;
	InputSampleRate = 48000;
	OutputSampleRate = 48000;
	NumInChannels = 1;
	NumOutChannels = 1;

	InitVoiceCapture();
	InitVoiceEncoder();
	InitVoiceDecoder();
	InitReplayManager();

	// dave
	if (URtConfig::VoiceTalk)
	{
		GetWorld()->GetTimerManager().SetTimer(PlayVoiceTimerHandler, this, &ARtVoiceManager::VoiceCaptureTick, 0.02f, true, 0.1f);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(PlayVoiceTimerHandler, this, &ARtVoiceManager::VoiceCaptureTickLocal, 0.02f, true, 0.1f);
	}
}

void ARtVoiceManager::InitVoiceCapture()
{
	if (FVoiceModule::Get().IsAvailable())
	{
		//VoiceCapture = URtVoiceCaptureManager::Get()->CreateVoiceCapture(DeviceName, InputSampleRate, NumInChannels);
		VoiceCapture = FVoiceModule::Get().CreateVoiceCapture(DeviceName, InputSampleRate, NumInChannels);
		if (VoiceCapture.IsValid())
		{
			MaxRawCaptureDataSize = VoiceCapture->GetBufferSize();
			RawCaptureData.Empty(MaxRawCaptureDataSize);
			RawCaptureData.AddUninitialized(MaxRawCaptureDataSize);



			VoiceCapture->Start();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Voice capture not supported on this platform."));
	}
}

void ARtVoiceManager::InitVoiceEncoder()
{
	if (VoiceCapture.IsValid())
	{
		VoiceEncoder = FVoiceModule::Get().CreateVoiceEncoder(InputSampleRate, NumInChannels, EncodeHint);
		if (VoiceEncoder.IsValid())
		{
			MaxRemainderSize = VOICE_STARTING_REMAINDER_SIZE;
			LastRemainderSize = 0;
			MaxCompressedDataSize = VOICE_MAX_COMPRESSED_BUFFER;

			CompressedData.Empty(MaxCompressedDataSize);
			CompressedData.AddUninitialized(MaxCompressedDataSize);

			Remainder.Empty(MaxRemainderSize);
			Remainder.AddUninitialized(MaxRemainderSize);
		}
	}
}

void ARtVoiceManager::InitVoiceDecoder()
{
	if (VoiceCapture.IsValid())
	{
		VoiceDecoder = FVoiceModule::Get().CreateVoiceDecoder(OutputSampleRate, NumOutChannels);
		if (VoiceDecoder.IsValid())
		{
			MaxUncompressedDataSize = NumOutChannels * OutputSampleRate * sizeof(uint16);

			UncompressedData.Empty(MaxUncompressedDataSize);
			UncompressedData.AddUninitialized(MaxUncompressedDataSize);

			MaxUncompressedDataQueueSize = MaxUncompressedDataSize * 5;
			{
				FScopeLock ScopeLock(&QueueLock);
				UncompressedDataQueue.Empty(MaxUncompressedDataQueueSize);
			}
		}
	}
}

void ARtVoiceManager::InitReplayManager()
{
	auto* gi = GetGameInstance();
	if (gi)
	{
		repmgr = gi->GetSubsystem<URtReplayManager>();
		if (repmgr)
		{
			repmgr->InitManager();
		}
	}
}

void ARtVoiceManager::Shutdown()
{
	RawCaptureData.Empty();
	CompressedData.Empty();
	UncompressedData.Empty();
	Remainder.Empty();

	{
		FScopeLock ScopeLock(&QueueLock);
		UncompressedDataQueue.Empty();
	}

	CleanupVoice();
	CleanupAudioComponent();
}

void ARtVoiceManager::CleanupVoice()
{
	if (VoiceCapture.IsValid())
	{
		VoiceCapture->Shutdown();
		VoiceCapture = nullptr;
	}

	VoiceEncoder = nullptr;
	VoiceDecoder = nullptr;
}
//AudioComponent->OnAudioEnvelopeValue.AddDynamic(this, &UMyClass::OnEnvelopeValue);
void ARtVoiceManager::CleanupAudioComponent()
{
	if (VoiceComp != nullptr)
	{
		VoiceComp->Stop();

		if (SoundStreaming)
		{
			SoundStreaming->OnSoundWaveProceduralUnderflow.Unbind();
			SoundStreaming = nullptr;
		}

		VoiceComp->RemoveFromRoot();
		VoiceComp = nullptr;

		bLastWasPlaying = false;
	}
}

void ARtVoiceManager::CleanupQueue()
{
	FScopeLock ScopeLock(&QueueLock);
	UncompressedDataQueue.Reset();
}

void ARtVoiceManager::GenerateData(USoundWaveProcedural* InProceduralWave, int32 SamplesRequired)
{
	const int32 SampleSize = sizeof(uint16) * NumOutChannels;
	{
		FScopeLock ScopeLock(&QueueLock);
		CurrentUncompressedDataQueueSize = UncompressedDataQueue.Num();
		const int32 AvailableSamples = CurrentUncompressedDataQueueSize / SampleSize;

		if (AvailableSamples > 0)
		{
			// dave - check overflow
			if (AvailableSamples * SampleSize > UncompressedDataQueue.Num())
			{
				UE_LOG(LogTemp, Warning, TEXT("Buffer size mismatch! AvailableSamples: %d, QueueSize: %d"),
					AvailableSamples, UncompressedDataQueue.Num());
				return;
			}

			InProceduralWave->QueueAudio(UncompressedDataQueue.GetData(), AvailableSamples * SampleSize);
			//CurrentUncompressedDataQueueSize -= (AvailableSamples * SampleSize);
			UncompressedDataQueue.RemoveAt(0, AvailableSamples * SampleSize, false);
			CurrentUncompressedDataQueueSize = UncompressedDataQueue.Num();

			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, FString::Printf(TEXT("GenerateData - Queued %d samples, Remaining: %d"), AvailableSamples, CurrentUncompressedDataQueueSize / SampleSize));
		}
		else
		{
			UE_LOG(LogTemp, Verbose, TEXT("GenerateData - No samples available"));
		}
	}
}

/*
// Origin
void ARtVoiceManager::DecodedAndRun(const TArray<uint8>& buffer, int CompressedSize)
{
	UE_LOG(LogTemp, Log, TEXT("CurrentUncompressedDataQueueSize %d!"), CurrentUncompressedDataQueueSize);

	uint32 UncompressedDataSize = 0;
	if (VoiceDecoder.IsValid() && CompressedSize > 0)
	{
		UncompressedDataSize = MaxUncompressedDataSize;
		VoiceDecoder->Decode(buffer.GetData(), CompressedSize,
			UncompressedData.GetData(), UncompressedDataSize);
		VOICE_BUFFER_CHECK(UncompressedData, UncompressedDataSize);
	}

	const uint8* VoiceDataPtr = nullptr;
	uint32 VoiceDataSize = 0;

	if (bUseDecompressed)
	{
		if (UncompressedDataSize > 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Queuing uncompressed data! %d"), UncompressedDataSize);
			if (bZeroOutput)
			{
				FMemory::Memzero((uint8*)UncompressedData.GetData(), UncompressedDataSize);
			}

			VoiceDataSize = UncompressedDataSize;
			VoiceDataPtr = UncompressedData.GetData();
		}
	}

	if (VoiceDataPtr && VoiceDataSize > 0)
	{
		FScopeLock ScopeLock(&QueueLock);

		const int32 OldSize = UncompressedDataQueue.Num();
		const int32 AmountToBuffer = (OldSize + (int32)VoiceDataSize);
		if (AmountToBuffer <= MaxUncompressedDataQueueSize)
		{
			UncompressedDataQueue.AddUninitialized(VoiceDataSize);

			VOICE_BUFFER_CHECK(UncompressedDataQueue, AmountToBuffer);
			FMemory::Memcpy(UncompressedDataQueue.GetData() + OldSize, VoiceDataPtr, VoiceDataSize);
			CurrentUncompressedDataQueueSize += VoiceDataSize;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UncompressedDataQueue Overflow!"));
		}
	}

	if (CurrentUncompressedDataQueueSize > (MaxUncompressedDataSize / 2))
	{
		VoiceComp->Play();
	}
}
*/

// dave
void ARtVoiceManager::DecodedAndRun(const TArray<uint8>& buffer, int CompressedSize)
{
	if (!VoiceDecoder.IsValid() || CompressedSize <= 0)
		return;

	UE_LOG(LogTemp, Log, TEXT("CurrentUncompressedDataQueueSize %d!"), CurrentUncompressedDataQueueSize);

	uint32 UncompressedDataSize = 0;
	if (VoiceDecoder.IsValid() && CompressedSize > 0)
	{
		UncompressedDataSize = MaxUncompressedDataSize;
		VoiceDecoder->Decode(buffer.GetData(), CompressedSize,
			UncompressedData.GetData(), UncompressedDataSize);

		if (UncompressedDataSize > (uint32)UncompressedData.Num())
		{
			UncompressedData.Empty(UncompressedDataSize);
			UncompressedData.AddUninitialized(UncompressedDataSize);
		}
	}

	if (UncompressedDataSize > 0)
	{
		FScopeLock ScopeLock(&QueueLock);

		const int32 TargetBufferSize = 48000 * 2;
		if (MaxUncompressedDataQueueSize != TargetBufferSize)
		{
			MaxUncompressedDataQueueSize = TargetBufferSize;
			UncompressedDataQueue.Reserve(MaxUncompressedDataQueueSize);
		}

		const int32 CurrentSize = UncompressedDataQueue.Num();
		const int32 NewSize = CurrentSize + UncompressedDataSize;

		if (NewSize > MaxUncompressedDataQueueSize)
		{
			const int32 RemoveSize = NewSize - MaxUncompressedDataQueueSize;
			if (RemoveSize > 0)
			{
				UncompressedDataQueue.RemoveAt(0, RemoveSize, false);
			}
		}

		UncompressedDataQueue.AddUninitialized(UncompressedDataSize);
		FMemory::Memcpy(UncompressedDataQueue.GetData() + (UncompressedDataQueue.Num() - UncompressedDataSize),
			UncompressedData.GetData(), UncompressedDataSize);
		CurrentUncompressedDataQueueSize = UncompressedDataQueue.Num();

		if (VoiceComp)
		{
			if (CurrentUncompressedDataQueueSize > (MaxUncompressedDataSize / 2))
			{
				VoiceComp->Play();
			}
		}
	}
}


//
void ARtVoiceManager::VoiceCaptureTick()
{
	QUICK_SCOPE_CYCLE_COUNTER(STAT_FTVoice_Tick);

	if (VoiceCapture.IsValid())
	{
		if (VoiceComp == nullptr)
		{
			VoiceComp = NewObject<UAudioComponent>(this);
			VoiceComp->SetupAttachment(RootComponent);
			VoiceComp->bAutoActivate = true;
			VoiceComp->bAlwaysPlay = true;
			VoiceComp->SetVolumeMultiplier(2.0f);

			if (SoundStreaming == nullptr)
			{
				SoundStreaming = NewObject<USoundWaveProcedural>();
				SoundStreaming->SetSampleRate(48000);
				SoundStreaming->NumChannels = 1;
				SoundStreaming->Duration = INDEFINITELY_LOOPING_DURATION;
				SoundStreaming->SoundGroup = SOUNDGROUP_Voice;
				SoundStreaming->bLooping = false;
				SoundStreaming->bProcedural = true;
				VoiceComp->Sound = SoundStreaming;

				// Bind the GenerateData callback with FOnSoundWaveProceduralUnderflow object
				SoundStreaming->OnSoundWaveProceduralUnderflow.BindUFunction(this, TEXT("GenerateData"));
			}
		}

		if (VoiceComp)
		{
			check(SoundStreaming);

			//bool bIsPlaying = VoiceComp->IsPlaying();
			//if (bIsPlaying != bLastWasPlaying)
			//{
			//	UE_LOG(LogTemp, Log, TEXT("VOIP audio component %s playing!"), bIsPlaying ? TEXT("is") : TEXT("is not"));
			//	bLastWasPlaying = bIsPlaying;
			//}

			//StarvedDataCount = (!bIsPlaying || (SoundStreaming->GetAvailableAudioByteCount() != 0)) ? 0 : (StarvedDataCount + 1);
			//if (StarvedDataCount > 1)
			//{
			//	UE_LOG(LogTemp, Log, TEXT("VOIP audio component starved %d frames!"), StarvedDataCount);
			//}

			bool bDoWork = false;
			uint32 TotalVoiceBytes = 0;

			uint32 NewVoiceDataBytes = 0;
			EVoiceCaptureState::Type MicState = VoiceCapture->GetCaptureState(NewVoiceDataBytes);
			if (MicState == EVoiceCaptureState::Ok && NewVoiceDataBytes > 0)
			{
				UE_LOG(LogTemp, Log, TEXT("Getting data! %d"), NewVoiceDataBytes);

				// dave - resize buffer
				uint32 RequiredBufferSize = NewVoiceDataBytes + LastRemainderSize;
				if (RequiredBufferSize > (uint32)MaxRawCaptureDataSize)
				{
					UE_LOG(LogTemp, Warning, TEXT("RawCaptureData buffer overflow! Resizing from %d to %d"),
						MaxRawCaptureDataSize, RequiredBufferSize);
					MaxRawCaptureDataSize = RequiredBufferSize;
					RawCaptureData.Empty(MaxRawCaptureDataSize);
					RawCaptureData.AddUninitialized(MaxRawCaptureDataSize);
				}

				if (LastRemainderSize > 0)
				{
					// Add back any data from the previous frame
					// dave 
					if (LastRemainderSize <= RawCaptureData.Num())
					{
						FMemory::Memcpy(RawCaptureData.GetData(), Remainder.GetData(), LastRemainderSize);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("LastRemainderSize exceeds RawCaptureData buffer size!"));
						LastRemainderSize = 0;
					}
				}

				// Add new data at the beginning of the last frame
				uint64 SampleCount;
				MicState = VoiceCapture->GetVoiceData(RawCaptureData.GetData() + LastRemainderSize, NewVoiceDataBytes, NewVoiceDataBytes, SampleCount);
				TotalVoiceBytes = NewVoiceDataBytes + LastRemainderSize;

				// Check to make sure this buffer has a valid, chronological buffer count.
				if (SampleCount <= CachedSampleCount)
				{
					UE_LOG(LogTemp, Log, TEXT("Out of order or ambiguous sample count detected! This sample count: %lu Previous sample count: %lu"), SampleCount, CachedSampleCount);
				}

				CachedSampleCount = SampleCount;

				// dave - voice ready?
				if (TotalVoiceBytes <= (uint32)RawCaptureData.Num())
				{
					bDoWork = (MicState == EVoiceCaptureState::Ok);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("TotalVoiceBytes exceeds RawCaptureData buffer size!"));
					bDoWork = false;
				}
			}

			if (bDoWork && TotalVoiceBytes > 0)
			{
				// ZERO INPUT
				if (bZeroInput)
				{
					FMemory::Memzero(RawCaptureData.GetData(), TotalVoiceBytes);
				}
				// ZERO INPUT END

				// COMPRESSION BEGIN
				uint32 CompressedDataSize = 0;
				if (VoiceEncoder.IsValid())
				{
					CompressedDataSize = MaxCompressedDataSize;
					LastRemainderSize = VoiceEncoder->Encode(RawCaptureData.GetData(), TotalVoiceBytes, CompressedData.GetData(), CompressedDataSize);

					if (LastRemainderSize > 0)
					{
						if (LastRemainderSize > MaxRemainderSize)
						{
							// dave - resize
							UE_LOG(LogTemp, Verbose, TEXT("Remainder buffer overflow! Resizing from %d to %d"),
								MaxRemainderSize, LastRemainderSize);
							MaxRemainderSize = LastRemainderSize;
							Remainder.Empty(MaxRemainderSize);
							Remainder.AddUninitialized(MaxRemainderSize);
						}

						if (LastRemainderSize <= Remainder.Num())
						{
							FMemory::Memcpy(Remainder.GetData(), RawCaptureData.GetData() + (TotalVoiceBytes - LastRemainderSize), LastRemainderSize);
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("LastRemainderSize exceeds Remainder buffer size!"));
							LastRemainderSize = 0;
						}
					}

					if (CompressedDataSize > 0)
					{
						UE_LOG(LogTemp, Log, TEXT("VOIP audio component Encoded %d !"), CompressedDataSize);
					}

				}
				// COMPRESSION END

				// UDP
				if (netmgr && CompressedDataSize > 0)
				{
					netmgr->SendPacket_UDP(CompressedData, CompressedDataSize);
				}
			}
		}
	}
}

// dave = local capture
void ARtVoiceManager::VoiceCaptureTickLocal()
{
	QUICK_SCOPE_CYCLE_COUNTER(STAT_FTVoice_Tick);

	if (!VoiceCapture.IsValid())
		return;

	if (VoiceComp == nullptr)
	{
		VoiceComp = NewObject<UAudioComponent>(this);
		VoiceComp->SetupAttachment(RootComponent);
		VoiceComp->bAutoActivate = true;
		VoiceComp->bAlwaysPlay = true;
		VoiceComp->SetVolumeMultiplier(2.0f);

		if (SoundStreaming == nullptr)
		{
			SoundStreaming = NewObject<USoundWaveProcedural>();
			SoundStreaming->SetSampleRate(48000);
			SoundStreaming->NumChannels = 1;
			SoundStreaming->Duration = INDEFINITELY_LOOPING_DURATION;
			SoundStreaming->SoundGroup = SOUNDGROUP_Voice;
			SoundStreaming->bLooping = false;
			SoundStreaming->bProcedural = true;
			VoiceComp->Sound = SoundStreaming;
			SoundStreaming->OnSoundWaveProceduralUnderflow.BindUFunction(this, TEXT("GenerateData"));
		}
	}

	uint32 AvailableVoiceData = 0;
	EVoiceCaptureState::Type CaptureState = VoiceCapture->GetCaptureState(AvailableVoiceData);

	if (CaptureState == EVoiceCaptureState::Ok && AvailableVoiceData > 0)
	{
		const int32 MaxProcessSize = FMath::Min(AvailableVoiceData, (uint32)MaxRawCaptureDataSize);
		if (MaxProcessSize > 0)
		{
			uint32 ReadBytes = 0;
			if (VoiceCapture->GetVoiceData(RawCaptureData.GetData(), MaxProcessSize, ReadBytes))
			{
				{
					FScopeLock ScopeLock(&QueueLock);
					const int32 NewSize = UncompressedDataQueue.Num() + ReadBytes;
					if (NewSize > MaxUncompressedDataQueueSize)
					{
						const int32 RemoveSize = NewSize - MaxUncompressedDataQueueSize;
						if (RemoveSize > 0)
						{
							UncompressedDataQueue.RemoveAt(0, RemoveSize, false);
						}
					}

					UncompressedDataQueue.AddUninitialized(ReadBytes);
					FMemory::Memcpy(UncompressedDataQueue.GetData() + (UncompressedDataQueue.Num() - ReadBytes),
						RawCaptureData.GetData(), ReadBytes);
					CurrentUncompressedDataQueueSize = UncompressedDataQueue.Num();
				}

				if (VoiceComp)
				{
					if (CurrentUncompressedDataQueueSize > (MaxUncompressedDataSize / 2))
					{
						VoiceComp->Play();
					}
				}
			}
		}
	}
}

void ARtVoiceManager::SetNetManager(URtNetworkManager* inNetmgr)
{
	netmgr = inNetmgr;
}

void ARtVoiceManager::StartCapture()
{
	FScopeLock Lock(&CaptureLock);

	Audio::FCaptureDeviceInfo DefaultInfo;
	int idx = GetDefaultDeviceIndex(&DefaultInfo);

	if (idx < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Start Capture Failed(Can't Found Default Mic)"));
		return;
	}

	if (CapturableSoundWave == nullptr)
	{
		CapturableSoundWave = NewObject<UCapturableSoundWave>();
		CapturableSoundWave->CreateCapturableSoundWave();
	}

	if (CapturableSoundWave->StartCapture(idx))
	{
		UE_LOG(LogTemp, Log, TEXT("Audio Capture Start with Device(%d) %s"), idx, *DefaultInfo.DeviceName);
		//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, TEXT(" CapturableSoundWave Start %s"), *DefaultInfo.DeviceName);
	}

	URtVoiceRecognitionManager* Recognizer = GetGameInstance()->GetSubsystem<URtVoiceRecognitionManager>();
	if (Recognizer)
	{
		Recognizer->StartCapture(CapturableSoundWave);
	}
}

void ARtVoiceManager::RestartCapture()
{
	FScopeLock Lock(&CaptureLock);

	if (CapturableSoundWave)
	{
		CapturableSoundWave->StopCapture();
		CapturableSoundWave->ReleaseMemory(); // dave
	}

}

void ARtVoiceManager::ExportCapture(int TID, int UID, FString TraineeCode)
{
	FScopeLock Lock(&CaptureLock);

	// Set Params for upload file
	rep_TID = TID;
	rep_UID = UID;
	VoiceFileName = TraineeCode;

	if (CapturableSoundWave)
	{
		CapturableSoundWave->StopCapture();

		// Stop recognizer
		URtVoiceRecognitionManager* Recognizer = GetGameInstance()->GetSubsystem<URtVoiceRecognitionManager>();
		if (Recognizer)
		{
			Recognizer->StoptRecognition();
		}

		URuntimeAudioExporter* RuntimeAudioExporter = NewObject<URuntimeAudioExporter>();
		if (RuntimeAudioExporter)
		{
			FOnAudioExportToFileResultNative result;
			result.BindUFunction(this, TEXT("FinishExportSound"));
			FRuntimeAudioExportOverrideOptions overrideoption;
			FString baseDir;
#if WITH_EDITOR
			baseDir = FPaths::ProjectDir();
#else
			baseDir = FPaths::LaunchDir();
#endif
			FString Directory_path = FString::Printf(TEXT("TrainingData/RecordingData/%d/%s.WAV"), TID, *TraineeCode);
			FString FilePath = baseDir + Directory_path;
			FString FullDirectoryPath = FPaths::GetPath(FilePath);
			if (!FPaths::DirectoryExists(FullDirectoryPath))
			{
				FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*FullDirectoryPath);
			}

			FPaths::MakeStandardFilename(FilePath);

			// dave
			result.BindLambda([=, this](bool bSuccess)
				{
					if (bSuccess)
					{
						UE_LOG(LogTemp, Log, TEXT("Export succeeded."));
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Export failed."));
					}

					if (IsValid(CapturableSoundWave))
					{
						CapturableSoundWave->ReleaseMemory();
					}
				});

			RuntimeAudioExporter->ExportSoundWaveToFile(CapturableSoundWave, FilePath, ERuntimeAudioFormat::Wav, 100, overrideoption, result);
		}
	}
}

void ARtVoiceManager::OnSpeechRecognizerStarted(bool isStarted)
{
	URtVoiceRecognitionManager* Recognizer = GetGameInstance()->GetSubsystem<URtVoiceRecognitionManager>();
	if (Recognizer)
	{
		Recognizer->GetSpeechRecognizer()->ForceProcessPendingAudioData();
	}
}

void ARtVoiceManager::FinishExportSound()
{
	if (repmgr)
	{
		FString str_TID = FString::FromInt(rep_TID);
		FString str_UID = FString::FromInt(rep_UID);
		repmgr->Upload_VoiceFile(str_TID, str_UID, VoiceFileName);
	}
}

// Called every frame
void ARtVoiceManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Get Aidio Segment
TArray<uint8> ARtVoiceManager::GetAudioSegment(float StartTimeSec, float DurationSec)
{
	FScopeLock ScopeLock(&QueueLock);

	int32 StartIndex = FMath::FloorToInt(StartTimeSec * InputSampleRate * NumInChannels * sizeof(uint16));
	int32 Length = FMath::FloorToInt(DurationSec * InputSampleRate * NumInChannels * sizeof(uint16));

	if (StartIndex + Length > UncompressedDataQueue.Num())
		return {};

	return TArray<uint8>(UncompressedDataQueue.GetData() + StartIndex, Length);
}

// Audio segment starts
void ARtVoiceManager::StartAudioSegment()
{
	SegmentStartTimeSec = static_cast<float>(UncompressedDataQueue.Num()) / (InputSampleRate * NumInChannels * sizeof(uint16));
}

// Audio segment ends 
TArray<uint8> ARtVoiceManager::EndAudioSegment()
{
	FScopeLock ScopeLock(&QueueLock);

	if (SegmentStartTimeSec < 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Segment start time not set."));
		return {};
	}

	float TotalDurationSec = static_cast<float>(UncompressedDataQueue.Num()) / (InputSampleRate * NumInChannels * sizeof(uint16));
	float SegmentEndTimeSec = TotalDurationSec;

	SegmentStartTimeSec = -1.0f;

	return GetAudioSegment(SegmentStartTimeSec, SegmentEndTimeSec);
}

int ARtVoiceManager::GetDefaultDeviceIndex(Audio::FCaptureDeviceInfo* OutDefaultDevice)
{
	int idx = -1;

	Audio::FAudioCapture AudioCapture;

	TArray<Audio::FCaptureDeviceInfo> InputDevices;
	
	Audio::FCaptureDeviceInfo DefaultInfo;
	AudioCapture.GetCaptureDevicesAvailable(InputDevices);
	AudioCapture.GetCaptureDeviceInfo(DefaultInfo);

	if (OutDefaultDevice)
		*OutDefaultDevice = DefaultInfo;

	idx = InputDevices.IndexOfByPredicate([&](const Audio::FCaptureDeviceInfo& info) {
		return info.DeviceName == DefaultInfo.DeviceName;
		});
	return idx;
}
