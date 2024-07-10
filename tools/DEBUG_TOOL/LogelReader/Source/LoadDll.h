// LoadDll.h: interface for the CLoadDll class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOADDLL_H__580D75FB_DC57_474F_A3F0_FC8D863F4976__INCLUDED_)
#define AFX_LOADDLL_H__580D75FB_DC57_474F_A3F0_FC8D863F4976__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Channelitf.h"

class CLoadDll  
{
public:
	
	HINSTANCE m_hCommDll;
    BOOL  UnloadCommHandler();
	BOOL  LoadCommHandler(LPCTSTR lpszDllName);
	CLoadDll();
	virtual ~CLoadDll();
	ICommunicationChannel * m_pCommhandler;
};
// extern CLoadDll g_dllSocObj;
// extern CLoadDll g_dllObj;
#endif // !defined(AFX_LOADDLL_H__580D75FB_DC57_474F_A3F0_FC8D863F4976__INCLUDED_)
