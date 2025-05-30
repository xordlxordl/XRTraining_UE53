// Fill out your copyright notice in the Description page of Project Settings.
#include "RtSceneManagerActor.h"

#include "../RtCore.h"

#include "../Fbx/GetPosition.h"
#include "../Fbx/DrawSceneUtil.h"

//using namespace std;

//#include <format>

// Sets default values
ARtSceneManagerActor::ARtSceneManagerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//if (mpFbxLoader == NULL)
	//{
	//	mpFbxLoader = MakeShared<RtFBXLoader>();
	//	mpFbxLoader->Init();
	//}
}

ARtSceneManagerActor::~ARtSceneManagerActor()
{
	//if (mpFbxLoader)
	//{
	//	delete mpFbxLoader;
	//}
}

void ARtSceneManagerActor::LoadSubMesh(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pGlobalPosition, FbxPose* pPose,
	RtProMeshSection* pProMesh)
{
	FbxMesh* lMesh = pNode->GetMesh();
	const int lVertexCount = lMesh->GetControlPointsCount();
	if (lVertexCount == 0)
	{
		return;
	}

	//const VBOMesh* lMeshCache = static_cast<const VBOMesh*>(lMesh->GetUserDataPtr());
// If it has some defomer connection, update the vertices position
	const bool lHasVertexCache = lMesh->GetDeformerCount(FbxDeformer::eVertexCache) &&
		(static_cast<FbxVertexCacheDeformer*>(lMesh->GetDeformer(0, FbxDeformer::eVertexCache)))->Active.Get();
	const bool lHasShape = lMesh->GetShapeCount() > 0;
	const bool lHasSkin = lMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
	const bool lHasDeformation = lHasVertexCache || lHasShape || lHasSkin;

	FbxVector4* lVertexArray = NULL;
// 	FbxVector4* lVertexArrayT = NULL;

	//if (!lMeshCache || lHasDeformation)
	lVertexArray = new FbxVector4[lVertexCount];
// 	lVertexArrayT = new FbxVector4[lVertexCount];

	memcpy(lVertexArray, lMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));

	if (lHasDeformation)
	{
		// Active vertex cache deformer will overwrite any other deformer
		if (lHasVertexCache)
		{
			//ReadVertexCacheData(lMesh, pTime, lVertexArray);
		}
		else
		{
			if (lHasShape)
			{
				// Deform the vertex array with the shapes.
				ComputeShapeDeformation(lMesh, pTime, pAnimLayer, lVertexArray);
			}

			//we need to get the number of clusters
			const int lSkinCount = lMesh->GetDeformerCount(FbxDeformer::eSkin);
			int lClusterCount = 0;
			for (int lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex)
			{
				lClusterCount += ((FbxSkin*)(lMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin)))->GetClusterCount();
			}

			if (lClusterCount)
			{
				// Deform the vertex array with the skin deformer.
				ComputeSkinDeformation(pGlobalPosition, lMesh, pTime, lVertexArray, pPose);

				//ApplyWorldTransform(pGlobalPosition); // DrawMesh animation
			}
		}

		//if (lMeshCache)
		//	lMeshCache->UpdateVertexPosition(lMesh, lVertexArray);
	}

	//float scale = 1.0f;
	for (int i = 0; i < lVertexCount; i++)
	{
		lVertexArray[i] = pGlobalPosition.MultT(lVertexArray[i]);
	}

	//glPushMatrix();
	//glMultMatrixd((const double*)pGlobalPosition);

	//// 알파 블랜드 처리 시작.
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//if (lMeshCache)
	//{
	//	lMeshCache->BeginDraw(pShadingMode);
	//	const int lSubMeshCount = lMeshCache->GetSubMeshCount();
	//	for (int lIndex = 0; lIndex < lSubMeshCount; ++lIndex)
	//	{
	//		if (pShadingMode == SHADING_MODE_SHADED)
	//		{
	//			const FbxSurfaceMaterial* lMaterial = pNode->GetMaterial(lIndex);
	//			if (lMaterial)
	//			{
	//				const MaterialCache* lMaterialCache = static_cast<const MaterialCache*>(lMaterial->GetUserDataPtr());
	//				if (lMaterialCache)
	//				{
	//					lMaterialCache->SetCurrentMaterial();
	//				}
	//			}
	//			else
	//			{
	//				// Draw green for faces without material
	//				MaterialCache::SetDefaultMaterial();
	//			}
	//		}

	//		lMeshCache->Draw(lIndex, pShadingMode);
	//	}
	//	lMeshCache->EndDraw();
	//}
	//else
	//{
	//	// OpenGL driver is too lower and use Immediate Mode
