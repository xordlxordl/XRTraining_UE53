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

#include "Rt_Place_Trainee_Role_Popup.generated.h"

class URt_Place_Trainee_Role_List;
class URt_Place_Trainee;
/**
 * 
 */
UCLASS()
class RTGAME_API URt_Place_Trainee_Role_Popup : public URt_PageBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget)) URt_Place_Trainee_Role_List* Role_List_Var;
	UPROPERTY(meta = (BindWidget)) UBorder* TapToCloseZone_var;


	virtual void Close_Page() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	UFUNCTION()	FEventReply OnClicked_TapToCloseZone(FGeometry Geometry, const FPointerEvent& MouseEvent);

public:
	UFUNCTION(BlueprintCallable) // for 10
	void Set_Widget_Place_Trainee(URt_Place_Trainee* InWidget);


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<URt_Place_Trainee> Widget_PlaceTrainee;

};
