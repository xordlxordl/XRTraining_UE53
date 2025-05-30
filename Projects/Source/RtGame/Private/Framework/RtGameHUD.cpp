// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtGameHUD.h"

// Framework
#include "Framework/RtPlayerController.h"

// Manager
#include "Manager/RtGameManager.h"
#include "Manager/RtWebManager.h"
#include "Manager/RtTableManager.h"

// Widget
#include "UI/Rt_PageBase.h"
#include "UI/Rt_PrimaryLayout.h"
#include "UI/Common/Rt_MessageBox.h"

// Data
#include "Common/RtModeSettingData.h"

DEFINE_LOG_CATEGORY(LogRtHUD);

bool ARtGameHUD::Init_Primary_Layer()
{
	if (Primary_Layout.Get())
		return true;
	if (auto* mgr = Get_GameManager()) {
		if (auto* table = Get_TableManager()) {
			if (auto widget_class = table->Get_UIWidget("PrimaryLayer")) {
				if (URt_PrimaryLayout* widget = CreateWidget<URt_PrimaryLayout>(GetOwningPlayerController(), widget_class)) {
					widget->AddToViewport();
					Primary_Layout = widget;
					return true;
				}
			}
		}
	}
	return false;
}

void ARtGameHUD::Open_HUD_Widget()
{
	Init_Primary_Layer();

	URt_PageBase* res = nullptr;
	auto* pc = GetOwningPlayerController();
	if (auto* mgr = Get_GameManager()) {

		if (auto ModeDefinition = mgr->ModeDefinition.Get()) {
			auto play_type = mgr->Get_Play_Type();

			auto mode = ModeDefinition->Get_Mode(play_type);

			if (mode.UI_Name != NAME_None) {
				res = Open_Page(mode.UI_Name, false);
				if (res) {
					res->Post_OpenPage();
				}
				else {
					UE_LOG(LogRtHUD, Warning, TEXT("Widget is missing or URt_PageBase is not a parent class"));
				}
			}
			GetOwningPlayerController()->SetShowMouseCursor(mode.ShowMouseCursor);
		}
	}
}

URt_PageBase* ARtGameHUD::Open_Page(const FName& InName, bool InPopup)
{
	if (Primary_Layout.Get()) {
		return Primary_Layout.Get()->Open_Page(InName, InPopup, Get_PlayerController(), nullptr);
	}
	return nullptr;
}

bool ARtGameHUD::Close_Popup(URt_PageBase* InPage)
{
	if (Primary_Layout.Get()) {
		return Primary_Layout.Get()->Close_Popup(InPage);
	}
	return false;
}

void ARtGameHUD::Show_MessageBox(const FName& InNmae, FRtDele_MessagResult ResultCallback)
{
	if (auto* gi = GetGameInstance()) {

		if (auto* mgr = gi->GetSubsystem<URtTableManager>()) {
			if (auto* row = mgr->Get_MessageBox(InNmae)) {
				if (URt_MessageBox* page = Cast<URt_MessageBox>(Open_Page("ConfirmPopup", true))) {
					page->Setup_Dialog(row, ResultCallback);
				}
			}
		}
	}
}

URt_PageBase* ARtGameHUD::Open_Page_Loading()
{
	if (LoadingWidget) {

	}
	else {
		if (Primary_Layout.Get()) {
			LoadingWidget = Primary_Layout.Get()->Open_Page("Loading", true, Get_PlayerController(), nullptr);
			return LoadingWidget;
		}
	}
	return nullptr;
}

bool ARtGameHUD::Close_Popup_Loading()
{
	if (Primary_Layout.Get()) {
		Primary_Layout.Get()->Close_Popup(LoadingWidget);
		LoadingWidget = nullptr;
		return true;
	}
	return false;
}

void ARtGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if (auto* player = Cast<ULocalPlayer>(GetNetOwningPlayer()))
	{
	    this->Open_HUD_Widget();
	}

	// 원래는 Client 가 등록이 되면 Local Player 임을 체크하여 Delegate 를 호출해야함
	// 저 코드가 돌아가는지 확인해야함
	// 0322 확인 : 로컬에서 클라가 OPEN PAGE 를 사용하기 위함 
	// 데이터는 읽기만 하고 수정하거나 할당하지 않으므로 구현하지 않아도 됨
}

void ARtGameHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

ARtPlayerController* ARtGameHUD::Get_PlayerController()
{

	return Cast<ARtPlayerController>(GetOwningPlayerController());
}

URtGameManager* ARtGameHUD::Get_GameManager()
{
	if (auto* gi = GetGameInstance())
		return gi->GetSubsystem<URtGameManager>();

	return nullptr;
}

URtTableManager* ARtGameHUD::Get_TableManager()
{
	if (auto* gi = GetGameInstance())
		return gi->GetSubsystem<URtTableManager>();

	return nullptr;
}

