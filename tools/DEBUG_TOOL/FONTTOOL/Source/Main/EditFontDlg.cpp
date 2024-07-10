// EditFontDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FontTool.h"
#include "EditFontDlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define   IsCtrlPressed()  ((GetKeyState(VK_CONTROL) & (1<<(sizeof(SHORT)*8-1)))!= 0) 

#define GET_WIDTH_BYTE(w,b)  ((((w)*(b)+31)/32)*4)

static const  int g_gray[] = {0,16,32,48,64,96,128,160,192,224,240,255};	

/////////////////////////////////////////////////////////////////////////////
// CEditFontDlg dialog


CEditFontDlg::CEditFontDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditFontDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditFontDlg)
	m_nGrayIdx = 0;
	//}}AFX_DATA_INIT
	m_bIsErase = FALSE;
	m_bIsPen = FALSE;
	m_bIsSelect = FALSE;
	m_bIsSelectDown = FALSE;
	m_bIsSelectUp = FALSE;
	m_bIsSelectMove = FALSE;
	m_bDraw = FALSE;
	m_bMove = FALSE;
	m_hinst = NULL;
	m_pBakDot = NULL;
	m_pModifyDot = NULL;

	m_nIndex = 0;
	m_nSize = 0;
	m_bIsModifyDown = FALSE;
	m_dMin = 1;
	m_nTotalDX = 0;
	m_nTotalDY = 0;
	m_nRange = 0;
	m_lx = 0;
	m_ty = 0;
	m_rx = 0;
	m_by = 0;
	m_bIsUndo = FALSE;

	m_nDeltX = 0;
	m_nDeltY = 0;
}


void CEditFontDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditFontDlg)
	DDX_Control(pDX, IDC_STATIC_EDITFONT, m_csEditFont);
	DDX_Control(pDX, IDC_STATIC_BROWSE, m_csBrowse);
	DDX_Control(pDX, IDC_BOX_PEN, m_csPen);
	DDX_Control(pDX, IDC_BOX_ERASE, m_csErase);
	DDX_Control(pDX, IDC_BOX_SELECT, m_csSelect);
	DDX_Control(pDX, IDC_BOX_UNDO, m_btnUndo);
	DDX_Control(pDX, IDC_SLIDER, m_sliderCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditFontDlg, CDialog)
	//{{AFX_MSG_MAP(CEditFontDlg)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_COMMAND(IDM_COPY, OnCopy)
	ON_COMMAND(IDM_PASTE, OnPaste)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_RDO_COLOR0, IDC_RDO_COLOR11,OnRdoColor)
	ON_COMMAND_RANGE( IDC_BOX_PEN , IDC_BOX_UNDO, OnClickFont )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditFontDlg message handlers

BOOL CEditFontDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_hinst = AfxGetInstanceHandle();

	//set bitmap for btn
	SetBtnBmp( &m_btnUndo, IDB_BITMAP_UNDO);
	SetBtnBmp( &m_csPen, IDB_BITMAP_PEN );
	SetBtnBmp( &m_csErase, IDB_BITMAP_ERASE );
	SetBtnBmp( &m_csSelect, IDB_BITMAP_SELECT );

	m_csEditFont.m_bDrawGrid = TRUE;
	m_csEditFont.m_bOnePixel = FALSE;
	m_csEditFont.m_bEdit = TRUE;

	m_csEditFont.m_nFBBX = m_csEditFont.m_nFBBX > m_csEditFont.m_nFBBY ? m_csEditFont.m_nFBBX: m_csEditFont.m_nFBBY;
	m_csEditFont.m_nFBBY = m_csEditFont.m_nFBBX;
	m_csBrowse.m_uBkColor = m_csEditFont.m_uBkColor = RGB(255,255,255);
	m_csBrowse.m_bIsDown = m_csEditFont.m_bIsDown = TRUE;
	m_csBrowse.m_nFBBX = m_csEditFont.m_nFBBX;
	m_csBrowse.m_nFBBY = m_csEditFont.m_nFBBY;
//	m_csBrowse.m_pChar = m_csEditFont.m_pChar;
	m_csBrowse.m_pGlyph = m_csEditFont.m_pGlyph;
	m_csBrowse.m_nMaxBearingY = m_csEditFont.m_nMaxBearingY;
	m_csBrowse.m_bAlpha = m_csEditFont.m_bAlpha;
	m_csBrowse.m_bSlider = TRUE;
	m_csBrowse.m_bOnePixel = FALSE;	
	m_csBrowse.m_bEdit = TRUE;

	
	InitColorCtrs();

	//set slider range
	m_nRange = GetBrsStcDmin();

	
	m_csBrowse.m_nDiff = -m_nRange;

	m_sliderCtrl.SetRangeMax(m_nRange);


	m_bIsPen = FALSE;
	m_bIsErase = FALSE;
	m_bIsSelect = FALSE;
	m_bIsSelectDown = FALSE;
	m_bIsModifyDown = FALSE;
	m_bIsSelectUp = TRUE;
	m_bMove = FALSE;
	m_bDraw = FALSE;

	m_FocusRect.SetRect(0,0,0,0);
	m_csEditFont.GetClientRect(&m_StcRect);
	m_nIndex = 0;

	m_lastPoint.x = 0;
	m_lastPoint.y = 0;
	m_nTotalDX = 0;
	m_nTotalDY = 0;
	
	//get dMin for edit static
	int dx = m_StcRect.Width()/m_csEditFont.m_nFBBX;
	int dy = m_StcRect.Height()/m_csEditFont.m_nFBBY;
	m_dMin = (dx < dy)?dx:dy;


	//
	InitBdfChar();
	
	//get byte of DotMatrix
	m_nSize = m_csEditFont.m_nFBBX * m_csEditFont.m_nFBBY;
 	m_pBakDot = new BYTE[m_nSize];
	memset(m_pBakDot,0,m_nSize);
//	memcpy(m_pBakDot, m_csEditFont.m_pGlyph->bitmap, m_nSize);
	m_pModifyDot = new BYTE[NUM_BAK * m_nSize];
	
