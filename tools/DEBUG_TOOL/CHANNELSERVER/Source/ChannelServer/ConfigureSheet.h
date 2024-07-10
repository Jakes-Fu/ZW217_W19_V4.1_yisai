#if !defined(AFX_CONFIGURESHEET_H__46B6838F_33CA_4920_B15F_EE77AAE7F50E__INCLUDED_)
#define AFX_CONFIGURESHEET_H__46B6838F_33CA_4920_B15F_EE77AAE7F50E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigureSheet.h : header file
//
#include "DlgUartConfig.h"
#include "DlgSocketConfig.h"
#include "DlgFileConfig.h"

/////////////////////////////////////////////////////////////////////////////
// CConfigureSheet dialog

class CConfigureSheet : public CPropertyPage
{
	DECLARE_DYNCREATE(CConfigureSheet)

// Construction
public:
	CConfigureSheet();
	~CConfigureSheet();

//attribute
public:
	BOOL SaveConfiguration();
	void GetConfiguration();
	void GetUartInfo();

	BOOL IsRightPort(DWORD nPort,LPCTSTR lpszPortName,BOOL bDiag = TRUE);
	BOOL IsDiagPort(DWORD nPort);
	BOOL IsLogPort(DWORD nPort);
	DWORD GetEnumPortDelay();
	int  GetEndian();

	void GetUartParam(DWORD &nDiagPort,DWORD &nDiagBaudRate,DWORD &nLogPort,DWORD &nLogBaudRate);
	void GetServerParam(DWORD &nPort,DWORD &nIP);
	void GetSocketParam(DWORD &nPort,DWORD &nIP, int &nFlowType);
	BOOL GetFileParam(int &nFlowType,CString &strFile);
	BOOL IsNeedPortReset();
	void CreateCtrls();
	void  DeleteMergedFile();

// Dialog Data
	//{{AFX_DATA(CConfigureSheet)
	enum { IDD = IDD_LOG_CONFIGURE };
	CComboBox	m_cmbChannelType;
	UINT		m_nTmpServerPort;
	UINT		m_nTmpTimeouts;
	//}}AFX_DATA

	//for ChannelServer 
	UINT  m_nServerPort;
	UINT  m_nServerIP;
	BOOL  m_bAutoTest; 	
	UINT  m_nTimeouts;
	int   m_nChannelType;
	int   m_nUpDataSpeedAlert;
	BOOL  m_bUILogEnable;

	BOOL  m_bEnableUartReconnect;
	DWORD m_dwReconnectGap;

	CDlgUartConfig m_dlgUartConfig;
	CDlgSocketConfig m_dlgSocketConfig;
	CDlgFileConfig m_dlgFileConfig;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConfigureSheet)
	public:
	virtual void OnOK();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:	
	// Generated message map functions
	//{{AFX_MSG(CConfigureSheet)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeChannelType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
   
    BOOL CheckData();
    BOOL CheckServerPort();
    BOOL CheckTimeout();

	void LoadSocketConfig(LPCTSTR pszCfgFile);
	void LoadUartConfig(LPCTSTR pszCfgFile);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGURESHEET_H__46B6838F_33CA_4920_B15F_EE77AAE7F50E__INCLUDED_)
