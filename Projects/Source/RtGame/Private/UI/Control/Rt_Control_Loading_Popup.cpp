// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Control_Loading_Popup.h"
// Manager
#include "Manager/RtConfig.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"


void URt_Control_Loading_Popup::Close_Page()
{
	Close_Popup(this);
}

void URt_Control_Loading_Popup::SetTextBlock(FString incontents)
{
	UStringTable* MessageStringTable = LoadObject<UStringTable>(nullptr, *URtConfig::MessageBoxPath);

	if (MessageStringTable == nullptr) return;

	FString OUTPUT;
	MessageStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey(incontents), OUTPUT);
	Text_Contents->SetText(FText::FromString(OUTPUT));
}
