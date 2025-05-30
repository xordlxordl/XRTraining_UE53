// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"
// UE 
#include "Components/TextBlock.h"
#include "Components/Button.h"

// UI
#include "UI/Control/Rt_Training_Viewer_Popup.h"
#include "Rt_Training_Accident_Block.generated.h"

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Training_Accident_Block : public URt_UserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget)) UTextBlock* Accident_var;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) UButton* AccidentBtn_var;

// Func 
	UFUNCTION() void OnClicked_Accident();

	URt_Training_Viewer_Popup* parentWidget;

	// delegate æ≤∏È ±ª¿Ã?
	//FString Get_SelectedAccident_str() { return selectedAccident; }

	UPROPERTY(BlueprintReadWrite) bool bIsSelected = false;

protected:
	virtual void NativeOnInitialized() override;
	FString selectedAccident;


};
