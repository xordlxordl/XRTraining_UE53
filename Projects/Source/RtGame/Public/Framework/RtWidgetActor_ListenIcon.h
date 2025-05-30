// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RtWidgetActor_ListenIcon.generated.h"

UCLASS()
class RTGAME_API ARtWidgetActor_ListenIcon : public AActor
{
	GENERATED_BODY()

	public:
	// Sets default values for this actor's properties
	ARtWidgetActor_ListenIcon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	void InitListenIcon();

	FString GetTargetMenuName() const;
	void SetTargetMenuName(const FString& InMenuName);

	const TArray<FString>& GetVoiceCommandList() const;
	void SetVoiceCommandList(const TArray<FString>& NewList);

	bool GetIsVisible();
	void SetIsVisible(bool InIsVisible);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* WidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString TargetMenuName;

	UWidgetComponent* GetWidgetComponent();

	void UpdateBillboardRotation();

private:
	bool IsVisible = false;
	TArray<FString> VoiceCommandList;
};
