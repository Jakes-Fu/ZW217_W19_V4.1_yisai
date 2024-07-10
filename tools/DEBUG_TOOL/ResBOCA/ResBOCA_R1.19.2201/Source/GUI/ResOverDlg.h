// ResOverDlg.h : header file
//

#if !defined(AFX_RESOVERDLG_H__D193E471_E85A_4BD7_BB3B_0438B84CBDCA__INCLUDED_)
#define AFX_RESOVERDLG_H__D193E471_E85A_4BD7_BB3B_0438B84CBDCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ResParser.h"
#include "ImgView.h"


/////////////////////////////////////////////////////////////////////////////
// CResOverDlg dialog

class CResOverDlg : public CDialog
{
// Construction
public:
	CResOverDlg(CWnd* pParent = NULL);	// standard constructor
	~CResOverDlg();

// Dialog Data
	//{{AFX_DATA(CResOverDlg)
	enum { IDD = IDD_RESOVER_DIALOG };
	CProgressCtrl	m_prog;
	CImgView  	m_imgView;
	CComboBox	m_cmbTheme;
	CComboBox	m_cmbResType;
	CListCtrl	m_lstResItem;
	CString	m_strItemInfo;
	CString	m_strResInfo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResOverDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CResOverDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnItemchangedLstRes(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLoad();
	afx_msg void OnReplace();
	afx_msg void OnSelchangeTheme();
	afx_msg void OnSave();
	afx_msg void OnUndo();
	afx_msg void OnSelchangeResType();
	afx_msg void OnHelp();
	afx_msg void OnExit();
	afx_msg void OnBtnLoadThmDir();
	afx_msg void OnBtnExportThmDir();
	afx_msg void OnExpStr();
	afx_msg void OnImpStr();
	afx_msg void OnEdtMtva();
	afx_msg void OnEdtNtac();
	afx_msg void OnEdtBkmk();
	//}}AFX_MSG
	afx_msg LRESULT OnProgressMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
//private:
public:
	CResParser  m_resParser;
	CImageList* m_pImglist;
	BOOL        m_bInitList;
	CString     m_strTmpFolder;
	CString     m_strResFile;
	BOOL        m_bImportString;
	CString     m_strExcelFile;
	BOOL        m_bDisable;
	BOOL        m_bSaveOk;
	BOOL        m_bSaving;
	CString     m_strSaveFile;
	
//private:
public:
	void ChangeItemState(BOOL bLoadOK);
	void InitCtrls();
	void FillReslist();
	CString GetAnimFolder(int nItemIndex, int nThemeIndex);
	void Load(LPCTSTR lpszResFile);

	CString GetFolder(LPCTSTR lpszTitle = NULL);
	void    ChgCtrlStatus(BOOL bEnable =TRUE);

public:
	static DWORD WINAPI GetThreadFunc(LPVOID lpParam);
	DWORD ImpExpFunc();
	
	afx_msg void OnBnClickedBtcEdtMtva();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESOVERDLG_H__D193E471_E85A_4BD7_BB3B_0438B84CBDCA__INCLUDED_)
