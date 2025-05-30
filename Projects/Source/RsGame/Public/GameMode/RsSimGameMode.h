// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RsSimGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RSGAME_API ARsSimGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:

	virtual void StartPlay() override;
};
