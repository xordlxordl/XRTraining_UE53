// Fill out your copyright notice in the Description page of Project Settings.


#include "RtObjectActorComponent.h"

#include "Blueprint\UserWidgetBlueprint.h"
#include "RtOverHeadMenuWidget.h"

#include "RtObjectActor.h"
#include "Engine/SkeletalMeshSocket.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

#include "Kismet/GameplayStatics.h"

#include "NiagaraComponent.h"	
#include "NiagaraDataSetReadback.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraGpuComputeDispatchInterface.h"
#include "NiagaraComputeExecutionContext.h"
#include "NiagaraScript.h"
#include "NiagaraSystem.h"
#include "NiagaraWorldManager.h"
#include "NiagaraDataSetDebugAccessor.h"


#include "RtSceneManagerBase.h"

//#include "RsGameCore\Public\Common\RsCommon.h"

//#define ON_CHANGE_BURNING_MODEL_TIME 10 // 연소 모델로 변경해야 하는 특정 시간(초). ( 이것은 데이터가 아닌 코딩으로 처리한다. )
//#define ON_CHANGE_END_BURNING_MODEL_TIME 20 // 전소 모델로 변경해야 하는 특정 시간(초). ( 이것은 데이터가 아닌 코딩으로 처리한다. )

#define FIRST_BURNING_TIME_MIN		3 // 불이 나서 중간 쯤 탄 모델로 변경되는 데 걸리는 시간(분).
#define SECOND_BURNING_TIME_MIN		4 // 중간 쯤 탄 모델로 변경된 시점에서 완전 전소된 모델로 변경되는 데 걸리는 시간(분).

#define MINUTE_SEC					60
#define ON_CHANGE_BURNING_MODEL_TIME (MINUTE_SEC*FIRST_BURNING_TIME_MIN) // 연소 모델로 변경해야 하는 특정 시간(초).
#define ON_CHANGE_END_BURNING_MODEL_TIME (ON_CHANGE_BURNING_MODEL_TIME+(MINUTE_SEC*SECOND_BURNING_TIME_MIN)) // 전소 모델로 변경해야 하는 특정 시간(초).

// Sets default values for this component's properties
URtObjectActorComponent::URtObjectActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));

	ObjectFunctionState = -1;
	SetIsUIOn(false);
	SetCloseUIClicked(false);
	SphereRadius = 0; // 값이 한 번 설정되면 계속 고정해서 쓴다.
	SetSelectMenuIndex(-1); // -1: 선택 안한 상태.

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCom"));
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &URtObjectActorComponent::OnBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &URtObjectActorComponent::OnEndOverlap);
	
//	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetCom"));
	
	//static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/RS_Fire/TestCCT/WBP_RsOverHeadMenu")); // WBP_SelectMenu
	//if (WidgetClassFinder.Succeeded())
	//{
	//	//WidgetClass = WidgetClassFinder.Class;
	//	WidgetComponent->SetWidgetClass(WidgetClassFinder.Class);

	//	int32 playerViewMode;
	//	if (GConfig && GConfig->GetInt(TEXT("RS_GAME"), TEXT("PlayerViewMode"), playerViewMode, GGameIni))
	//	{
	//		if (playerViewMode == 10) // 
	//		{
	//			WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	//		}
	//		else
	//		{
	//			WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	//			WidgetComponent->SetWorldRotation(FRotator(0,90,0));
	//			WidgetComponent->SetWorldScale3D(FVector(0.1, 0.1, 0.1));
	//		}

	//		WidgetComponent->SetWorldLocation(FVector(0, 0, 0));
	//	}
	//}

	ReferenceTctType = ERtTctType::ENone;
	ReferenceIndex = -1;
	ReferenceTctModelKey = "";

	RHandDeviceObjectActor = nullptr;
	RtSceneManager = nullptr;

//	RHandDeviceOperationCounter = 0;
	bShoot = false;

	FrameStateRate = -1;

	RHandDeviceMovingPosition = FVector();
	RHandDeviceMovingRotation = FRotator();
}

