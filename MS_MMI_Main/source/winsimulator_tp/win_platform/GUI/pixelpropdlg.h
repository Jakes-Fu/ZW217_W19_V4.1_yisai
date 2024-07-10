#if !defined(AFX_PIXELPROPDLG_H__D73DCFFC_8CC0_4AD2_92D7_7E9354C31395__INCLUDED_)
#define AFX_PIXELPROPDLG_H__D73DCFFC_8CC0_4AD2_92D7_7E9354C31395__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PixelPropDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPixelPropDlg dialog

class CPixelPropDlg : public CDialog
{
// Construction
public:
	CPixelPropDlg(CWnd* pParent = NULL);   // standard constructor

	void SetCoordinate( int x,int y );
	void SetMsColor( DWORD dwColor,DWORD r_mask,DWORD g_mask,DWORD b_mask );
	void SetPcColor( COLORREF color );

protected:
	DWORD GetColor( DWORD dwColor,DWORD dwMask );

	COLORREF m_Color;
	CBrush m_Brush;

// Dialog Data
	//{{AFX_DATA(CPixelPropDlg)
	enum { IDD = IDD_DIALOG_PIXEL_PROP };
	CString	m_strCoordinate;
	CString	m_strRgb;
	CString	m_strRgb565;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPixelPropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPixelPropDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PIXELPROPDLG_H__D73DCFFC_8CC0_4AD2_92D7_7E9354C31395__INCLUDED_)
