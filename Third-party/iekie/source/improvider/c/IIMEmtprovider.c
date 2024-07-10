/**************************************************************************
 IIMEmtprovider.c

     Description:

     Author:      ke wen (kew@IIME.com.cn)
     Date Time:   12/4/2005 12:33:08 PM

     Copyright (C) 2000-2005 Beijing Chinese Star Cyber Technology Ltd.
     All rights reserved.

 **************************************************************************/
#if 0
#include "IIMEstd.h"
#include "provider_com.h"
#include "IIMEmtprovider.h"
#include "ldbapi4.h"

#if defined(KMX_LDB_TELUGU) || defined(KMX_LDB_MARATHI) || defined(KMX_LDB_TAMIL) || defined(KMX_LDB_PUNJABI) || \
    defined(KMX_LDB_GUJARATI) || defined(LMX_LDB_ORIYA) || defined(KMX_LDB_KANNADA) || \
    defined(KMX_LDB_MALAYALAM) || defined(KMX_LDB_KHMER) || defined(KMX_LDB_LAO) || defined(KMX_LDB_BENGALI)
#include "inc/kmxime.h"

#include "IIMEmtprovider.h"
#include "IIMEpvdlatinfull.h"
#include "indic_mt.h"
#endif
#else
#include "IIMEstd.h"
#include "provider_com.h"
#include "IIMEmtprovider.h"
#include "ldbapi4.h"

#if defined(KMX_LDB_TELUGU) || defined(KMX_LDB_MARATHI) || defined(KMX_LDB_TAMIL) || defined(KMX_LDB_PUNJABI) || \
    defined(KMX_LDB_GUJARATI) || defined(LMX_LDB_ORIYA) || defined(KMX_LDB_KANNADA) || \
    defined(KMX_LDB_MALAYALAM) || defined(KMX_LDB_KHMER) || defined(KMX_LDB_LAO) || defined(KMX_LDB_BENGALI)
#include "kmxime.h"

#include "IIMEmtprovider.h"
#include "IIMEpvdlatinfull.h"
#include "indic_mt.h"
#endif
#endif

#include "IIMEstring.h"

#if defined (KMX_LDB_VIETNAMESE)
    #define IIME_SUPPORT_VIETNAMESE 
#endif

#ifdef KMX_LDB_HANGUL
#define IIME_SUPPORT_HANGUL
#endif

#define IIME_UI_COMMIT_BACKSPACE		0x04000000		/* backspace the last char */

/*
 Declaration
 */

DECLARE_PROVIDER_FUNCTIONS(IIMEMTProvider)

static LDB_CHAR IIMEMT_Key2MIMECHAR(IIME_UINT iMsg);
static IIME_BOOL IIMEMT_QueryMiscDB(IIMEMTProvider* pThis, IIME_UINT iMsg, IIME_INT *pnNumber, const LDB_CHAR **ppList);

static IIME_RESULT IIMEMT_KeyboardHandle(IIMEMTProvider *pThis, IIME_UINT iMsg);
static IIME_RESULT IIMEMT_TimerHandle(IIMEMTProvider *pThis);

static void IIMEMT_FillCommit(IIMEMTProvider *pThis);
static void IIMEMT_FillUnderline(IIMEMTProvider *pThis);

