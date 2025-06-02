#include "MyInclude.h"

#include "MyFileDown.h"

CUser::CUser( void )
{
	m_hSocket = NULL;
	m_RecvWrite = 0;

	m_SendSize = 0;

	m_UID = 0;
	m_Index = INVALID_VALUE;
	m_nGrade = 100;

	memset( m_Name, 0x00, sizeof( m_Name ) );
	
	
}

CUser::~CUser( void )
{
	Clear();
}

void CUser::Clear()
{
	if( m_hSocket != NULL )
	{
		shutdown( m_hSocket, SD_SEND );
		shutdown( m_hSocket, SD_RECEIVE );
	}
	SAFE_CLOSESOCK( m_hSocket );

	m_RecvWrite = 0;
	memset( m_RecvBuffer, 0x00, sizeof( m_RecvBuffer ) );

	m_SendSize = 0;
	memset( m_SendBuffer, 0x00, sizeof( m_SendBuffer ) );

	memset( m_Name, 0x00, sizeof( m_Name ) );

	m_UID = 0;
	m_nGrade = ROLE_END;
	m_Index = INVALID_VALUE;
	
	m_ThreadNum = INVALID_VALUE;
	
	
}

bool CUser::Init( int index, SOCKET sock, sockaddr_in ip )
{
	if( index < 0 || index >= MAX_USER )
		return false;

	Clear();

	m_hSocket = sock;
	m_Index = index;

	m_ThreadNum = index / g_UserPer;

	

	SendConnect();

	int t = m_ThreadNum;
	if( t < 0 || t > MAX_QUEUE )
		return false;

	return true;

}


void CUser::LogOut()
{
	//
	if( m_hSocket )
	{
		int t = m_ThreadNum;
		if( t < 0 || t > MAX_QUEUE )
			return;

	}
	Clear();
}

void CUser::EmptyRecvBuffer()
{
	memset( m_RecvBuffer, 0x00, sizeof( m_RecvBuffer ) );
	m_RecvWrite = 0;
}

int CUser::FlushSendBuffer()
{

	int sendsize = 0;
	do
	{
		sendsize = send( m_hSocket, m_SendBuffer, m_SendSize, 0 );
		if( sendsize == SOCKET_ERROR )
		{
			unsigned int erro = WSAGetLastError();
			return erro;
		}
		else
		{
			memmove( m_SendBuffer, &m_SendBuffer[ sendsize ], m_SendSize - sendsize );
			m_SendSize -= sendsize;
		}
	} while( m_SendSize );

	return m_SendSize;
}

bool CUser::AddSendBuffer( char* buff, int size )
{
	if( buff == NULL )
		return false;
	if( m_SendSize + size >= MAX_SEND )
	{
		Clear();
		return false;
	}

	memcpy( &m_SendBuffer[ m_SendSize ], buff, size );
	m_SendSize += size;
	return true;
}

bool CUser::CheckObjID( char* id )
{
	if( m_ObjID.compare( id ) == 0 )
		return true;

	return false;
}

void CUser::Send( char* buff, int size )
{
	if( m_hSocket == NULL )
	{
		return;
	}

	if( buff == NULL )
		return;

	int sendsize, error = 0;
	if( m_SendSize <= 0 ) 
	{
		do
		{
			sendsize = send( m_hSocket, buff, size, 0 );

			if( sendsize < 0 )
			{
				AddSendBuffer( buff, size );
				break;
			}
			else
			{
				buff = buff + sendsize; // 
				size -= sendsize;		// 
			}
		} while( size );//Size
	}
	else //
	{
		if( AddSendBuffer( buff, size ) )
		{
			FlushSendBuffer();
		}
		else
		{
			FlushSendBuffer();
		}
	}
}


