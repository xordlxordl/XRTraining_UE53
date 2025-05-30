// Fill out your copyright notice in the Description page of Project Settings.


#include "RtJsonFunctionLib.h"
#include "RtReadWriteFile.h"
	
#include "Serialization/JsonSerializer.h" //Json
#include "JsonObjectConverter.h" // JsonUtilities


// ReadStructFromJsonFile Base
FRtMCTTemplate URtJsonFunctionLib::ReadStructFromJsonFile(FString JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage)
{
	// Try to read generic json object from file
	TSharedPtr<FJsonObject> JsonObject = ReadJson(JsonFilePath, bOutSuccess, OutInfoMessage);
	//if (!bOutSuccess)
	//{
	//	return FRtMCTTemplate();
	//}

	if( nullptr != JsonObject )
	{
		FRtMCTTemplate RetRtTemplateStruct;

		// Try to convert generic json object to the desired structure. Output goes in RetRtTemplateStruct
		if (!FJsonObjectConverter::JsonObjectToUStruct<FRtMCTTemplate>(JsonObject.ToSharedRef(), &RetRtTemplateStruct))
		{
			bOutSuccess = false;
			OutInfoMessage = FString::Printf(TEXT("Read Struct Json Failed - Was not able to convert the json object to your desired structure. Is it the rigth format / struct? - '%s'"), *JsonFilePath);
			return FRtMCTTemplate();
		}

		bOutSuccess = true;
		OutInfoMessage = FString::Printf(TEXT("Read Struct Json Succeeded - '%s'"), *JsonFilePath);
		return RetRtTemplateStruct;
	}

	return FRtMCTTemplate();
}



// WriteStructToJsonFile Base
void URtJsonFunctionLib::WriteStructToJsonFile(FString JsonFilePath, FRtMCTTemplate Struct, bool& bOutSuccess, FString& OutInfoMessage)
{
	// Try to convert struct to generic json object
	TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(Struct);
	if (JsonObject == nullptr)
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Write Struct Json Failed - Was not able to convert the struct to a json object. This shouldn't really happen. "));
		return;
	}

	// Try to write json to file
	WriteJson(JsonFilePath, JsonObject, bOutSuccess, OutInfoMessage);
}



TSharedPtr<FJsonObject> URtJsonFunctionLib::ReadJson(FString JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage)
{
	//Try to read file 
	FString JsonString = URtReadWriteFile::ReadStringFromFile(JsonFilePath, bOutSuccess, OutInfoMessage);
	if (!bOutSuccess)
	{
		return nullptr;
	}

	TSharedPtr<FJsonObject> RetJsonObject;

	// Try to convert string to json object. Output goes in RetJsonObject
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), RetJsonObject))
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Read Json Failed - Was not able to deserialize the json string. Is it the right format? - '%s'"), *JsonString);
		return nullptr;
	}

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Read Json Succeeded - '%s'"), *JsonFilePath);
	return RetJsonObject;
}


void URtJsonFunctionLib::WriteJson(FString JsonFilePath, TSharedPtr<FJsonObject> JsonObject, bool& bOutSucssess, FString& OutInfoMessage)
{
	FString JsonString;

	// Try to convert json object to string. Output goes in JsonString
	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&JsonString,0)))
	{
		bOutSucssess = false;
		OutInfoMessage = FString::Printf(TEXT("Write Json Failed - Was not able to serialize the json to string. Is the JsonObject valid?"));
		return;
	}

	// Try to write json string to file
	URtReadWriteFile::WriteStringToFile(JsonFilePath, JsonString, bOutSucssess, OutInfoMessage);
	if (!bOutSucssess)
	{
		return;
	}

	bOutSucssess = true;
	OutInfoMessage = FString::Printf(TEXT("Write Json Succeeded - '%s'"), *JsonFilePath);

}

void URtJsonFunctionLib::ParseAsComponent()
{
	TSharedPtr<FJsonObject> ComponentJsonObject = MakeShareable(new FJsonObject);
	ComponentJsonObject->SetStringField("ComponentType", "None");



	FString OutPutString;
	//TSharedPtr<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutPutString);
	FJsonSerializer::Serialize(ComponentJsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&OutPutString));


	//FJsonSerializer::Serialize(ComponentJsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&JsonString, 0))
}

//// UStruct 객체를 Json 파일로 저장한다.
//bool URtJsonFunctionLib::WriteCctDataToJsonFile(FString outputFilePath, FRtScenarioData cctData)
//{
//	// Try to convert struct to generic json object
//	TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(cctData);
//	if (JsonObject == nullptr)
//	{
//		return false;
//	}
//
//	FString newPath = outputFilePath; //FPaths::ProjectDir() + cctFilePath;
//
//	bool bSuccess = false;
//	FString OutInfoMessage;
//	// Try to write json to file
//	URtJsonFunctionLib::WriteJson(newPath, JsonObject, bSuccess, OutInfoMessage);
//
//	return bSuccess;
//}