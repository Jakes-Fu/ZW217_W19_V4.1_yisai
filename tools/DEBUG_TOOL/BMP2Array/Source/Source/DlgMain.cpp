// DlgMain.cpp : implementation file
//

#include "stdafx.h"
#include "BmpToArray.h"
#include "DlgMain.h"
#include "sci_types.h"
#include "Generater.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const _TCHAR CDlgMain::SZEXTENSION[] = _T(".c");
/////////////////////////////////////////////////////////////////////////////
// CDlgMain dialog


CDlgMain::CDlgMain(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMain::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMain)
	m_strAnimArrayFileName = _T("");
	m_strAnimDirName = _T("");
	m_strBmpDirArrayFileName = _T("");
	m_strBmpDirName = _T("");
	m_strBmpArrayFileName = _T("");
	m_strBmpFileName = _T("");
	m_bBigEndian = TRUE;
	m_nCmpType = 0;
	//}}AFX_DATA_INIT
	m_strBmpBinFileName = _T("");
	m_strBmpDirBinPath = _T("");
	m_strAnimBinFile = _T("");

}


void CDlgMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMain)
	DDX_Text(pDX, IDC_DM_EDT_ANIM_DIR_ARRAY_NAME, m_strAnimArrayFileName);
	DDX_Text(pDX, IDC_DM_EDT_ANIM_DIR_NAME, m_strAnimDirName);
	DDX_Text(pDX, IDC_DM_EDT_BMP_DIR_ARRAY_NAME, m_strBmpDirArrayFileName);
	DDX_Text(pDX, IDC_DM_EDT_BMP_DIR_NAME, m_strBmpDirName);
	DDX_Text(pDX, IDC_DM_EDT_BMP_FILE_ARRAY_NAME, m_strBmpArrayFileName);
	DDX_Text(pDX, IDC_DM_EDT_BMP_FILE_NAME, m_strBmpFileName);
	DDX_Check(pDX, IDC_DM_CHK_BIGENDIAN, m_bBigEndian);
	DDX_CBIndex(pDX, IDC_CMB_CMP_IMG_TYPE, m_nCmpType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMain, CDialog)
	//{{AFX_MSG_MAP(CDlgMain)
	ON_BN_CLICKED(IDC_DM_BTN_SEL_BMP_FILE, OnBtnSelBmpFile)
	ON_BN_CLICKED(IDC_DM_BTN_SEL_BMP_DIR, OnBtnSelBmpDir)
	ON_BN_CLICKED(IDC_DM_BTN_SEL_ANIM_DIR, OnBtnSelAnimDir)
	ON_BN_CLICKED(IDC_DM_BTN_SEL_BMP_FILE_ARRAY_FILE, OnBtnSelBmpFileArrayFile)
	ON_BN_CLICKED(IDC_DM_BTN_SEL_BMP_DIR_ARRAY_FILE, OnBtnSelBmpDirArrayFile)
	ON_BN_CLICKED(IDC_DM_BTN_SEL_ANIM_DIR_ARRAY_FILE, OnBtnSelAnimDirArrayFile)
	ON_BN_CLICKED(IDC_DM_BTN_GEN, OnBtnGen)
	ON_CBN_SELCHANGE(IDC_CMB_CMP_IMG_TYPE, OnSelchangeCmbCmpImgType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMain message handlers

void CDlgMain::OnBtnSelBmpFile() 
{
	// TODO: Add your control notification handler code here
    CFileDialog fd(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, 
		           _T("BMP Files (*.bmp)|*.bmp|JPG Files (*.jpeg;*.jpg)|*.jpeg;*.jpg|GIF Files (*.gif)|*.gif|PNG Files (*.png)|*.png||"));
	
    if( fd.DoModal() == IDCANCEL )
    {
		return;
	}	
	
	CString strExt = fd.GetFileExt();
    UpdateData(TRUE);	

	CComboBox *pWnd = (CComboBox*)GetDlgItem(IDC_CMB_CMP_IMG_TYPE);
	if(pWnd != NULL)
	{
		if(strExt.CompareNoCase(_T("png"))==0)
		{
			pWnd->SetCurSel(3);
			pWnd->EnableWindow(FALSE);
		}
		else if(strExt.CompareNoCase(_T("gif"))==0)
		{
			pWnd->SetCurSel(2);
			pWnd->EnableWindow(FALSE);
		}
		else if(strExt.CompareNoCase(_T("jpg"))==0 || strExt.CompareNoCase(_T("jpeg"))==0)
		{
			pWnd->SetCurSel(1);
			pWnd->EnableWindow(FALSE);
		}
		else if(strExt.CompareNoCase(_T("bmp"))==0)
		{
			pWnd->SetCurSel(0);			
		}
		OnSelchangeCmbCmpImgType();
	}	

	m_strBmpFileName = fd.GetPathName();
    
    if( !m_strBmpFileName.IsEmpty() )
    {
		m_strBmpArrayFileName = m_strBmpFileName;
        int nFind = m_strBmpArrayFileName.ReverseFind(_T('.'));
        if( nFind != -1 )
        {
            m_strBmpArrayFileName = m_strBmpArrayFileName.Left( nFind );
        }		
        m_strBmpArrayFileName += SZEXTENSION;
		
		if (g_theApp.m_bGenBinFile) //gen bin file
		{
			m_strBmpBinFileName = m_strBmpArrayFileName.Left(m_strBmpArrayFileName.ReverseFind(_T('.')));
			m_strBmpBinFileName += g_theApp.m_szBinSuffixed;

		}
        UpdateData(FALSE);
    }
}

void CDlgMain::OnBtnSelBmpDir() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	m_strBmpDirName = g_theApp.SelectFolder(this->m_hWnd, _T("Please select a folder!"));

    UpdateData(FALSE);
	
    m_strBmpDirName.TrimRight(_T('\\'));
	
    if( !m_strBmpDirName.IsEmpty() )
    {
        int nFind = m_strBmpDirName.ReverseFind(_T('\\'));
        if( nFind > 1 )
        {
            m_strBmpDirArrayFileName = m_strBmpDirName + m_strBmpDirName.Right(m_strBmpDirName.GetLength() - nFind) + SZEXTENSION;
		}
        else
        {
            m_strBmpDirArrayFileName = m_strBmpDirName + _T("\\BmpDirArray") + SZEXTENSION;
        }
		if (g_theApp.m_bGenBinFile) //gen bin file
		{
			m_strBmpDirBinPath = m_strBmpDirName;
		}
		
        UpdateData(FALSE);
    }	
}

