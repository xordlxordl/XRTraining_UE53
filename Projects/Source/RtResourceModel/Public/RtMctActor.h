// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RtEnums.h"
#include "Models/RtActorModel.h"
#include "GameFramework/Actor.h"
#include "RtMctActor.generated.h"

/* UCLASS ǥ�� ���� : public UObject�� ����ؾ� �ϸ� Ŭ���� �̸� �տ� 'U' �� �ٿ� �־�� ��
*  BlueprintType �̶�� �����ڸ� �ٿ������μ� �������Ʈ ���̾�׷����� ������ ����� �� �ְ� �ȴ�
UCLASS(BlueprintType)
class URtMCTDatas : public UObject
{
	GENERATED_BODY()


};
*/




UCLASS()
class RTRESOURCEMODEL_API ARtMctActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARtMctActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};







