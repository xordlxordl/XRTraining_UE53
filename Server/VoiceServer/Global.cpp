#include "Global.h"

HWND g_ListWnd = NULL;

CIni g_Ini( "Ini.ini" );

int g_UDPPort = 0;
int g_SendToME = 0;

HFONT g_ArialFont;

CUDPServer g_UDP[ MAX_UDP ];

char g_HostName[ 128 ];
char g_IP[ 20 ];


int logcount = 0;

void Log( const char* format, ... )
{
    va_list argptr;
    char msg[ 4096 ]; memset( msg, 0x00, sizeof( msg ) );
    va_start( argptr, format );
    vsprintf( msg, format, argptr );
    va_end( argptr );

    //SendMessage( ListboxWindow, LB_ADDSTRING, 0, (LPARAM)msg );
    int count = 0;
    count = SendMessage( g_ListWnd, LB_GETCOUNT, 0, 0 );
    if( count > 1000 )
    {
        SendMessage( g_ListWnd, LB_RESETCONTENT, 0, 0 );
        logcount++;
    }

    SendMessage( g_ListWnd, LB_INSERTSTRING, 0, ( LPARAM ) msg );
    char buffer[ 1024 ];	memset( buffer, 0x00, sizeof( buffer ) );
    sprintf( buffer, "Log\\Log%d.txt", logcount );
    DebugLog( buffer, "%s\n", msg );
}

void DebugLog( char* filename, const char* format, ... )
{
    va_list argptr;
    char msg[ 10000 ];	memset( msg, 0x00, sizeof( msg ) );
    va_start( argptr, format );
    vsprintf( msg, format, argptr );
    va_end( argptr );

    FILE* fp;
    fp = fopen( filename, "at" );
    fprintf( fp, msg );
    fclose( fp );
}
