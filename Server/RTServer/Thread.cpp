
#include "MyInclude.h"

static unsigned  WINAPI pThreadProc( void* pArg )
{
	CThread* pThis = ( CThread* ) pArg;
	return pThis->ThreadProc();
}

CThread::CThread( void )
{
	m_ThreadHandle = NULL;
	m_ThredID = 0;
	m_bRun = false;
}

CThread::~CThread( void )
{
	End();
}

bool CThread::Begin()
{
	if( m_bRun )
		return true;
	m_ThreadHandle = ( HANDLE ) _beginthreadex( NULL, 0, pThreadProc, this, 0, ( unsigned* ) &m_ThredID );

	if( m_ThreadHandle == NULL )
		return false;

	m_bRun = true;
	return true;
}

void CThread::End()
{
	if( m_ThreadHandle != NULL )
	{
		CloseHandle( m_ThreadHandle );
	}
	m_ThreadHandle = NULL;
	m_bRun = false;
}

bool CThread::IsRun()
{
	return m_bRun;
}