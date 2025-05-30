// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Components/TextBlock.h"
#include "Rt_Control_Result_Block.generated.h"



/**
 * 
 */
UCLASS()
class RTGAME_API URt_Control_Result_Block : public URt_UserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Text_Device;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Text_Role;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Text_Trainee;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Text_SN;


	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	/** Follows the same pattern as the NativeOn[X] methods in UUserWidget - super calls are expected in order to route the event to BP. */
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override; // 아이템 설정시 호출

	/**
	 * These follow the same pattern as the NativeOn[X] methods in UUserWidget - super calls are expected in order to route the event to BP.
	 * See the BP events below for descriptions.
	 */
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override; // 아이템 선택시 호출


	
};
