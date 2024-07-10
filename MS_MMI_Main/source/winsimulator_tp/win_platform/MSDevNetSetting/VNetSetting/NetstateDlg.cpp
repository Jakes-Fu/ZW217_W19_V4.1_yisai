// NetstateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VNetSetting.h"
#include "NetstateDlg.h"

#include <WINSOCK2.H>
#include "msdev_param.h"
#include "net_driver.h"
#include "file_path.h"
#include "CloseWaitDlg.h"
#include "NetsettingDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*---------------------------------------------------------------------------*
 *                         MACRO DEFINITION                                  *
 *---------------------------------------------------------------------------*/
#define FILE_NAME               "NetstateDlg.cpp"
#define STRING_UNKNOWN          "( UNKNOWN )"

#define PARAM_LOAD_TIMER_ID     0x01
#define PARAM_LOAD_TIME_LEN     1500

#define COLOR_DEFAULT           RGB(0, 0, 0)
#define COLOR_NORMAL            RGB(0, 128, 0)
#define COLOR_ABNORMAL          RGB(192, 0, 0)
#define COLOR_NOTDETERMINED     RGB(0, 0, 192)


/////////////////////////////////////////////////////////////////////////////
// CNetstateDlg property page

IMPLEMENT_DYNCREATE(CNetstateDlg, CPropertyPage)

CNetstateDlg::CNetstateDlg() : CPropertyPage(CNetstateDlg::IDD)
{
	//{{AFX_DATA_INIT(CNetstateDlg)
	//}}AFX_DATA_INIT
    memset(&m_netParam, 0, sizeof(MSDEV_NETPARAM_T));
    MSDEVPARAM_LoadFromFile();
    m_bDrvInstall = FALSE;
    m_colorNetid = m_colorNetState = COLOR_DEFAULT;
}

CNetstateDlg::~CNetstateDlg()
{
}

void CNetstateDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetstateDlg)
	DDX_Control(pDX, IDC_EDIT_DNS2, m_editDns2);
	DDX_Control(pDX, IDC_EDIT_DNS1, m_editDns1);
	DDX_Control(pDX, IDC_CHECK_SEND, m_chkSend);
	DDX_Control(pDX, IDC_CHECK_RECV, m_chkRecv);
	DDX_Control(pDX, IDC_EDIT_CFGFILE, m_editCfgFile);
	DDX_Control(pDX, IDC_EDIT_NETID, m_editNetid);
	DDX_Control(pDX, IDC_EDIT_SNMASK, m_editSnmask);
	DDX_Control(pDX, IDC_EDIT_DEVICE_DESC, m_editDevDesc);
	DDX_Control(pDX, IDC_EDIT_DEVICE_NAME, m_editDevName);
	DDX_Control(pDX, IDC_EDIT_DEVICE_STATE, m_editDevState);
	DDX_Control(pDX, IDC_EDIT_GATEWAY, m_editGateway);
	DDX_Control(pDX, IDC_EDIT_IP, m_editIp);
	DDX_Control(pDX, IDC_EDIT_MAC, m_editMac);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetstateDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CNetstateDlg)
	ON_BN_CLICKED(IDC_BTN_SETTING, OnBtnSetting)
	ON_BN_CLICKED(IDC_BTN_DRV_INSTALL, OnBtnDrvInstall)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_SEND, OnCheckSend)
	ON_BN_CLICKED(IDC_CHECK_RECV, OnCheckRecv)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetstateDlg none-message handlers

