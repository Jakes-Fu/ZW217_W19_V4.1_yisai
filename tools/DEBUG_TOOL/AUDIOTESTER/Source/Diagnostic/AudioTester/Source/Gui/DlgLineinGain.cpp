// DlgLineinGain.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgLineinGain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//GOL/R Table
//客户可选值 
float gol_dB_array[32] ={4.5,4.0,3.5,3.0,2.5,2.0,1.5,1.0,0.5,-0.5,-1.5,-2.5,-3.5,-4.5,-5.5,-6.5,-7.5,-8.5,-9.5,-10.5,-11.5,-13.5,-15.5,-17.5,-19.5,-21.5,-23.5,-25.5,-27.5,-29.5,-31.5,-33.5};

// nv 设定值 
short gol_value_array[32]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};


//GOBL/R Table
//客户可选值 
float gobl_dB_array[32]={6.0,4.5,3.0,1.5,0.0,-1.5,-3.0,-4.5,-6.0,-7.5,-9.0,-10.5,-12.0,-13.5,-15.0,-16.5,-18.0,-19.5,-21.0,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5};

// nv 设定值 
short gobl_value_array[32]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

//for 6530
//客户可选值 
float gobl_dB_6530_array[16]={-6,-3,0,3,6,9,12,15,18,21,24,27,30,33,36,39};

// nv 设定值 
short gobl_value_6530_array[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};


//客户可选值 
float gol_dB_6530_handsfree_array[16]={-255, -24,-21,-18,-15,-12,-9,-6,-3,0,3,6,9,12,15,18};
float gol_dB_6530_headset_array[16]={-255, -33, -30, -27, -24,-21,-18,-15,-12,-9,-6,-3,0,3,6,9};

// nv 设定值 
short gol_value_6530_array[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};


//客户可选值 
float gol_dB_smartphone_array[32]={6,4.5,3,1.5,0,-1.5,-3,-4.5,-6,-7.5,-9,-10.5,-12,-13.5,-15,-16.5,-18,-19.5,-21,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5};
// nv 设定值 
short gol_value_smartphone_array[32]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

//客户可选值 
float gobl_dB_smartphone_array[32]={6,4.5,3,1.5,0,-1.5,-3,-4.5,-6,-7.5,-9,-10.5,-12,-13.5,-15,-16.5,-18,-19.5,-21,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5,-22.5};
// nv 设定值 
short gobl_value_smartphone_array[32]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

//inner PA
const short pa_value_handsfree_array[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
//for smartphone
const float pa_dB_handsfree_array[16]={-255,-21,-18,-15,-12,-9,-6,-3,0,3,6,9,12,15,18,21};//-255 = mute

/////////////////////////////////////////////////////////////////////////////
// CDlgLineinGain dialog


CDlgLineinGain::CDlgLineinGain(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLineinGain::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLineinGain)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	memset(&m_volume_data, 0, sizeof(VOLUME_STRUCT));
	m_bDspCodecInfo = FALSE;
	m_bSmartPhone = FALSE;
	m_nLstLevel = 15;
}


void CDlgLineinGain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLineinGain)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_LIST_LINEIN_GAIN, m_lstVolume);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLineinGain, CDialog)
	//{{AFX_MSG_MAP(CDlgLineinGain)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLineinGain message handlers

BOOL CDlgLineinGain::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitListCtrl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLineinGain::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData(TRUE))
    {
        TRACE0("UpdateData failed during dialog termination.\n");
        // the UpdateData routine will set focus to correct item
        return;
    }
	
	int nValue = 0;
	for( int i=0;i<m_nLstLevel;i++ )
	{
		int nIndexGOL = (int)(m_vAllLinesData[i].at( 1 ).m_lValue);
		int nIndexGOBL = (int)(m_vAllLinesData[i].at( 2 ).m_lValue);

		//teana hu 2012.02.02
		if(m_bDspCodecInfo)
		{
			nValue = gobl_value_6530_array[nIndexGOBL] << 4 | gol_value_6530_array[nIndexGOL];
			m_volume_data.reserve[i+18] &= 0xff00; 
			m_volume_data.reserve[i+18] |= nValue;
		}
		else if(m_bSmartPhone)
		{
			nValue = (gobl_value_smartphone_array[nIndexGOBL] << 8) | gol_value_smartphone_array[nIndexGOL];
			m_volume_data.reserve[41] = nValue;
			m_volume_data.reserve[42] = nValue;

			//teana hu 2012.06.12
			if(m_strTitle == _T("Handsfree") && (m_volume_data.reserve[44] & 0xff) == 1)
			{
				int nIndexPA = (int)(m_vAllLinesData[i].at( 3 ).m_lValue);
				nValue = pa_value_handsfree_array[nIndexPA];
				m_volume_data.reserve[45] &= 0x00ff;
				m_volume_data.reserve[45] |= (nValue << 8);
			}
			//
		}
		else
		{
			nValue = (gol_value_array[nIndexGOL] << 5) | (gobl_value_array[nIndexGOBL]);
			m_volume_data.reserve[i+18] = nValue;
		}
		//
	}
	
    EndDialog(IDOK);
	
	CDialog::OnOK();
}

