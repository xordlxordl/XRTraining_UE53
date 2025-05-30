// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Rt_PrimaryLayout.h"

//Framework
#include "Framework/RtPlayerController.h"
#include "Framework/RtGameMode.h"

// Manager
#include "Manager/RtGameManager.h"
#include "Manager/RtTableManager.h"

// Data
#include "Common/RtDataTableRows.h"

// Widget
#include "UI/Rt_PageBase.h"


DEFINE_LOG_CATEGORY(LogRtPrimaryLayout);


void URt_PrimaryLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void URt_PrimaryLayout::NativeDestruct()
{
	Super::NativeDestruct();
}

bool URt_PrimaryLayout::Initialize()
{
	bool res = Super::Initialize();
	return res;
}

void URt_PrimaryLayout::BeginDestroy()
{
	Super::BeginDestroy();
}

URt_PageBase* URt_PrimaryLayout::Open_Page(const FName& InName, bool InPopup, APlayerController* InPC, UUserWidget* InConnection)
{
	UE_LOG(LogRtPrimaryLayout, Log, TEXT("Open_PopupUI: %s"), *InName.ToString());
	
	if (ARtPlayerController* PC = Cast<ARtPlayerController>(InPC)) {
		if (auto* mgr = GetGameManager()) {

			if (auto* table = mgr->Get_TableManager()) {
				if (FRtRow_UI* table_row = table->Get_UI(InName)) {
					if (table_row && table_row->WidgetClass) {
						if (InPopup)
							return Inter_Open_Popup(InName, table_row->WidgetClass, InPC, InConnection);
						else
							return Inter_Open_Page(InName, table_row->WidgetClass, InPC, InConnection);
					}
				}
			}
		}
	}
	return 0;
	return nullptr;
}

bool URt_PrimaryLayout::Close_Popup(URt_PageBase* InWidget)
{
	bool res = false;

	if(Popup_Stack.Num() == 0) return false;

	auto* last = Popup_Stack.Last();
	if (last) {
		if (last == InWidget) {
			last->RemoveFromParent();
			Popup_Stack.Pop();
		}
		else {
			//check(0);
			return false;
		}
	}
	return res;
}

URt_PageBase* URt_PrimaryLayout::Inter_Open_Page(const FName& InName, TSubclassOf<UUserWidget> UserWidgetClass, APlayerController* InPC, UUserWidget* InConnection)
{
	if (UUserWidget* widget = CreateWidget<UUserWidget>(InPC, UserWidgetClass)) {
		if (auto RtWidget = Cast<URt_PageBase>(widget)) {
			if (InConnection) {
				RtWidget->Widget_Name = InName;
				RtWidget->Connection_Widget = InConnection;
			}
			Inter_Change_PageWidget(widget);
			return RtWidget;
		}
	}
	return nullptr;
}

URt_PageBase* URt_PrimaryLayout::Inter_Open_Popup(const FName& InName, TSubclassOf<UUserWidget> UserWidgetClass, APlayerController* InPC, UUserWidget* InConnection)
{
	if (UUserWidget* widget = CreateWidget<UUserWidget>(InPC, UserWidgetClass)) {
		if (auto RtWidget = Cast<URt_PageBase>(widget)) {
			if (InConnection) {
				RtWidget->Widget_Name = InName;
				RtWidget->Connection_Widget = InConnection;
			}
			Inter_Add_PopupWidget(RtWidget);
			return RtWidget;
		}
	}
	return nullptr;
}

void URt_PrimaryLayout::Inter_Change_PageWidget(UUserWidget* InWidget)
{
	if (Game_Layer && InWidget) {
		Game_Layer->ClearChildren();
		Game_Layer->AddChild(InWidget);
	}
}

void URt_PrimaryLayout::Inter_Add_PopupWidget(URt_PageBase* InWidget)
{
	if (InWidget && Popup_Layer) {
		Popup_Stack.Push(InWidget);
		Popup_Layer->AddChild(InWidget);
	}
}
