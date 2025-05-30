// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Place_Trainee_Assign_Popup.h"

// Framework
#include "Framework/RtPlayerController.h"
#include "Framework/RtGameState.h"
#include "Framework/RtPlayerState.h"

// Manager
#include "Manager/RtWebManager.h"
#include "Manager/RtGameManager.h"
#include "Manager/RtTrainingManager.h"

// UI
#include "UI/Control/Rt_Place_Trainee_Assign_List.h"
#include "UI/Control/Rt_Place_Trainee.h"


void URt_Place_Trainee_Assign_Popup::Close_Page()
{
	Close_Popup(this);
}

void URt_Place_Trainee_Assign_Popup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (TapToCloseZone_var) {
		TapToCloseZone_var->OnMouseButtonDownEvent.BindUFunction(this, "OnClicked_TapToCloseZone");
	}
}

void URt_Place_Trainee_Assign_Popup::NativeDestruct()
{
	Super::NativeDestruct();
}

FEventReply URt_Place_Trainee_Assign_Popup::OnClicked_TapToCloseZone(FGeometry Geometry, const FPointerEvent& MouseEvent)
{
	Close_Page();
	return FEventReply();
}

void URt_Place_Trainee_Assign_Popup::Update_TraineeInfo_Cashed()
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

void URt_Place_Trainee_Assign_Popup::Update_TraineeInfo(APlayerController* InPlayerController, const TArray<FRtTraineeInfo>& InTraineeArray)
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

void URt_Place_Trainee_Assign_Popup::Set_Widget_Place_Trainee(URt_Place_Trainee* InWidget)
{
	Widget_PlaceTrainee = InWidget;

	if (Assign_List_Var) {
		Assign_List_Var->Widget_Trainee = Widget_PlaceTrainee;
		Assign_List_Var->Widget_TraineePopup = this;
	}
}
