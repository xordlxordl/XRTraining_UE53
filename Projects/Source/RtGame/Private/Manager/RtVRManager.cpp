// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/RtVRManager.h"

#include "HeadMountedDisplayFunctionLibrary.h"


DEFINE_LOG_CATEGORY(RtVRManager);

// Sets default values
ARtVRManager::ARtVRManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARtVRManager::BeginPlay()
{
	Super::BeginPlay();
	return;
	
	if(UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				PassthroughHandle = UViveOpenXRPassthroughFunctionLibrary::CreatePassthroughUnderlay(EXrPassthroughLayerForm::Planar);

				if (PassthroughHandle.Valid)
				{
					UE_LOG(RtVRManager, Log, TEXT("Create Passthrough Underlay : True"));
					GetWorldTimerManager().ClearTimer(TimerHandle);
				}
				else
				{
					UE_LOG(RtVRManager, Log, TEXT("Create Passthrough Underlay : False"));
				}
			}), 5.f, true, -2.f);
	}
}

void ARtVRManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	return;
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		bool ResultOfDestroyPassthroughUnderlay = UViveOpenXRPassthroughFunctionLibrary::DestroyPassthroughUnderlay(PassthroughHandle);
		UE_LOG(RtVRManager, Log, TEXT("Destroy Passthrough Underlay : %s"), ResultOfDestroyPassthroughUnderlay ? TEXT("True") : TEXT("False"));
	}
}

// Called every frame
void ARtVRManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

