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
#include "IIMEpvdlatinfull.h"
#else
#include "IIMEstd.h"
#include "provider_com.h"

#include "kmxime.h"
#include "kmxime_latin.h"
//#include "IIMEpvdlatinfull.h"//ZHMCH
#include "IIMEpvdamharicfull.h"//ZHMCH
#endif
#ifdef SPREADTRUM_SPECIAL_DOLLAR
#include "qwerty_keymap_dollar.h"
#else
#include "qwerty_keymap.h"
#endif

//daodejing
#if defined(KMX_LDB_HINDI) || defined(KMX_LDB_BENGALI) || defined(KMX_LDB_TELUGU) || defined(KMX_LDB_MARATHI) \
    || defined(KMX_LDB_TAMIL) || defined(KMX_LDB_PUNJABI) || defined(KMX_LDB_GUJARATI) || defined(KMX_LDB_ORIYA) \
    || defined(KMX_LDB_KANNADA) || defined(KMX_LDB_MALAYALAM) || defined(KMX_LDB_KHMER) || defined(KMX_LDB_LAO) \
    || defined(KMX_LDB_MYANMAR) || defined(KMX_LDB_NEPALI)|| defined(KMX_LDB_SINHALA) || defined(KMX_LDB_BODO) \
    || defined(KMX_LDB_DOGRI) || defined(KMX_LDB_SANSKRIT) || defined(KMX_LDB_KASHMIRI) || defined(KMX_LDB_KONKANI) \
    || defined(KMX_LDB_MAITHILI) || defined(KMX_LDB_MANIPURI) || defined(KMX_LDB_SANTALI) || defined(KMX_LDB_SOMALI) || defined(KMX_LDB_SINDHI)
#include "indic_mt.h"
#endif

#define SYMBOL_KEY     0x0F    /* symbol input key value */

#define MAX_ITEM_LENGTH 64
#define IIME_VK_CLEARALL	(0x00010000 | IIME_VK_BACKSPACE)

/*
 Declaration
 */


DECLARE_PROVIDER_FUNCTIONS(IIMEAmharicFullProvider)

static IIME_RESULT EventHandle_Smart(IIMEAmharicFullProvider *pProvider,
                                   IIME_UINT hEvent,
                                   IIME_UINT iMsg,
                                   IIME_IMM_CONTENT *pContent);

static IIME_RESULT EventHandle_Multitap(IIMEAmharicFullProvider *pProvider,
                                   IIME_UINT hEvent,
                                   IIME_UINT iMsg,
                                   IIME_IMM_CONTENT *pContent);

static int PostProcess(unsigned short *pBuffer, IIMEAmharicFullProvider *pProvider);

static IIME_RESULT Smart_KeyHandle(IIMEAmharicFullProvider *pThis, IIME_UINT iMsg);
static IIME_RESULT Smart_CharHandle(IIMEAmharicFullProvider *pThis, IIME_UINT iMsg);
static IIME_RESULT Smart_TimerHandle(IIMEAmharicFullProvider *pThis);

static IIME_RESULT Multitap_KeyHandle(IIMEAmharicFullProvider *pThis, IIME_UINT iMsg);
static IIME_RESULT Multitap_CharHandle(IIMEAmharicFullProvider *pThis, IIME_UINT iMsg);
static IIME_RESULT Multitap_TimerHandle(IIMEAmharicFullProvider *pThis);

static IIME_RESULT Smart_Commit(IIMEAmharicFullProvider *pThis, int idx);
static IIME_RESULT Multitap_Commit(IIMEAmharicFullProvider *pThis, int idx);

static void LatinFull_FillCandidate(IIMEAmharicFullProvider *pThis);
static void LatinFull_ClearCandidate(IIMEAmharicFullProvider *pThis);
static void LatinFull_ResetPageInfo(IIMEAmharicFullProvider *pThis);
static void LatinFull_Recognize(IIMEAmharicFullProvider *pThis);
static void LatinFull_ResetMultiKey(IIMEAmharicFullProvider *pThis);
static void LatinFull_Reset(IIMEAmharicFullProvider *pThis);

static IIME_RESULT Smart_AddChar(IIMEAmharicFullProvider *pThis);
static IIME_RESULT Smart_DeleteChar(IIMEAmharicFullProvider *pThis);
static IIME_RESULT Smart_DeleteAll(IIMEAmharicFullProvider *pThis);
static IIME_RESULT Smart_NaviCandidate(IIMEAmharicFullProvider *pThis, IIME_UINT iMsg);
static void Smart_CheckCase(IIMEAmharicFullProvider *pThis);

static void LatinFull_GetSymList(IIMEAmharicFullProvider *pThis, LDB_CHAR* pMultiList, int* pCount);

static const QWERTY_KEYMAP* LatinFull_FindKeymap(IIMEAmharicFullProvider *pThis, const IIME_QWERTY_INFO *pQwertyInfo);
static int LatinFull_KeymapKey(IIMEAmharicFullProvider *pThis, unsigned short key);
static const unsigned short * LatinFull_KeymapValue(IIMEAmharicFullProvider *pThis, unsigned short key);

/* for english, Hinglish, Indonesian, malay, swahili*/
static void Basic_GetMultitype(
    IIMEAmharicFullProvider     *pThis, 
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
	OUT LDB_CHAR*			pMultiList,
    OUT int*                count);

static int Basic_ValidateInput(IIMEAmharicFullProvider *pThis, LDB_CHAR c);

#ifdef KMX_LDB_THAI
static void Thai_GetMultitype(
    IIMEAmharicFullProvider     *pThis, 
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
	OUT LDB_CHAR*			pMultiList,
    OUT int*                count);

static int Thai_ValidateInput(IIMEAmharicFullProvider *pThis, LDB_CHAR c);
#endif /* KMX_LDB_THAI */

#if defined(KMX_LDB_ARABIC) || defined(KMX_LDB_HEBREW) || defined(KMX_LDB_URDU) || defined(KMX_LDB_PERSIAN) || defined(KMX_LDB_UYGHUR)
static void Arabic_Hebrew_GetMultitype(
    IIMEAmharicFullProvider     *pThis, 
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
	OUT LDB_CHAR*			pMultiList,
    OUT int*                count);

static int Arabic_Hebrew_ValidateInput(IIMEAmharicFullProvider *pThis, LDB_CHAR c);
#endif /* defined(KMX_LDB_ARABIC) || defined(KMX_LDB_HEBREW) || defined(KMX_LDB_URDU) || defined(KMX_LDB_PERSIAN) || defined(KMX_LDB_UYGHUR) */

#if defined(KMX_LDB_ARABIC)
static int Arabic_Normalize(unsigned short *pBuffer, IIMEAmharicFullProvider *pThis);
#endif /* KMX_LDB_ARABIC */

#if defined(KMX_LDB_ARMENIAN) || defined(KMX_LDB_BULGARIAN) || defined(KMX_LDB_CZECH) || defined(KMX_LDB_DANISH) || \
    defined(KMX_LDB_FINNISH) || defined(KMX_LDB_FRENCH) || defined(KMX_LDB_GERMAN) || defined(KMX_LDB_GREEK) || \
    defined(KMX_LDB_HUNGARIAN) || defined(KMX_LDB_ITALIAN) || defined(KMX_LDB_LITHUANIAN) || defined(KMX_LDB_NORWEGIAN) || \
    defined(KMX_LDB_POLISH) || defined(KMX_LDB_PORTUGUESE) || defined(KMX_LDB_ROMANIAN) || defined(KMX_LDB_RUSSIAN) || \
    defined(KMX_LDB_SLOVAK) || defined(KMX_LDB_SLOVENIAN) || defined(KMX_LDB_SPANISH) || defined(KMX_LDB_SWEDISH) || \
    defined(KMX_LDB_TURKISH) || defined(KMX_LDB_UKRAINIAN)
static void Latin2_GetMultitype(
    IIMEAmharicFullProvider     *pThis, 
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
	OUT LDB_CHAR*			pMultiList,
    OUT int*                count);

static int Latin2_ValidateInput(IIMEAmharicFullProvider *pThis, LDB_CHAR c);
static int Latin2_Normalize(unsigned short *pBuffer, IIMEAmharicFullProvider *pThis);

#endif /* defined(KMX_LDB_ARMENIAN) || defined(KMX_LDB_BULGARIAN) */

#if defined(KMX_LDB_VIETNAMESE)
static void Latin3_GetMultitype(
    IIMEAmharicFullProvider     *pThis, 
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
	OUT LDB_CHAR*			pMultiList,
    OUT int*                count);

static int Latin3_ValidateInput(IIMEAmharicFullProvider *pThis, LDB_CHAR c);
static int Vietnamese_Normalize(unsigned short *pBuffer, IIMEAmharicFullProvider *pThis);
#endif

//daodejing
#if defined(KMX_LDB_HINDI) || defined(KMX_LDB_BENGALI) || defined(KMX_LDB_TELUGU) || defined(KMX_LDB_MARATHI) \
    || defined(KMX_LDB_TAMIL) || defined(KMX_LDB_PUNJABI) || defined(KMX_LDB_GUJARATI) || defined(KMX_LDB_ORIYA) \
    || defined(KMX_LDB_KANNADA) || defined(KMX_LDB_MALAYALAM) || defined(KMX_LDB_KHMER) || defined(KMX_LDB_LAO) \
    || defined(KMX_LDB_MYANMAR)|| defined(KMX_LDB_ASSAMESE)||defined(KMX_LDB_NEPALI)||defined(KMX_LDB_SINHALA)||defined(KMX_LDB_BODO) \
    || defined(KMX_LDB_DOGRI)|| defined(KMX_LDB_SANSKRIT)|| defined(KMX_LDB_KASHMIRI)|| defined(KMX_LDB_KONKANI)|| defined(KMX_LDB_MAITHILI) \
    || defined(KMX_LDB_MANIPURI)|| defined(KMX_LDB_SANTALI)|| defined(KMX_LDB_SOMALI)|| defined(KMX_LDB_SINDHI)

static void Indic_GetMultitype(
    IIMEAmharicFullProvider     *pThis, 
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
	OUT LDB_CHAR*			pMultiList,
    OUT int*                count);

static int Indic_ValidateInput(
    IIMEAmharicFullProvider *pThis, 
    LDB_CHAR c);
#endif

#if defined(KMX_LDB_AMHARIC)
static void Amharic_GetMultitype(
    IIMEAmharicFullProvider     *pThis, 
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
	OUT LDB_CHAR*			pMultiList,
    OUT int*                count);

static int Amharic_ValidateInput(IIMEAmharicFullProvider *pThis, LDB_CHAR c);
#endif

/*
 Constructor an destructor
 */

