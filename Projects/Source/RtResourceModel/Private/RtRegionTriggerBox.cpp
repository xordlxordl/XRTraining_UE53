// Fill out your copyright notice in the Description page of Project Settings.


#include "RtRegionTriggerBox.h"
#include "RtDefines.h"

//#include "Manager/RsGameManager.h"


ARtRegionTriggerBox::ARtRegionTriggerBox()
{
	OnActorBeginOverlap.AddDynamic(this, &ARtRegionTriggerBox::OnActorBeginOverlaped);
}

void ARtRegionTriggerBox::Init()
{
//	UBlueprint* BlueprintObject = UBlueprint::NewObject(BP_RtRegionDebugBox);
}

void ARtRegionTriggerBox::OnActorBeginOverlaped(AActor* pOverlappedActor, AActor* pOtherActor)
{
	FString overlappedName, otherName;
	//GetName(myName);
	pOverlappedActor->GetName(overlappedName);
	pOtherActor->GetName(otherName);

	//APP_LOG("ARtRegionTriggerBox::OnActorBeginOverlaped overlapped %s, other %s", *overlappedName, *otherName);

//	auto* pGameManager = GetGameInstance()->GetSubsystem<URsGameManager>();
//	if (pGameManager)
//	{
		//pGameManager->RegionEntryEventMessage.Broadcast(overlappedName, otherName);
//	}
}

