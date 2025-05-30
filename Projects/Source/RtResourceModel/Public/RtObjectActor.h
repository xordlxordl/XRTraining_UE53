// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "RtFBXLoader.h"
#include "Models/RtActorModel.h"


#include "Components\BoxComponent.h"
#include "Components\CapsuleComponent.h"
#include "Components\DecalComponent.h"

#include "RtDefines.h"
#include "RtObjectOptionComponent.h"

#include "RtObjectActor.generated.h"

class UNiagaraComponent;

DECLARE_MULTICAST_DELEGATE(FDelegate_SetVisible);

UCLASS()
class RTRESOURCEMODEL_API ARtObjectActor : public APawn//, public IRtObjectActorInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARtObjectActor();
	bool Init(TSharedPtr<RtActorModel> actorModel);
	bool InitRenew(TSharedPtr<FRtTctStruct> actorModel);

	UFUNCTION(BlueprintCallable)
	bool CreateTempModel();

	//void UpdatePositionInvisiblePickingStaticMesh();
	//bool SetInvisiblePickingStaticMesh();
	bool ConvertProceduralToStaticMesh();

	bool CreateModel_LoadSkeletalMesh(FString modelPath);
	bool CreateModel_LoadStaticMesh(FString modelPath, ERtProductType productType);

	UFUNCTION(BlueprintCallable)
	bool CreateModel(FString externalModelPath, ERtProductType productType);

	UFUNCTION(BlueprintCallable)
	bool CreateModelRenew(FString externalModelPath, ERtProductType productType);

	UFUNCTION(BlueprintCallable, Category = "glTFRuntimPlugIn")
	bool CreateModel_glTFRuntimeFBX(FString externalModelPath, ERtProductType productType);

	UFUNCTION(BlueprintCallable, Category = "glTFRuntimPlugIn")
	bool PlayGltfAssetAnimation(UglTFRuntimeAsset* animAsset, bool bLoop);

	UFUNCTION(BlueprintCallable, Category = "glTFRuntimPlugIn")
	bool AddExternalFBXAnimSequence(FString externalFbxPath, FString& Key, UAnimSequence*& animSeq);

	UFUNCTION(BlueprintCallable, Category = "glTFRuntimPlugIn")
	bool PlayExternalFBXAnimation(FString externalFbxPath, bool bLoop);

	UFUNCTION(BlueprintCallable, Category = "glTFRuntimPlugIn")
	bool PlayFBXSelfAnimation(int32 animIndex, bool bLoop);

	UFUNCTION(BlueprintCallable, Category = "glTFRuntimPlugIn")
	bool PlayFBXSelfAnimationByName(FString animName, bool bLoop);

