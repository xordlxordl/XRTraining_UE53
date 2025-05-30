// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Trainee/Rt_Trainee_Result.h"

// Data
#include "Common/RtWebCommon.h"

// Manager
#include "Manager/RtConfig.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"

//UI
#include "UI/Control/Rt_Control_ResultDetail_Block.h"

void URt_Trainee_Result::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Init_ResultList();
}

void URt_Trainee_Result::Update_ResultList(FResponse_TrngSimpleResult ResultSturct)
{
	Role_Block->SetTextBlocks(ResultSturct.Role);
	Time_Block->SetTextBlocks(ResultSturct.PlayTimeSec);
	Distance_Block->SetTextBlocks(ResultSturct.Distance);
	FireCount_Block->SetTextBlocks(ResultSturct.FireCnt);
	HitCount_Block->SetTextBlocks(ResultSturct.HitCnt);
	BadFireCount_Block->SetTextBlocks(ResultSturct.BadFireCnt);

	ChallengeCount_Block->SetTextBlocks(ResultSturct.ChallengeCnt);
	SurrenderCount_Block->SetTextBlocks(ResultSturct.SurrenderCnt);
	BindCount_Block->SetTextBlocks(ResultSturct.BindCnt);
	Breif_Block->SetTextBlocks(ResultSturct.BreifCnt);
	Call_Block->SetTextBlocks(ResultSturct.CallCnt);
	EOD_Block->SetTextBlocks(ResultSturct.EODCnt);
}

void URt_Trainee_Result::Init_ResultList()
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
	DetailStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("BadFireCount"), OUTPUT);
	BadFireCount_Block->InitTextBlocks(OUTPUT);

	DetailStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("ChallengeCount"), OUTPUT);
	ChallengeCount_Block->InitTextBlocks(OUTPUT);
	DetailStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("SurrenderCount"), OUTPUT);
	SurrenderCount_Block->InitTextBlocks(OUTPUT);
	DetailStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("BindCount"), OUTPUT);
	BindCount_Block->InitTextBlocks(OUTPUT);
	DetailStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("BreifCount"), OUTPUT);
	Breif_Block->InitTextBlocks(OUTPUT);
	DetailStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("CallCount"), OUTPUT);
	Call_Block->InitTextBlocks(OUTPUT);
	DetailStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("EodCount"), OUTPUT);
	EOD_Block->InitTextBlocks(OUTPUT);
}