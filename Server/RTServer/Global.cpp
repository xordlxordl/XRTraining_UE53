#include "Global.h"

HWND g_MainWnd = NULL;
HWND ListboxWindow = NULL;
HFONT ArialFont = NULL;

int g_UserPer = 0;
int MAX_QUEUE = 0;
int MAX_THREAD = 0;
int g_bUseUDP = 1;
int g_bUDPTest = 0;
int g_bUseTime = 0;

char g_HostName[ 256 ];
char g_IP[ 20 ];
int g_ServerPort = 0;

SOCKET g_ServerSocket = NULL;
HANDLE g_ServerHandle = NULL;

WSAEVENT g_Event[ MAX_USER ];
HANDLE g_UserHandle[ MAX_USER ];

HANDLE g_TimeHandle = NULL;

int g_ServerNum = 0;

CUDPServer g_UDP[ MAX_UDP ];
int g_UDPPort = 0;

CIni g_Ini( "Ini.ini" );

CUserManager g_User;

CPlayScenario g_Play;

//-----------------------------------------------------------------------------



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
    count = SendMessage( ListboxWindow, LB_GETCOUNT, 0, 0 );
    if( count > 1000 )
    {
        SendMessage( ListboxWindow, LB_RESETCONTENT, 0, 0 );
        logcount++;
    }

    SendMessage( ListboxWindow, LB_INSERTSTRING, 0, ( LPARAM ) msg );
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

string AnsiToUtf8( string strAnsi )
{
	string ret;
	if( strAnsi.length() <= 0 )
		return ret;

	int nWideStrLength = MultiByteToWideChar( CP_ACP, 0, strAnsi.c_str(), -1, NULL, 0 );
	WCHAR* pwszBuf = new WCHAR[ nWideStrLength + 1 ];

	memset( pwszBuf, 0, nWideStrLength + 1 );

	MultiByteToWideChar( CP_ACP, 0, strAnsi.c_str(), -1, pwszBuf, nWideStrLength + 1 );

	int nUtf8Length = WideCharToMultiByte( CP_UTF8, 0, pwszBuf, -1, NULL, 0, NULL, FALSE );

	char* pszUtf8Buf = new char[ nUtf8Length + 1 ];
	memset( pszUtf8Buf, 0, nUtf8Length + 1 );

	WideCharToMultiByte( CP_UTF8, 0, pwszBuf, -1, pszUtf8Buf, nUtf8Length + 1, NULL, FALSE );

	ret = pszUtf8Buf;

	SAFE_DELETE_ARRAY( pszUtf8Buf );
	SAFE_DELETE_ARRAY( pwszBuf );

	return ret;
}

string Utf8ToAnsi( string strUTF8 )
{
	string ret;
	if( strUTF8.length() <= 0 )
		return ret;

	int nWideStrLength = MultiByteToWideChar( CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0 );

	WCHAR* pwszBuf = new WCHAR[ nWideStrLength + 1 ];

	memset( pwszBuf, 0, nWideStrLength + 1 );
	MultiByteToWideChar( CP_UTF8, 0, strUTF8.c_str(), -1, pwszBuf, nWideStrLength + 1 );

	int nAnsiStrLength = WideCharToMultiByte( CP_ACP, 0, pwszBuf, -1, NULL, 0, NULL, FALSE );

	char* pszAnsiBuf = new char[ nAnsiStrLength + 1 ];
	memset( pszAnsiBuf, 0, nAnsiStrLength + 1 );

	WideCharToMultiByte( CP_ACP, 0, pwszBuf, -1, pszAnsiBuf, nAnsiStrLength + 1, NULL, FALSE );
	ret = pszAnsiBuf;

	SAFE_DELETE_ARRAY( pszAnsiBuf );
	SAFE_DELETE_ARRAY( pwszBuf );

	return ret;
}

wstring Utf8ToWchar( string strAnsi )
{
	std::wstring ret;
	if( strAnsi.length() <= 0 )
		return ret;

	int nWideStrLength = MultiByteToWideChar( CP_UTF8, 0, strAnsi.c_str(), -1, NULL, 0 );

	WCHAR* pwszBuf = new WCHAR[ nWideStrLength + 1 ];
	memset( pwszBuf, 0, nWideStrLength + 1 );

	MultiByteToWideChar( CP_UTF8, 0, strAnsi.c_str(), -1, pwszBuf, nWideStrLength + 1 );

	ret = pwszBuf;

	SAFE_DELETE_ARRAY( pwszBuf );
	return ret;
}

string WcharToAnsi( wstring strWchar )
{
	string ret;
	if( strWchar.length() <= 0 )
		return ret;

	int iLength = WideCharToMultiByte( CP_ACP, 0, strWchar.c_str(), -1, NULL, 0, NULL, FALSE );

	char* pChar = new char[ iLength + 1 ];
	memset( pChar, 0, iLength + 1 );

	WideCharToMultiByte( CP_ACP, 0, strWchar.c_str(), -1, pChar, iLength + 1, NULL, FALSE );
	ret = pChar;

	SAFE_DELETE_ARRAY( pChar );

	return ret;
}

string WcharToUtf8( wstring strWchar )
{
	string ret;
	if( strWchar.length() <= 0 )
		return ret;

	int iLength = WideCharToMultiByte( CP_UTF8, 0, strWchar.c_str(), -1, NULL, 0, NULL, FALSE );

	char* pChar = new char[ iLength + 1 ];
	memset( pChar, 0, iLength + 1 );

	WideCharToMultiByte( CP_UTF8, 0, strWchar.c_str(), -1, pChar, iLength + 1, NULL, FALSE );
	ret = pChar;

	SAFE_DELETE_ARRAY( pChar );

	return ret;
}
