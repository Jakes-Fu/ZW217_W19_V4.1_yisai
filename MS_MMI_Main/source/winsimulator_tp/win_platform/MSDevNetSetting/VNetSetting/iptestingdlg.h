#if !defined(AFX_IPTESTINGDLG_H__1CD23231_199A_413A_B25F_CD76B37DBDA5__INCLUDED_)
#define AFX_IPTESTINGDLG_H__1CD23231_199A_413A_B25F_CD76B37DBDA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IpTestingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIpTestingDlg dialog input parameter

typedef struct IPTESTING_INPUT_PARAM_TAG
{
    DWORD dwTestIp;
    DWORD dwLocalIp;
    DWORD dwGateway;
    BYTE  szMacAddr[ETHER_MAC_LEN];
    char  szDeviceName[DEVICE_NAME_LEN];
} IPTESTING_INPUT_PARAM_T;


/////////////////////////////////////////////////////////////////////////////
// CIpTestingDlg dialog

class CIpTestingDlg : public CDialog
{
// Construction
public:
	CIpTestingDlg(
        IPTESTING_INPUT_PARAM_T* pParam = NULL,
        CWnd* pParent = NULL);   // standard constructor

	~CIpTestingDlg();   // standard destructor

// Dialog Data
	//{{AFX_DATA(CIpTestingDlg)
	enum { IDD = IDD_IPTESTING };
	CStatic	m_staticNotes;
	CButton	m_btnOk;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIpTestingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
    BOOL IsTestIpUsable(void);

// Implementation
protected:
    IPTESTING_INPUT_PARAM_T m_input;

// IP test para
protected:
    BOOL m_bIpUsable;
    BOOL m_bInConnTest, m_bInIpTest;

// Implementation
protected:
    void TestStart( void );
    void TestEnd( CString strMsg );
    int StartIpTest( void );
    int StartConnTest( void );

	// Generated message map functions
	//{{AFX_MSG(CIpTestingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    // 自定义消息响应
    afx_msg LONG OnEthernetRecvPacket( WPARAM wParam, LPARAM lParam ); 
    afx_msg LONG OnEthernetRecvTimeout( WPARAM wParam, LPARAM lParam );    
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPTESTINGDLG_H__1CD23231_199A_413A_B25F_CD76B37DBDA5__INCLUDED_)
