#pragma once
class CUser
{
public:
	SOCKET m_hSocket;

	int m_ThreadNum;

	int m_Index;
	int m_UID;

	int m_RecvWrite;
	char m_RecvBuffer[ MAX_RECV ];

	int m_SendSize;
	char m_SendBuffer[ MAX_SEND ];

	int m_nGrade;
	char m_Name[32];

	string m_ObjID;

public:
	CUser( void );
	~CUser( void );
	void Clear();

	bool Init( int index, SOCKET sock, sockaddr_in ip );

	void LogOut();

	void EmptyRecvBuffer();

	int FlushSendBuffer();
	bool AddSendBuffer( char* buff, int size );

protected:
	bool CheckObjID( char* id );

public:
	void Send( char* buff, int size );

	void Recv();
	void Parse( int protocol, char* packet );

public: //RECV
	void RecvLoginReq( char* packet );
	
	void RcvSetSCTReq( char* packet );
	void RecvStartReq( char* packet );

public:	//SEND
	void SendDefault( char* packet );
	void SendConnect();
	void SendSCT();
};

