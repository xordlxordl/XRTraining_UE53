// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AudioCaptureComponent.h"
#include "RtAudioCaptureComponent.generated.h"


class ARtVoiceManager;

/**
 * 
 */
UCLASS(ClassGroup = Synth, meta = (BlueprintSpawnnableComponent))
class RTGAME_API URtAudioCaptureComponent : public UAudioCaptureComponent
{
	GENERATED_BODY()
	
protected:

	URtAudioCaptureComponent(const FObjectInitializer& ObjectInitializer);
	virtual int32 OnGenerateAudio(float* OutAudio, int32 NumSamples) override;

public:
	TQueue<TArray<float>>* AudioQueue;
	ARtVoiceManager* Owner;
};
