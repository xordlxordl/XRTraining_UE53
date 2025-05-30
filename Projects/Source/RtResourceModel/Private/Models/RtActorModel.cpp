// Fill out your copyright notice in the Description page of Project Settings.

#include "Models/RtActorModel.h"

#include "RtDefines.h"

#include "RtFbxLoaderSubsystem.h"

TMap<ERtTctType, ERtComponentTypeInfo> RtActorModel::mComInfoMap;

// 객체 컴포넌트 태그명
const char* RtTctData::obj_model_name		= "name";
const char* RtTctData::obj_tag_name			= "tag";
const char* RtTctData::obj_tct_type			= "tct_type";
const char* RtTctData::obj_model_file_name	= "model_file_name";
const char* RtTctData::obj_model_file_path	= "model_file_path";
const char* RtTctData::obj_tile_x			= "tile_size_x";
const char* RtTctData::obj_tile_y			= "tile_size_y";
const char* RtTctData::obj_durability		= "durability";
const char* RtTctData::obj_pos_x			= "pos_x";
const char* RtTctData::obj_pos_y			= "pos_y";
const char* RtTctData::obj_pos_z			= "pos_z";
const char* RtTctData::obj_rot_x = "rot_x";
const char* RtTctData::obj_rot_y = "rot_y";
const char* RtTctData::obj_rot_z = "rot_z";
const char* RtTctData::obj_scale_x = "scale_x";
const char* RtTctData::obj_scale_y = "scale_y";
const char* RtTctData::obj_scale_z = "scale_z";

// 충돌 컴포넌트 태그명
const char* RtTctData::collision_com			= "collision_com";
const char* RtTctData::col_ele_type				= "col_ele_type";
const char* RtTctData::col_x					= "col_x";
const char* RtTctData::col_y					= "col_y";
const char* RtTctData::col_z					= "col_z";
const char* RtTctData::col_sx					= "col_sx";
const char* RtTctData::col_sy					= "col_sy";
const char* RtTctData::col_sz					= "col_sz";
const char* RtTctData::col_name					= "col_name";

// 머티리얼 컴포넌트 태그명
const char* RtTctData::material_com				= "material_com";
const char* RtTctData::mat_tex_file_name		= "mat_tex_file_name";
const char* RtTctData::mat_tex_file_path		= "mat_tex_file_path";

// 라이트 컴포넌트 태그명
const char* RtTctData::light_com				= "lit_com";
const char* RtTctData::lit_ele_type				= "lit_ele_type";
const char* RtTctData::lit_pos_x				= "lit_pos_x";
const char* RtTctData::lit_pos_y				= "lit_pos_y";
const char* RtTctData::lit_pos_z				= "lit_pos_z";
const char* RtTctData::lit_intensity			= "lit_intensity";
const char* RtTctData::lit_radius				= "lit_radius";
const char* RtTctData::lit_dir_x				= "lit_dir_x";
const char* RtTctData::lit_dir_y				= "lit_dir_y";
const char* RtTctData::lit_dir_z				= "lit_dir_z";
const char* RtTctData::lit_inner_angle			= "lit_inner_angle";
const char* RtTctData::lit_outer_angle			= "lit_outer_angle";

// 오디오 컴포넌트 태그명
const char* RtTctData::audio_com				= "audio_com";
const char* RtTctData::aud_file_name			= "aud_file_name";
const char* RtTctData::aud_file_path			= "aud_file_path";
const char* RtTctData::aud_play_option			= "aud_play_option";

// 물 분사 컴포넌트 태그명
const char* RtTctData::water_spray_com			= "water_spray_com";
const char* RtTctData::wsp_pos_x				= "wsp_pos_x";
const char* RtTctData::wsp_pos_y				= "wsp_pos_y";
const char* RtTctData::wsp_pos_z				= "wsp_pos_z";

// 연소 컴포넌트 태그명.
const char* RtTctData::combustion_com				= "combustion_com";
const char* RtTctData::com_is_burning				= "com_is_burning";
const char* RtTctData::com_ignition_point			= "com_ignition_point";
const char* RtTctData::com_burning_hp				= "com_burning_hp";
const char* RtTctData::com_burning_max_deg			= "com_burning_max_deg";
const char* RtTctData::com_burning_kind				= "com_burning_kind";
const char* RtTctData::com_flame_min_size			= "com_flame_min_size";
const char* RtTctData::com_flame_max_size			= "com_flame_max_size";
const char* RtTctData::com_heat_absorption_rate		= "com_heat_absorption_rate";
const char* RtTctData::com_heat_dissipation_rate	= "com_heat_dissipation_rate";
const char* RtTctData::com_smoke_color				= "com_smoke_color";
const char* RtTctData::com_smoke_size				= "com_smoke_size";
const char* RtTctData::com_burning_model_name		= "com_burning_model_name";
const char* RtTctData::com_burning_model_path		= "com_burning_model_path";
const char* RtTctData::com_end_burning_model_name	= "com_end_burning_model_name";
const char* RtTctData::com_end_burning_model_path	= "com_end_burning_model_path";

