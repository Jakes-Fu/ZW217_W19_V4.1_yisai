#if !defined(AFX_FINDTPDLG_H__93493392_B78B_4727_B013_3794A162BC64__INCLUDED_)
#define AFX_FINDTPDLG_H__93493392_B78B_4727_B013_3794A162BC64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindTpDlg.h : header file
//

#include "DspKernelExport.h"

/////////////////////////////////////////////////////////////////////////////
// CFindTpDlg dialog

class CTestPointsView;

class CFindTpDlg : public CDialog
{
// Construction
public:
	CFindTpDlg(CWnd* pParent = NULL);   // standard constructor

public:

    void SetFindData( PTP_FIND_DATA pFindData, int nNum, int nSize )
    {
        _ASSERTE( pFindData != NULL && nSize > 0 );
        _ASSERTE( nNum <= nSize );

        m_pFindData = pFindData;
        m_nFindNum  = nNum;
        m_nFindSize = nSize;
    }

    int m_nFindNum;

protected:
    
// Dialog Data
	//{{AFX_DATA(CFindTpDlg)
	enum { IDD = IDD_DLG_FIND_TP };
	CListCtrl	m_lstInfo;
	CComboBox	m_cmbAddr;
	CComboBox   m_cmbData;
	CString	m_strAddr;
	CString	m_strData;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindTpDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
   
    PTP_FIND_DATA m_pFindData;
    int           m_nFindSize;

	// Generated message map functions
	//{{AFX_MSG(CFindTpDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDel();
	afx_msg void OnItemchangedLstInfo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDTPDLG_H__93493392_B78B_4727_B013_3794A162BC64__INCLUDED_)
