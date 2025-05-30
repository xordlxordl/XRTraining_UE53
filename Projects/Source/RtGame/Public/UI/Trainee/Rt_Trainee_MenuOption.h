// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Trainee/Rt_Trainee_Text.h"

#include "Components/Image.h"

#include "Rt_Trainee_MenuOption.generated.h"

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Trainee_MenuOption : public URt_Trainee_Text
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Img;

protected:
	UPROPERTY(BlueprintReadWrite, BlueprintReadWrite)
	UMaterialInterface* CircleChargeMaterial = nullptr;

	UPROPERTY(BlueprintReadWrite, BlueprintReadWrite)
	FSlateBrush CircleChargeBrush;

public:
	void SetPercent(float NewPercent);
};
