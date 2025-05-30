// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "RtFbxProcedualMeshInfo.h"

#include "RtEnums.h"
#include "RtDefines.h"
#include "RtJsonFunctionLib.h"

#include "RtActorModel2.generated.h"

class UPointLightComponent;
class USpotLightComponent;

USTRUCT(BlueprintType, Category = "FRtComObject")
struct FRtComObject
{
	GENERATED_BODY()

public:
	FRtComObject()
	{
	}
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComObject")
		FString ModelName; // Tct 모델 이름
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComObject")
		FString TagName; // Tct 태그 이름
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComObject")
		ERtTctType TctType = ERtTctType::ENone; // Tct 타입
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComObject")
		FString ModelFileName; // Tct 모델 파일 이름. (Fbx) 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComObject")
		FString ModelFilePath; // Tct 모델 파일 경로. (Fbx 파일 경로) : 툴 개발 전용.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComObject")
		int32 TileX = 0; // 이 모델이 차지할 타일 X 영역 크기.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComObject")
		int32 TileY = 0; // 이 모델이 차지할 타일 Y 영역 크기.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComObject")
		FRotator Rotator = FRotator(); // 회전 각도.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComObject")
		int32 Durability = 0; // 내구도.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComObject")
		FVector Position = FVector(0,0,0); // 위치
};

USTRUCT(BlueprintType, Category = "FRtComCollision")
struct FRtComCollision
{
	GENERATED_BODY()

public:
	FRtComCollision()
	{
	}
	FRtComCollision(ERtTctComponentEleType eleType, FVector pos, FVector size, FString name)
	{
		ElementType = eleType;
		Position = pos;
		Size = size;
		Name = name;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCollision")
		ERtTctComponentEleType ElementType = ERtTctComponentEleType::ENone;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCollision")
		FVector Position; // 위치
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCollision")
		FVector Size; // 크기
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCollision")
		FString Name; // 이름.
};

USTRUCT(BlueprintType, Category = "FRtComMaterial")
struct FRtComMaterial
{
	GENERATED_BODY()

public:
	FRtComMaterial(){}
	FRtComMaterial(FString materialFileName, FString materialFilePath)
	{
		MaterialFileName = materialFileName;
		MaterialFilePath = materialFilePath;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComMaterial")
		FString MaterialFileName; // 머티리얼 파일 이름.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComMaterial")
		FString MaterialFilePath; // 머티리얼 파일 경로.
};

USTRUCT(BlueprintType, Category = "FRtComAudio")
struct FRtComAudio
{
	GENERATED_BODY()

public:
	FRtComAudio()
	{
	}

	FRtComAudio(FString fileName, FString filePath, ERtComponentComboType playOption)
	{
		AudioFileName = fileName;
		AudioFilePath = filePath;
		PlayOption = playOption;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComAudio")
		FString AudioFileName;	// 오디오 이름.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComAudio")
		FString AudioFilePath; // 오디오 파일 경로.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComAudio")
		ERtComponentComboType PlayOption = ERtComponentComboType::ENone; // 재생 옵션.

};

USTRUCT(BlueprintType, Category = "FRtComEffect")
struct FRtComEffect
{
	GENERATED_BODY()

public:
	FRtComEffect()
	{
		Position = FVector::Zero();
	}
	FRtComEffect(FString eftFileName, FString eftFilePath, FTransform transform, float speed)
	{
		EffectFileName = eftFileName;
		EffectFilePath = eftFilePath;
		Position = transform.GetLocation();
		Rotation = transform.GetRotation().Rotator();
		Scale3D = transform.GetScale3D();
		Speed = speed;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComEffect")
	FString EffectFileName;	// 이펙트 이름.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComEffect")
	FString EffectFilePath; // 이펙트 파일 경로.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComEffect")
	FVector Position = FVector::ZeroVector; // 재생 위치.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComEffect")
	FRotator Rotation = FRotator::ZeroRotator; // 재생 각도

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComEffect")
	FVector Scale3D = FVector::ZeroVector; // 이펙트 크기

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComEffect")
	float Speed = 1.f; // 재생 속도
};

USTRUCT(BlueprintType, Category = "FRtComLight")
struct FRtComLight
{
	GENERATED_BODY()

public:
	FRtComLight()
	{
		Position = FVector::Zero();
		Direction = FVector::Zero();
	}
	FRtComLight(FVector pos, float intensity, float attenuationRadius)
	{
		Position = pos;
		Intensity = Intensity;
		AttenuationRadius = attenuationRadius;
	}
	FRtComLight(FVector pos, float intensity, float attenuationRadius, FVector direction, float innerConeAngle, float outerConeAngle)
	{
		Position = pos;
		Intensity = Intensity;
		AttenuationRadius = attenuationRadius;

		Direction = direction;
		InnerConeAngle = innerConeAngle;
		OuterConeAngle = outerConeAngle;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComLight")
	// 	ELightPoint	= 3,
	//  ELightSpot = 4,
	ERtTctComponentEleType ElementType = ERtTctComponentEleType::ENone;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComLight")
	FVector Position;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComLight")
	float Intensity;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComLight")
	float AttenuationRadius;

	// 스팟 라이트 전용 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComLight")
	FVector Direction; // 방향
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComLight")
	float InnerConeAngle; // 내부 원뿔 각도
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComLight")
	float OuterConeAngle; // 외부 원뿔 각도

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
};

USTRUCT(BlueprintType, Category = "FRtComFunctionElement")
struct FRtComFunctionElement {
	GENERATED_BODY()

public:
	FRtComFunctionElement() {
		EffectPos = FVector::Zero();
	}

