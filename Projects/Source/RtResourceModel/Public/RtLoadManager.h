// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RtJsonFunctionLib.h"
#include "RtLoadManager.generated.h"


class mctTemplateData {


public:
// Default, 모든 템플릿 공통 항목


	FString mTemplateValue;

	FString mModelName;
	FString mModelPath;

	float mTemplatePosX;
	float mTemplatePosY;
	float mTemplatePosZ;

	float mTemplateRotY;
	float mTemplateRotZ;
	float mTemplateRotX;



};

UCLASS()
class RTRESOURCEMODEL_API ARtLoadManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARtLoadManager();

	mctTemplateData tempData;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
