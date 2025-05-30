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

		// �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property Data")
		FString TemplateName;

	// �±�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property Data")
		FString TemplateTag;

	// ��Ű (MCT ������ Load �� �� �ʿ�)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property Data")
		FString TemplateModelKey;

	// �޽� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property Data")
		FString TemplateMeshPath;

	// ��Ƽ���� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property Data")
		FString TemplateMatPath;

	// ������
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
* ---------�浹 ������Ʈ-----------------------------------------------------
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


	// �浹 ������Ʈ ��� Ÿ��.
	//ECollisionCube = 1,
	//ECollisionCapsule = 2,
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collsion Data")
	ERtTctComponentEleType CollsionType = ERtTctComponentEleType::ECollisionCube;
};

USTRUCT(BlueprintType)
struct FTemplateCombution
{
	GENERATED_USTRUCT_BODY()
	// ���� ������Ʈ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		int32 Ability = 1; // 1 : ����, 2 : �ҿ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		float IgnitionPoint = 0; // ��ȭ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		float BurningTime = 0; // ���� �ð�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		float MaxTemperature = 0; // �ְ� �µ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combution Data")
		int32 BurningKind = 3; // ���� ���� , 3 : ���ؿ��� (����) , 4 : �Ҳɿ��� (����)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combution Data")
		int32 FireMaxSize = 0; // ȭ�� �ִ� ũ�� 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combution Data")
		int32 SmokeColor = 0; // ���� ���� , 5 : �� , 6 : ȸ , 7 : ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combution Data")
		int32 SmokeSize = 0; // ���� ũ��, �� 8: ��, 9 : �� . 10 : ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combution Data")
		FString BurningModelName; // ���� �� �� �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combution Data")
		FString BurnedModelName; // ���� �� �� �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combution Data")
		FString BurningModelPath; // ���� �� �� ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combution Data")
		FString BurnedModelPath; // ���� �� �� ���
};

USTRUCT(BlueprintType)
struct FTemplateEffect
{
	GENERATED_USTRUCT_BODY()
	// ����Ʈ ������Ʈ
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

	// ���� ������Ʈ
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

	// ��� ������Ʈ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		FString AnimationName; // �⺻���� �ִϸ��̼� �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		FString AnimationPath; // �⺻���� �ִϸ��̼� ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		FString EffectName; // �⺻���� ����Ʈ �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		FString EffectPath; // �⺻���� ����Ʈ ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")

		FVector EffectPos = FVector::Zero(); // �⺻���� ����Ʈ ��ġ 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		FString SoundName; // �⺻���� ���� �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		FString SoundPath; // �⺻ ���� ���� ��� 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		TArray<FString> OperModelName; // �۵� ���� �� �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		TArray<FString> OperModelPath; // �۵� ���� �� ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		TArray<FString> OperAniName; // �۵� ���� �ִϸ��̼� �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		TArray<FString> OperAniPath; // �۵� ���� �ִϸ��̼� ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		TArray<FString> OperEftName; // �۵� ���� ����Ʈ �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		TArray<FString> OperEftPath; // �۵� ���� ����Ʈ ��� 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		TArray<FVector> OperEftPos; // �۵� ���� ����Ʈ ��ġ 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		TArray<FString> OperSoundName; // �۵� ���� ���� �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
		TArray<FString> OperSoundPath; // �۵� ���� ���� ���
};

USTRUCT(BlueprintType)
struct FTemplateSpotLight
{
	GENERATED_USTRUCT_BODY()

	// ���� ����Ʈ ������Ʈ 
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

	// ����Ʈ ����Ʈ ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
	FVector PointPos = FVector::Zero();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
	float PointIntensity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
	float PointRadius = 0;
};

USTRUCT(BlueprintType)
struct FTemplateCharacter // �⺻���� TCT���� ������ , ���� MCT Data
{
	GENERATED_USTRUCT_BODY()

