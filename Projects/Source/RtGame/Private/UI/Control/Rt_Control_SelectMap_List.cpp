// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Control_SelectMap_List.h"

// Framework
#include "Framework/RtPlayerController.h"
#include "Framework/RtGameState.h"
// Data
#include "Common/RtBlueprintAssetPath.h"
#include "Common/RtRpcCommon.h"
// Widget
#include "UI/Control/Rt_Control_SelectMap_Block.h"
#include "UI/Control/Rt_Control_SelectMap_Info.h"
#include "UI/Control/Rt_Control_SelectMap.h"

void URt_Control_SelectMap_List::Set_ScenarioList(const TArray<FString>& ScenarioList)
{
	if (auto* pc = Get_PlayerController())
	{
		ARtGameState* ps = Get_GameState();

		for (FString element : ScenarioList)
		{

			FRtScenarioInfo scenarioInfo;
			scenarioInfo = pc->ReadScenarioFile(element);
			// State 
			ps->Set_Scenario_Info(scenarioInfo);

			// 서버까지

			// Create Widget
			TSubclassOf<UUserWidget> widgetclaass;
			widgetclaass = LoadClass<UUserWidget>(nullptr, RtControlWidgetBlueprint::ControlSelectMapBlock);
			URt_Control_SelectMap_Block* widget = CreateWidget<URt_Control_SelectMap_Block>(GetWorld(), widgetclaass);
			widget->Set_ScenarioName(scenarioInfo);
			widget->ParentsWidget = this;

			if (VB_ScenarioList)
			{
				VB_ScenarioList->AddChild(widget);
			}
		}
	}
}

void URt_Control_SelectMap_List::Set_OwnerWidget(URt_Control_SelectMap* parenstwidget)
{
	ParentsWidget = parenstwidget;
}

void URt_Control_SelectMap_List::Set_ContentsInfo(URt_Control_SelectMap_Info* contentsinfo)
{
	ContentsInfo = contentsinfo;
}

void URt_Control_SelectMap_List::Set_ActiveCheck(URt_Control_SelectMap_Block* CurrSelectedBlock)
{
	if (VB_ScenarioList) {
		for (auto* blocks : VB_ScenarioList->GetAllChildren()) {
			if (blocks == CurrSelectedBlock) {
				Cast<URt_Control_SelectMap_Block>(blocks)->IsActive = true;
			}
			else {
				Cast<URt_Control_SelectMap_Block>(blocks)->IsActive = false;
			}
		}
	}
}

void URt_Control_SelectMap_List::FillContentsBlock(const FRtScenarioInfo& InInfo)
{
	if (ContentsInfo)
	{
		ContentsInfo->FillContentsTextBox(InInfo);
		if (ParentsWidget)
			ParentsWidget->SetCurrScenarioInfo(InInfo);
			ParentsWidget->HavingSelectedBlock = true;
	}
}
