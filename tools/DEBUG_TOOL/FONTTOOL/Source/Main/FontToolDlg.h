// FontToolDlg.h : header file
//

#if !defined(AFX_FONTTOOLDLG_H__720D9D0D_B7A3_447B_A8A1_7666B94ED9EB__INCLUDED_)
#define AFX_FONTTOOLDLG_H__720D9D0D_B7A3_447B_A8A1_7666B94ED9EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CoolStatic.h"
#include "EditFontDlg.h"
#include "Global.h"
#include "GlyphData.h"
/////////////////////////////////////////////////////////////////////////////
// CFontToolDlg dialog

class CFontToolDlg : public CDialog
{
// Construction
public:
	CFontToolDlg(CWnd* pParent = NULL);	// standard constructor
	CGlyphData  m_ftParser;
	void FillGlyphGrid(UINT uCode,BOOL bClean = FALSE);
	BOOL m_bFromClick;
	BOOL m_bFromInput;
	BOOL m_bFromFind;
	BOOL m_bFromFontSetCmb;
	DWORD m_dwUCBegin;
	DWORD m_dwLCBegin;
	CEditFontDlg m_EditFontDlg;
// Dialog Data
	//{{AFX_DATA(CFontToolDlg)
	enum { IDD = IDD_FONTTOOL_DIALOG };
	CComboBox	m_cmbPtSize;
	CComboBox	m_cmbSortBy;
	CStatic	m_stcWorking;
	CEdit	m_edtChar;
	CProgressCtrl	m_progress;
	CEdit	m_edtCurUCcode;
	CScrollBar	m_scb;
	CCoolStatic    m_csR0C0;
	CCoolStatic    m_csR0C1;
	CCoolStatic    m_csR0C2;
	CCoolStatic    m_csR0C3;
	CCoolStatic    m_csR0C4;
	CCoolStatic    m_csR0C5;
	CCoolStatic    m_csR0C6;
	CCoolStatic    m_csR0C7;
	CCoolStatic    m_csR0C8;
	CCoolStatic    m_csR0C9;
	CCoolStatic    m_csR0C10;
	CCoolStatic    m_csR0C11;
	CCoolStatic    m_csR0C12;
	CCoolStatic    m_csR0C13;
	CCoolStatic    m_csR0C14;
	CCoolStatic    m_csR0C15;
	CCoolStatic    m_csR1C0;
	CCoolStatic    m_csR1C1;
	CCoolStatic    m_csR1C2;
	CCoolStatic    m_csR1C3;
	CCoolStatic    m_csR1C4;
	CCoolStatic    m_csR1C5;
	CCoolStatic    m_csR1C6;
	CCoolStatic    m_csR1C7;
	CCoolStatic    m_csR1C8;
	CCoolStatic    m_csR1C9;
	CCoolStatic    m_csR1C10;
	CCoolStatic    m_csR1C11;
	CCoolStatic    m_csR1C12;
	CCoolStatic    m_csR1C13;
	CCoolStatic    m_csR1C14;
	CCoolStatic    m_csR1C15;
	CCoolStatic    m_csR2C0;
	CCoolStatic    m_csR2C1;
	CCoolStatic    m_csR2C2;
	CCoolStatic    m_csR2C3;
	CCoolStatic    m_csR2C4;
	CCoolStatic    m_csR2C5;
	CCoolStatic    m_csR2C6;
	CCoolStatic    m_csR2C7;
	CCoolStatic    m_csR2C8;
	CCoolStatic    m_csR2C9;
	CCoolStatic    m_csR2C10;
	CCoolStatic    m_csR2C11;
	CCoolStatic    m_csR2C12;
	CCoolStatic    m_csR2C13;
	CCoolStatic    m_csR2C14;
	CCoolStatic    m_csR2C15;
	CCoolStatic    m_csR3C0;
	CCoolStatic    m_csR3C1;
	CCoolStatic    m_csR3C2;
	CCoolStatic    m_csR3C3;
	CCoolStatic    m_csR3C4;
	CCoolStatic    m_csR3C5;
	CCoolStatic    m_csR3C6;
	CCoolStatic    m_csR3C7;
	CCoolStatic    m_csR3C8;
	CCoolStatic    m_csR3C9;
	CCoolStatic    m_csR3C10;
	CCoolStatic    m_csR3C11;
	CCoolStatic    m_csR3C12;
	CCoolStatic    m_csR3C13;
	CCoolStatic    m_csR3C14;
	CCoolStatic    m_csR3C15;
	CCoolStatic    m_csR4C0;
	CCoolStatic    m_csR4C1;
	CCoolStatic    m_csR4C2;
	CCoolStatic    m_csR4C3;
	CCoolStatic    m_csR4C4;
	CCoolStatic    m_csR4C5;
	CCoolStatic    m_csR4C6;
	CCoolStatic    m_csR4C7;
	CCoolStatic    m_csR4C8;
	CCoolStatic    m_csR4C9;
	CCoolStatic    m_csR4C10;
	CCoolStatic    m_csR4C11;
	CCoolStatic    m_csR4C12;
	CCoolStatic    m_csR4C13;
	CCoolStatic    m_csR4C14;
	CCoolStatic    m_csR4C15;
	CCoolStatic    m_csR5C0;
	CCoolStatic    m_csR5C1;
	CCoolStatic    m_csR5C2;
	CCoolStatic    m_csR5C3;
	CCoolStatic    m_csR5C4;
	CCoolStatic    m_csR5C5;
	CCoolStatic    m_csR5C6;
	CCoolStatic    m_csR5C7;
	CCoolStatic    m_csR5C8;
	CCoolStatic    m_csR5C9;
	CCoolStatic    m_csR5C10;
	CCoolStatic    m_csR5C11;
	CCoolStatic    m_csR5C12;
	CCoolStatic    m_csR5C13;
	CCoolStatic    m_csR5C14;
	CCoolStatic    m_csR5C15;
	CCoolStatic	m_stcDisp;
	CString	m_strCurUCcode;
	CString	m_strChar;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFontToolDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
    CHAR_SET_INFO_PTR m_pCSI;

	CUIntArray m_agGlyfID;

	int m_nSortBySel;
	int m_nPtSizeIdx;

	BOOL LoadDmf(LPCTSTR lpszFileName);
	BOOL AddDmf(LPCTSTR lpszFileName);
	void Clear();
	void Fresh();
	void UpdateGlyfInfo(UINT height,UINT width,UINT bitmap_h,UINT bitmap_w,int x,int y);
	void ChangeBtnState();
	// Generated message map functions
	//{{AFX_MSG(CFontToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnLoadBdf();
	afx_msg void OnBtnSaveDotmatrix();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEdtCurUccode();
	afx_msg void OnBtnLoadTtf();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnBtnLoadDmf();
	afx_msg void OnChangeEdtChar();
	afx_msg void OnBtnExit();
	afx_msg void OnBtnSaveBdf();
	afx_msg void OnSelchangeCmbSortBy();
	afx_msg void OnSelchangeCmbFontSize();
	afx_msg void OnBtnAdjustParam();
	afx_msg void OnBtnMinus();
	afx_msg void OnBtnPlus();
	afx_msg void OnReplBdf();
	//}}AFX_MSG
	void OnClickFont(UINT uID);
	void OnProgressMsg(WPARAM wParam, LPARAM lParam);
	void OnLoadSetting(WPARAM wparam,LPARAM lparam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONTTOOLDLG_H__720D9D0D_B7A3_447B_A8A1_7666B94ED9EB__INCLUDED_)
