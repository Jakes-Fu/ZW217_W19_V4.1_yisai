/****************************************************************************
 *
 * IIMEpvdahmaric.c
 *
 */
#if 0
#include "utli/IIMEstd.h"
#include "improvider/provider_com.h"
#else
#include "IIMEstd.h"
#include "provider_com.h"
#endif
#include "IIMEpvdamharic.h"

typedef enum tag_status
{
    En_Nothing,
    En_Lead,
    En_LeadTimeOut,
    En_Symbol,
}STATUS;

typedef struct tagAmharicProvider
{
    DECLARE_PROVIDER_INTERFACE()

    IIME_INT          m_idMethod;
    const void*     pdbMisc;

    STATUS          m_iStatus;

    IIME_WCHAR        m_cUnderLine;
    IIME_WCHAR        m_cCommit;
    IIME_WCHAR        m_preKey;
    int             m_nCount;

} AmharicProvider;

/*
 Declaration
 */

DECLARE_PROVIDER_FUNCTIONS(AmharicProvider)

static IIME_RESULT Amharic_KeyHandle(AmharicProvider *pThis, IIME_WCHAR iMsg);
static IIME_RESULT Amharic_TimerHandle(AmharicProvider *pThis);

static IIME_WCHAR GetLeadCandidate(AmharicProvider *pThis);
static void CommitUnderline(AmharicProvider *pThis);
static IIME_BOOL CommitByNumber(AmharicProvider *pThis, IIME_WCHAR iMsg);

/*
 Constructor an destructor
 */

Provider_Com* AmharicProvider_Create(
    PIIME_IMM_PARAMS          pImmParams,
    PIIME_USER_PARAMS         pUserParam1,
    PIIME_USER_PARAMS         pUserParam2,
    const void*             pdbMisc,
	const void*				pdbMO,
    const IIME_MULTI_STRING_TABLE* pMulti_string_table,
    IIME_INT                  idMethod)
{
    AmharicProvider *pThis = (AmharicProvider *)IIME_MALLOC(sizeof(AmharicProvider));
	memset(pThis, 0, sizeof(AmharicProvider));

   	/* initialize interface functions*/
	INITIALIZE_PROVIDER_INTERFACE(AmharicProvider)

    return (Provider_Com*)pThis;
}

void AmharicProvider_Delete(
    Provider_Com*           pProvider)
{
    AmharicProvider *pThis = (AmharicProvider*)pProvider;

    if (pThis!= NULL)
        IIME_FREE(pThis);
}

/*
 interface
 */

static IIME_RESULT EventHandle_AmharicProvider(Provider_Com *pProvider,
                                   IIME_UINT hEvent,
                                   IIME_UINT iMsg,
                                   IIME_IMM_CONTENT *pContent)
{
    IIME_RESULT ret = IIME_ERROR_UNUSED;
    AmharicProvider *pThis = (AmharicProvider*)pProvider;

	/* clear commit*/
    pThis->m_cCommit = 0;
    memset(pContent, 0, sizeof(IIME_IMM_CONTENT));

    if (hEvent == IIME_EVENT_KEYBOARD)
    {
        ret = Amharic_KeyHandle(pThis, (IIME_WCHAR)iMsg);
    }
    else if (hEvent == IIME_EVENT_TIMER)
    {
        ret = Amharic_TimerHandle(pThis);
    }
    else
        ret = IIME_ERROR_UNKNOWN;

    if (pThis->m_cCommit)
    {
        pContent->pCommit[0] = pThis->m_cCommit;
        pContent->pCommit[1] = 0;
    }

    if (pThis->m_cUnderLine)
    {
        pContent->pUnderlineBuffer[0] = pThis->m_cUnderLine;
        pContent->pUnderlineBuffer[1] = 0;

        pContent->nUnderline = 1;
    }

    if (pThis->m_iStatus == En_Lead || pThis->m_iStatus == En_Symbol)
        pContent->flagTimer = IIME_TIMER_SET;
    else
        pContent->flagTimer = IIME_TIMER_CLEAR;

    return ret;
}

static IIME_RESULT SetOptions_AmharicProvider(Provider_Com *pProvider,
                                  const IIME_IMM_OPTIONS *pOption)
{
    return IIME_ERROR_SUCCESS;
}

static IIME_RESULT GetOptions_AmharicProvider(Provider_Com *pProvider,
                                  IIME_IMM_OPTIONS *pOption)
{
    return IIME_ERROR_SUCCESS;
}

static IIME_RESULT AddPhrase_AmharicProvider(Provider_Com* pProvider, unsigned short* szPhrase, int nLen)
{
    return IIME_ERROR_UNKNOWN;
}

