// IpSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VNetSetting.h"

#include <WINSOCK2.H>
#include "msnet_param.h"
#include "net_test.h"

#include "IpSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIpSearchDlg macros

#define SEARCH_ENDING_TIMER_ID  0x01
#define SEARCH_ENDING_TIME_LEN  800


/////////////////////////////////////////////////////////////////////////////
// CIpSearchDlg dialog


CIpSearchDlg::CIpSearchDlg(
    IPSEARCH_INPUT_PARAM_T* pParam, /*=NULL*/
    CWnd* pParent /*=NULL*/)
	: CDialog(CIpSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIpSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    if( NULL != pParam )
    {
        memcpy(&m_input, pParam, sizeof(m_input));
        m_dwIpMin = (0 != (m_input.dwLocalIp & m_input.dwSnmask))?
            (m_input.dwLocalIp & m_input.dwSnmask) + 1 : 0;
        m_dwIpMax = (0 != (m_input.dwLocalIp | ~m_input.dwSnmask))?
            (m_input.dwLocalIp | ~m_input.dwSnmask) - 1 : 0;
    }
    else
    {
        memset(&m_input, 0, sizeof(m_input));
        m_dwIpMin = m_dwIpMax = IP_NULL;
    }
    m_dwIpAvail = IP_NULL;
    m_bSearchForward = TRUE;
    m_bSearching = FALSE;
    m_dwTestIp = IP_NULL;
    m_bInConnTest = m_bInIpTest = FALSE;
}

CIpSearchDlg::~CIpSearchDlg()
{
    NETTEST_CloseEthernetTask();
}

void CIpSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIpSearchDlg)
	DDX_Control(pDX, IDC_BTN_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_COMBO_RESULT, m_comboResult);
	DDX_Control(pDX, IDC_STATIC_NOTES, m_staticNotes);
	DDX_Control(pDX, IDC_RADIO_FORWARD, m_radioForw);
	DDX_Control(pDX, IDC_RADIO_BACKWARD, m_radioBack);
	DDX_Control(pDX, IDC_IPADDRESS_END, m_ipEnd);
	DDX_Control(pDX, IDC_IPADDRESS_BEGIN, m_ipBegin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIpSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CIpSearchDlg)
	ON_BN_CLICKED(IDC_RADIO_FORWARD, OnRadioForward)
	ON_BN_CLICKED(IDC_RADIO_BACKWARD, OnRadioBackward)
	ON_BN_CLICKED(IDC_BTN_NEXT, OnBtnNext)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
    // 自定义消息响应
    ON_MESSAGE( WM_ETHERNET_RECV_PACKET,  OnEthernetRecvPacket  )
    ON_MESSAGE( WM_ETHERNET_RECV_TIMEOUT, OnEthernetRecvTimeout )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIpSearchDlg none-message handlers

/***************************************************************************/
// Descrption : check IP searching parameters
// Global resource : 
// Author: yifeng.wang 
// Note: 
/***************************************************************************/
BOOL CIpSearchDlg::RangeCheck( void )
{
    CString strMsg("");
    BOOL ret = FALSE;
    DWORD dwIpMin = (0 != (m_input.dwLocalIp & m_input.dwSnmask))?
        (m_input.dwLocalIp & m_input.dwSnmask) + 1 : 0;
    DWORD dwIpMax = (0 != (m_input.dwLocalIp | ~m_input.dwSnmask))?
        (m_input.dwLocalIp | ~m_input.dwSnmask) - 1 : 0;

    // auto search
    m_ipBegin.GetAddress(m_dwIpMin);
    m_ipEnd.GetAddress(m_dwIpMax);

    // swap IP range if possible
    if( m_dwIpMax < m_dwIpMin )
    {
        DWORD dwSwap = m_dwIpMin;
        m_dwIpMin = m_dwIpMax;
        m_dwIpMax = dwSwap;
    }

    // normal check
    if( IP_NULL == m_dwIpMin || IP_BROADCAST == m_dwIpMin ||
        IP_NULL == m_dwIpMax || IP_BROADCAST == m_dwIpMax )        
    {
        strMsg.Format("%s", _T("test IP can't be 0 or broadcast!"));
        goto ERROR_END;
    }

    // check if IP in valid range
    if( (m_dwIpMin & m_input.dwSnmask) != (m_input.dwLocalIp & m_input.dwSnmask) ||
         m_dwIpMin == (m_input.dwLocalIp &  m_input.dwSnmask) ||
        (m_dwIpMax & m_input.dwSnmask) != (m_input.dwLocalIp & m_input.dwSnmask) ||
         m_dwIpMax == (m_input.dwLocalIp | ~m_input.dwSnmask) )
    {
        CString strA(""), strB("");
        strA.Format(_T("%u.%u.%u.%u"), PUSH_IPADDR(dwIpMin));
        strB.Format(_T("%u.%u.%u.%u"), PUSH_IPADDR(dwIpMax));
        strMsg.Format(_T("IP Search range should be in %s ~ %s!"),
            strA.GetBuffer(strA.GetLength()), strB.GetBuffer(strB.GetLength()));
        goto ERROR_END;
    }

    return TRUE;

ERROR_END:
    MessageBox(strMsg, _T("Range Check"), MB_OK|MB_ICONWARNING);
    return FALSE;
}

