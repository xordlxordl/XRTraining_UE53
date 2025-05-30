// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtTraineePawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "MotionControllerComponent.h"
//#include "XRMotionControllerBase.h"
#include "Components/DecalComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/SphereComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Net/UnrealNetwork.h"

#include "Common/RtBlueprintAssetPath.h"
#include "Framework/RtPlayerController.h"
#include "Network/RtNetworkManager.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Framework/RtClone.h"
#include "Framework/RtVRMeshes.h"
#include "Framework/RtWidgetActor_Menu.h"
#include "Framework/RtWidgetActor_Radio.h"
#include "Framework/RtWalkieTalkieActor.h"
#include "Framework/RtWalkieTalkieMenuActor.h"
#include "Framework/RtWidgetActor_TraineeResult.h"

#include "Manager/RtConfig.h"
#include "Manager/RtTableManager.h"
#include "Manager/RtGameManager.h"
#include "Manager/RtTrainingManager.h"
#include "Manager/RtGameScenarioController.h"
#include "Manager/RtGameSceneManager.h"
#include "Components/WidgetComponent.h"
#include "UI/Character/Rt_CharacterTopView.h"
#include "UI/Trainee/Rt_Trainee_Main.h"

//UT
#include "ViveMocapTypes.h"
#include "ViveMocapFunctionLibrary.h"
#include "CaptureDevice.h"
#include "TrackingDataComponents.h"
#include "MotionTrackedDeviceFunctionLibrary.h"

#include "FrameWork/RtWidgetActor.h"
#include "Framework/RtNpcObjectActor.h"
#include "Framework/RtStartAreaEffect.h"
#include "Framework/RtDirectionArrowComponent.h"

//Hand Tracking

ARtTraineePawn::ARtTraineePawn()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	DefaultRoot->SetupAttachment(RootComponent);
	DefaultRoot->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(DefaultRoot);

	HMDMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HMDMesh"));
	HMDMesh->SetupAttachment(Camera);
	HMDMesh->SetCollisionProfileName(TEXT("NoCollision"));

	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(HMDMesh);
	HeadMesh->SetCollisionProfileName(TEXT("NoCollision"));
	HeadMesh->SetRelativeLocation(FVector(-12.0f, 0.0f, -10));
	HeadMesh->SetRelativeRotation(FRotator(0, -90, 0));

	HMDMesh->SetOwnerNoSee(true);
	HeadMesh->SetOwnerNoSee(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(DefaultRoot);
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 160.0f));
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightController"));
	RightController->SetupAttachment(DefaultRoot);
	RightController->MotionSource = IMotionController::RightHandSourceId;

	RightHandCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RightHandCollision"));
	RightHandCollision->SetupAttachment(RightController);
	RightHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCollision->bNavigationRelevant = false;

	LeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftController"));
	LeftController->SetupAttachment(DefaultRoot);
	LeftController->MotionSource = IMotionController::LeftHandSourceId;

	LeftHandCollision = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHandCollision"));
	LeftHandCollision->SetupAttachment(LeftController);
	LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCollision->bNavigationRelevant = false;

	TrackerPelvis = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("TrackerPelvis"));
	TrackerPelvis->SetupAttachment(DefaultRoot);
	TrackerPelvis->MotionSource = FName(TEXT("Waist"));

	TrackerRightFoot = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("TrackerRightFoot"));
	TrackerRightFoot->SetupAttachment(DefaultRoot);
	TrackerRightFoot->MotionSource = FName(TEXT("RightFoot"));

	TrackerLeftFoot = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("TrackerLeftFoot"));
	TrackerLeftFoot->SetupAttachment(DefaultRoot);
	TrackerLeftFoot->MotionSource = FName(TEXT("LeftFoot"));

	//TrackerLeftHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("TrackerLeftHand"));
	//TrackerLeftHand->SetupAttachment(DefaultRoot);
	//TrackerLeftHand->MotionSource = FName(TEXT("tracker_shoulder_left"));

	//TrackerRightHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("TrackerRightHand"));
	//TrackerRightHand->SetupAttachment(DefaultRoot);
	//TrackerRightHand->MotionSource = FName(TEXT("tracker_shoulder_right"));

	TrackerRifle = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("TrackerRifle"));
	TrackerRifle->SetupAttachment(DefaultRoot);
	TrackerRifle->MotionSource = FName(TEXT("Pad"));

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(DefaultRoot);
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	SkeletalMesh->SetVisibility(true);

	PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMesh"));
	PoseableMesh->SetupAttachment(DefaultRoot);
	PoseableMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	PoseableMesh->SetVisibility(false);
	//bUseControllerRotationYaw = true;

	// TopView Widget
	Create_TraineeName_Widget();

	AttachWeaponRoot = CreateDefaultSubobject<USceneComponent>(TEXT("AttachWeaponRoot"));
	AttachWeaponRoot->SetupAttachment(TrackerRifle);

	ShotStart = CreateDefaultSubobject<USceneComponent>(TEXT("ShotStart"));
	ShotStart->SetupAttachment(AttachWeaponRoot);

	ShotHitDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("ShotHitDecal"));
	ShotHitDecal->SetupAttachment(ShotStart);

	CaptureDevice = CreateDefaultSubobject<UCaptureDevice>(TEXT("CaptureDevice"));
	CaptureDevice->MotionControllersTrackingRole = ETrackingDeviceRole::AnyRole;
	CaptureDevice->bDebugLog = false;

	TrackingData = CreateDefaultSubobject<UTrackingDataComponents>(TEXT("TrackingData"));
	TrackingData->SceneComponentNames = { RightController->GetFName(), LeftController->GetFName(), TrackerPelvis->GetFName(), TrackerRightFoot->GetFName(), TrackerLeftFoot->GetFName() };
	TrackingData->RightControllerComponentName = RightController->GetFName();
	TrackingData->LeftControllerComponentName = LeftController->GetFName();

	DirectionArrow = CreateDefaultSubobject<URtDirectionArrowComponent>(TEXT("DirectionArrow"));
	DirectionArrow->SetupAttachment(DefaultRoot);
	// Voice Construct
	//VoiceCaptureAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("VoiceCaptureAudioComponent"));
}

void ARtTraineePawn::Create_TraineeName_Widget()
{
	if (NULL == IconTopViewWidget)
	{
		IconTopViewWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameText"));
		IconTopViewWidget->SetupAttachment(SkeletalMesh);

		IconTopViewWidget->SetWidgetSpace(EWidgetSpace::World);
		IconTopViewWidget->SetDrawSize(FVector2D(1500 * 0.7f, 750 * 0.7f));
		IconTopViewWidget->SetPivot(FVector2D(0.5f, 1.f));
		IconTopViewWidget->AddRelativeLocation(FVector(0, 0, 200));
		IconTopViewWidget->AddRelativeRotation(FRotator(90, -90, GetActorRotation().Roll - 90));
		IconTopViewWidget->SetCollisionResponseToAllChannels(ECR_Ignore);
		IconTopViewWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		IconTopViewWidget->SetGenerateOverlapEvents(false);

		// dave
		IconTopViewWidget->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

		IconTopViewWidget->SetCastShadow(false);
	}
	IconTopViewWidget->SetIsReplicated(false);

	if (IconTopViewWidget->GetWidgetClass())
		return;

	UGameInstance* GI = UGameplayStatics::GetGameInstance(this);
	URtTableManager* table = GI ? GI->GetSubsystem<URtTableManager>() : NULL;
	TSubclassOf<UUserWidget> widgetclass = table ? table->Get_UIWidget("CharacterTopView") : NULL;
	if (widgetclass)
		IconTopViewWidget->SetWidgetClass(widgetclass);
}

void ARtTraineePawn::BeginPlay()
{
	Super::BeginPlay();

	if (UGameInstance* gi = GetGameInstance())
	{
		auto* GameManager = gi->GetSubsystem<URtGameManager>();

		if (GameManager)
		{
			if (GameManager->Get_Play_Type() == ERt_PlayType::E_Replay)
			{
				return;
			}
		}
	}

	if (!IsUsingMotion && !FloatingPawnMovement)
		FloatingPawnMovement = Cast<UFloatingPawnMovement>(AddComponentByClass(UFloatingPawnMovement::StaticClass(), true, FTransform::Identity, false));

	if (NULL == SkeletalMesh->GetSkeletalMeshAsset())
	{
		if (USkeletalMesh* skeletalMeshAsset = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, RtBlueprintAsset::TraineeSkeletalMesh)))
			SkeletalMesh->SetSkeletalMesh(skeletalMeshAsset);
	}

	if (UTAnimInstanceClass == nullptr)
		UTAnimInstanceClass = LoadClass<UAnimInstance>(nullptr, RtBlueprintAsset::TraineeUTAnimClass);

	if (UTAnimInstanceClass)
		SkeletalMesh->SetAnimClass(UTAnimInstanceClass);

	UT_LoadSkeletalMeshSetUp();

	if (UGameplayStatics::GetPlayerController(this, 0) == Controller)
		SetPlaySettings();

	UGameInstance* GI = UGameplayStatics::GetGameInstance(this);

	if (IconTopViewWidget)
	{
		if (!IconTopViewWidget->GetWidget())
		{
			URtTableManager* table = GI ? GI->GetSubsystem<URtTableManager>() : NULL;
			TSubclassOf<UUserWidget> widgetclass = table ? table->Get_UIWidget("CharacterTopView") : NULL;
			if (widgetclass)
				IconTopViewWidget->SetWidgetClass(widgetclass);

			IconTopViewWidget->InitWidget();
			IconTopViewWidget->UpdateWidget();
		}

		IconTopViewWidget->SetVisibleInSceneCaptureOnly(true);
	}

	if (DangerAlertSound == nullptr)
	{
		DangerAlertSound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, RtBlueprintAsset::DangerAlertSound));
	}

	if (AudioComp == nullptr)
	{
		AudioComp = NewObject<UAudioComponent>(this);
		AudioComp->SetupAttachment(RootComponent);
		AudioComp->bAutoActivate = true;
		AudioComp->bAlwaysPlay = true;

		if (GI)
		{
			if (URtConfig* config = GI->GetSubsystem<URtConfig>())
				AudioComp->SetVolumeMultiplier(config->TrainingOptions.AlertVolume);
		}

		if (DangerAlertSound)
		{
			AudioComp->SetSound(DangerAlertSound);
		}

	}

	TraineeLocActor = MakeWeakObjectPtr<AActor>(GetWorld()->SpawnActor<AActor>(LoadClass<AActor>(nullptr, RtSctPath::RtTraineeLocActor)));
	TraineeLocActor->AttachToComponent(Camera, FAttachmentTransformRules::KeepRelativeTransform);

	Set_Visiblity_MotionController(IsVR);
	if (!IsVR)
	{
		ShotStart->AttachToComponent(Camera, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		ShotStart->SetRelativeTransform(FTransform::Identity);
	}

	// Voice Init
	//VoiceCapture = FVoiceModule::Get().CreateVoiceCapture(""); // Get Default Voice Capture Device
	//VoiceCapture->Start();
	//
	//VoiceCaptureAudioComponent = NewObject<UAudioComponent>(this);
	//VoiceCaptureAudioComponent->SetupAttachment(RootComponent);
	//VoiceCaptureAudioComponent->bAutoActivate = true;
	//VoiceCaptureAudioComponent->bAlwaysPlay = true;
	////VoiceCaptureAudioComponent->PitchMultiplier = 0.85f;
	////VoiceCaptureAudioComponent->VolumeMultiplier = 5.f;
	//VoiceCaptureSoundWaveProcedural = NewObject<USoundWaveProcedural>();
	//VoiceCaptureSoundWaveProcedural->SetSampleRate(24000);
	//VoiceCaptureSoundWaveProcedural->NumChannels = 1;
	//VoiceCaptureSoundWaveProcedural->Duration = INDEFINITELY_LOOPING_DURATION;
	//VoiceCaptureSoundWaveProcedural->SoundGroup = SOUNDGROUP_Voice;
	//VoiceCaptureSoundWaveProcedural->bLooping = false;
	//VoiceCaptureSoundWaveProcedural->bProcedural = true;
	////VoiceCaptureSoundWaveProcedural->Pitch = 0.85f;
	////VoiceCaptureSoundWaveProcedural->Volume = 5.f;
	//
	//// Timer Set of Voice
	//GetWorldTimerManager().SetTimer(VoiceCaptureTickTimer, this, &ARtTraineePawn::VoiceCaptureTick, 0.02400f, true, 0.f); //<- VoiceCaptureTick 함수를 Tick 에서 호출하는 것으로 대체,0.024000f
	//GetWorldTimerManager().SetTimer(PlayVoiceCaptureTimer, this, &ARtTraineePawn::PlayVoiceCapture, 0.02400f, true, 0.f);

#if !WITH_EDITOR
	FWorldContext* context = GEngine->GetWorldContextFromWorld(this->GetWorld());
	UE_LOG(LogTemp, Log, TEXT("Instance%d : IsVR(%d) / WeaponTracker(%d) / UsingMotion(%d)"), context->PIEInstance, IsVR, WeaponTrackerMode, IsUsingMotion);
#endif
}

