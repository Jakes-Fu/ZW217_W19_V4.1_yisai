// ResOverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ResOver.h"
#include "ResOverDlg.h"
#include <afxtempl.h>
#include "mmi_ring_def.h"
#include "XAboutDlg.h"
#include "DlgReport.h"
#include "DlgNtac.h"
#include "DlgMtva.h"
#include "DlgBkmk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MODIFIED_FALG_IMG_INDEX (1)
UINT SaveResThread(LPVOID pParam);
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
// CResOverDlg dialog

CResOverDlg::CResOverDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResOverDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResOverDlg)
	m_strItemInfo = _T("");
	m_strResInfo = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pImglist = NULL;

	m_bInitList = FALSE;

	m_bImportString = FALSE;

	m_bDisable = FALSE;
}

CResOverDlg::~CResOverDlg()
{
/*lint -save -e1551 */
	if(m_pImglist != NULL)
	{
		m_pImglist->DeleteImageList(); 
		delete m_pImglist; 
		m_pImglist = NULL;
	}
	if(m_hIcon != NULL)
	{
		::DeleteObject(m_hIcon);
		m_hIcon = NULL;
	}

/*lint -restore */
}
void CResOverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResOverDlg)
	DDX_Control(pDX, IDC_PROG, m_prog);
	DDX_Control(pDX, IDC_STC_PREVIEW, m_imgView);
	DDX_Control(pDX, IDC_CMB_THEME, m_cmbTheme);
	DDX_Control(pDX, IDC_CMB_RES_TYPE, m_cmbResType);
	DDX_Control(pDX, IDC_LST_RES, m_lstResItem);
	DDX_Text(pDX, IDC_STC_ITEM_INFO, m_strItemInfo);
	DDX_Text(pDX, IDC_STC_RES_INFO, m_strResInfo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CResOverDlg, CDialog)
	//{{AFX_MSG_MAP(CResOverDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LST_RES, OnItemchangedLstRes)
	ON_BN_CLICKED(IDC_BTN_LOAD, OnLoad)
	ON_BN_CLICKED(IDC_BTN_REPLACE, OnReplace)
	ON_CBN_SELCHANGE(IDC_CMB_THEME, OnSelchangeTheme)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnSave)
	ON_BN_CLICKED(IDC_BTN_UNDO, OnUndo)
	ON_CBN_SELCHANGE(IDC_CMB_RES_TYPE, OnSelchangeResType)
	ON_BN_CLICKED(IDC_BTN_HELP, OnHelp)
	ON_BN_CLICKED(IDCANCEL, OnExit)
	ON_BN_CLICKED(IDC_BTN_LOAD_THM_DIR, OnBtnLoadThmDir)
	ON_BN_CLICKED(IDC_BTN_EXPORT_THM_DIR, OnBtnExportThmDir)
	ON_BN_CLICKED(IDC_BTN_EXP_STR, OnExpStr)
	ON_BN_CLICKED(IDC_BTN_IMP_STR, OnImpStr)
	ON_BN_CLICKED(IDC_BTC_EDT_MTVA, OnEdtMtva)
	ON_BN_CLICKED(IDC_BTC_EDT_NTAC, OnEdtNtac)
	ON_BN_CLICKED(IDC_BTN_EDT_BKMK, OnEdtBkmk)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PROGRESS, OnProgressMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResOverDlg message handlers

