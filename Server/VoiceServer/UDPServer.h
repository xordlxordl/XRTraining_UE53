#pragma once

#include <WinSock2.h>
#include <thread>
#include "MyDefine.h"
#include <concurrent_queue.h>
#include <vector>

using namespace std;
using namespace Concurrency;

class CUDPServer
{
protected:
	WSAEVENT m_hEvent;

	int m_ServerPort;

	MYADDR m_ClientAddr;
	int m_ClientIP;
	int m_ClientPort;

	SOCKET m_hSocket;
	bool m_bActive;

	int m_Index;


	std::thread m_RecvThread;
	std::thread m_SendThread;

	concurrent_queue< vector<unsigned char> > m_queue;

	void RecvProc();
	void SendProc();

	void Recv();

	void SendTo( unsigned char* buff, int size );
	void SendTo( unsigned char* buff, int size, char* ip, int port );
	void SendTo( unsigned char* buff, int size, int ip, int port );
	void SendToOther( int index, unsigned char* buff, int size );

public:
	CUDPServer();
	~CUDPServer( void );
	void Finish();

	bool Start( int i, int port );

};

