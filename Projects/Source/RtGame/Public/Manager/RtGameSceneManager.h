// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RtSceneManagerBase.h"
#include "RtGameSceneManager.generated.h"

/**
 * 
 */
class ADirectionalLight;
class ASkyAtmosphere;
class ARtReplayManagerActor;
class UAudioComponent;


DECLARE_MULTICAST_DELEGATE(FOnMapLoadComplete_Delegate)

UCLASS()
class RTGAME_API ARtGameSceneManager : public ARtSceneManagerBase
{
	GENERATED_BODY()
public:
	struct FInitObjectData
	{
	public:
		FInitObjectData(ARtObjectActor* objectActor);
		void InitData(ARtObjectActor* objectActor);

	public:
		FVector Loc;
		FRotator Rot;
		bool Visible;
		FString Anim;
		bool IsAnimLoop;
		AActor* AttachParent;

	};

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TWeakObjectPtr<class AStaticMeshActor> Floor;

public:
	ARtGameSceneManager();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	AActor* AddSpawnNRegist(FRtObjectSpawnData spawnData);
//		ERtMctSpawnType mctSpawnType, FString modelKey, FString actorName, FVector vPos, FRotator rRot, FVector totalPosOffset, FRotator totalRotOffset, FMatrix rotationMatrix);

	// 간단한 프로젝트 시나리오 열기 함수.( 이 함수를 쓰시오 ) 
	UFUNCTION(BlueprintCallable)
	bool LoadSimpleProjectScenarioScene(FString subProjectDirName, FString subSysDirName, FString scenarioFileName, FRotator totalRotOffset, FVector totalPosOffset);

	// 저급 시나리오 열기 함수.
	UFUNCTION(BlueprintCallable)
	bool LoadScenarioScene(FString tctFilesPath, FString sysFilesPath, FString scenarioFilePath, bool bIsFireZone, FRotator totalRotOffset, FVector totalPosOffset);

	UFUNCTION(BlueprintCallable)
	bool SpawnMctItemProcessTM(FVector totalPosOffset, FRotator totalRotOffset, FMatrix rotationMatrix, bool asyncLoad, ERtMctSpawnType spawnType, FTemplateBase item);

	UFUNCTION(BlueprintCallable)
	void SpawnMctItemProcess(bool asyncLoad, FRtObjectSpawnData & spawnData);

	UFUNCTION(BlueprintCallable)
	bool SpawnMapData(FString tctFilesPath, FString sysFilesPath, FString mctFilePath, FRotator totalRotOffset, FVector totalPosOffset, bool asyncLoad = true);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<class ARtNpcPawn>> NpcList; // 생성된 Npc 객체들을 관리한다.

	UFUNCTION(BlueprintCallable)
	void SetDayNight(bool isNight);

	UFUNCTION(BlueprintCallable)
	void AsyncMapLoadProcess();
	
	UFUNCTION(BlueprintCallable)
	ARtObjectActor* GetPlayerMeshActor(const FString& ObjectKeyName);


	// dave - test
	UFUNCTION(BlueprintCallable)
	void DestroyAllSpawnedObjects();
	void ClearReplay();

	ARtObjectActor* FindObjectByName(FString objectName) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class URtGameManager* Get_GameManager();

protected:
	virtual void Server_DestroyAllSpawnedObjects_Implementation();
	virtual bool Server_DestroyAllSpawnedObjects_Validate();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ADirectionalLight* Day_DirectionalLight = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ASkyAtmosphere* Day_SkyAtmosphere = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ADirectionalLight* Night_DirectionalLight = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ASkyAtmosphere* Night_SkyAtmosphere = nullptr;

	// 비동기 맵 로딩을 위한 변수들.
	TArray<FRtObjectSpawnData> AsyncMapLoadInfoArray;
	int32 AsyncMapLoadInfoArraySize;

	TMap<ARtObjectActor*, FInitObjectData> InitObjectDataMap; // Object, FInitObjectData

	void SetInitObjectDatas();
	void AddInitObjectDatas(const FRtEventCommand& eventCommand);

	TMap<FString, ARtObjectActor*> RtObjectMap;
	TMap<FString, class ARtNpcObjectActor*> RtNpcMap;
	TMap<FString, class ARtTraineePawn*> RtTraineeMap;

	TArray<FTransform> FreeMoveAreaList;

	void CreateTctSoundWaves();
	USoundWave* CreateSoundWaveByPath(FString filePath);
	TMap<FString, USoundWave*> SoundWaveMap; // FileName, Wave
	
	void CreateGuideHands();
	void CreateListenIcons();

public:
	TMap<ARtObjectActor*, FInitObjectData> GetInitObjectDatas() { return InitObjectDataMap; }

	const TMap<FString, ARtObjectActor*>& GetRtObjectMap() { return RtObjectMap; }
	const TMap<FString, class ARtNpcObjectActor*>& GetRtNpcMap() { return RtNpcMap; }
	const TMap<FString, class ARtTraineePawn*>& GetRtTraineeMap() { return RtTraineeMap; }
	void AddTraineeMap(FString roleID, ARtTraineePawn* traineePawn) { RtTraineeMap.Add({ roleID, traineePawn }); }

	const TArray<FTransform> GetFreeMoveAreaList() { return FreeMoveAreaList; }

	FString CurrentSctFileName;

	FOnMapLoadComplete_Delegate OnMapLoadComplete;

public:
	static ARtGameSceneManager* Get(UObject* worldContext) { return Cast<ARtGameSceneManager>(ARtSceneManagerBase::Get(worldContext)); };

};
