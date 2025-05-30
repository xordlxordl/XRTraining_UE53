// Fill out your copyright notice in the Description page of Project Settings.


#include "Models/RtMapModel.h"

// Sets default values
ARtMapModel::ARtMapModel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARtMapModel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARtMapModel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

