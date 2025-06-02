
#include "UDPServer.h"
#include <WS2tcpip.h>
#include <iostream>
#include <string>

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

CUDPServer::CUDPServer( void )
{
	m_ClientPort = 0;
	m_ClientAddr.i = m_ClientIP = 0;

	m_bActive = false;
	
	
	m_Index = INVALID_VALUE;
	m_hEvent = NULL;
	m_hSocket = NULL;
	
	m_ServerPort = 0;

	
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
		Log( "m_hSocket == INVALID_SOCKET PORT %d", port );
		return false;
	}

	SOCKADDR_IN	addr;
	memset( ( char* ) &addr, 0, sizeof( addr ) );

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl( INADDR_ANY );
	addr.sin_port = htons( m_ServerPort );

	if( bind( m_hSocket, ( LPSOCKADDR ) &addr, sizeof( addr ) ) == SOCKET_ERROR )
	{
		Log( " bind == SOCKET_ERROR" );
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

BOOL CUDPServer::WinSockStart()
{
	WSAData wsaData;
	int     error;

	error = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
	if( error == SOCKET_ERROR )
	{
		WSACleanup();
		return FALSE;
	}

	char str[ 256 ]; memset( str, 0x00, sizeof( str ) );

	memset( m_HostName, 0x00, sizeof( m_HostName ) );
	
	
	addrinfo info;	
	addrinfo* res, *p;
	memset( &info, 0, sizeof( info ) );
	info.ai_family = AF_INET;
	//info.ai_socktype = SOCK_STREAM;

	gethostname( m_HostName, sizeof( m_HostName ) );


	int status;
	if( ( status = getaddrinfo( m_HostName, NULL, &info, &res ) ) != 0 )
	{
		sprintf_s( str, "getaddrinfo ERROR %d", WSAGetLastError() );
		Log( str );
	}
	else
	{
		for( p = res; p != NULL; p = p->ai_next )
		{
			char ipstr[ INET_ADDRSTRLEN ];
			void* addr;
			std::string ipver;

			// IPv4와 IPv6 처리
			if( p->ai_family == AF_INET )
			{ // IPv4
				struct sockaddr_in* ipv4 = ( struct sockaddr_in* ) p->ai_addr;
				addr = &( ipv4->sin_addr );
				ipver = "IPv4";
			}
			else
			{ // IPv6
				struct sockaddr_in6* ipv6 = ( struct sockaddr_in6* ) p->ai_addr;
				addr = &( ipv6->sin6_addr );
				ipver = "IPv6";
			}

			// 주소를 텍스트로 변환
			inet_ntop( p->ai_family, addr, ipstr, sizeof( ipstr ) );
			
			Log( "%s : %s: %s", m_HostName, ipver.c_str(), ipstr );
		}
		
	}
	return TRUE;
}

void CUDPServer::SendTo( char* buff, int size, char* ip, int port )
{
	struct sockaddr_in addr;

	int result = 0;

	memset( &addr, 0, sizeof( addr ) );
	addr.sin_family = AF_INET;

	int ServerIP;
	inet_pton( AF_INET, ip, &ServerIP );

	addr.sin_addr.s_addr = ServerIP;//inet_addr( ip );
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
		//	Log( "sucees sendto 1 %d.%d.%d.%d:%d Size:%d", my.b[0], my.b[1], my.b[2], my.b[3], port,result  );
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

	UDPProc( buffer, nRead, addr );
}

void CUDPServer::UDPProc( char* packet,  int size, SOCKADDR_IN saddr )
{
	m_ClientAddr.i = saddr.sin_addr.S_un.S_addr;
	m_ClientIP = m_ClientAddr.i;
	m_ClientPort = htons( saddr.sin_port );

	
	//SendToOther( m_Index, packet, size );

}


void CUDPServer::Log( const char* format, ... )
{

	va_list argptr;
	char msg[ 4096 ]; memset( msg, 0x00, sizeof( msg ) );
	va_start( argptr, format );
	vsprintf_s( msg, format, argptr );
	va_end( argptr );

	
	printf( msg );
	
	
}