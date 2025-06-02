#include "MyInclude.h"

#define VER_KOREA

CUserManager::CUserManager(void)
{
	m_PreIndex = -1;
	m_ChatIndex = 0;
	m_bStart = false;
}


CUserManager::~CUserManager(void)
{

}

CUser* CUserManager::GetUser( int uid )
{
	if( uid <= 0 )
		return NULL;
	
	int i;
	for( i = 0; i < MAX_USER; ++i )
	{
		if( m_User[ i ].m_UID == uid )
			return &m_User[i];
	}
	return NULL;
}

bool CUserManager::AddUser( SOCKET sock,  sockaddr_in ip )
{
	int i;
	for( i = 0; i < MAX_USER; ++i )
	{
		if( m_User[ i ].m_hSocket != NULL )
			continue;

		WSAResetEvent( g_Event[ i ] );
		WSAEventSelect( sock, g_Event[ i ], FD_READ | FD_WRITE | FD_CLOSE );

		BOOL opt_val = TRUE;
		setsockopt( sock, IPPROTO_TCP, TCP_NODELAY, (char*)&opt_val, sizeof( opt_val ) );

		DWORD recvsize = MAX_RECV;
		DWORD sendsize = MAX_SEND;
		setsockopt( sock, SOL_SOCKET, SO_SNDBUF, (const char*)&recvsize, sizeof(recvsize) );
		setsockopt( sock, SOL_SOCKET, SO_RCVBUF, (const char*)&sendsize, sizeof(sendsize) );
		
		struct linger Linger;
		Linger.l_onoff = 1; //¸µ°Å ²ô±â
		Linger.l_linger = 0;
		setsockopt( sock, SOL_SOCKET, SO_LINGER, (const char*)&Linger, sizeof(Linger) );


		m_User[i].Init( i, sock, ip );

		Log("AddUser: %d %d.%d.%d.%d", i, 
			 ip.sin_addr.S_un.S_un_b.s_b1,
			 ip.sin_addr.S_un.S_un_b.s_b2,
			 ip.sin_addr.S_un.S_un_b.s_b3,
			 ip.sin_addr.S_un.S_un_b.s_b4 );

		return true;

	}
	return false;
}

void CUserManager::SendMsg( char* buff, int size, int FID )
{
	CUser* pUser = GetUser( FID );

	if( pUser == NULL )
	{
		return;
	}
	pUser->Send( buff, size );

}

void CUserManager::DelUser( int index )
{
	if( index < 0 || index >= MAX_USER )
		return;

	m_User[ index ].LogOut();

}

void CUserManager::SendOther( int index, char* buff, int size )
{
	int i;
	for( i = 0; i < MAX_USER; ++i )
	{
		if( i == index )
			continue;

		m_User[ i ].Send( buff, size );
	}
}

void CUserManager::SendAll( char* buff, int size )
{
	int i;
	for( i = 0; i < MAX_USER; ++i )
	{
		m_User[i].Send( buff, size );
	}
}

int CUserManager::GetUserCount()
{
	int i;
	int nCount = 0;
	for( i = 0; i < MAX_USER; ++i )
	{
		if( m_User[i].m_hSocket == NULL )
			continue;
		nCount++;
	}
	return nCount;
}

