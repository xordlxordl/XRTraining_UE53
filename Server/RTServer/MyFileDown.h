#pragma once
#include <string>

using namespace std;
using std::string;

class CMyFileDown
{
public:
	CMyFileDown();
	~CMyFileDown();

	bool DownLoad( string filename );

	void Recv( string* pString );
};

extern CMyFileDown* g_pHttp;