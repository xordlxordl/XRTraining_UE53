// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Manager/RtReplayManagerActor.h"
#include "RtReplayTrainee.generated.h"

UCLASS()
class RTGAME_API ARtReplayTrainee : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARtReplayTrainee();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> DefaultRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateRenderTargetCamera();
	void ClearScencCaptureTextureTarget();
	void SetFirstPersonView();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMotionTransformData MotionData;

	FString RoleID = TEXT("");

};
