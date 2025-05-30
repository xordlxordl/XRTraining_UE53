// Fill out your copyright notice in the Description page of Project Settings.


#include "RtObjectActor.h"

#include "RtReadWriteFile.h"

#include "RtDefines.h"
#include "RtTextureUtil.h"
#include "RtScenarioControllerBase.h"
#include "RtBluePrintCommonUtil.h"

#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/AudioComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"

#include "NiagaraComponent.h"	
#include "NiagaraDataSetReadback.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraGpuComputeDispatchInterface.h"
#include "NiagaraComputeExecutionContext.h"
#include "NiagaraScript.h"
#include "NiagaraSystem.h"
#include "NiagaraWorldManager.h"
#include "NiagaraDataSetDebugAccessor.h"

#include "ProceduralMeshConversion.h"

#include "RtResourceModelSubsystem.h"
#include "Animation/AnimSingleNodeInstance.h"

#include "glTFRuntimeFunctionLibrary.h"
#include "glTFRuntimeFBXFunctionLibrary.h"
#include "glTFRuntimeFBXAssetActor.h"

#include "Engine/SkeletalMeshSocket.h"
#include "EffekseerEmitterComponent.h"

#include "Misc/MessageDialog.h"

//#include "Blueprint\UserWidgetBlueprint.h"

// Sets default values

// ** 컴포넌트 이름 수정에 유의 바람 **
// 이 클래스를 기반으로 이미 파생된 블루프린트 객체의 경우에 컴포넌트 이름이 바뀔 경우 갱신되지 못하는 문제가 있음.
// 반드시 이름 변경이 필요할 경우 파생 블루프린트와 그것을 사용하는 모든 곳을 수정할 대비와 시간을 두고 진행해야 함!!
ARtObjectActor::ARtObjectActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	// !! 주의 !! : 이것은 더미 스테틱 컴포넌트로 직접 활용하면 안된다.
	// 루트 컴포넌트의 위치나 로테이션을 바꿀경우 아래 컴포넌트가 초기화 되는 이슈를 막기위한 
	// 일종의 버퍼 컴포넌트이므로 지우거나 활용을 하면 안된다. 이후에 나오는 컴포넌트만으로 이용하면 된다.
	DummyStaticComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyStatic"));
	DummyStaticComponent->SetupAttachment(RootComponent);
	DummyStaticComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	// FBX 모델 부모용. (TCT 트랜스폼 데이터 전용).
	ProMeshParentComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProMeshParent"));
	ProMeshParentComponent->SetupAttachment(RootComponent);

	// FBX 모델용. 익스포트 트랜스폼 전용. ( 모델링 툴 별 익스포트 특성 반영 )
	ProMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ARtObjectActorPro"));
	ProMeshComponent->SetupAttachment(ProMeshParentComponent);
	ProMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	mpStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ARtObjectActorStatic"));
	mpStaticMeshComponent->SetupAttachment(RootComponent);
	mpStaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	mpSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ARtObjectActorSkeletal"));
	mpSkeletalMeshComponent->SetupAttachment(RootComponent);
	//mpSkeletalMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);


	mCurrentModelKind = ERtModelKind::ENone;

	ObjectOptionComponent = CreateDefaultSubobject<URtObjectOptionComponent>(TEXT("ObjectOptionCom"));
	bSelectOn = false;

	//	static ConstructorHelpers::FObjectFinder<USkeleton> DefaultCharacterSkeletonObject(TEXT("/Game/RS/Common/Characters/Mannequin_UE4/Meshes/SK_Mannequin_Skeleton.SK_Mannequin_Skeleton"));
	//	static ConstructorHelpers::FObjectFinder<USkeleton> DefaultCharacterSkeletonObject(TEXT("/Game/CommonModels/SkeletalModels/Models/Default_Character/Mesh/UE4_Mannequin_Skeleton.UE4_Mannequin_Skeleton"));
	//	DefaultCharacterSkeleton = DefaultCharacterSkeletonObject.Object;

	SetReplicates(false);
}

bool ARtObjectActor::Init(TSharedPtr<RtActorModel> actorModel)
{
	if (actorModel.IsValid())
	{
		mActorModel = actorModel;
		return true;
	}

	return false;
}

bool ARtObjectActor::InitRenew(TSharedPtr<FRtTctStruct> actorModel)
{
	if (actorModel.IsValid())
	{
		mActorModelRenew = actorModel;
		return true;
	}

	return false;
}

bool ARtObjectActor::CreateTempModel()
{
	if (mActorModel.IsValid())
	{
		bool isFbxFile = false;

		TSharedPtr<FRtTctStruct> pTctStruct = mActorModel->GetTctStruct();
		if (pTctStruct.Get())
		{
			if (pTctStruct->ComDefault.TctType >= ERtTctType2::EPlayerSpawn)
			{

				FString modelPath = pTctStruct->ComDefault.ModelFileName;

				mpStaticMeshComponent->SetStaticMesh(nullptr);
				mpSkeletalMeshComponent->SetSkeletalMesh(nullptr);
				ProMeshComponent->ClearAllMeshSections();

				UStaticMesh* pStaticMesh = URtReadWriteFile::LoadStaticMeshFromPath(modelPath);
				if (nullptr != pStaticMesh)
				{
					mpStaticMeshComponent->SetStaticMesh(pStaticMesh);
					CurrentMeshComponent = mpStaticMeshComponent;
					mCurrentModelKind = ERtModelKind::EDefaultModel;
				}
				else
				{
					USkeletalMesh* pSkeletalMesh = URtReadWriteFile::LoadSkeletalFromPath(modelPath);
					if (nullptr != pSkeletalMesh)
					{
						mpSkeletalMeshComponent->SetSkeletalMesh(pSkeletalMesh);
						CurrentMeshComponent = mpSkeletalMeshComponent;
						mCurrentModelKind = ERtModelKind::EDefaultModel;
					}
				}
			}
			else
			{
				mCurrentModelKind = ERtModelKind::EDefaultModel;
				FString fbxFileName = pTctStruct->ComDefault.ModelFileName;
				FglTFRuntimeFBX_PreLoadInfo fbxInfo;
				FString fbxFilePath = FString::Printf(TEXT("%s/%s"), *URtResourceModelSubsystem::Get()->GetFbxModelPathBase(), *fbxFileName);
				bool fbxInfoOk = UglTFRuntimeFBXFunctionLibrary::GetFbxInfo(fbxFilePath, fbxInfo);
				if (fbxInfoOk)
				{
					if (!fbxInfo.bSkeletonMesh)
					{ // 스테틱 메시 FBX
						if (URtResourceModelSubsystem::Get()->LoadFbxFileToObjectActor(fbxFileName, this, false, fbxInfo))
						{
							CurrentMeshComponent = ProMeshComponent;
							isFbxFile = true;
						}
						else
						{
							return false;
						}
					}
					else
					{ // 스켈레톤 메시 FBX
						UglTFRuntimeAsset* gltfAsset = URtResourceModelSubsystem::Get()->LoadGltfAsset(fbxFilePath, this, false);
						UWorld* pWorld = GetWorld();
						AglTFRuntimeFBXAssetActor* glTFRuntimeFbxAssetActor = pWorld->SpawnActor<AglTFRuntimeFBXAssetActor>(
							AglTFRuntimeFBXAssetActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);

						if (gltfAsset && glTFRuntimeFbxAssetActor)
						{
							glTFRuntimeFbxAssetActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
							glTFRuntimeFbxAssetActor->Asset = gltfAsset;
							glTFRuntimeFbxAssetActor->ForceFBXLoadAsset();

							CurrentGltfFBXAssetActor = glTFRuntimeFbxAssetActor;

							TArray<UActorComponent*> SkeletalMeshComArr = CurrentGltfFBXAssetActor->K2_GetComponentsByClass(USkeletalMeshComponent::StaticClass());
							for (auto com : SkeletalMeshComArr)
							{
								USkeletalMeshComponent* skelMeshCom = Cast<USkeletalMeshComponent>(com);
								if (skelMeshCom)
								{
									CurrentMeshComponent = skelMeshCom;
									break;
								}
							}
						}
					}
				}
			}

		}
		else
		{

			TSharedPtr<RtTctData> pTctData = mActorModel->GetTctData();
			if (pTctData.Get())
			{

				FString modelPath = pTctData->mModelFilePath;

				mpStaticMeshComponent->SetStaticMesh(nullptr);
				mpSkeletalMeshComponent->SetSkeletalMesh(nullptr);
				ProMeshComponent->ClearAllMeshSections();

				UStaticMesh* pStaticMesh = URtReadWriteFile::LoadStaticMeshFromPath(modelPath);
				if (nullptr != pStaticMesh)
				{
					mpStaticMeshComponent->SetStaticMesh(pStaticMesh);
					CurrentMeshComponent = mpStaticMeshComponent;
					mCurrentModelKind = ERtModelKind::EDefaultModel;
				}
				else
				{
					USkeletalMesh* pSkeletalMesh = URtReadWriteFile::LoadSkeletalFromPath(modelPath);
					if (nullptr != pSkeletalMesh)
					{
						mpSkeletalMeshComponent->SetSkeletalMesh(pSkeletalMesh);
						CurrentMeshComponent = mpSkeletalMeshComponent;
						mCurrentModelKind = ERtModelKind::EDefaultModel;
					}
					else
					{
						mCurrentModelKind = ERtModelKind::EDefaultModel;
						FString fbxFileName = mActorModel->GetTctData().Get()->mModelFileName;
						FglTFRuntimeFBX_PreLoadInfo fbxInfo;
						FString fbxFilePath = FString::Printf(TEXT("%s/%s"), *URtResourceModelSubsystem::Get()->GetFbxModelPathBase(), *fbxFileName);
						bool fbxInfoOk = UglTFRuntimeFBXFunctionLibrary::GetFbxInfo(fbxFilePath, fbxInfo);
						if (fbxInfoOk)
						{
							if (!fbxInfo.bSkeletonMesh)
							{ // 스테틱 메시 FBX
								if (URtResourceModelSubsystem::Get()->LoadFbxFileToObjectActor(fbxFileName, this, false, fbxInfo))
								{
									CurrentMeshComponent = ProMeshComponent;
									isFbxFile = true;
								}
								else
								{
									return false;
								}
							}
							else
							{ // 스켈레톤 메시 FBX
								UglTFRuntimeAsset* gltfAsset = URtResourceModelSubsystem::Get()->LoadGltfAsset(modelPath, this, false);
								UWorld* pWorld = GetWorld();
								AglTFRuntimeFBXAssetActor* glTFRuntimeFbxAssetActor = pWorld->SpawnActor<AglTFRuntimeFBXAssetActor>(
									AglTFRuntimeFBXAssetActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);

								if (gltfAsset && glTFRuntimeFbxAssetActor)
								{
									glTFRuntimeFbxAssetActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
									glTFRuntimeFbxAssetActor->Asset = gltfAsset;
									glTFRuntimeFbxAssetActor->ForceFBXLoadAsset();

									CurrentGltfFBXAssetActor = glTFRuntimeFbxAssetActor;

									TArray<UActorComponent*> SkeletalMeshComArr = CurrentGltfFBXAssetActor->K2_GetComponentsByClass(USkeletalMeshComponent::StaticClass());
									for (auto com : SkeletalMeshComArr)
									{
										USkeletalMeshComponent* skelMeshCom = Cast<USkeletalMeshComponent>(com);
										if (skelMeshCom)
										{
											CurrentMeshComponent = skelMeshCom;
											break;
										}
									}
								}
							}
						}
					}
				}

				if (CurrentMeshComponent)
				{
					CurrentMeshComponent->SetMobility(EComponentMobility::Movable);
					CurrentMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

					if (!(pTctData->mTctType == ERtTctType::EEquipProp ||
						pTctData->mTctType == ERtTctType::EGun ||
						pTctData->mTctType == ERtTctType::EFireNozzle))
					{
						// TCT 자체 위치 및 회전 처리
						USceneComponent* curMeshCom = GetCurrentMeshComponentForTransform();
						if (curMeshCom)
						{
							curMeshCom->SetRelativeLocation(FVector(pTctData->mPosX, pTctData->mPosY, pTctData->mPosZ));
							curMeshCom->SetRelativeRotation(FRotator(pTctData->mRotY, pTctData->mRotZ, pTctData->mRotX));
						}
					}
				}
			}
		}
	}

	return (CurrentMeshComponent != nullptr);
}

//void ARtObjectActor::UpdatePositionInvisiblePickingStaticMesh()
//{ 
//	RtTctData* pTctData = GetTct();
//	int32 colCount = pTctData->GetElementCount(ERtComponentType::ECollision);
//	if (colCount > 0)
//	{
//		if (mpStaticMeshComponent && CurrentMeshComponent)
//		{
//			USceneComponent* curMeshCom = GetCurrentMeshComponentForTransform();
//			if (curMeshCom) {
//				FTransform tmRoot = curMeshCom->GetComponentTransform();
//				FRotator rotRoot = tmRoot.GetRotation().Rotator();
//
//				FTransform tm = mpStaticMeshComponent->GetComponentTransform();
//
//				RtComCollisionElement* pElememt = pTctData->GetElementEx<RtComCollisionElement>(ERtComponentType::ECollision, 0);
//				if (pElememt)
//				{
//					FVector posMod = FVector(pElememt->mPosX, pElememt->mPosY, pElememt->mPosZ);
//					posMod = rotRoot.RotateVector(posMod);
//					tm.AddToTranslation(posMod);
//					mpStaticMeshComponent->SetWorldTransform(tm);
//				}
//			}
//		}
//	}
//}

