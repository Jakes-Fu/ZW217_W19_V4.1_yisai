/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1999 by Seain B. Conover, Tarasoft Corporation
// All rights reserved
//
// Distribute freely. Absolutely do not remove my name from the source or
// documentation (don't take credit for my work), mark your changes (don't
// get me blamed for your possible bugs), don't alter or remove this
// notice.
//
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Read *Atlas Shrugged*, http://www.aynrand.org
//
/////////////////////////////////////////////////////////////////////////////


// ModemInfo.cpp : implementation file
//

#include "stdafx.h"

#include "ModemInfoClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Delete by apple gao
//#define MAX_ENUM_COUNT		10

/////////////////////////////////////////////////////////////////////////////
// CModemInfo

CModemInfo::CModemInfo()
{
}

CModemInfo::~CModemInfo()
{
}

/////////////////////////////////////////////////////////////////////////////
// CModemInfo message handlers

const CString CModemInfo::GetProfileString( LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault, HKEY hKey ) const
{
	CString strValue = lpszDefault;

	// get section key
	HKEY hSectionKey = NULL;
	::RegOpenKeyEx( hKey, lpszSection, 0, KEY_READ, &hSectionKey );

	// if section does not exist, return default
	if ( hSectionKey != NULL )
	{
		// get desired entry
		DWORD dwType = 0;
		DWORD dwCount = 0;

		if ( ::RegQueryValueEx( hSectionKey, (LPTSTR)lpszEntry, NULL, &dwType, NULL, &dwCount ) == ERROR_SUCCESS )
		{
			ASSERT( dwType == REG_SZ );
	
			if ( ::RegQueryValueEx( hSectionKey, (LPTSTR)lpszEntry, NULL, &dwType, (LPBYTE)strValue.GetBuffer( dwCount / sizeof( TCHAR ) ), &dwCount ) == ERROR_SUCCESS )
			{
				strValue.ReleaseBuffer();
			}
			else
			{
				strValue.ReleaseBuffer();
				strValue = lpszDefault; // just in case
			}
		}

		::RegCloseKey( hSectionKey );
	}

	return strValue;
}


const CString CModemInfo::GetPaddedIndex( const int nIndex ) const
{
	CString strIndex;
	strIndex.Format( _T("%d"), nIndex );
	return CString( "0000" ).Left( 4 - strIndex.GetLength() ) + strIndex;
}

const int CModemInfo::GetModemIndex( CString strName ) const
{
	int nResult = -1;

	if ( !strName.IsEmpty() )
	{
		strName.MakeUpper();

		int nMaxCount = GetModemCount();

		int nCount;
		for ( nCount = 0; nCount < nMaxCount; nCount++ )
		{
			CString strGetModemName = GetModemName( nCount );
			strGetModemName.MakeUpper();
			if ( strName == strGetModemName && GetModemComPortByIndex( nCount ) > 0 )
			{
				nResult = nCount;
				break;
			}
		}
	}

	return nResult;
}

//Add by Apple Gao:begin
const int CModemInfo::GetModemIndexByFriendlyName( CString strName ) const
{
	int nResult = -1;

	if ( !strName.IsEmpty() )
	{
		strName.MakeUpper();

		int nMaxCount = GetModemCount();
		int nCount;
		for ( nCount = 0; nCount < nMaxCount; nCount++ )
		{
			CString strModemFriendlyName = GetModemFriendlyName( nCount );
			strModemFriendlyName.MakeUpper();
			if ( strName == strModemFriendlyName && GetModemComPortByIndex( nCount ) > 0 )
			{
				nResult = nCount;
				break;
			}
		}
	}

	return nResult;
}
//Add by Apple Gao:end

const BOOL CModemInfo::GetIsModem( const CString strName ) const
{
	return GetModemIndex( strName ) != -1;
}

