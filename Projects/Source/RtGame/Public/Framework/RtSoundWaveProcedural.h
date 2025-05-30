// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundWaveProcedural.h"
#include "RtSoundWaveProcedural.generated.h"

/**
 * 
 */
UCLASS()
class RTGAME_API URtSoundWaveProcedural : public USoundWaveProcedural
{
	GENERATED_BODY()
	
public:

	TArray<float> AudioData;
	int32 SampleRate;

	virtual void BeginDestroy() override;
	virtual int32 GeneratePCMData(uint8* PCMData, const int32 SamplesNeeded) override;

	void SetAudioData(const TArray<float>& InAudioData, int32 InSampleRate);

};
