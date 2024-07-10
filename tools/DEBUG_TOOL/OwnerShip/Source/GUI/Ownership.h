// Ownership.h : main header file for the OWNERSHIP application
//

#if !defined(AFX_OWNERSHIP_H__07C17C82_5987_45C7_A19A_F72D805452A6__INCLUDED_)
#define AFX_OWNERSHIP_H__07C17C82_5987_45C7_A19A_F72D805452A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCityMapApp:
// See CityMap.cpp for the implementation of this class
//
#pragma warning(push,3)
#include <vector>
#include <list>
#include <algorithm>
#pragma warning(pop)

typedef struct _NUMBER_INFO_T
{
	WORD  wNumber;
	WORD  wCityID;
	
	_NUMBER_INFO_T()
	{
		memset(this,0,sizeof(_NUMBER_INFO_T));
	}
	_NUMBER_INFO_T(WORD wN,WORD wC)
	{
		wNumber=wN;
		wCityID=wC;
	}
	
}NUMBER_INFO_T,*NUMBER_INFO_PTR;

// template<>
// struct std::greater<NUMBER_INFO_PTR>: public binary_function<NUMBER_INFO_PTR,NUMBER_INFO_PTR,bool>
// {
// 	bool operator()(const NUMBER_INFO_PTR & pa,const NUMBER_INFO_PTR & pb)const
// 	{
// 		return (pa->wNumber > pb->wNumber);
// 	}
// };
// 
// struct lessthan: public std::greater<NUMBER_INFO_PTR>
// {
// 	bool operator()(const NUMBER_INFO_PTR & pa,const NUMBER_INFO_PTR & pb)const
// 	{	
// 		return (pa->wNumber < pb->wNumber);
// 	}
// };


class COwnershipApp : public CWinApp
{
public:
	COwnershipApp();

    BOOL LoadConfig();
	CUIntArray m_agNumberSector;
	
	BOOL RemoveFileReadOnlyAttr(LPCTSTR lpszFileName);

	BOOL m_bCityToTxt;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COwnershipApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(COwnershipApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
extern COwnershipApp g_theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OWNERSHIP_H__07C17C82_5987_45C7_A19A_F72D805452A6__INCLUDED_)
