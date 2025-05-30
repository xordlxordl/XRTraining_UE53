// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_PageBase.h"
// UE
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
// Data
#include "Common/RtRpcCommon.h"

#include "Rt_Control_SelectMap.generated.h"

class URt_Control_SelectMap_List;
class URt_Control_SelectMap_Info;

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Control_SelectMap : public URt_PageBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	bool bNextStep;


public:
	UPROPERTY(meta = (BindWidget))	UTextBlock* ScenarioTitle_text;
	UPROPERTY(meta = (BindWidget))	UTextBlock* ScenarioTitleToolTip_text;
	UPROPERTY(meta = (BindWidget))	UTextBlock* ScenarioList_text;
	UPROPERTY(meta = (BindWidget))	UTextBlock* Logout_text;
	UPROPERTY(meta = (BindWidget))	UTextBlock* Complete_text;

	UPROPERTY(meta = (BindWidget))	UButton* SelectContent_Btn;
	UPROPERTY(meta = (BindWidget))	UButton* LogOut_Btn;
	UPROPERTY(meta = (BindWidget))  URt_Control_SelectMap_List* WBP_Map_List;
	UPROPERTY(meta = (BindWidget))  URt_Control_SelectMap_Info* WBP_ContentsInfo;

	// Func
	UFUNCTION() void OnClicked_SelectContent();
	UFUNCTION() void OnClicked_LogOut();

	bool HavingSelectedBlock= false;

	UFUNCTION() void OpenNextPage();
	virtual void NativeOnInitialized() override;
	
	void SetCurrScenarioInfo(const FRtScenarioInfo& InInfo);



};
