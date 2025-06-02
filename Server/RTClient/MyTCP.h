#pragma once
#include "Thread.h"

class CMyTCP : public CThread
{
public:
	int m_Index;	//브릿지 서버에서 게임서버의 인덱스
	bool m_bConnect;
	SOCKET m_hSocket;

	char m_RecvBuffer[ MAX_RECV ];

	int m_RecvWrite;

	WSAEVENT m_hEvent;

	int m_IP;
	int m_Port;

public:
	CMyTCP( void );
	~CMyTCP( void );

	void EmptyRecvBuffer();

	bool Connect();
	bool Connect( char* ip, int port );
	bool Connect( int ip, int port );
	void DisConnect();
	void Recv();

	void Send( char* buff, int size );

	void Parse( int protocol, char* packet );
	unsigned int ThreadProc();

public://Recv
	void RecvBChat( char* packet );

public://Send

	void SendSetSCT( char* filename );
};

