// FirAutoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "FirAutoDlg.h"

#include "fir_eq.h"

#include "AudioTesterSheet.h"
#include <math.h>
#pragma  warning(push,3)
#include <complex>
#pragma  warning(pop)

#include "comdef.h" 
#include "excel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFirAutoDlg dialog

/*static*/
CString CFirAutoDlg::m_strTarEqFile;

int CFirAutoDlg::m_vPowerFrequence[ROW_COUNT] = 
{
    200,224,250,281,315,355,400,447,500,561,630,710,800,894,1000,1118,1250,1424,1580,1789,2000,2236,2500,2806,3150,3520
};

CFirAutoDlg::CFirAutoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFirAutoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFirAutoDlg)
	m_bShowOri = TRUE;
	m_bShowTar = TRUE;
	m_bShowReal = TRUE;
	m_nGain = 0;
	m_bShowModified = TRUE;
	m_bShowSpec = TRUE;
	m_dOffset = 0.0;
	m_bShowFinal = FALSE;
	//}}AFX_DATA_INIT

    m_nEqNum = 0;
    m_bOriReady = FALSE;
    m_bTarReady = FALSE;
    m_bReady = FALSE;

    m_Graphy.m_pDataSource = this;

    m_nHighEqNum = 0;
    m_nLowEqNum = 0;
	m_bAuto = TRUE;

	m_pTempArea = NULL;
	m_nLoadOriginalFlag = NO_LOAD_ORG;
	m_bVisibleFirButton = TRUE;
}


void CFirAutoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFirAutoDlg)
	DDX_Control(pDX, IDC_STATIC_FIR_AUTO_GRAPHY, m_Graphy);
	DDX_Control(pDX, IDC_STATIC_FIR_DRAG, m_Drag);
	DDX_Check(pDX, IDC_CHECK_ORIGINAL, m_bShowOri);
	DDX_Check(pDX, IDC_CHECK_TARGET, m_bShowTar);
	DDX_Check(pDX, IDC_CHECK_RESULT, m_bShowReal);
	DDX_Text(pDX, IDC_EDIT_EQ_FIR_GAIN_AUTO, m_nGain);
	DDX_Check(pDX, IDC_CHECK_MODIFIED, m_bShowModified);
	DDX_Check(pDX, IDC_CHECK_SPEC, m_bShowSpec);
	DDX_Text(pDX, IDC_EDIT_FIR_OFFSET, m_dOffset);
	DDX_Check(pDX, IDC_CHECK_FINAL, m_bShowFinal);
	DDX_Control(pDX, IDC_LIST_FIR_VALUE, m_listFir);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFirAutoDlg, CDialog)
	//{{AFX_MSG_MAP(CFirAutoDlg)
	ON_BN_CLICKED(IDC_BUTTON_FIR_SPEC, OnButtonFirSpec)
	ON_BN_CLICKED(IDC_BUTTON_FIR_ORIGINAL, OnButtonFirOriginal)
	ON_BN_CLICKED(IDC_BUTTON_FIR_TARGET, OnButtonFirTarget)
	ON_BN_CLICKED(IDC_BUTTON_FIR_ADJUST, OnButtonFirAdjust)
	ON_BN_CLICKED(IDC_CHECK_ORIGINAL, OnCheckOriginal)
	ON_BN_CLICKED(IDC_CHECK_RESULT, OnCheckResult)
	ON_BN_CLICKED(IDC_CHECK_TARGET, OnCheckTarget)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_MODIFIED, OnCheckModified)
	ON_BN_CLICKED(IDC_CHECK_SPEC, OnCheckSpec)
	ON_BN_CLICKED(IDC_CHECK_FINAL, OnCheckFinal)
	ON_BN_CLICKED(IDC_BUTTON_OFFSET_APPLY, OnButtonOffsetApply)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT_FIR, OnButtonDefaultFir)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_FIR, OnButtonRefreshFir)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFirAutoDlg message handlers

