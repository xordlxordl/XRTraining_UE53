using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

public enum enFileType
{
    SCT = 1,
    MCT,
    WCT,
    TCT,
    FBX,
    PNG,
    WAV,
    FBXANIM,
    EFF,
    MAX_FILE,
};

//모든 로그 패킷에는 기본적으로 tid uid 가 있어야 됨
//{"tid":"1", "uid":"1"}

public enum enLogProtocol
{
    LOG_START = 0,
    LOG_FIRE,                // 사격 
    LOG_HIT,                 // 적중
    LOG_CHALLENGE,           // 수하
    LOG_SURRENDER,           // 투항명령
    LOG_BIND,                // 포박
    LOG_COMMANDMOVE,         // 이동지시
    LOG_BREIFSITUATION,      // 상황보고
    LOG_CALL,                // 증원요청
    LOG_EOD,                 // EOD 요청
    LOG_BADFIRE,              // 오인사격
    // Protocol TID UID
                              //----------------------------------------------------
    LOG_TR_HMD,              // transform  x y z p r w {"x":"0.0","y":"0.0","z":"0.0","p":"0.0","r":"0.0","w":"0.0"}
    LOG_TR_GUN,              // transform  x y z p r w {"x":"0.0","y":"0.0","z":"0.0","p":"0.0","r":"0.0","w":"0.0"}

    LOG_DISTANCE,            // float 훈련 종료 시 클라 -> 웹로그 호출  {"dist":"0.0"}

    LOG_MOTION,             // tid uid motion( json )
    LOG_OBJECT,             // tid motion( json )
    LOG_END
};


public enum enWEBProtocol
{
	webErrorServerCheck = -5,
    webErrorBlock = -4,
    webError = -3,
    webErrorDup = -2,
    webErrorParse = -1,
    WEB_PROTOCOL_START = 0,
    //-----------------------------------------------------
    //프로토콜 번호와 파일 타입 번호를 마추기 위해 변경

    //파일 목록 프로토콜
    webGetSCTList,  //1
    webGetMCTList,  //2
    webGetWCTList,  //3
    webGetTCTList,  //4
    webGetFBXList,  //5
    webGetPNGList,  //6
    webGetWAVList,  //7
    webGetFBXAnimList,  //8
    webGetEffList,      //9
    webGetAllFileList,

    //-----------------------------------------------------
    //기능
    webLogin,
    webGetMachineList,
    webGetUserList,

    webStart,
    webFinish,
    webEnterUser,
    webTrainingUserList,
    webTrainingDetail,
    //-----------------------------------------------------
    webGetMotionLog,
    webGetVoiceList,
    webGetObjectLog,
    //-----------------------------------------------------

    webCheckFile,


    WEB_PROTOCOL_END,
};
