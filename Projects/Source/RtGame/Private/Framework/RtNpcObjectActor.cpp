// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/RtNpcObjectActor.h"
#include "Framework/RtAIController.h"
#include "Framework/RtTraineePawn.h"
#include "Manager/RtGameScenarioController.h"

#include "Common/RtBlueprintAssetPath.h"

#include "GameFramework/FloatingPawnMovement.h"
#include <Kismet/GameplayStatics.h>
#include "BehaviorTree/BehaviorTreeComponent.h"

#include "RtBluePrintCommonUtil.h"
#include "RtDefines.h"

#define CheckFallHeight 90.0f // 낙하 체크 높이
#define GravitionalForce 980.0f // 중력 가속도
#define TerminalVelocity 5000.0f // 종단 속도 53 ~ 56m/s (공기저항 == 중력 가속도)

// Sets default values
ARtNpcObjectActor::ARtNpcObjectActor()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	static ConstructorHelpers::FClassFinder<AAIController> aiControllerFinder(RtSctPath::RtAIControllerPath);
	AIControllerClass = aiControllerFinder.Class;
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	FloatingPawnMovement->MaxSpeed = 50.0f;
}

// Called when the game starts or when spawned
void ARtNpcObjectActor::BeginPlay()
{
	Super::BeginPlay();

	AIController = MakeWeakObjectPtr(Cast<ARtAIController>(GetController()));
	UE_LOG(LogTemp, Warning, TEXT("%s NetMode : %d"), *GetName(), GetNetMode());
}

void ARtNpcObjectActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (!ARtGameScenarioController::Get(GetWorld()))
		return;
	ARtGameScenarioController::Get(GetWorld())->SetSctGamePlay.RemoveDynamic(this, &ARtNpcObjectActor::SetGamePlay);
	NpcBehaviorStateUpdateDelegate.Clear();
}

void ARtNpcObjectActor::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AIController = Cast<ARtAIController>(NewController);
}

// Called every frame
void ARtNpcObjectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!ARtGameScenarioController::Get(GetWorld()))
		return;

	if (ARtGameScenarioController::Get(GetWorld())->bIsGamePlay && AIController != nullptr)
	{
		Gravity(DeltaTime);
		if (AIController->GetMoveStatus() == EPathFollowingStatus::Moving)
			LookAtMoveDirection(DeltaTime);
		else if (GetNpcActState().Target.IsValid())
			LookAtAttackTarget(DeltaTime);
	}

}

void ARtNpcObjectActor::Gravity(float DeltaTime)
{
	FVector loc = GetActorLocation();
	FVector start = loc + FVector(0.0f, 0.0f, CheckFallHeight);

	FHitResult hitResult;
	TArray<AActor*> actorsToIgnore = { this };
	EDrawDebugTrace::Type debugLineType = EDrawDebugTrace::None;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // 히트 가능한 오브젝트 유형들.
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), start, loc,
		ObjectTypes, true, actorsToIgnore, debugLineType, hitResult, true, FLinearColor::Red, FLinearColor::Green, 1.0f))
	{
		loc.Z = hitResult.Location.Z;
		DropSpeed = 0.0f;

	}
	else
	{
		if (DropSpeed >= TerminalVelocity) // 종단 속도 체크
			DropSpeed = TerminalVelocity;
		else
			DropSpeed += GravitionalForce * DeltaTime; // 중력 가속도에 의한 낙하 속도

		loc.Z -= DropSpeed * DeltaTime; // 낙하

		// 낙하 속도로 인한 순간이동 방지
		if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), start, loc,
			ObjectTypes, true, actorsToIgnore, debugLineType, hitResult, true, FLinearColor::Red, FLinearColor::Green, 1.0f))
		{
			loc.Z = hitResult.Location.Z;
			DropSpeed = 0.0f;

		}
	}
	SetActorLocation(loc);
}