BOOL BrowseFile( CString& strFile,CString& strFilter )
{
    CFileDialog dlg( TRUE,NULL,strFile,0,strFilter );
    if( IDOK == dlg.DoModal() )
    {
        strFile = dlg.GetPathName();
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// Load *.trc file
BOOL LoadTrcFile( CString strFile,int* pFrequency,double* pEq,int* pEqNum )
{
    CStdioFile file( strFile,CFile::modeRead );

    CString strValue;
    BOOL bBegin = FALSE;
    *pEqNum = 0;
    while( file.ReadString( strValue ) )
    {
        if( !bBegin )
        {
            if( -1 != strValue.Find( _T("#--") ) )
            {
                // The content in the file from this line are the 
                // eq values we want to read
                bBegin = TRUE;
            }
        }
        else
        {
            // Format:  frequency   value
            double dEq;
#ifdef _UNICODE
			int num = swscanf( strValue,_T("%d %lf"),pFrequency,&dEq );
#else
			int num = sscanf( (LPSTR)(LPCTSTR)strValue,"%d %lf",pFrequency,&dEq );
#endif
            if( num == 2 )
            {
                (*pEqNum)++;
                pFrequency++;

                // The eq value in trc file is absolute value,
                // we change it to db value
                *pEq = 20 * log10( dEq );
                //*pEq = dEq;
                pEq++;
            }
            else
            {
                // the end of valid eq value
                break;
            }
        }
    }

    file.Close();

    return TRUE;
}

// Load *.xls file
BOOL LoadXlsFile( CString strFile,int* pFrequency,double* pEq,int* pEqNum )
{

    CoInitialize(NULL);
    
	_Application	ExcelApp; 
	Workbooks		wbsWorkbooks; 
	_Workbook		wbWorkbook; 
	Worksheets		wssWorksheets; 
	_Worksheet		wsWorksheet; 
    LPDISPATCH      lpdisp;

    *pEqNum = 0;
	
	//创建 Excel 服务器(启动Excel) 	
	if (!ExcelApp.CreateDispatch(_T("Excel.Application"),NULL)) 
	{ 
		AfxMessageBox(_T("create Excel server fail!")); 
        CoUninitialize();
		return FALSE; 
	} 
	
	COleVariant covOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR); 
	COleVariant covFalse((short)FALSE);
	
	wbsWorkbooks.AttachDispatch(ExcelApp.GetWorkbooks());
	
	lpdisp=wbsWorkbooks.Open(strFile, //初始化. 
		covOpt,covOpt,covOpt,covOpt,covOpt,covOpt,covOpt,covOpt,covOpt,covOpt,covOpt,covOpt); 
    wbWorkbook.AttachDispatch(lpdisp);
	wssWorksheets.AttachDispatch(wbWorkbook.GetWorksheets(),true); 	
	wsWorksheet.AttachDispatch(wssWorksheets.GetItem(_variant_t((long)1))); 
	
	Range rgUsedRange;
	rgUsedRange.AttachDispatch(wsWorksheet.GetUsedRange()); 

	VARIANT ret;
	ret = rgUsedRange.GetValue2();
	COleSafeArray sa(ret);
	long lNumRows;
	long lNumCols;

	sa.GetUBound(1,&lNumRows);
	sa.GetUBound(2,&lNumCols);
    long index[2];
    VARIANT var;

	CStringArray aRow;
	CString strID = _T("");
	CString strCell;

	int i;

	///检测列数和列名
	if(lNumCols == 0)
	{
		AfxMessageBox(_T("Sheet1 doesn't exist or is empty!"));
		wbWorkbook.Close(covFalse,COleVariant(strFile),covOpt); 
		wbsWorkbooks.Close(); 
		ExcelApp.Quit(); 
        CoUninitialize();
		return FALSE;
	}	
	
	for(i = 15;i<=lNumRows;i++)
	{
		aRow.RemoveAll();
		
		//read freqency
		index[0]=i;
		index[1]=1;
		sa.GetElement(index,&var);

        if( var.dblVal < 100 || var.dblVal > 4000 )
        {
            // We focus on eq that freq is between 100 and 4000
            continue;
        }

        *pFrequency = (int)var.dblVal;
        pFrequency++;

        // Read eq
        index[1]=2;
        sa.GetElement(index,&var);
        
		*pEq = var.dblVal;
        pEq++;
        
        (*pEqNum)++;
	}

	wbWorkbook.Close(covFalse,COleVariant(strFile),covOpt); 
	wbsWorkbooks.Close(); 
	ExcelApp.Quit(); 
 //	CoUninitialize();
///////////////////////////////////////////////////////
    return TRUE;
}

BOOL LoadSpec( CString& strFile,int* pHighFreq,double* pHighEq,int& high_num,
                                int* pLowFreq,double* pLowEq,int& low_num )
{               
    CStdioFile file( strFile,CFile::modeRead );
    
    CString strValue;
    high_num = 0;
    low_num = 0;
    while( file.ReadString( strValue ) )
    {
        if( 0 == strValue.Find( _T("High_Freq") ) )
        {
            // Read high freq
            // Remove letters
            strValue.Delete( 0,9 );
#ifdef _UNICODE
			int nLen = strValue.GetLength();
			char *pszTmp = new char[nLen * 2];
			WideCharToMultiByte(CP_ACP,0,strValue.GetBuffer(0),-1,pszTmp,nLen*2,NULL,NULL);
            char* lptoken = strtok( pszTmp," \t\n" );		
#else
			char* lptoken = strtok( strValue.GetBuffer(0)," \t\n" );
#endif

            // Read numbers
            while( lptoken)
            {
                *pHighFreq = atoi( lptoken );
                pHighFreq++;

                lptoken = strtok( NULL," \t\n" );
                high_num++;
            }
#ifdef _UNICODE
			if(pszTmp != NULL)
			{
				delete [] pszTmp;
				pszTmp = NULL;
			}
#endif
        }
        else if( 0 == strValue.Find( _T("High_Am") ) )
        {
            // Read high freq
            // Remove letters
            strValue.Delete( 0,7 );
#ifdef _UNICODE
			int nLen = strValue.GetLength();
			char *pszTmp = new char[nLen * 2];
			WideCharToMultiByte(CP_ACP,0,strValue.GetBuffer(0),-1,pszTmp,nLen*2,NULL,NULL);
            char* lptoken = strtok( pszTmp," \t\n" );
#else
			char* lptoken = strtok( strValue.GetBuffer(0)," \t\n" );
#endif
            // Read numbers
            while( lptoken )
            {
                *pHighEq = atoi( lptoken );
                pHighEq++;

                lptoken = strtok( NULL," \t\n" );
            }
#ifdef _UNICODE
			if(pszTmp != NULL)
			{
				delete [] pszTmp;
				pszTmp = NULL;
			}
#endif
            
        }
        else if( 0 == strValue.Find( _T("Low_Freq") ) )
        {
            // Read low freq
            // Remove letters
            strValue.Delete( 0,8 );
#ifdef _UNICODE
			int nLen = strValue.GetLength();
			char *pszTmp = new char[nLen * 2];
			WideCharToMultiByte(CP_ACP,0,strValue.GetBuffer(0),-1,pszTmp,nLen*2,NULL,NULL);
            char* lptoken = strtok( pszTmp," \t\n" );
#else
			char* lptoken = strtok( strValue.GetBuffer(0)," \t\n" );
#endif
            // Read numbers
            while( lptoken )
            {
                *pLowFreq = atoi( lptoken );
                pLowFreq++;

                lptoken = strtok( NULL," \t\n" );
                low_num++;
            }
#ifdef _UNICODE
			if(pszTmp != NULL)
			{
				delete [] pszTmp;
				pszTmp = NULL;
			}
#endif
            
        }
        else if( 0 == strValue.Find( _T("Low_Am") ) )
        {
            // Read low am
            // Remove letters
            strValue.Delete( 0,6 );
#ifdef _UNICODE
			int nLen = strValue.GetLength();
			char *pszTmp = new char[nLen * 2];
			WideCharToMultiByte(CP_ACP,0,strValue.GetBuffer(0),-1,pszTmp,nLen*2,NULL,NULL);
            char* lptoken = strtok( pszTmp," \t\n" );
#else
			char* lptoken = strtok( strValue.GetBuffer(0)," \t\n" );
#endif
            // Read numbers
            while( lptoken )
            {
                *pLowEq = atoi( lptoken );
                pLowEq++;

                lptoken = strtok( NULL," \t\n" );
            } 
#ifdef _UNICODE
			if(pszTmp != NULL)
			{
				delete [] pszTmp;
				pszTmp = NULL;
			}
#endif
        }
    }
    return TRUE;
}

// Load eq file
BOOL LoadEqFile( CString strFile,int* pFrequency,double* pEq,int* pEqNum )
{
    CString strExt = strFile.Right(3);
    if( !strExt.CompareNoCase( _T("trc") ) )
    {
        return LoadTrcFile( strFile,pFrequency,pEq,pEqNum );
    }
    else if( !strExt.CompareNoCase( _T("xls") ) )
    {
        return LoadXlsFile( strFile,pFrequency,pEq,pEqNum );
    }
    else
    {
        AfxMessageBox( IDS_INVALID_FILE_TYPE );
        return FALSE;
    }
}

void CFirAutoDlg::OnButtonFirSpec() 
{
	CString strFile,strFilter = "Spec files(*.spec)|*.spec||";
    if( !BrowseFile( strFile,strFilter ) )
    {
        return;
    }

    m_bSpecReady = LoadSpec( strFile,m_arrHighFreq,m_arrHighEq,m_nHighEqNum,
                                     m_arrLowFreq,m_arrLowEq,m_nLowEqNum);

    m_Graphy.Invalidate();
}

void CFirAutoDlg::OnButtonFirOriginal() 
{
	CString strFile,strFilter = "Eq files(*.trc,*.xls)|*.trc;*.xls||";
    if( !BrowseFile( strFile,strFilter ) )
    {
        return;
    }

    int arrFrequency[MAX_EQ_NUMBER];
    int nEqNum;
    if( LoadEqFile( strFile,arrFrequency,m_arrOriEq,&nEqNum ) )
    {
		//teana hu 2011.02.18
		m_pTempArea->m_strOriFile = strFile;
		m_nLoadOriginalFlag = LOAD_ORG;
		//
        CheckEqData( nEqNum,arrFrequency,FALSE );
		m_bDefaultFir = FALSE;
    }
}

void CFirAutoDlg::OnButtonFirTarget() 
{
    CString strFile,strFilter = "Eq files(*.trc,*.xls)|*.trc;*.xls||";
    if( !BrowseFile( m_strTarEqFile,strFilter ) )
    {
        return;
    }
    
    int arrFrequency[MAX_EQ_NUMBER];
    int nEqNum;
    if( LoadEqFile( m_strTarEqFile,arrFrequency,m_arrTarEq,&nEqNum ) )
    {
        CheckEqData( nEqNum,arrFrequency,TRUE );
    }
}


// The original and target eq number and frequency must be identical,
// We check it here
BOOL CFirAutoDlg::CheckEqData( int nEqNum,int* pFrequency,BOOL bTarget )
{
    BOOL bNeedCompare = FALSE;
    BOOL bOverWrite = FALSE;

    if( bTarget )
    {
        // Input is target        
        if( m_bOriReady )
        {
            bNeedCompare = TRUE;
        }
        else
        {
            bNeedCompare = FALSE;
            bOverWrite = TRUE;
            m_bTarReady = TRUE;
        }
    }
    else
    {
        // Input is original
        if( m_bTarReady )
        {
            bNeedCompare = TRUE;
        }
        else
        {
            bNeedCompare = FALSE;
            bOverWrite = TRUE;
            m_bOriReady = TRUE;
        }
    }

    if( bNeedCompare )
    {
        // Compare
        if( nEqNum == m_nEqNum )
        {
            for( int i=0;i<m_nEqNum;i++ )
            {
                if( pFrequency[i] != m_arrFrequency[i] )
                {
                    bOverWrite = TRUE;
                    break;
                }
            }
        }
        else
        {
            bOverWrite = TRUE;
        }

        if( bOverWrite )
        {
            int nRet = AfxMessageBox( IDS_ORI_TAR_NOT_IDENTICAL,MB_YESNO );
            if( nRet == IDYES )
            {
                bOverWrite = FALSE;
            }
            else
            {
                m_bTarReady = bTarget;
                m_bOriReady = !bTarget;
            }
        }
        else
        {
            m_bOriReady = TRUE;
            m_bTarReady = TRUE;
        }
    }

    if( bOverWrite )
    {
        // Overwrite current data
        m_nEqNum = nEqNum;
        for( int i=0;i<m_nEqNum;i++ )
        {
            m_arrFrequency[i] = pFrequency[i];
        }
    }

    UpdateCtrl();

    return TRUE;
}

void CFirAutoDlg::UpdateCtrl()
{
    GetDlgItem( IDC_BUTTON_FIR_ADJUST )->EnableWindow( m_bOriReady && m_bTarReady );
    m_Graphy.Invalidate();
}

void CFirAutoDlg::OnButtonFirAdjust() 
{
    fireqstate firstate;
    firstate.chip_type = 0; // not d chip
    firstate.fir_taps = 33;
    firstate.frequency_set_length = m_nEqNum;
    firstate.scale_flag = 1; // db
    firstate.bandwidth = m_nSampleRate;

    audio_fir_eq( m_arrFrequency,m_arrOriEq,m_arrTarEq,&firstate,m_arrEq,&m_nGain );

    m_bReady = TRUE;

    UpdateData( FALSE );
    m_Graphy.Invalidate();
}

#define PI 3.1415926

double CalculateEq( int nEqNum,
                    double* pEqData,
                    int nGain,
                    double dFreq
                  )
{
    double dYScale =0.0;
    std::complex<float> total;
    for( int j=0; j<nEqNum; ++j )
    {
        float dAngle = (float)(2.0*PI*dFreq*j/8000.0);
//        float dCos = (float)(cos(dAngle));
//        float dSin = (float)(sin(dAngle));
        
        double deq = pEqData[j] * 4096;
        float dR = (float)(deq * cos(dAngle));
        float dI = (float)(-deq * sin(dAngle));            
        std::complex<float> temp(dR   ,  dI );
        total += temp;            
    }
    struct _complex   totalCType;
    totalCType.x = total.real();
    totalCType.y = total.imag();
    dYScale = _cabs(totalCType);
    
    dYScale = 20* log10(dYScale/4096.0*double(nGain)/4096.0 );   
    return dYScale;
}


void CalculateGraphyData( std::vector<CFrequencyGraphyStaticEx::FreqItem>* vAllFreqItems,
                          int nEqNum,
                          double* pEqData,
                          int nGain
                        )
{
#define IX_SCALE_COUNT 500
    double dXScaleStep = (CFrequencyGraphyStaticEx::m_dMaxXValue - CFrequencyGraphyStaticEx::m_dMinXValue)/IX_SCALE_COUNT;
    
    double dXScale=CFrequencyGraphyStaticEx::m_dMinXValue;
    
    for( int i=0; i<IX_SCALE_COUNT; ++i )
    {        
        CFrequencyGraphyStaticEx::FreqItem tempItem;
        tempItem.first = log10( dXScale );
        tempItem.second = CalculateEq( nEqNum,pEqData,nGain,dXScale );
        vAllFreqItems->push_back( tempItem );
        dXScale += dXScaleStep;
    }
}

void CFirAutoDlg::GetOriEq( std::vector<CFrequencyGraphyStaticEx::FreqItem>* vAllFreqItems )
{
    if( !m_bOriReady )
    {
        // No valid data
        return;
    }

	
	if( m_nLoadOriginalFlag == LOAD_ORG && m_pTempArea->m_bModified == FALSE)
	{
		for( int i=0;i<m_nEqNum;i++ )
		{
			CFrequencyGraphyStaticEx::FreqItem fi;
			fi.first = log10( m_arrFrequency[i] );
			fi.second = m_arrOriEq[i];
			vAllFreqItems->push_back( fi );
		}
	}
	else
	{
		std::vector<SpecialRectOfCoefficient::FreqItem> arrFreq = m_pTempArea->GetFreqItem();
		int nSize = arrFreq.size();
		for( int i=0;i<arrFreq.size();i++ )
		{
			CFrequencyGraphyStaticEx::FreqItem fi;
			fi.first = arrFreq[i].first;
			fi.second = arrFreq[i].second + m_arrOriEq[i];
			vAllFreqItems->push_back( fi );
		}
	}

//	SetListData();

    //CalculateGraphyData( vAllFreqItems,m_nEqNum,m_arrOriEq );
}

void CFirAutoDlg::GetOriEqXValue(double dX[65])
{
	for( int i=0;i<m_nEqNum;i++ )
	{
		dX[i] = m_arrFrequency[i];
	}
}
void CFirAutoDlg::GetTarEq( std::vector<CFrequencyGraphyStaticEx::FreqItem>* vAllFreqItems )
{
    if( !m_bTarReady )
    {
        // No valid data
        return;
    }
    
    for( int i=0;i<m_nEqNum;i++ )
    {
        CFrequencyGraphyStaticEx::FreqItem fi;
        fi.first = log10( m_arrFrequency[i] );
        fi.second = m_arrTarEq[i];
        vAllFreqItems->push_back( fi );
    }
//    CalculateGraphyData( vAllFreqItems,m_nEqNum,m_arrTarEq );
}

void CFirAutoDlg::GetRealEq( std::vector<CFrequencyGraphyStaticEx::FreqItem>* vAllFreqItems )
{
    if( !m_bReady )
    {
        // No valid data;
        return;
    }
    double eq[33];
    for( int i=0;i<33;i++ )
    {
        eq[i] = m_arrEq[i]/4096.0;
    }
    CalculateGraphyData( vAllFreqItems,33,eq,m_nGain );
}


void CFirAutoDlg::OnCheckOriginal() 
{
	m_bShowOri = !m_bShowOri;
    m_Graphy.Invalidate();
}

void CFirAutoDlg::OnCheckResult() 
{
	m_bShowReal = !m_bShowReal;
    m_Graphy.Invalidate();
}

void CFirAutoDlg::OnCheckTarget() 
{
	m_bShowTar = !m_bShowTar;
    m_Graphy.Invalidate();
}

HBRUSH CFirAutoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
    if( pWnd->GetDlgCtrlID() == IDC_STATIC_ORIGINAL )
    {
        return CreateSolidBrush( RGB( 0,127,0 ) );
    }
    
    if( pWnd->GetDlgCtrlID() == IDC_STATIC_TARGET )
    {
        return CreateSolidBrush( RGB( 255,255,0 ) );
    }
    
    if( pWnd->GetDlgCtrlID() == IDC_STATIC_REAL )
    {
        return CreateSolidBrush( RGB( 0,0,255 ) );
    }

    if( pWnd->GetDlgCtrlID() == IDC_STATIC_MODIFIED )
    {
        return CreateSolidBrush( RGB( 127,0,0 ) );
    }
    
    if( pWnd->GetDlgCtrlID() == IDC_STATIC_SPEC )
    {
        return CreateSolidBrush( RGB( 255,0,255 ) );
    }

    if( pWnd->GetDlgCtrlID() == IDC_STATIC_FINAL )
    {
        return CreateSolidBrush( RGB( 0,0,127 ) );
    }
    
    return hbr;
}

