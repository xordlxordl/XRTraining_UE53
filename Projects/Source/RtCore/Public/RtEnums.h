#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "RtCctEnums.h"

#include "RtTctEnums.h"

// TCT 각 컴포넌트의 모든 요소 타입 정보.
UENUM(BlueprintType)
enum class ERtTctComponentEleType : uint8 {
	ENone = 0,

	// 충돌 컴포넌트 요소 타입.
	ECollisionCube		= 1,
	ECollisionCapsule	= 2,

	// 라이트 컴포넌트 요소 타입.
	ELightPoint			= 3,
	ELightSpot			= 4,

	// 이펙트 컴포넌트 요소 타입.
	EEffect				= 5,

	// 기능 컴포넌트 요소 타입.
	EFunction			= 6,

	// 인덱스.
	EIndexColStart		= ECollisionCube,
	EIndexColEnd		= ECollisionCapsule,

	EIndexLightStart	= ELightPoint,
	EIndexLightEnd		= ELightSpot
};

// TCT 각 컴포넌트의 모든 콤보 타입 정보.
UENUM(BlueprintType)
enum class ERtComponentComboType : uint8 {
	ENone = 0,

	// [ 연소 컴포넌트 ]
	
	// 연소 여부.
	ECombuIsBurning				= 1, //가연
	ECombuIsNotBurning			= 2, //불연

	// 연소 종류.
	ECombuDecomposedCombustion	= 3, //분해연소(유염)
	ECombuFlamingCombustion		= 4, //불꽃연소(무염)

	// 연기 색.
	ECombuWhiteColor			= 5, //흰색
	ECombuGrayColor				= 6, //회색
	ECombuBlackColor			= 7, //검정색

	// 연기 양.
	ECombuSmall					= 8, //소량
	ECombuNormal				= 9, //일반
	ECombuBig					= 10, //대량

	// 재생 옵션.
	EPlayOptionOneTime			= 11,
	EPlayOptionLoop				= 12,

	EIndexCombuFlammStart		= ECombuIsBurning,
	EIndexCombuFlammEnd			= ECombuIsNotBurning,

	EIndexCombuKindStart		= ECombuDecomposedCombustion,
	EIndexCombuKindEnd			= ECombuFlamingCombustion,

	EIndexCombuSmokeColorStart	= ECombuWhiteColor,
	EIndexCombuSmokeColorEnd	= ECombuBlackColor,

	EIndexCombuSmokeSizeStart	= ECombuSmall,
	EIndexCombuSmokeSizeEnd		= ECombuBig,

	EIndexPlayOptionStart		= EPlayOptionOneTime,
	EIndexPlayOptionEnd			= EPlayOptionLoop,
};

// 장착 컴포넌트 소켓 콤보 타입.
UENUM(BlueprintType)
enum class ERtEquipComSocketComboType : uint8 {
	ENone = 0,

	EEquip_MouthSocket = 1, // 소켓 이름 : "mouth_socket"
	EEquip_RightHand = 2 // 소켓 이름 : "r_hand_socket"
};

// TCT 타입 ID
UENUM(BlueprintType)
enum class ERtTctType : uint8 {

	ENone			= 0,

	ECharacter		= 1,	// 캐릭터.(1)
	EWall			= 2,	// 벽.(2)
	ETile			= 3,	// 타일.(3)
	EProp			= 4,	// 소품.(4)
	EEquipProp		= 5,	// 장착 소픔.(5)
	ECar			= 6,	// 차량.(6)
	EGun			= 7,	// 총.(7)
	EFireNozzle		= 8,	// 소방 관창.(8)

	// TCT 툴에서는 사용되지 않는다. CCT 및 시뮬레이터 전용.
	EPlayerSpawn	= 101,	// 플레이어 스폰.
	EWayPoint		= 102,	// 웨이 포인트.
	EArea			= 103,	// 영역.
	EInvisibleWall	= 104,  // 투명 벽.
	EMapBoundary	= 105,  // 맵 경계 영역 표시용.
	ETriggerBox		= 106,  // 트리거 박스. ( 스케일이 가능하다 )
};

// Spawn 타입 ID
UENUM(BlueprintType)
enum class ERtMctSpawnType : uint8 {
	ENone = 0,
	EPC = 1,	// PC.
	ENPC = 2,	// NPC.
	EOther = 3	// 다른 객체들
};

// TCT 컴포넌트 ID
UENUM(BlueprintType)
enum class ERtComponentType : uint8 {

	ENone = 0,