//		glColor4f(0.9f, 0.9f, 0.1f, 1.0f); // xrdev mod.. 큐브 색상 변경.

	int lPolygonCount = lMesh->GetPolygonCount();
	bool isUnmaped;

	//	APP_LOG("LoadMesh~ poly count %d\n", lPolygonCount);

	FbxStringList lUVNames;
	lMesh->GetUVSetNames(lUVNames);
	const char* lUVName = NULL;
	if (lUVNames.GetCount())
	{
		lUVName = lUVNames[0];
	}

	if (pProMesh != NULL)
	{
		FbxVector4 normalList[] = { FbxVector4(0,0,0),FbxVector4(0,0,0),FbxVector4(0,0,0),FbxVector4(0,0,0) };
		FbxVector2 uvList[] = { FbxVector2(0,0),FbxVector2(0,0),FbxVector2(0,0),FbxVector2(0,0) };
		int indexList[] = { 0,0,0,0 };

		for (int lPolygonIndex = pProMesh->mSubMeshIndexOffset; lPolygonIndex < pProMesh->mSubMeshIndexOffsetEnd; lPolygonIndex++)
		{
			int lVerticeCount = lMesh->GetPolygonSize(lPolygonIndex);
			//			PRINT_LOG(TEXT("poly4 index : %d vert count : %d\n"), lPolygonIndex, lVerticeCount);

			for (int lVerticeIndex = 0; lVerticeIndex < lVerticeCount; lVerticeIndex++)
			{
				int index = lMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex);
				if (index < 0)
				{
					continue;
				}

				indexList[lVerticeIndex] = index;
				lMesh->GetPolygonVertexNormal(lPolygonIndex, lVerticeIndex, normalList[lVerticeIndex]);
				lMesh->GetPolygonVertexUV(lPolygonIndex, lVerticeIndex, lUVName, uvList[lVerticeIndex], isUnmaped);
			}

			int indexOrders[] = { 0,1,2,0,2,3 };

			int loopCount = lVerticeCount == 4 ? 6 : 3;
			for (int i = 0; i < loopCount; i++)
			{
				int index = indexList[indexOrders[i]];
				pProMesh->mVertices.Add(FVector(lVertexArray[index].mData[0], lVertexArray[index].mData[1], -lVertexArray[index].mData[2]));
				pProMesh->mTriangles.Add(pProMesh->mIndexCounter++);

				FbxVector4& normal = normalList[indexOrders[i]];
				pProMesh->mNormals.Add(FVector(normal.mData[0], normal.mData[1], -normal.mData[2]));

				FbxVector2& uv = uvList[indexOrders[i]];
				pProMesh->mUV.Add(FVector2D(uv.mData[0], 1 - uv.mData[1]));
			}
		}
	}

	//APP_LOG("LoadMesh~ poly count %d\n", lPolygonCount);

	//}
	//glDisable(GL_BLEND); // 알파 블랜드 처리 종료.
	//glPopMatrix();

	delete[] lVertexArray;
// 	delete[] lVertexArrayT;
}

void ARtSceneManagerActor::LoadMesh(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pGlobalPosition, FbxPose* pPose,
	RtFbxProcedualMeshInfo* pFbxMeshInfo) // RtProMeshSection* pProMesh)
{
	//FString meshName = FString::Printf(TEXT("%s%d"), pNode->GetNameOnly().Buffer(), 0);
	auto pMeshArray = pFbxMeshInfo->FindMeshArray(STR(pNode->GetNameOnly().Buffer()));
	if (pMeshArray && pMeshArray->Num() > 1)
	{
		for (auto & item : *pMeshArray)
		{
			LoadSubMesh(pNode, pTime, pAnimLayer, pGlobalPosition, pPose, item.Get());
		}
	}
	else
	{
		RtProMeshSection* pProMesh = pFbxMeshInfo->AddProcedualMeshSection(STR(pNode->GetNameOnly().Buffer())); // STR(pNode->GetNameOnly().Buffer()));

		const FbxSurfaceMaterial* lMaterial = pNode->GetMaterial(0);
		pProMesh->mMaterialName = lMaterial->GetName();

		FbxMesh* lMesh = pNode->GetMesh();
		pProMesh->mSubMeshIndexOffset = 0;
		pProMesh->mSubMeshIndexOffsetEnd = lMesh->GetPolygonCount();

		LoadSubMesh(pNode, pTime, pAnimLayer, pGlobalPosition, pPose, pProMesh);
	}
}

void ARtSceneManagerActor::LoadNode(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pParentGlobalPosition, FbxAMatrix& pGlobalPosition, FbxPose* pPose,
	RtFbxProcedualMeshInfo* pFbxMeshInfo)
{
	FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

	//PRINT_LOG(TEXT("LoadNode~\n"));

	if (lNodeAttribute)
	{
		// All lights has been processed before the whole scene because they influence every geometry.
		if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMarker)
		{
			// 			DrawMarker(pGlobalPosition);
		}
		else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			// 			DrawSkeleton(pNode, pParentGlobalPosition, pGlobalPosition);
		}
		// NURBS and patch have been converted into triangluation meshes.
		else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			APP_LOG("node : %s\n", STR(pNode->GetNameOnly().Buffer()));

			//RtProMeshSection* pProMesh = pFbxMeshInfo->AddProcedualMeshSection(STR(pNode->GetNameOnly().Buffer()));
			//			RtProcedualMeshSection* pProMesh = mpProcedualMeshActor->AddProcedualMeshSection();

			LoadMesh(pNode, pTime, pAnimLayer, pGlobalPosition, pPose, pFbxMeshInfo);
		}
		else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eCamera)
		{
			// 			DrawCamera(pNode, pTime, pAnimLayer, pGlobalPosition);
		}
		else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eNull)
		{
			// 			DrawNull(pGlobalPosition);
		}
	}
	else
	{
		// Draw a Null for nodes without attribute.
		//DrawNull(pGlobalPosition);
	}
}