BOOL CResOverDlg::OnInitDialog()
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
	m_pImglist = (CImageList *)new CImageList();
	m_pImglist->Create(IDB_BMP_FLAG,16,2,RGB(255,0,255));

	m_lstResItem.SetImageList(m_pImglist,LVSIL_SMALL);
	m_lstResItem.SetBkColor(RGB(232, 232, 232));	
	m_lstResItem.ModifyStyle(0, LVS_SHOWSELALWAYS);
	
	DWORD dwExStyle = m_lstResItem.GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	m_lstResItem.SetExtendedStyle(dwExStyle);
	
	m_lstResItem.InsertColumn(0,_T("Desc"),LVCFMT_LEFT,200);
	m_lstResItem.InsertColumn(1,_T("Index"),LVCFMT_LEFT,100);
	m_lstResItem.InsertColumn(2,_T("Module"),LVCFMT_LEFT,100);
	m_lstResItem.InsertColumn(3,_T("Type"),LVCFMT_LEFT,150);
	
	ChangeItemState(FALSE);	

	
	
	_TCHAR path[_MAX_PATH] = {0};

	if( GetTempPath(_MAX_PATH,path) == 0)
	{
		::GetModuleFileName(g_theApp.m_hInstance,path,_MAX_PATH);

		_TCHAR *pFind = _tcsrchr(path,_T('\\'));
		if(pFind != NULL)
		{
			*pFind = _T('\0');
		}		
	}	

	_TCHAR szTmp[100]={0};
	_stprintf(szTmp,_T("\\resover_temp_%d"),GetTickCount());
	_tcscat(path,szTmp);

	g_theApp.DeleteDirectory(path);

	m_strTmpFolder = path;

	if(!CreateDirectory(path,NULL))
	{
		m_resParser.m_strTmpFolder.Empty();
	}
	else
	{
		m_resParser.m_strTmpFolder = path;
	}

	m_resParser.m_hWnd = GetSafeHwnd();

	_TCHAR pathIni[_MAX_PATH] = {0};
	::GetModuleFileName(g_theApp.m_hInstance,pathIni,_MAX_PATH);
	_TCHAR * pFind = _tcsrchr(pathIni,_T('.'));
	if(pFind != NULL)
	{
		*pFind = _T('\0');
	}
	_tcscat(pathIni,_T(".ini"));

	m_resParser.m_dwMaxResSize = (DWORD)GetPrivateProfileInt(_T("Settings"),_T("MaxResSize"),(INT)MAXDWORD,pathIni);
	m_resParser.m_bCheckImagWH = GetPrivateProfileInt(_T("Settings"),_T("CheckImageWH"),TRUE,pathIni);
	m_resParser.m_bCheckAnimWH = GetPrivateProfileInt(_T("Settings"),_T("CheckAnimWH"),TRUE,pathIni);
	m_resParser.m_bCheckAnimFrame= GetPrivateProfileInt(_T("Settings"),_T("CheckAnimFrame"),TRUE,pathIni);
	m_resParser.m_bCheckRingType = GetPrivateProfileInt(_T("Settings"),_T("CheckRingType"),TRUE,pathIni);
	m_imgView.m_bDrawImageBG    = GetPrivateProfileInt(_T("Settings"),_T("DrawImageBG"),TRUE,pathIni);
	m_resParser.m_nExcelParserType = GetPrivateProfileInt(_T("Settings"),_T("ExcelParser"),-1,pathIni);
	m_resParser.m_bShowWarning = GetPrivateProfileInt(_T("Settings"),_T("ShowWarning"),TRUE,pathIni);

	m_bDisable = GetPrivateProfileInt(_T("GUI"),_T("Disable"),FALSE,pathIni);

	if(m_bDisable)
	{
		GetDlgItem(IDC_BTN_LOAD_THM_DIR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_EXPORT_THM_DIR)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_RES_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_REPLACE)->EnableWindow(FALSE);	
		GetDlgItem(IDC_BTN_UNDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_THEME)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_EDT_BKMK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTC_EDT_NTAC)->EnableWindow(FALSE);
		GetDlgItem(IDC_LST_RES)->EnableWindow(FALSE);
	}

	CXVerInfo ver;
	CString strTitle;

#ifdef _DEBUG
	strTitle.Format(_T("ResOver - D%s"),ver.GetSPRDVersionString());
#else
	strTitle.Format(_T("ResBOCA - R%s"),ver.GetSPRDVersionString());
#endif

	SetWindowText(strTitle.operator LPCTSTR());

	m_imgView.CreateCtrls();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CResOverDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CXAboutDlg dlgAbout;
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

void CResOverDlg::OnPaint() 
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
// 		CRect rc;
// 		m_imgView.GetClientRect(&rc);
// 		m_imgView.ClientToScreen(&rc);
// 		ScreenToClient(&rc);

		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CResOverDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CResOverDlg::OnItemchangedLstRes(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_bInitList)
		return;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int nItem = pNMListView->iItem ;
	int nThemeItem = m_cmbTheme.GetCurSel();
	if(nItem< 0 || nThemeItem < 0)
		return;
	DWORD dwIndex = m_lstResItem.GetItemData(nItem);
	DWORD dwThemeIndex = m_cmbTheme.GetItemData(nThemeItem);
	m_strItemInfo = m_resParser.GetResItemInfo(dwIndex,dwThemeIndex);

	RES_ITEM_PTR pRI = m_resParser.m_vResItem[dwIndex];

	if(!m_bDisable)
	{
		if(pRI->itemInfo[dwThemeIndex].wExist == 0)
		{
			GetDlgItem(IDC_BTN_REPLACE)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_BTN_REPLACE)->EnableWindow(TRUE);
		}	
		GetDlgItem(IDC_BTN_UNDO)->EnableWindow(m_resParser.m_vResItem[dwIndex]->isModified);
	}


	m_imgView.UpdateResItem(pRI,(int)dwThemeIndex);

	UpdateData(FALSE);
		
	*pResult = 0;
}
void CResOverDlg::ChangeItemState(BOOL bLoadOK)
{
	BOOL bModifed = m_resParser.IsModified();
	BOOL bStrExported = m_resParser.IsStrExported();
	
	if(bLoadOK && bModifed)
	{
		GetDlgItem(IDC_BTN_SAVE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_SAVE)->EnableWindow(FALSE);
	}

	if(bLoadOK)
	{		
		GetDlgItem(IDC_BTN_EXP_STR)->EnableWindow(TRUE);	
		GetDlgItem(IDC_BTN_IMP_STR)->EnableWindow(bStrExported);
	}
	else
	{

		GetDlgItem(IDC_BTN_EXP_STR)->EnableWindow(FALSE);	
		GetDlgItem(IDC_BTN_IMP_STR)->EnableWindow(FALSE);
	}
	
	if(m_bDisable)
		return;

	if(bLoadOK)
	{		
		GetDlgItem(IDC_BTN_LOAD_THM_DIR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_EXPORT_THM_DIR)->EnableWindow(TRUE);	
	}
	else
	{
		GetDlgItem(IDC_BTN_LOAD_THM_DIR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_EXPORT_THM_DIR)->EnableWindow(FALSE);
	}

	GetDlgItem(IDC_CMB_RES_TYPE)->EnableWindow(bLoadOK);
	GetDlgItem(IDC_BTN_REPLACE)->EnableWindow(bLoadOK);	
	GetDlgItem(IDC_CMB_THEME)->EnableWindow(bLoadOK);
}

