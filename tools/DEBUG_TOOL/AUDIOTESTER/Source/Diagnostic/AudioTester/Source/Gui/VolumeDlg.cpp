// VolumeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "BMPCtrlWithSpecRect.h"
#include "VolumeDlg.h"

#include "ConvertFunc.h"
#include <math.h>
#include "AudioTesterSheet.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//teana hu 2012.06.12
// nv �趨ֵ :   inner PA
const short CVolumeDlg::pa_value_handsfree_array[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

//for smartphone
const float CVolumeDlg::pa_dB_handsfree_array[16]={-255,-21,-18,-15,-12,-9,-6,-3,0,3,6,9,12,15,18,21};//-255 = mute

/////////////////////////////////////////////////////////////////////////////
// CVolumeDlg dialog


CVolumeDlg::CVolumeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVolumeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVolumeDlg)
		// NOTE: the ClassWizard will add member initialization here
	m_nStep = 0;
	//}}AFX_DATA_INIT
	m_pArea = NULL;
	m_bDspCodecInfo = FALSE;
	m_bSmartPhone = FALSE;
}


void CVolumeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVolumeDlg)
	DDX_Control(pDX, IDC_LIST_VOLUME, m_lstVolume);
	DDX_Text(pDX, IDC_EDT_STEP_VOLUME, m_nStep);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVolumeDlg, CDialog)
	//{{AFX_MSG_MAP(CVolumeDlg)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_VOLUME_LIST, OnButtonRefreshVolumeList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVolumeDlg message handlers