void CDlgMain::OnBtnSelAnimDir() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CString strDir  = g_theApp.SelectFolder(this->m_hWnd, _T("Please select a folder!"));
	if(strDir.IsEmpty())
	{
		return;
	}

	m_strAnimDirName = strDir;
    UpdateData(FALSE);	
	
    m_strAnimDirName.TrimRight(_T('\\'));
	
    if( !m_strAnimDirName.IsEmpty() )
    {
        int nFind = m_strAnimDirName.ReverseFind(_T('\\'));
        if( nFind > 1 )
        {
            m_strAnimArrayFileName = m_strAnimDirName + m_strAnimDirName.Right(m_strAnimDirName.GetLength() - nFind) + SZEXTENSION;
		}
        else
        {
            m_strAnimArrayFileName = m_strAnimDirName + _T("\\AnimArray") + SZEXTENSION;
        }
		if (g_theApp.m_bGenBinFile) //gen bin file
		{
			m_strAnimBinFile = m_strAnimArrayFileName.Left(m_strAnimArrayFileName.ReverseFind(_T('.')));
			m_strAnimBinFile +=g_theApp.m_szBinSuffixed;
		}

        UpdateData(FALSE);
    }
}

void CDlgMain::OnBtnSelBmpFileArrayFile() 
{
	// TODO: Add your control notification handler code here
	m_strBmpArrayFileName = g_theApp.SelectFile(FALSE, _T("c"));
	GetDlgItem(IDC_DM_EDT_BMP_FILE_ARRAY_NAME)->SetWindowText(m_strBmpArrayFileName);
	if (g_theApp.m_bGenBinFile) //gen bin file
	{
		m_strBmpBinFileName = m_strBmpArrayFileName.Left(m_strBmpArrayFileName.ReverseFind(_T('.')));
		m_strBmpBinFileName +=g_theApp.m_szBinSuffixed;
	}

}

