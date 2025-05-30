// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/Control/Rt_Training_Viewer.h"
// UE 
#include "Engine/TextureRenderTarget2D.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBoxSlot.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "TimerManager.h"
// Framework 
#include "Framework/RtPlayerController.h"
#include "Framework/RtGameHUD.h"
#include "Framework/RtGameState.h"
#include "Framework/RtGameMode.h"
#include "Framework/RtPlayerState.h"

// UI
#include "UI/Control/Rt_Training_Viewer_TraineeList.h"
#include "UI/Rt_ListViewObject.h"
// Manager
#include "Manager/RtConfig.h"
#include "Manager/RtGameManager.h"
#include "Manager/RtWebManager.h"
#include "Manager/RtTrainingManager.h"
// Network
#include "Network/RtNetworkManager.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"
#include "Manager/RtGameSceneManager.h"
#include <Components/SceneCaptureComponent2D.h>

#include "Manager/RtReplayManagerActor.h"
#include "Manager/RtGameScenarioController.h"

void URt_Training_Viewer::NativeConstruct()
{
	Super::NativeConstruct();
	FViewport::ViewportResizedEvent.AddUObject(this, &URt_Training_Viewer::OnViewportResized);
	bUseRenderTarget = true; // using render target 

	if (auto* gs = Get_GameState())
	{
		gs->Bind();
	}
}

void URt_Training_Viewer::NativeDestruct()
{
	// clear timer
	if (auto* world = GetWorld()) {
		if (TimerHandle.IsValid())
			world->GetTimerManager().ClearTimer(TimerHandle);
	}
	FViewport::ViewportResizedEvent.RemoveAll(this);

	// dave
	if (IsLocalPlayer()) 
	{
		if (auto pc = Get_PlayerController()) 
		{
			pc->Client_Ready_Delegate.RemoveAll(this);
			if (IsControl_PlayType()) 
			{
				ARtGameState::OnDeviceInfoChanged.RemoveAll(this);
			}
		}
	}

	if (auto* gs = Get_GameState()) 
	{
		gs->UnBind();
	}
	
	Super::NativeDestruct();
}

