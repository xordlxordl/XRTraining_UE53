// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"
// UI
#include "Components/TextBlock.h"
#include "Components/Button.h"
// Data
#include "Common/RtWebCommon.h"
#include "Rt_Result_DetailBox.generated.h"

class URt_Control_ResultDetail_Block;

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Result_DetailBox : public URt_UserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;


public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) URt_Control_ResultDetail_Block* Role_Block;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) URt_Control_ResultDetail_Block* Time_Block;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) URt_Control_ResultDetail_Block* Distance_Block;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) URt_Control_ResultDetail_Block* FireCount_Block;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) URt_Control_ResultDetail_Block* HitCount_Block;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) URt_Control_ResultDetail_Block* ChallengeCount_Block;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) URt_Control_ResultDetail_Block* SurrenderCount_Block;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) URt_Control_ResultDetail_Block* BindCount_Block;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) URt_Control_ResultDetail_Block* Command_Block;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) URt_Control_ResultDetail_Block* Breif_Block;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) URt_Control_ResultDetail_Block* Call_Block;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) URt_Control_ResultDetail_Block* EOD_Block;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite) URt_Control_ResultDetail_Block* BadFireCount_Block;

	UPROPERTY(meta = (BindWidget)) UButton* PathBtn_var;

	UPROPERTY(meta = (BindWidget)) UTextBlock* TraineeName;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TrainingListVar;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Result_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* ViewPath_text;

	void Update_ResultList(FResponse_TrngSimpleResult ResultSturct);
	void Init_ResultList();







	
};
