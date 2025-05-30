// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtWidgetActor_Radio.h"

#include "Kismet/GameplayStatics.h"

#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"

#include "Common/RtBlueprintAssetPath.h"
#include "UI/Trainee/Rt_Trainee_MenuOption.h"
#include "UI/Trainee/Rt_Trainee_SoldierRadio.h"
#include "Manager/RtGameScenarioController.h"
#include "Models/RtScenarioModel.h"
#include "Framework/RtTraineePawn.h"

////////////////////
/// Radio Menu Option
////////////////////

ARtWidgetActor_RadioOption::ARtWidgetActor_RadioOption()
{

}

void ARtWidgetActor_RadioOption::PostExecute(ARtWidgetActor_Menu* Menu)
{
	if(ARtWidgetActor_Radio* Radio = Cast<ARtWidgetActor_Radio>(Menu))
		Radio->TryCommunication();
}

////////////////////
/// System Menu Option
////////////////////

ARtWidgetActor_SystemOption::ARtWidgetActor_SystemOption()
{

}

void ARtWidgetActor_SystemOption::Execute()
{
	Super::Unhover();

	if (ExecuteSystemOption.IsBound())
		ExecuteSystemOption.Execute();
}

////////////////////
/// Radio Menu
////////////////////

ARtWidgetActor_Radio::ARtWidgetActor_Radio()
{
	PrimaryActorTick.bCanEverTick = true;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RelativeRoot);
	WidgetComponent->SetDrawSize(FVector2D(1600.0f));
	WidgetComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	WidgetComponent->SetRelativeScale3D(FVector(0.05f));

	WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SystemRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SystemRoot"));
	SystemRoot->SetupAttachment(RelativeRoot);
}

void ARtWidgetActor_Radio::BeginPlay()
{
	Super::BeginPlay();

}

void ARtWidgetActor_Radio::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (RadioState)
	{
	case ESoldierRadioState::ETryOpen:
		CurTime += DeltaTime;
		if (CurTime >= MaxGrapTime) {
			CurTime = 0.0f;
			PrevRadioState = RadioState;
			RadioState = ESoldierRadioState::ESelect;
		}
		Update();
		break;
	case ESoldierRadioState::ETryClose:
		CurTime += DeltaTime;
		if (CurTime >= MaxGrapTime) {
			CurTime = 0.0f;
			PrevRadioState = RadioState;
			RadioState = ESoldierRadioState::ENone;
		}
		Update();
		break;
	default:
		break;
	}
}

void ARtWidgetActor_Radio::RadioInit()
{
	ARtGameScenarioController* gsc = ARtGameScenarioController::Get(GetWorld());
	ARtScenarioModel* scenarioModel = gsc->ScenarioModel.Get();
	FRtRadioDropboxCommand* menuSetCommand = &(scenarioModel->RadioDropboxCommand);

	if (!menuSetCommand->DropboxElemantList.IsEmpty()) {
		Init(12);

		//Set Menu Commands
		//data.EventIndex
		int32 index = 0;
		for (auto& data : menuSetCommand->DropboxElemantList) {
			CreateRadioOption(index++, data.EventCommandName, data.EventID);
		}
	}

	int8 LeftOptionNum = 0;
	int8 RightOptionNum = 0;
	SystemInit(LeftOptionNum, RightOptionNum);

	if (gsc->GetIsSingleMode()) {
		for (const FRtScenarioSuddenEvent& data : scenarioModel->SctData.SuddenEventList) {
			FString EventName = data.EventName.ToString();
			if (EventName.IsEmpty())
				EventName = data.EventNameTID;
			CreateSuddenOption(EventName, data.EventNameTID, LeftOptionNum, RightOptionNum, SuddenOptionColor);
		}
	}

	HideOptions();

}

