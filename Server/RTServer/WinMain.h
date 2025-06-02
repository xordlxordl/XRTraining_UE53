#pragma once

#include "resource.h"


void SetINI();

BOOL WinSockStart();
void UDPStart( int startport );

void Log( const char* format, ... );
void DebugLog( char* filename, const char* format, ... );

std::vector<std::string> split( std::string input, char delimiter );

unsigned __stdcall ServerThread( void* pArg );
unsigned __stdcall UserThread( void* pArg );
unsigned __stdcall TimeThread( void* pArg );