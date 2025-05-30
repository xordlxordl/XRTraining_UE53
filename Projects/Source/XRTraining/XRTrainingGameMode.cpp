// Copyright Epic Games, Inc. All Rights Reserved.

#include "XRTrainingGameMode.h"
#include "XRTrainingCharacter.h"
#include "UObject/ConstructorHelpers.h"

AXRTrainingGameMode::AXRTrainingGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