void ARtSceneManagerActor::LoadNodeRecursive(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pParentGlobalPosition, FbxPose* pPose,
	RtFbxProcedualMeshInfo* pFbxMeshInfo)
{
	FbxAMatrix lGlobalPosition = GetGlobalPosition(pNode, pTime, pPose, &pParentGlobalPosition);

	if (pNode->GetNodeAttribute())
	{
		// Geometry offset.
		// it is not inherited by the children.
		FbxAMatrix lGeometryOffset = GetGeometry(pNode);
		FbxAMatrix lGlobalOffPosition = lGlobalPosition * lGeometryOffset;

		LoadNode(pNode, pTime, pAnimLayer, pParentGlobalPosition, lGlobalOffPosition, pPose, pFbxMeshInfo);
	}

	//	bool iamTop = (pRootNode == pNode);

	const int lChildCount = pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		LoadNodeRecursive(pNode->GetChild(lChildIndex), pTime, pAnimLayer, lGlobalPosition, pPose, pFbxMeshInfo);
	}

}

void ARtSceneManagerActor::UpdateSubMesh(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pGlobalPosition, FbxPose* pPose, RtProMeshSection* pProMesh)
{
	FbxMesh* lMesh = pNode->GetMesh();
	const int lVertexCount = lMesh->GetControlPointsCount();
	if (lVertexCount == 0)
	{
		return;
	}

	//const VBOMesh* lMeshCache = static_cast<const VBOMesh*>(lMesh->GetUserDataPtr());
// If it has some defomer connection, update the vertices position
	const bool lHasVertexCache = lMesh->GetDeformerCount(FbxDeformer::eVertexCache) &&
		(static_cast<FbxVertexCacheDeformer*>(lMesh->GetDeformer(0, FbxDeformer::eVertexCache)))->Active.Get();
	const bool lHasShape = lMesh->GetShapeCount() > 0;
	const bool lHasSkin = lMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
	const bool lHasDeformation = lHasVertexCache || lHasShape || lHasSkin;

	FbxVector4* lVertexArray = new FbxVector4[lVertexCount];
	//lVertexArrayT = new FbxVector4[lVertexCount];

	memcpy(lVertexArray, lMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));

	if (lHasDeformation)
	{
		// Active vertex cache deformer will overwrite any other deformer
		if (lHasVertexCache)
		{
			//ReadVertexCacheData(lMesh, pTime, lVertexArray);
		}
		else
		{
			if (lHasShape)
			{
				// Deform the vertex array with the shapes.
				ComputeShapeDeformation(lMesh, pTime, pAnimLayer, lVertexArray);
			}

			//we need to get the number of clusters
			const int lSkinCount = lMesh->GetDeformerCount(FbxDeformer::eSkin);
			int lClusterCount = 0;
			for (int lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex)
			{
				lClusterCount += ((FbxSkin*)(lMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin)))->GetClusterCount();
			}

			if (lClusterCount)
			{
				// Deform the vertex array with the skin deformer.
				ComputeSkinDeformation(pGlobalPosition, lMesh, pTime, lVertexArray, pPose);

				//ApplyWorldTransform(pGlobalPosition); // DrawMesh animation
			}
		}

		//if (lMeshCache)
		//	lMeshCache->UpdateVertexPosition(lMesh, lVertexArray);
	}

	//float scale = 1.0f;
	for (int i = 0; i < lVertexCount; i++)
	{
		//FbxVector4 newVertex = pGlobalPosition.MultT(lVertexArray[i]);
		//lVertexArrayT[i] = newVertex;

		lVertexArray[i] = pGlobalPosition.MultT(lVertexArray[i]);
	}

	int lPolygonCount = lMesh->GetPolygonCount();
	bool isUnmaped;

	//	APP_LOG("LoadMesh~ poly count %d\n", lPolygonCount);

	FbxStringList lUVNames;
	lMesh->GetUVSetNames(lUVNames);
	const char* lUVName = NULL;
	if (lUVNames.GetCount())
	{
		lUVName = lUVNames[0];
	}
	
	if (pProMesh != NULL)
	{
		FbxVector4 normalList[] = { FbxVector4(0,0,0),FbxVector4(0,0,0),FbxVector4(0,0,0),FbxVector4(0,0,0) };
		FbxVector2 uvList[] = { FbxVector2(0,0),FbxVector2(0,0),FbxVector2(0,0),FbxVector2(0,0) };
		int indexList[] = { 0,0,0,0 };
		int dataIndex = 0;

		for (int lPolygonIndex = pProMesh->mSubMeshIndexOffset; lPolygonIndex < pProMesh->mSubMeshIndexOffsetEnd; lPolygonIndex++)
		{
			int lVerticeCount = lMesh->GetPolygonSize(lPolygonIndex);
			//			PRINT_LOG(TEXT("poly4 index : %d vert count : %d\n"), lPolygonIndex, lVerticeCount);

			for (int lVerticeIndex = 0; lVerticeIndex < lVerticeCount; lVerticeIndex++)
			{
				int index = lMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex);
				if (index < 0)
				{
					continue;
				}

				indexList[lVerticeIndex] = index;
				lMesh->GetPolygonVertexNormal(lPolygonIndex, lVerticeIndex, normalList[lVerticeIndex]);
				lMesh->GetPolygonVertexUV(lPolygonIndex, lVerticeIndex, lUVName, uvList[lVerticeIndex], isUnmaped);
			}

			int indexOrders[] = { 0,1,2,0,2,3 };

			int loopCount = lVerticeCount == 4 ? 6 : 3;
			for (int i = 0; i < loopCount; i++)
			{
				int index = indexList[indexOrders[i]];
				pProMesh->mVertices[dataIndex] = FVector(lVertexArray[index].mData[0], lVertexArray[index].mData[1], -lVertexArray[index].mData[2]);

				FbxVector4& normal = normalList[indexOrders[i]];
				pProMesh->mNormals[dataIndex] = FVector(normal.mData[0], normal.mData[1], -normal.mData[2]);

				dataIndex++;
			}
		}
	}

	//APP_LOG("LoadMesh~ poly count %d\n", lPolygonCount);

	//}
	//glDisable(GL_BLEND); // 알파 블랜드 처리 종료.
	//glPopMatrix();

	delete[] lVertexArray;
}

