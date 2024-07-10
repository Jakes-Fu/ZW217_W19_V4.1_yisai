// CoefficientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AudioTester.h"
#include "CoefficientDlg.h"
#include "UserDefMsg.h"
#include "AlertMsgDlg.h"

#include <Math.h>
#include "ConvertFunc.h"

#include "FirAutoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define PO_MAX_VALUE 18.1
#define PO_MIN_VALUE -65.1 
/////////////////////////////////////////////////////////////////////////////
// CCoefficientDlg dialog
int CCoefficientDlg::m_vPowerFrequence[ROW_COUNT] = 
{
    200,224,250,281,315,355,400,447,500,561,630,710,800,894,1000,1118,1250,1424,1580,1789,2000,2236,2500,2806,3150,3520
}; 
 
CCoefficientDlg::CCoefficientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCoefficientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCoefficientDlg)
	m_strCoefficient = _T("");
	m_bWriteImmediately = TRUE;
	m_dAddValue = -1.5;
    m_pArea = NULL;
    m_pCoefficientAreaTemp = new SpecialRectOfCoefficient();
	m_dDlEqFirGain = 0.0;
	m_nSampleRate = 0;
	//}}AFX_DATA_INIT
}
CCoefficientDlg::~CCoefficientDlg()
{
    delete m_pCoefficientAreaTemp;
    m_pCoefficientAreaTemp = NULL;
}

void CCoefficientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCoefficientDlg)
	DDX_Control(pDX, IDC_STATIC_DESCRIPT, m_staticDescript);
	DDX_Control(pDX, IDC_EDIT_POWER_ADD_VALUE, m_editPowerAddValue);
	DDX_Control(pDX, IDC_STATIC_COEFF_TOOLBAR_POS, m_staticToolBarPos);
	DDX_Control(pDX, IDC_EDIT_COEFFICIENT, m_editCoefficient);
	DDX_Control(pDX, IDC_STATIC_GRAPHY, m_staticGraphy);
	DDX_Control(pDX, IDC_LIST_PO_VALUE, m_listPo);
	DDX_Text(pDX, IDC_EDIT_COEFFICIENT, m_strCoefficient);
	DDX_Check(pDX, IDC_CHECK_WRITE_IMMEDIATE, m_bWriteImmediately);
	DDX_Text(pDX, IDC_EDIT_POWER_ADD_VALUE, m_dAddValue);
	DDX_Text(pDX, IDC_EDIT_DL_EQ_FIR_GAIN, m_dDlEqFirGain);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCoefficientDlg, CDialog)
	//{{AFX_MSG_MAP(CCoefficientDlg)
	ON_WM_CREATE()
    ON_NOTIFY_EX( TTN_NEEDTEXT, 0, ShowToolTips )
	ON_COMMAND(ID_BUTTON_POWER_ADD_VALUE, OnButtonPowerAddValue)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PO_VALUE, OnItemchangedListPoValue)
	ON_BN_CLICKED(IDC_AUTO_ADJUST, OnAutoAdjust)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_PO, OnButtonUpdatePo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
 


