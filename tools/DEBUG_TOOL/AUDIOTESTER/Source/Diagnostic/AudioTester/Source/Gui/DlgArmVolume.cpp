// DlgArmVolume.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "DlgArmVolume.h"
#include "BMPCtrlWithSpecRect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Vdg Table
//客户可选值 : Vdg dB
float vdg_dB_array[128] ={-48.00,-47.25,-46.50,-45.75,-45.00,-44.25,-43.50,-42.75,-42.00,-41.25,-40.50,-39.75,-39.00,-38.25,-37.50,-36.75,-36.00,-35.25,-34.50,-33.75,-33.00,-32.25,-31.50,-30.75,-30.00,-29.25,-28.50,-27.75,-27.00,-26.25,-25.50,-24.75,-24.00,-23.25,-22.50,-21.75,-21.00,-20.25,-19.50,-18.75,-18.00,-17.25,-16.50,-15.75,-15.00,-14.25,-13.50,-12.75,-12.00,-11.25,-10.50,-9.75,-9.00,-8.25,-7.50,-6.75,-6.00,-5.25,-4.50,-3.75,-3.00,-2.25,-1.50,-0.75,0.00,0.75,1.50,2.25,3.00,3.75,4.50,5.25,6.00,6.75,7.50,8.25,9.00,9.75,10.50,11.25,12.00,12.75,13.50,14.25,15.00,15.75,16.50,17.25,18.00,18.75,19.50,20.25,21.00,21.75,22.50,23.25,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00,24.00};

// nv 设定值 : Vdg value      对应于 volume value: bit15~bit 9
short vdg_value_array[128]={-64,-63,-62,-61,-60,-59,-58,-57,-56,-55,-54,-53,-52,-51,-50,-49,-48,-47,-46,-45,-44,-43,-42,-41,-40,-39,-38,-37,-36,-35,-34,-33,-32,-31,-30,-29,-28,-27,-26,-25,-24,-23,-22,-21,-20,-19,-18,-17,-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63};



//  P2(GOL/R) Table
//客户可选值 P2(GOL/R)  dB
float p2_dB_array[32]={4.5,4.0,3.5,3.0,2.5,2.0,1.5,1.0,0.5,-0.5,-1.5,-2.5,-3.5,-4.5,-5.5,-6.5,-7.5,-8.5,-9.5,-10.5,-11.5,-13.5,-15.5,-17.5,-19.5,-21.5,-23.5,-25.5,-27.5,-29.5,-31.5,-33.5};

// nv 设定值 : P2(GOL/R) value 对应于 volume value: bit8 ~ bit4
short p2_value_array[32]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};


//  P1(GODL/R) Table
//客户可选值   P1(GODL/R)  dB
float p1_dB_array[16]={0.0,-1.5,-3.0,-4.5,-6.0,-7.5,-9.0,-10.5,-12.0,-13.5,-15.0,-16.5,-18.0,-19.5,-21.0,-22.5};

// nv 设定值 :   P1(GODL/R) value 对应于 volume value: bit3 ~ bit0
short p1_value_array[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

/////////////////////////////////////////////////////////////////////////////
// CDlgArmVolume dialog

CDlgArmVolume::CDlgArmVolume(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgArmVolume::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgArmVolume)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pArea = NULL;
}


void CDlgArmVolume::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgArmVolume)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_LIST_ARM_VOLUME, m_lstVolume);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgArmVolume, CDialog)
	//{{AFX_MSG_MAP(CDlgArmVolume)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgArmVolume message handlers

