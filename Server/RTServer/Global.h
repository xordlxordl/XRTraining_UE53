#pragma once

#include "MyInclude.h"

//
extern HWND g_MainWnd;
extern HWND ListboxWindow;
extern HFONT ArialFont;
extern int g_UserPer;
extern int MAX_QUEUE;
extern int MAX_THREAD;
extern int g_bUseUDP;

extern int g_bUDPTest;
extern int g_bUseTime;

//TCP
extern char g_HostName[ 256 ];
extern char g_IP[ 20 ];
extern int g_ServerPort;
extern SOCKET g_ServerSocket;
extern HANDLE g_ServerHandle;
extern int MAX_THREAD;
extern WSAEVENT g_Event[ MAX_USER ];
extern HANDLE g_UserHandle[ MAX_USER ];
extern int g_ServerNum;

extern HANDLE g_TimeHandle;

//UDP
extern CUDPServer g_UDP[ MAX_UDP ];
extern int g_UDPPort;

extern CIni g_Ini;

extern CUserManager g_User;
extern CPlayScenario g_Play;
//
void Log( const char* format, ... );
void DebugLog( char* filename, const char* format, ... );




string AnsiToUtf8( string strAnsi );
string Utf8ToAnsi( string strUTF8 );
wstring Utf8ToWchar( string strAnsi );
string WcharToAnsi( wstring strWchar );
string WcharToUtf8( wstring strWchar );

#define SAFE_DELETE(p) { if(p) { delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete [] (p); (p) = NULL; } }

#define SAFE_CLOSESOCK(p){ if(p) { closesocket(p);(p) = NULL; }}