static IIME_RESULT CheckAsso_AmharicProvider(Provider_Com* pProvider, unsigned short wContext, unsigned short* szAsso, int* pNumber)
{
    return IIME_ERROR_UNKNOWN;
}

static IIME_RESULT QueryProperties_AmharicProvider(Provider_Com* pProvider, int* pProperties)
{
    return IIME_ERROR_UNKNOWN;
}

/* 
 * implemention
 *
 */

static IIME_RESULT Amharic_KeyHandle(AmharicProvider *pThis, IIME_WCHAR iMsg)
{
    IIME_RESULT ret = IIME_ERROR_UNUSED;

    if (pThis->m_iStatus == En_Nothing)
    {
        switch(iMsg)
        {
        case IIME_VK_0:
        case IIME_VK_1:
        case IIME_VK_STAR:
            pThis->m_iStatus = En_Symbol;
            pThis->m_preKey = iMsg;
            pThis->m_nCount = 1;
            pThis->m_cUnderLine = GetLeadCandidate(pThis);
            ret = IIME_ERROR_USED;
            break;
        case IIME_VK_2:
        case IIME_VK_3:
        case IIME_VK_4:
        case IIME_VK_5:
        case IIME_VK_6:
        case IIME_VK_7:
        case IIME_VK_8:
        case IIME_VK_9:
            pThis->m_iStatus = En_Lead;
            pThis->m_preKey = iMsg;
            pThis->m_nCount = 1;
            pThis->m_cUnderLine = GetLeadCandidate(pThis);
            ret = IIME_ERROR_USED;
            break;
        default:
            break;
        }
    }
    else if (pThis->m_iStatus == En_Lead)
    {
        if (iMsg == pThis->m_preKey)
        {
            pThis->m_nCount++;
            pThis->m_cUnderLine = GetLeadCandidate(pThis);

            ret = IIME_ERROR_USED;
        }
        else
        {
            switch (iMsg)
            {
            case IIME_VK_0:
            case IIME_VK_1:
            case IIME_VK_STAR:
                CommitUnderline(pThis);
                pThis->m_iStatus = En_Symbol;
                pThis->m_preKey = iMsg;
                pThis->m_nCount = 1;
                pThis->m_cUnderLine = GetLeadCandidate(pThis);
                ret = IIME_ERROR_USED;
                break;
            case IIME_VK_2:
            case IIME_VK_3:
            case IIME_VK_4:
            case IIME_VK_5:
            case IIME_VK_6:
            case IIME_VK_7:
            case IIME_VK_8:
            case IIME_VK_9:
                if (pThis->m_cUnderLine >= '2' && pThis->m_cUnderLine <= '9')
                {
                    pThis->m_cCommit = pThis->m_cUnderLine;
                    pThis->m_preKey = iMsg;
                    pThis->m_nCount = 1;
                    pThis->m_cUnderLine = GetLeadCandidate(pThis);

                    ret = IIME_ERROR_USED;
                }
                else
                {
                    IIME_BOOL bCommit = CommitByNumber(pThis, iMsg);
                    if (bCommit)
                    {
                        pThis->m_iStatus = En_Nothing;
                    }
                    else
                    {
                        pThis->m_iStatus = En_LeadTimeOut;
                    }
                    ret = IIME_ERROR_USED;
                }
                break;
            default:
                CommitUnderline(pThis);
                pThis->m_iStatus = En_Nothing;
                ret = IIME_ERROR_UNUSED;
                break;
            }
        }
    }
    else if (pThis->m_iStatus == En_LeadTimeOut)
    {
        switch (iMsg)
        {
        case IIME_VK_2:
        case IIME_VK_3:
        case IIME_VK_4:
        case IIME_VK_5:
        case IIME_VK_6:
        case IIME_VK_7:
        case IIME_VK_8:
        case IIME_VK_9:
            {
                IIME_BOOL bCommit = CommitByNumber(pThis, iMsg);
                if (bCommit)
                    pThis->m_iStatus = En_Nothing;
                ret = IIME_ERROR_USED;
            }
            break;
        case IIME_VK_0:
        case IIME_VK_1:
        case IIME_VK_STAR:
            CommitUnderline(pThis);
            pThis->m_iStatus = En_Symbol;
            pThis->m_preKey = iMsg;
            pThis->m_nCount = 1;
            pThis->m_cUnderLine = GetLeadCandidate(pThis);
            ret = IIME_ERROR_USED;
            break;
        default:
            CommitUnderline(pThis);
            pThis->m_iStatus = En_Nothing;
            ret = IIME_ERROR_UNUSED;
            break;
        }
    }
    else if (pThis->m_iStatus == En_Symbol)
    {
        switch (iMsg)
        {
        case IIME_VK_0:
        case IIME_VK_1:
        case IIME_VK_STAR:
            pThis->m_preKey = iMsg;
            pThis->m_nCount++;
            pThis->m_cUnderLine = GetLeadCandidate(pThis);
            ret = IIME_ERROR_USED;
            break;
        case IIME_VK_2:
        case IIME_VK_3:
        case IIME_VK_4:
        case IIME_VK_5:
        case IIME_VK_6:
        case IIME_VK_7:
        case IIME_VK_8:
        case IIME_VK_9:
            pThis->m_iStatus = En_Lead;
            pThis->m_preKey = iMsg;
            pThis->m_nCount = 1;
            pThis->m_cUnderLine = GetLeadCandidate(pThis);
            ret = IIME_ERROR_USED;
            break;
        default:
            CommitUnderline(pThis);
            pThis->m_iStatus = En_Nothing;
            IIME_ERROR_UNUSED;
            break;
        }

    }

    return ret;
}

