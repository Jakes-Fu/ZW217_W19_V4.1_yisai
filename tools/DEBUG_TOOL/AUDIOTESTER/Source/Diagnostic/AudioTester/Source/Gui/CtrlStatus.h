// CtrlStatus.h: interface for the CCtrlStatus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTRLSTATUS_H__AD079497_56B1_4EED_AF0E_30A256E1BFFC__INCLUDED_)
#define AFX_CTRLSTATUS_H__AD079497_56B1_4EED_AF0E_30A256E1BFFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#define MODE_NAME_LEN 16
#define ID_NAME_LEN 64
struct AU_CTRL_STATUS
{
	char szModeName[MODE_NAME_LEN];		// Audio mode name,empty means all modes should be changed
	char szId[ID_NAME_LEN];				// Control id name, can not be empty
	BOOL bVisible;
	BOOL bEnable;
	RECT rcCtrl;						// Rect of the control
};

class CCtrlStatus  
{
public:
	CCtrlStatus();
	virtual ~CCtrlStatus();

public:
	// Read control status from file
	void InitCtrlStatus();

	// Refresh control status
	void RefreshControl( CString& strMode,DWORD dwId,CWnd* pWnd );

protected:
	void SetWindow( CWnd* pWnd,AU_CTRL_STATUS& act );

protected:
	CArray<AU_CTRL_STATUS,AU_CTRL_STATUS> m_arrCtrl;
};

#endif // !defined(AFX_CTRLSTATUS_H__AD079497_56B1_4EED_AF0E_30A256E1BFFC__INCLUDED_)