// 장착 컴포넌트 태그명.
const char* RtTctData::equip_com				= "equip_com";
const char* RtTctData::equip_sockettype			= "equip_sockettype";

// 총알 발사 컴포넌트 태그명
const char* RtTctData::gun_fire_com				= "gun_fire_com";
const char* RtTctData::gf_speed					= "gf_speed";
const char* RtTctData::gf_pos_x					= "gf_pos_x";
const char* RtTctData::gf_pos_y					= "gf_pos_y";
const char* RtTctData::gf_pos_z					= "gf_pos_z";

// 이펙트 컴포넌트 태그명
const char* RtTctData::effect_com				= "effect_com";
const char* RtTctData::eft_ele_type				= "eft_ele_type";
const char* RtTctData::eft_pos_x				= "eft_pos_x";
const char* RtTctData::eft_pos_y				= "eft_pos_y";
const char* RtTctData::eft_pos_z				= "eft_pos_z";
const char* RtTctData::eft_rot_x = "eft_rot_x";
const char* RtTctData::eft_rot_y = "eft_rot_y";
const char* RtTctData::eft_rot_z = "eft_rot_z";
const char* RtTctData::eft_scale_x = "eft_scale_x";
const char* RtTctData::eft_scale_y = "eft_scale_y";
const char* RtTctData::eft_scale_z = "eft_scale_z";
const char* RtTctData::eft_speed = "eft_speed";
const char* RtTctData::eft_file_name			= "eft_file_name";
const char* RtTctData::eft_file_path			= "eft_file_path";

// 기능 컴포넌트 태그명
const char* RtTctData::function_com				= "function_com";
const char* RtTctData::fun_ele_type				= "fun_ele_type";
const char* RtTctData::fun_element				= "fun_element";
const char* RtTctData::fun_ani_filename			= "fun_ani_filename";
const char* RtTctData::fun_ani_filepath			= "fun_ani_filepath";
const char* RtTctData::fun_eft_filename			= "fun_eft_filename";
const char* RtTctData::fun_eft_filepath			= "fun_eft_filepath";
const char* RtTctData::fun_eft_pos_x			= "fun_eft_pos_x";
const char* RtTctData::fun_eft_pos_y			= "fun_eft_pos_y";
const char* RtTctData::fun_eft_pos_z			= "fun_eft_pos_z";
const char* RtTctData::fun_snd_filename			= "fun_snd_filename";
const char* RtTctData::fun_snd_filepath			= "fun_snd_filepath";
const char* RtTctData::fun_model_filename		= "fun_model_filename";
const char* RtTctData::fun_model_filepath		= "fun_model_filepath";

RtActorModel::RtActorModel()
{
}

RtActorModel::~RtActorModel()
{
}

