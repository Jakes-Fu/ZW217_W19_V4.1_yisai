#if !defined(AFX_GSENSORPROPERYPAGE_H__58293BF3_9DA2_4ABD_9068_1B5059D465E2__INCLUDED_)
#define AFX_GSENSORPROPERYPAGE_H__58293BF3_9DA2_4ABD_9068_1B5059D465E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GSensorProperyPage.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CGSensorProperyPage dialog

class CGSensorProperyPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CGSensorProperyPage)

// Construction
public:
	CGSensorProperyPage();
	~CGSensorProperyPage();

// Dialog Data
	//{{AFX_DATA(CGSensorProperyPage)
	enum { IDD = IDD_DIALOG_GSENSOR };
	CComboBox	m_cmbGSStatus;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGSensorProperyPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGSensorProperyPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmbGsensor();
	//}}AFX_MSG
	afx_msg void OnTestDM();
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GSENSORPROPERYPAGE_H__58293BF3_9DA2_4ABD_9068_1B5059D465E2__INCLUDED_)
