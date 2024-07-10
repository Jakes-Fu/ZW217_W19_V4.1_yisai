// BMPCtrlWithSpecRect.cpp: implementation of the CBMPCtrlWithSpecRect class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Audiotester.h"
#include "BMPCtrlWithSpecRect.h"
#include "ConvertFunc.h"
//#include "UserDefMsg.h"
//#include "ModifyGainDlg.h"
//#include "CoefficientDlg.h"
//#include "IIRCoefficientDlg.h"
//#include "PGADlg.h"
//#include "MathTools.h"
#pragma  warning( push,3 )
#include <complex>
#pragma  warning( pop )
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define Round(d) (d) <= 0 ? (int) ( (d)-0.5 ) : (int)( (d) +0.5 ) 
//BOOL SpecialRect::m_bNeedAlertMessage = TRUE;

BEGIN_MESSAGE_MAP(CBMPCtrlWithSpecRect   , CBitmapCtrl)
    //{{AFX_MSG_MAP(CBitmapCtrl)    
    ON_WM_LBUTTONDOWN()	
    ON_WM_MOUSEMOVE()
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_WM_CREATE()
    ON_WM_RBUTTONDOWN()
    //ON_NOTIFY_EX( TTN_NEEDTEXT   ,  0   ,  OnToolTipNeedText )    
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBMPCtrlWithSpecRect::CBMPCtrlWithSpecRect():m_iIdxOfCurrentArea(-1)   , m_bIsHandCur(FALSE)
{
    m_pChToolTips = new char[MAX_TOOL_TIPS];
    m_pvSpecialRects = NULL;
}

CBMPCtrlWithSpecRect::~CBMPCtrlWithSpecRect()
{
    if( m_pChToolTips )
    {
        delete [] m_pChToolTips;
        m_pChToolTips = NULL;
    }
}

void CBMPCtrlWithSpecRect::OnLButtonDown(UINT nFlags   ,  CPoint point) 
{    
    CBitmapCtrl::OnLButtonDown(nFlags   ,  point);
    int iIdx = -1;
    if( (iIdx = PointInWhichRect(point)) >= 0 )
    {
        ReleaseCapture();   
        
        // tell parent wnd to hand this message
//??        ::PostMessage(GetParent()->GetSafeHwnd()   , USER_MSG_LBUTTON_DOWN_AND_IN_SPEC_AREA   ,  0   , (LPARAM)m_pvSpecialRects->at( iIdx));
    }
     
}
void CBMPCtrlWithSpecRect::OnRButtonDown(UINT nFlags   ,  CPoint point) 
{	
    CBitmapCtrl::OnRButtonDown(nFlags   ,  point);
    ClientToScreen( &point );
//??    ::SendMessage(GetParent()->GetSafeHwnd()   , USER_MSG_RBUTTON_DOWN   ,  nFlags   , (LPARAM)&point );
    
}
void CBMPCtrlWithSpecRect::OnMouseMove(UINT nFlags   ,  CPoint point) 
{
//??    ::PostMessage(GetParent()->GetSafeHwnd()   , USER_MSG_MOUSE_MOVE   , NULL   , (LPARAM)&point);
    
    
    int iIdxArea = -1 ;
    if( (iIdxArea = PointInWhichRect(point) ) >= 0 ) 
    {        
        if( !m_bIsHandCur )
        {            
            m_bIsHandCur = TRUE;
            // set up hand shape of cursor
            SetCursor(LoadCursor(NULL   ,  MAKEINTRESOURCE(32649))); 
            
            CClientDC dc(this);
            CPen pen(PS_DOT   ,  0   ,  RGB(0   ,  0   ,  255));
            CPen* pPenSave = dc.SelectObject(&pen);
            dc.SetBkColor(RGB(192   , 192   , 192));
            
            m_iIdxOfCurrentArea = iIdxArea;
            
            // draw a blue    ,  dot poly line to show the 'hot' area 
            CRect reArea(m_pvSpecialRects->at(iIdxArea)->m_rectArea);
            CPoint ptArray[5] ={ reArea.TopLeft()   , CPoint(reArea.right   , reArea.top)   , 
                                 reArea.BottomRight()   , CPoint(reArea.left   , reArea.bottom)   , reArea.TopLeft() };
                       
            dc.Polyline(ptArray   , 5);
            dc.SelectObject(pPenSave);
            SetCapture();
          
        }
    }  
    else
    {
        if( m_bIsHandCur && m_iIdxOfCurrentArea >=0  )
        {            
            // restore cursor shape
            SetCursor(LoadCursor(NULL   ,  IDC_ARROW));       
            m_bIsHandCur = FALSE;
            CClientDC dc(this);
            CPen pen(PS_DOT   ,  0   ,  RGB(0   ,  0   ,  0));
             dc.SetBkColor(RGB(192   , 192   , 192));
            CPen* pPenSave = dc.SelectObject(&pen);           

            // restore the area when mouse move out 'hot' area
            CRect reArea(m_pvSpecialRects->at(m_iIdxOfCurrentArea)->m_rectArea);
            CPoint ptArray[5] ={ reArea.TopLeft()   , CPoint(reArea.right   , reArea.top)   , 
                reArea.BottomRight()   , CPoint(reArea.left   , reArea.bottom)   , reArea.TopLeft() };
            m_iIdxOfCurrentArea = -1;
            dc.Polyline(ptArray   , 5);
            dc.SelectObject(pPenSave);

            ReleaseCapture();               
        }
    }
        CBitmapCtrl::OnMouseMove(nFlags   ,  point);
}
void CBMPCtrlWithSpecRect::OnHScroll(UINT nSBCode   ,  UINT nPos   ,  CScrollBar* pScrollBar) 
{
    CBitmapCtrl::OnHScroll(nSBCode   , nPos   , pScrollBar );
    int nSmall = 1;
    int nLarge = 20;
    int iCount = m_pvSpecialRects->size();
    
    if(pScrollBar == GetScrollBarCtrl(SB_HORZ))
    {
        if(nSBCode == SB_LEFT || nSBCode == SB_LINELEFT)
        {
            m_iXOffset -= nSmall;
            for( int i=0; i<iCount; ++i )
            {
               m_pvSpecialRects->at( i )->m_rectArea -= CPoint(-2*nSmall   , 0);
               m_pvSpecialRects->at( i )->m_rectValueArea = m_pvSpecialRects->at( i )->m_rectArea;
            }                      
        }
        if(nSBCode == SB_PAGELEFT)
        {
            m_iXOffset -= nLarge;
            for( int i=0; i<iCount; ++i )
            {
                m_pvSpecialRects->at( i )->m_rectArea -= CPoint(-2*nLarge   , 0);
                m_pvSpecialRects->at( i )->m_rectValueArea = m_pvSpecialRects->at( i )->m_rectArea;
            }            
        }
        if(nSBCode == SB_RIGHT || nSBCode == SB_LINERIGHT)
        {
            for( int i=0; i<iCount; ++i )
            {
                m_pvSpecialRects->at( i )->m_rectArea += CPoint(-2*nSmall   , 0);
                m_pvSpecialRects->at( i )->m_rectValueArea = m_pvSpecialRects->at( i )->m_rectArea;
            }
            m_iXOffset += nSmall;
        }
        if(nSBCode == SB_PAGERIGHT)
        {
            for( int i=0; i<iCount; ++i )
            {
                m_pvSpecialRects->at( i )->m_rectArea += CPoint(-2*nLarge   , 0);
                m_pvSpecialRects->at( i )->m_rectValueArea = m_pvSpecialRects->at( i )->m_rectArea;
            }
            m_iXOffset += nLarge;
        }
        if(nSBCode == SB_THUMBPOSITION)
        {
            m_iXOffset = (int)nPos;            
            for( int i=0; i<iCount; ++i )
            {
                m_pvSpecialRects->at( i )->m_rectArea.left =m_pvSpecialRects->at( i )->m_rectDefaultArea.left - nPos ;
                m_pvSpecialRects->at( i )->m_rectArea.right =m_pvSpecialRects->at( i )->m_rectArea.left 
                                                            + m_pvSpecialRects->at( i )->m_rectDefaultArea.Width();  
            }
        }
        if(nSBCode == SB_THUMBTRACK)
        {
            m_iXOffset = (int)nPos;
            for( int i=0; i<iCount; ++i )
            {
                m_pvSpecialRects->at( i )->m_rectValueArea.left =m_pvSpecialRects->at( i )->m_rectDefaultArea.left - nPos ;
                m_pvSpecialRects->at( i )->m_rectValueArea.right =m_pvSpecialRects->at( i )->m_rectValueArea.left 
                    + m_pvSpecialRects->at( i )->m_rectDefaultArea.Width();                 

            }
            
        }
        if (m_iXOffset<0)
        {
            m_iXOffset = 0;

            for( int i=0; i<iCount; ++i )
            {
                m_pvSpecialRects->at( i )->m_rectArea.left = m_pvSpecialRects->at( i )->m_rectDefaultArea.left;
                m_pvSpecialRects->at( i )->m_rectArea.right = m_pvSpecialRects->at( i )->m_rectDefaultArea.right;

                m_pvSpecialRects->at( i )->m_rectValueArea = m_pvSpecialRects->at( i )->m_rectArea;
            }
        }
        if (m_iXOffset>m_sizeImage.cx - m_rcClient.Width())
        {
            m_iXOffset = m_sizeImage.cx - m_rcClient.Width(); 
            for( int i=0; i<iCount; ++i )
            {
                m_pvSpecialRects->at( i )->m_rectArea.left = m_pvSpecialRects->at( i )->m_rectDefaultArea.left - m_iXOffset;
                m_pvSpecialRects->at( i )->m_rectArea.right = m_pvSpecialRects->at( i )->m_rectDefaultArea.left 
                                                            + m_pvSpecialRects->at( i )->m_rectDefaultArea.Width(); 
                m_pvSpecialRects->at( i )->m_rectValueArea = m_pvSpecialRects->at( i )->m_rectArea;
            }
        }
        SetScrollPos(SB_HORZ   ,  m_iXOffset   ,  TRUE);
        DrawBitmap();
    }
}

void CBMPCtrlWithSpecRect::OnVScroll(UINT nSBCode   ,  UINT nPos   ,  CScrollBar* pScrollBar) 
{
    CBitmapCtrl::OnVScroll(nSBCode   , nPos   , pScrollBar );
    int nSmall = 1;
    int nLarge = 20;
    int iCount = m_pvSpecialRects->size();
    if(pScrollBar == GetScrollBarCtrl(SB_VERT))
    {
        if(nSBCode == SB_TOP || nSBCode == SB_LINEUP)
        {
            m_iYOffset -= nSmall;

            for( int i=0; i<iCount; ++i )
            {
                m_pvSpecialRects->at( i )->m_rectArea -= CPoint(0   , -2*nSmall);
                m_pvSpecialRects->at( i )->m_rectValueArea = m_pvSpecialRects->at( i )->m_rectArea;
            }
        }
        if(nSBCode == SB_PAGEUP)
        {
            m_iYOffset -= nLarge;

            for( int i=0; i<iCount; ++i )
            {
                m_pvSpecialRects->at( i )->m_rectArea -= CPoint(0   , -2*nLarge);
                m_pvSpecialRects->at( i )->m_rectValueArea = m_pvSpecialRects->at( i )->m_rectArea;
            }
        }
        if(nSBCode == SB_BOTTOM || nSBCode == SB_LINEDOWN)
        {            
            m_iYOffset += nSmall;       

            for( int i=0; i<iCount; ++i )
            {
                m_pvSpecialRects->at( i )->m_rectArea += CPoint(0   , -2*nSmall);
                m_pvSpecialRects->at( i )->m_rectValueArea = m_pvSpecialRects->at( i )->m_rectArea;
            }
        }
        if(nSBCode == SB_PAGEDOWN)
        {            
            m_iYOffset += nLarge;
            for( int i=0; i<iCount; ++i )
            {
                m_pvSpecialRects->at( i )->m_rectArea += CPoint(0   , -2*nLarge);
                m_pvSpecialRects->at( i )->m_rectValueArea = m_pvSpecialRects->at( i )->m_rectArea;
            }
        }
        if(nSBCode == SB_THUMBPOSITION)
        {
            m_iYOffset = (int)nPos;   
            for( int i=0; i<iCount; ++i )
            {
                m_pvSpecialRects->at( i )->m_rectArea.top = m_pvSpecialRects->at( i )->m_rectDefaultArea.top - nPos;
                m_pvSpecialRects->at( i )->m_rectArea.bottom = m_pvSpecialRects->at( i )->m_rectArea.top 
                                                              +m_pvSpecialRects->at( i )->m_rectDefaultArea.Height();

                m_pvSpecialRects->at( i )->m_rectValueArea = m_pvSpecialRects->at( i )->m_rectArea;
            }
        }
        if(nSBCode == SB_THUMBTRACK)
        {
            m_iYOffset = (int)nPos;
            for( int i=0; i<iCount; ++i )
            {
                m_pvSpecialRects->at( i )->m_rectValueArea.top =m_pvSpecialRects->at( i )->m_rectDefaultArea.top - nPos ;
                m_pvSpecialRects->at( i )->m_rectValueArea.bottom =m_pvSpecialRects->at( i )->m_rectValueArea.top 
                    + m_pvSpecialRects->at( i )->m_rectDefaultArea.Width();                 
                
            }
        }

        if (m_iYOffset<0)
        {
            m_iYOffset = 0;
            for( int i=0; i<iCount; ++i )
            {
                m_pvSpecialRects->at( i )->m_rectArea.top = m_pvSpecialRects->at( i )->m_rectDefaultArea.top;
                m_pvSpecialRects->at( i )->m_rectArea.bottom = m_pvSpecialRects->at( i )->m_rectDefaultArea.bottom;     
                m_pvSpecialRects->at( i )->m_rectValueArea = m_pvSpecialRects->at( i )->m_rectArea;
            }
        }
        if (m_iYOffset>m_sizeImage.cy - m_rcClient.Height())
        {
            m_iYOffset = m_sizeImage.cy - m_rcClient.Height();      
            for( int i=0; i<iCount; ++i )
            {
                m_pvSpecialRects->at( i )->m_rectArea.top = m_pvSpecialRects->at( i )->m_rectDefaultArea.top - m_iYOffset;
                m_pvSpecialRects->at( i )->m_rectArea.bottom = m_pvSpecialRects->at( i )->m_rectArea.top
                                                            + m_pvSpecialRects->at( i )->m_rectDefaultArea.Height();

                m_pvSpecialRects->at( i )->m_rectValueArea = m_pvSpecialRects->at( i )->m_rectArea;
            }
        }
        
        SetScrollPos(SB_VERT   ,  m_iYOffset   ,  TRUE);
        DrawBitmap();
    }
}
//paint specific value    ,  unit of every rect
void CBMPCtrlWithSpecRect::PaintSpecialValue(CDC* pDC) 
{    
    if( NULL == m_pvSpecialRects )
    {
        return;
    }

    int iCount = m_pvSpecialRects->size();
//    int iCountOfArea = SpecialRect::m_pAreaList->size();
    SpecialRect* pArea = NULL;
    for( int i=0; i<iCount; ++i )
    {
        //int iType = (int)m_pvSpecialRects->at( i )->m_enumParaType;
       // pArea = SpecialRect::m_pAreaList->at( iType );
        pArea = m_pvSpecialRects->at(i);
        ASSERT( pArea );
        pDC->TextOut(pArea->m_rectValueArea.right+1   , 
                     pArea->m_rectValueArea.top+5    , 
                     pArea->m_strOtherValue+pArea->m_strUnit );  
    }    
}

//test to see if the test point is in any rect   ,  if yes   ,  return its idx    , otherwise return -1
int CBMPCtrlWithSpecRect::PointInWhichRect(const CPoint& ptTest )const
{
    if( NULL == m_pvSpecialRects )
    {
        return -1;
    }

    int iCount = m_pvSpecialRects->size();
    for( int i=0; i<iCount; ++i )
    {
        if( m_pvSpecialRects->at( i )->m_rectArea.PtInRect(ptTest) )
        {
            return i;
        }
    }
    return -1;
}
int CBMPCtrlWithSpecRect::OnToolHitTest( CPoint point   ,  TOOLINFO* pTI ) const
{    
    int iIdx = PointInWhichRect( point);
    if( iIdx >= 0 )
    {
        pTI->hwnd = m_hWnd;
        pTI->uId = 0;
        pTI->uFlags = 0;
        GetClientRect(&(pTI->rect));      
        //tell windows that i want to provide the tool tip string by call back 
        pTI->lpszText = LPSTR_TEXTCALLBACK;      
       return 1;
    }
    else
    {
        return -1;
    }
    
}
int CBMPCtrlWithSpecRect::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CBitmapCtrl::OnCreate(lpCreateStruct) == -1)
        return -1;
    EnableToolTips(TRUE);
    return 0;
}