void RtActorModel::InitEmptyData(ERtTctType tctType)
{
	if (mComInfoMap.IsEmpty())
	{
		mComInfoMap.Add(ERtTctType::ECharacter, ERtComponentTypeInfo::EInfo_ComMix_Character);
		mComInfoMap.Add(ERtTctType::EWall, ERtComponentTypeInfo::EInfo_ComMix_Wall);
		mComInfoMap.Add(ERtTctType::ETile, ERtComponentTypeInfo::EInfo_ComMix_Tile);
		mComInfoMap.Add(ERtTctType::EProp, ERtComponentTypeInfo::EInfo_ComMix_Prop);
		mComInfoMap.Add(ERtTctType::EEquipProp, ERtComponentTypeInfo::EInfo_ComMix_EquipProp);
		mComInfoMap.Add(ERtTctType::ECar, ERtComponentTypeInfo::EInfo_ComMix_Car);
		mComInfoMap.Add(ERtTctType::EGun, ERtComponentTypeInfo::EInfo_ComMix_Gun);
		mComInfoMap.Add(ERtTctType::EFireNozzle, ERtComponentTypeInfo::EInfo_ComMix_FireNozzle);
	}

	if (!mTctData.IsValid())
	{
		mTctData = MakeShared<RtTctData>();

		mTctData->mModelName = "[new tct]";
		mTctData->mTagName = "[new tct tag]";
		
		mTctData->mTctType = tctType;
		
		mTctData->mModelFileName = "";
		mTctData->mModelFilePath = "";

		mTctData->mTileX = 1;
		mTctData->mTileY = 1;

		mTctData->mRotX = 0;
		mTctData->mRotY = 0;
		mTctData->mRotZ = 0;

		mTctData->mDurability = 0;

		mTctData->mPosX = 0;
		mTctData->mPosY = 0;
		mTctData->mPosZ = 0;

		ERtComponentTypeInfo *pComTypeInfo = mComInfoMap.Find(tctType);
		if( NULL != pComTypeInfo )
		{
			if ((*pComTypeInfo) & ERtComponentTypeInfo::EInfoCollision)
			{
				mTctData->AddComponent<RtTctData::RtComCollision>();
			}

			if ((*pComTypeInfo) & ERtComponentTypeInfo::EInfoMaterial)
			{
				mTctData->AddComponent<RtTctData::RtComMaterial>();
			} 

			if ((*pComTypeInfo) & ERtComponentTypeInfo::EInfoAnimationViewer)
			{
				mTctData->AddComponent<RtTctData::RtComAniViewer>();
			}

			if ((*pComTypeInfo) & ERtComponentTypeInfo::EInfoAudio)
			{
				mTctData->AddComponent<RtTctData::RtComAudio>();
			}

			if ((*pComTypeInfo) & ERtComponentTypeInfo::EInfoLight)
			{
				mTctData->AddComponent<RtTctData::RtComLight>();
			}

			if ((*pComTypeInfo) & ERtComponentTypeInfo::EInfoEffect)
			{
				mTctData->AddComponent<RtTctData::RtComEffect>();
			}

			if ((*pComTypeInfo) & ERtComponentTypeInfo::EInfoFunction)
			{
				mTctData->AddComponent<RtTctData::RtComFunction>();
			}

			if ((*pComTypeInfo) & ERtComponentTypeInfo::EInfoGunFire)
			{
				mTctData->AddComponent<RtTctData::RtComGunFire>();
			}

			if ((*pComTypeInfo) & ERtComponentTypeInfo::EInfoWaterSpray)
			{
				mTctData->AddComponent<RtTctData::RtComWaterSpray>();
			}

			if ((*pComTypeInfo) & ERtComponentTypeInfo::EInfoCombustion)
			{
				mTctData->AddComponent<RtTctData::RtComCombustion>();
			}

			if ((*pComTypeInfo) & ERtComponentTypeInfo::EEquip)
			{
				mTctData->AddComponent<RtTctData::RtComEquip>();
			}

			if ((*pComTypeInfo) & ERtComponentTypeInfo::EInfoInteract)
			{
				mTctData->AddComponent<RtTctData::RtComInteract>();
			}

			if ((*pComTypeInfo) & ERtComponentTypeInfo::EInfoInsideModel)
			{
				mTctData->AddComponent<RtTctData::RtComInsideModel>();
			}

		}
	}

	if (!NewTctData.IsValid())
	{
		NewTctData = MakeShared<FRtTctData>();
	}
}

// 리뉴얼 되는 빈 새 TCT 구조체 데이터를 만든다.
void RtActorModel::InitEmptyData2(ERtTctType2 tctType)
{
	GetTctStruct(false);
	TctStruct->ComDefault.TctType = tctType;
	TctStruct->ComDefault.ModelFileName = TEXT("");
	TctStruct->ComDefault.ModelName = TEXT("");
}

