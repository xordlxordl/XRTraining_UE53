// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RtGuideHand.generated.h"

UCLASS()
class RTGAME_API ARtGuideHand : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARtGuideHand();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> DefaultRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> RightHand;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString TargetMenuName = TEXT("");

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void UpdateVisible();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GrapRate = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GrapSpeed = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReleaseSpeed = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GrapDuration = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReleaseDuration = 0.0f;

	bool IsGraped = false; // 유저가 상호작용 중일 때

protected:
	bool IsGrapMode = true;

};
