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

// Tct 데이터 자료구조.
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

		// 충돌 컴포넌트 ---------------------------------------------------------------- //

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

				float mPosX; // 중심 좌표 X
				float mPosY; // 중심 좌표 Y
				float mPosZ; // 중심 좌표 Z
				float mSizeX; // 크기 X
				float mSizeY; // 크기 Y
				float mSizeZ; // 크기 Z

				FString mName; // 컬리전 이름( 빈값이 허용되지만, 값이 있다면 같은 값끼리의 상호 중복은 불가함 )
			};

			virtual RtElement* AddElement() override {
				return AddElementBase<RtTctData::RtComCollision::RtElement>().Get();
			}
		};

		// ----------------------------------------------------------------------------- //

		// 머티리얼 컴포넌트 ------------------------------------------------------------- //
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

			FString mMaterialFileName; // 머티리얼 파일 이름.
			FString mMaterialFilePath; // 머티리얼 파일 경로.
		};

		// ----------------------------------------------------------------------------- //

		// 애니메이션 뷰어 컴포넌트 ------------------------------------------------------------- //

		class RtComAniViewer : public RtComponentBase {
		public:
			RtComAniViewer() { mComType = ERtComponentType::EAnimationViewer; }
		};

		// ----------------------------------------------------------------------------- //
		
		// 오디오 컴포넌트 ------------------------------------------------------------- //
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
					// 현재 오디오는 요소 타입의 종류가 없다.aud_play_option
					mAudioFileName = "";
					mAudioFilePath = "";
					mPlayOption = ERtComponentComboType::EPlayOptionOneTime;
				}

				FString mAudioFileName;	// 오디오 이름.
				FString mAudioFilePath; // 오디오 파일 경로.
				ERtComponentComboType mPlayOption; // 재생 옵션.
			};

			virtual RtElement* AddElement() override {
				return AddElementBase<RtTctData::RtComAudio::RtElement>().Get();
			}
		};

		// ----------------------------------------------------------------------------- //

		// 라이트 컴포넌트 ------------------------------------------------------------- //

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

			// 포인트 라이트 정보.
			class RtElementPoint : public RtElementBase {
			public:
				RtElementPoint() {
					mComEleType = ERtTctComponentEleType::ELightPoint;
					mPosX = mPosY = mPosZ = 0;
					mIntensity = 8.0f;
					mAttenuationRadius = 1000.0f;
					mpPointLight = nullptr;
				}

				// 포인트, 스팟 공용 -------------------------- //
				float mPosX; // 중심 좌표 X
				float mPosY; // 중심 좌표 Y
				float mPosZ; // 중심 좌표 Z
				float mIntensity; // 강도.
				float mAttenuationRadius; // 감쇠 반지름

				UPointLightComponent* mpPointLight; 
			};

			// 스팟 라이트 정보.
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

				// 스팟 전용 --------------------------------- //
				float mDirX; // 방향 X
				float mDirY; // 방향 Y
				float mDirZ; // 방향 Z
				float mInnerConeAngle; // 내부 원뿔 각도
				float mOuterConeAngle; // 외부 원뿔 각도

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

		// 이펙트 컴포넌트 ------------------------------------------------------------- //

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

				FString mEffectFileName; // 이펙트 파일이름.
				FString mEffectFilePath; // 이펙트 파일경로.

				float mPosX; // 중심 좌표 X
				float mPosY; // 중심 좌표 Y
				float mPosZ; // 중심 좌표 Z

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

		// 기능 컴포넌트 ------------------------------------------------------------- //

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
				FString mModelFileName; // 작동 모델 이름(fbx)
				FString mModelFilePath; // 작동 모델 경로(fbx)

				FString mAniFileName; // 작동 모델 애니메이션 이름(fbx)
				FString mAniFilePath; // 작동 모델 애니메이션 경로(fbx)

				FString mEffectFileName; // 작동 모델 이펙트 이름(eft)
				FString mEffectFilePath; // 작동 모델 이펙트 경로(eft)

				float mEffectPosX; // 작동 모델 이펙트 좌표 X
				float mEffectPosY; // 작동 모델 이펙트 좌표 Y
				float mEffectPosZ; // 작동 모델 이펙트 좌표 Z

				FString mSoundFileName; // 작동 모델 사운드 이름(eft)
				FString mSoundFilePath; // 작동 모델 사운드 경로(eft)
			};

			FString mAniFileName; // 기본 상태 애니메이션 이름(fbx)
			FString mAniFilePath; // 기본 상태 애니메이션 경로(fbx)

			FString mEffectFileName; // 기본 상태 이펙트 이름(eft)
			FString mEffectFilePath; // 기본 상태 이펙트 경로(eft)

			float mEffectPosX; // 기본 상태 이펙트 좌표 X
			float mEffectPosY; // 기본 상태 이펙트 좌표 Y
			float mEffectPosZ; // 기본 상태 이펙트 좌표 Z

			FString mSoundFileName; // 기본 상태 사운드 이름(eft)
			FString mSoundFilePath; // 기본 상태 사운드 경로(eft)

			virtual RtElement* AddElement() override {
				return AddElementBase<RtTctData::RtComFunction::RtElement>().Get();
			}
		};

		// ---------------------------------------------------------------------------- //

		// 총 컴포넌트 ------------------------------------------------------------- //
		static const char* gun_fire_com;
		static const char* gf_speed;
		static const char* gf_pos_x;
		static const char* gf_pos_y;
		static const char* gf_pos_z;

		class RtComGunFire : public RtComponentBase {
		public:
			RtComGunFire() { 
				mComType = ERtComponentType::EGunFire; 
				mFireSpeed = 60; // 분당 60발 (Rounds per minute)
				mFirePosX = mFirePosY = mFirePosZ = 0;
			}

			int32 mFireSpeed; // 발사 속도.(연사력)(Rounds per minute)
			float mFirePosX; // 발사 위치 X
			float mFirePosY; // 발사 위치 Y
			float mFirePosZ; // 발사 위치 Z
		};

		// ---------------------------------------------------------------------------- //

		// 물 분사 컴포넌트 ------------------------------------------------------------- //
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

			float mFirePosX; // 발사 위치 X
			float mFirePosY; // 발사 위치 Y
			float mFirePosZ; // 발사 위치 Z
		};

		// ---------------------------------------------------------------------------- //
		
		// 연소 컴포넌트 ------------------------------------------------------------- //
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

			ERtComponentComboType mIsBurning; // 연소여부(연소 중/연소 전)
			float mIgnitionPoint;// 발화점(섭씨)
			int32 mBurningHP;// 연소체력
			float mBurningMaxDeg;// 최고온도(섭씨)
			ERtComponentComboType mBurningKind;// 연소종류 (유염/무염), 무염이면 화염크기, 연기색상, 연기양은 비활성화.
			float mFlameMinSize;// 화염 최소 크기
			float mFlameMaxSize;// 화염 최대 크기
			float mHeatAbsorptionRate; // 열 흡수율 (0~1)
			float mHeatDissipationRate; // 열 방출율 (0~1)
			ERtComponentComboType mSmokeColor;// 연기색상
			ERtComponentComboType mSmokeSize;// 연기 양
			FString mBurningModelName;// 연소 중 모델
			FString mBurningModelPath;// 연소 중 모델 경로
			FString mEndBurningModelName;// 연소 종료 모델
			FString mEndBurningModelPath;// 연소 종료 모델 경로
		};

		// ---------------------------------------------------------------------------- //

		// 장착 컴포넌트 ------------------------------------------------------------- //
		static const char* equip_com;
		static const char* equip_sockettype;

		class RtComEquip : public RtComponentBase {
		public:
			RtComEquip() {
				mComType = ERtComponentType::EEquip;
				mSocketType = ERtEquipComSocketComboType::ENone;
			}

			ERtEquipComSocketComboType mSocketType; // 장착할 소켓 타입.
		};

		// ---------------------------------------------------------------------------- //

		// 상호작용 컴포넌트 ------------------------------------------------------------- //
		class RtComInteract : public RtComponentBase {
		public:
			RtComInteract() { mComType = ERtComponentType::EInteract; }
		};

		// ---------------------------------------------------------------------------- //

		// 실내모델 컴포넌트 ------------------------------------------------------------- //
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
		FString mModelName; // Tct 모델 이름
		FString mTagName; // Tct 태그 이름
		ERtTctType mTctType; // Tct 타입
		FString mModelFileName; // Tct 모델 파일 이름. (Fbx) 
		FString mModelFilePath; // Tct 모델 파일 경로. (Fbx 파일 경로) : 툴 개발 전용.
		int32 mTileX; // 이 모델이 차지할 타일 X 영역 크기.
		int32 mTileY; // 이 모델이 차지할 타일 Y 영역 크기.
		float mRotX; // 회전 값 X (각도)
		float mRotY; // 회전 값 Y (각도)
		float mRotZ; // 회전 값 Z (각도)

		int32 mDurability; // 내구도.

		float mPosX; // 위치 값 X
		float mPosY; // 위치 값 Y
		float mPosZ; // 위치 값 Z

		float mScaleX;//스케일값 추가
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

	TSharedPtr<RtTctData> GetTctData() { return mTctData; } // 레거시 기본 tct 구조
	TSharedPtr<FRtTctData> GetNewTctData() { return NewTctData; } // 레거시 기본 tct 의 언리얼 구조체 버전.

	TSharedPtr<FRtTctStruct> GetTctStruct(bool noMake = true); // 리뉴얼 tct 구조체 가져오기.

	TSharedPtr<RtFbxProcedualMeshInfo> GetDefaultFbxProMeshInfo() { return mDefaultFbxProMeshInfo; }
	//TSharedPtr<RtFbxProcedualMeshInfo> GetBurningFbxProMeshInfo() { return mBurningFbxProMeshInfo; }
	//TSharedPtr<RtFbxProcedualMeshInfo> GetEndBurningFbxProMeshInfo() { return mEndBurningFbxProMeshInfo; }

	FString GetRealModelNameKey() const { return RealModelNameKey; }

private:

	/// 다음 데이터들은 향후 안쓰게 됨 /////////////////////////////////////////////////
	TSharedPtr<RtTctData> mTctData;	// Tct 원본 데이터 버퍼.	
	TSharedPtr<FRtTctData> NewTctData;	// mTctData -> NewTctData
	////////////////////////////////////////////////////////////////////////////////////

	TSharedPtr<FRtTctStruct> TctStruct; // 리뉴얼된 Tct 구조 데이터. 
		 
	// 진짜 FBX 파일을 불러올 경우 사용.
	TSharedPtr<RtFbxProcedualMeshInfo> mDefaultFbxProMeshInfo; // 기본 모델 정보.
	//TSharedPtr<RtFbxProcedualMeshInfo> mBurningFbxProMeshInfo; // 타는 모델 정보.
	//TSharedPtr<RtFbxProcedualMeshInfo> mEndBurningFbxProMeshInfo; // 다 탄 모델 정보.

	static TMap<ERtTctType, ERtComponentTypeInfo> mComInfoMap;

	FString RealModelNameKey; // 실제 모델 이름 키 값.
};
