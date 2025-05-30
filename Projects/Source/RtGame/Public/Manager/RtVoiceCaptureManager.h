// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "VoiceModule.h"
#include "Voice.h"
#include "Sound/SoundWaveProcedural.h"

#include "RtVoiceCaptureManager.generated.h"

class UCapturableSoundWave;
class UAudioComponent;
class USoundWaveProcedural;
class IVoiceCapture;
class IVoiceEncoder;
class IVoiceDecoder;
enum class EAudioEncodeHint : uint8;

//
class IVoiceCaptureListener
{
public:
	virtual ~IVoiceCaptureListener() = default;
	virtual void OnVoiceDataReceived(const TArray<uint8>& AudioData, uint32 SampleRate, int32 NumChannels) = 0;
};

/// <summary>
/// Voice Capture manager
/// </summary>
UCLASS()
class RTGAME_API URtVoiceCaptureManager : public UObject
{
    GENERATED_BODY()

public:
    URtVoiceCaptureManager();
    static URtVoiceCaptureManager* Get(UWorld* WorldContext = nullptr);
    static void Destroy();

    void StartCapture();
    void StopCapture();

    void RegisterListener(IVoiceCaptureListener* Listener);
    void UnregisterListener(IVoiceCaptureListener* Listener);

    void Tick(float DeltaTime);

public:
    TSharedPtr<IVoiceCapture> GetVoiceCapture() const;
    TSharedPtr<IVoiceCapture> CreateVoiceCapture(FString DeviceName, int32 InSampleRate, int32 InNumChannels);

private:
    TSharedPtr<IVoiceCapture, ESPMode::ThreadSafe> VoiceCapture;
    TArray<IVoiceCaptureListener*> Listeners;

    int32 NumChannels;
    int32 SampleRate;

    FCriticalSection Mutex;

    void Initialize(FString DeviceName, int32 InSampleRate, int32 InNumChannels);
    void PollVoiceData();
    void BroadcastVoiceData(const TArray<uint8>& AudioData);
};