BOOL Hex2Decimal( const CString& _strHexValue, CString& _strDecValue )
{
#define  BASE 16
    int nLength = _strHexValue.GetLength();
    int nPos = 0;
    int nConvertValue =0;
    while( --nLength >= 0 )
    {
        char chValue = _strHexValue.GetAt( nLength );
        int nValue =0;
        if( chValue >= '0' && chValue <= '9' )
        {
            nValue = atoi( &chValue );
        }
        else if( chValue >= 'a' && chValue <='f' )
        {
            nValue = chValue - 'a' + 10;
        }
        else if( chValue >= 'A' && chValue <= 'F' )
        {
            nValue = chValue - 'A' + 10;
        }
        else
        {
            return FALSE;
        }
        
        nConvertValue += (int)pow( BASE, nPos )* nValue;
        ++nPos;
    }
    _strDecValue.Format(_T("%d"),nConvertValue);
    return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CCoefficientDlg message handlers

BOOL CCoefficientDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
    InitToolBar();
    ASSERT( m_pArea );
    *m_pCoefficientAreaTemp = *m_pArea;
    m_staticGraphy.m_pDataSource = m_pCoefficientAreaTemp;
    
    m_listPo.SetEditCheckRange(TRUE,10000, -10000 );
    CRect rect;
    this->GetDlgItem(IDC_STATIC_GRAPHY)->GetWindowRect(&rect);
    ScreenToClient( &rect );
	SetListCtrl();
    InitListData();
    LoadListData();

    CalcDigGainRev( m_dDlEqFirGain,m_pArea->m_dl_fir_eq_gain );

	SetWindowText( m_pArea->m_strName );
    RefreshCoefficientValue( TRUE );
    ChangeFont();
 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCoefficientDlg::SetListCtrl()
{

    DWORD dwStyle = m_listPo.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES ;    
    //dwStyle ^= LVS_EX_HEADERDRAGDROP ;
    dwStyle &= ~LVS_EX_HEADERDRAGDROP;
    m_listPo.SetExtendedStyle( dwStyle );
    
    
    LV_COLUMN	lvc;
    lvc.mask = LVCF_WIDTH | LVCF_TEXT ;
    char columnLabel[3][128];
    strcpy( columnLabel[0], "Po" );	
    strcpy( columnLabel[1], "Value" );		
    strcpy( columnLabel[2], "Sign" );		
    
    int DefaultColumnWidth[] = { 90,50,39 };
//    int nFormat[] = {	LVCFMT_CENTER, LVCFMT_CENTER ,LVCFMT_CENTER};
    for( int i=0; i<3; i++ )
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
        
        m_listPo.InsertColumn( i, &lvc );
    }
        
}

void CCoefficientDlg::InitListData()
{
    m_vAllLinesData.clear();
    for( int i=0; i<ROW_COUNT; ++i )
    {
        ROW_DATA singleLineData; 
        CListCtrlEx::COLUMN_DATA columnData;    
        // row = i, column = 0;
        columnData.m_enShowType = CListCtrlEx::NOT_EDIT;            
        columnData.m_strShowString.Format(_T("PO[%d] (%d)"),i,m_vPowerFrequence[i] );
        columnData.m_lValue = -1;
        columnData.m_dReserved = 0.0;
        columnData.m_nSpinLow=-1;
        columnData.m_nSpinHigh = -1;
        columnData.m_bShowTitle = FALSE;
        columnData.m_enumEditType = CInPlaceEdit::DEFAULT;
        singleLineData.push_back( columnData );


        // row = i, column =1;
        columnData.m_enShowType = CListCtrlEx::ONLY_EDIT_TYPE;
        columnData.m_lValue = (long)(m_pCoefficientAreaTemp->m_vPo.m_vPoValue[i]);
        CString strCoefficent;
        strCoefficent.Format(_T("%.1f"),m_pCoefficientAreaTemp->m_vPo.m_vPoValue[i]);
        columnData.m_strShowString = strCoefficent;
        columnData.m_nSpinLow=0;
        columnData.m_dReserved = m_pCoefficientAreaTemp->m_vPo.m_vPoValue[i];
        columnData.m_nSpinHigh = 9999;
        columnData.m_bShowTitle = FALSE;
        columnData.m_enumEditType = CInPlaceEdit::FLOAT_NUMBER;
        singleLineData.push_back( columnData );

        // row = i, column =2;
        columnData.m_enShowType = CListCtrlEx::COMBOX_TYPE;        
        columnData.m_lValue = m_pCoefficientAreaTemp->m_vPo.m_vPoSign[i];
        if( 0 < columnData.m_lValue)
        {
            columnData.m_strShowString = "+";
        }
        else
        {
            columnData.m_strShowString = "-";
        }
        columnData.m_vComboxStrings.push_back("+");
        columnData.m_vComboxStrings.push_back("-");        
        columnData.m_nSpinLow=-1;
        columnData.m_nSpinHigh = -1;                
        columnData.m_bShowTitle = FALSE;               
        singleLineData.push_back( columnData );        
        
        m_vAllLinesData.push_back( singleLineData );
    }    
}

