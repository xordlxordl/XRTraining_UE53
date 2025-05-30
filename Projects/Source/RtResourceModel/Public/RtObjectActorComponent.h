// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Models/RtScenarioModel.h"



#include "RtObjectActorComponent.generated.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// URtObjectActorComponent 설치 가이드.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 1. 대상 객체 클래스 생성자 함수에 아래 코드를 삽입한다.
// 
// ObjectActorComponent = CreateDefaultSubobject<URtObjectActorComponent>(TEXT("Option_ObjectActorCom"));
//
// 2. 대상 객체 클래스 BeginPlay 함수에 아래 코드를 삽입한다.
// 
// if (ObjectActorComponent)
// {
//     ObjectActorComponent->SetupComponents();
// }
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangeOverHeadNameDelegate, FString);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTRESOURCEMODEL_API URtObjectActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URtObjectActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnChangeModelToBurningModel();
	void OnChangeModelToEndBurningModel();
	void ReserveChangeModel();

	//FOnChangeOverHeadNameDelegate OnChangeOverHeadNameDelegate;

public:
	//UFUNCTION(BlueprintCallable)
	//void ChangeOverHeadName(FString overHeadName);

	UFUNCTION(BlueprintCallable)
	void SetupComponents();

	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void AddOverlapedObjectName(FString objectName);

	UFUNCTION(BlueprintCallable)
	void RemoveOverlapedObjectName(FString objectName);

	UFUNCTION(BlueprintCallable)
	bool FindOverlapedObjectName(FString objectName);

	UFUNCTION(BlueprintCallable)
	FString GetOwnerName();

	UFUNCTION(BlueprintCallable)
	bool SetOwnerRename(FString newName);

	UFUNCTION(BlueprintCallable)
	void SetOverHeadTextColor(FLinearColor InColorAndOpacity);
	
	UFUNCTION(BlueprintCallable)
	void ShowOverHeadText(bool showOverHeadText);

	UFUNCTION(BlueprintCallable)
	void SetReferenceIndex(ERtTctType tctType, int32 index, FString modelKey);

	UFUNCTION(BlueprintCallable)
	ARtSceneManagerBase* FindRtSceneManager();

	UFUNCTION(BlueprintCallable)
	FTemplateProps GetTemplatePropsInfo(int32 index, bool& bSuccess);

	UFUNCTION(BlueprintCallable)
	FTemplateProps GetMyTemplateProps(bool& bSuccess);

	UFUNCTION(BlueprintCallable)
	void ShowMenu();

	UFUNCTION(BlueprintCallable)
	void OutSphere();

	UFUNCTION(BlueprintCallable)
	void InSphereClosed();

	UFUNCTION(BlueprintCallable)
	AActor* FindObjectActor(FString ObjectName);

	UFUNCTION(BlueprintCallable)
	void AttachObjectToMovingPosition(FString sourceObjectName);

	UFUNCTION(BlueprintCallable)		
	void AttachObjectToSocket(FString ObjectName, FName SocketName);

	UFUNCTION(BlueprintCallable)
	void RHandDeviceOperation(bool bDoOperation);

	//UFUNCTION(BlueprintCallable)
	//void PlayAnimation(FString animationSequecePath, bool bLooping);

	UFUNCTION(BlueprintCallable)
	void SetSkeletalMeshAsset(FString meshAssetPath);

	UFUNCTION(BlueprintCallable)
	void PlayEffect(AActor * pParentActor, FString effectSystemPath, FVector vStartPos);
	
	UFUNCTION(BlueprintCallable)
	void StopEffect(AActor* pParentActor);

	UFUNCTION(BlueprintCallable)
	bool IsEffectOn()
	{
		return (bool)EffectNiagaraComponent;
	}


	UPROPERTY(EditAnywhere)	
	USceneComponent* DefaultRoot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ObjectFunctionState; // 시뮬레이터에서 사용할 객체 기능 상태 정보.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SphereRadius; // 구 충돌 반지름.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EventID; // 현재 작동중인 이벤트 ID.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRtProcessUI UIData; // UI 표시 데이터.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<FString> OverlapedObjectNameList; // 자신의 범위 안에 들어온 객체 이름들.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* SphereComponent; // 구 충돌 체크용.

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//class UWidgetComponent* WidgetComponent; // UI 띄우기용.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> EquipList; // 장착한 장비 이름 리스트.

	bool GetCloseUIClicked() const { return bCloseUIClicked; }
	void SetCloseUIClicked(bool val) { bCloseUIClicked = val; }
	
	bool GetIsUIOn() const { return bIsUIOn; }
	void SetIsUIOn(bool val) { bIsUIOn = val; }

	int32 GetSelectMenuIndex() const { return SelectMenuIndex; }
	void SetSelectMenuIndex(int32 val) { SelectMenuIndex = val; }

	ERtCombustionState GetCombustionState() const { return CombustionState; }
	void SetCombustionState(ERtCombustionState val) { CombustionState = val; }

	FVector GetCombustionPointOffset() const { return CombustionPointOffset; }
	void SetCombustionPointOffset(FVector val) { CombustionPointOffset = val; }

	bool IsValidChangeModelTimer() {
		return OnChangeModelToBurningModelTimerHandle.IsValid();
	}

	void ClearChangeModelTimer() 
	{
		GetWorld()->GetTimerManager().ClearTimer(OnChangeModelToBurningModelTimerHandle);
	}

	//UFUNCTION(BlueprintCallable)
	//void InitRHandDeviceOperationCounter()
	//{
	//	RHandDeviceOperationCounter = 0;
	//}

	bool GetShoot() const { return bShoot; }
	void SetShoot(bool val) { bShoot = val; }

	UFUNCTION(BlueprintCallable)
	URtObjectActorComponent* GetRHandDeviceObjectActorCom()
	{
		if (nullptr != RHandDeviceObjectActor)
		{
			URtObjectActorComponent* otherObjectActorCom = Cast<URtObjectActorComponent>(RHandDeviceObjectActor->GetComponentByClass(URtObjectActorComponent::StaticClass()));

			return otherObjectActorCom;
		}

		return nullptr;
	}

	UFUNCTION(BlueprintCallable)
	AActor* GetRHandDeviceObjectActor() 
	{
		return RHandDeviceObjectActor;
	}

	UFUNCTION(BlueprintCallable)
	void SetRHandDeviceMovingTransform(FVector pos, FRotator rot) {
		RHandDeviceMovingPosition = pos;
		RHandDeviceMovingRotation = rot;
	}

	UFUNCTION(BlueprintCallable)
	void SetRHandDeviceMovingPosition(FVector pos) {
		RHandDeviceMovingPosition = pos;
	}

	UFUNCTION(BlueprintCallable)
	void SetRHandDeviceMovingRotation(FRotator rot) {
		RHandDeviceMovingRotation = rot;
	}

	float GetFrameStateRate() const { return FrameStateRate; }

	void SetFrameStateRate(float val) { FrameStateRate = val; }

	UFUNCTION(BlueprintCallable)
	FString GetObjectKeyName() {
		return ObjectKeyName;
	}

	UFUNCTION(BlueprintCallable)
	void SetObjectKeyName(FString objKeyName) {
		ObjectKeyName = objKeyName;
	}

	UFUNCTION(BlueprintCallable)
	FString GetObjectModelKeyName()
	{
		return ReferenceTctModelKey;
	}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> OverHeadWidgetCom;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERtCombustionState CombustionState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CombustionPointOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SelectMenuIndex; // 선택 메뉴 색인 (0~ )

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsUIOn; // UI 표시 상태 정보.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCloseUIClicked; // UI 표시 상태에서 닫기를 클릭한 상태 정보.
		
	FTimerHandle OnChangeModelToBurningModelTimerHandle; // 연소중 모델로 교체하기 위한 타이머.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERtTctType ReferenceTctType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ReferenceIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ReferenceTctModelKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraComponent* EffectNiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* RHandDeviceObjectActor; // 총 또는 관창.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ARtSceneManagerBase* RtSceneManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShoot; // 총 또는 관창 발사 할 경우 설정.

	UPROPERTY(VisibleAnywhere)
	float FrameStateRate;  // 화염의 정도를 나타냄. ( 0 ~ 1 ) 1 은 최대 화염 상태, -1 은 초기상태(정상상태). 

	UPROPERTY(VisibleAnywhere)
	FVector RHandDeviceMovingPosition;

	UPROPERTY(VisibleAnywhere)
	FRotator RHandDeviceMovingRotation;

	// 객체의 식별을 위한 키 이름 ( 기존에 AActor 의 name 값을 써보려 했으나 이것은 
	// 언리얼 시스템에서 관리 되는 녀석이라 별도의 캐싱으로 인해 마음대로 지우고 생성하는 것이 
	// 자유롭지 못한 이슈가 있어서 별도의 키값을 가지는 것으로 변경한다 )
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ObjectKeyName; 
};

