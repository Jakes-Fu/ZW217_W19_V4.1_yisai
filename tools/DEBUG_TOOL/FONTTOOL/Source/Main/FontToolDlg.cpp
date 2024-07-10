// FontToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FontTool.h"
#include "FontToolDlg.h"
#include "Global.h"


#include "GlyphData.h"
#include "DlgOTFSeting.h"
#include "XAboutDlg.h"

#include "DlgAdjustParam.h"


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

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
// CFontToolDlg dialog

CFontToolDlg::CFontToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFontToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFontToolDlg)
	m_strCurUCcode = _T("");
	m_strChar = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bFromClick = FALSE;
	m_bFromInput = FALSE;
	m_bFromFind = FALSE;
	m_bFromFontSetCmb = FALSE;
	m_dwUCBegin = 0x20;
	m_dwLCBegin = 0x0;
	m_pCSI = NULL;
	m_nSortBySel = 0;
	m_nPtSizeIdx = 0;
}

void CFontToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFontToolDlg)
	DDX_Control(pDX, IDC_CMB_FONT_SIZE, m_cmbPtSize);
	DDX_Control(pDX, IDC_CMB_SORT_BY, m_cmbSortBy);
	DDX_Control(pDX, IDC_STC_WORKING, m_stcWorking);
	DDX_Control(pDX, IDC_EDT_CHAR, m_edtChar);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_EDT_CUR_UCCODE, m_edtCurUCcode);
	DDX_Control(pDX, IDC_SCB, m_scb);
	 DDX_Control(pDX, IDC_STC_R0_C0, 	m_csR0C0);
	 DDX_Control(pDX, IDC_STC_R0_C1, 	m_csR0C1);
	 DDX_Control(pDX, IDC_STC_R0_C2,	m_csR0C2);
	 DDX_Control(pDX, IDC_STC_R0_C3,	m_csR0C3);
	 DDX_Control(pDX, IDC_STC_R0_C4,	m_csR0C4);
	 DDX_Control(pDX, IDC_STC_R0_C5,	m_csR0C5);
	 DDX_Control(pDX, IDC_STC_R0_C6,	m_csR0C6);
	 DDX_Control(pDX, IDC_STC_R0_C7,	m_csR0C7);
	 DDX_Control(pDX, IDC_STC_R0_C8,	m_csR0C8);
	 DDX_Control(pDX, IDC_STC_R0_C9,	m_csR0C9);
	 DDX_Control(pDX, IDC_STC_R0_C10,	m_csR0C10);
	 DDX_Control(pDX, IDC_STC_R0_C11,	m_csR0C11);
	 DDX_Control(pDX, IDC_STC_R0_C12,	m_csR0C12);
	 DDX_Control(pDX, IDC_STC_R0_C13,	m_csR0C13);
	 DDX_Control(pDX, IDC_STC_R0_C14,	m_csR0C14);
	 DDX_Control(pDX, IDC_STC_R0_C15,	m_csR0C15);
	 DDX_Control(pDX, IDC_STC_R1_C0, 	m_csR1C0);
	 DDX_Control(pDX, IDC_STC_R1_C1, 	m_csR1C1);
	 DDX_Control(pDX, IDC_STC_R1_C2,	m_csR1C2);
	 DDX_Control(pDX, IDC_STC_R1_C3,	m_csR1C3);
	 DDX_Control(pDX, IDC_STC_R1_C4,	m_csR1C4);
	 DDX_Control(pDX, IDC_STC_R1_C5,	m_csR1C5);
	 DDX_Control(pDX, IDC_STC_R1_C6,	m_csR1C6);
	 DDX_Control(pDX, IDC_STC_R1_C7,	m_csR1C7);
	 DDX_Control(pDX, IDC_STC_R1_C8,	m_csR1C8);
	 DDX_Control(pDX, IDC_STC_R1_C9,	m_csR1C9);
	 DDX_Control(pDX, IDC_STC_R1_C10,	m_csR1C10);
	 DDX_Control(pDX, IDC_STC_R1_C11,	m_csR1C11);
	 DDX_Control(pDX, IDC_STC_R1_C12,	m_csR1C12);
	 DDX_Control(pDX, IDC_STC_R1_C13,	m_csR1C13);
	 DDX_Control(pDX, IDC_STC_R1_C14,	m_csR1C14);
	 DDX_Control(pDX, IDC_STC_R1_C15,	m_csR1C15);
	 DDX_Control(pDX, IDC_STC_R2_C0, 	m_csR2C0);
	 DDX_Control(pDX, IDC_STC_R2_C1, 	m_csR2C1);
	 DDX_Control(pDX, IDC_STC_R2_C2,	m_csR2C2);
	 DDX_Control(pDX, IDC_STC_R2_C3,	m_csR2C3);
	 DDX_Control(pDX, IDC_STC_R2_C4,	m_csR2C4);
	 DDX_Control(pDX, IDC_STC_R2_C5,	m_csR2C5);
	 DDX_Control(pDX, IDC_STC_R2_C6,	m_csR2C6);
	 DDX_Control(pDX, IDC_STC_R2_C7,	m_csR2C7);
	 DDX_Control(pDX, IDC_STC_R2_C8,	m_csR2C8);
	 DDX_Control(pDX, IDC_STC_R2_C9,	m_csR2C9);
	 DDX_Control(pDX, IDC_STC_R2_C10,	m_csR2C10);
	 DDX_Control(pDX, IDC_STC_R2_C11,	m_csR2C11);
	 DDX_Control(pDX, IDC_STC_R2_C12,	m_csR2C12);
	 DDX_Control(pDX, IDC_STC_R2_C13,	m_csR2C13);
	 DDX_Control(pDX, IDC_STC_R2_C14,	m_csR2C14);
	 DDX_Control(pDX, IDC_STC_R2_C15,	m_csR2C15);
	 DDX_Control(pDX, IDC_STC_R3_C0, 	m_csR3C0);
	 DDX_Control(pDX, IDC_STC_R3_C1, 	m_csR3C1);
	 DDX_Control(pDX, IDC_STC_R3_C2,	m_csR3C2);
	 DDX_Control(pDX, IDC_STC_R3_C3,	m_csR3C3);
	 DDX_Control(pDX, IDC_STC_R3_C4,	m_csR3C4);
	 DDX_Control(pDX, IDC_STC_R3_C5,	m_csR3C5);
	 DDX_Control(pDX, IDC_STC_R3_C6,	m_csR3C6);
	 DDX_Control(pDX, IDC_STC_R3_C7,	m_csR3C7);
	 DDX_Control(pDX, IDC_STC_R3_C8,	m_csR3C8);
	 DDX_Control(pDX, IDC_STC_R3_C9,	m_csR3C9);
	 DDX_Control(pDX, IDC_STC_R3_C10,	m_csR3C10);
	 DDX_Control(pDX, IDC_STC_R3_C11,	m_csR3C11);
	 DDX_Control(pDX, IDC_STC_R3_C12,	m_csR3C12);
	 DDX_Control(pDX, IDC_STC_R3_C13,	m_csR3C13);
	 DDX_Control(pDX, IDC_STC_R3_C14,	m_csR3C14);
	 DDX_Control(pDX, IDC_STC_R3_C15,	m_csR3C15);
	 DDX_Control(pDX, IDC_STC_R4_C0, 	m_csR4C0);
	 DDX_Control(pDX, IDC_STC_R4_C1, 	m_csR4C1);
	 DDX_Control(pDX, IDC_STC_R4_C2,	m_csR4C2);
	 DDX_Control(pDX, IDC_STC_R4_C3,	m_csR4C3);
	 DDX_Control(pDX, IDC_STC_R4_C4,	m_csR4C4);
	 DDX_Control(pDX, IDC_STC_R4_C5,	m_csR4C5);
	 DDX_Control(pDX, IDC_STC_R4_C6,	m_csR4C6);
	 DDX_Control(pDX, IDC_STC_R4_C7,	m_csR4C7);
	 DDX_Control(pDX, IDC_STC_R4_C8,	m_csR4C8);
	 DDX_Control(pDX, IDC_STC_R4_C9,	m_csR4C9);
	 DDX_Control(pDX, IDC_STC_R4_C10,	m_csR4C10);
	 DDX_Control(pDX, IDC_STC_R4_C11,	m_csR4C11);
	 DDX_Control(pDX, IDC_STC_R4_C12,	m_csR4C12);
	 DDX_Control(pDX, IDC_STC_R4_C13,	m_csR4C13);
	 DDX_Control(pDX, IDC_STC_R4_C14,	m_csR4C14);
	 DDX_Control(pDX, IDC_STC_R4_C15,	m_csR4C15);
	 DDX_Control(pDX, IDC_STC_R5_C0, 	m_csR5C0);
	 DDX_Control(pDX, IDC_STC_R5_C1, 	m_csR5C1);
	 DDX_Control(pDX, IDC_STC_R5_C2,	m_csR5C2);
	 DDX_Control(pDX, IDC_STC_R5_C3,	m_csR5C3);
	 DDX_Control(pDX, IDC_STC_R5_C4,	m_csR5C4);
	 DDX_Control(pDX, IDC_STC_R5_C5,	m_csR5C5);
	 DDX_Control(pDX, IDC_STC_R5_C6,	m_csR5C6);
	 DDX_Control(pDX, IDC_STC_R5_C7,	m_csR5C7);
	 DDX_Control(pDX, IDC_STC_R5_C8,	m_csR5C8);
	 DDX_Control(pDX, IDC_STC_R5_C9,	m_csR5C9);
	 DDX_Control(pDX, IDC_STC_R5_C10,	m_csR5C10);
	 DDX_Control(pDX, IDC_STC_R5_C11,	m_csR5C11);
	 DDX_Control(pDX, IDC_STC_R5_C12,	m_csR5C12);
	 DDX_Control(pDX, IDC_STC_R5_C13,	m_csR5C13);
	 DDX_Control(pDX, IDC_STC_R5_C14,	m_csR5C14);
	 DDX_Control(pDX, IDC_STC_R5_C15,	m_csR5C15);
	DDX_Control(pDX, IDC_STC_DISP, m_stcDisp);
	DDX_Text(pDX, IDC_EDT_CUR_UCCODE, m_strCurUCcode);
	DDV_MaxChars(pDX, m_strCurUCcode, 4);
	DDX_Text(pDX, IDC_EDT_CHAR, m_strChar);
	DDV_MaxChars(pDX, m_strChar, 1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFontToolDlg, CDialog)
	//{{AFX_MSG_MAP(CFontToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LOAD_BDF, OnBtnLoadBdf)
	ON_BN_CLICKED(IDC_BTN_SAVE_DOTMATRIX, OnBtnSaveDotmatrix)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDT_CUR_UCCODE, OnChangeEdtCurUccode)
	ON_BN_CLICKED(IDC_BTN_LOAD_TTF, OnBtnLoadTtf)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BTN_LOAD_DMF, OnBtnLoadDmf)
	ON_EN_CHANGE(IDC_EDT_CHAR, OnChangeEdtChar)
	ON_BN_CLICKED(IDCANCEL, OnBtnExit)
	ON_BN_CLICKED(IDC_BTN_SAVE_BDF, OnBtnSaveBdf)
	ON_CBN_SELCHANGE(IDC_CMB_SORT_BY, OnSelchangeCmbSortBy)
	ON_CBN_SELCHANGE(IDC_CMB_FONT_SIZE, OnSelchangeCmbFontSize)
	ON_BN_CLICKED(IDC_BTN_ADJUST_PARAM, OnBtnAdjustParam)
	ON_BN_CLICKED(IDC_BTN_MINUS, OnBtnMinus)
	ON_BN_CLICKED(IDC_BTN_PLUS, OnBtnPlus)
	ON_BN_CLICKED(IDC_BTN_REPL_BDF, OnReplBdf)
	//}}AFX_MSG_MAP
    ON_COMMAND_RANGE( IDC_STC_R0_C0 , IDC_STC_R5_C15, OnClickFont )
	ON_MESSAGE(WM_PROGRESS, OnProgressMsg)
	ON_MESSAGE(WM_LOAD_SETTING,OnLoadSetting)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontToolDlg message handlers