void URt_Training_Viewer::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UStringTable* ControlStringTable = LoadObject<UStringTable>(nullptr, *URtConfig::ControlMainPath);
	if (ControlStringTable == nullptr) return;	
	FString OUTPUT;
	if (RunningTime_text)
	{
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("RunningTime"), OUTPUT);
		RunningTime_text->SetText(FText::FromString(OUTPUT));
	}

	if (WaitingTitle_var && WaitingTitleToolTip_var && Backstep_text && SelectScenario_text && StartTraining_text)
	{
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("WaitTraining"), OUTPUT);
		WaitingTitle_var->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("WaitTraining_ToolTip"), OUTPUT);
		WaitingTitleToolTip_var->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("BackStep"), OUTPUT);
		Backstep_text->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("SelectScenario"), OUTPUT);
		SelectScenario_text->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("TrainingStart"), OUTPUT);
		StartTraining_text->SetText(FText::FromString(OUTPUT));
	}

	if (ProgressTitle_var && ProgressTitleToolTip_var && Contingency_text && Quit_text && Restart_text)
	{
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("RunTraining"), OUTPUT);
		ProgressTitle_var->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("RunTraining_ToolTip"), OUTPUT);
		ProgressTitleToolTip_var->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("AddAccident"), OUTPUT);
		Contingency_text->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Quit"), OUTPUT);
		Quit_text->SetText(FText::FromString(OUTPUT));
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("RestartTraining"), OUTPUT);
		Restart_text->SetText(FText::FromString(OUTPUT));
	}


	if (IsLocalPlayer()) {
		if (auto pc = Get_PlayerController()) {
		
			GetWorld()->GetTimerManager().ClearTimer(ResizeTimerHandler);
			GetWorld()->GetTimerManager().SetTimer(ResizeTimerHandler, this, &URt_Training_Viewer::ResizeControlModeCamera, 1.f, false, 0.1f);

			pc->Update_DeviceList_Delegate.AddUObject(this, &ThisClass::Update_DeviceInfo);
			pc->Client_Confirm_TraineeDeployment_Delegate.AddUObject(this, &URt_Training_Viewer::Client_Confirm_TraineeDeployment_Delegate);
			pc->Client_Ready_Delegate.AddUObject(this, &URt_Training_Viewer::Client_Ready_For_Training_Delegate);
			
			if (IsControl_PlayType()) {
				//dave
				ARtGameState::OnDeviceInfoChanged.AddUObject(this, &URt_Training_Viewer::HandleDeviceInfoChanged);
				
				newInfos.Empty();
				Update_DeviceInfo_Cached();

				//BindRenderTarget(pc->GetRT());
			}
		}
	}

	BPNEvent_Update_UI_State(ERt_TrainingView_State::Ready);

	// set tiemer
	if (auto* world = GetWorld()) {
		world->GetTimerManager().SetTimer(TimerHandle, this, &URt_Training_Viewer::RepeatingFunction, 1.0f, true);
	}

	if (TrainingStart_Btn) {
		TrainingStart_Btn->OnClicked.AddDynamic(this, &URt_Training_Viewer::OnClicked_TrainingStart);
	}
	if (Accident_Btn) {
		Accident_Btn->OnClicked.AddDynamic(this, &URt_Training_Viewer::OnClicked_AddAccident);
	}
	if (TrainingQuit_Btn) {
		TrainingQuit_Btn->OnClicked.AddDynamic(this, &URt_Training_Viewer::OnClicked_TrainingQuit);
	}
	if (TrainingRestart_Btn) {
		TrainingRestart_Btn->OnClicked.AddDynamic(this, &URt_Training_Viewer::OnClicked_TrainingRestart);
	}

	// dave
	if (BackStepBtn_var) {
		BackStepBtn_var->OnClicked.AddDynamic(this, &URt_Training_Viewer::OnClicked_BackStep);
	}
	if (SelectBtn_var) {
		SelectBtn_var->OnClicked.AddDynamic(this, &URt_Training_Viewer::OnClicked_SelectScenario);
	}
	TrainingStart_Btn->SetIsEnabled(false);
}

// dave - Check device flag for ready
void URt_Training_Viewer::HandleDeviceInfoChanged(int32 PlayerId, const FRtDeviceInfo& Info)
{
	// Check User Block 
	
	if (auto* pc = Get_PlayerController()) {
		auto& device_array = pc->Get_Accessible_PlayerDevices();

		//TArray<FRtDeviceInfo> newInfos;


		// dave
		FRtDeviceInfo* myId = newInfos.FindByPredicate([PlayerId](const FRtDeviceInfo& info) { return info.PlayerId == PlayerId; });
		if (myId == nullptr)
		{
			for (auto beforeDevice : device_array)
			{
				if (beforeDevice.PlayerId == PlayerId)
				{
					//beforeDevice.Ready = IsReady;
					beforeDevice.Ready[0] = Info.CompleteMapLoad;
					beforeDevice.Ready[1] = Info.InStartingPoint;
					beforeDevice.Ready[2] = Info.CompleteCalibration;
					newInfos.Add(beforeDevice);
				}
				else
				{
					newInfos.Add(beforeDevice);
				}
			}
		}
		else
		{
			//myId->Ready = IsReady;
			myId->Ready[0] = Info.CompleteMapLoad;
			myId->Ready[1] = Info.InStartingPoint;
			myId->Ready[2] = Info.CompleteCalibration;
		}

		if (TraineeList) {
			TraineeList->Update_ReadyState(newInfos);
		}

		// dave
		if (VerifyReady(PlayerId, newInfos))
		{
			UE_LOG(LogTemp, Error, TEXT("ALL Trainees are ready !!"));

			// 
			TrainingStart_Btn->SetIsEnabled(true);
		}
	}
}

