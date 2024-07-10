// NetsettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VNetSetting.h"

#include <WINSOCK2.H>
#include "msdev_param.h"
#include "msnet_param.h"

#include "RegOpt.h"
#include "IpTestingDlg.h"
#include "IpSearchDlg.h"
#include "MacTestingDlg.h"
#include "NetsettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
#define MAC_UNIT_LEN    2
#define FILE_NAME       "NetsettingDlg.cpp"

#define COLOR_BLUE      RGB(0, 0, 192)


/////////////////////////////////////////////////////////////////////////////
// CNetsettingDlg dialog


CNetsettingDlg::CNetsettingDlg(MSDEV_NETPARAM_T* pParam, CWnd* pParent /*=NULL*/)
	: CDialog(CNetsettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetsettingDlg)
	//}}AFX_DATA_INIT

    // network info been given from caller
    if( NULL != pParam )
    {
        memcpy(&m_netParam, pParam, sizeof(MSDEV_NETPARAM_T));
        m_dwIpToTest = m_netParam.uIpAddr;
    }
    else
    {
        memset(&m_netParam, 0, sizeof(MSDEV_NETPARAM_T));
        m_dwIpToTest = IP_NULL;
    }

    // current adapter info
    m_pCurrentAdapt = NULL;

    // to test parameter
    m_dwIpToTest = IP_NULL;
    memset(m_szMacToTest, 0, sizeof(m_szMacToTest));

    // test result record
    m_dwAvailIp = IP_NULL;
    memset(m_szAvailMac, 0, sizeof(m_szAvailMac));
    m_pipAvailIp = m_pipAvailMac = NULL;
}

CNetsettingDlg::~CNetsettingDlg()
{
    MSNET_ReleaseAdaptersInfo();
    MSNET_ReleaseNetworkParams();
}

void CNetsettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetsettingDlg)
	DDX_Control(pDX, IDC_CHK_IPSEARCH, m_chkIpSearch);
	DDX_Control(pDX, IDC_EDIT_SNMASK, m_editSnmask);
	DDX_Control(pDX, IDC_EDIT_LMAC, m_editLmac);
	DDX_Control(pDX, IDC_EDIT_LIP, m_editLip);
	DDX_Control(pDX, IDC_EDIT_GATEWAY, m_editGateway);
	DDX_Control(pDX, IDC_EDIT_DHCP, m_editDhcp);
	DDX_Control(pDX, IDC_EDIT_DEVDESC, m_editDevDesc);
	DDX_Control(pDX, IDC_COMBO_ADAPTER, m_comboAdapt);
	DDX_Control(pDX, IDC_COMBO_DNS2, m_comboDns2);
	DDX_Control(pDX, IDC_COMBO_DNS1, m_comboDns1);
	DDX_Control(pDX, IDC_CHK_DHCP, m_chkDhcp);
	DDX_Control(pDX, IDC_IPADDRESS_SIMU, m_ipAddr);
	DDX_Control(pDX, IDC_EDIT_MAC6, m_editMac6);
	DDX_Control(pDX, IDC_EDIT_MAC5, m_editMac5);
	DDX_Control(pDX, IDC_EDIT_MAC4, m_editMac4);
	DDX_Control(pDX, IDC_EDIT_MAC3, m_editMac3);
	DDX_Control(pDX, IDC_EDIT_MAC2, m_editMac2);
	DDX_Control(pDX, IDC_EDIT_MAC1, m_editMac1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetsettingDlg, CDialog)
	//{{AFX_MSG_MAP(CNetsettingDlg)
	ON_BN_CLICKED(IDC_CHK_DHCP, OnChkDhcp)
	ON_CBN_SELCHANGE(IDC_COMBO_ADAPTER, OnSelchangeComboAdapter)
	ON_BN_CLICKED(IDC_BTN_MACTEST, OnBtnMactest)
	ON_BN_CLICKED(IDC_BTN_IPTEST, OnBtnIptest)
	ON_BN_CLICKED(IDC_CHK_IPSEARCH, OnChkIpsearch)
	ON_EN_KILLFOCUS(IDC_EDIT_MAC1, OnKillfocusEditMac1)
	ON_EN_KILLFOCUS(IDC_EDIT_MAC2, OnKillfocusEditMac2)
	ON_EN_KILLFOCUS(IDC_EDIT_MAC3, OnKillfocusEditMac3)
	ON_EN_KILLFOCUS(IDC_EDIT_MAC4, OnKillfocusEditMac4)
	ON_EN_KILLFOCUS(IDC_EDIT_MAC5, OnKillfocusEditMac5)
	ON_EN_KILLFOCUS(IDC_EDIT_MAC6, OnKillfocusEditMac6)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_SIMU, OnFieldchangedIpaddressSimu)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetsettingDlg none-message handlers

/*****************************************************************************/
// Description : 初始化模拟器参数信息
// Global : 
// Author : yifeng.wang
// Note : 
/*****************************************************************************/
void CNetsettingDlg::InitSimulatorNetInfo( void )
{
    CString str;

    // if Mac Addr is NULL, load MAC from register
    if( 0 == m_netParam.szMacAddr[0] &&
        0 == m_netParam.szMacAddr[1] &&
        0 == m_netParam.szMacAddr[2] &&
        0 == m_netParam.szMacAddr[3] &&
        0 == m_netParam.szMacAddr[4] &&
        0 == m_netParam.szMacAddr[5] )
    {
        MSDEVPARAM_LoadFromReg(m_netParam.szMacAddr, ETHER_MAC_LEN, &m_netParam.uIpAddr);
    }

    // set MAC control text
    str.Format("%02X", m_netParam.szMacAddr[0]);
    m_editMac1.SetWindowText(str);
    str.Format("%02X", m_netParam.szMacAddr[1]);
    m_editMac2.SetWindowText(str);
    str.Format("%02X", m_netParam.szMacAddr[2]);
    m_editMac3.SetWindowText(str);
    str.Format("%02X", m_netParam.szMacAddr[3]);
    m_editMac4.SetWindowText(str);
    str.Format("%02X", m_netParam.szMacAddr[4]);
    m_editMac5.SetWindowText(str);
    str.Format("%02X", m_netParam.szMacAddr[5]);
    m_editMac6.SetWindowText(str);

    // limit MAC address edit control length
    m_editMac1.LimitText(MAC_UNIT_LEN);
    m_editMac2.LimitText(MAC_UNIT_LEN);
    m_editMac3.LimitText(MAC_UNIT_LEN);
    m_editMac4.LimitText(MAC_UNIT_LEN);
    m_editMac5.LimitText(MAC_UNIT_LEN);
    m_editMac6.LimitText(MAC_UNIT_LEN);

    // set IP address control
    m_dwIpToTest = m_netParam.uIpAddr;
    m_ipAddr.SetAddress(PUSH_IPADDR(m_netParam.uIpAddr));

    // init DHCP related controls
    m_chkIpSearch.SetCheck(TRUE);
    RefreshDhcpControls( 0 != m_netParam.uDhcp && m_netParam.bUseDhcp );
}

