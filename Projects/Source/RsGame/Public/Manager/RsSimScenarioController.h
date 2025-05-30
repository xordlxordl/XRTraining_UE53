// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RtScenarioControllerBase.h"
#include "RsSimScenarioController.generated.h"

/**
 * 
 */

RSGAME_API DECLARE_LOG_CATEGORY_EXTERN(Log_RsSimScenarioController, Log, All);

UCLASS()
class RSGAME_API ARsSimScenarioController : public ARtScenarioControllerBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	bool InitializeScenario();

	UFUNCTION(BlueprintCallable)
	void SetPlayerActor(class ARsSimCharacter* myPlayerActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<class ARsSimCharacter> MyPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MyPlayerNameID; // 내 캐릭터 이름 ID ( 화면상에 표시되는 이름과는 다르다. 월드상의 유니크 ID ).

	virtual void Tick(float DeltaTime) override;

///////////////////////////////////////////////////
// Scenario Events
///////////////////////////////////////////////////
protected:
	static ARtSceneManagerBase* RtSceneManager;

	//Bind Console Event
	void BindRsFunctions();

	//Scenario Events Start
	void RsSetVisible(const FString& targetId, const bool& bVisible);//"NpcStranger_1",true
	void RsMoveToWayPoint(const FString& ownerId, const FString& targetFormat, const int32& startIndex, const int32& endIndex, const float& moveSpeed);//"NpcStranger_1","wp1_%d",1,10,400
	void RsMoveToX(const FString& ownerId, const float& x, const float& y, const float& moveTime);
	void RsWarpTo(const FString& ownerId, const FString& targetId, const float& safeDistance);
	void RsRotateZTo(const FString& ownerId, const FString& targetId);

	//Scenario Events Progress
	void RsMoveToWayPoint_Progress(float DeltaTime);
	void RsMoveToX_Progress(float DeltaTime);
	void RsRotateZTo_Progress(float DeltaTime);

	TMap<AActor*, TPair<TArray<AActor*>, float>> MoveToWayPointMaps; //MoveActor, WayPoints, MoveSpeed
	TMap<AActor*, TPair<FVector, float>> MoveToXMaps; //MoveActor, MoveToLocation, MoveSpeed
	TMap <AActor*, AActor*> RotateZToMaps; //OwnerActor, TargetActor
};
