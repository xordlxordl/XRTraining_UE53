// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RtVRMeshes.generated.h"

class ARtTraineePawn;

UCLASS()
class RTGAME_API ARtVRMeshes : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARtVRMeshes();

public:
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//UStaticMeshComponent* HMDMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* LeftControllerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* RightControllerMesh;

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
	UStaticMeshComponent* TrackerRifleMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void Init(ARtTraineePawn* traineePawn);
	void SetTrackersVisibility(bool isVisible);

protected:
	TWeakObjectPtr<ARtTraineePawn> OwnerTraineePawn;

	TArray<UStaticMeshComponent*> Trackers;
};
