/****************************************************************************
 *
 * IIMEpvdlatininline.c
 *
 */

#ifdef IIME_LATIN_INLINE

#include "utli/IIMEstd.h"
#include "improvider/provider_com.h"

#include "inc/kmxime.h"
#include "core/ldbapi4/kmxime_latin.h"
#include "IIMEpvdlatininline.h"

#define MAX_MATCH 32
#define IIME_VK_CLEARALL	(0x00010000 | IIME_VK_BACKSPACE)

typedef struct tagIIMELatinInlineProvider
{
	DECLARE_PROVIDER_INTERFACE()

	KMX_IME_ENGINE	m_Engine;

	IIME_INT          m_idMethod;
    short           m_case_info;
    const void*     pdbMisc;

	char            m_pInput[64];
	int				m_nInput;

	KMX_LATIN_CANDIDATE	m_candidate[MAX_MATCH];
	int				m_nCandidate;
    int             m_iHotCandidate;

	IIME_IMM_CONTENT	m_immContent;

	/* imm metriIIME */
    int						m_nWinWidth;           /* [in] Candidate window width*/
    int						m_nLableWidth;         /* [in] Lable width*/
    IIME_GET_TEXT_WIDTH_FUNC	m_pGetTextWidthFunc;

} IIMELatinInlineProvider;

/*
 Declaration
 */

DECLARE_PROVIDER_FUNCTIONS(IIMELatinInlineProvider)

static IIME_RESULT LatinInline_KeyHandle(IIMELatinInlineProvider *pThis, IIME_UINT iMsg);

static IIME_RESULT LatinInline_Add(IIMELatinInlineProvider *pThis, IIME_UINT iMsg);
static IIME_RESULT LatinInline_Iterate(IIMELatinInlineProvider *pThis, IIME_UINT iMsg);
static IIME_RESULT LatinInline_Commit(IIMELatinInlineProvider *pThis, IIME_UINT iMsg);
static IIME_RESULT LatinInline_Remove(IIMELatinInlineProvider *pThis);
static IIME_RESULT LatinInline_RemoveAll(IIMELatinInlineProvider *pThis);
static IIME_RESULT LatinInline_SwitchCase(IIMELatinInlineProvider *pThis);

static void LatinInline_ResetAll(IIMELatinInlineProvider *pThis);
static void LatinInline_ResetCandidate(IIMELatinInlineProvider *pThis);
static void LatinInline_Recognize(IIMELatinInlineProvider *pThis);
static void LatinInline_UpdateCase(IIMELatinInlineProvider *pThis);
/*
 Constructor an destructor
 */

Provider_Com* IIMELatinInlineProvider_Create(
    PIIME_IMM_PARAMS          pImmParams,
    PIIME_USER_PARAMS         pUserParam1,
    PIIME_USER_PARAMS         pUserParam2,
    const void*             pdbMisc,
	const void*				pdbMO,
    const IIME_MULTI_STRING_TABLE* pMulti_string_table,
    IIME_INT                  idMethod)
{
	KMX_IME_ENGINE_INIT	init_info;

	IIMELatinInlineProvider *pThis = IIME_MALLOC(sizeof(IIMELatinInlineProvider));
	memset(pThis, 0, sizeof(IIMELatinInlineProvider));

    /* initialize interface functions*/
	INITIALIZE_PROVIDER_INTERFACE(IIMELatinInlineProvider);

	pThis->m_idMethod = idMethod;
	pThis->m_nWinWidth = pImmParams->nWinWidth;
    pThis->m_nLableWidth = pImmParams->nLableWidth;
	pThis->m_pGetTextWidthFunc = pImmParams->pGetTextWidthFunc;

	pThis->m_Engine.engine = IIME_MALLOC(16 * 1024);

	init_info.database = pdbMisc;
	init_info.user_data = pUserParam1->pUserCache;
	init_info.user_data_size = pUserParam1->iUserCacheSize;
	init_info.flags = 0;

	switch(pThis->m_idMethod % 10)
	{
	case 7:
        pThis->m_case_info = KMX_LATIN_INITIAL_CASE;
		break;
	case 8:
		pThis->m_case_info = KMX_LATIN_UPPER_CASE;
		break;
	case 9:
		pThis->m_case_info = KMX_LATIN_LOWER_CASE;
		break;
	default:
		pThis->m_case_info = KMX_LATIN_INITIAL_CASE;
		break;
	}

    if (!kmx_latin_initialize_ex(&(pThis->m_Engine), &init_info))
	{
		IIME_FREE(pThis->m_Engine.engine);
		IIME_FREE(pThis);

		return 0;
	}

	return (Provider_Com*)pThis;

}

