// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Place_Trainee_Role_Popup.h"
// Framework
#include "Framework/RtPlayerController.h"
#include "Framework/RtGameState.h"
#include "Framework/RtPlayerState.h"

// Manager
#include "Manager/RtWebManager.h"

// UI
#include "UI/Control/Rt_Place_Trainee_Role_List.h"
#include "UI/Control/Rt_Place_Trainee.h"

void URt_Place_Trainee_Role_Popup::Close_Page()
{
	Close_Popup(this);
}

void URt_Place_Trainee_Role_Popup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (TapToCloseZone_var) {
		TapToCloseZone_var->OnMouseButtonDownEvent.BindUFunction(this, "OnClicked_TapToCloseZone");
	}
}

void URt_Place_Trainee_Role_Popup::NativeDestruct()
{
	Super::NativeDestruct();
}

FEventReply URt_Place_Trainee_Role_Popup::OnClicked_TapToCloseZone(FGeometry Geometry, const FPointerEvent& MouseEvent)
{
	Close_Page();
	return FEventReply();
}

void URt_Place_Trainee_Role_Popup::Set_Widget_Place_Trainee(URt_Place_Trainee* InWidget)
{
	Widget_PlaceTrainee = InWidget;

	if (Role_List_Var) {
		Role_List_Var->Widget_Trainee = Widget_PlaceTrainee;
		Role_List_Var->Widget_RolePopup = this;
	}
}
