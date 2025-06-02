#pragma once
#include "json_struct.h"

// �ùķ��̼� ������ ���
enum class ERtCombustionState
{
	ENone = 0,

	ECombustionStart,

	// ���Ұ� ���� �Ǿ� Ÿ���� ���� �ܰ� ( �⺻ �� ���� )
	ECombustionProgressStap1,

	// ���Ұ� ���� ���� �Ǿ� ��ü�� ������ �� �ܰ� ( ������ �� ���� )
	ECombustionProgressStap2,

	ECombustionEnd,
};

// condition_type
/*
enum class ERtSctCondition // �̺�Ʈ�� ������ �����Ѵ�.
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


// target1_type : ETime_Timer, EState_Region, EDistance_Distance �� ���� ���. 
// target2_type : EState_Region, EDistance_Distance �� ���.

enum class ERtSctTarget
{
	ENone = 0,
	EOwner,

	// Npc, �뿪( 10(Npc), 11~19(Npc����) )
	EState_Npc = 10,

	EState_Npc_Ally,
	EState_Npc_Enemy,

	EState_Npc_NeedRescue,
	EState_Npc_Hostage,

	EState_Npc_Strange,

	// pc, �뿪(20~29)
	EState_Pc = 20,

	// ������, �뿪 30~39 
	EState_Structure = 30,

	// ȯ��, �뿪 40~49  
	EState_Environment = 40,

	// ����, �뿪 50~59 
	EState_Region = 50,

	// �̺�Ʈ, �뿪 60~69 
	EState_Event = 60,

	EPos_Npc = 70,
	EPos_Pc,

	ETime_RemainingTime = 80,
	ETime_ElapsedTime,

	// Ÿ�̸�. (�̺�Ʈ ����Ʈ��)
	ETime_Timer,

	// �Ÿ�. (�̺�Ʈ ����Ʈ��)
	EDistance_Distance = 90,
	EDistance_Npc,
	EDistance_Pc,
	EDistance_Struct,
	EDistance_Region,

	// ��ġ. 
	ENumber_Count = 100,
};

// target1_state
enum class ERtSctTargetState
{
	ENone = 0,

	// [ ����� : ���� ]
	// 
	// ���� NPC
	EState_Tar1MiliNpc_Appear = 10,
	EState_Tar1MiliNpc_Death,
	EState_Tar1MiliNpc_Survival,
	EState_Tar1MiliNpc_Retreat,

	// �䱸����, ���� NPC
	EState_Tar1RescueNpc_Death = 20,
	EState_Tar1RescueNpc_Survival,
	EState_Tar1RescueNpc_Rescue,
	EState_Tar1RescueNpc_Equip,
	EState_Tar1RescueNpc_IsInViewPort,

	// �ż��� NPC
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

	// ������
	EState_Tar1Struct_Normal = 50,
	EState_Tar1Struct_Destroy,
	EState_Tar1Struct_Combustion,
	EState_Tar1Struct_Operation,
	EState_Tar1Struct_StopOperation,
	EState_Tar1Struct_PutOutFire,
	EState_Tar1Struct_Shoot,
	EState_Tar1Struct_IsInViewPort,
	EState_Tar1Struct_Aimed,

	// ȯ��
	EState_Tar1Envion_Fire = 60,
	EState_Tar1Envion_Pollution,
	EState_Tar1Envion_Normal,

	// ����
	EState_Tar1Region_Fire = 70,
	EState_Tar1Region_Pollution,
	EState_Tar1Region_Normal,
	EState_Tar1Region_FlashOver,
	EState_Tar1Region_BackDraft,

	// �̺�Ʈ
	EState_Tar1Event_Start = 80,
	EState_Tar1Event_End,

	// [ ����� : ��ġ ]

	EPosition_PlayerIn = 90,
	EPosition_PlayerOut,

	// [ ����� : �ð� ]

	ETime_Less = 100,
	ETime_Over,

	// [ ����� : �Ÿ� ]

	EDistance_Less = 110,
	EDistance_Over,

	// [[ �̺�Ʈ ����Ʈ ���� ]]

	// [ ����� : ���� ]

	//// �ż��� NPC
	//EEvent_State_Tar1StrangeNpc_Appear = 120 UMETA(DisplayName = "Event State : Target1(Strange NPC) ����"),
	//EEvent_State_Tar1StrangeNpc_Death UMETA(DisplayName = "Event State : Target1(Strange NPC) ���"),
	//EEvent_State_Tar1StrangeNpc_Suppress UMETA(DisplayName = "Event State : Target1(Strange NPC) ����"),
	//EEvent_State_Tar1StrangeNpc_Breakaway UMETA(DisplayName = "Event State : Target1(Strange NPC) ��Ż"),
	//EEvent_State_Tar1StrangeNpc_Aim UMETA(DisplayName = "Event State : Target1(Strange NPC) ����"),

	// �̺�Ʈ
	EEvent_State_Tar1Event_BeforeOccur = 130,
	EEvent_State_Tar1Event_Occurring,
	EEvent_State_Tar1Event_EndOccur,

	// [ ����� : �ð� ]

	// Ÿ�̸�
	EEvent_Time_Tar1Timer_Operation = 140,
	EEvent_Time_Tar1Timer_EndOperation,

};

// process_type

enum class ERtSctEventProcess
{
	ENone = 0,

	// ����� ��ü�� �ִ� ���. ����� ��ü�� ���� �������� Ÿ���� ������.
	EOwner,

	// ȯ�� Ÿ�� ( ����� ��ü�� ��� ���� )
	EEnviron,

	// Ÿ�̸� Ÿ�� ( ����� ��ü�� ��� ���� )
	ETimer,

	// �̺�Ʈ Ÿ�� ( ����� ��ü�� ��� ���� )
	EEvent,

	// ��� Ÿ�� ( ����� ��ü�� ���� (����� ��ü�� ��� ������Ʈ�� ���� ���) )
	EFunction,
};

// process_category

enum class ERtSctEventProcessCategory
{
	ENone = 0,

	// ����ʰ� NPC
	EOwnerNpc_Arrange,
	EOwnerNpc_State,
	EOwnerNpc_Move,
	EOwnerNpc_Action,
	EOwnerNpc_Speech,
	EOwnerNpc_Equip,

	// ����ʰ� ������
	EOwnerStruct_State = 20,
	EOwnerStruct_Operation,

	// ����ʰ� ����
	EOwnerRegion_State = 30,

	// ����ʰ� ȯ��
	EOwnerEnviron_State = 40,

	// ����ʰ� Ÿ�̸�
	EOwnerTimer_State = 50,

	// ����ʰ� �̺�Ʈ
	EOwnerEvent_State = 60,
};

// process_value

enum class ERtSctEventProcessCategorySub
{
	ENone = 0,

	// ����ʰ� NPC�̰� ��ġ ī�װ�
	EOwnerNpc_Arrange_Appear = 10,
	EOwnerNpc_Arrange_Disappear,

	// ����ʰ� NPC�̰� ���� ī�װ�
	EOwnerNpc_State_Death = 20,
	EOwnerNpc_State_Wound,
	EOwnerNpc_State_Recovery,
	EOwnerNpc_State_Suppress,

	// ����ʰ� NPC�̰� �̵� ī�װ�
	EOwnerNpc_Move_ToPosition = 30,
	EOwnerNpc_Move_ToPlayer,
	EOwnerNpc_Move_ToNpc,
	EOwnerNpc_Move_ToObject,

	// ����ʰ� NPC�̰� �׼� ī�װ�
	EOwnerNpc_Action_OnePattern = 40,
	EOwnerNpc_Action_RandomPattern,

	// ����ʰ� NPC�̰� ��� ī�װ�
	EOwnerNpc_Speech_Voice = 50,
	EOwnerNpc_Speech_Text,

	// ����ʰ� �������̰� ���� ī�װ�
	EOwnerStruct_State_Combustion = 60,
	EOwnerStruct_State_PutOutFire,
	EOwnerStruct_State_Destroy,

	// ����ʰ� �������̰� �۵� ī�װ�
	EOwnerStruct_Operation_Start = 70,
	EOwnerStruct_Operation_End,

	// ����ʰ� �����̰� ���� ī�װ�
	EOwnerRegion_State_Decontamination = 80,
	EOwnerRegion_State_PutOutFire,
	EOwnerRegion_State_FlashOver,
	EOwnerRegion_State_BackDraft,

	// ȯ�� ���� ī�װ�
	EEnviron_State_Decontamination = 90,
	EEnviron_State_PutOutFire,

	// Ÿ�̸� ���� ī�װ�
	ETimer_State_Create = 100,
	ETimer_State_Start,
	ETimer_State_Stop,
	ETimer_State_ReStart,

	// �̺�Ʈ ���� ī�װ�
	EEvent_State_Start = 110,
	EEvent_State_End,
};

// ���μ��� ������ Ÿ��
enum class ERtSctEventProcessData
{
	ENone = 0,

	// UI ������
	EUI,
	// �̵� ������
	EMove,
	// ����
	EPattern,
	// ��ũ��Ʈ
	EScript,
};

// ���μ��� ������ UI Ÿ��
enum class ERtSctEventProcessDataUI
{
	ENone = 0,

	// ���� UI
	ECircle,
	// ���簢 UI
	ERectangle,
};

// ���� sct �� ���� ( ����� �ҹ�� ���¸� �� )
enum class ERtSctEvaluationState
{
	ENone = 0,

	// ��ȭ�� ī�޶� On
	EThermalImagingCameraOn,

	// ȭ�� ���� �Ϸ�
	EEFireFightingComplete,

	// ���� �Ϸ�
	ERescueComplete,

	// ȭ�� ���� ���� ����
	EFireSiteEntryReport,

	// ��ȭ�� �߰� ����
	EFoundIgnitionPointReport,

	// ȭ�� ���� ���� ����	
	EStartFireFightingReport,

	// �䱸���� �߰� ����
	EFoundRescueTargetReport,

	// �䱸���� ���� ����
	ERescueTargetStateReport,

	// �䱸���� ���� ���� ����
	ERescueStartReport,

	// �䱸���� ���� �Ϸ� ����
	ERescueCompleteReport,

	// ȭ�� ���� �Ϸ� ����
	EFireSuppressionCompleteReport,

	// �� ����.
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

	EServer, // �������� �����Ǵ� Ÿ��.
	EClient, // Ŭ�󿡼� �����Ǵ� Ÿ��. 
	EServerClient, // ����, Ŭ�� ��ο��� �����Ǵ� Ÿ��.
};
*/
JS_ENUM( ERtEventHandleType, ENone, EServer, EClient, EServerClient, EListenServer )
JS_ENUM_DECLARE_STRING_PARSER( ERtEventHandleType );

