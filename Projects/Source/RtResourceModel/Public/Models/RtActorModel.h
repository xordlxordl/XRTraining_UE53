// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RtFbxProcedualMeshInfo.h"

#include "RtEnums.h"
#include "RtDefines.h"
#include "RtJsonFunctionLib.h"
#include "RtActorModel2.h"

#include "RtTemplateModel.h"

class UPointLightComponent;
class USpotLightComponent;

// Tct ������ �ڷᱸ��.
class RTRESOURCEMODEL_API RtTctData
{
	public:
		class RtElementBase {
		public:
			ERtTctComponentEleType mComEleType = ERtTctComponentEleType::ENone;
			void * mpUserData = NULL;
		};

		class RtComponentBase {
		public:
			ERtComponentType mComType = ERtComponentType::ENone;

			TArray<TSharedPtr<RtElementBase>> mElementList;

			template<typename T>
			TSharedPtr<T> AddElementBase() {
				TSharedPtr<T> element = MakeShared<T>();
				mElementList.Add(element);
				return element;
			}

			template<typename T>
			TSharedPtr<T> InsertElementBase(int32 index) {
				TSharedPtr<T> element = MakeShared<T>();
				mElementList.Insert(element, index);
				return element;
			}

			virtual RtElementBase* AddElement() { return nullptr; }
			virtual RtElementBase* AddElement(ERtTctComponentEleType eleType) { 
				if (ERtTctComponentEleType::ENone == eleType) {
					return AddElement();
				}
				return nullptr; 
			}

			//virtual RtElementBase* InsertElement(int index) { return nullptr; }
			//virtual RtElementBase* Insertlement(ERtTctComponentEleType eleType, int index) {
			//	if (ERtTctComponentEleType::ENone == eleType) {
			//		return InsertElement(index);
			//	}
			//	return nullptr;
			//}

			virtual RtElementBase* ChangeElement(int32 index, ERtTctComponentEleType eleType) { return nullptr; }

			bool RemoveElement(int32 index) { 
				if( index < mElementList.Num() )
				{
					mElementList.RemoveAt(index);
					return true;
				}

				return false; 
			}

			int32 GetElementCount() { return mElementList.Num(); }

			virtual ~RtComponentBase() {}
		};

		// �浹 ������Ʈ ---------------------------------------------------------------- //

		static const char* collision_com;
		static const char* col_ele_type;
		static const char* col_x;
		static const char* col_y;
		static const char* col_z;
		static const char* col_sx;
		static const char* col_sy;
		static const char* col_sz;
		static const char* col_name;

		class RtComCollision : public RtComponentBase {
		public:
			RtComCollision() { mComType = ERtComponentType::ECollision; }

			class RtElement : public RtElementBase {
			public:
				RtElement() {
					mComEleType = ERtTctComponentEleType::ECollisionCube;
					mPosX = mPosY = mPosZ = 0;
					mSizeX = mSizeY = mSizeZ = 0;
					mName = TEXT("");
				}

				float mPosX; // �߽� ��ǥ X
				float mPosY; // �߽� ��ǥ Y
				float mPosZ; // �߽� ��ǥ Z
				float mSizeX; // ũ�� X
				float mSizeY; // ũ�� Y
				float mSizeZ; // ũ�� Z

				FString mName; // �ø��� �̸�( ���� ��������, ���� �ִٸ� ���� �������� ��ȣ �ߺ��� �Ұ��� )
			};

			virtual RtElement* AddElement() override {
				return AddElementBase<RtTctData::RtComCollision::RtElement>().Get();
			}
		};

		// ----------------------------------------------------------------------------- //

		// ��Ƽ���� ������Ʈ ------------------------------------------------------------- //
		static const char* material_com;
		static const char* mat_tex_file_name;
		static const char* mat_tex_file_path;

		class RtComMaterial : public RtComponentBase {
		public:
			RtComMaterial() { 
				mComType = ERtComponentType::EMaterial; 
				mMaterialFileName = "";
				mMaterialFilePath = "";
			}

