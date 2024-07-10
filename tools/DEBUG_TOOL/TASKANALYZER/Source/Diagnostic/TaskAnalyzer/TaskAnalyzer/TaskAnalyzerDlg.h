// TaskAnalyzerDlg.h : header file
//

#if !defined(AFX_TASKANALYZERDLG_H__B6A1B840_A6B9_4286_974A_6784C22D387A__INCLUDED_)
#define AFX_TASKANALYZERDLG_H__B6A1B840_A6B9_4286_974A_6784C22D387A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NumericEdit.h"
#include "ColorListBox.h"   
#include "resource.h"
#include "TaskSwitchFunc.h" 

#define NUMERIC_EDIT_NUM 7
#define LIST_LEN 1024
#define FAIL_COLOR RGB(255,0,0)
#define NORMAL_COLOR RGB(0,0,0)

/////////////////////////////////////////////////////////////////////////////
// CTaskAnalyzerDlg dialog

class CTaskAnalyzerDlg : public CDialog
{
// Construction
public:
	CTaskAnalyzerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTaskAnalyzerDlg)
	enum { IDD = IDD_TASKANALYZER_DIALOG };
	CComboBox	m_cboProductList;
	CButton m_btnByMs;
	CButton m_btnByUs;
	CString m_strEdtDescribe;
	CColorListBox m_lstBox;
	CButton m_btnEndian;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskAnalyzerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	CNumericEdit m_editSetting[NUMERIC_EDIT_NUM];
	CString m_strSelectProduct;

	// Generated message map functions
	//{{AFX_MSG(CTaskAnalyzerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnMapFileBtn();
	afx_msg void OnSelchangeComboProduct();
	afx_msg void OnChangeCheck();
	afx_msg void OnSourceFileBtn();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	afx_msg void OnCheckEndian();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:
	void SetSettingItemsStatus( BOOL bStatus = TRUE );
	void WriteIniFile();
	BOOL GetMemParam(const CString &szFileName);
public:
	void AddTextToList(const CString &strText, COLORREF clr, BOOL bModify);
private:
	char m_szTaskList[LIST_LEN];
	char m_szIntList[LIST_LEN];
	CString m_strIniFile;
	UINT m_uiMaxFileSize;		//file size
	UINT m_uiDataAreaSize;		//entry[0] size
	UINT m_uiShareOffset;	
	UINT m_uiIramOffset;	
	UINT m_uiRegOffset;	
	UINT m_uiDspOffset;	
	UINT m_uiShareMemorySize;	//entry[2] size
	UINT m_uiIramSize;			//entry[3] size
	UINT m_uiRegSize;			//entry[4] size
	UINT m_uiOffSize;			//entry[0] start addr
	UINT m_uiDspSize;			//entry[1] size;

	BOOL m_bNewVersion;
	
	//teana.hu 2011.01.17
	TM_INFO m_TMInfo;
public:
	//teana hu 2010.04.09
	CString m_strErr;
	//

	//teana hu 2010.08.30
	BOOL m_bBigEndian;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKANALYZERDLG_H__B6A1B840_A6B9_4286_974A_6784C22D387A__INCLUDED_)
