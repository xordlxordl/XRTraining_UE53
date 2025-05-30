// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Http
#include "http.h"
#include "RtPacketSession.h"
// Protocol
#include "Common/RtWebCommon.h"
// Data
#include "Common/RtRpcCommon.h"

RTGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogNetworkThread, Log, All);

class FSocket;
class RtPacketSession;

class RTGAME_API RecvWorker : public FRunnable
{
protected:
	FRunnableThread* Thread = nullptr;
	bool bRun = true;
	FSocket* Socket;
	TWeakPtr< RtPacketSession > Session;

public:
	RecvWorker(FSocket* sock, TSharedPtr< RtPacketSession > session);
	~RecvWorker();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void Destroy();

private:
	bool ReceivePacket(TArray<uint8>& OutPacket);
	bool ReceiveDesiredBytes(uint8* Results, int32 Size);
};

class RTGAME_API SendWorker : public FRunnable
{
protected:
	FRunnableThread* Thread = nullptr;
	bool bRun = true;
	FSocket* Socket;
	TWeakPtr< RtPacketSession > Session;

public:
	SendWorker(FSocket* sock, TSharedPtr< RtPacketSession > session);
	~SendWorker();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	bool SendPacket(SendBufferRef SendBuffer);

	void Destroy();

private:
	bool SendDesiredBytes(const uint8* Buffer, int32 Size);


};

// M : Motion
class RTGAME_API RecvWorker_UDP : public FRunnable
{
public:
	FRunnableThread* Thread = nullptr;
	bool bRun = true;
	FSocket* Socket;
	TWeakPtr< RtUDPPacketSession > Session;

public:
	RecvWorker_UDP(FSocket* sock, TSharedPtr< RtUDPPacketSession > session);
	~RecvWorker_UDP();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void Destroy();

private:
	bool ReceivePacket(TArray<uint8>& OutPacket);
	// 제거예정
	//bool ReceiveDesiredBytes(uint8* Results, int32 Size);
};


class RTGAME_API SendWorker_UDP : public FRunnable
{
public:
	FRunnableThread* Thread = nullptr;
	bool bRun = true;
	FSocket* Socket;
	TWeakPtr< RtUDPPacketSession > Session;

public:
	SendWorker_UDP(FSocket* sock, TSharedPtr< RtUDPPacketSession > session);
	~SendWorker_UDP();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	bool SendPacket(TSharedPtr<SendBuffer_UDP> SendBuffer);

	void Destroy();
};



