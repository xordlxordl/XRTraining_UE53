// UDPTest.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "UDPServer.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <thread>


using namespace std;

#define _ARR 200

struct MyStruct
{
	int b[ _ARR ];

	MyStruct()
	{
		int i;
		for( i = 0; i < _ARR; ++i )
			b[i] = i;
	}
};

int main()
{

	MyStruct packet;

	CUDPServer udp;

	udp.WinSockStart();
	udp.Start( 0, 7500 );

	char buffer[65000];	 memset( buffer, 0x00, sizeof( buffer ) );
	memcpy( buffer, &packet, sizeof( MyStruct ) );

	MYADDR addr;
	addr.b[0] = 192; addr.b[ 1 ] = 168; addr.b[ 2 ] = 0; addr.b[ 3 ] = 159;

	udp.SendTo( buffer, sizeof( MyStruct ), addr.i, 8000 );
	

	while( 1 )
	{

		string line;
		getline( cin, line );
		if( line == "exit" )
			break;

		this_thread::sleep_for( chrono::milliseconds( 1 ) );
	}

	udp.Finish();
}