			FString mMaterialFileName; // ��Ƽ���� ���� �̸�.
			FString mMaterialFilePath; // ��Ƽ���� ���� ���.
		};

		// ----------------------------------------------------------------------------- //

		// �ִϸ��̼� ��� ������Ʈ ------------------------------------------------------------- //

		class RtComAniViewer : public RtComponentBase {
		public:
			RtComAniViewer() { mComType = ERtComponentType::EAnimationViewer; }
		};

		// ----------------------------------------------------------------------------- //
		
		// ����� ������Ʈ ------------------------------------------------------------- //
		static const char* audio_com;
		static const char* aud_file_name;
		static const char* aud_file_path;
		static const char* aud_play_option;

		class RtComAudio : public RtComponentBase {
		public:
			RtComAudio() { mComType = ERtComponentType::EAudio; }

			class RtElement : public RtElementBase {
			public:
				RtElement() {
					// ���� ������� ��� Ÿ���� ������ ����.aud_play_option
					mAudioFileName = "";
					mAudioFilePath = "";
					mPlayOption = ERtComponentComboType::EPlayOptionOneTime;
				}

				FString mAudioFileName;	// ����� �̸�.
				FString mAudioFilePath; // ����� ���� ���.
				ERtComponentComboType mPlayOption; // ��� �ɼ�.
			};

			virtual RtElement* AddElement() override {
				return AddElementBase<RtTctData::RtComAudio::RtElement>().Get();
			}
		};

		// ----------------------------------------------------------------------------- //

		// ����Ʈ ������Ʈ ------------------------------------------------------------- //

		static const char* light_com;
		static const char* lit_ele_type;
		static const char* lit_pos_x;
		static const char* lit_pos_y;
		static const char* lit_pos_z;
		static const char* lit_intensity;
		static const char* lit_radius;
		static const char* lit_dir_x;
		static const char* lit_dir_y;
		static const char* lit_dir_z;
		static const char* lit_inner_angle;
		static const char* lit_outer_angle;

		class RtComLight : public RtComponentBase {
		public:
			RtComLight() { mComType = ERtComponentType::ELight; }

			// ����Ʈ ����Ʈ ����.
			class RtElementPoint : public RtElementBase {
			public:
				RtElementPoint() {
					mComEleType = ERtTctComponentEleType::ELightPoint;
					mPosX = mPosY = mPosZ = 0;
					mIntensity = 8.0f;
					mAttenuationRadius = 1000.0f;
					mpPointLight = nullptr;
				}

				// ����Ʈ, ���� ���� -------------------------- //
				float mPosX; // �߽� ��ǥ X
				float mPosY; // �߽� ��ǥ Y
				float mPosZ; // �߽� ��ǥ Z
				float mIntensity; // ����.
				float mAttenuationRadius; // ���� ������

				UPointLightComponent* mpPointLight; 
			};

			// ���� ����Ʈ ����.
			class RtElementSpot : public RtElementPoint {
			public:
				RtElementSpot() {
					mComEleType = ERtTctComponentEleType::ELightSpot;
					mPosX = mPosY = mPosZ = 0;
					mIntensity = 8.0f;
					mAttenuationRadius = 1000.0f;
					mpPointLight = nullptr;

					mDirX = 0;
					mDirY = 270.0f;
					mDirZ = 0;
					mInnerConeAngle = 0;
					mOuterConeAngle = 44.0f;
					mpSpotLight = nullptr;
				}

				// ���� ���� --------------------------------- //
				float mDirX; // ���� X
				float mDirY; // ���� Y
				float mDirZ; // ���� Z
				float mInnerConeAngle; // ���� ���� ����
				float mOuterConeAngle; // �ܺ� ���� ����

				USpotLightComponent* mpSpotLight;
			};

			virtual RtElementPoint* AddElement() override {
				return AddElementBase<RtTctData::RtComLight::RtElementPoint>().Get();
			}

