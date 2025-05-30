// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/RtVoiceRecognitionManager.h"

#include "Voice.h"
#include "Sound/CapturableSoundWave.h"
#include "SpeechRecognizer.h"

#define __VOICE_RECOGNITION_CUSTOM_PARAMETERS__

// Sets default values
URtVoiceRecognitionManager::URtVoiceRecognitionManager()
{
}

void URtVoiceRecognitionManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SampleRate = UVOIPStatics::GetVoiceSampleRate();
	NumOfChannels = UVOIPStatics::GetVoiceNumChannels();

	SpeechRecognition = USpeechRecognizer::CreateSpeechRecognizer();
}

void URtVoiceRecognitionManager::Deinitialize()
{
	Super::Deinitialize();

	if (CapturableSoundWave)
	{
		CapturableSoundWave->ReleaseMemory();
		CapturableSoundWave->MarkPendingKill();
	}

	if (SpeechRecognition)
	{
		Update_OnSpeechRecognizerStarted.Clear();
		SpeechRecognition->OnRecognitionError.RemoveAll(this);
		SpeechRecognition->OnRecognizedTextSegment.RemoveAll(this);
		SpeechRecognition->OnRecognitionStopped.RemoveAll(this);

		SpeechRecognition->MarkPendingKill();
	}
}

// BP
void URtVoiceRecognitionManager::BP_SetCapturableSoundWave(AActor* actor)
{
	UClass* BPSpeechRecognizerClass = StaticLoadClass(AActor::StaticClass(), nullptr, TEXT("/Game/RS/Training/Blueprints/SpeechRecognize/BP_SpeechRecognizer.BP_SpeechRecognizer_C"));

	if (BPSpeechRecognizerClass)
	{
		UFunction* func = BPSpeechRecognizerClass->FindFunctionByName(TEXT("SetCapturableSoundWave"));
		if (func)
		{
			actor->ProcessEvent(func, CapturableSoundWave);
		}
	}
}

