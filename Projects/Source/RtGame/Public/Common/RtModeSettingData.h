// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// Data
#include "RtCore/Public/Common/RtCommon.h"

#include "RtModeSettingData.generated.h"


USTRUCT()
struct FRtModeInfo
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere)
		FName UI_Name;

	UPROPERTY(EditAnywhere)
		uint8 ShowMouseCursor : 1;

	FRtModeInfo() :UI_Name(NAME_None), ShowMouseCursor(false)
	{}
};
/**
 * 
 */
UCLASS()
class RTGAME_API URtModeSettingData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		FName StartUI_Fist;

	UPROPERTY(EditAnywhere)
		FName StartUI_ControlMode;

	UPROPERTY(EditAnywhere)
		FName StartUI_Client;

	UPROPERTY(EditAnywhere)
		FName StartUI_Server;

	UPROPERTY(EditAnywhere)
		FName StartUI_Spectator;

	UPROPERTY(EditAnywhere)
		FRtModeInfo ControlMode;

	UPROPERTY(EditAnywhere)
		FRtModeInfo ClientMode;

	UPROPERTY(EditAnywhere)
		FRtModeInfo ServerMode;

	UPROPERTY(EditAnywhere)
		FRtModeInfo TrainerMode;

	UPROPERTY(EditAnywhere)
		FRtModeInfo ReplayMode;

	UPROPERTY(EditAnywhere)
		FRtModeInfo DefaultMode;
public:
	FName Get_UI(ERt_PlayType InPlayType);
	const FRtModeInfo& Get_Mode(ERt_PlayType InPlayType);
};