	// �⺻ ������Ƽ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTemplateBase BasicInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EquipObjectTemplateID = TEXT(""); // ���������� ������ ���� ��ü ���ø� ID
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EquipObjectID = TEXT(""); // ���� ��ü ID (������ ��ü�� ������ NULL, ������ ��ü�� ID(�̴� ������ ���Ͽ� �ٴ´�)) 
};

USTRUCT(BlueprintType)
struct FTemplateTile // �⺻���� TCT���� ������ , ���� MCT Data
{
	GENERATED_USTRUCT_BODY()

// ���� ����
	// �⺻ ������Ƽ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTemplateBase TileBasic;

	// ���� ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTemplateCombution TileCombution;

	// ��� ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTemplateFunction  TileFunction;



// �迭 ����
	// �浹 ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTemplateCollision> TileCollision;

	// ����Ʈ ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTemplateEffect> TileEffect;

	// ���� ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTemplateSound> TileSound;

	// ���� ����Ʈ ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTemplateSpotLight> TileSpot;

	// ����Ʈ ����Ʈ ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTemplatePointLight> TilePoint;
	
};


USTRUCT(BlueprintType)
struct FTemplateWall // �⺻���� TCT���� ������ , ���� MCT Data
{
	GENERATED_USTRUCT_BODY()

		// ���� ����
	// �⺻ ������Ƽ 
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTemplateBase WallBasic;

	// ���� ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTemplateCombution WallCombution;

	// ��� ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTemplateFunction  WallFunction;


	// �迭 ����
	// �浹 ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateCollision> WallCollision;

	// ����Ʈ ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateEffect> WallEffect;

	// ���� ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateSound> WallSound;

	// ���� ����Ʈ ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateSpotLight> WallSpot;

	// ����Ʈ ����Ʈ ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplatePointLight> WallPoint;


};

USTRUCT(BlueprintType)
struct FTemplateEquips // �⺻���� TCT���� ������ , ���� MCT Data
{
	GENERATED_USTRUCT_BODY()

	// ���� ����
	// �⺻ ������Ƽ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTemplateBase WeaponBasic;

	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 SocketIndex = 0;

	// �迭 ����
	// �浹 ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateCollision> WeaponCollision;

	// ����Ʈ ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateEffect> WeaponEffect;

	// ���� ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateSound> WeaponSound;

	// �߻�ü ���ǵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ShotSpeed = 0.0f;

	// �߻�ü �߻� ��ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector ShotFirePos = FVector::Zero();

	//// ���� ����Ʈ ������Ƽ
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TArray<FTemplateSpotLight> WeaponSpot;

	//// ����Ʈ ����Ʈ ������Ƽ
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TArray<FTemplatePointLight> WeaponPoint;

	// ���� ������Ʈ
	// �� �߻� ������Ʈ
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
	//FVector gGshotFirePos = FVector::Zero();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
	//float gGshotFireSpeed = 0;

	//// �� �л� ������Ʈ
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Template Data")
	//FVector gWSprayPos = FVector::Zero();

};

USTRUCT(BlueprintType)
struct FTemplateProps // �⺻���� TCT���� ������ , ���� MCT Data
{
	GENERATED_USTRUCT_BODY()


		// ���� ����
	// �⺻ ������Ƽ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTemplateBase PropBasic;

	// ���� ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTemplateCombution PropCombution;

	// ��� ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTemplateFunction  PropFunction;

	// �迭 ����
// �浹 ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateCollision> PropCollision;

	// ����Ʈ ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateEffect> PropEffect;

	// ���� ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateSound> PropSound;

	// ���� ����Ʈ ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateSpotLight> PropSpot;

	// ����Ʈ ����Ʈ ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplatePointLight> PropPoint;

	// ��� ������Ʈ ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTemplateEquips> PropEquip;


	
};

USTRUCT(BlueprintType)
struct FTemplateArea // �⺻���� TCT���� ������ , ���� MCT Data
{
	GENERATED_USTRUCT_BODY()

		// ���� ����
        // �⺻ ������Ƽ 
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTemplateBase AreaBasic;

	//
	//// �浹ü ���� 

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

	//// ���� �±�
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