void ARtSceneManagerActor::UpdateMesh(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pGlobalPosition, FbxPose* pPose, RtFbxProcedualMeshInfo* pFbxMeshInfo)
{
	//FString meshName = FString::Printf(TEXT("%s%d"), pNode->GetNameOnly().Buffer(), 0);
	auto pMeshArray = pFbxMeshInfo->FindMeshArray(STR(pNode->GetNameOnly().Buffer()));
	if (pMeshArray && pMeshArray->Num() > 1)
	{
		for (auto& item : *pMeshArray)
		{
			UpdateSubMesh(pNode, pTime, pAnimLayer, pGlobalPosition, pPose, item.Get());
		}
	}
	else
	{
		RtProMeshSection* pProMesh = pFbxMeshInfo->FindProcedualMeshSection(STR(pNode->GetNameOnly().Buffer())); // STR(pNode->GetNameOnly().Buffer()));
		if (pProMesh)
		{
			const FbxSurfaceMaterial* lMaterial = pNode->GetMaterial(0);
			pProMesh->mMaterialName = lMaterial->GetName();

			FbxMesh* lMesh = pNode->GetMesh();
			pProMesh->mSubMeshIndexOffset = 0;
			pProMesh->mSubMeshIndexOffsetEnd = lMesh->GetPolygonCount();

			UpdateSubMesh(pNode, pTime, pAnimLayer, pGlobalPosition, pPose, pProMesh);
		}
	}
}

void ARtSceneManagerActor::UpdateNode(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pParentGlobalPosition, FbxAMatrix& pGlobalPosition, FbxPose* pPose, 
	RtFbxProcedualMeshInfo* pFbxMeshInfo)
{
	FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

	//PRINT_LOG(TEXT("LoadNode~\n"));

	if (lNodeAttribute)
	{
		// All lights has been processed before the whole scene because they influence every geometry.
		if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMarker)
		{
			// 			DrawMarker(pGlobalPosition);
		}
		else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			// 			DrawSkeleton(pNode, pParentGlobalPosition, pGlobalPosition);
		}
		// NURBS and patch have been converted into triangluation meshes.
		else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			//APP_LOG("node : %s\n", STR(pNode->GetNameOnly().Buffer()));

			//RtProMeshSection* pProMesh = pFbxMeshInfo->AddProcedualMeshSection(STR(pNode->GetNameOnly().Buffer()));
			//			RtProcedualMeshSection* pProMesh = mpProcedualMeshActor->AddProcedualMeshSection();

			UpdateMesh(pNode, pTime, pAnimLayer, pGlobalPosition, pPose, pFbxMeshInfo);
		}
		else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eCamera)
		{
			// 			DrawCamera(pNode, pTime, pAnimLayer, pGlobalPosition);
		}
		else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eNull)
		{
			// 			DrawNull(pGlobalPosition);
		}
	}
	else
	{
		// Draw a Null for nodes without attribute.
		//DrawNull(pGlobalPosition);
	}

}

