// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtWidgetActor_Menu.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

#include "Common/RtBlueprintAssetPath.h"
#include "UI/Trainee/Rt_Trainee_MenuOption.h"
#include "Framework/RtTraineePawn.h"
#include "Framework/RtGuideHand.h"
#include "Manager/RtGameScenarioController.h"
#include "Network/RtNetworkManager.h"
#include "Framework/RtWidgetActor_Radio.h"

ARtWidgetActor_MenuOption::ARtWidgetActor_MenuOption()
{
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(DefaultRoot);
	SphereCollision->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(SphereCollision);
	WidgetComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	WidgetComponent->SetRelativeScale3D(FVector(DefaultSize));
	WidgetComponent->SetDrawSize(FVector2D(100.0f, 200.0f));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("/Game/Materials/M_WidgetActor.M_WidgetActor"));
	if (Material.Succeeded())
		WidgetComponent->SetMaterial(0, Material.Object);
	WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ARtWidgetActor_MenuOption::BeginPlay()
{
	Super::BeginPlay();

	WidgetComponent->SetRelativeScale3D(FVector(DefaultSize));

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ARtWidgetActor_MenuOption::OnOverlapBegin);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ARtWidgetActor_MenuOption::OnOverlapEnd);

}

void ARtWidgetActor_MenuOption::Hover(float NewPercent)
{
	WidgetComponent->SetRelativeScale3D(FVector(HoverSize));
	SetPercent(NewPercent);
}

void ARtWidgetActor_MenuOption::Unhover()
{
	WidgetComponent->SetRelativeScale3D(FVector(DefaultSize));
	SetPercent(0.0f);
}

void ARtWidgetActor_MenuOption::SetPercent(float NewPercent)
{
	URt_Trainee_MenuOption* OptionWidget = Cast<URt_Trainee_MenuOption>(WidgetComponent->GetWidget());
	if (OptionWidget == nullptr)
		return;
	OptionWidget->SetPercent(NewPercent);

}

void ARtWidgetActor_MenuOption::Execute()
{
	Unhover();

	if (ARtGameScenarioController* gsc = ARtGameScenarioController::Get(GetWorld())) {
		if (gsc->CurrentPlayType != ERt_PlayType::E_Single) {
			// 멀티 플레이
			if (gsc->bIsGamePlay == false) {
				//HideMenuWidget();

				return;
			}

			if (auto* nmgr = gsc->Get_NetManager()) {
				nmgr->SendPacket_SCTEventID(EventID);
			}

		}
		else {
			// 싱글 플레이
			gsc->ExecuteServerEvent(EventID);
		}

		TPair<const FRtEventCommand*, FString> data = gsc->ScenarioModel->ServerEventCommandMap.FindRef(EventID);
		FRtEventCommand* commands = (FRtEventCommand*)data.Key;
		const FString menuSetID = data.Value;

		if (commands == nullptr)
			return;

		gsc->Force_RequestRandomNum(&commands->ProcessCommandList);

		for (const FRtSctFunctionCommand& command : commands->ProcessCommandList) {
			const ERtSctFunCmd& cmd = command.FunctionCommand;
			if (cmd == ERtSctFunCmd::RecordResultLog) {
				if (auto found = gsc->CallCommandFunctionMap.Find(cmd)) {
					bool retValue = false;
					if (found->ExecuteIfBound(command.Params, retValue))
					{
					}
				}
			}
		}

	}
}

void ARtWidgetActor_MenuOption::PostExecute(ARtWidgetActor_Menu* Menu)
{
	if (ARtGameScenarioController* gsc = ARtGameScenarioController::Get(GetWorld())) {
		gsc->HideMenuWidget(true);
	}
}

void ARtWidgetActor_MenuOption::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		// Your overlap begin logic here
		UE_LOG(LogTemp, Warning, TEXT("Overlap Begin with %s"), *OtherActor->GetName());
	}

	if (ARtTraineePawn* traineePawn = Cast<ARtTraineePawn>(GetWorld()->GetFirstPlayerController()->GetPawn())) {
		if (traineePawn == nullptr)
			return;
		if (OtherActor != traineePawn
			|| (OtherComp != traineePawn->LeftHandCollision
				&& OtherComp != traineePawn->RightHandCollision)) {
			return;
		}
	}

	AddHoverOption.ExecuteIfBound(this);
}

