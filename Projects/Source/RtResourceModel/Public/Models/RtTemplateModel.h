// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "RtEnums.h"
#include "RtDefines.h"

#include "RtTemplateModel.generated.h"

USTRUCT(BlueprintType, Category = "FRtTctComTransformNoScale")
struct FRtTctComTransformNoScale
{
	GENERATED_BODY()
public:
	FRtTctComTransformNoScale()
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComTransformNoScale")
	FVector Position = FVector::ZeroVector; // ��ġ

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComTransformNoScale")
	FRotator Rotation = FRotator::ZeroRotator; // ȸ��.

	FTransform GetTransformNoScale() {
		return FTransform(Rotation, Position, FVector(1, 1, 1));
	}
};

USTRUCT(BlueprintType, Category = "FRtTctComTransform")
struct FRtTctComTransform : public FRtTctComTransformNoScale
{
	GENERATED_BODY()
public:
	FRtTctComTransform()
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComTransform")
	FVector Scale = FVector::OneVector; // ũ��.

	FTransform GetTransform() {
		return FTransform(Rotation, Position, Scale);
	}
};

// �⺻ ������Ʈ.
USTRUCT(BlueprintType, Category = "FRtTctComDefault")
struct FRtTctComDefault : public FRtTctComTransform
{
	GENERATED_BODY()
public:
	FRtTctComDefault()
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDefault")
	ERtTctType2 TctType = ERtTctType2::ENone; // Tct Ÿ��
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDefault")
	FString ModelName; // Tct �� �̸�.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDefault")
	FString ModelFileName; // Tct �� ���� �̸�. (Fbx ���ϸ�) 
};

// �浹 ������Ʈ
USTRUCT(BlueprintType, Category = "FRtTctComCollision")
struct FRtTctComCollision : public FRtTctComTransform
{
	GENERATED_BODY()
public:
	FRtTctComCollision()
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComCollision")
	ERtCollisionShape CollisionShape = ERtCollisionShape::ENone; // �浹ü ��� ( �ڽ� / ĸ�� )

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComCollision")
	ERtCollisionType CollisionType = ERtCollisionType::ENone; // �浹ü Ÿ�� ( ��� ���� / ��� ���� )
};

// ���� ������Ʈ�� �׸�
USTRUCT(BlueprintType, Category = "FRtTctComDirectionItem")
struct FRtTctComDirectionItem
{
	GENERATED_BODY()
public:
	FRtTctComDirectionItem()
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDirectionItem")
	ERtAnimType AnimationType = ERtAnimType::ENone; // Animation Ÿ��, �Ϲ� Ÿ���� ENone, �� �ܴ� �⺻ �ִϸ��̼� �������̵�

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDirectionItem")
	FString AnimationID; // Animation ID

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDirectionItem")
	FString AnimationFileName; // Animation Fbx ���ϸ� �Ǵ� Animation Name ( ���� �ִϸ��̼� )

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDirectionItem")
	bool bIsEmbeddingAnimation; // ���� �ִϸ��̼��� ���� ���� ����.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDirectionItem")
	FString SoundFileName; // Wav ���ϸ�

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDirectionItem")
	FString EffectFileName; // Effect ���ϸ� ( Ȯ���ڴ� efk, ���� �θ� ������ ���� �� : "Blow/Blow.efk" )

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDirectionItem")
	FRtTctComTransform EffectTransform; // Effect Ʈ������

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDirectionItem")
	float EffectSpeed = 1.0f; // Effect �ӵ�.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDirectionItem")
	int32 AnimFrameIndex_Sound = -1; // �ִϸ��̼� �����ӿ� ���� �Ҵ�

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDirectionItem")
	int32 AnimFrameIndex_Effect = -1; // �ִϸ��̼� �����ӿ� ����Ʈ �Ҵ�
};

// ���� ������Ʈ
USTRUCT(BlueprintType, Category = "FRtTctComDirection")
struct FRtTctComDirection
{
	GENERATED_BODY()
public:
	FRtTctComDirection()
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDirection")
	TArray<FRtTctComDirectionItem> DirectionItemList; // ���� �׸� ����Ʈ.
};