// TCT �� ������Ʈ�� ��� ��� Ÿ�� ����.
enum class ERtTctComponentEleType
{
	ENone = 0,

	// �浹 ������Ʈ ��� Ÿ��.
	ECollisionCube = 1,
	ECollisionCapsule = 2,

	// ����Ʈ ������Ʈ ��� Ÿ��.
	ELightPoint = 3,
	ELightSpot = 4,

	// ����Ʈ ������Ʈ ��� Ÿ��.
	EEffect = 5,

	// ��� ������Ʈ ��� Ÿ��.
	EFunction = 6,

	// �ε���.
	EIndexColStart = ECollisionCube,
	EIndexColEnd = ECollisionCapsule,

	EIndexLightStart = ELightPoint,
	EIndexLightEnd = ELightSpot
};

// TCT �� ������Ʈ�� ��� �޺� Ÿ�� ����.
enum class ERtComponentComboType
{
	ENone = 0,

	// [ ���� ������Ʈ ]

	// ���� ����.
	ECombuIsBurning = 1, //����
	ECombuIsNotBurning = 2, //�ҿ�

	// ���� ����.
	ECombuDecomposedCombustion = 3, //���ؿ���(����)
	ECombuFlamingCombustion = 4, //�Ҳɿ���(����)

	// ���� ��.
	ECombuWhiteColor = 5, //���
	ECombuGrayColor = 6, //ȸ��
	ECombuBlackColor = 7, //������

