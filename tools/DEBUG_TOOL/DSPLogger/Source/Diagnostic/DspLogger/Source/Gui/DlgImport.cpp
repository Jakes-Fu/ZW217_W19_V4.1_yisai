// DlgImport.cpp : implementation file
//

#include "stdafx.h"
#include "dsplogger.h"
#include "DlgImport.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgImport dialog


CDlgImport::CDlgImport(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgImport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgImport)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nConvertCheck = 1;
}


void CDlgImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgImport)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Text(pDX, IDC_EDIT_IMPORT_PATH, m_strPath);
	DDX_Control(pDX, IDC_CHECK_CONVERT_TIME, m_btnConvert);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgImport, CDialog)
	//{{AFX_MSG_MAP(CDlgImport)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_BROWSE, OnButtonImportBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgImport message handlers

void CDlgImport::OnButtonImportBrowse() 
{
	// TODO: Add your control notification handler code here
	char filter[] = "Log Files (*.tp)|*.tp|All Files (*.*)|*.*||";
	CFileDialog dlgFile(TRUE,NULL,NULL,OFN_FILEMUSTEXIST,filter);
    if(IDOK == dlgFile.DoModal())
    {		
		m_strPath = dlgFile.GetPathName();
		UpdateData(FALSE);	
	}
}

void CDlgImport::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	CMainFrame *pFrm = (CMainFrame *) AfxGetMainWnd();
	if(m_btnConvert.GetCheck() == BST_CHECKED)
	{
		m_nConvertCheck = 1;
		//char szToolPath[ MAX_PATH ] = {0};	
		char szExePath[MAX_PATH*2] = {0};
		GetModuleFileName(NULL, szExePath, MAX_PATH*2);

		CString strOutputPath = m_strPath;
		int nIndex = strOutputPath.ReverseFind('.');
		if(nIndex != -1)
		{
			strOutputPath = strOutputPath.Left(nIndex);
			strOutputPath += _T("_d0a0.tp");
		}

		/*

		char szPara[1024] = {0};
		sprintf(szPara, "0xd0a0 %s %s", m_strPath, strOutputPath);

		SHELLEXECUTEINFO shellInfo;
		memset(&shellInfo, 0, sizeof(SHELLEXECUTEINFO));
		shellInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		shellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;	
		shellInfo.lpFile = szToolPath;
		shellInfo.nShow = SW_HIDE;
		shellInfo.lpParameters = szPara;
		ShellExecuteEx(&shellInfo);	
		Sleep(1000);


		CFileFind fFind;
		BOOL bRet = fFind.FindFile(strOutputPath);
		int nSize = 0;
		if(bRet)
		{
			fFind.FindNextFile();
			nSize = fFind.GetLength();
			if(nSize == 0)
			{
				AfxMessageBox("No D0A0 data in file, Converting failed!");
				return;
			}
		}
		else
		{
			AfxMessageBox("Can not convert tp file by VP_CONVERT.exe!");
			return;
		}
		*/

		BOOL bRet = ConvertTP((LPSTR)(LPCTSTR)m_strPath, (LPSTR)(LPCTSTR)strOutputPath);
		if(!bRet)
		{
			AfxMessageBox("No D0A0 data in file, Converting failed!");
			::DeleteFile(strOutputPath);
			return;
		}
		
		m_strPath = strOutputPath;
		m_strConvertFile = strOutputPath;
	}
	else
	{
		m_nConvertCheck = 0;
	}

	pFrm->LoadLogFile(m_strPath);

	if(pFrm->OpenMarkFile(READ_MODE))
	{
		pFrm->LoadMarkFile();
		pFrm->CloseMarkFile();
	}
	
	CDialog::OnOK();
}

