// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/RtNetworkManager.h"
// Socket
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
// UE
#include "Serialization/ArrayWriter.h"
// Framework
#include "Framework/RtPlayerController.h"
#include "Framework/RtPlayerState.h"
// Manager 
#include "Manager/RtConfig.h"
#include "Manager/RtGameManager.h"
#include "Manager/RtTrainingManager.h"
#include "Manager/RtGameScenarioController.h"
#include "Manager/RtGameSceneManager.h"
#include "Manager/RtVoiceManager.h"
// Data 
#include "RtCore/Public/Common/RtCommon.h"
#include "RtCore/Public/Common/RtPacket.h"
#include "RtCore/Public/Common/RtProtocol.h"
// Network
#include "Network/RtPacketSession.h"
#include "Network/RtWorkerThread.h"

#include "RtBluePrintCommonUtil.h"
#include "RtDefines.h"

#include "Manager/RtReplayManagerActor.h"
#include "Manager/RtVoiceRecognitionManager.h"
#include "RtGameInstance.h"

DEFINE_LOG_CATEGORY(LogNetworkManager);


void URtNetworkManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// SET INI DATAS
	IpAddress_TCP = URtConfig::TCPSocketIP;
	Port_TCP = URtConfig::TCPPort;
	IpAddress_UDP = URtConfig::UDPSocketIP;
	SendPort_UDP = URtConfig::UDPPort;

	// dave
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		URtGameInstance* RtGameInstance = Cast<URtGameInstance>(GameInstance);
		if (RtGameInstance)
		{
			RtGameInstance->OnNetworkFailure_Delegate.AddUObject(this, &URtNetworkManager::OnNetworkFailure);
		}
	}
}

void URtNetworkManager::Deinitialize()
{
	Super::Deinitialize();
}

URtTrainingManager* URtNetworkManager::Get_TrainingManager()
{
	if (UGameInstance* gi = GetGameInstance()) {
		if (URtGameManager* gmgr = gi->GetSubsystem<URtGameManager>()) {
			if (URtTrainingManager* tmgr = gmgr->TrngManager)
				return tmgr;
		}
	}
	return nullptr;
}

void URtNetworkManager::Connect()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress_TCP, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port_TCP);

	UE_LOG(LogNetworkManager, Log, TEXT("Connecting To Server"));

	bool Connected = Socket->Connect(*InternetAddr);

	if (Connected)
	{
		UE_LOG(LogNetworkManager, Log, TEXT("Connection Success"));

		// Session
		GetWorld()->GetTimerManager().SetTimer(ReceiveTimerHandle, this, &URtNetworkManager::HandleRecvPackets, 0.01f, true);
		GameServerPacketSession = MakeShared<RtPacketSession>(Socket);
		GameServerPacketSession->RunThread();
	}
	else
	{
		UE_LOG(LogNetworkManager, Log, TEXT("Connection Failed"));
	}
}

// dave
void URtNetworkManager::Disconnect()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(ReceiveTimerHandle);
	TimerManager.ClearTimer(ReceiveTimerHandle_UDP);

	// udp session
	if (GameServerPacketSession_UDP)
	{
		GameServerPacketSession_UDP->DisConnect();
		GameServerPacketSession_UDP.Reset();
	}

	// udp socket
	if (UDPSocket)
	{
		UDPSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(UDPSocket);
		UDPSocket = nullptr;
	}

	// tcp session
	if (GameServerPacketSession)
	{
		GameServerPacketSession->DisConnect();
		GameServerPacketSession.Reset();
	}

	// ip addr
	InternetAddr_UDP_Send.Reset();
	InternetAddr_UDP_Recv.Reset();

	// socket
	if (Socket)
	{
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
		Socket = nullptr;
	}
}

