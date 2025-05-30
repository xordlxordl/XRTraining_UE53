// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RtSceneManagerBase.h"
#include "RsSimSceneManager.generated.h"

/**
 * 
 */
UCLASS()
class RSGAME_API ARsSimSceneManager : public ARtSceneManagerBase
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	 
	UFUNCTION(BlueprintCallable)
	AActor* AddSpawnNRegist(ERtMctSpawnType mctSpawnType, FString modelKey, FString actorName, FVector vPos, FRotator rRot);

	// 간단한 프로젝트 시나리오 열기 함수.( 이 함수를 쓰시오 ) 
	UFUNCTION(BlueprintCallable)
	bool LoadSimpleProjectScenarioScene(FString subProjectDirName, FString subSysDirName, FString scenarioFileName, FRotator totalRotOffset, FVector totalPosOffset);

	// 저급 시나리오 열기 함수.
	UFUNCTION(BlueprintCallable)
	bool LoadScenarioScene(FString tctFilesPath, FString sysFilesPath, FString scenarioFilePath, bool bIsFireZone, FRotator totalRotOffset, FVector totalPosOffset);

	UFUNCTION(BlueprintCallable)
	bool SpawnMapData(FString tctFilesPath, FString sysFilesPath, FString mctFilePath, FRotator totalRotOffset, FVector totalPosOffset);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<class ARsSimNPC>> NpcList; // 생성된 Npc 객체들을 관리한다.
};