USTRUCT(BlueprintType, Category = "FRtTctComDefaultAnimStruct")
struct FRtTctComDefaultAnimStruct
{
	GENERATED_BODY()
public:
	FRtTctComDefaultAnimStruct()
	{
	}

	FRtTctComDefaultAnimStruct(float animSpeed, float moveSpeed)
	{
		AnimationSpeed = animSpeed;
		MoveSpeed = moveSpeed;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDefaultAnimStruct")
	float AnimationSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDefaultAnimStruct")
	float MoveSpeed;
};

// �ൿ ������Ʈ
USTRUCT(BlueprintType, Category = "FRtTctComBehavior")
struct FRtTctComBehavior
{
	GENERATED_BODY()
public:
	FRtTctComBehavior()
	{
		for(int32 i = (int32)ERtAnimType::EIdle; i<(int32)ERtAnimType::ESize; i++) {
			DefaultAnimSpeedMap.Add(i, FRtTctComDefaultAnimStruct(TOOL_SYSTEM_FBX_ANIMATION_DEFAULT_SPEED, TOOL_SYSTEM_DEFAULT_MOVE_SPEED));
		}
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComBehavior")
	ERtRoleType RoleType = ERtRoleType::ENone; // ���� Ÿ��

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComBehavior")
	TMap<int32, FRtTctComDefaultAnimStruct> DefaultAnimSpeedMap;

	float GetDefaultAnimSpeed(ERtAnimType animType) {
		if (animType < ERtAnimType::ESize) {
			FRtTctComDefaultAnimStruct* find = DefaultAnimSpeedMap.Find((int32)animType);
			if (find) {
				return find->AnimationSpeed;
			}
		}
		return 0.0f;
	}

	void SetDefaultAnimSpeed(ERtAnimType animType, float speed) {
		if (animType < ERtAnimType::ESize) {
			DefaultAnimSpeedMap[(int32)animType].AnimationSpeed = speed;
		}
	}

	float GetDefaultMoveSpeed(ERtAnimType animType) {
		if (animType < ERtAnimType::ESize) {
			FRtTctComDefaultAnimStruct* find = DefaultAnimSpeedMap.Find((int32)animType);
			if (find) {
				return find->MoveSpeed;
			}
		}
		return 0.0f;
	}

	void SetDefaultMoveSpeed(ERtAnimType animType, float speed) {
		if (animType < ERtAnimType::ESize) {
			DefaultAnimSpeedMap[(int32)animType].MoveSpeed = speed;
		}
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComBehavior")
	ERtSex Sex = ERtSex::ENone; // ����

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComBehavior")
	ERtAgeRange AgeRange = ERtAgeRange::ENone; // ���̴�

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComBehavior")
	ERtHeightRange HeightRange = ERtHeightRange::ENone; // ����

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComBehavior")
	ERtPersonState PersonState = ERtPersonState::ENone; // ��ֿ���

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComBehavior")
	ERtWoundState WoundState = ERtWoundState::ENone; // �λ󿩺�
};

USTRUCT(BlueprintType, Category = "FRtTctComSocket")
struct FRtTctComSocket : public FRtTctComTransformNoScale
{
	GENERATED_BODY()
public:
	FRtTctComSocket()
	{
	}
};

// ���� ������Ʈ
USTRUCT(BlueprintType, Category = "FRtTctComEquip")
struct FRtTctComEquip
{
	GENERATED_BODY()
public:
	FRtTctComEquip()
	{
	}
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComEquip")
	FRtTctComSocket TrackerSocket; // Ʈ��Ŀ Ʈ������ ����

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComEquip")
	FRtTctComSocket LeftHandSocket; // �޼� ���� ���� ��� Ʈ������ ����

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComEquip")
	FRtTctComSocket RightHandSocket; // ������ ���� ���� ��� Ʈ������ ����
};

USTRUCT(BlueprintType, Category = "FRtTctComCarSeat")
struct FRtTctComCarSeat : public FRtTctComTransformNoScale
{
	GENERATED_BODY()
public:
	FRtTctComCarSeat()
	{
	}
};

// ���� ������Ʈ
USTRUCT(BlueprintType, Category = "FRtTctComCar")
struct FRtTctComCar
{
	GENERATED_BODY()
public:
	FRtTctComCar() {
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComCar")
	TMap<ERtCarAnimType, FString> CarAnimMap; // ����� �̵� �ϳ��� ����.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComCar")
//	TMap<FString, FRtTctComCarSeat> CarSeatMap; // Key : �¼� �̸�, Data : Transform, ( �¼� �̸��� �¼�01, �¼�02, .. �̷��� �ڵ����� �ٿ��� )
	TArray<FRtTctComCarSeat> CarSeatList; // Data : Transform
};

USTRUCT(BlueprintType, Category = "FRtTctComEnvironment")
struct FRtTctComEnvironment
{
	GENERATED_BODY()
public:
	FRtTctComEnvironment() {
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComEnvironment")
	FString SkyBoxFileName; // SkyBox Fbx ���ϸ�.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComEnvironment")
	FRtTctComTransform SkyBoxTM; // SkyBox Ʈ������

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComEnvironment")
	FString EnvironmentSoundFileName; // ȯ�� ���� Wav ���ϸ�. (��: ���� �����ʹ� �Ҹ�, �ڵ��� ���� ���)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComEnvironment")
	FString BackgroundMusicFileName; // ��� ���� Wav ���ϸ�. 
};

// ������ TCT ���� ����ü
USTRUCT(BlueprintType, Category = "FRtTctStruct")
struct FRtTctStruct
{
	GENERATED_BODY()
public:
	FRtTctStruct()
	{
		Version = 0;
	}

	void SetVersion() {
		Version = TCT_FILE_VERSION;
	}

	void SetModelName(FString modelName) {
		ComDefault.ModelName = modelName;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctStruct")
	int32 Version; // Tct ���� ����.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctStruct")
	FRtTctComDefault ComDefault; // �⺻ ������Ʈ. (��� Ÿ�Կ��� ������ �ִ�)

	// **����**
	// Ÿ�� ���� �� ���� �־ �Ǵ� ����ü�鵵 ���� TArray�� �־ �ۼ��ϴ� ������ 
	// ���� FRtTctStruct ����ü�� ��� Tct Ÿ���� ���� �� �� �ִ� �����ͱ����� ����� ������
	// �̷��� �ϸ� ������ �ʴ� ������Ʈ�� ��� �����̳� �̸��� ����ǹǷ� ������ ũ�⸦ ���� �� ����.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctStruct")
	TArray<FRtTctComCollision> ComCollisionList; // �浹 ������Ʈ ����Ʈ. ( ��Ƽ ) 

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctStruct")
	TArray<FRtTctComDirection> ComDirectionList; // ���� ������Ʈ ����Ʈ. ( �̱� )

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctStruct")
	TArray<FRtTctComBehavior> ComBehaviorList; // �ൿ ������Ʈ ����Ʈ. ( �̱� )

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctStruct")
	TArray<FRtTctComEquip> ComEquipList; // ���� ������Ʈ ����Ʈ. ( �̱� )

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctStruct")
	TArray<FRtTctComCar> ComCarList; // ���� ������Ʈ ����Ʈ. ( �̱� )

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctStruct")
	TArray<FRtTctComEnvironment> ComEnvironment; // ȯ�� ������Ʈ ����Ʈ. ( �̱� )
};

UCLASS()
class RTRESOURCEMODEL_API ARtTemplateModel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARtTemplateModel();

	UFUNCTION(BlueprintCallable, Category = "ARtTemplateModel")
	static bool LoadTctFile(FString tctFilePath, FRtTctStruct& targetData); // Tct ���� �ε�.
	
	UFUNCTION(BlueprintCallable, Category = "ARtTemplateModel")
	static bool SaveTctFile(FString tctFilePath, const FRtTctStruct& sourceData); // Tct ���� ���̺�.

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
