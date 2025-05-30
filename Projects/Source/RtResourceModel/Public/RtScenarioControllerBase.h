// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Models/RtScenarioModel.h"
#include "RtSceneManagerBase.h"

#include "InputCoreTypes.h"

#include "RtScenarioControllerBase.generated.h"

UCLASS()
class RTRESOURCEMODEL_API ARtScenarioControllerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARtScenarioControllerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	TArray<FString> ParseCommaString(FString strData);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool LoadSctOnlyDataStruct(FString sctFilePath);

	UFUNCTION(BlueprintCallable)
	bool LoadSCTData(FString sctFilePath);

	UFUNCTION(BlueprintCallable)
	bool SetSceneManagerRef(ARtSceneManagerBase* pSceneManager = nullptr);

	UFUNCTION(BlueprintCallable)
	ARtSceneManagerBase* GetSceneManagerRef();

	UFUNCTION(BlueprintCallable)
	void UnloadSct();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<ARtScenarioModel> ScenarioModel; // sct 데이터를 들고 있는 액토 변수.

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TMap<FString, AActor*> MainObjectMap; // sct 처리를 위한 핵심 객체 맵.

	FRtScenarioPlayer MyScenarioPlayer; // 자신의 플레이어 정보.

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<ARtSceneManagerBase> SceneManagerRef; // sct 데이터를 들고 있는 액토 변수.

	bool IsSingleMode = false;

public:
	static ARtScenarioControllerBase* Get(UObject* worldContext);
	bool GetIsSingleMode() { return IsSingleMode; }
};