LRESULT CCoefficientDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
     if( message == WM_LISTEX_COLLUM_INDEX )
        {
        int nRow = m_listPo.GetCurSel();
        int nCol = m_listPo.GetCurSelCol();
        int nSpinLow = m_vAllLinesData[nRow].at(nCol).m_nSpinLow;
        int nSpinHigh = m_vAllLinesData[nRow].at(nCol).m_nSpinHigh;        
        
        m_listPo.SetShowType( m_vAllLinesData[nRow].at( nCol).m_enShowType );
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
            m_listPo.SetSpinTitle( sTitle );
        }    
        else
        {
            m_listPo.SetSpinTitle("");
        }
        
        if( CListCtrlEx::COMBOX_TYPE ==  m_vAllLinesData[nRow].at( nCol ).m_enShowType )
        {
            m_listPo.SetComBoxStrings( m_vAllLinesData[nRow].at( nCol ).m_vComboxStrings );
        }
        m_listPo.SetInplaceEditType( m_vAllLinesData[nRow].at( nCol ).m_enumEditType );
        // set spin range of every cell
        if( -1 != nSpinHigh && -1 != nSpinLow )
            m_listPo.SetSpinRange( nSpinLow, nSpinHigh );
        return TRUE;
    }
    else if( message == WM_INPLACE_KILL_FOCUS )
    {
        CString* pStrValue = (CString*)lParam;
        int nRow = m_listPo.GetCurSel();
        int nCol = m_listPo.GetCurSelCol();               
        if( nRow != -1 && nCol != -1 )
        {
            m_vAllLinesData[ nRow ].at(nCol).m_strShowString = *pStrValue;
#ifdef _UNICODE
			char szText[32] = {0};
			WideCharToMultiByte(CP_ACP,0,*pStrValue,-1,szText,16,NULL,NULL);
			m_vAllLinesData[ nRow ].at(nCol).m_dReserved = atof( szText );
#else
			m_vAllLinesData[ nRow ].at(nCol).m_dReserved  = atof( (LPSTR)(LPCTSTR)*pStrValue );
#endif
            
        }
        RefreshCoefficientValue();
        return TRUE;
    }
    else if ( message == WM_INPLACE_COMBOX )
    {
        LPCOMBOX_DATA pData = (LPCOMBOX_DATA)lParam;
        int nRow = m_listPo.GetCurSel();
        int nCol = m_listPo.GetCurSelCol();      
        ASSERT( nRow >=0 && nRow < (int)m_vAllLinesData.size() );
        
        m_vAllLinesData[nRow].at( nCol ).m_lValue = pData->m_nSelectIdx;
        m_vAllLinesData[nRow].at( nCol ).m_strShowString 
            = m_vAllLinesData[nRow].at( nCol ).m_vComboxStrings[ pData->m_nSelectIdx ];   
        CString sTest(m_vAllLinesData[ nRow ].at(nCol).m_vComboxStrings[pData->m_nSelectIdx]);
        m_vAllLinesData[ nRow ].at(nCol).m_strShowString
            = m_vAllLinesData[ nRow ].at(nCol).m_vComboxStrings[pData->m_nSelectIdx];
        RefreshCoefficientValue();
        return TRUE;
    
    }
	
	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CCoefficientDlg::LoadListData(int iSelect)
{
    m_listPo.DeleteAllItems();
    
    for( UINT i=0; i<m_vAllLinesData.size(); ++i )
    {        
        int nColumnCount = m_vAllLinesData[i].size();
        m_listPo.InsertItem( i,_T("") );
        for( int j=0; j<nColumnCount; ++j )
        {            
            m_listPo.SetItemText( i, j, m_vAllLinesData[i].at( j).m_strShowString );
        }        
    }    
    if( -1 != iSelect)
    {
        m_listPo.SetItemState(iSelect, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);     
        m_listPo.EnsureVisible( iSelect,FALSE );
    }
}

void CCoefficientDlg::OnOK() 
{	
    UpdateData();
    SpecialRect::m_bImmediateWrite = m_bWriteImmediately;

    ASSERT( m_pArea );
 
    m_pCoefficientAreaTemp->CalculateCoefficients();
    m_pCoefficientAreaTemp->CalculateFreqGraphyData();

    *m_pArea = *m_pCoefficientAreaTemp;

    CalcDigGain( m_dDlEqFirGain,m_pArea->m_dl_fir_eq_gain );
    
	CDialog::OnOK();
}