void CFirAutoDlg::OnCheckModified() 
{
    m_bShowModified = !m_bShowModified;
    m_Graphy.Invalidate();
}

void CFirAutoDlg::GetModifiedEq( std::vector<CFrequencyGraphyStaticEx::FreqItem>* vAllFreqItems )
{

    double eq[33];
    for( int i=0;i<33;i++ )
    {
        eq[i] = m_arrEq[i]/4096.0;
    }
    
    for( i=0;i<m_nEqNum;i++ )
    {
        CFrequencyGraphyStaticEx::FreqItem fi;
        fi.first = log10( m_arrFrequency[i] );
        fi.second = m_arrOriEq[i] + CalculateEq( 33,eq,m_nGain,m_arrFrequency[i] );
        vAllFreqItems->push_back( fi );
    }    
}

void CFirAutoDlg::OnCheckSpec() 
{
    m_bShowSpec = !m_bShowSpec;
    m_Graphy.Invalidate();
}

void CFirAutoDlg::GetHighEq( std::vector<CFrequencyGraphyStaticEx::FreqItem>* vAllFreqItems )
{
	for( int i=0;i<m_nHighEqNum;i++ )
	{
		CFrequencyGraphyStaticEx::FreqItem fi;
		fi.first = log10( m_arrHighFreq[i] );
		fi.second = m_arrHighEq[i];
		vAllFreqItems->push_back( fi );
	}  
}

