#if !defined(AFX_STNGDLG_H__CC8FBE29_1AEB_4DAF_AC1B_E277D2D1863B__INCLUDED_)
#define AFX_STNGDLG_H__CC8FBE29_1AEB_4DAF_AC1B_E277D2D1863B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StNgDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStNgDlg dialog

class SpecialRectOfStng;


class CStNgDlg : public CDialog
{
// Construction
public:
	CStNgDlg(CWnd* pParent = NULL);   // standard constructor
	SpecialRectOfStng* m_pArea;

// Dialog Data
	//{{AFX_DATA(CStNgDlg)
	enum { IDD = IDD_DIALOG_SIDETONE_NG };
	int		m_st_ng_switch;
	int		m_st_att;
	int		m_st_holdc;
	int		m_st_noise_threshold;
	int		m_st_voice_threshold;
	int		m_st_zcr_ceil;
	int		m_st_zcr_threshold;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStNgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStNgDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STNGDLG_H__CC8FBE29_1AEB_4DAF_AC1B_E277D2D1863B__INCLUDED_)
