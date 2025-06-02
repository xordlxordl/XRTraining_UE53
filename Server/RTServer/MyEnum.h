#pragma once
#include "json_struct.h"

// 시뮬레이션 내에서 사용
enum class ERtCombustionState
{
	ENone = 0,

	ECombustionStart,

	// 연소가 시작 되어 타들어가는 시작 단계 ( 기본 모델 적용 )
	ECombustionProgressStap1,

	// 연소가 많이 진행 되어 물체에 변형이 온 단계 ( 연소중 모델 적용 )
	ECombustionProgressStap2,

	ECombustionEnd,
};

// condition_type
/*
enum class ERtSctCondition // 이벤트의 주제를 설정한다.
{
	ENone = 0,

	EState,
	EPosition,
	ETime,
	EDistance,
};
*/
JS_ENUM( ERtSctCondition, ENone, EState, EPosition, ETime, EDistance )
JS_ENUM_DECLARE_STRING_PARSER( ERtSctCondition );


// target1_type : ETime_Timer, EState_Region, EDistance_Distance 외 전부 사용. 
// target2_type : EState_Region, EDistance_Distance 만 사용.

enum class ERtSctTarget
{
	ENone = 0,
	EOwner,

	// Npc, 대역( 10(Npc), 11~19(Npc세부) )
	EState_Npc = 10,

	EState_Npc_Ally,
	EState_Npc_Enemy,

	EState_Npc_NeedRescue,
	EState_Npc_Hostage,

	EState_Npc_Strange,

	// pc, 대역(20~29)
	EState_Pc = 20,

	// 구조물, 대역 30~39 
	EState_Structure = 30,

	// 환경, 대역 40~49  
	EState_Environment = 40,

	// 영역, 대역 50~59 
	EState_Region = 50,

	// 이벤트, 대역 60~69 
	EState_Event = 60,

	EPos_Npc = 70,
	EPos_Pc,

	ETime_RemainingTime = 80,
	ETime_ElapsedTime,

	// 타이머. (이벤트 리스트용)
	ETime_Timer,

	// 거리. (이벤트 리스트용)
	EDistance_Distance = 90,
	EDistance_Npc,
	EDistance_Pc,
	EDistance_Struct,
	EDistance_Region,

	// 수치. 
	ENumber_Count = 100,
};

// target1_state
enum class ERtSctTargetState
{
	ENone = 0,

	// [ 컨디션 : 상태 ]
	// 
	// 군대 NPC
	EState_Tar1MiliNpc_Appear = 10,
	EState_Tar1MiliNpc_Death,
	EState_Tar1MiliNpc_Survival,
	EState_Tar1MiliNpc_Retreat,

	// 요구조자, 인질 NPC
	EState_Tar1RescueNpc_Death = 20,
	EState_Tar1RescueNpc_Survival,
	EState_Tar1RescueNpc_Rescue,
	EState_Tar1RescueNpc_Equip,
	EState_Tar1RescueNpc_IsInViewPort,

	// 거수자 NPC
	EState_Tar1StrangeNpc_Appear = 30,
	EState_Tar1StrangeNpc_Death,
	EState_Tar1StrangeNpc_Suppress,
	EState_Tar1StrangeNpc_Breakaway,
	EState_Tar1StrangeNpc_Aimed,

	// PC
	EState_Tar1Pc_Death = 40,
	EState_Tar1Pc_Survival,
	EState_Tar1Pc_Wound,
	EState_Tar1Pc_Degree,

	// 구조물
	EState_Tar1Struct_Normal = 50,
	EState_Tar1Struct_Destroy,
	EState_Tar1Struct_Combustion,
	EState_Tar1Struct_Operation,
	EState_Tar1Struct_StopOperation,
	EState_Tar1Struct_PutOutFire,
	EState_Tar1Struct_Shoot,
	EState_Tar1Struct_IsInViewPort,
	EState_Tar1Struct_Aimed,

	// 환경
	EState_Tar1Envion_Fire = 60,
	EState_Tar1Envion_Pollution,
	EState_Tar1Envion_Normal,

	// 영역
	EState_Tar1Region_Fire = 70,
	EState_Tar1Region_Pollution,
	EState_Tar1Region_Normal,
	EState_Tar1Region_FlashOver,
	EState_Tar1Region_BackDraft,

