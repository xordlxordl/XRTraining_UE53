#pragma once
#include "afxdialogex.h"


// CDlgNet 대화 상자

class CDlgNet : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlgNet)

public:
	CDlgNet(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgNet();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_NET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CRichEditCtrl m_Edit_FileName;
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonLoadSct();
	afx_msg void OnEditPaste();
};
