// ImgDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MSDev.h"
#include "ImgDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImgDialog dialog


CImgDialog::CImgDialog(UINT nIDTemplate,CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	//{{AFX_DATA_INIT(CImgDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bShown = FALSE;
	m_nAlpha = 0;
	m_Blend.BlendOp=0; //theonlyBlendOpdefinedinWindows2000
	m_Blend.BlendFlags=0; //nothingelseisspecial...
	m_Blend.AlphaFormat=1; //...
	m_Blend.SourceConstantAlpha=m_nAlpha;//AC_SRC_ALPHA // ???

	UpdateLayeredWindow = NULL;
	SetLayeredWindowAttr = NULL;
}


void CImgDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImgDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImgDialog, CDialog)
	//{{AFX_MSG_MAP(CImgDialog)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_WM_INITMENUPOPUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImgDialog message handlers
void CImgDialog::OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu)
{
    ASSERT(pPopupMenu != NULL);
    // Check the enabled state of various menu items.
    CCmdUI state;
    state.m_pMenu = pPopupMenu;
    ASSERT(state.m_pOther == NULL);
    ASSERT(state.m_pParentMenu == NULL);
	
    // Determine if menu is popup in top-level menu and set m_pOther to
    // it if so (m_pParentMenu == NULL indicates that it is secondary popup).
    HMENU hParentMenu;
    if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
	{
        state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
	}
    else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
    {
        CWnd* pParent = this;
        // Child windows don't have menus--need to go to the top!
        if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
        {
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int _nIndex = 0; _nIndex < nIndexMax; _nIndex++)
			{
				if (::GetSubMenu(hParentMenu, _nIndex) == pPopupMenu->m_hMenu)
				{
					// When popup is found, m_pParentMenu is containing menu.
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
        }
    }	

    state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	
    for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
	state.m_nIndex++)
    {
        state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
        if (state.m_nID == 0)
			continue; // Menu separator or invalid cmd - ignore it. 
		
        ASSERT(state.m_pOther == NULL);
        ASSERT(state.m_pMenu != NULL);
        if (state.m_nID == (UINT)-1)
        {
			// Possibly a popup menu, route to first item of that popup.
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // First item of popup can't be routed to.
			}
			state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
        }
        else
        {
			// Normal menu item.
			// Auto enable/disable if frame window has m_bAutoMenuEnable
			// set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, FALSE);
        }
		
        // Adjust for menu deletions and additions.
        UINT nCount = pPopupMenu->GetMenuItemCount();
        if (nCount < state.m_nIndexMax)
        {
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
        }
        state.m_nIndexMax = nCount;
    }
}

int CImgDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	HMODULE hFuncInst = LoadLibrary("User32.DLL");
	if(hFuncInst)
	{
		UpdateLayeredWindow=(PFUN_UPDATE_LAYERED_WINDOW)GetProcAddress(hFuncInst, "UpdateLayeredWindow");
		SetLayeredWindowAttr=(PFUN_SET_LAYERED_WINDOW_ATTR)GetProcAddress(hFuncInst, "SetLayeredWindowAttributes");
	}
	else
	{
		AfxMessageBox("User32.dll ERROR!");
		exit(0);
	}
	return 0;
}

UINT __cdecl CImgDialog::ShowMotionThread( LPVOID pParam )
{
	CImgDialog * pThis = static_cast<CImgDialog*>(pParam);
	
	while (pThis->m_nAlpha < 255)
	{
		pThis->m_nAlpha += 30;
		if( pThis->m_nAlpha > 255 )
			pThis->m_nAlpha = 255;
		pThis->m_Blend.SourceConstantAlpha = pThis->m_nAlpha;
		::SendMessage( pThis->GetSafeHwnd(), WM_PAINT, 0, 0);
		Sleep(60);
	}
	
	return 0;
}



void CImgDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	::ShowWindow( GetSafeHwnd(), bShow ? SW_SHOWNA : SW_HIDE);

	if( !m_bShown )
	{
		m_bShown = TRUE;
		AfxBeginThread( (AFX_THREADPROC)ShowMotionThread, this);
	}
}