/***************************************************************************/
// Descrption : network connection test
// Global resource : 
// Author: yifeng.wang 
// Note: 
/***************************************************************************/
int CIpSearchDlg::StartConnTest( void )
{
    // ARP request parameters
    BYTE  ucMacSrc[ETHER_MAC_LEN];
    DWORD dwIPSrc(0), dwIPDst(0);

    // check network connection status
    if( !MSNET_IsNetworkConnected() )
    {
        TRACE("CIpSearchDlg::StartConnTest - MSNET_IsNetworkConnected() return FALSE!\n");
        ::PostMessage( GetSafeHwnd(), WM_ETHERNET_RECV_TIMEOUT, (WPARAM)(TYPE_TEST_CONN), 0 );
        //return ETHERR_OK;
    }

    // set parameters
    memcpy(ucMacSrc, m_input.szMacAddr, ETHER_MAC_LEN);
    dwIPSrc = htonl( m_input.dwLocalIp );
    dwIPDst = htonl( m_input.dwGateway );

    // set test time
    NETTEST_SetTestTime(GetTickCount(), NET_CONN_TEST_TIME_OUT);

    // send ARP request packet
    return NETTEST_SendNetTestData(TYPE_TEST_CONN, ucMacSrc, dwIPSrc, dwIPDst);
}

/***************************************************************************/
// Descrption : IP test
// Global resource : 
// Author: yifeng.wang 
// Note: 
/***************************************************************************/
int CIpSearchDlg::StartIpTest( DWORD dwIpTest )
{
    // ARP request parameters
    BYTE  ucMacSrc[ETHER_MAC_LEN];
    DWORD dwIPSrc(0), dwIPDst(0);

    // set parameters
    memcpy(ucMacSrc, m_input.szMacAddr, ETHER_MAC_LEN);
    dwIPSrc = htonl( m_input.dwLocalIp );
    dwIPDst = htonl( dwIpTest );

    // set test time
    NETTEST_SetTestTime(GetTickCount(), NET_IP_TEST_TIME_OUT);

    // send ARP request packet
    return NETTEST_SendNetTestData(TYPE_TEST_IP, ucMacSrc, dwIPSrc, dwIPDst);
}

/***************************************************************************/
// Descrption : start test
// Global resource : 
// Author: yifeng.wang 
// Note: 
/***************************************************************************/
void CIpSearchDlg::TestStart( void )
{
    CString strMsg("");
    int ret_val = 0;

    m_bInIpTest = m_bInConnTest = FALSE;

    // create ethernet task
    if( !NETTEST_CreateEthernetTask(this->GetSafeHwnd(), m_input.szDeviceName) )
    {
        ret_val = NETTEST_GetEthernetTaskError();
        TRACE("CIpSearchDlg::TestStart - NETTEST_CreateEthernetTask() error %d!\n", ret_val);
        strMsg.Format("ERROR: start IP searching error %d!", ret_val);
        TestEnd(strMsg, TRUE);
        return;
    }

    // check if in IP searching
    if( m_bSearching )
    {
        strMsg.Format(_T("ERROR: Searching is already running ...!"));
        TestEnd(strMsg, TRUE);
        return;
    }

    // check ethernet receive task is running
    if( !NETTEST_IsEthernetTaskRun() )
    {
        TestEnd(NETTEST_GetLastErrorMsg(), TRUE);
        return;
    }

    // start network connection test first, when this test succeed, then start IP test
    ret_val = StartConnTest();
    if( ETHERR_OK == ret_val )
    {
        m_bInConnTest = TRUE;
        m_staticNotes.SetWindowText(_T("Check network status ..."));
    }
    else
    {
        TestEnd(NETTEST_GetErrorMsg(ret_val), TRUE);
    }
}

