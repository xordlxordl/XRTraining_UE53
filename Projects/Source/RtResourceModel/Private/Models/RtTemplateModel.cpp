// Fill out your copyright notice in the Description page of Project Settings.


#include "Models/RtTemplateModel.h"

#include "RtDefines.h"
#include "RtJsonFunctionLib.h"
#include "JsonObjectConverter.h"

#include "RtResourceModelSubsystem.h"

// Sets default values
ARtTemplateModel::ARtTemplateModel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARtTemplateModel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARtTemplateModel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// FRtTctStruct 구조체로 json 데이터 불러오기
bool ARtTemplateModel::LoadTctFile(FString tctFilePath, FRtTctStruct& targetData)
{
	bool bOutSuccess = false;
	FString outInfoMessage;
	TSharedPtr<FJsonObject> JsonObject = URtJsonFunctionLib::ReadJson(tctFilePath, bOutSuccess, outInfoMessage);

	if (JsonObject && bOutSuccess) {
		if (!FJsonObjectConverter::JsonObjectToUStruct<FRtTctStruct>(JsonObject.ToSharedRef(), &targetData))
		{
			APP_LOG("FRtTctStruct::LoadTctFile Error Log : FJsonObjectConverter::JsonObjectToUStruct");
			return false;
		}

		if (targetData.Version == 0)
		{
			APP_LOG("FRtTctStruct::LoadTctFile Error Log : It is Old Type Tct ~~~");
			
			if (URtResourceModelSubsystem::Get())
			{
				URtResourceModelSubsystem::Get()->LastError = ERtResModelLastError::EError_TctLoad_OldVersion;
			}
			
			return false;
		}
		return true;
	} else {
		APP_LOG("FRtTctStruct::LoadTctFile Error Log : %s", *outInfoMessage);
	}

	return false;
}

// FRtTctStruct 구조체로 json 데이터 저장하기.
bool ARtTemplateModel::SaveTctFile(FString tctFilePath, const FRtTctStruct& sourceData)
{
	TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(sourceData);
	if (JsonObject == nullptr)
	{
		return false;
	}

	bool bSuccess = false;
	FString outInfoMessage;
	URtJsonFunctionLib::WriteJson(tctFilePath, JsonObject, bSuccess, outInfoMessage);

	if(!bSuccess)
	{
		APP_LOG("FRtTctStruct::SaveTctFile Error Log : %s", *outInfoMessage);
	}

	return bSuccess;
}
