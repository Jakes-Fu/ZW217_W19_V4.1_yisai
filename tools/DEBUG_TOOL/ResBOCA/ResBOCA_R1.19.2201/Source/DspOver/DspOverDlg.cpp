// DspOverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DspOver.h"
#include "DspOverDlg.h"
#include "../bzp/BZip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define  SAFE_DEL(p) do{ \
	if(NULL != p) \
{ \
	delete [] p; \
	p=NULL;\
} \
}while(0)
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
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDspOverDlg dialog

CDspOverDlg::CDspOverDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDspOverDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDspOverDlg)
	m_strFullFile = _T("");
	m_strDspFile = _T("");
	m_strOutFile = _T("");
	m_nCBCmpType = 1 ; 
	m_nCmpLevel = 2;
	m_bOverExtPart = FALSE;
	//}}AFX_DATA_INIT
	bDebugOn = false;
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDspOverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDspOverDlg)
	DDX_Control(pDX, IDC_CMB_EXT_PARTITION, m_cbExtTbl);
	DDX_Text(pDX, IDC_EDIT_INTEGRATED_FILE, m_strFullFile);
	DDX_Text(pDX, IDC_EDIT_DSP_FILE, m_strDspFile);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_strOutFile);
	DDX_CBIndex(pDX, IDC_CB_COMPRESSED_TYPE, m_nCBCmpType);
	DDX_Text(pDX, IDC_EDIT_COMPRESSED_LEVEL, m_nCmpLevel);
	DDV_MinMaxUInt(pDX, m_nCmpLevel, 0, 9);
	//DDV_MaxChars(pDX, m_nCmpLevel, 1);
	DDX_Check(pDX, IDC_CHK_EXT_PARTITION, m_bOverExtPart);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDspOverDlg, CDialog)
	//{{AFX_MSG_MAP(CDspOverDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_COMPARE, OnBtnOverWrite)
	ON_EN_UPDATE(IDC_EDIT_INTEGRATED_FILE, OnUpdateEditIntegratedFile)
	ON_EN_UPDATE(IDC_EDIT_DSP_FILE, OnUpdateEditDspFile)
	ON_EN_UPDATE(IDC_EDIT_OUTPUT, OnUpdateEditOutput)
	ON_BN_CLICKED(IDC_CHK_EXT_PARTITION, OnChkExtPartition)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_BTN_INTEGRATED, IDC_BTN_OUTPUT, OnBtnInputFile)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDspOverDlg message handlers