bool URt_Training_Viewer::IsVR(int32 PlayerId) 
{
	if (auto* gs = Get_GameState())
	{
		return gs->Find_AssignedDeviceById(PlayerId)->IsVR;
	}
	return false;
}

// dave - test binding
void URt_Training_Viewer::BindRenderTarget(UTextureRenderTarget2D* TargetRT)
{
	FString RenderTargetPath = TEXT("/Game/RS/Control/Camera/TopView_Control_Mat.TopView_Control_Mat");
	UMaterialInterface* LoadedRenderTarget = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *RenderTargetPath));
	if (LoadedRenderTarget)
	{
		UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(LoadedRenderTarget, this);
		UTexture* RenderTarget = TargetRT;
		if (TargetRT && DynMat)
		{
			DynMat->SetTextureParameterValue("RenderTarget", RenderTarget);
			Image_56->SetBrushFromMaterial(DynMat);
		}
	}
}

void URt_Training_Viewer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (IsStartTraining == false) {
		if (auto* tmgr = GetTrainingManager()) {
			if (tmgr->state_ctrl == EControlFlow::CONTROL_PROCESSINGTRAINING) {
				StartTrainingScenario(tmgr->TrainingIndex);
			}
		}
		
	}
	else {
		if (auto* tmgr = GetTrainingManager()) {
			if (tmgr->state_ctrl == EControlFlow::CONTROL_RESULT) {
				EndTrainingScenario();
			}
		}
	}
}

// dave = handle mouse events
FReply URt_Training_Viewer::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FGeometry ImageGeometry = Image_56->GetCachedGeometry();
	if (ImageGeometry.IsUnderLocation(InMouseEvent.GetScreenSpacePosition()))
	{
		bIsDragging = true;
		PreviousMousePosition = InMouseEvent.GetScreenSpacePosition();

		// Get Slate widget reference
		TSharedPtr<SWidget> SlateWidget = GetCachedWidget();
		if (SlateWidget.IsValid())
		{
			return FReply::Handled().CaptureMouse(SlateWidget.ToSharedRef());
		}
	}
	return FReply::Handled();
}

// dave = handle mouse events
FReply URt_Training_Viewer::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FGeometry ImageGeometry = Image_56->GetCachedGeometry();
	if (bIsDragging && ImageGeometry.IsUnderLocation(InMouseEvent.GetScreenSpacePosition()))
	{
		const FVector2D CurrentPosition = InMouseEvent.GetScreenSpacePosition();
		const FVector2D Delta = CurrentPosition - PreviousMousePosition;
		PreviousMousePosition = CurrentPosition;

		if (ARtPlayerController* MyPC = Cast<ARtPlayerController>(GetOwningPlayer()))
		{
			const float MouseSensitivity = 0.05f;
			FVector2D ScaledDelta = Delta * MouseSensitivity;
			MyPC->MoveCameraByDelta(ScaledDelta);
		}
	}
	return FReply::Handled();
}

// dave = handle mouse events
FReply URt_Training_Viewer::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsDragging = false;
	return FReply::Handled().ReleaseMouseCapture();
}

// dave = handle mouse events
FReply URt_Training_Viewer::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FGeometry ImageGeometry = Image_56->GetCachedGeometry();
	if (ImageGeometry.IsUnderLocation(InMouseEvent.GetScreenSpacePosition()))
	{
		float ScrollDelta = InMouseEvent.GetWheelDelta();
		if (ARtPlayerController* PC = Cast<ARtPlayerController>(GetOwningPlayer()))
		{
			PC->ZoomCamera(ScrollDelta);
		}
	}
	return FReply::Handled();
}