void ARtNpcObjectActor::LookAtMoveDirection(float DeltaTime)
{
	FVector velocity = GetVelocity();
	velocity.Z = 0.0f;
	FVector direction = velocity.GetSafeNormal();
	if (!direction.IsNearlyZero())
	{
		FRotator newRotation = direction.Rotation();
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), newRotation, DeltaTime, 100.0f));
	}
}

void ARtNpcObjectActor::LookAtAttackTarget(float DeltaTime)
{
	AActor* target = GetNpcActState().Target.Get();

	FVector endLoc;
	if (ARtTraineePawn* targetTrainee = Cast<ARtTraineePawn>(target))
		endLoc = targetTrainee->GetTraineeLocActor()->GetActorLocation();
	else
		endLoc = target->GetActorLocation();

	FVector2D start = FVector2D(GetActorLocation());
	FVector2D end = FVector2D(endLoc);

	FVector direction = FVector(end - start, 0.0f).GetSafeNormal();
	if (!direction.IsNearlyZero())
	{
		FRotator newRotation = direction.Rotation();
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), newRotation, DeltaTime, 100.0f));
	}
}

void ARtNpcObjectActor::CreateAICtrller()
{
	if (AIController.IsValid())
		return;

	TSubclassOf<ARtAIController> aiControllerClass = LoadClass<ARtAIController>(nullptr, RtSctPath::RtAIControllerPath);
	ARtAIController* rtAiCtrller = GetWorld()->SpawnActor<ARtAIController>(aiControllerClass, FTransform());
	rtAiCtrller->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	rtAiCtrller->Possess(this);
	AIController = MakeWeakObjectPtr(Cast<ARtAIController>(GetController()));
}

void ARtNpcObjectActor::InitNpc()
{
	if (GetTctStruct() == nullptr)
		return;

	ARtGameScenarioController::Get(GetWorld())->SetSctGamePlay.AddDynamic(this, &ARtNpcObjectActor::SetGamePlay);

	AnimMap = {
		{ ERtAnimType::EIdle, {TOOL_SYSTEM_FBX_PATH_DEFAULT_ANIM_IDLE, true, true}},
		{ ERtAnimType::EWalk, {TOOL_SYSTEM_FBX_PATH_DEFAULT_ANIM_WALK, true, true} },
		{ ERtAnimType::ERun, {TOOL_SYSTEM_FBX_PATH_DEFAULT_ANIM_RUN, true, true}},
		{ ERtAnimType::EAttack, {TOOL_SYSTEM_FBX_PATH_DEFAULT_ANIM_ATTACK, true, true}},
		{ ERtAnimType::EDefend, {TOOL_SYSTEM_FBX_PATH_DEFAULT_ANIM_DEFEND, true, true}},
		{ ERtAnimType::ECollapse, {TOOL_SYSTEM_FBX_PATH_DEFAULT_ANIM_COLLAPSE, false, true}},
		{ ERtAnimType::ESurrender, {TOOL_SYSTEM_FBX_PATH_DEFAULT_ANIM_SURRENDER, false, true}},
		{ ERtAnimType::ELieDown, {TOOL_SYSTEM_FBX_PATH_DEFAULT_ANIM_LIE_DOWN, false, true}},
		{ ERtAnimType::ERestrain, {TOOL_SYSTEM_FBX_PATH_DEFAULT_ANIM_RESTRAIN, false, true}},
		{ ERtAnimType::EStandUp, {TOOL_SYSTEM_FBX_PATH_DEFAULT_ANIM_STAND_UP, false, true}},
		{ ERtAnimType::EEscort, {TOOL_SYSTEM_FBX_PATH_DEFAULT_ANIM_ESCORT, true, true}}
	};

	const TArray<FRtTctComDirection>& dataList = GetTctStruct()->ComDirectionList;
	if (dataList.IsEmpty())
		return;
	FRtTctComDirection data = dataList[0];
	if (data.DirectionItemList.IsEmpty())
		return;

	for (const FRtTctComDirectionItem animData : data.DirectionItemList)
	{
		if (animData.AnimationType != ERtAnimType::ENone)
			AnimMap.Add(animData.AnimationType, { animData.AnimationFileName, AnimMap.FindRef(animData.AnimationType).Get<1>(), false });
	}

	InitNpcState();
}

