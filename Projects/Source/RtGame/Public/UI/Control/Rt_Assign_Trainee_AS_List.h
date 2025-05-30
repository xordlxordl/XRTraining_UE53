// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"
// UE Component of Widget
#include "Components/ListView.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
// Data
#include "Common/RtRpcCommon.h"

#include "Rt_Assign_Trainee_AS_List.generated.h"


class URt_Assign_Trainee;
class URt_Assign_Trainee_Popup;
class URt_ListViewObject_TraineeInfo;

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Assign_Trainee_AS_List : public URt_UserWidget
{
	GENERATED_BODY()

public:
	// List View 
	UPROPERTY(meta = (BindWidget))	UListView* ListView_Var;
	UPROPERTY(meta = (BindWidget))	UButton* StartSearch_var;
	UPROPERTY(meta = (BindWidget))	UEditableText* SearchString_var;
	UPROPERTY(meta = (BindWidget))	UButton* Selected_Var;
	UPROPERTY(meta = (BindWidget))	UButton* Exit_Var;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TraineeWindow_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Select_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Quit_text;
	TWeakObjectPtr<URt_Assign_Trainee> Widget_Trainee;
	TWeakObjectPtr<URt_Assign_Trainee_Popup> Widget_TraineePopup;
	TWeakObjectPtr<URt_ListViewObject_TraineeInfo> Clicked_ListObject;




private:
	bool bSearching = false;
public:
	void Close_Page();

public:
	UFUNCTION()	void Handle_ItemSelectionChanged(UObject* InObject);
	UFUNCTION() void Handle_ItemSelectionDoubleClicked(UObject* InObject);
	UFUNCTION()	void OnClicked_StartSearch();
	UFUNCTION()	void OnTextCommitted_SearchString(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION() void OnClicked_Selected();
	UFUNCTION() void OnClicked_Exit();
	UFUNCTION() void OnClicked_EmptySearch(); // for init trainee list

	void Add_Item_ToListView(UObject* InObject);
	bool Get_SelectedItem_InListView(FRtTraineeInfo& OutInfo);

	bool Update_ListView(const TArray<FRtTraineeInfo>& InInfo);

	int32 Get_NumberOfActiveTrainee();


protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

private:
	bool IsAlreadyTraineeAssiged(const FRtTraineeInfo& TraineeInfo);

};