void ARtSceneManagerActor::UpdateNodeRecursive(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pParentGlobalPosition, FbxPose* pPose,
	RtFbxProcedualMeshInfo* pFbxMeshInfo)
{
	FbxAMatrix lGlobalPosition = GetGlobalPosition(pNode, pTime, pPose, &pParentGlobalPosition);

	if (pNode->GetNodeAttribute())
	{
		// Geometry offset.
		// it is not inherited by the children.
		FbxAMatrix lGeometryOffset = GetGeometry(pNode);
		FbxAMatrix lGlobalOffPosition = lGlobalPosition * lGeometryOffset;

		UpdateNode(pNode, pTime, pAnimLayer, pParentGlobalPosition, lGlobalOffPosition, pPose, pFbxMeshInfo);
	}

	//	bool iamTop = (pRootNode == pNode);

	const int lChildCount = pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		UpdateNodeRecursive(pNode->GetChild(lChildIndex), pTime, pAnimLayer, lGlobalPosition, pPose, pFbxMeshInfo);
	}

}


TSet<FbxFileTexture*> GetFbxMaterialTextures(const FbxSurfaceMaterial& Material)
{
	TSet<FbxFileTexture*> TextureSet;

	int32 TextureIndex;
	FBXSDK_FOR_EACH_TEXTURE(TextureIndex)
	{
		FbxProperty Property = Material.FindProperty(FbxLayerElement::sTextureChannelNames[TextureIndex]);

		if (Property.IsValid())
		{
			//We use auto as the parameter type to allow for a generic lambda accepting both FbxProperty and FbxLayeredTexture
			auto AddSrcTextureToSet = [&TextureSet](const auto& InObject) {
				int32 NbTextures = InObject.template GetSrcObjectCount<FbxTexture>();
				for (int32 TexIndex = 0; TexIndex < NbTextures; ++TexIndex)
				{
					FbxFileTexture* Texture = InObject.template GetSrcObject<FbxFileTexture>(TexIndex);
					if (Texture)
					{
						TextureSet.Add(Texture);
					}
				}
			};

			//Here we have to check if it's layered textures, or just textures:
			const int32 LayeredTextureCount = Property.GetSrcObjectCount<FbxLayeredTexture>();
			if (LayeredTextureCount > 0)
			{
				for (int32 LayerIndex = 0; LayerIndex < LayeredTextureCount; ++LayerIndex)
				{
					if (const FbxLayeredTexture* lLayeredTexture = Property.GetSrcObject<FbxLayeredTexture>(LayerIndex))
					{
						AddSrcTextureToSet(*lLayeredTexture);
					}
				}
			}
			else
			{
				//no layered texture simply get on the property
				AddSrcTextureToSet(Property);
			}
		}
	}

	return TextureSet;
}

// Bake node attributes and materials under this node recursively.
// Currently only mesh, light and material.
void LoadMaterialRecursive(FbxNode* pNode, FbxAnimLayer* pAnimLayer, RtFbxProcedualMeshInfo* pFbxProMeshInfo)
{
	// Bake material and hook as user data.
	const int lMaterialCount = pNode->GetMaterialCount();
	for (int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; ++lMaterialIndex)
	{
		FbxSurfaceMaterial* lMaterial = pNode->GetMaterial(lMaterialIndex);
		if (lMaterial)
		{
			APP_LOG("Mat Name %s\n", STR(lMaterial->GetName()));

			RtMaterialInfo* pMatInfo = pFbxProMeshInfo->FindMaterialInfo(FString(lMaterial->GetName()));
			if (pMatInfo == NULL)
			{
				pMatInfo = pFbxProMeshInfo->AddMaterialInfo(FString(lMaterial->GetName()));

				auto data = GetFbxMaterialTextures(*lMaterial);
				for (auto pItem : data)
				{
					int destCount = pItem->GetDstPropertyCount();
					if (destCount > 0)
					{
						FbxProperty pProp = pItem->GetDstProperty(0);

						if (pProp.GetName() == "DiffuseColor")
						{
							pMatInfo->mDiffuseFilePath = FString(pItem->GetFileName());
						}
						else if (pProp.GetName() == "NormalMap")
						{
							pMatInfo->mNormalFilePath = FString(pItem->GetFileName());
						}
						else if (pProp.GetName() == "SpecularColor")
						{
							pMatInfo->mSpecularFilePath = FString(pItem->GetFileName());
						}

						APP_LOG("=== tex Path %s, tex Name %s, %s\n",
							STR(pItem->GetFileName()),
							STR(pItem->GetName()),
							STR(pItem->GetPropertyHandle().GetName()));
						APP_LOG("=== dest prop %s \n", STR(pProp.GetName().Buffer()));
					}
				}

				pMatInfo->CreateMaterial();
			}
		}
	}

	TArray<int> subMeshOffsetArr;
	
	FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();
	if (lNodeAttribute)
	{
		// Bake mesh as VBO(vertex buffer object) into GPU.
		if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh* pMesh = pNode->GetMesh();
			if (pMesh && !pMesh->GetUserDataPtr())
			{
				if (pMesh->GetNode())
				{
					const int lPolygonCount = pMesh->GetPolygonCount();

					// Count the polygon count of each material
					FbxLayerElementArrayTemplate<int>* lMaterialIndice = NULL;
					FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;
					if (pMesh->GetElementMaterial())
					{
						lMaterialIndice = &pMesh->GetElementMaterial()->GetIndexArray();
						lMaterialMappingMode = pMesh->GetElementMaterial()->GetMappingMode();
						if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
						{
							FBX_ASSERT(lMaterialIndice->GetCount() == lPolygonCount);
							if (lMaterialIndice->GetCount() == lPolygonCount)
							{
								for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
								{
									const int lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
									if ( subMeshOffsetArr.Num() < lMaterialIndex + 1 )
									{
										subMeshOffsetArr.Add(lPolygonIndex);
									}
								}
							}
						}
					}
				}
			}

			APP_LOG("===> %s : %d", STR(pMesh->GetNameOnly().Buffer()), subMeshOffsetArr.Num());
		}
	}

	int count = 0;
	for (int i=0; i<subMeshOffsetArr.Num(); i++)
	{
		RtProMeshSection* pProMesh = pFbxProMeshInfo->AddProcedualMeshSection(STR(pNode->GetNameOnly().Buffer()), true);

		pProMesh->mSubMeshIndexOffset = subMeshOffsetArr[i];

		if (i + 1 < subMeshOffsetArr.Num())
		{
			pProMesh->mSubMeshIndexOffsetEnd = subMeshOffsetArr[i + 1] - 1;
		}
		else
		{
			FbxMesh* pMesh = pNode->GetMesh();
			pProMesh->mSubMeshIndexOffsetEnd = pMesh->GetPolygonCount();
		}

		const FbxSurfaceMaterial* lMaterial = pNode->GetMaterial(count++);
		pProMesh->mMaterialName = lMaterial->GetName();
	}

	const int lChildCount = pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		LoadMaterialRecursive(pNode->GetChild(lChildIndex), pAnimLayer, pFbxProMeshInfo);
	}
}

