// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtWidgetActor_TraineeResult.h"
#include "Framework/RtPlayerController.h"
#include "Framework/RtGameState.h"
#include "Framework/RtPlayerState.h"
#include "Components/WidgetComponent.h"
#include "UI/Trainee/Rt_Trainee_Result.h"

// Data
#include "Common/RtWebCommon.h"

// Manager
#include "Manager/RtConfig.h"
#include "Manager/RtWebManager.h"
#include "Manager/RtLoadingManager.h"
#include "Manager/RtGameManager.h"

#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"

#include "Components/WidgetComponent.h"

ARtWidgetActor_TraineeResult::ARtWidgetActor_TraineeResult()
{
    PrimaryActorTick.bCanEverTick = false;

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ResultWidget"));
    RootComponent = WidgetComponent;

    WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
    WidgetComponent->SetDrawSize(FVector2D(500.f, 300.f));
    WidgetComponent->SetRelativeScale3D(FVector(0.1f));
    WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WidgetComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

    SetActorEnableCollision(false);
}

void ARtWidgetActor_TraineeResult::BeginPlay()
{
    Super::BeginPlay();
}

void ARtWidgetActor_TraineeResult::InitWidget()
{
    /*if (URt_Trainee_Result* ResultWidget = Cast<URt_Trainee_Result>(WidgetComponent->GetUserWidgetObject()))
    {
        FResponse_TrngSimpleResult result;
        ResultWidget->Update_ResultList(result);
    }*/

	int32 tid = -1;
	int32 uid = -1;

	if (auto* gmgr = GetGameInstance()->GetSubsystem<URtGameManager>())
	{
		if (ARtPlayerController* pc = gmgr->Get_LocalPlayerController()) {
			// Get TID
			if (ARtGameState* gs = pc->Get_GameState())
			{
				tid = gs->Training_Index;
			}
			// Get UID
			if (ARtPlayerState* ps = pc->Get_PlayerState())
			{
				FRtDeviceInfo device_info = ps->Get_PlayerInfo();
				uid = device_info.TraineeUID;
			}

			UE_LOG(LogTemp, Log, TEXT("TID UID TEST : TID = %d, UID = %d"), tid, uid);

			if (tid > 0 && uid > 0)
			{
				// Result Detail Get from Web
				URtWebManager* WebSubsys = GetGameInstance()->GetSubsystem<URtWebManager>();
				if (WebSubsys->IsInitialized())
				{
					WebSubsys->Handle_GetResult.AddUFunction(this, FName("UpdateWidget"));
					WebSubsys->GetResult(tid, uid);
				}

				// Loding Popup Open
				if (auto* gi = GetGameInstance()) {
					if (auto* lm = gi->GetSubsystem<URtLoadingManager>()) {
						lm->LoadLoadingPopup(pc, "GetResult");
					}
				}
			}
		}
	}
}

void ARtWidgetActor_TraineeResult::UpdateWidget(const FResponse_TrngSimpleResult& Result)
{
	// Loding Popup Close
	if (auto* gi = GetGameInstance()) {
		if (auto* lm = gi->GetSubsystem<URtLoadingManager>()) {
			lm->OnLevelLoaded_TypePopup();
		}
	}

    if (URt_Trainee_Result* ResultWidget = Cast<URt_Trainee_Result>(WidgetComponent->GetUserWidgetObject()))
    {
        ResultWidget->Update_ResultList(Result);
    }
}