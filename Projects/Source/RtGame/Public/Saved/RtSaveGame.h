// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RtSaveGame.generated.h"

USTRUCT()
struct FRtPlayerSaveData
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, Category = Basic)
		FText PlayerName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex = 0;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		FName CharRowName;
	UPROPERTY(VisibleAnywhere, Category = Basic)
		AActor* CharacterClass = NULL;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		FName ModelName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		FName SkinName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		int32 PlayerScore = 0;
};

/**
 * 
 */
UCLASS()
class RTGAME_API URtSaveGame : public USaveGame
{
	GENERATED_BODY()

// Various
public:
		UPROPERTY(VisibleAnywhere, Category = Basic)
			FRtPlayerSaveData PlayerProfile;

public:
// Fucntions
		URtSaveGame();
	
};
