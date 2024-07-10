// PixelPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MSDev.h"
#include "PixelPropDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPixelPropDlg dialog


CPixelPropDlg::CPixelPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPixelPropDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPixelPropDlg)
	m_strCoordinate = _T("");
	m_strRgb = _T("");
	m_strRgb565 = _T("");
	//}}AFX_DATA_INIT

	m_Color = CLR_INVALID;
}


void CPixelPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPixelPropDlg)
	DDX_Text(pDX, IDC_STATIC_COORDINATE, m_strCoordinate);
	DDX_Text(pDX, IDC_STATIC_RGB24, m_strRgb);
	DDX_Text(pDX, IDC_STATIC_RGB565, m_strRgb565);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPixelPropDlg, CDialog)
	//{{AFX_MSG_MAP(CPixelPropDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPixelPropDlg message handlers
void CPixelPropDlg::SetCoordinate( int x,int y )
{
	if( x == -1 || y == -1 )
	{
		m_strCoordinate = "x =             y =       ";
	}
	else
	{
		m_strCoordinate.Format( "x = %3d(0x%03x)  y = %3d(0x%03x)",x,x,y,y );
	}

	UpdateData( FALSE );
}

void CPixelPropDlg::SetMsColor( DWORD dwColor,DWORD r_mask,DWORD g_mask,DWORD b_mask )
{
	if( dwColor != 0x80000000 )
	{
		DWORD r,g,b;
		
		r = GetColor( dwColor,r_mask );
		g = GetColor( dwColor,g_mask );
		b = GetColor( dwColor,b_mask );
		
		m_strRgb565.Format( "R:%3d G:%3d B:%3d RGB:0x%04x",r,g,b,dwColor );
	}
	else
	{
		m_strRgb565 = ( "R:    G:    B:    RGB: " );
	}

	UpdateData( FALSE );
}

DWORD CPixelPropDlg::GetColor( DWORD dwColor,DWORD dwMask )
{
	dwColor &= dwMask;

	while( ( dwMask & 1 ) == 0 )
	{
		dwColor >>= 1;
		dwMask >>= 1;
	}

	return dwColor;
}

void CPixelPropDlg::SetPcColor( COLORREF color )
{
	if( color == CLR_INVALID )
	{
		m_strRgb = "Can not retrieve screen color";
	}
	else
	{
		m_strRgb.Format( "R:%3d G:%3d B:%3d RGB:0x%06x", GetRValue( color ),GetGValue( color ),GetBValue( color ), color );
	}

	m_Color = color;
	GetDlgItem( IDC_STATIC_PANEL )->Invalidate();
	UpdateData( FALSE );
}

HBRUSH CPixelPropDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if( pWnd->m_hWnd == GetDlgItem( IDC_STATIC_PANEL )->m_hWnd )
	{
		if( CLR_INVALID != m_Color )
		{
			m_Brush.Detach();
			m_Brush.CreateSolidBrush( m_Color );
			return HBRUSH( m_Brush.m_hObject );
		}
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CPixelPropDlg::OnCancel() 
{
	// Hide window
	ShowWindow( SW_HIDE );
}

void CPixelPropDlg::OnOK() 
{
	// Do nothing
}
