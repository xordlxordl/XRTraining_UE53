// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RtCore/Public/Common/RtCommon.h"
#include "RtRpcCommon.generated.h"

// ENUM
UENUM(BlueprintType)
enum class EStatusInTraining : uint8
{
	Default,	// not participating in training
	Ready,		// participate in training
	Training,
	TrainingEnd,
	Exclusion,	//out of training , Training exclusion status by manager(controller)
	Error,
};

UENUM(BlueprintType)
enum class EResultList : uint8
{
	Default,	// not participating in training
	Ready,		// participate in training
	Training,
	TrainingEnd,
	Exclusion,	//out of training , Training exclusion status by manager(controller)
	Error,
};

// Struct
// RPC
struct FRtUserInfo_PreLogin
{
	FString Options;
	FString Address;
	FString UniqueId;
	FString ErrorMessage;
	FString TimeStr;

	ERt_PlayType PlayType;
	FString PlayerName;
	FString PlayerId;
	FString PlayerPassword;
	FString DeviceName;
	FString DeviceId;

	void Print_Log();
	void Set_Options(const FString& option_p);
};

struct FRtUserInfo_PostLogin
{
	FString Unique_Id;
	FString Player_Id;
	FString Player_Name;
	FString TimeStr;

	void Print_Log();
};

USTRUCT(BlueprintType)
struct RTGAME_API FRtScenarioInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 PlayerNum;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 TimeLimit;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	    FString Summary;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Destination;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	    FString FileName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	    FString SCTFileName;


	FRtScenarioInfo() :
		Name(""),
		PlayerNum(1),
		TimeLimit(60),
		Summary(""),
		Destination(""),
		FileName(""),
		SCTFileName("")
	{
	}
};

USTRUCT(BlueprintType)
struct RTGAME_API FRtPlaySettings
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString ServerIp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 PlayViewMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 TrackerGroupId;

	bool bDefaultData;
	FName ServerIp_Name;

	FRtPlaySettings() :
		ServerIp(""),
		PlayViewMode(0),
		TrackerGroupId(0),
		bDefaultData(true)
	{
	}

	void Set_ServerId(const FString & InStr) {
		ServerIp = InStr;
		ServerIp_Name = FName(ServerIp);
	}
};


//USTRUCT(BlueprintType)
//struct RTGAME_API FRtDeviceIdentity
//{
//	GENERATED_USTRUCT_BODY()
//
//public:
//	UPROPERTY(EditAnywhere, BlueprintReadOnly)
//		FString Name;
//	UPROPERTY(EditAnywhere, BlueprintReadOnly)
//		FString Id;
//	UPROPERTY(EditAnywhere, BlueprintReadOnly)
//		int32 TrackerGroupId;
//
//	FRtDeviceIdentity() :
//		Name(""),
//		Id("")
//	{}
//};
//

USTRUCT(BlueprintType)
struct RTGAME_API FRtSuddenEvtList
{
	GENERATED_USTRUCT_BODY()
public:
	TArray<FString> EvtList;
};

USTRUCT(BlueprintType)
struct RTGAME_API FRtRoleInfoList
{
	GENERATED_USTRUCT_BODY()
public:
	TArray<FRtRoleInfo> RoleList;

	FRtRoleInfoList() = default;
};

USTRUCT(BlueprintType)
struct RTGAME_API FRtRoleInfo
{
	GENERATED_USTRUCT_BODY()

public :
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	FString TraineeRole;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)  FString RoleID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)  int32 RID;

	FRtRoleInfo() = default;
};



USTRUCT(BlueprintType)
struct RTGAME_API FRtTraineeInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	FString TraineeName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	FString TraineeSN;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)  float Traineeheight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 UID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString RoleName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)  FString RoleID;
	
	static TArray<FRtTraineeInfo> DefualtArray;


	//FRtTraineeInfo() :TraineeName(TEXT("훈련자 할당")), TraineeSN(TEXT("군번")) {}
	FRtTraineeInfo() :TraineeName(TEXT("Trainee")), TraineeSN(TEXT("Server Number")), Traineeheight(0.0f), UID(0), RoleName(""), RoleID("") {}
	void Empty() {
		TraineeName = "";
		TraineeSN = "";
		RoleName = "";
		Traineeheight = 0.0f;
		UID = 0;
		RoleID = "";
	}
	FRtTraineeInfo(const FString& InTraineeSN, const FString& InTraineeName, const FString& InUID, const float& Inheight, const FString& InRoleName, const FString& InRoleid)
	{
		TraineeName = InTraineeName;
		TraineeSN = InTraineeSN;
		Traineeheight = Inheight;
		RoleName = InRoleName;
		RoleID = InRoleid;
		if (FCString::IsNumeric(*InUID))
			UID = FCString::Atoi(*InUID);
		else
			UID = 0;
	}
};

