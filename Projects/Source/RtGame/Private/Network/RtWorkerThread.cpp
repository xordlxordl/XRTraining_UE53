// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/RtWorkerThread.h"
// Socket
#include "Sockets.h"
#include "Common/UdpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
// Data
#include "RtCore/Public/Common/RtPacket.h"

 
DEFINE_LOG_CATEGORY(LogNetworkThread);

RecvWorker::RecvWorker(FSocket* sock, TSharedPtr<RtPacketSession> session) : Socket(sock) , Session(session)
{
	Thread = FRunnableThread::Create(this, TEXT("RecvWorkerThread"));

}

RecvWorker::~RecvWorker()
{
	Destroy();
}

bool RecvWorker::Init()
{
	UE_LOG(LogNetworkThread, Log, TEXT("Recv Thread Init"));
	return true;
}

uint32 RecvWorker::Run()
{
	while (bRun)
	{
		TArray<uint8> Packet;
		if (ReceivePacket(OUT Packet))
		{
			if (TSharedPtr<RtPacketSession> pSession = Session.Pin())
			{
				pSession->RecvPacketQueue.Enqueue(Packet);
			}
		}
	}
	return 0;
}

bool RecvWorker::ReceivePacket(TArray<uint8>& OutPacket)
{

	// 패킷 헤더 파싱
	const int32 HeaderSize = HEADSIZE;//sizeof( stHeader );
	TArray<uint8> HeaderBuffer;
	HeaderBuffer.AddZeroed(HeaderSize);

	if (ReceiveDesiredBytes(HeaderBuffer.GetData(), HeaderSize) == false)
		return false;

	// ID, Size 추출
	stHeader Header;
	{
		FMemoryReader Reader(HeaderBuffer);
		Reader << Header;
		UE_LOG(LogTemp, Log, TEXT("Recv PacketID : %d, PacketSize : %d"), Header.nID, Header.nSize);
	}
	// 패킷 헤더 복사
	OutPacket = HeaderBuffer;

	// 패킷 내용 파싱
	TArray<uint8> PayloadBuffer;
	const int32 PayloadSize = Header.nSize - HeaderSize;
	if (PayloadSize == 0)
		return true;

	OutPacket.AddZeroed(PayloadSize);

	if (ReceiveDesiredBytes(&OutPacket[HeaderSize], PayloadSize))
		return true;

	return false;
}

bool RecvWorker::ReceiveDesiredBytes(uint8* Results, int32 Size)
{
	if (!Socket) return false;

	uint32 PendingDataSize;
	if (Socket->HasPendingData(PendingDataSize) == false || PendingDataSize <= 0)
		return false;

	int32 Offset = 0;

	while (Size > 0)
	{
		int32 NumRead = 0;
		Socket->Recv(Results + Offset, Size, OUT NumRead);
		check(NumRead <= Size);

		if (NumRead <= 0)
			return false;

		Offset += NumRead;
		Size -= NumRead;
	}

	return true;
}

void RecvWorker::Exit()
{
	bRun = false;
}

void RecvWorker::Destroy()
{
	bRun = false;
}


//--------------------------------------------------------------------------------------------------------------------------
// SendWorker
//--------------------------------------------------------------------------------------------------------------------------
SendWorker::SendWorker(FSocket* sock, TSharedPtr<RtPacketSession> session) : Socket(sock), Session(session)
{
	Thread = FRunnableThread::Create(this, TEXT("SendWorkerThread"));
}

SendWorker::~SendWorker()
{
	Destroy();
}

bool SendWorker::Init()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Send Thread Init")));

	return true;
}

uint32 SendWorker::Run()
{
	while (bRun)
	{
		SendBufferRef SendBuffer;

		if (TSharedPtr<RtPacketSession> pSession = Session.Pin())
		{
			if (pSession->SendPacketQueue.Dequeue(OUT SendBuffer))
			{
				SendPacket(SendBuffer);
			}
		}
	}

	return 0;
}

void SendWorker::Exit()
{
	bRun = false;
}

