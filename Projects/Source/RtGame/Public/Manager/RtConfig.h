// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

// Data
#include "RtCore/Public/Common/RtCommon.h"
#include "Common/RtRpcCommon.h"

#include "RtConfig.generated.h"

RTGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogRtConfig, Log, All);

class URtWebManager;

/**
 * 
 */
UCLASS()
class RTGAME_API URtConfig : public UGameInstanceSubsystem
{
	GENERATED_BODY()

// Various
public:

	static bool bMoveUsingHmdController;// true : Move using hmd controller



	static FString Solo_MapName;
	static FString Menu_MapName;
	static FString Lobby_MapName;     // Server
	static FString Game_MapName;
	static FString Replay_MapName;
    static FString Device_Name;
	static int32 Device_Index;

   static FString Custom_MapName;  // Map Name from JSON File


   // Url
   static FString ListenUrl;
   static FString WebServerUrl;
   static FString WebScenarioUrl;
   static FString WebReplayUploadUrl;
   static FString WebReplayDownloadUrl;
   static FString WebLogUrl;

   // Socket
   static FString TCPSocketIP;
   static FString UDPSocketIP;
   static int32 TCPPort;
   static int32 UDPPort;
   static int32 UDPRecvPort;
   static bool UsingVoice;
   static bool VoiceTalk; 

   // for 10->20
   static int32 PlayerNum;

   // stringtable
   static FString MessageBoxPath;
   static FString ControlMainPath;
   static FString ErrorMsgPath;
   static FString ResultDetailPath;

   // nation
   static int32 NationCode;
   // Conne
   static int32 NumPublicConnections;
protected:
   static FString RtSettingsFile;


// Functions
public:
    // Server
	static const FName& Get_ServerLevel();

	// PlayerSetting
	static FRtPlaySettings Play_Settings;
	
	static struct FRtTrainingOptions
	{
	public:
		FRtTrainingOptions() {};

		// Tracking
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsVR = true; // VR (1), Screen & Keyboard & Mouse(0)
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 WeaponTrackerMode = 0; // Tracker(0), LeftController(1), RightController(2)
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsUsingMotion = true; // UsingMotionCature(1), NotMotionCapture(0)
		
		// VR Trainee Safety
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 MapSizeX = 30;
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 MapSizeY = 30;
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float AlertDistance_Wall = 2.0f;
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float AlertDistance_HMD = 1.0f;

		// Shot
		bool IsGuideAim = false; // Aim Guide Line
		float IntimidatingFire_Distance = 1000.0f; // 위협사격 범위 - 거리(cm)
		float IntimidatingFire_Angle = 45.0f; // 위협사격 범위 - 반경 각도(도)

		// Sound
		float AlertVolume = 1.0f;
		float SCT_Volume = 1.0f;

		float TCT_Volume = 1.0f;
		float TCT_AttenuationStart = 0.0f; // 감쇠 시작 거리
		float TCT_AttenuationEnd = 1000.0f; // 감쇠 최대 거리(볼륨이 0이 되는 거리)

	}	TrainingOptions;

	// XMl Load
	static FString Get_IpSettingsFileUrl();
	static FString Get_TrainingOptionsFileUrl();
    // 제거예정
	//static FString Get_PlaySettingsFileUrl();
	//static FString Get_DevicesFileUrl();

	//static bool Load_PlaySettings();
	//static bool Load_PlaySettings_ini();

	static bool Load_IpSettings();
	static bool Load_TrainingOptions();

// Get 
	UFUNCTION(BlueprintCallable)
	FString GetDeviceName() { 
#if WITH_EDITOR
		FWorldContext* context = GEngine->GetWorldContextFromWorld(this->GetWorld());
		if (context)
		{
			TArray<FString> strings;
			Device_Name.ParseIntoArray(strings, TEXT("-"));
			int num = FCString::Atoi(*strings[1]);
			return FString::Printf(TEXT("VR-%02d"), num + context->PIEInstance);
		}
#endif
		return Device_Name; 
	}

	UFUNCTION(BlueprintCallable)
	int32 GetDeviceIndex() { return Device_Index; }

	UFUNCTION(BlueprintCallable)
	int32 GetNationCode() { return NationCode; }
	
	UFUNCTION(BlueprintPure)
	FVector GetAlertDistance_Wall() { return FVector(TrainingOptions.MapSizeX, TrainingOptions.MapSizeY, TrainingOptions.AlertDistance_Wall); } // MapSizeX, MapSizeY, AlertDistance_Wall
	
	UFUNCTION(BlueprintPure)
	float GetAlertDistance_HMD() { return TrainingOptions.AlertDistance_HMD; } // MapSizeX, MapSizeY, AlertDistance_Wall

protected:
	URtConfig();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	static void NationCode_Parse(const int32 nationcode);
	void Setting_From_InitFile(const FString& InIni, bool InUseDefaultVaule);
	void Setting_From_InitFile();
	static void Setting_From_xmlFile(const FString& TempTag, const FString& TempString);
	
	void UpdateTraineeStringTable();

};