void ARtTraineePawn::Set_Visiblity_MotionController(bool isVisible)
{
	HMDMesh->SetVisibility(isVisible && IsVR);
	HeadMesh->SetVisibility(isVisible && IsVR);
	TrackerPelvis->SetShowDeviceModel(isVisible && IsUsingMotion);
	TrackerLeftFoot->SetShowDeviceModel(isVisible && IsUsingMotion);
	TrackerRightFoot->SetShowDeviceModel(isVisible && IsUsingMotion);
	TrackerRifle->SetShowDeviceModel(isVisible && IsUsingMotion);
	LeftController->SetShowDeviceModel(isVisible && IsUsingMotion);
	RightController->SetShowDeviceModel(isVisible && IsUsingMotion);
}

void ARtTraineePawn::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameInstance* gi = GetGameInstance();
	URtConfig* config = gi ? gi->GetSubsystem<URtConfig>() : NULL;
	if (config)
	{
		IsVR = config->TrainingOptions.IsVR && UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayConnected()/* && UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled()*/;

		UE_LOG(LogTemp, Log, TEXT("Option IsVR(%d) / IsConnected(%d) / IsEnabled(%d)"), config->TrainingOptions.IsVR, UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayConnected(), UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled());

#if WITH_EDITOR
		FWorldContext* context = GEngine->GetWorldContextFromWorld(this->GetWorld());
		IsVR = context->PIEInstance == 0 && IsVR;
		UE_LOG(LogTemp, Log, TEXT("PIEInstance %d"), context->PIEInstance);
#endif

		WeaponTrackerMode = config->TrainingOptions.WeaponTrackerMode;
		IsUsingMotion = IsVR && config->TrainingOptions.IsUsingMotion;

		Camera->bLockToHmd = IsVR;

		TrackerPelvis->bAutoActivate = TrackerLeftFoot->bAutoActivate = TrackerRightFoot->bAutoActivate = TrackerRifle->bAutoActivate = LeftController->bAutoActivate = RightController->bAutoActivate = IsUsingMotion;
		TrackerPelvis->SetComponentTickEnabled(IsUsingMotion);
		TrackerLeftFoot->SetComponentTickEnabled(IsUsingMotion);
		TrackerRightFoot->SetComponentTickEnabled(IsUsingMotion);
		TrackerRifle->SetComponentTickEnabled(IsUsingMotion);
		LeftController->SetComponentTickEnabled(IsUsingMotion);
		RightController->SetComponentTickEnabled(IsUsingMotion);
	}

	UE_LOG(LogTemp, Log, TEXT("PreInitailized  IsVR(%d) / WeaponTracker(%d) / UsingMotion(%d)"), IsVR, WeaponTrackerMode, IsUsingMotion);
}

void ARtTraineePawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ARtTraineePawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	IconTopViewWidget->SetWidget(NULL);
	if (APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (controller != Controller)
			return;

		if (Clone.Get())
			Clone->Destroy();

		if (MainWidget.Get())
			MainWidget->DestroyWidgetActor();


		GetWorld()->GetTimerManager().ClearTimer(AlertTimerHandle);

	}

	Super::EndPlay(EndPlayReason);
}

void ARtTraineePawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (auto* pc = Cast<ARtPlayerState>(GetPlayerState()))
	{
		pc->Get_OnPlayerBasicInfoChangedDelegate().AddDynamic(this, &ThisClass::OnPlayerBasicInfoChangedDelegate);
		pc->IsVR = IsVR;
	}
}

void ARtTraineePawn::UnPossessed()
{
	auto* pc = Cast<ARtPlayerState>(GetPlayerState());
	if (pc)
	{
		pc->Get_OnPlayerBasicInfoChangedDelegate().RemoveAll(this);
	}
	Super::UnPossessed();
}

void ARtTraineePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UGameplayStatics::GetPlayerController(GetWorld(), 0) == Controller)
	{
		if (!IsVR && !HasAuthority())
		{
			if (PrevWeaponRootLocation != AttachWeaponRoot->GetComponentLocation() ||
				PrevWeaponRootRotation != AttachWeaponRoot->GetComponentRotation())
				Server_SetNotVR_Transform(GetActorLocation(), GetActorRotation(),
					AttachWeaponRoot->GetComponentLocation(),
					AttachWeaponRoot->GetComponentRotation(),
					ShotHitDecal->GetComponentLocation(),
					ShotHitDecal->GetComponentRotation());
		}

		if (IsVR)
		{
			if (PrevCameraLocation != Camera->GetComponentLocation() ||
				PrevCameraRotation != Camera->GetComponentRotation())
			{
				Server_SetVR_Transform(
					Camera->GetComponentLocation(),
					LeftController->GetComponentLocation(),
					RightController->GetComponentLocation(),
					AttachWeaponRoot->GetComponentLocation(),
					ShotHitDecal->GetComponentLocation(),
					Camera->GetComponentRotation(),
					LeftController->GetComponentRotation(),
					RightController->GetComponentRotation(),
					AttachWeaponRoot->GetComponentRotation(),
					ShotHitDecal->GetComponentRotation());
			}
			UpdateDangerLevel();
		}

	}
	UpdateIconWidget();
	SetPoseableMeshPose();

	ARtGameScenarioController* ScenarioController = ARtGameScenarioController::Get(GetWorld());
	if (ScenarioController)
	{
		ARtScenarioModel* ScenarioModel = ScenarioController->ScenarioModel.Get();

		if (ScenarioModel)
		{
			if (ScenarioModel->GameState == ERtSctGameState::EGameState_End)
			{
				if (IsLocallyControlled())
				{
					ElapedTimeAfterEnd += DeltaTime;

					// Open Result Popup
					if (!ResultWidgetOpened && ElapedTimeAfterEnd > 1.0f)
					{
						ResultWidgetOpened = true;
						ElapedTimeAfterEnd = 0.0f;
						ShowResultWidget();
					}

					// Close Result Popup
					if (ResultWidgetOpened && ElapedTimeAfterEnd > 15.0f)
					{
						ScenarioModel->GameState = ERtSctGameState::EGameState_None;
						ResultWidgetOpened = false;
						ElapedTimeAfterEnd = 0.0f;
						CloseResultWidget();
					}
				}
			}
		}
	}

	if (IsGamePlay() && IsLocallyControlled())
	{
		// 현재 명령 가능한 NPC의 이름을 업데이트
		FString FoundNpcName = FindNpcInCamView();

		if (FoundNpcName != CurListeningNpcName)
		{
			ARtGameScenarioController::Get(GetWorld())->UpdateListeningNpc(FoundNpcName);
			CurListeningNpcName = FoundNpcName;
		}

		// 일정 간격마다 마이크 연결 상태 체크
		MicConnectMonitorAccumulator += DeltaTime;

		if (MicConnectMonitorAccumulator > MicConnectMonitorInterval)
		{
			UpdateMicState();

			MicConnectMonitorAccumulator = 0.0f;
		}
	}
}

// dave
void ARtTraineePawn::UpdateIconWidget()
{
	if (IsValid(IconTopViewWidget))
	{
		IconTopViewWidget->SetWorldLocation(GetActorLocation() + 200 * FVector::UpVector);
	}
}

// Called to bind functionality to input
void ARtTraineePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARtTraineePawn::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARtTraineePawn::Look);

		//SctInteract
		EnhancedInputComponent->BindAction(SctInteractAction, ETriggerEvent::Started, this, &ARtTraineePawn::SctInteract);

		//SctMenuSelect
		EnhancedInputComponent->BindAction(SctSelectMenuAction, ETriggerEvent::Started, this, &ARtTraineePawn::SelectMenu);
		EnhancedInputComponent->BindAction(SctSelectMenuAction, ETriggerEvent::Completed, this, &ARtTraineePawn::ReleaseMenu);

		//SctShot
		EnhancedInputComponent->BindAction(SctShotAction, ETriggerEvent::Started, this, &ARtTraineePawn::Shot);

		//ChangeSelectionMenu
		EnhancedInputComponent->BindAction(ChangeSelectionMenuAction, ETriggerEvent::Started, this, &ARtTraineePawn::ChangeSelectionMenu);

		//ChangeSelectionMenu
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ARtTraineePawn::ChangeAmmunitionMode);

	}

	PlayerInputComponent->BindAction("BtnX", EInputEvent::IE_Pressed, this, &ARtTraineePawn::NextCloneMode);
	PlayerInputComponent->BindAction("BtnY", EInputEvent::IE_Pressed, this, &ARtTraineePawn::NextCalibration);
	PlayerInputComponent->BindAction("BtnA", EInputEvent::IE_Pressed, this, &ARtTraineePawn::SelectMenu);
	PlayerInputComponent->BindAction("BtnB", EInputEvent::IE_Pressed, this, &ARtTraineePawn::SctInteract);
	PlayerInputComponent->BindAxis("ThumbX", this, &ARtTraineePawn::MoveForward);
	PlayerInputComponent->BindAxis("ThumbY", this, &ARtTraineePawn::MoveRight);

	if (IsVR)
	{
		// LEGACY

		//PlayerInputComponent->BindAction("TriggerL", EInputEvent::IE_Pressed, this, &ARtTraineePawn::Shot);
		//PlayerInputComponent->BindAction("TriggerR", EInputEvent::IE_Pressed, this, &ARtTraineePawn::Shot);
		//PlayerInputComponent->BindAction("Stick_R_Up", EInputEvent::IE_Pressed, this, &ARtTraineePawn::Stick_R_Up);
		//PlayerInputComponent->BindAction("Stick_R_Down", EInputEvent::IE_Pressed, this, &ARtTraineePawn::Stick_R_Down);
		//PlayerInputComponent->BindAction("GripL", EInputEvent::IE_Pressed, this, &ARtTraineePawn::ChangeAmmunitionMode);
		//PlayerInputComponent->BindAction("GripR", EInputEvent::IE_Pressed, this, &ARtTraineePawn::ChangeAmmunitionMode);

		EnhancedInputComponent->BindAction(BtnX, ETriggerEvent::Started, this, &ARtTraineePawn::NextCloneMode);
		EnhancedInputComponent->BindAction(BtnY, ETriggerEvent::Started, this, &ARtTraineePawn::NextCalibration);
		EnhancedInputComponent->BindActionValue(GripLAction);
		EnhancedInputComponent->BindActionValue(GripRAction);

		switch (WeaponTrackerMode)
		{
		case 1:
			PlayerInputComponent->BindAction("TriggerL", EInputEvent::IE_Pressed, this, &ARtTraineePawn::Shot);
			PlayerInputComponent->BindAction("GripL", EInputEvent::IE_Pressed, this, &ARtTraineePawn::ChangeAmmunitionMode);
			PlayerInputComponent->BindAction("GripR", EInputEvent::IE_Pressed, this, &ARtTraineePawn::GripR_Pressed);
			PlayerInputComponent->BindAction("GripR", EInputEvent::IE_Released, this, &ARtTraineePawn::GripR_Released);

			if (EnhancedInputComponent)
			{
				EnhancedInputComponent->BindAction(GripLAction, ETriggerEvent::Started, this, &ARtTraineePawn::ChangeAmmunitionMode);
				EnhancedInputComponent->BindAction(GripRAction, ETriggerEvent::Started, this, &ARtTraineePawn::GripR_Pressed);
				EnhancedInputComponent->BindAction(GripRAction, ETriggerEvent::Completed, this, &ARtTraineePawn::GripR_Released);
			}
			break;
		case 2:
			PlayerInputComponent->BindAction("TriggerR", EInputEvent::IE_Pressed, this, &ARtTraineePawn::Shot);
			PlayerInputComponent->BindAction("GripR", EInputEvent::IE_Pressed, this, &ARtTraineePawn::ChangeAmmunitionMode);
			PlayerInputComponent->BindAction("GripL", EInputEvent::IE_Pressed, this, &ARtTraineePawn::GripL_Pressed);
			PlayerInputComponent->BindAction("GripL", EInputEvent::IE_Released, this, &ARtTraineePawn::GripL_Released);
			if (EnhancedInputComponent)
			{
				EnhancedInputComponent->BindAction(GripRAction, ETriggerEvent::Started, this, &ARtTraineePawn::ChangeAmmunitionMode);
				EnhancedInputComponent->BindAction(GripLAction, ETriggerEvent::Started, this, &ARtTraineePawn::GripL_Pressed);
				EnhancedInputComponent->BindAction(GripLAction, ETriggerEvent::Completed, this, &ARtTraineePawn::GripL_Released);
			}
			break;
		default:
			PlayerInputComponent->BindAction("GripL", EInputEvent::IE_Pressed, this, &ARtTraineePawn::GripL_Pressed);
			PlayerInputComponent->BindAction("GripL", EInputEvent::IE_Released, this, &ARtTraineePawn::GripL_Released);
			PlayerInputComponent->BindAction("GripR", EInputEvent::IE_Pressed, this, &ARtTraineePawn::GripR_Pressed);
			PlayerInputComponent->BindAction("GripR", EInputEvent::IE_Released, this, &ARtTraineePawn::GripR_Released);

			if (EnhancedInputComponent)
			{
				EnhancedInputComponent->BindAction(GripLAction, ETriggerEvent::Started, this, &ARtTraineePawn::GripL_Pressed);
				EnhancedInputComponent->BindAction(GripLAction, ETriggerEvent::Completed, this, &ARtTraineePawn::GripL_Released);
				EnhancedInputComponent->BindAction(GripRAction, ETriggerEvent::Started, this, &ARtTraineePawn::GripR_Pressed);
				EnhancedInputComponent->BindAction(GripRAction, ETriggerEvent::Completed, this, &ARtTraineePawn::GripR_Released);
			}
			break;
		}
	}
}

