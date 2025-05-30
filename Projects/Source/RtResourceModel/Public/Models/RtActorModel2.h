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
		FString ModelName; // Tct �� �̸�
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComObject")
		FString TagName; // Tct �±� �̸�
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComObject")
		ERtTctType TctType = ERtTctType::ENone; // Tct Ÿ��
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComObject")
		FString ModelFileName; // Tct �� ���� �̸�. (Fbx) 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComObject")
		FString ModelFilePath; // Tct �� ���� ���. (Fbx ���� ���) : �� ���� ����.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComObject")
		int32 TileX = 0; // �� ���� ������ Ÿ�� X ���� ũ��.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComObject")
		int32 TileY = 0; // �� ���� ������ Ÿ�� Y ���� ũ��.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComObject")
		FRotator Rotator = FRotator(); // ȸ�� ����.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComObject")
		int32 Durability = 0; // ������.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComObject")
		FVector Position = FVector(0,0,0); // ��ġ
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
		FVector Position; // ��ġ
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCollision")
		FVector Size; // ũ��
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCollision")
		FString Name; // �̸�.
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
		FString MaterialFileName; // ��Ƽ���� ���� �̸�.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComMaterial")
		FString MaterialFilePath; // ��Ƽ���� ���� ���.
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
		FString AudioFileName;	// ����� �̸�.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComAudio")
		FString AudioFilePath; // ����� ���� ���.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComAudio")
		ERtComponentComboType PlayOption = ERtComponentComboType::ENone; // ��� �ɼ�.

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
	FString EffectFileName;	// ����Ʈ �̸�.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComEffect")
	FString EffectFilePath; // ����Ʈ ���� ���.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComEffect")
	FVector Position = FVector::ZeroVector; // ��� ��ġ.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComEffect")
	FRotator Rotation = FRotator::ZeroRotator; // ��� ����

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComEffect")
	FVector Scale3D = FVector::ZeroVector; // ����Ʈ ũ��

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComEffect")
	float Speed = 1.f; // ��� �ӵ�
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

	// ���� ����Ʈ ���� ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComLight")
	FVector Direction; // ����
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComLight")
	float InnerConeAngle; // ���� ���� ����
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComLight")
	float OuterConeAngle; // �ܺ� ���� ����

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
	FString ModelFileName; // �۵� �� �̸�(fbx)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
	FString ModelFilePath; // �۵� �� ���(fbx)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
	FString AniFileName; // �۵� �� �ִϸ��̼� �̸�(fbx)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
	FString AniFilePath; // �۵� �� �ִϸ��̼� ���(fbx)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
	FString EffectFileName; // �۵� �� ����Ʈ �̸�(eft)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
	FString EffectFilePath; // �۵� �� ����Ʈ ���(eft)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
	FVector EffectPos; // �۵� �� ����Ʈ ��ġ.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
	FString SoundFileName; // �۵� �� ���� �̸�(eft)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
	FString SoundFilePath; // �۵� �� ���� ���(eft)
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
		FString AniFileName; // �۵� �� �ִϸ��̼� �̸�(fbx)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
		FString AniFilePath; // �۵� �� �ִϸ��̼� ���(fbx)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
		FString EffectFileName; // �۵� �� ����Ʈ �̸�(eft)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
		FString EffectFilePath; // �۵� �� ����Ʈ ���(eft)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
		FVector EffectPos; // �۵� �� ����Ʈ ��ġ.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
		FString SoundFileName; // �۵� �� ���� �̸�(eft)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComFunctionElement")
		FString SoundFilePath; // �۵� �� ���� ���(eft)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtTctData")
		TArray<FRtComFunctionElement> FunctionElementList; // �۵� ��� ����Ʈ.
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
	ERtComponentComboType IsBurning; // ���ҿ���(���� ��/���� ��)
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	float IgnitionPoint;// ��ȭ��(����)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	int32 BurningHP;// ����ü��

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	float BurningMaxDeg;// �ְ�µ�(����)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	ERtComponentComboType BurningKind;// �������� (����/����), �����̸� ȭ��ũ��, �������, ������� ��Ȱ��ȭ.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	float FlameMinSize;// ȭ�� �ּ� ũ��

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	float FlameMaxSize;// ȭ�� �ִ� ũ��

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	float HeatAbsorptionRate; // �� ����� (0~1)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	float HeatDissipationRate; // �� ������ (0~1)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	ERtComponentComboType SmokeColor;// �������

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	ERtComponentComboType SmokeSize;// ���� ��

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	FString BurningModelName;// ���� �� ��

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	FString BurningModelPath;// ���� �� �� ���

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	FString EndBurningModelName;// ���� ���� ��

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComCombustion")
	FString EndBurningModelPath;// ���� ���� �� ���
};

USTRUCT(BlueprintType, Category = "FRtComGunFire")
struct FRtComGunFire
{
	GENERATED_BODY()
public:
	FRtComGunFire() {
		FireSpeed = 60; // �д� 60�� (Rounds per minute)
		FirePos = FVector::Zero();
	}
	FRtComGunFire(int32 fireSpeed, FVector firePos)
	{
		FireSpeed = fireSpeed;
		FirePos = firePos;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComGunFire")
		int32 FireSpeed; // �߻� �ӵ�.(�����)(Rounds per minute)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FRtComGunFire")
		FVector FirePos; // �߻� ��ġ
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

// tct ���� ����ü ( ���Ž� ����ü�� ���� �ȵ� )
USTRUCT(BlueprintType, Category = "FRtTctData")
struct FRtTctData // ( ���Ž� ����ü�� ���� �ȵ� )
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
	// ������ ���Ǹ� ���ؼ� ��̸� ���� �����δ� �Ѱ��� �����͸� ����.
	TArray<FRtTctData> TctDataList;

	static TMap<ERtTctType, ERtComponentTypeInfo> ComInfoMap;

};