void CDlgMain::OnBtnSelBmpDirArrayFile() 
{
	// TODO: Add your control notification handler code here
	m_strBmpDirArrayFileName = g_theApp.SelectFile(FALSE, _T("c"));
	GetDlgItem(IDC_DM_EDT_BMP_DIR_ARRAY_NAME)->SetWindowText(m_strBmpDirArrayFileName);
	if (g_theApp.m_bGenBinFile) //gen bin file
	{
		m_strBmpDirBinPath = m_strBmpDirArrayFileName.Left(m_strBmpDirArrayFileName.ReverseFind(_T('\\')));
	}

}



void CDlgMain::OnBtnSelAnimDirArrayFile() 
{
	// TODO: Add your control notification handler code here
	m_strAnimArrayFileName = g_theApp.SelectFile(FALSE, _T("c"));
	GetDlgItem(IDC_DM_EDT_ANIM_DIR_ARRAY_NAME)->SetWindowText(m_strAnimArrayFileName);
	if (g_theApp.m_bGenBinFile) //gen bin file
	{
		m_strAnimBinFile = m_strAnimArrayFileName.Left(m_strAnimArrayFileName.ReverseFind(_T('.')));
		m_strAnimBinFile += g_theApp.m_szBinSuffixed;
	}

}

void CDlgMain::OnBtnGen() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	if( m_strBmpFileName.IsEmpty() && m_strBmpDirName.IsEmpty() && m_strAnimDirName.IsEmpty() )
    {
        AfxMessageBox(ID_ERR_SELECT);
        return;
    }

	if( (!m_strBmpFileName.IsEmpty() && m_strBmpArrayFileName.IsEmpty()) ||
		(!m_strBmpDirName.IsEmpty() && m_strBmpDirArrayFileName.IsEmpty()) ||
		(!m_strAnimDirName.IsEmpty() && m_strAnimArrayFileName.IsEmpty()) )
	{
		AfxMessageBox(_T("Please input the ouput file path!"));
        return;
	}

		
    m_strBmpArrName = DistillFileName(m_strBmpArrayFileName);
    m_strAniArrName = DistillFileName(m_strAnimArrayFileName);

	GenerateArray();	


}

