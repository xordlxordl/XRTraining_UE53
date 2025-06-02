// VoiceServer.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "MyInclude.h"
#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
CHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
CHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_VOICESERVER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VOICESERVER));


    SetINI();
    WinSockStart();
    UDPStart( g_UDPPort );
    

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VOICESERVER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_VOICESERVER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindow( szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 0, 0, 320, 240, nullptr, nullptr, hInstance, nullptr );

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
            int error = _mkdir( "Log" );

            g_ListWnd = CreateWindowEx( WS_EX_CLIENTEDGE, "listbox", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOINTEGRALHEIGHT, 0, 20, 0, 0, hWnd, ( HMENU ) 101, ( HINSTANCE ) ( GetWindowLongPtr( hWnd, GWLP_HINSTANCE ) ), NULL );

            if( g_ListWnd != NULL )
            {
                LOGFONT logfont;

                memset( &logfont, 0, sizeof( LOGFONT ) );

                logfont.lfCharSet = DEFAULT_CHARSET;
                logfont.lfHeight = -12;
                strcpy( logfont.lfFaceName, "Arial" );
                g_ArialFont = CreateFontIndirect( &logfont );
                SendMessage( g_ListWnd, WM_SETFONT, ( WPARAM ) g_ArialFont, ( LPARAM ) 0 );
            }
        }break;

    case WM_SIZE:
        {
            if( wParam != SIZE_MINIMIZED )
            {
                MoveWindow( g_ListWnd, 0, 0, LOWORD( lParam ), HIWORD( lParam ), TRUE );
            }
        }break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        {
            WSACleanup();
            if( g_ArialFont != NULL )
                DeleteObject( g_ArialFont );

            UDPStop();

            PostQuitMessage( 0 );
        }break;
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
        
        g_UDPPort = g_Ini.GetInt( "SERVER", "UDP", 7600 );
        g_SendToME = g_Ini.GetInt( "CONFIG", "SENDTOME", 0 );
        g_Ini.clear();
    }

    Log( "Start UDP Port [ %d ]", g_UDPPort );
}

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
    
    int i;
    for( i = 0; i < MAX_UDP; ++i )
    {
        g_UDP[ i ].Start( i, startport + i );
    }
}
void UDPStop()
{
    int i;
    for( i = 0; i < MAX_UDP; ++i )
    {
        g_UDP[ i ].Finish();
    }
}