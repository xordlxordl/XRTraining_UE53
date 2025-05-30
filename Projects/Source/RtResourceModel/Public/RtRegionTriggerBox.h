// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "RtRegionTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class RTRESOURCEMODEL_API ARtRegionTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
public:
	ARtRegionTriggerBox();

	void Init();

	UFUNCTION()
	void OnActorBeginOverlaped(AActor* pOverlappedActor, AActor* pOtherActor);

};
