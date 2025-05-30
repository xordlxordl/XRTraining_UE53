// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"
// UE
#include "Components/ListView.h"
#include "Components/TextBlock.h"
// Data
#include "Common/RtRpcCommon.h"

#include "Rt_Training_Viewer_TraineeList.generated.h"

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Training_Viewer_TraineeList : public URt_UserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UListView* ListView_var;

	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_Device;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_Role;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_Trainee;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_ServerNumber;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_Ready;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:

	bool Update_ListView(const TArray<FRtDeviceInfo>& InInfo);
	bool Update_ReadyState(const TArray<FRtDeviceInfo>& InInfo);
};
