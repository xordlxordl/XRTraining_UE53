// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_PageBase.h"

// UE Component for Widget
#include "Components/ListView.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
// Data
#include "Common/RtRpcCommon.h"

#include "Rt_Place_Trainee.generated.h"

class URt_Place_Trainee_List;

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Place_Trainee : public URt_PageBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget)) UTextBlock* PlaceTitle_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* PlaceTitleToolTip_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Trainee_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* backstep_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* SelectScenario_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Complete_text;

	UPROPERTY(meta = (BindWidget))	URt_Place_Trainee_List* Trainee_List_var;
 	UButton* PlaceEnd_Btn;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	void Update_DeviceInfo_Cashed();
	void Update_DeviceInfo(APlayerController* InPlayerController, const TArray<FRtDeviceInfo>& InDeviceArray);
	void Update_PlayerReadiness(APlayerController* InPlayerController, const TArray<FRtDeviceInfo>& InInfo);
	void Client_Confirm_TraineeAassignment_Delegate(APlayerController* InPlayerController, int32 InResult);
private:
	void Remove_PlayerState(APlayerState* InPlayerStat);
	void Add_PlayerState(APlayerState* InPlayerStat);

public:
	UFUNCTION()
	void TraineeList_ItemClicked(UObject* InObject);

	bool Change_DeviceInfo(const FRtTraineeInfo& InInfo);
	bool Change_DeviceInfo(const FRtRoleInfo& InInfo);

	UFUNCTION()
	void Updated_PlayerState(ARtPlayerState* InPlayerState);

	FRtScenarioInfo Senario_Info;

	void Update_ScenarioInfo();



	UFUNCTION(BlueprintImplementableEvent)  void BREvent_Changed_DeviceInfo(const FRtDeviceInfo InInfo);
	UFUNCTION(BlueprintImplementableEvent)	void BREvent_Changed_ScenarioInfo(const FRtScenarioInfo InInfo);

protected:
	UFUNCTION(BlueprintCallable)	bool BPCall_Confirm_TraineeAssignment();
	UFUNCTION(BlueprintCallable)	bool BPCall_Back();
	UFUNCTION(BlueprintCallable)	bool BPCall_Quit();

	UFUNCTION(BlueprintImplementableEvent)  void BPEvent_Confirm_TraineeAssignment();



};
