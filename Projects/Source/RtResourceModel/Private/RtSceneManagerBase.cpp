// Fill out your copyright notice in the Description page of Project Settings.


#include "RtSceneManagerBase.h"
#include "RtJsonFunctionLib.h"

#include "Serialization/JsonSerializer.h" //Json
#include "JsonObjectConverter.h" // JsonUtilities

#include "Kismet/GameplayStatics.h"

#include "RtGlobalActorManager.h"
#include "RtBluePrintCommonUtil.h"

#include "EngineUtils.h"
#include "RtScenarioControllerBase.h"
#include "RtToolSystemFileData.h"

#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"

//#include "MocopiLiveLink.h"
//#include "MocopiLiveLink\Private\MocopiLiveLinkSource.h"
//#include "MocopiBPLibrary.h"

const FString ARtSceneManagerBase::TctSubFolderName = TEXT("Tctfiles/");
const FString ARtSceneManagerBase::FbxSubFolderName = TEXT("Fbxfiles/");
const FString ARtSceneManagerBase::PngSubFolderName = TEXT("Pngfiles/");
const FString ARtSceneManagerBase::WavSubFolderName = TEXT("Wavfiles/");
const FString ARtSceneManagerBase::MapSubFolderName = TEXT("Mapfiles/");

// Sets default values
ARtSceneManagerBase::ARtSceneManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//if (mpFbxLoader == NULL)
	//{
	//	mpFbxLoader = MakeShared<RtFBXLoader>();
	//	mpFbxLoader->Init();
	//}

	SetMctFileLoaded(false);
}

// mct 로드 기본.
FRtMCTTemplate ARtSceneManagerBase::LoadMCT(FString mctFilePath)
{
	bool bOutSuccess;
	FString OutInfoMessage;
//	FRtMCTTemplate LoadedMCT;

	MctData = URtJsonFunctionLib::ReadStructFromJsonFile(mctFilePath, bOutSuccess, OutInfoMessage);

	SetMctFileLoaded(bOutSuccess);

	//int32 test = FMocopiLiveLinkModule::test_mocopi;

//	FMocopiLiveLinkSource* pLLSource = static_cast<FMocopiLiveLinkSource*>(UMocopiBPLibrary::MocopiSourceRef.Get());
	//FMocopiLiveLinkSource* pLLSource2 = UMocopiBPLibrary::MocopiSourceRef2;
	//int32 aaa = pLLSource2->test_mocopi;

	return MctData;
}

// mct 임시 저장 기능. ( 임시 mct 데이터 만들기 툴용 )
bool ARtSceneManagerBase::SaveMCT(FString mctFilePath)
{
	// Try to convert struct to generic json object
	TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(MctData);
	if (JsonObject == nullptr)
	{
		return false;
	}

	bool bSuccess = false;
	FString OutInfoMessage;
	// Try to write json to file
	URtJsonFunctionLib::WriteJson(mctFilePath, JsonObject, bSuccess, OutInfoMessage);

	return bSuccess;
}

bool ARtSceneManagerBase::SaveMCT_WallInstallInfo(FString mctWallInfoFilePath, FRtTemplateWallInstallInfo wallInstallInfo)
{
	// Try to convert struct to generic json object
	TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(wallInstallInfo);
	if (JsonObject == nullptr)
	{
		return false;
	}

//	FString newPath = FPaths::ProjectDir() + mctWallInfoFilePath;

	bool bSuccess = false;
	FString OutInfoMessage;
	// Try to write json to file
	URtJsonFunctionLib::WriteJson(mctWallInfoFilePath, JsonObject, bSuccess, OutInfoMessage);

	return bSuccess;
}

bool ARtSceneManagerBase::UpdateSystemActors(bool bIsNewTctStruct)
{
	AActor* pMapBoundaryActor = FindObjectByName(TOOL_SYSTEM_FILE_NAME_MAP_BOUNDARY_OBJ1);

	if (pMapBoundaryActor==nullptr)
	{
		pMapBoundaryActor = Cast<ARtObjectActor>(SpawnActorBase<ARtObjectActor>(
			TOOL_SYSTEM_FILE_NAME_MAP_BOUNDARY,
			TEXT(""),
			FVector(0, 0, 0),
			FRotator(0, 0, 0),
			ERtProductType::ENone));

		if (pMapBoundaryActor == nullptr)
		{
			if(RegistSystemModels(bIsNewTctStruct) )
			{
				pMapBoundaryActor = Cast<ARtObjectActor>(SpawnActorBase<ARtObjectActor>(
					TOOL_SYSTEM_FILE_NAME_MAP_BOUNDARY,
					TEXT(""),
					FVector(0, 0, 0),
					FRotator(0, 0, 0),
					ERtProductType::ENone));
			}
		}

		if( pMapBoundaryActor )
		{
			SetComponentObjectNameKey(pMapBoundaryActor, TOOL_SYSTEM_FILE_NAME_MAP_BOUNDARY_OBJ1);
			pMapBoundaryActor->SetActorEnableCollision(false);
		}
	}

	if (pMapBoundaryActor)
	{
		// float calcSize = TileSize*0.01f;

		//float xSize = mMCTTempeteData.mapSize.X * calcSize;
		//float ySize = mMCTTempeteData.mapSize.Y * calcSize;

		//pMapBoundaryActor->SetActorScale3D(FVector(xSize, ySize, 10.f));

		int32 startX = MctData.TrainingRangeStart.X - (MctData.mapSize.X / 2);
		int32 startY = MctData.TrainingRangeStart.Y - (MctData.mapSize.Y / 2);

		float comX = MctData.TrainingRangeSize.X * TileSize / 2.0f;
		float comY = MctData.TrainingRangeSize.Y * TileSize / 2.0f;

		float comX2 = ((int32)MctData.mapSize.X % 2 > 0) ? -1 * (TileSize * 0.5f) : 0;
		float comY2 = ((int32)MctData.mapSize.Y % 2 > 0) ? -1 * (TileSize * 0.5f) : 0;

		FVector pos = FVector(startX * TileSize + comX + comX2, startY * TileSize + comY + comY2, 0);
		pMapBoundaryActor->SetActorLocation(pos);

		float xSize = MctData.TrainingRangeSize.X;
		float ySize = MctData.TrainingRangeSize.Y;
		pMapBoundaryActor->SetActorScale3D(FVector(xSize, ySize, 10.f));

		return true;
	}

	return false;
}

bool ARtSceneManagerBase::AddActorModelFromTemplateBase(FString tctFilesPath, FTemplateBase base)
{
	TSharedPtr<RtActorModel>* pActorModel = ObjectModelMap.Find(base.TemplateModelKey);
	if (pActorModel == nullptr)
	{
		if (!base.TemplateModelKey.IsEmpty())
		{
			FString tctFilePath = tctFilesPath + base.TemplateModelKey + TEXT(".tct");
			if (!AddActorModel(base.TemplateModelKey, tctFilePath, true))
			{
				ErrorLog = FString::Printf(TEXT("AddActorModelFromTemplateBase Error : AddActorModel %s"), *base.TemplateModelKey);
				return false;
			}
		}
	}

	return true;
}

