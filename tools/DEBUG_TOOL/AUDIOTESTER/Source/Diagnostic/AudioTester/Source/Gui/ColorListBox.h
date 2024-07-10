#if !defined(AFX_COLORLISTBOX_H__E2D08D7B_3B70_482C_ABC4_8CB7E94DC816__INCLUDED_)
#define AFX_COLORLISTBOX_H__E2D08D7B_3B70_482C_ABC4_8CB7E94DC816__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorListBox window
#pragma  warning(push,3)
#include <vector>
#pragma  warning(pop)
class CColorListBox : public CListBox
{
// Construction
public:
	CColorListBox();
    class ITEM_FACE
    {
    public:
        CString m_strShowString;
        COLORREF m_color;
        //LOGFONT m_font; not support now

        ITEM_FACE()
        {
            m_color = RGB(0,0,0);//
        }
    };
// Attributes
public:
    void AddStringEx(const ITEM_FACE& newFace );
    void ClearString() { m_vOutline.clear();    }
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorListBox)
public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorListBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
        void MeasureItem(LPMEASUREITEMSTRUCT);
    int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
protected:
    std::vector<ITEM_FACE>m_vOutline;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORLISTBOX_H__E2D08D7B_3B70_482C_ABC4_8CB7E94DC816__INCLUDED_)
