// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RtBTDecorator.h"
#include "Framework/RtAIController.h"
#include "Framework/RtNpcObjectActor.h"
#include "Framework/RtTraineePawn.h"
#include "Manager/RtGameScenarioController.h"
#include "Manager/RtGameSceneManager.h"


#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "NavMesh/RecastNavMesh.h"

ARtNpcObjectActor* URtBTDecorator::GetNpc(UBehaviorTreeComponent& OwnerComp) const
{
	return Cast<ARtNpcObjectActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor")));
}

bool URtBTDecorator::IsSct(UBehaviorTreeComponent& OwnerComp) const
{
	ARtNpcObjectActor* npc = GetNpc(OwnerComp);
	const FRtNpcBehaviorState& state = npc->GetNpcBehaviorState();
	bool isSctMode = state.SctFunCmd != ERtSctFunCmd::ENone;
	return npc->IsVisible() && isSctMode;
}

bool URtBTDecorator::CanMoveTo(UBehaviorTreeComponent& OwnerComp, FVector endLoc) const
{
	ARtNpcObjectActor* npc = GetNpc(OwnerComp);
	ARtAIController* aiCtrller = Cast<ARtAIController>(npc->GetController());
	UNavigationSystemV1* navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(aiCtrller->GetWorld());
	if (!navSys)
		return false;

	FPathFindingResult result;

	ANavigationData* navData = navSys->GetNavDataForProps(aiCtrller->GetNavAgentPropertiesRef());

	FVector::FReal pathCost = 0.0f;
	result = navData->CalcPathCost(npc->GetActorLocation(), endLoc, pathCost);

	bool canMoveTo = result.IsSuccessful();

	return npc->IsVisible() && canMoveTo;
}

FVector URtBTDecorator::GetLocation(AActor* target) const
{
	if (ARtTraineePawn* trainee = Cast<ARtTraineePawn>(target)) {
		return trainee->GetTraineeLocActor()->GetActorLocation();
	}

	return target->GetActorLocation();
}

bool URtBTD_Sct::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return IsSct(OwnerComp);
}

bool URtBTD_TargetSct::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (UObject* target = OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Target")))
		return true;

	return false;
}

bool URtBTD_CanMoveTo::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ARtNpcObjectActor* npc = GetNpc(OwnerComp);
	ARtAIController* aiCtrller = Cast<ARtAIController>(npc->GetController());

	FVector endLoc;

	if (UObject* target = OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Target"))) {
		if (ARtTraineePawn* targetTrainee = Cast<ARtTraineePawn>(target))
			endLoc = targetTrainee->GetTraineeLocActor()->GetActorLocation();
		else if (AActor* targetActor = Cast<AActor>(target)) {
			endLoc = targetActor->GetActorLocation();
			if (!CanMoveTo(OwnerComp, endLoc)) {
				if (ARtObjectActor* obj = Cast<ARtObjectActor>(targetActor)) {
					if (obj->GetTctType2() == ERtTctType2::ENormal) {
						FVector start = npc->GetActorLocation();
						FVector end = obj->GetActorLocation();

						TArray<FHitResult> hitResults;
						TArray<AActor*> actorsToIgnore = { npc };
						EDrawDebugTrace::Type debugLineType = EDrawDebugTrace::None;

						TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // 히트 가능한 오브젝트 유형들.
						ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
						ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

						if (UKismetSystemLibrary::LineTraceMultiForObjects(npc->GetWorld(), start, end, ObjectTypes, false, actorsToIgnore, debugLineType, hitResults, true)) {
							for (const FHitResult& hitResult : hitResults) {
								if (hitResult.GetActor() == obj) {
									endLoc = hitResult.ImpactPoint;
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	else {
		FVector freeMove = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("FreeMove"));
		if (freeMove.IsZero())
			return false;
		endLoc = freeMove;
	}
	
	bool isSuccessful = CanMoveTo(OwnerComp, endLoc);

	if (!isSuccessful) {
		npc->UpdateAnimState(ERtAnimType::EIdle);

		FRtNpcBehaviorState state = npc->GetNpcBehaviorState();
		if (bool isSctMode = state.SctFunCmd != ERtSctFunCmd::ENone) {
			state.SctFunCmd = ERtSctFunCmd::ENone;
			state.SctTarget.Reset();
			npc->UpdateNpcBehaviorState(state);
		}
		else {
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("Target"), nullptr);
		}
	}

	return isSuccessful;
}

bool URtBTD_FindAttackTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if(bool isSct = IsSct(OwnerComp))
		return false;

	ARtNpcObjectActor* npc = GetNpc(OwnerComp);
	if (!npc->IsVisible())
		return false;

	const ERtRoleType role = npc->GetRoleType();
	AActor* attackTarget = nullptr;
	ARtGameSceneManager* gsm = ARtGameSceneManager::Get(GetWorld());
	const TMap<FString, ARtNpcObjectActor*>& allNpcs = gsm->GetRtNpcMap();
	const TMap<FString, ARtTraineePawn*>& allTrainees = gsm->GetRtTraineeMap();
	AActor* nearestTarget = nullptr;
	float minDist = FLT_MAX;

	switch (role)
	{
	case ERtRoleType::ERoleSuspicious: //거수자
		break;
	case ERtRoleType::ERoleTerrorist: //테러범
	{
		for (TPair<FString, ARtTraineePawn*> newTargetPair : allTrainees) {
			ARtTraineePawn* newTarget = newTargetPair.Value;
			FVector targetLoc = GetLocation(newTarget);
			float newDist = FVector::Distance(npc->GetActorLocation(), targetLoc);
			if (newDist < minDist && CanMoveTo(OwnerComp, targetLoc)) {
				nearestTarget = newTarget;
				minDist = newDist;
			}
		}

		for (TPair<FString, ARtNpcObjectActor*> newTargetPair : allNpcs) {
			ARtNpcObjectActor* newTarget = newTargetPair.Value;
			if (newTarget->GetRoleType() == ERtRoleType::ERoleSoldier) {
				if (newTarget->GetNpcBehaviorState().ActType == ERtAnimType::ECollapse)
					continue;
				FVector targetLoc = GetLocation(newTarget);
				float newDist = FVector::Distance(npc->GetActorLocation(), targetLoc);
				if (newDist < minDist && CanMoveTo(OwnerComp, targetLoc)) {
					nearestTarget = newTarget;
					minDist = newDist;
				}
			}
		}

	}
		break;
	case ERtRoleType::ERoleSoldier: //군인
		for (TPair<FString, ARtNpcObjectActor*> newTargetPair : allNpcs) {
			ARtNpcObjectActor* newTarget = newTargetPair.Value;
			if (newTarget->GetRoleType() == ERtRoleType::ERoleTerrorist) {
				if (newTarget->GetNpcBehaviorState().ActType == ERtAnimType::ECollapse ||
					newTarget->GetNpcBehaviorState().ActType == ERtAnimType::ESurrender)
					continue;
				FVector targetLoc = GetLocation(newTarget);
				float newDist = FVector::Distance(npc->GetActorLocation(), targetLoc);
				if (newDist < minDist && CanMoveTo(OwnerComp, targetLoc)) {
					nearestTarget = newTarget;
					minDist = newDist;
				}
			}
		}
		break;
	default:
		break;
	}

	if (minDist <= FindRange) {
		FRtNpcBehaviorState state = npc->GetNpcBehaviorState();
		state.MoveType = ERtAnimType::ERun;
		npc->UpdateNpcBehaviorState(state);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("Target"), nearestTarget);
		return true;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("Target"), nullptr);

	return false;
}