Provider_Com* IIMEAmhaircFullProvider_Create(
    PIIME_IMM_PARAMS          pImmParams,
    PIIME_USER_PARAMS         pUserParam1,
    PIIME_USER_PARAMS         pUserParam2,
    const void*             pdbMisc,
	const void*				pdbMO,
    const IIME_MULTI_STRING_TABLE* pMulti_string_table,
    IIME_INT                  idMethod)
{
    LDB_INFO            ldb_info;
	KMX_IME_ENGINE_INIT	init_info;

	IIMEAmharicFullProvider *pThis = IIME_MALLOC(sizeof(IIMEAmharicFullProvider));
	memset(pThis, 0, sizeof(IIMEAmharicFullProvider));

   	/* initialize interface functions*/
	INITIALIZE_PROVIDER_INTERFACE(IIMEAmharicFullProvider);

	pThis->m_idMethod = idMethod;
	pThis->m_nWinWidth = pImmParams->nWinWidth;
    pThis->m_nLableWidth = pImmParams->nLableWidth;
	pThis->m_pGetTextWidthFunc = pImmParams->pGetTextWidthFunc;

	pThis->m_Engine.engine = IIME_MALLOC(16 * 1024);

    memset(&init_info, 0, sizeof(init_info));
	init_info.database = pdbMisc;
	init_info.user_data = pUserParam1->pUserCache;
	init_info.user_data_size = pUserParam1->iUserCacheSize;
	init_info.flags = 0;

    pThis->pdbMisc = pdbMisc;

    switch(idMethod - IM_FULLKEY_BASE)
    {
    case IM_TELUGU_MT:
        pThis->m_language = IIME_LANG_TELUGU;
        break;
    case IM_MARATHI_MT:
        pThis->m_language = IIME_LANG_MARATHI;
        break;
    case IM_TAMIL_MT:
        pThis->m_language = IIME_LANG_TAMIL;
        break;
    case IM_PUNJABI_MT:
        pThis->m_language = IIME_LANG_PUNJABI;
        break;
    case IM_GUJARATI_MT:
        pThis->m_language = IIME_LANG_GUJARATI;
        break;
    case IM_ORIYA_MT:
        pThis->m_language = IIME_LANG_ORIYA;
        break;
    case IM_KANNADA_MT:
        pThis->m_language = IIME_LANG_KANNADA;
        break;
    case IM_MALAYALAM_MT:
        pThis->m_language = IIME_LANG_MALAYALAM;
        break;
    case IM_KHMER_MT:
        pThis->m_language = IIME_LANG_KHMER;
        break;
    case IM_LAO_MT:
        pThis->m_language = IIME_LANG_LAO;
        break;
    case IM_AMHARIC_MT:
        pThis->m_language = IIME_LANG_AMHARIC;
        break;
    case IM_MYANMAR_MT:
        pThis->m_language = IIME_LANG_MYANMAR;
        break;
    default:
        kmx_get_db_info(pdbMisc, &ldb_info);
        pThis->m_language = ldb_info.language;
    }

    switch(pThis->m_language)
    {
    case IIME_LANG_THAI:
    case IIME_LANG_HEBREW:
    case IIME_LANG_PERSIAN:
    case IIME_LANG_ARABIC:
    case IIME_LANG_HINDI:
    case IIME_LANG_BENGALI:
    case IIME_LANG_HANGUL:
    case IIME_LANG_UYGHUR:
    case IIME_LANG_URDU:
    case IIME_LANG_TELUGU:
    case IIME_LANG_MARATHI:
    case IIME_LANG_TAMIL:
    case IIME_LANG_PUNJABI:
    case IIME_LANG_GUJARATI:
    case IIME_LANG_ORIYA:
    case IIME_LANG_KANNADA:
    case IIME_LANG_MALAYALAM:
    case IIME_LANG_KHMER:
    case IIME_LANG_LAO:
    case IIME_LANG_AMHARIC:
    case IIME_LANG_MYANMAR:
    case IIME_LANG_NEPALI:
    case IIME_LANG_SINHALA:
    case IIME_LANG_BODO://daodejing
    case IIME_LANG_DOGRI:
	case IIME_LANG_SANSKRIT:
	case IIME_LANG_KASHMIRI:
	case IIME_LANG_KONKANI:
	case IIME_LANG_MAITHILI:
	case IIME_LANG_MANIPURI:
	case IIME_LANG_SANTALI:
	case IIME_LANG_SOMALI:
	case IIME_LANG_SINDHI:
    case IIME_LANG_ASSAMESE:
        if ((idMethod & ~(IM_FULLKEY_BASE)) % 10 == 1)
            pThis->m_flagFinish = 1;
        else
            pThis->m_flagFinish = 0;
        break;
    default:
        switch ((idMethod & ~(IM_FULLKEY_BASE))% 10)
        {
        case 1:
        case 7:
        case 8:
        case 9:
            pThis->m_flagFinish = 1;
            break;
        default:
            pThis->m_flagFinish = 0;
            break;
        }
    }

    switch ((idMethod & ~(IM_FULLKEY_BASE)) % 10)
    {
    case 1:
        pThis->m_flagCase = KMX_LATIN_INITIAL_CASE;
        break;
    case 2:
        pThis->m_flagCase = KMX_LATIN_LOWER_CASE;
        break;
    case 3:
        pThis->m_flagCase = KMX_LATIN_UPPER_CASE;
        break;
    case 4:
        pThis->m_flagCase = KMX_LATIN_INITIAL_CASE;
        pThis->m_bCapLock = IIME_TRUE;
        break;
    case 5:
        pThis->m_flagCase = KMX_LATIN_INITIAL_CASE;
        break;
    case 6:
        pThis->m_flagCase = 0;
        break;
    case 7:
        pThis->m_flagCase = KMX_LATIN_INITIAL_CASE;
        break;
    case 8:
        pThis->m_flagCase = KMX_LATIN_UPPER_CASE;
        break;
    case 9:
        pThis->m_flagCase = KMX_LATIN_LOWER_CASE;
        break;
    }

    switch(pThis->m_language)
    {
    case IIME_LANG_HEBREW:
    case IIME_LANG_ARABIC:
    case IIME_LANG_PERSIAN:
    case IIME_LANG_UYGHUR:
    case IIME_LANG_URDU:
        pThis->m_flagRTL = 1;
        break;
    default:
        pThis->m_flagRTL= 0;
    }

	switch (pThis->m_language)
	{
//daodejing
#if defined(KMX_LDB_HINDI) || defined(KMX_LDB_BENGALI) || defined(KMX_LDB_TELUGU) || \
    defined(KMX_LDB_MARATHI) || defined(KMX_LDB_TAMIL) || defined(KMX_LDB_PUNJABI)|| \
    defined(KMX_LDB_GUJARATI) || defined(KMX_LDB_ORIYA) || defined(KMX_LDB_KANNADA) || \
    defined(KMX_LDB_MALAYALAM) || defined(KMX_LDB_KHMER) || defined(KMX_LDB_LAO) || \
    defined(KMX_LDB_MYANMAR) || defined(KMX_LDB_ASSAMESE)||defined(KMX_LDB_NEPALI)||defined(KMX_LDB_SINHALA)||defined(KMX_LDB_BODO) || \
    defined(KMX_LDB_DOGRI) ||defined(KMX_LDB_SANSKRIT) ||defined(KMX_LDB_KASHMIRI) ||defined(KMX_LDB_KONKANI) || \
    defined(KMX_LDB_MAITHILI) ||defined(KMX_LDB_MANIPURI) ||defined(KMX_LDB_SANTALI) ||defined(KMX_LDB_SOMALI) ||defined(KMX_LDB_SINDHI)
	case IIME_LANG_HINDI:
    case IIME_LANG_BENGALI:
    case IIME_LANG_TELUGU:
    case IIME_LANG_MARATHI:
    case IIME_LANG_TAMIL:
    case IIME_LANG_PUNJABI:
    case IIME_LANG_GUJARATI:
    case IIME_LANG_ORIYA:
    case IIME_LANG_KANNADA:
    case IIME_LANG_MALAYALAM:
    case IIME_LANG_KHMER:
    case IIME_LANG_LAO:
    case IIME_LANG_MYANMAR:
    case IIME_LANG_NEPALI:
    case IIME_LANG_SINHALA:
    case IIME_LANG_BODO://daodejing
    case IIME_LANG_DOGRI:
	case IIME_LANG_SANSKRIT:
	case IIME_LANG_KASHMIRI:
	case IIME_LANG_KONKANI:
	case IIME_LANG_MAITHILI:
	case IIME_LANG_MANIPURI:
	case IIME_LANG_SANTALI:
	case IIME_LANG_SOMALI:
	case IIME_LANG_SINDHI:
    case IIME_LANG_ASSAMESE:
		pThis->m_pfnGetMultiType = Indic_GetMultitype;
		pThis->m_pfnIsValidateInput = Indic_ValidateInput;
		break;
#endif

#ifdef KMX_LDB_THAI
	case IIME_LANG_THAI:
		pThis->m_pfnGetMultiType = Thai_GetMultitype;
		pThis->m_pfnIsValidateInput = Thai_ValidateInput;
		break;
#endif

#if defined(KMX_LDB_ARABIC) || defined(KMX_LDB_HEBREW) || defined(KMX_LDB_URDU) || defined(KMX_LDB_PERSIAN) || defined(KMX_LDB_UYGHUR)
	case IIME_LANG_ARABIC:
    case IIME_LANG_HEBREW:
    case IIME_LANG_URDU:
    case IIME_LANG_PERSIAN:
    case IIME_LANG_UYGHUR:
		pThis->m_pfnGetMultiType = Arabic_Hebrew_GetMultitype;
		pThis->m_pfnIsValidateInput = Arabic_Hebrew_ValidateInput;
		break;
#endif

#if defined(KMX_LDB_ARMENIAN) || defined(KMX_LDB_BULGARIAN) || defined(KMX_LDB_CZECH) || defined(KMX_LDB_DANISH) || \
    defined(KMX_LDB_FINNISH) || defined(KMX_LDB_FRENCH) || defined(KMX_LDB_GERMAN) || defined(KMX_LDB_GREEK) || \
    defined(KMX_LDB_HUNGARIAN) || defined(KMX_LDB_ITALIAN) || defined(KMX_LDB_LITHUANIAN) || defined(KMX_LDB_NORWEGIAN) || \
    defined(KMX_LDB_POLISH) || defined(KMX_LDB_PORTUGUESE) || defined(KMX_LDB_ROMANIAN) || defined(KMX_LDB_RUSSIAN) || \
    defined(KMX_LDB_SLOVAK) || defined(KMX_LDB_SLOVENIAN) || defined(KMX_LDB_SPANISH) || defined(KMX_LDB_SWEDISH) || \
    defined(KMX_LDB_TURKISH) || defined(KMX_LDB_UKRAINIAN)
    case IIME_LANG_ARMENIAN:
    case IIME_LANG_BULGARIAN:
    case IIME_LANG_CZECH:
    case IIME_LANG_DANISH:
    case IIME_LANG_FINNISH:
    case IIME_LANG_FRENCH:
    case IIME_LANG_GERMAN:
    case IIME_LANG_GREEK:
    case IIME_LANG_HUNGARIAN:
    case IIME_LANG_ITALIAN:
    case IIME_LANG_LITHUANIAN:
    case IIME_LANG_NORWEGIAN:
    case IIME_LANG_POLISH:
    case IIME_LANG_PORTUGUESE:
    case IIME_LANG_ROMANIAN:
    case IIME_LANG_RUSSIAN:
    case IIME_LANG_SLOVAK:
    case IIME_LANG_SLOVENIAN:
    case IIME_LANG_SPANISH:
    case IIME_LANG_SWEDISH:
    case IIME_LANG_TURKISH:
    case IIME_LANG_UKRAINIAN:
        pThis->m_pfnGetMultiType = Latin2_GetMultitype;
		pThis->m_pfnIsValidateInput = Latin2_ValidateInput;
		break;
#endif

#if defined(KMX_LDB_VIETNAMESE)
    case IIME_LANG_VIETNAMESE:
        pThis->m_pfnGetMultiType = Latin3_GetMultitype;
		pThis->m_pfnIsValidateInput = Latin3_ValidateInput;
		break;
#endif

#if defined(KMX_LDB_AMHARIC)
    case IIME_LANG_AMHARIC:
        pThis->m_pfnGetMultiType = Amharic_GetMultitype;
		pThis->m_pfnIsValidateInput = Amharic_ValidateInput;
		break;
#endif

    default:
		pThis->m_pfnGetMultiType = Basic_GetMultitype;
		pThis->m_pfnIsValidateInput = Basic_ValidateInput;
		break;
	}

    if (idMethod == IM_TELUGU_MT + IM_FULLKEY_BASE || 
        idMethod == IM_MARATHI_MT + IM_FULLKEY_BASE || 
        idMethod == IM_TAMIL_MT + IM_FULLKEY_BASE ||
        idMethod == IM_PUNJABI_MT + IM_FULLKEY_BASE ||
        idMethod == IM_GUJARATI_MT + IM_FULLKEY_BASE ||
        idMethod == IM_ORIYA_MT + IM_FULLKEY_BASE ||
        idMethod == IM_KANNADA_MT + IM_FULLKEY_BASE ||
        idMethod == IM_MALAYALAM_MT + IM_FULLKEY_BASE ||
        idMethod == IM_KHMER_MT + IM_FULLKEY_BASE ||
        idMethod == IM_LAO_MT + IM_FULLKEY_BASE ||
        idMethod == IM_AMHARIC_MT + IM_FULLKEY_BASE || 
        idMethod == IM_MYANMAR_MT + IM_FULLKEY_BASE ||
        idMethod == IM_NEPALI_MT + IM_FULLKEY_BASE ||
        idMethod == IM_BODO_MT + IM_FULLKEY_BASE ||
		idMethod == IM_DOGRI_MT + IM_FULLKEY_BASE ||
		idMethod == IM_SANSKRIT_MT + IM_FULLKEY_BASE ||
		idMethod == IM_KASHMIRI_MT + IM_FULLKEY_BASE ||
		idMethod == IM_KONKANI_MT + IM_FULLKEY_BASE ||
		idMethod == IM_MAITHILI_MT + IM_FULLKEY_BASE ||
		idMethod == IM_MANIPURI_MT + IM_FULLKEY_BASE ||
		idMethod == IM_SANTALI_MT + IM_FULLKEY_BASE ||
		idMethod == IM_SOMALI_MT + IM_FULLKEY_BASE ||
		idMethod == IM_SINDHI_MT + IM_FULLKEY_BASE ||
        idMethod == IM_ASSAMESE_MT + IM_FULLKEY_BASE
        )//daodejing
    {
        IIME_FREE(pThis->m_Engine.engine);
        pThis->m_Engine.engine = 0;
    }
	else if (!kmx_latin_initialize_ex(&(pThis->m_Engine), &init_info))
	{
		IIME_FREE(pThis->m_Engine.engine);
		IIME_FREE(pThis);

		return 0;
	}

    if (pMulti_string_table)
        pThis->m_keymap = (const QWERTY_KEYMAP*)pMulti_string_table;
    else
        pThis->m_keymap = 0;/*LatinFull_FindKeymap(pThis);*/
	
	return (Provider_Com*)pThis;
}

