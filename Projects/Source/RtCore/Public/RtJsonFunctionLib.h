// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RtEnums.h"

#include "RtJsonFunctionLib.generated.h"

class FJsonObject;

USTRUCT(BlueprintType)
struct FTemplateBase
{
	GENERATED_USTRUCT_BODY()

		// 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property Data")
		FString TemplateName;

	// 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property Data")
		FString TemplateTag;

	// 모델키 (MCT 내에서 Load 할 때 필요)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property Data")
		FString TemplateModelKey;

	// 메시 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property Data")
		FString TemplateMeshPath;

	// 머티리얼 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property Data")
		FString TemplateMatPath;

	// 내구도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property Data")
		float TemplateDurability = 100.0f;

	// Pos
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property Data")
		FVector TemplateLocation = FVector::Zero();

	// Rot 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property Data")
		FRotator TemplateRotation = FRotator::ZeroRotator;

	// Scale
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property Data")
		FVector TemplateScale = FVector::OneVector;

	// Hierachy Name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property Data")
		FString HierachyName;
};

USTRUCT(BlueprintType)
struct FTemplateCollision
{
	GENERATED_USTRUCT_BODY()
	/*
* ---------충돌 컴포넌트-----------------------------------------------------
*/
// BoxCollision Extent
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collsion Data")
	FVector CollExtent = FVector::Zero();

	// BoxCollision Pos
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collsion Data")
	FVector CollLocation = FVector::Zero();

	// BoxCollsion Rot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collsion Data")
	FRotator CollRotation = FRotator::ZeroRotator;

	// BoxCollsion Scale
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collsion Data")
	FVector CollScale = FVector::Zero();


	// 충돌 컴포넌트 요소 타입.
	//ECollisionCube = 1,
	//ECollisionCapsule = 2,
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collsion Data")
	ERtTctComponentEleType CollsionType = ERtTctComponentEleType::ECollisionCube;
};

USTRUCT(BlueprintType)
struct FTemplateCombution
{
	GENERATED_USTRUCT_BODY()
	// 연소 컴포넌트 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		int32 Ability = 1; // 1 : 가연, 2 : 불연

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		float IgnitionPoint = 0; // 발화점

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		float BurningTime = 0; // 연소 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		float MaxTemperature = 0; // 최고 온도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combution Data")
		int32 BurningKind = 3; // 연소 종류 , 3 : 분해연소 (유염) , 4 : 불꽃연소 (무염)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combution Data")
		int32 FireMaxSize = 0; // 화염 최대 크기 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combution Data")
		int32 SmokeColor = 0; // 연기 색상 , 5 : 흰 , 6 : 회 , 7 : 검

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combution Data")
		int32 SmokeSize = 0; // 연기 크기, 양 8: 소, 9 : 중 . 10 : 대

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combution Data")
		FString BurningModelName; // 연소 중 모델 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combution Data")
		FString BurnedModelName; // 연소 후 모델 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combution Data")
		FString BurningModelPath; // 연소 중 모델 경로

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combution Data")
		FString BurnedModelPath; // 연소 후 모델 경로
};

USTRUCT(BlueprintType)
struct FTemplateEffect
{
	GENERATED_USTRUCT_BODY()
	// 이펙트 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data")
		FString EffectName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data")
		FString EffectPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data")
		FVector EffectPos = FVector::Zero();
};

USTRUCT(BlueprintType)
struct FTemplateSound
{
	GENERATED_USTRUCT_BODY()

	// 사운드 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Data")
	FString SoundName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Data")
	FString SoundPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Data")
	int32 PlayOption = 1;
};

USTRUCT(BlueprintType)
struct FTemplateFunction
{
	GENERATED_USTRUCT_BODY()

	// 기능 컴포넌트 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		FString AnimationName; // 기본상태 애니메이션 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		FString AnimationPath; // 기본상태 애니메이션 경로

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		FString EffectName; // 기본상태 이펙트 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		FString EffectPath; // 기본상태 이펙트 경로
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")

		FVector EffectPos = FVector::Zero(); // 기본상태 이펙트 위치 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		FString SoundName; // 기본상태 사운드 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		FString SoundPath; // 기본 상태 사운드 경로 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		TArray<FString> OperModelName; // 작동 상태 모델 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		TArray<FString> OperModelPath; // 작동 상태 모델 경로

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		TArray<FString> OperAniName; // 작동 상태 애니메이션 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		TArray<FString> OperAniPath; // 작동 상태 애니메이션 경로

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		TArray<FString> OperEftName; // 작동 상태 이펙트 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		TArray<FString> OperEftPath; // 작동 상태 이펙트 경로 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		TArray<FVector> OperEftPos; // 작동 상태 이펙트 위치 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		TArray<FString> OperSoundName; // 작동 상태 사운드 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		TArray<FString> OperSoundPath; // 작동 상태 사운드 경로
};

