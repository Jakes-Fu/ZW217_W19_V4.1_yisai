/**************************************************************************
 IIMEudmtprovider.c

     Description:

     Author:      ke wen (kew@IIME.com.cn)
     Date Time:   12/4/2005 12:33:08 PM

     Copyright (C) 2000-2005 Beijing Chinese Star Cyber Technology Ltd.
     All rights reserved.

 **************************************************************************/
#if 0
#include "IIMEstd.h"
#include "provider_com.h"
#include "IIMEudmtprovider.h"
#else
#include "IIMEstd.h"
#include "provider_com.h"
#include "IIMEudmtprovider.h"
#endif

#include "IIMEstring.h"//IIMEString_Append		ZHMCH


#define IIME_UI_COMMIT_BACKSPACE		0x04000000		/* backspace the last char */

/*
 Declaration
 */

DECLARE_PROVIDER_FUNCTIONS(IIMEUDMTProvider)


static IIME_RESULT IIMEUDMT_KeyboardHandle(IIMEUDMTProvider *pThis, IIME_UINT iMsg);
static IIME_RESULT IIMEUDMT_TimerHandle(IIMEUDMTProvider *pThis);

static void IIMEMT_FillCommit(IIMEUDMTProvider *pThis);
static void IIMEMT_FillUnderline(IIMEUDMTProvider *pThis);


/*
 Constructor an destructor
 */

Provider_Com * IIMEUDMTProvider_Create(PIIME_IMM_PARAMS  pImmParams,
								   PIIME_USER_PARAMS pUserParam1,
								   PIIME_USER_PARAMS pUserParam2,
								   const void*	  pdbMisc,
							       const void*	  pdbMO,
                                   const IIME_MULTI_STRING_TABLE* pMulti_string_table,
								   IIME_INT idMethod)
{
    IIMEUDMTProvider *pThis = (IIMEUDMTProvider *)IIME_MALLOC(sizeof(IIMEUDMTProvider));
	memset(pThis, 0, sizeof(IIMEUDMTProvider));

   	/* initialize interface functions*/
	INITIALIZE_PROVIDER_INTERFACE(IIMEUDMTProvider)

    pThis->m_idMethod = idMethod;
    pThis->m_pMultiTable = pMulti_string_table;

    return (Provider_Com*)pThis;
}

void IIMEUDMTProvider_Delete(Provider_Com *pThis)
{
    IIMEUDMTProvider *p = (IIMEUDMTProvider *)pThis;
    if (p != IIME_NULL)
        IIME_FREE(pThis);
}

/*
 interface
 */

static IIME_RESULT EventHandle_IIMEUDMTProvider(Provider_Com *pProvider,
                                   IIME_UINT hEvent,
                                   IIME_UINT iMsg,
                                   IIME_IMM_CONTENT *pContent)
{
	IIME_BOOL ret;
	IIMEUDMTProvider *pThis = (IIMEUDMTProvider *)pProvider;

	pThis->m_dwUpdateFlag = 0;
    memset(&(pThis->m_immContent), 0, sizeof(IIME_IMM_CONTENT));

    if (hEvent == IIME_EVENT_KEYBOARD){
		ret = IIMEUDMT_KeyboardHandle(pThis, iMsg);
        if (iMsg & IIME_VK_LONGCLICK)
            pThis->m_wPreLongKey = (IIME_WCHAR)(iMsg & 0x0000FFFF);
        else
            pThis->m_wPreLongKey = 0;
    }
	else if (hEvent == IIME_EVENT_TIMER)
		ret = IIMEUDMT_TimerHandle(pThis);
	else
		ret = IIME_ERROR_UNKNOWN;

    if (ret != IIME_ERROR_UNUSED)
    {
	    if (pThis->m_dwUpdateFlag & IIME_UI_COMMIT_BACKSPACE){
		    pContent->pCommit[0] = 0x08;		/* backspace */
	    }

        if (pThis->m_immContent.nUnderline)
        {
            pThis->m_immContent.flagTimer = IIME_TIMER_SET;
        }
        else
            pThis->m_immContent.flagTimer = IIME_TIMER_CLEAR;
    }

    memcpy(pContent, &(pThis->m_immContent), sizeof(IIME_IMM_CONTENT));

    return ret;
}

