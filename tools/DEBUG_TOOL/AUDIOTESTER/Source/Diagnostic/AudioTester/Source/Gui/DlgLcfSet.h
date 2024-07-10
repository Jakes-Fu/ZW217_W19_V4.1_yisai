#if !defined(AFX_DLGLCFSET_H__E4560291_0F54_409B_B177_A4F76AD59EFF__INCLUDED_)
#define AFX_DLGLCFSET_H__E4560291_0F54_409B_B177_A4F76AD59EFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLcfSet.h : header file
//
#include "typedef.h"
#include "LcfStatic.h"

#define NUM_FILTER_TYPE 2
const CString strFilterType[]=
{
	_T("f1f1"),
	_T("butterworth"),
	_T("chebyshev"),
	_T("bessel"),
	_T("ellipitical")
};
/////////////////////////////////////////////////////////////////////////////
// CDlgLcfSet dialog

class CDlgLcfSet : public CDialog
{
// Construction
public:
	CDlgLcfSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgLcfSet)
	enum { IDD = IDD_DIALOG_LCF_SET };
		// NOTE: the ClassWizard will add data members here
	CLcfStatic m_stcGraph;
	int m_nCBLcfSw;
	int m_nCBFilterType;
	SHORT m_nFp;
	SHORT m_nF1fp;
	SHORT m_nF2fp;
	float m_fF1g0;
	float m_fF1g1;
	float m_fF2g0;
	float m_fF2g1;
	int m_nSamplerate;
	CComboBox m_boxFilterType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLcfSet)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetValue(VOLUME_STRUCT& volume_data);
	void FetchValue(VOLUME_STRUCT& volume_data);
	void SetEQValue(EQ_STRUCT_G_T& eq_data);
	void FetchEQValue(EQ_STRUCT_G_T& eq_data);
	void SetSerialPara(BOOL bSerialPara){m_bSerialPara = bSerialPara;}
	void SetSampleRate(int nSamplerate){m_nSamplerate = nSamplerate;}

private:
	void GetValue(VOLUME_STRUCT& volume_data);
	void InitValue(VOLUME_STRUCT& volume_data);
	void ReflashGraph();
	void Enable(BOOL bEnable);
	void GetEQValue(EQ_STRUCT_G_T& eq_data);
	void InitEQValue(EQ_STRUCT_G_T& eq_data);
	void RefreshControl();

private:
	VOLUME_STRUCT m_VolumeData;
	BOOL m_bInit;
	EQ_STRUCT_G_T m_eq_data;
	BOOL m_bSerialPara;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLcfSet)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeComboFilterType();
	afx_msg void OnBtnLcfReflash();
	afx_msg void OnSelchangeComboLcfSw();
	afx_msg void OnChangeEditF1Fp();
	afx_msg void OnChangeEditF1G0();
	afx_msg void OnChangeEditF1G1();
	afx_msg void OnChangeEditF2Fp();
	afx_msg void OnChangeEditF2G0();
	afx_msg void OnChangeEditF2G1();
	afx_msg void OnChangeEditFp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLCFSET_H__E4560291_0F54_409B_B177_A4F76AD59EFF__INCLUDED_)
