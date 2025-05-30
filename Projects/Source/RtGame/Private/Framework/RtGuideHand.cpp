// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtGuideHand.h"

#include "Manager/RtGameScenarioController.h"

// Sets default values
ARtGuideHand::ARtGuideHand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	DefaultRoot->SetupAttachment(RootComponent);
	DefaultRoot->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	RightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHand"));
	RightHand->SetupAttachment(DefaultRoot);
	RightHand->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

}

// Called when the game starts or when spawned
void ARtGuideHand::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARtGuideHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsGrapMode) {
		if (GrapRate > GrapDuration + 1.0f)
			IsGrapMode = false;
	}
	else {
		if (GrapRate < -ReleaseDuration)
			IsGrapMode = true;
	}
	if (IsGrapMode)
		GrapRate += GrapSpeed * DeltaTime;
	else
		GrapRate -= ReleaseSpeed * DeltaTime;

}

void ARtGuideHand::UpdateVisible()
{
	if (IsGraped) {
		RightHand->SetVisibility(false);
		return;
	}

	ARtGameScenarioController* gsc = ARtGameScenarioController::Get(GetWorld());
	ARtObjectActor* parent = Cast<ARtObjectActor>(GetAttachParentActor());
	if (!gsc->IsTargetMenuEnable(TargetMenuName)) {
		RightHand->SetVisibility(false);
		return;
	}
	if (!parent->IsVisible()) {
		RightHand->SetVisibility(false);
		return;
	}
	RightHand->SetVisibility(true);
}

