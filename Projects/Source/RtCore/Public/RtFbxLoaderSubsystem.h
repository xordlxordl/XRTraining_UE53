// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "fbxsdk.h"

#include "RtFbxProcedualMeshInfo.h"

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RtFbxLoaderSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class RTCORE_API URtFbxLoaderSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	// 다음 기능은 서브 시스템을 cpp 코드에서도 쉽게 사용하기 위해 만든 것임!
	// 기본적으로 아래와 같은 방법으로도 접근이 가능함
	//	UGameInstance* GameInstance = GetGameInstance();
	//  UMyGameSubsystem* MySubsystem = GameInstance->GetSubsystem<UMyGameSubsystem>();
public:
	static void InitSubSystem(UGameInstance* pGameInstance);
	static URtFbxLoaderSubsystem* Get() { return mpInstance; }
protected:
	static URtFbxLoaderSubsystem* mpInstance;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	bool Init();


	TSet<FbxFileTexture*> _GetFbxMaterialTextures(const FbxSurfaceMaterial& Material);
	void _LoadMaterialRecursive(FbxNode* pNode, FbxAnimLayer* pAnimLayer, RtFbxProcedualMeshInfo* pFbxProMeshInfo);
	void _UnloadCacheRecursive(FbxNode* pNode);
	void _UnloadCacheRecursive(FbxScene* pScene);

	void LoadSubMesh(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pGlobalPosition, FbxPose* pPose, RtProMeshSection* pProMesh);
	void LoadMesh(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pGlobalPosition, FbxPose* pPose, RtFbxProcedualMeshInfo* pFbxMeshInfo); // RtProMeshSection* pProMesh)
	void LoadNode(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pParentGlobalPosition, FbxAMatrix& pGlobalPosition, FbxPose* pPose, RtFbxProcedualMeshInfo* pFbxMeshInfo);
	void LoadNodeRecursive(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pParentGlobalPosition, FbxPose* pPose, RtFbxProcedualMeshInfo* pFbxMeshInfo);

	TSharedPtr<RtFbxProcedualMeshInfo> LoadFbx(const FString& fbxFilePath);

	void InitializeSdkObjects();
	void DestroySdkObjects();
	bool LoadScene(const char* pFilename);

	bool SetCurrentAnimStack(int pIndex);

	FbxScene* GetScene() const { return mpScene; }

	void Tick(float DeltaTime);

	FbxTime& GetCurrentTime() { return mCurrentTime; }

	//FbxAnimLayer* GetCurrentAnimLayer() { return mpCurrentAnimLayer; }

	FbxArray<FbxString*> mAnimStackNameArray;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = RtFbxLoaderSubsystem)
	//int test;

private:
	FbxManager* mpSdkManager;
	FbxScene* mpScene;
	FbxImporter* mpImporter;

	//	FbxAnimLayer* mpCurrentAnimLayer;

	FbxTime mFrameTime, mStart, mStop, mCurrentTime;
	FbxTime mCache_Start, mCache_Stop;

	bool mLoop = true;
	bool mPause = false;
};
