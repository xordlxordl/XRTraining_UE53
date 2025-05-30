// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Control/Rt_Control_SelectMap_Block.h"
#include "UI/Control/Rt_Control_SelectMap_List.h"

#include "Kismet/KismetTextLibrary.h"

void URt_Control_SelectMap_Block::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Scenario_Btn) {
		Scenario_Btn->OnClicked.AddDynamic(this, &URt_Control_SelectMap_Block::FillScenarioInfo);
	}

}


void URt_Control_SelectMap_Block::FillScenarioInfo()
{
	if (ParentsWidget)
	{
		ParentsWidget->FillContentsBlock(ScenarioInfo);
		ParentsWidget->Set_ActiveCheck(this);
	}
}


void URt_Control_SelectMap_Block::Set_ScenarioName(const FRtScenarioInfo& InStruct)
{
	ScenarioInfo = InStruct;

	if (TB_ScenarioName)
	{
		FString ScenarioName = ScenarioInfo.Name;
		FText Text_ScenarioName = UKismetTextLibrary::Conv_StringToText(ScenarioName);

		TB_ScenarioName->SetText(Text_ScenarioName);
	}
}
