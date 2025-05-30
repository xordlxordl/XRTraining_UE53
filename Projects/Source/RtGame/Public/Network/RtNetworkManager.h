// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
// Protocol
#include "RtCore/Public/Common/RtProtocol.h"
#include "RtNetworkManager.generated.h"


RTGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogNetworkManager, Log, All);

class URtConfig;
class URtTrainingManager;
class RtPacketSession;
class RtUDPPacketSession;

UCLASS()
class RTGAME_API URtNetworkManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;


	class FSocket* Socket;
	class FSocket* UDPSocket;
	
	FString IpAddress_TCP;
	int16 Port_TCP;
	FString IpAddress_UDP;
	int16 SendPort_UDP;      // From setting.csv
 	int16 RecvPort_UDP;      // From ConnectAck

	TSharedPtr<RtPacketSession> GameServerPacketSession;
	FTimerHandle ReceiveTimerHandle;
	FTimerHandle ReceiveTimerHandle_UDP;
	TSharedPtr<FInternetAddr> InternetAddr_UDP_Send;
	TSharedPtr<FInternetAddr> InternetAddr_UDP_Recv;
	TSharedPtr<RtUDPPacketSession> GameServerPacketSession_UDP;
	class ARtVoiceManager* VM;

	// Get
	URtTrainingManager* Get_TrainingManager();
	ARtVoiceManager* Get_VoiceManager() { return VM; }

protected:
	// dave
	void OnNetworkFailure(ENetworkFailure::Type FailureType);

public:
	UFUNCTION(BlueprintCallable)
	void Connect();

	void Start_UDP();

	UFUNCTION(BlueprintCallable)
	void Disconnect();

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();


	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets_UDP();

	void Parse(int32 protocol, TArray<uint8> packet);

	// Common Send Packet
	void SendPacket(stHeader* packetData);

	// Custom Send Packet
	void SendPacket_Login(FString playerObjectID, enRole Role);
	void SendPacket_SetScenario(FString SCTFileName);
	void SendPacket_Ready(int32 UID, bool IsReady);
	void SendPacket_TrainingStart(int32 TID);
	void SendPacket_TrainingEnd(int32 TID);
	void SendPacket_PutAccident(FString AccidentName);
	void SendPacket_SCTEventID(FString EventID);
	void SendPacket_InteractKey(int32 UID);
	void SendPacket_TraineeAim(int32 UID, FString ObjectID);
	void SendPacket_TraineeShot(FString weaponID, FString hitObjectID, FVector hitPosition, FVector shotPosition, FVector shotDirection, bool amm);
	void SendPacket_DayNight(bool isNight);
	void SendPacket_RestartSame();
	void SendPacket_RestartAll();
	void SendPacket_NpcState(struct stNpcState& npcState);

	//////////////////////////////////////////////////////////////////////////////
	///// UDP Send //////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	void SendPacket_UDP(const TArray<uint8>& Data, int CompressedSize);


	// Recv Packet 
	void RecvConnectAck(TArray<uint8> packet);
	void RecvLoginAck(TArray<uint8> packet);
	void RecvSetSCTAck(TArray<uint8> packet);
	void RecvReady(TArray<uint8> packet);
	void RecvStartAck(TArray<uint8> packet);
	void RecvFinish(TArray<uint8> packet);
	void RecvAccident(TArray<uint8> packet);
	void RecvSCTEventID(TArray<uint8> packet);
	void RecvInteractKey(TArray<uint8> packet);
	void RecvTraineeAim(TArray<uint8> packet);
	void RecvTraineeShot(TArray<uint8> packet);
	void RecvDayNight(TArray<uint8> packet);
	void RecvRestartSame(TArray<uint8> packet);
	void RecvRestartAll(TArray<uint8> packet);
	void RecvNpcState(TArray<uint8> packet);

	//////////////////////////////////////////////////////////////////////////////
    ///// UDP Recv //////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////

	void RecvVoiceBuffer(TArray<uint8> packet);
};