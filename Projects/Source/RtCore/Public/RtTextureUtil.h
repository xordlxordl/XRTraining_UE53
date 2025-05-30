// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RtTextureUtil.generated.h"

/**
 * 
 */
UCLASS()
class RTCORE_API URtTextureUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "URtTextureUtil")
	static UTexture2D* LoadTexture2D_FromFile(const FString& FullFilePath, bool& IsValid, int32& Width, int32& Height);

	UFUNCTION(BlueprintCallable, Category = "URtTextureUtil")	
	static UMaterialInstanceDynamic* CreateDefaultMaterial(const FString& basePath, const FString& normalPath, const FString& specularPath);

	UFUNCTION(BlueprintCallable, Category = "URtTextureUtil")
	static UMaterialInstanceDynamic* CreateMaterialFromBaseTexture(UMaterialInterface* pMaterialInterface, const FString& basePath);

	UFUNCTION(BlueprintCallable, Category = "URtTextureUtil")
	static bool SetMaterialFromBaseTexture(UMaterialInstanceDynamic* pMaterialInterface, const FString& basePath);
};
