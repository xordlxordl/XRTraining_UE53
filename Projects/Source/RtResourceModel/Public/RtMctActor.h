// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RtEnums.h"
#include "Models/RtActorModel.h"
#include "GameFramework/Actor.h"
#include "RtMctActor.generated.h"

/* UCLASS 표준 형식 : public UObject를 상속해야 하며 클래스 이름 앞에 'U' 를 붙여 주어야 함
*  BlueprintType 이라는 지정자를 붙여줌으로서 블루프린트 다이어그램에서 변수로 사용할 수 있게 된다
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







