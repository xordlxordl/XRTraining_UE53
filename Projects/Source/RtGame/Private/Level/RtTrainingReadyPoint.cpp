// Fill out your copyright notice in the Description page of Project Settings.
// 스타트 포인트 호출을 SCT에서 하게 되어 폐기

#include "Level/RtTrainingReadyPoint.h"
// UE
#include "UObject/ConstructorHelpers.h"
#include "Components/ArrowComponent.h"
#include "Components/BillboardComponent.h"
#include "Engine/Texture2D.h"
#include "Components/DecalComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GamePlayStatics.h"

// Framework
#include "Framework/RtGameMode.h"
#include "Framework/RtTraineePawn.h"
#include "Framework/RtPlayerState.h"
#include "Framework/RtGameState.h"

DEFINE_LOG_CATEGORY(LogStartPoint);

ARtTrainingReadyPoint::ARtTrainingReadyPoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//Owner_PlayerId = 0;
	bReplicates = true;
	DecalNormalColor = FLinearColor::Blue;
	DecalEffectColor = FLinearColor::Red;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));

	RootComponent = SceneComponent;

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	if (DecalComponent) {
		DecalComponent->SetupAttachment(SceneComponent);
		DecalComponent->SetRelativeRotation(FRotator(90, 0, 0));
		DecalComponent->SetupAttachment(RootComponent);
	}

	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextComp"));
	if (TextComponent) {
		TextComponent->SetText(FText::FromString(TEXT("")));
		TextComponent->SetHorizontalAlignment(EHTA_Center);
		TextComponent->SetupAttachment(RootComponent);
	}

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	if (CapsuleComponent) {
		CapsuleComponent->SetCapsuleHalfHeight(400.f);
		CapsuleComponent->SetCapsuleRadius(200.f);


		CapsuleComponent->SetVisibility(false);
		CapsuleComponent->bHiddenInGame = true;
		
		CapsuleComponent->SetupAttachment(RootComponent);

		CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OverlapBegin);
		CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OverlapEnd);
	}
}

void ARtTrainingReadyPoint::OnRep_IsValidSPoint()
{
}

void ARtTrainingReadyPoint::OnRep_ChangedState()
{
}

void ARtTrainingReadyPoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARtTrainingReadyPoint, State);
	DOREPLIFETIME(ARtTrainingReadyPoint, IsValidSPoint);
	//DOREPLIFETIME(ARtTrainingReadyPoint, Owner_PlayerId);
}

// Sets default values
ARtTrainingReadyPoint::ARtTrainingReadyPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARtTrainingReadyPoint::BeginPlay()
{
	Super::BeginPlay();

	if (auto* mat = DecalComponent->GetDecalMaterial()) {
		DecalMaterial = UMaterialInstanceDynamic::Create(mat, NULL);
		DecalComponent->SetDecalMaterial(DecalMaterial);
	}
}

void ARtTrainingReadyPoint::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* world = GetWorld();
	if (!world)
		return;
	if (HasAuthority() == false) {
		UE_LOG(LogStartPoint, Warning, TEXT("RTDEBUG: ARtTrainingReadyPoint::4"));
		return;
	}
	UE_LOG(LogStartPoint, Warning, TEXT("RTDEBUG: ARtTrainingReadyPoint::5"));

	ARtTraineePawn* TempPawn = Cast<ARtTraineePawn>(OtherActor);
	if (TempPawn)
	{
		UE_LOG(LogStartPoint, Warning, TEXT("RTDEBUG: ARtTrainingReadyPoint::6"));
		if (auto* gm = Cast<ARtGameMode>(world->GetAuthGameMode())) {
			if (auto* ps = TempPawn->Get_PlayerState()) {
				int32 point_index = ps->Get_PlayerInfo().StartTargetPointIndex;
				UE_LOG(LogStartPoint, Warning, TEXT("RTDEBUG: ARtTrainingReadyPoint::7"));
				if (point_index == -1) {
					UE_LOG(LogStartPoint, Warning, TEXT("Senario start point has not been set"))
				}
				if (point_index == Index) {

					Change_State(ERtStartPoint_State::Visible_Occupied);
					//gm->Occupied_StartPoint(TempPawn->Get_PlayerController(), Index, true);
					UE_LOG(LogStartPoint, Warning, TEXT("RTDEBUG: ARtTrainingReadyPoint::9"));
				}
			}
			UE_LOG(LogStartPoint, Warning, TEXT("RTDEBUG: ARsStartTargetPoint::10"));
		}
	}
	UE_LOG(LogStartPoint, Warning, TEXT("RTDEBUG: ARtTrainingReadyPoint::12"));
}

