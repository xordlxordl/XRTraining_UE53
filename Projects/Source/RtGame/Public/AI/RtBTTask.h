// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RtBTTask.generated.h"

class ARtObjectActor;
class ARtNpcObjectActor;

UCLASS()
class RTGAME_API URtBTTask : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	ARtNpcObjectActor* GetNpc(UBehaviorTreeComponent& OwnerComp);

	bool IsAttackTargetLive(ARtNpcObjectActor* npc, AActor* target);


};

UCLASS()
class RTGAME_API URtBTT_Down : public URtBTTask
{
	GENERATED_BODY()
	
public:
	URtBTT_Down(const FObjectInitializer& ObjectInitializer);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	bool IsDown(UBehaviorTreeComponent& OwnerComp);

	UFUNCTION()
	void AbortNotDown(ARtNpcObjectActor* npc);

};

UCLASS()
class RTGAME_API URtBTT_Surrender : public URtBTTask
{
	GENERATED_BODY()
	
public:
	URtBTT_Surrender(const FObjectInitializer& ObjectInitializer);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	bool IsSurrender(UBehaviorTreeComponent& OwnerComp);

	UFUNCTION()
	void AbortNotSurrender(ARtNpcObjectActor* npc);

};

UCLASS()
class RTGAME_API URtBTT_MoveToActor : public URtBTTask
{
	GENERATED_BODY()
	
public:
	URtBTT_MoveToActor(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, meta = (UIMin = 0, ClampMin = 0))
	float AcceptanceRadius = 5.0f; // 허용 거리

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	virtual void OnMoveCompleted(ARtAIController* aiCtrller, uint8 Result);

};

UCLASS()
class RTGAME_API URtBTT_MoveToLocation : public URtBTTask
{
	GENERATED_BODY()

public:
	URtBTT_MoveToLocation(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, meta = (UIMin = 0, ClampMin = 0))
	float AcceptanceRadius = 5.0f; // 허용 거리

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	virtual void OnMoveCompleted(ARtAIController* aiCtrller, uint8 Result);

};

UCLASS()
class RTGAME_API URtBTT_Attack : public URtBTTask
{
	GENERATED_BODY()

public:
	URtBTT_Attack(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, meta = (UIMin = 0, ClampMin = 0))
	float MaxAttackDistance = 100.0f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UFUNCTION()
	void AbortNotAttack(ARtNpcObjectActor* npc);

};

UCLASS()
class RTGAME_API URtBTT_Idle : public URtBTTask
{
	GENERATED_BODY()

public:
	URtBTT_Idle(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, meta = (UIMin = 0, ClampMin = 0))
	float WaitTime = 5.0f;

	UPROPERTY(EditAnywhere, meta = (UIMin = 0, ClampMin = 0))
	float RandomDeviation = 0.0f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	bool IsIdle(UBehaviorTreeComponent& OwnerComp);

	UFUNCTION()
	void AbortNotIdle(ARtNpcObjectActor* npc);
	
};

UCLASS()
class RTGAME_API URtBTT_EctSct : public URtBTTask
{
	GENERATED_BODY()

public:
	URtBTT_EctSct(const FObjectInitializer& ObjectInitializer);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void AbortChangeSct(ARtNpcObjectActor* npc);

};
