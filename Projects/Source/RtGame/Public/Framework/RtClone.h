// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RtClone.generated.h"

class ARtTraineePawn;

UCLASS()
class RTGAME_API ARtClone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARtClone();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> DefaultRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> DefaultMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TrackerPelvisMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TrackerLeftFootMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TrackerRightFootMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TrackerLeftHandMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TrackerRightHandMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsMirror = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ShowOption(float Yaw, bool isM);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ARtTraineePawn* TraineePawn = nullptr;

public:
	void CalibrationClone();
	void ShowClone();
	void ShowMirrorClone();
	void HideClone();
};