void URtNetworkManager::Start_UDP()
{
	UDPSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("DGram"), TEXT("Client UDP Socket"));

	// Set Port from TCP for Recv
	FIPv4Address Addr;
	FIPv4Address::Parse(IpAddress_UDP, Addr);

	InternetAddr_UDP_Recv = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr_UDP_Recv->SetIp(Addr.Value);
	InternetAddr_UDP_Recv->SetPort(RecvPort_UDP);

	// Set Port from Setting for Send
	FIPv4Address Addr_s;
	FIPv4Address::Parse(IpAddress_UDP, Addr_s);

	if (auto* gmgr = GetGameInstance()->GetSubsystem<URtGameManager>()) {
		ARtPlayerController* pc = gmgr->Get_LocalPlayerController();
		// 클라 Player Controller에 접근

		FRtDeviceInfo outInfo;
		pc->Get_DeviceInfo(outInfo);

		SendPort_UDP += outInfo.DeviceIndex;
	}

	InternetAddr_UDP_Send = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr_UDP_Send->SetIp(Addr.Value);
	InternetAddr_UDP_Send->SetPort(SendPort_UDP);


	if (UDPSocket) {

		GetWorld()->GetTimerManager().SetTimer(ReceiveTimerHandle_UDP, this, &URtNetworkManager::HandleRecvPackets_UDP, 0.04f, true);
		GameServerPacketSession_UDP = MakeShared<RtUDPPacketSession>(UDPSocket, InternetAddr_UDP_Recv, InternetAddr_UDP_Send);
		GameServerPacketSession_UDP->RunThread();

		// for hole puching
		TArray<uint8> holePacket;
		holePacket.AddZeroed(4);
		SendPacket_UDP(holePacket, sizeof(holePacket));

		UE_LOG(LogNetworkManager, Log, TEXT("Start GameSession_UDP"));
	}
	else {
		UE_LOG(LogNetworkManager, Log, TEXT("Failed to Create UDP Socket"));
	}

}


void URtNetworkManager::HandleRecvPackets()
{
	if (Socket == nullptr || GameServerPacketSession == nullptr)
		return;

	GameServerPacketSession->HandleRecvPackets();
}

void URtNetworkManager::HandleRecvPackets_UDP()
{
	if (UDPSocket == nullptr || GameServerPacketSession_UDP == nullptr)
		return;

	GameServerPacketSession_UDP->HandleRecvPackets();
}

void URtNetworkManager::Parse(int32 protocol, TArray<uint8> packet)
{
#if WITH_EDITOR
	APP_LOG("URtNetworkManager::Parse %d", protocol);
#endif
	QUICK_SCOPE_CYCLE_COUNTER(STAT_Network_Parse);

	switch (protocol)
	{
	case prConnectAck:	RecvConnectAck(packet);	 break;
	case prLoginAck: RecvLoginAck(packet);       break;
	case prSetSCTAck: RecvSetSCTAck(packet);     break;
	case prReady: RecvReady(packet);             break;
	case prStartAck: RecvStartAck(packet);       break;
	case prFinish: RecvFinish(packet);           break;
	case prAccident: RecvAccident(packet);       break;
	case prSctEvent: RecvSCTEventID(packet);     break;
	case prInteractKey: RecvInteractKey(packet); break;
	case prTraineeAim: RecvTraineeAim(packet);   break;
	case prTraineeShot: RecvTraineeShot(packet); break;
	case prDayNight: RecvDayNight(packet);       break;
	case prRestartSame: RecvRestartSame(packet); break;
	case prRestartAll: RecvRestartAll(packet);   break;
	case prNpcState: RecvNpcState(packet);       break;
	}
}

void URtNetworkManager::SendPacket(stHeader* packetData)
{
	if (packetData)
	{
		int32 packetSize = packetData->nSize;

#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
		SendBufferRef sendBuffer = MakeShared<SendBuffer>(packetSize);
#else
		SendBufferRef sendBuffer = nake_shared<SendBuffer>(packetSize);
#endif
		packetData->nSize = packetSize;
		sendBuffer->CopyData(packetData, packetSize);
		sendBuffer->Close(packetSize);
	
		if (GameServerPacketSession) {
			GameServerPacketSession->SendPacketQueue.Enqueue(sendBuffer);
		}
	}
}


