// Fill out your copyright notice in the Description page of Project Settings.
#include "RtFbxLoaderSubsystem.h"

#include "../RtCore.h"

#include "../Fbx/SceneCache.h"
#include "../Fbx/GetPosition.h"
#include "../Fbx/DrawSceneUtil.h"

#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(mpSdkManager->GetIOSettings()))
#endif

typedef unsigned int GLuint;

URtFbxLoaderSubsystem* URtFbxLoaderSubsystem::mpInstance;

void URtFbxLoaderSubsystem::InitSubSystem(UGameInstance* pGameInstance)
{
	if( NULL != pGameInstance )
	{
		mpInstance = pGameInstance->GetSubsystem<URtFbxLoaderSubsystem>();
		mpInstance->Init();
	}
}

// Unload the cache and release the memory under this node recursively.
void URtFbxLoaderSubsystem::_UnloadCacheRecursive(FbxNode* pNode)
{
	// Unload the material cache
	const int lMaterialCount = pNode->GetMaterialCount();
	for (int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; ++lMaterialIndex)
	{
		FbxSurfaceMaterial* lMaterial = pNode->GetMaterial(lMaterialIndex);
		if (lMaterial && lMaterial->GetUserDataPtr())
		{
			MaterialCache* lMaterialCache = static_cast<MaterialCache*>(lMaterial->GetUserDataPtr());
			lMaterial->SetUserDataPtr(NULL);
			delete lMaterialCache;
		}
	}

	FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();
	if (lNodeAttribute)
	{
		// Unload the mesh cache
		if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh* lMesh = pNode->GetMesh();
			if (lMesh && lMesh->GetUserDataPtr())
			{
				VBOMesh* lMeshCache = static_cast<VBOMesh*>(lMesh->GetUserDataPtr());
				lMesh->SetUserDataPtr(NULL);
				delete lMeshCache;
			}
		}
		// Unload the light cache
		else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eLight)
		{
			FbxLight* lLight = pNode->GetLight();
			if (lLight && lLight->GetUserDataPtr())
			{
				LightCache* lLightCache = static_cast<LightCache*>(lLight->GetUserDataPtr());
				lLight->SetUserDataPtr(NULL);
				delete lLightCache;
			}
		}
	}

	const int lChildCount = pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		_UnloadCacheRecursive(pNode->GetChild(lChildIndex));
	}
}

// Unload the cache and release the memory fro this scene and release the textures in GPU
void URtFbxLoaderSubsystem::_UnloadCacheRecursive(FbxScene* pScene)
{
	const int lTextureCount = pScene->GetTextureCount();
	for (int lTextureIndex = 0; lTextureIndex < lTextureCount; ++lTextureIndex)
	{
		FbxTexture* lTexture = pScene->GetTexture(lTextureIndex);
		FbxFileTexture* lFileTexture = FbxCast<FbxFileTexture>(lTexture);
		if (lFileTexture && lFileTexture->GetUserDataPtr())
		{
			GLuint* lTextureName = static_cast<GLuint*>(lFileTexture->GetUserDataPtr());
			//lFileTexture->SetUserDataPtr(NULL);
			//glDeleteTextures(1, lTextureName);
			delete lTextureName;
		}
	}

	_UnloadCacheRecursive(pScene->GetRootNode());
}

void URtFbxLoaderSubsystem::InitializeSdkObjects()
{
}

void URtFbxLoaderSubsystem::DestroySdkObjects()
{
	if (mpScene) {
		_UnloadCacheRecursive(mpScene);
		mpScene->Clear();
	}

	Init();

	//Delete the FBX Manager. All the objects that have been allocated using the FBX Manager and that haven't been explicitly destroyed are also automatically destroyed.
	if (mpSdkManager) {
		mpSdkManager->Destroy();
	}
}

