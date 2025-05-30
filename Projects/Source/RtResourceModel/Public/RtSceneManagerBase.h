// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RtMctActor.h"
#include "RtFBXLoader.h"
#include "Models/RtActorModel.h"
#include "RtObjectActor.h"
#include "Models/RtScenarioModel.h"
#include "Models/RtActorModel2.h"
#include "Kismet/GameplayStatics.h"

//#include "RtTctStructActor.h"

#include "RtSceneManagerBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpawnRegionTriggerBoxDelegate, FTemplateArea, areaData);

class RtMoveAnimData
{
public:
	RtMoveAnimData(FVector startPos, FVector endPos, float playTime, AActor* targetActor)
	{
		ProgressPos = StartPos = startPos;
		EndPos = endPos;
		PlayTime = playTime;
		PlayTimer = 0.0f;
		TargetActor = targetActor;
	}

	bool Update(float deltaTime)
	{
		if(PlayTimer < PlayTime)
		{
			PlayTimer += deltaTime;

			if( PlayTimer >= PlayTime )
			{
				PlayTimer = PlayTime;
			}

			float ProgressRate = PlayTimer / PlayTime;

			ProgressPos = FMath::Lerp<FVector, float>(StartPos, EndPos, ProgressRate);
		}

		if (PlayTimer >= PlayTime)
		{
			return false;
		}

		return true;
	}

	FVector ProgressPos;
	float PlayTimer;

	FVector StartPos;
	FVector EndPos;
	float PlayTime;

