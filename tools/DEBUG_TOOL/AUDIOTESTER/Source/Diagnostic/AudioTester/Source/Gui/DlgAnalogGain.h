#if !defined(AFX_DLGANALOGGAIN_H__FFA0D25C_8079_4C8B_AA0C_DE55644D8CBF__INCLUDED_)
#define AFX_DLGANALOGGAIN_H__FFA0D25C_8079_4C8B_AA0C_DE55644D8CBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAnalogGain.h : header file
//
#include "typedef.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgAnalogGain dialog

class CDlgAnalogGain : public CDialog
{
// Construction
public:
	CDlgAnalogGain(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAnalogGain)
	enum { IDD = IDD_DIALOG_ANALOG_GAIN };
		// NOTE: the ClassWizard will add data members here
	int m_nCBIndexGIM;
	int m_nCBIndexGain;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAnalogGain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetValue(VOLUME_STRUCT& volume_data);
	void FetchValue(VOLUME_STRUCT& volume_data);
		
private:
	void InitValue(VOLUME_STRUCT& volume_data);
	void GetValue(VOLUME_STRUCT& volume_data);
		
private:
	VOLUME_STRUCT m_VolumeData;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAnalogGain)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGANALOGGAIN_H__FFA0D25C_8079_4C8B_AA0C_DE55644D8CBF__INCLUDED_)
