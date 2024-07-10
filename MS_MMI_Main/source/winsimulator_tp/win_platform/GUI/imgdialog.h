#if !defined(AFX_IMGDIALOG_H__1BE436BB_6366_4C79_86F3_E1E29BF32B0C__INCLUDED_)
#define AFX_IMGDIALOG_H__1BE436BB_6366_4C79_86F3_E1E29BF32B0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImgDialog.h : header file
//

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED 0x80000
#endif

#ifndef LWA_APPHA
#define LWA_APPHA 0x2
#endif

#ifndef LWA_COLORKEY
#define LWA_COLORKEY 0x1
#endif

typedef BOOL (WINAPI*PFUN_UPDATE_LAYERED_WINDOW)(HWND,HDC,POINT*,SIZE*,HDC,POINT*, COLORREF,BLENDFUNCTION*,DWORD);
typedef BOOL (WINAPI*PFUN_SET_LAYERED_WINDOW_ATTR)(HWND,COLORREF,BYTE,DWORD);

/////////////////////////////////////////////////////////////////////////////
// CImgDialog dialog

class CImgDialog : public CDialog
{
// Construction
public:
	CImgDialog(UINT nIDTemplate,CWnd* pParent = NULL);   // standard constructor

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImgDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	PFUN_UPDATE_LAYERED_WINDOW   UpdateLayeredWindow;
	PFUN_SET_LAYERED_WINDOW_ATTR SetLayeredWindowAttr;

// Implementation
protected:
	BLENDFUNCTION m_Blend;
	int           m_nAlpha;
	BOOL          m_bShown;

	// Generated message map functions
	//{{AFX_MSG(CImgDialog)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	afx_msg void OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu);
	DECLARE_MESSAGE_MAP()
private:
	static UINT __cdecl ShowMotionThread( LPVOID pParam );
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGDIALOG_H__1BE436BB_6366_4C79_86F3_E1E29BF32B0C__INCLUDED_)
