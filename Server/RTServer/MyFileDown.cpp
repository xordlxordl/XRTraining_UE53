
#include "json/json.h"
#include "Global.h"
#include "MyFileDown.h"
#include "Singleton.h"
#include <curl/curl.h>
#include < io.h >
#include "WinMain.h"

#define HTTP_URL	"http://192.168.0.64/ARTS/download.aspx"
//#define HTTP_URL	"http://localhost:57159/ARTS/download.aspx"
CMyFileDown* g_pHttp = &CSingleton< CMyFileDown >::GetSingleton();


static size_t CURLRecv( void* ptr, size_t size, size_t nmemb, void* stream )
{
	size_t written = fwrite( ptr, size, nmemb, ( FILE* ) stream );
	return written;
}

static size_t header_callback( char* buffer, size_t size,
							   size_t nitems, void* userdata )
{
	std::string* headers = ( std::string* ) userdata;
	headers->append( buffer, nitems * size );
	return nitems * size;
}

CMyFileDown::CMyFileDown()
{
}

CMyFileDown::~CMyFileDown()
{
}

bool CMyFileDown::DownLoad( string filename )
{

	if( ( _access( filename.c_str(), 0 ) ) != -1 )
	{
		Log("%s : aleady have file ", filename.c_str() );
		return true;
	}
	Log( "%s : filename", filename.c_str() );

	std::string filenameutf8 = AnsiToUtf8( filename );


    Json::Value root;
    root[ "filename" ] = filenameutf8;
    root[ "filetype" ] = "tct";
    string str = root.toStyledString();

	char szUrl[ MAX_PATH ]; memset( szUrl, 0x00, sizeof( szUrl ) );
	sprintf( szUrl, HTTP_URL );

	std::string resultString;

	CURL* curl;
	CURLcode res;

	FILE* fp;

	curl_global_init( CURL_GLOBAL_ALL );
	curl = curl_easy_init();

	fp = fopen( filename.c_str(), "wb" );

	curl_easy_setopt( curl, CURLOPT_URL, szUrl );

	curl_easy_setopt( curl, CURLOPT_VERBOSE, 1L );
	curl_easy_setopt( curl, CURLOPT_NOPROGRESS, 1L );

	curl_easy_setopt( curl, CURLOPT_POST, 1 );

	curl_easy_setopt( curl, CURLOPT_POSTFIELDS, str.c_str() );

	curl_easy_setopt( curl, CURLOPT_HEADERFUNCTION, header_callback );

	curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, CURLRecv );

	curl_easy_setopt( curl, CURLOPT_WRITEDATA, fp );

	string header;
	curl_easy_setopt( curl, CURLOPT_HEADERDATA, &header );

	res = curl_easy_perform( curl );

	struct curl_header* type;
	CURLHcode h;
	h = curl_easy_header( curl, "filename", 0, CURLH_HEADER, -1, &type );
	

	fclose( fp );

	curl_easy_cleanup( curl );


	if( h == CURLHE_OK )
	{
		return true;
	}
	else
	{
		int r = remove( filename.c_str() );

		if( r == 0 )
		{
			Log( "[%s] delete file", filename.c_str() );
		}
		return false;
	}
	
	
	//Json::Value HeaderRoot;
	//Json::Reader reader;
	//bool bResult = reader.parse( header, HeaderRoot );

	if( res == CURLE_OK )
	{
		//Recv( &resultString );
		
		return true;
	}


	return false;
}

void CMyFileDown::Recv( string* pString )
{
}
