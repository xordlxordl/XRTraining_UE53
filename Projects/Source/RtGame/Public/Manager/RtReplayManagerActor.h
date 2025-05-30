// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RtReplayManagerActor.generated.h"

class ARtTraineePawn;
class ARtReplayTrainee;
class ARtGameScenarioController;
class ARtObjectActor;
class ARtReplayManagerActor;
class ARtNpcObjectActor;

USTRUCT(BlueprintType)
struct FMotionTransformData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Scale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Head;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Pelvis;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform HandLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform HandRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform FootLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform FootRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform UpperArmLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform UpperArmRight;
	
public:
	void SetBoneDatas(USkeletalMeshComponent* skeletalMesh);
	//FMotionData CastMotionData();
	void ConvertToMotionJson(TSharedPtr<FJsonObject> motionData);
	void ConvertFromMotionJson(TSharedPtr<FJsonObject> motionData);
	FString ToString();
	FString ToString(const FTransform& transform);

};

USTRUCT(BlueprintType)
struct FReplayMotionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMotionTransformData> MotionDatas;
};

USTRUCT(BlueprintType)
struct FRecordObjectData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ObjectID = TEXT("");
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Visible = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform ActorTransform = FTransform();	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMotionTransformData CharacterMotionData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString NpcAnimFileName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bGunFired = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bGunFiredPlayOk = false;

	FRecordObjectData() {};

	void ConvertDataToJson(TSharedRef<FJsonObject> jsonObject, ARtObjectActor* actor);

};

USTRUCT(BlueprintType)
struct FReplayObjectDatas
{
	GENERATED_BODY()

public:
	FReplayObjectDatas() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRecordObjectData> RecordDatas;

};

USTRUCT(BlueprintType)
struct FPlayerDistanceInfo
{
	GENERATED_BODY()

public:
	FPlayerDistanceInfo() {};

	UPROPERTY()
	FVector PreviousLocation = FVector::ZeroVector;

	UPROPERTY()
	FVector CurrentLocation = FVector::ZeroVector;

	UPROPERTY()
	float TotalMovedDistance = 0;

	void Init(FVector currentLocation);
	void AccumulateDistance(FVector currentLocation);
};

DECLARE_DELEGATE(FReplayEnd);

UCLASS()
class RTGAME_API ARtReplayManagerActor : public AActor
{
	GENERATED_BODY()

public:
	enum ESaveMode
	{
		Save_None = 0,
		Server,
		LocalFile
	};

public:	
	// Sets default values for this actor's properties
	ARtReplayManagerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	float ReplayInterval = 0.1f;
	//bool IsLoop = false;
	ESaveMode RecordSaveMode = ESaveMode::LocalFile; //ESaveMode::LocalFile; // ESaveMode::Server;

	bool IsConsolePlay = false;
	float TestTime = 0.0f;
	
	UPROPERTY()
	TMap<int32, ARtReplayTrainee*> ReplayPawnMap; //int32 UID, ARtReplayTrainee* ReplayPawn

	UPROPERTY()
	TMap<int32, FReplayMotionData> ReplayMotionDataMap; //int32 UID, FReplayMotionData ReplayData

	float TotalReplayTime = -1.0f;
	float CurrentReplayTimeRate = -1.0f;
	
	UPROPERTY()
	TMap<ARtTraineePawn*, int32> TraineeUIDMap; // For Record Trainee Motion
	
	UPROPERTY()
	TArray<TWeakObjectPtr<ARtObjectActor>> RecordObjects;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString , FReplayObjectDatas> RecordDataMap; // ObjectId, RecordData
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ARtObjectActor* , FRecordObjectData> CurrentDataMap;
	
	FReplayEnd ReplayEndEvent;

	UPROPERTY()
	TMap<int32, FPlayerDistanceInfo> PlayerDistanceInfoMap;

private:
	UINT DataSize = 0;

public:
	void InitReplay();
	void StartRecord();
	void PlayReplay();
	void AllNpcAnimPlayOrStop(bool play);
	void PauseReplay();
	void SpawnReplayTrainee(int32 uid, FString role);

	bool Replaying(float playTime);

	void WriteRecordDataFile();
	void SendAllPlayerDistanceLog();

	void Record();
	void LoadRecordData(FString folder); // FolderName = TID(Temporary)

	void SaveMotionDatas(TSharedPtr<FJsonObject> jsonObject);

	bool ReplayingMotion(float playTime);
	FMotionTransformData LerpMotionTransformData(const FMotionTransformData& prevData, const FMotionTransformData& nextData, float alpha);

	void SaveObjectsDatas(TSharedPtr<FJsonObject> jsonObject);
	void SetEntitiesDatas(const FString& replayDatas); // 불러온 녹화 데이터를 사용할 수 있게 변환한다.
	void InitGunFiredFlag();
	void PostProcessEntities(); // NPC 등 오브젝트에 후처리

	bool ReplayingObjects(float playTime);

	void SetCurrentPlayTime(float currentPlayTimeRate);

protected:
	FTransform LerpTransform(FTransform a, FTransform b, float alpha);

	int32 RecordCount = 0;
	FTimerHandle RecordTimerHandle;

private:
	int32* TID = nullptr;
	
	// For Test
	void SetID(int32 tid, int32 uid);
	void PrintTime();
	void SetTestTime(float newTime);
	void SetSkeletalMeshAsset(ARtReplayTrainee* pawn, const FString& role);

	TArray<TSharedPtr<FJsonObject>> SaveEntityDatasForLocal;

	TMap<FString, FString> FbxFileToInternalAssetMap;

public:
	int32 GetTID();
	void LoadSctReplay(const struct FRtReplayStruct& replayInfo);

public:
	static void SetIntField(TSharedPtr<FJsonObject> jsonObj, const FString& key, const int value);
	static void SetIntField(TSharedPtr<FJsonObject> jsonObj, const FString& key, const double value);
	static void SetFloatField(TSharedPtr<FJsonObject> jsonObj, const FString& key, const float value);
	//static void SetStringField(TSharedPtr<FJsonObject> jsonObj, const FString& key, const float value);

public:
	static ARtReplayManagerActor* Get(UObject* worldContext);

};
