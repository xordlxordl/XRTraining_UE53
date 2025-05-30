// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"
// UE Component of Widget
#include "Components/ListView.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
// Data
#include "Common/RtRpcCommon.h"

#include "Rt_Assign_Trainee_Role_List.generated.h"

class URt_Assign_Trainee;
class URt_Assign_Trainee_Role_Popup;
class URt_ListViewObject_TraineeInfo;
class URt_ListViewObject_RoleInfo;

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Assign_Trainee_Role_List : public URt_UserWidget
{
	GENERATED_BODY()
	
public:
	// List View 
	UPROPERTY(meta = (BindWidget))	UListView* ListView_Var;
	UPROPERTY(meta = (BindWidget))	UButton* Selected_Var;
	UPROPERTY(meta = (BindWidget))	UButton* Exit_Var;
	
	TWeakObjectPtr<URt_Assign_Trainee> Widget_Trainee;
	TWeakObjectPtr<URt_Assign_Trainee_Role_Popup> Widget_RolePopup;
	TWeakObjectPtr<URt_ListViewObject_RoleInfo> Clicked_ListObject;
	UPROPERTY(meta = (BindWidget)) UTextBlock* RoleWindow_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Select_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Quit_text;

public:
	void Close_Page();

public:
	UFUNCTION()	void Handle_ItemSelectionChanged(UObject* InObject);
	UFUNCTION()	void Handle_DoubleClicked(UObject* InObject);
	UFUNCTION() void OnClicked_Selected();
	UFUNCTION() void OnClicked_Exit();

	void Add_Item_ToListView(UObject* InObject);
	bool Get_SelectedItem_InListView(FRtTraineeInfo& OutInfo);

	bool Update_ListView(const TArray<FRtTraineeInfo>& InInfo);



protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

private:
	bool IsAlreadyRoleAssiged(const FRtRoleInfo& RoleInfo); // dave
};
