// CtrlStatus.cpp: implementation of the CCtrlStatus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CtrlStatus.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCtrlStatus::CCtrlStatus()
{

}

CCtrlStatus::~CCtrlStatus()
{

}

// Read control status from file
void CCtrlStatus::InitCtrlStatus()
{
	// Get config file path name
    TCHAR szName[_MAX_PATH];
    DWORD dwRet = ::GetModuleFileName(AfxGetApp()->m_hInstance,szName,_MAX_PATH);
    if(dwRet == 0)
    {
        szName[0] = 0;
        return;
    }
    
    TCHAR* pResult = _tcsrchr(szName,_T('\\'));
    if(pResult == NULL)
    {
        szName[0] = 0;
        return;
    }
    
    *pResult = 0;
    _tcscat(szName,_T("\\CtrlList.dat"));

	try
	{
		CFile file( szName,CFile::modeRead );
		DWORD dwFileLen = file.GetLength();

		if( dwFileLen % sizeof( AU_CTRL_STATUS ) )
		{
			// not a valid file
			throw;
		}

		AU_CTRL_STATUS act;
		while( sizeof( act ) == file.Read( &act,sizeof( act ) ) )
		{
			act.szModeName[ MODE_NAME_LEN - 1 ] = 0;
			if( !stricmp( act.szModeName,"ALL" ) )
			{
				act.szModeName[0] = 0;
			}
			act.szId[ ID_NAME_LEN - 1 ] = 0;
			m_arrCtrl.Add( act );
		}
		file.Close();		
	}
	catch(...)
	{
		// Do nothing
	}
}

// Refresh control status
void CCtrlStatus::RefreshControl( CString& strMode,DWORD dwId,CWnd* pWnd )
{
	ASSERT( pWnd );

	for( int i=0;i<m_arrCtrl.GetSize();i++ )
	{
		AU_CTRL_STATUS act = m_arrCtrl[i];

		if( strlen( act.szModeName ) )
		{
#ifdef _UNICODE
			TCHAR szTmp[MODE_NAME_LEN*2] = {0};
			MultiByteToWideChar(CP_ACP,0,act.szModeName,-1,szTmp,MODE_NAME_LEN*2);
			if( strMode.Compare( szTmp ) )
#else
			if( strMode.Compare( act.szModeName ) )
#endif		
			{
				// mismatch 
				continue;
			}
		}

		extern int FindIdByName( CString& );
		CString strId(act.szId);
		DWORD nId = FindIdByName( strId );
		if( nId == dwId )
		{
			// Set window status
			SetWindow( pWnd,act );
		}
		else
		{
			// To see if it is a child control
			CWnd* pChild = pWnd->GetDlgItem( nId );
			if( pChild )
			{
				TCHAR szClassName[_MAX_PATH];
				szClassName[0] = 0;
				GetClassName( pChild->m_hWnd,szClassName,_MAX_PATH );

				if( !_tcsicmp( szClassName,_T( "ComboBox" ) ) )
				{
					// ComboBox
					if( act.bEnable & act.bVisible )
					{
						// Remove some strings
						CComboBox* pBox = (CComboBox*)pChild;
						for( int i=pBox->GetCount();i>=act.rcCtrl.left;i-- )
						{
							pBox->DeleteString(i);
						}
					}
					else
					{
						// Set window status
						SetWindow( pChild,act );
					}
				}
				else
				{
					// Set window status
					SetWindow( pChild,act );
				}
			}
		}
	}
}

void CCtrlStatus::SetWindow( CWnd* pWnd,AU_CTRL_STATUS& act )
{
	ASSERT( pWnd );
	
	if( act.bVisible )
	{
		pWnd->ShowWindow( SW_SHOW );
	}
	else
	{
		pWnd->ShowWindow( SW_HIDE );
	}
	
	pWnd->EnableWindow( act.bEnable );
	
	if( act.rcCtrl.left > 0 && act.rcCtrl.top > 0 && 
		act.rcCtrl.right > act.rcCtrl.left &&
		act.rcCtrl.bottom > act.rcCtrl.top 
	  )
	{
		pWnd->MoveWindow( &act.rcCtrl );
	}
	else if( act.rcCtrl.left == -1 && act.rcCtrl.top == -1 )
	{
		CRect rc;
		pWnd->GetWindowRect( &rc );
		rc.right += act.rcCtrl.right;
		rc.bottom += act.rcCtrl.bottom;

		if( pWnd->IsKindOf( RUNTIME_CLASS( CDialog ) ) )
		{
			pWnd->SetWindowPos( &CWnd::wndTop,rc.left,rc.top,rc.Width(),rc.Height(),SWP_SHOWWINDOW );
		}
		else
		{
			CWnd* pParent = pWnd->GetParent();
			if( pParent )
			{
				pParent->ScreenToClient( &rc );
			}
			pWnd->MoveWindow( rc );
		}
	}
	else if( act.rcCtrl.left == -2 && act.rcCtrl.top == -2 )
	{
		CRect rc;
		pWnd->GetWindowRect( &rc );
		rc.left += act.rcCtrl.right;
		rc.right += act.rcCtrl.right;
		rc.top += act.rcCtrl.bottom;
		rc.bottom += act.rcCtrl.bottom;

		if( pWnd->IsKindOf( RUNTIME_CLASS( CDialog ) ) )
		{
			pWnd->SetWindowPos( &CWnd::wndTop,rc.left,rc.top,rc.Width(),rc.Height(),SWP_SHOWWINDOW );
		}
		else
		{
			CWnd* pParent = pWnd->GetParent();
			if( pParent )
			{
				pParent->ScreenToClient( &rc );
			}
			pWnd->MoveWindow( rc );
		}
		
	}
}