/***************************************************************************/
// Descrption : end test
// Global resource : 
// Author: yifeng.wang 
// Note: 
/***************************************************************************/
void CIpSearchDlg::TestEnd( CString strMsg, BOOL bEnd )
{
    // refresh notes
    //TRACE("yifengdebug: %s\n", strMsg.GetBuffer(strMsg.GetLength()));
    m_staticNotes.SetWindowText(strMsg);

    if( bEnd )
    {
        m_bInIpTest = m_bInConnTest = FALSE;
        NETTEST_CloseEthernetTask();

        // check ethernet receive task is running
        SetTimer(SEARCH_ENDING_TIMER_ID, SEARCH_ENDING_TIME_LEN, 0);
    }
}

/***************************************************************************/
// Descrption : get next test IP
// Global resource : 
// Author: yifeng.wang 
// Note: 
/***************************************************************************/
BOOL CIpSearchDlg::GetNextTestIp( DWORD* pTestIp )
{
    BOOL ret = TRUE;

    // check input
    if( NULL == pTestIp )
        return FALSE;

    // loop for next test IP
    do
    {
        // check if IP has gone to range edge
        if( ( m_bSearchForward && m_dwTestIp == m_dwIpMax) ||
            (!m_bSearchForward && m_dwTestIp == m_dwIpMin) )
        {
            ret = FALSE;
            break;
        }
        // change by 1 on searching direction
        m_dwTestIp = (m_bSearchForward)?
            (m_dwTestIp + 1) : (m_dwTestIp - 1);
    }
    while( m_dwTestIp == m_input.dwLocalIp || m_dwTestIp == m_input.dwGateway );
    *pTestIp = m_dwTestIp;
    return ret;
}

/***************************************************************************/
// Descrption : get available IP
// Global resource : 
// Author: yifeng.wang 
// Note: 
/***************************************************************************/
DWORD CIpSearchDlg::GetAvailIp( void )
{
    return m_dwIpAvail;
}

/***************************************************************************/
// Descrption : add searching result to combo list
// Global resource : 
// Author: yifeng.wang 
// Note: 
/***************************************************************************/
void CIpSearchDlg::AddStringToResultList( CString str )
{
    int nMatch(-1);
    nMatch = m_comboResult.FindString(0, str);
    if( -1 == nMatch )
    {
        m_comboResult.AddString(str);
        m_comboResult.SetCurSel(m_comboResult.GetCount()-1);
    }
    else
    {
        m_comboResult.SetCurSel(nMatch);
    }
}


/////////////////////////////////////////////////////////////////////////////
// CIpSearchDlg message handlers

BOOL CIpSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_radioForw.SetCheck(m_bSearchForward);
    m_radioBack.SetCheck(!m_bSearchForward);

    m_ipBegin.SetAddress(PUSH_IPADDR(m_dwIpMin));
    m_ipEnd.SetAddress(PUSH_IPADDR(m_dwIpMax));
    
    m_comboResult.EnableWindow(m_comboResult.GetCount());
    m_staticNotes.SetWindowText(_T("Ready to start searching ..."));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIpSearchDlg::OnRadioForward() 
{
	SetDlgItemText(IDC_STATIC_RANGE, _T("->"));
	m_radioBack.SetCheck(FALSE);
}

void CIpSearchDlg::OnRadioBackward() 
{
	SetDlgItemText(IDC_STATIC_RANGE, _T("<-"));
	m_radioForw.SetCheck(FALSE);
}

void CIpSearchDlg::OnBtnNext() 
{
	// TODO: Add your control notification handler code here
	if( RangeCheck() )
    {
        m_btnNext.EnableWindow(FALSE);
        m_bSearchForward = m_radioForw.GetCheck();
        m_dwTestIp = (m_bSearchForward)? m_dwIpMin : m_dwIpMax;
        TestStart();
    }
}

