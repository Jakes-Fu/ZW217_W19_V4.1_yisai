// DataView.cpp : implementation file
//

#include "stdafx.h"
#include "channelserver.h"
#include "DataView.h"
#include "Global.h"
#include "Function.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataView dialog


CDataView::CDataView(CWnd* pParent /*=NULL*/)
	: CDialog(CDataView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataView)
	//}}AFX_DATA_INIT
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    // Initializations
    m_hWhiteBrush   = ::CreateSolidBrush(RGB(255, 255, 255));

    if (GetSafeHwnd() == NULL)
    {
        Create(IDD_DLG_DATA_LOG);            
    }
 
    m_bEnableShow = FALSE;
    m_fpToolToCSLog = NULL;
    m_fpCSToMSLog = NULL;
    m_fpMSToCSLog = NULL;
    m_fpCSToToolLog = NULL;

	m_bHideArmLog = FALSE;
}

CDataView::~CDataView()
{
    if(m_fpToolToCSLog != NULL)
    {
        fclose(m_fpToolToCSLog);
        m_fpToolToCSLog = NULL;
    }
    if(m_fpCSToMSLog != NULL)
    {
        fclose(m_fpCSToMSLog);
        m_fpCSToMSLog = NULL;
    }
    if(m_fpMSToCSLog != NULL)
    {
        fclose(m_fpMSToCSLog);
        m_fpMSToCSLog = NULL;
    }
    if(m_fpCSToToolLog != NULL)
    {
        fclose(m_fpCSToToolLog);
        m_fpCSToToolLog = NULL;
    }
    
    ::DeleteObject(m_hWhiteBrush);
    DestroyWindow();
}

void CDataView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataView)
	DDX_Control(pDX, IDC_TOOL_CS_BUFFER, m_wndToolToCSBuffer);
	DDX_Control(pDX, IDC_CS_TOOL_BUFFER, m_wndCSToToolBuffer);
	DDX_Control(pDX, IDC_MS_CS_BUFFER, m_wndMSToCSBuffer);
	DDX_Control(pDX, IDC_CS_MS_BUFFER, m_wndCSToMSBuffer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataView, CDialog)
	//{{AFX_MSG_MAP(CDataView)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_CLEAR, OnBtnClear)
	ON_BN_CLICKED(IDC_HIDE_ARMLOG, OnHideArmlog)
	//}}AFX_MSG_MAP
    /*lint -save -e665 */
    ON_MESSAGE(WM_SHOW_DATA, OnShowData)
    /*lint -restore */
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataView message handlers
/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg message handlers


BOOL CDataView::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

    ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);

    //Open Log File
    char AppPath[MAX_PATH];

    GetAppPath(AppPath);

    CString szTool_CS_FileName = _T(AppPath);
    szTool_CS_FileName         += "\\Tool_To_CS.log";
        
    m_fpToolToCSLog  = NULL;
    m_fpToolToCSLog = fopen(szTool_CS_FileName.operator LPCTSTR (), "w");
    //ASSERT(m_fpToolLog != NULL);
                     
    CString szCS_MS_FileName = _T(AppPath);
    szCS_MS_FileName         += "\\CS_To_MS.log";
    m_fpCSToMSLog    = NULL;    
    m_fpCSToMSLog    = fopen(szCS_MS_FileName.operator LPCTSTR (), "w");
    //ASSERT(m_fpMSLog != NULL);

    
                     
    CString szMS_CS_FileName = _T(AppPath);
    szMS_CS_FileName         += "\\MS_To_CS.log";
    m_fpMSToCSLog    = NULL;    
    m_fpMSToCSLog    = fopen(szMS_CS_FileName.operator LPCTSTR (), "w");

    CString szCS_Tool_FileName = _T(AppPath);
    szCS_Tool_FileName         += "\\CS_To_Tool.log";        
    m_fpCSToToolLog            = NULL;
    m_fpCSToToolLog            = fopen(szCS_Tool_FileName.operator LPCTSTR (), "w");
    //ASSERT(m_fpToolLog != NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataView::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDataView::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDataView::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

HBRUSH CDataView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
    
    if (pWnd->GetDlgCtrlID() == IDC_TOOL_CS_BUFFER)
    {
        pDC->SetBkColor(RGB(255, 255, 255));
        pDC->SetTextColor(RGB(0, 0, 255));
        return m_hWhiteBrush;
    }
    else if (pWnd->GetDlgCtrlID() == IDC_CS_TOOL_BUFFER)
    {
        pDC->SetBkColor(RGB(255, 255, 255));
        pDC->SetTextColor(RGB(255, 0, 0));
        return m_hWhiteBrush;
    }
    else if (pWnd->GetDlgCtrlID() == IDC_CS_MS_BUFFER)
    {
        pDC->SetBkColor(RGB(255, 255, 255));
        pDC->SetTextColor(RGB(150, 30, 150));
        return m_hWhiteBrush;
    }
	else if (pWnd->GetDlgCtrlID() == IDC_MS_CS_BUFFER)
    {
        pDC->SetBkColor(RGB(255, 255, 255));
        pDC->SetTextColor(RGB(0, 0, 0));
        return m_hWhiteBrush;
    }
    

	// TODO: Return a different brush if the default is not desired
    return hbr;
}

