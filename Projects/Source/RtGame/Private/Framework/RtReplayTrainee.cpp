// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtReplayTrainee.h"
#include "Manager/RtReplayManagerActor.h"

#include "Camera/CameraActor.h"
#include "Common/RtGameplayTags.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

// Sets default values
ARtReplayTrainee::ARtReplayTrainee()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	DefaultRoot->SetupAttachment(RootComponent);
	DefaultRoot->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(DefaultRoot);
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	SkeletalMesh->SetVisibility(true);

}

// Called when the game starts or when spawned
void ARtReplayTrainee::BeginPlay()
{
	Super::BeginPlay();
//	SetActorRelativeTransform
}

// Called every frame
void ARtReplayTrainee::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//SkeletalMesh->SetRelativeScale3D(MotionData.Scale);

}

void ARtReplayTrainee::UpdateRenderTargetCamera()
{
	FTransform head = MotionData.Head;

	FRotator rot = FRotator(head.GetRotation());
	
	head.SetRotation(FQuat(FRotator(0, rot.Yaw + 180.0f, 0))); 	// 회전 조정.

	SetActorTransform(head);
}

void ARtReplayTrainee::ClearScencCaptureTextureTarget()
{
	USceneCaptureComponent2D* sceneCaptureCom = Cast<USceneCaptureComponent2D>(
		GetComponentByClass(USceneCaptureComponent2D::StaticClass()));

	if (sceneCaptureCom) {
		sceneCaptureCom->TextureTarget = nullptr;
	}
}

void ARtReplayTrainee::SetFirstPersonView()
{
	USceneCaptureComponent2D* sceneCaptureCom = Cast<USceneCaptureComponent2D>(
		GetComponentByClass(USceneCaptureComponent2D::StaticClass()));

	if (sceneCaptureCom) {
		FString RenderTargetPath = TEXT("/Game/RS/Control/Camera/ReplayView_RT.ReplayView_RT");
		UTextureRenderTarget2D* LoadedRenderTarget = Cast<UTextureRenderTarget2D>(
			StaticLoadObject(UTextureRenderTarget2D::StaticClass(), nullptr, *RenderTargetPath));

		if (sceneCaptureCom && LoadedRenderTarget) {
			sceneCaptureCom->TextureTarget = LoadedRenderTarget;
		}
	}
}

