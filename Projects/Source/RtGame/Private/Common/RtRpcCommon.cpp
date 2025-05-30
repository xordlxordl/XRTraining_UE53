// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/RtRpcCommon.h"

namespace RtString_Option
{
	const FString PlayerType = FString(L"PlayerType");
	const FString PlayerName = FString(L"PlayerName");
	const FString PlayerId = FString(L"PlayerId");
	const FString PlayerPassword = FString(L"PlayerPassword");
	const FString DeviceName = FString(L"DeviceName");
	const FString DeviceId = FString(L"DeviceId");
}
FRtDeviceInfo FRtDeviceInfo::Defualt;
TArray<FRtDeviceInfo> FRtDeviceInfo::DefualtArray;
TArray<FRtTraineeInfo> FRtTraineeInfo::DefualtArray;