// mct를 로드하고 mct 내 모델들을 등록한다. 실제 스폰을 위한 준비 과정.
bool ARtSceneManagerBase::PrepareSpawnMapData(FString tctFilesPath, FString systemTctFilesPath, FString mctFilePath)
{
	FString mctPath = mctFilePath;
	MctData = LoadMCT(mctPath);

	bool bIsNewTctStruct = (MctData.Version > 0);

	ErrorLog = TEXT("");

	int32 LastSlashPos = INDEX_NONE;
	if (tctFilesPath.FindLastChar('/', LastSlashPos))
	{
		if (LastSlashPos+1 < tctFilesPath.Len())
		{
			tctFilesPath += TEXT("/");
		}
	}

	if (systemTctFilesPath.FindLastChar('/', LastSlashPos))
	{
		if (LastSlashPos + 1 < systemTctFilesPath.Len())
		{
			systemTctFilesPath += TEXT("/");
		}
	}	
	
	APP_LOG("PrepareSpawnMapData : systemTctFilesPath (%s)\n", *systemTctFilesPath);

	/// Tct 리뉴얼 데이터 로드 ///////////////////////////////////////////////////////////////////////////////

	if (bIsNewTctStruct) {
		// 일반 타입 등록.
		for (const auto& item : MctData.NormalTypeList)
		{
			if (!AddActorModelFromTemplateBase(tctFilesPath, item)) {
				return false;
			}
		}

		// 캐릭터 타입 등록.
		for (const auto& item : MctData.CharacterTypeList)
		{
			if (!AddActorModelFromTemplateBase(tctFilesPath, item)) {
				return false;
			}
		}

		// 장비 타입 등록.
		for (const auto& item : MctData.EquipmentTypeList)
		{
			if (!AddActorModelFromTemplateBase(tctFilesPath, item)) {
				return false;
			}
		}

		// 차량 타입 등록.
		for (const auto& item : MctData.CarTypeList)
		{
			if (!AddActorModelFromTemplateBase(tctFilesPath, item)) {
				return false;
			}
		}

		// 환경 타입 등록.
		for (const auto& item : MctData.EnvironmentTypeList)
		{
			if (!AddActorModelFromTemplateBase(tctFilesPath, item)) {
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	else {

		for (const FTemplateCharacter& charData : MctData.CharacterDatas)
		{
			TSharedPtr<RtActorModel>* pActorModel = ObjectModelMap.Find(charData.BasicInfo.TemplateModelKey);
			if (nullptr == pActorModel)
			{
				if (!charData.BasicInfo.TemplateModelKey.IsEmpty())
				{
					FString tctFilePath = tctFilesPath + charData.BasicInfo.TemplateModelKey + TEXT(".tct");
					if (!AddActorModel(charData.BasicInfo.TemplateModelKey, tctFilePath))
					{
						ErrorLog = FString::Printf(TEXT("PrepareSpawnMapData Character Error : AddActorModel %s"), *charData.BasicInfo.TemplateModelKey);
						return false;
					}
				}
				else
				{
					//				ensureMsgf(false, TEXT("tilesData.TileBasic.TemplateModelKey is empty"));
				}
			}
		}

		for (const FTemplateTile& tilesData : MctData.tilesDatas)
		{
			TSharedPtr<RtActorModel>* pActorModel = ObjectModelMap.Find(tilesData.TileBasic.TemplateModelKey);
			if (nullptr == pActorModel)
			{
				if (!tilesData.TileBasic.TemplateModelKey.IsEmpty())
				{
					FString tctFilePath = tctFilesPath + tilesData.TileBasic.TemplateModelKey + TEXT(".tct");
					if (!AddActorModel(tilesData.TileBasic.TemplateModelKey, tctFilePath))
					{
						ErrorLog = FString::Printf(TEXT("PrepareSpawnMapData Tile Error : AddActorModel %s"), *tilesData.TileBasic.TemplateModelKey);
						return false;
					}
				}
				else
				{
					//				ensureMsgf(false, TEXT("tilesData.TileBasic.TemplateModelKey is empty"));
				}
			}
		}

		for (const FTemplateWall& wallsData : MctData.wallsDatas)
		{
			TSharedPtr<RtActorModel>* pActorModel = ObjectModelMap.Find(wallsData.WallBasic.TemplateModelKey);
			if (nullptr == pActorModel)
			{
				if (!wallsData.WallBasic.TemplateModelKey.IsEmpty())
				{
					FString tctFilePath = tctFilesPath + wallsData.WallBasic.TemplateModelKey + TEXT(".tct");
					if (!AddActorModel(wallsData.WallBasic.TemplateModelKey, tctFilePath))
					{
						ErrorLog = FString::Printf(TEXT("PrepareSpawnMapData Wall Error : AddActorModel %s"), *wallsData.WallBasic.TemplateModelKey);
						return false;
					}
				}
				else
				{
					//				ensureMsgf(false, TEXT("wallsData.WallBasic.TemplateModelKey is empty"));
				}
			}
		}

		for (const FTemplateProps& propData : MctData.propsDatas)
		{
			TSharedPtr<RtActorModel>* pActorModel = ObjectModelMap.Find(propData.PropBasic.TemplateModelKey);
			if (nullptr == pActorModel)
			{
				if (!propData.PropBasic.TemplateModelKey.IsEmpty())
				{
					FString tctFilePath = tctFilesPath + propData.PropBasic.TemplateModelKey + TEXT(".tct");
					if (!AddActorModel(propData.PropBasic.TemplateModelKey, tctFilePath))
					{
						ErrorLog = FString::Printf(TEXT("PrepareSpawnMapData Prop Error : AddActorModel %s"), *propData.PropBasic.TemplateModelKey);
						return false;
					}
				}
				else
				{
					//			ensureMsgf(false, TEXT("propData.PropBasic.TemplateModelKey is empty"));
				}
			}
		}

		for (const FTemplateEquips& equipData : MctData.EquipsDatas)
		{
			TSharedPtr<RtActorModel>* pActorModel = ObjectModelMap.Find(equipData.WeaponBasic.TemplateModelKey);
			if (nullptr == pActorModel)
			{
				if (!equipData.WeaponBasic.TemplateModelKey.IsEmpty())
				{
					FString tctFilePath = tctFilesPath + equipData.WeaponBasic.TemplateModelKey + TEXT(".tct");
					if (!AddActorModel(equipData.WeaponBasic.TemplateModelKey, tctFilePath))
					{
						ErrorLog = FString::Printf(TEXT("PrepareSpawnMapData Equip Prop Error : AddActorModel %s"), *equipData.WeaponBasic.TemplateModelKey);
						return false;
					}
				}
				else
				{
					//			ensureMsgf(false, TEXT("propData.PropBasic.TemplateModelKey is empty"));
				}
			}
		}
	}

	// 시스템 파일들 로드.

	for (const FTemplatePlayerSpawn& spawnData : MctData.PlayerSpawnDatas) // 플레이어 시작 지점.
	{
		TSharedPtr<RtActorModel>* pActorModel = ObjectModelMap.Find(spawnData.SpawnBasic.TemplateModelKey);
		if (nullptr == pActorModel)
		{
			if (!spawnData.SpawnBasic.TemplateModelKey.IsEmpty())
			{
				if (!AddActorModelByJsonString(spawnData.SpawnBasic.TemplateModelKey, TctJsonString_PlayerStart, bIsNewTctStruct)) {
					ErrorLog = FString::Printf(TEXT("PrepareSpawnMapData PlayerSpawn Error : AddActorModel %s"), *spawnData.SpawnBasic.TemplateModelKey);
					return false;
				}
				else
				{
					break;
				}

				//FString tctFilePath = systemTctFilesPath + spawnData.SpawnBasic.TemplateModelKey + TEXT(".tct");
				//if (!AddActorModel(spawnData.SpawnBasic.TemplateModelKey, tctFilePath))
				//{
				//	ErrorLog = FString::Printf(TEXT("PrepareSpawnMapData PlayerSpawn Error : AddActorModel %s"), *spawnData.SpawnBasic.TemplateModelKey);
				//	return false;
				//}
			}
			else
			{
				//			ensureMsgf(false, TEXT("propData.PropBasic.TemplateModelKey is empty"));
			}
		}
	}

	for (const FTemplateWayPoint& wpData : MctData.WayPointDatas)
	{
		TSharedPtr<RtActorModel>* pActorModel = ObjectModelMap.Find(wpData.WayPointBasic.TemplateModelKey);
		if (nullptr == pActorModel)
		{
			if (!wpData.WayPointBasic.TemplateModelKey.IsEmpty())
			{
				if (!AddActorModelByJsonString(wpData.WayPointBasic.TemplateModelKey, TctJsonString_WayPoint, bIsNewTctStruct)) {
					ErrorLog = FString::Printf(TEXT("PrepareSpawnMapData WayPoint Error : AddActorModel %s"), *wpData.WayPointBasic.TemplateModelKey);
					return false;
				}
				else
				{
					break;
				}

				//FString tctFilePath = systemTctFilesPath + wpData.WayPointBasic.TemplateModelKey + TEXT(".tct");
				//if (!AddActorModel(wpData.WayPointBasic.TemplateModelKey, tctFilePath))
				//{
				//	ErrorLog = FString::Printf(TEXT("PrepareSpawnMapData WayPoint Error : AddActorModel %s"), *wpData.WayPointBasic.TemplateModelKey);
				//	return false;
				//}
			}
			else
			{
				//			ensureMsgf(false, TEXT("propData.PropBasic.TemplateModelKey is empty"));
			}
		}
	}

	for (const FTemplateInvisibleWall& iwData : MctData.InvisibleWallDatas)
	{
		TSharedPtr<RtActorModel>* pActorModel = ObjectModelMap.Find(iwData.InvisibleWallBasic.TemplateModelKey);
		if (nullptr == pActorModel)
		{
			if (!iwData.InvisibleWallBasic.TemplateModelKey.IsEmpty())
			{
				if (!AddActorModelByJsonString(iwData.InvisibleWallBasic.TemplateModelKey, TctJsonString_InvisibleWall, bIsNewTctStruct)) {
					ErrorLog = FString::Printf(TEXT("PrepareSpawnMapData InvisibleWall Error : AddActorModel %s"), *iwData.InvisibleWallBasic.TemplateModelKey);
					return false;
				}
				else
				{
					break;
				}

				//FString tctFilePath = systemTctFilesPath + iwData.InvisibleWallBasic.TemplateModelKey + TEXT(".tct");
				//if (!AddActorModel(iwData.InvisibleWallBasic.TemplateModelKey, tctFilePath))
				//{
				//	ErrorLog = FString::Printf(TEXT("PrepareSpawnMapData InvisibleWall Error : AddActorModel %s"), *iwData.InvisibleWallBasic.TemplateModelKey);
				//	return false;
				//}
			}
			else
			{
				//			ensureMsgf(false, TEXT("propData.PropBasic.TemplateModelKey is empty"));
			}
		}
	}

	for (const FTemplateTriggerBox& tbData : MctData.TriggerBoxDatas)
	{
		TSharedPtr<RtActorModel>* pActorModel = ObjectModelMap.Find(tbData.TriggerBoxBasic.TemplateModelKey);
		if (nullptr == pActorModel)
		{
			if (!tbData.TriggerBoxBasic.TemplateModelKey.IsEmpty())
			{
				if (!AddActorModelByJsonString(tbData.TriggerBoxBasic.TemplateModelKey, TctJsonString_TriggerBox, bIsNewTctStruct)) {
					ErrorLog = FString::Printf(TEXT("PrepareSpawnMapData TriggerBox Error : AddActorModel %s"), *tbData.TriggerBoxBasic.TemplateModelKey);
					return false;
				}
				else
				{
					break;
				}

				//FString tctFilePath = systemTctFilesPath + tbData.TriggerBoxBasic.TemplateModelKey + TEXT(".tct");
				//if (!AddActorModel(tbData.TriggerBoxBasic.TemplateModelKey, tctFilePath))
				//{
				//	ErrorLog = FString::Printf(TEXT("PrepareSpawnMapData TriggerBox Error : AddActorModel %s"), *tbData.TriggerBoxBasic.TemplateModelKey);
				//	return false;
				//}
			}
			else
			{
				//			ensureMsgf(false, TEXT("propData.PropBasic.TemplateModelKey is empty"));
			}
		}
	}

	RegistSystemModels(bIsNewTctStruct);

	return true;
}

//bool ARtSceneManagerBase::RegistMapBoundary()
//{
//	// 맵경계 객체를 생성 하기 위한 임의 등록.
//	//FString sysMapBoundaryFilePath = systemTctFilesPath + TEXT("/MapBoundary.tct");
//	//sysMapBoundaryFilePath = sysMapBoundaryFilePath.Replace(TEXT("//"), TEXT("/"));
//	//if (!AddActorModel(TEXT("MapBoundary"), sysMapBoundaryFilePath))
//
//	if (!AddActorModelByJsonString(TOOL_SYSTEM_FILE_NAME_MAP_BOUNDARY, TctJsonString_MapBoundery)) {
//		ErrorLog = FString::Printf(TEXT("ARtSceneManagerBase::RegistMapBoundary Error : AddActorModel MapBoundary"));
//		return false;
//	}
//
//	return true;
//}

bool ARtSceneManagerBase::RegistSystemModels(bool bIsNewTctStruct) {
	if (!AddActorModelByJsonString(TOOL_SYSTEM_FILE_NAME_PLAYER_START, TctJsonString_PlayerStart, bIsNewTctStruct)) {
		ErrorLog = FString::Printf(TEXT("ARtSceneManagerBase::RegistMapBoundary Error : AddActorModel PlayerStart"));
		return false;
	}
	if (!AddActorModelByJsonString(TOOL_SYSTEM_FILE_NAME_WAY_POINT, TctJsonString_WayPoint, bIsNewTctStruct)) {
		ErrorLog = FString::Printf(TEXT("ARtSceneManagerBase::RegistMapBoundary Error : AddActorModel WayPoint"));
		return false;
	}
	if (!AddActorModelByJsonString(TOOL_SYSTEM_FILE_NAME_INVISIBLE_WALL, TctJsonString_InvisibleWall, bIsNewTctStruct)) {
		ErrorLog = FString::Printf(TEXT("ARtSceneManagerBase::RegistMapBoundary Error : AddActorModel InvisibleWall"));
		return false;
	}
	if (!AddActorModelByJsonString(TOOL_SYSTEM_FILE_NAME_TRIGGER_BOX, TctJsonString_TriggerBox, bIsNewTctStruct)) {
		ErrorLog = FString::Printf(TEXT("ARtSceneManagerBase::RegistMapBoundary Error : AddActorModel TriggerBox"));
		return false;
	}
	if (!AddActorModelByJsonString(TOOL_SYSTEM_FILE_NAME_MAP_BOUNDARY, TctJsonString_MapBoundery, bIsNewTctStruct)) {
		ErrorLog = FString::Printf(TEXT("ARtSceneManagerBase::RegistMapBoundary Error : AddActorModel MapBoundary"));
		return false;
	}
	return true;
}

// tct 모델 추가
RtActorModel* ARtSceneManagerBase::AddActorModelByJsonString(FString modelKey, FString tctJsonString, bool bIsNewTctStruct)
{
	auto* pData = ObjectModelMap.Find(modelKey);
	if (pData == nullptr)
	{
		TSharedPtr<RtActorModel> actorModel = MakeShared<RtActorModel>();
		if (actorModel->LoadTCTJsonString(modelKey, tctJsonString, bIsNewTctStruct))
		{
			ObjectModelMap.Add(modelKey, actorModel);

			return actorModel.Get();
		}
	}
	else
	{
		return pData->Get();
	}

	return nullptr;
}

// tct 모델 추가
RtActorModel* ARtSceneManagerBase::AddActorModel(FString modelKey, FString tctFilePath, bool bIsNewTctStruct)
{
	auto* pData = ObjectModelMap.Find(modelKey);
	if( pData == nullptr )
	{
		TSharedPtr<RtActorModel> actorModel = MakeShared<RtActorModel>();
		if (actorModel->LoadTCT(tctFilePath, bIsNewTctStruct))
		{
			ObjectModelMap.Add(modelKey, actorModel);

			//URtActorModel2* pRtActorModel2 = NewObject<URtActorModel2>();
			//pRtActorModel2->LoadFromActorModel1(actorModel.Get());
			//ObjectModel2Map.Add(modelKey, pRtActorModel2);

			return actorModel.Get();
		}
	}
	else
	{
		return pData->Get();
	}

	return nullptr;
}

// 베이스 폴더의 모든 tct를 파일 이름을 모델 키로 하여 등록한다.
bool ARtSceneManagerBase::AutoAddActorModel(FString tctFilePathBase)
{
	TArray<FString> fileList;

	URtBluePrintCommonUtil::FindAllFileList(tctFilePathBase, fileList);

	for (FString& fileName : fileList)
	{
		FString filePath = tctFilePathBase + "\\" + fileName;
		TArray<FString> parseData;
		fileName.ParseIntoArray(parseData, TEXT("."));

		AddActorModel(parseData[0], filePath);
	}

	return true;
}

AActor* ARtSceneManagerBase::SpawnActor(FString modelKey, FString actorName, FVector vPos, FRotator rRot)
{
	return SpawnActorBase<ARtObjectActor>(modelKey, actorName, vPos, rRot, ProductType);
}

// 기본 액토 월드에 스폰.
AActor* ARtSceneManagerBase::SpawnActor(FString modelKey, FString actorName, float x, float y, float z, float pitch /*= 0*/, float yaw /*= 90*/, float roll /*= -90*/)
{
	return SpawnActorBase<ARtObjectActor>(modelKey, actorName, FVector(x, y, z), FRotator(pitch, yaw, roll), ProductType);
}

//AActor* ARtSceneManagerBase::AddSpawnNRegist(FString modelKey, FString actorName, FVector vPos, FRotator rRot, bool isForGame)
//{
//	ARtObjectActor * pObjectActor = Cast<ARtObjectActor>(SpawnActor(modelKey, actorName, vPos, rRot, isForGame));
//	if (pObjectActor && AddSpawnActor(pObjectActor, modelKey, vPos, rRot, isForGame))
//	{
//		return pObjectActor;
//	}
//
//	return nullptr;
//}

bool ARtSceneManagerBase::AddSpawnActor(ARtObjectActor* pObjectActor, FString modelKey, FVector vPos, FRotator rRot)
{
	return AddSpawnActor(pObjectActor, modelKey, vPos.X, vPos.Y, vPos.Z, rRot.Pitch, rRot.Yaw, rRot.Roll);
}

// 외부에서 스폰된 객체를 가져와 추가한다.
bool ARtSceneManagerBase::AddSpawnActor(ARtObjectActor* pObjectActor, FString modelKey, 
			float x, float y, float z, float pitch /*= 0*/, float yaw /*= 90*/, float roll /*= -90*/, bool isCreateModel,
			bool bIsNewTctStruct)
{
	TSharedPtr<RtActorModel>* pFoundModel = ObjectModelMap.Find(modelKey);
	if (pFoundModel != nullptr)
	{
		FActorSpawnParameters spawnParams;
		FRotator spawnRotator;

		if (nullptr != pObjectActor)
		{
			if( isCreateModel )
			{
				if (pObjectActor->Init(*pFoundModel))
				{
					if (!bIsNewTctStruct) {
						//기존 fbx로더 사용 코드
						pObjectActor->CreateModel("", ProductType);
					} else {
						pObjectActor->CreateModelRenew("", ProductType);
					}

					////gltf 파일 로드를 위한 코드
					//pObjectActor->CreateModelGLTF(pFoundModel->Get()->GetTctData()->mModelFilePath, ProductType);
				}
			}

			pObjectActor->SetActorLocation(FVector(x, y, z));
			pObjectActor->SetActorRotation(FRotator(pitch, yaw, roll));

			ObjectList.Add(pObjectActor);

			return true;
		}
	}

	return NULL;

}

// tct 모델 제거. 만일 해당 모델이 스폰되어 있다면 같이 제거 해 준다.
bool ARtSceneManagerBase::RemoveActorModel(FString modelKey)
{
	if (!ObjectModelMap.IsEmpty())
	{
		auto pFound = ObjectModelMap.Find(modelKey);
		if( NULL != pFound )
		{
			RtActorModel* pModel = (*pFound).Get();

			UWorld* pWorld = GetWorld();

			ObjectList.RemoveAll(
				[&](AActor* pActor) {

					ARtObjectActor* pObjActor = Cast<ARtObjectActor>(pActor);
					if( pObjActor && pObjActor->GetActorModel().Get() == pModel )
					{
						return pWorld->DestroyActor(pActor);
					}

					return false;
				}
			);

			//ObjectModel2Map.Remove(modelKey);

			return ObjectModelMap.Remove(modelKey) > 0;
		}
	}

	return false;
}

bool ARtSceneManagerBase::RemoveAllActorModel()
{
	ObjectModelMap.Empty();

	return true;
}

// 스폰된 특정 객체 제거.
bool ARtSceneManagerBase::DestroySpawnActor(ARtObjectActor* pTargetActor)
{
	if (nullptr != pTargetActor && 
		INDEX_NONE != ObjectList.Find(pTargetActor))
	{
		ObjectList.Remove(pTargetActor);

		UWorld* pWorld = GetWorld();
		pWorld->DestroyActor(pTargetActor);

		return true;
	}

	return false;
}

// 스폰된 모든 객체 제거.
void ARtSceneManagerBase::DestroyAllSpawnActor()
{
	// 툴에서 장비 장착을 지원하므로 장비를 장착한 객체들을 
	// 먼저 모두 떼어낸 후에 액토를 지워야 한다.
	for (auto& pActor : ObjectList)	{
		ARtObjectActor* pObjectActor = Cast<ARtObjectActor>(pActor);
		if (pObjectActor) {

			RtTctData* pTctData = pObjectActor->GetTct();
			if (pTctData) {
				if (pTctData->mTctType == ERtTctType::ECharacter) {
					TArray<AActor*> ChildActors;
					pObjectActor->GetAttachedActors(ChildActors);
					for (AActor* ChildActor : ChildActors)
					{
						ChildActor->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
					}
				}
			}
			else {
				FRtTctStruct* pTctStruct = pObjectActor->GetTctStruct();
				if (pTctStruct) {
					if (pTctStruct->ComDefault.TctType == ERtTctType2::ECharacter) {
						TArray<AActor*> ChildActors;
						pObjectActor->GetAttachedActors(ChildActors);
						for (AActor* ChildActor : ChildActors)
						{
							ChildActor->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
						}
					}
				}
			}
		}
	}

	for (auto& pActor : ObjectList)	{
		if (pActor.Get()) {
			pActor->Destroy();
		}
	}

	ObjectList.Reset(); 
}

FRtTctData ARtSceneManagerBase::GetTctDataByObjectName(FString objectName, bool& bIsSuccess)
{
	bIsSuccess = false;

	//TArray<AActor*> AllObjectActors;
	//	
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARtObjectActor::StaticClass(), AllObjectActors);
	//for (AActor* pActor : AllObjectActors)
	//{
	//	FString objectNameKey = GetComponentObjectNameKey(pActor);
	//	if( objectNameKey == objectName )
	//	{
	//		bIsSuccess = true;
	//		return GetTctDataByModelKey(objectNameKey, bIsSuccess);
	//	}
	//}

	// 객체가 생성되지 않는 경우를 대비하여 실제 데이터에서 검색.

	for (auto& data : MctData.propsDatas)
	{
		if (data.PropBasic.TemplateName == objectName)
		{
			return GetTctDataByModelKey(data.PropBasic.TemplateModelKey, bIsSuccess);
		}
	}
	
	for (auto& data : MctData.wallsDatas)
	{
		if (data.WallBasic.TemplateName == objectName)
		{
			return GetTctDataByModelKey(data.WallBasic.TemplateModelKey, bIsSuccess);
		}
	}

	for (auto& data : MctData.tilesDatas)
	{
		if (data.TileBasic.TemplateName == objectName)
		{
			return GetTctDataByModelKey(data.TileBasic.TemplateModelKey, bIsSuccess);
		}
	}

	for (auto& data : MctData.EquipsDatas)
	{
		if (data.WeaponBasic.TemplateName == objectName)
		{
			return GetTctDataByModelKey(data.WeaponBasic.TemplateModelKey, bIsSuccess);
		}
	}

	for (auto& data : MctData.CharacterDatas)
	{
		if (data.BasicInfo.TemplateName == objectName)
		{
			return GetTctDataByModelKey(data.BasicInfo.TemplateModelKey, bIsSuccess);
		}
	}

	return FRtTctData();
}

FRtTctData ARtSceneManagerBase::GetTctDataByModelKey(FString modelKey, bool& bIsSuccess)
{
	TSharedPtr<RtActorModel> Model = ObjectModelMap.FindRef(modelKey);
	if (Model.IsValid() && Model->GetNewTctData().Get() && Model->GetNewTctData().IsValid())		
	{
		bIsSuccess = true;
		return *Model->GetNewTctData().Get();
	}

//	TObjectPtr<URtActorModel2>* pModel2 = ObjectModel2Map.Find(modelKey);
	//if (pModel2)
	//{
	//	return (*pModel2)->GetTctData(bIsSuccess);
	//}

	bIsSuccess = false;
	return FRtTctData();
}

bool ARtSceneManagerBase::GetTctStructByModelKey(FString modelKey, FRtTctStruct & tctStruct)
{
	TSharedPtr<RtActorModel>* found = ObjectModelMap.Find(modelKey);
	if (found)
	{
		tctStruct = *(*found)->GetTctStruct().Get();
		return true;
	}

	return false;
}

bool ARtSceneManagerBase::GetTctDataByObjectNameInData(FString objectName, FRtTctStruct& tctStruct)
{
	if (DataObjectName2ModelKeyMap.IsEmpty())
	{
		for (auto& data : MctData.NormalTypeList)
		{
			DataObjectName2ModelKeyMap.Add(data.TemplateName, data.TemplateModelKey);
		}

		for (auto& data : MctData.CharacterTypeList)
		{
			DataObjectName2ModelKeyMap.Add(data.TemplateName, data.TemplateModelKey);
		}

		for (auto& data : MctData.EquipmentTypeList)
		{
			DataObjectName2ModelKeyMap.Add(data.TemplateName, data.TemplateModelKey);
		}

		for (auto& data : MctData.CarTypeList)
		{
			DataObjectName2ModelKeyMap.Add(data.TemplateName, data.TemplateModelKey);
		}
	}

	FString* found = DataObjectName2ModelKeyMap.Find(objectName);
	if (found) {
		return GetTctStructByModelKey(*found, tctStruct);
	}

	return false;
}

bool ARtSceneManagerBase::GetTctStructByObjectName(FString objectName, FRtTctStruct& tctStruct)
{
	if (!GetTctStructByObjectNameInList<FMapTemplateNormal>(MctData.NormalTypeList, objectName, tctStruct))
	{
		if (!GetTctStructByObjectNameInList<FMapTemplateCharacter>(MctData.CharacterTypeList, objectName, tctStruct))
		{
			if (!GetTctStructByObjectNameInList<FMapTemplateEquipment>(MctData.EquipmentTypeList, objectName, tctStruct))
			{
				if (!GetTctStructByObjectNameInList<FMapTemplateCar>(MctData.CarTypeList, objectName, tctStruct))
				{
					if (!GetTctStructByObjectNameInList<FMapTemplateEnvironment>(MctData.EnvironmentTypeList, objectName, tctStruct))
					{
						return false;
					}
				}
			}
		}
	}
	return true;

	//TArray<AActor*> AllObjectActors;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARtObjectActor::StaticClass(), AllObjectActors);
	//for (AActor* pActor : AllObjectActors) {
	//	FString objectNameKey = GetComponentObjectNameKey(pActor);
	//	if (objectNameKey == objectName)
	//	{
	//		return GetTctStructByModelKey(objectNameKey, tctStruct);
	//	}
	//}

	//return GetTctDataByObjectNameInData(objectName, tctStruct);
}

int ARtSceneManagerBase::GetTileSizeXFromModel(FString modelKey)
{
	TSharedPtr<RtActorModel> Model = ObjectModelMap[modelKey];
	int TileSizeX =  Model->GetTctData()->mTileX;
	return TileSizeX;
}

int ARtSceneManagerBase::GetTileSizeYFromModel(FString modelKey)
{
	TSharedPtr<RtActorModel> Model = ObjectModelMap[modelKey];
	int TileSizeY = Model->GetTctData()->mTileY;
	return TileSizeY;
}

ARtObjectActor* ARtSceneManagerBase::FindObjectByName(FString objectName)
{
	for (TObjectPtr<ARtObjectActor> pActor : ObjectList) {
		FString objectNameKey = GetComponentObjectNameKey(pActor.Get());
		if (objectNameKey == objectName)
		{
			return Cast<ARtObjectActor>(pActor);
		}

	}
	
	return nullptr;
}

bool ARtSceneManagerBase::DeleteObjectMctTemplateData(FString objectNameKey)
{
// TCT 리뉴얼 ////////////////////////////////////////////////////////////////////////////////////////////
	for (int32 i = MctData.NormalTypeList.Num() - 1; i >= 0; --i)
	{
		auto& data = MctData.NormalTypeList[i];
		if (data.TemplateName == objectNameKey)
		{
			MctData.NormalTypeList.RemoveAt(i);
			return true;
		}
	}

	for (int32 i = MctData.CharacterTypeList.Num() - 1; i >= 0; --i)
	{
		auto& data = MctData.CharacterTypeList[i];
		if (data.TemplateName == objectNameKey)
		{
			MctData.CharacterTypeList.RemoveAt(i);
			return true;
		}
	}

	for (int32 i = MctData.EquipmentTypeList.Num() - 1; i >= 0; --i)
	{
		auto& data = MctData.EquipmentTypeList[i];
		if (data.TemplateName == objectNameKey)
		{
			MctData.EquipmentTypeList.RemoveAt(i);
			return true;
		}
	}

	for (int32 i = MctData.CarTypeList.Num() - 1; i >= 0; --i)
	{
		auto& data = MctData.CarTypeList[i];
		if (data.TemplateName == objectNameKey)
		{
			MctData.CarTypeList.RemoveAt(i);
			return true;
		}
	}

	for (int32 i = MctData.EnvironmentTypeList.Num() - 1; i >= 0; --i)
	{
		auto& data = MctData.EnvironmentTypeList[i];
		if (data.TemplateName == objectNameKey)
		{
			MctData.EnvironmentTypeList.RemoveAt(i);
			return true;
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

	for (int32 i = MctData.CharacterDatas.Num() - 1; i >= 0; --i)
	{
		auto& data = MctData.CharacterDatas[i];
		if (data.BasicInfo.TemplateName == objectNameKey)
		{
			MctData.CharacterDatas.RemoveAt(i);
			return true;
		}
	}

	for (int32 i = MctData.propsDatas.Num() - 1; i >= 0; --i)
	{
		auto& data = MctData.propsDatas[i];
		if (data.PropBasic.TemplateName == objectNameKey)
		{
			MctData.propsDatas.RemoveAt(i);
			return true;
		}
	}

	for (int32 i = MctData.wallsDatas.Num() - 1; i >= 0; --i)
	{
		auto& data = MctData.wallsDatas[i];
		if (data.WallBasic.TemplateName == objectNameKey)
		{
			MctData.wallsDatas.RemoveAt(i);
			return true;
		}
	}

	for (int32 i = MctData.tilesDatas.Num() - 1; i >= 0; --i)
	{
		auto& data = MctData.tilesDatas[i];
		if (data.TileBasic.TemplateName == objectNameKey)
		{
			MctData.tilesDatas.RemoveAt(i);
			return true;
		}
	}

	for (int32 i = MctData.EquipsDatas.Num() - 1; i >= 0; --i)
	{
		auto& data = MctData.EquipsDatas[i];
		if (data.WeaponBasic.TemplateName == objectNameKey)
		{
			MctData.EquipsDatas.RemoveAt(i);
			return true;
		}
	}

	for (int32 i = MctData.PlayerSpawnDatas.Num() - 1; i >= 0; --i)
	{
		auto& data = MctData.PlayerSpawnDatas[i];
		if (data.SpawnBasic.TemplateName == objectNameKey)
		{
			MctData.PlayerSpawnDatas.RemoveAt(i);
			return true;
		}
	}

	for (int32 i = MctData.WayPointDatas.Num() - 1; i >= 0; --i)
	{
		auto& data = MctData.WayPointDatas[i];
		if (data.WayPointBasic.TemplateName == objectNameKey)
		{
			MctData.WayPointDatas.RemoveAt(i);
			return true;
		}
	}

	for (int32 i = MctData.InvisibleWallDatas.Num() - 1; i >= 0; --i)
	{
		auto& data = MctData.InvisibleWallDatas[i];
		if (data.InvisibleWallBasic.TemplateName == objectNameKey)
		{
			MctData.InvisibleWallDatas.RemoveAt(i);
			return true;
		}
	}

	for (int32 i = MctData.TriggerBoxDatas.Num() - 1; i >= 0; --i)
	{
		auto& data = MctData.TriggerBoxDatas[i];
		if (data.TriggerBoxBasic.TemplateName == objectNameKey)
		{
			MctData.TriggerBoxDatas.RemoveAt(i);
			return true;
		}
	}
	
	return false;
}

ENetMode ARtSceneManagerBase::GetNetMode()
{
	return GetWorld()->GetNetMode();
}

void ARtSceneManagerBase::PlayMovePosActor(FString ObjectNameKey, FVector StartPos, FVector EndPos, float PlayTime)
{
	auto* pFound = ObjectMoveDataMap.Find(ObjectNameKey);
	if(pFound==nullptr)
	{
		AActor* pFoundActor = FindObjectByName(ObjectNameKey);
		if(pFoundActor)
		{
			ObjectMoveDataMap.Add(ObjectNameKey, RtMoveAnimData(StartPos, EndPos, PlayTime, pFoundActor));
		}
	}
}

AActor* ARtSceneManagerBase::AddSpawnNRegistBase(FString modelKey, FString actorName, FVector vPos, FRotator rRot, bool isForGame)
{
	if (!GIsEditor)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddSpawnNRegist_1"));
		UE_LOG(LogTemp, Warning, TEXT("%s"), *modelKey);
	}


	TSharedPtr<RtActorModel>* pFoundModel = ObjectModelMap.Find(modelKey);
	if (pFoundModel)
	{

		if (!GIsEditor)
		{
			UE_LOG(LogTemp, Warning, TEXT("AddSpawnNRegist_2"));
		}
		if (pFoundModel->Get()->GetTctData()->mTagName == "character tag")
		{
			FActorSpawnParameters spawnParams;
			if (!actorName.IsEmpty()) {
				spawnParams.Name = FName(*actorName);
			}

			//UWorld* pWorld = GetWorld();
			//ARsNPC* pNpcActor = pWorld->SpawnActor<ARsNPC>(ARsNPC::StaticClass(), vPos, rRot, spawnParams);
			//if (pNpcActor)
			//{
			//	URtObjectActorComponent* pActorCom = Cast<URtObjectActorComponent>(pNpcActor->GetComponentByClass(URtObjectActorComponent::StaticClass()));
			//	if (pActorCom)
			//	{
			//		pActorCom->SetSkeletalMeshAsset(pFoundModel->Get()->GetTctData()->mModelFilePath);
			//	}

			//	NpcList.Add(pNpcActor);
			//	return pNpcActor;
			//}
		}
		else
		{
			ARtObjectActor* pObjectActor = Cast<ARtObjectActor>(SpawnActor(modelKey, actorName, vPos, rRot));

			if (!GIsEditor)
			{
				UE_LOG(LogTemp, Warning, TEXT("AddSpawnNRegist_3"));
			}

			//if (pObjectActor && AddSpawnActor(pObjectActor, modelKey, vPos, rRot, isForGame))
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("AddSpawnNRegist_4"));

			//	return pObjectActor;
			//}
		}
	}

	return nullptr;
}


bool ARtSceneManagerBase::SpawnMapDataBase(FString tctFilesPath, FString sysFilesPath, FString mctFilePath, bool isForGame, FRotator totalRotOffset, FVector totalPosOffset)
{
	if (!PrepareSpawnMapData(tctFilesPath, sysFilesPath, mctFilePath))
	{
		return false;
	}

	FQuat rotationQuat = totalRotOffset.Quaternion();
	FMatrix rotationMatrix = rotationQuat.ToMatrix();

	// 타일 스폰
	for (const FTemplateTile& tileData : MctData.tilesDatas)
	{
		AActor* pActor = AddSpawnNRegistBase(
			tileData.TileBasic.TemplateModelKey,
			tileData.TileBasic.TemplateName,
			tileData.TileBasic.TemplateLocation,
			tileData.TileBasic.TemplateRotation,
			isForGame);

		if (pActor)
		{
			SetComponentObjectNameKey(pActor, tileData.TileBasic.TemplateName);

			pActor->AddActorWorldRotation(totalRotOffset);
			FVector newLocation = rotationMatrix.TransformVector(tileData.TileBasic.TemplateLocation);
			newLocation += totalPosOffset;
			pActor->SetActorLocation(newLocation);

			//FRotator CurrentRotation = FRotator(0, 0, 0);
			//FVector CenterPoint(0, 0, 0);
			//FRotator RotatedRotation = CurrentRotation.RotateAroundAxis(CenterPoint, 45);
			//CurrentRotation.RotateVector()
			//		pActor->
			//		tileData.TileBasic.TemplateLocation
			//		rotationMatrix 
		}
	}

	// 벽 스폰
	for (const FTemplateWall& wallsData : MctData.wallsDatas)
	{
		AActor* pActor = AddSpawnNRegistBase(
			wallsData.WallBasic.TemplateModelKey,
			wallsData.WallBasic.TemplateName,
			wallsData.WallBasic.TemplateLocation,
			wallsData.WallBasic.TemplateRotation,
			isForGame);

		if (pActor)
		{
			SetComponentObjectNameKey(pActor, wallsData.WallBasic.TemplateName);

			pActor->SetActorScale3D(FVector(0.5f, 1.05f, 1.f));
			pActor->SetActorRelativeLocation(pActor->GetActorLocation() + FVector(-0.5f, -0.5f, 1.f));

			pActor->AddActorWorldRotation(totalRotOffset);
			FVector newLocation = rotationMatrix.TransformVector(wallsData.WallBasic.TemplateLocation);
			newLocation += totalPosOffset;
			pActor->SetActorLocation(newLocation);
		}
	}

	////return true;

	// 소품 스폰 ( 또는 캐릭터 )
	int propCounter = 0;
	for (const FTemplateProps& propData : MctData.propsDatas)
	{
		AActor* pActor = AddSpawnNRegistBase(
			propData.PropBasic.TemplateModelKey,
			propData.PropBasic.TemplateName,
			propData.PropBasic.TemplateLocation,
			propData.PropBasic.TemplateRotation,
			isForGame);

		if (pActor)
		{
			URtObjectOptionComponent* pObjectOptionCom = GetObjectOptionComponent(pActor);
			if (pObjectOptionCom)
			{
				pObjectOptionCom->SetObjectKeyName(propData.PropBasic.TemplateName);
				pObjectOptionCom->SetReferenceInfo(propData.PropBasic.TemplateModelKey, ERtTctType::EProp, ERtTctType2::ENone, propCounter);
			}

//			pActor->SetActorScale3D(FVector(1));

			pActor->AddActorWorldRotation(totalRotOffset);
			FVector newLocation = rotationMatrix.TransformVector(propData.PropBasic.TemplateLocation);
			newLocation += totalPosOffset;
			pActor->SetActorLocation(newLocation);

			propCounter++;
		}
	}

	//float MapBlockSize = 104.5f;
	//float MapBlockHalfSize = MapBlockSize * 0.5f;
	//UWorld* pWorld = GetWorld();
	//for (const FTemplateArea& areaData : mMCTTempeteData.AreasDatas)
	//{
	//	FActorSpawnParameters spawnParams;
	//	//if (!actorName.IsEmpty()) {
	//	//	spawnParams.Name = FName(*actorName);
	//	//}
	//	//FRotator spawnRotator;
	//	//FVector spawnLocation = FVector::ZeroVector;
	//	//spawnLocation.Set(x, y, z);

	//	ARsRegionTriggerBox* pTBox = pWorld->SpawnActor<ARsRegionTriggerBox>(ARsRegionTriggerBox::StaticClass(), areaData.AreaBasic.TemplateLocation, areaData.AreaBasic.TemplateRotation, spawnParams);
	//	if (pTBox)
	//	{
	//		pTBox->AreaNameList = areaData.aAreaTag;

	//		Cast<UBoxComponent>(pTBox->GetRootComponent())->SetBoxExtent(FVector(MapBlockHalfSize, MapBlockHalfSize, MapBlockHalfSize));
	//	}

	//	pTBox->AddActorWorldRotation(totalRotOffset);
	//	FVector newLocation = rotationMatrix.TransformVector(areaData.AreaBasic.TemplateLocation);
	//	newLocation += totalPosOffset;
	//	pTBox->SetActorLocation(newLocation);

	//	//		OnSpawnRegionTriggerBox.Broadcast(areaData);
	//}

	return true;
}

bool ARtSceneManagerBase::LoadSimpleProjectMapSceneBase(FString subProjectDirName, FString subSysDirName, FString mapFileName, FRotator totalRotOffset, FVector totalPosOffset)
{
	FString baseDir;

#if WITH_EDITOR
	baseDir = FPaths::ProjectDir();
#else
	baseDir = FPaths::LaunchDir();
#endif

	FString projectPath = baseDir + subProjectDirName + TEXT("/");
	FString systemPath = baseDir + subSysDirName + TEXT("/");
	systemPath = systemPath.Replace(TEXT("\\"), TEXT("/"));

	projectPath = FPaths::ConvertRelativePathToFull(projectPath);
	systemPath = FPaths::ConvertRelativePathToFull(systemPath);

	FString tctFilesPath = projectPath + ARtSceneManagerBase::TctSubFolderName;
	FString mapFilePath = projectPath + ARtSceneManagerBase::MapSubFolderName + TEXT("/") + mapFileName;

	return SpawnMapDataBase(tctFilesPath, systemPath, mapFilePath, true, totalRotOffset, totalPosOffset);
}

bool ARtSceneManagerBase::LoadSimpleProjectScenarioSceneBase(FString subProjectDirName, FString subSysDirName, FString scenarioFileName, FRotator totalRotOffset, FVector totalPosOffset)
{
	FString baseDir;

#if WITH_EDITOR
	baseDir = FPaths::ProjectDir();
#else
	baseDir = FPaths::LaunchDir();
#endif

	FString projectPath = baseDir + subProjectDirName + TEXT("/");
	FString systemPath = baseDir + subSysDirName + TEXT("/");

	projectPath = FPaths::ConvertRelativePathToFull(projectPath);
	systemPath = FPaths::ConvertRelativePathToFull(systemPath);

	FString tctFilesPath = projectPath + ARtSceneManagerBase::TctSubFolderName;

	FString scenarioFilePath = projectPath + ARtSceneManagerBase::MapSubFolderName + TEXT("/") + scenarioFileName;

	return LoadScenarioSceneBase(tctFilesPath, systemPath, scenarioFilePath, false, totalRotOffset, totalPosOffset);
}

bool ARtSceneManagerBase::LoadScenarioSceneBase(FString tctFilesPath, FString sysFilesPath, FString scenarioFilePath, bool bIsFireZone, FRotator totalRotOffset, FVector totalPosOffset)
{
	ARtScenarioControllerBase* pScenarioController = ARtScenarioControllerBase::Get(GetWorld());

	UE_LOG(LogTemp, Warning, TEXT("LoadScenarioScene_1"));

	tctFilesPath = tctFilesPath.Replace(TEXT("\\"), TEXT("/"));
	sysFilesPath = sysFilesPath.Replace(TEXT("\\"), TEXT("/"));
	scenarioFilePath = scenarioFilePath.Replace(TEXT("\\"), TEXT("/"));
	scenarioFilePath = scenarioFilePath.Replace(TEXT("//"), TEXT("/"));

	if (!GIsEditor)
	{
		FString folderPath = FPaths::LaunchDir();
		TArray<FString> Paths;

		folderPath.ParseIntoArray(Paths, TEXT("/"));
		folderPath = Paths[0].Append("\\MapFiles\\");

		TArray<FString> ScenarioPathsArray;
		scenarioFilePath.ParseIntoArray(ScenarioPathsArray, TEXT("MapFiles/"));

		UE_LOG(LogTemp, Warning, TEXT("ScenarioPathsArray[0] : %s"), *ScenarioPathsArray[0]);
		UE_LOG(LogTemp, Warning, TEXT("ScenarioPathsArray[1] : %s"), *ScenarioPathsArray[1]);

		scenarioFilePath = folderPath.Append(ScenarioPathsArray[1]);

		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, scenarioFilePath);
		UE_LOG(LogTemp, Warning, TEXT("LoadScenarioScene_2"));
		UE_LOG(LogTemp, Warning, TEXT("scenarioFilePath : %s"), *scenarioFilePath);
	}

	if (pScenarioController && pScenarioController->LoadSCTData(scenarioFilePath))
	{
		pScenarioController->SetSceneManagerRef(this);

		//// 플레이어 캐릭터 설정.
		//TArray<AActor*> foundActors;
		//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARsCharacter::StaticClass(), foundActors);
		//if (foundActors.Num() > 0) // 일단 멀티 개념은 나중에 생각한다.
		//{
		//	pScenarioController->SetPlayerActor(Cast<ARsCharacter>(foundActors[0]));
		//}

		FString frontPath = FPaths::GetPath(scenarioFilePath);
		FString mapFilePath = frontPath + "/" + pScenarioController->ScenarioModel->SctData.MctFileName;

		if (!GIsEditor)
		{
			FString folderPath = FPaths::LaunchDir();
			TArray<FString> Paths;

			folderPath.ParseIntoArray(Paths, TEXT("/"));
			folderPath = Paths[0].Append("\\MapFiles\\");

			TArray<FString> mapFilePathsArray;
			mapFilePath.ParseIntoArray(mapFilePathsArray, TEXT("MapFiles/"));

			mapFilePath = folderPath.Append(mapFilePathsArray[1]);

			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::White, mapFilePath);

			FString TCTfolderPath;
			TCTfolderPath = Paths[0].Append("TCTFiles\\");

			tctFilesPath = TCTfolderPath;

			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, tctFilesPath);
			UE_LOG(LogTemp, Warning, TEXT("LoadScenarioScene_3"));
			UE_LOG(LogTemp, Warning, TEXT("tctFilesPath : %s"), *tctFilesPath);
			//return false;
		}

		//FRotator totalRotDelta = FRotator(0, 90, 0);
		//FVector totalPosDelta = FVector(200, 100, 0);
		// 
		UE_LOG(LogTemp, Warning, TEXT("LoadScenarioScene_4"));
		// 
		//totalRotOffset = FRotator(0, 0, 0);
//		totalPosOffset = FVector(0,0,0);

		if (SpawnMapDataBase(tctFilesPath, sysFilesPath, mapFilePath, true, totalRotOffset, totalPosOffset))
		{
			//if (pScenarioController->InitializeScenario())
			{
				//if (bIsFireZone)
				//{
				//	// xrdev mod
				//	ARtHeatmapRM* pHeatMap = Cast<ARtHeatmapRM>(UGameplayStatics::GetActorOfClass(GetWorld(), ARtHeatmapRM::StaticClass()));
				//	if (pHeatMap)
				//	{
				//		float tileSize = 104.5f;
				//		float tileSizeHalf = tileSize * 0.5f;
				//		float mapSizeX = mMCTTempeteData.mapSize.X;
				//		float mapSizeY = mMCTTempeteData.mapSize.Y;
				//		float offsetXSize = (tileSize * mapSizeX) * 0.5f;// + totalPosOffset.X;
				//		float offsetYSize = (tileSize * mapSizeY) * 0.5f;// + totalPosOffset.Y;

				//		//FVector heatMapPos = pHeatMap->GetActorLocation();
				//		//FQuat rotationQuat = totalRotOffset.Quaternion();
				//		//FMatrix rotationMatrix = rotationQuat.ToMatrix();
				//		//FVector newLocation = rotationMatrix.TransformVector(heatMapPos);
				//		//newLocation += totalPosOffset;
				//		//pHeatMap->SetActorLocation(newLocation);

				//		pHeatMap->SetHeatBoxCellSize(mapSizeX, mapSizeY, 3);

				//		pHeatMap->AddActorWorldOffset(FVector(-offsetXSize + totalPosOffset.X + tileSizeHalf, -offsetYSize + totalPosOffset.Y + tileSizeHalf, 0));

				//		pHeatMap->SetOffset(totalRotOffset, totalPosOffset);

				//		//						pHeatMap->InitializeHeatmap();
				//		//						pHeatMap->RegisterNewBoxOwners();
				//		//						
				//		//						//{
				//		//						//	// 현재 히트맵에 버그가 있음. 회전을 안시켜도 적용되어야 맞는데 꼭 회전을 시켜야 되는 문제가 있음.
				//		//						//	// 아래 코드는 이러한 문제를 회피하기 위해 임시로 회전과 오프셋을 적용한 코드임.
				//		//						//	FVector heatMapPos = pHeatMap->GetActorLocation();
				//		//						//	FQuat rotationQuat = totalRotOffset.Quaternion();
				//		//						//	FMatrix rotationMatrix = rotationQuat.ToMatrix();
				//		//						//	FVector newLocation = rotationMatrix.TransformVector(heatMapPos);
				//		//						//	newLocation += totalPosOffset;
				//		//						//	pHeatMap->SetActorLocation(newLocation);
				//		//
				//		//						//	pHeatMap->AddActorWorldOffset(FVector(-offsetXSize, -offsetYSize, 0));
				//		//						//}
				//		//
				//	}

				//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//	// 소방용 하드 코딩
				//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				//	//foundActors.Empty();
				//	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), foundActors);

				//	//for (AActor* actor : foundActors)
				//	//{
				//	//	if (actor->GetName() == "door01")
				//	//	{
				//	//		//actor->SetActorHiddenInGame(true);

				//	//		USkeletalMeshComponent* pSkeletalMeshCom = Cast<USkeletalMeshComponent>(actor->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
				//	//		if (pSkeletalMeshCom)
				//	//		{
				//	//			pSkeletalMeshCom->SetVisibility(false); // 메시만 안보이게 처리.
				//	//		}

				//	//		//							actor->GetComponentsByClass()
				//	//		break;
				//	//	}
				//	//}

				//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				//}

				//auto* pGameManager = GetGameInstance()->GetSubsystem<URsGameManager>();
				//if (pGameManager)
				//{
				//	pGameManager->TestEventMessage.Broadcast();
				//}

				return true;
			}
		}
	}

	return false;
}

