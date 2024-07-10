#if !defined(AFX_IMGVIEW_H__5CBCEB6F_6AAC_4A09_BE48_406A4D935442__INCLUDED_)
#define AFX_IMGVIEW_H__5CBCEB6F_6AAC_4A09_BE48_406A4D935442__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImgView.h : header file
//

#include "ResParser.h"
#include "DlgNtac.h"
#include "DlgBkmk.h"
#include "DlgMtva.h"

/////////////////////////////////////////////////////////////////////////////
// CImgView window

class CImgView : public CStatic
{
// Construction
public:
	CImgView();

// Attributes
public:	
	BOOL		m_bDrawImageBG;

// Operations
public:
	void UpdateResItem(RES_ITEM_PTR pRI,int nThemeIndex);
	void CreateCtrls();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImgView)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImgView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CImgView)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
protected:
	void    Init();
	void    GetAnimFrameFile(LPCTSTR lpszAnimFolder,UINT uCmpType);
	Image*  LoadImage( UINT nID, LPCTSTR lpszType, HINSTANCE hInstance =NULL);
	void    SetNextFrame();

protected:
	Image     *m_pImage;
	HBITMAP   m_hBmpMem;
	int       m_cx;
	int       m_cy;
	DWORD     m_dwBGBufSize;
	LPBYTE    m_pBGBuf;

	Image     *m_pBGImage;
	Image     *m_pRingImage;

	CStringArray m_agAnimFrameFile;
	BOOL       m_bColor;
	ARGB       m_dwColor;

	int        m_nGifFrameNum;
	int        m_nInterTime;
	int        m_nGifFramePos;

	CDlgNtac   m_wndNtac;
	CDlgBkmk   m_wndBkmk;
	CDlgMtva   m_wndMtva;

	
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGVIEW_H__5CBCEB6F_6AAC_4A09_BE48_406A4D935442__INCLUDED_)
