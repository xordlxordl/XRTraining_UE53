// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/RtWidgetActor.h"
#include "RtWidgetActor_Menu.generated.h"

/**
 * 
 */

class USphereComponent;
class ARtWidgetActor_MenuOption;

DECLARE_DELEGATE_OneParam(FAddHoverOption, ARtWidgetActor_MenuOption*);
DECLARE_DELEGATE_OneParam(FRemoveHoverOption, ARtWidgetActor_MenuOption*);

UCLASS()
class RTGAME_API ARtWidgetActor_MenuOption : public AActor
{
	GENERATED_BODY()

public:
	ARtWidgetActor_MenuOption();

	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> DefaultRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* SphereCollision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UWidgetComponent> WidgetComponent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EventName = TEXT("");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EventID = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DefaultFontSize = 16.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultSize = 0.2f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HoverSize = 0.35f;

public:
	void Hover(float NewPercent);
	void Unhover();
	void SetPercent(float NewPercent);

	virtual void Execute();
	virtual void PostExecute(class ARtWidgetActor_Menu* Menu);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FAddHoverOption AddHoverOption;
	FRemoveHoverOption RemoveHoverOption;

};

UCLASS()
class RTGAME_API ARtWidgetActor_Menu : public AActor
{
	GENERATED_BODY()
	
public:
	ARtWidgetActor_Menu();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> DefaultRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> RelativeRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin="2", ClampMax="12"))
	int32 OptionsNum = 12;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SphereRadius = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxOptionOverlapTime = 1.0f;

	UPROPERTY(VisibleAnywhere)
	float OptionOverlapTime = 0.0f;

	TArray<ARtWidgetActor_MenuOption*> MenuOptionArray;
	float Theta = 0.0f;
	float Radius = 15.0f;

	const float MenuOptionMaintainDistance = 200.0f;
	const float RadioOptionMaintainDistance = 400.0f;

public:
	void Clear();
	void Init(int32 NewOptionsNum);
	void CreateMenuOption(int32 Index, FString EventName, FString EventID);
	void SetMenuOption(ARtWidgetActor_MenuOption* MenuOption, int32 Index, FString EventName, FString EventID, FSlateColor Color = FColor::White);
	void SetRadiusPosition(ARtWidgetActor_MenuOption* MenuOption, int32 Index);
	void SetTarget(AActor* target, FVector worldLocation);
	void UpdateBillboardRotation();

	/*virtual*/ void ShowOptions();
	/*virtual*/ void HideOptions();

	TWeakObjectPtr<ARtWidgetActor_MenuOption> HoverOption;

	TWeakObjectPtr<class ARtGuideHand> GuideHand;

};
