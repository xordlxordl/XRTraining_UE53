// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtWalkieTalkieMenuActor.h"
#include "Manager/RtGameScenarioController.h"
#include "Network/RtNetworkManager.h"

#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

#include "Components/WidgetComponent.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"

// Sets default values
ARtWalkieTalkieMenuActor::ARtWalkieTalkieMenuActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // 위젯 컴포넌트 생성
    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
    RootComponent = WidgetComponent;

    // 3D 공간에 표시되도록 설정
    WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
    WidgetComponent->SetDrawSize(FVector2D(400, 300));
    WidgetComponent->SetPivot(FVector2D(0.5f, 0.5f));
    WidgetComponent->SetTwoSided(true);
    WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WidgetComponent->SetCastShadow(false);
}

// Called when the game starts or when spawned
void ARtWalkieTalkieMenuActor::BeginPlay()
{
    Super::BeginPlay();

    WidgetClass = WidgetComponent->GetWidgetClass();

    if (WidgetClass)
    {
        WidgetComponent->SetWidgetClass(WidgetClass);
    }

    // 애니메이션 로드
    OpenPopupMenuAnim = FindWidgetAnimationByName("OpenPopupMenu");
    ClosePopupMenuAnim = FindWidgetAnimationByName("ClosePopupMenu");
    MenuUpAnim = FindWidgetAnimationByName("MenuUp");
    MenuDownAnim = FindWidgetAnimationByName("MenuDown");
    TransmittingAnim = FindWidgetAnimationByName("Transmitting");
    TransmitEndAnim = FindWidgetAnimationByName("TransmitEnd");

    // Init
    UpdateEventIDList();
}

// Called every frame
void ARtWalkieTalkieMenuActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ARtWalkieTalkieMenuActor::UpdateEventIDList()
{
    EventIDList.Empty();

    ARtScenarioModel* scenarioModel = ARtGameScenarioController::Get(GetWorld())->ScenarioModel.Get();
    FRtRadioDropboxCommand* menuSetCommand = &(scenarioModel->RadioDropboxCommand);

    for (auto& data : menuSetCommand->DropboxElemantList)
    {
        if (data.bVisible)
        {
            EventIDList.Add(data.EventID);
        }
    }

    PrevSelectedMenuIdx = 0;
    CurSelectedMenuIdx = 0;
    EventIDNum = EventIDList.Num();

    ChangeMenuTexts();
    ChangeSelectedMenuText();

}

void ARtWalkieTalkieMenuActor::ChangeMenuTexts()
{
    if (!WidgetComponent) return;

    UUserWidget* Widget = Cast<UUserWidget>(WidgetComponent->GetWidget());
    if (!Widget) return;

    UTextBlock* MenuTextBlock1 = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("Text_PopupMenu_1")));
    UTextBlock* MenuTextBlock2 = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("Text_PopupMenu_2")));
    UTextBlock* MenuTextBlock3 = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("Text_PopupMenu_3")));
    UTextBlock* MenuTextBlock4 = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("Text_PopupMenu_4")));
    UTextBlock* MenuTextBlock5 = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("Text_PopupMenu_5")));

    if (!MenuTextBlock1 || !MenuTextBlock2 || !MenuTextBlock3 || !MenuTextBlock4 || !MenuTextBlock5)
        return;

    if (EventIDNum < 1)
    {
        MenuTextBlock1->SetText(FText::FromString(TEXT("")));
        MenuTextBlock2->SetText(FText::FromString(TEXT("")));
        MenuTextBlock3->SetText(FText::FromString(TEXT("")));
        MenuTextBlock4->SetText(FText::FromString(TEXT("")));
        MenuTextBlock5->SetText(FText::FromString(TEXT("")));
    }
    else
    {
        MenuTextBlock1->SetText(FText::FromString(EventIDList[WrapIndex(CurSelectedMenuIdx, -2, EventIDNum)]));
        MenuTextBlock2->SetText(FText::FromString(EventIDList[WrapIndex(CurSelectedMenuIdx, -1, EventIDNum)]));
        MenuTextBlock3->SetText(FText::FromString(EventIDList[CurSelectedMenuIdx]));
        MenuTextBlock4->SetText(FText::FromString(EventIDList[WrapIndex(CurSelectedMenuIdx, 1, EventIDNum)]));
        MenuTextBlock5->SetText(FText::FromString(EventIDList[WrapIndex(CurSelectedMenuIdx, 2, EventIDNum)]));
    }
}

void ARtWalkieTalkieMenuActor::ChangeSelectedMenuText()
{
    if (!WidgetComponent) return;

    UUserWidget* Widget = Cast<UUserWidget>(WidgetComponent->GetWidget());
    if (!Widget) return;

    if (UTextBlock* SelectedMenuText = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("Text_MenuName"))))
    {
        if (EventIDNum < 1)
        {
            SelectedMenuText->SetText(FText::FromString(TEXT("")));
        }
        else
        {
            SelectedMenuText->SetText(FText::FromString(EventIDList[PrevSelectedMenuIdx]));
        }
    }
}

