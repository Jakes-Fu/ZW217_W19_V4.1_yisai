// SetComDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "SetComDlg.h"
#include <Winsock2.h>

#include "DspKernelExport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEF_BAUD	     460800
#define DEF_PORT	     1
#define DEF_FRAMEHEAD    0xFBA5

#define DEF_AUTOGENERATE  "Auto Generate"

#define SECTION_PORT     "Serial port"
#define ENTRY_PORT       "Port"
#define ENTRY_BAUDRATE   "Baudrate"

#define SECTION_PATH     "Path"
#define ENTRY_PATH       "LogPath"

#define SECTION_FILE     "Log File"
#define ENTRY_SAVERECENT "SaveRecent"
#define ENTRY_FILESIZE	 "MaxNum"
#define ENTRY_TPNUM		 "TPNum"

#define SECTION_CHANNEL  "ChannelType"
#define ENTRY_CHANNEL    "ChannelTYpe"

#define SECTION_SOCKET  "Socket"
#define ENTRY_SOCKET_PORT "Port"
#define ENTRY_SOCKET_ADDR "IP Address"
#define ENTRY_SOCKET_ENDIAN "Endian"

#define GENERATE_TIME   "GenerateTime"
#define ENTRY_GENERATE_TIME "GenerateTime"

#define GENERATE_ORGFILE   "GenerateOrgFile"
#define ENTRY_GENERATE_ORGFILE "GenerateOrgFile"

#define SECTION_SETTINGS "Settings"
#define ENTRY_AUTOTEST   "Autotest"

/////////////////////////////////////////////////////////////////////////////
// CSetComDlg dialog


CSetComDlg::CSetComDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetComDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetComDlg)
	m_strPort = _T("");
	m_strLogPath = _T("");
	m_strLogName = _T("");
	m_nMaxNum = 5;
	m_nTPNum = 1;
	m_nSocketPort = 0;
	m_bNotFilterAddr = FALSE;
	//}}AFX_DATA_INIT
    m_nPortIndex   = DEF_PORT;
    m_nBaudIndex   = DEF_BAUD;
	m_strRate = _T("");
	m_strFrameHead  = _T("0xFBA5");
    m_bReadOnly    = FALSE;
	m_bSaveRecent  = FALSE;
	m_strLogName   = DEF_AUTOGENERATE;
	m_nChannelType = TOOL_CHANNEL_UART;
	m_dwIpAddr = 0;
	m_dwGenerateTPTime = TOOL_TIME;
	m_bGenerateOrgFile = TRUE;
	m_strIpAddr = _T("127.0.0.1");
	m_nEndian = 1;
	m_bAutotest		= FALSE;
	m_bAutoConnect	= FALSE;
	m_bLogSN		= FALSE;
	m_dwMemFileSize	=0;
	memset(m_szCfgFile,0,sizeof(m_szCfgFile));
	m_nLogType = E_COMMON_DSP;
}


void CSetComDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetComDlg)
	DDX_Control(pDX, IDC_IPADDRESS, m_ctrlIpAddr);
	DDX_Control(pDX, IDC_EDIT_LOGNAME, m_editLogName);
	DDX_Control(pDX, IDC_EDIT_LOG_PATH, m_edtPath);
	DDX_Control(pDX, IDC_COMBO_PORT, m_cmbPort);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_cmbBaudRate);
	DDX_CBString(pDX, IDC_COMBO_BAUDRATE, m_strRate);
	DDX_Text(pDX, IDC_EDIT_FRAME_HEAD,  m_strFrameHead);
	DDV_MaxChars(pDX, m_strFrameHead, 6);
	DDX_CBString(pDX, IDC_COMBO_PORT, m_strPort);
	DDX_Text(pDX, IDC_EDIT_LOG_PATH, m_strLogPath);
	DDX_Text(pDX, IDC_EDIT_LOGNAME, m_strLogName);
	DDX_Text(pDX, IDC_EDT_NUM, m_nMaxNum);
	DDX_Text(pDX, IDC_EDIT_TPNUM, m_nTPNum);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nSocketPort);
	DDX_Control(pDX, IDC_RADIO_GENERATE_TP_TIME_BY_TOOL, m_btnGenerateTPTimeByTool);
	DDX_Control(pDX, IDC_GENERATE_ORG_FILE, m_btnGenerateOrgFile);
	DDX_Control(pDX, IDC_RADIO_GENERATE_TP_TIME_BY_D0A0, m_btnGenerateTPTimeByD0A0);
	DDX_Control(pDX, IDC_RADIO_GENERATE_TP_TIME_BY_PHONE, m_btnGenerateTPTimeByPhone);
	DDX_Radio(pDX, IDC_RDO_COMMON, m_nLogType);
	DDX_Check(pDX, IDC_CHK_UNFILTER_ADDRESS, m_bNotFilterAddr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetComDlg, CDialog)
	//{{AFX_MSG_MAP(CSetComDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_RADIO_SAVEALL, OnRadioSaveAll)
	ON_BN_CLICKED(IDC_RADIO_SAVERCT, OnRadioSaveRecent)
	ON_EN_SETFOCUS(IDC_EDIT_LOGNAME, OnSetfocusEditLogname)
	ON_EN_KILLFOCUS(IDC_EDIT_LOGNAME, OnKillfocusEditLogname)
	ON_BN_CLICKED(IDC_RADIO_UART, OnRadioUart)
	ON_BN_CLICKED(IDC_RADIO_SOCKET, OnRadioSocket)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUDRATE, OnSelchangeComboBaudrate)
	ON_CBN_EDITCHANGE(IDC_COMBO_BAUDRATE, OnEditchangeComboBaudrate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetComDlg message handlers

BOOL CSetComDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    int nBaudrate = 0;
    CString	strText;
    EnumSerialPorts( GetPort , 0 );

	LoadSettings();
	EnableSocketSetting( (m_nChannelType==TOOL_CHANNEL_SOCKET_CLIENT));
	
	int nCount = m_cmbBaudRate.GetCount();
    for(int i=0;i<nCount;i++)
    {
	    m_cmbBaudRate.GetLBText(i,strText);
        nBaudrate = atoi( strText );
        if( nBaudrate == m_nBaudIndex )
        {
           m_cmbBaudRate.SetCurSel(i);
           break;
        }
    }

	m_strRate.Format("%d", m_nBaudIndex);
    
    CSpinButtonCtrl * pSpn = (CSpinButtonCtrl *)GetDlgItem(IDC_SPN_NUM);
    _ASSERTE( pSpn != NULL);
    pSpn->SetBase(1);
    pSpn->SetRange(1, 5);

    pSpn = (CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_TPNUM);
    _ASSERTE( pSpn != NULL);
    pSpn->SetBase(1);
    pSpn->SetRange(1, 5);

	UpdateSaveType(m_bSaveRecent);

    if(m_bReadOnly)
    {
        // Just show settings
        m_cmbPort.EnableWindow(FALSE);
		m_cmbBaudRate.EnableWindow(FALSE);
						
		((CButton*)GetDlgItem(IDC_RADIO_SAVEALL))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_SAVERCT))->EnableWindow(FALSE);

        ((CEdit*)GetDlgItem(IDC_EDIT_LOGNAME))->SetReadOnly();
        ((CEdit*)GetDlgItem(IDC_EDIT_LOG_PATH))->SetReadOnly();

		GetDlgItem(IDC_SPIN_TPNUM)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPN_NUM)->EnableWindow(FALSE);	

        GetDlgItem(IDC_BUTTON_BROWSE)->EnableWindow(FALSE);
        GetDlgItem(IDOK)->EnableWindow(FALSE);
    }

    if( m_nChannelType == TOOL_CHANNEL_UART )
    {
        ( (CButton*) GetDlgItem( IDC_RADIO_UART ) )->SetCheck( TRUE );
    }
    else if( m_nChannelType == TOOL_CHANNEL_SOCKET_CLIENT )
    {
        ( (CButton*) GetDlgItem( IDC_RADIO_SOCKET ) )->SetCheck( TRUE );
    }   

	
    m_ctrlIpAddr.SetAddress( m_dwIpAddr );
	((CComboBox*)GetDlgItem( IDC_COMBO_ENDIAN ))->SetCurSel( m_nEndian );

	//teana hu 2009.05.15
	if(m_dwGenerateTPTime == TOOL_TIME)
	{
		m_btnGenerateTPTimeByTool.SetCheck(1);
		m_btnGenerateTPTimeByD0A0.SetCheck(0);
		m_btnGenerateTPTimeByPhone.SetCheck(0);
	}
	else if(m_dwGenerateTPTime == D0A0_TIME)
	{
		m_btnGenerateTPTimeByTool.SetCheck(0);
		m_btnGenerateTPTimeByD0A0.SetCheck(1);
		m_btnGenerateTPTimeByPhone.SetCheck(0);
	}
	else if(m_dwGenerateTPTime == PHONE_TIME)
	{
		m_btnGenerateTPTimeByTool.SetCheck(0);
		m_btnGenerateTPTimeByD0A0.SetCheck(0);
		m_btnGenerateTPTimeByPhone.SetCheck(1);
	}

	if(m_bGenerateOrgFile)
	{
		m_btnGenerateOrgFile.SetCheck(1);
	}
	else
	{
		m_btnGenerateOrgFile.SetCheck(0);
	}
	

	UpdateData(FALSE);
	return TRUE;  
}