void CFirAutoDlg::GetLowEq( std::vector<CFrequencyGraphyStaticEx::FreqItem>* vAllFreqItems )
{
    for( int i=0;i<m_nLowEqNum;i++ )
    {
        CFrequencyGraphyStaticEx::FreqItem fi;
        fi.first = log10( m_arrLowFreq[i] );
        fi.second = m_arrLowEq[i];
        vAllFreqItems->push_back( fi );
    }    
}

void CFirAutoDlg::GetFinalEq( std::vector<CFrequencyGraphyStaticEx::FreqItem>* vAllFreqItems )
{
    if( !m_bReady )
    {
        // No valid data;
        return;
    }
    
    GetRealEq( vAllFreqItems );

    for( UINT i=0;i<vAllFreqItems->size();i++ )
    {
        vAllFreqItems->at(i).second += m_dOffset;
    }
}

void CFirAutoDlg::OnOK() 
{
	CDialog::OnOK();

    double dOffset = pow( 10,m_dOffset / 20 );
    for( int i=0;i<m_nEqNum;i++ )
    {
        m_arrEq[i] = (int)floor( (int)(dOffset * m_arrEq[i]) );
    }
	if(!m_bDefaultFir && m_Drag.m_nSel != -1)
	{
		m_pTempArea->m_bModified = TRUE;
		m_pTempArea->CalculateCoefficients();
	}
	
	*m_pArea = *m_pTempArea;	
}

