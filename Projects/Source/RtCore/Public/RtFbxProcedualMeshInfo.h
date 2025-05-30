// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "fbxsdk.h"

#include "ProceduralMeshComponent.h"
#include "RtTextureUtil.h"

#include "CoreMinimal.h"

// ���ν��� �Ž� ��ο쿡 �ʿ��� ������.
// FBX �� �Ž� ����. ���ο� ���� �Žð� ���� �� �ִ�.
class RtProMeshSection
{
public:
	FString mMaterialName; // �Žÿ��� ����� ��Ƽ���� �̸�.

	TArray<FVector> mVertices;
	TArray<int32> mTriangles;
	TArray<FVector> mNormals;
	TArray<FVector2D> mUV;
	TArray<FProcMeshTangent> mTangents; // ���� ����
	TArray<FLinearColor> mVertexColors; // 

	int mIndexCounter = 0;
	int mSectionID = 0;

	int mSubMeshIndexOffset = 0;
	int mSubMeshIndexOffsetEnd = 0;

	void Init()
	{
		mVertices.Empty();
		mTriangles.Empty();
		mNormals.Empty();
		mUV.Empty();
		mTangents.Empty();
		mVertexColors.Empty();

		mIndexCounter = 0;
		mSectionID = 0;

		//mModelName = "";
		mMaterialName = "";

		//mSubProMeshSectArray.Empty();
		mSubMeshIndexOffset = 0;
		mSubMeshIndexOffsetEnd = 0;
	}
};

// ��Ƽ���� ����.
class RtMaterialInfo
{
public:
	FString mDiffuseFilePath;
	FString mNormalFilePath;
	FString mSpecularFilePath;
	FString mShininessExponentFilePath;
	FString mAlphaFilePath;

	UMaterialInstanceDynamic* mpMatInsDyna = NULL;

	bool CreateMaterial()//FString diffuseFilePath, FString normalFilePath, FString specularFilePath)
	{
		if (mDiffuseFilePath.IsEmpty())
			return false;

		FString BaseMaterialAddress = "Material'/Game/Materials/MaterialInstanceDynamic_Base.MaterialInstanceDynamic_Base'";
		UMaterial* pMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, *BaseMaterialAddress, nullptr, LOAD_None, nullptr));

		//mpProMeshComp->SetMaterial(0, material);
		//UMaterial* material2 = CreateDefaultSubobject<UMaterial>(TEXT("Material2"));

		mpMatInsDyna = UMaterialInstanceDynamic::Create(pMaterial, NULL);

		bool isValid = false;
		int32 width = 0;
		int32 height = 0;

		UTexture2D* pDiffTexture = URtTextureUtil::LoadTexture2D_FromFile(mDiffuseFilePath, isValid, width, height);
		UTexture2D* pNormalTexture = URtTextureUtil::LoadTexture2D_FromFile(mNormalFilePath, isValid, width, height);
		UTexture2D* pSpecTexture = URtTextureUtil::LoadTexture2D_FromFile(mSpecularFilePath, isValid, width, height);
		UTexture2D* pRoughnessTexture = URtTextureUtil::LoadTexture2D_FromFile(mShininessExponentFilePath, isValid, width, height);
		UTexture2D* pAlphaTexture = URtTextureUtil::LoadTexture2D_FromFile(mAlphaFilePath, isValid, width, height);
				
//		if (pDiffTexture != NULL)
			mpMatInsDyna->SetTextureParameterValue(TEXT("BaseTexture"), pDiffTexture);

//		if (pNormalTexture != NULL)
			mpMatInsDyna->SetTextureParameterValue(TEXT("NormalTexture"), pNormalTexture);

//		if (pSpecTexture != NULL)
			mpMatInsDyna->SetTextureParameterValue(TEXT("SpecularTexture"), pSpecTexture);

		//if (pRoughnessTexture != NULL)
			mpMatInsDyna->SetTextureParameterValue(TEXT("RoughnessTexture"), pRoughnessTexture);

			mpMatInsDyna->SetTextureParameterValue(TEXT("AlphaTexture"), pAlphaTexture);

		return true;
	}
};

// Fbx ���ν��� �Ž� ����. 
class RtFbxProcedualMeshInfo
{
public:
	FString mModelFileName; // Fbx ���ϸ�.

	// [Key:�Ž� �̸�, Data:TArray<TSharedPtr<RtProMeshSection>>] : �޽� �̸� ������ ���ν��� ����(���� �Ž� ���� ����)�� ��´�.
	TMap<FString, TArray<TSharedPtr<RtProMeshSection>>> mProcedualMap;

	// [Key:��Ƽ���� �̸�, Data:RtMaterialInfo*] : ��Ƽ���� �̸� ���� ��Ƽ���� ������ ��´�.
	TMap<FString, TSharedPtr<RtMaterialInfo>> mMaterialMap;

	//TSharedPtr<RtFBXLoader> mpFbxLoader;
	//void Init()
	//{
	//	if (mpFbxLoader == NULL)
	//	{
	//		mpFbxLoader = MakeShared<RtFBXLoader>();
	//		mpFbxLoader->Init();
	//	}
	//}
	//RtFBXLoader* GetFbxLoader() { return mpFbxLoader.Get(); }

	TArray<TSharedPtr<RtProMeshSection>>* FindMeshArray(FString key)
	{
		TArray<TSharedPtr<RtProMeshSection>>* proMeshArr = mProcedualMap.Find(key);
		if (proMeshArr != NULL)
		{
			return proMeshArr;
		}

		return NULL;
	}

	RtProMeshSection* AddProcedualMeshSection(FString key, bool addArray = false)
	{
		TArray<TSharedPtr<RtProMeshSection>>* proMeshArr = mProcedualMap.Find(key);
		if (proMeshArr == NULL)
		{
			TArray<TSharedPtr<RtProMeshSection>>& value = mProcedualMap.Add(key, TArray<TSharedPtr<RtProMeshSection>>());
			TSharedPtr<RtProMeshSection> sect = MakeShared<RtProMeshSection>();
			value.Add(sect);

			return sect.Get();
		}
		else
		{
			if (addArray)
			{
				TSharedPtr<RtProMeshSection> sect = MakeShared<RtProMeshSection>();
				(*proMeshArr).Add(sect);
				return sect.Get();
			}

			return (*proMeshArr)[0].Get();
		}
	}

	RtProMeshSection* FindProcedualMeshSection(FString key, int subMeshIndex = 0)
	{
		if (!mProcedualMap.IsEmpty())
		{
			TArray<TSharedPtr<RtProMeshSection>>* proMeshArr = mProcedualMap.Find(key);
			if (proMeshArr != NULL)
			{
				if ((*proMeshArr).Num() > 0)
				{
					if (subMeshIndex < (*proMeshArr).Num())
						return (*proMeshArr)[subMeshIndex].Get();
					else
						return (*proMeshArr)[0].Get();
				}
			}
		}

		return NULL;
	}

	RtMaterialInfo* AddMaterialInfo(FString key)
	{
		TSharedPtr<RtMaterialInfo>* ppMatInfo = mMaterialMap.Find(key);
		if (ppMatInfo == NULL)
		{
			return mMaterialMap.Add(key, MakeShared<RtMaterialInfo>()).Get();
		}

		return NULL;
	}

	RtMaterialInfo* FindMaterialInfo(FString key)
	{
		if (!mMaterialMap.IsEmpty())
		{
			TSharedPtr<RtMaterialInfo>* ppMatInfo = mMaterialMap.Find(key);
			if (ppMatInfo != NULL)
			{
				return ppMatInfo->Get();
			}
		}

		return NULL;
	}
};
