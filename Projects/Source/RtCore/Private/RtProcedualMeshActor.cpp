// Fill out your copyright notice in the Description page of Project Settings.


#include "RtProcedualMeshActor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/TextureRenderTarget2D.h"

#include "Runtime/ImageWrapper/Public/IImageWrapper.h"
#include "Runtime/ImageWrapper/Public/IImageWrapperModule.h"
#include "Runtime/Engine/Public/TextureResource.h"
#include "Runtime/Engine/Public/HighResScreenshot.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"

#include "RtSceneManagerActor.h"

//#include "Runtime/CoreUObject/Public/Serialization/BulkData.h"


// Sets default values
ARtProcedualMeshActor::ARtProcedualMeshActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mpProMeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = mpProMeshComp;
	mpProMeshComp->bUseAsyncCooking = true;

	mpFbxMeshInfo = NULL;

//	mProcedualArray.Add(RtProcedualMeshSection());
}

// Called when the game starts or when spawned
void ARtProcedualMeshActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// 프로시절 매시 테스트 함수.
void ARtProcedualMeshActor::CreateTriangle(FVector A, FVector B, FVector C)
{
	TArray<FVector> vertices;
	vertices.Add(A);
	vertices.Add(B);
	vertices.Add(C);

	TArray<int32> Triangles;
	Triangles.Add(0);
	Triangles.Add(1);
	Triangles.Add(2);

	TArray<FVector> normals; // 법선 백터.
	normals.Add(FVector(0, 0, 1));
	normals.Add(FVector(0, 0, 1));
	normals.Add(FVector(0, 0, 1));

	TArray<FVector2D> UV0;
	UV0.Add(FVector2D(0, 0));
	UV0.Add(FVector2D(1, 0));
	UV0.Add(FVector2D(0, 1));

	TArray<FProcMeshTangent> tangents; // 접선 백터
	//tangents.Add(FProcMeshTangent(0, 1, 0));
	//tangents.Add(FProcMeshTangent(0, 1, 0));
	//tangents.Add(FProcMeshTangent(0, 1, 0));

	TArray<FLinearColor> vertexColors;
	//vertexColors.Add(FLinearColor(1.0, 1.0, 1.0, 1.0));
	//vertexColors.Add(FLinearColor(1.0, 1.0, 1.0, 1.0));
	//vertexColors.Add(FLinearColor(1.0, 1.0, 1.0, 1.0));

	mpProMeshComp->CreateMeshSection_LinearColor(1, vertices, Triangles, normals, UV0, vertexColors, tangents, false);

	// Enable collision data
	mpProMeshComp->ContainsPhysicsTriMeshData(true);

	FString MaterialAddress = "Material'/Game/Characters/FBX_Character/Remy/Bottommat.Bottommat'";
	UMaterial* material;
	material = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, *MaterialAddress, nullptr, LOAD_None, nullptr));

	mpProMeshComp->SetMaterial(1, material);

}

void ARtProcedualMeshActor::InitProcedualArray()
{
	//mProcedualArray.RemoveAll([](int32 Val) {
	//	return true;
	//	});
//
//	//for (RtProcedualMeshSection& proMesh : mProcedualArray)
//	//{
//	//	proMesh.Init();
//	//}
}

UTexture2D* LoadTexture2D_FromFile(const FString& FullFilePath, bool& IsValid, int32& Width, int32& Height)
{
	IsValid = false;

	UTexture2D* LoadedT2D = NULL;

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));

	EImageFormat imageFormat = EImageFormat::PNG;

	//TArray<FString> ParseArray = {};
	//FullFilePath.ParseIntoArray(ParseArray, TEXT("."));
	//if (ParseArray.Num() > 1)
	//{
	//	FString ext = ParseArray[ParseArray.Num() - 1];
	//	if (ext == FString("tga"))
	//	{
	//		imageFormat = EImageFormat::TGA;
	//	}
	//}

	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(imageFormat);

	TArray<uint8> RawFileData;

	if (!FFileHelper::LoadFileToArray(RawFileData, *FullFilePath))
	{
		return NULL;
	}

	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
	{
//		const TArray<uint8> * UncompressedBRGA = NULL;
		TArray<uint8> UncompressedBRGA;

		if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBRGA))
		{
			LoadedT2D = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);

			if (!LoadedT2D)
			{
				return NULL;
			}

			Width = ImageWrapper->GetWidth();
			Height = ImageWrapper->GetHeight();

			void* TextureData = LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(TextureData, UncompressedBRGA.GetData(), UncompressedBRGA.Num());
			LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();

			LoadedT2D->UpdateResource();
		}
	}

	IsValid = true;

	return LoadedT2D;
}

