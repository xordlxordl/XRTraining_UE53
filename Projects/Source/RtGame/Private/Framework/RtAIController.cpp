// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtAIController.h"
#include "Framework/RtNpcObjectActor.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ARtAIController::ARtAIController(const FObjectInitializer& ObjectInitializer)
{
	BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComp"));
	SetReplicates(false);
}

ARtNpcObjectActor* ARtAIController::GetNpc()
{
	if (!Npc.IsValid())
		Npc = MakeWeakObjectPtr<ARtNpcObjectActor>(Cast<ARtNpcObjectActor>(GetPawn()));

	return Npc.Get();
}

void ARtAIController::SetBehaviorTreeMode(bool isGamePlay)
{
	if (isGamePlay && BehaviorTree)
		RunBehaviorTree(BehaviorTree);
	else
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
}

void ARtAIController::UpdateBlackboard()
{
	ARtNpcObjectActor* npc = GetNpc();
	UBlackboardComponent* board = BehaviorTreeComponent->GetBlackboardComponent();
	FRtNpcBehaviorState curState = npc->GetNpcBehaviorState();
	board->SetValueAsEnum(FName("SctFunCmd"), static_cast<uint8>(curState.SctFunCmd));
	board->SetValueAsObject(FName("Target"), curState.SctTarget.Get());
}

UObject* ARtAIController::GetBlackboardTarget()
{
	if (UBlackboardComponent* board = BehaviorTreeComponent->GetBlackboardComponent()) {
		return board->GetValueAsObject(FName("Target"));
	}
	return nullptr;
}

FVector ARtAIController::GetBlackboardFreeMove()
{
	if (UBlackboardComponent* board = BehaviorTreeComponent->GetBlackboardComponent()) {
		return board->GetValueAsVector(FName("FreeMove"));
	}
	return FVector::ZeroVector;
}

void ARtAIController::StopMove()
{
	StopMovement();
}

void ARtAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (GetNetMode() == ENetMode::NM_Client) {
		return;
	}

	FRtNpcBehaviorState newState = GetNpc()->GetNpcBehaviorState();
	if (newState.SctFunCmd == ERtSctFunCmd::NpcMoveToWayPoint &&
		newState.SctTarget.IsValid() &&
		newState.SctTarget.Get() == GetBlackboardTarget()) {
		newState.SctTarget.Reset();
		GetNpc()->UpdateNpcBehaviorState(newState);
	}

	GetNpc()->IsUpdateAct = true;

	if (Result.Code == EPathFollowingResult::Success)
		OnRtMoveCompleted.Broadcast(this, EBTNodeResult::Succeeded);
	else
		OnRtMoveCompleted.Broadcast(this, EBTNodeResult::Failed);
	OnRtMoveCompleted.Clear();
}
