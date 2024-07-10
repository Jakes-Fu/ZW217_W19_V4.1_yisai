// TaskAnalyzerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TaskAnalyzer.h"
#include "TaskAnalyzerDlg.h"

  

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern unsigned long g_nMaxSwitchNum;
extern unsigned int g_percentMode;
extern char g_szDebugString[MAX_PATH];
unsigned long g_nTaskTableOffset;
unsigned long g_nTcbPtr[128];
CTaskAnalyzerDlg* g_pdlg = NULL;

BOOL g_bBigEndian = TRUE;

static int EnumKeys(char* pSection,CStringArray* pKeys)
{
    ASSERT(pSection != NULL);
    ASSERT(pKeys != NULL);

	if(pSection == NULL || pKeys == NULL)
	{
		return -1;
	}

    int number = 0;
    while(1)//lint !e716
    {
        CString strLine = pSection;
        int nLen = strLine.GetLength();
        if(0 == nLen)
        {
            // All keys are read
            break;
        }
        
        int nIndex = strLine.Find('=');
        if(-1 == nIndex)
        {
            // Not a valid key
            continue;
        }
        CString strKey = strLine.Left(nIndex);
        strKey.TrimLeft();
        strKey.TrimRight();
        pKeys->Add(strKey);

        CString strData = strLine.Right(nLen - nIndex - 1);
        strData.TrimLeft();
        strData.TrimRight();
        pKeys->Add(strData);

        number++;

        pSection += nLen + 1;

    }

    return number;
}

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
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskAnalyzerDlg dialog

CTaskAnalyzerDlg::CTaskAnalyzerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTaskAnalyzerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaskAnalyzerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(m_szIntList, 0, LIST_LEN);
	memset(m_szTaskList, 0, LIST_LEN);
	m_uiMaxFileSize = 0;
	m_uiDataAreaSize = 0;	
	m_uiShareMemorySize = 0;
	m_uiIramSize = 0;	
	m_uiRegSize = 0;
	m_uiOffSize = 0;
	m_uiShareOffset = 0;
	m_uiIramOffset = 0;	
	m_uiRegOffset = 0;	
	m_uiDspOffset = 0;	
	m_uiDspSize = 0;
	m_strErr = "";

	m_bNewVersion = FALSE;
	
	//teana hu 2010.08.30
	m_bBigEndian = TRUE;
	//
}

void CTaskAnalyzerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaskAnalyzerDlg)
	DDX_Control(pDX, IDC_COMBO_PRODUCT, m_cboProductList);
	DDX_Control(pDX, IDC_RADIO_BY_MS, m_btnByMs);
	DDX_Control(pDX, IDC_RADIO_BY_US, m_btnByUs);
	DDX_Text(pDX, IDC_DESCRIBE, m_strEdtDescribe);
	DDX_Control(pDX, IDC_LIST_RESULT, m_lstBox);
	DDX_Control(pDX, IDC_CHECK_ENDIAN, m_btnEndian);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTaskAnalyzerDlg, CDialog)
	//{{AFX_MSG_MAP(CTaskAnalyzerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_MAP_FILE_BTN, OnMapFileBtn)
	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCT, OnSelchangeComboProduct)
	ON_BN_CLICKED(IDC_CHANGE_CHECK, OnChangeCheck)
	ON_BN_CLICKED(IDC_SOURCE_FILE_BTN, OnSourceFileBtn)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK_ENDIAN, OnCheckEndian)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskAnalyzerDlg message handlers

