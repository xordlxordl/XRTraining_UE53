// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/RtGameSceneManager.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/DirectionalLight.h"
#include "Engine/StaticMeshActor.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

//#include "Framework/RtNpcPawn.h"
#include "Framework/RtNpcObjectActor.h"
#include "Framework/RtTraineePawn.h"
#include "Framework/RtGuideHand.h"
#include "Framework/RtWidgetActor_ListenIcon.h"
#include "Manager/RtGameScenarioController.h"
#include "Manager/RtLoadingManager.h"
#include "Manager/RtReplayManagerActor.h"

#include "RtDefines.h"
#include "RtBluePrintCommonUtil.h"
#include "Manager/RtConfig.h"
#include "Common/RtBlueprintAssetPath.h"

#include "Misc/MessageDialog.h"
#include "Sound/SoundWave.h"
#include "Components/AudioComponent.h"

#include "glTFRuntimeFBXAssetActor.h"
#include <JsonObjectConverter.h>

#include "Manager/RtGameManager.h"

ARtGameSceneManager::ARtGameSceneManager()
{
	Init(ERtProductType::EGame);
}

void ARtGameSceneManager::BeginPlay()
{
	Super::BeginPlay();

	UGameInstance* GameInstance = GetGameInstance();
	//URtFbxLoaderSubsystem::InitSubSystem(GameInstance);

	AsyncMapLoadInfoArraySize = 0;

}

AActor* ARtGameSceneManager::AddSpawnNRegist(FRtObjectSpawnData spawnData)
//ERtMctSpawnType mctSpawnType, FString modelKey, FString actorName, FVector vPos, FRotator rRot, 
//FVector totalPosOffset, FRotator totalRotOffset, FMatrix rotationMatrix)
{
	if (!GIsEditor)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddSpawnNRegist_1"));
		UE_LOG(LogTemp, Warning, TEXT("%s"), *spawnData.ModelKey);
	}

	TSharedPtr<RtActorModel>* pFoundModel = ObjectModelMap.Find(spawnData.ModelKey);
	if (pFoundModel)
	{
		ARtObjectActor* pActor = nullptr;
		switch (spawnData.SpawnType)
		{
		case ERtMctSpawnType::EPC:
		{
			ARtObjectActor* PcPawn = SpawnActorBase<ARtObjectActor>(spawnData.ModelKey, spawnData.ObjectKeyName,
				spawnData.Location, spawnData.Rotation, ERtProductType::EGame);
			pActor = PcPawn;
		}
		break;

		case ERtMctSpawnType::ENPC:
		{
			ARtNpcObjectActor* pNpcPawn = SpawnActorBase<ARtNpcObjectActor>(spawnData.ModelKey, spawnData.ObjectKeyName,
				spawnData.Location, spawnData.Rotation, ERtProductType::EGame);
			pActor = pNpcPawn;

			pNpcPawn->CreateAICtrller();
			pNpcPawn->InitNpc();
		}
		break;

		default:
		{
			ARtObjectActor* pObjectActor = Cast<ARtObjectActor>(SpawnActor(spawnData.ModelKey, spawnData.ObjectKeyName, spawnData.Location, spawnData.Rotation));
			if (pObjectActor && AddSpawnActor(pObjectActor, spawnData.ModelKey, spawnData.Location, spawnData.Rotation))
			{
				pActor = pObjectActor;
			}
		}
		break;
		}

		if (pActor)
		{
			// �ɼ� ������Ʈ �̸� ����.
			SetComponentObjectNameKey(pActor, spawnData.ObjectKeyName);

			////////////////////////////////////////////////////////////////////
			// ��ü ��ġ ���� �� ��ü ȸ�� ����
			////////////////////////////////////////////////////////////////////
			pActor->AddActorWorldRotation(spawnData.TotalRotOffset);
			FVector newLocation = spawnData.TotalRotationMatrix.TransformVector(spawnData.Location);
			newLocation += spawnData.TotalPosOffset;
			pActor->SetActorLocation(newLocation);
			////////////////////////////////////////////////////////////////////

			FRtTctStruct* pTctStruct = pActor->GetTctStruct();
			if (pTctStruct)
			{
				// �ɼ� ������Ʈ�� ���� ����.
				SetComponentObjectReferenceInfo(pActor, spawnData.ModelKey, ERtTctType::ENone, pTctStruct->ComDefault.TctType, 0);

				pActor->SetActorScale3D(spawnData.Scale);

				switch (pTctStruct->ComDefault.TctType)
				{
				case ERtTctType2::ENormal:
				{

				}
				break;
				case ERtTctType2::EEquipment:
				{

				}
				break;
				default:
					// �ý��� Ÿ���� ������ �ʰ� ó��.
					if (pTctStruct->ComDefault.TctType >= ERtTctType2::EPlayerSpawn)
					{
						pActor->SetVisible(false);
					}
					break;
				}

				// Add SoundWaves
				const TArray<FRtTctComDirection>& directionData = pTctStruct->ComDirectionList;
				if (!directionData.IsEmpty())
				{

					TArray<FRtTctComDirectionItem> directionItemList = directionData[0].DirectionItemList;
					if (!directionItemList.IsEmpty())
					{
						for (const FRtTctComDirectionItem& data : directionItemList)
						{
							FString soundFileName = data.SoundFileName;
							if (USoundWave* wave = SoundWaveMap.FindRef(soundFileName))
							{
								pActor->AddSoundWave(wave);

								if (UGameInstance* gi = GetGameInstance())
								{
									if (URtConfig* config = gi->GetSubsystem<URtConfig>())
									{
										pActor->SetAttenuation(config->TrainingOptions.TCT_Volume, config->TrainingOptions.TCT_AttenuationStart, config->TrainingOptions.TCT_AttenuationEnd);
									}
								}
							}
						}
					}
				}

			}
			else // ������ ��
			{
				// �ɼ� ������Ʈ�� ���� ����.
				TSharedPtr<RtTctData> tctData = (*pFoundModel)->GetTctData();
				SetComponentObjectReferenceInfo(pActor, spawnData.ModelKey, tctData.Get()->mTctType, ERtTctType2::ENone, 0);

				// Ÿ�Ժ� �߰� ó��.
				switch (tctData.Get()->mTctType)
				{
				case ERtTctType::EProp:
				{
					pActor->SetActorScale3D(FVector(1));

				}
				break;
				case ERtTctType::EEquipProp:
				{
					pActor->SetActorScale3D(FVector(1));
				}
				break;

				default:
					// �ý��� Ÿ���� ������ �ʰ� ó��.
					if (tctData.Get()->mTctType >= ERtTctType::EPlayerSpawn)
					{
						pActor->SetVisible(false);
						if (tctData.Get()->mTctType == ERtTctType::EWayPoint)
						{
							pActor->GetCurrentMesh()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
						}
					}
					break;
				}
			}

			return pActor;
		}
	}

	FText errorTitle = FText::FromString("Spawn Fail");
	FString errorMsg = spawnData.ObjectKeyName;
	CUSTOM_FILE_APP_LOG_EX("App.log", "%s: %s", *errorTitle.ToString(), *errorMsg);
	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(errorMsg), &errorTitle);

	return nullptr;
}

