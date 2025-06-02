#pragma once

#include "CoreMinimal.h"
#include "RTProtocol.h"

#pragma pack(1)
struct stConnectAck : public stHeader
{
	int32 UDPPort;
	int32 Index;

	stConnectAck()
	{
		UDPPort = 0;
		Index = 0;

		SetHeader( prConnectAck, sizeof( stConnectAck ) );
	};
};

struct stLoginReq : public stHeader
{
	int32 grade;
	int8 objID[ 64 ];

	stLoginReq()
	{
		grade = ROLE_USER;
		memset( objID, 0x00, sizeof( objID ) );
		SetHeader( prLoginReq, sizeof( stLoginReq ) );
	}
};

struct stLoginAck : public stHeader
{
	//1이 아니면 실패  예) -1 objID 가 SCT 그룹 리스트에 없음
	int32 nResult;

	stLoginAck()
	{
		nResult = 0;

		SetHeader( prLoginAck, sizeof( stLoginAck ) );
	}
};


struct stSetSCTReq : public stHeader
{
	int16 filename[ 64 ];
	stSetSCTReq()
	{
		memset( filename, 0x00, sizeof( filename ) );
		SetHeader( prSetSCTReq, sizeof( stSetSCTReq ) );
	}
};


struct stSetSCTAck : public stHeader
{
	int16 filename[ 64 ];
	stSetSCTAck()
	{
		memset( filename, 0x00, sizeof( filename ) );
		SetHeader( prSetSCTAck, sizeof( stSetSCTAck ) );
	}
};

struct stStartReq : public stHeader
{
	int TID;
	stStartReq()
	{
		TID = 0;
		SetHeader( prStartReq, sizeof( stStartReq ) );
	}
};
struct stStartAck : public stHeader
{
	int TID;
	stStartAck()
	{
		TID = 0;
		SetHeader( prStartAck, sizeof( stStartAck ) );
	}
};


//TOOL--------------------------------------------------------------------------------

#pragma pack()
