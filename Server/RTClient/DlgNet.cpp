// DlgNet.cpp: 구현 파일
//

#include "pch.h"
#include "RTClient.h"
#include "afxdialogex.h"
#include "DlgNet.h"
#include "MainFrm.h"

// CDlgNet 대화 상자

IMPLEMENT_DYNAMIC(CDlgNet, CMFCPropertyPage)

CDlgNet::CDlgNet(CWnd* pParent /*=nullptr*/)
	: CMFCPropertyPage(IDD_DIALOG_NET)
{
	m_psp.dwFlags |= PSP_USEHICON;
	m_psp.hIcon = AfxGetApp()->LoadIconW( IDI_ICON1 );
}

CDlgNet::~CDlgNet()
{
}

void CDlgNet::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_RICHEDIT2_FILE, m_Edit_FileName );
}


BEGIN_MESSAGE_MAP(CDlgNet, CMFCPropertyPage)
	ON_BN_CLICKED( IDC_BUTTON_CONNECT, &CDlgNet::OnBnClickedButtonConnect )
	ON_BN_CLICKED( IDC_BUTTON_SEND, &CDlgNet::OnBnClickedButtonSend )
	ON_BN_CLICKED( IDC_BUTTON_LOAD_SCT, &CDlgNet::OnBnClickedButtonLoadSct )
	ON_COMMAND( ID_EDIT_PASTE, &CDlgNet::OnEditPaste )
END_MESSAGE_MAP()


// CDlgNet 메시지 처리기


void CDlgNet::OnBnClickedButtonConnect()
{
	CMainFrame* pMain = ( CMainFrame* ) AfxGetMainWnd();

	pMain->m_TCP.Connect( "192.168.0.205", 12000 );
}


void CDlgNet::OnBnClickedButtonSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlgNet::OnBnClickedButtonLoadSct()
{
	CMainFrame* pMain = ( CMainFrame* ) AfxGetMainWnd();

	CString str;
	m_Edit_FileName.GetWindowText( str );


	char filename[ 64 ];	memset( filename, 0x00, sizeof( filename ) );

	WtoM( str.GetBuffer(), filename, sizeof( filename ) );

	pMain->m_TCP.SendSetSCT( filename );

	int a = 0;
}


void CDlgNet::OnEditPaste()
{
	CRichEditCtrl* pEdit = ( CRichEditCtrl* ) GetFocus();
	if( pEdit )
	{
		pEdit->Paste();
	}
}
