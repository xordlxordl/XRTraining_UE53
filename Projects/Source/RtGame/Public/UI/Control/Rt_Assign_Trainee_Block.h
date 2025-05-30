// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"

// widget Comp
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Math/Color.h"

#include "Rt_Assign_Trainee_Block.generated.h"

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Assign_Trainee_Block : public URt_UserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	// Basic Text
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Text_Device_Var;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Text_Role_Var;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Text_Trainee_Var;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Text_SN_Var;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Text_Reset_Var;

	// Changable Text
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Text_Role_Value;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Text_Trainee_Value;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Text_SN_Value;

	// Execute Button
	UPROPERTY(meta = (BindWidget))	UButton* Role_Btn;
	UPROPERTY(meta = (BindWidget))	UButton* Trainee_Btn;
	UPROPERTY(meta = (BindWidget))  UButton* Reset_Btn;

	UPROPERTY(BlueprintReadOnly) class URt_ListViewObejct_DeviceInfo* DeviceInfoObj;

	UPROPERTY(BlueprintReadOnly) bool bIsSelected = false;


	// Color
	FSlateColor UnActiveColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f); // Red
	FSlateColor ActiveColor = FLinearColor(50.0f / 255.0f, 205.0f / 255.0f, 50.0f / 255.0f, 1.0f); // LimeGreen

	//UFUNCTION(BlueprintCallable)
	//void Set_PlaceTraineeList(URt_Place_Trainee* InWidget);
	UFUNCTION(BlueprintCallable)
	void OnClicked_Role_Btn();
	UFUNCTION(BlueprintCallable)
	void OnClicked_Trainee_Btn();
	UFUNCTION(BlueprintCallable)
	void OnClicked_Reset();

	void FillTextBlocks(class URt_ListViewObejct_DeviceInfo* IndeviceInfoObj);

};
