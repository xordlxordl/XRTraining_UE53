// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/RsSimScenarioController.h"

#include "Actor/RsSimCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(Log_RsSimScenarioController);

ARtSceneManagerBase* ARsSimScenarioController::RtSceneManager = nullptr;

void ARsSimScenarioController::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARtSceneManagerBase::StaticClass(), actors);
	if (actors.Num())
	{
		RtSceneManager = Cast<ARtSceneManagerBase>(actors[0]);
		BindRsFunctions();
	}
}

// cct, mct 가 모두 로드가 되고 객체들이 스폰까지 된 이후에 호출 되어야 한다.
bool ARsSimScenarioController::InitializeScenario()
{
	//ThemalImageCamOn = false;
	//ActorEntryRegionInfoMap.Empty();
	//EvaluationStateMap.Empty();

	//ARsSceneManager* pSceneManager = Cast<ARsSceneManager>(SceneManagerRef);

	//if (nullptr != ScenarioModel && pSceneManager && pSceneManager->GetMctFileLoaded())
	//{
	//	GatherMainObjectList();

	//	for (const FRtScenarioEvent& eventItem : ScenarioModel->SctData.EventList)
	//	{
	//		if (!eventItem.OwnerObjectID.IsEmpty())
	//		{
	//			AActor** ppFoundActor = MainObjectMap.Find(eventItem.OwnerObjectID);
	//			if (nullptr != ppFoundActor)
	//			{
	//				AActor* pOwnerObjectActor = *ppFoundActor;
	//				URtObjectActorComponent* pOwnerObjectActor_DataCom = pOwnerObjectActor->FindComponentByClass<URtObjectActorComponent>();
	//				USphereComponent* pOwnerObjectActor_SphereCom = pOwnerObjectActor->FindComponentByClass<USphereComponent>();

	//				for (const FRtScenarioCondition& scenarioCon : eventItem.EventOccurConditionList)
	//				{
	//					if (scenarioCon.ConditionType == ERtSctCondition::EDistance)
	//					{
	//						float mm = FCString::Atof(*scenarioCon.Target1Value);

	//						if (pOwnerObjectActor_DataCom->SphereRadius == 0)
	//						{
	//							pOwnerObjectActor_DataCom->SphereRadius = mm / 10.0f;
	//							pOwnerObjectActor_SphereCom->SetSphereRadius(pOwnerObjectActor_DataCom->SphereRadius); // 거리 설정.
	//						}
	//					}
	//				}
	//			}
	//		}
	//		else
	//		{

	//		}
	//	}

	//	TimerHandleMap.Empty();


	//	return true;
	//}

	return false;
}

void ARsSimScenarioController::SetPlayerActor(ARsSimCharacter* myPlayerActor)
{
	if (myPlayerActor)
	{
		MyPlayer = myPlayerActor;
		MyPlayerNameID = MyPlayer->GetName();
	}
}

void ARsSimScenarioController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RsMoveToWayPoint_Progress(DeltaTime);
	RsMoveToX_Progress(DeltaTime);
	RsRotateZTo_Progress(DeltaTime);
}

///////////////////////////////////////////////////
// Scenario Events
///////////////////////////////////////////////////

//Bind Console Event
void ARsSimScenarioController::BindRsFunctions()
{
	{
		IConsoleManager::Get().RegisterConsoleCommand(
			TEXT("Rsf.SetVisible"),
			TEXT("targetId bVisible"),
			FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args)
				{
					if (Args.Num() < 2) return;
					RsSetVisible(*Args[0], FCString::ToBool(*Args[1]));
				}));
	}
	{
		IConsoleManager::Get().RegisterConsoleCommand(
			TEXT("Rsf.MoveToWayPoint"),
			TEXT("ownerId targetFormat startIndex endIndex moveSpeed"),
			FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args)
				{
					if (Args.Num() < 5) return;
					RsMoveToWayPoint(*Args[0], *Args[1], FCString::Atoi(*Args[2]), FCString::Atoi(*Args[3]), FCString::Atof(*Args[4]));
				}));
	}
	{
		IConsoleManager::Get().RegisterConsoleCommand(
			TEXT("Rsf.MoveToX"),
			TEXT("ownerId inputX inputY moveTime"),
			FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args)
				{
					if (Args.Num() < 4) return;
					RsMoveToX(*Args[0], FCString::Atof(*Args[1]), FCString::Atof(*Args[2]), FCString::Atof(*Args[3]));
				}));
	}
	{
		IConsoleManager::Get().RegisterConsoleCommand(
			TEXT("Rsf.WarpTo"),
			TEXT("ownerId targetId safeDistance"),
			FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args)
				{
					if (Args.Num() < 3) return;
					RsWarpTo(*Args[0], *Args[1], FCString::Atof(*Args[2]));
				}));
	}
	{
		IConsoleManager::Get().RegisterConsoleCommand(
			TEXT("Rsf.RotateZTo"),
			TEXT("ownerId targetId"),
			FConsoleCommandWithArgsDelegate::CreateLambda([&](const TArray<FString>& Args)
				{
					if (Args.Num() < 2) return;
					RsRotateZTo(*Args[0], *Args[1]);
				}));
	}
}