BOOL CTaskAnalyzerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
//	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
//	ASSERT(IDM_ABOUTBOX < 0xF000);

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
	for( int i = 0 ; i < NUMERIC_EDIT_NUM; i++ )
	{
		m_editSetting[i].SubclassDlgItem( IDC_FILE_MAX_SIZE_EDIT + i ,this );
	}

	CString strIniFile;
    char szDirectory[MAX_PATH];	
	
    GetModuleFileName(NULL, szDirectory, MAX_PATH);
    strIniFile = szDirectory;
    strIniFile = strIniFile.Left(strIniFile.ReverseFind('\\') + 1);
	strIniFile += "TaskAnalyzer.ini";
	
	m_strIniFile = strIniFile;

	char szProduct[64];
    char szSection[1024];
	memset(szProduct, 0, 64);
    memset(szSection,0,1024);
    
    ::GetPrivateProfileString("TaskSwitchSetting","SelectProduct",NULL,szProduct,64,m_strIniFile );
	m_strSelectProduct =  szProduct;   

    ::GetPrivateProfileSection( "ProductionList",szSection,1024,m_strIniFile );  
	
	m_bBigEndian = GetPrivateProfileInt( "Setting", "BigEndian", 1,m_strIniFile );

	if(m_bBigEndian)
	{
		m_btnEndian.SetCheck(1);
	}
	else
	{
		m_btnEndian.SetCheck(0);
	}

    CStringArray KeyAndData;
    if(0 == EnumKeys( szSection,&KeyAndData))
    {
        return FALSE;
    }

    for( i = 0 ; i < KeyAndData.GetSize(); i+=2 )
    {		
		int nIndex = m_cboProductList.AddString( KeyAndData[i+1] );
		if(  strcmp( KeyAndData[i+1], szProduct ) == 0 )
		{
			m_cboProductList.SetCurSel( nIndex );
		}
	}
	OnSelchangeComboProduct();
	CheckDlgButton( IDC_CHANGE_CHECK, 0 );	
	m_btnByMs.SetCheck(1);

	g_pdlg = this;
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTaskAnalyzerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTaskAnalyzerDlg::OnPaint() 
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
HCURSOR CTaskAnalyzerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CTaskAnalyzerDlg::OnCancel() 
{	
	//teana hu 2010.08.30
	char szValue[5] = {0};
	if(m_bBigEndian)
	{
		strcpy(szValue, "1");
	}
	else
	{
		strcpy(szValue, "0");
	}
	WritePrivateProfileString("Setting", "BigEndian", szValue, m_strIniFile);
	//
	CDialog::OnCancel();
}

void CTaskAnalyzerDlg::OnMapFileBtn() 
{

    CString strMapFile;
    GetDlgItem( IDC_MAP_FILE_EDIT )->GetWindowText( strMapFile );	

    static char BASED_CODE szFilter[] = "Memory Files (*.mem)|*.mem||";
	
    CFileDialog dlg(TRUE, NULL, strMapFile, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter, NULL);
    if(dlg.DoModal() == IDOK)
    {
        strMapFile = dlg.GetPathName();
		GetDlgItem( IDC_MAP_FILE_EDIT )->SetWindowText( strMapFile );
    }
}