void ARtTraineePawn::Handle_Changed_DeviceInfo()
{
	if (auto* ps = Get_PlayerState())
	{
		OnChanged_DeviceInfo_delegate.Broadcast(ps->Get_PlayerInfo(), ps);
	}
}

void ARtTraineePawn::OnPlayerBasicInfoChangedDelegate(ARtPlayerState* InPlayerState)
{
	if (InPlayerState)
	{
		Change_IconName(FText::FromString(InPlayerState->Login_DeviceName));
	}
}

void ARtTraineePawn::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (auto* ps = Cast<ARtPlayerState>(GetPlayerState()))
	{
		ps->Get_OnPlayerBasicInfoChangedDelegate().AddDynamic(this, &ARtTraineePawn::OnPlayerBasicInfoChangedDelegate);
		OnPlayerBasicInfoChangedDelegate(ps);
		ps->CallOrRegister_DeviceInfoChanged(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ARtTraineePawn::Handle_Changed_DeviceInfo));
	}
}

void ARtTraineePawn::InitWalkieTalkie()
{
	if (WalkieTalkieActor && !WalkieTalkieActor->IsPendingKill())
	{
		InitWalkieTalkieMenuWidget();

		WalkieTalkieActor->Destroy();
		WalkieTalkieActor = nullptr;
	}

	UClass* BPWalkieTalkieActorClass = StaticLoadClass(AActor::StaticClass(), nullptr, TEXT("/Game/RS/Training/Blueprints/WalkieTalkie/BP_WalkieTalkieActor.BP_WalkieTalkieActor_C"));

	if (BPWalkieTalkieActorClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			WalkieTalkieActor = World->SpawnActor<AActor>(BPWalkieTalkieActorClass);

			if (WalkieTalkieActor && Camera)
			{
				WalkieTalkieActor->AttachToComponent(Camera, FAttachmentTransformRules::KeepRelativeTransform);

				// 위치 조정: 플레이어 기준 앞쪽 + 위쪽
				const FVector OffsetLocation = FVector(25.0f, 30.0f, -11.0f);
				const FRotator OffsetRotation = FRotator(-5.0f, 120.0f, -5.0f);

				// Relative 위치 설정
				WalkieTalkieActor->SetActorRelativeLocation(OffsetLocation);
				WalkieTalkieActor->SetActorRelativeRotation(OffsetRotation);

				//WalkieTalkieActor->SetActorHiddenInGame(true);

				InitWalkieTalkieMenuWidget();
			}
		}
	}
}

void ARtTraineePawn::InitWalkieTalkieMenuWidget()
{
	if (WalkieTalkieMenuActor && !WalkieTalkieMenuActor->IsPendingKill())
	{
		WalkieTalkieMenuActor->Destroy();
		WalkieTalkieMenuActor = nullptr;

		return;
	}

	if (!WalkieTalkieActor) return;

	UClass* WalkieTalkieMenuClass = StaticLoadClass(AActor::StaticClass(), nullptr,
		TEXT("/Game/RS/Training/Blueprints/WalkieTalkie/BP_WalkieTalkieMenu.BP_WalkieTalkieMenu_C"));

	if (WalkieTalkieMenuClass)
	{
		WalkieTalkieMenuActor = GetWorld()->SpawnActor<AActor>(WalkieTalkieMenuClass);
		WalkieTalkieMenuActor->AttachToActor(WalkieTalkieActor, FAttachmentTransformRules::KeepRelativeTransform);

		const FVector OffsetLocation = FVector(0.0f, 2.25f, 7.0f);
		const FRotator OffsetRotation = FRotator(0.0f, 90.0f, 0.0f);

		WalkieTalkieMenuActor->SetActorRelativeLocation(OffsetLocation);
		WalkieTalkieMenuActor->SetActorRelativeRotation(OffsetRotation);
		WalkieTalkieMenuActor->SetActorRelativeScale3D(FVector(0.015f, 0.015f, 0.015f));

		//WalkieTalkieMenuActor->SetActorHiddenInGame(true);
	}
}

void ARtTraineePawn::ToggleHideWalkieTalkie()
{
	if (!WalkieTalkieActor || !WalkieTalkieMenuActor) return;

	ARtWalkieTalkieActor* WalkieTalkie = Cast<ARtWalkieTalkieActor>(WalkieTalkieActor);
	if (!WalkieTalkie) return;

	ARtWalkieTalkieMenuActor* WakieTalkieMenu = Cast<ARtWalkieTalkieMenuActor>(WalkieTalkieMenuActor);
	if (!WakieTalkieMenu) return;

	if (WakieTalkieMenu->GetIsAnimationPlaying()) return;

	if (WalkieTalkie->GetIsMoving()) return;

	if (WakieTalkieMenu->GetIsPopupMenuOpened())
	{
		WakieTalkieMenu->PressedBack();
	}

	if (WalkieTalkie->GetIsActivated())
	{
		WalkieTalkie->SetIsActivated(false);
		WalkieTalkie->MoveAnimation(FVector(25.0f, 8.0f, -11.0f), FVector(25.0f, 30.0f, -11.0f), 0.2f);
	}
	else
	{
		WalkieTalkie->SetIsActivated(true);
		WalkieTalkie->MoveAnimation(FVector(25.0f, 30.0f, -11.0f), FVector(25.0f, 8.0f, -11.0f), 0.2f);
	}
}

void ARtTraineePawn::WalkieTalkieTransmitStart()
{
	if (!WalkieTalkieActor || !WalkieTalkieMenuActor) return;

	ARtWalkieTalkieActor* WalkieTalkie = Cast<ARtWalkieTalkieActor>(WalkieTalkieActor);
	if (!WalkieTalkie) return;

	ARtWalkieTalkieMenuActor* WakieTalkieMenu = Cast<ARtWalkieTalkieMenuActor>(WalkieTalkieMenuActor);
	if (!WakieTalkieMenu) return;

	if (!WalkieTalkie->GetIsActivated() || WalkieTalkie->GetIsMoving()) return;

	if (WakieTalkieMenu->GetIsAnimationPlaying()) return;

	if (WakieTalkieMenu->GetIsPopupMenuOpened()) return;

	WalkieTalkie->ToggleScreenMaterial();
	WakieTalkieMenu->PressedTransmitting();
}

void ARtTraineePawn::UpdateWalkieTalkieMenuList()
{
	if (!WalkieTalkieMenuActor) return;

	ARtWalkieTalkieMenuActor* WakieTalkieMenu = Cast<ARtWalkieTalkieMenuActor>(WalkieTalkieMenuActor);
	if (!WakieTalkieMenu) return;

	WakieTalkieMenu->UpdateEventIDList();
}

// 시야 내의 가장 가까운 NPC의 이름 찾기
FString ARtTraineePawn::FindNpcInCamView()
{
	// 시야 내의 가장 가까운 NPC 찾기
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return FString();

	FVector CamLoc;
	FRotator CamRot;
	PC->GetPlayerViewPoint(CamLoc, CamRot);
	FVector CamForward = CamRot.Vector();

	TArray<AActor*> FoundNPCs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARtNpcObjectActor::StaticClass(), FoundNPCs);

	AActor* ClosestNpc = nullptr;
	float MinDist = FLT_MAX;
	float ViewAngleLimit = 30.0f;

	for (AActor* NPC : FoundNPCs)
	{
		if (!IsValid(NPC))
			continue;

		FVector ToNpc = NPC->GetActorLocation() - CamLoc;
		float Distance = ToNpc.Size();
		ToNpc.Normalize();

		// 시야 내 각도 제한, 가까운 거리라면 각도제한 없음
		if (Distance > 300.0f)
		{
			float Dot = FVector::DotProduct(CamForward, ToNpc);
			float Angle = FMath::Acos(Dot) * (180.f / PI);
			if (Angle > ViewAngleLimit)
				continue;
		}

		// 유효한 NPC인지 확인
		FString NpcName = Cast<ARtNpcObjectActor>(NPC)->GetObjectName();

		if (NpcName.IsEmpty())
			continue;

		if (!Cast<ARtObjectActor>(NPC)->IsVisible())
			continue;

		if (Distance < MinDist)
		{
			MinDist = Distance;
			ClosestNpc = NPC;
		}
	}

	if (ClosestNpc)
	{
		// 명령 가능 범위 밖이면 return
		FVector NpcLoc = ClosestNpc->GetActorLocation();
		float Distance = FVector::Dist(CamLoc, NpcLoc);
		if (Distance > AllowedVoiceCommandDistance)
		{
			return FString();
		}

		FString TargetNpcName = *Cast<ARtNpcObjectActor>(ClosestNpc)->GetObjectName();
		return TargetNpcName;
	}

	return FString();
}

void ARtTraineePawn::ExecuteCommandFromString(const FString& RecognizedSpeechStr)
{
	if (CurListeningNpcName.IsEmpty()) return;

	ARtGameScenarioController::Get(GetWorld())->ExecuteCommandToNpc(CurListeningNpcName, RecognizedSpeechStr);
}

void ARtTraineePawn::UpdateMicState()
{
	if (VoiceCapture.IsValid())
	{
		uint32 AvailableBytes = 0;
		EVoiceCaptureState::Type CaptureState = VoiceCapture->GetCaptureState(AvailableBytes);

		if (CaptureState == EVoiceCaptureState::Error)
		{
			if (IsActiveSpeechRecognizer)
			{
				DestroySpeechRecognizer();
			}

			InitVoiceCapture();
		}
		else
		{
			if (!IsActiveSpeechRecognizer)
			{
				CreateSpeechRecognizer();
				InitVoiceCapture();
			}
		}
	}
	else
	{
		if (InitVoiceCapture())
		{
			CreateSpeechRecognizer();
		}
	}
}