//Get the port number
DWORD CSetComDlg::GetPortNum()
{
    DWORD dwPort;
    m_strPort.MakeUpper();
    sscanf(m_strPort,"COM%ld",&dwPort);
    return dwPort;

//	return m_strPort.GetAt(3) - 48;
}

//Get the baud rate
DWORD CSetComDlg::GetBaudRate()
{
	UpdateData(FALSE);
	DWORD rate;
	sscanf(m_strRate,"%d",&rate);
	return rate;
}

DWORD CSetComDlg::GetFrameHead()
{
	WORD wFrameHead = (WORD)strtol(m_strFrameHead, '\0', 16);
	return wFrameHead;
}

void CSetComDlg::OnButtonBrowse() 
{
    char path[MAX_PATH];
    BROWSEINFO bi;
    memset(&bi,0,sizeof(bi));
    bi.hwndOwner = m_hWnd;
    bi.pszDisplayName = path;
	LPITEMIDLIST lpid = SHBrowseForFolder(&bi);
    
    if( NULL != lpid )
    {
        SHGetPathFromIDList(lpid,path);
        m_strLogPath = path;
        m_edtPath.SetWindowText(m_strLogPath);
    }
}

// Save serial port settings
void CSetComDlg::SaveSettings()
{
	//polo.jiang change cfg from reg to ini
    GetModuleFileName(NULL,m_szCfgFile,_MAX_PATH);
    TCHAR* p = _tcsrchr(m_szCfgFile,_T('.'));
    if(p != NULL)
    {
        *p = _T('\0');
    } 
	_tcscat(m_szCfgFile,_T(".ini"));
	WritePrivateProfileInt( SECTION_SETTINGS, ENTRY_CHANNEL,_T("%d"),m_nChannelType,m_szCfgFile);
	WritePrivateProfileInt( SECTION_SETTINGS, ENTRY_GENERATE_TIME, _T("%d"), m_dwGenerateTPTime,m_szCfgFile);
	WritePrivateProfileInt( SECTION_SETTINGS, ENTRY_GENERATE_ORGFILE, _T("%d"), m_bGenerateOrgFile,m_szCfgFile);
	WritePrivateProfileInt( SECTION_SETTINGS, ENTRY_AUTOTEST, _T("%d"),m_bAutotest, m_szCfgFile);
	WritePrivateProfileInt( SECTION_SETTINGS, "AutoConnect", _T("%d"),m_bAutoConnect, m_szCfgFile);
	
	WritePrivateProfileInt( SECTION_SETTINGS, "memfilesize", _T("%d"),m_dwMemFileSize, m_szCfgFile);

	WritePrivateProfileInt(SECTION_PORT, ENTRY_PORT,_T("%d"), m_nPortIndex,m_szCfgFile);
	WritePrivateProfileInt(SECTION_PORT, ENTRY_BAUDRATE,_T("%d"), m_nBaudIndex,m_szCfgFile);

	WritePrivateProfileInt( SECTION_FILE,  ENTRY_SAVERECENT, _T("%d"), m_bSaveRecent,m_szCfgFile);
    WritePrivateProfileInt( SECTION_FILE,  ENTRY_FILESIZE, _T("%d"), m_nMaxNum,m_szCfgFile);
    WritePrivateProfileInt( SECTION_FILE,  ENTRY_TPNUM, _T("%d"), m_nTPNum,m_szCfgFile);
	WritePrivateProfileString(SECTION_FILE,ENTRY_PATH,m_strLogPath.operator LPCTSTR(),m_szCfgFile);
	  
    WritePrivateProfileInt( SECTION_SOCKET, ENTRY_SOCKET_PORT, _T("%d"), m_nSocketPort,m_szCfgFile);
    //WritePrivateProfileInt( SECTION_SOCKET, ENTRY_SOCKET_ADDR, _T("%d"), m_dwIpAddr,m_szCfgFile);
	WritePrivateProfileString(SECTION_SOCKET,ENTRY_SOCKET_ADDR,m_strIpAddr.operator LPCTSTR(),m_szCfgFile);
	WritePrivateProfileInt( SECTION_SOCKET, ENTRY_SOCKET_ENDIAN,_T("%d"), m_nEndian,m_szCfgFile);

	//polo add dsp type
	WritePrivateProfileInt( SECTION_SETTINGS, "DSPType", _T("%d"), m_nLogType,m_szCfgFile);
	
   /* CWinApp* pApp = AfxGetApp();

    pApp->WriteProfileInt(SECTION_PORT,ENTRY_PORT,m_nPortIndex);
    pApp->WriteProfileInt(SECTION_PORT,ENTRY_BAUDRATE,m_nBaudIndex);

    pApp->WriteProfileInt(SECTION_FILE, ENTRY_SAVERECENT, m_bSaveRecent);
    pApp->WriteProfileInt(SECTION_FILE, ENTRY_FILESIZE, m_nMaxNum);
    pApp->WriteProfileInt(SECTION_FILE, ENTRY_TPNUM, m_nTPNum);

    pApp->WriteProfileString(SECTION_PATH,ENTRY_PATH,m_strLogPath);

    pApp->WriteProfileInt( SECTION_CHANNEL,ENTRY_CHANNEL,m_nChannelType );
    
    pApp->WriteProfileInt( SECTION_SOCKET,ENTRY_SOCKET_PORT,m_nSocketPort );
    pApp->WriteProfileInt( SECTION_SOCKET,ENTRY_SOCKET_ADDR,m_dwIpAddr );
	pApp->WriteProfileInt( SECTION_SOCKET,ENTRY_SOCKET_ENDIAN,m_nEndian );

	//teana hu 2009.06.02
	pApp->WriteProfileInt( GENERATE_TIME, ENTRY_GENERATE_TIME, m_dwGenerateTPTime);
	pApp->WriteProfileInt( GENERATE_ORGFILE, ENTRY_GENERATE_ORGFILE, m_bGenerateOrgFile);
	*/
}