void URtNetworkManager::SendPacket_Login(FString playerObjectID, enRole Role)
{
	stLoginReq packetStruct;
	packetStruct.grade = Role;
	if (Role == enRole::ROLE_USER)
	{
		char* CharArray = TCHAR_TO_ANSI(*playerObjectID);
		memcpy(packetStruct.objID, CharArray, playerObjectID.Len());
	}

	SendPacket(&packetStruct);
}

void URtNetworkManager::SendPacket_SetScenario(FString SCTFileName)
{
	stSetSCTReq packetStruct;

	wchar_t* CharArray = TCHAR_TO_WCHAR(*SCTFileName);
	memcpy(packetStruct.filename, CharArray, SCTFileName.Len() * 2);

	SendPacket(&packetStruct);
}

void URtNetworkManager::SendPacket_Ready(int32 UID, bool IsReady)
{
	stReady packetStruct;
	packetStruct.UID = UID;
	packetStruct.IsReady = IsReady;

	SendPacket(&packetStruct);

}

void URtNetworkManager::SendPacket_TrainingStart(int32 TID)
{
	stStartReq packetStruct;
	packetStruct.TID = TID;

	SendPacket(&packetStruct);
}

void URtNetworkManager::SendPacket_TrainingEnd(int32 TID)
{
	stFinish packetStruct;
	packetStruct.TID = TID;

	SendPacket(&packetStruct);
}

void URtNetworkManager::SendPacket_PutAccident(FString AccidentName)
{
	if (AccidentName.Len() > 64)
		return;

	wchar_t* charArray = TCHAR_TO_WCHAR(*AccidentName);

	stAccident packetStruct;
	FMemory::Memcpy(packetStruct.AccidentName, charArray, AccidentName.Len() * 2);

	SendPacket(&packetStruct);
}

void URtNetworkManager::SendPacket_SCTEventID(FString EventID)
{
	if (EventID.Len() > 64)
		return;

	const wchar_t* charArray = TCHAR_TO_WCHAR(*EventID);

	stSctEvent packetStruct;
	FMemory::Memcpy(packetStruct.SctEventID, charArray, EventID.Len() * 2);

	SendPacket(&packetStruct);
}

void URtNetworkManager::SendPacket_InteractKey(int32 UID)
{
	stInteractKey packetStruct;
	packetStruct.PlayerID = UID;

	SendPacket(&packetStruct);
}

void URtNetworkManager::SendPacket_TraineeAim(int32 UID, FString ObjectID)
{
	if (ObjectID.Len() > 64)
		return;

	const wchar_t* charArray = TCHAR_TO_WCHAR(*ObjectID);

	stTraineeAim packetStruct;
	packetStruct.PlayerID = UID;
	FMemory::Memcpy(packetStruct.ObjectID, charArray, ObjectID.Len() * 2);

	SendPacket(&packetStruct);
}

void URtNetworkManager::SendPacket_TraineeShot(FString weaponID, FString hitObjectID, FVector hitPosition, FVector shotPosition, FVector shotDirection, bool amm)
{
	if (weaponID.Len() > 64 || hitObjectID.Len() > 64)
		return;

	const wchar_t* charArrayWID = TCHAR_TO_WCHAR(*weaponID);
	const wchar_t* charArrayHOID = TCHAR_TO_WCHAR(*hitObjectID);

	stTraineeShot packetStruct;
	FMemory::Memcpy(packetStruct.WID, charArrayWID, weaponID.Len() * 2);
	FMemory::Memcpy(packetStruct.HOID, charArrayHOID, hitObjectID.Len() * 2);
	packetStruct.HPos = hitPosition;
	packetStruct.SPos = shotPosition;
	packetStruct.SDir= shotDirection;
	packetStruct.Amm = amm; 
//	packetStruct.Amm = true; // 일단 공포탄 기능은 제거함.

	SendPacket(&packetStruct);

#if WITH_EDITOR
		FILE_APP_LOG(
			"server_debug.log",
			"URtNetworkManager::SendPacket_TraineeShot() - hitObject : %s",
			*hitObjectID
		);
#endif 
}

