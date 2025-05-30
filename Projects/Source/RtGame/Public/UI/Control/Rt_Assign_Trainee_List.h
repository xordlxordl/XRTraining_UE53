// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_UserWidget.h"
// UE Component
#include "Components/UniformGridPanel.h"

#include "UI/Control/Rt_Assign_Trainee_Block.h"
#include "UI/Control/Rt_Assign_Trainee.h"

#include "Rt_Assign_Trainee_List.generated.h"

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Assign_Trainee_List : public URt_UserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

public: 
	UPROPERTY(meta = (BindWidget)) UUniformGridPanel* uniformGridPanel;

	URt_Assign_Trainee_Block* CurrentSelectedBlock;
	URt_Assign_Trainee* OwnerWidget;

	int32 Get_NumberOfActiveDevice();

	bool Get_SelectedItem_InUniformGridPanel(FRtDeviceInfo& OutInfo);
	
	bool Update_ListView(const TArray<FRtDeviceInfo>& InInfo);

};