BOOL CBMPCtrlWithSpecRect::OnNotify(WPARAM wParam   ,  LPARAM lParam   ,  LRESULT* pResult) 
{
    
    if( TTN_GETDISPINFO  == (((LPNMHDR) lParam)->code)  )
    {    // need provide tooltips now     
        ASSERT( m_iIdxOfCurrentArea >= 0 && m_iIdxOfCurrentArea< (int)(m_pvSpecialRects->size()) );
        CString strTest(m_pvSpecialRects->at(m_iIdxOfCurrentArea)->GetToolTips());
        int ilen = strTest.GetLength();
        
        memcpy(m_pChToolTips   , strTest   , ilen );
        m_pChToolTips[ilen] = '\0';
        LPNMTTDISPINFO lpttd = (LPNMTTDISPINFO)lParam;
        ::SendMessage(((LPNMHDR) lParam)->hwndFrom   ,  TTM_SETMAXTIPWIDTH   ,  0   ,  700);  
#ifdef _UNICODE
		TCHAR* pszTmp = new TCHAR[ilen];
		MultiByteToWideChar(CP_ACP,0,m_pChToolTips,-1,pszTmp,ilen+1);
        lpttd->lpszText = pszTmp; 
		if(pszTmp != NULL)
		{
			delete [] pszTmp;
			pszTmp = NULL;
		}
#else
		lpttd->lpszText = m_pChToolTips;
#endif  
        return 0;
    }
    
    
    return CBitmapCtrl::OnNotify(wParam   ,  lParam   ,  pResult);
}

/**************class SpecialRect ******************************************/
/*************************************************************************/
#define  CHANGE_COEFFICIENT 4096.0
BOOL SpecialRect::m_bImmediateWrite = FALSE;
// formate tool tips string
CString SpecialRect::GetToolTips()const
{
	int iDBValue = 0;
#ifdef _UNICODE
	char szTmp[16] = {0};
	WideCharToMultiByte(CP_ACP,0,m_strOtherValue,-1,szTmp,16,NULL,NULL);
    iDBValue = atoi(szTmp);
#else
	iDBValue = atoi( (LPSTR)(LPCTSTR)m_strOtherValue) ;
#endif 
    double dShowValue = pow(10.0   , iDBValue/20.0);
    dShowValue *= CHANGE_COEFFICIENT;
    CString strShowValue;
    strShowValue.Format(_T("0x%X")   , (int)dShowValue );
    return m_strName +" " +strShowValue +" (" + m_strOtherValue + m_strUnit +")";
}
//restore to default value   , especily set the m_rectArea as m_rectDefaultArea
//cause when we scroll the window   ,  we will change the value of m_rectArea
void SpecialRect::SetToDefault()
{
    m_rectArea = m_rectDefaultArea;
    m_rectValueArea = m_rectDefaultArea;
}

void SpecialRect::ReadFromFile(CArchive &file)
{
    Read( file );
}
void SpecialRect::WriteToFile(CArchive &file) const
{
    Write( file );
}
void SpecialRect::FetchValue(AUDIO_MODE& _value)const
{
    Fetch( _value );
}
void SpecialRect::Read(CArchive &file)
{
    float fValue = 0 ;
    file >> fValue;
    m_strOtherValue.Format(_T("%.1f")   , fValue );
    TRACE(_T("\n SpecialRect::read %.2f\n")   , fValue );
}

void SpecialRect::Write(CArchive &file)const
{
	float fValue = 0.0;
#ifdef _UNICODE
	char szText[32] = {0};
	WideCharToMultiByte(CP_ACP,0,m_strOtherValue,-1,szText,16,NULL,NULL);
	fValue = atof( szText );
#else
	fValue = (float)atof( (LPSTR)(LPCTSTR)m_strOtherValue );
#endif
    
    file << fValue;
    TRACE(_T("\n SpecialRect::write %.2f\n")   , fValue );
}

void SpecialRect::Fetch(AUDIO_MODE& _value)const
{
	float fValue = 0.0;
#ifdef _UNICODE
	char szText[32] = {0};
	WideCharToMultiByte(CP_ACP,0,m_strOtherValue,-1,szText,16,NULL,NULL);
	fValue = atof( szText );
#else
	fValue = (float)atof( (LPSTR)(LPCTSTR)m_strOtherValue );
#endif
    
    switch( m_enumParaType )
    {       
        case digital_sidetone_gain:
            ::CalcDigGain( fValue   , _value.audio_data.digital_sidetone_gain );
            break;
        default :
            ASSERT(0);
    }
}

void SpecialRect::SetValue( AUDIO_STRUCT& value )
{
    double fValue = 0.0;
    
    switch( m_enumParaType )
    {       
    case digital_sidetone_gain:
        ::CalcDigGainRev( fValue   , value.digital_sidetone_gain );
        break;
    default :
        ASSERT(0);
    }

    m_strOtherValue.Format( _T("%lf")   , fValue );
}

int SpecialRect::ClickAction(CWnd* pParent)
{
	UNREFERENCED_PARAMETER(pParent);
    //??
    /*
    CModifyGainDlg dlg(pParent);    
    dlg.m_pArea = this;
    dlg.m_strParaName = m_strName;
    dlg.m_strUnit = m_strUnit;
    dlg.m_fGain = atof(m_strOtherValue);  
    return dlg.DoModal(); 
    */
    return 0;
}

/*********************class SpecialRectOfVolumeCtrl************************************************/
/*******************************************************************************************/
int SpecialRectOfVolumeCtrl::m_iArrayVolume[VOLUME_COUNT] = 
{
    -9   , -6   , -3   , 0   , 3   , 6   , 9   , 12   , 15
};

SpecialRectOfVolumeCtrl::SpecialRectOfVolumeCtrl()
:m_iValue(0)
{
	//teana hu 2011.05.09
	m_nLevel = VOLUME_COUNT;
	//
     
}
 //pop up a dialog to modify related value    , such as m_iValue
int SpecialRectOfVolumeCtrl::ClickAction(CWnd* pParent)
{
	UNREFERENCED_PARAMETER(pParent);
    /*
      CPGADlg dlg( pParent );
      dlg.m_pPGAArea = this;
      return dlg.DoModal();
    */
    return 0;
}
//get DA/AD gain value string
void SpecialRectOfVolumeCtrl::FormatValueString( std::vector<CString>&vValue )const
{
    int iSize = 0;
    int* pArray = NULL;
    if( m_enumParaType == volume_ctrl )
    {
        iSize = m_nLevel;
        pArray = (int*)m_iArrayVolume;
    }
   
    for( int i=iSize-1; i>=0; --i )
    {
        CString strValue;
        int iIdx = i;
        if( m_enumParaType == volume_ctrl )
        {
            ++iIdx;
        }
//        int iValue = pArray[i];

		//teana hu 2009.09.01
        strValue.Format(_T("0X%02X volume level %d")   , iIdx   , i + 1 );
        vValue.push_back( strValue );
    }
}
CString SpecialRectOfVolumeCtrl::GetToolTips()const
{
    std::vector<CString>vString;
    FormatValueString( vString );
    int iSize = vString.size();
    ASSERT(m_iValue>=0 && m_iValue < iSize );
    
    return m_strName + " " + vString[ iSize - m_iValue -1 ];
}
//whenever user modify the m_iValue   ,  re-caculate all related value   ,  such as m_strOtherValue
void SpecialRectOfVolumeCtrl::ReCaculate()
{    
    int iSize = 0;
    int* pArray = NULL;
    if( volume_ctrl == m_enumParaType )
    {
        iSize = m_nLevel;
        pArray = m_iArrayVolume;
    }
   
    ASSERT( m_iValue >=0 && m_iValue < iSize ); 
    if( INFINITE == pArray[m_iValue] )//lint !e661
    {
         m_strOtherValue= "-INFINITE";
    }
    else
    {
        if( iSize -1 == m_iValue )
        {
            //m_strOtherValue.Format("MAX %d"   , pArray[ m_iValue ] );
           m_strOtherValue.Format(_T("%ddb MAX")   , pArray[ m_iValue ] ); 
        }
        else if( 0 == m_iValue )
        {
            m_strOtherValue.Format(_T("%ddb MIN ")   , pArray[ m_iValue ] );
        }
        else if( iSize / 2  == m_iValue  )
        {
            m_strOtherValue.Format(_T("%ddb Normal ")   , pArray[ m_iValue ] );
        }       
        else
        {
            m_strOtherValue.Format(_T("%ddb")   , pArray[ m_iValue ] );//lint !e661
        }
    }
   
}

void SpecialRectOfVolumeCtrl::Write( CArchive& file )const
{
    file << m_enumParaType;
    file <<m_iValue;
    TRACE(_T("\nSpecialRectOfVolumeCtrl::Write %d   ,  %d \n" )  , m_enumParaType   , m_iValue );
}
void SpecialRectOfVolumeCtrl::Read( CArchive& file )
{
    int iType =0;
    file >> iType;
    m_enumParaType = (AUDIO_PARA_TYPE)iType;
    file >> m_iValue;
    TRACE(_T("\n SpecialRectOfVolumeCtrl::read %d   , %d\n")   , iType   , m_iValue);
    ReCaculate();
}
void SpecialRectOfVolumeCtrl::Fetch(AUDIO_MODE& )const
{
    // Do nothing
}

void SpecialRectOfVolumeCtrl::SetValue( AUDIO_STRUCT&  )
{
    // Do nothing
}

/*********************class SpecialRectOfEQ************************************************/
/*******************************************************************************************/

double SpecialRectOfEQ::m_dMinXValue = 100.0;
double SpecialRectOfEQ::m_dMaxXValue = 4000.0;
double SpecialRectOfEQ::m_dXScale[X_SCALE_COUNT] =
{
0.025   , 0.05   , 0.075   , 0.125   , 0.175   , 0.25   , 0.5   , 0.75   , 1
};

SpecialRectOfEQ::SpecialRectOfEQ()
{
    m_wBassAlpha = 0;
    m_wBassBeta = 0;
    m_wBassGain = 0;
    m_wBassGama = 0;
    m_dDbBass = 0;
    m_dDbTreble = 0;
    m_dDfBass = 0;
    m_dDfTreble = 0;
    m_dF0Bass = 0;
    m_dF0Treble = 0;
    m_dGainBass = 0;
    m_dGainTreble = 0;
    m_wTrebleAlpha = 0;
    m_wTrebleBeta = 0;
    m_wTrebleGain = 0;
    m_wTrebleGama = 0;
    
    m_wMidAlpha = 0;
    m_wMidBeta = 0;
    m_wMidGain = 0;
    m_wMidGama = 0;
    m_dDbMid = 0;
    m_dDfMid = 0;
    m_dF0Mid = 0;
    m_dGainMid = 0;

    m_pSampleRate = NULL;
}

void SpecialRectOfEQ::CalculateFreqGraphyData( int nXScaleCount /*=400*/ )
{
    m_vAllFreqItem.clear();
    m_vAllBassFreqItem.clear();
    m_vAllTrebleFreqItem.clear();
    m_vAllMidFreqItem.clear();

    double dx = pow( m_dMaxXValue / m_dMinXValue   , 1.0 / nXScaleCount );
    double x = m_dMinXValue;

    for( int i=0;i<nXScaleCount;i++ )
    {
        FreqItem tempItem;
        double treble   , bass   , mid = 0;
        tempItem.first = log10(x);
        treble = ::CalcFrequencyResponse( m_wTrebleAlpha   , 
                                          m_wTrebleBeta   , 
                                          m_wTrebleGama   , 
                                          m_wTrebleGain   , 
                                          x   , 
                                          *m_pSampleRate
                                        );
        bass = ::CalcFrequencyResponse( m_wBassAlpha   , 
                                        m_wBassBeta   , 
                                        m_wBassGama   , 
                                        m_wBassGain   , 
                                        x   , 
                                        *m_pSampleRate
                                      );

        if( dl_eq == m_enumParaType )
        {
            mid = ::CalcFrequencyResponse( m_wMidAlpha   , 
                                           m_wMidBeta   , 
                                           m_wMidGama   , 
                                           m_wMidGain   , 
                                           x   , 
                                           *m_pSampleRate
                                         );            
        }
                                  
        tempItem.second = ::CalcFrequencyResponse( bass   , treble   , mid );

        m_vAllFreqItem.push_back( tempItem );
        tempItem.second = treble;
        m_vAllTrebleFreqItem.push_back( tempItem );
        tempItem.second = bass;
        m_vAllBassFreqItem.push_back( tempItem );
        
        if( dl_eq == m_enumParaType )
        {
            tempItem.second = mid;
            m_vAllMidFreqItem.push_back( tempItem );
        }

        x *= dx;
    }
}

void SpecialRectOfEQ::Write( CArchive& file ) const
{
    file << m_dF0Bass;
    file << m_dDbBass;
    file << m_dDfBass;
    file << m_dGainBass;

    file << m_dF0Mid;
    file << m_dDbMid;
    file << m_dDfMid;
    file << m_dGainMid;

    file << m_dF0Treble;
    file << m_dDbTreble;
    file << m_dDfTreble;
    file << m_dGainTreble;
}

void SpecialRectOfEQ::Read( CArchive& file )
{
    file >> m_dF0Bass;
    file >> m_dDbBass;
    file >> m_dDfBass;
    file >> m_dGainBass;
    
    file >> m_dF0Mid;
    file >> m_dDbMid;
    file >> m_dDfMid;
    file >> m_dGainMid;

    file >> m_dF0Treble;
    file >> m_dDbTreble;
    file >> m_dDfTreble;
    file >> m_dGainTreble;
}

void SpecialRectOfEQ::Fetch(AUDIO_MODE& _value)const
{
    switch( m_enumParaType )
    {
    case dl_eq:
        _value.audio_data.dl_eq_bass_alpha = m_wBassAlpha;
        _value.audio_data.dl_eq_bass_beta = m_wBassBeta;
        _value.audio_data.dl_eq_bass_gama = m_wBassGama;
        _value.audio_data.dl_eq_bass_gain = m_wBassGain;
        _value.audio_data.dl_eq_mid_alpha = m_wMidAlpha;
        _value.audio_data.dl_eq_mid_beta = m_wMidBeta;
        _value.audio_data.dl_eq_mid_gama = m_wMidGama;
        _value.audio_data.dl_eq_mid_gain = m_wMidGain;
        _value.audio_data.dl_eq_treble_alpha = m_wTrebleAlpha;
        _value.audio_data.dl_eq_treble_beta = m_wTrebleBeta;
        _value.audio_data.dl_eq_treble_gama = m_wTrebleGama;
        _value.audio_data.dl_eq_treble_gain = m_wTrebleGain;
        break;
    case ul_eq:
        _value.audio_data.ul_eq_bass_alpha = m_wBassAlpha;
        _value.audio_data.ul_eq_bass_beta = m_wBassBeta;
        _value.audio_data.ul_eq_bass_gama = m_wBassGama;
        _value.audio_data.ul_eq_bass_gain = m_wBassGain;
        _value.audio_data.ul_eq_treble_alpha = m_wTrebleAlpha;
        _value.audio_data.ul_eq_treble_beta = m_wTrebleBeta;
        _value.audio_data.ul_eq_treble_gama = m_wTrebleGama;
        _value.audio_data.ul_eq_treble_gain = m_wTrebleGain;
        break;
    default:
        ASSERT(0);
        break;
    }
}