void CResOverDlg::OnLoad() 
{
	// TODO: Add your control notification handler code here
// 	if(m_resParser.IsModified())
// 	{
// 		if(AfxMessageBox(_T("You have not save the modified resource, if continue?"),MB_YESNO) == IDNO)
// 		{
// 			return;
// 		}
// 	}

	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
		            _T("Res files (*.bin)|*.bin|Theme files (*.thm)|*.thm||"));
	if(dlg.DoModal() == IDCANCEL)
		return;

	Load(dlg.GetPathName().operator LPCTSTR());


}
void CResOverDlg::InitCtrls()
{
	m_lstResItem.DeleteAllItems();

	UINT nCount = m_resParser.m_vResItem.size();
	CString strTmp;
	CMap<UINT,UINT,UINT,UINT> mapResType;

	UINT i = 0;
	UINT j = 0;
	int nIdx =0;

	while(m_cmbResType.GetCount() >0)
	{
		m_cmbResType.DeleteString(0);
	}	
	nIdx = m_cmbResType.AddString(_T("ALL"));
	m_cmbResType.SetItemData(nIdx,MAXDWORD);
	

	while(m_cmbTheme.GetCount() >0)
	{
		m_cmbTheme.DeleteString(0);
	}

	for(i=0;i< m_resParser.m_uThemeNum; i++)
	{
		nIdx = m_cmbTheme.AddString(m_resParser.m_agThemeName.GetAt(i));
		m_cmbTheme.SetItemData(nIdx,i);
	}
	m_cmbTheme.SetCurSel(0);	

	m_bInitList = TRUE;
	BOOL bHasLogo = FALSE;
	for(i = 0; i<nCount; i++)
	{
		RES_ITEM_PTR pRI = m_resParser.m_vResItem[i];
		if(pRI->bLogo) bHasLogo = TRUE;
		if(pRI->uResType == MMI_RT_MAX_NUM)
			continue;

		m_lstResItem.InsertItem(j,pRI->szDesc);
		
		strTmp.Format(_T("%d"),pRI->uResIndex);
		m_lstResItem.SetItemText(j,1,strTmp);
		strTmp.Format(_T("%d"),pRI->uModuleID);
		m_lstResItem.SetItemText(j,2,strTmp);
		m_lstResItem.SetItemText(j,3,m_resParser.GetResTypeString(pRI->uResType));
		m_lstResItem.SetItemData(j,i);
		mapResType.SetAt(pRI->uResType,pRI->uResType);

		j++;	}
	m_lstResItem.SetFocus();
	m_bInitList = FALSE;

	if(bHasLogo)
	{
		nIdx = m_cmbResType.AddString(_T("LOGO"));
		m_cmbResType.SetItemData(nIdx,MAXDWORD-1);
	}

	UINT uResType;
	for(i=0; i< 8; i++)
	{
		if(mapResType.Lookup(i,uResType))
		{
			strTmp = m_resParser.GetResTypeString(uResType);
			nIdx = m_cmbResType.AddString(strTmp);
			m_cmbResType.SetItemData(nIdx,uResType);
		}
	}
	m_cmbResType.SetCurSel(0);

	if(nCount > 0)
	{
		LVITEM lv = {0};
		lv.iItem = 0;
		lv.iSubItem = 0;
		lv.mask = LVIF_STATE;
		lv.stateMask = LVIS_SELECTED;
		lv.state = LVIS_SELECTED;
		m_lstResItem.SetItem(&lv);
		
		if(!m_bDisable)
		{
			if(m_resParser.m_vResItem[0]->itemInfo[0].wExist == 0)
			{
				GetDlgItem(IDC_BTN_REPLACE)->EnableWindow(FALSE);
			}
			else
			{
				GetDlgItem(IDC_BTN_REPLACE)->EnableWindow(TRUE);
			}
			GetDlgItem(IDC_BTN_UNDO)->EnableWindow(m_resParser.m_vResItem[0]->isModified);
		}
	}
}

