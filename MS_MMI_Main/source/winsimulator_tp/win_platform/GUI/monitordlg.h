// MonitorDlg.h : header file
//

#if !defined(AFX_MONITORDLG_H__4E492496_50D7_42A1_9F17_0C13E7ED3DA1__INCLUDED_)
#define AFX_MONITORDLG_H__4E492496_50D7_42A1_9F17_0C13E7ED3DA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "MSSim.h"
#include "HexEdit.h"
#include "CommSock.h"

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg dialog

class CMonitorDlg : public CDialog
{
// Exported Functions
public:
    // Display the buffer content in edit control
    void DisplayBuffer(BYTE *pBuffer, int nLength, CString strSrc);
    // Write the buffer to specified log file
    void WriteLogFile(CString strFileName, BYTE *pBuffer, int nLength);

// Construction
public:
	CMonitorDlg(CWnd* pParent = NULL);	// standard constructor
    ~CMonitorDlg();

// Dialog Data
	//{{AFX_DATA(CMonitorDlg)
	enum { IDD = IDD_MONITOR };
	CHexEdit	m_wndRtosBuffer;
	CHexEdit	m_wndClientBuffer;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonitorDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMonitorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Functions
private:

// Data
private:
    HBRUSH      m_hWhiteBrush;      // Handle of white brush
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONITORDLG_H__4E492496_50D7_42A1_9F17_0C13E7ED3DA1__INCLUDED_)
