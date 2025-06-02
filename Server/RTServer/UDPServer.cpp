
#include "MyInclude.h"

#define USE_EVENT

#ifdef USE_EVENT
unsigned int CUDPServer::ThreadProc()
{
	Log( "Thread Start [Index : %d] Port : %d", m_Index, m_ServerPort );

	WSANETWORKEVENTS events;

	DWORD dwReturn = 0, dwRet = 0;

	while( m_bActive )
	{
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
				}
			}
		}
		Sleep( 1 );
	}
	return 1;
}
#else
unsigned int CUDPServer::ThreadProc()
{
	Log( "Thread Start [Index : %d] Port : %d", m_Index, m_ServerPort );
	int nRead;
	SOCKADDR_IN	addr;
	int addrlen = sizeof( addr );

	while( m_bActive )
	{
		char buffer[ MAX_BUFFER ]; memset( buffer, 0x00, sizeof( buffer ) );

		nRead = recvfrom( m_hSocket, buffer, MAX_BUFFER, 0, ( struct sockaddr FAR* ) & addr, &addrlen );

		if( nRead > 0 )
		{
			short len = 0;
			if( addr.sin_port < 0 )
				continue;

			UDPProc( buffer, nRead, addr ); // ���� ��Ŷ�� 1�� ó���Ѵ�.

		}
		Sleep( 1 );
	}
	return 1;
	
}
#endif

CUDPServer::CUDPServer( void )
{
	m_ClientPort = 0;
	m_ClientAddr.i = m_ClientIP = 0;

	m_bActive = false;
	
	
	m_Index = INVALID_VALUE;
	m_hEvent = NULL;
	m_hSocket = NULL;
	
	m_ServerPort = 0;
	m_RecvTime = 0;
	
}

CUDPServer::~CUDPServer( void )
{
	Finish();
}

void CUDPServer::Finish()
{
	m_bActive = false;
	CThread::End();

	if( m_hSocket != NULL )
	{
		closesocket( m_hSocket );
		m_hSocket = NULL;
	}

	if( m_hEvent )
	{
		WSACloseEvent( m_hEvent );
		m_hEvent = NULL;
	}
	m_RecvTime = 0;
}

bool CUDPServer::Start( int i, int port )
{
	if( m_hSocket != NULL )
	{
		closesocket( m_hSocket );
		m_hSocket = NULL;
	}

	m_ServerPort = port;
	m_hSocket = socket( AF_INET, SOCK_DGRAM, 0 );

	if( m_hSocket == INVALID_SOCKET )
	{
		Log( "ERROR: UDP Socket Error. PORT %d", port );
		return false;
	}

	SOCKADDR_IN	addr;
	memset( ( char* ) &addr, 0, sizeof( addr ) );

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl( INADDR_ANY );
	addr.sin_port = htons( m_ServerPort );

	if( ::bind( m_hSocket, ( LPSOCKADDR ) &addr, sizeof( addr ) ) == SOCKET_ERROR )
	{
		Log( "ERROR: UDP bind Error." );
		return false;
	}

	m_hEvent = WSACreateEvent();
	WSAEventSelect( m_hSocket, m_hEvent, FD_READ );

	
	DWORD recvsize = MAX_BUFFER;
	DWORD sendsize = MAX_BUFFER;
	setsockopt( m_hSocket, SOL_SOCKET, SO_RCVBUF, ( const char* ) &recvsize, sizeof( recvsize ) );
	setsockopt( m_hSocket, SOL_SOCKET, SO_SNDBUF, ( const char* ) &sendsize, sizeof( sendsize ) );

	m_bActive = true;
	m_Index = i;

	CThread::Begin();
	

	return true;
}

void CUDPServer::InitClientAddr()
{
	m_ClientAddr.i = 0;
	m_ClientIP = 0;
	m_ClientPort = 0;
	m_RecvTime = 0;
	
}

