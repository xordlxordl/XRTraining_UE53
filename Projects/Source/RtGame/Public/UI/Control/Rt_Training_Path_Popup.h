// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_PageBase.h"
// UI
#include "Components/Border.h"
#include "Components/TextBlock.h"

#include "Rt_Training_Path_Popup.generated.h"

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Training_Path_Popup : public URt_PageBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget)) UBorder* TapToCloseZone_var;

	UPROPERTY(meta = (BindWidget)) UTextBlock* TrainingPath_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TrainingPathView_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* LeaningPath_text;
	virtual void Close_Page() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	UFUNCTION()	FEventReply OnClicked_TapToCloseZone(FGeometry Geometry, const FPointerEvent& MouseEvent);
	
    
};
