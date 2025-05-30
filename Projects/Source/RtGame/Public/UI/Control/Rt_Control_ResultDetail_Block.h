// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"
// UE
#include "Components/TextBlock.h"
#include "Rt_Control_ResultDetail_Block.generated.h"

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Control_ResultDetail_Block : public URt_UserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))	UTextBlock* TrainingList_var;
	UPROPERTY(meta = (BindWidget))	UTextBlock* TrainingResult_var;

	// 블럭은 String 값만 받아 적용 해야함. sturct 에 따른 분기는 DetailBox 에서 수행해야 한다.
	UFUNCTION(BlueprintCallable) void InitTextBlocks(FString list);
	UFUNCTION(BlueprintCallable) void SetTextBlocks(FString result);
};
