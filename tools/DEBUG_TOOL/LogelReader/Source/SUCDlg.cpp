// SUCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SUC.h"
#include "SUCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSUCDlg dialog

CSUCDlg::CSUCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSUCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSUCDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSUCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSUCDlg)
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSUCDlg, CDialog)
	//{{AFX_MSG_MAP(CSUCDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSUCDlg message handlers

BOOL CSUCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (GetModuleFileName(NULL, m_szFileDir,MAX_PATH)!=0)
	{
		char *pDir = strrchr(m_szFileDir, '\\');
		ASSERT(NULL != pDir);
		*pDir = '\0';
		strcat(pDir, "\\LogelReader.ini");
	}

	m_pTab1Dlg=new CTab1Dlg();
	m_pTab2Dlg=new CTab2Dlg();
	m_pDeviceDlg = new CDeviceSendDlg();

	m_pTab1Dlg->Create(IDD_SEND_DIALOG,&m_Tab);
	m_pTab2Dlg->Create(IDD_RECEIVE_DIALOG,&m_Tab);
	m_pDeviceDlg->Create(IDD_DEVICE_DLG, &m_Tab);

	GetDisplayPage();

	m_Tab.SetMinTabWidth(-1);
	m_Tab.SetCurSel(0);
	
	SetPage();
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSUCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSUCDlg::OnPaint() 
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
HCURSOR CSUCDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CSUCDlg::SetDlgState(CWnd *pWnd, BOOL bShow)
{
	pWnd->EnableWindow(bShow);
	if (bShow)
	{
		pWnd->ShowWindow(SW_SHOW);
		pWnd->CenterWindow();
	} 
	else  pWnd->ShowWindow(SW_HIDE);
}

void CSUCDlg::DoTab(int nTab)
{
	if(nTab>2)nTab=2;
	if(nTab<0)nTab=0;

	BOOL bTab[3];
	bTab[0]=bTab[1]=bTab[2]=FALSE;
	bTab[nTab]=TRUE;

	SetDlgState(m_pTab1Dlg,bTab[0]);
	SetDlgState(m_pTab2Dlg,bTab[1]);
	SetDlgState(m_pDeviceDlg, bTab[2]);
}

void CSUCDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SetPage();
	*pResult = 0;
}

void CSUCDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (AfxMessageBox(IDS_STRING_SUC_CLOSE,MB_YESNO)==IDNO)
	{
		return;
	}
	if(m_pTab1Dlg) 
	{
		m_pTab1Dlg->m_dllSocObj.UnloadCommHandler();
		delete m_pTab1Dlg;
		CDialog::OnDestroy();
	}
	if(m_pTab2Dlg) 
	{
		m_pTab2Dlg->m_dllObj.UnloadCommHandler();
		delete m_pTab2Dlg;
		CDialog::OnDestroy();
	}
	if(m_pDeviceDlg)
	{
		m_pDeviceDlg->ReleaseResource();
		m_pDeviceDlg->m_dllSocObj.UnloadCommHandler();
		delete m_pDeviceDlg;
		CDialog::OnDestroy();
	}
}

void CSUCDlg::GetDisplayPage()
{
	BOOL bShow = 0;
	CString str;
	
	bShow = GetPrivateProfileInt("DisplayPage","Send",0,m_szFileDir);
	if(bShow == 1)
	{
		str.LoadString(IDS_STRING_BUTTON_SEND);
		m_Tab.InsertItem(0,(LPCTSTR)str);
		DoTab(0);
	}

	bShow = GetPrivateProfileInt("DisplayPage","Receive",0,m_szFileDir);
	if(bShow == 1)
	{
		str.LoadString(IDS_STRING_RECEIVE);
		m_Tab.InsertItem(1,(LPCTSTR)str);
		DoTab(1);
	}

	bShow = GetPrivateProfileInt("DisplayPage","SDCardSend",1,m_szFileDir);
	if(bShow == 1)
	{
		str.LoadString(IDS_STRING_DEVICE);
		m_Tab.InsertItem(2,(LPCTSTR)str);
		DoTab(2);
	}
}

void CSUCDlg::SetPage()
{
	TCITEM item;
	memset(&item, 0, sizeof(TCITEM));
	item.mask = TCIF_TEXT;
	item.pszText = new TCHAR[MAX_PATH];
	item.cchTextMax = MAX_PATH - 1;
	int nSelect = m_Tab.GetCurSel();
	if(nSelect >= 0) 
	{
		m_Tab.GetItem(nSelect, &item);
		if(_tcscmp(item.pszText, "Send") == 0)
		{
			DoTab(0);
		}
		if(_tcscmp(item.pszText, "Receive") == 0)
		{
			DoTab(1);
		}
		if(_tcscmp(item.pszText, "SDCardSend") == 0)
		{
			DoTab(2);
		}
		
	}
	if(item.pszText != NULL)
	{
		delete [] item.pszText;
	}
}

extern CSUCApp theApp;
BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem( IDC_STATIC_VERSION )->SetWindowText( theApp.m_strVersion );
    GetDlgItem( IDC_STATIC_BUILD )->SetWindowText( theApp.m_strBuild );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
