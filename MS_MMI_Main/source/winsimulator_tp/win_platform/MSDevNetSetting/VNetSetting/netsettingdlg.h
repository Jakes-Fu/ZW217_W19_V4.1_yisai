#if !defined(AFX_NETSETTINGDLG_H__E54CA1CE_E8E0_4FDE_A8EA_6E4EBDF56574__INCLUDED_)
#define AFX_NETSETTINGDLG_H__E54CA1CE_E8E0_4FDE_A8EA_6E4EBDF56574__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetsettingDlg.h : header file
//

#include "iphlpapi.h"
#include "HexEdit.h"
#include "msdev_param.h"

/////////////////////////////////////////////////////////////////////////////
// CNetsettingDlg dialog

class CNetsettingDlg : public CDialog
{
// Construction
public:
	CNetsettingDlg(
        MSDEV_NETPARAM_T* pParam = NULL, 
        CWnd* pParent = NULL);   // standard constructor
    ~CNetsettingDlg();

// Dialog Data
	//{{AFX_DATA(CNetsettingDlg)
	enum { IDD = IDD_NETSETTING_PANAL };
	CButton	m_chkIpSearch;
	CEdit	m_editSnmask;
	CEdit	m_editLmac;
	CEdit	m_editLip;
	CEdit	m_editGateway;
	CEdit	m_editDhcp;
	CEdit	m_editDevDesc;
	CComboBox	m_comboAdapt;
	CComboBox	m_comboDns2;
	CComboBox	m_comboDns1;
	CButton	m_chkDhcp;
	CIPAddressCtrl	m_ipAddr;
	CHexEdit	m_editMac6;
	CHexEdit	m_editMac5;
	CHexEdit	m_editMac4;
	CHexEdit	m_editMac3;
	CHexEdit	m_editMac2;
	CHexEdit	m_editMac1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetsettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
    BOOL GetParam( MSDEV_NETPARAM_T* pParam );

// CButtonST style operations
protected:
    void      InitialSTStyleButton( void );
    CButtonST m_btnIpTest;
    CButtonST m_btnMacTest;

private:
    MSDEV_NETPARAM_T m_netParam;
    PIP_ADAPTER_INFO m_pCurrentAdapt;

private:
    // to test parameter
    DWORD m_dwIpToTest;
    BYTE m_szMacToTest[ETHER_MAC_LEN];
    // testing result record
    DWORD m_dwAvailIp;
    BYTE m_szAvailMac[ETHER_MAC_LEN];
    //BOOL m_bIpAvail, m_bMacAvail;
    PIP_ADAPTER_INFO m_pipAvailIp, m_pipAvailMac;

// Implementation
protected:
    void InitSimulatorNetInfo( void );
    void InitSystemNetInfo( void );
    void RefreshDhcpControls( BOOL bUseDhcp );
    void ShowTestIp( BOOL bShowIp );
    void RefreshAdaptNetInfo( PIP_ADAPTER_INFO pAdapt );
    BOOL IsTestIpValid( void );
    BOOL IsTestMacValid( void );

    BOOL IsIpTested( void );
    BOOL IsMacTested( void );

    BOOL SaveParamToFile( void );
    BOOL WriteStringToFile( CStdioFile* pFile );
    void SaveParaToSystem( void );

	// Generated message map functions
	//{{AFX_MSG(CNetsettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChkDhcp();
	afx_msg void OnSelchangeComboAdapter();
	afx_msg void OnBtnMactest();
	afx_msg void OnBtnIptest();
	afx_msg void OnChkIpsearch();
	afx_msg void OnKillfocusEditMac1();
	afx_msg void OnKillfocusEditMac2();
	afx_msg void OnKillfocusEditMac3();
	afx_msg void OnKillfocusEditMac4();
	afx_msg void OnKillfocusEditMac5();
	afx_msg void OnKillfocusEditMac6();
	virtual void OnOK();
	afx_msg void OnFieldchangedIpaddressSimu(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETSETTINGDLG_H__E54CA1CE_E8E0_4FDE_A8EA_6E4EBDF56574__INCLUDED_)
