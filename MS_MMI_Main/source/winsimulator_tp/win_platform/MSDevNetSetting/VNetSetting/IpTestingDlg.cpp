// IpTestingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VNetSetting.h"
#include "net_test.h"
#include "msnet_param.h"
#include <WINSOCK2.h>
#include "IpTestingDlg.h"

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
// CIpTestingDlg dialog


CIpTestingDlg::CIpTestingDlg(
    IPTESTING_INPUT_PARAM_T* pParam /*=NULL*/,
    CWnd* pParent /*=NULL*/)
	: CDialog(CIpTestingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIpTestingDlg)
	//}}AFX_DATA_INIT
    memset(&m_input, 0, sizeof(m_input));
    if( NULL != pParam )
    {
        memcpy(&m_input, pParam, sizeof(IPTESTING_INPUT_PARAM_T));
    }
    m_bIpUsable = FALSE;
    m_bInConnTest = m_bInIpTest = FALSE;
}

CIpTestingDlg::~CIpTestingDlg()
{
    NETTEST_CloseEthernetTask();
}

void CIpTestingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIpTestingDlg)
	DDX_Control(pDX, IDC_STATIC_NOTES, m_staticNotes);
	DDX_Control(pDX, IDOK, m_btnOk);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIpTestingDlg, CDialog)
	//{{AFX_MSG_MAP(CIpTestingDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
    // 自定义消息响应
    ON_MESSAGE( WM_ETHERNET_RECV_PACKET,  OnEthernetRecvPacket  )
    ON_MESSAGE( WM_ETHERNET_RECV_TIMEOUT, OnEthernetRecvTimeout )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIpTestingDlg none-message handlers