void SpecialRectOfEQ::SetValue( AUDIO_STRUCT& value )
{
    switch( m_enumParaType )
    {
    case dl_eq:
        m_wBassAlpha = value.dl_eq_bass_alpha;
        m_wBassBeta = value.dl_eq_bass_beta;
        m_wBassGama = value.dl_eq_bass_gama;
        m_wBassGain = value.dl_eq_bass_gain;
        m_wMidAlpha = value.dl_eq_mid_alpha;
        m_wMidBeta = value.dl_eq_mid_beta;
        m_wMidGama = value.dl_eq_mid_gama;
        m_wMidGain = value.dl_eq_mid_gain;
        m_wTrebleAlpha = value.dl_eq_treble_alpha;
        m_wTrebleBeta = value.dl_eq_treble_beta;
        m_wTrebleGama = value.dl_eq_treble_gama;
        m_wTrebleGain = value.dl_eq_treble_gain;
        break;
    case ul_eq:
        m_wBassAlpha = value.ul_eq_bass_alpha;
        m_wBassBeta = value.ul_eq_bass_beta;
        m_wBassGama = value.ul_eq_bass_gama;
        m_wBassGain = value.ul_eq_bass_gain;
        m_wTrebleAlpha = value.ul_eq_treble_alpha;
        m_wTrebleBeta = value.ul_eq_treble_beta;
        m_wTrebleGama = value.ul_eq_treble_gama;
        m_wTrebleGain = value.ul_eq_treble_gain;
        break;
    default:
        ASSERT(0);
        break;
    }

    alpha2df( m_dDfTreble   , m_wTrebleAlpha   , *m_pSampleRate );
    beta2f0( m_dF0Treble   , m_wTrebleBeta   , *m_pSampleRate );
    gama2db( m_dDbTreble   , m_wTrebleGama );
    rgain2gain( m_dGainTreble   , m_wTrebleGain );
    
    if( dl_eq == m_enumParaType )
    {
        alpha2df( m_dDfMid   , m_wMidAlpha   , *m_pSampleRate );
        beta2f0( m_dF0Mid   , m_wMidBeta   , *m_pSampleRate );
        gama2db( m_dDbMid   , m_wMidGama );
        rgain2gain( m_dGainMid   , m_wMidGain );
    }

    alpha2df( m_dDfBass   , m_wBassAlpha   , *m_pSampleRate );
    beta2f0( m_dF0Bass   , m_wBassBeta   , *m_pSampleRate );
    gama2db( m_dDbBass   , m_wBassGama );
    rgain2gain( m_dGainBass   , m_wBassGain );
}

//teana hu 2010.06.11
/*********************class SpecialRectOfEQMode************************************************/
/*******************************************************************************************/

double SpecialRectOfEQMode::m_dMinXValue = 1;
double SpecialRectOfEQMode::m_dMaxXValue = 22050.0;
double SpecialRectOfEQMode::m_dXScale[X_SCALE_MODE_COUNT] =
{
  0.00004535, 0.000907, 0.00227,   0.004535 , 0.00907   , 0.01814   , 0.0272   , 0.0363   , 0.04535   , 0.11337   , 0.1814   , 0.2721   , 0.72565    , 1
};

SpecialRectOfEQMode::SpecialRectOfEQMode()
{
    m_wBassf0 = 0;
    m_wBassq = 0;
    m_wBassboost = 0;
    m_wBassgain = 0;
	m_wTreblef0 = 0;
    m_wTrebleq = 0;
    m_wTrebleboost = 0;
    m_wTreblegain = 0;
	m_wf1_g0 = 0;
    m_wf1_g1 = 0;
	m_wf1_fp = 0;
    m_wf2_g0 = 0;
    m_wf2_g1 = 0;
    m_wf2_fp = 0;
	m_fEQGain = 0;
}


void SpecialRectOfEQMode::CalculateFreqGraphyData( int nXScaleCount /*=22049*/ )
{
    m_vAllFreqItem.clear();
    m_vAllBassFreqItem.clear();
    m_vAllTrebleFreqItem.clear();
    m_vAllLcfFreqItem.clear();

    double dx = pow( m_dMaxXValue / m_dMinXValue   , 1.0 / nXScaleCount );
    double x = m_dMinXValue;
	IIR_FILTER_PARA_T filter_para_lcf;
	Filter_CalcF1f1Coeffs(m_wf1_g0, m_wf1_g1, m_wf1_fp, m_wf2_g0, m_wf2_g1, m_wf2_fp, SAMPLE_FREQUENCY, &filter_para_lcf);
	IIR_FILTER_PARA_T filter_para_bass;
	Filter_CalcEqCoeffs(m_wBassf0, m_wBassq, m_wBassboost, m_wBassgain, SAMPLE_FREQUENCY, &filter_para_bass);
	IIR_FILTER_PARA_T filter_para_treble;
	Filter_CalcEqCoeffs(m_wTreblef0, m_wTrebleq, m_wTrebleboost, m_wTreblegain, SAMPLE_FREQUENCY, &filter_para_treble);

    for( int i=0;i<nXScaleCount;i++ )
    {
        FreqItem tempItem;
        double treble   , bass   , lcf = 0;
		tempItem.first = log10(x);
		
		
		lcf = ::CalcFrequencyResponse(filter_para_lcf.A0,
										filter_para_lcf.A1,
										filter_para_lcf.A2,
										filter_para_lcf.B0,
										filter_para_lcf.B1,
										filter_para_lcf.B2,
										x);
		
		bass = ::CalcFrequencyResponse(filter_para_bass.A0,
										filter_para_bass.A1,
										filter_para_bass.A2,
										filter_para_bass.B0,
										filter_para_bass.B1,
										filter_para_bass.B2,
										x);
		
		treble = ::CalcFrequencyResponse(filter_para_treble.A0,
										filter_para_treble.A1,
										filter_para_treble.A2,
										filter_para_treble.B0,
										filter_para_treble.B1,
										filter_para_treble.B2,
										x);
                                  
        tempItem.second = ::CalcFrequencyResponse( bass   , treble   , lcf );

		//teana hu 2011.03.24
		tempItem.second += m_fEQGain;
		//

        m_vAllFreqItem.push_back( tempItem );
        tempItem.second = treble;
        m_vAllTrebleFreqItem.push_back( tempItem );
        tempItem.second = bass;
        m_vAllBassFreqItem.push_back( tempItem );
        
        tempItem.second = lcf;
        m_vAllLcfFreqItem.push_back( tempItem );

        x *= dx;
    }
}


void SpecialRectOfEQMode::Fetch(EQ_MODE_NV_PARAM_T& _value)const
{
   _value.lcf_filter.f1_g0 = m_wf1_g0;
   _value.lcf_filter.f1_g1 = m_wf1_g1;
   _value.lcf_filter.f1_fp = m_wf1_fp;
   _value.lcf_filter.f2_g0 = m_wf2_g0;
   _value.lcf_filter.f2_g1 = m_wf2_g1;
   _value.lcf_filter.f2_fp = m_wf2_fp;
   _value.eq_bass.bass_f0 = m_wBassf0;
   _value.eq_bass.bass_q = m_wBassq;
   _value.eq_bass.bass_boostdB = m_wBassboost;
   _value.eq_bass.bass_gaindB = m_wBassgain;
   _value.eq_treble.treble_f0 = m_wTreblef0;
   _value.eq_treble.treble_q = m_wTrebleq;
   _value.eq_treble.treble_boostdB = m_wTrebleboost;
   _value.eq_treble.treble_gaindB = m_wTreblegain;
   _value.agc_in_gain = (double)pow(10, m_fEQGain/20.0) * 4096;
}

void SpecialRectOfEQMode::SetValue( EQ_MODE_NV_PARAM_T& _value )
{
	m_wf1_g0 = _value.lcf_filter.f1_g0;
	m_wf1_g1 = _value.lcf_filter.f1_g1;
	m_wf1_fp = _value.lcf_filter.f1_fp;
	m_wf2_g0 = _value.lcf_filter.f2_g0;
	m_wf2_g1 = _value.lcf_filter.f2_g1;
	m_wf2_fp = _value.lcf_filter.f2_fp;
	m_wBassf0 = _value.eq_bass.bass_f0;
	m_wBassq = _value.eq_bass.bass_q;
	m_wBassboost = _value.eq_bass.bass_boostdB;
	m_wBassgain = _value.eq_bass.bass_gaindB;
	m_wTreblef0 = _value.eq_treble.treble_f0;
	m_wTrebleq = _value.eq_treble.treble_q;
	m_wTrebleboost = _value.eq_treble.treble_boostdB;
	m_wTreblegain = _value.eq_treble.treble_gaindB;
	m_fEQGain = 20 * (double)log10((double)(_value.agc_in_gain/(double)4096.0));
	m_fEQGain = (short)(m_fEQGain * 10) / 10.0;
}
//

/*********************class SpecialRectOfAgc************************************************/
/*******************************************************************************************/
SpecialRectOfAgc::SpecialRectOfAgc()
{
    m_iValue = 0;
    m_enumParaType = agc_switch;
	m_bAudio4 = FALSE;
	m_bAudio41 = FALSE;
	m_nALCDPthreshold = 0;
	m_nALCcprsThreshold = 0;
	m_nALCcprsLimit = 0;
	m_nALCexpdThreshold = 0;
	m_nALCexpdLimit = 0;
	m_nALCcprsRatio = 0;
	m_nALCexpdLimit = 0;
}

void SpecialRectOfAgc::SetValue( AUDIO_STRUCT& value )
{
	if(value.reserved5[4] & 0x1 == 1 && value.reserved1 >= 0x41)
	{
		m_bAudio41 = TRUE;
		m_bAudio4 = FALSE;
		m_nALCcprsThreshold = value.reserved5[13];
		m_nALCcprsLimit = value.dl_drc_dgain[0];
		m_nALCexpdThreshold = value.dl_drc_dgain[2];
		m_nALCexpdLimit = value.dl_drc_dgain[3];
		m_iValue = (value.reserved5[8] & 0x1000) >> 12;
		//teana hu 2011.05.23
		m_nALCcprsRatio = value.dl_drc_dgain[1];
		m_nALCexpdRatio = value.dl_drc_dgain[4];
		//
	}
	else if(value.reserved5[4] & 0x1 == 1)
	{
		m_bAudio4 = TRUE;
		m_bAudio41 = FALSE;
		m_iValue = (value.reserved5[8] & 0x1000) >> 12;
		m_nALCDPthreshold = value.reserved5[13];
	}
	else
	{
		m_bAudio4 = FALSE;
		m_bAudio41 = FALSE;
		m_iValue = value.dl_agc_switch & 0x1;
	}
}

void SpecialRectOfAgc::Write( CArchive& file )const
{
    file << m_iValue;
}

void SpecialRectOfAgc::Read( CArchive& file )
{
    file >> m_iValue;
}
    
void SpecialRectOfAgc::Fetch(AUDIO_MODE& _value)const
{
	if(m_bAudio4)
	{
		_value.audio_data.reserved5[8] = (_value.audio_data.reserved5[8] & 0xefff) | (m_iValue << 12);
		_value.audio_data.reserved5[13] = m_nALCDPthreshold;
	}
	else if(m_bAudio41)
	{
		_value.audio_data.reserved5[8] = (_value.audio_data.reserved5[8] & 0xefff) | (m_iValue << 12);
		_value.audio_data.reserved5[13] = m_nALCcprsThreshold;
		_value.audio_data.dl_drc_dgain[0] = m_nALCcprsLimit;
		_value.audio_data.dl_drc_dgain[2] = m_nALCexpdThreshold;
		_value.audio_data.dl_drc_dgain[3] = m_nALCexpdLimit;
		//teana hu 2011.05.23
		_value.audio_data.dl_drc_dgain[1] = m_nALCcprsRatio;
		_value.audio_data.dl_drc_dgain[4] = m_nALCexpdRatio;
		//
	}
	else
	{
		_value.audio_data.dl_agc_switch &= 0xFFFE;
		_value.audio_data.dl_agc_switch |= m_iValue;
	}
}

/*********************class SpecialRectOfGain************************************************/
/*******************************************************************************************/
SpecialRectOfGain::SpecialRectOfGain()
{
    m_dGain = 0;
    m_dDpAtt = 0;
    m_iValue = 0;
}

void SpecialRectOfGain::SetValue( AUDIO_STRUCT& value )
{
    switch( m_enumParaType )
    {
    case dl_digital_gain:
        ::CalcDigGainRev( m_dGain   , value.dl_dp_gain );
        ::CalcDigGainRev( m_dDpAtt   , value.dl_dp_attenu );
        break;
    case ul_digital_gain:
        ::CalcDigGainRev( m_dGain   , value.ul_dp_gain );
        ::CalcDigGainRev( m_dDpAtt   , value.ul_dp_attenu );
        break;            
    }    
}

void SpecialRectOfGain::Write( CArchive& file )const
{
    file << m_dGain;
    file << m_dDpAtt;
}

void SpecialRectOfGain::Read( CArchive& file )
{
    file >> m_dGain;
    file >> m_dDpAtt;
}

void SpecialRectOfGain::Fetch(AUDIO_MODE& _value)const
{
    switch( m_enumParaType )
    {
    case dl_digital_gain:
        ::CalcDigGain( m_dGain   , _value.audio_data.dl_dp_gain );
        ::CalcDigGain( m_dDpAtt   , _value.audio_data.dl_dp_attenu );
        break;
    case ul_digital_gain:
        ::CalcDigGain( m_dGain   , _value.audio_data.ul_dp_gain );
        ::CalcDigGain( m_dDpAtt   , _value.audio_data.ul_dp_attenu );
        break;            
    }    
}


/*********************************************************/
// SpecialRectOfPop
/*********************************************************/
SpecialRectOfPop::SpecialRectOfPop()
{
    m_bPopEnable = FALSE;
    m_nDaLimit = 0;
}

void SpecialRectOfPop::Fetch(AUDIO_MODE& _value)const
{
    _value.audio_data.dl_pop_switch = (WORD)m_bPopEnable;
    _value.audio_data.da_limit = m_nDaLimit;
}

//pop up a dialog to modify related value 
int SpecialRectOfPop::ClickAction(CWnd* pParent)
{
	UNREFERENCED_PARAMETER(pParent);
    return -1;
}

void SpecialRectOfPop::Write( CArchive& file )const
{
    file << m_bPopEnable;
    file << m_nDaLimit;
}

void SpecialRectOfPop::Read( CArchive& file )
{
    file >> m_bPopEnable;
    file >> m_nDaLimit;
}

void SpecialRectOfPop::SetValue( AUDIO_STRUCT& value )
{
    m_bPopEnable = value.dl_pop_switch;
    m_nDaLimit = value.da_limit;
}

/*********************************************************/
// SpecialRectOfNg
/*********************************************************/
void SpecialRectOfNg::Read(CArchive &file)
{
    file >> 	m_nNgSelect;
    
    file >> 	m_nAlphaDistor;
    file >> 	m_nBetaDistor;
    
    file >> 	m_nUlNgClkAtt;
    file >> 	m_nUlNgClkHoldc;
    file >> 	m_nUlNgClkWpyyA;
    file >> 	m_nUlNgClkWpyyN;
    file >> 	m_nUlNgPlkAtt;
    file >> 	m_nUlNgPlkHoldc;
    file >> 	m_nUlNgPlkWpyyA;
    file >> 	m_nUlNgPlkWpyyN;
    
    file >> 	m_nDlNgClkAtt;
    file >> 	m_nDlNgClkHoldc;
    file >> 	m_nDlNgClkWpyyA;
    file >> 	m_nDlNgClkWpyyN;
    file >> 	m_nDlNgPlkAtt;
    file >> 	m_nDlNgPlkHoldc;
    file >> 	m_nDlNgPlkWpyyA;
    file >> 	m_nDlNgPlkWpyyN;
}

void SpecialRectOfNg::Write( CArchive& file )const
{
    file << 	m_nNgSelect;
    
    file << 	m_nAlphaDistor;
    file << 	m_nBetaDistor;
    
    file << 	m_nUlNgClkAtt;
    file << 	m_nUlNgClkHoldc;
    file << 	m_nUlNgClkWpyyA;
    file << 	m_nUlNgClkWpyyN;
    file << 	m_nUlNgPlkAtt;
    file << 	m_nUlNgPlkHoldc;
    file << 	m_nUlNgPlkWpyyA;
    file << 	m_nUlNgPlkWpyyN;
    
    file << 	m_nDlNgClkAtt;
    file << 	m_nDlNgClkHoldc;
    file << 	m_nDlNgClkWpyyA;
    file << 	m_nDlNgClkWpyyN;
    file << 	m_nDlNgPlkAtt;
    file << 	m_nDlNgPlkHoldc;
    file << 	m_nDlNgPlkWpyyA;
    file << 	m_nDlNgPlkWpyyN;
}