BOOL CFontToolDlg::OnInitDialog()
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
    m_stcDisp.m_bDrawGrid = TRUE;
	m_stcDisp.m_uBkColor = RGB(255,255,255);
	m_stcDisp.m_bIsDown = TRUE;
	m_stcDisp.m_bOnePixel = FALSE;

	ChangeBtnState();

//	m_bdf.m_hWnd = this->m_hWnd;
	m_ftParser.Init(this->m_hWnd);
	m_progress.ShowWindow(SW_HIDE);
	m_stcWorking.ShowWindow(SW_HIDE);
//	m_cmbFontSet.InsertString(0,_T("ALL"));
//	m_cmbFontSet.SetCurSel(0);
	m_cmbSortBy.SetCurSel(0);
	m_cmbSortBy.EnableWindow(FALSE);
	m_cmbPtSize.SetCurSel(0);
	m_cmbPtSize.EnableWindow(FALSE);
//	m_cmbFontSet.EnableWindow(FALSE);
	
	PostMessage(WM_LOAD_SETTING);
//  just for pc-lint with no error 755	
    UNUSED_ALWAYS(IDC_STC_LINE_ONE);
    UNUSED_ALWAYS(IDC_STC_LINE_TWO);  
    UNUSED_ALWAYS(IDC_STC_LINE_THREE);
    UNUSED_ALWAYS(IDC_STC_LINE_FOR);  
    UNUSED_ALWAYS(IDC_STC_LINE_FIVE);
    UNUSED_ALWAYS(IDC_STC_LINE_SIX);  
	
	CXVerInfo vi;
	CString strVer = vi.GetSPRDVersionString();
	CString strTitle;

#ifdef _DEBUG
	strTitle.Format(_T("FontTool - D%s"),strVer.operator LPCTSTR());
#else
	strTitle.Format(_T("FontTool - R%s"),strVer.operator LPCTSTR());
#endif
	SetWindowText(strTitle.operator LPCTSTR());

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFontToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFontToolDlg::OnPaint() 
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
HCURSOR CFontToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFontToolDlg::OnBtnLoadBdf() 
{
	CDlgOTFSeting otfDlg;
	otfDlg.m_bOtf = FALSE;
	if(otfDlg.DoModal() == IDCANCEL)
	{
		return;
	}
	
	m_cmbPtSize.EnableWindow(FALSE);
	m_cmbSortBy.EnableWindow(FALSE);

	EnableWindow(FALSE);

	Clear();
	m_ftParser.Clear();
	m_pCSI = otfDlg.m_pCSI;
	m_ftParser.SetCharSetInfo(m_pCSI);

	if(!m_ftParser.LoadBdfs(otfDlg.m_agBdfs,otfDlg.m_agExBdfs))
	{
		AfxMessageBox(_T("Load failed!"));
		EnableWindow(TRUE);
		return;
	}
	m_pCSI = m_ftParser.GetCharSetInfo();
	Fresh();

	EnableWindow(TRUE);

}

