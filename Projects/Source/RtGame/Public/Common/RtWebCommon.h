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
	LOG_FIRE,                // 사격
	LOG_HIT,                 // 적중
	LOG_CHALLENGE,           // 수하
	LOG_SURRENDER,           // 투항명령
	LOG_BIND,                // 포박
	LOG_COMMANDMOVE,         // 이동지시
	LOG_BREIFSITUATION,      // 상황보고
	LOG_CALL,                // 증원요청
	LOG_EOD,			     // EOD 요청
	LOG_BADFIRE,             // 오인사격
	//-----------------------------------------------------

	LOG_TR_HMD,              // transform  x y z p r w {"x":"0.0","y":"0.0","z":"0.0","p":"0.0","r":"0.0","w":"0.0"}
	LOG_TR_GUN,              // transform  x y z p r w {"x":"0.0","y":"0.0","z":"0.0","p":"0.0","r":"0.0","w":"0.0"}
	
	LOG_DISTANCE,            // float 훈련 종료 시 클라 -> 웹로그 호출

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
	ERtWebProtocolType ErrorType; //저장된 결과 ID
	ERtWebProtocolType ResponseError; // blueprint 용으로 가공된 error type

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
struct RTGAME_API FResponse_TrngSimpleResult  // 리스트에 사용되는 요약 결과 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 TID;	          //TID

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Name;         // 훈련자 명

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Role;         // 훈련 보직 .

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString PlayTimeSec;	  // 훈련 시간 

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Distance;       // 이동 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString FireCnt;        // 사격 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString HitCnt;         // 적중 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString ChallengeCnt;   // 수하 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString SurrenderCnt;   // 투항 명령 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString BindCnt;        // 포박 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString CommandMoveCnt; // 이동 지시 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString BreifCnt;       // 상황 보고 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString CallCnt;        // 증원 요청 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString EODCnt;         // EOD 요청 횟수 
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString BadFireCnt;     // 오인 사격 횟수

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString ReplayFileName; //"ReplayFileName":
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString ReplayID; // "ReplayID": 

	FResponse_TrngSimpleResult() {
		
	}
};