void ARtTraineePawn::CreateSpeechRecognizer()
{
	UClass* BPSpeechRecognizerClass = StaticLoadClass(AActor::StaticClass(), nullptr, TEXT("/Game/RS/Training/Blueprints/SpeechRecognize/BP_SpeechRecognizer.BP_SpeechRecognizer_C"));

	if (BPSpeechRecognizerClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			AActor* SpeechRecognizerActor = World->SpawnActor<AActor>(BPSpeechRecognizerClass);

			if (SpeechRecognizerActor)
			{
				SpeechRecognizerActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

				SpeechRecognizerActorRef = SpeechRecognizerActor;
				IsActiveSpeechRecognizer = true;

				UE_LOG(LogTemp, Error, TEXT("Mic Connect Test: CreateSpeechRecognizer"));
			}
		}
	}
}

void ARtTraineePawn::DestroySpeechRecognizer()
{
	if (SpeechRecognizerActorRef && SpeechRecognizerActorRef->IsValidLowLevel())
	{
		SpeechRecognizerActorRef->Destroy();
		SpeechRecognizerActorRef = nullptr;
		IsActiveSpeechRecognizer = false;

		UE_LOG(LogTemp, Log, TEXT("Mic Connect Test: DestroySpeechRecognizer"));
	}
}

bool ARtTraineePawn::InitVoiceCapture()
{
	if (VoiceCapture.IsValid())
	{
		VoiceCapture->Shutdown();
		VoiceCapture.Reset();
	}

	VoiceCapture = FVoiceModule::Get().CreateVoiceCapture(TEXT(""));

	if (VoiceCapture.IsValid())
	{
		VoiceCapture->Start();
		UE_LOG(LogTemp, Log, TEXT("Mic Connect Test: Microphone reset successful"));
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Mic Connect Test: Failed to reset microphone"));
		return false;
	}
}

ARtPlayerState* ARtTraineePawn::Get_PlayerState()
{
	return Cast<ARtPlayerState>(GetPlayerState());
}

ARtPlayerController* ARtTraineePawn::Get_PlayerController()
{
	return Cast<ARtPlayerController>(Controller);
}

void ARtTraineePawn::Change_IconName(const FText& InText)
{
	if (IconTopViewWidget)
	{
		if (auto* widget = Cast<URt_CharacterTopView>(IconTopViewWidget->GetWidget()))
		{
			widget->Change_Text(InText);
		}
	}
}

void ARtTraineePawn::SendVRComponentTransforms()
{
	if (UGameInstance* gi = GetGameInstance())
	{
		auto* gmr = gi->GetSubsystem<URtGameManager>();

		//int32 did = gmr->TrngManager

		TArray<FVector> locations = {
		Camera->GetComponentLocation(),
		LeftController->GetComponentLocation(),
		RightController->GetComponentLocation(),
		TrackerPelvis->GetComponentLocation(),
		TrackerLeftFoot->GetComponentLocation(),
		TrackerRightFoot->GetComponentLocation()
		};

		TArray<FRotator> rotations = {
		Camera->GetComponentRotation(),
		LeftController->GetComponentRotation(),
		RightController->GetComponentRotation(),
		TrackerPelvis->GetComponentRotation(),
		TrackerLeftFoot->GetComponentRotation(),
		TrackerRightFoot->GetComponentRotation()
		};

	}
}

void ARtTraineePawn::ResvVRComponentTransforms(TArray<FVector> Pos, TArray<FRotator> Rot)
{
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0) == Controller)
	{
		PrevCameraLocation = Pos[0];
		PrevCameraRotation = Rot[0];
	}
	else
	{
		Camera->SetWorldLocation(Pos[0]);
		Camera->SetWorldRotation(Rot[0]);

		LeftController->SetWorldLocation(Pos[1]);
		LeftController->SetWorldRotation(Rot[1]);

		RightController->SetWorldLocation(Pos[2]);
		RightController->SetWorldRotation(Rot[2]);

		TrackerPelvis->SetWorldLocation(Pos[3]);
		TrackerPelvis->SetWorldRotation(Rot[3]);

		TrackerLeftFoot->SetWorldLocation(Pos[4]);
		TrackerLeftFoot->SetWorldRotation(Rot[4]);

		TrackerRightFoot->SetWorldLocation(Pos[5]);
		TrackerRightFoot->SetWorldRotation(Rot[5]);
	}
}

void ARtTraineePawn::SetPlaySettings()
{
	URt_Trainee_Main* mainWidget = nullptr;

	if (!MainWidget)
	{
		MainWidget = ARtWidgetActor::CreateRtWidgetActor();

		UUserWidget* widget = MainWidget->Init(RtTraineeWidgetBlueprint::TraineeMainWidget);
		mainWidget = Cast<URt_Trainee_Main>(widget);
	}
	else
		mainWidget = Cast<URt_Trainee_Main>(MainWidget->WidgetComponent->GetWidget());

	mainWidget->UpdateRoleText();

	UHeadMountedDisplayFunctionLibrary::EnableHMD(IsVR);
	if (IsVR) // VR
	{
		Camera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		Camera->SetRelativeTransform(FTransform());

		SetInputMode(false);

		DangerDirectionWidget = ARtWidgetActor::CreateRtWidgetActor();
		if (DangerDirectionWidget)
		{
			UUserWidget* widget = DangerDirectionWidget->Init(RtTraineeWidgetBlueprint::DangerDirectionWidget);
			if (widget == nullptr)
			{
				DangerDirectionWidget->DestroyWidgetActor();
				DangerDirectionWidget = nullptr;
				UE_LOG(LogTemp, Log, TEXT("CreateDangerDirectionWidget : Create Widget Fail"));
			}
			DangerDirectionWidget->SetLength(200.f);
			DangerDirectionWidget->SetWidgetScale(5.0f);
		}

		UMotionControllerComponent* WeaponParent = nullptr;
		switch (WeaponTrackerMode)
		{
		case 1:
			WeaponParent = LeftController;
			RightHandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			RightHandCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
			RightHandCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
			break;
		case 2:
			WeaponParent = RightController;
			LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			LeftHandCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
			LeftHandCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
			break;
		default:
			WeaponParent = TrackerRifle;
			LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			LeftHandCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
			LeftHandCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
			RightHandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			RightHandCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
			RightHandCollision->SetCollisionResponseToAllChannels(ECR_Overlap);

			break;
		}
		AttachWeaponRoot->AttachToComponent(WeaponParent, FAttachmentTransformRules::KeepRelativeTransform);
		AttachWeaponRoot->SetRelativeTransform(FTransform());

		if (IsUsingMotion)
		{
			//FTimerHandle timerHandle;

			//GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateUObject(
			//	this, &ARtTraineePawn::UT_Init), 3.f, false);
			UT_Init();

			mainWidget->SetUsingTrackers(true);
		}
	}
	else // Screen & Keyboard & Mouse
	{
		SpringArm->TargetArmLength = 0.0f;
		Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
		Camera->SetRelativeTransform(FTransform());
		AttachWeaponRoot->AttachToComponent(Camera, FAttachmentTransformRules::KeepRelativeTransform);
		AttachWeaponRoot->SetRelativeTransform(FTransform());

		SetInputMode(true);

		if (Controller && !Get_PlayerController()->Get_GameManager()->Is_Play_Type(ERt_PlayType::E_Replay))
		{
			if (ARtPlayerController* rtPlayerController = Cast<ARtPlayerController>(Controller))
			{
				rtPlayerController->SetShowMouseCursor(false);
			}
		}
	}

	Delegate_CompleteCalibration.AddDynamic(this, &ARtTraineePawn::OnCompleteCalibration);

	ARtGameSceneManager* SceneManager = ARtGameSceneManager::Get(this);
	if (SceneManager)
		SceneManager->OnMapLoadComplete.AddUObject(this, &ARtTraineePawn::OnLoadMapComplete);

	SetVisibleHead(UGameplayStatics::GetPlayerController(GetWorld(), 0) != Controller);
	//SkeletalMesh->HideBoneByName(FName("neck_01"), EPhysBodyOp::PBO_None);//해당 본 그림자도 없어짐
	//PoseableMesh->HideBoneByName(FName("neck_01"), EPhysBodyOp::PBO_None);//해당 본 그림자도 없어짐


}

void ARtTraineePawn::SetMeshVisible(bool isVisible)
{
	if (!HasAuthority())
		Server_SetMeshVisible(isVisible);
	else
		Multicast_SetMeshVisible(isVisible);
}

void ARtTraineePawn::Server_SetMeshVisible_Implementation(bool isVisible)
{
	Multicast_SetMeshVisible(isVisible);
}

void ARtTraineePawn::Multicast_SetMeshVisible_Implementation(bool isVisible)
{
	if (IsPoseableMeshPose)
	{
		SkeletalMesh->SetVisibility(false);
		PoseableMesh->SetVisibility(isVisible);
	}
	else
	{
		SkeletalMesh->SetVisibility(isVisible);
		PoseableMesh->SetVisibility(false);
	}

	HeadMesh->SetVisibility(!isVisible && IsVR);
	HMDMesh->SetVisibility(!isVisible && IsVR);
}

void ARtTraineePawn::SetViewpoint(uint8 viewpoint, float armLength)
{
	bool isVRView = viewpoint != 1 && viewpoint != 3;

	UHeadMountedDisplayFunctionLibrary::EnableHMD(isVRView);
	SpringArm->TargetArmLength = viewpoint == 1 ? 0.0f : armLength;
	if (isVRView)
	{
		Camera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		AttachWeaponRoot->AttachToComponent(RightController, FAttachmentTransformRules::KeepRelativeTransform);
	}
	else
	{
		Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
		AttachWeaponRoot->AttachToComponent(Camera, FAttachmentTransformRules::KeepRelativeTransform);
	}
	Camera->SetRelativeTransform(FTransform());
	AttachWeaponRoot->SetRelativeTransform(FTransform());

	if (viewpoint != 3)
	{
		SetVisibleHead(false);
		//SkeletalMesh->HideBoneByName(FName("neck_01"), EPhysBodyOp::PBO_None);//해당 본 그림자도 없어짐
		//PoseableMesh->HideBoneByName(FName("neck_01"), EPhysBodyOp::PBO_None);//해당 본 그림자도 없어짐
	}
	else
	{
		SetVisibleHead(true);
		//SkeletalMesh->UnHideBoneByName(FName("neck_01"));
		//PoseableMesh->UnHideBoneByName(FName("neck_01"));
	}

	if (ARtPlayerController* rtPlayerController = Cast<ARtPlayerController>(Controller))
	{
		rtPlayerController->SetShowMouseCursor(!isVRView);
	}
}

bool ARtTraineePawn::IsGamePlay()
{
	if (ARtGameScenarioController* gsc = ARtGameScenarioController::Get(GetWorld()))
		return gsc->bIsGamePlay;
	else
		return false;
}

void ARtTraineePawn::RtResetTransform()
{
	SetActorLocation(FVector());
	SetActorRotation(FRotator());
	Controller->ClientSetRotation(FRotator(), false);
}

void ARtTraineePawn::Server_SetNotVR_Transform_Implementation(
	const FVector& pawnLoc, const FRotator& pawnRot, const FVector& rifleLoc, const FRotator& rifleRot, const FVector& rifleHitLoc, const FRotator& rifleHitRot)
{
	Multicast_SetNotVR_Transform(pawnLoc, pawnRot, rifleLoc, rifleRot, rifleHitLoc, rifleHitRot);
}

void ARtTraineePawn::Multicast_SetNotVR_Transform_Implementation(
	const FVector& pawnLoc, const FRotator& pawnRot, const FVector& rifleLoc, const FRotator& rifleRot, const FVector& rifleHitLoc, const FRotator& rifleHitRot)
{
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0) == Controller)
	{
		PrevWeaponRootLocation = rifleLoc;
		PrevWeaponRootRotation = rifleRot;
	}
	else
	{
		SetActorLocation(pawnLoc);
		SetActorRotation(pawnRot);
		AttachWeaponRoot->SetWorldLocation(rifleLoc);
		AttachWeaponRoot->SetWorldRotation(rifleRot);
		ShotHitDecal->SetWorldLocation(rifleHitLoc);
		ShotHitDecal->SetWorldRotation(rifleHitRot);
	}
}

void ARtTraineePawn::Server_SetVR_Transform_Implementation(
	const FVector& headLoc, const FVector& lControllerLoc, const FVector& rControllerLoc, const FVector& rifleRootLoc, const FVector& rifleHitLoc,
	const FRotator& headRot, const FRotator& lControllerRot, const FRotator& rControllerRot, const FRotator& rifleRootRot, const FRotator& rifleHitRot)
{
	Multicast_SetVR_Transform(
		headLoc, lControllerLoc, rControllerLoc, rifleRootLoc, rifleHitLoc,
		headRot, lControllerRot, rControllerRot, rifleRootRot, rifleHitRot);
}