/*****************************************************************************/
// Description : 初始化系统网络信息
// Global : 
// Author : yifeng.wang
// Note : 
/*****************************************************************************/
void CNetsettingDlg::InitSystemNetInfo( void )
{
    PFIXED_INFO pFixedInfo = MSNET_GetNetworkParams();
    PIP_ADAPTER_INFO pAdaptBest = NULL, pAdapt = MSNET_GetAdaptersInfo();

    CString strAdapterName("");
    int nIndex = -1;

    if( NULL == pFixedInfo || NULL == pAdapt )
    {
        TRACE("%s(%d) : InitSystemNetInfo() - get network info failed!\n", FILE_NAME, __LINE__);
        m_chkDhcp.EnableWindow(FALSE);
        RefreshDhcpControls(FALSE);
        return;
    }

    // initialize from m_pFixedInfo
    if( NULL != pFixedInfo )
    {
        CString strDns;
        PIP_ADDR_STRING pAddrStr = &pFixedInfo->DnsServerList;

        // set and show DNS information
        while( NULL != pAddrStr )
        {
            strDns.Format("%s", pAddrStr->IpAddress.String );
            m_comboDns1.AddString(strDns);
            m_comboDns2.AddString(strDns);
            pAddrStr = pAddrStr->Next;  
        }
        // set current item focus
        m_comboDns1.SetCurSel(0);
        if( m_comboDns1.GetCount() <= 1 )
        {
            m_comboDns2.EnableWindow(FALSE);
            m_comboDns2.ShowWindow(FALSE);
        }
        else
        {
            m_comboDns2.SetCurSel((m_comboDns2.GetCount()>1)? 1:0);
        }
    }

    // initialize from m_pAdapterInfo  
    while( NULL != pAdapt )
    {
        if( MIB_IF_TYPE_ETHERNET == pAdapt->Type )
        {
            // set adapter device names
            strAdapterName.Format("%s", pAdapt->AdapterName);
            m_comboAdapt.AddString(strAdapterName);
        }
        pAdapt = pAdapt->Next;
    }

    // set combo curser to best default adapter
    pAdaptBest = MSNET_GetDefaultAdapter();
    nIndex = m_comboAdapt.FindString(0, pAdaptBest->AdapterName);
    m_comboAdapt.SetCurSel( (nIndex != CB_ERR)? nIndex : 0 );

    // show adapter network information
    m_comboAdapt.GetWindowText(strAdapterName);
    m_pCurrentAdapt = MSNET_GetAdapterByName(strAdapterName);
    RefreshAdaptNetInfo( m_pCurrentAdapt );
}

/*****************************************************************************/
// Description : 刷新网络设备信息
// Global : 
// Author : yifeng.wang
// Note : 
/*****************************************************************************/
void CNetsettingDlg::RefreshAdaptNetInfo( PIP_ADAPTER_INFO pAdapt )
{
	uint32 uDhcp = IP_NULL;
    CString str("");

    // input check
    if( NULL == pAdapt )
    {
        TRACE("%s(%d) : RefreshAdaptNetInfo() - invalid input!\n", FILE_NAME, __LINE__);
        m_chkDhcp.EnableWindow(FALSE);
        RefreshDhcpControls(FALSE);
        return;
    }

    // set local device description
    str.Format("%s", pAdapt->Description);
    m_editDevDesc.SetWindowText(str);

    // set local MAC
    str.Format("%02X:%02X:%02X:%02X:%02X:%02X",
        pAdapt->Address[0], pAdapt->Address[1], pAdapt->Address[2],
        pAdapt->Address[3], pAdapt->Address[4], pAdapt->Address[5]);
    m_editLmac.SetWindowText(str);

    // set local IP
    str.Format("%s", pAdapt->IpAddressList.IpAddress.String);
    m_editLip.SetWindowText(str);

    // set local subnet mask
    str.Format("%s", pAdapt->IpAddressList.IpMask.String);
    m_editSnmask.SetWindowText(str);
    
    // set local gateway
    str.Format("%s", pAdapt->GatewayList.IpAddress.String);
    m_editGateway.SetWindowText(str);

    // set DHCP server IP combobox
    str.Format("%s", pAdapt->DhcpServer.IpAddress.String);
    m_editDhcp.SetWindowText(str);

    // refresh simulator control state by DHCP parameter
    uDhcp = inet_addr(pAdapt->DhcpServer.IpAddress.String);
    //m_chkDhcp.EnableWindow(pAdapt->DhcpEnabled && IP_NULL != uDhcp);
    RefreshDhcpControls(pAdapt->DhcpEnabled && IP_NULL != uDhcp);
    ShowTestIp( !m_chkDhcp.GetCheck() );
    if( m_pipAvailMac != m_pCurrentAdapt )
    {
        m_btnMacTest.SetIcon(IDI_ICON_NO);
    }
}

/*****************************************************************************/
// Description : 根据是否使用 DHCP 更新相关控件显示
// Global : 
// Author : yifeng.wang
// Note : 
/*****************************************************************************/
void CNetsettingDlg::RefreshDhcpControls( BOOL bUseDhcp )
{
    if( bUseDhcp )
    {
        m_chkDhcp.SetCheck(TRUE);
        m_chkIpSearch.ShowWindow(FALSE);
        m_btnIpTest.ShowWindow(FALSE);
        m_ipAddr.EnableWindow(FALSE);
    }
    else
    {
        m_chkDhcp.SetCheck(FALSE);
        m_chkIpSearch.ShowWindow(TRUE);
        m_btnIpTest.ShowWindow(TRUE);
        m_ipAddr.EnableWindow(!m_chkIpSearch.GetCheck());
    }
    ShowTestIp( !bUseDhcp );
    m_btnIpTest.SetWindowText((m_chkIpSearch.GetCheck())? _T("IP Search"):_T("IP Test"));
}

