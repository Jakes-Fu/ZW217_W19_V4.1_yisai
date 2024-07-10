/****************************************************************************
 *
 * IIMEpvdahmaric2.c
 *
 */

#ifndef GTIDE_PRODUCT
#if 0
#include "utli/IIMEstd.h"
#include "improvider/provider_com.h"
#else
#include "IIMEstd.h"
#include "provider_com.h"
#endif
#include "IIMEpvdamharic_2.h"

typedef enum tag_status
{
    En_Nothing,
    En_Lead,
    En_Colomn,
    En_Row,
    En_Symbol
}STATUS;

#define MAX_LEADER  8
#define MAX_CANDIDATE   16

typedef struct tagAmharicProvider
{
    DECLARE_PROVIDER_INTERFACE()

    IIME_INT          m_idMethod;
    const void*     pdbMisc;

    STATUS          m_iStatus;

    IIME_WCHAR        m_pCandidate[MAX_CANDIDATE];
    int             m_iHotCand;
    int             m_nCand;
    int             m_cand_per_page;

    IIME_WCHAR        m_chLeader;

    IIME_GET_TEXT_WIDTH_FUNC m_pfnGetWidth;
    int             m_nWinWidth;           /* [in] Candidate window width*/
    int             m_nLableWidth;         /* [in] Lable width*/

    IIME_WCHAR        m_cUnderLine;
    IIME_WCHAR        m_cCommit;
    int             m_nCount;

} AmharicProvider;

/*
 Declaration
 */

DECLARE_PROVIDER_FUNCTIONS(AmharicProvider)

static IIME_RESULT Amharic_KeyHandle(AmharicProvider *pThis, IIME_WCHAR iMsg);
static IIME_RESULT Amharic_TimerHandle(AmharicProvider *pThis);

static void CommitUnderline(AmharicProvider *pThis);
static IIME_BOOL CommitByNumber(AmharicProvider *pThis, IIME_WCHAR iMsg);

static IIME_WCHAR GetSymbolChar(AmharicProvider *pThis, IIME_WCHAR iMsg);

static void FillLeadCandidate(AmharicProvider *pThis, IIME_WCHAR iMsg);
static void FillColomnCandidate(AmharicProvider *pThis, IIME_WCHAR iMsg);
static void FillRowCandidate(AmharicProvider *pThis);

static void IterateCandidate(AmharicProvider *pThis, IIME_WCHAR iMsg);
static void ClearCandidate(AmharicProvider *pThis);


static IIME_BOOL CandidatesMoreThanWindow(AmharicProvider *pThis);

/*
 Constructor an destructor
 */

Provider_Com* Amharic2Provider_Create(
    PIIME_IMM_PARAMS          pImmParams,
    PIIME_USER_PARAMS         pUserParam1,
    PIIME_USER_PARAMS         pUserParam2,
    const void*             pdbMisc,
	const void*				pdbMO,
    const IIME_MULTI_STRING_TABLE* pMulti_string_table,
    IIME_INT                  idMethod)
{
    IIME_WCHAR chAmharic = 0x1200;
    int      width_amharic = 0;

    AmharicProvider *pThis = (AmharicProvider *)IIME_MALLOC(sizeof(AmharicProvider));
	memset(pThis, 0, sizeof(AmharicProvider));

   	/* initialize interface functions*/
	INITIALIZE_PROVIDER_INTERFACE(AmharicProvider)

    pThis->m_pfnGetWidth = pImmParams->pGetTextWidthFunc;
    pThis->m_nLableWidth = pImmParams->nLableWidth;
    pThis->m_nWinWidth = pImmParams->nWinWidth;

    width_amharic = (IIME_WORD)(pThis->m_pfnGetWidth(&chAmharic, 1));
    width_amharic = (width_amharic > 8) ? width_amharic : 8;
    
    pThis->m_cand_per_page = pImmParams->nWinWidth / (width_amharic + pImmParams->nLableWidth);
    return (Provider_Com*)pThis;
}