// Load serial port settings
void CSetComDlg::LoadSettings()
{
	//polo.jiang change cfg from reg to ini
	TCHAR szPath[_MAX_PATH] = {0};
	TCHAR szValue[_MAX_PATH] = {0};
    GetModuleFileName(NULL,szPath,_MAX_PATH);
	_tcscpy(m_szCfgFile,szPath);
    TCHAR* p = _tcsrchr(szPath,_T('\\'));
    if(p != NULL)
    {
        *p = _T('\0');
    } 
	p=_tcsrchr(m_szCfgFile,_T('.'));
    if(p != NULL)
    {
        *p = _T('\0');
    } 
	_tcscat(m_szCfgFile,_T(".ini"));

	m_nChannelType = GetPrivateProfileInt( SECTION_SETTINGS,ENTRY_CHANNEL,m_nChannelType, m_szCfgFile );
	m_dwGenerateTPTime = GetPrivateProfileInt( SECTION_SETTINGS, ENTRY_GENERATE_TIME, TOOL_TIME, m_szCfgFile );
	m_bGenerateOrgFile = GetPrivateProfileInt( SECTION_SETTINGS, ENTRY_GENERATE_ORGFILE, 1, m_szCfgFile );
	m_bAutotest		= GetPrivateProfileInt( SECTION_SETTINGS,ENTRY_AUTOTEST,0, m_szCfgFile );
	m_bAutoConnect	= GetPrivateProfileInt( SECTION_SETTINGS,"AutoConnect",0, m_szCfgFile );
	m_bLogSN        = GetPrivateProfileInt( SECTION_SETTINGS,"LogSN",0, m_szCfgFile );

	m_dwMemFileSize=GetPrivateProfileInt( SECTION_SETTINGS, "memfilesize", 0, m_szCfgFile );
	m_nPortIndex   = GetPrivateProfileInt( SECTION_PORT,ENTRY_PORT, m_nPortIndex, m_szCfgFile );
	m_nBaudIndex   = GetPrivateProfileInt( SECTION_PORT,ENTRY_BAUDRATE, m_nBaudIndex, m_szCfgFile );
	
	m_bSaveRecent  = GetPrivateProfileInt( SECTION_FILE, ENTRY_SAVERECENT, FALSE, m_szCfgFile );
    m_nMaxNum      = GetPrivateProfileInt( SECTION_FILE, ENTRY_FILESIZE, 5, m_szCfgFile );
    m_nTPNum       = GetPrivateProfileInt( SECTION_FILE, ENTRY_TPNUM, 1, m_szCfgFile );
	GetPrivateProfileString(SECTION_FILE,ENTRY_PATH,szPath,szValue, _MAX_PATH, m_szCfgFile);
	CreateDir(szValue);
	m_strLogPath = szValue;
    
    
    m_nSocketPort = GetPrivateProfileInt( SECTION_SOCKET,ENTRY_SOCKET_PORT,m_nSocketPort, m_szCfgFile );
    //m_dwIpAddr = GetPrivateProfileInt( SECTION_SOCKET,ENTRY_SOCKET_ADDR,m_dwIpAddr, m_szCfgFile );
	GetPrivateProfileString(SECTION_SOCKET,ENTRY_SOCKET_ADDR,m_strIpAddr.operator LPCTSTR(),szValue, _MAX_PATH, m_szCfgFile);
	m_strIpAddr = szValue;
	m_dwIpAddr = ntohl(inet_addr(m_strIpAddr.operator LPCTSTR()));
	m_nEndian = GetPrivateProfileInt( SECTION_SOCKET,ENTRY_SOCKET_ENDIAN,m_nEndian, m_szCfgFile );
	m_nLogType = GetPrivateProfileInt( "Settings","DSPType",E_COMMON_DSP, m_szCfgFile );
	/* 
	CWinApp* pApp  = AfxGetApp();
    m_nPortIndex   = pApp->GetProfileInt(SECTION_PORT,ENTRY_PORT,m_nPortIndex);
    m_nBaudIndex   = pApp->GetProfileInt(SECTION_PORT,ENTRY_BAUDRATE,m_nBaudIndex);

    m_bSaveRecent  = pApp->GetProfileInt(SECTION_FILE, ENTRY_SAVERECENT, FALSE);
    m_nMaxNum      = pApp->GetProfileInt(SECTION_FILE, ENTRY_FILESIZE, 5);
    m_nTPNum       = pApp->GetProfileInt(SECTION_FILE, ENTRY_TPNUM, 1);

    
    m_strLogPath = pApp->GetProfileString(SECTION_PATH,ENTRY_PATH,szPath);

    m_nChannelType = pApp->GetProfileInt( SECTION_CHANNEL,ENTRY_CHANNEL,m_nChannelType );
    
    m_nSocketPort = pApp->GetProfileInt( SECTION_SOCKET,ENTRY_SOCKET_PORT,m_nSocketPort );
    m_dwIpAddr = pApp->GetProfileInt( SECTION_SOCKET,ENTRY_SOCKET_ADDR,m_dwIpAddr );
	m_nEndian = pApp->GetProfileInt( SECTION_SOCKET,ENTRY_SOCKET_ENDIAN,m_nEndian );

	//teana hu 2009.06.02
	m_dwGenerateTPTime = pApp->GetProfileInt(GENERATE_TIME, ENTRY_GENERATE_TIME, TOOL_TIME);
	m_bGenerateOrgFile = pApp->GetProfileInt(GENERATE_ORGFILE, ENTRY_GENERATE_ORGFILE, 1);
	
	m_bAutotest = pApp->GetProfileInt( SECTION_SETTINGS,ENTRY_AUTOTEST,0 );
	*/
	
}