	// 이벤트
	EState_Tar1Event_Start = 80,
	EState_Tar1Event_End,

	// [ 컨디션 : 위치 ]

	EPosition_PlayerIn = 90,
	EPosition_PlayerOut,

	// [ 컨디션 : 시간 ]

	ETime_Less = 100,
	ETime_Over,

	// [ 컨디션 : 거리 ]

	EDistance_Less = 110,
	EDistance_Over,

	// [[ 이벤트 리스트 전용 ]]

	// [ 컨디션 : 상태 ]

	//// 거수자 NPC
	//EEvent_State_Tar1StrangeNpc_Appear = 120 UMETA(DisplayName = "Event State : Target1(Strange NPC) 등장"),
	//EEvent_State_Tar1StrangeNpc_Death UMETA(DisplayName = "Event State : Target1(Strange NPC) 사망"),
	//EEvent_State_Tar1StrangeNpc_Suppress UMETA(DisplayName = "Event State : Target1(Strange NPC) 제압"),
	//EEvent_State_Tar1StrangeNpc_Breakaway UMETA(DisplayName = "Event State : Target1(Strange NPC) 이탈"),
	//EEvent_State_Tar1StrangeNpc_Aim UMETA(DisplayName = "Event State : Target1(Strange NPC) 조준"),

	// 이벤트
	EEvent_State_Tar1Event_BeforeOccur = 130,
	EEvent_State_Tar1Event_Occurring,
	EEvent_State_Tar1Event_EndOccur,

	// [ 컨디션 : 시간 ]

	// 타이머
	EEvent_Time_Tar1Timer_Operation = 140,
	EEvent_Time_Tar1Timer_EndOperation,

};

// process_type

enum class ERtSctEventProcess
{
	ENone = 0,

	// 오우너 객체가 있는 경우. 오우너 객체에 따라 데이터의 타입이 결정됨.
	EOwner,

	// 환경 타입 ( 오우너 객체와 상관 없음 )
	EEnviron,

	// 타이머 타입 ( 오우너 객체와 상관 없음 )
	ETimer,

	// 이벤트 타입 ( 오우너 객체와 상관 없음 )
	EEvent,

	// 기능 타입 ( 오우너 객체와 연관 (오우너 객체가 기능 컴포넌트를 가진 경우) )
	EFunction,
};

// process_category

enum class ERtSctEventProcessCategory
{
	ENone = 0,

	// 오우너가 NPC
	EOwnerNpc_Arrange,
	EOwnerNpc_State,
	EOwnerNpc_Move,
	EOwnerNpc_Action,
	EOwnerNpc_Speech,
	EOwnerNpc_Equip,

	// 오우너가 구조물
	EOwnerStruct_State = 20,
	EOwnerStruct_Operation,

	// 오우너가 영역
	EOwnerRegion_State = 30,

	// 오우너가 환경
	EOwnerEnviron_State = 40,

	// 오우너가 타이머
	EOwnerTimer_State = 50,

	// 오우너가 이벤트
	EOwnerEvent_State = 60,
};

// process_value

enum class ERtSctEventProcessCategorySub
{
	ENone = 0,

	// 오우너가 NPC이고 배치 카테고리
	EOwnerNpc_Arrange_Appear = 10,
	EOwnerNpc_Arrange_Disappear,

	// 오우너가 NPC이고 상태 카테고리
	EOwnerNpc_State_Death = 20,
	EOwnerNpc_State_Wound,
	EOwnerNpc_State_Recovery,
	EOwnerNpc_State_Suppress,

	// 오우너가 NPC이고 이동 카테고리
	EOwnerNpc_Move_ToPosition = 30,
	EOwnerNpc_Move_ToPlayer,
	EOwnerNpc_Move_ToNpc,
	EOwnerNpc_Move_ToObject,

	// 오우너가 NPC이고 액션 카테고리
	EOwnerNpc_Action_OnePattern = 40,
	EOwnerNpc_Action_RandomPattern,

	// 오우너가 NPC이고 대사 카테고리
	EOwnerNpc_Speech_Voice = 50,
	EOwnerNpc_Speech_Text,

	// 오우너가 구조물이고 상태 카테고리
	EOwnerStruct_State_Combustion = 60,
	EOwnerStruct_State_PutOutFire,
	EOwnerStruct_State_Destroy,

