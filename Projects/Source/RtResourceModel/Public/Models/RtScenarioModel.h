// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "RtEnums.h"
#include "RtDefines.h"

#include "RtResourceModelSubsystem.h"

#include "RtScenarioModel.generated.h"

RTRESOURCEMODEL_API DECLARE_LOG_CATEGORY_EXTERN(RtEvent, Log, All);

USTRUCT(BlueprintType, Category = "FRtEventProcess")
struct FRtEventProcess
{
	GENERATED_BODY()
public:
	FRtEventProcess()
	{}

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtEventProcess")
	//ERtSctEventProcess ProcessType = ERtSctEventProcess::ENone;
	//
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtEventProcess")
	//ERtSctEventProcessCategory ProcessCategory = ERtSctEventProcessCategory::ENone;
	//
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtEventProcess")
	//ERtSctEventProcessCategorySub ProcessCategorySub = ERtSctEventProcessCategorySub::ENone;
	//
	// mProcessType: EEvent 인 경우 Event ID,
	// mProcessType: EFunction 인 경우 오우너 객체의 기능 컴포넌트 작동 Index ("0", "1", ~)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtEventProcess")
	FString ProcessScript;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtEventProcess")
	//FString ProcessSubValue;
	//
	//// [ mProcessType : EOwner 인 경우이면서 오우너의 타입이 관계 없는 경우.. ]
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtEventProcess")
	//TArray<FRtProcessUI> UIList; // 기본적으로 1개만 허용한다. 1개 인데 컨테이너 구조를 갖는 이유는 이 데이터를 사용 안할때의 데이터 사이즈를 최소화 하기 위해서다.
	//
	//// [ mProcessType : EOwner 인 경우이면서 오우너 타입이 NPC 인경우.. ]
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtEventProcess")
	//TArray<FRtProcessNPC> NpcList; // 기본적으로 1개만 허용한다. 1개 인데 컨테이너 구조를 갖는 이유는 이 데이터를 사용 안할때의 데이터 사이즈를 최소화 하기 위해서다.
	//	 
	//// [ mProcessType : ETimer 인 경우.. ]
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtEventProcess")
	//TArray<FRtProcessTimer> TimerList; // 기본적으로 1개만 허용한다. 1개 인데 컨테이너 구조를 갖는 이유는 이 데이터를 사용 안할때의 데이터 사이즈를 최소화 하기 위해서다.
};

USTRUCT(BlueprintType, Category = "FRtEvent")
struct FRtEvent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtEvent")
	FString EventNameTID; // 이벤트 이름 TID

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtEvent")
	FText EventName; // 이벤트 이름

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtEvent")
	TArray<FRtEventProcess> EventProcessList; // 실행 이벤트 리스트

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtEvent")
	//FString ProcessScript; // 실행 이벤트
};

USTRUCT(BlueprintType, Category = "FRtUIMenuElement")
struct FRtUIMenuElement : public FRtEvent
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, Category = "FRtUIMenuElement")
	int32 EventIndex = 0; // 몇 시 방향인지(12시 방향이 Index 0)

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtUIMenuElement")
	//FString MenuNameTID; // 메뉴 이름 TID
	//
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtUIMenuElement")
	//FText MenuName; // 메뉴 이름
	//
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtUIMenuElement")
	//FString ProcessScript; // 메뉴 선택시 실핼 될 이벤트 이름 ID
};

USTRUCT(BlueprintType, Category = "FRtNpcPatternElement")
struct FRtNpcPatternElement
{
	GENERATED_BODY()
public:
	FRtNpcPatternElement()
	{
		Weight = 0;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtNpcPatternElement")
	FString PatternID; // 패턴 ID

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtNpcPatternElement")
	int32 Weight = 0; // 비중 ( 0~100 )
};

USTRUCT(BlueprintType, Category = "FRtNpcScriptElement")
struct FRtNpcScriptElement
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtNpcScriptElement")
	FString ScriptSoundFilePath; // 사운드 파일 경로

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtNpcScriptElement")
	FString ScriptTextTID; // 스크립트 텍스트 TID

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtNpcScriptElement")
	FText ScriptText; // 스크립트 텍스트
};

