// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtVRMeshes.h"
#include "Framework/RtTraineePawn.h"

// Sets default values
ARtVRMeshes::ARtVRMeshes()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//HMDMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HMDMesh"));
	//HMDMesh->SetCollisionProfileName(FName("NoCollision"));

	LeftControllerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftControllerMesh"));
	LeftControllerMesh->SetCollisionProfileName(FName("NoCollision"));
	LeftControllerMesh->SetVisibility(false);

	RightControllerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightControllerMesh"));
	RightControllerMesh->SetCollisionProfileName(FName("NoCollision"));
	RightControllerMesh->SetVisibility(false);

	TrackerPelvisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrackerPelvisMesh"));
	TrackerPelvisMesh->SetCollisionProfileName(FName("NoCollision"));
	TrackerPelvisMesh->SetVisibility(false);

	TrackerLeftFootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrackerLeftFootMesh"));
	TrackerLeftFootMesh->SetCollisionProfileName(FName("NoCollision"));
	TrackerLeftFootMesh->SetVisibility(false);

	TrackerRightFootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrackerRightFootMesh"));
	TrackerRightFootMesh->SetCollisionProfileName(FName("NoCollision"));
	TrackerRightFootMesh->SetVisibility(false);

	TrackerLeftHandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrackerLeftHandMesh"));
	TrackerLeftHandMesh->SetCollisionProfileName(FName("NoCollision"));
	TrackerLeftHandMesh->SetVisibility(false);

	TrackerRightHandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrackerRightHandMesh"));
	TrackerRightHandMesh->SetCollisionProfileName(FName("NoCollision"));
	TrackerRightHandMesh->SetVisibility(false);

	TrackerRifleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrackerRifleMesh"));
	TrackerRifleMesh->SetCollisionProfileName(FName("NoCollision"));
	TrackerRifleMesh->SetVisibility(false);

	Trackers = { TrackerPelvisMesh, TrackerLeftFootMesh, TrackerRightFootMesh, TrackerLeftHandMesh, TrackerRightHandMesh };
}

// Called when the game starts or when spawned
void ARtVRMeshes::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARtVRMeshes::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//LeftControllerMesh->SetVisibility(OwnerTraineePawn->IsTracking(OwnerTraineePawn->LeftController));
	//RightControllerMesh->SetVisibility(OwnerTraineePawn->IsTracking(OwnerTraineePawn->RightController));

	TrackerRifleMesh->SetVisibility(!OwnerTraineePawn->IsGamePlay()
		&& OwnerTraineePawn->IsTracking(OwnerTraineePawn->TrackerRifle));
	
}

void ARtVRMeshes::Init(ARtTraineePawn* traineePawn)
{
	OwnerTraineePawn = TWeakObjectPtr<ARtTraineePawn>(traineePawn);

	if (OwnerTraineePawn == nullptr)
		return;

	// Attach to Motion Controller Components
	LeftControllerMesh->AttachToComponent(Cast<USceneComponent>(OwnerTraineePawn->LeftController), FAttachmentTransformRules::KeepRelativeTransform);
	RightControllerMesh->AttachToComponent(Cast<USceneComponent>(OwnerTraineePawn->RightController), FAttachmentTransformRules::KeepRelativeTransform);
	TrackerPelvisMesh->AttachToComponent(Cast<USceneComponent>(OwnerTraineePawn->TrackerPelvis), FAttachmentTransformRules::KeepRelativeTransform);
	TrackerLeftFootMesh->AttachToComponent(Cast<USceneComponent>(OwnerTraineePawn->TrackerLeftFoot), FAttachmentTransformRules::KeepRelativeTransform);
	TrackerRightFootMesh->AttachToComponent(Cast<USceneComponent>(OwnerTraineePawn->TrackerRightFoot), FAttachmentTransformRules::KeepRelativeTransform);
	//TrackerLeftHandMesh->AttachToComponent(Cast<USceneComponent>(OwnerTraineePawn->TrackerLeftHand), FAttachmentTransformRules::KeepRelativeTransform);
	//TrackerRightHandMesh->AttachToComponent(Cast<USceneComponent>(OwnerTraineePawn->TrackerRightHand), FAttachmentTransformRules::KeepRelativeTransform);
	TrackerRifleMesh->AttachToComponent(Cast<USceneComponent>(OwnerTraineePawn->TrackerRifle), FAttachmentTransformRules::KeepRelativeTransform);	
}

void ARtVRMeshes::SetTrackersVisibility(bool isVisible)
{
	for (UStaticMeshComponent* tracker : Trackers)
		tracker->SetVisibility(isVisible);
}

