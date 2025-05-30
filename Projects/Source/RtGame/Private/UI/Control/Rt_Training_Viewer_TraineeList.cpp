// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Training_Viewer_TraineeList.h"

// Framework
#include "Framework/RtPlayerController.h"
// UI
#include "UI/Rt_ListViewObject.h"
// Manager
#include "Manager/RtConfig.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"

void URt_Training_Viewer_TraineeList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UStringTable* ControlStringTable = LoadObject<UStringTable>(nullptr, *URtConfig::ControlMainPath);
	if (ControlStringTable == nullptr) return;

	if (Text_Device && Text_Role && Text_Trainee && Text_ServerNumber && Text_Ready)
	{
		FString OUTPUT;
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("DeviceSet"), OUTPUT);
		Text_Device->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Role"), OUTPUT);
		Text_Role->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Trainee"), OUTPUT);
		Text_Trainee->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("TraineeID"), OUTPUT);
		Text_ServerNumber->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Ready"), OUTPUT);
		Text_Ready->SetText(FText::FromString(OUTPUT));
	}

}

void URt_Training_Viewer_TraineeList::NativeDestruct()
{
	Super::NativeDestruct();

}

bool URt_Training_Viewer_TraineeList::Update_ListView(const TArray<FRtDeviceInfo>& InInfo)
{
	if(ListView_var == nullptr)
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

bool URt_Training_Viewer_TraineeList::Update_ReadyState(const TArray<FRtDeviceInfo>& InInfo)
{
	if (ListView_var == nullptr)
		return false;

	ListView_var->ClearListItems();

	for (auto& it : InInfo) {
		if (it.StatusInTraining != EStatusInTraining::Default) {  // default 제외 모두 출력
			auto* new_item = NewObject<URt_ListViewObejct_DeviceInfo>();
			new_item->Fill_Data(it);
			ListView_var->AddItem(new_item);
		}
	}
	return true;
}
