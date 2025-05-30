// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"

// UE Component of Widget
#include "Components/TextBlock.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Components/Button.h"
#include "Components/ListView.h"

// UI
#include "UI/Control/Rt_Place_Trainee.h"
#include "UI/Rt_ListViewObject.h"

#include "Rt_Place_Trainee_Block.generated.h"

class URt_Place_Trainee;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class RTGAME_API URt_Place_Trainee_Block : public URt_UserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	/** Follows the same pattern as the NativeOn[X] methods in UUserWidget - super calls are expected in order to route the event to BP. */
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override; // 아이템 설정시 호출

	virtual void NativeOnInitialized() override;

	/**
	 * These follow the same pattern as the NativeOn[X] methods in UUserWidget - super calls are expected in order to route the event to BP.
	 * See the BP events below for descriptions.
	 */
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override; // 아이템 선택시 호출
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UTextBlock*> TextBlock_List;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Text_Device_Var;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Text_Role_Var;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Text_Trainee_Var;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Text_ID_Var;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Text_Reset_Var;

	UPROPERTY(meta = (BindWidget))	UButton* Role_Btn;
	UPROPERTY(meta = (BindWidget))	UButton* Assign_Btn;
	UPROPERTY(meta = (BindWidget))  UButton* Reset_Btn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) URt_ListViewObejct_DeviceInfo* MyLVObject;
	URt_Place_Trainee* PlaceTraineeList;

	UFUNCTION(BlueprintCallable)
	void Set_PlaceTraineeList(URt_Place_Trainee* InWidget);
	UFUNCTION(BlueprintCallable)
	void Set_SelectedItem_InList();
	UFUNCTION(BlueprintCallable)
	void Set_InitAssignedBlock();
};
