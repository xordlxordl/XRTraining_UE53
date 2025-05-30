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
	FVector Position = FVector::ZeroVector; // 위치

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComTransformNoScale")
	FRotator Rotation = FRotator::ZeroRotator; // 회전.

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
	FVector Scale = FVector::OneVector; // 크기.

	FTransform GetTransform() {
		return FTransform(Rotation, Position, Scale);
	}
};

// 기본 컴포넌트.
USTRUCT(BlueprintType, Category = "FRtTctComDefault")
struct FRtTctComDefault : public FRtTctComTransform
{
	GENERATED_BODY()
public:
	FRtTctComDefault()
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDefault")
	ERtTctType2 TctType = ERtTctType2::ENone; // Tct 타입
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDefault")
	FString ModelName; // Tct 모델 이름.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDefault")
	FString ModelFileName; // Tct 모델 파일 이름. (Fbx 파일명) 
};

// 충돌 컴포넌트
USTRUCT(BlueprintType, Category = "FRtTctComCollision")
struct FRtTctComCollision : public FRtTctComTransform
{
	GENERATED_BODY()
public:
	FRtTctComCollision()
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComCollision")
	ERtCollisionShape CollisionShape = ERtCollisionShape::ENone; // 충돌체 모양 ( 박스 / 캡슐 )

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComCollision")
	ERtCollisionType CollisionType = ERtCollisionType::ENone; // 충돌체 타입 ( 통과 가능 / 통과 못함 )
};

// 연출 컴포넌트의 항목
USTRUCT(BlueprintType, Category = "FRtTctComDirectionItem")
struct FRtTctComDirectionItem
{
	GENERATED_BODY()
public:
	FRtTctComDirectionItem()
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDirectionItem")
	ERtAnimType AnimationType = ERtAnimType::ENone; // Animation 타입, 일반 타입은 ENone, 그 외는 기본 애니메이션 오버라이드

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDirectionItem")
	FString AnimationID; // Animation ID

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDirectionItem")
	FString AnimationFileName; // Animation Fbx 파일명 또는 Animation Name ( 내장 애니메이션 )

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDirectionItem")
	bool bIsEmbeddingAnimation; // 현재 애니메이션이 내장 인지 여부.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDirectionItem")
	FString SoundFileName; // Wav 파일명

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDirectionItem")
	FString EffectFileName; // Effect 파일명 ( 확장자는 efk, 로컬 부모 폴더명 포함 예 : "Blow/Blow.efk" )

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDirectionItem")
	FRtTctComTransform EffectTransform; // Effect 트랜스폼

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDirectionItem")
	float EffectSpeed = 1.0f; // Effect 속도.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDirectionItem")
	int32 AnimFrameIndex_Sound = -1; // 애니메이션 프레임에 사운드 할당

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDirectionItem")
	int32 AnimFrameIndex_Effect = -1; // 애니메이션 프레임에 이펙트 할당
};

// 연출 컴포넌트
USTRUCT(BlueprintType, Category = "FRtTctComDirection")
struct FRtTctComDirection
{
	GENERATED_BODY()
public:
	FRtTctComDirection()
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComDirection")
	TArray<FRtTctComDirectionItem> DirectionItemList; // 연출 항목 리스트.
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

// 행동 컴포넌트
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
	ERtRoleType RoleType = ERtRoleType::ENone; // 역할 타입

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
	ERtSex Sex = ERtSex::ENone; // 성별

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComBehavior")
	ERtAgeRange AgeRange = ERtAgeRange::ENone; // 나이대

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComBehavior")
	ERtHeightRange HeightRange = ERtHeightRange::ENone; // 신장

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComBehavior")
	ERtPersonState PersonState = ERtPersonState::ENone; // 장애여부

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComBehavior")
	ERtWoundState WoundState = ERtWoundState::ENone; // 부상여부
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

// 부착 컴포넌트
USTRUCT(BlueprintType, Category = "FRtTctComEquip")
struct FRtTctComEquip
{
	GENERATED_BODY()
public:
	FRtTctComEquip()
	{
	}
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComEquip")
	FRtTctComSocket TrackerSocket; // 트래커 트랜스폼 정보

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComEquip")
	FRtTctComSocket LeftHandSocket; // 왼손 소켓 부착 상대 트랜스폼 정보

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComEquip")
	FRtTctComSocket RightHandSocket; // 오른손 소켓 부착 상대 트랜스폼 정보
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

// 차량 컴포넌트
USTRUCT(BlueprintType, Category = "FRtTctComCar")
struct FRtTctComCar
{
	GENERATED_BODY()
public:
	FRtTctComCar() {
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComCar")
	TMap<ERtCarAnimType, FString> CarAnimMap; // 현재는 이동 하나만 들어간다.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComCar")
//	TMap<FString, FRtTctComCarSeat> CarSeatMap; // Key : 좌석 이름, Data : Transform, ( 좌석 이름은 좌석01, 좌석02, .. 이렇게 자동으로 붙여짐 )
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
	FString SkyBoxFileName; // SkyBox Fbx 파일명.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComEnvironment")
	FRtTctComTransform SkyBoxTM; // SkyBox 트랜스폼

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComEnvironment")
	FString EnvironmentSoundFileName; // 환경 사운드 Wav 파일명. (예: 새들 지저귀는 소리, 자동차 경적 등등)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctComEnvironment")
	FString BackgroundMusicFileName; // 배경 음악 Wav 파일명. 
};

// 리뉴얼 TCT 메인 구조체
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
	int32 Version; // Tct 파일 버전.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctStruct")
	FRtTctComDefault ComDefault; // 기본 컴포넌트. (모든 타입에서 가지고 있다)

	// **주의**
	// 타입 별로 한 개만 있어도 되는 구조체들도 굳이 TArray에 넣어서 작성하는 이유는 
	// 현재 FRtTctStruct 구조체가 모든 Tct 타입을 포함 할 수 있는 데이터구조로 만들기 위함임
	// 이렇게 하면 사용되지 않는 컴포넌트의 경우 컨테이너 이름만 저장되므로 데이터 크기를 줄일 수 있음.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctStruct")
	TArray<FRtTctComCollision> ComCollisionList; // 충돌 컴포넌트 리스트. ( 멀티 ) 

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctStruct")
	TArray<FRtTctComDirection> ComDirectionList; // 연출 컴포넌트 리스트. ( 싱글 )

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctStruct")
	TArray<FRtTctComBehavior> ComBehaviorList; // 행동 컴포넌트 리스트. ( 싱글 )

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctStruct")
	TArray<FRtTctComEquip> ComEquipList; // 부착 컴포넌트 리스트. ( 싱글 )

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctStruct")
	TArray<FRtTctComCar> ComCarList; // 차량 컴포넌트 리스트. ( 싱글 )

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctStruct")
	TArray<FRtTctComEnvironment> ComEnvironment; // 환경 컴포넌트 리스트. ( 싱글 )
};

UCLASS()
class RTRESOURCEMODEL_API ARtTemplateModel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARtTemplateModel();

	UFUNCTION(BlueprintCallable, Category = "ARtTemplateModel")
	static bool LoadTctFile(FString tctFilePath, FRtTctStruct& targetData); // Tct 파일 로드.
	
	UFUNCTION(BlueprintCallable, Category = "ARtTemplateModel")
	static bool SaveTctFile(FString tctFilePath, const FRtTctStruct& sourceData); // Tct 파일 세이브.

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
