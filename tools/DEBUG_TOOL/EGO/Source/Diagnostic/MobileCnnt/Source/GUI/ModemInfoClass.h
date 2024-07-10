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


#if !defined(AFX_MODEMINFO_H__F83AE331_CF9D_11D2_9E1C_0060672CA551__INCLUDED_)
#define AFX_MODEMINFO_H__F83AE331_CF9D_11D2_9E1C_0060672CA551__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ModemInfo.h : header file
//
#include "RegistryEx.h"
/////////////////////////////////////////////////////////////////////////////
// CModemInfo class

class CModemInfo
{
// Construction
public:
	CModemInfo();
	virtual ~CModemInfo();


// Operations
public:

	// number of modems in machine
	const int		GetModemCount() const;			

	// reset string
	const CString	GetModemResetString( const CString strName ) const;

	// init string, with optional desired volume level specified
	// error control on, compression on, hardward flow control, modulation CCITT, blind off, call setup fail timer, inactivity timer + if requested, includes  volume init
	const CString	GetModemInitString( const CString strName, const int nSpeakerVolume = -1 ) const;

	//add by Apple Gao:begin
	const CString GetModemHangupString(const CString strName) const;
	//Add by Apple Gao:end

	// com port
	const int		GetModemComPort( const CString strName ) const;
	//add by Apple Gao:begin
	const int		GetModemComPortByFriendlyName( const CString strName ) const;
	//add by Apple Gao:end

	// maximum bps as per control panel
	const int		GetModemBps( const CString strName ) const;

	// fills CComboBoxBox with list of modem names
	void			FillComboBoxWithModemNames( CComboBox* pComboBox );

	// fills CListBox control with list of modem names
	void			FillListBoxWithModemNames( CListBox* pListBox );

	// first listed non-virtual modem name in registry
	const CString	GetFirstModemName() const;

	// modem name based on index
	const CString	GetModemName( const int nIndex ) const;

//Add by Apple Gao(2004-2-19)

	//modem friendly name based on index
	const CString GetModemFriendlyName( const int nIndex ) const;

//Add by Apple Gao(2004-2-19)
	
	// does modem have speaker
	const BOOL		GetModemHasSpeaker( const CString strName ) const;

	// how many volume levels? 
	const int		GetModemVolumeLevelsCount( const CString strName ) const;

	// is strName a valid modem?
	const BOOL		GetIsModem( const CString strName ) const;

protected:
	const CString	GetProfileString( LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault, HKEY hKey ) const;
	const CString	GetModemRegistrySection( const int nIndex ) const;
	const CString	GetModemClassGuid() const;
	const int		GetModemIndex( CString strName ) const;
	//add by Apple Gao:begin
	const int		GetModemIndexByFriendlyName( CString strName ) const;
	//add by Apple Gao:end
	const CString	GetPaddedIndex( const int nIndex ) const;
	const CString	GetFirstRegistrySubKey( const HKEY hKey, const CString strSection ) const;

//Changed by Apple Gao(2004-2-19)
public:
//Changed by Apple Gao
	const int		GetModemComPortByIndex( const int nIndex ) const;

// Attributes
protected:
	CStringArray m_aModems;
	CString	m_strModemClassGuid;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODEMINFO_H__F83AE331_CF9D_11D2_9E1C_0060672CA551__INCLUDED_)