void SpecialRectOfNg::Fetch(AUDIO_MODE& _value)const
{
    _value.audio_data.ng_select = m_nNgSelect;
    
    _value.audio_data.alpha_distor = m_nAlphaDistor;
    _value.audio_data.beta_distor =	m_nBetaDistor;
    
    _value.audio_data.ul_ng_clk_att = m_nUlNgClkAtt;
    _value.audio_data.ul_ng_clk_holdc =	m_nUlNgClkHoldc;
    _value.audio_data.ul_ng_clk_wpyy_a = m_nUlNgClkWpyyA;
    _value.audio_data.ul_ng_clk_wpyy_n = m_nUlNgClkWpyyN;
    _value.audio_data.ul_ng_plk_att = m_nUlNgPlkAtt;
    _value.audio_data.ul_ng_plk_holdc =	m_nUlNgPlkHoldc;
    _value.audio_data.ul_ng_plk_wpyy_a = m_nUlNgPlkWpyyA;
    _value.audio_data.ul_ng_plk_wpyy_n = m_nUlNgPlkWpyyN;
    
    _value.audio_data.dl_ng_clk_att = m_nDlNgClkAtt;
    _value.audio_data.dl_ng_clk_holdc =	m_nDlNgClkHoldc;
    _value.audio_data.dl_ng_clk_wpyy_a = m_nDlNgClkWpyyA;
    _value.audio_data.dl_ng_clk_wpyy_n = m_nDlNgClkWpyyN;
    _value.audio_data.dl_ng_plk_att = m_nDlNgPlkAtt;
    _value.audio_data.dl_ng_plk_holdc =	m_nDlNgPlkHoldc;
    _value.audio_data.dl_ng_plk_wpyy_a = m_nDlNgPlkWpyyA;
    _value.audio_data.dl_ng_plk_wpyy_n = m_nDlNgPlkWpyyN;
}

void SpecialRectOfNg::SetValue( AUDIO_STRUCT& value )
{
    m_nNgSelect = value.ng_select;
    
    m_nAlphaDistor = value.alpha_distor;
    m_nBetaDistor =	value.beta_distor;
    
    m_nUlNgClkAtt = value.ul_ng_clk_att;
    m_nUlNgClkHoldc = value.ul_ng_clk_holdc;
    m_nUlNgClkWpyyA = value.ul_ng_clk_wpyy_a;
    m_nUlNgClkWpyyN = value.ul_ng_clk_wpyy_n;
    m_nUlNgPlkAtt = value.ul_ng_plk_att;
    m_nUlNgPlkHoldc = value.ul_ng_plk_holdc;
    m_nUlNgPlkWpyyA = value.ul_ng_plk_wpyy_a;
    m_nUlNgPlkWpyyN = value.ul_ng_plk_wpyy_n;
    
    m_nDlNgClkAtt = value.dl_ng_clk_att;
    m_nDlNgClkHoldc = value.dl_ng_clk_holdc;
    m_nDlNgClkWpyyA = value.dl_ng_clk_wpyy_a;
    m_nDlNgClkWpyyN = value.dl_ng_clk_wpyy_n;
    m_nDlNgPlkAtt = value.dl_ng_plk_att;
    m_nDlNgPlkHoldc = value.dl_ng_plk_holdc;
    m_nDlNgPlkWpyyA = value.dl_ng_plk_wpyy_a;
    m_nDlNgPlkWpyyN = value.dl_ng_plk_wpyy_n;
}

/*********************************************************/
// SpecialRectOfAec
/*********************************************************/
SpecialRectOfAec::SpecialRectOfAec()
{
    m_nUlFirHpf = 1;
    m_nAecEnable = 0;
    m_nPdelay = 8;
    m_nDlRefHpf = 1;
    m_nDecorFilter = 0;
    m_nFirTaps = 32;
    m_nCoeffFrozen = 0;
    m_nAecFrozen = 0;
    m_nDtDectThreshold = 409;
    m_nDtNoiseFloor = 1;
    m_nStepSize = 1;
    m_nCoeffNormShift = 0;
    m_nSaCtrl = 0;
    m_nSendAttenuInDt = 4096;
    m_nSendAttenuInRv = 4096;
    m_nSendThreshold = 8192;
    m_nR2dtThreshold = 6144;
    m_nS2dtThreshold = 4096;
    m_nRecvThreshold = 2048;
    m_nBn40 = 8;
    m_nSignalDelay = 0;
    m_nSaDelay = 0;
	
	//teana hu 2011.02.14
	m_nCGNGain = 0;
	m_bAudio4 = FALSE;
}

void SpecialRectOfAec::Fetch(AUDIO_MODE& _value)const
{
	//teana hu 2011.02.14
	if(m_bAudio4)
	{
		_value.audio_data.ul_ng_plk_att = m_nSendAttenuInDt;
		_value.audio_data.ul_ng_clk_wpyy_a = m_nSendAttenuInRv;
		_value.audio_data.ul_ng_plk_holdc = m_nCGNGain;
		_value.audio_data.ul_ng_plk_wpyy_n = m_nSendThreshold;
		_value.audio_data.ul_ng_plk_wpyy_a = m_nRecvThreshold;
	}
	else
	{
		_value.audio_data.send_attenu_in_dt = m_nSendAttenuInDt;
		_value.audio_data.send_attenu_in_rv = m_nSendAttenuInRv;
		_value.audio_data.send_threshold = m_nSendThreshold;
		_value.audio_data.recv_threshold = m_nRecvThreshold;
	}
	
    _value.audio_data.ul_fir_hpf_enable = m_nUlFirHpf;
    _value.audio_data.aec_enable = m_nAecEnable;
    _value.audio_data.pdelay = m_nPdelay;
    _value.audio_data.dl_ref_hpf_enable = m_nDlRefHpf;
    _value.audio_data.decor_filter_enable = m_nDecorFilter;
    _value.audio_data.fir_taps = m_nFirTaps;
    _value.audio_data.aec_frozen = m_nAecFrozen;
    _value.audio_data.coeff_frozen = m_nCoeffFrozen;
    _value.audio_data.dt_dect_threshold = m_nDtDectThreshold;
    _value.audio_data.dt_noise_floor = m_nDtNoiseFloor;
    _value.audio_data.step_size = m_nStepSize;
    
    _value.audio_data.coeff_norm_shift = m_nCoeffNormShift;
    _value.audio_data.coeff_frozen = m_nCoeffFrozen;
//    _value.audio_data.send_attenu_in_dt = m_nSendAttenuInDt;
//    _value.audio_data.send_attenu_in_rv = m_nSendAttenuInRv;
//     _value.audio_data.send_threshold = m_nSendThreshold;
    _value.audio_data.r2dt_threshold = m_nR2dtThreshold;
    _value.audio_data.s2dt_threshold = m_nS2dtThreshold;
//     _value.audio_data.recv_threshold = m_nRecvThreshold;
    _value.audio_data.bn40 = m_nBn40;
    
    _value.audio_data.sa_ctrl = m_nSaCtrl;
    _value.audio_data.sa_ar = (WORD)( ( m_nSaDelay << 8 ) | m_nSignalDelay );
}

void SpecialRectOfAec::SetValue( AUDIO_STRUCT& value )
{
	//teana hu 2011.02.14
	if(value.reserved5[4] &0x1 == 1)
	{
		m_bAudio4 = TRUE;
		m_nSendAttenuInDt = value.ul_ng_plk_att;
		m_nSendAttenuInRv = value.ul_ng_clk_wpyy_a;
		m_nCGNGain = value.ul_ng_plk_holdc;
		m_nSendThreshold = value.ul_ng_plk_wpyy_n;
		m_nRecvThreshold = value.ul_ng_plk_wpyy_a;
	}
	else
	{
		m_bAudio4 = FALSE;
		m_nSendAttenuInDt = value.send_attenu_in_dt;
		m_nSendAttenuInRv = value.send_attenu_in_rv;
		m_nSendThreshold = value.send_threshold;
		m_nRecvThreshold = value.recv_threshold;
	}

	
    m_nUlFirHpf = value.ul_fir_hpf_enable;
    m_nAecEnable = value.aec_enable;
    m_nPdelay = value.pdelay;
    m_nDlRefHpf = value.dl_ref_hpf_enable;
    m_nDecorFilter = value.decor_filter_enable;
    m_nFirTaps = value.fir_taps;
    m_nAecFrozen = value.aec_frozen;
    m_nCoeffFrozen = value.coeff_frozen;
    m_nDtDectThreshold = value.dt_dect_threshold ;
    m_nDtNoiseFloor = value.dt_noise_floor;
    m_nStepSize = value.step_size;
    
    m_nCoeffNormShift = value.coeff_norm_shift;
    m_nSaCtrl = value.sa_ctrl;
    
    //m_nSendAttenuInDt = value.send_attenu_in_dt;
    //m_nSendAttenuInRv = value.send_attenu_in_rv;
    //m_nSendThreshold = value.send_threshold;
    m_nR2dtThreshold = value.r2dt_threshold;
    m_nS2dtThreshold = value.s2dt_threshold;
    //m_nRecvThreshold = value.recv_threshold;
    m_nBn40 = value.bn40;


    m_nSignalDelay = (short)( value.sa_ar & 0xFF );
    m_nSaDelay = (short)( ( value.sa_ar >> 8 ) & 0xFF ); 
}

void SpecialRectOfAec::Read(CArchive &file)
{
    file >> m_nUlFirHpf;
    file >> m_nAecEnable;
    file >> m_nPdelay;
    file >> m_nDlRefHpf;
    file >> m_nDecorFilter;
    file >> m_nFirTaps;
    file >> m_nCoeffFrozen;
    file >> m_nAecFrozen;
    file >> m_nDtDectThreshold;
    file >> m_nDtNoiseFloor;
    file >> m_nStepSize;
    file >> m_nCoeffNormShift;
    file >> m_nSaCtrl;
    file >> m_nSendAttenuInDt;
    file >> m_nSendAttenuInRv;
    file >> m_nSendThreshold;
    file >> m_nR2dtThreshold;
    file >> m_nS2dtThreshold;
    file >> m_nRecvThreshold;
    file >> m_nBn40;

    file >> m_nSignalDelay;
    file >> m_nSaDelay;
}

void SpecialRectOfAec::Write( CArchive& file )const
{
    file << m_nUlFirHpf;
    file << m_nAecEnable;
    file << m_nPdelay;
    file << m_nDlRefHpf;
    file << m_nDecorFilter;
    file << m_nFirTaps;
    file << m_nCoeffFrozen;
    file << m_nAecFrozen;
    file << m_nDtDectThreshold;
    file << m_nDtNoiseFloor;
    file << m_nStepSize;
    file << m_nCoeffNormShift;
    file << m_nSaCtrl;
    file << m_nSendAttenuInDt;
    file << m_nSendAttenuInRv;
    file << m_nSendThreshold;
    file << m_nR2dtThreshold;
    file << m_nS2dtThreshold;
    file << m_nRecvThreshold;
    file << m_nBn40;
    
    file << m_nSignalDelay;
    file << m_nSaDelay;
}

/*************** class SpecialRectOfCoefficient***********************************************/
/*********************************************************************************************/
SpecialRectOfCoefficient::SpecialRectOfCoefficient():m_dMaxYValue(0.0)
{
	m_bModified = FALSE;
	m_strOriFile = _T("");
    CalculateCoefficients();
    CalculateFreqGraphyData();
}

int SpecialRectOfCoefficient::ClickAction(CWnd* pParent)
{
	UNREFERENCED_PARAMETER(pParent);
    /*
    CCoefficientDlg dlg(pParent);
    dlg.m_pCoefficientArea = this;
    return dlg.DoModal();   
    */
    ASSERT( FALSE );
    return 0;
}

// caculate all 17 coefficents
void SpecialRectOfCoefficient::CalculateCoefficients()
{
    double dwWeight[ROW_COUNT];
    for( int i=0; i<ROW_COUNT; ++i )
    {
        dwWeight[i] = pow( 10   , m_vPo.m_vPoValue[i]/20.0 );
    }     
//    double dMaxValue = -1.0;

    int* pArray = m_iArrayFreq[0];

    int max = 0;
    for( i=0; i<COLUMN_COUNT; ++i )
    {
        double dwCoefficient = 0.0;                     

        for( int j=0; j<ROW_COUNT; ++j )
        {            
            int iSign = m_vPo.m_vPoSign[j] == 1 ? -1 : 1;
            dwCoefficient += iSign * dwWeight[j] * pArray[j*COLUMN_COUNT+i];            
        }       
        m_vCoefficients[i] = dwCoefficient/8.0;

        int ntime = (int)(m_vCoefficients[i] / 32767);
        if( ntime > max )
        {
            max = ntime;
        }
    }

    // Ensure all coefficients less than 32767
    if( max >= 1 )
    {
        max = ( max << max );
        for( i=0;i<COLUMN_COUNT;i++ )
        {
            m_vCoefficients[i] = m_vCoefficients[i] / max ;
        }
    }
}

//restore to default value   , especially set the m_rectArea as m_rectDefaultArea
//cause when we scroll the window   ,  we will change the value of m_rectArea
void SpecialRectOfCoefficient::SetToDefault()
{
    SpecialRect::SetToDefault();
} 

const double SpecialRectOfCoefficient::PI = 3.1415926;
const double SpecialRectOfCoefficient::m_dMinXValue = 100.0;
const double SpecialRectOfCoefficient::m_dMaxXValue = 4000.0;

/*
int SpecialRectOfCoefficient::m_iXScale[X_SCALE_COUNT_FIR] = 
{ 100   , 200   , 300   , 400   , 500   , 600   , 700   , 800   , 900   , 1000   , 2000   , 3000   , 4000 } ;
*/

int SpecialRectOfCoefficient::m_iXScale[X_SCALE_COUNT_FIR] = 
{ 200   , 224   , 250   , 281   , 315   , 355   , 400   , 447   , 500   , 561   , 630   , 710   , 800   , 894   , 1000   , 1118   , 1250   , 1424   , 1580   , 1789   , 2000   , 2236   , 2500   , 2806   , 3150   , 3520 } ;

