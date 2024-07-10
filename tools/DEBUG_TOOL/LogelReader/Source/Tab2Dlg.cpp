// Tab2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "SUC.h"
#include "Tab2Dlg.h"
#include "LoadDll.h"
//#include "Uart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTab2Dlg dialog

CTab2Dlg::CTab2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTab2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTab2Dlg)
 	m_uiERport    = 0;
 	m_uiEbaudrate = 0;
	m_bFlagOpen   =FALSE;
	m_pFile       =NULL;
	//}}AFX_DATA_INIT
}


void CTab2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTab2Dlg)
	DDX_Control(pDX, IDC_RELIST, m_mylistre);
	DDX_Text(pDX, IDC_ERPORT, m_uiERport);
	DDX_Text(pDX, IDC_EBAUDRATE, m_uiEbaudrate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTab2Dlg, CDialog)
	//{{AFX_MSG_MAP(CTab2Dlg)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_RECEIVESTOP, OnReceivestop)
	ON_BN_CLICKED(IDC_RECEIVECLEAR, OnReceiveclear)
	ON_MESSAGE(RECMESSAGE,OnReceiveBuffer)
	ON_BN_CLICKED(IDC_SEND_DUMP, OnSendDump)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTab2Dlg message handlers
BOOL CTab2Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString Uart="UartComm.dll";
	//CString Uart="SocketComm.dll";
	m_dllObj.LoadCommHandler((LPCTSTR) Uart);

	(CButton*)GetDlgItem(IDC_START)->EnableWindow(FALSE); //start»Ò»¯
	(CButton*)GetDlgItem(IDC_RECEIVESTOP)->EnableWindow(FALSE);//stop»Ò»¯

	//get file path
	if (GetModuleFileName(NULL, m_rzFileDir,MAX_PATH)!=0)
	{
		char *pDir = strrchr(m_rzFileDir, '\\');
		ASSERT(NULL != pDir);
		*pDir = '\0';
		strcat(pDir, "\\LogelReader.ini");
		m_uiERport = GetPrivateProfileInt("ReceiveInfo","Port",RECEIVE_PORT_DEFAULT,m_rzFileDir);
 		m_uiEbaudrate = GetPrivateProfileInt("ReceiveInfo","BaudRate",BAUD_RATE_DEFAULT,m_rzFileDir);
	} 
	else
	{
		m_uiERport    = 0;
 		m_uiEbaudrate = 0;
	}
	UpdateData(FALSE);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CTab2Dlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	const char strFilter[]  = "Information file (*.*)|*.*||";
	const char strCaption[] = "Save to...";
	// Get the path name

	CFileDialog dlg(FALSE,
					"(*.*)|*.*||",
					NULL,
					OFN_HIDEREADONLY| OFN_FILEMUSTEXIST,
					strFilter);

 	dlg.m_ofn.lpstrTitle = strCaption;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	m_strPathName = dlg.GetPathName();
	m_mylistre.AddString(m_strPathName);
	int nCount = m_mylistre.GetCount();
	m_mylistre.SetTopIndex(nCount - 1);
	if (nCount > 0)
		m_mylistre.SetCurSel(nCount-1);

	UpdateData(FALSE);
	GetDlgItem(IDC_START)->EnableWindow(TRUE);
}


void CTab2Dlg::OnReceiveBuffer(WPARAM wParam, LPARAM lParam)
{
	LPBYTE  m_pReBuffer   = ( LPBYTE )wParam;
	DWORD   m_dwSize      = ( DWORD )lParam;

	if (m_bFlagRe==TRUE)
	{
		fwrite(m_pReBuffer,1,m_dwSize,m_pFile);
	}
}