void CTaskAnalyzerDlg::OnOK() 
{
	//teana hu 2010.08.30
	if(m_btnEndian.GetCheck() == 1)
	{
		m_bBigEndian = TRUE;
	}
	else
	{
		m_bBigEndian = FALSE;
	}
	g_bBigEndian = m_bBigEndian;
	//
	//teana hu 2010.04.08
	m_lstBox.ResetContent();
	//
	CString strPathName;
	UINT uiMaxFileSize = m_editSetting[0].GetDigit();
	UINT uiDataAreaSize = m_editSetting[1].GetDigit();
	UINT uiShareMemorySize = m_editSetting[2].GetDigit();
	UINT uiIramSize = m_editSetting[3].GetDigit();
	UINT uiRegSize = m_editSetting[4].GetDigit();
	UINT uiOffSize = m_editSetting[5].GetDigit();

	g_nMaxSwitchNum = m_editSetting[6].GetDigit();

	GetDlgItem( IDC_MAP_FILE_EDIT )->GetWindowText( strPathName );
	if( strPathName.IsEmpty() )
	{
		AfxMessageBox("Please select *.mem file.\n");
		return;
	}

	if( strPathName.Find(".mem") == -1)
	{
		AfxMessageBox("Wrong file.\r\nPlease select *.mem file.\n");
		return;
	}

	CString strSourceBinFile;
	GetDlgItem( IDC_SOURCE_FILE_EDIT  )->GetWindowText( strSourceBinFile );

	
	GetPrivateProfileString( m_strSelectProduct, "TASK_TABLE", NULL,m_szTaskList,1024,m_strIniFile );
	GetPrivateProfileString( m_strSelectProduct, "INT_TABLE", NULL,m_szIntList,1024,m_strIniFile );
	
	//teana hu 2008-07-04
	//[[
	if(m_btnByMs.GetCheck() == 1)
	{
		g_percentMode = 0;
	}
	if(m_btnByUs.GetCheck() == 1)
	{
		g_percentMode = 1;
	}
	//]]

	unsigned long* buf = NULL;

	if(m_bNewVersion)
	{
		AddTextToList("Analyzing memory head ... ...", NORMAL_COLOR, FALSE);
		if(GetMemParam(strPathName) == FALSE)
		{
			CString strContent;
			strContent.Format("    Error: %s", m_strErr.operator LPCTSTR ( ));
			AddTextToList("Analyze memory head failed!", FAIL_COLOR, TRUE);
			AddTextToList(strContent, FAIL_COLOR, FALSE);
			AfxMessageBox( "Analyze Failed!", MB_ICONERROR, 0 );
			return;
		}
		AddTextToList("Analyze memory head successfully!", NORMAL_COLOR, TRUE);
		buf = new unsigned long [ m_uiMaxFileSize  / 4 ]; //lint !e119
		if( buf == NULL )
		{
			AddTextToList("Max File Size is too big!", FAIL_COLOR, FALSE);
			AfxMessageBox( "Analyze Failed!", MB_ICONERROR, 0 );
			return;
		}
		memset( buf, 0, (m_uiMaxFileSize / 4) * sizeof( unsigned long) );
		
		AddTextToList("Analyzing test point table ... ...", NORMAL_COLOR, FALSE);
		char szTaskList[LIST_LEN] = {0};
		strcpy(szTaskList, m_szTaskList);
		if( Analyze( strPathName, buf, m_uiMaxFileSize, m_uiOffSize , m_szIntList, m_szTaskList) && AnalyzeCsvFile(&m_TMInfo, szTaskList, (LPSTR)(LPCTSTR)strPathName))
		{
			AddTextToList("Analyze test point table successfully!", NORMAL_COLOR, TRUE);
			if(SplitFile( strPathName,  buf, m_uiDspSize, m_uiShareOffset, m_uiIramOffset, 
				m_uiRegOffset, m_uiDspOffset,m_uiShareMemorySize, 
				m_uiIramSize, m_uiRegSize, m_uiMaxFileSize/4 ) )
			{
				AfxMessageBox("Analyze Complete!", MB_OK, 0);
			}
			else
			{
				AfxMessageBox( "Analyze Failed!", MB_ICONERROR, 0 );
			}	         
		}
		else
		{
			CString strContent;
			strContent.Format("    Error: %s", m_strErr.operator LPCTSTR ( ));
			AddTextToList("Analyze test point table failed!", FAIL_COLOR, TRUE);
			AddTextToList(strContent, FAIL_COLOR, FALSE);
			AfxMessageBox( "Analyze Failed!", MB_ICONERROR, 0 );
		}  
		
		//test
		//AnalyzeCsvFile(&m_TMInfo, szTaskList, (LPSTR)(LPCTSTR)strPathName);
		//test
	}
	else
	{
		buf = new unsigned long [ uiMaxFileSize  / 4 ];//lint !e119
		if( buf == NULL )
		{
			AfxMessageBox("Max File Size is too big!", IDOK, 0);  
			return;
		}
		memset( buf, 0, (uiMaxFileSize / 4) * sizeof( unsigned long) );
		strSourceBinFile.TrimLeft();
		strSourceBinFile.TrimRight();
		
		if( Analyze( m_strSelectProduct, strPathName, buf,uiMaxFileSize, uiOffSize , m_szTaskList, m_szIntList, 
			strSourceBinFile.IsEmpty()? NULL: strSourceBinFile.GetBuffer( strSourceBinFile.GetLength()) ) ) 
		{
			SplitFile( strPathName,  buf, uiDataAreaSize, 
				uiShareMemorySize,uiIramSize , uiRegSize ) ;
			AfxMessageBox("Analyze Complete!", MB_OK, 0);
			 
			WriteIniFile();
		}
		else
		{
			AfxMessageBox( "Analyze Fail!", MB_ICONERROR, 0 );
		} 
	}

	if(buf != NULL)//lint !e774
	{
		delete [] buf;
		buf = NULL;
	}
}

void CTaskAnalyzerDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	GetDlgItem( IDC_MAP_FILE_EDIT )->SetFocus();
	
}

void CTaskAnalyzerDlg::OnSelchangeComboProduct() 
{
	m_cboProductList.GetWindowText( m_strSelectProduct );

	char szSection[128];
    memset(szSection,0,128 );

	BOOL bEnable = TRUE;

	if(m_strSelectProduct == "MOCOR")
	{
		GetPrivateProfileString( m_strSelectProduct, "DebugString", NULL,szSection,64,m_strIniFile );
		_tcscpy(g_szDebugString, szSection);
		
		m_bNewVersion = TRUE;
		bEnable = FALSE;
	}
	else if(m_strSelectProduct == "OTHERS")
	{
		m_bNewVersion = TRUE;
		bEnable = FALSE;
	}
	else
	{
		m_bNewVersion = FALSE;
	}

	((CButton *)GetDlgItem(IDC_CHANGE_CHECK))->EnableWindow(bEnable);
    
	GetPrivateProfileString( m_strSelectProduct, "Describe", NULL,szSection,64,m_strIniFile );
	m_strEdtDescribe = szSection;
	UpdateData(FALSE);
	GetPrivateProfileString( m_strSelectProduct, "FileMaxSize", NULL,szSection,64,m_strIniFile );
	m_editSetting[0].SetWindowText( szSection );
	GetPrivateProfileString( m_strSelectProduct, "DataAreaSize", NULL,szSection,64,m_strIniFile );
	m_editSetting[1].SetWindowText( szSection );
	GetPrivateProfileString( m_strSelectProduct , "ShareMemorySize", NULL,szSection,64,m_strIniFile );
	m_editSetting[2].SetWindowText( szSection );
	GetPrivateProfileString( m_strSelectProduct, "IramSize", NULL,szSection,64,m_strIniFile );
	m_editSetting[3].SetWindowText( szSection );
	GetPrivateProfileString( m_strSelectProduct, "RegSize", NULL,szSection,64,m_strIniFile );
	m_editSetting[4].SetWindowText( szSection );
	GetPrivateProfileString( m_strSelectProduct, "OffsetSize", NULL,szSection,64,m_strIniFile );
	m_editSetting[5].SetWindowText( szSection );	
	GetPrivateProfileString( m_strSelectProduct, "SwitchNum", NULL,szSection,64,m_strIniFile );
	m_editSetting[6].SetWindowText( szSection );	
	
	SetSettingItemsStatus( FALSE );
	CheckDlgButton( IDC_CHANGE_CHECK, 0 );
	
}

void CTaskAnalyzerDlg::WriteIniFile()
{
	m_cboProductList.GetWindowText( m_strSelectProduct );

    UpdateData();

	WritePrivateProfileString(m_strSelectProduct, "Describe", m_strEdtDescribe, m_strIniFile);
    
}

void CTaskAnalyzerDlg::OnChangeCheck() 
{
	SetSettingItemsStatus( IsDlgButtonChecked( IDC_CHANGE_CHECK ) );
}

