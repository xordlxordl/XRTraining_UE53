// Fill out your copyright notice in the Description page of Project Settings.

#include "Models/RtActorModel2.h"

#include "Serialization/JsonSerializer.h" //Json
#include "JsonObjectConverter.h" // JsonUtilities

#include "Kismet/GameplayStatics.h"

#include "RtBluePrintCommonUtil.h"

#include "EngineUtils.h"

#include "Models/RtActorModel.h"

TMap<ERtTctType, ERtComponentTypeInfo> URtActorModel2::ComInfoMap;

URtActorModel2::URtActorModel2()
{

}

URtActorModel2::~URtActorModel2()
{

}

void URtActorModel2::InitEmptyData(ERtTctType tctType)
{
	if (ComInfoMap.IsEmpty())
	{
		ComInfoMap.Add(ERtTctType::ECharacter, ERtComponentTypeInfo::EInfo_ComMix_Character);
		ComInfoMap.Add(ERtTctType::EWall, ERtComponentTypeInfo::EInfo_ComMix_Wall);
		ComInfoMap.Add(ERtTctType::ETile, ERtComponentTypeInfo::EInfo_ComMix_Tile);
		ComInfoMap.Add(ERtTctType::EProp, ERtComponentTypeInfo::EInfo_ComMix_Prop);
		ComInfoMap.Add(ERtTctType::EEquipProp, ERtComponentTypeInfo::EInfo_ComMix_EquipProp);
		ComInfoMap.Add(ERtTctType::ECar, ERtComponentTypeInfo::EInfo_ComMix_Car);
		ComInfoMap.Add(ERtTctType::EGun, ERtComponentTypeInfo::EInfo_ComMix_Gun);
		ComInfoMap.Add(ERtTctType::EFireNozzle, ERtComponentTypeInfo::EInfo_ComMix_FireNozzle);
	}
	
	FRtTctData tctData;

	tctData.ComObject.ModelName = "[new tct]";
	tctData.ComObject.TagName = "[new tct tag]";

	tctData.ComObject.TctType = tctType;

	tctData.ComObject.ModelFileName = "";
	tctData.ComObject.ModelFilePath = "";

	tctData.ComObject.TileX = 1;
	tctData.ComObject.TileY = 1;

	tctData.ComObject.Rotator = FRotator::ZeroRotator;

	tctData.ComObject.Durability = 0;

	tctData.ComObject.Position = FVector::Zero();

	TctDataList.Empty();
	TctDataList.Add(tctData);
}

bool URtActorModel2::LoadTCT(FString tctFilePath)
{
	bool bOutSuccess;
	FString outInfoMessage;
	// Try to read generic json object from file
	TSharedPtr<FJsonObject> JsonObject = URtJsonFunctionLib::ReadJson(tctFilePath, bOutSuccess, outInfoMessage);

	if (nullptr != JsonObject)
	{
		FRtTctData tempData;

		// Try to convert generic json object to the desired structure. Output goes in tempData
		if (!FJsonObjectConverter::JsonObjectToUStruct<FRtTctData>(JsonObject.ToSharedRef(), &tempData))
		{
			bOutSuccess = false;
//			outInfoMessage = FString::Printf(TEXT("Read Struct Json Failed - Was not able to convert the json object to your desired structure. Is it the rigth format / struct? - '%s'"), *JsonFilePath);
			return bOutSuccess;
		}

		TctDataList.Empty();
		TctDataList.Add(tempData);

		bOutSuccess = true;
		//outInfoMessage = FString::Printf(TEXT("Read Struct Json Succeeded - '%s'"), *JsonFilePath);
		return bOutSuccess;
	}

	return bOutSuccess;
}

bool URtActorModel2::SaveTCT(FString tctFilePath)
{
	if( TctDataList.Num() > 0 )
	{
		// Try to convert struct to generic json object
		TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(TctDataList[0]);
		if (JsonObject == nullptr)
		{
			return false;
		}

		bool bSuccess = false;
		FString OutInfoMessage;
		// Try to write json to file
		URtJsonFunctionLib::WriteJson(tctFilePath, JsonObject, bSuccess, OutInfoMessage);

		return bSuccess;
	}

	return false;
}

