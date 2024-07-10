#if !defined(AFX_NETSTATEDLG_H__18C05F05_A5F1_4023_88B1_97865DEF0ABF__INCLUDED_)
#define AFX_NETSTATEDLG_H__18C05F05_A5F1_4023_88B1_97865DEF0ABF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetstateDlg.h : header file
//

#include "msdev_param.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CNetstateDlg dialog

class CNetstateDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CNetstateDlg)

// Construction
public:
	CNetstateDlg();
	~CNetstateDlg();

// Dialog Data
	//{{AFX_DATA(CNetstateDlg)
	enum { IDD = IDD_NETSTATE_PANAL };
	CEdit	m_editDns2;
	CEdit	m_editDns1;
	CButton	m_chkSend;
	CButton	m_chkRecv;
	CEdit	m_editCfgFile;
	CEdit	m_editNetid;
	CEdit	m_editSnmask;
	CEdit	m_editDevDesc;
	CEdit	m_editDevName;
	CEdit	m_editDevState;
	CEdit	m_editGateway;
	CEdit	m_editIp;
	CEdit	m_editMac;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CNetstateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// CButtonST style operations
protected:
    void      InitialSTStyleButton( void );
    CButtonST m_btnDrvInstall;
    BOOL      m_bDrvInstall;

private:
    COLORREF  m_colorMac, m_colorIp, m_colorSnmask, m_colorGateway, m_colorDns1, m_colorDns2;
    COLORREF  m_colorCfgFile, m_colorNetid, m_colorNetState;
    BOOL IsIpNormal( DWORD dwIp );
    BOOL IsMacMormal( const uint8* pszMac, int iLen );
    BOOL IsSnmaskNormal( DWORD dwSnmask, DWORD dwIp );
    void SaveSendEnableFlag( BOOL bEnable );
    void SaveRecvEnableFlag( BOOL bEnable );

private:
    MSDEV_NETPARAM_T  m_netParam;

// Implementation
protected:
    void ParamRefresh( const MSDEV_NETPARAM_T* pParam );
    void GetAndUpdateNetState( void );

protected:
	// Generated message map functions
	//{{AFX_MSG(CNetstateDlg)
	afx_msg void OnBtnSetting();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnDrvInstall();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCheckSend();
	afx_msg void OnCheckRecv();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETSTATEDLG_H__18C05F05_A5F1_4023_88B1_97865DEF0ABF__INCLUDED_)
