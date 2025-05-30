// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Check_wdt.h"
// Framework
#include "Framework/RtGameHUD.h"	
// UI
#include "Kismet/GameplayStatics.h"
// Manager
#include "Manager/RtConfig.h"
#include "Manager/RtTrainingManager.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"

void URt_Check_wdt::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// GetBackThis Widget
	if (auto* tmgr = GetTrainingManager()) {
		tmgr->SetControlFlow(EControlFlow::CONTROL_INSTALLWALL);
	}


	if(BackStepBtn_var)
		BackStepBtn_var->OnClicked.AddUniqueDynamic(this, &URt_Check_wdt::OnClicked_BackStepBtn);

	if (SelectBtn_var)
		SelectBtn_var->OnClicked.AddUniqueDynamic(this, &URt_Check_wdt::OnClicked_SelectBtn);

	if (ConfirmBtn_var)
		ConfirmBtn_var->OnClicked.AddUniqueDynamic(this, &URt_Check_wdt::OnClicked_ConfrimBtn);

	UStringTable* ControlStringTable = LoadObject<UStringTable>(nullptr, *URtConfig::ControlMainPath);
	if (ControlStringTable == nullptr) return;
	if (WDTTitle_text && WDTTitleToolTip_text && DeplotMain_text && Backstep_text && Complete_text && SelectScenario_text) {

		FString OUTPUT;
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("WDT"), OUTPUT);
		WDTTitle_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("WDT_ToolTip"), OUTPUT);
		WDTTitleToolTip_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("WDT_Main"), OUTPUT);
		DeplotMain_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Logout"), OUTPUT);
		Backstep_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("SelectScenario"), OUTPUT);
		SelectScenario_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("WDTConfirm"), OUTPUT);
		Complete_text->SetText(FText::FromString(OUTPUT));
	}




}

void URt_Check_wdt::OnClicked_BackStepBtn()
{
	// 이전단계 : 시나리오 선택
	BPCall_Open_Page("ControlSelectMap");
}

void URt_Check_wdt::OnClicked_SelectBtn()
{
	// 시나리오 선택
	BPCall_Open_Page("ControlSelectMap");
}

void URt_Check_wdt::OnClicked_ConfrimBtn()
{
	// 원래는 wdt 신호를 체크해야함
	if (auto* hud = Get_HUD()) {
		FRtDele_MessagResult ResultCallback = FRtDele_MessagResult::CreateLambda([this](ERt_MessageResult ConfirmationResult) {
			if (ConfirmationResult == ERt_MessageResult::Confirmed) {
				// Log Out Data Code 
				if (URtConfig::PlayerNum == 10) {
					BPCall_Open_Page("ControlPlaceTrainee");
				}
				else if (URtConfig::PlayerNum == 20) {
					BPCall_Open_Page("ControlPlaceTrainee20");
				}
			}
			});
		hud->Show_MessageBox("WDT", ResultCallback);
	}
}