void ARtNpcObjectActor::SetVisible(bool bNewVisible)
{
	Super::SetVisible(bNewVisible);

	if (AIController.IsValid())
		AIController->StopMovement();

	//if (GetNetMode() != NM_Client)
	//	UpdateNpcBehaviorState(FRtNpcBehaviorState());
}

void ARtNpcObjectActor::SctMoveToTarget(AActor* target, FString animName, ERtAnimType animType)
{
	if (GetTctStruct() == nullptr)
		return;

	FRtNpcBehaviorState npcState = NpcBehaviorState;
	if (npcState.ActType != ERtAnimType::ECollapse)
		npcState.ActType = ERtAnimType::ENone;
	npcState.MoveType = animType;
	npcState.SctFunCmd = ERtSctFunCmd::NpcMoveToWayPoint;
	npcState.SctTarget = MakeWeakObjectPtr(target);
	npcState.SctString = animName;

	UpdateNpcBehaviorState(npcState);
	PlayAnim(animType, ERtSctFunCmd::NpcMoveToWayPoint, animName, true);
}

void ARtNpcObjectActor::SctAttackTarget(AActor* target)
{
	if (GetTctStruct() == nullptr)
		return;

	FRtNpcBehaviorState npcState = NpcBehaviorState;
	if (npcState.ActType != ERtAnimType::ECollapse)
		npcState.ActType = ERtAnimType::ENone;
	npcState.MoveType = ERtAnimType::ERun;
	npcState.SctFunCmd = ERtSctFunCmd::AttackTarget;
	npcState.SctTarget = MakeWeakObjectPtr(target);

	UpdateNpcBehaviorState(npcState);
}

void ARtNpcObjectActor::SctStopAct()
{
	StopAct();
	return;

	FRtNpcBehaviorState npcState = NpcBehaviorState;
	npcState.MoveType = ERtAnimType::EWalk;
	npcState.SctFunCmd = ERtSctFunCmd::ENone;
	npcState.SctTarget.Reset();
	npcState.SctString.Empty();

	UpdateNpcBehaviorState(npcState);
}

void ARtNpcObjectActor::SctPlayAnim(ERtAnimType animType, ERtSctFunCmd cmd, FString animName, bool isLoop)
{
	FRtNpcBehaviorState state = GetNpcBehaviorState();

	if (state.ActType == ERtAnimType::ECollapse)
		return;

	if (!state.SctString.IsEmpty() && !animName.IsEmpty() && state.SctString == animName)
		return;

	state.ActType = animType;
	state.SctFunCmd = ERtSctFunCmd::PlayAnim;
	state.SctString = animName;
	state.SctBool = isLoop;
	UpdateNpcBehaviorState(state);
}

void ARtNpcObjectActor::MoveToActor(AActor* targetActor, FString animName, ERtAnimType animType)
{
	if (GetTctStruct() == nullptr)
		return;

	UpdateSpeed(animType);
	AIController->MoveToActor(targetActor);
#if WITH_EDITOR
	auto* objectActor = Cast<ARtObjectActor>(targetActor);
	if (objectActor)
	{
		auto* tctStruct = objectActor->GetTctStruct();
		if (tctStruct)
		{
			APP_LOG("ARtNpcObjectActor::MoveToActor -> tctType : %d, Object Name %s", tctStruct->ComDefault.TctType, *objectActor->GetObjectName());
		}
	}
#endif

	UpdateAnimState(animType, animName, false);
}

void ARtNpcObjectActor::MoveToLocation(FVector moveLoc)
{
	if (GetTctStruct() == nullptr)
		return;

	UpdateSpeed(ERtAnimType::EWalk);
	AIController->MoveToLocation(moveLoc);
	UpdateAnimState(ERtAnimType::EWalk);
}

void ARtNpcObjectActor::StopMove()
{
	if(AIController.IsValid())
		AIController->StopMove();
}

