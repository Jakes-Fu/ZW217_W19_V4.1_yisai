#if !defined(AFX_DLGMAINLCDLAYER_H__33B62BEB_9B50_4E4E_9B40_53A6031117FD__INCLUDED_)
#define AFX_DLGMAINLCDLAYER_H__33B62BEB_9B50_4E4E_9B40_53A6031117FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMainLcdLayer.h : header file
//
#include "ResizableDialog.h"
#include "CoolStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMainLcdLayer dialog

class CDlgMainLcdLayer : public CResizableDialog
{
// Construction
public:
	CDlgMainLcdLayer(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CDlgMainLcdLayer)
	enum { IDD = IDD_DIALOG_MAIN_LCD_UI_LAYER };
	CCoolStatic	m_stcDisp;
	BOOL	m_bDrawBorer;
	BOOL	m_bDrawRect;
	BOOL	m_bDrawInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMainLcdLayer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	BOOL Init(DISPLAY_BLEND_LAYER_T *pLayerInfo, DWORD dwLcdHeight, DWORD dwLcdWidth);
	void LayerInvalidate();
	void LayerInvalidateRect(WORD left,WORD top,WORD right,WORD bottom);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMainLcdLayer)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDrawBorder();
	afx_msg void OnDrawRect();
	afx_msg void OnDrawInfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL m_bShow;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMAINLCDLAYER_H__33B62BEB_9B50_4E4E_9B40_53A6031117FD__INCLUDED_)
