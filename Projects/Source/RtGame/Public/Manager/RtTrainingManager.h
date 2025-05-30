// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

// Data
#include "RtCore/Public/Common/RtCommon.h"
#include "Common/RtRpcCommon.h"

#include "RtTrainingManager.generated.h"

class ARtGameMode;
class ARtPlayerController;
/**
 * 
 */

UENUM(BlueprintType)
enum class EControlFlow : uint8
{
	CONTROL_NONE,
	CONTROL_LOGIN,
	CONTROL_SETSCT,
	CONTROL_INSTALLWALL,
	CONTROL_ASSIGNTRAINEE,
	CONTROL_WAITINGTRAINING,
	CONTROL_PROCESSINGTRAINING,
	CONTROL_RESULT
};


UCLASS()
class RTGAME_API URtTrainingManager : public UObject
{
	GENERATED_BODY()
	
// Various
public:
	UPROPERTY()	FString TraingGroundID;
	UPROPERTY(BlueprintReadWrite)	TArray<FRtDeviceInfo> Device_IdentityArray;
	UPROPERTY()	TArray<FRtDeviceInfo> Device_StatusArray;
	UPROPERTY() TArray<FRtTraineeInfo> Trainee_StatusArray; // Only List of accessible player
 	UPROPERTY() TArray<FString> ScenarioName_Array;
	UPROPERTY() TArray<FRtRoleInfo> Role_StatusArray;
	UPROPERTY() TMap<FString, FRtRoleInfoList> Whole_Role_Container;
	UPROPERTY() TMap<FString, FRtSuddenEvtList> Whold_SEvt_Container;

	UPROPERTY(BlueprintReadOnly)	TArray<FRtDeviceInfo> DeviceArray_BeingTrained;  //State list of devices participating in scenario training
	UPROPERTY()	TMap<int32, FRtTraineeInfo> User_Container; //List of accessible player
	UPROPERTY()	TMap<int32, FRtRoleInfo> Role_Container; //List of accessible Role
	
	UPROPERTY(BlueprintReadOnly)	TArray<FString> AssignedRoles; // There's not Overlapped Value. Never
	UPROPERTY(BlueprintReadOnly)    TArray<FString> SuddenEvtList; // for Training View Widget POP UP
	UPROPERTY(BlueprintReadOnly) TMap<FString/*RoleName*/, FString/*DeviceName*/> AssignedRole_Container;
	UPROPERTY(BlueprintReadOnly) TMap<FString/*TraineeNumber*/, FString /*DeviceName*/> AssignedTrainee_Container;

	FRtScenarioInfo Scenario_Info;

	// Flow
	UPROPERTY(BlueprintReadWrite) EControlFlow state_ctrl = EControlFlow::CONTROL_NONE;
	void SetControlFlow(EControlFlow ctrlflow);

	// test cnt

	// TID
	UPROPERTY(BlueprintReadWrite) int32 TrainingIndex;
	void SetTID(int32 TID);

// Functions
public :
	bool Initialize_Manager();
	bool IsAssigned_ToDevice(int32 InPlayerId); // Is it assigned to a device?
	void Update_TraineeArray(const TArray<FRtTraineeInfo>& InData);
	void Update_RoleArray(const TArray<FRtRoleInfo>& InData);
	void Update_WebDeviceArray(const TArray<FRtDeviceInfo>& InData);
	void Update_ScenarioArray(const TArray<FString>& InData);
	void Update_SuddenEvtArray(const TArray<FString>& InData);

	// Custom Get Set
	FRtTraineeInfo* Get_TraineeInfo(int32 InUID);
	const FRtScenarioInfo& Get_ScenarioInfo() { return Scenario_Info; }
	//``디바이스 정보
	const TArray<FRtDeviceInfo>& Get_Device_StatusArray() { return Device_StatusArray; }  //! not const
	// 훈련자 정보
	const TArray<FRtTraineeInfo>& Get_Trainee_StatusArray() { return Trainee_StatusArray; }
	// SuddenEvtInfo
	const TArray<FString>& Get_SuddenEvtArray() { return SuddenEvtList; }

	FRtDeviceInfo* Get_DeviceStatus(const FString& InDeviceName);
	FRtDeviceInfo* AssignTrainee_ToDevice(const FString& InDeviceName, const FRtTraineeInfo& InInfo);
	FRtDeviceInfo* AssignRole_ToDevice(const FString& InDeviceName, const FRtRoleInfo& InInfo);

	bool Check_TrngStartCondition();

};