void ARtWidgetActor_MenuOption::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		// Your overlap end logic here
		UE_LOG(LogTemp, Warning, TEXT("Overlap End with %s"), *OtherActor->GetName());
	}

	if (APlayerController* pc = GetWorld()->GetFirstPlayerController()) {
		if (ARtTraineePawn* traineePawn = Cast<ARtTraineePawn>(pc->GetPawn())) {
			if (OtherActor != traineePawn
				|| (OtherComp != traineePawn->LeftHandCollision
					&& OtherComp != traineePawn->RightHandCollision)) {
				return;
			}
		}

		RemoveHoverOption.ExecuteIfBound(this);
	}

}

ARtWidgetActor_Menu::ARtWidgetActor_Menu()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	DefaultRoot->SetupAttachment(RootComponent);

	RelativeRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RelativeRoot"));
	RelativeRoot->SetupAttachment(DefaultRoot);
}

void ARtWidgetActor_Menu::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Clear();

	Super::EndPlay(EndPlayReason);
}

void ARtWidgetActor_Menu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateBillboardRotation();

	if (HoverOption.IsValid()) {
		OptionOverlapTime += DeltaTime;
		if (OptionOverlapTime >= MaxOptionOverlapTime) {
			HoverOption.Get()->Execute();
			HoverOption.Get()->PostExecute(this);
			HideOptions();
			HoverOption.Reset();
			OptionOverlapTime = 0.0f;
			return;
		}
		HoverOption.Get()->Hover(OptionOverlapTime / MaxOptionOverlapTime);
	}
}

void ARtWidgetActor_Menu::Clear()
{
	if (!MenuOptionArray.IsEmpty()) {
		for (ARtWidgetActor_MenuOption* MenuOption : MenuOptionArray) {
			if (MenuOption)
				MenuOption->Destroy();
		}
		MenuOptionArray.Empty();
	}

	HoverOption.Reset();
	
	if (GuideHand.Get()) {
		GuideHand->IsGraped = false;
		GuideHand->UpdateVisible();
	}

}

