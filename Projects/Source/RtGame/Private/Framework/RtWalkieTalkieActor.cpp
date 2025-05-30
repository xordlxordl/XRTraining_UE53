// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtWalkieTalkieActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARtWalkieTalkieActor::ARtWalkieTalkieActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WalkieTalkieMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WalkieMesh"));
	WalkieTalkieMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = WalkieTalkieMesh;
}

// Called when the game starts or when spawned
void ARtWalkieTalkieActor::BeginPlay()
{
	Super::BeginPlay();

	// 입력 바인딩 설정
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		EnableInput(PC);
		InputComponent->BindKey(EKeys::Zero, IE_Pressed, this, &ARtWalkieTalkieActor::ToggleScreenMaterial);
	}
}

// Called every frame
void ARtWalkieTalkieActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMoving)
	{
		MoveElapsedTime += DeltaTime;
		float Alpha = FMath::Clamp(MoveElapsedTime / MoveDuration, 0.0f, 1.0f);

		FVector NewLocation = FMath::Lerp(MoveStartLocation, MoveTargetLocation, Alpha);
		SetActorRelativeLocation(NewLocation);

		if (Alpha >= 1.0f)
		{
			bIsMoving = false;
		}
	}
}

bool ARtWalkieTalkieActor::GetIsSending()
{
	return bIsSending;
}

bool ARtWalkieTalkieActor::GetIsActivated()
{
	return bIsActivated;
}

void ARtWalkieTalkieActor::SetIsActivated(bool InIsActivated)
{
	bIsActivated = InIsActivated;
}

void ARtWalkieTalkieActor::ToggleScreenMaterial()
{
	if (!WalkieTalkieMesh || !GreenMaterial || !OrangeMaterial) return;

	int32 MaterialSlotIndex = 1;

	if (bIsSending)
	{
		WalkieTalkieMesh->SetMaterial(MaterialSlotIndex, GreenMaterial);
		bIsSending = false;
	}
	else
	{
		WalkieTalkieMesh->SetMaterial(MaterialSlotIndex, OrangeMaterial);
		bIsSending = true;
	}
}

bool ARtWalkieTalkieActor::GetIsMoving()
{
	return bIsMoving;
}

void ARtWalkieTalkieActor::MoveAnimation(FVector InStartLocation, FVector InTargetLocation, float InDuration)
{
	if (InDuration <= 0.f) return;

	MoveStartLocation = InStartLocation;
	MoveTargetLocation = InTargetLocation;
	MoveDuration = InDuration;
	MoveElapsedTime = 0.0f;

	SetActorRelativeLocation(InStartLocation);
	bIsMoving = true;
}