//  just for pc-lint with no error 755
    UNUSED_ALWAYS(IDC_RDO_COLOR0);
    UNUSED_ALWAYS(IDC_RDO_COLOR1);
    UNUSED_ALWAYS(IDC_RDO_COLOR2);
    UNUSED_ALWAYS(IDC_RDO_COLOR3);
    UNUSED_ALWAYS(IDC_RDO_COLOR4);
    UNUSED_ALWAYS(IDC_RDO_COLOR5);
    UNUSED_ALWAYS(IDC_RDO_COLOR6);
    UNUSED_ALWAYS(IDC_RDO_COLOR7);
    UNUSED_ALWAYS(IDC_RDO_COLOR8);
    UNUSED_ALWAYS(IDC_RDO_COLOR9);
	UNUSED_ALWAYS(IDC_RDO_COLOR10);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditFontDlg::OnClickFont( UINT nID )
{
	for(UINT i = 0; i<4; i++)
	{
		if( (IDC_BOX_PEN+i) != nID)
		{
			CButton * pWnd = (CButton *)GetDlgItem(IDC_BOX_PEN + i);
			if(pWnd->GetCheck() == 1)
			{
				pWnd->SetCheck(0);
			}

		}
	}
	
	m_FocusRect.SetRect(0,0,0,0);
	m_csEditFont.Invalidate();
	if(nID == IDC_BOX_PEN)
	{
		m_bIsPen = TRUE;
		m_bIsErase = FALSE;
		m_bIsSelect = FALSE;
	}
	if(nID == IDC_BOX_ERASE)
	{
		m_bIsPen = FALSE;
		m_bIsErase = TRUE;
		m_bIsSelect = FALSE;
	}
	if(nID == IDC_BOX_SELECT)
	{
		m_bIsPen = FALSE;
		m_bIsErase = FALSE;
		m_bIsSelect = TRUE;
	}
	if(nID == IDC_BOX_UNDO)
	{
		HCURSOR hCurSor;
		hCurSor = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
		SetCursor(hCurSor);
		m_csPen.SetCheck(0);
		m_csErase.SetCheck(0);
		m_csSelect.SetCheck(0);
		m_bIsPen = FALSE;
		m_bIsErase = FALSE;
		m_bIsSelect = FALSE;
		Undo();
	}

	if(((CButton*)GetDlgItem(nID))->GetCheck() == 0)
	{
		m_bIsPen = FALSE;
		m_bIsErase = FALSE;
		m_bIsSelect = FALSE;
	}
}

void CEditFontDlg::OnOK() 
{
	// TODO: Add extra validation here
	AbstractGlyph(&m_bdfChar);

	if(m_bdfChar.dwBBw == 0 || m_bdfChar.dwBBh== 0)
	{
		AfxMessageBox(_T("Empty glyph is not permitted!"));
		return;
	}

	if(m_pBakDot != NULL)
	{
		delete [] m_pBakDot;
		m_pBakDot = NULL;
	}
	if(m_pModifyDot != NULL)
	{
		delete [] m_pModifyDot;
		m_pModifyDot = NULL;
	}

	GLYPH_BODY_PTR pGlyf = m_csEditFont.m_pGlyph;

	
 	pGlyf->x     = (short)(pGlyf->x + m_nDeltX);
 	pGlyf->y     = (short)(pGlyf->y - m_nDeltY);
	pGlyf->width = (BYTE)m_bdfChar.dwBBw;
	pGlyf->height = (BYTE)m_bdfChar.dwBBh;




	SAFE_FREE_ARRAY(pGlyf->bitmap);

	if(!m_csEditFont.m_bAlpha)
	{
		int nSize = ((pGlyf->width + 7) / 8) * pGlyf->height;
		pGlyf->bitmap = new BYTE[nSize];
		memcpy(pGlyf->bitmap,m_bdfChar.bBitmap,nSize);
	}
	else
	{
		int nSize = pGlyf->width * pGlyf->height;
		pGlyf->bitmap = new BYTE[nSize];
		memcpy(pGlyf->bitmap,m_bdfChar.bBitmap,nSize);
	}

	SAFE_FREE_ARRAY(m_bdfChar.bBitmap);
	SAFE_FREE_ARRAY(m_bdfChar.bDotMatrix);

	
	CDialog::OnOK();
}

void CEditFontDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	memcpy(m_csEditFont.m_pChar->bDotMatrix, m_pBakDot, m_nSize);
 	if(m_pBakDot != NULL)
	{
		delete [] m_pBakDot;
		m_pBakDot = NULL;
	}
	if(m_pModifyDot != NULL)
	{
		delete [] m_pModifyDot;
		m_pModifyDot = NULL;
	}

	CDialog::OnCancel();
}

void CEditFontDlg::SetBtnBmp(CButton *pBtn, UINT uID)
{
	CBitmap bitmap;
	bitmap.LoadBitmap( uID );
	pBtn->SetBitmap((HBITMAP)bitmap.Detach());
}

void CEditFontDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ClientToScreen(&point);
	CRect rect = m_StcRect;
	m_csEditFont.ClientToScreen(&rect);
	HCURSOR hCurSor;
	CDC *pDC = GetDC();
	//if in edit static
	if(rect.PtInRect(point))
	{
		//draw focus rect second time
		if(m_bIsSelectDown)
		{
			pDC->DrawFocusRect(&m_FocusRect);
		}
		
		if(m_bIsPen)
		{
			hCurSor = ::LoadCursor(m_hinst, MAKEINTRESOURCE(IDC_CURSOR_PEN));
			SetCursor(hCurSor);
			if(m_bIsModifyDown)
			{
				CPoint blackPoint = GetStcPoint(point);
				ModifyChar(blackPoint, g_gray[MAX_COLOR-m_nGrayIdx-1]);
			}
		}
		if(m_bIsErase)
		{
			hCurSor = ::LoadCursor(m_hinst, MAKEINTRESOURCE(IDC_CURSOR_ERASE));
			SetCursor(hCurSor);
			if(m_bIsModifyDown)
			{
				CPoint whitePoint = GetStcPoint(point);
				ModifyChar(whitePoint, 0);
			}
		}
		if(m_bIsSelect)
		{
			ScreenToClient(&point);
			if(m_FocusRect.PtInRect(point))
			{
				hCurSor = ::LoadCursor(NULL, IDC_SIZEALL);
				SetCursor(hCurSor);
			}
			else
			{
				hCurSor = ::LoadCursor(NULL, IDC_CROSS);
				SetCursor(hCurSor);
			}

			if(m_bIsSelectDown)
			{
				//move the selected rect
				if(m_bMove)
				{
					m_bIsSelectUp = FALSE;
					hCurSor = ::LoadCursor(NULL, IDC_SIZEALL);
					SetCursor(hCurSor);				
					int nDX = point.x - m_lastPoint.x;
					int nDY = point.y - m_lastPoint.y;					

					m_nTotalDX += nDX;
					m_nTotalDY += nDY;

					//if the distance of the cursor's moving >= dMin
					if(fabs(m_nTotalDX) >= m_dMin || fabs(m_nTotalDY) >= m_dMin)
					{
						CRect newRect(point.x - m_lx, point.y - m_ty, point.x + m_rx, point.y + m_by);
						ClientToScreen(&newRect);
						GetMoveRect(&newRect);
						AdjustRect(&newRect);
						ScreenToClient(&newRect);
						ReDrawFont(m_FocusRect, newRect);
						m_FocusRect = newRect;
			
						m_nTotalDX = 0;
						m_nTotalDY = 0;
					}
					m_lastPoint = point;
					pDC->Draw3dRect(m_FocusRect.left, m_FocusRect.top, m_FocusRect.Width(), m_FocusRect.Height(), RGB(255,0,0), RGB(255,0,0));
				}
				//select a rect for moving
				if(m_bDraw)
				{
					if(m_bIsSelectUp)
					{
						hCurSor = ::LoadCursor(NULL, IDC_CROSS);
						SetCursor(hCurSor);
						ClientToScreen(&point);
						CPoint endPoint = point;
						CPoint SPt, EPt;
						if(endPoint.x > m_StartPoint.x)
						{
							if(endPoint.y > m_StartPoint.y)			//from left-top to right-bottom
							{
								SPt = GetRectPoint(m_StartPoint, LT_POINT);
								EPt = GetRectPoint(endPoint, RB_POINT);
								ScreenToClient(&SPt);
								ScreenToClient(&EPt);
								m_FocusRect.SetRect(SPt.x, SPt.y, EPt.x, EPt.y);
							}
							else								//form left-bottom to right-top
							{
								SPt = GetRectPoint(m_StartPoint, LB_POINT);
								EPt = GetRectPoint(endPoint, RT_POINT);
								ScreenToClient(&SPt);
								ScreenToClient(&EPt);
								m_FocusRect.SetRect(SPt.x, EPt.y, EPt.x, SPt.y);
							}
						}
						if(endPoint.x < m_StartPoint.x)
						{
							if(endPoint.y < m_StartPoint.y)		//from right-bottom to left-top
							{
								SPt = GetRectPoint(m_StartPoint, RB_POINT);
								EPt = GetRectPoint(endPoint, LT_POINT);
								ScreenToClient(&SPt);
								ScreenToClient(&EPt);
								m_FocusRect.SetRect( EPt.x, EPt.y, SPt.x, SPt.y);
							}
							else								//from right-top to left-bottom
							{
								SPt = GetRectPoint(m_StartPoint, RT_POINT);
								EPt = GetRectPoint(endPoint, LB_POINT);
								ScreenToClient(&SPt);
								ScreenToClient(&EPt);
								m_FocusRect.SetRect(EPt.x, SPt.y, SPt.x, EPt.y);
							}
						}
						pDC->DrawFocusRect(&m_FocusRect);
					}
					else
					{
						hCurSor = ::LoadCursor(NULL, IDC_SIZEALL);
						SetCursor(hCurSor);
					}
				}
			}
		}
	}
	else
	{
		hCurSor = LoadCursor(NULL, IDC_ARROW);
		SetCursor(hCurSor);
	}
	if(pDC != NULL)
	{
		ReleaseDC(pDC);
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CEditFontDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetCapture();
	ClientToScreen(&point);
	CRect rect = m_StcRect;
	m_csEditFont.ClientToScreen(&rect);
	HCURSOR hCurSor;
	if(rect.PtInRect(point))
	{
		m_csEditFont.SetFocus();
		
		if(m_bIsPen)
		{
			m_bIsModifyDown = TRUE;
			hCurSor = ::LoadCursor(m_hinst, MAKEINTRESOURCE(IDC_CURSOR_PEN));
			SetCursor(hCurSor);
			CPoint blackPoint = GetStcPoint(point);
			SaveDotMatrix();
			ModifyChar(blackPoint, g_gray[MAX_COLOR-m_nGrayIdx-1]);
		}
		if(m_bIsErase)
		{
			m_bIsModifyDown = TRUE;
			hCurSor = ::LoadCursor(m_hinst, MAKEINTRESOURCE(IDC_CURSOR_ERASE));
			SetCursor(hCurSor);
			CPoint whitePoint = GetStcPoint(point);
			SaveDotMatrix();
			ModifyChar(whitePoint, 0);
		}
		if(m_bIsSelect)
		{
			m_bIsModifyDown = FALSE;
			m_bIsSelectDown = TRUE;
			m_StartPoint = point;
			ScreenToClient(&point);
			//if or not in the selected rect
			if(!m_FocusRect.PtInRect(point))
			{
				hCurSor = ::LoadCursor(NULL, IDC_CROSS);
				SetCursor(hCurSor);
				m_bDraw = TRUE;
				m_bMove = FALSE;
				m_csEditFont.Invalidate();
				m_FocusRect.SetRect(0,0,0,0);
			}
			else
			{
				SaveDotMatrix();
				hCurSor = ::LoadCursor(NULL, IDC_SIZEALL);
				SetCursor(hCurSor);
				m_bMove = TRUE;
				m_bDraw = FALSE;
				CRect tmpRect = m_FocusRect;
				m_ty = point.y - tmpRect.top;
				m_lx = point.x - tmpRect.left;
				m_rx = tmpRect.right - point.x;
				m_by = tmpRect.bottom - point.y;
				m_lastPoint = point;
			}
		}
	}
	else
	{
		hCurSor = LoadCursor(NULL, IDC_ARROW);
		SetCursor(hCurSor);
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CEditFontDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bIsModifyDown = FALSE;
	m_bDraw = FALSE;
	m_bMove = FALSE;
	ClientToScreen(&point);
	CRect rect = m_StcRect;
	m_csEditFont.ClientToScreen(&rect);
	HCURSOR hCurSor;
	if(rect.PtInRect(point))
	{
		
		if(m_bIsPen)
		{
			hCurSor = ::LoadCursor(m_hinst, MAKEINTRESOURCE(IDC_CURSOR_PEN));
			SetCursor(hCurSor);
		}
		if(m_bIsErase)
		{
			
			hCurSor = ::LoadCursor(m_hinst, MAKEINTRESOURCE(IDC_CURSOR_ERASE));
			SetCursor(hCurSor);
		}
		if(m_bIsSelect)
		{
			m_bIsSelectUp = TRUE;
			ScreenToClient(&point);
			if(m_FocusRect.PtInRect(point))
			{
				hCurSor = ::LoadCursor(NULL, IDC_SIZEALL);
				SetCursor(hCurSor);
			}
			else
			{
				hCurSor = ::LoadCursor(NULL, IDC_CROSS);
				SetCursor(hCurSor);
			}
			m_csEditFont.Set2Step(FALSE);
			m_bIsSelectDown = FALSE;
		}
	}
	else
	{
		hCurSor = LoadCursor(NULL, IDC_ARROW);
		SetCursor(hCurSor);
	}
	CDC* pDC = GetDC();
	pDC->Draw3dRect(m_FocusRect.left, m_FocusRect.top, m_FocusRect.Width(), m_FocusRect.Height(), RGB(255,0,0), RGB(255,0,0));
	if(pDC != NULL)
	{
		ReleaseDC(pDC);
	}
	m_bIsSelectDown = FALSE;
	ReleaseCapture();
	
	
	CDialog::OnLButtonUp(nFlags, point);
}

//get the point of the rect in the static
CPoint CEditFontDlg::GetStcPoint(CPoint point)
{
	CRect rcCenter(&m_StcRect);
	int wOff = (m_StcRect.Width() - m_dMin*m_csEditFont.m_nFBBX)%2;
	int hOff = (m_StcRect.Height() - m_dMin*m_csEditFont.m_nFBBY)%2;
	rcCenter.left += (m_StcRect.Width() - m_dMin*m_csEditFont.m_nFBBX)/2 ;
	rcCenter.right -= ((m_StcRect.Width() - m_dMin*m_csEditFont.m_nFBBX)/2 +wOff);
	rcCenter.bottom -= (m_StcRect.Height() - m_dMin*m_csEditFont.m_nFBBY)/2;
	rcCenter.top += ((m_StcRect.Height() - m_dMin*m_csEditFont.m_nFBBY)/2 + hOff);

	m_csEditFont.ClientToScreen(&rcCenter);
	
	int nX = (point.x - rcCenter.left) / m_dMin;
	int nY = (point.y - rcCenter.top) / m_dMin;

	return CPoint(nX, nY);

}

//get the four corner point of the rect
CPoint CEditFontDlg::GetRectPoint(CPoint point, DWORD dwFlag)
{
	CRect rcCenter(&m_StcRect);
	int wOff = (m_StcRect.Width() - m_dMin*m_csEditFont.m_nFBBX)%2;
	int hOff = (m_StcRect.Height() - m_dMin*m_csEditFont.m_nFBBY)%2;
	rcCenter.left += (m_StcRect.Width() - m_dMin*m_csEditFont.m_nFBBX)/2 ;
	rcCenter.right -= ((m_StcRect.Width() - m_dMin*m_csEditFont.m_nFBBX)/2 +wOff);
	rcCenter.bottom -= (m_StcRect.Height() - m_dMin*m_csEditFont.m_nFBBY)/2;
	rcCenter.top += ((m_StcRect.Height() - m_dMin*m_csEditFont.m_nFBBY)/2 + hOff);

	m_csEditFont.ClientToScreen(&rcCenter);
	
	int nX = (point.x - rcCenter.left) / m_dMin;
	int nY = (point.y - rcCenter.top) / m_dMin;

	int nRectX = 0;
	int nRectY = 0;

	switch(dwFlag)
	{
	case LT_POINT:
		nRectX = rcCenter.left + nX*m_dMin;
		nRectY = rcCenter.top + nY*m_dMin;
		break;
	case RT_POINT:
		nRectX = rcCenter.left + (nX + 1)*m_dMin;
		nRectY = rcCenter.top + nY*m_dMin; 
		break;
	case LB_POINT:
		nRectX = rcCenter.left + nX*m_dMin;
		nRectY = rcCenter.top + (nY + 1)*m_dMin;
		break;
	case RB_POINT:
		nRectX = rcCenter.left + (nX + 1)*m_dMin;
		nRectY = rcCenter.top + (nY +1 )*m_dMin;
		break;
	default:
		break;
	}
	if(nRectX < rcCenter.left)
	{
		nRectX = rcCenter.left;
	}
	if(nRectX > rcCenter.right)
	{
		nRectX = rcCenter.right;
	}
	if(nRectY < rcCenter.top)
	{
		nRectY = rcCenter.top;
	}
	if(nRectY > rcCenter.bottom)
	{
		nRectY = rcCenter.bottom;
	}
	return CPoint(nRectX, nRectY);
}

void CEditFontDlg::ModifyChar(CPoint point, int nVal)
{
	int nOldVal = m_csEditFont.GetPtValue(m_csEditFont.m_pChar->bDotMatrix, point.x, point.y);
	if(nVal == nOldVal)
	{
		return;
	}
	m_csEditFont.SaveCharByPose(m_csEditFont.m_pChar, point.x, point.y, nVal);
	m_csEditFont.Invalidate();
	m_csBrowse.Invalidate();
}

void CEditFontDlg::SaveDotMatrix()
{
	if(m_nIndex < NUM_BAK)
	{
		memcpy(m_pModifyDot + m_nSize * m_nIndex, m_csEditFont.m_pChar->bDotMatrix, m_nSize);
		m_nIndex ++;		
	}
	else
	{
		for(int i = 0; i < NUM_BAK - 1; i++)
		{
			memcpy(m_pModifyDot + i * m_nSize, m_pModifyDot + (i + 1) * m_nSize, m_nSize);
		}
		memcpy(m_pModifyDot + (NUM_BAK - 1) * m_nSize, m_csEditFont.m_pChar->bDotMatrix, m_nSize);
	}
}

void CEditFontDlg::Undo()
{
	if(m_nIndex > 0)
	{
		memcpy(m_csEditFont.m_pChar->bDotMatrix, m_pModifyDot + (m_nIndex - 1) * m_nSize, m_nSize);
		m_csEditFont.Invalidate();
		m_csBrowse.Invalidate();
		m_nIndex --;
	}
}

void CEditFontDlg::GetMoveRect(CRect* rect)
{	
	CRect rcCenter(&m_StcRect);
	int wOff = (m_StcRect.Width() - m_dMin*m_csEditFont.m_nFBBX)%2;
	int hOff = (m_StcRect.Height() - m_dMin*m_csEditFont.m_nFBBY)%2;
	rcCenter.left += (m_StcRect.Width() - m_dMin*m_csEditFont.m_nFBBX)/2 ;
	rcCenter.right -= ((m_StcRect.Width() - m_dMin*m_csEditFont.m_nFBBX)/2 +wOff);
	rcCenter.bottom -= (m_StcRect.Height() - m_dMin*m_csEditFont.m_nFBBY)/2;
	rcCenter.top += ((m_StcRect.Height() - m_dMin*m_csEditFont.m_nFBBY)/2 + hOff);

	m_csEditFont.ClientToScreen(&rcCenter);

	int nWdith = rect->Width();
	int nHeight = rect->Height();
	
	rect->left = rcCenter.left + (rect->left - rcCenter.left) / m_dMin * m_dMin;
	rect->top = rcCenter.top + (rect->top - rcCenter.top) / m_dMin * m_dMin;
	rect->right = rect->left + nWdith;
	rect->bottom = rect->top + nHeight;
}

//adjust the rect in the static when moving
void CEditFontDlg::AdjustRect(CRect* rect)
{
	CRect rcCenter(&m_StcRect);
	int wOff = (m_StcRect.Width() - m_dMin*m_csEditFont.m_nFBBX)%2;
	int hOff = (m_StcRect.Height() - m_dMin*m_csEditFont.m_nFBBY)%2;
	rcCenter.left += (m_StcRect.Width() - m_dMin*m_csEditFont.m_nFBBX)/2 ;
	rcCenter.right -= ((m_StcRect.Width() - m_dMin*m_csEditFont.m_nFBBX)/2 +wOff);
	rcCenter.bottom -= (m_StcRect.Height() - m_dMin*m_csEditFont.m_nFBBY)/2;
	rcCenter.top += ((m_StcRect.Height() - m_dMin*m_csEditFont.m_nFBBY)/2 + hOff);

	m_csEditFont.ClientToScreen(&rcCenter);
	
	if(rect->left < rcCenter.left)
	{
		int nDiff = rcCenter.left - rect->left;
		rect->left = rcCenter.left;
		rect->right += nDiff;
	}
	if(rect->top < rcCenter.top)
	{
		int nDiff = rcCenter.top - rect->top;
		rect->top = rcCenter.top;
		rect->bottom += nDiff;
	}
	if(rect->right > rcCenter.right)
	{
		int nDiff = rect->right - rcCenter.right;
		rect->right = rcCenter.right;
		rect->left -= nDiff;
	}
	if(rect->bottom > rcCenter.bottom)
	{
		int nDiff = rect->bottom - rcCenter.bottom;
		rect->bottom = rcCenter.bottom;
		rect->top -= nDiff;
	}
}

//get dMin for browse static
int CEditFontDlg::GetBrsStcDmin()
{
	if(m_csBrowse.m_nFBBX == 0 || m_csBrowse.m_nFBBY == 0)
		return 0;
	CRect rect;
	m_csBrowse.GetClientRect(&rect);

	int dx = rect.Width()/*/m_csBrowse.m_nFBBX*/;
	int dy = rect.Height()/*/m_csBrowse.m_nFBBY*/;

	int dMin = (dx < dy)?dx:dy;
	dMin /= 2; 

	return dMin - 6;

}

void CEditFontDlg::ReDrawFont(CRect oldRect, CRect newRect)
{
	CPoint OldLT(oldRect.left, oldRect.top);
	CPoint OldRB(oldRect.right, oldRect.bottom);
	CPoint NewLT(newRect.left, newRect.top);
	ClientToScreen(&OldLT);
	ClientToScreen(&OldRB);
	ClientToScreen(&NewLT);
	OldLT = GetStcPoint(OldLT);
	OldRB = GetStcPoint(OldRB);
	NewLT = GetStcPoint(NewLT);
	
	m_csEditFont.MoveSelRegion(m_csEditFont.m_pChar, OldLT.x, OldLT.y, (int)fabs(OldRB.x - OldLT.x), (int)fabs(OldRB.y - OldLT.y), NewLT.x, NewLT.y);
	m_csEditFont.Set2Step(TRUE);
	m_csEditFont.Invalidate();
	m_csBrowse.Invalidate();

}

void CEditFontDlg::DeleteFont()
{
	m_csEditFont.Set2Step(FALSE);
	CRect tmpRect = m_FocusRect;
	ClientToScreen(&tmpRect);
	CPoint ltPt(tmpRect.left, tmpRect.top);
	CPoint rbPt(tmpRect.right, tmpRect.bottom);
	ltPt = GetStcPoint(ltPt);
	rbPt = GetStcPoint(rbPt);
	for(int i = ltPt.x; i < rbPt.x; i++)
	{
		for(int j = ltPt.y; j < rbPt.y; j++)
		{
			ModifyChar(CPoint(i, j), 0);
		}
	}
}

void CEditFontDlg::MoveRect(DWORD dwFlag)
{
	CRect oldRect = m_FocusRect;
	CRect newRect = m_FocusRect;

	switch(dwFlag)
	{
	case LEFT_KEY:
		newRect.left -= m_dMin;
		newRect.right -= m_dMin;
		break;
	case RIGHT_KEY:
		newRect.left += m_dMin;
		newRect.right += m_dMin;
		break;
	case UP_KEY:
		newRect.top -= m_dMin;
		newRect.bottom -= m_dMin;
		break;
	case DOWN_KEY:
		newRect.top += m_dMin;
		newRect.bottom += m_dMin;
		break;
	default:
		break;
	}
	ClientToScreen(&newRect);
	AdjustRect(&newRect);
	ScreenToClient(&newRect);
	m_FocusRect = newRect;
	ReDrawFont(oldRect, newRect);
	
	CDC *pDC = GetDC();
	pDC->Draw3dRect(m_FocusRect.left, m_FocusRect.top, m_FocusRect.Width(), m_FocusRect.Height(), RGB(255,0,0), RGB(255,0,0));
	if(pDC != NULL)
	{
		ReleaseDC(pDC);
	}
	
}

void CEditFontDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	dc.DrawFocusRect(&m_FocusRect);
	m_FocusRect.SetRect(0,0,0,0);
	
	
	// Do not call CDialog::OnPaint() for painting messages
}


void CEditFontDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nSBCode)
	{
	case SB_LEFT:
		break;
	case SB_ENDSCROLL:
		break;
	case SB_LINELEFT:
		break;
	case SB_LINERIGHT:
		break;
	case SB_PAGELEFT:
		if(m_csBrowse.m_nDiff <= 0)
		{
	// 		m_csBrowse.m_nDiff --;
			m_csBrowse.m_nDiff -= m_nRange/6;
		}
		m_csBrowse.Invalidate();
		break;
	case SB_PAGERIGHT:
		if(m_csBrowse.m_nDiff >= (-m_nRange))
		{
			//m_csBrowse.m_nDiff ++;
			m_csBrowse.m_nDiff += m_nRange/6;
		}
		m_csBrowse.Invalidate();
		break;
	case SB_RIGHT:
		break;
	case SB_THUMBPOSITION:
		break;
	case SB_THUMBTRACK:
		m_csBrowse.m_nDiff = (-m_nRange) + nPos;
		m_csBrowse.Invalidate();
		break;
	}
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CEditFontDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(IsCtrlPressed())
		{
			switch(pMsg->wParam)
			{
			case 'C':  // ctrl + c
				OnCopy();
				break;
			case 'V':  // ctrl + v
				OnPaste();
				break;
			default:
				break;
			}
		}
	}

	CWnd *pWnd = (CWnd *)GetDlgItem(IDC_STATIC_EDITFONT);
	if(pMsg->hwnd != pWnd->m_hWnd)
	{
		return CDialog::PreTranslateMessage(pMsg);
	}
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_DELETE:
			SaveDotMatrix();
			DeleteFont();
			break;
		case VK_LEFT:
			MoveRect(LEFT_KEY);
			break;
		case VK_RIGHT:
			MoveRect(RIGHT_KEY);
			break;
		case VK_UP:
			MoveRect(UP_KEY);
			break;
		case VK_DOWN:
			MoveRect(DOWN_KEY);
			break;
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CEditFontDlg::FillDotMatrix(PBDF_CHAR_T pChar)
{
	GLYPH_BODY_PTR pGlyf = m_csEditFont.m_pGlyph;
	int nBBX = m_csEditFont.m_nFBBX;
	int nBBY = m_csEditFont.m_nFBBY;
	int nXoff = pGlyf->x;
	int nYoff = pGlyf->y;


	if(pChar->bDotMatrix != NULL)
	{
		delete [] pChar->bDotMatrix;
		pChar->bDotMatrix = NULL;
	}

	pChar->bDotMatrix = new BYTE[nBBX * nBBY];  // there exist redundancy 

	memset(pChar->bDotMatrix, 0, nBBX * nBBY);

	if(pChar->bBitmap == NULL)
	{
		return TRUE;
	}

	int nBBw    = pChar->dwBBw;   //glyph width (bits)
	int nBBh    = pChar->dwBBh;
//	int nBBxoff0x = pChar->nBBxoff0x;
//	int nBByoff0y = pChar->nBByoff0y;

	int nXXLeft = /*nBBxoff0x - */nXoff;
//	int nXXRight = nBBX - nBBw -nXXLeft;
//	int nYYBottom = nYoff + nBBY/*nBByoff0y - nYoff*/;
	int nYYTop = nYoff/*nBBY - nBBh - nYYBottom*/;

	// adjust top space line number
	//if((nBBY - nBBh) <= 4)
//	if( nBBY != 0 && pChar->dwUCCode >= 0x80 && (((double)nBBh/(double)nBBY) > 0.5))
//	{
		nYYTop = (int)floor((nBBY - nBBh)/2.0);
//	}

	if(nYYTop < 0)
	{
		nYYTop = 0;
	}
	// adjust left space bit number
	//if((nBBX - nBBw) <= 4)
//	if( nBBX != 0 && (((double)nBBw / (double)nBBX) > 0.5))
//	{
		nXXLeft = (int)floor((nBBX - nBBw)/2.0);
//	}
	if(nXXLeft < 0)
	{
		nXXLeft = 0;
	}

	// adjust the glyph vertical
	if((nBBh + nYYTop)>nBBY)
	{
//		TRACE(_T("Height is truncated [UC:0x%04X]\n"),pChar->dwUCCode);
		if( (nBBY - nBBh) > 0)
		{
			nYYTop = nBBY - nBBh ;
		}
		else
		{
			nYYTop = 0;
			nBBh = nBBY;
		}
	}
	// adjust the glyph horizon
	if( (nBBw+nXXLeft) > nBBX)
	{
//		TRACE(_T("Width is truncated [UC:0x%04X]\n"),pChar->dwUCCode);
		if((nBBX - nBBw) > 0)
		{
			nXXLeft = nBBX - nBBw ;
		}
		else
		{
			nXXLeft = 0;
			nBBw = nBBX;
		}
	}

	// the height of glyph is large than the height of the char
	if(nBBh > nBBY)
	{
		nBBh = nBBY;
	}

	int nLeftSpace = pChar->dwBBw;
	int nTopSpace  = 0;
	int nTopCount  = 0;


	if(!m_csEditFont.m_bAlpha)
	{
		int nBBWbytes = (pChar->dwBBw + 7 )/ 8;

		for(int i = 0; i< nBBh; i++)
		{
			//copy bits from bitmap to dot-matrix
			int nLeftPos = CopyBitsByPositon(pChar->bDotMatrix + (i+nYYTop)*nBBX,nBBw,pChar->bBitmap + nBBWbytes * i,pChar->dwBBw,nXXLeft);
			
			//由于有些字型四周有空白，需要去掉，这会影响字型的x,y
			//所以需要重新计算x，y
			if(nLeftPos != -1 && nLeftPos < nLeftSpace)
			{
				nLeftSpace = nLeftPos;
			}

			nTopCount++;

			if(nLeftPos == -1 && (nTopCount - nTopSpace) == 1)
			{
				nTopSpace ++;
			}

		}
	}
	else
	{
		int nBBWbytes = pChar->dwBBw;

		for(int i = 0; i< nBBh; i++)
		{
			//copy bits from bitmap to dot-matrix
			memcpy(pChar->bDotMatrix + (i+nYYTop)*nBBX+nXXLeft,pChar->bBitmap + nBBWbytes * i,nBBWbytes);

			int nLeftPos = -1;

			for(int j=0; j<nBBWbytes; j++)
			{
				if(*(pChar->bBitmap + nBBWbytes * i + j))
				{
					nLeftPos = j;
					break;
				}
			}

			if(nLeftPos != -1 && nLeftPos < nLeftSpace)
			{
				nLeftSpace = nLeftPos;
			}

			nTopCount++;

			if(nLeftPos == -1 && (nTopCount - nTopSpace) == 1)
			{
				nTopSpace ++;
			}
		}
	}

	m_nDeltX = nLeftSpace;
	m_nDeltY = nTopSpace;

	// the height of glyph is large than the height of the char
	// or the width of glyph is large than the width of the char
//	if(pChar->dwBBw > (UINT)nBBX || pChar->dwBBh > (UINT)nBBY)
//	{
//		AbstractGlyph(pChar);
//	}

	return TRUE;
}