// ROW_COUNT=13   , COLUMN_COUNT=33
int SpecialRectOfCoefficient::m_iArrayFreq[ROW_COUNT][COLUMN_COUNT] =
{
    {-139       , -122       , -102       , -79       , -54       , -28       , -2       , 24       , 50       , 75       , 98       , 119       , 137       , 151       , 162       , 168       , 170       , 168       , 162       , 151       , 137       , 119       , 98       , 75       , 50       , 24       , -2       , -28       , -54       , -79       , -102       , -122       , -139}   ,                                          
    {-192       , -179       , -160       , -137       , -108       , -76       , -42       , -7       , 28       , 63       , 95       , 125       , 151       , 173       , 189       , 199       , 202       , 199       , 189       , 173       , 151       , 125       , 95       , 63       , 28       , -7       , -42       , -76       , -108       , -137       , -160       , -179       , -192}   ,                                    
    {-176       , -160       , -140       , -116       , -90       , -63       , -35       , -8       , 18       , 42       , 65       , 85       , 102       , 116       , 126       , 132       , 133       , 132       , 126       , 116       , 102       , 85       , 65       , 42       , 18       , -8       , -35       , -63       , -90       , -116       , -140       , -160       , -176}   ,                                        
    {-220       , -232       , -233       , -224       , -205       , -175       , -137       , -92       , -41       , 9       , 60       , 108       , 151       , 186       , 212       , 228       , 233       , 228       , 212       , 186       , 151       , 108       , 60       , 9       , -41       , -92       , -137       , -175       , -205       , -224       , -233       , -232       , -220}   ,                              
    {-194       , -237       , -266       , -279       , -275       , -255       , -220       , -171       , -111       , -44       , 25       , 93       , 155       , 207       , 246       , 270       , 278       , 270       , 246       , 207       , 155       , 93       , 25       , -44       , -111       , -171       , -220       , -255       , -275       , -279       , -266       , -237       , -194}   ,                        
    {-80       , -165       , -237       , -290       , -321       , -326       , -307       , -264       , -200       , -122       , -34       , 56       , 143       , 218       , 278       , 315       , 328       , 315       , 278       , 218       , 143       , 56       , -34       , -122       , -200       , -264       , -307       , -326       , -321       , -290       , -237       , -165       , -80}   ,                      
    {121       , 5       , -112       , -219       , -304       , -359       , -379       , -361       , -308       , -225       , -119       , -1       , 116       , 223       , 307       , 361       , 380       , 361       , 307       , 223       , 116       , -1       , -119       , -225       , -308       , -361       , -379       , -359       , -304       , -219       , -112       , 5       , 121}   ,                          
    {340       , 231       , 92       , -58       , -202       , -321       , -400       , -431       , -408       , -335       , -222       , -82       , 69       , 212       , 330       , 407       , 434       , 407       , 330       , 212       , 69       , -82       , -222       , -335       , -408       , -431       , -400       , -321       , -202       , -58       , 92       , 231       , 340}   ,                            
    {472       , 442       , 343       , 190       , 6       , -177       , -336       , -443       , -483       , -449       , -345       , -188       , -2       , 184       , 343       , 449       , 486       , 449       , 343       , 184       , -2       , -188       , -345       , -449       , -483       , -443       , -336       , -177       , 6       , 190       , 343       , 442       , 472}   ,                              
    {377       , 505       , 538       , 466       , 303       , 81       , -157       , -368       , -508       , -551       , -488       , -330       , -108       , 134       , 352       , 502       , 556       , 502       , 352       , 134       , -108       , -330       , -488       , -551       , -508       , -368       , -157       , 81       , 303       , 466       , 538       , 505       , 377}   ,                          
    {-58       , 263       , 525       , 662       , 640       , 461       , 169       , -165       , -462       , -649       , -680       , -546       , -278       , 56       , 378       , 610       , 694       , 610       , 378       , 56       , -278       , -546       , -680       , -649       , -462       , -165       , 169       , 461       , 640       , 662       , 525       , 263       , -58}   ,                            
    {-595       , -345       , 13       , 374       , 624       , 685       , 536       , 221       , -163       , -501       , -688       , -665       , -438       , -76       , 309       , 602       , 710       , 602       , 309       , -76       , -438       , -665       , -688       , -501       , -163       , 221       , 536       , 685       , 624       , 374       , 13       , -345       , -595}   ,                          
    {-583       , -743       , -618       , -251       , 218       , 610       , 770       , 634       , 251       , -231       , -629       , -787       , -642       , -248       , 241       , 640       , 793       , 640       , 241       , -248       , -642       , -787       , -629       , -231       , 251       , 634       , 770       , 610       , 218       , -251       , -618       , -743       , -583}   ,                    
    {163       , -274       , -590       , -629       , -366       , 76       , 489       , 674       , 538       , 144       , -323       , -640       , -654       , -355       , 112       , 529       , 694       , 529       , 112       , -355       , -654       , -640       , -323       , 144       , 538       , 674       , 489       , 76       , -366       , -629       , -590       , -274       , 163}   ,                        
    {876       , 646       , 24       , -628       , -921       , -672       , -18       , 658       , 955       , 689       , 11       , -680       , -975       , -696       , -3       , 693       , 982       , 693       , -3       , -696       , -975       , -680       , 11       , 689       , 955       , 658       , -18       , -672       , -921       , -628       , 24       , 646       , 876}   ,                                
    {44       , 783       , 968       , 443       , -425       , -1004       , -857       , -76       , 776       , 1075       , 590       , -333       , -1023       , -971       , -210       , 707       , 1112       , 707       , -210       , -971       , -1023       , -333       , 590       , 1075       , 776       , -76       , -857       , -1004       , -425       , 443       , 968       , 783       , 44}   ,                   
    {-1038       , -633       , 368       , 1075       , 827       , -182       , -1057       , -997       , -31       , 984       , 1129       , 258       , -858       , -1214       , -482       , 686       , 1243       , 686       , -482       , -1214       , -858       , 258       , 1129       , 984       , -31       , -997       , -1057       , -182       , 827       , 1075       , 368       , -633       , -1038}   ,           
    {640       , -528       , -1145       , -468       , 777       , 1178       , 240       , -1003       , -1135       , 26       , 1183       , 1015       , -309       , -1300       , -827       , 584       , 1340       , 584       , -827       , -1300       , -309       , 1015       , 1183       , 26       , -1135       , -1003       , 240       , 1178       , 777       , -468       , -1145       , -528       , 640}   ,         
    {579       , 1183       , 166       , -1148       , -934       , 593       , 1369       , 278       , -1239       , -1096       , 562       , 1489       , 391       , -1260       , -1210       , 491       , 1531       , 491       , -1210       , -1260       , 391       , 1489       , 562       , -1096       , -1239       , 278       , 1369       , 593       , -934       , -1148       , 166       , 1183       , 579}   ,         
    {-1040       , -850       , 850       , 1206       , -514       , -1461       , 63       , 1565       , 447       , -1488       , -951       , 1227       , 1375       , -807       , -1658       , 281       , 1758       , 281       , -1658       , -807       , 1375       , 1227       , -951       , -1488       , 447       , 1565       , 63       , -1461       , -514       , 1206       , 850       , -850       , -1040}   ,       
    {1247       , 29       , -1380       , -27       , 1502       , 25       , -1611       , -21       , 1703       , 17       , -1778       , -13       , 1832       , 8       , -1865       , -2       , 1876       , -2       , -1865       , 8       , 1832       , -13       , -1778       , 17       , 1703       , -21       , -1611       , 25       , 1502       , -27       , -1380       , 29       , 1247}   ,                         
    {-1232       , 389       , 1261       , -947       , -1039       , 1463       , 565       , -1814       , 99       , 1899       , -840       , -1669       , 1516       , 1143       , -1988       , -406       , 2158       , -406       , -1988       , 1143       , 1516       , -1669       , -840       , 1899       , 99       , -1814       , 565       , 1463       , -1039       , -947       , 1261       , 389       , -1232}   ,   
    {1184       , -380       , -1105       , 1380       , 132       , -1690       , 1242       , 866       , -2070       , 731       , 1635       , -2085       , -61       , 2218       , -1683       , -944       , 2435       , -944       , -1683       , 2218       , -61       , -2085       , 1635       , 731       , -2070       , 866       , 1242       , -1690       , 132       , 1380       , -1105       , -380       , 1184}   ,   
    {-775       , -171       , 1217       , -1390       , 334       , 1216       , -1944       , 1073       , 837       , -2234       , 1857       , 119       , -2120       , 2454       , -768       , -1590       , 2677       , -1590       , -768       , 2454       , -2120       , 119       , 1857       , -2234       , 837       , 1073       , -1944       , 1216       , 334       , -1390       , 1217       , -171       , -775}   , 
    {-347       , 963       , -1286       , 1032       , -177       , -974       , 1893       , -2074       , 1307       , 169       , -1737       , 2668       , -2474       , 1170       , 707       , -2331       , 2970       , -2331       , 707       , 1170       , -2474       , 2668       , -1737       , 169       , 1307       , -2074       , 1893       , -974       , -177       , 1032       , -1286       , 963       , -347}   , 
    {609       , -650       , 503       , -142       , -406       , 1064       , -1710       , 2204       , -2418       , 2264       , -1716       , 828       , 277       , -1429       , 2433       , -3117       , 3359       , -3117       , 2433       , -1429       , 277       , 828       , -1716       , 2264       , -2418       , 2204       , -1710       , 1064       , -406       , -142       , 503       , -650       , 609}
};

int SpecialRectOfCoefficient::m_iDy[ROW_COUNT][POWERDATA_WEIGHT_COUNT] = 
{
    { -53020,	-46341,	-38522,-29753,-20252,-10253,-1,10252,20251,29752,38521,46340,53019,58393,62328,64729,32768           } ,
    { -62070,	-57430,	-51018,-43031,-33715,-23358,-12281,-824,10658,21811,32291,41774,49967,56618,61520,64524,32768      } ,
    { -65536,	-64277,	-60548,-54492,-46341,-36410,-25080,-12786,0,12785,25079,36409,46340,54491,60547,64276,32768         } ,
    { -60626,	-64604,	-65448,-63118,-57725,-49532,-38937,-26452,-12685,1698,15998,29523,41615,51688,59254,63946,32768   } ,
    { -44863,	-55196,	-62168,-65354,-64561,-59836,-51467,-39964,-26028,-10507,5654,21471,35980,48298,57675,63540,32768 } ,
    { -16299,	-33139,	-47420,-58039,-64175,-65354,-61486,-52869,-40168,-24365,-6680,11520,28831,43915,55607,63005,32768 } ,
    { 20251,	-1,	-20252,-38522,-53020,-62329,-65536,-62329,-53020,-38522,-20252,-1,20251,38521,53019,62328,32768     } ,
    { 51530,   34461,13189,-9693,-31392,-49262,-61122,-65526,-61936,-50791,-33450,-12028,10861,32425,50033,61538,32768   } ,
    { 65536,	60547,46340,25079,-1,-25080,-46341,-60548,-65536,-60548,-46341,-25080,0,25079,46340,60547,32768          } ,
    { 47206,	62085,65105,55689,35635,8775,-19762,-44524,-60782,-65429,-57578,-38730,-12483,16148,41694,59276,32768    } ,
    { -4116,	27437,52408,64807,61661,43724,15299,-16797,-44863,-62168,-64561,-51467,-26028,5654,35980,57675,32768      } ,
    { -57430,	-32023,3087,37261,60145,64807,49833,19761,-16299,-47420,-64175,-61486,-40168,-6680,28831,55607,32768      } ,
    { -53020,	-65536,-53020,-20252,20251,53019,65536,53019,20251,-20252,-53020,-65536,-53020,-20252,20251,53019,32768   } ,
    { 15499,	-29294,-60228,-62670,-35463,8520,48472,65493,51530,13189,-31392,-61122,-61936,-33450,10861,50033,32768   } ,
    { 65536,	46340,-1,-46341,-65536,-46341,0,46340,65536,46340,-1,-46341,-65536,-46341,0,46340,32768                   } ,
    { 5757,	53912,63103,26687,-29017,-63750,-52409,-3190,48333,64924,34592,-20741,-61085,-57281,-12078,41853,32768       } ,
    { -65536,	-36410,25079,64276,46340,-12786,-60548,-54492,0,54491,60547,12785,-46341,-64277,-25080,36409,32768         }, 
    { 37851,	-31573,-65454,-25650,43030,63267,12280,-52532,-58205,1646,59644,50496,-15500,-64047,-40493,28646,32768   } ,
    { 35115,	63725,6167,-59730,-44863,30666,64729,11267,-57430,-48473,26027,65333,16298,-54776,-51784,21228,32768       }, 
    { -57822,	-39964,44636,54690,-26594,-63465,5654,65329,15898,-60085,-35723,48298,51657,-31256,-61970,10810,32768     } ,
    { 65536,	-1,-65536,0,65536,-1,-65536,0,65536,-1,-65536,0,65536,-1,-65536,0,32768                                   } ,
    { -64525,	23165,55985,-43802,-39842,58486,18283,-65226,5757,63103,-29017,-52409,48333,34592,-61085,-12078,32768     } ,
    { 65536,	-25080,-46341,60547,0,-60548,46340,25079,-65536,25079,46340,-60548,0,60547,-46341,-25080,32768             } ,
    { -49968,	-4629,55443,-60972,16696,41216,-65464,36238,22586,-62963,51909,1544,-53737,62035,-19664,-38771,32768       } ,
    { -20252,	54491,-65334,48124,-10253,-32023,60547,-63076,38521,2572,-42563,64276,-58394,27437,15299,-51467,32768     } ,
    { 63477,	-53020,35115,-12281,-12281,35115,-53020,63477,-65020,57429,-41775,20251,4115,-27904,47773,-60934,32768   } ,
};


//calculate all frequence curve x value and y value
void SpecialRectOfCoefficient::CalculateFreqGraphyData(int iXScaleCount /*=500*/ )
{
    m_vAllFreqItem.clear();
    //x--->f   ,  y--->db
    double dXScaleStep = (m_dMaxXValue - m_dMinXValue)/iXScaleCount;
    
    double dXScale=m_dMinXValue;
    double dMaxYValue = 10.0;
    
    for( int i=0; i<iXScaleCount; ++i )
    {        
        double dYScale =0.0;
        std::complex<float> total;
        for( int j=0; j<COLUMN_COUNT; ++j )
        {
            float dAngle = (float)(2.0*PI*dXScale*j/8000.0);
//            float dCos = (float)cos(dAngle);
//            float dSin = (float)sin(dAngle);
            
            float dR = (float)(m_vCoefficients[ j ] * cos(dAngle));
            float dI = (float)(-m_vCoefficients[ j ] * sin(dAngle));            
            std::complex<float> temp(dR   ,  dI );
            total += temp;            
        }
        struct _complex   totalCType;
        totalCType.x = total.real();
        totalCType.y = total.imag();
        dYScale = _cabs(totalCType);
       
        dYScale = 20* log10(dYScale/4096.0 );       
        //dYScale = 20* log10(dYScale/32767.0 );       
       
        FreqItem tempItem;
        tempItem.first = log10( dXScale );
        tempItem.second = dYScale;
        m_vAllFreqItem.push_back( tempItem );
        dXScale += dXScaleStep;
    }
    m_dMaxYValue = dMaxYValue;    
}

//calculate all frequence curve x value and y value
void SpecialRectOfCoefficient::CalculateFreqGraphyDataEx(double dX[65], int iXScaleCount /*=65*/ )
{
    m_vAllFreqItem.clear();
    //x--->f   ,  y--->db
    double dXScaleStep = (m_dMaxXValue - m_dMinXValue)/iXScaleCount;
    
    double dMaxYValue = 10.0;
    
    for( int i=0; i<iXScaleCount; ++i )
    {        
        double dYScale =0.0;
        std::complex<float> total;
        for( int j=0; j<COLUMN_COUNT; ++j )
        {
            float dAngle = (float)(2.0*PI*dX[i]*j/8000.0);
            
            float dR = (float)(m_vCoefficients[ j ] * cos(dAngle));
            float dI = (float)(-m_vCoefficients[ j ] * sin(dAngle));            
            std::complex<float> temp(dR   ,  dI );
            total += temp;            
        }
        struct _complex   totalCType;
        totalCType.x = total.real();
        totalCType.y = total.imag();
        dYScale = _cabs(totalCType);
       
        dYScale = 20* log10(dYScale/4096.0 );             
       
        FreqItem tempItem;
        tempItem.first = log10( dX[i] );
        tempItem.second = dYScale;
        m_vAllFreqItem.push_back( tempItem );
    }
    m_dMaxYValue = dMaxYValue;    
}

void SpecialRectOfCoefficient::CalculatePowerData()
{
    double weight[ROW_COUNT];

    for( int i=0;i<ROW_COUNT;i++ )
    {
        weight[i] = 0;
        for( int j=0;j<POWERDATA_WEIGHT_COUNT;j++ )
        {
            weight[i] += ( m_vCoefficients[j] / 4096 ) * ( m_iDy[i][j] / 32768.0 );
        }

        m_vPo.m_vPoSign[i] = 1 ;
        m_vPo.m_vPoValue[i] = 20 * log10( fabs( weight[i] ) );
    }
}

void SpecialRectOfCoefficient::Write(CArchive &file)const
{
    for( int i=0; i<ROW_COUNT; ++i )
    {
        file << m_vPo.m_vPoValue[ i ];        
        file << m_vPo.m_vPoSign[ i ];
    }
}