void IIMELatinInlineProvider_Delete(
    Provider_Com*           pProvider)
{
	IIMELatinInlineProvider *pThis = (IIMELatinInlineProvider *)pProvider;
	if (pThis)
	{
		IIME_FREE(pThis->m_Engine.engine);
		IIME_FREE(pThis);
	}

	return;
}
/*
 interface
 */

static IIME_RESULT EventHandle_IIMELatinInlineProvider(Provider_Com *pProvider,
                                   IIME_UINT hEvent,
                                   IIME_UINT iMsg,
                                   IIME_IMM_CONTENT *pContent)
{
	IIME_RESULT ret = IIME_ERROR_UNUSED;
	IIMELatinInlineProvider *pThis = (IIMELatinInlineProvider *)pProvider;

	memset(pThis->m_immContent.pCommit, 0, sizeof(unsigned short) * 64);

    if (hEvent == IIME_EVENT_KEYBOARD)
	{
		ret = LatinInline_KeyHandle(pThis, iMsg);
	}
	else
		ret = IIME_ERROR_UNUSED;

	memset(pContent, 0, sizeof(IIME_IMM_CONTENT));
	memcpy(pContent, &(pThis->m_immContent), sizeof(IIME_IMM_CONTENT));

    if (pThis->m_nCandidate > 0)
    {
        if (pThis->m_iHotCandidate < pThis->m_nCandidate)
        {
            KMX_LATIN_CANDIDATE *pCandidate = pThis->m_candidate + pThis->m_iHotCandidate;
            memcpy(pContent->pUnderlineBuffer, pCandidate->string, sizeof(unsigned short) * pCandidate->length);
            pContent->nUnderline = pCandidate->length;
        }
    }
	return ret;
}

static IIME_RESULT SetOptions_IIMELatinInlineProvider(Provider_Com *pProvider,
                                  const IIME_IMM_OPTIONS *pOption)
{
	return IIME_ERROR_SUCCESS;
}

static IIME_RESULT GetOptions_IIMELatinInlineProvider(Provider_Com *pProvider,
                                  IIME_IMM_OPTIONS *pOption)
{
    return IIME_ERROR_SUCCESS;
}

static IIME_RESULT AddPhrase_IIMELatinInlineProvider(Provider_Com* pProvider, unsigned short* szPhrase, int nLen)
{
    return IIME_ERROR_UNKNOWN;
}

static IIME_RESULT CheckAsso_IIMELatinInlineProvider(Provider_Com* pProvider, unsigned short wContext, unsigned short* szAsso, int* pNumber)
{
    return IIME_ERROR_UNKNOWN;
}

static IIME_RESULT QueryProperties_IIMELatinInlineProvider(Provider_Com* pProvider, int* pProperties)
{
    return IIME_ERROR_UNKNOWN;
}

/*
 implementions
 */

static IIME_RESULT LatinInline_KeyHandle(IIMELatinInlineProvider *pThis, IIME_UINT iMsg)
{
    IIME_RESULT ret = IIME_ERROR_UNUSED;

    switch (iMsg)
    {
    case IIME_VK_1:
    case IIME_VK_2:
    case IIME_VK_3:
    case IIME_VK_4:
    case IIME_VK_5:
    case IIME_VK_6:
    case IIME_VK_7:
    case IIME_VK_8:
    case IIME_VK_9:
        ret = LatinInline_Add(pThis, iMsg);
        break;
    case IIME_VK_UP:
    case IIME_VK_DOWN:
        ret = LatinInline_Iterate(pThis, iMsg);
        break;
    case IIME_VK_BACKSPACE:
        ret = LatinInline_Remove(pThis);
        break;
    case IIME_VK_CLEARALL:
        ret = LatinInline_RemoveAll(pThis);
        break;
    case IIME_VK_SELECT:
        ret = LatinInline_Commit(pThis, 0);
        break;
    case IIME_VK_0:
        ret = LatinInline_Commit(pThis, iMsg);
        break;
    case IIME_VK_POUND:
        ret = LatinInline_SwitchCase(pThis);
        break;
    default:
        ret = LatinInline_Commit(pThis, 0);
        ret = IIME_ERROR_UNUSED;
        break;
    }

    return ret;
}