void CFontToolDlg::FillGlyphGrid(UINT uCode,BOOL bClean /*= FALSE*/)
{
	UINT uIdxStart = IDC_STC_R0_C0;

	//PBDF_CHAR_T pCurChar = NULL;
	GLYPH_BODY_PTR pGlyph = NULL;
	
	CString strText;

	BOOL bUnicodeSort = TRUE;

	if(m_nSortBySel == 1)
	{
		bUnicodeSort = FALSE;
	}

	CUIntArray * pUA = &m_agGlyfID;

	for(int i = 0; i< 6; i++)
	{
		for( int j = 0; j< 16; j++)
		{
			CCoolStatic * pWnd = (CCoolStatic *)GetDlgItem(uIdxStart+i*16 +j);
			
			if(pWnd == NULL)
				continue;

			DWORD dwCurCode = i*16 + j + uCode;
			if(!bUnicodeSort && pUA != NULL)
			{
				if(dwCurCode < (DWORD)pUA->GetSize())
				{
					dwCurCode = pUA->GetAt(dwCurCode);
				}
				else
				{
					dwCurCode = 0;
				}
			}
			

			if(!bClean)
			{
                pGlyph = NULL;							

				pWnd->m_pGlyph = m_ftParser.GetGlyph(dwCurCode,m_nPtSizeIdx,bUnicodeSort); 

				pWnd->m_nFBBY = m_ftParser.GetPointSize(m_nPtSizeIdx);
				pWnd->m_nMaxBearingY = m_ftParser.GetMaxBearingY(m_nPtSizeIdx);
				pWnd->m_bAlpha = (m_ftParser.GetGlyfTblHdr(m_nPtSizeIdx)->type != FONT_TYPE_MONO) ? TRUE: FALSE;

				if(pWnd->m_pGlyph != NULL)
				{				
					pWnd->m_nFBBX = pWnd->m_pGlyph->dwidth;
					if(pWnd->m_nFBBX == 0 && pWnd->m_pGlyph->width != 0)
					{
						pWnd->m_nFBBX = pWnd->m_pGlyph->width;
					}
				}
				else
				{
					pWnd->m_nFBBX = pWnd->m_nFBBY;
				}

				pWnd->m_bNormalGlyf = TRUE;
		
			}
			else
			{
				pWnd->m_pGlyph = NULL;
				pWnd->m_bIsDown = FALSE;
				pWnd->m_nFBBX = 0;
				pWnd->m_nFBBY = 0;
				pWnd->m_nMaxBearingY = 0;
				pWnd->m_bNormalGlyf = TRUE;
			}

			pWnd->m_uUnicode = dwCurCode;
			if(!bUnicodeSort)
			{
				pWnd->m_uUnicode = m_ftParser.GetUniFormID(dwCurCode);
				if(pWnd->m_uUnicode == 0 && pWnd->m_pGlyph != NULL)
					pWnd->m_bNormalGlyf = FALSE;
			}

			if(pWnd->m_bIsDown && !m_bFromInput)
			{
				m_stcDisp.m_pGlyph = pWnd->m_pGlyph;
				m_stcDisp.m_nFBBX = pWnd->m_nFBBX;
				m_stcDisp.m_nFBBY = pWnd->m_nFBBY * 3/2;
				m_stcDisp.m_nMaxBearingY = pWnd->m_nMaxBearingY;
				m_stcDisp.m_bAlpha = pWnd->m_bAlpha;
				m_stcDisp.Invalidate();
                m_bFromClick = TRUE;
				_TCHAR cSel = _T('\0');
				if(pWnd->m_pGlyph != NULL )
				{				
					cSel = (_TCHAR)(pWnd->m_uUnicode);
					strText.Format(_T("%04X"),pWnd->m_uUnicode);
					GetDlgItem(IDC_EDT_CUR_UCCODE)->SetWindowText(strText);	
					if(!bUnicodeSort)
					{					
						strText.Format(_T("%04X"),dwCurCode);
					}
					else
					{
						strText.Format(_T("%04X"),m_ftParser.GetIDFromUni(dwCurCode));
					}
					GetDlgItem(IDC_EDT_GLYF_ID)->SetWindowText(strText);	

					UpdateGlyfInfo(pWnd->m_nFBBY,pWnd->m_pGlyph->dwidth,pWnd->m_pGlyph->height,
						           pWnd->m_pGlyph->width,pWnd->m_pGlyph->x,pWnd->m_pGlyph->y);
				}
				else
				{
					GetDlgItem(IDC_EDT_CUR_UCCODE)->SetWindowText(_T("0000"));	
					GetDlgItem(IDC_EDT_GLYF_ID)->SetWindowText(_T("0000"));
					UpdateGlyfInfo(pWnd->m_nFBBY,pWnd->m_nFBBX,0,0,0,0);

				}
				strText = cSel;
				m_edtChar.SetWindowText(strText);
				m_bFromClick = FALSE;
			}

			pWnd->Invalidate();			
		}
	}


	for(UINT k= IDC_STC_LINE_ONE; k <= IDC_STC_LINE_SIX; k++)
	{
		strText.Format(_T("%04X"),uCode + (k-IDC_STC_LINE_ONE)*16);
		GetDlgItem(k)->SetWindowText(strText);
	}
}