// 메시 제작 테스트 함수.
void ARtProcedualMeshActor::CreateMeshs_Test(bool enableCollision)
{
	if (mpProMeshComp != NULL)
	{
		FString MaterialAddress = "Material'/Game/Materials/MaterialInstanceDynamic_Base.MaterialInstanceDynamic_Base'";

		UMaterial* material;
		material = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, *MaterialAddress, nullptr, LOAD_None, nullptr));

		//mpProMeshComp->SetMaterial(0, material);
		//UMaterial* material2 = CreateDefaultSubobject<UMaterial>(TEXT("Material2"));

		UMaterialInstanceDynamic* fbxMID = UMaterialInstanceDynamic::Create(material, mpProMeshComp);
		//ExampleStaticMesh->SetMaterial(ExampleMID, 0);
		//FLinearColor Red(FLineaerColor::Red);
		//ExampleMID->SetVectorParameter("ParameterName", Red);
		//	UTextureRenderTarget2D* pTexture = NewObject<UTextureRenderTarget2D>(this);

		FString diffuseFilePath = "C:/work/Dev/RND/unreal_total_test/character_texture/maria_diffuse.png";
		FString normalFilePath = "C:/work/Dev/RND/unreal_total_test/character_texture/maria_normal.png";
		FString specularFilePath = "C:/work/Dev/RND/unreal_total_test/character_texture/maria_specular.png";
		bool isValid = false;
		int32 width = 0;
		int32 height = 0;

		UTexture2D* pDiffTexture = LoadTexture2D_FromFile(diffuseFilePath, isValid, width, height);
		UTexture2D* pNormalTexture = LoadTexture2D_FromFile(normalFilePath, isValid, width, height);
		UTexture2D* pSpecTexture = LoadTexture2D_FromFile(specularFilePath, isValid, width, height);

		fbxMID->SetTextureParameterValue(TEXT("BaseTexture"), pDiffTexture);
		fbxMID->SetTextureParameterValue(TEXT("NormalTexture"), pNormalTexture);
		fbxMID->SetTextureParameterValue(TEXT("SpecularTexture"), pSpecTexture);

		mpProMeshComp->SetMaterial(0, fbxMID);

		//mpProMeshComp->CreateDynamicMaterialInstance(0);

		if (0 < mProcedualArray.Num())
		{
			RtProMeshSection * pProMesh = &mProcedualArray[0];
			mpProMeshComp->CreateMeshSection_LinearColor(0,
				pProMesh->mVertices,
				pProMesh->mTriangles,
				pProMesh->mNormals,
				pProMesh->mUV,
				pProMesh->mVertexColors,
				pProMesh->mTangents,
				enableCollision);

			// Enable collision data
			mpProMeshComp->ContainsPhysicsTriMeshData(enableCollision);

			//ConstructorHelpers::FObjectFinder<UMaterialInterface> 
			//	tempMaterial(TEXT("Material'/Game/Characters/FBX_Character/MariaMat.MariaMat'"));
		}
	}
}

// CreateMeshs : 
// 프로시절 정보를 바탕으로 프로시절 메시를 제작 한다.
// 머티리얼 정보를 바탕으로 머티리얼을 설정한다.
void ARtProcedualMeshActor::CreateMeshs(RtFbxProcedualMeshInfo* pFbxMeshInfo, bool enableCollision)
{
	if (mpProMeshComp != NULL && pFbxMeshInfo != NULL)
	{
		if (0 < pFbxMeshInfo->mProcedualMap.Num())
		{
			int sect = 0;
			for (auto & Elem : pFbxMeshInfo->mProcedualMap)
			{
				auto & pData = Elem.Value;

				for (int i = 0; i < pData.Num(); i++)
				{
					pData[i]->mSectionID = sect;

					RtMaterialInfo* pMatInfo = pFbxMeshInfo->FindMaterialInfo(pData[i]->mMaterialName);
					if (pMatInfo != NULL)
					{
						mpProMeshComp->SetMaterial(pData[i]->mSectionID, pMatInfo->mpMatInsDyna);
					}

					mpProMeshComp->CreateMeshSection_LinearColor(pData[i]->mSectionID,
						pData[i]->mVertices,
						pData[i]->mTriangles,
						pData[i]->mNormals,
						pData[i]->mUV,
						pData[i]->mVertexColors,
						pData[i]->mTangents,
						enableCollision);

					sect++;
				}
			}
			
			// Enable collision data
			mpProMeshComp->ContainsPhysicsTriMeshData(enableCollision);

			mpFbxMeshInfo = pFbxMeshInfo;
		}
	}
}