	// 오우너가 구조물이고 작동 카테고리
	EOwnerStruct_Operation_Start = 70,
	EOwnerStruct_Operation_End,

	// 오우너가 영역이고 상태 카테고리
	EOwnerRegion_State_Decontamination = 80,
	EOwnerRegion_State_PutOutFire,
	EOwnerRegion_State_FlashOver,
	EOwnerRegion_State_BackDraft,

	// 환경 상태 카테고리
	EEnviron_State_Decontamination = 90,
	EEnviron_State_PutOutFire,

	// 타이머 상태 카테고리
	ETimer_State_Create = 100,
	ETimer_State_Start,
	ETimer_State_Stop,
	ETimer_State_ReStart,

	// 이벤트 상태 카테고리
	EEvent_State_Start = 110,
	EEvent_State_End,
};

// 프로세스 데이터 타입
enum class ERtSctEventProcessData
{
	ENone = 0,

	// UI 데이터
	EUI,
	// 이동 데이터
	EMove,
	// 패턴
	EPattern,
	// 스크립트
	EScript,
};

// 프로세스 데이터 UI 타입
enum class ERtSctEventProcessDataUI
{
	ENone = 0,

	// 원형 UI
	ECircle,
	// 직사각 UI
	ERectangle,
};

// 각종 sct 평가 상태 ( 현재는 소방용 상태만 들어감 )
enum class ERtSctEvaluationState
{
	ENone = 0,

	// 열화상 카메라 On
	EThermalImagingCameraOn,

	// 화재 진압 완료
	EEFireFightingComplete,

	// 구조 완료
	ERescueComplete,

	// 화재 현장 진입 보고
	EFireSiteEntryReport,

	// 발화점 발견 보고
	EFoundIgnitionPointReport,

	// 화재 진압 시작 보고	
	EStartFireFightingReport,

	// 요구조자 발견 보고
	EFoundRescueTargetReport,

	// 요구조자 상태 보고
	ERescueTargetStateReport,

	// 요구조자 구조 시작 보고
	ERescueStartReport,

	// 요구조자 구조 완료 보고
	ERescueCompleteReport,

	// 화재 진압 완료 보고
	EFireSuppressionCompleteReport,

	// 총 수량.
//	EEvaluationStateNum,
};

/*
enum class ERtPlayerType
{
	ENone = 0,

	EPC,
	ENPC,
};
*/
JS_ENUM( ERtPlayerType, ENone, EPC, ENPC )
JS_ENUM_DECLARE_STRING_PARSER( ERtPlayerType );

/*
enum class ERtEventHandleType
{
	ENone = 0,

	EServer, // 서버에서 구동되는 타입.
	EClient, // 클라에서 구동되는 타입. 
	EServerClient, // 서버, 클로 모두에서 구동되는 타입.
};
*/
JS_ENUM( ERtEventHandleType, ENone, EServer, EClient, EServerClient, EListenServer )
JS_ENUM_DECLARE_STRING_PARSER( ERtEventHandleType );

// TCT 각 컴포넌트의 모든 요소 타입 정보.
enum class ERtTctComponentEleType
{
	ENone = 0,

	// 충돌 컴포넌트 요소 타입.
	ECollisionCube = 1,
	ECollisionCapsule = 2,

	// 라이트 컴포넌트 요소 타입.
	ELightPoint = 3,
	ELightSpot = 4,

	// 이펙트 컴포넌트 요소 타입.
	EEffect = 5,

	// 기능 컴포넌트 요소 타입.
	EFunction = 6,

	// 인덱스.
	EIndexColStart = ECollisionCube,
	EIndexColEnd = ECollisionCapsule,

	EIndexLightStart = ELightPoint,
	EIndexLightEnd = ELightSpot
};

// TCT 각 컴포넌트의 모든 콤보 타입 정보.
enum class ERtComponentComboType
{
	ENone = 0,

	// [ 연소 컴포넌트 ]

	// 연소 여부.
	ECombuIsBurning = 1, //가연
	ECombuIsNotBurning = 2, //불연

	// 연소 종류.
	ECombuDecomposedCombustion = 3, //분해연소(유염)
	ECombuFlamingCombustion = 4, //불꽃연소(무염)

