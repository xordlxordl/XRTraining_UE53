// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RtWidgetActor_TraineeResult.generated.h"

UCLASS()
class RTGAME_API ARtWidgetActor_TraineeResult : public AActor
{
	GENERATED_BODY()
	
public:
    ARtWidgetActor_TraineeResult();

    UFUNCTION() void InitWidget();
    UFUNCTION() void UpdateWidget(const FResponse_TrngSimpleResult& Result);

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere)
    UWidgetComponent* WidgetComponent;
};
