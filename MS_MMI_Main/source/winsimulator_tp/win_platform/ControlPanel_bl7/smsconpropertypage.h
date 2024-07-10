#if !defined(AFX_SMSCONPROPERTYPAGE_H__55F876CA_8C0E_4182_849A_57974153309E__INCLUDED_)
#define AFX_SMSCONPROPERTYPAGE_H__55F876CA_8C0E_4182_849A_57974153309E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SmsConPropertyPage.h : header file
//
#include "stdafx.h"
#include "resource.h"
#include "ConPanLayer.h"
#include "SMSINFO.h"
/////////////////////////////////////////////////////////////////////////////
// CSmsConPropertyPage dialog


class CSmsConPropertyPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSmsConPropertyPage)

// Construction
public:
	CSmsConPropertyPage();
	~CSmsConPropertyPage();
public:
	int  sys_id;
	void DealCPMsg(void *pmsg);

// Dialog Data
	//{{AFX_DATA(CSmsConPropertyPage)
	enum { IDD = IDD_DIALOG_SMSCON };
	CButton	m_wndSMSDataType;
	CButton m_wndSMSBinary;
	CButton	m_wndSMSScStatus;
	CButton	m_wndSmsRevBySME;
	CButton	m_wndSmsNotConfirm;
	CButton	m_wndSmsReplaceBySC;
	CButton	m_wndSmsSendLater;
	CButton	m_wndSmsNotSendAgain;
	CButton	m_wndSmsInvalidSR;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSmsConPropertyPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSmsConPropertyPage)
	afx_msg void OnButtonSend();
	afx_msg void OnButtonSendStatusReport();
	virtual BOOL OnInitDialog();
	afx_msg void OnMaxtextEditSmssend();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBUTTONFormatSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
    
	void PlaySendSMSSound();
    void PlayRecvSMSSound();

    void StartSmsStatusReport();
    void StopSmsStatusReport();

	CSMSINFO	sms_info;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSCONPROPERTYPAGE_H__55F876CA_8C0E_4182_849A_57974153309E__INCLUDED_)
