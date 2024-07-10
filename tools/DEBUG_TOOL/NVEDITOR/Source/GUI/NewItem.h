#if !defined(AFX_NEWITEM_H__40B224A1_F04D_4028_AEC2_814531DC022D__INCLUDED_)
#define AFX_NEWITEM_H__40B224A1_F04D_4028_AEC2_814531DC022D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewItem.h : header file
//


class CFileParser;  //lint !e763
/////////////////////////////////////////////////////////////////////////////
// CNewItem dialog

class CNewItem : public CDialog
{
// Construction
public:
	void LoadParent();
	void SetProject(CFileParser *pProject, UINT nModuleIndex);
	CNewItem(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewItem)
	enum { IDD = IDD_NEW_ITEM };
	CEdit	m_edtStrLen;
	CEdit	m_ctlItemArrSize;
	CComboBox	m_ctlItemType;
	CComboBox	m_ctlItemParent;
	CButton	m_ctlBtnOk;
	CButton	m_ctlBtnLast;
	CButton	m_ctlBtnNext;
	CString	m_szItemName;
	CString	m_szItemType;
	CString	m_szItemParent;
	CString	m_szItemDesc;
	int		m_nItemID;
	int		m_nArraySize;
	BOOL	m_bCheckArray;
	UINT	m_nStrlen;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewItem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewItem)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnLast();
	afx_msg void OnBtnNext();
	virtual void OnOK();
	afx_msg void OnChangeItemName();
	afx_msg void OnSelchangeItemType();
	afx_msg void OnCheckArray();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CFileParser * m_pProject;

    INT m_nCurrItemIndex;
	INT m_nTotalItemCount;
    INT m_nModuleIndex;


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWITEM_H__40B224A1_F04D_4028_AEC2_814531DC022D__INCLUDED_)
