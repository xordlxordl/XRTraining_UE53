
#include "MyInclude.h"

void CUDPServer::RecvProc()
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
		//Sleep( 1 );
		this_thread::sleep_for( chrono::milliseconds( 1 ) );
	}
}

void CUDPServer::SendProc()
{
	while( m_bActive )
	{
		if( m_queue.empty() ) 
		{
			//Sleep( 1 );
			this_thread::sleep_for( chrono::milliseconds(1) );
			continue;
		}

		vector<unsigned char> packet;
		m_queue.try_pop( packet );

		if( g_SendToME )
			SendTo( packet.data(), ( int )packet.size() );
		SendToOther( m_Index, packet.data(), ( int )packet.size() );


		//Sleep( 1 );
		this_thread::sleep_for( chrono::milliseconds( 1 ) );
	}
}

CUDPServer::CUDPServer()
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
	
	SAFE_CLOSESOCK( m_hSocket );

	if( m_hEvent )
	{
		WSACloseEvent( m_hEvent );
		m_hEvent = NULL;
	}

}

bool CUDPServer::Start( int i, int port )
{
	
	SAFE_CLOSESOCK( m_hSocket );

	m_ServerPort = port;

	m_hSocket = socket( AF_INET, SOCK_DGRAM, 0 );

	if( m_hSocket == INVALID_SOCKET )
	{
		Log( "ERROR: INVALID_SOCKET.. PORT:[ %d ]", port );
		return false;
	}

	SOCKADDR_IN	addr;
	memset( ( char* ) &addr, 0, sizeof( addr ) );

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl( INADDR_ANY );
	addr.sin_port = htons( m_ServerPort );

	if( ::bind( m_hSocket, ( LPSOCKADDR ) &addr, sizeof( addr ) ) == SOCKET_ERROR )
	{
		Log( "ERROR: UDP Bind Error PORT:[ %d ]", m_ServerPort );
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

	m_RecvThread = thread( &CUDPServer::RecvProc, this );
	m_SendThread = thread( &CUDPServer::SendProc, this );

	
	m_RecvThread.detach();
	m_SendThread.detach();

	return true;
}

void CUDPServer::Recv()
{
	int nRead;
	SOCKADDR_IN	addr;
	int addrlen = sizeof( addr );

	unsigned char buffer[ MAX_BUFFER ]; memset( buffer, 0x00, sizeof( buffer ) );

	nRead = recvfrom( m_hSocket,(char*) buffer, MAX_BUFFER, 0, ( struct sockaddr FAR* ) & addr, &addrlen );

	if( nRead <= 0 )
		return;

	if( addr.sin_port <= 0 )
		return;

	m_ClientAddr.i = addr.sin_addr.S_un.S_addr;
	m_ClientIP = m_ClientAddr.i;
	m_ClientPort = htons( addr.sin_port );

	vector< unsigned char > packet( buffer, buffer + nRead );

	m_queue.push( packet );


	//UDPProc( buffer, nRead, addr ); //
}

void CUDPServer::SendTo( unsigned char* buff, int size )
{
	if( m_ClientIP == 0 )
		return;
	if( m_ClientPort == 0 )
		return;

	struct sockaddr_in addr;

	int result = 0;

	memset( &addr, 0, sizeof( addr ) );
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = m_ClientIP;
	addr.sin_port = htons( m_ClientPort );

	result = sendto( m_hSocket, ( const char* ) buff, size, 0, ( sockaddr* ) &addr, sizeof( addr ) );

	if( result < size )
	{

		stMyAddr my;
		my.i = m_ClientIP;
		Log( "fail sendto error:%d,  %d.%d.%d.%d:%d", WSAGetLastError(), my.b[ 0 ], my.b[ 1 ], my.b[ 2 ], my.b[ 3 ], m_ClientPort );

		return;
	}
	else
	{
		//Log( "sucees sendto 1 %s:%d Size:%d", m_ClientIP, m_ClientPort, result );
	}

}

void CUDPServer::SendTo( unsigned char* buff, int size, char* ip, int port )
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

		stMyAddr my;
		my.i = inet_addr( ip );
		Log( "fail sendto error:%d,  %d.%d.%d.%d:%d", WSAGetLastError(), my.b[ 0 ], my.b[ 1 ], my.b[ 2 ], my.b[ 3 ], m_ClientPort );

		return;
	}
	else
	{
		//Log( "sucees sendto 1 %s:%d Size:%d", ip, port, result );
	}
}

void CUDPServer::SendTo( unsigned char* buff, int size, int ip, int port )
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

void CUDPServer::SendToOther( int index, unsigned char* buff, int size )
{
	int i;
	for( i = 0; i < MAX_UDP; ++i )
	{
		if( g_UDP[ i ].m_Index == index )
			continue;

		if( g_UDP[ i ].m_ClientIP == 0 || g_UDP[ i ].m_ClientPort == 0 )
			continue;

		SendTo( buff, size, g_UDP[ i ].m_ClientIP, g_UDP[ i ].m_ClientPort );

	}
}