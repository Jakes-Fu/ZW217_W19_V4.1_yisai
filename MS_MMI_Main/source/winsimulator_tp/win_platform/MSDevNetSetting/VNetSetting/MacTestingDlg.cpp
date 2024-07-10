// MacTestingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VNetSetting.h"

#include <WINSOCK2.H>
#include "msnet_param.h"
#include "net_test.h"
#include "MacTestingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define START_WAITING_TIMER_ID      0x01
#define START_WAITING_TIME_LEN      100
   
#define END_WAITING_TIMER_ID        0x02
#define END_WAITING_TIME_LEN        3000


/////////////////////////////////////////////////////////////////////////////
// CMacTestingDlg dialog


CMacTestingDlg::CMacTestingDlg(
    MACTESTING_INPUT_PARAM_T* pParam,                         
    CWnd* pParent /*=NULL*/)
	: CDialog(CMacTestingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMacTestingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    if( NULL != pParam )
    {
        memcpy(&m_input, pParam, sizeof(m_input));
        m_dwAvailIp = m_input.dwAvailIp;
    }
    else
    {
        memset(&m_input, 0, sizeof(m_input));
        m_dwAvailIp = IP_NULL;
    }
    m_bMacUsable = FALSE;
    m_bInConnTest = m_bInMacTest = m_bInPingTest = m_bInIpTest = FALSE;
    m_nIpCounter = 0;
}

CMacTestingDlg::~CMacTestingDlg()
{
    NETTEST_CloseEthernetTask();
}


void CMacTestingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMacTestingDlg)
	DDX_Control(pDX, IDC_STATIC_NOTES, m_staticNotes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMacTestingDlg, CDialog)
	//{{AFX_MSG_MAP(CMacTestingDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
    ON_MESSAGE( WM_ETHERNET_RECV_PACKET,  OnEthernetRecvPacket  )
    ON_MESSAGE( WM_ETHERNET_RECV_TIMEOUT, OnEthernetRecvTimeout )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMacTestingDlg none-message handlers

/***************************************************************************/
// Descrption : network connection test
// Global resource : 
// Author: yifeng.wang 
// Note: 
/***************************************************************************/
int CMacTestingDlg::StartConnTest( void )
{
    // ARP request parameters
    BYTE  ucMacSrc[ETHER_MAC_LEN];
    DWORD dwIPSrc(0), dwIPDst(0);

    // check network connection status
    if( !MSNET_IsNetworkConnected() )
    {
        TRACE("CMacTestingDlg::StartConnTest - MSNET_IsNetworkConnected() return FALSE!\n");
        ::PostMessage(GetSafeHwnd(), WM_ETHERNET_RECV_TIMEOUT, (WPARAM)(TYPE_TEST_CONN), 0);
    }

    // set parameters
    memcpy(ucMacSrc, m_input.szMacHost, ETHER_MAC_LEN);
    dwIPSrc = htonl( m_input.dwLocalIp );
    dwIPDst = htonl( m_input.dwGateway );

    // set test time
    NETTEST_SetTestTime(GetTickCount(), NET_CONN_TEST_TIME_OUT);

    // send ARP request packet
    return NETTEST_SendNetTestData(TYPE_TEST_CONN, ucMacSrc, dwIPSrc, dwIPDst);
}

