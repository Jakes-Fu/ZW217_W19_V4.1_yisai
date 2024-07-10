// DeviceSendDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SUC.h"
#include "DeviceSendDlg.h"
#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeviceSendDlg dialog


CDeviceSendDlg::CDeviceSendDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDeviceSendDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeviceSendDlg)
	m_uiEdelay = 0;
 	m_uiEport  = 0;
 	m_uiEsize  = 0;
	m_strDevice = "G";
	m_pBuffer  = NULL;
	m_bFlag = FALSE;

	m_dwStartSecPos = 0;
	m_dwSize = NULL;
	m_fpData = NULL;

	m_dwTotalData = 0;
	m_nLogCount = 0;
	m_nSendCount = 0;
	m_dwSendLen = 0;
	m_hResult = INVALID_HANDLE_VALUE;
	//test
	m_pFile = NULL;
	m_bSaveTmpFile = FALSE;
	memset(m_tmpFile, 0, MAX_PATH);
	
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDeviceSendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeviceSendDlg)
	DDX_Control(pDX, IDC_LIST_DEVICE, m_mylist);
	DDX_Text(pDX, IDC_DELAY_EDT, m_uiEdelay);
	DDX_Text(pDX, IDC_PORT_EDT, m_uiEport);
	DDX_Text(pDX, IDC_SIZE_EDT, m_uiEsize);
	DDX_Text(pDX, IDC_DISK_EDT, m_strDevice);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDeviceSendDlg, CDialog)
	//{{AFX_MSG_MAP(CDeviceSendDlg)
	ON_BN_CLICKED(IDC_SETUP_BTN, OnSetupBtn)
	ON_BN_CLICKED(IDC_SEND_BTN, OnSendBtn)
	ON_BN_CLICKED(IDC_STOP_BTN, OnStopBtn)
	ON_BN_CLICKED(IDC_CLEAR_BTN, OnClearBtn)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_LOAD_BTN, OnLoadBtn)
	ON_BN_CLICKED(IDC_OPEN_BTN, OnOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeviceSendDlg message handlers

void CDeviceSendDlg::OnSetupBtn() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_PORT_EDT)->EnableWindow(FALSE);
	GetDlgItem(IDC_DISK_EDT)->EnableWindow(FALSE);
	GetDlgItem(IDC_SETUP_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP_BTN)->EnableWindow(TRUE);

	BYTE OpenArgument[12];
	*((DWORD*)OpenArgument) = m_uiEport;
    *((DWORD*)(OpenArgument + sizeof( DWORD))) = (BYTE)124;
    *((DWORD*)(OpenArgument + sizeof( DWORD) * 2 )) = FALSE;
	m_dllSocObj.m_pCommhandler->SetReceiver( WM_RECV_DATA, FALSE, GetSafeHwnd() );
	if ((m_dllSocObj.m_pCommhandler->Open(OpenArgument)) == S_OK)
	{
		SetListStr("Setup successful!");
		GetDlgItem(IDC_SEND_BTN)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_SETUP_BTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_PORT_EDT)->EnableWindow(TRUE);
		SetListStr("Setup fail!");
	}
	UpdateData(FALSE);
	
}

