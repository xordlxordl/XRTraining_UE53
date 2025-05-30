// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RtBTTask.h"
#include "Framework/RtAIController.h"
#include "Framework/RtNpcObjectActor.h"
#include "Framework/RtTraineePawn.h"
#include "BehaviorTree/BlackboardComponent.h"

ARtNpcObjectActor* URtBTTask::GetNpc(UBehaviorTreeComponent& OwnerComp)
{
	return Cast<ARtNpcObjectActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor")));
}

bool URtBTTask::IsAttackTargetLive(ARtNpcObjectActor* npc, AActor* target)
{
	if (target == nullptr)
		return false;

	if (ARtObjectActor* targetObj = Cast<ARtObjectActor>(target)) {
		if (targetObj->IsVisible() == false)
			return false;

		if (ARtNpcObjectActor* targetNpc = Cast<ARtNpcObjectActor>(targetObj)) {
			const FRtNpcBehaviorState& targetState = targetNpc->GetNpcBehaviorState();
			const FRtNpcBehaviorState& npcState = npc->GetNpcBehaviorState();
			if (!targetNpc->IsVisible() || targetState.ActType == ERtAnimType::ECollapse || targetState.ActType == ERtAnimType::ESurrender)
				return false;

			bool isAttack = npcState.SctFunCmd == ERtSctFunCmd::ENone
				|| npcState.SctFunCmd == ERtSctFunCmd::AttackTarget;
			return isAttack;
		}

		return true;
	}
	else if (ARtTraineePawn* targetTrainee = Cast<ARtTraineePawn>(target))
		return true;

	return false;
}

//////////////////////////////////////////////////
// Down
//////////////////////////////////////////////////

URtBTT_Down::URtBTT_Down(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Down");
}