bool RtActorModel::LoadTCTJsonObject(TSharedPtr<FJsonObject> tctJsonObject, bool bIsNewTctStruct)
{
	if (tctJsonObject.IsValid())
	{
		if (bIsNewTctStruct)
		{
			GetTctStruct(false);
			TctStruct->SetVersion();
			TctStruct->SetModelName(tctJsonObject->GetStringField(RtTctData::obj_model_name));
			TctStruct->ComDefault.TctType = (ERtTctType2)tctJsonObject->GetIntegerField(RtTctData::obj_tct_type); // 시스템 타입은 값을 맞추어서 그대로 써도 됨.
			TctStruct->ComDefault.ModelFileName = tctJsonObject->GetStringField(RtTctData::obj_model_file_path); // 일부로 파일 패스를 입력 함.
			TctStruct->ComDefault.Rotation.Roll = tctJsonObject->GetNumberField(RtTctData::obj_rot_x);
			TctStruct->ComDefault.Rotation.Pitch = tctJsonObject->GetNumberField(RtTctData::obj_rot_y);
			TctStruct->ComDefault.Rotation.Yaw = tctJsonObject->GetNumberField(RtTctData::obj_rot_z);
			TctStruct->ComDefault.Position.X = tctJsonObject->GetNumberField(RtTctData::obj_pos_x);
			TctStruct->ComDefault.Position.Y = tctJsonObject->GetNumberField(RtTctData::obj_pos_y);
			TctStruct->ComDefault.Position.Z = tctJsonObject->GetNumberField(RtTctData::obj_pos_z);
			TctStruct->ComDefault.Scale.X = tctJsonObject->GetNumberField(RtTctData::obj_scale_x);
			TctStruct->ComDefault.Scale.Y = tctJsonObject->GetNumberField(RtTctData::obj_scale_y);
			TctStruct->ComDefault.Scale.Z = tctJsonObject->GetNumberField(RtTctData::obj_scale_z);

			return true;
		}
		else
		{
			if (!mTctData.IsValid())
			{
				mTctData = MakeShared<RtTctData>();
			}

			mTctData->mModelName = tctJsonObject->GetStringField(RtTctData::obj_model_name);
			mTctData->mTagName = tctJsonObject->GetStringField(RtTctData::obj_tag_name);
			mTctData->mTctType = (ERtTctType)tctJsonObject->GetIntegerField(RtTctData::obj_tct_type);
			mTctData->mModelFileName = tctJsonObject->GetStringField(RtTctData::obj_model_file_name);
			mTctData->mModelFilePath = tctJsonObject->GetStringField(RtTctData::obj_model_file_path);
			mTctData->mTileX = tctJsonObject->GetNumberField(RtTctData::obj_tile_x);
			mTctData->mTileY = tctJsonObject->GetNumberField(RtTctData::obj_tile_y);
			mTctData->mRotX = tctJsonObject->GetNumberField(RtTctData::obj_rot_x);
			mTctData->mRotY = tctJsonObject->GetNumberField(RtTctData::obj_rot_y);
			mTctData->mRotZ = tctJsonObject->GetNumberField(RtTctData::obj_rot_z);
			mTctData->mDurability = tctJsonObject->GetNumberField(RtTctData::obj_durability);
			mTctData->mPosX = tctJsonObject->GetNumberField(RtTctData::obj_pos_x);
			mTctData->mPosY = tctJsonObject->GetNumberField(RtTctData::obj_pos_y);
			mTctData->mPosZ = tctJsonObject->GetNumberField(RtTctData::obj_pos_z);
			mTctData->mScaleX = tctJsonObject->GetNumberField(RtTctData::obj_scale_x);
			mTctData->mScaleY = tctJsonObject->GetNumberField(RtTctData::obj_scale_y);
			mTctData->mScaleZ = tctJsonObject->GetNumberField(RtTctData::obj_scale_z);

			if (mTctData->mTctType < ERtTctType::EWayPoint)
			{
				LoadTCT_Com_Collision(tctJsonObject); // 충돌 컴포넌트.
				LoadTCT_Com_Material(tctJsonObject); // 머티리얼 컴포넌트.
				LoadTCT_Com_Audio(tctJsonObject); // 오디오 컴포넌트.
				LoadTCT_Com_Light(tctJsonObject); // 라이트 컴포넌트.
				LoadTCT_Com_Combustion(tctJsonObject); // 연소 컴포넌트.
				LoadTCT_Com_WaterSpray(tctJsonObject); // 물 분사 컴포넌트.
				LoadTCT_Com_GunFire(tctJsonObject); // 총알 발사 컴포넌트.
				LoadTCT_Com_Effect(tctJsonObject); // 이펙트 컴포넌트.
				LoadTCT_Com_Function(tctJsonObject); // 기능 컴포넌트.
				LoadTCT_Com_Equip(tctJsonObject); // 장비 장착 컴포넌트.
			}

			if (!NewTctData.IsValid())
			{
				NewTctData = MakeShared<FRtTctData>();
				NewTctData->LoadFromActorModel1(this);
			}

			return true;
		}
	}

	return false;
}

bool RtActorModel::LoadTCT(FString tctFilePath, bool bIsNewTctStruct)
{
	if (bIsNewTctStruct) {
		GetTctStruct(false);
		return ARtTemplateModel::LoadTctFile(tctFilePath, *TctStruct.Get());
	}

	bool isSuccess = false;
	FString outMessage;

	TSharedPtr<FJsonObject> tctJsonObject = URtJsonFunctionLib::ReadJson(tctFilePath, isSuccess, outMessage);
	if (isSuccess)
	{
		RealModelNameKey = FPaths::GetBaseFilename(tctFilePath);

		LoadTCTJsonObject(tctJsonObject, bIsNewTctStruct);
	}

	return isSuccess;
}

