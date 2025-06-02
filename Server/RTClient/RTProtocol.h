#pragma once
#pragma pack(1)

#include "CoreMinimal.h"

#define GAME_ID 1818

enum enRole
{
	ROLE_START = 0,
	ROLE_ADMIN,
	ROLE_TRANDER,
	ROLE_USER,
	ROLE_END
};

enum enProtocol
{
	PROTOCOL_START = 0,
					prConnectAck,
	prLoginReq,		prLoginAck,
	prSetSCTReq,	prSetSCTAck,
	prStartReq,		prStartAck,
	PROTOCOL_END,
};


struct stHeader
{
	uint16 nID;
	uint16 nSize;
	uint16 nType;
	uint16 nCheckSum;

	stHeader()
	{
		nID = nSize = nType = nCheckSum = 0;
	};
	void SetHeader( int id, int len )
	{
		nType = GAME_ID;
		nID = id;
		nSize = len;
		nCheckSum = nType + nID + nSize;
	};

#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1

	friend FArchive& operator<<( FArchive& Ar, stHeader& Header )
	{
		Ar << Header.nID;
		Ar << Header.nSize;
		Ar << Header.nType;
		Ar << Header.nCheckSum;
		return Ar;
	}

#endif
};



#define HEADSIZE sizeof( stHeader )

#pragma pack()