EBTNodeResult::Type URtBTT_Down::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if (IsDown(OwnerComp)) {
		ARtNpcObjectActor* npc = GetNpc(OwnerComp);
		ARtAIController* aiCtrller = Cast<ARtAIController>(npc->GetController());
		aiCtrller->StopMove();
		npc->UpdateAnimState(ERtAnimType::ECollapse);
		
		if( !npc->NpcBehaviorStateUpdateDelegate.IsAlreadyBound(this, &URtBTT_Down::AbortNotDown) )
			npc->NpcBehaviorStateUpdateDelegate.AddDynamic(this, &URtBTT_Down::AbortNotDown);

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

bool URtBTT_Down::IsDown(UBehaviorTreeComponent& OwnerComp)
{
	ARtNpcObjectActor* npc = GetNpc(OwnerComp);
	const FRtNpcBehaviorState& npcState = npc->GetNpcBehaviorState();

	return !npc->IsVisible() || npcState.ActType == ERtAnimType::ECollapse;
}

void URtBTT_Down::AbortNotDown(ARtNpcObjectActor* npc)
{
	UBehaviorTreeComponent* btComp = Cast<ARtAIController>(npc->GetController())->BehaviorTreeComponent;
	if (IsDown(*btComp) == false) {
		npc->SetIsUpdateAct(true);
		npc->NpcBehaviorStateUpdateDelegate.RemoveDynamic(this, &URtBTT_Down::AbortNotDown);
		FinishLatentTask(*btComp, EBTNodeResult::Succeeded);
	}
}

//////////////////////////////////////////////////
// Surrender
//////////////////////////////////////////////////

URtBTT_Surrender::URtBTT_Surrender(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Surrender");
}

EBTNodeResult::Type URtBTT_Surrender::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if (IsSurrender(OwnerComp)) {
		ARtNpcObjectActor* npc = GetNpc(OwnerComp);
		ARtAIController* aiCtrller = Cast<ARtAIController>(npc->GetController());
		aiCtrller->StopMove();
		npc->UpdateAnimState(ERtAnimType::ESurrender);

		if (!npc->NpcBehaviorStateUpdateDelegate.IsAlreadyBound(this, &URtBTT_Surrender::AbortNotSurrender))
			npc->NpcBehaviorStateUpdateDelegate.AddDynamic(this, &URtBTT_Surrender::AbortNotSurrender);

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

bool URtBTT_Surrender::IsSurrender(UBehaviorTreeComponent& OwnerComp)
{
	ARtNpcObjectActor* npc = GetNpc(OwnerComp);
	const FRtNpcBehaviorState& npcState = npc->GetNpcBehaviorState();

	return npc->IsVisible() && npcState.ActType != ERtAnimType::ECollapse && npcState.ActType == ERtAnimType::ESurrender;
}

void URtBTT_Surrender::AbortNotSurrender(ARtNpcObjectActor* npc)
{
	UBehaviorTreeComponent* btComp = Cast<ARtAIController>(npc->GetController())->BehaviorTreeComponent;
	if (IsSurrender(*btComp) == false) {
		npc->SetIsUpdateAct(true);
		npc->NpcBehaviorStateUpdateDelegate.RemoveDynamic(this, &URtBTT_Surrender::AbortNotSurrender);
		FinishLatentTask(*btComp, EBTNodeResult::Succeeded);
	}
}

//////////////////////////////////////////////////
// MoveToActor
//////////////////////////////////////////////////

URtBTT_MoveToActor::URtBTT_MoveToActor(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("MoveToActor");
}

EBTNodeResult::Type URtBTT_MoveToActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ARtNpcObjectActor* npc = GetNpc(OwnerComp);
	const FRtNpcBehaviorState& npcState = npc->GetNpcBehaviorState();
	ARtAIController* aiCtrller = Cast<ARtAIController>(npc->GetController());
	FAIMoveRequest MoveRequest;

	UObject* target = OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Target"));

	if (ARtObjectActor* targetObj = Cast<ARtObjectActor>(target)) {
		if(targetObj->GetTctType2() != ERtTctType2::ENormal)
			MoveRequest.SetGoalActor(targetObj);
		else {
			FVector lineStart = npc->GetActorLocation();
			FVector lineEnd = targetObj->GetActorLocation();

			TArray<FHitResult> hitResults;
			TArray<AActor*> actorsToIgnore = { npc };
			EDrawDebugTrace::Type debugLineType = EDrawDebugTrace::None;

			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // 히트 가능한 오브젝트 유형들.
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

			if (UKismetSystemLibrary::LineTraceMultiForObjects(npc->GetWorld(), lineStart, lineEnd, ObjectTypes, false, actorsToIgnore, debugLineType, hitResults, true)) {
				for (const FHitResult& hitResult : hitResults) {
					if (hitResult.GetActor() == targetObj) {
						MoveRequest.SetGoalLocation(hitResult.ImpactPoint);
						break;
					}
				}
			}
		}
	}
	else if (ARtTraineePawn* targetTrainee = Cast<ARtTraineePawn>(target)) {
		MoveRequest.SetGoalActor(targetTrainee->GetTraineeLocActor());
	}
	else {
		return EBTNodeResult::Failed;
	}
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);

	if (!aiCtrller->OnRtMoveCompleted.IsAlreadyBound(this, &URtBTT_MoveToActor::OnMoveCompleted))
		aiCtrller->OnRtMoveCompleted.AddDynamic(this, &URtBTT_MoveToActor::OnMoveCompleted);

	EPathFollowingRequestResult::Type MoveResult = aiCtrller->MoveTo(MoveRequest);

	if (MoveResult == EPathFollowingRequestResult::Failed) {
		FRtNpcBehaviorState newState = npcState;
		newState.SctTarget.Reset();
		npc->UpdateNpcBehaviorState(newState);
		aiCtrller->OnRtMoveCompleted.Clear();
		return EBTNodeResult::Failed;
	}
	
	if (MoveResult == EPathFollowingRequestResult::AlreadyAtGoal) {
		if (npcState.SctFunCmd == ERtSctFunCmd::NpcMoveToWayPoint) {
			FRtNpcBehaviorState newState = npcState;
			newState.SctTarget.Reset();
			npc->UpdateNpcBehaviorState(newState);
		}
		aiCtrller->OnRtMoveCompleted.Clear();
		return EBTNodeResult::Succeeded;
	}

	npc->UpdateAnimState(npcState.MoveType);
	npc->SetIsUpdateAct(true);
	return EBTNodeResult::InProgress;
}

void URtBTT_MoveToActor::OnMoveCompleted(ARtAIController* aiCtrller, uint8 Result)
{
	FinishLatentTask(*aiCtrller->BehaviorTreeComponent, (EBTNodeResult::Type)Result);
}

//////////////////////////////////////////////////
// MoveToLocation
//////////////////////////////////////////////////

URtBTT_MoveToLocation::URtBTT_MoveToLocation(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("MoveToLocation");
}

