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

struct stReady : public stHeader
{
	int32 UID;
	bool IsReady;
	stReady()
	{
		UID = 0;
		IsReady = false;
		SetHeader(prReady, sizeof(stReady));
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
	int32 TID;
	stStartReq()
	{
		TID = 0;
		SetHeader( prStartReq, sizeof( stStartReq ) );
	}
};
struct stStartAck : public stHeader
{
	int32 TID;
	stStartAck()
	{
		TID = 0;
		SetHeader( prStartAck, sizeof( stStartAck ) );
	}
};


struct stFinish : public stHeader
{
	int32 TID;
	stFinish()
	{
		TID = 0;
		SetHeader( prFinish, sizeof( stFinish ) );
	}
};


struct stAccident : public stHeader
{
	int16 AccidentName[64];
	stAccident() {
		memset(AccidentName, 0x00, sizeof(AccidentName));
		SetHeader(prAccident, sizeof(stAccident));
	}
};

struct stSctEvent : public stHeader
{
	int16 SctEventID[64];
	stSctEvent()
	{
		memset(SctEventID, 0x00, sizeof(SctEventID));
		SetHeader(prSctEvent, sizeof(stSctEvent));
	}
};

struct stInteractKey : public stHeader
{
	int32 PlayerID = 0;
	stInteractKey()
	{
		SetHeader(prInteractKey, sizeof(stInteractKey));
	}
};

struct stTraineeAim : public stHeader
{
	int32 PlayerID = 0;
	int16 ObjectID[64];
	stTraineeAim()
	{
		memset(ObjectID, 0x00, sizeof(ObjectID));
		SetHeader(prTraineeAim, sizeof(stTraineeAim));
	}
};


#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
struct stTraineeShot : public stHeader
{
	int16 WID[64]; // Weapon ID
	int16 HOID[64]; // Hit Object ID
	FVector HPos = FVector::ZeroVector; // Hit Position
	FVector SPos = FVector::ZeroVector; // Shot Position
	FVector SDir = FVector::ZeroVector; // Shot Direction
	bool Amm = false; // Is Live Ammunition (실탄인가?)

	stTraineeShot()
	{
		memset(WID, 0x00, sizeof(WID));
		memset(HOID, 0x00, sizeof(HOID));
		SetHeader(prTraineeShot, sizeof(stTraineeShot));
	}
};
#endif

struct stDayNight : public stHeader
{
	bool IsNight;
	stDayNight()
	{
		IsNight = false;
		SetHeader(prDayNight, sizeof(stDayNight));
	}
};

struct stRestartSame : public stHeader
{
	stRestartSame()
	{
		SetHeader(prRestartSame, sizeof(stRestartSame));
	}
};

struct stRestartAll : public stHeader
{
	stRestartAll()
	{
		SetHeader(prRestartAll, sizeof(stRestartAll));
	}
};

struct stNpcState : public stHeader
{
	int16 NpcID[64];
	int8 SctFunCmd = 0;
	int8 State = 0; // AnimType
	float Speed = 0.0f;
	union {
		int16 SctString[64]; // TargetID
		float SctFloat = 0.0f; // Free Move Location X
	} Data1;	
	union {
		int16 SctString[64]; // AnimName
		float SctFloat = 0.0f; // Free Move Location Y
	} Data2;	
	union {
		int16 SctString[64]; // EctString
		float SctFloat = 0.0f; // Free Move Location Z
		bool SctBool; // IsAnimLoop
	} Data3;	

	stNpcState()
	{
		memset(NpcID, 0x00, sizeof(NpcID));
		memset(Data1.SctString, 0x00, sizeof(Data1.SctString));
		memset(Data2.SctString, 0x00, sizeof(Data2.SctString));
		memset(Data3.SctString, 0x00, sizeof(Data3.SctString));
		SetHeader(prNpcState, sizeof(stNpcState));
	}
};

//TOOL--------------------------------------------------------------------------------

#pragma pack()
