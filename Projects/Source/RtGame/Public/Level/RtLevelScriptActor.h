// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "RtLevelScriptActor.generated.h"

class ARtTrainingReadyPoint;
struct FRtDeviceInfo;
class URtGameManager;
class URtNetworkManager;

/**
 * 
 */
UCLASS()
class RTGAME_API ARtLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
protected:

	TArray<TWeakObjectPtr<ARtTrainingReadyPoint>> StartingPointArray;
protected:
	virtual void BeginPlay() override;
public:

	URtNetworkManager* NetworkSubsystem;

	virtual void Tick(float DeltaTime) override;
	template< typename T >
	static void Find_AllActors(UWorld* InWorld, TArray<T>& OutActorArray) {
		for (TActorIterator<T> It(InWorld, T::StaticClass()); It; ++It) {
			T* Actor = Cast<T>(*It);
			if (IsValid(Actor))
				OutActorArray.Add(Actor);
		}
	}
	TArray<TWeakObjectPtr<ARtTrainingReadyPoint>>& Get_StartingPointArray();
	ARtTrainingReadyPoint* Get_StartTargetPoint(int32 InIndex);
	void Hide_StartingPoint();


	URtGameManager* GetGameManager();
	void Init_PlayMode();

	// Encoding
	std::string GetBase64Decording(std::string encoded_string);
	bool IsBase64(uint8 c);
};