USTRUCT(BlueprintType)
struct FTemplateSpotLight
{
	GENERATED_USTRUCT_BODY()

	// 스팟 라이트 컴포넌트 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
	FVector SpotPos = FVector::Zero();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
	FVector SpotDir = FVector::Zero();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
	float SpotInnerCone = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
	float SpotOuterCone = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
	float SpotIntensity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
	float SpotRadius = 0;


};

USTRUCT(BlueprintType)
struct FTemplatePointLight
{
	GENERATED_USTRUCT_BODY()

	// 포인트 라이트 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
	FVector PointPos = FVector::Zero();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
	float PointIntensity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
	float PointRadius = 0;
};

USTRUCT(BlueprintType)
struct FTemplateCharacter // 기본값은 TCT에서 가져옴 , 추후 MCT Data
{
	GENERATED_USTRUCT_BODY()

	// 기본 프로퍼티 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTemplateBase BasicInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EquipObjectTemplateID = TEXT(""); // 마지막으로 선택한 장착 객체 템플릿 ID
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EquipObjectID = TEXT(""); // 장착 객체 ID (장착될 객체가 없으면 NULL, 있으면 객체의 ID(이는 오른손 소켓에 붙는다)) 
};

USTRUCT(BlueprintType)
struct FTemplateTile // 기본값은 TCT에서 가져옴 , 추후 MCT Data
{
	GENERATED_USTRUCT_BODY()

// 단일 관리
	// 기본 프로퍼티 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTemplateBase TileBasic;

	// 연소 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTemplateCombution TileCombution;

	// 기능 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTemplateFunction  TileFunction;



// 배열 관리
	// 충돌 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTemplateCollision> TileCollision;

	// 이펙트 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTemplateEffect> TileEffect;

	// 사운드 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTemplateSound> TileSound;

	// 스팟 라이트 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTemplateSpotLight> TileSpot;

	// 포인트 라이트 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTemplatePointLight> TilePoint;
	
};


USTRUCT(BlueprintType)
struct FTemplateWall // 기본값은 TCT에서 가져옴 , 추후 MCT Data
{
	GENERATED_USTRUCT_BODY()

		// 단일 관리
	// 기본 프로퍼티 
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTemplateBase WallBasic;

	// 연소 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTemplateCombution WallCombution;

	// 기능 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTemplateFunction  WallFunction;


	// 배열 관리
	// 충돌 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateCollision> WallCollision;

	// 이펙트 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateEffect> WallEffect;

	// 사운드 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateSound> WallSound;

	// 스팟 라이트 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateSpotLight> WallSpot;

	// 포인트 라이트 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplatePointLight> WallPoint;


};

USTRUCT(BlueprintType)
struct FTemplateEquips // 기본값은 TCT에서 가져옴 , 추후 MCT Data
{
	GENERATED_USTRUCT_BODY()

	// 단일 관리
	// 기본 프로퍼티 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTemplateBase WeaponBasic;

	// 소켓 관리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 SocketIndex = 0;

	// 배열 관리
	// 충돌 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateCollision> WeaponCollision;

	// 이펙트 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateEffect> WeaponEffect;

	// 사운드 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateSound> WeaponSound;

	// 발사체 스피드
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ShotSpeed = 0.0f;

	// 발사체 발사 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector ShotFirePos = FVector::Zero();

	//// 스팟 라이트 프로퍼티
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TArray<FTemplateSpotLight> WeaponSpot;

	//// 포인트 라이트 프로퍼티
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TArray<FTemplatePointLight> WeaponPoint;

	// 고유 컴포넌트
	// 총 발사 컴포넌트
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
	//FVector gGshotFirePos = FVector::Zero();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
	//float gGshotFireSpeed = 0;

	//// 물 분사 컴포넌트
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
	//FVector gWSprayPos = FVector::Zero();

};

USTRUCT(BlueprintType)
struct FTemplateProps // 기본값은 TCT에서 가져옴 , 추후 MCT Data
{
	GENERATED_USTRUCT_BODY()


		// 단일 관리
	// 기본 프로퍼티 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTemplateBase PropBasic;

	// 연소 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTemplateCombution PropCombution;

	// 기능 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTemplateFunction  PropFunction;

	// 배열 관리
// 충돌 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateCollision> PropCollision;

	// 이펙트 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateEffect> PropEffect;

	// 사운드 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateSound> PropSound;

	// 스팟 라이트 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateSpotLight> PropSpot;

	// 포인트 라이트 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplatePointLight> PropPoint;

	// 장비 컴포넌트 프로퍼티
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateEquips> PropEquip;


	
};

