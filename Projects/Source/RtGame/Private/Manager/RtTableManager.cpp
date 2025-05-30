// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/RtTableManager.h"
// UE Main
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Manager/RtConfig.h"
// Data
#include "Common/RtDataTableRows.h" 

// Table Load Macro
#define RTDEF_TABLE_PATH "/Game/RS/Common/Table/"
#define RTDEF_LOAD_TABLE(table_value, table_name) \
static ConstructorHelpers::FObjectFinder<UDataTable>Object_##table_value(TEXT(RTDEF_TABLE_PATH#table_name"."#table_name)); \
if (Object_##table_value.Succeeded() && Object_##table_value.Object != nullptr) { \
		table_value = Object_##table_value.Object;	\
}


URtTableManager::URtTableManager()
{
	RTDEF_LOAD_TABLE(ControlWidget_Table, DT_Widget);
	RTDEF_LOAD_TABLE(MessageBox_Table_Eng, DT_MessageBox_Eng);
	RTDEF_LOAD_TABLE(MessageBox_Table, DT_MessageBox);
}

void URtTableManager::Initialize(FSubsystemCollectionBase& Collection)
{


}

void URtTableManager::Deinitialize()
{
}

FRtRow_UI* URtTableManager::Get_UI(const FName& row_name)
{
	FRtRow_UI* OutRow = ControlWidget_Table->FindRow<FRtRow_UI>(row_name, "");
	return OutRow;
}

FRtTableRow_MessageBox* URtTableManager::Get_MessageBox(const FName& row_name)
{
	FRtTableRow_MessageBox* OutRow;
	switch (URtConfig::NationCode)
	{
	case 1:
		OutRow = MessageBox_Table_Eng->FindRow<FRtTableRow_MessageBox>(row_name, "");
		break;

	case 82:
		OutRow = MessageBox_Table->FindRow<FRtTableRow_MessageBox>(row_name, "");
		break;

	default:
		OutRow = nullptr;
		break;
	}

	return OutRow;
}

TSubclassOf<UUserWidget> URtTableManager::Get_UIWidget(const FName& row_name)
{
	if (FRtRow_UI* table_row = Get_UI(row_name)) {
		return table_row->WidgetClass;
	}
	return nullptr;
}
