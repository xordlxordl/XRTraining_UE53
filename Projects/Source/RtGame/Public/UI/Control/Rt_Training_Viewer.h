// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Rt_PageBase.h"
// UE Widget
#include "Components/Sizebox.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
// Data
#include "Common/RtRpcCommon.h"
#include "Common/RtWebCommon.h"
#include "RtCore/Public/Common/RtCommon.h"

#include "Rt_Training_Viewer.generated.h"

class ARtPlayerState;
class URt_Training_Viewer_TraineeList;

UENUM(BlueprintType)
enum class ERt_TrainingView_State : uint8
{
	Ready,
	Proceeding,
	Suspended
};

UENUM(BlueprintType)
enum class ENameDisplayMode : uint8
{
	RoleAndName   UMETA(DisplayName = "Role + Name"),
	NameOnly      UMETA(DisplayName = "Name"),
	NameAndSN     UMETA(DisplayName = "Name + SN"),
	Max UMETA(Hidden)
};

/**
 * 
 */
UCLASS()
class RTGAME_API URt_Training_Viewer : public URt_PageBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))	USizeBox* SizeBox_Screen_Var;
	UPROPERTY(meta = (BindWidget))	URt_Training_Viewer_TraineeList* TraineeList;
	UPROPERTY(meta = (BindWidget))  UButton* TrainingStart_Btn;
	UPROPERTY(meta = (BindWidget))  UButton* Accident_Btn;
	UPROPERTY(meta = (BindWidget))  UButton* TrainingQuit_Btn;
	UPROPERTY(meta = (BindWidget))  UButton* TrainingRestart_Btn;

	UPROPERTY(meta = (BindWidget))  UButton* BackStepBtn_var;
	UPROPERTY(meta = (BindWidget))  UButton* SelectBtn_var;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)  UComboBoxString* Sunset_Combo;



	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UImage* Image_Blind_var;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UImage* Image_56;

	// Waiting
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UTextBlock* WaitingTitle_var;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UTextBlock* WaitingTitleToolTip_var;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UTextBlock* Backstep_text;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UTextBlock* SelectScenario_text;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UTextBlock* StartTraining_text;

	// Progress
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UTextBlock* ProgressTitle_var;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UTextBlock* ProgressTitleToolTip_var;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UTextBlock* Contingency_text;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UTextBlock* Quit_text;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UTextBlock* Restart_text;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UTextBlock* RunningTime_text;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))	UTextBlock* TB_TimeLimit_var;

	bool bUseRenderTarget;
	ERt_TrainingView_State UI_State = ERt_TrainingView_State::Ready;

private:
	//dave
	TArray<FRtDeviceInfo> newInfos;
	FTimerHandle ResizeTimerHandler;
	FTimerHandle TimerHandle;
	int CurrentTime = 0;
	TEnumAsByte<ENameDisplayMode> CurrentNameInfoIndex;
	bool IsStartTraining = false;
	bool bConfirm_TraineeDeployment = false;

private:
	bool bIsDragging = false;
	FVector2D PreviousMousePosition;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);

public:

	// Resize
	void OnViewportResized(FViewport* Viewport, uint32 Unused);
	// Timer
	void ResizeControlModeCamera();
protected:
	void RepeatingFunction();
	void Update_SenarioInfo();

	// Start Training Flow
	UFUNCTION() void OnClicked_TrainingStart();
	UFUNCTION() void OnClicked_AddAccident();
	UFUNCTION() void OnClicked_TrainingQuit();
	UFUNCTION() void OnClicked_TrainingRestart();
	UFUNCTION() void OnClicked_BackStep();
	UFUNCTION() void OnClicked_SelectScenario();

	UFUNCTION() void SendPacketToScenarioServerForStart(int32 TID);
	UFUNCTION()	void StartTrainingScenario(int32 TID);
//	UFUNCTION() void SendPacketToScenarioServerForEnd(int32 TID);
	UFUNCTION() void EndTrainingScenario();
	UFUNCTION() void BindRenderTarget(UTextureRenderTarget2D* RenderTarget);

public:
	// BP Event

	UFUNCTION(BlueprintCallable)    bool BPCall_Quit();
	UFUNCTION(BlueprintCallable)    bool BPCall_Pause_Training(); // 기획은 없지만 뭐 필요하겠지
	UFUNCTION(BlueprintCallable)    bool BPCall_ReStart_Training();
	UFUNCTION(BlueprintCallable)    void BPCall_Add_Accident();
	UFUNCTION(BlueprintCallable)    void BPCall_SetSunset(bool input);
	UFUNCTION(BlueprintCallable)    void BPCall_OnItemSelectionCharged(UObject* InObject);

	UFUNCTION(BlueprintImplementableEvent)	void BPEvent_FadeOut_ViewBlindImage();
	UFUNCTION(BlueprintImplementableEvent)	void BPEvent_Changed_ScenarioInfo(const FRtScenarioInfo InInfo);
	UFUNCTION(BlueprintNativeEvent) 	void BPNEvent_Update_UI_State(ERt_TrainingView_State InState);
	UFUNCTION() void BPEvent_Update_UI_State(ERt_TrainingView_State InState);

	// Delegate
	void Client_Confirm_TraineeDeployment_Delegate(APlayerController* InPlayerController, int32 InResult);
	void Client_Ready_For_Training_Delegate(int32 UID, bool IsReady);
protected:
	void Update_DeviceInfo_Cached();
	void Update_DeviceInfo(APlayerController* InPlayerController, const TArray<FRtDeviceInfo>& InDeviceArray);

private:
	// dave
	UFUNCTION()
	void HandleDeviceInfoChanged(int32 PlayerId, const FRtDeviceInfo& Info);
	bool VerifyReady(int32 playerId, const TArray<FRtDeviceInfo>& deviceInfo);
	bool IsVR(int32 PlayerId);
	void UpdateIcons();
};