void CResOverDlg::OnReplace() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lstResItem.GetFirstSelectedItemPosition();
	int nItem = m_lstResItem.GetNextSelectedItem(pos);
	if(nItem == -1)
	{
		return;
	}

	DWORD dwIndex = m_lstResItem.GetItemData(nItem);

	int nSubItem = m_cmbTheme.GetCurSel();

	DWORD dwThemeIndex = m_cmbTheme.GetItemData(nSubItem);

	BOOL bReplaceOK = FALSE;

	RES_ITEM_PTR pRI = m_resParser.m_vResItem[dwIndex];
	WORD wItemType = pRI->itemInfo[0].org.wItemType;

	if(pRI->uResType == MMI_RT_IMAGE)
	{
		CString strFilter;		

		if( wItemType == IMG_BMP )
		{
			strFilter = _T("BMP files (*.bmp) |*.bmp||");
		}
		else if( wItemType == IMG_GIF )
		{
			strFilter = _T("GIF files (*.gif) |*.gif||");
		}
		else if( wItemType == IMG_JPG )
		{
			strFilter = _T("JPG files (*.jpg;*.jpeg) |*.jpg;*.jpeg||");
		}
		else if( wItemType == IMG_PNG )
		{
			strFilter = _T("PNG files (*.png) |*.png||");
		}
		else
		{
			strFilter = _T("All files (*.*) |*.*||");
		}
		CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
			strFilter.operator LPCTSTR());

		if(dlg.DoModal()==IDCANCEL)
			return;

		bReplaceOK = m_resParser.RelaceImag(dwIndex,dwThemeIndex,dlg.GetPathName().operator LPCTSTR());		
	}
	else if(pRI->uResType == MMI_RT_ANIM)
	{
		CString strAnimFolder = GetAnimFolder(dwIndex,dwThemeIndex);
		if(strAnimFolder.IsEmpty())
			return;

		bReplaceOK = m_resParser.RelaceAnim(dwIndex,dwThemeIndex,strAnimFolder.operator LPCTSTR());
	}
	else if(pRI->uResType == MMI_RT_RING)
	{
		CString strFilter;	 

		if(wItemType == MMIAUDIO_MIDI)
		{
			strFilter = _T("Ring files (*.mid;*.midi) |*.mid;*.midi||");
		}
		else if(wItemType == MMIAUDIO_SMAF)
		{
			strFilter = _T("Ring files (*.mmf) |*.mmf||");
		}
		else if(wItemType == MMIAUDIO_WAVE)
		{
			strFilter = _T("Ring files (*.wav) |*.wav||");
		}
		else if(wItemType == MMIAUDIO_MP3)
		{
			strFilter = _T("Ring files (*.mp3) |*.mp3||");
		}
		else if(wItemType == MMIAUDIO_AAC)
		{
			strFilter = _T("Ring files (*.aac) |*.aac||");
		}
		else if(wItemType == MMIAUDIO_M4A)
		{
			strFilter = _T("Ring files (*.m4a) |*.m4a||");
		}
		else if(wItemType == MMIAUDIO_AMR)
		{
			strFilter = _T("Ring files (*.amr) |*.amr||");
		}
		else if(wItemType == MMIAUDIO_WMA)
		{
			strFilter = _T("Ring files (*.wma) |*.wma||");
		}
		else
		{
			strFilter = _T("All files (*.*) |*.*||");
		}
		CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
			strFilter.operator LPCTSTR());

		if(dlg.DoModal()==IDCANCEL)
			return;

		bReplaceOK = m_resParser.RelaceRing(dwIndex,dwThemeIndex,dlg.GetPathName().operator LPCTSTR());
	}
	else if(pRI->uResType == MMI_RT_COLOR)
	{
		CColorDialog dlg(pRI->itemInfo[dwThemeIndex].org.dwColor);
		if(dlg.DoModal()==IDCANCEL)
			return;
		DWORD dwColor = dlg.GetColor();
		dwColor = g_theApp.Color24To565(dwColor);
		bReplaceOK = m_resParser.RelaceColor(dwIndex,dwThemeIndex,dwColor);
	}
	else if(pRI->uResType == MMI_RT_OTHER)
	{
		if(_tcsicmp(pRI->szDesc,_T("R_NETWORK_ACCOUNT")) == 0)
		{
			CDlgNtac ntac;
			BOOL bOK = TRUE;	
			if(pRI->isModified)
			{
				bOK = ntac.SetBuf(pRI->itemInfo[dwThemeIndex].rep.pBuf,pRI->itemInfo[dwThemeIndex].rep.dwSize,m_strTmpFolder);
			}
			else
			{
				bOK = ntac.SetBuf(pRI->itemInfo[dwThemeIndex].org.pBuf,pRI->itemInfo[dwThemeIndex].org.dwSize,m_strTmpFolder);
			}

			if(!bOK)
			{
				AfxMessageBox(_T("Load network account data failed!"));
			}

			if(ntac.DoModal() == IDCANCEL || ntac.m_strNTAC.IsEmpty())
			{
				return;
			}

			bReplaceOK = m_resParser.RelaceData(dwIndex,dwThemeIndex,ntac.m_strNTAC.operator LPCTSTR());
		}
		else if(_tcsicmp(pRI->szDesc,_T("R_MULIT_VARIANT")) == 0)
		{
			CDlgMtva mtva;
			BOOL bOK = TRUE;	
			if(pRI->isModified)
			{
				bOK = mtva.SetBuf(pRI->itemInfo[dwThemeIndex].rep.pBuf,pRI->itemInfo[dwThemeIndex].rep.dwSize,m_strTmpFolder);
			}
			else
			{
				bOK = mtva.SetBuf(pRI->itemInfo[dwThemeIndex].org.pBuf,pRI->itemInfo[dwThemeIndex].org.dwSize,m_strTmpFolder);
			}

			if(!bOK)
			{
				AfxMessageBox(_T("Load network account data failed!"));
			}

			if(mtva.DoModal() == IDCANCEL || mtva.m_strMTVA.IsEmpty())
			{
				return;
			}

			bReplaceOK = m_resParser.RelaceData(dwIndex,dwThemeIndex,mtva.m_strMTVA.operator LPCTSTR());
		}
		else if(_tcsicmp(pRI->szDesc,_T("R_NETWORK_BOOKMARK")) == 0)
		{
			CDlgBkmk bkmk;
			BOOL bOK = TRUE;
			if(pRI->isModified)
			{
				bOK = bkmk.SetBuf(pRI->itemInfo[dwThemeIndex].rep.pBuf,pRI->itemInfo[dwThemeIndex].rep.dwSize,m_strTmpFolder);
			}
			else
			{
				bOK = bkmk.SetBuf(pRI->itemInfo[dwThemeIndex].org.pBuf,pRI->itemInfo[dwThemeIndex].org.dwSize,m_strTmpFolder);
			}
			if(!bOK)
			{
				AfxMessageBox(_T("Load network bookmark data failed!"));
			}

			if(bkmk.DoModal() == IDCANCEL || bkmk.m_strBKMK.IsEmpty() )
			{
				return;
			}

			bReplaceOK = m_resParser.RelaceData(dwIndex,dwThemeIndex,bkmk.m_strBKMK.operator LPCTSTR());
		}
		else
		{
			CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
							_T("All files (*.*) |*.*||"));

			if(dlg.DoModal()==IDCANCEL)
				return;

			bReplaceOK = m_resParser.RelaceData(dwIndex,dwThemeIndex,dlg.GetPathName().operator LPCTSTR());
		}
	}
	
	if(bReplaceOK)
	{
		LVITEM lv = {0};
		lv.iItem = nItem;
		lv.iSubItem = 0;
		lv.mask = LVIF_IMAGE;
		lv.iImage = MODIFIED_FALG_IMG_INDEX;
		m_lstResItem.SetItem(&lv);

		m_strItemInfo = m_resParser.GetResItemInfo(dwIndex,dwThemeIndex);
		m_strResInfo = m_resParser.GetResInfo();
	    UpdateData(FALSE);

		GetDlgItem(IDC_BTN_SAVE)->EnableWindow(TRUE);

		m_imgView.UpdateResItem(pRI,dwThemeIndex);
//		m_imgView.Invalidate(FALSE);

	}

}