void ARtNpcObjectActor::UpdateAnimState(ERtAnimType newState, FString animFileName, bool bLooping)
{
	if (newState != ERtAnimType::ENone && newState == NpcAnimState)
	{
		UE_LOG(LogTemp, Log, TEXT("%s / %s"), *(UEnum::GetValueAsString(newState)), *(UEnum::GetValueAsString(NpcAnimState)));
		return;
	}

	NpcAnimState = newState;
	NpcBehaviorState.ActType = newState;

	if (NpcAnimState == ERtAnimType::ENone)
	{
		//StopAnimation();
		return;
	}

	const TArray<FRtTctComBehavior>& dataList = GetTctStruct()->ComBehaviorList;
	if (dataList.IsEmpty())
		return;
	FRtTctComBehavior data = dataList[0];
	float animSpeed = data.GetDefaultAnimSpeed(newState);
	TTuple<FString, bool, bool> animData = AnimMap.FindRef(NpcAnimState);
	bool bUseLooping = animData.Get<1>();

	if (animFileName.IsEmpty())
	{
		bool bOverrideAnim = false;
		FString useAnimFileName;
		const TArray<FRtTctComDirection>& directionList = GetTctStruct()->ComDirectionList;
		if (!directionList.IsEmpty()) {
			for (auto& item : directionList[0].DirectionItemList)
			{
				if (item.AnimationType == newState)
				{
					bOverrideAnim = true;
					useAnimFileName = item.AnimationFileName;
					break;
				}
			}
		}

		if (!bOverrideAnim)
		{
			useAnimFileName = animData.Get<0>();
		}

#if WITH_EDITOR
		FILE_APP_LOG(
			"server_debug.log",
			"ARtNpcObjectActor::UpdateAnimState() 1 - ani : %s, type : %s, loop : %d, override : %d",
			*useAnimFileName,
			*URtResourceModelSubsystem::GetEnumToString(TEXT("ERtAnimType"), (int32)newState),
			(int32)bUseLooping,
			(int32)bOverrideAnim
		);
#endif

		PlayAnimation(useAnimFileName, bUseLooping, animSpeed, !bOverrideAnim);
	}
	else
	{
#if WITH_EDITOR
		FILE_APP_LOG(
			"server_debug.log",
			"ARtNpcObjectActor::UpdateAnimState() 2 - ani : %s, type : %s, loop : %d, override : %d",
			*animFileName,
			*URtResourceModelSubsystem::GetEnumToString(TEXT("ERtAnimType"), (int32)newState),
			(int32)bUseLooping,
			0
		);
#endif 
		PlayAnimation(animFileName, bUseLooping, animSpeed, false);
	}
}

void ARtNpcObjectActor::UpdateSpeed(ERtAnimType moveType)
{
	const TArray<FRtTctComBehavior>& dataList = GetTctStruct()->ComBehaviorList;
	if (dataList.IsEmpty())
		return;
	FRtTctComBehavior data = dataList[0];
	float curSpeed = data.GetDefaultMoveSpeed(moveType);
	//TODO: 상태에 따른 speed 변화 추가 예정

	FloatingPawnMovement->MaxSpeed = curSpeed;

}

float ARtNpcObjectActor::GetSpeed()
{
	return FloatingPawnMovement->MaxSpeed;
}

void ARtNpcObjectActor::InitNpcState()
{
	if (GetTctStruct() == nullptr)
		return;

	UpdateSpeed(ERtAnimType::EWalk);
	if (AIController.IsValid())
		AIController->StopMovement();

	//UpdateNpcBehaviorState(FRtNpcBehaviorState());

}