// * ������ �󿡼��� �ַ�� ������ �ִ� ���� ����.
// * ���� �Ŀ��� ���������� �ִ� ���� ����.
// 
// subProjectDirName : ������Ʈ ��� ���� ���
// subSysDirName : �ý��� ���� ��� ���� ���
// scenarioFileName : �ó����� ���� �̸�
bool ARtGameSceneManager::LoadSimpleProjectScenarioScene(FString subProjectDirName, FString subSysDirName, FString scenarioFileName, FRotator totalRotOffset, FVector totalPosOffset)
{
	if (ProductType == ERtProductType::ENone)
	{
		return false;
	}

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

	return LoadScenarioScene(tctFilesPath, systemPath, scenarioFilePath, false, totalRotOffset, totalPosOffset);
}

// tctFilesPath : tct ���� ���� ���� ���..
// sysFilesPath : �ý��� ���� ���� ���� ���..
// scenarioFilePath : �ó����� ���� ���� ���..
// bIsFireZone : �ҹ� �ϵ��ڵ�
// totalRotOffset : �� ��ü ȸ�� ��.
// totalPosOffset : �� ��ü ��ġ ��.
// 
// �ó����� ������ �������� ���� �����Ѵ�.
// 1. sct -> mct -> tct ���ϵ� �ε�
// 2. ��� �� ��ü�� ���� ( �� ���� )
// 3. �ó����� ������ ���� �ʱ�ȭ �۾�.
bool ARtGameSceneManager::LoadScenarioScene(FString tctFilesPath, FString sysFilesPath, FString scenarioFilePath, bool bIsFireZone, FRotator totalRotOffset, FVector totalPosOffset)
{
	if (ProductType == ERtProductType::ENone)
	{
		return false;
	}

	ARtGameScenarioController* pScenarioController = ARtGameScenarioController::Get(GetWorld());

	/////////////////////////////////////////////////////////////////////////
	// �̹� �ε�� �����͵� ��� UnLoad
	/////////////////////////////////////////////////////////////////////////
	UnloadMct();
	pScenarioController->UnloadSct();
	/////////////////////////////////////////////////////////////////////////

	UE_LOG(LogTemp, Warning, TEXT("LoadScenarioScene_1"));

	tctFilesPath = tctFilesPath.Replace(TEXT("\\"), TEXT("/"));
	sysFilesPath = sysFilesPath.Replace(TEXT("\\"), TEXT("/"));
	scenarioFilePath = scenarioFilePath.Replace(TEXT("\\"), TEXT("/"));

	tctFilesPath = tctFilesPath.Replace(TEXT("//"), TEXT("/"));
	sysFilesPath = sysFilesPath.Replace(TEXT("//"), TEXT("/"));
	scenarioFilePath = scenarioFilePath.Replace(TEXT("//"), TEXT("/"));

	CurrentSctFileName = FPaths::GetBaseFilename(scenarioFilePath);

	int32 dotIndex;
	if (CurrentSctFileName.FindChar('.', dotIndex))
	{
		CurrentSctFileName = CurrentSctFileName.Left(dotIndex);
	}

	CUSTOM_FILE_APP_LOG_EX("App.log", "LoadSCTData %s", *scenarioFilePath);

	// Sct�� ������ ������ ���� �о� ���δ�.
	if (pScenarioController && pScenarioController->LoadSctOnlyDataStruct(scenarioFilePath))
	{
		pScenarioController->SetSceneManagerRef(this);
		CUSTOM_FILE_APP_LOG_EX("App.log", "LoadSCTData Ok~");

		FString frontPath = FPaths::GetPath(scenarioFilePath);
		FString mapFilePath = frontPath + "/" + pScenarioController->ScenarioModel->SctData.MctFileName;

		UE_LOG(LogTemp, Warning, TEXT("LoadScenarioScene_4"));

		CUSTOM_FILE_APP_LOG_EX("App.log", "tctFilesPath : %s", *tctFilesPath);
		CUSTOM_FILE_APP_LOG_EX("App.log", "sysFilesPath : %s", *sysFilesPath);
		CUSTOM_FILE_APP_LOG_EX("App.log", "mapFilePath : %s", *mapFilePath);

		// ���� ���� �ε��ϰ� �ó������� �ε��ϰ� �����Ѵ�. �⺻ �ִϸ��̼� ������ ����..
		if (SpawnMapData(tctFilesPath, sysFilesPath, mapFilePath, totalRotOffset, totalPosOffset))
		{
			if (pScenarioController && pScenarioController->LoadSCTData(scenarioFilePath))
			{
				CUSTOM_FILE_APP_LOG_EX("App.log", "SpawnMapData Ok~");
				return true;
			}
		}
	}

	//if (pScenarioController && pScenarioController->LoadSCTData(scenarioFilePath))
	//{
	//	pScenarioController->SetSceneManagerRef(this);
	//	CUSTOM_FILE_APP_LOG_EX("App.log", "LoadSCTData Ok~");

	//	FString frontPath = FPaths::GetPath(scenarioFilePath);
	//	FString mapFilePath = frontPath + "/" + pScenarioController->ScenarioModel->SctData.MctFileName;

	//	UE_LOG(LogTemp, Warning, TEXT("LoadScenarioScene_4"));

	//	CUSTOM_FILE_APP_LOG_EX("App.log", "tctFilesPath : %s", *tctFilesPath);
	//	CUSTOM_FILE_APP_LOG_EX("App.log", "sysFilesPath : %s", *sysFilesPath);
	//	CUSTOM_FILE_APP_LOG_EX("App.log", "mapFilePath : %s", *mapFilePath);

	//	if (SpawnMapData(tctFilesPath, sysFilesPath, mapFilePath, totalRotOffset, totalPosOffset))
	//	{
	//		//if (pScenarioController->InitializeScenario())
	//		//{
	//		//	return true;
	//		//}

	//		CUSTOM_FILE_APP_LOG_EX("App.log", "SpawnMapData Ok~");
	//	}
	//}

	return false;
}

