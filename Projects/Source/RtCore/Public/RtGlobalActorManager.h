// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RtGlobalActorManager.generated.h"

/**
 * 
 */
UCLASS()
class RTCORE_API URtGlobalActorManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	TMap<FString, AActor*> ManagerMap;

};
