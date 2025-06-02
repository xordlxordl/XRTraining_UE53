#pragma once

#include "MyInclude.h"

extern HWND g_ListWnd;

extern CIni g_Ini;
extern int g_UDPPort;
extern int g_SendToME;
extern HFONT g_ArialFont;

extern char g_HostName[128];
extern char g_IP[ 20 ];

extern CUDPServer g_UDP[ MAX_UDP ];

void Log( const char* format, ... );
void DebugLog( char* filename, const char* format, ... );