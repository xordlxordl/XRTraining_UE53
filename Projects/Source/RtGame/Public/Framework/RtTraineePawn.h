// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "InputActionValue.h"

#include "RtDefines.h"
// Framework
#include "Framework/RtPlayerState.h"
// Data
#include "Common/RtRpcCommon.h"
//Hand Tracking
//#include "SteamVRInputDeviceFunctionLibrary.h"
//Voice
#include "VoiceModule.h"
#include "Voice.h"

// Passthrough
#include "ViveOpenXRPassthroughFunctionLibrary.h"

#include "RtTraineePawn.generated.h"

class UWidgetComponent;
class ARtPlayerController;
class URtNetworkManager;
class ARtGameScenarioController;
class UMotionControllerComponent;
class UCaptureDevice;
class UTrackingDataComponents;
class ARtClone;
class ARtVRMeshes;
class ARtObjectActor;
class ARtWidgetActor_TraineeResult;
class URtDirectionArrowComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_CompleteCalibaration);

/*
교육생 Pawn
VR, 트래커, 인터랙션 처리
*/
UCLASS()
class RTGAME_API ARtTraineePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARtTraineePawn();

#pragma region Overrides
protected:
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Set_Visiblity_MotionController(bool isVisible);
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnRep_PlayerState() override;
#pragma endregion Overrides

#pragma region Delgates
public:
	// Delegate
	void Handle_Changed_DeviceInfo();

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChanged_DeviceInfo, const FRtDeviceInfo&, ARtPlayerState*);
	FOnChanged_DeviceInfo OnChanged_DeviceInfo_delegate;
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChanged_TraineeInfo, const FRtTraineeInfo&, ARtPlayerState*);
	FOnChanged_TraineeInfo OnChanged_TraineeInfo_delegate;

	UFUNCTION()
	void OnPlayerBasicInfoChangedDelegate(ARtPlayerState* InPlayerState);

#pragma endregion Delgates

#pragma region Inputs
protected:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* PcMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SctInteractAction;

	/** MenuSelect Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SctSelectMenuAction;

	/** Shot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SctShotAction;

	/** Change Selection Menu */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ChangeSelectionMenuAction;

	/** Reload Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* GripRAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* GripLAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BtnX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BtnY;

#pragma endregion Inputs

#pragma region Components
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> DefaultRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UMotionControllerComponent> LeftController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UMotionControllerComponent> RightController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class USphereComponent> LeftHandCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class USphereComponent> RightHandCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UMotionControllerComponent> TrackerPelvis;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UMotionControllerComponent> TrackerLeftFoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UMotionControllerComponent> TrackerRightFoot;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//TObjectPtr<UMotionControllerComponent> TrackerLeftHand;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//TObjectPtr<UMotionControllerComponent> TrackerRightHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UMotionControllerComponent> TrackerRifle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCaptureDevice> CaptureDevice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTrackingDataComponents> TrackingData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UFloatingPawnMovement> FloatingPawnMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> IconTopViewWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> ShotStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UDecalComponent> ShotHitDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> AttachWeaponRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAudioComponent> AudioComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UPoseableMeshComponent> PoseableMesh; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> HMDMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> HeadMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<URtDirectionArrowComponent> DirectionArrow;
#pragma endregion Components

#pragma region Transforms
	// 캐릭터 애니매이션 IK용 Transform
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform AttachLeftHandRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform AttachLeftHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform AttachRightHandRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform AttachRightHand;

	void SendVRComponentTransforms();
	void ResvVRComponentTransforms(TArray<FVector> Pos, TArray<FRotator> Rot);

	UFUNCTION(BlueprintCallable)
	void GetFingerCurlsAndSplays(/*EHand Hand, FSteamVRFingerCurls& FingerCurls, FSteamVRFingerSplays& FingerSplays, ESkeletalSummaryDataType SummaryDataType = ESkeletalSummaryDataType::VR_SummaryType_FromAnimation*/);

	UFUNCTION(BlueprintCallable)
	void SetFingerCurls(/*FSteamVRFingerCurls& FingerCurls, float Thumb, float Index, float Middle, float Ring, float Pinky*/);

	// 무기, HMD 서버 통신용
