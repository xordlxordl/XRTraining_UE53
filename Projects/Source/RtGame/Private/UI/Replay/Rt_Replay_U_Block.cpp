// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Replay/Rt_Replay_U_Block.h"
#include "Framework/RtPlayerController.h"
// UI
#include "UI/Replay/Rt_ReplayViewer.h"

void URt_Replay_U_Block::SetTraineeInfo(FString InName, FString InRole, int32 InUID)
{
	TraineeName->SetText(FText::FromString(InName));
	TraineeRole->SetText(FText::FromString(InRole));
	myUID = InUID;
}

void URt_Replay_U_Block::SetParentsWidget(URt_ReplayViewer* InParentsWidget)
{
	ParentsWidget = InParentsWidget;
}

void URt_Replay_U_Block::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UGameInstance* gi = GetGameInstance();
	if (gi) {
		RepManager = gi->GetSubsystem<URtReplayManager>();
	}

	Trainee_Btn->OnClicked.AddDynamic(this, &URt_Replay_U_Block::OnClicked_Btn);
}

void URt_Replay_U_Block::OnClicked_Btn()
{
	if (auto* pc = Get_PlayerController()) {
		pc->SetReplayTrainee(myUID);

		if (ParentsWidget) {
			ParentsWidget->ClearAllSelectTraineeList();

			SetSelected(true);

			ParentsWidget->OnClicked_Pause();
			ParentsWidget->ChangeWavFileByUID(myUID);
		}
	}
}

void URt_Replay_U_Block::SetSelected(bool selected)
{
	if (Border_Selected) {
		Border_Selected->BrushColor.A = (selected) ? 1.0f : 0.0f;
		Border_Selected->SynchronizeProperties();
	}
}