public:

	UFUNCTION(BlueprintCallable)
	void SwitchCollision(bool isForGame);

	UFUNCTION(BlueprintCallable)
	bool ChangeModel(ERtModelKind modelKind, int index, bool forceChange = false);

	void ApplyComponentCollision(RtTctData* pTctData, bool isNoCollision, bool showBoxComponent = false);
	void ApplyComponentCollisionRenew(FRtTctStruct* pTctData, bool showBoxComponent = false);
	void ApplyComponentData(RtTctData* pTctData, bool isForGame);
	virtual void SetCollisionType(UShapeComponent* collisionComp, ERtCollisionType collisionType);

	void ApplyComponentDataRenew(FRtTctStruct* pTctData, bool isForGame);

	bool CreateProMesh(RtFbxProcedualMeshInfo* pProMeshInfo, bool enableCollision);

	bool SetMaterialProMesh();

	TSharedPtr<RtActorModel> GetActorModel()
	{
		return mActorModel;
	}

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetStaticMesh()
	{
		return mpStaticMeshComponent;
	}

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetSkeletalMesh()
	{
		return mpSkeletalMeshComponent;
	}

	UFUNCTION(BlueprintCallable)
	UProceduralMeshComponent* GetProcedualMesh()
	{
		return ProMeshComponent;
	}

	UFUNCTION(BlueprintCallable)
	UMeshComponent* GetCurrentMesh();

	bool GetFirstDirectionEffectTransform(FTransform& outTransform);

	UFUNCTION(BlueprintCallable)
	void SetProcedualMesh(UProceduralMeshComponent* proMeshCom)
	{
		ProMeshComponent = proMeshCom;
	}

	UFUNCTION(BlueprintCallable)
	bool IsStaticMesh()
	{
		if (mpStaticMeshComponent)
		{
			UStaticMesh* pStaticMesh = mpStaticMeshComponent->GetStaticMesh();
			return (nullptr != pStaticMesh);
		}

		return false;
	}

	UFUNCTION(BlueprintCallable)
	bool IsSkeletalMesh()
	{
		if (mpSkeletalMeshComponent)
		{
			USkeletalMesh* pSkeletalMesh = mpSkeletalMeshComponent->GetSkeletalMeshAsset();
			return (nullptr != pSkeletalMesh);
		}

		return false;
	}

	UFUNCTION(BlueprintCallable)
	void PlayAnimation(FString animationSequeceAnimName, bool bLooping, float animSpeed = -1.0f, bool bIsSystemDefault = false);

	UFUNCTION(BlueprintCallable)
	void PlayLegacyAnimation(FString animationSequecePath, bool bLooping);

	TPair<FString, float> GetAnimationProgress();
	bool IsLoopingAnim();

	UFUNCTION(BlueprintCallable)
	void StopAnimation();

	UFUNCTION(BlueprintCallable)
	void ChangeAnimationSpeedFromFactor(float speedFactor);

	UFUNCTION(BlueprintCallable)
	void ChangeAnimationSpeed(float speed);

	UFUNCTION(BlueprintCallable)
	bool AttachToNpc(ARtObjectActor* npc);

	// 이펙트 플레이 스탑 원형 --------------------------------------------------- //

	UFUNCTION(BlueprintCallable)
	void PlayEffect(FString effectSystemPath, FVector vStartPos);

	UFUNCTION(BlueprintCallable)
	void StopEffect();

	UFUNCTION(BlueprintCallable)
	void SetEffectArraySize(int arraySize);

	UFUNCTION(BlueprintCallable)
	void PlayNiagaraEffect(FString effectSystemPath, FVector spawnPos, FRotator spawnRot, float size);

	UFUNCTION(BlueprintCallable)
	void PlayEffectArray(FString effectSystemPath, int index, FVector vStartPos);

	UFUNCTION(BlueprintCallable)
	void StopEffectArray(int index);
	// ------------------------------------------------------------------------ //

	// efk 이펙트 --------------------------------------------------- //

	UFUNCTION(BlueprintCallable)
	bool PlayEfkEffect(FString& efkFilePath, bool bIsLoop);
	bool PlayEfkEffect(FString& efkFilePath, const FTransform& transform, const float& speed = 1.f, bool bIsLoop = false);

	UFUNCTION(BlueprintCallable)
	void StopEfkEffect();

	// ------------------------------------------------------------------------ //

	// 이펙트 플레이 스탑 응용 --------------------------------------------------- //

	UFUNCTION(BlueprintCallable)
	void PlaySingleEffect(ERtComponentType componentType, FVector vStartPos);

	UFUNCTION(BlueprintCallable)
	void StopSingleEffect()
	{
		StopEffect();
	}

	UFUNCTION(BlueprintCallable)
	void PlayArrayElementEffect(ERtComponentType componentType, int index, FVector vStartPos);

	UFUNCTION(BlueprintCallable)
	void StopArrayElementEffect(int index)
	{
		StopEffectArray(index);
	}

	// ------------------------------------------------------------------------ //

	RtTctData* GetTct()
	{
		if (mActorModel)
		{
			return mActorModel->GetTctData().Get();
		}

		return nullptr;
	}

	FRtTctStruct* GetTctStruct()
	{
		if (mActorModel)
		{
			return mActorModel->GetTctStruct().Get();
		}

		return nullptr;
	}

	UFUNCTION(BlueprintCallable)
	ERtTctType GetTctType()
	{
		if (mActorModel)
		{
			if (mActorModel->GetTctData().Get())
			{
				return mActorModel->GetTctData().Get()->mTctType;
			}
		}

		return ERtTctType::ENone;
	}

	UFUNCTION(BlueprintCallable)
	ERtTctType2 GetTctType2()
	{
		if (mActorModel)
		{
			if (mActorModel->GetTctStruct().Get())
			{
				return mActorModel->GetTctStruct().Get()->ComDefault.TctType;
			}
		}

		return ERtTctType2::ENone;
	}

	RtTctData::RtElementBase* GetElement(ERtComponentType comType, int index)
	{
		RtTctData* pData = GetTct();
		if (pData)
		{
			return pData->GetElement(comType, index);
		}

		return nullptr;
	}

	UFUNCTION(BlueprintCallable)
	bool IsThereComponent(ERtComponentType comType)
	{
		RtTctData* pData = GetTct();
		if (pData)
		{
			for (auto item : pData->mComponents)
			{
				if (item->mComType == comType)
				{
					return true;
				}
			}

			if (comType == ERtComponentType::EAnimationViewer)
			{
				if (pData->mTctType == ERtTctType::ECharacter ||
					pData->mTctType == ERtTctType::EEquipProp ||
					pData->mTctType == ERtTctType::EGun ||
					pData->mTctType == ERtTctType::EFireNozzle)
				{
					return true;
				}
			}
		}

		return false;
	}

	UFUNCTION(BlueprintCallable)
	int GetElementCount(ERtComponentType comType)
	{
		RtTctData* pData = GetTct();
		if (pData)
		{
			for (auto item : pData->mComponents)
			{
				if (item->mComType == comType)
				{
					return item->mElementList.Num();
				}
			}
		}

		return -1;
	}

	template<typename T>
	T* GetComponent(ERtComponentType comType)
	{
		RtTctData* pData = GetTct();
		if (pData)
		{
			T* pComData = StaticCast<T*>(pData->GetComponent(comType));
			IS_NOT_NULL(pComData)
			{
				return pComData;
			}
		}

		return nullptr;
	}

	template<typename T>
	T* GetComponentRenew(ERtComponentType comType)
	{
		FRtTctStruct pData = GetTctStruct();
		if (pData)
		{
			T* pComData = StaticCast<T*>(pData->GetComponentRenew(comType));
			IS_NOT_NULL(pComData)
			{
				return pComData;
			}
		}

		return nullptr;
	}

	template <typename T>
	T* GetElementEx(ERtComponentType comType, int index)
	{
		RtTctData::RtElementBase* pComEle = GetElement(comType, index);
		if (pComEle)
		{
			T* pComSomeEle = StaticCast<T*>(pComEle);
			return pComSomeEle;
		}
		return nullptr;
	}

	UFUNCTION(BlueprintCallable)
	FString GetFunctionAniFilePath()
	{
		RtTctData::RtComFunction* pComData = GetComponent<RtTctData::RtComFunction>(ERtComponentType::EFunction);
		if (pComData)
		{
			return pComData->mAniFilePath;
		}
		return FString();
	}

	UFUNCTION(BlueprintCallable)
	FString GetFunctionElementAniFilePath(int index)
	{
		auto pComEle = GetElementEx<RtTctData::RtComFunction::RtElement>(ERtComponentType::EFunction, index);
		if (pComEle)
		{
			return pComEle->mAniFilePath;
		}
		return FString();
	}

	UFUNCTION(BlueprintCallable)
	void SetSelectSwitch(bool selectOn)
	{
		TObjectPtr<UStaticMesh> staticMesh = mpStaticMeshComponent->GetStaticMesh();
		if (nullptr != staticMesh.Get())
		{
			mpStaticMeshComponent->SetRenderCustomDepth(selectOn);
		}
		else
		{
			const USkeletalMesh* skeletalMesh = mpSkeletalMeshComponent->GetSkeletalMeshAsset();
			if (nullptr != skeletalMesh)
			{
				mpSkeletalMeshComponent->SetRenderCustomDepth(selectOn);
			}
			else
			{
				ProMeshComponent->SetRenderCustomDepth(selectOn);
			}
		}

		bSelectOn = selectOn;
	}

	UFUNCTION(BlueprintCallable)
	bool GetSelectSwitch()
	{
		return bSelectOn;
	}

	UFUNCTION(BlueprintCallable)
	virtual void SetVisible(bool bNewVisible);
	bool IsVisible();

	FDelegate_SetVisible Delegate_SetVisible;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;


	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void AddOverlapedObjectName(FString objectName);
	void RemoveOverlapedObjectName(FString objectName);
	bool FindOverlapedObjectName(FString objectName);

	bool SetDecal_StartPointNotReady(bool isNotReady);

	UFUNCTION(BlueprintCallable)
	void SetStaticMeshVisible(bool bVisible);

	UFUNCTION(BlueprintCallable)
	void SetVisibleCollisionComponents(bool isVisible);

	UFUNCTION(BlueprintCallable)
	bool IsFBXMeshType();

	UFUNCTION(BlueprintCallable)
	USceneComponent* GetCurrentMeshComponentForTransform();

	TArray<UShapeComponent*> GetCollisions() { return Collisions; }
	TMap<UShapeComponent*, ERtCollisionType> GetCollisionMap() { return CollisionMap; }

