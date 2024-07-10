// ColorListBox.cpp : implementation file
//

#include "stdafx.h"
#include "ColorListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorListBox

CColorListBox::CColorListBox()
{
}

CColorListBox::~CColorListBox()
{
}


BEGIN_MESSAGE_MAP(CColorListBox, CListBox)
	//{{AFX_MSG_MAP(CColorListBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorListBox message handlers
void CColorListBox::AddStringEx(const ITEM_FACE& newFace )
{    
    m_vOutline.push_back( newFace );
    AddString( newFace.m_strShowString );
}

void CColorListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);
    LPCTSTR lpszText = (LPCTSTR) lpDrawItemStruct->itemData;
    
    if (lpszText != NULL)
    {
        CDC dc;
    
        dc.Attach(lpDrawItemStruct->hDC);
    
        // Save these value to restore them when done drawing.
        COLORREF crOldTextColor = dc.GetTextColor();
        COLORREF crOldBkColor = dc.GetBkColor();

    
        COLORREF colorText = m_vOutline[lpDrawItemStruct->itemID].m_color;
        dc.SetTextColor( colorText );    
        dc.FillSolidRect(&lpDrawItemStruct->rcItem, crOldBkColor);
    
    
        // draw the focus item,
        if ((lpDrawItemStruct->itemAction | ODA_FOCUS) &&
            (lpDrawItemStruct->itemState & ODS_FOCUS))
        {
            CBrush br(RGB(10, 36, 106));
            dc.FillRect(&lpDrawItemStruct->rcItem, &br);
        
            dc.SetBkColor( RGB(10, 36, 106) );
            colorText =~colorText;
            colorText &= 0x00111111;
            dc.SetTextColor( RGB(255,255,255 ) );
        }
    
        // Draw the text.
        dc.DrawText(
            lpszText,
            _tcslen(lpszText),
            &lpDrawItemStruct->rcItem,
            DT_LEFT|DT_SINGLELINE|DT_VCENTER);
    
        // Reset the background color and the text color back to their
        // original values.
        dc.SetTextColor(crOldTextColor);
        dc.SetBkColor(crOldBkColor);
    
        dc.Detach();
    }
}
void CColorListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	UNREFERENCED_PARAMETER(lpMeasureItemStruct);
    ASSERT(lpMeasureItemStruct->CtlType == ODT_LISTBOX); 
}
int CColorListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
    ASSERT(lpCompareItemStruct->CtlType == ODT_LISTBOX);
    LPCTSTR lpszText1 = (LPCTSTR) lpCompareItemStruct->itemData1;
    ASSERT(lpszText1 != NULL);
    LPCTSTR lpszText2 = (LPCTSTR) lpCompareItemStruct->itemData2;
    ASSERT(lpszText2 != NULL);
    
    return _tcscmp(lpszText2, lpszText1);
    
}