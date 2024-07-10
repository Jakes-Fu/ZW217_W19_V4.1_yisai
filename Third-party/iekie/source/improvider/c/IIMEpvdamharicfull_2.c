/****************************************************************************
 *
 * IIMEpvdlatinfull.c
 *
 */

#ifndef IIME_DIGIT_ONLY
#if 0
#include "IIMEstd.h"
#include "provider_com.h"

#include "kmxime.h"
#include "kmxime_latin.h"
#include "IIMEpvdamharicfull_2.h"
#else
#include "IIMEstd.h"
#include "provider_com.h"

#include "kmxime.h"
#include "kmxime_latin.h"
#include "IIMEpvdamharicfull_2.h"
#endif
#define MAX_ITEM_LENGTH 64
#define IIME_VK_CLEARALL	(0x00010000 | IIME_VK_BACKSPACE)

#define MAX_CANDIDATE 8
#define SYMBOL_KEY     0x0F    /* symbol input key value */

typedef enum tagSTATUS
{
    En_Nothing,
    En_Cand,
    En_Symbol
} STATUS;

/*
 Declaration
 */
typedef struct
{
    unsigned short key;
    unsigned short value1[7];
    unsigned short value2[7];
}AMHARIC_KEYMAP;

static const AMHARIC_KEYMAP amharic_keymap[] = 
{
    {'a', {0x12A0,0x12A1,0x12A2,0x12A3,0x12A4,0x12A5,0x12A6}, 0 },
    {'b', {0x1260,0x1261,0x1262,0x1263,0x1264,0x1265,0x1266}, 0 },
    {'c', {0x1278,0x1279,0x127A,0x127B,0x127C,0x127D,0x127E}, {0x1328,0x1329,0x132A,0x132B,0x132C,0x132D,0x132E} },
    {'d', {0x12F0,0x12F1,0x12F2,0x12F3,0x12F4,0x12F5,0x12F6}, 0 },
    {'e', {0x12D0,0x12D1,0x12D2,0x12D3,0x12D4,0x12D5,0x12D6}, 0 },
    {'f', {0x1348,0x1349,0x134A,0x134B,0x134C,0x134D,0x134E}, 0 },
    {'g', {0x1308,0x1309,0x130A,0x130B,0x130C,0x130D,0x130E}, 0 },
    {'h', {0x1200,0x1201,0x1202,0x1203,0x1204,0x1205,0x1206}, {0x1280,0x1281,0x1282,0x1283,0x1284,0x1285,0x1286} },
    {'i', {0x1210,0x1211,0x1212,0x1213,0x1214,0x1215,0x1216}, 0 },
    {'j', {0x1300,0x1301,0x1302,0x1303,0x1304,0x1305,0x1306}, 0 },
    {'k', {0x12A8,0x12A9,0x12AA,0x12AB,0x12AC,0x12AD,0x12AE}, {0x12B8,0x12B9,0x12BA,0x12BB,0x12BC,0x12BD,0x12BE} },
    {'l', {0x1208,0x1209,0x120A,0x120B,0x120C,0x120D,0x120E}, 0 },
    {'m', {0x1218,0x1219,0x121A,0x121B,0x121C,0x121D,0x121E}, 0 },
    {'n', {0x1290,0x1291,0x1292,0x1293,0x1294,0x1295,0x1296}, {0x1298,0x1299,0x129A,0x129B,0x129C,0x129D,0x129E} },
    {'o', {0x12A6}, 0 },
    {'p', {0x1350,0x1351,0x1352,0x1353,0x1354,0x1355,0x1356}, {0x1330,0x1331,0x1332,0x1333,0x1334,0x1335,0x1336} },
    {'q', {0x1240,0x1241,0x1242,0x1243,0x1244,0x1245,0x1246}, 0 },
    {'r', {0x1228,0x1229,0x122A,0x122B,0x122C,0x122D,0x122E}, 0 },
    {'s', {0x1230,0x1231,0x1232,0x1233,0x1234,0x1235,0x1236}, 0 },
    {'t', {0x1270,0x1271,0x1272,0x1273,0x1274,0x1275,0x1276}, {0x1320,0x1321,0x1322,0x1323,0x1324,0x1325,0x1326} },
    {'u', {0x12E9}, 0 },
    {'v', {0x1268,0x1269,0x126A,0x126B,0x126C,0x126D,0x126E}, 0 },
    {'w', {0x12C8,0x12C9,0x12CA,0x12CB,0x12CC,0x12CD,0x12CE}, 0 },
    {'x', {0x1238,0x1239,0x123A,0x123B,0x123C,0x123D,0x123E}, 0 },
    {'y', {0x12E8,0x12E9,0x12EA,0x12EB,0x12EC,0x12ED,0x12EE}, 0 },
    {'z', {0x12D8,0x12D9,0x12DA,0x12DB,0x12DC,0x12DD,0x12DE}, {0x12E0,0x12E1,0x12E2,0x12E3,0x12E4,0x12E5,0x12E6} },
    {'$', {0x1338,0x1339,0x133A,0x133B,0x133C,0x133D,0x133E}, {0x1340,0x1341,0x1342,0x1343,0x1344,0x1345,0x1346} },
    0
};