void CTaskAnalyzerDlg::SetSettingItemsStatus( BOOL bStatus /* = TRUE  */ )
{	
	GetDlgItem( IDC_DESCRIBE )->EnableWindow( FALSE );
	GetDlgItem( IDC_FILE_MAX_SIZE_EDIT )->EnableWindow( bStatus );
	GetDlgItem( IDC_FILE_MAX_SIZE_EDIT2 )->EnableWindow( bStatus );
	GetDlgItem( IDC_FILE_MAX_SIZE_EDIT3 )->EnableWindow( bStatus );
	GetDlgItem( IDC_FILE_MAX_SIZE_EDIT4 )->EnableWindow( bStatus );
	GetDlgItem( IDC_FILE_MAX_SIZE_EDIT5 )->EnableWindow( bStatus );
	GetDlgItem( IDC_FILE_MAX_SIZE_EDIT6 )->EnableWindow( bStatus );
	GetDlgItem( IDC_FILE_MAX_SIZE_EDIT7 )->EnableWindow( bStatus );

	if(m_bNewVersion)
	{
		GetDlgItem( IDC_FILE_MAX_SIZE_EDIT )->EnableWindow( FALSE );
		GetDlgItem( IDC_FILE_MAX_SIZE_EDIT2 )->EnableWindow( FALSE );
		GetDlgItem( IDC_FILE_MAX_SIZE_EDIT3 )->EnableWindow( FALSE );
		GetDlgItem( IDC_FILE_MAX_SIZE_EDIT4 )->EnableWindow( FALSE );
		GetDlgItem( IDC_FILE_MAX_SIZE_EDIT5 )->EnableWindow( FALSE );
		GetDlgItem( IDC_FILE_MAX_SIZE_EDIT6 )->EnableWindow( FALSE );
		GetDlgItem( IDC_FILE_MAX_SIZE_EDIT7 )->EnableWindow( FALSE );
	}
}



void CTaskAnalyzerDlg::OnSourceFileBtn() 
{
	CString strSourceFile;
    GetDlgItem( IDC_SOURCE_FILE_EDIT  )->GetWindowText( strSourceFile );	
	
    static char BASED_CODE szFilter[] = "Bin Files (*.bin)|*.bin||";
	
    CFileDialog dlg(TRUE, NULL, strSourceFile, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter, NULL);
    if(dlg.DoModal() == IDOK)
    {
        strSourceFile = dlg.GetPathName();
		GetDlgItem( IDC_SOURCE_FILE_EDIT  )->SetWindowText( strSourceFile );
	}
}