void CDeviceSendDlg::OnSendBtn() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_bFlag = !m_bFlag;

	if (m_bFlag)
	{
		GetDlgItem(IDC_SIZE_EDT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DELAY_EDT)->EnableWindow(FALSE);
		
		CString str1,str2;
		str1.LoadString(IDS_STRING_BUTTON_PAUSE);
		GetDlgItem(IDC_SEND_BTN)->SetWindowText((LPCTSTR )str1);
		
		m_fpData = fopen(m_tmpFile, "rb");
		if(m_fpData == NULL)
		{
			SetListStr("Can not open temp file!");
			return;
		}
		m_pBuffer = new BYTE[1024*m_uiEsize];
		m_uiTimeEvent = SetTimer(TIMER_EVENT,m_uiEdelay,NULL);		
	} 
	else
	{
		if (m_pBuffer != NULL)
		{
			delete[] m_pBuffer;
			m_pBuffer = NULL;
		}
		GetDlgItem(IDC_SIZE_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DELAY_EDT)->EnableWindow(TRUE);
		
		CString str2;
		str2.LoadString(IDS_STRING_BUTTON_SEND);
		GetDlgItem(IDC_SEND_BTN)->SetWindowText((LPCTSTR)str2);	
	}

	CString StrTmpPort,StrTmpSize,StrTmpDelay;
	StrTmpPort.Format("%d",m_uiEport);
	StrTmpSize.Format("%d",m_uiEsize);
	StrTmpDelay.Format("%d",m_uiEdelay);
	::WritePrivateProfileString("SendInfo","Port",StrTmpPort,m_szFileDir);
	::WritePrivateProfileString("SendInfo","Size",StrTmpSize,m_szFileDir);
	::WritePrivateProfileString("SendInfo","Delay",StrTmpDelay,m_szFileDir);
	
}

void CDeviceSendDlg::OnStopBtn() 
{
	// TODO: Add your control notification handler code here
	if (m_uiTimeEvent!=0)
	{
		KillTimer(TIMER_EVENT);
	}
	if (m_pBuffer != NULL)
	{
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}
	if(m_dwSize != NULL)
	{
		delete [] m_dwSize;
		m_dwSize = NULL;
	}
	GetDlgItem(IDC_PORT_EDT)->EnableWindow(TRUE);
	GetDlgItem(IDC_DISK_EDT)->EnableWindow(TRUE);
	GetDlgItem(IDC_SIZE_EDT)->EnableWindow(TRUE);
	GetDlgItem(IDC_DELAY_EDT)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOAD_BTN)->EnableWindow(TRUE);
	GetDlgItem(IDC_SETUP_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_SEND_BTN)->EnableWindow(FALSE);
	
	m_bFlag = FALSE;
	m_dwTotalData = 0;
	m_nLogCount = 0;
	m_nSendCount = 0;
	m_dwSendLen = 0;
	
	CString str;
	str.LoadString(IDS_STRING_BUTTON_SEND);
	GetDlgItem(IDC_SEND_BTN)->SetWindowText((LPCTSTR)str);
	 
	m_dllSocObj.m_pCommhandler->Close();
	fclose(m_fpData);
	if(!m_bSaveTmpFile)
	{
		DeleteFile(m_tmpFile);
	}
	
}

void CDeviceSendDlg::OnLoadBtn() 
{
	// TODO: Add your control notification handler code here
	SetListStr("Load data.....please wait");
	GetFileName();
	if(MapToBuf() == FALSE)
	{
		return;
	}
	
//	if(OpenDiskAndRead() == FALSE)
//	{
//		return;
//	}
//
	MSG msg;
	while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);	
	}
	if(LoadData() == FALSE)
	{
		return;
	}
	SetListStr("Load data success!");
	GetDlgItem(IDC_DISK_EDT)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_LOAD_BTN)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_SETUP_BTN)->EnableWindow(TRUE);
}

void CDeviceSendDlg::OnClearBtn() 
{
	// TODO: Add your control notification handler code here
	m_mylist.ResetContent();
	
}