/*****************************************************************************/
// Description : 显示测试 IP
// Global : 
// Author : yifeng.wang
// Note : 
/*****************************************************************************/
void CNetsettingDlg::ShowTestIp( BOOL bShowIp )
{
    // refresh test IP and control status
    if( NULL != m_pCurrentAdapt && m_pCurrentAdapt == m_pipAvailIp )
    {
        m_dwIpToTest = m_dwAvailIp;
    }

    // check IP to be presented
    if( bShowIp )
    {
        if( NULL != m_pCurrentAdapt )
        {
            // get current net adapter IP and subnet mask
            DWORD dwHostIp = inet_addr(m_pCurrentAdapt->IpAddressList.IpAddress.String);
            DWORD dwSnmask = inet_addr(m_pCurrentAdapt->IpAddressList.IpMask.String);
            DWORD dwGateway = inet_addr(m_pCurrentAdapt->GatewayList.IpAddress.String);

            dwHostIp = ntohl(dwHostIp);
            dwSnmask = ntohl(dwSnmask);
            dwGateway = ntohl(dwGateway);

            // get MIN & MAX of test IP range
            DWORD dwTestMin = (0 != (dwHostIp &  dwSnmask))? (dwHostIp &  dwSnmask) + 1 : 0;
            DWORD dwTestMax = (0 != (dwHostIp | ~dwSnmask))? (dwHostIp | ~dwSnmask) - 1 : 0;

            if( IP_NULL == m_dwIpToTest || IP_BROADCAST == m_dwIpToTest ||
                (IP_NULL != dwSnmask && (m_dwIpToTest & dwSnmask)!=(dwHostIp & dwSnmask)) ||
                (dwHostIp & dwSnmask) == m_dwIpToTest || (dwHostIp | ~dwSnmask) == m_dwIpToTest )
            {
                DWORD dwTestRange = (dwTestMax >= dwTestMin)? (dwTestMax-dwTestMin+1) : 1;

                // check if current test IP valid
                if( IP_NULL == m_dwIpToTest || IP_BROADCAST == m_dwIpToTest ||
                    (IP_NULL != dwSnmask && (m_dwIpToTest & dwSnmask) != (dwHostIp & dwSnmask)) ||
                    (dwHostIp & dwSnmask) == m_dwIpToTest || (dwHostIp | ~dwSnmask) == m_dwIpToTest )
                {
                    srand(GetTickCount());
                    do
                    {
                        m_dwIpToTest = dwTestMin + rand()%dwTestRange;
                    }
                    while( dwHostIp == m_dwIpToTest || dwGateway == m_dwIpToTest );
                }
            }
        }
    }

    // refresh test IP control
    if( IP_NULL == m_dwIpToTest || !bShowIp )
    {
        m_ipAddr.SetWindowText(_T(""));
    }
    else
    {
        m_ipAddr.SetAddress(PUSH_IPADDR(m_dwIpToTest));
    }
    m_btnIpTest.SetIcon( IsIpTested()? IDI_ICON_YES : IDI_ICON_NO );
}

/*****************************************************************************/
// Description : 判断待测 IP 地址是否合理
// Global : 
// Author : yifeng.wang
// Note : 
/*****************************************************************************/
BOOL CNetsettingDlg::IsTestIpValid( void )
{
    CString strMsg(""), strA(""), strB("");
    DWORD dwHostIp(0), dwSnmask(0), dwIpMin(0), dwIpMax(0);

    // 获得系统当前 IP 参数
    if( NULL != m_pCurrentAdapt )
    {
        dwHostIp = inet_addr(m_pCurrentAdapt->IpAddressList.IpAddress.String);
        dwHostIp = ntohl(dwHostIp);
        dwSnmask = inet_addr(m_pCurrentAdapt->IpAddressList.IpMask.String);
        dwSnmask = ntohl(dwSnmask);
    }
    else
    {
        CString str(_T("Network is unavailable, can not do IP test!"));
        MessageBox(str, _T("IP test"), MB_OK|MB_ICONSTOP);
        return FALSE;
    }

    // 获得 IP 合理取值范围
    dwIpMin = (0 != (dwHostIp &  dwSnmask))? (dwHostIp &  dwSnmask) + 1 : 0;
    dwIpMax = (0 != (dwHostIp | ~dwSnmask))? (dwHostIp | ~dwSnmask) - 1 : 0;

    // 获得待测试 IP 地址
    m_ipAddr.GetAddress(m_dwIpToTest);

    // IP 初步检查
    if( IP_NULL == m_dwIpToTest || IP_BROADCAST == m_dwIpToTest )        
    {
        strMsg.Format("%s", _T("test IP can't be 0 or broadcast!"));
        goto ERROR_END;
    }

    // 和当前系统网络参数进行比较
    if( NULL != m_pCurrentAdapt )
    {
        // 测试 IP 和本地 IP 处于同一网段
        if( (m_dwIpToTest & dwSnmask) != (dwHostIp & dwSnmask) ||
             m_dwIpToTest == (dwHostIp &  dwSnmask) ||
             m_dwIpToTest == (dwHostIp | ~dwSnmask) )
        {
            strA.Format("%u.%u.%u.%u", PUSH_IPADDR(dwIpMin));
            strB.Format("%u.%u.%u.%u", PUSH_IPADDR(dwIpMax));
            strMsg.Format(_T("test IP should between %s ~ %s!"),
                strA.GetBuffer(strA.GetLength()), strB.GetBuffer(strB.GetLength()));
            goto ERROR_END;
        }

        // 测试 IP 和本地 IP 相同
        if( m_dwIpToTest == dwHostIp )
        {
            strMsg.Format("%s", _T("test IP can't be the same as local host IP!"));
            goto ERROR_END;
        }

        // 测试 IP 和网关 IP 相同
        if( m_dwIpToTest == ntohl(inet_addr(m_pCurrentAdapt->GatewayList.IpAddress.String)) )
        {
            strMsg.Format("%s", _T("test IP can't be the same as Gateway!"));
            goto ERROR_END;
        }
        return TRUE;
    }
    else
    {
        TRACE("CNetsettingDlg::IsTestIpValid() - invalid adapter pointer!\n");
    }
    return FALSE;

ERROR_END:
    MessageBox(strMsg, _T("IP Check"), MB_OK|MB_ICONWARNING);
    return FALSE;
}

