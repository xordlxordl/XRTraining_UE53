// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
// Data
#include "Common/RtModeSettingData.h"

#include "RtAssetManager.generated.h"


RTGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogRtAssetManager, Log, All);

/**
 * 
 */
UCLASS()
class RTGAME_API URtAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
// Functions
public:
    
	virtual void StartInitialLoading()override;


	/** Returns the current AssetManager object */
	static URtAssetManager& Get();

	void Preload(FStreamableDelegate DelegateToCall);
	
	// Load Mode Setting Data
	URtModeSettingData* ForceLoad_ModeDefinition(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning);
	URtModeSettingData* Load_ModeSetting();
	TSharedPtr<FStreamableHandle> LoadHandle;

};