void URt_Training_Viewer::OnViewportResized(FViewport* Viewport, uint32 Unused)
{
	if (Viewport == nullptr) {
		return;
	}
	GetWorld()->GetTimerManager().ClearTimer(ResizeTimerHandler);
	GetWorld()->GetTimerManager().SetTimer(ResizeTimerHandler, this, &URt_Training_Viewer::ResizeControlModeCamera, 1.f, false, 0.1f);
}

void URt_Training_Viewer::ResizeControlModeCamera()
{
	if (SizeBox_Screen_Var) {
		if (auto* pc = Get_PlayerController()) {
			auto geometry = SizeBox_Screen_Var->GetCachedGeometry();
			FVector2D view_size = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
			auto Rate = geometry.Size * geometry.Scale / view_size;
			pc->Update_ControlModeCamera(Rate);

			BPEvent_FadeOut_ViewBlindImage();

			//BPNEvent_Update_UI_State(ERt_TrainingView_State::Ready);
		}
	}
}

void URt_Training_Viewer::RepeatingFunction()
{
	if (auto* world = GetWorld()) {

		if (UI_State == ERt_TrainingView_State::Proceeding) {
			if (TB_TimeLimit_var) {
				if (auto* gs = Get_GameState()) {
					//TB_TimeLimit_var->SetText(FText::FromString(FString::FromInt(gs->RemainTime)));
					if (IsStartTraining)
					{
						CurrentTime++;

						int currentMinute;
						int currentSecond;

						currentMinute = CurrentTime / 60;
						currentSecond = CurrentTime % 60;

				    	FString currentTime;
					    currentTime = FString::Printf(TEXT(" %d : %d "), currentMinute, currentSecond);

					    TB_TimeLimit_var->SetText(FText::FromString(currentTime));
	  		     		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, currentTime);
					}
				}
			}
		}
	}
}

void URt_Training_Viewer::Update_SenarioInfo()
{
	if (auto* gs = Get_GameState()) {
		auto info = gs->Get_Scenario_Info();
		BPEvent_Changed_ScenarioInfo(info);
	}
}

void URt_Training_Viewer::OnClicked_TrainingStart()
{
	// Start with Web
	auto* WebSubsys = GetRtWebManager();
	if (WebSubsys->IsInitialized())
	{
		if (auto* gs = Get_GameState()) {
			auto info = gs->Get_Scenario_Info();
			
			FString InId; 
			InId = info.Destination;
		
			// 1. Control GetTID from Web Server 
			WebSubsys->GetTID(InId);
			WebSubsys->Handle_GetTID.AddUFunction(this, FName("SendPacketToScenarioServerForStart"));
		}
	}

}

void URt_Training_Viewer::OnClicked_AddAccident()
{
	if (ARtPlayerController* pc = Get_PlayerController()) {
		pc->Open_Popup("AccidentPopup");
	}
}

void URt_Training_Viewer::OnClicked_TrainingQuit()
{
	if (auto* hud = Get_HUD()) {
		FRtDele_MessagResult ResultCallback = FRtDele_MessagResult::CreateLambda([this](ERt_MessageResult ConfirmationResult) {
			if (ConfirmationResult == ERt_MessageResult::Confirmed) {
				// Log Out Data Code 
				// 종료 패킷 + 게임종료 코드
				if (ARtGameState* gs = Get_GameState())
				{   
					// 거리 로그를 먼저 보낸다.
					ARtReplayManagerActor::Get(GetWorld())->SendAllPlayerDistanceLog();

					// 웹이 추가 되면 변함
					int32 TID = gs->Training_Index;

					if (ARtGameScenarioController* gsc = ARtGameScenarioController::Get(GetWorld())) {
						gsc->SendPacketToScenarioServerForEnd(TID);
					}

//					SendPacketToScenarioServerForEnd(TID);
				}
			}
			});
		hud->Show_MessageBox("QuitTraining", ResultCallback);
	}
}

