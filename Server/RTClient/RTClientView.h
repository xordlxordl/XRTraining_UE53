
// RTClientView.h: CRTClientView 클래스의 인터페이스
//

#pragma once

#include "resource.h"
#include "ColorSheet.h"
#include "DlgTraining.h"
#include "DlgNet.h"
class CRTClientView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CRTClientView() noexcept;
	DECLARE_DYNCREATE(CRTClientView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_RTCLIENT_FORM };
#endif

public:

	CRichEditCtrl m_Edit_FileName;

	CColorSheet	m_Sheet;
	CDlgTraining m_DlgTraining;
	CDlgNet m_DlgNet;

// 특성입니다.
public:
	CRTClientDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CRTClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonSend();
	
	afx_msg void OnBnClickedButtonLoadSct();
	
	
	
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSize( UINT nType, int cx, int cy );
};

#ifndef _DEBUG  // RTClientView.cpp의 디버그 버전
inline CRTClientDoc* CRTClientView::GetDocument() const
   { return reinterpret_cast<CRTClientDoc*>(m_pDocument); }
#endif