BOOL CVolumeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//teana hu 2012.04.05
	if(m_pArea->m_nLevel > VOLUME_LEVEL_BASE)
	{
		GetDlgItem(IDC_EDT_STEP_VOLUME)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDT_STEP_VOLUME)->EnableWindow(FALSE);
	}
	//
	
	InitListCtrl();

	m_nStep = CaclStep(m_pArea->m_nStep);
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVolumeDlg::InitListCtrl()
{
	// Set style
	DWORD dwStyle = m_lstVolume.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES ;    
    dwStyle &= ~LVS_EX_HEADERDRAGDROP;
    m_lstVolume.SetExtendedStyle( dwStyle );

	// Set Data range
	m_lstVolume.SetEditCheckRange(TRUE,18,-72 );
	m_lstVolume.SetInplaceEditType( CInPlaceEdit::FLOAT_NUMBER,5 );

	// Insert columns
    LV_COLUMN	lvc;
    lvc.mask = LVCF_WIDTH | LVCF_TEXT ;
	const int lstColumnNum = 13;
	int lstVolLevel = m_pArea->m_nLevel;
    char columnLabel[lstColumnNum][128];
    strcpy( columnLabel[0], "Vol level" );	
    strcpy( columnLabel[1], "dl st gain(db)" );		
    strcpy( columnLabel[2], "value" );		
    strcpy( columnLabel[3], "dl PGA0(db)" );	
    strcpy( columnLabel[4], "dl dgain(db)" );		
    strcpy( columnLabel[5], "value" );				
	strcpy( columnLabel[6], "ul PGA0(db)" );
	strcpy( columnLabel[7], "ul dgain(db)");
	strcpy( columnLabel[8], "value" );
	strcpy( columnLabel[9], "aec vol comp gain(db)" );	
    strcpy( columnLabel[10], "value" );
	strcpy( columnLabel[11], "inner PA(db)" );
	strcpy( columnLabel[12], "value" );

	const double dl_pga0_gain_table[] = { -22.5,-21,-19.5,-18,-16.5,-15,-13.5,-12,-10.5,-9,-7.5,-6,-4.5,-3,-1.5,0 };
	const double ul_pga0_gain_table[] = { 0,1.5,3,4.5,6,7.5,9,10.5,12,13.5,15,16.5,18,19.5,21,22.5 };

	const double dl_pga0_gain_6530_table[] = {-255, -33,-30,-27,-24,-21,-18,-15,-12,-9,-6,-3,0,3,6,9 }; //-255 = mute
	const double ul_pga0_gain_6530_table[] = {  -6,-3,0,3,6,9,12,15,18,21,24,27,30,33,36,39 };
	const double dl_pga0_gain_6530_handsfree_table[] = {-255,-21,-18,-15,-12,-9,-6,-3,0,3,6,9,12,15,18,21 }; //-255 = mute
	
	CString strTitle;

	//teana hu 2012.02.21
	CAudioTesterSheet *pSheet = (CAudioTesterSheet *)GetParent();
	if(pSheet != NULL)
	{
		( (CAudioModePage*)pSheet->GetActivePage() )->GetTitle( strTitle );
	}
	
	//

	// Set column styles
    int DefaultColumnWidth[] = { 90,120,0,100,100,0,100,100,0,180,0,100,0 };
//    int nFormat[] = {	LVCFMT_CENTER, LVCFMT_CENTER ,LVCFMT_CENTER};
	//teana hu 2012.06.12
	if(!m_bSmartPhone)
	{
		if(strTitle != _T("Handsfree") || strTitle != _T("HandsfreeTD"))
		{
			DefaultColumnWidth[11] = 0;
			DefaultColumnWidth[12] = 0;
		}
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

	//teana hu 2012.03.29
	if(m_bDspCodecInfo)
	{
		//lstVolLevel = m_pArea->m_nLevel6530;
		for( i=0; i<lstVolLevel; ++i )
		{
			ROW_DATA singleLineData; 
			CListCtrlEx::COLUMN_DATA columnData;    
			
			// row = i, column = 0;
			columnData.m_enShowType = CListCtrlEx::NOT_EDIT;            
			columnData.m_strShowString.Format(_T("Vol %d"),i+1 );
			columnData.m_lValue = -1;
			columnData.m_dReserved = 0.0;
			columnData.m_nSpinLow=-1;
			columnData.m_nSpinHigh = -1;
			columnData.m_bShowTitle = FALSE;
			columnData.m_enumEditType = CInPlaceEdit::DEFAULT;
			singleLineData.push_back( columnData );
			
			
			// row = i, column =1;
			if(i < m_pArea->m_nLevel - VOLUME_LEVEL_BASE)
			{
				columnData.m_enShowType = CListCtrlEx::NOT_EDIT;
			}
			else
			{
				columnData.m_enShowType = CListCtrlEx::ONLY_EDIT_TYPE;
			}
			columnData.m_lValue = 0;
			CalcDigGainRev( columnData.m_dReserved,(unsigned short)m_pArea->m_dl_st_gain[i] );	
			//teana hu 2012.06.11
			columnData.m_dReserved = columnData.m_dReserved > 18 ? 18 : columnData.m_dReserved;
			columnData.m_dReserved = columnData.m_dReserved < -72 ? -72 :columnData.m_dReserved;
			//
			columnData.m_strShowString.Format( _T("%f"), columnData.m_dReserved );
			columnData.m_nSpinLow=0;
			columnData.m_nSpinHigh = 32767;
			columnData.m_bShowTitle = FALSE;
			columnData.m_enumEditType = CInPlaceEdit::FLOAT_NUMBER;
			singleLineData.push_back( columnData );
			
			
			// row = i, column = 2;
			columnData.m_enShowType = CListCtrlEx::NOT_EDIT;            
			columnData.m_strShowString.Format( _T("0x%x"),m_pArea->m_dl_st_gain[i] );
			columnData.m_lValue = -1;
			columnData.m_dReserved = m_pArea->m_dl_st_gain[i];
			columnData.m_nSpinLow=-1;
			columnData.m_nSpinHigh = -1;
			columnData.m_bShowTitle = FALSE;
			columnData.m_enumEditType = CInPlaceEdit::DEFAULT;
			singleLineData.push_back( columnData );
			
			
			// row = i, column =3;
			columnData.m_enShowType = CListCtrlEx::COMBOX_TYPE;           
			columnData.m_lValue = m_pArea->m_dl_PGA0[i];
			if(m_pArea->m_dl_PGA0[i] == 0)
			{
				columnData.m_strShowString = _T("mute");
			}
			else
			{
				if(strTitle == _T("Handsfree"))
				{
					columnData.m_strShowString.Format( _T("%.1f"),dl_pga0_gain_6530_handsfree_table[m_pArea->m_dl_PGA0[i]] );
				}
				else
				{
					columnData.m_strShowString.Format( _T("%.1f"),dl_pga0_gain_6530_table[m_pArea->m_dl_PGA0[i]] );
				}
				
			}
			
			
			columnData.m_vComboxStrings.clear();
			for( int j=0;j<16;j++ )
			{
				if(j == 0)
				{
					strFormat = _T("mute");
				}
				else
				{
					if(strTitle == _T("Handsfree"))
					{
						strFormat.Format( _T("%.1f"),dl_pga0_gain_6530_handsfree_table[j] );
					}
					else
					{
						strFormat.Format( _T("%.1f"),dl_pga0_gain_6530_table[j] );
					}
					
				}
				
				columnData.m_vComboxStrings.push_back(strFormat);
			}
			
			columnData.m_nSpinLow=-1;
			columnData.m_nSpinHigh = -1;                
			columnData.m_bShowTitle = FALSE;               
			singleLineData.push_back( columnData );      
			
			// row = i, column =4;
			if(i < m_pArea->m_nLevel - VOLUME_LEVEL_BASE)
			{
				columnData.m_enShowType = CListCtrlEx::NOT_EDIT;
				CalcDigGainRev( columnData.m_dReserved,(unsigned short)m_pArea->m_dl_digital_gain[i], m_pArea->m_nStep, m_pArea->m_nLevel - VOLUME_LEVEL_BASE - i );	
			}
			else
			{
				columnData.m_enShowType = CListCtrlEx::ONLY_EDIT_TYPE;
				CalcDigGainRev( columnData.m_dReserved,(unsigned short)m_pArea->m_dl_digital_gain[i] );	
			} 
			//teana hu 2012.06.11
			columnData.m_dReserved = columnData.m_dReserved > 18 ? 18 : columnData.m_dReserved;
			columnData.m_dReserved = columnData.m_dReserved < -72 ? -72 :columnData.m_dReserved;
			//
			columnData.m_lValue = 0;	 
			columnData.m_strShowString.Format( _T("%f"), columnData.m_dReserved );
			columnData.m_nSpinLow=0;
			columnData.m_nSpinHigh = 32767;
			columnData.m_bShowTitle = FALSE;
			columnData.m_enumEditType = CInPlaceEdit::FLOAT_NUMBER;
			singleLineData.push_back( columnData );
			
			// row = i, column = 5;
			columnData.m_enShowType = CListCtrlEx::NOT_EDIT;            
			columnData.m_strShowString.Format( _T("0x%x"),m_pArea->m_dl_digital_gain[i] );
			columnData.m_lValue = -1;
			columnData.m_dReserved = m_pArea->m_dl_digital_gain[i];
			columnData.m_nSpinLow=-1;
			columnData.m_nSpinHigh = -1;
			columnData.m_bShowTitle = FALSE;
			columnData.m_enumEditType = CInPlaceEdit::DEFAULT;
			singleLineData.push_back( columnData );
			
			
			// row = i, column =6;
			columnData.m_enShowType = CListCtrlEx::COMBOX_TYPE;     
			columnData.m_lValue = m_pArea->m_ul_PGA0[i];
			columnData.m_strShowString.Format( _T("%.1f"),ul_pga0_gain_6530_table[m_pArea->m_ul_PGA0[i]] ); 
			columnData.m_vComboxStrings.clear();
			for( j=0;j<16;j++ )
			{
				strFormat.Format( _T("%.1f"),ul_pga0_gain_6530_table[j] );
				columnData.m_vComboxStrings.push_back(strFormat);
			}
			
			columnData.m_nSpinLow=-1;
			columnData.m_nSpinHigh = -1;                
			columnData.m_bShowTitle = FALSE;               
			singleLineData.push_back( columnData );   
			
			// row = i, column = 7;
			if(i < m_pArea->m_nLevel - VOLUME_LEVEL_BASE)
			{
				columnData.m_enShowType = CListCtrlEx::NOT_EDIT;
			}
			else
			{
				columnData.m_enShowType = CListCtrlEx::ONLY_EDIT_TYPE;
			}
			columnData.m_lValue = 0;
			CalcDigGainRev( columnData.m_dReserved,(unsigned short)m_pArea->m_ul_digital_gain[i] );	
			//teana hu 2012.06.11
			columnData.m_dReserved = columnData.m_dReserved > 18 ? 18 : columnData.m_dReserved;
			columnData.m_dReserved = columnData.m_dReserved < -72 ? -72 :columnData.m_dReserved;
			//
			columnData.m_strShowString.Format( _T("%f"), columnData.m_dReserved );
			columnData.m_nSpinLow=0;
			columnData.m_nSpinHigh = 32767;
			columnData.m_bShowTitle = FALSE;
			columnData.m_enumEditType = CInPlaceEdit::FLOAT_NUMBER;
			singleLineData.push_back( columnData );
			
			// row = i, column =8;
			columnData.m_enShowType = CListCtrlEx::NOT_EDIT;            
			columnData.m_strShowString.Format( _T("0x%x"),m_pArea->m_ul_digital_gain[i] );
			columnData.m_lValue = -1;
			columnData.m_dReserved = m_pArea->m_ul_digital_gain[i];
			columnData.m_nSpinLow=-1;
			columnData.m_nSpinHigh = -1;
			columnData.m_bShowTitle = FALSE;
			columnData.m_enumEditType = CInPlaceEdit::DEFAULT;
			singleLineData.push_back( columnData );
			
			// row = i, column =9;
			if(i < m_pArea->m_nLevel - VOLUME_LEVEL_BASE)
			{
				columnData.m_enShowType = CListCtrlEx::NOT_EDIT;
			}
			else
			{
				columnData.m_enShowType = CListCtrlEx::ONLY_EDIT_TYPE;
			}
			columnData.m_lValue = 0;
			CalcDigGainRev( columnData.m_dReserved,(unsigned short)m_pArea->m_aec_vol_comp_gain[i] );
			//teana hu 2012.06.11
			columnData.m_dReserved = columnData.m_dReserved > 18 ? 18 : columnData.m_dReserved;
			columnData.m_dReserved = columnData.m_dReserved < -72 ? -72 :columnData.m_dReserved;
			//
			columnData.m_dReserved = columnData.m_dReserved < -72 ? -72 : columnData.m_dReserved;
			columnData.m_dReserved = columnData.m_dReserved > 18 ? 18 : columnData.m_dReserved;
			columnData.m_strShowString.Format( _T("%f"), columnData.m_dReserved );
			columnData.m_nSpinLow=0;
			columnData.m_nSpinHigh = 32767;
			columnData.m_bShowTitle = FALSE;
			columnData.m_enumEditType = CInPlaceEdit::FLOAT_NUMBER;
			singleLineData.push_back( columnData );

			
			// row = i, column = 10;
			columnData.m_enShowType = CListCtrlEx::NOT_EDIT;            
			columnData.m_strShowString.Format( _T("0x%x"),m_pArea->m_aec_vol_comp_gain[i] );
			columnData.m_lValue = -1;
			columnData.m_dReserved = m_pArea->m_aec_vol_comp_gain[i];
			columnData.m_nSpinLow=-1;
			columnData.m_nSpinHigh = -1;
			columnData.m_bShowTitle = FALSE;
			columnData.m_enumEditType = CInPlaceEdit::DEFAULT;
			singleLineData.push_back( columnData );   

			// row = i, column =11;
			columnData.m_enShowType = CListCtrlEx::COMBOX_TYPE;  
			int nIndex = CalcPA(columnData.m_dReserved, (unsigned short)m_pArea->m_nInnerPA); 
			if(nIndex == 0)
			{
				columnData.m_strShowString = _T("mute");
			}
			else
			{
				columnData.m_strShowString.Format( _T("%.1f"), columnData.m_dReserved );
			}		
			columnData.m_lValue = columnData.m_dReserved ;
			columnData.m_vComboxStrings.clear();
			
			for( j=0;j<16;j++ )
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

			// row = i, column = 12;
			columnData.m_enShowType = CListCtrlEx::NOT_EDIT;            
			columnData.m_strShowString.Format( _T("0x%x"),m_pArea->m_nInnerPA );
			columnData.m_lValue = -1;
			columnData.m_dReserved = m_pArea->m_nInnerPA;
			columnData.m_nSpinLow=-1;
			columnData.m_nSpinHigh = -1;
			columnData.m_bShowTitle = FALSE;
			columnData.m_enumEditType = CInPlaceEdit::DEFAULT;
			singleLineData.push_back( columnData );
			
			m_vAllLinesData.push_back( singleLineData );
		}  
	}
	//
	else
	{
		for( i=0; i<lstVolLevel; ++i )
		{
			ROW_DATA singleLineData; 
			CListCtrlEx::COLUMN_DATA columnData;    
			
			// row = i, column = 0;
			columnData.m_enShowType = CListCtrlEx::NOT_EDIT;            
			columnData.m_strShowString.Format(_T("Vol %d"),i+1 );
			columnData.m_lValue = -1;
			columnData.m_dReserved = 0.0;
			columnData.m_nSpinLow=-1;
			columnData.m_nSpinHigh = -1;
			columnData.m_bShowTitle = FALSE;
			columnData.m_enumEditType = CInPlaceEdit::DEFAULT;
			singleLineData.push_back( columnData );
			
			
			// row = i, column =1;
			if(i < m_pArea->m_nLevel - VOLUME_LEVEL_BASE)
			{
				columnData.m_enShowType = CListCtrlEx::NOT_EDIT;
			}
			else
			{
				columnData.m_enShowType = CListCtrlEx::ONLY_EDIT_TYPE;
			}
			columnData.m_lValue = 0;
			CalcDigGainRev( columnData.m_dReserved,(unsigned short)m_pArea->m_dl_st_gain[i] );
			//teana hu 2012.06.11
			columnData.m_dReserved = columnData.m_dReserved > 18 ? 18 : columnData.m_dReserved;
			columnData.m_dReserved = columnData.m_dReserved < -72 ? -72 :columnData.m_dReserved;
			//
			columnData.m_strShowString.Format( _T("%f"), columnData.m_dReserved );
			columnData.m_nSpinLow=0;
			columnData.m_nSpinHigh = 32767;
			columnData.m_bShowTitle = FALSE;
			columnData.m_enumEditType = CInPlaceEdit::FLOAT_NUMBER;
			singleLineData.push_back( columnData );
			
			
			// row = i, column = 2;
			columnData.m_enShowType = CListCtrlEx::NOT_EDIT;            
			columnData.m_strShowString.Format( _T("0x%x"),m_pArea->m_dl_st_gain[i] );
			columnData.m_lValue = -1;
			columnData.m_dReserved = m_pArea->m_dl_st_gain[i];
			columnData.m_nSpinLow=-1;
			columnData.m_nSpinHigh = -1;
			columnData.m_bShowTitle = FALSE;
			columnData.m_enumEditType = CInPlaceEdit::DEFAULT;
			singleLineData.push_back( columnData );
			
			
			// row = i, column =3;
			if(i < m_pArea->m_nLevel - VOLUME_LEVEL_BASE)
			{
				columnData.m_enShowType = CListCtrlEx::NOT_EDIT;
			}
			else
			{
				columnData.m_enShowType = CListCtrlEx::COMBOX_TYPE;
			}       
			columnData.m_lValue = m_pArea->m_dl_PGA0[i];
			columnData.m_strShowString.Format( _T("%.1f"),dl_pga0_gain_table[m_pArea->m_dl_PGA0[i]] );
			
			columnData.m_vComboxStrings.clear();
			for( int j=0;j<16;j++ )
			{
				strFormat.Format( _T("%.1f"),dl_pga0_gain_table[j] );
				columnData.m_vComboxStrings.push_back(strFormat);
			}
			
			columnData.m_nSpinLow=-1;
			columnData.m_nSpinHigh = -1;                
			columnData.m_bShowTitle = FALSE;               
			singleLineData.push_back( columnData );      
			
			// row = i, column =4;
			if(i < m_pArea->m_nLevel - VOLUME_LEVEL_BASE)
			{
				columnData.m_enShowType = CListCtrlEx::NOT_EDIT;
				CalcDigGainRev( columnData.m_dReserved,(unsigned short)m_pArea->m_dl_digital_gain[i], m_pArea->m_nStep, m_pArea->m_nLevel - VOLUME_LEVEL_BASE - i );	
			}
			else
			{
				columnData.m_enShowType = CListCtrlEx::ONLY_EDIT_TYPE;
				CalcDigGainRev( columnData.m_dReserved,(unsigned short)m_pArea->m_dl_digital_gain[i] );	
			} 
			//teana hu 2012.06.11
			columnData.m_dReserved = columnData.m_dReserved > 18 ? 18 : columnData.m_dReserved;
			columnData.m_dReserved = columnData.m_dReserved < -72 ? -72 :columnData.m_dReserved;
			//
			columnData.m_lValue = 0;	 
			columnData.m_strShowString.Format( _T("%f"), columnData.m_dReserved );
			columnData.m_nSpinLow=0;
			columnData.m_nSpinHigh = 32767;
			columnData.m_bShowTitle = FALSE;
			columnData.m_enumEditType = CInPlaceEdit::FLOAT_NUMBER;
			singleLineData.push_back( columnData );
			
			// row = i, column = 5;
			columnData.m_enShowType = CListCtrlEx::NOT_EDIT;            
			columnData.m_strShowString.Format( _T("0x%x"),m_pArea->m_dl_digital_gain[i] );
			columnData.m_lValue = -1;
			columnData.m_dReserved = m_pArea->m_dl_digital_gain[i];
			columnData.m_nSpinLow=-1;
			columnData.m_nSpinHigh = -1;
			columnData.m_bShowTitle = FALSE;
			columnData.m_enumEditType = CInPlaceEdit::DEFAULT;
			singleLineData.push_back( columnData );
			
			
			// row = i, column =6;
			if(i < m_pArea->m_nLevel - VOLUME_LEVEL_BASE)
			{
				columnData.m_enShowType = CListCtrlEx::NOT_EDIT;
			}
			else
			{
				columnData.m_enShowType = CListCtrlEx::COMBOX_TYPE;
			}         
			columnData.m_lValue = m_pArea->m_ul_PGA0[i];
			columnData.m_strShowString.Format( _T("%.1f"),ul_pga0_gain_table[m_pArea->m_ul_PGA0[i]] );
			
			columnData.m_vComboxStrings.clear();
			for( j=0;j<16;j++ )
			{
				strFormat.Format( _T("%.1f"),ul_pga0_gain_table[j] );
				columnData.m_vComboxStrings.push_back(strFormat);
			}
			
			columnData.m_nSpinLow=-1;
			columnData.m_nSpinHigh = -1;                
			columnData.m_bShowTitle = FALSE;               
			singleLineData.push_back( columnData );   
			
			// row = i, column = 7;
			if(i < m_pArea->m_nLevel - VOLUME_LEVEL_BASE)
			{
				columnData.m_enShowType = CListCtrlEx::NOT_EDIT;
			}
			else
			{
				columnData.m_enShowType = CListCtrlEx::ONLY_EDIT_TYPE;
			}
			columnData.m_lValue = 0;
			CalcDigGainRev( columnData.m_dReserved,(unsigned short)m_pArea->m_ul_digital_gain[i] );	
			//teana hu 2012.06.11
			columnData.m_dReserved = columnData.m_dReserved > 18 ? 18 : columnData.m_dReserved;
			columnData.m_dReserved = columnData.m_dReserved < -72 ? -72 :columnData.m_dReserved;
			//
			columnData.m_strShowString.Format( _T("%f"), columnData.m_dReserved );
			columnData.m_nSpinLow=0;
			columnData.m_nSpinHigh = 32767;
			columnData.m_bShowTitle = FALSE;
			columnData.m_enumEditType = CInPlaceEdit::FLOAT_NUMBER;
			singleLineData.push_back( columnData );
			
			// row = i, column =8;
			columnData.m_enShowType = CListCtrlEx::NOT_EDIT;            
			columnData.m_strShowString.Format( _T("0x%x"),m_pArea->m_ul_digital_gain[i] );
			columnData.m_lValue = -1;
			columnData.m_dReserved = m_pArea->m_ul_digital_gain[i];
			columnData.m_nSpinLow=-1;
			columnData.m_nSpinHigh = -1;
			columnData.m_bShowTitle = FALSE;
			columnData.m_enumEditType = CInPlaceEdit::DEFAULT;
			singleLineData.push_back( columnData );
			
			// row = i, column =9;
			if(i < m_pArea->m_nLevel - VOLUME_LEVEL_BASE)
			{
				columnData.m_enShowType = CListCtrlEx::NOT_EDIT;
			}
			else
			{
				columnData.m_enShowType = CListCtrlEx::ONLY_EDIT_TYPE;
			}
			columnData.m_lValue = 0;
			CalcDigGainRev( columnData.m_dReserved,(unsigned short)m_pArea->m_aec_vol_comp_gain[i] );
			//teana hu 2012.06.11
			columnData.m_dReserved = columnData.m_dReserved > 18 ? 18 : columnData.m_dReserved;
			columnData.m_dReserved = columnData.m_dReserved < -72 ? -72 :columnData.m_dReserved;
			//
			columnData.m_dReserved = columnData.m_dReserved < -72 ? -72 : columnData.m_dReserved;
			columnData.m_dReserved = columnData.m_dReserved > 18 ? 18 : columnData.m_dReserved;
			columnData.m_strShowString.Format( _T("%f"), columnData.m_dReserved );
			columnData.m_nSpinLow=0;
			columnData.m_nSpinHigh = 32767;
			columnData.m_bShowTitle = FALSE;
			columnData.m_enumEditType = CInPlaceEdit::FLOAT_NUMBER;
			singleLineData.push_back( columnData );
			
			// row = i, column = 10;
			columnData.m_enShowType = CListCtrlEx::NOT_EDIT;            
			columnData.m_strShowString.Format( _T("0x%x"),m_pArea->m_aec_vol_comp_gain[i] );
			columnData.m_lValue = -1;
			columnData.m_dReserved = m_pArea->m_aec_vol_comp_gain[i];
			columnData.m_nSpinLow=-1;
			columnData.m_nSpinHigh = -1;
			columnData.m_bShowTitle = FALSE;
			columnData.m_enumEditType = CInPlaceEdit::DEFAULT;
			singleLineData.push_back( columnData );   

			// row = i, column =11;
			columnData.m_enShowType = CListCtrlEx::COMBOX_TYPE;  
			int nIndex = CalcPA(columnData.m_dReserved, (unsigned short)m_pArea->m_nInnerPA); 
			if(nIndex == 0)
			{
				columnData.m_strShowString = _T("mute");
			}
			else
			{
				columnData.m_strShowString.Format( _T("%.1f"), columnData.m_dReserved );
			}		
			columnData.m_lValue = columnData.m_dReserved;	
			
			columnData.m_vComboxStrings.clear();
			
			for( j=0;j<16;j++ )
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
			
			// row = i, column = 12;
			columnData.m_enShowType = CListCtrlEx::NOT_EDIT;            
			columnData.m_strShowString.Format( _T("0x%x"),m_pArea->m_nInnerPA );
			columnData.m_lValue = -1;
			columnData.m_dReserved = m_pArea->m_nInnerPA;
			columnData.m_nSpinLow=-1;
			columnData.m_nSpinHigh = -1;
			columnData.m_bShowTitle = FALSE;
			columnData.m_enumEditType = CInPlaceEdit::DEFAULT;
			singleLineData.push_back( columnData );
			
			m_vAllLinesData.push_back( singleLineData );
		}   
	}
	//
	
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

LRESULT CVolumeDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
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

			//teana hu 2011.05.04
			if(nRow == m_pArea->m_nLevel - VOLUME_LEVEL_BASE)
			{
				for( int i=0; i<m_pArea->m_nLevel - VOLUME_LEVEL_BASE; ++i )
				{
					if(nCol != 4)
					{
						m_vAllLinesData[ i ].at(nCol).m_strShowString = *pStrValue;
						m_vAllLinesData[ i ].at(nCol).m_dReserved  = dGain;
						m_lstVolume.SetItemText( i,nCol,*pStrValue );
						
						CString strValue;
						strValue.Format( _T("0x%x"),nGain );
						
						m_vAllLinesData[ i ].at(nCol+1).m_dReserved = nGain;
						m_vAllLinesData[ i ].at(nCol+1).m_strShowString.Format(_T("%d"), nGain);
						m_lstVolume.SetItemText( i,nCol+1,strValue );
					}
					else
					{
						double dValue = dGain + (m_pArea->m_nLevel - VOLUME_LEVEL_BASE - i) * m_nStep;
						CString strtext;
						strtext.Format(_T("%.6f"), dValue);
						m_vAllLinesData[ i ].at(nCol).m_strShowString = strtext;
						m_vAllLinesData[ i ].at(nCol).m_dReserved  = dValue;
						m_lstVolume.SetItemText( i,nCol,strtext );
						
						CString strValue;
						strValue.Format( _T("0x%x"),nGain );
						
						m_vAllLinesData[ i ].at(nCol+1).m_dReserved = nGain;
						m_vAllLinesData[ i ].at(nCol+1).m_strShowString.Format(_T("%d"), nGain);
						m_lstVolume.SetItemText( i,nCol+1,strValue );
					}
				}
			}
			//
        }
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
		//teana hu 2011.05.04
		if(nRow == m_pArea->m_nLevel - VOLUME_LEVEL_BASE)
		{
			for( int i=0; i<m_pArea->m_nLevel - VOLUME_LEVEL_BASE; ++i )
			{
				m_vAllLinesData[i].at( nCol ).m_lValue = pData->m_nSelectIdx;
				m_vAllLinesData[i].at( nCol ).m_strShowString 
					= m_vAllLinesData[nRow].at( nCol ).m_vComboxStrings[ pData->m_nSelectIdx ];   
				CString sTest(m_vAllLinesData[ nRow ].at(nCol).m_vComboxStrings[pData->m_nSelectIdx]);
				m_vAllLinesData[ i ].at(nCol).m_strShowString
					= m_vAllLinesData[ nRow ].at(nCol).m_vComboxStrings[pData->m_nSelectIdx];

				m_lstVolume.SetItemText( i,nCol,m_vAllLinesData[i].at( nCol ).m_strShowString );
			}
		}
		//

		//teana hu 2012.06.13
		if(nCol == 11) //for inner PA
		{
			int nValue = pa_value_handsfree_array[pData->m_nSelectIdx];
			m_vAllLinesData[ nRow ].at(nCol+1).m_dReserved = nValue;
			m_vAllLinesData[ nRow ].at(nCol+1).m_strShowString.Format(_T("%d"), nValue);

			for(int i = 0; i < m_pArea->m_nLevel; i++)
			{
				if(i != nRow)
				{
					m_vAllLinesData[i].at( nCol ).m_lValue = pData->m_nSelectIdx;
					m_vAllLinesData[i].at( nCol+1 ).m_dReserved = nValue;
					m_vAllLinesData[i].at( nCol ).m_strShowString 
						= m_vAllLinesData[i].at( nCol ).m_vComboxStrings[ pData->m_nSelectIdx ];  
					
					m_lstVolume.SetItemText( i,nCol,m_vAllLinesData[i].at( nCol ).m_strShowString );
				}
			}
		}
		//
        return TRUE;
		
    }

	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CVolumeDlg::OnOK() 
{
    if (!UpdateData(TRUE))
    {
        TRACE0("UpdateData failed during dialog termination.\n");
        // the UpdateData routine will set focus to correct item
        return;
    }

	int nLevel = 0;

	if(m_bDspCodecInfo)
	{
		nLevel = m_pArea->m_nLevel6530;
	}
	else
	{
		nLevel = m_pArea->m_nLevel;
	}

	//teana hu 2012.06.12
	CString strTitle = _T("");
	CAudioTesterSheet *pSheet = (CAudioTesterSheet *)GetParent();
	if(pSheet != NULL)
	{
		( (CAudioModePage*)pSheet->GetActivePage() )->GetTitle( strTitle );
	}
	//

	for( int i=0;i<nLevel;i++ )
	{
		m_pArea->m_dl_st_gain[i] = (int)(m_vAllLinesData[i].at( 2 ).m_dReserved);
		m_pArea->m_dl_PGA0[i] = (int)(m_vAllLinesData[i].at( 3 ).m_lValue);	
		m_pArea->m_dl_digital_gain[i] = (int)(m_vAllLinesData[i].at( 5 ).m_dReserved);
		m_pArea->m_ul_digital_gain[i] = (int)(m_vAllLinesData[i].at(8).m_dReserved);
		m_pArea->m_ul_PGA0[i] = m_vAllLinesData[i].at( 6 ).m_lValue;
		m_pArea->m_aec_vol_comp_gain[i] = (int)(m_vAllLinesData[i].at( 10 ).m_dReserved);

		//teana hu 2012.06.12
		if(m_bSmartPhone && (strTitle == _T("Handsfree") || strTitle == _T("HandsfreeTD")))
		{
			m_pArea->m_nInnerPA = (int)(m_vAllLinesData[i].at( 12 ).m_dReserved);
		}
		//
	}
	

	m_pArea->m_nStep = ReCaclStep(m_nStep);

    EndDialog(IDOK);
}

