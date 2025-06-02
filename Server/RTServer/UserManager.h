#pragma once
#include "User.h"
class CUserManager
{
public:
	CUser m_User[ MAX_USER ];
	int m_PreIndex;
	int m_ChatIndex;

	bool m_bStart;

public:
	CUserManager(void);
	~CUserManager(void);

	bool AddUser( SOCKET sock,  sockaddr_in ip );
	void DelUser( int index );
	CUser* GetUser( int uid );

	void SendOther( int index, char* buff, int size );
	void SendAll( char* buff, int size );

	void SendMsg( char* buff, int size, int FID );

	int GetUserCount();

	
	

};
