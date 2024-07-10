#if !defined(AFX_XAGCDLG_H__5E5C0282_AE23_4A08_AD69_4DF098368AF6__INCLUDED_)
#define AFX_XAGCDLG_H__5E5C0282_AE23_4A08_AD69_4DF098368AF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XagcDlg.h : header file
//
#include "BMPCtrlWithSpecRect.h"

/////////////////////////////////////////////////////////////////////////////
// CXagcDlg dialog

class CXagcDlg : public CDialog
{
// Construction
public:
	CXagcDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CXagcDlg)
	enum { IDD = IDD_DIALOG_XAGC };
	int		m_nMode;
	int		m_nNoiseGateOn;
	int		m_nTargetTight;
	int		m_nAttack;
	int		m_nDecay;
	int		m_nFrameTime;
	int		m_nHoldTime;
	int		m_nMaxPgaGain;
	int		m_nMinPgaGain;
	int		m_nNoiseGateAttenu;
	int		m_nNoiseGateThresh;
	int		m_nPgaStep;
	int		m_nTarget;
	int		m_nNoiseGateHold;
	//}}AFX_DATA

    SpecialRectOfAec* m_pArea;
    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXagcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXagcDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XAGCDLG_H__5E5C0282_AE23_4A08_AD69_4DF098368AF6__INCLUDED_)