bool ARtGameSceneManager::SpawnMctItemProcessTM(
	FVector totalPosOffset, FRotator totalRotOffset, FMatrix rotationMatrix, bool asyncLoad,
	ERtMctSpawnType spawnType, FTemplateBase item)
{
	FRtObjectSpawnData spawnData = FRtObjectSpawnData(spawnType,
		item.TemplateModelKey,
		item.TemplateName,
		item.TemplateLocation,
		item.TemplateRotation,
		item.TemplateScale,
		totalPosOffset,
		totalRotOffset,
		rotationMatrix);

	SpawnMctItemProcess(asyncLoad, spawnData);

	return true;
}

void ARtGameSceneManager::SpawnMctItemProcess(bool asyncLoad, FRtObjectSpawnData& spawnData)
{
	if (asyncLoad)
	{
		AsyncMapLoadInfoArray.Add(spawnData);
	}
	else
	{
		AddSpawnNRegist(spawnData);
	}
}

// mct ������ �ε� �� ���忡 ���� �Ѵ�.
bool ARtGameSceneManager::SpawnMapData(FString tctFilesPath, FString sysFilesPath, FString mctFilePath, FRotator totalRotOffset, FVector totalPosOffset, bool asyncLoad)
{
	RtObjectMap.Empty();
	RtNpcMap.Empty();
	RtTraineeMap.Empty();

	FreeMoveAreaList.Empty();

	for (TPair<FString, TSharedPtr<USoundWave>> wavePtr : SoundWaveMap)
	{
		// dave
		if (!wavePtr.Value) continue;
		if (!wavePtr.Value->IsValidLowLevel()) continue;
		//

		wavePtr.Value->ConditionalBeginDestroy();
	}
	SoundWaveMap.Empty();

	if (!PrepareSpawnMapData(tctFilesPath, sysFilesPath, mctFilePath))
	{
		FString errorMsg = FString::Printf(TEXT("PrepareSpawnMapData Error !!"));
		CUSTOM_FILE_APP_LOG_EX("App.log", "%s", *errorMsg);
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(errorMsg));

		if (!ErrorLog.IsEmpty())
		{
			errorMsg = FString::Printf(TEXT("PrepareSpawnMapData Error %s"), *ErrorLog);
			CUSTOM_FILE_APP_LOG_EX("App.log", "%s", *errorMsg);
			CLIENT_FILE_LOG("%s", *ErrorLog);
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(errorMsg));
		}

		return false;
	}

	AsyncMapLoadInfoArray.Empty();
	AsyncMapLoadInfoArraySize = 0;

	CUSTOM_FILE_APP_LOG_EX("App.log", "PrepareSpawnMapData Ok~");

	CreateTctSoundWaves();

	// �����(quaternion) ���·� ��ȯ�Ͽ� ȸ�� ����� �����մϴ�.
	FQuat rotationQuat = totalRotOffset.Quaternion();
	FMatrix rotationMatrix = rotationQuat.ToMatrix();

	UpdateSystemActors();

	if (MctData.Version > 0)
	{
		// �Ϲ� Ÿ�� ����
		for (const auto& item : MctData.NormalTypeList)
		{
			SpawnMctItemProcessTM(totalPosOffset, totalRotOffset, rotationMatrix, asyncLoad, ERtMctSpawnType::EOther, item);
		}

		// ĳ���� ����
		for (const auto& item : MctData.CharacterTypeList)
		{
			FString ObjectKeyName = item.TemplateName;

			ERtPlayerType playerType = ERtPlayerType::ENone;
			if (ScenarioControllerRef.IsValid())
			{
				ARtGameScenarioController* gameScenarioController = Cast<ARtGameScenarioController>(ScenarioControllerRef);
				if (gameScenarioController)
				{
					FRtScenarioPlayerGroup* scenarioPlayerGroup = gameScenarioController->GetScenarioPlayerGroupInfo(ObjectKeyName);
					if (scenarioPlayerGroup)
					{
						playerType = scenarioPlayerGroup->PlayerType;
					}
				}
			}

			//ERtMctSpawnType spawnType = (playerType == ERtPlayerType::ENPC) ? ERtMctSpawnType::ENPC : ERtMctSpawnType::EPC;
			if (playerType == ERtPlayerType::ENPC)
			{
				SpawnMctItemProcessTM(totalPosOffset, totalRotOffset, rotationMatrix, asyncLoad, ERtMctSpawnType::ENPC, item);
			}

		}

		// ��� Ÿ�� ����
		for (const auto& item : MctData.EquipmentTypeList)
		{
			SpawnMctItemProcessTM(totalPosOffset, totalRotOffset, rotationMatrix, asyncLoad, ERtMctSpawnType::EOther, item);
		}

		// ���� Ÿ�� ����
		for (const auto& item : MctData.CarTypeList)
		{
			SpawnMctItemProcessTM(totalPosOffset, totalRotOffset, rotationMatrix, asyncLoad, ERtMctSpawnType::EOther, item);
		}

		// ȯ�� Ÿ�� ����
		for (const auto& item : MctData.EnvironmentTypeList)
		{
			SpawnMctItemProcessTM(totalPosOffset, totalRotOffset, rotationMatrix, asyncLoad, ERtMctSpawnType::EOther, item);
		}

		// ���� �̵� ���� ����
		TArray<FTemplateArea> datas = MctData.AreasDatas;
		for (FTemplateArea data : datas)
		{
			FTemplateBase areaData = data.AreaBasic;
			FreeMoveAreaList.Add(FTransform(areaData.TemplateRotation, areaData.TemplateLocation, areaData.TemplateScale * 100.0f));
		}

		ApplyExponentialHeightFog();
	}
	else
	{
		FString ObjectKeyName;
		// ĳ���� ����
		for (const FTemplateCharacter& charData : MctData.CharacterDatas)
		{
			ObjectKeyName = charData.BasicInfo.TemplateName;

			ERtPlayerType playerType = ERtPlayerType::ENone;
			if (ScenarioControllerRef.IsValid())
			{
				ARtGameScenarioController* gameScenarioController = Cast<ARtGameScenarioController>(ScenarioControllerRef);
				if (gameScenarioController)
				{
					FRtScenarioPlayerGroup* scenarioPlayerGroup = gameScenarioController->GetScenarioPlayerGroupInfo(ObjectKeyName);
					if (scenarioPlayerGroup)
					{
						playerType = scenarioPlayerGroup->PlayerType;
					}
				}
			}

			//ERtMctSpawnType spawnType = (playerType == ERtPlayerType::ENPC) ? ERtMctSpawnType::ENPC : ERtMctSpawnType::EPC;
			if (playerType == ERtPlayerType::ENPC)
			{
				ERtMctSpawnType spawnType = ERtMctSpawnType::ENPC;

				FRtObjectSpawnData spawnData = FRtObjectSpawnData(spawnType,
					charData.BasicInfo.TemplateModelKey, charData.BasicInfo.TemplateName,
					charData.BasicInfo.TemplateLocation,
					charData.BasicInfo.TemplateRotation,
					charData.BasicInfo.TemplateScale,
					totalPosOffset, totalRotOffset, rotationMatrix);

				SpawnMctItemProcess(asyncLoad, spawnData);
			}
		}

		// Ÿ�� ����
		for (const FTemplateTile& tileData : MctData.tilesDatas)
		{
			FRtObjectSpawnData spawnData = FRtObjectSpawnData(
				ERtMctSpawnType::EOther,
				tileData.TileBasic.TemplateModelKey,
				tileData.TileBasic.TemplateName,
				tileData.TileBasic.TemplateLocation,
				tileData.TileBasic.TemplateRotation,
				tileData.TileBasic.TemplateScale,
				totalPosOffset, totalRotOffset, rotationMatrix);

			SpawnMctItemProcess(asyncLoad, spawnData);
		}

		// �� ����
		for (const FTemplateWall& wallsData : MctData.wallsDatas)
		{
			FRtObjectSpawnData spawnData = FRtObjectSpawnData(
				ERtMctSpawnType::EOther,
				wallsData.WallBasic.TemplateModelKey,
				wallsData.WallBasic.TemplateName,
				wallsData.WallBasic.TemplateLocation,
				wallsData.WallBasic.TemplateRotation,
				wallsData.WallBasic.TemplateScale,
				totalPosOffset, totalRotOffset, rotationMatrix);

			SpawnMctItemProcess(asyncLoad, spawnData);
		}

		// ��ǰ ����
		//int propCounter = 0;
		for (const FTemplateProps& propData : MctData.propsDatas)
		{
			FRtObjectSpawnData spawnData = FRtObjectSpawnData(
				ERtMctSpawnType::EOther,
				propData.PropBasic.TemplateModelKey,
				propData.PropBasic.TemplateName,
				propData.PropBasic.TemplateLocation,
				propData.PropBasic.TemplateRotation,
				propData.PropBasic.TemplateScale,
				totalPosOffset, totalRotOffset, rotationMatrix);

			SpawnMctItemProcess(asyncLoad, spawnData);
		}

		// ���� ��ǰ ����
		//int equipPropCounter = 0;
		for (const FTemplateEquips& equipData : MctData.EquipsDatas)
		{
			FRtObjectSpawnData spawnData = FRtObjectSpawnData(
				ERtMctSpawnType::EOther,
				equipData.WeaponBasic.TemplateModelKey,
				equipData.WeaponBasic.TemplateName,
				equipData.WeaponBasic.TemplateLocation,
				equipData.WeaponBasic.TemplateRotation,
				equipData.WeaponBasic.TemplateScale,
				totalPosOffset, totalRotOffset, rotationMatrix);

			SpawnMctItemProcess(asyncLoad, spawnData);
		}
	}

	// �÷��̾� ����.
	for (const FTemplatePlayerSpawn& playerSpawnData : MctData.PlayerSpawnDatas)
	{
		FRtObjectSpawnData spawnData = FRtObjectSpawnData(
			ERtMctSpawnType::EOther,
			playerSpawnData.SpawnBasic.TemplateModelKey,
			playerSpawnData.SpawnBasic.TemplateName,
			playerSpawnData.SpawnBasic.TemplateLocation,
			playerSpawnData.SpawnBasic.TemplateRotation,
			playerSpawnData.SpawnBasic.TemplateScale,
			totalPosOffset, totalRotOffset, rotationMatrix);

		SpawnMctItemProcess(asyncLoad, spawnData);
	}

	// ���� ����Ʈ.
	for (const FTemplateWayPoint& wpData : MctData.WayPointDatas)
	{
		FRtObjectSpawnData spawnData = FRtObjectSpawnData(
			ERtMctSpawnType::EOther,
			wpData.WayPointBasic.TemplateModelKey,
			wpData.WayPointBasic.TemplateName,
			wpData.WayPointBasic.TemplateLocation,
			wpData.WayPointBasic.TemplateRotation,
			wpData.WayPointBasic.TemplateScale,
			totalPosOffset, totalRotOffset, rotationMatrix);

		SpawnMctItemProcess(asyncLoad, spawnData);
	}

	// ���� ��.
	for (const FTemplateInvisibleWall& iwData : MctData.InvisibleWallDatas)
	{
		FRtObjectSpawnData spawnData = FRtObjectSpawnData(
			ERtMctSpawnType::EOther,
			iwData.InvisibleWallBasic.TemplateModelKey,
			iwData.InvisibleWallBasic.TemplateName,
			iwData.InvisibleWallBasic.TemplateLocation,
			iwData.InvisibleWallBasic.TemplateRotation,
			iwData.InvisibleWallBasic.TemplateScale,
			totalPosOffset, totalRotOffset, rotationMatrix);

		SpawnMctItemProcess(asyncLoad, spawnData);
	}

	// Ʈ���� �ڽ�.
	for (const FTemplateTriggerBox& tbData : MctData.TriggerBoxDatas)
	{
		FRtObjectSpawnData spawnData = FRtObjectSpawnData(
			ERtMctSpawnType::EOther,
			tbData.TriggerBoxBasic.TemplateModelKey,
			tbData.TriggerBoxBasic.TemplateName,
			tbData.TriggerBoxBasic.TemplateLocation,
			tbData.TriggerBoxBasic.TemplateRotation,
			tbData.TriggerBoxBasic.TemplateScale,
			totalPosOffset, totalRotOffset, rotationMatrix);

		SpawnMctItemProcess(asyncLoad, spawnData);
	}

	AsyncMapLoadInfoArraySize = AsyncMapLoadInfoArray.Num();

	return true;
}


