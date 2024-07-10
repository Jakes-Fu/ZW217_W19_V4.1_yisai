#if !defined(AFX_DLGOTFSETING_H__A5491BAB_4791_4702_9BB3_856C5F6E1358__INCLUDED_)
#define AFX_DLGOTFSETING_H__A5491BAB_4791_4702_9BB3_856C5F6E1358__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOTFSeting.h : header file
//

#include "CoolListCtrl.h"
#include "Global.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgOTFSeting dialog

class CDlgOTFSeting : public CDialog
{
// Construction
public:
	CDlgOTFSeting(CWnd* pParent = NULL);   // standard constructor
	~CDlgOTFSeting();

	BOOL m_bOtf;

// Dialog Data
	//{{AFX_DATA(CDlgOTFSeting)
	enum { IDD = IDD_DLG_OTF_SETTING };
	CCoolListCtrl	m_lstExFiles;
	CCoolListCtrl	m_lstFiles;
	CComboBox	m_cmbCharSet;
	CString	m_strPointSize;
	CString	m_strFontFile;
	CString	m_strExPtSize;
	BOOL	m_bVector;
	UINT	m_nVectorShrinkPercent;
	BOOL	m_bCmpMore;
	BOOL	m_bCmp;
	int		m_nMaxBearingYDelta;
	//}}AFX_DATA


	CHAR_SET_INFO_PTR m_pCSI;
	CStringArray m_agBdfs;
	CStringArray m_agExBdfs;

	BOOL         m_bGray;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOTFSeting)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOTFSeting)
	afx_msg void OnOfsBrower();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeOfsCmbCharSet();
	afx_msg void OnEndlabeleditFfsLstExfiles(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditFfsLstFiles(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkFfsLstExfiles(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkFfsLstFiles(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAdvSetting();
	afx_msg void OnChkVecFlag();
	afx_msg void OnChkVecCmp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int GetPtSizeArray(LPCTSTR lpszBuf,CUIntArray &agPtSize);

	void ModifyBorder();

	void ClearFtProperty();

	void SetPtInfo(int ** pPtSize, CUIntArray &agPtSize);

private:
	MAP_FTPROP  m_mapFontProperty;
	CString m_strConfigFile;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOTFSETING_H__A5491BAB_4791_4702_9BB3_856C5F6E1358__INCLUDED_)
