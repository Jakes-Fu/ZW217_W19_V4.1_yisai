#if !defined(AFX_MSGFLOWPROPERTYPAGE_H__3BF67A4E_51DC_4F45_80D5_1DC03054B3DE__INCLUDED_)
#define AFX_MSGFLOWPROPERTYPAGE_H__3BF67A4E_51DC_4F45_80D5_1DC03054B3DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgFlowPropertyPage.h : header file
//
#include "stdafx.h"
#include "resource.h"
#include "MsgFlowView.h"
#include "ConPanLayer.h" 

/////////////////////////////////////////////////////////////////////////////
// CMsgFlowPropertyPage dialog

class CMsgFlowPropertyPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CMsgFlowPropertyPage)

// Construction
public:
	CMsgFlowPropertyPage();
	virtual ~CMsgFlowPropertyPage();

// Dialog Data
	//{{AFX_DATA(CMsgFlowPropertyPage)
	enum { IDD = IDD_DIALOG_MSGFLOW };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMsgFlowPropertyPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CString m_csMsgFlowFile;
    UINT    m_nFileLastLength;
    CMsgFlowView * m_pevMsgFlow;
    BOOL    m_bScrollToEnd;
    BOOL    m_bRefreshNewMsg;
protected:
	// Generated message map functions
	//{{AFX_MSG(CMsgFlowPropertyPage)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonOpenFile();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonSavefile();
	afx_msg void OnCheckScrend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGFLOWPROPERTYPAGE_H__3BF67A4E_51DC_4F45_80D5_1DC03054B3DE__INCLUDED_)
