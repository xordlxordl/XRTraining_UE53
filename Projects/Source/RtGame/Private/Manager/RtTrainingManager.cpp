// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/RtTrainingManager.h"

// Manager 
#include "Manager/RtConfig.h"





void URtTrainingManager::SetTID(int32 TID)
{
	TrainingIndex = TID;
}

bool URtTrainingManager::Initialize_Manager()
{

	return true;
}

FRtTraineeInfo* URtTrainingManager::Get_TraineeInfo(int32 InUID)
{
    return nullptr;
}

bool URtTrainingManager::IsAssigned_ToDevice(int32 InPlayerId)
{
	if (Device_StatusArray.ContainsByPredicate([InPlayerId](const FRtDeviceInfo& InInfo) { return InInfo.PlayerId == InPlayerId; })) {
		UE_LOG(LogTemp, Log, TEXT("Already Exist player id %d"), InPlayerId);
		return true;
	}
    return false;
}

void URtTrainingManager::Update_TraineeArray(const TArray<FRtTraineeInfo>& InData)
{
	for (auto& it : InData) {
		User_Container.FindOrAdd(it.UID, it);
		Trainee_StatusArray.Add(it);
	}
}

void URtTrainingManager::Update_RoleArray(const TArray<FRtRoleInfo>& InData)
{
	Role_Container.Empty();
	for (int i = 0; i < InData.Num(); i++) {
		FRtRoleInfo Info = InData[i];
		Role_Container.Add(Role_Container.Num(), Info);
	}
}

void URtTrainingManager::Update_WebDeviceArray(const TArray<FRtDeviceInfo>& InData)
{
	Device_IdentityArray.Empty();

	for (auto& it : InData) {
		Device_IdentityArray.Add(it);
		Device_StatusArray.Add(it);
	}
}

void URtTrainingManager::Update_ScenarioArray(const TArray<FString>& InData)
{
	ScenarioName_Array.Empty();

	for (auto& it : InData) {
		ScenarioName_Array.Add(it);
	}
}

void URtTrainingManager::Update_SuddenEvtArray(const TArray<FString>& InData)
{
	SuddenEvtList = InData;
}

FRtDeviceInfo* URtTrainingManager::Get_DeviceStatus(const FString& InDeviceName)
{
	auto* found_device_info = Device_StatusArray.FindByPredicate([InDeviceName](const FRtDeviceInfo& InInfo) { return InInfo.Name.Equals(InDeviceName); });
	return found_device_info;
}

FRtDeviceInfo* URtTrainingManager::AssignTrainee_ToDevice(const FString& InDeviceName, const FRtTraineeInfo& InInfo)
{
	if (auto* found = Get_DeviceStatus(InDeviceName)) {
		found->Assign_Trainee(InInfo);
		return found;
	}
	return nullptr;
}


FRtDeviceInfo* URtTrainingManager::AssignRole_ToDevice(const FString& InDeviceName, const FRtRoleInfo& InInfo)
{
	if (auto* found = Get_DeviceStatus(InDeviceName)) {
		FRtRoleInfo AssignedRoleInfo = InInfo;
		found->Assign_Role(AssignedRoleInfo);
		return found;
	}
	return nullptr;
}

bool URtTrainingManager::Check_TrngStartCondition()
{
	int cnt = 0;
	for (auto& it : Device_StatusArray) {
		if (it.Active && it.InStartingPoint) {
			++cnt;
		}
	}

	if (Scenario_Info.PlayerNum != cnt) {
		return false;
	}
	return true;
}

// Flow
void URtTrainingManager::SetControlFlow(EControlFlow ctrlflow)
{
	state_ctrl = ctrlflow;
}