bool RtActorModel::LoadTCTJsonString(FString modelNameKey, FString tctJsonString, bool bIsNewTctStruct)
{
	if (modelNameKey.IsEmpty() || tctJsonString.IsEmpty()) {
		return false;
	}

	TSharedPtr<FJsonObject> tctJsonObject;
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(tctJsonString), tctJsonObject))
	{
		return false;
	}

	RealModelNameKey = modelNameKey;

	return LoadTCTJsonObject(tctJsonObject, bIsNewTctStruct);
}

void RtActorModel::LoadTCT_Com_Collision(TSharedPtr<FJsonObject> tctJsonObject)
{
	if (!tctJsonObject->HasField(RtTctData::collision_com))
	{
		return;
	}

	auto component = mTctData->AddComponent<RtTctData::RtComCollision>(); //RtTctData::RtComCollision

	TArray<TSharedPtr<FJsonValue>> jvArray = tctJsonObject->GetArrayField(RtTctData::collision_com);
	if (jvArray.Num() > 0)
	{
		for (int32 i = 0; i < jvArray.Num(); i++)
		{
			TSharedPtr<FJsonValue> JsonValue = jvArray[i];
			TSharedPtr<FJsonObject> JsonValueObject = JsonValue->AsObject();

			auto element = component->AddElement(); //AddElementBase<RtTctData::RtComCollision::RtElement>();
			element->mComEleType = (ERtTctComponentEleType)JsonValueObject->GetIntegerField(RtTctData::col_ele_type);

			element->mName = JsonValueObject->GetStringField(RtTctData::col_name);

			element->mPosX = JsonValueObject->GetNumberField(RtTctData::col_x);
			element->mPosY = JsonValueObject->GetNumberField(RtTctData::col_y);
			element->mPosZ = JsonValueObject->GetNumberField(RtTctData::col_z);

			element->mSizeX = JsonValueObject->GetNumberField(RtTctData::col_sx);
			element->mSizeY = JsonValueObject->GetNumberField(RtTctData::col_sy);
			element->mSizeZ = JsonValueObject->GetNumberField(RtTctData::col_sz);
		}
	}
}

void RtActorModel::LoadTCT_Com_Material(TSharedPtr<FJsonObject> tctJsonObject)
{
	if (!tctJsonObject->HasField(RtTctData::material_com))
	{
		return;
	}

	TSharedPtr<FJsonObject> joMatCom = tctJsonObject->GetObjectField(RtTctData::material_com);
	if (joMatCom.IsValid())
	{
		auto component = mTctData->AddComponent<RtTctData::RtComMaterial>();

		component->mMaterialFileName = joMatCom->GetStringField(RtTctData::mat_tex_file_name);
		component->mMaterialFilePath = joMatCom->GetStringField(RtTctData::mat_tex_file_path);
	}
}

void RtActorModel::LoadTCT_Com_Audio(TSharedPtr<FJsonObject> tctJsonObject)
{
	if( !tctJsonObject->HasField(RtTctData::audio_com) ) 
	{
		return;
	}	

	auto component = mTctData->AddComponent<RtTctData::RtComAudio>();

	TArray<TSharedPtr<FJsonValue>> jvArray = tctJsonObject->GetArrayField(RtTctData::audio_com);
	if (jvArray.Num() > 0)
	{
		for (int32 i = 0; i < jvArray.Num(); i++)
		{
			TSharedPtr<FJsonValue> JsonValue = jvArray[i];
			TSharedPtr<FJsonObject> JsonValueObject = JsonValue->AsObject();

			auto element = component->AddElement();//;;AddElementBase<RtTctData::RtComAudio::RtElement>();
			element->mAudioFileName = JsonValueObject->GetStringField(RtTctData::aud_file_name);
			element->mAudioFilePath = JsonValueObject->GetStringField(RtTctData::aud_file_path);
			element->mPlayOption = (ERtComponentComboType)JsonValueObject->GetNumberField(RtTctData::aud_play_option);
			
		}
	}
}