			virtual RtElementPoint* AddElement(ERtTctComponentEleType eleType) override {

				if (eleType == ERtTctComponentEleType::ELightPoint)
				{
					return AddElementBase<RtTctData::RtComLight::RtElementPoint>().Get();
				}
				else if( eleType == ERtTctComponentEleType::ELightSpot )
				{
					return AddElementBase<RtTctData::RtComLight::RtElementSpot>().Get();
				}

				return nullptr;
			}
		};

		// ---------------------------------------------------------------------------- //

		// ����Ʈ ������Ʈ ------------------------------------------------------------- //

		static const char* effect_com;
		static const char* eft_ele_type;
		static const char* eft_pos_x;
		static const char* eft_pos_y;
		static const char* eft_pos_z;
		static const char* eft_rot_x;
		static const char* eft_rot_y;
		static const char* eft_rot_z;
		static const char* eft_scale_x;
		static const char* eft_scale_y;
		static const char* eft_scale_z;
		static const char* eft_speed;

		static const char* eft_file_name;
		static const char* eft_file_path;

		class RtComEffect : public RtComponentBase {
		public:
			RtComEffect() { mComType = ERtComponentType::EEffect; }

			class RtElement : public RtElementBase {
			public:
				RtElement() {
					mComEleType = ERtTctComponentEleType::EEffect;
					mEffectFileName = mEffectFilePath = "";
					mPosX = mPosY = mPosZ = 0;
				}

				FString mEffectFileName; // ����Ʈ �����̸�.
				FString mEffectFilePath; // ����Ʈ ���ϰ��.

				float mPosX; // �߽� ��ǥ X
				float mPosY; // �߽� ��ǥ Y
				float mPosZ; // �߽� ��ǥ Z

				float mRotX;
				float mRotY;
				float mRotZ;

				float mScaleX;
				float mScaleY;
				float mScaleZ;

				float mSpeed;
			};

			virtual RtElement* AddElement() override {
				return AddElementBase<RtTctData::RtComEffect::RtElement>().Get();
			}
		};

		// ---------------------------------------------------------------------------- //

		// ��� ������Ʈ ------------------------------------------------------------- //

		static const char* function_com;
		static const char* fun_ele_type;
		static const char* fun_element;		
		static const char* fun_ani_filename;
		static const char* fun_ani_filepath;
		static const char* fun_eft_filename;
		static const char* fun_eft_filepath;
		static const char* fun_eft_pos_x;
		static const char* fun_eft_pos_y;
		static const char* fun_eft_pos_z;
		static const char* fun_snd_filename;
		static const char* fun_snd_filepath;
		static const char* fun_model_filename;
		static const char* fun_model_filepath;

		class RtComFunction : public RtComponentBase {
		public:
			RtComFunction() {
				mComType = ERtComponentType::EFunction; 
				mAniFileName = mAniFilePath = "";
				mEffectFileName = mEffectFilePath = "";
				mEffectPosX = mEffectPosY = mEffectPosZ = 0;
				mSoundFileName = mSoundFilePath = "";
			}

			class RtElement : public RtElementBase {
			public:
				RtElement() {
					mComEleType = ERtTctComponentEleType::EFunction;
					mModelFileName = mModelFilePath = "";
					mAniFileName = mAniFilePath = "";
					mEffectFileName = mEffectFilePath = "";
					mEffectPosX = mEffectPosY = mEffectPosZ = 0;
					mSoundFileName = mSoundFilePath = "";
				}
				FString mModelFileName; // �۵� �� �̸�(fbx)
				FString mModelFilePath; // �۵� �� ���(fbx)

				FString mAniFileName; // �۵� �� �ִϸ��̼� �̸�(fbx)
				FString mAniFilePath; // �۵� �� �ִϸ��̼� ���(fbx)

				FString mEffectFileName; // �۵� �� ����Ʈ �̸�(eft)
				FString mEffectFilePath; // �۵� �� ����Ʈ ���(eft)