void CResOverDlg::OnSelchangeTheme() 
{
	// TODO: Add your control notification handler code here
	int nThemeItem = m_cmbTheme.GetCurSel();
	if(nThemeItem == -1)
	{
		return;
	}
    int nThemeIndex = (int)m_cmbTheme.GetItemData(nThemeItem);
	POSITION pos = m_lstResItem.GetFirstSelectedItemPosition();
	int nItem = m_lstResItem.GetNextSelectedItem(pos);
	if(nItem == -1)
	{
		return;
	}
	DWORD dwIndex = m_lstResItem.GetItemData(nItem);

	RES_ITEM_PTR  pRI = m_resParser.m_vResItem[dwIndex];

	if(pRI->itemInfo[nThemeIndex].wExist == 0)
	{
		GetDlgItem(IDC_BTN_REPLACE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_REPLACE)->EnableWindow(TRUE);
	}
	m_strItemInfo = m_resParser.GetResItemInfo(dwIndex,nThemeIndex);
	UpdateData(FALSE);

	m_imgView.UpdateResItem(pRI,nThemeIndex);
}

CString CResOverDlg::GetFolder(LPCTSTR lpszTitle /*= NULL*/)
{
	::CoInitialize(NULL);
	
	CString strTitle = _T("Please select a folder");
	if(lpszTitle != NULL)
	{
		strTitle = lpszTitle;
	}
	
	_TCHAR SZ_SEL_DIR[ _MAX_PATH ] = _T("");
    LPMALLOC lpMalloc = NULL;
    SZ_SEL_DIR[0] = _T('\0');
    if(::SHGetMalloc(&lpMalloc) == E_FAIL)
    {
        return SZ_SEL_DIR;
    }
	
    BROWSEINFO browseInfo;
	
    browseInfo.hwndOwner      = this->m_hWnd;
    browseInfo.pidlRoot       = NULL; 
    browseInfo.pszDisplayName = NULL;
    browseInfo.lpszTitle      = strTitle.operator LPCTSTR();   
    browseInfo.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_EDITBOX;      
    browseInfo.lpfn           = NULL;       
    browseInfo.lParam         = 0;    
	
    LPITEMIDLIST lpItemIDList = NULL;
    if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo)) != NULL)
    {
        ::SHGetPathFromIDList(lpItemIDList, SZ_SEL_DIR);
		
        lpMalloc->Free(lpItemIDList);
        lpMalloc->Release();      
    }
	
    ::CoUninitialize();
	
    return SZ_SEL_DIR;

}
CString CResOverDlg::GetAnimFolder(int nItemIndex, int nThemeIndex)
{
	UNUSED_ALWAYS(nItemIndex);
	UNUSED_ALWAYS(nThemeIndex);

	CString strTitle = _T("Please select a destination folder");
	RES_ITEM_PTR pRI = m_resParser.m_vResItem[nItemIndex];
	WORD wItemType = pRI->itemInfo[nThemeIndex].org.wItemType;
	if(wItemType == IMG_BMP)
	{
		strTitle = _T("Please select a destination folder (BMPs)");
	}
	else if(wItemType == IMG_PNG)
	{
		strTitle = _T("Please select a destination folder (PNGs)");
	}

	return GetFolder(strTitle);		
	
}
UINT SaveResThread(LPVOID pParam)	
{
	CResOverDlg* pDlg = (CResOverDlg*)pParam;
	UINT nRet = 1;
	BOOL bOK = FALSE;
	if (NULL==pDlg)
	{
		pDlg->m_bSaveOk = FALSE;
		pDlg->m_bSaving = FALSE;
		return nRet;
	}
	
	CString strResFile=pDlg->m_strSaveFile;
	pDlg->m_bSaving = TRUE;
	pDlg->m_bSaveOk = FALSE;
	bOK = (pDlg->m_resParser).SaveResFile(strResFile.operator LPCTSTR());
	if(bOK)
	{
		pDlg->m_strResFile = strResFile;
		pDlg->m_bSaveOk = TRUE;
		nRet = 0;
	}
	pDlg->m_bSaving = FALSE;
	return nRet;
}
void    CResOverDlg::ChgCtrlStatus(BOOL bEnable/* =TRUE*/)
{
	GetDlgItem(IDC_BTN_LOAD)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SAVE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_LOAD_THM_DIR)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_EXPORT_THM_DIR)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTC_EDT_NTAC)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_EDT_BKMK)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_RES_TYPE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_REPLACE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_UNDO)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_IMP_STR)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_EXP_STR)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_THEME)->EnableWindow(bEnable);
}
void CResOverDlg::OnSave() 
{
	// TODO: Add your control notification handler code here
	if(m_resParser.IsOverMaxSize())
	{
		if(AfxMessageBox(_T("It is over the max size!\nCan not save resource bin file!\nDo want to see the detail?"),MB_YESNO) == IDYES)
		{
			CDlgReport dlg;
			dlg.m_pResParser = &m_resParser;
			dlg.DoModal();
		}
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	//polo.jiang 20140410
	//由于usr/res压缩level大于2时，压缩比较耗时，故采用线程的方式处理，解决UI卡死的问题
	CWaitCursor wait;
	m_bSaveOk = FALSE;
	m_bSaving = TRUE;

	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Res files (*.bin)|*.bin|Theme files (*.thm)|*.thm||"));
	if(dlg.DoModal()== IDCANCEL)
		return;
	
	m_strSaveFile = dlg.GetPathName();
	ChgCtrlStatus(FALSE);
	CWinThread* pThread = AfxBeginThread(SaveResThread, this);
	MSG msg;
	::PostMessage(m_hWnd,WM_PROGRESS,PROG_BEGIN,100);
	while (m_bSaving)
	{
		for(int i=0 ;i< 100; i++)
		{
			::PostMessage(m_hWnd,WM_PROGRESS,PROG_PROCEED,i+1);
			while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			wait.Restore();
			Sleep(1000);
		}
	}
	::PostMessage(m_hWnd,WM_PROGRESS,PROG_END,100);
	ChgCtrlStatus(TRUE);
	GetDlgItem(IDC_BTN_IMP_STR)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_UNDO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SAVE)->EnableWindow(FALSE);
	if(m_bSaveOk)
	{	
		AfxMessageBox(_T("Success!"));
		Load(m_strResFile.operator LPCTSTR());
	}
	//////////////////////////////////////////////////////////////////////////
/*	CWaitCursor wait;
	BOOL bOK = FALSE;
	CString strResFile = m_strResFile;
//	if(AfxMessageBox(_T("Overwrite the original resource bin file?"),MB_YESNO) == IDYES)
//	{
//		wait.Restore();
//		bOK = m_resParser.SaveResFile(NULL);
//	}
//	else
	{
		CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			           _T("Res files (*.bin)|*.bin|Theme files (*.thm)|*.thm||"));
		if(dlg.DoModal()== IDCANCEL)
			return;

		wait.Restore();
		strResFile = dlg.GetPathName();
		bOK = m_resParser.SaveResFile(strResFile.operator LPCTSTR());

	}
	if(bOK)
	{
		m_strResFile = strResFile;
		AfxMessageBox(_T("Success!"));
		wait.Restore();

		Load(strResFile.operator LPCTSTR());
	}

	
*/
}

