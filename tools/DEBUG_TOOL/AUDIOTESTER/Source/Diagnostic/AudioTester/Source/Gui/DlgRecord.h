#if !defined(AFX_DLGRECORD_H__CE6D0670_34CF_4CDF_838A_99A804C3EAB3__INCLUDED_)
#define AFX_DLGRECORD_H__CE6D0670_34CF_4CDF_838A_99A804C3EAB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRecord.h : header file
//
#include "typedef.h"
#include "AudioTesterSheet.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgRecord dialog

class CDlgRecord : public CDialog
{
// Construction
public:
	CDlgRecord(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRecord)
	enum { IDD = IDD_DIALOG_RECORD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRecord)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL
public:
	void SetValue(EQ_STRUCT_G_T& eq_data);
	void FetchValue(EQ_STRUCT_G_T& eq_data);
	void SetArmValue( VOLUME_STRUCT& volume_data );
	void FetchArmValue( VOLUME_STRUCT& volume_data );
	void SetIndex(int nEQIndex, int nPageIndex){m_nEQIndex = nEQIndex; m_nPageIndex = nPageIndex;}
	void SetControlFlag(BOOL bDspCodecInfo, BOOL bSmartPhone){m_bDspCodecInfo = bDspCodecInfo; m_bSmartPhone = bSmartPhone;}
	
	//teana hu 2012.06.14
	void FetchValueEx(EQ_STRUCT_G_T& eq_data);
	//

private:
	void DrawTransparentBitmap(CDC *pDC, UINT IDImage, CRect &rect, COLORREF rgbMask);
	void InitToolbar();
		
private:
	EQ_STRUCT_G_T m_eq_data;
	VOLUME_STRUCT m_VolumeData;
		
	CToolBar m_toolBarDP;
	int m_nEQIndex;
	int m_nPageIndex;
	BOOL m_bDspCodecInfo;
	BOOL m_bSmartPhone;
		
public:
	CAudioTesterSheet *m_pSheet;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRecord)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonDpLoadCurrentModeFromFlash();
	afx_msg void OnButtonDpLoadCurrentModeFromRam();
	afx_msg void OnButtonDpWriteCurrentModeToFlash();
	afx_msg void OnButtonDpWriteCurrentModeToRam();
	afx_msg void OnButtonNormalRecord();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRECORD_H__CE6D0670_34CF_4CDF_838A_99A804C3EAB3__INCLUDED_)
