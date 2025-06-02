#pragma once
// CColorSheet

class CColorSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CColorSheet)
public:
	
public:
	CColorSheet();
	CColorSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CColorSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

	virtual ~CColorSheet();


protected:
	DECLARE_MESSAGE_MAP()
public:

	void ResizeControl();
	void ResizeSubSheet();

	virtual BOOL OnInitDialog();


};