static const unsigned short Amharic_Symbol[] = 
{
    0x1361, 0x1362, 0x1368, 0x1360, 0x1363, 0x1365, 0x1364, 0x1373, 
    0x1373, 0x1374, 0x1375, 0x1376, 0x1377, 0x1378, 0x1379, 0x137A, 
    0x137B, 0x003F, 0x0021, 0x0027, 0x0022, 0x0040, 0x003A, 0x005F, 
    0x003B, 0x002B, 0x002D, 0x002A, 0x003D, 0x0025, 0x0026, 0x003C, 
    0x003E, 0x0024, 0x007e, 0x0023
};

typedef struct tagAmharicFullProvider2
{
    DECLARE_PROVIDER_INTERFACE()

    IIME_INT          m_idMethod;

    STATUS          m_iStatus;

    IIME_WCHAR        m_pCandidate[MAX_CANDIDATE];
    int             m_iHotCand;
    int             m_nCand;
    int             m_cand_per_page;
    int             m_nCurPage;
    int             m_nTotalPage;

    IIME_WCHAR        m_chPre;
    IIME_WCHAR        m_chPreNumber;

    int             m_nWinWidth;           /* [in] Candidate window width*/
    int             m_nLableWidth;         /* [in] Lable width*/

    IIME_WCHAR        m_cUnderLine;
    IIME_WCHAR        m_cCommit;
    int             m_nCount;

} AmharicFullProvider2;

/*
 Declaration
 */

DECLARE_PROVIDER_FUNCTIONS(IIMEAmharicFullProvider2)


static IIME_RESULT Amharic_KeyHandle(AmharicFullProvider2 *pThis, IIME_UINT iMsg);
static IIME_RESULT Amharic_CharHandle(AmharicFullProvider2 *pThis, IIME_UINT iMsg);

static IIME_RESULT Amharic_Commit(AmharicFullProvider2 *pThis, int idx);
static IIME_BOOL Amharic_IsValidateInput(AmharicFullProvider2 *pThis, IIME_WCHAR ch);

static int Amharic_FillCandidate(AmharicFullProvider2 *pThis);
static void Amharic_ResetPageInfo(AmharicFullProvider2 *pThis);
static void Amharic_Reset(AmharicFullProvider2 *pThis);

static IIME_RESULT Amharic_AddChar(AmharicFullProvider2 *pThis);
static IIME_RESULT Amharic_DeleteChar(AmharicFullProvider2 *pThis);
static IIME_RESULT Amharic_DeleteAll(AmharicFullProvider2 *pThis);
static IIME_RESULT Amharic_NaviCandidate(AmharicFullProvider2 *pThis, IIME_UINT iMsg);

static void Amharic_GetSymList(AmharicFullProvider2 *pThis, LDB_CHAR* pMultiList, int* pCount);


/*
 Constructor an destructor
 */