void Amharic2Provider_Delete(
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

    if (pThis->m_nCand)
    {
        int i;
        if (CandidatesMoreThanWindow(pThis))
        //if (pThis->m_nCand > pThis->m_cand_per_page)
        {
            //if (HotCandidateInFirstPage(pThis))
            if (pThis->m_iHotCand < pThis->m_cand_per_page)
            {
                pContent->iHotCand = (unsigned short)(pThis->m_iHotCand);
                pContent->nCand = (unsigned short)(pThis->m_cand_per_page);
                for (i = 0; i < pContent->nCand; i++)
                {
                    pContent->pCandBuffer[i * 2] = pThis->m_pCandidate[i];
                    pContent->pCandBuffer[i * 2 + 1] = 0;
                }

                pContent->flagPageCand = IIME_FLAG_PAGEUP;
            }
            else
            {
                pContent->nCand = (unsigned short)(pThis->m_nCand - pThis->m_cand_per_page);
                pContent->iHotCand = (unsigned short)(pThis->m_iHotCand - pThis->m_cand_per_page);
                for (i = 0; i < pContent->nCand; i++)
                {
                    pContent->pCandBuffer[i * 2] = pThis->m_pCandidate[i + pThis->m_cand_per_page];
                    pContent->pCandBuffer[i * 2 + 1] = 0;
                }

                pContent->flagPageCand = IIME_FLAG_PAGEDOWN;
            }
        }
        else
        {
            pContent->nCand = (unsigned short)(pThis->m_nCand);
            pContent->iHotCand = (unsigned short)(pThis->m_iHotCand);
            for (i = 0; i < pThis->m_nCand; i++)
            {
                pContent->pCandBuffer[i * 2] = pThis->m_pCandidate[i];
                pContent->pCandBuffer[i * 2 + 1] = 0;
            }
        }

        if (pThis->m_iStatus == En_Row || pThis->m_iStatus == En_Colomn)
            pContent->flagState = IIME_FLAG_STATE_CAND;
    }

    if (pThis->m_iStatus == En_Symbol)
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
            pThis->m_nCount = 1;
            pThis->m_cUnderLine = GetSymbolChar(pThis, iMsg);
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
            FillLeadCandidate(pThis, iMsg);
            ret = IIME_ERROR_USED;
            break;
        default:
            break;
        }
    }
    else if (pThis->m_iStatus == En_Lead)
    {
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
            pThis->m_iStatus = En_Colomn;
            FillColomnCandidate(pThis, iMsg);
            ret = IIME_ERROR_USED;
            break;
        case IIME_VK_LEFT:
        case IIME_VK_RIGHT:
            IterateCandidate(pThis, iMsg);
            ret = IIME_ERROR_USED;
            break;
        case IIME_VK_SELECT:
            pThis->m_iStatus = En_Row;
            FillRowCandidate(pThis);
            ret = IIME_ERROR_USED;
            break;
        case IIME_VK_BACKSPACE:
            pThis->m_iStatus = En_Nothing;
            ClearCandidate(pThis);
            ret = IIME_ERROR_USED;
            break;
        default:
            ret = IIME_ERROR_USED;
            break;
        }
    }
    else if (pThis->m_iStatus == En_Colomn || pThis->m_iStatus == En_Row)
    {
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
            {
                IIME_BOOL bCommit = CommitByNumber(pThis, iMsg);
                if (bCommit)
                    pThis->m_iStatus = En_Nothing;
                ret = IIME_ERROR_USED;
            }
            break;
        case IIME_VK_LEFT:
        case IIME_VK_RIGHT:
            IterateCandidate(pThis, iMsg);
            ret = IIME_ERROR_USED;
            break;
        case IIME_VK_BACKSPACE:
            pThis->m_iStatus = En_Lead;
            FillLeadCandidate(pThis, 0);
            ret = IIME_ERROR_USED;
            break;
        case IIME_VK_SELECT:
            CommitByNumber(pThis, 0xFFFF);
            pThis->m_iStatus = En_Nothing;
            ret = IIME_ERROR_USED;
            break;
        default:
            ret = IIME_ERROR_USED;
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
            pThis->m_nCount++;
            pThis->m_cUnderLine = GetSymbolChar(pThis, iMsg);
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
            CommitUnderline(pThis);
            pThis->m_iStatus = En_Lead;
            FillLeadCandidate(pThis, iMsg);
            ret = IIME_ERROR_USED;
            break;
        default:
            CommitUnderline(pThis);
            pThis->m_iStatus = En_Nothing;
            ret = IIME_ERROR_UNUSED;
            break;
        }
    }

    return ret;
}

