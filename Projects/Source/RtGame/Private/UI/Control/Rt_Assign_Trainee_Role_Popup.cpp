// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Assign_Trainee_Role_Popup.h"
// Framework
#include "Framework/RtPlayerController.h"
#include "Framework/RtGameState.h"
#include "Framework/RtPlayerState.h"

// Manager
#include "Manager/RtWebManager.h"

// UI
#include "UI/Control/Rt_Assign_Trainee_Role_List.h"
#include "UI/Control/Rt_Assign_Trainee.h"


void URt_Assign_Trainee_Role_Popup::Close_Page()
{
	Close_Popup(this);
}

void URt_Assign_Trainee_Role_Popup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (TapToCloseZone_var) {
		TapToCloseZone_var->OnMouseButtonDownEvent.BindUFunction(this, "OnClicked_TapToCloseZone");
	}
}

void URt_Assign_Trainee_Role_Popup::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(TapToCloseZone_var))
	{
		TapToCloseZone_var->OnMouseButtonDownEvent.Unbind();
	}
}

FEventReply URt_Assign_Trainee_Role_Popup::OnClicked_TapToCloseZone(FGeometry Geometry, const FPointerEvent& MouseEvent)
{
	Close_Page();
	return FEventReply();
}

void URt_Assign_Trainee_Role_Popup::Set_Widget_Assign_Trainee(URt_Assign_Trainee* InWidget)
{
	Widget_AssignTrainee = InWidget;

	if (Role_List_Var) {
		Role_List_Var->Widget_Trainee = Widget_AssignTrainee;
		Role_List_Var->Widget_RolePopup = this;
	}
}
