// DlgOSD.cpp : implementation file
//

#include "stdafx.h"
#include "BmpToArray.h"
#include "DlgOSD.h"
#include "sci_types.h"
#include "OsdGenerater.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_COLOR_NUM  6
#define MAX_COLOR_VALUE_NUM  64

const _TCHAR CDlgOSD::SZEXTENSION[] = _T(".c");

const TCHAR CDlgOSD::SZ_SEC_BMPRECT[]       = _T("BmpRect");
const TCHAR CDlgOSD::SZ_KEY_BMPRECT_INDEX[] = _T("br");
const TCHAR CDlgOSD::SZ_SEC_TRANSPARENT_COLOR[] = _T("TransparentColor");
const TCHAR CDlgOSD::SZ_KEY_TRANSPARENT_COLOR[] = _T("tcRGB");
const TCHAR CDlgOSD::SZ_SEC_EXPAND_FACTOR[] = _T("ExpandFactor");
const TCHAR CDlgOSD::SZ_KEY_EXPAND_FACTOR[] = _T("efValue");
const TCHAR CDlgOSD::SZ_SEC_COLOR_SPACE[] = _T("ColorSpace");
const TCHAR CDlgOSD::SZ_KEY_COLOR_SPACE[] = _T("colorspace");

/////////////////////////////////////////////////////////////////////////////
// CDlgOSD dialog


CDlgOSD::CDlgOSD(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOSD::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOSD)
	m_strBmpDir = _T("");
	m_strBmpDirArrayFile = _T("");
	m_strTranspColor = _T("");
	m_strExpandFactor = _T("");
	//}}AFX_DATA_INIT
	memset(m_parrBmpRect,0,sizeof(m_parrBmpRect));
	m_nArrSize = 0;
	m_nTransparentColor = 0;
	m_narrExpandFactor.RemoveAll();
	m_nColorSpace = 0;
//	m_bFresh = FALSE;
}
CDlgOSD::~CDlgOSD()
{
	for(int i = m_nArrSize -1 ; i>=0;i--)
	{
		delete m_parrBmpRect[i];
		m_parrBmpRect[i] = NULL;
	}
}

void CDlgOSD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOSD)
	DDX_Control(pDX, IDC_DO_CMB_EXPAND_FACTOR, m_cmbExpandFactor);
	DDX_Control(pDX, IDC_DO_STC_COLOR, m_stcColor);
	DDX_Text(pDX, IDC_DO_EDT_BMP_DIR, m_strBmpDir);
	DDX_Text(pDX, IDC_DO_EDT_BMP_DIR_ARRAY, m_strBmpDirArrayFile);
	DDX_Text(pDX, IDC_DO_EDT_TRANSPARENT_COLOR, m_strTranspColor);
	DDV_MaxChars(pDX, m_strTranspColor, MAX_COLOR_NUM);
	DDX_CBString(pDX, IDC_DO_CMB_EXPAND_FACTOR, m_strExpandFactor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOSD, CDialog)
	//{{AFX_MSG_MAP(CDlgOSD)
	ON_BN_CLICKED(IDC_DO_BTN_SEL_BMP_DIR, OnBtnSelBmpDir)
	ON_BN_CLICKED(IDC_DO_BTN_SEL_BMP_DIR_ARRAY_FILE, OnBtnSelBmpDirArrayFile)
	ON_BN_CLICKED(IDC_DO_BTN_GEN, OnBtnGen)
	ON_EN_MAXTEXT(IDC_DO_EDT_TRANSPARENT_COLOR, OnMaxtextEdtTransparentColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOSD message handlers

void CDlgOSD::OnBtnSelBmpDir() 
{
	// TODO: Add your control notification handler code here
	m_strBmpDir = g_theApp.SelectFolder(this->m_hWnd, NULL);
    UpdateData(FALSE);
	
    m_strBmpDir.TrimRight(_T('\\'));
	
    if( !m_strBmpDir.IsEmpty() )
    {
        int nFind = m_strBmpDir.ReverseFind(_T('\\'));
        if( nFind > 1 )
        {
            m_strBmpDirArrayFile = m_strBmpDir + m_strBmpDir.Right(m_strBmpDir.GetLength() - nFind) + SZEXTENSION;
        }
        else
        {
            m_strBmpDirArrayFile = m_strBmpDir + _T("\\BmpDirArray") + SZEXTENSION;
        }
        UpdateData(FALSE);
    }	
}

void CDlgOSD::OnBtnSelBmpDirArrayFile() 
{
	// TODO: Add your control notification handler code here
	m_strBmpDirArrayFile = g_theApp.SelectFile(FALSE, _T("c"));
	
    UpdateData(FALSE);	
}

void CDlgOSD::OnBtnGen() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(!CheckTransparentColor())
		return;
	
	if( m_strBmpDir.IsEmpty() || m_strBmpDirArrayFile.IsEmpty())
    {
        AfxMessageBox(ID_ERR_SELECT);
        return;
    }
	
	m_strBmpDirArrName = DistillFileName(m_strBmpDirArrayFile);	
	
	GenerateTargetFile();
}
CString CDlgOSD::DistillFileName(LPCTSTR pPath)
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

