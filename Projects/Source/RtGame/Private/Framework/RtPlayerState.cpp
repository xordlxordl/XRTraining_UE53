// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RtPlayerState.h"
// Framework
#include "Framework/RtPlayerController.h"
#include "Framework/RtTraineePawn.h"

// Data
#include "Manager/RtConfig.h"

ARtPlayerState::FUpdated ARtPlayerState::Updated_Delegate;

void ARtPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(ARtPlayerState, Player_Custom_Name);
	DOREPLIFETIME(ARtPlayerState, Play_Type);
	DOREPLIFETIME(ARtPlayerState, Login_PlayerName);
	DOREPLIFETIME(ARtPlayerState, Login_PlayerId);
	DOREPLIFETIME(ARtPlayerState, Login_PlayerPassword);
	DOREPLIFETIME(ARtPlayerState, Login_DeviceName);
	DOREPLIFETIME(ARtPlayerState, MyPlayerConnectionType);
	DOREPLIFETIME(ARtPlayerState, IsVR);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ARtPlayerState, DeviceInfo, SharedParams);
	//DOREPLIFETIME_WITH_PARAMS_FAST(ARtPlayerState, TrngEvaluationRecord, SharedParams);
}


void ARtPlayerState::Set_PlayerInfo(const FRtDeviceInfo& InInfo)
{
	DeviceInfo = InInfo;
	OnChanged_DeviceInfo_Delegate.Broadcast();
}

void ARtPlayerState::Set_LoginInfo(const FString& InOptions, bool InServer)
{
	FString temp = UGameplayStatics::ParseOption(InOptions, RtString_Option::PlayerType);
	uint8 temp_interger = FCString::Atoi(*temp);

	if (InServer)
	{
		if (temp_interger == 5) {
			// replay 
			Play_Type = ERt_PlayType::E_Replay;
		}
		else
		{
			Play_Type = ERt_PlayType::E_Server;
		}

	}
	else {
		if (temp_interger >= (uint8)(ERt_PlayType::E_Single)) {
			Play_Type = ERt_PlayType::E_None;
		}
		else {
			Play_Type = (ERt_PlayType)temp_interger;
		}
	}
	Login_PlayerName = UGameplayStatics::ParseOption(InOptions, RtString_Option::PlayerName);
	Login_PlayerId = UGameplayStatics::ParseOption(InOptions, RtString_Option::PlayerId);
	Login_PlayerPassword = UGameplayStatics::ParseOption(InOptions, RtString_Option::PlayerPassword);
	Login_DeviceName = UGameplayStatics::ParseOption(InOptions, RtString_Option::DeviceName);
}



ARtPlayerState::ARtPlayerState(const FObjectInitializer& ObjectInitializer) : APlayerState(ObjectInitializer)
, MyPlayerConnectionType(ERtPlayerConnectionType::Player)
{
	NetUpdateFrequency = 100.0f;
}

void ARtPlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);
}

const FRtDeviceInfo& ARtPlayerState::Get_PlayerInfo()
{
	return DeviceInfo;
}

ARtPlayerController* ARtPlayerState::Get_PlayerController()
{
	return Cast< ARtPlayerController>(GetPlayerController());
}

void ARtPlayerState::Set_AddtionalInformation(const FRtUserInfo_PreLogin& InInfo)
{
	Play_Type = InInfo.PlayType;
}

void ARtPlayerState::Server_Change_ReadyState_Implementation(const FRtDeviceInfo& Info)
{
	Multicast_Change_ReadyState(Info);
}

void ARtPlayerState::Multicast_Change_ReadyState_Implementation(const FRtDeviceInfo& Info)
{
	DeviceInfo.CompleteMapLoad = Info.CompleteMapLoad;
	DeviceInfo.InStartingPoint = Info.InStartingPoint;
	DeviceInfo.CompleteCalibration = Info.CompleteCalibration;

	// 
	if (Play_Type == ERt_PlayType::E_Control || Play_Type == ERt_PlayType::E_Server)
		return;

	OnDeviceInfoChanged.Broadcast(PlayerId, Info);
}


void ARtPlayerState::CallOrRegister_DeviceInfoChanged(FSimpleMulticastDelegate::FDelegate Delegate)
{
	bool bAleardyBound = Delegate.GetUObject() != nullptr ?
		OnChanged_DeviceInfo_Delegate.IsBoundToObject(Delegate.GetUObject()) : false;
	if (!bAleardyBound) {
		OnChanged_DeviceInfo_Delegate.Add(Delegate);
	}
}


void ARtPlayerState::OnRep_DeviceInfoChanged(const FRtDeviceInfo& InInfo)
{
	if (OnChanged_DeviceInfo_Delegate.IsBound() == false) {

		if (auto* pc = Get_PlayerController())
			pc->CallBeforeBindingToPlayerState();

		if (auto* pawn = Cast<ARtTraineePawn>(GetPawn())) {
			pawn->Handle_Changed_DeviceInfo();
		}
	}
	else {
		OnChanged_DeviceInfo_Delegate.Broadcast();
	}
	Updated_Delegate.Broadcast(this);


	//TODO: 키 정보 - Mocopi 폐기로 필요 없어짐
	// 키 정보
	//float tall = DeviceInfo.PlayerInfo.Traineeheight;
	//
	//UE_LOG(LogTemp, Log, TEXT("Tall : %f"), tall);
	//
	//if (tall < 100.0f || tall > 300.0f)
	//	tall = 175.0f;
	//
	//if(GetPlayerController())
	//{
	//	if (ARtTraineePawn* traineePawn = Cast<ARtTraineePawn>(GetPlayerController()->GetPawn()))
	//	{
	//		traineePawn->SetTall(tall);
	//	}
	//}

}

void ARtPlayerState::OnRep_Login_DeviceName()
{
	Get_OnPlayerBasicInfoChangedDelegate().Broadcast(this);
}

void ARtPlayerState::OnRep_PlayerName()
{
	Super::OnRep_PlayerName();
}

void ARtPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
}