void ARtTraineePawn::Multicast_SetVR_Transform_Implementation(const FVector& headLoc, const FVector& lControllerLoc, const FVector& rControllerLoc, const FVector& rifleRootLoc, const FVector& rifleHitLoc,
	const FRotator& headRot, const FRotator& lControllerRot, const FRotator& rControllerRot, const FRotator& rifleRootRot, const FRotator& rifleHitRot)
{
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0) == Controller)
	{
		PrevCameraLocation = headLoc;
		PrevCameraRotation = headRot;
	}
	else
	{
		Camera->SetWorldLocation(headLoc);
		LeftController->SetWorldLocation(lControllerLoc);
		RightController->SetWorldLocation(rControllerLoc);
		AttachWeaponRoot->SetWorldLocation(rifleRootLoc);
		ShotHitDecal->SetWorldLocation(rifleHitLoc);
		Camera->SetWorldRotation(headRot);
		LeftController->SetWorldRotation(lControllerRot);
		RightController->SetWorldRotation(rControllerRot);
		AttachWeaponRoot->SetWorldRotation(rifleRootRot);
		ShotHitDecal->SetWorldRotation(rifleHitRot);
	}
}

//Control for Test

void ARtTraineePawn::SetInputMode(bool isInputMode)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (isInputMode)
			{
				Subsystem->AddMappingContext(PcMappingContext, 0);
			}
			else
			{
				Subsystem->ClearAllMappings();
				Controller->ClientSetRotation(GetActorRotation(), false);
			}
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			bUseControllerRotationYaw = isInputMode;
		}
	}
}

void ARtTraineePawn::Move(const FInputActionValue& value)
{
	// input is a Vector2D
	FVector2D movementVector = value.Get<FVector2D>();

	// find out which way is forward
	const FRotator rotation = Controller->GetControlRotation();
	const FRotator yawRotation(0, rotation.Yaw, 0);

	// get forward vector
	const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	AddMovementInput(forwardDirection, movementVector.Y);
	AddMovementInput(rightDirection, movementVector.X);
}

void ARtTraineePawn::MoveForward(float value)
{
	const FRotator rotation = Camera->GetComponentRotation();
	const FRotator yawRotation(0, rotation.Yaw, 0);
	const FVector forwardDirection = yawRotation.Quaternion().GetRightVector();

	AddMovementInput(forwardDirection, value);
}

void ARtTraineePawn::MoveRight(float value)
{
	const FRotator rotation = Camera->GetComponentRotation();
	const FRotator yawRotation(0, rotation.Yaw, 0);
	const FVector rightDirection = yawRotation.Quaternion().GetForwardVector();

	AddMovementInput(rightDirection, value);
}

void ARtTraineePawn::Look(const FInputActionValue& value)
{
	// input is a Vector2D
	FVector2D lookAxisVector = value.Get<FVector2D>();

	// add yaw and pitch input to controller
	AddControllerYawInput(lookAxisVector.X);
	AddControllerPitchInput(lookAxisVector.Y);
}

void ARtTraineePawn::ChangeSelectionMenu(const FInputActionValue& value)
{
	if (!WalkieTalkieActor || !WalkieTalkieMenuActor) return;

	ARtWalkieTalkieActor* WalkieTalkie = Cast<ARtWalkieTalkieActor>(WalkieTalkieActor);
	if (!WalkieTalkie) return;

	ARtWalkieTalkieMenuActor* WakieTalkieMenu = Cast<ARtWalkieTalkieMenuActor>(WalkieTalkieMenuActor);
	if (!WakieTalkieMenu) return;

	if (!WalkieTalkie->GetIsActivated() || WalkieTalkie->GetIsMoving()) return;

	if (WakieTalkieMenu->GetIsPopupMenuOpened())
	{
		FVector2D changeVector = value.Get<FVector2D>();

		if (changeVector.Equals(FVector2D(0, 1), KINDA_SMALL_NUMBER))
		{
			// W
			WakieTalkieMenu->MoveMenuUp();
		}
		else if (changeVector.Equals(FVector2D(0, -1), KINDA_SMALL_NUMBER))
		{
			// X
			WakieTalkieMenu->MoveMenuDown();
		}
		else if (changeVector.Equals(FVector2D(1, 0), KINDA_SMALL_NUMBER))
		{
			// D
			WakieTalkieMenu->PressedOK();
		}
		else if (changeVector.Equals(FVector2D(-1, 0), KINDA_SMALL_NUMBER))
		{
			// A
			WakieTalkieMenu->PressedBack();
		}
	}
	else
	{
		WakieTalkieMenu->OpenPopupMenu();
	}
}

void ARtTraineePawn::SetVisibleHead(bool isVisible)
{
	TArray<USkinnedMeshComponent*> meshes = { SkeletalMesh, PoseableMesh };
	FName boneName("neck_01");

	for (USkinnedMeshComponent* mesh : meshes)
	{
		if (!mesh)
			continue;

		int32 boneIdx = INDEX_NONE;

		if (INDEX_NONE == (boneIdx = mesh->GetBoneIndex(boneName)))
			continue;

		if (isVisible)
			mesh->UnHideBone(boneIdx);
		else
			mesh->HideBone(boneIdx, EPhysBodyOp::PBO_None);
	}
}

bool ARtTraineePawn::IsHandNearEquipment(bool isRight)
{
	if (!CaptureDevice->IsCalibrated())
		return true;

	TArray<AActor*> actors;

	UKismetSystemLibrary::SphereOverlapActors(this, isRight ? RightHandCollision->GetComponentLocation() : LeftHandCollision->GetComponentLocation(), 15.f, { EObjectTypeQuery::ObjectTypeQuery2 }, NULL, {}, actors);

	return actors.Contains(Equipment);
}

void ARtTraineePawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

//For UT

void ARtTraineePawn::UT_LoadSkeletalMeshSetUp()
{
	CaptureDevice->InitializeReferences(SkeletalMesh);

	FSkeletonBasicPreset skeletonBasicPreset;
	skeletonBasicPreset.SkeletonBonesMap = CaptureDevice->SkeletonBonesMap;
	skeletonBasicPreset.SocketsMap = CaptureDevice->SocketsMap;
	skeletonBasicPreset.ForearmTwistsRight = CaptureDevice->ForearmTwistsRight;
	skeletonBasicPreset.ForearmTwistsLeft = CaptureDevice->ForearmTwistsLeft;
	skeletonBasicPreset.UpperarmTwistsRight = CaptureDevice->UpperarmTwistsRight;
	skeletonBasicPreset.UpperarmTwistsLeft = CaptureDevice->UpperarmTwistsLeft;
	skeletonBasicPreset.CalfTwistsRight = CaptureDevice->CalfTwistsRight;
	skeletonBasicPreset.CalfTwistsLeft = CaptureDevice->CalfTwistsLeft;
	skeletonBasicPreset.ThighTwistsRight = CaptureDevice->ThighTwistsRight;
	skeletonBasicPreset.ThighTwistsLeft = CaptureDevice->ThighTwistsLeft;

	CaptureDevice->LoadSkeletonSetupFromStruct(skeletonBasicPreset);
	CaptureDevice->InitializeFingers();

	CaptureDevice->UpdateSkeletonSetup();
}

void ARtTraineePawn::UT_Init()
{
	CanUTCalibrate = true;

	SetMeshVisible(false);
	Set_Visiblity_MotionController(true);

	//if (VRMeshes.Get())
	//	VRMeshes->SetTrackersVisibility(true);
}

void ARtTraineePawn::UT_CalibrationInit()
{
	SetMeshVisible(false);
	HideClone();

	if (CanCalibrate() == false)
	{
		GetMainWidget()->SetSctText(TEXT("트래커를 확인해주세요"), FColor::Red);
		FTimerHandle retimerHandle;
		GetWorld()->GetTimerManager().SetTimer(retimerHandle, FTimerDelegate::CreateUObject(
			this, &ARtTraineePawn::UT_CalibrationInit), 1.f, false);

		return;
	}

	SkeletalMesh->SetRelativeLocation(FVector::Zero());
	SkeletalMesh->SetRelativeScale3D(FVector(1.0f));
	MotionScale = FVector(1.0f);

	Set_Visiblity_MotionController(true);
	//if (VRMeshes.Get())
	//	VRMeshes->SetTrackersVisibility(true);

	FString str = FString::Printf(TEXT("캘리브레이션을 진행합니다. 잠시 멈춰서 대기해주세요\n%d"), 10);
	GetMainWidget()->SetSctText(*str, FColor::Red);

	double startTime = GWorld->TimeSeconds;
	TSharedPtr<FTimerHandle> textHandle = MakeShared<FTimerHandle>();
	GetWorld()->GetTimerManager().SetTimer(*textHandle, FTimerDelegate::CreateLambda([this, textHandle, startTime]() {
		int Time = GWorld->TimeSeconds - startTime;
		Time = 10 - Time;
		
		if (Time <= 0)
		{
			GetMainWidget()->RemoveSctText();
			GetWorld()->GetTimerManager().ClearTimer(*textHandle);
			return;
		}

		FString str = FString::Printf(TEXT("캘리브레이션을 진행합니다. 잠시 멈춰서 대기해주세요\n%d"), Time);
		GetMainWidget()->SetSctText(*str, FColor::Red);
		}), 1, true);

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateUObject(
		this, &ARtTraineePawn::UT_CalibrationPrepare), 10, false);
}

void ARtTraineePawn::UT_CalibrationPrepare()
{
	if (CanCalibrate() == false)
	{
		GetMainWidget()->SetSctText(TEXT("트래커를 확인해주세요"), FColor::Red);
		FTimerHandle retimerHandle;
		GetWorld()->GetTimerManager().SetTimer(retimerHandle, FTimerDelegate::CreateUObject(
			this, &ARtTraineePawn::UT_CalibrationPrepare), 1.f, false);
		return;
	}

	CaptureDevice->ToggleCapture(false);
	CaptureDevice->PrepareForCalibration();

	MotionScale = FVector::OneVector * SkeletalMesh->GetRelativeScale3D().Z;
	SkeletalMesh->SetRelativeScale3D(MotionScale);
	PoseableMesh->SetRelativeScale3D(MotionScale);

	GetMainWidget()->SetSctText(TEXT("앞으로 나란히 자세를 취하고 잠시 유지해주세요"), FColor::Red);

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateUObject(
		this, &ARtTraineePawn::UT_CalibrationReset), 3.f, false);
}

void ARtTraineePawn::UT_CalibrationReset()
{
	if (CanCalibrate() == false)
	{
		GetMainWidget()->SetSctText(TEXT("트래커를 확인해주세요"), FColor::Red);
		FTimerHandle retimerHandle;
		GetWorld()->GetTimerManager().SetTimer(retimerHandle, FTimerDelegate::CreateUObject(
			this, &ARtTraineePawn::UT_CalibrationReset), 1.f, false);
		return;
	}

	CaptureDevice->ResetPreCalibrationBinding();
	CaptureDevice->BindPreCalibrationTracker(GetTrackerId(TrackerPelvis), EHumanoidBone::Pelvis, false);
	CaptureDevice->BindPreCalibrationTracker(GetTrackerId(TrackerLeftFoot), EHumanoidBone::FootLeft, false);
	CaptureDevice->BindPreCalibrationTracker(GetTrackerId(TrackerRightFoot), EHumanoidBone::FootRight, false);
	//CaptureDevice->BindPreCalibrationTracker(GetTrackerId(LeftController), EHumanoidBone::PalmLeft, false);
	//CaptureDevice->BindPreCalibrationTracker(GetTrackerId(RightController), EHumanoidBone::PalmRight, false);
	//CaptureDevice->BindPreCalibrationTracker(GetTrackerId(TrackerRifle), EHumanoidBone::_TrackedDevices, true);
	//CaptureDevice->BindPreCalibrationTracker(GetTrackerId(TrackerLeftHand), EHumanoidBone::PalmLeft, false);
	//CaptureDevice->BindPreCalibrationTracker(GetTrackerId(TrackerRightHand), EHumanoidBone::PalmRight, false);
	CaptureDevice->UpdateSkeletonSetup();

	IsUTCalibrating = true;

	ShowClone(true);
	SetMeshVisible(true);

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateUObject(
		this, &ARtTraineePawn::UT_Calibrate), 0.1f, false);
}

