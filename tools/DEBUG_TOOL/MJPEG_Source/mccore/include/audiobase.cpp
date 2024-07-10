/******************************************************************************
|* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
|* ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
|* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
|* PARTICULAR PURPOSE.
|* 
|* Copyright 1995-2005 Nero AG. All Rights Reserved.
|*-----------------------------------------------------------------------------
|* NeroSDK / AudioPluginManager
|*
|* FILE: audiobase.cpp
|*
|* PURPOSE: Implementation of the CStatus helper class
******************************************************************************/

//#include "stdafx.h"
#define ASSERT
#include "atlstr.h"
#include "AudioBase.h"

///////////////////////////////////////////////////////////////////// CStatus //

CStatus::CStatus(DWORD syserror)
:	m_lRef			(1),
	m_dwErr			(syserror),
	m_eAudioError	(AE_Undefined),
	m_category		(ESC_General)
{}

CStatus::CStatus(EAudioError audioerror)
:	m_lRef			(1),
	m_dwErr			(0),
	m_eAudioError	(audioerror),
	m_category		(ESC_General)
{}

CStatus::CStatus(const char *szError)
:	m_lRef			(1),
	m_dwErr			(0),
	m_eAudioError	(AE_Undefined),
	m_csError		(szError),
	m_category		(ESC_General)
{}
																// IUnknown

STDMETHODIMP CStatus::QueryInterface(REFIID riid,
										void __RPC_FAR *__RPC_FAR *ppObj)
{
	if(!ppObj)
	{
		ASSERT(FALSE);
		
		return E_INVALIDARG;
	}

	*ppObj = NULL;

	if(IsEqualIID(riid, IID_IUnknown))
		*ppObj = static_cast<IUnknown*>(static_cast<IStatus*>(this));
	else
	if(IsEqualIID(riid, IID_IStatus))
		*ppObj = static_cast<IStatus*>(this);
	else
	if(IsEqualIID(riid, IID_IStatusCategory))
	{
		if(ESC_General != m_category)
			*ppObj = static_cast<IStatusCategory*>(this);
	}

	if(*ppObj)
		AddRef();

	return *ppObj? S_OK: E_NOINTERFACE;
}
    
STDMETHODIMP_(ULONG) CStatus::AddRef()
{
	InterlockedIncrement(&m_lRef);

	return m_lRef;
}
            
STDMETHODIMP_(ULONG) CStatus::Release()
{
	InterlockedDecrement(&m_lRef);

	if(!m_lRef)
	{
		delete this;

		return 0;
	}

	return m_lRef;
}
																// IStatus
const char * CStatus::GetDescription()
{
	return m_csError;
}

EAudioError CStatus::GetAudioError()
{
	return m_eAudioError;
}

DWORD CStatus::GetSysError()
{
	return m_dwErr;
}

void CStatus::SetCategory(EStatusCategory cat)
{
	m_category = cat;
}

EStatusCategory CStatus::GetCategory()
{
	return m_category;
}
