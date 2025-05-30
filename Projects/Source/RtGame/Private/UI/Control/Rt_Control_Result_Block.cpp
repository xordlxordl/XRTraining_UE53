// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Control_Result_Block.h"
// Manager
#include "Manager/RtConfig.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"

void URt_Control_Result_Block::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UStringTable* ControlStringTable = LoadObject<UStringTable>(nullptr, *URtConfig::ControlMainPath);
	if (ControlStringTable == nullptr) return;

	if (Text_Device && Text_Role && Text_Trainee && Text_SN)
	{
		FString OUTPUT;
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("DeviceSet"), OUTPUT);
		Text_Device->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Role"), OUTPUT);
		Text_Role->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("TraineeName"), OUTPUT);
		Text_Trainee->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("TraineeID"), OUTPUT);
		Text_SN->SetText(FText::FromString(OUTPUT));

	}
}

void URt_Control_Result_Block::NativeConstruct()
{
	Super::NativeConstruct();
}

void URt_Control_Result_Block::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}

void URt_Control_Result_Block::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
}