void CFirAutoDlg::OnCheckFinal() 
{
    m_bShowFinal = !m_bShowFinal;
    m_Graphy.Invalidate();
}

void CFirAutoDlg::OnButtonOffsetApply() 
{
    UpdateData();
    Invalidate();    
}

// Load default spec and target file if they exist
void CFirAutoDlg::LoadDefault()
{
    extern int gnFirType;
    char szTargetFile[30],szSpecFile[30];

    if( gnFirType == SpecialRect::dl_fir_eq_coeff )
    {
        strcpy( szSpecFile,"\\GSM_handset_RFR_SPEC.spec" );
        strcpy( szTargetFile,"\\RFR target.xls" );
    }
    else if( gnFirType == SpecialRect::ul_fir_eq_coeff )
    {
        strcpy( szSpecFile,"\\GSM_handset_SFR_SPEC.spec" );
        strcpy( szTargetFile,"\\SFR target.xls" );
    }
    else
    {
        // ST fir do not load default file
        return;
    }

    // Get exe file path
    TCHAR szName[_MAX_PATH];
    DWORD dwRet = ::GetModuleFileName(AfxGetApp()->m_hInstance,szName,_MAX_PATH);
    if(dwRet == 0)
    {
        szName[0] = 0;
        return;
    }
    
    TCHAR* pResult = _tcsrchr(szName,'\\');
    if(pResult == NULL)
    {
        szName[0] = 0;
        return;
    }
    
    *pResult = 0;

    // Get default spec file path name
    TCHAR szFile[_MAX_PATH];
    _tcscpy( szFile,szName );
#ifdef _UNICODE
	TCHAR szTmp[MAX_PATH] = {0};
	MultiByteToWideChar(CP_ACP,0,szSpecFile,-1,szTmp,MAX_PATH+1);
    _tcscat( szFile,szTmp );
#else
    strcat( szFile,szSpecFile );
#endif

    CFileFind ff;
    if( ff.FindFile( szFile ) )
    {
		CString strFile(szFile);
        m_bSpecReady = LoadSpec( strFile,m_arrHighFreq,m_arrHighEq,m_nHighEqNum,
                             m_arrLowFreq,m_arrLowEq,m_nLowEqNum);
    }
    ff.Close();
    
	if( m_bAuto )
	{
		// Only auto adjust need target
		// Get default target file path name		
		_tcscpy( szFile,szName );
#ifdef _UNICODE
		ZeroMemory(szTmp, MAX_PATH);
		MultiByteToWideChar(CP_ACP,0,szTargetFile,-1,szTmp,MAX_PATH+1);
		_tcscat( szFile,szTmp);
#else
		strcat( szFile,szTargetFile );
#endif
		
		if( ff.FindFile( szFile ) )
		{
			int arrFrequency[MAX_EQ_NUMBER];
			int nEqNum;
			if( LoadEqFile( szFile,arrFrequency,m_arrTarEq,&nEqNum ) )
			{
				CheckEqData( nEqNum,arrFrequency,TRUE );
			}
		}
		ff.Close();
	}
}

