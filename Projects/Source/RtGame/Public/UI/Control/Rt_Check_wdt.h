// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_PageBase.h"

// UE
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Rt_Check_wdt.generated.h"

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Check_wdt : public URt_PageBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget)) UTextBlock* WDTTitle_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* WDTTitleToolTip_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* DeplotMain_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Backstep_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Complete_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* SelectScenario_text;

	UPROPERTY(meta = (BindWidget))  UButton* BackStepBtn_var;    // 이전 단계
	UPROPERTY(meta = (BindWidget))  UButton* SelectBtn_var;      // 시나리오 선택
	UPROPERTY(meta = (BindWidget))  UButton* ConfirmBtn_var;     // WDT 확인 : 가벽 배치 완료

    UFUNCTION() void OnClicked_BackStepBtn();
	UFUNCTION() void OnClicked_SelectBtn();
	UFUNCTION() void OnClicked_ConfrimBtn();

protected:
	virtual void NativeOnInitialized() override;


};