				float mEffectPosX; // �۵� �� ����Ʈ ��ǥ X
				float mEffectPosY; // �۵� �� ����Ʈ ��ǥ Y
				float mEffectPosZ; // �۵� �� ����Ʈ ��ǥ Z

				FString mSoundFileName; // �۵� �� ���� �̸�(eft)
				FString mSoundFilePath; // �۵� �� ���� ���(eft)
			};

			FString mAniFileName; // �⺻ ���� �ִϸ��̼� �̸�(fbx)
			FString mAniFilePath; // �⺻ ���� �ִϸ��̼� ���(fbx)

			FString mEffectFileName; // �⺻ ���� ����Ʈ �̸�(eft)
			FString mEffectFilePath; // �⺻ ���� ����Ʈ ���(eft)

			float mEffectPosX; // �⺻ ���� ����Ʈ ��ǥ X
			float mEffectPosY; // �⺻ ���� ����Ʈ ��ǥ Y
			float mEffectPosZ; // �⺻ ���� ����Ʈ ��ǥ Z

			FString mSoundFileName; // �⺻ ���� ���� �̸�(eft)
			FString mSoundFilePath; // �⺻ ���� ���� ���(eft)

			virtual RtElement* AddElement() override {
				return AddElementBase<RtTctData::RtComFunction::RtElement>().Get();
			}
		};

		// ---------------------------------------------------------------------------- //

		// �� ������Ʈ ------------------------------------------------------------- //
		static const char* gun_fire_com;
		static const char* gf_speed;
		static const char* gf_pos_x;
		static const char* gf_pos_y;
		static const char* gf_pos_z;

		class RtComGunFire : public RtComponentBase {
		public:
			RtComGunFire() { 
				mComType = ERtComponentType::EGunFire; 
				mFireSpeed = 60; // �д� 60�� (Rounds per minute)
				mFirePosX = mFirePosY = mFirePosZ = 0;
			}

			int32 mFireSpeed; // �߻� �ӵ�.(�����)(Rounds per minute)
			float mFirePosX; // �߻� ��ġ X
			float mFirePosY; // �߻� ��ġ Y
			float mFirePosZ; // �߻� ��ġ Z
		};

		// ---------------------------------------------------------------------------- //

		// �� �л� ������Ʈ ------------------------------------------------------------- //
		static const char* water_spray_com;
		static const char* wsp_pos_x;
		static const char* wsp_pos_y;
		static const char* wsp_pos_z;

		class RtComWaterSpray : public RtComponentBase {
		public:
			RtComWaterSpray() { 
				mComType = ERtComponentType::EWaterSpray; 
				mFirePosX = mFirePosY = mFirePosZ = 0;
			}

			float mFirePosX; // �߻� ��ġ X
			float mFirePosY; // �߻� ��ġ Y
			float mFirePosZ; // �߻� ��ġ Z
		};

		// ---------------------------------------------------------------------------- //
		
		// ���� ������Ʈ ------------------------------------------------------------- //
		static const char* combustion_com;
		static const char* com_is_burning;
		static const char* com_ignition_point;
		static const char* com_burning_hp;
		static const char* com_burning_max_deg;
		static const char* com_burning_kind;
		static const char* com_flame_min_size;
		static const char* com_flame_max_size;
		static const char* com_heat_absorption_rate;
		static const char* com_heat_dissipation_rate;
		static const char* com_smoke_color;
		static const char* com_smoke_size;
		static const char* com_burning_model_name;
		static const char* com_burning_model_path;
		static const char* com_end_burning_model_name;
		static const char* com_end_burning_model_path;