void CFontToolDlg::OnBtnSaveDotmatrix() 
{
	CFileDialog dlg(FALSE,_T(".LIB"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("font files (*.lib)|*.lib||"));
	if(dlg.DoModal() == IDCANCEL)
		return;
	CString strFileName = dlg.GetPathName();

	static _TCHAR szPath[_MAX_PATH] = {0};
	::GetModuleFileName(NULL,szPath,_MAX_PATH);
	CString strFile = szPath;
	int nFind = strFile.ReverseFind(_T('\\'));
	strFile = strFile.Left(nFind + 1);
	CString strFontToolIni = strFile +  _T("FontTool.ini");
	BOOL bBigEndian = ::GetPrivateProfileInt(_T("Settings"),_T("BigEndian"),1,strFontToolIni);
	
	EnableWindow(FALSE);
	m_ftParser.Save(strFileName,TRUE,bBigEndian);
	EnableWindow(TRUE);
	AfxMessageBox(_T("Complete!"));
}

void CFontToolDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(!m_ftParser.IsReady())
	{
		CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
		return;
	}

	int minpos = 0;
	int maxpos = 0;
	//int maxpos = (m_bdf.m_tBH.dwUCEnd- (m_bdf.m_tBH.dwUCEnd % 16)-m_dwUCBegin )/16 -5;
    m_scb.GetScrollRange(&minpos,&maxpos);
	
	int curpos = m_scb.GetScrollPos();
    
	switch (nSBCode)   
	{   
	case SB_TOP:
		curpos = minpos;
		m_scb.SetScrollPos(curpos);   
		break;   
		
	case SB_BOTTOM:	
		curpos = maxpos;
		m_scb.SetScrollPos(curpos);   
		break;   
		
	case SB_ENDSCROLL:
// 		if(curpos > maxpos)
// 		{
// 			curpos = maxpos;
// 			m_scb.SetScrollPos(curpos); 
// 		}
// 		else if(curpos < minpos)
// 		{
// 			curpos = minpos;
// 			m_scb.SetScrollPos(curpos); 
// 		}		 
		break;   
		
	case SB_LINEUP:
		if (curpos > minpos)   
		{   
			curpos -= 1;     
			m_scb.SetScrollPos(curpos);   
		}   
		
		break;   
		
	case SB_LINEDOWN:   
		if(curpos < maxpos)   
		{   
			curpos += 1;     
			m_scb.SetScrollPos(curpos);   
		}     
 
		break;   
		
	case SB_PAGEUP:  
		if(curpos > minpos)
		{
			curpos = max(minpos,   curpos - 6);  
			m_scb.SetScrollPos(curpos);
		}      
		break;   
		
	case SB_PAGEDOWN: 		
		if(curpos < maxpos)
		{
			curpos = min(maxpos, curpos + 6); 
			m_scb.SetScrollPos(curpos);
		}
		break;   
		
	case SB_THUMBPOSITION:
		curpos = nPos;   
		m_scb.SetScrollPos(curpos);
		break;   
		
// 	case SB_THUMBTRACK:
// 		curpos = nPos;   
// 		m_scb.SetScrollPos(curpos);
// 		break;   
	  }   



	if(m_nSortBySel == 0)
	{
		FillGlyphGrid(m_dwUCBegin+curpos*16);
	}
	else
	{
		FillGlyphGrid(m_dwLCBegin+curpos*16);
	}


	
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CFontToolDlg::OnChangeEdtCurUccode() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
	if(m_pCSI == NULL || !m_ftParser.IsReady() || m_bFromClick || m_bFromFind)
		return;

	CString strText;
	GetDlgItem(IDC_EDT_CUR_UCCODE)->GetWindowText(strText);

	UINT uUCCode = 0;
    
	_stscanf(strText,_T("%04X"),&uUCCode);
	CString strChar = (_TCHAR)uUCCode;
	m_bFromInput = TRUE;
	m_edtChar.SetWindowText(strChar);
	m_bFromInput = FALSE;

	GLYPH_BODY_PTR pGlyph = NULL;

	int nMaxPos = 0;
	int nMinPos = 0;
	int nPos = 0;

	pGlyph = m_ftParser.GetGlyph(uUCCode,m_nPtSizeIdx);
    

	if(pGlyph != NULL)
	{
		m_stcDisp.m_pGlyph  = pGlyph;
		m_stcDisp.m_uUnicode = uUCCode;
		m_stcDisp.m_nFBBY = m_ftParser.GetPointSize(m_nPtSizeIdx) * 3/2;
		m_stcDisp.m_nFBBX = pGlyph->dwidth;
		m_stcDisp.m_nMaxBearingY = m_ftParser.GetMaxBearingY(m_nPtSizeIdx);
		m_stcDisp.m_bAlpha = (m_ftParser.GetGlyfTblHdr(m_nPtSizeIdx)->type != FONT_TYPE_MONO) ? TRUE: FALSE;					

		if(m_stcDisp.m_nFBBX == 0 && m_stcDisp.m_pGlyph->width != 0)
		{
			m_stcDisp.m_nFBBX =m_stcDisp.m_pGlyph->width;
		}
		m_stcDisp.m_bNormalGlyf = TRUE;

		m_stcDisp.Invalidate();	

		UpdateGlyfInfo(m_stcDisp.m_nFBBY,pGlyph->dwidth,pGlyph->height,pGlyph->width,
			           pGlyph->x,pGlyph->y);

		UINT uCurStart = 0;
		UINT uCurEnd = 0;
		GetDlgItem(IDC_STC_LINE_ONE)->GetWindowText(strText);
		_stscanf(strText,_T("%04X"),&uCurStart);
		GetDlgItem(IDC_STC_LINE_SIX)->GetWindowText(strText);
		_stscanf(strText,_T("%04X"),&uCurEnd);
		uCurEnd += 15;

		DWORD dwUCEnd = m_pCSI->pUCBoundTable[m_pCSI->nSectorNum-1].dwUCEnd;

		if(uUCCode>uCurEnd || uUCCode < uCurStart)
		{
			m_bFromInput = TRUE;
			if(uUCCode < (dwUCEnd - dwUCEnd %16 - 16 * 5))
				FillGlyphGrid(uUCCode - (uUCCode % 16));
			else
				FillGlyphGrid((dwUCEnd - dwUCEnd %16 - 16 * 5));
 			m_bFromInput = FALSE;
			
			GetDlgItem(IDC_STC_LINE_ONE)->GetWindowText(strText);
			_stscanf(strText,_T("%04X"),&uCurStart);
			GetDlgItem(IDC_STC_LINE_SIX)->GetWindowText(strText);
			_stscanf(strText,_T("%04X"),&uCurEnd);
			uCurEnd += 15;
			
			nPos = (uUCCode - (uUCCode % 16)-m_dwUCBegin)/16;
			nMaxPos = 0;
			nMinPos = 0;
			m_scb.GetScrollRange(&nMinPos,&nMaxPos);
			if(nPos >  nMaxPos)
				nPos = nMaxPos;
			m_scb.SetScrollPos(nPos);
		}


		int nIdx = uUCCode - uCurStart;

		CCoolStatic * pWnd = (CCoolStatic *)GetDlgItem(IDC_STC_R0_C0 + nIdx);
		pWnd->m_bIsDown = TRUE;
		pWnd->Invalidate();

		for(int i = 0; i<96;i++)
		{
			if(i != nIdx)
			{
				pWnd = (CCoolStatic *)GetDlgItem(IDC_STC_R0_C0 + i);
				pWnd->m_bIsDown = FALSE;
				pWnd->Invalidate();
			}
		}			
	}	

}