// AddFbxFile : fbx 를 로드하고 컨테이너에 담는 함수.
// filePath : Fbx 파일 절대 경로 입력.
// 예) "C:/work/Dev/RND/unreal_total_test/Idle.fbx"
bool ARtSceneManagerActor::AddFbxFile(FString filePath)
{
	std::string path = TCHAR_TO_ANSI(*filePath);
	auto index = path.find_last_of('/');
	std::string key = path.substr(index + 1);

	if (FindFbxInfo(key.c_str())==NULL)
	{
		TSharedPtr<RtFbxProcedualMeshInfo> spFbxInfo = MakeShared<RtFbxProcedualMeshInfo>();
//		spFbxInfo->Init();

//		if (spFbxInfo->mpFbxLoader->LoadScene(TCHAR_TO_ANSI(*filePath)))
		if( RtFBXLoader::Get()->LoadScene(TCHAR_TO_ANSI(*filePath)))
		{
			FbxTime CurrentTime;
			FbxAnimLayer* pCurrentAnimLayer = NULL;
			FbxAMatrix lDummyGlobalPosition;
			FbxPose* pPose = NULL;
			FbxNode* pRootNode = RtFBXLoader::Get()->GetScene()->GetRootNode(); //spFbxInfo->mpFbxLoader->GetScene()->GetRootNode();

//			TSharedPtr<RtFbxProcedualMeshInfo> spFbxInfo = MakeShared<RtFbxProcedualMeshInfo>();
			spFbxInfo->mModelFileName = key.c_str();
			mFbxInfoMap.Add(FString(key.c_str()), spFbxInfo);

			LoadMaterialRecursive(pRootNode, pCurrentAnimLayer, spFbxInfo.Get());

			LoadNodeRecursive(pRootNode, CurrentTime, pCurrentAnimLayer, lDummyGlobalPosition, pPose, spFbxInfo.Get());

			return true;
		}
	}
	else
	{
		// 같은 키 값이 이미 있다.
	}


	return false;
}

bool ARtSceneManagerActor::UpdateModelData(FString modelKey)
{
	RtFbxProcedualMeshInfo* pFbxInfo = FindFbxInfo(modelKey);
	if (pFbxInfo != NULL)
	{

		return true;
	}

	return false;
}

// SpawnActor : Fbx 모델을 담는 액토 스폰 함수.
// modelKey : fbx 파일명. 
// 예) Idle.fbx
// x,y,z = 좌표.
// pitch, yaw, roll =  회전.
ARtProcedualMeshActor* ARtSceneManagerActor::FbxSpawnActor(FString modelKey, float x, float y, float z, float pitch, float yaw, float roll)
{
	RtFbxProcedualMeshInfo* pFbxInfo = FindFbxInfo(modelKey);
	if (pFbxInfo != NULL)
	{
		FActorSpawnParameters spawnParams;
		FRotator spawnRotator;
		FVector spawnLocation = FVector::ZeroVector;
		spawnLocation.Set(x, y, z);

		UWorld* pWorld = GetWorld();
		ARtProcedualMeshActor* pProMeshActor = pWorld->SpawnActor<ARtProcedualMeshActor>(
			ARtProcedualMeshActor::StaticClass(), spawnLocation, spawnRotator, spawnParams);
		if (pProMeshActor != NULL)
		{
			pProMeshActor->CreateMeshs(pFbxInfo, false);
			pProMeshActor->mArrayIndex = mFbxActorArray.Num();

			mFbxActorArray.Add(pProMeshActor);

			FRotator LookAtRotation = FRotator(pitch, yaw, roll);
			pProMeshActor->SetActorRotation(LookAtRotation);

			return pProMeshActor;
		}
	}

	return NULL;
}

