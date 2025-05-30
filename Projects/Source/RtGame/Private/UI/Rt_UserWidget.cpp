// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Rt_UserWidget.h"

// Framework
#include "Framework/RtPlayerController.h"
#include "Framework/RtGameState.h"
#include "Framework/RtGameHUD.h"
#include "Framework/RtGameMode.h"

// Manager
#include "Manager/RtGameManager.h"
#include "Manager/RtWebManager.h"
#include "Manager/RtTrainingManager.h"

// Network
#include "Network/RtNetworkManager.h"

URtGameManager* URt_UserWidget::GetGameManager()
{
	if (auto* gi = GetGameInstance())
		return gi->GetSubsystem<URtGameManager>();

	return nullptr;
}

URtNetworkManager* URt_UserWidget::GetNetworkManager()
{
	if (auto* gi = GetGameInstance())
		return gi->GetSubsystem<URtNetworkManager>();

	return nullptr;
}

URtWebManager* URt_UserWidget::GetRtWebManager()
{
	if (auto* gi = GetGameInstance())
		return gi->GetSubsystem<URtWebManager>();

	return nullptr;
}

URtTrainingManager* URt_UserWidget::GetTrainingManager()
{
	if (UGameInstance* gi = GetGameInstance()) {
		if (URtGameManager* gmgr = gi->GetSubsystem<URtGameManager>()) {
			if (URtTrainingManager* tmgr = gmgr->TrngManager)
				return tmgr;
		}
	}
	return nullptr;
}


ARtPlayerController* URt_UserWidget::Get_PlayerController()
{
	return Cast<ARtPlayerController>(GetOwningPlayer());
}

ARtGameState* URt_UserWidget::Get_GameState()
{
	return GetWorld() != NULL ? GetWorld()->GetGameState<ARtGameState>() : nullptr;
}

ARtGameMode* URt_UserWidget::Get_GameMode()
{
	UWorld* world = GetWorld();
	check(world);
	return Cast<ARtGameMode>(GetWorld()->GetAuthGameMode());
}

ARtGameHUD* URt_UserWidget::Get_HUD()
{
	if (auto* pc = Get_PlayerController()) {
		return pc->Get_HUD();
	}
	return nullptr;
}

bool URt_UserWidget::IsControl_PlayType()
{
	if (auto* pc = Get_PlayerController()) {
		return pc->Get_PlayType() == ERt_PlayType::E_Control;
	}
	return false;
}

bool URt_UserWidget::IsLocalPlayer()
{
	if (auto* pc = Get_PlayerController()) {
		if (pc->IsLocalController())
			return true;
	}
	return false;
}

bool URt_UserWidget::IsLocalConllerPlayType()
{
	if (auto* pc = Get_PlayerController()) {
		if (pc->IsLocalController())
			return pc->Get_PlayType() == ERt_PlayType::E_Control;
	}
	return false;
}

void URt_UserWidget::Begin_Play()
{
}

void URt_UserWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
}

TSharedRef<SWidget> URt_UserWidget::RebuildWidget() 
{
	return Super::RebuildWidget();
}