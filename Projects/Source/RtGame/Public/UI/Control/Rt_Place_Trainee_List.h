// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"

// UE Component of Widget
#include "Components/ListView.h"
#include "Components/TextBlock.h"

// Data
#include "Common/RtRpcCommon.h"

#include "Rt_Place_Trainee_List.generated.h"

class URt_Place_Trainee;
/**
 * 
 */
UCLASS()
class RTGAME_API URt_Place_Trainee_List : public URt_UserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget)) UTextBlock* Device_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Role_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Trainee_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* ServerNum_text;


	// List View 
	UPROPERTY(meta = (BindWidget))	UListView* ListView_Var;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) URt_Place_Trainee* OwnerWidget;

	// Delegate , when clicked(selected) Blocks in ListView
	DECLARE_MULTICAST_DELEGATE_OneParam(FItemClicked, UObject*);
	FItemClicked OnItemClicked_Delegate;

public:
	UFUNCTION()	void Handle_ItemClicked(UObject* InObject);

	void Add_Item_ToListView(UObject* InObject);
	bool Get_SelectedItem_InListView(FRtDeviceInfo& OutInfo);

	bool Update_ListView(const TArray<FRtDeviceInfo>& InInfo);

	int32 Get_NumberOfActiveDevice();

	void Set_SelectedItem_InListView(UObject* InObject);

protected:
	virtual void NativeOnInitialized() override;
};