static IIME_RESULT LatinInline_Add(IIMELatinInlineProvider *pThis, IIME_UINT iMsg)
{
    IIME_RESULT ret = IIME_ERROR_UNUSED;

    if (pThis->m_nInput < MAX_MATCH)
    {
        pThis->m_pInput[pThis->m_nInput++] = (char)iMsg;

        LatinInline_Recognize(pThis);

        ret = IIME_ERROR_USED;
    }

    return ret;
}

static IIME_RESULT LatinInline_Iterate(IIMELatinInlineProvider *pThis, IIME_UINT iMsg)
{
    IIME_RESULT ret = IIME_ERROR_UNUSED;
    if (pThis->m_nInput > 0)
    {
        if (iMsg == IIME_VK_DOWN)
        {
            pThis->m_iHotCandidate = (pThis->m_iHotCandidate + 1) % pThis->m_nCandidate;
        }
        else if (iMsg == IIME_VK_UP)
        {
            if (pThis->m_iHotCandidate > 1)
                pThis->m_iHotCandidate--;
            else
                pThis->m_iHotCandidate = pThis->m_nCandidate - 1;
        }

        ret = IIME_ERROR_USED;
    }

    return ret;
}

static IIME_RESULT LatinInline_Commit(IIMELatinInlineProvider *pThis, IIME_UINT iMsg)
{
    IIME_RESULT ret = IIME_ERROR_UNUSED;
    if (pThis->m_nInput)
    {
        KMX_LATIN_CANDIDATE *pCandidate = pThis->m_candidate + pThis->m_iHotCandidate;
        memcpy(pThis->m_immContent.pCommit, pCandidate->string, sizeof(unsigned short) * pCandidate->length);

        ret = IIME_ERROR_USED;
    }

    LatinInline_ResetAll(pThis);

    if (iMsg == IIME_VK_0)
    {
        IIME_WCHAR wszSpace[] = L" ";
        IIMEString_Append(pThis->m_immContent.pCommit, wszSpace);

        ret = IIME_ERROR_USED;
    }

    return ret;
}

static IIME_RESULT LatinInline_Remove(IIMELatinInlineProvider *pThis)
{
    IIME_RESULT ret = IIME_ERROR_UNUSED;
    if (pThis->m_nInput > 0)
    {
        pThis->m_pInput[pThis->m_nInput--] = 0;
        if (pThis->m_nInput > 0)
        {
            LatinInline_Recognize(pThis);
        }
        else
        {
            LatinInline_ResetCandidate(pThis);
        }

        ret = IIME_ERROR_USED;
    }

    return ret;
}

static IIME_RESULT LatinInline_RemoveAll(IIMELatinInlineProvider *pThis)
{
    IIME_RESULT ret = IIME_ERROR_UNUSED;
    if (pThis->m_nInput > 0)
    {
        LatinInline_ResetAll(pThis);
        ret = IIME_ERROR_USED;
    }

    return ret;        
}