USTRUCT(BlueprintType, Category = "FRtProcessTimer")
struct FRtProcessTimer
{
	GENERATED_BODY()
public:
	FRtProcessTimer()
	{
		TimerValue = 0;
	}
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtProcessTimer")
	FString TimerID; // 대상 타이머 ( 생성 및 사용 )

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtProcessTimer")
	float TimerValue = 0; // 대상 타이머 값 ( 생성 전용 ) 
};

USTRUCT(BlueprintType, Category = "FRtProcessUI")
struct FRtProcessUI
{
	GENERATED_BODY()
public:
	FRtProcessUI()
	{
		UIType = ERtSctEventUIType::ENone;
	}
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtProcessUI")
	ERtSctEventUIType UIType = ERtSctEventUIType::ENone;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtProcessUI")
	TArray<FRtUIMenuElement> UIMenuElementList;
};

USTRUCT(BlueprintType, Category = "FRtProcessNPC")
struct FRtProcessNPC
{
	GENERATED_BODY()
public:
	// mProcessCategory : EOwnerNpc_Move 일 경우 이동 위치.
	////////////////////////////////////////////////////////////////////////////
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtProcessNPC")
	TArray<FVector2D> NpcMoveToList; // 기본적으로 1개만 허용한다. 1개 인데 컨테이너 구조를 갖는 이유는 데이터 사이즈를 최소화 하기 위해서다.

	////////////////////////////////////////////////////////////////////////////

	// mProcessCategory : EOwnerNpc_Action 일 경우..
	////////////////////////////////////////////////////////////////////////////
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtProcessNPC")
	TArray<FRtNpcPatternElement> NpcPatternList; // 갯수가 1개이면 해당 패턴만을, 2개 이상이면 내부 가중치 기반의 랜덤 값을 사용.
	////////////////////////////////////////////////////////////////////////////

	// mProcessCategory : EOwnerNpc_Speech 일 경우..
	////////////////////////////////////////////////////////////////////////////

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtProcessNPC")
	TArray<FRtNpcScriptElement> NpcScriptList; // 기본적으로 1개만 허용한다. 1개 인데 컨테이너 구조를 갖는 이유는 데이터 사이즈를 최소화 하기 위해서다.
};

USTRUCT(BlueprintType, Category = "FRtScenarioCondition")
struct FRtScenarioCondition
{
	GENERATED_BODY()
public:
	FRtScenarioCondition()
	{}

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioCondition")
	//ERtSctCondition ConditionType = ERtSctCondition::ENone;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioCondition")
	//ERtSctTarget Target1Type = ERtSctTarget::ENone;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioCondition")
	//ERtSctTargetState Target1State = ERtSctTargetState::ENone;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioCondition")
	FString ConditionScript;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioCondition")
	//ERtSctTarget Target2Type = ERtSctTarget::ENone;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioCondition")
	//FString Target2Value;

};

// sct 핵심 자료 구조
// 이벤트는 조건과 처리라는 두가지 주제로 작동된다. 조건들이 모두 만족하면 처리되는 구조.
USTRUCT(BlueprintType, Category = "FRtScenarioEvent")
struct FRtScenarioEvent : public FRtEvent
{
	GENERATED_BODY()
public:
	FRtScenarioEvent()
	{
		//EventHandleType = ERtEventHandleType::ENone;
	}

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioEvent")
	//FString EventNameTID; // 이벤트 이름 TID

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioEvent")
	//FText EventName; // 이벤트 이름

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioEvent")
	//FString EventCategory; // 이벤트 카테고리 ( 이 값을 기준으로 최우선 소트 된다 )

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioEvent")
	//ERtEventHandleType EventHandleType; // 이벤트 핸들 타입 ( 어디에서 구동되는지를 알려준다, 서버 or 클라이언트 ) 

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioEvent")
	//FString OwnerID; // 오우너 객체 ID (복수입력가능 : "a,b,c")

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioEvent")
	//FString TargetID; // 대상 ID( 일반적으로 PC or NPC ) (복수입력가능 : "a,b,c")

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioEvent")
	TArray<FRtScenarioCondition> EventOccurConditionList;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioEvent")
	//TArray<FRtEventProcess> EventProcessList;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioEvent")
	//FString PreconditionEventNameTIDs; // 전제 조건 이벤트 ID들.

};