void CResOverDlg::OnUndo() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lstResItem.GetFirstSelectedItemPosition();
	int nItem = m_lstResItem.GetNextSelectedItem(pos);
	if(nItem == -1)
	{
		return;
	}

	DWORD dwIndex = m_lstResItem.GetItemData(nItem);

	int nSubItem = m_cmbTheme.GetCurSel();

	DWORD dwThemeIndex = m_cmbTheme.GetItemData(nSubItem);

	m_resParser.UndoResItem(dwIndex,dwThemeIndex);

	if(!m_resParser.m_vResItem[dwIndex]->isModified)
	{
		LVITEM lv = {0};
		lv.iItem = nItem;
		lv.iSubItem = 0;
		lv.mask = LVIF_IMAGE;
		lv.iImage = 0;
		m_lstResItem.SetItem(&lv);
	}

	GetDlgItem(IDC_BTN_UNDO)->EnableWindow(FALSE);
	m_strItemInfo = m_resParser.GetResItemInfo(dwIndex,dwThemeIndex);
	m_strResInfo = m_resParser.GetResInfo();
	UpdateData(FALSE);
}
void CResOverDlg::FillReslist()
{
	m_lstResItem.DeleteAllItems();

	UINT nCount = m_resParser.m_vResItem.size();
	CString strTmp;

	UINT i = 0;
	int nIdx =0;

	int nItem = m_cmbResType.GetCurSel();
	if(nItem == -1)
		return;

	DWORD dwResType = m_cmbResType.GetItemData(nItem);

	m_bInitList = TRUE;
	for(i = 0; i<nCount; i++)
	{
		RES_ITEM_PTR pRI = m_resParser.m_vResItem[i];			
		if(  dwResType == MAXDWORD || 
			(!pRI->bLogo && pRI->uResType == dwResType &&  dwResType != (MAXDWORD-1)) ||
			(pRI->bLogo &&  dwResType == (MAXDWORD-1)) )
		{
			m_lstResItem.InsertItem(nIdx,pRI->szDesc);
			strTmp.Format(_T("%d"),pRI->uResIndex);
			m_lstResItem.SetItemText(nIdx,1,strTmp);
			strTmp.Format(_T("%d"),pRI->uModuleID);
			m_lstResItem.SetItemText(nIdx,2,strTmp);
			m_lstResItem.SetItemText(nIdx,3,m_resParser.GetResTypeString(pRI->uResType));
			m_lstResItem.SetItemData(nIdx,i);
			if(pRI->isModified)
			{
				LVITEM lv = {0};
				lv.iItem = nIdx;
				lv.iSubItem = 0;
				lv.mask = LVIF_IMAGE;
				lv.iImage = MODIFIED_FALG_IMG_INDEX;
				m_lstResItem.SetItem(&lv);
			}
			nIdx++;
		}
	}
	m_bInitList = FALSE;

	if(m_lstResItem.GetItemCount() > 0)
	{
		LVITEM lv = {0};
		lv.iItem = 0;
		lv.iSubItem = 0;
		lv.mask = LVIF_STATE;
		lv.stateMask = LVIS_SELECTED;
		lv.state = LVIS_SELECTED;
		m_lstResItem.SetItem(&lv);

		int nIndex = (int)m_lstResItem.GetItemData(0);
		if(nIndex < 0 || nIndex >= (int)nCount)
		{
			return;
		}

		RES_ITEM_PTR pRI = m_resParser.m_vResItem[nIndex];
		
		if(!m_bDisable)
		{
			if(pRI->itemInfo[0].wExist == 0)
			{
				GetDlgItem(IDC_BTN_REPLACE)->EnableWindow(FALSE);
			}
			else
			{
				GetDlgItem(IDC_BTN_REPLACE)->EnableWindow(TRUE);
			}
			GetDlgItem(IDC_BTN_UNDO)->EnableWindow(pRI->isModified);
		}	

		int nThemeItem = m_cmbTheme.GetCurSel();
		if( nThemeItem < 0)
		{
			m_strItemInfo.Empty();
		}
		else
		{
			int nThemeIndex = (int)m_cmbTheme.GetItemData(nThemeItem);
			m_strItemInfo = m_resParser.GetResItemInfo(0,nThemeIndex);
			m_imgView.UpdateResItem(pRI,nThemeIndex);
		}	
		UpdateData(FALSE);
		
	}

}
void CResOverDlg::OnSelchangeResType() 
{
	// TODO: Add your control notification handler code here
	FillReslist();
}