/***************************************************************************/
// Descrption : MAC test
// Global resource : 
// Author: yifeng.wang 
// Note: 
/***************************************************************************/
int CMacTestingDlg::StartMacTest( void )
{
    if( IP_NULL == m_dwAvailIp )
    {
        return ETHERR_NO_AVAIL_IP;
    }
    m_input.dwAvailIp = m_dwAvailIp;

    // check network connection status
    if( !MSNET_IsNetworkConnected() )
    {
        TRACE("CMacTestingDlg::StartMacTest - MSNET_IsNetworkConnected() return FALSE!\n");
        ::PostMessage(GetSafeHwnd(), WM_ETHERNET_RECV_TIMEOUT, (WPARAM)(TYPE_TEST_CONN), 0);
		//return ETHERR_OK;
    }

    // parameters
    BYTE  ucMacSrc[ETHER_MAC_LEN];
    DWORD dwIPSrc(0), dwIPDst(0);

    // set MAC test parameters
    memcpy(ucMacSrc, m_input.szMacTest, ETHER_MAC_LEN);
    dwIPSrc = htonl( m_input.dwAvailIp );
    dwIPDst = htonl( m_input.dwGateway );

    // set peer MAC to get its IP
    NETTEST_SetPeerIp( m_input.dwGateway );

    // set test time
    NETTEST_SetTestTime(GetTickCount(), NET_MAC_TEST_TIME_OUT);

    // send ARP request packet
    return NETTEST_SendNetTestData(TYPE_TEST_MAC, ucMacSrc, dwIPSrc, dwIPDst);
}

/***************************************************************************/
// Descrption : ping echo test
// Global resource : 
// Author: yifeng.wang 
// Note: 
/***************************************************************************/
int CMacTestingDlg::StartPingTest( void )
{
    BYTE  ucMacSrc[ETHER_MAC_LEN], ucMacDst[ETHER_MAC_LEN];
    DWORD dwIPSrc(0), dwIPDst(0);

    // check network connection status
    if( !MSNET_IsNetworkConnected() )
    {
        TRACE("CMacTestingDlg::StartPingTest - MSNET_IsNetworkConnected() return FALSE!\n");
        ::PostMessage(GetSafeHwnd(), WM_ETHERNET_RECV_TIMEOUT, (WPARAM)(TYPE_TEST_CONN), 0);
        //return ETHERR_OK;
    }

    // set parameters
    if( NULL != NETTEST_GetPeerMac(m_input.dwGateway) )
    {
        memcpy(ucMacDst, NETTEST_GetPeerMac(m_input.dwGateway), ETHER_MAC_LEN);
    }
    else
    {
        return ETHERR_INVALID_PEER_MAC;
    }

    memcpy(ucMacSrc, m_input.szMacTest, ETHER_MAC_LEN);
    dwIPSrc = htonl( m_input.dwAvailIp );
    dwIPDst = htonl( m_input.dwGateway );

    // set test time
    NETTEST_SetTestTime(GetTickCount(), NET_PING_TEST_TIME_OUT);

    // send ping packet
    return NETTEST_SendPingTestData(TYPE_TEST_PING, ucMacSrc, ucMacDst, dwIPSrc, dwIPDst);
}