// fbx 로드 테스트 함수.
void ARtSceneManagerActor::LoadFbxFile(FString filePath)
{
	//mpFbxLoader->LoadScene(TCHAR_TO_ANSI(*filePath));

	//FActorSpawnParameters spawnParams;
	//FRotator rotator;
	//FVector spawnLocation = FVector::ZeroVector;
	//spawnLocation.Set(1000, 700, 60);

	//UWorld* pWorld = GetWorld();
	//mpProcedualMeshActor = pWorld->SpawnActor<ARtProcedualMeshActor>(ARtProcedualMeshActor::StaticClass(), spawnLocation, rotator, spawnParams);
	//if (mpProcedualMeshActor != NULL)
	//{
	//	FbxTime CurrentTime;
	//	FbxAnimLayer* pCurrentAnimLayer = NULL;
	//	FbxAMatrix lDummyGlobalPosition;
	//	FbxPose* pPose = NULL;
	//	FbxNode* pRootNode = mpFbxLoader->GetScene()->GetRootNode();

	//	//	LoadNode(pRootNode, CurrentTime, pCurrentAnimLayer, lDummyGlobalPosition, lDummyGlobalPosition, pPose);

	//	mpProcedualMeshActor->InitProcedualArray();

	//	LoadNodeRecursive(pRootNode, CurrentTime, pCurrentAnimLayer, lDummyGlobalPosition, pPose, NULL);

	//	mpProcedualMeshActor->CreateTriangle(FVector(0, 0, 0), FVector(0, 300, 0), FVector(300, 0, 0));

	//	mpProcedualMeshActor->CreateMeshs(false);
	//}

	//FRotator LookAtRotation = FRotator(0, 90.f, -90.f);
	//mpProcedualMeshActor->SetActorRotation(LookAtRotation);
}

// Called when the game starts or when spawned
void ARtSceneManagerActor::BeginPlay()
{
	Super::BeginPlay();


	//	if (mpFbxLoader->Init())
		//{

		//	//		mpFbxLoader->LoadScene("C:/work/Dev/RND/unreal_total_test/Swat.fbx");
		//	//mpFbxLoader->LoadScene("C:/work/Dev/RND/unreal_total_test/Remy.fbx");
		//	//		mpFbxLoader->LoadScene("C:/work/Dev/RND/unreal_total_test/Idle.fbx");
		//	//		mpFbxLoader->LoadScene("C:/work/Dev/RND/unreal_total_test/Box_1.fbx");
		//	//		mpFbxLoader->LoadScene("C:/work/Dev/RND/unreal_total_test/pure_box2.fbx");
		//	//	   mpFbxLoader->LoadScene("C:/work/Dev/RND/unreal_total_test/Tri.fbx");
		//	//		mpFbxLoader->LoadScene("C:/work/Dev/RND/unreal_total_test/Wire_Fence.fbx");
		//}

//	LoadFbxFile("C:/work/Dev/RND/unreal_total_test/Remy.fbx");

	//if (AddFbxFile("C:/work/Dev/RND/unreal_total_test/Remy.fbx"))
	//{
	//	SpawnActor("Remy.fbx", 1000, 700, 60, 0, 90, -90);
	//	SpawnActor("Remy.fbx", 1300, 700, 60, 0, 90, -90);
	//}

	FString projectPath = FPaths::ProjectUserDir();
	FString launchPath = FPaths::LaunchDir();
	ARtProcedualMeshActor* pActor;

	if (AddFbxFile(projectPath + "Idle.fbx"))
	{
		pActor = FbxSpawnActor("Idle.fbx", 1500, 700, 60, 0, 90, -90);
		//pActor->PlayAnimation(0);

		pActor = FbxSpawnActor("Idle.fbx", 1500, 800, 60, 0, 90, -90);
// 		pActor->PlayAnimation(0);

		pActor = FbxSpawnActor("Idle.fbx", 1500, 900, 60, 0, 90, -90);
//		pActor->PlayAnimation(0);

		pActor = FbxSpawnActor("Idle.fbx", 1500, 1000, 60, 0, 90, -90);
//		pActor->PlayAnimation(0);

		pActor = FbxSpawnActor("Idle.fbx", 1500, 1100, 60, 0, 90, -90);
//		pActor->PlayAnimation(0);
	}
	else if(AddFbxFile(launchPath + "Idle.fbx"))
	{
		pActor = FbxSpawnActor("Idle.fbx", 1500, 700, 60, 0, 90, -90);
	}

	if (AddFbxFile(projectPath + "Swat.fbx"))
	{
		for (int i = 0; i < 10; i++)
		{
			pActor = FbxSpawnActor("Swat.fbx", 1000, i*200, 60, 0, 90, -90);
		}
	}

	//if (AddFbxFile("C:/work/Dev/RND/unreal_total_test/Idle.fbx"))
	//{
	//	SpawnActor("Idle.fbx", 1500, 700, 60, 0, 90, -90);
	//}

	//if (AddFbxFile("C:/work/Dev/RND/unreal_total_test/Remy.fbx"))
	//{
	//	SpawnActor("Remy.fbx", 1600, 700, 60, 0, 90, -90);
	//}

	//if (AddFbxFile(projectPath + "Swat.fbx"))
	//{
	//	SpawnActor("Swat.fbx", 1500, 850, 60, 0, 90, -90);
	//}

	//if (AddFbxFile("C:/work/Dev/RND/unreal_total_test/Box_1.fbx"))
	//{
	//	SpawnActor("Box_1.fbx", 1700, 900, 60, 0, 90, -90);
	//}

//	mpFbxLoader->SetCurrentAnimStack(0);

	//LoadJson();
}

