// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_PageBase.h"

// UE Component for Widget
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
// UI
#include "Rt_Training_Viewer_Popup.generated.h"

class URt_Training_Accident_Block;

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Training_Viewer_Popup : public URt_PageBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	UFUNCTION()	FEventReply OnClicked_TapToCloseZone(FGeometry Geometry, const FPointerEvent& MouseEvent);

public:
	UPROPERTY(meta = (BindWidget)) UButton* ActiveBtn_var;
	UPROPERTY(meta = (BindWidget)) UButton* QuitBtn_var;
	UPROPERTY(meta = (BindWidget)) UBorder* TapToCloseZone_var;
	UPROPERTY(meta = (BindWidget)) UScrollBox* AccidentList_SB_var;

	UPROPERTY(meta = (BindWidget)) UTextBlock* Run_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Quit_text;
 
	FString SelectedAccident_str;

	UFUNCTION() void Add_Accident_List();
	UFUNCTION() void OnClicked_Active();
	UFUNCTION() void OnClicked_Quit();
	void CheckSelected(URt_Training_Accident_Block* InWidget);
	virtual void Close_Page() override;
};