BOOL CEditFontDlg::CopyBitsByPositon(LPBYTE lpbDst,int nDstSize,LPBYTE lpbPlus,int nPlusSize, int nPos)
{
//	memset(lpbDst,0xFF,nDstSize);
	memset(lpbDst,0,nDstSize);

	LPBYTE pTmp = lpbDst+nPos;

	int nBBWbytes = (nPlusSize + 7 )/ 8;

	int nMinLeft = -1;
	int k=0;
	
	for(int i=0; i< nBBWbytes; i++)
	{
		BYTE bCurByte = lpbPlus[i];
		for(int j = 0; j< 8 && (i*8+j)<nPlusSize; j++)
		{
			BYTE bValue = (BYTE)(bCurByte<<j);
			bValue =(BYTE)( bValue>>7);
			pTmp[i*8+j]= (BYTE)(bValue==1? 0xFF:0);
			if(bValue && nMinLeft == -1)
			{
				nMinLeft = k;
			}
			k++;
		}
	}

	return nMinLeft;
}

BOOL CEditFontDlg::AbstractGlyph(PBDF_CHAR_T pChar)
{
	if(pChar->bDotMatrix == NULL)
	{
		return TRUE;
	}

	int nBBX = m_csEditFont.m_nFBBX;
	int nBBY = m_csEditFont.m_nFBBY;
	int nXoff = pChar->nBBxoff0x;
	int nYoff = pChar->nBByoff0y;


	int nHT = 0;        // the top  black point 
	int nHB = nBBY-1;   // the bottom black point 

	int i=0; 
	int j=0;
	LPBYTE pCurLine = NULL;
	BOOL bFind = FALSE;
	for(i=0; i< nBBY; i++ )
	{
		pCurLine = pChar->bDotMatrix + i*nBBX;
		for(j = 0; j<nBBX;j++)
		{
			if(*(pCurLine+j) != 0/*0xFF*/)
			{
				nHT = i;
				bFind = TRUE;
				break;
			}
		}
		if(bFind)
			break;
	}
	bFind = FALSE;
	for(i=nBBY-1; i>=0; i-- )
	{
		pCurLine = pChar->bDotMatrix + i*nBBX;
		for(j = 0; j<nBBX;j++)
		{
			if(*(pCurLine+j) != 0 /*0xFF*/)
			{
				nHB = i;
				bFind = TRUE;
				break;
			}
		}
		if(bFind)
			break;
	}

	pChar->dwBBh = nHB - nHT + 1;  // real glyph height

	if((int)pChar->dwBBh < 0)
	{
		pChar->dwBBw = 0;
		pChar->dwBBh = 0;
		pChar->nBBxoff0x = nXoff;
		pChar->nBByoff0y = nYoff;
		return TRUE;
	}
	
	int nWL = 0;			// the left black point 
	int nWR = nBBX-1;		// the right black point
	bFind = FALSE;
	int nLMin=nWR;
	int nRMax=0;
	for(i=nHT; i<= nHB; i++ )
	{
		for(j = 0; j<nBBX;j++)
		{			
			if(pChar->bDotMatrix[nBBX*i+j] != 0/*0xFF*/)
			{
				if(j<nLMin)
				{
					nLMin = j;
				}
				if(j>nRMax)
				{
					nRMax = j;
				}
			}
		}
	}
	nWL = nLMin;
	nWR = nRMax;
	
	pChar->dwBBw = nWR-nWL +1;  // real glyph width

	if((int)pChar->dwBBw < 0)
	{
		nWL = 0;
		nWR = 0;
		pChar->dwBBw = 0;
		pChar->dwBBh = 0;
		pChar->nBBxoff0x = nXoff;
		pChar->nBByoff0y = nYoff;
		return TRUE;
	}
	
	pChar->nBBxoff0x = nWL;
	pChar->nBByoff0y = nBBY-nHT-1;
//	pChar->nBByoff0y = nBBY-nHB-1;


//	pChar->nBBxoff0x += nXoff;
//	pChar->nBByoff0y += nYoff;

	int nFullW = (pChar->dwBBw + 7) / 8;
	if(m_csEditFont.m_bAlpha)
	{
		nFullW = pChar->dwBBw;
	}
	
	if(pChar->bBitmap != NULL)
	{
		delete [] pChar->bBitmap;
		pChar->bBitmap = NULL;
	}

	pChar->bBitmap = new BYTE[nFullW * pChar->dwBBh];
	memset(pChar->bBitmap, 0, nFullW * pChar->dwBBh);
	BYTE nPtValue = 0;
	for(i = nHT; i<= nHB; i++)
	{
		pCurLine = pChar->bBitmap + (i-nHT);
		for(j= nWL; j<= nWR; j++)
		{
			nPtValue = pChar->bDotMatrix[nBBX*i+j];
			if(!m_csEditFont.m_bAlpha)
			{				
				if(nPtValue != 0) // black *****
				{
					SetBitByPose(pChar->bBitmap,nFullW,j-nWL,i-nHT,1);
				}
			}
			else
			{
				pChar->bBitmap[nFullW*(i-nHT)+j-nWL] = (BYTE)nPtValue;
			}
		}
	}

	return TRUE;
}