void CDlgOSD::EnumFiles(CStringArray &arrFiles, LPCTSTR pDir)
{
    _ASSERTE( pDir != NULL );
	
    arrFiles.RemoveAll();
	
    CString strDir  = pDir;
    CString strName = pDir;
    strName += _T("\\*");
	
    WIN32_FIND_DATA findData;
    HANDLE hFile = ::FindFirstFile(strName, &findData);
    if( hFile != INVALID_HANDLE_VALUE )
    {
        ::FindNextFile(hFile, &findData);
        while( ::FindNextFile(hFile, &findData) )
        {
            strName = findData.cFileName;
            strName.MakeLower();
            if( strName.Find(_T(".bmp")) != -1 )
            {
                arrFiles.Add( strDir + _T("\\") + strName);
            }
        }
    }
}

uint32 RGB2YUV(uint32 rgb_value)
{
	//input: 0x0RGB , output: 0x0YUV
	uint8 r = 0;
	uint8 g = 0;
	uint8 b = 0;
	
	uint8 y = 0;
	uint8 u = 0;
	uint8 v = 0;
	
	uint32 yuv_value = 0;
	
	r = uint8((rgb_value & 0x00ff0000) >> 16);
	g = uint8((rgb_value & 0x0000ff00) >> 8);
	b = uint8((rgb_value & 0x000000ff));
	
	y = uint8(  0.299 * r + 0.587 * g + 0.114 * b  - 128);
	u = uint8(-0.147 * r - 0.289 * g + 0.436 * b);
	v = uint8(0.615 *r - 0.515 * g - 0.100 * b);
	
	yuv_value = ((y << 16) | (u << 8) | v) & 0x00FFFFFF;
	
	return yuv_value;
}

uint32 RGB2YCbCr(uint32 rgb_value)
{
	//input: 0x0RGB , output: 0x0YUV
	uint8 r = 0;
	uint8 g = 0;
	uint8 b = 0;
	
	uint8 y = 0;
	uint8 cb = 0;
	uint8 cr = 0;
	
	uint32 yCbCr_value = 0;
	
	r = uint8((rgb_value & 0x00ff0000) >> 16);
	g = uint8((rgb_value & 0x0000ff00) >> 8);
	b = uint8((rgb_value & 0x000000ff));
	
	y = uint8(  0.299 * r + 0.587 * g + 0.114 * b  - 128);
	cb = uint8(-0.1687 * r - 0.3313 * g + 0.5 * b);
	cr = uint8(0.5 *r - 0.4187 * g - 0.0813 * b);
	
	yCbCr_value = ((y << 16) | (cb << 8) | cr) & 0x00FFFFFF;
	
	return yCbCr_value;
}