// 
// << 현재까지 진행된 R&D 내용 정리 >>
// 현재 프로시절 매시는 LineTraceSingle 함수에서 Hit 판정이 되지 않음.
// 프로시절 매시를 기반으로 StaticMesh 를 만드는 것이 가능하지만
// 에디터 모드상의 실행은 잘되지만 빌드 버전에서는 LineTraceSingle 함수에서 역시 Hit 판정이 되지 않음.
// 다양한 방법을 시도 해 보았지만 모두 실패함.
// 
// 대안 : 
//	대안으로 임베딩된 박스를 로드해서 박스의 사이즈와 위치를 충돌 컴포넌트의 값으로 대치하여 사용하면 된다.
// 

//bool ARtObjectActor::SetInvisiblePickingStaticMesh()
//{
//	bool bDebugCollideBoxView = false;
//
//	RtTctData* pTctData = GetTct();
//	int32 colCount = pTctData->GetElementCount(ERtComponentType::ECollision);
//	if (colCount > 0)
//	{
//		UProceduralMeshComponent* ProcMeshComp = ProMeshComponent;
//		if (ProcMeshComp != nullptr)
//		{
//			ProcMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 컬리전 활성화
//			ProcMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block); // 모든 충돌 채널에 대해 블록
//			FString LineTraceBoxPath = FString(TEXT("/Game/CommonModels/StaticModels/LineTraceBox.LineTraceBox"));
//			UStaticMesh* staticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *LineTraceBoxPath));
//
//			if (mpStaticMeshComponent->SetStaticMesh(staticMesh))
//			{
//				float boxSizeMod = 0.01f;
//				FTransform tm = mpStaticMeshComponent->GetComponentTransform();
//				RtComCollisionElement* pElememt = pTctData->GetElementEx<RtComCollisionElement>(ERtComponentType::ECollision, 0);
//				if (pElememt)
//				{
//					//						tm.AddToTranslation(FVector(pElememt->mPosX, pElememt->mPosY, pElememt->mPosZ));
//					tm.SetScale3D(FVector(pElememt->mSizeX * boxSizeMod, pElememt->mSizeY * boxSizeMod, pElememt->mSizeZ * boxSizeMod));
//					mpStaticMeshComponent->SetWorldTransform(tm);
//				}
//
//				mpStaticMeshComponent->SetVisibility(bDebugCollideBoxView);
//			}
//
//			return true;
//		}
//	}
//
//	return false;
//}

bool ARtObjectActor::ConvertProceduralToStaticMesh()
{
	return false;
}

bool ARtObjectActor::CreateModel_LoadSkeletalMesh(FString modelPath)
{
	if (modelPath.IsEmpty())
		return false;

	USkeletalMesh* pSkeletalMesh = URtReadWriteFile::LoadSkeletalFromPath(modelPath);
	if (nullptr != pSkeletalMesh)
	{
		mpSkeletalMeshComponent->SetSkeletalMesh(pSkeletalMesh);
		mpSkeletalMeshComponent->SetMobility(EComponentMobility::Movable);
		CurrentMeshComponent = mpSkeletalMeshComponent;

		mCurrentModelKind = ERtModelKind::EDefaultModel;
	}
	else
	{
		return false;
	}

	return true;
}

bool ARtObjectActor::CreateModel_LoadStaticMesh(FString modelPath, ERtProductType productType)
{
	if (modelPath.IsEmpty())
		return false;

	UStaticMesh* pStaticMesh = URtReadWriteFile::LoadStaticMeshFromPath(modelPath);
	if (nullptr != pStaticMesh)
	{
		mpStaticMeshComponent->SetStaticMesh(pStaticMesh);
		mpStaticMeshComponent->SetMobility((productType == ERtProductType::EGame) ? EComponentMobility::Static : EComponentMobility::Movable);
		mpStaticMeshComponent->SetVisibility(true);
		CurrentMeshComponent = mpStaticMeshComponent;

		mCurrentModelKind = ERtModelKind::EDefaultModel;
	}
	else
	{
		return false;
	}

	return true;
}

bool ARtObjectActor::CreateModel(FString externalModelPath, ERtProductType productType)
{
	if (mActorModel.IsValid())
	{
		TSharedPtr<FRtTctStruct> pTctStruct = mActorModel->GetTctStruct();
		if (pTctStruct.Get())
		{
			return CreateModelRenew(externalModelPath, productType);
		}
		else
		{

			TSharedPtr<RtTctData> pTctData = mActorModel->GetTctData();
			if (!pTctData.Get())
			{
				return false;
			}

			RtTctData::RtComCombustion* pComCoubu = static_cast<RtTctData::RtComCombustion*>(pTctData->GetComponent(ERtComponentType::ECombustion));

			if (!externalModelPath.IsEmpty())
			{
				int32 matCount = mpStaticMeshComponent->GetMaterials().Num();
				for (int32 i = 0; i < matCount; i++)
				{
					mpStaticMeshComponent->SetMaterial(i, nullptr);
				}
				matCount = mpSkeletalMeshComponent->GetMaterials().Num();
				for (int32 i = 0; i < matCount; i++)
				{
					mpSkeletalMeshComponent->SetMaterial(i, nullptr);
				}
				matCount = ProMeshComponent->GetMaterials().Num();
				for (int32 i = 0; i < matCount; i++)
				{
					ProMeshComponent->SetMaterial(i, nullptr);
				}

				if (externalModelPath == "None")
				{
					pTctData->mModelFilePath = "";
					pTctData->mModelFileName = "";

					mpStaticMeshComponent->SetStaticMesh(nullptr);
					mpSkeletalMeshComponent->SetSkeletalMesh(nullptr);
					ProMeshComponent->ClearAllMeshSections();

					//mCurrentModelKind = ERtModelKind::EDefaultModel;

					return true;
				}
				else
				{
					pTctData->mModelFilePath = externalModelPath;
					pTctData->mModelFileName = FPaths::GetCleanFilename(externalModelPath);
				}
			}
			else
			{
				if (URtResourceModelSubsystem::Get())
				{
					FString fbxFileName = pTctData->mModelFileName;
					FString fbxModelPathBase = URtResourceModelSubsystem::Get()->GetFbxModelPathBase();
					externalModelPath = FString::Printf(TEXT("%s/%s"), *fbxModelPathBase, *fbxFileName);
				}
			}

			FString modelPath = pTctData->mModelFilePath;

			mpStaticMeshComponent->SetStaticMesh(nullptr);
			mpSkeletalMeshComponent->SetSkeletalMesh(nullptr);
			ProMeshComponent->ClearAllMeshSections();

			bool isFbxFile = false;
			mCurrentModelKind = ERtModelKind::EDefaultModel;

			bool isSystemData = (mActorModel->GetTctData().Get()->mTctType >= ERtTctType::EPlayerSpawn);
			FString fbxFileName = mActorModel->GetTctData().Get()->mModelFileName;
			bool isFbxFileType = fbxFileName.Contains(TEXT("fbx"));

			RemoveFBXAssetActor();

			if (!isSystemData && isFbxFileType)
			{
				FglTFRuntimeFBX_PreLoadInfo fbxInfo;
				bool fbxInfoOk = UglTFRuntimeFBXFunctionLibrary::GetFbxInfo(externalModelPath, fbxInfo);
				if (fbxInfoOk)
				{
					if (!fbxInfo.bSkeletonMesh)
					{ // 스테틱 메시 FBX
						if (URtResourceModelSubsystem::Get() &&
							URtResourceModelSubsystem::Get()->LoadFbxFileToObjectActor(fbxFileName, this, productType == ERtProductType::EGame, fbxInfo))
						{
							ProMeshComponent->SetMobility(EComponentMobility::Movable);
							CurrentMeshComponent = ProMeshComponent;

							isFbxFile = true;
						}
					}
					else
					{ // 스켈레톤 메시 FBX
						UglTFRuntimeAsset* gltfAsset = URtResourceModelSubsystem::Get()->LoadGltfAsset(externalModelPath, this, productType == ERtProductType::EGame);
						UWorld* pWorld = GetWorld();
						AglTFRuntimeFBXAssetActor* glTFRuntimeFbxAssetActor = pWorld->SpawnActor<AglTFRuntimeFBXAssetActor>(
							AglTFRuntimeFBXAssetActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);

						if (gltfAsset && glTFRuntimeFbxAssetActor)
						{
							glTFRuntimeFbxAssetActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
							glTFRuntimeFbxAssetActor->Asset = gltfAsset;
							glTFRuntimeFbxAssetActor->ForceFBXLoadAsset();

							CurrentGltfFBXAssetActor = glTFRuntimeFbxAssetActor;

							TArray<UActorComponent*> SkeletalMeshComArr = CurrentGltfFBXAssetActor->K2_GetComponentsByClass(USkeletalMeshComponent::StaticClass());
							for (auto com : SkeletalMeshComArr)
							{
								USkeletalMeshComponent* skelMeshCom = Cast<USkeletalMeshComponent>(com);
								if (skelMeshCom)
								{
									CurrentMeshComponent = skelMeshCom;
									break;
								}
							}

							if (CurrentMeshComponent)
							{
								TArray<FglTFRuntimeFBXAnim> fbxAnimArray = CurrentGltfFBXAssetActor->GetFBXAnimations();
								USkeletalMeshComponent* skelMeshCom = Cast<USkeletalMeshComponent>(CurrentMeshComponent);
								if (skelMeshCom)
								{
									CurrentGltfFBXSelfAnimArray.Empty();
									CurrentGltfFBXSelfAnimIndexMap.Empty();
									int count = 0;
									for (const auto& fbxAnim : fbxAnimArray)
									{
										UAnimSequence* animSeq = CurrentGltfFBXAssetActor->LoadFBXAnimation(skelMeshCom, fbxAnim);
										CurrentGltfFBXSelfAnimArray.Add(animSeq);
										CurrentGltfFBXSelfAnimIndexMap.Add(fbxAnim.Name, count++);
									}

									struct FDynamicSocket
									{
										FName SocketName;
										FName BoneName;
										FDynamicSocket(FName socketName, FName boneName)
										{
											SocketName = socketName;
											BoneName = boneName;
										}
									};

									// 동적으로 소켓 추가하기.
									TArray<FDynamicSocket> registSocketList = {
										FDynamicSocket(FName(R_HAND_SOCKET_NAME), FName(RIGHT_HAND_BONE_NAME)), // 오른손에 동적으로 소켓을 추가한다.
										FDynamicSocket(FName(MOUTH_SOCKET_NAME), FName(HEAD_BONE_NAME)), // 입에 동적으로 소켓을 추가한다.
										FDynamicSocket(FName(L_HAND_SOCKET_NAME), FName(LEFT_HAND_BONE_NAME)) // 왼손에 동적으로 소켓을 추가한다.										
									};

									if (skelMeshCom->GetSkeletalMeshAsset())
									{
										USkeleton* skeleton = skelMeshCom->GetSkeletalMeshAsset()->GetSkeleton();
										if (skeleton)
										{
											for (auto& item : registSocketList)
											{
												USkeletalMeshSocket* SkeletalSocket = NewObject<USkeletalMeshSocket>(skeleton);
												if (SkeletalSocket)
												{
													SkeletalSocket->SocketName = item.SocketName;
													SkeletalSocket->BoneName = item.BoneName;
													SkeletalSocket->RelativeLocation = FVector::ZeroVector;
													SkeletalSocket->RelativeRotation = FRotator::ZeroRotator;
													SkeletalSocket->RelativeScale = FVector::OneVector;
													skeleton->Sockets.Add(SkeletalSocket);
												}
											}

											skelMeshCom->GetSkeletalMeshAsset()->RebuildSocketMap(); // 이것은 빌드 버전에서 반드시 필요한 코드다.
										}
									}
								}
							}
						}
					}
				}
			}
			else
			{
				if (modelPath.Contains(TEXT("SkeletalModels")))
				{
					if (!CreateModel_LoadSkeletalMesh(modelPath))
					{
						if (!CreateModel_LoadStaticMesh(modelPath, productType))
						{
							return false;
						}
					}
				}
				else
				{
					if (!CreateModel_LoadStaticMesh(modelPath, productType))
					{
						if (!CreateModel_LoadSkeletalMesh(modelPath))
						{
							return false;
						}
					}
				}
			}

			if (CurrentMeshComponent.Get())
			{
				if (pComCoubu && pComCoubu->mIsBurning == ERtComponentComboType::ECombuIsBurning)
				{
					CurrentMeshComponent->SetCollisionObjectType(ECC_GameTraceChannel4); // fire box
					CurrentMeshComponent->SetGenerateOverlapEvents(true);
				}
				else
				{
					if (pTctData->mTctType == ERtTctType::EInvisibleWall)
					{
						//CurrentMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
					}
					else
					{
						if (productType == ERtProductType::EGame)
						{
							//CurrentMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
						}
						else
						{
							//CurrentMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
						}
					}
				}

				if (mActorModel.IsValid())
				{
					if (!(pTctData->mTctType == ERtTctType::EEquipProp ||
						pTctData->mTctType == ERtTctType::EGun ||
						pTctData->mTctType == ERtTctType::EFireNozzle))
					{
						if (productType != ERtProductType::ETct)
						{
							// TCT 자체 위치 및 회전 처리
							USceneComponent* curMeshCom = GetCurrentMeshComponentForTransform();
							if (curMeshCom)
							{
								curMeshCom->SetRelativeLocation(FVector(pTctData->mPosX, pTctData->mPosY, pTctData->mPosZ));
								curMeshCom->SetRelativeRotation(FRotator(pTctData->mRotY, pTctData->mRotZ, pTctData->mRotX));
							}

							// 툴용 컬라이더 TCT 자체 위치 및 회전 처리
	//						if (isFbxFile)
	//						{
	//							mpStaticMeshComponent->SetRelativeLocation(FVector(pTctData->mPosX, pTctData->mPosY, pTctData->mPosZ));
	//							mpStaticMeshComponent->SetRelativeRotation(FRotator(pTctData->mRotY, pTctData->mRotZ, pTctData->mRotX));
	//						}
						}
					}
				}

				InitCurMeshRotation = CurrentMeshComponent->GetRelativeRotation();
			}

			ApplyComponentData(pTctData.Get(), productType == ERtProductType::EGame);

		}
	}

	if (CurrentMeshComponent == nullptr)
	{
		FText errorTitle = FText::FromString("Create False Error");
		FString errorMsg = FString::Printf(TEXT("%s"), *externalModelPath);
		CUSTOM_FILE_APP_LOG_EX("App.log", "%s: %s", *errorTitle.ToString(), *errorMsg);
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(errorMsg), &errorTitle);
	}

	return (CurrentMeshComponent != nullptr);
}

