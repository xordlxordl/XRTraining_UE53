// Fill out your copyright notice in the Description page of Project Settings.


#include "RtTextureUtil.h"

#include "Runtime/ImageWrapper/Public/IImageWrapper.h"
#include "Runtime/ImageWrapper/Public/IImageWrapperModule.h"
#include "Runtime/Engine/Public/TextureResource.h"
#include "Runtime/Engine/Public/HighResScreenshot.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"

#include "ImageUtils.h"

UTexture2D* URtTextureUtil::LoadTexture2D_FromFile(const FString& FullFilePath, bool& IsValid, int32& Width, int32& Height)
{
	UTexture2D* retTexture = FImageUtils::ImportFileAsTexture2D(FullFilePath);
	IsValid = (NULL != retTexture);
	if (IsValid)
	{
		Width = retTexture->GetSizeX();
		Height = retTexture->GetSizeY();
	}

	return retTexture;

	//IsValid = false;

	//UTexture2D* LoadedT2D = NULL;

	//IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));

	//EImageFormat imageFormat = EImageFormat::PNG;

	////TArray<FString> ParseArray = {};
	////FullFilePath.ParseIntoArray(ParseArray, TEXT("."));
	////if (ParseArray.Num() > 1)
	////{
	////	FString ext = ParseArray[ParseArray.Num() - 1];
	////	if (ext == FString("tga"))
	////	{
	////		imageFormat = EImageFormat::TGA;
	////	}
	////}

	//TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(imageFormat);

	//TArray<uint8> RawFileData;

	//if (!FFileHelper::LoadFileToArray(RawFileData, *FullFilePath))
	//{
	//	return NULL;
	//}

	//if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
	//{
	//	//		const TArray<uint8> * UncompressedBRGA = NULL;
	//	TArray<uint8> UncompressedBRGA;

	//	if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBRGA))
	//	{
	//		LoadedT2D = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);

	//		if (!LoadedT2D)
	//		{
	//			return NULL;
	//		}

	//		Width = ImageWrapper->GetWidth();
	//		Height = ImageWrapper->GetHeight();

	//		void* TextureData = LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	//		FMemory::Memcpy(TextureData, UncompressedBRGA.GetData(), UncompressedBRGA.Num());
	//		LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();

	//		LoadedT2D->UpdateResource();
	//	}
	//}

	//IsValid = true;

	//return LoadedT2D;
}

UMaterialInstanceDynamic* URtTextureUtil::CreateDefaultMaterial(const FString& basePath, 
	const FString& normalPath, const FString& specularPath)
{
	FString MaterialAddress = "Material'/Game/Materials/MaterialInstanceDynamic_Base.MaterialInstanceDynamic_Base'";

	UMaterial* material;
	material = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, *MaterialAddress, nullptr, LOAD_None, nullptr));

	UMaterialInstanceDynamic* pMatInsDyn = UMaterialInstanceDynamic::Create(material, NULL);

	//FString diffuseFilePath = "C:/work/Dev/RND/unreal_total_test/character_texture/maria_diffuse.png";
	//FString normalFilePath = "C:/work/Dev/RND/unreal_total_test/character_texture/maria_normal.png";
	//FString specularFilePath = "C:/work/Dev/RND/unreal_total_test/character_texture/maria_specular.png";

	bool isValid = false;
	int32 width = 0;
	int32 height = 0;

	UTexture2D* pDiffTexture = URtTextureUtil::LoadTexture2D_FromFile(basePath, isValid, width, height);
	UTexture2D* pNormalTexture = URtTextureUtil::LoadTexture2D_FromFile(normalPath, isValid, width, height);
	UTexture2D* pSpecTexture = URtTextureUtil::LoadTexture2D_FromFile(specularPath, isValid, width, height);

	pMatInsDyn->SetTextureParameterValue(TEXT("BaseTexture"), pDiffTexture);
	pMatInsDyn->SetTextureParameterValue(TEXT("NormalTexture"), pNormalTexture);
	pMatInsDyn->SetTextureParameterValue(TEXT("SpecularTexture"), pSpecTexture);

	return pMatInsDyn;
}

UMaterialInstanceDynamic* URtTextureUtil::CreateMaterialFromBaseTexture(UMaterialInterface* pMaterialInterface, const FString& basePath)
{
	if( basePath.IsEmpty() )
	{
		return NULL;
	}

	UMaterialInstanceDynamic* pMatInsDyn = UMaterialInstanceDynamic::Create(pMaterialInterface, NULL);

	bool isValid = false;
	int32 width = 0;
	int32 height = 0;

	UTexture2D* pDiffTexture = URtTextureUtil::LoadTexture2D_FromFile(basePath, isValid, width, height);

	pMatInsDyn->SetTextureParameterValue(TEXT("BaseTexture"), pDiffTexture);

	return pMatInsDyn;
}


bool URtTextureUtil::SetMaterialFromBaseTexture(UMaterialInstanceDynamic* pMaterialInterface, const FString& basePath)
{
	if (basePath.IsEmpty())
	{
		return false;
	}

	bool isValid = false;
	int32 width = 0;
	int32 height = 0;

	UTexture2D* pDiffTexture = URtTextureUtil::LoadTexture2D_FromFile(basePath, isValid, width, height);

	pMaterialInterface->SetTextureParameterValue(TEXT("BaseTexture"), pDiffTexture);

	return true;
}
