// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Rt_ListViewObject.h"

void URt_ListViewObject_ScenarioInfo::FillData_Scenario(const FRtScenarioInfo& InScenarioData)
{
	Info_Scenario = InScenarioData;
}

void URt_ListViewObejct_DeviceInfo::Fill_Data(const FRtDeviceInfo& InDeviceStatus)
{
	Info = InDeviceStatus;
}

void URt_ListViewObejct_DeviceInfo::Change_Player_Name(const FString& InName)
{
	Info.PlayerInfo.TraineeName = InName;
}

void URt_ListViewObejct_DeviceInfo::Change_Player_SN(const FString& InSN)
{
	Info.PlayerInfo.TraineeSN = InSN;
}

void URt_ListViewObject_TraineeInfo::Fill_Data(const FRtTraineeInfo& InPlayerStatus)
{
	Info = InPlayerStatus;
}

void URt_ListViewObject_RoleInfo::Fill_Data(const FRtRoleInfo& InPlayerStatus)
{
	Info = InPlayerStatus;
}

