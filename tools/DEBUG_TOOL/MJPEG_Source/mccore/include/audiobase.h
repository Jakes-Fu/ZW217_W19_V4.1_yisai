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
|* FILE: AudioBase.h
|*
|* PURPOSE: Definition of base classes for components and some helper classes
******************************************************************************/

#ifndef _AUDIO_BASE
#	define _AUDIO_BASE

#include "AudioPluginEnv.h"

///////////////////////////////////////////////////////////////////// CStatus //

class CStatus : public IStatus, public IStatusCategory
{
// Construction/Destruction
public:
	CStatus(DWORD		syserror);
	CStatus(EAudioError	audioerror);
	CStatus(const char *szError);

// Operations
public:
	void					SetCategory(EStatusCategory cat);

																// IUnknown

	STDMETHODIMP			QueryInterface(REFIID riid,
											void __RPC_FAR *__RPC_FAR *ppObj);
	STDMETHODIMP_(ULONG)	AddRef();
	STDMETHODIMP_(ULONG)	Release();
																// IStatus
	virtual const char *	GetDescription();
	virtual EAudioError		GetAudioError();
	virtual DWORD			GetSysError();

															// IStatusCategory
	virtual EStatusCategory GetCategory();

protected:

// Data
protected:
	long					m_lRef;

	DWORD					m_dwErr;
	EAudioError				m_eAudioError;

	EStatusCategory			m_category;

	CString					m_csError;
};

/////////////////////////////////////////////////////////////// CAggregatable //

#define AGGFLAG_NODELETE	0x00000001

class CAggregatable : public IAggregatable
{
public:
	CAggregatable(DWORD dwFlags)
	:	m_pAggregator	(NULL),
		m_lRef			(0),
		m_dwFlags		(dwFlags)
	{}

	virtual ~CAggregatable()
	{}
															// IAggregatable
	virtual bool SetAggregator(IUnknown *pAggregator)
	{
		m_pAggregator = pAggregator;

		return true;
	}

	virtual bool GetAggregator(IUnknown **ppAggregator)
	{
		if(!ppAggregator)
		{
			ASSERT(FALSE);
			return false;
		}

		*ppAggregator = m_pAggregator;

		if(m_pAggregator)
			(*ppAggregator)->AddRef();

		return true;
	}

	virtual ULONG STDMETHODCALLTYPE InnerAddRef()
	{
		InterlockedIncrement(&m_lRef);

		return m_lRef;
	}
            
	virtual ULONG STDMETHODCALLTYPE InnerRelease()
	{
		InterlockedDecrement(&m_lRef);

		if(!m_lRef)
		{
			if(!(m_dwFlags & AGGFLAG_NODELETE))
				delete this;

			return 0;
		}

		return m_lRef;
	}

protected:
	IUnknown					*m_pAggregator;

	long						m_lRef;

	DWORD						m_dwFlags;
};

//////////////////////////////////////////////////////// AGGREGATABLE_UNKNOWN //

#define AGGREGATABLE_INTERFACE_MAP_BEGIN									\
																			\
STDMETHODIMP QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppObj)	\
{																			\
	return m_pAggregator? m_pAggregator->QueryInterface(riid, ppObj):		\
							InnerQueryInterface(riid, ppObj);				\
}																			\
        																	\
STDMETHODIMP_(ULONG) AddRef()												\
{																			\
	return m_pAggregator? m_pAggregator->AddRef(): InnerAddRef();			\
}																			\
        																	\
STDMETHODIMP_(ULONG) Release()												\
{																			\
	return m_pAggregator? m_pAggregator->Release(): InnerRelease();			\
}																			\
																			\
virtual HRESULT STDMETHODCALLTYPE											\
InnerQueryInterface( REFIID riid, void __RPC_FAR *__RPC_FAR *ppObj)			\
{																			\
	if(!ppObj)																\
	{																		\
		ASSERT(FALSE);														\
																			\
		return E_INVALIDARG;												\
	}																		\
																			\
	*ppObj = NULL;															\
																			\
	if(IsEqualIID(riid, IID_IUnknown))										\
		*ppObj = static_cast<IUnknown*>(static_cast<IAggregatable*>(this));	\
	else																	\
	if(IsEqualIID(riid, IID_IAggregatable))									\
		*ppObj = static_cast<IAggregatable*>(this);							\


////////////////////////////////////////////// AGGREGATABLE_INTERFACE_MAP_END //

#define AGGREGATABLE_INTERFACE_MAP_END										\
																			\
	if(*ppObj)																\
		AddRef();															\
																			\
	return *ppObj? S_OK: E_NOINTERFACE;										\
																			\
}																			\

#define INTERFACE_ENTRY(i)													\
		if(IsEqualIID(riid, IID_##i))										\
			*ppObj = static_cast<i*>(this);									\


#endif // _AUDIO_BASE
