// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/Safety_WallDetecter.h"

// Sets default values
ASafety_WallDetecter::ASafety_WallDetecter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionResults.Init(0.0f, 4);
	CollisionNums.Init(-1, 8);
	CollisionCheck.Init(false, 8);
}

// Called when the game starts or when spawned
void ASafety_WallDetecter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASafety_WallDetecter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASafety_WallDetecter::UpdateDatas(int32 directIndex, int32 wallNum, bool IsCollision)
{
	CollisionNums[directIndex] = IsCollision ? wallNum : -1;
	SetResult();
}

void ASafety_WallDetecter::SetResult()
{
	uint8 index = 0;

	while (index < 8) {
		CollisionCheck[index] = CollisionNums[index] >= 0;
		index++;
	}

	index = 0;

	//Cardinal Directions
	while (index < 4) {
		uint8 prevIndex = index > 0 ? index + 3 : 7;
		uint8 nextIndex = index + 4;
		uint8 cur = CollisionNums[index];
		uint8 prev = CollisionNums[prevIndex];
		uint8 next = CollisionNums[nextIndex];

		if (prev == next) {
			CollisionCheck[prevIndex] = false;
			CollisionCheck[nextIndex] = false;
		}

		index++;
	}

	//Diagonal Directions
	//while (index < 8) {



	//	index++;
	//}

	index = 0;
	while (index < 4) {
		uint8 prevIndex = index > 0 ? index + 3 : 7;
		uint8 nextIndex = index + 4;
		CollisionResults[index] = CollisionCheck[index] || CollisionCheck[prevIndex] || CollisionCheck[nextIndex];

		index++;
	}

}