	EObject			= 1,  // 기본 객체 컴포넌트 ( 공통 컴포넌트 ).
	ECollision		= 2,  // 충돌 박스 컴포넌트.
	EMaterial		= 3,  // 재질 컴포넌트.
	EAnimationViewer= 4,  // 애니메이션 컴포넌트. ( 뷰어형 컴포넌트 : 실 데이터가 저장되지 않는다. )
	EAudio			= 5,  // 오디오 컴포넌트.
	ELight			= 6,  // 라이트 컴포넌트.
	EEffect			= 7,  // 이펙트 컴포넌트.
	EFunction		= 8,  // 기능 컴포넌트.
	EGunFire		= 9,  // 총 발사 컴포넌트. ( 우선 순위 밀림 )
	EWaterSpray		= 10, // 물 분사 컴포넌트.
	ECombustion		= 11, // 연소 컴포넌트.	
	EEquip			= 12, // 장착 컴포넌트.	

		EInteract	= 13, // 상호작용 컴포넌트. ( 우선 순위 밀림 )
		EInsideModel= 14, // 실내 모델 컴포넌트. ( 우선 순위 밀림 )
};

// TCT 컴포넌트 정보 ENUM ( 블루프린트에서는 쓸 수 없다 !! )
// 기본적으로 컴포넌트 타입은 ERtComponentType 을 사용하여 쓴다. 
// 기본적인 정보 개념을 쓸 때는 순수 enum ERtComponentTypeInfo를 이용하여 쓴다. 
// ( C++ 에서만 사용 )
enum ERtComponentTypeInfo {

	EInfoNone = 0,

	EInfoObject				= 1 << 0,	// 기본 객체 컴포넌트 ( 공통 컴포넌트 ).
	EInfoCollision			= 1 << 1,	// 충돌 박스 컴포넌트.
	EInfoMaterial			= 1 << 2,	// 재질 컴포넌트.
	EInfoAnimationViewer	= 1 << 3,	// 애니메이션 컴포넌트. ( 뷰어형 컴포넌트 : 실 데이터가 저장되지 않는다. )
	EInfoAudio				= 1 << 4,	// 오디오 컴포넌트.
	EInfoLight				= 1 << 5,	// 라이트 컴포넌트.
	EInfoEffect				= 1 << 6,	// 이펙트 컴포넌트.
	EInfoFunction			= 1 << 7,	// 기능 컴포넌트.
	EInfoGunFire			= 1 << 8,	// 총 발사 컴포넌트. 
	EInfoWaterSpray			= 1 << 9,	// 물 분사 컴포넌트.
	EInfoCombustion			= 1 << 10,	// 연소 컴포넌트.	
	EEquip					= 1 << 11,	// 장착 컴포넌트.

		EInfoInteract		= 1 << 12,	// 상호작용 컴포넌트. ( 우선 순위 밀림 )
		EInfoInsideModel	= 1 << 13,	// 실내 모델 컴포넌트. ( 우선 순위 밀림 )

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
UENUM(BlueprintType)
enum class ERtComponentUnits : uint8
{
	ENone = 0,

	ECollision_AddUnit,		// 충돌 컴포넌트 엘리먼트 추가 유닛.
	ECollision_AddUnit_AddBtn,	// 충돌 컴포넌트 엘리먼트 추가 버튼.

	ECollision_ElementUnit,		// 충돌 컴포넌트 엘리먼트 유닛.
};

UENUM(BlueprintType)
enum class ERtCommandType : uint8
{
	ENone = 0,

	ECmd_Coll_AddEle,
};

// 파일 가져오기 대화상자용 ( 내부 데이터 가져오기 용 )
UENUM(BlueprintType)
enum class ERtModelKind : uint8
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
UENUM(BlueprintType)
enum class ERtImportDialogType : uint8
{
	ENone = 0,

	EImportModel, 
	EImportAnimationSequence,
	EImportEffectModel,
};

UENUM	(BlueprintType)
enum class RtMCTAction : uint8 {
	None, // 최초
	Idle, // 기본 상태
    Move, // 움직임
	Selected, // 선택
};

UENUM(BlueprintType)
enum class ERtSctGameState : uint8 {
	EGameState_None = 0,
	EGameState_GotoReadyPlace,
	EGameState_AllArriveReadyPlace,
	EGameState_Play,
	EGameState_Pause,
	EGameState_End,
};

UENUM(BlueprintType)
enum class ERtSctFunCmd : uint8 {
	ENone = 0,

	LoopEvent, // 이벤트 루프를 돌린다.

	IsGameState, // 현재 게임 상태 확인
	IsArriveGameReadyPlace, // 게임 준비 장소에 도착 여부