void URtNetworkManager::SendPacket_DayNight(bool isNight)
{
	stDayNight packetStruct;
	packetStruct.IsNight = isNight;

	SendPacket(&packetStruct);
}

void URtNetworkManager::SendPacket_RestartSame()
{
	stRestartSame packetStruct;

	SendPacket(&packetStruct);
}

void URtNetworkManager::SendPacket_RestartAll()
{
	stRestartAll packetStruct;

	SendPacket(&packetStruct);
}

void URtNetworkManager::SendPacket_NpcState(stNpcState& npcState)
{
	//SendPacket(&npcState);
}

// UDP 
void URtNetworkManager::SendPacket_UDP(const TArray<uint8>& Data, int CompressedSize)
{
	if (UDPSocket == nullptr) {
		UE_LOG(LogNetworkManager, Log, TEXT("InValied Socket"));
		return;
	}

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Data, CompressedSize]()
	{
		// 헤더 
		stVoiceHeader Header;
		int totalsize;
		totalsize = VHEADSIZE + Data.Num();
		// 송신 버퍼
		TArray<uint8> totalPacket;
		totalPacket.SetNumUninitialized(totalsize);

		Header.SetHeader(CompressedSize, totalsize);
		memcpy(totalPacket.GetData(), &Header, VHEADSIZE);                     
		memcpy(totalPacket.GetData() + VHEADSIZE, Data.GetData(), Data.Num());

#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
		TSharedPtr<class SendBuffer_UDP> sendBuffer = MakeShared<SendBuffer_UDP>(totalsize);
#else
		SendBufferRef sendBuffer = nake_shared<SendBuffer_UDP>(totalsize);
#endif
		sendBuffer->CopyData(totalPacket.GetData(), totalsize);
		sendBuffer->Close(totalsize);

		if (GameServerPacketSession_UDP) {
			GameServerPacketSession_UDP->SendPacketQueue_U.Enqueue(sendBuffer);
		}

	});
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// RecvPacket

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void URtNetworkManager::RecvConnectAck(TArray<uint8> packet)
{
	// if UsingVoice value is false, don't work udp client
	if (!URtConfig::UsingVoice) return;
	if (!URtConfig::VoiceTalk) return;   // dave

	stConnectAck ConnectAck;
	memcpy(&ConnectAck, packet.GetData(), sizeof(stConnectAck));
    
	RecvPort_UDP = ConnectAck.UDPPort;

	if (auto* rtmgr = GetGameInstance()->GetSubsystem<URtGameManager>())
	{
		ERt_PlayType myType = rtmgr->Get_Play_Type();

		if (myType == ERt_PlayType::E_Trainee)
		{
			// UDP Start, only Client
			Start_UDP();
		}
	}
}

void URtNetworkManager::RecvLoginAck(TArray<uint8> packet)
{
	stLoginAck LoginAck;
	memcpy(&LoginAck, packet.GetData(), sizeof(LoginAck));

	switch (LoginAck.nResult)
	{
	case enErrorCode::LOGIN_SUCCESS : 
		UE_LOG(LogNetworkManager, Log, TEXT("Login Success !!")) break;
	case enErrorCode::FAIL : 
		UE_LOG(LogNetworkManager, Log, TEXT("Login Fail !!"))break;
	case enErrorCode::ERROR_DUP_ACCOUNT :
		UE_LOG(LogNetworkManager, Log, TEXT("Duplicate Account."))break;
	case enErrorCode::ERROR_ACCOUNT:
		UE_LOG(LogNetworkManager, Log, TEXT("No Account in Server."))break;
	case enErrorCode::ERROR_PASSWORD:
		UE_LOG(LogNetworkManager, Log, TEXT("Wrong Password."))break;
	default:
		break;
	}
}