EBTNodeResult::Type URtBTT_MoveToLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ARtNpcObjectActor* npc = GetNpc(OwnerComp);
	const FRtNpcBehaviorState& npcState = npc->GetNpcBehaviorState();
	ARtAIController* aiCtrller = Cast<ARtAIController>(npc->GetController());
	FAIMoveRequest MoveRequest;

	FVector freeMove = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("FreeMove"));

	if (freeMove.FVector::IsZero())
		return EBTNodeResult::Failed;

	FRtNpcBehaviorState state = npc->GetNpcBehaviorState();
	state.MoveType = ERtAnimType::EWalk;
	npc->UpdateNpcBehaviorState(state);

	MoveRequest.SetGoalLocation(freeMove);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);

	if (!aiCtrller->OnRtMoveCompleted.IsAlreadyBound(this, &URtBTT_MoveToLocation::OnMoveCompleted))
		aiCtrller->OnRtMoveCompleted.AddDynamic(this, &URtBTT_MoveToLocation::OnMoveCompleted);

	EPathFollowingRequestResult::Type MoveResult = aiCtrller->MoveTo(MoveRequest);

	if (MoveResult == EPathFollowingRequestResult::Failed) {
		aiCtrller->OnRtMoveCompleted.Clear();
		return EBTNodeResult::Failed;
	}

	if (MoveResult == EPathFollowingRequestResult::AlreadyAtGoal) {
		aiCtrller->OnRtMoveCompleted.Clear();
		return EBTNodeResult::Succeeded;
	}

	npc->UpdateAnimState(npcState.MoveType);
	npc->SetIsUpdateAct(true);
	return EBTNodeResult::InProgress;
}

void URtBTT_MoveToLocation::OnMoveCompleted(ARtAIController* aiCtrller, uint8 Result)
{
	FinishLatentTask(*aiCtrller->BehaviorTreeComponent, (EBTNodeResult::Type)Result);
}

//////////////////////////////////////////////////
// Attack
//////////////////////////////////////////////////

URtBTT_Attack::URtBTT_Attack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Attack");
	bNotifyTick = true;

}

EBTNodeResult::Type URtBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ARtNpcObjectActor* npc = GetNpc(OwnerComp);
	
	APawn* target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Target")));

	if (IsAttackTargetLive(npc, target)) {
		npc->UpdateAnimState(ERtAnimType::EAttack);
	
		if (!npc->NpcBehaviorStateUpdateDelegate.IsAlreadyBound(this, &URtBTT_Attack::AbortNotAttack))
			npc->NpcBehaviorStateUpdateDelegate.AddDynamic(this, &URtBTT_Attack::AbortNotAttack);

		npc->SetIsUpdateAct(true);
		return EBTNodeResult::InProgress;
	}

	FRtNpcBehaviorState npcState = npc->GetNpcBehaviorState();
	npcState.SctFunCmd = ERtSctFunCmd::ENone;
	npcState.SctTarget.Reset();
	npcState.SctString.Empty();
	npc->UpdateNpcBehaviorState(npcState);

	return EBTNodeResult::Failed;
}

void URtBTT_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ARtNpcObjectActor* npc = GetNpc(OwnerComp);
	AActor* target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Target")));
	if (target == nullptr) {
		AbortNotAttack(npc);
		return;
	}

	if (!IsAttackTargetLive(npc, target)) {
		AbortNotAttack(npc);
		return;
	}

	FVector endLoc;
	if (ARtTraineePawn* targetTrainee = Cast<ARtTraineePawn>(target))
		endLoc = targetTrainee->GetTraineeLocActor()->GetActorLocation();
	else
		endLoc = target->GetActorLocation();
	
	FVector2D start = FVector2D(npc->GetActorLocation());
	FVector2D end = FVector2D(endLoc);

	float dist = FVector::Distance(npc->GetActorLocation(), endLoc);
	if (dist > MaxAttackDistance) {
		if (ARtObjectActor* obj = Cast<ARtObjectActor>(target)) {
			if (obj->GetTctType2() == ERtTctType2::ENormal) {
				FVector lineStart = npc->GetActorLocation();
				FVector lineEnd = obj->GetActorLocation();

				TArray<FHitResult> hitResults;
				TArray<AActor*> actorsToIgnore = { npc };
				EDrawDebugTrace::Type debugLineType = EDrawDebugTrace::None;

				TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // 히트 가능한 오브젝트 유형들.
				ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
				ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

				if (UKismetSystemLibrary::LineTraceMultiForObjects(npc->GetWorld(), lineStart, lineEnd, ObjectTypes, false, actorsToIgnore, debugLineType, hitResults, true)) {
					for (const FHitResult& hitResult : hitResults) {
						if (hitResult.GetActor() == obj) {
							endLoc = hitResult.ImpactPoint;
							end = FVector2D(endLoc);

							dist = FVector::Distance(npc->GetActorLocation(), endLoc);
							if (dist > MaxAttackDistance) {
								AbortNotAttack(npc);
								return;
							}
							break;
						}
					}
				}

			}
			else {
				AbortNotAttack(npc);
			}

		}
		else {
			AbortNotAttack(npc);
			return;
		}
	}

	FVector direction = FVector(end - start, 0.0f).GetSafeNormal();
	if (!direction.IsNearlyZero())
	{
		FRotator newRotation = direction.Rotation();
		npc->SetActorRotation(FMath::RInterpTo(npc->GetActorRotation(), newRotation, DeltaSeconds, 100.0f));
	}
}