void CSetComDlg::EnumSerialPorts(LPCALLBACKFUNC pFunc,DWORD dwUserArg)
{
    //Get the UART count from register table
    HKEY  hOpenKey;
    const cLen = 255;
    DWORD dwValueLen;
    DWORD dwDataLen;
    char szValueName[cLen];
    char szData[cLen];
    DWORD dwType;
    DWORD dwRet;
    int i = 0;
    
    if((dwRet =RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
        _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"),
        NULL,
        KEY_QUERY_VALUE | KEY_READ,
        &hOpenKey)) != ERROR_SUCCESS)
    {
        return;
    }
    
    while(1)
    {
        dwValueLen = cLen;
        dwDataLen = cLen;
        szValueName[0] = 0;
        szData[0] = 0;
        
        dwRet = ::RegEnumValue(hOpenKey,
            i++,
            szValueName,
            &dwValueLen,
            NULL,
            &dwType,
            (BYTE*)szData,
            &dwDataLen
            );
        
        if(dwRet == ERROR_NO_MORE_ITEMS)
            break;
        
        int port;
        int nRet = sscanf(szData,"COM%d",&port);
        
        if(nRet == 0)
        {
            continue;
        }
        
        (this->*pFunc)(port,dwUserArg);
    }
    
    RegCloseKey(hOpenKey);
}

