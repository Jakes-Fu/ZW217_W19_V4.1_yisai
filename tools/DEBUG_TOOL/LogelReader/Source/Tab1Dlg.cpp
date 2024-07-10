// Tab1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "SUC.h"
#include "Tab1Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define WM_RECV_DATA    WM_APP + 300 
/////////////////////////////////////////////////////////////////////////////
// CTab1Dlg dialog

CTab1Dlg::CTab1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTab1Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTab1Dlg)
 	m_uiEdelay = 0;
 	m_uiEport  = 0;
 	m_uiEsize  = 0;
	m_pBuffer  = NULL;
	m_pFile    = NULL;
	//}}AFX_DATA_INIT
}


void CTab1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTab1Dlg)
	DDX_Control(pDX, IDC_LIST1, m_mylist);
	DDX_Text(pDX, IDC_EDELAY, m_uiEdelay);
	DDX_Text(pDX, IDC_EPORT, m_uiEport);
	DDX_Text(pDX, IDC_ESIZE, m_uiEsize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTab1Dlg, CDialog)
	//{{AFX_MSG_MAP(CTab1Dlg)
	ON_BN_CLICKED(IDC_OPEN, OnOpen)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SET_UP, OnSetUp)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_SENDCLEAR, OnSendclear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTab1Dlg message handlers

BOOL CTab1Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	//open socket
	CString Sock="SocketComm.dll";
	m_dllSocObj.LoadCommHandler((LPCTSTR)Sock);
	
	(CButton*)GetDlgItem(IDC_SEND)->EnableWindow(FALSE); 
	(CButton*)GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_OPEN)->EnableWindow(FALSE);


	if (GetModuleFileName(NULL, m_szFileDir,MAX_PATH)!=0)
	{
		char *pDir = strrchr(m_szFileDir, '\\');
		ASSERT(NULL != pDir);
		*pDir = '\0';
		strcat(pDir, "\\LogelReader.ini");
		m_uiEdelay = GetPrivateProfileInt("SendInfo","Delay",DELAY_DEFAULT,m_szFileDir);
		m_uiEport = GetPrivateProfileInt("SendInfo","Port",SEND_PORT_DEFAULT,m_szFileDir);
		m_uiEsize = GetPrivateProfileInt("SendInfo","Size",SIZE_DEFAULT,m_szFileDir);
	}
	else
	{
		m_uiEdelay = 0;
 		m_uiEport  = 0;
 		m_uiEsize  = 0;
	}
	UpdateData(FALSE);
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CTab1Dlg::OnOpen() 
{
	// TODO: Add your control notification handler code here
	const char strFilter[]  = "Information file (*.*)|*.*||";
	const char strCaption[] = "Open";
	// Get the path name

	CFileDialog dlg(TRUE,
					"(*.*)|*.*||",
					NULL,
					OFN_HIDEREADONLY| OFN_FILEMUSTEXIST,
					strFilter);

 	dlg.m_ofn.lpstrTitle = strCaption;
	if (dlg.DoModal() != IDOK)

		return;          
	m_strPathName = dlg.GetPathName();
	m_mylist.AddString(m_strPathName);
	UpdateData(FALSE);
	GetDlgItem(IDC_SEND)->EnableWindow(TRUE);
	//CDialog::OnOK();
}

void CTab1Dlg::OnSend() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_OPEN)->EnableWindow(FALSE);

	if(m_bFlag==TRUE)
		m_bFlag =FALSE;
	else
		m_bFlag =TRUE;

	if (m_bFlag==TRUE)
	{
		GetDlgItem(IDC_ESIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDELAY)->EnableWindow(FALSE);
		
		CString str1,str2;
		str1.LoadString(IDS_STRING_BUTTON_PAUSE);
		GetDlgItem(IDC_SEND)->SetWindowText((LPCTSTR )str1);
		
		m_pFile=fopen(m_strPathName,"rb");
		m_pBuffer=new BYTE[1024*m_uiEsize];
		if (m_pFile==NULL)
		{
			AfxMessageBox(str2.LoadString(IDS_STRING_SEND_OPEN),IDYES);
			return;
		} 
		else
		{	
			m_uiTimeEvent=SetTimer(TIMER_EVENT,m_uiEdelay,NULL);
		}		
	} 
	else
	{
		if (m_pBuffer != NULL)
		{
			delete[] m_pBuffer;
			m_pBuffer = NULL;
		}
		GetDlgItem(IDC_ESIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDELAY)->EnableWindow(TRUE);
		
		CString str2;
		str2.LoadString(IDS_STRING_BUTTON_SEND);
		GetDlgItem(IDC_SEND)->SetWindowText((LPCTSTR)str2);	
	}

	CString StrTmpPort,StrTmpSize,StrTmpDelay;
	StrTmpPort.Format("%d",m_uiEport);
	StrTmpSize.Format("%d",m_uiEsize);
	StrTmpDelay.Format("%d",m_uiEdelay);
	::WritePrivateProfileString("SendInfo","Port",StrTmpPort,m_szFileDir);
	::WritePrivateProfileString("SendInfo","Size",StrTmpSize,m_szFileDir);
	::WritePrivateProfileString("SendInfo","Delay",StrTmpDelay,m_szFileDir);
}