void ARtTraineePawn::UT_Calibrate()
{
	if (!CanCalibrate())
	{
		GetMainWidget()->SetSctText(TEXT("트래커를 확인해주세요"), FColor::Red);
		FTimerHandle retimerHandle;
		GetWorld()->GetTimerManager().SetTimer(retimerHandle, FTimerDelegate::CreateUObject(
			this, &ARtTraineePawn::UT_Calibrate), 1.f, false);
		return;
	}

	bool isGrab = false;

	bool isleft = UEnhancedInputLibrary::GetBoundActionValue(this, GripLAction).Get<bool>();
	bool isright = UEnhancedInputLibrary::GetBoundActionValue(this, GripRAction).Get<bool>();

	isGrab = isleft && isright;

	//FString log = FString::Printf(TEXT("Left(%s) / Right(%s)")
	//	, *UKismetStringLibrary::Conv_BoolToString(isleft)
	//	, *UKismetStringLibrary::Conv_BoolToString(isright));
	//UKismetSystemLibrary::PrintString(this, log, true, true, FLinearColor::Red, 2, TEXT("Grab"));

	FString text = FString::Printf(TEXT("자세를 유지한 채 주먹을 쥐어주세요"));
	GetMainWidget()->SetSctText(text, FColor::Red);

	if (!isGrab)
	{
		FTimerHandle retimerHandle;
		GetWorld()->GetTimerManager().SetTimer(retimerHandle, FTimerDelegate::CreateUObject(
			this, &ARtTraineePawn::UT_Calibrate), 0.1f, false);
		return;
	}

	CaptureDevice->CalibrateBones();

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateUObject(
		this, &ARtTraineePawn::UT_CalibrationEnd), 0.2f, false);
}

void ARtTraineePawn::UT_CalibrationEnd()
{
	if (CanCalibrate() == false)
	{
		GetMainWidget()->SetSctText(TEXT("캘리브레이션을 실패했습니다.\n캘리브레이션을 다시 시작합니다."), FColor::Red);
		FTimerHandle retimerHandle;
		GetWorld()->GetTimerManager().SetTimer(retimerHandle, FTimerDelegate::CreateUObject(
			this, &ARtTraineePawn::UT_CalibrationInit), 1.f, false);

		return;
	}

	CaptureDevice->ToggleCapture(true);
	IsUTCalibrating = false;
	MirrorClone();

	CaptureDevice->ClavicleRotationOffset_Pitch = 0;
	CaptureDevice->ClavicleRotationOffset_Yaw = 0;

	GetMainWidget()->SetSctText(TEXT("캘리브레이션이 완료되었습니다"), FColor::Green);

	if (Delegate_CompleteCalibration.IsBound())
		Delegate_CompleteCalibration.Broadcast();

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([this]() {
		HideClone();
		GetMainWidget()->RemoveSctText();
		Set_Visiblity_MotionController(false);
		}), 5, false);

	//if (VRMeshes.Get())
	//	VRMeshes->SetTrackersVisibility(false);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("UT Calibration End"));
}

int32 ARtTraineePawn::GetTrackerId(UMotionControllerComponent* device)
{
	int32 id = UViveMocapFunctionLibrary::GetInternalDeviceIdByMotionSource(device->MotionSource);
	//FString s = FString::Printf(TEXT("TrackerID : %d : %s"), id, *(device->MotionSource.ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Magenta, s);
	return id;
}

bool ARtTraineePawn::IsTracking(UMotionControllerComponent* tracker)
{
	return tracker ? tracker->IsTracked() : false;
	//FVector loc = tracker->GetRelativeLocation();

	//return !loc.IsZero();
}

bool ARtTraineePawn::CanCalibrate()
{
	bool result = IsTracking(TrackerPelvis) && IsTracking(TrackerLeftFoot) && IsTracking(TrackerRightFoot);
	//result = result && IsTracking(LeftController) && IsTracking(RightController);

	return result;
}

void ARtTraineePawn::OnCompleteCalibration()
{
	ARtPlayerState* pState = Get_PlayerState();
	if (!pState)
		return;

	FRtDeviceInfo info = pState->Get_PlayerInfo();
	info.CompleteCalibration = true;
	pState->Server_Change_ReadyState(info);
}

URtNetworkManager* ARtTraineePawn::Get_NetManager()
{
	if (auto* gi = GetGameInstance())
		return gi->GetSubsystem<URtNetworkManager>();

	return nullptr;
}

void ARtTraineePawn::SelectMenu()
{
	ARtGameScenarioController::Get(GetWorld())->SelectMenu();

	/*if (!WalkieTalkieActor || !WalkieTalkieMenuActor) return;

	ARtWalkieTalkieActor* WalkieTalkie = Cast<ARtWalkieTalkieActor>(WalkieTalkieActor);
	if (!WalkieTalkie) return;

	ARtWalkieTalkieMenuActor* WakieTalkieMenu = Cast<ARtWalkieTalkieMenuActor>(WalkieTalkieMenuActor);
	if (!WakieTalkieMenu) return;

	if (!WalkieTalkie->GetIsActivated() || WalkieTalkie->GetIsMoving()) return;

	if (WakieTalkieMenu->GetIsAnimationPlaying()) return;

	if (WakieTalkieMenu->GetIsPopupMenuOpened()) return;

	WalkieTalkie->ToggleScreenMaterial();
	WakieTalkieMenu->PressedTransmitting();*/
}

void ARtTraineePawn::ReleaseMenu()
{
	if (!WalkieTalkieActor || !WalkieTalkieMenuActor) return;

	ARtWalkieTalkieActor* WalkieTalkie = Cast<ARtWalkieTalkieActor>(WalkieTalkieActor);
	if (!WalkieTalkie) return;

	ARtWalkieTalkieMenuActor* WakieTalkieMenu = Cast<ARtWalkieTalkieMenuActor>(WalkieTalkieMenuActor);
	if (!WakieTalkieMenu) return;

	if (!WalkieTalkie->GetIsSending()) return;

	WalkieTalkie->ToggleScreenMaterial();
	WakieTalkieMenu->ReleasedTransmitting();
}

void ARtTraineePawn::SctInteract()
{
	ARtGameScenarioController::Get(GetWorld())->Interact_AimTarget();

	/*if (!WalkieTalkieActor || !WalkieTalkieMenuActor) return;

	ARtWalkieTalkieActor* WalkieTalkie = Cast<ARtWalkieTalkieActor>(WalkieTalkieActor);
	if (!WalkieTalkie) return;

	ARtWalkieTalkieMenuActor* WakieTalkieMenu = Cast<ARtWalkieTalkieMenuActor>(WalkieTalkieMenuActor);
	if (!WakieTalkieMenu) return;

	if (WakieTalkieMenu->GetIsAnimationPlaying()) return;

	if (WalkieTalkie->GetIsMoving()) return;

	if (WakieTalkieMenu->GetIsPopupMenuOpened())
	{
		WakieTalkieMenu->PressedBack();
	}

	if (WalkieTalkie->GetIsActivated())
	{
		WalkieTalkie->SetIsActivated(false);
		WalkieTalkie->MoveAnimation(FVector(25.0f, 15.0f, -11.0f), FVector(25.0f, 35.0f, -11.0f), 0.2f);
	}
	else
	{
		WalkieTalkie->SetIsActivated(true);
		WalkieTalkie->MoveAnimation(FVector(25.0f, 35.0f, -11.0f), FVector(25.0f, 15.0f, -11.0f), 0.2f);
	}*/
}

void ARtTraineePawn::Shot()
{
	//UKismetSystemLibrary::ExecuteConsoleCommand(this, TEXT("HighResShot 2"));
	//return;

	//#ifdef WITH_EDITOR
	//
	//	FRotator Rotation;
	//	FVector Postion;
	//	FTransform trackerTransform(_Direction, _Position);
	//	FTransform attachTransform;
	//	attachTransform = attachTransform.GetRelativeTransform(trackerTransform);
	//	Equipment->SetActorRelativeTransform(attachTransform);
	//
	//#endif

#if WITH_EDITOR
	if (ARtGameScenarioController::Get(GetWorld()))
	{
		FString logStr = FString::Printf(TEXT("Send Shot: %s"), *ARtGameScenarioController::Get(GetWorld())->RifleAimedObjectKeyName);
		const uint64 screenKey = GetTypeHash(FName("Shot"));
		GEngine->AddOnScreenDebugMessage(screenKey, 5.f, FColor::Orange, logStr);
	}
#endif

	if (Equipment.IsValid())
	{
		Equipment.Get()->PlaySound();
		Equipment->PlaySingleEffect(ERtComponentType::EEffect, FVector::ZeroVector);
	}

	if (!IsGamePlay())
		return;

	FRtDeviceInfo currinfo;
	if (Get_PlayerController()->Get_DeviceInfo(currinfo))
	{
		int32 UID = currinfo.PlayerInfo.UID;
		FString weaponID = Equipment.IsValid() ? Equipment.Get()->GetObjectName() : TEXT("");
		FString hitObjID = ARtGameScenarioController::Get(GetWorld())->RifleAimedObjectKeyName;
		FVector hitPos = ARtGameScenarioController::Get(GetWorld())->RifleAimedHitLocation;

		if (!Get_PlayerController()->Get_GameManager()->Is_Play_Type(ERt_PlayType::E_Single))
		{
			if (auto* nmgr = Get_NetManager())
			{
				nmgr->SendPacket_TraineeShot(weaponID, hitObjID, hitPos, ShotStart->GetComponentLocation(), ShotStart->GetForwardVector(), AmmunitionMode);
			}

			if (AmmunitionMode)
			{
				Get_PlayerController()->Log_Message(ELogProtocol::LOG_FIRE); // 실탄 사격 횟수 증가

				if (ARtNpcObjectActor* npc = Cast<ARtNpcObjectActor>(ARtGameScenarioController::Get(GetWorld())->AimedTarget.Get()))
				{
					if (npc->GetNpcBehaviorState().ActType != ERtAnimType::ECollapse)
					{
						ERtRoleType roleType = npc->GetRoleType();
						switch (roleType)
						{
						case ERtRoleType::ERoleCivilian:
						case ERtRoleType::ERoleSoldier:
						case ERtRoleType::ERoleHostage:
							Get_PlayerController()->Log_Message(ELogProtocol::LOG_BADFIRE); // 오인 사격
							break;
						case ERtRoleType::ERoleSuspicious:
						case ERtRoleType::ERoleTerrorist:
							Get_PlayerController()->Log_Message(ELogProtocol::LOG_HIT); // 사격 적중
							break;
						default:
							break;
						}
					}
				}
			}
		}
		else
			ARtGameScenarioController::Get(GetWorld())->AddShotList(weaponID, hitObjID, hitPos, ShotStart->GetComponentLocation(), ShotStart->GetForwardVector(), AmmunitionMode);
	}
}

void ARtTraineePawn::GripL_Pressed()
{
	if (IsHandNearEquipment(false))
		return;

	Interact_Hand(LeftHandCollision.Get());
}

void ARtTraineePawn::GripR_Pressed()
{
	if (IsHandNearEquipment(true))
		return;

	Interact_Hand(RightHandCollision.Get());
}

void ARtTraineePawn::GripL_Released()
{
	ARtGameScenarioController::Get(GetWorld())->HideMenuWidget();
}

void ARtTraineePawn::GripR_Released()
{
	ARtGameScenarioController::Get(GetWorld())->HideMenuWidget();
}

void ARtTraineePawn::Interact_Hand(USphereComponent* handCollision)
{
	TArray<AActor*> actors;
	handCollision->GetOverlappingActors(actors);
	actors.Remove(this);

	if (actors.IsEmpty())
	{
		ARtGameScenarioController::Get(GetWorld())->Interact_Target(nullptr, handCollision);
		return;
	}

	for (AActor* actor : actors)
	{
		if (ARtObjectActor* objectActor = Cast<ARtObjectActor>(actor))
		{
			ARtGameScenarioController::Get(GetWorld())->Interact_Target(objectActor, handCollision);
			return;
		}
	}
}