void IIMEAmharicFullProvider_Delete(
    Provider_Com*           pProvider)
{
	IIMEAmharicFullProvider *pThis = (IIMEAmharicFullProvider *)pProvider;
	if (pThis)
	{
        if (pThis->m_Engine.engine)
		    IIME_FREE(pThis->m_Engine.engine);

		IIME_FREE(pThis);
	}

	return;
}


/*
 interface
 */

static IIME_RESULT EventHandle_IIMEAmharicFullProvider(Provider_Com *pProvider,
                                   IIME_UINT hEvent,
                                   IIME_UINT iMsg,
                                   IIME_IMM_CONTENT *pContent)
{
	IIME_RESULT ret = IIME_ERROR_UNUSED;
	IIMEAmharicFullProvider *pThis = (IIMEAmharicFullProvider *)pProvider;

	memset(pThis->m_immContent.pCommit, 0, sizeof(unsigned short) * 64);

    if (pThis->m_flagFinish)
        ret = EventHandle_Smart(pThis, hEvent, iMsg, pContent);
    else
        ret = EventHandle_Multitap(pThis, hEvent, iMsg, pContent);

    return ret;
}

static IIME_RESULT SetOptions_IIMEAmharicFullProvider(Provider_Com *pProvider,
                                  const IIME_IMM_OPTIONS *pOption)
{
    IIMEAmharicFullProvider *pThis = (IIMEAmharicFullProvider *)pProvider;
    if (pOption->pfnGetContext)
        pThis->m_pfnGetContext = pOption->pfnGetContext;

    if (pOption->pQwertyInfo)
    {
        if (pThis->m_keymap == 0)
            pThis->m_keymap = LatinFull_FindKeymap(pThis, pOption->pQwertyInfo);
    }
	return IIME_ERROR_SUCCESS;
}

static IIME_RESULT GetOptions_IIMEAmharicFullProvider(Provider_Com *pProvider,
                                  IIME_IMM_OPTIONS *pOption)
{
    return IIME_ERROR_SUCCESS;
}

static IIME_RESULT AddPhrase_IIMEAmharicFullProvider(Provider_Com* pProvider, unsigned short* szPhrase, int nLen)
{
    return IIME_ERROR_UNKNOWN;
}

static IIME_RESULT CheckAsso_IIMEAmharicFullProvider(Provider_Com* pProvider, unsigned short wContext, unsigned short* szAsso, int* pNumber)
{
    return IIME_ERROR_UNKNOWN;
}

static IIME_RESULT QueryProperties_IIMEAmharicFullProvider(Provider_Com* pProvider, int* pProperties)
{
    return IIME_ERROR_UNKNOWN;
}


/*
 implementions
 */


static IIME_RESULT EventHandle_Smart(IIMEAmharicFullProvider *pThis, 
                                   IIME_UINT hEvent,
                                   IIME_UINT iMsg,
                                   IIME_IMM_CONTENT *pContent)
{
    int i;
	IIME_RESULT ret = IIME_ERROR_UNUSED;
    LDB_CHAR commit_string[8] = {0};
    LDB_CHAR commit_char = 0;

    if (pThis->m_nInput > 0)
	    pThis->m_PreChar = pThis->m_pInput[pThis->m_nInput - 1];
    else
	    pThis->m_PreChar = 0;

    if (pThis->m_pfnGetContext && ( (pThis->m_idMethod & ~(IM_FULLKEY_BASE)) % 10 == 1 ) )
    {
        Smart_CheckCase(pThis);
    }

	if (hEvent == IIME_EVENT_KEYBOARD)
	{
		ret = Smart_KeyHandle(pThis, iMsg);
	}
	else if (hEvent == IIME_EVENT_CHAR)
	{		
        ret = Smart_CharHandle(pThis, iMsg);
	}
	else if (hEvent == IIME_EVENT_TIMER)
	{
		ret = Smart_TimerHandle(pThis);
	}
	else
		ret = IIME_ERROR_UNUSED;

    memset(pContent, 0, sizeof(IIME_IMM_CONTENT));
    memcpy(pContent, &(pThis->m_immContent), sizeof(IIME_IMM_CONTENT));
    pContent->flagState = IIME_FLAG_STATE_CAND;

    if (ret == IIME_ERROR_COMMIT && ((pThis->m_idMethod & ~(IM_FULLKEY_BASE)) % 10 == 4)) 
        pThis->m_bCapLock = IIME_TRUE;

    if (pThis->m_pfnGetContext == 0)
    {
        i = 0;
        if (pContent->pCommit[0])
        {
            while(pContent->pCommit[i])
            {
                commit_string[0] = pContent->pCommit[i];
                i++;
            }
            i = 1;
        }
        if (ret == IIME_ERROR_COMMIT)
        {
            commit_string[i] = (LDB_CHAR)iMsg;
            i++;
        }

        if (commit_string[i - 1] == ' ')
        {
            i--;
        }

        if (i > 0)
        {
            commit_char = commit_string[i - 1];
        }

        if (((pThis->m_idMethod & ~(IM_FULLKEY_BASE)) % 10 == 1) && commit_char != 0)
        {
            if (kmx_is_sou_delimiter(&(pThis->m_Engine), (LDB_CHAR)iMsg))
                pThis->m_flagCase = KMX_LATIN_INITIAL_CASE;
            else
                pThis->m_flagCase = KMX_LATIN_LOWER_CASE;
        }
    }

	if(pThis->m_UnderChar)
		pContent->flagTimer = IIME_TIMER_SET;
    else
        pContent->flagTimer = IIME_TIMER_CLEAR;

    if (&(pThis->m_Engine) && pContent->pCommit[0])
    {
        LDB_CHAR *pCommit = pContent->pCommit;
        i = 0;
        while (*pCommit)
        {
            i++;
            pCommit++;
        }

        kmx_latin_update_cache(&(pThis->m_Engine), pContent->pCommit, (unsigned short)i);
    }

    return ret;
}

static IIME_RESULT Smart_KeyHandle(IIMEAmharicFullProvider *pThis, IIME_UINT iMsg)
{
	IIME_RESULT ret = IIME_ERROR_UNUSED;

	if (pThis->m_UnderChar)
	{
		pThis->m_pInput[pThis->m_nInput++] = pThis->m_UnderChar;
		LatinFull_ResetMultiKey(pThis);
	}

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
        ret = Smart_Commit(pThis, iMsg - IIME_VK_1);
        break;
	case IIME_VK_LEFT:
	case IIME_VK_RIGHT:
	case IIME_VK_UP:
	case IIME_VK_DOWN:
		ret = Smart_NaviCandidate(pThis, iMsg);
		break;
	case IIME_VK_BACKSPACE:
		ret = Smart_DeleteChar(pThis);
		break;
	case IIME_VK_CLEARALL:
		ret = Smart_DeleteAll(pThis);
		break;
	case IIME_VK_SELECT:
		ret = Smart_Commit(pThis, -1);
        pThis->m_bSym = 0;
		break;
    case IIME_VK_POUND:
        LatinFull_Reset(pThis);
        ret = IIME_ERROR_UNUSED;
        break;
	default:
		Smart_Commit(pThis, -1);
		ret = IIME_ERROR_UNUSED;
		break;
	}

	return ret;
}

static IIME_RESULT Smart_CharHandle(IIMEAmharicFullProvider *pThis, IIME_UINT iMsg)
{
	IIME_RESULT ret = IIME_ERROR_UNUSED;

	LDB_CHAR chr = (LDB_CHAR)iMsg;
	if (pThis->m_pfnIsValidateInput(pThis, chr))
	{
		LDB_CHAR preChar = pThis->m_PreChar;
		LDB_CHAR pMultiList[MAX_MATCH + 2];
		int count = 0;

        if (pThis->m_bSym)
        {
            Smart_Commit(pThis, -1);
        }

		if (pThis->m_PreKey != chr && pThis->m_UnderChar)
		{
			pThis->m_pInput[pThis->m_nInput++] = pThis->m_UnderChar;
			preChar = pThis->m_UnderChar;

			LatinFull_ResetMultiKey(pThis);
		}

		memset(pMultiList, 0, sizeof(LDB_CHAR) * (MAX_MATCH + 2));
		pThis->m_pfnGetMultiType(pThis, chr, preChar, pMultiList, &count);
		if (count == 0)
		{
            if (LatinFull_KeymapKey(pThis, chr))
                ret = IIME_ERROR_UNUSED;
            else
            {
			    Smart_Commit(pThis, -1);
			    if (pThis->m_nInput == 0 && pThis->m_UnderChar == 0)
			    {
				    ret = IIME_ERROR_COMMIT;
			    }
			    else
			    {
				    ret = IIME_ERROR_UNUSED;
			    }
            }
		}
		else{
			if (count == 1)
			{
				if (pThis->m_UnderChar)
				{
					pThis->m_pInput[pThis->m_nInput++] = pThis->m_UnderChar;
					LatinFull_ResetMultiKey(pThis);
				}
				pThis->m_pInput[pThis->m_nInput++] = pMultiList[0];
			}
			else if (pThis->m_PreKey == chr)
			{
				pThis->m_nCount++;
				pThis->m_UnderChar = pMultiList[(pThis->m_nCount - 1) % count];
			}
			else
			{
				if (pThis->m_UnderChar)
                {
                    pThis->m_pInput[pThis->m_nInput++] = pThis->m_UnderChar;
                }

				pThis->m_nCount = 1;
				pThis->m_PreKey = chr;
				pThis->m_PreChar = pThis->m_UnderChar;
				pThis->m_UnderChar = pMultiList[0];
			}

			ret = Smart_AddChar(pThis);
		}
	}
    else if (chr == SYMBOL_KEY)
    {
        if (pThis->m_nInput == 0 && pThis->m_UnderChar == 0)
        {
            int i;
            int count;
            LDB_CHAR pSymList[MAX_MATCH + 2];

            memset(pSymList, 0, sizeof(LDB_CHAR) * MAX_MATCH);

            pThis->m_bSym = 1;

            if (pThis->m_PreChar != chr)
            {
                LatinFull_GetSymList(pThis, pSymList, &count);
                for (i = 0; i < count; i++)
                {
                    pThis->m_candidate[i].length = 1;
                    pThis->m_candidate[i].string[0] = pSymList[i];
                }

                pThis->m_nCandidate = count;

                pThis->m_pPageMark[0] = 0;
	            LatinFull_FillCandidate(pThis);
            }
            else
            {
                pThis->m_nCount++;  /* something to do!*/
            }


            ret = IIME_ERROR_USED;
        }
        else
            ret = IIME_ERROR_UNUSED;
    }
	else
	{
		Smart_Commit(pThis, -1);
		ret = IIME_ERROR_COMMIT;
	}

	return ret;
}

static IIME_RESULT Smart_TimerHandle(IIMEAmharicFullProvider *pThis)
{
	IIME_RESULT ret = IIME_ERROR_UNUSED;

	if (pThis->m_UnderChar)
	{
		pThis->m_pInput[pThis->m_nInput++] = pThis->m_UnderChar;
		LatinFull_ResetMultiKey(pThis);
	}

	return ret;
}

static IIME_RESULT Smart_Commit(IIMEAmharicFullProvider *pThis, int idx)
{
	IIME_RESULT ret = IIME_ERROR_UNUSED;

    if (idx != -1)
    {
        if (idx < pThis->m_immContent.nCand)
        {
            pThis->m_immContent.iHotCand = (unsigned short)idx;
        }
        else
            return ret;
    }

    if (pThis->m_immContent.nCand)
    {
	    int i = 0;
	    unsigned short *pBuffer = pThis->m_immContent.pCandBuffer;
	    while (i < pThis->m_immContent.iHotCand){
		    pBuffer += IIMEString_Length(pBuffer) + 1;
		    i++;
	    }

	    IIMEString_Copy(pThis->m_immContent.pCommit, pBuffer);

	    LatinFull_Reset(pThis);

        ret = IIME_ERROR_USED;
    }

    return ret;
}

static IIME_RESULT Smart_NaviCandidate(IIMEAmharicFullProvider *pThis, IIME_UINT iMsg)
{
	IIME_RESULT ret = IIME_ERROR_USED;
	IIME_IMM_CONTENT *pContent = &(pThis->m_immContent);

	if (pThis->m_nCandidate == 0)
		return IIME_ERROR_UNUSED;

    if (pThis->m_flagRTL)
    {
        if (iMsg == IIME_VK_RIGHT)
            iMsg = IIME_VK_LEFT;
        else if (iMsg == IIME_VK_LEFT)
            iMsg = IIME_VK_RIGHT;
    }

	switch (iMsg)
	{
	case IIME_VK_RIGHT:
		if (pContent->iHotCand < pContent->nCand - 1)
		{
			pContent->iHotCand++;
		}
		else if (pContent->flagPageCand & IIME_FLAG_PAGEUP)
		{
			pThis->m_nCurPage++;
			LatinFull_FillCandidate(pThis);
		}
		break;
	case IIME_VK_LEFT:
		if (pContent->iHotCand > 0)
		{
			pContent->iHotCand--;
		}
		else if (pThis->m_nCurPage > 0)
		{
			pThis->m_nCurPage--;
			LatinFull_FillCandidate(pThis);
		}
		break;
	case IIME_VK_UP:
		if (pThis->m_nCurPage > 0)
		{
			pThis->m_nCurPage--;
			LatinFull_FillCandidate(pThis);
		}
		break;
	case IIME_VK_DOWN:
		if (pContent->flagPageCand & IIME_FLAG_PAGEUP)
		{
			pThis->m_nCurPage++;
			LatinFull_FillCandidate(pThis);
		}
		break;
	default:
		ret = IIME_ERROR_UNUSED;
		break;
	}
	
	return ret;
}