// ERtTctType::EPlayerSpawn
// 객체 타입 별 비지블 설정.
void ARtSceneManagerBase::ObjectAllVisibleByTctType(bool visible, ERtTctType tctType)
{
	for (TObjectPtr<ARtObjectActor> pActor : ObjectList) {
		if(ARtObjectActor* pObjectActor = pActor.Get())
		{
			if (pObjectActor->GetTctType() == tctType)
			{
				pObjectActor->SetVisible(visible);
			}
		}
	}
}

void ARtSceneManagerBase::AllSwitchCollision(bool isForGame)
{
	for (TObjectPtr<ARtObjectActor> pActor : ObjectList) {
		if(ARtObjectActor* pObjectActor = pActor.Get())
		{
			pObjectActor->SwitchCollision(isForGame);
		}
	}
}

void ARtSceneManagerBase::SetScenarioControllerRef(ARtScenarioControllerBase * scenarioController)
{
	if(scenarioController)
	{
		ScenarioControllerRef = scenarioController;
	}
}

void ARtSceneManagerBase::ApplyExponentialHeightFog()
{
	TArray<AActor*> foundFogs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AExponentialHeightFog::StaticClass(), foundFogs);

	if (foundFogs.Num() > 0)
	{
		AExponentialHeightFog* Fog = Cast<AExponentialHeightFog>(foundFogs[0]);
		if (Fog && Fog->GetComponent())
		{
			UExponentialHeightFogComponent* FogComponent = Fog->GetComponent();

			if (FogComponent)
			{
				// 안개 밀도 조정
				FogComponent->SetFogDensity(MctData.ExponentialHeightFogData.FogDensity);

				// 안개의 높이 기반 감소 정도
				FogComponent->SetFogHeightFalloff(MctData.ExponentialHeightFogData.FogHeightFallOut);

				// 안개 색상 설정
				FogComponent->SetFogInscatteringColor(MctData.ExponentialHeightFogData.FogInscatteringColor);

				// 안개 시작 거리
				FogComponent->StartDistance = MctData.ExponentialHeightFogData.StartDistance;

				// 안개 표시 여부
				FogComponent->SetVisibility(MctData.ExponentialHeightFogData.Visible);

				// Directional Inscattering 설정
				//FogComponent->SetDirectionalInscatteringColor(FLinearColor::White);
				//FogComponent->SetDirectionalInscatteringExponent(4.0f);
				//FogComponent->SetDirectionalInscatteringStartDistance(3000.0f);

				// 변경사항 반영
				FogComponent->MarkRenderStateDirty();
			}
		}
	}
}

