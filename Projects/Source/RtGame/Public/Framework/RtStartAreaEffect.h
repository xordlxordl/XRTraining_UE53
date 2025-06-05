// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "RtJsonFunctionLib.h"
#include "NavigationPath.h"
#include "RtStartAreaEffect.generated.h"

class ARtTraineePawn;

UCLASS()
class ARtStartAreaEffect : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARtStartAreaEffect(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	bool CreateDynamicMaterial();

	UFUNCTION()
	void OnStartTraining(bool isStart);
	void SetEffectColor(const FColor& color);
	void Check_Area();

public:
	void OnChangeInPlayer(bool isIn);
	void SetPlayer(ARtTraineePawn* pPlayer);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UMaterialInstanceDynamic> MID;
	
	UPROPERTY(EditAnywhere)
	FName ColorParameterName;

	TObjectPtr<ARtTraineePawn> player;

	const float fCheckRange = 100; // cm

// Navigation 경로 표시용
//public:
//	UFUNCTION()
//	void Setup_Path(UNavigationPath* AffectedPath, TEnumAsByte<ENavPathEvent::Type> PathEvent);
//
//protected:
//	TObjectPtr<class UNavigationPath> NavPath;
//
//	UPROPERTY(EditAnywhere)
//	TObjectPtr<class USplineComponent> PathSpline;
};