LRESULT CDlgLineinGain::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( message == WM_LISTEX_COLLUM_INDEX )
	{
        int nRow = m_lstVolume.GetCurSel();
        int nCol = m_lstVolume.GetCurSelCol();
        int nSpinLow = m_vAllLinesData[nRow].at(nCol).m_nSpinLow;
        int nSpinHigh = m_vAllLinesData[nRow].at(nCol).m_nSpinHigh;        
        
        m_lstVolume.SetShowType( m_vAllLinesData[nRow].at( nCol).m_enShowType );
        if( m_vAllLinesData[nRow].at( nCol).m_bShowTitle )
        {
            CString sNum;
            
            CString sTitle(m_vAllLinesData[nRow].at( nCol ).m_strShowString);
            if( CInPlaceEdit::HEX_NUMBER == m_vAllLinesData[nRow].at( nCol ).m_enumEditType )
            {
                sNum.Format(_T("%X"),m_vAllLinesData[nRow].at( nCol ).m_lValue);
            }
            else
            {
                sNum.Format(_T("%d"),m_vAllLinesData[nRow].at( nCol ).m_lValue);                
            }            
            sTitle.Replace(sNum,_T(""));
            m_lstVolume.SetSpinTitle( sTitle );
        }    
        else
        {
            m_lstVolume.SetSpinTitle("");
        }
        
        if( CListCtrlEx::COMBOX_TYPE ==  m_vAllLinesData[nRow].at( nCol ).m_enShowType )
        {
            m_lstVolume.SetComBoxStrings( m_vAllLinesData[nRow].at( nCol ).m_vComboxStrings );
        }
        // set spin range of every cell
        if( -1 != nSpinHigh && -1 != nSpinLow )
            m_lstVolume.SetSpinRange( nSpinLow, nSpinHigh );
        return TRUE;
    }
    else if( message == WM_INPLACE_KILL_FOCUS )
    {
		/*
		CString* pStrValue = (CString*)lParam;
		int nRow = m_lstVolume.GetCurSel();
		int nCol = m_lstVolume.GetCurSelCol();               
		if( nRow != -1 && nCol != -1 )
		{
			double dGain = 0.0;
#ifdef _UNICODE
			char szText[32] = {0};
			WideCharToMultiByte(CP_ACP,0,*pStrValue,-1,szText,32,NULL,NULL);
			dGain = atof( szText );
#else
			dGain = atof( (LPSTR)(LPCTSTR)*pStrValue );
#endif
			
			unsigned short nGain;
			::CalcDigGain( dGain,nGain );
			
			m_vAllLinesData[ nRow ].at(nCol).m_strShowString = *pStrValue;
			m_vAllLinesData[ nRow ].at(nCol).m_dReserved  = dGain;
			
			CString strValue;
			strValue.Format( _T("0x%x"),nGain );
			
			m_vAllLinesData[ nRow ].at(nCol+1).m_dReserved = nGain;
			m_vAllLinesData[ nRow ].at(nCol+1).m_strShowString.Format(_T("%d"), nGain);
			//m_vAllLinesData[ nRow ].at(nCol+1).m_strShowString = nGain;
			m_lstVolume.SetItemText( nRow,nCol+1,strValue );
		}*/
        return TRUE;
    }
    else if ( message == WM_INPLACE_COMBOX )
    {
        LPCOMBOX_DATA pData = (LPCOMBOX_DATA)lParam;
        int nRow = m_lstVolume.GetCurSel();
        int nCol = m_lstVolume.GetCurSelCol();      
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

void CDlgLineinGain::SetValue(VOLUME_STRUCT& volume_data)
{
	memcpy(&m_volume_data, &volume_data, sizeof(VOLUME_STRUCT));
}

void CDlgLineinGain::FetchValue(VOLUME_STRUCT& volume_data)
{
	memcpy(&volume_data, &m_volume_data, sizeof(VOLUME_STRUCT));
}
	
void CDlgLineinGain::InitListCtrl()
{
	// Set style
	DWORD dwStyle = m_lstVolume.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES ;    
    dwStyle &= ~LVS_EX_HEADERDRAGDROP;
    m_lstVolume.SetExtendedStyle( dwStyle );

	// Set Data range
	//m_lstVolume.SetEditCheckRange(TRUE,18,-100 );
	m_lstVolume.SetInplaceEditType( CInPlaceEdit::FLOAT_NUMBER,5 );

	// Insert columns
    LV_COLUMN	lvc;
    lvc.mask = LVCF_WIDTH | LVCF_TEXT ;
	const int lstColumnNum = 5;
	//int lstVolLevel = 15;

    char columnLabel[lstColumnNum][128];
    strcpy( columnLabel[0], "Level" );			
    strcpy( columnLabel[1], "GOL/R" );		
    strcpy( columnLabel[2], "GOBL/R" );	
	strcpy( columnLabel[3], "inner PA" );
	strcpy( columnLabel[4], "nv value" );

	// Set column styles
    int DefaultColumnWidth[] = { 120,116,116,116,116};
	//teana hu 2012.02.02
// 	if(m_bDspCodecInfo)
// 	{
// 		DefaultColumnWidth[1] = 0;
// 	}
	//

	//teana hu 2012.06.13
	if(m_bSmartPhone)
	{
		if(m_strTitle == _T("Handsfree"))
		{
			DefaultColumnWidth[1] = 0;
		}
		else
		{
			DefaultColumnWidth[3] = 0;
		}
	}
	else
	{
		DefaultColumnWidth[3] = 0;
	}
	//
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
        
        m_lstVolume.InsertColumn( i, &lvc );
    } 
	
	m_vAllLinesData.clear();
	CString strFormat;

    for( i=0; i<m_nLstLevel; ++i )
    {
        ROW_DATA singleLineData; 
        CListCtrlEx::COLUMN_DATA columnData;    

        // row = i, column = 0;
        columnData.m_enShowType = CListCtrlEx::NOT_EDIT;
		if(m_bDspCodecInfo)
		{
			columnData.m_strShowString.Format(_T("Level %d"),i+1);
		}
		else
		{
			columnData.m_strShowString.Format(_T("Level %d"),i);
		}
        
        columnData.m_lValue = -1;
        columnData.m_dReserved = 0.0;
        columnData.m_nSpinLow=-1;
        columnData.m_nSpinHigh = -1;
        columnData.m_bShowTitle = FALSE;
        columnData.m_enumEditType = CInPlaceEdit::DEFAULT;
        singleLineData.push_back( columnData );


        // row = i, column =1;
        columnData.m_enShowType = CListCtrlEx::COMBOX_TYPE;
		int nIndex  = 0;
		//teana hu 2012.06.06
		if(m_bDspCodecInfo)
		{
			nIndex = CalcGOL( columnData.m_dReserved,m_volume_data.reserve[18+i] );
			if(nIndex == 0)
			{
				columnData.m_strShowString = _T("mute");
			}
			else
			{
				columnData.m_strShowString.Format( _T("%.2f"), columnData.m_dReserved );
			}
		}
		//teana hu 2012.06.12
		else if(m_bSmartPhone)
		{
			nIndex = CalcGOL( columnData.m_dReserved,m_volume_data.reserve[41] );
			columnData.m_strShowString.Format( _T("%.1f"), columnData.m_dReserved );
		}
		//
		else
		{
			nIndex = CalcGOL( columnData.m_dReserved,m_volume_data.reserve[18+i] );	
			columnData.m_strShowString.Format( _T("%.2f"), columnData.m_dReserved );
		}
			 
		columnData.m_lValue = nIndex;
        columnData.m_vComboxStrings.clear();
		//teana hu 2012.06.06
		if(m_bDspCodecInfo)
		{
			for( int j=0;j<16;j++ )
			{
				if(j == 0)
				{
					strFormat = _T("mute");
				}
				else
				{
					if(m_strTitle == _T("Handsfree"))
					{
						strFormat.Format( _T("%.2f"),gol_dB_6530_handsfree_array[j] );
					}
					else if(m_strTitle == _T("Headset"))
					{
						strFormat.Format( _T("%.2f"),gol_dB_6530_headset_array[j] );
					}
				}		
				columnData.m_vComboxStrings.push_back(strFormat);
			}
		}
		else if(m_bSmartPhone)
		{
			for( int j=0;j<32;j++ )
			{
				strFormat.Format( _T("%.1f"),gol_dB_smartphone_array[j] );		
				columnData.m_vComboxStrings.push_back(strFormat);
			}
		}
		else
		{
			for( int j=0;j<32;j++ )
			{
				strFormat.Format( _T("%.2f"),gol_dB_array[j] );
				
				columnData.m_vComboxStrings.push_back(strFormat);
			}
		}
		

        columnData.m_nSpinLow=-1;
        columnData.m_nSpinHigh = -1;                
        columnData.m_bShowTitle = FALSE;               
        singleLineData.push_back( columnData );

        // row = i, column = 2;
        columnData.m_enShowType = CListCtrlEx::COMBOX_TYPE;  
		//teana hu 2012.02.02
		if(m_bDspCodecInfo)
		{
			nIndex = CalcGOBL( columnData.m_dReserved,m_volume_data.reserve[18+i] );
		}
		else if(m_bSmartPhone)
		{
			nIndex = CalcGOBL( columnData.m_dReserved,m_volume_data.reserve[41] );
		}
		else
		{
			nIndex = CalcGOBL( columnData.m_dReserved,m_volume_data.reserve[18+i] );	
		}
		//
        columnData.m_strShowString.Format( _T("%.1f"), columnData.m_dReserved );
	    columnData.m_lValue = nIndex;
        columnData.m_vComboxStrings.clear();
		if(m_bDspCodecInfo)
		{
			for(int j=0;j<16;j++ )
			{
				strFormat.Format( _T("%.1f"),gobl_dB_6530_array[j] );
				
				columnData.m_vComboxStrings.push_back(strFormat);
			}
		}
		else if(m_bSmartPhone)
		{
			for(int j=0;j<32;j++ )
			{
				strFormat.Format( _T("%.1f"),gobl_dB_smartphone_array[j] );
				
				columnData.m_vComboxStrings.push_back(strFormat);
			}
		}
		else
		{
			for(int j=0;j<32;j++ )
			{
				strFormat.Format( _T("%.1f"),gobl_dB_array[j] );
				
				columnData.m_vComboxStrings.push_back(strFormat);
			}
		}

        columnData.m_nSpinLow=-1;
        columnData.m_nSpinHigh = -1;                
        columnData.m_bShowTitle = FALSE;               
        singleLineData.push_back( columnData );

		//teana hu 2012.06.12
		// row = i, column =3;
        columnData.m_enShowType = CListCtrlEx::COMBOX_TYPE;  
		nIndex = CalcPA( columnData.m_dReserved,(unsigned short)m_volume_data.reserve[45] );
		if(nIndex == 0)
		{
			columnData.m_strShowString = _T("mute");
		}
		else
		{
			columnData.m_strShowString.Format( _T("%.1f"), columnData.m_dReserved );
		}		
        
		columnData.m_lValue = nIndex;
		columnData.m_vComboxStrings.clear();
		
		for( int j=0;j<16;j++ )
		{
			if(j == 0)
			{
				strFormat = _T("mute");
			}
			else
			{
				strFormat.Format( _T("%.1f"),pa_dB_handsfree_array[j] );
			}
			
			columnData.m_vComboxStrings.push_back(strFormat);
		}
		
        columnData.m_nSpinLow=-1;
        columnData.m_nSpinHigh = -1;                
        columnData.m_bShowTitle = FALSE;               
        singleLineData.push_back( columnData ); 
		//

		// row = i, column =4;
        columnData.m_enShowType = CListCtrlEx::NOT_EDIT;
		//teana hu 2012.02.02
		if(m_bDspCodecInfo)
		{
			columnData.m_lValue = (unsigned short)m_volume_data.reserve[18+i];
			columnData.m_strShowString.Format( _T("0x%x"), (unsigned short)m_volume_data.reserve[18+i]);
		}
		else if(m_bSmartPhone)
		{
			columnData.m_lValue = (unsigned short)m_volume_data.reserve[41];
			columnData.m_strShowString.Format( _T("0x%x"), (unsigned short)m_volume_data.reserve[41]);
		}
		else
		{
			columnData.m_lValue = (unsigned short)m_volume_data.reserve[18+i];
			columnData.m_strShowString.Format( _T("0x%x"), (unsigned short)m_volume_data.reserve[18+i]);
		}
		//
        columnData.m_nSpinLow=0;
        columnData.m_nSpinHigh = 32767;
        columnData.m_bShowTitle = FALSE;
        columnData.m_enumEditType = CInPlaceEdit::HEX_NUMBER;
        singleLineData.push_back( columnData );

		m_vAllLinesData.push_back( singleLineData );
    }   
	
	for( i=0; i<(signed)m_vAllLinesData.size(); ++i )
    {        
        int nColumnCount = m_vAllLinesData[i].size();
        m_lstVolume.InsertItem( i,_T("") );
        for( int j=0; j<nColumnCount; ++j )
        {            
            m_lstVolume.SetItemText( i, j, m_vAllLinesData[i].at( j).m_strShowString );
        }        
    }
}