void ARtWidgetActor_Menu::SetTarget(AActor* target, FVector worldLocation)
{
	if (APlayerCameraManager* cameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager) {
		FVector cameraLocation = cameraManager->GetCameraLocation();
		FVector newLocation;
		FRotator newRotation = FRotator().ZeroRotator;

		newLocation = target->GetActorLocation();
		SetActorLocation(newLocation);
		SetActorRelativeRotation(FRotator());
		FVector cameraLocCalc = cameraLocation;
		cameraLocCalc.Z = worldLocation.Z;
		newRotation = UKismetMathLibrary::FindLookAtRotation(cameraLocCalc, worldLocation);
		RelativeRoot->SetWorldRotation(newRotation);
		RelativeRoot->SetWorldLocation(worldLocation);

		AttachToActor(target, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void ARtWidgetActor_Menu::UpdateBillboardRotation()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	APlayerCameraManager* CameraManager = PC->PlayerCameraManager;
	if (!CameraManager || !RelativeRoot) return;

	//FVector CameraLocation = CameraManager->GetCameraLocation();
	//FVector ActorLocation = GetActorLocation();
	//CameraLocation.Z = ActorLocation.Z;

	//// LookAt 회전값 계산
	//FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, ActorLocation);

	//// 부모의 회전 영향을 제거하기 위해 상대 회전 초기화
	//SetActorRelativeRotation(FRotator::ZeroRotator);

	//// 루트 컴포넌트를 월드 기준으로 회전 적용
	//RelativeRoot->SetWorldRotation(LookAtRotation);

	// 카메라가 바라보는 회전값
	FRotator CameraRotation = CameraManager->GetCameraRotation();

	// 내 액터가 따라야 할 방향: 카메라가 보고 있는 방향의 Yaw만 추출
	FRotator NewRotation = FRotator(0.0f, CameraRotation.Yaw, 0.0f);

	// 부모의 회전 영향을 제거 (상대 회전 초기화)
	SetActorRelativeRotation(FRotator::ZeroRotator);

	// 루트 컴포넌트를 월드 기준으로 회전 적용
	RelativeRoot->SetWorldRotation(NewRotation);


	// 일정 거리 이상 멀어지면, 메뉴창 Off
	FVector CameraLocation = CameraManager->GetCameraLocation();
	FVector ActorLocation = GetActorLocation();
	float Distance = FVector::Dist(CameraLocation, ActorLocation);

	ARtGameScenarioController* gsc = ARtGameScenarioController::Get(GetWorld());
	if (!gsc) return;

	if (ARtWidgetActor_Radio* RadioWidgetActor = Cast<ARtWidgetActor_Radio>(this))
	{
		// 현재 위젯액터가 Radio 메뉴일 때
		if (Distance > RadioOptionMaintainDistance)
		{
			if (RadioWidgetActor->RadioState != ESoldierRadioState::ENone)
			{
				RadioWidgetActor->TryClose(true);
			}
		}
	}
	else
	{
		// 현재 위젯액터가 Dropbox 메뉴일 때
		if (Distance > MenuOptionMaintainDistance)
		{
			// RtWidgetActor_Menu 액터일 경우
			if (MenuOptionArray.Num() > 0)
			{
				gsc->HideMenuWidget(true);
			}
		}
	}
}

void ARtWidgetActor_Menu::ShowOptions()
{
	for (ARtWidgetActor_MenuOption* MenuOption : MenuOptionArray) {
		if (MenuOption == nullptr)
			continue;
		MenuOption->SetActorHiddenInGame(false);
		MenuOption->SetActorEnableCollision(true);
	}
}

void ARtWidgetActor_Menu::HideOptions()
{
	for (ARtWidgetActor_MenuOption* MenuOption : MenuOptionArray) {
		if (MenuOption == nullptr)
			continue;
		MenuOption->SetActorHiddenInGame(true);
		MenuOption->SetActorEnableCollision(false);
	}
}

void ARtWidgetActor_Menu::Init(int32 NewOptionsNum)
{
	Clear();

	OptionsNum = FMath::Clamp(NewOptionsNum, 2, 24);

	Theta = PI * 2 / OptionsNum;
	//Radius = FMath::Max(16.0f, (SphereRadius + 2.0f) / sin(Theta) * 2.0f);

}

void ARtWidgetActor_Menu::CreateMenuOption(int32 Index, FString EventName, FString EventID)
{
	ARtWidgetActor_MenuOption* MenuOption = GetWorld()->SpawnActor<ARtWidgetActor_MenuOption>(ARtWidgetActor_MenuOption::StaticClass());

	SetMenuOption(MenuOption, Index, EventName, EventID);

	MenuOption->AttachToComponent(RelativeRoot, FAttachmentTransformRules::KeepRelativeTransform);
	MenuOption->SetActorRelativeTransform(FTransform());

	SetRadiusPosition(MenuOption, Index);

}

void ARtWidgetActor_Menu::SetMenuOption(ARtWidgetActor_MenuOption* MenuOption, int32 Index, FString EventName, FString EventID, FSlateColor Color)
{
	USphereComponent* SphereCollision = MenuOption->SphereCollision;
	SphereCollision->SetSphereRadius(SphereRadius);

	MenuOption->AddHoverOption.BindLambda([this](ARtWidgetActor_MenuOption* option) {
		HoverOption = option;
		OptionOverlapTime = 0.0f;
		});
	MenuOption->RemoveHoverOption.BindLambda([this](ARtWidgetActor_MenuOption* option) {
		if (HoverOption == option)
			HoverOption = nullptr;
		option->Unhover();
		OptionOverlapTime = 0.0f;
		});

	MenuOptionArray.Add(MenuOption);
	MenuOption->EventName = EventName;
	MenuOption->EventID = EventID;

	FString widgetPath = RtTraineeWidgetBlueprint::TraineeMenuOption;
	TSubclassOf<UUserWidget> widgetClass = LoadClass<UUserWidget>(nullptr, *widgetPath);
	URt_Trainee_MenuOption* widget = CreateWidget<URt_Trainee_MenuOption>(GetWorld(), widgetClass);
	MenuOption->WidgetComponent->SetWidget(widget);

	widget->SetText(FText::FromString(*EventName));//Set Text
	widget->SetFontSize(MenuOption->DefaultFontSize);//Set Text
	widget->SetFontColor(Color);
}

void ARtWidgetActor_Menu::SetRadiusPosition(ARtWidgetActor_MenuOption* MenuOption, int32 Index)
{
	float theta = Theta * Index;
	if (OptionsNum == 2)
		theta -= PI / 2;
	MenuOption->SetActorRelativeLocation(FVector(0, Radius * sin(theta) * 1.5, Radius * cos(theta) * 1.5));

}
