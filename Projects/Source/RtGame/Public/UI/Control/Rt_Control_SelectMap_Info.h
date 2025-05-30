// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"
// UE
#include "Components/TextBlock.h"
// Data
#include "Common/RtRpcCommon.h"


#include "Rt_Control_SelectMap_Info.generated.h"



/**
 * 
 */
UCLASS()
class RTGAME_API URt_Control_SelectMap_Info : public URt_UserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))	UTextBlock* ScenarioInfo_text;
	UPROPERTY(meta = (BindWidget))	UTextBlock* ScenarioName_text;
	UPROPERTY(meta = (BindWidget))	UTextBlock* Summary_text;
	UPROPERTY(meta = (BindWidget))	UTextBlock* Participants_text;
	UPROPERTY(meta = (BindWidget))	UTextBlock* MCTFileName_text;
	UPROPERTY(meta = (BindWidget))	UTextBlock* Memo_text;



	UPROPERTY(meta = (BindWidget))	UTextBlock* TB_Destination;
	UPROPERTY(meta = (BindWidget))  UTextBlock* TB_FileName;
	UPROPERTY(meta = (BindWidget))  UTextBlock* TB_PlayerNum_var;
	UPROPERTY(meta = (BindWidget))  UTextBlock* TB_Summ;
	FString SelectedFileName;

	// Function

	void FillContentsTextBox(const FRtScenarioInfo& ScenarioInfo);


protected:
	virtual void NativeOnInitialized() override;
};