void CUDPServer::SendTo( char* buff, int size, char* ip, int port )
{
	struct sockaddr_in addr;

	int result = 0;

	memset( &addr, 0, sizeof( addr ) );
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr( ip );
	addr.sin_port = htons( port );

	result = sendto( m_hSocket, ( const char* ) buff, size, 0, ( sockaddr* ) &addr, sizeof( addr ) );

	if( result < size )
	{

		Log( "fail sendto 1 %s:%d", ip, port );

		return;
	}
	else
	{
		Log( "sucees sendto 1 %s:%d Size:%d", ip, port, result );
	}
}

void CUDPServer::SendTo( char* buff, int size )
{
	if( m_ClientIP <= 0 )
		return;
	if( m_ClientPort <= 0 )
		return;

	struct sockaddr_in addr;

	int result = 0;

	memset( &addr, 0, sizeof( addr ) );
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr =  m_ClientIP;
	addr.sin_port = htons( m_ClientPort );

	result = sendto( m_hSocket, ( const char* ) buff, size, 0, ( sockaddr* ) &addr, sizeof( addr ) );

	if( result < size )
	{

		Log( "fail sendto 1 %s:%d", m_ClientIP, m_ClientPort );

		return;
	}
	else
	{
		//Log( "sucees sendto 1 %s:%d Size:%d", m_ClientIP, m_ClientPort, result );
	}

}

void CUDPServer::SendTo( char* buff, int size, int ip, int port )
{
	struct sockaddr_in addr;

	int result = 0;

	memset( &addr, 0, sizeof( addr ) );
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ip;
	addr.sin_port = htons( port );

	result = sendto( m_hSocket, ( const char* ) buff, size, 0, ( sockaddr* ) &addr, sizeof( addr ) );

	if( result < size )
	{
		stMyAddr my;
		my.i = ip;
		Log( "fail sendto 1 %d.%d.%d.%d:%d", my.b[ 0 ], my.b[ 1 ], my.b[ 2 ], my.b[ 3 ], port );
		return;
	}
	else
	{
		stMyAddr my;
		my.i = ip;
		//Log( "sucees sendto 1 %d.%d.%d.%d:%d Size:%d", my.b[0], my.b[1], my.b[2], my.b[3], port, result  );
	}
}

void CUDPServer::SendToOther( int index, char* buff, int size )
{
	int i;
	for( i = 0; i < MAX_UDP; ++i )
	{
		if( g_UDP[i].m_Index == index )
			continue;

		if( g_UDP[ i ].m_ClientIP == 0 || g_UDP[ i ].m_ClientPort == 0 )
			continue;
		
		SendTo( buff, size, g_UDP[i].m_ClientIP, g_UDP[i].m_ClientPort );
		
	}
}

void CUDPServer::Recv()
{
	int nRead;
	SOCKADDR_IN	addr;
	int addrlen = sizeof( addr );

	char buffer[ MAX_BUFFER ]; memset( buffer, 0x00, sizeof( buffer ) );

	nRead = recvfrom( m_hSocket, buffer, MAX_BUFFER, 0, ( struct sockaddr FAR* ) & addr, &addrlen );

	if( nRead <= 0 )
		return;
	
	if( addr.sin_port <= 0 )
		return;

	UDPProc( buffer, nRead, addr ); //
}

void CUDPServer::UDPProc( char* packet, int size, SOCKADDR_IN saddr )
{

	//stUDPPacket ack;
	//memcpy( &ack, packet, sizeof(stUDPPacket) );

	m_RecvTime = GetTickCount64();

	m_ClientAddr.i = saddr.sin_addr.S_un.S_addr;
	m_ClientIP = m_ClientAddr.i;
	m_ClientPort = htons( saddr.sin_port );

	if( g_bUDPTest )
		SendTo( packet, size, m_ClientIP, m_ClientPort );

	SendToOther( m_Index, packet, size );
}