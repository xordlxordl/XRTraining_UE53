// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/RtLevelScriptActor.h"
// UE
#include "EngineUtils.h" //TActorIterator
#include "Kismet/Gameplaystatics.h"
// Framework
#include "Framework/RtPlayerController.h"
#include "Framework/RtPlayerState.h"
// Manager
#include "Manager/RtGameManager.h"

// Data 
#include "Common/RtRpcCommon.h"

// Actor
#include "Level/RtTrainingReadyPoint.h"

// Network
#include "Network/RtNetworkManager.h"

void ARtLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
	auto* world = GetWorld();

	for (TActorIterator<ARtTrainingReadyPoint> It(world); It; ++It) {
		ARtTrainingReadyPoint* Actor = Cast<ARtTrainingReadyPoint>(*It);
		if (IsValid(Actor)) {
			StartingPointArray.Add(Actor);
		}
	}

	if (auto* gi = GetGameInstance())
	{
		NetworkSubsystem = gi->GetSubsystem<URtNetworkManager>();
		if(NetworkSubsystem)
		{
			NetworkSubsystem->Connect();
		}
	}
}

void ARtLevelScriptActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


TArray<TWeakObjectPtr<ARtTrainingReadyPoint>>& ARtLevelScriptActor::Get_StartingPointArray()
{
	return StartingPointArray;
}

ARtTrainingReadyPoint* ARtLevelScriptActor::Get_StartTargetPoint(int32 InIndex)
{
	if (auto* world = GetWorld()) {
		TArray<AActor*> OutActor;
		UGameplayStatics::GetAllActorsOfClass(world, ARtTrainingReadyPoint::StaticClass(), OutActor);
		for (auto* it : OutActor) {
			if (auto* temp = Cast<ARtTrainingReadyPoint>(it)) {
				if (temp->Get_Index() == InIndex)
					return temp;
			}
		}
	}
	return nullptr;
}

URtGameManager* ARtLevelScriptActor::GetGameManager()
{
	if (auto* gi = GetGameInstance())
		return gi->GetSubsystem<URtGameManager>();

	return nullptr;
}



void ARtLevelScriptActor::Hide_StartingPoint()
{
	for (auto& it : StartingPointArray) {
		if (it.IsValid()) {
			it.Get()->Set_IsValidSPoint(false);
		}
	}
}



void ARtLevelScriptActor::Init_PlayMode()
{
	FString start_mode;
	auto* mgr = GetGameManager();
	UWorld* World = GetWorld();
	check(mgr);
	check(World);
	bool bRun_StartMode = false;
	FString current_map_name = World->GetMapName();
	bool ck_gamemap = false;// mgr->IsGameMap(current_map_name);

	mgr->Start_PlayMode();
	return;

	switch (mgr->Get_Start_Play_Type())
	{
	case ERt_PlayType::E_Server:
	{
		if (ck_gamemap == false) {
			mgr->Start_Server();
			bRun_StartMode = true;
		}
	}
	break;
	case ERt_PlayType::E_Control:
	{
		if (ck_gamemap == false) {
			mgr->Start_ControlMode();
			bRun_StartMode = true;
		}
	}
	break;
	case ERt_PlayType::E_Trainee:
	{
		if (ck_gamemap == false) {
			FString device_name;
			FString device_id;
			bool ck_name = FParse::Value(FCommandLine::Get(), TEXT("DeviceName="), device_name);
			bool ck_id = FParse::Value(FCommandLine::Get(), TEXT("DeviceId="), device_id);
			mgr->Start_Client(device_name, device_id);
			bRun_StartMode = true;
		}
	}
	break;
	case ERt_PlayType::E_Replay:
	{
		if (ck_gamemap == false) {
			UE_LOG(LogGameManager, Log, TEXT("E_Replay"));
			FString replay_TID;
			FString replay_UID;
			FString replay_SCN;

			// 제거예정
			//bool ck_tid = FParse::Value(FCommandLine::Get(), TEXT("TID="), replay_TID);
			//bool ck_uid = FParse::Value(FCommandLine::Get(), TEXT("UID="), replay_UID);
			//bool ck_scn = FParse::Value(FCommandLine::Get(), TEXT("SNAME="), replay_SCN);

			FString low_commandLine = FCommandLine::Get();
			std::string de_commandLine_str = TCHAR_TO_UTF8(*low_commandLine);
			std::string commandLine_str = GetBase64Decording(de_commandLine_str);
			FString commandLine = UTF8_TO_TCHAR(commandLine_str.c_str());

			TArray<FString> result;
			commandLine.ParseIntoArray(result, TEXT(":"), true);

			TArray<FString> result2;
			result[1].ParseIntoArray(result2, TEXT("/"), true);

			replay_TID = result2[0];
			replay_UID = result2[1];
			replay_SCN = result2[2];

			mgr->Start_ReplayMode(replay_TID, replay_UID, replay_SCN);
			bRun_StartMode = true;
		}
	}
	break;
	case ERt_PlayType::E_Single:
	{
		if (ck_gamemap == false) {
			mgr->Start_SingleMode();
			bRun_StartMode = true;
		}
	}
	break;
	default:
		break;
	}
}


std::string ARtLevelScriptActor::GetBase64Decording(std::string encoded_string)
{
	const std::string base64_chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4] = { 0, }, char_array_3[3] = { 0, };

	std::string ret;

	while (in_len-- && (encoded_string[in_] != '=') && IsBase64(encoded_string[in_]))
	{
		char_array_4[i++] = encoded_string[in_];
		in_++;

		if (i == 4)
		{
			for (i = 0; i < 4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i]; i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 4; j++)
			char_array_4[j] = 0;

		for (j = 0; j < 4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++)
			ret += char_array_3[j];
	}

	return ret;
}

bool ARtLevelScriptActor::IsBase64(uint8 c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}


