// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "RtEnums.h"

#include "RtResourceModelSubsystem.generated.h"

UENUM(BlueprintType)
enum class FRtScriptParsingResult : uint8 {
	ENone = 0,
	ESuccess = 1,

	EErrorNoInputData,
	EErrorParamCount,
	EErrorParamType,
};

class FRtSctFunctionCommandParam
{
public:
	FRtSctFunctionCommandParam()
	{
		Init();
	}

	FRtSctFunctionCommandParam(ERtSctFunCmdParam Type, FString OriginData)
	{
		SetData(Type, OriginData);
	}

	ERtSctFunCmdParam ParamType;

	FString ParamString;
	TArray<FString> ParamStringList;

	int32 ParamInt;
	float Paramfloat;

	void Init();

	bool SetData(ERtSctFunCmdParam Type, FString OriginData);

	bool IsParamString() {
		return (ERtSctFunCmdParam::ENone < ParamType && ParamType < ERtSctFunCmdParam::EIntValue);
	}

	bool IsParamInt() {
		return ParamType == ERtSctFunCmdParam::EIntValue;
	}

	bool IsParamFloat() {
		return ParamType == ERtSctFunCmdParam::EFloatValue;
	}

	bool IsParamStringList() {
		return ParamType == ERtSctFunCmdParam::EStringList;
	}

	FString ToString();
};

USTRUCT(BlueprintType, Category = "FRtSctFunctionCommand")
struct FRtSctFunctionCommand
{
	GENERATED_BODY()

public:
	FRtSctFunctionCommand()
	{
		FunctionCommand = ERtSctFunCmd::ENone;
	}

	UPROPERTY(BlueprintReadOnly)
	ERtSctFunCmd FunctionCommand;

	//	UPROPERTY(BlueprintReadOnly)
	TArray<FRtSctFunctionCommandParam> Params;

	FString ToString()
	{
		FString DebugString = FString::Printf(TEXT("Command : %s\n"), *UEnum::GetValueAsString(FunctionCommand));
		DebugString += "Params : {\n";

		for (FRtSctFunctionCommandParam param : Params)
			DebugString += FString::Printf(TEXT("\t%s\n"), *param.ToString());
		
		DebugString += "}\n";

		return DebugString;
	}
};

/**
 * RealTimeFbx 플러그인 기능
 * 기타 리소스 모델 지원 기능들
 */
UCLASS()
class RTRESOURCEMODEL_API URtResourceModelSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	//////////////////////////////////////////////////////////////////////////
	// 
	// C++ 기본 참조 유틸리티
public:
	static void InitReference(UGameInstance* pGameInstance);
	static URtResourceModelSubsystem* Get() { return RMSystem; }

protected:
	static URtResourceModelSubsystem* RMSystem;
	// 
	//////////////////////////////////////////////////////////////////////////