void ARtGameSceneManager::Server_DestroyAllSpawnedObjects_Implementation()
{
}

bool ARtGameSceneManager::Server_DestroyAllSpawnedObjects_Validate()
{
	return true;
}

void ARtGameSceneManager::SetDayNight(bool isNight)
{
	if (Day_DirectionalLight)
		Day_DirectionalLight->GetRootComponent()->SetVisibility(!isNight);
	if (Day_SkyAtmosphere)
		Day_SkyAtmosphere->GetRootComponent()->SetVisibility(!isNight);
	if (Night_DirectionalLight)
		Night_DirectionalLight->GetRootComponent()->SetVisibility(isNight);
	if (Night_SkyAtmosphere)
		Night_SkyAtmosphere->GetRootComponent()->SetVisibility(isNight);
}

void ARtGameSceneManager::AsyncMapLoadProcess()
{
	// �� ���� ������ �ε� �� ����.
	if (!AsyncMapLoadInfoArray.IsEmpty())
	{
		FRtObjectSpawnData spawnData = AsyncMapLoadInfoArray.Pop();
		if (ARtObjectActor* spawnObj = Cast<ARtObjectActor>(AddSpawnNRegist(spawnData)))
		{
			RtObjectMap.Add(spawnData.ObjectKeyName, spawnObj);
			if (ARtNpcObjectActor* spawnNpc = Cast<ARtNpcObjectActor>(spawnObj))
			{
				RtNpcMap.Add(spawnData.ObjectKeyName, spawnNpc);
			}
		}

		int32 loadingCount = AsyncMapLoadInfoArraySize - AsyncMapLoadInfoArray.Num();
		APP_LOG("map loading ~ [ %d / %d ]", loadingCount, AsyncMapLoadInfoArraySize);

		if (AsyncMapLoadInfoArray.IsEmpty()) // �ε� �Ϸ�� ó��..
		{
			CUSTOM_FILE_APP_LOG_EX("App.log", "Complete Load Map");
			// Todo : Fin
				// as
			if (auto* gi = GetGameInstance())
			{
				if (auto* lm = gi->GetSubsystem<URtLoadingManager>())
				{
					if (lm)
					{
						lm->OnLevelLoaded();
					}
				}
			}
			AsyncMapLoadInfoArraySize = 0;

			SetInitObjectDatas();
			if (ARtTraineePawn* trainee = Cast<ARtTraineePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
			{
				trainee->Init();
			}

			CreateGuideHands();

			if (Get_GameManager()->Get_Play_Type() == ERt_PlayType::E_Trainee)
			{
				CreateListenIcons();
			}

			if (Floor.IsValid())
			{
				Floor->GetStaticMeshComponent()->SetVisibility(false);
			}

			OnMapLoadComplete.Broadcast();
		}
	}
}

ARtObjectActor* ARtGameSceneManager::GetPlayerMeshActor(const FString& ObjectKeyName)
{
	// ������ ���� ĳ���� ����
	for (const FMapTemplateCharacter& charData : MctData.CharacterTypeList)
	{
		if (ObjectKeyName != charData.TemplateName)
			continue;

		ERtPlayerType playerType = ERtPlayerType::ENone;
		if (ScenarioControllerRef.IsValid())
		{
			ARtGameScenarioController* gameScenarioController = Cast<ARtGameScenarioController>(ScenarioControllerRef);
			if (gameScenarioController)
			{
				FRtScenarioPlayerGroup* scenarioPlayerGroup = gameScenarioController->GetScenarioPlayerGroupInfo(ObjectKeyName);
				if (scenarioPlayerGroup)
				{
					playerType = scenarioPlayerGroup->PlayerType;
				}
			}
		}

		if (playerType == ERtPlayerType::EPC)
		{
			FRtObjectSpawnData spawnData = FRtObjectSpawnData(ERtMctSpawnType::EPC,
				charData.TemplateModelKey, charData.TemplateName,
				charData.TemplateLocation,
				charData.TemplateRotation,
				charData.TemplateScale,
				FVector(), FRotator(), FMatrix());

			AActor* pActor = AddSpawnNRegist(spawnData);
			return Cast<ARtObjectActor>(pActor);
		}
	}

	// ������ ���� ĳ���� ����
	for (const FTemplateCharacter& charData : MctData.CharacterDatas)
	{
		if (ObjectKeyName != charData.BasicInfo.TemplateName)
			continue;

		ERtPlayerType playerType = ERtPlayerType::ENone;
		if (ScenarioControllerRef.IsValid())
		{
			ARtGameScenarioController* gameScenarioController = Cast<ARtGameScenarioController>(ScenarioControllerRef);
			if (gameScenarioController)
			{
				FRtScenarioPlayerGroup* scenarioPlayerGroup = gameScenarioController->GetScenarioPlayerGroupInfo(ObjectKeyName);
				if (scenarioPlayerGroup)
				{
					playerType = scenarioPlayerGroup->PlayerType;
				}
			}
		}

		if (playerType == ERtPlayerType::EPC)
		{
			ERtMctSpawnType spawnType = ERtMctSpawnType::ENPC;

			FRtObjectSpawnData spawnData = FRtObjectSpawnData(spawnType,
				charData.BasicInfo.TemplateModelKey, charData.BasicInfo.TemplateName,
				charData.BasicInfo.TemplateLocation,
				charData.BasicInfo.TemplateRotation,
				charData.BasicInfo.TemplateScale,
				FVector(), FRotator(), FMatrix());

			AActor* pActor = AddSpawnNRegist(spawnData);
			return Cast<ARtObjectActor>(pActor);
		}
	}

	return nullptr;
}

ARtObjectActor* ARtGameSceneManager::FindObjectByName(FString objectName)
{
	return RtObjectMap.FindRef(objectName);
}

void ARtGameSceneManager::Tick(float DeltaTime) // ƽ�� �ȵ���;;;
{
	Super::Tick(DeltaTime);

	// AsyncMapLoadProcess();
}

URtGameManager* ARtGameSceneManager::Get_GameManager()
{
	if (auto* gi = GetGameInstance())
		return gi->GetSubsystem<URtGameManager>();

	return nullptr;
}

void ARtGameSceneManager::SetInitObjectDatas()
{
	InitObjectDataMap.Empty();

	ARtReplayManagerActor* replayManagerActor = ARtReplayManagerActor::Get(GetWorld());
	replayManagerActor->RecordObjects.Empty();

	ARtGameScenarioController* scenarioController = Cast<ARtGameScenarioController>(ScenarioControllerRef);

	bool isReplay = false;
	if (Get_GameManager() &&
		Get_GameManager()->Get_Play_Type() == ERt_PlayType::E_Replay) {
		isReplay = true; //scenarioController->CurrentPlayType == ERt_PlayType::E_Replay;
	} 

	CUSTOM_FILE_APP_LOG_EX("App.log", "Start Equipment Initializing");

	TArray<TPair<FString, FString>> charactersAndEquipments;

	for (const FTemplateCharacter& charData : MctData.CharacterDatas)
	{
		FString charId = charData.BasicInfo.TemplateName;
		FString equipObjId = charData.EquipObjectID;
		charactersAndEquipments.Add({ charId, equipObjId });
	}

	for (const FMapTemplateCharacter& charData : MctData.CharacterTypeList)
	{
		FString charId = charData.TemplateName;
		FString equipObjId = charData.EquipObjectID;
		charactersAndEquipments.Add({ charId, equipObjId });
	}

	for (TPair<FString, FString> charAndEquip : charactersAndEquipments)
	{
		FString charId = charAndEquip.Key;
		FString equipObjId = charAndEquip.Value;

		if (charId.IsEmpty() || equipObjId.IsEmpty())
		{
			continue;
		}

		ARtObjectActor* equipActor = FindObjectByName(equipObjId);
		if (equipActor == nullptr)
		{
			CUSTOM_FILE_APP_LOG_EX("App.log", "Not Found: %s", *equipObjId);
			continue;
		}

		if (FRtScenarioPlayerGroup* scenarioPlayerGroup = scenarioController->GetScenarioPlayerGroupInfo(charId))
		{
			ERtPlayerType playerType = scenarioPlayerGroup->PlayerType;

			if (playerType == ERtPlayerType::ENPC)
			{
				if (ARtNpcObjectActor* npc = Cast<ARtNpcObjectActor>(FindObjectByName(charId)))
				{
					if (FInitObjectData* objectDataPtr = InitObjectDataMap.Find(npc))
						continue;
					InitObjectDataMap.Add(npc, FInitObjectData(npc));
					replayManagerActor->RecordObjects.Add(npc);
				}
				else
				{
					CUSTOM_FILE_APP_LOG_EX("App.log", "Not Found: %s", *charId);
					continue;
				}
			}

			if (isReplay || scenarioController->AttachWeapon(charId, equipObjId))
			{
				if (FInitObjectData* objectDataPtr = InitObjectDataMap.Find(equipActor))
					continue;

				InitObjectDataMap.Add(equipActor, FInitObjectData(equipActor));
				replayManagerActor->RecordObjects.Add(equipActor);
				CUSTOM_FILE_APP_LOG_EX("App.log", "Equip: %s, %s", *charId, *equipObjId);
			}
			else
			{
				equipActor->SetVisible(false);
				CUSTOM_FILE_APP_LOG_EX("App.log", "Equip Fail: %s, %s", *charId, *equipObjId);
			}
		}
		else
		{
			equipActor->SetVisible(false);
			CUSTOM_FILE_APP_LOG_EX("App.log", "No Group: %s", *charId);
			continue;
		}
	}

	CUSTOM_FILE_APP_LOG_EX("App.log", "Success Equipment Initializing");

	ARtScenarioModel* scenarioModel = scenarioController->ScenarioModel.Get();

	for (TPair<FString, FRtScenarioEventCommand> pair : scenarioModel->EventCommandMap)
		AddInitObjectDatas(pair.Value);

	for (TPair<FString, TPair<const FRtEventCommand*, FString>> pair : scenarioModel->ServerEventCommandMap)
		AddInitObjectDatas(*pair.Value.Key);

	CUSTOM_FILE_APP_LOG_EX("App.log", "Success Save Initializing");

	if (isReplay && replayManagerActor)
		replayManagerActor->PostProcessEntities();

}

// dave
void ARtGameSceneManager::ClearReplay() 
{
	InitObjectDataMap.Empty();

	ARtReplayManagerActor* replayManagerActor = ARtReplayManagerActor::Get(GetWorld());
	replayManagerActor->RecordObjects.Empty();
}

void ARtGameSceneManager::AddInitObjectDatas(const FRtEventCommand& eventCommand)
{
	ARtGameScenarioController* scenarioController = Cast<ARtGameScenarioController>(ScenarioControllerRef);

	for (FRtSctFunctionCommand proCmd : eventCommand.ProcessCommandList)
	{
		TArray<FRtSctFunctionCommandParam> param = proCmd.Params;
		switch (proCmd.FunctionCommand)
		{
		case ERtSctFunCmd::SetVisible:
		case ERtSctFunCmd::PlayAnim:
		case ERtSctFunCmd::MoveToWayPoint:
		case ERtSctFunCmd::NpcMoveToWayPoint:
		{
			if (param.Num() > 0)
			{
				TArray<FString> whoIDList = scenarioController->GetObjectIDList(param[0].ParamString);
				if (whoIDList.IsEmpty())
					return;

				for (FString objectID : whoIDList)
				{
					ARtObjectActor* actor = FindObjectByName(objectID);
					if (actor == nullptr)
						continue;
					if (FInitObjectData* objectDataPtr = InitObjectDataMap.Find(actor))
						continue;
					else
					{
						InitObjectDataMap.Add(actor, FInitObjectData(actor));
						ARtReplayManagerActor::Get(GetWorld())->RecordObjects.Add(actor);
					}
				}
			}
		}
		break;
		case ERtSctFunCmd::AttachWeapon:
		{
			if (param.Num() > 1)
			{
				TArray<FString> whoIDList = scenarioController->GetObjectIDList(param[1].ParamString);
				if (whoIDList.IsEmpty())
					return;

				for (FString objectID : whoIDList)
				{
					ARtObjectActor* actor = FindObjectByName(objectID);
					if (actor == nullptr)
						continue;
					if (FInitObjectData* objectDataPtr = InitObjectDataMap.Find(actor))
						continue;
					else
					{
						InitObjectDataMap.Add(actor, FInitObjectData(actor));
						ARtReplayManagerActor::Get(GetWorld())->RecordObjects.Add(actor);
					}
				}
			}
		}
		break;
		default:
			break;
		}
	}
}

void ARtGameSceneManager::CreateTctSoundWaves()
{
	FString savedSoundPath = FPaths::Combine(URtResourceModelSubsystem::Get()->GetTrainingDataPath(), PROJECT_PATH_WAVFILES);
	savedSoundPath = savedSoundPath.Replace(TEXT("\\"), TEXT("/"));

	for (TPair<FString, TSharedPtr<RtActorModel>> objectModelDataPtr : ObjectModelMap)
	{
		if (FRtTctStruct* tctData = objectModelDataPtr.Value.Get()->GetTctStruct().Get())
		{
			const TArray<FRtTctComDirection>& directionData = tctData->ComDirectionList;
			if (directionData.IsEmpty())
				continue;

			TArray<FRtTctComDirectionItem> directionItemList = directionData[0].DirectionItemList;
			for (const FRtTctComDirectionItem& data : directionItemList)
			{
				FString soundFileName = data.SoundFileName;
				if (soundFileName.IsEmpty() || SoundWaveMap.Contains(soundFileName))
					continue;
				FString soundPath = savedSoundPath + FString::Printf(TEXT("/%s"), *soundFileName);

				if (USoundWave* soundWave = CreateSoundWaveByPath(soundPath))
				{
					if (soundWave->GetClass() != USoundWave::StaticClass())
						continue;
					SoundWaveMap.Add(soundFileName, soundWave);
				}
			}
		}
	}
}

USoundWave* ARtGameSceneManager::CreateSoundWaveByPath(FString filePath)
{
	USoundWave* SoundWave = NULL;
	FName name = MakeUniqueObjectName(this, USoundWave::StaticClass(), *FPaths::GetBaseFilename(filePath));
	FindObjectFast<USoundWave>(this, name);
	if (SoundWave)
		return SoundWave;

	if (!FPaths::FileExists(filePath))
	{
		FText errorTitle = FText::FromString("Create Wave False Error");
		FString errorMsg = FString::Printf(TEXT("WAV File Is Not Exist: %s"), *filePath);
		UE_LOG(LogTemp, Error, TEXT("%s"), *errorMsg);
		CUSTOM_FILE_APP_LOG_EX("App.log", "%s: %s", *errorTitle.ToString(), *errorMsg);
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(errorMsg), &errorTitle);

		return nullptr;
	}

	TArray<uint8> RawFile;
	bool ck = FPlatformFileManager::Get().GetPlatformFile().FileExists(*filePath);
	if (FFileHelper::LoadFileToArray(RawFile, *filePath))
	{
		FWaveModInfo WaveInfo;
		if (WaveInfo.ReadWaveInfo(RawFile.GetData(), RawFile.Num()))
		{
			SoundWave = NewObject<USoundWave>(this, name);

			// Set WAV Info to SoundWave
			SoundWave->SetSampleRate(*WaveInfo.pSamplesPerSec);
			SoundWave->NumChannels = *WaveInfo.pChannels;
			SoundWave->Duration = static_cast<float>(*WaveInfo.pWaveDataSize) / *WaveInfo.pAvgBytesPerSec;
			SoundWave->RawPCMDataSize = *WaveInfo.pWaveDataSize;

			// Copy Audio Data
			SoundWave->RawPCMData = (uint8*)FMemory::Malloc(SoundWave->RawPCMDataSize);
			FMemory::Memcpy(SoundWave->RawPCMData, WaveInfo.SampleDataStart, SoundWave->RawPCMDataSize);

			// Set Additional Info
			SoundWave->SoundGroup = ESoundGroup::SOUNDGROUP_Default;
			SoundWave->NumChannels = *WaveInfo.pChannels;

			// Set Cue Point
			for (const FWaveCue& WaveCue : WaveInfo.WaveCues)
			{
				FSoundWaveCuePoint NewCuePoint;
				NewCuePoint.CuePointID = WaveCue.CuePointID;
				NewCuePoint.Label = WaveCue.Label.IsEmpty() ? FString::Printf(TEXT("Cue_%d"), WaveCue.CuePointID) : WaveCue.Label;
				NewCuePoint.FramePosition = WaveCue.Position;
				NewCuePoint.FrameLength = WaveCue.SampleLength;
				SoundWave->CuePoints.Add(NewCuePoint);
			}

			return SoundWave;
		}
	}

	FText errorTitle = FText::FromString("Create Wave False Error");
	FString errorMsg = FString::Printf(TEXT("WAV File Load Fail: %s"), *filePath);
	UE_LOG(LogTemp, Error, TEXT("%s"), *errorMsg);
	CUSTOM_FILE_APP_LOG_EX("App.log", "%s: %s", *errorTitle.ToString(), *errorMsg);
	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(errorMsg), &errorTitle);

	return nullptr;
}