static void IIMEMT_FillCommit(IIMEUDMTProvider *pThis)
{
    IIME_WCHAR *pCommitString = pThis->m_immContent.pCommit;
	IIME_WCHAR wszCommit[2];
	wszCommit[0] = pThis->m_wCommit;
	wszCommit[1] = 0;

	IIMEString_Append(pCommitString, wszCommit);

}

static void IIMEMT_FillUnderline(IIMEUDMTProvider *pThis)
{
    IIME_PWSTR pUnderlineString = pThis->m_immContent.pUnderlineBuffer;
    IIME_WORD *pUnderlineStringLen = &(pThis->m_immContent.nUnderline);
    IIME_WORD *pCaretPos = &(pThis->m_immContent.iCaretPos);

	pUnderlineString[0] = pThis->m_wUnderline;
	pUnderlineString[1] = 0;
	*pUnderlineStringLen = 1;

	*pCaretPos = 0;
}

static IIME_RESULT SetOptions_IIMEUDMTProvider(Provider_Com *pProvider,
                                  const IIME_IMM_OPTIONS *pOption)
{
    return IIME_ERROR_SUCCESS;
}

static IIME_RESULT GetOptions_IIMEUDMTProvider(Provider_Com *pProvider,
                                  IIME_IMM_OPTIONS *pOption)
{
    return IIME_ERROR_SUCCESS;
}

static IIME_RESULT AddPhrase_IIMEUDMTProvider(Provider_Com* pProvider, unsigned short* szPhrase, int nLen)
{
    return IIME_ERROR_UNKNOWN;
}

static IIME_RESULT CheckAsso_IIMEUDMTProvider(Provider_Com* pProvider, unsigned short wContext, unsigned short* szAsso, int* pNumber)
{
    return IIME_ERROR_UNKNOWN;
}

static IIME_RESULT QueryProperties_IIMEUDMTProvider(Provider_Com* pProvider, int* pProperties)
{
    return IIME_ERROR_UNKNOWN;
}
/*
 implemention
 */


static IIME_BOOL IIMEUDMT_IsUsedKey(IIMEUDMTProvider *pThis, IIME_UINT iMsg)
{
    int i;
    for (i = 0; i < pThis->m_pMultiTable->long_multi_key_number; i++)
    {
        const MULTI_STRING * pMultiString = pThis->m_pMultiTable->long_multi_table + i;
        unsigned short key = (*pMultiString)[0];
        if (iMsg == key)
            return IIME_TRUE;
    }

    for (i = 0; i < pThis->m_pMultiTable->multi_key_number; i++)
    {
        const MULTI_STRING *pMultiString = pThis->m_pMultiTable->multi_table + i;
        unsigned short key = (*pMultiString)[0];
        if (iMsg == key)
            return IIME_TRUE;
    }

    return IIME_FALSE;

}

static IIME_BOOL IIMEUDMT_IsLongKey(IIMEUDMTProvider *pThis, IIME_UINT iMsg)
{
    int i;
    for (i = 0; i < pThis->m_pMultiTable->long_multi_key_number; i++)
    {
        const MULTI_STRING * pMultiString = pThis->m_pMultiTable->long_multi_table + i;
        unsigned short key = (*pMultiString)[0];
        if (iMsg == key)
            return IIME_TRUE;
    }

    return IIME_FALSE;
}

static unsigned short IIMEUDMT_GetSureKeyValue(IIMEUDMTProvider *pThis, IIME_UINT iMsg)
{
    int i;
    unsigned short value = 0;

    for (i = 0; i < pThis->m_pMultiTable->long_multi_key_number; i++)
    {
        const MULTI_STRING *pMultiString = pThis->m_pMultiTable->long_multi_table + i;
        unsigned short key = (*pMultiString)[0];
        if (iMsg == key){
            value = (*pMultiString)[1];
            if (value == 0)
                value = key;
            break;
        }
    }

    return value;
}

static unsigned short IIMEUDMT_GetLongKeyValue(IIMEUDMTProvider *pThis, IIME_UINT iMsg)
{
    int i;
    unsigned short value = 0;

    for (i = 0; i < pThis->m_pMultiTable->multi_key_number; i++)
    {
        const MULTI_STRING *pMultiString = pThis->m_pMultiTable->multi_table + i;
        unsigned short key = (*pMultiString)[0];
        if (iMsg == key){
            value = (*pMultiString)[0];
            if (value == 0)
                value = key;
            break;
        }
    }

    return value;
}