void CTab2Dlg::OnStart() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDC_ERPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EBAUDRATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_RECEIVESTOP)->EnableWindow(TRUE);
	
	if (m_bFlagOpen==FALSE)
	{
		BYTE OpenArgument[8];
		//BYTE OpenArgument[12];
		//  open the UART
		*((DWORD*)OpenArgument)=m_uiERport;
		*((DWORD*)(OpenArgument + sizeof( DWORD)))=m_uiEbaudrate;
		//*((DWORD*)(OpenArgument + sizeof( DWORD) * 2 ))=TRUE;
		m_dllObj.m_pCommhandler->SetReceiver( RECMESSAGE, FALSE, GetSafeHwnd() );

		
		if (m_dllObj.m_pCommhandler->Open(OpenArgument)==S_OK)
		{
			
			m_mylistre.AddString("Receiver Set up Successful!");
			int nCount = m_mylistre.GetCount();
			m_mylistre.SetTopIndex(nCount - 1);
			if (nCount > 0)
			{
				m_mylistre.SetCurSel(nCount-1);
			}
		}
		else
		{
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			GetDlgItem(IDC_ERPORT)->EnableWindow(TRUE);
			GetDlgItem(IDC_EBAUDRATE)->EnableWindow(TRUE);
			CString str;
			str.LoadString(IDS_STRING_BUTTON_START);
			GetDlgItem(IDC_START)->SetWindowText((LPCTSTR)str);	

			m_mylistre.AddString("Receiver Set up Fail!");
			int nCount = m_mylistre.GetCount();
			m_mylistre.SetTopIndex(nCount - 1);
			if (nCount > 0)
			{
				m_mylistre.SetCurSel(nCount-1);
			}
			return;
		}

		m_pFile = fopen(m_strPathName,"wb");
		CString str;
		if (m_pFile==NULL)
		{
			str.LoadString(IDS_STRING_RECEIVE_OPEN);
			AfxMessageBox(IDS_STRING_RECEIVE_OPEN,IDYES);
			return;
		}
		m_bFlagOpen=TRUE;
		UpdateData(FALSE);
	}

	if(m_bFlagRe==TRUE)
		m_bFlagRe =FALSE;
	else
		m_bFlagRe =TRUE;

	CString str1,str2;
	if (m_bFlagRe==TRUE)
	{
		str1.LoadString(IDS_STRING_BUTTON_PAUSE);
		GetDlgItem(IDC_START)->SetWindowText((LPCTSTR)str1);	
	}
	else
	{
		str2.LoadString(IDS_STRING_BUTTON_START);
		GetDlgItem(IDC_START)->SetWindowText((LPCTSTR)str2);	
		
	}

	CString StrTmpPort,StrTmpBaudRate;
	//convert the type of 'UINT' to 'Cstring'
	StrTmpPort.Format("%d",m_uiERport);
	StrTmpBaudRate.Format("%d",m_uiEbaudrate);
	//write the key to ini file
	::WritePrivateProfileString("ReceiveInfo","Port",StrTmpPort,m_rzFileDir);
	::WritePrivateProfileString("ReceiveInfo","BaudRate",StrTmpBaudRate,m_rzFileDir);
}

void CTab2Dlg::OnReceivestop() 
{
	// TODO: Add your control notification handler code here
	m_bFlagRe  =FALSE;
	m_bFlagOpen=FALSE;
	fclose( m_pFile );
	
	GetDlgItem(IDC_ERPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_EBAUDRATE)->EnableWindow(TRUE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);

	CString str2;
	str2.LoadString(IDS_STRING_BUTTON_START);
	GetDlgItem(IDC_START)->SetWindowText((LPCTSTR)str2);
	GetDlgItem(IDC_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_RECEIVESTOP)->EnableWindow(FALSE);
	
	m_dllObj.m_pCommhandler->Close();
}

void CTab2Dlg::OnReceiveclear() 
{
	// TODO: Add your control notification handler code here
	m_mylistre.ResetContent();
	
}

void CTab2Dlg::OnSendDump() 
{
	// TODO: Add your control notification handler code here

	BYTE request[8] = {00,00,00,00,00,00,05,04};
	m_dllObj.m_pCommhandler->Write(request, 8);
}
