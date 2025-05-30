// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"

#include "Components/TextBlock.h"

#include "Rt_Training_View_Blockt.generated.h"

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Training_View_Blockt : public URt_UserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Text_Device;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Text_Role;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Text_Trainee;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Text_ServerNumber;

	virtual void NativeOnInitialized() override;

};