void SpecialRectOfCoefficient::Read(CArchive &file)
{
    for( int i=0; i<ROW_COUNT; ++i )
    {
        file >> m_vPo.m_vPoValue[ i ];
        file >> m_vPo.m_vPoSign[ i ];
    }
    CalculateCoefficients();
    CalculateFreqGraphyData();
}
void SpecialRectOfCoefficient::Fetch(AUDIO_MODE& _value)const
{
    switch( m_enumParaType )
    {
    case dl_fir:
        {
            _value.audio_data.dl_eq_bass_alpha = (WORD)m_vCoefficients[0];
            _value.audio_data.dl_eq_bass_beta = 	(WORD)m_vCoefficients[1];
            _value.audio_data.dl_eq_bass_gama = (WORD)m_vCoefficients[2];
            _value.audio_data.dl_eq_bass_gain =	(WORD)m_vCoefficients[3];
            _value.audio_data.dl_eq_mid_alpha = (WORD)m_vCoefficients[4];
            _value.audio_data.dl_eq_mid_beta = (WORD)m_vCoefficients[5];
            _value.audio_data.dl_eq_mid_gama = (WORD)m_vCoefficients[6];
            _value.audio_data.dl_eq_mid_gain = (WORD)m_vCoefficients[7];
            _value.audio_data.dl_eq_treble_alpha = (WORD)m_vCoefficients[8];
            _value.audio_data.dl_eq_treble_beta = (WORD)m_vCoefficients[9];
            _value.audio_data.dl_eq_treble_gama = (WORD)m_vCoefficients[10];
            _value.audio_data.dl_eq_treble_gain = (WORD)m_vCoefficients[11];
            _value.audio_data.dl_ng_plk_wpyy_a = (WORD)m_vCoefficients[12];
            _value.audio_data.dl_ng_plk_wpyy_n = (WORD)m_vCoefficients[13];
            _value.audio_data.dl_ng_plk_holdc = (WORD)m_vCoefficients[14];
            _value.audio_data.dl_ng_plk_att = (WORD)m_vCoefficients[15];
            _value.audio_data.dl_ng_clk_wpyy_a = (WORD)m_vCoefficients[16]; 
            
            _value.audio_data.dl_ng_clk_wpyy_n = m_dl_fir_eq_gain;
        }
        break;
    case dl_fir_eq_coeff:
        {
            for( int i=0;i<DL_FIR_EQ_COUNT;i++ )
            {
                _value.audio_data.dl_fir_eq_coeff[i] = (WORD)m_vCoefficients[i];
            }
        }
        break;
    case ul_fir_eq_coeff:
        {
            for( int i=0;i<UL_FIR_EQ_COUNT;i++ )
            {
                _value.audio_data.ul_fir_eq_coeff[i] = (WORD)m_vCoefficients[i];
            }
        }
        break;
    case st_fir_eq_coeff:
        {
            for( int i=0;i<ST_FIR_EQ_COUNT;i++ )
            {
                _value.audio_data.st_fir_eq_coeff[i] = (WORD)m_vCoefficients[i];
            }
        }
        break;
    }
}

void SpecialRectOfCoefficient::SetValue( AUDIO_STRUCT& _value )
{
    switch( m_enumParaType )
    {
    case dl_fir:
        {
            m_vCoefficients[0]  = short(_value.dl_eq_bass_alpha);
            m_vCoefficients[1]  = short(_value.dl_eq_bass_beta);
            m_vCoefficients[2]  = short(_value.dl_eq_bass_gama);
            m_vCoefficients[3] = short(_value.dl_eq_bass_gain);   
            m_vCoefficients[4] = short(_value.dl_eq_mid_alpha);   
            m_vCoefficients[5] = short(_value.dl_eq_mid_beta);    
            m_vCoefficients[6] = short(_value.dl_eq_mid_gama);    
            m_vCoefficients[7] = short(_value.dl_eq_mid_gain);    
            m_vCoefficients[8] = short(_value.dl_eq_treble_alpha);
            m_vCoefficients[9] = short(_value.dl_eq_treble_beta); 
            m_vCoefficients[10]= short(_value.dl_eq_treble_gama); 
            m_vCoefficients[11]= short(_value.dl_eq_treble_gain); 
            m_vCoefficients[12]= short(_value.dl_ng_plk_wpyy_a);  
            m_vCoefficients[13]= short(_value.dl_ng_plk_wpyy_n);  
            m_vCoefficients[14]= short(_value.dl_ng_plk_holdc);   
            m_vCoefficients[15]= short(_value.dl_ng_plk_att); 
            m_vCoefficients[16]= short(_value.dl_ng_clk_wpyy_a);

            for( int i=0;i<16;i++ )
            {
                m_vCoefficients[32-i] = m_vCoefficients[i];
            }
            
            m_dl_fir_eq_gain = _value.dl_ng_clk_wpyy_n;
        }
        break;
    case dl_fir_eq_coeff:
        {
            for( int i=0;i<DL_FIR_EQ_COUNT;i++ )
            {
                m_vCoefficients[i] = short(_value.dl_fir_eq_coeff[i]);
            }
            for( i=0;i<DL_FIR_EQ_COUNT-1;i++ )
            {
                m_vCoefficients[UL_FIR_EQ_COUNT-1-i] = short(_value.dl_fir_eq_coeff[i]);
            }
        }
        break;
    case ul_fir_eq_coeff:
        {
            for( int i=0;i<UL_FIR_EQ_COUNT;i++ )
            {
                m_vCoefficients[i] = short(_value.ul_fir_eq_coeff[i]);
            }
        }
        break;
    case st_fir_eq_coeff:
        {
            for( int i=0;i<ST_FIR_EQ_COUNT;i++ )
            {
                m_vCoefficients[i] = short(_value.st_fir_eq_coeff[i]);
            }
        }
        break;
    }

    CalculatePowerData();
}


//set a set of special data into coefficients
void SpecialRectOfCoefficient::SetSpecialData()
{
   ZeroMemory( m_vCoefficients   ,  sizeof(double)*COLUMN_COUNT);
   
   m_vCoefficients[ COLUMN_COUNT / 2 ] = 0X7FFF;
}

// formate tool tips string
CString SpecialRectOfCoefficient::GetToolTips()const
{    
    CString strToolTips;
    for( int i=0; i<COLUMN_COUNT; ++i )
    {
        CString strSingleLine;
        strSingleLine.Format(_T("%s[%d] = %d")   , m_strName   , i   , Round(m_vCoefficients[i]) );
        
        strToolTips += strSingleLine;
        strToolTips += "\r\n";
    }
    
    return strToolTips;
}


/*************** class SpecialRectOfCoefficientEx***********************************************/
/*********************************************************************************************/
SpecialRectOfCoefficientEx::SpecialRectOfCoefficientEx():m_dMaxYValue(0.0)
{
	m_bModified = FALSE;
	m_strOriFile = _T("");
    CalculateCoefficients();
    CalculateFreqGraphyData();
}

void SpecialRectOfCoefficientEx::audio_fir_eq_65(
      int * frequency_set,//频率设置
	  double * target_eq,//目标频响
	  int  frequency_scale,//频率设置尺度
	  int * fir_coef//fir_eq系数
	  )
{
//   double target_G[65];
   double fir_design[4001];
   int i,j;
   double ap1,ap2;
   double temp; 
   double temp_fir[65];
   double pi=3.1415926;
   if (frequency_scale==1)//     1/12 octave
   {
	   
	   j=1;
	   for (i=frequency_set[0];i<frequency_set[64]+1;i++)
	   {
            while (i>frequency_set[j])
			{
		        j++;
			}
	        ap1=(double)(frequency_set[j]-i)/(frequency_set[j]-frequency_set[j-1]);
	        ap2=(double)(-frequency_set[j-1]+i)/(frequency_set[j]-frequency_set[j-1]);
            fir_design[i]=ap1*target_eq[j-1]+ap2*target_eq[j];
	   }
	   if (fir_design[100]>0)
	   {
		   for (i=0;i<100;i++)
		   {
			 fir_design[i]=(double)(0.5*fir_design[100]*i*i/100/100)+0.5*fir_design[100];
		   }
	   }
	   else
	   {
           for (i=0;i<100;i++)
		   {
			 fir_design[i]=(double)(-0.5*fir_design[100]*i*i/100/100)+1.5*fir_design[100];
		   }
	   }
   }
   else
   {
       
	   j=1;
	   for (i=frequency_set[0];i<frequency_set[32]+1;i++)
	   {
            while (i>frequency_set[j])
			{
		        j++;
			}
	        ap1=(double)(frequency_set[j]-i)/(frequency_set[j]-frequency_set[j-1]);
	        ap2=(double)(-frequency_set[j-1]+i)/(frequency_set[j]-frequency_set[j-1]);
            fir_design[i]=ap1*target_eq[j-1]+ap2*target_eq[j];
	   }
	   if (fir_design[100]>0)
	   {
		   for (i=0;i<100;i++)
		   {
			 fir_design[i]=(double)(0.5*fir_design[100]*i*i/100/100)+0.5*fir_design[100];
		   }
	   }
	   else
	   {
           for (i=0;i<100;i++)
		   {
			 fir_design[i]=(double)(-0.5*fir_design[100]*i*i/100/100)+1.5*fir_design[100];
		   }
	   }
   }
   for (i=0;i<4001;i++)
   {
	   fir_design[i]=pow(10,fir_design[i]/20);
   }
   // cal coef
   for (i=32;i<65;i++)
   {
	   temp=0;
	   for (j=0;j<4001;j++)
	   {
		   temp=temp+fir_design[j]*cos(2*pi*j*(i-32)/8000);
	   }
       temp_fir[i]=temp/4000;
       temp_fir[64-i]= temp/4000;
   }
   for (i=0;i<65;i++)
   {
	   fir_coef[i]=(int)floor(temp_fir[i]*4096+0.5);
   }

}
void SpecialRectOfCoefficientEx::audio_calculate_responese_65(
	  int * fir_coef, //fir_eq系数
	  int * frequency_set,//频率设置
	  int  frequency_scale,//频率设置尺度
      double * fir_gain_dB//输出频响
	  )
{
	double temp_real;
	double temp_image;
	double temp;
	double pi=3.1415926;
	int i ,j;
   if (frequency_scale==1)  //1/12 octave
   {
	   for (i=0;i<65;i++)
	   {
		   temp_real=0;
		   temp_image=0;
		   for (j=0;j<65;j++)
		   {
			   //origin_dB(i) = origin_dB(i)+Origin_FIR_par(j)*(cos(2*pi*(j-1)*frequencies(i)/8000)-sqrt(-1)*sin(2*pi*(j-1)*frequencies(i)/8000));
			  temp_real=temp_real+fir_coef[j]*cos(2*pi*j*frequency_set[i]/8000);
              temp_image=temp_image-fir_coef[j]*sin(2*pi*j*frequency_set[i]/8000); 
		   }
		   temp=temp_real*temp_real+temp_image*temp_image;
           temp=sqrt(temp)/4096;
           fir_gain_dB[i]=20*log10(temp);
	   }
   }
   else  //1/6 octave
   {
       for (i=0;i<33;i++)
	   {
		   temp_real=0;
		   temp_image=0;
		   for (j=0;j<65;j++)
		   {
			   //origin_dB(i) = origin_dB(i)+Origin_FIR_par(j)*(cos(2*pi*(j-1)*frequencies(i)/8000)-sqrt(-1)*sin(2*pi*(j-1)*frequencies(i)/8000));
			  temp_real=temp_real+fir_coef[j]*cos(2*pi*j*frequency_set[i]/8000);
              temp_image=temp_image-fir_coef[j]*sin(2*pi*j*frequency_set[i]/8000); 
		   }
		   temp=temp_real*temp_real+temp_image*temp_image;
           temp=sqrt(temp)/4096;
           fir_gain_dB[i]=20*log10(temp);
	   }
   }
}

int SpecialRectOfCoefficientEx::ClickAction(CWnd* pParent)
{
	UNREFERENCED_PARAMETER(pParent);
    /*
    CCoefficientDlg dlg(pParent);
    dlg.m_pCoefficientArea = this;
    return dlg.DoModal();   
    */
    ASSERT( FALSE );
    return 0;
}

const double SpecialRectOfCoefficientEx::PI = 3.1415926;
const double SpecialRectOfCoefficientEx::m_dMinXValue = 100.0;
const double SpecialRectOfCoefficientEx::m_dMaxXValue = 4000.0;
const int SpecialRectOfCoefficientEx::m_frequency_scale = 1;

int SpecialRectOfCoefficientEx::m_iXScale[X_SCALE_COUNT_FIR_EX] = 
{100, 106, 112, 119, 126, 133, 141, 150, 159, 168, 178, 189, 200, 
212, 224 ,238, 252, 267, 283, 300, 318, 336, 356, 378, 400,
424, 449, 476, 504, 534, 566, 599, 635, 673, 713, 755, 800,
848, 898, 951, 1008, 1068, 1131, 1199, 1270, 1345, 1425, 1510, 1600,
1695, 1796, 1903, 2016, 2136, 2263, 2397, 2540, 2691, 2851, 3020, 3200,
3390, 3592, 3806, 4000};

// caculate all 65 coefficents
void SpecialRectOfCoefficientEx::CalculateCoefficients()
{
    audio_fir_eq_65(m_iXScale,m_vPoValue,m_frequency_scale,m_vCoefficients);
}

//restore to default value   , especially set the m_rectArea as m_rectDefaultArea
//cause when we scroll the window   ,  we will change the value of m_rectArea
void SpecialRectOfCoefficientEx::SetToDefault()
{
    SpecialRect::SetToDefault();
} 

//calculate all frequence curve x value and y value
void SpecialRectOfCoefficientEx::CalculateFreqGraphyData(int iXScaleCount /*=500*/ )
{
    m_vAllFreqItem.clear();
    //x--->f   ,  y--->db
    double dXScaleStep = (m_dMaxXValue - m_dMinXValue)/iXScaleCount;
    
    double dXScale=m_dMinXValue;
    double dMaxYValue = 10.0;
    
    for( int i=0; i<iXScaleCount; ++i )
    {        
        double dYScale =0.0;
        std::complex<float> total;
        for( int j=0; j<COLUMN_COUNT_EX; ++j )
        {
            float dAngle = (float)(2.0*PI*dXScale*j/8000.0);
            
            float dR = (float)(m_vCoefficients[ j ] * cos(dAngle));
            float dI = (float)(-m_vCoefficients[ j ] * sin(dAngle));            
            std::complex<float> temp(dR   ,  dI );
            total += temp;            
        }
        struct _complex   totalCType;
        totalCType.x = total.real();
        totalCType.y = total.imag();
        dYScale = _cabs(totalCType);
       
        dYScale = 20* log10(dYScale/4096.0 );            
       
        FreqItem tempItem;
        tempItem.first = log10( dXScale );
        tempItem.second = dYScale;
        m_vAllFreqItem.push_back( tempItem );
        dXScale += dXScaleStep;
    }
    m_dMaxYValue = dMaxYValue;    
}

//calculate all frequence curve x value and y value
void SpecialRectOfCoefficientEx::CalculateFreqGraphyDataEx(double dX[65], int iXScaleCount /*=65*/ )
{
    m_vAllFreqItem.clear();
    //x--->f   ,  y--->db
    double dXScaleStep = (m_dMaxXValue - m_dMinXValue)/iXScaleCount;
    
    double dMaxYValue = 10.0;
    
    for( int i=0; i<iXScaleCount; ++i )
    {        
        double dYScale =0.0;
        std::complex<float> total;
        for( int j=0; j<COLUMN_COUNT_EX; ++j )
        {
            float dAngle = (float)(2.0*PI*dX[i]*j/8000.0);
            
            float dR = (float)(m_vCoefficients[ j ] * cos(dAngle));
            float dI = (float)(-m_vCoefficients[ j ] * sin(dAngle));            
            std::complex<float> temp(dR   ,  dI );
            total += temp;            
        }
        struct _complex   totalCType;
        totalCType.x = total.real();
        totalCType.y = total.imag();
        dYScale = _cabs(totalCType);
       
        dYScale = 20* log10(dYScale/4096.0 );             
       
        FreqItem tempItem;
        tempItem.first = log10( dX[i] );
        tempItem.second = dYScale;
        m_vAllFreqItem.push_back( tempItem );
    }
    m_dMaxYValue = dMaxYValue;    
}

void SpecialRectOfCoefficientEx::CalculatePowerData()
{
    audio_calculate_responese_65(m_vCoefficients,m_iXScale,m_frequency_scale,m_vPoValue);
}

void SpecialRectOfCoefficientEx::Write(CArchive &file)const
{
    for( int i=0; i<ROW_COUNT_EX; ++i )
    {
        file << m_vPoValue[ i ];        
    }
}