void CResOverDlg::OnHelp() 
{
	// TODO: Add your control notification handler code here
	CXAboutDlg dlg;
	dlg.DoModal();
}

void CResOverDlg::OnExit() 
{
	// TODO: Add your control notification handler code here
	if(AfxMessageBox(_T("Do you want to exit now?"),MB_YESNO) == IDNO)
	{
		return ;
	}
	m_imgView.UpdateResItem(NULL,0);

	g_theApp.DeleteDirectory(m_strTmpFolder);

	CDialog::OnCancel();
}

void CResOverDlg::Load(LPCTSTR lpszResFile)
{
	CWaitCursor wait;

	m_imgView.UpdateResItem(NULL,0);

	MSG msg;
	while(PeekMessage(&msg,this->m_hWnd,0,0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	g_theApp.DeleteDirectory(m_strTmpFolder.operator LPCTSTR());
	CreateDirectory(m_strTmpFolder.operator LPCTSTR(),NULL);


	wait.Restore();

	CString strFile = lpszResFile;

	m_lstResItem.DeleteAllItems();

	BOOL bOK = m_resParser.LoadResFile(strFile);	

	if(bOK)
	{
		m_strResFile = lpszResFile;
		InitCtrls();
		m_strResInfo = m_resParser.GetResInfo();
	//	FillReslist();
		UpdateData(FALSE);
	}

	ChangeItemState(bOK);
}

void CResOverDlg::OnBtnLoadThmDir() 
{
	// TODO: Add your control notification handler code here
	CString strThmFolder = GetFolder();
	if(!strThmFolder.IsEmpty())
	{
		CWaitCursor wait;

		BOOL bOK = m_resParser.LoadThmFolder(strThmFolder);

		FillReslist();
			
		if(bOK)
		{
			AfxMessageBox(_T("Load success!"));
			GetDlgItem(IDC_BTN_SAVE)->EnableWindow(TRUE);
		}
		else
		{
			AfxMessageBox(_T("Load failed!"));
		}

	}
}

void CResOverDlg::OnBtnExportThmDir() 
{
	// TODO: Add your control notification handler code here
	CString strThmFolder = GetFolder();
	if(!strThmFolder.IsEmpty())
	{
		CWaitCursor wait;
		if(m_resParser.ExportThmFolder(strThmFolder))
		{
			AfxMessageBox(_T("Export success!"));
		}
		else
		{
			AfxMessageBox(_T("Load failed!"));
		}
	}
}

void CResOverDlg::OnExpStr() 
{
	// TODO: Add your control notification handler code here
	CString strFilter = _T("Excel (97-2003) (*.xls)|*.xls||");
	if(m_resParser.m_nExcelParserType != 2)
	{
		strFilter = _T("Excel files (*.xls,*xlsx)|*.xls;*xlsx||");
	}
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,strFilter);
	if(dlg.DoModal() == IDCANCEL)
		return;

	MSG msg;
	while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CWaitCursor wait;
/*
	m_strExcelFile = dlg.GetPathName();
	m_bImportString = FALSE;
	
	DWORD dwThreadID = 0;
	HANDLE hThread = CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)GetThreadFunc,
		(LPVOID)this,
		NULL,
		&dwThreadID);
	
	MSG msg;
	while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}	
	if( hThread == NULL)
	{
		if(m_resParser.ExportString(m_strExcelFile.operator LPCTSTR()))
		{
			AfxMessageBox(_T("Export string complete!"));
		}
		else
		{
			AfxMessageBox(_T("Export string failed!"));
		}

		ChangeItemState(TRUE);
	}
	else
	{
		WaitForSingleObject( hThread, INFINITE);	
		CloseHandle( hThread);
		hThread = NULL;
	}
*/

	CString strPath = dlg.GetPathName();
	CString strName = dlg.GetFileName();
	if(strName.GetLength()<5)
	{
		strPath += _T(".xls");
	}
	else
	{
		if(strName.Right(4).CompareNoCase(_T(".xls")) != 0 && 
           strName.Right(4).CompareNoCase(_T("xlsx")) != 0)
		{
			strPath += _T(".xls");
		}
	}
	
	if(m_resParser.ExportString(strPath.operator LPCTSTR()))
	{
		AfxMessageBox(_T("Export string complete!"));
	}
	else
	{
		AfxMessageBox(_T("Export string failed!"));
	}

	ChangeItemState(TRUE);
	
}

void CResOverDlg::OnImpStr() 
{
	// TODO: Add your control notification handler code here
	CString strFilter = _T("Excel (97-2003) (*.xls)|*.xls||");
	if(m_resParser.m_nExcelParserType != 2)
	{
		strFilter = _T("Excel files (*.xls,*xlsx)|*.xls;*xlsx||");
	}

	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,strFilter);
	if(dlg.DoModal() == IDCANCEL)
		return;
	
	MSG msg;
	while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CWaitCursor wait;

/*
	m_strExcelFile = dlg.GetPathName();
	m_bImportString = TRUE;

	DWORD dwThreadID = 0;
	HANDLE hThread = CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)GetThreadFunc,
		(LPVOID)this,
		NULL,
		&dwThreadID);
	
	MSG msg;
	while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}	
	if( hThread == NULL)
	{
		if(m_resParser.ImportString(m_strExcelFile.operator LPCTSTR()))
		{
			AfxMessageBox(_T("Import string complete!"));
		}
		else
		{
			AfxMessageBox(_T("Import string failed!"));
		}
		
		ChangeItemState(TRUE);	
	}
	else
	{
		WaitForSingleObject( hThread, INFINITE);	
		CloseHandle( hThread);
		hThread = NULL;
	}
*/
	
	if(m_resParser.ImportString(dlg.GetPathName().operator LPCTSTR()))
	{
		AfxMessageBox(_T("Import string complete!"));
	}
	else
	{
		AfxMessageBox(_T("Import string failed!"));
	}

	ChangeItemState(TRUE);
    
}
void CResOverDlg::OnEdtMtva() 
{
	// TODO: Add your control notification handler code here
	CDlgMtva dlg;
	dlg.m_bViewOnly = FALSE;
	dlg.DoModal();
}

