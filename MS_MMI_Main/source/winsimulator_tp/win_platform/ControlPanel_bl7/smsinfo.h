#if !defined(AFX_SMSINFO_H__4EB7EAD2_8032_449B_8857_5893951F9452__INCLUDED_)
#define AFX_SMSINFO_H__4EB7EAD2_8032_449B_8857_5893951F9452__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SMSINFO.h : header file
#include "Resource.h"
#include "mn_type.h"
//

/////////////////////////////////////////////////////////////////////////////
// CSMSINFO dialog
typedef enum
{
	SMS_CAT_8BIT_REF,
	SMS_CAT_16BIT_REF
} SMS_CAT_REF_E;
typedef enum
{
	SMS_PORT_8BIT,
	SMS_PORT_16BIT
} SMS_PORT_REF_E;
typedef struct  {
	BOOLEAN is_port_exist;
	SMS_CAT_REF_E  ref_type;
	SMS_PORT_REF_E	port_type;
	uint32	sour_port;
	uint32  dest_port;
} SMS_USER_HEADER_T;

typedef struct {
	MN_SMS_DCS_T	dcs;
	SMS_USER_HEADER_T	user_header;
	BOOLEAN		is_pid_exist;
	MN_SMS_PID_E	pid;
} SMS_FORMAT_T;
class CSMSINFO : public CDialog
{
// Construction
public:
	SMS_FORMAT_T  m_formatData;
	CSMSINFO(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSMSINFO)
	enum { IDD = IDD_DIALOG_SMSINFO };
	CButton	m_winClassPresent;
	CButton m_winMsgWaitPresent;
	CButton m_winSaveMsg;
	CButton m_winDiscardMsg;
	CButton m_winActiveInd;
	CButton m_winDactiveInd;
	CButton m_winCatRef8;
	CButton m_winCatRef16;
	CButton m_winPort8;
	CButton m_winPort16;
	UINT	m_winSourPort;
	UINT	m_winDestPort;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMSINFO)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSMSINFO)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckPid();
	afx_msg void OnCheckPort();
	afx_msg void OnRadioMsgwait();
	afx_msg void OnRadioClass();
	afx_msg void OnSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSINFO_H__4EB7EAD2_8032_449B_8857_5893951F9452__INCLUDED_)
