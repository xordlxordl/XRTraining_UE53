// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtNpcPawn.h"

#include "RtReadWriteFile.h"

#include "RtDefines.h"
#include "RtTextureUtil.h"

#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

#include "NiagaraComponent.h"	
#include "NiagaraDataSetReadback.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraGpuComputeDispatchInterface.h"
#include "NiagaraComputeExecutionContext.h"
#include "NiagaraScript.h"
#include "NiagaraSystem.h"
#include "NiagaraWorldManager.h"
#include "NiagaraDataSetDebugAccessor.h"

#include "Blueprint\UserWidgetBlueprint.h"

// Sets default values
ARtNpcPawn::ARtNpcPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RtNpcPawn_Capsule"));
	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
	CapsuleComponent->SetCanEverAffectNavigation(false);
	CapsuleComponent->bDynamicObstacle = true;

	RootComponent = CapsuleComponent;

	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("RtNpcPawn_Mesh"));
	if (Mesh)
	{
		Mesh->AlwaysLoadOnClient = true;
		Mesh->AlwaysLoadOnServer = true;
		Mesh->bOwnerNoSee = false;
		Mesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
		Mesh->bCastDynamicShadow = true;
		Mesh->bAffectDynamicIndirectLighting = true;
		Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		//		Mesh->SetupAttachment(CapsuleComponent);
		static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
		Mesh->SetCollisionProfileName(MeshCollisionProfileName);
		Mesh->SetGenerateOverlapEvents(false);
		Mesh->SetCanEverAffectNavigation(false);

		Mesh->SetupAttachment(RootComponent);

		AddInstanceComponent(Mesh);
	}

	//ObjectActorComponent = CreateDefaultSubobject<URtObjectActorComponent>(TEXT("ObjectActorComponent"));
	ObjectOptionComponent = CreateDefaultSubobject<URtObjectOptionComponent>(TEXT("ObjectOptionCom"));
}

// Called when the game starts or when spawned
void ARtNpcPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ARtNpcPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARtNpcPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ARtNpcPawn::Init(TSharedPtr<RtActorModel> actorModel)
{
	if (actorModel.IsValid())
	{
		ActorModel = actorModel;
		return true;
	}

	return false;
}

bool ARtNpcPawn::CreateModel(FString externalModelPath, ERtProductType productType)
{
	bool retValue = false;

	if (ActorModel.IsValid())
	{
		TSharedPtr<RtTctData> pTctData = ActorModel->GetTctData();
		RtTctData::RtComCombustion* pComCoubu = static_cast<RtTctData::RtComCombustion*>(pTctData->GetComponent(ERtComponentType::ECombustion));

		if (!externalModelPath.IsEmpty())
		{
			int32 matCount = Mesh->GetMaterials().Num();
			for (int32 i = 0; i < matCount; i++) {
				Mesh->SetMaterial(i, nullptr);
			}

			if (externalModelPath == "None")
			{
				pTctData->mModelFilePath = "";
				pTctData->mModelFileName = "";

				Mesh->SetSkeletalMesh(nullptr);
				CurrentModelKind = ERtModelKind::EDefaultModel;

				return true;
			}
			else
			{
				pTctData->mModelFilePath = externalModelPath;
				pTctData->mModelFileName = FPaths::GetCleanFilename(externalModelPath);
			}
		}

		FString modelPath = pTctData->mModelFilePath;
		USkeletalMesh* pSkeletalMesh = URtReadWriteFile::LoadSkeletalFromPath(modelPath);
		if (pSkeletalMesh)
		{
			Mesh->SetMobility(EComponentMobility::Movable);
			Mesh->SetSkeletalMesh(pSkeletalMesh);
			Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			CurrentModelKind = ERtModelKind::EDefaultModel;

			retValue = true;
		}

		ApplyComponentData(pTctData.Get(), productType == ERtProductType::EGame);
	}

	return retValue;
}