BOOL CFontToolDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd * pWnd = GetDlgItem(IDC_EDT_CUR_UCCODE);
    _ASSERTE( pWnd != NULL );

    CWnd * pFocus = GetFocus();
    if( pMsg->message == WM_CHAR && pFocus != NULL && pWnd != NULL &&  pFocus->m_hWnd == pWnd->m_hWnd )
    {
        _TCHAR cInput = (_TCHAR)pMsg->wParam;
        if( cInput < _T('0') || cInput > _T('9') )
        {
            if( cInput < _T('a') || cInput > _T('f') )
            {
                if( cInput != _T('_') && (cInput < _T('A') || cInput > _T('F')) && cInput != 8 /* backspace*/ )
                {
                    if( !(GetKeyState(VK_LCONTROL) & 0x0100) )
                        return TRUE;
                }
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

void CFontToolDlg::OnClickFont( UINT nID )
{
	if(!m_ftParser.IsReady())
		return;

	CString strText;
	CCoolStatic * pWnd = (CCoolStatic *)GetDlgItem(nID);
	if(pWnd->m_pGlyph != NULL )
	{		

		m_bFromClick = TRUE;
		strText.Format(_T("%04X"),pWnd->m_uUnicode);
		GetDlgItem(IDC_EDT_CUR_UCCODE)->SetWindowText(strText);
		strText = (_TCHAR)pWnd->m_uUnicode;
		m_edtChar.SetWindowText(strText);
		m_bFromClick = FALSE;

		UINT dwCode = 0;
		
		GetDlgItem(IDC_STC_LINE_ONE)->GetWindowText(strText);
		_stscanf(strText,_T("%04X"),&dwCode);

		if(m_nSortBySel==0)
		{
			strText.Format(_T("%04X"),m_ftParser.GetIDFromUni(pWnd->m_uUnicode));
		}
		else
		{
			strText = _T("");
			dwCode += (nID - IDC_STC_R0_C0);
			if(m_pCSI != NULL)
			{
				if(dwCode< (UINT)m_agGlyfID.GetSize())
				{
					strText.Format(_T("%04X"),m_agGlyfID.GetAt(dwCode));
				}			
			}
		}
		GetDlgItem(IDC_EDT_GLYF_ID)->SetWindowText(strText);



		UpdateGlyfInfo(pWnd->m_nFBBY,pWnd->m_pGlyph->dwidth,
			           pWnd->m_pGlyph->height,pWnd->m_pGlyph->width,
					   pWnd->m_pGlyph->x,pWnd->m_pGlyph->y);
	}
	else
	{
		m_bFromClick = TRUE;
		UINT dwCode = 0;
		
		GetDlgItem(IDC_STC_LINE_ONE)->GetWindowText(strText);
		_stscanf(strText,_T("%04X"),&dwCode);

		if(m_nSortBySel==0)
		{
			dwCode += (nID - IDC_STC_R0_C0);
		}
		else
		{
			dwCode = 0;
		}
		strText.Format(_T("%04X"),dwCode);
		
		GetDlgItem(IDC_EDT_CUR_UCCODE)->SetWindowText(strText);		
		strText = (_TCHAR)dwCode;
		m_edtChar.SetWindowText(strText);		
		m_bFromClick = FALSE;

		GetDlgItem(IDC_EDT_GLYF_ID)->SetWindowText(_T("0000"));

		UpdateGlyfInfo(pWnd->m_nFBBY,pWnd->m_nFBBX,0,0, 0,0);
	}
	
	m_stcDisp.m_pGlyph = pWnd->m_pGlyph;
	m_stcDisp.m_nFBBY = pWnd->m_nFBBY* 3/2;
	m_stcDisp.m_nMaxBearingY = pWnd->m_nMaxBearingY;
	m_stcDisp.m_bAlpha = pWnd->m_bAlpha;

	if(pWnd->m_pGlyph != NULL)
	{
		m_stcDisp.m_nFBBX =  pWnd->m_pGlyph->dwidth;
		if(pWnd->m_pGlyph->dwidth == 0 && pWnd->m_pGlyph->width > 0)
		{
			m_stcDisp.m_nFBBX = pWnd->m_pGlyph->width;
		}
	}
	else
	{
		m_stcDisp.m_nFBBX = pWnd->m_nFBBX;
	}


	m_stcDisp.Invalidate();

	pWnd->m_bIsDown = TRUE;
	pWnd->Invalidate();

	pWnd->UpdateWindow();
	
	for(UINT i = 0; i<96;i++)
	{
		if( (IDC_STC_R0_C0+i) != nID)
		{
			CCoolStatic * pWndCS = (CCoolStatic *)GetDlgItem(IDC_STC_R0_C0 + i);
			if(pWndCS->m_bIsDown)
			{
				pWndCS->m_bIsDown = FALSE;
				pWndCS->Invalidate();
			}

		}
	}

}

void CFontToolDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ClientToScreen(&point);
	CRect rect;
	m_stcDisp.GetClientRect(&rect);
	m_stcDisp.ClientToScreen(&rect);
	UINT uUCCode = 0;
	CString strText;	
	GetDlgItem(IDC_EDT_CUR_UCCODE)->GetWindowText(strText);
	_stscanf(strText,_T("%04X"),&uUCCode);

	if(rect.PtInRect(point))
	{
		if(m_stcDisp.m_nFBBX ==0 || m_stcDisp.m_nFBBY == 0 )
			return;
		
		m_EditFontDlg.m_csEditFont.m_nFBBX = m_stcDisp.m_nFBBX;
		m_EditFontDlg.m_csEditFont.m_nFBBY = m_stcDisp.m_nFBBY;	
		m_EditFontDlg.m_csEditFont.m_bAlpha = m_stcDisp.m_bAlpha;
		if(m_stcDisp.m_pGlyph == NULL )
		{
			//m_stcDisp.m_pChar = m_bdf.AppendChar(uUCCode);
			//if(m_stcDisp.m_pChar == NULL)
				return;
		}
		
		if(!m_ftParser.IsReady())
		{
			return;
		}
		else if (m_ftParser.GetGlyfTblHdr(0)->type == FONT_TYPE_VECTOR_CMP || 
			     m_ftParser.GetGlyfTblHdr(0)->type == FONT_TYPE_VECTOR )
		{
			return;
		}

		m_EditFontDlg.m_csEditFont.m_pGlyph = m_stcDisp.m_pGlyph;
		if(m_EditFontDlg.DoModal()==IDCANCEL)
		{
			return;
			//m_bdf.AbstractGlyph(m_stcDisp.m_pChar);
		}
		m_stcDisp.Invalidate();
		
		for(int i = 0; i<96;i++)
		{
			CCoolStatic * pWnd = (CCoolStatic *)GetDlgItem(IDC_STC_R0_C0 + i);
			if(pWnd->m_bIsDown )
			{
				pWnd->m_pGlyph = m_stcDisp.m_pGlyph;
				pWnd->Invalidate();
				UpdateGlyfInfo(pWnd->m_nFBBY,pWnd->m_pGlyph->dwidth,
			           pWnd->m_pGlyph->height,pWnd->m_pGlyph->width,
					   pWnd->m_pGlyph->x,pWnd->m_pGlyph->y);
				break;
			}		
		}
	}
	
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CFontToolDlg::OnBtnLoadTtf() 
{
	// TODO: Add your control notification handler code here

/*	CGlyphData gd;
	CHAR_SET_INFO csi;
	csi.nFtTblNum = 2;
	csi.pFtTblSize = new int[csi.nFtTblNum ];
	csi.pFtTblSize[0] = 14;
	csi.pFtTblSize[1] = 16;
	csi.nSectorNum = 1;
	csi.pUCBoundTable = new UC_BOUND_T[csi.nSectorNum];
	csi.pUCBoundTable[0].dwUCEnd   = 0x7E;
	csi.pUCBoundTable[0].dwUCStart = 0x20;
	csi.tag[0]='l';
	csi.tag[1]='a';
	csi.tag[2]='t';
	csi.tag[3]='n';

	gd.Init(&csi);
	gd.LoadOtff(_T("E:\\ARIALUNI.TTF"));
	gd.Save(_T("E:\\fontdata.lib"));

	gd.Clear();

	return;*/

	if(m_ftParser.IsReady())
	{
		if(AfxMessageBox(_T("It will clear current glyphs first,\nif continue?"),MB_YESNO) == IDNO)
			return;
	}


	Clear();
	m_ftParser.Clear();
	m_cmbPtSize.EnableWindow(FALSE);
	m_cmbSortBy.EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SAVE_BDF)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SAVE_DOTMATRIX)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ADJUST_PARAM)->EnableWindow(FALSE);


	CDlgOTFSeting otfDlg;
	if(otfDlg.DoModal() == IDCANCEL)
	{
		return;
	}

	EnableWindow(FALSE);

	m_pCSI = otfDlg.m_pCSI;
	m_pCSI->dZoomRate = VECTOR_BASE_ZOOM;

	m_ftParser.SetCharSetInfo(m_pCSI);

	if(!m_ftParser.LoadOtff(otfDlg.m_strFontFile))
	{
		ChangeBtnState();
		AfxMessageBox(_T("Load failed!"));
		EnableWindow(TRUE);
		return;
	
	}

	m_pCSI = m_ftParser.GetCharSetInfo();

	Fresh();
	EnableWindow(TRUE);

	if(/*otfDlg.m_bGray */otfDlg.m_bVector)
	{
		GetDlgItem(IDC_BTN_SAVE_BDF)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ADJUST_PARAM)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_SAVE_BDF)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_ADJUST_PARAM)->EnableWindow(TRUE);
	}
	
	if(m_pCSI->bVector)
	{
		m_cmbPtSize.EnableWindow(FALSE);
	}
	
	ChangeBtnState();

	return;

}

void CFontToolDlg::OnProgressMsg(WPARAM wParam, LPARAM lParam)
{
	//TRACE(_T("Progress :%d\n"),lParam);
	switch(wParam)
	{
	case PROG_BEGIN:
		m_progress.ShowWindow(SW_SHOW);
		m_progress.SetRange32(0, lParam);
		m_progress.SetPos(0);
		break;
	case PROG_PROCEED:
		m_progress.SetPos(lParam);
		break;
	case PROG_END:
		if(lParam == 100)
		{
			int nLower = 0;
			int nUpper = 0;
			m_progress.GetRange(nLower,nUpper);
			m_progress.SetPos(nUpper);
		}
		m_progress.ShowWindow(SW_HIDE);
		break;
	default:
		_ASSERTE(0);
	}
}
BOOL CFontToolDlg::LoadDmf(LPCTSTR lpszFileName) 
{

	m_cmbPtSize.EnableWindow(FALSE);
	m_cmbSortBy.EnableWindow(FALSE);

	Clear();	
	
	m_ftParser.SetCharSetInfo(NULL);

	if(!m_ftParser.LoadFlib(lpszFileName))
	{
		AfxMessageBox(_T("Load failed!"));
		return FALSE;
	}

	m_pCSI = m_ftParser.GetCharSetInfo();

	Fresh();

	if(m_ftParser.IsVector())
	{
		GetDlgItem(IDC_BTN_SAVE_BDF)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ADJUST_PARAM)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_SAVE_BDF)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_ADJUST_PARAM)->EnableWindow(TRUE);
	}
 
	return TRUE;
	
}
BOOL CFontToolDlg::AddDmf(LPCTSTR lpszFileName) 
{

	m_cmbPtSize.EnableWindow(FALSE);
	m_cmbSortBy.EnableWindow(FALSE);

	Clear();	

	CGlyphData gd;	
	gd.Init(this->m_hWnd,TRUE);
	gd.SetCharSetInfo(NULL);

	if(!gd.LoadFlib(lpszFileName))
	{
		AfxMessageBox(_T("Load failed!"));
		gd.Clear();
		return FALSE;
	}

	if(!m_ftParser.DoMerge(gd))
	{
		gd.Clear();
		Fresh();
		return FALSE;
	}

	gd.Clear();

	m_pCSI = m_ftParser.GetCharSetInfo();

	Fresh();

	if(m_ftParser.IsVector())
	{
		GetDlgItem(IDC_BTN_SAVE_BDF)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ADJUST_PARAM)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_SAVE_BDF)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_ADJUST_PARAM)->EnableWindow(TRUE);
	}
 
	return TRUE;
	
}
void CFontToolDlg::OnBtnLoadDmf() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,_T(".lib"),NULL,OFN_HIDEREADONLY | OFN_FILEMUSTEXIST ,_T("DMF files (*.lib)|*.lib||"));
	if(dlg.DoModal() == IDCANCEL)
		return;
	CString strFileName = dlg.GetPathName();

	EnableWindow(FALSE);
	LoadDmf(strFileName);
	EnableWindow(TRUE);

	ChangeBtnState();
	
}