//Scenario Events Start
void ARsSimScenarioController::RsSetVisible(const FString& targetId, const bool& bVisible)
{
	//UE_LOG(Log_RsSimScenarioController, Display, TEXT("SetVisible : %s, %d"), *targetId, bVisible);
	//AActor* target = RtSceneManager->FindObjectByNameKey(targetId);
	//if (target)
	//	target->SetActorHiddenInGame(!bVisible);
	//else
	//	UE_LOG(Log_RsSimScenarioController, Warning, TEXT("No Target"));

}

void ARsSimScenarioController::RsMoveToWayPoint(const FString& ownerId, const FString& targetFormat, const int32& startIndex, const int32& endIndex, const float& moveSpeed)
{
	//UE_LOG(Log_RsSimScenarioController, Display, TEXT("MoveToWayPoint : %s, %s, %d, %d, %f"), *ownerId , *targetFormat, startIndex, endIndex, moveSpeed);
	//if (moveSpeed < 1.f) return;

	//AActor* owner = RtSceneManager->FindObjectByNameKey(ownerId);
	//if (owner == nullptr)
	//{
	//	UE_LOG(Log_RsSimScenarioController, Warning, TEXT("No Owner"));
	//	return;
	//}

	//MoveToWayPointMaps.Add(owner);
	//MoveToWayPointMaps.Find(owner)->Value = moveSpeed;

	//for (int i = startIndex; i <= endIndex; i++)
	//{
	//	FString targetId = targetFormat.Replace(TEXT("%d"), *FString::FromInt(i));
	//	//UE_LOG(Log_RsSimScenarioController, Display, TEXT("targetId : %s"), *targetId);
	//	AActor* wayPoint = RtSceneManager->FindObjectByNameKey(targetId);
	//	if (wayPoint)
	//	{
	//		MoveToWayPointMaps.Find(owner)->Key.Add(wayPoint);
	//	}
	//}

	//if (MoveToWayPointMaps.Find(owner)->Key.Num() <= 0)
	//{
	//	UE_LOG(Log_RsSimScenarioController, Warning, TEXT("No WayPoints"));
	//	MoveToWayPointMaps.Remove(owner);
	//	return;
	//}

}

void ARsSimScenarioController::RsMoveToX(const FString& ownerId, const float& x, const float& y, const float& moveTime)
{
	//UE_LOG(Log_RsSimScenarioController, Display, TEXT("MoveToX : %s, %f, %f, %f"), *ownerId , x, y, moveTime);
	//if (moveTime <= 0.f) return;

	//AActor* owner = RtSceneManager->FindObjectByNameKey(ownerId);
	//if (owner == nullptr)
	//{
	//	UE_LOG(Log_RsSimScenarioController, Warning, TEXT("No Owner"));
	//	return;
	//}

	//MoveToXMaps.Add(owner);
	//FVector ownerLocation = owner->GetActorLocation();
	//FVector goalLocation = FVector(x, y, owner->GetActorLocation().Z);
	//MoveToXMaps.Find(owner)->Key = goalLocation;
	//MoveToXMaps.Find(owner)->Value = FVector::Distance(ownerLocation, goalLocation) / moveTime;
}

void ARsSimScenarioController::RsWarpTo(const FString& ownerId, const FString& targetId, const float& safeDistance)
{
	////UE_LOG(Log_RsSimScenarioController, Display, TEXT("WarpTo : %s, %f, %f, %f"), *ownerId , x, y, moveTime);
	//if (safeDistance <= 0.f) return;

	//AActor* owner = RtSceneManager->FindObjectByNameKey(ownerId);
	//if (owner == nullptr)
	//{
	//	UE_LOG(Log_RsSimScenarioController, Warning, TEXT("No Owner"));
	//	return;
	//}

	//AActor* target = RtSceneManager->FindObjectByNameKey(targetId);
	//if (target == nullptr)
	//{
	//	UE_LOG(Log_RsSimScenarioController, Warning, TEXT("No Target"));
	//	return;
	//}

	//FVector ownerLocation = owner->GetActorLocation();
	//FVector targetLocation = target->GetActorLocation();
	//targetLocation.Z = ownerLocation.Z; // Z축 무시
	//FVector distanceTo = ownerLocation - targetLocation;
	//FVector direction = distanceTo.GetSafeNormal();
	//owner->SetActorLocation(targetLocation + direction * safeDistance);
}

void ARsSimScenarioController::RsRotateZTo(const FString& ownerId, const FString& targetId)
{
	//AActor* owner = RtSceneManager->FindObjectByNameKey(ownerId);
	//if (owner == nullptr)
	//{
	//	UE_LOG(Log_RsSimScenarioController, Warning, TEXT("No Owner"));
	//	return;
	//}

	//AActor* target = RtSceneManager->FindObjectByNameKey(targetId);
	//if (target == nullptr)
	//{
	//	UE_LOG(Log_RsSimScenarioController, Warning, TEXT("No Target"));
	//	return;
	//}

	//RotateZToMaps.Add(owner, target);
}