	// 연기 색.
	ECombuWhiteColor = 5, //흰색
	ECombuGrayColor = 6, //회색
	ECombuBlackColor = 7, //검정색

	// 연기 양.
	ECombuSmall = 8, //소량
	ECombuNormal = 9, //일반
	ECombuBig = 10, //대량

	// 재생 옵션.
	EPlayOptionOneTime = 11,
	EPlayOptionLoop = 12,

	EIndexCombuFlammStart = ECombuIsBurning,
	EIndexCombuFlammEnd = ECombuIsNotBurning,

	EIndexCombuKindStart = ECombuDecomposedCombustion,
	EIndexCombuKindEnd = ECombuFlamingCombustion,

	EIndexCombuSmokeColorStart = ECombuWhiteColor,
	EIndexCombuSmokeColorEnd = ECombuBlackColor,

	EIndexCombuSmokeSizeStart = ECombuSmall,
	EIndexCombuSmokeSizeEnd = ECombuBig,

	EIndexPlayOptionStart = EPlayOptionOneTime,
	EIndexPlayOptionEnd = EPlayOptionLoop,
};

// 장착 컴포넌트 소켓 콤보 타입.
enum class ERtEquipComSocketComboType
{
	ENone = 0,

	EEquip_MouthSocket = 1, // 소켓 이름 : "mouth_socket"
	EEquip_RightHand = 2 // 소켓 이름 : "r_hand_socket"
};

// TCT 타입 ID
enum class ERtTctType
{

	ENone = 0,

	ECharacter = 1,	// 캐릭터.(1)
	EWall = 2,	// 벽.(2)
	ETile = 3,	// 타일.(3)
	EProp = 4,	// 소품.(4)
	EEquipProp = 5,	// 장착 소픔.(5)
	ECar = 6,	// 차량.(6)
	EGun = 7,	// 총.(7)
	EFireNozzle = 8,	// 소방 관창.(8)

	// TCT 툴에서는 사용되지 않는다. CCT 및 시뮬레이터 전용.
	EPlayerSpawn = 101,	// 플레이어 스폰.
	EWayPoint = 102,	// 웨이 포인트.
	EArea = 103,	// 영역.
	EInvisibleWall = 104,  // 투명 벽.
	EMapBoundary = 105,  // 맵 경계 영역 표시용.
	ETriggerBox = 106,  // 트리거 박스. ( 스케일이 가능하다 )
};

// Spawn 타입 ID
enum class ERtMctSpawnType
{
	ENone = 0,
	EPC = 1,	// PC.
	ENPC = 2,	// NPC.
	EOther = 3	// 다른 객체들
};

// TCT 컴포넌트 ID
enum class ERtComponentType
{

	ENone = 0,

	EObject = 1,  // 기본 객체 컴포넌트 ( 공통 컴포넌트 ).
	ECollision = 2,  // 충돌 박스 컴포넌트.
	EMaterial = 3,  // 재질 컴포넌트.
	EAnimationViewer = 4,  // 애니메이션 컴포넌트. ( 뷰어형 컴포넌트 : 실 데이터가 저장되지 않는다. )
	EAudio = 5,  // 오디오 컴포넌트.
	ELight = 6,  // 라이트 컴포넌트.
	EEffect = 7,  // 이펙트 컴포넌트.
	EFunction = 8,  // 기능 컴포넌트.
	EGunFire = 9,  // 총 발사 컴포넌트. ( 우선 순위 밀림 )
	EWaterSpray = 10, // 물 분사 컴포넌트.
	ECombustion = 11, // 연소 컴포넌트.	
	EEquip = 12, // 장착 컴포넌트.	

	EInteract = 13, // 상호작용 컴포넌트. ( 우선 순위 밀림 )
	EInsideModel = 14, // 실내 모델 컴포넌트. ( 우선 순위 밀림 )
};

// TCT 컴포넌트 정보 ENUM ( 블루프린트에서는 쓸 수 없다 !! )
// 기본적으로 컴포넌트 타입은 ERtComponentType 을 사용하여 쓴다. 
// 기본적인 정보 개념을 쓸 때는 순수 enum ERtComponentTypeInfo를 이용하여 쓴다. 
// ( C++ 에서만 사용 )
enum ERtComponentTypeInfo 
{