void ARtNpcPawn::ApplyComponentData(RtTctData* pTctData, bool isForGame)
{
	if (isForGame) // 게임 전용..
	{
		// 충돌 컴포넌트 기능 적용.
		RtTctData::RtComCollision* pCollisionCom = GetComponent<RtTctData::RtComCollision>(ERtComponentType::ECollision);
		IS_NOT_NULL(pCollisionCom) {
			for (auto ele : pCollisionCom->mElementList)
			{
				auto* pColEle = static_cast<RtTctData::RtComCollision::RtElement*>(ele.Get());
				if (nullptr != pColEle)
				{
					if (pColEle->mComEleType == ERtTctComponentEleType::ECollisionCube)
					{
						UBoxComponent* pBoxCollision = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass());//, FName("ColBox"));

						pBoxCollision->SetupAttachment(RootComponent);

						pBoxCollision->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
						pBoxCollision->InitBoxExtent(FVector(pColEle->mSizeX * 0.5f, pColEle->mSizeY * 0.5f, pColEle->mSizeZ * 0.5f));
						pBoxCollision->SetRelativeLocation(FVector(pColEle->mPosX, pColEle->mPosY, pColEle->mPosZ + pColEle->mSizeZ * 0.5f));
						pBoxCollision->SetRelativeRotation(FRotator(pTctData->mRotX, pTctData->mRotY, pTctData->mRotZ));
						//pBoxCollision->SetCollisionObjectType(ECC_GameTraceChannel4);
						pBoxCollision->RegisterComponent();

						AddInstanceComponent(pBoxCollision);
					}
					else if (pColEle->mComEleType == ERtTctComponentEleType::ECollisionCapsule)
					{
						//UCapsuleComponent* pCapsuleCollision = NewObject<UCapsuleComponent>(this, UCapsuleComponent::StaticClass());

						//pCapsuleCollision->SetupAttachment(RootComponent);

						//pCapsuleCollision->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
						//pCapsuleCollision->InitCapsuleSize(pColEle->mSizeY * 0.5f, pColEle->mSizeX * 0.5f); // 인자 1 : 반경, 인자 2 : 절반 높이
						//pCapsuleCollision->SetRelativeLocation(FVector(pColEle->mPosX, pColEle->mPosY, pColEle->mPosZ));
						//pCapsuleCollision->SetRelativeRotation(FRotator(pTctData->mRotX, pTctData->mRotY, pTctData->mRotZ));

						CapsuleComponent->InitCapsuleSize(pColEle->mSizeY * 0.5f, pColEle->mSizeX * 0.5f);
						Mesh->SetRelativeLocation(FVector(0, 0, -pColEle->mPosZ));

						//						pCapsuleCollision->SetCollisionObjectType(ECC_GameTraceChannel4);
						//pCapsuleCollision->RegisterComponent();

						//AddInstanceComponent(pCapsuleCollision);
					}
				}
			}
		}

		// 머티리얼 컴포넌트 적용.
		RtTctData::RtComMaterial* pMaterialCom = GetComponent<RtTctData::RtComMaterial>(ERtComponentType::EMaterial);
		IS_NOT_NULL(pMaterialCom) {
			if (!pMaterialCom->mMaterialFilePath.IsEmpty())
			{
				FString filePath = pMaterialCom->mMaterialFilePath + "/" + pMaterialCom->mMaterialFileName;

				USkeletalMesh* skeletalMesh = Mesh->GetSkeletalMeshAsset();
				if (skeletalMesh)
				{
					if (skeletalMesh->GetMaterials().Num() > 0)
					{
						auto& Mat = skeletalMesh->GetMaterials();

						UMaterialInstanceDynamic* pMat = URtTextureUtil::CreateMaterialFromBaseTexture(Mat[0].MaterialInterface, filePath);

						Mat[0].MaterialInterface = pMat;
					}
				}

			}
		}

		// 이펙트 컴포넌트 적용.
		RtTctData::RtComEffect* pEffectCom = GetComponent<RtTctData::RtComEffect>(ERtComponentType::EEffect);
		IS_NOT_NULL(pEffectCom) {
			int effectElementCount = pEffectCom->GetElementCount();
			SetEffectArraySize(effectElementCount);
		}
	}

	// 라이트 컴포넌트 적용.
	RtTctData::RtComLight* pLightCom = GetComponent<RtTctData::RtComLight>(ERtComponentType::ELight);
	IS_NOT_NULL(pLightCom) {
		for (auto ele : pLightCom->mElementList)
		{
			if (ele->mComEleType == ERtTctComponentEleType::ELightPoint)
			{
				auto pPointEle = static_cast<RtTctData::RtComLight::RtElementPoint*>(ele.Get());
				if (nullptr != pPointEle)
				{
					UPointLightComponent* pPointLight = NewObject<UPointLightComponent>(this, UPointLightComponent::StaticClass());

					pPointLight->SetupAttachment(RootComponent);

					pPointLight->SetAttenuationRadius(pPointEle->mAttenuationRadius);
					pPointLight->SetIntensity(pPointEle->mIntensity);
					pPointLight->SetRelativeLocation(FVector(pPointEle->mPosX, pPointEle->mPosY, pPointEle->mPosZ));
					pPointLight->SetRelativeRotation(FRotator(pTctData->mRotX, pTctData->mRotY, pTctData->mRotZ));
					if (isForGame)
					{
						pPointLight->SetMobility(EComponentMobility::Stationary);
					}

					pPointLight->RegisterComponent();

					AddInstanceComponent(pPointLight);

					pPointEle->mpPointLight = pPointLight;
				}
			}
			else if (ele->mComEleType == ERtTctComponentEleType::ELightSpot)
			{
				auto pSpotEle = static_cast<RtTctData::RtComLight::RtElementSpot*>(ele.Get());
				if (nullptr != pSpotEle)
				{
					USpotLightComponent* pSpotLight = NewObject<USpotLightComponent>(this, USpotLightComponent::StaticClass());

					pSpotLight->SetupAttachment(RootComponent);

					pSpotLight->SetAttenuationRadius(pSpotEle->mAttenuationRadius);
					pSpotLight->SetIntensity(pSpotEle->mIntensity);
					pSpotLight->SetRelativeLocation(FVector(pSpotEle->mPosX, pSpotEle->mPosY, pSpotEle->mPosZ));
					pSpotLight->SetRelativeRotation(FRotator(pTctData->mRotX, pTctData->mRotY, pTctData->mRotZ));

					pSpotLight->SetRelativeRotation(FRotator(pSpotEle->mDirY, pSpotEle->mDirZ, pSpotEle->mDirX));
					pSpotLight->SetInnerConeAngle(pSpotEle->mInnerConeAngle);
					pSpotLight->SetOuterConeAngle(pSpotEle->mOuterConeAngle);
					if (isForGame)
					{
						pSpotLight->SetMobility(EComponentMobility::Stationary);
					}

					pSpotLight->RegisterComponent();

					AddInstanceComponent(pSpotLight);

					pSpotEle->mpSpotLight = pSpotLight;
				}
			}
		}
	}
}

void ARtNpcPawn::SetEffectArraySize(int newArraySize)
{
	int currentSize = SpawnedEffectArray.Num();

	if (currentSize == newArraySize) { // 사이즈가 같을 때..
		return;
	}
	else if (currentSize > newArraySize) // 사이즈가 줄어들 때..
	{
		for (int i = newArraySize; i < currentSize; i++)
		{
			UNiagaraComponent* NiagaraComponent = SpawnedEffectArray[i].Get();
			if (NiagaraComponent)
			{
				if (IsValid(NiagaraComponent))
				{
					NiagaraComponent->DestroyComponent();
				}

				SpawnedEffectArray[i] = nullptr;
			}
		}
	}

	SpawnedEffectArray.SetNum(newArraySize);
}