	FRtComFunctionElement(FString modelFileName, FString modelFilePath, 
						FString aniFileName, FString aniFilePath, 
						FString effectFileName, FString effectFilePath, 
						FVector effectPos, FString soundFileName, FString soundFilePath)
	{
		ModelFileName = modelFileName;
		ModelFilePath = modelFilePath;
		AniFileName = aniFileName;
		AniFilePath = aniFilePath;
		EffectFileName = effectFileName;
		EffectFilePath = effectFilePath;
		EffectPos = effectPos;
		SoundFileName = soundFileName;
		SoundFilePath = soundFilePath;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
	FString ModelFileName; // 작동 모델 이름(fbx)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
	FString ModelFilePath; // 작동 모델 경로(fbx)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
	FString AniFileName; // 작동 모델 애니메이션 이름(fbx)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
	FString AniFilePath; // 작동 모델 애니메이션 경로(fbx)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
	FString EffectFileName; // 작동 모델 이펙트 이름(eft)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
	FString EffectFilePath; // 작동 모델 이펙트 경로(eft)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
	FVector EffectPos; // 작동 모델 이펙트 위치.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
	FString SoundFileName; // 작동 모델 사운드 이름(eft)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
	FString SoundFilePath; // 작동 모델 사운드 경로(eft)
};

USTRUCT(BlueprintType, Category = "FRtComFunction")
struct FRtComFunction
{
	GENERATED_BODY()
public:
	FRtComFunction()
	{
		EffectPos = FVector::Zero();
	}

	FRtComFunction(FString aniFileName, FString aniFilePath, FString effectFileName, FString effectFilePath, FVector effectPos, FString soundFileName, FString soundFilePath )
	{
		AniFileName = aniFileName;
		AniFilePath = aniFilePath;
		EffectFileName = effectFileName;
		EffectFilePath = effectFilePath;
		EffectPos = effectPos;
		SoundFileName = soundFileName;
		SoundFilePath = soundFilePath;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
		FString AniFileName; // 작동 모델 애니메이션 이름(fbx)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
		FString AniFilePath; // 작동 모델 애니메이션 경로(fbx)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
		FString EffectFileName; // 작동 모델 이펙트 이름(eft)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
		FString EffectFilePath; // 작동 모델 이펙트 경로(eft)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
		FVector EffectPos; // 작동 모델 이펙트 위치.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
		FString SoundFileName; // 작동 모델 사운드 이름(eft)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
		FString SoundFilePath; // 작동 모델 사운드 경로(eft)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctData")
		TArray<FRtComFunctionElement> FunctionElementList; // 작동 요소 리스트.
};

USTRUCT(BlueprintType, Category = "FRtComCombustion")
struct FRtComCombustion
{
	GENERATED_BODY()
public:
	FRtComCombustion() {
	}

	FRtComCombustion(ERtComponentComboType isBurning, float ignitionPoint, int32 burningHP, float burningMaxDeg, 
					 ERtComponentComboType burningKind, float flameMinSize, float flameMaxSize, float heatAbsorptionRate,
					 float heatDissipationRate, ERtComponentComboType smokeColor, ERtComponentComboType smokeSize, 
					FString burningModelName, FString burningModelPath, FString endBurningModelName, FString endBurningModelPath)
	{
		IsBurning = isBurning;
		IgnitionPoint = ignitionPoint;
		BurningHP = burningHP;
		BurningMaxDeg = burningMaxDeg;
		BurningKind = burningKind;
		FlameMinSize = flameMinSize;
		FlameMaxSize = flameMaxSize;
		HeatAbsorptionRate = heatAbsorptionRate;
		HeatDissipationRate = heatDissipationRate;
		SmokeColor = smokeColor;
		SmokeSize = smokeSize;
		BurningModelName = burningModelName;
		BurningModelPath = burningModelPath;
		EndBurningModelName = endBurningModelName;
		EndBurningModelPath = endBurningModelPath;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	ERtComponentComboType IsBurning; // 연소여부(연소 중/연소 전)
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	float IgnitionPoint;// 발화점(섭씨)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	int32 BurningHP;// 연소체력

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	float BurningMaxDeg;// 최고온도(섭씨)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	ERtComponentComboType BurningKind;// 연소종류 (유염/무염), 무염이면 화염크기, 연기색상, 연기양은 비활성화.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	float FlameMinSize;// 화염 최소 크기

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	float FlameMaxSize;// 화염 최대 크기

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	float HeatAbsorptionRate; // 열 흡수율 (0~1)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	float HeatDissipationRate; // 열 방출율 (0~1)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	ERtComponentComboType SmokeColor;// 연기색상

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	ERtComponentComboType SmokeSize;// 연기 양

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	FString BurningModelName;// 연소 중 모델

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	FString BurningModelPath;// 연소 중 모델 경로

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	FString EndBurningModelName;// 연소 종료 모델

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	FString EndBurningModelPath;// 연소 종료 모델 경로
};

USTRUCT(BlueprintType, Category = "FRtComGunFire")
struct FRtComGunFire
{
	GENERATED_BODY()
public:
	FRtComGunFire() {
		FireSpeed = 60; // 분당 60발 (Rounds per minute)
		FirePos = FVector::Zero();
	}
	FRtComGunFire(int32 fireSpeed, FVector firePos)
	{
		FireSpeed = fireSpeed;
		FirePos = firePos;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComGunFire")
		int32 FireSpeed; // 발사 속도.(연사력)(Rounds per minute)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComGunFire")
		FVector FirePos; // 발사 위치
};

USTRUCT(BlueprintType, Category = "FRtComWaterSpray")
struct FRtComWaterSpray
{
	GENERATED_BODY()

public:
	FRtComWaterSpray() {
		WaterSprayPos = FVector::Zero();
	}

