/**************************************************************************
 wrappingprovider.c

     Description:

     Author:      ke wen (kew@IIME.com.cn)
     Date Time:   11/30/2005 10:58:06 AM

     Copyright (C) 2000-2005 Beijing Chinese Star Cyber Technology Ltd.
     All rights reserved.

 **************************************************************************/

#include "IIMEstd.h"

#include "provider_com.h"
#include "IIMEk2imm.h"

#include "wrappingprovider.h"

#include "IIMEpvdmo.h"
#if 0//ZHMCH	无此文件
#include "IIMEdpyprovider.h"
#include "IIMEmtzyprovider.h"
#include "IIMEmtfzyprovider.h"
#include "IIMEpvdstk.h"
#include "IIMEdzpyprovider.h"
#endif


/*
 Declarations
 */
typedef struct tagNestProvider
{
	IIME_UINT		idMethod;
    PFN_PROVIDER_CREATE m_pfnCreate;
    PFN_PROVIDER_DELETE m_pfnDelete;
} NestProvider;

static const NestProvider arrNestProvider[] = {
#ifdef IIME_PINYIN
	{IM_PINYIN, IIMEDPYProvider_Create, IIMEDPYProvider_Delete},
#endif

#ifdef IIME_PINYIN_MINI
	{IM_PINYIN_DZ, IIMEDZPYProvider_Create, IIMEDZPYProvider_Delete},
#endif

#if defined(IIME_ZHUYIN)
	{IM_ZHUYIN, IIMEDPYProvider_Create, IIMEDPYProvider_Delete},
	{IM_ZHUYIN_MT, IIMEMTZYProvider_Create, IIMEMTZYProvider_Delete},
	{IM_ZHUYIN_MT_FULL, IIMEMTFZYProvider_Create, IIMEMTFZYProvider_Delete},
#endif

#if defined(IIME_ZHUYIN_MINI)
	{IM_ZHUYIN, IIMEDPYProvider_Create, IIMEDPYProvider_Delete},
	{IM_ZHUYIN_MT, IIMEMTZYProvider_Create, IIMEMTZYProvider_Delete},
#endif

#if defined(IIME_STROKE)
	{IM_STROKE, IIMEStkProvider_Create, IIMEStkProvider_Delete},
	{IM_STROKE_BIG5, IIMEStkProvider_Create, IIMEStkProvider_Delete},
#endif
	0
};

static PFN_PROVIDER_CREATE Wrapping_FindCreateFunc(IIME_UINT idMethod)
{
	int i;
	for (i = 0; i < sizeof(arrNestProvider) / sizeof(arrNestProvider[0]) - 1; i++){
		if (arrNestProvider[i].idMethod == idMethod)
			return arrNestProvider[i].m_pfnCreate;
	}

	return IIME_NULL;
}

static PFN_PROVIDER_DELETE Wrapping_FindDeleteFunc(IIME_UINT idMethod)
{
	int i;
	for (i = 0; i < sizeof(arrNestProvider) / sizeof(arrNestProvider[0]) - 1; i++){
		if (arrNestProvider[i].idMethod == idMethod)
			return arrNestProvider[i].m_pfnDelete;
	}
	return IIME_NULL;
}

#define WRAPPING_STATE_PRIMER       0x0001
#define WRAPPING_STATE_MAGICONE     0x0002
#define WRAPPING_STATE_MOBACK       0x0003

DECLARE_PROVIDER_FUNCTIONS(Wrapping)

/* default nest provider implements*/
#define NEST_PROVIDER_PRECALL()\
	IIME_RESULT ret;\
	Provider_Com *pNestProvider;\
\
	WrappingProvider *pWrapping = (WrappingProvider *)pProvider;\
	if (pWrapping == IIME_NULL)\
		return IIME_ERROR_UNKNOWN;\
\
	pNestProvider = pWrapping->m_pNestProvider;\
	if (pNestProvider == IIME_NULL)\
		return IIME_ERROR_UNKNOWN;

static Provider_Com* WrappingProvider_SwitchProvider(WrappingProvider *pWrapping, IIME_BOOL bToMagicOne);

static IIME_UINT Wrapping_PreTranslateKey(WrappingProvider* pWrapping, IIME_UINT iKey);
/*
 implemention
 */

