#if !defined(AFX_SETCOMDLG_H__BE267B8B_1315_4897_B51C_BE1DC6D34AB1__INCLUDED_)
#define AFX_SETCOMDLG_H__BE267B8B_1315_4897_B51C_BE1DC6D34AB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetComDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetComDlg dialog

class CSetComDlg : public CDialog
{
// Construction
public:
	CSetComDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetComDlg)
	enum { IDD = IDD_SETCOM_DLG };
	CIPAddressCtrl	m_ctrlIpAddr;
	CEdit		m_editLogName;
	CEdit		m_edtPath;
	CComboBox	m_cmbPort;
	CComboBox	m_cmbBaudRate;
	CString		m_strRate;
	CString		m_strFrameHead;
	CString		m_strPort;
	CString		m_strLogPath;
	CString		m_strLogName;
	int			m_nMaxNum;
	int			m_nTPNum;
	int		m_nSocketPort;
	CButton     m_btnGenerateTPTimeByTool;
	CButton		m_btnGenerateOrgFile;
	CButton		m_btnGenerateTPTimeByD0A0;
	CButton		m_btnGenerateTPTimeByPhone;
	int		m_nLogType;
	BOOL	m_bNotFilterAddr;
	//}}AFX_DATA
	BOOL		m_bSaveRecent;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetComDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
    int		  m_nPortIndex;
    int		  m_nBaudIndex;

    int  m_nChannelType;
    DWORD m_dwIpAddr;
	
	BOOL m_bAutotest;
	BOOL m_bAutoConnect;
	BOOL m_bLogSN;
	
	CString  m_strIpAddr;

    // TRUE:  Just to show the settings
    // FALSE: Allow user to change settings
    BOOL m_bReadOnly;

	//teana hu 2009.05.15
	DWORD  m_dwGenerateTPTime;
	BOOL  m_bGenerateOrgFile;

	//polo.jiang 2013.01.06
	TCHAR	m_szCfgFile[MAX_PATH];
	DWORD	m_dwMemFileSize;

	// Generated message map functions
	//{{AFX_MSG(CSetComDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonBrowse();
	afx_msg void OnSelchangeComboPort();
	virtual void OnOK();
	afx_msg void OnRadioSaveAll();
	afx_msg void OnRadioSaveRecent();
	afx_msg void OnSetfocusEditLogname();
	afx_msg void OnKillfocusEditLogname();
	afx_msg void OnRadioUart();
	afx_msg void OnRadioSocket();
	afx_msg void OnSelchangeComboBaudrate();
	afx_msg void OnEditchangeComboBaudrate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
    typedef int (CSetComDlg::* LPCALLBACKFUNC)(int nPort,DWORD dwUserArg); 
    void	EnumSerialPorts(LPCALLBACKFUNC,DWORD);

private: 
	int		GetPort(int nPort,DWORD dwUserArg);
	BOOL    CheckLogName();

	void    UpdateSaveType(BOOL bSaveRecent);
	BOOL	WritePrivateProfileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpFormat,	int nValue, LPCTSTR lpFileName);
	void	EnableSocketSetting(BOOL bEnable=TRUE);

public:
	BOOL	CheckLogPath();
	//Get the port number
	DWORD	GetPortNum();

	//Get the baud rate
	DWORD	GetBaudRate();

    // Save serial port settings
    void	SaveSettings();
    
    // Load serial port settings
    void	LoadSettings();
	
	//get log file name
	CString GetLogName();

	//get frame head
	DWORD GetFrameHead();

    void GetChannelArgs( LPDWORD lpArg );
	
	int m_nEndian;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETCOMDLG_H__BE267B8B_1315_4897_B51C_BE1DC6D34AB1__INCLUDED_)
