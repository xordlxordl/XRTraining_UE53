// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ViveOpenXRPassthroughFunctionLibrary.h"
#include "RtVRManager.generated.h"

RTGAME_API DECLARE_LOG_CATEGORY_EXTERN(RtVRManager, Log, All);

UCLASS()
class RTGAME_API ARtVRManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARtVRManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FPassthroughHandle PassthroughHandle;
};
