#if !defined(AFX_PBDLG_H__B52D98A2_02D4_4C5D_9F82_2FDC2B73BDE0__INCLUDED_)
#define AFX_PBDLG_H__B52D98A2_02D4_4C5D_9F82_2FDC2B73BDE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PBDlg.h : header file
//
#include "..\effect\CoolEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CPBDlg dialog

struct _PB;
class IPB;
class CInfoWrapper;
class CPBDlg : public CDialog
{
// Construction
public:
	CPBDlg(CWnd* pParent = NULL);   // standard constructor

	_PB * m_pPB;
	BOOL  m_bNew;
	CStringArray m_arrGroupName;
// Dialog Data
	//{{AFX_DATA(CPBDlg)
	enum { IDD = IDD_DLG_PB };
	CStatic	m_stcPlace;
	CStatic m_stcGroup;
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CString	m_strEmail;
	CString	m_strHomePhone;
	CString	m_strName;
	CString	m_strOfficePhone;
	CString	m_strPersonPhone;
	CString m_strFax;
	int		m_nStore;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPBDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitCtrls(void);
	void EnableNoneSimInfo(BOOL bEnable);
	BOOL CheckPhoneNum();

	void SetSMSPlace( WORD wStore );

	// Store index to store ID
	__inline int GetStoreIDFromIdx(int nIdx);
	// Store ID to store index
	__inline int GetStoreIdxFromID(int nID);

	// Generated message map functions
	//{{AFX_MSG(CPBDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void OnClose();
	afx_msg void OnSelchangeCmbStore();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int  m_nUsedInPhone;
	int  m_nTotalInPhone;
	int  m_nUsedInSIM;
	int  m_nTotalInSIM;
	
	int  m_nUsedInSIM2;
	int  m_nTotalInSIM2;

	int  m_nUsedInSIM3;
	int  m_nTotalInSIM3;

	int  m_nUsedInSIM4;
	int  m_nTotalInSIM4;
	
	enum{
		STORE_IN_PC = 0,
		STORE_IN_BM = 1,
		STORE_IN_SM = 2,
		STORE_IN_SM2 = 3,
		STORE_IN_SM3 = 4,
		STORE_IN_SM4 = 5
	};

	CCoolEdit m_edtName;
	CCoolEdit m_edtPersonPhone;
	CCoolEdit m_edtHomePhone;
	CCoolEdit m_edtOfficePhone;
	CCoolEdit m_edtEmail;
	CCoolEdit m_edtFax;
    CComboBox m_cmbGroup;
	CComboBox m_cmbStore;

	int            m_nOldStore;
	CInfoWrapper * m_pIW;
	IPB          * m_pPBFactory;

	CMap<int, int, int, int> m_mapSPIndex;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PBDLG_H__B52D98A2_02D4_4C5D_9F82_2FDC2B73BDE0__INCLUDED_)