// dave - debug
void URtNetworkManager::RecvSetSCTAck(TArray<uint8> packet)
{
	stSetSCTAck ack;

	memcpy(&ack, packet.GetData(), sizeof(ack));
	FString filename = (TCHAR*)ack.filename;

	UE_LOG(LogTemp, Log, TEXT("RecvSetSCTAck - Filename: %s"), *filename);

	if (auto* tmgr = Get_TrainingManager()) 
	{
		UE_LOG(LogTemp, Log, TEXT("TrainingManager found, current state: %d"), (int32)tmgr->state_ctrl);
		tmgr->SetControlFlow(EControlFlow::CONTROL_INSTALLWALL);
		UE_LOG(LogTemp, Log, TEXT("SetControlFlow called, new state: %d"), (int32)tmgr->state_ctrl);
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("TrainingManager not found!"));
	}
}

void URtNetworkManager::RecvReady(TArray<uint8> packet)
{
	stReady ack;
	memcpy(&ack, packet.GetData(), sizeof(ack));

	if (auto* rtmgr = GetGameInstance()->GetSubsystem<URtGameManager>())
	{
		ERt_PlayType myType = rtmgr->Get_Play_Type();
		if (myType == ERt_PlayType::E_Control)
		{
			int32 UID = ack.UID;	
			bool IsReady = ack.IsReady;
			ARtPlayerController* pc = rtmgr->Get_LocalPlayerController();
			pc->Client_Ready_Delegate.Broadcast(UID, IsReady);
		}
	}
}

void URtNetworkManager::RecvStartAck(TArray<uint8> packet)
{
	stStartAck ack;
	memcpy(&ack, packet.GetData(), sizeof(ack));

	if (auto* rtmgr = GetGameInstance()->GetSubsystem<URtGameManager>())
	{
		ERt_PlayType myType = rtmgr->Get_Play_Type();
		if (auto* tmgr = Get_TrainingManager()) 
		{
			// TID InValid
			if (ack.TID == 0) {
				UE_LOG(LogTemp, Warning, TEXT(" TID is not Invalid. "));
				return;
			}
			// SetTID
			tmgr->SetTID(ack.TID);

			if (myType == ERt_PlayType::E_Control)
			{
				UE_LOG(LogTemp, Warning, TEXT(" Control : Training Start !! "));
				tmgr->SetControlFlow(EControlFlow::CONTROL_PROCESSINGTRAINING);

				ARtGameScenarioController::Get(GetWorld())->SetGamePlay(true);
			}
			else if (myType == ERt_PlayType::E_Trainee)
			{
				UE_LOG(LogTemp, Warning, TEXT(" Trainee : Training Start !! "));
				if (auto* gmgr = GetGameInstance()->GetSubsystem<URtGameManager>()) {
					ARtPlayerController* pc = gmgr->Get_LocalPlayerController();
					pc->StartTraining(ack.TID);
				}

				// Find Voice Manager
				AActor* FoundActor;
				FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ARtVoiceManager::StaticClass());
				VM = Cast<ARtVoiceManager>(FoundActor);
				if (VM) {
					if (URtConfig::UsingVoice)
					{
						VM->Init();
						VM->SetNetManager(this);
						VM->StartCapture(); // Voice Capture
					}
				}
			}
			else if (myType == ERt_PlayType::E_Server)
			{
				UE_LOG(LogTemp, Warning, TEXT(" Server : Training Start !! "));
				tmgr->SetControlFlow(EControlFlow::CONTROL_PROCESSINGTRAINING);

				ARtGameScenarioController::Get(GetWorld())->SetGamePlay(true);
			}
		}
	}
}

