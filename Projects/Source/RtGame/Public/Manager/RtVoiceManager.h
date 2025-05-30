// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/CapturableSoundWave.h"

// TestVoice
#include "Misc/CoreMisc.h"
#include "Containers/Ticker.h"
#include "RtVoiceManager.generated.h"

// add class 
class UAudioComponent;
class USoundWaveProcedural;
class UCapturableSoundWave;
class IVoiceCapture;
class IVoiceEncoder;
class IVoiceDecoder;
enum class EAudioEncodeHint : uint8;
struct Audio::FCaptureDeviceInfo;

// add Define
#define VOICE_MAX_COMPRESSED_BUFFER 20 * 1024
#define VOICE_STARTING_REMAINDER_SIZE 1 * 1024



UCLASS()
class RTGAME_API ARtVoiceManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARtVoiceManager();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init();

private:


	UPROPERTY()	UAudioComponent* VoiceComp;
	UPROPERTY() USoundWaveProcedural* SoundStreaming;

	TSharedPtr<IVoiceCapture> VoiceCapture;
	TSharedPtr<IVoiceEncoder> VoiceEncoder;
	TSharedPtr<IVoiceDecoder> VoiceDecoder;

	FString DeviceName;
	EAudioEncodeHint EncodeHint;                 // Encoding Rules
	int32 InputSampleRate;
	int32 OutputSampleRate;
	int32 NumInChannels;
	int32 NumOutChannels;

	bool bLastWasPlaying;
	int32 StarvedDataCount;                      // Number of consecutive frames that playback has been starved. (why need this...?)

	TArray<uint8> RawCaptureData;                // Before encoding
	int32 MaxRawCaptureDataSize;                 // channel * samplerate * sizeof(uint)

	TArray<uint8> CompressedData;                // After encoding
	int32 MaxCompressedDataSize;                 // Maximum size of a single encoded packet

	TArray<uint8> UncompressedData;              // After decoding
	int32 MaxUncompressedDataSize;               // Maximum size of a single decoded packet

	mutable FCriticalSection QueueLock;
	TQueue<TArray<uint8>> CompressedDataQueue;           // After encoding data's queue (FROM UDP)
	TArray<uint8> UncompressedDataQueue;         // After decoding data's queue
	int32 CurrentUncompressedDataQueueSize;      
	int32 MaxUncompressedDataQueueSize;

	TArray<uint8> Remainder;                     // Remain data after encoding, remainder used next encoding
	int32 MaxRemainderSize;
	int32 LastRemainderSize;                     // why need this????
	uint64 CachedSampleCount;

	bool bZeroInput;
	bool bUseDecompressed;                       // skip Encode/Decode
	bool bZeroOutput;

	void InitVoiceCapture();                     // re init with current setting and realloc buffers
	void InitVoiceEncoder();					 // re init with current setting and realloc buffers
	void InitVoiceDecoder();					 // re init with current setting and realloc buffers
	void InitReplayManager();

	void Shutdown();

	void CleanupVoice();
	void CleanupAudioComponent();
	void CleanupQueue();


	/*
	========================================================================================================
	====        Record      ================================================================================
	========================================================================================================
	*/
public:
	UPROPERTY()
	UCapturableSoundWave* CapturableSoundWave;

	TSharedPtr<IVoiceCapture> GetVoiceCapture() const 
	{ 
		return VoiceCapture; 
	}

private:
	// udp
	class URtNetworkManager* netmgr;
	// rep
	class URtReplayManager* repmgr;

	FString VoiceFileName;
	int32 rep_TID;
	int32 rep_UID;

	float SegmentStartTimeSec;
	FCriticalSection CaptureLock;

public:
	/**
     * Callback from streaming audio when data is requested for playback
     *
     * @InProceduralWave : SoundWave requesting more data
     * @SamplesRequired  : number of samples needed for immediate playback
     */
	UFUNCTION()
	void GenerateData(USoundWaveProcedural* InProceduralWave, int32 SamplesRequired);

	UFUNCTION()
	void DecodedAndRun(const TArray<uint8>& buffer, int CompressedSize);

	FTimerHandle PlayVoiceTimerHandler;
	FTimerHandle DequeHandler;
	
	void VoiceCaptureTick();
	void VoiceCaptureTickLocal();	// dave

	void SetNetManager(URtNetworkManager* inNetmgr);
	URtReplayManager* GetRepManager(){ return repmgr; }
	void OnSpeechRecognizerStarted(bool isStarted);
/*
========================================================================================================
====        Record      ================================================================================
========================================================================================================
*/
	bool IsCapturing() const;

	// Bind and Execute when beginplay
	UFUNCTION()
	void StartCapture();

	UFUNCTION()
	void RestartCapture();

	// Bind beginplay and Execute when finish training
	UFUNCTION()
	void ExportCapture(int TID, int UID, FString TraineeCode);

	UFUNCTION()
	void FinishExportSound();


	// dave
	UFUNCTION()
	TArray<uint8> GetAudioSegment(float StartTimeSec, float DurationSec);
	
	UFUNCTION()
	void StartAudioSegment();

	UFUNCTION()
	TArray<uint8> EndAudioSegment();

	int GetDefaultDeviceIndex(Audio::FCaptureDeviceInfo* DefaultInfo);

};