	EInfoNone = 0,

	EInfoObject = 1 << 0,	// 기본 객체 컴포넌트 ( 공통 컴포넌트 ).
	EInfoCollision = 1 << 1,	// 충돌 박스 컴포넌트.
	EInfoMaterial = 1 << 2,	// 재질 컴포넌트.
	EInfoAnimationViewer = 1 << 3,	// 애니메이션 컴포넌트. ( 뷰어형 컴포넌트 : 실 데이터가 저장되지 않는다. )
	EInfoAudio = 1 << 4,	// 오디오 컴포넌트.
	EInfoLight = 1 << 5,	// 라이트 컴포넌트.
	EInfoEffect = 1 << 6,	// 이펙트 컴포넌트.
	EInfoFunction = 1 << 7,	// 기능 컴포넌트.
	EInfoGunFire = 1 << 8,	// 총 발사 컴포넌트. 
	EInfoWaterSpray = 1 << 9,	// 물 분사 컴포넌트.
	EInfoCombustion = 1 << 10,	// 연소 컴포넌트.	
	EEquip = 1 << 11,	// 장착 컴포넌트.

	EInfoInteract = 1 << 12,	// 상호작용 컴포넌트. ( 우선 순위 밀림 )
	EInfoInsideModel = 1 << 13,	// 실내 모델 컴포넌트. ( 우선 순위 밀림 )

	// 캐릭터 기본 컴포넌트.
	EInfo_ComMix_Character = EInfoObject | EInfoMaterial | EInfoCollision | EInfoAnimationViewer,

	// 벽 기본 컴포넌트.
	EInfo_ComMix_Wall = EInfoObject | EInfoMaterial | EInfoCollision | EInfoFunction | EInfoCombustion,

	// 타일 기본 컴포넌트.
	EInfo_ComMix_Tile = EInfoObject | EInfoMaterial | EInfoCollision | EInfoFunction | EInfoCombustion,

	// 프롭(소품) 기본 컴포넌트.
	EInfo_ComMix_Prop = EInfoObject | EInfoMaterial | EInfoCollision | EInfoFunction | EInfoCombustion,

	// 장착용 프롭(소품) 기본 컴포넌트.
	EInfo_ComMix_EquipProp = EInfoObject | EInfoMaterial | EInfoGunFire | EInfoEffect | EInfoAudio | EEquip | EInfoAnimationViewer,

	// 차량 기본 컴포넌트.
	EInfo_ComMix_Car = EInfoObject | EInfoMaterial | EInfoCollision | EInfoInteract | EInfoInsideModel | EInfoFunction | EInfoCombustion,

	// 총 기본 컴포넌트.
	EInfo_ComMix_Gun = EInfoObject | EInfoMaterial | EInfoGunFire | EInfoEffect | EInfoAudio | EEquip | EInfoAnimationViewer,

	// 관창 기본 컴포넌트.
	EInfo_ComMix_FireNozzle = EInfoObject | EInfoMaterial | EInfoWaterSpray | EInfoEffect | EInfoAudio | EEquip | EInfoAnimationViewer,

	// 임의 추가 가능 컴포넌트. ( 기본 컴포넌트 위에 임의로 추가가 가능한 컴포넌트 나열 )
	EInfo_ComMix_AddEnable = EInfoAudio | EInfoLight | EInfoEffect,
};

// 컴포넌트 내 콘트롤 ID. 
enum class ERtComponentUnits
{
	ENone = 0,

	ECollision_AddUnit,		// 충돌 컴포넌트 엘리먼트 추가 유닛.
	ECollision_AddUnit_AddBtn,	// 충돌 컴포넌트 엘리먼트 추가 버튼.

	ECollision_ElementUnit,		// 충돌 컴포넌트 엘리먼트 유닛.
};


enum class ERtCommandType
{
	ENone = 0,

	ECmd_Coll_AddEle,
};

// 파일 가져오기 대화상자용 ( 내부 데이터 가져오기 용 )
enum class ERtModelKind
{
	ENone = 0,

	EDefaultModel, // 객체 기본 모델

	// 이펙트 컴포넌트 용.
	EEffectComEffectModel,		// 이펙트 모델

	// 연소 컴포넌트 용.
	ECombustionBurningModel,	// 객체가 타는 모델
	ECombustionEndBurningModel, // 객체가 다 탄 모델