void ARtGameSceneManager::CreateGuideHands()
{
	// ��ӹڽ� �̺�Ʈ�� �а�
	ARtGameScenarioController* scenarioController = Cast<ARtGameScenarioController>(ScenarioControllerRef);
	ARtScenarioModel* scenarioModel = scenarioController->ScenarioModel.Get();
	scenarioController->GuideHandMap.Empty();

	UClass* MyActorClass = LoadObject<UClass>(nullptr, RtBlueprintAsset::GuideHand);
	if (MyActorClass == nullptr)
		return;

	for (TPair<FString, FRtIDList> dropboxPair : scenarioModel->TargetDropboxMap)
	{
		// �ش� ������Ʈ ã��
		FString objectID = dropboxPair.Key;
		TArray<FString> targetMenuList = dropboxPair.Value;
		ARtObjectActor* object = FindObjectByName(objectID);
		if (object == nullptr)
			continue;


		for (FString targetMenu : targetMenuList)
		{
			// �̺�Ʈ�� �Ҵ�� ������Ʈ�� �����Ͽ� Attach
			AActor* guideHandActor = GetWorld()->SpawnActor<AActor>(MyActorClass, FTransform());
			ARtGuideHand* guideHand = Cast<ARtGuideHand>(guideHandActor);
			// HACK: ���� ������Ʈ ��ġ�� ����. �ӽ÷� ������Ʈ�� �߾ӿ�, NPC�� ĸ�� �߾ӿ� ����.
			guideHand->AttachToActor(object, FAttachmentTransformRules::KeepRelativeTransform);
			guideHand->SetActorScale3D(FVector(1.0f));
			guideHand->TargetMenuName = targetMenu;

			scenarioController->GuideHandMap.Add(targetMenu, guideHand);

			object->Delegate_SetVisible.AddLambda([guideHand]() {
				guideHand->UpdateVisible();
				});

			if (Cast<ARtNpcObjectActor>(object))
			{
				for (const TPair<UShapeComponent*, ERtCollisionType>& collisionsPair : object->GetCollisionMap())
				{
					guideHandActor->AttachToComponent(collisionsPair.Key, FAttachmentTransformRules::KeepRelativeTransform);
					if (const UCapsuleComponent* capsule = Cast<UCapsuleComponent>(collisionsPair.Key))
					{
						guideHandActor->SetActorRelativeLocation(FVector(capsule->GetUnscaledCapsuleRadius() * 0.8f, 0.0f, 0.0f));
						guideHandActor->SetActorRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
					}
				}
			}

		}

	}

	// �Ʒ� ���� �� Visible?


}

