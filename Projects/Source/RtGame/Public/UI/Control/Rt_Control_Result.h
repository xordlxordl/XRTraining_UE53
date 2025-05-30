// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_PageBase.h"
// UE
#include "Components/Button.h"
#include "Components/TextBlock.h"
// Data
#include "Common/RtRpcCommon.h"
#include "Common/RtWebCommon.h"
#include "RtCore/Public/Common/RtCommon.h"
// UI
#include "UI/Control/Rt_Result_DetailBox.h"
#include "UI/Control/Rt_Control_Result_List.h"
#include "Rt_Control_Result.generated.h"

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Control_Result : public URt_PageBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget)) UTextBlock* ResultTitle_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* ResultTitleToolTip_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TrainingList_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Logout_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* SelectScenario_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* StartNew_text;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Restart_text;


	UPROPERTY(meta = (BindWidget))	UButton* Select_Btn_var;
	UPROPERTY(meta = (BindWidget))	UButton* Logout_Btn_var;
	UPROPERTY(meta = (BindWidget))	UButton* NewScenario_Btn_var;
	UPROPERTY(meta = (BindWidget))	UButton* SameScenario_Btn_var;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)	URt_Result_DetailBox* ResultDetail;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)  URt_Control_Result_List* TraineeList;
	TWeakObjectPtr<class URt_ListViewObejct_DeviceInfo> Clicked_ListObject;

	UFUNCTION() void OnClicked_Select();
	UFUNCTION() void OnClicked_Logout();
	UFUNCTION() void OnClicked_NewScenario();
	UFUNCTION() void OnClicked_SameScenario();
	UFUNCTION() void OnClicked_TraineeListItem(UObject* InObject);

	UFUNCTION() void Update_DetialBox(const FResponse_TrngSimpleResult& inresult );

protected:
	virtual void NativeOnInitialized() override;

	void Update_DeviceInfo_Cached();
	void Update_DeviceInfo(APlayerController* InPlayerController, const TArray<FRtDeviceInfo>& InDeviceArray);
};