		class RtComCombustion : public RtComponentBase {
		public:
			RtComCombustion() {
				mComType = ERtComponentType::ECombustion; 

				mIsBurning = ERtComponentComboType::ENone;
				mIgnitionPoint = 100;
//				mBurningTime = 100;
				mBurningHP = 100;
				mBurningMaxDeg = 100;
				mBurningKind = ERtComponentComboType::ENone;
				mFlameMinSize = 50;
				mFlameMaxSize = 100;
				mHeatAbsorptionRate  = 0.5f;
				mHeatDissipationRate = 0.5f;
				mSmokeColor = ERtComponentComboType::ENone;
				mSmokeSize = ERtComponentComboType::ENone;
				mBurningModelName = "";
				mBurningModelPath = "";
				mEndBurningModelName = "";
				mEndBurningModelPath = "";
			}

			ERtComponentComboType mIsBurning; // ���ҿ���(���� ��/���� ��)
			float mIgnitionPoint;// ��ȭ��(����)
			int32 mBurningHP;// ����ü��
			float mBurningMaxDeg;// �ְ�µ�(����)
			ERtComponentComboType mBurningKind;// �������� (����/����), �����̸� ȭ��ũ��, �������, ������� ��Ȱ��ȭ.
			float mFlameMinSize;// ȭ�� �ּ� ũ��
			float mFlameMaxSize;// ȭ�� �ִ� ũ��
			float mHeatAbsorptionRate; // �� ����� (0~1)
			float mHeatDissipationRate; // �� ������ (0~1)
			ERtComponentComboType mSmokeColor;// �������
			ERtComponentComboType mSmokeSize;// ���� ��
			FString mBurningModelName;// ���� �� ��
			FString mBurningModelPath;// ���� �� �� ���
			FString mEndBurningModelName;// ���� ���� ��
			FString mEndBurningModelPath;// ���� ���� �� ���
		};

		// ---------------------------------------------------------------------------- //

		// ���� ������Ʈ ------------------------------------------------------------- //
		static const char* equip_com;
		static const char* equip_sockettype;

		class RtComEquip : public RtComponentBase {
		public:
			RtComEquip() {
				mComType = ERtComponentType::EEquip;
				mSocketType = ERtEquipComSocketComboType::ENone;
			}

			ERtEquipComSocketComboType mSocketType; // ������ ���� Ÿ��.
		};

		// ---------------------------------------------------------------------------- //

		// ��ȣ�ۿ� ������Ʈ ------------------------------------------------------------- //
		class RtComInteract : public RtComponentBase {
		public:
			RtComInteract() { mComType = ERtComponentType::EInteract; }
		};

		// ---------------------------------------------------------------------------- //

		// �ǳ��� ������Ʈ ------------------------------------------------------------- //
		class RtComInsideModel : public RtComponentBase {
		public:
			RtComInsideModel() { mComType = ERtComponentType::EInsideModel; }
		};

		// ---------------------------------------------------------------------------- //

		static const char* obj_model_name;
		static const char* obj_tag_name;
		static const char* obj_tct_type;
		static const char* obj_model_file_name;
		static const char* obj_model_file_path;
		static const char* obj_tile_x;
		static const char* obj_tile_y;
		static const char* obj_rot_x;
		static const char* obj_rot_y;
		static const char* obj_rot_z;
		static const char* obj_durability;
		static const char* obj_pos_x;
		static const char* obj_pos_y;
		static const char* obj_pos_z;
		static const char* obj_scale_x;
		static const char* obj_scale_y;
		static const char* obj_scale_z;

	public:
		FString mModelName; // Tct �� �̸�
		FString mTagName; // Tct �±� �̸�
		ERtTctType mTctType; // Tct Ÿ��
		FString mModelFileName; // Tct �� ���� �̸�. (Fbx) 
		FString mModelFilePath; // Tct �� ���� ���. (Fbx ���� ���) : �� ���� ����.
		int32 mTileX; // �� ���� ������ Ÿ�� X ���� ũ��.
		int32 mTileY; // �� ���� ������ Ÿ�� Y ���� ũ��.
		float mRotX; // ȸ�� �� X (����)
		float mRotY; // ȸ�� �� Y (����)
		float mRotZ; // ȸ�� �� Z (����)

		int32 mDurability; // ������.

		float mPosX; // ��ġ �� X
		float mPosY; // ��ġ �� Y
		float mPosZ; // ��ġ �� Z

