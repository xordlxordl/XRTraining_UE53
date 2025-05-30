// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Rt_Replay_U_Block.generated.h"

class URt_ReplayViewer;
class URtReplayManager;
/**
 * 
 */
UCLASS()
class RTGAME_API URt_Replay_U_Block : public URt_UserWidget
{
	GENERATED_BODY()
private:
	URt_ReplayViewer* ParentsWidget;
	URtReplayManager* RepManager;
	

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UButton* Trainee_Btn;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))   UTextBlock* TraineeName;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))   UTextBlock* TraineeRole;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))   UBorder* Border_Selected;


	UFUNCTION(BlueprintCallable) void SetTraineeInfo(FString InName, FString InRole, int32 InUID);
	UFUNCTION(BlueprintCallable) void SetParentsWidget(URt_ReplayViewer* InParentsWidget);

	// for Camera
	int32 myUID;

	// Get
	URtReplayManager* Get_RepManager() { return RepManager; }

	void SetSelected(bool selected);

protected:
	virtual void NativeOnInitialized() override;
	
public:
	UFUNCTION(BlueprintCallable) void OnClicked_Btn();
};