BOOL CTaskAnalyzerDlg::GetMemParam(const CString &strFileName)
{
	char szFileName[256] = {0};
	_tcscpy(szFileName, strFileName);
	HANDLE hFile = ::CreateFile(szFileName, GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile) 
	{
		m_strErr = "Can not open *.mem file!";
		//AfxMessageBox("Can not open *.mem file.\n");
		return FALSE;
	}
	
	HANDLE hMap = ::CreateFileMapping(hFile, NULL, PAGE_READONLY,0, 0, NULL);
	if (NULL == hMap) 
	{
		m_strErr = "Failed to create file mapping!";
		//AfxMessageBox("Failed to create file mapping.\n");
		return FALSE;
	}
	LPVOID lpData = NULL;
	lpData = ::MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
	if (NULL == lpData)
	{
		m_strErr = "MapViewOfFile failed!";
		//AfxMessageBox("MapViewOfFile failed.\n");
		return FALSE;
	}

	//get MaxFileSize
	m_uiMaxFileSize = GetFileSize(hFile, NULL);
	if(m_uiMaxFileSize <= 16)
	{
		m_strErr = "Memory file is too short, less than 16 bytes!";
		//AfxMessageBox("File is too short.\n");
		return FALSE;
	}
	
	const unsigned char * buf = (const unsigned char *)lpData;
	const char *magic = "TASKMONITORBEGIN";
	const unsigned char *end = buf + m_uiMaxFileSize;
	const unsigned char *tm_head = NULL;
	const unsigned char *irq_tab = NULL;
	unsigned short      nrof_irq;			//number of entries in irq table
	unsigned short      irq_offset;			//offset of irq table
	const unsigned char *mem_tab = NULL;
	unsigned short      nrof_mem;			//number of entries in memory region table
	unsigned short      mem_offset;			//offset of memory region table
	const unsigned char *point_tab = NULL;
	unsigned short      nrof_point;			//number of entries in task point table
	unsigned short      point_offset;		//offset of task point table
	const unsigned short *p;
	const unsigned char  *q;
	const unsigned int   *s;
	const unsigned char  *task_tab = NULL;
	
	//search magic word TASKMONITORBEGIN
	for (unsigned int i = 0; i <= m_uiMaxFileSize - 16; ++i) 
	{
		if ('T' == *(buf + i)) 
		{
			if (0 == ::memcmp(buf + i, magic, 16)) 
			{
				tm_head = (buf + i);
			//	break;
			}
		}
	}

	if (NULL == tm_head) 
	{
		m_strErr = "\"TASKMONITORBEGIN\" does not find in memory file!";
		//AfxMessageBox("Failed to find magic word.\n");
		return FALSE;
	}
	
	if (tm_head + 28 > end) 
	{
		m_strErr = "Out of file range in head!";
		//AfxMessageBox("Out of range in head.\n");
		return FALSE;
	}

	//irq
	p = (WORD *)(tm_head + 16);
	if(m_bBigEndian)
	{
		nrof_irq = SwapWord(*p);
	}
	else
	{
		nrof_irq = *p;
	}

	m_TMInfo.head.nrof_irq = nrof_irq;
	
	if(nrof_irq == 0)
	{
		m_strErr = "The number of irq table is 0!";
		//AfxMessageBox("The number of irq table is 0.\nPlease check *.mem file.");
		return FALSE;
	}
	p++;
	if(m_bBigEndian)
	{
		irq_offset = SwapWord(*p);
	}
	else
	{
		irq_offset = *p;
	}
	m_TMInfo.head.irq_tab_offset = irq_offset;
		
	if(irq_offset >= m_uiMaxFileSize)
	{
		m_strErr = "Irq offset is out of file range!";
		//AfxMessageBox("Irq offset out of range.\n");
		return FALSE;
	}
	p++;
	irq_tab = tm_head + irq_offset;
	if (irq_tab + nrof_irq * 16 > end) 
	{
		m_strErr = "Out of file range in irq table!";
		//AfxMessageBox("Out of range in irq table.\n");
		return FALSE;
	}
	
	//memory region
	if(m_bBigEndian)
	{
		nrof_mem = SwapWord(*p);
	}
	else
	{
		nrof_mem = *p;
	}

	m_TMInfo.head.nrof_mem_region = nrof_mem;
	if(nrof_mem == 0)
	{
		m_strErr = "The number of memory region table is 0!";
		//AfxMessageBox("The number of memory region table is 0.\nPlease check *.mem file.");
		return FALSE;
	}
	p++;
	if(m_bBigEndian)
	{
		mem_offset = SwapWord(*p);
	}
	else
	{
		mem_offset = *p;
	}

	m_TMInfo.head.mem_tab_offset = mem_offset;
		
	if(mem_offset >= m_uiMaxFileSize)
	{
		m_strErr = "Memory region offset is out of file range!";
		//AfxMessageBox("Memory offset out of range.\n");
		return FALSE;
	}
	p++;
	mem_tab = tm_head + mem_offset;
	if (mem_tab + nrof_mem * 12 > end) 
	{
		m_strErr = "Out of file range in memory region table!";
		//AfxMessageBox("Out of range in memory region table.\n");
		return FALSE;
	}

	//task point
	if(m_bBigEndian)
	{
		nrof_point = SwapWord(*p);
	}
	else
	{
		nrof_point = *p;
	}
	m_TMInfo.head.nrof_point = nrof_point;
	
	if(nrof_point == 0)
	{
		m_strErr == "The number of test point table is 0!";
		//AfxMessageBox("The number of test point table is 0.\nPlease check *.mem file.");
		return FALSE;
	}
	p++;
	if(m_bBigEndian)
	{
		point_offset = SwapWord(*p);
	}
	else
	{
		point_offset = *p;
	}
	m_TMInfo.head.point_tab_offset = point_offset;

	if(point_offset >= m_uiMaxFileSize)
	{
		m_strErr = "Test point offset is out of file range!";
		//AfxMessageBox("Test point offset out of range.\n");
		return FALSE;
	}
	p++;
	point_tab = tm_head + point_offset;
	if (point_tab + nrof_point * 16 > end)
	{
		m_strErr = "Out of file range in task point table";
		//AfxMessageBox("Out of range in task point table.\n");
		return FALSE;
	}

	//teana.hu 2011.01.17
	int nPointNum = nrof_point > 3000 ? 3000 : nrof_point;
	for (i = 0; i < nPointNum; i++)
	{
		POINT_TAB pointTab;
		memcpy(&pointTab, point_tab, sizeof(POINT_TAB));

		if(m_bBigEndian)
		{
			pointTab.id = SwapWord(pointTab.id);
			pointTab.acuracy_time = SwapWord(pointTab.acuracy_time);
			pointTab.pointer = SwapDword(pointTab.pointer);
			pointTab.return_addr = SwapDword(pointTab.return_addr);
			pointTab.time = SwapDword(pointTab.time);
		}

		m_TMInfo.point_tab[i].id = pointTab.id;
		m_TMInfo.point_tab[i].acuracy_time = pointTab.acuracy_time;
		m_TMInfo.point_tab[i].pointer = pointTab.pointer;
		m_TMInfo.point_tab[i].return_addr = pointTab.return_addr;
		m_TMInfo.point_tab[i].time = pointTab.time;

		point_tab += sizeof(POINT_TAB);
	}
	//

	g_nMaxSwitchNum = nrof_point;	//set task switch number
	g_nTaskTableOffset = point_offset;

	//irq table
	CString strIntList;
	strIntList.Empty();
	q = irq_tab;
	for (i = 0; i < nrof_irq; ++i) 
	{
		char szTmp[16] = {0};
		memcpy(szTmp, q, 16); 
		strIntList += szTmp;
		strIntList += ",";
		q += 16;
	}
	int nIntLen = strIntList.GetLength();
	memcpy(m_szIntList, strIntList, nIntLen);		//set int table

	//memory region table
	BOOL bReadTask = TRUE;
	s = (const unsigned int *)mem_tab;
	for (i = 0; i < nrof_mem; i++) 
	{
		unsigned int start_addr = 0;
		unsigned int size = 0;
		unsigned int offset = 0;
		if(m_bBigEndian)
		{
			start_addr = SwapDword(*s);
			s++;
			size = SwapDword(*s);
			s++;
			offset = SwapDword(*s);
			s++;
		}
		else
		{
			start_addr = *s;
			s++;
			size = *s;
			s++;
			offset = *s;
			s++;
			
		}
		if(i == 0)
		{
			m_uiDataAreaSize = size;		//set Data Area Size
			m_uiOffSize = start_addr;		//set Offset Size
		}
		if(i == 1)
		{
			if(offset >= m_uiMaxFileSize)
			{
				bReadTask = FALSE;
			}
			task_tab = buf + offset;
		}
		if(i == 2)
		{
			m_uiShareMemorySize = size;		//set Share Memory Size
			m_uiShareOffset = offset;
		}
		if(i == 3)
		{
			m_uiIramSize = size;			//set Iram Size
			m_uiIramOffset = offset;
		}
		if(i == 4)
		{
			m_uiRegSize = size;				//set Register Size
			m_uiRegOffset = offset;
		}
		if(i == 5)
		{
			m_uiDspSize = size;
			m_uiDspOffset = offset;
		}

		//teana.hu 2011.01.17
		m_TMInfo.mem_tab[i].start_addr = start_addr;
		m_TMInfo.mem_tab[i].size = size;
		m_TMInfo.mem_tab[i].offset = offset;
	}
	if(task_tab == NULL)
	{
		return FALSE;
	}

	if(bReadTask)
	{
	
		//task table
		CString strTaskList;
		strTaskList.Empty();
		int index = 0;
		for (;;)
		{
			unsigned long fcb_ptr = 0;
//			if(m_bBigEndian)
//			{
//				//fcb_ptr = SwapDword(*(unsigned long *)task_tab);
//				fcb_ptr = *(unsigned long *)task_tab;
//			}
//			else
			{
				//fcb_ptr = *(unsigned long *)task_tab;
				fcb_ptr = SwapDword(*(unsigned long *)task_tab);
			}
			task_tab += 4;
			if (0 == fcb_ptr) 
			{
				break;
			}
			g_nTcbPtr[index] = fcb_ptr;
			char szTmp[32] = {0};
			memcpy(szTmp, task_tab, 32); 
			strTaskList += szTmp;
			strTaskList += ",";
			index ++;
			task_tab += 32;	
		}
		int nTaskLen = strTaskList.GetLength();
		if(nTaskLen > LIST_LEN)
		{
			m_strErr = "Task table is too long, more than 1024!";
			//AfxMessageBox("task table is too long, please check it.");
			//release
			if (NULL != lpData) {//lint !e774
				::UnmapViewOfFile(lpData);
			}
			
			if (NULL != hMap) {//lint !e774
				::CloseHandle(hMap);
			}
			
			if (INVALID_HANDLE_VALUE != hFile) {
				::CloseHandle(hFile);
			}
			return FALSE;
		}
		memcpy(m_szTaskList, strTaskList, nTaskLen);		//set int table
	}

	//release
	if (NULL != lpData) {//lint !e774
		::UnmapViewOfFile(lpData);
	}

	if (NULL != hMap) {//lint !e774
		::CloseHandle(hMap);
	}

	if (INVALID_HANDLE_VALUE != hFile) {
		::CloseHandle(hFile);
	}
	return TRUE;
}

