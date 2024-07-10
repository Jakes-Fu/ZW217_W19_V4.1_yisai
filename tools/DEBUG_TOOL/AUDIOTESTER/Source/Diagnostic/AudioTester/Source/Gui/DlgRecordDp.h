#if !defined(AFX_DLGRECORDDP_H__43229C3B_0495_414C_BA39_8EE879DD0023__INCLUDED_)
#define AFX_DLGRECORDDP_H__43229C3B_0495_414C_BA39_8EE879DD0023__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRecordDp.h : header file
//
#include "typedef.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgRecordDp dialog

class CDlgRecordDp : public CDialog
{
// Construction
public:
	CDlgRecordDp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRecordDp)
	enum { IDD = IDD_DIALOG_RECORD_DP };
	CStatic m_stcArrow1;
	CStatic m_stcArrow2;
	CStatic m_stcArrow3;
	CStatic m_stcArrow4;
	CStatic m_stcArrow5;
	CStatic m_stcArrow6;
	CStatic m_stcArrow7;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRecordDp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	void SetValue(EQ_STRUCT_G_T& eq_data);
	void FetchValue(EQ_STRUCT_G_T& eq_data);
	void SetArmValue( VOLUME_STRUCT& volume_data );
	void FetchArmValue( VOLUME_STRUCT& volume_data );
	void SetControlFlag(BOOL bDspCodecInfo, BOOL bSmartPhone);
		
private:
	void DrawTransparentBitmap(CDC *pDC, UINT IDImage, CRect &rect, COLORREF rgbMask);

private:
	EQ_STRUCT_G_T m_eq_data;
	VOLUME_STRUCT m_VolumeData;
	BOOL m_bDspCodecInfo;
	BOOL m_bSmartPhone;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRecordDp)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	afx_msg void OnBtnDpInputGain();
	afx_msg void OnBtnDpAlc();
	afx_msg void OnBtnDpLcf();
	afx_msg void OnBtnDpEq();
	afx_msg void OnBtnAnalogGain();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonDpLoadCurrentModeFromFlash();
	afx_msg void OnButtonDpLoadCurrentModeFromRam();
	afx_msg void OnButtonDpWriteCurrentModeToFlash();
	afx_msg void OnButtonDpWriteCurrentModeToRam();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRECORDDP_H__43229C3B_0495_414C_BA39_8EE879DD0023__INCLUDED_)