	// ���� ��.
	ECombuSmall = 8, //�ҷ�
	ECombuNormal = 9, //�Ϲ�
	ECombuBig = 10, //�뷮

	// ��� �ɼ�.
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

// ���� ������Ʈ ���� �޺� Ÿ��.
enum class ERtEquipComSocketComboType
{
	ENone = 0,

	EEquip_MouthSocket = 1, // ���� �̸� : "mouth_socket"
	EEquip_RightHand = 2 // ���� �̸� : "r_hand_socket"
};

// TCT Ÿ�� ID
enum class ERtTctType
{

	ENone = 0,

	ECharacter = 1,	// ĳ����.(1)
	EWall = 2,	// ��.(2)
	ETile = 3,	// Ÿ��.(3)
	EProp = 4,	// ��ǰ.(4)
	EEquipProp = 5,	// ���� ����.(5)
	ECar = 6,	// ����.(6)
	EGun = 7,	// ��.(7)
	EFireNozzle = 8,	// �ҹ� ��â.(8)

	// TCT �������� ������ �ʴ´�. CCT �� �ùķ����� ����.
	EPlayerSpawn = 101,	// �÷��̾� ����.
	EWayPoint = 102,	// ���� ����Ʈ.
	EArea = 103,	// ����.
	EInvisibleWall = 104,  // ���� ��.
	EMapBoundary = 105,  // �� ��� ���� ǥ�ÿ�.
	ETriggerBox = 106,  // Ʈ���� �ڽ�. ( �������� �����ϴ� )
};

// Spawn Ÿ�� ID
enum class ERtMctSpawnType
{
	ENone = 0,
	EPC = 1,	// PC.
	ENPC = 2,	// NPC.
	EOther = 3	// �ٸ� ��ü��
};

// TCT ������Ʈ ID
enum class ERtComponentType
{