void URtBTT_Attack::AbortNotAttack(ARtNpcObjectActor* npc)
{
	const FRtNpcBehaviorState& npcState = npc->GetNpcBehaviorState();
	UBehaviorTreeComponent* btComp = Cast<ARtAIController>(npc->GetController())->BehaviorTreeComponent;
	npc->NpcBehaviorStateUpdateDelegate.RemoveDynamic(this, &URtBTT_Attack::AbortNotAttack);
	npc->SetIsUpdateAct(true);
	FinishLatentTask(*btComp, EBTNodeResult::Succeeded);
}

//////////////////////////////////////////////////
// Idle
//////////////////////////////////////////////////

URtBTT_Idle::URtBTT_Idle(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Idle");
	bTickIntervals = true;
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type URtBTT_Idle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if (IsIdle(OwnerComp)) {
		ARtNpcObjectActor* npc = GetNpc(OwnerComp);
		ARtAIController* aiCtrller = Cast<ARtAIController>(npc->GetController());
		aiCtrller->StopMove();
		npc->UpdateAnimState(ERtAnimType::EIdle);

		if (!npc->NpcBehaviorStateUpdateDelegate.IsAlreadyBound(this, &URtBTT_Idle::AbortNotIdle))
			npc->NpcBehaviorStateUpdateDelegate.AddDynamic(this, &URtBTT_Idle::AbortNotIdle);

		npc->SetIsUpdateAct(true);

		SetNextTickTime(NodeMemory, WaitTime);

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void URtBTT_Idle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	SetNextTickTime(NodeMemory, 0.0f);
	ARtNpcObjectActor* npc = GetNpc(OwnerComp);
	npc->NpcBehaviorStateUpdateDelegate.RemoveDynamic(this, &URtBTT_Idle::AbortNotIdle);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

bool URtBTT_Idle::IsIdle(UBehaviorTreeComponent& OwnerComp)
{
	ARtNpcObjectActor* npc = GetNpc(OwnerComp);
	const FRtNpcBehaviorState& npcState = GetNpc(OwnerComp)->GetNpcBehaviorState();

	return npc->IsVisible() &&
		npcState.ActType != ERtAnimType::ECollapse
		&& npcState.ActType != ERtAnimType::ESurrender
		&& npcState.SctFunCmd == ERtSctFunCmd::ENone
		&& OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Target")) == nullptr;
}

void URtBTT_Idle::AbortNotIdle(ARtNpcObjectActor* npc)
{
	UBehaviorTreeComponent* btComp = Cast<ARtAIController>(npc->GetController())->BehaviorTreeComponent;
	if (IsIdle(*btComp) == false) {
		npc->NpcBehaviorStateUpdateDelegate.RemoveDynamic(this, &URtBTT_Idle::AbortNotIdle);
		npc->SetIsUpdateAct(true);
		FinishLatentTask(*btComp, EBTNodeResult::Succeeded);
	}
}

URtBTT_EctSct::URtBTT_EctSct(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("EctSct");
}

EBTNodeResult::Type URtBTT_EctSct::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ARtNpcObjectActor* npc = GetNpc(OwnerComp);
	const FRtNpcBehaviorState& npcState = npc->GetNpcBehaviorState();

	switch (npcState.SctFunCmd)
	{
	case ERtSctFunCmd::PlayAnim:
	{
		if (!npc->NpcBehaviorStateUpdateDelegate.IsAlreadyBound(this, &URtBTT_EctSct::AbortChangeSct))
			npc->NpcBehaviorStateUpdateDelegate.AddDynamic(this, &URtBTT_EctSct::AbortChangeSct);

		ARtAIController* aiCtrller = Cast<ARtAIController>(npc->GetController());
		npc->SetIsUpdateAct(true);

		return EBTNodeResult::InProgress;
	}
	break;
	}

	return EBTNodeResult::Failed;
}

void URtBTT_EctSct::AbortChangeSct(ARtNpcObjectActor* npc)
{
	const FRtNpcBehaviorState& npcState = npc->GetNpcBehaviorState();
	UBehaviorTreeComponent* btComp = Cast<ARtAIController>(npc->GetController())->BehaviorTreeComponent;
	npc->NpcBehaviorStateUpdateDelegate.RemoveDynamic(this, &URtBTT_EctSct::AbortChangeSct);
	npc->SetIsUpdateAct(true);
	FinishLatentTask(*btComp, EBTNodeResult::Succeeded);
}
