#if !defined(AFX_REPLAYSETTING_H__AEC12A23_8DEB_4A9D_94C0_4592BE58896D__INCLUDED_)
#define AFX_REPLAYSETTING_H__AEC12A23_8DEB_4A9D_94C0_4592BE58896D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReplaySetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReplaySetting dialog

class CReplaySetting : public CDialog
{
// Construction
public:
	CReplaySetting(CWnd* pParent = NULL);   // standard constructor

public:
	UINT m_nMaxNum;

// Dialog Data
	//{{AFX_DATA(CReplaySetting)
	enum { IDD = IDD_DLG_REPLAY_SETTING };
	UINT m_nEdtFrom;
	UINT m_nEdtTo;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReplaySetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReplaySetting)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPLAYSETTING_H__AEC12A23_8DEB_4A9D_94C0_4592BE58896D__INCLUDED_)