void URtNetworkManager::RecvFinish(TArray<uint8> packet)
{
	stFinish ack;
	memcpy(&ack, packet.GetData(), sizeof(ack));

	ENetMode currentNetMode = GetWorld()->GetNetMode();

	if (auto* rtmgr = GetGameInstance()->GetSubsystem<URtGameManager>())
	{
		ERt_PlayType myType = rtmgr->Get_Play_Type();
		if (auto* tmgr = Get_TrainingManager())
		{
			// TID InValid
			if (ack.TID == 0) {
				UE_LOG(LogTemp, Warning, TEXT(" TID is not Invalid. "));
				return;
			}
			// SetTID
			tmgr->SetTID(ack.TID);

			if (myType == ERt_PlayType::E_Control) {
				UE_LOG(LogTemp, Warning, TEXT(" Control : Training Finish !! "));
				tmgr->SetControlFlow(EControlFlow::CONTROL_RESULT);

				ARtReplayManagerActor::Get(GetWorld())->WriteRecordDataFile();
				
				ARtGameScenarioController::Get(GetWorld())->SetGamePlay(false);
			}
			else if (myType == ERt_PlayType::E_Trainee)
			{
				UE_LOG(LogTemp, Warning, TEXT(" Trainee : Training Finish !! "));
				if (auto* gmgr = GetGameInstance()->GetSubsystem<URtGameManager>()) {
					ARtPlayerController* pc = gmgr->Get_LocalPlayerController();
					// 클라 Player Controller에 접근
					ARtGameScenarioController::Get(GetWorld())->SetGamePlay(false);

					// 임시 State 변경
					ARtGameScenarioController::Get(GetWorld())->ScenarioModel.Get()->GameState = ERtSctGameState::EGameState_End;
					
					FRtDeviceInfo outInfo;
					pc->Get_DeviceInfo(outInfo);

					if (VM) {
						if (URtConfig::UsingVoice)
						{
							VM->ExportCapture(ack.TID, outInfo.DeviceIndex, outInfo.Get_String());
						}
					}
				}
			}
			else if (myType == ERt_PlayType::E_Server)
			{
				UE_LOG(LogTemp, Warning, TEXT(" Server : Training Finish !! "));
				ARtGameScenarioController::Get(GetWorld())->SetGamePlay(false);
			}
		}
	}
}

void URtNetworkManager::RecvAccident(TArray<uint8> packet)
{
	stAccident ack;
	memcpy(&ack, packet.GetData(), sizeof(ack));
	FString AccidentName_str = UTF8_TO_TCHAR(ack.AccidentName);

}

void URtNetworkManager::RecvSCTEventID(TArray<uint8> packet)
{
	stSctEvent ack;
	memcpy(&ack, packet.GetData(), sizeof(stSctEvent));
	FString sctEventID = (TCHAR*)ack.SctEventID;

	ARtGameScenarioController::Get(GetWorld())->ExecuteServerEvent(sctEventID);

}

void URtNetworkManager::RecvInteractKey(TArray<uint8> packet)
{
	stInteractKey ack;
	memcpy(&ack, packet.GetData(), sizeof(stInteractKey));

	ARtGameScenarioController::Get(GetWorld())->AddInteractKeyList(ack.PlayerID);
}

void URtNetworkManager::RecvTraineeAim(TArray<uint8> packet)
{
	stTraineeAim ack;
	memcpy(&ack, packet.GetData(), sizeof(ack));
	FString objectID = (TCHAR*)ack.ObjectID;

	ARtGameScenarioController::Get(GetWorld())->AddAimMap(ack.PlayerID, objectID);
}