Provider_Com* IIMEAmharicFullProvider2_Create(
    PIIME_IMM_PARAMS          pImmParams,
    PIIME_USER_PARAMS         pUserParam1,
    PIIME_USER_PARAMS         pUserParam2,
    const void*             pdbMisc,
	const void*				pdbMO,
    const IIME_MULTI_STRING_TABLE* pMulti_string_table,
    IIME_INT                  idMethod)
{
    IIME_WCHAR chAmharic = 0x132E;
    int      width_amharic = 0;

	AmharicFullProvider2 *pThis = IIME_MALLOC(sizeof(AmharicFullProvider2));
	memset(pThis, 0, sizeof(AmharicFullProvider2));

   	/* initialize interface functions*/
	INITIALIZE_PROVIDER_INTERFACE(IIMEAmharicFullProvider2);

	pThis->m_idMethod = idMethod;

	pThis->m_nWinWidth = pImmParams->nWinWidth;
    pThis->m_nLableWidth = pImmParams->nLableWidth;

    width_amharic = (IIME_WORD)(pImmParams->pGetTextWidthFunc(&chAmharic, 1));
    width_amharic = (width_amharic > 8) ? width_amharic : 8;

    pThis->m_cand_per_page = pImmParams->nWinWidth / (width_amharic + pImmParams->nLableWidth);
    if (pThis->m_cand_per_page > MAX_CANDIDATE)
        pThis->m_cand_per_page = MAX_CANDIDATE;

	return (Provider_Com*)pThis;
}

void IIMEAmharicFullProvider2_Delete(
    Provider_Com*           pProvider)
{
	AmharicFullProvider2 *pThis = (AmharicFullProvider2 *)pProvider;
	if (pThis)
	{
		IIME_FREE(pThis);
	}

	return;
}


/*
 interface
 */

static IIME_RESULT EventHandle_IIMEAmharicFullProvider2(Provider_Com *pProvider,
                                   IIME_UINT hEvent,
                                   IIME_UINT iMsg,
                                   IIME_IMM_CONTENT *pContent)
{
	IIME_RESULT ret = IIME_ERROR_UNUSED;
	AmharicFullProvider2 *pThis = (AmharicFullProvider2 *)pProvider;

	memset(pContent, 0, sizeof(IIME_IMM_CONTENT));

    if (hEvent == IIME_EVENT_KEYBOARD)
    {
        ret = Amharic_KeyHandle(pThis, (IIME_WCHAR)iMsg);
    }
    else if (hEvent == IIME_EVENT_CHAR)
    {
        ret = Amharic_CharHandle(pThis, (IIME_WCHAR)iMsg);
    }
    else
        ret = IIME_ERROR_UNKNOWN;

    if (pThis->m_cCommit)
    {
        pContent->pCommit[0] = pThis->m_cCommit;
        pContent->pCommit[1] = 0;

        pThis->m_cCommit = 0;
    }

    if (pThis->m_nCand)
    {
        int i;

        for (i = 0; i < pThis->m_nCand; i++)
            pContent->pCandBuffer[i*2] = pThis->m_pCandidate[i];

        pContent->iHotCand = pThis->m_iHotCand;
        pContent->nCand = pThis->m_nCand;

        if (pThis->m_nCurPage > 0)
            pContent->flagPageCand |= IIME_FLAG_PAGEDOWN;
        if (pThis->m_nCurPage < pThis->m_nTotalPage - 1)
            pContent->flagPageCand |= IIME_FLAG_PAGEUP;
    }

    return ret;
}

static IIME_RESULT SetOptions_IIMEAmharicFullProvider2(Provider_Com *pProvider,
                                  const IIME_IMM_OPTIONS *pOption)
{

    return IIME_ERROR_SUCCESS;
}

static IIME_RESULT GetOptions_IIMEAmharicFullProvider2(Provider_Com *pProvider,
                                  IIME_IMM_OPTIONS *pOption)
{
    return IIME_ERROR_SUCCESS;
}

static IIME_RESULT AddPhrase_IIMEAmharicFullProvider2(Provider_Com* pProvider, unsigned short* szPhrase, int nLen)
{
    return IIME_ERROR_UNKNOWN;
}

