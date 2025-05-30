// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Assign_Trainee_List.h"

// ListViewObject <- Need for Data Synchronization between List and Block
#include "UI/Rt_ListViewObject.h"
// Framework
#include "Framework/RtPlayerState.h"
#include "Framework/RtGameState.h"
// Manager
#include "Manager/RtConfig.h"
#include "Components/UniformGridSlot.h"
#include "UI/Control/Rt_Assign_Trainee_Block.h"
// Data
#include "Common/RtBlueprintAssetPath.h"

void URt_Assign_Trainee_List::NativeOnInitialized()
{
	Super::NativeOnInitialized();




}

int32 URt_Assign_Trainee_List::Get_NumberOfActiveDevice()
{
	int32 res_num = 0;
	if (uniformGridPanel) {
		TArray<UWidget*> array = uniformGridPanel->GetAllChildren();
		for (auto* it : array) {
			URt_Assign_Trainee_Block* ChildWidget = Cast<URt_Assign_Trainee_Block>(it);
			bool active = ChildWidget->DeviceInfoObj->Info.Active;
			bool HasTrainee = ChildWidget->DeviceInfoObj->Info.bHasTrainee();
			if (active && HasTrainee) {
				res_num++;
			}
		}
	}

	//if (ListView_Var) {
	//	auto& array = ListView_Var->GetListItems();
	//	for (auto* it : array) {
	//		if (auto* device_info = Cast<URt_ListViewObejct_DeviceInfo>(it)) {
	//			if (device_info->Info.Active == true && !device_info->Info.bHasTrainee() == false) {
	//				res_num++;
	//			}
	//		}
	//	}
	//}
	return res_num;
}

bool URt_Assign_Trainee_List::Get_SelectedItem_InUniformGridPanel(FRtDeviceInfo& OutInfo)
{
	if (uniformGridPanel == nullptr)
		return false;

	if (CurrentSelectedBlock) {
		OutInfo = CurrentSelectedBlock->DeviceInfoObj->Info;
		return true;
	}
	return false;
}

bool URt_Assign_Trainee_List::Update_ListView(const TArray<FRtDeviceInfo>& InInfo)
{
	if (uniformGridPanel == nullptr)
		return false;

	if (auto* gs = Get_GameState())
	{
		int32 infoNum = InInfo.Num();
		if (infoNum > 20) infoNum = 20;


		TSubclassOf<URt_Assign_Trainee_Block> widgetclaass;
		widgetclaass = LoadClass<UUserWidget>(nullptr, RtControlWidgetBlueprint::ControlAssignedBlock);

		uniformGridPanel->ClearChildren();
		for (int32 i = 0; i < infoNum; i++) {
			auto& it = InInfo[i];
			auto* new_item = NewObject<URt_ListViewObejct_DeviceInfo>();

			if (ARtPlayerState* ps = gs->Find_DeviceUser(it.Name)) {

				// Set Item
				new_item->Fill_Data(it);
				auto& info = ps->Get_PlayerInfo();
				new_item->Change_Player_Name(info.PlayerInfo.TraineeName);
				new_item->Change_Player_SN(info.PlayerInfo.TraineeSN);
				new_item->Widget_AssingTrainee = OwnerWidget;
			}
			else {
				new_item->Fill_Data(it);
			}
			// Add Grid Slot
			UUserWidget* NewWidget = CreateWidget<UUserWidget>(this, widgetclaass);
			URt_Assign_Trainee_Block* NewTraineeBlock = Cast<URt_Assign_Trainee_Block>(NewWidget);
			if (NewTraineeBlock)
			{
				NewTraineeBlock->FillTextBlocks(new_item);
			}

			if (UUniformGridSlot* GridSlot = uniformGridPanel->AddChildToUniformGrid(NewTraineeBlock))
			{
				int32 Row;
				int32 Column;
				Row = i / 5;
				Column = i % 5;
				GridSlot->SetRow(Row);
				GridSlot->SetColumn(Column);
				GridSlot->SetHorizontalAlignment(HAlign_Fill);
				GridSlot->SetVerticalAlignment(VAlign_Fill);
			}

		}
	}


	return true;
}
