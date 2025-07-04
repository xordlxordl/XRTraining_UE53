﻿#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.


#include <afxdisp.h>        // MFC 자동화 클래스입니다.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원


#include <afxsock.h>            // MFC 소켓 확장







#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#define MAX_RECV 65535//0x8000
#define MAX_SEND 65535//0x8000

#define INVALID_VALUE -1

#define	DEL(p)	{if(p){delete   (p); (p) = NULL;}} // 1차원 동적배열 메모리 해제
#define DELS(p)	{if(p){delete[] (p); (p) = NULL;}}


#define SAFE_DELETE_ARRAY(p) { if(p) { delete [] (p); (p) = NULL; } }
#define SAFE_DELETE(p) { if(p) { delete (p); (p) = NULL; } }

#define SAFE_CLOSE(p){ if(p) { CloseHandle(p);(p) = NULL; }}
#define SAFE_CLOSESOCK(p){ if(p) { closesocket(p);(p) = NULL; }}

#define RANDOM( min, max ) (int)( min + rand() % ( max - min + 1 ) )

#define WtoM( wstr, mstr, nmax ) WideCharToMultiByte( CP_ACP, 0, (wstr), -1, (mstr), nmax, NULL, NULL )
#define MtoW( mstr, wstr, nmax ) MultiByteToWideChar( CP_ACP, MB_ERR_INVALID_CHARS, (mstr), -1, (wstr), nmax )

#define MAX_CHANNEL 1