void RtActorModel::LoadTCT_Com_Light(TSharedPtr<FJsonObject> tctJsonObject)
{
	if (!tctJsonObject->HasField(RtTctData::light_com))
	{
		return;
	}

	auto component = mTctData->AddComponent<RtTctData::RtComLight>();

	TArray<TSharedPtr<FJsonValue>> jvArray = tctJsonObject->GetArrayField(RtTctData::light_com);
	if (jvArray.Num() > 0)
	{
		for (int32 i = 0; i < jvArray.Num(); i++)
		{
			TSharedPtr<FJsonValue> JsonValue = jvArray[i];
			TSharedPtr<FJsonObject> JsonValueObject = JsonValue->AsObject();

			ERtTctComponentEleType ComEleType = (ERtTctComponentEleType)JsonValueObject->GetIntegerField(RtTctData::lit_ele_type);
			
			auto element = component->AddElement(ComEleType); // component->AddElementBase<RtTctData::RtComLight::RtElement>();

			element->mPosX = JsonValueObject->GetNumberField(RtTctData::lit_pos_x);
			element->mPosY = JsonValueObject->GetNumberField(RtTctData::lit_pos_y);
			element->mPosZ = JsonValueObject->GetNumberField(RtTctData::lit_pos_z);
			element->mIntensity = JsonValueObject->GetNumberField(RtTctData::lit_intensity);
			element->mAttenuationRadius = JsonValueObject->GetNumberField(RtTctData::lit_radius);

			if( ComEleType == ERtTctComponentEleType::ELightSpot )
			{
				auto * spot = static_cast<RtTctData::RtComLight::RtElementSpot*>(element);
				spot->mDirX = JsonValueObject->GetNumberField(RtTctData::lit_dir_x);
				spot->mDirY = JsonValueObject->GetNumberField(RtTctData::lit_dir_y);
				spot->mDirZ = JsonValueObject->GetNumberField(RtTctData::lit_dir_z);

				spot->mInnerConeAngle = JsonValueObject->GetNumberField(RtTctData::lit_inner_angle);
				spot->mOuterConeAngle = JsonValueObject->GetNumberField(RtTctData::lit_outer_angle);
			}
		}
	}
}

void RtActorModel::LoadTCT_Com_Equip(TSharedPtr<FJsonObject> tctJsonObject)
{
	if (!tctJsonObject->HasField(RtTctData::equip_com))
	{
		return;
	}

	TSharedPtr<FJsonObject> joEquipCom = tctJsonObject->GetObjectField(RtTctData::equip_com);
	if (joEquipCom.IsValid())
	{
		auto component = mTctData->AddComponent<RtTctData::RtComEquip>();

		component->mSocketType = (ERtEquipComSocketComboType)joEquipCom->GetNumberField(RtTctData::equip_sockettype); 
	}
}

void RtActorModel::LoadTCT_Com_Combustion(TSharedPtr<FJsonObject> tctJsonObject)
{
	if (!tctJsonObject->HasField(RtTctData::combustion_com))
	{
		return;
	}
	
	TSharedPtr<FJsonObject> joCombustionCom = tctJsonObject->GetObjectField(RtTctData::combustion_com);
	if (joCombustionCom.IsValid())
	{
		auto component = mTctData->AddComponent<RtTctData::RtComCombustion>();

		component->mIsBurning = (ERtComponentComboType)joCombustionCom->GetNumberField(RtTctData::com_is_burning); // 연소여부(가연/불연)
		component->mIgnitionPoint = joCombustionCom->GetNumberField(RtTctData::com_ignition_point);// 발화점(섭씨)
		component->mBurningHP = joCombustionCom->GetNumberField(RtTctData::com_burning_hp); // 연소시간(초)
		component->mBurningMaxDeg = joCombustionCom->GetNumberField(RtTctData::com_burning_max_deg); // 최고온도(섭씨)
		component->mBurningKind = (ERtComponentComboType)joCombustionCom->GetNumberField(RtTctData::com_burning_kind);// 연소종류 (유염/무염), 무염이면 화염크기, 연기색상, 연기양은 비활성화.
		component->mFlameMinSize = joCombustionCom->GetNumberField(RtTctData::com_flame_min_size); // 화염 최소 크기
		component->mFlameMaxSize = joCombustionCom->GetNumberField(RtTctData::com_flame_max_size); // 화염 최대 크기
		component->mHeatAbsorptionRate = joCombustionCom->GetNumberField(RtTctData::com_heat_absorption_rate); // 열 흡수율
		component->mHeatDissipationRate = joCombustionCom->GetNumberField(RtTctData::com_heat_dissipation_rate); // 열 방출율
		component->mSmokeColor = (ERtComponentComboType)joCombustionCom->GetNumberField(RtTctData::com_smoke_color);// 연기색상
		component->mSmokeSize = (ERtComponentComboType)joCombustionCom->GetNumberField(RtTctData::com_smoke_size);// 연기 양
		component->mBurningModelName = joCombustionCom->GetStringField(RtTctData::com_burning_model_name);// 연소 중 모델
		component->mBurningModelPath = joCombustionCom->GetStringField(RtTctData::com_burning_model_path);// 연소 중 모델 경로
		component->mEndBurningModelName = joCombustionCom->GetStringField(RtTctData::com_end_burning_model_name);// 연소 종료 모델
		component->mEndBurningModelPath = joCombustionCom->GetStringField(RtTctData::com_end_burning_model_path);// 연소 종료 모델 경로
	}
}

