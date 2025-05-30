// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RtObjectActor.h"
#include "RtNpcObjectActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNpcBehaviorStateUpdateDelegate, class ARtNpcObjectActor*, Npc);

class ARtGameScenarioController;

USTRUCT(BlueprintType)
struct FRtNpcBehaviorState // 비헤이비어 트리를 돌리기 위한 상태 변수
{
	GENERATED_USTRUCT_BODY()
public:
	FRtNpcBehaviorState() {}

	ERtAnimType ActType = ERtAnimType::ENone;
	ERtAnimType MoveType = ERtAnimType::EWalk;
	ERtSctFunCmd SctFunCmd = ERtSctFunCmd::ENone;
	TWeakObjectPtr<AActor> SctTarget = nullptr;
	FString SctString = "";
	bool SctBool = false;
};

USTRUCT(BlueprintType)
struct FRtNpcActState // 비헤이비어 트리에 의한 결과 변수
{
	GENERATED_USTRUCT_BODY()
public:
	FRtNpcActState() {}

	ERtSctFunCmd SctFunCmd = ERtSctFunCmd::ENone;
	TWeakObjectPtr<AActor> Target = nullptr;
	FString SctString = "";
};

USTRUCT(BlueprintType)
struct FRtNpcState
{
	GENERATED_USTRUCT_BODY()
public:
	FRtNpcState() {}

	ERtSctFunCmd SctFunCmd = ERtSctFunCmd::ENone;
	ERtAnimType AnimType = ERtAnimType::ENone;
	FString AnimName = "";

	bool operator == (const FRtNpcState& dst) const;
	bool operator != (const FRtNpcState& dst) const;
};

UCLASS()
class RTGAME_API ARtNpcObjectActor : public ARtObjectActor
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARtNpcObjectActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFloatingPawnMovement* FloatingPawnMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PossessedBy(AController* NewController) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	friend class ARtAIController;
	TWeakObjectPtr<class ARtAIController> AIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERtAnimType NpcAnimState = ERtAnimType::ENone;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Cur_AnimName = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRtNpcState CurState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRtNpcState PrevState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRtNpcBehaviorState NpcBehaviorState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRtNpcActState NpcActState;
	
	TMap<ERtAnimType, TTuple<FString, bool, bool>> AnimMap; // NpcAnimState, <AnimFileName, IsLoop, IsOverrided>

	void Gravity(float DeltaTime);
	void LookAtMoveDirection(float DeltaTime);
	void LookAtAttackTarget(float DeltaTime);

	float DropSpeed = 0.0f; // 낙하 속도

public:
	FString Get_CurrentAnimName()
	{
		return Cur_AnimName;
	}

	FString Find_AnimName(ERtAnimType animType)
	{
		TTuple<FString, bool, bool>* pTuple = AnimMap.Find(animType);
		return (pTuple ? FPaths::GetCleanFilename(pTuple->Get<0>()) : TEXT(""));
	}

	// New Functions
public:
	void InitNpc();
	void SetVisible(bool bNewVisible) override;
	void InitNpcState();
	void PlayAnim(ERtAnimType animType, ERtSctFunCmd cmd, FString animName, bool isLoop);
	void MoveToTarget(AActor* target, FString animName, ERtAnimType animType);
	void StopAct();

public:
	void CreateAICtrller();

	// SCT로 상태 변경: 리슨
	void SctMoveToTarget(AActor* target, FString animName, ERtAnimType animType);
	void SctAttackTarget(AActor* target);
	void SctStopAct();
	void SctPlayAnim(ERtAnimType animType, ERtSctFunCmd cmd, FString animName, bool isLoop);

	// NPC 이동: 클라
	void MoveToActor(AActor* targetActor, FString animName, ERtAnimType animType);
	void MoveToLocation(FVector moveLoc);
	void StopMove();

	void UpdateAnimState(ERtAnimType newState, FString animFileName = TEXT(""), bool bLooping = false);
	void UpdateSpeed(ERtAnimType moveType);
	float GetSpeed() ;

	bool UpdateNpcBehaviorState(FRtNpcBehaviorState newState);
	bool UpdateNpcActState(FRtNpcActState newState);

	const FRtNpcBehaviorState& GetNpcBehaviorState() { return NpcBehaviorState; }
	const FRtNpcActState& GetNpcActState() { return NpcActState; }

	UFUNCTION()
	void SetGamePlay(bool isGamePlay);
	
	FNpcBehaviorStateUpdateDelegate NpcBehaviorStateUpdateDelegate;

	const ERtRoleType GetRoleType();
	
	ARtAIController* GetAIController() { return AIController.Get(); }

	void SetIsUpdateAct(bool bSet);
	bool GetIsUpdateAct() {
		return IsUpdateAct;
	}

	void SetCollisionType(UShapeComponent* collisionComp, ERtCollisionType collisionType) override;
protected:

	bool IsUpdateAct = false;

};