void URtObjectActorComponent::SetupComponents()
{
	//if( !SphereComponent )
	//{
	//	SphereComponent = NewObject<USphereComponent>(this, TEXT("ObjectAcor_SphereCom"));
	//}
//	APP_LOG("SetupComponents : get owner name : %s", *GetOwnerName());

	if( SphereComponent )
	{
		SphereComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}

	//if( WidgetComponent )
	//{
	//	WidgetComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	//	URtOverHeadMenuWidget* OverHeadMenuWidget = Cast<URtOverHeadMenuWidget>(WidgetComponent->GetWidget());
	//	if(OverHeadMenuWidget)
	//	{
	//		OverHeadMenuWidget->ObjectActorComponent = this;
	//	}
	//}

	//bSetupComponentsFlag = true;
}

void URtObjectActorComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AActor* pOwnerActor = GetOwner();
		URtObjectActorComponent* OtherObjectActorCom = Cast<URtObjectActorComponent>(OtherActor->GetComponentByClass(URtObjectActorComponent::StaticClass()));
		if (OtherObjectActorCom)
		{
			AddOverlapedObjectName(OtherObjectActorCom->GetOwnerName());

			//APP_LOG("OnBeginOverlap : Owner %s Other %s", *GetOwnerName(), *OtherActor->GetName());
		}
	}
}

void URtObjectActorComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		URtObjectActorComponent* OtherObjectActorCom = Cast<URtObjectActorComponent>(OtherActor->GetComponentByClass(URtObjectActorComponent::StaticClass()));
		if (OtherObjectActorCom)
		{
			RemoveOverlapedObjectName(OtherObjectActorCom->GetOwnerName());
		}
	}
}

// Called when the game starts
void URtObjectActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void URtObjectActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//if( WidgetComponent )
	//{
	//	if (WidgetComponent->IsVisible())
	//	{
	//		if (!GetIsUIOn())
	//		{
	//			WidgetComponent->SetVisibility(false);
	//		}
	//	}
	//	else
	//	{
	//		if (GetIsUIOn())
	//		{
	//			AActor* pParentActor = GetOwner();
	//			FVector pos = pParentActor->GetActorLocation();

	//			WidgetComponent->SetRelativeLocation(FVector(0, 0, 150 - pos.Z)); // 고정 위치로 표시하기 위해 설정.

	//			WidgetComponent->SetVisibility(true);
	//		}
	//	}
	//}

	if (CombustionState == ERtCombustionState::ECombustionStart)
	{
		CombustionState = ERtCombustionState::ECombustionProgressStap1;
				
		//// 이하에 불을 붙이는 코드를 넣으면 된다.
		//// xrdev mod
		//ARtHeatmapRM* HeatMap = Cast<ARtHeatmapRM>(UGameplayStatics::GetActorOfClass(GetWorld(), ARtHeatmapRM::StaticClass()));
		//if(HeatMap)
		//{
		//	AActor* pParentActor = GetOwner();
		//	HeatMap->StartTargetActorBurning(pParentActor);
		//	ReserveChangeModel();
		//
		//	//HeatMap->RegisterNewBoxOwners();
		//	//HeatMap->BurningOnPlayStart(pParentActor);
		//		
		//	//HeatMap->StartTargetActorBurning(pParentActor);
		//
		//	//int32 TimerSec = ON_CHANGE_BURNING_MODEL_TIME;
		//	//
		//	//GetWorld()->GetTimerManager().SetTimer(OnChangeModelToBurningModelTimerHandle, this, &URtObjectActorComponent::OnChangeModelToBurningModel, TimerSec, false, TimerSec);
		//}

		// 임시 코드임.
		//FString effectPath = "/Game/M5VFXVOL2/Niagara/Reference/Fireloop/3_NblowingfireB_fwd_pt.3_NblowingfireB_fwd_pt";
		//TObjectPtr<UNiagaraSystem> NiagaraSystem = LoadObject<UNiagaraSystem>(nullptr, *effectPath);

		//AActor* pParentActor = GetOwner();
		//if (pParentActor)
		//{
		//	bool bAttachToPlayer = false;
		//	bool bAutoDestroy = true;
		//	bool bAutoActivate2 = true;
		//	FVector Location = pParentActor->GetActorLocation() + GetCombustionPointOffset();//FVector::ZeroVector;
		//	ENCPoolMethod PoolingMethod = ENCPoolMethod::None;
		//	bool bPreCullCheck = true;

		//	UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystem, pParentActor->GetRootComponent(), NAME_None,
		//		Location, FRotator::ZeroRotator, EAttachLocation::SnapToTarget,
		//		bAutoDestroy, bAutoActivate2, PoolingMethod, bPreCullCheck);
		//}
	}

	int32 playerViewMode;
	if (GConfig && GConfig->GetInt(TEXT("RS_GAME"), TEXT("PlayerViewMode"), playerViewMode, GGameIni))
	{
		if (playerViewMode != 10 )// (int)ERs_PlayerViewMode::PC_ThirdPerson)
		{
			if (RHandDeviceObjectActor)
			{
				RHandDeviceObjectActor->SetActorLocation(RHandDeviceMovingPosition);
				RHandDeviceObjectActor->SetActorRotation(RHandDeviceMovingRotation);
			}
		}
	}
}