bool ARtObjectActor::CreateModelRenew(FString externalModelPath, ERtProductType productType)
{
	if (mActorModel.IsValid())
	{
		TSharedPtr<FRtTctStruct> pTctStruct = mActorModel->GetTctStruct();
		if (!pTctStruct.Get())
		{
			return false;
		}

		if (!externalModelPath.IsEmpty())
		{
			int32 matCount = mpStaticMeshComponent->GetMaterials().Num();
			for (int32 i = 0; i < matCount; i++)
			{
				mpStaticMeshComponent->SetMaterial(i, nullptr);
			}
			matCount = mpSkeletalMeshComponent->GetMaterials().Num();
			for (int32 i = 0; i < matCount; i++)
			{
				mpSkeletalMeshComponent->SetMaterial(i, nullptr);
			}
			matCount = ProMeshComponent->GetMaterials().Num();
			for (int32 i = 0; i < matCount; i++)
			{
				ProMeshComponent->SetMaterial(i, nullptr);
			}

			if (externalModelPath == "None")
			{
				pTctStruct->ComDefault.ModelFileName = TEXT("");
				pTctStruct->ComDefault.ModelName = TEXT("");

				mpStaticMeshComponent->SetStaticMesh(nullptr);
				mpSkeletalMeshComponent->SetSkeletalMesh(nullptr);
				ProMeshComponent->ClearAllMeshSections();

				return true;
			}
			else
			{
				pTctStruct->ComDefault.ModelFileName = FPaths::GetCleanFilename(externalModelPath);
				pTctStruct->ComDefault.ModelName = FPaths::GetBaseFilename(externalModelPath);
			}
		}
		else
		{
			if (URtResourceModelSubsystem::Get())
			{
				FString fbxFileName = pTctStruct->ComDefault.ModelFileName;
				FString fbxModelPathBase = URtResourceModelSubsystem::Get()->GetFbxModelPathBase();
				externalModelPath = FString::Printf(TEXT("%s/%s"), *fbxModelPathBase, *fbxFileName);
			}
		}

		mpStaticMeshComponent->SetStaticMesh(nullptr);
		mpSkeletalMeshComponent->SetSkeletalMesh(nullptr);
		ProMeshComponent->ClearAllMeshSections();

		bool isFbxFile = false;
		mCurrentModelKind = ERtModelKind::EDefaultModel;

		bool isSystemData = pTctStruct->ComDefault.TctType >= ERtTctType2::EPlayerSpawn;
		//(mActorModel->GetTctData().Get()->mTctType >= ERtTctType::EPlayerSpawn);
		FString fbxFileName = pTctStruct->ComDefault.ModelFileName;
		bool isFbxFileType = fbxFileName.Contains(TEXT("fbx"));

		RemoveFBXAssetActor();

		if (!isSystemData && isFbxFileType)
		{
			FglTFRuntimeFBX_PreLoadInfo fbxInfo;
			bool fbxInfoOk = UglTFRuntimeFBXFunctionLibrary::GetFbxInfo(externalModelPath, fbxInfo);
			if (fbxInfoOk)
			{
				if (!fbxInfo.bSkeletonMesh)
				{ // 스테틱 메시 FBX
					if (URtResourceModelSubsystem::Get() &&
						URtResourceModelSubsystem::Get()->LoadFbxFileToObjectActor(fbxFileName, this, productType == ERtProductType::EGame, fbxInfo))
					{
						ProMeshComponent->SetMobility(EComponentMobility::Movable);
						CurrentMeshComponent = ProMeshComponent;

						isFbxFile = true;
					}
				}
				else
				{ // 스켈레톤 메시 FBX
					UglTFRuntimeAsset* gltfAsset = URtResourceModelSubsystem::Get()->LoadGltfAsset(externalModelPath, this, productType == ERtProductType::EGame);
					UWorld* pWorld = GetWorld();
					AglTFRuntimeFBXAssetActor* glTFRuntimeFbxAssetActor = pWorld->SpawnActor<AglTFRuntimeFBXAssetActor>(
						AglTFRuntimeFBXAssetActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);

					glTFRuntimeFbxAssetActor->SkeletalMeshConfig.bAutoGeneratePhysicsAssetBodies = true;
					glTFRuntimeFbxAssetActor->SkeletalMeshConfig.PhysicsAssetAutoBodyConfig.MinBoneSize = 10;
					glTFRuntimeFbxAssetActor->SkeletalMeshConfig.PhysicsAssetAutoBodyConfig.bDisableOverlappingCollisions = false;
					glTFRuntimeFbxAssetActor->SkeletalMeshConfig.PhysicsAssetAutoBodyConfig.CollisionTraceFlag = ECollisionTraceFlag::CTF_UseSimpleAndComplex;


					if (gltfAsset && glTFRuntimeFbxAssetActor)
					{
						glTFRuntimeFbxAssetActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
						glTFRuntimeFbxAssetActor->Asset = gltfAsset;
						glTFRuntimeFbxAssetActor->ForceFBXLoadAsset();

						CurrentGltfFBXAssetActor = glTFRuntimeFbxAssetActor;

						TArray<UActorComponent*> SkeletalMeshComArr = CurrentGltfFBXAssetActor->K2_GetComponentsByClass(USkeletalMeshComponent::StaticClass());
						for (auto com : SkeletalMeshComArr)
						{
							USkeletalMeshComponent* skelMeshCom = Cast<USkeletalMeshComponent>(com);
							if (skelMeshCom)
							{
								CurrentMeshComponent = skelMeshCom;
								break;
							}
						}

						if (CurrentMeshComponent)
						{
							TArray<FglTFRuntimeFBXAnim> fbxAnimArray = CurrentGltfFBXAssetActor->GetFBXAnimations();
							USkeletalMeshComponent* skelMeshCom = Cast<USkeletalMeshComponent>(CurrentMeshComponent);
							if (skelMeshCom)
							{
								CurrentGltfFBXSelfAnimArray.Empty();
								CurrentGltfFBXSelfAnimIndexMap.Empty();
								int count = 0;
								for (const auto& fbxAnim : fbxAnimArray)
								{
									UAnimSequence* animSeq = CurrentGltfFBXAssetActor->LoadFBXAnimation(skelMeshCom, fbxAnim);
									CurrentGltfFBXSelfAnimArray.Add(animSeq);
									CurrentGltfFBXSelfAnimIndexMap.Add(fbxAnim.Name, count++);
								}

								struct FDynamicSocket
								{
									FName SocketName;
									FName BoneName;
									FDynamicSocket(FName socketName, FName boneName)
									{
										SocketName = socketName;
										BoneName = boneName;
									}
								};

								// 동적으로 소켓 추가하기.
								TArray<FDynamicSocket> registSocketList = {
									FDynamicSocket(FName(R_HAND_SOCKET_NAME), FName(RIGHT_HAND_BONE_NAME)), // 오른손에 동적으로 소켓을 추가한다.
									FDynamicSocket(FName(MOUTH_SOCKET_NAME), FName(HEAD_BONE_NAME)), // 입에 동적으로 소켓을 추가한다.
									FDynamicSocket(FName(L_HAND_SOCKET_NAME), FName(LEFT_HAND_BONE_NAME)) // 왼손에 동적으로 소켓을 추가한다.										
								};

								if (skelMeshCom->GetSkeletalMeshAsset())
								{
									USkeleton* skeleton = skelMeshCom->GetSkeletalMeshAsset()->GetSkeleton();
									if (skeleton)
									{
										for (auto& item : registSocketList)
										{
											USkeletalMeshSocket* SkeletalSocket = NewObject<USkeletalMeshSocket>(skeleton);
											if (SkeletalSocket)
											{
												SkeletalSocket->SocketName = item.SocketName;
												SkeletalSocket->BoneName = item.BoneName;
												SkeletalSocket->RelativeLocation = FVector::ZeroVector;
												SkeletalSocket->RelativeRotation = FRotator::ZeroRotator;
												SkeletalSocket->RelativeScale = FVector::OneVector;
												skeleton->Sockets.Add(SkeletalSocket);
											}
										}
										skelMeshCom->SetCollisionProfileName("Ragdoll");
										skelMeshCom->GetSkeletalMeshAsset()->RebuildSocketMap(); // 이것은 빌드 버전에서 반드시 필요한 코드다.
									}
								}
							}
						}
					}
				}
			}
		}
		else
		{
			FString modelPath = pTctStruct->ComDefault.ModelFileName;

			if (modelPath.Contains(TEXT("SkeletalModels")))
			{
				if (!CreateModel_LoadSkeletalMesh(modelPath))
				{
					if (!CreateModel_LoadStaticMesh(modelPath, productType))
					{
						return false;
					}
				}
			}
			else
			{
				if (!CreateModel_LoadStaticMesh(modelPath, productType))
				{
					if (!CreateModel_LoadSkeletalMesh(modelPath))
					{
						return false;
					}
				}
			}
		}

		if (CurrentMeshComponent.Get())
		{
			if (mActorModel.IsValid())
			{
				if (!(pTctStruct->ComDefault.TctType == ERtTctType2::EEquipment))
				{
					if (productType != ERtProductType::ETct)
					{
						// TCT 자체 위치 및 회전 처리
						USceneComponent* curMeshCom = GetCurrentMeshComponentForTransform();
						if (curMeshCom)
						{
							curMeshCom->SetRelativeLocation(pTctStruct->ComDefault.Position);
							curMeshCom->SetRelativeRotation(pTctStruct->ComDefault.Rotation);
							curMeshCom->SetRelativeScale3D(pTctStruct->ComDefault.Scale);
						}

						// 툴용 컬라이더 TCT 자체 위치 및 회전 처리
						if (isFbxFile)
						{
							mpStaticMeshComponent->SetRelativeLocation(pTctStruct->ComDefault.Position);
							mpStaticMeshComponent->SetRelativeRotation(pTctStruct->ComDefault.Rotation);
							mpStaticMeshComponent->SetRelativeScale3D(pTctStruct->ComDefault.Scale);
						}
					}
				}
				else
				{
					USceneComponent* curMeshCom = GetCurrentMeshComponentForTransform();
					if (curMeshCom != CurrentMeshComponent)
					{
						curMeshCom->SetRelativeTransform(CurrentMeshComponent->GetRelativeTransform());
						CurrentMeshComponent->SetRelativeTransform(FTransform::Identity);
					}

				}
			}
		}

		ApplyComponentDataRenew(pTctStruct.Get(), productType == ERtProductType::EGame);
	}

	return (CurrentMeshComponent != nullptr);
}

