#if !defined(AFX_EDITITEMID_H__B08942C8_0BD1_4F58_BD91_3E2993F70641__INCLUDED_)
#define AFX_EDITITEMID_H__B08942C8_0BD1_4F58_BD91_3E2993F70641__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditItemID.h : header file
//
class CFileParser;
/////////////////////////////////////////////////////////////////////////////
// CEditItemID dialog

class CEditItemID : public CDialog
{
// Construction
public:
	void SetProject(CFileParser *pProject, UINT nModuleIndex, UINT nItemIndex);
	UINT GetNewID();
	CEditItemID(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditItemID)
	enum { IDD = IDD_CHANGID_DLG };
	CEdit	m_ctrlItemName;
	CEdit	m_ctrlOldID;
	CEdit	m_ctrlNewID;
	CString m_strName;
	UINT	m_nOldItemID;
	

	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditItemID)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEditItemID)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CFileParser * m_pProject;
	
    INT m_nCurrItemIndex;
	INT m_nTotalItemCount;
    INT m_nModuleIndex;

	UINT    m_nNewID;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITITEMID_H__B08942C8_0BD1_4F58_BD91_3E2993F70641__INCLUDED_)