public:
	URtResourceModelSubsystem();

	void Init();

	UFUNCTION(BlueprintCallable, Category = "URtResourceModelSubsystem")
	int32 ScriptParsingComma(FString scriptData, TArray<FString>& scriptUnitList);

	//UFUNCTION(BlueprintCallable, Category = "URtResourceModelSubsystem")
	FString ConvertAnimationItem(TArray<FString>* parsedStrings, TArray<ERtSctFunCmdParam>* paramList, FString inputAnimItem);

	UFUNCTION(BlueprintCallable, Category = "URtResourceModelSubsystem")
	FRtScriptParsingResult ScriptParsing(FString ScriptData, FRtSctFunctionCommand& Command);

	UFUNCTION(BlueprintCallable, Category = "URtResourceModelSubsystem")
	UMaterialInstanceDynamic* CreateMaterial(UTexture2D* pDiffTex, UTexture2D* pNormalTex);

	UFUNCTION(BlueprintCallable, Category = "URtResourceModelSubsystem")
	UMaterialInstanceDynamic* CreateAlphaMaterial(UTexture2D* pAlphaTex, FLinearColor diffColor);

	UFUNCTION(BlueprintCallable, Category = "URtResourceModelSubsystem")
	UMaterialInstanceDynamic* CreateColorMaterial(FLinearColor diffColor);

	UFUNCTION(BlueprintCallable, Category = "URtResourceModelSubsystem")
	bool MakeOneModelStructToObjectActor(ARtObjectActor* pObjectActor, struct FRTIModelStruct& modelStruct, int32& sectionIndex);

	UFUNCTION(BlueprintCallable, Category = "URtResourceModelSubsystem")
	bool LoadFbxFileToObjectActor_Sub(ARtObjectActor* pObjectActor, TArray<struct FRTIModelStruct> modelStructs, bool isForGame);

	UFUNCTION(BlueprintCallable, Category = "URtResourceModelSubsystem")
	bool LoadFbxFileToObjectActor(FString fbxFileName, ARtObjectActor* pObjectActor, bool isForGame, FglTFRuntimeFBX_PreLoadInfo preLoadInfo);

	UFUNCTION(BlueprintCallable, Category = "URtResourceModelSubsystem")
	void SetFbxModelPathBase(FString fbxModelPathBase);

	UFUNCTION(BlueprintCallable, Category = "URtResourceModelSubsystem")
	void ClearFbxProMeshCloneMap();

	UFUNCTION(BlueprintCallable, Category = "URtResourceModelSubsystem")
	void LoadFbxModelPathBase();

	UFUNCTION(BlueprintCallable, Category = "URtResourceModelSubsystem")
	UglTFRuntimeAsset* LoadGltfAssetSub(const FString& fbxFileName);

	UFUNCTION(BlueprintCallable, Category = "URtResourceModelSubsystem")
	UglTFRuntimeAsset* LoadGltfAsset(const FString& fbxFileName, ARtObjectActor* pObjectActor, bool isForGame);

	UFUNCTION(BlueprintCallable, Category = "URtResourceModelSubsystem")
	FString GetFbxModelPathBase() {
		if (FbxModelPathBase.IsEmpty()) {
			LoadFbxModelPathBase();
		}
		return FbxModelPathBase;
	}

	UFUNCTION(BlueprintCallable, Category = "URtResourceModelSubsystem")
	void UpdateTrainingDataPath(bool isDefault);

	UFUNCTION(BlueprintCallable, Category = "URtResourceModelSubsystem")
	FString GetTrainingDataPath() {
		if (TrainingDataPath.IsEmpty())
			UpdateTrainingDataPath(true);

		return TrainingDataPath;
	}
	
	UFUNCTION(BlueprintCallable, Category = "SinglePlay")
	FString GetSingleSctFileName() {
		if (Single_SctFileName.IsEmpty())
			UpdateTrainingDataPath(false);

		return Single_SctFileName;
	}
	
	UFUNCTION(BlueprintCallable, Category = "SinglePlay")
	FString GetSingleRoleID() {
		if (Single_RoleID.IsEmpty())
			UpdateTrainingDataPath(false);

		return Single_RoleID;
	}
	
	//UPROPERTY(BlueprintReadOnly)
	TMap<FString, ERtSctFunCmd> FunctionConverterMap;
	TMap<ERtSctFunCmd, TArray<ERtSctFunCmdParam>> FunctionCommandParamInfo;

	TMap<FString, TArray<struct FRTIModelStruct>> FbxProMeshCloneMap;

	TMap<FString, bool> FbxCreatorBlenderMap;

public:

#if WITH_EDITOR	
	void InitFunctionScriptInfoCollector() {
		FuctionSpecCollectorMap.Empty();
		AllFuctionCommandList.Empty();
	}
	const TMap<FString, FString> GetFuctionSpecCollectorMap() {
		return FuctionSpecCollectorMap;
	}
	const TArray<FString> GetAllFunctionCommandList() {
		return AllFuctionCommandList;
	}
	TMap<FString, FString> FuctionSpecCollectorMap; // 함수 명세 수집기.
	TArray<FString> AllFuctionCommandList; // 모든 함수 리스트 검사기.
