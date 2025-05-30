// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Place_Trainee_Assign_List.h"

// ListViewObject <- Need for Data Synchronization between ListView and Block in List
#include "UI/Rt_ListViewObject.h"
#include "UI/Control/Rt_Place_Trainee.h"
#include "UI/Control/Rt_Place_Trainee_List.h"
#include "UI/Control/Rt_Place_Trainee_Assign_Popup.h"
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



void URt_Place_Trainee_Assign_List::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UStringTable* ControlStringTable = LoadObject<UStringTable>(nullptr, *URtConfig::ControlMainPath);
	if (ControlStringTable == nullptr) return;

	if (TraineeWindow_text && Select_text && Quit_text)
	{
		FString OUTPUT;
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("TraineeSelect"), OUTPUT);
		TraineeWindow_text->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Select"), OUTPUT);
		Select_text->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Quit"), OUTPUT);
		Quit_text->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("TraineeSearch"), OUTPUT);
		SearchString_var->SetText(FText::FromString(OUTPUT));
	}

	if (ListView_Var) {
		ListView_Var->OnItemSelectionChanged().AddUObject(this, &URt_Place_Trainee_Assign_List::Handle_ItemSelectionChanged);
	}
	if (StartSearch_var) {
		StartSearch_var->OnClicked.AddDynamic(this, &URt_Place_Trainee_Assign_List::OnClicked_StartSearch);
	}
	if (SearchString_var) {
		SearchString_var->OnTextCommitted.AddDynamic(this, &URt_Place_Trainee_Assign_List::OnTextCommitted_SearchString);
	}
	if (Selected_Var) {
		Selected_Var->OnClicked.AddDynamic(this, &URt_Place_Trainee_Assign_List::OnClicked_Selected);
	}

	// �Ʒô�� ��� ����
	OnClicked_EmptySearch();
	//OnClicked_StartSearch();
}

void URt_Place_Trainee_Assign_List::NativeDestruct()
{
	Super::NativeDestruct();
	Close_Page();
}

void URt_Place_Trainee_Assign_List::Close_Page()
{
	if (Widget_TraineePopup.IsValid()) {
		Widget_TraineePopup.Get()->Close_Page();
	}
}

void URt_Place_Trainee_Assign_List::Handle_ItemSelectionChanged(UObject* InObject)
{
	// ���� �ϸ� ����
	// ������ ��Ƴ�����~
	Clicked_ListObject = Cast<URt_ListViewObject_TraineeInfo>(InObject);
}

void URt_Place_Trainee_Assign_List::OnClicked_StartSearch()
{
	if (bSearching)
		return;
	bSearching = true;
	StartSearch_var->SetIsEnabled(false);

	if (auto* mgr = GetGameManager()) {
		if (auto* trmgr = mgr->TrngManager) {

			if (ListView_Var && SearchString_var) {
				ListView_Var->ClearListItems();

				auto text = SearchString_var->GetText();
				auto search_word = text.ToString().TrimStartAndEnd();

				//'' empty �̸� ��� ���
				if (search_word.IsEmpty()) {
					for (auto& it : trmgr->User_Container) {
						URt_ListViewObject_TraineeInfo* obj = NewObject<URt_ListViewObject_TraineeInfo>();
						obj->Fill_Data(it.Value);
						ListView_Var->AddItem(obj);
					}
				}
				else {
					for (auto& it : trmgr->User_Container) {
						if (it.Value.TraineeName.Contains(search_word)) {
							URt_ListViewObject_TraineeInfo* obj = NewObject<URt_ListViewObject_TraineeInfo>();
							obj->Fill_Data(it.Value);
							ListView_Var->AddItem(obj);
						}
					}
				}
			}
		}
	}
	bSearching = false;
	StartSearch_var->SetIsEnabled(true);
}

void URt_Place_Trainee_Assign_List::OnTextCommitted_SearchString(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (bSearching)
		return;
	OnClicked_StartSearch();
}

