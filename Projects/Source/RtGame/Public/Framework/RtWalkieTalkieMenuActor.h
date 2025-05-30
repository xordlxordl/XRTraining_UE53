// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Animation/WidgetAnimation.h"
#include "Animation/UMGSequencePlayer.h"
#include "Components/WidgetComponent.h"

#include "RtWalkieTalkieMenuActor.generated.h"

UCLASS()
class RTGAME_API ARtWalkieTalkieMenuActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARtWalkieTalkieMenuActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateEventIDList();

	void ChangeSelectedMenuText();

	bool GetIsAnimationPlaying();

	bool GetIsPopupMenuOpened();
	void SetIsPopupMenuOpened(bool InIsPopupMenuOpened);

	UWidgetAnimation* FindWidgetAnimationByName(const FString& AnimName);

	void PressedOK();
	void PressedBack();

	void PressedTransmitting();
	void ReleasedTransmitting();

	void OpenPopupMenu();
	void ClosePopupMenu();

	void MoveMenuUp();
	void MoveMenuDown();

	int WrapIndex(int baseIdx, int offset, int max);

	UFUNCTION()
	void ChangeMenuTexts();

	UFUNCTION()
	void OnAnimationFinished();

	// À§Á¬ ÄÄÆ÷³ÍÆ®
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
	UWidgetComponent* WidgetComponent;

	// À§Á¬ Å¬·¡½º (UMG)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UUserWidget> WidgetClass;

	// Animations
	UWidgetAnimation* OpenPopupMenuAnim;
	UWidgetAnimation* ClosePopupMenuAnim;
	UWidgetAnimation* MenuUpAnim;
	UWidgetAnimation* MenuDownAnim;
	UWidgetAnimation* TransmittingAnim;
	UWidgetAnimation* TransmitEndAnim;

private:
	UUMGSequencePlayer* AnimPlayer = nullptr;

	int PrevSelectedMenuIdx = -1;
	int CurSelectedMenuIdx = -1;
	int EventIDNum = -1;

	bool IsPopupMenuOpened = false;
	bool IsAnimationPlaying = false;

	TArray<FString> EventIDList;
};
