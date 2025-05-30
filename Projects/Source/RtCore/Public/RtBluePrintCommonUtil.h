// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/ComboBoxString.h" // Build.cs 에 UMG 추가 해야함.
#include "RtEnums.h"
#include "RtBluePrintCommonUtil.generated.h"

/**
 * 
 */
UCLASS()
class RTCORE_API URtBluePrintCommonUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "URtBluePrintCommonUtil")
	static int GetComboIndexFromString(UComboBoxString* pComboBox, FString comboValue);

	//UFUNCTION(BlueprintCallable, Category = "URtBluePrintCommonUtil")
	//static ERtComponentComboType ConvertIndexToEnum_ComponentComboType(ERtComponentComboType comboStart, int comboIndex);

	//UFUNCTION(BlueprintCallable, Category = "URtBluePrintCommonUtil")
	//static ERtTctComponentEleType ConvertIndexToEnum_ComponentEleType(ERtTctComponentEleType comboStart, int comboIndex);

	template<typename EnumType>
	static EnumType GetComboTypeFromString(UComboBoxString* pComboBox, EnumType comboStart, FString comboValue);

	UFUNCTION(BlueprintCallable, Category = "URtBluePrintCommonUtil")
	static ERtComponentComboType GetComponentComboTypeFromString(UComboBoxString* pComboBox, ERtComponentComboType comboStart, FString comboValue);

	UFUNCTION(BlueprintCallable, Category = "URtBluePrintCommonUtil")
	static ERtTctComponentEleType GetComponentEleTypeFromString(UComboBoxString* pComboBox, ERtTctComponentEleType comboStart, FString comboValue);

	UFUNCTION(BlueprintCallable, Category = "URtBluePrintCommonUtil")
	static TArray<FString> SearchContentFolderFiles(TArray<FString> basePathList, TArray<FString> assetFilter, TArray<FString> exceptNameList);

	UFUNCTION(BlueprintCallable, Category = "URtBluePrintCommonUtil")
	static bool FindAllFileList(const FString& folderPath, TArray<FString>& outFileList);

	UFUNCTION(BlueprintCallable, Category = "URtBluePrintCommonUtil")
	static FString RemoveLastFolder(const FString& InFolderPath);

	// 커스텀 파일에 로그 쓰기 함수.
	UFUNCTION(BlueprintCallable, Category = "URtBluePrintCommonUtil")
	static void WriteToFileLog(const FString& LogFileName, const FString& LogMessage);

	// 외부 실행 파일을 실행한다.
	UFUNCTION(BlueprintCallable, Category = "URtBluePrintCommonUtil")
	static void RunExternalExeFile(FString ExeFileName, const FString ExeFilePath);
};