static IIME_RESULT Amharic_TimerHandle(AmharicProvider *pThis)
{
    IIME_RESULT ret = IIME_ERROR_USED;
    if (pThis->m_iStatus == En_Symbol)
    {
        CommitUnderline(pThis);
        pThis->m_iStatus = En_Nothing;
    }
    else
        ret = IIME_ERROR_UNUSED;

    return ret;
}

static void CommitUnderline(AmharicProvider *pThis)
{
    pThis->m_cCommit = pThis->m_cUnderLine;
    pThis->m_cUnderLine = 0;
    pThis->m_nCount = 0;
}

typedef struct tagAMHARICLEADCHAR
{
    int num;
    IIME_WCHAR value[8];
}AMHARICLEADCHAR;

static const AMHARICLEADCHAR Amharic_Symbol[] = 
{
    {2, {' ', '0'}},
    {7, {0x1362, 0x1363, 0x1364, 0x1365, '?', '!', '1'}}, 
    {6, {0x1362, 0x1363, 0x1364, 0x1365, '?', '!'}}
};

static const AMHARICLEADCHAR Amharic_Lead[] = 
{
    {3, {0x1200, 0x1208, 0x1218}},
    {4, {0x1228, 0x1230, 0x1238, 0x1240}},
    {4, {0x1260, 0x1270, 0x1278, 0x1290}},
    {3, {0x1298, 0x12A0, 0x12A8}}, 
    {3, {0x12C8, 0x12D8, 0x12E0}},
    {4, {0x12E8, 0x12F0, 0x1300, 0x1308}},
    {3, {0x1320, 0x1328, 0x1330}},
    {4, {0x1340, 0x1348, 0x1350, 0x1268}},
};


static IIME_WCHAR GetSymbolChar(AmharicProvider *pThis, IIME_WCHAR iMsg)
{

    IIME_WCHAR    c = 0;
    int num = 0;
    const IIME_WCHAR *pList = 0;
    if (iMsg == IIME_VK_0)
    {
        pList = Amharic_Symbol[0].value;
        num = Amharic_Symbol[0].num;
    }
    else if (iMsg == IIME_VK_1)
    {
        pList = Amharic_Symbol[1].value;
        num = Amharic_Symbol[1].num;
    }
    else if (iMsg == IIME_VK_STAR)
    {
        pList = Amharic_Symbol[2].value;
        num = Amharic_Symbol[2].num;
    }
    else 
        return 0;

    c = pList[(pThis->m_nCount - 1) % num];

    return c;
}

static void FillLeadCandidate(AmharicProvider *pThis, IIME_WCHAR iMsg)
{
    int i, num = 0;
    const IIME_WCHAR *pList = 0;

    if (iMsg == 0)
        iMsg = pThis->m_chLeader;

    if (iMsg >= IIME_VK_2 && iMsg <= IIME_VK_9)
    {
        pList = Amharic_Lead[iMsg - IIME_VK_2].value;
        num = Amharic_Lead[iMsg - IIME_VK_2].num;
    }
    else 
        return;

    memset(pThis->m_pCandidate, 0, sizeof(IIME_WCHAR) * MAX_CANDIDATE);

    for (i = 0; i < num; i++)
    {
        pThis->m_pCandidate[i] = pList[i];
    }
    
    pThis->m_iHotCand = 0;
    pThis->m_nCand = num;

    pThis->m_chLeader = iMsg;
    
    return;
}

static void FillColomnCandidate(AmharicProvider *pThis, IIME_WCHAR iMsg)
{
    int i, num, offset, pos;
    const IIME_WCHAR *pList;

    if (pThis->m_chLeader >= IIME_VK_2 && pThis->m_chLeader <= IIME_VK_9)
    {
        pList = Amharic_Lead[pThis->m_chLeader - IIME_VK_2].value;
        num = Amharic_Lead[pThis->m_chLeader - IIME_VK_2].num;
    }
    else
        return;

    offset = iMsg - IIME_VK_1;
    pos = 0;

    memset(pThis->m_pCandidate, 0, sizeof(IIME_WCHAR) * MAX_CANDIDATE);
    for (i = 0; i < num; i++)
    {
        IIME_WCHAR c1 = pList[i];
        if (offset == 7)
        {
            switch (c1)
            {
            case 0x1200:
            //case 0x12A0:
            //case 0x12A8:
            case 0x12C8:
            case 0x12E8:
            //case 0x1300:
            //case 0x1308:
            case 0x1330:
            case 0x1340:
            case 0x1350:
            case 0x1368:
            case 0x1268:
                break;
            case 0x1240:
                pThis->m_pCandidate[pos++] = 0x124B;
                break;
            //case 0x1340:
            //    pThis->m_pCandidate[pos++] = 0x133F;
            //    break;
            case 0x12A8:
                pThis->m_pCandidate[pos++] = 0x12B3;
                break;
            case 0x1308:
                pThis->m_pCandidate[pos++] = 0x1313;
                break;
            default:
                pThis->m_pCandidate[pos++] = (IIME_WCHAR)(c1 + offset);
                break;
            }
        }
        else
            pThis->m_pCandidate[pos++] = (IIME_WCHAR)(c1 + offset);
    }

    pThis->m_iHotCand = 0;
    pThis->m_nCand = pos;

}