void CFontToolDlg::OnChangeEdtChar() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(!m_ftParser.IsReady()|| m_bFromClick || m_bFromInput)
		return;

//	m_edtChar.UpdateData(TRUE);
	CString strText;
	m_edtChar.GetWindowText(strText);
	if(strText.IsEmpty())
	{
		return;
	}

	UINT uUCCode = 0;
	uUCCode = strText.GetAt(0);

	GLYPH_BODY_PTR pGlyph = NULL;
	
	int nMaxPos = 0;
	int nMinPos = 0;
	int nPos = 0;

	pGlyph =m_ftParser.GetGlyph(uUCCode,m_nSortBySel);
    

	if(pGlyph != NULL)
	{	
		m_stcDisp.m_pGlyph  = pGlyph;
		m_stcDisp.m_uUnicode = uUCCode;
		m_stcDisp.m_nFBBY = m_ftParser.GetPointSize(m_nSortBySel)* 3/2;
		m_stcDisp.m_nFBBX = pGlyph->dwidth;
		if(m_stcDisp.m_nFBBX == 0 && pGlyph->width != 0)
		{
			m_stcDisp.m_nFBBX = pGlyph->width;
		}
		m_stcDisp.m_bAlpha = m_ftParser.GetGlyfTblHdr(m_nPtSizeIdx)->type != FONT_TYPE_MONO ? TRUE : FALSE;
		m_stcDisp.m_bNormalGlyf = TRUE;
		m_stcDisp.Invalidate();

		m_bFromFind = TRUE;
		strText.Format(_T("%04X"),uUCCode);
		GetDlgItem(IDC_EDT_CUR_UCCODE)->SetWindowText(strText);
		m_bFromFind = FALSE;

		UpdateGlyfInfo(m_stcDisp.m_nFBBY,pGlyph->dwidth,pGlyph->height,pGlyph->width,
			           pGlyph->x,pGlyph->y);

		UINT uCurStart = 0;
		UINT uCurEnd = 0;
		GetDlgItem(IDC_STC_LINE_ONE)->GetWindowText(strText);
		_stscanf(strText,_T("%04X"),&uCurStart);
		GetDlgItem(IDC_STC_LINE_SIX)->GetWindowText(strText);
		_stscanf(strText,_T("%04X"),&uCurEnd);
		uCurEnd += 15;

		DWORD dwUCEnd = m_pCSI->pUCBoundTable[m_pCSI->nSectorNum-1].dwUCEnd;

		if(uUCCode>uCurEnd || uUCCode < uCurStart)
		{
			m_bFromInput = TRUE;
			if(uUCCode < (dwUCEnd - dwUCEnd %16 - 16 * 5))
				FillGlyphGrid(uUCCode - (uUCCode % 16));
			else
				FillGlyphGrid((dwUCEnd - dwUCEnd %16 - 16 * 5));
 			m_bFromInput = FALSE;
			
			GetDlgItem(IDC_STC_LINE_ONE)->GetWindowText(strText);
			_stscanf(strText,_T("%04X"),&uCurStart);
			GetDlgItem(IDC_STC_LINE_SIX)->GetWindowText(strText);
			_stscanf(strText,_T("%04X"),&uCurEnd);
			uCurEnd += 15;
			
			nPos = (uUCCode - (uUCCode % 16)-m_dwUCBegin)/16;			
			nMaxPos = 0;
			nMinPos = 0;
			m_scb.GetScrollRange(&nMaxPos,&nMaxPos);
			if(nPos >  nMaxPos)
				nPos = nMaxPos;
			m_scb.SetScrollPos(nPos);
		}

		int nIdx = uUCCode - uCurStart;

		CCoolStatic * pWnd = (CCoolStatic *)GetDlgItem(IDC_STC_R0_C0 + nIdx);
		pWnd->m_bIsDown = TRUE;
		pWnd->Invalidate();

		for(int i = 0; i<96;i++)
		{
			if(i != nIdx)
			{
				CCoolStatic * pWndCS = (CCoolStatic *)GetDlgItem(IDC_STC_R0_C0 + i);
				pWndCS->m_bIsDown = FALSE;
				pWndCS->Invalidate();
			}
		}
	}


}

void CFontToolDlg::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	
	if(AfxMessageBox(_T("Do you want to exit!!"),MB_YESNO) == IDNO)
		return;
	
	m_ftParser.Clear(TRUE);

	this->DestroyWindow();

}

void CFontToolDlg::OnBtnSaveBdf() 
{
	// TODO: Add your control notification handler code here

	CFileDialog dlg(FALSE,_T(".BDF"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("BDF files (*.bdf)|*.bdf||"));
	if(dlg.DoModal() == IDCANCEL)
		return;
	CString strFileName = dlg.GetPathName();

	EnableWindow(FALSE);
	//m_bdf.SaveToBdf(strFileName);
	m_ftParser.SaveToBdf(strFileName);
	EnableWindow(TRUE);

	AfxMessageBox(_T("Complete!"));	
}
void CFontToolDlg::Clear()
{
	FillGlyphGrid(m_dwUCBegin,TRUE);	

	m_stcDisp.m_nFBBX =0;
	m_stcDisp.m_nFBBY = 0;
	m_stcDisp.m_pGlyph = NULL;
	m_cmbSortBy.SetCurSel(0);
	m_nSortBySel = 0;
	GetDlgItem(IDC_EDT_CUR_UCCODE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDT_CHAR)->EnableWindow(TRUE);

	Invalidate();
}

void CFontToolDlg::OnLoadSetting(WPARAM wparam,LPARAM lparam)
{
	UNUSED_ALWAYS(wparam);
	UNUSED_ALWAYS(lparam);

	EnableWindow(FALSE);
	if(!g_theApp.LoadSetting())
	{
		DestroyWindow();
		return;
	}
	EnableWindow(TRUE);
}

void CFontToolDlg::OnSelchangeCmbSortBy() 
{
	// TODO: Add your control notification handler code here
	m_nSortBySel = m_cmbSortBy.GetCurSel();

	if(m_bFromFontSetCmb || m_pCSI == NULL)
		return;

	EnableWindow(FALSE);

	int nMax = 0;

	if(m_nSortBySel == 0)
	{
// 		if(m_nPtSizeIdx < m_pCSI->nFtTblNum)
		{
			m_dwUCBegin = m_pCSI->pUCBoundTable[0].dwUCStart;
			nMax = (m_pCSI->pUCBoundTable[m_pCSI->nSectorNum-1].dwUCEnd-m_dwUCBegin)/16-5;
		}
// 		else
// 		{
// 			m_dwUCBegin = m_pCSI->pExUCBoundTable[0].dwUCStart;
// 			nMax = (m_pCSI->pExUCBoundTable[m_pCSI->nExSectorNum-1].dwUCEnd-m_dwUCBegin)/16-5;
// 		}

		if(m_dwUCBegin % 16 != 0)
		{
			m_dwUCBegin = (m_dwUCBegin / 16 ) *16;		
		}

		FillGlyphGrid(m_dwUCBegin);
	}
	else
	{
	
		//m_dwLCBegin = m_agGlyfID[0];
		//nMax = (m_agGlyfID[m_agGlyfID.GetSize() - 1]-m_dwLCBegin)/16-5;
		m_dwLCBegin = 0;
		nMax= m_agGlyfID.GetSize()/16-5;

		if(m_dwLCBegin % 16 != 0)
		{
			m_dwUCBegin = (m_dwLCBegin / 16 ) *16;		
		}

		FillGlyphGrid(m_dwLCBegin);
	}


	if(nMax < 0)
	{
		nMax = 0;
	}
	
	m_scb.SetScrollRange(0,nMax);
	m_scb.SetScrollPos(0);

	if(m_nSortBySel == 1 )
	{
		GetDlgItem(IDC_EDT_CUR_UCCODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDT_CHAR)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDT_CUR_UCCODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDT_CHAR)->EnableWindow(TRUE);
	}


	EnableWindow(TRUE);
}

