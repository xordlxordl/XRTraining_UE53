// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RtListenIconUserWidget.generated.h"

/**
 *
 */
UCLASS()
class RTGAME_API URtListenIconUserWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
	UPROPERTY(BlueprintReadWrite, Category = "ListenIcon", meta = (AllowPrivateAccess = "true"))
	bool IsListening = false;

	UPROPERTY(BlueprintReadWrite, Category = "ListenIcon", meta = (AllowPrivateAccess = "true"))
	bool PrevIsListening = false;

	UPROPERTY(BlueprintReadWrite, Category = "ListenIcon", meta = (AllowPrivateAccess = "true"))
	FString CommandString = FString();

	UFUNCTION(BlueprintCallable, Category = "ListenIcon")
	bool GetIsListening();

	UFUNCTION(BlueprintCallable, Category = "ListenIcon")
	void SetIsListening(bool InIsListening);

	UFUNCTION(BlueprintCallable, Category = "ListenIcon")
	bool GetPrevIsListening();

	UFUNCTION(BlueprintCallable, Category = "ListenIcon")
	void SetPrevIsListening(bool InPrevIsListening);

	void SetCommandString(const FString& InCommandString);
};