/*****************************************************************************/
// Func Descrption : 参数显示更新
// Global resource : none
// Note : 
/*****************************************************************************/
void CNetstateDlg::ParamRefresh( const MSDEV_NETPARAM_T* pParam )
{
    CString str;
    const uint8* mac_ptr = NULL;

    // get net parameters
    if( NULL == pParam )
    {
        TRACE("%s(%d) : ParamRefresh() - MSDEVPARAM_GetParam return NULL\n", FILE_NAME, __LINE__);
        return;
    }

    // record net parameters
    memcpy(&m_netParam, pParam, sizeof(MSDEV_NETPARAM_T));

    // show net parameters
    mac_ptr = m_netParam.szMacAddr;
    str.Format("%02X:%02X:%02X:%02X:%02X:%02X",
        mac_ptr[0], mac_ptr[1], mac_ptr[2], mac_ptr[3], mac_ptr[4], mac_ptr[5]);
    m_editMac.SetWindowText(str);
    m_colorMac = IsMacMormal((uint8*)(m_netParam.szMacAddr), ETHER_MAC_LEN)? COLOR_NORMAL : COLOR_ABNORMAL;

    if( m_netParam.bUseDhcp )
    {
        str.Format("( USE DHCP )");
        m_editIp.SetWindowText(str);
        m_colorIp = COLOR_NORMAL;
    }
    else
    {
        str.Format(_T("%u.%u.%u.%u"), PUSH_IPADDR(m_netParam.uIpAddr));
        m_editIp.SetWindowText(str);
        m_colorIp = IsIpNormal(m_netParam.uIpAddr)? COLOR_NORMAL : COLOR_ABNORMAL;
    }

    str.Format(_T("%u.%u.%u.%u"), PUSH_IPADDR(m_netParam.uSnmask));
    m_editSnmask.SetWindowText(str);
    m_colorSnmask = IsSnmaskNormal(
        m_netParam.uSnmask, (m_netParam.bUseDhcp)? IP_BROADCAST:m_netParam.uIpAddr)? COLOR_NORMAL : COLOR_ABNORMAL;

    str.Format(_T("%u.%u.%u.%u"), PUSH_IPADDR(m_netParam.uGateway));
    m_editGateway.SetWindowText(str);
    m_colorGateway = IsIpNormal(m_netParam.uGateway)? COLOR_NORMAL : COLOR_ABNORMAL;

    str.Format(_T("%u.%u.%u.%u"), PUSH_IPADDR(m_netParam.uDns1));
    m_editDns1.SetWindowText(str);
    m_colorDns1 = IsIpNormal(m_netParam.uDns1)? COLOR_NORMAL : COLOR_ABNORMAL;

    str.Format(_T("%u.%u.%u.%u"), PUSH_IPADDR(m_netParam.uDns2));
    m_editDns2.ShowWindow((IP_NULL!=m_netParam.uDns2)? TRUE:FALSE);
    m_editDns2.SetWindowText(str);
    m_colorDns2 = IsIpNormal(m_netParam.uDns2)? COLOR_NORMAL : COLOR_ABNORMAL;

    str.Format("%s", m_netParam.szDeviceDesc);
    m_editDevDesc.SetWindowText(str);
    str.Format("%s", m_netParam.szDeviceName);
    m_editDevName.SetWindowText(str);

    // show configuration file path
    MSDEVPARAM_GetFilePath(str);
    m_editCfgFile.SetWindowText(str);
    m_colorCfgFile = FILEPATH_IsExisted(str)? COLOR_DEFAULT : COLOR_ABNORMAL;
}