// start capture
void URtVoiceRecognitionManager::StartCapture(UCapturableSoundWave* sound)
{
	if (this && this->IsValidLowLevel()) 
	{
		if (IsValid(sound))
		{
			CapturableSoundWave = sound;
			SampleRate = CapturableSoundWave->GetSampleRateForCurrentPlatform();
			NumOfChannels = CapturableSoundWave->GetNumberOfChannels();

			CapturableSoundWave->OnPopulateAudioDataNative.Clear();
			CapturableSoundWave->OnPopulateAudioDataNative.AddUObject(this, &URtVoiceRecognitionManager::OnPopulateAudioData);

			if (IsValid(SpeechRecognition))
			{
				StartRecognition();
			}
			else 
			{
				UE_LOG(LogTemp, Warning, TEXT("StartRecognition: Invalid recognition object."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("StartCapture: Invalid sound object."));
		}
	}
}

USpeechRecognizer* URtVoiceRecognitionManager::GetSpeechRecognizer() const
{
	return SpeechRecognition;
}

// Voice Recognizer starts
void URtVoiceRecognitionManager::StartRecognition()
{
	if (SpeechRecognition)
	{
		// Check 
		Update_OnSpeechRecognizerStarted.Unbind();
		SpeechRecognition->OnRecognitionError.RemoveDynamic(this, &URtVoiceRecognitionManager::OnRecognitionError);
		SpeechRecognition->OnRecognizedTextSegment.RemoveDynamic(this, &URtVoiceRecognitionManager::OnRecognizedTextSegment);
		SpeechRecognition->OnRecognitionStopped.RemoveDynamic(this, &URtVoiceRecognitionManager::OnRecognitionStopped);

		Update_OnSpeechRecognizerStarted.BindDynamic(this, &URtVoiceRecognitionManager::OnSpeechRecognitionStarted);
		SpeechRecognition->OnRecognitionError.AddDynamic(this, &URtVoiceRecognitionManager::OnRecognitionError);
		SpeechRecognition->OnRecognizedTextSegment.AddDynamic(this, &URtVoiceRecognitionManager::OnRecognizedTextSegment);
		SpeechRecognition->OnRecognitionStopped.AddDynamic(this, &URtVoiceRecognitionManager::OnRecognitionStopped);

#ifdef __VOICE_RECOGNITION_CUSTOM_PARAMETERS__
		SpeechParameters.NumOfThreads = 0; 
		SpeechParameters.bTranslateToEnglish = false;
		SpeechParameters.StepSizeMs = 100;  
		SpeechParameters.bNoContext = true;
		SpeechParameters.bSingleSegment = true;
		SpeechParameters.MaxTokens = 32;
		SpeechParameters.AudioContextSize = 768;
		SpeechParameters.bSpeedUp = false;
		SpeechParameters.TemperatureToIncrease = -1.0f;
		SpeechParameters.EntropyThreshold = 2.4f;
		SpeechParameters.bSuppressBlank = true;
		SpeechParameters.bSuppressNonSpeechTokens = true;
		SpeechParameters.BeamSize = -1; 
		SpeechParameters.InitialPrompt = "";

		SpeechRecognition->SetRecognitionParameters(SpeechParameters);
#else
		SpeechRecognition->SetStreamingDefaults();
#endif
		SpeechRecognition->SetLanguage(ESpeechRecognizerLanguage::Ko);

		if (SpeechRecognition->GetIsStopped()) 
		{
			SpeechRecognition->StartSpeechRecognition(Update_OnSpeechRecognizerStarted);
		}
		else 
		{
			UE_LOG(LogTemp, Error, TEXT("Unable to stop the recognizer because it is already stopped."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to start the recognizer because it hasn't stopped yet."));
	}
}

// From Mic
void URtVoiceRecognitionManager::OnVoiceDataReceived(const TArray<uint8>& AudioData, uint32 AudioSampleRate, int32 AudioNumChannels)
{
	UE_LOG(LogTemp, Log, TEXT("Voice data received: %d bytes"), AudioData.Num());
}

//-- Callbacks --//

// recognizer started callback
void URtVoiceRecognitionManager::OnSpeechRecognitionStarted(bool bSucceeded)
{
	if (bSucceeded)
	{
		UE_LOG(LogTemp, Log, TEXT("Speech recognition started successfully."));

		CapturableSoundWave->ToggleVAD(true);
		CapturableSoundWave->SetVADMode(ERuntimeVADMode::Aggressive);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to start speech recognition."));
	}
}

// Recognized words callback
void URtVoiceRecognitionManager::OnRecognizedTextSegment(const FString& RecognizedWords)
{
	UE_LOG(LogTemp, Log, TEXT("RecognizedWords: %s"), *RecognizedWords);
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, FString::Printf(TEXT("RecognizedWords - Queued %s "), *RecognizedWords));
}

// Recognition fails callback
void URtVoiceRecognitionManager::OnRecognitionError(const FString& ShortErrorMessage, const FString& LongErrorMessage)
{
	UE_LOG(LogTemp, Log, TEXT("Recognize Errors: %s : %s"), *ShortErrorMessage, *LongErrorMessage);
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT(">> Recognize Errors : %s"), *LongErrorMessage));
}

// When recognizer stopped callback
void URtVoiceRecognitionManager::OnRecognitionStopped()
{
	UE_LOG(LogTemp, Log, TEXT("Recognized Stopped"));
}

// PCM datas 
void URtVoiceRecognitionManager::OnPopulateAudioData(const TArray<float>& PopulatedAudioData) const
{
	UE_LOG(LogTemp, Log, TEXT("Audio data populated with %d elements."), PopulatedAudioData.Num());

	if (SpeechRecognition)
	{
		if (SpeechRecognition->GetIsStopped() || SpeechRecognition->GetIsStopping())
		{
		}
		else
		{
			SpeechRecognition->ProcessAudioData(PopulatedAudioData, CapturableSoundWave->GetSampleRateForCurrentPlatform(), CapturableSoundWave->GetNumberOfChannels(), false);
			//SpeechRecognition->ProcessAudioData(PopulatedAudioData, SampleRate, NumOfChannels, false);
		}
	}
}

// Stop recognition
void URtVoiceRecognitionManager::StoptRecognition() 
{
	if (SpeechRecognition->GetIsStopped())
	{
		UE_LOG(LogTemp, Log, TEXT("Unable to stop the recognizer because it is already stopped."));
	}
	else 
	{
		SpeechRecognition->StopSpeechRecognition();
	}
}

