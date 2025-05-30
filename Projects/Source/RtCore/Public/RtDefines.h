#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

//#include "RtBluePrintCommonUtil.h"
//#include "RtDefines.h"

#define CALLINFO ( FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")") )
#define APP_LOG(fmt,...) UE_LOG(LogTemp, Warning, TEXT("%s %s\n"), *CALLINFO, *FString::Printf(TEXT(fmt), ##__VA_ARGS__))
#define SCREEN_LOG(fmt,...) if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT(fmt), ##__VA_ARGS__)); }
#define SCREEN_LOG_EX(time, fmt,...) if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, time, FColor::Yellow, FString::Printf(TEXT(fmt), ##__VA_ARGS__)); }
#define CUSTOM_FILE_APP_LOG(fmt,...) URtBluePrintCommonUtil::WriteToFileLog(TEXT("CustomFileLog.log"), *FString::Printf(TEXT(fmt), ##__VA_ARGS__))
#define CUSTOM_FILE_APP_LOG_EX(filename,fmt,...) URtBluePrintCommonUtil::WriteToFileLog(TEXT(filename), *FString::Printf(TEXT(fmt), ##__VA_ARGS__))
#define CLIENT_FILE_LOG(fmt,...) URtBluePrintCommonUtil::WriteToFileLog(TEXT("ClientFileLog.log"), *FString::Printf(TEXT(fmt), ##__VA_ARGS__))

// ���� ���忡���� ������ ���� �α� ��ũ���̹Ƿ� ���� �Ͽ� ����� ��! CUSTOM_FILE_APP_LOG_EX �� ���� ����
#define FILE_APP_LOG(filename,fmt,...) URtBluePrintCommonUtil::WriteToFileLog(TEXT(filename), *FString::Printf(TEXT(fmt), ##__VA_ARGS__))

#define STR(X) *FString(X)

#define IS_NULL(x) if( nullptr == x )
#define IS_NOT_NULL(x) if( nullptr != x )

#define R_HAND_SOCKET_NAME TEXT("r_hand_socket")
#define MOUTH_SOCKET_NAME TEXT("mouth_socket")
#define L_HAND_SOCKET_NAME TEXT("l_hand_socket")

#define RIGHT_HAND_BONE_NAME TEXT("hand_r")
#define HEAD_BONE_NAME TEXT("head")
#define LEFT_HAND_BONE_NAME TEXT("hand_l")

// Sct ID ���ξ� ����.
#define SCT_PREFIX_PLAYER_GROUP_ID TEXT("PGID_")
#define SCT_PREFIX_DROPBOX_ID TEXT("MSID_")
#define SCT_PREFIX_MENU_ID TEXT("MID_")
#define SCT_PREFIX_VARIABLE_ID TEXT("VAR_")
#define SCT_PREFIX_SUDDEN_EVENT_ID TEXT("SuddenEID_")
#define SCT_PREFIX_EVENT_ID TEXT("EID_")

// Sct Sentence Button Type Format
#define SENTENCE_BTN_TYPE_TAG TEXT("T:")

#define SENTENCE_BTN_TYPE_DEFAULT TEXT("Def")
#define SENTENCE_BTN_TYPE_OBJECT TEXT("Obj")
#define SENTENCE_BTN_TYPE_WAYPOINT TEXT("Way")
#define SENTENCE_BTN_TYPE_TIRGGER TEXT("Tri")
#define SENTENCE_BTN_TYPE_NUMBER TEXT("Num")
#define SENTENCE_BTN_TYPE_TEXT TEXT("Txt")
#define SENTENCE_BTN_TYPE_ANIMATION TEXT("Ani")
#define SENTENCE_BTN_TYPE_KEY TEXT("Key")
#define SENTENCE_BTN_TYPE_VARIABLE TEXT("Var")
#define SENTENCE_BTN_TYPE_COLOR TEXT("Clr")
#define SENTENCE_BTN_TYPE_SELECT TEXT("Sel")
#define SENTENCE_BTN_TYPE_SOUND TEXT("Snd")