	FRtComWaterSpray(FVector waterSprayPos) {
		WaterSprayPos = waterSprayPos;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComWaterSpray")
		FVector WaterSprayPos;
};

USTRUCT(BlueprintType, Category = "FRtComEquip")
struct FRtComEquip
{
	GENERATED_BODY()
public:
	FRtComEquip() {
		SocketType = ERtEquipComSocketComboType::ENone;
	}

	FRtComEquip(ERtEquipComSocketComboType socketType) {
		SocketType = socketType;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComEquip")
		ERtEquipComSocketComboType SocketType; 

};

// tct 메인 구조체 ( 레거시 구조체임 쓰면 안됨 )
USTRUCT(BlueprintType, Category = "FRtTctData")
struct FRtTctData // ( 레거시 구조체임 쓰면 안됨 )
{
	GENERATED_BODY()
public:
	FRtTctData()
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctData")
		FRtComObject ComObject;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctData")
		TArray<FRtComCollision> ComCollision;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctData")
		TArray<FRtComMaterial> ComMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctData")
		TArray<FRtComEffect> ComEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctData")
		TArray<FRtComAudio> ComAudio;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctData")
		TArray<FRtComLight> ComLight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctData")
		TArray<FRtComFunction> ComFunction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctData")
		TArray<FRtComCombustion> ComCombustion;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctData")
		TArray<FRtComWaterSpray> ComWaterSpray;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctData")
		TArray<FRtComGunFire> ComGunFire;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctData")
		TArray<FRtComEquip> ComEquip;
	
	bool LoadFromActorModel1(class RtActorModel* pActorModel);
};

USTRUCT(BlueprintType, Category = "FRtObjectSpawnData")
struct FRtObjectSpawnData
{
	GENERATED_BODY()
public:
	FRtObjectSpawnData()
	{
	}
	FRtObjectSpawnData(ERtMctSpawnType spawnType, FString modelKey, FString objectKeyName, 
		FVector location, FRotator rotation, FVector scale,
		FVector totalPosOffset, FRotator totalRotOffset, FMatrix totalRotationMatrix)
	{
		SpawnType = spawnType;
		ModelKey = modelKey;
		ObjectKeyName = objectKeyName;
		Location = location;
		Rotation = rotation;
		Scale = scale;
		TotalPosOffset = totalPosOffset;
		TotalRotOffset = totalRotOffset;
		TotalRotationMatrix = totalRotationMatrix;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtObjectSpawnData")
	ERtMctSpawnType SpawnType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtObjectSpawnData")
	FString ModelKey;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtObjectSpawnData")
	FString ObjectKeyName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtObjectSpawnData")
	FVector Location;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtObjectSpawnData")
	FRotator Rotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtObjectSpawnData")
	FVector Scale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtObjectSpawnData")
	FVector TotalPosOffset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtObjectSpawnData")
	FRotator TotalRotOffset;
		
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtObjectSpawnData")
	FMatrix TotalRotationMatrix;
};

/**
 * 
 */
UCLASS()
class RTRESOURCEMODEL_API URtActorModel2 : public UObject
{
	GENERATED_BODY()
public:
	URtActorModel2();
	~URtActorModel2();

	void InitEmptyData(ERtTctType tctType);

	bool LoadTCT(FString tctFilePath);
	bool SaveTCT(FString tctFilePath);

	bool LoadFromActorModel1(class RtActorModel* pActorModel);

	FRtTctData GetTctData(bool & bIsSuccess) {
		if( TctDataList.Num()>0 )
		{
			bIsSuccess = true;
			return TctDataList[0]; 
		}

		bIsSuccess = false;
		return FRtTctData();
	}

private:
	// FRtTctData TctData;
	// 삭제의 편의를 위해서 어레이를 쓸뿐 실제로는 한개의 데이터만 들어간다.
	TArray<FRtTctData> TctDataList;

	static TMap<ERtTctType, ERtComponentTypeInfo> ComInfoMap;

};
