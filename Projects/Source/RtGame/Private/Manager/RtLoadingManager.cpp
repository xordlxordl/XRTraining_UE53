// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/RtLoadingManager.h"
// UE
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
// Framework
#include "Framework/RtGameMode.h"
#include "Framework/RtPlayerController.h"
// Common
#include "Common/RtBlueprintAssetPath.h"

// UI
#include "UI/Control/Rt_Control_Loading_Popup.h"

URtLoadingManager::FLoadingFinished URtLoadingManager::Update_OnLoadingFinished_Delegate;

void URtLoadingManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void URtLoadingManager::Deinitialize()
{
    Super::Deinitialize();

    // dave
    URtLoadingManager::Update_OnLoadingFinished_Delegate.RemoveAll(this);
}


void URtLoadingManager::LoadLevelWithLoadingScreen(FName LevelName, APlayerController* InPC)
{
    LoadingScreenWidgetClass = LoadClass<UUserWidget>(nullptr, RtBlueprintAsset::LoadingScreenWidget);
    if (isLoading) return;

    // Create and show the loading screen widget
    if (LoadingScreenWidgetClass)
    {
        LoadingScreenWidget = CreateWidget<UUserWidget>(InPC, LoadingScreenWidgetClass);
        if (LoadingScreenWidget)
        {
            UE_LOG(LogTemp, Warning, TEXT("CreateWidget from URtLoadingManager::LoadLevelWithLoadingScreen"));
            LoadingScreenWidget->AddToViewport(100);
            isLoading = true;
        }
    }

    // Load the level asynchronously
    FLatentActionInfo LatentInfo;
    LatentInfo.CallbackTarget = this;
    //LatentInfo.ExecutionFunction = "OnLevelLoaded";
    LatentInfo.Linkage = 0;
    LatentInfo.UUID = __LINE__;
    UGameplayStatics::LoadStreamLevel(this, LevelName, true, true, LatentInfo);
}

void URtLoadingManager::LoadLoadingPopup(APlayerController* InPC, FString InContens)
{

    LoadingPopupWidgetClass = LoadClass<UUserWidget>(nullptr, RtBlueprintAsset::LoadingPopupWidget);

    // Create and show the loading screen widget
    if (LoadingPopupWidgetClass)
    {
        LoadingPopupWidget = CreateWidget<UUserWidget>(InPC, LoadingPopupWidgetClass);
        if (LoadingPopupWidget)
        {
            LoadingPopupWidget->AddToViewport();
            URt_Control_Loading_Popup* loadpopupclass = Cast<URt_Control_Loading_Popup>(LoadingPopupWidget);
            if (loadpopupclass)
            {
                loadpopupclass->SetTextBlock(InContens);
            }
            isLoading = true;
        }
    }
}

void URtLoadingManager::OnLevelLoaded()
{
    // Remove the loading screen widget
    if (LoadingScreenWidget.Get())
    {
        isLoading = false;
        LoadingScreenWidget->RemoveFromParent();

        // dave
        if (Update_OnLoadingFinished_Delegate.IsBound())
        {
            Update_OnLoadingFinished_Delegate.Broadcast();
        }

        UE_LOG(LogTemp, Warning, TEXT("RemoveFromParent() from URtLoadingManager::OnLevelLoaded"));
    }
}

void URtLoadingManager::OnLevelLoaded_TypePopup()
{
    // Remove the loading popup widget
    if (LoadingPopupWidget.Get())
    {
        isLoading = false;
        LoadingPopupWidget->RemoveFromParent();
    }
}