BOOL CFirAutoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if( m_bAuto )
	{
		SetWindowText(_T("Fir auto adjust"));
		m_nLoadOriginalFlag = LOAD_ORG;
		// Hide m_Drag and extend m_Graphy
		RECT rcDrag,rcGraph;
		CWnd* pDrag = GetDlgItem( IDC_STATIC_FIR_DRAG );
		CWnd* pGraph = GetDlgItem( IDC_STATIC_FIR_AUTO_GRAPHY ); 
		pDrag->GetWindowRect( &rcDrag );
		pGraph->GetWindowRect( &rcGraph );
		rcGraph.top = rcDrag.top;
		ScreenToClient( &rcGraph );
		pGraph->MoveWindow( &rcGraph );
		pDrag->ShowWindow( SW_HIDE );
		GetDlgItem(IDC_LIST_FIR_VALUE)->ShowWindow(SW_HIDE);
	}
	else
	{
		//SetWindowText(_T("Fir adjust"));
		GetDlgItem( IDC_BUTTON_FIR_TARGET )->EnableWindow( FALSE );
//		GetDlgItem( IDC_BUTTON_FIR_ORIGINAL )->EnableWindow( FALSE );
		GetDlgItem( IDC_CHECK_SPEC )->EnableWindow( FALSE );
		GetDlgItem( IDC_CHECK_ORIGINAL )->EnableWindow( FALSE );
		GetDlgItem( IDC_CHECK_TARGET )->EnableWindow( FALSE );
		GetDlgItem( IDC_CHECK_RESULT )->EnableWindow( FALSE );
		GetDlgItem( IDC_CHECK_MODIFIED )->EnableWindow( FALSE );
		GetDlgItem( IDC_CHECK_FINAL )->EnableWindow( FALSE );
		//teana hu 2011.02.17
		GetDlgItem(IDC_BUTTON_OFFSET_APPLY)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FIR_OFFSET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_FIR_TARGET)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_FIR_ADJUST)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_TARGET)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_RESULT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_MODIFIED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FIR_OFFSET)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_FIR_OFFSET)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_OFFSET_APPLY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_FINAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_EQ_FIR_GAIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_EQ_FIR_GAIN_AUTO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TARGET)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_REAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MODIFIED)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FINAL)->ShowWindow(SW_HIDE);
		//

		m_pTempArea = new SpecialRectOfCoefficient;
		*m_pTempArea = *m_pArea;

		for( int i=0;i<X_SCALE_COUNT_FIR;i++ )
		{
// 			if(m_pArea->m_bModified)
 			{
				m_Drag.m_arrPowerData[i] = m_pArea->m_vPo.m_vPoValue[i];
 			}
//  			else
// 			{
//  				m_Drag.m_arrPowerData[i] = 0;
//  			}		
		}

		
		m_bOriReady = TRUE;
		m_Drag.m_pRelateWnd = &m_Graphy;

		if(m_pTempArea->m_strName == _T("DL_FIR_EQ_COEFF"))
		{
			SetWindowText(_T("DL Fir adjust"));
		}
		else if(m_pTempArea->m_strName == _T("UL_FIR_EQ_COEFF"))
		{
			SetWindowText(_T("UL Fir adjust"));
		}

		SetListCtrl();
		SetListData();

		m_bDefaultFir = FALSE;
		//load last ori file 
		if(m_pTempArea->m_bModified && !m_pTempArea->m_strOriFile.IsEmpty())
		{
			int arrFrequency[MAX_EQ_NUMBER];
			int nEqNum;
			if( LoadEqFile( m_pTempArea->m_strOriFile,arrFrequency,m_arrOriEq,&nEqNum ) )
			{
				m_nLoadOriginalFlag = LOAD_ORG;
				CheckEqData( nEqNum,arrFrequency,FALSE );
			}
		}
		GetConfig();
	}
	//teana hu 2011.03.02
	if(!m_bVisibleFirButton)
	{
		GetDlgItem(IDC_BUTTON_DEFAULT_FIR)->ShowWindow(SW_HIDE);
	}
    // Load default spec and target file
    LoadDefault();
	m_Drag.m_pDataSource = m_pTempArea;
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFirAutoDlg::ResetTarEq(int nIndex, double dTarEq)
{
	m_arrTarEq[nIndex] = dTarEq;
}

