// DlgTraining.cpp: 구현 파일
//

#include "pch.h"
#include "RTClient.h"
#include "afxdialogex.h"
#include "DlgTraining.h"


// CDlgTraining 대화 상자

IMPLEMENT_DYNAMIC(CDlgTraining, CMFCPropertyPage)

CDlgTraining::CDlgTraining(CWnd* pParent /*=nullptr*/)
	: CMFCPropertyPage(IDD_DIALOG_TRAINING)
{
	m_psp.dwFlags |= PSP_USEHICON;
	m_psp.hIcon = AfxGetApp()->LoadIconW( IDI_ICON2 );
}

CDlgTraining::~CDlgTraining()
{
}

void CDlgTraining::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTraining, CMFCPropertyPage)
END_MESSAGE_MAP()


// CDlgTraining 메시지 처리기