/*****************************************************************************/
// Description : 判断待测 MAC 地址是否合理
// Global : 
// Author : yifeng.wang
// Note : 
/*****************************************************************************/
BOOL CNetsettingDlg::IsTestMacValid( void )
{
    CString strMsgCap("MAC Check");
    BYTE szMacHost[ETHER_MAC_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    // get test MAC
    CString str1, str2, str3, str4, str5, str6;
    m_editMac1.GetWindowText( str1 );
    m_editMac2.GetWindowText( str2 );
    m_editMac3.GetWindowText( str3 );
    m_editMac4.GetWindowText( str4 );
    m_editMac5.GetWindowText( str5 );
    m_editMac6.GetWindowText( str6 );

    // check if MAC string length is valid
    if( (2 != str1.GetLength()) || (2 != str2.GetLength()) || (2 != str3.GetLength()) || 
        (2 != str4.GetLength()) || (2 != str5.GetLength()) || (2 != str6.GetLength()) )
    {
        MessageBox(_T("Set MAC address first!"), strMsgCap, MB_OK|MB_ICONWARNING);
        return FALSE;
    }

    // check if MAC all zero
    if( (str1 == "00") && (str2 == "00") && (str3 == "00") && (str4 == "00") && (str5 == "00") && (str6 == "00") )
    {
        MessageBox(_T("MAC address should not be zero!"), strMsgCap, MB_OK|MB_ICONWARNING);
        return FALSE;
    } 

    // get test MAC in hex value
    char *pStop = NULL;
    m_szMacToTest[0] = (BYTE)strtoul( str1, &pStop, 16 );
    m_szMacToTest[1] = (BYTE)strtoul( str2, &pStop, 16 );
    m_szMacToTest[2] = (BYTE)strtoul( str3, &pStop, 16 );
    m_szMacToTest[3] = (BYTE)strtoul( str4, &pStop, 16 );
    m_szMacToTest[4] = (BYTE)strtoul( str5, &pStop, 16 );
    m_szMacToTest[5] = (BYTE)strtoul( str6, &pStop, 16 );

    // compared with local MAC
    if( NULL != m_pCurrentAdapt )
    {
        memcpy(szMacHost, m_pCurrentAdapt->Address, ETHER_MAC_LEN);

        // check if host MAC and test MAC are equal
        if( 0 == memcmp(m_szMacToTest, szMacHost, ETHER_MAC_LEN) )
        {
            MessageBox(_T("Test MAC cann't be the same as host MAC!"), strMsgCap, MB_OK|MB_ICONWARNING);
            return FALSE;
        }
    }
    else
    {
        TRACE("warning: CNetsettingDlg::IsTestMacValid - m_pCurrentAdapt is NULL!\n");
        MessageBox(_T("MAC test is not allowed as host MAC is unavailable!"), strMsgCap, MB_OK|MB_ICONWARNING);
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************/
// Func Descrption : 初始化 ButtonST 风格按钮
// Global resource : none
// Note : 
/*****************************************************************************/
void CNetsettingDlg::InitialSTStyleButton( void )
{
    m_btnIpTest.SubclassDlgItem( IDC_BTN_IPTEST, this );
    m_btnIpTest.SetIcon( IsIpTested()? IDI_ICON_YES : IDI_ICON_NO );
    m_btnIpTest.SetFlat( FALSE );

    m_btnMacTest.SubclassDlgItem( IDC_BTN_MACTEST, this );
    m_btnMacTest.SetIcon( IsMacTested()? IDI_ICON_YES : IDI_ICON_NO );
    m_btnMacTest.SetFlat( FALSE );
}

/*****************************************************************************/
// Func Descrption : 判断并更新“MAC Test”图标
// Global resource : none
// Note : 
/*****************************************************************************/
//void CNetsettingDlg::UpdateMacTestStatus( void )
//{
//    if( NULL != m_pipAvailMac && m_pipAvailMac == m_pCurrentAdapt )
//    {
//        BYTE szMacCurr[ETHER_MAC_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//
//        // check if tested MAC is all zero
//        if( 0 == m_szAvailMac[0] && 0 == m_szAvailMac[1] && 0 == m_szAvailMac[2] &&
//            0 == m_szAvailMac[3] && 0 == m_szAvailMac[4] && 0 == m_szAvailMac[5] )
//        {
//            m_btnMacTest.SetIcon(IDI_ICON_NO);
//            return;
//        }
//
//        // get test MAC
//        CString str1, str2, str3, str4, str5, str6;
//        m_editMac1.GetWindowText( str1 );
//        m_editMac2.GetWindowText( str2 );
//        m_editMac3.GetWindowText( str3 );
//        m_editMac4.GetWindowText( str4 );
//        m_editMac5.GetWindowText( str5 );
//        m_editMac6.GetWindowText( str6 );
//
//        // check if MAC string length is valid
//        if( (2 != str1.GetLength()) || (2 != str2.GetLength()) || (2 != str3.GetLength()) || 
//            (2 != str4.GetLength()) || (2 != str5.GetLength()) || (2 != str6.GetLength()) )
//        {
//            m_btnMacTest.SetIcon(IDI_ICON_NO);
//            return;
//        }
//
//        // check if MAC all zero
//        if( (str2 == "00") && (str3 == "00") && (str4 == "00") && (str5 == "00") && (str6 == "00") )
//        {
//            m_btnMacTest.SetIcon(IDI_ICON_NO);
//            return;
//        }
//
//        // get test MAC in hex value
//        char *pStop = NULL;
//        szMacCurr[0] = (BYTE)strtoul( str1, &pStop, 16 );
//        szMacCurr[1] = (BYTE)strtoul( str2, &pStop, 16 );
//        szMacCurr[2] = (BYTE)strtoul( str3, &pStop, 16 );
//        szMacCurr[3] = (BYTE)strtoul( str4, &pStop, 16 );
//        szMacCurr[4] = (BYTE)strtoul( str5, &pStop, 16 );
//        szMacCurr[5] = (BYTE)strtoul( str6, &pStop, 16 );
//
//        // compared current MAC with last tested MAC 
//        if( 0 == memcmp(szMacCurr, m_szAvailMac, ETHER_MAC_LEN) )
//        {
//            m_btnMacTest.SetIcon(IDI_ICON_YES);
//        }
//        else
//        {
//            m_btnMacTest.SetIcon(IDI_ICON_NO);
//        }
//    }
//}

/*****************************************************************************/
// Func Descrption : 获得设定的参数
// Global resource : none
// Note : 
/*****************************************************************************/
BOOL CNetsettingDlg::GetParam( MSDEV_NETPARAM_T* pParam )
{
    if( NULL == pParam )
        return FALSE;

    memcpy(pParam, &m_netParam, sizeof(MSDEV_NETPARAM_T));
    return TRUE;
}

/*****************************************************************************/
// Func Descrption : 检查当前 IP 是否已经过测试
// Global resource : none
// Note : 
/*****************************************************************************/
BOOL CNetsettingDlg::IsIpTested( void )
{
    BOOL bIpTested = TRUE;
    if( !m_chkDhcp.GetCheck() )
    {
        DWORD dwCurrIp = IP_NULL;
        m_ipAddr.GetAddress(dwCurrIp);
        if( IP_NULL != m_dwAvailIp &&
            dwCurrIp == m_dwAvailIp &&
            NULL != m_pCurrentAdapt &&
            m_pipAvailIp == m_pCurrentAdapt )            
        {
            bIpTested = TRUE;
        }
        else
        {
            bIpTested = FALSE;
        }
    }
    return bIpTested;
}

/*****************************************************************************/
// Func Descrption : 检查当前 MAC 是否已经过测试
// Global resource : none
// Note : 
/*****************************************************************************/
BOOL CNetsettingDlg::IsMacTested( void )
{
    BOOL bMacTested = FALSE;
    BYTE szCurrMac[ETHER_MAC_LEN], szMacNull[ETHER_MAC_LEN];
    CString str1, str2, str3, str4, str5, str6;
    
    m_editMac1.GetWindowText(str1);
    m_editMac2.GetWindowText(str2);
    m_editMac3.GetWindowText(str3);
    m_editMac4.GetWindowText(str4);
    m_editMac5.GetWindowText(str5);
    m_editMac6.GetWindowText(str6);
    
    // get MAC in hex value
    char *pStop = NULL;
    szCurrMac[0] = (BYTE)strtoul(str1, &pStop, 16);
    szCurrMac[1] = (BYTE)strtoul(str2, &pStop, 16);
    szCurrMac[2] = (BYTE)strtoul(str3, &pStop, 16);
    szCurrMac[3] = (BYTE)strtoul(str4, &pStop, 16);
    szCurrMac[4] = (BYTE)strtoul(str5, &pStop, 16);
    szCurrMac[5] = (BYTE)strtoul(str6, &pStop, 16);
    
    memset(szMacNull, 0, sizeof(ETHER_MAC_LEN));
    if( 0 != memcmp(szCurrMac, szMacNull, ETHER_MAC_LEN) &&
        0 == memcmp(szCurrMac, m_szAvailMac, ETHER_MAC_LEN) &&
        NULL != m_pCurrentAdapt &&
        m_pipAvailMac == m_pCurrentAdapt )
    {
        bMacTested = TRUE;
    }
    return bMacTested;
}

/*****************************************************************************/
// Func Descrption : 将字符串写入文件
// Global resource : none
// Note : 
/*****************************************************************************/
BOOL CNetsettingDlg::WriteStringToFile( CStdioFile* pFile )
{
    CString strContent("");
    if( NULL == pFile )
        return FALSE;

    strContent.Format(_T("[%s]\n"), SECTION_NAME_BASE);
    pFile->WriteString(strContent);

    strContent.Format(_T("%s = %02X-%02X-%02X-%02X-%02X-%02X\n"),
        KEY_NAME_MAC, m_netParam.szMacAddr[0], m_netParam.szMacAddr[1], m_netParam.szMacAddr[2],
        m_netParam.szMacAddr[3], m_netParam.szMacAddr[4], m_netParam.szMacAddr[5]);
    pFile->WriteString(strContent);

    strContent.Format(_T("%s = %u.%u.%u.%u\n"), KEY_NAME_IP, PUSH_IPADDR(m_netParam.uIpAddr));
    pFile->WriteString(strContent);

    strContent.Format(_T("%s = %u.%u.%u.%u\n"), KEY_NAME_SNMASK, PUSH_IPADDR(m_netParam.uSnmask));
    pFile->WriteString(strContent);

    strContent.Format(_T("%s = %u.%u.%u.%u\n"), KEY_NAME_GATEWAY, PUSH_IPADDR(m_netParam.uGateway));
    pFile->WriteString(strContent);

    strContent.Format(_T("%s = %u.%u.%u.%u\n"), KEY_NAME_DNS_PRI, PUSH_IPADDR(m_netParam.uDns1));
    pFile->WriteString(strContent);

    strContent.Format(_T("%s = %u.%u.%u.%u\n"), KEY_NAME_DNS_SEC, PUSH_IPADDR(m_netParam.uDns2));
    pFile->WriteString(strContent);

    strContent.Format(_T("%s = %u.%u.%u.%u\n"), KEY_NAME_DHCP, PUSH_IPADDR(m_netParam.uDhcp));
    pFile->WriteString(strContent);
    
    strContent.Format(_T("%s = %s\n"), KEY_NAME_DEVICE_DESC,m_netParam.szDeviceDesc);
    pFile->WriteString(strContent);

    strContent.Format(_T("%s = %s\n"), KEY_NAME_DEVICE_NAME,m_netParam.szDeviceName);
    pFile->WriteString(strContent);

    return TRUE;
}

/*****************************************************************************/
// Func Descrption : 将参数保存到配置文件
// Global resource : none
// Note : 
/*****************************************************************************/
BOOL CNetsettingDlg::SaveParamToFile( void )
{
    // get file path name
    CString strFilePath;
    if( !MSDEVPARAM_GetFilePath(strFilePath) )
    {
        TRACE("warning: SaveParamToFile() - MSDEVPARAM_GetFilePath() failed!\n");
        return FALSE;
    }

    // create or open configure file
    CStdioFile      fileCfg;
    CFileException  excFile;
    if( !fileCfg.Open(strFilePath, (CFile::modeCreate|CFile::modeWrite|CFile::typeText), &excFile) ) 
    {
        CString strMsg, strTemp, strCap(_T("FILE ERROR"));
        TRACE("warning: open file \"%s\" failed!\n", strFilePath);
        strTemp.Format(_T("Open file \"%s\" failed!\n"), strFilePath);
        switch( excFile.m_cause )
        {   
        case CFileException::none:              strMsg.Format("%sNo error!", strTemp);                  break;
        case CFileException::generic:           strMsg.Format("%sGeneric error!", strTemp);             break;
        case CFileException::fileNotFound:      strMsg.Format("%sNo file found!", strTemp);             break;
        case CFileException::badPath:           strMsg.Format("%sBad path!", strTemp);                  break;
        case CFileException::tooManyOpenFiles:  strMsg.Format("%sToo many open files!", strTemp);       break;
        case CFileException::accessDenied:      strMsg.Format("%sAccess Denied!", strTemp);             break;
        case CFileException::invalidFile:       strMsg.Format("%sInvalid file!", strTemp);              break;
        case CFileException::removeCurrentDir:  strMsg.Format("%sRemove current directory!", strTemp);  break;
        case CFileException::directoryFull:     strMsg.Format("%sDirectory full!", strTemp);            break;
        case CFileException::badSeek:           strMsg.Format("%sBad seek pointer!", strTemp);          break;
        case CFileException::hardIO:            strMsg.Format("%sHardware IO error!", strTemp);         break;
        case CFileException::sharingViolation:  strMsg.Format("%sSharing violation!", strTemp);         break;
        case CFileException::lockViolation:     strMsg.Format("%sAccess lock violation!", strTemp);     break;
        case CFileException::diskFull:          strMsg.Format("%sDisk full!", strTemp);                 break;
        case CFileException::endOfFile:         strMsg.Format("%sEnd of file!", strTemp);               break;
        default:
            strMsg.Format("%sUnknown error!", strTemp);
            break;
        }
        MessageBox(strMsg, strCap, MB_OK|MB_ICONSTOP);
        return FALSE;
    }

    // write string to file
    WriteStringToFile( &fileCfg );

    fileCfg.Close();
    return TRUE;
}

/*****************************************************************************/
// Func Descrption : 保存参数到注册表
// Global resource : 
// Note : 
/*****************************************************************************/
void CNetsettingDlg::SaveParaToSystem( void )
{
    // 打开注册表记录入口
	CRegOpt regOpt(HKEY_LOCAL_MACHINE);
	CString strKeyPath(REG_KEYPATH);
	if( !regOpt.VerifyKey(strKeyPath) )	
    {
		if( !regOpt.CreateKey(HKEY_LOCAL_MACHINE, strKeyPath) )	
        {
			TRACE("warning: SaveParaToSystem - create register key failed!\n");
			return;
		}
	}
	
    // 保存参数到注册表
	CString strIp, strMac;
    strIp.Format("%u.%u.%u.%u", PUSH_IPADDR(m_netParam.uIpAddr));
    regOpt.Write( REG_KEYNAME_VIP, strIp );
    strMac.Format("%02X", m_netParam.szMacAddr[0]);
    regOpt.Write( REG_KEYNAME_VMAC1, strMac );
    strMac.Format("%02X", m_netParam.szMacAddr[1]);
    regOpt.Write( REG_KEYNAME_VMAC2, strMac );
    strMac.Format("%02X", m_netParam.szMacAddr[2]);
    regOpt.Write( REG_KEYNAME_VMAC3, strMac );
    strMac.Format("%02X", m_netParam.szMacAddr[3]);
    regOpt.Write( REG_KEYNAME_VMAC4, strMac );
    strMac.Format("%02X", m_netParam.szMacAddr[4]);
    regOpt.Write( REG_KEYNAME_VMAC5, strMac );
    strMac.Format("%02X", m_netParam.szMacAddr[5]);
    regOpt.Write( REG_KEYNAME_VMAC6, strMac );
}


/////////////////////////////////////////////////////////////////////////////
// CNetsettingDlg message handlers

BOOL CNetsettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    InitialSTStyleButton();
    InitSimulatorNetInfo();
    InitSystemNetInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 复选框“Use DHCP”点击响应
void CNetsettingDlg::OnChkDhcp() 
{
    RefreshDhcpControls(m_chkDhcp.GetCheck());
}

// 复选框“IP Search”点击响应
void CNetsettingDlg::OnChkIpsearch() 
{
    BOOL bIpSearch = m_chkIpSearch.GetCheck();
	m_ipAddr.EnableWindow(!bIpSearch);
    m_btnIpTest.SetWindowText((bIpSearch)? _T("IP Search"):_T("IP Test"));
    ShowTestIp(TRUE);
}

// 列表框“Adapter”选项发生变动的响应
void CNetsettingDlg::OnSelchangeComboAdapter() 
{
    CString strName("");
    PIP_ADAPTER_INFO pAdapt = NULL;
    m_comboAdapt.GetWindowText(strName);
    pAdapt = MSNET_GetAdapterByName(strName);
    if( NULL != pAdapt && pAdapt != m_pCurrentAdapt )
    {
        m_pCurrentAdapt = pAdapt;
        RefreshAdaptNetInfo( m_pCurrentAdapt );
    }
    else
    {
        if( NULL == pAdapt )
        {
            TRACE("%s(%d) : OnSelchangeComboAdapter() - invalid adapter pointer!\n", FILE_NAME, __LINE__);
        }
    }
}

// 按钮“MAC Test”响应
void CNetsettingDlg::OnBtnMactest() 
{
	if( IsTestMacValid() )
    {
        if( NULL != m_pCurrentAdapt )
        {
            // get local MAC
            BYTE szMacHost[ETHER_MAC_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
            memcpy(szMacHost, m_pCurrentAdapt->Address, ETHER_MAC_LEN);

            // set input parameter
            MACTESTING_INPUT_PARAM_T param;

            // set IP parameters
            CString str("");
            m_editLip.GetWindowText(str);
            param.dwLocalIp = inet_addr(str.GetBuffer(str.GetLength()));
            param.dwLocalIp = ntohl(param.dwLocalIp);
            m_editSnmask.GetWindowText(str);
            param.dwSnmask = inet_addr(str.GetBuffer(str.GetLength()));
            param.dwSnmask = ntohl(param.dwSnmask);
            m_editGateway.GetWindowText(str);
            param.dwGateway = inet_addr(str.GetBuffer(str.GetLength()));
            param.dwGateway = ntohl(param.dwGateway);

            // set MAC address
            memcpy(param.szMacHost, szMacHost, ETHER_MAC_LEN);
            memcpy(param.szMacTest, m_szMacToTest, ETHER_MAC_LEN);
            _snprintf(param.szDeviceName, DEVICE_NAME_LEN, "%s", m_pCurrentAdapt->AdapterName);
            param.szDeviceName[DEVICE_NAME_LEN-1] = 0;

            // set available IP
            param.dwAvailIp = 
                ( IP_NULL != m_dwAvailIp && m_pCurrentAdapt == m_pipAvailIp )? m_dwAvailIp : IP_NULL;

            // open MAC testing dialog
            CMacTestingDlg dlg(&param);
            dlg.DoModal();
            if( dlg.IsTestMacUsable() )
            {
                //m_bMacAvail = TRUE;
                memcpy(m_szAvailMac, param.szMacTest, ETHER_MAC_LEN);
                m_pipAvailMac = m_pCurrentAdapt;
                m_btnMacTest.SetIcon(IsMacTested()? IDI_ICON_YES : IDI_ICON_NO);
            }
        }
    }
}

// 按钮“IP Test/IP Search”响应
void CNetsettingDlg::OnBtnIptest() 
{
    if( m_chkIpSearch.GetCheck() )
    {
        CString str("");
        IPSEARCH_INPUT_PARAM_T param;
        memset(&param, 0, sizeof(param));
        
        // set IP parameters
        m_editLip.GetWindowText(str);
        param.dwLocalIp = inet_addr(str.GetBuffer(str.GetLength()));
        param.dwLocalIp = ntohl(param.dwLocalIp);
        m_editSnmask.GetWindowText(str);
        param.dwSnmask = inet_addr(str.GetBuffer(str.GetLength()));
        param.dwSnmask = ntohl(param.dwSnmask);
        m_editGateway.GetWindowText(str);
        param.dwGateway = inet_addr(str.GetBuffer(str.GetLength()));
        param.dwGateway = ntohl(param.dwGateway);
        
        // set MAC addr
        if( NULL != m_pCurrentAdapt )
        {
            memcpy(param.szMacAddr, m_pCurrentAdapt->Address, ETHER_MAC_LEN);
            _snprintf(param.szDeviceName, DEVICE_NAME_LEN, "%s", m_pCurrentAdapt->AdapterName);
            param.szDeviceName[DEVICE_NAME_LEN-1] = 0;
        }
        else
        {
            str.Format(_T("Network is unavailable, can not do IP search!"));
            MessageBox(str, _T("IP search"), MB_OK|MB_ICONSTOP);
            return;
        }
        
        // check gateway
        if( IP_NULL == param.dwGateway || IP_BROADCAST == param.dwGateway )
        {
            str.Format(_T("Gateway is invalid, can not do IP search!"));
            MessageBox(str, _T("IP search"), MB_OK|MB_ICONSTOP);
            return;
        }
        
        // open test control dialog
        CIpSearchDlg dlg(&param);
        dlg.DoModal();
        if( IP_NULL != dlg.GetAvailIp() )
        {
            //m_bIpAvail = TRUE;
            m_dwAvailIp = dlg.GetAvailIp();
            m_ipAddr.SetAddress(PUSH_IPADDR(m_dwAvailIp));
            m_pipAvailIp = m_pCurrentAdapt;
            //m_btnIpTest.SetIcon((m_bIpAvail)? IDI_ICON_YES : IDI_ICON_NO);
            m_btnIpTest.SetIcon( IsIpTested()? IDI_ICON_YES : IDI_ICON_NO );
        }
    }
    else
    {
        if( IsTestIpValid() )
        {
            CString str("");
            IPTESTING_INPUT_PARAM_T param;
            memset(&param, 0, sizeof(param));

            // set input parameters
            param.dwTestIp = m_dwIpToTest;

            // get local IP
            m_editLip.GetWindowText(str);
            param.dwLocalIp = inet_addr(str.GetBuffer(str.GetLength()));
            param.dwLocalIp = ntohl(param.dwLocalIp);

            // get gateway
            m_editGateway.GetWindowText(str);
            param.dwGateway = inet_addr(str.GetBuffer(str.GetLength()));
            param.dwGateway = ntohl(param.dwGateway);

            // check gateway
            if( IP_NULL == param.dwGateway || IP_BROADCAST == param.dwGateway )
            {
                str.Format(_T("Gateway is invalid, can not do IP test!"));
                MessageBox(str, _T("IP test"), MB_OK|MB_ICONSTOP);
                return;
            }

            // set MAC addr
            if( NULL != m_pCurrentAdapt )
            {
                memcpy(param.szMacAddr, m_pCurrentAdapt->Address, ETHER_MAC_LEN);
                _snprintf(param.szDeviceName, DEVICE_NAME_LEN, "%s", m_pCurrentAdapt->AdapterName);
                param.szDeviceName[DEVICE_NAME_LEN-1] = 0;
            }
            else
            {
                str.Format(_T("Network is unavailable, can not do IP test!"));
                MessageBox(str, _T("IP test"), MB_OK|MB_ICONSTOP);
                return;
            }

            // open testing dialog
            CIpTestingDlg dlg(&param);
            dlg.DoModal();
            if( dlg.IsTestIpUsable() )
            {
                //m_bIpAvail = TRUE;
                m_dwAvailIp = param.dwTestIp;
                m_pipAvailIp = m_pCurrentAdapt;
                TRACE("OnBtnIptest: IP %u.%u.%u.%u is available!\n", PUSH_IPADDR(param.dwTestIp));
            }
            else
            {
                //m_bIpAvail = FALSE;
                if( m_dwAvailIp == param.dwTestIp )
                {
                    m_dwAvailIp = IP_NULL;
                    m_pipAvailIp = NULL;
                }
                //TRACE("debug: IP %u.%u.%u.%u is not available!\n", PUSH_IPADDR(param.dwTestIp));
            }
            //m_btnIpTest.SetIcon((m_bIpAvail)? IDI_ICON_YES : IDI_ICON_NO);
            m_btnIpTest.SetIcon( IsIpTested()? IDI_ICON_YES : IDI_ICON_NO );
        }
    }
}

void CNetsettingDlg::OnKillfocusEditMac1() 
{
	//UpdateMacTestStatus();
    m_btnMacTest.SetIcon( IsMacTested()? IDI_ICON_YES : IDI_ICON_NO );
}

void CNetsettingDlg::OnKillfocusEditMac2() 
{
	//UpdateMacTestStatus();
    m_btnMacTest.SetIcon( IsMacTested()? IDI_ICON_YES : IDI_ICON_NO );
}

void CNetsettingDlg::OnKillfocusEditMac3() 
{
	//UpdateMacTestStatus();
    m_btnMacTest.SetIcon( IsMacTested()? IDI_ICON_YES : IDI_ICON_NO );
}

void CNetsettingDlg::OnKillfocusEditMac4() 
{
	//UpdateMacTestStatus();
    m_btnMacTest.SetIcon( IsMacTested()? IDI_ICON_YES : IDI_ICON_NO );
}

void CNetsettingDlg::OnKillfocusEditMac5() 
{
	//UpdateMacTestStatus();
    m_btnMacTest.SetIcon( IsMacTested()? IDI_ICON_YES : IDI_ICON_NO );
}

void CNetsettingDlg::OnKillfocusEditMac6() 
{
	//UpdateMacTestStatus();
    m_btnMacTest.SetIcon( IsMacTested()? IDI_ICON_YES : IDI_ICON_NO );
}

void CNetsettingDlg::OnOK() 
{
    // parameter saving notes
    if( !IsIpTested() || !IsMacTested() )
    {
        int ret = MessageBox(
            _T("WARNING: Simulator parameters are not tested!\nPress YES to save parameters or NO to cancel!"),
            _T("Saving Notes"), MB_YESNO|MB_ICONINFORMATION);
        if( IDNO == ret )
        {
            return;
        }
    }

	// save parameters
    DWORD dwIp = IP_NULL;
    CString str("");
    memset(&m_netParam, 0, sizeof(m_netParam));

    // get IP
    m_ipAddr.GetAddress(dwIp);
    m_netParam.uIpAddr = dwIp;

    // get subnet mask
    m_editSnmask.GetWindowText(str);
    dwIp = inet_addr(str.GetBuffer(str.GetLength()));
    m_netParam.uSnmask = ntohl(dwIp);

    // get gateway
    m_editGateway.GetWindowText(str);
    dwIp = inet_addr(str.GetBuffer(str.GetLength()));
    m_netParam.uGateway = ntohl(dwIp);

    // get dns1
    m_comboDns1.GetWindowText(str);
    dwIp = inet_addr(str.GetBuffer(str.GetLength()));
    m_netParam.uDns1 = ntohl(dwIp);

    // get dns2
    m_comboDns2.GetWindowText(str);
    dwIp = inet_addr(str.GetBuffer(str.GetLength()));
    m_netParam.uDns2 = ntohl(dwIp);

    // get DHCP
    m_editDhcp.GetWindowText(str);
    dwIp = inet_addr(str.GetBuffer(str.GetLength()));
    m_netParam.bUseDhcp = m_chkDhcp.GetCheck();
	if( m_netParam.bUseDhcp )
	{
		m_netParam.uDhcp = (0 != dwIp)? ntohl(dwIp) : IP_BROADCAST;
	}
	else
	{
		m_netParam.uDhcp = IP_NULL;
	}

    // get MAC address
    {
        // get current MAC address
        BYTE szMacCurr[ETHER_MAC_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        CString str1, str2, str3, str4, str5, str6;
        m_editMac1.GetWindowText( str1 );
        m_editMac2.GetWindowText( str2 );
        m_editMac3.GetWindowText( str3 );
        m_editMac4.GetWindowText( str4 );
        m_editMac5.GetWindowText( str5 );
        m_editMac6.GetWindowText( str6 );

        // convert MAC string in hex value
        char *pStop = NULL;
        szMacCurr[0] = (BYTE)strtoul( str1, &pStop, 16 );
        szMacCurr[1] = (BYTE)strtoul( str2, &pStop, 16 );
        szMacCurr[2] = (BYTE)strtoul( str3, &pStop, 16 );
        szMacCurr[3] = (BYTE)strtoul( str4, &pStop, 16 );
        szMacCurr[4] = (BYTE)strtoul( str5, &pStop, 16 );
        szMacCurr[5] = (BYTE)strtoul( str6, &pStop, 16 );
        memcpy(m_netParam.szMacAddr, szMacCurr, ETHER_MAC_LEN);
    }

    // get device description and name
    if( NULL != m_pCurrentAdapt )
    {
        _snprintf(m_netParam.szDeviceDesc, DEVICE_DESC_LEN, "%s", m_pCurrentAdapt->Description);
        m_netParam.szDeviceDesc[DEVICE_DESC_LEN-1] = 0;

        _snprintf(m_netParam.szDeviceName, DEVICE_NAME_LEN, "\\Device\\NPF_%s", m_pCurrentAdapt->AdapterName);
        m_netParam.szDeviceName[DEVICE_NAME_LEN-1] = 0;
    }

    // save parameter to file
    SaveParamToFile();

    // save parameter to register
    SaveParaToSystem();

	CDialog::OnOK();
}

void CNetsettingDlg::OnFieldchangedIpaddressSimu(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    m_btnIpTest.SetIcon( IsIpTested()? IDI_ICON_YES : IDI_ICON_NO );

	*pResult = 0;
}

HBRUSH CNetsettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if( NULL != pWnd )
    {
        switch( pWnd->GetDlgCtrlID() )
        {
        case IDC_STATIC_NOTE1:
        case IDC_STATIC_NOTE2:
            pDC->SetTextColor( COLOR_BLUE );
            pDC->SetBkMode( TRANSPARENT );
            break;
        }
    }

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