bool URtActorModel2::LoadFromActorModel1(RtActorModel* pActorModel)
{
	if (pActorModel)
	{
		RtTctData* pTctData = pActorModel->GetTctData().Get();
		FRtTctData TctStruct;

		// 객체
		TctStruct.ComObject.ModelName = pTctData->mModelName;
		TctStruct.ComObject.TagName = pTctData->mTagName;
		TctStruct.ComObject.TctType = pTctData->mTctType;
		TctStruct.ComObject.ModelFilePath = pTctData->mModelFilePath;
		TctStruct.ComObject.Position = FVector(pTctData->mPosX, pTctData->mPosY, pTctData->mPosZ);
		TctStruct.ComObject.Rotator = FRotator(pTctData->mRotY, pTctData->mRotZ, pTctData->mRotX);
		TctStruct.ComObject.Durability = pTctData->mDurability;

		// 머티리얼
		RtComMaterial* pComMat = static_cast<RtComMaterial*>(pTctData->GetComponent(ERtComponentType::EMaterial));
		if (pComMat)
		{
			TctStruct.ComMaterial.Add(FRtComMaterial(pComMat->mMaterialFileName, pComMat->mMaterialFilePath));
		}

		// 컬리전
		int32 colCount = pTctData->GetElementCount(ERtComponentType::ECollision);
		if (colCount > 0)
		{
			for (int32 i = 0; i < colCount; i++)
			{
				auto pElememt = pTctData->GetElementEx<RtComCollisionElement>(ERtComponentType::ECollision, i);
				if (pElememt)
				{
					TctStruct.ComCollision.Add(FRtComCollision(pElememt->mComEleType,
						FVector(pElememt->mPosX, pElememt->mPosY, pElememt->mPosZ),
						FVector(pElememt->mSizeX, pElememt->mSizeY, pElememt->mSizeZ), 
						pElememt->mName)
					);
				}
			}
		}

		// 이펙트
		int32 eftCount = pTctData->GetElementCount(ERtComponentType::EEffect);
		if (eftCount > 0)
		{
			for (int32 i = 0; i < eftCount; i++)
			{
				auto pElememt = pTctData->GetElementEx<RtComEffectElement>(ERtComponentType::EEffect, i);
				if (pElememt)
				{
					FTransform tempTransform;
					tempTransform.SetLocation(FVector(pElememt->mPosX, pElememt->mPosY, pElememt->mPosZ));
					tempTransform.SetRotation(FQuat(FRotator(pElememt->mRotY, pElememt->mRotZ, pElememt->mRotX)));
					tempTransform.SetScale3D(FVector(pElememt->mScaleX, pElememt->mScaleY, pElememt->mScaleZ));

					TctStruct.ComEffect.Add(FRtComEffect(pElememt->mEffectFileName, pElememt->mEffectFilePath, tempTransform, pElememt->mSpeed));
				}
			}
		}

		// 오디오
		int32 audCount = pTctData->GetElementCount(ERtComponentType::EAudio);
		if (audCount > 0)
		{
			for (int32 i = 0; i < audCount; i++)
			{
				auto pElememt = pTctData->GetElementEx<RtComAudioElement>(ERtComponentType::EAudio, i);
				if (pElememt)
				{
					TctStruct.ComAudio.Add(FRtComAudio(pElememt->mAudioFileName, pElememt->mAudioFilePath, pElememt->mPlayOption));
				}
			}
		}

		// 라이트
		int32 lightCount = pTctData->GetElementCount(ERtComponentType::ELight);
		if (lightCount > 0)
		{
			for (int32 i = 0; i < lightCount; i++)
			{
				auto pElememt = pTctData->GetElementEx<RtComPointLightElement>(ERtComponentType::ELight, i);
				if (pElememt)
				{
					if (pElememt->mComEleType == ERtTctComponentEleType::ELightSpot)
					{
						RtComSpotLightElement* pSpotElement = static_cast<RtComSpotLightElement*>(pElememt);
						if (pSpotElement)
						{
							TctStruct.ComLight.Add(FRtComLight(FVector(pElememt->mPosX, pElememt->mPosY, pElememt->mPosZ),
								pElememt->mIntensity, pElememt->mAttenuationRadius, FVector(pSpotElement->mDirX, pSpotElement->mDirY, pSpotElement->mDirZ),
								pSpotElement->mInnerConeAngle, pSpotElement->mOuterConeAngle));
						}
					}
					else
					{
						TctStruct.ComLight.Add(
							FRtComLight(FVector(pElememt->mPosX, pElememt->mPosY, pElememt->mPosZ), pElememt->mIntensity, pElememt->mAttenuationRadius));
					}
				}
			}
		}

		// 기능
		RtComFunction* pComFunction = static_cast<RtComFunction*>(pTctData->GetComponent(ERtComponentType::EFunction));
		if (pComFunction)
		{
			FRtComFunction comFun = FRtComFunction(pComFunction->mAniFileName, pComFunction->mAniFilePath,
				pComFunction->mEffectFileName, pComFunction->mEffectFilePath,
				FVector(pComFunction->mEffectPosX, pComFunction->mEffectPosY, pComFunction->mEffectPosZ),
				pComFunction->mSoundFileName, pComFunction->mSoundFilePath);

			int32 funCount = pTctData->GetElementCount(ERtComponentType::EFunction);
			if (funCount > 0)
			{
				for (int32 i = 0; i < funCount; i++)
				{
					auto pElememt = pTctData->GetElementEx<RtComFunElement>(ERtComponentType::EFunction, i);

					comFun.FunctionElementList.Add(FRtComFunctionElement(pElememt->mModelFileName, pElememt->mModelFilePath, pElememt->mAniFileName, pElememt->mAniFilePath,
						pElememt->mEffectFileName, pElememt->mEffectFilePath, FVector(pElememt->mEffectPosX, pElememt->mEffectPosY, pElememt->mEffectPosZ),
						pElememt->mSoundFileName, pElememt->mSoundFilePath));
				}
			}

			TctStruct.ComFunction.Add(comFun);
		}

		// 연소
		RtComCombustion* pComCom = static_cast<RtComCombustion*>(pTctData->GetComponent(ERtComponentType::ECombustion));
		if (pComCom)
		{
			TctStruct.ComCombustion.Add(FRtComCombustion(
				pComCom->mIsBurning,
				pComCom->mIgnitionPoint,
				pComCom->mBurningHP,
				pComCom->mBurningMaxDeg,
				pComCom->mBurningKind,
				pComCom->mFlameMinSize,
				pComCom->mFlameMaxSize,
				pComCom->mHeatAbsorptionRate,
				pComCom->mHeatDissipationRate,
				pComCom->mSmokeColor,
				pComCom->mSmokeSize,
				pComCom->mBurningModelName,
				pComCom->mBurningModelPath,
				pComCom->mEndBurningModelName,
				pComCom->mEndBurningModelPath
			));
		}

		// 물분사
		RtComWaterSpray* pComWat = static_cast<RtComWaterSpray*>(pTctData->GetComponent(ERtComponentType::EWaterSpray));
		if (pComWat)
		{
			TctStruct.ComWaterSpray.Add(FRtComWaterSpray(FVector(pComWat->mFirePosX, pComWat->mFirePosY, pComWat->mFirePosZ)));
		}

		// 총발사
		RtComGunFire* pComGun = static_cast<RtComGunFire*>(pTctData->GetComponent(ERtComponentType::EGunFire));
		if (pComGun)
		{
			TctStruct.ComGunFire.Add(FRtComGunFire(pComGun->mFireSpeed, FVector(pComGun->mFirePosX, pComGun->mFirePosY, pComGun->mFirePosZ)));
		}

		// 장비
		RtComEquip* pComEquip = static_cast<RtComEquip*>(pTctData->GetComponent(ERtComponentType::EEquip));
		if (pComEquip)
		{
			TctStruct.ComEquip.Add(FRtComEquip(pComEquip->mSocketType));
		}

		TctDataList.Empty();
		TctDataList.Add(TctStruct);

		return true;
	}

	return false;
}