// 버닝 모델로 변경 하기 위한 타이머 예약 작업.
void URtObjectActorComponent::ReserveChangeModel()
{
	if( !IsValidChangeModelTimer() )
	{
		int32 TimerSec = ON_CHANGE_BURNING_MODEL_TIME;
		GetWorld()->GetTimerManager().SetTimer(OnChangeModelToBurningModelTimerHandle, this, &URtObjectActorComponent::OnChangeModelToBurningModel, TimerSec, false, TimerSec);

		FrameStateRate = 1.0f;
	}
}

// 전소 모델로 변경 하기 위한 타이머 예약 작업.
void URtObjectActorComponent::OnChangeModelToBurningModel()
{
	AActor* pParentActor = GetOwner();
	ARtObjectActor * pObjectActor = Cast<ARtObjectActor>(pParentActor);
	if (pObjectActor)
	{
		pObjectActor->ChangeModel(ERtModelKind::ECombustionBurningModel, 0);
	}

	CombustionState = ERtCombustionState::ECombustionProgressStap2;

	GetWorld()->GetTimerManager().ClearTimer(OnChangeModelToBurningModelTimerHandle);

	int32 TimerSec = ON_CHANGE_END_BURNING_MODEL_TIME;
	GetWorld()->GetTimerManager().SetTimer(OnChangeModelToBurningModelTimerHandle, this, &URtObjectActorComponent::OnChangeModelToEndBurningModel, TimerSec, false, TimerSec);
}

void URtObjectActorComponent::OnChangeModelToEndBurningModel()
{
	AActor* pParentActor = GetOwner();
	ARtObjectActor* pObjectActor = Cast<ARtObjectActor>(pParentActor);
	if (pObjectActor)
	{
		pObjectActor->ChangeModel(ERtModelKind::ECombustionEndBurningModel, 0);
	}

	CombustionState = ERtCombustionState::ECombustionEnd;

	GetWorld()->GetTimerManager().ClearTimer(OnChangeModelToBurningModelTimerHandle);
}

void URtObjectActorComponent::AddOverlapedObjectName(FString objectName)
{
	OverlapedObjectNameList.Add(objectName);
}

void URtObjectActorComponent::RemoveOverlapedObjectName(FString objectName)
{
	OverlapedObjectNameList.Remove(objectName);
}

bool URtObjectActorComponent::FindOverlapedObjectName(FString objectName)
{
	return OverlapedObjectNameList.Contains(objectName);
}

FString URtObjectActorComponent::GetOwnerName()
{
	return ObjectKeyName;

	//AActor* pActor = GetOwner();
	//if (pActor)
	//{
	//	return pActor->GetName();
	//}

	//return FString();
}

