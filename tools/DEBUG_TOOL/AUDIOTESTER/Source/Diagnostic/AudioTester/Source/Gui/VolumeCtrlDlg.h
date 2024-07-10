#if !defined(AFX_VOLUMECTRLDLG_H__C7F88DC0_331D_4416_9552_8E6B8BB01B0F__INCLUDED_)
#define AFX_VOLUMECTRLDLG_H__C7F88DC0_331D_4416_9552_8E6B8BB01B0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VolumeCtrlDlg.h : header file
//

class SpecialRectOfVolumeCtrl;
/////////////////////////////////////////////////////////////////////////////
// CVolumeCtrlDlg dialog

class CVolumeCtrlDlg : public CDialog
{
// Construction
public:
	CVolumeCtrlDlg(CWnd* pParent = NULL);   // standard constructor

public:
    SpecialRectOfVolumeCtrl* m_pVolumeCtrlArea;

// Dialog Data
	//{{AFX_DATA(CVolumeCtrlDlg)
	enum { IDD = IDD_DIALOG_VOLUME_CTRL };
	CListBox	m_listValue;
	BOOL	m_bWriteImmediately;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVolumeCtrlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVolumeCtrlDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    void InitListBox();
    void ChangeFont();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VOLUMECTRLDLG_H__C7F88DC0_331D_4416_9552_8E6B8BB01B0F__INCLUDED_)