void URtNetworkManager::RecvTraineeShot(TArray<uint8> packet)
{
	stTraineeShot ack;
	memcpy(&ack, packet.GetData(), sizeof(ack));
	FString weaponID = (TCHAR*)ack.WID;
	FString objectID = (TCHAR*)ack.HOID;

	ARtGameScenarioController::Get(GetWorld())->AddShotList(weaponID, objectID, ack.HPos, ack.SPos, ack.SDir, ack.Amm);

#if WITH_EDITOR
	FILE_APP_LOG(
		"server_debug.log",
		"URtNetworkManager::RecvTraineeShot() - weapon : %s, hitObject : %s",
		*weaponID, *objectID
	);
#endif

}

void URtNetworkManager::RecvDayNight(TArray<uint8> packet)
{
	stDayNight ack;
	memcpy(&ack, packet.GetData(), sizeof(ack));
	if(ARtGameSceneManager* gameSceneManager = ARtGameSceneManager::Get(GetWorld())){
		gameSceneManager->SetDayNight(!ack.IsNight);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT(" SceneManager is Not Found! "));
	}
}

void URtNetworkManager::RecvRestartSame(TArray<uint8> packet)
{
	if (auto* gmgr = GetGameInstance()->GetSubsystem<URtGameManager>()) {
		ARtGameScenarioController::Get(GetWorld())->SetGamePlay(false);
		ARtGameScenarioController::Get(GetWorld())->InitScenario();

		ERt_PlayType myType = gmgr->Get_Play_Type();
		if (myType == ERt_PlayType::E_Control) {
			if (auto* tmgr = Get_TrainingManager()) {
				tmgr->SetControlFlow(EControlFlow::CONTROL_WAITINGTRAINING);
        	}
		}
		else if (myType == ERt_PlayType::E_Trainee) {
			// dave - update ready state
			UE_LOG(LogTemp, Warning, TEXT(" Trainee : Restart !! "));
			ARtPlayerController* pc = gmgr->Get_LocalPlayerController();
			pc->Exx_Send_Packet_Ready();

			if (VM) {
				if (URtConfig::UsingVoice)
				{
					VM->RestartCapture();
				}
			}
		}
	}
}


// dave - reset training
void URtNetworkManager::RecvRestartAll(TArray<uint8> packet)
{
	if (auto* gameManager = GetGameInstance()->GetSubsystem<URtGameManager>()) 
	{
		if (ARtGameScenarioController* scenarioConrtoller = ARtGameScenarioController::Get(GetWorld())) 
		{
			scenarioConrtoller->SetGamePlay(false);
			scenarioConrtoller->UnloadSct();
			scenarioConrtoller->Init(ERt_PlayType::E_None, TEXT(""));
		}

		if (ARtGameSceneManager* gameSceneManager = ARtGameSceneManager::Get(GetWorld()))
		{
			gameSceneManager->ClearReplay();
			gameSceneManager->DestroyAllSpawnedObjects();

			ERt_PlayType myType = gameManager->Get_Play_Type();
			ARtPlayerController* pc = gameManager->Get_LocalPlayerController();

			if (auto* trainingManager = Get_TrainingManager())
			{
				if (myType == ERt_PlayType::E_Control)
				{
					UE_LOG(LogTemp, Warning, TEXT(" Control : Return to start !! "));
					trainingManager->SetControlFlow(EControlFlow::CONTROL_SETSCT);

					check(pc);
					pc->Server_RequestResetTraining();
				}
				else if (myType == ERt_PlayType::E_Trainee)
				{
					UE_LOG(LogTemp, Warning, TEXT(" Trainee : Return to start !! "));
				}
				else if (myType == ERt_PlayType::E_Server)
				{
					UE_LOG(LogTemp, Warning, TEXT(" Server : Return to start !! "));
					trainingManager->SetControlFlow(EControlFlow::CONTROL_SETSCT);
				}
			}
		}
	}
}