	AActor* TargetActor;
};
//TODO
UCLASS(Blueprintable)
class RTRESOURCEMODEL_API ARtSceneManagerBase : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ARtSceneManagerBase();

	UFUNCTION(BlueprintCallable)
	void Init(ERtProductType productType) {
		ProductType = productType;
	}

	// 로드 --------------------------------------------------------------- //

	UFUNCTION(BlueprintCallable)
	FRtMCTTemplate LoadMCT(FString mctFilePath);

	// 블루프린트에서 바인딩할 델리게이트 변수
	UPROPERTY(BlueprintAssignable, Category = "ARtSceneManagerBase Delegate")
	FOnSpawnRegionTriggerBoxDelegate OnSpawnRegionTriggerBox;

	UFUNCTION(BlueprintCallable)
	void TempSetMctData(FRtMCTTemplate mctData) // 개발을 위한 임시 함수. 나중에 뺄 것임!
	{
		MctData = mctData;
	}

	UFUNCTION(BlueprintCallable)
	bool SaveMCT(FString mctFilePath);

	UFUNCTION(BlueprintCallable)
	bool SaveMCT_WallInstallInfo(FString mctWallInfoFilePath, FRtTemplateWallInstallInfo wallInstallInfo);

	UFUNCTION(BlueprintCallable)
	bool UpdateSystemActors(bool bIsNewTctStruct = false);

	bool AddActorModelFromTemplateBase(FString tctFilesPath, FTemplateBase base);

	UFUNCTION(BlueprintCallable)
	bool PrepareSpawnMapData(FString tctFilesPath, FString systemTctFilesPath, FString mctFilePath);

	UFUNCTION(BlueprintCallable)
	bool RegistSystemModels(bool bIsNewTctStruct = false);

	RtActorModel* AddActorModelByJsonString(FString modelKey, FString tctJsonString, bool bIsNewTctStruct = false);

	RtActorModel* AddActorModel(FString modelKey, FString tctFilePath, bool bIsNewTctStruct = false); //a

	UFUNCTION(BlueprintCallable)
	bool AutoAddActorModel(FString tctFilePathBase);

	// 생성 --------------------------------------------------------------- //

	template<typename TObjectActor>
	TObjectActor* SpawnActorBase(FString modelKey, FString actorName, FVector vPos, FRotator rRot, ERtProductType productType); 

	virtual AActor* SpawnActor(FString modelKey, FString actorName, float x, float y, float z, float pitch /*= 0*/, float yaw /*= 90*/, float roll /*= -90*/);
	virtual AActor* SpawnActor(FString modelKey, FString actorName, FVector vPos, FRotator rRot);

	bool AddSpawnActor(ARtObjectActor* pObjectActor, FString modelKey, float x, float y, float z, float pitch /*= 0*/, float yaw /*= 90*/, float roll /*= -90*/, bool isCreateModel=false, bool bIsNewTctStruct = false);
	
	UFUNCTION(BlueprintCallable)
	bool AddSpawnActor(ARtObjectActor* pObjectActor, FString modelKey, FVector vPos, FRotator rRot);

	//UFUNCTION(BlueprintCallable)
	//virtual AActor* AddSpawnNRegist(FString modelKey, FString actorName, FVector vPos, FRotator rRot, bool isForGame);

	// 제거 --------------------------------------------------------------- //

	bool RemoveActorModel(FString modelKey); //

	bool RemoveAllActorModel();

	bool DestroySpawnActor(ARtObjectActor* pTargetActor);

	UFUNCTION(BlueprintCallable)
	void DestroyAllSpawnActor();

	void ClearAllData()
	{
		DestroyAllSpawnActor();
	}
	
	UFUNCTION(BlueprintCallable)
	void UnloadMct()
	{
		DestroyAllSpawnActor();
		MctData = FRtMCTTemplate();
	}

	// 검색 --------------------------------------------------------------- //
	TSharedPtr<RtActorModel>* FindModel(FString modelKey) {
		TSharedPtr<RtActorModel>* pFoundModel = ObjectModelMap.Find(modelKey);
		if (pFoundModel != NULL)
		{
			return pFoundModel;
		}

		return NULL;
	}

	// 객체의 고유 이름으로 TctData 를 얻어 온다.
	UFUNCTION(BlueprintCallable)
	FRtTctData GetTctDataByObjectName(FString objectName, bool& bIsSuccess); // 레거시 함수 ( 앞으로 쓰면 안됨 ) --> GetTctDataByObjectNameInData로 대체 해야함.
	
	// 객체 모델 키값으로 TctData 를 얻어 온다.
	UFUNCTION(BlueprintCallable)
	FRtTctData GetTctDataByModelKey(FString modelKey, bool& bIsSuccess); // 레거시 함수 ( 앞으로 쓰면 안됨 ) --> GetTctStructByModelKey 로 대체 해야함.

	// 객체 모델 키값으로 TctData 를 얻어 온다.
	//UFUNCTION(BlueprintCallable)
	//FRtTctData FindTctData(FString modelKey, bool& bIsSuccess); // 이것은 추후 사용 안할 예정. 대신 GetTctDataByModelKey 함수를 쓰자.

	UFUNCTION(BlueprintCallable)
	bool GetTctStructByModelKey(FString modelKey, FRtTctStruct& tctStruct);

	UFUNCTION(BlueprintCallable)
	bool GetTctDataByObjectNameInData(FString objectName, FRtTctStruct& tctStruct);

	template<typename TT> // TT 는 FTemplateBase를 상속받는 구조체만으로 한정한다.
	bool GetTctStructByObjectNameInList(TArray<TT> list, FString objectName, FRtTctStruct& tctStruct)
	{
		for (auto& item : list) {
			if (item.TemplateName == objectName) {
				return GetTctStructByModelKey(item.TemplateModelKey, tctStruct);
			}
		}

		return false;
	}

	UFUNCTION(BlueprintCallable)
	bool GetTctStructByObjectName(FString objectName, FRtTctStruct& tctStruct);

	UFUNCTION(BlueprintCallable)
	FRtMCTTemplate & GetMctData() {
		return MctData;
	}

	UFUNCTION(BlueprintCallable)
	int GetTileSizeXFromModel(FString modelKey);

	UFUNCTION(BlueprintCallable)
	int GetTileSizeYFromModel(FString modelKey);

	UFUNCTION(BlueprintCallable)
	bool GetMctFileLoaded() const { return MctFileLoaded; }

	void SetMctFileLoaded(bool val) { MctFileLoaded = val; }

	//UFUNCTION(BlueprintCallable)
	//	void SetCurrentUIShowActor(AActor* pActor) {
	//	CurrentUIShowActor = pActor;
	//}

	virtual ARtObjectActor* FindObjectByName(FString objectName);

	//UFUNCTION(BlueprintCallable)
	//	AActor* GetCurrentUIShowActor() {
	//	return CurrentUIShowActor;
	//}

	bool DeleteObjectMctTemplateData(FString objectNameKey);

	template<class TObject, class TCharacter>
	AActor* FindObjectByObjectKeyName(FString objectKeyName) 
	{
		AActor * Found = nullptr;
		LoopProcess_ObjectActor<TObject>(
				[&](TObject* pToolObjectActor)
				{
					URtObjectOptionComponent* pObjectActorCom = Cast<URtObjectOptionComponent>(pToolObjectActor->GetComponentByClass(URtObjectOptionComponent::StaticClass()));
					if (pObjectActorCom)
					{
						if (pObjectActorCom->GetObjectKeyName() == objectKeyName)
						{
							Found = pToolObjectActor;
							return true;
						}
					}


					return false;
				}
		);
		
		if (Found)
		{
			return Found;
		}

		LoopProcess_ObjectActor<TCharacter>(
			[&](TCharacter* pToolCharacter)
			{
				URtObjectOptionComponent* pObjectActorCom = Cast<URtObjectOptionComponent>(pToolCharacter->GetComponentByClass(URtObjectOptionComponent::StaticClass()));
				if (pObjectActorCom)
				{
					if (pObjectActorCom->GetObjectKeyName() == objectKeyName)
					{
						Found = pToolCharacter;
						return true;
					}
				}

				return false;
			}
		);

		return Found;
	}

	// 생성된 객체들의 루프를 돌며 무언가 처리를 할 수 있다.
	template<class T>
	void LoopProcess_ObjectActor(std::function<bool(T*)> LoopItemAction)
	{
		TArray<AActor*> AllToolObjectActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), T::StaticClass(), AllToolObjectActors);
		for (AActor* pActor : AllToolObjectActors)
		{
			T* pToolObjectActor = Cast<T>(pActor);
			if (pToolObjectActor)
			{
				if (LoopItemAction(pToolObjectActor))
				{
					break;
				}
			}
		}
	}

	ENetMode GetNetMode();

	UFUNCTION(BlueprintCallable)
	void PlayMovePosActor(FString ObjectNameKey, FVector StartPos, FVector EndPos, float PlayTime);

	UFUNCTION(BlueprintCallable)
	AActor* AddSpawnNRegistBase(FString modelKey, FString actorName, FVector vPos, FRotator rRot, bool isForGame);

	UFUNCTION(BlueprintCallable)
	bool SpawnMapDataBase(FString tctFilesPath, FString sysFilesPath, FString mctFilePath, bool isForGame, FRotator totalRotOffset, FVector totalPosOffset);

	// 간단한 프로젝트 맵 파일 열기 함수.
	UFUNCTION(BlueprintCallable)
	bool LoadSimpleProjectMapSceneBase(FString subProjectDirName, FString subSysDirName, FString mapFileName, FRotator totalRotOffset, FVector totalPosOffset);

	// 간단한 프로젝트 시나리오 열기 함수.( 이 함수를 쓰시오 ) 
	UFUNCTION(BlueprintCallable)
	bool LoadSimpleProjectScenarioSceneBase(FString subProjectDirName, FString subSysDirName, FString scenarioFileName, FRotator totalRotOffset, FVector totalPosOffset);

	// 저급 시나리오 열기 함수.
	UFUNCTION(BlueprintCallable)
	bool LoadScenarioSceneBase(FString tctFilesPath, FString sysFilesPath, FString scenarioFilePath, bool bIsFireZone, FRotator totalRotOffset, FVector totalPosOffset);

	UFUNCTION(BlueprintCallable)
	void ObjectAllVisibleByTctType(bool visible, ERtTctType tctType);

	UFUNCTION(BlueprintCallable)
	void AllSwitchCollision(bool isForGame);
	
	UFUNCTION(BlueprintCallable)
	URtObjectOptionComponent* GetObjectOptionComponent(AActor* pActor);

	UFUNCTION(BlueprintCallable)
	FString GetComponentObjectNameKey(AActor* pActor);

	UFUNCTION(BlueprintCallable)
	bool SetComponentObjectNameKey(AActor* pActor, FString objectNameKey);

	UFUNCTION(BlueprintCallable)

	bool SetComponentObjectReferenceInfo(AActor* pActor, FString modelKey, ERtTctType tctType, ERtTctType2 tctType2, int32 index);

	UFUNCTION(BlueprintCallable)
	ERtProductType GetProductType() {
		return ProductType;
	}
	
	UFUNCTION(BlueprintCallable)
	void SetScenarioControllerRef(ARtScenarioControllerBase* scenarioController);

	UFUNCTION(BlueprintCallable)
	void ApplyExponentialHeightFog();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	// 프로젝트 경로의 서브 폴더 이름들..
	static const FString TctSubFolderName;
	static const FString FbxSubFolderName;
	static const FString PngSubFolderName;
	static const FString WavSubFolderName;
	static const FString MapSubFolderName;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<ARtObjectActor>> ObjectList; // 생성된 TCT 객체들을 관리한다.
	
	// [액토 모델 이름, 액토 모델 데이터(TCT 데이터)] : TCT 데이터 모델들을 관리한다.
	// 이것을 사용할때 따로 초기화가 필요하지 않다. 다만 프로젝트 단위의 변경이 실행 시간대에 이루어 진다면
	// 초기화가 필요하다. 하지만 프로젝트 단위의 변경이 실시간으로 일어날 일도 없으므로 그냥 툴이든 클라이언트든 
	// 계속 쌓아가면 된다.
	TMap<FString, TSharedPtr<RtActorModel>> ObjectModelMap; 

	TMap<FString, FString> DataObjectName2ModelKeyMap; // 

	//TMap<FString, TObjectPtr<URtActorModel2>> ObjectModel2Map; // [액토 모델 이름, 액토 모델 데이터(TCT 데이터)] : TCT 데이터 모델들을 관리한다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRtMCTTemplate MctData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool MctFileLoaded;

	FString ProjectPath; // 프로젝트 경로.
	
	TMap<FString, RtMoveAnimData> ObjectMoveDataMap;

	UPROPERTY(BlueprintReadOnly)
	ERtProductType ProductType = ERtProductType::ENone;

	UPROPERTY(BlueprintReadOnly)
	float TileSize = 100.0f;
	
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<ARtScenarioControllerBase> ScenarioControllerRef;
		
	UPROPERTY(BlueprintReadOnly)
	FString ErrorLog;



	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//AActor* CurrentUIShowActor; // 현재 메뉴를 보여주는 액토.

public:
	static ARtSceneManagerBase* Get(UObject* worldContext);

};

// [주의] : 이 함수의 정의는 cpp 파일에 넣으면 안된다. 템플릿 함수는 선언과 정의를 모두 헤더 파일에서 해야 한다.
template<typename TObjectActor>
TObjectActor* ARtSceneManagerBase::SpawnActorBase(FString modelKey, FString actorName, FVector vPos, FRotator rRot, ERtProductType productType)
{
	TSharedPtr<RtActorModel>* pFoundModel = ObjectModelMap.Find(modelKey);
	if (pFoundModel)
	{
		FActorSpawnParameters spawnParams;

		UWorld* pWorld = GetWorld();
		TObjectActor* pObjectActor = pWorld->SpawnActor<TObjectActor>(
			TObjectActor::StaticClass(), vPos, rRot, spawnParams);

		if (pObjectActor)
		{
			if (pObjectActor->Init(*pFoundModel))
			{
				pObjectActor->CreateModel(TEXT(""), productType);
			}

			ObjectList.Add(pObjectActor);

			return pObjectActor;
		}
	}

	return NULL;
}