BOOL CDeviceSendDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//open socket
	CString Sock="SocketComm.dll";
	m_dllSocObj.LoadCommHandler((LPCTSTR)Sock);

	(CButton*)GetDlgItem(IDC_SETUP_BTN)->EnableWindow(FALSE); 
	(CButton*)GetDlgItem(IDC_SEND_BTN)->EnableWindow(FALSE); 
	(CButton*)GetDlgItem(IDC_STOP_BTN)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_LOAD_BTN)->EnableWindow(FALSE);

	((CEdit*)GetDlgItem(IDC_DISK_EDT))->SetLimitText(1);


	if (GetModuleFileName(NULL, m_szFileDir,MAX_PATH)!=0)
	{
		char *pDir = strrchr(m_szFileDir, '\\');
		ASSERT(NULL != pDir);
		*pDir = '\0';
		strcat(pDir, "\\LogelReader.ini");
		m_uiEdelay = GetPrivateProfileInt("SendInfo","Delay",DELAY_DEFAULT,m_szFileDir);
		m_uiEport = GetPrivateProfileInt("SendInfo","Port",SEND_PORT_DEFAULT,m_szFileDir);
		m_uiEsize = GetPrivateProfileInt("SendInfo","Size",SIZE_DEFAULT,m_szFileDir);
		m_bSaveTmpFile = GetPrivateProfileInt("SaveTmpFile", "Save", 0, m_szFileDir);
	}
	else
	{
		m_uiEdelay = 0;
 		m_uiEport  = 0;
 		m_uiEsize  = 0;
		m_strDevice = "G";
	}
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

#define WM_LOGEL_RENEWLOG   (WM_USER + 5001)  // Notify Logel to renew a log

#define ITC_TOOL_NAME_LEN 32

typedef struct tag_ITC_Req_T
{
	DWORD dwReqType;	// Request type
	char szToolName[ITC_TOOL_NAME_LEN];	// target tool name
}ITC_Req_T;

typedef struct Protocol_Header_Tag
{
    DWORD        sequenceNum;    //Protocol Packet Sequence Number
    WORD        length;         //Protocol Packet Length
    BYTE        type;           //Protocol Packet type
    BYTE        subtype;        //Protocol Packet subtype
}Protocol_Header,*Protocol_Header_Ptr;

#define PROTOCOL_HEADER_SIZE   sizeof(Protocol_Header)

#define ITC_REQ_TYPE	209
enum ITC_Req_Type_E
{
	Reg_Tool,
	Unreg_Tool,
	Query_Tool,
	Logel_ClearLog,
	Logel_SaveLog,
	Logel_Rec_keynote,
	Script_Resume,
};

#define ITC_REQ_SUBTYPE 100
#define ITC_REP_SUBTYPE 101

static BOOL CALLBACK LogelRelateCallBack(HWND hwnd,LPARAM lParam)
{	
    char szTitle[MAX_PATH];
    ::GetWindowText( hwnd, szTitle, MAX_PATH );
	
    if(strstr(szTitle,"Spreadtrum Logel"))
    {
		COPYDATASTRUCT cs;
		char szTime[ MAX_PATH] = {0};
		ltoa( lParam, szTime, 10 );
		cs.dwData = WM_LOGEL_RENEWLOG;
		cs.cbData = 0;
		cs.lpData = NULL;
		SendMessage( hwnd, WM_COPYDATA,NULL,LPARAM(&cs));
        return FALSE;
    }
	
    return TRUE;
}

void CDeviceSendDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == TIMER_EVENT)
	{
		if(m_bFlag)
		{
			if(!feof(m_fpData))
			{
				if(m_dwTotalData > 0)
				{
					if(m_dwTotalData > 1024 * m_uiEsize)
					{
						m_dwSendLen = 1024 * m_uiEsize;
					}
					else
					{
						m_dwSendLen = m_dwTotalData;
					}
					READ_FILE(m_pBuffer, m_dwSendLen, 1, m_fpData);
					if (!(ferror(m_fpData)))
					{
						m_dllSocObj.m_pCommhandler->Write(m_pBuffer, m_dwSendLen);
						m_dwTotalData -= m_dwSendLen;
					}
				}	
				else  //end
				{
					if(m_nSendCount < m_nLogCount)
					{
						TRACE("--------aaaa\n");
						Sleep( 2000 );
						/*
						int nLength = PROTOCOL_HEADER_SIZE + sizeof(ITC_Req_T) ;
						LPBYTE lpBuf = new BYTE[nLength];
						Protocol_Header ph;
						ph.type = ITC_REQ_TYPE;
						ph.subtype = ITC_REQ_SUBTYPE;
						ph.length = PROTOCOL_HEADER_SIZE + sizeof(ITC_Req_T);
						
						ITC_Req_T ItcReq;
						memset(&ItcReq, 0, sizeof(ITC_Req_T));
						ItcReq.dwReqType = Logel_ClearLog;
						_tcscpy(ItcReq.szToolName, "logel");
						//lpBuf[0]= 0x7E;
						memcpy(lpBuf , &ph, PROTOCOL_HEADER_SIZE);
						memcpy(lpBuf + PROTOCOL_HEADER_SIZE, &ItcReq, sizeof(ITC_Req_T));
						//lpBuf[nLength -1]= 0x7E;
						m_dllSocObj.m_pCommhandler->Write(lpBuf, nLength);	
						*/
						//::EnumDesktopWindows(NULL,LogelRelateCallBack,0);
						BYTE rebuf[11] = {0x7e,0x7e,0,0,0,0,0,0,MSG_RENEW_LOG,0,0x7e};
						m_dllSocObj.m_pCommhandler->Write(rebuf, 11);	
						Sleep( 2000 );
					}
					m_nSendCount ++;
					m_dwTotalData = m_dwSize[m_nSendCount];
				}
			}
			else
			{
				KillTimer(TIMER_EVENT);
				fclose(m_fpData);
				SetListStr("Send Over!");
				if(m_pBuffer != NULL)
				{
					delete [] m_pBuffer;
					m_pBuffer = NULL;
				}
				m_dwTotalData = 0;
				m_nLogCount = 0;
				m_nSendCount = 0;
				m_dwSendLen = 0;
				m_bFlag = FALSE;
				GetDlgItem(IDC_SIZE_EDT)->EnableWindow(TRUE);
				GetDlgItem(IDC_DELAY_EDT)->EnableWindow(TRUE);
				CString str;
				str.LoadString(IDS_STRING_BUTTON_SEND);
				GetDlgItem(IDC_SEND_BTN)->SetWindowText((LPCTSTR )str);
				UpdateData(FALSE);
			}
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

BOOL CDeviceSendDlg::MapToBuf()
{
	m_pFile = fopen(m_strPathName, "rb");
	if(m_pFile == NULL)
	{
		SetListStr("Can not open file.\n");
		return FALSE;
	}
	return TRUE;
}

void CDeviceSendDlg::GetLogData(int nSecSize, DWORD nSecStartPos, int nSecNum)
{
	DWORD nBlockSize = nSecSize * SECT_SIZE;
	BOOL bOneLog = TRUE;
	size_t size = sizeof(DWORD);
	m_dwSize = new DWORD[nSecNum];
	memset(m_dwSize, 0, nSecNum * sizeof(DWORD));
	SEGMENTINFO_T segmentInfo;
	SEGMENTINFO_T firSegmentInfo;
	m_dwStartSecPos = DISKINFO_SIZE * nSecStartPos;
	SEEK_FILE(m_pFile, m_dwStartSecPos, SEEK_SET);
	READ_FILE(&firSegmentInfo, SEGMENTINFO_SIZE, 1, m_pFile);

	DWORD dwFirGroupFlag = firSegmentInfo.groupFlag;
	DWORD dwFirSN = firSegmentInfo.SN;

	DWORD dwMinGFlag = dwFirGroupFlag;
	DWORD dwMinSN = dwFirSN;
	int nSignGroup = 0;
	int nSignSN = 0;

	m_fpData = NULL;
	m_fpData = fopen(m_tmpFile, "w+b");

	//find for the start pos
	for( int i = 0; i < nSecNum; i++)
	{
		SEEK_FILE(m_pFile, m_dwStartSecPos + i * nBlockSize, SEEK_SET);
		READ_FILE(&segmentInfo, SEGMENTINFO_SIZE, 1, m_pFile);
		if(segmentInfo.groupFlag == 0xFFFFFFFF)
		{
			break;
		}
		DWORD dwCurGFlag = segmentInfo.groupFlag;
		DWORD dwCurSN = segmentInfo.SN;
		
		if(dwCurGFlag <= dwMinGFlag)
		{
			dwMinGFlag = dwCurGFlag;
			nSignGroup = i;
		}
		if(dwCurSN <= dwMinSN)
		{
			dwMinSN = dwCurSN;
			nSignSN = i;
		}
		if(dwCurGFlag != dwFirGroupFlag)
		{
			bOneLog = FALSE;
		}
	}
	int nSign = 0;
	int nLastGFlag = dwMinGFlag;
	if(bOneLog)
	{
		nSign = nSignSN;
	}
	else
	{
		nSign = nSignGroup;
	}
	BYTE buf[BUF_SIZE];
	for( i = nSign - 1; i < nSecNum; i++)
	{
		DWORD dwStartPos = m_dwStartSecPos + i * nBlockSize;
		SEEK_FILE(m_pFile, dwStartPos, SEEK_SET);
		READ_FILE(&segmentInfo, SEGMENTINFO_SIZE, 1, m_pFile);
		
		if(segmentInfo.groupFlag == 0xFFFFFFFF)
		{
			break;
		}
		int nCurGFlag = segmentInfo.groupFlag;
		SEEK_FILE(m_pFile, dwStartPos + segmentInfo.dataStartPos * SECT_SIZE, SEEK_SET);
		DWORD wtSize = segmentInfo.dataSize;
		while(wtSize > 0)
		{
			DWORD nLen = 0;
			if(wtSize > BUF_SIZE)
			{
				nLen = BUF_SIZE;
			}
			else
			{
				nLen = wtSize;
			}
			READ_FILE(&buf, nLen, 1, m_pFile);
			fwrite(&buf, nLen, 1, m_fpData);
			wtSize -= nLen;
		}
		if(nLastGFlag == nCurGFlag)
		{
			m_dwSize[m_nLogCount] += segmentInfo.dataSize;
		}
		else
		{
			m_nLogCount ++;
			m_dwSize[m_nLogCount] = segmentInfo.dataSize;
		}
		nLastGFlag = nCurGFlag;
	}

	if(nSign != 0)
	{
		for( i = 0; i < nSign - 1; i++)
		{
			DWORD dwStartPos = m_dwStartSecPos + i * nBlockSize;
			SEEK_FILE(m_pFile, dwStartPos, SEEK_SET);
			READ_FILE(&segmentInfo, SEGMENTINFO_SIZE, 1, m_pFile);
			
			if(segmentInfo.groupFlag == 0xFFFFFFFF)
			{
				break;
			}
			int nCurGFlag = segmentInfo.groupFlag;
			SEEK_FILE(m_pFile, dwStartPos + segmentInfo.dataStartPos * SECT_SIZE, SEEK_SET);
			DWORD wtSize = segmentInfo.dataSize;
			while(wtSize > 0)
			{
				DWORD nLen = 0;
				if(wtSize > BUF_SIZE)
				{
					nLen = BUF_SIZE;
				}
				else
				{
					nLen = wtSize;
				}
				READ_FILE(&buf, nLen, 1, m_pFile);
				fwrite(&buf, nLen, 1, m_fpData);
				wtSize -= nLen;
			}
			
			if(nLastGFlag == nCurGFlag)
			{
				m_dwSize[m_nLogCount] += segmentInfo.dataSize;
			}
			else
			{
				m_nLogCount ++;
				m_dwSize[m_nLogCount] = segmentInfo.dataSize;
			}
			nLastGFlag = nCurGFlag;
		}
	}
	m_dwTotalData = m_dwSize[0];
	fclose(m_fpData);
	CLOSE_FILE(m_pFile);
	//CLOSE_FILE(m_hResult);
	TRACE("-------------%d\n", m_nLogCount);
	
	
}

BOOL CDeviceSendDlg::LoadData()
{
	DISKINFO_T diskInfo;
	READ_FILE(&diskInfo, DISKINFO_SIZE, 1, m_pFile);
	if(_tcscmp((const char *)diskInfo.OEMname, "SPREADTRUM") == 0)
	{
		GetLogData(diskInfo.segmentSize, diskInfo.segmentStartPos, diskInfo.segmentCount);	
		return TRUE;
	}
	else
	{
		SetListStr("Invalidate SD card!");
		return FALSE;
	}
}

BOOL CDeviceSendDlg::OpenDiskAndRead()
{
	UpdateData();
	TCHAR szName[MAX_PATH] = {0};
	sprintf(szName, "\\\\.\\%s:", m_strDevice);
	m_hResult = ::CreateFile(szName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if(m_hResult == INVALID_HANDLE_VALUE)
	{
		DWORD dwErr = GetLastError();
		char  wszMsgBuff[512];
		FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwErr,
			0,
			wszMsgBuff,
			200,
			NULL );

		SetListStr("Failed to open disk!");
		return FALSE;
	}
	return TRUE;
}

void CDeviceSendDlg::SetListStr(LPCTSTR lpStr)
{
	m_mylist.AddString(lpStr);
	int nCount = m_mylist.GetCount();
	m_mylist.SetTopIndex(nCount - 1);
	if (nCount > 0)
	{
		m_mylist.SetCurSel(nCount-1);
	}
}

BOOL CDeviceSendDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd * pWnd = GetDlgItem(IDC_DISK_EDT);
    _ASSERTE( pWnd != NULL );

    CWnd * pFocus = GetFocus();
    if( pMsg->message == WM_CHAR && pFocus != NULL && pFocus->m_hWnd == pWnd->m_hWnd )
    {
        _TCHAR cInput = (_TCHAR)pMsg->wParam;
		if( cInput < _T('a') || cInput > _T('z') )
		{
			if( cInput != _T('_') && (cInput < _T('A') || cInput > _T('Z')) && cInput != 8 /* backspace*/ )
			{
				if( !(GetKeyState(VK_LCONTROL) & 0x0100) )
					return TRUE;
			}
		}
    }

	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE ||pMsg->wParam == VK_RETURN  )//ESC or Enter
		{
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CDeviceSendDlg::ReleaseResource()
{
	if (m_pBuffer != NULL)
	{
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}
	if(m_dwSize != NULL)
	{
		delete [] m_dwSize;
		m_dwSize = NULL;
	}
	if (m_uiTimeEvent!=0)
	{
		KillTimer(TIMER_EVENT);
	} 
	if(m_fpData != NULL)
	{
		fclose(m_fpData);
		if(!m_bSaveTmpFile)
		{
			DeleteFile(m_tmpFile);
		}
	}
}

void CDeviceSendDlg::OnOpen() 
{
	// TODO: Add your control notification handler code here
	const char strFilter[]  = "Information file (*.img)|*.img||";
	const char strCaption[] = "Open";
	// Get the path name

	CFileDialog dlg(TRUE,
					"(*.img)|*.img||",
					NULL,
					OFN_HIDEREADONLY| OFN_FILEMUSTEXIST,
					strFilter);

 	dlg.m_ofn.lpstrTitle = strCaption;
	if (dlg.DoModal() != IDOK)

		return;          
	m_strPathName = dlg.GetPathName();
	m_mylist.AddString(m_strPathName);
	UpdateData(FALSE);
	(CButton*)GetDlgItem(IDC_LOAD_BTN)->EnableWindow(TRUE);
}

void CDeviceSendDlg::GetFileName()
{
	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);

	if (GetModuleFileName(NULL, m_tmpFile,MAX_PATH)!=0)
	{
		char *pDir = strrchr(m_tmpFile, '\\');
		ASSERT(NULL != pDir);
		*pDir = '\0';
	}
	TCHAR szFile[MAX_PATH] = {0};
	sprintf(szFile, "\\SDCard_%d_%d_%d_%d_%d.log", sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
	strcat(m_tmpFile, szFile);
}