void SpecialRectOfCoefficientEx::Read(CArchive &file)
{
    for( int i=0; i<ROW_COUNT_EX; ++i )
    {
        file >> m_vPoValue[ i ];
    }
    CalculateCoefficients();
    CalculateFreqGraphyData();
}
void SpecialRectOfCoefficientEx::Fetch(AUDIO_MODE& _value)const
{
    switch( m_enumParaType )
    {
    case dl_fir_eq_coeff:
        {
            for( int i=0;i<FIR_EQ_COUNT;i++ )
            {
                _value.audio_data.st_fir_eq_coeff[i] = (WORD)m_vCoefficients[i];
            }
        }
        break;
    case ul_fir_eq_coeff:
        {
            for( int i=0;i<FIR_EQ_COUNT;i++ )
            {
                _value.audio_data.ul_fir_eq_coeff[i] = (WORD)m_vCoefficients[i];
            }
        }
        break;
	default:
		break;
    }
}

void SpecialRectOfCoefficientEx::SetValue( AUDIO_STRUCT& _value )
{
    switch( m_enumParaType )
    {
    case dl_fir_eq_coeff:
        {
            for( int i=0;i<FIR_EQ_COUNT;i++ )
            {
                m_vCoefficients[i] = short(_value.st_fir_eq_coeff[i]);
            }
            for( i=0;i<FIR_EQ_COUNT-1;i++ )
            {
                m_vCoefficients[FIR_EQ_COUNT-1-i] = short(_value.st_fir_eq_coeff[i]);
            }
        }
        break;
    case ul_fir_eq_coeff:
        {
            for( int i=0;i<FIR_EQ_COUNT;i++ )
            {
                m_vCoefficients[i] = short(_value.ul_fir_eq_coeff[i]);
            }
            for( i=0;i<FIR_EQ_COUNT-1;i++ )
            {
                m_vCoefficients[FIR_EQ_COUNT-1-i] = short(_value.ul_fir_eq_coeff[i]);
            }
        }
        break;
	default:
		break;
    }

    CalculatePowerData();
}


//set a set of special data into coefficients
void SpecialRectOfCoefficientEx::SetSpecialData()
{
   ZeroMemory( m_vCoefficients   ,  sizeof(double)*COLUMN_COUNT_EX);
   
   m_vCoefficients[ COLUMN_COUNT_EX / 2 ] = 0X7FFF;
}

// formate tool tips string
CString SpecialRectOfCoefficientEx::GetToolTips()const
{    
    CString strToolTips;
    for( int i=0; i<COLUMN_COUNT_EX; ++i )
    {
        CString strSingleLine;
        strSingleLine.Format(_T("%s[%d] = %d")   , m_strName   , i   , Round(m_vCoefficients[i]) );
        
        strToolTips += strSingleLine;
        strToolTips += "\r\n";
    }
    
    return strToolTips;
}

/*********************************************************/
// SpecialRectOfDlSettings
/*********************************************************/
SpecialRectOfDlSettings::SpecialRectOfDlSettings()
{
	m_dl_no_vad_cnt_thd = 0;
	m_dl_min_psne = 0;
	m_dl_max_temp_uamn = 0;
	m_dl_vad_thd = 0;
	m_dl_active_thd = 0;
	m_dl_noise_thd = 0;
	m_dl_max_psne = 0;
	m_dl_voise_burst =0 ;
	m_dl_noise_tail = 0;
	m_dl_rfilter_delay = 0;
	m_dl_rfilter_tail = 0;
	m_dl_rfilter = 0;
	m_dl_dgain = 0;
	m_dl_sim_M = 0;
	m_dl_sim_fac = 0;
	m_dl_dac_limt = 0;
	m_dl_ns_factor = 0;
	m_dl_ns_limit = 0;
	m_dl_dis_snr_thd = 0;
	m_dl_dis_band_1k = 0;
	m_dl_dis_limit = 0;
	m_dl_drc_thd = 0;
	m_dl_drc_ratio = 0;
	m_dl_drc_dstep = 0;
	m_dl_drc_ustep = 0;
	m_dl_drc_cnt = 0;
	m_dl_clarity_switch = 0;
	m_dl_vad_switch = 0;
	m_dl_complex_switch = 0;
	m_dl_complex_switch = 0;
	m_dl_noise_est_switch = 0;
	m_dl_nr_switch = 0;
	m_dl_nr_switch = 0;
	m_dl_nr_switch = 0;
	m_dl_dis_switch = 0;
	m_dl_amb_noise_est_switch = 0;
	m_dl_amb_eq_switch = 0;
	m_dl_acoustic_eq_switch = 0;
	m_dl_drc_switch = 0;
	m_dl_dac_limit_switch = 0;
	m_dl_hpf_set = 0;
	m_dl_lpf_set = 0;
	m_dl_ns_up_factor = 0;
}

void SpecialRectOfDlSettings::Fetch(AUDIO_MODE& _value)const
{
    _value.audio_data.dl_no_vad_cnt_thd = m_dl_no_vad_cnt_thd;
    _value.audio_data.dl_min_psne = m_dl_min_psne;
    _value.audio_data.dl_max_temp_uamn = m_dl_max_temp_uamn;
    _value.audio_data.dl_vad_thd = m_dl_vad_thd;
    _value.audio_data.dl_active_thd = m_dl_active_thd;
    _value.audio_data.dl_noise_thd = m_dl_noise_thd;
    _value.audio_data.dl_max_psne = m_dl_max_psne;
    _value.audio_data.dl_voise_burst = m_dl_voise_burst;
    _value.audio_data.dl_noise_tail = m_dl_noise_tail;
    _value.audio_data.dl_rfilter_delay = m_dl_rfilter_delay;
    _value.audio_data.dl_rfilter_tail = m_dl_rfilter_tail;
    _value.audio_data.dl_rfilter = m_dl_rfilter;
    _value.audio_data.dl_dgain = m_dl_dgain;
    _value.audio_data.dl_sim_M = m_dl_sim_M;
    _value.audio_data.dl_sim_fac = m_dl_sim_fac;
    _value.audio_data.dl_dac_limt = m_dl_dac_limt;
    _value.audio_data.dl_ns_factor = m_dl_ns_factor;
    _value.audio_data.dl_ns_limit = m_dl_ns_limit;
    _value.audio_data.dl_dis_snr_thd = m_dl_dis_snr_thd;
    _value.audio_data.dl_dis_band_1k = m_dl_dis_band_1k;
    _value.audio_data.dl_dis_limit = m_dl_dis_limit;
    _value.audio_data.dl_drc_thd = m_dl_drc_thd;
    _value.audio_data.dl_drc_ratio = m_dl_drc_ratio;
    _value.audio_data.dl_drc_dstep = m_dl_drc_dstep;
    _value.audio_data.dl_drc_ustep = m_dl_drc_ustep;
    _value.audio_data.dl_drc_cnt = m_dl_drc_cnt;

	_value.audio_data.dl_clarity_switch = m_dl_clarity_switch;
    _value.audio_data.dl_vad_switch = m_dl_vad_switch;
    _value.audio_data.dl_complex_switch = m_dl_complex_switch;
    _value.audio_data.dl_noise_est_switch = m_dl_noise_est_switch;
    _value.audio_data.dl_nr_switch = m_dl_nr_switch;
    
    _value.audio_data.dl_dis_switch = m_dl_dis_switch;
    _value.audio_data.dl_amb_noise_est_switch = m_dl_amb_noise_est_switch;
    _value.audio_data.dl_amb_eq_switch = m_dl_amb_eq_switch;
    _value.audio_data.dl_acoustic_eq_switch = m_dl_acoustic_eq_switch;
    
    _value.audio_data.dl_drc_switch = m_dl_drc_switch;
    _value.audio_data.dl_dac_limit_switch = m_dl_dac_limit_switch;
    _value.audio_data.dl_hpf_set = m_dl_hpf_set;
    _value.audio_data.dl_lpf_set = m_dl_lpf_set;

	_value.audio_data.dl_ns_up_factor = m_dl_ns_up_factor;

}

//pop up a dialog to modify related value 
int SpecialRectOfDlSettings::ClickAction(CWnd* pParent)
{
	UNREFERENCED_PARAMETER(pParent);
    return -1;
}

void SpecialRectOfDlSettings::Write( CArchive& file )const
{
	UNREFERENCED_PARAMETER(file);
    // Should not go here
    ASSERT( 0 );
}

void SpecialRectOfDlSettings::Read( CArchive& file )
{
	UNREFERENCED_PARAMETER(file);
    // Should not go here
    ASSERT( 0 );
}

void SpecialRectOfDlSettings::SetValue( AUDIO_STRUCT& value )
{
    m_dl_no_vad_cnt_thd = value.dl_no_vad_cnt_thd;
    m_dl_min_psne = value.dl_min_psne;
    m_dl_max_temp_uamn = value.dl_max_temp_uamn;
    m_dl_vad_thd = value.dl_vad_thd;
    m_dl_active_thd = value.dl_active_thd;
    m_dl_noise_thd = value.dl_noise_thd;
    m_dl_max_psne = value.dl_max_psne;
    m_dl_voise_burst = value.dl_voise_burst;
    m_dl_noise_tail = value.dl_noise_tail;
    m_dl_rfilter_delay = value.dl_rfilter_delay;
    m_dl_rfilter_tail = value.dl_rfilter_tail;
    m_dl_rfilter = value.dl_rfilter;
    m_dl_dgain = value.dl_dgain;
    m_dl_sim_M = value.dl_sim_M;
    m_dl_sim_fac = value.dl_sim_fac;
    m_dl_dac_limt = value.dl_dac_limt;
    m_dl_ns_factor = value.dl_ns_factor;
    m_dl_ns_limit = value.dl_ns_limit;
    m_dl_dis_snr_thd = value.dl_dis_snr_thd;
    m_dl_dis_band_1k = value.dl_dis_band_1k;
    m_dl_dis_limit = value.dl_dis_limit;
    m_dl_drc_thd = value.dl_drc_thd;
    m_dl_drc_ratio = value.dl_drc_ratio;
    m_dl_drc_dstep = value.dl_drc_dstep;
    m_dl_drc_ustep = value.dl_drc_ustep;
    m_dl_drc_cnt = value.dl_drc_cnt;

	m_dl_clarity_switch = value.dl_clarity_switch;
    m_dl_vad_switch = value.dl_vad_switch;
    m_dl_complex_switch = value.dl_complex_switch;
    m_dl_noise_est_switch = value.dl_noise_est_switch;
    m_dl_nr_switch = value.dl_nr_switch;
    
    m_dl_dis_switch = value.dl_dis_switch;
    m_dl_amb_noise_est_switch = value.dl_amb_noise_est_switch;
    m_dl_amb_eq_switch = value.dl_amb_eq_switch;
    m_dl_acoustic_eq_switch = value.dl_acoustic_eq_switch;
    
    m_dl_drc_switch = value.dl_drc_switch;
    m_dl_dac_limit_switch = value.dl_dac_limit_switch;
    m_dl_hpf_set = value.dl_hpf_set;
    m_dl_lpf_set = value.dl_lpf_set;

	m_dl_ns_up_factor = value.dl_ns_up_factor;
}

/*********************************************************/
// SpecialRectOfUlSettings
/*********************************************************/
SpecialRectOfUlSettings::SpecialRectOfUlSettings()
{
	m_ul_no_vad_cnt_thd = 0;
	m_ul_min_psne = 0;
	m_ul_max_temp_uamn = 0;
	m_ul_vad_thd = 0;
	m_ul_active_thd = 0;
	m_ul_noise_thd = 0;
	m_ul_max_psne = 0;
	m_ul_max_psne = 0;
	m_ul_voise_burst = 0;
	m_ul_noise_tail = 0;
	m_ul_rfilter_delay = 0;
	m_ul_rfilter_tail = 0;
	m_ul_rfilter = 0;
	m_ul_dgain = 0;
	m_ul_sim_M = 0;
	m_ul_sim_fac = 0;
	m_ul_ns_factor = 0;
	m_ul_ns_limit = 0;
	m_ul_dis_snr_thd = 0;
	m_ul_dis_band_1k = 0;
	m_ul_dis_limit = 0;
	m_ul_drc_thd= 0;
	m_ul_drc_ratio = 0;
	m_ul_drc_dstep = 0;
	m_ul_drc_ustep = 0;
	m_ul_drc_cnt = 0;
	m_ul_clarity_switch = 0;
	m_aec_sa_in_select = 0;
	m_aec_sa_off = 0;
	m_aec_coeff_frozen = 0;
	m_ul_vad_switch = 0;
	m_ul_complex_switch = 0;
	m_ul_noise_est_switch = 0;
	m_ul_nr_switch = 0;
	m_ul_dis_switch = 0;
	m_ul_amb_noise_est_switch = 0;
	m_ul_amb_eq_switch= 0;
	m_ul_drc_switch = 0;
	m_ul_echo_residual_switch = 0;
	m_ul_hpf_set = 0;
	m_ul_lpf_set = 0;
	m_ul_amb_attack = 0;
	m_ul_amb_ndefault = 0;
	m_ul_amb_release = 0;
	m_ul_dac_limit = 0;
	m_ul_echo_ns_limit = 0;
	m_ul_ns_up_factor = 0;
}

void SpecialRectOfUlSettings::Fetch(AUDIO_MODE& _value)const
{
    _value.audio_data.ul_no_vad_cnt_thd = m_ul_no_vad_cnt_thd;
    _value.audio_data.ul_min_psne = m_ul_min_psne;
    _value.audio_data.ul_max_temp_uamn = m_ul_max_temp_uamn;
    _value.audio_data.ul_vad_thd = m_ul_vad_thd;
    _value.audio_data.ul_active_thd = m_ul_active_thd;
    _value.audio_data.ul_noise_thd = m_ul_noise_thd;
    _value.audio_data.ul_max_psne = m_ul_max_psne;
    _value.audio_data.ul_voise_burst = m_ul_voise_burst;
    _value.audio_data.ul_noise_tail = m_ul_noise_tail;
    _value.audio_data.ul_rfilter_delay = m_ul_rfilter_delay;
    _value.audio_data.ul_rfilter_tail = m_ul_rfilter_tail;
    _value.audio_data.ul_rfilter = m_ul_rfilter;
    _value.audio_data.ul_dgain = m_ul_dgain;
    _value.audio_data.ul_sim_M = m_ul_sim_M;
    _value.audio_data.ul_sim_fac = m_ul_sim_fac;
    _value.audio_data.ul_ns_factor = m_ul_ns_factor;
    _value.audio_data.ul_ns_limit = m_ul_ns_limit;
    _value.audio_data.ul_dis_snr_thd = m_ul_dis_snr_thd;
    _value.audio_data.ul_dis_band_1k = m_ul_dis_band_1k;
    _value.audio_data.ul_dis_limit = m_ul_dis_limit;
    _value.audio_data.ul_drc_thd = m_ul_drc_thd;
    _value.audio_data.ul_drc_ratio = m_ul_drc_ratio;
    _value.audio_data.ul_drc_dstep = m_ul_drc_dstep;
    _value.audio_data.ul_drc_ustep = m_ul_drc_ustep;
    _value.audio_data.ul_drc_cnt = m_ul_drc_cnt;

	_value.audio_data.ul_clarity_switch = m_ul_clarity_switch;
    
    _value.audio_data.aec_sa_in_select = m_aec_sa_in_select;
    _value.audio_data.aec_sa_off = m_aec_sa_off;
    _value.audio_data.aec_coeff_frozen = m_aec_coeff_frozen;
    
    _value.audio_data.ul_vad_switch = m_ul_vad_switch;
    _value.audio_data.ul_complex_switch = m_ul_complex_switch;
    _value.audio_data.ul_noise_est_switch = m_ul_noise_est_switch;
    _value.audio_data.ul_nr_switch = m_ul_nr_switch;
    
    _value.audio_data.ul_dis_switch = m_ul_dis_switch;
    _value.audio_data.ul_amb_noise_est_switch = m_ul_amb_noise_est_switch;
    _value.audio_data.ul_amb_eq_switch = m_ul_amb_eq_switch;
    
    _value.audio_data.ul_drc_switch = m_ul_drc_switch;
    _value.audio_data.ul_echo_residual_switch = m_ul_echo_residual_switch;
    _value.audio_data.ul_hpf_set = m_ul_hpf_set;
    _value.audio_data.ul_lpf_set = m_ul_lpf_set;

	_value.audio_data.ul_amb_attack = m_ul_amb_attack;
	_value.audio_data.ul_amb_ndefault = m_ul_amb_ndefault;
	_value.audio_data.ul_amb_release = m_ul_amb_release;
	_value.audio_data.ul_dac_limit = m_ul_dac_limit;
	_value.audio_data.ul_echo_ns_limit = m_ul_echo_ns_limit;
	_value.audio_data.ul_ns_up_factor = m_ul_ns_up_factor;

}