int  CEditFontDlg::GetPtValue(LPBYTE lpbDotMatrix,int nFullW,int nX,int nY)
{	
	BYTE bCurByte = *(lpbDotMatrix + ( nY * nFullW + (int)(nX / 8) ));
	int nDt = nX % 8;
	BYTE bBase = 1;
	bBase = (BYTE)(bBase << (7-nDt));
	bCurByte = (BYTE)(bCurByte & bBase);
	bCurByte = (BYTE)(bCurByte>>(7-nDt));

	return (int)bCurByte;
}

BOOL CEditFontDlg::SetBitByPose(LPBYTE lpbBitmap,int nFullW,int nX,int nY, int nPtValue)
{
	
	LPBYTE pCurByte = lpbBitmap + ( nY * nFullW + (int)(nX / 8) );

	int nDt = nX % 8;
	BYTE bRst = *pCurByte;
	BYTE bBase = 0;

	if(nPtValue == 0 )
	{
		bBase = 0xFF;
		BYTE bTemp = 1;
		bTemp = (BYTE)(bTemp << (7-nDt));

		bBase = (BYTE)(bBase - bTemp);
        bRst = (BYTE)(bRst & bBase);
	}
	else
	{
		bBase = 1;
		bBase = (BYTE)(bBase << (7-nDt));
		bRst = (BYTE)((bRst ^ bBase) | bBase);
	}
	
	*pCurByte = bRst;

	return TRUE;
}

