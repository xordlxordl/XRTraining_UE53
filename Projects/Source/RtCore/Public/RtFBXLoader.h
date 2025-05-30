// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "fbxsdk.h"

#include "ProceduralMeshComponent.h"
#include "RtTextureUtil.h"
#include "RtFbxProcedualMeshInfo.h"

#include "CoreMinimal.h"

/**
 * Fbx 로드 유틸리티
 * Fbx 바이너리 파일 로드, 프로시절 메시를 만들수 있는 버퍼에 담아 리턴.
 * 해당 버퍼를 모델 데이터가 가지고 있다가 복제가 일어날 때 프로시절 메시를 만들어주는 개념.
 */
class RTCORE_API RtFBXLoader //: public UGameInstanceSubsystem
{
public:
	RtFBXLoader();
	~RtFBXLoader();

public:
	static void InitSubSystem(UGameInstance* pInstance);
	static RtFBXLoader* Get() { return mInstance; }
protected:
	static RtFBXLoader* mInstance;

public:
	bool Init();

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

	FbxTime & GetCurrentTime() { return mCurrentTime; }

	//FbxAnimLayer* GetCurrentAnimLayer() { return mpCurrentAnimLayer; }

	FbxArray<FbxString*> mAnimStackNameArray;

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
