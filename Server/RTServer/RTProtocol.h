#pragma once
#pragma pack(1)

#include "CoreMinimal.h"

#define GAME_ID 1818

enum enErrorCode
{
	ERROR_NODEVICE = -43,
	ERROR_NOTRAINEE = -42,
	ERROR_ALREADY_RESULT = -41, // There is Result of Training already.
	ERROR_NOTRAININGNUMBER = -40,
	ERROR_DUP_ROLE = -7, // Duplicate Role ID
	ERROR_DUP_TRAINEE = -6, // Duplicate Trainee (UID)
	ERROR_DUO_NUMBER = -5 , // Duplicate Army Number 
	ERROR_PASSWORD = -4,
	ERROR_ACCOUNT = -2,   // No Account
	ERROR_DUP_ACCOUNT = -1,
	FAIL = 0,
	SUCCESS = 1 
};


enum enRole
{
	ROLE_START = 0,
	ROLE_ADMIN,
	ROLE_TRANDER,
	ROLE_OBSERVER,
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
	prFinish,       prReady,
	prAccident,
	prSctEvent,		prInteractKey,
	prTraineeAim,	prTraineeShot,
	prDayNight,
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

struct stUDPPacket
{

	double v[3][6];
	double q[3][6];
	uint32 did;

	stUDPPacket()
	{
		did = 0;
		memset( v, 0, sizeof( v ) );
		memset( q, 0, sizeof( q ) );
	}

};


#define HEADSIZE sizeof( stHeader )

#pragma pack()