int CDlgLineinGain::CalcGOL ( double& gol, int volume )
{
	if(m_bDspCodecInfo)
	{
		unsigned short value = volume & 0x000f;
		for(int i = 0; i < 16; i++)
		{
			//teana hu 2012.06.06
			if(m_strTitle == _T("Handsfree"))
			{
				if(gol_value_6530_array[i] == value)
				{
					gol = gol_dB_6530_handsfree_array[i];
					break;
				}
			}
			else if(m_strTitle == _T("Headset"))
			{
				if(gol_value_6530_array[i] == value)
				{
					gol = gol_dB_6530_headset_array[i];
					break;
				}
			}
			
		}
		return i;
	}
	else if(m_bSmartPhone)
	{
		unsigned short value = volume & 0x00ff;
		for(int i = 0; i < 32; i++)
		{
			if(gol_value_smartphone_array[i] == value)
			{
				gol = gol_dB_smartphone_array[i];
				break;
			}
		}
		return i;
	}
	else
	{
		unsigned short value = (volume & 0x03e0) >> 5;
		for(int i = 0; i < 32; i++)
		{
			if(gol_value_array[i] == value)
			{
				gol = gol_dB_array[i];
				break;
			}
		}
		return i;
	}
}

int CDlgLineinGain::CalcGOBL ( double& gobl, unsigned short volume )
{
	if(m_bDspCodecInfo)
	{
		unsigned short value = (volume & 0xf0) >> 4;
		for(int i = 0; i < 16; i++)
		{
			if(gobl_value_6530_array[i] == value)
			{
				gobl = gobl_dB_6530_array[i];
				break;
			}
		}
		return i;
	}
	else if(m_bSmartPhone)
	{
		unsigned short value = (volume & 0xff00) >> 8;
		for(int i = 0; i < 32; i++)
		{
			if(gobl_value_smartphone_array[i] == value)
			{
				gobl = gobl_dB_smartphone_array[i];
				break;
			}
		}
		return i;
	}
	else
	{
		unsigned short value = volume & 0x1f;
		for(int i = 0; i < 32; i++)
		{
			if(gobl_value_array[i] == value)
			{
				gobl = gobl_dB_array[i];
				break;
			}
		}
		return i;
	}
}

int CDlgLineinGain::CalcPA( double& pa, LONG volume )
{
	unsigned short value = (volume & 0xff00) >> 8;
	for(int i = 0; i < 16; i++)
	{
		if(m_strTitle == _T("Handsfree") || m_strTitle == _T("Headfree"))
		{
			if(pa_value_handsfree_array[i] == value)
			{
				pa = pa_dB_handsfree_array[i];
				break;
			}
		}
	}
	if(i >= 16)
	{
		i = 255;
	}
	return i;
}