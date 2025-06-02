#include "pch.h"
#include "RTPacket.h"
#include "MyTCP.h"

unsigned int CMyTCP::ThreadProc()
{
	WSANETWORKEVENTS events;

	DWORD dwReturn = 0, dwRet = 0;
	bool result = false;
	while( m_bRun )
	{
		if( m_bConnect == false )
		{
			result = Connect( m_IP, m_Port );

			if( result == false )
			{
				continue;
			}
			else
			{
				
			}
		}
		dwReturn = WSAWaitForMultipleEvents( 1, &m_hEvent, FALSE, WSA_INFINITE, FALSE );

		if( dwReturn != WSA_WAIT_FAILED )
		{
			if( m_hSocket )
			{
				dwRet = WSAEnumNetworkEvents( m_hSocket, m_hEvent, &events );
				if( dwRet == 0 )
				{
					if( ( events.lNetworkEvents & FD_READ ) == FD_READ )
					{
						Recv();
					}
					if( ( events.lNetworkEvents & FD_CLOSE ) == FD_CLOSE )
					{
						DisConnect();
					}
				}
			}
		}
		Sleep( 1 );
	}
	return 1;
}


CMyTCP::CMyTCP( void )
{
	m_Index = -1;
	m_bConnect = false;
	m_hSocket = NULL;

	memset( m_RecvBuffer, 0x00, sizeof( m_RecvBuffer ) );

	m_RecvWrite = 0;

	m_hEvent = NULL;
	m_IP = 0;
	m_Port = 0;

}

CMyTCP::~CMyTCP( void )
{
	DisConnect();
}

void CMyTCP::EmptyRecvBuffer()
{
	memset( m_RecvBuffer, 0x00, sizeof( m_RecvBuffer ) );
	m_RecvWrite = 0;
}

union uADDR
{
	int IP;
	unsigned char BP[ 4 ];
};

bool CMyTCP::Connect()
{
	uADDR addr;
	addr.IP = m_IP;
	
	return Connect( m_IP, m_Port );
}

bool CMyTCP::Connect( char* ip, int port )
{
	//int ServerIP = inet_addr( ip );
	int ServerIP;
	inet_pton( AF_INET, ip, &ServerIP );
	return Connect( ServerIP, port );
}

bool CMyTCP::Connect( int ip, int port )
{
	if( m_bConnect )
	{
		DisConnect();
		//return false;
	}

	sockaddr_in ServerAddress;
	m_hSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

	if( m_hSocket == INVALID_SOCKET )
	{
		return false;
	}

	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = ip;
	ServerAddress.sin_port = htons( port );

	int error = connect( m_hSocket, ( struct sockaddr* ) &ServerAddress, sizeof( ServerAddress ) );
	if( error == SOCKET_ERROR )
	{
		SAFE_CLOSESOCK( m_hSocket );

		return false;
	}

	//Create Event
	m_hEvent = WSACreateEvent();
	//Select Socket Event
	WSAEventSelect( m_hSocket, m_hEvent, FD_READ | FD_CLOSE );

	BOOL opt_val = TRUE;
	error = setsockopt( m_hSocket, IPPROTO_TCP, TCP_NODELAY, ( char* ) &opt_val, sizeof( opt_val ) );

	DWORD size = 65535;
	setsockopt( m_hSocket, SOL_SOCKET, SO_SNDBUF, ( const char* ) &size, sizeof( size ) );
	setsockopt( m_hSocket, SOL_SOCKET, SO_RCVBUF, ( const char* ) &size, sizeof( size ) );

	m_bConnect = true;

	m_Index = INVALID_VALUE;

	CThread::Begin();


	return true;
}
void CMyTCP::DisConnect()
{
	SAFE_CLOSESOCK( m_hSocket );

	m_bConnect = false;
	CThread::End();
}

void CMyTCP::Recv()
{
	if( m_hSocket == NULL )
		return;

	int size = 0;
	if( m_RecvWrite < MAX_RECV )
		size = recv( m_hSocket, &m_RecvBuffer[ m_RecvWrite ], MAX_RECV - m_RecvWrite, 0 );

	if( size > 0 )
	{
		//현재 리시브버퍼의 길이에 더한다
		m_RecvWrite += size;

		if( m_RecvWrite >= MAX_RECV )
		{
			
		}

		while( m_RecvWrite >= HEADSIZE )
		{

			stHeader header;
			memcpy( &header, m_RecvBuffer, HEADSIZE );

			if( header.nID >= PROTOCOL_END || header.nID <= PROTOCOL_START )
			{
				//Clear();
				EmptyRecvBuffer();
				return;
			}
			if( header.nSize <= 0 )
			{
				EmptyRecvBuffer();
				return;
			}
			int iCheckSum = header.nType + header.nSize + header.nID;
			if( header.nCheckSum != iCheckSum )
			{
				EmptyRecvBuffer();

				return;
			}


			if( m_RecvWrite >= header.nSize )
			{
				Parse( header.nID, m_RecvBuffer );
				memmove( m_RecvBuffer, &m_RecvBuffer[ header.nSize ], m_RecvWrite );
				m_RecvWrite -= header.nSize;

			}
			else
			{
				break;
			}
		}
	}
}

void CMyTCP::Send( char* buff, int size )
{
	if( m_hSocket == NULL )
		return;

	if( buff == NULL )
		return;

	int sendsize, error = 0;

	do
	{
		sendsize = send( m_hSocket, buff, size, 0 );


		if( sendsize <= 0 )
		{

			break;
		}
		else
		{
			buff = buff + sendsize; // 버퍼의 위치를 보낸만큼 뒤로 뷁
			size -= sendsize;		// 패킷 사이즈를 보낸만큼 빼준다.

		}
	} while( size );//Size가 0이 될때까지..보낸다
}

void CMyTCP::Parse( int protocol, char* packet )
{
}

void CMyTCP::RecvBChat( char* packet )
{
}

void CMyTCP::SendSetSCT( char* filename )
{
	stSetSCTReq req;

	MtoW( filename, (wchar_t*) req.filename, sizeof( req.filename ) );

	//sprintf_s( req.filename, filename );

	char buffer[1024];	memset( buffer, 0x00, sizeof( buffer ) );

	memcpy( buffer, &req, sizeof( stSetSCTReq ) );
	Send( buffer, sizeof( stSetSCTReq ) );
}