		float mScaleX;//�����ϰ� �߰�
		float mScaleY;
		float mScaleZ;

		TArray<TSharedPtr<RtComponentBase>> mComponents;

		RtTctData() {
			mModelName = "";
			mTagName = "";
			mTctType = ERtTctType::ENone;
			mModelFileName = "";
			mModelFilePath = "";
			mTileX = 0;
			mTileY = 0;
			mRotX = mRotY = mRotZ = 0;
			mDurability = 0;
			mPosX = mPosY = mPosZ = 0;
		}
				
		template<typename T>
		TSharedPtr<T> AddComponent() {
			TSharedPtr<T> component = MakeShared<T>();
			mComponents.Add(component);

			return component;
		}

		RtComponentBase* GetComponent(ERtComponentType comType)
		{
			for (auto item : mComponents)
			{
				if (item->mComType == comType)
				{
					return item.Get();
				}
			}

			return nullptr;
		}
		
		TArray<RtComponentBase*> GetComponents(ERtComponentType comType)
		{
			TArray<RtComponentBase*> results;

			for (auto item : mComponents)
			{
				if (item->mComType == comType)
				{
					results.Add(item.Get());
				}
			}

			return results;
		}

		RtElementBase* AddElement(ERtComponentType comType)
		{
			for (auto item : mComponents)
			{
				if (item->mComType == comType)
				{
					return item->AddElement();
				}
			}

			return nullptr;
		}

		RtElementBase* AddElement(ERtComponentType comType, ERtTctComponentEleType eleType)
		{
			for (auto item : mComponents)
			{
				if (item->mComType == comType)
				{
					return item->AddElement(eleType);
				}
			}

			return nullptr;
		}

		//RtElementBase* InsertElement(ERtComponentType comType, ERtTctComponentEleType eleType, int index)
		//{
		//	for (auto item : mComponents)
		//	{
		//		if (item->mComType == comType)
		//		{
		//			return item->InsertElementBase(index);
		//		}
		//	}

		//	return nullptr;
		//}

		int32 GetElementCount(ERtComponentType comType)
		{
			for (auto item : mComponents)
			{
				if (item->mComType == comType)
				{
					return item->mElementList.Num();
				}
			}

			return -1;
		}


		RtElementBase* GetElement(ERtComponentType comType, int32 index)
		{
			if( index < 0 ) {
				return nullptr;
			}

			for (auto item : mComponents)
			{
				if (item->mComType == comType)
				{
					if (index < item->mElementList.Num())
					{
						return item->mElementList[index].Get();
					}
				}
			}

			return nullptr;
		}

		template <typename T>
		T* GetElementEx(ERtComponentType comType, int index)
		{
			RtElementBase* pComEle = GetElement(comType, index);
			IS_NOT_NULL(pComEle)
			{
				T* pComSomeEle = StaticCast<T*>(pComEle);
				return pComSomeEle;
			}
			return nullptr;
		}

		void RemoveElement(ERtComponentType comType, int32 index)
		{
			if (index < 0) {
				return;
			}

			for (auto item : mComponents)
			{
				if (item->mComType == comType)
				{
					if (index < item->mElementList.Num())
					{
						item->mElementList.RemoveAt(index);
					}
				}
			}
		}
};

typedef class RtTctData::RtComCombustion				RtComCombustion;
typedef class RtTctData::RtComMaterial					RtComMaterial;
typedef class RtTctData::RtComWaterSpray				RtComWaterSpray;
typedef class RtTctData::RtComGunFire					RtComGunFire;
typedef class RtTctData::RtComFunction					RtComFunction;
typedef class RtTctData::RtComEquip						RtComEquip;

typedef class RtTctData::RtComCollision::RtElement		RtComCollisionElement;
typedef class RtTctData::RtComEffect::RtElement			RtComEffectElement;
typedef class RtTctData::RtComAudio::RtElement			RtComAudioElement;
typedef class RtTctData::RtComFunction::RtElement		RtComFunElement;
typedef class RtTctData::RtComLight::RtElementPoint		RtComPointLightElement;
typedef class RtTctData::RtComLight::RtElementSpot		RtComSpotLightElement;

