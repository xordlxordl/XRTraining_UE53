// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProceduralMeshComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "RtFBXLoader.h"

#include "RtProcedualMeshActor.generated.h"

UTexture2D* LoadTexture2D_FromFile(const FString& FullFilePath, bool& IsValid, int32& Width, int32& Height);

// 섹션별로 메시를 로드
UCLASS()
class RTCORE_API ARtProcedualMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARtProcedualMeshActor();

	int mArrayIndex = -1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	RtProMeshSection * GetProcedualMeshSection(unsigned int sectionIndex) 
	{
		if (sectionIndex < (unsigned int)mProcedualArray.Num())
		{
			return &mProcedualArray[sectionIndex];
		}

		return NULL;
	}

	RtProMeshSection* AddProcedualMeshSection()
	{
		return &mProcedualArray.Add_GetRef(RtProMeshSection());
	}

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CreateTriangle(FVector A, FVector B, FVector C);
	void InitProcedualArray();
	//UTexture2D* LoadTexture2D_FromFile(const FString& FullFilePath, bool& IsValid, int32& Width, int32& Height);
	void CreateMeshs_Test(bool enableCollision);
	void CreateMeshs(RtFbxProcedualMeshInfo* pFbxMeshInfo, bool enableCollision);

	void UpdateMeshs(RtFbxProcedualMeshInfo* pFbxMeshInfo);

	bool PlayAnimation(int aniIndex);
	RtFbxProcedualMeshInfo* GetProMeshInfo() { return mpFbxMeshInfo; }

private:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* mpProMeshComp; // FBX 프로시절 메시 콤포넌트 데이터.

	TArray<RtProMeshSection> mProcedualArray; // FBX 내 메시 Array.

	RtFbxProcedualMeshInfo* mpFbxMeshInfo = NULL;

	FString mFbxModelKey; 

	FbxAnimLayer* mpCurrentAnimLayer = NULL;

	FbxTime mFrameTime, mStart, mStop, mCurrentTime;
	FbxTime mCache_Start, mCache_Stop;

	bool mLoop = true;
	bool mPause = false;

	float mDeltaACC = 0;

	bool mAnimEnable = false; // 에니메이션은 실험단계이므로 기본은 폴스.
};
