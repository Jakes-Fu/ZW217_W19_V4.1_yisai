#if !defined(AFX_DLGRECORDDPINPUTGAIN_H__E3421C62_9156_48F9_A4AA_F8133D8FFBAD__INCLUDED_)
#define AFX_DLGRECORDDPINPUTGAIN_H__E3421C62_9156_48F9_A4AA_F8133D8FFBAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRecordDpInputGain.h : header file
//
#include "typedef.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgRecordDpInputGain dialog

class CDlgRecordDpInputGain : public CDialog
{
// Construction
public:
	CDlgRecordDpInputGain(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRecordDpInputGain)
	enum { IDD = IDD_DIALOG_RECORD_DP_INPUT_GAIN };
		// NOTE: the ClassWizard will add data members here
	int m_nDPInputGain;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRecordDpInputGain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	void SetValue(EQ_STRUCT_G_T& eq_data);
	void FetchValue(EQ_STRUCT_G_T& eq_data);

private:
	void InitValue(EQ_STRUCT_G_T& eq_data);
	void GetValue(EQ_STRUCT_G_T& eq_data);

private:
	EQ_STRUCT_G_T m_eq_data;
	int m_nDPInputGainBak;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRecordDpInputGain)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRECORDDPINPUTGAIN_H__E3421C62_9156_48F9_A4AA_F8133D8FFBAD__INCLUDED_)
