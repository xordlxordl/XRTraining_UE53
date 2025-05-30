// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/RtAssetManager.h"

DEFINE_LOG_CATEGORY(LogRtAssetManager);

URtAssetManager& URtAssetManager::Get()
{
	URtAssetManager* This = Cast<URtAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return *This;
	}
	else
	{
		return *NewObject<URtAssetManager>(); // never calls this
	}
}

void URtAssetManager::Preload(FStreamableDelegate DelegateToCall)
{
	FPrimaryAssetType AssetType = FPrimaryAssetType(URtModeSettingData::StaticClass()->GetFName());
	TArray< FPrimaryAssetId> AssetsToLoad;
	GetPrimaryAssetIdList(AssetType, AssetsToLoad);
	TArray<FName> LoadBundles;
	
	LoadHandle = LoadPrimaryAssets(AssetsToLoad, LoadBundles);
	
	if (LoadHandle.IsValid())
	{
		if (!LoadHandle->HasLoadCompleted())
		{
			LoadHandle->BindCompleteDelegate(DelegateToCall);
			return;
		}
	}
}

void URtAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
}



URtModeSettingData* URtAssetManager::ForceLoad_ModeDefinition(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{
	FSoftObjectPath ObjPath = GetPrimaryAssetPath(PrimaryAssetId);

	// This does a synchronous load and may hitch
	URtModeSettingData* obj = Cast<URtModeSettingData>(ObjPath.TryLoad());

	if (bLogWarning && obj == nullptr)
	{
		UE_LOG(LogRtAssetManager, Warning, TEXT("Failed to obj for identifier %s!"), *PrimaryAssetId.ToString());
	}

	return obj;
}

URtModeSettingData* URtAssetManager::Load_ModeSetting()
{
	FPrimaryAssetId AssetId = FPrimaryAssetId(FPrimaryAssetType(URtModeSettingData::StaticClass()->GetFName()), "ModeDatas");
	return ForceLoad_ModeDefinition(AssetId, true);
}
