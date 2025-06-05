// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtStartAreaEffect.h"
#include "Manager/RtGameScenarioController.h"
#include "Framework/RtTraineePawn.h"
#include "CaptureDevice.h"
#include "Camera/CameraComponent.h"
#include "NavigationSystem.h"

#include "Components/SplineComponent.h"
#include "UI/Trainee/Rt_Trainee_Main.h"

// Sets default values
ARtStartAreaEffect::ARtStartAreaEffect(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bNetLoadOnClient = false;
	bReplicates = false;
	GetStaticMeshComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	GetStaticMeshComponent()->SetRelativeScale3D(FVector(fCheckRange / 50.f, fCheckRange / 50.f, 1));

	//PathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	//PathSpline->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARtStartAreaEffect::BeginPlay()
{
	Super::BeginPlay();

	CreateDynamicMaterial();
	SetEffectColor(FColor::Yellow);

	ARtGameScenarioController* pController = ARtGameScenarioController::Get(this);
	if (!pController)
		return;

	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	pController->SetSctGamePlay.AddDynamic(this, &ARtStartAreaEffect::OnStartTraining);
}

void ARtStartAreaEffect::Tick(float DeltaTime)
{
	Check_Area();
}

bool ARtStartAreaEffect::CreateDynamicMaterial()
{
	TObjectPtr<UStaticMeshComponent> staticmesh = GetStaticMeshComponent();
	if (!staticmesh)
		return false;

	TObjectPtr<UMaterialInterface> mat = staticmesh->GetMaterial(0);
	if (!mat)
		return false;

	MID = staticmesh->CreateDynamicMaterialInstance(0, mat);

	return IsValid(MID);
}

void ARtStartAreaEffect::OnStartTraining(bool isStart)
{
	ARtGameScenarioController* pController = ARtGameScenarioController::Get(this);
	
	player->GetMainWidget()->RemoveSctText();

	if (pController && pController->SetSctGamePlay.IsAlreadyBound(this, &ARtStartAreaEffect::OnStartTraining))
		pController->SetSctGamePlay.RemoveDynamic(this, &ARtStartAreaEffect::OnStartTraining);

	Destroy();
}

void ARtStartAreaEffect::SetEffectColor(const FColor& color)
{
	if (!MID)
		return;

	MID->SetVectorParameterValue(ColorParameterName, color);
}

void ARtStartAreaEffect::Check_Area()
{
	if (!player)
		return;

	FVector Location = player->GetRootPosition();

	bool result = FVector::Dist2D(GetActorLocation(), Location) < fCheckRange;

	ARtPlayerState* pState = player->Get_PlayerState();
	if (pState)
	{
		FRtDeviceInfo info = pState->Get_PlayerInfo();
		if (info.InStartingPoint != result)
		{
			OnChangeInPlayer(result);
			pState->Change_ReadyStartingPoint(result);
		}
	}
}

//void ARtStartAreaEffect::Setup_Path(UNavigationPath* AffectedPath, TEnumAsByte<ENavPathEvent::Type> PathEvent)
//{
//	if (PathEvent >= ENavPathEvent::Invalidated && PathEvent <= ENavPathEvent::RePathFailed)
//		return;
//
//	PathSpline->ClearSplinePoints();
//
//	FTransform worldTransform = GetActorTransform();
//
//	for (FVector point : AffectedPath->PathPoints)
//	{
//		FVector location = point;
//		if (AffectedPath->PathPoints.Num() - 1 == PathSpline->GetNumberOfSplinePoints())
//			location = player->GetRootPosition();
//
//		FSplinePoint splinePoint(PathSpline->GetNumberOfSplinePoints(), worldTransform.InverseTransformPositionNoScale(location));
//
//		splinePoint.Type = ESplinePointType::Linear;
//		PathSpline->AddPoint(splinePoint);
//
//		//UKismetSystemLibrary::DrawDebugSphere(this, point, 10);
//	}
//}

void ARtStartAreaEffect::OnChangeInPlayer(bool isIn)
{
	SetEffectColor(isIn ? FColor::Green : FColor::Yellow);
}

void ARtStartAreaEffect::SetPlayer(ARtTraineePawn* pPlayer)
{
	player = pPlayer;

	//if (!player)
	//	return;

	//NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), player->GetTraineeLocActor());
	//NavPath->PathUpdatedNotifier.AddDynamic(this, &ARtStartAreaEffect::Setup_Path);
}