void URtNetworkManager::RecvNpcState(TArray<uint8> packet)
{
	ENetMode currentNetMode = GetWorld()->GetNetMode();
	
#if WITH_EDITOR
	FILE_APP_LOG(
		"shipping_debug.log",
		"URtNetworkManager::RecvNpcState() - currentNetMode : %d", (int32)currentNetMode
	);
#endif

	if (currentNetMode != ENetMode::NM_Client) {
		return;
	}

	stNpcState ack;
	memcpy(&ack, packet.GetData(), sizeof(ack));
	ARtGameScenarioController::Get(GetWorld())->UpdateNpcState(ack);
}

//void URtNetworkManager::RecvVoiceBuffer(TArray<uint8> packet)
//{
//	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, packet]()
//	{
//		stVoiceHeader Header;
//		memcpy(&Header, packet.GetData(), VHEADSIZE);
//
//		int CompressedSize = Header.nCompressedSize;
//		int TotalPacketSize = Header.nTotalPacketSize;
//		int PayLoadSize = TotalPacketSize - VHEADSIZE;
//
//		if (TotalPacketSize == packet.Num()) // 홀펀칭 거르고 이상한 패킷 거름
//		{
//			TArray<uint8> VoiceData;
//			VoiceData.SetNumUninitialized(PayLoadSize);
//			memcpy(VoiceData.GetData(), packet.GetData() + VHEADSIZE, PayLoadSize);
//
//			// 메인 스레드에서 데이터 처리
//			AsyncTask(ENamedThreads::GameThread, [this, VoiceData, CompressedSize]()
//			{
//				if (VM && VM->IsValidLowLevel()) {
//					VM->DecodedAndRun(VoiceData, CompressedSize);
//					UE_LOG(LogTemp, Warning, TEXT(" Recieved Buffer - Size %d"), VoiceData.Num());
//				}
//			});
//		}
//	});
//}

// dave
void URtNetworkManager::RecvVoiceBuffer(TArray<uint8> packet)
{
	if (!VM || !URtConfig::UsingVoice)
		return;

	// dave - if local capture
	if (!URtConfig::VoiceTalk)
		return;

	if (packet.Num() < VHEADSIZE)
	{
		UE_LOG(LogNetworkManager, Warning, TEXT("Invalid voice packet size"));
		return;
	}

	const int32 VoiceDataSize = packet.Num() - VHEADSIZE;
	if (VoiceDataSize <= 0)
	{
		UE_LOG(LogNetworkManager, Warning, TEXT("No voice data in packet"));
		return;
	}
	
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, packet, VoiceDataSize]()
	{
		if (!ensure(packet.GetData() != nullptr))
			return;

		TArray<uint8> VoiceData;
		VoiceData.Append(packet.GetData() + sizeof(stVoiceHeader), VoiceDataSize);

		// 메인 스레드에서 데이터 처리
		AsyncTask(ENamedThreads::GameThread, [this, VoiceData]()
		{
			if (ensure(VM != nullptr && VM->IsValidLowLevel()))
			{
				//VM->DecodedAndRun(VoiceData, CompressedSize);
				VM->DecodedAndRun(VoiceData, VoiceData.Num());
				//UE_LOG(LogNetworkManager, Warning, TEXT(" Recieved Buffer - Size %d"), VoiceData.Num());
			}
		});
	});
}

// dave
void URtNetworkManager::OnNetworkFailure(ENetworkFailure::Type FailureType)
{
	switch (FailureType)
	{
		case ENetworkFailure::FailureReceived:
		{
			break;
		}
		case ENetworkFailure::PendingConnectionFailure:
		{
			break;
		}
		case ENetworkFailure::ConnectionLost:
		case ENetworkFailure::ConnectionTimeout:
		{
			Disconnect();
			break;
		}
		case ENetworkFailure::NetDriverAlreadyExists:
		case ENetworkFailure::NetDriverCreateFailure:
		case ENetworkFailure::OutdatedClient:
		case ENetworkFailure::OutdatedServer:
		default:
			break;
	}
}
