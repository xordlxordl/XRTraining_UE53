// Fill out your copyright notice in the Description page of Project Settings.

#include "Network/RtPacketSession.h"
// Data
#include "RtCore/Public/Common/RtProtocol.h"
// Manager
#include "Network/RtWorkerThread.h"
#include "Network/RtNetworkManager.h"
#include <SocketSubsystem.h>


SendBuffer::SendBuffer(int32 bufferSize)
{
	_buffer.SetNum(bufferSize);
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::CopyData(void* data, int32 len)
{
	::memcpy(_buffer.GetData(), data, len);
	_writeSize = len;
}

void SendBuffer::Close(uint32 writeSize)
{
	_writeSize = writeSize;
}

//-------------------------------------------------------------------------------------------
//

RtPacketSession::RtPacketSession(FSocket* sock) : Socket( sock ), QueueSize( 0 )
{
}

RtPacketSession::~RtPacketSession()
{
	DisConnect();
}

// PacketSeesion's Main Function
void RtPacketSession::RunThread()
{
	RecvWorkerThread = MakeShared<RecvWorker>(Socket, AsShared());
	SendWorkerThread = MakeShared<SendWorker>(Socket, AsShared());
}

void RtPacketSession::DisConnect()
{
	RecvPacketQueue.Empty();
	SendPacketQueue.Empty();

	if (RecvWorkerThread)
	{
		RecvWorkerThread->Destroy();
		RecvWorkerThread = nullptr;
	}

	// dave
	if (SendWorkerThread)
	{
		SendWorkerThread->Destroy();
		SendWorkerThread = nullptr;
	}
}

// dave
bool RtPacketSession::TryEnqueuePacket(const TArray<uint8>& Packet)
{
	FScopeLock Lock(&QueueLock);

	if (QueueSize >= 1000000)
	{
		UE_LOG(LogNetworkThread, Warning, TEXT("RecvPacketQueue is full. Size: %d"), QueueSize);
		return false;
	}

	if (RecvPacketQueue.Enqueue(Packet))
	{
		++QueueSize;
		return true;
	}
	return false;
}

// dave
bool RtPacketSession::DequeuePacket(TArray<uint8>& OutPacket)
{
	FScopeLock Lock(&QueueLock);
	if (RecvPacketQueue.Dequeue(OutPacket))
	{
		--QueueSize;
		return true;
	}
	return false;
}

int32 RtPacketSession::GetQueueSize() const
{
	FScopeLock Lock(&QueueLock);
	return QueueSize;
}

//
bool IsValidProtocol(uint16 ProtocolID)
{
	return ProtocolID > PROTOCOL_START && ProtocolID < PROTOCOL_END;
}

void RtPacketSession::HandleRecvPackets()
{
	while (true)
	{
		TArray<uint8> Packet;
		if (!RecvPacketQueue.Dequeue(OUT Packet))
			break;

		if (Packet.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("Received an empty packet."));
			continue;
		}

		if (!IsValid(GWorld))
			break;

		auto* GameInstance = GWorld->GetGameInstance()->GetSubsystem<URtNetworkManager>();

		if (GameInstance)
		{
			if (Packet.Num() < sizeof(stHeader))
			{
				UE_LOG(LogTemp, Warning, TEXT("Invalid packet size: %d"), Packet.Num());
				continue;
			}

			stHeader header;
			memcpy(&header, Packet.GetData(), sizeof(stHeader));

			// valid protocol?
			if (!IsValidProtocol(header.nID))
			{
				UE_LOG(LogTemp, Warning, TEXT("Invalid protocol ID: %d"), header.nID);
				continue;
			}

			GameInstance->Parse(header.nID, Packet);
		}
	}
}


RtUDPPacketSession::RtUDPPacketSession(FSocket* sock, TSharedPtr<FInternetAddr> InternetAddr_recv, TSharedPtr<FInternetAddr> InternetAddr_send) :
	Socket_U(sock), InternetAddr_Recv_U(InternetAddr_recv), InternetAddr_Send_U(InternetAddr_send)
{
	
}

RtUDPPacketSession::~RtUDPPacketSession()
{
	DisConnect();
}

void RtUDPPacketSession::RunThread()
{
	RecvWorkerThread_U = MakeShared<RecvWorker_UDP>(Socket_U, AsShared());
	SendWorkerThread_U = MakeShared<SendWorker_UDP>(Socket_U, AsShared());
}

void RtUDPPacketSession::DisConnect()
{
	if (RecvWorkerThread_U)
	{
		RecvWorkerThread_U->Destroy();
		RecvWorkerThread_U = nullptr;
	}

	// dave
	if (SendWorkerThread_U)
	{
		SendWorkerThread_U->Destroy();
		SendWorkerThread_U = nullptr;
	}
}

void RtUDPPacketSession::HandleRecvPackets()
{
	while (true)
	{
		TArray<uint8> Packet;
		if (RecvPacketQueue_U.Dequeue(OUT Packet) == false)
			break;

		if (Packet.IsEmpty())
			break;

		if (!IsValid(GWorld))	// dave
			break;

		auto* GameInstance = GWorld->GetGameInstance()->GetSubsystem<URtNetworkManager>();

		if (GameInstance)
		{
			GameInstance->RecvVoiceBuffer(Packet);
		}
	}
}

SendBuffer_UDP::SendBuffer_UDP(int32 bufferSize)
{
	_buffer.SetNum(bufferSize);
}

SendBuffer_UDP::~SendBuffer_UDP()
{
}

void SendBuffer_UDP::CopyData(void* data, int32 len)
{
	::memcpy(_buffer.GetData(), data, len);
	_writeSize = len;
}

void SendBuffer_UDP::Close(uint32 writeSize)
{
	_writeSize = writeSize;
}
