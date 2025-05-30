#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

// 시뮬레이션 내에서 사용
UENUM(BlueprintType)
enum class ERtCombustionState : uint8
{
	ENone = 0 UMETA(DisplayName = "None"),

	ECombustionStart UMETA(DisplayName = "연소 시작 신호"),
	
	// 연소가 시작 되어 타들어가는 시작 단계 ( 기본 모델 적용 )
	ECombustionProgressStap1 UMETA(DisplayName = "연소 진행 1 단계"), 

	// 연소가 많이 진행 되어 물체에 변형이 온 단계 ( 연소중 모델 적용 )
	ECombustionProgressStap2 UMETA(DisplayName = "연소 진행 2 단계"),

	ECombustionEnd UMETA(DisplayName = "연소 종료 신호"),
};

// condition_type
UENUM(BlueprintType)
enum class ERtSctCondition : uint8 // 이벤트의 주제를 설정한다.
{
	ENone = 0 UMETA(DisplayName = "None"),
	 
	EState UMETA(DisplayName = "상태"), 
	EPosition UMETA(DisplayName = "위치"),
	ETime UMETA(DisplayName = "시간"),
	EDistance UMETA(DisplayName = "거리"),
};

// target1_type : ETime_Timer, EState_Region, EDistance_Distance 외 전부 사용. 
// target2_type : EState_Region, EDistance_Distance 만 사용.
UENUM(BlueprintType)
enum class ERtSctTarget : uint8
{	
	ENone = 0 UMETA(DisplayName = "None"),
	EOwner UMETA(DisplayName = "Owner"),

	// Npc, 대역( 10(Npc), 11~19(Npc세부) )
	EState_Npc = 10 UMETA(DisplayName = "상태 : NPC"), 

		EState_Npc_Ally UMETA(DisplayName = "상태 : 아군 NPC"), 
		EState_Npc_Enemy UMETA(DisplayName = "상태 : 적군 NPC"),

		EState_Npc_NeedRescue UMETA(DisplayName = "상태 : 일반(요구조자) NPC"),
		EState_Npc_Hostage UMETA(DisplayName = "상태 : 일반(인질) NPC"),

		EState_Npc_Strange UMETA(DisplayName = "상태 : 일반(거수자) NPC"),

	// pc, 대역(20~29)
	EState_Pc = 20 UMETA(DisplayName = "상태 : PC"), 

	// 구조물, 대역 30~39 
	EState_Structure = 30 UMETA(DisplayName = "상태 : 구조물"), 

	// 환경, 대역 40~49  
	EState_Environment = 40  UMETA(DisplayName = "상태 : 환경"), 

	// 영역, 대역 50~59 
	EState_Region = 50 UMETA(DisplayName = "상태 : 영역"), 

	// 이벤트, 대역 60~69 
	EState_Event = 60 UMETA(DisplayName = "상태 : 이벤트"), 

	EPos_Npc = 70 UMETA(DisplayName = "위치 : NPC"), 
	EPos_Pc UMETA(DisplayName = "위치 : PC"),

	ETime_RemainingTime = 80 UMETA(DisplayName = "시간 : 남은 시간"),
	ETime_ElapsedTime UMETA(DisplayName = "시간 : 경과 시간"),

	// 타이머. (이벤트 리스트용)
	ETime_Timer UMETA(DisplayName = "시간 : 타이머"),

	// 거리. (이벤트 리스트용)
	EDistance_Distance = 90 UMETA(DisplayName = "거리 : 거리"),
		EDistance_Npc UMETA(DisplayName = "거리 : NPC"),
		EDistance_Pc UMETA(DisplayName = "거리 : PC"),
		EDistance_Struct UMETA(DisplayName = "거리 : 구조물"),
		EDistance_Region UMETA(DisplayName = "거리 : 영역"),

	// 수치. 
	ENumber_Count = 100 UMETA(DisplayName = "수치 : 카운트"),
};

// target1_state
UENUM(BlueprintType)
enum class ERtSctTargetState: uint8
{
	ENone = 0 UMETA(DisplayName = "None"),
	
