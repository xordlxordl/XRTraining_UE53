// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RtWidgetActor.generated.h"

UCLASS()
class RTGAME_API ARtWidgetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARtWidgetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> DefaultRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> RelativeRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UWidgetComponent> WidgetComponent;

	UFUNCTION(BlueprintCallable)
	UUserWidget* Init(const FString widgetPath);

	UFUNCTION(BlueprintCallable)
	void SetWidgetSize(const float widgetSizeX = 1000.0f, const float widgetSizeY = 1000.0f);

	UFUNCTION(BlueprintCallable)
	void SetPivot(const float pivotX = 0.5f, const float pivotY = 0.5f);

	UFUNCTION(BlueprintCallable)
	void SetWidgetRelativeLocation(const float locX = 0.0f, const float locY = 0.0f);

	UFUNCTION(BlueprintCallable)
	void SetWidgetScale(const float widgetScale = 1.0f);

	UFUNCTION(BlueprintCallable)
	void SetTarget(AActor* followTarget = nullptr, FVector worldLocation = FVector::ZeroVector);

	void DestroyWidgetActor();
	void SetLength(float length);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<AActor> FollowTarget = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsFixScale = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WidgetScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Length = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScaleRate = 0.1f;

public:
	UFUNCTION(BlueprintCallable)
	static ARtWidgetActor* CreateRtWidgetActor();
	
	UPROPERTY()
	FTimerHandle TimerHandle;
};