#define CLIENT_PROJECT_DATA_PATH TEXT("TrainingData")
#define CLIENT_PROJECT_SYSTEM_DATA_PATH TEXT("TrainingSystemData")

#define EVALUATION_CHECK_TIME_INTERVAL 1.0f

#define TOOL_SYSTEM_FILE_NAME_PLAYER_START TEXT("PlayerStart")
#define TOOL_SYSTEM_FILE_NAME_WAY_POINT TEXT("WayPoint")
#define TOOL_SYSTEM_FILE_NAME_INVISIBLE_WALL TEXT("InvisibleWall")
#define TOOL_SYSTEM_FILE_NAME_MAP_BOUNDARY TEXT("MapBoundary")
#define TOOL_SYSTEM_FILE_NAME_TRIGGER_BOX TEXT("TriggerBox")
#define TOOL_SYSTEM_FILE_NAME_AREA TEXT("Area")

#define TOOL_SYSTEM_FILE_NAME_MAP_BOUNDARY_OBJ1 TEXT("MapBoundary_1")

#define TOOL_SYSTEM_FBX_PATH_DEFAULT_ANIM_IDLE			TEXT("System/SysDefaultAnimFiles/default_idle.FBX")
#define TOOL_SYSTEM_FBX_PATH_DEFAULT_ANIM_WALK			TEXT("System/SysDefaultAnimFiles/default_walk.FBX")
#define TOOL_SYSTEM_FBX_PATH_DEFAULT_ANIM_RUN			TEXT("System/SysDefaultAnimFiles/default_run.FBX")
#define TOOL_SYSTEM_FBX_PATH_DEFAULT_ANIM_ATTACK		TEXT("System/SysDefaultAnimFiles/default_attack.FBX")
#define TOOL_SYSTEM_FBX_PATH_DEFAULT_ANIM_DEFEND		TEXT("System/SysDefaultAnimFiles/default_defend.FBX")
#define TOOL_SYSTEM_FBX_PATH_DEFAULT_ANIM_COLLAPSE		TEXT("System/SysDefaultAnimFiles/default_collapse.FBX")
#define TOOL_SYSTEM_FBX_PATH_DEFAULT_ANIM_SURRENDER		TEXT("System/SysDefaultAnimFiles/default_surrender.FBX")
#define TOOL_SYSTEM_FBX_PATH_DEFAULT_ANIM_LIE_DOWN		TEXT("System/SysDefaultAnimFiles/default_lie_down.FBX")
#define TOOL_SYSTEM_FBX_PATH_DEFAULT_ANIM_RESTRAIN		TEXT("System/SysDefaultAnimFiles/default_restrain.FBX")
#define TOOL_SYSTEM_FBX_PATH_DEFAULT_ANIM_STAND_UP		TEXT("System/SysDefaultAnimFiles/default_stand_up.FBX")
#define TOOL_SYSTEM_FBX_PATH_DEFAULT_ANIM_ESCORT		TEXT("System/SysDefaultAnimFiles/default_escort.FBX")

#define TOOL_SYSTEM_FBX_ANIMATION_MAX_SPEED 300
#define TOOL_SYSTEM_FBX_ANIMATION_DEFAULT_SPEED 100
#define TOOL_SYSTEM_DEFAULT_MOVE_SPEED 100
#define GET_SPEEDVALUE_FROM_SPEEDFACTOR(Factor) ((float)Factor / (float)TOOL_SYSTEM_FBX_ANIMATION_DEFAULT_SPEED)

#define TOOL_SYSTEM_FBX_PATH_DEFAULT_CHARACTER1 TEXT("System/SysDefaultModelFiles/DefaultCharacter1.FBX")