bool URtFbxLoaderSubsystem::LoadScene(const char* pFilename)
{
	if (mpScene) //&& mStatus != UNLOADED)
	{
		_UnloadCacheRecursive(mpScene);
		mpScene->Clear();
		mpScene = FbxScene::Create(mpSdkManager, "My Scene");
		//		mStatus = UNLOADED;
	}

	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor, lSDKMinor, lSDKRevision;
	//int lFileFormat = -1;
	int lAnimStackCount;
	bool lStatus;
	char lPassword[1024];

	// Get the file version number generate by the FBX SDK.
	FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(mpSdkManager, "");

	// Initialize the importer by providing a filename.
	const bool lImportStatus = lImporter->Initialize(pFilename, -1, mpSdkManager->GetIOSettings());
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	if (!lImportStatus)
	{
		FbxString error = lImporter->GetStatus().GetErrorString();
		//FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
		//FBXSDK_printf("Error returned: %s\n\n", error.Buffer());

		if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			//FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
			//FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
		}

		return false;
	}

	//FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);

	if (lImporter->IsFBX())
	{
		//FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);

		// From this point, it is possible to access animation stack information without
		// the expense of loading the entire file.

		//FBXSDK_printf("Animation Stack Information\n");

		lAnimStackCount = lImporter->GetAnimStackCount();

		//FBXSDK_printf("    Number of Animation Stacks: %d\n", lAnimStackCount);
		//FBXSDK_printf("    Current Animation Stack: \"%s\"\n", lImporter->GetActiveAnimStackName().Buffer());
		//FBXSDK_printf("\n");

		for (int i = 0; i < lAnimStackCount; i++)
		{
			FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);

			//FBXSDK_printf("    Animation Stack %d\n", i);
			//FBXSDK_printf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
			//FBXSDK_printf("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());

			// Change the value of the import name if the animation stack should be imported 
			// under a different name.
			//FBXSDK_printf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());

			// Set the value of the import state to false if the animation stack should be not
			// be imported. 
			//FBXSDK_printf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
			//FBXSDK_printf("\n");
		}

		FbxIOSettings* pIOSet = mpSdkManager->GetIOSettings();
		if (pIOSet != NULL)
		{
			pIOSet->SetBoolProp(IMP_FBX_MATERIAL, true);
			pIOSet->SetBoolProp(IMP_FBX_MATERIAL, true);
			pIOSet->SetBoolProp(IMP_FBX_TEXTURE, true);
			pIOSet->SetBoolProp(IMP_FBX_LINK, true);
			pIOSet->SetBoolProp(IMP_FBX_SHAPE, true);
			pIOSet->SetBoolProp(IMP_FBX_GOBO, true);
			pIOSet->SetBoolProp(IMP_FBX_ANIMATION, true);
			pIOSet->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
		}

		// Set the import states. By default, the import states are always set to 
		// true. The code below shows how to change these states.
		//IOS_REF.SetBoolProp(IMP_FBX_MATERIAL,        true);
		//IOS_REF.SetBoolProp(IMP_FBX_TEXTURE,         true);
		//IOS_REF.SetBoolProp(IMP_FBX_LINK,            true);
		//IOS_REF.SetBoolProp(IMP_FBX_SHAPE,           true);
		//IOS_REF.SetBoolProp(IMP_FBX_GOBO,            true);
		//IOS_REF.SetBoolProp(IMP_FBX_ANIMATION,       true);
		//IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}

	// Import the scene.
	lStatus = lImporter->Import(mpScene);
	if (lStatus == false && lImporter->GetStatus() == FbxStatus::ePasswordError)
	{
		//FBXSDK_printf("Please enter password: ");

		lPassword[0] = '\0';

		FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
			scanf("%s", lPassword);
		FBXSDK_CRT_SECURE_NO_WARNING_END

			FbxString lString(lPassword);

		IOS_REF.SetStringProp(IMP_FBX_PASSWORD, lString);
		IOS_REF.SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);

		lStatus = lImporter->Import(mpScene);

		if (lStatus == false && lImporter->GetStatus() == FbxStatus::ePasswordError)
		{
			//FBXSDK_printf("\nPassword is wrong, import aborted.\n");
		}
	}

	if (!lStatus || (lImporter->GetStatus() != FbxStatus::eSuccess))
	{
		//FBXSDK_printf("********************************************************************************\n");
		if (lStatus)
		{
			//FBXSDK_printf("WARNING:\n");
			//FBXSDK_printf("   The importer was able to read the file but with errors.\n");
			//FBXSDK_printf("   Loaded scene may be incomplete.\n\n");
		}
		else
		{
			//FBXSDK_printf("Importer failed to load the file!\n\n");
		}

		if (lImporter->GetStatus() != FbxStatus::eSuccess)
		{	
		//FBXSDK_printf("   Last error message: %s\n", lImporter->GetStatus().GetErrorString());
		}

		FbxArray<FbxString*> history;
		lImporter->GetStatus().GetErrorStringHistory(history);
		if (history.GetCount() > 1)
		{
			//FBXSDK_printf("   Error history stack:\n");
			for (int i = 0; i < history.GetCount(); i++)
			{
				//FBXSDK_printf("      %s\n", history[i]->Buffer());
			}
		}
		FbxArrayDelete<FbxString*>(history);
		//FBXSDK_printf("********************************************************************************\n");
	}

	// Get the list of all the animation stack.
	mpScene->FillAnimStackNameArray(mAnimStackNameArray);

	// Destroy the importer.
	lImporter->Destroy();

	return lStatus;
}