// sct 핵심 자료 구조
// 이벤트는 조건과 처리라는 두가지 주제로 작동된다. 조건들이 모두 만족하면 처리되는 구조.
USTRUCT(BlueprintType, Category = "FRtScenarioSuddenEvent")
struct FRtScenarioSuddenEvent : public FRtEvent
{
	GENERATED_BODY()
public:
	FRtScenarioSuddenEvent()
	{}

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioSuddenEvent")
	//FString SuddenEventNameTID; // 돌발 이벤트 TID
	//
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioSuddenEvent")
	//FText SuddenEventName; // 돌발 이벤트 이름
	//
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioSuddenEvent")
	//TArray<FRtEventProcess> EventProcessList;
};

USTRUCT(BlueprintType, Category = "FRtScenarioScript")
struct FRtScenarioScript
{
	GENERATED_BODY()
public:
	FRtScenarioScript()
	{
		FunctionCommand = ERtSctFunCmd::ENone;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioScript")
	ERtSctFunCmd FunctionCommand; // 함수 명령.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioScript")
	TArray<FString> FunctionParamList; // 파라메터 리스트.

};

//USTRUCT(BlueprintType, Category = "FRtScenarioMission")
//struct FRtScenarioMission
//{
//	GENERATED_BODY()
//public:
//	FRtScenarioMission()
//	{
//	}
//	
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioMission")
//	FString MissionNameTID; // 미션 이름 TID
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioMission")
//	FString MissionSummaryTID; // 미션 개요 TID
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioMission")
//	FText MissionName; // 미션 이름
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioMission")
//	FText MissionSummary; // 미션 개요
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioMission")
//	TArray<FRtScenarioCondition> MissionStartConditionList;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioMission")
//	TArray<FRtScenarioCondition> MissionGoalConditionList;
//};

USTRUCT(BlueprintType, Category = "FRtScenarioPlayer")
struct FRtScenarioPlayer
{
	GENERATED_BODY()
public:
	FRtScenarioPlayer()
	{
		PlayerObjectID = TEXT("");
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioPlayer")
	FString PlayerObjectID; // mct 상의 생성될 플레이어 객체 ID

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioPlayer")
	FString PlayerName; // 플레이어 이름

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioPlayer")
	FString GameReadyPlaceObjectID; // 게임 준비 장소 객체 ID

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioPlayer")
	FString EquipObjectID; // 장비 장착 객체 ID

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioPlayer")
	int32 IsGroupLeader = 0; // 그룹의 리더 여부. (1: 리더, 0: 대원)
};

// Pc, Npc 모두 포함하는 개념임.
USTRUCT(BlueprintType, Category = "FRtScenarioPlayerGroup")
struct FRtScenarioPlayerGroup
{
	GENERATED_BODY()
public:
	FRtScenarioPlayerGroup()
	{}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioPlayerGroup")
	FString PlayerGroupID; // 플레이어 그룹 ID

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioPlayerGroup")
	FString PlayerGroupName; // 플레이어 그룹 이름

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioPlayerGroup")
	ERtPlayerType PlayerType; // 플레이어 타입. PC or NPC

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioPlayerGroup")
	FString GroupInfoText; // PC 플레이어 그룹 좌상단 표시 텍스트, NPC는 사용 안함

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioNpcAI")
	TArray<FRtScenarioPlayer> PlayerList;  // 플레이어 리스트
};

USTRUCT(BlueprintType, Category = "FRtDropboxBase")
struct FRtDropboxBase
{
	GENERATED_BODY()
public:
	FRtDropboxBase()
	{}
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtDropboxBase")
	FString DropboxID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtDropboxBase")
	FString DropboxName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtDropboxBase")
	TArray<FRtUIMenuElement> DropboxElementList;
};

USTRUCT(BlueprintType, Category = "FRtDropbox")
struct FRtDropbox : public FRtDropboxBase
{
	GENERATED_BODY()
public:
	FRtDropbox()
	{
		//UIType = ERtSctEventUIType::ENone;
	}
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtDropbox")
	//	FString DropboxID;
	//
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtDropbox")
	//	FString DropboxName;
	//
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtDropbox")
	//	ERtSctEventUIType UIType = ERtSctEventUIType::ENone;