bool FRtTctData::LoadFromActorModel1(RtActorModel* pActorModel)
{
	if (pActorModel)
	{
		RtTctData* pTctData = pActorModel->GetTctData().Get();

		// 객체
		ComObject.ModelName = pTctData->mModelName;
		ComObject.TagName = pTctData->mTagName;
		ComObject.TctType = pTctData->mTctType;
		ComObject.ModelFilePath = pTctData->mModelFilePath;
		ComObject.Position = FVector(pTctData->mPosX, pTctData->mPosY, pTctData->mPosZ);
		ComObject.Rotator = FRotator(pTctData->mRotY, pTctData->mRotZ, pTctData->mRotX);
		ComObject.Durability = pTctData->mDurability;

		// 머티리얼
		RtComMaterial* pComMat = static_cast<RtComMaterial*>(pTctData->GetComponent(ERtComponentType::EMaterial));
		if (pComMat)
		{
			ComMaterial.Add(FRtComMaterial(pComMat->mMaterialFileName, pComMat->mMaterialFilePath));
		}

		// 컬리전
		int32 colCount = pTctData->GetElementCount(ERtComponentType::ECollision);
		if (colCount > 0)
		{
			for (int32 i = 0; i < colCount; i++)
			{
				auto pElememt = pTctData->GetElementEx<RtComCollisionElement>(ERtComponentType::ECollision, i);
				if (pElememt)
				{
					ComCollision.Add(FRtComCollision(pElememt->mComEleType,
						FVector(pElememt->mPosX, pElememt->mPosY, pElememt->mPosZ),
						FVector(pElememt->mSizeX, pElememt->mSizeY, pElememt->mSizeZ),
						pElememt->mName)
					);
				}
			}
		}

		// 이펙트
		int32 eftCount = pTctData->GetElementCount(ERtComponentType::EEffect);
		if (eftCount > 0)
		{
			for (int32 i = 0; i < eftCount; i++)
			{
				auto pElement = pTctData->GetElementEx<RtComEffectElement>(ERtComponentType::EEffect, i);
				if (pElement)
				{
					ComEffect.Add(FRtComEffect(pElement->mEffectFileName, pElement->mEffectFilePath,
						FTransform(
						FRotator(pElement->mRotX, pElement->mRotY, pElement->mRotZ), 
						FVector(pElement->mPosX, pElement->mPosY, pElement->mPosZ),
						FVector(pElement->mScaleX, pElement->mScaleY, pElement->mScaleZ)
						),
						pElement->mSpeed));
				}
			}
		}

		// 오디오
		int32 audCount = pTctData->GetElementCount(ERtComponentType::EAudio);
		if (audCount > 0)
		{
			for (int32 i = 0; i < audCount; i++)
			{
				auto pElememt = pTctData->GetElementEx<RtComAudioElement>(ERtComponentType::EAudio, i);
				if (pElememt)
				{
					ComAudio.Add(FRtComAudio(pElememt->mAudioFileName, pElememt->mAudioFilePath, pElememt->mPlayOption));
				}
			}
		}

		// 라이트
		int32 lightCount = pTctData->GetElementCount(ERtComponentType::ELight);
		if (lightCount > 0)
		{
			for (int32 i = 0; i < lightCount; i++)
			{
				auto pElememt = pTctData->GetElementEx<RtComPointLightElement>(ERtComponentType::ELight, i);
				if (pElememt)
				{
					if (pElememt->mComEleType == ERtTctComponentEleType::ELightSpot)
					{
						RtComSpotLightElement* pSpotElement = static_cast<RtComSpotLightElement*>(pElememt);
						if (pSpotElement)
						{
							ComLight.Add(FRtComLight(FVector(pElememt->mPosX, pElememt->mPosY, pElememt->mPosZ),
								pElememt->mIntensity, pElememt->mAttenuationRadius, FVector(pSpotElement->mDirX, pSpotElement->mDirY, pSpotElement->mDirZ),
								pSpotElement->mInnerConeAngle, pSpotElement->mOuterConeAngle));
						}
					}
					else
					{
						ComLight.Add(
							FRtComLight(FVector(pElememt->mPosX, pElememt->mPosY, pElememt->mPosZ), pElememt->mIntensity, pElememt->mAttenuationRadius));
					}
				}
			}
		}

		// 기능
		RtComFunction* pComFunction = static_cast<RtComFunction*>(pTctData->GetComponent(ERtComponentType::EFunction));
		if (pComFunction)
		{
			FRtComFunction comFun = FRtComFunction(pComFunction->mAniFileName, pComFunction->mAniFilePath,
				pComFunction->mEffectFileName, pComFunction->mEffectFilePath,
				FVector(pComFunction->mEffectPosX, pComFunction->mEffectPosY, pComFunction->mEffectPosZ),
				pComFunction->mSoundFileName, pComFunction->mSoundFilePath);

			int32 funCount = pTctData->GetElementCount(ERtComponentType::EFunction);
			if (funCount > 0)
			{
				for (int32 i = 0; i < funCount; i++)
				{
					auto pElememt = pTctData->GetElementEx<RtComFunElement>(ERtComponentType::EFunction, i);

					comFun.FunctionElementList.Add(FRtComFunctionElement(pElememt->mModelFileName, pElememt->mModelFilePath, pElememt->mAniFileName, pElememt->mAniFilePath,
						pElememt->mEffectFileName, pElememt->mEffectFilePath, FVector(pElememt->mEffectPosX, pElememt->mEffectPosY, pElememt->mEffectPosZ),
						pElememt->mSoundFileName, pElememt->mSoundFilePath));
				}
			}

			ComFunction.Add(comFun);
		}

		// 연소
		RtComCombustion* pComCom = static_cast<RtComCombustion*>(pTctData->GetComponent(ERtComponentType::ECombustion));
		if (pComCom)
		{
			ComCombustion.Add(FRtComCombustion(
				pComCom->mIsBurning,
				pComCom->mIgnitionPoint,
				pComCom->mBurningHP,
				pComCom->mBurningMaxDeg,
				pComCom->mBurningKind,
				pComCom->mFlameMinSize,
				pComCom->mFlameMaxSize,
				pComCom->mHeatAbsorptionRate,
				pComCom->mHeatDissipationRate,
				pComCom->mSmokeColor,
				pComCom->mSmokeSize,
				pComCom->mBurningModelName,
				pComCom->mBurningModelPath,
				pComCom->mEndBurningModelName,
				pComCom->mEndBurningModelPath
			));
		}

		// 물분사
		RtComWaterSpray* pComWat = static_cast<RtComWaterSpray*>(pTctData->GetComponent(ERtComponentType::EWaterSpray));
		if (pComWat)
		{
			ComWaterSpray.Add(FRtComWaterSpray(FVector(pComWat->mFirePosX, pComWat->mFirePosY, pComWat->mFirePosZ)));
		}

		// 총발사
		RtComGunFire* pComGun = static_cast<RtComGunFire*>(pTctData->GetComponent(ERtComponentType::EGunFire));
		if (pComGun)
		{
			ComGunFire.Add(FRtComGunFire(pComGun->mFireSpeed, FVector(pComGun->mFirePosX, pComGun->mFirePosY, pComGun->mFirePosZ)));
		}

		// 장비
		RtComEquip* pComEquip = static_cast<RtComEquip*>(pTctData->GetComponent(ERtComponentType::EEquip));
		if (pComEquip)
		{
			ComEquip.Add(FRtComEquip(pComEquip->mSocketType));
		}

		return true;
	}

	return false;
}
