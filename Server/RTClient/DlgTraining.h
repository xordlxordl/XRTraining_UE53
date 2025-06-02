#pragma once
#include "afxdialogex.h"


// CDlgTraining 대화 상자

class CDlgTraining : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDlgTraining)

public:
	CDlgTraining(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgTraining();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TRAINING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
