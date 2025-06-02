#pragma once
#include <windows.h>
#include <stdio.h>

class CIni
{
private:
	char m_szFileName[1024];
	int m_bOpenFlag;
	char m_szSelectedData[1024];
	

	FILE* in;
public:
	char TempBuffer[ 1024 ];

private:
	int Getnextline();
	int GetData( const char* section, const char* key);

public:
	
	CIni();
	CIni( const char* filename);
	virtual ~CIni();

	int IsOpened() { return m_bOpenFlag; };
	int clear();
	void SetFileName(const char* fn);
	char* GetString( const char* section, const char* key,const char* strData );
	void GetString( const char* section, const char* key, const char* defString, char strData[], int srcsize );

	int GetInt(const char* section, const char* key, int default_value);
};