BOOL CDlgImport::ConvertTP(LPSTR lpInputFile, LPSTR lpOutputFile)
{
	FILE *fp_input = fopen(lpInputFile, "rb");
	if(fp_input == NULL)
	{
		AfxMessageBox("Open source tp file failed!");
		return FALSE;
	}

	FILE *fp_output = fopen(lpOutputFile, "wb");
	if(fp_output == NULL)
	{
		AfxMessageBox("Open dest tp file failed!");
		return FALSE;
	}

	WORD wBuf = 0;
	DWORD dwTime = 0;
	DWORD dwTemp = 0;

	BOOL bFindTime = FALSE;
	BOOL bExist = FALSE;
	BOOL bStartFlag = TRUE;

	//find first time
	while(!feof(fp_input))
	{
		int nSize = fread(&wBuf, sizeof(WORD), 1, fp_input);
		if(nSize == 0)
		{
			break;
		}
		
		if(wBuf == 0xa0d0 && !bFindTime)
		{
			bFindTime = TRUE;
			fread(&wBuf, sizeof(WORD), 1, fp_input);
			CONVERT_SHORT(wBuf, dwTemp);
			dwTemp = dwTemp << 16;
			for(int i = 0; i < 2; i++)
			{
				fread(&wBuf, sizeof(WORD), 1, fp_input);
			}
		}
		else if(bFindTime)
		{
			if(wBuf == 0xa0d0)
			{
				fread(&wBuf, sizeof(WORD), 1, fp_input);
				CONVERT_SHORT(wBuf, wBuf);
				dwTime = dwTemp | wBuf;
				bFindTime = FALSE;
				bExist = TRUE;
				break;
			}
			else
			{
				bFindTime = FALSE;
				fread(&wBuf, sizeof(WORD), 1, fp_input);
				for(int i = 0; i < 2; i++)
				{
					fread(&wBuf, sizeof(WORD), 1, fp_input);
				}
			}
		}
		else
		{
			fread(&wBuf, sizeof(WORD), 1, fp_input);
			for(int i = 0; i < 2; i++)
			{
				fread(&wBuf, sizeof(WORD), 1, fp_input);
			}
		}
	}

	if(!bExist)
	{
		fclose(fp_input);
		fclose(fp_output);
		return FALSE;
	}

	bFindTime = FALSE;
	fseek(fp_input, 0, SEEK_SET);

	while(!feof(fp_input))
	{
		int nSize = fread(&wBuf, sizeof(WORD), 1, fp_input);
		if(nSize == 0)
		{
			break;
		}

		if(wBuf == 0xa0d0 && !bFindTime)
		{
			bFindTime = TRUE;
			fwrite(&wBuf, sizeof(WORD), 1, fp_output);
			fread(&wBuf, sizeof(WORD), 1, fp_input);
			fwrite(&wBuf, sizeof(WORD), 1, fp_output);
			CONVERT_SHORT(wBuf, dwTemp);
			 dwTemp = dwTemp << 16;
			for(int i = 0; i < 2; i++)
			{
				fread(&wBuf, sizeof(WORD), 1, fp_input);
			}
			fwrite(&dwTime, sizeof(DWORD), 1, fp_output);
		}
		else if(bFindTime)
		{
			if(wBuf == 0xa0d0)
			{
				fwrite(&wBuf, sizeof(WORD), 1, fp_output);
				fread(&wBuf, sizeof(WORD), 1, fp_input);
				fwrite(&wBuf, sizeof(WORD), 1, fp_output);
				CONVERT_SHORT(wBuf, wBuf);
				dwTime = dwTemp | wBuf;
				dwTemp = 0;
				for(int i = 0; i < 2; i++)
				{
					fread(&wBuf, sizeof(WORD), 1, fp_input);
				}
				fwrite(&dwTime, sizeof(DWORD), 1, fp_output);
				bFindTime = FALSE;
				bExist = TRUE;
			}
			else
			{
				fwrite(&wBuf, sizeof(WORD), 1, fp_output);
				bFindTime = FALSE;
				fread(&wBuf, sizeof(WORD), 1, fp_input);
				fwrite(&wBuf, sizeof(WORD), 1, fp_output);
				for(int i = 0; i < 2; i++)
				{
					fread(&wBuf, sizeof(WORD), 1, fp_input);
				}
				fwrite(&dwTime, sizeof(DWORD), 1, fp_output);
			}
		}
		else
		{
			fwrite(&wBuf, sizeof(WORD), 1, fp_output);
			fread(&wBuf, sizeof(WORD), 1, fp_input);
			fwrite(&wBuf, sizeof(WORD), 1, fp_output);
			for(int i = 0; i < 2; i++)
			{
				fread(&wBuf, sizeof(WORD), 1, fp_input);
			}
			if (bStartFlag)//Fixed CR 242487
			{
				bStartFlag = FALSE;
				DWORD dwInit = 0;
				fwrite(&dwInit, sizeof(DWORD), 1, fp_output);
			}
			else
			{
				fwrite(&dwTime, sizeof(DWORD), 1, fp_output);
			}
			
		}
	}


	fclose(fp_input);
	fclose(fp_output);

	return bExist;
}

BOOL CDlgImport::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	_TCHAR szConfigFile[_MAX_PATH]={0};
	_stprintf(szConfigFile,_T("%s\\DspLogger.ini"),GetModuleDir(FALSE));
	
	m_nConvertCheck = GetPrivateProfileInt(_T("GUI"),_T("ConvertTpTimeByD0A0"),1,szConfigFile);      
	if(m_nConvertCheck == 1)
	{
		m_btnConvert.SetCheck(BST_CHECKED);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgImport::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	_TCHAR szConfigFile[_MAX_PATH]={0};
	_stprintf(szConfigFile,_T("%s\\DspLogger.ini"),GetModuleDir(FALSE));
	CString strTmp;
	strTmp.Format(_T("%d"),m_nConvertCheck);
    WritePrivateProfileString(_T("GUI"),_T("ConvertTpTimeByD0A0"),strTmp,szConfigFile ); 
	return CDialog::DestroyWindow();
}
