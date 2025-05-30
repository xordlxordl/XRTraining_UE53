// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Control_SelectMap.h"

// Framework
#include "Framework/RtPlayerController.h"
#include "Framework/RtGameState.h"
#include "Framework/RtGameHUD.h"	
// Manager
#include "Manager/RtConfig.h"
#include "Manager/RtWebManager.h"
#include "Manager/RtGameManager.h"
#include "Manager/RtTrainingManager.h"
// UI
#include "Kismet/GameplayStatics.h"
#include "UI/Control/Rt_Control_SelectMap_List.h"
#include "UI/Control/Rt_Control_SelectMap_Info.h"
// Network
#include "Network/RtNetworkManager.h"

// Local
#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"

void URt_Control_SelectMap::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Init bool when primary widget Backs this widget
	bNextStep = false;
	if (auto* tmgr = GetTrainingManager()) {
		tmgr->SetControlFlow(EControlFlow::CONTROL_SETSCT);
	}

	if (SelectContent_Btn) {
		SelectContent_Btn->OnClicked.AddDynamic(this, &URt_Control_SelectMap::OnClicked_SelectContent);
	}
	if (LogOut_Btn) {
		LogOut_Btn->OnClicked.AddDynamic(this, &URt_Control_SelectMap::OnClicked_LogOut);
	}

	if (auto* gi = GetGameInstance()) {
		if (auto* mgr = gi->GetSubsystem<URtGameManager>()) {
			if (auto* tmgr = mgr->TrngManager) {
				if (WBP_Map_List)
				{
					WBP_Map_List->Set_ScenarioList(tmgr->ScenarioName_Array);
					WBP_Map_List->Set_OwnerWidget(this);
					if (WBP_ContentsInfo)
					{
						WBP_Map_List->Set_ContentsInfo(WBP_ContentsInfo);
					}
				}
			}
		}
	}

	// 제거예정
	//if (auto* pc = Get_PlayerController())
	//{
	//
	//
	//	if (auto* webmgr = GetRtWebManager())
	//	{
	//		TArray<FString> SctFileNames;
	//		SctFileNames = webmgr->sctfileNames;
	//
	//		if (WBP_Map_List)
	//		{
	//			WBP_Map_List->Set_ScenarioList(SctFileNames);
	//			WBP_Map_List->Set_OwnerWidget(this);
	//			if (WBP_ContentsInfo)
	//			{
	//				WBP_Map_List->Set_ContentsInfo(WBP_ContentsInfo);
	//			}
	//		}
	//	}
	//}

	UStringTable* ControlStringTable = LoadObject<UStringTable>(nullptr, *URtConfig::ControlMainPath);
	if (ControlStringTable == nullptr) return;
	if (ScenarioTitle_text && ScenarioTitleToolTip_text && ScenarioList_text && Logout_text && Complete_text) {

		FString OUTPUT;
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("SelectScenario"), OUTPUT);
		ScenarioTitle_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("SelectScenario_ToolTip"), OUTPUT);
		ScenarioTitleToolTip_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("ScenarioList"), OUTPUT);
		ScenarioList_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Logout"), OUTPUT);
		Logout_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("SelectConfirm"), OUTPUT);
		Complete_text->SetText(FText::FromString(OUTPUT));
	}


	

}

void URt_Control_SelectMap::SetCurrScenarioInfo(const FRtScenarioInfo& InInfo)
{
	// ScenarioInfo Update
	if (ARtPlayerController* pc = Get_PlayerController())
	{
		pc->Set_ScenarioInfo(InInfo);
		pc->Get_GameState()->Set_Scenario_Info(InInfo);
	}
}

void URt_Control_SelectMap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(bNextStep == false)
	{
		if (auto* tmgr = GetTrainingManager()) {
			if (tmgr->state_ctrl == EControlFlow::CONTROL_INSTALLWALL) {
				OpenNextPage();
				bNextStep = true;
			}
		}
	}
}

void URt_Control_SelectMap::OnClicked_SelectContent()
{
	if (HavingSelectedBlock)
	{
		auto* netmgr = GetNetworkManager();
		if (netmgr) {
			netmgr->SendPacket_SetScenario(WBP_ContentsInfo->SelectedFileName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Please Select Contents !!"));

		if (auto* hud = Get_HUD()) {
			FRtDele_MessagResult ResultCallback = FRtDele_MessagResult::CreateLambda([this](ERt_MessageResult ConfirmationResult) {
				if (ConfirmationResult == ERt_MessageResult::Confirmed) {
			
				}
				});
			hud->Show_MessageBox("ScenarioSelect", ResultCallback);
		}
	}

}

void URt_Control_SelectMap::OnClicked_LogOut()
{
	if (auto* hud = Get_HUD()) {
		FRtDele_MessagResult ResultCallback = FRtDele_MessagResult::CreateLambda([this](ERt_MessageResult ConfirmationResult) {
			if (ConfirmationResult == ERt_MessageResult::Confirmed) {
				// Log Out Data Code 
				Open_Page("ControlLogin");
			}
			});
		hud->Show_MessageBox("Logout", ResultCallback);
	}
}

void URt_Control_SelectMap::OpenNextPage()
{
	// ScenarioInfo Update _ Server
	if (ARtPlayerController* pc = Get_PlayerController())
	{
		pc->Server_Set_Scenario_Info(pc->Current_Scenario);
	}

	
	Open_Page("ControlInstallWall");
}