void RtActorModel::LoadTCT_Com_WaterSpray(TSharedPtr<FJsonObject> tctJsonObject)
{
	if (!tctJsonObject->HasField(RtTctData::water_spray_com))
	{
		return;
	}

	TSharedPtr<FJsonObject> joWaterSprayCom = tctJsonObject->GetObjectField(RtTctData::water_spray_com);
	if (joWaterSprayCom.IsValid())
	{
		auto component = mTctData->AddComponent<RtTctData::RtComWaterSpray>();

		component->mFirePosX = joWaterSprayCom->GetNumberField(RtTctData::wsp_pos_x);
		component->mFirePosY = joWaterSprayCom->GetNumberField(RtTctData::wsp_pos_y);
		component->mFirePosZ = joWaterSprayCom->GetNumberField(RtTctData::wsp_pos_z);
	}
}

void RtActorModel::LoadTCT_Com_GunFire(TSharedPtr<FJsonObject> tctJsonObject)
{
	if (!tctJsonObject->HasField(RtTctData::gun_fire_com))
	{
		return;
	}

	TSharedPtr<FJsonObject> joGunFireCom = tctJsonObject->GetObjectField(RtTctData::gun_fire_com);
	if (joGunFireCom.IsValid())
	{
		auto component = mTctData->AddComponent<RtTctData::RtComGunFire>();

		component->mFireSpeed = joGunFireCom->GetNumberField(RtTctData::gf_speed);
		component->mFirePosX = joGunFireCom->GetNumberField(RtTctData::gf_pos_x);
		component->mFirePosY = joGunFireCom->GetNumberField(RtTctData::gf_pos_y);
		component->mFirePosZ = joGunFireCom->GetNumberField(RtTctData::gf_pos_z);
	}
}

void RtActorModel::LoadTCT_Com_Effect(TSharedPtr<FJsonObject> tctJsonObject)
{
	if (!tctJsonObject->HasField(RtTctData::effect_com))
	{
		return;
	}

	auto component = mTctData->AddComponent<RtTctData::RtComEffect>(); 

	TArray<TSharedPtr<FJsonValue>> jvArray = tctJsonObject->GetArrayField(RtTctData::effect_com);
	if (jvArray.Num() > 0)
	{
		bool isSuccess = false;
		FString outMessage;

		for (int32 i = 0; i < jvArray.Num(); i++)
		{
			TSharedPtr<FJsonValue> JsonValue = jvArray[i];
			TSharedPtr<FJsonObject> JsonValueObject = JsonValue->AsObject();

			auto element = component->AddElement();
			element->mComEleType = (ERtTctComponentEleType)JsonValueObject->GetIntegerField(RtTctData::eft_ele_type);

			element->mEffectFileName = JsonValueObject->GetStringField(RtTctData::eft_file_name);
			element->mEffectFilePath = JsonValueObject->GetStringField(RtTctData::eft_file_path);

			element->mPosX = JsonValueObject->GetNumberField(RtTctData::eft_pos_x);
			element->mPosY = JsonValueObject->GetNumberField(RtTctData::eft_pos_y);
			element->mPosZ = JsonValueObject->GetNumberField(RtTctData::eft_pos_z);

			element->mRotX = JsonValueObject->GetNumberField(RtTctData::eft_rot_x);
			element->mRotY = JsonValueObject->GetNumberField(RtTctData::eft_rot_y);
			element->mRotZ = JsonValueObject->GetNumberField(RtTctData::eft_rot_z);

			element->mScaleX = JsonValueObject->GetNumberField(RtTctData::eft_scale_x);
			element->mScaleY = JsonValueObject->GetNumberField(RtTctData::eft_scale_y);
			element->mScaleZ = JsonValueObject->GetNumberField(RtTctData::eft_scale_z);

			element->mSpeed = JsonValueObject->GetNumberField(RtTctData::eft_speed);
		}
	}
}