static IIME_RESULT Amharic_TimerHandle(AmharicProvider *pThis)
{
    IIME_RESULT ret = IIME_ERROR_USED;
    if (pThis->m_iStatus == En_Lead)
    {
        if (pThis->m_cUnderLine >= '2' && pThis->m_cUnderLine <= '9')
        {
            CommitUnderline(pThis);
            pThis->m_iStatus = En_Nothing;
        }
        else
            pThis->m_iStatus = En_LeadTimeOut;
    }
    else if (pThis->m_iStatus == En_Symbol)
    {
        CommitUnderline(pThis);
        pThis->m_iStatus = En_Nothing;
    }
    else
        ret = IIME_ERROR_UNUSED;

    return ret;
}

static IIME_WCHAR GetLeadCandidate(AmharicProvider *pThis)
{
    typedef struct tagAMHARICLEADCHAR
    {
        int num;
        IIME_WCHAR value[8];
    }AMHARICLEADCHAR;

    static const AMHARICLEADCHAR Amharic_Lead[] = 
    {
        {2, {' ', '0'}},
        {7, {0x1362, 0x1363, 0x1364, 0x1365, '?', '!', '1'}}, 
        {4, {0x1200, 0x1208, 0x1218, '2'}},
        {5, {0x1228, 0x1230, 0x1238, 0x1240, '3'}},
        {5, {0x1260, 0x1270, 0x1278, 0x1290, '4'}},
        {4, {0x1298, 0x12A0, 0x12A8, '5'}}, 
        {4, {0x12C8, 0x12D8, 0x12E0, '6'}},
        {5, {0x12E8, 0x12F0, 0x1300, 0x1308, '7'}},
        {4, {0x1320, 0x1328, 0x1330, '8'}},
        {5, {0x1340, 0x1348, 0x1350, 0x1268, '9'}},
        {6, {0x1362, 0x1363, 0x1364, 0x1365, '?', '!'}}
    };

    IIME_WCHAR    c = 0;
    int num = 0;
    const IIME_WCHAR *pList = 0;
    if (pThis->m_preKey >= IIME_VK_0 && pThis->m_preKey <= IIME_VK_9)
    {
        pList = Amharic_Lead[pThis->m_preKey - IIME_VK_0].value;
        num = Amharic_Lead[pThis->m_preKey - IIME_VK_0].num;
    }
    else if (pThis->m_preKey == IIME_VK_STAR)
    {
        pList = Amharic_Lead[10].value;
        num = Amharic_Lead[10].num;
    }
    else 
        return 0;

    c = pList[(pThis->m_nCount - 1) % num];

    return c;
}

static void CommitUnderline(AmharicProvider *pThis)
{
    pThis->m_cCommit = pThis->m_cUnderLine;
    pThis->m_cUnderLine = 0;
    pThis->m_preKey = 0;
    pThis->m_nCount = 0;
}

static IIME_BOOL CommitByNumber(AmharicProvider *pThis, IIME_WCHAR iMsg)
{
    IIME_WCHAR c = 0;
    IIME_WCHAR c1 = pThis->m_cUnderLine;
    
    if (pThis->m_iStatus == En_Lead || pThis->m_iStatus == En_LeadTimeOut)
    {
        if (iMsg >= IIME_VK_2 && iMsg <= IIME_VK_9)
        {
            int offset = iMsg - IIME_VK_2;
            if ((c1 == 0x1200 || c1 == 0x12A0 || c1 == 0x12A8) && offset == 7)
                c = 0;
            else if (c1 == 0x1240 && offset == 7)
            {
                c = 0x124B;
            }
            else
            {
                c = (IIME_WCHAR)(c1 + offset);
            }
        }
    }

    if (c)
    {
        pThis->m_cCommit = c;
        pThis->m_cUnderLine = 0;

        return IIME_TRUE;
    }
    else
        return IIME_FALSE;
}