bool URtFbxLoaderSubsystem::SetCurrentAnimStack(int pIndex)
{
	//const int lAnimStackCount = mAnimStackNameArray.GetCount();
	//if (!lAnimStackCount || pIndex >= lAnimStackCount)
	//{
	//	return false;
	//}

	//// select the base layer from the animation stack
	//FbxAnimStack* lCurrentAnimationStack = mpScene->FindMember<FbxAnimStack>(mAnimStackNameArray[pIndex]->Buffer());
	//if (lCurrentAnimationStack == NULL)
	//{
	//	// this is a problem. The anim stack should be found in the scene!
	//	return false;
	//}

	//// we assume that the first animation layer connected to the animation stack is the base layer
	//// (this is the assumption made in the FBXSDK)
	//mpCurrentAnimLayer = lCurrentAnimationStack->GetMember<FbxAnimLayer>();
	//mpScene->SetCurrentAnimationStack(lCurrentAnimationStack);

	//FbxTakeInfo* lCurrentTakeInfo = mpScene->GetTakeInfo(*(mAnimStackNameArray[pIndex]));
	//if (lCurrentTakeInfo)
	//{
	//	mStart = lCurrentTakeInfo->mLocalTimeSpan.GetStart();
	//	mStop = lCurrentTakeInfo->mLocalTimeSpan.GetStop();
	//}
	//else
	//{
	//	// Take the time line value
	//	FbxTimeSpan lTimeLineTimeSpan;
	//	mpScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(lTimeLineTimeSpan);

	//	mStart = lTimeLineTimeSpan.GetStart();
	//	mStop = lTimeLineTimeSpan.GetStop();
	//}

	//// check for smallest start with cache start
	//if (mCache_Start < mStart)
	//	mStart = mCache_Start;

	//// check for biggest stop with cache stop
	//if (mCache_Stop > mStop)
	//	mStop = mCache_Stop;

	//// move to beginning
	//mCurrentTime = mStart;

	//// Set the scene status flag to refresh 
	//// the scene in the next timer callback.
	////mStatus = MUST_BE_REFRESHED;

	return true;
}

void URtFbxLoaderSubsystem::Tick(float DeltaTime)
{
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

}

void URtFbxLoaderSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{

}

void URtFbxLoaderSubsystem::Deinitialize()
{
	DestroySdkObjects();
}

//URtFbxLoaderSubsystem::~URtFbxLoaderSubsystem()
//{
//	DestroySdkObjects();
//}

bool URtFbxLoaderSubsystem::Init()
{
	//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
	mpSdkManager = FbxManager::Create();

	if (!mpSdkManager)
	{
		//FBXSDK_printf("Error: Unable to create FBX Manager!\n");
		return false;
	}
	else
	{
		//FBXSDK_printf("Autodesk FBX SDK version %s\n", mpSdkManager->GetVersion());
	}

	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(mpSdkManager, IOSROOT);
	mpSdkManager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	FbxString lPath = FbxGetApplicationDirectory();
	// mpSdkManager->LoadPluginsDirectory(lPath.Buffer()); // 여기서 에러 남..;;;

	//Create an FBX scene. This object holds most objects imported/exported from/to files.
	mpScene = FbxScene::Create(mpSdkManager, "My Scene");
	if (!mpScene)
	{
		//FBXSDK_printf("Error: Unable to create FBX scene!\n");
		return false;
	}

	//InitializeSdkObjects();

	return true;
}