void ARtGameSceneManager::CreateListenIcons()
{
	// ��ӹڽ� �̺�Ʈ�� �а�
	ARtGameScenarioController* scenarioController = Cast<ARtGameScenarioController>(ScenarioControllerRef);
	ARtScenarioModel* scenarioModel = scenarioController->ScenarioModel.Get();
	scenarioController->ListenIconMap.Empty();

	UClass* ListenWidgetActorClass = LoadObject<UClass>(nullptr, RtBlueprintAsset::ListenIcon);
	if (ListenWidgetActorClass == nullptr)
		return;

	for (TPair<FString, FRtIDList> dropboxPair : scenarioModel->TargetDropboxMap)
	{
		// �ش� ������Ʈ ã��
		FString objectID = dropboxPair.Key;
		TArray<FString> targetMenuList = dropboxPair.Value;
		ARtObjectActor* object = FindObjectByName(objectID);
		if (object == nullptr)
			continue;


		for (FString targetMenu : targetMenuList)
		{
			// Npc�� �ƴ϶�� Continue
			ARtNpcObjectActor* NpcObject = Cast<ARtNpcObjectActor>(object);
			if (!NpcObject) continue;

			// VoiceCommandList ã��
			FRtDropboxCommand* dropboxCommand = scenarioModel->DropboxCommandMap.Find(targetMenu);
			if (!dropboxCommand) continue;

			for (FRtDropboxEventCommand dropboxEventCommand : dropboxCommand->DropboxElemantList)
			{
				TArray<FString> voiceCommandList = dropboxEventCommand.VoiceCommandList;
				FString eventID = dropboxEventCommand.EventID;

				if (voiceCommandList.Num() > 0)
				{
					// �̺�Ʈ�� �Ҵ�� ������Ʈ�� �����Ͽ� Attach
					AActor* ListenWidgetActor = GetWorld()->SpawnActor<AActor>(ListenWidgetActorClass, FTransform());
					ARtWidgetActor_ListenIcon* ListenIcon = Cast<ARtWidgetActor_ListenIcon>(ListenWidgetActor);

					// HACK: ���� ������Ʈ ��ġ�� ����. �ӽ÷� ������Ʈ�� �߾ӿ�, NPC�� ĸ�� �߾ӿ� ����.
					ListenIcon->AttachToActor(object, FAttachmentTransformRules::KeepRelativeTransform);
					ListenIcon->SetActorScale3D(FVector(1.0f));
					ListenIcon->SetTargetMenuName(eventID);
					ListenIcon->SetVoiceCommandList(voiceCommandList);

					scenarioController->ListenIconMap.Add(targetMenu, ListenIcon);

					for (const TPair<UShapeComponent*, ERtCollisionType>& collisionsPair : object->GetCollisionMap())
					{
						ListenWidgetActor->AttachToComponent(collisionsPair.Key, FAttachmentTransformRules::KeepRelativeTransform);
						ListenWidgetActor->SetActorRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
						ListenWidgetActor->SetActorRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
					
						// Test Code
						UE_LOG(LogTemp, Log, TEXT("********Create Listen Icon********"));
						UE_LOG(LogTemp, Log, TEXT("< targetMenu : %s >"), *targetMenu);
						UE_LOG(LogTemp, Log, TEXT("< eventID : %s >"), *eventID);
						for (FString voiceCommand : voiceCommandList)
						{
							UE_LOG(LogTemp, Log, TEXT("voiceCommand : %s"), *voiceCommand);
						}
						UE_LOG(LogTemp, Log, TEXT("**********************************\n\n"));
					}
				}
			}
		}

	}
}

