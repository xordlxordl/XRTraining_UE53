// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProceduralMeshComponent.h"

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RtReadWriteFile.generated.h"


/**
 * 
 */
UCLASS()
class RTCORE_API URtReadWriteFile : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/**
	* Open a text File and Read its content
	* 
	* @param FilePath         the path of your file 
	* @param bOutSuccess      If the action was a success or not
	* @param OutInfoMessage  More information about the action's result
	* 
	* @return The string content of your file
	*/
	UFUNCTION(BlueprintCallable, Category = "Read Write File")
	static FString ReadStringFromFile(FString FilePath, bool& bOutSuccess, FString& OutInfoMessage);

	/**
	* Create a text file with your content
	* 
	* @param FilePath         The path of your file 
	* @param String           The string to want to write in the file 
	* @param bOutSuceess      If the action was a success or not 
	* @param OutInfoMessage   More information about the action's result
	* 
	*/

	UFUNCTION(BlueprintCallable, Category = "Read Write File")
	static void WriteStringToFile(FString filePath, FString String, bool& bOutSuccess, FString& OutInfoMessage);




	/**
	*  Load UAsset File from Path
	* 
	* @Param UAssetPath        The path of your UAssetFile Path 
	* ex) "/Game/TempCommonFbx/Wall/Wire_Fence/Wire_Fence.Wire_Fence",
	*/


	UFUNCTION(BlueprintCallable, Category = "Load UAsset File")
		static	UTexture2D* LoadTextureFromPath(const FString& UAssetPath);

	UFUNCTION(BlueprintCallable, Category = "Load UAsset File")
		static	UMaterialInstance* LoadMaterialFromPath(const FString& UAssetPath);

	UFUNCTION(BlueprintCallable, Category = "Load UAsset File")
		static	UStaticMesh* LoadStaticMeshFromPath(const FString& UAssetPath);

	UFUNCTION(BlueprintCallable, Category = "Load UAsset File")
		static	USkeletalMesh* LoadSkeletalFromPath(const FString& UAssetPath);

	UFUNCTION(BlueprintCallable, Category = "Load UAsset File")
	    static UNiagaraSystem* LoadNiagaraSystemFromPath(const FString& UAssetPath);

};