#endif

	UMaterialInterface* StartPosNotReadyMaterial;
	UMaterialInterface* StartPosReadyMaterial;

public:

	// ERtEquipComSocketComboType 참고
	UPROPERTY(BlueprintReadOnly)
	FString CharacterSocket1_Mouth = TEXT("mouth_socket");

	// ERtEquipComSocketComboType 참고
	UPROPERTY(BlueprintReadOnly)
	FString CharacterSocket2_RHand = TEXT("r_hand_socket");

	UPROPERTY(BlueprintReadOnly)
	FString FbxImportErrorMessage = TEXT("");
protected:

	UPROPERTY()
	class URealTimeImportMeshFBX* ImporterFBX = nullptr;

	UPROPERTY(BlueprintReadOnly)
	FString FbxModelPathBase = TEXT("");

	UPROPERTY(BlueprintReadOnly)
	FString TrainingDataPath = TEXT("");

	UPROPERTY(BlueprintReadOnly)
	FString Single_SctFileName = TEXT("");

	UPROPERTY(BlueprintReadOnly)
	FString Single_RoleID = TEXT("");

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UglTFRuntimeAsset* ModelAssetBP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterialInterface* tempMat;

public:
	UFUNCTION(BlueprintCallable, Category = "EffekseerTest")
	static FString GetFolderNameFromPath(const FString& FilePath);

	UFUNCTION(BlueprintCallable, Category = "EffekseerTest")
	UEffekseerEffect* ImportEffekseerEffect(const FString& FilePath);

	UFUNCTION(BlueprintCallable, Category = "EffekseerTest")
	void PlayEffekseerEffectLooping(UEffekseerEmitterComponent* newEffekseer);

	UFUNCTION(BlueprintCallable, Category = "EffekseerTest")
	void PlayEffekseerEffectOnce(UEffekseerEmitterComponent* newEffekseer);

	UFUNCTION(BlueprintCallable, Category = "EffekseerTest")
	bool StopEffekseerEffect(AActor* firstActor);

	UFUNCTION(BlueprintCallable, Category = "EffekseerTest")
	void SetEffekseerEffectSpeed(AActor* firstActor, float newSpeed);
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	TObjectPtr<class UEffekseerSystemComponent> EffectSystemComponent;
	TObjectPtr<class UEffekseerEmitterComponent> EffectEmitterComponent;
	TArray<TObjectPtr<class UEffekseerEffect>> EffectFileArray;
	int32 EffectInitCounter = 0;
	FTimerHandle EffectInitTimerHandle;

	UFUNCTION(BlueprintCallable)
	bool InitEffekseerEffect(FString efkFolderPath);

	UFUNCTION(BlueprintCallable)
	bool GetEffekseerEmitterComponent(AActor* firstActor, UEffekseerEmitterComponent*& emitterComponent);

	UFUNCTION(BlueprintCallable)
	bool PlayEffekseerEffect(AActor* firstActor, bool bIsLoop);

	UFUNCTION(BlueprintCallable)
	bool AttachEffekseerEmitterComponent(AActor* firstActor, UEffekseerEmitterComponent*& emitterComponent);

	UFUNCTION(BlueprintCallable)
	bool SetEffekseerEffect(AActor* firstActor, const FString& FilePath);

	UFUNCTION(BlueprintCallable)
	bool ImportEffekseerEffectCVersion(AActor* firstActor, const FString& FilePath, bool bIsLoop);

	UFUNCTION(BlueprintCallable)
	void UpdateStringTableFromAsset(const FString& AssetPath, const FString& TargetTableName);

public:

	UFUNCTION(BlueprintCallable)
	int32 CheckTctFileVersion(FString tctFilePath);

	UFUNCTION(BlueprintCallable)
	static int32 GetMapFileVersion(FString mapFilePath);

	UPROPERTY(BlueprintReadWrite)
	ERtResModelLastError LastError;

	UFUNCTION(BlueprintCallable)
	static FString GetEnumToString(FString enumClassName, int32 enumValue);

};