	// 기능 컴포넌트 용.
	EFunctionAnimationModel,	// 기능 애니메이션 모델
	EFunctionEffectModel,		// 기능 이펙트 모델

	EFunctionOperateModel,		// 기능 작동 모델
};

// 파일 가져오기 대화상자의 대화상자 타입.
enum class ERtImportDialogType
{
	ENone = 0,

	EImportModel,
	EImportAnimationSequence,
	EImportEffectModel,
};

enum class RtMCTAction
{
	None, // 최초
	Idle, // 기본 상태
	Move, // 움직임
	Selected, // 선택
};

enum class ERtSctGameState
{
	EGameState_None = 0,
	EGameState_GotoReadyPlace,
	EGameState_AllArriveReadyPlace,
	EGameState_Play,
	EGameState_Pause,
	EGameState_End,
};

//JS_ENUM( ERtPlayerType, ENone, EPC, ENPC )
//JS_ENUM_DECLARE_STRING_PARSER( ERtPlayerType );

JS_ENUM( ERtSctFunCmd,
	ENone,
	ReqAutoCloseDoor,
	ReqAutoOpenDoor,
	ReqManualCloseDoor,
	ReqManualOpenDoor,
	ReqChallenge,
	AckChallenge,
	ReqSurrenderOrder,
	AckSurrenderOrder,
	ReqBindUp,
	AckBindUp,
	ReqMoveOrder,
	AckMoveOrder,
	ReqStatusReport,
	ReqSetValue,
	AckSetValue,
	ReqCallEOD,
	IsGameState,
	SetGameState,
	EndEvent,
	IsArriveGameReadyPlace,
	AckGameState,
	DelayTime,
	AckSetVisible,
	IsVisible,
	MoveToWayPoint,
	IsInRadius,
	PlayAnim,
	PlayChainAnim,
	IsValue,
	IsOldValue,
	MoveToX,
	SetEnableGameReadyPlace,
	ShowMessage,
	ReqArriveGameReadyPlace,
	IsTargetAimed,
	Show3DMenu,
	IsKeyRelease,
	ShowScreenMenu,
	IsHitObject,
	ReqPlayAnim,
	AckPlayAnim,
	ReqPlayChainAnim,
	AckPlayChainAnim,
	IsAnim,
	ShowScreen,
	ReqGameEnd,
	SetVisible,
	PlayMovie,
	CheckCollision,
	IsOkThreateningFire,
	IsAnims )
JS_ENUM_DECLARE_STRING_PARSER( ERtSctFunCmd );


enum class ERtSctFunCmdParam
{
	ENone = 0,

	EStringVariable, // 문자열 변수명.
	EStringGameState, // 게임 상태 문자열.
	EStringID, // 객체 인스턴스 ID.
	EStringMenuSetID, // 메뉴 세트 ID
	EStringMessage, // 문자열 메시지.
	EStringAniName, // 애니메이션 이름.
	EStringAniNameList, // 애니메이션 이름 리스트.
	EStringVideoFile, // mp4, avi 비디오 파일명.
	EStringFormat, // 포맷 문자열.
	EStringFileName, // 파일 이름.
	EStringEventID, // 이벤트 ID.
	EStringKeyValue, // 입력 키 값.

	EIntValue, // Int 설정 값.
	EFloatValue, // Float 설정 값.

	EStringList, // 스트링 리스트.
};

// 프로세스 데이터 UI 타입
/*
enum class ERtSctEventUIType
{
	ENone = 0,
	// 3D 원형 UI ( 메뉴가 3차원상에 떠있는 방식 )
	E3DCircle,
	// 3D 직사각 UI ( 메뉴가 3차원상에 떠있는 방식 )
	E3DRectangle,
	// 스크린 원형 UI ( 메뉴가 스크린에 붙어 있는 방식 )
	EScreenCircle,
	// 스크린 직사각 UI ( 메뉴가 스크린에 붙어 있는 방식 )
	EScreenRectangle
};
*/
JS_ENUM( ERtSctEventUIType, ENone, E3DCircle, E3DRectangle, EScreenCircle, EScreenRectangle )
JS_ENUM_DECLARE_STRING_PARSER( ERtSctEventUIType );
