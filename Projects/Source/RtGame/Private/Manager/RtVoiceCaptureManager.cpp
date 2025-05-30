// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/RtVoiceCaptureManager.h"
#include "VoiceModule.h"
#include "Voice.h"

URtVoiceCaptureManager* URtVoiceCaptureManager::Get(UWorld* WorldContext)
{
	if (!WorldContext)
	{
		WorldContext = GWorld; 
		if (!WorldContext)
		{
			UE_LOG(LogTemp, Error, TEXT("URtVoiceCaptureManager::Get - No valid world context"));
			return nullptr;
		}
	}

	UGameInstance* GameInstance = WorldContext->GetGameInstance();
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("URtVoiceCaptureManager::Get - No valid game instance"));
		return nullptr;
	}

	static URtVoiceCaptureManager* SharedInstance = nullptr;

	if (!SharedInstance)
	{
		SharedInstance = NewObject<URtVoiceCaptureManager>(GameInstance);
	}
	return SharedInstance;
}

TSharedPtr<IVoiceCapture> URtVoiceCaptureManager::GetVoiceCapture() const
{
	return VoiceCapture;
}

TSharedPtr<IVoiceCapture> URtVoiceCaptureManager::CreateVoiceCapture(FString DeviceName, int32 InSampleRate, int32 InNumChannels)
{
	Initialize(DeviceName, InSampleRate, InNumChannels);
	return VoiceCapture;
}

URtVoiceCaptureManager::URtVoiceCaptureManager() 
{
	VoiceCapture = nullptr;
}

void URtVoiceCaptureManager::Initialize(FString DeviceName, int32 InSampleRate, int32 InNumChannels)
{
	if (VoiceCapture.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("VoiceCapture is already initialized"));
		return;
	}

	SampleRate = InSampleRate;
	NumChannels = InNumChannels;

	VoiceCapture = FVoiceModule::Get().CreateVoiceCapture(DeviceName, SampleRate, NumChannels);

	if (!VoiceCapture.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("CreateVoiceCapture returned null for device: %s"), *DeviceName);
		return;
	}

	if (VoiceCapture.Get() == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("VoiceCapture internal pointer is null even though IsValid() returned true!"));
		return;
	}

	if (!VoiceCapture->Init(DeviceName, SampleRate, NumChannels))
	{
		UE_LOG(LogTemp, Error, TEXT("VoiceCapture->Init() failed for device: %s"), *DeviceName);
		VoiceCapture.Reset();
		return;
	}
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &URtVoiceCaptureManager::PollVoiceData, 0.02f, true);
}

void URtVoiceCaptureManager::StartCapture()
{
	if (VoiceCapture.IsValid())
	{
		VoiceCapture->Start();
	}
}

void URtVoiceCaptureManager::StopCapture()
{
	if (VoiceCapture.IsValid())
	{
		VoiceCapture->Stop();
	}
}

void URtVoiceCaptureManager::Destroy()
{
	URtVoiceCaptureManager* Instance = Get();
	if (Instance)
	{
		Instance = nullptr;
	}
}

void URtVoiceCaptureManager::RegisterListener(IVoiceCaptureListener* Listener)
{
	FScopeLock Lock(&Mutex);
	Listeners.AddUnique(Listener);
}

void URtVoiceCaptureManager::UnregisterListener(IVoiceCaptureListener* Listener)
{
	FScopeLock Lock(&Mutex);
	Listeners.Remove(Listener);
}

void URtVoiceCaptureManager::Tick(float DeltaTime)
{
	PollVoiceData();
}

void URtVoiceCaptureManager::PollVoiceData()
{
	if (!VoiceCapture.IsValid()) return;

	uint32 NewVoiceDataBytes = 0;
	uint32 BytesAvailable = 0;
	EVoiceCaptureState::Type MicState = VoiceCapture->GetCaptureState(NewVoiceDataBytes);
	if (MicState == EVoiceCaptureState::Ok && NewVoiceDataBytes > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Getting data! %d"), NewVoiceDataBytes);

		TArray<uint8> AudioData;
		AudioData.SetNumUninitialized(BytesAvailable);
		uint32 ReadBytes = 0;
		VoiceCapture->GetVoiceData(AudioData.GetData(), BytesAvailable, ReadBytes);

		if (ReadBytes > 0)
		{
			AudioData.SetNum(ReadBytes);
			BroadcastVoiceData(AudioData);
		}
	}
}

void URtVoiceCaptureManager::BroadcastVoiceData(const TArray<uint8>& AudioData)
{
	FScopeLock Lock(&Mutex);
	for (IVoiceCaptureListener* Listener : Listeners)
	{
		if (Listener)
		{
			Listener->OnVoiceDataReceived(AudioData, SampleRate, NumChannels);
		}
	}
}
