// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Place_Trainee_List.h"

// ListViewObject <- Need for Data Synchronization between ListView and Block in List
#include "UI/Rt_ListViewObject.h"
// Framework
#include "Framework/RtPlayerState.h"
#include "Framework/RtGameState.h"
// Manager
#include "Manager/RtConfig.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"

void URt_Place_Trainee_List::NativeOnInitialized()
{
	Super::NativeOnInitialized();
    
	// Selected Item Handle
	if (ListView_Var) {
		ListView_Var->OnItemClicked().AddUObject(this, &ThisClass::Handle_ItemClicked);
	}

	UStringTable* ControlStringTable = LoadObject<UStringTable>(nullptr, *URtConfig::ControlMainPath);
	if (ControlStringTable == nullptr) return;
	if ( Device_text && Role_text && Trainee_text && ServerNum_text) {

		FString OUTPUT;
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("DeviceSet"), OUTPUT);
		Device_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Role"), OUTPUT);
		Role_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("TraineeName"), OUTPUT);
		Trainee_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("TraineeName"), OUTPUT);
		ServerNum_text->SetText(FText::FromString(OUTPUT));

	}


}

void URt_Place_Trainee_List::Handle_ItemClicked(UObject* InObject)
{
	if (auto obj = Cast<URt_ListViewObejct_DeviceInfo>(InObject)) {
		if (obj->Info.Active == true) {
			OnItemClicked_Delegate.Broadcast(InObject);
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("This Device is not logged in."));
			// Popup Widget 
		}
	}
}

void URt_Place_Trainee_List::Add_Item_ToListView(UObject* InObject)
{
	if (ListView_Var) {
		ListView_Var->AddItem(InObject);
	}
}

bool URt_Place_Trainee_List::Get_SelectedItem_InListView(FRtDeviceInfo& OutInfo)
{
	if (!ListView_Var)
		return false;

	TArray<UObject*> OutSelectedItems;
	int32 selected = ListView_Var->GetSelectedItems(OutSelectedItems);
	if (selected == 0)
		return false;

	if (auto* selected_item = Cast< URt_ListViewObejct_DeviceInfo>(OutSelectedItems[0])) {
		OutInfo = selected_item->Info;
		return true;
	}
	return false;
}

bool URt_Place_Trainee_List::Update_ListView(const TArray<FRtDeviceInfo>& InInfo)
{
	if (ListView_Var == nullptr)
		return false;

	//재선택 작업 변수.
	URt_ListViewObejct_DeviceInfo* old_selected_item = Cast<URt_ListViewObejct_DeviceInfo>(ListView_Var->GetSelectedItem());
	URt_ListViewObejct_DeviceInfo* new_selected_item = nullptr;

	if (auto* gs = Get_GameState())
	{
		int32 infoNum = InInfo.Num();
		if (infoNum >= 20) infoNum = 19;


		if (URtConfig::PlayerNum == 10) {
			infoNum = 10;
		}

		ListView_Var->ClearListItems();
		for (int32 i = 0; i < infoNum; i++) {
			auto& it = InInfo[i];
			auto* new_item = NewObject<URt_ListViewObejct_DeviceInfo>();

			if (ARtPlayerState* ps = gs->Find_DeviceUser(it.Name)) {
				if (OwnerWidget)
				{
					new_item->Fill_Data(it);
					auto& info = ps->Get_PlayerInfo();
					new_item->Change_Player_Name(info.PlayerInfo.TraineeName);
					new_item->Change_Player_SN(info.PlayerInfo.TraineeSN);
				    new_item->Widget_PlaceTrainee = OwnerWidget;
				}
			}
			else {
				new_item->Fill_Data(it);
			}
			if (old_selected_item && it.Name.IsEmpty() == false) {
				if (it.Name.Equals(old_selected_item->Info.Name)) {
					new_selected_item = new_item;
				}
			}
			ListView_Var->AddItem(new_item);
		}
		if (new_selected_item) {
			ListView_Var->SetSelectedItem(new_selected_item);
		}
	}


	return true;
}

int32 URt_Place_Trainee_List::Get_NumberOfActiveDevice()
{
	int32 res_num = 0;
	if (ListView_Var) {
		auto& array = ListView_Var->GetListItems();
		for (auto* it : array) {
			if (auto* device_info = Cast<URt_ListViewObejct_DeviceInfo>(it)) {
				if (device_info->Info.Active == true && !device_info->Info.bHasTrainee() == false) {
					res_num++;
				}
			}
		}
	}
	return res_num;
}

void URt_Place_Trainee_List::Set_SelectedItem_InListView(UObject* InObject)
{
	if (InObject)
	{
		ListView_Var->SetSelectedItem(InObject);
	}


}

