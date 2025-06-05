// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "RtLoadingManager.generated.h"

/**
 *
 */
UCLASS()
class RTGAME_API URtLoadingManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    DECLARE_MULTICAST_DELEGATE(FLoadingFinished);
    static FLoadingFinished Update_OnLoadingFinished_Delegate;

    UFUNCTION(BlueprintCallable)
    void LoadLevelWithLoadingScreen(FName LevelName, APlayerController* InPC);
    UFUNCTION(BlueprintCallable)
    void  LoadLoadingPopup(APlayerController* InPC, FString InContens);
    UFUNCTION(BlueprintCallable)
    void OnLevelLoaded();
    UFUNCTION(BlueprintCallable)
    void OnLevelLoaded_TypePopup();

    UFUNCTION(BlueprintCallable)
    void LoadLoadingPopupDelayed(APlayerController* InPC, FString InContents);
    UFUNCTION(BlueprintCallable)
    void HideLoadingPopup();

private:
    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> LoadingScreenWidgetClass;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> LoadingPopupWidgetClass;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UUserWidget> LoadingScreenWidget;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UUserWidget> LoadingPopupWidget;

    FTimerHandle DelayedLoadingPopupTimerHandle;

    bool isLoading = false;
    bool bIsPopupRequested = false;
    bool bIsPopupVisible = false;

    float PopupDelayThreshold = 0.2f;
    float PendingLoadingPopupStartTime = 0.0f;
    float LoadingPopupStartTime = 0.0f;

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
};