//pop up a dialog to modify related value 
int SpecialRectOfUlSettings::ClickAction(CWnd* pParent)
{
	UNREFERENCED_PARAMETER(pParent);
    return -1;
}

void SpecialRectOfUlSettings::Write( CArchive& file )const
{
	UNREFERENCED_PARAMETER(file);
    // Should not go here
    ASSERT( 0 );
}

void SpecialRectOfUlSettings::Read( CArchive& file )
{
	UNREFERENCED_PARAMETER(file);
    // Should not go here
    ASSERT( 0 );
}

void SpecialRectOfUlSettings::SetValue( AUDIO_STRUCT& value )
{
    m_ul_no_vad_cnt_thd = value.ul_no_vad_cnt_thd;
    m_ul_min_psne = value.ul_min_psne;
    m_ul_max_temp_uamn = value.ul_max_temp_uamn;
    m_ul_vad_thd = value.ul_vad_thd;
    m_ul_active_thd = value.ul_active_thd;
    m_ul_noise_thd = value.ul_noise_thd;
    m_ul_max_psne = value.ul_max_psne;
    m_ul_voise_burst = value.ul_voise_burst;
    m_ul_noise_tail = value.ul_noise_tail;
    m_ul_rfilter_delay = value.ul_rfilter_delay;
    m_ul_rfilter_tail = value.ul_rfilter_tail;
    m_ul_rfilter = value.ul_rfilter;
    m_ul_dgain = value.ul_dgain;
    m_ul_sim_M = value.ul_sim_M;
    m_ul_sim_fac = value.ul_sim_fac;
    m_ul_ns_factor = value.ul_ns_factor;
    m_ul_ns_limit = value.ul_ns_limit;
    m_ul_dis_snr_thd = value.ul_dis_snr_thd;
    m_ul_dis_band_1k = value.ul_dis_band_1k;
    m_ul_dis_limit = value.ul_dis_limit;
    m_ul_drc_thd = value.ul_drc_thd;
    m_ul_drc_ratio = value.ul_drc_ratio;
    m_ul_drc_dstep = value.ul_drc_dstep;
    m_ul_drc_ustep = value.ul_drc_ustep;
    m_ul_drc_cnt = value.ul_drc_cnt;

	m_ul_clarity_switch = value.ul_clarity_switch;
    
    m_aec_sa_in_select = value.aec_sa_in_select;
    m_aec_sa_off = value.aec_sa_off;
    m_aec_coeff_frozen = value.aec_coeff_frozen;
    
    m_ul_vad_switch = value.ul_vad_switch;
    m_ul_complex_switch = value.ul_complex_switch;
    m_ul_noise_est_switch = value.ul_noise_est_switch;
    m_ul_nr_switch = value.ul_nr_switch;
    
    m_ul_dis_switch = value.ul_dis_switch;
    m_ul_amb_noise_est_switch = value.ul_amb_noise_est_switch;
    m_ul_amb_eq_switch = value.ul_amb_eq_switch;
    
    m_ul_drc_switch = value.ul_drc_switch;
    m_ul_echo_residual_switch = value.ul_echo_residual_switch;
    m_ul_hpf_set = value.ul_hpf_set;
    m_ul_lpf_set = value.ul_lpf_set;

	m_ul_amb_attack = value.ul_amb_attack;
	m_ul_amb_ndefault = value.ul_amb_ndefault;
	m_ul_amb_release = value.ul_amb_release;
	m_ul_dac_limit = value.ul_dac_limit;
	m_ul_echo_ns_limit = value.ul_echo_ns_limit;
	m_ul_ns_up_factor = value.ul_ns_up_factor;

}

/*********************************************************/
// SpecialRectOfStng
/*********************************************************/
SpecialRectOfStng::SpecialRectOfStng()
{
	m_st_ng_switch = 0;
	m_st_voice_threshold = 0;
	m_st_noise_threshold = 0;
	m_st_holdc = 0;
	m_st_att = 0;
	m_st_zcr_threshold = 0;
	m_st_zcr_ceil = 0;
}

void SpecialRectOfStng::Fetch(AUDIO_MODE& _value)const
{
	_value.audio_data.sidetone_switch &= 0xefff;
	_value.audio_data.sidetone_switch |= ( m_st_ng_switch << 12 );
    _value.audio_data.st_voice_threshold = (WORD)m_st_voice_threshold;
    _value.audio_data.st_noise_threshold = (WORD)m_st_noise_threshold;
    _value.audio_data.st_holdc = (WORD)m_st_holdc;
    _value.audio_data.st_att = (WORD)m_st_att;
    _value.audio_data.st_zcr_threshold = (WORD)m_st_zcr_threshold;
    _value.audio_data.st_zcr_ceil = (WORD)m_st_zcr_ceil;
}

//pop up a dialog to modify related value 
int SpecialRectOfStng::ClickAction(CWnd* pParent)
{
	UNREFERENCED_PARAMETER(pParent);
    return -1;
}

void SpecialRectOfStng::Write( CArchive& file )const
{
	UNREFERENCED_PARAMETER(file);
    // Should not go here
    ASSERT( 0 );
}

void SpecialRectOfStng::Read( CArchive& file )
{
	UNREFERENCED_PARAMETER(file);
    // Should not go here
    ASSERT( 0 );
}

void SpecialRectOfStng::SetValue( AUDIO_STRUCT& value )
{
	m_st_ng_switch = ( value.sidetone_switch & 0x1000 ) >> 12;
	m_st_voice_threshold = value.st_voice_threshold;
	m_st_noise_threshold = value.st_noise_threshold;
	m_st_holdc = (short)value.st_holdc;
	m_st_att = value.st_att;
	m_st_zcr_threshold = value.st_zcr_threshold;
	m_st_zcr_ceil = value.st_zcr_ceil;
}

/*********************************************************/
// SpecialRectOfVolume
/*********************************************************/
SpecialRectOfVolume::SpecialRectOfVolume()
{
	memset(&m_dl_st_gain, 0, sizeof(VOLUME_LEVEL));
	memset(&m_dl_PGA0, 0, sizeof(VOLUME_LEVEL));
	memset(&m_dl_digital_gain, 0, sizeof(VOLUME_LEVEL));
	//teana hu 2009.09.03
	memset(&m_ul_digital_gain, 0, sizeof(VOLUME_LEVEL));
	//
	memset(&m_aec_vol_comp_gain, 0, sizeof(VOLUME_LEVEL));
	memset(&m_ul_PGA0, 0, sizeof(VOLUME_LEVEL));
	
	m_nLevel = 0;
	m_nStep = 0;
	m_bDspCodecInfo = FALSE;
	m_nInnerPA = 0;
}

void SpecialRectOfVolume::Fetch(AUDIO_MODE& _value)const
{
	//teana hu 2012.06.13
	_value.audio_data.dl_pga_gain_h = (WORD)m_nInnerPA;
	//
	//teana hu 2011.05.04
	if(m_nLevel > VOLUME_LEVEL_BASE)
	{
		_value.audio_data.dsp_volume[0] = m_nStep;
		_value.audio_data.dsp_volume[1] = m_nLevel;
	}

	for( int i=0;i<VOLUME_LEVEL_BASE;i++ )
	{
		_value.audio_data.st_dgain[i] = (WORD)m_dl_st_gain[i+m_nLevel-VOLUME_LEVEL_BASE];
		if(!m_bDspCodecInfo)
		{
			_value.audio_data.dl_PGA0[i] = (WORD)m_dl_PGA0[i+m_nLevel-VOLUME_LEVEL_BASE];
		}
		_value.audio_data.dl_d_gain[i] = (WORD)m_dl_digital_gain[i+m_nLevel-VOLUME_LEVEL_BASE];
		_value.audio_data.ul_dgain1[i] = (WORD)m_ul_digital_gain[i+m_nLevel-VOLUME_LEVEL_BASE];
		_value.audio_data.ref_dgain[i] = (WORD)m_aec_vol_comp_gain[i+m_nLevel-VOLUME_LEVEL_BASE];
		if(!m_bDspCodecInfo)
		{
			_value.audio_data.ul_PGA0[i] = (WORD)m_ul_PGA0[i+m_nLevel-VOLUME_LEVEL_BASE];
		}
	}
	//

	/*
	for( int i=0;i<VOLUME_LEVEL;i++ )
	{
		_value.audio_data.st_dgain[i] = (WORD)m_dl_st_gain[i];
		_value.audio_data.dl_PGA0[i] = (WORD)m_dl_PGA0[i];
		_value.audio_data.dl_d_gain[i] = (WORD)m_dl_digital_gain[i];
		//teana hu 2009.09.03
		_value.audio_data.ul_dgain1[i] = (WORD)m_ul_digital_gain[i];
		//
		_value.audio_data.ref_dgain[i] = (WORD)m_aec_vol_comp_gain[i];
		_value.audio_data.ul_PGA0[i] = (WORD)m_ul_PGA0[i];
	}
	*/
}

void SpecialRectOfVolume::FetchDspCodecValue(AUDIO_NV_DSP_MODE_STRUCT_T& _value)
{
	for(int i = 0; i < m_nLevel6530; i++)
	{
		_value.vol_cfg1[i+1] = m_dl_PGA0[i] | (m_dl_PGA0[i] << 8) ;
		_value.vol_cfg0[i+1] = m_ul_PGA0[i];
	}
}

//pop up a dialog to modify related value 
int SpecialRectOfVolume::ClickAction(CWnd* pParent)
{
	UNREFERENCED_PARAMETER(pParent);
    return -1;
}

void SpecialRectOfVolume::Write( CArchive& file )const
{
	UNREFERENCED_PARAMETER(file);
    // Should not go here
    ASSERT( 0 );
}

void SpecialRectOfVolume::Read( CArchive& file )
{
	UNREFERENCED_PARAMETER(file);
    // Should not go here
    ASSERT( 0 );
}

void SpecialRectOfVolume::SetDspCodecValue(AUDIO_NV_DSP_MODE_STRUCT_T& value)
{
	for(int i = 0; i < m_nLevel6530; i++)
	{
		m_dl_PGA0[i] = value.vol_cfg1[i+1] & 0xFF;
		m_ul_PGA0[i] = value.vol_cfg0[i+1];
	}
}

void SpecialRectOfVolume::SetValue( AUDIO_STRUCT& value )
{
	//teana hu 2012.06.13
	m_nInnerPA = value.dl_pga_gain_h;
	//
	//teana hu 2011.05.04
	if(value.dsp_volume[1] <= VOLUME_LEVEL_BASE)
	{
		m_nLevel = VOLUME_LEVEL_BASE;
		for(int i = 0; i < VOLUME_LEVEL_BASE; i++)
		{
			m_dl_st_gain[i] = value.st_dgain[i];
			m_dl_PGA0[i] = value.dl_PGA0[i];
			m_dl_digital_gain[i] = value.dl_d_gain[i];
			//teana hu 2009.09.03
			m_ul_digital_gain[i] = value.ul_dgain1[i];
			//
			m_aec_vol_comp_gain[i] = value.ref_dgain[i];
			m_ul_PGA0[i] = value.ul_PGA0[i];
		}
	}
	else if(value.dsp_volume[1] > VOLUME_LEVEL_BASE)
	{
		m_nLevel = value.dsp_volume[1];
		m_nStep = value.dsp_volume[0];
		for(int i = 0 ; i < VOLUME_LEVEL_BASE; i++)
		{
			m_dl_st_gain[i+m_nLevel-VOLUME_LEVEL_BASE] = value.st_dgain[i];
			m_dl_PGA0[i+m_nLevel-VOLUME_LEVEL_BASE] = value.dl_PGA0[i];
			m_dl_digital_gain[i+m_nLevel-VOLUME_LEVEL_BASE] = value.dl_d_gain[i];
			m_ul_digital_gain[i+m_nLevel-VOLUME_LEVEL_BASE] = value.ul_dgain1[i];
			m_aec_vol_comp_gain[i+m_nLevel-VOLUME_LEVEL_BASE] = value.ref_dgain[i];
			m_ul_PGA0[i+m_nLevel-VOLUME_LEVEL_BASE] = value.ul_PGA0[i];
		}

		for(i = 0; i < m_nLevel - VOLUME_LEVEL_BASE; i++)
		{
			m_dl_st_gain[i] = m_dl_st_gain[m_nLevel-VOLUME_LEVEL_BASE];
			m_dl_PGA0[i] = m_dl_PGA0[m_nLevel-VOLUME_LEVEL_BASE];
			m_dl_digital_gain[i] = m_dl_digital_gain[m_nLevel-VOLUME_LEVEL_BASE];
			m_ul_digital_gain[i] = m_ul_digital_gain[m_nLevel-VOLUME_LEVEL_BASE];
			m_aec_vol_comp_gain[i] = m_aec_vol_comp_gain[m_nLevel-VOLUME_LEVEL_BASE];
			m_ul_PGA0[i] = m_ul_PGA0[m_nLevel-VOLUME_LEVEL_BASE];
		}
	}
	//
	/*
	for( int i=0;i<VOLUME_LEVEL;i++ )
	{
		m_dl_st_gain[i] = value.st_dgain[i];
		m_dl_PGA0[i] = value.dl_PGA0[i];
		m_dl_digital_gain[i] = value.dl_d_gain[i];
		//teana hu 2009.09.03
		m_ul_digital_gain[i] = value.ul_dgain1[i];
		//
		m_aec_vol_comp_gain[i] = value.ref_dgain[i];
		m_ul_PGA0[i] = value.ul_PGA0[i];
	}
	*/
}

/*********************************************************/
// SpecialRectOfArmVolume
/*********************************************************/
SpecialRectOfArmVolume::SpecialRectOfArmVolume()
{
	memset(&m_armVolume, 0, sizeof(ARM_VOLUME_SIZE));
	memset(&m_armVolumeMul, 0, sizeof(m_armVolumeMul));
	memset(&m_narmVolumeLevel, 0, sizeof(AUDIO_ARM_APP_TYPE_MAX)*sizeof(int));
	m_nCurType = 0;
	m_bOldVer = TRUE;
}

void SpecialRectOfArmVolume::Fetch(AUDIO_MODE& _value)const
{
	for( int i=0;i<ARM_VOLUME_SIZE;i++ )
	{
		_value.audio_data.arm_volume[i] = (WORD)m_armVolume[i];
	}	
}

//pop up a dialog to modify related value 
int SpecialRectOfArmVolume::ClickAction(CWnd* pParent)
{
	UNREFERENCED_PARAMETER(pParent);
    return -1;
}

void SpecialRectOfArmVolume::Write( CArchive& file )const
{
	UNREFERENCED_PARAMETER(file);
    // Should not go here
    ASSERT( 0 );
}

void SpecialRectOfArmVolume::Read( CArchive& file )
{
	UNREFERENCED_PARAMETER(file);
    // Should not go here
    ASSERT( 0 );
}

void SpecialRectOfArmVolume::SetValue( AUDIO_STRUCT& _value )
{
	for( int i=0;i<ARM_VOLUME_SIZE;i++ )
	{
		m_armVolume[i] = _value.arm_volume[i];
	}
}

void SpecialRectOfArmVolume::SetVolumeValue( VOLUME_STRUCT& value, int nIndex)
{
	for( int i=0;i<AUDIO_ARM_VOLUME_LEVEL;i++ )
	{
		m_armVolumeMul[nIndex][i] = value.app_config_info_set.app_config_info[nIndex].arm_volume[i];
	}
}

void SpecialRectOfArmVolume::FetchVolume(VOLUME_MODE& _value, int nIndex)
{
	for( int i=0;i<AUDIO_ARM_VOLUME_LEVEL;i++ )
	{
		_value.volume_data.app_config_info_set.app_config_info[nIndex].arm_volume[i] = (WORD)m_armVolumeMul[nIndex][i];
	}
}
