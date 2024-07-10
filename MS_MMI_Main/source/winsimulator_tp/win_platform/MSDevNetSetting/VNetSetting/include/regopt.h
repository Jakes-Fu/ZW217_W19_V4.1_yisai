// RegOpt.h: interface for the CRegOpt class.
//
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998 by Shane Martin
// All rights reserved
//
// Distribute freely, except: don't remove my name from the source or
// documentation (don't take credit for my work), mark your changes (don't
// get me blamed for your possible bugs), don't alter or remove this
// notice.
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc., and
// I'll try to keep a version up to date.  I can be reached as follows:
//    shane.kim@kaiserslautern.netsurf.de
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGOPT_H__64755AFC_2CA0_4523_8B1E_F6B2B94699A7__INCLUDED_)
#define AFX_REGOPT_H__64755AFC_2CA0_4523_8B1E_F6B2B94699A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winreg.h>

#define		REG_RECT	0x0001
#define		REG_POINT	0x0002

class CRegOpt : public CObject  
{
public:
	CRegOpt(HKEY hKeyRoot = HKEY_LOCAL_MACHINE);
	virtual ~CRegOpt();

	struct REGINFO
	{
		LONG lMessage;
		DWORD dwType;
		DWORD dwSize;
	} m_Info;

// Operations
public:
	BOOL VerifyKey (HKEY hKeyRoot, LPCTSTR pszPath);
	BOOL VerifyKey (LPCTSTR pszPath);
	BOOL VerifyValue (LPCTSTR pszValue);
	BOOL CreateKey (HKEY hKeyRoot, LPCTSTR pszPath);
	BOOL Open (HKEY hKeyRoot, LPCTSTR pszPath);
	void Close();

	BOOL DeleteValue (LPCTSTR pszValue);
	BOOL DeleteValueKey (HKEY hKeyRoot, LPCTSTR pszPath);

	BOOL Write (LPCTSTR pszKey, int iVal);
	BOOL Write (LPCTSTR pszKey, double dVal);
	BOOL Write (LPCTSTR pszKey, DWORD dwVal);
	BOOL Write (LPCTSTR pszKey, LPCTSTR pszVal);
	BOOL Write (LPCTSTR pszKey, CStringList& scStringList);
	BOOL Write (LPCTSTR pszKey, CByteArray& bcArray);
	BOOL Write (LPCTSTR pszKey, CStringArray& scArray);
	BOOL Write (LPCTSTR pszKey, CDWordArray& dwcArray);
	BOOL Write (LPCTSTR pszKey, CWordArray& wcArray);
	BOOL Write (LPCTSTR pszKey, LPCRECT rcRect);
	BOOL Write (LPCTSTR pszKey, LPPOINT& lpPoint);

	BOOL Read (LPCTSTR pszKey, int& iVal);
	BOOL Read (LPCTSTR pszKey, double& dVal);
	BOOL Read (LPCTSTR pszKey, DWORD& dwVal);
	BOOL Read (LPCTSTR pszKey, CString& sVal);
	BOOL Read (LPCTSTR pszKey, CStringList& scStringList);
	BOOL Read (LPCTSTR pszKey, CStringArray& scArray);
	BOOL Read (LPCTSTR pszKey, CDWordArray& dwcArray);
	BOOL Read (LPCTSTR pszKey, CWordArray& wcArray);
	BOOL Read (LPCTSTR pszKey, CByteArray& bcArray);
	BOOL Read (LPCTSTR pszKey, LPPOINT& lpPoint);
	BOOL Read (LPCTSTR pszKey, LPRECT& rcRect);

protected:	
	HKEY 	m_hKey;
	CString m_sPath;
};

#endif // !defined(AFX_REGOPT_H__64755AFC_2CA0_4523_8B1E_F6B2B94699A7__INCLUDED_)
