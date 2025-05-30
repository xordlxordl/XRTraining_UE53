// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RtEnvQueryTest.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "Kismet/GameplayStatics.h"

#include "Framework/RtNpcObjectActor.h"
#include "Manager/RtGameScenarioController.h"
#include "Manager/RtGameSceneManager.h"

URtEQT_IsFreeMoveArea::URtEQT_IsFreeMoveArea()
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
}

void URtEQT_IsFreeMoveArea::RunTest(FEnvQueryInstance& QueryInstance) const {
	UObject* QueryOwner = QueryInstance.Owner.Get(); if (QueryOwner == nullptr) { return; }
	
	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It) {
		FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());
		bool IsFreeArea = IsInFreeMoveArea(ItemLocation);

		It.ForceItemState(IsFreeArea ? EEnvItemStatus::Passed : EEnvItemStatus::Failed);
		It.SetScore(TestPurpose, FilterType, IsFreeArea, 0.f, 1.f);
	}
}

bool URtEQT_IsFreeMoveArea::IsInFreeMoveArea(FVector point) const
{
	ARtGameSceneManager* scm = ARtGameSceneManager::Get(GetWorld());
	const TArray<FTransform>& FreeMoveAreas = scm->GetFreeMoveAreaList();

	for (FTransform areaTransform : FreeMoveAreas) {
		FVector localPoint = areaTransform.InverseTransformPosition(point);
		if (localPoint.X >= 0.0f && localPoint.X <= 1.0f &&
			localPoint.Y >= 0.0f && localPoint.Y <= 1.0f &&
			localPoint.Z >= 0.0f && localPoint.Z <= 1.0f) {
			return true;
		}
	}

	return false;
}