void ARtNpcObjectActor::PlayAnim(ERtAnimType animType, ERtSctFunCmd cmd, FString animName, bool isLoop)
{
	FRtTctStruct* tct = GetTctStruct();

	switch (animType)
	{
	case ERtAnimType::ENone:
		if (animName.IsEmpty() || animName == Cur_AnimName)
			return;

		NpcAnimState = animType;
		Cur_AnimName = animName;

		PlayAnimation(animName, isLoop, -1, false);

		if (tct)
		{
			for (FRtTctComDirectionItem item : tct->ComDirectionList[0].DirectionItemList)
			{
				if (item.AnimationFileName == animName)
				{
					NpcAnimState = item.AnimationType;
					break;
				}
			}
		}

		break;
	default:
		TTuple<FString, bool, bool>* tuple = AnimMap.Find(animType);
		if (!tuple)
			return;

		float animSpeed = -1;
		if (tct)
			animSpeed = tct->ComBehaviorList[0].GetDefaultAnimSpeed(animType);

		NpcAnimState = animType;
		Cur_AnimName = tuple->Get<0>();
		PlayAnimation(tuple->Get<0>(), tuple->Get<1>(), animSpeed, tuple->Get<2>());

		break;
	}

	UpdateSpeed(NpcAnimState);
}

void ARtNpcObjectActor::MoveToTarget(AActor* target, FString animName, ERtAnimType animType)
{
	if (AIController.IsValid())
		AIController->MoveToActor(target);

	PlayAnim(animType, ERtSctFunCmd::NpcMoveToWayPoint, animName, true);
}

void ARtNpcObjectActor::StopAct()
{
	if (AIController.IsValid())
		AIController->StopMovement();
}

bool ARtNpcObjectActor::UpdateNpcBehaviorState(FRtNpcBehaviorState newState)
{
	if (GetTctStruct() == nullptr)
		return false;

	const TArray<FRtTctComBehavior>& dataList = GetTctStruct()->ComBehaviorList;
	if (dataList.IsEmpty())
		return false;

	FRtNpcBehaviorState oldState = NpcBehaviorState;
	NpcBehaviorState = newState;

	bool isUpdate = false;

	if (oldState.ActType != NpcBehaviorState.ActType)
	{

		if (!IsVisible())
		{
			isUpdate = true;
			UpdateSpeed(NpcBehaviorState.MoveType);
		}
		else if (NpcBehaviorState.ActType == ERtAnimType::ECollapse)
		{
			NpcBehaviorState = FRtNpcBehaviorState();
			NpcBehaviorState.ActType = ERtAnimType::ECollapse;
			NpcBehaviorState.SctFunCmd = ERtSctFunCmd::PlayAnim;
			isUpdate = true;
		}
		else if (NpcBehaviorState.ActType == ERtAnimType::ESurrender)
		{
			NpcBehaviorState = FRtNpcBehaviorState();
			NpcBehaviorState.ActType = ERtAnimType::ESurrender;
			NpcBehaviorState.SctFunCmd = ERtSctFunCmd::PlayAnim;
			isUpdate = true;
		}
		else if ((oldState.ActType == ERtAnimType::ECollapse
			|| oldState.ActType == ERtAnimType::ESurrender)
			&& (NpcBehaviorState.ActType == ERtAnimType::ENone))
		{
			isUpdate = true;
		}
	}

	if (!isUpdate)
	{
		if (oldState.SctFunCmd != NpcBehaviorState.SctFunCmd)
		{
			isUpdate = true;
		}
		if (oldState.MoveType != NpcBehaviorState.MoveType)
		{
			isUpdate = true;
		}
		if (oldState.SctTarget != NpcBehaviorState.SctTarget)
		{
			isUpdate = true;
		}
		if (oldState.SctString != NpcBehaviorState.SctString)
		{
			isUpdate = true;
		}

		if (!isUpdate)
			return false;
	}

	if (oldState.MoveType != NpcBehaviorState.MoveType)
	{
		UpdateSpeed(NpcBehaviorState.MoveType);
	}

	switch (NpcBehaviorState.SctFunCmd)
	{
	case ERtSctFunCmd::ENone:
		NpcBehaviorState.SctTarget.Reset();
		NpcBehaviorState.SctString.Empty();
		break;
	case ERtSctFunCmd::AttackTarget:
		NpcBehaviorState.MoveType = ERtAnimType::ERun;
		NpcBehaviorState.SctString.Empty();
	case ERtSctFunCmd::NpcMoveToWayPoint:
		if (NpcBehaviorState.MoveType == ERtAnimType::ENone)
		{
			UpdateSpeed(ERtAnimType::EWalk);
			//UpdateSpeed(NpcBehaviorState.MoveType);
			PlayAnimation(NpcBehaviorState.SctString, true);
		}
		else
			NpcBehaviorState.SctString.Empty();
		if (!NpcBehaviorState.SctTarget.IsValid())
		{
			NpcBehaviorState.SctFunCmd = ERtSctFunCmd::ENone;
			NpcBehaviorState.SctString.Empty();
		}
		break;
	case ERtSctFunCmd::PlayAnim:
		if (NpcBehaviorState.ActType == ERtAnimType::ENone)
		{
			NpcBehaviorState.SctTarget.Reset();
			UpdateAnimState(ERtAnimType::ENone, NpcBehaviorState.SctString, NpcBehaviorState.SctBool);
			PlayAnimation(NpcBehaviorState.SctString, NpcBehaviorState.SctBool);
		}
		else
		{
			NpcBehaviorState.SctTarget.Reset();
			UpdateAnimState(NpcBehaviorState.ActType);
		}
		break;
	}


	if (!isUpdate)
		return false;


	AIController->StopMove();
	AIController->UpdateBlackboard();

	NpcBehaviorStateUpdateDelegate.Broadcast(this);

#if WITH_EDITOR

	FILE_APP_LOG(
		"server_debug.log",
		"ARtNpcObjectActor::UpdateNpcBehaviorState() ActType : %s(%d), MoveType : %s(%d), SctFunCmd : %s, SctString : %s",
		*URtResourceModelSubsystem::GetEnumToString(TEXT("ERtAnimType"), (int32)NpcBehaviorState.ActType), (int32)NpcBehaviorState.ActType,
		*URtResourceModelSubsystem::GetEnumToString(TEXT("ERtAnimType"), (int32)NpcBehaviorState.MoveType), (int32)NpcBehaviorState.MoveType,
		*URtResourceModelSubsystem::GetEnumToString(TEXT("ERtSctFunCmd"), (int32)NpcBehaviorState.SctFunCmd),
		*NpcBehaviorState.SctString
	);

#endif 
	return true;
}