bool URtObjectActorComponent::SetOwnerRename(FString newName)
{
	if (newName.IsEmpty()) {
		return false;
	}

	ObjectKeyName = newName;
		
	//if (OverHeadWidgetCom)
	//{
	//	URtOverHeadWidget* pOverHeadWidget = Cast<URtOverHeadWidget>(OverHeadWidgetCom->GetWidget());
	//	if (pOverHeadWidget)
	//	{
	//		pOverHeadWidget->SetOverHeadName(ObjectKeyName);
	//	}
	//}

//	if (GIsEditor)
#if WITH_EDITOR
	{
		AActor* pActor = GetOwner();
		FString label = pActor->GetActorLabel();
		label += "_" + newName;
		pActor->SetActorLabel(*label);
	}
#endif

//	AActor* pActor = GetOwner();
//	if (pActor)
//	{
//		if (pActor->Rename(*newName, nullptr, REN_Test))
//		{
//#if WITH_EDITOR
//			FString label = pActor->GetActorLabel();
//			label += "_" + newName;
//			pActor->SetActorLabel(*label);
//#endif
//			return pActor->Rename(*newName);
//		}
//		else
//		{
////			ensureMsgf(false, TEXT("%s SetOwnerRename error!! %s"), *pActor->GetName(), *newName);
//			APP_LOG("%s SetOwnerRename error!! %s", *pActor->GetName(), *newName);
//		}
//	}

	return true;
}

void URtObjectActorComponent::SetOverHeadTextColor(FLinearColor InColorAndOpacity)
{
	if (OverHeadWidgetCom)
	{
		//URtOverHeadWidget* pOverHeadWidget = Cast<URtOverHeadWidget>(OverHeadWidgetCom->GetWidget());
		//if (pOverHeadWidget)
		//{
		//	pOverHeadWidget->SettOverHeadNameColorAndOpacity(InColorAndOpacity);
		//}
	}
}

void URtObjectActorComponent::ShowOverHeadText(bool showOverHeadText)
{
	if (OverHeadWidgetCom)
	{
		//URtOverHeadWidget* pOverHeadWidget = Cast<URtOverHeadWidget>(OverHeadWidgetCom->GetWidget());
		//if (pOverHeadWidget)
		//{
		//	pOverHeadWidget->ShowTextBlock(showOverHeadText);
		//}
	}
}

void URtObjectActorComponent::SetReferenceIndex(ERtTctType tctType, int32 index, FString modelKey)
{
	ReferenceTctType = tctType;
	ReferenceIndex = index;
	ReferenceTctModelKey = modelKey;
}

ARtSceneManagerBase* URtObjectActorComponent::FindRtSceneManager()
{
	if( nullptr == RtSceneManager )
	{
		TArray<AActor*> allObjectActors;

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARtSceneManagerBase::StaticClass(), allObjectActors);

		if (allObjectActors.Num() > 0)
		{
			ARtSceneManagerBase* pSceneManager = Cast<ARtSceneManagerBase>(allObjectActors[0]);
			if (pSceneManager)
			{
				RtSceneManager = pSceneManager;

				return RtSceneManager;
			}
		}
	}
	else 
	{
		return RtSceneManager;
	}

	return nullptr;
}

FTemplateProps URtObjectActorComponent::GetTemplatePropsInfo(int32 index, bool& bSuccess)
{
	ARtSceneManagerBase* pSceneManager = FindRtSceneManager();
	if(pSceneManager)
	{
		bSuccess = true;
		return pSceneManager->GetMctData().propsDatas[index];
	}

	bSuccess = false;
	return FTemplateProps();
}

FTemplateProps URtObjectActorComponent::GetMyTemplateProps(bool& bSuccess)
{
	if( ReferenceTctType == ERtTctType::EProp ||
		ReferenceTctType == ERtTctType::EEquipProp)
	{
		return GetTemplatePropsInfo(ReferenceIndex, bSuccess);
	}
	
	bSuccess = false;
	return FTemplateProps();
}

void URtObjectActorComponent::ShowMenu()
{
	SetIsUIOn(true);
	//URtOverHeadMenuWidget* pOverHeadWidget = Cast<URtOverHeadMenuWidget>(WidgetComponent->GetUserWidgetObject());
	//if (pOverHeadWidget)
	//{
	//	pOverHeadWidget->SetInitSelectMenuIndex();
	//}
}

void URtObjectActorComponent::OutSphere()
{
	SetIsUIOn(false);
	SetCloseUIClicked(false);
}

