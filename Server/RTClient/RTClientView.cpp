
// RTClientView.cpp: CRTClientView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "RTClient.h"
#endif

#include "RTClientDoc.h"
#include "RTClientView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRTClientView

IMPLEMENT_DYNCREATE(CRTClientView, CFormView)

BEGIN_MESSAGE_MAP(CRTClientView, CFormView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFormView::OnFilePrintPreview)
	
	
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CRTClientView 생성/소멸

CRTClientView::CRTClientView() noexcept
	: CFormView(IDD_RTCLIENT_FORM)
{
	
	m_Sheet.AddPage( &m_DlgNet );
	m_Sheet.AddPage( &m_DlgTraining );

}

CRTClientView::~CRTClientView()
{
}

void CRTClientView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange( pDX );

	
}

BOOL CRTClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CRTClientView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	CMainFrame* pMain = ( CMainFrame* ) AfxGetMainWnd();
	pMain->m_pMainView = this;


}


// CRTClientView 인쇄

BOOL CRTClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CRTClientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CRTClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CRTClientView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 여기에 사용자 지정 인쇄 코드를 추가합니다.
}


// CRTClientView 진단

#ifdef _DEBUG
void CRTClientView::AssertValid() const
{
	CFormView::AssertValid();
}

void CRTClientView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CRTClientDoc* CRTClientView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRTClientDoc)));
	return (CRTClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CRTClientView 메시지 처리기


int CRTClientView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if( CFormView::OnCreate( lpCreateStruct ) == -1 )
		return -1;
	

	if( !m_Sheet.Create( this, DS_3DLOOK | DS_CONTEXTHELP | DS_SETFONT | WS_VISIBLE | WS_CHILD, WS_EX_CONTROLPARENT ) )
		return -1;

	m_Sheet.SetActivePage( &m_DlgTraining );
	m_Sheet.SetActivePage( &m_DlgNet );

	m_Sheet.MoveWindow( 0, 0, 2000, 2000 );
	
	
	return 0;
}


void CRTClientView::OnSize( UINT nType, int cx, int cy )
{
	CFormView::OnSize( nType, cx, cy );

	CRect rect;
	GetClientRect( rect );
	if( m_Sheet.GetSafeHwnd() )
	{
		m_Sheet.MoveWindow( 0, 0, cx, cy, TRUE );
	}
}