private:
	FVector PrevWeaponRootLocation;
	FRotator PrevWeaponRootRotation;

	FVector PrevCameraLocation;
	FRotator PrevCameraRotation;

	UFUNCTION(Server, Unreliable)
	void Server_SetNotVR_Transform(const FVector& pawnLoc, const FRotator& pawnRot, const FVector& rifleLoc, const FRotator& rifleRot, const FVector& rifleHitLoc, const FRotator& rifleHitRot);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_SetNotVR_Transform(const FVector& pawnLoc, const FRotator& pawnRot, const FVector& rifleLoc, const FRotator& rifleRot, const FVector& rifleHitLoc, const FRotator& rifleHitRot);
	UFUNCTION(Exec)
	void RtResetTransform();

	UFUNCTION(Server, Unreliable)
	void Server_SetVR_Transform(
		const FVector& headLoc, const FVector& lControllerLoc, const FVector& rControllerLoc, const FVector& rifleRootLoc, const FVector& rifleHitLoc,
		const FRotator& headRot, const FRotator& lControllerRot, const FRotator& rControllerRot, const FRotator& rifleRootRot, const FRotator& rifleHitRot);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_SetVR_Transform(
		const FVector& headLoc, const FVector& lControllerLoc, const FVector& rControllerLoc, const FVector& rifleRootLoc, const FVector& rifleHitLoc,
		const FRotator& headRot, const FRotator& lControllerRot, const FRotator& rControllerRot, const FRotator& rifleRootRot, const FRotator& rifleHitRot);
#pragma endregion Transforms

#pragma region Walkie Talkie
public:
	void InitWalkieTalkie();
	void InitWalkieTalkieMenuWidget();

	void ToggleHideWalkieTalkie();
	void WalkieTalkieTransmitStart();

	void UpdateWalkieTalkieMenuList();

private:
	AActor* WalkieTalkieActor = nullptr;
	AActor* WalkieTalkieMenuActor = nullptr;

#pragma endregion

#pragma region Runtime Speech Recognizer
public:
	UPROPERTY()
	AActor* SpeechRecognizerActorRef;

	FString CurListeningNpcName;

	FString FindNpcInCamView();
	void ExecuteCommandFromString(const FString& RecognizedSpeechStr);
	void UpdateMicState();

	void CreateSpeechRecognizer();
	void DestroySpeechRecognizer();

private:
	TSharedPtr<IVoiceCapture> VoiceCapture;
	bool InitVoiceCapture();

	// 마이크 연결 상태 체크
	float MicConnectMonitorAccumulator = 0.0f;
	const float MicConnectMonitorInterval = 1.0f;

	bool IsActiveSpeechRecognizer = false;
	const float AllowedVoiceCommandDistance = 2000.0f; // 음성 명령 가능 범위
#pragma endregion Runtime Speech Recognizer

public:
	// Custom Get / Set
	ARtPlayerState* Get_PlayerState();
	ARtPlayerController* Get_PlayerController();
	bool IsGamePlay();

private:
	URtNetworkManager* Get_NetManager();

public:
	UPROPERTY(BlueprintReadOnly)
	bool IsVR = true; // VR (1), Screen & Keyboard & Mouse(0)
	UPROPERTY(BlueprintReadOnly)
	int32 WeaponTrackerMode = 0; // Tracker(0), LeftController(1), RightController(2)
	UPROPERTY(BlueprintReadOnly)
	bool IsUsingMotion = false; // UsingMotionCature(1), NotMotionCapture(0)

private:
	void SetPlaySettings();

	//dave
	void UpdateIconWidget();

public:
	void SetMeshVisible(bool isVisible);
	UFUNCTION(Server, Reliable)
	void Server_SetMeshVisible(bool isVisible);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetMeshVisible(bool isVisible);

	UFUNCTION(Exec)
	void SetViewpoint(uint8 viewpoint = 0, float armLength = 300.f);

	/*Set Keyboard, Mouse Input for No VR Debug*/
	UFUNCTION(Exec)
	void SetInputMode(bool isInputMode);
	/** Called for movement input */
	void Move(const FInputActionValue& value);
	void MoveForward(float value);
	void MoveRight(float value);
	/** Called for looking input */
	void Look(const FInputActionValue& value);
	/** Called for change selection menu input */
	void ChangeSelectionMenu(const FInputActionValue& value);

	void SetVisibleHead(bool isVisible);

	UFUNCTION(BlueprintCallable)
	bool IsHandNearEquipment(bool isRight);