void CUser::Recv()
{
	if( m_hSocket == NULL )
		return;
	if( m_Index < 0 )
		return;

	int size = 0;
	if( m_RecvWrite < MAX_RECV )
		size = recv( m_hSocket, &m_RecvBuffer[ m_RecvWrite ], MAX_RECV - m_RecvWrite, 0 );

	if( size > 0 )
	{
		
		m_RecvWrite += size;

		if( m_RecvWrite >= MAX_RECV )
		{
			Log( "RecvBuffer MAX.." );
		}

		while( m_RecvWrite >= HEADSIZE )
		{

			stHeader header;
			memcpy( &header, m_RecvBuffer, HEADSIZE );
			/*
			if( header.nID >= PROTOCOL_END || header.nID <= PROTOCOL_START )
			{
				//Clear();
				EmptyRecvBuffer();
				return;
			}
			*/
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
void CUser::Parse( int protocol, char* packet )
{
	switch( protocol )
	{
	case prLoginReq:	RecvLoginReq( packet );		break;
	
	case prSetSCTReq:	RcvSetSCTReq( packet );		break;
	case prStartReq:	RecvStartReq( packet );		break;

	default:			SendDefault( packet );		break;
	}
}

void CUser::RecvStartReq( char* packet )
{

	stStartReq req;

	memcpy( &req, packet, sizeof( stStartReq ) );

	stStartAck ack;

	ack.TID = req.TID;

	char buffer[64];	memset( buffer, 0x00, sizeof( buffer ) );
	memcpy( buffer, &ack, sizeof( stStartAck ) );
	g_User.SendAll( buffer, sizeof( stStartAck ) );

}

void CUser::RecvLoginReq( char* packet )
{
	stLoginReq req;
	memcpy( &req, packet, sizeof( stLoginReq ) );

	int grade = req.grade;

	stLoginAck ack;
	char buffer[ 64 ];	memset( buffer, 0x00, sizeof( buffer ) );


	if( grade < ROLE_USER && grade > ROLE_START )
	{
		m_nGrade = grade;
		ack.nResult = 1;

		memcpy( buffer, &ack, sizeof( stLoginAck ) );
		Send( buffer, sizeof( stLoginAck ) );
		return;
	}

	
/*
	if( g_Play.IsSctObjectID( req.objID ) )
	{
		m_ObjID = req.objID;
		ack.nResult = 1;
	}
	else
	{
		ack.nResult = 0;
	}
*/
	ack.nResult = 1;
	if( grade > ROLE_START && grade < ROLE_END )
	{
		m_nGrade = grade;
	}

	

	memcpy( buffer, &ack, sizeof( stLoginAck ) );
	Send( buffer, sizeof( stLoginAck ));

	SendSCT();

}
void CUser::SendSCT()
{
	if( g_Play.m_SCTFileName.size() <= 0 )
		return;

	char buffer[ 1024 ];	memset( buffer, 0x00, sizeof( buffer ) );

	stSetSCTAck ack;
	
	//sprintf( ack.filename, g_Play.m_SCTFileName.c_str() );

	swprintf( ( wchar_t* ) ack.filename, ( wchar_t* ) g_Play.m_WFileName );

	memcpy( buffer, &ack, sizeof( stSetSCTAck ) );
	

}

void CUser::RcvSetSCTReq( char* packet )
{
	stSetSCTReq req;
	memcpy( &req, packet, sizeof( stSetSCTReq ) );
	
	
	char buffer[1024];	memset( buffer, 0x00, sizeof( buffer ) );

	stSetSCTAck ack;
	swprintf( (wchar_t*)ack.filename, ( wchar_t* ) req.filename );
	
	
	//sprintf( ack.filename, req.filename );
	memcpy( buffer, &ack, sizeof( stSetSCTAck ) );

	g_User.SendAll( buffer, sizeof( stSetSCTAck ) );

	wstring wfilename = (wchar_t*)req.filename;
	string filename = WcharToAnsi( wfilename );;

	g_Play.m_SCTFileName = filename;
	swprintf( ( wchar_t* ) g_Play.m_WFileName, ( wchar_t* ) req.filename );

	/*
	if( g_pHttp->DownLoad( filename ) )
	{
		SendMessage( g_MainWnd, WM_LOADSCT, 0, 0 );
	}
	*/
	
}

void CUser::SendDefault( char* packet )
{
	stHeader header;
	memcpy( &header, packet, HEADSIZE );

	int size = header.nSize;
	g_User.SendAll( packet, size );


}

void CUser::SendConnect()
{
	stConnectAck ack;
	ack.Index = m_Index;
	ack.UDPPort = g_UDPPort + m_Index;

	char buffer[ 128 ];	memset( buffer, 0x00, sizeof( buffer ) );
	memcpy( buffer, &ack, sizeof( stConnectAck ) );
	Send( buffer, sizeof( stConnectAck ) );

}