bool ARtNpcObjectActor::UpdateNpcActState(FRtNpcActState newState)
{
	NpcActState = newState;

	return true;
}

void ARtNpcObjectActor::SetGamePlay(bool isGamePlay)
{
	if (GetNetMode() != ENetMode::NM_Client)
	{
		InitNpcState();
		AIController->SetBehaviorTreeMode(false);
	}

	StopMove();

	UpdateAnimState(isGamePlay ? ERtAnimType::EIdle : ERtAnimType::ENone);

}

const ERtRoleType ARtNpcObjectActor::GetRoleType()
{
	if (FRtTctStruct* tctStructData = GetTctStruct())
		return tctStructData->ComBehaviorList[0].RoleType;

	return ERtRoleType::ENone;
}

void ARtNpcObjectActor::SetIsUpdateAct(bool bSet)
{
	IsUpdateAct = bSet;
}

void ARtNpcObjectActor::SetCollisionType(UShapeComponent* collisionComp, ERtCollisionType collisionType)
{
	Super::SetCollisionType(collisionComp, collisionType);
	collisionComp->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
}

bool FRtNpcState::operator==(const FRtNpcState& dst) const
{
	if (this->SctFunCmd != dst.SctFunCmd)
		return false;

	if (this->AnimType == ERtAnimType::ENone || dst.AnimType == ERtAnimType::ENone)
		return this->AnimName == dst.AnimName;
	else
		return this->AnimType == dst.AnimType;

	return false;
}

bool FRtNpcState::operator!=(const FRtNpcState& dst) const
{
	return !(*this == dst);
}
