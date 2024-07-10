#if !defined(AFX_MARKDLG_H__E41CFE46_92C1_4DC6_8F41_069F5C9B4086__INCLUDED_)
#define AFX_MARKDLG_H__E41CFE46_92C1_4DC6_8F41_069F5C9B4086__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CMarkDlg dialog
class CMarkSet;

class CMarkDlg : public CDialog
{
// Construction
public:
	CMarkDlg(CWnd* pParent = NULL);   // standard constructor
    CMarkDlg(CMarkSet* pMarkManage,BOOL bMarkDlg = TRUE,CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CMarkDlg)
	enum { IDD = IDD_DIALOG_MARK };
    CString m_strMark;
	//}}AFX_DATA

public:
    CString& GetMarkString()
    {
        return m_strMark;
    }


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMarkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CMarkSet* m_pMarkManage;
    BOOL m_bMarkDlg;
	// Generated message map functions
	//{{AFX_MSG(CMarkDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListMarks();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MARKDLG_H__E41CFE46_92C1_4DC6_8F41_069F5C9B4086__INCLUDED_)
