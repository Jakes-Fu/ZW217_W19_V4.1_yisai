/**************************************************************************
 provider_com.h

     Description:

     Author:      ke wen (kew@IIME.com.cn)
     Date Time:   11/28/2005 11:25:45 AM

     Copyright (C) 2000-2005 Beijing Chinese Star Cyber Technology Ltd.
     All rights reserved.

 **************************************************************************/

#ifndef __PROVIDER_COM_H__
#define __PROVIDER_COM_H__

IIME_FORWARD_STRUCT(Provider_Com);

/* EventHandle*/
typedef IIME_RESULT (*PFN_EVENTHANDLER)(Provider_Com *pProvider,
									  IIME_UINT hEvent,
									  IIME_UINT iMsg, 
									  PIIME_IMM_CONTENT pContent);

/*SetOptions*/
typedef IIME_RESULT (*PFN_SETOPTIONS)(Provider_Com *pProvider,
									const IIME_IMM_OPTIONS *pOption);

/* GetOptions*/
typedef IIME_RESULT (*PFN_GETOPTIONS)(Provider_Com *pProvider,
									IIME_IMM_OPTIONS *pOption);

/* add phrase */
typedef IIME_RESULT (*PFN_ADDPHRASE)(Provider_Com *pProvider,
                                   unsigned short* szPhrase,
                                   int nLen);

typedef IIME_RESULT (*PFN_CHECKASSO)(Provider_Com *pProvider,
                                   unsigned short wContext,
                                   unsigned short* szAsso,
                                   int* pNumber);

typedef IIME_RESULT (*PFN_QUERYPROPERTIES)(Provider_Com *pProvider,
                                   int *pProperties);
/*
 used for declare provider interface functions
 */
#define DECLARE_PROVIDER_FUNCTIONS(name)\
static IIME_RESULT EventHandle_##name(Provider_Com *pProvider,\
                             IIME_UINT hEvent,\
                             IIME_UINT iMsg,\
                             PIIME_IMM_CONTENT pContent);\
\
static IIME_RESULT SetOptions_##name(Provider_Com *pProvider,\
                            const IIME_IMM_OPTIONS *pOption);\
\
static IIME_RESULT GetOptions_##name(Provider_Com *pProvider,\
                            IIME_IMM_OPTIONS *pOption);\
\
static IIME_RESULT AddPhrase_##name(Provider_Com *pProvider, \
                                unsigned short* szPhrase, \
                                int nLen); \
\
static IIME_RESULT CheckAsso_##name(Provider_Com *pProvider, \
                                unsigned short wContext, \
                                unsigned short *szAsso, \
                                int* pNumber);\
\
static IIME_RESULT QueryProperties_##name(Provider_Com *pProvider, \
                                int *pProperties);

/*
 used for declare in provider
 */
#define DECLARE_PROVIDER_INTERFACE() \
	PFN_SETOPTIONS				m_pfnSetOptions;\
	PFN_GETOPTIONS				m_pfnGetOptions;\
\
	PFN_EVENTHANDLER			m_pfnEventHandle;\
\
    PFN_ADDPHRASE               m_pfnAddPhrase; \
    PFN_CHECKASSO               m_pfnCheckAsso; \
    PFN_QUERYPROPERTIES         m_pfnQueryProperties;


/*/
 used for initialize function pointer
 */
#define INITIALIZE_PROVIDER_INTERFACE(name) \
	pThis->m_pfnSetOptions = SetOptions_##name;\
	pThis->m_pfnGetOptions = GetOptions_##name;\
\
	pThis->m_pfnEventHandle = EventHandle_##name;\
\
    pThis->m_pfnAddPhrase = AddPhrase_##name; \
    pThis->m_pfnCheckAsso = CheckAsso_##name; \
    pThis->m_pfnQueryProperties = QueryProperties_##name;

struct tagProvider_Com
{
	DECLARE_PROVIDER_INTERFACE()
};

#endif /*__PROVIDER_COM_H__*/