bool ARtSceneManagerActor::LoadJson()
{
//	FString jsonFilePath = "C:/work/Dev/Rt/TCTData/swat.json";
//
//	FString load_jsonstr;	// Recive load json text
//	FFileHelper::LoadFileToString(load_jsonstr, *jsonFilePath);	// Load json to filePath
//
//	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(load_jsonstr);
//	TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject());
//
//	if (FJsonSerializer::Deserialize(reader, jsonObj) && jsonObj.IsValid())
//	{
//		TSharedPtr<FJsonValue> jdata = jsonObj->TryGetField(FString("name"));	// One object of current Object
////		FString data = jdata->AsString();
//		APP_LOG("json load test : %s", *jdata->AsString());
//
//		jsonObj->SetStringField(FString("name"), FString("oh my god~ddo!"));
//	}
//
//	FString save_jsonstr;
//	FString jsonWriteFilePath = "C:/work/Dev/Rt/TCTData/swat_ed.json";
//	//TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> writer = 
//	//	TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&jsonWriteFilePath);
//	
//	if (FJsonSerializer::Serialize(jsonObj.ToSharedRef(), TJsonWriterFactory<>::Create(&save_jsonstr, 0)))
//	{
//		FFileHelper::SaveStringToFile(save_jsonstr, *jsonWriteFilePath);	// Load json to filePath
//		return true;
//	}
//
//	FFileHelper::SaveStringToFile(save_jsonstr, *jsonWriteFilePath);	// Load json to filePath
//
//	//FString RequestBody;
//	//TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
//	//FJsonSerializer::Serialize(jsonObj, Writer);

	return true;
}

bool ARtSceneManagerActor::SaveJson()
{
	////Key Value 입력하는 방식
	//FString FileStr;
	//FString FilePath = TEXT("C:/work/Dev/Rt/TCTData/swat_ed.json");
	//TSharedRef<TJsonWriter<TCHAR>> Writer = TJsonWriterFactory<>::Create(&FileStr);

	//Writer->WriteObjectStart();

	return true;
}

// Called every frame
void ARtSceneManagerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (mActorArray.Num() > 0)
	//{
	//	auto pProActor = mActorArray[0];
	//	
	//	RtFbxProcedualMeshInfo* pProMeshInfo = pProActor->GetProMeshInfo();

	//	if (pProMeshInfo != NULL)
	//	{
	//		pProMeshInfo->mpFbxLoader->Tick(DeltaTime);

	//		auto pFbxLoader = pProMeshInfo->GetFbxLoader();
	//		auto pScene = pFbxLoader->GetScene();
	//		
	//		FbxPose* lPose = NULL;
	//		FbxAMatrix lDummyGlobalPosition;

	//		//RtFbxProcedualMeshInfo* pProMeshInfo = FindFbxInfo("Idle.fbx");
	//		UpdateNodeRecursive(pScene->GetRootNode(), pFbxLoader->GetCurrentTime(), pFbxLoader->GetCurrentAnimLayer(), lDummyGlobalPosition, lPose, pProMeshInfo);

	//		pProActor->UpdateMeshs(pProMeshInfo);
	//	}
	//}

	//if (mpFbxLoader != NULL)
	//{
	//	mpFbxLoader->Tick(DeltaTime);

	//	auto pScene = mpFbxLoader->GetScene();

	//	FbxPose* lPose = NULL;
	//	FbxAMatrix lDummyGlobalPosition;
	//	
	//	RtFbxProcedualMeshInfo* pProMeshInfo = FindFbxInfo("Idle.fbx");
	//	if (pProMeshInfo != NULL)
	//	{
	//		UpdateNodeRecursive(pScene->GetRootNode(), mpFbxLoader->GetCurrentTime(), mpFbxLoader->GetCurrentAnimLayer(), lDummyGlobalPosition, lPose, pProMeshInfo);

	//		mActorArray[0]->UpdateMeshs(pProMeshInfo);
	//	}
	//}

//	UpdateNodeRecursive()
}