static IIME_BOOL IIMEUDMT_QueryMultiValues1(IIMEUDMTProvider *pThis, IIME_UINT iMsg, int *pNum, const unsigned short **ppList)
{
    int i;

    for (i = 0; i < pThis->m_pMultiTable->long_multi_key_number; i++)
    {
        const MULTI_STRING *pMultiString = pThis->m_pMultiTable->long_multi_table + i;
        unsigned short key = (*pMultiString)[0];
        if (iMsg == key){
            int j = 0;
            const unsigned short *pList = (*pMultiString) + 2;
            *ppList = pList;
            while(*(pList)){
                j++;
                pList++;
            }
            *pNum = j;
            return IIME_TRUE;
        }
    }

    return IIME_FALSE;
}

static IIME_BOOL IIMEUDMT_QueryMultiValues2(IIMEUDMTProvider *pThis, IIME_UINT iMsg, int *pNum, const unsigned short **ppList)
{
    int i;

    for (i = 0; i < pThis->m_pMultiTable->multi_key_number; i++)
    {
        const MULTI_STRING *pMultiString = pThis->m_pMultiTable->multi_table + i;
        unsigned short key = (*pMultiString)[0];
        if (iMsg == key){
            int j = 0;
            const unsigned short *pList = (*pMultiString) + 2;
            *ppList = pList;
            while(*(pList)){
                j++;
                pList++;
            }
            *pNum = j;
            return IIME_TRUE;
        }
    }

    return IIME_FALSE;
}

static IIME_RESULT IIMEMT_LongKeyHandle(IIMEUDMTProvider *pThis, IIME_UINT iMsg)
{
    int num;
	const unsigned short *pList;

    IIME_RESULT ret = IIME_ERROR_USED;
	IIME_BOOL bLongClick = iMsg & IIME_VK_LONGCLICK;

	iMsg &= ~IIME_VK_LONGCLICK;

    if (iMsg == pThis->m_nPreKey)	/* same key as before*/
    {
        if (bLongClick)
        {
            ;/* Never reach here! pThis->m_nCount++;*/
        }
        else
        {
            if (pThis->m_wUnderline)
                pThis->m_wCommit = pThis->m_wUnderline;
            else
                pThis->m_wCommit = IIMEUDMT_GetSureKeyValue(pThis, iMsg);

            IIMEMT_FillCommit(pThis);

		    pThis->m_wUnderline = 0;
		    pThis->m_nCount = 0;
		    pThis->m_nPreKey = 0;
        }
    }
    else
    {
		if (pThis->m_nPreKey){
            IIMEMT_FillCommit(pThis);
		}

		if (bLongClick){
            pThis->m_nCount++;
            if (IIMEUDMT_QueryMultiValues1(pThis, iMsg, &num, &pList))
            {
                if (num > 1)
                {
                    pThis->m_wUnderline = pList[(pThis->m_nCount - 1) % num];
                    pThis->m_wCommit = 0;

                    IIMEMT_FillUnderline(pThis);
                }
                else
                {
                    pThis->m_wUnderline = 0;
                    pThis->m_wCommit = pList[0];

                    IIMEMT_FillCommit(pThis);
                }
            }

            pThis->m_nPreKey = iMsg;
        }
        else{
            pThis->m_wCommit = IIMEUDMT_GetSureKeyValue(pThis, iMsg);
            IIMEMT_FillCommit(pThis);
        }

    }
    return ret;
}

