// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Result_DetailBox.h"
// Data
#include "Common/RtWebCommon.h"

// Manager
#include "Manager/RtConfig.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"
//UI
#include "UI/Control/Rt_Control_ResultDetail_Block.h"

void URt_Result_DetailBox::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UStringTable* ControlStringTable = LoadObject<UStringTable>(nullptr, *URtConfig::ControlMainPath);
	if (ControlStringTable == nullptr) return;

	if (TraineeName && TrainingListVar && Result_text && ViewPath_text)
	{
		FString OUTPUT;
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Trainee"), OUTPUT);
		TraineeName->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("TrainingList"), OUTPUT);
		TrainingListVar->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("ResultTraining"), OUTPUT);
		Result_text->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("TraineePath"), OUTPUT);
		ViewPath_text->SetText(FText::FromString(OUTPUT));
	}

	Init_ResultList();
}

void URt_Result_DetailBox::Update_ResultList(FResponse_TrngSimpleResult ResultSturct)
{
	TraineeName->SetText(FText::FromString(ResultSturct.Name));

	Role_Block->SetTextBlocks(ResultSturct.Role);
	Time_Block->SetTextBlocks(ResultSturct.PlayTimeSec);
	Distance_Block->SetTextBlocks(ResultSturct.Distance);
	FireCount_Block->SetTextBlocks(ResultSturct.FireCnt);
	HitCount_Block->SetTextBlocks(ResultSturct.HitCnt);
	ChallengeCount_Block->SetTextBlocks(ResultSturct.ChallengeCnt);
	SurrenderCount_Block->SetTextBlocks(ResultSturct.SurrenderCnt);
	BindCount_Block->SetTextBlocks(ResultSturct.BindCnt);
	Command_Block->SetTextBlocks(ResultSturct.CommandMoveCnt);
	Breif_Block->SetTextBlocks(ResultSturct.BreifCnt);
	Call_Block->SetTextBlocks(ResultSturct.CallCnt);
	EOD_Block->SetTextBlocks(ResultSturct.EODCnt);
	BadFireCount_Block->SetTextBlocks(ResultSturct.BadFireCnt);
}

void URt_Result_DetailBox::Init_ResultList()
{
	UStringTable* DetailStringTable = LoadObject<UStringTable>(nullptr, *URtConfig::ResultDetailPath);
	if (DetailStringTable == nullptr) return;

	FString OUTPUT;
	DetailStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Job"), OUTPUT);
	Role_Block->InitTextBlocks(OUTPUT);
	DetailStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("PlayTime"), OUTPUT);
	Time_Block->InitTextBlocks(OUTPUT);
	DetailStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Distance"), OUTPUT);
	Distance_Block->InitTextBlocks(OUTPUT);
	DetailStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("FireCount"), OUTPUT);
	FireCount_Block->InitTextBlocks(OUTPUT);
	DetailStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("HitCount"), OUTPUT);
	HitCount_Block->InitTextBlocks(OUTPUT);
	DetailStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("ChallengeCount"), OUTPUT);
	ChallengeCount_Block->InitTextBlocks(OUTPUT);
	DetailStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("SurrenderCount"), OUTPUT);
	SurrenderCount_Block->InitTextBlocks(OUTPUT);
	DetailStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("BindCount"), OUTPUT);
	BindCount_Block->InitTextBlocks(OUTPUT);
	DetailStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("CommanMoveCount"), OUTPUT);
	Command_Block->InitTextBlocks(OUTPUT);
	DetailStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("BreifCount"), OUTPUT);
	Breif_Block->InitTextBlocks(OUTPUT);
	DetailStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("CallCount"), OUTPUT);
	Call_Block->InitTextBlocks(OUTPUT);
	DetailStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("EodCount"), OUTPUT);
	EOD_Block->InitTextBlocks(OUTPUT);
	DetailStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("BadFireCount"), OUTPUT);
	BadFireCount_Block->InitTextBlocks(OUTPUT);
}
