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
#include "UI/Control/Rt_Assign_Trainee_Block.h"
#include "Rt_Assign_Trainee_Role_Popup.generated.h"

class URt_Assign_Trainee_Role_List;
class URt_Assign_Trainee;

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Assign_Trainee_Role_Popup : public URt_PageBase
{
	GENERATED_BODY()
	
	// Selected Block for Open Popup
public:
	URt_Assign_Trainee_Block* OpenerBlock;


	UPROPERTY(meta = (BindWidget)) URt_Assign_Trainee_Role_List* Role_List_Var;
	UPROPERTY(meta = (BindWidget)) UBorder* TapToCloseZone_var;


	virtual void Close_Page() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	UFUNCTION()	FEventReply OnClicked_TapToCloseZone(FGeometry Geometry, const FPointerEvent& MouseEvent);

public:

	UFUNCTION(BlueprintCallable) // for 20
		void Set_Widget_Assign_Trainee(URt_Assign_Trainee* InWidget);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<URt_Assign_Trainee> Widget_AssignTrainee;

};