bool ARtWalkieTalkieMenuActor::GetIsAnimationPlaying()
{
    return IsAnimationPlaying;
}

bool ARtWalkieTalkieMenuActor::GetIsPopupMenuOpened()
{
    return IsPopupMenuOpened;
}

void ARtWalkieTalkieMenuActor::SetIsPopupMenuOpened(bool InIsPopupMenuOpened)
{
    IsPopupMenuOpened = InIsPopupMenuOpened;
}

UWidgetAnimation* ARtWalkieTalkieMenuActor::FindWidgetAnimationByName(const FString& AnimName)
{
    UUserWidget* Widget = Cast<UUserWidget>(WidgetComponent->GetWidget());
    if (!Widget) return nullptr;

    UClass* WidgetUClass = Widget->GetClass();

    for (TFieldIterator<FObjectProperty> PropIt(WidgetUClass); PropIt; ++PropIt)
    {
        FObjectProperty* Property = *PropIt;
        if (Property->PropertyClass == UWidgetAnimation::StaticClass())
        {
            UObject* AnimationObj = Property->GetObjectPropertyValue_InContainer(Widget);
            if (AnimationObj && AnimationObj->GetName().Contains(AnimName))
            {
                return Cast<UWidgetAnimation>(AnimationObj);
            }
        }
    }

    return nullptr;
}

void ARtWalkieTalkieMenuActor::PressedOK()
{
    if (IsAnimationPlaying) return;

    ClosePopupMenu();

    PrevSelectedMenuIdx = CurSelectedMenuIdx;

    ChangeSelectedMenuText();
}

void ARtWalkieTalkieMenuActor::PressedBack()
{
    if (IsAnimationPlaying) return;

    ClosePopupMenu();

    CurSelectedMenuIdx = PrevSelectedMenuIdx;
}

void ARtWalkieTalkieMenuActor::PressedTransmitting()
{
    if (EventIDNum < 1) return;

    if (IsAnimationPlaying) return;

    if (WidgetComponent)
    {
        UUserWidget* Widget = Cast<UUserWidget>(WidgetComponent->GetWidget());

        if (Widget && TransmittingAnim)
        {
            AnimPlayer = Widget->PlayAnimation(
                TransmittingAnim,
                0.0f,
                0.0f,
                EUMGSequencePlayMode::Forward,
                1.0f,
                false
            );

            if (AnimPlayer)
            {
                AnimPlayer->SetNumLoopsToPlay(0);
            }

            IsAnimationPlaying = true;
        }
    }
}

