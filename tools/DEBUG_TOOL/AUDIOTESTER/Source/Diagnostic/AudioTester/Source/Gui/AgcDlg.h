#if !defined(AFX_AGCDLG_H__AFEFF8E7_513F_47B5_A7E2_89A7E4758CAE__INCLUDED_)
#define AFX_AGCDLG_H__AFEFF8E7_513F_47B5_A7E2_89A7E4758CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AgcDlg.h : header file
//
#include "ALCStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CAgcDlg dialog
class SpecialRect;

class CAgcDlg : public CDialog
{
// Construction
public:
	CAgcDlg(CWnd* pParent = NULL);   // standard constructor

    SpecialRect* m_pArea;

    
// Dialog Data
	//{{AFX_DATA(CAgcDlg)
	enum { IDD = IDD_DIALOG_AGC };
	BOOL	m_bWriteImmediately;
	int		m_nSwitch;
	short	m_nALCDPthreshold;
	short	m_nALCcprsThreshold;
	short	m_nALCcprsLimit;
	short	m_nALCexpdThreshold;
	short	m_nALCexpdLimit;
	short	m_nALCcprsRatio;
	short	m_nALCexpdRatio;
	CALCStatic m_stcGraph;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAgcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	void RefeshGraph();

// Implementation
protected:
	BOOL m_bAudio4;
	//teanahu 2011.03.24
	BOOL m_bAudio41;
	//

	// Generated message map functions
	//{{AFX_MSG(CAgcDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeEditAlcExpandLimit();
	afx_msg void OnChangeEditAlcExpandThreshold();
	afx_msg void OnChangeEditAlcCompressThreshold();
	afx_msg void OnChangeEditAlcCompressLimit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AGCDLG_H__AFEFF8E7_513F_47B5_A7E2_89A7E4758CAE__INCLUDED_)