TSet<FbxFileTexture*> URtFbxLoaderSubsystem::_GetFbxMaterialTextures(const FbxSurfaceMaterial& Material)
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
void URtFbxLoaderSubsystem::_LoadMaterialRecursive(FbxNode* pNode, FbxAnimLayer* pAnimLayer, RtFbxProcedualMeshInfo* pFbxProMeshInfo)
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

				auto data = _GetFbxMaterialTextures(*lMaterial);
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
						else if (pProp.GetName() == "ShininessExponent")
						{
							pMatInfo->mShininessExponentFilePath = FString(pItem->GetFileName());
						}
						else if (pProp.GetName() == "TransparencyFactor")
						{
							pMatInfo->mAlphaFilePath = FString(pItem->GetFileName());
						}

						APP_LOG("=== tex Path '%s', tex Name '%s', '%s', prop Name '%s'\n",
							STR(pItem->GetFileName()),
							STR(pItem->GetName()),
							STR(pItem->GetPropertyHandle().GetName()), 
							STR(pProp.GetName()));
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
									if (subMeshOffsetArr.Num() < lMaterialIndex + 1)
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
	for (int i = 0; i < subMeshOffsetArr.Num(); i++)
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
		_LoadMaterialRecursive(pNode->GetChild(lChildIndex), pAnimLayer, pFbxProMeshInfo);
	}
}

void URtFbxLoaderSubsystem::LoadSubMesh(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pGlobalPosition, FbxPose* pPose,
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

void URtFbxLoaderSubsystem::LoadMesh(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pGlobalPosition, FbxPose* pPose,
	RtFbxProcedualMeshInfo* pFbxMeshInfo) // RtProMeshSection* pProMesh)
{
	//FString meshName = FString::Printf(TEXT("%s%d"), pNode->GetNameOnly().Buffer(), 0);
	auto pMeshArray = pFbxMeshInfo->FindMeshArray(STR(pNode->GetNameOnly().Buffer()));
	if (pMeshArray && pMeshArray->Num() > 1)
	{
		for (auto& item : *pMeshArray)
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

void URtFbxLoaderSubsystem::LoadNode(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pParentGlobalPosition, FbxAMatrix& pGlobalPosition, FbxPose* pPose,
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

void URtFbxLoaderSubsystem::LoadNodeRecursive(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, FbxAMatrix& pParentGlobalPosition, FbxPose* pPose,
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

TSharedPtr<RtFbxProcedualMeshInfo> URtFbxLoaderSubsystem::LoadFbx(const FString& fbxFilePath)
{
	if( !fbxFilePath.IsEmpty() )
	{
		if (LoadScene(TCHAR_TO_ANSI(*fbxFilePath)))
		{
			//FString fileName = FPaths::GetBaseFilename(fbxFilePath);
			//CurrentLodingFbxPath = FPaths::GetPath(fbxFilePath);
			//CurrentLodingFbxPath_fbm = FString::Format(TEXT("{0}/{1}.fbm"), { CurrentLodingFbxPath, fileName });
			
			FbxTime CurrentTime;
			FbxAnimLayer* pCurrentAnimLayer = NULL;
			FbxAMatrix lDummyGlobalPosition;
			FbxPose* pPose = NULL;
			FbxNode* pRootNode = GetScene()->GetRootNode();

			TSharedPtr<RtFbxProcedualMeshInfo> spFbxInfo = MakeShared<RtFbxProcedualMeshInfo>();
			//spFbxInfo->mModelFileName = key.c_str();
			//mFbxInfoMap.Add(FString(key.c_str()), spFbxInfo);

			_LoadMaterialRecursive(pRootNode, pCurrentAnimLayer, spFbxInfo.Get());

			LoadNodeRecursive(pRootNode, CurrentTime, pCurrentAnimLayer, lDummyGlobalPosition, pPose, spFbxInfo.Get());

			return spFbxInfo;
		}
	}

	return nullptr;
}