/***************************************************************************/
// Descrption : 以太网接收应答成功的消息响应
// Global resource : 
// Author: yifeng.wang 
// Note: 
/***************************************************************************/
LONG CIpSearchDlg::OnEthernetRecvPacket( WPARAM wParam, LPARAM lParam )
{
    CString strMsg("");
    int ret_val = ETHERR_OK;
    DWORD dwTestIp = IP_NULL;
    NETTEST_TYPE_E eTestType = (NETTEST_TYPE_E)wParam;

    switch( eTestType )
    {
    // connection test result, then go on to next step
    case TYPE_TEST_CONN:
        m_dwTestIp = (m_bSearchForward)? m_dwIpMin : m_dwIpMax;
        strMsg.Format(_T("start check IP %u.%u.%u.%u ..."), PUSH_IPADDR(m_dwTestIp));
        TestEnd(strMsg, FALSE);
        ret_val = StartIpTest( m_dwTestIp );
        if( ETHERR_OK == ret_val )
        {
            m_bInIpTest = TRUE;
        }
        else
        {
            TestEnd(NETTEST_GetErrorMsg(ret_val), TRUE);
        }
        break;

    // IP testing result, means IP is in-use
    case TYPE_TEST_IP:
        if( GetNextTestIp( &dwTestIp ) )
        {
            //TRACE("start check IP %u.%u.%u.%u ...\n", PUSH_IPADDR(dwTestIp));
            strMsg.Format(_T("start check IP %u.%u.%u.%u ..."), PUSH_IPADDR(m_dwTestIp));
            TestEnd(strMsg, FALSE);
            StartIpTest( dwTestIp );
        }
        else
        {
            TRACE("IP searching is END!\n");
            strMsg.Format(_T("IP searching is END!"));
            TestEnd(strMsg, TRUE);
        }
        break;

    default:
        strMsg.Format(_T("ERROR: wrong test type - %d"), eTestType);
        TestEnd(strMsg, TRUE);
        break;
    }
    return 0;
}

/***************************************************************************/
// Descrption : 以太网接收超时的消息响应
// Global resource : 
// Author: yifeng.wang 
// Note: 
/***************************************************************************/
LONG CIpSearchDlg::OnEthernetRecvTimeout( WPARAM wParam, LPARAM lParam )
{
    CString strMsg("");
    int ret_val = ETHERR_OK;
    DWORD dwTestIp = IP_NULL, dwTemp = IP_NULL;
    NETTEST_TYPE_E eTestType = (NETTEST_TYPE_E)wParam;

    switch( eTestType )
    {
    case TYPE_TEST_CONN:
        strMsg = _T("ERROR: Network connection check failed!");
        TestEnd(strMsg, TRUE);
        break;

    // IP test time-out, means IP is available
    case TYPE_TEST_IP:
        dwTemp = m_dwTestIp;
        strMsg.Format(_T("IP %u.%u.%u.%u is available!"), PUSH_IPADDR(m_dwTestIp));
        TestEnd(strMsg, FALSE);
        
        // Add result to combo list
        strMsg.Format(_T("%u.%u.%u.%u"), PUSH_IPADDR(m_dwTestIp));
        AddStringToResultList(strMsg);
        //m_comboResult.AddString(strMsg);
        //m_comboResult.SetCurSel(m_comboResult.GetCount()-1);
        m_comboResult.EnableWindow(TRUE);

        // get next test IP
        if( !GetNextTestIp( &dwTestIp ) )
        {
            TRACE("IP searching is END!\n");
            strMsg.Format(_T("IP searching is END!"));
            TestEnd(strMsg, TRUE);
        }
        else
        {
            // jump out message tips
            strMsg.Format(_T("Found available IP %u.%u.%u.%u!\nContinue to search more? "), PUSH_IPADDR(dwTemp));
            if( IDYES == MessageBox(strMsg, _T("Searching Tip"), MB_YESNO|MB_ICONINFORMATION) )
            {
                strMsg.Format(_T("start check IP %u.%u.%u.%u ..."), PUSH_IPADDR(dwTestIp));
                TestEnd(strMsg, FALSE);
                StartIpTest(dwTestIp);
            }
            else
            {
                strMsg.Format(_T("IP searching is closed!"));
                TestEnd(strMsg, TRUE);
            }
        }
        break;

    default:
        strMsg.Format(_T("ERROR: Test time-out! Unknown test type %d!"), eTestType);
        TestEnd(strMsg, TRUE);
        break;
    }
    return 0;
}

void CIpSearchDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
    {
    case SEARCH_ENDING_TIMER_ID:
        m_btnNext.EnableWindow(TRUE);
        // check ethernet receive task is running
        if( !NETTEST_IsEthernetTaskRun() )
        {
            KillTimer(SEARCH_ENDING_TIMER_ID);
        }
        else
        {
            TRACE("debug: CIpSearchDlg::OnTimer - wait to check if test task end!\n");
        }
        break;

    default:
        break;
    }

	CDialog::OnTimer(nIDEvent);
}

void CIpSearchDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// get available IP
    CString strMsg("");
    DWORD dwIp = IP_NULL;

    m_comboResult.GetWindowText(strMsg);
    dwIp = inet_addr(strMsg.GetBuffer(strMsg.GetLength()));
    m_dwIpAvail = ntohl(dwIp);	
}