// Called when the game starts or when spawned
void ARtSceneManagerBase::BeginPlay()
{
	Super::BeginPlay();

	//AddActorModel("wall1", "C:/work/Dev/Rt/TCTData/wall_1.json");
	//
	//SpawnActor("wall1", 100, 100, 60, 0, 90, -90);
	//SpawnActor("wall1", 200, 200, 60, 0, 90, -90);

	if (URtGlobalActorManager* gam = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<URtGlobalActorManager>()) {
		FString key = TEXT("SceneManager");
		gam->ManagerMap.Add({ key, this });
	}
}

// Called every frame
void ARtSceneManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<FString> RemoveKeyArray;

	for(auto & data : ObjectMoveDataMap)
	{
		if (data.Value.Update(DeltaTime))
		{
			data.Value.TargetActor->SetActorLocation(data.Value.ProgressPos);
		}
		else
		{
			RemoveKeyArray.Add(data.Key);
		}
	}

	if (RemoveKeyArray.Num() > 0)
	{
		for (const auto& data : RemoveKeyArray)
		{
			ObjectMoveDataMap.Remove(data);
		}
	}
}

ARtSceneManagerBase* ARtSceneManagerBase::Get(UObject* worldContext)
{
	if (URtGlobalActorManager* gam = UGameplayStatics::GetGameInstance(worldContext)->GetSubsystem<URtGlobalActorManager>()) {
		FString key = TEXT("SceneManager");

		if (AActor* findActor = gam->ManagerMap.FindRef(key)) {
			if( IsValid(findActor) )
				return Cast<ARtSceneManagerBase>(findActor);
		}
		if (AActor* findActor = UGameplayStatics::GetActorOfClass(worldContext, ARtSceneManagerBase::StaticClass())) {
			if (IsValid(findActor))
				return Cast<ARtSceneManagerBase>(findActor);
		}
	}

	return nullptr;
}

