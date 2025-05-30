// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Assign_Trainee_Block.h"

// ListViewObject <- Need for Data Synchronization between List and Block
#include "UI/Rt_ListViewObject.h"
// Manager
#include "Manager/RtConfig.h"

// Framework
#include "Framework/RtPlayerState.h"
#include "Framework/RtGameState.h"
#include "Framework/RtPlayerController.h"

// UI 
#include "UI/Control/Rt_Assign_Trainee_Popup.h"
#include "UI/Control/Rt_Assign_Trainee_Role_Popup.h"
#include "UI/Control/Rt_Assign_Trainee_List.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"


void URt_Assign_Trainee_Block::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Static Text like 'Role' . 'Trainee' , 'SerialNumber' etc.. 
	UStringTable* ControlStringTable = LoadObject<UStringTable>(nullptr, *URtConfig::ControlMainPath);
	if (ControlStringTable == nullptr) return;
	if (Text_Device_Var && Text_Role_Var && Text_Trainee_Var && Text_SN_Var && Text_Reset_Var) {

		FString OUTPUT;
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("DeviceSet"), OUTPUT);
		Text_Device_Var->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Role"), OUTPUT);
		Text_Role_Var->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("TraineeName"), OUTPUT);
		Text_Trainee_Var->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("TraineeID"), OUTPUT);
		Text_SN_Var->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Reset"), OUTPUT);
		Text_Reset_Var->SetText(FText::FromString(OUTPUT));
	}

	// Button Binding
	Role_Btn->OnClicked.AddDynamic(this, &URt_Assign_Trainee_Block::OnClicked_Role_Btn);
	Trainee_Btn->OnClicked.AddDynamic(this, &URt_Assign_Trainee_Block::OnClicked_Trainee_Btn);

	// dave - reset
	Reset_Btn->OnClicked.AddDynamic(this, &URt_Assign_Trainee_Block::OnClicked_Reset);
}

void URt_Assign_Trainee_Block::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(Role_Btn))
	{
		Role_Btn->OnClicked.RemoveDynamic(this, &URt_Assign_Trainee_Block::OnClicked_Role_Btn);
	}
	if (IsValid(Trainee_Btn))
	{
		Trainee_Btn->OnClicked.RemoveDynamic(this, &URt_Assign_Trainee_Block::OnClicked_Trainee_Btn);
	}
	if (IsValid(Reset_Btn))
	{
		Reset_Btn->OnClicked.RemoveDynamic(this, &URt_Assign_Trainee_Block::OnClicked_Reset);
	}
}

// dave - Reset 
void URt_Assign_Trainee_Block::OnClicked_Reset()
{
	if (auto* pc = Get_PlayerController()) 
	{
		const TArray<FRtDeviceInfo>& DeviceArray = pc->Get_Accessible_PlayerDevices();
		TArray<FRtDeviceInfo> ModifiedArray = DeviceArray;
			
		if (auto* it = ModifiedArray.FindByPredicate([this](auto& element) { return element.Name == DeviceInfoObj->Info.Name; }))
		{
			int32 PlayerId = it->PlayerId;
			it->Logout();
			it->Active = true;
			it->PlayerId = PlayerId;

			if (auto* gs = Get_GameState())
			{
				if (ARtPlayerState* ps = gs->Find_DeviceUser(DeviceInfoObj->Info.Name))
				{
					ps->Set_PlayerInfo(FRtDeviceInfo());
				}
			}
			pc->Set_DeviceInfo(ModifiedArray);
		}
	}
}

void URt_Assign_Trainee_Block::OnClicked_Role_Btn()
{
	if (auto* pc = Get_PlayerController()) {
		if (DeviceInfoObj->Info.Active) {
			URt_PageBase* openedPopup =	pc->Open_Popup("RoleAssignPopup20");
			URt_Assign_Trainee_Role_Popup* RolePopup = Cast<URt_Assign_Trainee_Role_Popup>(openedPopup);
			RolePopup->Set_Widget_Assign_Trainee(DeviceInfoObj->Widget_AssingTrainee);
			if (RolePopup->Widget_AssignTrainee->Trainee_List_var->CurrentSelectedBlock) {
				// Selected Widget already
				RolePopup->Widget_AssignTrainee->Trainee_List_var->CurrentSelectedBlock->bIsSelected = false;
			}
			RolePopup->Widget_AssignTrainee->Trainee_List_var->CurrentSelectedBlock = this;
			bIsSelected = true;
			RolePopup->OpenerBlock = this;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Not Connected Device."));
		}
	}
}

void URt_Assign_Trainee_Block::OnClicked_Trainee_Btn()
{
	if (auto* pc = Get_PlayerController()) {
		if (DeviceInfoObj->Info.Active) {
			URt_PageBase* openedPopup = pc->Open_Popup("TraineeAssignPopup20");
			URt_Assign_Trainee_Popup* AssignPopup = Cast<URt_Assign_Trainee_Popup>(openedPopup);
			AssignPopup->Set_Widget_Assign_Trainee(DeviceInfoObj->Widget_AssingTrainee);
			if (AssignPopup->Widget_AssignTrainee->Trainee_List_var->CurrentSelectedBlock) {
				// Selected Widget already
				AssignPopup->Widget_AssignTrainee->Trainee_List_var->CurrentSelectedBlock->bIsSelected = false;
			}
			AssignPopup->Widget_AssignTrainee->Trainee_List_var->CurrentSelectedBlock = this;
			bIsSelected = true;
			AssignPopup->OpenerBlock = this;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Not Connected Device."));
		}
	}
}

void URt_Assign_Trainee_Block::FillTextBlocks(URt_ListViewObejct_DeviceInfo* IndeviceInfoObj)
{

	DeviceInfoObj = IndeviceInfoObj;
	FRtDeviceInfo INFO = DeviceInfoObj->Info;
	bool ActiveDevice = INFO.Active;
	bool HasTrainee = INFO.bHasTrainee();
	bool HasRole = INFO.bHasRole();
	Text_Device_Var->SetText(FText::FromString(INFO.Name));
	if (ActiveDevice) {
		Text_Device_Var->SetColorAndOpacity(ActiveColor);
	}
	else {
		Text_Device_Var->SetColorAndOpacity(UnActiveColor);
	}

	Text_Role_Value->SetText(FText::FromString(INFO.PlayerRole));
	if (HasRole) {
		Text_Role_Value->Font.OutlineSettings.OutlineSize = 1;
	}
	else {
		Text_Role_Value->Font.OutlineSettings.OutlineSize = 0;
	}

	Text_Trainee_Value->SetText(FText::FromString(INFO.PlayerInfo.TraineeName));
	if (HasTrainee) {
		Text_Trainee_Value->Font.OutlineSettings.OutlineSize = 1;
	}
	else {
		Text_Trainee_Value->Font.OutlineSettings.OutlineSize = 0;
	}

	Text_SN_Value->SetText(FText::FromString(INFO.PlayerInfo.TraineeSN));
}

