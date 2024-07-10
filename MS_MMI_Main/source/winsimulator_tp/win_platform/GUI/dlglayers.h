#if !defined(AFX_DLGLAYERS_H__B88872B3_BFCD_4CDD_A43F_A3AE1902C13C__INCLUDED_)
#define AFX_DLGLAYERS_H__B88872B3_BFCD_4CDD_A43F_A3AE1902C13C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLayers.h : header file
//
#include "ResizableDialog.h"
#include "XTabCtrl.h"
#include "DlgMainLcdLayer.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgLayers dialog

class CDlgLayers : public CResizableDialog
{
// Construction
public:
	CDlgLayers(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgLayers)
	enum { IDD = IDD_DIALOG_UI_LAYER_DISP };
	CXTabCtrl	m_tab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLayers)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLayers)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CDlgMainLcdLayer m_dlgMainLcd;
	CDlgMainLcdLayer m_dlgMainLcd2;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLAYERS_H__B88872B3_BFCD_4CDD_A43F_A3AE1902C13C__INCLUDED_)
