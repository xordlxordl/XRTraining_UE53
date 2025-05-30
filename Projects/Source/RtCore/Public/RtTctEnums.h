#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#define TCT2_VERSION_TAG TEXT("version")

// 리뉴얼 TCT //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// *네이밍 규칙 : 기존 enum 이름에 숫자 2를 붙여 넘버링 하였다.

// 리뉴얼 TCT : TCT 타입
UENUM(BlueprintType)
enum class ERtTctType2 : uint8 {
	ENone = 0,

	ENormal			= 1, // 일반
	ECharacter		= 2, // 캐릭터
	EEquipment		= 3, // 장비
	ECar			= 4, // 자동차
	EEnvironment	= 5, // 환경

	// 특수 타입의 경우 그대로 사용함.
	// TCT 툴에서는 사용되지 않는다. CCT 및 시뮬레이터 전용.	
	EPlayerSpawn = 101,	// 플레이어 스폰.
	EWayPoint = 102,	// 웨이 포인트.
	EArea = 103,	// 영역.
	EInvisibleWall = 104,  // 투명 벽.
	EMapBoundary = 105,  // 맵 경계 영역 표시용.
	ETriggerBox = 106,  // 트리거 박스. ( 스케일이 가능하다 )
};

// 리뉴얼 TCT : TCT 컴포넌트 타입
UENUM(BlueprintType)
enum class ERtComponentType2 : uint8 {
	ENone			= 0,

	EDefault		= 1 UMETA(DisplayName = "Default Component"), // 기본 컴포넌트
	ECollision		= 2 UMETA(DisplayName = "Collision Component"), // 충돌 컴포넌트.
	EDirection		= 3 UMETA(DisplayName = "Direction Component"), // 연출 컴포넌트.
	EBehavior		= 4 UMETA(DisplayName = "Behavior Component"), // 액션 컴포넌트.
	EEquip			= 5 UMETA(DisplayName = "Equip Component"), // 장착 컴포넌트.
	ECar			= 6 UMETA(DisplayName = "Car Component"), // 차량 컴포넌트.
	EEnvironment	= 7 UMETA(DisplayName = "Environment Component"), // 환경 컴포넌트.
};

// TCT 컴포넌트 정보 ENUM ( 블루프린트에서는 쓸 수 없다 !! )
// 기본적으로 컴포넌트 타입은 ERtTctComponentType 을 사용하여 쓴다. 
// 기본적인 정보 개념을 쓸 때는 순수 enum ERtTctComponentTypeInfo를 이용하여 쓴다. ( C++ 에서만 사용 )
// 리뉴얼 TCT : TCT 타입에 따른 TCT 컴포넌트 구성 정보
enum ERtComponentTypeInfo2 {
	EComNone			= 0,

	EComDefault			= 1 << 0, // 기본 컴포넌트.
	EComCollision		= 1 << 1, // 충돌 컴포넌트.
	EComDirection		= 1 << 2, // 연출 컴포넌트.
	EComBehavior		= 1 << 3, // 행동 컴포넌트.
	EComEquip			= 1 << 4, // 부착 컴포넌트.
	EComCar				= 1 << 5, // 자동차 컴포넌트.
	EComEnvironment		= 1 << 6, // 환경 컴포넌트.

	// ERtTctType2의 5종 Tct 타입 별 컴포넌트 구성 정보를 나열한다.
	ECom_TctType_Normal		= EComDefault | EComCollision | EComDirection, // Tct 일반 타입의 컴포넌트 구성.
	ECom_TctType_Character	= EComDefault | EComCollision | EComDirection | EComBehavior, // Tct 캐릭터 타입의 컴포넌트 구성.
	ECom_TctType_Equipment	= EComDefault | EComCollision | EComDirection | EComEquip, // Tct 장비 타입의 컴포넌트 구성.
	ECom_TctType_Car		= EComDefault | EComCollision | EComDirection | EComCar, // Tct 자동차 타입의 컴포넌트 구성.
	ECom_TctType_Environment= EComDefault | EComEnvironment, // Tct 환경 타입의 컴포넌트 구성.
};

UENUM(BlueprintType)
enum class ERtCollisionShape : uint8 { // 충돌체 모양 
	ENone = 0,

	EBoxShape,
	ECapsuleShape
};

UENUM(BlueprintType)
enum class ERtCollisionType : uint8 { // 충돌체 특성
	ENone = 0,

	ECollisionPass, // 충돌 통과 가능.
	ECollisionBlock, // 충돌 통과 못함.
};

UENUM(BlueprintType)
enum class ERtRoleType : uint8 { // 역할 타입
	ENone = 0,

	ERoleCivilian, // 민간인
	ERoleSuspicious, // 거수자
	ERoleTerrorist, // 테러범
	ERoleHostage, // 인질
	ERoleSoldier, // 군인
};

UENUM(BlueprintType)
enum class ERtSex : uint8 { // 성별
	ENone = 0,

	EMale, // 남성
	EFemale, // 여성
};

UENUM(BlueprintType)
enum class ERtAgeRange : uint8 { // 나이대
	ENone = 0,

	EAge_10_50, // 10~50대
	EAge_60_, // 60대
	EAge_70_, // 70대
};

UENUM(BlueprintType)
enum class ERtHeightRange : uint8 { // 신장
	ENone = 0,
	EHeight_170,
	EHeight_180,
	EHeight_190,
};

UENUM(BlueprintType)
enum class ERtPersonState : uint8 { // 장애여부
	ENone = 0,

	ENonDisabledPerson, // 일반인 ( 없음 ) 
	EDisabledPerson, // 장애인 ( 장애 )
};

UENUM(BlueprintType)
enum class ERtWoundState : uint8 { // 부상여부
	ENone = 0,

	ENotWounded, // 다치지 않음.
	EWounded, // 다침.
};

UENUM(BlueprintType)
enum class ERtCarAnimType : uint8 { // 자동차 애니메이션 타입.
	ENone = 0,

	EMove, // 이동
	ELeftDoorOpen, // 왼쪽문 열기.
	ELeftDoorClose, // 왼쪽문 닫기.
	ERightDoorOpen, // 오른쪽문 열기.
	ERightDoorClose, // 오른쪽문 닫기.
};

UENUM(BlueprintType)
enum class ERtAnimType : uint8 { // 애니메이션 타입
	ENone = 0,

	EIdle, // 대기
	EWalk, // 걷기
	ERun, // 뛰기
	EAttack, // 공격
	EDefend, // 방어 
	ECollapse, // 쓰러짐 ( 서 있는 상태에서 쓰러짐 애니 )
	ESurrender, // 항복 ( 서 있는 상태에서 항복하기까지 애니 )
	ELieDown, // 엎드리기 ( 서 있는 상태에서 엎드리기까지 애니 )
	ERestrain, // 포박 당함 ( 엎드린 상태에서 포복하기 애니 )
	EStandUp, // 일어서기 ( 포박상태에서 일어서기까지 애니 )
	EEscort, // 포박 이동 ( 일어선 포박 상태에서 걷기 애니 )

	ESize, // 
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////