void ARtTraineePawn::OnLoadMapComplete()
{
	ARtGameSceneManager* SceneManager = ARtGameSceneManager::Get(this);
	ARtPlayerState* pState = Get_PlayerState();

	if (SceneManager && pState)
	{
		FRtMCTTemplate mct = SceneManager->GetMctData();
		FRtDeviceInfo Info = pState->Get_PlayerInfo();
		Info.CompleteMapLoad = true;
		pState->Server_Change_ReadyState(Info);

		FString role = Info.PlayerRoleID;

		FMapTemplateCharacter* pTemplate = mct.CharacterTypeList.FindByPredicate([&](const FMapTemplateCharacter& character) {
			return character.TemplateName == role;
			});

		if (pTemplate)
		{
			GetMainWidget()->SetSctText(TEXT("훈련 준비를 위해 화살표가 가리키는 곳으로 이동해주세요"), FColor::White);

			TObjectPtr<ARtStartAreaEffect> pEffect = Cast<ARtStartAreaEffect>(GetWorld()->SpawnActor<AActor>(LoadClass<AActor>(nullptr, RtBlueprintAsset::RtStartAreaEffect), pTemplate->TemplateLocation, pTemplate->TemplateRotation));

			if (pEffect)
				pEffect->SetPlayer(this);

			DirectionArrow->SetTargetActor(pEffect);

			GetWorld()->GetTimerManager().SetTimer(Calibration_Timer, FTimerDelegate::CreateLambda([&]() {
				if (Get_PlayerState()->Get_PlayerInfo().InStartingPoint)
				{
					if (IsVR)
					{
						bool isMotion = UMotionTrackedDeviceFunctionLibrary::IsMotionSourceTracking(0, TEXT("Waist")) ||
							UMotionTrackedDeviceFunctionLibrary::IsMotionSourceTracking(0, TEXT("LeftFoot")) ||
							UMotionTrackedDeviceFunctionLibrary::IsMotionSourceTracking(0, TEXT("RightFoot"));

						if (!isMotion)
						{
							SetMeshVisible(true);
							Delegate_CompleteCalibration.Broadcast();
						}
						else
							NextCalibration();
					}
					else
						Delegate_CompleteCalibration.Broadcast();

					GetWorld()->GetTimerManager().ClearTimer(Calibration_Timer);
				}
				}), 0.1, true);
		}
		else
			UE_LOG(LogTemp, Error, TEXT("Can't Found %s Role In MCT Data"), *role);
	}
}

//void ARtTraineePawn::Stick_R_Up()
//{
//	if (GetMainWidget()->SelectUp())
//		return;
//
//	if (ARtGameScenarioController* sceCont = ARtGameScenarioController::Get(GetWorld()))
//		sceCont->ChangeSelectionMenu(1.f);
//}
//
//void ARtTraineePawn::Stick_R_Down()
//{
//	if (GetMainWidget()->SelectDown())
//		return;
//
//	if (ARtGameScenarioController* sceCont = ARtGameScenarioController::Get(GetWorld()))
//		sceCont->ChangeSelectionMenu(-1.f);
//}

void ARtTraineePawn::ShowClone(bool isCali)
{
	IsClone = true;
	if (!Clone.Get())
		Clone = GetWorld()->SpawnActor<ARtClone>(LoadClass<ARtClone>(nullptr, RtBlueprintAsset::RtClone));

	if (isCali)
		Clone->CalibrationClone();
	else
		Clone->ShowClone();
}

void ARtTraineePawn::MirrorClone()
{
	IsClone = true;
	if (Clone.Get())
		Clone->ShowMirrorClone();
}

void ARtTraineePawn::HideClone()
{
	IsClone = false;
	if (Clone.Get())
		Clone->HideClone();
}

void ARtTraineePawn::NextCloneMode()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("Press Btn X"));
	if (IsClone)
	{
		if (Clone.Get())
		{
			if (Clone->IsMirror)
				HideClone();
			else
				MirrorClone();
		}
	}
	else
		ShowClone();
}

void ARtTraineePawn::NextCalibration()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("Press Btn Y"));
	if (!CanUTCalibrate)
		return;
	if (IsUTCalibrating)
	{
		UT_Calibrate();
	}
	else
	{
		UT_CalibrationInit();
	}
}

void ARtTraineePawn::UpdateDangerLevel()
{
	ARtGameSceneManager* gameSceneManager = ARtGameSceneManager::Get(GetWorld());
	if (!IsValid(gameSceneManager))
		return;

	const TMap<FString, ARtTraineePawn*>& traineeMap = gameSceneManager->GetRtTraineeMap();

	FVector alertSettings = FVector::ZeroVector;
	float alertDistance_HMD = 0.f;
	if (UGameInstance* gi = GetGameInstance())
		if (URtConfig* config = gi->GetSubsystem<URtConfig>())
		{
			alertSettings = config->GetAlertDistance_Wall();
			alertDistance_HMD = config->GetAlertDistance_HMD();
		}

	if (alertDistance_HMD <= 0.0f)
		return;

	float minDistance = FMath::Max(alertSettings.X, alertSettings.Y) * 100.0f; // 30m => 3,000cm
	float alertStepDistance = alertDistance_HMD * 100.0f; // 1m => 100cm 마다 알림 단계 증가

	for (TPair<FString, ARtTraineePawn*> traineePair : traineeMap)
	{
		ARtTraineePawn* traineePawn = traineePair.Value;
		// dave
		if (!IsValid(traineePawn) || !traineePawn->IsValidLowLevel())
			continue;

		if (traineePawn == this)
			continue;

		FVector myLoc = Camera->GetComponentLocation();
		myLoc.Z = 0.f;
		FVector otherLoc = traineePawn->Camera->GetComponentLocation();
		otherLoc.Z = 0.f;
		float distance = FVector::Distance(myLoc, otherLoc);
		minDistance = FMath::Min(distance, minDistance);
	}

	int level = 3 - FMath::Clamp(FMath::FloorToInt(minDistance / alertStepDistance), 0, 3);

	//FString str = FString::Printf(TEXT("Danger Lv %d, %f"), level, minDistance);
	//GEngine->AddOnScreenDebugMessage(0, 0.1f, FColor::Emerald, str);

	if (DagerLevel == level)
		return;

	DagerLevel = level;

	if (DagerLevel <= 0)
		GetWorld()->GetTimerManager().ClearTimer(AlertTimerHandle);
	else
	{
		float interval = 0;
		switch (DagerLevel)
		{
		case 1:
			interval = 2.0f;
			break;
		case 2:
			interval = 1.0f;
			break;
		case 3:
			interval = 0.5f;
			break;
		}


		float remain = GetWorld()->GetTimerManager().IsTimerActive(AlertTimerHandle) ?
			GetWorld()->GetTimerManager().GetTimerRemaining(AlertTimerHandle) : 0.0f;
		GetWorld()->GetTimerManager().ClearTimer(AlertTimerHandle);
		//GetWorld()->GetTimerManager().SetTimer(AlertTimerHandle, FTimerDelegate::CreateUObject(
		//	this, &ARtTraineePawn::PlayDangerAlert), interval, true, FMath::Min(remain, interval));

		GetWorld()->GetTimerManager().SetTimer(AlertTimerHandle, this, &ARtTraineePawn::PlayDangerAlert, interval, true, FMath::Min(remain, interval));
	}

}

void ARtTraineePawn::PlayDangerAlert()
{
	if (AudioComp)
	{
		AudioComp->Play();
	}

	//if (DangerAlertSound) {
	//	if (!DangerAlertSound->GetSoundClass()) {
	//		UE_LOG(LogTemp, Log, TEXT("No Sound Class"));
	//	}
	//	UE_LOG(LogTemp, Log, TEXT("Play DangerAlertSound"));
	//	UGameplayStatics::PlaySound2D(GetWorld(), DangerAlertSound, 1.0f, 1.0f);
	//}
}


void ARtTraineePawn::SetPoseableMeshPose()
{
	if (!PoseableMesh->GetSkinnedAsset() || !PoseableMesh->IsVisible())
		return;

	int boneNum = SkeletalMesh->GetBoneSpaceTransforms().Num();
	if (boneNum == 0)
		return;

	if (CaptureDevice->IsCapturing() && CaptureDevice->IsCalibrated())
		PoseableMesh->SetWorldLocation(SkeletalMesh->GetBoneLocation("Root"));
	else
		PoseableMesh->SetWorldLocation(Camera->GetComponentLocation());

	for (int i = 0; i < boneNum; i++)
	{
		FName boneName = SkeletalMesh->GetBoneName(i);
		FTransform localTransform = SkeletalMesh->GetBoneTransform(i);

		FTransform rotT;
		rotT.SetRotation(FQuat(FRotator(-90.0f, 0.0f, -90.0f)));
		localTransform = rotT * localTransform;
		PoseableMesh->SetBoneTransformByName(boneName, localTransform, EBoneSpaces::WorldSpace);

	}
}

bool ARtTraineePawn::AttachWeapon(ARtObjectActor* weapon)
{
	if (weapon == nullptr)
	{
		AttachLeftHandRoot.SetLocation(FVector::ZeroVector);
		AttachLeftHandRoot.SetRotation(FRotator::ZeroRotator.Quaternion());
		AttachLeftHand.SetLocation(FVector::ZeroVector);

		AttachRightHandRoot.SetLocation(FVector::ZeroVector);
		AttachRightHandRoot.SetRotation(FRotator::ZeroRotator.Quaternion());
		AttachRightHand.SetLocation(FVector::ZeroVector);

		return false;
	}

	if (weapon->GetAttachParentActor() == this)
		return true;

	weapon->AttachToComponent(AttachWeaponRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(""));

	// 무기 리소스 어태치
	if (const FRtTctStruct* data = weapon->GetTctStruct())
	{
		if (!data->ComEquipList.IsEmpty())
		{
			const FRtTctComEquip& equipData = data->ComEquipList[0];

			FTransform trackerTransform(equipData.TrackerSocket.Rotation.Quaternion(), equipData.TrackerSocket.Position);
			FTransform LHandTransform(equipData.LeftHandSocket.Rotation.Quaternion(), equipData.LeftHandSocket.Position);
			FTransform RHandTransform(equipData.RightHandSocket.Rotation.Quaternion(), equipData.RightHandSocket.Position);

			FTransform attachTransform = FTransform::Identity;
			attachTransform = attachTransform.GetRelativeTransform(trackerTransform);
			/*	LHandTransform = LHandTransform.GetRelativeTransform(trackerTransform);
				RHandTransform = RHandTransform.GetRelativeTransform(trackerTransform);*/

			FVector lL = LHandTransform.GetLocation();
			FVector rL = RHandTransform.GetLocation();
			FRotator lR = LHandTransform.GetRotation().Rotator();
			FRotator rR = RHandTransform.GetRotation().Rotator();

			//attachTransform.SetScale3D(data->ComDefault.Scale);
			AttachWeaponRoot->SetRelativeTransform(attachTransform);

			AttachLeftHandRoot.SetLocation(LHandTransform.GetLocation());
			AttachLeftHandRoot.SetRotation(LHandTransform.GetRotation());
			AttachLeftHand.SetLocation(FVector(-10.0f, 4.0f, -2.0f));

			AttachRightHandRoot.SetLocation(RHandTransform.GetLocation());
			AttachRightHandRoot.SetRotation(RHandTransform.GetRotation());
			AttachRightHand.SetLocation(FVector(15.5f, -3.5f, -3.0f));
		}
	}

	// TCT 리뉴얼 전
	//else if (UGameInstance* gi = GetGameInstance())
	//{
	//	if (URtConfig* config = gi->GetSubsystem<URtConfig>())
	//	{
	//		if (config->TrainingOptions.IsWeaponTracker) // K2C1
	//		{
	//			FTransform attachTransform;
	//			attachTransform.SetLocation(FVector(-28.0f, 0.0f, -20.0f));
	//			attachTransform.SetRotation(FRotator(0.0f, -90.0f, 0.0f).Quaternion());
	//			attachTransform.SetScale3D(FVector(0.7f));
	//			weapon->SetActorRelativeTransform(attachTransform);
	//			weapon->GetCurrentMesh()->SetRelativeRotation(FRotator::ZeroRotator);

	//			AttachLeftHandRoot.SetLocation(FVector(-16.0f, 0.0f, -6.0f));
	//			AttachLeftHandRoot.SetRotation(FRotator(-28.0f, 80.0f, 90.0f).Quaternion());
	//			AttachLeftHand.SetLocation(FVector(-10.0f, 4.0f, -2.0f));

	//			AttachRightHandRoot.SetLocation(FVector(-33.0f, 2.0f, -7.0f));
	//			AttachRightHandRoot.SetRotation(FRotator(-10.f, -165.0f, -8.0f).Quaternion());
	//			AttachRightHand.SetLocation(FVector(15.5f, -3.5f, -3.0f));
	//		}			
	//		else // Only Controller
	//		{
	//			FTransform attachTransform;
	//			attachTransform.SetLocation(FVector(7.0f, 0.0f, -23.0f));
	//			attachTransform.SetRotation(FRotator(0.0f, -90.0f, 0.0f).Quaternion());
	//			attachTransform.SetScale3D(FVector(0.7f));
	//			weapon->SetActorRelativeTransform(attachTransform);
	//			weapon->GetCurrentMesh()->SetRelativeRotation(FRotator::ZeroRotator);

	//			AttachLeftHandRoot.SetLocation(FVector(0.0f, 0.0f, -8.0f));
	//			AttachLeftHandRoot.SetRotation(FRotator(-28.0f, 80.0f, 90.0f).Quaternion());
	//			AttachLeftHand.SetLocation(FVector(-10.0f, 4.0f, -2.0f));

	//			AttachRightHandRoot.SetLocation(FVector(-10.0f, 2.0f, -8.0f));
	//			AttachRightHandRoot.SetRotation(FRotator(-10.f, -165.0f, -8.0f).Quaternion());
	//			AttachRightHand.SetLocation(FVector(15.5f, -3.5f, -3.0f));
	//		}
	//	}
	//}

	IsEquip = true;
	Equipment = MakeWeakObjectPtr<ARtObjectActor>(weapon);

	return true;
}

