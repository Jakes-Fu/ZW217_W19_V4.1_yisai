#if !defined(AFX_DLGALCSET_H__6B7A1009_7864_4B5B_BC73_0A28ED5DAB9E__INCLUDED_)
#define AFX_DLGALCSET_H__6B7A1009_7864_4B5B_BC73_0A28ED5DAB9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAlcSet.h : header file
//
#include "typedef.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgAlcSet dialog

class CDlgAlcSet : public CDialog
{
// Construction
public:
	CDlgAlcSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAlcSet)
	enum { IDD = IDD_DIALOG_ALC_SET };
		// NOTE: the ClassWizard will add data members here
	int m_nCBAlcSw;
	int m_nHold;
	int m_nRise;
	int m_nFall;
	int m_nLimit;
	int m_nThreshold;
	int m_nRatio;
	int m_nCgVar;
	int m_nReleaseRate;
	int m_nAttackRate;
	int m_nReleaseRateEx;
	int m_nAttackRateEx;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAlcSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetValue(VOLUME_STRUCT& volume_data);
	void FetchValue(VOLUME_STRUCT& volume_data);
	void SetEQValue(EQ_STRUCT_G_T& eq_data);
	void FetchEQValue(EQ_STRUCT_G_T& eq_data);
	void SetSerialPara(BOOL bSerialPara){m_bSerialPara = bSerialPara;}

private:
	void GetValue(VOLUME_STRUCT& volume_data);
	void InitValue(VOLUME_STRUCT& volume_data);
	void GetEQValue(EQ_STRUCT_G_T& eq_data);
	void InitEQValue(EQ_STRUCT_G_T& eq_data);

private:
	VOLUME_STRUCT m_VolumeData;
	EQ_STRUCT_G_T m_eq_data;
	BOOL m_bSerialPara;

private:
	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAlcSet)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeComboAlcSw();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGALCSET_H__6B7A1009_7864_4B5B_BC73_0A28ED5DAB9E__INCLUDED_)
