// ColorSheet.cpp : 구현 파일입니다.
//

#include "pch.h"
#include "ColorSheet.h"
#include "HighColorTab.hpp"

// CColorSheet

IMPLEMENT_DYNAMIC(CColorSheet, CPropertySheet)

CColorSheet::CColorSheet():CPropertySheet()
{

}
CColorSheet::CColorSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CColorSheet::CColorSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

CColorSheet::~CColorSheet()
{
}


BEGIN_MESSAGE_MAP(CColorSheet, CPropertySheet)
END_MESSAGE_MAP()


// CColorSheet 메시지 처리기입니다.


BOOL CColorSheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	HighColorTab::UpdateImageList( *this );

	return bResult;
}

void CColorSheet::ResizeControl()
{
	
	CWnd* pParent;
	CRect rectParent;

	pParent = GetParent();
	if (pParent == NULL)
	{
		return;
	}

	// Get parents client area
	pParent->GetClientRect(&rectParent);

	// Resize the sheet
	// First find relative change
	CSize sizeRelChange = CSize( 0, 0 );
	CRect rectWindow;

	GetWindowRect( &rectWindow );
	ScreenToClient( &rectWindow );
	
	sizeRelChange.cx = rectWindow.Width() - rectParent.Width();
	sizeRelChange.cy = rectWindow.Height() - rectParent.Height();

	rectWindow.top = 26;
	rectWindow.right -= ( sizeRelChange.cx );
	rectWindow.bottom -= ( sizeRelChange.cy - rectWindow.top );
	
	// Then resize the sheet
	MoveWindow( rectWindow );

	// Resize the CTabCtrl
	CRect rectTab;
	CTabCtrl* pTab = GetTabControl();
	pTab->GetWindowRect(&rectTab);
	ScreenToClient(&rectTab);

	rectTab.right -= ( sizeRelChange.cx  );
	rectTab.bottom -= ( sizeRelChange.cy );
	pTab->MoveWindow(&rectTab);
	
	// Resize the active page
	CPropertyPage* pPage = GetActivePage();
	CRect rectPage;
	pPage->GetWindowRect( &rectPage );
	ScreenToClient( &rectPage );
	
	rectPage.right -= ( sizeRelChange.cx );
	rectPage.bottom -= ( sizeRelChange.cy );
	pPage->MoveWindow( &rectPage );
	
}

void CColorSheet::ResizeSubSheet()
{
	
	CWnd* pParent;
	CRect rectParent;

	pParent = GetParent();
	if (pParent == NULL)
	{
		return;
	}

	// Get parents client area
	pParent->GetClientRect(&rectParent);

	// Resize the sheet
	// First find relative change
	CSize sizeRelChange;
	CRect rectWindow;

	GetWindowRect(&rectWindow);
	ScreenToClient(&rectWindow);
	sizeRelChange.cx = rectWindow.Width() - rectParent.Width();
	sizeRelChange.cy = rectWindow.Height() - rectParent.Height();

	rectWindow.right -= ( sizeRelChange.cx );
	rectWindow.bottom -= ( sizeRelChange.cy );
	// Then resize the sheet
	MoveWindow(&rectWindow);

	// Resize the CTabCtrl
	CTabCtrl* pTab = GetTabControl();
	pTab->GetWindowRect(&rectWindow);
	ScreenToClient(&rectWindow);
	rectWindow.right -= ( sizeRelChange.cx  );
	rectWindow.bottom -= ( sizeRelChange.cy );
	pTab->MoveWindow(&rectWindow);

	// Resize the active page
	CPropertyPage* pPage = GetActivePage();
	CRect m_rectPage;
	pPage->GetWindowRect( &m_rectPage );
	ScreenToClient( &m_rectPage );

	m_rectPage.right -= ( sizeRelChange.cx );
	m_rectPage.bottom -= ( sizeRelChange.cy );
	pPage->MoveWindow( &m_rectPage );
	

}