	// [ 컨디션 : 상태 ]
	// 
	// 군대 NPC
	EState_Tar1MiliNpc_Appear = 10 UMETA(DisplayName = "State: Target1(Mili NPC) 등장"),
	EState_Tar1MiliNpc_Death UMETA(DisplayName = "State: Target1(Mili NPC) 사망"),
	EState_Tar1MiliNpc_Survival UMETA(DisplayName = "State: Target1(Mili NPC) 생존"),
	EState_Tar1MiliNpc_Retreat UMETA(DisplayName = "State: Target1(Mili NPC) 퇴각"),

	// 요구조자, 인질 NPC
	EState_Tar1RescueNpc_Death = 20 UMETA(DisplayName = "State: Target1(Rescue NPC) 사망"),
	EState_Tar1RescueNpc_Survival UMETA(DisplayName = "State: Target1(Rescue NPC) 생존"),
	EState_Tar1RescueNpc_Rescue UMETA(DisplayName = "State: Target1(Rescue NPC) 구출"),
	EState_Tar1RescueNpc_Equip UMETA(DisplayName = "State: Target1(Rescue NPC) 장착"),
	EState_Tar1RescueNpc_IsInViewPort UMETA(DisplayName = "State: Target1(Rescue NPC) 보이다"),

	// 거수자 NPC
	EState_Tar1StrangeNpc_Appear = 30 UMETA(DisplayName = "State: Target1(Strange NPC) 등장"),
	EState_Tar1StrangeNpc_Death UMETA(DisplayName = "State: Target1(Strange NPC) 사망"),
	EState_Tar1StrangeNpc_Suppress UMETA(DisplayName = "State: Target1(Strange NPC) 제압"),
	EState_Tar1StrangeNpc_Breakaway UMETA(DisplayName = "State: Target1(Strange NPC) 이탈"),
	EState_Tar1StrangeNpc_Aimed UMETA(DisplayName = "State : Target1(Strange NPC) 조준되다"),

	// PC
	EState_Tar1Pc_Death = 40 UMETA(DisplayName = "State: Target1(PC) 사망"),
	EState_Tar1Pc_Survival  UMETA(DisplayName = "State: Target1(PC) 생존"),
	EState_Tar1Pc_Wound UMETA(DisplayName = "State: Target1(PC) 부상"),
	EState_Tar1Pc_Degree UMETA(DisplayName = "State: Target1(PC) 각도 범위"),

	// 구조물
	EState_Tar1Struct_Normal = 50 UMETA(DisplayName = "State: Target1(Struct) 일반"),
	EState_Tar1Struct_Destroy UMETA(DisplayName = "State: Target1(Struct) 파괴"),
	EState_Tar1Struct_Combustion UMETA(DisplayName = "State: Target1(Struct) 연소"),
	EState_Tar1Struct_Operation UMETA(DisplayName = "State: Target1(Struct) 작동"),
	EState_Tar1Struct_StopOperation UMETA(DisplayName = "State: Target1(Struct) 작동 정지"), 
	EState_Tar1Struct_PutOutFire UMETA(DisplayName = "State: Target1(Struct) 소화"),
	EState_Tar1Struct_Shoot UMETA(DisplayName = "State: Target1(Struct) 발사"),
	EState_Tar1Struct_IsInViewPort UMETA(DisplayName = "State: Target1(Struct) 보이다"),
	EState_Tar1Struct_Aimed UMETA(DisplayName = "State: Target1(Struct) 조준되다"),

	// 환경
	EState_Tar1Envion_Fire = 60 UMETA(DisplayName = "State: Target1(Envion) 화재"), 
	EState_Tar1Envion_Pollution UMETA(DisplayName = "State: Target1(Envion) 오염"), 
	EState_Tar1Envion_Normal UMETA(DisplayName = "State: Target1(Envion) 일반"), 

	// 영역
	EState_Tar1Region_Fire = 70 UMETA(DisplayName = "State: Target1(Region) 화재"),
	EState_Tar1Region_Pollution UMETA(DisplayName = "State: Target1(Region) 오염"),
	EState_Tar1Region_Normal UMETA(DisplayName = "State: Target1(Region) 일반"),
	EState_Tar1Region_FlashOver UMETA(DisplayName = "State: Target1(Region) 플래쉬 오버"),
	EState_Tar1Region_BackDraft UMETA(DisplayName = "State: Target1(Region) 백 드래프트"),

	// 이벤트
	EState_Tar1Event_Start = 80 UMETA(DisplayName = "State: Target1(Event) Start"),
	EState_Tar1Event_End UMETA(DisplayName = "State: Target1(Event) End"),

	// [ 컨디션 : 위치 ]
	
