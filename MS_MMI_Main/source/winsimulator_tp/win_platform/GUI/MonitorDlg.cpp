// MonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MSDev.h"
#include "MonitorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg dialog

CMonitorDlg::CMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonitorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMonitorDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    // Initializations
    m_hWhiteBrush   = ::CreateSolidBrush(RGB(255, 255, 255));
}

CMonitorDlg::~CMonitorDlg()
{
    ::DeleteObject(m_hWhiteBrush);
}

void CMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonitorDlg)
	DDX_Control(pDX, IDC_RTOS_BUFFER, m_wndRtosBuffer);
	DDX_Control(pDX, IDC_CLIENT_BUFFER, m_wndClientBuffer);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMonitorDlg, CDialog)
	//{{AFX_MSG_MAP(CMonitorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonitorDlg message handlers

BOOL CMonitorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

    ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMonitorDlg::OnPaint() 
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
HCURSOR CMonitorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

HBRUSH CMonitorDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
    
    if (pWnd->GetDlgCtrlID() == IDC_CLIENT_BUFFER)
    {
        pDC->SetBkColor(RGB(255, 255, 255));
        pDC->SetTextColor(RGB(0, 0, 255));
        return m_hWhiteBrush;
    }
    else if (pWnd->GetDlgCtrlID() == IDC_RTOS_BUFFER)
    {
        pDC->SetBkColor(RGB(255, 255, 255));
        pDC->SetTextColor(RGB(255, 0, 0));
        return m_hWhiteBrush;
    }
	
	// TODO: Return a different brush if the default is not desired
    return hbr;
}


// ----------------------------------------------------------------------------
// FUNCTION NAME:   DisplayBuffer
// AUTHOR:          Liu Jun
// INPUT:           pBuffer         ----    Buffer starting address
//                  nLength         ----    Buffer size
// RETURN:          None
// DESCRIPTION:     Display the buffer content in edit control
// ----------------------------------------------------------------------------
void CMonitorDlg::DisplayBuffer(BYTE *pBuffer, int nLength, CString strSrc)
{
    // Display message content
    if (strSrc == "Client")
        m_wndClientBuffer.DisplayBuffer(pBuffer, nLength);
    else if (strSrc == "RTOS")
        m_wndRtosBuffer.DisplayBuffer(pBuffer, nLength);
}


// ----------------------------------------------------------------------------
// FUNCTION NAME:   WriteLogFile
// AUTHOR:          Liu Jun
// INPUT:           strFileName     ----    Log file name
//                  pBuffer         ----    Buffer starting address
//                  nLength         ----    Buffer size
// RETURN:          None
// DESCRIPTION:     Write the received buffer to a binary log file for testing
// ----------------------------------------------------------------------------
void CMonitorDlg::WriteLogFile(CString strFileName, BYTE *pBuffer, int nLength)
{
    // Open the file for writing
    CFile logFile(strFileName, CFile::modeWrite | CFile::shareDenyWrite | 
                               CFile::modeCreate | CFile::modeNoTruncate);
    
    // Seek to the file end
    logFile.SeekToEnd();

    // Write the starting flag : "7F7F"
    char flag[] = "7F7F";
    logFile.Write(flag, sizeof(flag));

    // Write the buffer to file
    logFile.Write(pBuffer, nLength);
}


void CMonitorDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
    DestroyWindow();

	// CDialog::OnClose();
}

BOOL CMonitorDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
        return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}
