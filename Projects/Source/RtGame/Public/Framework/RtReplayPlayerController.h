// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Framework/RtPlayerController.h"
#include "RtReplayPlayerController.generated.h"

class URt_PrimaryLayout;
class ARtTraineePawn;
class URtReplayManager;

/**
 * 
 */
UCLASS()
class RTGAME_API ARtReplayPlayerController : public ARtPlayerController
{
	GENERATED_BODY()

private:
	TWeakObjectPtr<URt_PrimaryLayout> Primary_Layout;


	
};
