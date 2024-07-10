// BitmapCtrl.h: interface for the CBitmapCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BITMAPCTRL_H__AB6337AF_3615_4D33_88A2_383B30CDD2EF__INCLUDED_)
#define AFX_BITMAPCTRL_H__AB6337AF_3615_4D33_88A2_383B30CDD2EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BITMAP_COUNT 7

class CBitmapCtrl : public CWnd
{
public:
    
    CRect m_rcClient;
    int m_iSrollbarWidth;
    int m_iXOffset;
    int m_iYOffset;
    CSize m_sizeImage; 
    HBITMAP m_hBitmap;
    HBITMAP m_hBitmaps[BITMAP_COUNT];
    
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CBitmapCtrl)
public:
    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd);
    //}}AFX_VIRTUAL
    
    // Implementation
public:
    virtual ~CBitmapCtrl();
    void SetBitmap(HBITMAP hBmp);
    void DrawBitmap();
    void SetBitmap( HBITMAP* phBmp );
    
    // Generated message map functions
protected:
    //{{AFX_MSG(CBitmapCtrl)
    afx_msg void OnPaint();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);	
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
        
protected:
    CDC memDC;
    CBitmap* pBitmap;
    CBitmap* pOldBitmap;
protected:
    //paint specific value , unit of every rect etc.
    virtual void PaintSpecialValue(CDC* /*pDC*/) {     };
};

#endif // !defined(AFX_BITMAPCTRL_H__AB6337AF_3615_4D33_88A2_383B30CDD2EF__INCLUDED_)
