// Dlg3D.cpp : implementation file
//

#include "stdafx.h"
#include "bmptoarray.h"
#include "Dlg3D.h"
#include "se_table_gen.h"
#include "FileDlg.h"
#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg3D dialog


CDlg3D::CDlg3D(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg3D::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg3D)
		// NOTE: the ClassWizard will add member initialization here
	m_uWSlide = 80;
	m_uWDesktop = 240;
	m_uHSlide = 80;
	m_uHDesktop = 320;
	m_strOutPut = "";
	//}}AFX_DATA_INIT

	memset(m_szTmpPath,0,sizeof(m_szTmpPath));
}


void CDlg3D::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg3D)
	DDX_Text(pDX, IDC_EDT_W_SLIDE, m_uWSlide);
	DDV_MinMaxInt(pDX, m_uWSlide, 1, 240);

	DDX_Text(pDX, IDC_EDT_W_ROT, m_uWDesktop);
	DDV_MinMaxInt(pDX, m_uWDesktop, 1, 640);

	DDX_Text(pDX, IDC_EDT_H_SLIDE, m_uHSlide);
	DDV_MinMaxInt(pDX, m_uHSlide, 1, 320);

	DDX_Text(pDX, IDC_EDT_H_ROT, m_uHDesktop);
	DDV_MinMaxInt(pDX, m_uHDesktop, 1, 480);

	DDX_Text(pDX, IDC_EDT_OUTPUT, m_strOutPut);
	DDX_Control(pDX, IDC_EDT_OUTPUT, m_edtOutPut);

	DDX_Text(pDX, IDC_EDT_IMPORT_FILE, m_strImportPath);
	DDX_Control(pDX, IDC_BTN_BROWSE, m_btnBrowse);
	DDX_Control(pDX, IDC_BTN_IMPORT, m_btnImport);

		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg3D, CDialog)
	//{{AFX_MSG_MAP(CDlg3D)
	ON_BN_CLICKED(IDC_CALC, OnCalc)
	ON_BN_CLICKED(IDC_BTN_BROWSE, OnBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_IMPORT, OnBtnImport)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg3D message handlers

BOOL CDlg3D::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg3D::OnCalc() 
{
	// TODO: Add your control notification handler code here
	USES_CONVERSION;
	UpdateData();
	
	if(m_uWSlide <= 0 || m_uWSlide > 240 || m_uWDesktop <= 0 || m_uWDesktop > 640 ||
		m_uHSlide <=0 || m_uHSlide > 320 || m_uHDesktop <= 0 || m_uHDesktop > 480)
	{
		return;
	}
	
	memset(m_szTmpPath,0,sizeof(m_szTmpPath));
	_stprintf(m_szTmpPath,_T("%s\\gen_3dtable_tmp%d"),_tgetenv(_T("tmp")),GetTickCount());

	if(!CreateDirectory(m_szTmpPath,NULL))
	{
		if( GetLastError() != ERROR_ALREADY_EXISTS )
        {     
			_tprintf(_T("\nCan not create temp folder [%s], [error:0x%X].\n"),m_szTmpPath,GetLastError());
			CString strErr;
			strErr.Format(_T("\nCan not create temp folder [%s], [error:0x%X].\n"),m_szTmpPath,GetLastError());
			AfxMessageBox(strErr.operator LPCTSTR());
            return;
        }
	}
#if defined(UNICODE) || defined(_UNICODE)
	SetTempPath(W2A(m_szTmpPath));
#else
	SetTempPath(m_szTmpPath);
#endif	

	SetTimer(EVENT_PROCESS, 1000, 0);

	CreateSlideMoveTable(m_uWSlide, m_uHSlide);
	CreateDistortMoveTable(m_uWSlide, m_uHSlide);
	CreateRotDesktopTable(m_uWDesktop, m_uHDesktop);
	CreateMicros(m_uWSlide, m_uHSlide, m_uWDesktop, m_uHDesktop);

	_TCHAR szPath[_MAX_PATH] = {0};

	_stprintf(szPath,_T("%s\\%s"),m_szTmpPath,SE_MICORS);	

	FILE *fp = _tfopen(szPath, _T("r"));
	char szLine[4096] = {0};
	CString strTmp = _T("");
	if(fp != NULL)
	{
		while(!feof(fp))
		{
			fgets(szLine, 4096, fp);
#ifdef UNICODE
			strTmp = A2W(szLine);
#else
			strTmp = szLine;
#endif
			strTmp.Replace(_T("\n"), _T("\r\n"));
			m_strOutPut += strTmp;
		}

		fclose(fp);
		fp = NULL;
	}

	//
	_stprintf(szPath,_T("%s\\%s"),m_szTmpPath,SE_SLIDE_MOVE_FILE);		
	fp = _tfopen(szPath, _T("r"));

	if(fp != NULL)
	{
		while(!feof(fp))
		{
			fgets(szLine, 4096, fp);
#ifdef UNICODE
			strTmp = A2W(szLine);
#else
			strTmp = szLine;
#endif
			strTmp.Replace(_T("\n"), _T("\r\n"));
			m_strOutPut += strTmp;
		}

		fclose(fp);
		fp = NULL;
	}


	//
	_stprintf(szPath,_T("%s\\%s"),m_szTmpPath,SE_DISTORT_MOVE_FILE);
	fp = _tfopen(szPath, _T("r"));

	if(fp != NULL)
	{
		while(!feof(fp))
		{
			fgets(szLine, 4096, fp);
#ifdef UNICODE
			strTmp = A2W(szLine);
#else
			strTmp = szLine;
#endif
			strTmp.Replace(_T("\n"), _T("\r\n"));
			m_strOutPut += strTmp;
		}

		fclose(fp);
		fp = NULL;
	}

	//
	_stprintf(szPath,_T("%s\\%s"),m_szTmpPath,SE_DESKTOP_ROTATE_FILE);	
	fp = _tfopen(szPath, _T("r"));

	if(fp != NULL)
	{
		while(!feof(fp))
		{
			fgets(szLine, 4096, fp);
#ifdef UNICODE
			strTmp = A2W(szLine);
#else
			strTmp = szLine;
#endif
			strTmp.Replace(_T("\n"), _T("\r\n"));
			m_strOutPut += strTmp;
		}

		fclose(fp);
		fp = NULL;
	}

	KillTimer(EVENT_PROCESS);
	m_edtOutPut.SetWindowText(_T(""));

	UpdateData(FALSE);

	AfxMessageBox(_T("OK"));
	
}