void ARtProcedualMeshActor::UpdateMeshs(RtFbxProcedualMeshInfo* pFbxMeshInfo)
{
	if (mpProMeshComp != NULL && pFbxMeshInfo != NULL)
	{
		if (0 < pFbxMeshInfo->mProcedualMap.Num())
		{
			for (auto & Elem : pFbxMeshInfo->mProcedualMap)
			{
				auto & pData = Elem.Value;

				for (int i = 0; i < pData.Num(); i++)
				{
					mpProMeshComp->UpdateMeshSection_LinearColor(pData[i]->mSectionID,
						pData[i]->mVertices,
						pData[i]->mNormals,
						pData[i]->mUV,
						pData[i]->mVertexColors,
						pData[i]->mTangents);
				}
			}
		}
	}
}

bool ARtProcedualMeshActor::PlayAnimation(int aniIndex)
{
	if (mpFbxMeshInfo!=NULL)
	{
		RtFBXLoader* pFbxLoader = RtFBXLoader::Get();// mpFbxMeshInfo->GetFbxLoader();
		if (pFbxLoader)
		{
			const int lAnimStackCount = pFbxLoader->mAnimStackNameArray.GetCount();
			if (!lAnimStackCount || aniIndex >= lAnimStackCount)
			{
				return false;
			}

			FbxScene* pScene = pFbxLoader->GetScene();

			// select the base layer from the animation stack
			FbxAnimStack* lCurrentAnimationStack = pScene->FindMember<FbxAnimStack>(pFbxLoader->mAnimStackNameArray[aniIndex]->Buffer());
			if (lCurrentAnimationStack == NULL)
			{
				// this is a problem. The anim stack should be found in the scene!
				return false;
			}

			// we assume that the first animation layer connected to the animation stack is the base layer
			// (this is the assumption made in the FBXSDK)
			mpCurrentAnimLayer = lCurrentAnimationStack->GetMember<FbxAnimLayer>();

			pScene->SetCurrentAnimationStack(lCurrentAnimationStack);

			FbxTakeInfo* lCurrentTakeInfo = pScene->GetTakeInfo(*(pFbxLoader->mAnimStackNameArray[aniIndex]));
			if (lCurrentTakeInfo)
			{
				mStart = lCurrentTakeInfo->mLocalTimeSpan.GetStart();
				mStop = lCurrentTakeInfo->mLocalTimeSpan.GetStop();
			}
			else
			{
				// Take the time line value
				FbxTimeSpan lTimeLineTimeSpan;
				pScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(lTimeLineTimeSpan);

				mStart = lTimeLineTimeSpan.GetStart();
				mStop = lTimeLineTimeSpan.GetStop();
			}

			// check for smallest start with cache start
			if (mCache_Start < mStart)
				mStart = mCache_Start;

			// check for biggest stop with cache stop
			if (mCache_Stop > mStop)
				mStop = mCache_Stop;

			// move to beginning
			mCurrentTime = mStart;

			// Set the scene status flag to refresh 
			// the scene in the next timer callback.
			//mStatus = MUST_BE_REFRESHED;
			
			mDeltaACC = 0;
			mAnimEnable = true;

			return true;//pFbxLoader->SetCurrentAnimStack(aniIndex);
		}
	}

	return false;
}

// Called every frame
void ARtProcedualMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!mAnimEnable)
	{
		return;
	}

	mDeltaACC += DeltaTime;

	if (mStop > mStart && !mPause)
	{
		// Set the scene status flag to refresh 
		// the scene in the next timer callback.
// 		mStatus = MUST_BE_REFRESHED;

		//mFrameTime.SetSecondDouble(CAppTimer::GetInstance().GetDeltaTime());
		mFrameTime.SetSecondDouble(DeltaTime);

		mCurrentTime += mFrameTime;

		if (mCurrentTime > mStop)
		{
			if (mLoop)
			{
				mCurrentTime = mStart;
			}
			else
			{
				mCurrentTime = 0;
				mPause = true;
			}
		}
	}
	// Avoid displaying the same frame on 
	// and on if the animation stack has no length.
	else
	{
		// Set the scene status flag to avoid refreshing 
		// the scene in the next timer callback.
// 		mStatus = REFRESHED;
	}

	if (mDeltaACC < 0.1f)
	{
		return;
	}
	
	if (mpFbxMeshInfo)
	{
		auto pFbxLoader = RtFBXLoader::Get(); //mpFbxMeshInfo->GetFbxLoader();
		if (pFbxLoader)
		{
			//			pFbxLoader->Tick(DeltaTime);


			auto pScene = pFbxLoader->GetScene();
			if (pScene && mpCurrentAnimLayer)
			{
				FbxPose* lPose = NULL;
				FbxAMatrix lDummyGlobalPosition;

				ARtSceneManagerActor::UpdateNodeRecursive(pScene->GetRootNode(), mCurrentTime,
					mpCurrentAnimLayer, lDummyGlobalPosition, lPose, mpFbxMeshInfo);

				UpdateMeshs(mpFbxMeshInfo);
			}
		}
	}

	mDeltaACC = 0;
	
}