#define PROJECT_PATH_TCTFILES TEXT("/TctFiles")
#define PROJECT_PATH_MAPFILES TEXT("/MapFiles")
#define PROJECT_PATH_PNGFILES TEXT("/PngFiles")
#define PROJECT_PATH_WAVFILES TEXT("/WavFiles")
#define PROJECT_PATH_FBXFILES TEXT("/FbxFiles")

#define PROJECT_PATH_FBX_AINM_FILES TEXT("/FbxAnimFiles")
#define PROJECT_PATH_EFFECT_FILES TEXT("/EffectFiles")

#define TRAINEE_STRINGTABLE_PATH TEXT("/Game/RS/Common/Table/ST_Trainee.ST_Trainee")

// ���̾ư��� ���� ȿ�� �ּ�
#define K2C1_FIRE_EFFECT1_PATH TEXT("/Game/Gun_VFX/Niagara_FX/Muzzle_Flashes/NS_AR_Muzzle_flashes_01.NS_AR_Muzzle_flashes_01")
#define K2C1_FIRE_EFFECT2_PATH TEXT("/Game/Gun_VFX/Niagara_FX/Muzzle_Flashes/NS_AR_Muzzle_flashes_01.NS_AR_Muzzle_flashes_02")

// TCT ������ ���� �� ���ϵ��� ���� ������ �߰��ȴ�.
//#define TCT_FILE_VERSION 1
#define TCT_FILE_VERSION_COLLISION_PIVOT 2 // �ø��� Pivot �߽��࿡�� Z�� �ٴ����� �̵�.
#define TCT_FILE_VERSION TCT_FILE_VERSION_COLLISION_PIVOT // �ø��� Pivot �߽��࿡�� Z�� �ٴ����� �̵�.

#define MCT_FILE_VERSION 1
#define SCT_FILE_VERSION 1

#define BUILD_TEMPORARY 0 // �ӽ÷� �ڵ� �۾��� �� �� ���
#define PROJECT_TEMPORARY TEXT("ETXR UnderDev") // �ӽ÷� ���� ��� �� �� ���Ǵ� �̸�

#define PROJECT_NAME TEXT("ETXR Training") // �⺻������ ���Ǵ� �̸�
//#define PROJECT_VERSION TEXT("0.9.0") // ���� ����
//#define PROJECT_VERSION TEXT("0.9.1") // ���� �ִϸ��̼� �⺻ ���� ���� ����( ���� �ǰ� )
//#define PROJECT_VERSION TEXT("0.9.2") // ���� ���� ���ö� PDB �߰��ϱ�
//#define PROJECT_VERSION TEXT("0.10.0") // �Ȱ� ��� �߰�.
//#define PROJECT_VERSION TEXT("0.10.1") // ���� ���� ����.
//#define PROJECT_VERSION TEXT("0.11.0") // �߰� Sct �Լ��� ����.
//#define PROJECT_VERSION TEXT("0.12.0") // NPC ��ǥ ������Ʈ ��� ����.
//#define PROJECT_VERSION TEXT("0.13.1") // ���� 5.3 ���� ���� ����.
//#define PROJECT_VERSION TEXT("0.13.2") // ���� ��û ó�� ���� ����.
//#define PROJECT_VERSION TEXT("0.14.0") // Sct ������ �޴� On/Off ��� �߰�, Sct App.log ��� �α� ����.
//#define PROJECT_VERSION TEXT("0.14.1") // ���߿� Ű�� �������� �� ��ġ �ȸ´� ���� ����.
#define PROJECT_VERSION TEXT("0.15.0") // Sct �Լ��� �߰�, ��Ÿ ��� ���׷��̵�.

#define SINGLE_NAME TEXT("MctSctPreviewer") // �̱۷� ���� �� �̸�

// [ ���� ���� ]
// 
// APP_LOG("%s %d %f", STR("test"), 10, 3.141592f);
//


//#define UserParam_Pos FName("User_Position")