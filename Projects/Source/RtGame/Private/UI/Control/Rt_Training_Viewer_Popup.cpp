// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Training_Viewer_Popup.h"
// UE
#include "Kismet/KismetTextLibrary.h"
// Framework
#include "Framework/RtPlayerController.h"
// Manager
#include "Manager/RtGameManager.h"
#include "Manager/RtTrainingManager.h"
// Data 
#include "Common/RtBlueprintAssetPath.h"
// UI
#include "UI/Control/Rt_Training_Accident_Block.h"
// Network
#include "Network/RtNetworkManager.h"

// Manager
#include "Manager/RtConfig.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"
// 내일 할일 : 실행, X 버튼 추가하기~ , ui 제작하기~ , 패킷 확인하기~



void URt_Training_Viewer_Popup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UStringTable* ControlStringTable = LoadObject<UStringTable>(nullptr, *URtConfig::ControlMainPath);
	if (ControlStringTable == nullptr) return;

	if (Run_text && Quit_text)
	{
		FString OUTPUT;
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Start"), OUTPUT);
		Run_text->SetText(FText::FromString(OUTPUT));
		
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Quit"), OUTPUT);
		Quit_text->SetText(FText::FromString(OUTPUT));
	}

	// Selected Item Handle
	if (ActiveBtn_var) {
		ActiveBtn_var->OnClicked.AddDynamic(this, &URt_Training_Viewer_Popup::OnClicked_Active);
	}
	if (QuitBtn_var) {
		QuitBtn_var->OnClicked.AddDynamic(this, &URt_Training_Viewer_Popup::OnClicked_Quit);
	}
	// 바깥 누르면 팝업창 꺼짐
	if (TapToCloseZone_var) {
		TapToCloseZone_var->OnMouseButtonDownEvent.BindUFunction(this, "OnClicked_TapToCloseZone");
	}

	Add_Accident_List();
}

FEventReply URt_Training_Viewer_Popup::OnClicked_TapToCloseZone(FGeometry Geometry, const FPointerEvent& MouseEvent)
{
	Close_Page();
	return FEventReply();
}

void URt_Training_Viewer_Popup::Add_Accident_List()
{
	TArray<FString> AccidentList;

	if (auto* mgr = GetGameManager()) {
		if (auto* tmgr = mgr->TrngManager) {
			AccidentList = tmgr->Get_SuddenEvtArray();
		}
	}

	for (FString list : AccidentList)
	{
		// Create Widget
		TSubclassOf<UUserWidget> widgetclaass;
		widgetclaass = LoadClass<UUserWidget>(nullptr, RtControlWidgetBlueprint::ControlAccidentBlock);
		URt_Training_Accident_Block* widget = CreateWidget<URt_Training_Accident_Block>(GetWorld(), widgetclaass);
		widget->parentWidget = this;
		widget->Accident_var->SetText(UKismetTextLibrary::Conv_StringToText(list));
		AccidentList_SB_var->AddChild(widget);
	}


}

void URt_Training_Viewer_Popup::OnClicked_Active()
{
	if (ARtPlayerController* pc = Get_PlayerController())
	{
		// Send Packet accident 
		auto* netmgr = GetNetworkManager();
		if (netmgr) {
			netmgr->SendPacket_SCTEventID(SelectedAccident_str);
		}
	}
}

void URt_Training_Viewer_Popup::OnClicked_Quit()
{
	Close_Page();
}

void URt_Training_Viewer_Popup::CheckSelected(URt_Training_Accident_Block* InWidget)
{
	if (AccidentList_SB_var) {
		for (auto* childwidget : AccidentList_SB_var->GetAllChildren()) {
			URt_Training_Accident_Block* selectedwidget = Cast<URt_Training_Accident_Block>(childwidget);
			if (selectedwidget == InWidget) {
				selectedwidget->bIsSelected = true;
				SelectedAccident_str = selectedwidget->Accident_var->GetText().ToString();
			}
			else {
				selectedwidget->bIsSelected = false;
			}
		}
	}
}

void URt_Training_Viewer_Popup::Close_Page()
{
	Close_Popup(this);
}