protected:
	TSharedPtr<RtActorModel> mActorModel;
	TSharedPtr<FRtTctStruct> mActorModelRenew;

	ERtModelKind mCurrentModelKind;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* mpStaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* mpSkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* ProMeshComponent; // FBX 프로시절 메시 콤포넌트 데이터.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMeshComponent> CurrentMeshComponent; // 현재 모델을 담고 있는 컴포넌트. mpStaticMeshComponent, mpSkeletalMeshComponent, mpProMeshComponent 중에 하나.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DummyStaticComponent;

	// FBX 프로시절 메시 콤포넌트의 부모 컴포넌트.
	// MAX 에서 익스포트 되는 데이터의 구조가 언리얼과 같지 않아 로드 후 프로시절 매시에서 회전을 적용하고
	// Tct 자체 회전값은 부모 컴포넌트에서 적용하여 로컬 회전과 Tct 회전을 분리하기 위해 설치함.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> ProMeshParentComponent;

	UNiagaraComponent* mpSpawnedEffect = nullptr; // 기능 컴포넌트 요소용..
	TArray<TWeakObjectPtr<UNiagaraComponent>> mSpawnedEffectArray; // 이펙트, 기능 컴포넌트 요소용..( 두 컴포넌트가 만일 동시에 존재하게 되면 

	bool bSelectOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<FString> OverlapedObjectNameList; // 자신의 범위 안에 들어온 객체 이름들.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDecalComponent> Decal;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URtObjectOptionComponent* ObjectOptionComponent;

	UPROPERTY()
	TArray<UShapeComponent*> Collisions; // 리뉴얼 이전

	UPROPERTY()
	TMap<UShapeComponent*, ERtCollisionType> CollisionMap; // 리뉴얼 이후

	FRotator InitCurMeshRotation = FRotator::ZeroRotator;
