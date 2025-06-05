// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Control_Result.h"
// Framework 
#include "Framework/RtGameHUD.h"
#include "Framework/RtPlayerController.h"
#include "Framework/RtGameState.h"

// Manager
#include "Manager/RtConfig.h"
#include "Manager/RtWebManager.h"
#include "Manager/RtLoadingManager.h"
#include "Network/RtNetworkManager.h"

// UI
#include "UI/Rt_ListViewObject.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"
#include <Manager/RtGameSceneManager.h>

void URt_Control_Result::NativeOnInitialized()
{
	Super::NativeOnInitialized();


	UStringTable* ControlStringTable = LoadObject<UStringTable>(nullptr, *URtConfig::ControlMainPath);
	if (ControlStringTable == nullptr) return;

	if (ResultTitle_text && ResultTitleToolTip_text && TrainingList_text && Logout_text 
		&& SelectScenario_text && StartNew_text && Restart_text)
	{
		FString OUTPUT;
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("ResultTraining"), OUTPUT);
		ResultTitle_text->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("ResultTraining_ToolTip"), OUTPUT);
		ResultTitleToolTip_text->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("TraineeList"), OUTPUT);
		TrainingList_text->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Logout"), OUTPUT);
		Logout_text->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("SelectScenario"), OUTPUT);
		SelectScenario_text->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("NewScenario"), OUTPUT);
		StartNew_text->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("ReStartScenario"), OUTPUT);
		Restart_text->SetText(FText::FromString(OUTPUT));
	}



	if (Select_Btn_var) {
		Select_Btn_var->OnClicked.AddDynamic(this, &URt_Control_Result::OnClicked_Select);
	}
	if (Logout_Btn_var) {
		Logout_Btn_var->OnClicked.AddDynamic(this, &URt_Control_Result::OnClicked_Logout);
	}
	if (NewScenario_Btn_var) {
		NewScenario_Btn_var->OnClicked.AddDynamic(this, &URt_Control_Result::OnClicked_NewScenario);
	}
	if (SameScenario_Btn_var) {
		SameScenario_Btn_var->OnClicked.AddDynamic(this, &URt_Control_Result::OnClicked_SameScenario);
	}
	if (TraineeList) {
		TraineeList->OnItemClicked_Delegate.AddUObject(this, &ThisClass::OnClicked_TraineeListItem);
		TraineeList->ParentsWidget = this;
	}

	if (IsLocalPlayer()) {
		if (auto pc = Get_PlayerController()) {

			pc->Update_DeviceList_Delegate.AddUObject(this, &ThisClass::Update_DeviceInfo);

			if (IsControl_PlayType()) {
				Update_DeviceInfo_Cached();
			}
		}
	}
}

void URt_Control_Result::Update_DeviceInfo_Cached()
{
	if (auto* pc = Get_PlayerController()) {
		auto& device_array = pc->Get_Accessible_PlayerDevices();
		if (TraineeList) {
			TraineeList->Update_ListView(device_array);
		}
	}
}

void URt_Control_Result::Update_DeviceInfo(APlayerController* InPlayerController, const TArray<FRtDeviceInfo>& InDeviceArray)
{
	if (auto* pc = Get_PlayerController()) {
		auto& device_array = pc->Get_Accessible_PlayerDevices();
		if (TraineeList) {
			TraineeList->Update_ListView(device_array);
		}
	}
}




// dave - start new scenario
void URt_Control_Result::OnClicked_Select()
{
	// 시나리오 선택 으로 이동
	if (auto* hud = Get_HUD()) {
		FRtDele_MessagResult ResultCallback = FRtDele_MessagResult::CreateLambda([this](ERt_MessageResult ConfirmationResult) {
			if (ConfirmationResult == ERt_MessageResult::Confirmed) {
				auto* netmgr = GetNetworkManager();
				if (netmgr) {
					netmgr->SendPacket_RestartAll();
				}
			}
			});
		hud->Show_MessageBox("ResetTraining", ResultCallback);
	}
}

void URt_Control_Result::OnClicked_Logout()
{
	if (auto* hud = Get_HUD()) {
		FRtDele_MessagResult ResultCallback = FRtDele_MessagResult::CreateLambda([this](ERt_MessageResult ConfirmationResult) {
			if (ConfirmationResult == ERt_MessageResult::Confirmed) {
				// Log Out Data Code 
				// 종료 패킷 + 게임종료 코드
			}
			});
		hud->Show_MessageBox("Logout", ResultCallback);
	}
}

// dave - start new scenario
void URt_Control_Result::OnClicked_NewScenario()
{
	if (auto* hud = Get_HUD()) {
		FRtDele_MessagResult ResultCallback = FRtDele_MessagResult::CreateLambda([this](ERt_MessageResult ConfirmationResult) {
			if (ConfirmationResult == ERt_MessageResult::Confirmed) {
				auto* netmgr = GetNetworkManager();
				if (netmgr) {
					netmgr->SendPacket_RestartAll();
				}
			}
			});
		hud->Show_MessageBox("ResetTraining", ResultCallback);
	}
}

void URt_Control_Result::OnClicked_SameScenario()
{
	if (auto* hud = Get_HUD()) {
		FRtDele_MessagResult ResultCallback = FRtDele_MessagResult::CreateLambda([this](ERt_MessageResult ConfirmationResult) {
			if (ConfirmationResult == ERt_MessageResult::Confirmed) {

				// Packet 
				auto* netmgr = GetNetworkManager();
				if (netmgr) {
					netmgr->SendPacket_RestartSame();
				}
				Open_Page("TrainingView");
			}
			});
		hud->Show_MessageBox("SameScenarioStart", ResultCallback);
	}
}

void URt_Control_Result::OnClicked_TraineeListItem(UObject* InObject)
{
	Clicked_ListObject = Cast<URt_ListViewObejct_DeviceInfo>(InObject);
	int32 uid = Clicked_ListObject->Info.TraineeUID;
	int32 tid;
	if (auto* pc = Get_PlayerController()) {
		if (auto* gs = pc->Get_GameState())
		{
			tid = gs->Training_Index;
		}
	}
	// Result Detail Get from Web
	auto* WebSubsys = GetRtWebManager();
	if (WebSubsys->IsInitialized())
	{
		WebSubsys->Handle_GetResult.AddUFunction(this, FName("Update_DetialBox"));
		WebSubsys->GetResult(tid, uid);
	}

	// recv 가 걸릴 때 까지 로딩 popup 을 열어줘야 함
	if (auto* gi = GetGameInstance()) {
		if (auto* lm = gi->GetSubsystem<URtLoadingManager>()) {
			//lm->LoadLoadingPopup(Get_PlayerController(), "GetResult");

			// dave - prevent flickerring
			lm->LoadLoadingPopupDelayed(Get_PlayerController(), "GetResult");
		}
	}
}

void URt_Control_Result::Update_DetialBox(const FResponse_TrngSimpleResult& inresult)
{
	// popup close
	if (auto* gi = GetGameInstance()) {
		if (auto* lm = gi->GetSubsystem<URtLoadingManager>()) {
			//lm->OnLevelLoaded_TypePopup();

			// dave - prevent flickerring
			lm->HideLoadingPopup();
		}
	}


	if (ResultDetail)
	{
		ResultDetail->Update_ResultList(inresult);
	}
}


