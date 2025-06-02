#include "Ini.h"


CIni::CIni()
{
	in = NULL;
	m_bOpenFlag = FALSE;

	memset(m_szFileName, 0x00, sizeof(m_szFileName));
	memset(m_szSelectedData, 0x00, sizeof(m_szSelectedData));
	memset( TempBuffer , 0x00 , sizeof( TempBuffer ) );
}

CIni::CIni( const char* filename )
{

	in = NULL;
	m_bOpenFlag = FALSE;
	memset( m_szFileName, 0x00, sizeof( m_szFileName ) );
	memset( m_szSelectedData, 0x00, sizeof( m_szSelectedData ) );
	memset( TempBuffer , 0x00 , sizeof( TempBuffer ) );

	if( filename == NULL )
		return;

	sprintf_s( m_szFileName, "%s", filename );

	 errno_t err = fopen_s( &in, m_szFileName, "r");

	if( in != NULL )
	{
		m_bOpenFlag = TRUE;
	}
}

CIni::~CIni(void)
{
	if( in )
	{
		fclose( in );
		in = NULL;
	}
}

int CIni::clear()
{
	if( in )
	{
		fclose( in );
		in = NULL;
		m_bOpenFlag = FALSE;
	}
	return 1;
}

void CIni::SetFileName( const char* fn)
{
	if( m_bOpenFlag == TRUE )
	{
		clear();
	}

	m_bOpenFlag = FALSE;
	m_szSelectedData[ 0 ] = '\0';
	memset( m_szFileName, 0x00, sizeof( m_szFileName ) );

	sprintf_s( m_szFileName, "%s", fn );

	errno_t err = fopen_s( &in, m_szFileName, "r");

	if( in != NULL )
	{
		m_bOpenFlag = TRUE;
	}

}

int CIni::Getnextline()
{
	int ch_int;
	char ch;

	do
	{
		ch_int = fgetc( in );
		if( ch_int == EOF )
			return -1;

		ch = (char)ch_int;
	} while( ch_int != 0x0d && ch_int != 0x0a );

	do
	{
		ch_int = fgetc( in );
		ch = (char)ch_int;

		if( ch_int == EOF )
			return 0;

	} while( ch_int == 0x0d || ch_int == 0x0a );

	return ch_int;
}

int CIni::GetData( const char* section, const char* key)
{
	int i;
	int ch_int, section_length;

	section_length = ( int )strlen( section );
	fseek( in, 0L, SEEK_SET );

	char szTemp[ 1024 ];

	do
	{
		memset( szTemp , 0 , sizeof( szTemp ) );

		
		do
		{
			ch_int = fgetc( in );

			
			if( ch_int == EOF )
				return 0;
		} while( ch_int != '[' );


		for( i = 0; i < section_length; i++ )
		{
			ch_int = fgetc( in );
			if (ch_int != ']')
			{
				szTemp[ i ] = ( char )ch_int;
			}
		};
	} while( strcmp( section, szTemp ) != 0 );

	if( ( ch_int = Getnextline() ) == -1 )
		return 0;

	do
	{
		i = 0;
		memset( szTemp, 0, sizeof( szTemp ) );
		szTemp[ i++ ] = (char)ch_int;
		do
		{
			ch_int = fgetc( in );
			if( ch_int == '=' )
				break;

			if( ch_int == ' ' )
				continue;

			szTemp[ i++ ] = ( char )ch_int;

			
			if( ch_int == EOF )
				return 0;

		} while( ch_int != '=' );


		if( strcmp( key, szTemp ) == 0)
		{
			break;
		}

		if( ( ch_int = Getnextline() ) == -1 )
			return 0;
		
	} while( 1 );

	i = 0;
	memset( m_szSelectedData, 0, sizeof( m_szSelectedData ) );
	do
	{
		ch_int = fgetc(in);
		if( ch_int == EOF || ch_int == 0x0d || ch_int == 0x0a )
			break;
		if( ch_int == ' ' )
			continue;

		m_szSelectedData[ i++ ] = ( char )ch_int;

	} while( ch_int != 0x0d && ch_int != 0x0a );

	m_szSelectedData[ i++ ] = '\0';

	return 1;
}

char* CIni::GetString( const char* section, const char* key, const char* strData )
{
	if( m_bOpenFlag == FALSE )
		return FALSE;

	if( GetData( section, key ) == 0 )
	{
		
		memset( TempBuffer , 0x00 , sizeof( TempBuffer ) );
		
		strcpy_s( TempBuffer , sizeof( TempBuffer ) , strData );
		return TempBuffer;
	}

	return m_szSelectedData;
}

void CIni::GetString( const char* section, const char* key, const char* defString, char strData[], int srcsize = 20 )
{
	if( m_bOpenFlag == FALSE )
	{
		strcpy_s( strData, srcsize, "" );
		return;
	}

	if( GetData( section, key ) == 0 )
	{
		strcpy_s( strData, srcsize, defString);
		return;
	}

	strcpy_s( strData, srcsize, m_szSelectedData);
}



int CIni::GetInt( const char* section, const char* key, int default_value)
{
	if( m_bOpenFlag == FALSE )
		return -1;

	if( GetData( section, key ) == 0 )
	{
		return default_value;
	}

	char strData[1024];	memset( strData, 0x00, sizeof( strData ) );

	strcpy_s( strData, sizeof( strData ), m_szSelectedData );

	return atoi( strData );
}
