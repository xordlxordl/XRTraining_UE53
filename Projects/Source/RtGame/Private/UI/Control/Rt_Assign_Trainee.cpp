// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Assign_Trainee.h"

// ListViewObject <- Need for Data Synchronization between List and Block
#include "UI/Rt_ListViewObject.h"

// Framework
#include "Framework/RtPlayerController.h"
#include "Framework/RtGameState.h"
#include "Framework/RtPlayerState.h"
#include "Framework/RtGameHUD.h"

// Manager
#include "Manager/RtConfig.h"
#include "Manager/RtWebManager.h"
#include "Manager/RtTrainingManager.h"
#include "Manager/RtLoadingManager.h"
// UI
#include "UI/Control/Rt_Assign_Trainee_List.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"

void URt_Assign_Trainee::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UStringTable* ControlStringTable = LoadObject<UStringTable>(nullptr, *URtConfig::ControlMainPath);
	if (ControlStringTable == nullptr) return;

	if (PlaceTitle_text && PlaceTitleToolTip_text && Complete_text && SelectScenario_text && backstep_text) {

		FString OUTPUT;
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("AssignTrainee"), OUTPUT);
		PlaceTitle_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("AssignTrainee_ToolTip"), OUTPUT);
		PlaceTitleToolTip_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("AssignTraineeConfirm"), OUTPUT);
		Complete_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("SelectScenario"), OUTPUT);
		SelectScenario_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("BackStep"), OUTPUT);
		backstep_text->SetText(FText::FromString(OUTPUT));
	}

	// Button Bind
	if (PlaceFin_Btn) {
		PlaceFin_Btn->OnClicked.AddUniqueDynamic(this, &URt_Assign_Trainee::OnClicked_PlaceFin_Btn);
	}
	
	// dave 
	if (BackStepBtn_var) {
		BackStepBtn_var->OnClicked.AddUniqueDynamic(this, &URt_Assign_Trainee::OnClicked_BackStepBtn);
	}

	// dave
	if (SelectBtn_var) {
		SelectBtn_var->OnClicked.AddUniqueDynamic(this, &URt_Assign_Trainee::OnClicked_SelectBtn);
	}

	// When GetBack This Widget 
	if (auto* tmgr = GetTrainingManager()) {
		tmgr->SetControlFlow(EControlFlow::CONTROL_ASSIGNTRAINEE);
	}
	
	if (auto pc = Get_PlayerController()) {
		if (pc->IsLocalController()) {
			if (Trainee_List_var) {
				Trainee_List_var->OwnerWidget = this;
			}

			pc->Update_DeviceList_Delegate.AddUObject(this, &URt_Assign_Trainee::Update_DeviceInfo);
			pc->Client_Confirm_TraineeAassignment_Delegate.AddUObject(this, &URt_Assign_Trainee::Client_Confirm_TraineeAassignment_Delegate);

			ARtGameState::Add_PlayerState.AddUObject(this, &URt_Assign_Trainee::Add_PlayerState);
			ARtGameState::Remove_PlayerState.AddUObject(this, &URt_Assign_Trainee::Remove_PlayerState);

			if (IsControl_PlayType()) {
				ARtPlayerState::Updated_Delegate.AddUObject(this, &URt_Assign_Trainee::Updated_PlayerState);

				Update_DeviceInfo_Cashed();
			}
		}
	}
}

void URt_Assign_Trainee::NativeDestruct()
{
	Super::NativeDestruct();
}

bool URt_Assign_Trainee::Initialize()
{
	return Super::Initialize();
}


void URt_Assign_Trainee::Update_DeviceInfo_Cashed()
{
	if (auto* pc = Get_PlayerController()) {
		auto& device_array = pc->Get_Accessible_PlayerDevices(); // get TArray<FRtDeviceInfo>
		Trainee_List_var->Update_ListView(device_array);
	}
}

void URt_Assign_Trainee::Update_DeviceInfo(APlayerController* InPlayerController, const TArray<FRtDeviceInfo>& InDeviceArray)
{
	// Same Function with Update_DeviceInfo_Cashed, But need for Delegate when update trainingdata 

	if (auto* pc = Get_PlayerController()) {
		auto& device_array = pc->Get_Accessible_PlayerDevices(); // get TArray<FRtDeviceInfo>
		Trainee_List_var->Update_ListView(device_array);
	}
}

void URt_Assign_Trainee::Updated_PlayerState(ARtPlayerState* InPlayerState)
{
	Update_DeviceInfo_Cashed();
}

void URt_Assign_Trainee::OnClicked_PlaceFin_Btn()
{
	if (IsStartTraining) return;

	IsStartTraining = CallConfirmTraineeAssignment();
}

