// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtWidgetActor_ListenIcon.h"
#include "Framework/RtListenIconUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/WidgetComponent.h"

// Sets default values
ARtWidgetActor_ListenIcon::ARtWidgetActor_ListenIcon()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // WidgetComponent 생성
    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
    RootComponent = WidgetComponent;

    WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
}

// Called when the game starts or when spawned
void ARtWidgetActor_ListenIcon::BeginPlay()
{
    Super::BeginPlay();

    SetIsVisible(true);
}

void ARtWidgetActor_ListenIcon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateBillboardRotation();
}

void ARtWidgetActor_ListenIcon::InitListenIcon()
{
    SetIsVisible(true);
    SetTargetMenuName(TEXT(""));
    SetVoiceCommandList({});
}

FString ARtWidgetActor_ListenIcon::GetTargetMenuName() const
{
    return TargetMenuName;
}

void ARtWidgetActor_ListenIcon::SetTargetMenuName(const FString& InMenuName)
{
    TargetMenuName = InMenuName;

    UUserWidget* UserWidget = WidgetComponent->GetUserWidgetObject();
    if (URtListenIconUserWidget* ListenIconWidget = Cast<URtListenIconUserWidget>(UserWidget))
    {
        ListenIconWidget->SetCommandString(TargetMenuName);
    }
}

const TArray<FString>& ARtWidgetActor_ListenIcon::GetVoiceCommandList() const
{
    return VoiceCommandList;
}

void ARtWidgetActor_ListenIcon::SetVoiceCommandList(const TArray<FString>& NewList)
{
    VoiceCommandList = NewList;
}

bool ARtWidgetActor_ListenIcon::GetIsVisible()
{
    return IsVisible;
}

void ARtWidgetActor_ListenIcon::SetIsVisible(bool InIsVisible)
{
    IsVisible = InIsVisible;

    SetActorHiddenInGame(!IsVisible);
}

UWidgetComponent* ARtWidgetActor_ListenIcon::GetWidgetComponent()
{
    return WidgetComponent;
}

void ARtWidgetActor_ListenIcon::UpdateBillboardRotation()
{
    if (!IsVisible)
    {
        //SetIconColor(FLinearColor::White.CopyWithNewOpacity(0.0f));
        return;
    }

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    APlayerCameraManager* CameraManager = PC->PlayerCameraManager;
    if (!CameraManager) return;

    // 내 위치 -> 카메라 위치 방향 벡터 계산
    FVector ToCamera = CameraManager->GetCameraLocation() - GetActorLocation();

    // 회전값으로 변환 (Yaw만 반영)
    FRotator LookAtRotation = ToCamera.Rotation();
    FRotator NewRotation = FRotator(0.0f, LookAtRotation.Yaw, 0.0f);

    // 회전 적용
    SetActorRotation(NewRotation);
}