static IIME_RESULT IIMEMT_NormalKeyHandle(IIMEUDMTProvider *pThis, IIME_UINT iMsg)
{
    int num;
	const unsigned short *pList;

    IIME_RESULT ret = IIME_ERROR_USED;
	IIME_BOOL bLongClick = iMsg & IIME_VK_LONGCLICK;

	iMsg &= ~IIME_VK_LONGCLICK;

    if (iMsg == pThis->m_nPreKey)	/* same key as before*/
    {
	    if (bLongClick)
        {
		    pThis->m_nCount = 0;
		    pThis->m_nPreKey = 0;

		    pThis->m_wUnderline = 0;
		    pThis->m_wCommit = IIMEUDMT_GetLongKeyValue(pThis, iMsg);
			
            IIMEMT_FillCommit(pThis);
	    }
	    else
        {
		    pThis->m_nCount++;
			
		    if(IIMEUDMT_QueryMultiValues2(pThis, iMsg, &num, &pList))
            {
			    if (num > 1){
				    pThis->m_wUnderline = pList[(pThis->m_nCount - 1) % num];
				    pThis->m_wCommit = 0;

                    IIMEMT_FillUnderline(pThis);
			    }
			    else{
				    pThis->m_wUnderline = 0;
				    pThis->m_wCommit = pList[0];

                    IIMEMT_FillCommit(pThis);
			    }
		    }

		    pThis->m_nPreKey = iMsg;
	    }
    }
    else        /* different with before */
    {
		if (pThis->m_nPreKey){
            pThis->m_wCommit = pThis->m_wUnderline;
            IIMEMT_FillCommit(pThis);
            pThis->m_wCommit = 0;
		}

		if (bLongClick){
            pThis->m_nPreKey = 0;
		    pThis->m_nCount = 0;

		    pThis->m_wUnderline = 0;
		    pThis->m_wCommit = IIMEUDMT_GetLongKeyValue(pThis, iMsg);

            IIMEMT_FillCommit(pThis);
		    ret = IIME_ERROR_USED;
		}
        else
        {
            if (pThis->m_wPreLongKey != iMsg)
            {
                pThis->m_nCount = 1;
		        pThis->m_nPreKey = iMsg;
        		
		        if (IIMEUDMT_QueryMultiValues2(pThis, iMsg, &num, &pList)){
			        if (num > 1){
				        pThis->m_wUnderline = pList[(pThis->m_nCount - 1) % num];

                        IIMEMT_FillUnderline(pThis);
			        }
			        else{
				        pThis->m_wUnderline = 0;
				        pThis->m_wCommit = pList[0];

                        IIMEMT_FillCommit(pThis);

				        pThis->m_nPreKey = 0;
				        pThis->m_nCount = 0;
			        }
		        }
            }
        }
    }

    return ret;
}

static IIME_RESULT IIMEUDMT_KeyboardHandle(IIMEUDMTProvider *pThis, IIME_UINT iMsg)
{
	IIME_RESULT ret;
/*  IIME_BOOL bLongClick = iMsg & IIME_VK_LONGCLICK;*/

/*  iMsg &= ~IIME_VK_LONGCLICK;*/

	if (!IIMEUDMT_IsUsedKey(pThis, iMsg & 0x0000FFFF)){
		ret = IIME_ERROR_UNUSED;


		if (pThis->m_nPreKey){
			pThis->m_wCommit = pThis->m_wUnderline;
			pThis->m_wUnderline = 0;
			pThis->m_nCount = 0;

            IIMEMT_FillCommit(pThis);

			pThis->m_nPreKey = 0;
			ret = IIME_ERROR_USED;
		}

	}
    else if (IIMEUDMT_IsLongKey(pThis, iMsg & 0x0000FFFF) )
    {
        ret = IIMEMT_LongKeyHandle(pThis, iMsg);
    }
    else
    {
        ret = IIMEMT_NormalKeyHandle(pThis, iMsg);
    }

    return ret;
}

static IIME_RESULT IIMEUDMT_TimerHandle(IIMEUDMTProvider *pThis)
{
    int num;
	const unsigned short *pList;
    int iMsg = pThis->m_nPreKey;

    if (IIMEUDMT_IsLongKey(pThis, iMsg))
    {
        pThis->m_nCount++;
    
        if (IIMEUDMT_QueryMultiValues1(pThis, iMsg, &num, &pList))
        {
            if (num > 1)
            {
                pThis->m_wUnderline = pList[(pThis->m_nCount - 1) % num];
                pThis->m_wCommit = 0;

                IIMEMT_FillUnderline(pThis);
            }
            else
            {
                pThis->m_wUnderline = 0;
                pThis->m_wCommit = pList[0];

                IIMEMT_FillCommit(pThis);
            }
        }
    }
    else
    {
	    pThis->m_wCommit = pThis->m_wUnderline;
	    pThis->m_wUnderline = 0;
	    pThis->m_nPreKey = 0;
	    pThis->m_nCount = 0;

        IIMEMT_FillCommit(pThis);
    }
	return IIME_ERROR_USED;
}