	// �⺻ ������Ƽ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTemplateBase SpawnBasic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTemplateCollision> SpawnCollision;
};

USTRUCT(BlueprintType)
struct FTemplateWayPoint 
{
	GENERATED_USTRUCT_BODY()

	// �⺻ ������Ƽ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTemplateBase WayPointBasic;

};

USTRUCT(BlueprintType)
struct FTemplateInvisibleWall
{
	GENERATED_USTRUCT_BODY()

	// �⺻ ������Ƽ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTemplateBase InvisibleWallBasic;

};

USTRUCT(BlueprintType)
struct FTemplateTriggerBox
{
	GENERATED_USTRUCT_BODY()

	// �⺻ ������Ƽ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTemplateBase TriggerBoxBasic;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TCT �������� ���� �߰� ����ü��
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
	FString EquipObjectTemplateID = TEXT(""); // ���������� ������ ���� ��ü ���ø� ID

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EquipObjectID = TEXT(""); // ���� ��ü ID (������ ��ü�� ������ NULL, ������ ��ü�� ID(�̴� ������ ���Ͽ� �ٴ´�)) 
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
struct FMapTemplateFog // ExponentialHeightFog �ֿ� ������ ����ü
{
	GENERATED_USTRUCT_BODY()
public:
	FMapTemplateFog() {
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FogDensity = 0.02f; // �Ȱ� �е�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FogHeightFallOut = 0.2f; // �Ȱ� ���� �е�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor FogInscatteringColor = FColor(0, 0, 0, 0xFF); // �Ȱ� Į��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartDistance = 0.0f; // ���� �Ÿ� (cm ����)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Visible = false; // �Ȱ� ǥ�� ����.
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// MCT Data Struct
USTRUCT(BlueprintType)
struct FRtMCTTemplate {

	GENERATED_USTRUCT_BODY()
public:
	FRtMCTTemplate() {
		Version = 0; // �������� 0.
	}

	void SetVersion(int32 version) {
		Version = version;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Version;

	// map info
	// 2D �� ������ (���� ��� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D mapSize = FVector2D::Zero();

	// ���� ��� �������� �Ʒ��ڰ� �ٴ� �� �ִ� ���� ����.
	// ��ü ������ 6x6 �����̶�� ������ ��� ��� ������ ����Ѵٸ� ������ ����.
	// TrainingRangeStart.X = 0, TrainingRangeStart.Y = 0,
	// TrainingRangeSize.X = 6, TrainingRangeSize.Y = 6
	// ��ü ������ 6x6 �����̶�� ������ ��� Ư�� ������ ����Ѵٸ� ������ ����.
	// TrainingRangeStart.X = 1, TrainingRangeStart.Y = 1,
	// TrainingRangeSize.X = 3, TrainingRangeSize.Y = 3

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D TrainingRangeStart = FVector2D::Zero(); // ���� ��� ���� ������ �Ʒ��ڵ��� �ٴ� �� �ִ� ���� ����.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D TrainingRangeSize = FVector2D::Zero(); // ���� ��� ���� ������ �Ʒ��ڵ��� �ٴ� �� �ִ� ��� ����.	

	// �� ����� ���� ���� Volume Location
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ControlVolumePos =  FVector::Zero(); 

	// �� ����� ���� ���� Volume Extent
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ControlVolumeExtent = FVector::Zero();


	// ���̽�
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
// TCT �������� ���� �߰� ������ (���� Ÿ���� ���������� �ʴ´�.)
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
	FMapTemplateFog ExponentialHeightFogData; // ���� ������
};

// ����ġ ������ ����� ������ ���ۿ�.
USTRUCT(BlueprintType)
struct FRtTemplateWallInstallInfo 
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D MapSize = FVector2D::ZeroVector;

	// �⺻ ������Ƽ 
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
		
	// FRtScenarioData ��ü�� Json ���Ϸ� �����Ѵ�.
	//UFUNCTION(BlueprintCallable, Category = "Read Write Json")
	//static bool WriteCctDataToJsonFile(FString outputFilePath, FRtScenarioData cctData);
};
