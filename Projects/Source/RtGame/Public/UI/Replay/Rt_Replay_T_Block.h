// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Rt_Replay_T_Block.generated.h"

class URt_ReplayViewer;
class URtReplayManager;
/**
 * 
 */
UCLASS()
class RTGAME_API URt_Replay_T_Block : public URt_UserWidget
{
	GENERATED_BODY()
private:
	URt_ReplayViewer* ParentsWidget;
	URtReplayManager* RepManager;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UButton* Training_Btn;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))   UTextBlock* TrainingName;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))   UBorder* Border_Selected;

	// Get
	URtReplayManager* Get_RepManager() { return RepManager; }

	// Set
	UFUNCTION(BlueprintCallable) void SetTrainingName(FString folderName, FString realPath);
	UFUNCTION(BlueprintCallable) void SetParentsWidget(URt_ReplayViewer* InParentsWidget);


	FString RealPath;
	FString MotionFilePath;
	FString VoiceFilePath;

	FString ScenarioName;
	int32 TID;

protected:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION() void OnClicked_Btn();
	UFUNCTION() void CallLoadSCT();

	void SetSelected(bool selected);
};
