// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "RtDirectionArrowComponent.generated.h"

class AActor;
class ARtTraineePawn;
/**
 * 
 */
UCLASS(Within = RtTraineePawn)
class RTGAME_API URtDirectionArrowComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	URtDirectionArrowComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnRegister() override;

public:
	// 목표 액터 설정, NULL인 경우 표시 안함
	void SetTargetActor(AActor* pActor);

	// SetTargetActor(NULL), 훈련 시작시 델리게이트를 위해 추가한 함수.
	UFUNCTION()
	void ResetTarget(bool isStart = true);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ARtTraineePawn> Player;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> TargetActor;

	const float fArrowHeight = 100;	// 바닥으로부터 높이
	const float fCheckNear = 100;	// 가까운 경우 표시 안함
};
