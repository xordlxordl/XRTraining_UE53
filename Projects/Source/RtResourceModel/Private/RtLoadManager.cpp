// Fill out your copyright notice in the Description page of Project Settings.


#include "RtLoadManager.h"

// Sets default values
ARtLoadManager::ARtLoadManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARtLoadManager::BeginPlay()
{
	Super::BeginPlay();


	/*FString mctFilePath;
	//mctFilePath = "D:/RtXR/MCTData/MctData.json";
	mctFilePath = FPaths::ProjectDir();
	mctFilePath.Append("MCTData/MctData.json");

	FString SaveTestPath;
	SaveTestPath = "D:/RtXR/MCTData/SaveTest.json";

	bool isSuccess = true;
	FString OutMessage;

	TSharedPtr<FJsonObject> simulateStruct;

	simulateStruct = URtJsonFunctionLib::ReadJson(mctFilePath, isSuccess, OutMessage);

	URtJsonFunctionLib::WriteJson(SaveTestPath, simulateStruct, isSuccess, OutMessage);

	TArray<TSharedPtr<FJsonValue>> TemplateArray;

	TemplateArray = simulateStruct->GetArrayField("MCT Template Data");

	for (int32 i = 0; i < TemplateArray.Num(); i++)
	{
		TSharedPtr<FJsonValue> RsTemplate = TemplateArray[i];
		TSharedPtr<FJsonObject> RsTemplateObj = RsTemplate->AsObject();

		tempData.mModelPath = RsTemplateObj->GetStringField("ModelPath");

		tempData.mTemplatePosX = RsTemplateObj->GetNumberField("TemplatePosX");
		tempData.mTemplatePosY = RsTemplateObj->GetNumberField("TemplatePosY");
		tempData.mTemplatePosZ = RsTemplateObj->GetNumberField("TemplatePosZ");

		tempData.mTemplateRotX = RsTemplateObj->GetNumberField("TemplateRotX");
		tempData.mTemplateRotY = RsTemplateObj->GetNumberField("TemplateRotY");
		tempData.mTemplateRotZ = RsTemplateObj->GetNumberField("TemplateRotZ");



	}*/


}

// Called every frame
void ARtLoadManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