void CTaskAnalyzerDlg::AddTextToList(const CString &strText, COLORREF clr, BOOL bModify)
{
	CString strContent = strText;
	
	if(!strContent.IsEmpty())
	{
		int nCount = m_lstBox.GetCount();

		if(bModify)
		{
			m_lstBox.DeleteString(nCount - 1);
		}

		m_lstBox.AddString(strContent, clr);
		nCount = m_lstBox.GetCount();
		m_lstBox.SetTopIndex(nCount - 1);
		if (nCount > 0)
		{
			m_lstBox.SetCurSel(nCount-1);
		}
	}
}

extern CTaskAnalyzerApp theApp;
BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem( IDC_STATIC_VERSION )->SetWindowText( theApp.m_strVersion );
    GetDlgItem( IDC_STATIC_BUILD )->SetWindowText( theApp.m_strBuild );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
	

void CTaskAnalyzerDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//teana hu 2010.08.30
	char szValue[5] = {0};
	if(m_bBigEndian)
	{
		strcpy(szValue, "1");
	}
	else
	{
		strcpy(szValue, "0");
	}
	WritePrivateProfileString("Setting", "BigEndian", szValue, m_strIniFile);
	//
	
	CDialog::OnClose();
}

void CTaskAnalyzerDlg::OnCheckEndian() 
{
	// TODO: Add your control notification handler code here
	if(m_btnEndian.GetCheck() == 1)
	{
		m_bBigEndian = TRUE;
	}
	else
	{
		m_bBigEndian = FALSE;
	}
	
}