static IIME_RESULT LatinInline_SwitchCase(IIMELatinInlineProvider *pThis)
{
    IIME_RESULT ret = IIME_ERROR_UNUSED;
    if (pThis->m_nInput && pThis->m_idMethod / 10 == 10)    /* only for english */
    {
        switch(pThis->m_case_info)
        {
        case KMX_LATIN_INITIAL_CASE:
            pThis->m_case_info = KMX_LATIN_UPPER_CASE;
            pThis->m_idMethod = 108;
            break;
        case KMX_LATIN_UPPER_CASE:
            pThis->m_case_info = KMX_LATIN_LOWER_CASE;
            pThis->m_idMethod = 109;
            break;
        case KMX_LATIN_LOWER_CASE:
            pThis->m_case_info = KMX_LATIN_INITIAL_CASE;
            pThis->m_idMethod = 107;
            break;
        }

        LatinInline_UpdateCase(pThis);

        ret = IIME_ERROR_USED | (pThis->m_idMethod << 16);
    }
    else if (pThis->m_nInput && pThis->m_idMethod / 10 == 40)    /* only for english */
    {
        switch(pThis->m_case_info)
        {
        case KMX_LATIN_INITIAL_CASE:
            pThis->m_case_info = KMX_LATIN_UPPER_CASE;
            pThis->m_idMethod = 408;
            break;
        case KMX_LATIN_UPPER_CASE:
            pThis->m_case_info = KMX_LATIN_LOWER_CASE;
            pThis->m_idMethod = 409;
            break;
        case KMX_LATIN_LOWER_CASE:
            pThis->m_case_info = KMX_LATIN_INITIAL_CASE;
            pThis->m_idMethod = 407;
            break;
        }

        LatinInline_UpdateCase(pThis);

        ret = IIME_ERROR_USED | (pThis->m_idMethod << 16);
    }

    return ret;
}

static void LatinInline_ResetAll(IIMELatinInlineProvider *pThis)
{
    memset(pThis->m_immContent.pUnderlineBuffer, 0, sizeof(unsigned short) * 64);
    pThis->m_immContent.nUnderline = 0;
    pThis->m_immContent.iHotUnderline = 0;

    memset(pThis->m_pInput, 0, sizeof(unsigned char) * 64);
    pThis->m_nInput = 0;

    LatinInline_ResetCandidate(pThis);
}

static void LatinInline_ResetCandidate(IIMELatinInlineProvider *pThis)
{
    memset(&(pThis->m_candidate), 0, sizeof(KMX_LATIN_CANDIDATE) * MAX_MATCH);
    pThis->m_nCandidate = 0;
    pThis->m_iHotCandidate = 0;
}

static void LatinInline_Recognize(IIMELatinInlineProvider *pThis)
{
    IIME_BOOL bAvaliable = 1;
    short pos = 0;
    KMX_LATIN_MATCH_RESULT match_result;

    LatinInline_ResetCandidate(pThis);

    if (pThis->m_nInput == 0)
        return;

    while (bAvaliable)
    {
        if (kmx_latin_match(&(pThis->m_Engine), NULL, pThis->m_pInput, (short)pThis->m_nInput, pThis->m_case_info, pos, &match_result))
        {
            int i;
            for (i = 0; i < match_result.count; i++)
            {
                KMX_LATIN_CANDIDATE *pCandidate = match_result.table + i;

                memcpy(pThis->m_candidate[pos].string, pCandidate->string, sizeof(unsigned short) * pCandidate->length);
                pThis->m_candidate[pos].length = pCandidate->length;

                pos++;
            }

            bAvaliable = match_result.more_available;
        }
        else
        {
            bAvaliable = IIME_FALSE;
        }
    }

    pThis->m_nCandidate = pos;
}

static void LatinInline_UpdateCase(IIMELatinInlineProvider *pThis)
{
    int i;
    for (i = 0; i < pThis->m_nCandidate; i++)
    {
        KMX_LATIN_CANDIDATE *pCandidate = pThis->m_candidate + i;
        switch(pThis->m_case_info)
        {
        case KMX_LATIN_INITIAL_CASE:
            kmx_latin_initial_case(&(pThis->m_Engine), pCandidate->string, pCandidate->length);
            break;
        case KMX_LATIN_UPPER_CASE:
            kmx_latin_upper_case(&(pThis->m_Engine), pCandidate->string, pCandidate->length);
            break;
        case KMX_LATIN_LOWER_CASE:
            kmx_latin_lower_case(&(pThis->m_Engine), pCandidate->string, pCandidate->length);
            break;
        }
    }
}
#else

void IIMELatinInlineProvider_Delete_Dummy(void)
{
    /* Slient empty translation unit warning if IIME_LATIN_INLINE is not defined. */
}

#endif
