// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RsSimNPC.h"

// Sets default values
ARsSimNPC::ARsSimNPC()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ObjectActorComponent = CreateDefaultSubobject<URtObjectActorComponent>(TEXT("ObjectActorComponent"));

}

// Called when the game starts or when spawned
void ARsSimNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARsSimNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARsSimNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

