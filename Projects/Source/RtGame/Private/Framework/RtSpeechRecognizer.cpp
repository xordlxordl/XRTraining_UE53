// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtSpeechRecognizer.h"

#include "Framework/RtPlayerController.h"
#include "Framework/RtTraineePawn.h"

// Sets default values
ARtSpeechRecognizer::ARtSpeechRecognizer()
{

}

void ARtSpeechRecognizer::HandleRecognizedSpeech(const FString& RecognizedText)
{
	UE_LOG(LogTemp, Warning, TEXT("Speech Recognizer Testing, Recognized Text : %s"), *RecognizedText);

	ARtPlayerController* controller = Cast<ARtPlayerController>(GetWorld()->GetFirstPlayerController());
	if (controller)
	{
		ARtTraineePawn* TraineePawn = Cast<ARtTraineePawn>(controller->GetPawn());

		TraineePawn->ExecuteCommandFromString(RecognizedText);
	}
}