Provider_Com * WrappingProvider_Create(PIIME_IMM_PARAMS  pImmParams,
									   PIIME_USER_PARAMS pUserParam1,
									   PIIME_USER_PARAMS pUserParam2,
									   const void* pdbMisc,
									   const void* pdbMO,
                                       const IIME_MULTI_STRING_TABLE* pMulti_table,
									   IIME_INT idMethod)
{
	PFN_PROVIDER_CREATE pfnCreate;
	WrappingProvider *pThis = (WrappingProvider *)IIME_MALLOC(sizeof(WrappingProvider));

	INITIALIZE_PROVIDER_INTERFACE(Wrapping)

    idMethod = (idMethod > IM_SURETYPE_BASE) ? idMethod - IM_SURETYPE_BASE : idMethod;

	pThis->m_pImmParam = pImmParams;
    pThis->m_UserParam1.iUserCacheSize = pUserParam1->iUserCacheSize;
    pThis->m_UserParam1.pUserCache = pUserParam1->pUserCache;

    pThis->m_UserParam2.iUserCacheSize = pUserParam2->iUserCacheSize;
    pThis->m_UserParam2.pUserCache = pUserParam2->pUserCache;

	pThis->m_pdbMisc = pdbMisc;
	pThis->m_pdbMO = pdbMO;
    pThis->m_idMethod = idMethod;
	pThis->m_pOption = IIME_NULL;

	pfnCreate = Wrapping_FindCreateFunc(idMethod);
	if (pfnCreate != IIME_NULL){
		pThis->m_pNestProvider = (*pfnCreate)(pImmParams, 
                                    &(pThis->m_UserParam1), 
                                    &(pThis->m_UserParam2), 
									pdbMisc,
									pdbMO,
                                    IIME_NULL,
									idMethod);
	}

	pThis->m_iState = WRAPPING_STATE_PRIMER;

	return (Provider_Com*)pThis;
}

void WrappingProvider_Delete(Provider_Com *pComProvider)
{
	WrappingProvider *pThis = (WrappingProvider*)pComProvider;

	if (pThis->m_iState == WRAPPING_STATE_MAGICONE || pThis->m_iState == WRAPPING_STATE_MOBACK)
		IIMEMagicProvider_Delete(pThis->m_pNestProvider);
	else{
		PFN_PROVIDER_DELETE pfnDelete = Wrapping_FindDeleteFunc(pThis->m_idMethod);
		(*pfnDelete)(pThis->m_pNestProvider);
	}

	IIME_FREE(pThis);
}

/* EventHandle*/
static IIME_RESULT EventHandle_Wrapping(Provider_Com *pProvider,
							   IIME_UINT hEvent,
							   IIME_UINT iMsg, 
							   PIIME_IMM_CONTENT pContent)
{
	NEST_PROVIDER_PRECALL();

	if (pWrapping->m_iState == WRAPPING_STATE_MOBACK){
		pNestProvider = WrappingProvider_SwitchProvider(pWrapping, IIME_FALSE);
		pWrapping->m_iState = WRAPPING_STATE_PRIMER;
	}

	if (hEvent == IIME_EVENT_KEYBOARD)
		iMsg = Wrapping_PreTranslateKey(pWrapping, iMsg);

	ret = (*(pNestProvider->m_pfnEventHandle))(pNestProvider, hEvent, iMsg, pContent);

	if (ret == IIME_ERROR_MAGIC_ONE){
		pNestProvider = WrappingProvider_SwitchProvider(pWrapping, IIME_TRUE);
		pWrapping->m_iState = WRAPPING_STATE_MAGICONE;

		if (IIMEMagicProvider_SetInputMethod(pNestProvider, IM_MAGIC_ONE, pContent) == IM_MAGIC_ONE)
			ret = IIME_ERROR_USED;
		else
			ret = IIME_ERROR_UNUSED;
	}
	else if (ret == IIME_ERROR_RETURN_LAST_PROVIDER){
		pWrapping->m_iState = WRAPPING_STATE_MOBACK;
		ret = IIME_ERROR_USED;
	}

	return ret;
 }


/*SetOptions*/
static IIME_RESULT SetOptions_Wrapping(Provider_Com *pProvider,
							   const IIME_IMM_OPTIONS *pOption)
{
	NEST_PROVIDER_PRECALL();

	ret = (*(pNestProvider->m_pfnSetOptions))(pNestProvider, pOption);

	pWrapping->m_pOption = (IIME_IMM_OPTIONS *)pOption;
	return ret;
 }

/* GetOptions*/
static IIME_RESULT GetOptions_Wrapping(Provider_Com *pProvider,
							   IIME_IMM_OPTIONS *pOption)
{
	NEST_PROVIDER_PRECALL();

	ret = (*(pNestProvider->m_pfnGetOptions))(pNestProvider, pOption);

	return ret;
 }

static IIME_RESULT AddPhrase_Wrapping(Provider_Com* pProvider,
                             unsigned short* szPhrase,
                             int len)
{
	NEST_PROVIDER_PRECALL();

	ret = (*(pNestProvider->m_pfnAddPhrase))(pNestProvider, szPhrase, len);

	return ret;
}

