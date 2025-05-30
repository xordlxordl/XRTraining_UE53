// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Safety_WallDetecter.generated.h"

UCLASS()
class RTGAME_API ASafety_WallDetecter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASafety_WallDetecter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<bool> CollisionResults; // Left, Forward, Right, Back

	TArray<int32> CollisionNums; // Left, Forward, Right, Back, FL, FR, BR, BL
	TArray<bool> CollisionCheck; // Left, Forward, Right, Back, FL, FR, BR, BL

public:
	UFUNCTION(BlueprintCallable)
	void UpdateDatas(int32 directIndex, int32 wallNum, bool IsCollision);

	void SetResult();
};
