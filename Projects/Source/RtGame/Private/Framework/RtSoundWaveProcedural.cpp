// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtSoundWaveProcedural.h"

void URtSoundWaveProcedural::BeginDestroy()
{
	Super::BeginDestroy();
}

int32 URtSoundWaveProcedural::GeneratePCMData(uint8* PCMData, const int32 SamplesNeeded)
{
    int32 BytesGenerated = 0;
    if (AudioData.Num() > 0)
    {
        int32 BytesToCopy = FMath::Min(SamplesNeeded, AudioData.Num()) * sizeof(float);
        FMemory::Memcpy(PCMData, AudioData.GetData(), BytesToCopy);
        BytesGenerated = BytesToCopy;
        AudioData.RemoveAt(0, BytesToCopy / sizeof(float));
    }
    return BytesGenerated;
}

void URtSoundWaveProcedural::SetAudioData(const TArray<float>& InAudioData, int32 InSampleRate)
{
    AudioData = InAudioData;
    SampleRate = InSampleRate;
    this->SampleRate = InSampleRate;
    this->NumChannels = 1; // assuming mono
    this->Duration = (float)AudioData.Num() / SampleRate;
    this->bLooping = false;
}