void CCoefficientDlg::RefreshCoefficientValue( BOOL bFirstTime)
{
    ASSERT( m_pArea );
    for( int i=0; i<ROW_COUNT; ++i )
    {
        double dValue = m_vAllLinesData[i].at(1).m_dReserved;
        m_pCoefficientAreaTemp->m_vPo.m_vPoValue[ i ] = dValue;
        int iSign=m_vAllLinesData[i].at(2).m_lValue;
        m_pCoefficientAreaTemp->m_vPo.m_vPoSign[ i ] = iSign;        
    }
    if( !bFirstTime )
    {
        m_pCoefficientAreaTemp->CalculateCoefficients();
    }
    m_pCoefficientAreaTemp->CalculateFreqGraphyData();
    m_strCoefficient = m_pCoefficientAreaTemp->GetToolTips();  
    m_staticGraphy.EraseBK();
    UpdateData( FALSE );
}
void CCoefficientDlg::ChangeFont()
{
    // iterate through and move all child windows and change their font.
    CWnd* pChildWnd = GetWindow(GW_CHILD);
    CFont* pFatherFont = GetParent()->GetFont();
    SetFont(pFatherFont);
    
    while (pChildWnd)
    {
        pChildWnd->SetFont(pFatherFont);
        pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
    }
}

void CCoefficientDlg::InitToolBar()
{
    CRect reResultPos;
    m_staticToolBarPos.GetWindowRect( &reResultPos );
    ScreenToClient( &reResultPos );
    m_staticToolBarPos.ShowWindow( SW_HIDE );
    m_toolBarPower.MoveWindow( &reResultPos );
    m_toolBarPower.GetToolBarCtrl().EnableButton( ID_BUTTON_POWER_ADD_VALUE, FALSE );
     
    m_editPowerAddValue.MoveWindow( reResultPos.right,reResultPos.top+5,50,reResultPos.Height()-5 );
    m_editPowerAddValue.ShowWindow(SW_HIDE);
    m_toolBarPower.ShowWindow( SW_SHOW );
    m_staticDescript.ShowWindow( SW_HIDE );
}

int CCoefficientDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    if (!m_toolBarPower.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
        | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
        !m_toolBarPower.LoadToolBar(IDR_TOOLBAR_COEFF_POWER))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;      // fail to create
    }		// TODO: Add your specialized creation code here
	
	return 0;
}
BOOL CCoefficientDlg::ShowToolTips(UINT id,NMHDR* pTTTStruct,LRESULT* pResult )
{  
	UNREFERENCED_PARAMETER(pResult);
	UNREFERENCED_PARAMETER(id);
    LPTOOLTIPTEXT pData = (LPTOOLTIPTEXT)pTTTStruct;    
    CString sTooltips(MAKEINTRESOURCE(pData->hdr.idFrom));           
    if( sTooltips.GetLength() > 0 )
    {
        _tcscpy( pData->szText,sTooltips);               
        return TRUE;
    }     
    return FALSE;    
}

void CCoefficientDlg::OnButtonPowerAddValue() 
{
	if( !UpdateData() )
        return;
    
    POSITION pos = m_listPo.GetFirstSelectedItemPosition();
    if (pos == NULL)
        TRACE0("No items were selected!\n");
    else
    {        
        CString strValue;
        while (pos)
        {
            int nItem = m_listPo.GetNextSelectedItem(pos);                
            m_pCoefficientAreaTemp->m_vPo.m_vPoValue[ nItem ] += m_dAddValue;      
            strValue.Format(_T("%.1f"),m_pCoefficientAreaTemp->m_vPo.m_vPoValue[ nItem ] );
            m_listPo.SetItemText( nItem,1,strValue);
            m_vAllLinesData[nItem].at(1).m_dReserved = m_pCoefficientAreaTemp->m_vPo.m_vPoValue[ nItem ] ;
        }
        RefreshCoefficientValue();
    }
}

void CCoefficientDlg::OnItemchangedListPoValue(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    POSITION pos =m_listPo.GetFirstSelectedItemPosition();
    if (pos)
    {            
        m_toolBarPower.GetToolBarCtrl().EnableButton( ID_BUTTON_POWER_ADD_VALUE, TRUE );
        m_editPowerAddValue.ShowWindow(SW_SHOW );
        m_staticDescript.ShowWindow( SW_SHOW );
    }
	*pResult = 0;
}