/*****************************************************************************/
// Func Descrption : 更新控制面板状态（从状态文件读取信息）
// Global resource : none
// Note : 
/*****************************************************************************/
void CNetstateDlg::GetAndUpdateNetState( void )
{
    const MSDEV_NETSTATE_T* pState = MSDEVPARAM_GetState();
    if( NULL != pState )
    {
        CString str;

        // show MAC
        const BYTE* mac_ptr = pState->szMacAddr;
        str.Format("%02X:%02X:%02X:%02X:%02X:%02X",
            mac_ptr[0], mac_ptr[1], mac_ptr[2], mac_ptr[3], mac_ptr[4], mac_ptr[5]);
        m_editMac.SetWindowText(str);
        m_colorMac = IsMacMormal((uint8*)(pState->szMacAddr), ETHER_MAC_LEN)? COLOR_NORMAL : COLOR_ABNORMAL;

        // show IPs
        if( STATE_TRUE == pState->iFlagDhcp )
        {
            str.Format(_T("%u.%u.%u.%u (from DHCP)"), PUSH_IPADDR(pState->uIpAddr));
        }
        else
        {
            str.Format(_T("%u.%u.%u.%u"), PUSH_IPADDR(pState->uIpAddr));
        }
        m_editIp.SetWindowText(str);
        m_colorIp = IsIpNormal(pState->uIpAddr)? COLOR_NORMAL : COLOR_ABNORMAL;

        str.Format(_T("%u.%u.%u.%u"), PUSH_IPADDR(pState->uSnmask));
        m_editSnmask.SetWindowText(str);
        m_colorSnmask = IsSnmaskNormal(pState->uSnmask, pState->uIpAddr)? COLOR_NORMAL : COLOR_ABNORMAL;

        str.Format(_T("%u.%u.%u.%u"), PUSH_IPADDR(pState->uGateway));
        m_editGateway.SetWindowText(str);
        m_colorGateway = IsIpNormal(pState->uGateway)? COLOR_NORMAL : COLOR_ABNORMAL;

        str.Format(_T("%u.%u.%u.%u"), PUSH_IPADDR(pState->uDns1));
        m_editDns1.SetWindowText(str);
        m_colorDns1 = IsIpNormal(pState->uDns1)? COLOR_NORMAL : COLOR_ABNORMAL;

        str.Format(_T("%u.%u.%u.%u"), PUSH_IPADDR(pState->uDns2));
        m_editDns2.ShowWindow((IP_NULL!=pState->uDns2)? TRUE:FALSE);
        m_editDns2.SetWindowText(str);
        m_colorDns2 = IsIpNormal(pState->uDns2)? COLOR_NORMAL : COLOR_ABNORMAL;

        str.Format("%s", pState->szDeviceDesc);
        m_editDevDesc.SetWindowText(str);
        str.Format("%s", pState->szDeviceName);
        m_editDevName.SetWindowText(str);

        // show NETID
        str.Format("0x%x", pState->uNetid);
        m_editNetid.SetWindowText(str);
        m_colorNetid = (0 == pState->uNetid)? COLOR_ABNORMAL : COLOR_DEFAULT;

        // show ethernet device state
        switch( pState->iNetState )
        {
        case STATE_NETOPEN:
            m_editDevState.SetWindowText(_T("OPEN"));
            m_colorNetState = COLOR_NORMAL;
            break;

        case STATE_NETERROR:
            m_editDevState.SetWindowText(_T("NOT OPEN"));
            m_colorNetState = COLOR_ABNORMAL;
            break;

        default:
            m_editDevState.SetWindowText(_T("UNKNOWN"));
            m_colorNetState = COLOR_NOTDETERMINED;
            break;
        }

        // show send state
        switch( pState->iFlagSend )
        {
        case STATE_TRUE:
        case STATE_FALSE:
            m_chkSend.SetCheck(pState->iFlagSend);
            //m_chkSend.EnableWindow(TRUE);
            break;

        default:
            m_chkSend.SetCheck(0);
            m_chkSend.EnableWindow(FALSE);
            break;
        }

        // show recv state
        switch( pState->iFlagRecv )
        {
        case STATE_TRUE:
        case STATE_FALSE:
            m_chkRecv.SetCheck(pState->iFlagRecv);
            //m_chkRecv.EnableWindow(TRUE);
            break;

        default:
            m_chkRecv.SetCheck(0);
            m_chkRecv.EnableWindow(FALSE);
            break;
        }
    }

//    // show configuration file path
//    MSDEVPARAM_GetFilePath(str);
//    m_editCfgFile.SetWindowText(str);
//    m_colorCfgFile = FILEPATH_IsExisted(str)? COLOR_NORMAL : COLOR_ABNORMAL;
}