void CEditFontDlg::InitBdfChar()
{
	m_nDeltX = 0;
	m_nDeltY = 0;
	GLYPH_BODY_PTR pGlyf = m_csEditFont.m_pGlyph;
	SAFE_FREE_ARRAY(m_bdfChar.bDotMatrix);
	SAFE_FREE_ARRAY(m_bdfChar.bBitmap);

	m_bdfChar.dwBBh = pGlyf->height;
	m_bdfChar.dwBBw = pGlyf->width;

	m_bdfChar.nBBxoff0x = pGlyf->x;
	m_bdfChar.nBByoff0y = pGlyf->y;

	if(!m_csEditFont.m_bAlpha)
	{
		int nSize = ((m_bdfChar.dwBBw + 7)/8)*m_bdfChar.dwBBh;

		m_bdfChar.bBitmap = new BYTE[nSize];
		memcpy(m_bdfChar.bBitmap,pGlyf->bitmap,nSize);
	}
	else
	{
		int nSize = m_bdfChar.dwBBw *m_bdfChar.dwBBh;

		m_bdfChar.bBitmap = new BYTE[nSize];
		memcpy(m_bdfChar.bBitmap,pGlyf->bitmap,nSize);
	}
	FillDotMatrix(&m_bdfChar);

	m_csEditFont.m_pChar = &m_bdfChar;
	m_csBrowse.m_pChar = &m_bdfChar;
}