void CFontToolDlg::Fresh()
{
	if(m_pCSI == NULL)
		return;

	int i;
	for (i=0;i < m_cmbPtSize.GetCount();i++)
	{
	  m_cmbPtSize.DeleteString( i );
	  i--;
	}
	
	for(i = 0; i< m_ftParser.GetGlyfTblCount(); i++)
	{
		CString strTmp;
		strTmp.Format(_T("%d"),m_ftParser.GetGlyfTblHdr(i)->size);
		m_cmbPtSize.AddString(strTmp);
	}
	
	m_cmbPtSize.EnableWindow(TRUE);
	m_cmbPtSize.SetCurSel(0);
	m_cmbSortBy.EnableWindow(TRUE);
	m_cmbSortBy.SetCurSel(0);

	m_nSortBySel = 0;
	m_nPtSizeIdx = 0;


	m_dwUCBegin = m_pCSI->pUCBoundTable[0].dwUCStart;

	if(m_dwUCBegin % 16 != 0)
	{
		m_dwUCBegin = (m_dwUCBegin / 16 ) *16;		
	}	


	m_ftParser.GetIDList(m_agGlyfID,0);

	CString strText;
	 
	m_stcDisp.m_nFBBY = m_ftParser.GetPointSize(0)* 3/2;
    m_stcDisp.m_pGlyph = m_ftParser.GetGlyph(m_dwUCBegin,0);
	m_stcDisp.m_nMaxBearingY = m_ftParser.GetMaxBearingY(0);
	m_bFromInput = TRUE;
	if(m_stcDisp.m_pGlyph != NULL)
	{
		m_stcDisp.m_nFBBX = m_stcDisp.m_pGlyph->dwidth;
		if(m_stcDisp.m_nFBBX == 0 && m_stcDisp.m_pGlyph->width != 0)
		{
			m_stcDisp.m_nFBBX  = m_stcDisp.m_pGlyph->width;
		}
		UpdateGlyfInfo(m_stcDisp.m_nFBBY,m_stcDisp.m_pGlyph->dwidth, 
			           m_stcDisp.m_pGlyph->height,m_stcDisp.m_pGlyph->width,
					   m_stcDisp.m_pGlyph->x,m_stcDisp.m_pGlyph->y);

		strText.Format(_T("%04X"),m_dwUCBegin);
		GetDlgItem(IDC_EDT_CUR_UCCODE)->SetWindowText(strText);	

		strText.Format(_T("%04X"),m_ftParser.GetIDFromUni(m_dwUCBegin));
		GetDlgItem(IDC_EDT_GLYF_ID)->SetWindowText(strText);	
	}
	else
	{
		m_stcDisp.m_nFBBX = m_ftParser.GetPointSize(0);
		UpdateGlyfInfo(m_stcDisp.m_nFBBY,m_stcDisp.m_nFBBX, 
						0,0,0,0);

		strText.Format(_T("%04X"),m_dwUCBegin);
		GetDlgItem(IDC_EDT_CUR_UCCODE)->SetWindowText(_T(""));	

		GetDlgItem(IDC_EDT_GLYF_ID)->SetWindowText(_T(""));	
		
	}
	m_bFromInput = FALSE;
	m_stcDisp.m_uUnicode = m_dwUCBegin;
	m_stcDisp.Invalidate(TRUE);

    

	GetDlgItem(IDC_EDT_FONT_NAME)->SetWindowText(m_pCSI->szCharSetName);
	TCHAR szTag[5] = {0};
	szTag[0]= m_pCSI->tag[0];
	szTag[1]= m_pCSI->tag[1];
	szTag[2]= m_pCSI->tag[2];
	szTag[3]= m_pCSI->tag[3];
	GetDlgItem(IDC_EDT_FONT_TAG)->SetWindowText(szTag);

	strText.Format(_T("%d"),m_ftParser.GetMaxBearingY(m_nPtSizeIdx));
	GetDlgItem(IDC_EDT_MAX_BEARING_Y)->SetWindowText(strText);

	strText.Format(_T("%d"),m_ftParser.GetMaxHeight(m_nPtSizeIdx));
	GetDlgItem(IDC_EDT_MAX_HEIGHT)->SetWindowText(strText);

    FillGlyphGrid(m_dwUCBegin);

	int  nMax = (m_pCSI->pUCBoundTable[m_pCSI->nSectorNum-1].dwUCEnd-m_dwUCBegin )/16-5;
	if(nMax < 0)
		nMax = 0;
	m_scb.SetScrollRange(0,nMax);
	m_scb.SetScrollPos(0);
	
	ChangeBtnState();
}



void CFontToolDlg::UpdateGlyfInfo(UINT height,UINT width,UINT bitmap_h,UINT bitmap_w,int x,int y)
{
	CString strText;
	strText.Format(_T("%d"),width);
	GetDlgItem(IDC_EDT_FONT_W)->SetWindowText(strText);

	strText.Format(_T("%d"),height);
	GetDlgItem(IDC_EDT_FONT_H)->SetWindowText(strText);

	strText.Format(_T("%d"),bitmap_w);
	GetDlgItem(IDC_EDT_BMP_W)->SetWindowText(strText);

	strText.Format(_T("%d"),bitmap_h);
	GetDlgItem(IDC_EDT_BMP_H)->SetWindowText(strText);

	strText.Format(_T("%d"),x);
	GetDlgItem(IDC_EDT_XOFF)->SetWindowText(strText);

	strText.Format(_T("%d"),y);
	GetDlgItem(IDC_EDT_YOFF)->SetWindowText(strText);
}