ARtGameSceneManager::FInitObjectData::FInitObjectData(ARtObjectActor* objectActor)
{
	Loc = objectActor->GetActorTransform().GetLocation();
	Rot = objectActor->GetActorTransform().GetRotation().Rotator();
	Visible = objectActor->IsVisible();
	Anim = objectActor->GetAnimationProgress().Key;
	IsAnimLoop = objectActor->IsLoopingAnim();
	AttachParent = objectActor->GetAttachParentActor();

	if (IsValid(AttachParent)) // dave
	{
		if (AglTFRuntimeFBXAssetActor* npcMesh = Cast<AglTFRuntimeFBXAssetActor>(AttachParent))
		{
			AttachParent = AttachParent->GetAttachParentActor();
		}
	}
}

void ARtGameSceneManager::FInitObjectData::InitData(ARtObjectActor* objectActor)
{
	if (IsValid(AttachParent) && AttachParent->IsValidLowLevel()) // dave 
	{
		if (ARtObjectActor* npcActor = Cast<ARtObjectActor>(AttachParent))
			objectActor->AttachToNpc(npcActor);
		else if (ARtTraineePawn* trainee = Cast<ARtTraineePawn>(AttachParent))
			trainee->AttachWeapon(objectActor);
	}
	else
	{
		if (ARtTraineePawn* curParentTrainee = Cast<ARtTraineePawn>(objectActor->GetAttachParentActor()))
		{
			curParentTrainee->IsEquip = false;
			curParentTrainee->Equipment = nullptr;
		}

		objectActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		objectActor->SetActorLocation(Loc);
		objectActor->SetActorRotation(Rot);
	}

	objectActor->SetVisible(Visible);

	if (Anim.IsEmpty())
		objectActor->StopAnimation();
	else
		objectActor->PlayAnimation(Anim, IsAnimLoop);
}


