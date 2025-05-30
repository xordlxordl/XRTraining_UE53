// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Rt_ServerWidget.h"
// Framework
#include "Framework/RtGameMode.h"
#include "Framework/RtPlayerController.h"

bool URt_ServerWidget::Initialize()
{

	Http = &FHttpModule::Get();//HTTP

	bool res = Super::Initialize();

	//ARtGameMode::Player_Connected_Delegate.AddUObject(this, &URt_ServerWidget::Update_Player_List);
	return res;
}

void URt_ServerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void URt_ServerWidget::NativeConstruct()
{
}

void URt_ServerWidget::NativeDestruct()
{
}

void URt_ServerWidget::Update_Player_List()
{
}


void URt_ServerWidget::Update_DeviceInfo(APlayerController* InPlayerController, const TArray<FRtDeviceInfo>& InDeviceArray)
{
}

void URt_ServerWidget::OnGameModeMatchStateSet(FName MatchState)
{

}

void URt_ServerWidget::Handle_PlayerLogin(AGameModeBase* InGameMode, AController* InPlayer)
{
}
