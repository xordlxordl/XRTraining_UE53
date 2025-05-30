// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtAudioCaptureComponent.h"
#include "Misc/DateTime.h"

#include "RtCore/Public/Common/RtPacket.h"
#include "RtCore/Public/Common/RtProtocol.h"
#include "Network/RtNetworkManager.h"
#include "Manager/RtVoiceManager.h"

URtAudioCaptureComponent::URtAudioCaptureComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	AudioQueue = new TQueue<TArray<float>>();
}

int32 URtAudioCaptureComponent::OnGenerateAudio(float* OutAudio, int32 NumSamples)
{
	int32 OutputSampleGenerated = 0;
	OutputSampleGenerated = Super::OnGenerateAudio(OutAudio, NumSamples);

	TArray<float> pcmData;
	pcmData.Init(0.f, OutputSampleGenerated);
	FMemory::Memcpy(pcmData.GetData(), OutAudio, OutputSampleGenerated * sizeof(float));

	// to do
	FDateTime currentTime = FDateTime::Now();
	UE_LOG(LogTemp, Log, TEXT("[%d : %d]PCM Data size : %d"), currentTime.GetSecond(), currentTime.GetMillisecond(), sizeof(pcmData));

	pcmData.Init(0.f, OutputSampleGenerated);
	FMemory::Memcpy(pcmData.GetData(), OutAudio, OutputSampleGenerated * sizeof(float));

	// Add PCM data to queue
	AudioQueue->Enqueue(pcmData);
	
	if (Owner == nullptr) return OutputSampleGenerated;


	if (UGameInstance* gi = Owner->GetGameInstance())
	{
		if (URtNetworkManager* nm = gi->GetSubsystem<URtNetworkManager>())
		{
			TArray<uint8> CapturedPCMDataUint8;

			CapturedPCMDataUint8.Reset();
			CapturedPCMDataUint8.AddUninitialized(pcmData.Num() * sizeof(int16));

			int16* Int16Data = reinterpret_cast<int16*>(CapturedPCMDataUint8.GetData());

			for (int32 Index = 0; Index < pcmData.Num(); ++Index)
			{
				// float 범위 (-1.0f to 1.0f) -> int16 범위 (-32768 to 32767)
				Int16Data[Index] = FMath::Clamp(pcmData[Index], -1.0f, 1.0f) * 32767;

				CapturedPCMDataUint8[Index * 2] = Int16Data[Index] & 0xFF;          // 하위 바이트
				CapturedPCMDataUint8[Index * 2 + 1] = (Int16Data[Index] >> 8) & 0xFF; // 상위 바이트
			}

			FString PCMDataString;
			for (int32 i = 0; i < CapturedPCMDataUint8.Num(); ++i)
			{
				PCMDataString += FString::Printf(TEXT("%02x "), CapturedPCMDataUint8[i]);

				// 너무 많은 데이터를 출력하지 않도록 일부 데이터만 출력
				if (i > 100)
				{
					PCMDataString += TEXT("...");
					break;
				}
			}

			UE_LOG(LogTemp, Log, TEXT("PCM Data: %s"), *PCMDataString);
		}
	}



	return OutputSampleGenerated;




	// Test 품질 개선 코드
	//::OnGenerateAudio(OutAudio, NumSamples);

	//int32 Samples = Super::OnGenerateAudio(OutAudio, NumSamples);
	//
	//for (int i = 0; i < Samples; i++)
	//{
	//	OutAudio[i] = OutAudio[i] / 10.f;
	//}
	//
	//
	//return Samples;
}