static IIME_RESULT Smart_DeleteChar(IIMEAmharicFullProvider *pThis)
{
	IIME_RESULT ret;
	int input_length = pThis->m_nInput;
	if (pThis->m_UnderChar)
		input_length++;

	if (input_length > 0)
	{
		if (pThis->m_UnderChar)
		{
			LatinFull_ResetMultiKey(pThis);
		}
		else
		{
			pThis->m_nInput--;
			pThis->m_pInput[pThis->m_nInput] = 0;
		}

		input_length--;

		if (input_length > 0)
		{
			LatinFull_Recognize(pThis);
		}
		else
		{
			LatinFull_Reset(pThis);
		}

		ret = IIME_ERROR_USED;
	}
    else if (pThis->m_bSym)
    {
        LatinFull_Reset(pThis);
        pThis->m_bSym = 0;

        ret = IIME_ERROR_USED;
    }
	else
	{
		ret = IIME_ERROR_UNUSED;
	}

	return ret;
}

static IIME_RESULT Smart_DeleteAll(IIMEAmharicFullProvider *pThis)
{
	int input_length = pThis->m_nInput;
	if (pThis->m_UnderChar)
		input_length++;
    if (pThis->m_bSym)
        input_length = 1;

	LatinFull_Reset(pThis);

	if (input_length)
		return IIME_ERROR_USED;
	else
		return IIME_ERROR_UNUSED;
}

static void Smart_CheckCaseByContext(IIMEAmharicFullProvider *pThis, const unsigned short *text, int length)
{
    IIME_BOOL has_space = 0;
    LDB_CHAR commit_char = 0;

    while (length > 0 && text[length - 1] == ' ')
    {
        has_space = 1;
        length--;
    }

    if (length > 0)
    {
        commit_char = text[length - 1];

        if (commit_char == 0x0A || commit_char == 0x0D)
            pThis->m_flagCase = KMX_LATIN_INITIAL_CASE;
        else if (commit_char == '.')
        {
            if (has_space)
                pThis->m_flagCase = KMX_LATIN_INITIAL_CASE;
            else
                pThis->m_flagCase = KMX_LATIN_LOWER_CASE;
        }
        else
        {
            if (kmx_is_sou_delimiter(&(pThis->m_Engine), commit_char))
                pThis->m_flagCase = KMX_LATIN_INITIAL_CASE;
            else
                pThis->m_flagCase = KMX_LATIN_LOWER_CASE;
        }
    }
}


static void Smart_CheckCase(IIMEAmharicFullProvider *pThis)
{
    const unsigned short *text = 0;
    int length = 0;

    pThis->m_flagCase = KMX_LATIN_INITIAL_CASE;
    
    if ((*(pThis->m_pfnGetContext))(&text, &length) != 0)
    {
        Smart_CheckCaseByContext(pThis, text, length);
    }
}

static int PostProcess(unsigned short *pBuffer, IIMEAmharicFullProvider *pThis)
{
    int length = 0;
    switch (pThis->m_language)
	{
//daodejing
#if defined(KMX_LDB_HINDI) || defined(KMX_LDB_BENGALI) || defined(KMX_LDB_TELUGU) || \
    defined(KMX_LDB_MARATHI) || defined(KMX_LDB_TAMIL) || defined(KMX_LDB_PUNJABI) || defined(KHMX_LDB_KHMER) || \
    defined(KMX_LDB_NEPALI) || defined(KMX_LDB_BODO) || defined(KMX_LDB_DOGRI) || defined(KMX_LDB_SANSKRIT) || defined(KMX_LDB_KASHMIRI) || \
    defined(KMX_LDB_KONKANI) || defined(KMX_LDB_MAITHILI) || defined(KMX_LDB_MANIPURI) || defined(KMX_LDB_SANTALI) || defined(KMX_LDB_SOMALI) || defined(KMX_LDB_SINDHI)
	case IIME_LANG_HINDI:
	case IIME_LANG_NEPALI:
	case IIME_LANG_SINHALA:
	case IIME_LANG_BODO://daodejing
	case IIME_LANG_DOGRI:
	case IIME_LANG_SANSKRIT:
	case IIME_LANG_KASHMIRI:
	case IIME_LANG_KONKANI:
	case IIME_LANG_MAITHILI:
	case IIME_LANG_MANIPURI:
	case IIME_LANG_SANTALI:
	case IIME_LANG_SOMALI:
	case IIME_LANG_SINDHI:
    case IIME_LANG_BENGALI:
    case IIME_LANG_TELUGU:
    case IIME_LANG_MARATHI:
    case IIME_LANG_TAMIL:
    case IIME_LANG_KHMER:
		break;
#endif

#ifdef KMX_LDB_THAI
	case IIME_LANG_THAI:
		break;
#endif

#if defined(KMX_LDB_ARABIC) || defined(KMX_LDB_HEBREW) || defined(KMX_LDB_URDU) || defined(KMX_LDB_PERSIAN)
	case IIME_LANG_ARABIC:
    case IIME_LANG_HEBREW:
    case IIME_LANG_URDU:
    case IIME_LANG_PERSIAN:
        Arabic_Normalize(pBuffer, pThis);
        break;
#endif

#if defined(KMX_LDB_ARMENIAN) || defined(KMX_LDB_BULGARIAN) || defined(KMX_LDB_CZECH) || defined(KMX_LDB_DANISH) || \
    defined(KMX_LDB_FINNISH) || defined(KMX_LDB_FRENCH) || defined(KMX_LDB_GERMAN) || defined(KMX_LDB_GREEK) || \
    defined(KMX_LDB_HUNGARIAN) || defined(KMX_LDB_ITALIAN) || defined(KMX_LDB_LITHUANIAN) || defined(KMX_LDB_NORWEGIAN) || \
    defined(KMX_LDB_POLISH) || defined(KMX_LDB_PORTUGUESE) || defined(KMX_LDB_ROMANIAN) || defined(KMX_LDB_RUSSIAN) || \
    defined(KMX_LDB_SLOVAK) || defined(KMX_LDB_SLOVENIAN) || defined(KMX_LDB_SPANISH) || defined(KMX_LDB_SWEDISH) || \
    defined(KMX_LDB_TURKISH) || defined(KMX_LDB_UKRAINIAN)
    case IIME_LANG_ARMENIAN:
    case IIME_LANG_BULGARIAN:
    case IIME_LANG_CZECH:
    case IIME_LANG_DANISH:
    case IIME_LANG_FINNISH:
    case IIME_LANG_FRENCH:
    case IIME_LANG_GERMAN:
    case IIME_LANG_GREEK:
    case IIME_LANG_HUNGARIAN:
    case IIME_LANG_ITALIAN:
    case IIME_LANG_LITHUANIAN:
    case IIME_LANG_NORWEGIAN:
    case IIME_LANG_POLISH:
    case IIME_LANG_PORTUGUESE:
    case IIME_LANG_ROMANIAN:
    case IIME_LANG_RUSSIAN:
    case IIME_LANG_SLOVAK:
    case IIME_LANG_SLOVENIAN:
    case IIME_LANG_SPANISH:
    case IIME_LANG_SWEDISH:
    case IIME_LANG_TURKISH:
    case IIME_LANG_UKRAINIAN:
        Latin2_Normalize(pBuffer, pThis);
		break;
#endif
#if defined(KMX_LDB_VIETNAMESE)
    case IIME_LANG_VIETNAMESE:
        Vietnamese_Normalize(pBuffer, pThis);
        break;
#endif
    default:
        break;
    }

    length = IIMEString_Length(pBuffer);
	return length;
}

static IIME_RESULT EventHandle_Multitap(IIMEAmharicFullProvider *pThis, 
                                   IIME_UINT hEvent,
                                   IIME_UINT iMsg,
                                   IIME_IMM_CONTENT *pContent)
{
	IIME_RESULT ret = IIME_ERROR_UNUSED;
    unsigned short original_case = pThis->m_flagCase;
    unsigned short pre_char = 0;

    if (pThis->m_pfnGetContext)
    {
        const unsigned short *context = 0;
        int len = 0;
        if ((*pThis->m_pfnGetContext)(&context, &len) != 0)
        {
            if (len > 0)
                pre_char = context[len - 1];
        }
    }
    else
        pre_char = pContent->iCaretPos;

    if (original_case == KMX_LATIN_INITIAL_CASE )
    {
        if (pThis->m_pfnGetContext && (pThis->m_idMethod - IM_FULLKEY_BASE) % 10 == 5)
        {
            Smart_CheckCase(pThis);
        }
        else
        {
            if (pre_char == ' ')
                pThis->m_flagCase = KMX_LATIN_UPPER_CASE;
            else if (pre_char == 0)
            {
                if (pThis->m_bCapLock)
                    pThis->m_flagCase = KMX_LATIN_UPPER_CASE;
                else
                    pThis->m_flagCase = KMX_LATIN_LOWER_CASE;
            }
            else
                pThis->m_flagCase = KMX_LATIN_LOWER_CASE;
        }
    }

    pThis->m_PreChar = pre_char;

    if (hEvent == IIME_EVENT_KEYBOARD)
	{
		ret = Multitap_KeyHandle(pThis, iMsg);
	}
	else if (hEvent == IIME_EVENT_CHAR)
	{		
        ret = Multitap_CharHandle(pThis, iMsg);
	}
	else if (hEvent == IIME_EVENT_TIMER)
	{
		ret = Multitap_TimerHandle(pThis);
	}
	else
		ret = IIME_ERROR_UNUSED;

    {
        if (pThis->m_UnderChar)
        {
            pContent->pUnderlineBuffer[0] = pThis->m_UnderChar;
            pContent->nUnderline = 1;
        }

        if (pThis->m_immContent.pCommit[0])
        {
            IIMEString_Copy(pContent->pCommit, pThis->m_immContent.pCommit);

            if ((pThis->m_idMethod & ~(IM_FULLKEY_BASE)) % 10 == 4)
                pThis->m_bCapLock = IIME_FALSE;
        }

    }

    if (ret == IIME_ERROR_COMMIT && ((pThis->m_idMethod & ~(IM_FULLKEY_BASE)) % 10 == 4)) 
        pThis->m_bCapLock = IIME_TRUE;

    pThis->m_flagCase = original_case;

	if(pThis->m_UnderChar)
    {
        pContent->flagTimer = IIME_TIMER_SET;
#ifdef MULTITAP_WITH_CANDIDATES
        if (pThis->m_nCandidate > 1)
        {
            int i;
            unsigned short *pBuffer = pContent->pCandBuffer;
            pContent->nCand = (unsigned short)pThis->m_nCandidate;
            for (i = 0; i < pThis->m_nCandidate; i++)
            {
                pBuffer[2 * i] = pThis->m_candidate[i].string[0];
                pBuffer[2 * i + 1] = 0;
            }

            pBuffer[2 * i] = 0;

            pContent->iHotCand = (unsigned short)((pThis->m_nCount - 1) % pContent->nCand);
            pContent->nUnderline = 0;
        }
#endif
    }
    else
        pContent->flagTimer = IIME_TIMER_CLEAR;

    {
        int len_old, len_new;
        unsigned short pBuffer[MAX_ITEM_LENGTH] = L"\0";
        if (pre_char)
        {
            pBuffer[0] = pre_char;
            pBuffer[1] = 0;
        }
        IIMEString_Append(pBuffer, pContent->pCommit);

        len_old = IIMEString_Length(pBuffer);
        len_new = PostProcess(pBuffer, pThis);
        if (len_new < len_old)
        {
            pContent->pCommit[0] = '\b';
            pContent->pCommit[1] = 0;
            IIMEString_Append(pContent->pCommit, pBuffer);
        }
    }

    return ret;
}


static IIME_RESULT Multitap_KeyHandle(IIMEAmharicFullProvider *pThis, IIME_UINT iMsg)
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
	case IIME_VK_LEFT:
	case IIME_VK_RIGHT:
	case IIME_VK_UP:
	case IIME_VK_DOWN:
        ret = Multitap_Commit(pThis, -1);
		break;
	case IIME_VK_BACKSPACE:
	case IIME_VK_CLEARALL:
        if (pThis->m_UnderChar)
        {
            LatinFull_Reset(pThis);
            ret = IIME_ERROR_USED;
        }
		break;
	case IIME_VK_SELECT:
        if (pThis->m_UnderChar)
		    ret = Multitap_Commit(pThis, -1);
		break;
	default:
		Multitap_Commit(pThis, -1);
		ret = IIME_ERROR_UNUSED;
		break;
	}

    pThis->m_bSym = 0;

	return ret;
}


