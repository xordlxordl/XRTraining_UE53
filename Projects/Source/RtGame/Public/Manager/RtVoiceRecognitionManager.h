// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "SpeechRecognizer.h"
#include "SpeechRecognizerThread.h"
#include "RtVoiceCaptureManager.h"

#include "RtVoiceRecognitionManager.generated.h"

class UCapturableSoundWave;
class USpeechRecognizer;

UCLASS()
class RTGAME_API URtVoiceRecognitionManager : public UGameInstanceSubsystem, public IVoiceCaptureListener
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	URtVoiceRecognitionManager();

protected:
	// Called when the game starts or when spawned
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void OnVoiceDataReceived(const TArray<uint8>& AudioData, uint32 SampleRate, int32 NumChannels) override;

private:
	UPROPERTY() 
	TObjectPtr<USpeechRecognizer> SpeechRecognition;

	UPROPERTY()
	TObjectPtr<UCapturableSoundWave> CapturableSoundWave;

	int32 SampleRate;
	int32 NumOfChannels;

public:
	UFUNCTION()
	void StartCapture(UCapturableSoundWave* sound);

public:
	FSpeechRecognitionParameters SpeechParameters;
	FOnSpeechRecognitionStartedDynamic Update_OnSpeechRecognizerStarted;
	FOnSpeechRecognitionErrorDynamic Update_OnSpeechRecognizerError;

	USpeechRecognizer* GetSpeechRecognizer() const;

	void StartRecognition();
	void StoptRecognition();

	UFUNCTION()
	void OnSpeechRecognitionStarted(bool bSucceeded);
	
	UFUNCTION()
	void OnRecognizedTextSegment(const FString& RecognizedWords);
	
	UFUNCTION()
	void OnRecognitionError(const FString& ShortErrorMessage, const FString& LongErrorMessage);
	
	UFUNCTION()
	void OnRecognitionStopped();
	
	UFUNCTION()
	void OnPopulateAudioData(const TArray<float>& PopulatedAudioData) const;

	UFUNCTION()
	void BP_SetCapturableSoundWave(AActor* actor);
};