	ENone = 0,

	EObject = 1,  // �⺻ ��ü ������Ʈ ( ���� ������Ʈ ).
	ECollision = 2,  // �浹 �ڽ� ������Ʈ.
	EMaterial = 3,  // ���� ������Ʈ.
	EAnimationViewer = 4,  // �ִϸ��̼� ������Ʈ. ( ����� ������Ʈ : �� �����Ͱ� ������� �ʴ´�. )
	EAudio = 5,  // ����� ������Ʈ.
	ELight = 6,  // ����Ʈ ������Ʈ.
	EEffect = 7,  // ����Ʈ ������Ʈ.
	EFunction = 8,  // ��� ������Ʈ.
	EGunFire = 9,  // �� �߻� ������Ʈ. ( �켱 ���� �и� )
	EWaterSpray = 10, // �� �л� ������Ʈ.
	ECombustion = 11, // ���� ������Ʈ.	
	EEquip = 12, // ���� ������Ʈ.	

	EInteract = 13, // ��ȣ�ۿ� ������Ʈ. ( �켱 ���� �и� )
	EInsideModel = 14, // �ǳ� �� ������Ʈ. ( �켱 ���� �и� )
};

// TCT ������Ʈ ���� ENUM ( �������Ʈ������ �� �� ���� !! )
// �⺻������ ������Ʈ Ÿ���� ERtComponentType �� ����Ͽ� ����. 
// �⺻���� ���� ������ �� ���� ���� enum ERtComponentTypeInfo�� �̿��Ͽ� ����. 
// ( C++ ������ ��� )
enum ERtComponentTypeInfo 
{

	EInfoNone = 0,

	EInfoObject = 1 << 0,	// �⺻ ��ü ������Ʈ ( ���� ������Ʈ ).
	EInfoCollision = 1 << 1,	// �浹 �ڽ� ������Ʈ.
	EInfoMaterial = 1 << 2,	// ���� ������Ʈ.
	EInfoAnimationViewer = 1 << 3,	// �ִϸ��̼� ������Ʈ. ( ����� ������Ʈ : �� �����Ͱ� ������� �ʴ´�. )
	EInfoAudio = 1 << 4,	// ����� ������Ʈ.
	EInfoLight = 1 << 5,	// ����Ʈ ������Ʈ.
	EInfoEffect = 1 << 6,	// ����Ʈ ������Ʈ.
	EInfoFunction = 1 << 7,	// ��� ������Ʈ.
	EInfoGunFire = 1 << 8,	// �� �߻� ������Ʈ. 
	EInfoWaterSpray = 1 << 9,	// �� �л� ������Ʈ.
	EInfoCombustion = 1 << 10,	// ���� ������Ʈ.	
	EEquip = 1 << 11,	// ���� ������Ʈ.

	EInfoInteract = 1 << 12,	// ��ȣ�ۿ� ������Ʈ. ( �켱 ���� �и� )
	EInfoInsideModel = 1 << 13,	// �ǳ� �� ������Ʈ. ( �켱 ���� �и� )

	// ĳ���� �⺻ ������Ʈ.
	EInfo_ComMix_Character = EInfoObject | EInfoMaterial | EInfoCollision | EInfoAnimationViewer,

	// �� �⺻ ������Ʈ.
	EInfo_ComMix_Wall = EInfoObject | EInfoMaterial | EInfoCollision | EInfoFunction | EInfoCombustion,

	// Ÿ�� �⺻ ������Ʈ.
	EInfo_ComMix_Tile = EInfoObject | EInfoMaterial | EInfoCollision | EInfoFunction | EInfoCombustion,

	// ����(��ǰ) �⺻ ������Ʈ.
	EInfo_ComMix_Prop = EInfoObject | EInfoMaterial | EInfoCollision | EInfoFunction | EInfoCombustion,