// dave  
// TODO : ��絥���� �ʱ�ȭ�Ǵ��� Ȯ��
void ARtGameSceneManager::DestroyAllSpawnedObjects()
{
	/*
	// All objs
	for (auto& Pair : RtObjectMap)
	{
		if (ARtObjectActor* Object = Pair.Value)
		{
			if (Object->IsValidLowLevel() && !IsValid(Object))
			{
				Object->Destroy();
			}
		}
	}
	RtObjectMap.Empty();


	// NPC
	/*for (auto& Pair : RtNpcMap)
	{
		if (ARtNpcObjectActor* Npc = Pair.Value)
		{
			if (Npc->IsValidLowLevel() && !IsValid(Npc))
			{
				Npc->Destroy();
			}
		}
	}
	RtNpcMap.Empty();
	*/

	// Trainee
	/*for (auto& Pair : RtTraineeMap)
	{
		if (ARtTraineePawn* Trainee = Pair.Value)
		{
			if (Trainee->IsValidLowLevel() && !IsValid(Trainee))
			{
				Trainee->Destroy();
			}
		}
	}*/

	RtTraineeMap.Empty();

	AsyncMapLoadInfoArray.Empty();
	AsyncMapLoadInfoArraySize = 0;

	// sounds
	for (auto& Pair : SoundWaveMap)
	{
		if (USoundWave* wav = Pair.Value) {
			if (wav->IsValidLowLevel() && !wav->IsPendingKill())
			{
				// dave
				if (!Pair.Value) continue;
				if (!Pair.Value->IsValidLowLevel()) continue;
				//
				Pair.Value->ConditionalBeginDestroy();
			}
		}
	}
	SoundWaveMap.Empty();

	// FreeMoveArea
	FreeMoveAreaList.Empty();

	// Guide Hands 
	UWorld* World = GetWorld();
	if (World)
	{
		TArray<AActor*> FoundGuideHands;
		UGameplayStatics::GetAllActorsOfClass(World, ARtGuideHand::StaticClass(), FoundGuideHands);

		for (AActor* GuideHand : FoundGuideHands)
		{
			if (GuideHand && !GuideHand->IsPendingKill())
			{
				GuideHand->Destroy();
			}
		}
	}

	UnloadMct();
	ARtGameScenarioController* pScenarioController = ARtGameScenarioController::Get(GetWorld());
	check(pScenarioController);
	pScenarioController->UnloadSct();

	GEngine->ForceGarbageCollection(true);
}