void CCoefficientDlg::OnAutoAdjust() 
{
    CFirAutoDlg dlg;
    dlg.m_nSampleRate = m_nSampleRate;
    if( IDOK == dlg.DoModal() )
    {
        AUDIO_STRUCT _value;

        _value.dl_eq_bass_alpha = (WORD)dlg.m_arrEq[0];
        _value.dl_eq_bass_beta = 	(WORD)dlg.m_arrEq[1];
        _value.dl_eq_bass_gama = (WORD)dlg.m_arrEq[2];
        _value.dl_eq_bass_gain =	(WORD)dlg.m_arrEq[3];
        _value.dl_eq_mid_alpha = (WORD)dlg.m_arrEq[4];
        _value.dl_eq_mid_beta = (WORD)dlg.m_arrEq[5];
        _value.dl_eq_mid_gama = (WORD)dlg.m_arrEq[6];
        _value.dl_eq_mid_gain = (WORD)dlg.m_arrEq[7];
        _value.dl_eq_treble_alpha = (WORD)dlg.m_arrEq[8];
        _value.dl_eq_treble_beta = (WORD)dlg.m_arrEq[9];
        _value.dl_eq_treble_gama = (WORD)dlg.m_arrEq[10];
        _value.dl_eq_treble_gain = (WORD)dlg.m_arrEq[11];
        _value.dl_ng_plk_wpyy_a = (WORD)dlg.m_arrEq[12];
        _value.dl_ng_plk_wpyy_n = (WORD)dlg.m_arrEq[13];
        _value.dl_ng_plk_holdc = (WORD)dlg.m_arrEq[14];
        _value.dl_ng_plk_att = (WORD)dlg.m_arrEq[15];
        _value.dl_ng_clk_wpyy_a = (WORD)dlg.m_arrEq[16]; 
        
        _value.dl_ng_clk_wpyy_n = (WORD)dlg.m_nGain;
        m_dDlEqFirGain = dlg.m_nGain;
    
        for( int i=0;i<DL_FIR_EQ_COUNT;i++ )
        {
            _value.dl_fir_eq_coeff[i] = (WORD)dlg.m_arrEq[i];
        }

        for(  i=0;i<UL_FIR_EQ_COUNT;i++ )
        {
            _value.ul_fir_eq_coeff[i] = (WORD)dlg.m_arrEq[i];
        }
        
        for(  i=0;i<ST_FIR_EQ_COUNT;i++ )
        {
            _value.st_fir_eq_coeff[i] = (WORD)dlg.m_arrEq[i];
        }
        
        m_pArea->SetValue( _value );
        *m_pCoefficientAreaTemp = *m_pArea;
        InitListData();
        RefreshCoefficientValue( TRUE );
        UpdateData( FALSE );
        m_staticGraphy.EraseBK();   
        LoadListData();
    }
}

void CCoefficientDlg::OnButtonUpdatePo() 
{
    m_pCoefficientAreaTemp->CalculatePowerData();
    InitListData();
    LoadListData();
}

BOOL CCoefficientDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	TOOLTIPTEXT *pTip = (TOOLTIPTEXT *)lParam;  

	CString strTip;  
	
	switch(pTip->hdr.code)  
	{  	
	case TTN_NEEDTEXT: 
		{
			switch(pTip->hdr.idFrom)  
			{  
			case ID_BUTTON_AUDIO_WRITE_CURRENT_MODE_TO_PHONE:   
				strTip.LoadString(IDS_WRITE_MS);
				break;  
			case ID_BUTTON_AUDIO_WRITE_CURRENT_MODE_TO_FLASH:  
				strTip.LoadString(IDS_WRITE_FLASH);  
				break;
			default:
				break;
			}  

			_tcscpy(pTip->szText, strTip.GetBuffer(strTip.GetLength()));  
			break;  
		}
	default:
		break;
	}
	
	strTip.ReleaseBuffer( strTip.GetLength() );
	
	return CDialog::OnNotify(wParam, lParam, pResult);
}
