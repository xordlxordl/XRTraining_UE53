// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"

#include "Components/WidgetSwitcher.h"

#include "Rt_Trainee_Ammunition.generated.h"

UCLASS()
class RTGAME_API URt_Trainee_Ammunition : public URt_UserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWidgetSwitcher* Switcher_Ammo;
	
public:
	void SetAmmunitionType(bool isLive);
	
};
