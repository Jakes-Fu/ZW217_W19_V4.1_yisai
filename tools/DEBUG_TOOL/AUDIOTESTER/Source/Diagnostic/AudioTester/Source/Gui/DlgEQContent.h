#if !defined(AFX_DLGEQCONTENT_H__F59A3838_23E1_4D72_B7A1_34AE7F45AEE3__INCLUDED_)
#define AFX_DLGEQCONTENT_H__F59A3838_23E1_4D72_B7A1_34AE7F45AEE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEQContent.h : header file
//
#include "DlgEQMode.h"
#include "XTabCtrl.h"
#include "typedef.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgEQContent dialog

class CDlgEQContent : public CDialog
{
// Construction
public:
	CDlgEQContent(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgEQContent)
	enum { IDD = IDD_DIALOG_EQ_CONTENT };
	int m_nEQSwitch;
	double m_dEQLimit;
	CXTabCtrl m_tabCtrl;
	int		m_nCBFilterType;
	double	m_fLcfGainL;
	double	m_fLcfGainR;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEQContent)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	CArray<CDlgEQMode*,CDlgEQMode*> m_arrDlgEQMode;

public:
	void DoTab(int nTab);
	void SetDlgState(CWnd *pWnd, BOOL bShow);
	void Init();
	void SetPara(AUDIO_EQ_STRUCT_T EQData,BUTTERWORTH_LCF_T* lp_lcf_data);
	BOOL GetPara(AUDIO_EQ_STRUCT_T& EQData,BUTTERWORTH_LCF_T* lp_lcf_data);

private:
	double m_dlimitBak;
	int m_nSoundEqVer;

// Implementation
protected:
	void RefreshControl();

	// Generated message map functions
	//{{AFX_MSG(CDlgEQContent)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTabEqContent(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillfocusEdtLcfGainL();
	afx_msg void OnKillfocusEdtLcfGainR();
	afx_msg void OnSelchangeComboFilterType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEQCONTENT_H__F59A3838_23E1_4D72_B7A1_34AE7F45AEE3__INCLUDED_)