void URtObjectActorComponent::InSphereClosed()
{
	SetIsUIOn(false);
	SetCloseUIClicked(true);
}

AActor* URtObjectActorComponent::FindObjectActor(FString ObjectName)
{
	TArray<AActor*> AllObjectActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllObjectActors);
	for (AActor* pActor : AllObjectActors)
	{
		if (pActor->GetName() == ObjectName)
		{
			return pActor;
		}
	}

	return nullptr;
}

void URtObjectActorComponent::AttachObjectToMovingPosition(FString sourceObjectName)
{
	AActor* pSourceObjectActor = FindObjectActor(sourceObjectName);

	RHandDeviceObjectActor = pSourceObjectActor;
}

void URtObjectActorComponent::AttachObjectToSocket(FString sourceObjectName, FName targetSocketName)
{
	// 객체 생성 및 소켓에 장착
	AActor* pOwnerActor = GetOwner();
	if (pOwnerActor)
	{
		USkeletalMeshComponent* pSkeletalMeshCom = pOwnerActor->FindComponentByClass<USkeletalMeshComponent>();
		if (pSkeletalMeshCom)
		{
			AActor* pSourceObjectActor = FindObjectActor(sourceObjectName);

			if(pSourceObjectActor)
			{
				const USkeletalMeshSocket* pTargetSocket = pSkeletalMeshCom->GetSocketByName(targetSocketName);
				if (pTargetSocket)
				{
					pTargetSocket->AttachActor(pSourceObjectActor, pSkeletalMeshCom);
					URtObjectActorComponent* pSrcObjectComponent = pSourceObjectActor->FindComponentByClass<URtObjectActorComponent>();
					if (pSrcObjectComponent)
					{
						bool bOk = false;
						FTemplateProps tempProp = pSrcObjectComponent->GetMyTemplateProps(bOk);
						if (bOk)
						{
							UStaticMeshComponent * pStaticMeshCom = pSourceObjectActor->FindComponentByClass<UStaticMeshComponent>();
							if (pStaticMeshCom)
							{
								pStaticMeshCom->SetVisibility(true);
							}

							pSourceObjectActor->SetActorRelativeLocation(tempProp.PropBasic.TemplateLocation);
							pSourceObjectActor->SetActorRelativeRotation(tempProp.PropBasic.TemplateRotation);
						}

						if (targetSocketName == R_HAND_SOCKET_NAME)
						{
							RHandDeviceObjectActor = pSourceObjectActor;
						}
					}

					//TSharedPtr<RtTctData> tctData = pSourceObjectActor->GetActorModel()->GetTctData();

					//float roll_x = tctData->mRotX;
					//float pitch_y = tctData->mRotY;
					//float yaw_z = tctData->mRotZ;

					//pSourceObjectActor->SetActorRelativeLocation(FVector(tctData->mPosX, tctData->mPosY, tctData->mPosZ));
					//pSourceObjectActor->SetActorRelativeRotation(FRotator(pitch_y, yaw_z, roll_x));
				}
			}
		}
	}
}

// 오른손 장비의 작동.
void URtObjectActorComponent::RHandDeviceOperation(bool bDoOperation)
{
	if (RHandDeviceObjectActor)
	{
		URtObjectActorComponent* pSrcObjectComponent = RHandDeviceObjectActor->FindComponentByClass<URtObjectActorComponent>();
		if (pSrcObjectComponent)
		{
			bool bOk = false;
			FTemplateProps tempProp = pSrcObjectComponent->GetMyTemplateProps(bOk);
			if (bOk)
			{
				if (tempProp.PropEffect.Num() > 0)  // mct 데이터에서 먼저 가져온다.
				{
					if (bDoOperation)
					{
						PlayEffect(RHandDeviceObjectActor, tempProp.PropEffect[0].EffectPath, tempProp.PropEffect[0].EffectPos);
					}
					else 
					{
						StopEffect(RHandDeviceObjectActor);
					}

					if (tempProp.PropSound.Num() > 0)
					{

					}
				}
				else // 원본인 tct 데이터에서 가져온다.
				{
					ARtSceneManagerBase* pSceneManager = FindRtSceneManager();
					if (pSceneManager)
					{
						bool bOk2 = false;
						FRtTctData tctStruct = pSceneManager->GetTctDataByModelKey(tempProp.PropBasic.TemplateModelKey, bOk2);
						if (bOk2)
						{
							if (tctStruct.ComEffect.Num() > 0)
							{
								if (bDoOperation)
								{
									PlayEffect(RHandDeviceObjectActor, tctStruct.ComEffect[0].EffectFilePath, tctStruct.ComEffect[0].Position);
								}
								else
								{
									StopEffect(RHandDeviceObjectActor);
								}
							}
						}
					}
				}
			}
		}
	}
}

