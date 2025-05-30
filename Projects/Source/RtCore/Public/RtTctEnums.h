#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#define TCT2_VERSION_TAG TEXT("version")

// ������ TCT //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// *���̹� ��Ģ : ���� enum �̸��� ���� 2�� �ٿ� �ѹ��� �Ͽ���.

// ������ TCT : TCT Ÿ��
UENUM(BlueprintType)
enum class ERtTctType2 : uint8 {
	ENone = 0,

	ENormal			= 1, // �Ϲ�
	ECharacter		= 2, // ĳ����
	EEquipment		= 3, // ���
	ECar			= 4, // �ڵ���
	EEnvironment	= 5, // ȯ��

	// Ư�� Ÿ���� ��� �״�� �����.
	// TCT �������� ������ �ʴ´�. CCT �� �ùķ����� ����.	
	EPlayerSpawn = 101,	// �÷��̾� ����.
	EWayPoint = 102,	// ���� ����Ʈ.
	EArea = 103,	// ����.
	EInvisibleWall = 104,  // ���� ��.
	EMapBoundary = 105,  // �� ��� ���� ǥ�ÿ�.
	ETriggerBox = 106,  // Ʈ���� �ڽ�. ( �������� �����ϴ� )
};

// ������ TCT : TCT ������Ʈ Ÿ��
UENUM(BlueprintType)
enum class ERtComponentType2 : uint8 {
	ENone			= 0,

	EDefault		= 1 UMETA(DisplayName = "Default Component"), // �⺻ ������Ʈ
	ECollision		= 2 UMETA(DisplayName = "Collision Component"), // �浹 ������Ʈ.
	EDirection		= 3 UMETA(DisplayName = "Direction Component"), // ���� ������Ʈ.
	EBehavior		= 4 UMETA(DisplayName = "Behavior Component"), // �׼� ������Ʈ.
	EEquip			= 5 UMETA(DisplayName = "Equip Component"), // ���� ������Ʈ.
	ECar			= 6 UMETA(DisplayName = "Car Component"), // ���� ������Ʈ.
	EEnvironment	= 7 UMETA(DisplayName = "Environment Component"), // ȯ�� ������Ʈ.
};

// TCT ������Ʈ ���� ENUM ( �������Ʈ������ �� �� ���� !! )
// �⺻������ ������Ʈ Ÿ���� ERtTctComponentType �� ����Ͽ� ����. 
// �⺻���� ���� ������ �� ���� ���� enum ERtTctComponentTypeInfo�� �̿��Ͽ� ����. ( C++ ������ ��� )
// ������ TCT : TCT Ÿ�Կ� ���� TCT ������Ʈ ���� ����
enum ERtComponentTypeInfo2 {
	EComNone			= 0,

	EComDefault			= 1 << 0, // �⺻ ������Ʈ.
	EComCollision		= 1 << 1, // �浹 ������Ʈ.
	EComDirection		= 1 << 2, // ���� ������Ʈ.
	EComBehavior		= 1 << 3, // �ൿ ������Ʈ.
	EComEquip			= 1 << 4, // ���� ������Ʈ.
	EComCar				= 1 << 5, // �ڵ��� ������Ʈ.
	EComEnvironment		= 1 << 6, // ȯ�� ������Ʈ.

	// ERtTctType2�� 5�� Tct Ÿ�� �� ������Ʈ ���� ������ �����Ѵ�.
	ECom_TctType_Normal		= EComDefault | EComCollision | EComDirection, // Tct �Ϲ� Ÿ���� ������Ʈ ����.
	ECom_TctType_Character	= EComDefault | EComCollision | EComDirection | EComBehavior, // Tct ĳ���� Ÿ���� ������Ʈ ����.
	ECom_TctType_Equipment	= EComDefault | EComCollision | EComDirection | EComEquip, // Tct ��� Ÿ���� ������Ʈ ����.
	ECom_TctType_Car		= EComDefault | EComCollision | EComDirection | EComCar, // Tct �ڵ��� Ÿ���� ������Ʈ ����.
	ECom_TctType_Environment= EComDefault | EComEnvironment, // Tct ȯ�� Ÿ���� ������Ʈ ����.
};

UENUM(BlueprintType)
enum class ERtCollisionShape : uint8 { // �浹ü ��� 
	ENone = 0,

	EBoxShape,
	ECapsuleShape
};

UENUM(BlueprintType)
enum class ERtCollisionType : uint8 { // �浹ü Ư��
	ENone = 0,

	ECollisionPass, // �浹 ��� ����.
	ECollisionBlock, // �浹 ��� ����.
};

UENUM(BlueprintType)
enum class ERtRoleType : uint8 { // ���� Ÿ��
	ENone = 0,

	ERoleCivilian, // �ΰ���
	ERoleSuspicious, // �ż���
	ERoleTerrorist, // �׷���
	ERoleHostage, // ����
	ERoleSoldier, // ����
};

UENUM(BlueprintType)
enum class ERtSex : uint8 { // ����
	ENone = 0,

	EMale, // ����
	EFemale, // ����
};

UENUM(BlueprintType)
enum class ERtAgeRange : uint8 { // ���̴�
	ENone = 0,

	EAge_10_50, // 10~50��
	EAge_60_, // 60��
	EAge_70_, // 70��
};

UENUM(BlueprintType)
enum class ERtHeightRange : uint8 { // ����
	ENone = 0,
	EHeight_170,
	EHeight_180,
	EHeight_190,
};

UENUM(BlueprintType)
enum class ERtPersonState : uint8 { // ��ֿ���
	ENone = 0,

	ENonDisabledPerson, // �Ϲ��� ( ���� ) 
	EDisabledPerson, // ����� ( ��� )
};

UENUM(BlueprintType)
enum class ERtWoundState : uint8 { // �λ󿩺�
	ENone = 0,

	ENotWounded, // ��ġ�� ����.
	EWounded, // ��ħ.
};

UENUM(BlueprintType)
enum class ERtCarAnimType : uint8 { // �ڵ��� �ִϸ��̼� Ÿ��.
	ENone = 0,

	EMove, // �̵�
	ELeftDoorOpen, // ���ʹ� ����.
	ELeftDoorClose, // ���ʹ� �ݱ�.
	ERightDoorOpen, // �����ʹ� ����.
	ERightDoorClose, // �����ʹ� �ݱ�.
};

UENUM(BlueprintType)
enum class ERtAnimType : uint8 { // �ִϸ��̼� Ÿ��
	ENone = 0,

	EIdle, // ���
	EWalk, // �ȱ�
	ERun, // �ٱ�
	EAttack, // ����
	EDefend, // ��� 
	ECollapse, // ������ ( �� �ִ� ���¿��� ������ �ִ� )
	ESurrender, // �׺� ( �� �ִ� ���¿��� �׺��ϱ���� �ִ� )
	ELieDown, // ���帮�� ( �� �ִ� ���¿��� ���帮����� �ִ� )
	ERestrain, // ���� ���� ( ���帰 ���¿��� �����ϱ� �ִ� )
	EStandUp, // �Ͼ�� ( ���ڻ��¿��� �Ͼ����� �ִ� )
	EEscort, // ���� �̵� ( �Ͼ ���� ���¿��� �ȱ� �ִ� )

	ESize, // 
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////