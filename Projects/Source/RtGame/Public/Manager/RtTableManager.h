// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
// UE Basic
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"

// Data
#include "Common/RtDataTableRows.h"

#include "RtTableManager.generated.h"

/**
 * 
 */
UCLASS()
class RTGAME_API URtTableManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
		URtTableManager();

// Various
public:

	UPROPERTY()	UDataTable* ControlWidget_Table;
	UPROPERTY()	UDataTable* MessageBox_Table;
	UPROPERTY()	UDataTable* MessageBox_Table_Eng;

// Functions
public:
    // Main Flow
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Custom Get Set
	FRtRow_UI* Get_UI(const FName& row_name);
	FRtTableRow_MessageBox* Get_MessageBox(const FName& row_name);
	TSubclassOf<UUserWidget> Get_UIWidget(const FName& row_name);
};