static void FillRowCandidate(AmharicProvider *pThis)
{
    int i;
    IIME_WCHAR c1 = pThis->m_pCandidate[pThis->m_iHotCand];

    memset(pThis->m_pCandidate, 0, sizeof(IIME_WCHAR) * MAX_CANDIDATE);
    for (i = 0; i < 7; i++)
        pThis->m_pCandidate[i] = (IIME_WCHAR)(c1 + i);

    switch (c1)
    {
    case 0x1200:
    //case 0x12A0:
    //case 0x12A8:
    case 0x12C8:
    case 0x12E8:
    //case 0x1300:
    //case 0x1308:
    case 0x1330:
    case 0x1350:
    case 0x1368:
    case 0x1340:
    case 0x1268:
        pThis->m_nCand = 7;
        break;
    case 0x1240:
        pThis->m_pCandidate[7] = 0x124B;
        pThis->m_nCand = 8;
        break;
    //case 0x1340:
    //    pThis->m_pCandidate[7] = 0x133F;
    //    pThis->m_nCand = 8;
    //    break;
    case 0x1308:
        pThis->m_pCandidate[7] = 0x1313;
        pThis->m_nCand = 8;
        break;
    case 0x12A8:
        pThis->m_pCandidate[7] = 0x12B3;
        pThis->m_nCand = 8;
        break;
    default:
        pThis->m_pCandidate[7] = (IIME_WCHAR)(c1 + 7);
        pThis->m_nCand = 8;
        break;
    }

    pThis->m_iHotCand = 0;
}

static void IterateCandidate(AmharicProvider *pThis, IIME_WCHAR iMsg)
{
    if (iMsg == IIME_VK_RIGHT)
        pThis->m_iHotCand++;
    if (iMsg == IIME_VK_LEFT)
        pThis->m_iHotCand--;

    if (pThis->m_iHotCand >= pThis->m_nCand)
        pThis->m_iHotCand = 0;

    if (pThis->m_iHotCand < 0)
        pThis->m_iHotCand = pThis->m_nCand - 1;
}

static void ClearCandidate(AmharicProvider *pThis)
{
    pThis->m_nCand = 0;
    pThis->m_iHotCand = 0;
    memset(pThis->m_pCandidate, 0, sizeof(IIME_WCHAR) * MAX_CANDIDATE);

}

static IIME_BOOL CommitByNumber(AmharicProvider *pThis, IIME_WCHAR iMsg)
{
    int pos;
    IIME_WCHAR c = 0;   

    if (iMsg == 0xFFFF)
        pos = pThis->m_iHotCand;
    else
    {
        if (pThis->m_iHotCand < pThis->m_cand_per_page)
            pos = iMsg - IIME_VK_1;
        else
            pos = iMsg - IIME_VK_1 + pThis->m_cand_per_page;
    }

    if (pos < pThis->m_nCand)
    {
        c = pThis->m_pCandidate[pos];
        ClearCandidate(pThis);
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

static IIME_BOOL CandidatesMoreThanWindow(AmharicProvider *pThis)
{
    int i;
    int width = 0;
    for (i = 0; i < pThis->m_nCand; i++)
    {
        unsigned short c = pThis->m_pCandidate[i];
        width += pThis->m_nLableWidth + (pThis->m_pfnGetWidth(&c, 1));

        if (width > pThis->m_nWinWidth)
        {
            pThis->m_cand_per_page = i;
            return IIME_TRUE;
        }
    }

    pThis->m_cand_per_page = pThis->m_nCand;
    return IIME_FALSE;
}

#endif /* GTIDE_PRODUCT */
