#pragma once

#include <windows.h>
class CMonitor
{
protected:
	HANDLE d_mutex;
public:
	CMonitor( void );
	CMonitor( char* name );
	~CMonitor( void );

	void On() const;
	void Off() const;

};
