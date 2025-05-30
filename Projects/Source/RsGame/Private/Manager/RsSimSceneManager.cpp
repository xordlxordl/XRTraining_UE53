// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/RsSimSceneManager.h"

#include "Kismet/GameplayStatics.h"

#include "Manager/RsSimScenarioController.h"
#include "Actor/RsSimCharacter.h"
#include "Actor/RsSimNPC.h"

#include "RtFbxLoaderSubsystem.h"

void ARsSimSceneManager::BeginPlay()
{
	UGameInstance* GameInstance = GetGameInstance();
	URtFbxLoaderSubsystem::InitSubSystem(GameInstance);
}

AActor* ARsSimSceneManager::AddSpawnNRegist(ERtMctSpawnType mctSpawnType, FString modelKey, FString actorName, FVector vPos, FRotator rRot)
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
		//if (pFoundModel->Get()->GetTctData()->mTagName == "character tag")
		//{
		//	FActorSpawnParameters spawnParams;
		//	if (!actorName.IsEmpty()) {
		//		spawnParams.Name = FName(*actorName);
		//	}

		//	UWorld* pWorld = GetWorld();
		//	ARsSimNPC* pNpcActor = pWorld->SpawnActor<ARsSimNPC>(ARsSimNPC::StaticClass(), vPos, rRot, spawnParams);
		//	if (pNpcActor)
		//	{
		//		URtObjectActorComponent* pActorCom = Cast<URtObjectActorComponent>(pNpcActor->GetComponentByClass(URtObjectActorComponent::StaticClass()));
		//		if (pActorCom)
		//		{
		//			pActorCom->SetSkeletalMeshAsset(pFoundModel->Get()->GetTctData()->mModelFilePath);
		//		}
		//		NpcList.Add(pNpcActor);
		//		return pNpcActor;
		//	}
		//}
		//else
		//{

		switch (mctSpawnType)
		{
			case ERtMctSpawnType::EPC:
			{
				//ARsSimCharacter* pSimCharacter = SpawnActorBase<ARsSimCharacter>(modelKey, actorName, vPos.X, vPos.Y, vPos.Z + 20, rRot.Pitch, rRot.Yaw, rRot.Roll, isForGame);
				return nullptr;
			}
			break;

			case ERtMctSpawnType::ENPC:
			{
			//	ARsSimNPC* pSimCNpc = SpawnActorBase<ARsSimNPC>(modelKey, actorName, vPos.X, vPos.Y, vPos.Z, rRot.Pitch, rRot.Yaw, rRot.Roll, isForGame);

				return nullptr;
			}
			break;

			default:
				{
					ARtObjectActor* pObjectActor = Cast<ARtObjectActor>(SpawnActor(modelKey, actorName, vPos, rRot));

					if (!GIsEditor)
					{
						UE_LOG(LogTemp, Warning, TEXT("AddSpawnNRegist_3"));
					}

					if (pObjectActor && AddSpawnActor(pObjectActor, modelKey, vPos, rRot))
					{
						UE_LOG(LogTemp, Warning, TEXT("AddSpawnNRegist_4"));

						return pObjectActor;
					}
				}
				break;
		}
	}

	return nullptr;
}


// * ������ �󿡼��� �ַ�� ������ �ִ� ���� ����.
// * ���� �Ŀ��� ���������� �ִ� ���� ����.
// 
// subProjectDirName : ������Ʈ ��� ���� ���
// subSysDirName : �ý��� ���� ��� ���� ���
// scenarioFileName : �ó����� ���� �̸�
bool ARsSimSceneManager::LoadSimpleProjectScenarioScene(FString subProjectDirName, FString subSysDirName, FString scenarioFileName, FRotator totalRotOffset, FVector totalPosOffset)
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
bool ARsSimSceneManager::LoadScenarioScene(FString tctFilesPath, FString sysFilesPath, FString scenarioFilePath, bool bIsFireZone, FRotator totalRotOffset, FVector totalPosOffset)
{
	ARsSimScenarioController* pScenarioController = Cast<ARsSimScenarioController>(UGameplayStatics::GetActorOfClass(GetWorld(), ARsSimScenarioController::StaticClass()));

	UE_LOG(LogTemp, Warning, TEXT("LoadScenarioScene_1"));

	tctFilesPath = tctFilesPath.Replace(TEXT("\\"), TEXT("/"));
	sysFilesPath = sysFilesPath.Replace(TEXT("\\"), TEXT("/"));
	scenarioFilePath = scenarioFilePath.Replace(TEXT("\\"), TEXT("/"));

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

		//pScenarioController->Init(scenarioFilePath);

		// �÷��̾� ĳ���� ����.
		TArray<AActor*> foundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARsSimCharacter::StaticClass(), foundActors);
		if (foundActors.Num() > 0) // �ϴ� ��Ƽ ������ ���߿� �����Ѵ�.
		{
			pScenarioController->SetPlayerActor(Cast<ARsSimCharacter>(foundActors[0]));
		}

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

		if (SpawnMapData(tctFilesPath, sysFilesPath, mapFilePath, totalRotOffset, totalPosOffset))
		{
			if (pScenarioController->InitializeScenario())
			{
				return true;
			}
		}
	}

	return false;
}