void URtObjectActorComponent::SetSkeletalMeshAsset(FString meshAssetPath)
{
	AActor* pOwnerActor = GetOwner();
	if (pOwnerActor)
	{
		USkeletalMeshComponent* pSkeletalMeshCom = pOwnerActor->FindComponentByClass<USkeletalMeshComponent>();
		if(pSkeletalMeshCom)
		{
			USkeletalMesh* pSkeletalMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *meshAssetPath));
			if(pSkeletalMesh)
			{
				pSkeletalMeshCom->SetSkeletalMeshAsset(pSkeletalMesh);
				pSkeletalMeshCom->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
			}
		}
	}
}

//void URtObjectActorComponent::PlayEffectOnOff(AActor* pParentActor, FString effectSystemPath, FVector vStartPos)
//{
//	
//	PlayEffect(AActor * pParentActor, FString effectSystemPath, FVector vStartPos)
//}

void URtObjectActorComponent::PlayEffect(AActor* pParentActor, FString effectSystemPath, FVector vStartPos)
{
	if (pParentActor && !effectSystemPath.IsEmpty())
	{
		TObjectPtr<UNiagaraSystem> NiagaraSystem = LoadObject<UNiagaraSystem>(nullptr, *effectSystemPath);

		bool bAttachToPlayer = false;
		bool bAutoDestroy = true;
		bool bAutoActivate2 = true;
		FVector Location = vStartPos;//FVector::ZeroVector;
		ENCPoolMethod PoolingMethod = ENCPoolMethod::None;
		bool bPreCullCheck = true;

		//UNiagaraComponent* SpawnedComponent = nullptr;

		StopEffect(pParentActor);
		//if (nullptr != mpSpawnedEffect) 
		//{
		//	if (IsValid(mpSpawnedEffect)) // 이펙트가 실제 유효한지 검사한다.
		//	{
		//		mpSpawnedEffect->DestroyComponent();
		//	}

		//	mpSpawnedEffect = nullptr;
		//}

		EffectNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystem, pParentActor->GetRootComponent(), NAME_None,
			Location, FRotator::ZeroRotator, EAttachLocation::SnapToTarget,
			bAutoDestroy, bAutoActivate2, PoolingMethod, bPreCullCheck);

		//pParentActor->GetCom

		URtObjectActorComponent* pEmitterObjectComponent = pParentActor->FindComponentByClass<URtObjectActorComponent>();
		if (nullptr != pEmitterObjectComponent)
		{
			pEmitterObjectComponent->SetShoot(true);
		}

		//bShoot = true;

//		EffectNiagaraComponent->SetGenerateOverlapEvents(true);
//		EffectNiagaraComponent->OnComponentBeginOverlap.AddDynamic(this, &URtObjectActorComponent::OnBeginOverlap2);

		//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem, FVector(0, 0, 0));
	}
}

void URtObjectActorComponent::StopEffect(AActor* pParentActor)
{
	if (nullptr != EffectNiagaraComponent)
	{
		//mpSpawnedComponent
		if (IsValid(EffectNiagaraComponent)) // 이펙트가 실제 유효한지 검사한다.
		{
			if (EffectNiagaraComponent->IsActive())
			{
				EffectNiagaraComponent->Deactivate();
			}
		}

		EffectNiagaraComponent = nullptr;

		URtObjectActorComponent* pEmitterObjectComponent = pParentActor->FindComponentByClass<URtObjectActorComponent>();
		if (nullptr != pEmitterObjectComponent)
		{
			pEmitterObjectComponent->SetShoot(false);
		}
	}
}


