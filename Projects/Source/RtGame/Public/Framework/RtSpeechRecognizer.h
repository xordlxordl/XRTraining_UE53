// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RtSpeechRecognizer.generated.h"

UCLASS()
class RTGAME_API ARtSpeechRecognizer : public AActor
{
	GENERATED_BODY()
	
public:	
	ARtSpeechRecognizer();

public:
	UFUNCTION(BlueprintCallable, Category = "Speech")
	void HandleRecognizedSpeech(const FString& RecognizedText);
};
