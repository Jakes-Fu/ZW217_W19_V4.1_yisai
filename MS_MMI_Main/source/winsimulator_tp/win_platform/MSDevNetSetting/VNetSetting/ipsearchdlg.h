#if !defined(AFX_IPSEARCHDLG_H__78EA188B_A01F_48A3_A6A5_E2E12AAFF3B3__INCLUDED_)
#define AFX_IPSEARCHDLG_H__78EA188B_A01F_48A3_A6A5_E2E12AAFF3B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IpSearchDlg.h : header file
//
#include "common.h"


/////////////////////////////////////////////////////////////////////////////
// CIpSearchDlg dialog input parameter

typedef struct IPSEARCH_INPUT_PARAM_TAG
{
    DWORD dwLocalIp;
    DWORD dwSnmask;
    DWORD dwGateway;
    BYTE  szMacAddr[ETHER_MAC_LEN];
    char  szDeviceName[DEVICE_NAME_LEN];
} IPSEARCH_INPUT_PARAM_T;


/////////////////////////////////////////////////////////////////////////////
// CIpSearchDlg dialog

class CIpSearchDlg : public CDialog
{
// Construction
public:
	CIpSearchDlg(
        IPSEARCH_INPUT_PARAM_T* pInputParam = NULL, 
        CWnd* pParent = NULL);   // standard constructor

    ~CIpSearchDlg();   // standard destructor

// Dialog Data
	//{{AFX_DATA(CIpSearchDlg)
	enum { IDD = IDD_IPSEARCHING };
	CButton	m_btnNext;
	CComboBox	m_comboResult;
	CStatic	m_staticNotes;
	CButton	m_radioForw;
	CButton	m_radioBack;
	CIPAddressCtrl	m_ipEnd;
	CIPAddressCtrl	m_ipBegin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIpSearchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
    DWORD GetAvailIp( void );

// Auto IP search parameters
protected:
    IPSEARCH_INPUT_PARAM_T m_input;
    DWORD m_dwIpMin, m_dwIpMax, m_dwIpAvail;
    BOOL  m_bSearchForward;

protected:
    DWORD m_dwTestIp;
    BOOL  m_bSearching, m_bInConnTest, m_bInIpTest;

// Implementation
protected:
    BOOL RangeCheck( void );
    void AddStringToResultList( CString str );

// searching implementation
protected:
    BOOL GetNextTestIp( DWORD* pTestIp );
    void TestStart( void );
    void TestEnd( CString strMsg, BOOL bEnd );
    int StartIpTest( DWORD dwIpTest );
    int StartConnTest( void );

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CIpSearchDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioForward();
	afx_msg void OnRadioBackward();
	afx_msg void OnBtnNext();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
    // 自定义消息响应
    afx_msg LONG OnEthernetRecvPacket( WPARAM wParam, LPARAM lParam ); 
    afx_msg LONG OnEthernetRecvTimeout( WPARAM wParam, LPARAM lParam );   
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPSEARCHDLG_H__78EA188B_A01F_48A3_A6A5_E2E12AAFF3B3__INCLUDED_)