/***************************************************************************/
// Descrption : IP test
// Global resource : 
// Author: yifeng.wang 
// Note: 
/***************************************************************************/
int CMacTestingDlg::StartIpTest( void )
{
    // check network connection status
    if( !MSNET_IsNetworkConnected() )
    {
        TRACE("CMacTestingDlg::StartIpTest - MSNET_IsNetworkConnected() return FALSE!\n");
        ::PostMessage(GetSafeHwnd(), WM_ETHERNET_RECV_TIMEOUT, (WPARAM)(TYPE_TEST_CONN), 0);
        //return ETHERR_OK;
    }

    // find a test IP
    DWORD dwTestMin = (0 != (m_input.dwLocalIp &  m_input.dwSnmask))? (m_input.dwLocalIp &  m_input.dwSnmask) + 1 : 0;
    DWORD dwTestMax = (0 != (m_input.dwLocalIp | ~m_input.dwSnmask))? (m_input.dwLocalIp | ~m_input.dwSnmask) - 1 : 0;
    DWORD dwTestRange = (dwTestMax >= dwTestMin)? (dwTestMax-dwTestMin+1) : 1;
    do
    {
        m_dwAvailIp = dwTestMin + rand()%dwTestRange;
        // check IP test counter value, avoid none-end loop
        if( ++m_nIpCounter > dwTestRange )
        {
            return ETHERR_NO_AVAIL_IP;
        }
    }
    while( m_input.dwLocalIp == m_dwAvailIp || m_input.dwGateway == m_dwAvailIp );

    // ARP request parameters
    BYTE  ucMacSrc[ETHER_MAC_LEN];
    DWORD dwIPSrc(0), dwIPDst(0);

    // set parameters
    memcpy(ucMacSrc, m_input.szMacHost, ETHER_MAC_LEN);
    dwIPSrc = htonl( m_input.dwLocalIp );
    dwIPDst = htonl( m_dwAvailIp );

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
void CMacTestingDlg::TestStart( void )
{
    CString strMsg("");
    int ret_val = 0;

    // reset test flag
    m_bInConnTest = m_bInMacTest = m_bInPingTest = m_bInIpTest = FALSE;

    // create ethernet task
    if( !NETTEST_CreateEthernetTask(this->GetSafeHwnd(), m_input.szDeviceName) )
    {
        ret_val = NETTEST_GetEthernetTaskError();
        TRACE("CMacTestingDlg::TestStart() - NETTEST_CreateEthernetTask() error %d!\n", ret_val);
        strMsg.Format("MAC test initialize error: %d!", ret_val);
        TestEnd(strMsg);
        return;
    }

    // check if in IP testing
    if( m_bInConnTest )
    {
        strMsg.Format(_T("ERROR: Already in MAC test!"));
        TestEnd(strMsg);
        return;
    }

    // check ethernet receive task is running
    if( !NETTEST_IsEthernetTaskRun() )
    {
        TestEnd( NETTEST_GetLastErrorMsg() );
        return;
    }

    // start network connection test first, when this test succeed, then start MAC test
	m_staticNotes.SetWindowText(_T("Check network status ..."));
    ret_val = StartConnTest();
    if( ETHERR_OK == ret_val )
    {
        m_bInConnTest = TRUE;
    }
    else
    {
        TestEnd(NETTEST_GetErrorMsg(ret_val));
    }
}

/***************************************************************************/
// Descrption : end test
// Global resource : 
// Author: yifeng.wang 
// Note: 
/***************************************************************************/
void CMacTestingDlg::TestEnd( CString strMsg )
{
    m_bInMacTest = m_bInConnTest = FALSE;

    // refresh notes
    //TRACE("yifeng debug: TestEnd() - %s\n", strMsg.GetBuffer(strMsg.GetLength()));
    m_staticNotes.SetWindowText(strMsg);

    // start timer to close the dialog
    SetTimer(END_WAITING_TIMER_ID, END_WAITING_TIME_LEN, 0);
}

/***************************************************************************/
// Descrption : get MAC test result
// Global resource : 
// Author: yifeng.wang 
// Note: 
/***************************************************************************/
BOOL CMacTestingDlg::IsTestMacUsable( void )
{
    return m_bMacUsable;
}


/////////////////////////////////////////////////////////////////////////////
// CMacTestingDlg message handlers

BOOL CMacTestingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    CString str;
    str.Format(_T("Testing %02X:%02X:%02X:%02X:%02X:%02X ..."), 
        m_input.szMacTest[0], m_input.szMacTest[1], m_input.szMacTest[2],
        m_input.szMacTest[3], m_input.szMacTest[4], m_input.szMacTest[5]);
    SetWindowText(str);

    m_staticNotes.SetWindowText(_T("MAC test is to start ..."));
    SetTimer(START_WAITING_TIMER_ID, START_WAITING_TIME_LEN, 0);

    // update random seed
    srand(GetTickCount());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/***************************************************************************/
// Descrption : 以太网接收应答成功的消息响应
// Global resource : 
// Author: yifeng.wang 
// Note: 
/***************************************************************************/
LONG CMacTestingDlg::OnEthernetRecvPacket( WPARAM wParam, LPARAM lParam )
{
    CString strMsg("");
    int ret_val = ETHERR_OK;
    NETTEST_TYPE_E eTestType = (NETTEST_TYPE_E)wParam;

    switch( eTestType )
    {
    // connection test result, then go to next step
    case TYPE_TEST_CONN:
        if( IP_NULL == m_dwAvailIp )
        {
			m_staticNotes.SetWindowText(_T("Searching available IP ..."));
            ret_val = StartIpTest();
            if( ETHERR_OK == ret_val )
            {
                m_bInIpTest = TRUE;
            }
            else
            {
                TestEnd( NETTEST_GetErrorMsg(ret_val) );
            }
        }
        else
        {
			m_staticNotes.SetWindowText(_T("Start MAC test ..."));
            ret_val = StartMacTest();
            if( ETHERR_OK == ret_val )
            {
                m_bInMacTest = TRUE;
            }
            else
            {
                TestEnd( NETTEST_GetErrorMsg(ret_val) );
            }
        }
        break;

    // MAC test result, means MAC test through ARP is OK
    case TYPE_TEST_MAC:
		m_staticNotes.SetWindowText(_T("Start PING test ..."));
        ret_val = StartPingTest();
        if( ETHERR_OK == ret_val )
        {
            m_bInPingTest = TRUE;
        }
        else
        {
            TestEnd( NETTEST_GetErrorMsg(ret_val) );
        }
        break;

    // PING test result, means MAC test through PING is OK
    case TYPE_TEST_PING:
        m_bMacUsable = TRUE;
        strMsg.Format(_T("MAC %02X:%02X:%02X:%02X:%02X:%02X is available!"),
            m_input.szMacTest[0], m_input.szMacTest[1], m_input.szMacTest[2],
            m_input.szMacTest[3], m_input.szMacTest[4], m_input.szMacTest[5]);
        TestEnd(strMsg);
        break;

    // IP test result, means IP is in-use by other device
    case TYPE_TEST_IP:
        ret_val = StartIpTest();
        if( ETHERR_OK == ret_val )
        {
            m_bInIpTest = TRUE;
        }
        else
        {
            TestEnd( NETTEST_GetErrorMsg(ret_val) );
        }
        break;

    default:
        strMsg.Format(_T("ERROR: wrong test type - %d"), eTestType);
        TestEnd(strMsg);
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
LONG CMacTestingDlg::OnEthernetRecvTimeout( WPARAM wParam, LPARAM lParam )
{
    CString strMsg("");
    int ret_val = ETHERR_OK;
    NETTEST_TYPE_E eTestType = (NETTEST_TYPE_E)wParam;

    switch( eTestType )
    {
    // CONNECTION test result, no-response means network is not connected
    case TYPE_TEST_CONN:
        strMsg = _T("ERROR: Network may not connected!");
        break;

    // MAC test result, no-response means MAC may be forbidden
    case TYPE_TEST_MAC:
        strMsg.Format(_T("%s"), "MAC test time-out! MAC may be unusable!");
        break;

    // PING test result, no-response means MAC with PING may be forbidden
    case TYPE_TEST_PING:
        strMsg.Format(_T("%s"), "MAC ping test time-out! MAC may be unusable!");
        break;

    // IP test result, no-response means IP is not in-use
    case TYPE_TEST_IP:
        ret_val = StartMacTest();
        if( ETHERR_OK == ret_val )
        {
            m_bInMacTest = TRUE;
            m_staticNotes.SetWindowText(_T("Start MAC test ..."));
        }
        else
        {
            TestEnd( NETTEST_GetErrorMsg(ret_val) );
        }
        break;

    default:
        strMsg.Format(_T("ERROR: MAC test time-out! Unknown test type %d!"), eTestType);
        break;
    }
    TestEnd(strMsg);
    return 0;
}

void CMacTestingDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
    {
    case START_WAITING_TIMER_ID:
        TestStart();
        KillTimer(START_WAITING_TIMER_ID);
        break;

    case END_WAITING_TIMER_ID:
        //TRACE("debug: CMacTestingDlg::OnTimer - post WM_CLOSE to itself ...\n");
        KillTimer(END_WAITING_TIMER_ID);
        ::PostMessage(this->GetSafeHwnd(), WM_CLOSE, 0, 0);
        break;

    default:
        TRACE("Unknown Timer ID - %d\n", nIDEvent);
        break;
    }

	CDialog::OnTimer(nIDEvent);
}
