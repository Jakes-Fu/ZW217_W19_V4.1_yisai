// DlgEqCoefs.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgEqCoefs.h"
#include "AudioFileOpenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEqCoefs dialog


CDlgEqCoefs::CDlgEqCoefs(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEqCoefs::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEqCoefs)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pItem = NULL;
	m_nItemCount = -1;
}


void CDlgEqCoefs::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEqCoefs)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_LIST_PEQ_COEFS, m_lst);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEqCoefs, CDialog)
	//{{AFX_MSG_MAP(CDlgEqCoefs)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnButtonImport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEqCoefs message handlers

BOOL CDlgEqCoefs::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//teana hu 2012.01.16
	GetDlgItem(IDC_BUTTON_IMPORT)->ShowWindow(SW_HIDE);
	//

	SetWindowText(m_strTitle);
	if(m_strTitle.Find(_T("Noise_Sensitivity")) != -1)
	{
		InitList(_T("Volume level %d"));
	}
	else if(m_strTitle.Find(_T("CompressorCurveInputLevels")) != -1)
	{
		InitList(_T("CompressorCurveInputLevels[%d]"));
	}
	else if(m_strTitle.Find(_T("CompressorCurveOutputLevels")) != -1)
	{
		InitList(_T("CompressorCurveOutputLevels[%d]"));
	}
	else if(m_strTitle.Find(_T("FSB_init_Table0")) != -1)
	{
		InitList(_T("FSB_init_Table0[%d]"));
	}
	else if(m_strTitle.Find(_T("FSB_init_Table1")) != -1)
	{
		InitList(_T("FSB_init_Table1[%d]"));
	}
	else
	{
		InitList(_T("EqCoef[%d]"));
		//teana hu 2012.01.16
		GetDlgItem(IDC_BUTTON_IMPORT)->ShowWindow(SW_SHOW);
		//
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEqCoefs::SetItemValue(short *pItem, int nItemCount)
{
	m_nItemCount = nItemCount;
	if(m_pItem == NULL && m_nItemCount != -1)
	{
		m_pItem = new short[m_nItemCount];
	}

	memcpy(m_pItem, pItem, nItemCount * sizeof(short));
}


void CDlgEqCoefs::InitList(CString strItemName)
{
	// Set style
	DWORD dwStyle = m_lst.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES;    
    dwStyle &= ~LVS_EX_HEADERDRAGDROP;
    m_lst.SetExtendedStyle( dwStyle );

	// Set Data range
	if(m_strTitle == _T("Noise_Sensitivity"))
	{
		m_lst.SetEditCheckRange(TRUE, 32767, -6709 );
	}
	else if(m_strTitle == _T("CompressorCurveInputLevels"))
	{
		m_lst.SetEditCheckRange(TRUE, 0, -96 );
	}
	else if(m_strTitle == _T("CompressorCurveOutputLevels"))
	{
		m_lst.SetEditCheckRange(TRUE, 0, -96 );
	}
	else if(m_strTitle == _T("FSB_init_Table0"))
	{
		m_lst.SetEditCheckRange(TRUE, 65535, 0 );
	}
	else if(m_strTitle == _T("FSB_init_Table1"))
	{
		m_lst.SetEditCheckRange(TRUE, 65535, 0 );
	}
	m_lst.SetInplaceEditType( CInPlaceEdit::FLOAT_NUMBER,5 );

	// Insert columns
    LV_COLUMN	lvc;
    lvc.mask = LVCF_WIDTH | LVCF_TEXT ;
	const int lstColumnNum = 2;
	//
    char columnLabel[lstColumnNum][128];
    strcpy( columnLabel[0], "Item" );	
    strcpy( columnLabel[1], "Value" );		

	// Set column styles
	int DefaultColumnWidth[] = { 140,135};
	for( int i=0; i<lstColumnNum; i++ )
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
		
		m_lst.InsertColumn( i, &lvc );
	} 
	
	m_vAllLinesData.clear();
	CString strFormat;

    for(i=0; i<m_nItemCount; ++i )
    {
        ROW_DATA singleLineData; 
        CListCtrlEx::COLUMN_DATA columnData;    

        // row = i, column = 0;
        columnData.m_enShowType = CListCtrlEx::NOT_EDIT;            
        columnData.m_strShowString.Format(strItemName,i+1 );
        columnData.m_lValue = -1;
        columnData.m_dReserved = 0.0;
        columnData.m_nSpinLow=-1;
        columnData.m_nSpinHigh = -1;
        columnData.m_bShowTitle = FALSE;
        columnData.m_enumEditType = CInPlaceEdit::DEFAULT;
        singleLineData.push_back( columnData );

		// row = i, column = 1;
        columnData.m_enShowType = CListCtrlEx::ONLY_EDIT_TYPE;            
        columnData.m_strShowString.Format(_T("%d"),m_pItem[i] );
        columnData.m_lValue = m_pItem[i];
        columnData.m_dReserved = 0.0;
        columnData.m_nSpinLow=0;
        columnData.m_nSpinHigh = 32767;
        columnData.m_bShowTitle = FALSE;
        columnData.m_enumEditType = CInPlaceEdit::DEFAULT;
        singleLineData.push_back( columnData );

		m_vAllLinesData.push_back( singleLineData );
    }   
	
	for( i=0; i<(signed)m_vAllLinesData.size(); ++i )
    {        
        int nColumnCount = m_vAllLinesData[i].size();
        m_lst.InsertItem( i,_T("") );
        for( int j=0; j<nColumnCount; ++j )
        {            
            m_lst.SetItemText( i, j, m_vAllLinesData[i].at(j).m_strShowString );
        }        
    }
}