void ARtWalkieTalkieMenuActor::ReleasedTransmitting()
{
    if (EventIDNum < 1) return;

    if (AnimPlayer && AnimPlayer->GetPlaybackStatus() == EMovieScenePlayerStatus::Playing)
    {
        AnimPlayer->Stop();
        AnimPlayer = nullptr;

        if (WidgetComponent)
        {
            UUserWidget* Widget = Cast<UUserWidget>(WidgetComponent->GetWidget());

            if (Widget && TransmitEndAnim)
            {
                Widget->PlayAnimation(TransmitEndAnim);

                FWidgetAnimationDynamicEvent EndDelegate;
                EndDelegate.BindDynamic(this, &ARtWalkieTalkieMenuActor::OnAnimationFinished);

                Widget->BindToAnimationFinished(TransmitEndAnim, EndDelegate);
            }
        }

        FString CurSelectedEventID = EventIDList[CurSelectedMenuIdx];

        if (CurSelectedEventID.IsEmpty()) return;

        if (ARtGameScenarioController* gsc = ARtGameScenarioController::Get(GetWorld())) {
            if (gsc->CurrentPlayType != ERt_PlayType::E_Single) {
                // 멀티 플레이
                if (gsc->bIsGamePlay == false) {
                    //HideMenuWidget();

                    return;
                }

                if (auto* nmgr = gsc->Get_NetManager()) {
                    nmgr->SendPacket_SCTEventID(CurSelectedEventID);
                }

            }
            else {
                // 싱글 플레이
                gsc->ExecuteServerEvent(CurSelectedEventID);
            }

            TPair<const FRtEventCommand*, FString> data = gsc->ScenarioModel->ServerEventCommandMap.FindRef(CurSelectedEventID);
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
}

void ARtWalkieTalkieMenuActor::OpenPopupMenu()
{
    if (EventIDNum < 1) return;

    if (IsAnimationPlaying) return;

    if (WidgetComponent)
    {
        UUserWidget* Widget = Cast<UUserWidget>(WidgetComponent->GetWidget());

        if (Widget && OpenPopupMenuAnim)
        {
            Widget->PlayAnimation(OpenPopupMenuAnim);
            IsAnimationPlaying = true;

            FWidgetAnimationDynamicEvent EndDelegate;
            EndDelegate.BindDynamic(this, &ARtWalkieTalkieMenuActor::OnAnimationFinished);

            Widget->BindToAnimationFinished(OpenPopupMenuAnim, EndDelegate);

            IsPopupMenuOpened = true;
        }
    }
}

void ARtWalkieTalkieMenuActor::ClosePopupMenu()
{
    if (WidgetComponent)
    {
        UUserWidget* Widget = Cast<UUserWidget>(WidgetComponent->GetWidget());

        if (Widget && ClosePopupMenuAnim)
        {
            Widget->PlayAnimation(ClosePopupMenuAnim);
            IsAnimationPlaying = true;

            FWidgetAnimationDynamicEvent EndDelegate;
            EndDelegate.BindDynamic(this, &ARtWalkieTalkieMenuActor::OnAnimationFinished);
            Widget->BindToAnimationFinished(ClosePopupMenuAnim, EndDelegate);

            FWidgetAnimationDynamicEvent EndDelegate2;
            EndDelegate2.BindDynamic(this, &ARtWalkieTalkieMenuActor::ChangeMenuTexts);
            Widget->BindToAnimationFinished(ClosePopupMenuAnim, EndDelegate2);

            IsPopupMenuOpened = false;
        }
    }
}

void ARtWalkieTalkieMenuActor::MoveMenuUp()
{
    if (IsAnimationPlaying) return;

    if (!WidgetComponent) return;

    UUserWidget* Widget = Cast<UUserWidget>(WidgetComponent->GetWidget());
    if (!Widget) return;

    if (!MenuUpAnim) return;

    /*ChangeMenuTexts();*/
    FWidgetAnimationDynamicEvent StartDelegate;
    StartDelegate.BindDynamic(this, &ARtWalkieTalkieMenuActor::ChangeMenuTexts);
    Widget->BindToAnimationStarted(MenuUpAnim, StartDelegate);

    Widget->PlayAnimation(MenuUpAnim);
    IsAnimationPlaying = true;

    FWidgetAnimationDynamicEvent EndDelegate;
    EndDelegate.BindDynamic(this, &ARtWalkieTalkieMenuActor::OnAnimationFinished);

    Widget->BindToAnimationFinished(MenuUpAnim, EndDelegate);

    CurSelectedMenuIdx = WrapIndex(CurSelectedMenuIdx, -1, EventIDNum);
}

void ARtWalkieTalkieMenuActor::MoveMenuDown()
{
    if (IsAnimationPlaying) return;

    if (!WidgetComponent) return;

    UUserWidget* Widget = Cast<UUserWidget>(WidgetComponent->GetWidget());
    if (!Widget) return;

    if (!MenuDownAnim) return;

    //ChangeMenuTexts();
    FWidgetAnimationDynamicEvent StartDelegate;
    StartDelegate.BindDynamic(this, &ARtWalkieTalkieMenuActor::ChangeMenuTexts);
    Widget->BindToAnimationStarted(MenuDownAnim, StartDelegate);

    Widget->PlayAnimation(MenuDownAnim);
    IsAnimationPlaying = true;

    FWidgetAnimationDynamicEvent EndDelegate;
    EndDelegate.BindDynamic(this, &ARtWalkieTalkieMenuActor::OnAnimationFinished);

    Widget->BindToAnimationFinished(MenuDownAnim, EndDelegate);

    CurSelectedMenuIdx = WrapIndex(CurSelectedMenuIdx, 1, EventIDNum);
}

int ARtWalkieTalkieMenuActor::WrapIndex(int baseIdx, int offset, int max)
{
    int NewIndex = baseIdx + offset;

    if (NewIndex < 0)
        NewIndex = max + NewIndex;

    if (NewIndex < 0)
        return 0;

    if (NewIndex >= max)
        NewIndex = NewIndex % max;

    return NewIndex;
}

void ARtWalkieTalkieMenuActor::OnAnimationFinished()
{
    IsAnimationPlaying = false;

    /*if (UUserWidget* Widget = Cast<UUserWidget>(WidgetComponent->GetWidget()))
    {
        if (UImage* Image = Cast<UImage>(Widget->GetWidgetFromName(TEXT("Image_SelectedMenu"))))
        {
            FWidgetTransform Transform;

            switch (CurSelectedMenu)
            {
            case EMenuType::SituationReport:
                Transform.Translation = FVector2D(0.f, -90.f);
                Image->SetRenderTransform(Transform);
                break;
            case EMenuType::FirePermission:
                Transform.Translation = FVector2D(0.f, 0.f);
                Image->SetRenderTransform(Transform);
                break;
            case EMenuType::RequestSupport:
                Transform.Translation = FVector2D(0.f, 90.f);
                Image->SetRenderTransform(Transform);
                break;
            }
        }
    }*/


}