	JustDoIt, // 즉시 실행
	IsValue, // 변수값이 인자와 같은지 확인
	IsOldValue, // 이전 변수값이 인자와 같은지 확인
	IsVisible, // 객체 보이는지 여부 확인
	IsAnimProgress, // 특정 캐릭터의 애니메이션을 체크한다.
	IsTargetInteract, // 타겟에 에임을 대고 상호작용 키를 눌렀을 때 신호 체크
	//IsKeyUp, // 키를 눌렀다가 뗐을 때 신호 체크
	IsInRadius, // 객체 간 구 충돌 확인
	IsTargetAimed, // 누가 누구(혹은 객체)를 조준하는 경우
	IsHitObject, // 누가 누구를 총으로 맞추었는지 확인한다.
	IsIntimidatingFire, // 위협사격 여부를 판단한다.
	IsTrainingElapsedAt, // 훈련 시작 후 몇 초나 지났는지 판단한다.
	CheckCollision, // 특정 객체의 중심 값이 트리거박스 영역에 들어 있는지 체크

	ReqGameEnd, // 훈련종료 조건이 되면 서버로 보내는 신호

	NotDoing, // 아무 일도 일어나지 않는다.

	SetGameState, // 게임 상태 설정
	SetEnableGameReadyPlace, // 게임 준비 장소 마크 보여주기
	SetValue, // 변수 설정
	DelayTime, // 시간 지연
	SetVisible, // 특정 객체를 보여주기
	SetVisibleGroup, // 특정 객체의 보여주기 그룹 색인을 설정한다.
	PlayAnim, // 애니메이션 플레이
	MoveToWayPoint, // 웨이포인트로 이동하기
	NpcMoveToWayPoint, // 웨이포인트로 이동하기
	StopAct, // 이동 멈춤
	AttachWeapon, // 캐릭터 무기 장착
	AttackTarget, // 캐릭터 공격
	NpcInit, // NPC 상태 초기화

	ShowScreenText, // HMD에 고정된 텍스트 메시지를 띄운다
	ShowTargetText, // 타겟에 고정된 텍스트 메시지를 띄운다
	ShowScreenMenu, // HMD에 고정된 선택 메뉴를 띄운다
	ShowTargetMenu, // 타겟에 고정된 선택 메뉴를 띄운다

	RemoveScreenText, // HMD에 고정된 텍스트 메시지를 띄운다
	RemoveTargetText, // 타겟에 고정된 텍스트 메시지를 띄운다
	RemoveMenu, // HMD에 고정된 선택 메뉴를 띄운다

	RecordResultLog, // 훈련 결과에 쓸 로그를 웹 서버로 보내 카운트
	PlayMovie, // HMD 상에 비디오 플레이

	SetTargetMenuEnabled, // 타겟 메뉴 활성화/비활성화

	VoiceCommand, // 음성 명령 함수.
	SendLog, // 로그 전달 함수.

	IsNotValue, // 변수값이 인자와 같지 않은지 확인
	IsGreaterEqualValue, // 변수값이 인자보다 크거나 같은지 확인
	IsLessEqualValue, // 변수값이 인자보다 작거나 같은지 확인

	RequestRandomNum, // 변수에 1 ~ N 중 1개의 랜덤 값을 리슨 서버로 요청하여 동기화를 시키는 함수. 

	SetVisibleRadioMenuItem, // 무전기 메뉴 중 항목 한 개를 보임, 안보임 설정.
	EndTraining, // 훈련 종료 신호를 보낸다.
};

UENUM(BlueprintType)
enum class ERtSctFunCmdParam : uint8 {
	ENone = 0,

	EStringVariable, // 문자열 변수명.
	EStringGameState, // 게임 상태 문자열.
	EStringID, // 객체 인스턴스 ID.
	EStringDropboxID, // 메뉴 세트 ID
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

UENUM(BlueprintType)
enum class ERtProductType : uint8 {
	ENone = 0,

	ETct,
	EMct,
	EGame
};

// sct ui 리뉴얼 메인 대화 윈도우
UENUM(BlueprintType)
enum class ERtSctItemListWindowType : uint8 {
	ENone = 0,

	EEventWindow, // 이벤트 윈도우
	EDropBoxWindow, // 드롭박스 윈도우
	ESuddenEventWindow, // 돌발 윈도우
	EPlayerGroupWindow, // 플레이어 그룹 윈도우
	EVariableWindow, // 변수 윈도우
	EWalkieTalkieWindow // 무전기 윈도우
};

// sct 리뉴얼 문장 태그 버튼 타입
UENUM(BlueprintType)
enum class ERtSctSentenceButtenType : uint8 {
	ENone = 0 UMETA(DisplayName = "No Button"), // 버튼이 아님.

