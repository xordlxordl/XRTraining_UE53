#pragma once



typedef union stMyAddr
{
	int i;
	unsigned char b[ 4 ];
}MYADDR;

#define INVALID_VALUE -1
#define MAX_BUFFER 65535
#define MAX_UDP 200