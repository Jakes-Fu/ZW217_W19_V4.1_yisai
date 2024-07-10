#if !defined(AFX_SMSPRESETDLG_H__A3F4245D_2A14_4CC1_AFA8_302BEB1BF52A__INCLUDED_)
#define AFX_SMSPRESETDLG_H__A3F4245D_2A14_4CC1_AFA8_302BEB1BF52A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SMSPresetDlg.h : header file
//

#include "..\effect\CoolEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CSMSPresetDlg dialog
class CInfoWrapper;
class CSMSPresetDlg : public CDialog
{
// Construction
public:
	void EnableSelect(BOOL bEnable = TRUE);
	CSMSPresetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSMSPresetDlg)
	enum { IDD = IDD_DLG_SMSPRESET };
	CButton	m_btnExit;
	CButton	m_btnSelect;
	CButton	m_btnOK;
	CButton	m_btnNew;
	CButton	m_btnEdit;
	CButton	m_btnDelete;
	CButton	m_btnCancel;
	CListBox	m_lstCaption;
	CString	m_strCaption;
	CString	m_strContent;
	CString	m_strContentView;
	CString	m_strInputNumE;
	CString	m_strLeftNumE;
	CString	m_strContentNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMSPresetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitCtrls(void);
	void FillCaptionList(void);
	void ShowEditSection(void);

	// Generated message map functions
	//{{AFX_MSG(CSMSPresetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSelect();
	afx_msg void OnBtnNew();
	afx_msg void OnBtnEidt();
	afx_msg void OnBtnDelete();
	afx_msg void OnBtnOk();
	afx_msg void OnBtnCancel();
	afx_msg void OnSelchangeLstCaption();
	afx_msg void OnVscrollEdtContent();
	afx_msg void OnChangeEdtContent();
	afx_msg void OnBtnExit();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bEdit;
	BOOL m_bEnableSelect;
	BOOL m_bShowEdit;
	CRect m_rcDlg;
	CRect m_rcEditPos;

	CCoolEdit m_edtCaption;
	CCoolEdit m_edtContent;

	CInfoWrapper * m_pIW;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSPRESETDLG_H__A3F4245D_2A14_4CC1_AFA8_302BEB1BF52A__INCLUDED_)
