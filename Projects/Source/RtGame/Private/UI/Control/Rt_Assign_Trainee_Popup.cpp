// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Assign_Trainee_Popup.h"
// Framework
#include "Framework/RtPlayerController.h"
#include "Framework/RtGameState.h"
#include "Framework/RtPlayerState.h"

// Manager
#include "Manager/RtWebManager.h"
#include "Manager/RtGameManager.h"
#include "Manager/RtTrainingManager.h"

void URt_Assign_Trainee_Popup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (TapToCloseZone_var) {
		TapToCloseZone_var->OnMouseButtonDownEvent.BindUFunction(this, "OnClicked_TapToCloseZone");
	}
}

void URt_Assign_Trainee_Popup::Close_Page()
{
	Close_Popup(this);
}



FEventReply URt_Assign_Trainee_Popup::OnClicked_TapToCloseZone(FGeometry Geometry, const FPointerEvent& MouseEvent)
{
	Close_Page();
	return FEventReply();
}

void URt_Assign_Trainee_Popup::Update_TraineeInfo_Cashed()
{
	if (auto* pc = Get_PlayerController()) {

		if (auto* gi = GetGameInstance()) {
			if (auto* mgr = gi->GetSubsystem<URtGameManager>()) {
				if (auto* tmgr = mgr->TrngManager) {
					Assign_List_Var->Update_ListView(tmgr->Trainee_StatusArray);
				}
			}
		}
	}
}

void URt_Assign_Trainee_Popup::Update_TraineeInfo(APlayerController* InPlayerController, const TArray<FRtTraineeInfo>& InTraineeArray)
{
	if (auto* pc = Get_PlayerController()) {

		if (auto* gi = GetGameInstance()) {
			if (auto* mgr = gi->GetSubsystem<URtGameManager>()) {
				if (auto* tmgr = mgr->TrngManager) {
					Assign_List_Var->Update_ListView(tmgr->Trainee_StatusArray);
				}
			}
		}
	}

}

void URt_Assign_Trainee_Popup::Set_Widget_Assign_Trainee(URt_Assign_Trainee* InWidget)
{
	Widget_AssignTrainee = InWidget;

	if (Assign_List_Var) {
		Assign_List_Var->Widget_Trainee = Widget_AssignTrainee;
		Assign_List_Var->Widget_TraineePopup = this;
	}
}

void URt_Assign_Trainee_Popup::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(TapToCloseZone_var))
	{
		TapToCloseZone_var->OnMouseButtonDownEvent.Unbind();
	}

	Close_Page();
}