	// ������ ����(��ǰ) �⺻ ������Ʈ.
	EInfo_ComMix_EquipProp = EInfoObject | EInfoMaterial | EInfoGunFire | EInfoEffect | EInfoAudio | EEquip | EInfoAnimationViewer,

	// ���� �⺻ ������Ʈ.
	EInfo_ComMix_Car = EInfoObject | EInfoMaterial | EInfoCollision | EInfoInteract | EInfoInsideModel | EInfoFunction | EInfoCombustion,

	// �� �⺻ ������Ʈ.
	EInfo_ComMix_Gun = EInfoObject | EInfoMaterial | EInfoGunFire | EInfoEffect | EInfoAudio | EEquip | EInfoAnimationViewer,

	// ��â �⺻ ������Ʈ.
	EInfo_ComMix_FireNozzle = EInfoObject | EInfoMaterial | EInfoWaterSpray | EInfoEffect | EInfoAudio | EEquip | EInfoAnimationViewer,

	// ���� �߰� ���� ������Ʈ. ( �⺻ ������Ʈ ���� ���Ƿ� �߰��� ������ ������Ʈ ���� )
	EInfo_ComMix_AddEnable = EInfoAudio | EInfoLight | EInfoEffect,
};

// ������Ʈ �� ��Ʈ�� ID. 
enum class ERtComponentUnits
{
	ENone = 0,

	ECollision_AddUnit,		// �浹 ������Ʈ ������Ʈ �߰� ����.
	ECollision_AddUnit_AddBtn,	// �浹 ������Ʈ ������Ʈ �߰� ��ư.

	ECollision_ElementUnit,		// �浹 ������Ʈ ������Ʈ ����.
};


enum class ERtCommandType
{
	ENone = 0,

	ECmd_Coll_AddEle,
};

// ���� �������� ��ȭ���ڿ� ( ���� ������ �������� �� )
enum class ERtModelKind
{
	ENone = 0,

	EDefaultModel, // ��ü �⺻ ��

	// ����Ʈ ������Ʈ ��.
	EEffectComEffectModel,		// ����Ʈ ��

	// ���� ������Ʈ ��.
	ECombustionBurningModel,	// ��ü�� Ÿ�� ��
	ECombustionEndBurningModel, // ��ü�� �� ź ��

	// ��� ������Ʈ ��.
	EFunctionAnimationModel,	// ��� �ִϸ��̼� ��
	EFunctionEffectModel,		// ��� ����Ʈ ��

	EFunctionOperateModel,		// ��� �۵� ��
};

// ���� �������� ��ȭ������ ��ȭ���� Ÿ��.
enum class ERtImportDialogType
{
	ENone = 0,

	EImportModel,
	EImportAnimationSequence,
	EImportEffectModel,
};

enum class RtMCTAction
{
	None, // ����
	Idle, // �⺻ ����
	Move, // ������
	Selected, // ����
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

	EStringVariable, // ���ڿ� ������.
	EStringGameState, // ���� ���� ���ڿ�.
	EStringID, // ��ü �ν��Ͻ� ID.
	EStringMenuSetID, // �޴� ��Ʈ ID
	EStringMessage, // ���ڿ� �޽���.
	EStringAniName, // �ִϸ��̼� �̸�.
	EStringAniNameList, // �ִϸ��̼� �̸� ����Ʈ.
	EStringVideoFile, // mp4, avi ���� ���ϸ�.
	EStringFormat, // ���� ���ڿ�.
	EStringFileName, // ���� �̸�.
	EStringEventID, // �̺�Ʈ ID.
	EStringKeyValue, // �Է� Ű ��.

	EIntValue, // Int ���� ��.
	EFloatValue, // Float ���� ��.

	EStringList, // ��Ʈ�� ����Ʈ.
};

// ���μ��� ������ UI Ÿ��
/*
enum class ERtSctEventUIType
{
	ENone = 0,
	// 3D ���� UI ( �޴��� 3������ ���ִ� ��� )
	E3DCircle,
	// 3D ���簢 UI ( �޴��� 3������ ���ִ� ��� )
	E3DRectangle,
	// ��ũ�� ���� UI ( �޴��� ��ũ���� �پ� �ִ� ��� )
	EScreenCircle,
	// ��ũ�� ���簢 UI ( �޴��� ��ũ���� �پ� �ִ� ��� )
	EScreenRectangle
};
*/
JS_ENUM( ERtSctEventUIType, ENone, E3DCircle, E3DRectangle, EScreenCircle, EScreenRectangle )
JS_ENUM_DECLARE_STRING_PARSER( ERtSctEventUIType );
