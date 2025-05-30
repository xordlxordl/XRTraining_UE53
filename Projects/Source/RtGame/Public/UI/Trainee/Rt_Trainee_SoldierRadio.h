// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"

#include "Components/Image.h"

#include "Framework/RtWidgetActor_Radio.h"

#include "Rt_Trainee_SoldierRadio.generated.h"

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Trainee_SoldierRadio : public URt_UserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Image_Radio;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Image_Progress;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Image_Fail;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* CircleCharge;

	UPROPERTY(BlueprintReadWrite, BlueprintReadWrite)
	UMaterialInterface* CircleChargeMaterial = nullptr;
	
	UPROPERTY(BlueprintReadWrite, BlueprintReadWrite)
	FSlateBrush CircleChargeBrush;

public:
	void Update(const ESoldierRadioState& NewState, float NewPercent = 0.0f);
	void SetPercent(float NewPercent);
};