public:
	FORCEINLINE FRotator GetInitCurMeshRotation() { return InitCurMeshRotation; }

	// [ gltfRuntimeFBX ]
public:
	UFUNCTION(BlueprintCallable)
	void SetGltfFBXAssetActor(class AglTFRuntimeFBXAssetActor* gltfFBXAssetActor);

	UFUNCTION(BlueprintCallable)
	void RemoveFBXAssetActor();

	// gltfRuntimeFBX 플러그인에서 생성한 FBX 모델 Actor.
	class AglTFRuntimeFBXAssetActor* CurrentGltfFBXAssetActor = nullptr;

	// [ 주의!! ]
	// 혹시 애니메이션 시퀀스 캐시를 외부로 빼서 같은 모델에 적용할 수 있을 것 같다는 상상은 버려야 한다.
	// 우선 같은 모델이라도 생성된 시점의 스켈레톤과 같은 모델만이 적용될 수 있다.
	// 그러므로 캐시는 객체 인스턴스 내에서만 존재할 수 있다. 새로운 스켈레톤이 생성될 때 마다 각자의
	// 애니메이션 시퀀스를 새롭게 만들어 주어야만 한다.

	// 외부 애니메이션 시퀀스 맵.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, TObjectPtr<UAnimSequence>> CurrentGltfFBXExternalAnimMap; // | Key : 애니메이션 Fbx 파일명 | Value : UAnimSequence |

	// FBX 파일 내부에 들어 있는 애니메이션에 대한 UAnimSequence* 리스트.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UAnimSequence>> CurrentGltfFBXSelfAnimArray;

	// 내부 애니메이션 시퀀스 색인 맵.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, int32> CurrentGltfFBXSelfAnimIndexMap; // | Key : FBX 내의 애니메이션 이름 | Value : CurrentGltfFBXSelfAnimArray 색인 |

	FString GetObjectName();

	FString CurrentAnimationFBXFileName; // 현재 플레이되는 애니메이션 FBX 파일명.

	bool bGunFired = false; // 총을 쏘았다.

protected:
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<class USoundWave*> SoundWaves;
	TObjectPtr<class UAudioComponent> AudioComponent;
	//TObjectPtr<class USoundAttenuation> SoundAttenuation;

public:
	void AddSoundWave(USoundWave* wave);
	void SetAttenuation(float volume, float AttenuationStart, float AttenuationEnd);
	void PlaySound(int32 audioIndex = 0);

};
