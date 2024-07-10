#if !defined(AFX_NEWITEMINFO_H__8D7C3DA9_C2C1_4B23_86E8_67B6EF21DBBE__INCLUDED_)
#define AFX_NEWITEMINFO_H__8D7C3DA9_C2C1_4B23_86E8_67B6EF21DBBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewItemInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewItemInfo dialog

class CNewItemInfo : public CDialog
{
// Construction
public:
    void SetItemFullType(void);
	void ClearItemType(void);
	void AddItemType(const CString & strType);
	CNewItemInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewItemInfo)
	enum { IDD = IDD_NEW_ITEM_INFO };
	CEdit	m_edtStrLen;
	CEdit	m_ctrlID;
	CEdit	m_ctrlArraySize;
	CButton	m_ctrlArray;
	CComboBox	m_ctlCombo;
	CString	m_strName;
	BOOL	m_fIsArray;
	int		m_nArraySize;
	CString	m_strDesc;
	CString	m_strDefaultValue;
	UINT	m_nItemID;
	UINT	m_nStrLen;
	//}}AFX_DATA
	UINT     m_nItemType;
	BOOL     m_fNeedID;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewItemInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
    //UINT m_uItemType;
	UINT GetItemType( LPCTSTR lpszTypeName );
    bool CheckStringLen() const;

	// Generated message map functions
	//{{AFX_MSG(CNewItemInfo)
	afx_msg void OnSelchangeComboType();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCheckArray();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	CString m_strType;
	
    // 存放条目类型
    CStringArray m_strArrType;
    static const _TCHAR * m_lpItemType[];
    static const int m_nTypeCount;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWITEMINFO_H__8D7C3DA9_C2C1_4B23_86E8_67B6EF21DBBE__INCLUDED_)
