// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Place_Trainee_Role_List.h"

// ListViewObject <- Need for Data Synchronization between ListView and Block in List
#include "UI/Rt_ListViewObject.h"
#include "UI/Control/Rt_Place_Trainee.h"
#include "UI/Control/Rt_Place_Trainee_List.h"
#include "UI/Control/Rt_Place_Trainee_Role_Popup.h"
// Framework
#include "Framework/RtPlayerState.h"
#include "Framework/RtGameState.h"
// Manager
#include "Manager/RtTrainingManager.h"
#include "Manager/RtGameManager.h"
// Manager
#include "Manager/RtConfig.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"

void URt_Place_Trainee_Role_List::Close_Page()
{
	if (Widget_RolePopup.IsValid()) {
		Widget_RolePopup.Get()->Close_Page();
	}
}

void URt_Place_Trainee_Role_List::Handle_ItemSelectionChanged(UObject* InObject)
{
	Clicked_ListObject = Cast<URt_ListViewObject_RoleInfo>(InObject);
}

void URt_Place_Trainee_Role_List::OnClicked_Selected()
{
	if (auto obj = Cast<URt_ListViewObject_RoleInfo>(Clicked_ListObject)) {
		if (Widget_Trainee.IsValid()) {
			if (auto* tmgr = GetGameManager()->TrngManager)
			{
				// Add RoleName in Array
				//tmgr->AssignedRoles.Add(obj->Info.TraineeRole);

				FRtDeviceInfo selectedInfo;
				Widget_Trainee->Trainee_List_var->Get_SelectedItem_InListView(selectedInfo);
				
				FString SelectedDeviceName = selectedInfo.Name;
				if (auto mapkey = tmgr->AssignedRole_Container.FindKey(SelectedDeviceName))
				{
					tmgr->AssignedRole_Container.Remove(*mapkey);
				}
				tmgr->AssignedRole_Container.Add(obj->Info.TraineeRole, selectedInfo.Name);
			}
			Widget_Trainee->Change_DeviceInfo(obj->Info);
			Close_Page();
		}
	}
}

void URt_Place_Trainee_Role_List::Add_Item_ToListView(UObject* InObject)
{

}

bool URt_Place_Trainee_Role_List::Get_SelectedItem_InListView(FRtTraineeInfo& OutInfo)
{
	return false;
}

bool URt_Place_Trainee_Role_List::Update_ListView(const TArray<FRtTraineeInfo>& InInfo)
{
	return false;
}

void URt_Place_Trainee_Role_List::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UStringTable* ControlStringTable = LoadObject<UStringTable>(nullptr, *URtConfig::ControlMainPath);
	if (ControlStringTable == nullptr) return;

	if (RoleWindow_text && Select_text && Quit_text)
	{
		FString OUTPUT;
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("RoleSelect"), OUTPUT);
		RoleWindow_text->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Select"), OUTPUT);
		Select_text->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Quit"), OUTPUT);
		Quit_text->SetText(FText::FromString(OUTPUT));
	}

	if (ListView_Var) {
		ListView_Var->OnItemSelectionChanged().AddUObject(this, &URt_Place_Trainee_Role_List::Handle_ItemSelectionChanged);
	}
	if (Selected_Var) {
		Selected_Var->OnClicked.AddDynamic(this, &URt_Place_Trainee_Role_List::OnClicked_Selected);
	}


	if (auto* mgr = GetGameManager()) {
		if (auto* trmgr = mgr->TrngManager) {

			if (ListView_Var) {
				ListView_Var->ClearListItems();

				for (auto& it : trmgr->Role_Container) {
					URt_ListViewObject_RoleInfo* obj = NewObject<URt_ListViewObject_RoleInfo>();
					obj->Fill_Data(it.Value);

					ListView_Var->AddItem(obj);
				}
			}
		}
	}

}

void URt_Place_Trainee_Role_List::NativeDestruct()
{

}
