// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/Control/Rt_Control_SelectMap_Info.h"

#include "Kismet/KismetTextLibrary.h"

// Manager
#include "Manager/RtConfig.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"

void URt_Control_SelectMap_Info::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UStringTable* ControlStringTable = LoadObject<UStringTable>(nullptr, *URtConfig::ControlMainPath);
	if (ControlStringTable == nullptr) return;
	if (ScenarioName_text && Summary_text && Participants_text && MCTFileName_text && Memo_text && ScenarioInfo_text) {

		FString OUTPUT;
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("ScenarioInfo"), OUTPUT);
		ScenarioInfo_text->SetText(FText::FromString(OUTPUT));
		
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("ScenarioName"), OUTPUT);
		ScenarioName_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Summary"), OUTPUT);
		Summary_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Participants"), OUTPUT);
		Participants_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("MCTFileName"), OUTPUT);
		MCTFileName_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("TrainingMemo"), OUTPUT);
		Memo_text->SetText(FText::FromString(OUTPUT));
	}

}



void URt_Control_SelectMap_Info::FillContentsTextBox(const FRtScenarioInfo& ScenarioInfo)
{
	if (TB_Destination)
	{
		FText scenarioName = UKismetTextLibrary::Conv_StringToText(ScenarioInfo.Name);
		TB_Destination->SetText(scenarioName);
	}

	if (TB_FileName)
	{
		FString FileName = ScenarioInfo.FileName;
		FText Text_FileName = UKismetTextLibrary::Conv_StringToText(FileName);
        
		SelectedFileName = ScenarioInfo.SCTFileName; 		//for Packet Send 
		TB_FileName->SetText(Text_FileName);
	}

	if (TB_PlayerNum_var)
	{
		int32 PlayerNumber = ScenarioInfo.PlayerNum;
		FText Text_PlayerNumber = UKismetTextLibrary::Conv_IntToText(PlayerNumber);

		TB_PlayerNum_var->SetText(Text_PlayerNumber);
	}


	if (TB_Summ)
	{
		FString Summary = ScenarioInfo.Summary;
		FText Text_Summary = UKismetTextLibrary::Conv_StringToText(Summary);

		TB_Summ->SetText(Text_Summary);
	}
}

