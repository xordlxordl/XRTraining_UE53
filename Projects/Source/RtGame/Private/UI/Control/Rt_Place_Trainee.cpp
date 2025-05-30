// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Control/Rt_Place_Trainee.h"

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
#include "UI/Control/Rt_Place_Trainee_List.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"

void URt_Place_Trainee::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UStringTable* ControlStringTable = LoadObject<UStringTable>(nullptr, *URtConfig::ControlMainPath);
	if (ControlStringTable == nullptr) return;
	if (PlaceTitle_text && PlaceTitleToolTip_text && Trainee_text && Complete_text && SelectScenario_text && backstep_text) {

		FString OUTPUT;
		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("AssignTrainee"), OUTPUT);
		PlaceTitle_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("AssignTrainee_ToolTip"), OUTPUT);
		PlaceTitleToolTip_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("Trainee"), OUTPUT);
		Trainee_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("AssignTraineeConfirm"), OUTPUT);
		Complete_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("SelectScenario"), OUTPUT);
		SelectScenario_text->SetText(FText::FromString(OUTPUT));

		ControlStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey("BackStep"), OUTPUT);
		backstep_text->SetText(FText::FromString(OUTPUT));
	}


	// When GetBack This Widget 
	if (auto* tmgr = GetTrainingManager()) {
		tmgr->SetControlFlow(EControlFlow::CONTROL_ASSIGNTRAINEE);
	}

	if (auto pc = Get_PlayerController()) {
		if (pc->IsLocalController()) {
			if (Trainee_List_var) {
				Trainee_List_var->OnItemClicked_Delegate.AddUObject(this, &ThisClass::TraineeList_ItemClicked);
				Trainee_List_var->OwnerWidget = this;
			}

			pc->Update_DeviceList_Delegate.AddUObject(this, &URt_Place_Trainee::Update_DeviceInfo);

			pc->RscDele_Update_PlayerPreparing.AddUObject(this, &URt_Place_Trainee::Update_PlayerReadiness);
			pc->Client_Confirm_TraineeAassignment_Delegate.AddUObject(this, &URt_Place_Trainee::Client_Confirm_TraineeAassignment_Delegate);

			ARtGameState::Add_PlayerState.AddUObject(this, &URt_Place_Trainee::Add_PlayerState);
			ARtGameState::Remove_PlayerState.AddUObject(this, &URt_Place_Trainee::Remove_PlayerState);

			if (IsControl_PlayType()) {
				ARtPlayerState::Updated_Delegate.AddUObject(this, &URt_Place_Trainee::Updated_PlayerState);

				Update_DeviceInfo_Cashed();
			}
		}
	}
	Update_ScenarioInfo();
}

void URt_Place_Trainee::NativeDestruct()
{
	Super::NativeDestruct();
	if (IsControl_PlayType()) {
		ARtPlayerState::Updated_Delegate.RemoveAll(this);
	}
}

void URt_Place_Trainee::Update_DeviceInfo_Cashed()
{
	if (auto* pc = Get_PlayerController()) {
		auto& device_array = pc->Get_Accessible_PlayerDevices();
		Trainee_List_var->Update_ListView(device_array);
	}
}

void URt_Place_Trainee::Update_DeviceInfo(APlayerController* InPlayerController, const TArray<FRtDeviceInfo>& InDeviceArray)
{
	if (auto* pc = Get_PlayerController()) {
		auto& device_array = pc->Get_Accessible_PlayerDevices();
		Trainee_List_var->Update_ListView(device_array);
	}
}

void URt_Place_Trainee::Update_PlayerReadiness(APlayerController* InPlayerController, const TArray<FRtDeviceInfo>& InInfo)
{
	if (auto* pc = Get_PlayerController()) {
		auto& device_array = pc->Get_Accessible_PlayerDevices();
		Trainee_List_var->Update_ListView(device_array);
	}
}

void URt_Place_Trainee::Client_Confirm_TraineeAassignment_Delegate(APlayerController* InPlayerController, int32 InResult)
{


	BPEvent_Confirm_TraineeAssignment();

	if (auto* world = GetWorld()) {
		UGameplayStatics::SetEnableWorldRendering(world, true);
	}
}

void URt_Place_Trainee::Remove_PlayerState(APlayerState* InPlayerStat)
{
}

void URt_Place_Trainee::Add_PlayerState(APlayerState* InPlayerStat)
{
}