class RTRESOURCEMODEL_API RtActorModel
{
public:
	RtActorModel();
	~RtActorModel();

	void InitEmptyData(ERtTctType tctType);
	void InitEmptyData2(ERtTctType2 tctType);

	bool LoadTCTJsonObject(TSharedPtr<FJsonObject> tctJsonObject, bool bIsNewTctStruct = false);

	bool LoadTCT(FString tctFilePath, bool bIsNewTctStruct = false);
	bool LoadTCTJsonString(FString modelNameKey, FString tctJsonString, bool bIsNewTctStruct = false);

	void LoadTCT_Com_Collision(TSharedPtr<FJsonObject> tctJsonObject);
	void LoadTCT_Com_Material(TSharedPtr<FJsonObject> tctJsonObject);
	void LoadTCT_Com_Audio(TSharedPtr<FJsonObject> tctJsonObject);
	void LoadTCT_Com_Light(TSharedPtr<FJsonObject> tctJsonObject);
	void LoadTCT_Com_Equip(TSharedPtr<FJsonObject> tctJsonObject);
	void LoadTCT_Com_Combustion(TSharedPtr<FJsonObject> tctJsonObject);
	void LoadTCT_Com_WaterSpray(TSharedPtr<FJsonObject> tctJsonObject);
	void LoadTCT_Com_GunFire(TSharedPtr<FJsonObject> tctJsonObject);
	void LoadTCT_Com_Effect(TSharedPtr<FJsonObject> tctJsonObject);
	void LoadTCT_Com_Function(TSharedPtr<FJsonObject> tctJsonObject);

	bool LoadDefaultFbx();
	//bool LoadBurningFbx();
	//bool LoadEndBurningFbx();

	TSharedPtr<RtTctData> GetTctData() { return mTctData; } // ���Ž� �⺻ tct ����
	TSharedPtr<FRtTctData> GetNewTctData() { return NewTctData; } // ���Ž� �⺻ tct �� �𸮾� ����ü ����.

	TSharedPtr<FRtTctStruct> GetTctStruct(bool noMake = true); // ������ tct ����ü ��������.

	TSharedPtr<RtFbxProcedualMeshInfo> GetDefaultFbxProMeshInfo() { return mDefaultFbxProMeshInfo; }
	//TSharedPtr<RtFbxProcedualMeshInfo> GetBurningFbxProMeshInfo() { return mBurningFbxProMeshInfo; }
	//TSharedPtr<RtFbxProcedualMeshInfo> GetEndBurningFbxProMeshInfo() { return mEndBurningFbxProMeshInfo; }

	FString GetRealModelNameKey() const { return RealModelNameKey; }

private:

	/// ���� �����͵��� ���� �Ⱦ��� �� /////////////////////////////////////////////////
	TSharedPtr<RtTctData> mTctData;	// Tct ���� ������ ����.	
	TSharedPtr<FRtTctData> NewTctData;	// mTctData -> NewTctData
	////////////////////////////////////////////////////////////////////////////////////

	TSharedPtr<FRtTctStruct> TctStruct; // ������� Tct ���� ������. 
		 
	// ��¥ FBX ������ �ҷ��� ��� ���.
	TSharedPtr<RtFbxProcedualMeshInfo> mDefaultFbxProMeshInfo; // �⺻ �� ����.
	//TSharedPtr<RtFbxProcedualMeshInfo> mBurningFbxProMeshInfo; // Ÿ�� �� ����.
	//TSharedPtr<RtFbxProcedualMeshInfo> mEndBurningFbxProMeshInfo; // �� ź �� ����.

	static TMap<ERtTctType, ERtComponentTypeInfo> mComInfoMap;

	FString RealModelNameKey; // ���� �� �̸� Ű ��.
};
