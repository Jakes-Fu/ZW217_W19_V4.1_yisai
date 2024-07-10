// RegOpt.cpp: implementation of the CRegOpt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RegOpt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegOpt::CRegOpt(HKEY hKeyRoot)
{
	m_hKey = hKeyRoot;
}

CRegOpt::~CRegOpt()
{
	Close();
}

//////////////////////////////////////////////////////////////////////

BOOL CRegOpt::VerifyKey (HKEY hKeyRoot, LPCTSTR pszPath)
{
	ASSERT (hKeyRoot);
	ASSERT (pszPath);

	LONG ReturnValue = RegOpenKeyEx (hKeyRoot, pszPath, 0L,
		KEY_ALL_ACCESS, &m_hKey);
	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = 0L;
	m_Info.dwType = 0L;

	return FALSE;
}

BOOL CRegOpt::VerifyKey (LPCTSTR pszPath)
{
	ASSERT (m_hKey);

	LONG ReturnValue = RegOpenKeyEx (m_hKey, pszPath, 0L,
		KEY_ALL_ACCESS, &m_hKey);
	
	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = 0L;
	m_Info.dwType = 0L;

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegOpt::VerifyValue (LPCTSTR pszValue)
{
	ASSERT(m_hKey);
	LONG lReturn = RegQueryValueEx(m_hKey, pszValue, NULL,
		NULL, NULL, NULL);

	m_Info.lMessage = lReturn;
	m_Info.dwSize = 0L;
	m_Info.dwType = 0L;

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegOpt::CreateKey (HKEY hKeyRoot, LPCTSTR pszPath)
{
	DWORD dw;

	LONG ReturnValue = RegCreateKeyEx (hKeyRoot, pszPath, 0L, NULL,
		REG_OPTION_RESERVED, KEY_ALL_ACCESS, NULL, 
		&m_hKey, &dw);

	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = 0L;
	m_Info.dwType = 0L;

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegOpt::Open (HKEY hKeyRoot, LPCTSTR pszPath)
{
	m_sPath = pszPath;

	LONG ReturnValue = RegOpenKeyEx (hKeyRoot, pszPath, 0L,
		KEY_ALL_ACCESS, &m_hKey);

	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = 0L;
	m_Info.dwType = 0L;

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

void CRegOpt::Close()
{
	if (m_hKey)
	{
		RegCloseKey (m_hKey);
		m_hKey = NULL;
	}
}

BOOL CRegOpt::Write (LPCTSTR pszKey, int iVal)
{
	DWORD dwValue;

	ASSERT(m_hKey);
	ASSERT(pszKey);
	
	dwValue = (DWORD)iVal;
	LONG ReturnValue = RegSetValueEx (m_hKey, pszKey, 0L, REG_DWORD,
		(CONST BYTE*) &dwValue, sizeof(DWORD));

	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = sizeof(DWORD);
	m_Info.dwType = REG_DWORD;

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegOpt::Write (LPCTSTR pszKey, double dVal)
{
	CString strVal;
	strVal.Format("%g", dVal);
	if (Write(pszKey, strVal))
		return TRUE;
	return FALSE;
}

BOOL CRegOpt::Write (LPCTSTR pszKey, DWORD dwVal)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	return RegSetValueEx (m_hKey, pszKey, 0L, REG_DWORD,
		(CONST BYTE*) &dwVal, sizeof(DWORD));
}

BOOL CRegOpt::Write (LPCTSTR pszKey, LPCTSTR pszData)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	ASSERT(pszData);
	ASSERT(AfxIsValidAddress(pszData, strlen(pszData), FALSE));

	LONG ReturnValue = RegSetValueEx (m_hKey, pszKey, 0L, REG_SZ,
		(CONST BYTE*) pszData, strlen(pszData) + 1);

	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = strlen(pszData) + 1;
	m_Info.dwType = REG_SZ;

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegOpt::Write (LPCTSTR pszKey, CStringList& scStringList)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const int iMaxChars = 4096;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	CMemFile file(byData, iMaxChars, 16);
	CArchive ar(&file, CArchive::store);
	ASSERT(scStringList.IsSerializable());
	scStringList.Serialize(ar);
	ar.Close();
	const DWORD dwLen = file.GetLength();
	ASSERT(dwLen < iMaxChars);
	LONG lReturn = RegSetValueEx(m_hKey, pszKey, 0, REG_BINARY,
		file.Detach(), dwLen);
	
	m_Info.lMessage = lReturn;
	m_Info.dwSize = dwLen;
	m_Info.dwType = REG_BINARY;

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegOpt::Write (LPCTSTR pszKey, CByteArray& bcArray)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const int iMaxChars = 4096;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	CMemFile file(byData, iMaxChars, 16);
	CArchive ar(&file, CArchive::store);
	ASSERT(bcArray.IsSerializable());
	bcArray.Serialize(ar);
	ar.Close();
	const DWORD dwLen = file.GetLength();
	ASSERT(dwLen < iMaxChars);
	LONG lReturn = RegSetValueEx(m_hKey, pszKey, 0, REG_BINARY,
		file.Detach(), dwLen);
	
	m_Info.lMessage = lReturn;
	m_Info.dwSize = dwLen;
	m_Info.dwType = REG_BINARY;

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegOpt::Write (LPCTSTR pszKey, CDWordArray& dwcArray)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const int iMaxChars = 4096;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	CMemFile file(byData, iMaxChars, 16);
	CArchive ar(&file, CArchive::store);
	ASSERT(dwcArray.IsSerializable());
	dwcArray.Serialize(ar);
	ar.Close();
	const DWORD dwLen = file.GetLength();
	ASSERT(dwLen < iMaxChars);
	LONG lReturn = RegSetValueEx(m_hKey, pszKey, 0, REG_BINARY,
		file.Detach(), dwLen);
	
	m_Info.lMessage = lReturn;
	m_Info.dwSize = dwLen;
	m_Info.dwType = REG_BINARY;

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegOpt::Write (LPCTSTR pszKey, CWordArray& wcArray)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const int iMaxChars = 4096;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	CMemFile file(byData, iMaxChars, 16);
	CArchive ar(&file, CArchive::store);
	ASSERT(wcArray.IsSerializable());
	wcArray.Serialize(ar);
	ar.Close();
	const DWORD dwLen = file.GetLength();
	ASSERT(dwLen < iMaxChars);
	LONG lReturn = RegSetValueEx(m_hKey, pszKey, 0, REG_BINARY,
		file.Detach(), dwLen);
	
	m_Info.lMessage = lReturn;
	m_Info.dwSize = dwLen;
	m_Info.dwType = REG_BINARY;

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegOpt::Write (LPCTSTR pszKey, CStringArray& scArray)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const int iMaxChars = 4096;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	CMemFile file(byData, iMaxChars, 16);
	CArchive ar(&file, CArchive::store);
	ASSERT(scArray.IsSerializable());
	scArray.Serialize(ar);
	ar.Close();
	const DWORD dwLen = file.GetLength();
	ASSERT(dwLen < iMaxChars);
	LONG lReturn = RegSetValueEx(m_hKey, pszKey, 0, REG_BINARY,
		file.Detach(), dwLen);
	
	m_Info.lMessage = lReturn;
	m_Info.dwSize = dwLen;
	m_Info.dwType = REG_BINARY;

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegOpt::Write(LPCTSTR pszKey, LPCRECT rcRect)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const int iMaxChars = 30;
	CDWordArray dwcArray;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	dwcArray.SetSize(5);
	dwcArray.SetAt(0, rcRect->top);
	dwcArray.SetAt(1, rcRect->bottom);
	dwcArray.SetAt(2, rcRect->left);
	dwcArray.SetAt(3, rcRect->right);

	CMemFile file(byData, iMaxChars, 16);
	CArchive ar(&file, CArchive::store);
	ASSERT(dwcArray.IsSerializable());
	dwcArray.Serialize(ar);
	ar.Close();
	const DWORD dwLen = file.GetLength();
	ASSERT(dwLen < iMaxChars);
	LONG lReturn = RegSetValueEx(m_hKey, pszKey, 0, REG_BINARY,
		file.Detach(), dwLen);
	
	m_Info.lMessage = lReturn;
	m_Info.dwSize = dwLen;
	m_Info.dwType = REG_RECT;

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegOpt::Write(LPCTSTR pszKey, LPPOINT& lpPoint)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const int iMaxChars = 20;
	CDWordArray dwcArray;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	dwcArray.SetSize(5);
	dwcArray.SetAt(0, lpPoint->x);
	dwcArray.SetAt(1, lpPoint->y);

	CMemFile file(byData, iMaxChars, 16);
	CArchive ar(&file, CArchive::store);
	ASSERT(dwcArray.IsSerializable());
	dwcArray.Serialize(ar);
	ar.Close();
	const DWORD dwLen = file.GetLength();
	ASSERT(dwLen < iMaxChars);
	LONG lReturn = RegSetValueEx(m_hKey, pszKey, 0, REG_BINARY,
		file.Detach(), dwLen);
	
	m_Info.lMessage = lReturn;
	m_Info.dwSize = dwLen;
	m_Info.dwType = REG_POINT;

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegOpt::Read(LPCTSTR pszKey, int& iVal)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);

	DWORD dwType;
	DWORD dwSize = sizeof (DWORD);
	DWORD dwDest;

	LONG lReturn = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,
		&dwType, (BYTE *) &dwDest, &dwSize);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwSize;

	if(lReturn == ERROR_SUCCESS)
	{
		iVal = (int)dwDest;
		return TRUE;
	}

	return FALSE;
}

BOOL CRegOpt::Read(LPCTSTR pszKey, double& dVal)
{
	CString strVal;
	if (Read(pszKey, strVal))
	{
		dVal = atof(strVal);
		return TRUE;
	}
	return FALSE;
}

BOOL CRegOpt::Read (LPCTSTR pszKey, DWORD& dwVal)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);

	DWORD dwType;
	DWORD dwSize = sizeof (DWORD);
	DWORD dwDest;

	LONG lReturn = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL, 
		&dwType, (BYTE *) &dwDest, &dwSize);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwSize;

	if(lReturn == ERROR_SUCCESS)
	{
		dwVal = dwDest;
		return TRUE;
	}

	return FALSE;
}