static IIME_RESULT Multitap_CharHandle(IIMEAmharicFullProvider *pThis, IIME_UINT iMsg)
{
	IIME_RESULT ret = IIME_ERROR_UNUSED;

	LDB_CHAR chr = (LDB_CHAR)iMsg;
	if (pThis->m_pfnIsValidateInput(pThis, chr))
	{
		LDB_CHAR preChar = pThis->m_PreChar;
		LDB_CHAR pMultiList[10];
		int count = 0;

        if (pThis->m_bSym)
        {
            Multitap_Commit(pThis, -1);
            pThis->m_bSym = 0;
        }
        else if (pThis->m_PreKey != chr && pThis->m_UnderChar)
        {
            Multitap_Commit(pThis, -1);
        }

        if (pThis->m_immContent.pCommit[0])
        {
            preChar = pThis->m_immContent.pCommit[0];
            if ((pThis->m_idMethod - IM_FULLKEY_BASE) % 10 == 5)
                Smart_CheckCaseByContext(pThis, &preChar, 1);
        }

		memset(pMultiList, 0, sizeof(LDB_CHAR) * 10);
		pThis->m_pfnGetMultiType(pThis, chr, preChar, pMultiList, &count);
		if (count == 0)
		{
            if (LatinFull_KeymapKey(pThis, chr))
            {
                ret = IIME_ERROR_UNUSED;
            }
            else if (pThis->m_immContent.pCommit[0])
            {
				ret = IIME_ERROR_UNUSED;
			}
			else
			{
				ret = IIME_ERROR_COMMIT;
			}

            LatinFull_Reset(pThis);
		}
		else{
			if (count == 1)
			{
				pThis->m_pInput[pThis->m_nInput++] = pMultiList[0];
                Multitap_Commit(pThis, -1);

                ret = IIME_ERROR_USED;
			}
			else if (pThis->m_PreKey == chr)
			{
				pThis->m_nCount++;
				pThis->m_UnderChar = pMultiList[(pThis->m_nCount - 1) % count];
			}
			else
			{
				pThis->m_nCount = 1;
				pThis->m_PreKey = chr;
				pThis->m_PreChar = pThis->m_UnderChar;
				pThis->m_UnderChar = pMultiList[0];
			}

#ifdef MULTITAP_WITH_CANDIDATES
            if (count > 1)
            {
                int i;
                pThis->m_nCandidate = count;
                for (i = 0; i < pThis->m_nCandidate; i++)
                {
                    pThis->m_candidate[i].length = 1;
                    pThis->m_candidate[i].string[0] = pMultiList[i];
                    pThis->m_candidate[i].string[1] = 0;
                }
            }
#endif
            ret = IIME_ERROR_USED;
		}
	}
    else if (chr == 0x0F)
    {
        int count = 0;
        LDB_CHAR pSymList[MAX_MATCH + 2];

        if (pThis->m_PreKey != chr)
        {
            if (pThis->m_UnderChar)
                Multitap_Commit(pThis, -1);
            LatinFull_Reset(pThis);
        }

        pThis->m_bSym = 1;

        memset(pSymList, 0, sizeof(LDB_CHAR) * MAX_MATCH);

        LatinFull_GetSymList(pThis, pSymList, &count);

        if (count > 0)
        {
            if (pThis->m_PreKey == chr)
            {
                pThis->m_nCount++;
                pThis->m_UnderChar = pSymList[(pThis->m_nCount - 1) % count];
            }
            else
            {
                pThis->m_nCount = 1;
                pThis->m_PreKey = chr;
                pThis->m_UnderChar = pSymList[0];
            }
        }

        ret = IIME_ERROR_USED;
    }
	else
	{
		Multitap_Commit(pThis, -1);
		ret = IIME_ERROR_COMMIT;
	}

	return ret;
}

static IIME_RESULT Multitap_TimerHandle(IIMEAmharicFullProvider *pThis)
{
	IIME_RESULT ret = IIME_ERROR_UNUSED;

	if (pThis->m_UnderChar)
	{
        Multitap_Commit(pThis, -1);
		LatinFull_Reset(pThis);
	}

	return ret;
}

static IIME_RESULT Multitap_Commit(IIMEAmharicFullProvider *pThis, int idx)
{
	IIME_RESULT ret = IIME_ERROR_UNUSED;
    unsigned short *pBuffer = pThis->m_immContent.pCommit;
    while (*pBuffer)
        pBuffer++;

    if (pThis->m_UnderChar)
    {
        *pBuffer++ = pThis->m_UnderChar;
        *pBuffer++ = 0;

        LatinFull_Reset(pThis);

        ret = IIME_ERROR_USED;
    }

    if (pThis->m_nInput)
    {
        *pBuffer++ = pThis->m_pInput[pThis->m_nInput-1];
        *pBuffer++ = 0;

        LatinFull_Reset(pThis);

        ret = IIME_ERROR_USED;
    }

    return ret;
}

static void LatinFull_Recognize(IIMEAmharicFullProvider *pThis)
{
	unsigned short index = 0;

	int bAvailable = 1;

	unsigned short input_string[64];
	unsigned short input_length;

	KMX_LATIN_CANDIDATE *pCandidate = pThis->m_candidate;
	KMX_LATIN_MATCH_RESULT match_result;

	memset(input_string, 0, sizeof(unsigned short) * 64);
	memcpy(input_string, pThis->m_pInput, sizeof(unsigned short) * 64);
	input_length = (unsigned short)pThis->m_nInput;
	if (pThis->m_UnderChar)
	{
		input_string[pThis->m_nInput] = pThis->m_UnderChar;
		input_length++;
	}

    input_length = (unsigned short)PostProcess(input_string, pThis);

    if (input_length == 0)
		return;

	memset(pCandidate, 0, sizeof(KMX_LATIN_CANDIDATE) * (MAX_MATCH + 1));
	LatinFull_ResetPageInfo(pThis);

    while(bAvailable)
	{
        if (kmx_latin_query_association(&(pThis->m_Engine), input_string, input_length, pThis->m_flagCase, index, &match_result))
		{
			int i = 0;
			for (i = 0; i < match_result.count; i++)
			{
				KMX_LATIN_CANDIDATE *pResult = match_result.table + i;
				pCandidate[index].length = pResult->length;
				memcpy(pCandidate[index].string, pResult->string, sizeof(unsigned short) * pResult->length);

				index++;
			}

			bAvailable = match_result.more_available;
		}
		else
		{
			bAvailable = 0;
		}
	}

    if (index == 0)
    {
        switch(pThis->m_flagCase)
		{
		case KMX_LATIN_INITIAL_CASE:
			kmx_latin_initial_case(&(pThis->m_Engine), input_string, input_length);
			break;
		case KMX_LATIN_UPPER_CASE:
			kmx_latin_upper_case(&(pThis->m_Engine), input_string, input_length);
			break;
		case KMX_LATIN_LOWER_CASE:
		default:
			kmx_latin_lower_case(&(pThis->m_Engine), input_string, input_length);
			break;
		}

        pCandidate[index].length = input_length;
        memcpy(pCandidate[index].string, input_string, sizeof(unsigned short) * input_length);

        index++;
    }

	pThis->m_nCandidate = index;

	pThis->m_pPageMark[0] = 0;

	LatinFull_FillCandidate(pThis);
}

static void LatinFull_FillCandidate(IIMEAmharicFullProvider *pThis)
{
    IIME_BOOL flagPageNext = IIME_FALSE;

	int pos, width;
	unsigned short *p = pThis->m_immContent.pCandBuffer;

	LatinFull_ClearCandidate(pThis);

	pos = pThis->m_pPageMark[pThis->m_nCurPage];
	if (pos == -1 || pThis->m_nCandidate == 0)
		return;
	
	width = 0;
	while(pos < pThis->m_nCandidate)
	{
		KMX_LATIN_CANDIDATE *pCand = pThis->m_candidate + pos;
		width += pThis->m_pGetTextWidthFunc(pCand->string, pCand->length) + pThis->m_nLableWidth;
		if (width > pThis->m_nWinWidth)
        {
            flagPageNext = IIME_TRUE;
			break;
        }

		memcpy(p, pCand->string, sizeof(unsigned short) * pCand->length);
		p += pCand->length + 1;
		pos++;
		pThis->m_immContent.nCand++;

		if (pThis->m_immContent.nCand >= 9)
			break;
	}

	if (flagPageNext || (pos < pThis->m_nCandidate - 1))
	{
		pThis->m_pPageMark[pThis->m_nCurPage + 1] = pos;
		pThis->m_immContent.flagPageCand |= IIME_FLAG_PAGEUP;
	}
	else
		pThis->m_immContent.flagPageCand &= ~IIME_FLAG_PAGEUP;

	if (pThis->m_nCurPage > 0)
		pThis->m_immContent.flagPageCand |= IIME_FLAG_PAGEDOWN;
	else
		pThis->m_immContent.flagPageCand &= ~IIME_FLAG_PAGEDOWN;

}

static IIME_RESULT Smart_AddChar(IIMEAmharicFullProvider *pThis)
{
	int bRemove = 0;

	unsigned short input_length = (unsigned short)pThis->m_nInput;
	unsigned short szInput[64];
	memset(szInput, 0, sizeof(unsigned short) * 64);
	memcpy(szInput, pThis->m_pInput, sizeof(unsigned short) * pThis->m_nInput);

	if (pThis->m_UnderChar)
	{
		szInput[input_length++] = pThis->m_UnderChar;
	}
	
    input_length = (unsigned short)PostProcess(szInput, pThis);

	if (input_length < MAX_MATCH)
	{
        switch(pThis->m_flagCase)
		{
		case KMX_LATIN_INITIAL_CASE:
			kmx_latin_initial_case(&(pThis->m_Engine), szInput, input_length);
			break;
		case KMX_LATIN_UPPER_CASE:
			kmx_latin_upper_case(&(pThis->m_Engine), szInput, input_length);
			break;
		case KMX_LATIN_LOWER_CASE:
		default:
			kmx_latin_lower_case(&(pThis->m_Engine), szInput, input_length);
			break;
		}

		if (pThis->m_pGetTextWidthFunc(szInput, input_length) + pThis->m_nLableWidth > pThis->m_nWinWidth)
		{
			bRemove = 1;
		}
		else
		{
			LatinFull_Recognize(pThis);
		}
	}
	else
		bRemove = 1;

	if (bRemove)
	{
		if (pThis->m_UnderChar)
			LatinFull_ResetMultiKey(pThis);
		else
			pThis->m_pInput[--pThis->m_nInput] = 0;

		return IIME_ERROR_UNUSED;
	}
	else
		return IIME_ERROR_USED;
}

static void LatinFull_ResetPageInfo(IIMEAmharicFullProvider *pThis)
{
    memset(pThis->m_pPageMark, 0xFF,  sizeof(int) * 32);
    pThis->m_nCurPage = 0;
    pThis->m_immContent.iHotCand = 0;
}

static void LatinFull_ClearCandidate(IIMEAmharicFullProvider *pThis)
{
	memset(pThis->m_immContent.pCandBuffer, 0, sizeof(unsigned short) * 64);
	pThis->m_immContent.iHotCand = 0;
	pThis->m_immContent.nCand = 0;
}

static void LatinFull_ResetMultiKey(IIMEAmharicFullProvider *pThis)
{
	pThis->m_PreChar = 0;		
	pThis->m_UnderChar = 0;	/* uncommit char */
	pThis->m_PreKey = 0;		/* the key type before */
	pThis->m_nCount = 0;		/* the times of the same key of m_PreKey */
}

static void LatinFull_Reset(IIMEAmharicFullProvider *pThis)
{
	LatinFull_ResetPageInfo(pThis);
	LatinFull_ClearCandidate(pThis);
	memset(pThis->m_pInput, 0, sizeof(unsigned short) * 32);
	pThis->m_nInput = 0;

	pThis->m_nCandidate = 0;
	memset(pThis->m_candidate, 0, sizeof(KMX_LATIN_CANDIDATE) * (MAX_MATCH + 1));

	LatinFull_ResetMultiKey(pThis);
}

static void Basic_GetMultitype(
    IIMEAmharicFullProvider     *pThis, 
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
	OUT LDB_CHAR*			pMultiList,
    OUT int*                count)
{
	pMultiList[0] = key;
	pMultiList[1] = 0;

	switch(pThis->m_flagCase)
	{
	case KMX_LATIN_INITIAL_CASE:
	case KMX_LATIN_UPPER_CASE:
		kmx_latin_upper_case(&(pThis->m_Engine), pMultiList, 1);
		break;
	case KMX_LATIN_LOWER_CASE:
    default:
		kmx_latin_lower_case(&(pThis->m_Engine), pMultiList, 1);
		break;
	}

	*count = 1;
}

static int Basic_ValidateInput(IIMEAmharicFullProvider *pThis, LDB_CHAR c)
{
	return kmx_latin_is_alphabeta(&(pThis->m_Engine), c);
}