void CTab1Dlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnTimer(nIDEvent);
	CString str;
	if (nIDEvent==TIMER_EVENT)
	{
		if ((m_bFlag==TRUE))
		{
			if(!feof(m_pFile))
			{
				int singlebuf=fread(m_pBuffer,sizeof(BYTE),1024*m_uiEsize,m_pFile);
				if (!(ferror(m_pFile)))
				{
					if (singlebuf>0)
					{
						m_dllSocObj.m_pCommhandler->Write(m_pBuffer,singlebuf);	
					}
					else //end if (singlebuf>0)
					{
						GetDlgItem(IDC_OPEN)->EnableWindow(TRUE);
						GetDlgItem(IDC_EDELAY)->EnableWindow(TRUE);
						GetDlgItem(IDC_ESIZE)->EnableWindow(TRUE);
						GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
						CString str;
						str.LoadString(IDS_STRING_BUTTON_SEND);
						GetDlgItem(IDC_SEND)->SetWindowText((LPCTSTR )str);
						m_mylist.AddString("The open file is empty!");
						int nCount = m_mylist.GetCount();
						m_mylist.SetTopIndex(nCount - 1);
						if (nCount > 0)
						{
							m_mylist.SetCurSel(nCount-1);
						}
						m_bFlag=FALSE;
						return;	
					}
				}
				else  //end if (!(ferror(m_pFile)))
				{
					AfxMessageBox(IDS_STRING_SEND_OPEN_ERROR,IDYES );
					return;
				}
			}	
			else  //end if(!feof(m_pFile))
			{
				KillTimer(TIMER_EVENT);
				fclose(m_pFile);
				delete[] m_pBuffer;
				m_pBuffer = NULL;

				GetDlgItem(IDC_OPEN)->EnableWindow(TRUE);
				m_bFlag=FALSE;
				m_mylist.AddString("Send Over!");
				int nCount = m_mylist.GetCount();
				m_mylist.SetTopIndex(nCount - 1);
				if (nCount > 0)
				{
					m_mylist.SetCurSel(nCount-1);
				}
				GetDlgItem(IDC_ESIZE)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDELAY)->EnableWindow(TRUE);
				str.LoadString(IDS_STRING_BUTTON_SEND);
				GetDlgItem(IDC_SEND)->SetWindowText((LPCTSTR )str);
				UpdateData(FALSE);
			}
		}
	} 
}


void CTab1Dlg::OnSetUp() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_EPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_SET_UP)->EnableWindow(FALSE);
	
	BYTE OpenArgument[12];
	*((DWORD*)OpenArgument)=m_uiEport;
    *((DWORD*)(OpenArgument + sizeof( DWORD)))=(BYTE)124;
    *((DWORD*)(OpenArgument + sizeof( DWORD) * 2 ))=FALSE;
	m_dllSocObj.m_pCommhandler->SetReceiver( WM_RECV_DATA, FALSE, GetSafeHwnd() );
	if ((m_dllSocObj.m_pCommhandler->Open(OpenArgument))==S_OK)
	{
		m_mylist.AddString("Setup successful!");
		int nCount = m_mylist.GetCount();
		m_mylist.SetTopIndex(nCount-1);
		if (nCount > 0)
		{
			m_mylist.SetCurSel(nCount-1);
		}
		GetDlgItem(IDC_OPEN)->EnableWindow(TRUE);

	}
	else
	{
		GetDlgItem(IDC_SET_UP)->EnableWindow(TRUE);
		GetDlgItem(IDC_EPORT)->EnableWindow(TRUE);
		m_mylist.AddString("Setup fail!");
		int nCount = m_mylist.GetCount();
		m_mylist.SetTopIndex(nCount - 1);
		if (nCount > 0)
		{
			m_mylist.SetCurSel(nCount-1);
		}
	}
	UpdateData(FALSE);
}

void CTab1Dlg::OnStop() 
{
	// TODO: Add your control notification handler code here
	if (m_pBuffer!=NULL)
	{
		delete[] m_pBuffer;
		m_pBuffer=NULL;
	}
	GetDlgItem(IDC_EPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_ESIZE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDELAY)->EnableWindow(TRUE);
	GetDlgItem(IDC_SET_UP)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_OPEN)->EnableWindow(FALSE);
	
	CString str;
	str.LoadString(IDS_STRING_BUTTON_SEND);
	GetDlgItem(IDC_SEND)->SetWindowText((LPCTSTR)str);
	if (m_uiTimeEvent!=0)
	{
		KillTimer(TIMER_EVENT);
	} 
	m_dllSocObj.m_pCommhandler->Close();
}

void CTab1Dlg::OnSendclear() 
{
	// TODO: Add your control notification handler code here
	m_mylist.ResetContent();
}
