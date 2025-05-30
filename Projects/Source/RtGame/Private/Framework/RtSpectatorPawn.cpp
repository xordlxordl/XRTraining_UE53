// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtSpectatorPawn.h"
#include "Components/SphereComponent.h"

ARtSpectatorPawn::ARtSpectatorPawn()
{
	GetCollisionComponent()->SetCollisionProfileName(FName("NoCollision"));
}