	EPosition_PlayerIn = 90 UMETA(DisplayName = "Position: Player(Pc/Npc) 진입"),
	EPosition_PlayerOut UMETA(DisplayName = "Position: Player(Pc/Npc) 이탈"),

	// [ 컨디션 : 시간 ]

	ETime_Less = 100  UMETA(DisplayName = "Time: 이하"),
	ETime_Over UMETA(DisplayName = "Time: 초과"),

	// [ 컨디션 : 거리 ]

	EDistance_Less = 110 UMETA(DisplayName = "Distance: 이하"),
	EDistance_Over UMETA(DisplayName = "Distance: 초과"),

	// [[ 이벤트 리스트 전용 ]]
	
	// [ 컨디션 : 상태 ]

	//// 거수자 NPC
	//EEvent_State_Tar1StrangeNpc_Appear = 120 UMETA(DisplayName = "Event State : Target1(Strange NPC) 등장"),
	//EEvent_State_Tar1StrangeNpc_Death UMETA(DisplayName = "Event State : Target1(Strange NPC) 사망"),
	//EEvent_State_Tar1StrangeNpc_Suppress UMETA(DisplayName = "Event State : Target1(Strange NPC) 제압"),
	//EEvent_State_Tar1StrangeNpc_Breakaway UMETA(DisplayName = "Event State : Target1(Strange NPC) 이탈"),
	//EEvent_State_Tar1StrangeNpc_Aim UMETA(DisplayName = "Event State : Target1(Strange NPC) 조준"),

	// 이벤트
	EEvent_State_Tar1Event_BeforeOccur = 130 UMETA(DisplayName = "Event State : Target1(Event) 발생전"),
	EEvent_State_Tar1Event_Occurring UMETA(DisplayName = "Event State : Target1(Event) 발생중"),
	EEvent_State_Tar1Event_EndOccur UMETA(DisplayName = "Event State : Target1(Event) 발생종료"),

	// [ 컨디션 : 시간 ]

	// 타이머
	EEvent_Time_Tar1Timer_Operation = 140 UMETA(DisplayName = "Event Time : Target1(Timer) 작동 중"),
	EEvent_Time_Tar1Timer_EndOperation UMETA(DisplayName = "Event Time : Target1(Timer) 작동 종료"),

};

// process_type
UENUM(BlueprintType)
enum class ERtSctEventProcess : uint8
{
	ENone = 0 UMETA(DisplayName = "None"),

	// 오우너 객체가 있는 경우. 오우너 객체에 따라 데이터의 타입이 결정됨.
	EOwner UMETA(DisplayName = "Owner"), 

	// 환경 타입 ( 오우너 객체와 상관 없음 )
	EEnviron UMETA(DisplayName = "환경"), 

	// 타이머 타입 ( 오우너 객체와 상관 없음 )
	ETimer UMETA(DisplayName = "타이머"), 

	// 이벤트 타입 ( 오우너 객체와 상관 없음 )
	EEvent UMETA(DisplayName = "이벤트"), 

	// 기능 타입 ( 오우너 객체와 연관 (오우너 객체가 기능 컴포넌트를 가진 경우) )
	EFunction UMETA(DisplayName = "기능"), 
};

// process_category
UENUM(BlueprintType)
enum class ERtSctEventProcessCategory : uint8
{
	ENone = 0 UMETA(DisplayName = "None"),

	// 오우너가 NPC
	EOwnerNpc_Arrange = 10  UMETA(DisplayName = "Owner(Npc) : 배치"), 
	EOwnerNpc_State UMETA(DisplayName = "Owner(Npc) : 상태"), 
	EOwnerNpc_Move UMETA(DisplayName = "Owner(Npc) : 이동"), 
	EOwnerNpc_Action UMETA(DisplayName = "Owner(Npc) : 액션"), 
	EOwnerNpc_Speech UMETA(DisplayName = "Owner(Npc) : 대사"),
	EOwnerNpc_Equip UMETA(DisplayName = "Owner(Npc) : 장착"),

	// 오우너가 구조물
	EOwnerStruct_State = 20 UMETA(DisplayName = "Owner(Struct) : 상태"),
	EOwnerStruct_Operation UMETA(DisplayName = "Owner(Struct) : 작동"),

	// 오우너가 영역
	EOwnerRegion_State = 30 UMETA(DisplayName = "Owner(Region) : 상태"),

