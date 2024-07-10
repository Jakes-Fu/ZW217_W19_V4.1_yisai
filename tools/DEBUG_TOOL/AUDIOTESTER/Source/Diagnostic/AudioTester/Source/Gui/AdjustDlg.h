#if !defined(AFX_ADJUSTDLG_H__4B11D540_9308_4C52_90E8_8CF647BD7F45__INCLUDED_)
#define AFX_ADJUSTDLG_H__4B11D540_9308_4C52_90E8_8CF647BD7F45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdjustDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdjustDlg dialog

class CAdjustDlg : public CDialog
{
// Construction
public:
	CAdjustDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAdjustDlg)
	enum { IDD = IDD_DIALOG_ADJUST };
	CString	m_strByte0;
	CString	m_strByte1;
	CString	m_strByte2;
	CString	m_strByte3;
	CString	m_strByte4;
	CString	m_strByte5;
	CString	m_strByte6;
	CString	m_strByte7;
	CString	m_strB0;
	CString	m_strB1;
	CString	m_strB2;
	CString	m_strB3;
	CString	m_strB4;
	CString	m_strB5;
	CString	m_strB6;
	CString	m_strB7;
	//}}AFX_DATA

	UINT	m_uByte0;
	UINT	m_uByte1;
	UINT	m_uByte2;
	UINT	m_uByte3;
	UINT	m_uByte4;
	UINT	m_uByte5;
	UINT	m_uByte6;
	UINT	m_uByte7;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdjustDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
    void Init( UINT uData,UINT uStartId );
    UINT GetResult();

protected:
    CString m_strTitle;
    CString m_strHiword;
    CString m_strLoword;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdjustDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADJUSTDLG_H__4B11D540_9308_4C52_90E8_8CF647BD7F45__INCLUDED_)