void CFontToolDlg::OnSelchangeCmbFontSize() 
{
	// TODO: Add your control notification handler code here
	m_nPtSizeIdx = m_cmbPtSize.GetCurSel();

	if(m_bFromFontSetCmb || m_pCSI == NULL)
		return;

	EnableWindow(FALSE);

	int nMax = 0;

	m_ftParser.GetIDList(m_agGlyfID,m_nPtSizeIdx);

	if(m_nSortBySel == 0)
	{
		if(m_nPtSizeIdx < m_pCSI->nFtTblNum)
		{
			m_dwUCBegin = m_pCSI->pUCBoundTable[0].dwUCStart;
			nMax = (m_pCSI->pUCBoundTable[m_pCSI->nSectorNum-1].dwUCEnd-m_dwUCBegin)/16-5;
		}
		else
		{
			m_dwUCBegin = m_pCSI->pExUCBoundTable[0].dwUCStart;
			nMax = (m_pCSI->pExUCBoundTable[m_pCSI->nExSectorNum-1].dwUCEnd-m_dwUCBegin)/16-5;
		}

		if(m_dwUCBegin % 16 != 0)
		{
			m_dwUCBegin = (m_dwUCBegin / 16 ) *16;		
		}

		FillGlyphGrid(m_dwUCBegin);
	}
	else
	{		
		//m_dwLCBegin = m_agGlyfID[0];
		//nMax = (m_agGlyfID[m_agGlyfID.GetSize() - 1]-m_dwLCBegin)/16-5;
		m_dwLCBegin = 0;
		nMax= m_agGlyfID.GetSize()/16-5;

		if(m_dwLCBegin % 16 != 0)
		{
			m_dwUCBegin = (m_dwLCBegin / 16 ) *16;		
		}

		FillGlyphGrid(m_dwLCBegin);
	}


	if(nMax < 0)
	{
		nMax = 0;
	}
	
	m_scb.SetScrollRange(0,nMax);
	m_scb.SetScrollPos(0);


	CString strText;

	strText.Format(_T("%d"),m_ftParser.GetMaxBearingY(m_nPtSizeIdx));
	GetDlgItem(IDC_EDT_MAX_BEARING_Y)->SetWindowText(strText);

	strText.Format(_T("%d"),m_ftParser.GetMaxHeight(m_nPtSizeIdx));
	GetDlgItem(IDC_EDT_MAX_HEIGHT)->SetWindowText(strText);


	EnableWindow(TRUE);
}

void CFontToolDlg::OnBtnAdjustParam() 
{
	// TODO: Add your control notification handler code here
	CDlgAdjustParam dlg;

	if(m_stcDisp.m_pGlyph != NULL)
	{
		dlg.m_nFBBX = (int)m_stcDisp.m_pGlyph->dwidth;
	}
	if(dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	if(m_stcDisp.m_pGlyph != NULL)
	{
		m_stcDisp.m_pGlyph->dwidth = (WORD)dlg.m_nFBBX;		
		m_stcDisp.m_nFBBX =  (WORD)dlg.m_nFBBX;	
	}

	if(dlg.m_nBXYRange == 0)
	{	
		if(m_stcDisp.m_pGlyph != NULL)
		{
			m_stcDisp.m_pGlyph->x = (short)(m_stcDisp.m_pGlyph->x + dlg.m_nBXDelta);
			m_stcDisp.m_pGlyph->y = (short)(m_stcDisp.m_pGlyph->y + dlg.m_nBYDelta);
			m_stcDisp.Invalidate();

			UINT uIdxStart = IDC_STC_R0_C0;
			for(int i = 0; i< 6; i++)
			{
				for( int j = 0; j< 16; j++)
				{
					CCoolStatic * pWnd = (CCoolStatic *)GetDlgItem(uIdxStart+i*16 +j);
					
					if(pWnd->m_bIsDown && pWnd->m_pGlyph == m_stcDisp.m_pGlyph)
					{
						pWnd->m_nFBBX = m_stcDisp.m_nFBBX;	
						pWnd->Invalidate();
						break;
					}		
				}
			}
		}
	}
	else 
	{
		int nPtIndex = -1;
		if(dlg.m_nBXYRange == 1)
		{
			nPtIndex = m_nPtSizeIdx;
		}
		m_ftParser.AdjustBearingXY(dlg.m_nBXDelta,dlg.m_nBYDelta,nPtIndex);
		Fresh();
	}	

	if(m_stcDisp.m_pGlyph != NULL)
	{
		UpdateGlyfInfo(m_stcDisp.m_nFBBY,m_stcDisp.m_pGlyph->dwidth,m_stcDisp.m_pGlyph->height,
					 m_stcDisp.m_pGlyph->width,m_stcDisp.m_pGlyph->x,m_stcDisp.m_pGlyph->y);
	}
	


}

void CFontToolDlg::OnBtnMinus() 
{
	// TODO: Add your control notification handler code here
	if(AfxMessageBox(_T("Do you want to remove the current font size from font lib?"),MB_YESNO) == IDNO)
	{
		return;
	}

	m_cmbPtSize.EnableWindow(FALSE);
	m_cmbSortBy.EnableWindow(FALSE);


	EnableWindow(FALSE);
	m_ftParser.RemoveFontSize(m_nPtSizeIdx);
	m_pCSI = m_ftParser.GetCharSetInfo();
	Fresh();
	EnableWindow(TRUE);

	ChangeBtnState();
}

void CFontToolDlg::OnBtnPlus() 
{
	// TODO: Add your control notification handler code here
	if(AfxMessageBox(_T("Do you want to merge another font lib?"),MB_YESNO) == IDNO)
	{
		return;
	}
	CFileDialog dlg(TRUE,_T(".lib"),NULL,OFN_HIDEREADONLY | OFN_FILEMUSTEXIST ,_T("DMF files (*.lib)|*.lib||"));
	if(dlg.DoModal() == IDCANCEL)
		return;
	CString strFileName = dlg.GetPathName();


	if(m_ftParser.IsReady() && m_ftParser.IsLoadLib())
	{
		EnableWindow(FALSE);
		m_ftParser.ReplaceBDF(strFileName,m_nPtSizeIdx);
		EnableWindow(TRUE);
	}
}

void CFontToolDlg::OnReplBdf() 
{
	// TODO: Add your control notification handler code here
	if(AfxMessageBox(_T("Do you want to replace glyphs of current font size with BDF file?"),MB_YESNO) == IDNO)
	{
		return;
	}
	CFileDialog dlg(TRUE,_T(".bdf"),NULL,OFN_HIDEREADONLY | OFN_FILEMUSTEXIST ,_T("BDF files (*.BDF)|*.BDF||"));
	if(dlg.DoModal() == IDCANCEL)
		return;
	CString strFileName = dlg.GetPathName();

	int nIdx = m_nPtSizeIdx;
	
	if(m_ftParser.IsReady() && m_ftParser.IsLoadLib() && !m_ftParser.IsVector() )
	{
		EnableWindow(FALSE);
		BOOL bOK = m_ftParser.ReplaceBDF(strFileName,m_nPtSizeIdx);	
		if(bOK)
		{
			Fresh();
			m_cmbPtSize.SetCurSel(nIdx);
			OnSelchangeCmbFontSize();
			AfxMessageBox(_T("Replace success!"));
		}
		else
		{
			AfxMessageBox(_T("Replace failed!"));
		}
		EnableWindow(TRUE);
	}
}

void CFontToolDlg::ChangeBtnState()
{
	if(!m_ftParser.IsReady())
	{
		GetDlgItem(IDC_BTN_SAVE_DOTMATRIX)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SAVE_BDF)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ADJUST_PARAM)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_MINUS)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PLUS)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_REPL_BDF)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_SAVE_DOTMATRIX)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_SAVE_BDF)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_ADJUST_PARAM)->EnableWindow(TRUE);
		
		if(m_ftParser.IsVector() || m_ftParser.GetGlyfTblCount() == 1)
		{
			GetDlgItem(IDC_BTN_MINUS)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_BTN_MINUS)->EnableWindow(TRUE);
		}
		
		if( m_ftParser.IsLoadLib() && !m_ftParser.IsVector() )
		{
			GetDlgItem(IDC_BTN_PLUS)->EnableWindow(TRUE);		
		}
		else
		{
			GetDlgItem(IDC_BTN_PLUS)->EnableWindow(FALSE);			
		}

		if(!m_ftParser.IsVector() && m_ftParser.IsLoadLib() )
		{
			GetDlgItem(IDC_BTN_REPL_BDF)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BTN_REPL_BDF)->EnableWindow(FALSE);
		}
	}
}
