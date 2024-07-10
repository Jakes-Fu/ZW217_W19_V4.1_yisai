#if !defined(AFX_PBGDLG_H__788869AC_4688_435B_BCF6_8DDA33F41FE7__INCLUDED_)
#define AFX_PBGDLG_H__788869AC_4688_435B_BCF6_8DDA33F41FE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PbgDlg.h : header file
//

#include "..\effect\CoolEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CPbgDlg dialog

struct _PBGROUP;
class IPBG;
class CInfoWrapper;

class CPbgDlg : public CDialog
{
// Construction
public:
	CPbgDlg(CWnd* pParent = NULL);   // standard constructor
	
	_PBGROUP * m_pPBG;
	BOOL       m_bNew;
// Dialog Data
	//{{AFX_DATA(CPbgDlg)
	enum { IDD = IDD_DLG_PBG };
	CStatic	m_stcPlace;
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CComboBox m_cmbStore;
	CString	m_strName;
	int		m_nStore;
	int		m_nMemNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPbgDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitCtrls(void);
	//void EnableNoneSimInfo(BOOL bEnable);

	void SetSMSPlace( WORD wStore );

	// Store index to store ID
	__inline int GetStoreIDFromIdx(int nIdx);
	// Store ID to store index
	__inline int GetStoreIdxFromID(int nID);

	// Generated message map functions
	//{{AFX_MSG(CPbgDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int  m_nUsedInPhone;
	int  m_nTotalInPhone;

	CCoolEdit m_edtName;
	CCoolEdit m_edtMemNum;

	int            m_nOldStore;
	CInfoWrapper * m_pIW;
	IPBG         * m_pPBGFactory;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PBGDLG_H__788869AC_4688_435B_BCF6_8DDA33F41FE7__INCLUDED_)
