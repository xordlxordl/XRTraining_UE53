// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FSocket;

#define USING_SHARED_PTR(name)	using name##Ref = TSharedPtr<class name>;

USING_SHARED_PTR(Session);
USING_SHARED_PTR(RtPacketSession);
USING_SHARED_PTR(SendBuffer);


class RTGAME_API RtPacketSession : public TSharedFromThis< RtPacketSession >	//스마트 포인터로
{
public:
	FSocket* Socket;
	TSharedPtr< class RecvWorker, ESPMode::ThreadSafe > RecvWorkerThread;   // dave
	TSharedPtr< class SendWorker, ESPMode::ThreadSafe > SendWorkerThread;

	// GameThread와 NetworkThread가 데이터 주고 받는 공용 큐.
	TQueue<TArray<uint8>> RecvPacketQueue;
	TQueue<SendBufferRef> SendPacketQueue;

public:
	RtPacketSession(FSocket* sock);
	~RtPacketSession();

	void RunThread();
	void DisConnect();

	// dave - test
	bool TryEnqueuePacket(const TArray<uint8>& Packet);
	bool DequeuePacket(TArray<uint8>& OutPacket);
	int32 GetQueueSize() const;

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

private:
	mutable FCriticalSection QueueLock;
	int32 QueueSize;
};


class SendBuffer : public TSharedFromThis<SendBuffer>
{
public:
	SendBuffer(int32 bufferSize);
	~SendBuffer();


	BYTE* Buffer() { return _buffer.GetData(); }
	int32 WriteSize() { return _writeSize; }
	int32 Capacity() { return static_cast<int32>(_buffer.Num()); }

	void CopyData(void* data, int32 len);
	void Close(uint32 writeSize);


private:
	TArray<BYTE>	_buffer;
	int32			_writeSize = 0;
};



//===========udp==================

class RTGAME_API RtUDPPacketSession : public TSharedFromThis<RtUDPPacketSession>
{
public:
	FSocket* Socket_U;
	TSharedPtr<FInternetAddr> InternetAddr_Recv_U;
	TSharedPtr<FInternetAddr> InternetAddr_Send_U;
	TSharedPtr< class RecvWorker_UDP, ESPMode::ThreadSafe > RecvWorkerThread_U;  // dave
	TSharedPtr< class SendWorker_UDP, ESPMode::ThreadSafe > SendWorkerThread_U;

	TQueue<TArray<uint8>> RecvPacketQueue_U;
	TQueue<TSharedPtr<class SendBuffer_UDP>> SendPacketQueue_U;

public:
	RtUDPPacketSession(FSocket* sock, TSharedPtr<FInternetAddr> InternetAddr_recv, TSharedPtr<FInternetAddr> InternetAddr_send);
	~RtUDPPacketSession();

	void RunThread();
	void DisConnect();


	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

};


class SendBuffer_UDP : public TSharedFromThis<SendBuffer_UDP>
{
public:
	SendBuffer_UDP(int32 bufferSize);
	~SendBuffer_UDP();


	BYTE* Buffer() { return _buffer.GetData(); }
	int32 WriteSize() { return _writeSize; }
	int32 Capacity() { return static_cast<int32>(_buffer.Num()); }

	void CopyData(void* data, int32 len);
	void Close(uint32 writeSize);


private:
	TArray<BYTE>	_buffer;
	int32			_writeSize = 0;
};