	UPROPERTY(BlueprintReadWrite, Category = "FRtDropbox")
	TArray<FString> DropboxOwnerList; // 메뉴 세트를 소유한 객체 ID 리스트.

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtDropbox")
	//	TArray<FRtUIMenuElement> DropboxElementList;
};

USTRUCT(BlueprintType, Category = "FRtRadioDropbox")
struct FRtRadioDropbox : public FRtDropboxBase // 무전기
{
	GENERATED_BODY()
public:
	FRtRadioDropbox()
	{}
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtRadioDropbox")
	//FString DropboxID;
	//
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtRadioDropbox")
	//FString DropboxName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtRadioDropbox")
	ERtSctSystemMenuScreenPos ScreenPosInfo = ERtSctSystemMenuScreenPos::ELeftBottom; // 좌하단이 기본 값.

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtRadioDropbox")
	//TArray<FRtUIMenuElement> DropboxElementList;
};

USTRUCT(BlueprintType, Category = "FRtVariable")
struct FRtVariable
{
	GENERATED_BODY()
public:
	FRtVariable()
	{}
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtVariable")
	FString VariableNameID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtVariable")
	FString VariableName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtVariable")
	int32 InitValue;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtVariable")
	//int32 IsUnexpectedVariable = 0; // 돌발 변수 여부. (1: 돌발 변수, 0: 일반 변수)
};

USTRUCT(BlueprintType, Category = "FRtScenarioData")
struct FRtScenarioData
{
	GENERATED_BODY()
public:
	FRtScenarioData()
	{
		Version = 0; // 구버전은 0.
	}

	void SetVersion(int32 version)
	{
		Version = version;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioData")
	int32 Version; // 버전

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioData")
	FString ScenarioNameTID; // 시나리오 이름 TID

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioData")
	FString ScenarioSummaryTID; // 시나리오 개요 TID

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioData")
	FString ScenarioGoalTID; // 시나리오 목표 TID

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioData")
	FText ScenarioName; // 시나리오 이름 (TID를 기반으로 텍스트를 가져와 설정한다)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioData")
	FText ScenarioSummary; // 시나리오 개요 (TID를 기반으로 텍스트를 가져와 설정한다)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioData")
	FText ScenarioGoal; // 시나리오 목표 (TID를 기반으로 텍스트를 가져와 설정한다)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioData")
	float TrainingTime = 0; // 훈련 시간(초단위)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioData")
	int32 PlayerCount = 0; // 플레이어 인원수

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioData")
	FString MctFileName; // Mct 파일 이름

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioData")
	FString MctFilePath; // Mct 파일 경로

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioData")
	TArray<FRtScenarioEvent> EventList; // 이벤트 리스트.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioData")
	TArray<FRtDropbox> DropboxList; // 메뉴 세트 리스트.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioData")
	TArray<FRtScenarioSuddenEvent> SuddenEventList; // 돌발 이벤트 리스트.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioData")
	TArray<FRtScenarioPlayerGroup> PlayerGroupList; // 플레이어 그룹 리스트.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioData")
	TArray<FRtVariable> VarialbeList; // 변수 리스트.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtScenarioData")
	FRtRadioDropbox RadioDropbox;  // 무전기 메뉴
};

USTRUCT(BlueprintType, Category = "FRtEventCommand")
struct FRtEventCommand
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "FRtEventCommand")
	FString EventID;