void CDlgEqCoefs::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(m_pItem != NULL)
	{
		delete [] m_pItem;
		m_pItem = NULL;
	}
	
	CDialog::OnClose();
}

LRESULT CDlgEqCoefs::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( message == WM_LISTEX_COLLUM_INDEX )
	{
        int nRow = m_lst.GetCurSel();
        int nCol = m_lst.GetCurSelCol();
        int nSpinLow = m_vAllLinesData[nRow].at(nCol).m_nSpinLow;
        int nSpinHigh = m_vAllLinesData[nRow].at(nCol).m_nSpinHigh;        
        
        m_lst.SetShowType( m_vAllLinesData[nRow].at( nCol).m_enShowType );
        if( m_vAllLinesData[nRow].at( nCol).m_bShowTitle )
        {
            CString sNum;
            
            CString sTitle(m_vAllLinesData[nRow].at( nCol ).m_strShowString);
            if( CInPlaceEdit::HEX_NUMBER == m_vAllLinesData[nRow].at( nCol ).m_enumEditType )
            {
                sNum.Format(_T("%d"),m_vAllLinesData[nRow].at( nCol ).m_lValue);
            }
            else
            {
                sNum.Format(_T("%d"),m_vAllLinesData[nRow].at( nCol ).m_lValue);                
            }            
            sTitle.Replace(sNum,_T(""));
            m_lst.SetSpinTitle( sTitle );
        }    
        else
        {
            m_lst.SetSpinTitle("");
        }
        
        if( CListCtrlEx::COMBOX_TYPE ==  m_vAllLinesData[nRow].at( nCol ).m_enShowType )
        {
            m_lst.SetComBoxStrings( m_vAllLinesData[nRow].at( nCol ).m_vComboxStrings );
        }
        // set spin range of every cell
        if( -1 != nSpinHigh && -1 != nSpinLow )
            m_lst.SetSpinRange( nSpinLow, nSpinHigh );
        return TRUE;
    }
    else if( message == WM_INPLACE_KILL_FOCUS )
    {
        CString* pStrValue = (CString*)lParam;
        int nRow = m_lst.GetCurSel();
        int nCol = m_lst.GetCurSelCol();               
        if( nRow != -1 && nCol != -1 )
        {
			short nValue = 0.0;
#ifdef _UNICODE
			char szText[32] = {0};
			WideCharToMultiByte(CP_ACP,0,*pStrValue,-1,szText,32,NULL,NULL);
			nValue = atoi( szText );
#else
			nValue = atoi( (LPSTR)(LPCTSTR)*pStrValue );
#endif
			
            m_vAllLinesData[ nRow ].at(nCol).m_strShowString.Format(_T("%d"), nValue);
            m_vAllLinesData[ nRow ].at(nCol).m_lValue  = nValue;
			
			CString strValue;
			strValue.Format( _T("%d"),nValue );
			*pStrValue = strValue;
			m_lst.SetItemText( nRow,nCol,strValue );
        }
        return TRUE;
    }
    else if ( message == WM_INPLACE_EDIT )
    {
        LPCOMBOX_DATA pData = (LPCOMBOX_DATA)lParam;
        int nRow = m_lst.GetCurSel();
        int nCol = m_lst.GetCurSelCol();      
        ASSERT( nRow >=0 && nRow < (int)m_vAllLinesData.size() );
        
        m_vAllLinesData[nRow].at( nCol ).m_lValue = pData->m_nSelectIdx;
        m_vAllLinesData[nRow].at( nCol ).m_strShowString 
            = m_vAllLinesData[nRow].at( nCol ).m_vComboxStrings[ pData->m_nSelectIdx ];   
        CString sTest(m_vAllLinesData[ nRow ].at(nCol).m_vComboxStrings[pData->m_nSelectIdx]);
        m_vAllLinesData[ nRow ].at(nCol).m_strShowString
            = m_vAllLinesData[ nRow ].at(nCol).m_vComboxStrings[pData->m_nSelectIdx];
        return TRUE;
		
    }	
	
	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CDlgEqCoefs::OnOK() 
{
	// TODO: Add extra validation here
	for(int i = 0; i < m_nItemCount; i++)
	{
		m_pItem[i] = m_vAllLinesData[i].at(1).m_lValue;
	}
	
	CDialog::OnOK();
}

void CDlgEqCoefs::GetItemValue(short *pItem)
{
	memcpy(pItem, m_pItem, m_nItemCount*sizeof(short));
}

void CDlgEqCoefs::OnButtonImport() 
{
	// TODO: Add your control notification handler code here
    CAudioFileOpenDlg dlg(TRUE,_T(".coef"),NULL,OFN_HIDEREADONLY,_T("coef files (*.coef]|*.coef|"),this);
	
    if( IDOK == dlg.DoModal() )
    {
        CWaitCursor temp;
        CString strFileName = dlg.GetFileName(); 	
		
		FILE *fpRead = NULL;
#ifdef _UNICODE
		fpRead = _wfopen(strFileName, _T("r"));
#else
		fpRead = fopen(strFileName, "r");
#endif		
		if(fpRead != NULL)
		{
			char szBuff[128] = {0};
			fgets(szBuff, 128, fpRead); //skip first line
			int nIndex = 0;
			while(fgets(szBuff, 128, fpRead) != NULL)
			{
				int nValue = atoi(szBuff);
				m_vAllLinesData[nIndex].at( 1 ).m_lValue = nValue;
				CString strText;
				strText.Format(_T("%d"), nValue);
				m_lst.SetItemText(nIndex, 1, strText);
				nIndex ++;

			}
		}	
		
	}
}
