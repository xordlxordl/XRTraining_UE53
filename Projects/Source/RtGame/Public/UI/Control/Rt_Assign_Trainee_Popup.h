// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_PageBase.h"

// UE Component for Widget
#include "Components/ListView.h"
#include "Components/Button.h"
#include "Components/Border.h"
// Data
#include "Common/RtRpcCommon.h"

// UI
#include "UI/Control/Rt_Assign_Trainee_Block.h"
#include "UI/Control/Rt_Assign_Trainee_AS_List.h"
#include "UI/Control/Rt_Assign_Trainee.h"

#include "Rt_Assign_Trainee_Popup.generated.h"

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Assign_Trainee_Popup : public URt_PageBase
{
	GENERATED_BODY()
	

public:

	// Selected Block for Open Popup
	URt_Assign_Trainee_Block* OpenerBlock;

	UPROPERTY(meta = (BindWidget)) URt_Assign_Trainee_AS_List* Assign_List_Var;
	UPROPERTY(meta = (BindWidget)) UBorder* TapToCloseZone_var;

	virtual void Close_Page() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	UFUNCTION()	FEventReply OnClicked_TapToCloseZone(FGeometry Geometry, const FPointerEvent& MouseEvent);
public:
	void Update_TraineeInfo_Cashed();
	void Update_TraineeInfo(APlayerController* InPlayerController, const TArray<FRtTraineeInfo>& InTraineeArray);


public:
	UFUNCTION(BlueprintCallable)
	void Set_Widget_Assign_Trainee(URt_Assign_Trainee* InWidget);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<URt_Assign_Trainee> Widget_AssignTrainee;



};