static IIME_RESULT CheckAsso_IIMEAmharicFullProvider2(Provider_Com* pProvider, unsigned short wContext, unsigned short* szAsso, int* pNumber)
{
    return IIME_ERROR_UNKNOWN;
}

static IIME_RESULT QueryProperties_IIMEAmharicFullProvider2(Provider_Com* pProvider, int* pProperties)
{
    return IIME_ERROR_UNKNOWN;
}


/*
 implementions
 */

static IIME_RESULT Amharic_KeyHandle(AmharicFullProvider2 *pThis, IIME_UINT iMsg)
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
        ret = Amharic_Commit(pThis, iMsg - IIME_VK_1);
        break;
	case IIME_VK_LEFT:
	case IIME_VK_RIGHT:
	case IIME_VK_UP:
	case IIME_VK_DOWN:
		ret = Amharic_NaviCandidate(pThis, iMsg);
		break;
	case IIME_VK_BACKSPACE:
		ret = Amharic_DeleteChar(pThis);
		break;
	case IIME_VK_CLEARALL:
		ret = Amharic_DeleteAll(pThis);
		break;
	case IIME_VK_SELECT:
		ret = Amharic_Commit(pThis, -1);
		break;
    case IIME_VK_POUND:
        Amharic_Reset(pThis);
        ret = IIME_ERROR_UNUSED;
        break;
	default:
		Amharic_Commit(pThis, -1);
		ret = IIME_ERROR_UNUSED;
		break;
	}

	return ret;
}

static IIME_RESULT Amharic_CharHandle(AmharicFullProvider2 *pThis, IIME_UINT iMsg)
{
	IIME_RESULT ret = IIME_ERROR_UNUSED;

	LDB_CHAR chr = (LDB_CHAR)iMsg;
	if (Amharic_IsValidateInput(pThis, chr))
	{

        if (pThis->m_chPre == chr){
            pThis->m_chPreNumber++;
        }
        else if (chr == SYMBOL_KEY)
        {
            Amharic_Commit(pThis, -1);
        }

        pThis->m_chPre = chr;

        pThis->m_iHotCand = 0;
        pThis->m_nCurPage = 0;
        pThis->m_nCand = Amharic_FillCandidate(pThis);
        if (pThis->m_nCand == 1)
            Amharic_Commit(pThis, -1);

        ret = IIME_ERROR_USED;
    }
	else
	{
		Amharic_Commit(pThis, -1);
		ret = IIME_ERROR_COMMIT;
	}

	return ret;
}

static IIME_RESULT Amharic_Commit(AmharicFullProvider2 *pThis, int idx)
{
	IIME_RESULT ret = IIME_ERROR_UNUSED;

    if (idx != -1)
    {
        if (idx < pThis->m_nCand)
        {
            pThis->m_iHotCand = (unsigned short)idx - 1;
        }
        else
            return ret;
    }

    if (pThis->m_nCand)
    {

        pThis->m_cCommit = pThis->m_pCandidate[pThis->m_iHotCand];

	    Amharic_Reset(pThis);

        ret = IIME_ERROR_USED;
    }

    return ret;
}

static IIME_RESULT Amharic_NaviCandidate(AmharicFullProvider2 *pThis, IIME_UINT iMsg)
{
	IIME_RESULT ret = IIME_ERROR_USED;

    if (pThis->m_nCand == 0)
		return IIME_ERROR_UNUSED;

	switch (iMsg)
	{
	case IIME_VK_RIGHT:
        if (pThis->m_iHotCand < pThis->m_nCand - 1)
		{
            pThis->m_iHotCand++;
		}
        else if (pThis->m_nCurPage < pThis->m_nTotalPage - 1)
		{
			pThis->m_nCurPage++;
            pThis->m_iHotCand = 0;
			Amharic_FillCandidate(pThis);
		}
		break;
	case IIME_VK_LEFT:
        if (pThis->m_iHotCand > 0)
		{
			pThis->m_iHotCand--;
		}
		else if (pThis->m_nCurPage > 0)
		{
			pThis->m_nCurPage--;
            pThis->m_iHotCand = pThis->m_cand_per_page - 1;
			Amharic_FillCandidate(pThis);
		}
		break;
	case IIME_VK_UP:
		if (pThis->m_nCurPage > 0)
		{
			pThis->m_nCurPage--;
            pThis->m_iHotCand = 0;
			Amharic_FillCandidate(pThis);
		}
		break;
	case IIME_VK_DOWN:
        if (pThis->m_nCurPage < pThis->m_nTotalPage - 1)
		{
			pThis->m_nCurPage++;
            pThis->m_iHotCand = 0;
			Amharic_FillCandidate(pThis);
		}
		break;
	default:
		ret = IIME_ERROR_UNUSED;
		break;
	}
	
	return ret;
}