int CSetComDlg::GetPort(int nPort,DWORD )
{
    CString strPort;
    strPort.Format("COM%d",nPort);
    int nIndex = m_cmbPort.AddString(strPort);
    
    if(nPort == m_nPortIndex)
    {
        m_cmbPort.SetCurSel(nIndex);
    }
    
    return 0;
}

void CSetComDlg::OnSelchangeComboPort() 
{
    int nIndex = m_cmbPort.GetCurSel();
    if(nIndex == -1)
    {
        return;
    }
    
    CString strText;
    m_cmbPort.GetLBText(nIndex,strText);
    sscanf(strText,"COM%d",&m_nPortIndex);
}

void CSetComDlg::OnOK() 
{

    if (!UpdateData(TRUE))
    {
        TRACE0("UpdateData failed during dialog termination.\n");
        // the UpdateData routine will set focus to correct item
        return;
    }

	//teana hu 2009.03.16
	if(m_strFrameHead.GetAt(0) != '0' || m_strFrameHead.GetAt(1) != 'x')
	{
		AfxMessageBox("FrameHead must be 0x****");
		return;
	}

    OnSelchangeComboBaudrate();
    OnSelchangeComboPort();

	m_nBaudIndex = atoi(m_strRate);

    if( !CheckLogPath() )
    {
        return;
    }

    m_ctrlIpAddr.GetAddress( m_dwIpAddr );

	BYTE bF1 = 0;
	BYTE bF2 = 0;
	BYTE bF3 = 0;
	BYTE bF4 = 0;
	m_ctrlIpAddr.GetAddress(bF1, bF2, bF3, bF4);
	m_strIpAddr.Format("%d.%d.%d.%d", bF1, bF2, bF3, bF4);
	m_nEndian = ((CComboBox*)GetDlgItem( IDC_COMBO_ENDIAN ))->GetCurSel();

	//teana hu 2009.05.15
	if(m_btnGenerateTPTimeByTool.GetCheck() == 1)
	{
		m_dwGenerateTPTime = TOOL_TIME;
	}
	else if(m_btnGenerateTPTimeByD0A0.GetCheck() == 1)
	{
		m_dwGenerateTPTime = D0A0_TIME;
	}
	else if(m_btnGenerateTPTimeByPhone.GetCheck() == 1)
	{
		m_dwGenerateTPTime = PHONE_TIME;
	}

	if(m_btnGenerateOrgFile.GetCheck() == 1)
	{
		m_bGenerateOrgFile = TRUE;
	}
	else
	{
		m_bGenerateOrgFile = FALSE;
	}

	SaveSettings();
    EndDialog(IDOK);
	
}

BOOL CSetComDlg::CheckLogPath()
{
    if( INVALID_HANDLE_VALUE == ::CreateFile( m_strLogPath,GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL ) )
    {
        CString strErr;
        strErr.Format( IDS_ACCESS_LOG_PATH_FAIL,m_strLogPath,GetLastError() );
        AfxMessageBox( strErr,MB_ICONSTOP );
        return FALSE;
    }
	return TRUE;
}

void CSetComDlg::OnRadioSaveAll() 
{
	int nStatus = ((CButton*)GetDlgItem(IDC_RADIO_SAVEALL))->GetCheck();
	if(nStatus==BST_CHECKED)
	{
		m_bSaveRecent= FALSE;
	}
	UpdateSaveType(FALSE);
}

void CSetComDlg::OnRadioSaveRecent() 
{
	int nStatus = ((CButton*)GetDlgItem(IDC_RADIO_SAVERCT))->GetCheck();
	if(nStatus==BST_CHECKED)
	{
		m_bSaveRecent= TRUE;
	}
	UpdateSaveType(TRUE);
}