	UPROPERTY(BlueprintReadOnly, Category = "FRtEventCommand")
	FString EventCommandName; // 드롭박스 등에서 보이는 이름

	UPROPERTY(BlueprintReadOnly, Category = "FRtEventCommand")
	TArray<FRtSctFunctionCommand> ProcessCommandList;

	UPROPERTY(BlueprintReadOnly, Category = "FRtEventCommand")
	bool bLocalProcessOnControl = false; // true 이면 처리 실행시 서버로 메시지를 날려서 처리 하지 않고 관제에서만 자체 처리를 한다.
};

USTRUCT(BlueprintType, Category = "FRtScenarioSuddenEventCommand")
struct FRtDropboxEventCommand : public FRtEventCommand
{
	GENERATED_BODY()
public:
	FRtDropboxEventCommand()
	{}

	UPROPERTY(BlueprintReadOnly, Category = "FRtEventCommand")
	int32 EventIndex = -1; // 몇 시 방향인지(12시 방향이 Index 0)

	UPROPERTY(BlueprintReadOnly, Category = "FRtEventCommand")
	TArray<FString> VoiceCommandList; // 보이스 명령 리스트.

	UPROPERTY(BlueprintReadOnly, Category = "FRtEventCommand")
	bool bVisible = true; // 보여주기 여부.
};

USTRUCT(BlueprintType, Category = "FRtScenarioSuddenEventCommand")
struct FRtScenarioSuddenEventCommand : public FRtEventCommand
{
	GENERATED_BODY()
public:
	FRtScenarioSuddenEventCommand()
	{}

};

USTRUCT(BlueprintType, Category = "FRtDropboxBaseCommand")
struct FRtDropboxBaseCommand
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "FRtDropboxBaseCommand")
	FString DropboxID;

	UPROPERTY(BlueprintReadOnly, Category = "FRtDropboxBaseCommand")
	TArray<FRtDropboxEventCommand> DropboxElemantList;
};

USTRUCT(BlueprintType, Category = "FRtDropboxCommand")
struct FRtDropboxCommand : public FRtDropboxBaseCommand
{
	GENERATED_BODY()
public:
	FRtDropboxCommand()
	{}

	UPROPERTY(BlueprintReadOnly, Category = "FRtDropboxCommand")
	TArray<FString> DropboxOwnerList; // 메뉴 세트를 소유한 객체 ID 리스트.
};

USTRUCT(BlueprintType, Category = "FRtRadioDropboxCommand")
struct FRtRadioDropboxCommand : public FRtDropboxBaseCommand
{
	GENERATED_BODY()
public:
	FRtRadioDropboxCommand()
	{}

	// Radio Widget Screen Position
};

USTRUCT(BlueprintType, Category = "FRtScenarioEventCommand")
struct FRtScenarioEventCommand : public FRtEventCommand
{
	GENERATED_BODY()
public:
	FRtScenarioEventCommand()
	{}

	UPROPERTY(BlueprintReadOnly, Category = "FRtScenarioEventCommand")
	TArray<FRtSctFunctionCommand> ConditionCommandList;

};

USTRUCT(BlueprintType, Category = "FRtMissionInfo")
struct FRtMissionInfo
{
	GENERATED_BODY()
public:
	FRtMissionInfo() : MissionStartConditionOk(false), MissionGoalConditionOk(false), MissionArrayIndex(-1)
	{}

	FRtMissionInfo(bool missionStartConOk, bool missionGoalConOk, int32 index)
	{
		MissionStartConditionOk = missionStartConOk;
		MissionGoalConditionOk = missionGoalConOk;
		MissionArrayIndex = index;
	}

	bool MissionStartConditionOk;
	bool MissionGoalConditionOk;
	int32 MissionArrayIndex;
};