/*****************************************************************************/
// Func Descrption : 判断 IP 地址是否异常
// Global resource : none
// Note : 
/*****************************************************************************/
BOOL CNetstateDlg::IsIpNormal( DWORD dwIp )
{
    if( IP_NULL == dwIp )
    {
        return FALSE;
    }

    if( (dwIp & IP_MASK_A) == IP_ADDR_A ||
        (dwIp & IP_MASK_B) == IP_ADDR_B ||
        (dwIp & IP_MASK_C) == IP_ADDR_C )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// Func Descrption : 判断 subnet mask 地址是否异常
// Global resource : none
// Note : 
/*****************************************************************************/
BOOL CNetstateDlg::IsSnmaskNormal( DWORD dwSnmask, DWORD dwIp )
{
    if( IP_NULL == dwIp || IP_BROADCAST == dwSnmask || IP_NULL == dwSnmask )
    {
        return FALSE;
    }

    if( ((IP_BROADCAST == dwIp || (dwIp & IP_MASK_A) == IP_ADDR_A) && (dwSnmask & IP_SNMASK_A) == IP_SNMASK_A) ||
        ((IP_BROADCAST == dwIp || (dwIp & IP_MASK_B) == IP_ADDR_B) && (dwSnmask & IP_SNMASK_B) == IP_SNMASK_B) ||
        ((IP_BROADCAST == dwIp || (dwIp & IP_MASK_C) == IP_ADDR_C) && (dwSnmask & IP_SNMASK_B) == IP_SNMASK_B) )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// Func Descrption : 判断 MAC 地址是否异常
// Global resource : none
// Note : 
/*****************************************************************************/
BOOL CNetstateDlg::IsMacMormal( const uint8* pszMac, int iLen )
{
    BYTE szMacNull[ETHER_MAC_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    if( NULL == pszMac || ETHER_MAC_LEN != iLen )
    {
        return FALSE;
    }
    return (0 == memcmp(szMacNull, pszMac, ETHER_MAC_LEN))? FALSE : TRUE;
}

/*****************************************************************************/
// Func Descrption : 保存发送标志
// Global resource : none
// Note : 
/*****************************************************************************/
void CNetstateDlg::SaveSendEnableFlag( BOOL bEnable )
{
    // to be continue ...
}

/*****************************************************************************/
// Func Descrption : 保存接收标志
// Global resource : none
// Note : 
/*****************************************************************************/
void CNetstateDlg::SaveRecvEnableFlag( BOOL bEnable )
{
    // to be continue ...
}


/////////////////////////////////////////////////////////////////////////////
// CNetstateDlg message handlers

// 对话框初始化
BOOL CNetstateDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_bDrvInstall = NETDRIVER_IsInstalled();
    InitialSTStyleButton();

	// parameter initialize
    m_editIp.SetWindowText(_T(STRING_UNKNOWN));
    m_editSnmask.SetWindowText(_T(STRING_UNKNOWN));
    m_editGateway.SetWindowText(_T(STRING_UNKNOWN));
    m_editDns1.SetWindowText(_T(STRING_UNKNOWN));
    m_editDns2.SetWindowText(_T(STRING_UNKNOWN));
	m_editNetid.SetWindowText(_T(STRING_UNKNOWN));
    m_editCfgFile.SetWindowText(_T(STRING_UNKNOWN));
    m_editDevDesc.SetWindowText(_T(STRING_UNKNOWN));
    m_editDevName.SetWindowText(_T(STRING_UNKNOWN));
    m_editDevState.SetWindowText(_T(STRING_UNKNOWN));

    // load parameter from file
    ParamRefresh( MSDEVPARAM_GetParam() );

    // init net state
    m_chkSend.SetCheck(FALSE);
    m_chkRecv.SetCheck(FALSE);
    m_chkSend.EnableWindow(FALSE);
    m_chkRecv.EnableWindow(FALSE);

    // start state check timer
    SetTimer(PARAM_LOAD_TIMER_ID, PARAM_LOAD_TIME_LEN, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// CButtonST style operations
void CNetstateDlg::InitialSTStyleButton( void )
{
    m_btnDrvInstall.SubclassDlgItem( IDC_BTN_DRV_INSTALL, this );
    m_btnDrvInstall.SetIcon( (m_bDrvInstall)? IDI_ICON_YES : IDI_ICON_NO );
    m_btnDrvInstall.SetFlat( FALSE );
}

// 按钮“Setting...”的响应
void CNetstateDlg::OnBtnSetting() 
{
    MSDEV_NETPARAM_T param;
	CNetsettingDlg dlg(&m_netParam);
    if( IDOK == dlg.DoModal() )
    {
        KillTimer(PARAM_LOAD_TIMER_ID);
        MessageBox(_T("Restart simulator to activate the new parameters!"), _T("Notes"), MB_OK|MB_ICONINFORMATION);
        dlg.GetParam(&param);
        ParamRefresh(&param);
    }
}

// 按钮“Install Driver”的响应
void CNetstateDlg::OnBtnDrvInstall() 
{
    // 检查驱动文件是否存在
    if( NETDRIVER_IsInstalled() )
    {
        if( IDNO == MessageBox(
            _T("Driver is already installed! Continue to reinstall?"),
            _T("Notes"), MB_YESNO|MB_ICONINFORMATION) )
        {
            return;
        }
    }

    // 安装驱动
    if( NETDRIVER_InstallDriver() )
    {
        //MessageBox(_T("When driver is installed, restart the simulator!"), _T("Notes"), MB_OK|MB_ICONINFORMATION);
        CCloseWaitDlg dlg;
        dlg.DoModal();

        // 关闭模拟器
        HWND handle = ::FindWindow(NULL, _T("MSDev"));
        if( NULL != handle )
        {
            ::PostMessage(handle, WM_QUIT, 0, 0);
        }    
    }
}

void CNetstateDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch( nIDEvent )
    {
    case PARAM_LOAD_TIMER_ID:
        GetAndUpdateNetState();
        break;

    default:
        break;
    }

	CPropertyPage::OnTimer(nIDEvent);
}


HBRUSH CNetstateDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if( NULL != pWnd )
    {
        switch( pWnd->GetDlgCtrlID() )
        {
        case IDC_EDIT_NETID:
            pDC->SetTextColor( m_colorNetid );
            pDC->SetBkMode( TRANSPARENT );
            break;

        case IDC_EDIT_DEVICE_STATE:
            pDC->SetTextColor( m_colorNetState );
            pDC->SetBkMode( TRANSPARENT );
            break;

        case IDC_EDIT_MAC:
            pDC->SetTextColor( m_colorMac );
            pDC->SetBkMode( TRANSPARENT );
            break;

        case IDC_EDIT_IP:
            pDC->SetTextColor( m_colorIp );
            pDC->SetBkMode( TRANSPARENT );
            break;

        case IDC_EDIT_SNMASK:
            pDC->SetTextColor( m_colorSnmask );
            pDC->SetBkMode( TRANSPARENT );
            break;

        case IDC_EDIT_GATEWAY:
            pDC->SetTextColor( m_colorGateway );
            pDC->SetBkMode( TRANSPARENT );
            break;

        case IDC_EDIT_CFGFILE:
            pDC->SetTextColor( m_colorCfgFile );
            pDC->SetBkMode( TRANSPARENT );
            break;

        case IDC_EDIT_DNS1:
            pDC->SetTextColor( m_colorDns1 );
            pDC->SetBkMode( TRANSPARENT );
            break;

        case IDC_EDIT_DNS2:
            pDC->SetTextColor( m_colorDns2 );
            pDC->SetBkMode( TRANSPARENT );
            break;
        }
    }

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CNetstateDlg::OnCheckSend() 
{
	// TODO: Add your control notification handler code here
	SaveSendEnableFlag( m_chkSend.GetCheck() );
}

void CNetstateDlg::OnCheckRecv() 
{
	// TODO: Add your control notification handler code here
	SaveSendEnableFlag( m_chkRecv.GetCheck() );
}

void CNetstateDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if( bShow )
    {
        GetAndUpdateNetState();
    }
}
