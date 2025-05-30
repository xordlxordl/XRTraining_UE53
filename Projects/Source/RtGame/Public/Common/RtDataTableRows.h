// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
// Data
#include "RtCore/public/Common/RtCommon.h"
#include "RtDataTableRows.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct FRtRow_UI : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
UPROPERTY(EditAnywhere) TSubclassOf<class UUserWidget> WidgetClass;
UPROPERTY(EditAnywhere) FString Description;
};

USTRUCT(BlueprintInternalUseOnly)
struct FRt_MessageBoxButtonAction
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Rs")	ERt_MessageResult Result = ERt_MessageResult::Unknown;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Rs")	FText Text;
};

USTRUCT(BlueprintInternalUseOnly)
struct FRtTableRow_MessageBox : public FTableRowBase
{

	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RtControl")	FText Header;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RtControl")	FText Body;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RtControl")	FString DebugText;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RtControl")	TArray<FRt_MessageBoxButtonAction> ButtonArray;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RtControl") UTexture2D* Texture_Normal;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RtControl") UTexture2D* Texture_Hover;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RtControl") UTexture2D* Texture_Cancel;

};

/**
 * 
 */
UCLASS()
class RTGAME_API URtDataTableRows : public UDataTable
{
	GENERATED_BODY()
	
};