	ESentenceButtenDefault UMETA(DisplayName = "Def"), // 기본 타입 버튼 ( 특정하지 않는 그냥 버튼 )
	ESentenceButtenObject UMETA(DisplayName = "Obj"), // 객체 타입 버튼
	ESentenceButtenWaypoint UMETA(DisplayName = "Way"), // 웨이포인트 타입 버튼
	ESentenceButtenTrigger UMETA(DisplayName = "Tri"), // 트리거 타입 버튼
	ESentenceButtenNumber UMETA(DisplayName = "Num"), // 수치 타입 버튼
	ESentenceButtenText UMETA(DisplayName = "Txt"), // 텍스트 타입 버튼
	ESentenceButtenAni UMETA(DisplayName = "Ani"), // 애니메이션 타입 버튼
	ESentenceButtenKey UMETA(DisplayName = "Key"), // 키 타입 버튼
	ESentenceButtenVariable UMETA(DisplayName = "Var"), // 변수 타입 버튼
	ESentenceButtenColor UMETA(DisplayName = "Clr"), // 칼라 타입 버튼
	ESentenceButtenSelect UMETA(DisplayName = "Sel"), // 선택 타입 버튼
	ESentenceButtenSound UMETA(DisplayName = "Snd"), // 사운드 타입 버튼
};

UENUM(BlueprintType)
enum class ERtSctSendLogType : uint8 {
	ENone = 0 UMETA(DisplayName = "No Log"),

	//	클라이언트 통신 프로토콜 로그와 비슷하게 이름을 정한다.
	//enum class ELogProtocol
	//{
	//	// protocol, TID , UID
	//	LOG_START = 0,
	//	LOG_FIRE,                // 사격
	//	LOG_HIT,                 // 적중
	//	LOG_CHALLENGE,           // 수하
	//	LOG_SURRENDER,           // 투항명령
	//	LOG_BIND,                // 포박
	//	LOG_COMMANDMOVE,         // 이동지시
	//	LOG_BREIFSITUATION,      // 상황보고
	//	LOG_CALL,                // 증원요청
	//	LOG_EOD,			     // EOD 요청
	//	LOG_BADFIRE,             // 오인사격

	ELog_Challenge UMETA(DisplayName = "Challenge Log"), // 수하
	ELog_Surrender UMETA(DisplayName = "Surrender Log"), // 투항명령
	ELog_Bind UMETA(DisplayName = "Bind Log"), // 포박
	ELog_CommandMove UMETA(DisplayName = "Command Move Log"), // 이동지시
	ELog_BreifSituation UMETA(DisplayName = "Breif Situation Log"), // 상황보고
	ELog_Call UMETA(DisplayName = "Call Log"), // 증원요청
	ELog_CallEOD UMETA(DisplayName = "EOD Log"), // EOD 요청
};

// sct 리뉴얼 메인 대화상자 버튼의 상태 정보.
// 서브 대화상자에서 추가와 편집으로 분기 될때 자신의 상태를 알고 내용을 전달할 때 사용됨.
UENUM(BlueprintType)
enum class ERtSctDialogButtonState : uint8 {
	ENone = 0 UMETA(DisplayName = "None"),

	EAddItem UMETA(DisplayName = "Add Item"),
	EEditItem UMETA(DisplayName = "Edit Item"),
};

// sct 메인 메뉴(무전기)의 스크린 위치 정보 
UENUM(BlueprintType)
enum class ERtSctSystemMenuScreenPos : uint8 {
	ENone = 0 UMETA(DisplayName = "None"),

	ELeftBottom UMETA(DisplayName = "Left Bottom"), // 좌하단
	ERightBottom UMETA(DisplayName = "Right Bottom"), // 우하단
	ERightTop UMETA(DisplayName = "Right Top"), // 우상단
};

//BP 파일로 선언돼있는 팝업 종류 enum 코드로 옮김
UENUM(BlueprintType)
enum class ERtPopupType : uint8 
{
	ENone = 0 UMETA(DisplayName = "None"),

	ESctEvent UMETA(DisplayName = "SctEvent"),
	ESuddenEvent UMETA(DisplayName = "SuddenEvent"),
	EDropBox UMETA(DisplayName = "Dropbox"),
	EGroup UMETA(DisplayName = "Group"),
	EVariable UMETA(DisplayName = "Variable"),
	EWalkietalkie UMETA(DisplayName = "Walkietalkie")
};

UENUM(BlueprintType)
enum class ELanguageType : uint8
{
	EN UMETA(DisplayName = "English"),
	KR UMETA(DisplayName = "Korean")
	//CN UMETA(DisplayName = "Chinese"),
	//VN UMETA(DisplayName = "Vietnamese")
	// 다른 언어 추가 가능
};

UENUM(BlueprintType)
enum class ERtResModelLastError : uint8 {
	EError_None = 0,

	EError_TctLoad_OldVersion, // 오래된 Tct 버전을 로드함.

	EError_MctLoad_OldVersion, // 오래된 Sct 버전을 로드함.

	EError_SctLoad_OldVersion, // 오래된 Sct 버전을 로드함.
};
