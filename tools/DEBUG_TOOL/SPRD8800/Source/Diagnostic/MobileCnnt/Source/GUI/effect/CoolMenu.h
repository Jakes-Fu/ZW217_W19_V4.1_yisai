#if !defined(AFX_COOLMENU_H__B59837D3_7752_481C_949F_9376C555282F__INCLUDED_)
#define AFX_COOLMENU_H__B59837D3_7752_481C_949F_9376C555282F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CoolMenu.h : header file
//
#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CCoolMenu window

class CCoolMenu : public CMenu
{
// Construction
public:
	CCoolMenu();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolMenu)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
	//}}AFX_VIRTUAL

// Implementation
public:
	void InitMenu(CMenu * pMenu, CToolBar * pToolBar = NULL);
	BOOL LoadPopupMenu( UINT uID, CToolBar * pToolBar = NULL);
	void ResetPopupMenu(CMenu * pMenu);

	virtual ~CCoolMenu();

public:
	
	enum{
		MAX_ITEM_TEXT_LEN = 31,
	};

protected:
	void ReleaseItemInfo(void);
	void InitToolBar(CToolBar * pToolBar);
	void DrawRect(CDC * pDC, LPCRECT pRect, UINT uState = ODS_DEFAULT, BOOL bSeparator = FALSE);
	void DrawSeparator(CDC *pDC, LPCRECT pRect, DWORD dwItemData);
	void DrawBmp( CDC * pDC, LPCRECT pRect, DWORD dwItemData, BOOL bChecked = FALSE);
	void DrawText( CDC * pDC, LPCRECT pRect, DWORD dwItemData, BOOL bGray = FALSE);

	void RecursiveInit(CMenu * pMenu);


private:
	CSize m_sizeKey;
	CSize m_sizeImg;
	
	BOOL m_bInitMenu;

	typedef struct _ITEM_INFO
	{
		_ITEM_INFO() { memset(this, 0, sizeof(_ITEM_INFO)); }

		UINT    uID;
	
		_TCHAR  szText[MAX_ITEM_TEXT_LEN + 1];

		HBITMAP hChecked; 
	    HBITMAP hUnchecked; 

		int     nImgIdx;
		UINT    uOriginalState;
	} ITEM_INFO, * PITEM_INFO;

	CTypedPtrList<CPtrList, PITEM_INFO>m_arrItemInfo;

	CToolBar  * m_pToolBar;
	CImageList  m_imgLst;
private:
	__inline void DrawBack(CDC * pDC, LPCRECT pRect, COLORREF clrBack);
	__inline int  GetImgIdxByID(UINT uCmdID);

	static const int   SEPARATOR_HEIGHT;

	static const DWORD CLR_GRAY;
	static const DWORD CLR_BACKGRND;
	static const DWORD CLR_HOT_FORE;
	static const DWORD CLR_HOT_BACK;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLMENU_H__B59837D3_7752_481C_949F_9376C555282F__INCLUDED_)
