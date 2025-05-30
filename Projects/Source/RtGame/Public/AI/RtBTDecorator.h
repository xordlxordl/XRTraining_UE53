// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "RtBTDecorator.generated.h"

class ARtNpcObjectActor;
class ARtObjectActor;

UCLASS()
class RTGAME_API URtBTDecorator : public UBTDecorator
{
	GENERATED_BODY()

public:
protected:
	ARtNpcObjectActor* GetNpc(UBehaviorTreeComponent& OwnerComp) const;
	bool IsSct(UBehaviorTreeComponent& OwnerComp) const;
	bool CanMoveTo(UBehaviorTreeComponent& OwnerComp, FVector endLoc) const;
	FVector GetLocation(AActor* target) const;
};

UCLASS()
class RTGAME_API URtBTD_Sct : public URtBTDecorator
{
	GENERATED_BODY()

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

};

UCLASS()
class RTGAME_API URtBTD_TargetSct : public URtBTDecorator
{
	GENERATED_BODY()

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

};

UCLASS()
class RTGAME_API URtBTD_CanMoveTo : public URtBTDecorator
{
	GENERATED_BODY()

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

};

UCLASS()
class RTGAME_API URtBTD_FindAttackTarget : public URtBTDecorator
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float FindRange = 500.0f;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

};