BOOL CRegOpt::Read (LPCTSTR pszKey, CString& sVal)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);

	DWORD dwType;
	DWORD dwSize = 200;
	char  szString[255];

	LONG lReturn = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,
		&dwType, (BYTE *) szString, &dwSize);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwSize;

	if(lReturn == ERROR_SUCCESS)
	{
		sVal = szString;
		return TRUE;
	}

	return FALSE;
}

BOOL CRegOpt::Read (LPCTSTR pszKey, CStringList& scStringList)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const int iMaxChars = 4096;
	DWORD dwType;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	LONG lReturn = RegQueryValueEx(m_hKey, pszKey, NULL, &dwType,
		byData, &dwData);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwData;

	if(lReturn == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		ASSERT(dwData < iMaxChars);
		CMemFile file(byData, dwData);
		CArchive ar(&file, CArchive::load);
		ar.m_bForceFlat = FALSE;
		ASSERT(ar.IsLoading());
		ASSERT(scStringList.IsSerializable());
		scStringList.RemoveAll();
		scStringList.Serialize(ar);
		ar.Close();
		file.Close();
	}

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegOpt::Read (LPCTSTR pszKey, CByteArray& bcArray)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const int iMaxChars = 4096;
	int OldSize = bcArray.GetSize();
	DWORD dwType;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	LONG lReturn = RegQueryValueEx(m_hKey, pszKey, NULL, &dwType,
		byData, &dwData);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwData;

	if(lReturn == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		ASSERT(dwData < iMaxChars);
		CMemFile file(byData, dwData);
		CArchive ar(&file, CArchive::load);
		ar.m_bForceFlat = FALSE;
		ASSERT(ar.IsLoading());
		ASSERT(bcArray.IsSerializable());
		bcArray.RemoveAll();
		bcArray.SetSize(10);
		bcArray.Serialize(ar);
		bcArray.SetSize(OldSize);
		ar.Close();
		file.Close();
	}

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegOpt::Read (LPCTSTR pszKey, CDWordArray& dwcArray)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const int iMaxChars = 4096;
	int OldSize = dwcArray.GetSize();
	DWORD dwType;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	LONG lReturn = RegQueryValueEx(m_hKey, pszKey, NULL, &dwType,
		byData, &dwData);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwData;

	if(lReturn == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		ASSERT(dwData < iMaxChars);
		CMemFile file(byData, dwData);
		CArchive ar(&file, CArchive::load);
		ar.m_bForceFlat = FALSE;
		ASSERT(ar.IsLoading());
		ASSERT(dwcArray.IsSerializable());
		dwcArray.RemoveAll();
		dwcArray.SetSize(10);
		dwcArray.Serialize(ar);
		dwcArray.SetSize(OldSize);
		ar.Close();
		file.Close();
	}

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegOpt::Read (LPCTSTR pszKey, CWordArray& wcArray)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const int iMaxChars = 4096;
	int OldSize = wcArray.GetSize();
	DWORD dwType;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	LONG lReturn = RegQueryValueEx(m_hKey, pszKey, NULL, &dwType,
		byData, &dwData);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwData;

	if(lReturn == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		ASSERT(dwData < iMaxChars);
		CMemFile file(byData, dwData);
		CArchive ar(&file, CArchive::load);
		ar.m_bForceFlat = FALSE;
		ASSERT(ar.IsLoading());
		ASSERT(wcArray.IsSerializable());
		wcArray.RemoveAll();
		wcArray.SetSize(10);
		wcArray.Serialize(ar);
		wcArray.SetSize(OldSize);
		ar.Close();
		file.Close();
	}

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegOpt::Read (LPCTSTR pszKey, CStringArray& scArray)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const int iMaxChars = 4096;
	int OldSize = scArray.GetSize();
	DWORD dwType;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	LONG lReturn = RegQueryValueEx(m_hKey, pszKey, NULL, &dwType,
		byData, &dwData);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwData;

	if(lReturn == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		ASSERT(dwData < iMaxChars);
		CMemFile file(byData, dwData);
		CArchive ar(&file, CArchive::load);
		ar.m_bForceFlat = FALSE;
		ASSERT(ar.IsLoading());
		ASSERT(scArray.IsSerializable());
		scArray.RemoveAll();
		scArray.SetSize(10);
		scArray.Serialize(ar);
		scArray.SetSize(OldSize);
		ar.Close();
		file.Close();
	}

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegOpt::Read(LPCTSTR pszKey, LPRECT& rcRect)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const int iMaxChars = 30;
	CDWordArray dwcArray;
	DWORD dwType;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	LONG lReturn = RegQueryValueEx(m_hKey, pszKey, NULL, &dwType,
		byData, &dwData);

	if(lReturn == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		ASSERT(dwData < iMaxChars);
		CMemFile file(byData, dwData);
		CArchive ar(&file, CArchive::load);
		ar.m_bForceFlat = FALSE;
		ASSERT(ar.IsLoading());
		ASSERT(dwcArray.IsSerializable());
		dwcArray.RemoveAll();
		dwcArray.SetSize(5);
		dwcArray.Serialize(ar);
		ar.Close();
		file.Close();
		rcRect->top = dwcArray.GetAt(0);
		rcRect->bottom = dwcArray.GetAt(1);
		rcRect->left = dwcArray.GetAt(2);
		rcRect->right = dwcArray.GetAt(3);
	}

	m_Info.lMessage = lReturn;
	m_Info.dwType = REG_RECT;
	m_Info.dwSize = sizeof(RECT);

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegOpt::Read(LPCTSTR pszKey, LPPOINT& lpPoint)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	const int iMaxChars = 20;
	CDWordArray dwcArray;
	DWORD dwType;
	DWORD dwData = iMaxChars;
	BYTE* byData = (BYTE*)::calloc(iMaxChars, sizeof(TCHAR));
	ASSERT(byData);

	LONG lReturn = RegQueryValueEx(m_hKey, pszKey, NULL, &dwType,
		byData, &dwData);

	if(lReturn == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		ASSERT(dwData < iMaxChars);
		CMemFile file(byData, dwData);
		CArchive ar(&file, CArchive::load);
		ar.m_bForceFlat = FALSE;
		ASSERT(ar.IsLoading());
		ASSERT(dwcArray.IsSerializable());
		dwcArray.RemoveAll();
		dwcArray.SetSize(5);
		dwcArray.Serialize(ar);
		ar.Close();
		file.Close();
		lpPoint->x = dwcArray.GetAt(0);
		lpPoint->y = dwcArray.GetAt(1);
	}

	m_Info.lMessage = lReturn;
	m_Info.dwType = REG_POINT;
	m_Info.dwSize = sizeof(POINT);

	if(byData)
	{
		free(byData);
		byData = NULL;
	}

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegOpt::DeleteValue (LPCTSTR pszValue)
{
	ASSERT(m_hKey);
	LONG lReturn = RegDeleteValue(m_hKey, pszValue);


	m_Info.lMessage = lReturn;
	m_Info.dwType = 0L;
	m_Info.dwSize = 0L;

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegOpt::DeleteValueKey (HKEY hKeyRoot, LPCTSTR pszPath)
{
	ASSERT(pszPath);
	ASSERT(hKeyRoot);

	LONG lReturn = RegDeleteKey(hKeyRoot, pszPath);

	m_Info.lMessage = lReturn;
	m_Info.dwType = 0L;
	m_Info.dwSize = 0L;

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}