void CResOverDlg::OnEdtNtac() 
{
	// TODO: Add your control notification handler code here
	CDlgNtac dlg;
	dlg.m_bViewOnly = FALSE;
	dlg.DoModal();
}

void CResOverDlg::OnEdtBkmk() 
{
	// TODO: Add your control notification handler code here
	CDlgBkmk dlg;
	dlg.m_bViewOnly = FALSE;
	dlg.DoModal();
}

DWORD WINAPI CResOverDlg::GetThreadFunc(LPVOID lpParam)
{
	CResOverDlg *pDlg = (CResOverDlg *)lpParam;	
	return pDlg->ImpExpFunc();
}

DWORD CResOverDlg::ImpExpFunc()
{
	CWaitCursor wait;
	if(m_bImportString)
	{
		if(m_resParser.ImportString(m_strExcelFile.operator LPCTSTR()))
		{
			AfxMessageBox(_T("Import string complete!"));
		}
		else
		{
			AfxMessageBox(_T("Import string failed!"));
		}
	}
	else
	{
		if(m_resParser.ExportString(m_strExcelFile.operator LPCTSTR()))
		{
			AfxMessageBox(_T("Export string complete!"));
		}
		else
		{
			AfxMessageBox(_T("Export string failed!"));
		}
	}
	
	ChangeItemState(TRUE);

	return 0;
}

LRESULT CResOverDlg::OnProgressMsg(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case PROG_BEGIN:
		//m_prog.ShowWindow(SW_SHOW);
		m_prog.SetRange32(0, lParam);
		m_prog.SetPos(0);
		break;
	case PROG_PROCEED:
		m_prog.SetPos(lParam);
		break;
	case PROG_END:
		if(lParam == 100)
		{
			int nLower = 0;
			int nUpper = 0;
			m_prog.GetRange(nLower,nUpper);
			m_prog.SetPos(nUpper);
		}
		m_prog.SetPos(0);
		//m_prog.ShowWindow(SW_HIDE);
		break;
	default:
		_ASSERTE(0);
	}
	return 0;
}