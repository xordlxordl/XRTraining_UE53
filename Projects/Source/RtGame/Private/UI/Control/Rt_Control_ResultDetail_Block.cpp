// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Control_ResultDetail_Block.h"
#include "UI/Control/Rt_Control_Result.h"
#include "Kismet/KismetTextLibrary.h"


void URt_Control_ResultDetail_Block::InitTextBlocks(FString list)
{
	if (TrainingList_var)
	{
		FString TrainingList = list;
		FText Text_TrainingList = UKismetTextLibrary::Conv_StringToText(list);
		TrainingList_var->SetText(Text_TrainingList);
	}
}
void URt_Control_ResultDetail_Block::SetTextBlocks(FString result)
{
	
	if (TrainingResult_var)
	{
		FString TrainingResult = result;
		FText Text_TrainingResult = UKismetTextLibrary::Conv_StringToText(result);
		TrainingResult_var->SetText(Text_TrainingResult);
	}

}

