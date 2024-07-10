#if !defined(AFX_DLGEQMODE_H__88A59585_6357_48AE_B5AB_67840E550F89__INCLUDED_)
#define AFX_DLGEQMODE_H__88A59585_6357_48AE_B5AB_67840E550F89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEQMode.h : header file
//
#include "typedef.h"
#include "EqModeStatic.h"
#include "BMPCtrlWithSpecRect.h"
#include "DlgEQMode2.h"
#include "DlgEQMode3.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgEQMode dialog

class CDlgEQMode : public CDialog
{
// Construction
public:
	CDlgEQMode(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgEQMode)
	enum { IDD = IDD_DIALOG_EQ_MODE };
		// NOTE: the ClassWizard will add data members here
	float m_fEQGain;
	int m_nSampleRate;
	//}}AFX_DATA

public:
	void SetPara(EQ_MODE_NV_PARAM_T EQMode, short eqControl,BUTTERWORTH_LCF_T lcf_data );
	BOOL GetPara(EQ_MODE_NV_PARAM_T &EQMode, short &eqControl,BUTTERWORTH_LCF_T& lcf_data );
	void ChangeFilterType( UINT nFilterType );
	void UpdateGain( double lcf_gain_l,double lcf_gain_r ){ m_DlgEQMode2.UpdateGain( lcf_gain_l,lcf_gain_r );}

private:
	CDlgEQMode2 m_DlgEQMode2;
	CDlgEQMode3 m_DlgEQMode3;
	UINT m_nFilterType;
	int m_nSoundEqVer;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEQMode)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEQMode)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEQMODE_H__88A59585_6357_48AE_B5AB_67840E550F89__INCLUDED_)
