// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Enums 
UENUM(BlueprintType)
enum class ERc_WidgetList : uint8
{
	Login,
	MainMenu,
};

UENUM(BlueprintType)
enum class ERt_PlayType : uint8
{
	E_None	UMETA(DisplayName = "None"),
	E_Server	UMETA(DisplayName = "Server"),        // Listen
	E_Control UMETA(DisplayName = "Controller"),      // 관제
	E_Trainee UMETA(DisplayName = "Trainee"),         // VR 훈련생
	E_Replay UMETA(DisplayName = "Replay"),           // 리플레이 뷰어
	E_Single UMETA(DisplayName = "Single"),           // 싱글플레이
};

/** Possible results from a dialog */
UENUM(BlueprintType)
enum class ERt_MessageResult : uint8
{
	/** The "yes" button was pressed */
	Confirmed,
	/** The "no" button was pressed */
	Declined,
	/** The "ignore/cancel" button was pressed */
	Cancelled,
	/** The dialog was explicitly killed (no user input) */
	Killed,
	Unknown UMETA(Hidden)
};
// 왜 굳이 common 에 선언했는지에 대한 의문 : 여러 군데서 이 핸들러를 사용하기 위함
DECLARE_DELEGATE_OneParam(FRtDele_MessagResult, ERt_MessageResult /* Result */);


enum class ERt_GameMode
{
	E_None,
	E_Menu,           // Used Test 
	E_Lobby,          // 
	E_Multiplay,     // Basic 
	E_Soloplay       // Used Test
};

// Structures


// namespaces
namespace RtMatchState
{
	extern RTCORE_API const FName EnteringMap;
	extern RTCORE_API const FName WaitingToStart;
	extern RTCORE_API const FName InProgress_StartTrng;
	extern RTCORE_API const FName InProgress_PlayerReady;
	extern RTCORE_API const FName InProgress_RunningTrng;
	extern RTCORE_API const FName InProgress_PreResult;
	extern RTCORE_API const FName InProgress_Result;
	extern RTCORE_API const FName WaitingPostMatch;
	extern RTCORE_API const FName LeavingMap;
	extern RTCORE_API const FName Aborted;
}



/**
 * 
 */
class RTCORE_API RtCommon
{
public:
	RtCommon();
	~RtCommon();
};