void URt_Training_Viewer::OnClicked_TrainingRestart()
{
	if (auto* hud = Get_HUD()) {
		FRtDele_MessagResult ResultCallback = FRtDele_MessagResult::CreateLambda([this](ERt_MessageResult ConfirmationResult) {
			if (ConfirmationResult == ERt_MessageResult::Confirmed) {
				// Log Out Data Code 
				// 재시작 패킷 + 로그데이터 초기화 혹은 임시 저장
				// ResetTimer
				CurrentTime = 0;
				FString currentTime;
				currentTime = FString::Printf(TEXT(" 0 : 0 "));
				TB_TimeLimit_var->SetText(FText::FromString(currentTime));
				BPNEvent_Update_UI_State(ERt_TrainingView_State::Ready);
				BPEvent_Update_UI_State(ERt_TrainingView_State::Ready);
				IsStartTraining = false;
				bConfirm_TraineeDeployment = false;

				// Packet 
				auto* netmgr = GetNetworkManager();
				if (netmgr) {
					netmgr->SendPacket_RestartSame();
				}
			}
			});
		hud->Show_MessageBox("RestartTraining", ResultCallback);
	}
}

void URt_Training_Viewer::SendPacketToScenarioServerForStart(int32 TID)
{
	// 3. Send Packet 'Start' to Socket Server
	auto* netmgr = GetNetworkManager();
	if (netmgr) {
		netmgr->SendPacket_TrainingStart(TID);
	}
}

void URt_Training_Viewer::StartTrainingScenario(int32 TID)
{
	if (bConfirm_TraineeDeployment) return;

	if (auto* pc = Get_PlayerController()) {
		if (auto* gs = pc->Get_GameState())
		{
			gs->Training_Index = TID;
		}
		pc->Server_Confirm_TraineeDeployment();
		bConfirm_TraineeDeployment = true;
	}
}

//void URt_Training_Viewer::SendPacketToScenarioServerForEnd(int32 TID)
//{
//	// Send Packet 'End'
//	auto* netmgr = GetNetworkManager();
//	if (netmgr) {
//		netmgr->SendPacket_TrainingEnd(TID);
//	}
//
//	auto* WebSubsys = GetRtWebManager();
//	if (WebSubsys->IsInitialized())
//	{
//		if (auto* gs = Get_GameState()) {
//			auto info = gs->Get_Scenario_Info();
//
//			FString InId;
//			InId = info.Destination;
//			WebSubsys->CheckTID(gs->Training_Index);
//		}
//	}
//
//}

// dave - start new scenario
void URt_Training_Viewer::OnClicked_SelectScenario()
{
	if (auto* hud = Get_HUD()) {
		FRtDele_MessagResult ResultCallback = FRtDele_MessagResult::CreateLambda([this](ERt_MessageResult ConfirmationResult) {
			if (ConfirmationResult == ERt_MessageResult::Confirmed) {
				auto* netmgr = GetNetworkManager();
				if (netmgr) {
					netmgr->SendPacket_RestartAll();
				}
			}
			});
		hud->Show_MessageBox("ResetTraining", ResultCallback);
	}
}

// dave - stepback
void URt_Training_Viewer::OnClicked_BackStep()
{
	if (auto* hud = Get_HUD()) {
		FRtDele_MessagResult ResultCallback = FRtDele_MessagResult::CreateLambda([this](ERt_MessageResult ConfirmationResult) {
			if (ConfirmationResult == ERt_MessageResult::Confirmed)
			{
				auto* netmgr = GetNetworkManager();
				if (netmgr) {
					netmgr->SendPacket_RestartAll();
				}
			}
			});
		hud->Show_MessageBox("ResetTraining", ResultCallback);
	}
}


void URt_Training_Viewer::EndTrainingScenario()
{
	// 결과창으로 이때 넘어가면 될듯
	Open_Page("Result");
}


bool URt_Training_Viewer::BPCall_Quit()
{
	return false;
}

bool URt_Training_Viewer::BPCall_Pause_Training()
{
	return false;
}

