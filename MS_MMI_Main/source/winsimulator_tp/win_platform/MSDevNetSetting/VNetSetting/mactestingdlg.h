#if !defined(AFX_MACTESTINGDLG_H__9E794953_FC2F_4942_9F07_33F4D41658CF__INCLUDED_)
#define AFX_MACTESTINGDLG_H__9E794953_FC2F_4942_9F07_33F4D41658CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MacTestingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMacTestingDlg dialog input parameter

typedef struct MACTESTING_INPUT_PARAM_TAG
{
    DWORD dwLocalIp;
    DWORD dwSnmask;
    DWORD dwGateway;
    DWORD dwAvailIp;
    BYTE  szMacHost[ETHER_MAC_LEN];
    BYTE  szMacTest[ETHER_MAC_LEN];
    char  szDeviceName[DEVICE_NAME_LEN];
} MACTESTING_INPUT_PARAM_T;


/////////////////////////////////////////////////////////////////////////////
// CMacTestingDlg dialog

class CMacTestingDlg : public CDialog
{
// Construction
public:
	CMacTestingDlg(
        MACTESTING_INPUT_PARAM_T* pParam = NULL,
        CWnd* pParent = NULL);   // standard constructor

    ~CMacTestingDlg();  // standard destructor


// Dialog Data
	//{{AFX_DATA(CMacTestingDlg)
	enum { IDD = IDD_MACTESTING };
	CStatic	m_staticNotes;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMacTestingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
    BOOL IsTestMacUsable(void);

// Implementation
protected:
    MACTESTING_INPUT_PARAM_T m_input;

// MAC test para
protected:
    BOOL m_bMacUsable;
    BOOL m_bInConnTest, m_bInMacTest, m_bInPingTest, m_bInIpTest;
    DWORD m_nIpCounter, m_dwAvailIp;

// Implementation
protected:
    void TestStart( void );
    void TestEnd( CString strMsg );
    int StartMacTest( void );
    int StartConnTest( void );
    int StartPingTest( void );
    int StartIpTest( void );

	// Generated message map functions
	//{{AFX_MSG(CMacTestingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
    // 自定义消息响应
    afx_msg LONG OnEthernetRecvPacket( WPARAM wParam, LPARAM lParam ); 
    afx_msg LONG OnEthernetRecvTimeout( WPARAM wParam, LPARAM lParam ); 
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACTESTINGDLG_H__9E794953_FC2F_4942_9F07_33F4D41658CF__INCLUDED_)
