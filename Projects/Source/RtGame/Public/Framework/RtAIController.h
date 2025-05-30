// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RtAIController.generated.h"

/**
 * 
 */

class ARtObjectActor;
class ARtNpcObjectActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRtMoveCompleted, class ARtAIController*, aiCtrller, uint8, result);

UCLASS()
class RTGAME_API ARtAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ARtAIController(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* BehaviorTree;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTreeComponent* BehaviorTreeComponent;

public:
	ARtNpcObjectActor* GetNpc();
	void SetBehaviorTreeMode(bool isGamePlay);

	void UpdateBlackboard();

	UObject* GetBlackboardTarget();
	FVector GetBlackboardFreeMove();

	void StopMove();

	FRtMoveCompleted OnRtMoveCompleted;

protected:
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

	TWeakObjectPtr<ARtNpcObjectActor> Npc;

};