// mct ������ �ε� �� ���忡 ���� �Ѵ�.
bool ARsSimSceneManager::SpawnMapData(FString tctFilesPath, FString sysFilesPath, FString mctFilePath, FRotator totalRotOffset, FVector totalPosOffset)
{
	if (!PrepareSpawnMapData(tctFilesPath, sysFilesPath, mctFilePath))
	{
		return false;
	}

	//FRotator totalRotDelta = FRotator(0, 90, 0);
	//FVector totalPosDelta = FVector(200, 100, 0);

	//// �� ���Ϳ� ������ ����Ͽ� ȸ�� ��������(Rotator)�� �����մϴ�.
	//FRotator rotationRotator(0.0f, 90.0f,0); // Euler ���� ����� ����

	// �����(quaternion) ���·� ��ȯ�Ͽ� ȸ�� ����� �����մϴ�.
	FQuat rotationQuat = totalRotOffset.Quaternion();
	FMatrix rotationMatrix = rotationQuat.ToMatrix();

	FString ObjectKeyName;
//	ObjectName_ActorMap.Empty();

	// ĳ���� ����
	for (const FTemplateCharacter& charData : MctData.CharacterDatas)
	{
		ObjectKeyName = charData.BasicInfo.TemplateName;

		ERtMctSpawnType spawnType = (charData.BasicInfo.TemplateTag == "NPC") ? ERtMctSpawnType::ENPC : ERtMctSpawnType::EPC;

		AActor* pActor = AddSpawnNRegist(
			spawnType,
			charData.BasicInfo.TemplateModelKey,
			ObjectKeyName,
			charData.BasicInfo.TemplateLocation,
			charData.BasicInfo.TemplateRotation);

		if(pActor)
		{
			URtObjectActorComponent* pObjectActorCom = Cast<URtObjectActorComponent>(pActor->GetComponentByClass(URtObjectActorComponent::StaticClass()));
			if (pObjectActorCom)
			{
				pObjectActorCom->SetOwnerRename(ObjectKeyName);
			}
		}

		//ObjectName_ActorMap.Emplace(ObjectKeyName, pActor);
	}

	// Ÿ�� ����
	for (const FTemplateTile& tileData : MctData.tilesDatas)
	{
		AActor* pActor = AddSpawnNRegist(
			ERtMctSpawnType::EOther,
			tileData.TileBasic.TemplateModelKey,
			tileData.TileBasic.TemplateName,
			tileData.TileBasic.TemplateLocation,
			tileData.TileBasic.TemplateRotation);

		if (pActor)
		{
			URtObjectActorComponent* pObjectActorCom = Cast<URtObjectActorComponent>(pActor->GetComponentByClass(URtObjectActorComponent::StaticClass()));
			if (pObjectActorCom)
			{
				pObjectActorCom->SetOwnerRename(tileData.TileBasic.TemplateName);
			}

			//			pActor->SetActorScale3D(FVector(1.05f, 1.05f, 1.f));

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

	// �� ����
	for (const FTemplateWall& wallsData : MctData.wallsDatas)
	{
		AActor* pActor = AddSpawnNRegist(
			ERtMctSpawnType::EOther,
			wallsData.WallBasic.TemplateModelKey,
			wallsData.WallBasic.TemplateName,
			wallsData.WallBasic.TemplateLocation,
			wallsData.WallBasic.TemplateRotation);

		if (pActor)
		{
			URtObjectActorComponent* pObjectActorCom = Cast<URtObjectActorComponent>(pActor->GetComponentByClass(URtObjectActorComponent::StaticClass()));
			if (pObjectActorCom)
			{
				pObjectActorCom->SetOwnerRename(wallsData.WallBasic.TemplateName);
			}

			pActor->SetActorScale3D(FVector(0.5f, 1.05f, 1.f));
			pActor->SetActorRelativeLocation(pActor->GetActorLocation() + FVector(-0.5f, -0.5f, 1.f));

			pActor->AddActorWorldRotation(totalRotOffset);
			FVector newLocation = rotationMatrix.TransformVector(wallsData.WallBasic.TemplateLocation);
			newLocation += totalPosOffset;
			pActor->SetActorLocation(newLocation);
		}
	}

	// ��ǰ ����
	int propCounter = 0;
	for (const FTemplateProps& propData : MctData.propsDatas)
	{
		AActor* pActor = AddSpawnNRegist(
			ERtMctSpawnType::EOther,
			propData.PropBasic.TemplateModelKey,
			propData.PropBasic.TemplateName,
			propData.PropBasic.TemplateLocation,
			propData.PropBasic.TemplateRotation);

		if (pActor)
		{
			URtObjectActorComponent* pObjectActorCom = Cast<URtObjectActorComponent>(pActor->GetComponentByClass(URtObjectActorComponent::StaticClass()));
			if (pObjectActorCom)
			{
				pObjectActorCom->SetOwnerRename(propData.PropBasic.TemplateName);
				pObjectActorCom->SetReferenceIndex(ERtTctType::EProp, propCounter, propData.PropBasic.TemplateModelKey);
			}

			//			propData.PropBasic.TemplateScale
			pActor->SetActorScale3D(FVector(1));

			pActor->AddActorWorldRotation(totalRotOffset);
			FVector newLocation = rotationMatrix.TransformVector(propData.PropBasic.TemplateLocation);
			newLocation += totalPosOffset;
			pActor->SetActorLocation(newLocation);

			propCounter++;
		}
	}

	// �÷��̾� ����.
	for (const FTemplatePlayerSpawn& spawnData : MctData.PlayerSpawnDatas)
	{
		ObjectKeyName = spawnData.SpawnBasic.TemplateName;

		AActor* pActor = AddSpawnNRegist(
			ERtMctSpawnType::EOther,
			spawnData.SpawnBasic.TemplateModelKey,
			spawnData.SpawnBasic.TemplateName,
			spawnData.SpawnBasic.TemplateLocation,
			spawnData.SpawnBasic.TemplateRotation);

		URtObjectActorComponent* pObjectActorCom = Cast<URtObjectActorComponent>(pActor->GetComponentByClass(URtObjectActorComponent::StaticClass()));
		if (pObjectActorCom)
		{
			pObjectActorCom->SetOwnerRename(ObjectKeyName);
		}

		//ObjectName_ActorMap.Emplace(ObjectKeyName, pActor);
	}

	// ���� ����Ʈ.
	for (const FTemplateWayPoint& wpData : MctData.WayPointDatas)
	{
		ObjectKeyName = wpData.WayPointBasic.TemplateName;

		AActor* pActor = AddSpawnNRegist(
			ERtMctSpawnType::EOther,
			wpData.WayPointBasic.TemplateModelKey,
			wpData.WayPointBasic.TemplateName,
			wpData.WayPointBasic.TemplateLocation,
			wpData.WayPointBasic.TemplateRotation);

		URtObjectActorComponent* pObjectActorCom = Cast<URtObjectActorComponent>(pActor->GetComponentByClass(URtObjectActorComponent::StaticClass()));
		if (pObjectActorCom)
		{
			pObjectActorCom->SetOwnerRename(ObjectKeyName);
		}

		//ObjectName_ActorMap.Emplace(ObjectKeyName, pActor);
	}

	// ���� ��.
	for (const FTemplateInvisibleWall& iwData : MctData.InvisibleWallDatas)
	{
		ObjectKeyName = iwData.InvisibleWallBasic.TemplateName;

		AActor* pActor = AddSpawnNRegist(
			ERtMctSpawnType::EOther,
			iwData.InvisibleWallBasic.TemplateModelKey,
			iwData.InvisibleWallBasic.TemplateName,
			iwData.InvisibleWallBasic.TemplateLocation,
			iwData.InvisibleWallBasic.TemplateRotation);

		URtObjectActorComponent* pObjectActorCom = Cast<URtObjectActorComponent>(pActor->GetComponentByClass(URtObjectActorComponent::StaticClass()));
		if (pObjectActorCom)
		{
			pObjectActorCom->SetOwnerRename(ObjectKeyName);
		}

		//ObjectName_ActorMap.Emplace(ObjectKeyName, pActor);
	}

	// Ʈ���� �ڽ�.
	for (const FTemplateTriggerBox& tbData : MctData.TriggerBoxDatas)
	{
		ObjectKeyName = tbData.TriggerBoxBasic.TemplateName;

		AActor* pActor = AddSpawnNRegist(
			ERtMctSpawnType::EOther,
			tbData.TriggerBoxBasic.TemplateModelKey,
			tbData.TriggerBoxBasic.TemplateName,
			tbData.TriggerBoxBasic.TemplateLocation,
			tbData.TriggerBoxBasic.TemplateRotation);

		URtObjectActorComponent* pObjectActorCom = Cast<URtObjectActorComponent>(pActor->GetComponentByClass(URtObjectActorComponent::StaticClass()));
		if (pObjectActorCom)
		{
			pObjectActorCom->SetOwnerRename(ObjectKeyName);
		}

		//ObjectName_ActorMap.Emplace(ObjectKeyName, pActor);
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