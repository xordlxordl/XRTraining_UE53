
#include "MyInclude.h"

#include "MyFileDown.h"
#define MAX_LOADSTRING 100


HINSTANCE hInst;
CHAR szTitle[MAX_LOADSTRING];
CHAR szWindowClass[MAX_LOADSTRING];


ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    // TODO: 여기에 코드를 입력합니다.

    
    LoadString( hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING );
    LoadString( hInstance, IDC_RTSERVER, szWindowClass, MAX_LOADSTRING );
    MyRegisterClass( hInstance );

    
    if( !InitInstance( hInstance, nCmdShow ) )
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_RTSERVER ) );

    //g_pHttp->DownLoad("");
    
    //TEST
    //g_SCTFileName = "Defense_Gate.sct.json";
    //LoadSCT();

    //----------------------------------------------------------------------------------------------------------
    //초기화
    SetINI();

    //----------------------------------------------------------------------------------------------------------
    //쓰레드 갯수 계산
    //----------------------------------------------------------------------------------------------------------

    MAX_THREAD = MAX_USER / g_UserPer;
    MAX_QUEUE = MAX_THREAD;

    

    //----------------------------------------------------------------------------------------------------------
    //윈속 스타트
    WinSockStart();

    //----------------------------------------------------------------------------------------------------------
    //이벤트 생성
    int i;
    for( i = 0; i < MAX_USER; ++i )
    {
        g_Event[ i ] = WSACreateEvent();
    }

    //----------------------------------------------------------------------------------------------------------
    //억셉트 쓰레드
    g_ServerHandle = ( HANDLE ) _beginthreadex( NULL, 0, ServerThread, NULL, 0, NULL );

    //유저 쓰레드
    for( i = 0; i < MAX_THREAD; ++i )
    {
        g_UserHandle[ i ] = NULL;
        g_UserHandle[ i ] = ( HANDLE ) _beginthreadex( NULL, 0, UserThread, ( void* ) i, 0, NULL );
    }


    if( g_bUseTime )
    {
        g_TimeHandle = ( HANDLE ) _beginthreadex( NULL, 0, TimeThread, NULL, 0, NULL );
    }

    UDPStart( g_UDPPort );

    MSG msg;

    
    while( GetMessage( &msg, nullptr, 0, 0 ) )
    {
        if( !TranslateAccelerator( msg.hwnd, hAccelTable, &msg ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE( IDC_RTSERVER ));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCE( IDC_RTSERVER );
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}


BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindow( szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 320, 240, nullptr, nullptr, hInstance, nullptr );

   if( !hWnd )
   {
      return FALSE;
   }

   ShowWindow( hWnd, nCmdShow );
   UpdateWindow( hWnd );
   g_MainWnd = hWnd;
   return TRUE;
}


LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch (message)
    {

    case WM_LOADSCT:
    {
        //g_Play.LoadSCT();
    }break;

    case WM_CREATE:
    {

        int error = _mkdir("Log");

        ListboxWindow = CreateWindowEx( WS_EX_CLIENTEDGE, "listbox", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOINTEGRALHEIGHT, 0, 20, 0, 0, hWnd, ( HMENU ) 101, ( HINSTANCE ) ( GetWindowLongPtr( hWnd, GWLP_HINSTANCE ) ), NULL );
        if( ListboxWindow != NULL )
        {
            LOGFONT logfont;

            memset( &logfont, 0, sizeof( LOGFONT ) );

            logfont.lfCharSet = DEFAULT_CHARSET;
            logfont.lfHeight = -12;
            strcpy( logfont.lfFaceName, "Arial" );
            ArialFont = CreateFontIndirect( &logfont );
            SendMessage( ListboxWindow, WM_SETFONT, ( WPARAM ) ArialFont, ( LPARAM ) 0 );
        }

    }
    break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch( wmId )
            {
            case IDM_ABOUT:
                DialogBox( hInst, MAKEINTRESOURCE( IDD_ABOUTBOX ), hWnd, About );
                break;
            case IDM_EXIT:
                DestroyWindow( hWnd );
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_SIZE:
    {
        if( wParam != SIZE_MINIMIZED )
        {
            MoveWindow( ListboxWindow, 0, 0, LOWORD( lParam ), HIWORD( lParam ), TRUE );
        }
    }break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint( hWnd, &ps );
        }
        break;
    case WM_CLOSE:
    {
        

        int i;
        SAFE_CLOSE( g_ServerHandle );


        for( i = 0; i < MAX_USER; ++i )
        {
            WSACloseEvent( g_Event[ i ] );
        }

        for( i = 0; i < MAX_THREAD; ++i )
        {
            SAFE_CLOSE( g_UserHandle[ i ] );
        }

        SAFE_CLOSESOCK( g_ServerSocket );


        WSACleanup();
        DestroyWindow( hWnd );
    }
    break;
    case WM_DESTROY:
        {
            WSACleanup();
            if( ArialFont != NULL )
                DeleteObject( ArialFont );
            PostQuitMessage(0);
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void SetINI()
{

    if( g_Ini.IsOpened() )
    {
        g_ServerPort = g_Ini.GetInt( "SERVER", "PORT", 12000 );

        g_UDPPort = g_Ini.GetInt( "UDP", "PORT", 7600 );

        string strip = g_Ini.GetString( "UDP", "IP", "127.0.0.1");

        int ip1 = inet_addr( strip.c_str() );

        vector<string> arrip = split( strip, '.');

        stMyAddr addr;
        if( arrip.size() == 4 )
        {
            addr.b[ 0 ] = atoi( arrip[ 0 ].c_str() );
            addr.b[ 1 ] = atoi( arrip[ 1 ].c_str() );
            addr.b[ 2 ] = atoi( arrip[ 2 ].c_str() );
            addr.b[ 3 ] = atoi( arrip[ 3 ].c_str() );

        }
        
        
        int intip = addr.i;

        int a = 0;


        g_ServerNum = g_Ini.GetInt( "CONFIG", "NUM", 0 );
        
        g_UserPer = g_Ini.GetInt( "CONFIG", "USERPER", 0 ); 

        g_bUseUDP = g_Ini.GetInt( "CONFIG", "USEUDP", 0 );

        g_bUDPTest = g_Ini.GetInt( "CONFIG", "UDPTEST", 0 );
        g_bUseTime = g_Ini.GetInt( "CONFIG", "TIME", 0 );

        g_Ini.clear();
    }
}

std::vector<std::string> split( std::string input, char delimiter )
{
    std::vector<std::string> answer;
    std::stringstream ss( input );
    std::string temp;

    while( getline( ss, temp, delimiter ) )
    {
        answer.push_back( temp );
    }

    return answer;
};



BOOL WinSockStart()
{
    WSAData wsaData;
    int     error;

    error = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
    if( error == SOCKET_ERROR )
    {
        WSACleanup();
        return FALSE;
    }

    char str[ 256 ]; memset( str, 0x00, sizeof( str ) );

    PHOSTENT phostent;
    IN_ADDR in;
    if( ( phostent = gethostbyname( g_HostName ) ) == NULL )
    {
        sprintf( str, "gethostbyname() generated error %d", WSAGetLastError() );
        Log( str );
    }
    else
    {
        memcpy( &in, phostent->h_addr, 4 );
        memset( g_IP, 0x00, sizeof( g_IP ) );
        sprintf( g_IP, "%s", inet_ntoa( in ) );
        sprintf( str, "Host Name Is [ %s ] IP : [%s]", phostent->h_name, g_IP );
        Log( str );
    }
    return TRUE;
}

void UDPStart( int startport )
{
    if( g_bUseUDP == 0 )
        return;

    int i;
    for( i = 0; i < MAX_UDP; ++i )
    {
        g_UDP[ i ].Start( i, startport + i );
    }
}


unsigned __stdcall ServerThread( void* pArg )
{
    Log( "ServerThread Start..TCPPort : %d", g_ServerPort );
    char szBuffer[ 1024 ];	memset( szBuffer, 0x00, sizeof( szBuffer ) );

    g_ServerSocket = socket( AF_INET, SOCK_STREAM, 0 );

    if( g_ServerSocket == INVALID_SOCKET )
    {
        sprintf( szBuffer, "socket error code=%x", WSAGetLastError() );
        Log( szBuffer );
        return 0;
    }

    //나겔알고리즘 비적용
    BOOL opt_val = TRUE;
    setsockopt( g_ServerSocket, IPPROTO_TCP, TCP_NODELAY, ( char* ) &opt_val, sizeof( opt_val ) );
    DWORD size = 0x8000;
    setsockopt( g_ServerSocket, SOL_SOCKET, SO_SNDBUF, ( const char* ) &size, sizeof( size ) );
    setsockopt( g_ServerSocket, SOL_SOCKET, SO_RCVBUF, ( const char* ) &size, sizeof( size ) );

    struct sockaddr_in sa;
    memset( &sa, 0, sizeof( sa ) );
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl( INADDR_ANY );
    sa.sin_port = htons( g_ServerPort );

    //Bind
    if( ::bind( g_ServerSocket, ( struct sockaddr* ) &sa, sizeof( sa ) ) != 0 )
    {
        sprintf( szBuffer, "bind error code=%d", WSAGetLastError() );
        Log( szBuffer );
        return 0;
    }

    //Listen
    if( listen( g_ServerSocket, 500 ) != 0 )
    {
        sprintf( szBuffer, "listen error code=%d", WSAGetLastError() );
        Log( szBuffer );
        return 0;
    }

    CMonitor AcceptMutex;

    while( g_ServerHandle )
    {
        struct sockaddr_in ca;
        int clientAddressLength = sizeof( ca );
        int nLength = sizeof( ca );

        AcceptMutex.On();
        SOCKET socket = accept( g_ServerSocket, ( struct sockaddr* ) &ca, &nLength );

        if( socket == INVALID_SOCKET )
        {
            closesocket( socket );

            Log( "소켓 생성 실패" );
            AcceptMutex.Off();
            continue;
        }
        //유저등록에 실패하면 소켓을 닫아버린다..
        if( g_User.AddUser( socket, ca ) == false )
        {
            closesocket( socket );

            Log( "ADDUser Fail %d.%d.%d.%d",
                 ca.sin_addr.S_un.S_un_b.s_b1,
                 ca.sin_addr.S_un.S_un_b.s_b2,
                 ca.sin_addr.S_un.S_un_b.s_b3,
                 ca.sin_addr.S_un.S_un_b.s_b4 );
        }
        AcceptMutex.Off();

        Sleep( 1 );
    }
    return 0;
}

unsigned __stdcall UserThread( void* pArg )
{
    WSANETWORKEVENTS events;

    DWORD dwReturn = 0, dwRet = 0;

    //int nread;
    int ThreadArray = ( int ) pArg;
    int UserArray = ( ThreadArray * g_UserPer );
    int i = 0;
    int inum = 0;
    //Log("UserThread %d", ThreadArray );
    while( g_UserHandle[ ThreadArray ] )
    {
        //64개..
        dwReturn = WSAWaitForMultipleEvents( g_UserPer, &g_Event[ UserArray ], FALSE, WSA_INFINITE, FALSE );

        if( dwReturn != WSA_WAIT_FAILED )
        {
            for( i = 0; i < g_UserPer; ++i )
            {
                inum = UserArray + i;

                if( g_User.m_User[ inum ].m_hSocket )
                {

                    dwRet = WSAEnumNetworkEvents( g_User.m_User[ inum ].m_hSocket, g_Event[ inum ], &events );

                    if( dwRet == 0 )
                    {
                        //FD_READ EVENT 면.
                        if( ( events.lNetworkEvents & FD_READ ) == FD_READ )
                        {
                            g_User.m_User[ inum ].Recv();
                        }
                        if( ( events.lNetworkEvents & FD_WRITE ) == FD_WRITE )
                        {
                            g_User.m_User[ inum ].FlushSendBuffer();
                        }
                        if( ( events.lNetworkEvents & FD_CLOSE ) == FD_CLOSE )
                        {
                            //접속 종료 처리
                            //Log("g_User.DelUser( inum %d );", inum );
                            g_User.DelUser( inum );
                        }
                    }
                }
            }
        }
        Sleep( 1 );
    }
    return 0;
}

unsigned __stdcall TimeThread( void* pArg )
{
    //바쁜길 쉬었다 갑시다
    Sleep( 1000 );


    while( g_TimeHandle )
    {
        int i;
        for( i = 0; i < MAX_UDP; ++i )
        {
            //패킷이 온적이 없다면 연결된 클라가 없다는것
            if( g_UDP[ i ].m_RecvTime <= 0 )
                continue;
            if( g_UDP[i].m_ClientIP <= 0 )
                continue;
            if( g_UDP[i].m_ClientPort <= 0 )
                continue;

            unsigned __int64 Tick = GetTickCount64() - g_UDP[ i ].m_RecvTime;

            if( Tick >= 10000 )
            {

                g_UDP[ i ].InitClientAddr();
                Log( "RecvTime Checkg_UDP[ %d ].InitClientAddr();", i );
            }

        }
        Sleep( 1 );
    }
    return 0;
}
