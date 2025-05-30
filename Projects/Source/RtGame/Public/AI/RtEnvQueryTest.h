// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "RtEnvQueryTest.generated.h"

/**
 * 
 */

class ARtGameSceneManager;

UCLASS()
class RTGAME_API URtEQT_IsFreeMoveArea : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	URtEQT_IsFreeMoveArea();

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

protected:
	bool IsInFreeMoveArea(FVector point) const;

};
