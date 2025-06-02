#pragma once

#include <WinSock2.h>
#include "Thread.h"
#include "MyDefine.h"

class CUDPServer : public CThread
{
public:
	WSAEVENT m_hEvent;

	int m_ServerPort;

	MYADDR m_ClientAddr;
	int m_ClientIP;
	int m_ClientPort;

	SOCKET m_hSocket;
	bool m_bActive;

	int m_Index;

	char m_HostName[128];
	char m_IP[20];

public:
	CUDPServer( void );
	~CUDPServer( void );

	void Finish();
	bool Start( int i, int port );

	void Recv();

	void SendTo( char* buff, int size, char* ip, int port );
	void SendTo( char* buff, int size, int ip, int port );
	

	void Log( const char* format, ... );
	BOOL WinSockStart();
protected:
	unsigned int ThreadProc();
	void UDPProc( char* packet, int size, SOCKADDR_IN saddr );

};

