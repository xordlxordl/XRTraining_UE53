// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtDirectionArrowComponent.h"
#include "Framework/RtTraineePawn.h"
#include "Common/RtBlueprintAssetPath.h"
#include "Manager/RtGameScenarioController.h"
#include "Kismet/KismetMathLibrary.h"

URtDirectionArrowComponent::URtDirectionArrowComponent(const FObjectInitializer& ObjectInitializer)
{
	SetCollisionProfileName(TEXT("NoCollision"));
	UStaticMesh* pMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, RtBlueprintAsset::DirectionArrowMesh));
	if(pMesh)
		SetStaticMesh(pMesh);

	SetRelativeScale3D(FVector(0.2f));
}

void URtDirectionArrowComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = GetOuterARtTraineePawn();
	ARtGameScenarioController* gsc = ARtGameScenarioController::Get(this);
	if (!gsc)
		return;
	
	gsc->SetSctGamePlay.AddDynamic(this, &URtDirectionArrowComponent::ResetTarget);


	SetVisibility(false);
	SetComponentTickEnabled(false);
}

void URtDirectionArrowComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Player || !TargetActor)
		return;

	FVector worldLocation = Player->GetRootPosition();
	FVector targetLocation = TargetActor->GetActorLocation();
	worldLocation.Z = 0;
	targetLocation.Z = 0;

	bool isNear = FVector::Dist(worldLocation, targetLocation) < fCheckNear;
	
	SetVisibility(!isNear);
	if (isNear)
		return;

	FRotator rot = UKismetMathLibrary::FindLookAtRotation(worldLocation, targetLocation);
	
	worldLocation += FVector::UpVector * fArrowHeight + UKismetMathLibrary::GetForwardVector(rot) * fCheckNear;
	SetWorldLocationAndRotation(worldLocation, rot);
}

void URtDirectionArrowComponent::OnRegister()
{
	PrimaryComponentTick.bCanEverTick = true;

	Super::OnRegister();
}

void URtDirectionArrowComponent::SetTargetActor(AActor * pActor)
{
	TargetActor = pActor;

	bool isValid = (bool)TargetActor;
	SetVisibility(isValid);
	SetComponentTickEnabled(isValid);
}

void URtDirectionArrowComponent::ResetTarget(bool isStart)
{
	if (isStart)
		SetTargetActor(NULL);
}