void ARtWidgetActor_Radio::SystemInit(int8& LeftOptionNum, int8& RightOptionNum)
{
	ARtGameScenarioController* gsc = ARtGameScenarioController::Get(GetWorld());
	ARtTraineePawn* Trainee = Cast<ARtTraineePawn>(GetWorld()->GetFirstPlayerController()->GetPawn());

	int8 idx = gsc->ScenarioModel->RadioDropboxCommand.DropboxElemantList.Num();

	// 캘리브레이션
	if (ARtWidgetActor_SystemOption* SystemOption = CreateSystemOption(TEXT("Calibrate"), idx, RightOptionNum, SystemOptionColor, true)) {
		SystemOption->ExecuteSystemOption.BindLambda([Trainee]() {
			Trainee->NextCalibration();
			});
	}
	++idx;
	// 클론
	if (ARtWidgetActor_SystemOption* SystemOption = CreateSystemOption(TEXT("Clone"), idx, RightOptionNum, SystemOptionColor, true)) {
		SystemOption->ExecuteSystemOption.BindLambda([Trainee]() {
			Trainee->NextCloneMode();
			});
	}

	if (!gsc->GetIsSingleMode())
		return;
	++idx;
	// 종료	
	if (ARtWidgetActor_SystemOption* SystemOption = CreateSystemOption(TEXT("QuitTraining"), idx, RightOptionNum, SystemOptionColor)) {
		SystemOption->ExecuteSystemOption.BindLambda([SystemOption]() {
			UKismetSystemLibrary::QuitGame(SystemOption->GetWorld(), UGameplayStatics::GetPlayerController(SystemOption->GetWorld(), 0), EQuitPreference::Quit, true);
			});
	}
	++idx;
	// 훈련 시작, 초기화
	if (ARtWidgetActor_SystemOption* SystemOption = CreateSystemOption(TEXT("StartTraining"), idx, RightOptionNum, SystemOptionColor)) {
		SystemOption->ExecuteSystemOption.BindLambda([gsc]() {
			gsc->SetGamePlay(true);
			});
	}


}

void ARtWidgetActor_Radio::CreateRadioOption(int32 Index, FString EventName, FString EventID, FSlateColor Color)
{
	ARtWidgetActor_RadioOption* MenuOption = GetWorld()->SpawnActor<ARtWidgetActor_RadioOption>(ARtWidgetActor_RadioOption::StaticClass());

	MenuOption->AttachToComponent(RelativeRoot, FAttachmentTransformRules::KeepRelativeTransform);

	SetMenuOption(MenuOption, Index, EventName, EventID, Color);
	MenuOption->SetActorRelativeTransform(FTransform());

	SetRadiusPosition(MenuOption, Index);

}

ARtWidgetActor_SystemOption* ARtWidgetActor_Radio::CreateSystemOption(FString EventName, int8& LeftOptionNum, int8& RightOptionNum, FSlateColor Color, bool IsRight)
{
	ARtWidgetActor_SystemOption* MenuOption = GetWorld()->SpawnActor<ARtWidgetActor_SystemOption>(ARtWidgetActor_SystemOption::StaticClass());

	MenuOption->AttachToComponent(RelativeRoot, FAttachmentTransformRules::KeepRelativeTransform);

	UStringTable* TraineeStringTable = LoadObject<UStringTable>(nullptr, TRAINEE_STRINGTABLE_PATH);
	FString OptionName;

	if (TraineeStringTable)
		if(TraineeStringTable->GetMutableStringTable().Get().GetSourceString(FTextKey(EventName), OptionName) == false)
			OptionName = EventName;

	SetSystemOption(MenuOption, OptionName, Color);
	MenuOption->SetActorRelativeTransform(FTransform());

	SetRadiusPosition(MenuOption, LeftOptionNum);
	//SetBoxPosition(MenuOption, LeftOptionNum, RightOptionNum, IsRight);

	return MenuOption;
}

void ARtWidgetActor_Radio::CreateSuddenOption(FString EventName, FString EventID, int8& LeftOptionNum, int8& RightOptionNum, FSlateColor Color)
{
	ARtWidgetActor_MenuOption* MenuOption = GetWorld()->SpawnActor<ARtWidgetActor_MenuOption>(ARtWidgetActor_MenuOption::StaticClass());

	MenuOption->AttachToComponent(SystemRoot, FAttachmentTransformRules::KeepRelativeTransform);

	SetMenuOption(MenuOption, -1, EventName, EventID, Color);
	MenuOption->SetActorRelativeTransform(FTransform());

	SetBoxPosition(MenuOption, LeftOptionNum, RightOptionNum);

}