bool ARtObjectActor::CreateModel_glTFRuntimeFBX(FString externalModelPath, ERtProductType productType)
{
	mpStaticMeshComponent->SetStaticMesh(nullptr);
	mpSkeletalMeshComponent->SetSkeletalMesh(nullptr);
	ProMeshComponent->ClearAllMeshSections();

	if (!mActorModel.IsValid())
	{
		return false;
	}

	RemoveFBXAssetActor();

	auto ClearMaterials = [](UActorComponent* Component)
		{
			if (UStaticMeshComponent* StaticMeshComp = Cast<UStaticMeshComponent>(Component))
			{
				int32 matCount = StaticMeshComp->GetMaterials().Num();
				for (int32 i = 0; i < matCount; i++)
				{
					StaticMeshComp->SetMaterial(i, nullptr);
				}
			}
			else if (USkeletalMeshComponent* SkeletalMeshComp = Cast<USkeletalMeshComponent>(Component))
			{
				int32 matCount = SkeletalMeshComp->GetMaterials().Num();
				for (int32 i = 0; i < matCount; i++)
				{
					SkeletalMeshComp->SetMaterial(i, nullptr);
				}
			}
			else if (UProceduralMeshComponent* ProcMeshComp = Cast<UProceduralMeshComponent>(Component))
			{
			}
		};

	ClearMaterials(mpStaticMeshComponent);
	ClearMaterials(mpSkeletalMeshComponent);
	ClearMaterials(ProMeshComponent);

	mpStaticMeshComponent->SetStaticMesh(nullptr);
	mpSkeletalMeshComponent->SetSkeletalMesh(nullptr);
	ProMeshComponent->ClearAllMeshSections();

	if (externalModelPath == "None")
	{
		TSharedPtr<RtTctData> pTctData = mActorModel->GetTctData();

		pTctData->mModelFilePath = "";
		pTctData->mModelFileName = "";

		mCurrentModelKind = ERtModelKind::EDefaultModel;
		return false;
	}

	FString fileExtension = FPaths::GetExtension(externalModelPath, true).ToLower();
	bool isModelLoaded = false;

	if (fileExtension == ".fbx")// || fileExtension == ".gltf" || fileExtension == ".glb")
	{
		if (URtResourceModelSubsystem::Get()->LoadGltfAsset(externalModelPath, this, productType == ERtProductType::EGame))
		{
			//CurrentMeshComponent = ProMeshComponent;
			isModelLoaded = true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadModel: Unsupported file extension: %s"), *fileExtension);
		return false;
	}

	//if (isModelLoaded && CurrentMeshComponent && CurrentMeshComponent->IsValidLowLevel())
	//{
	//	if (productType != ERtProductType::ETct)
	//	{
	//		FVector location = FVector(pTctData->mPosX, pTctData->mPosY, pTctData->mPosZ);
	//		FRotator rotation = FRotator(pTctData->mRotY, pTctData->mRotZ, pTctData->mRotX);

	//		CurrentMeshComponent->SetRelativeLocation(location);
	//		CurrentMeshComponent->SetRelativeRotation(rotation);
	//		if (UStaticMeshComponent* StaticMeshComp = Cast<UStaticMeshComponent>(CurrentMeshComponent))
	//		{
	//			StaticMeshComp->SetRelativeLocation(location);
	//			StaticMeshComp->SetRelativeRotation(rotation);
	//		}
	//	}

	//	ApplyComponentData(pTctData.Get(), productType == ERtProductType::EGame);
	//}

	return isModelLoaded;
}

bool ARtObjectActor::PlayGltfAssetAnimation(UglTFRuntimeAsset* animAsset, bool bLoop)
{
	if (animAsset == nullptr)
	{
		APP_LOG("Error!! animAsset is nullptr");
		return false;
	}

	if (CurrentGltfFBXAssetActor == nullptr)
	{
		APP_LOG("Error!! CurrentGltfFBXAssetActor is nullptr");
		return false;
	}

	if (animAsset)
	{
		TArray<UActorComponent*> SkeletalMeshComArr = CurrentGltfFBXAssetActor->K2_GetComponentsByClass(USkeletalMeshComponent::StaticClass());

		if (SkeletalMeshComArr.Num() > 0)
		{
			FglTFRuntimeFBXAnim DefaultFBXAnim;
			if (UglTFRuntimeFBXFunctionLibrary::GetFBXDefaultAnimation(animAsset, DefaultFBXAnim))
			{
				FglTFRuntimeSkeletalAnimationConfig config;

				for (auto com : SkeletalMeshComArr)
				{
					USkeletalMeshComponent* skelMeshCom = Cast<USkeletalMeshComponent>(com);
					if (skelMeshCom)
					{
						USkeletalMesh* pSkeletalMesh = skelMeshCom->GetSkeletalMeshAsset();
						if (pSkeletalMesh)
						{
							UAnimSequence* AnimSeq = UglTFRuntimeFBXFunctionLibrary::LoadFBXExternalAnimAsSkeletalMeshAnimation(animAsset, DefaultFBXAnim, pSkeletalMesh, config);
							skelMeshCom->PlayAnimation(AnimSeq, bLoop);

							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

// 외부 애니메이션 FBX를 읽어 UAnimSequence* 캐시에 넣고 키값을 리턴해 준다.
bool ARtObjectActor::AddExternalFBXAnimSequence(FString externalFbxPath, FString& Key, UAnimSequence*& animSeq)
{
	Key = TEXT("");
	animSeq = nullptr;

	if (externalFbxPath.IsEmpty())
	{
		APP_LOG("Error!! externalFbxPath IsEmpty!");
		return false;
	}

	if (CurrentGltfFBXAssetActor == nullptr)
	{
		APP_LOG("Error!! CurrentGltfFBXAssetActor is nullptr");
		return false;
	}

	FString fileName = FPaths::GetCleanFilename(externalFbxPath);
	if (CurrentGltfFBXExternalAnimMap.Contains(fileName))
	{
		auto* found = CurrentGltfFBXExternalAnimMap.Find(fileName);
		animSeq = *found;

		APP_LOG("Error!! CurrentGltfFBXExternalAnimMap already has the %s", *fileName);
		return false;
	}

	FglTFRuntimeConfig LoaderConfig;
	LoaderConfig.bAllowExternalFiles = true;
	LoaderConfig.bAsBlob = true;
	UglTFRuntimeAsset* Asset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromFilename(externalFbxPath, false, LoaderConfig);

	if (Asset)
	{
		TArray<UActorComponent*> SkeletalMeshComArr = CurrentGltfFBXAssetActor->K2_GetComponentsByClass(USkeletalMeshComponent::StaticClass());

		if (SkeletalMeshComArr.Num() > 0)
		{
			FglTFRuntimeFBXAnim DefaultFBXAnim;
			if (UglTFRuntimeFBXFunctionLibrary::GetFBXDefaultAnimation(Asset, DefaultFBXAnim))
			{
				FglTFRuntimeSkeletalAnimationConfig config;

				for (auto com : SkeletalMeshComArr)
				{
					USkeletalMeshComponent* skelMeshCom = Cast<USkeletalMeshComponent>(com);
					if (skelMeshCom)
					{
						USkeletalMesh* pSkeletalMesh = skelMeshCom->GetSkeletalMeshAsset();
						if (pSkeletalMesh)
						{
							UAnimSequence* AnimSeq = UglTFRuntimeFBXFunctionLibrary::LoadFBXExternalAnimAsSkeletalMeshAnimation(Asset, DefaultFBXAnim, pSkeletalMesh, config);
							AnimSeq->Rename(*fileName);

							CurrentGltfFBXExternalAnimMap.Add(fileName, AnimSeq);
							Key = fileName;
							animSeq = AnimSeq;
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

// 기본적으로 외부 애니메이션 FBX 파일을 읽어서 플레이를 하지만
// 최초 한 번만 그렇게 하고 전부 캐시로 넣어 이후부터는 캐시 데이터로만 플레이 한다.
bool ARtObjectActor::PlayExternalFBXAnimation(FString externalFbxPath, bool bLoop)
{
	if (externalFbxPath.IsEmpty())
	{
		APP_LOG("Error!! externalFbxPath IsEmpty!");
		return false;
	}

	if (CurrentGltfFBXAssetActor == nullptr)
	{
		APP_LOG("Error!! CurrentGltfFBXAssetActor is nullptr");
		return false;
	}

	FString fileNameKey;
	UAnimSequence* animSequence = nullptr;
	AddExternalFBXAnimSequence(externalFbxPath, fileNameKey, animSequence);

	if (animSequence && CurrentMeshComponent.Get())
	{
		USkeletalMeshComponent* skeletalMeshCom = Cast<USkeletalMeshComponent>(CurrentMeshComponent);
		if (skeletalMeshCom && animSequence->IsValidLowLevel())
		{
			skeletalMeshCom->PlayAnimation(animSequence, bLoop);

			return true;
		}
	}

	return false;
}

bool ARtObjectActor::PlayFBXSelfAnimation(int32 animIndex, bool bLoop)
{
	if (animIndex < 0)
	{
		APP_LOG("Error!! animIndex is less than 0");
		return false;
	}
	if (CurrentGltfFBXAssetActor == nullptr)
	{
		APP_LOG("Error!! CurrentGltfFBXAssetActor is nullptr");
		return false;
	}

	if (animIndex < CurrentGltfFBXSelfAnimArray.Num())
	{
		if (CurrentMeshComponent.Get())
		{
			USkeletalMeshComponent* skeletalMeshCom = Cast<USkeletalMeshComponent>(CurrentMeshComponent);
			if (skeletalMeshCom)
			{
				UAnimSequence* animSequence = CurrentGltfFBXSelfAnimArray[animIndex];
				if (animSequence && animSequence->IsValidLowLevel())
				{
					skeletalMeshCom->PlayAnimation(animSequence, bLoop);

					return true;
				}
			}
		}
	}
	else
	{
		APP_LOG("Error!! animIndex is Over");
	}

	return false;
}

bool ARtObjectActor::PlayFBXSelfAnimationByName(FString animName, bool bLoop)
{
	int32* foundValue = CurrentGltfFBXSelfAnimIndexMap.Find(animName);
	if (foundValue)
	{
		int32 index = *foundValue;
		return PlayFBXSelfAnimation(index, bLoop);
	}

	return false;
}

void ARtObjectActor::SwitchCollision(bool isForGame)
{
	if (mActorModel.IsValid())
	{
		TSharedPtr<RtTctData> pTctData = mActorModel->GetTctData();

		if (pTctData->mTctType == ERtTctType::EPlayerSpawn)
		{
			return;
		}

		if (CurrentMeshComponent.Get())
		{
			RtTctData::RtComCombustion* pComCoubu = static_cast<RtTctData::RtComCombustion*>(pTctData->GetComponent(ERtComponentType::ECombustion));

			if (pComCoubu && pComCoubu->mIsBurning == ERtComponentComboType::ECombuIsBurning)
			{
				CurrentMeshComponent->SetCollisionObjectType(ECC_GameTraceChannel4); // fire box
				CurrentMeshComponent->SetGenerateOverlapEvents(true);
			}
			else
			{
				if (pTctData->mTctType == ERtTctType::EInvisibleWall)
				{
					//CurrentMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
				}
				else
				{
					if (isForGame)
					{
						//CurrentMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					}
					else
					{
						//CurrentMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
					}
				}
			}
		}

	}
}

void ARtObjectActor::ApplyComponentCollision(RtTctData* pTctData, bool isOverlapCollision, bool showBoxComponent)
{
	if (pTctData)
	{
		// 충돌 컴포넌트 기능 적용.
		RtTctData::RtComCollision* pCollisionCom = GetComponent<RtTctData::RtComCollision>(ERtComponentType::ECollision);
		if (pCollisionCom)
		{
			int colCounter = 0;
			for (auto ele : pCollisionCom->mElementList)
			{
				auto* pColEle = static_cast<RtTctData::RtComCollision::RtElement*>(ele.Get());
				if (nullptr != pColEle)
				{
					if (pColEle->mComEleType == ERtTctComponentEleType::ECollisionCube)
					{
						FName componentName = FName(FString::Printf(TEXT("BoxCom_%d(%s)"), colCounter, *pColEle->mName));
						UBoxComponent* pBoxCollision = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(), componentName);//, FName("ColBox"));

						pBoxCollision->SetupAttachment(RootComponent);

						if (isOverlapCollision)
						{
							pBoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
							//pBoxCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
							pBoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

							pBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ARtObjectActor::OnBeginOverlap);
							pBoxCollision->OnComponentEndOverlap.AddDynamic(this, &ARtObjectActor::OnEndOverlap);
						}
						else
						{
							pBoxCollision->SetCollisionProfileName(FName(TEXT("OverlapAll")));
						}

						///////////////////////////////////////////////////////////////////////////////////////
						// Tct 로컬 좌표 및 회전 적용...
						///////////////////////////////////////////////////////////////////////////////////////
						pBoxCollision->SetRelativeLocation(FVector(pTctData->mPosX, pTctData->mPosY, pTctData->mPosZ));
						pBoxCollision->SetRelativeRotation(FRotator(pTctData->mRotY, pTctData->mRotZ, pTctData->mRotX));
						///////////////////////////////////////////////////////////////////////////////////////

						// 박스 사이즈 설정.
						pBoxCollision->InitBoxExtent(FVector(pColEle->mSizeX * 0.5f, pColEle->mSizeY * 0.5f, pColEle->mSizeZ * 0.5f));

						// 로컬 박스 좌표 적용.
						USceneComponent* curMeshCom = GetCurrentMeshComponentForTransform();
						if (curMeshCom)
						{
							FTransform tmRoot = curMeshCom->GetComponentTransform();
							FRotator rotRoot = tmRoot.GetRotation().Rotator();
							FTransform tm = pBoxCollision->GetComponentTransform();
							FVector posMod = FVector(pColEle->mPosX, pColEle->mPosY, pColEle->mPosZ + pColEle->mSizeZ * 0.5f);
							posMod = rotRoot.RotateVector(posMod);
							tm.AddToTranslation(posMod);
							pBoxCollision->SetWorldTransform(tm);
						}
						// 로컬 박스 좌표 적용.
						//pBoxCollision->SetRelativeLocation(FVector(pColEle->mPosX, pColEle->mPosY, pColEle->mPosZ + pColEle->mSizeZ * 0.5f));

						pBoxCollision->RegisterComponent();
						AddInstanceComponent(pBoxCollision);

						pBoxCollision->SetHiddenInGame(!showBoxComponent);
					}
					else if (pColEle->mComEleType == ERtTctComponentEleType::ECollisionCapsule)
					{
						FName componentName = FName(FString::Printf(TEXT("CapsuleCom_%d(%s)"), colCounter, *pColEle->mName));
						UCapsuleComponent* pCapsuleCollision = NewObject<UCapsuleComponent>(this, UCapsuleComponent::StaticClass(), componentName);

						pCapsuleCollision->SetupAttachment(RootComponent);
						Collisions.Add(pCapsuleCollision);

						//pCapsuleCollision->SetCollisionProfileName(isOverlapCollision ? UCollisionProfile::NoCollision_ProfileName : UCollisionProfile::BlockAll_ProfileName);
						if (isOverlapCollision)
						{
							pCapsuleCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
							pCapsuleCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

							pCapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &ARtObjectActor::OnBeginOverlap);
							pCapsuleCollision->OnComponentEndOverlap.AddDynamic(this, &ARtObjectActor::OnEndOverlap);
						}
						else
						{
							pCapsuleCollision->SetCollisionProfileName(FName(TEXT("OverlapAll")));
						}

						///////////////////////////////////////////////////////////////////////////////////////
						// Tct 로컬 좌표 및 회전 적용...
						///////////////////////////////////////////////////////////////////////////////////////
						pCapsuleCollision->SetRelativeLocation(FVector(pTctData->mPosX, pTctData->mPosY, pTctData->mPosZ));
						pCapsuleCollision->SetRelativeRotation(FRotator(pTctData->mRotY, pTctData->mRotZ, pTctData->mRotX));
						///////////////////////////////////////////////////////////////////////////////////////

						// 캡슐 사이즈 설정.
						pCapsuleCollision->InitCapsuleSize(pColEle->mSizeY * 0.5f, pColEle->mSizeX * 0.5f); // 인자 1 : 반경, 인자 2 : 절반 높이

						// 캡슐 로컬 좌표 적용.
						USceneComponent* curMeshCom = GetCurrentMeshComponentForTransform();
						if (curMeshCom)
						{
							FTransform tmRoot = curMeshCom->GetComponentTransform();
							FRotator rotRoot = tmRoot.GetRotation().Rotator();
							FTransform tm = pCapsuleCollision->GetComponentTransform();
							FVector posMod = FVector(pColEle->mPosX, pColEle->mPosY, pColEle->mPosZ + pColEle->mSizeZ * 0.5f);
							posMod = rotRoot.RotateVector(posMod);
							tm.AddToTranslation(posMod);
							pCapsuleCollision->SetWorldTransform(tm);
						}

						//pCapsuleCollision->SetRelativeLocation(FVector(pColEle->mPosX, pColEle->mPosY, pColEle->mPosZ));

						pCapsuleCollision->RegisterComponent();
						AddInstanceComponent(pCapsuleCollision);
						pCapsuleCollision->SetHiddenInGame(!showBoxComponent);
					}
				}

				colCounter++;
			}
		}
	}
}

void ARtObjectActor::ApplyComponentCollisionRenew(FRtTctStruct* pTctData, bool showBoxComponent)
{
	if (pTctData)
	{
		// 충돌 컴포넌트 기능 적용.
		FRtTctStruct* pCollisionCom = GetTctStruct();
		if (pCollisionCom)
		{
			int colCounter = 0;
			for (auto ele : pCollisionCom->ComCollisionList)
			{
				/*if (nullptr != ele)
				{*/
				if (ele.CollisionShape == ERtCollisionShape::EBoxShape)
				{
					FName componentName = FName(FString::Printf(TEXT("BoxCom_%d"), colCounter));
					UBoxComponent* pBoxCollision = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(), componentName);//, FName("ColBox"));

					pBoxCollision->SetupAttachment(RootComponent);
					CollisionMap.Add(pBoxCollision, ele.CollisionType);
					SetCollisionType(pBoxCollision, ele.CollisionType);

					// 박스 사이즈 설정.
					pBoxCollision->InitBoxExtent(FVector(ele.Scale.X * 0.5f, ele.Scale.Y * 0.5f, ele.Scale.Z * 0.5f));

					if (pTctData->ComDefault.TctType == ERtTctType2::EEquipment)
					{
						pBoxCollision->SetRelativeLocation(ele.Position + (FVector::UpVector * ele.Scale.Z * 0.5));
						pBoxCollision->SetRelativeRotation(ele.Rotation);
					}
					else
					{
						///////////////////////////////////////////////////////////////////////////////////////
						// Tct 로컬 좌표 및 회전 적용...
						///////////////////////////////////////////////////////////////////////////////////////
						pBoxCollision->SetRelativeLocation(FVector(pTctData->ComDefault.Position.X, pTctData->ComDefault.Position.Y, pTctData->ComDefault.Position.Z));
						pBoxCollision->SetRelativeRotation(FRotator(pTctData->ComDefault.Rotation.Pitch, pTctData->ComDefault.Rotation.Yaw, pTctData->ComDefault.Rotation.Roll));
						///////////////////////////////////////////////////////////////////////////////////////

						// 로컬 박스 좌표 적용.
						USceneComponent* curMeshCom = GetCurrentMeshComponentForTransform();
						if (curMeshCom)
						{
							FTransform tmRoot = curMeshCom->GetComponentTransform();
							FRotator rotRoot = tmRoot.GetRotation().Rotator();
							FTransform tm = pBoxCollision->GetComponentTransform();
							float Z_ModifyValue = 0;
							if (pTctData->ComDefault.TctType != ERtTctType2::EEquipment)
							{
								Z_ModifyValue = (pTctData->Version >= TCT_FILE_VERSION_COLLISION_PIVOT) ? ele.Scale.Z * 0.5f : 0;
							}
							FVector posMod = FVector(ele.Position.X, ele.Position.Y, ele.Position.Z + Z_ModifyValue); //  + ele.Scale.Z * 0.5f
							posMod = rotRoot.RotateVector(posMod);
							tm.AddToTranslation(posMod);
							pBoxCollision->SetWorldTransform(tm);
						}
					}
					// 로컬 박스 좌표 적용.
					//pBoxCollision->SetRelativeLocation(FVector(pColEle->mPosX, pColEle->mPosY, pColEle->mPosZ + pColEle->mSizeZ * 0.5f));

					pBoxCollision->RegisterComponent();
					AddInstanceComponent(pBoxCollision);

					pBoxCollision->SetHiddenInGame(!showBoxComponent);
				}
				else if (ele.CollisionShape == ERtCollisionShape::ECapsuleShape)
				{
					FName componentName = FName(FString::Printf(TEXT("CapsuleCom_%d"), colCounter));
					UCapsuleComponent* pCapsuleCollision = NULL;

					pCapsuleCollision = NewObject<UCapsuleComponent>(this, UCapsuleComponent::StaticClass(), componentName);

					pCapsuleCollision->SetupAttachment(RootComponent);


					CollisionMap.Add(pCapsuleCollision, ele.CollisionType);
					SetCollisionType(pCapsuleCollision, ele.CollisionType);

					// 캡슐 사이즈 설정.
					pCapsuleCollision->InitCapsuleSize(ele.Scale.Y * 0.5f, ele.Scale.X * 0.5f); // 인자 1 : 반경, 인자 2 : 절반 높이

					if (pTctData->ComDefault.TctType == ERtTctType2::EEquipment)
					{
						pCapsuleCollision->SetRelativeLocation(ele.Position);
						pCapsuleCollision->SetRelativeRotation(ele.Rotation);
					}
					else
					{
						///////////////////////////////////////////////////////////////////////////////////////
						// Tct 로컬 좌표 및 회전 적용...
						///////////////////////////////////////////////////////////////////////////////////////
						pCapsuleCollision->SetRelativeLocation(FVector(pTctData->ComDefault.Position.X, pTctData->ComDefault.Position.Y, pTctData->ComDefault.Position.Z));
						pCapsuleCollision->SetRelativeRotation(FRotator(pTctData->ComDefault.Rotation.Pitch, pTctData->ComDefault.Rotation.Yaw, pTctData->ComDefault.Rotation.Roll));
						///////////////////////////////////////////////////////////////////////////////////////

						// 캡슐 로컬 좌표 적용.
						USceneComponent* curMeshCom = GetCurrentMeshComponentForTransform();
						if (curMeshCom)
						{
							FTransform tmRoot = curMeshCom->GetComponentTransform();
							FRotator rotRoot = tmRoot.GetRotation().Rotator();
							FTransform tm = pCapsuleCollision->GetComponentTransform();
							float Z_ModifyValue = 0;
							if (pTctData->ComDefault.TctType != ERtTctType2::EEquipment)
							{
								Z_ModifyValue = (pTctData->Version >= TCT_FILE_VERSION_COLLISION_PIVOT) ? ele.Scale.Z * 0.5f : 0;
							}
							FVector posMod = FVector(ele.Position.X, ele.Position.Y, ele.Position.Z + Z_ModifyValue);
							posMod = rotRoot.RotateVector(posMod);
							tm.AddToTranslation(posMod);
							pCapsuleCollision->SetWorldTransform(tm);
						}
					}

					//pCapsuleCollision->SetRelativeLocation(FVector(pColEle->mPosX, pColEle->mPosY, pColEle->mPosZ));

					pCapsuleCollision->RegisterComponent();
					AddInstanceComponent(pCapsuleCollision);
					pCapsuleCollision->SetHiddenInGame(!showBoxComponent);
					//}
				}

				colCounter++;
			}
		}
	}
}

void ARtObjectActor::SetCollisionType(UShapeComponent* collisionComp, ERtCollisionType collisionType)
{
	if (collisionType == ERtCollisionType::ECollisionPass)
	{
		collisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		collisionComp->SetCollisionProfileName(TEXT("WorldDynamic"));
		collisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
		//collisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
		collisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

		collisionComp->OnComponentBeginOverlap.RemoveDynamic(this, &ARtObjectActor::OnBeginOverlap);
		collisionComp->OnComponentBeginOverlap.AddDynamic(this, &ARtObjectActor::OnBeginOverlap);
		collisionComp->OnComponentEndOverlap.RemoveDynamic(this, &ARtObjectActor::OnEndOverlap);
		collisionComp->OnComponentEndOverlap.AddDynamic(this, &ARtObjectActor::OnEndOverlap);
	}
	else
	{
		collisionComp->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	}

}

void ARtObjectActor::ApplyComponentData(RtTctData* pTctData, bool isForGame)
{
	if (!pTctData)
	{
		return;
	}

	if (pTctData->mTctType == ERtTctType::EPlayerSpawn)
	{
		ApplyComponentCollision(pTctData, true);
		SetDecal_StartPointNotReady(true);
	}
	else if (!isForGame) // 툴 전용..
	{
		ApplyComponentCollision(pTctData, false);
	}
	else // 게임 전용..
	{
		ApplyComponentCollision(pTctData, false);

		if (Collisions.IsEmpty())
		{
			CurrentMeshComponent->SetCollisionProfileName(FName(TEXT("OverlapAllDynamic")));
		}

		// 머티리얼 컴포넌트 적용.
		RtTctData::RtComMaterial* pMaterialCom = GetComponent<RtTctData::RtComMaterial>(ERtComponentType::EMaterial);
		if (pMaterialCom)
		{
			if (!pMaterialCom->mMaterialFilePath.IsEmpty())
			{
				FString filePath = pMaterialCom->mMaterialFilePath + "/" + pMaterialCom->mMaterialFileName;

				TObjectPtr<UStaticMesh> staticMesh = mpStaticMeshComponent->GetStaticMesh();
				if (nullptr != staticMesh.Get())
				{
					UMaterialInstanceDynamic* pMat = URtTextureUtil::CreateMaterialFromBaseTexture(staticMesh->GetMaterial(0), filePath);
					mpStaticMeshComponent->SetMaterial(0, pMat);
				}
				else
				{
					TSharedPtr<RtFbxProcedualMeshInfo> proMeshInfo = mActorModel->GetDefaultFbxProMeshInfo();

					bool isValid = false;
					int32 width = 0;
					int32 height = 0;

					UTexture2D* pDiffTexture = URtTextureUtil::LoadTexture2D_FromFile(filePath, isValid, width, height);
					for (auto& item : proMeshInfo->mMaterialMap)
					{
						item.Value->mpMatInsDyna->SetTextureParameterValue(TEXT("BaseTexture"), pDiffTexture);
						break;
					}

					SetMaterialProMesh();
				}
			}
		}

		// 이펙트 컴포넌트 적용.
		RtTctData::RtComEffect* pEffectCom = GetComponent<RtTctData::RtComEffect>(ERtComponentType::EEffect);
		if (pEffectCom)
		{
			int effectElementCount = pEffectCom->GetElementCount();
			SetEffectArraySize(effectElementCount);
		}
	}

	// 라이트 컴포넌트 적용.
	RtTctData::RtComLight* pLightCom = GetComponent<RtTctData::RtComLight>(ERtComponentType::ELight);
	if (pLightCom)
	{
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

void ARtObjectActor::ApplyComponentDataRenew(FRtTctStruct* pTctData, bool isForGame)
{
	if (!pTctData)
	{
		return;
	}

	ApplyComponentCollisionRenew(pTctData);

	if (pTctData->ComDefault.TctType == ERtTctType2::EPlayerSpawn)
	{
		SetDecal_StartPointNotReady(true);
	}
	else if (!isForGame) // 툴 전용..
	{

	}
	else // 게임 전용..
	{

		//	// 머티리얼 컴포넌트 적용.
		//	RtTctData::RtComMaterial* pMaterialCom = GetComponent<RtTctData::RtComMaterial>(ERtComponentType::EMaterial);
		//	if (pMaterialCom) {
		//		if (!pMaterialCom->mMaterialFilePath.IsEmpty())
		//		{
		//			FString filePath = pMaterialCom->mMaterialFilePath + "/" + pMaterialCom->mMaterialFileName;

		//			TObjectPtr<UStaticMesh> staticMesh = mpStaticMeshComponent->GetStaticMesh();
		//			if (nullptr != staticMesh.Get())
		//			{
		//				UMaterialInstanceDynamic* pMat = URtTextureUtil::CreateMaterialFromBaseTexture(staticMesh->GetMaterial(0), filePath);
		//				mpStaticMeshComponent->SetMaterial(0, pMat);
		//			}
		//			else
		//			{
		//				TSharedPtr<RtFbxProcedualMeshInfo> proMeshInfo = mActorModel->GetDefaultFbxProMeshInfo();

		//				bool isValid = false;
		//				int32 width = 0;
		//				int32 height = 0;

		//				UTexture2D* pDiffTexture = URtTextureUtil::LoadTexture2D_FromFile(filePath, isValid, width, height);
		//				for (auto& item : proMeshInfo->mMaterialMap)
		//				{
		//					item.Value->mpMatInsDyna->SetTextureParameterValue(TEXT("BaseTexture"), pDiffTexture);
		//					break;
		//				}

		//				SetMaterialProMesh();
		//			}
		//		}
		//	}

			// 이펙트 컴포넌트 적용.
			/*FRtTctComDirection* pEffectCom = GetComponent<FRtTctComDirection>(ERtComponentType::EEffect);
			if (pEffectCom) {
				int effectElementCount = pEffectCom->DirectionItemList.Num();
				SetEffectArraySize(effectElementCount);
			}*/
	}
}

bool ARtObjectActor::ChangeModel(ERtModelKind modelKind, int index, bool forceChange)
{
	if (!forceChange)
	{
		if (mCurrentModelKind == modelKind)
		{
			return false;
		}
	}

	TSharedPtr<RtTctData> pTctData = mActorModel->GetTctData();
	if (nullptr != pTctData.Get())
	{
		FString modelPath;

		switch (modelKind)
		{
		case ERtModelKind::EDefaultModel:
			modelPath = pTctData->mModelFilePath;
			break;

		case ERtModelKind::ECombustionBurningModel: // 연소 컴포넌트 타는 모델.
		{
			RtTctData::RtComCombustion* pCombu = static_cast<RtTctData::RtComCombustion*>(pTctData->GetComponent(ERtComponentType::ECombustion));
			IS_NOT_NULL(pCombu)
			{
				modelPath = pCombu->mBurningModelPath;
			}
		}
		break;

		case ERtModelKind::ECombustionEndBurningModel: // 연소 컴포넌트 다 탄 모델.
		{
			RtTctData::RtComCombustion* pCombu = static_cast<RtTctData::RtComCombustion*>(pTctData->GetComponent(ERtComponentType::ECombustion));
			IS_NOT_NULL(pCombu)
			{
				modelPath = pCombu->mEndBurningModelPath;
			}
		}
		break;

		case ERtModelKind::EFunctionOperateModel: // 기능 컴포넌트 요소 작동 모델.
		{
			RtTctData::RtComFunction* pFun = static_cast<RtTctData::RtComFunction*>(pTctData->GetComponent(ERtComponentType::EFunction));
			IS_NOT_NULL(pFun)
			{
				auto pEle = GetElementEx<RtTctData::RtComFunction::RtElement>(ERtComponentType::EFunction, index);
				if (nullptr != pEle)
				{
					modelPath = pEle->mModelFilePath;
				}
			}
		}
		break;

		}

		UStaticMesh* pStaticMesh = URtReadWriteFile::LoadStaticMeshFromPath(modelPath);
		if (nullptr != pStaticMesh)
		{
			mpStaticMeshComponent->SetMobility(EComponentMobility::Movable);
			mpStaticMeshComponent->SetStaticMesh(pStaticMesh);
			mpSkeletalMeshComponent->SetSkeletalMesh(nullptr);
			ProMeshComponent->ClearAllMeshSections();
		}
		else
		{
			USkeletalMesh* pSkeletalMesh = URtReadWriteFile::LoadSkeletalFromPath(modelPath);
			if (nullptr != pSkeletalMesh)
			{
				mpStaticMeshComponent->SetStaticMesh(nullptr);
				mpSkeletalMeshComponent->SetSkeletalMesh(pSkeletalMesh);
				ProMeshComponent->ClearAllMeshSections();
			}
			else
			{
				RtFbxProcedualMeshInfo* pProMeshInfo = mActorModel->GetDefaultFbxProMeshInfo().Get();
				if (nullptr != pProMeshInfo)
				{
					mpStaticMeshComponent->SetStaticMesh(nullptr);
					mpSkeletalMeshComponent->SetSkeletalMesh(nullptr);
				}
			}
		}

		mCurrentModelKind = modelKind;
	}

	return true;
}

bool ARtObjectActor::CreateProMesh(RtFbxProcedualMeshInfo* pProMeshInfo, bool enableCollision)
{
	if (ProMeshComponent != nullptr && pProMeshInfo != nullptr)
	{
		ProMeshComponent->ClearAllMeshSections();

		if (0 < pProMeshInfo->mProcedualMap.Num())
		{
			int sect = 0;
			for (auto& Elem : pProMeshInfo->mProcedualMap)
			{
				auto& pData = Elem.Value;

				for (int i = 0; i < pData.Num(); i++)
				{
					pData[i]->mSectionID = sect;

					RtMaterialInfo* pMatInfo = pProMeshInfo->FindMaterialInfo(pData[i]->mMaterialName);
					if (pMatInfo != nullptr)
					{
						ProMeshComponent->SetMaterial(pData[i]->mSectionID, pMatInfo->mpMatInsDyna);
					}

					ProMeshComponent->CreateMeshSection_LinearColor(pData[i]->mSectionID,
						pData[i]->mVertices,
						pData[i]->mTriangles,
						pData[i]->mNormals,
						pData[i]->mUV,
						pData[i]->mVertexColors,
						pData[i]->mTangents,
						enableCollision);

					sect++;
				}
			}

			// Enable collision data
			ProMeshComponent->ContainsPhysicsTriMeshData(enableCollision);
			return true;
		}
	}

	return false;
}

bool ARtObjectActor::SetMaterialProMesh()
{
	if (ProMeshComponent != NULL)
	{
		RtFbxProcedualMeshInfo* pProMeshInfo = mActorModel->GetDefaultFbxProMeshInfo().Get();

		if (0 < pProMeshInfo->mProcedualMap.Num())
		{
			int sect = 0;
			for (auto& Elem : pProMeshInfo->mProcedualMap)
			{
				auto& pData = Elem.Value;

				for (int i = 0; i < pData.Num(); i++)
				{
					pData[i]->mSectionID = sect;

					RtMaterialInfo* pMatInfo = pProMeshInfo->FindMaterialInfo(pData[i]->mMaterialName);
					if (pMatInfo != NULL)
					{
						ProMeshComponent->SetMaterial(pData[i]->mSectionID, pMatInfo->mpMatInsDyna);
					}

					sect++;
				}
			}
			return true;
		}
	}

	return false;
}

UMeshComponent* ARtObjectActor::GetCurrentMesh()
{
	if (CurrentMeshComponent == nullptr)
	{
		FText errorTitle = FText::FromString("No Current Mesh");
		FString errorMsg = ObjectOptionComponent->GetObjectKeyName();
		CUSTOM_FILE_APP_LOG_EX("App.log", "%s: %s", *errorTitle.ToString(), *errorMsg);
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(errorMsg), &errorTitle);
	}

	return CurrentMeshComponent;
}

bool ARtObjectActor::GetFirstDirectionEffectTransform(FTransform& outTransform)
{
	TSharedPtr<FRtTctStruct> pTctStruct = mActorModel->GetTctStruct();

	if (!pTctStruct)
		return false;

	for (FRtTctComDirection directionlist : pTctStruct->ComDirectionList)
	{
		for (FRtTctComDirectionItem direction : directionlist.DirectionItemList)
		{
			outTransform = direction.EffectTransform.GetTransform();
			return true;
		}
	}

	return false;
}

// 현재는 기능 컴포넌트 전용으로 사용한다.
void ARtObjectActor::PlaySingleEffect(ERtComponentType componentType, FVector vStartPos)
{
	TSharedPtr<FRtTctStruct> pTctStruct = mActorModel->GetTctStruct();

	if (!pTctStruct)
		return;

	switch (componentType)
	{
	case ERtComponentType::EEffect:
		for (FRtTctComDirection directionlist : pTctStruct->ComDirectionList)
		{
			for (FRtTctComDirectionItem direction : directionlist.DirectionItemList)
			{
				if (FPaths::GetExtension(direction.EffectFileName).Equals("efk"))
				{
					bool result = PlayEfkEffect(direction.EffectFileName, direction.EffectTransform.GetTransform(), direction.EffectSpeed);
					//FString str = FString::Printf(TEXT("%s Effect %s"), *FPaths::GetCleanFilename(direction.EffectFileName), *UKismetStringLibrary::Conv_BoolToString(result));
					//UKismetSystemLibrary::PrintString(this, *str);
				}
				else
					PlayEffect(direction.EffectFileName, vStartPos);
			}
		}

		break;
	default:
		break;
	}

	return;

	// 구버전 이펙트 플레이
	//TSharedPtr<RtTctData> pTctData = mActorModel->GetTctData();
	//if (nullptr != pTctData.Get())
	//{
	//	if (componentType == ERtComponentType::EFunction)
	//	{
	//		RtTctData::RtComFunction* pFunctionCom = static_cast<RtTctData::RtComFunction*>(pTctData->GetComponent(componentType));
	//		if (nullptr != pFunctionCom)
	//		{
	//			PlayEffect(pFunctionCom->mEffectFilePath, vStartPos);
	//		}
	//	}
	//}
}

// 현재는 기능, 이펙트 컴포넌트 전용으로 사용한다.
void ARtObjectActor::PlayArrayElementEffect(ERtComponentType componentType, int index, FVector vStartPos)
{
	TSharedPtr<RtTctData> pTctData = mActorModel->GetTctData();
	if (nullptr != pTctData.Get())
	{
		if (componentType == ERtComponentType::EFunction)
		{
			RtTctData::RtComFunction* pFunctionCom = static_cast<RtTctData::RtComFunction*>(pTctData->GetComponent(componentType));
			if (nullptr != pFunctionCom)
			{
				if (0 <= index && index < pFunctionCom->GetElementCount())
				{
					for (TSharedPtr<RtTctData::RtElementBase> element : pFunctionCom->mElementList)
					{
						RtTctData::RtComFunction::RtElement* pEle = static_cast<RtTctData::RtComFunction::RtElement*>(element.Get());

						PlayEffectArray(pEle->mEffectFilePath, index, vStartPos);
					}
				}
			}
		}
		else if (componentType == ERtComponentType::EEffect)
		{
			RtTctData::RtComEffect* pEffectCom = static_cast<RtTctData::RtComEffect*>(pTctData->GetComponent(componentType));
			if (nullptr != pEffectCom)
			{
				if (0 <= index && index < pEffectCom->GetElementCount())
				{
					for (TSharedPtr<RtTctData::RtElementBase> element : pEffectCom->mElementList)
					{
						RtTctData::RtComEffect::RtElement* pEle = static_cast<RtTctData::RtComEffect::RtElement*>(element.Get());

						PlayEffectArray(pEle->mEffectFilePath, index, vStartPos);
					}
				}
			}
		}
	}
}

void ARtObjectActor::SetVisible(bool bNewVisible)
{
	if (CurrentMeshComponent.Get())
	{
		CurrentMeshComponent->SetVisibility(bNewVisible);
		if (CurrentMeshComponent == mpSkeletalMeshComponent && Collisions.IsEmpty() && CollisionMap.IsEmpty())
			CurrentMeshComponent->SetCollisionProfileName(bNewVisible ? FName(TEXT("OverlapAllDynamic")) : UCollisionProfile::NoCollision_ProfileName);
	}

	if (Decal.Get())
	{
		Decal->SetVisibility(bNewVisible);
	}

	// 리뉴얼 이전
	for (UShapeComponent* collision : Collisions)
	{
		collision->SetCollisionProfileName(bNewVisible ? FName(TEXT("OverlapAllDynamic")) : UCollisionProfile::NoCollision_ProfileName);
	}

	//리뉴얼 이후
	for (TPair<UShapeComponent*, ERtCollisionType> data : CollisionMap)
	{
		if (bNewVisible)
			SetCollisionType(data.Key, data.Value);
		else
			data.Key->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	}

	Delegate_SetVisible.Broadcast();

}

bool ARtObjectActor::IsVisible()
{
	if (CurrentMeshComponent == nullptr)
	{
		FText errorTitle = FText::FromString("NotFound: CurrentMeshComponent");
		FString errorMsg = FString::Printf(TEXT("%s"), *ObjectOptionComponent->GetObjectKeyName());
		CUSTOM_FILE_APP_LOG_EX("App.log", "%s: %s", *errorTitle.ToString(), *errorMsg);
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(errorMsg), &errorTitle);
	}

	return CurrentMeshComponent->IsVisible(); // CurrentMeshComponent==nullptr이면 정상적인 훈련 상황이 아니므로 일부러 터지게 만들었음
}

// Rename 함수 래퍼
//bool ARtObjectActor::ReNameID(FString newName)
//{
//	if( newName.IsEmpty() ) {
//		return false;
//	}
//
//	FString label = GetActorLabel();
//	label += "_" + newName;
//	SetActorLabel(*label);
//
//	if( Rename(*newName, nullptr, REN_Test) )
//	{
//		return Rename(*newName);
//	}
//	else
//	{
//		APP_LOG("%s rename error!! %s", *GetName(), *newName);
//	}
//
//	return false;
//}

//
//void ARtObjectActor::SpawnChildTriggerSphere()
//{
//	FString myName = GetName();
//
//	FActorSpawnParameters spawnParams;
//	spawnParams.Name = FName(*FString(myName + "_trigger_sphere"));
//
//	FRotator spawnRotator;
//	FVector spawnLocation = FVector::ZeroVector;
//	spawnLocation.Set(0, 0, 0);
//
//	UWorld* pWorld = GetWorld();
//	mpTriggerSphere = pWorld->SpawnActor<ATriggerSphere>(
//		ATriggerSphere::StaticClass(), spawnLocation, spawnRotator, spawnParams
//		);
//
//	mpTriggerSphere->
//
//	EAttachmentRule LocationRule = EAttachmentRule::KeepRelative; 
//	EAttachmentRule RotationRule = EAttachmentRule::KeepRelative; 
//	EAttachmentRule ScaleRule = EAttachmentRule::KeepRelative;
//	bool bWeldSimulatedBodies = true;
//
//	mpTriggerSphere->AttachToActor(this, FAttachmentTransformRules(LocationRule, RotationRule, ScaleRule, bWeldSimulatedBodies));
//
//	mpTriggerSphere->OnActorBeginOverlap.AddDynamic(this, &ARtObjectActor::OnActorBeginOverlaped);
//}
//
//void ARtObjectActor::OnActorBeginOverlaped(AActor* pOverlappedActor, AActor* pOtherActor)
//{
//	
//}


void ARtObjectActor::PlayEffect(FString effectSystemPath, FVector vStartPos)
{
	if (!effectSystemPath.IsEmpty())
	{
		TObjectPtr<UNiagaraSystem> NiagaraSystem = LoadObject<UNiagaraSystem>(nullptr, *effectSystemPath);

		bool bAttachToPlayer = false;
		bool bAutoDestroy = true;
		bool bAutoActivate = true;
		FVector Location = vStartPos;//FVector::ZeroVector;
		ENCPoolMethod PoolingMethod = ENCPoolMethod::None;
		bool bPreCullCheck = true;

		//UNiagaraComponent* SpawnedComponent = nullptr;

		StopEffect();
		//if (nullptr != mpSpawnedEffect) 
		//{
		//	if (IsValid(mpSpawnedEffect)) // 이펙트가 실제 유효한지 검사한다.
		//	{
		//		mpSpawnedEffect->DestroyComponent();
		//	}

		//	mpSpawnedEffect = nullptr;
		//}

		mpSpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystem, RootComponent, NAME_None,
			Location, FRotator::ZeroRotator, EAttachLocation::SnapToTarget,
			bAutoDestroy, bAutoActivate, PoolingMethod, bPreCullCheck);

		//mpSpawnedEffect->SetVariableVec3(UserParam_Pos, vStartPos);
		//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem, FVector(0, 0, 0));
	}
}

void ARtObjectActor::StopEffect()
{
	if (nullptr != mpSpawnedEffect)
	{
		//mpSpawnedComponent
		if (IsValid(mpSpawnedEffect)) // 이펙트가 실제 유효한지 검사한다.
		{
			if (mpSpawnedEffect->IsActive())
			{
				mpSpawnedEffect->Deactivate();
			}
		}

		mpSpawnedEffect = nullptr;
	}
}

//void ARtObjectActor::SetPosEffect(FVector vSetPos)
//{
//	if (nullptr != mpSpawnedEffect)
//	{
//		//mpSpawnedComponent
//		if (IsValid(mpSpawnedEffect)) // 이펙트가 실제 유효한지 검사한다.
//		{
//			if (mpSpawnedEffect->IsActive())
//			{
//				mpSpawnedEffect->SetVariableVec3(UserParam_Pos, vSetPos);
//			}
//		}
//	}
//}

void ARtObjectActor::SetEffectArraySize(int newArraySize)
{
	int currentSize = mSpawnedEffectArray.Num();

	if (currentSize == newArraySize)
	{ // 사이즈가 같을 때..
		return;
	}
	else if (currentSize > newArraySize) // 사이즈가 줄어들 때..
	{
		for (int i = newArraySize; i < currentSize; i++)
		{
			UNiagaraComponent* NiagaraComponent = mSpawnedEffectArray[i].Get();
			if (nullptr != NiagaraComponent)
			{
				if (IsValid(NiagaraComponent))
				{
					NiagaraComponent->DestroyComponent();
				}

				mSpawnedEffectArray[i] = nullptr;
			}
		}
	}

	mSpawnedEffectArray.SetNum(newArraySize);
}

void ARtObjectActor::PlayNiagaraEffect(FString effectSystemPath, FVector spawnPos, FRotator spawnRot, float size)
{
	// Niagara 시스템 애셋을 로드하거나 에디터에서 할당
	//static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraAsset(TEXT("/Game/YourPath/YourNiagaraSystem.YourNiagaraSystem"));

	TObjectPtr<UNiagaraSystem> NiagaraSystem = LoadObject<UNiagaraSystem>(nullptr, *effectSystemPath);
	if (NiagaraSystem.Get())
	{
		// Niagara 이펙트 스폰
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			NiagaraSystem,
			spawnPos,
			spawnRot,
			FVector(size),    // 스케일
			true,             // AutoDestroy
			true,             // AutoActivate
			ENCPoolMethod::None,
			true              // PreCullCheck
		);
	}
}

void ARtObjectActor::PlayEffectArray(FString effectSystemPath, int index, FVector vStartPos)
{
	if (0 <= index && index < mSpawnedEffectArray.Num() && !effectSystemPath.IsEmpty())
	{
		TObjectPtr<UNiagaraSystem> NiagaraSystem = LoadObject<UNiagaraSystem>(nullptr, *effectSystemPath);

		bool bAttachToPlayer = false;
		bool bAutoDestroy = true;
		bool bAutoActivate = true;
		FVector Location = vStartPos;// FVector::ZeroVector;
		ENCPoolMethod PoolingMethod = ENCPoolMethod::None;
		bool bPreCullCheck = true;

		if (nullptr != mSpawnedEffectArray[index])
		{
			UNiagaraComponent* pSpawnedComponent = mSpawnedEffectArray[index].Get();
			if (IsValid(pSpawnedComponent)) // 이펙트가 실제 유효한지 검사한다.
			{
				pSpawnedComponent->DestroyComponent();
			}

			mSpawnedEffectArray[index] = nullptr;
		}

		mSpawnedEffectArray[index] = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystem, RootComponent, NAME_None,
			Location, FRotator::ZeroRotator, EAttachLocation::SnapToTarget,
			bAutoDestroy, bAutoActivate, PoolingMethod, bPreCullCheck);
	}
}

void ARtObjectActor::StopEffectArray(int index)
{
	if (0 <= index && index < mSpawnedEffectArray.Num())
	{
		if (nullptr != mSpawnedEffectArray[index])
		{
			UNiagaraComponent* pSpawnedComponent = mSpawnedEffectArray[index].Get();
			//mpSpawnedComponent
			if (IsValid(pSpawnedComponent)) // 이펙트가 실제 유효한지 검사한다.
			{
				if (pSpawnedComponent->IsActive())
				{
					pSpawnedComponent->Deactivate();
				}
			}

			mSpawnedEffectArray[index] = nullptr;
		}
	}
}

//void ARtObjectActor::SetPosEffectArray(int index, FVector vSetPos)
//{
//	if (0 <= index && index < mSpawnedEffectArray.Num())
//	{
//		if (nullptr != mSpawnedEffectArray[index])
//		{
//			UNiagaraComponent* pSpawnedComponent = mSpawnedEffectArray[index].Get();
//			
//			if (IsValid(pSpawnedComponent)) // 이펙트가 실제 유효한지 검사한다.
//			{
//				if (pSpawnedComponent->IsActive())
//				{
//					pSpawnedComponent->SetVariableVec3(UserParam_Pos, vSetPos);
//				}
//			}
//		}
//	}
//}

bool ARtObjectActor::PlayEfkEffect(FString& efkFilePath, bool bIsLoop)
{
	FString ProjectDir = FPaths::ProjectDir();
	// 상대 경로를 절대 경로로 변환

	URtResourceModelSubsystem* resModel = URtResourceModelSubsystem::Get();

	if (resModel)
	{
		ProjectDir = resModel->GetTrainingDataPath();
		ProjectDir = FPaths::Combine(ProjectDir, PROJECT_PATH_EFFECT_FILES);
		FString AbsolutePath = FPaths::ConvertRelativePathToFull(ProjectDir, efkFilePath);
		UEffekseerEmitterComponent* emitterComponent = nullptr;
		resModel->AttachEffekseerEmitterComponent(this, emitterComponent);

		if (emitterComponent)
		{
			if (resModel->SetEffekseerEffect(this, AbsolutePath))
			{
				return resModel->PlayEffekseerEffect(this, bIsLoop);
			}
		}
	}

	return false;
}

bool ARtObjectActor::PlayEfkEffect(FString& efkFilePath, const FTransform& transform, const float& speed, bool bIsLoop)
{
	FString ProjectDir = FPaths::ProjectDir();
	// 상대 경로를 절대 경로로 변환

	URtResourceModelSubsystem* resModel = URtResourceModelSubsystem::Get();

	if (resModel)
	{
		ProjectDir = resModel->GetTrainingDataPath();
		ProjectDir = FPaths::Combine(ProjectDir, PROJECT_PATH_EFFECT_FILES);
		FString AbsolutePath = FPaths::ConvertRelativePathToFull(ProjectDir, efkFilePath);
		UEffekseerEmitterComponent* emitterComponent = nullptr;

		bool battachEmitter = resModel->AttachEffekseerEmitterComponent(this, emitterComponent);


		if (emitterComponent)
		{
			emitterComponent->SetRelativeTransform(transform);
			emitterComponent->Speed = speed;
			if (resModel->SetEffekseerEffect(this, AbsolutePath))
			{
				return resModel->PlayEffekseerEffect(this, bIsLoop);
			}
		}
	}

	return false;
}

void ARtObjectActor::StopEfkEffect()
{
	URtResourceModelSubsystem* resModel = URtResourceModelSubsystem::Get();
	if (resModel)
	{
		resModel->StopEffekseerEffect(this);
	}
}

void ARtObjectActor::PlayAnimation(FString animationSequeceAnimName, bool bLooping, float animSpeed, bool bIsSystemDefault)
{
	if (animationSequeceAnimName.IsEmpty())
		return;

	if (CurrentGltfFBXAssetActor != nullptr)
	{
		FString animFileName = FPaths::GetCleanFilename(animationSequeceAnimName);
		if (!CurrentAnimationFBXFileName.IsEmpty() &&
			CurrentAnimationFBXFileName.Mid(3) == animFileName)
		{
			return;
		}

		FString SavedAnimPath;

#if WITH_EDITOR
		SavedAnimPath = FPaths::ProjectDir();
#else
		SavedAnimPath = FPaths::LaunchDir();
#endif
		FString cleanFileName;
		if (bIsSystemDefault)
		{
			FString animationPath = SavedAnimPath + FString::Printf(TEXT("/%s"), *animationSequeceAnimName);
			PlayExternalFBXAnimation(animationPath, bLooping);

			cleanFileName = FPaths::GetCleanFilename(animationSequeceAnimName);
			CurrentAnimationFBXFileName = TEXT("S"); // System
		}
		else
		{
			SavedAnimPath = FPaths::Combine(URtResourceModelSubsystem::Get()->GetTrainingDataPath(), PROJECT_PATH_FBX_AINM_FILES);

			FString animationPath = SavedAnimPath + FString::Printf(TEXT("/%s"), *animationSequeceAnimName);
			PlayExternalFBXAnimation(animationPath, bLooping);

			cleanFileName = animationSequeceAnimName;
			CurrentAnimationFBXFileName = TEXT("N"); // Normal
		}

		if (animSpeed < 0.0f)
			animSpeed = TOOL_SYSTEM_FBX_ANIMATION_DEFAULT_SPEED;

		ChangeAnimationSpeedFromFactor(animSpeed);

		if (bLooping)
		{
			CurrentAnimationFBXFileName += TEXT("L:"); // Loop Play
		}
		else
		{
			CurrentAnimationFBXFileName += TEXT("1:"); // One Play
		}

		CurrentAnimationFBXFileName += cleanFileName;
	}
}

void ARtObjectActor::PlayLegacyAnimation(FString animationSequecePath, bool bLooping)
{
	if (IsSkeletalMesh())
	{
		UAnimSequence* pAnimSequence = Cast<UAnimSequence>(StaticLoadObject(UAnimSequence::StaticClass(), nullptr, *animationSequecePath, nullptr, LOAD_None, nullptr));
		if (nullptr != pAnimSequence)
		{
			USkeletalMesh* pSkeletalMesh = mpSkeletalMeshComponent->GetSkeletalMeshAsset();
			if (nullptr != pSkeletalMesh)
			{
				pAnimSequence->SetSkeleton(pSkeletalMesh->GetSkeleton());
				mpSkeletalMeshComponent->PlayAnimation(pAnimSequence, bLooping);
			}
		}
	}
}

TPair<FString, float> ARtObjectActor::GetAnimationProgress()
{
	FString animPath = "";
	UAnimSingleNodeInstance* singleNodeInstance = NULL;
	if (IsSkeletalMesh())
		singleNodeInstance = mpSkeletalMeshComponent->GetSingleNodeInstance();
	else
	{
		USkeletalMeshComponent* skeletalMeshCom = Cast<USkeletalMeshComponent>(CurrentMeshComponent);
		if (skeletalMeshCom)
			singleNodeInstance = skeletalMeshCom->GetSingleNodeInstance();
	}

	if (singleNodeInstance == nullptr)
		return TPair<FString, float>("", -1.0f);

	if (singleNodeInstance->GetAnimationAsset())
	{
		if (IsSkeletalMesh())
			animPath = singleNodeInstance->GetAnimationAsset()->GetPathName();
		else
		{
			UAnimSequence* pSequence = Cast<UAnimSequence>(singleNodeInstance->CurrentAsset);

			if (pSequence)
			{
				const FString* value = CurrentGltfFBXExternalAnimMap.FindKey(pSequence);
				if (value)
				{
					animPath = *value;
				}
			}
		}
	}

	float fullTime = singleNodeInstance->GetLength();
	float curTime = singleNodeInstance->GetCurrentTime();


	return TPair<FString, float>(animPath, curTime / fullTime);
}

bool ARtObjectActor::IsLoopingAnim()
{
	if (IsSkeletalMesh())
	{
		UAnimSingleNodeInstance* singleNodeInstance = mpSkeletalMeshComponent->GetSingleNodeInstance();
		if (singleNodeInstance)
			return singleNodeInstance->IsLooping();
	}

	return false;
}

void ARtObjectActor::StopAnimation()
{
	if (CurrentMeshComponent.Get())
	{
		USkeletalMeshComponent* skeletalMeshCom = Cast<USkeletalMeshComponent>(CurrentMeshComponent);
		if (skeletalMeshCom)
		{
			skeletalMeshCom->Stop();
			skeletalMeshCom->PlayAnimation(nullptr, false);
			CurrentAnimationFBXFileName = TEXT("");
		}
	}
}

// speedFactor = TOOL_SYSTEM_FBX_ANIMATION_DEFAULT_SPEED : 정상 속도
void ARtObjectActor::ChangeAnimationSpeedFromFactor(float speedFactor)
{
	ChangeAnimationSpeed(GET_SPEEDVALUE_FROM_SPEEDFACTOR(speedFactor));
}

// speed = 1 : 정상 속도
void ARtObjectActor::ChangeAnimationSpeed(float speed)
{
	if (CurrentMeshComponent.Get())
	{
		USkeletalMeshComponent* skeletalMeshCom = Cast<USkeletalMeshComponent>(CurrentMeshComponent);
		if (skeletalMeshCom)
		{
			skeletalMeshCom->GlobalAnimRateScale = speed;
		}
	}
}

bool ARtObjectActor::AttachToNpc(ARtObjectActor* npc)
{
	if (npc == nullptr)
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		GetCurrentMesh()->SetRelativeRotation(GetInitCurMeshRotation());
		return true;
	}

	bool isGetTctData = false;

	ARtSceneManagerBase* smb = ARtSceneManagerBase::Get(GetWorld());
	if (smb == nullptr)
		return false;

	// 리뉴얼 이후
	if (FRtTctStruct* TctStructData = GetActorModel()->GetTctStruct().Get())
	{
		if (GetParentActor() == npc)
			return true;

		const FRtTctComDefault& transformData = TctStructData->ComDefault;
		FTransform attachTransform = FTransform(transformData.Rotation, transformData.Position, transformData.Scale);

		AttachToComponent(npc->GetCurrentMeshComponentForTransform(), FAttachmentTransformRules::KeepRelativeTransform, FName(R_HAND_SOCKET_NAME));
		SetActorRelativeTransform(attachTransform);

		return true;
	}

	// 리뉴얼 이전
	FRtTctData data = smb->GetTctDataByModelKey(GetActorModel()->GetRealModelNameKey(), isGetTctData);
	if (isGetTctData == false)
		return false;

	TArray<FRtComEquip> comEquipList = data.ComEquip;
	if (comEquipList.IsEmpty())
		return false;

	FRtComEquip comEquip = comEquipList[0];
	FName socketName = "";
	switch (comEquip.SocketType)
	{
	case ERtEquipComSocketComboType::EEquip_MouthSocket:
		socketName = MOUTH_SOCKET_NAME;
		break;
	case ERtEquipComSocketComboType::EEquip_RightHand:
		socketName = R_HAND_SOCKET_NAME;
		break;
	}

	FRtComObject comObject = data.ComObject;
	FTransform attachTransform;
	attachTransform.SetLocation(comObject.Position);
	attachTransform.SetRotation(comObject.Rotator.Quaternion());

	if (GetParentActor() == npc)
		return true;

	AttachToComponent(npc->GetSkeletalMesh(), FAttachmentTransformRules::KeepRelativeTransform, socketName);
	SetActorRelativeTransform(attachTransform);

	return true;
}

// Called when the game starts or when spawned
void ARtObjectActor::BeginPlay()
{
	Super::BeginPlay();

	if (ObjectOptionComponent)
	{
		ObjectOptionComponent->SetupComponents();
	}

	CurrentAnimationFBXFileName = TEXT("");
}

// Called every frame
void ARtObjectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARtObjectActor::Destroyed()
{
	Super::Destroyed();

	RemoveFBXAssetActor();
}

void ARtObjectActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && ObjectOptionComponent)
	{
		URtObjectOptionComponent* pObjectOption = Cast<URtObjectOptionComponent>(OtherActor->GetComponentByClass(URtObjectOptionComponent::StaticClass()));
		if (pObjectOption)
		{
			AddOverlapedObjectName(pObjectOption->GetObjectKeyName());
		}
	}
}

void ARtObjectActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		URtObjectOptionComponent* pObjectOption = Cast<URtObjectOptionComponent>(OtherActor->GetComponentByClass(URtObjectOptionComponent::StaticClass()));
		if (pObjectOption)
		{
			RemoveOverlapedObjectName(pObjectOption->GetObjectKeyName());
		}
	}
}

void ARtObjectActor::AddOverlapedObjectName(FString objectName)
{
	OverlapedObjectNameList.Add(objectName);
}

void ARtObjectActor::RemoveOverlapedObjectName(FString objectName)
{
	OverlapedObjectNameList.Remove(objectName);
}

bool ARtObjectActor::FindOverlapedObjectName(FString objectName)
{
	return OverlapedObjectNameList.Contains(objectName);
}

bool ARtObjectActor::SetDecal_StartPointNotReady(bool isNotReady)
{
	if (!Decal)
	{
		Decal = NewObject<UDecalComponent>(this);
		Decal->SetupAttachment(RootComponent);
		Decal->RegisterComponent();
		AddInstanceComponent(Decal);
	}

	if (Decal && URtResourceModelSubsystem::Get())
	{
		Decal->SetVisibility(true);
		Decal->SetWorldRotation(FRotator(90, 0, 0));
		Decal->DecalSize = FVector(25, 50, 50);

		if (URtResourceModelSubsystem::Get()->StartPosNotReadyMaterial &&
			URtResourceModelSubsystem::Get()->StartPosReadyMaterial)
		{
			Decal->SetDecalMaterial((isNotReady) ?
				URtResourceModelSubsystem::Get()->StartPosNotReadyMaterial :
				URtResourceModelSubsystem::Get()->StartPosReadyMaterial);
		}

		return true;
	}

	return false;
}

void ARtObjectActor::SetStaticMeshVisible(bool bVisible)
{
	if (mpStaticMeshComponent)
	{
		mpStaticMeshComponent->SetVisibility(bVisible);
	}
}

void ARtObjectActor::SetVisibleCollisionComponents(bool isVisible)
{
	auto boxComponents = K2_GetComponentsByClass(UBoxComponent::StaticClass());
	if (boxComponents.Num() > 0)
	{
		for (auto& item : boxComponents)
		{
			UBoxComponent* boxCom = Cast<UBoxComponent>(item);
			if (boxCom)
			{
				boxCom->SetHiddenInGame(!isVisible);
			}
		}
	}

	auto capComponents = K2_GetComponentsByClass(UCapsuleComponent::StaticClass());
	if (capComponents.Num() > 0)
	{
		for (auto& item : capComponents)
		{
			UCapsuleComponent* capCom = Cast<UCapsuleComponent>(item);
			if (capCom)
			{
				capCom->SetHiddenInGame(!isVisible);
			}
		}
	}
}

bool ARtObjectActor::IsFBXMeshType()
{
	return (ProMeshComponent == CurrentMeshComponent.Get());
}

USceneComponent* ARtObjectActor::GetCurrentMeshComponentForTransform()
{
	if (CurrentMeshComponent &&
		CurrentMeshComponent->GetAttachParent() == ProMeshParentComponent)
	{
		return ProMeshParentComponent;
	}

	return CurrentMeshComponent;
}

void ARtObjectActor::SetGltfFBXAssetActor(AglTFRuntimeFBXAssetActor* gltfFBXAssetActor)
{
	CurrentGltfFBXAssetActor = gltfFBXAssetActor;
}

void ARtObjectActor::RemoveFBXAssetActor()
{
	if (CurrentGltfFBXAssetActor)
	{
		//		FBXAssetActor->ConditionalBeginDestroy();
		CurrentGltfFBXAssetActor->Destroy();
		CurrentGltfFBXAssetActor = nullptr;

		CurrentGltfFBXExternalAnimMap.Empty();
		CurrentGltfFBXSelfAnimArray.Empty();
		CurrentGltfFBXSelfAnimIndexMap.Empty();
	}
}

FString ARtObjectActor::GetObjectName()
{
	if (ObjectOptionComponent)
	{
		return ObjectOptionComponent->GetObjectKeyName();
	}

	return FString();
}

void ARtObjectActor::AddSoundWave(USoundWave* wave)
{
	SoundWaves.Add(wave);

	if (AudioComponent.Get() == nullptr)
	{
		AudioComponent = Cast<UAudioComponent>(AddComponentByClass(UAudioComponent::StaticClass(), true, FTransform::Identity, false));
		//AudioComponent = UGameplayStatics::SpawnSoundAttached(wave, RootComponent);
		//AudioComponent->Stop();
		//AudioComponent->SetRelativeTransform(FTransform::Identity);

		AudioComponent->bAutoDestroy = false;

		AudioComponent->bIsUISound = false;
		AudioComponent->bAllowSpatialization = true;
		AudioComponent->bOverrideAttenuation = true;

		FSoundAttenuationSettings& AttenuationOverrides = AudioComponent->AttenuationOverrides;
		AttenuationOverrides.bAttenuate = true;
		AttenuationOverrides.AttenuationShapeExtents = FVector(0.0f, 0.0f, 0.0f);
		AttenuationOverrides.FalloffDistance = 1000.0f;
		AttenuationOverrides.SpatializationAlgorithm = ESoundSpatializationAlgorithm::SPATIALIZATION_HRTF;
		AttenuationOverrides.DistanceAlgorithm = EAttenuationDistanceModel::NaturalSound;
	}
}

void ARtObjectActor::SetAttenuation(float volume, float AttenuationStart, float AttenuationEnd)
{
	if (!AudioComponent)
		return;

	AudioComponent->SetVolumeMultiplier(volume);

	FSoundAttenuationSettings& AttenuationOverrides = AudioComponent->AttenuationOverrides;
	AttenuationOverrides.AttenuationShapeExtents.X = AttenuationStart;
	AttenuationOverrides.FalloffDistance = AttenuationEnd;
}

void ARtObjectActor::PlaySound(int32 audioIndex)
{
	if (!AudioComponent)
		return;

	if (SoundWaves.IsValidIndex(audioIndex))
	{
		USoundWave* newWave = SoundWaves[audioIndex];
		if (newWave->GetClass() != USoundWave::StaticClass())
			return;

		//if (newWave->bLooping && AudioComponent->GetSound() == newWave)
		//	return;

		AudioComponent->SetSound(newWave);
		AudioComponent->Play();

	}
}