void URt_Place_Trainee::TraineeList_ItemClicked(UObject* InObject)
{
	if (InObject) {
		if (auto* pc = Get_PlayerController()) {

		}
	}
}

bool URt_Place_Trainee::Change_DeviceInfo(const FRtTraineeInfo& InInfo)
{
	if (!Trainee_List_var)
		return false;

	//'' empty �����͸� ���� �����ͷ� ����Ѵ�. 
	FRtDeviceInfo DeviceInfo;
	if (Trainee_List_var->Get_SelectedItem_InListView(DeviceInfo)) {
		if (auto* pc = Get_PlayerController()) {

			if (InInfo.TraineeSN.IsEmpty()) { // empty ���� �Ѿ�ð��. ���� �Ҵ�� �Ʒ������� �����.
				if (DeviceInfo.bHasTrainee() == false) {
					return false; // ���氪�� ���� ���� ��� empty �ΰ�� ó������ �ʴ´�.
				}
				else {
					pc->Server_AssignTrainee_ToDevice(DeviceInfo.Name, InInfo);
					return true;
				}
			}
			else {
				if (pc->IsAlreadyAssignedTrainee(InInfo)) { // �̹� ��򰡿� �Ҵ�Ǿ� �ִ� �Ʒ��ڸ� ���Ҵ��� ��� �߻��Ѵ�.
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

bool URt_Place_Trainee::Change_DeviceInfo(const FRtRoleInfo& InInfo)
{
	if (!Trainee_List_var)
		return false;

	//'' empty �����͸� ���� �����ͷ� ����Ѵ�. 
	FRtDeviceInfo DeviceInfo;
	if (Trainee_List_var->Get_SelectedItem_InListView(DeviceInfo)) {
		if (auto* pc = Get_PlayerController()) {

			if (InInfo.TraineeRole.IsEmpty()) { // empty ���� �Ѿ�ð��. ���� �Ҵ�� �Ʒ������� �����.
				if (DeviceInfo.bHasTrainee() == false) {
					return false; // ���氪�� ���� ���� ��� empty �ΰ�� ó������ �ʴ´�.
				}
				else {
					
					pc->Server_AssignRole_ToDevice(DeviceInfo.Name, InInfo);
					return true;
				}
			}
			else {
				if (pc->IsAlreadyAssignedRole(InInfo)) { // �̹� ��򰡿� �Ҵ�Ǿ� �ִ� �Ʒ��ڸ� ���Ҵ��� ��� �߻��Ѵ�.
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

void URt_Place_Trainee::Updated_PlayerState(ARtPlayerState* InPlayerState)
{
	if (auto* pc = Get_PlayerController()) {
		auto& device_array = pc->Get_Accessible_PlayerDevices();
		Trainee_List_var->Update_ListView(device_array);
	}
}

void URt_Place_Trainee::Update_ScenarioInfo()
{
	if (auto* gs = Get_GameState()) {
		auto info = gs->Get_Scenario_Info();
		BREvent_Changed_ScenarioInfo(info);
	}
}

bool URt_Place_Trainee::BPCall_Confirm_TraineeAssignment()
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

					pc->Server_Confirm_TraineeAassignment();
					return true;
				}
			}
			else if (number_of_activedevice == info.PlayerNum)
			{
				//if (auto* gi = GetGameInstance()) {
				//	if (auto* lm = gi->GetSubsystem<URtLoadingManager>()) {
				//		lm->LoadLevelWithLoadingScreen("Lobby_Map", pc);
				//	}
				//}
				pc->Server_Confirm_TraineeAassignment();
				return true;
			}
			else
			{
				if (auto* hud = Get_HUD()) {
					FRtDele_MessagResult ResultCallback = FRtDele_MessagResult::CreateLambda([this](ERt_MessageResult ConfirmationResult) {
						if (ConfirmationResult == ERt_MessageResult::Confirmed) {
							// Log Out Data Code 
							auto* pc = Get_PlayerController();
							//if (auto* gi = GetGameInstance()) {
							//	if (auto* lm = gi->GetSubsystem<URtLoadingManager>()) {
							//		lm->LoadLevelWithLoadingScreen("Lobby_Map", pc);
							//	}
							//}
							pc->Server_Confirm_TraineeAassignment();
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

bool URt_Place_Trainee::BPCall_Back()
{
	return false;
}

bool URt_Place_Trainee::BPCall_Quit()
{
	return false;
}

