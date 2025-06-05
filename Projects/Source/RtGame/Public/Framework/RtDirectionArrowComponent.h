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
	// ��ǥ ���� ����, NULL�� ��� ǥ�� ����
	void SetTargetActor(AActor* pActor);

	// SetTargetActor(NULL), �Ʒ� ���۽� ��������Ʈ�� ���� �߰��� �Լ�.
	UFUNCTION()
	void ResetTarget(bool isStart = true);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ARtTraineePawn> Player;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> TargetActor;

	const float fArrowHeight = 100;	// �ٴ����κ��� ����
	const float fCheckNear = 100;	// ����� ��� ǥ�� ����
};