#pragma region UltamateTracker & Calibration
public:
	//For UT
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> UTAnimInstanceClass;

	UPROPERTY(VisibleAnywhere)
	bool CanUTCalibrate = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsUTCalibrating = false;
	FVector MotionScale = FVector(1.0f);

	// CaptureDevice 컴포넌트에 SkeletalMesh 프리셋 설정
	void UT_LoadSkeletalMeshSetUp();
	void UT_Init();
	void UT_CalibrationInit();
	void UT_CalibrationPrepare();
	void UT_CalibrationReset();
	void UT_Calibrate();
	void UT_CalibrationEnd();

	UFUNCTION(BlueprintCallable)
	void NextCalibration(); // UT Calibration

	int32 GetTrackerId(UMotionControllerComponent* device);
	UFUNCTION(BlueprintPure)
	bool IsTracking(UMotionControllerComponent* tracker);
	bool CanCalibrate();

	UFUNCTION()
	void OnCompleteCalibration();

	FDelegate_CompleteCalibaration Delegate_CompleteCalibration; // 캘리브레이션 완료시 호출

private:
	FTimerHandle Calibration_Timer;
#pragma endregion UltamateTracker

	//캘리브레이션용 클론
#pragma region Clone
protected:
	bool IsClone = false;
	TObjectPtr<ARtClone> Clone = nullptr;

	UFUNCTION(Exec)
	void ShowClone(bool isCali = false);
	UFUNCTION(Exec)
	void MirrorClone();
	UFUNCTION(Exec)
	void HideClone();

public:
	void NextCloneMode(); // Clone
#pragma endregion Clone

#pragma region Widgets
public:
	TObjectPtr<class ARtWidgetActor> MainWidget;
	TObjectPtr<class ARtWidgetActor_Radio> RadioWidget;
	TObjectPtr<class ARtWidgetActor_Menu> TargetMenuWidget;
	TObjectPtr<class ARtWidgetActor> DangerDirectionWidget;
	TObjectPtr<class ARtWidgetActor_TraineeResult> ResultWidget;


	void Create_TraineeName_Widget();
	void Change_IconName(const FText& InText);
	void Init();
	class URt_Trainee_Main* GetMainWidget();

	void ShowResultWidget();
	void CloseResultWidget();

protected:
	void InitMainWidget();
	void InitMenuWidgets();

private:
	bool ResultWidgetOpened = false;
	float ElapedTimeAfterEnd = 0.0f;
	FTimerHandle OpenResultWidgetTimerHandle;
	FTimerHandle CloseResultWidgetTimerHandle;

#pragma endregion Widgets

#pragma region VR Passthrough
public:
	void ActivePassthrough(bool passthorughOnOff);

private:
	bool IsPassthroughMode = false;
	FPassthroughHandle PlanerHandle;
#pragma endregion VR Passthrough

	// Danger Alert
private:
	void UpdateDangerLevel();
	int32 DagerLevel = 0;
	UFUNCTION() void PlayDangerAlert();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USoundBase> DangerAlertSound = nullptr;
	FTimerHandle AlertTimerHandle;

protected:
	void SctInteract();
	void SelectMenu();
	void ReleaseMenu();
	//void Stick_R_Up();
	//void Stick_R_Down();

	void Shot();

	void GripL_Pressed();
	void GripR_Pressed();
	void GripL_Released();
	void GripR_Released();
	void Interact_Hand(class USphereComponent* handCollision);

	UFUNCTION()
	void OnLoadMapComplete();

	TWeakObjectPtr<AActor> TraineeLocActor;

public:
	bool IsPoseableMeshPose = false;

protected:
	void SetPoseableMeshPose();

public:
	AActor* GetTraineeLocActor();

	bool AttachWeapon(ARtObjectActor* weapon);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsEquip = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<ARtObjectActor> Equipment;

	// VR인경우 Actor는 움직이지 않기 때문에 실제 위치를 가져오기 위한 함수
	// 캘리브레이션이 된 경우는 SkeletaMesh, 캘리브레이션을 하지 않았거나 트래커를 사용하지 않는 경우 Camera(Z=0)
	FVector GetRootPosition(); 

protected:
	void ChangeAmmunitionMode();
	bool AmmunitionMode = false; // false: blank ammunition(공포탄), true: live ammunition(실탄). 실탄 종류도 여러 개로 바뀔 수 있음..
};