#ifdef KMX_LDB_THAI
static void Thai_GetMultitype(
    IIMEAmharicFullProvider     *pThis, 
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
	OUT LDB_CHAR*			pMultiList,
    OUT int*                count)
{

    int                         i;
	const LDB_CHAR	*pString = 0;

    if (!count)
        return;

    *count = 0;

    if (LatinFull_KeymapKey(pThis, key))
    {
        pString = LatinFull_KeymapValue(pThis, key);
    }
    else if (kmx_latin_is_alphabeta(&(pThis->m_Engine), key))
    {
        pMultiList[0] = key;
        *count = 1;
        return;
    }
    else
		return ;

	i = 0;
	while (*pString)
	{
		if (kmx_is_valid_thai_follow_char(prev_char, *pString))
			pMultiList[i++] = *pString;
		pString++;
	}

	if (i > 0)
	{
		*count = i;
		return ;
	}
	else
	{
		*count = 0;
		return ;
	}
}

static int Thai_ValidateInput(IIMEAmharicFullProvider *pThis, LDB_CHAR c)
{
    if (LatinFull_KeymapKey(pThis, c))
		return 1;
	else if (kmx_latin_is_alphabeta(&(pThis->m_Engine), c))
        return 1;
    else
		return 0;
}

#endif /* KMX_LDB_THAI */

#if defined(KMX_LDB_BULGARIAN) || defined(KMX_LDB_ARMENIAN) || defined(KMX_LDB_CZECH) || defined(KMX_LDB_DANISH) || \
    defined(KMX_LDB_FINNISH) || defined(KMX_LDB_FRENCH) || defined(KMX_LDB_GERMAN) || defined(KMX_LDB_GREEK) || \
    defined(KMX_LDB_HUNGARIAN) || defined(KMX_LDB_ITALIAN) || defined(KMX_LDB_LITHUANIAN) || defined(KMX_LDB_NORWEGIAN) || \
    defined(KMX_LDB_POLISH) || defined(KMX_LDB_PORTUGUESE) || defined(KMX_LDB_ROMANIAN) || defined(KMX_LDB_RUSSIAN) || \
    defined(KMX_LDB_SLOVAK) || defined(KMX_LDB_SLOVENIAN) || defined(KMX_LDB_SPANISH) || defined(KMX_LDB_SWEDISH) || \
    defined(KMX_LDB_TURKISH) || defined(KMX_LDB_UKRAINIAN)

static const unsigned short latin2_symbols[] = 
{
    0x0060, 0x00B4, 0x005E, 0x007E, 0x00A8, 0x00B0, 0x00B8, 0x02C7, 0x02DB, 0x0301, 0x0308
};

static IIME_BOOL IsLatin2Symbol(LDB_CHAR c)
{
    int i;
    for (i =0; i < sizeof(latin2_symbols) / sizeof(unsigned short); i++)
    {
        if (c == latin2_symbols[i])
            return IIME_TRUE;
    }

    return IIME_FALSE;
}

static LDB_CHAR Latin2_Translate(LDB_CHAR c1, LDB_CHAR c2)
{
	typedef struct tagSYMBOLS_MAP
	{
		unsigned short key;
		unsigned short value;
	}SYMBOLS_MAP;

	static const SYMBOLS_MAP * pSymbol_map = 0;

	static const SYMBOLS_MAP latin2_symbols_map1[] = 
    {
        /****Grave Accent***/
        {0x0061, 0xE0}, {0x0065, 0xE8}, {0x0069, 0xEC}, {0x006F, 0xF2}, {0x0075, 0xF9},

		{0x0041, 0xC0}, {0x0045, 0xC8}, {0x0049, 0xCC}, {0x004F, 0xD2}, {0x0055, 0xD9}, 0
	};

	static const SYMBOLS_MAP latin2_symbols_map2[] = 
    {
        /***Acute Accent***/
        {0x0061, 0xE1}, {0x0065, 0xE9}, {0x0069, 0xED}, {0x006F, 0xF3}, {0x0075, 0xFA}, {0x0079, 0xFD},

        {0x0041, 0xC1}, {0x0045, 0xC9}, {0x0049, 0xCD}, {0x004F, 0xD3}, {0x0055, 0xDA}, {0x0059, 0xDD},

        /*Liu Jun add 20070319*/
        {0x004C, 0x0139}, {0x006C, 0x013A}, {0x0052, 0x0154}, {0x0072, 0x0155}, 
        /*Liu Jun add end*/

        {0x03B1, 0x03AC}, {0x0391, 0x0386}, {0x03B5, 0x03AD}, {0x0395, 0x0388}, {0x03B7, 0x03AE}, {0x0397, 0x0389},
        {0x03B9, 0x03AF}, {0x0399, 0x038A}, {0x03BF, 0x03CC}, {0x039F, 0x038C}, {0x03C5, 0x03CD}, {0x03A5, 0x038E},
        {0x03C9, 0x03CE}, {0x03A9, 0x038F},
        {0x03CA, 0x0390}, {0x03CB, 0x03B0}, 
        {0, 0}
	};

	static const SYMBOLS_MAP latin2_symbols_map3[] = 
    {
        /***circumflex accent***/
        {0x0061, 0xE2}, {0x0065, 0xEA}, {0x0069, 0xEE}, {0x006F, 0xF4}, {0x0075, 0xFB},

        {0x0041, 0xC2}, {0x0045, 0xCA}, {0x0049, 0xCE}, {0x004F, 0xD4}, {0x0055, 0xDB}, 0
	};

 	static const SYMBOLS_MAP latin2_symbols_map4[] = 
    {
        /***Tilde***/
        {0x0061, 0xE3}, {0x006F, 0xF5}, {0x006E, 0xF1}, {0x0041, 0xC3}, {0x004F, 0xD5}, {0x004E, 0xD1}, 0
	};

 	static const SYMBOLS_MAP latin2_symbols_map5[] = 
    {
        /***Diaeresis***/
        {0x0061, 0xE4}, {0x0065, 0xEB}, {0x0069, 0xEF}, {0x006F, 0xF6}, {0x0075, 0xFC}, {0x0079, 0xFF},

        {0x0041, 0xC4}, {0x0045, 0xCB}, {0x0049, 0xCF}, {0x004F, 0xD6}, {0x0055, 0xDC}, 

        {0x03B9, 0x03CA}, {0x0399, 0x03AA}, {0x03C5, 0x03CB}, {0x03A5, 0x03AB}, {0, 0}
	};
 
 	static const SYMBOLS_MAP latin2_symbols_map6[] = 
    {
        /** degree */
        {0x0041, 0xC5}, {0x0061, 0xE5}, 0
	};
 
 	static const SYMBOLS_MAP latin2_symbols_map7[] = 
    {
        /***Cedilla***/
        {0x0053, 0x15E}, {0x0073, 0x15F}, {0x0043, 0xC7}, {0x0063, 0xE7}, 0
	};
 
 	static const SYMBOLS_MAP latin2_symbols_map8[] = 
    {
        /***Hacek***/
        {0x0063, 0x10D}, {0x0064, 0x10F}, {0x0043, 0x10C}, {0x0044, 0x10E}, {0x0045, 0x11A}, {0x0065, 0x11B},
        {0x004E, 0x147}, {0x006E, 0x148}, {0x0052, 0x158}, {0x0072, 0x159}, {0x0053, 0x160}, {0x0073, 0x161},
        {0x0054, 0x164}, {0x0074, 0x165}, {0x005A, 0x17D}, {0x007A, 0x17E}, {0x006C, 0x13E}, {0x004C, 0x13D}, 0
	};
 
 	static const SYMBOLS_MAP latin2_symbols_map9[] = 
    {
        /***Ogonek***/
        {0x0041, 0x0104}, {0x0045, 0x0118}, {0x0061, 0x0105}, {0x0065, 0x0119}, {0, 0}
	};

    static const SYMBOLS_MAP latin2_symbols_map10[] = 
    {
        /* 0x0301 */
        {0x03B1, 0x03AC}, {0x0391, 0x0386}, {0x03B5, 0x03AD}, {0x0395, 0x0388}, {0x03B7, 0x03AE}, {0x0397, 0x0389},
        {0x03B9, 0x03AF}, {0x0399, 0x038A}, {0x03BF, 0x03CC}, {0x039F, 0x038C}, {0x03C5, 0x03CD}, {0x03A5, 0x038E},
        {0x03C9, 0x03CE}, {0x03A9, 0x038F},
        {0x03CA, 0x0390}, {0x03CB, 0x03B0}, 
        {0, 0}
    };

    static const SYMBOLS_MAP latin2_symbols_map11[] = 
    {
        /* 0x0308 */
        {0x03B9, 0x03CA}, {0x0399, 0x03AA}, {0x03C5, 0x03CB}, {0x03A5, 0x03AB}, {0, 0}
    };

	switch(c2)
	{
	case 0x0060:
		pSymbol_map = latin2_symbols_map1;
		break;
	case 0x00B4:
		pSymbol_map = latin2_symbols_map2;
		break;
	case 0x005E:
		pSymbol_map = latin2_symbols_map3;
		break;
	case 0x007E:
		pSymbol_map = latin2_symbols_map4;
		break;
	case 0x00A8:
		pSymbol_map = latin2_symbols_map5;
		break;
	case 0x00B0:
		pSymbol_map = latin2_symbols_map6;
		break;
	case 0x00B8:
		pSymbol_map = latin2_symbols_map7;
		break;
	case 0x02C7:
		pSymbol_map = latin2_symbols_map8;
		break;
	case 0x02DB:
		pSymbol_map = latin2_symbols_map9;
		break;
    case 0x0301:
        pSymbol_map = latin2_symbols_map10;
        break;
    case 0x0308:
        pSymbol_map = latin2_symbols_map11;
        break;
	default:
		return 0;
	}

	while (pSymbol_map->key != 0)
	{
		if (c1 == pSymbol_map->key)
			return pSymbol_map->value;

		pSymbol_map++;
	}

	return 0;
}

static void Latin2_GetMultitype(
    IIMEAmharicFullProvider     *pThis, 
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
	OUT LDB_CHAR*			pMultiList,
    OUT int*                count)
{
    int   i;
	const LDB_CHAR	*pString = 0;
    LDB_CHAR temp_string[10];

    if (!count)
        return;

    *count = 0;
    memset(temp_string, 0, sizeof(LDB_CHAR) * 10);

    if (LatinFull_KeymapKey(pThis, key))
    {
        pString = LatinFull_KeymapValue(pThis, key);
    }
    else if (kmx_latin_is_alphabeta(&(pThis->m_Engine), key))
    {
        pMultiList[0] = key;
        *count = 1;
        return;
    }
    else if (IsLatin2Symbol(key) && Latin2_Translate(prev_char, key) != 0)
    {
        pMultiList[0] = key;
        *count = 1;
        return;
    }
	else
		return ;

    i = 0;
    while(*pString)
    {
        if (IsLatin2Symbol(*pString))
        {
            if (Latin2_Translate(prev_char, *pString) != 0)
                temp_string[i++] = *pString;
        }
        else
        {
            temp_string[i++] = *pString;
        }
        pString++;
    }

    switch(pThis->m_flagCase)
    {
	case KMX_LATIN_INITIAL_CASE:
        if (pThis->m_nInput > 0)
    		kmx_latin_lower_case(&(pThis->m_Engine), temp_string, (unsigned short)i);
        else
    		kmx_latin_upper_case(&(pThis->m_Engine), temp_string, (unsigned short)i);
        break;
	case KMX_LATIN_UPPER_CASE:
		kmx_latin_upper_case(&(pThis->m_Engine), temp_string, (unsigned short)i);
		break;
	case KMX_LATIN_LOWER_CASE:
    default:
		kmx_latin_lower_case(&(pThis->m_Engine), temp_string, (unsigned short)i);
		break;
    }

    i = 0;
	while (temp_string[i])
	{
        int j = 0;
        int bFind = 0;
        while (pMultiList[j])
        {
            if (pMultiList[j] == temp_string[i])
            {
                bFind = 1;
                break;
            }
            j++;
        }

        if (!bFind)
            pMultiList[j] = temp_string[i];

        i++;
	}

    i = 0;
    while(pMultiList[i])
        i++;

	if (i > 0)
		*count = i;

    return ;
}

static int Latin2_ValidateInput(IIMEAmharicFullProvider *pThis, LDB_CHAR c)
{
	if (LatinFull_KeymapKey(pThis, c))
		return 1;
	else if (kmx_latin_is_alphabeta(&(pThis->m_Engine), c))
        return 1;
    else if (IsLatin2Symbol(c))
        return 1;
    else
		return 0;
}

static int Latin2_Normalize(unsigned short *pBuffer, IIMEAmharicFullProvider *pThis)
{
    unsigned short *p = pBuffer;

    while (*p && *(p+1))
    {
        LDB_CHAR c1 = *p;
        LDB_CHAR c2 = *(p+1);

        LDB_CHAR c3 = Latin2_Translate(c1, c2);
        if (c3)
        {
            *p++ = c3;
            IIMEString_Copy(p, p + 1);
        }
        else
            p++;
    }

    return IIMEString_Length(pBuffer);
}

#endif /* defined(KMX_LDB_BULGARIAN) || defined(KMX_LDB_ARMENIAN) */

/* vietnamese */

