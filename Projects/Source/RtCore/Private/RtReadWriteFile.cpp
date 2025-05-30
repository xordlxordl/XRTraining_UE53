// Fill out your copyright notice in the Description page of Project Settings.


#include "RtReadWriteFile.h"

#include "NiagaraComponent.h"	
#include "NiagaraDataSetReadback.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraGpuComputeDispatchInterface.h"
#include "NiagaraComputeExecutionContext.h"
#include "NiagaraScript.h"
#include "NiagaraSystem.h"
#include "NiagaraWorldManager.h"
#include "NiagaraDataSetDebugAccessor.h"

FString URtReadWriteFile::ReadStringFromFile(FString FilePath, bool& bOutSuccess, FString& OutInfoMessage)
{
	// Check if the file exists
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Read String From File Failed - File doesn't exist - '%s'"), *FilePath);
		return "";
	}

	FString RetString = "";

	// Try to read the file. Output goes in RetString
	if (!FFileHelper::LoadFileToString(RetString, *FilePath))
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Read String From File Failed - Was not able to read file. Is this a text file? - '%s'"), *FilePath);
		return "";
	}

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Read String From File Succeeded - '%s'"), *FilePath);
	return RetString;

}

void URtReadWriteFile::WriteStringToFile(FString FilePath, FString String, bool& bOutSuccess, FString& OutInfoMessage)
{
	// Try to write the string into the file 
	if (!FFileHelper::SaveStringToFile(String, *FilePath))
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Write Strint To File Failed - was not able to write to file. Is your file read only? Is the Path Valid? - '%s'"), *FilePath);
		return;
	}

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Write Stirng To File Succeeded - '%s'"), *FilePath);

}

UTexture2D* URtReadWriteFile::LoadTextureFromPath(const FString& UAssetPath)
{
	UTexture2D* sprite = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *UAssetPath));
	return sprite;
}

UMaterialInstance* URtReadWriteFile::LoadMaterialFromPath(const FString& UAssetPath)
{
	UMaterialInstance* mats = Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), nullptr, *UAssetPath));
	return mats;
}

UStaticMesh* URtReadWriteFile::LoadStaticMeshFromPath(const FString& UAssetPath)
{
	UStaticMesh* staticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *UAssetPath));
	return staticMesh;
}

USkeletalMesh* URtReadWriteFile::LoadSkeletalFromPath(const FString& UAssetPath)
{
	USkeletalMesh* skeletalMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *UAssetPath));
	return skeletalMesh;
}

UNiagaraSystem* URtReadWriteFile::LoadNiagaraSystemFromPath(const FString& UAssetPath)
{
	UNiagaraSystem* niagaraParticle = Cast<UNiagaraSystem>(StaticLoadObject(UNiagaraSystem::StaticClass(), nullptr, *UAssetPath));
	return niagaraParticle;
}