void CEditFontDlg::InitColorCtrs()
{
	m_nGrayIdx=0;
	for(int i = 0; i< MAX_COLOR; i++)
	{
		m_btnColor[i].SubclassDlgItem(IDC_RDO_COLOR0+i,this);
		COLORREF color = RGB(g_gray[i],g_gray[i],g_gray[i]);
		m_btnColor[i].SetColors(color,color,color);
		if(!m_csEditFont.m_bAlpha)
		{
			m_btnColor[i].ShowWindow(SW_HIDE);
		}
		if(i==0)
		{
			m_btnColor[i].SetClicked(TRUE);
		}
		else
		{
			m_btnColor[i].SetClicked(FALSE);
		}
	}
	
}

void CEditFontDlg::OnRdoColor(UINT nID)
{
	m_nGrayIdx = nID-IDC_RDO_COLOR0;
	for(int i= 0; i< MAX_COLOR; i++)
	{
		if(m_nGrayIdx != i)
		{
			m_btnColor[i].SetClicked(FALSE);
		}
		else
		{
			m_btnColor[i].SetClicked(TRUE);
		}
	}
}


void CEditFontDlg::OnCopy() 
{
	// TODO: Add your command handler code here
	if ( !OpenClipboard() )
	{
		AfxMessageBox( _T("Cannot open the Clipboard") );
		return;
	}
	// Remove the current Clipboard contents
	if( !EmptyClipboard() )
	{
		AfxMessageBox( _T("Cannot empty the Clipboard") );
		return;
	}

	int nWidth = m_csEditFont.m_nFBBX;
	int nHeight = m_csEditFont.m_nFBBY;
	BITMAPINFO bmi = {0};
	BITMAPINFOHEADER &bmih = bmi.bmiHeader;
	bmih.biHeight = -nHeight;
	bmih.biWidth  = nWidth;
	bmih.biBitCount = 24;
	bmih.biPlanes = 1;
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biCompression = BI_RGB;
	int biWidthBytes = GET_WIDTH_BYTE(bmih.biWidth,bmih.biBitCount);
	bmih.biSizeImage = -biWidthBytes*bmih.biHeight;	

	LPBYTE pBuf = NULL;

	HBITMAP hBitmap = ::CreateDIBSection(NULL,&bmi,DIB_RGB_COLORS,(LPVOID *)&pBuf,0,0);	

	LPBYTE pData = pBuf;
	for(int i= 0; i<nHeight;i++)
	{
		for(int j=0; j<nWidth;j++)
		{
			*(pBuf+3*j+0) = (BYTE)(255 - m_bdfChar.bDotMatrix[nWidth*i + j]);
			*(pBuf+3*j+1) = (BYTE)(255 - m_bdfChar.bDotMatrix[nWidth*i + j]);
			*(pBuf+3*j+2) = (BYTE)(255 - m_bdfChar.bDotMatrix[nWidth*i + j]);	
		}
		
		pBuf += biWidthBytes;
	}


	HGLOBAL hMem = ::GlobalAlloc(GMEM_FIXED, bmih.biSizeImage+sizeof(bmi));
	LPBYTE pMem = (LPBYTE)::GlobalLock(hMem);

	memcpy( pMem, &bmi, sizeof(bmi));

	memcpy( pMem+sizeof(bmi), pData, bmih.biSizeImage);
	
	::GlobalUnlock(hMem);	


	
	// For the appropriate data formats...
	if ( ::SetClipboardData( CF_DIB, hMem ) == NULL )
	{
		AfxMessageBox( _T("Unable to set Clipboard data") );
		CloseClipboard();
		return;
	}
	// ...
	CloseClipboard();	
	
	::DeleteObject(hBitmap);


}

void CEditFontDlg::OnPaste() 
{
	// TODO: Add your command handler code here
	if ( !OpenClipboard() )
	{
		AfxMessageBox( _T("Cannot open the Clipboard") );
		return;
	}

	HGLOBAL hGlobal = GetClipboardData(CF_DIB);

	LPBYTE pMem = (LPBYTE)::GlobalLock(hGlobal);

	BITMAPINFO bmi = {0};
	
	memcpy( &bmi, pMem, sizeof(bmi));

	if(bmi.bmiHeader.biBitCount != 24)
	{
		::GlobalUnlock(hGlobal);
		return;
	}

	BOOL bVReverse = FALSE;

	int nHeight = bmi.bmiHeader.biHeight;
	if(nHeight < 0)
	{
		nHeight = 0 - nHeight;
	}
	else
	{
		bVReverse = TRUE;
	}

	int nMinWidth = m_csEditFont.m_nFBBX > bmi.bmiHeader.biWidth ? bmi.bmiHeader.biWidth : m_csEditFont.m_nFBBX;
	int nMinHeight = m_csEditFont.m_nFBBY > nHeight ? nHeight : m_csEditFont.m_nFBBY;

	int nWidthByte = bmi.bmiHeader.biSizeImage / nHeight;


	LPBYTE pData = pMem + sizeof(bmi);

	for(int i= 0; i<nMinHeight; i++)
	{
		for(int j=0; j< nMinWidth; j++)
		{
		    if(!bVReverse)
			{
				m_bdfChar.bDotMatrix[i*m_csEditFont.m_nFBBX + j] = (BYTE)(255 - *(pData+i*nWidthByte + 3*j));
			}
			else
			{
				m_bdfChar.bDotMatrix[i*m_csEditFont.m_nFBBX + j] = (BYTE)(255 - *(pData+(nHeight-i-1)*nWidthByte + 3*j));
			}
			
		}
	}

	::GlobalUnlock(hGlobal);

	CloseClipboard();

	m_csEditFont.Invalidate();



}

void CEditFontDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	UNUSED_ALWAYS(pWnd);

	CPoint pt(point);

	ClientToScreen(&point);
	CRect rect = m_StcRect;
	ClientToScreen(&rect);
	m_csEditFont.ClientToScreen(&rect);
	//if in edit static
	if(!rect.PtInRect(point))
	{
		return;
	}


	CMenu menu;
    menu.LoadMenu(IDR_MENU);
	
	CMenu *pTestMenu = menu.GetSubMenu(0);
	if(pTestMenu != NULL)
	{	
		if(!::IsClipboardFormatAvailable(CF_DIB))
		{
			pTestMenu->EnableMenuItem(1,MF_BYPOSITION | MF_GRAYED);
		}
	}

    menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this); 
}