// dave
void URt_Assign_Trainee::OnClicked_BackStepBtn()
{
	BPCall_Open_Page("ControlSelectMap");
}

// dave
void URt_Assign_Trainee::OnClicked_SelectBtn()
{
	BPCall_Open_Page("ControlSelectMap");
}

bool URt_Assign_Trainee::Change_DeviceInfo(const FRtTraineeInfo& InInfo)
{
if (!Trainee_List_var)
		return false;

	FRtDeviceInfo DeviceInfo;
	if (Trainee_List_var->Get_SelectedItem_InUniformGridPanel(DeviceInfo)) {
		if (auto* pc = Get_PlayerController()) {

			if (InInfo.TraineeSN.IsEmpty()) { 
				if (DeviceInfo.bHasTrainee() == false) {
					return false; 
				}
				else {
					pc->Server_AssignTrainee_ToDevice(DeviceInfo.Name, InInfo);
					return true;
				}
			}
			else {
				if (pc->IsAlreadyAssignedTrainee(InInfo)) { 
					UE_LOG(LogTemp, Warning, TEXT("Already Assigned Trainee (SN:%s)"), *InInfo.TraineeSN);
				}
				else {
					pc->Server_AssignTrainee_ToDevice(DeviceInfo.Name, InInfo);
					return true;
				}
			}
		}
	}

	return false;
}

bool URt_Assign_Trainee::Change_DeviceInfo(const FRtRoleInfo& InInfo)
{
	if (!Trainee_List_var)
		return false;

	FRtDeviceInfo DeviceInfo;
	if (Trainee_List_var->Get_SelectedItem_InUniformGridPanel(DeviceInfo)) {
		if (auto* pc = Get_PlayerController()) {

			if (InInfo.TraineeRole.IsEmpty()) { 
				if (DeviceInfo.bHasTrainee() == false) {
					return false; 
				}
				else {

					pc->Server_AssignRole_ToDevice(DeviceInfo.Name, InInfo);
					return true;
				}
			}
			else {
				if (pc->IsAlreadyAssignedRole(InInfo)) { 
					UE_LOG(LogTemp, Warning, TEXT("Already Assigned Role (SN:%s)"), *InInfo.TraineeRole);
				}
				else {
					pc->Server_AssignRole_ToDevice(DeviceInfo.Name, InInfo);
					return true;
				}
			}
		}
	}
	return false;
}

void URt_Assign_Trainee::Client_Confirm_TraineeAassignment_Delegate(APlayerController* InPlayerController, int32 InResult)
{
	if (ARtPlayerController* pc = Get_PlayerController()) {
		pc->Open_Page("TrainingView");
	}

	if (auto* world = GetWorld()) {
		UGameplayStatics::SetEnableWorldRendering(world, true);
	}
}

void URt_Assign_Trainee::Remove_PlayerState(APlayerState* InPlayerStat)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, TEXT(" Remove_PlayerState from Rt_Assign_Trainee "));
}

void URt_Assign_Trainee::Add_PlayerState(APlayerState* InPlayerStat)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, TEXT(" Add_PlayerState from Rt_Assign_Trainee "));
}

bool URt_Assign_Trainee::CallConfirmTraineeAssignment()
{
	if (auto* pc = Get_PlayerController()) {
		if (auto* gs = Get_GameState()) {
			auto& info = gs->Get_Scenario_Info();
			int32 number_of_activedevice = Trainee_List_var->Get_NumberOfActiveDevice();
			// 인원 수 맞아야 실행인데 
			if (number_of_activedevice == 0)
			{
				// 훈련 진행 불가
				if (auto* hud = Get_HUD()) {
					FRtDele_MessagResult ResultCallback = FRtDele_MessagResult::CreateLambda([this](ERt_MessageResult ConfirmationResult) {
						if (ConfirmationResult == ERt_MessageResult::Confirmed) {
							// Log Out Data Code 
						}
						});
					hud->Show_MessageBox("AssignedTrainee", ResultCallback);
					return false;
				}
			}
			else if (number_of_activedevice == info.PlayerNum)
			{
				pc->Server_Confirm_TraineeAassignment();
				return true;
			}
			else
			{
				if (auto* hud = Get_HUD()) {
					FRtDele_MessagResult ResultCallback = FRtDele_MessagResult::CreateLambda([this](ERt_MessageResult ConfirmationResult) {
						if (ConfirmationResult == ERt_MessageResult::Confirmed) {
							if (auto* pc = Get_PlayerController()) 
							{
								pc->Server_Confirm_TraineeAassignment();
							}
						}
						// dave
						else {
							IsStartTraining = false;
						}
						});
					hud->Show_MessageBox("ConfirmAssign", ResultCallback);
					return true;
				}
			}
		}
	}
	return false;
}