void RtActorModel::LoadTCT_Com_Function(TSharedPtr<FJsonObject> tctJsonObject)
{
	if (!tctJsonObject->HasField(RtTctData::function_com))
	{
		return;
	}

	auto component = mTctData->AddComponent<RtTctData::RtComFunction>();

	TSharedPtr<FJsonObject> joFunctionCom = tctJsonObject->GetObjectField(RtTctData::function_com);

	component->mAniFileName = joFunctionCom->GetStringField(RtTctData::fun_ani_filename);
	component->mAniFilePath = joFunctionCom->GetStringField(RtTctData::fun_ani_filepath);
	component->mEffectFileName = joFunctionCom->GetStringField(RtTctData::fun_eft_filename);
	component->mEffectFilePath = joFunctionCom->GetStringField(RtTctData::fun_eft_filepath);
	component->mEffectPosX = joFunctionCom->GetNumberField(RtTctData::fun_eft_pos_x);
	component->mEffectPosY = joFunctionCom->GetNumberField(RtTctData::fun_eft_pos_y);
	component->mEffectPosZ = joFunctionCom->GetNumberField(RtTctData::fun_eft_pos_z);
	component->mSoundFileName = joFunctionCom->GetStringField(RtTctData::fun_snd_filename);
	component->mSoundFilePath = joFunctionCom->GetStringField(RtTctData::fun_snd_filepath);

	TArray<TSharedPtr<FJsonValue>> jvArray = joFunctionCom->GetArrayField(RtTctData::fun_element);
	if (jvArray.Num() > 0)
	{
		bool isSuccess = false;
		FString outMessage;

		for (int32 i = 0; i < jvArray.Num(); i++)
		{
			TSharedPtr<FJsonValue> JsonValue = jvArray[i];
			TSharedPtr<FJsonObject> JsonValueObject = JsonValue->AsObject();

			auto element = component->AddElement();
			element->mComEleType = (ERtTctComponentEleType)JsonValueObject->GetIntegerField(RtTctData::fun_ele_type);

			element->mModelFileName = JsonValueObject->GetStringField(RtTctData::fun_model_filename);
			element->mModelFilePath = JsonValueObject->GetStringField(RtTctData::fun_model_filepath);
			element->mAniFileName = JsonValueObject->GetStringField(RtTctData::fun_ani_filename);
			element->mAniFilePath = JsonValueObject->GetStringField(RtTctData::fun_ani_filepath);
			element->mEffectFileName = JsonValueObject->GetStringField(RtTctData::fun_eft_filename);
			element->mEffectFilePath = JsonValueObject->GetStringField(RtTctData::fun_eft_filepath);
			element->mEffectPosX = JsonValueObject->GetNumberField(RtTctData::fun_eft_pos_x);
			element->mEffectPosY = JsonValueObject->GetNumberField(RtTctData::fun_eft_pos_y);
			element->mEffectPosZ = JsonValueObject->GetNumberField(RtTctData::fun_eft_pos_z);
			element->mSoundFileName = JsonValueObject->GetStringField(RtTctData::fun_snd_filename);
			element->mSoundFilePath = JsonValueObject->GetStringField(RtTctData::fun_snd_filepath);
		}
	}
}

bool RtActorModel::LoadDefaultFbx()
{
	if( URtFbxLoaderSubsystem::Get() )
	{
		mDefaultFbxProMeshInfo = URtFbxLoaderSubsystem::Get()->LoadFbx(mTctData->mModelFilePath);
	}
	else
	{
		mDefaultFbxProMeshInfo = nullptr;
	}

	return (NULL != mDefaultFbxProMeshInfo);
}

TSharedPtr<FRtTctStruct> RtActorModel::GetTctStruct(bool noMake)
{
	if (!noMake) {
		if (!TctStruct.IsValid())
		{
			TctStruct = MakeShared<FRtTctStruct>();
		}
	}

	return TctStruct;
}

//bool RtActorModel::LoadBurningFbx()
//{
//	RtTctData::RtComCombustion * pCombu = static_cast<RtTctData::RtComCombustion*>(mTctData->GetComponent(ERtComponentType::ECombustion));
//	IS_NOT_NULL(pCombu)
//	{
//		mBurningFbxProMeshInfo = URtFbxLoaderSubsystem::Get()->LoadFbx(pCombu->mBurningModelPath);
//	}
//
//	return (NULL != mBurningFbxProMeshInfo);
//}
//
//bool RtActorModel::LoadEndBurningFbx()
//{
//	RtTctData::RtComCombustion* pCombu = static_cast<RtTctData::RtComCombustion*>(mTctData->GetComponent(ERtComponentType::ECombustion));
//	IS_NOT_NULL(pCombu)
//	{
//		mEndBurningFbxProMeshInfo = URtFbxLoaderSubsystem::Get()->LoadFbx(pCombu->mEndBurningModelPath);
//	}
//
//	return (NULL != mEndBurningFbxProMeshInfo);
//}

