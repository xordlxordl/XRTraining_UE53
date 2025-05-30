// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_PageBase.h"

// UE Component for Widget
#include "Components/Button.h"
#include "Components/TextBlock.h"

// Data for DeviceInfo
#include "Common/RtRpcCommon.h"

#include "Rt_Assign_Trainee.generated.h"

class URt_Assign_Trainee_List;

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Assign_Trainee : public URt_PageBase
{
	GENERATED_BODY()
	
protected: 
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual bool Initialize() override;

private:
	bool IsStartTraining;


public:
	UPROPERTY(meta = (BindWidget)) UTextBlock* PlaceTitle_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* PlaceTitleToolTip_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* backstep_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* SelectScenario_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Complete_text;

	UPROPERTY(meta = (BindWidget)) UButton* PlaceFin_Btn;
	UPROPERTY(meta = (BindWidget)) UButton* BackStepBtn_var;
	UPROPERTY(meta = (BindWidget)) UButton* SelectBtn_var;

	UPROPERTY(meta = (BindWidget))	URt_Assign_Trainee_List* Trainee_List_var;


public:
	void Client_Confirm_TraineeAassignment_Delegate(APlayerController* InPlayerController, int32 InResult);

	void Update_DeviceInfo_Cashed();

	void Update_DeviceInfo(APlayerController* InPlayerController, const TArray<FRtDeviceInfo>& InDeviceArray);

	UFUNCTION()
	void Updated_PlayerState(ARtPlayerState* InPlayerState);

	UFUNCTION()
	void OnClicked_PlaceFin_Btn();

	UFUNCTION() 
	void OnClicked_BackStepBtn();
	
	UFUNCTION() 
	void OnClicked_SelectBtn();

	bool Change_DeviceInfo(const FRtTraineeInfo& InInfo);
	bool Change_DeviceInfo(const FRtRoleInfo& InInfo);

private:
	void Remove_PlayerState(APlayerState* InPlayerStat);
	void Add_PlayerState(APlayerState* InPlayerStat);


	bool CallConfirmTraineeAssignment();

};
