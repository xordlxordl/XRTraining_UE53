// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Place_Trainee_Block.h"
#include "UI/Rt_ListViewObject.h"

// Framework
#include "Framework/RtPlayerController.h"
// Manager
#include "Manager/RtTrainingManager.h"
#include "Manager/RtGameManager.h"
// Data
#include "Common/RtRpcCommon.h"
// UI
#include "UI/Control/Rt_Place_Trainee_List.h"
#include "UI/Control/Rt_Place_Trainee.h"

// Manager
#include "Manager/RtConfig.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"

void URt_Place_Trainee_Block::NativeConstruct()
{
	UUserWidget::NativeConstruct();
}

void URt_Place_Trainee_Block::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}

void URt_Place_Trainee_Block::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UStringTable* ControlStringTable = LoadObject<UStringTable>(nullptr, *URtConfig::ControlMainPath);
	if (ControlStringTable == nullptr) return;
	if (Text_Device_Var && Text_Role_Var && Text_Trainee_Var && Text_ID_Var && Text_Reset_Var) {

		FString OUTPUT;
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("DeviceSet"), OUTPUT);
		Text_Device_Var->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Role"), OUTPUT);
		Text_Role_Var->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("TraineeName"), OUTPUT);
		Text_Trainee_Var->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("TraineeID"), OUTPUT);
		Text_ID_Var->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Reset"), OUTPUT);
		Text_Reset_Var->SetText(FText::FromString(OUTPUT));

	}

	Role_Btn->OnClicked.AddDynamic(this, &URt_Place_Trainee_Block::Set_SelectedItem_InList);
	Assign_Btn->OnClicked.AddDynamic(this, &URt_Place_Trainee_Block::Set_SelectedItem_InList);
	Reset_Btn->OnClicked.AddDynamic(this,&URt_Place_Trainee_Block::Set_InitAssignedBlock);
}

void URt_Place_Trainee_Block::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
}

void URt_Place_Trainee_Block::Set_PlaceTraineeList(URt_Place_Trainee* InWidget)
{
	if (InWidget)
	{
    	PlaceTraineeList = InWidget;
	}
}

void URt_Place_Trainee_Block::Set_SelectedItem_InList()
{
	if (MyLVObject && PlaceTraineeList)
	{
    	PlaceTraineeList->Trainee_List_var->Set_SelectedItem_InListView(MyLVObject);

	}

}

void URt_Place_Trainee_Block::Set_InitAssignedBlock()
{
	if (auto* pc = Get_PlayerController()) {
		// 블럭 초기화 
		// Get LV Object 
		FRtDeviceInfo objectInfo = MyLVObject->Info;
		// Device Info Init
		FRtTraineeInfo resetInfo;

		FRtRoleInfo resetRoleInfo;
		resetRoleInfo.TraineeRole = TEXT("보직");
		resetRoleInfo.RoleID = TEXT("Role_ID");

		pc->Server_AssignTrainee_ToDevice(objectInfo.Name, resetInfo);
		pc->Server_AssignRole_ToDevice(objectInfo.Name, resetRoleInfo);

		// REMOVE value in Array
	}
}
