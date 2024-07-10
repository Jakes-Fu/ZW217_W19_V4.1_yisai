#if !defined(AFX_AECDIALOG_H__21031900_D065_44A6_8A55_1DB461FC00F4__INCLUDED_)
#define AFX_AECDIALOG_H__21031900_D065_44A6_8A55_1DB461FC00F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AecDialog.h : header file
//

#include "BMPCtrlWithSpecRect.h"

/////////////////////////////////////////////////////////////////////////////
// CAecDialog dialog

class CAecDialog : public CDialog
{
// Construction
public:
	CAecDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAecDialog)
	enum { IDD = IDD_DIALOG_AEC };
	CButton	m_btnAdvance;
	int		m_nUlFirHpf;
	int		m_nAecEnable;
	short	m_nPdelay;
	int		m_nDlRefHpfData;
	int		m_nDlRefHpfPga;
	short	m_nFirTaps;
	int		m_nAecFrozen;
	int		m_nCoeffFrozen;
	short	m_nDtDectThreshold;
	short	m_nDtNoiseFloor;
	short	m_nStepSize;
	short	m_nCoeffNormShift;
	short	m_nSendAttenuInDt;
	short	m_nSendAttenuInRv;
	short	m_nSendThreshold;
	short	m_nR2dtThreshold;
	short	m_nRecvThreshold;
	short	m_nS2dtThreshold;
	short	m_nBn40;
	BOOL	m_bWriteToPhone;
	short	m_nSaDelay;
	short	m_nSignalDelay;
	int		m_nSaCtrl;
	int		m_nDecorFilter;
	short	m_nCGNGain;
	//}}AFX_DATA

    SpecialRectOfAec* m_pArea;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAecDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    BOOL m_bAdvance;
    void ShowAdvance();

	//teana hu 2011.02.14
	BOOL m_bAudio4;

	// Generated message map functions
	//{{AFX_MSG(CAecDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonAecAdvance();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AECDIALOG_H__21031900_D065_44A6_8A55_1DB461FC00F4__INCLUDED_)
