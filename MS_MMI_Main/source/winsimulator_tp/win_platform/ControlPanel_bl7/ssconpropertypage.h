#if !defined(AFX_SSCONPROPERTYPAGE_H__B837DB37_9312_45A7_B80E_4E8F897A8D8D__INCLUDED_)
#define AFX_SSCONPROPERTYPAGE_H__B837DB37_9312_45A7_B80E_4E8F897A8D8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SSConPropertyPage.h : header file
//
#include "stdafx.h"
#include "resource.h"
#include "CIniFile.h"

/////////////////////////////////////////////////////////////////////////////
// CSSConPropertyPage dialog

class CSSConPropertyPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSSConPropertyPage)

// Construction
public:
	CSSConPropertyPage();
	~CSSConPropertyPage();

public:
	int  sys_id;
	
   	void DealCPMsg(void *pmsg);
    void DealMsgReg(void *pmsg);
    void DealMsgUnReg(void *pmsg);
    void DealMsgActivate(void *pmsg);
    void DealMsgDeactivate(void *pmsg);
    void DealMsgInterogate(void *pmsg);
    void DealMsgModifyPwd(void *pmsg);
    void DealMsgPwdRsp(void *pmsg);
	void DealMsgProcUSSD(void *pmsg);
	void DealMsgUssdRsp(void *pmsg);
public:
// Dialog Data
	//{{AFX_DATA(CSSConPropertyPage)
	enum { IDD = IDD_DIALOG_SSCON };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSSConPropertyPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSSConPropertyPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
    
	typedef enum{
        SS_UNKNOWN = -1,
        SS_ACTIVATE,
        SS_DEACTIVATE,
        SS_MODIFYPWD_CHECK_OLDPWD,
        SS_MODIFYPWD_CHECK_NEWPWD_FIRSTLY,
        SS_MODIFYPWD_CHECK_NEWPWD_SECONDLY,
    } ACTION_AFTER_GET_PASSWORD_E;

    ACTION_AFTER_GET_PASSWORD_E m_apActAfterGetPwd;
    void    *m_pSigKeptForAct; //kept for action after get password.
    CString m_csNewPwd;
    
    CString  m_csSSIniFile;
    CIniFile m_ifSSIni;

    BOOL InitializeSSIniFile(CString csFile);
    BOOL InitializeAllControls();

    void SetActAfterGetPwd(int nActAfterGetPwd, void *pSig = NULL);
    BOOL GetIDCFromMNSSType(int nMNSSType, UINT &nIDC);
    BOOL FlushIniByIDC(UINT nIDC, CString &csText);

    BOOL SetItemServiceStatus(int nMNSSType, int nSvrState);    
    BOOL GetItemServiceStatus(int nMNSSType, int &nSvrState); 
    BOOL SetIDCText(UINT nIDC, CString csText);
    BOOL GetIDCText(UINT nIDC, CString &csText);

   


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSCONPROPERTYPAGE_H__B837DB37_9312_45A7_B80E_4E8F897A8D8D__INCLUDED_)