const int CModemInfo::GetModemCount() const
{
//	int nResult = 0;

//	int nCount;
//	for ( nCount = 0; nCount < MAX_ENUM_COUNT; nCount++ )
//	{
//		CString strName = GetModemName( nCount );
//		if ( !strName.IsEmpty() && strName.Find( _T("IP Virtual Modem") ) == -1 ) // don't count COM/IP Virtual Modem
//		{
//			nResult++;
//		}
//	}
//
//	return nResult;

	//////////////////////////////////////////////////////////////////////////
	
	CString strSection = "SYSTEM\\CurrentControlSet\\Control\\Class\\" + GetModemClassGuid();

	HKEY hResult = NULL;
	LONG lRet = ::RegOpenKey( HKEY_LOCAL_MACHINE, strSection, &hResult );
	if( ERROR_SUCCESS != lRet )
		return FALSE;

	DWORD dwSubKeys = 0;
	LONG lReturn = RegQueryInfoKey( hResult, NULL, NULL, NULL
			, &dwSubKeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
	
	UNUSED_ALWAYS(lReturn);

	::RegCloseKey(hResult);

	return dwSubKeys;
}

const CString CModemInfo::GetModemClassGuid() const
{
//	return GetProfileString( "SYSTEM\\CurrentControlSet\\Enum\\Root\\MDMGEN\\" + GetFirstRegistrySubKey( HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Enum\\Root\\MDMGEN" ), "ClassGUID", "", HKEY_LOCAL_MACHINE );
	return "{4D36E96D-E325-11CE-BFC1-08002BE10318}";
}

const CString CModemInfo::GetModemRegistrySection( const int nIndex ) const
{
	CString strResult;

	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	GetVersionEx( &osvi );

	if ( osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
	{
		strResult = "SYSTEM\\CurrentControlSet\\Services\\Class\\Modem\\" + GetPaddedIndex( nIndex );
	}
	else
	{
		strResult = "SYSTEM\\CurrentControlSet\\Control\\Class\\" + GetModemClassGuid() + "\\" + GetPaddedIndex( nIndex );
	}

	return strResult;
}

const CString CModemInfo::GetFirstModemName() const
{
	CString strResult;

	int nMaxCount = GetModemCount();
	int nCount;
	for ( nCount = 0; nCount < nMaxCount; nCount++ )
	{
		CString strName = GetModemName( nCount );
		if ( !strName.IsEmpty() && strName.Find( _T("IP Virtual Modem") ) == -1 ) // don't add COM/IP Virtual Modem
		{
			strResult = strName;
			break;
		}
	}

	return strResult;
}

const CString CModemInfo::GetModemName( const int nIndex ) const
{
	return GetProfileString( GetModemRegistrySection( nIndex ), _T("Model"), _T(""), HKEY_LOCAL_MACHINE );
}

//Add by Apple Gao(2004-2-19)

const CString CModemInfo::GetModemFriendlyName( const int nIndex ) const
{
	return GetProfileString( GetModemRegistrySection( nIndex ), _T("FriendlyName"), _T(""), HKEY_LOCAL_MACHINE );
}

//Add by Apple Gao(2004-2-19)


const CString CModemInfo::GetModemResetString( const CString strName ) const
{
	CString strResetString = GetProfileString( GetModemRegistrySection( GetModemIndex( strName ) ), _T("Reset"), _T(""), HKEY_LOCAL_MACHINE );
	
	CString strUpper = strResetString;
	strUpper.MakeUpper();
	if ( strUpper == "NONE" )
	{
		strResetString.Empty();
	}

	return strResetString;
}

//Add by Apple Gao:begin
const CString CModemInfo::GetModemHangupString(const CString strName) const
{
	CString strHangupString = GetProfileString( 
				GetModemRegistrySection( GetModemIndex( strName ) ) + _T("\\Hangup"), 
				_T("1"), 
				_T(""), 
				HKEY_LOCAL_MACHINE );
	
	return strHangupString;
}
//Add by Apple Gao:End

const CString CModemInfo::GetModemInitString( const CString strName, const int nSpeakerVolume ) const
{
	CString strInit = _T("AT");

	const CString strKey = GetModemRegistrySection( GetModemIndex( strName ) ) + _T("\\Settings");

	CString strCallSetupFailTimer = GetProfileString( strKey, _T("CallSetupFailTimer"), _T(""), HKEY_LOCAL_MACHINE );
	strCallSetupFailTimer.Replace( _T("<#>"), _T("60") );

	CString strInactivityTimeout = GetProfileString( strKey, _T("InactivityTimeout"), _T(""), HKEY_LOCAL_MACHINE );
	strInactivityTimeout.Replace( _T("<#>"), _T("0") );

	strInit += strCallSetupFailTimer + strInactivityTimeout;

	if ( nSpeakerVolume != -1 && GetModemHasSpeaker( strName ) )
	{
		const int nNumVolumeLevels = GetModemVolumeLevelsCount( strName );
		const CString strSpeakerOn = GetProfileString( GetModemRegistrySection( GetModemIndex( strName ) ) + _T("\\Settings"), _T("SpeakerMode_Dial"), _T(""), HKEY_LOCAL_MACHINE );

		switch ( nSpeakerVolume )
		{
		case 0:
			strInit += GetProfileString( strKey, _T("SpeakerMode_OFF"), _T(""), HKEY_LOCAL_MACHINE );
			break;
		case 1:
			strInit += nNumVolumeLevels == 0 ? strSpeakerOn : GetProfileString( strKey, _T("SpeakerVolume_Low"), _T(""), HKEY_LOCAL_MACHINE ) + strSpeakerOn;
			break;
		case 2:
			strInit += nNumVolumeLevels > 0 ? GetProfileString( strKey, _T("SpeakerVolume_Med"), _T(""), HKEY_LOCAL_MACHINE ) + strSpeakerOn : "";
			break;
		case 3:
			strInit += nNumVolumeLevels > 0 ? GetProfileString( strKey, _T("SpeakerVolume_High"), _T(""), HKEY_LOCAL_MACHINE ) + strSpeakerOn : "";
			break;
		default:
			ASSERT( FALSE );
		}
	}

	strInit += GetProfileString( strKey, _T("ErrorControl_On"), _T(""), HKEY_LOCAL_MACHINE );
	strInit += GetProfileString( strKey, _T("Compression_On"), _T(""), HKEY_LOCAL_MACHINE );
	strInit += GetProfileString( strKey, _T("FlowControl_Hard"), _T(""), HKEY_LOCAL_MACHINE );
	strInit += GetProfileString( strKey, _T("Modulation_CCITT"), _T(""), HKEY_LOCAL_MACHINE );
	strInit += GetProfileString( strKey, _T("Blind_Off"), _T(""), HKEY_LOCAL_MACHINE );

	return strInit + _T("<cr>");
}

const BOOL CModemInfo::GetModemHasSpeaker( const CString strName ) const 
{
	return !GetProfileString( GetModemRegistrySection( GetModemIndex( strName ) ) + _T("\\Settings"), _T("SpeakerMode_Dial"), _T(""), HKEY_LOCAL_MACHINE ).IsEmpty() &&
		   !GetProfileString( GetModemRegistrySection( GetModemIndex( strName ) ) + _T("\\Settings"), _T("SpeakerMode_OFF"), _T(""), HKEY_LOCAL_MACHINE ).IsEmpty();
}

const int CModemInfo::GetModemVolumeLevelsCount( const CString strName ) const
{
	int nResult = 0;

	if ( !GetProfileString( GetModemRegistrySection( GetModemIndex( strName ) ) + _T("\\Settings"), _T("SpeakerVolume_Low"), _T(""), HKEY_LOCAL_MACHINE ).IsEmpty() ) nResult++;
	if ( !GetProfileString( GetModemRegistrySection( GetModemIndex( strName ) ) + _T("\\Settings"), _T("SpeakerVolume_Med"), _T(""), HKEY_LOCAL_MACHINE ).IsEmpty() ) nResult++;
	if ( !GetProfileString( GetModemRegistrySection( GetModemIndex( strName ) ) + _T("\\Settings"), _T("SpeakerVolume_High"), _T(""), HKEY_LOCAL_MACHINE ).IsEmpty() ) nResult++;

	return nResult;
}

const CString CModemInfo::GetFirstRegistrySubKey( const HKEY hKey, const CString strSection ) const
{
	CString strResult;

	CRegistryEx RegistryEx;
	if ( RegistryEx.Open( hKey, _T("")) )
	{
		CStringArray arSubKeys;
		if ( RegistryEx.ListKey( strSection, arSubKeys ) )
		{
			strResult = arSubKeys[ 0 ];
		}
		RegistryEx.Close();
	}
	
	return strResult;
}

const int CModemInfo::GetModemBps( const CString strName ) const
{
	int nResult = 0;

	CRegistryEx RegistryEx;
	if ( RegistryEx.Open( HKEY_LOCAL_MACHINE, GetModemRegistrySection( GetModemIndex( strName ) ) ) )
	{
		const int nSize = 255;
		char* pBuffer = new char[ nSize + 1];
		if ( RegistryEx.Read( _T("DCB"), pBuffer, nSize ) )
		{
			memcpy( &nResult, &pBuffer[4], 4 );
//			if ( nResult != 230400 &&
//				 nResult != 115200 &&
//				 nResult !=  57600 &&
//				 nResult !=  38400 &&
//				 nResult !=  19200 &&
//				 nResult !=   9600 &&
//				 nResult !=   4800 &&
//				 nResult !=   2400 &&
//				 nResult !=   1200 &&
//				 nResult !=    300 &&
//				 nResult !=    110 )
//			{
//				nResult = 0;
//			}
		}
		delete [] pBuffer;
		RegistryEx.Close();
	}

	return nResult;
}

const int CModemInfo::GetModemComPortByIndex( const int nIndex ) const
{
	int nResult = 0;

	const CString strPort = GetProfileString( GetModemRegistrySection( nIndex ), _T("AttachedTo"), _T(""), HKEY_LOCAL_MACHINE );
	if ( !strPort.IsEmpty() )
	{
		nResult = _ttol((strPort.Mid( 3 )) );		// ie, COM3
	}
	else
	{
//		CRegistryEx RegistryEx;
//		if ( RegistryEx.Open( HKEY_LOCAL_MACHINE, _T("") ) )
//		{
//			CString strKey = RegistryEx.FindKey( _T("Enum"), _T("Driver"), _T("Modem\\") + GetPaddedIndex( nIndex ) );
//			if ( !strKey.IsEmpty() )
//			{
//				const CString strPort = GetProfileString( strKey, _T("PORTNAME"), _T(""), HKEY_LOCAL_MACHINE );
//				if ( !strPort.IsEmpty() )
//				{
//					nResult = _ttol((strPort.Mid( 3 )) );
//				}
//			}
//			RegistryEx.Close();
//		}
	}
		
	return nResult;
}

const int CModemInfo::GetModemComPort( const CString strName ) const
{
	return GetModemComPortByIndex( GetModemIndex( strName ) );
}

//Add by Apple Gao:begin

const int CModemInfo::GetModemComPortByFriendlyName( const CString strName ) const
{
	return GetModemComPortByIndex( GetModemIndexByFriendlyName( strName ) );
}

//Add by Apple Gao:end

void CModemInfo::FillComboBoxWithModemNames( CComboBox* pComboBox )
{
	// delete exiting items
	while ( pComboBox->DeleteString( 0 ) != CB_ERR );

	// add all detected modems
	int nMaxCount = GetModemCount();
	int nCount;
	for ( nCount = 0; nCount < nMaxCount; nCount++ )
	{
		CString strName = GetModemName( nCount );
		if ( !strName.IsEmpty() && strName.Find( _T("IP Virtual Modem") ) == -1 ) // don't add COM/IP Virtual Modem
		{
			pComboBox->AddString( strName );
		}
	}
}

void CModemInfo::FillListBoxWithModemNames( CListBox* pListBox )
{
	// delete exiting items
	while ( pListBox->DeleteString( 0 ) != CB_ERR );

	// add all detected modems
	int nMaxCount = GetModemCount();
	int nCount;
	for ( nCount = 0; nCount < nMaxCount; nCount++ )
	{
		CString strName = GetModemName( nCount );
		if ( !strName.IsEmpty() && strName.Find( _T("IP Virtual Modem") ) == -1 ) // don't add COM/IP Virtual Modem
		{
			pListBox->AddString( strName );
		}
	}
}