USTRUCT(BlueprintType)
struct FTemplateArea // 기본값은 TCT에서 가져옴 , 추후 MCT Data
{
	GENERATED_USTRUCT_BODY()

		// 단일 관리
        // 기본 프로퍼티 
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTemplateBase AreaBasic;

	//
	//// 충돌체 관련 

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FVector PlayerStartCollPos = FVector(0,0,90);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FVector WayPointCollPos = FVector(0, 0, 30);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FVector AreaCollPos = FVector(0, 0, 40);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FVector PlayerStartCollExtent = FVector(22, 0, 44);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FVector WayPointCollExtent = FVector(32, 32, 32);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FVector AreaCollExtent = FVector(32, 32, 32);

	//// 영역 태그
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
	//	TArray<FString> aAreaTag;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
	//	FString PlayerTag;

	//

};

USTRUCT(BlueprintType)
struct FTemplatePlayerSpawn 
{
	GENERATED_USTRUCT_BODY()

	// 기본 프로퍼티 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTemplateBase SpawnBasic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTemplateCollision> SpawnCollision;
};

USTRUCT(BlueprintType)
struct FTemplateWayPoint 
{
	GENERATED_USTRUCT_BODY()

	// 기본 프로퍼티 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTemplateBase WayPointBasic;

};

USTRUCT(BlueprintType)
struct FTemplateInvisibleWall
{
	GENERATED_USTRUCT_BODY()

	// 기본 프로퍼티 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTemplateBase InvisibleWallBasic;

};

USTRUCT(BlueprintType)
struct FTemplateTriggerBox
{
	GENERATED_USTRUCT_BODY()