int	CFirAutoDlg::GetHighEQYValue(int x)
{
	for( int i=0;i<m_nHighEqNum;i++ )
	{
		if(log10( m_arrHighFreq[i] ) <= x && log(m_arrHighFreq[i+1]) >= x)
		{
			int nValue = m_arrHighEq[i] > m_arrHighEq[i+1] ? m_arrHighEq[i] : m_arrHighEq[i+1];
			return nValue;
		}
	}

	return INVALID_VALUE;
}

void CFirAutoDlg::ModifySpecFreqValue(int nDiff)
{
	for( int i=0;i<m_nHighEqNum;i++ )
	{
		m_arrHighEq[i] += nDiff;
	} 

	for( i=0;i<m_nLowEqNum;i++ )
    {
		m_arrLowEq[i] += nDiff;
    } 

	
}
void CFirAutoDlg::OnButtonDefaultFir() 
{
	// TODO: Add your control notification handler code here
	//AfxMessageBox(_T("If you want load original data from ACQUA, Please click <Default Fir> button, clear FIR parameters first, then test RFR or SFR case."));
	m_pTempArea->m_bModified = FALSE;
	m_nLoadOriginalFlag = NO_LOAD_ORG;
	m_bDefaultFir = TRUE;

	for( int i=0;i<X_SCALE_COUNT_FIR;i++ )
	{
		m_pTempArea->m_vPo.m_vPoValue[i] = 0;
		m_Drag.m_arrPowerData[i] = m_pTempArea->m_vPo.m_vPoValue[i];	
	}
	
	SetListData();
	m_Drag.Invalidate();
	m_Graphy.Invalidate();
	
	CAudioTesterSheet* pSheet = (CAudioTesterSheet*)AfxGetMainWnd();
	if(pSheet == NULL)
	{
		return;
	}

	AUDIO_STRUCT value;
	memset(&value, 0, sizeof(AUDIO_STRUCT));
	
	if(m_pTempArea->m_strName == _T("DL_FIR_EQ_COEFF"))
	{
		pSheet->WriteDefaultFirToPhone( pSheet->GetActiveIndex() - 1 ); 
		for( int i=0;i<16;i++ )
		{
			value.dl_fir_eq_coeff[i] = 0;
		}
		value.dl_fir_eq_coeff[16] = 4096;

		m_pTempArea->SetValue(value);
	}
	else if(m_pTempArea->m_strName == _T("UL_FIR_EQ_COEFF"))
	{
		pSheet->WriteDefaultUlFirToPhone( pSheet->GetActiveIndex() - 1 );
		for( int i=0;i<33;i++ )
		{
			value.ul_fir_eq_coeff[i] = 0;
		}
		value.ul_fir_eq_coeff[16] = 4096;
		m_pTempArea->SetValue(value);
	}
	
}

void CFirAutoDlg::OnButtonRefreshFir()
{
	if(m_bDefaultFir)
	{
		return;
	}
	m_pTempArea->CalculatePowerData();
	for( int i=0;i<X_SCALE_COUNT_FIR;i++ )
	{
		m_Drag.m_arrPowerData[i] = m_pTempArea->m_vPo.m_vPoValue[i];
	}
	m_Drag.Invalidate();
	SetListData();
}