int CVolumeDlg::CaclStep(int nStep)
{
	int nValue = 20 * log10( nStep / 4096.0 );

	return nValue;
}

int CVolumeDlg::ReCaclStep(int nStep)
{
	int nValue = ((double)pow( 10, nStep / 20.0 )) * 4096;
	return nValue;
}

void CVolumeDlg::OnButtonRefreshVolumeList() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	m_pArea->m_nStep = ReCaclStep(m_nStep);

	double dGain = 0.0;
	unsigned short nGain;
	CString strtext;
	CString strValue;

	for( int i=0; i<m_pArea->m_nLevel - VOLUME_LEVEL_BASE; ++i )
	{	
		strtext = m_lstVolume.GetItemText(m_pArea->m_nLevel - VOLUME_LEVEL_BASE, 4);
#ifdef _UNICODE
		char szText[32] = {0};
		WideCharToMultiByte(CP_ACP,0,strtext,-1,szText,32,NULL,NULL);
		dGain = atof( szText );
#else
		dGain = atof( (LPSTR)(LPCTSTR)strtext );
#endif
		::CalcDigGain( dGain,nGain );
		double dValue = dGain + (m_pArea->m_nLevel - VOLUME_LEVEL_BASE - i) * m_nStep;
		
		strtext.Format(_T("%.6f"), dValue);
		m_vAllLinesData[ i ].at(4).m_strShowString = strtext;
		m_vAllLinesData[ i ].at(4).m_dReserved  = dValue;
		m_lstVolume.SetItemText( i,4,strtext );
		
		strValue.Format( _T("0x%x"),nGain );
		
		m_vAllLinesData[ i ].at(4+1).m_dReserved = nGain;
		m_vAllLinesData[ i ].at(4+1).m_strShowString.Format(_T("%d"), nGain);
		m_lstVolume.SetItemText( i,4+1,strValue );
	}
	
}

int CVolumeDlg::CalcPA( double& pa, LONG volume )
{
	unsigned short value = volume;
	for(int i = 0; i < 16; i++)
	{
		if(pa_value_handsfree_array[i] == value)
		{
			pa = pa_dB_handsfree_array[i];
			break;
		}
	}
	if(i >= 16)
	{
		i = ERROR_INDEX;
	}
	return i;
}