// ComSettings.cpp : implementation file
//

#include "stdafx.h"
#include "msdev.h"
#include "ComSettings.h"
#include "SerialCom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComSettings dialog


CComSettings::CComSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CComSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComSettings)
	m_port = 0;
	m_baud = 0;
	//}}AFX_DATA_INIT
}


void CComSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComSettings)
	DDX_Control(pDX, IDC_COMBO1, m_portList);
	DDX_Text(pDX, IDC_EDIT1, m_port);
	//DDV_MinMaxUInt(pDX, m_port, 1, 50);
	DDX_Text(pDX, IDC_EDIT2, m_baud);
	DDV_MinMaxUInt(pDX, m_baud, 0, 1000000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CComSettings, CDialog)
	//{{AFX_MSG_MAP(CComSettings)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComSettings message handlers

BOOL CComSettings::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString str;
	CUIntArray ports;
	CSerialCom com;
	//枚举当前设备上的所有串口
	com.EnumerateSerialPorts(ports);
	if(ports.GetSize() > 0)
	{
		for(int i=0;i < ports.GetSize();i++)
		{
			CString strPort;
			if(ports.GetAt(i) > 0)
			{
			    strPort.Format("%d",ports.GetAt(i));
                m_portList.AddString(strPort);
			}
		}
	}
	//从配置文件中读取串口号和波特率
    GetPrivateProfileString("com settings","com","1", str.GetBuffer(MAX_PATH),MAX_PATH,".\\comsettings.ini");
	m_portList.SetWindowText(str);
    //m_port = (UINT)atoi(str.GetBuffer(str.GetLength()));
    GetPrivateProfileString("com settings","baud","9600", str.GetBuffer(MAX_PATH),MAX_PATH,".\\comsettings.ini");
    m_baud = (UINT)atoi(str.GetBuffer(str.GetLength()));
	UpdateData(FALSE);
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CComSettings::OnButton1() 
{
	// TODO: Add your control notification handler code here
	WriteIni();
	EndDialog(OPEN_PORT);
	//CDialog::OnOK();
}

void CComSettings::OnButton2() 
{
	// TODO: Add your control notification handler code here
	WriteIni();
	EndDialog(CLOSE_PORT);
	//CDialog::OnCancel();
}
//写配置文件
void CComSettings::WriteIni()
{
    UpdateData();
	CString str;
	m_portList.GetWindowText(str);
	//str.Format("%d",m_port);
	WritePrivateProfileString("com settings","com",str.GetBuffer(MAX_PATH),".\\comsettings.ini");
	str.ReleaseBuffer();
	str.Format("%d",m_baud);
	WritePrivateProfileString("com settings","baud",str.GetBuffer(MAX_PATH),".\\comsettings.ini");
	str.ReleaseBuffer();
} 
