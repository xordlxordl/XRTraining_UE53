// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Control_Result_List.h"
// Framework
#include "Framework/RtPlayerController.h"
// UI
#include "UI/Rt_ListViewObject.h"
// Manager
#include "Manager/RtConfig.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"


void URt_Control_Result_List::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UStringTable* ControlStringTable = LoadObject<UStringTable>(nullptr, *URtConfig::ControlMainPath);
	if (ControlStringTable == nullptr) return;

	if (Text_Device && Text_Role && Text_Trainee && Text_SN)
	{
		FString OUTPUT;
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("DeviceSet"), OUTPUT);
		Text_Device->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Role"), OUTPUT);
		Text_Role->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("TraineeName"), OUTPUT);
		Text_Trainee->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("TraineeID"), OUTPUT);
		Text_SN->SetText(FText::FromString(OUTPUT));


	}
	// Selected Item Handle
	if (ListView_var) {
		ListView_var->OnItemClicked().AddUObject(this, &URt_Control_Result_List::Handle_ItemClicked);
	}
}



void URt_Control_Result_List::NativeDestruct()
{
	Super::NativeDestruct();
}

bool URt_Control_Result_List::Update_ListView(const TArray<FRtDeviceInfo>& InInfo)
{
	if (ListView_var == nullptr)
		return false;

	ListView_var->ClearListItems();
	if (auto* pc = Get_PlayerController()) {
		auto& device_array = pc->Get_Accessible_PlayerDevices();

		for (auto& it : device_array) {
			if (it.StatusInTraining != EStatusInTraining::Default) {  // default 제외 모두 출력
				auto* new_item = NewObject<URt_ListViewObejct_DeviceInfo>();
				new_item->Fill_Data(it);
				ListView_var->AddItem(new_item);
			}
		}
	}
	return true;
}

void URt_Control_Result_List::Handle_ItemClicked(UObject* InObject)
{
	if (auto obj = Cast<URt_ListViewObejct_DeviceInfo>(InObject)) {
		if (obj->Info.Active == true) {
			OnItemClicked_Delegate.Broadcast(InObject);
		}
	}
}