	// 기본 프로퍼티 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTemplateBase TriggerBoxBasic;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TCT 리뉴얼을 위한 추가 구조체들
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FMapTemplateNormal : public FTemplateBase
{
	GENERATED_USTRUCT_BODY()
public:
	FMapTemplateNormal() {}
};

USTRUCT(BlueprintType)
struct FMapTemplateCharacter : public FTemplateBase
{
	GENERATED_USTRUCT_BODY()
public:
	FMapTemplateCharacter() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EquipObjectTemplateID = TEXT(""); // 마지막으로 선택한 장착 객체 템플릿 ID

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EquipObjectID = TEXT(""); // 장착 객체 ID (장착될 객체가 없으면 NULL, 있으면 객체의 ID(이는 오른손 소켓에 붙는다)) 
};

USTRUCT(BlueprintType)
struct FMapTemplateEquipment : public FTemplateBase
{
	GENERATED_USTRUCT_BODY()
public:
	FMapTemplateEquipment() {}
};

USTRUCT(BlueprintType)
struct FMapTemplateCar : public FTemplateBase
{
	GENERATED_USTRUCT_BODY()
public:
	FMapTemplateCar() {}
};

USTRUCT(BlueprintType)
struct FMapTemplateEnvironment : public FTemplateBase
{
	GENERATED_USTRUCT_BODY()
public:
	FMapTemplateEnvironment() {}
};


USTRUCT(BlueprintType)
struct FMapTemplateFog // ExponentialHeightFog 주요 데이터 구조체
{
	GENERATED_USTRUCT_BODY()
public:
	FMapTemplateFog() {
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FogDensity = 0.02f; // 안개 밀도

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FogHeightFallOut = 0.2f; // 안개 높이 밀도

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor FogInscatteringColor = FColor(0, 0, 0, 0xFF); // 안개 칼라

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartDistance = 0.0f; // 시작 거리 (cm 단위)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Visible = false; // 안개 표시 여부.
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// MCT Data Struct
USTRUCT(BlueprintType)
struct FRtMCTTemplate {

	GENERATED_USTRUCT_BODY()
public:
	FRtMCTTemplate() {
		Version = 0; // 구버전은 0.
	}

	void SetVersion(int32 version) {
		Version = version;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Version;

	// map info
	// 2D 맵 사이즈 (실제 사용 면적)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D mapSize = FVector2D::Zero();

	// 실제 사용 면적에서 훈련자가 다닐 수 있는 영역 설정.
	// 전체 면적이 6x6 면적이라고 가정할 경우 모든 면적을 사용한다면 다음과 같다.
	// TrainingRangeStart.X = 0, TrainingRangeStart.Y = 0,
	// TrainingRangeSize.X = 6, TrainingRangeSize.Y = 6
	// 전체 면적이 6x6 면적이라고 가정할 경우 특정 면적을 사용한다면 다음과 같다.
	// TrainingRangeStart.X = 1, TrainingRangeStart.Y = 1,
	// TrainingRangeSize.X = 3, TrainingRangeSize.Y = 3

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D TrainingRangeStart = FVector2D::Zero(); // 실제 사용 면적 내에서 훈련자들이 다닐 수 있는 좌하 색인.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D TrainingRangeSize = FVector2D::Zero(); // 실제 사용 면적 내에서 훈련자들이 다닐 수 있는 우상 색인.	

	// 맵 사이즈에 따른 관제 Volume Location
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ControlVolumePos =  FVector::Zero(); 

	// 맵 사이즈에 따른 관제 Volume Extent
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ControlVolumeExtent = FVector::Zero();


	// 베이스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString baseMapPath;
	



	// BP_Template Path
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString bpCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString bpWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString bpTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString bpProps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString bpEquips;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString bpPlayerStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString bpWayPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString bpBoundary;

	// Simulator TemplatePath
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString bpRsGame_Char;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString bpRsGame_Wall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString bpRsGame_Tile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString bpRsGame_Props;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString bpRsGame_PlayerStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString bpRsGame_WayPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString bpRsGame_Area;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString bpRsGame_Equip;

	// prop mct sturct
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TArray<FTemplateProps> propsDatas;

	// wall mct struct
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TArray<FTemplateWall> wallsDatas;
	
	// tile mct struct
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TArray<FTemplateTile> tilesDatas;

	// weapon mct struct
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	    TArray<FTemplateEquips> EquipsDatas;

	// Area mct struct
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateArea> AreasDatas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateCharacter> CharacterDatas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTemplatePlayerSpawn> PlayerSpawnDatas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTemplateWayPoint> WayPointDatas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTemplateInvisibleWall> InvisibleWallDatas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTemplateTriggerBox> TriggerBoxDatas;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TCT 리뉴얼을 위한 추가 데이터 (기존 타입을 제거하지는 않는다.)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMapTemplateNormal> NormalTypeList;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMapTemplateCharacter> CharacterTypeList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMapTemplateEquipment> EquipmentTypeList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMapTemplateCar> CarTypeList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMapTemplateEnvironment> EnvironmentTypeList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMapTemplateFog ExponentialHeightFogData; // 포그 데이터
};

// 벽배치 툴에서 사용할 데이터 제작용.
USTRUCT(BlueprintType)
struct FRtTemplateWallInstallInfo 
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D MapSize = FVector2D::ZeroVector;

	// 기본 프로퍼티 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector2D> HorizontalWallInfoList;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector2D> VertivalWallInfoList;
};

/**
 * 
 */
UCLASS()
class RTCORE_API URtJsonFunctionLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	


public:




	/**
	* Open a json file, read its content and dump it into a structure
	* 
	* @param JsonFilePath            The path of your json file : "C:/Temp/MyJson.json"
	* @param bOutSuccess             If the action was a success or not
	* @param OutInfoMessage         More information about the action's result
	* 
	* @return The structure content of your json file
	*/
	UFUNCTION(BlueprintCallable, Category = "Read Write Json")
	static FRtMCTTemplate ReadStructFromJsonFile(FString JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage);



	/**
	* Convert your structure to a string, create a json file and dump the string in it
	* 
	* @param JsonFilePath         The path of your json file : "C:/Temp/MyJson.json"
	* @param Struct                The struct to want to write in the file
	* @param bOutSuccess          If the action was a success or not 
	* @param OutInfoMessage       Mere information about the action's result
	*/
	UFUNCTION(BlueprintCallable, Category = "Read Write Json")
	static void WriteStructToJsonFile(FString JsonFilePath, FRtMCTTemplate Struct, bool& bOutSuccess, FString& OutInfoMessage);


	/**
	* Open a json file, read its content and convert it to a JsonObject
	* 
	* @param JsonFilePath     the path of your json file : "C :/Temp/MyJson.json"
	* @param bOutSuccess      If the action was a success or not 
	* @param OutInfoMessage   More Information about the action's result
	* 
	* @return The JsonObject content of your json file
	
	*/
	static TSharedPtr<FJsonObject> ReadJson(FString JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage);

	/**
	 * Convert your JsonObject to a string, create a json file and dump the string in it 
	 * 
	 * @param JsonFilePath      the path of your json file : "C:/Temp/MyJson.json
	 * @param JsonObject        the JsonObject to want to write in the file
	 * @param bOutSuccess       If the action was a success or not 
	 * @param OutInfoMessage    More Information about the action's result
	 */
	static void WriteJson(FString JsonFilePath, TSharedPtr<FJsonObject> JsonObject, bool& bOutSucssess, FString& OutInfoMessage);


	static void ParseAsComponent();
		
	// FRtScenarioData 객체를 Json 파일로 저장한다.
	//UFUNCTION(BlueprintCallable, Category = "Read Write Json")
	//static bool WriteCctDataToJsonFile(FString outputFilePath, FRtScenarioData cctData);
};
