#if !defined(AFX_DLGEQCOEFS_H__CDD5D264_E16E_4FF5_94A4_3C31FB49FFEC__INCLUDED_)
#define AFX_DLGEQCOEFS_H__CDD5D264_E16E_4FF5_94A4_3C31FB49FFEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEqCoefs.h : header file
//
#include "ListCtrlEx.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgEqCoefs dialog

class CDlgEqCoefs : public CDialog
{
// Construction
public:
	CDlgEqCoefs(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgEqCoefs)
	enum { IDD = IDD_DIALOG_PEQ_COEFS };
		// NOTE: the ClassWizard will add data members here
	CListCtrlEx m_lst;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEqCoefs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
public:

	void SetItemValue(short *pItem, int nItemCount);
	void GetItemValue(short *pItem);
	void SetDlgTitle(CString strTitle){m_strTitle = strTitle;}

private:

	void InitList(CString strItemName);


private:

	int m_nItemCount;
	short *m_pItem;
	CString m_strTitle;


// Implementation
protected:
	typedef CListCtrlEx::COLUMN_DATA LIST_COLUME;
	typedef std::vector<LIST_COLUME>ROW_DATA;   
	typedef std::vector<ROW_DATA> LINE_DATA;
	LINE_DATA m_vAllLinesData;

	// Generated message map functions
	//{{AFX_MSG(CDlgEqCoefs)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	virtual void OnOK();
	afx_msg void OnButtonImport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEQCOEFS_H__CDD5D264_E16E_4FF5_94A4_3C31FB49FFEC__INCLUDED_)