	// 오우너가 환경
	EOwnerEnviron_State = 40 UMETA(DisplayName = "Owner(Environment) : 상태"), 

	// 오우너가 타이머
	EOwnerTimer_State = 50 UMETA(DisplayName = "Owner(Timer) : 상태"),

	// 오우너가 이벤트
	EOwnerEvent_State = 60 UMETA(DisplayName = "Owner(Event) : 상태"),
};

// process_value
UENUM(BlueprintType)
enum class ERtSctEventProcessCategorySub : uint8
{
	ENone = 0 UMETA(DisplayName = "None"),

	// 오우너가 NPC이고 배치 카테고리
	EOwnerNpc_Arrange_Appear = 10 UMETA(DisplayName = "Owner(Npc) : 배치 - 등장"),
	EOwnerNpc_Arrange_Disappear UMETA(DisplayName = "Owner(Npc) : 배치 - 퇴장"),

	// 오우너가 NPC이고 상태 카테고리
	EOwnerNpc_State_Death = 20 UMETA(DisplayName = "Owner(Npc) : State - 사망"),
	EOwnerNpc_State_Wound  UMETA(DisplayName = "Owner(Npc) : State - 부상"),
	EOwnerNpc_State_Recovery UMETA(DisplayName = "Owner(Npc) : State - 회복"),
	EOwnerNpc_State_Suppress UMETA(DisplayName = "Owner(Npc) : State - 제압"),
		
	// 오우너가 NPC이고 이동 카테고리
	EOwnerNpc_Move_ToPosition = 30 UMETA(DisplayName = "Owner(Npc) : Move - 위치 이동"),
	EOwnerNpc_Move_ToPlayer UMETA(DisplayName = "Owner(Npc) : Move - 플레이어 따라 이동"),
	EOwnerNpc_Move_ToNpc UMETA(DisplayName = "Owner(Npc) : Move - NPC 따라 이동"),
	EOwnerNpc_Move_ToObject UMETA(DisplayName = "Owner(Npc) : Move - 특정 객체로 이동"),

	// 오우너가 NPC이고 액션 카테고리
	EOwnerNpc_Action_OnePattern = 40 UMETA(DisplayName = "Owner(Npc) : Action - 특정 패턴"),
	EOwnerNpc_Action_RandomPattern UMETA(DisplayName = "Owner(Npc) : Action - 랜덤 패턴"),
	
	// 오우너가 NPC이고 대사 카테고리
	EOwnerNpc_Speech_Voice = 50 UMETA(DisplayName = "Owner(Npc) : Speech - 음성"),
	EOwnerNpc_Speech_Text UMETA(DisplayName = "Owner(Npc) : Speech - 텍스트"),

	// 오우너가 구조물이고 상태 카테고리
	EOwnerStruct_State_Combustion = 60 UMETA(DisplayName = "Owner(Struct) : State - 연소"),
	EOwnerStruct_State_PutOutFire UMETA(DisplayName = "Owner(Struct) : State - 소화"),
	EOwnerStruct_State_Destroy UMETA(DisplayName = "Owner(Struct) : State - 파괴"),

	// 오우너가 구조물이고 작동 카테고리
	EOwnerStruct_Operation_Start = 70 UMETA(DisplayName = "Owner(Struct) : 작동 시작"),
	EOwnerStruct_Operation_End UMETA(DisplayName = "Owner(Struct) : 작동 종료"),

	// 오우너가 영역이고 상태 카테고리
	EOwnerRegion_State_Decontamination = 80 UMETA(DisplayName = "Owner(Region) : State - 오염 해제"),
	EOwnerRegion_State_PutOutFire UMETA(DisplayName = "Owner(Region) : State - 소화"),
	EOwnerRegion_State_FlashOver UMETA(DisplayName = "Owner(Region) : State - 플래쉬 오버"),
	EOwnerRegion_State_BackDraft UMETA(DisplayName = "Owner(Region) : State - 백 드래프트"),
	
	// 환경 상태 카테고리
	EEnviron_State_Decontamination = 90 UMETA(DisplayName = "Environ : State - 오염 해제"),
	EEnviron_State_PutOutFire UMETA(DisplayName = "Environ : State - 소화"),

