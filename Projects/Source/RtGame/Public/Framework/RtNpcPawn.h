// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Models/RtActorModel.h"
#include "RtEnums.h"
#include "RtObjectOptionComponent.h"

#include "RtNpcPawn.generated.h"

UCLASS()
class RTGAME_API ARtNpcPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARtNpcPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	RtTctData* GetTct()
	{
		if (ActorModel)
		{
			return ActorModel->GetTctData().Get();
		}

		return nullptr;
	}

	template<typename T>
	T* GetComponent(ERtComponentType comType)
	{
		RtTctData* pData = GetTct();
		T* pComData = StaticCast<T*>(pData->GetComponent(comType));
		IS_NOT_NULL(pComData)
		{
			return pComData;
		}
		return nullptr;
	}

	bool Init(TSharedPtr<RtActorModel> actorModel);

	bool CreateModel(FString externalModelPath, ERtProductType productType);

	void ApplyComponentData(RtTctData* pTctData, bool isForGame);

	void SetEffectArraySize(int newArraySize);

protected:
	TSharedPtr<RtActorModel> ActorModel;

	ERtModelKind CurrentModelKind;

	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY()
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	TObjectPtr<class URtObjectActorComponent> ObjectActorComponent;
	TObjectPtr<class URtObjectOptionComponent> ObjectOptionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UNiagaraComponent> SpawnedEffect; // 기능 컴포넌트 요소용..

	UPROPERTY()
	TArray<TObjectPtr<class UNiagaraComponent>> SpawnedEffectArray; // 이펙트, 기능 컴포넌트 요소용..

};
