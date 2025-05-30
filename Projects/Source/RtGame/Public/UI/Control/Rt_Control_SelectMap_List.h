// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"
// UE
#include "Components/VerticalBox.h"

// Data
#include "Common/RtRpcCommon.h"

#include "Rt_Control_SelectMap_List.generated.h"


class URt_Control_SelectMap;
class URt_Control_SelectMap_Info; 
class URt_Control_SelectMap_Block;

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Control_SelectMap_List : public URt_UserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))	UVerticalBox* VB_ScenarioList;

	URt_Control_SelectMap* ParentsWidget;
	URt_Control_SelectMap_Info* ContentsInfo;

	URt_Control_SelectMap_Block* SelectedBlock;



// Function
	void Set_ScenarioList(const TArray<FString>& ScenarioList);
	void Set_OwnerWidget(URt_Control_SelectMap* parenstwidget);
	void Set_ContentsInfo(URt_Control_SelectMap_Info* contentsinfo);
	void Set_ActiveCheck(URt_Control_SelectMap_Block* CurrSelectedBlock);
	
	void FillContentsBlock(const FRtScenarioInfo& InInfo);
};
