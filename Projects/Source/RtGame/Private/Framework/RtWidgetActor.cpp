// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/RtWidgetActor.h"
#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Common/RtBlueprintAssetPath.h"

#include "Framework/RtNpcPawn.h"
#include "RtResourceModel/Public/RtObjectActor.h"
#include "RtResourceModel/Public/RtObjectOptionComponent.h"
#include "UI/Trainee/Rt_Trainee_Text.h"
#include "UI/Trainee/Rt_Trainee_SelectionMenu.h"

// Sets default values
ARtWidgetActor::ARtWidgetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	DefaultRoot->SetupAttachment(RootComponent);

	RelativeRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RelativeRoot"));
	RelativeRoot->SetupAttachment(DefaultRoot);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RelativeRoot);
	WidgetComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ARtWidgetActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ARtWidgetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (APlayerCameraManager* cameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager)
	{
		FVector cameraLocation = cameraManager->GetCameraLocation();
		FVector newLocation;
		FRotator newRotation = FRotator().ZeroRotator;

		if (FollowTarget == nullptr) // Screen Mode
		{
			newLocation = cameraLocation;
			newRotation = cameraManager->GetCameraRotation();

			SetActorLocation(newLocation);
			SetActorRotation(newRotation);

			FRotator newWidgetRotation = UKismetMathLibrary::FindLookAtRotation(WidgetComponent->GetComponentLocation(), cameraLocation);
			WidgetComponent->SetWorldRotation(newWidgetRotation);
		}
		else
		{
			newLocation = FollowTarget->GetActorLocation();
			SetActorLocation(newLocation);
			newRotation = FollowTarget->GetActorRotation();
			SetActorRotation(newRotation);
			newRotation = UKismetMathLibrary::FindLookAtRotation(cameraLocation, RelativeRoot->GetComponentLocation());
			RelativeRoot->SetWorldRotation(newRotation);

			newRotation = UKismetMathLibrary::FindLookAtRotation(WidgetComponent->GetComponentLocation(), cameraLocation);
			WidgetComponent->SetWorldRotation(newRotation);
		}

		if (FollowTarget == nullptr)
			return;
		if (IsFixScale == false)
			return;

		double scale;
		float distanceToTarget = FVector::Distance(cameraLocation, FollowTarget->GetActorLocation());

		if (IsFixScale == false)
		{
			scale = distanceToTarget / Length + 1;
			scale *= WidgetScale * ScaleRate / Length;
		}
		else
		{
			scale = distanceToTarget / Length + 1;
			scale *= WidgetScale * ScaleRate;
		}

		WidgetComponent->SetRelativeScale3D(FVector(scale));
	}
}

UUserWidget* ARtWidgetActor::Init(const FString widgetPath)
{
	TSubclassOf<UUserWidget> widgetClass = LoadClass<UUserWidget>(nullptr, *widgetPath);
	if (widgetClass == nullptr)
		return nullptr;

	UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), widgetClass);
	if (widget == nullptr)
		return nullptr;

	WidgetComponent->SetWidget(widget);

	if (URt_Trainee_SelectionMenu* selectionMenu = Cast<URt_Trainee_SelectionMenu>(widget))
	{
		selectionMenu->SetOwner_WidgetActor(this);
	}

	SetTarget();
	SetWidgetScale();

	return widget;
}

void ARtWidgetActor::SetWidgetSize(const float widgetSizeX, const float widgetSizeY)
{
	WidgetComponent->SetDrawSize(FVector2D(widgetSizeX, widgetSizeY));

	//if (URt_Trainee_SelectionMenu* selectionMenu = Cast<URt_Trainee_SelectionMenu>(WidgetComponent->GetWidget()))
	//{
	//	selectionMenu->SetScrollHeight(widgetSizeY);
	//}
}

void ARtWidgetActor::SetPivot(const float pivotX, const float pivotY)
{
	WidgetComponent->SetPivot(FVector2D(pivotX, pivotY));
}

void ARtWidgetActor::SetWidgetRelativeLocation(const float locX, const float locY)
{
	FVector location = RelativeRoot->GetComponentLocation();
	location.Y = locX;
	location.Z = locY;
	RelativeRoot->SetRelativeLocation(location);
}

void ARtWidgetActor::SetWidgetScale(const float widgetScale)
{
	WidgetScale = widgetScale;
	WidgetComponent->SetRelativeScale3D(FVector(WidgetScale * ScaleRate));
}

void ARtWidgetActor::SetTarget(AActor* followTarget, FVector worldLocation)
{
	FollowTarget = followTarget;

	if (FollowTarget == nullptr)
	{
		FVector location = RelativeRoot->GetComponentLocation();
		location.X = Length;
		RelativeRoot->SetRelativeLocation(location);
	}
	else
	{
		if (APlayerCameraManager* cameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager)
		{
			FVector cameraLocation = cameraManager->GetCameraLocation();
			FVector newLocation;
			FRotator newRotation = FRotator().ZeroRotator;

			newLocation = FollowTarget->GetActorLocation();
			SetActorLocation(newLocation);
			newRotation = FollowTarget->GetActorRotation();
			SetActorRotation(newRotation);
			RelativeRoot->SetWorldLocation(worldLocation);

			FVector location = FVector::ZeroVector;
			location.X = Length * 2;
			WidgetComponent->SetRelativeLocation(location);
		}
	}
}

void ARtWidgetActor::DestroyWidgetActor()
{
	if(WidgetComponent)
		WidgetComponent->SetWidget(nullptr);
	//UKismetSystemLibrary::CollectGarbage();
	Destroy();
}

void ARtWidgetActor::SetLength(float length)
{
	Length = length;
}

ARtWidgetActor* ARtWidgetActor::CreateRtWidgetActor()
{
	UClass* rtWidgetActorClass = LoadClass<ARtWidgetActor>(nullptr, RtTraineeWidgetBlueprint::RtWidgetActor);
	if (rtWidgetActorClass == nullptr)
		return nullptr;

	UWorld* world = GEngine->GameViewport->GetWorld();
	ARtWidgetActor* rtWidgetActor = world->SpawnActor<ARtWidgetActor>(rtWidgetActorClass, FTransform());

	return rtWidgetActor;
}