#if defined(KMX_LDB_VIETNAMESE)

static const unsigned short VietnameseTone[5] = 
{
    0x0300, 0x0301, 0x0309, 0x0303, 0x0323
};

static const unsigned short VietnameseMapping[][8] = 
{
/* A */ {0x0061, 0x0103, 0x00E2, 0x0041, 0x0102, 0x00C2, 0x0000, 0x0000},
/* B */	{0x0062, 0x0042, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/* C */	{0x0063, 0x0043, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/* D */	{0x0064, 0x0111, 0x20AB, 0x0044, 0x0110, 0x0000, 0x0000, 0x0000},
/* E */	{0x0065, 0x00EA, 0x0045, 0x00CA, 0x0000, 0x0000, 0x0000, 0x0000},
/* F */	{0x0300, 0x0066, 0x0046, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/* G */	{0x0067, 0x0047, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/* H */	{0x0068, 0x0048, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/* I */	{0x0069, 0x0049, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/* J */	{0x0323, 0x006A, 0x004A, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/* K */	{0x006B, 0x004B, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/* L */	{0x006C, 0x004C, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/* M */	{0x006D, 0x004D, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/* N */	{0x006E, 0x004E, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/* O */	{0x006F, 0x00F4, 0x01A1, 0x004F, 0x00D4, 0x01A0, 0x0000, 0x0000},
/* P */	{0x0070, 0x0050, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/* Q */	{0x0071, 0x0051, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/* R */	{0x0309, 0x0072, 0x0052, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/* S */	{0x0301, 0x0073, 0x0053, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/* T */	{0x0074, 0x0054, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/* U */	{0x0075, 0x01B0, 0x0055, 0x01AF, 0x0000, 0x0000, 0x0000, 0x0000},
/* V */	{0x0076, 0x0056, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/* W */	{0x0077, 0x0057, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/* X */	{0x0303, 0x0078, 0x0058, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/* Y */	{0x0079, 0x0059, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
/* Z */	{0x007A, 0x005A, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
#if 0
#ifdef SPREADTRUM_SPECIAL_DOLLAR
/* DOLLAR */ {0x0300, 0x0301, 0x0303, 0x0309, 0x0323, 0x0000, 0x0000, 0x0000}
#else
/* PERIOD */ {0x0300, 0x0301, 0x0303, 0x0309, 0x0323, 0x0000, 0x0000, 0x0000}
#endif
#endif
};

static const unsigned short VietnameseToneMap[24][6] = {
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

static int VietnameseFindVowel(LDB_CHAR chr)
{
    int i;
    for (i = 0; i < 24; i++)
        if (chr == VietnameseToneMap[i][0])
            return i;

    return -1;
}

static int VietnameseFindTone(LDB_CHAR chr)
{
    int i;
    for (i = 0; i < 5; i++)
        if (chr == VietnameseTone[i])
            return i;
    return -1;
}

static void Latin3_GetMultitype(
    IIMEAmharicFullProvider     *pThis, 
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
	OUT LDB_CHAR*			pMultiList,
    OUT int*                count)
{
    int   i;
    IIME_BOOL bIsVowel;
	const LDB_CHAR	*pString = 0;
    LDB_CHAR temp_string[10];

    if (!count)
        return;

    *count = 0;
    memset(temp_string, 0, sizeof(LDB_CHAR) * 10);

	if (key >= 'a' && key <= 'z')
	{
		i = key - 'a';
	}
#if 0
#ifdef SPREADTRUM_SPECIAL_DOLLAR
    else if (key == '$')
#else
    else if (key == '.')
#endif
    {
        i = 26;
	}
#endif
    else if (kmx_latin_is_alphabeta(&(pThis->m_Engine), key))
    {
        pMultiList[0] = key;
        *count = 1;
        return;
    }
    else
		return ;

    bIsVowel = (VietnameseFindVowel(prev_char) != -1);

    pString = VietnameseMapping[i];

    i = 0;
    while(*pString)
    {
        if (VietnameseFindTone(*pString) != -1)
        {
            if (bIsVowel)
                temp_string[i++] = *pString;
        }
        else
            temp_string[i++] = *pString;
    
        pString++;
    }

    switch(pThis->m_flagCase)
    {
	case KMX_LATIN_INITIAL_CASE:
        if (pThis->m_nInput > 0)
    		kmx_latin_lower_case(&(pThis->m_Engine), temp_string, (unsigned short)i);
        else
    		kmx_latin_upper_case(&(pThis->m_Engine), temp_string, (unsigned short)i);
        break;
	case KMX_LATIN_UPPER_CASE:
		kmx_latin_upper_case(&(pThis->m_Engine), temp_string, (unsigned short)i);
		break;
	case KMX_LATIN_LOWER_CASE:
    default:
		kmx_latin_lower_case(&(pThis->m_Engine), temp_string, (unsigned short)i);
		break;
    }

    i = 0;
	while (temp_string[i])
	{
        int j = 0;
        int bFind = 0;
        while (pMultiList[j])
        {
            if (pMultiList[j] == temp_string[i])
            {
                bFind = 1;
                break;
            }
            j++;
        }

        if (!bFind)
            pMultiList[j] = temp_string[i];

        i++;
	}

    i = 0;
    while(pMultiList[i])
        i++;

	if (i > 0)
		*count = i;

    return ;
}

static int Latin3_ValidateInput(IIMEAmharicFullProvider *pThis, LDB_CHAR c)
{
#if 0
#ifdef SPREADTRUM_SPECIAL_DOLLAR
    if ((c >= 'a' && c<= 'z') || c == '$')
#else
    if ((c >= 'a' && c<= 'z') || c == '.')
#endif
#endif
    if (c >= 'a' && c<= 'z')
        return 1;
    else if (kmx_latin_is_alphabeta(&(pThis->m_Engine), c))
        return 1;
	else
		return 0;
}

static int Vietnamese_Normalize(unsigned short *buf, IIMEAmharicFullProvider *pThis)
{
    int i, j;
    unsigned short temp[MAX_ITEM_LENGTH];
    memset(temp, 0, sizeof(unsigned short) * MAX_ITEM_LENGTH);

    i = 0;
    j = 0;
    for (i = 0; i < MAX_ITEM_LENGTH - 1; i++)
    {
        int iTone = VietnameseFindTone(buf[i+1]);
        if (iTone != -1)
        {
            int iVowel;
            iVowel = VietnameseFindVowel(buf[i]);
            temp[j++] = VietnameseToneMap[iVowel][iTone + 1];
            i++;
        }
        else
            temp[j++] = buf[i];
    }

    if (VietnameseFindTone(buf[MAX_ITEM_LENGTH - 1]) == -1)
        temp[j++] = buf[MAX_ITEM_LENGTH - 1];

    memcpy(buf, temp, sizeof(unsigned short) * MAX_ITEM_LENGTH);

    return IIMEString_Length(buf);
}

#endif /* KMX_LDB_VIETNAMESE */

#if defined(KMX_LDB_AMHARIC)
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

static void Amharic_GetMultitype(
    IIMEAmharicFullProvider     *pThis, 
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
	OUT LDB_CHAR*			pMultiList,
    OUT int*                count)
{
    if (key >= 'a' && key <= 'z')
    {
        int i = key - 'a';
        if (i < 17)
        {
            *count = 2;
            pMultiList[0] = (LDB_CHAR)(0x1200 + (i * 2) * 8);
            pMultiList[1] = (LDB_CHAR)(0x1200 + (i * 2 + 1) * 8);
            pMultiList[2] = 0;
        }
        else
        {
            *count = 1;
            pMultiList[0] = (LDB_CHAR)(0x1200 + (16 + i + 1) * 8);
            pMultiList[1] = 0;
        }
    }
    else if (key == '.')
    {
        IIME_BOOL fBackspace = IIME_FALSE;
        LDB_CHAR *p = pMultiList;
        int i;
        LDB_CHAR start_char;

        if (pThis->m_UnderChar)
            i = (int)(pThis->m_UnderChar);
        else{
            i = (int)prev_char;
            fBackspace = IIME_TRUE;
        }

        if (i >= 0x1200 && i <= 0x1357)
        {
            start_char = (LDB_CHAR)(i / 8 * 8);
        }
        else if (i == 0x1359)
        {
            start_char = 0x1218;
        }
        else if (i == 0x1358)
        {
            start_char = 0x1228;
        }
        else
        {
            pMultiList[0] = '.';
            pMultiList[1] = 0;
            *count = 1;
            return;
        }

        if (fBackspace)
            pThis->m_immContent.pCommit[0] = '\b';

        for (i = 0; i < 8; i++)
        {
            LDB_CHAR c = (LDB_CHAR)(start_char + i);

            if (!Amharic_FindSkipChar(c))
            {
                *p = c;
                p++;
            }
        }
        switch (start_char)
        {
        case 0x1218:
            *p = 0x1359;
            p++;
            break;
        case 0x1228:
            *p = 0x1358;
            p++;
            break;
        case 0x1348:
            *p = 0x135A;
            p++;
            break;
        }

        *count = p - pMultiList;
    }
    else if (key >= 0x1200 && key < 0x137F)
    {
        pMultiList[0] = key;
        pMultiList[1] = 0;
        *count  = 1;
    }


    return;
}

static int Amharic_ValidateInput(IIMEAmharicFullProvider *pThis, LDB_CHAR c)
{
    if ((c >= 'a' && c <= 'z') || c == '.')
        return 1;
    else if (c >= 0x1200 && c <= 0x137F)
        return 1;
    else
        return 0;
}
#endif /* KMX_LDB_AMHARIC */

#if defined(KMX_LDB_ARABIC) || defined(KMX_LDB_HEBREW) || defined(KMX_LDB_PERSIAN) || defined(KMX_LDB_URDU) || defined(KMX_LDB_UYGHUR)
static void Arabic_Hebrew_GetMultitype(
    IIMEAmharicFullProvider     *pThis, 
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
	OUT LDB_CHAR*			pMultiList,
    OUT int*                count)
{
    int   i;
	const LDB_CHAR	*pString = 0;

    if (!count)
        return;

    *count = 0;

    if (LatinFull_KeymapKey(pThis, key))
    {
        pString = LatinFull_KeymapValue(pThis, key);
    }
    else if (kmx_latin_is_alphabeta(&(pThis->m_Engine), key))
    {
        pMultiList[0] = key;
        *count = 1;

        return;
    }
    else
		return ;

    i = 0;
	while (*pString)
	{
        pMultiList[i++] = *pString;
		pString++;
	}

	if (i > 0)
		*count = i;

    return ;
}

static int Arabic_Hebrew_ValidateInput(IIMEAmharicFullProvider *pThis, LDB_CHAR c)
{
	if (LatinFull_KeymapKey(pThis, c))
		return 1;
	else if(kmx_latin_is_alphabeta(&(pThis->m_Engine), c))
        return 1;
    else
		return 0;
}
#endif /* defined(KMX_LDB_ARABIC) || defined(KMX_LDB_HEBREW) */

#if defined(KMX_LDB_ARABIC)
static const unsigned short araibc_trans[] = 
{
    0x0622, 0x0623, 0x0625, 0x0627
};

/*
    0x0644 0x0622 == 0xFEF5
    0x0644 0x0623 == 0xFEF7
    0x0644 0x0625 == 0xFFF9
    0x0644 0x0627 == 0xFEFB
*/

static int Arabic_Normalize(unsigned short *buf, IIMEAmharicFullProvider *pThis)
{
#if !defined(PROWAVE_QWERTY_DEFINITION)
    int i, j;
    unsigned short temp[MAX_ITEM_LENGTH];
    memset(temp, 0, sizeof(unsigned short) * MAX_ITEM_LENGTH);

    i = 0; 
    j = 0;
    for (i = 0; i < MAX_ITEM_LENGTH && j < MAX_ITEM_LENGTH; i++)
    {
        if (buf[i] > 0xFE00)
        {
            temp[j++] = 0x0644;
            temp[j++] = araibc_trans[(buf[i] - 0xFEF5) / 2];
        }
        else
            temp[j++] = buf[i];
    }

    memcpy(buf, temp, sizeof(unsigned short) * MAX_ITEM_LENGTH);
#endif
    return IIMEString_Length(buf);
}

#endif /* KMX_LDB_ARABIC */

//daodejing
#if defined(KMX_LDB_HINDI) || defined(KMX_LDB_BENGALI) || defined(KMX_LDB_TELUGU) || defined(KMX_LDB_MARATHI) || \
    defined(KMX_LDB_TAMIL) || defined(KMX_LDB_PUNJABI) || defined(KMX_LDB_GUJARATI) || defined(KMX_LDB_ORIYA) || \
    defined(KMX_LDB_KANNADA) || defined(KMX_LDB_MALAYALAM) || defined(KMX_LDB_KHMER) || defined(KMX_LDB_LAO) || \
    defined(KMX_LDB_MYANMAR) || defined(KMX_LDB_NEPALI) || defined(KMX_LDB_SINHALA) || defined(KMX_LDB_BODO) || \
    defined(KMX_LDB_DOGRI) || defined(KMX_LDB_SANSKRIT) || defined(KMX_LDB_KASHMIRI) || defined(KMX_LDB_KONKANI) || \
    defined(KMX_LDB_MAITHILI) || defined(KMX_LDB_MANIPURI) || defined(KMX_LDB_SANTALI) || defined(KMX_LDB_SOMALI) || defined(KMX_LDB_SINDHI)
static void Indic_GetMultitype(
    IIMEAmharicFullProvider     *pThis, 
    IN LDB_CHAR             key,
    IN LDB_CHAR             prev_char,
	OUT LDB_CHAR*			pMultiList,
    OUT int*                count)
{
    typedef int (*IS_VALID_WORD_START_CHAR_DB)(const void *database, LDB_CHAR c);
    typedef int (*IS_VALID_FOLLOW_CHAR_DB)(const void *database, LDB_CHAR c1, LDB_CHAR c2);

    typedef int (*IS_VALID_WORD_START_CHAR)(LDB_CHAR c);
    typedef int (*IS_VALID_FOLLOW_CHAR)(LDB_CHAR c1, LDB_CHAR c2);

    int                         i;
	const LDB_CHAR	*pString = 0;

    IS_VALID_WORD_START_CHAR_DB pfnIsValidStartChar_db = 0;
    IS_VALID_FOLLOW_CHAR_DB pfnIsValidFollowChar_db = 0;

    IS_VALID_WORD_START_CHAR pfnIsValidStartChar = 0;
    IS_VALID_FOLLOW_CHAR pfnIsValidFollowChar = 0;

    if (!count)
        return;

    *count = 0;

    if (LatinFull_KeymapKey(pThis, key))
    {
        pString = LatinFull_KeymapValue(pThis, key);
    }
    else if (pThis->m_Engine.engine != 0 && kmx_latin_is_alphabeta(&(pThis->m_Engine), key))
    {
        pMultiList[0] = key;
        *count = 1;
        return;
    }
	else
		return ;

    switch (pThis->m_language)
    {
#if defined(KMX_LDB_HINDI)
    case IIME_LANG_HINDI:
        pfnIsValidStartChar = kmx_is_valid_hindi_word_start_char;
        pfnIsValidFollowChar = kmx_is_valid_hindi_follow_char;
        break;
#endif

#if defined(KMX_LDB_NEPALI)
    case IIME_LANG_NEPALI:
        pfnIsValidStartChar = kmx_is_valid_nepali_word_start_char;
        pfnIsValidFollowChar = kmx_is_valid_nepali_follow_char;
        break;
#endif

#if defined(KMX_LDB_SINHALA)
    case IIME_LANG_SINHALA:
        pfnIsValidStartChar = kmx_is_valid_sinhala_word_start_char;
        pfnIsValidFollowChar = kmx_is_valid_sinhala_follow_char;
        break;
#endif

//daodejing
#if defined(KMX_LDB_BODO)
    case IIME_LANG_BODO:
        pfnIsValidStartChar = kmx_is_valid_bodo_word_start_char;
        pfnIsValidFollowChar = kmx_is_valid_bodo_follow_char;
        break;
#endif

#if defined(KMX_LDB_DOGRI)
    case IIME_LANG_DOGRI:
        pfnIsValidStartChar = kmx_is_valid_dogri_word_start_char;
        pfnIsValidFollowChar = kmx_is_valid_dogri_follow_char;
        break;
#endif

#if defined(KMX_LDB_SANSKRIT)
    case IIME_LANG_SANSKRIT:
        pfnIsValidStartChar = kmx_is_valid_sanskrit_word_start_char;
        pfnIsValidFollowChar = kmx_is_valid_sanskrit_follow_char;
        break;
#endif

#if defined(KMX_LDB_KASHMIRI)
    case IIME_LANG_KASHMIRI:
        pfnIsValidStartChar = kmx_is_valid_kashmiri_word_start_char;
        pfnIsValidFollowChar = kmx_is_valid_kashmiri_follow_char;
        break;
#endif

#if defined(KMX_LDB_KONKANI)
    case IIME_LANG_KONKANI:
        pfnIsValidStartChar = kmx_is_valid_konkani_word_start_char;
        pfnIsValidFollowChar = kmx_is_valid_konkani_follow_char;
        break;
#endif

#if defined(KMX_LDB_MAITHILI)
    case IIME_LANG_MAITHILI:
        pfnIsValidStartChar = kmx_is_valid_maithili_word_start_char;
        pfnIsValidFollowChar = kmx_is_valid_maithili_follow_char;
        break;
#endif

#if defined(KMX_LDB_MANIPURI)
    case IIME_LANG_MANIPURI:
        pfnIsValidStartChar = kmx_is_valid_manipuri_word_start_char;
        pfnIsValidFollowChar = kmx_is_valid_manipuri_follow_char;
        break;
#endif

#if defined(KMX_LDB_SANTALI)
    case IIME_LANG_SANTALI:
        pfnIsValidStartChar = kmx_is_valid_santali_word_start_char;
        pfnIsValidFollowChar = kmx_is_valid_santali_follow_char;
        break;
#endif

#if defined(KMX_LDB_SOMALI)
    case IIME_LANG_SOMALI:
        pfnIsValidStartChar = kmx_is_valid_somali_word_start_char;
        pfnIsValidFollowChar = kmx_is_valid_somali_follow_char;
        break;
#endif

#if defined(KMX_LDB_SINDHI)
    case IIME_LANG_SINDHI:
        pfnIsValidStartChar = kmx_is_valid_sindhi_word_start_char;
        pfnIsValidFollowChar = kmx_is_valid_sindhi_follow_char;
        break;
#endif

#if defined(KMX_LDB_BENGALI) || defined(KMX_LDB_ASSAMESE)
    case IIME_LANG_BENGALI:
    case IIME_LANG_ASSAMESE:
        pfnIsValidStartChar = kmx_is_valid_bengali_word_start_char;
        pfnIsValidFollowChar = kmx_is_valid_bengali_follow_char;
        break;
#endif
#if defined(KMX_LDB_TELUGU)
    case IIME_LANG_TELUGU:
        pfnIsValidStartChar_db = kmx_is_valid_telugu_word_start_char;
        pfnIsValidFollowChar_db = kmx_is_valid_telugu_follow_char;
        break;
#endif
#if defined(KMX_LDB_MARATHI)
    case IIME_LANG_MARATHI:
        pfnIsValidStartChar_db = kmx_is_valid_marathi_word_start_char;
        pfnIsValidFollowChar_db = kmx_is_valid_marathi_follow_char;
        break;
#endif
#if defined(KMX_LDB_TAMIL)
    case IIME_LANG_TAMIL:
        pfnIsValidStartChar_db = kmx_is_valid_tamil_word_start_char;
        pfnIsValidFollowChar_db = kmx_is_valid_tamil_follow_char;
        break;
#endif
#if defined(KMX_LDB_PUNJABI)
    case IIME_LANG_PUNJABI:
        pfnIsValidStartChar_db = kmx_is_valid_punjabi_word_start_char;
        pfnIsValidFollowChar_db = kmx_is_valid_punjabi_follow_char;
        break;
#endif
#if defined(KMX_LDB_GUJARATI)
    case IIME_LANG_GUJARATI:
        pfnIsValidStartChar_db = kmx_is_valid_gujarati_word_start_char;
        pfnIsValidFollowChar_db = kmx_is_valid_gujarati_follow_char;
        break;
#endif
#if defined(KMX_LDB_ORIYA)
    case IIME_LANG_ORIYA:
        pfnIsValidStartChar_db = kmx_is_valid_oriya_word_start_char;
        pfnIsValidFollowChar_db = kmx_is_valid_oriya_follow_char;
        break;
#endif
#if defined(KMX_LDB_KANNADA)
    case IIME_LANG_KANNADA:
        pfnIsValidStartChar_db = kmx_is_valid_kannada_word_start_char;
        pfnIsValidFollowChar_db = kmx_is_valid_kannada_follow_char;
        break;
#endif
#if defined(KMX_LDB_MALAYALAM)
    case IIME_LANG_MALAYALAM:
        pfnIsValidStartChar_db = kmx_is_valid_malayalam_word_start_char;
        pfnIsValidFollowChar_db = kmx_is_valid_malayalam_follow_char;
        break;
#endif
#if defined(KMX_LDB_KHMER)
    case IIME_LANG_KHMER:
        pfnIsValidStartChar_db = kmx_is_valid_khmer_word_start_char;
        pfnIsValidFollowChar_db = kmx_is_valid_khmer_follow_char;
        break;
#endif
#if defined(KMX_LDB_LAO)
    case IIME_LANG_LAO:
        pfnIsValidStartChar_db = kmx_is_valid_lao_word_start_char;
        pfnIsValidFollowChar_db = kmx_is_valid_lao_follow_char;
        break;
#endif
#if defined(KMX_LDB_MYANMAR)
    case IIME_LANG_MYANMAR:
        pfnIsValidStartChar_db = kmx_is_valid_myanmar_word_start_char;
        pfnIsValidFollowChar_db = kmx_is_valid_myanmar_follow_char;
        break;
#endif

    default:
        return;
    }

	if (prev_char == 0)
	{
		i = 0;
		while (*pString)
		{
            if (pfnIsValidStartChar)
            {
                if (pfnIsValidStartChar(*pString))
				    pMultiList[i++] = *pString;
            }
            else if (pfnIsValidStartChar_db)
            {
                if (pfnIsValidStartChar_db(pThis->pdbMisc, *pString))
				    pMultiList[i++] = *pString;
            }
			pString++;
		}
	}
	else
	{
		i = 0;
		while (*pString)
		{
            if (pfnIsValidFollowChar)
            {
                if (pfnIsValidFollowChar(prev_char, *pString))
				    pMultiList[i++] = *pString;
            }
            else if (pfnIsValidFollowChar_db)
            {
                if (pfnIsValidFollowChar_db(pThis->pdbMisc, prev_char, *pString))
				    pMultiList[i++] = *pString;
            }
			pString++;
		}
	}

	if (i > 0)
	{
		*count = i;
		return ;
	}
	else
	{
		*count = 0;
		return ;
	}
}

static int Indic_ValidateInput(IIMEAmharicFullProvider *pThis, LDB_CHAR c)
{
	if (LatinFull_KeymapKey(pThis, c))
		return 1;
    else if (pThis->m_Engine.engine != 0 && kmx_latin_is_alphabeta(&(pThis->m_Engine), c))
        return 1;
	else
		return 0;
}

#endif	/* defined(KMX_LDB_HINDI) || defined(KMX_LDB_BENGALI) */

static void LatinFull_GetSymList(IIMEAmharicFullProvider *pThis, LDB_CHAR* pMultiList, int* pCount)
{
    const unsigned short *pSymbol = 0;
    unsigned short pos, match_pos;
    KMX_LATIN_MATCH_RESULT match_result;

    pos = 0;
    match_pos = 0;

    pSymbol = LatinFull_KeymapValue(pThis, SYMBOL_KEY);
    if (pSymbol)
    {
        while(*pSymbol)
        {
            *pMultiList = *pSymbol;
            pMultiList++;
            pSymbol++;
            pos++;
        }
    }
    else
    {
        if (pThis->m_Engine.engine == 0)
        {
            *pCount = 0;
            return;
        }

        while(kmx_latin_match(&(pThis->m_Engine), NULL, "1", 1, 0, match_pos, &match_result))
        {
            int i;
            for (i = 0; i < match_result.count; i++)
            {
                if (match_result.table[i].string[0] != '1')
                    pMultiList[pos++] = match_result.table[i].string[0];
            }

            if (match_result.more_available == 0)
                break;
            else
                match_pos = (unsigned short )(match_pos + match_result.count);
        }
    }

    if (pos > 0)
        *pCount = pos;
    else
        *pCount = 0;
}

static const QWERTY_KEYMAP* LatinFull_FindKeymap(IIMEAmharicFullProvider *pThis, const IIME_QWERTY_INFO* pQwertyInfo)
{
    const QWERTY_KEYMAP* keymap = 0;
    const IIME_QWERTY_INFO *qwerty_info = pQwertyInfo;
    while (qwerty_info->lang)
    {
        if (qwerty_info->lang== pThis->m_language)
        {
            keymap = qwerty_info->keymap;
            break;
        }

        qwerty_info++;
    }

    return keymap;
}

static int LatinFull_KeymapKey(IIMEAmharicFullProvider *pThis, unsigned short key)
{
    const QWERTY_KEYMAP* keymap = pThis->m_keymap;
    if (key == SYMBOL_KEY)
        return 0;

    if (!keymap)
        return 0;

    while (keymap->key)
    {
        if (key == keymap->key)
            return 1;

        keymap++;
    }

    return 0;
}

static const unsigned short * LatinFull_KeymapValue(IIMEAmharicFullProvider *pThis, unsigned short key)
{
    const QWERTY_KEYMAP* keymap = pThis->m_keymap;
    if (!keymap)
        return 0;

    while (keymap->key)
    {
        if (key == keymap->key)
            return keymap->value;

        keymap++;
    }

    return 0;
}
#endif