void ARtWidgetActor_Radio::SetSystemOption(ARtWidgetActor_SystemOption* MenuOption, FString EventName, FSlateColor Color)
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

	FString widgetPath = RtTraineeWidgetBlueprint::TraineeMenuOption;
	TSubclassOf<UUserWidget> widgetClass = LoadClass<UUserWidget>(nullptr, *widgetPath);
	URt_Trainee_MenuOption* widget = CreateWidget<URt_Trainee_MenuOption>(GetWorld(), widgetClass);
	MenuOption->WidgetComponent->SetWidget(widget);

	widget->SetText(FText::FromString(*EventName));//Set Text
	widget->SetFontSize(MenuOption->DefaultFontSize);//Set Text
	widget->SetFontColor(Color);

}

void ARtWidgetActor_Radio::SetBoxPosition(ARtWidgetActor_MenuOption* MenuOption, int8& LeftOptionNum, int8& RightOptionNum, bool IsRight)
{
	int8 index = IsRight ? RightOptionNum++ : LeftOptionNum++;
	float Height = SphereRadius * 3.0f;
	float HalfWith = SphereRadius * 3.f;

	int8 CurWithIndex = FMath::Floor(index / Column);

	float CurWith = HalfWith / 2.f + index * HalfWith;
	float CurHeight = -Height;

	MenuOption->SetActorRelativeLocation(FVector(0.0f, IsRight ? CurWith : -CurWith, CurHeight));
}

void ARtWidgetActor_Radio::TryOpen()
{
	PrevRadioState = RadioState;
	RadioState = ESoldierRadioState::ETryOpen;
	ARtTraineePawn* Trainee = Cast<ARtTraineePawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	FTransform MainWidgetTransform = Trainee->MainWidget->RelativeRoot->GetComponentTransform();
	//SystemRoot->SetWorldLocation(MainWidgetTransform.GetLocation());
	//SystemRoot->SetWorldRotation(MainWidgetTransform.GetRotation());

}

void ARtWidgetActor_Radio::TryClose(bool IsImmediate)
{
	PrevRadioState = RadioState;
	if (IsImmediate) {
		RadioState = ESoldierRadioState::ENone;
		CurTime = 0.0f;
		Update();
		return;
	}
	RadioState = ESoldierRadioState::ETryClose;

}

void ARtWidgetActor_Radio::Update()
{
	//Options
	switch (RadioState)
	{
	case ESoldierRadioState::ENone:
		HideOptions();
		break;
	case ESoldierRadioState::ESelect:
		ShowOptions();
		break;
	}

	//Widgets
	URt_Trainee_SoldierRadio* RadioWidget = Cast<URt_Trainee_SoldierRadio>(WidgetComponent->GetWidget());
	if (RadioWidget == nullptr)
		return;

	RadioWidget->Update(RadioState, FMath::Clamp(CurTime, 0.0f, MaxGrapTime) / MaxGrapTime);

}

void ARtWidgetActor_Radio::Cancle()
{
	RadioState = PrevRadioState;
	CurTime = 0.0f;
	Update();
}

//void ARtWidgetActor_Radio::ShowOptions()
//{
//	ARtWidgetActor_Menu::ShowOptions();
//	SystemRoot->SetVisibility(true, true);
//}
//
//void ARtWidgetActor_Radio::HideOptions()
//{
//	ARtWidgetActor_Menu::HideOptions();
//	SystemRoot->SetVisibility(false, true);
//}

void ARtWidgetActor_Radio::TryCommunication()
{
	bool isSuccess = FMath::RandRange(0, 1) > 0;
	if (isSuccess) {
		CommunicationSuccess();
	}
	else {
		CommunicationFail();
	}
}

void ARtWidgetActor_Radio::CommunicationSuccess()
{
	RadioState = ESoldierRadioState::ECommunicationSuccess;
	Update();
}

void ARtWidgetActor_Radio::CommunicationFail()
{
	RadioState = ESoldierRadioState::ECommunicationFail;
	Update();
}

