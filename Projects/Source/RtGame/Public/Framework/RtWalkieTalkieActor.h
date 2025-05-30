// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RtWalkieTalkieActor.generated.h"

UCLASS()
class RTGAME_API ARtWalkieTalkieActor : public AActor
{
    GENERATED_BODY()

    public:
    // Sets default values for this actor's properties
    ARtWalkieTalkieActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // StaticMesh
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* WalkieTalkieMesh;

    // Materials
    UPROPERTY(EditAnywhere, Category = "Materials")
    UMaterialInterface* GreenMaterial;

    UPROPERTY(EditAnywhere, Category = "Materials")
    UMaterialInterface* OrangeMaterial;

    // 상태
    bool GetIsSending();

    bool GetIsActivated();
    void SetIsActivated(bool InIsActivated);

    // 입력 함수
    void ToggleScreenMaterial();

    // 무전기 이동 애니메이션
    bool GetIsMoving();
    void MoveAnimation(FVector InStartLocation, FVector InTargetLocation, float InDuration);

private:
    AActor* MenuActor = nullptr;

    bool bIsSending = false;

    bool bIsActivated = false;

    bool bIsMoving = false;

    FVector MoveStartLocation;
    FVector MoveTargetLocation;
    float MoveDuration = 1.0f;
    float MoveElapsedTime = 0.0f;
};
