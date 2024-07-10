#if !defined(AFX_DLGEQSET_H__B95255B9_4006_4E86_AE41_B70573B09787__INCLUDED_)
#define AFX_DLGEQSET_H__B95255B9_4006_4E86_AE41_B70573B09787__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEQSet.h : header file
//
#include "DlgEQContent.h"
#include "AudioTesterSheet.h"
#include "XTabCtrl.h"
#include "typedef.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgEQSet dialog

class CDlgEQSet : public CDialog
{
// Construction
public:
	CDlgEQSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgEQSet)
	enum { IDD = IDD_DIALOG_EQ_SET };
		// NOTE: the ClassWizard will add data members here
	CXTabCtrl m_tabCtrl;
	//}}AFX_DATA
public:
	CArray<CDlgEQContent*,CDlgEQContent*> m_arrDlgEQContent;
	int m_nCurrentPage;
	CAudioTesterSheet *m_pSheet;

public:
	void DoTab(int nTab);
	void SetDlgState(CWnd *pWnd, BOOL bShow);
	void InitToolbar();
	void GetEQPara();

protected:
	BOOL GetLcfData();
	BOOL SetLcfData();

private:
	CToolBar m_toolBarEQ;
	EQ_DATA m_EQData;
	BUTTERWORTH_LCF_T m_lcfData[TOTAL_EQ_MODE_NUM];	
	int m_nSoundEqVer;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEQSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEQSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTabEqSet(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnButtonEqWriteCurrentModeToFlash();
	afx_msg void OnButtonEqWriteCurrentModeToPhone();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnOK();
	afx_msg void OnButtonEqLoadCurrentModeFromFlash();
	afx_msg void OnButtonEqLoadCurrentModeFromPhone();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEQSET_H__B95255B9_4006_4E86_AE41_B70573B09787__INCLUDED_)