BOOL CDspOverDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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
	TCHAR szPathName[MAX_PATH] = {0};
	::GetModuleFileName( NULL, szPathName, _MAX_PATH );
	CString strConfigFile = szPathName;

	strConfigFile = strConfigFile.Left(strConfigFile.ReverseFind(_T('\\')) + 1);
	strConfigFile += _T("DspOver.ini");

	bDebugOn = 	GetPrivateProfileInt(_T("SETTING"),_T("debugon"),0,strConfigFile) == 1 ? true : false;
	GetDlgItem(IDC_CB_COMPRESSED_TYPE)->EnableWindow(bDebugOn);
	GetDlgItem(IDC_EDIT_COMPRESSED_LEVEL)->EnableWindow(bDebugOn);
	GetDlgItem(IDC_CHK_EXT_PARTITION)->EnableWindow(FALSE);
	m_cbExtTbl.EnableWindow(m_bOverExtPart);
	m_cbExtTbl.SetCurSel(0);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDspOverDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDspOverDlg::OnPaint() 
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
HCURSOR CDspOverDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDspOverDlg::OnBtnInputFile(UINT nID) 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	static _TCHAR szFilter[] = _T("BIN Files (*.bin)|*.bin|");
	BOOL bOpenFileDialog = TRUE;
	if (IDC_BTN_OUTPUT == nID)
	{
		bOpenFileDialog = FALSE;
	}
	CFileDialog fdlg(bOpenFileDialog, _T("bin"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	CBZip cBzp;
    if( fdlg.DoModal() != IDOK )
    {
		return;
	}
	switch(nID)
	{
	case IDC_BTN_INTEGRATED:
		m_strFullFile = fdlg.GetPathName();
		GetDlgItem(IDC_CHK_EXT_PARTITION)->EnableWindow(cBzp.IsExtendPtnTbl(m_strFullFile));
		break;
	case IDC_BTN_DSP:
		m_strDspFile = fdlg.GetPathName();
		break;
	case IDC_BTN_OUTPUT:
		m_strOutFile = fdlg.GetPathName();
		break;
	default:
		break;
	}

	UpdateData(false);
}

void CDspOverDlg::OnBtnOverWrite() 
{
	// TODO: Add your control notification handler code here
	CFileFind finder;
	UpdateData();
	if (m_strFullFile.IsEmpty() || m_strDspFile.IsEmpty() )
	{
		AfxMessageBox(_T("Integrated file and Dsp file can not be empty!"));
		return;
	}
	if (m_strOutFile.IsEmpty() )
	{
		AfxMessageBox(_T("Out file can not be empty!"));
		return;
	}
	if(!m_strFullFile.IsEmpty() )
	{
		if(!finder.FindFile(m_strFullFile))
		{
			AfxMessageBox(_T("Cannot find integrated file!"));
			return;
		}
		finder.Close();
		
	}
	if(!m_strDspFile.IsEmpty() )
	{
		if(!finder.FindFile(m_strDspFile))
		{
			AfxMessageBox(_T("Cannot find dsp file!"));
			return;
		}
		finder.Close();
	}
	CBZip cBzp;
	CWaitCursor wait; 
	_TCHAR szPeelDspZfile[MAX_PATH*2] = {0};
	_TCHAR szName[MAX_PATH] = {0};
	BZP_PARAM_T param = {0};
	int nPtnTblIndex = 3;
	
	GetTempPath(MAX_PATH*2,szPeelDspZfile);
	_stprintf(szName,_T("PeelDspZ%d.bin"),GetTickCount());
	_tcscat(szPeelDspZfile,szName);

	nPtnTblIndex = m_bOverExtPart ?m_cbExtTbl.GetCurSel()+5 : 3;

	if (!cBzp.RemovePartiton(m_strFullFile,szPeelDspZfile,nPtnTblIndex))
	{
		AfxMessageBox(_T("Splited bzp file failed!"));
		return;
	}
	cBzp.GetLimitParam(param);
	param.nCmpType = m_nCBCmpType;
	param.nLevel = m_nCmpLevel;
	cBzp.SetLimitParam(param);
	
	if (!cBzp.OverWtirePartiton(szPeelDspZfile,m_strDspFile,nPtnTblIndex,m_strOutFile))	
	{
		AfxMessageBox(_T("OverWrite file failed!"));
		return;
	}

	
/*	if (!m_bOverExtPart)
	{
		if (!cBzp.RemoveDspZFile(m_strFullFile,szPeelDspZfile))
		{
			AfxMessageBox(_T("Splited dspbzp file failed!"));
			return;
		}
		cBzp.GetLimitParam(param);
		param.nCmpType = m_nCBCmpType;
		param.nLevel = m_nCmpLevel;
		cBzp.SetLimitParam(param);
		
		if (!cBzp.OverWtireDspZ(szPeelDspZfile,m_strDspFile,m_strOutFile))	
		{
			AfxMessageBox(_T("OverWrite dspz file failed!"));
			return;
		}
	}
	else
	{
		if (!cBzp.RemoveExtZFile(m_strFullFile,szPeelDspZfile,m_cbExtTbl.GetCurSel()))
		{
			AfxMessageBox(_T("Splited Extz file failed!"));
			return;
		}
		cBzp.GetLimitParam(param);
		param.nCmpType = m_nCBCmpType;
		param.nLevel = m_nCmpLevel;
		cBzp.SetLimitParam(param);
		
		if (!cBzp.OverWtireExtZ(szPeelDspZfile,m_strDspFile,m_cbExtTbl.GetCurSel(),m_strOutFile) )	
		{
			AfxMessageBox(_T("OverWrite Extz file failed!"));
			return;
		}
	}
	*/
	DeleteFile(szPeelDspZfile);
	wait.Restore();
	AfxMessageBox(_T("Gen new file Successfully!"));
}

void CDspOverDlg::OnUpdateEditIntegratedFile() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
//	UpdateData();
	GetDlgItem(IDC_EDIT_INTEGRATED_FILE)->GetWindowText(m_strFullFile);
}

void CDspOverDlg::OnUpdateEditDspFile() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_EDIT_DSP_FILE)->GetWindowText(m_strDspFile);
}
void CDspOverDlg::OnUpdateEditOutput() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
//	UpdateData();
	GetDlgItem(IDC_EDIT_OUTPUT)->GetWindowText(m_strOutFile);
}


void CDspOverDlg::OnChkExtPartition() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_cbExtTbl.EnableWindow(m_bOverExtPart);
	UpdateData(FALSE);
}
