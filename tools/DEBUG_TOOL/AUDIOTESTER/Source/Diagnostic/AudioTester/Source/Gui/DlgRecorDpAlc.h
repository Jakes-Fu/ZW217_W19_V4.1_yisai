#if !defined(AFX_DLGRECORDPALC_H__DCDDF7A4_0FD5_44E5_8223_ED8D25285BED__INCLUDED_)
#define AFX_DLGRECORDPALC_H__DCDDF7A4_0FD5_44E5_8223_ED8D25285BED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRecorDpAlc.h : header file
//
#include "typedef.h"
#include "ALCStatic.h"
#include "RecordStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgRecorDpAlc dialog

class CDlgRecorDpAlc : public CDialog
{
// Construction
public:
	CDlgRecorDpAlc(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRecorDpAlc)
	enum { IDD = IDD_DIALOG_RECORD_DP_ALC };
		// NOTE: the ClassWizard will add data members here
	int m_nCBDPZcSw;
	int m_nCBDPSw;
	int m_nDPSdelay;
	int m_nDPLimitUp;
	int m_nDPLimitDown;
	int m_nCompressorThreshold;
	int m_nCompressorRatio;
	int m_nCompressorAttack;
	int m_nCompressorHold;
	int m_nCompressorRelease;
	int m_nExpanderThreshold;
	int m_nExpanderRatio;
	int m_nExpanderAttack;
	int m_nExpanderHold;
	int m_nExpanderRelease;
	CALCStatic m_stcGraph1;
	CRecordStatic m_stcGraph2;
	CRecordStatic m_stcGraph3;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRecorDpAlc)
	public:
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	void SetValue(EQ_STRUCT_G_T& eq_data);
	void FetchValue(EQ_STRUCT_G_T& eq_data);
		
private:
	void InitValue(EQ_STRUCT_G_T& eq_data);
	void GetValue(EQ_STRUCT_G_T& eq_data);
	void RefeshGraph();
		
private:
	EQ_STRUCT_G_T m_eq_data;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRecorDpAlc)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeEditDpLimitUp();
	afx_msg void OnChangeEditDpLimitDown();
	afx_msg void OnChangeEditCompressorThreshold();
	afx_msg void OnChangeEditExpanderThreshold();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRECORDPALC_H__DCDDF7A4_0FD5_44E5_8223_ED8D25285BED__INCLUDED_)
