// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"
// UE
#include "Components/TextBlock.h"
#include "Components/Button.h"
// Data
#include "Common/RtRpcCommon.h"

#include "Rt_Control_SelectMap_Block.generated.h"

class URt_Control_SelectMap_List;

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Control_SelectMap_Block : public URt_UserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))	UTextBlock* TB_ScenarioName;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)  UButton* Scenario_Btn;

	URt_Control_SelectMap_List* ParentsWidget;
	FRtScenarioInfo ScenarioInfo;

	UPROPERTY(BlueprintReadOnly) bool IsActive = false;

	virtual void NativeOnInitialized() override;
	//virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	UFUNCTION() void FillScenarioInfo();

// Function 
	void Set_ScenarioName(const FRtScenarioInfo& InStruct);

};
