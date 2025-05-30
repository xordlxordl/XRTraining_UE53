// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_PageBase.h"

// UI
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

#include "Rt_Control_Loading_Popup.generated.h"

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Control_Loading_Popup : public URt_PageBase
{
	GENERATED_BODY()

public:

	// Don't need Close this wiget
	UPROPERTY(meta = (BindWidget)) UBorder* TapToCloseZone_var;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_Contents;

	virtual void Close_Page() override;

	void SetTextBlock(FString incontents);

};