void CFirAutoDlg::GetConfig()
{
	TCHAR szCfgPathName[MAX_PATH];
	::GetModuleFileName( NULL, szCfgPathName, _MAX_PATH );
	LPTSTR pExt = _tcsrchr(szCfgPathName, _T('.'));
	_tcscpy(pExt + 1, _T("ini"));

	CString strText;
	int nIndex = 0;
	int nMin = -5;
	int nMax = 5;

	_TCHAR szText[128] = {0};

	GetPrivateProfileString( _T("Settings"), _T("FirParamRange"), _T("[-5,5]"), szText, MAX_PATH, szCfgPathName );
	strText = szText;
	nIndex = strText.Find(_T("["));
	if(nIndex != -1)
	{
		strText.Delete(0, nIndex +1);
		nIndex = strText.Find(_T(","));
		if(nIndex != -1)
		{
#ifdef _UNICODE
			swscanf(strText.Left(nIndex),_T("%d"),&nMin);
#else
			sscanf((LPSTR)(LPCTSTR)strText.Left(nIndex), "%d", &nMin);
#endif
			strText.Delete(0, nIndex + 1);
			nIndex = strText.Find(_T("]"));
			if(nIndex != -1)
			{
#ifdef _UNICODE
				swscanf(strText.Left(nIndex),_T("%d"),&nMax);
#else
				sscanf((LPSTR)(LPCTSTR)strText.Left(nIndex), "%d", &nMax);
#endif
			}
		}
	}

	m_Drag.SetYMinMaxValue(nMin, nMax);

	nMin = -30;
	nMax = 30;
	GetPrivateProfileString( _T("Settings"), _T("RFR_Fir_Range"), _T("[-30,30]"), szText, MAX_PATH, szCfgPathName );

	strText = szText;
	nIndex = strText.Find(_T("["));
	if(nIndex != -1)
	{
		strText.Delete(0, nIndex +1);
		nIndex = strText.Find(_T(","));
		if(nIndex != -1)
		{
#ifdef _UNICODE
			swscanf(strText.Left(nIndex),_T("%d"),&nMin);
#else
			sscanf((LPSTR)(LPCTSTR)strText.Left(nIndex), "%d", &nMin);
#endif
			strText.Delete(0, nIndex + 1);
			nIndex = strText.Find(_T("]"));
			if(nIndex != -1)
			{
#ifdef _UNICODE
				swscanf(strText.Left(nIndex),_T("%d"),&nMax);
#else
				sscanf((LPSTR)(LPCTSTR)strText.Left(nIndex), "%d", &nMax);
#endif
			}
		}
	}

	if(m_pTempArea->m_strName == _T("DL_FIR_EQ_COEFF"))
	{
		m_Graphy.SetYMinMaxValue(nMin, nMax);
	}

	nMin = -50;
	nMax = 10;
	GetPrivateProfileString( _T("Settings"), _T("SFR_Fir_Range"), _T("[-50,10]"), szText, MAX_PATH, szCfgPathName );

	strText = szText;
	nIndex = strText.Find(_T("["));
	if(nIndex != -1)
	{
		strText.Delete(0, nIndex +1);
		nIndex = strText.Find(_T(","));
		if(nIndex != -1)
		{
#ifdef _UNICODE
			swscanf(strText.Left(nIndex),_T("%d"),&nMin);
#else
			sscanf((LPSTR)(LPCTSTR)strText.Left(nIndex), "%d", &nMin);
#endif
			strText.Delete(0, nIndex + 1);
			nIndex = strText.Find(_T("]"));
			if(nIndex != -1)
			{
#ifdef _UNICODE
				swscanf(strText.Left(nIndex),_T("%d"),&nMax);
#else
				sscanf((LPSTR)(LPCTSTR)strText.Left(nIndex), "%d", &nMax);
#endif
			}
		}
	}

	if(m_pTempArea->m_strName == _T("UL_FIR_EQ_COEFF"))
	{
		m_Graphy.SetYMinMaxValue(nMin, nMax);
	}
}
/*
BOOL CFirAutoDlg::LoadOriFile(CString strFile,int* pFrequency,double* pEq,int* pEqNum )
{
	BOOL bRet = LoadEqFile( strFile, pFrequency, pEq,pEqNum );
	if(bRet)
	{
		double dX[65] = {0};
		GetOriEqXValue( dX );
		m_pTempArea->CalculateFreqGraphyDataEx(dX,65);

		std::vector<SpecialRectOfCoefficient::FreqItem> arrFreq = m_pTempArea->GetFreqItem();
		int nCount = arrFreq.size();
		int nSize = *pEqNum;
		for( int i=0;i<nSize;i++ )
		{
			pEq[i] -= arrFreq[i].second;
		}
	}

	return bRet;
}
*/
void CFirAutoDlg::SetListCtrl()
{
	
    DWORD dwStyle = m_listFir.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES ;    
    dwStyle &= ~LVS_EX_HEADERDRAGDROP;
    m_listFir.SetExtendedStyle( dwStyle );
    
    
    LV_COLUMN	lvc;
    lvc.mask = LVCF_WIDTH | LVCF_TEXT ;
    char columnLabel[2][128];
    strcpy( columnLabel[0], "Po" );	
    strcpy( columnLabel[1], "Value" );				
    
    int DefaultColumnWidth[] = { 140,55};
    for( int i=0; i<2; i++ )
    {
#ifdef _UNICODE
		TCHAR szTmp[128] = {0};
		MultiByteToWideChar(CP_ACP,0,columnLabel[i],-1,szTmp,128);
        lvc.pszText = szTmp;
#else
		lvc.pszText = columnLabel[i];
#endif
        lvc.cx = DefaultColumnWidth[i];
        lvc.fmt = 0;   
        
        m_listFir.InsertColumn( i, &lvc );
    }

	for(i = 0; i < ROW_COUNT; i++)
	{
		CString strName;
		strName.Format(_T("Po[%d] (%d)"), i, m_vPowerFrequence[i]);
		m_listFir.InsertItem(i+1, strName);
	}
	
}

void CFirAutoDlg::SetListData()
{
    for( int i=0; i<ROW_COUNT; ++i )
    {
		CString strText;
		strText.Format(_T("%.1f"), m_pTempArea->m_vPo.m_vPoValue[i]);
		m_listFir.SetItemText(i, 1, strText);
    }    
	UpdateData(FALSE);
}

void CFirAutoDlg::SetFirListHighLight(int nItem)
{
	if(nItem == -1)
	{
		return;
	}
	POSITION pos = m_listFir.GetFirstSelectedItemPosition();
    while(NULL != pos)
    {
        int nIndex = m_listFir.GetNextSelectedItem(pos);
        m_listFir.SetItemState(nIndex, 0, LVIS_SELECTED);
    }
    m_listFir.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
	
}