void URt_Place_Trainee_Assign_List::OnClicked_Selected()
{
	if (auto obj = Cast<URt_ListViewObject_TraineeInfo>(Clicked_ListObject)) {
		if (Widget_Trainee.IsValid()) {
			if (auto* tmgr = GetGameManager()->TrngManager)
			{
				// Add RoleName in Array
				//tmgr->AssignedRoles.Add(obj->Info.TraineeRole);

				FRtDeviceInfo selectedInfo;
				Widget_Trainee->Trainee_List_var->Get_SelectedItem_InListView(selectedInfo);

				FString SelectedDeviceName = selectedInfo.Name;
				if (auto mapkey = tmgr->AssignedTrainee_Container.FindKey(SelectedDeviceName))
				{
					tmgr->AssignedTrainee_Container.Remove(*mapkey);
				}
				tmgr->AssignedTrainee_Container.Add(obj->Info.TraineeSN, selectedInfo.Name);
			}
			Widget_Trainee->Change_DeviceInfo(obj->Info);
			Close_Page();
		}
	}
}

void URt_Place_Trainee_Assign_List::OnClicked_EmptySearch()
{
	if (bSearching)
		return;
	bSearching = true;
	StartSearch_var->SetIsEnabled(false);

	if (auto* mgr = GetGameManager()) {
		if (auto* trmgr = mgr->TrngManager) {

			if (ListView_Var && SearchString_var) {
				ListView_Var->ClearListItems();

				FText text = FText::GetEmpty();
				auto search_word = text.ToString().TrimStartAndEnd();

				//'' empty �̸� ��� ���
				if (search_word.IsEmpty()) {
					for (auto& it : trmgr->User_Container) {
						URt_ListViewObject_TraineeInfo* obj = NewObject<URt_ListViewObject_TraineeInfo>();
						obj->Fill_Data(it.Value);
						ListView_Var->AddItem(obj);
					}
				}
				else {
					for (auto& it : trmgr->User_Container) {
						if (it.Value.TraineeName.Contains(search_word)) {
							URt_ListViewObject_TraineeInfo* obj = NewObject<URt_ListViewObject_TraineeInfo>();
							obj->Fill_Data(it.Value);
							ListView_Var->AddItem(obj);
						}
					}
				}
			}
		}
	}
	bSearching = false;
	StartSearch_var->SetIsEnabled(true);
}


void URt_Place_Trainee_Assign_List::Add_Item_ToListView(UObject* InObject)
{
	if (ListView_Var) {
		ListView_Var->AddItem(InObject);
	}
}

bool URt_Place_Trainee_Assign_List::Get_SelectedItem_InListView(FRtTraineeInfo& OutInfo)
{
	if (!ListView_Var)
		return false;

	TArray<UObject*> OutSelectedItems;
	int32 selected = ListView_Var->GetSelectedItems(OutSelectedItems);
	if (selected == 0)
		return false;

	if (auto* selected_item = Cast< URt_ListViewObject_TraineeInfo>(OutSelectedItems[0])) {
		OutInfo = selected_item->Info;
		return true;
	}
	return false;
}

bool URt_Place_Trainee_Assign_List::Update_ListView(const TArray<FRtTraineeInfo>& InInfo)
{
	if (ListView_Var == nullptr)
		return false;

	//�缱�� �۾� ����.
	URt_ListViewObject_TraineeInfo* old_selected_item = Cast<URt_ListViewObject_TraineeInfo>(ListView_Var->GetSelectedItem());
	URt_ListViewObject_TraineeInfo* new_selected_item = nullptr;

	if (auto* gs = Get_GameState())
	{
		ListView_Var->ClearListItems();
		for (auto& it : InInfo) {
			auto* new_item = NewObject<URt_ListViewObject_TraineeInfo>();

			// gs ���� DeviceInfo �� �����ͼ� Player Info �κ��� ���� ������ �Ǵ�����
			if (ARtPlayerState* ps = gs->Find_AssignedDevice(it.TraineeName)) {
				continue;
			}
			else {
				new_item->Fill_Data(it);
			}

			if (old_selected_item && it.TraineeName.IsEmpty() == false) {
				if (it.TraineeName.Equals(old_selected_item->Info.TraineeName)) {
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

int32 URt_Place_Trainee_Assign_List::Get_NumberOfActiveTrainee()
{
	return int32();
}