void ARtTrainingReadyPoint::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HasAuthority() == false) {
		return;
	}
	auto* world = GetWorld();
	if (!world)
		return;

	ARtTraineePawn* TempPawn = Cast<ARtTraineePawn>(OtherActor);

	if (TempPawn)
	{
		if (auto* gm = Cast<ARtGameMode>(world->GetAuthGameMode())) {
			if (auto* ps = TempPawn->Get_PlayerState()) {
				int32 point_index = ps->Get_PlayerInfo().StartTargetPointIndex;
				if (point_index == -1) {
					UE_LOG(LogStartPoint, Warning, TEXT("Senario start point has not been set"))
				}
				if (point_index == Index) {
					Change_State(ERtStartPoint_State::Visible_Unoccupied);
					//gm->Occupied_StartPoint(TempPawn->Get_PlayerController(), Index, false);
					UE_LOG(LogStartPoint, Warning, TEXT("RTDEBUG: ARtTrainingReadyPoint::202"));
				}
			}
		}
		UE_LOG(LogStartPoint, Warning, TEXT("RTDEBUG: ARtTrainingReadyPoint::203"));
	}
	UE_LOG(LogStartPoint, Warning, TEXT("RTDEBUG: ARtTrainingReadyPoint::204"));
}

void ARtTrainingReadyPoint::Change_State(ERtStartPoint_State InState)
{
	State = InState;
	if (HasAuthority()) {
		Inter_Change_State(State);
	}
}

void ARtTrainingReadyPoint::Inter_Change_State(ERtStartPoint_State InState)
{
	switch (State)
	{
	case ERtStartPoint_State::Visible_Unoccupied:
	{
		Show_TargetPoint(true);
		if (DecalMaterial)
			DecalMaterial->SetVectorParameterValue(TEXT("Color"), DecalNormalColor);
	}
	break;
	case ERtStartPoint_State::Visible_Occupied:
	{
		Show_TargetPoint(true);
		if (DecalMaterial)
			DecalMaterial->SetVectorParameterValue(TEXT("Color"), DecalEffectColor);

	}
	break;
	default:
		break;
	}
}

// Called every frame
void ARtTrainingReadyPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ARtTrainingReadyPoint::Get_Index()
{
	return Index;
}

void ARtTrainingReadyPoint::Show_TargetPoint(bool InParam)
{
	DecalComponent->SetVisibility(InParam);

	if (TextComponent) {
		if (InParam) {
			TextComponent->SetVisibility(InParam);
		}
		else {
			TextComponent->SetVisibility(InParam);
		}
	}
}

void ARtTrainingReadyPoint::Set_Text(const FString& InStr)
{
	if (TextComponent) {
		TextComponent->SetText(FText::FromString(InStr));
	}
}

void ARtTrainingReadyPoint::Set_IsValidSPoint(bool InBool)
{
	IsValidSPoint = InBool;
	if (HasAuthority()) {
		OnRep_IsValidSPoint();
	}
}

void ARtTrainingReadyPoint::DisableActor(bool toHide)
{
	// Hides visible components
	SetActorHiddenInGame(toHide);

	if (DecalComponent)
		DecalComponent->SetHiddenInGame(toHide);
}

