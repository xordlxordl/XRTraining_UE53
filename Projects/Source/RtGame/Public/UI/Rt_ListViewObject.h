// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

// Data
#include "RtCore/Public/Common/RtCommon.h"
#include "Common/RtRpcCommon.h"

#include "Rt_ListViewObject.generated.h"

class URt_Place_Trainee;
class URt_Assign_Trainee;
/**
 * 
 */

UCLASS()
class RTGAME_API URt_ListViewObject_ScenarioInfo : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FRtScenarioInfo Info_Scenario;

	void FillData_Scenario(const FRtScenarioInfo& InScenarioData);

};

UCLASS(BlueprintType)
class RTGAME_API URt_ListViewObejct_DeviceInfo : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)	FRtDeviceInfo Info;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)  URt_Place_Trainee* Widget_PlaceTrainee;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)  URt_Assign_Trainee* Widget_AssingTrainee;


	UFUNCTION(BlueprintCallable)
	void Fill_Data(const FRtDeviceInfo& InDeviceStatus);
	UFUNCTION(BlueprintCallable)
	void Change_Player_Name(const FString& InName);
	UFUNCTION(BlueprintCallable)
	void Change_Player_SN(const FString& InSN );


};

UCLASS(BlueprintType)
class RTGAME_API URt_ListViewObject_TraineeInfo : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere) FRtTraineeInfo Info;

	UFUNCTION(BlueprintCallable)
	void Fill_Data(const FRtTraineeInfo& InPlayerStatus);


};


UCLASS(BlueprintType)
class RTGAME_API URt_ListViewObject_RoleInfo : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere) FRtRoleInfo Info;


	UFUNCTION(BlueprintCallable)
	void Fill_Data(const FRtRoleInfo& InPlayerStatus);


};