BOOL CDlgOSD::GenerateTargetFile(void)
{
    uint8 * pBmpContent      = NULL;
    uint32  nBmpContentSize  = 0;
    BOOL bBmp  = FALSE;

    COsdGenerater generater; 

	generater.SetParameters(m_nTransparentColor,m_parrBmpRect,m_nArrSize);
    
    CStringArray arrFiles;
	CString strFile;

	CString strError;

    if( !m_strBmpDir.IsEmpty() )
    {
        EnumFiles(arrFiles, m_strBmpDir);
    }

	if(arrFiles.GetSize()==0)
	{
		strError.Format(IDS_ERR_NO_BMP,m_strBmpDir);
		AfxMessageBox(strError);
		return FALSE;
	}
	
	generater.BeginGenerate( m_strBmpDirArrayFile);
    for( int i = 0; i < arrFiles.GetSize(); ++i)
    {
        strFile = arrFiles[i];
        if( generater.LoadFromFile( strFile, &pBmpContent, (int&)nBmpContentSize ) )
        {
            bBmp = generater.Generate(DistillFileName(strFile), pBmpContent, nBmpContentSize );
            generater.DeleteLoadContent(pBmpContent);
        }
		else
		{
			generater.EndGenerate();
			bBmp = FALSE;

			CString strErr;
			strErr.Format(IDS_ERR_LOAD_FILE, strFile);
			AfxMessageBox(strErr);
			
			VERIFY(::DeleteFile(m_strBmpDirArrayFile));
			return FALSE;
		}
    }	
	generater.EndGenerate();

    typedef uint32 (*PCONVERT_FUNC)(uint32 rgb_value);
	PCONVERT_FUNC pConvertFunc;
	if(m_nColorSpace == 0)
		pConvertFunc = RGB2YCbCr;
	else
		pConvertFunc = RGB2YUV;

//存储调色板信息
	int		palette_size = MAX_COLOR_VALUE_NUM;
	uint32	palette_arr[MAX_COLOR_VALUE_NUM] = {0};
	uint32	*pContent = palette_arr;
	uint32	yuv_value = 0;
	
	memcpy(palette_arr, generater.m_arrPalette, palette_size * sizeof(uint32));
//	if (generater.m_arrPalette[MAX_COLOR_VALUE_NUM] != 0)
//	{
//		AfxMessageBox(IDS_ERR_NEW_FAIL);
//		return FALSE;
//	}

	CStdioFile m_file;
	strFile = m_strBmpDirArrayFile;
	strFile.Replace(_T(".c"), _T(".pale"));
    if( !m_file.Open(strFile, CFile::modeCreate | CFile::modeWrite) )
    {
        AfxMessageBox(IDS_ERR_CREATE_FILE);
        return FALSE;
    }
    // write some flag(myself define)
    m_file.WriteString( _T("\n// Array file.\n\n") );
    CString strLine;
    strLine.Format(_T("const MMIDC_OSD_PALATTE_T palette =\n{"));
    m_file.WriteString(strLine);
    
    char szValue[ 16 ] = { 0 };

    for(i = 0; i < palette_size; ++i)
    {
        if( i % 16 == 0 )
        {
            m_file.WriteString(_T("\n    "));
        }
        
		yuv_value = pConvertFunc(*pContent);
		sprintf(szValue, "%08x", yuv_value);
		strLine.Format(_T("0x%s,"), szValue);
		m_file.WriteString(strLine);  

        pContent++;
    }
    
    m_file.WriteString(_T("\n};\n"));

	m_file.Close();
//end

    arrFiles.RemoveAll();



    if( bBmp)
    {
        AfxMessageBox(_T("Generate Complete!"));
    }
    else
    {
        AfxMessageBox(_T("Generate Fail!"));
    }

    return TRUE;	
}

BOOL CDlgOSD::CheckTransparentColor()
{
	UpdateData();
    CEdit *pEdit = (CEdit *)GetDlgItem(IDC_DO_EDT_TRANSPARENT_COLOR);

	if(m_strTranspColor.GetLength() != MAX_COLOR_NUM )
	{
		AfxMessageBox(_T("The transparent color value is not correct!"));
		pEdit->SetFocus();
		return FALSE;		
	}
	
	TCHAR *p= m_strTranspColor.GetBuffer(0);
	p=p+2;
	while(*p!='\0')
	{
		if(!((*p>='0' && *p<='9')|| (*p>='a' && *p<='f') || (*p>='A' && *p<='F')) )
		{
		    AfxMessageBox(_T("The transparent color value is not correct!"));
			pEdit->SetFocus();
			return FALSE;		
		}
		p++;
	}
	return TRUE;
}

void  CDlgOSD::SetColor(CString strColor)
{
	_ASSERT(!strColor.IsEmpty());
	_TCHAR *pszColor = strColor.GetBuffer(0);
	_stscanf(pszColor,_T("%x"),&m_nTransparentColor);
	strColor.ReleaseBuffer();
}

void CDlgOSD::OnMaxtextEdtTransparentColor() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
    if(CheckTransparentColor())
	{
		_TCHAR *pszColor = m_strTranspColor.GetBuffer(0);
		_stscanf(pszColor,_T("%x"),&m_nTransparentColor);
		m_strTranspColor.ReleaseBuffer();	
		m_stcColor.SetColor(m_nTransparentColor);
        Invalidate();
	}
}