BOOL CSetComDlg::CheckLogName()
{

	return FALSE;
}


void CSetComDlg::OnSetfocusEditLogname() 
{
	UpdateData();

	if (DEF_AUTOGENERATE == m_strLogName)
	{
		// Delete all of the text.
		m_editLogName.SetSel(0, -1);
		m_editLogName.Clear();
		m_editLogName.SetFocus();	
	
	}
	else
	{
		m_editLogName.SetSel(0, -1);
		m_editLogName.SetFocus();			
	}

}

void CSetComDlg::OnKillfocusEditLogname() 
{
	UpdateData();

	if ("" == m_strLogName)
	{
		m_strLogName= DEF_AUTOGENERATE;
	}
	else
	{
		CheckLogName();
	}
	UpdateData(FALSE);
}

void CSetComDlg::UpdateSaveType(BOOL bSaveRecent)
{
	if (bSaveRecent == FALSE)
	{
		((CButton*)GetDlgItem(IDC_RADIO_SAVEALL))->SetCheck(BST_CHECKED);
		GetDlgItem(IDC_SPN_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_TPNUM)->EnableWindow(FALSE);		
	}
	else
	{	
		((CButton*)GetDlgItem(IDC_RADIO_SAVERCT))->SetCheck(BST_CHECKED);		
		GetDlgItem(IDC_SPIN_TPNUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPN_NUM)->EnableWindow(FALSE);		
	}
}

CString CSetComDlg::GetLogName()
{
	CString strFileName;
	if (m_strLogName == DEF_AUTOGENERATE)
	{
		strFileName.Empty();
	}
	else
	{
		strFileName=m_strLogName;
	}
	return strFileName;
}

void CSetComDlg::OnRadioUart() 
{
    m_nChannelType = TOOL_CHANNEL_UART; 
	EnableSocketSetting(FALSE);
}

void CSetComDlg::OnRadioSocket() 
{
	m_nChannelType = TOOL_CHANNEL_SOCKET_CLIENT;
	EnableSocketSetting(TRUE);
}
void CSetComDlg::EnableSocketSetting(BOOL bEnable)
{
	GetDlgItem(IDC_COMBO_ENDIAN)->EnableWindow(bEnable);
	GetDlgItem(IDC_RDO_COMMON)->EnableWindow(bEnable);
	GetDlgItem(IDC_RDO_LTE)->EnableWindow(bEnable);
	GetDlgItem(IDC_RDO_TG)->EnableWindow(bEnable);
	GetDlgItem(IDC_RDO_WCDMA)->EnableWindow(bEnable);
	GetDlgItem(IDC_RDO_AGDSP)->EnableWindow(bEnable);
}

void CSetComDlg::GetChannelArgs( LPDWORD lpArg )
{
    if( m_nChannelType == TOOL_CHANNEL_UART )
    {
        lpArg[0] = m_nPortIndex;
        lpArg[1] = m_nBaudIndex;
    }
    else
    {
        lpArg[0] = m_nSocketPort;
        CONVERT_INT( m_dwIpAddr,lpArg[1] );
        lpArg[2] = TRUE; // Socket client
    }
}

void CSetComDlg::OnSelchangeComboBaudrate() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_cmbBaudRate.GetCurSel();
    if(nIndex == -1)
    {
        return;
    }
    
    CString strText;
    m_cmbBaudRate.GetLBText(nIndex,strText);
    m_nBaudIndex = atoi(strText);
}

void CSetComDlg::OnEditchangeComboBaudrate() 
{
	// TODO: Add your control notification handler code here
	CString strInput;
	m_cmbBaudRate.GetWindowText(strInput);
	int nCount = strInput.GetLength();

	for(int i = 0; i < nCount; i++)
	{
		char ch = strInput.GetAt(i);
		
		if(ch < '0' || ch > '9')
		{
			AfxMessageBox("BaudRate must be digit!");
			m_strRate = "";
			UpdateData(FALSE);
			return;
		}
	}
}
BOOL CSetComDlg::WritePrivateProfileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpFormat,	int nValue, LPCTSTR lpFileName)
{
	CString lpString;
	lpString.Format(lpFormat, nValue);
	return WritePrivateProfileString(lpAppName, lpKeyName, lpString, lpFileName);
}