	// 타이머 상태 카테고리
	ETimer_State_Create = 100 UMETA(DisplayName = "Timer : State - 생성"),
	ETimer_State_Start UMETA(DisplayName = "Timer : State - 시작"),
	ETimer_State_Stop UMETA(DisplayName = "Timer : State - 중단"),
	ETimer_State_ReStart UMETA(DisplayName = "Timer : State - 재시작"),

	// 이벤트 상태 카테고리
	EEvent_State_Start = 110 UMETA(DisplayName = "Event : State - 시작"),
	EEvent_State_End UMETA(DisplayName = "Event : State - 종료"),
};

// 프로세스 데이터 타입
UENUM(BlueprintType)
enum class ERtSctEventProcessData : uint8
{
	ENone = 0 UMETA(DisplayName = "None"),

	// UI 데이터
	EUI UMETA(DisplayName = "UI"), 
	// 이동 데이터
	EMove UMETA(DisplayName = "Move"), 
	// 패턴
	EPattern UMETA(DisplayName = "Pattern"), 
	// 스크립트
	EScript UMETA(DisplayName = "Script") 
};

// 프로세스 데이터 UI 타입
UENUM(BlueprintType)
enum class ERtSctEventUIType : uint8
{
	ENone = 0 UMETA(DisplayName = "None"),

	// 3D 원형 UI ( 메뉴가 3차원상에 떠있는 방식 )
	E3DCircle UMETA(DisplayName = "Circle"),

	// 3D 직사각 UI ( 메뉴가 3차원상에 떠있는 방식 )
	E3DRectangle UMETA(DisplayName = "Rectangle"),

	// 스크린 원형 UI ( 메뉴가 스크린에 붙어 있는 방식 )
	EScreenCircle UMETA(DisplayName = "ScreenCircle"),

	// 스크린 직사각 UI ( 메뉴가 스크린에 붙어 있는 방식 )
	EScreenRectangle UMETA(DisplayName = "ScreenRectangle")
};

// 각종 sct 평가 상태 ( 현재는 소방용 상태만 들어감 )
UENUM(BlueprintType)
enum class ERtSctEvaluationState : uint8
{
	ENone = 0 UMETA(DisplayName = "None"),

	// 열화상 카메라 On
	EThermalImagingCameraOn UMETA(DisplayName = "열화상 카메라 On"),
	
	// 화재 진압 완료
	EEFireFightingComplete UMETA(DisplayName = "화재 진압 완료"),
 
	// 구조 완료
	ERescueComplete UMETA(DisplayName = "구조 완료"),

	// 화재 현장 진입 보고
	EFireSiteEntryReport UMETA(DisplayName = "화재 현장 진입 보고"),

	// 발화점 발견 보고
	EFoundIgnitionPointReport UMETA(DisplayName = "발화점 발견 보고"),

	// 화재 진압 시작 보고	
	EStartFireFightingReport UMETA(DisplayName = "화재 진압 시작 보고"),

	// 요구조자 발견 보고
	EFoundRescueTargetReport UMETA(DisplayName = "요구조자 발견 보고"),

	// 요구조자 상태 보고
	ERescueTargetStateReport UMETA(DisplayName = "요구조자 상태 보고"),
		 
	// 요구조자 구조 시작 보고
	ERescueStartReport UMETA(DisplayName = "요구조자 구조 시작 보고"),

	// 요구조자 구조 완료 보고
	ERescueCompleteReport UMETA(DisplayName = "요구조자 구조 완료 보고"),

	// 화재 진압 완료 보고
	EFireSuppressionCompleteReport UMETA(DisplayName = "화재 진압 완료 보고"),

	// 총 수량.
//	EEvaluationStateNum,
};


UENUM(BlueprintType)
enum class ERtPlayerType : uint8 
{
	ENone = 0 UMETA(DisplayName = "None"),

	EPC UMETA(DisplayName = "PC"),
	ENPC UMETA(DisplayName = "NPC"),
};

UENUM(BlueprintType)
enum class ERtEventHandleType : uint8
{
	ENone = 0 UMETA(DisplayName = "None"),

	EServer UMETA(DisplayName = "Server"), // 서버에서 구동되는 타입.
	EClient UMETA(DisplayName = "Client"), // 클라에서 구동되는 타입. 
	EServerClient UMETA(DisplayName = "Server & Client"), // 서버, 클로 모두에서 구동되는 타입.
	EListenServer UMETA(DisplayName = "Listen Server"), // 리슨 서버 타입.
};

