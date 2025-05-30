// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RtFBXLoader.h"
#include "RtProcedualMeshActor.h"

//#include "RtActorData.h"
//#include "RtObjectActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RtSceneManagerActor.generated.h"


// [ FBX ���Ŵ��� Ŭ���� ]
// 
// �ǽð����� FBX�� �ε��ϴ� ���.
// FBX�� �����ϴ� ���ν��� �Ž� ����(RtProcedualMeshActor)�� ���� ��ġ�ϴ� ���.
// ��ġ�� ������� �����ϴ� ���.

UCLASS()
class RTCORE_API ARtSceneManagerActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARtSceneManagerActor();
	~ARtSceneManagerActor();
	
// �ǽð� Fbx ���� ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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

	//Document mTCTFileDoc; // ���� �ε�Ǿ��ų� ���� ������� TCT JSON ����.

	// ���� : [Unreal Engine 4] �޸� ���� - https ://openmynotepad.tistory.com/89

	//TSharedPtr<RtFBXLoader> mpFbxLoader = NULL; // �׽�Ʈ �δ�.
	ARtProcedualMeshActor* mpProcedualMeshActor = NULL; // �׽�Ʈ ����.
	TArray<ARtProcedualMeshActor*> mFbxActorArray; // ������ Fbx ������� �����Ѵ�.
	TMap<FString, TSharedPtr<RtFbxProcedualMeshInfo>> mFbxInfoMap; // [Fbx ���ϸ�, ���ν��� �Ž� ����] : ��� fbx ���ϵ��� ���ν��� �����͸� ��´�.

	//TArray<ARtObjectActor*> mActorArray; // ������ �⺻ ������� �����Ѵ�.
	//TMap<FString, TSharedPtr<RtActorData>> mActorInfoMap; // [���� �� �̸�, ���� �� ������] : ������ �𵨵��� �����Ѵ�.
};