static IIME_RESULT CheckAsso_Wrapping(Provider_Com* pProvider,
                             unsigned short wContext,
                             unsigned short *szAsso,
                             int *pNumber)
{
	NEST_PROVIDER_PRECALL();

	ret = (*(pNestProvider->m_pfnCheckAsso))(pNestProvider, wContext, szAsso, pNumber);

	return ret;
}

static IIME_RESULT QueryProperties_Wrapping(Provider_Com* pProvider,
                             int *pProperties)
{
	NEST_PROVIDER_PRECALL();

	ret = (*(pNestProvider->m_pfnQueryProperties))(pNestProvider, pProperties);

	return ret;
}

/*
 implemention
 */

static Provider_Com * WrappingProvider_SwitchProvider(WrappingProvider *pWrapping, IIME_BOOL bToMagicOne)
{
    PFN_PROVIDER_CREATE pfnCreate;
    PFN_PROVIDER_DELETE pfnDelete;

#ifdef SELFALLOC_PRODUCT
	IIME_BYTE *pFrom = (IIME_BYTE *)(pWrapping->m_pNestProvider);
#endif

	if(pWrapping->m_pNestProvider == IIME_NULL)
		return IIME_NULL;

	if (bToMagicOne){
		pfnDelete = Wrapping_FindDeleteFunc(pWrapping->m_idMethod);
		if (pfnDelete != IIME_NULL){
			(*pfnDelete)(pWrapping->m_pNestProvider);
		}

/*
		if ((pWrapping->m_idMethod == IM_PINYIN) || (pWrapping->m_idMethod == IM_ZHUYIN))
			IIMEDPYProvider_Delete(pWrapping->m_pNestProvider);
		else
			IIMEStkProvider_Delete(pWrapping->m_pNestProvider);
*/

#ifdef SELFALLOC_PRODUCT
		IIME_MRESET(pFrom);
#endif
		pWrapping->m_pNestProvider = IIMEMagicProvider_Create(pWrapping->m_pImmParam,
														    &(pWrapping->m_UserParam2),
														    &(pWrapping->m_UserParam1),
														    pWrapping->m_pdbMO,
														    pWrapping->m_pdbMisc,
                                                            IIME_NULL,
														    pWrapping->m_idMethod);
        (*(pWrapping->m_pNestProvider->m_pfnSetOptions))(pWrapping->m_pNestProvider, pWrapping->m_pOption);
	}
	else{
		IIMEMagicProvider_Delete(pWrapping->m_pNestProvider);
		
#ifdef SELFALLOC_PRODUCT
		IIME_MRESET(pFrom);
#endif
		pfnCreate = Wrapping_FindCreateFunc(pWrapping->m_idMethod);
		if (pfnCreate != IIME_NULL){
			pWrapping->m_pNestProvider = (*pfnCreate)(pWrapping->m_pImmParam,
													  &(pWrapping->m_UserParam1),
													  &(pWrapping->m_UserParam2),
													  pWrapping->m_pdbMisc,
													  pWrapping->m_pdbMO,
                                                      IIME_NULL,
													  pWrapping->m_idMethod);
			(*(pWrapping->m_pNestProvider->m_pfnSetOptions))(pWrapping->m_pNestProvider, pWrapping->m_pOption);
		}
	}

	return pWrapping->m_pNestProvider;
}

static IIME_UINT Wrapping_PreTranslateKey(WrappingProvider* pWrapping, IIME_UINT iKey)
{
	/*IIME_UINT iRetKey  = iKey;*/
	IIME_INT iState = pWrapping->m_iState;
#ifdef KEYBOARD_IME_SHARP
	if (iKey == IIME_VK_0)
	{
		if (iState == WRAPPING_STATE_MAGICONE){
		}
		else if(pWrapping->m_idMethod == IM_ZHUYIN_MT){
		}
		else{
			iKey = IIME_VK_SPACE;
		}
	}
	else if (iKey == IIME_VK_STAR)
	{
		if (iState == WRAPPING_STATE_MAGICONE){
			iKey = IIME_VK_AUX;
		}
		else if (pWrapping->m_idMethod == IM_ZHUYIN || pWrapping->m_idMethod == IM_ZHUYIN_MT){
		}
		else{
			iKey = IIME_VK_AUX;
		}

	}
    else if (iKey == IIME_VK_POUND)
    {
        if (iState != WRAPPING_STATE_MAGICONE && pWrapping->m_idMethod == IM_ZHUYIN){
            iKey = IIME_VK_AUX;
        }
    }

#elif defined KEYBOARD_IME_STAR
	if (iState != WRAPPING_STATE_MAGICONE && 
		(pWrapping->m_idMethod == IM_PINYIN || pWrapping->m_idMethod == IM_ZHUYIN)
		&& iKey == IIME_VK_0)
		iKey = IIME_VK_AUX;
#else
	#error No keyboard type defined!
#endif

	return iKey;
}