static const LDB_CHAR g_pDigit[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

#if defined(LENOVO_ERROR_STAR)
static const LDB_CHAR g_pZeroError[] = {'0', '+'};
static const LDB_CHAR g_pZero[] = {' '};
#elif defined(LEADCORE_PRODUCT)
static const LDB_CHAR g_pZero[] = {'0', '+', 'P', 'W'};
#else
static const LDB_CHAR g_pZero[] = {' ', '0'};
#endif

#if defined(LEADCORE_PRODUCT)
#if defined(LENOVO_ERROR_STAR)
static const LDB_CHAR g_pStar[] = {'*', 'P','W'};
static const LDB_CHAR g_pPound[] = {'#'};
#else
static const LDB_CHAR g_pStar[] = { '*' }; //{'*','+','P','W'};
static const LDB_CHAR g_pPound[] = {'#'};
#endif
#endif

#ifdef KMX_LDB_ARABIC
static const LDB_CHAR ARABIC_DIGIT[] = 
{
    0x0660, 0x0661, 0x0662, 0x0663, 0x0664, 0x0665, 0x0666, 0x0667, 0x0668, 0x0669
};
#endif

#ifdef KMX_LDB_PERSIAN
static const LDB_CHAR PERSIAN_DIGIT[] =
{
    0x06F0, 0x06F1, 0x06F2, 0x06F3, 0x06F4, 0x06F5, 0x06F6, 0x06F7, 0x06F8, 0x06F9
};
#endif

#if defined(KMX_LDB_HINDI)
static const LDB_CHAR HINDI_DIGIT[] =
{
    0x0966, 0x0967, 0x0968, 0x0969, 0x096A, 0x096B, 0x096C, 0x096D, 0x096E, 0x096F
};
#endif

#if defined(KMX_LDB_NEPALI)//ZHMCH	need 还需要参照HINDI_DIGIT继续copy
static const LDB_CHAR NEPALI_DIGIT[] =
{
    0x0966, 0x0967, 0x0968, 0x0969, 0x096A, 0x096B, 0x096C, 0x096D, 0x096E, 0x096F
};
#endif

#if defined(KMX_LDB_SINHALA)
static const LDB_CHAR SINHALA_DIGIT[] =
{
    0x0966, 0x0967, 0x0968, 0x0969, 0x096A, 0x096B, 0x096C, 0x096D, 0x096E, 0x096F
};
#endif

//daodejing  need
#if defined(KMX_LDB_BODO)
static const LDB_CHAR BODO_DIGIT[] =
{
    0x0966, 0x0967, 0x0968, 0x0969, 0x096A, 0x096B, 0x096C, 0x096D, 0x096E, 0x096F
};
#endif

#if defined(KMX_LDB_DOGRI)
static const LDB_CHAR DOGRI_DIGIT[] =
{
    0x0966, 0x0967, 0x0968, 0x0969, 0x096A, 0x096B, 0x096C, 0x096D, 0x096E, 0x096F
};
#endif

#if defined(KMX_LDB_SANSKRIT)
static const LDB_CHAR SANSKRIT_DIGIT[] =
{
    0x0966, 0x0967, 0x0968, 0x0969, 0x096A, 0x096B, 0x096C, 0x096D, 0x096E, 0x096F
};
#endif

#if defined(KMX_LDB_KASHMIRI)
static const LDB_CHAR KASHMIRI_DIGIT[] =
{
    0x0966, 0x0967, 0x0968, 0x0969, 0x096A, 0x096B, 0x096C, 0x096D, 0x096E, 0x096F
};
#endif

#if defined(KMX_LDB_KONKANI)
static const LDB_CHAR KONKANI_DIGIT[] =
{
    0x0966, 0x0967, 0x0968, 0x0969, 0x096A, 0x096B, 0x096C, 0x096D, 0x096E, 0x096F
};
#endif

#if defined(KMX_LDB_MAITHILI)
static const LDB_CHAR MAITHILI_DIGIT[] =
{
    0x0966, 0x0967, 0x0968, 0x0969, 0x096A, 0x096B, 0x096C, 0x096D, 0x096E, 0x096F
};
#endif

#if defined(KMX_LDB_MANIPURI)
static const LDB_CHAR MANIPURI_DIGIT[] =
{
    0x0966, 0x0967, 0x0968, 0x0969, 0x096A, 0x096B, 0x096C, 0x096D, 0x096E, 0x096F
};
#endif

#if defined(KMX_LDB_SANTALI)
static const LDB_CHAR SANTALI_DIGIT[] =
{
    0x0966, 0x0967, 0x0968, 0x0969, 0x096A, 0x096B, 0x096C, 0x096D, 0x096E, 0x096F
};
#endif

#if defined(KMX_LDB_SOMALI)
static const LDB_CHAR SOMALI_DIGIT[] =
{
    0x0966, 0x0967, 0x0968, 0x0969, 0x096A, 0x096B, 0x096C, 0x096D, 0x096E, 0x096F
};
#endif

#if defined(KMX_LDB_SINDHI)
static const LDB_CHAR SINDHI_DIGIT[] =
{
    0x0966, 0x0967, 0x0968, 0x0969, 0x096A, 0x096B, 0x096C, 0x096D, 0x096E, 0x096F
};
#endif

#if defined(KMX_LDB_BENGALI) || defined(KMX_LDB_ASSAMESE)
static const LDB_CHAR BENGALI_DIGIT[] =
{
    0x09E6, 0x09E7, 0x09E8, 0x09E9, 0x09EA, 0x09EB, 0x09EC, 0x09ED, 0x09EE, 0x09EF
};
#endif

#ifdef KMX_LDB_THAI
static const LDB_CHAR THAI_DIGIT[] =
{
    0x0E50, 0x0E51, 0x0E52, 0x0E53, 0x0E54, 0x0E55, 0x0E56, 0x0E57, 0x0E58, 0x0E59
};
#endif

#ifdef KMX_LDB_TIBETAN
static const LDB_CHAR TIBETAN_DIGIT[] =
{
    0x0F20 ,0x0F21 ,0x0F22 ,0x0F23 ,0x0F24 ,0x0F25 ,0x0F26 ,0x0F27 ,0x0F28 ,0x0F29
};
#endif

#ifdef KMX_LDB_KHMER
static const LDB_CHAR KHMER_DIGIT[] =
{
    0x17E0 ,0x17E1 ,0x17E2 ,0x17E3 ,0x17E4 ,0x17E5 ,0x17E6 ,0x17E7 ,0x17E8 ,0x17E9
};
#endif

static int GetMultitypeEx(
        IN void*                engobj,
		IN const void*          db,
        IN int                  language,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		IN int                  flags,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr);

#ifdef IIME_SUPPORT_VIETNAMESE

static int Vietname_GetMultiTypeEx(
		IN int					iMsg,
		IN int                  flags,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr);

static IIME_BOOL IIMEMT_IsVowelVi(IIME_WCHAR ch);
static IIME_RESULT IIMEMT_VietnameseToneHandle(IIMEMTProvider *pThis, IIME_UINT iMsg);

#endif /* IIME_SUPPORT_VIETNAMESE */

#ifdef IIME_SUPPORT_HANGUL

static int HANGUL_GetMultiTypeEx(
		IN int					iMsg,
		IN IIME_WCHAR             previous_char,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr,
        OUT IIME_BOOL*            has_valid_comp);

static int IIMEMT_HangulTransform(IIMEMTProvider *pThis);

static int IIMEMT_SetHangulCompositon(IIMEMTProvider *pThis, IIME_WCHAR ch);

static int IIMEMT_EndHangulCompostion(IIMEMTProvider *pThis);

static IIME_RESULT IIMEMT_HangulKeyboardHandle(IIMEMTProvider *pThis, IIME_UINT iMsg, IIME_BOOL bLongClick);

#endif /* IIME_SUPPORT_HANGUL */

#ifdef KMX_LDB_TIBETAN
static IIME_RESULT IIMEMT_TibetanConvertionHandle(IIMEMTProvider *pThis, IIME_UINT iMsg);
#endif

#ifdef KMX_LDB_AMHARIC
static IIME_RESULT IIMEMT_AmharicKeyboardHandle(IIMEMTProvider *pThis, IIME_UINT iMsg, IIME_BOOL bLongClick);
#endif


/*
 Constructor an destructor
 */

Provider_Com * IIMEMTProvider_Create(PIIME_IMM_PARAMS  pImmParams,
								   PIIME_USER_PARAMS pUserParam1,
								   PIIME_USER_PARAMS pUserParam2,
								   const void*	  pdbMisc,
							       const void*	  pdbMO,
                                   const IIME_MULTI_STRING_TABLE* pMulti_string_table,
								   IIME_INT idMethod)
{
    IIMEMTProvider *pThis;
    LDB_INFO     dbinfo = {0};
    void*        dbobj = 0;
    int          is_mt_engine_support = 0;
    unsigned int total_object_size;
    
    if (idMethod == IM_THAI_MT || idMethod == IM_HINDI_MT || idMethod == IM_BENGALI_MT || idMethod == IM_TIBETAN_MT)
    {
        is_mt_engine_support = 1;
    }

    total_object_size = sizeof(IIMEMTProvider);

    if (is_mt_engine_support)
    {
        if (!ldbapi4_get_db_info(pdbMisc, &dbinfo))
        {
            /* Error: database not found */
            return 0;
        }
        if (dbinfo.database_id == 9201)
            is_mt_engine_support = 0;
        else
            total_object_size += dbinfo.mt_engine_object_size;
    }    

    pThis = (IIMEMTProvider *)IIME_MALLOC(total_object_size);

    if (!pThis)
    {
        /* Error: not enough memory */
        return 0;
    }

	memset(pThis, 0, sizeof(IIMEMTProvider));
	pThis->m_bFirstChar = IIME_TRUE;
	pThis->m_bCapsLock = IIME_FALSE;

   	/* initialize interface functions*/
	INITIALIZE_PROVIDER_INTERFACE(IIMEMTProvider)

    pThis->m_MagicOne = IIME_MAGICONE_DEFAULT;
    pThis->m_idMethod = idMethod;
    pThis->pdbMisc = pdbMisc;
    pThis->did = *((int*)pdbMisc);

    if (is_mt_engine_support)
    {
        /* Initialize database object */
        dbobj = (char*)pThis + sizeof(IIMEMTProvider);

        if (!ldbapi4_mt_initialize(dbobj, pdbMisc))
        {
            /* Error: invalid database */
            IIME_FREE(pThis);
            return 0;
        }

        pThis->dbobj = dbobj;
        pThis->did   = dbinfo.database_id;
    }

    return (Provider_Com*)pThis;
}

void IIMEMTProvider_Delete(Provider_Com *pThis)
{
    IIMEMTProvider *p = (IIMEMTProvider *)pThis;
    if (p != IIME_NULL)
    {
        if (p->dbobj)
        {
            ldbapi4_mt_cleanup(p->dbobj);
            p->dbobj = 0;
        }
        IIME_FREE(pThis);          
    }       
}

static int PostBengaliChars(IIMEMTProvider *pThis, unsigned short *pBuffer)
{
    int i;
    unsigned short pNewBuffer[16];
    unsigned short *p = pBuffer;

    i = 0;
    memset(pNewBuffer, 0, sizeof(unsigned short) * 16);
    while(*pBuffer)
    {
        switch(*pBuffer)
        {
        case 0x09FC:
            pNewBuffer[i++] = 0x09CD;
            pNewBuffer[i++] = 0x09B0;
            break;
        case 0x09FD:
            pNewBuffer[i++] = 0x09CD;
            pNewBuffer[i++] = 0x09AF;
            break;
        case 0x09FE:
            pNewBuffer[i++] = '\b';
            pNewBuffer[i++] = 0x09B0;
            pNewBuffer[i++] = 0x09CD;
            pNewBuffer[i++] = pThis->m_wPreChar;
            break;
        default:
            pNewBuffer[i++] = *pBuffer;
            break;
        }

        pBuffer++;
    }

    memcpy(p, pNewBuffer, sizeof(unsigned short) * i);
    p[i] = 0;

    return i;
}

/*
 interface
 */

static IIME_RESULT EventHandle_IIMEMTProvider(Provider_Com *pProvider,
                                   IIME_UINT hEvent,
                                   IIME_UINT iMsg,
                                   IIME_IMM_CONTENT *pContent)
{
	IIME_BOOL ret;
	IIMEMTProvider *pThis = (IIMEMTProvider *)pProvider;

	pThis->m_dwUpdateFlag = 0;
    memset(&(pThis->m_immContent), 0, sizeof(IIME_IMM_CONTENT));

    if (pThis->m_pfnGetContext)
    {
        const unsigned short *text;
        int len;
        pThis->m_wPreChar = 0;
        if ((*pThis->m_pfnGetContext)(&text, &len) != 0)
        {
            if (len > 0)
                pThis->m_wPreChar = text[len - 1];
        }
    }
	else if (pContent->iCaretPos == 0)
		pThis->m_wPreChar = 0;
	else
		pThis->m_wPreChar = (IIME_WCHAR)(pContent->iCaretPos);

	if (hEvent == IIME_EVENT_KEYBOARD)
		ret = IIMEMT_KeyboardHandle(pThis, iMsg);
	else if (hEvent == IIME_EVENT_TIMER)
		ret = IIMEMT_TimerHandle(pThis);
	else
		ret = IIME_ERROR_UNKNOWN;

    if (ret != IIME_ERROR_UNUSED)
    {
	    if (pThis->m_dwUpdateFlag & IIME_UI_COMMIT_BACKSPACE){
            pThis->m_immContent.pCommit[0] = 0x08;		/* backspace */
	    }

        if (pThis->m_immContent.nUnderline)
        {
            pThis->m_immContent.flagTimer = IIME_TIMER_SET;
        }
        else
            pThis->m_immContent.flagTimer = IIME_TIMER_CLEAR;
    }

    memcpy(pContent, &(pThis->m_immContent), sizeof(IIME_IMM_CONTENT));

#ifdef KMX_LDB_BENGALI
    if (pThis->did == 9201)
    {
        if (pContent->pUnderlineBuffer[0])
        {
            pContent->nUnderline = (unsigned short) PostBengaliChars(pThis, pContent->pUnderlineBuffer);
        }
        if (pContent->pCommit[0])
        {
            PostBengaliChars(pThis, pContent->pCommit);
        }
    }
#endif

    return ret;
}

static void IIMEMT_FillCommit(IIMEMTProvider *pThis)
{
    IIME_WCHAR *pCommitString = pThis->m_immContent.pCommit;
	IIME_WCHAR wszCommit[2];
	wszCommit[0] = pThis->m_wCommit;
	wszCommit[1] = 0;

	IIMEString_Append(pCommitString, wszCommit);

	if (pThis->m_wCommit){
		if (pThis->m_wCommit == ' ')
			pThis->m_bFirstChar = IIME_TRUE;
		else
			pThis->m_bFirstChar = IIME_FALSE;
	}

}

static void IIMEMT_FillUnderline(IIMEMTProvider *pThis)
{
    IIME_PWSTR pUnderlineString = pThis->m_immContent.pUnderlineBuffer;
    IIME_WORD *pUnderlineStringLen = &(pThis->m_immContent.nUnderline);
    IIME_WORD *pCaretPos = &(pThis->m_immContent.iCaretPos);

	pUnderlineString[0] = pThis->m_wUnderline;
	pUnderlineString[1] = 0;
	*pUnderlineStringLen = 1;

	*pCaretPos = 0;
}

static IIME_RESULT SetOptions_IIMEMTProvider(Provider_Com *pProvider,
                                  const IIME_IMM_OPTIONS *pOption)
{
    IIMEMTProvider *pThis = (IIMEMTProvider *)pProvider;

    if (pOption->dwMagicOne == IIME_MAGICONE_STAR)
        pThis->m_MagicOne = IIME_MAGICONE_STAR;

    if (pOption->pfnGetContext)
        pThis->m_pfnGetContext = pOption->pfnGetContext;

    return IIME_ERROR_SUCCESS;
}

static IIME_RESULT GetOptions_IIMEMTProvider(Provider_Com *pProvider,
                                  IIME_IMM_OPTIONS *pOption)
{
    return IIME_ERROR_SUCCESS;
}

static IIME_RESULT AddPhrase_IIMEMTProvider(Provider_Com* pProvider, unsigned short* szPhrase, int nLen)
{
    return IIME_ERROR_UNKNOWN;
}

static IIME_RESULT CheckAsso_IIMEMTProvider(Provider_Com* pProvider, unsigned short wContext, unsigned short* szAsso, int* pNumber)
{
    return IIME_ERROR_UNKNOWN;
}

static IIME_RESULT QueryProperties_IIMEMTProvider(Provider_Com* pProvider, int* pProperties)
{
    return IIME_ERROR_UNKNOWN;
}
/*
 implemention
 */

static LDB_CHAR IIMEMT_Key2MIMECHAR(IIME_UINT iMsg)
{
    static const LDB_CHAR Key2Char[] = {
        LDB_KEY_0, LDB_KEY_1, LDB_KEY_2, LDB_KEY_3, LDB_KEY_4, LDB_KEY_5, LDB_KEY_6, LDB_KEY_7, LDB_KEY_8, LDB_KEY_9, 
        LDB_KEY_10, LDB_KEY_11, LDB_KEY_12, LDB_KEY_13, LDB_KEY_14, LDB_KEY_15};

    int idx = (iMsg & 0xFFFF) - IIME_VK_0;

    if (iMsg == IIME_VK_STAR)
        return LDB_KEY_STAR;

   if (iMsg == IIME_VK_POUND)
        return LDB_KEY_POUND;
   
    if (idx < sizeof(Key2Char) / sizeof(Key2Char[0]))
        return Key2Char[idx];
    else
        return 0;
}

static int CheckSmartCase(IIMEMTProvider* pThis)
{
    int flag = LDB_UPPER_CASE;

    const unsigned short *text = 0;
    int length = 0;

    IIME_BOOL has_space = 0;
    LDB_CHAR commit_char = 0;

    if ((*(pThis->m_pfnGetContext))(&text, &length) != 0)
    {
        if (pThis->m_immContent.pCommit[0])
        {
            if (pThis->m_immContent.pCommit[0] == ' ')
                has_space = 1;
            else
                commit_char = pThis->m_immContent.pCommit[0];
        }

        if (commit_char == 0)
        {
            while (length > 0 && text[length - 1] == ' ')
            {
                has_space = 1;
                length--;
            }

            if (length > 0 && commit_char == 0)
                commit_char = text[length - 1];
        }
    }
    else if(pThis->m_immContent.pCommit[0])
    {
        if (pThis->m_immContent.pCommit[0] == ' ')
            has_space = 1;
        else
            commit_char = pThis->m_immContent.pCommit[0];
    }

    if (commit_char)
    {
        if (commit_char == 0x0A || commit_char == 0x0D)
        {
            flag = LDB_UPPER_CASE;
        }
        else if (commit_char == '.')
        {
            if (!has_space)
                flag = LDB_LOWER_CASE;
        }
        else
        {
            if (pThis->did)
            {
                if (!ldbapi4_ldb_is_sou_delimiter(pThis->pdbMisc, commit_char))
                    flag = LDB_LOWER_CASE;
            }
        }
    }

    return flag;
}

static IIME_BOOL IIMEMT_QueryMiscDB(IIMEMTProvider* pThis, IIME_UINT iMsg, IIME_INT *pnNumber, const LDB_CHAR **ppList)
{
	int flag;
    LDB_INFO info;

    switch (pThis->m_idMethod)
    {
	case IM_123:
		*pnNumber = 1;
		*(ppList) = g_pDigit + (iMsg - '0');
		return IIME_TRUE;
#ifdef KMX_LDB_ARABIC
    case IM_ARABIC_123:
		*pnNumber = 1;
		*(ppList) = ARABIC_DIGIT + (iMsg - '0');
		return IIME_TRUE;
#endif

#ifdef KMX_LDB_PERSIAN
    case IM_PERSIAN_123:
		*pnNumber = 1;
		*(ppList) = PERSIAN_DIGIT + (iMsg - '0');
		return IIME_TRUE;
#endif

#if defined(KMX_LDB_HINDI)
    case IM_HINDI_123:
		*pnNumber = 1;
		*(ppList) = HINDI_DIGIT + (iMsg - '0');
		return IIME_TRUE;
#endif

#if defined(KMX_LDB_NEPALI)
    case IM_NEPALI_123:
		*pnNumber = 1;
		*(ppList) = NEPALI_DIGIT + (iMsg - '0');
		return IIME_TRUE;
#endif

#if defined(KMX_LDB_SINHALA)
    case IM_SINHALA_123:
		*pnNumber = 1;
		*(ppList) = SINHALA_DIGIT + (iMsg - '0');
		return IIME_TRUE;
#endif

//daodejing  need
#if defined(KMX_LDB_BODO)
    case IM_BODO_123:
		*pnNumber = 1;
		*(ppList) = BODO_DIGIT + (iMsg - '0');
		return IIME_TRUE;
#endif

#if defined(KMX_LDB_DOGRI)
    case IM_DOGRI_123:
		*pnNumber = 1;
		*(ppList) = DOGRI_DIGIT + (iMsg - '0');
		return IIME_TRUE;
#endif

#if defined(KMX_LDB_SANSKRIT)
    case IM_SANSKRIT_123:
		*pnNumber = 1;
		*(ppList) = SANSKRIT_DIGIT + (iMsg - '0');
		return IIME_TRUE;
#endif

#if defined(KMX_LDB_KASHMIRI)
    case IM_KASHMIRI_123:
		*pnNumber = 1;
		*(ppList) = KASHMIRI_DIGIT + (iMsg - '0');
		return IIME_TRUE;
#endif

#if defined(KMX_LDB_KONKANI)
    case IM_KONKANI_123:
		*pnNumber = 1;
		*(ppList) = KONKANI_DIGIT + (iMsg - '0');
		return IIME_TRUE;
#endif

#if defined(KMX_LDB_MAITHILI)
    case IM_MAITHILI_123:
		*pnNumber = 1;
		*(ppList) = MAITHILI_DIGIT + (iMsg - '0');
		return IIME_TRUE;
#endif

#if defined(KMX_LDB_MANIPURI)
    case IM_MANIPURI_123:
		*pnNumber = 1;
		*(ppList) = MANIPURI_DIGIT + (iMsg - '0');
		return IIME_TRUE;
#endif

#if defined(KMX_LDB_SANTALI)
    case IM_SANTALI_123:
		*pnNumber = 1;
		*(ppList) = SANTALI_DIGIT + (iMsg - '0');
		return IIME_TRUE;
#endif

#if defined(KMX_LDB_SOMALI)
    case IM_SOMALI_123:
		*pnNumber = 1;
		*(ppList) = SOMALI_DIGIT + (iMsg - '0');
		return IIME_TRUE;
#endif

#if defined(KMX_LDB_SINDHI)
    case IM_SINDHI_123:
		*pnNumber = 1;
		*(ppList) = SINDHI_DIGIT + (iMsg - '0');
		return IIME_TRUE;
#endif

#if defined(KMX_LDB_BENGALI)
    case IM_BENGALI_123:
    case IM_ASSAMESE_123:
		*pnNumber = 1;
		*(ppList) = BENGALI_DIGIT + (iMsg - '0');
		return IIME_TRUE;
#endif

#ifdef KMX_LDB_THAI
    case IM_THAI_123:
		*pnNumber = 1;
		*(ppList) = THAI_DIGIT + (iMsg - '0');
		return IIME_TRUE;
#endif

#ifdef KMX_LDB_TIBETAN
    case IM_TIBETAN_123:
		*pnNumber = 1;
		*(ppList) = TIBETAN_DIGIT + (iMsg - '0');
		return IIME_TRUE;
#endif

#ifdef KMX_LDB_KHMER
    case IM_KHMER_123:
		*pnNumber = 1;
		*(ppList) = KHMER_DIGIT + (iMsg - '0');
		return IIME_TRUE;
#endif

#if defined(LEADCORE_PRODUCT)
    case IM_NUMBER:
        if (iMsg == IIME_VK_POUND)
        {
            *pnNumber = 1;
            *(ppList) = g_pPound;
        }
        else if (iMsg == IIME_VK_STAR)
        {
            *pnNumber = sizeof(g_pStar) / sizeof(LDB_CHAR);
            *(ppList) = g_pStar;
        }
        else if (iMsg == IIME_VK_0)
        {
#ifdef LENOVO_ERROR_STAR
		    *pnNumber = sizeof(g_pZeroError) / sizeof(LDB_CHAR);
		    *(ppList) = g_pZeroError;
#else
		    *pnNumber = sizeof(g_pZero) / sizeof(LDB_CHAR);
		    *(ppList) = g_pZero;
#endif
        }
        else
        {
		    *pnNumber = 1;
		    *(ppList) = g_pDigit + (iMsg - '0');
        }

        return IIME_TRUE;
#endif
    default:
        if (pThis->m_idMethod % 10 == 2){           /* lower */
    		flag = LDB_LOWER_CASE;
        }
        else if (pThis->m_idMethod % 10 == 3){      /* upper*/
    		flag = LDB_UPPER_CASE;
        }
        else if (pThis->m_idMethod % 10 == 4){      /* lead upper*/
            if (pThis->m_wPreChar)
            {
                if (pThis->m_wPreChar == ' ')
                    flag = LDB_UPPER_CASE;
                else
                    flag = LDB_LOWER_CASE;
            }
		    else if (pThis->m_bFirstChar)
			    flag = LDB_UPPER_CASE;
		    else
			    flag = LDB_LOWER_CASE;
        }
        else if (pThis->m_idMethod % 10 == 5)       /* smart case*/
        {
            if (pThis->m_pfnGetContext)
                flag = CheckSmartCase(pThis);
            else
                flag = LDB_UPPER_CASE;
        }
        else{
            return IIME_FALSE;
        }

    }

#if defined(KMX_LDB_TELUGU)
    if (pThis->m_idMethod == IM_TELUGU_MT)
        return GetMultitype_Telugu(pThis->pdbMisc, pThis->m_wPreChar, iMsg, pnNumber, ppList);
#endif
#if defined(KMX_LDB_MARATHI)
    if (pThis->m_idMethod == IM_MARATHI_MT)
        return GetMultitype_Marathi(pThis->pdbMisc, pThis->m_wPreChar, iMsg, pnNumber, ppList);
#endif
#if  defined(KMX_LDB_TAMIL)
    if (pThis->m_idMethod == IM_TAMIL_MT)
        return GetMultitype_Tamil(pThis->pdbMisc, pThis->m_wPreChar, iMsg, pnNumber, ppList);
#endif
#if  defined(KMX_LDB_PUNJABI)
    if (pThis->m_idMethod == IM_PUNJABI_MT)
        return GetMultitype_Punjabi(pThis->pdbMisc, pThis->m_wPreChar, iMsg, pnNumber, ppList);
#endif
#if  defined(KMX_LDB_GUJARATI)
    if (pThis->m_idMethod == IM_GUJARATI_MT)
        return GetMultitype_Gujarati(pThis->pdbMisc, pThis->m_wPreChar, iMsg, pnNumber, ppList);
#endif
#if  defined(KMX_LDB_ORIYA)
    if (pThis->m_idMethod == IM_ORIYA_MT)
        return GetMultitype_Oriya(pThis->pdbMisc, pThis->m_wPreChar, iMsg, pnNumber, ppList);
#endif
#if  defined(KMX_LDB_KANNADA)
    if (pThis->m_idMethod == IM_KANNADA_MT)
        return GetMultitype_Kannada(pThis->pdbMisc, pThis->m_wPreChar, iMsg, pnNumber, ppList);
#endif
#if  defined(KMX_LDB_MALAYALAM)
    if (pThis->m_idMethod == IM_MALAYALAM_MT)
        return GetMultitype_Malayalam(pThis->pdbMisc, pThis->m_wPreChar, iMsg, pnNumber, ppList);
#endif
#if  defined(KMX_LDB_KHMER)
    if (pThis->m_idMethod == IM_KHMER_MT)
        return GetMultitype_Khmer(pThis->pdbMisc, pThis->m_wPreChar, iMsg, pnNumber, ppList);
#endif

#if  defined(KMX_LDB_LAO)
    if (pThis->m_idMethod == IM_LAO_MT)
        return GetMultitype_Lao(pThis->pdbMisc, pThis->m_wPreChar, iMsg, pnNumber, ppList);
#endif
#if defined(KMX_LDB_BENGALI)
    if (pThis->did == 9201)
        return GetMultitype_Bengali(pThis->pdbMisc, pThis->m_wPreChar, iMsg, pnNumber, ppList);
#endif

#if  defined(KMX_LDB_MYANMAR)
    if (pThis->m_idMethod == IM_MYANMAR_MT)
    {
        LDB_CHAR key = IIMEMT_Key2MIMECHAR(iMsg);
        *ppList = ldbapi4_get_myanmar_multitype(key, pThis->m_wPreChar, pnNumber);
        if (*ppList)
            return 1;
        else
            return 0;
    }
#endif


    if (pThis->did == 0)
        return 0;

    if (ldbapi4_is_indic_dummy_did(pThis->did))
        return 0;

    if (!ldbapi4_get_db_info(pThis->pdbMisc, &info))
        return 0;

#ifdef KEYBOARD_IME_SHARP
    if ( ( info.language != IIME_LANG_THAI && info.language != IIME_LANG_PERSIAN && info.language != IIME_LANG_TIBETAN)
    #if defined(KMX_LDB_ARABIC)
    && ( (info.database_id != 801) && (info.database_id != 802) )        
    #endif
	//daodejing not
    #if defined(KMX_LDB_HINDI)//KMX_LDB_NEPALI	 KMX_LDB_SINHALA     KMX_LDB_DOGRI     KMX_LDB_SANSKRIT     KMX_LDB_KASHMIRI     KMX_LDB_KONKANI     KMX_LDB_MAITHILI     KMX_LDB_MANIPURI     KMX_LDB_SANTALI     KMX_LDB_SOMALI     KMX_LDB_SINDHI
    && ( (info.database_id != 902) && (info.database_id != 909) && (info.database_id != 903) && (info.database_id != 904) && (info.database_id != 9010) && (info.database_id != 9015) && (info.database_id != 9013) && (info.database_id != 9018))
    #endif
    #if defined(KMX_LDB_BENGALI)
    && ( (info.database_id != 923) && (info.database_id != 924) && (info.database_id != 921) && (info.database_id != 928) && (info.database_id != 922) && (info.database_id != 929) && (info.database_id != 9200) && (info.database_id != 9202) && (info.database_id != 9203))
    #endif    
        )
    {
	    if (iMsg == IIME_VK_0)
        {
		    *pnNumber = sizeof(g_pZero) / sizeof(LDB_CHAR);
		    *(ppList) = g_pZero;
            return IIME_TRUE;
	    }
    }
#endif

    return GetMultitypeEx(pThis->dbobj, pThis->pdbMisc, info.language, pThis->m_wPreChar, iMsg, flag, pnNumber, ppList);
}

static IIME_BOOL IIMEMT_IsUsedKey(IIMEMTProvider *pThis, IIME_UINT iMsg)
{
    if ((pThis->m_idMethod == IM_123
        || pThis->m_idMethod == IM_ARABIC_123
        || pThis->m_idMethod == IM_PERSIAN_123
        || pThis->m_idMethod == IM_HINDI_123
        || pThis->m_idMethod == IM_BENGALI_123
        || pThis->m_idMethod == IM_THAI_123
        || pThis->m_idMethod == IM_ASSAMESE_123
        || pThis->m_idMethod == IM_NEPALI_123
        || pThis->m_idMethod == IM_SINHALA_123
        || pThis->m_idMethod == IM_BODO_123
        || pThis->m_idMethod == IM_DOGRI_123
        || pThis->m_idMethod == IM_SANSKRIT_123
        || pThis->m_idMethod == IM_KASHMIRI_123
        || pThis->m_idMethod == IM_KONKANI_123
        || pThis->m_idMethod == IM_MAITHILI_123
        || pThis->m_idMethod == IM_MANIPURI_123
        || pThis->m_idMethod == IM_SANTALI_123
        || pThis->m_idMethod == IM_SOMALI_123
        || pThis->m_idMethod == IM_SINDHI_123
        ) && (iMsg == IIME_VK_POUND || iMsg == IIME_VK_STAR))//daodejing
    {
        return IIME_FALSE;
    }

    if (iMsg >= IIME_VK_0 && iMsg <= IIME_VK_R3)
    {
        return IIME_TRUE;
    }

    if (pThis->did > 0 && ldbapi4_is_key_star_used(pThis->pdbMisc) && iMsg == IIME_VK_STAR)
    {
        return IIME_TRUE;
    }

    if (pThis->did > 0 && ldbapi4_is_key_pound_used(pThis->pdbMisc) && iMsg == IIME_VK_POUND)
    {
        return IIME_TRUE;
    }
    if (pThis->m_MagicOne == IIME_MAGICONE_STAR && iMsg == IIME_VK_STAR)
    {
        if (pThis->m_idMethod != IM_123
            && pThis->m_idMethod != IM_ARABIC_123
            && pThis->m_idMethod != IM_PERSIAN_123
            && pThis->m_idMethod != IM_HINDI_123
            && pThis->m_idMethod != IM_BENGALI_123
            && pThis->m_idMethod != IM_THAI_123
            && pThis->m_idMethod != IM_ASSAMESE_123
            && pThis->m_idMethod != IM_NEPALI_123
            && pThis->m_idMethod != IM_SINHALA_123
            && pThis->m_idMethod != IM_BODO_123
            && pThis->m_idMethod != IM_DOGRI_123
            && pThis->m_idMethod != IM_SANSKRIT_123
            && pThis->m_idMethod != IM_KASHMIRI_123
            && pThis->m_idMethod != IM_KONKANI_123
            && pThis->m_idMethod != IM_MAITHILI_123
            && pThis->m_idMethod != IM_MANIPURI_123
            && pThis->m_idMethod != IM_SANTALI_123
            && pThis->m_idMethod != IM_SOMALI_123
            && pThis->m_idMethod != IM_SINDHI_123
            )//daodejing
            return IIME_TRUE;
    }

    if (pThis->m_idMethod == IM_NUMBER && (iMsg == IIME_VK_STAR || iMsg == IIME_VK_POUND))
    {
        return IIME_TRUE;
    }

    if (pThis->m_idMethod == IM_MYANMAR_MT && iMsg == IIME_VK_STAR)
    {
        return IIME_TRUE;
    }

    return IIME_FALSE;

}

static IIME_RESULT IIMEMT_KeyboardHandle(IIMEMTProvider *pThis, IIME_UINT iMsg)
{
	IIME_RESULT ret;
	IIME_BOOL bLongClick = iMsg & IIME_VK_LONGCLICK;

	iMsg &= ~IIME_VK_LONGCLICK;

#ifdef IIME_SUPPORT_HANGUL
    if (pThis->m_idMethod == IM_HANGUL_MT)
        return IIMEMT_HangulKeyboardHandle(pThis, iMsg, bLongClick);
#endif /* IIME_SUPPORT_HANGUL */

#ifdef KMX_LDB_AMHARIC
    if (pThis->m_idMethod == IM_AMHARIC_MT)
        return IIMEMT_AmharicKeyboardHandle(pThis, iMsg, bLongClick);
#endif

#ifdef IIME_SUPPORT_VIETNAMESE
	/* Vietnamese tone handle */
	if (pThis->m_idMethod == IM_ABC_LOWER_VI || pThis->m_idMethod == IM_ABC_UPPER_VI || pThis->m_idMethod == IM_ABC_LEAD_UPPER_VI){
		if (iMsg == IIME_VK_STAR){
			if (IIMEMT_VietnameseToneHandle(pThis, iMsg) == IIME_ERROR_USED)
				return IIME_ERROR_USED;
            else
            {
		        if (pThis->m_nPreKey){
			        pThis->m_wCommit = pThis->m_wUnderline;
			        pThis->m_wUnderline = 0;
			        pThis->m_nCount = 0;
                    IIMEMT_FillCommit(pThis);
                }
                return IIME_ERROR_UNUSED;
            }
		}
	}
#endif

#ifdef KMX_LDB_TIBETAN
    if (pThis->m_idMethod == IM_TIBETAN_MT)
    {
        if (IIMEMT_TibetanConvertionHandle(pThis, iMsg) == IIME_ERROR_USED)
            return IIME_ERROR_USED;
    }
#endif

    if (!IIMEMT_IsUsedKey(pThis, iMsg))
    {
		ret = IIME_ERROR_UNUSED;

		if (pThis->m_nPreKey){
			pThis->m_wCommit = pThis->m_wUnderline;
			pThis->m_wUnderline = 0;
			pThis->m_nCount = 0;

            IIMEMT_FillCommit(pThis);

#ifdef TECHFAITH_PRODUCT
			if (iMsg == IIME_VK_LEFT || iMsg == IIME_VK_RIGHT || iMsg == IIME_VK_UP || iMsg == IIME_VK_DOWN || iMsg == IIME_VK_SELECT)
			{
				pThis->m_nPreKey = 0;
				ret = IIME_ERROR_USED;
			}
#else
			if (iMsg == IIME_VK_SELECT)
			{
				pThis->m_nPreKey = 0;
				ret = IIME_ERROR_USED;
			}
#endif
		}
	}
	else if (iMsg == pThis->m_nPreKey){	/* same key as before*/
		int i, j, num;
		const LDB_CHAR *pList;
        LDB_CHAR pMultiList[64];

		if (bLongClick){
			pThis->m_nCount = 0;
			pThis->m_nPreKey = 0;

			pThis->m_wUnderline = 0;
            if (iMsg == IIME_VK_STAR)
            {
                pThis->m_wCommit = '*';                
            }
            else
            {
                pThis->m_wCommit = g_pDigit[iMsg - '0'];
            }
			
            IIMEMT_FillCommit(pThis);
			ret = IIME_ERROR_USED;
		}
		else{
			pThis->m_nCount++;
			
			if(IIMEMT_QueryMiscDB(pThis, iMsg, &num, &pList)){
                j = 0;
                               
                for (i = 0; i < num; i++)
                {
                    if (pThis->m_MagicOne == IIME_MAGICONE_STAR && iMsg == IIME_VK_STAR && pList[i] == '1')
                    {
                    }
                    else
                    {
                        pMultiList[j] = pList[i];
                        j++;
                    }
                }

                #if defined(KMX_LDB_THAI) && defined(KMX_IME_THAI_KEY0_SPACE_MULTITAP)
                if (pThis->did == 360 || pThis->did == 361 || pThis->did == 365  || pThis->did == 367)
                {
                    if (iMsg == IIME_VK_0)
                    {
                        LDB_CHAR last;
                        /* Add space before last */
                        last = pMultiList[j - 1];
                        pMultiList[j - 1] = 0x20;
                        pMultiList[j] = last;                        
                        j++;
                    }
                }
                #endif

                num = j;

				if (num > 1){
					pThis->m_wUnderline = pMultiList[(pThis->m_nCount - 1) % num];
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
			ret = IIME_ERROR_USED;
		}
	}
	else{
		int i, j, num;
		const LDB_CHAR *pList;
        LDB_CHAR pMultiList[64];

		if (pThis->m_nPreKey){
            pThis->m_wCommit = pThis->m_wUnderline;
            
            if (pThis->m_wUnderline != 0x09FE)
                pThis->m_wPreChar = pThis->m_wUnderline;
            
            IIMEMT_FillCommit(pThis);

			if (pThis->m_wCommit){
				if (pThis->m_wCommit == ' ')
					pThis->m_bFirstChar = IIME_TRUE;
				else
					pThis->m_bFirstChar = IIME_FALSE;
			}
		}

		if (bLongClick){
			pThis->m_nPreKey = 0;
			pThis->m_nCount = 0;

			pThis->m_wUnderline = 0;
			if (iMsg == IIME_VK_STAR)
            {
                pThis->m_wCommit = '*';                
            }
            else
            {
                pThis->m_wCommit = g_pDigit[iMsg - '0'];
            }

            IIMEMT_FillCommit(pThis);
			/*pThis->m_dwUpdateFlag = IIME_UI_COMMIT;*/
			ret = IIME_ERROR_USED;
		}
		else{
			pThis->m_nCount = 1;
			pThis->m_nPreKey = iMsg;
			
			if (IIMEMT_QueryMiscDB(pThis, iMsg, &num, &pList)){
                j = 0;
                for (i = 0; i < num; i++)
                {
                    if (pThis->m_MagicOne == IIME_MAGICONE_STAR && iMsg == IIME_VK_STAR && pList[i] == '1')
                    {
                    }
                    else
                    {
                        pMultiList[j] = pList[i];
                        j++;
                    }
                }
                #if defined(KMX_LDB_THAI) && defined(KMX_IME_THAI_KEY0_SPACE_MULTITAP)
                if (pThis->did == 360 || pThis->did == 361 || pThis->did == 365 || pThis->did == 367)
                {
                    if (iMsg == IIME_VK_0)
                    {
                        LDB_CHAR last;
                        /* Add space before last */
                        last = pMultiList[j - 1];
                        pMultiList[j - 1] = 0x20;
                        pMultiList[j] = last;
                        j++;
                    }
                }
                #endif

                num = j;

                if (num > 1){
					pThis->m_wUnderline = pMultiList[(pThis->m_nCount - 1) % num];

                    IIMEMT_FillUnderline(pThis);
					/*pThis->m_dwUpdateFlag |= IIME_UI_COMMIT_UNDERLINE;*/
				}
				else{
					pThis->m_wUnderline = 0;
					pThis->m_wCommit = pList[0];

                    IIMEMT_FillCommit(pThis);
					/*pThis->m_dwUpdateFlag = IIME_UI_COMMIT;*/

					pThis->m_nPreKey = 0;
					pThis->m_nCount = 0;
				}
			}

			/*pThis->m_nPreKey = iMsg;*/
		}
		ret = IIME_ERROR_USED;
	}

	return ret;
}

static IIME_RESULT IIMEMT_TimerHandle(IIMEMTProvider *pThis)
{
	pThis->m_wCommit = pThis->m_wUnderline;
	pThis->m_wUnderline = 0;
	pThis->m_nPreKey = 0;
	pThis->m_nCount = 0;

    IIMEMT_FillCommit(pThis);
	/*pThis->m_dwUpdateFlag = IIME_UI_COMMIT;*/

	return IIME_ERROR_USED;
}

static int GetMultitypeEx(
        IN void*                engobj,
		IN const void*          db,
        IN int                  language,
        IN LDB_CHAR             previous_char,
		IN int					iMsg,
		IN int                  flags,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr)
{
	if (language == IIME_LANG_VIETNAMESE)
    {
#ifdef IIME_SUPPORT_VIETNAMESE
		return Vietname_GetMultiTypeEx(iMsg, flags, count_ptr, char_list_ptr);
#else
		return 0;
#endif
	}
    else if (language == IIME_LANG_THAI)
    {
		LDB_CHAR key = IIMEMT_Key2MIMECHAR(iMsg);
        *char_list_ptr = ldbapi4_get_thai_multitype(engobj, key, previous_char, count_ptr);
        if (*char_list_ptr)
            return 1;
        else
            return 0;
    }
    else if (language == IIME_LANG_HINDI)
    {
		LDB_CHAR key = IIMEMT_Key2MIMECHAR(iMsg);
        *char_list_ptr = ldbapi4_get_hindi_multitype(engobj, key, previous_char, count_ptr);
        if (*char_list_ptr)
            return 1;
        else
            return 0;
    }
	//IM_NEPALI_SUPPORT
	else if (language == IIME_LANG_NEPALI)
    {
		LDB_CHAR key = IIMEMT_Key2MIMECHAR(iMsg);
        *char_list_ptr = ldbapi4_get_nepali_multitype(engobj, key, previous_char, count_ptr);
        if (*char_list_ptr)
            return 1;
        else
            return 0;
    }
	//IM_SINHALA_SUPPORT
	else if (language == IIME_LANG_SINHALA)
    {
		LDB_CHAR key = IIMEMT_Key2MIMECHAR(iMsg);
        *char_list_ptr = ldbapi4_get_sinhala_multitype(engobj, key, previous_char, count_ptr);
        if (*char_list_ptr)
            return 1;
        else
            return 0;
    }
	//daodejing
	else if (language == IIME_LANG_BODO)
    {
		LDB_CHAR key = IIMEMT_Key2MIMECHAR(iMsg);
        *char_list_ptr = ldbapi4_get_bodo_multitype(engobj, key, previous_char, count_ptr);
        if (*char_list_ptr)
            return 1;
        else
            return 0;
    }
	else if (language == IIME_LANG_DOGRI)
    {
		LDB_CHAR key = IIMEMT_Key2MIMECHAR(iMsg);
        *char_list_ptr = ldbapi4_get_dogri_multitype(engobj, key, previous_char, count_ptr);
        if (*char_list_ptr)
            return 1;
        else
            return 0;
    }
	else if (language == IIME_LANG_SANSKRIT)
    {
		LDB_CHAR key = IIMEMT_Key2MIMECHAR(iMsg);
        *char_list_ptr = ldbapi4_get_sanskrit_multitype(engobj, key, previous_char, count_ptr);
        if (*char_list_ptr)
            return 1;
        else
            return 0;
    }
	else if (language == IIME_LANG_KASHMIRI)
    {
		LDB_CHAR key = IIMEMT_Key2MIMECHAR(iMsg);
        *char_list_ptr = ldbapi4_get_kashmiri_multitype(engobj, key, previous_char, count_ptr);
        if (*char_list_ptr)
            return 1;
        else
            return 0;
    }
	else if (language == IIME_LANG_KONKANI)
    {
		LDB_CHAR key = IIMEMT_Key2MIMECHAR(iMsg);
        *char_list_ptr = ldbapi4_get_konkani_multitype(engobj, key, previous_char, count_ptr);
        if (*char_list_ptr)
            return 1;
        else
            return 0;
    }
	else if (language == IIME_LANG_MAITHILI)
    {
		LDB_CHAR key = IIMEMT_Key2MIMECHAR(iMsg);
        *char_list_ptr = ldbapi4_get_maithili_multitype(engobj, key, previous_char, count_ptr);
        if (*char_list_ptr)
            return 1;
        else
            return 0;
    }
	else if (language == IIME_LANG_MANIPURI)
    {
		LDB_CHAR key = IIMEMT_Key2MIMECHAR(iMsg);
        *char_list_ptr = ldbapi4_get_manipuri_multitype(engobj, key, previous_char, count_ptr);
        if (*char_list_ptr)
            return 1;
        else
            return 0;
    }
	else if (language == IIME_LANG_SANTALI)
    {
		LDB_CHAR key = IIMEMT_Key2MIMECHAR(iMsg);
        *char_list_ptr = ldbapi4_get_santali_multitype(engobj, key, previous_char, count_ptr);
        if (*char_list_ptr)
            return 1;
        else
            return 0;
    }
	else if (language == IIME_LANG_SOMALI)
    {
		LDB_CHAR key = IIMEMT_Key2MIMECHAR(iMsg);
        *char_list_ptr = ldbapi4_get_somali_multitype(engobj, key, previous_char, count_ptr);
        if (*char_list_ptr)
            return 1;
        else
            return 0;
    }
	else if (language == IIME_LANG_SINDHI)
    {
		LDB_CHAR key = IIMEMT_Key2MIMECHAR(iMsg);
        *char_list_ptr = ldbapi4_get_sindhi_multitype(engobj, key, previous_char, count_ptr);
        if (*char_list_ptr)
            return 1;
        else
            return 0;
    }
    else if (language == IIME_LANG_BENGALI)
    {
		LDB_CHAR key = IIMEMT_Key2MIMECHAR(iMsg);
        *char_list_ptr = ldbapi4_get_bengali_multitype(engobj, key, previous_char, count_ptr);
        if (*char_list_ptr)
            return 1;
        else
            return 0;
    }
    else if (language == IIME_LANG_TIBETAN)
    {
		LDB_CHAR key = IIMEMT_Key2MIMECHAR(iMsg);
        *char_list_ptr = ldbapi4_get_tibetan_multitype(engobj, key, previous_char, count_ptr);
        if (*char_list_ptr)
            return 1;
        else
            return 0;
    }    
	else	
    {
		LDB_CHAR key;
        if (iMsg == IIME_VK_STAR)
            iMsg = IIME_VK_1;
		
        key = IIMEMT_Key2MIMECHAR(iMsg);

        *char_list_ptr = ldbapi4_get_db_multitype(db, key, flags, count_ptr);
        if (char_list_ptr)
            return 1;
        else
            return 0;
	}
}

#ifdef IIME_SUPPORT_VIETNAMESE

static const int IIME_vietnamese_keymap_number[9] = {13, 6, 6, 4, 4, 6, 5, 5, 5};

static const IIME_WCHAR IIME_vietnamese_keymap_1[] = {
#ifdef SPREADTONE_SPECIAL
    0x002E, 0x002C, 0x003F, 0x0021, 0x002D, 0x0027, 0x0022, 0x0028, 0x0029, 0x0040, 0x002F, 0x003A
#else
    0x002E, 0x002C, 0x003F, 0x0021, 0x002D, 0x0027, 0x0022, 0x0031, 0x0028, 0x0029, 0x0040, 0x002F, 0x003A
#endif
};

static const IIME_WCHAR IIME_vietnamese_keymap_lower[8][6] = {
	{0x0061, 0x0103, 0x00E2, 0x0062, 0x0063, 0x0032},

	{0x0064, 0x0111, 0x0065, 0x00EA, 0x0066, 0x0033},

	{0x0067, 0x0068, 0x0069, 0x0034, 0x0000, 0x0000},

	{0x006A, 0x006B, 0x006C, 0x0035, 0x0000, 0x0000},

	{0x006D, 0x006E, 0x006F, 0x00F4, 0x01A1, 0x0036},

	{0x0070, 0x0071, 0x0072, 0x0073, 0x0037, 0x0000},

	{0x0074, 0x0075, 0x01B0, 0x0076, 0x0038, 0x0000},

	{0x0077, 0x0078, 0x0079, 0x007A, 0x0039, 0x0000}
};

static const IIME_WCHAR IIME_vietnamese_keymap_upper[8][6] = {
	{0x0041, 0x0102, 0x00C2, 0x0042, 0x0043, 0x0032},

	{0x0044, 0x0110, 0x0045, 0x00CA, 0x0046, 0x0033},

	{0x0047, 0x0048, 0x0049, 0x0034, 0x0000, 0x0000},

	{0x004A, 0x004B, 0x004C, 0x0035, 0x0000, 0x0000},

	{0x004D, 0x004E, 0x004F, 0x00D4, 0x01A0, 0x0036},

	{0x0050, 0x0051, 0x0052, 0x0053, 0x0037, 0x0000},

	{0x0054, 0x0055, 0x01AF, 0x0056, 0x0038, 0x0000},

	{0x0057, 0x0058, 0x0059, 0x005A, 0x0039, 0x0000}
};

static const IIME_WCHAR IIME_vietnames_tonemap[24][6] = {
	{0x0061, 0x00E0, 0x00E1, 0x1EA3, 0x00E3, 0x1EA1},
	{0x0041, 0x00C0, 0x00C1, 0x1EA2, 0x00C3, 0x1EA0},

	{0x0103, 0x1EB1, 0x1EAF, 0x1EB3, 0x1EB5, 0x1EB7},
	{0x0102, 0x1EB0, 0x1EAE, 0x1EB2, 0x1EB4, 0x1EB6},

	{0x00E2, 0x1EA7, 0x1EA5, 0x1EA9, 0x1EAB, 0x1EAD},
	{0x00C2, 0x1EA6, 0x1EA4, 0x1EA8, 0x1EAA, 0x1EAC},

	{0x0065, 0x00E8, 0x00E9, 0x1EBB, 0x1EBD, 0x1EB9},
	{0x0045, 0x00C8, 0x00C9, 0x1EBA, 0x1EBC, 0x1EB8},

	{0x00EA, 0x1EC1, 0x1EBF, 0x1EC3, 0x1EC5, 0x1EC7},
	{0x00CA, 0x1EC0, 0x1EBE, 0x1EC2, 0x1EC4, 0x1EC6},

	{0x0069, 0x00EC, 0x00ED, 0x1EC9, 0x0129, 0x1ECB},
	{0x0049, 0x00CC, 0x00CD, 0x1EC8, 0x0128, 0x1ECA},

	{0x006F, 0x00F2, 0x00F3, 0x1ECF, 0x00F5, 0x1ECD},
	{0x004F, 0x00D2, 0x00D3, 0x1ECE, 0x00D5, 0x1ECC},

	{0x00F4, 0x1ED3, 0x1ED1, 0x1ED5, 0x1ED7, 0x1ED9},
	{0x00D4, 0x1ED2, 0x1ED0, 0x1ED4, 0x1ED6, 0x1ED8},

	{0x01A1, 0x1EDD, 0x1EDB, 0x1EDF, 0x1EE1, 0x1EE3},
	{0x01A0, 0x1EDC, 0x1EDA, 0x1EDE, 0x1EE0, 0x1EE2},

	{0x0075, 0x00F9, 0x00FA, 0x1EE7, 0x0169, 0x1EE5},
	{0x0055, 0x00D9, 0x00DA, 0x1EE6, 0x0168, 0x1EE4},

	{0x01B0, 0x1EEB, 0x1EE9, 0x1EED, 0x1EEF, 0x1EF1},
	{0x01AF, 0x1EEA, 0x1EE8, 0x1EEC, 0x1EEE, 0x1EF0},
	
	{0x0079, 0x1EF3, 0x00FD, 0x1EF7, 0x1EF9, 0x1EF5},
	{0x0059, 0x1EF2, 0x00DD, 0x1EF6, 0x1EF8, 0x1EF4}
};

static int Vietname_GetMultiTypeEx(
		IN int					iMsg,
		IN int                  flags,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr)
{
	IIME_ASSERT(iMsg >= IIME_VK_1 && iMsg <= IIME_VK_9);
	*count_ptr = IIME_vietnamese_keymap_number[iMsg - IIME_VK_1];

	if (iMsg == IIME_VK_1)
		*char_list_ptr = IIME_vietnamese_keymap_1;
	else if (flags == LDB_UPPER_CASE){
		*char_list_ptr = IIME_vietnamese_keymap_upper[iMsg - IIME_VK_2];
	}
	else if (flags == LDB_LOWER_CASE){
		*char_list_ptr = IIME_vietnamese_keymap_lower[iMsg - IIME_VK_2];
	}

	return 1;
}

static IIME_BOOL IIMEMT_IsVowelVi(IIME_WCHAR ch)
{
	int i;
	for (i = 0; i < sizeof(IIME_vietnames_tonemap) / sizeof(IIME_vietnames_tonemap[0]); i++){
		const IIME_WCHAR *p = IIME_vietnames_tonemap[i];
		if (ch == p[0])
			return IIME_TRUE;
	}

	return IIME_FALSE;
}

static IIME_WCHAR IIMEMT_GetVowelTone(IIME_WCHAR ch, int count)
{
	int i;
	for (i = 0; i < sizeof(IIME_vietnames_tonemap) / sizeof(IIME_vietnames_tonemap[0]); i++){

		if (IIME_vietnames_tonemap[i][0] == ch){
			const IIME_WCHAR *p = IIME_vietnames_tonemap[i] + 1;		/* skip the first */
			int num = sizeof(IIME_vietnames_tonemap[i]) / sizeof(IIME_vietnames_tonemap[i][0]) - 1;

			return p[(count - 1) % num];
		}
		
	}

	return 0;
}

static IIME_RESULT IIMEMT_VietnameseToneHandle(IIMEMTProvider *pThis, IIME_UINT iMsg)
{
	IIME_WCHAR wchHeader = 0;

	if (pThis->m_nPreKey == 0){
		wchHeader = pThis->m_wPreChar;
		pThis->m_dwUpdateFlag |= IIME_UI_COMMIT_BACKSPACE;
	}
	else if (pThis->m_nPreKey >> 16 == 0){
		wchHeader = pThis->m_wUnderline;
		pThis->m_nCount = 0;
	}
	else{
		wchHeader = (IIME_WCHAR)(pThis->m_nPreKey >> 16);
	}

	if (IIMEMT_IsVowelVi(wchHeader)){
		pThis->m_nPreKey = wchHeader << 16;
		pThis->m_nCount++;

		pThis->m_wUnderline = IIMEMT_GetVowelTone(wchHeader, pThis->m_nCount);

        IIMEMT_FillUnderline(pThis);
		/*pThis->m_dwUpdateFlag |= IIME_UI_COMMIT_UNDERLINE;*/

		return IIME_ERROR_USED;
	}
	else{
		pThis->m_dwUpdateFlag &= ~IIME_UI_COMMIT_BACKSPACE;
		return IIME_ERROR_UNUSED;
	}
}

#endif /* IIME_SUPPORT_VIETNAMESE */


#ifdef IIME_SUPPORT_HANGUL

static int IIMEMT_HangulTransform(IIMEMTProvider *pThis)
{
    int num;
    const LDB_CHAR *pList;
    IIME_BOOL has_valid_comp;
    int ret;

    ret = IIME_ERROR_UNUSED;

    if (HANGUL_GetMultiTypeEx(IIME_VK_STAR, pThis->m_wUnderline, &num, &pList, &has_valid_comp))
    {
        if (has_valid_comp)
        {
            IIMEMT_SetHangulCompositon(pThis, pList[0]);
            ret = IIME_ERROR_USED;
        }
    }
    
    return ret;
}

static int IIMEMT_SetHangulCompositon(IIMEMTProvider *pThis, IIME_WCHAR ch)
{
    pThis->m_wUnderline = ch;
    pThis->m_wCommit = 0;
    IIMEMT_FillUnderline(pThis);
    return 0;
}

static int IIMEMT_EndHangulCompostion(IIMEMTProvider *pThis)
{
    pThis->m_wPreComp = 0;
    if (pThis->m_wUnderline)
    {
        pThis->m_wCommit = pThis->m_wUnderline;
        pThis->m_wUnderline = 0;    
        IIMEMT_FillCommit(pThis);    
    }    
    return 0;
}

static int HANGUL_GetMultiTypeEx(
		IN int					iMsg,
		IN IIME_WCHAR             previous_char,
		OUT int*                count_ptr,
		OUT const LDB_CHAR**    char_list_ptr,
        OUT IIME_BOOL*            has_valid_comp)
{
    LDB_CHAR key = IIMEMT_Key2MIMECHAR(iMsg);
    *char_list_ptr = ldbapi4_get_hangul_multitype(key, previous_char, count_ptr, has_valid_comp);
    if (*char_list_ptr)
        return 1;
    else
        return 0;
}

static int HANGUL_GetMultiTypeComp(
        IN int					iMsg,
		IN IIME_WCHAR             previous_char,
		OUT IIME_WCHAR*           commit_char,
		OUT IIME_WCHAR*           comp_char,
        OUT IIME_WCHAR*           prev_comp_char)
{
    LDB_CHAR key = IIMEMT_Key2MIMECHAR(iMsg);
    /* try to use the T part of previous char and the new input to compose 2 syllabe */
    return ldbapi4_get_hangul_multitype_ex(key, previous_char, commit_char, comp_char, prev_comp_char);
}

static IIME_RESULT IIMEMT_HangulKeyboardHandle(IIMEMTProvider *pThis, IIME_UINT iMsg, IIME_BOOL bLongClick)
{
    IIME_RESULT ret;
    IIME_BOOL   has_valid_comp;
	iMsg &= ~IIME_VK_LONGCLICK;

	if ((iMsg < IIME_VK_0 || iMsg > IIME_VK_R3)){	/* out of range[IIME_VK_0, IIME_VK_9]*/	
		ret = IIME_ERROR_UNUSED;

        if (iMsg == IIME_VK_STAR)
        {
            if (IIMEMT_HangulTransform(pThis) == IIME_ERROR_USED)
                return IIME_ERROR_USED;                
        }

		if (pThis->m_nPreKey){
			pThis->m_wCommit = pThis->m_wUnderline;
			pThis->m_wUnderline = 0;
			pThis->m_nCount = 0;

            IIMEMT_FillCommit(pThis);

            IIMEMT_EndHangulCompostion(pThis);

#ifdef TECHFAITH_PRODUCT
			if (iMsg == IIME_VK_LEFT || iMsg == IIME_VK_RIGHT || iMsg == IIME_VK_UP || iMsg == IIME_VK_DOWN || iMsg == IIME_VK_SELECT)
			{
				pThis->m_nPreKey = 0;
				ret = IIME_ERROR_USED;
			}
#else
			if (iMsg == IIME_VK_SELECT)
			{
				pThis->m_nPreKey = 0;
				ret = IIME_ERROR_USED;

			}
#endif
		}		
	}
	else if (iMsg == pThis->m_nPreKey){	/* same key as before*/
		int num;
		const LDB_CHAR *pList;

		if (bLongClick){
			pThis->m_nCount = 0;
			pThis->m_nPreKey = 0;

			pThis->m_wUnderline = 0;
			pThis->m_wCommit = g_pDigit[iMsg - '0'];
			
            IIMEMT_FillCommit(pThis);
			ret = IIME_ERROR_USED;
		}
		else{
			pThis->m_nCount++;
			
			if(HANGUL_GetMultiTypeEx(iMsg, pThis->m_wPreComp, &num, &pList, &has_valid_comp)){
				if (num > 1){
					pThis->m_wUnderline = pList[(pThis->m_nCount - 1) % num];
					pThis->m_wCommit = 0;                    
                    IIMEMT_FillUnderline(pThis);
				}
				else{
					pThis->m_wUnderline = 0;
                    pThis->m_wPreComp   = 0;
					pThis->m_wCommit = pList[0];
                    IIMEMT_FillCommit(pThis);
                    /* Set new underline */
                    if(HANGUL_GetMultiTypeEx(iMsg, pThis->m_wPreComp, &num, &pList, &has_valid_comp))
                    {
                        pThis->m_nCount = 1;
                        pThis->m_wUnderline = pList[(pThis->m_nCount - 1) % num];
                        IIMEMT_FillUnderline(pThis);
                    }
				}
			}

			pThis->m_nPreKey = iMsg;
			ret = IIME_ERROR_USED;
		}
	}
	else{
		int num;
		const LDB_CHAR *pList;
		
		if (bLongClick){
			pThis->m_nPreKey = 0;
			pThis->m_nCount = 0;

			pThis->m_wUnderline = 0;
			pThis->m_wCommit = g_pDigit[iMsg - '0'];

            IIMEMT_FillCommit(pThis);			
			ret = IIME_ERROR_USED;
		}
		else{
			pThis->m_nCount = 1;
			pThis->m_nPreKey = iMsg;
			
            if (iMsg == IIME_VK_0)
            {
                IIMEMT_EndHangulCompostion(pThis);             
				pThis->m_wCommit = ' ';

                IIMEMT_FillCommit(pThis);				

				pThis->m_nPreKey = 0;
				pThis->m_nCount = 0;
                
            }
			else if (HANGUL_GetMultiTypeEx(iMsg, pThis->m_wUnderline, &num, &pList, &has_valid_comp))
			{			
                if (!has_valid_comp)
                {
                    if (HANGUL_GetMultiTypeComp(iMsg, pThis->m_wUnderline, &pThis->m_wCommit, &pThis->m_wUnderline,  &pThis->m_wPreComp))
                    {                        
                        IIMEMT_FillCommit(pThis);
                        IIMEMT_FillUnderline(pThis);
                        return IIME_ERROR_USED;
                    }
                    
                    pThis->m_wCommit = pThis->m_wUnderline;
                    IIMEMT_FillCommit(pThis);
                }
                pThis->m_wPreComp = pThis->m_wUnderline; 
				pThis->m_wUnderline = pList[(pThis->m_nCount - 1) % num];
                IIMEMT_FillUnderline(pThis);                    
			}
		}
		ret = IIME_ERROR_USED;
	}

	return ret;
}

#endif /* IIME_SUPPORT_HANGUL */

#ifdef KMX_LDB_AMHARIC

/*
 *  Amharic 
 *              key1: 0x1200, 0x1208, 0x1210, 0x1218, 0x1220
 *              key2: 0x1228, 0x1230, 0x1238, 0x1240, 0x1248
 *              key3: 0x1250, 0x1258, 0x1260, 0x1268, 0x1270
 *              key4: 0x1278, 0x1280, 0x1288, 0x1290, 0x1298
 *              key5: 0x12A0, 0x12A8, 0x12B0, 0x12B8, 0x12C0
 *              key6: 0x12C8, 0x12D0, 0x12D8, 0x12E0, 0x12E8
 *              key7: 0x12F0, 0x12F8, 0x1300, 0x1308, 0x1310
 *              key8: 0x1318, 0x1320, 0x1328, 0x1330
 *              key9: 0x1338, 0x1340, 0x1348, 0x1350
                    
 *              key_star: vowel
 *              key_0: Zero, Space, [0x135F, 0x1368]
 */

static LDB_CHAR Amharic_skipped_char[] = 
{
    0x1249, 0x124E, 0x124F,
    0x1257, 0x1259, 0x125E, 0x125F,
    0x1289, 0x128E, 0x128F,
    0x12B6, 0x12B7, 0x12BF,
    0x12C1, 0x12C6, 0x12C7,
    0x12D7,
    0x1311, 0x1316, 0x1317
};
    
static LDB_CHAR Amharic_MultiList[][7] = 
{
    {0x1200, 0x1208, 0x1210, 0x1218, 0x1220, 0x0000},   /* vk_1 */
    {0x1228, 0x1230, 0x1238, 0x1240, 0x1248, 0x0000},   /* vk_2 */
    {0x1250, 0x1258, 0x1260, 0x1268, 0x1270, 0x0000},   /* vk_3 */
    {0x1278, 0x1280, 0x1288, 0x1290, 0x1298, 0x0000},   /* vk_4 */
    {0x12A0, 0x12A8, 0x12B0, 0x12B8, 0x12C0, 0x0000},   /* vk_5 */
    {0x12C8, 0x12D0, 0x12D8, 0x12E0, 0x12E8, 0x0000},   /* vk_6 */
    {0x12F0, 0x12F8, 0x1300, 0x1308, 0x1310, 0x0000},   /* vk_7 */
    {0x1318, 0x1320, 0x1328, 0x1330, 0x0000, 0x0000},   /* vk_8 */
    {0x1338, 0x1340, 0x1348, 0x1350, 0x0000, 0x0000}    /* vk_9 */
};

static IIME_BOOL Amharic_IsUsedKey(int iMsg)
{
    if (iMsg >= IIME_VK_0 && iMsg <= IIME_VK_9)
        return IIME_TRUE;
    else if (iMsg == IIME_VK_STAR)
        return IIME_TRUE;
    else
        return IIME_FALSE;
}

static IIME_BOOL Amharic_FindSkipChar(LDB_CHAR c)
{
    int beg, end, mid;
    beg = 0;
    end = sizeof(Amharic_skipped_char) / sizeof(LDB_CHAR);
    while(beg <= end)
    {
        mid = (beg + end) / 2;
        if (c > Amharic_skipped_char[mid])
            beg = mid + 1;
        else if (c < Amharic_skipped_char[mid])
            end = mid - 1;
        else
            return 1;
    }

    return 0;
}

static LDB_CHAR Amharic_GetNextVowel(LDB_CHAR preChar, int nCount)
{
    LDB_CHAR next;
    switch(preChar)
    {
    case 0x12F:
        next = 0x1359;
        break;
    case 0x122F:
        next = 0x1358;
        break;
    case 0x134F:
        next = 0x135A;
        break;
    case 0x1359:
        next = 0x1218;
        break;
    case 0x1358:
        next = 0x1228;
        break;
    case 0x135A:
        next = 0x1348;
        break;
    default:
        next = preChar + 1;
        while(Amharic_FindSkipChar(next))
            next++;

        if (next % 8 == 0)
            next -= 8;
        break;
    }
    return next;
}

static LDB_CHAR Amharic_GetNextConsonant(int iMsg, int nCount)
{
    LDB_CHAR *pList = Amharic_MultiList[iMsg - IIME_VK_1];

    int nTotal = 5;
    if ((iMsg - IIME_VK_1) > 6)
        nTotal = 4;

    nCount = (nCount - 1) % nTotal;
    return pList[nCount];
}

static LDB_CHAR GetAmharicChar(IIMEMTProvider *pThis, LDB_CHAR preChar, int iMsg)
{
    LDB_CHAR chAmharic = 0;

    if (preChar >= 0x1200 && preChar <= 0x135A)
    {
        switch(iMsg)
        {
        case IIME_VK_STAR:
            chAmharic = Amharic_GetNextVowel(preChar, pThis->m_nCount);
            break;
        case IIME_VK_0:
            chAmharic = 0x135F;
            break;
        default:
            chAmharic = Amharic_GetNextConsonant(iMsg, pThis->m_nCount);
            break;
        }

    }
    else
    {
        switch(iMsg)
        {
        case IIME_VK_STAR:
            break;
        case IIME_VK_0:
            chAmharic = ' ';
            break;
        default:
            chAmharic = Amharic_MultiList[iMsg - IIME_VK_1][0];
            break;
        }
    }

    return chAmharic;
}

static IIME_RESULT IIMEMT_AmharicKeyboardHandle(IIMEMTProvider *pThis, IIME_UINT iMsg, IIME_BOOL bLongClick)
{
    IIME_RESULT ret;

    iMsg &= ~IIME_VK_LONGCLICK;

    if (!Amharic_IsUsedKey(iMsg))
    {
        ret = IIME_ERROR_UNUSED;

        if (pThis->m_nPreKey)
        {
            pThis->m_wCommit = pThis->m_wUnderline;
            
            pThis->m_nPreKey = 0;
            pThis->m_wUnderline = 0;
            pThis->m_nCount = 0;

            IIMEMT_FillCommit(pThis);

            if (iMsg == IIME_VK_SELECT)
            {
                pThis->m_nPreKey = 0;
                ret = IIME_ERROR_USED;
            }
        }
    }
    else if (bLongClick)
    {
        if (pThis->m_nPreKey)
        {
            pThis->m_wCommit = pThis->m_wUnderline;
            
            pThis->m_nPreKey = 0;
            pThis->m_wUnderline = 0;
            pThis->m_nCount = 0;

            IIMEMT_FillCommit(pThis);
        }

        if (iMsg == IIME_VK_STAR)
            ret = IIME_ERROR_UNUSED;
        else
        {
            pThis->m_wCommit = g_pDigit[iMsg - '0'];
            IIMEMT_FillCommit(pThis);
            
            ret = IIME_ERROR_USED;
        }
    }
    else if (iMsg == pThis->m_nPreKey)
    {
        if (iMsg == IIME_VK_0)
        {
            pThis->m_wCommit = ' ';
            IIMEMT_FillCommit(pThis);
            ret = IIME_ERROR_USED;
        }
        else
        {
            LDB_CHAR c;
            pThis->m_nCount++;
        
            c = GetAmharicChar(pThis, pThis->m_wUnderline, iMsg);
    
            if (c == 0)
                ret = IIME_ERROR_UNUSED;
            else
            {
                pThis->m_wUnderline = c;
                IIMEMT_FillUnderline(pThis);

                ret = IIME_ERROR_USED;
            }
        }
    }
    else
    {
        if (iMsg == IIME_VK_0)
        {
            if (pThis->m_nPreKey)
            {
                pThis->m_nPreKey = 0;
                pThis->m_nCount = 0;

                pThis->m_wCommit = pThis->m_wUnderline;
                IIMEMT_FillCommit(pThis);

                pThis->m_wUnderline = 0;
            }

            pThis->m_wCommit = ' ';
            IIMEMT_FillCommit(pThis);
            ret = IIME_ERROR_USED;
        }
        else
        {
            LDB_CHAR previous_char, c;
        
            if (iMsg == IIME_VK_STAR)
            {
                if (pThis->m_wUnderline)
                    previous_char = pThis->m_wUnderline;
                else
                {
            		pThis->m_dwUpdateFlag |= IIME_UI_COMMIT_BACKSPACE;

                    previous_char = pThis->m_wPreChar;
                }
            }
            else
            {
                if (pThis->m_nPreKey)
                {
                    pThis->m_wCommit = pThis->m_wUnderline;
                    IIMEMT_FillCommit(pThis);
                }
                previous_char = 0;
            }

            pThis->m_nCount = 1;
            pThis->m_nPreKey = iMsg;

            c = GetAmharicChar(pThis, previous_char, iMsg);
            if (c == 0)
                ret = IIME_ERROR_UNUSED;
            else
            {
                pThis->m_wUnderline = c;
                IIMEMT_FillUnderline(pThis);
                ret = IIME_ERROR_USED;
            }
        }

    }

    return ret;
}

static IIME_RESULT IIMEMT_TibetanConvertionHandle(IIMEMTProvider *pThis, IIME_UINT iMsg)
{
    if (iMsg == IIME_VK_UP || iMsg == IIME_VK_DOWN)
    {
        if (pThis->m_wUnderline != 0)
        {
            unsigned char char_type = LDB_TIBETAN_OOV;
            
            char_type = ldbapi4_get_tibetan_char_type(pThis->m_wUnderline);
            if (char_type == LDB_TIBETAN_CON)
                pThis->m_wUnderline = ldbapi4_tibetan_consonant_to_subjoined(pThis->m_wUnderline);
            else if (char_type == LDB_TIBETAN_JOC)
                pThis->m_wUnderline = ldbapi4_tibetan_subjoined_to_consonant(pThis->m_wUnderline);
            
            IIMEMT_FillUnderline(pThis);                
                
            return IIME_ERROR_USED;
        }
    }
    
    return IIME_ERROR_UNUSED;
}

#endif