bool SendWorker::SendPacket(SendBufferRef SendBuffer)
{
	if (SendDesiredBytes(SendBuffer->Buffer(), SendBuffer->WriteSize()) == false)
		return false;

	return true;
}

void SendWorker::Destroy()
{
	bRun = false;
}

bool SendWorker::SendDesiredBytes(const uint8* Buffer, int32 Size)
{
	while (Size > 0)
	{
		int32 BytesSent = 0;
		if (Socket->Send(Buffer, Size, BytesSent) == false)
			return false;

		Size -= BytesSent;
		Buffer += BytesSent;
	}

	return true;
}


//--------------------------------------------------------------------------------------------------------------------------
// RecvWorker_UDP
//--------------------------------------------------------------------------------------------------------------------------


RecvWorker_UDP::RecvWorker_UDP(FSocket* sock, TSharedPtr<RtUDPPacketSession> session) : Socket(sock), Session(session)
{
	Thread = FRunnableThread::Create(this, TEXT("RecvWorkerThreadForUDP"));
}

RecvWorker_UDP::~RecvWorker_UDP()
{
	Destroy();
}

bool RecvWorker_UDP::Init()
{
	UE_LOG(LogNetworkThread, Log, TEXT("Recv_UDP Thread Init"));
	return true;
}

uint32 RecvWorker_UDP::Run()
{
	while (bRun)
	{
		TArray<uint8> Packet;
		if (ReceivePacket(OUT Packet))
		{
			if (Packet.IsEmpty()) continue;

			if (TSharedPtr<RtUDPPacketSession> pSession = Session.Pin())
			{
				pSession->RecvPacketQueue_U.Enqueue(Packet);
			}
		}
	}
	return 0;
}

void RecvWorker_UDP::Exit()
{
	bRun = false;
}

void RecvWorker_UDP::Destroy()
{
	bRun = false;
}

bool RecvWorker_UDP::ReceivePacket(TArray<uint8>& OutPacket)
{
	if (!Socket) return false;
	uint32 ReceivedDataSize;
	TArray<uint8> ReceivedData;
	
	while (Socket->HasPendingData(ReceivedDataSize))
	{
		ReceivedData.SetNumUninitialized(ReceivedDataSize);
		// maybe need empty addr
		int32 BytesRead = 0;
		Socket->RecvFrom(ReceivedData.GetData(), ReceivedDataSize, OUT BytesRead, *Session.Pin()->InternetAddr_Recv_U);
		OutPacket = ReceivedData;
	}
	FPlatformProcess::Sleep(0.01f);
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------
// SendWorker_UDP
//--------------------------------------------------------------------------------------------------------------------------

SendWorker_UDP::SendWorker_UDP(FSocket* sock, TSharedPtr<RtUDPPacketSession> session) : Socket(sock), Session(session)
{
	Thread = FRunnableThread::Create(this, TEXT("SendWorkerThreadForUDP"));
}

SendWorker_UDP::~SendWorker_UDP()
{
	Destroy();
}

bool SendWorker_UDP::Init()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Send UDP Thread Init")));

	return true;
}

uint32 SendWorker_UDP::Run()
{
	while (bRun)
	{
		TSharedPtr<SendBuffer_UDP> SendBuffer;

		if (TSharedPtr<RtUDPPacketSession> pSession = Session.Pin())
		{
			if (pSession->SendPacketQueue_U.Dequeue(OUT SendBuffer))
			{
				SendPacket(SendBuffer);
			}
		}
	}

	return 0;
}

void SendWorker_UDP::Exit()
{
	bRun = false;
}

bool SendWorker_UDP::SendPacket(TSharedPtr<SendBuffer_UDP> SendBuffer)
{
	if (Socket == nullptr) {
		UE_LOG(LogNetworkThread, Log, TEXT("InValid Socket"));
		return false;
	}
	uint8* data = SendBuffer->Buffer();
	int32 size = SendBuffer->WriteSize();
	int32 sent = 0;

	if (size <= 0) return true;
	// Server's Recv Port
	Socket->SendTo(data, size, sent, *Session.Pin()->InternetAddr_Send_U);
	return true;
}

void SendWorker_UDP::Destroy()
{
	bRun = false;
}
