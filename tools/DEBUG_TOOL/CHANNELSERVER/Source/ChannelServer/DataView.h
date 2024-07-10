#if !defined(AFX_DATAVIEW_H__25A0F86F_9565_45A2_A875_CD68D605F261__INCLUDED_)
#define AFX_DATAVIEW_H__25A0F86F_9565_45A2_A875_CD68D605F261__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataView.h : header file
//
#include "resource.h"
#include "HexEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDataView dialog

class CDataView : public CDialog
{
// Construction
public:
	CDataView(CWnd* pParent = NULL);   // standard constructor
    ~CDataView();

// Dialog Data
	//{{AFX_DATA(CDataView)
	enum { IDD = IDD_DLG_DATA_LOG };
	CHexEdit	m_wndToolToCSBuffer;
	CHexEdit	m_wndCSToToolBuffer;
	CHexEdit	m_wndMSToCSBuffer;
	CHexEdit	m_wndCSToMSBuffer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support	
	//}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDataView)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnClear();
	afx_msg void OnHideArmlog();
	//}}AFX_MSG
    afx_msg void OnShowData(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

// Data
private:
	
	FILE *m_fpToolToCSLog;
    FILE *m_fpCSToMSLog;
    FILE *m_fpMSToCSLog;
    FILE *m_fpCSToToolLog;

    HBRUSH      m_hWhiteBrush;      // Handle of white brush

	BOOL m_bHideArmLog;

public:
    BOOL        m_bEnableShow;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAVIEW_H__25A0F86F_9565_45A2_A875_CD68D605F261__INCLUDED_)