AActor* ARtTraineePawn::GetTraineeLocActor()
{
	if (TraineeLocActor.IsValid())
		return TraineeLocActor.Get();

	TraineeLocActor = MakeWeakObjectPtr<AActor>(GetWorld()->SpawnActor<AActor>(LoadClass<AActor>(nullptr, RtSctPath::RtTraineeLocActor)));
	TraineeLocActor->AttachToComponent(Camera, FAttachmentTransformRules::KeepRelativeTransform);

	return TraineeLocActor.Get();
}

void ARtTraineePawn::Init()
{
	AmmunitionMode = false;

	InitMainWidget();
	InitMenuWidgets();

	DestroySpeechRecognizer();
	InitWalkieTalkie();
	CurListeningNpcName = TEXT("");
}

void ARtTraineePawn::ShowResultWidget()
{
	UClass* ResultWidgetActorClass = LoadClass<AActor>(nullptr, RtTraineeWidgetBlueprint::RtTraineeResultWidgetActor);
	if (!ResultWidgetActorClass) return;

	UWorld* World = GetWorld();
	if (World)
	{
		ResultWidget = World->SpawnActor<ARtWidgetActor_TraineeResult>(ResultWidgetActorClass);

		if (ResultWidget && Camera)
		{
			ResultWidget->AttachToComponent(Camera, FAttachmentTransformRules::KeepRelativeTransform);

			// 위치 조정: 플레이어 기준 앞쪽 + 위쪽
			const FVector OffsetLocation = FVector(5.0f, 0.0f, 0.0f);
			const FRotator OffsetRotation = FRotator(0.0f, 180.0f, 0.0f);

			// Relative 위치 설정
			ResultWidget->SetActorRelativeLocation(OffsetLocation);
			ResultWidget->SetActorRelativeRotation(OffsetRotation);

			ResultWidget->InitWidget();
		}
	}

	// Shot Hit Decal Off
	if (ShotHitDecal)
	{
		ShotHitDecal->SetVisibility(false);
	}
}

void ARtTraineePawn::CloseResultWidget()
{
	if (ResultWidget && ResultWidget->IsValidLowLevel())
	{
		ResultWidget->Destroy();
		ResultWidget = nullptr;
	}

	if (ShotHitDecal)
	{
		ShotHitDecal->SetVisibility(true);
	}
}

void ARtTraineePawn::InitMainWidget()
{
	URt_Trainee_Main* mainWidget = nullptr;

	if (!MainWidget)
	{
		MainWidget = ARtWidgetActor::CreateRtWidgetActor();

		UUserWidget* widget = MainWidget->Init(RtTraineeWidgetBlueprint::TraineeMainWidget);
		mainWidget = Cast<URt_Trainee_Main>(widget);
	}
	else
		mainWidget = GetMainWidget();

	mainWidget->UpdateRoleText();
	mainWidget->UpdateRadioMenu();

	mainWidget->SetAmmunitionType(AmmunitionMode);

}

void ARtTraineePawn::InitMenuWidgets()
{
	//Create Radio Widget Actor
	if (!RadioWidget)
	{
		RadioWidget = GetWorld()->SpawnActor<ARtWidgetActor_Radio>(ARtWidgetActor_Radio::StaticClass(), FTransform());

		TSubclassOf<UUserWidget> widgetClass = LoadClass<UUserWidget>(nullptr, RtTraineeWidgetBlueprint::SoldierRadioWidget);
		if (widgetClass == nullptr)
			return;

		UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), widgetClass);
		if (widget == nullptr)
			return;

		RadioWidget->WidgetComponent->SetWidget(widget);

	}

	RadioWidget->RadioInit();

	//Create Target Menu Widget Actor
	if (!TargetMenuWidget)
	{
		TargetMenuWidget = GetWorld()->SpawnActor<ARtWidgetActor_Menu>(ARtWidgetActor_Menu::StaticClass(), FTransform());
	}
}

URt_Trainee_Main* ARtTraineePawn::GetMainWidget()
{
	return Cast<URt_Trainee_Main>(MainWidget->WidgetComponent->GetWidget());
}

FVector ARtTraineePawn::GetRootPosition()
{
	FVector Location = FVector::ZeroVector;
	if (IsVR)
	{
		if (CaptureDevice->IsCalibrated())
			Location = SkeletalMesh->GetComponentLocation();
		else
		{
			Location = Camera->GetComponentLocation();
			Location.Z = 0;
		}
	}
	else
		Location = GetActorLocation();

	return Location;
}

void ARtTraineePawn::ChangeAmmunitionMode()
{
	AmmunitionMode = !AmmunitionMode;
	if (URt_Trainee_Main* mainWidget = GetMainWidget())
	{
		mainWidget->SetAmmunitionType(AmmunitionMode);

	}
}

void ARtTraineePawn::GetFingerCurlsAndSplays(/*EHand Hand, FSteamVRFingerCurls& FingerCurls, FSteamVRFingerSplays& FingerSplays, ESkeletalSummaryDataType SummaryDataType*/)
{
	/*if (UGameplayStatics::GetPlayerController(GetWorld(), 0) != Controller)
	{
		SetFingerCurls(FingerCurls, 0.75f, 0.75f, 0.75f, 0.75f, 0.75f);
		return;
	}

	UDEPRECATED_USteamVRInputDeviceFunctionLibrary::GetFingerCurlsAndSplays(Hand, FingerCurls, FingerSplays, SummaryDataType);

	FingerCurls.Thumb = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 0.7f), FVector2D(0.0f, 1.0f), FingerCurls.Thumb);
	FingerCurls.Index = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 0.8f), FVector2D(0.0f, 1.0f), FingerCurls.Index);
	FingerCurls.Middle = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 0.8f), FVector2D(0.0f, 1.0f), FingerCurls.Middle);
	FingerCurls.Ring = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 0.8f), FVector2D(0.0f, 1.0f), FingerCurls.Ring);
	FingerCurls.Pinky = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 0.8f), FVector2D(0.0f, 1.0f), FingerCurls.Pinky);*/

}

void ARtTraineePawn::SetFingerCurls(/*FSteamVRFingerCurls& FingerCurls, float Thumb, float Index, float Middle, float Ring, float Pinky*/)
{
	/*FingerCurls.Thumb = Thumb;
	FingerCurls.Index = Index;
	FingerCurls.Middle = Middle;
	FingerCurls.Ring = Ring;
	FingerCurls.Pinky = Pinky;*/
}


//// Mocopi Test
//void ARtTraineePawn::OnLiveLinkUpdated(float InDeltaTime)
//{
//	//USkeletalMeshComponent* mesh = SkeletalMesh;
//	//if (!mesh)
//	//	return;
//	//FTransform mesh_tr = mesh->GetComponentTransform();
//	//FVector mesh_loc = mesh->GetComponentLocation();
//	//FRotator mesh_rot = mesh->GetComponentRotation();
//	//
//	//FName root_name = TEXT("root");
//	//FSubjectFrameHandle Mocopi_SubjectFrameHandle;
//	//if (Mocopi_LLComp) {
//	//	bool bSuccess;
//	//	Mocopi_LLComp->GetSubjectData(Mocopi_SubjectName, bSuccess, Mocopi_SubjectFrameHandle);
//	//	if (bSuccess) {
//	//		FLiveLinkTransform OutLiveLinkTransform;
//	//		Mocopi_SubjectFrameHandle.GetTransformByName(root_name, OutLiveLinkTransform);
//	//
//	//		const FLiveLinkAnimationFrameData* LLAnimationFrameData = Mocopi_SubjectFrameHandle.GetSourceAnimationFrameData();
//	//		const FLiveLinkSkeletonStaticData* LLSkeletonStaticData = Mocopi_SubjectFrameHandle.GetSourceSkeletonStaticData();
//	//
//	//		if (LLAnimationFrameData && LLSkeletonStaticData) {
//	//			const TArray<FTransform>& temp_Bone_Transforms = LLAnimationFrameData->Transforms;
//	//			const TArray<FName>& Bone_Names = LLSkeletonStaticData->BoneNames;
//	//			const TArray<int32>& BoneParents = LLSkeletonStaticData->BoneParents;
//	//
//	//			int32 bone_cnt = Bone_Names.Num();
//	//			int32 tr_cnt = temp_Bone_Transforms.Num();
//	//
//	//			for (int i = 0; i < Bone_Names.Num(); ++i) {
//	//				auto& tr = Mocopi_BoneTransform.FindOrAdd(Bone_Names[i]);
//	//				int32 parent_idx = BoneParents[i];
//	//				FTransform temp_tr;
//	//				Get_Transform(Bone_Names[i], temp_tr);
//	//
//	//				FQuat rotation = ConvertBoneRotation(temp_tr.GetRotation());
//	//				FVector position = ConvertRootPosition(temp_tr.GetLocation());
//	//				FTransform hipsTransform(rotation, position);
//	//				hipsTransform = hipsTransform * mesh_tr;
//	//				tr = hipsTransform;
//	//			}
//	//		}
//	//	}
//	//}
//	//Debug_Draw_MocopiBones();
//}
//
//FVector ARtTraineePawn::ConvertRootPosition(FVector position) const
//{
//	return FVector(position.X, position.Z, position.Y);
//}
//
//
//FQuat ARtTraineePawn::ConvertBoneRotation(FQuat rotation) const
//{
//	return FQuat(rotation.X, rotation.Z, rotation.Y, -rotation.W);
//}
//
//void ARtTraineePawn::Get_Transform(FName InName, FTransform& OutTransform)
//{
//	FSubjectFrameHandle Mocopi_SubjectFrameHandle;
//	if (Mocopi_LLComp) {
//		bool bSuccess;
//		Mocopi_LLComp->GetSubjectData(Mocopi_SubjectName, bSuccess, Mocopi_SubjectFrameHandle);
//		if (bSuccess) {
//			FLiveLinkTransform OutLiveLinkTransform;
//			Mocopi_SubjectFrameHandle.GetTransformByName(InName, OutLiveLinkTransform);
//			OutLiveLinkTransform.GetTransformRootSpace(OutTransform);
//		}
//	}
//}
//
//void ARtTraineePawn::Debug_Draw_MocopiBones()
//{
//	for (auto& it : Mocopi_BoneTransform) {
//		Debug_DrawCircle(GetWorld(), it.Value);
//	}
//}
//
//void ARtTraineePawn::Debug_DrawCircle(UWorld* World, FTransform& InTrans)
//{
//	FVector temp = InTrans.GetLocation();
//	DrawDebugCircle(World, temp, 4, 10, FColor(181, 0, 0), false, 1.f);
//}