BOOL CDataView::PreTranslateMessage(MSG* pMsg) 
{
// TODO: Add your specialized code here and/or call the base class

    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
        return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDataView::OnShowData(WPARAM wParam, LPARAM lParam)
{
    COMMONMSG_PTR       pRecvData;
    Protocol_Header_Ptr pRecvPktHeader;

    char          *szHexData = NULL;
    int           nHexDataSize;
    SYSTEMTIME    szTime;
    
    GetLocalTime(&szTime);

    pRecvData      = (COMMONMSG_PTR)wParam;
    pRecvPktHeader = (Protocol_Header_Ptr)pRecvData->lpData ;

	if( m_bHideArmLog )
	{
		// do not display arm log and trace data
		if( pRecvPktHeader->type == DIAG_LOG_A )
		{
			return;
		}
	}

    switch(lParam)
    {
    case MSG_TOOL_TO_CS:        
        szHexData = CHexEdit::BufferToString (pRecvData->lpData, pRecvData->nDataLen, 16, &nHexDataSize);
        if(m_bEnableShow)
        {
            m_wndToolToCSBuffer.DisplayBuffer (szHexData, nHexDataSize);
        }
        if(m_fpToolToCSLog != NULL)
        {
            fprintf(m_fpToolToCSLog,"MSG_TOOL_TO_CS: %d-%d-%d %d:%d:%.2d DataLength = %d\r\n%s\r\n\r\n",
                szTime.wYear ,szTime.wMonth ,szTime.wDay ,szTime.wHour ,
                szTime.wMinute ,szTime.wSecond ,pRecvData->nDataLen,szHexData);
            fflush(m_fpToolToCSLog);
        }        
        break;
    case MSG_CS_TO_MS:        
        szHexData = CHexEdit::BufferToString (pRecvData->lpData, pRecvData->nDataLen, 16, &nHexDataSize);
        if(m_bEnableShow)
        {
            m_wndCSToMSBuffer.DisplayBuffer (szHexData, nHexDataSize);
        }
        if(m_fpCSToMSLog != NULL)
        {
            fprintf(m_fpCSToMSLog,"MSG_CS_TO_MS: %d-%d-%d %d:%d:%.2d DataLength = %d\r\n%s\r\n\r\n",
                szTime.wYear ,szTime.wMonth ,szTime.wDay ,szTime.wHour ,
                szTime.wMinute ,szTime.wSecond ,pRecvData->nDataLen,szHexData);
            fflush(m_fpCSToMSLog);
        }        
        break;
    case MSG_MS_TO_CS:        
        szHexData = CHexEdit::BufferToString (pRecvData->lpData, pRecvData->nDataLen, 16, &nHexDataSize);
        if(m_bEnableShow)
        {
            m_wndMSToCSBuffer.DisplayBuffer (szHexData, nHexDataSize);
        }
        if(m_fpMSToCSLog != NULL)
        {
            fprintf(m_fpMSToCSLog,"MSG_MS_TO_CS: %d-%d-%d %d:%d:%.2d DataLength = %d\r\n%s\r\n\r\n",
                szTime.wYear ,szTime.wMonth ,szTime.wDay ,szTime.wHour ,
                szTime.wMinute ,szTime.wSecond ,pRecvData->nDataLen,szHexData);
            fflush(m_fpMSToCSLog);
        }        
        break;
    case MSG_CS_TO_TOOL:        
        szHexData = CHexEdit::BufferToString (pRecvData->lpData, pRecvData->nDataLen, 16, &nHexDataSize);
        if(m_bEnableShow)
        {
            m_wndCSToToolBuffer.DisplayBuffer (szHexData, nHexDataSize);
        }
        if(m_fpCSToToolLog != NULL)
        {            
            fprintf(m_fpCSToToolLog,"MSG_CS_TO_TOOL: %d-%d-%d %d:%d:%.2d DataLength = %d\r\n%s\r\n\r\n",
                szTime.wYear ,szTime.wMonth ,szTime.wDay ,szTime.wHour , 
                szTime.wMinute ,szTime.wSecond ,pRecvData->nDataLen,szHexData);
            
            fflush(m_fpCSToToolLog);
        }        
        break;
    default:
        break;
    }

    if(NULL != szHexData)
    {
        delete szHexData;
    }
}


void CDataView::OnBtnClear() 
{
	// TODO: Add your control notification handler code here
	m_wndToolToCSBuffer.Clear ();
    m_wndCSToMSBuffer.Clear ();
    m_wndMSToCSBuffer.Clear ();
    m_wndCSToToolBuffer.Clear ();
}


void CDataView::OnHideArmlog() 
{
	m_bHideArmLog = !m_bHideArmLog;	
}