void CDlgMain::EnumFiles(CStringArray &arrFiles, LPCTSTR pDir)
{
    _ASSERTE( pDir != NULL );
	
    arrFiles.RemoveAll();
	
    CString strDir  = pDir;
    CString strName = pDir;
	CString strName2 = _T("");
	if(m_nCmpType == SPNG)
	{
		strName += _T("\\*.png");
	}
	else if(m_nCmpType == SGIF)
	{
		strName += _T("\\*.gif");
	}
	else if(m_nCmpType == SJPG)
	{
		strName += _T("\\*.jpg");
		strName2 = pDir;
		strName2 +=  _T("\\*.jpeg");
	}
	else if(m_nCmpType == BMP565_RLE)
	{
		strName += _T("\\*.bmp");
	}
	else
	{
		return;
	}
	
    WIN32_FIND_DATA findData;
    HANDLE hFile = ::FindFirstFile(strName, &findData);
    if( hFile != INVALID_HANDLE_VALUE )
    {        
        do
        {
            strName = findData.cFileName;
            strName.MakeLower();
            if( (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 )
            {
                arrFiles.Add( strDir + _T("\\") + strName);
            }
        }while( ::FindNextFile(hFile, &findData) );

		::FindClose(hFile);
    }

	if(!strName2.IsEmpty())
	{
		hFile = ::FindFirstFile(strName, &findData);
		if( hFile != INVALID_HANDLE_VALUE )
		{
			//::FindNextFile(hFile, &findData);
			while( ::FindNextFile(hFile, &findData) )
			{
				strName = findData.cFileName;
				strName.MakeLower();
				if( (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 )
				{
					arrFiles.Add( strDir + _T("\\") + strName);
				}
			}
			::FindClose(hFile);
		}
	}

}

CString CDlgMain::DistillFileName(LPCTSTR pPath)
{
    _ASSERTE( pPath != NULL );
	
    CString strRet;
    LPTSTR pFind = _tcsrchr(pPath, _T('\\'));
    if( pFind != NULL )
    {
        strRet = pFind + 1;
        int nFind = strRet.ReverseFind(_T('.'));
        if( nFind != -1 )
        {
            strRet = strRet.Left(nFind);
        }
    }
    else
    {
        strRet = _T("ArrayName");
    }
    
    return strRet;
}

BOOL CDlgMain::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgMain::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE ||pMsg->wParam == VK_RETURN  )//ESC or Enter
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgMain::GenerateArray()
{
    uint8 * pImgContent      = NULL;
    uint32  nImgContentSize  = 0;
    uint8 * pAnimContent     = NULL;
    uint32  nAnimContentSize = 0;

    CString strFileName;
    BOOL bImgGen = FALSE;
	BOOL bImgDirGen = FALSE;
    BOOL bAnimGen = FALSE;

    CGenerater generater(m_bBigEndian);

    if( !m_strBmpFileName.IsEmpty() && !m_strBmpArrayFileName.IsEmpty() )
    {
        bImgGen = generater.LoadImage(m_strBmpFileName, &pImgContent, &nImgContentSize,m_nCmpType);
        if( bImgGen )
		{
			if(generater.BeginGenerate( m_strBmpArrayFileName ))
			{
				bImgGen = generater.Generate(m_strBmpArrName, pImgContent, nImgContentSize);
				generater.EndGenerate();
			}
			if (g_theApp.m_bGenBinFile) //gen bin file
			{
				generater.GenerateBinFile(m_strBmpBinFileName, pImgContent, nImgContentSize);
			}
		} 

        generater.FreeImage(pImgContent);
    }

    CStringArray arrFiles;
    if( !m_strBmpDirName.IsEmpty() && !m_strBmpDirArrayFileName.IsEmpty() )
    {
        EnumFiles(arrFiles, m_strBmpDirName);
		if(arrFiles.GetSize() > 0)
		{     	
			if( generater.BeginGenerate( m_strBmpDirArrayFileName ) )
			{
				CString strFile;
				CString strDestBinFile;
				for( int i = 0; i < arrFiles.GetSize(); ++i)
				{
					strFile = arrFiles[i];
					pImgContent = NULL;
					bImgDirGen = generater.LoadImage(strFile, &pImgContent, &nImgContentSize,m_nCmpType);
					if( bImgDirGen )
					{
						generater.Generate(DistillFileName(strFile), pImgContent, nImgContentSize);
						if (g_theApp.m_bGenBinFile) //gen bin file
						{
							strDestBinFile = m_strBmpDirBinPath + _T("\\") + DistillFileName(strFile) + g_theApp.m_szBinSuffixed;
							generater.GenerateBinFile(strDestBinFile, pImgContent, nImgContentSize);
						}

					}

					generater.FreeImage(pImgContent);
				}

				generater.EndGenerate();
			}
		}
		else
		{
			AfxMessageBox(_T("Image folder does not include invalid images!"));
		}
        
        arrFiles.RemoveAll();
    }

    if( !m_strAnimDirName.IsEmpty() && !m_strAnimArrayFileName.IsEmpty() )
    {
        bAnimGen = generater.LoadAnim(m_strAnimDirName, &pAnimContent, &nAnimContentSize,0);
        if( bAnimGen )
		{
			if( generater.BeginGenerate( m_strAnimArrayFileName ) )
			{
				bAnimGen = generater.Generate(m_strAniArrName, pAnimContent, nAnimContentSize);
				generater.EndGenerate();
			}	
			if (g_theApp.m_bGenBinFile) //gen bin file
			{
				generater.GenerateBinFile(m_strAnimBinFile, pImgContent, nImgContentSize);
			}


		}		
        generater.FreeAnim(pAnimContent);
    }

   
    if( bImgGen || bImgDirGen || bAnimGen )
    {
        AfxMessageBox(_T("Generate Ok!"));
    }
    else
    {		
		AfxMessageBox(_T("Generate Fail!"));	
    }
    return TRUE;
}

void CDlgMain::OnSelchangeCmbCmpImgType() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_nCmpType == 0)
	{
		GetDlgItem(IDC_DM_CHK_BIGENDIAN)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_DM_CHK_BIGENDIAN)->ShowWindow(SW_HIDE);
	}
}