USTRUCT(BlueprintType)
struct RTGAME_API FRtDeviceInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString PlayerRole;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString PlayerRoleID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 PlayerRoleIndex; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	    int32 DeviceIndex;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 TraineeUID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	    float TraineeHeight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool Active; // true : login 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<bool> Ready;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 PlayerId; // player id in playerstate
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 StartTargetPointIndex; //        ġ
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool InStartingPoint; // occupied starting point
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool CompleteCalibration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool CompleteMapLoad;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EStatusInTraining StatusInTraining;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool Assigned_Trainee; // true : Assigned_Trainee
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool Assigned_Role; // true : Assigned_Role


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRtTraineeInfo PlayerInfo;


	static FRtDeviceInfo Defualt;
	static TArray<FRtDeviceInfo> DefualtArray;

	FRtDeviceInfo() :
		Name(""),
		Id(""),
		//PlayerRole(TEXT("보직")),
		PlayerRole(TEXT("Role")),
		PlayerRoleID(TEXT("ID")),
		PlayerRoleIndex(),
		DeviceIndex(0),
		//TraineeName(""),
		TraineeUID(),
		TraineeHeight(),
		Active(false),
		//Ready(false),
		PlayerId(0),
		StartTargetPointIndex(-1),
		InStartingPoint(false),
		CompleteCalibration(false),
		CompleteMapLoad(false),
		StatusInTraining(EStatusInTraining::Default),
		Assigned_Trainee(false),
		Assigned_Role(false)
	{
		Ready.Init(false, 3); 
	}
	FRtDeviceInfo(const FRtDeviceInfo& InInfo) :FRtDeviceInfo() {
		Id = InInfo.Id;
		Name = InInfo.Name;
		PlayerRole = InInfo.PlayerRole;
		PlayerRoleID = InInfo.PlayerRoleID;
		PlayerRoleIndex = InInfo.PlayerRoleIndex;
		DeviceIndex = InInfo.DeviceIndex;
		TraineeUID = InInfo.TraineeUID;
		TraineeHeight = InInfo.TraineeHeight;
		Active = InInfo.Active;
		Ready = InInfo.Ready;
		PlayerId = InInfo.PlayerId;
		StartTargetPointIndex = InInfo.StartTargetPointIndex;
		InStartingPoint = InInfo.InStartingPoint;
		CompleteCalibration = InInfo.CompleteCalibration;
		CompleteMapLoad = InInfo.CompleteMapLoad;
		StatusInTraining = InInfo.StatusInTraining;
		Assigned_Trainee = InInfo.Assigned_Trainee;
		Assigned_Role = InInfo.Assigned_Role;
		PlayerInfo = InInfo.PlayerInfo;
	}
	void Logout() {
		// 해당 부분은 관제에서 VR-07 보직 부분이 BLANK 처리가 됨
		//Id = "";
		//Name = "";
		PlayerRole = "";
		PlayerRoleID = "";
		PlayerRoleIndex = 0;
		TraineeUID = 0;
		TraineeHeight = 0;
		Active = false;
		//Ready = false;
		Ready.Init(false, 3);
		PlayerId = 0;
		StatusInTraining = EStatusInTraining::Default;
		Assigned_Trainee = false;
		Assigned_Role = false;
		PlayerInfo = FRtTraineeInfo();
	}
    
	// 관찰용 Logout
	void Logout_Ob()
	{
		TraineeUID = 0;
		TraineeHeight = 0;
		Active = false;
		PlayerId = 0;
		StatusInTraining = EStatusInTraining::Default;
		Assigned_Trainee = false;
		Assigned_Role = false;
		PlayerInfo = FRtTraineeInfo();
		PlayerInfo.TraineeName = TEXT("관찰생");
	}
	void Reset_Trainee()
	{
	    
	}

	void Assign_Trainee(int32 InTraineeUID, int32 InPlayerId, float InTraineeHeight) {
		TraineeUID = InTraineeUID;
		TraineeHeight = InTraineeHeight;
		PlayerInfo.Empty();
		check(PlayerId == InPlayerId);
	}

	void Assign_Trainee(const FRtTraineeInfo& InPlayerInfo, int32 InPlayerId, float InTraineeHeight) {
		TraineeUID = InPlayerInfo.UID;
		TraineeHeight = InTraineeHeight;
		PlayerInfo = InPlayerInfo;
		check(PlayerId == InPlayerId);
	}
	void Assign_Trainee(const FRtTraineeInfo& InPlayerInfo) {
		TraineeUID = InPlayerInfo.UID;
		TraineeHeight = InPlayerInfo.Traineeheight;
		PlayerInfo = InPlayerInfo;
	}

	void Assign_Role(const FRtRoleInfo& InRoleInfo) {
		PlayerRole = InRoleInfo.TraineeRole;
		PlayerRoleID = InRoleInfo.RoleID;
		PlayerRoleIndex = InRoleInfo.RID;
	}

	bool IsValid_TraineeInformation() { //``Check valid trainee information
		if (TraineeUID == 0)
			return false;
		return true;
	}

	FString Get_String() {
		return FString::Printf(TEXT("[%s] %s,%d"), *Name, *PlayerInfo.TraineeName, TraineeUID);
	}

	bool bHasTrainee() {
		return (TraineeUID > 0) ? true : false;
	}

	bool bHasRole() {
		return (PlayerRoleIndex > 0) ? true : false;
	}

};




// namespace
namespace RtString_Option
{
	extern RTGAME_API const FString PlayerType;
	extern RTGAME_API const FString PlayerName;
	extern RTGAME_API const FString PlayerId;
	extern RTGAME_API const FString PlayerPassword;
	extern RTGAME_API const FString DeviceName;
	extern RTGAME_API const FString DeviceId;
}