/***************************************************************************/
// Descrption : network connection test
// Global resource : 
// Author: yifeng.wang 
// Note: 
/***************************************************************************/
int CIpTestingDlg::StartConnTest( void )
{
    // ARP request parameters
    BYTE  ucMacSrc[ETHER_MAC_LEN];
    DWORD dwIPSrc(0), dwIPDst(0);

    // check network connection status
    if( !MSNET_IsNetworkConnected() )
    {
        TRACE("CIpTestingDlg::StartConnTest - MSNET_IsNetworkConnected() return FALSE!\n");
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
int CIpTestingDlg::StartIpTest( void )
{
    // ARP request parameters
    BYTE  ucMacSrc[ETHER_MAC_LEN];
    DWORD dwIPSrc(0), dwIPDst(0);

    // set parameters
    memcpy(ucMacSrc, m_input.szMacAddr, ETHER_MAC_LEN);
    dwIPSrc = htonl( m_input.dwLocalIp );
    dwIPDst = htonl( m_input.dwTestIp );

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
void CIpTestingDlg::TestStart( void )
{
    CString strMsg("");
    int ret_val = 0;

    m_bInIpTest = m_bInConnTest = FALSE;

    // create ethernet task
    if( !NETTEST_CreateEthernetTask(this->GetSafeHwnd(), m_input.szDeviceName) )
    {
        ret_val = NETTEST_GetEthernetTaskError();
        TRACE("CIpTestingDlg::TestStart() - NETTEST_CreateEthernetTask() error %d!\n", ret_val);
        strMsg.Format("IP test initialize error: %d!", ret_val);
        TestEnd(strMsg);
        return;
    }

    // check if in IP testing
    if( m_bInConnTest )
    {
        strMsg.Format(_T("ERROR: Already in IP test!"));
        TestEnd(strMsg);
        return;
    }

    // check ethernet receive task is running
    if( !NETTEST_IsEthernetTaskRun() )
    {
        TestEnd( NETTEST_GetLastErrorMsg() );
        return;
    }

    // start network connection test first, when this test succeed, then start IP test
    //TRACE("yifengdebug: to call StartConnTest()\n");
    ret_val = StartConnTest();
    if( ETHERR_OK == ret_val )
    {
        m_bInConnTest = TRUE;
        m_staticNotes.SetWindowText(_T("Check network status ..."));
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
void CIpTestingDlg::TestEnd( CString strMsg )
{
    // refresh notes
    //TRACE("yifeng debug: TestEnd() - %s\n", strMsg.GetBuffer(strMsg.GetLength()));
    m_staticNotes.SetWindowText(strMsg);

    // start timer to close the dialog
    SetTimer(END_WAITING_TIMER_ID, END_WAITING_TIME_LEN, 0);
}

/***************************************************************************/
// Descrption : get test result
// Global resource : 
// Author: yifeng.wang 
// Note: 
/***************************************************************************/
BOOL CIpTestingDlg::IsTestIpUsable( void )
{
    return m_bIpUsable;
}

/////////////////////////////////////////////////////////////////////////////
// CIpTestingDlg message handlers

BOOL CIpTestingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    CString str;
    str.Format(_T("Testing %u.%u.%u.%u ..."), PUSH_IPADDR(m_input.dwTestIp));
    SetWindowText(str);

    m_staticNotes.SetWindowText(_T("IP test is to start ..."));
    SetTimer(START_WAITING_TIMER_ID, START_WAITING_TIME_LEN, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/***************************************************************************/
// Descrption : 以太网接收应答成功的消息响应
// Global resource : 
// Author: yifeng.wang 
// Note: 
/***************************************************************************/
LONG CIpTestingDlg::OnEthernetRecvPacket( WPARAM wParam, LPARAM lParam )
{
    CString strMsg("");
    int ret_val = ETHERR_OK;
    NETTEST_TYPE_E eTestType = (NETTEST_TYPE_E)wParam;

    switch( eTestType )
    {
    // connection test result, then go on to next step
    case TYPE_TEST_CONN:
        if( !m_bInIpTest )
        {
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
            TestEnd( CString(_T("ERROR: already in IP test!")) );
        }
        break;

    // IP test result, means IP is in-use
    case TYPE_TEST_IP:
        strMsg.Format(_T("IP %u.%u.%u.%u is being used by other device!"), PUSH_IPADDR(m_input.dwTestIp));
        TestEnd(strMsg);
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
LONG CIpTestingDlg::OnEthernetRecvTimeout( WPARAM wParam, LPARAM lParam )
{
    CString strMsg("");
    int ret_val = ETHERR_OK;
    NETTEST_TYPE_E eTestType = (NETTEST_TYPE_E)wParam;

    m_bInIpTest = m_bInConnTest = FALSE;

    switch( eTestType )
    {
    case TYPE_TEST_CONN:
        strMsg = _T("ERROR: Network is not connected!");
        break;

    // IP test result, means IP is not in-use
    case TYPE_TEST_IP:
        // 注意：向待测虚拟IP发送ARP请求，若没有应答（超时），认为该 IP 没有被可用
        m_bIpUsable = TRUE;
        strMsg.Format(_T("IP %u.%u.%u.%u is available!"), PUSH_IPADDR(m_input.dwTestIp));
        break;

    default:
        strMsg.Format(_T("IP test time-out! Unknown test type %d!"), eTestType);
        break;
    }

    TestEnd(strMsg);
    return 0;
}

void CIpTestingDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
    {
    case START_WAITING_TIMER_ID:
        TestStart();
        KillTimer(START_WAITING_TIMER_ID);
        break;

    case END_WAITING_TIMER_ID:
        //TRACE("debug: CIpTestingDlg::OnTimer - post WM_CLOSE to itself ...\n");
        KillTimer(END_WAITING_TIMER_ID);
        ::PostMessage(this->GetSafeHwnd(), WM_CLOSE, 0, 0);
        break;

    default:
        TRACE("Unknown Timer ID - %d\n", nIDEvent);
        break;
    }

	CDialog::OnTimer(nIDEvent);
}