USTRUCT(BlueprintType, Category = "FRtIDList")
struct FRtIDList
{
	GENERATED_BODY()
public:
	FRtIDList() {}
	FRtIDList(const FRtIDList& idlist) { this->IDs = idlist.IDs; }
	FRtIDList(const TArray<FString>& arr) { IDs = arr; }
	FRtIDList(TArray<FString>& arr) { IDs = arr; }

	operator TArray<FString>() const { return IDs; }
	FRtIDList& operator = (TArray<FString>& list)
	{
		IDs = list;
		return *this;
	}

	UPROPERTY()
	TArray<FString> IDs;
};

UCLASS()
class RTRESOURCEMODEL_API ARtScenarioModel : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARtScenarioModel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:

	UFUNCTION(BlueprintCallable)
	bool LoadScenarioDataStruct(FString sctfilePath);

	UFUNCTION(BlueprintCallable)
	bool LoadSCT(FString sctfilePath);

	//const FRtScenarioEvent* FindEvent(FString eventID);

	bool RemoveRegistOccorConEvent(FString eventID);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	void SortEventList();

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	void SortPlayerGroupList();

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	void SortDropboxList();

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	void SortVariableList();

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	FString GetEventStringType(int32 eventIndex)
	{
		if (eventIndex < SctData.EventList.Num())
		{
			int32 conCount = SctData.EventList[eventIndex].EventOccurConditionList.Num();
			int32 processCount = SctData.EventList[eventIndex].EventProcessList.Num();

			if (conCount > 0 && processCount > 0)
				return TEXT("E");
			else if (conCount > 0)
				return TEXT("C");
			else if (processCount > 0)
				return TEXT("P");
		}

		return TEXT("");
	}

	/// Find ////////////////////////////////////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool FindEventItem(FString itemKey, FRtScenarioEvent& foundItem, int32& index);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool FindDropboxItem(FString itemKey, FRtDropbox& foundItem, int32& index);

	bool CheckDropboxOwner(TArray<FString> itemKey);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool FindSuddenEventItem(FString itemKey, FRtScenarioSuddenEvent& foundItem, int32& index);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool FindPlayerGroupItem(FString itemKey, FRtScenarioPlayerGroup& foundItem, int32& index);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool FindVariableItem(FString itemKey, FRtVariable& foundItem, int32& index);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool FindSystemMenuElementItem(FString itemKey, FRtUIMenuElement& foundItem, int32& index);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/// Remove ////////////////////////////////////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	int32 RemoveEventItem(FString itemKey);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	int32 RemoveDropboxItem(FString itemKey);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	int32 RemoveSuddenEventItem(FString itemKey);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	int32 RemovePlayerGroupItem(FString itemKey);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	int32 RemoveVariableItem(FString itemKey);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	int32 RemoveSystemMenuElementItem(FString itemKey);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/// Duplicate ////////////////////////////////////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool DuplicateEventItem(FString itemKey, FString postFix);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool DuplicateDropboxItem(FString itemKey, FString postFix);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool DuplicateSuddenEventItem(FString itemKey, FString postFix);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool DuplicatePlayerGroupItem(FString itemKey, FString postFix);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool DuplicateVariableItem(FString itemKey, FString postFix);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool DuplicateSystemMenuElementItem(FString itemKey, FString postFix);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/// Add ////////////////////////////////////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool AddEventItem(FRtScenarioEvent newItem);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool AddDropboxItem(FRtDropbox newItem);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool AddSuddenEventItem(FRtScenarioSuddenEvent newItem);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool AddPlayerGroupItem(FRtScenarioPlayerGroup newItem);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool AddVariableItem(FRtVariable newItem);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool AddSystemMenuElementItem(FRtUIMenuElement newItem);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/// Insert ////////////////////////////////////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool InsertEventItem(FRtScenarioEvent newItem, int32 insertIndex);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool InsertDropboxItem(FRtDropbox newItem, int32 insertIndex);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool InsertSuddenEventItem(FRtScenarioSuddenEvent newItem, int32 insertIndex);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool InsertPlayerGroupItem(FRtScenarioPlayerGroup newItem, int32 insertIndex);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool InsertVariableItem(FRtVariable newItem, int32 insertIndex);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool InsertSystemMenuElementItem(FRtUIMenuElement newItem, int32 insertIndex);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/// Set ////////////////////////////////////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool SetEventItem(FString itemKey, FRtScenarioEvent newItem);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool SetDropboxItem(FString itemKey, FRtDropbox newItem);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool SetSuddenEventItem(FString itemKey, FRtScenarioSuddenEvent newItem);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool SetPlayerGroupItem(FString itemKey, FRtScenarioPlayerGroup newItem);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool SetVariableItem(FString itemKey, FRtVariable newItem);

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	bool SetSystemMenuElementItem(FString itemKey, FRtUIMenuElement newItem);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	void ChangeSystemMenuScreenPos();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	// FRtScenarioData 객체를 Json 파일로 저장한다.
	UFUNCTION(BlueprintCallable, Category = "ARtScenarioModel")
	static bool WriteSctDataToJsonFile(FString outputFilePath, FRtScenarioData sctData);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ARtScenarioModel")
	FRtScenarioData SctData;

	UPROPERTY()
	TMap<FString, FRtDropboxCommand> DropboxCommandMap; // 키 : 드롭박스ID, 값 : 타겟 드롭박스 커맨드

	UPROPERTY()
	FRtRadioDropboxCommand RadioDropboxCommand; // 무전기 드롭박스 커맨드

	UPROPERTY()
	TMap<FString, int32> InitVarialbleMap; // 키 : 변수명 , 값 : int

	UPROPERTY()
	TMap<FString, int32> VarialbleMap; // 키 : 변수명 , 값 : int

	UPROPERTY()
	TMap<FString, int32> OldVarialbleMap; // 키 : 변수명 , 값 : int

	UPROPERTY()
	TMap<FString, FRtScenarioEventCommand> EventCommandMap; // 키 : 이벤트ID, 값 : 실행 이벤트 커맨드

	UPROPERTY()
	TArray<FString> InitEventIDLoopList; // 값 : 이벤트 ID ( 루프를 돌 때 실행할 Event ID 리스트의 초기값 )

	UPROPERTY()
	TArray<FString> EventIDLoopList; // 값 : 이벤트 ID ( 루프를 돌 때 실행할 Event ID 리스트 )

	UPROPERTY()
	ERtSctGameState GameState; // 게임 상태 변수.

	UPROPERTY()
	TMap<FString, FRtIDList> TargetDropboxMap; // 키 : 타겟 ObjectID, 값 : 열을 DropboxID

	TMap<FString, TPair<const FRtEventCommand*, FString>> ServerEventCommandMap; // 키 : 이벤트 ID, 값 : FRtEventCommand의 포인터, 닫을 DropboxID

	UPROPERTY()
	TMap<FString, FRtScenarioSuddenEventCommand> SuddenEventCommandMap; // 키 : 돌발 이벤트 ID, 값 : 실행할 돌발 이벤트 리스트

	UPROPERTY()
	TMap<FString, FRtEventCommand> ReplayEventCommandMap; // 리플레이를 위한 모든 이벤트 맵. 키 : 이벤트 ID, 값 : 실행할 이벤트 리스트

	UPROPERTY()
	TArray<FString> TargetInteractObjects; // 타겟에 B키를 눌러 상호작용하는 오브젝트 목록

	void SetEventCommand(FRtEventCommand& cmdData, const FRtEvent& eventData);
	void SetDropboxElemantCommands(FRtDropboxBaseCommand& cmdData, const FRtDropboxBase& dropboxData);
	void AddServerEventCommandMap(const FRtDropboxBaseCommand& dropboxCommand);
	const FRtEventCommand* FindEventCommand(const FString& eventID);

	void SetVariableValue(FString key, int32 value);

public:
	void Show_DropboxCommands();
	void Show_LoopEventList();
	void Show_ServerEventCommands();
	void Show_SuddenEventCommands();
};
