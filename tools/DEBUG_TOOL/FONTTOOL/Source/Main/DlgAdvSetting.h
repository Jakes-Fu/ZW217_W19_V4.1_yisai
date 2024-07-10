#if !defined(AFX_DLGADVSETTING_H__3B091022_7871_4F64_AA89_4E27C38FBB6C__INCLUDED_)
#define AFX_DLGADVSETTING_H__3B091022_7871_4F64_AA89_4E27C38FBB6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAdvSetting.h : header file
//

#include "CoolListCtrl.h"
#include "Global.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgAdvSetting dialog

class CDlgAdvSetting : public CDialog
{
// Construction
public:
	CDlgAdvSetting(CWnd* pParent = NULL);   // standard constructor

	void SetPtSizeProp(FONT_PROPERTY_PTR pProp,int nNum);
	void SetExPtSizeProp(FONT_PROPERTY_PTR pProp,int nNum);

// Dialog Data
	//{{AFX_DATA(CDlgAdvSetting)
	enum { IDD = IDD_DLG_ADV_SETTING };
	CCoolListCtrl	m_lstExPtSize;
	CCoolListCtrl	m_lstPtSize;
	BOOL	m_bGlobalOption;
	int		m_nFontType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAdvSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitPtSizeList(CListCtrl * pCtrl,FONT_PROPERTY_PTR pPtSizeProp,int nNum);
	void SetFontType(int nType,CListCtrl * pCtrl);
	// Generated message map functions
	//{{AFX_MSG(CDlgAdvSetting)
	virtual BOOL OnInitDialog();
	afx_msg void OnRdoMono();
	afx_msg void OnRdoGray();
	afx_msg void OnChkGFt();
	virtual void OnOK();
	afx_msg void OnEndlabeleditAsLstPtsize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditAsLstPtsizeEx(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkAsLstPtsize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkAsLstPtsizeEx(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CStringArray m_agFontType;

	FONT_PROPERTY_PTR m_pPtSizeProp;
	FONT_PROPERTY_PTR m_pExPtSizeProp;
	int               m_nPtSizeNum;
	int               m_nExPtSizeNum;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADVSETTING_H__3B091022_7871_4F64_AA89_4E27C38FBB6C__INCLUDED_)