BOOL CDlgArmVolume::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitListCtrl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgArmVolume::InitListCtrl()
{
	// Set style
	DWORD dwStyle = m_lstVolume.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES ;    
    dwStyle &= ~LVS_EX_HEADERDRAGDROP;
    m_lstVolume.SetExtendedStyle( dwStyle );

	// Set Data range
	m_lstVolume.SetEditCheckRange(TRUE,18,-100 );
	m_lstVolume.SetInplaceEditType( CInPlaceEdit::FLOAT_NUMBER,5 );

	// Insert columns
    LV_COLUMN	lvc;
    lvc.mask = LVCF_WIDTH | LVCF_TEXT ;
	const int lstColumnNum = 5;
	int lstVolLevel = 0;
	//teana hu 2011.02.21
	if(m_pArea->m_bOldVer)
	{
		lstVolLevel = 10;
	}
	else
	{
		lstVolLevel = m_pArea->m_narmVolumeLevel[m_pArea->m_nCurType];
	}
	//
    char columnLabel[lstColumnNum][128];
    strcpy( columnLabel[0], "Volume level" );	
    strcpy( columnLabel[1], "Vdg(dB)" );		
    strcpy( columnLabel[2], "P2(GOL/R)(dB)" );		
    strcpy( columnLabel[3], "P1(GODL/R)(dB)" );	
    strcpy( columnLabel[4], "Volume value" );		


	// Set column styles
    int DefaultColumnWidth[] = { 120,155,155,155,155 };
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

    for( i=0; i<lstVolLevel; ++i )
    {
        ROW_DATA singleLineData; 
        CListCtrlEx::COLUMN_DATA columnData;    

        // row = i, column = 0;
        columnData.m_enShowType = CListCtrlEx::NOT_EDIT;            
        columnData.m_strShowString.Format(_T("Level %d"),i );
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
		if(m_pArea->m_bOldVer)
		{
			nIndex = CalcVdg( columnData.m_dReserved,m_pArea->m_armVolume[i] );	
		}
		else
		{
			nIndex = CalcVdg( columnData.m_dReserved,m_pArea->m_armVolumeMul[m_pArea->m_nCurType][i] );	
		}
			 
        columnData.m_strShowString.Format( _T("%.2f"), columnData.m_dReserved );
		columnData.m_lValue = nIndex;
        columnData.m_vComboxStrings.clear();
		for( int j=0;j<128;j++ )
		{
			strFormat.Format( _T("%.2f"),vdg_dB_array[j] );
			
			columnData.m_vComboxStrings.push_back(strFormat);
        }

        columnData.m_nSpinLow=-1;
        columnData.m_nSpinHigh = -1;                
        columnData.m_bShowTitle = FALSE;               
        singleLineData.push_back( columnData );


        // row = i, column = 2;
        columnData.m_enShowType = CListCtrlEx::COMBOX_TYPE;  
		if(m_pArea->m_bOldVer)
		{
			nIndex = CalcP2( columnData.m_dReserved,(unsigned short)m_pArea->m_armVolume[i] );
		}
		else
		{
			nIndex = CalcP2( columnData.m_dReserved,(unsigned short)m_pArea->m_armVolumeMul[m_pArea->m_nCurType][i] );
		}
        columnData.m_strShowString.Format( _T("%.1f"), columnData.m_dReserved );
	    columnData.m_lValue = nIndex;
        columnData.m_vComboxStrings.clear();
		for( j=0;j<32;j++ )
		{
			strFormat.Format( _T("%.1f"),p2_dB_array[j] );
			
			columnData.m_vComboxStrings.push_back(strFormat);
        }

        columnData.m_nSpinLow=-1;
        columnData.m_nSpinHigh = -1;                
        columnData.m_bShowTitle = FALSE;               
        singleLineData.push_back( columnData );


        // row = i, column =3;
        columnData.m_enShowType = CListCtrlEx::COMBOX_TYPE;  
		if(m_pArea->m_bOldVer)
		{
			nIndex = CalcP1( columnData.m_dReserved,(unsigned short)m_pArea->m_armVolume[i] );	
		}
		else
		{
			nIndex = CalcP1( columnData.m_dReserved,(unsigned short)m_pArea->m_armVolumeMul[m_pArea->m_nCurType][i] );		
		}
        columnData.m_strShowString.Format( _T("%.1f"), columnData.m_dReserved );
	    columnData.m_lValue = nIndex;
		columnData.m_vComboxStrings.clear();
		for( j=0;j<16;j++ )
		{
			strFormat.Format( _T("%.1f"),p1_dB_array[j] );
			
			columnData.m_vComboxStrings.push_back(strFormat);
        }

        columnData.m_nSpinLow=-1;
        columnData.m_nSpinHigh = -1;                
        columnData.m_bShowTitle = FALSE;               
        singleLineData.push_back( columnData );      
		
		// row = i, column =4;
        columnData.m_enShowType = CListCtrlEx::NOT_EDIT;
		if(m_pArea->m_bOldVer)
		{
			columnData.m_lValue = (unsigned short)m_pArea->m_armVolume[i];
			columnData.m_strShowString.Format( _T("0x%x"), (unsigned short)m_pArea->m_armVolume[i] );
		}
		else
		{
			columnData.m_lValue = (unsigned short)m_pArea->m_armVolumeMul[m_pArea->m_nCurType][i];
			columnData.m_strShowString.Format( _T("0x%x"), (unsigned short)m_pArea->m_armVolumeMul[m_pArea->m_nCurType][i]);
		}
        
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

int CDlgArmVolume::CalcVdg ( double& vdg, int volume )
{
	short value = (volume & 0xFFFFFE00) >> 9;
	for(int i = 0; i < 128; i++)
	{
		if(vdg_value_array[i] == value)
		{
			vdg = vdg_dB_array[i];
			break;
		}
	}
	return i;
}

int CDlgArmVolume::CalcP2 ( double& p2, unsigned short volume )
{
	unsigned short value = (volume & 0x01F0) >> 4;
	for(int i = 0; i < 128; i++)
	{
		if(p2_value_array[i] == value)
		{
			p2 = p2_dB_array[i];
			break;
		}
	}
	return i;
}

int CDlgArmVolume::CalcP1 ( double& p1, unsigned short volume )
{
	unsigned short value = volume & 0x000F;
	for(int i = 0; i < 128; i++)
	{
		if(p1_value_array[i] == value)
		{
			p1 = p1_dB_array[i];
			break;
		}
	}
	return i;
}

LRESULT CDlgArmVolume::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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
//        CString* pStrValue = (CString*)lParam;
//        int nRow = m_lstVolume.GetCurSel();
//        int nCol = m_lstVolume.GetCurSelCol();               
//        if( nRow != -1 && nCol != -1 )
//        {
//			double dGain = 0.0;
//#ifdef _UNICODE
//			char szText[32] = {0};
//			WideCharToMultiByte(CP_ACP,0,*pStrValue,-1,szText,32,NULL,NULL);
//			dGain = atof( szText );
//#else
//			dGain = atof( (LPSTR)(LPCTSTR)*pStrValue );
//#endif
//			
//			unsigned short nGain;
//			::CalcDigGain( dGain,nGain );
//
//            m_vAllLinesData[ nRow ].at(nCol).m_strShowString = *pStrValue;
//            m_vAllLinesData[ nRow ].at(nCol).m_dReserved  = dGain;
//
//			CString strValue;
//			strValue.Format( _T("0x%x"),nGain );
//
//			m_vAllLinesData[ nRow ].at(nCol+1).m_dReserved = nGain;
//			m_vAllLinesData[ nRow ].at(nCol+1).m_strShowString.Format(_T("%d"), nGain);
//			//m_vAllLinesData[ nRow ].at(nCol+1).m_strShowString = nGain;
//			m_lstVolume.SetItemText( nRow,nCol+1,strValue );
//         }
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

		int nIndexVdg = (int)(m_vAllLinesData[nRow].at( 1 ).m_lValue);
		int nIndexP2 = (int)(m_vAllLinesData[nRow].at( 2 ).m_lValue);
		int nIndexP1 = (int)(m_vAllLinesData[nRow].at( 3 ).m_lValue);
		m_vAllLinesData[nRow].at( 4 ).m_lValue = (vdg_value_array[nIndexVdg] << 9) | (p2_value_array[nIndexP2] << 4) | p1_value_array[nIndexP1];
		m_vAllLinesData[nRow].at( 4 ).m_strShowString.Format( _T("0x%x"),m_vAllLinesData[nRow].at( 4 ).m_lValue );
        m_lstVolume.SetItemText( nRow, 4, m_vAllLinesData[nRow].at(4).m_strShowString );
		return TRUE;		
    }	

	
	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CDlgArmVolume::OnOK() 
{
	// TODO: Add extra validation here
	 if (!UpdateData(TRUE))
    {
        TRACE0("UpdateData failed during dialog termination.\n");
        // the UpdateData routine will set focus to correct item
        return;
    }

	int nValue = 0;
	int nSize = 0;
	if(m_pArea->m_bOldVer)
	{
		nSize = ARM_VOLUME_SIZE;
	}
	else
	{
		nSize = m_pArea->m_narmVolumeLevel[m_pArea->m_nCurType];
	}
	for( int i=0;i<nSize;i++ )
	{
		int nIndexVdg = (int)(m_vAllLinesData[i].at( 1 ).m_lValue);
		int nIndexP2 = (int)(m_vAllLinesData[i].at( 2 ).m_lValue);
		int nIndexP1 = (int)(m_vAllLinesData[i].at( 3 ).m_lValue);

		nValue = (vdg_value_array[nIndexVdg] << 9) | (p2_value_array[nIndexP2] << 4) | p1_value_array[nIndexP1];

		if(m_pArea->m_bOldVer)
		{
			m_pArea->m_armVolume[i] = nValue;
		}
		else
		{
			m_pArea->m_armVolumeMul[m_pArea->m_nCurType][i] = nValue;
		}
	}

    EndDialog(IDOK);
	
//	CDialog::OnOK();
}
