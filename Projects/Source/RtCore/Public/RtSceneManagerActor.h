// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RtFBXLoader.h"
#include "RtProcedualMeshActor.h"

//#include "RtActorData.h"
//#include "RtObjectActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RtSceneManagerActor.generated.h"


// [ FBX 씬매니저 클래스 ]
// 
// 실시간으로 FBX를 로드하는 기능.
// FBX를 포함하는 프로시절 매시 액토(RtProcedualMeshActor)를 씬에 배치하는 기능.
// 배치된 액토들을 관리하는 기능.

UCLASS()
class RTCORE_API ARtSceneManagerActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARtSceneManagerActor();
	~ARtSceneManagerActor();
	
// 실시간 Fbx 관련 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// {

	void LoadSubMesh(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pGlobalPosition, FbxPose* pPose, RtProMeshSection* pProMesh);
	void LoadMesh(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pGlobalPosition, FbxPose* pPose, RtFbxProcedualMeshInfo* pFbxMeshInfo);
	void LoadNode(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pParentGlobalPosition, FbxAMatrix& pGlobalPosition, FbxPose* pPose,
		RtFbxProcedualMeshInfo* pFbxMeshInfo);
	void LoadNodeRecursive(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pParentGlobalPosition, FbxPose* pPose,
		RtFbxProcedualMeshInfo* pFbxMeshInfo);

	static void UpdateSubMesh(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pGlobalPosition, FbxPose* pPose, RtProMeshSection* pProMesh);
	static void UpdateMesh(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pGlobalPosition, FbxPose* pPose, RtFbxProcedualMeshInfo* pFbxMeshInfo);
	static void UpdateNode(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pParentGlobalPosition, FbxAMatrix& pGlobalPosition, FbxPose* pPose,
		RtFbxProcedualMeshInfo* pFbxMeshInfo);
	static void UpdateNodeRecursive(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pParentGlobalPosition, FbxPose* pPose, RtFbxProcedualMeshInfo* pFbxMeshInfo);

	bool AddFbxFile(FString filePath);
	bool UpdateModelData(FString modelKey);
	ARtProcedualMeshActor* FbxSpawnActor(FString modelKey, float x, float y, float z, float pitch = 0, float yaw = 90, float roll = -90);
	ARtProcedualMeshActor* FbxGetSpawnActor(unsigned int index)
	{
		if (index < (unsigned int)mFbxActorArray.Num())
		{
			return mFbxActorArray[index];
		}

		return NULL;
	}

	bool FbxRemoveSpawnActor(ARtProcedualMeshActor* pActor)
	{
		UWorld* pWorld = GetWorld();
		if (pWorld)
		{
			pWorld->RemoveActor(pActor, false);
			return true;
		}

		return false;
	}

	bool FbxRemoveSpawnActor(unsigned int index)
	{
		auto actor = FbxGetSpawnActor(index);
		if (actor)
		{
			UWorld* pWorld = GetWorld();
			pWorld->RemoveActor(actor, false);

			mFbxActorArray.RemoveAt(index);

			return true;
		}

		return false;
	}

	void LoadFbxFile(FString filePath);

	RtFbxProcedualMeshInfo* FindFbxInfo(FString key)
	{
		if (!mFbxInfoMap.IsEmpty())
		{
			TSharedPtr<RtFbxProcedualMeshInfo>* spData = mFbxInfoMap.Find(key);
			if (spData != NULL)
			{
				return spData->Get();
			}
		}

		return NULL;
	}

// }
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	bool LoadJson();
	bool SaveJson();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	//Document mTCTFileDoc; // 현재 로드되었거나 새로 만들어진 TCT JSON 파일.

	// 참고 : [Unreal Engine 4] 메모리 관리 - https ://openmynotepad.tistory.com/89

	//TSharedPtr<RtFBXLoader> mpFbxLoader = NULL; // 테스트 로더.
	ARtProcedualMeshActor* mpProcedualMeshActor = NULL; // 테스트 액토.
	TArray<ARtProcedualMeshActor*> mFbxActorArray; // 생성된 Fbx 액토들을 관리한다.
	TMap<FString, TSharedPtr<RtFbxProcedualMeshInfo>> mFbxInfoMap; // [Fbx 파일명, 프로시절 매시 정보] : 모든 fbx 파일들의 프로시절 데이터를 담는다.

	//TArray<ARtObjectActor*> mActorArray; // 생성된 기본 액토들을 관리한다.
	//TMap<FString, TSharedPtr<RtActorData>> mActorInfoMap; // [액토 모델 이름, 액토 모델 데이터] : 생성할 모델들을 관리한다.
};