static IIME_RESULT Amharic_DeleteChar(AmharicFullProvider2 *pThis)
{
	IIME_RESULT ret;

    if (pThis->m_nCand > 0)
    {
        Amharic_Reset(pThis);
        ret = IIME_ERROR_USED;
    }
    else
        ret = IIME_ERROR_UNUSED;

    return ret;
}

static IIME_RESULT Amharic_DeleteAll(AmharicFullProvider2 *pThis)
{
	IIME_RESULT ret;

    if (pThis->m_nCand > 0)
    {
        Amharic_Reset(pThis);
        ret = IIME_ERROR_USED;
    }
    else
        ret = IIME_ERROR_UNUSED;

    return ret;
}


static void Amharic_ResetPageInfo(AmharicFullProvider2 *pThis)
{
    pThis->m_nCand = 0;
    pThis->m_nTotalPage = 0;
    pThis->m_nCurPage = 0;
    pThis->m_iHotCand = 0;
}

static void Amharic_Reset(AmharicFullProvider2 *pThis)
{
	Amharic_ResetPageInfo(pThis);
    pThis->m_chPre = 0;
    pThis->m_chPreNumber = 0;

	memset(pThis->m_pCandidate, 0, sizeof(IIME_WCHAR) * (MAX_CANDIDATE));
}

static IIME_BOOL Amharic_IsValidateInput(AmharicFullProvider2 *pThis, IIME_WCHAR ch)
{
    if (ch >= 'a' && ch <= 'z')
        return IIME_TRUE;
    if (ch == '$' || ch == SYMBOL_KEY)
        return IIME_TRUE;

    return IIME_FALSE;
}

static int Amharic_FillCandidate(AmharicFullProvider2 *pThis)
{
    unsigned short key = pThis->m_chPre;
    const unsigned short *pValue = 0;
    int num = 0;

    if (key == SYMBOL_KEY){
        pValue = Amharic_Symbol;
        num = sizeof(Amharic_Symbol) / sizeof (unsigned short);
    }
    else {
        const AMHARIC_KEYMAP *pKeymap = amharic_keymap;
        const unsigned short *pValue1 = 0;
        const unsigned short *pValue2 = 0;

        while ((pKeymap->key) != 0){
            if (key == pKeymap->key) {
                pValue1 = pKeymap->value1;
                pValue2 = pKeymap->value2;
                break;
            }
            pKeymap++;
        }
        if (*pValue1){
            pValue = pValue1;
            if (pValue1[1] != 0)
                num = 7;
            else
                num = 1;
        }
        if (*pValue2 && pThis->m_chPreNumber % 2){
            pValue = pValue2;
            num = 7;
        }
    }

    if (num > 0)
    {
        int beg, end, i;
        pThis->m_nTotalPage = (num  - 1) / pThis->m_cand_per_page + 1;

        beg = pThis->m_cand_per_page * pThis->m_nCurPage;
        end = beg + pThis->m_cand_per_page;
        if (end > num)
            end = num;
        
        pThis->m_nCand = 0;
        for (i = beg; i < end; i++)
            pThis->m_pCandidate[pThis->m_nCand++] = pValue[i];

        return pThis->m_nCand;
    }
    else
        return 0;
}

#endif

