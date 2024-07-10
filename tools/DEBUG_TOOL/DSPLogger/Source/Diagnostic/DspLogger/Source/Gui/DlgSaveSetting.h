#if !defined(AFX_DLGSAVESETTING_H__79F0A6CA_D997_4EB8_BB77_5D82A9F8111F__INCLUDED_)
#define AFX_DLGSAVESETTING_H__79F0A6CA_D997_4EB8_BB77_5D82A9F8111F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSaveSetting.h : header file
//
typedef enum
{
	MODE_ALL,
	MODE_RECENTLY,
	MODE_CURRENT
};
/////////////////////////////////////////////////////////////////////////////
// CDlgSaveSetting dialog

class CDlgSaveSetting : public CDialog
{
// Construction
public:
	CDlgSaveSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSaveSetting)
	enum { IDD = IDD_DLG_SAVE_SETTING };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
public:
	DWORD m_dwSaveMode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSaveSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSaveSetting)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSAVESETTING_H__79F0A6CA_D997_4EB8_BB77_5D82A9F8111F__INCLUDED_)
