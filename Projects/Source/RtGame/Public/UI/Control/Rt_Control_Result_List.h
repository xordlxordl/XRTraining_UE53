// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"
// UE
#include "Components/ListView.h"
#include "Components/TextBlock.h"
// Data
#include "Common/RtRpcCommon.h"

#include "Rt_Control_Result_List.generated.h"

class URt_Control_Result;
/**
 * 
 */
UCLASS()
class RTGAME_API URt_Control_Result_List : public URt_UserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget)) UListView* ListView_var;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_Device;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_Role;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_Trainee;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Text_SN;


protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	bool Update_ListView(const TArray<FRtDeviceInfo>& InInfo);
	URt_Control_Result* ParentsWidget;

	// Delegate , when clicked(selected) Blocks in ListView
	DECLARE_MULTICAST_DELEGATE_OneParam(FItemClicked, UObject*);
	FItemClicked OnItemClicked_Delegate;

	UFUNCTION()	void Handle_ItemClicked(UObject* InObject);

};
