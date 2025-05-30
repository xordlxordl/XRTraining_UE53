// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Data
#include "RtRpcCommon.h"
#include "RtWebCommon.generated.h"

UENUM()
enum class ELogProtocol	
{
	// protocol, TID , UID
	LOG_START = 0 ,               
	LOG_FIRE,                // ���
	LOG_HIT,                 // ����
	LOG_CHALLENGE,           // ����
	LOG_SURRENDER,           // ���׸��
	LOG_BIND,                // ����
	LOG_COMMANDMOVE,         // �̵�����
	LOG_BREIFSITUATION,      // ��Ȳ����
	LOG_CALL,                // ������û
	LOG_EOD,			     // EOD ��û
	LOG_BADFIRE,             // ���λ��
	//-----------------------------------------------------

	LOG_TR_HMD,              // transform  x y z p r w {"x":"0.0","y":"0.0","z":"0.0","p":"0.0","r":"0.0","w":"0.0"}
	LOG_TR_GUN,              // transform  x y z p r w {"x":"0.0","y":"0.0","z":"0.0","p":"0.0","r":"0.0","w":"0.0"}
	
	LOG_DISTANCE,            // float �Ʒ� ���� �� Ŭ�� -> ���α� ȣ��

	LOG_MOTION,             // tid uid motion( json )
	LOG_OBJECT,             // tid motion
	LOG_END 
};

enum class ERtWebProtocolType
{
	webErrorServerCheck = -5,
	webErrorBlock = -4,
	WebError = -3,	//webError
	webErrorDup = -2,
	webErrorParse = -1,
	WEB_PROTOCOL_START = 0,
	//-----------------------------------------------------

	webGetSCTList,
	webGetMCTList,
	webGetWCTList,
	webGetTCTList,
	webGetFBXList,
	webGetPNGList,
	webGetWAVList,
	webGetFBXAnimList,  //8
	webGetEffList,
	webGetAllFileList, // 

	webLogin,  //webLogin  11 
	webGetMachineList,
	webGetUserList,       

	webStart,            // 14
	webFinish,            
	webEnterUser,
	webTrainingUserList,     
	webTrainingDetail,
	//-----------------------------------------------------
	webGetMotionLog,   //19, TID, UID
	webGetVoiceList,   // TID, recv i
	webGetObjectLog,   // TID
	WEB_PROTOCOL_END,  //WEB_PROTOCOL_END
};


USTRUCT(BlueprintType)
struct RTGAME_API FResponse_Login_Result {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString InID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString InPW;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Result;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString UID;

	FResponse_Login_Result() :Result(""), Grade(""), UID("")
	{}
};

USTRUCT()
struct RTGAME_API FResponse_Data
{
	GENERATED_BODY()

public:

	FResponse_Login_Result Login_Result;
	TArray<FRtTraineeInfo> UserArray;
	TArray<FRtDeviceInfo> DeviceArray;
	TArray<FResponse_ReplayUserList> ReplayArray;
	TArray<FResponse_TrngSimpleResult>  TrainingSimpleResultArray;
	FResponse_Data() {}
};

USTRUCT()
struct RTGAME_API FResponse_Result
{
	GENERATED_BODY()

public:
	ERtWebProtocolType ProtocolType; //nID
	ERtWebProtocolType ErrorType; //����� ��� ID
	ERtWebProtocolType ResponseError; // blueprint ������ ������ error type

	FResponse_Result()
		:ProtocolType(ERtWebProtocolType::WEB_PROTOCOL_END), ErrorType(ERtWebProtocolType::WEB_PROTOCOL_START), ResponseError(ERtWebProtocolType::WEB_PROTOCOL_START) {
	}

	void Reset() {
		ProtocolType = ERtWebProtocolType::WEB_PROTOCOL_END;
		ErrorType = ERtWebProtocolType::WEB_PROTOCOL_START;
		ResponseError = ERtWebProtocolType::WEB_PROTOCOL_START;
	}

	static ERtWebProtocolType Get_Protocol(const FString& InStr);
	static ERtWebProtocolType Get_Error(const FString& InStr);

	void Set_ProtocolType(const FString& InStr);
	void Set_ErrorType(const FString& InStr);

	void Set_Type(const FString& InProtocol, const FString& InError);
};

USTRUCT(BlueprintType)
struct RTGAME_API FResponse_ReplayUserList
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Role;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString TraineeName;

	FResponse_ReplayUserList() {}
	FResponse_ReplayUserList(FString InRole, FString InTraineeName) {
		Role = InRole;
		TraineeName = InTraineeName;
	}
};

USTRUCT(BlueprintType)
struct RTGAME_API FResponse_TrngSimpleResult  // ����Ʈ�� ���Ǵ� ��� ��� 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 TID;	          //TID

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Name;         // �Ʒ��� ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Role;         // �Ʒ� ���� .

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString PlayTimeSec;	  // �Ʒ� �ð� 

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Distance;       // �̵� �Ÿ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString FireCnt;        // ��� Ƚ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString HitCnt;         // ���� Ƚ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString ChallengeCnt;   // ���� Ƚ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString SurrenderCnt;   // ���� ��� Ƚ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString BindCnt;        // ���� Ƚ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString CommandMoveCnt; // �̵� ���� Ƚ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString BreifCnt;       // ��Ȳ ���� Ƚ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString CallCnt;        // ���� ��û Ƚ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString EODCnt;         // EOD ��û Ƚ�� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString BadFireCnt;     // ���� ��� Ƚ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString ReplayFileName; //"ReplayFileName":
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString ReplayID; // "ReplayID": 

	FResponse_TrngSimpleResult() {
		
	}
};