bool URt_Training_Viewer::BPCall_ReStart_Training()
{
	return false;
}

void URt_Training_Viewer::BPCall_Add_Accident()
{
}

void URt_Training_Viewer::BPCall_SetSunset(bool input)
{
	if (auto* nmgr = GetNetworkManager()) {

		input = (Sunset_Combo->GetSelectedIndex() == 0);
		nmgr->SendPacket_DayNight(input);
	}
}

void URt_Training_Viewer::BPCall_OnItemSelectionCharged(UObject* InObject)
{
	if (auto* obj = Cast<URt_ListViewObejct_DeviceInfo>(InObject)) {
		if (auto* pc = Get_PlayerController()) {
			pc->Change_TraineeSelectionView(obj->Info.Name);
		}
	}
}

void URt_Training_Viewer::BPNEvent_Update_UI_State_Implementation(ERt_TrainingView_State InState)
{
	
}

void URt_Training_Viewer::BPEvent_Update_UI_State(ERt_TrainingView_State InState)
{
	switch (InState)
	{
	case ERt_TrainingView_State::Ready:
		break;
	case ERt_TrainingView_State::Proceeding:
		break;
	case ERt_TrainingView_State::Suspended:
		break;
	default:
		break;
	}
	UI_State = InState;
}

void URt_Training_Viewer::Client_Confirm_TraineeDeployment_Delegate(APlayerController* InPlayerController, int32 InResult)
{
	// Listen 과의 Start 통신 완료 후 처리 
	// 여기까지 처리되어 디버그 걸리면 훈련 시작 완료
	BPNEvent_Update_UI_State(ERt_TrainingView_State::Proceeding);
	BPEvent_Update_UI_State(ERt_TrainingView_State::Proceeding);
	IsStartTraining = true;
}

void URt_Training_Viewer::Client_Ready_For_Training_Delegate(int32 UID, bool IsReady)
{
	AGameStateBase* GameState = GetWorld()->GetGameState();
	if (GameState)
	{
		for (APlayerState* PlayerState : GameState->PlayerArray)
		{
			ARtPlayerState* RtPlayerState = Cast<ARtPlayerState>(PlayerState);
			if (RtPlayerState)
			{
				UE_LOG(LogTemp, Log, TEXT("Player Info: %s"), *RtPlayerState->GetPlayerName());
				FRtDeviceInfo info = RtPlayerState->Get_PlayerInfo();
				if (info.StatusInTraining == EStatusInTraining::Default)
					continue;
				HandleDeviceInfoChanged(info.PlayerId, info);
			}
		}
	}
}


// dave - TEST
bool URt_Training_Viewer::VerifyReady(int32 playerId, const TArray<FRtDeviceInfo>& deviceInfo)
{
	int count = 0, ready = 0;

	for (auto& it : deviceInfo) 
	{
		if (it.StatusInTraining != EStatusInTraining::Default)
			count++;
		
		const int32 Loop = IsVR(playerId) ? 3 : 2;			// if VR's not on, skip calibration check
		bool Ready = true;
		for (int i = 0; i < Loop; ++i)
		{
			if (!it.Ready[i]) 
			{
				Ready = false;
				break;
			}
		}

		if (Ready)
			ready++;
	}
	return (ready == count);
}


void URt_Training_Viewer::Update_DeviceInfo_Cached()
{
	if (auto* pc = Get_PlayerController()) {
		auto& device_array = pc->Get_Accessible_PlayerDevices();
		if (TraineeList) {
			TraineeList->Update_ListView(device_array);
		}
	}
}

void URt_Training_Viewer::Update_DeviceInfo(APlayerController* InPlayerController, const TArray<FRtDeviceInfo>& InDeviceArray)
{
	if (auto* pc = Get_PlayerController()) {
		auto& device_array = pc->Get_Accessible_PlayerDevices();
		if (TraineeList) {
			TraineeList->Update_ListView(device_array);
		}
	}
}