URtObjectOptionComponent* ARtSceneManagerBase::GetObjectOptionComponent(AActor* pActor)
{
	if (pActor)
	{
		URtObjectOptionComponent* pObjectOptionCom = Cast<URtObjectOptionComponent>(pActor->GetComponentByClass(URtObjectOptionComponent::StaticClass()));
		return pObjectOptionCom;
	}

	return nullptr;
}

FString ARtSceneManagerBase::GetComponentObjectNameKey(AActor * pActor)
{
	if (pActor)
	{
		URtObjectOptionComponent* pObjectOptionCom = Cast<URtObjectOptionComponent>(pActor->GetComponentByClass(URtObjectOptionComponent::StaticClass()));
		if (pObjectOptionCom)
		{
			return pObjectOptionCom->GetObjectKeyName();
		}
	}

	return TEXT("");
}

bool ARtSceneManagerBase::SetComponentObjectNameKey(AActor* pActor, FString objectNameKey)
{
	if(pActor && !objectNameKey.IsEmpty())
	{
		URtObjectOptionComponent* pObjectOptionCom = Cast<URtObjectOptionComponent>(pActor->GetComponentByClass(URtObjectOptionComponent::StaticClass()));
		if (pObjectOptionCom)
		{
			pObjectOptionCom->SetObjectKeyName(objectNameKey);
			return true;
		}
	}

	return false;
}

bool ARtSceneManagerBase::SetComponentObjectReferenceInfo(AActor* pActor, FString modelKey, ERtTctType tctType, ERtTctType2 tctType2, int32 index)
{
	if (pActor && !modelKey.IsEmpty())
	{
		URtObjectOptionComponent* pObjectOptionCom = Cast<URtObjectOptionComponent>(pActor->GetComponentByClass(URtObjectOptionComponent::StaticClass()));
		if (pObjectOptionCom)
		{
			pObjectOptionCom->SetReferenceInfo(modelKey, tctType, tctType2, index);
			return true;
		}
	}

	return false;
}