void CDlg3D::OnBtnBrowse() 
{
	// TODO: Add your control notification handler code here
	CFileDlg dlg;
	m_strImportPath =  dlg.GetSelectDir();
	if(!m_strImportPath.IsEmpty())
	{
		m_strImportPath += "\\";
	}
	
	UpdateData(FALSE);
}

void CDlg3D::OnBtnImport() 
{
	// TODO: Add your control notification handler code here
	USES_CONVERSION;
	if(m_strImportPath.IsEmpty())
	{
		AfxMessageBox(_T("Please input import file path first!\n"));
		return;
	}

	if(m_strOutPut.IsEmpty())
	{
		AfxMessageBox(_T("Please calculate table first!\n"));
		return;
	}
	
	CString strOutput = m_strImportPath + OUT_PUT_FILE;

	_TCHAR szDirectory[MAX_PATH];		
	GetModuleFileName(NULL, szDirectory, MAX_PATH);
	CString strCfile= _T("");
	strCfile += szDirectory;
	strCfile = strCfile.Left(strCfile.ReverseFind('\\') + 1);	
	strCfile += OUT_PUT_FILE;

	FILE *fpCfile = NULL;
	fpCfile = _tfopen(strCfile, _T("r"));
	if(fpCfile == NULL)
	{
		_TCHAR szPrompt[128] = {0};
		_stprintf(szPrompt, _T("Open %s failed!"), strCfile);
		AfxMessageBox(szPrompt);
		g_theApp.XDelete3DFiles(m_szTmpPath);
		return;
	}

	BOOL bSame = FALSE;
	if(strOutput == strCfile)
	{
		bSame = TRUE;
		strOutput += _T(".bak");
		//remove readonly
		DWORD dwAttr = ::GetFileAttributes(strCfile);
		dwAttr &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(strCfile, dwAttr);
	}

	FILE *fpOutput = NULL;
	fpOutput = _tfopen(strOutput, _T("w"));
	if(fpOutput == NULL)
	{
		_fcloseall();
		_TCHAR szPrompt[128] = {0};
		_stprintf(szPrompt, _T("Open %s failed!"), strOutput);
		AfxMessageBox(szPrompt);
		_fcloseall();
		g_theApp.XDelete3DFiles(m_szTmpPath);
		return;
	}

	_TCHAR szPath[_MAX_PATH] = {0};

	_stprintf(szPath,_T("%s\\%s"),m_szTmpPath,SE_MICORS);	
	FILE *fp_Icon = NULL;
	fp_Icon = _tfopen(szPath, _T("r"));
	if(fp_Icon == NULL)
	{
		_fcloseall();
		_TCHAR szPrompt[128] = {0};
		_stprintf(szPrompt, _T("Open %s failed!"), SE_MICORS);
		AfxMessageBox(szPrompt);	
		g_theApp.XDelete3DFiles(m_szTmpPath);
		return;
	}

	_stprintf(szPath,_T("%s\\%s"),m_szTmpPath,SE_SLIDE_MOVE_FILE);	
	FILE *fp_Slide = NULL;
	fp_Slide = _tfopen(szPath, _T("r"));
	if(fp_Slide == NULL)
	{
		_fcloseall();
		_TCHAR szPrompt[128] = {0};
		_stprintf(szPrompt, _T("Open %s failed!"), SE_SLIDE_MOVE_FILE);
		AfxMessageBox(szPrompt);		
		g_theApp.XDelete3DFiles(m_szTmpPath);
		return;
	}

	_stprintf(szPath,_T("%s\\%s"),m_szTmpPath,SE_DISTORT_MOVE_FILE);	
	FILE *fp_Distork = NULL;
	fp_Distork = _tfopen(szPath, _T("r"));
	if(fp_Distork == NULL)
	{
		_fcloseall();
		_TCHAR szPrompt[128] = {0};
		_stprintf(szPrompt, _T("Open %s failed!"), SE_DISTORT_MOVE_FILE);
		AfxMessageBox(szPrompt);
		g_theApp.XDelete3DFiles(m_szTmpPath);
		return;
	}

    _stprintf(szPath,_T("%s\\%s"),m_szTmpPath,SE_DESKTOP_ROTATE_FILE);
	FILE *fp_Desktop = NULL;
	fp_Desktop = _tfopen(szPath, _T("r"));
	if(fp_Desktop == NULL)
	{
		_fcloseall();
		_TCHAR szPrompt[128] = {0};
		_stprintf(szPrompt, _T("Open %s failed!"), SE_DESKTOP_ROTATE_FILE);
		AfxMessageBox(szPrompt);
		g_theApp.XDelete3DFiles(m_szTmpPath);
		return;
	}


	char szLine[4096] = {0};
	char szRead[4096] = {0};

	while(!feof(fpCfile))
	{
		fgets(szLine, 4096, fpCfile);
		fwrite(szLine, 1, strlen(szLine), fpOutput);
		fflush(fpOutput);
		if(strstr(szLine, ICON_FLAG) != NULL)
		{
			while(!feof(fp_Icon))
			{
				if(fgets(szRead, 4096, fp_Icon) != NULL)
				{
					fwrite(szRead, 1, strlen(szRead), fpOutput);
					memset(szRead, 0, 4096);
					fflush(fpOutput);
				}
			}
			fwrite("\n", 1, 1, fpOutput);
			fflush(fpOutput);
			continue;
		}
		else if(strstr(szLine, SLIDE_FLAG) != NULL)
		{
			while (!feof(fp_Slide))
			{
				if(fgets(szRead, 4096, fp_Slide) != NULL)
				{
					fwrite(szRead, 1, strlen(szRead), fpOutput);
					memset(szRead, 0, 4096);
					fflush(fpOutput);
				}
			}
			fwrite("\n", 1, 1, fpOutput);
			fflush(fpOutput);
			continue;
		}
		else if(strstr(szLine, DISTORT_FLAG) != NULL)
		{
			while (!feof(fp_Distork))
			{
				if(fgets(szRead, 4096, fp_Distork) != NULL)
				{
					fwrite(szRead, 1, strlen(szRead), fpOutput);
					memset(szRead, 0, 4096);
					fflush(fpOutput);
				}
			}
			fwrite("\n", 1, 1, fpOutput);
			fflush(fpOutput);
			continue;
			
		}
		else if(strstr(szLine, DESKTOP_FLAG) != NULL)
		{
			while (!feof(fp_Desktop))
			{
				if(fgets(szRead, 4096, fp_Desktop) != NULL)
				{
					fwrite(szRead, 1, strlen(szRead), fpOutput);
					memset(szRead, 0, 4096);
					fflush(fpOutput);
				}
			}
			fwrite("\n", 1, 1, fpOutput);
			fflush(fpOutput);
			continue;
		}

		memset(szLine, 0, 4096);
	}

	_fcloseall();
	fpCfile = NULL;
	fpOutput = NULL;
	fp_Icon = NULL;
	fp_Slide = NULL;
	fp_Distork = NULL;
	fp_Desktop = NULL;

	if(bSame)
	{
		CopyFile(strOutput, strCfile, FALSE);
		DeleteFile(strOutput);
	}

	AfxMessageBox(_T("OK"));
	g_theApp.XDelete3DFiles(m_szTmpPath);
	
}

void CDlg3D::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	static int nCount = 0;

	if(nIDEvent == EVENT_PROCESS)
	{
		if(nCount % 3 == 0)
		{
			m_edtOutPut.SetWindowText(_T("Calc.."));
		}
		else if(nCount % 3 == 1)
		{
			m_edtOutPut.SetWindowText(_T("Calc..."));
		}
		else
		{
			m_edtOutPut.SetWindowText(_T("Calc......"));
		}

		nCount ++;

	}
	
	CDialog::OnTimer(nIDEvent);
}

BOOL CDlg3D::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (WM_KEYDOWN == pMsg->message)
    {
        // Process when receive ENTER
        if (VK_RETURN == pMsg->wParam || pMsg->wParam == VK_ESCAPE)
        {
            return TRUE;
        }
    }
	
	return CDialog::PreTranslateMessage(pMsg);
}