BOOL CDlgOSD::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	LoadConfig();

	CString str;
	for(int i=0;i<m_narrExpandFactor.GetSize();i++)
	{
		str.Format(_T("%d"),m_narrExpandFactor[i]);
		m_cmbExpandFactor.AddString(str);
	}	

    UpdateData(FALSE);
	
	m_cmbExpandFactor.SetCurSel(0);
	UpdateData(TRUE);	
	
	m_cmbExpandFactor.EnableWindow(FALSE);
	
	m_stcColor.SetColor(m_nTransparentColor);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgOSD::LoadConfig() 
{
	_TCHAR szPathName[_MAX_PATH];
    ::GetModuleFileName(NULL, szPathName, _MAX_PATH);
	
	LPTSTR pFind = _tcsrchr(szPathName, _T('.'));
	_tcscpy( pFind + 1, _T("ini") );
	
    int nIndex = 1;	
	
	CString strID;
	CString strKeyName;
    BOOL bMore = TRUE;
	BMP_RECT_T *pBRT;
	int nRet;
	TCHAR szTemp[256];
	
	//read bmprect	
	while(bMore)
	{
		strID.Format(_T("%d"),nIndex);
		strKeyName= SZ_KEY_BMPRECT_INDEX + strID;
		nIndex++;

		nRet = GetPrivateProfileString(SZ_SEC_BMPRECT, strKeyName, _T(""),szTemp,256, szPathName);
		if(nRet == 0)
		{
			bMore = FALSE;
			break;
		}
		CString strBmpRect(szTemp);
        strBmpRect.Replace(_T(" "),_T(""));
		int nFind = strBmpRect.Find(',',0);
		if(nFind <= 0)
		{
			CString strError;
			strError.Format(_T("Read configure [%s] file failed"),SZ_SEC_BMPRECT);
			AfxMessageBox(strError);
			continue;
		}
		pBRT = new BMP_RECT_T;
		pBRT->nHeight = _ttoi(strBmpRect.Left(nFind));
		pBRT->nWidth = _ttoi(strBmpRect.Right(strBmpRect.GetLength() -nFind -1));
		m_parrBmpRect[m_nArrSize++] = pBRT;		

	}
	
	//read transparent color	
	GetPrivateProfileString(SZ_SEC_TRANSPARENT_COLOR, SZ_KEY_TRANSPARENT_COLOR, _T("0xFF00FF"),szTemp,256, szPathName);
	
	

    m_strTranspColor = szTemp;
	m_strTranspColor.Delete(0,2);
	_stscanf(szTemp,_T("%x"),&m_nTransparentColor);

	
	//read expand factor
	bMore = TRUE;
	nIndex = 1;
	int nValue = 0;
	while(bMore)
	{
		strID.Format(_T("%d"),nIndex);
		strKeyName= SZ_KEY_EXPAND_FACTOR + strID;
		
		nValue = GetPrivateProfileInt(SZ_SEC_EXPAND_FACTOR, strKeyName, 99999, szPathName);
		if(nValue == 99999)
		{
			bMore = FALSE;
			break;
		}
		m_narrExpandFactor.Add(nValue);
		nIndex++;
		
	}
	
	m_nColorSpace = 0; //YCbCr
	//Read ColorSpace
	m_nColorSpace = GetPrivateProfileInt(SZ_SEC_COLOR_SPACE, SZ_KEY_COLOR_SPACE, 0, szPathName);

	return TRUE;
}

BOOL CDlgOSD::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE ||pMsg->wParam == VK_RETURN  )//ESC or Enter
		{
			return TRUE;
		}
	}

	CWnd * pEdtColor = GetDlgItem(IDC_DO_EDT_TRANSPARENT_COLOR);
	if( pMsg->message == WM_CHAR )
	{
		CWnd * pWnd = GetFocus();
		if( pWnd != NULL && pWnd->m_hWnd == pEdtColor->m_hWnd )
		{
			_TCHAR cValue = (_TCHAR)(pMsg->wParam);
			if( cValue < _T('0') || cValue > _T('9') )
			{
				if( cValue < _T('a') || cValue > _T('f') )
				{	
					if( cValue < _T('A') || cValue > _T('F') )
					{						
						if(  cValue != VK_SPACE && cValue != VK_BACK  && cValue != 3 && cValue != 22 && cValue != 24) //Ctrl+c Ctrl+v Ctrl+x
						{
							return TRUE;
						}	
					}
				}
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