//Scenario Events Progress
void ARsSimScenarioController::RsMoveToWayPoint_Progress(float DeltaTime)
{
	if (MoveToWayPointMaps.IsEmpty())
		return;

	TArray<AActor*> endMoveActors;

	for (auto& moveMap : MoveToWayPointMaps)
	{
		AActor* moveActor = moveMap.Key;
		TArray<AActor*>& wayPoints = moveMap.Value.Key;
		float moveSpeed = moveMap.Value.Value;

		if (wayPoints.IsEmpty())
		{
			//UE_LOG(Log_RsSimScenarioController, Display, TEXT("End MoveToWayPoint : %s"), *moveActor->GetName());
			endMoveActors.Add(moveActor);
			continue;
		}

		AActor* wayPoint = wayPoints[0];

		if (wayPoint == nullptr)
		{
			UE_LOG(Log_RsSimScenarioController, Warning, TEXT("No WayPoint"));
			wayPoints.Remove(wayPoint);
			continue;
		}

		FVector moveActorLocation = moveActor->GetActorLocation();
		FVector wpLocation = wayPoint->GetActorLocation();
		wpLocation.Z = moveActorLocation.Z;
		FVector distanceVector = wpLocation - moveActorLocation;

		if (distanceVector.Length() < 10)//오차 범위 체크
		{
			//UE_LOG(Log_RsSimScenarioController, Display, TEXT("Next WayPoint"));
			wayPoints.Remove(wayPoint);
			continue;
		}

		//Move To WayPoint
		FVector direction = distanceVector.GetSafeNormal();
		moveActor->SetActorLocation(moveActorLocation + direction * moveSpeed * DeltaTime);
		moveActor->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(moveActorLocation, wpLocation));
	}

	for (AActor* moveActor : endMoveActors)
		MoveToWayPointMaps.Remove(moveActor);
}

void ARsSimScenarioController::RsMoveToX_Progress(float DeltaTime)
{
	if (MoveToXMaps.IsEmpty())
		return;

	TArray<AActor*> endMoveActors;

	for (auto& moveMap : MoveToXMaps)
	{
		AActor* moveActor = moveMap.Key;
		FVector& goalLocation = moveMap.Value.Key;
		float moveSpeed = moveMap.Value.Value;

		FVector moveActorLocation = moveActor->GetActorLocation();
		FVector distanceVector = goalLocation - moveActorLocation;

		if (distanceVector.Length() < 10)//오차 범위 체크
		{
			//UE_LOG(Log_RsSimScenarioController, Display, TEXT("End MoveToX"));
			endMoveActors.Add(moveActor);
			continue;
		}

		//Move To WayPoint
		FVector direction = distanceVector.GetSafeNormal();
		moveActor->SetActorLocation(moveActorLocation + direction * moveSpeed * DeltaTime);
		moveActor->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(moveActorLocation, goalLocation));
	}

	for (AActor* moveActor : endMoveActors)
		MoveToXMaps.Remove(moveActor);

}

void ARsSimScenarioController::RsRotateZTo_Progress(float DeltaTime)
{
	if (RotateZToMaps.IsEmpty())
		return;

	float rotSpeed = 360.f;
	TArray<AActor*> endRotateActors;

	for (auto& rotateMap : RotateZToMaps)
	{
		AActor* rotateActor = rotateMap.Key;
		AActor* targetActor = rotateMap.Value;
		FVector rotateActorLocation = rotateActor->GetActorLocation();
		FRotator rotateActorRotation = rotateActor->GetActorRotation();
		FVector targetActorLocation = targetActor->GetActorLocation();
		targetActorLocation.Z = rotateActorLocation.Z;
		FRotator goalRotate = UKismetMathLibrary::FindLookAtRotation(rotateActorLocation, targetActorLocation);
		//UE_LOG(Log_RsSimScenarioController, Display, TEXT("goalRotate : %s"), *goalRotate.ToString());

		float needRot = goalRotate.Yaw - rotateActorRotation.Yaw;
		//UE_LOG(Log_RsSimScenarioController, Display, TEXT("needRot : %f"), needRot);
		if (abs(needRot) > 180.f)
			needRot = goalRotate.Yaw > rotateActorRotation.Yaw ? needRot - 360 : needRot + 360;
		//UE_LOG(Log_RsSimScenarioController, Display, TEXT("needRot : %f"), needRot);
		float addRot = needRot > 0.f ? rotSpeed * DeltaTime : -rotSpeed * DeltaTime;
		rotateActorRotation.Yaw += addRot;
		if (abs(needRot) <= abs(addRot))
		{
			rotateActorRotation = goalRotate;
			endRotateActors.Add(rotateActor);
		}
		rotateActor->SetActorRotation(rotateActorRotation);
	}

	for (AActor* rotateActor : endRotateActors)
		RotateZToMaps.Remove(rotateActor);
}