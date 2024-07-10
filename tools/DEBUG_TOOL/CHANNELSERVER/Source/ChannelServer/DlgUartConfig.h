#if !defined(AFX_DLGUARTCONFIG_H__7A396C7E_F0AE_42BE_A08E_6EAB4CADDEB4__INCLUDED_)
#define AFX_DLGUARTCONFIG_H__7A396C7E_F0AE_42BE_A08E_6EAB4CADDEB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUartConfig.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDlgUartConfig dialog
#include "IDevHound.h"

#pragma warning( push, 3 )
#include <vector>
#pragma warning( pop )

class CDlgUartConfig : public CDialog
{
// Construction
public:
	CDlgUartConfig(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgUartConfig)
	enum { IDD = IDD_UART_CONFIG };
	BOOL	m_bTmpNameMatch;
	BOOL	m_bTmpPortReset;
	UINT	m_nTmpLogBaudrate;
	UINT	m_nTmpDiagBaudrate;
	//}}AFX_DATA

	CString m_strDiagPortName;
	CString m_strLogPortName;
	UINT    m_nDiagPort;
	UINT    m_nLogPort;
	UINT    m_nDiagBaudrate;
	UINT    m_nLogBaudrate;
	BOOL	m_bNameMatch;
	BOOL	m_bPortReset;
	UINT    m_nEnumPortDelay;
	int  	m_nEndian;  

	std::vector<DEV_INFO> m_vPorts;

	CStringArray m_agDiagPortNames;
	CStringArray m_agLogPortNames;

	void LoadConfig(LPCTSTR pszCfgFile);
	void SaveConfig(LPCTSTR pszCfgFile);
	void LoadPortFilters(LPCTSTR lpszConfig);
	BOOL CheckConfig();
	void UpdateConfig();
	
	BOOL RecreatePortCtrl(int nID,  BOOL bList );
	BOOL IsRightPort(DWORD nPort,LPCTSTR lpszPortName,BOOL bDiag = TRUE);
	void GetUartInfo();
	void UpdateControls(BOOL bDiag);
	void UpdatePorts(BOOL bDiag);

	void ChangeDialogFont(CFont *pFont);	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUartConfig)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:	
	void _GetPortInfo(CComboBox *pCmb,	UINT &nPort,CString  &strName);
	// Generated message map functions
	//{{AFX_MSG(CDlgUartConfig)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUARTCONFIG_H__7A396C7E_F0AE_42BE_A08E_6EAB4CADDEB4__INCLUDED_)
