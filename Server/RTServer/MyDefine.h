#pragma once


typedef union stMyAddr
{
	int i;
	unsigned char b[ 4 ];
}MYADDR;


#define WM_LOADSCT WM_USER+1

#define INVALID_VALUE -1
#define MAX_BUFFER 65535

#define MAX_RECV 65535//0x8000
#define MAX_SEND 65535//0x8000

#define MAX_UDP 50
#define MAX_USER 250

#define	DEL(p)	{if(p){delete   (p); (p) = NULL;}} // 
#define DELS(p)	{if(p){delete[] (p); (p) = NULL;}}


#define SAFE_DELETE_ARRAY(p) { if(p) { delete [] (p); (p) = NULL; } }
#define SAFE_DELETE(p) { if(p) { delete (p); (p) = NULL; } }

#define SAFE_CLOSE(p){ if(p) { CloseHandle(p);(p) = NULL; }}
#define SAFE_CLOSESOCK(p){ if(p) { closesocket(p);(p) = NULL; }}

#define RANDOM( min, max ) (int)( min + rand() % ( max - min + 1 ) )

#define WtoM( wstr, mstr, nmax ) WideCharToMultiByte( CP_ACP, 0, (wstr), -1, (mstr), nmax, NULL, NULL )
#define MtoW( mstr, wstr, nmax ) MultiByteToWideChar( CP_ACP, MB_ERR_INVALID_CHARS, (mstr), -1, (wstr), nmax )