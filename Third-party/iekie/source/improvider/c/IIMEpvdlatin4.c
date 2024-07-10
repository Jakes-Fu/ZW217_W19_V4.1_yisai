
/****************************************************************************
 *
 * IIMEpvdlatin4.c
 *
 */
#if 0
#include "IIMEstd.h"
#include "provider_com.h"

#include "IIMEpvdlatin4.h"
#include "ldbapi4.h"
#else
#include "IIMEstd.h"
#include "provider_com.h"

#include "IIMEpvdlatin4.h"
#include "ldbapi4.h"
#endif

#include "IIMEstring.h"


#ifdef KMX_THI_MT_CANDIDATE_BOX
extern int IIME_ime_is_thai_mt(void);
extern int IIME_ime_is_en_mt(void);
extern IIME_ime_is_en_Abc_mt();
extern int IIME_ime_is_en_abc_mt(void);
#endif
#ifndef IIME_VK_10
    #define IIME_VK_10    0x3A
    #define IIME_VK_11    0x3B
    #define IIME_VK_12    0x3C
    #define IIME_VK_13    0x3D
    #define IIME_VK_14    0x3E
    #define IIME_VK_15    0x3F
#endif

/****************************************************************************
 *
 * Configuration options
 *
 */

/*
 * To enable eudw navigation wrapping:
 *   #define OPT_EUDW_WRAP
 */

/*
 * To enable extra space for empty edit items:
 *   #define OPT_EXTRA_SPACE
 */

/****************************************************************************
 *
 * product identification
 *
 */

#define OPT_PID_KEY_LEN     24

static const IIME_WCHAR OPT_PID_KEY_STR[OPT_PID_KEY_LEN] =
{
    LDB_KEY_6, LDB_KEY_8, LDB_KEY_4, LDB_KEY_3, LDB_KEY_5, LDB_KEY_9, LDB_KEY_9, LDB_KEY_5,
    LDB_KEY_6, LDB_KEY_8, LDB_KEY_4, LDB_KEY_3, LDB_KEY_5, LDB_KEY_9, LDB_KEY_9, LDB_KEY_5,
    LDB_KEY_6, LDB_KEY_8, LDB_KEY_4, LDB_KEY_3, LDB_KEY_5, LDB_KEY_9, LDB_KEY_9, LDB_KEY_5
};

static int match_pid_key(const IIME_WCHAR* key, int len)
{
    int i;

    if (len == OPT_PID_KEY_LEN)
    {
        for (i = 0; i < OPT_PID_KEY_LEN; i++)
        {
            if (key[i] != OPT_PID_KEY_STR[i])
                return 0;
        }
        return 1;
    }

    return 0;
}

#ifndef IIME_PID_NAME0
    #error Missing definition for IIME_PID_NAME0
#endif

#ifndef IIME_PID_NAME1
    #error Missing definition for IIME_PID_NAME1
#endif

#ifndef IIME_PID_NAME2
    #error Missing definition for IIME_PID_NAME2
#endif

#ifndef IIME_PID_NAME3
    #error Missing definition for IIME_PID_NAME3
#endif

#ifndef IIME_PID_NAME4
    #error Missing definition for IIME_PID_NAME4
#endif

#ifndef IIME_PID_NAME5
    #error Missing definition for IIME_PID_NAME5
#endif

#ifndef IIME_PID_VERMAJOR
    #error Missing definition for IIME_PID_VERMAJOR
#endif

#ifndef IIME_PID_VERMINOR
    #error Missing definition for IIME_PID_VERMINOR
#endif

static const char OPT_PID_NAME[6] =
{
    IIME_PID_NAME0 - 19,
    IIME_PID_NAME1 - 19,
    IIME_PID_NAME2 - 19,
    IIME_PID_NAME3 - 19,
    IIME_PID_NAME4 - 19,
    IIME_PID_NAME5 - 19
};

static const char OPT_PID_DATE[] = __DATE__;

#define OPT_STR(x)  # x
#define OPT_XSTR(x) OPT_STR(x)
#define OPT_VERSTR  OPT_XSTR(IIME_PID_VERMAJOR) OPT_XSTR(IIME_PID_VERMINOR)

static const char OPT_PID_VER[] = OPT_VERSTR;

static void setup_pid_string(IIME_WCHAR* s)
{
    int i;

    /* PID name */
    for (i = 0; i < 6; i++)
        s[i] = (IIME_WCHAR)(OPT_PID_NAME[i] + 19);

    /* PID year */
    for (i = 0; i < 2; i++)
        s[6+i] = (IIME_WCHAR)(OPT_PID_DATE[9+i]);

    /* PID month */
    for (i = 0; i < 3; i++)
        s[8+i] = (IIME_WCHAR)(OPT_PID_DATE[i]);

    /* PID day */
    if (OPT_PID_DATE[4] == 0x20)
        s[11] = 0x30;
    else
        s[11] = (IIME_WCHAR)(OPT_PID_DATE[4]);
    s[12] = (IIME_WCHAR)(OPT_PID_DATE[5]);

    for (i = 0; i < 3; i++)
        s[13+i] = (IIME_WCHAR)(OPT_PID_VER[i]);
}

/****************************************************************************
 *
 * Content limits
 *
 */

#define CNT_MAX_COMMIT      64      /* IIME_IMM_CONTENT.pCommit         */
#define CNT_MAX_EDIT        64      /* IIME_IMM_CONTENT.pPreBuffer      */
#define CNT_MAX_CAND        32      /* IIME_IMM_CONTENT.pCandBuffer     */
#define CNT_MAX_USER        64      /* IIME_IMM_CONTENT.pUserWordBuffer */

/****************************************************************************
 *
 * Private data limits
 *
 */

/* Edit item buffer size */
#define EDIT_BUFFER_SIZE    CNT_MAX_EDIT

/* Limit of edit item string length */
#define EDIT_LENGTH_LIMIT   (EDIT_BUFFER_SIZE - 1)

/* Edit list size */
#define EDIT_LIST_SIZE      (CNT_MAX_EDIT / 2)

/* Commit buffer size */
#define COMMIT_BUFFER_SIZE  CNT_MAX_COMMIT

/* Limit of commit string length */
#define COMMIT_LENGTH_LIMIT (COMMIT_BUFFER_SIZE - 1)

/* Candidate list size */
#define CAND_LIST_SIZE      64

/* EUDW buffer size */
#define EUDW_BUFFER_SIZE    EDIT_BUFFER_SIZE

#define MAX_MULTITAP_LENGTH 48
/****************************************************************************
 *
 * Private data defines
 *
 */

#ifndef IIME_VK_LONGBACKSPACE
    #define IIME_VK_LONGBACKSPACE (IIME_VK_BACKSPACE | IIME_VK_LONGCLICK)
#endif

/* Constants for THIS_DATA.state */

#define STATE_EDIT          1
#define STATE_CAND          2
#define STATE_EUDW          3

/* Constants for flags */

#define FLAG_MULTITAP       0x0001  /* Set if multitap actived */
#define FLAG_ACTIVED        0x0002  /* Set if actived */

#define FLAG_SMART_CASE     0x0010
#define FLAG_LOWER_CASE     0x0020
#define FLAG_UPPER_CASE     0x0040
#define FLAG_INITIAL_CASE   0x0080
#define FLAG_HINT_ICASE     0x0100  /* Set if next phase start with initial case */

#define FLAG_STAR_AS_ONE    0x0200

#define FLAG_EDIT_DIRTY     0x1000
#define FLAG_CAND_DIRTY     0x2000
#define FLAG_EUDW_DIRTY     0x4000
#define FLAG_COMMIT         0x8000

#define FLAG_EUDW_DISABLED  0x10000
#define FLAG_WORD_INPUT     0x20000

#define FLAG_MATCH_KEY      0x0001

#define FLAG_PAGE_START     0x0001
#define FLAG_CAND_EUDW      0x0002

/* Constants for options */

#define OPTION_CASE_SENSITIVE_LANGUAGE      0x0001
#define OPTION_RIGHT_TO_LEFT_LANGUAGE       0x0002
#define OPTION_COMPLEX_RENDERING_LANGUAGE   0x0004
#define OPTION_ENABLE_KEY0_CHAR_MAP         0x0008
#define OPTION_ENABLE_KEY_STAR_CHAR_MAP     0x0100
#define OPTION_SINGLE_WORD_EDIT             0x0010
#define OPTION_UPPER_CASE_ALWAYS            0x0020
#define OPTION_LOWER_CASE_ALWAYS            0x0040
#define OPTION_INITIAL_CASE_ALWAYS          0x0080

#define OPTION_STAR_AS_ONE                  0x0200
#define OPTION_ARABIC_WITHOUT_SORT			0x0400

#define OPTION_ENABLE_KEY_POUND_CHAR_MAP     0x0800

typedef struct tagEDIT
{
    int                     pos;
    int                     len;    /* Length in chars, can be zero */
    int                     flags;
} EDIT;

typedef struct tagCAND
{
    int                     len;
    const IIME_WCHAR*         str;
    int                     width;
    int                     flags;
} CAND;

typedef struct tagTHIS_DATA
{
    int                     options;
    int                     flags;
    int                     state;

    /* Edit info */
    EDIT                    edit[EDIT_LIST_SIZE];   /* Edit item list */
    IIME_WCHAR                edit_str[EDIT_BUFFER_SIZE];
    IIME_WCHAR                edit_key[EDIT_BUFFER_SIZE];
    int                     ecnt;   /* Edit item count */
    int                     ecur;   /* Current edit item index */

    /* Candidate info */
    CAND                    cand[CAND_LIST_SIZE];   /* Candidate list */
    int                     ccnt;   /* Total candidates count */
    int                     cpfi;   /* Index of first candidate at current page */
    int                     cpct;   /* Candidates count at current page */
    int                     cpcu;   /* Offset of selected candidate at current page */

    /* EUDW info */
    IIME_WCHAR                ustr[EUDW_BUFFER_SIZE]; /* Eudw string */
    int                     ulen;   /* Eudw string length */
    int                     upos;   /* Caret position in eudw string */
    int                     umax;   /* Eudw string length limit */
    IIME_WCHAR                umts[MAX_MULTITAP_LENGTH];   /* Multitap list */
    int                     umtl;   /* Multitap list length */
    int                     umtp;   /* Multitap rolling position */
    int                     umtk;   /* Multitap key */

    /* Commit info */
    int                     commit_len;
    IIME_WCHAR                commit[COMMIT_BUFFER_SIZE];

    /* UI parameters to calculate candidate count in page */
    IIME_GET_TEXT_WIDTH_FUNC  pGetTextWidthFunc;
    int                     label_text_width;
    int                     min_candidate_width;
    int                     total_window_width;

    /* Database engine object */
    int                     language;
    int                     is_sure_type;
    void*                   dbobj;

    /* Cached info from database engine */
    int                     emax;   /* Max edit item length set by database engine */
    const IIME_WCHAR*         eudwp;
    int                     eudwp_len;
    const IIME_WCHAR*         eudwt;
    int                     eudwt_len;
    
    /* call back for get context */
    CALLBACK_GET_CONTEXT    m_pfnGetContext;    
	#ifdef KMX_THI_MT_CANDIDATE_BOX
	IIME_WCHAR	m_wPreChar;
	#endif
} THIS_DATA;

/****************************************************************************
 *
 * Helper defines
 *
 */

/* Characters constants in UTF16 */

#define CHAR_SPACE          0x20
#define CHAR_NUM0           0x30
#define CHAR_NUM9           0x39

/* For calculating string byte size */

#define STR_BYTES(count)    (sizeof(IIME_WCHAR) * (count))

/* Shortcuts to test/set/clear flag */

#define IS_FLAG_SET(n, f)   (((n) & (f)) == (f))
#define IS_FLAG_CLR(n, f)   (((n) & (f)) != (f))

#define SET_FLAG(n, f)      (n) |= (f)
#define CLR_FLAG(n, f)      (n) &= ~(f)

/****************************************************************************
 *
 * Forward declaration of local functions
 *
 */

/* MODE functions */

static void InitDefaultMode(
    THIS_DATA*              pThis);

/* EDIT functions */

static void SwitchEditState(
    THIS_DATA*              pThis);

#if 0
static int OnEditKeyQuery(
    THIS_DATA*              pThis,
    int                     key);
#endif
static int OnEditKeyEvent(
    THIS_DATA*              pThis,
    int                     key);

static void OnEditNew(
    THIS_DATA*              pThis);

static void OnEditAppend(
    THIS_DATA*              pThis,
    int                     key);

static void OnEditDelete(
    THIS_DATA*              pThis,
    int                     all);

static void OnEditDeleteItem(
    THIS_DATA*              pThis);

static void OnEditReplace(
    THIS_DATA*              pThis,
    const IIME_WCHAR*         buffer,
    int                     length);

static void OnEditSwitchCase(
    THIS_DATA*              pThis);

static void OnEditPrev(
    THIS_DATA*              pThis);

static void OnEditNext(
    THIS_DATA*              pThis);

static void OnEditCommit(
    THIS_DATA*              pThis);

/* CANDIDATE functions */

static void SwitchCandState(
    THIS_DATA*              pThis);

#if 0
static int OnCandKeyQuery(
    THIS_DATA*              pThis,
    int                     key);
#endif

static int OnCandKeyEvent(
    THIS_DATA*              pThis,
    int                     key);

static void OnCandCommit(
    THIS_DATA*              pThis,
    int                     index);

static void OnCandLocateFocus(
    THIS_DATA*              pThis,
    int                     index);

static void OnCandSync(
    THIS_DATA*              pThis);

static void OnCandFocusPrev(
    THIS_DATA*              pThis);

static void OnCandFocusNext(
    THIS_DATA*              pThis);

static void OnCandFocusPrevPage(
    THIS_DATA*              pThis);

static void OnCandFocusNextPage(
    THIS_DATA*              pThis);

/* EUDW functions */

static void SwitchEudwState(
    THIS_DATA*              pThis);

#if 0
static int OnEudwKeyQuery(
    THIS_DATA*              pThis,
    int                     key);
#endif

static int OnEudwKeyEvent(
    THIS_DATA*              pThis,
    int                     key);

static void OnEudwTimerEvent(
    THIS_DATA*              pThis);

static void OnEudwMultitapKey(
    THIS_DATA*              pThis,
    int                     key);

static void OnEudwMultitapStart(
    THIS_DATA*              pThis,
    int                     key);

static void OnEudwMultitapScroll(
    THIS_DATA*              pThis);

static void OnEudwMultitapStop(
    THIS_DATA*              pThis);

static void OnEudwAuxKey(
    THIS_DATA*              pThis);

static void OnEudwDelete(
    THIS_DATA*              pThis,
    int                     all);

static void OnEudwCaretBackward(
    THIS_DATA*              pThis);

static void OnEudwCaretForward(
    THIS_DATA*              pThis);

static void OnEudwCommit(
    THIS_DATA*              pThis);

static void OnEudwCancel(
    THIS_DATA*              pThis);

/* MISC functions */

static int GetEditFreeSpace(
    THIS_DATA*              pThis);

static int GetHistory(
    THIS_DATA*              pThis,
    IIME_WCHAR*               buffer,
    int                     max_chars);

static void GenerateCandidates(
    THIS_DATA*              pThis);

static int IsEudwCandidate(
    THIS_DATA*              pThis,
    int                     index);

static void GetContent(
    THIS_DATA*              pThis,
    PIIME_IMM_CONTENT         pContent);

static void InitWidthParams(
    THIS_DATA*              pThis,
    const IIME_IMM_PARAMS*    imm_params);

static int GetCandStrWidth(
    THIS_DATA*              pThis,
    const IIME_WCHAR*         str,
    int                     len);
/****************************************************************************
 *
 * Implementation of interface functions
 *
 */

static IIME_RESULT IIMELatin4Provider_EventHandler(
    Provider_Com*           pIMP,
    IIME_UINT                 hEvent,
    IIME_UINT                 iMsg,
    PIIME_IMM_CONTENT         pContent)
{
    THIS_DATA*              pThis;
    int                     event;
    int                     key;
    int                     is_handled;
    int                     pflags;

    pThis = (THIS_DATA*)(((IIMELatin4Provider*)pIMP)->m_pData);

    event = hEvent;
    key = iMsg;
#ifdef KMX_THI_MT_CANDIDATE_BOX
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
#endif
    #ifdef KEYBOARD_IME_SHARP
	     if (key == IIME_VK_0)
            {
                #ifdef KMX_THI_MT_CANDIDATE_BOX
                if(!IIME_ime_is_thai_mt()&&!IIME_ime_is_en_mt())
                #endif
	        key = IIME_VK_SPACE;
            }
	     else if (key == IIME_VK_STAR && !IS_FLAG_SET(pThis->options, OPTION_ENABLE_KEY_STAR_CHAR_MAP))
		    key = IIME_VK_AUX;
    #endif

    /* Save a copy of current flags */

    pflags = pThis->flags;

    /* No commit by default */

    CLR_FLAG(pThis->flags, FLAG_COMMIT);

    /* Handle event */

    is_handled = 0;

    switch (event)
    {
        case IIME_EVENT_KEYBOARD:
            switch (pThis->state)
            {
                case STATE_EDIT:
                    is_handled = OnEditKeyEvent(pThis, key);
                    break;

                case STATE_CAND:
                    is_handled = OnCandKeyEvent(pThis, key);
                    break;

                case STATE_EUDW:
                    is_handled = OnEudwKeyEvent(pThis, key);
                    break;
            }
            break;

        case IIME_EVENT_TIMER:
            is_handled = 1;
            if (pThis->state == STATE_EUDW)
                OnEudwTimerEvent(pThis);
            break;
    }

    /* Generate output content */

    if (pContent)
    {
        GetContent(pThis, pContent);
        /*
         * Turn on timer when multitap is actived.
         * Turn off timer when it's not needed.
         */
        if (IS_FLAG_SET(pThis->flags, FLAG_MULTITAP))
        {
            SET_FLAG(pContent->flagTimer, IIME_TIMER_SET);
        }
        else
        {
            if ( IS_FLAG_SET(pflags, FLAG_MULTITAP) || (event == IIME_EVENT_TIMER) )
                SET_FLAG(pContent->flagTimer, IIME_TIMER_CLEAR);
        }
    }

    return is_handled ? IIME_ERROR_USED : IIME_ERROR_UNUSED;
}

static IIME_RESULT IIMELatin4Provider_SetOptions(
    Provider_Com*           pIMP,
    const IIME_IMM_OPTIONS*   pOption)
{
    THIS_DATA*              pThis;

    pThis = (THIS_DATA*)(((IIMELatin4Provider*)pIMP)->m_pData);

    if (pOption->dwPYFuzzy & IIME_EUDW_DISABLED){
        SET_FLAG(pThis->flags, FLAG_EUDW_DISABLED);
    }
    else
    {
        #ifdef KMX_THI_MT_CANDIDATE_BOX
        if(IIME_ime_is_thai_mt()||IIME_ime_is_en_mt())
        {
            SET_FLAG(pThis->flags, FLAG_EUDW_DISABLED);
        }
        else
        #endif
        CLR_FLAG(pThis->flags, FLAG_EUDW_DISABLED);
    }

    if (pOption->dwAlphaBeticInputMode == IIME_ALPHABETIC_WORD_INPUT)
    {
        SET_FLAG(pThis->flags, FLAG_WORD_INPUT);
        SET_FLAG(pThis->options, OPTION_SINGLE_WORD_EDIT);
    }
    else
    {
        CLR_FLAG(pThis->flags, FLAG_WORD_INPUT);
    }
    
    if (pOption->pfnGetContext)
        pThis->m_pfnGetContext = pOption->pfnGetContext;


    return IIME_ERROR_SUCCESS;
}
static IIME_RESULT IIMELatin4Provider_GetOptions(
    Provider_Com*           pIMP,
    IIME_IMM_OPTIONS*         pOption)
{
    /* No options provided currently.
       Do nothing by design. */

    return IIME_ERROR_SUCCESS;
}

static IIME_RESULT IIMELatin4Provider_AddPhrase(Provider_Com* pImp, unsigned short* szPhrase, int nLen)
{
    THIS_DATA* pThis;
    /* Without any check, may cause serious problem. */
    if (!pImp)
        return IIME_ERROR_UNKNOWN;
    
    pThis = (THIS_DATA*)(((IIMELatin4Provider*)pImp)->m_pData);

    if (!pThis)
        return IIME_ERROR_UNKNOWN;

    if (!pThis->dbobj)
        return IIME_ERROR_UNKNOWN;
    
    if (nLen > 0)
    {
        ldbapi4_update_user_data(pThis->dbobj, szPhrase, nLen, LDB_EUDW);
    }
    return IIME_ERROR_SUCCESS;
}

Provider_Com* IIMELatin4Provider_Create(
    PIIME_IMM_PARAMS          pImmParams,
    PIIME_USER_PARAMS         pUserParam1,
    PIIME_USER_PARAMS         pUserParam2,
    const void*				pdbMisc,
	const void*				pdbMO,
    const IIME_MULTI_STRING_TABLE* pMulti_string_table,
    IIME_INT                  idMethod)
{
    IIMELatin4Provider*       pProvider;
    THIS_DATA*              pThis;
    void*                   dbobj;
    LDB_INFO                dbinfo;
    LDB_INIT                dbinit;
    const void*             database;

    database = pdbMisc;

    /* Query database */

    if (!ldbapi4_get_db_info(database, &dbinfo))
    {
        /* Error: database not found */
        return 0;
    }

    if (dbinfo.database_id == 9201 || dbinfo.database_id == 8071 || dbinfo.database_id == 1071 ||
        dbinfo.database_id == 4671 || dbinfo.database_id == 9071 || dbinfo.database_id == 3671 ||
        dbinfo.database_id == 4071 || dbinfo.database_id == 4271 || dbinfo.database_id == 3171 || 
        dbinfo.database_id == 6071 || dbinfo.database_id == 5071 || dbinfo.database_id == 9171 || 
        dbinfo.database_id == 3271 || dbinfo.database_id == 3371 || dbinfo.database_id == 3471 || 
        dbinfo.database_id == 2071 || dbinfo.database_id == 9571)
    {
        /* bengali dummy database */
        return 0;
    }

    /*
     * Alloc memory for:
     *   1. provider object
     *   2. provider object private data
     *   3. database object
     */

    pProvider = (IIMELatin4Provider*) IIME_MALLOC(
        sizeof(IIMELatin4Provider) + sizeof(THIS_DATA) + dbinfo.engine_object_size);

    if (!pProvider)
    {
        /* Error: not enough memory */
        return 0;
    }

    pThis = (THIS_DATA*) ((char*)pProvider + sizeof(IIMELatin4Provider));

    /* Initialize provider object interface */

    memset(pProvider, 0, sizeof(IIMELatin4Provider));

    pProvider->m_pData = pThis;

    pProvider->m_pfnEventHandle           = IIMELatin4Provider_EventHandler;
    pProvider->m_pfnSetOptions            = IIMELatin4Provider_SetOptions;
    pProvider->m_pfnGetOptions            = IIMELatin4Provider_GetOptions;
    pProvider->m_pfnAddPhrase             = IIMELatin4Provider_AddPhrase;

    /* Initialize database object */
    
    dbobj = (char*)pThis + sizeof(THIS_DATA);

    memset(&dbinit, 0, sizeof(dbinit));
    dbinit.database = database;
    dbinit.user_data = pUserParam1->pUserCache;
    dbinit.user_data_size = pUserParam1->iUserCacheSize;

    if (!ldbapi4_initialize(dbobj, &dbinit))
    {
        /* Error: invalid database */
        IIME_FREE(pProvider);
        return 0;
    }

    /* Initialize provider object */

    memset(pThis, 0, sizeof(THIS_DATA));

	pThis->language = dbinfo.language;
    pThis->is_sure_type = dbinfo.is_sure_type;
    pThis->dbobj = dbobj;

    pThis->emax = dbinfo.max_input_length;
    if (pThis->emax > CNT_MAX_CAND - 1)
        pThis->emax = CNT_MAX_CAND - 1;

    pThis->eudwp = dbinfo.eudw_prompt;
    pThis->eudwp_len = dbinfo.eudw_prompt_length;
    
    pThis->eudwt = dbinfo.eudw_title;
    pThis->eudwt_len = dbinfo.eudw_title_length;

    InitWidthParams(pThis, pImmParams);

    InitDefaultMode(pThis);

    switch (idMethod % 10)
    {
        case 7:
            SET_FLAG(pThis->options, OPTION_INITIAL_CASE_ALWAYS);
            SET_FLAG(pThis->flags, FLAG_INITIAL_CASE);
            break;
        case 8:
            SET_FLAG(pThis->options, OPTION_UPPER_CASE_ALWAYS);
            SET_FLAG(pThis->flags, FLAG_UPPER_CASE);
            break;
        case 9:
            SET_FLAG(pThis->options, OPTION_LOWER_CASE_ALWAYS);
            SET_FLAG(pThis->flags, FLAG_LOWER_CASE);
            break;
        default:
            SET_FLAG(pThis->flags, FLAG_SMART_CASE);
            break;
    }

    /* Initialize various language dependent options */

    switch (pThis->language)
    {
        case IIME_LANG_CHS:
        case IIME_LANG_CHT:
        case IIME_LANG_THAI:
        case IIME_LANG_HEBREW:
        case IIME_LANG_ARABIC:
        case IIME_LANG_PERSIAN:
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
        case IIME_LANG_UYGHUR:    
        case IIME_LANG_URDU:            
            CLR_FLAG(pThis->options, OPTION_CASE_SENSITIVE_LANGUAGE);
            break;
        default:
            SET_FLAG(pThis->options, OPTION_CASE_SENSITIVE_LANGUAGE);
            break;
    }

    switch (pThis->language)
    {
        case IIME_LANG_HEBREW:
        case IIME_LANG_ARABIC:
        case IIME_LANG_PERSIAN:
        case IIME_LANG_UYGHUR:
        case IIME_LANG_URDU:
            SET_FLAG(pThis->options, OPTION_RIGHT_TO_LEFT_LANGUAGE);
            break;
        default:
            CLR_FLAG(pThis->options, OPTION_RIGHT_TO_LEFT_LANGUAGE);
            break;
    }

    switch (pThis->language)
    {
        case IIME_LANG_THAI:
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
        case IIME_LANG_HEBREW:
        case IIME_LANG_ARABIC:
        case IIME_LANG_PERSIAN:
        case IIME_LANG_UYGHUR:
        case IIME_LANG_URDU:
            SET_FLAG(pThis->options, OPTION_COMPLEX_RENDERING_LANGUAGE);
            break;
        default:
            CLR_FLAG(pThis->options, OPTION_COMPLEX_RENDERING_LANGUAGE);
            break;
    }

    switch (pThis->language)
    {
        case IIME_LANG_THAI:
            SET_FLAG(pThis->options, OPTION_ENABLE_KEY0_CHAR_MAP);
            break;
#ifdef KMX_LDB_HINDI        
        case IIME_LANG_HINDI:
            if (dbinfo.database_id == 902 || dbinfo.database_id == 909 || dbinfo.database_id == 903 || dbinfo.database_id == 9010 || dbinfo.database_id == 9015 || dbinfo.database_id == 9013 || dbinfo.database_id == 9018)
            SET_FLAG(pThis->options, OPTION_ENABLE_KEY0_CHAR_MAP);
            break;        
#endif
#ifdef KMX_LDB_NEPALI
        case IIME_LANG_NEPALI:
            if (dbinfo.database_id == 902)
            SET_FLAG(pThis->options, OPTION_ENABLE_KEY0_CHAR_MAP);
            break;        
#endif
#ifdef KMX_LDB_SINHALA:
        case IIME_LANG_SINHALA:
            if (dbinfo.database_id == 902)
            SET_FLAG(pThis->options, OPTION_ENABLE_KEY0_CHAR_MAP);
            break;
#endif
//daodejing
#ifdef KMX_LDB_BODO
        case IIME_LANG_BODO:
            if (dbinfo.database_id == 1100)
            SET_FLAG(pThis->options, OPTION_ENABLE_KEY0_CHAR_MAP);
            break;
#endif
#ifdef KMX_LDB_DOGRI
        case IIME_LANG_DOGRI:
            if (dbinfo.database_id == 1110)
            SET_FLAG(pThis->options, OPTION_ENABLE_KEY0_CHAR_MAP);
            break;
#endif
#ifdef KMX_LDB_SANSKRIT
        case IIME_LANG_SANSKRIT:
            if (dbinfo.database_id == 1120)
            SET_FLAG(pThis->options, OPTION_ENABLE_KEY0_CHAR_MAP);
            break;
#endif
#ifdef KMX_LDB_KASHMIRI
        case IIME_LANG_KASHMIRI:
            if (dbinfo.database_id == 1130)
            SET_FLAG(pThis->options, OPTION_ENABLE_KEY0_CHAR_MAP);
            break;
#endif
#ifdef KMX_LDB_KONKANI
        case IIME_LANG_KONKANI:
            if (dbinfo.database_id == 1140)
            SET_FLAG(pThis->options, OPTION_ENABLE_KEY0_CHAR_MAP);
            break;
#endif
#ifdef KMX_LDB_MAITHILI
        case IIME_LANG_MAITHILI:
            if (dbinfo.database_id == 1150)
            SET_FLAG(pThis->options, OPTION_ENABLE_KEY0_CHAR_MAP);
            break;
#endif
#ifdef KMX_LDB_MANIPURI
        case IIME_LANG_MANIPURI:
            if (dbinfo.database_id == 1160)
            SET_FLAG(pThis->options, OPTION_ENABLE_KEY0_CHAR_MAP);
            break;
#endif
#ifdef KMX_LDB_SANTALI
        case IIME_LANG_SANTALI:
            if (dbinfo.database_id == 1170)
            SET_FLAG(pThis->options, OPTION_ENABLE_KEY0_CHAR_MAP);
            break;
#endif
#ifdef KMX_LDB_SOMALI
        case IIME_LANG_SOMALI:
            if (dbinfo.database_id == 1180)
            SET_FLAG(pThis->options, OPTION_ENABLE_KEY0_CHAR_MAP);
            break;
#endif
#ifdef KMX_LDB_SINDHI
        case IIME_LANG_SINDHI:
            if (dbinfo.database_id == 1190)
            SET_FLAG(pThis->options, OPTION_ENABLE_KEY0_CHAR_MAP);
            break;
#endif
#ifdef KMX_LDB_BENGALI        
        case IIME_LANG_BENGALI:
            if (dbinfo.database_id == 923 || dbinfo.database_id == 924 || dbinfo.database_id == 921 || dbinfo.database_id == 928 || dbinfo.database_id == 922 || dbinfo.database_id == 929 || dbinfo.database_id == 9200 || dbinfo.database_id == 9202 || dbinfo.database_id == 9203)
            SET_FLAG(pThis->options, OPTION_ENABLE_KEY0_CHAR_MAP);
            break;        
#endif
        default:
            CLR_FLAG(pThis->options, OPTION_ENABLE_KEY0_CHAR_MAP);
            break;
    }

    switch (pThis->language)
    {
        case IIME_LANG_THAI:
            SET_FLAG(pThis->options, OPTION_SINGLE_WORD_EDIT);
            break;
	#ifdef KMX_THI_MT_CANDIDATE_BOX
	case IIME_LANG_ENGLISH:
		if(IIME_ime_is_en_mt())
		{
			SET_FLAG(pThis->options, OPTION_SINGLE_WORD_EDIT);
		}
		else
		{
			CLR_FLAG(pThis->options, OPTION_SINGLE_WORD_EDIT);
		}
		break;
	#endif
        default:
            CLR_FLAG(pThis->options, OPTION_SINGLE_WORD_EDIT);
            break;
    }

    if(ldbapi4_is_key_star_used(database))
        SET_FLAG(pThis->options, OPTION_ENABLE_KEY_STAR_CHAR_MAP);


    if(ldbapi4_is_key_pound_used(database))
        SET_FLAG(pThis->options, OPTION_ENABLE_KEY_POUND_CHAR_MAP);

#ifdef KMX_LDB_THAI        
    if (dbinfo.database_id == 366 || dbinfo.database_id == 369)
    {
        if (!IS_FLAG_SET(pThis->flags, FLAG_SMART_CASE))
        {
            SET_FLAG(pThis->options, OPTION_STAR_AS_ONE);
        }
    }
#endif

	if (dbinfo.database_id == 803 || dbinfo.database_id == 809)
	{
		SET_FLAG(pThis->options, OPTION_ARABIC_WITHOUT_SORT);
	}

	return (Provider_Com*) pProvider;
}

void IIMELatin4Provider_Delete(
    Provider_Com*           pProvider)
{
    THIS_DATA*              pThis;

    if (pProvider)
    {
        pThis = (THIS_DATA*)(((IIMELatin4Provider*)pProvider)->m_pData);

        if (pThis->dbobj)
        {
            ldbapi4_cleanup(pThis->dbobj);
            pThis->dbobj = 0;
        }

        IIME_FREE(pProvider);
    }
}
/****************************************************************************
 *
 * Implementation of mode functions.
 *
 */

static void InitDefaultMode(
    THIS_DATA*              pThis)
/*
 * Initialize to default mode.
 */
{
    pThis->flags = FLAG_HINT_ICASE;
    pThis->state = STATE_EDIT;

    pThis->ecnt = 0;
    pThis->ecur = 0;

    pThis->ccnt = 0;
    pThis->cpfi = 0;
    pThis->cpct = 0;
    pThis->cpcu = 0;
}

/****************************************************************************
 *
 * Implementation of EDIT functions.
 *
 */

static void SwitchEditState(
    THIS_DATA*              pThis)
/*
 * Switch to EDIT state.
 */
{
    pThis->state = STATE_EDIT;
    SET_FLAG(pThis->flags, FLAG_EDIT_DIRTY);
    CLR_FLAG(pThis->flags, FLAG_MULTITAP);
}

#if 0
static int OnEditKeyQuery(
    THIS_DATA*              pThis,
    int                     key)
/*
 * Query if given key is acceptable.
 *
 * Return non-zero if the key is acceptable.
 * Return zero otherwise.
 */
{
    int                     is_acceptable;

    is_acceptable = 0;

    switch (key)
    {
        case IIME_VK_0:
        case IIME_VK_1:
        case IIME_VK_2:
        case IIME_VK_3:
        case IIME_VK_4:
        case IIME_VK_5:
        case IIME_VK_6:
        case IIME_VK_7:
        case IIME_VK_8:
        case IIME_VK_9:
        case IIME_VK_10:
        case IIME_VK_11:
        case IIME_VK_12:
        case IIME_VK_13:
        case IIME_VK_14:
        case IIME_VK_15:
        case IIME_VK_SPACE:
            is_acceptable = 1;
            break;

        case IIME_VK_LEFT:
        case IIME_VK_RIGHT:
        case IIME_VK_UP:
        case IIME_VK_DOWN:
        case IIME_VK_BACKSPACE:
        case IIME_VK_LONGBACKSPACE:
        case IIME_VK_SELECT:
        case IIME_VK_AUX:
        case IIME_VK_STAR:
        case IIME_VK_POUND:
            is_acceptable = (pThis->ecnt > 0) ? 1 : 0;
            break;
    }

    return is_acceptable;
}
#endif
#ifdef KMX_THI_MT_CANDIDATE_BOX
int preKey = -1;
#endif
static int OnEditKeyEvent(
    THIS_DATA*              pThis,
    int                     key)
/*
 * Process given key event.
 *
 * Return non-zero if the event is handled.
 * Return zero otherwise.
 */
{
    int                     is_handled;

    is_handled = 0;

#ifdef KMX_THI_MT_CANDIDATE_BOX
    if(IIME_ime_is_thai_mt()||IIME_ime_is_en_mt())
    {
        if(key >= IIME_VK_0 && key<=IIME_VK_9)
        {
            preKey = key;
        }
    }
#endif
    switch (key)
    {
        case IIME_VK_0:
            is_handled = 1;
            #ifdef KMX_THI_MT_CANDIDATE_BOX
            if(IIME_ime_is_thai_mt()||IIME_ime_is_en_mt())
            {
                if (pThis->ecnt > 0)
                {
                    OnEditDelete(pThis, 0);
                }
                OnEditAppend(pThis, key);
                SwitchCandState(pThis);
            }else
            #endif
            if (pThis->ecnt == 0)
            {
                pThis->commit[0] = CHAR_NUM0;
                pThis->commit[1] = 0;
                pThis->commit_len = 1;
                SET_FLAG(pThis->flags, FLAG_COMMIT);
            }
            else
            {
                OnEditAppend(pThis, key);
            }
            break;

        case IIME_VK_1:
        case IIME_VK_2:
        case IIME_VK_3:
        case IIME_VK_4:
        case IIME_VK_5:
        case IIME_VK_6:
        case IIME_VK_7:
        case IIME_VK_8:
        case IIME_VK_9:
        case IIME_VK_10:
        case IIME_VK_11:
        case IIME_VK_12:
        case IIME_VK_13:
        case IIME_VK_14:
        case IIME_VK_15:
            is_handled = 1;
            #ifdef KMX_THI_MT_CANDIDATE_BOX
            if ((IIME_ime_is_thai_mt() ||IIME_ime_is_en_mt())&& pThis->ecnt > 0)
            {
                OnEditDelete(pThis, 0);
            }
            #endif
            OnEditAppend(pThis, key);
            #ifdef KMX_THI_MT_CANDIDATE_BOX
            if(IIME_ime_is_thai_mt()||IIME_ime_is_en_mt())
                SwitchCandState(pThis);
            #endif
            break;

        case IIME_VK_BACKSPACE:
            if (pThis->ecnt > 0)
            {
                is_handled = 1;
                OnEditDelete(pThis, 0);
            }
            break;

        case IIME_VK_LONGBACKSPACE:
            if (pThis->ecnt > 0)
            {
                is_handled = 1;
                OnEditDelete(pThis, 1);
            }
            break;

        case IIME_VK_SELECT:
            if (pThis->ecnt > 0)
            {
                is_handled = 1;
                OnEditCommit(pThis);
            }
            break;

        case IIME_VK_SPACE:
            is_handled = 1;
            if (IS_FLAG_SET(pThis->options, OPTION_ENABLE_KEY0_CHAR_MAP))
            {
                OnEditAppend(pThis, IIME_VK_0);
            }
            else if (pThis->ecnt == 0)
            {
                pThis->commit[0] = CHAR_SPACE;
                pThis->commit[1] = 0;
                pThis->commit_len = 1;
                SET_FLAG(pThis->flags, FLAG_COMMIT);
            }
            else
            {
                if (IS_FLAG_SET(pThis->flags, FLAG_WORD_INPUT))
                {
                    OnEditCommit(pThis);                    
                    pThis->commit[pThis->commit_len] = CHAR_SPACE;
                    pThis->commit_len++;
                    pThis->commit[pThis->commit_len] = 0;
                }
                else
                {
                    OnEditNew(pThis);
                }
            }
            break;

        case IIME_VK_STAR:
            if (IS_FLAG_SET(pThis->options, OPTION_ENABLE_KEY_STAR_CHAR_MAP))
            {
                is_handled = 1;
                OnEditAppend(pThis, IIME_VK_STAR);
            }
            else if (pThis->ecnt > 0)
                is_handled = 1;
            break;
        case IIME_VK_POUND:
            if (IS_FLAG_SET(pThis->options, OPTION_ENABLE_KEY_POUND_CHAR_MAP))
            {
                is_handled = 1;
                OnEditAppend(pThis, IIME_VK_POUND);
            }            
            else if (pThis->ecnt > 0)
                is_handled = 1;
            break;

        case IIME_VK_AUX:
            if (pThis->ecnt > 0)
            {
                is_handled = 1;
                if (IS_FLAG_SET(pThis->options, OPTION_CASE_SENSITIVE_LANGUAGE))
                {
                    if (IS_FLAG_SET(pThis->flags, FLAG_SMART_CASE))
                        OnEditSwitchCase(pThis);
                }
            }
            else if (IS_FLAG_SET(pThis->options, FLAG_STAR_AS_ONE))
            {
                SET_FLAG(pThis->flags, FLAG_STAR_AS_ONE);
                is_handled = 1;
                OnEditAppend(pThis, IIME_VK_1);
                CLR_FLAG(pThis->flags, FLAG_STAR_AS_ONE);
            }
            break;

        case IIME_VK_LEFT:
            if (IS_FLAG_SET(pThis->options, OPTION_RIGHT_TO_LEFT_LANGUAGE))
            {
                /*
                 * Right-To-Left language:
                 *      Left key moves to next item.
                 */
                if (pThis->ecnt > 0)
                {
                    is_handled = 1;
                    if (IS_FLAG_SET(pThis->options, OPTION_SINGLE_WORD_EDIT))
                    {
                        SwitchCandState(pThis);
                        OnCandFocusNext(pThis);
                    }
                    else if ( (pThis->ccnt > 0) && (pThis->ecur + 1 == pThis->ecnt) )
                    {
                        SwitchCandState(pThis);
                        OnCandFocusNext(pThis);
                    }
                    else
                    {
                        OnEditNext(pThis);
                    }
                }
            }
            else
            {
                /*
                 * Left-To-Right language:
                 *      Left key moves to previous item.
                 */
                if (pThis->ecnt > 0)
                {
                    is_handled = 1;
                    if (IS_FLAG_SET(pThis->options, OPTION_SINGLE_WORD_EDIT))
                    {
                    }
                    else
                    {
                        OnEditPrev(pThis);
                    }
                }
            }
            break;

        case IIME_VK_RIGHT:
            if (IS_FLAG_SET(pThis->options, OPTION_RIGHT_TO_LEFT_LANGUAGE))
            {
                /*
                 * Right-To-Left language:
                 *      Right key moves to previous item.
                 */
                if (pThis->ecnt > 0)
                {
                    is_handled = 1;
                    if (IS_FLAG_SET(pThis->options, OPTION_SINGLE_WORD_EDIT))
                    {
                    }
                    else
                    {
                        OnEditPrev(pThis);
                    }
                }
            }
            else
            {
                /*
                 * Left-To-Right language:
                 *      Right key moves to next item.
                 */
                if (pThis->ecnt > 0)
                {
                    is_handled = 1;
                    if (IS_FLAG_SET(pThis->options, OPTION_SINGLE_WORD_EDIT))
                    {
                        SwitchCandState(pThis);
                        OnCandFocusNext(pThis);
                    }
                    else if ( (pThis->ccnt > 0) && (pThis->ecur + 1 == pThis->ecnt) )
                    {
                        SwitchCandState(pThis);
                        OnCandFocusNext(pThis);
                    }
                    else
                    {
                        OnEditNext(pThis);
                    }
                }
            }
            break;

        case IIME_VK_UP:
            if ( IS_FLAG_SET(pThis->options, OPTION_CASE_SENSITIVE_LANGUAGE) &&
                 (pThis->ecnt > 0) && IS_FLAG_SET(pThis->flags, FLAG_SMART_CASE) )
            {
                is_handled = 1;
                OnEditSwitchCase(pThis);
            }
            else if (pThis->ecnt > 0)
            {
                is_handled = 1;
                if (pThis->ccnt > 0)
                    SwitchCandState(pThis);
            }
            break;

        case IIME_VK_DOWN:
            if (pThis->ecnt > 0)
            {
                is_handled = 1;
                if (pThis->ccnt > 0)
                    SwitchCandState(pThis);
            }
            break;
    }

    return is_handled;
}
static void OnEditNew(
    THIS_DATA*              pThis)
/*
 * Insert a new item at current position.
 */
{
    EDIT*                   e;
    EDIT*                   n;

    IIME_ASSERT(pThis->ecnt > 0);
    IIME_ASSERT( (pThis->ecur >= 0) && (pThis->ecur < pThis->ecnt) );

    e = pThis->edit + pThis->ecur;

    /* Do nothing if current item is empty */

    if (e->len == 0)
        return;

    /* Check for maximum length limit */

    if (GetEditFreeSpace(pThis) == 0)
        return;

    /* Exit current item */

    CLR_FLAG(e->flags, FLAG_MATCH_KEY);

    /* Initialize an empty new item */

    memmove(e + 2, e + 1, sizeof(EDIT) * (pThis->ecnt - pThis->ecur - 1));
    n = e + 1;
    n->pos   = e->pos + e->len;
    n->len   = 0;
    n->flags = 0;

    pThis->ecnt++;
    pThis->ecur++;

    /* Mark edit content changed */

    SET_FLAG(pThis->flags, FLAG_EDIT_DIRTY);

    /* Generate new candidates */

    GenerateCandidates(pThis);
}
static void OnEditAppend(
    THIS_DATA*              pThis,
    int                     key)
/*
 * Append a new char at the end of current item.
 */
{
    EDIT*                   e;
    int                     i;
    int                     p1;
    int                     p2;

    /* Create the first edit item */

    if (pThis->ecnt == 0)
    {
        e = pThis->edit;
        e->pos = 0;
        e->len = 0;
        e->flags = 0;
        if (IS_FLAG_SET(pThis->flags, FLAG_LOWER_CASE))
        {
            SET_FLAG(e->flags, FLAG_LOWER_CASE);
        }
        else if (IS_FLAG_SET(pThis->flags, FLAG_UPPER_CASE))
        {
            SET_FLAG(e->flags, FLAG_UPPER_CASE);
        }
        else /* IS_FLAG_SET(pThis->flags, FLAG_SMART_CASE) */
        {
            if (IS_FLAG_SET(pThis->options, OPTION_CASE_SENSITIVE_LANGUAGE)
            && IS_FLAG_SET(pThis->flags, FLAG_WORD_INPUT)
            && IS_FLAG_SET(pThis->flags, FLAG_SMART_CASE)             
            && pThis->m_pfnGetContext)
            {
                const unsigned short *text = 0;
                int length = 0;
                
                SET_FLAG(e->flags, FLAG_INITIAL_CASE);
                                                
                if (pThis->m_pfnGetContext(&text, &length) != 0)
                {
                    IIME_BOOL has_space = 0;
                    while (length > 0 && text[length - 1] == CHAR_SPACE)
                    {
                        has_space = 1;
                        length--;
                    }
                    
                    if (length > 0)
                    {
                        if (text[length - 1] == '.')
                        {
                            if (!has_space)
                            {
                                CLR_FLAG(e->flags, FLAG_INITIAL_CASE);
                                SET_FLAG(e->flags, FLAG_LOWER_CASE);
                            }
                        }
                        else if (!ldbapi4_is_sou_delimiter(pThis->dbobj, text[length - 1]))
                        {
                            CLR_FLAG(e->flags, FLAG_INITIAL_CASE);
                            SET_FLAG(e->flags, FLAG_LOWER_CASE);
                        }                        
                    }
                }                                                    
            }
            else
            {
                /* Context is not available, use default logic. */
                if (IS_FLAG_SET(pThis->flags, FLAG_HINT_ICASE))
                    SET_FLAG(e->flags, FLAG_INITIAL_CASE);
                else
                    SET_FLAG(e->flags, FLAG_LOWER_CASE);
            }
        }
        pThis->ecur = 0;
        pThis->ecnt = 1;
    }
    else
    {
        e = pThis->edit + pThis->ecur;
    }

    /* Check for maximum length limit */

    if (GetEditFreeSpace(pThis) <= 0)
        return;

    IIME_ASSERT( (e->len >= 0) && (e->len <= pThis->emax) );
    if (e->len >= pThis->emax)
        return;

    IIME_ASSERT( (e->len >= 0) && (e->len <= EDIT_LENGTH_LIMIT) );
    if (e->len >= EDIT_LENGTH_LIMIT)
        return;
#ifdef KMX_THI_MT_CANDIDATE_BOX
    if((IIME_ime_is_thai_mt() || IIME_ime_is_en_mt())&&e->len >= 1)
        return ;
#endif
    /* Append the key at the end of item */

    if (pThis->ecur + 1 < pThis->ecnt)
    {
        p1 = e->pos + e->len;
        p2 = pThis->edit[pThis->ecnt-1].pos + pThis->edit[pThis->ecnt-1].len;
        IIME_ASSERT(p1 <= p2);
        for (i = p2; i >= p1; i--)
        {
            pThis->edit_str[i+1] = pThis->edit_str[i];
            pThis->edit_key[i+1] = pThis->edit_key[i];
        }
        for (i = pThis->ecur + 1; i < pThis->ecnt; i++)
            pThis->edit[i].pos++;
    }

    i = e->pos + e->len;
    if (key == IIME_VK_STAR && IS_FLAG_SET(pThis->options, OPTION_ENABLE_KEY_STAR_CHAR_MAP))
    {
        pThis->edit_str[i] = (IIME_WCHAR)(LDB_KEY_STAR);
        pThis->edit_key[i] = (IIME_WCHAR)(LDB_KEY_STAR);    
    }
    else if (key == IIME_VK_POUND && IS_FLAG_SET(pThis->options, OPTION_ENABLE_KEY_POUND_CHAR_MAP))
    {
        pThis->edit_str[i] = (IIME_WCHAR)(LDB_KEY_POUND);
        pThis->edit_key[i] = (IIME_WCHAR)(LDB_KEY_POUND);    
    }    
    else
    {
        pThis->edit_str[i] = (IIME_WCHAR)(LDB_KEY_0 + (key - IIME_VK_0));
        pThis->edit_key[i] = (IIME_WCHAR)(LDB_KEY_0 + (key - IIME_VK_0));
    }
    e->len++;
    SET_FLAG(e->flags, FLAG_MATCH_KEY);

    /* Mark edit content changed */

    SET_FLAG(pThis->flags, FLAG_EDIT_DIRTY);

    /* Generate new candidates */

    GenerateCandidates(pThis);
}

static void OnEditDelete(
    THIS_DATA*              pThis,
    int                     all)
/*
 * Delete last char of current item or all content.
 */
{
    EDIT*                   e;
    int                     i;
    int                     p1;
    int                     p2; 

    IIME_ASSERT(pThis->ecnt > 0);
    IIME_ASSERT( (pThis->ecur >= 0) && (pThis->ecur < pThis->ecnt) );

    if (all)
    {
        pThis->ecnt = 0;
        pThis->ecur = 0;
        pThis->edit[0].pos = 0;
        pThis->edit[0].len = 0;
    }
    else
    {
        e = pThis->edit + pThis->ecur;

        /* Delete last char of current item */

        if (e->len > 0)
        {
            e->len--;
            SET_FLAG(e->flags, FLAG_MATCH_KEY);
            if (pThis->ecur + 1 < pThis->ecnt)
            {
                p1 = e->pos + e->len + 1;
                p2 = pThis->edit[pThis->ecnt-1].pos + pThis->edit[pThis->ecnt-1].len;
                IIME_ASSERT(p1 <= p2);
                for (i = p1; i <= p2; i++)
                {
                    pThis->edit_str[i-1] = pThis->edit_str[i];
                    pThis->edit_key[i-1] = pThis->edit_key[i];
                }
                for (i = pThis->ecur + 1; i < pThis->ecnt; i++)
                    pThis->edit[i].pos--;
            }
        }

        /* Delete the item if it is empty */

        if (e->len == 0)
        {
            if (pThis->ecur == 0)
            {
                if (pThis->ecnt == 1)
                    pThis->ecnt = 0;
            }
            else
            {
                memmove(
                    pThis->edit + pThis->ecur,
                    pThis->edit + pThis->ecur + 1,
                    sizeof(EDIT) * (pThis->ecnt - pThis->ecur - 1));
                pThis->ecnt--;
                pThis->ecur--;      /* Advanced to previous item */
            }
        }
    }

    /* Mark edit content changed */

    SET_FLAG(pThis->flags, FLAG_EDIT_DIRTY);

    /* Generate new candidates */

    GenerateCandidates(pThis);
}

static void OnEditDeleteItem(
    THIS_DATA*              pThis)
/*
 * Delete last item.
 */
{
    EDIT*                   e;
    int                     len;
    int                     i;
    int                     p1;
    int                     p2;

    if (pThis->ecnt > 0)
    {
        IIME_ASSERT( (pThis->ecur >= 0) && (pThis->ecur < pThis->ecnt) );

        e = pThis->edit + pThis->ecur;
        len = e->len;

        if ( (len > 0) && (pThis->ecur + 1 < pThis->ecnt) )
        {
            p1 = e->pos + e->len;
            p2 = pThis->edit[pThis->ecnt-1].pos + pThis->edit[pThis->ecnt-1].len;
            IIME_ASSERT(p1 <= p2);
            for (i = p1; i <= p2; i++)
            {
                pThis->edit_str[i-len] = pThis->edit_str[i];
                pThis->edit_key[i-len] = pThis->edit_key[i];
            }
            for (i = pThis->ecur + 1; i < pThis->ecnt; i++)
                pThis->edit[i].pos -= len;
        }

        if (pThis->ecur + 1 < pThis->ecnt)
        {
            memmove(
                pThis->edit + pThis->ecur,
                pThis->edit + pThis->ecur + 1,
                sizeof(EDIT) * (pThis->ecnt - pThis->ecur - 1));
        }

		pThis->ecnt--;

        /* Set focus to previous item */
        if (pThis->ecur > 0)
            pThis->ecur--;

        /* Mark edit content changed */
        SET_FLAG(pThis->flags, FLAG_EDIT_DIRTY);
    }
}

static void OnEditReplace(
    THIS_DATA*              pThis,
    const IIME_WCHAR*         buffer,
    int                     length)
/*
 * Replace content of current item with specified string.
 */
{
    EDIT*                   e;
    int                     i;
    int                     d;
    int                     p1;
    int                     p2;

    e = pThis->edit + pThis->ecur;

    d = length - e->len;
    if ( (d != 0) && (pThis->ecur + 1 < pThis->ecnt) )
    {
        p1 = e->pos + e->len;
        p2 = pThis->edit[pThis->ecnt-1].pos + pThis->edit[pThis->ecnt-1].len;
        IIME_ASSERT(p1 <= p2);
        memmove(
            pThis->edit_str + p1 + d,
            pThis->edit_str + p1,
            STR_BYTES(p2 - p1));
        memmove(
            pThis->edit_key + p1 + d,
            pThis->edit_key + p1,
            STR_BYTES(p2 - p1));
        for (i = pThis->ecur + 1; i < pThis->ecnt; i++)
            pThis->edit[i].pos += d;
    }

    memcpy(pThis->edit_str + e->pos, buffer, STR_BYTES(length));
    for (i = 0; i < length; i++)
        pThis->edit_key[e->pos+i] = ldbapi4_char_to_key(pThis->dbobj, buffer[i]);
    e->len = length;

    SET_FLAG(pThis->flags, FLAG_EDIT_DIRTY);
}

static void OnEditSwitchCase(
    THIS_DATA*              pThis)
{
    EDIT*                   e;
    int                     i;
    int                     skip_initial_case;

    IIME_ASSERT(IS_FLAG_SET(pThis->flags, FLAG_SMART_CASE));
    IIME_ASSERT(pThis->ecnt > 0);
    IIME_ASSERT( (pThis->ecur >= 0) && (pThis->ecur < pThis->ecnt) );

    e = pThis->edit + pThis->ecur;

    /* Warp scroll to next case */

    skip_initial_case = 1;
    if (pThis->is_sure_type)
    {
        if ( (e->len > 1) && (pThis->edit_key[e->pos] >= LDB_KEY_1) && (pThis->edit_key[e->pos] <= LDB_KEY_14) )
        {
            for (i = 1; i < e->len; i++)
            {
                if ( (pThis->edit_key[e->pos+i] >= LDB_KEY_1) && (pThis->edit_key[e->pos+i] <= LDB_KEY_14) )
                {
                    skip_initial_case = 0;
                    break;
                }
            }
        }
    }
    else
    {
        if ( (e->len > 1) && (pThis->edit_key[e->pos] >= LDB_KEY_2) && (pThis->edit_key[e->pos] <= LDB_KEY_9) )
        {
            for (i = 1; i < e->len; i++)
            {
                if ( (pThis->edit_key[e->pos+i] >= LDB_KEY_2) && (pThis->edit_key[e->pos+i] <= LDB_KEY_9) )
                {
                    skip_initial_case = 0;
                    break;
                }
            }
        }
    }

    if (IS_FLAG_SET(e->flags, FLAG_INITIAL_CASE))
    {
        CLR_FLAG(e->flags, FLAG_INITIAL_CASE);
        if (skip_initial_case)
            SET_FLAG(e->flags, FLAG_LOWER_CASE);
        else
            SET_FLAG(e->flags, FLAG_UPPER_CASE);
    }
    else if (IS_FLAG_SET(e->flags, FLAG_UPPER_CASE))
    {
        CLR_FLAG(e->flags, FLAG_UPPER_CASE);
        SET_FLAG(e->flags, FLAG_LOWER_CASE);
    }
    else /* IS_FLAG_SET(e->flags, FLAG_LOWER_CASE) */
    {
        CLR_FLAG(e->flags, FLAG_LOWER_CASE);
        if (skip_initial_case)
            SET_FLAG(e->flags, FLAG_UPPER_CASE);
        else
            SET_FLAG(e->flags, FLAG_INITIAL_CASE);
    }

    /* Mark edit content changed */

    SET_FLAG(pThis->flags, FLAG_EDIT_DIRTY);

    /* Generate new candidates */

    GenerateCandidates(pThis);
}

static void OnEditPrev(
    THIS_DATA*              pThis)
/*
 * Select previous item in list as current.
 * Update candiates as current item changed.
 */
{
    EDIT*                   e;

    IIME_ASSERT(pThis->ecnt > 0);

    /* Back to previous item, empty item is removed */

    e = pThis->edit + pThis->ecur;

    if (pThis->ecur == 0)
    {
        if (e->len == 0)
        {
            /* Do nothing */
            return;
        }
        else
        {
            /* We should create an empty item at start, and move to it */
            if (GetEditFreeSpace(pThis) > 0)
            {
                CLR_FLAG(e->flags, FLAG_MATCH_KEY);
                memmove(e + 1, e, sizeof(EDIT) * pThis->ecnt);
                e->pos = 0;
                e->len = 0;
                e->flags = 0;
                pThis->ecnt++;
            }
            else
            {
                /* Do nothing */
                return;
            }
        }
    }
    else
    {
        CLR_FLAG(e->flags, FLAG_MATCH_KEY);
        if (e->len == 0)
        {
            memmove(e, e + 1, sizeof(EDIT) * (pThis->ecnt - pThis->ecur - 1));
            pThis->ecnt--;
        }
        pThis->ecur--;
    }

    /* Mark edit content changed */

    SET_FLAG(pThis->flags, FLAG_EDIT_DIRTY);

    /* Generate new candidates */

    GenerateCandidates(pThis);
}

static void OnEditNext(
    THIS_DATA*              pThis)
/*
 * Select next item in list as current.
 * Update candiates as current item changed.
 */
{
    EDIT*                   e;

    IIME_ASSERT(pThis->ecnt > 0);
    
    if (pThis->ecur + 1 < pThis->ecnt)
    {
        /* Advance to next item, empty item is removed */

        e = pThis->edit + pThis->ecur;

        CLR_FLAG(e->flags, FLAG_MATCH_KEY);
        if (e->len == 0)
        {
            memmove(e, e + 1, sizeof(EDIT) * (pThis->ecnt - pThis->ecur - 1));
            pThis->ecnt--;
        }
        else
        {
            pThis->ecur++;
        }

        /* Mark edit content changed */

        SET_FLAG(pThis->flags, FLAG_EDIT_DIRTY);

        /* Generate new candidates */

        GenerateCandidates(pThis);
    }
    else
    {
        /* Do nothing if current item is the last*/
    }
}

static void OnEditCommit(
    THIS_DATA*              pThis)
/*
 * Commit current content and reset to ready.
 */
{
    IIME_WCHAR*               p;
    EDIT*                   e;
    int                     c;
    int                     i;

    /* Generate commit content from each edit item */

    c = 0;
    e = pThis->edit;
    p = pThis->commit;
    for (i = 0; i < pThis->ecnt; i++, e++)
    {
        memcpy(p, pThis->edit_str + e->pos, STR_BYTES(e->len));
        c += pThis->edit[i].len;
        p += pThis->edit[i].len;
        if (i != pThis->ecnt - 1)
        {
            *p = CHAR_SPACE;
            p++;
            c++;
        }
    }
    pThis->commit_len = c;

    /* Mark commit content is available */

    SET_FLAG(pThis->flags, FLAG_COMMIT);

    /* Update database engine cache */

    ldbapi4_update_user_data(pThis->dbobj, pThis->commit, pThis->commit_len, 0);

    /* Reset to idle state */

    pThis->ecnt = 0;
    pThis->ecur = 0;

    pThis->ccnt = 0;
    pThis->cpfi = 0;
    pThis->cpct = 0;
    pThis->cpcu = 0;

    /* Detect best case for next phase */

    i = pThis->commit_len - 1;
    while ( (i >= 0) && (pThis->commit[i] == CHAR_SPACE) )
        i--;    /* Skip trailing space */

    if (i >= 0)
    {
        if (ldbapi4_is_sou_delimiter(pThis->dbobj, pThis->commit[i]))
            SET_FLAG(pThis->flags, FLAG_HINT_ICASE);
        else
            CLR_FLAG(pThis->flags, FLAG_HINT_ICASE);
    }
}

/****************************************************************************
 *
 * Implementation of CANDIDATE functions.
 *
 */

static void SwitchCandState(
    THIS_DATA*              pThis)
/*
 * Switch to CANDIDATE state.
 */
{
    IIME_ASSERT(pThis->ccnt > 0);     /* Candidate must be available */

    pThis->state = STATE_CAND;
    SET_FLAG(pThis->flags, FLAG_CAND_DIRTY);
    CLR_FLAG(pThis->flags, FLAG_MULTITAP);
}

#if 0
static int OnCandKeyQuery(
    THIS_DATA*              pThis,
    int                     key)
/*
 * Query if given key is acceptable.
 *
 * Return non-zero if the key is acceptable.
 * Return zero otherwise.
 */
{
    int                     is_acceptable;

    is_acceptable = 0;

    switch (key)
    {
        case IIME_VK_0:
        case IIME_VK_1:
        case IIME_VK_2:
        case IIME_VK_3:
        case IIME_VK_4:
        case IIME_VK_5:
        case IIME_VK_6:
        case IIME_VK_7:
        case IIME_VK_8:
        case IIME_VK_9:
        case IIME_VK_10:
        case IIME_VK_11:
        case IIME_VK_12:
        case IIME_VK_13:
        case IIME_VK_14:
        case IIME_VK_15:
        case IIME_VK_BACKSPACE:
        case IIME_VK_LONGBACKSPACE:
        case IIME_VK_SELECT:
        case IIME_VK_SPACE:
        case IIME_VK_AUX:
        case IIME_VK_STAR:
        case IIME_VK_POUND:
        case IIME_VK_LEFT:
        case IIME_VK_RIGHT:
        case IIME_VK_UP:
        case IIME_VK_DOWN:
            is_acceptable = 1;
            break;
    }

    return is_acceptable;
}
#endif

#if 1//IEKIE_CAND_LINK
extern int iekie_cand_link_index;

//在此处打开网页
void iekie_candidate_link(void)
{
  
}
#endif//IEKIE_CAND_LINK

static int OnCandKeyEvent(
    THIS_DATA*              pThis,
    int                     key)
/*
 * Process given key event.
 *
 * Return non-zero if the event is handled.
 * Return zero otherwise.
 */
{
    int                     is_handled;
    int                     i;

    is_handled = 0;
    #ifdef KMX_THI_MT_CANDIDATE_BOX
    if(IIME_ime_is_thai_mt()||IIME_ime_is_en_mt())
    {
        if(key >= IIME_VK_0 && key<=IIME_VK_9)
        {
            if(preKey != key)
            {
                preKey = key;
            }
            else
            {
                if(pThis->ecnt > 0)
                {
                    key = IIME_VK_RIGHT;
                }
            }
        }
    }
    #endif
    switch (key)
    {
        case IIME_VK_0:
            #ifdef KMX_THI_MT_CANDIDATE_BOX
            if(IIME_ime_is_thai_mt())
            {
                OnEditKeyEvent(pThis, key);
            }
            #endif
            is_handled = 1;
            /* Do nothing by design */
            break;

        case IIME_VK_1:
        case IIME_VK_2:
        case IIME_VK_3:
        case IIME_VK_4:
        case IIME_VK_5:
        case IIME_VK_6:
        case IIME_VK_7:
        case IIME_VK_8:
        case IIME_VK_9:
        case IIME_VK_10:
        case IIME_VK_11:
        case IIME_VK_12:
        case IIME_VK_13:
        case IIME_VK_14:
        case IIME_VK_15:
            is_handled = 1;
            i = key - IIME_VK_1;
            #ifdef KMX_THI_MT_CANDIDATE_BOX
            if(IIME_ime_is_thai_mt() ||IIME_ime_is_en_mt())
            {
                OnEditKeyEvent(pThis, key);
            }
            else
            #endif
            if (i < pThis->cpct)
            {
                i += pThis->cpfi;
                if (IsEudwCandidate(pThis, i))
                {
                    OnCandLocateFocus(pThis, 0);
                    OnCandSync(pThis);
                    SwitchEudwState(pThis);
                }
                else
                {
                    OnCandCommit(pThis, i);
                }
            }
            
            break;

        case IIME_VK_BACKSPACE:
        case IIME_VK_LONGBACKSPACE:
            is_handled = 1;
            OnCandLocateFocus(pThis, 0);
            OnCandSync(pThis);
            SwitchEditState(pThis);
            #ifdef KMX_THI_MT_CANDIDATE_BOX
            if(IIME_ime_is_thai_mt()||IIME_ime_is_en_mt())
            {
                OnEditDelete(pThis, 0);
                preKey = -1;
            }
            #endif
            break;

        case IIME_VK_SELECT:
            is_handled = 1;
            i = pThis->cpfi + pThis->cpcu;
            if (IsEudwCandidate(pThis, i))
            {
                SwitchEudwState(pThis);
            }
            else
            {
              #if 1//IEKIE_CAND_LINK
              if (i == iekie_cand_link_index)
              {
              	iekie_candidate_link();//可以直接打开网址!
              }
			  else
			  #endif
			  {
                OnCandCommit(pThis, i);
			  }
            }
            break;

        case IIME_VK_SPACE:
            is_handled = 1;
            i = pThis->cpfi + pThis->cpcu;
            if (IsEudwCandidate(pThis, i))
            {
                /* Do nothing by design */
            }
            else if (IS_FLAG_SET(pThis->options, OPTION_SINGLE_WORD_EDIT))
            {
                OnCandCommit(pThis, i);
                pThis->commit[pThis->commit_len] = CHAR_SPACE;
                pThis->commit_len++;
            }
            else if ( (pThis->ecnt == 1) && (pThis->edit[0].len == 1) && (pThis->edit_key[0] == LDB_KEY_1) )
            {
                OnCandCommit(pThis, i);
                pThis->commit[pThis->commit_len] = CHAR_SPACE;
                pThis->commit_len++;
            }
            else
            {
                OnCandCommit(pThis, i);
                OnEditNew(pThis);
            }
            break;

        case IIME_VK_AUX:
        case IIME_VK_STAR:
        case IIME_VK_POUND:
            is_handled = 1;
            /* Do nothing by design */
            break;

        case IIME_VK_LEFT:
            if (IS_FLAG_SET(pThis->options, OPTION_RIGHT_TO_LEFT_LANGUAGE))
            {
                /*
                 * Right-To-Left language:
                 *      Left key moves to next item.
                 */
                is_handled = 1;
                OnCandFocusNext(pThis);
            }
            else
            {
                /*
                 * Left-To-Right language:
                 *      Left key moves to previous item.
                 */
                is_handled = 1;
                OnCandFocusPrev(pThis);
            }
            break;

        case IIME_VK_RIGHT:
            if (IS_FLAG_SET(pThis->options, OPTION_RIGHT_TO_LEFT_LANGUAGE))
            {
                /*
                 * Right-To-Left language:
                 *      Right key moves to previous item.
                 */
                is_handled = 1;
                OnCandFocusPrev(pThis);
            }
            else
            {
                /*
                 * Left-To-Right language:
                 *      Right key moves to next item.
                 */
                is_handled = 1;
                OnCandFocusNext(pThis);
            }
            break;

        case IIME_VK_UP:
            is_handled = 1;
            OnCandFocusPrevPage(pThis);
            break;

        case IIME_VK_DOWN:
            is_handled = 1;
            OnCandFocusNextPage(pThis);
            break;
    }

    return is_handled;
}

static void OnCandCommit(
    THIS_DATA*              pThis,
    int                     index)
{
    EDIT*                   e;

    if (index != pThis->cpfi + pThis->cpcu)
    {
        OnCandLocateFocus(pThis, index);
        OnCandSync(pThis);
    }

    e = pThis->edit + pThis->ecur;
    CLR_FLAG(e->flags, FLAG_MATCH_KEY);

    if (IS_FLAG_SET(pThis->options, OPTION_SINGLE_WORD_EDIT))
    {
        SwitchEditState(pThis);
        OnEditCommit(pThis);
    }
    else if ( (pThis->ecnt == 1) && (pThis->edit[0].len == 1) && (pThis->edit_key[0] == LDB_KEY_1) )
    {
        SwitchEditState(pThis);
        OnEditCommit(pThis);
    }
    else
    {
        SwitchEditState(pThis);
        GenerateCandidates(pThis);
    }
}

static void OnCandLocateFocus(
    THIS_DATA*              pThis,
    int                     index)
/*
 * Locate page contains the specified candidate, set it as focused.
 *
 * Rules for locating candidate in page:
 *
 *   1) AT LEAST 1
 *      Each page must hold at least one candidate
 *
 *   2) AT MOST 9
 *      Each page can hold at most 9 candidates
 *
 *   3) LIMITED TOTAL WIDTH
 *      Total display width of candidates plus label in each page
 *      can not exceed window width limit
 *
 *   4) LIMITED TOTAL LENGTH
 *      Total string length plus inter-concat null char in each page
 *      can not exceed content buffer limit
 */
{
    int                     width;
    int                     length;

    int                     start;
    int                     count;

    int                     i;

    SET_FLAG(pThis->flags, FLAG_CAND_DIRTY);

    if (pThis->ccnt == 0)
    {
        pThis->cpfi = 0;
        pThis->cpct = 0;
        pThis->cpcu = 0;
        return;
    }

    IIME_ASSERT( (index >= 0) && (index < pThis->ccnt) );

    start = 0;
    count = pThis->ccnt;

    i = 0;
    while (i < pThis->ccnt)
    {
        width = pThis->cand[i].width;
        length = pThis->cand[i].len + 1;

        start = i;
        count = 1;           /* Rule 1: AT LEAST 1 */
        i++;

        while ( (i < pThis->ccnt) &&
                (width < pThis->total_window_width) &&
                (length < CNT_MAX_CAND) )
        {
            width += pThis->cand[i].width;
            length += pThis->cand[i].len + 1;

            if ( (width > pThis->total_window_width) || (length > CNT_MAX_CAND) )
                break;       /* Rule 3,4: LIMITED TOTAL WIDTH/LENGTH */

            count++;
            if (count >= 9)  /* Rule 2: AT MOST 9 */
                break;

            i++;
        }

        if ( (index >= start) && (index < start + count) )
            break;

        i = start + count;
    }

    pThis->cpfi = start;
    pThis->cpct = count;
    pThis->cpcu = index - start;
}

static void OnCandSync(
    THIS_DATA*              pThis)
/*
 * Update current edit item by focused candidate.
 */
{
    int                     i;

    i = pThis->cpfi + pThis->cpcu;
    if (IsEudwCandidate(pThis, i))
        i = 0;

    OnEditReplace(pThis, pThis->cand[i].str, pThis->cand[i].len);
}

static void OnCandFocusPrev(
    THIS_DATA*              pThis)
/*
 * Set focus to previous adjacent candidate.
 * Switch to previous page if needed.
 */
{
    int                     i;

    i = pThis->cpfi + pThis->cpcu;

    if (i > 0)
    {
        OnCandLocateFocus(pThis, i - 1);
        OnCandSync(pThis);
    }
}

static void OnCandFocusNext(
    THIS_DATA*              pThis)
/*
 * Set focus to next adjacent candidate.
 * Switch to previous page if needed.
 */
{
    int                     i;

    i = pThis->cpfi + pThis->cpcu;

    if (i + 1 < pThis->ccnt)
    {
        OnCandLocateFocus(pThis, i + 1);
        OnCandSync(pThis);
    }
#ifdef KMX_THI_MT_CANDIDATE_BOX
    else if(IIME_ime_is_thai_mt() ||IIME_ime_is_en_mt())
    {
        OnCandLocateFocus(pThis, 0);
        OnCandSync(pThis);
    }
#endif
}

static void OnCandFocusPrevPage(
    THIS_DATA*              pThis)
/*
 * Set focus to candidate in previous page.
 */
{
    if (pThis->cpfi > 0)
    {
        int focus = pThis->cpcu;
        OnCandLocateFocus(pThis, pThis->cpfi - 1);
        if (focus < pThis->cpct)
            pThis->cpcu = focus;
        else
            pThis->cpcu = pThis->cpct - 1;
        OnCandSync(pThis);
    }
}

static void OnCandFocusNextPage(
    THIS_DATA*              pThis)
/*
 * Set focus to candidate in next page.
 */
{
    if (pThis->cpfi + pThis->cpct < pThis->ccnt)
    {
        int focus = pThis->cpcu;
        OnCandLocateFocus(pThis, pThis->cpfi + pThis->cpct);
        if (focus < pThis->cpct)
            pThis->cpcu = focus;
        else
            pThis->cpcu = pThis->cpct - 1;
        OnCandSync(pThis);
    }
}

/****************************************************************************
 *
 * Implementation of EUDW functions.
 *
 */

static void SwitchEudwState(
    THIS_DATA*              pThis)
/*
 * Switch to EUDW state.
 */
{
    EDIT*                   e;

    pThis->state = STATE_EUDW;
    SET_FLAG(pThis->flags, FLAG_EUDW_DIRTY);

    /* Multitap is not actived initialy */

    CLR_FLAG(pThis->flags, FLAG_MULTITAP);

    /* Set initial content by current edit item */

    e = pThis->edit + pThis->ecur;
    pThis->ulen = e->len;
    memcpy(pThis->ustr, pThis->edit_str + e->pos, STR_BYTES(e->len));

    /* set product ID info */

    if (match_pid_key(pThis->edit_key + e->pos, e->len))
        setup_pid_string(pThis->ustr);

    /* Place caret at the end */

    pThis->upos = pThis->ulen;

    /* Set maximum length limit */

    IIME_ASSERT(GetEditFreeSpace(pThis) >= 0);
    IIME_ASSERT(e->len <= pThis->emax);
    
    pThis->umax = e->len + GetEditFreeSpace(pThis);
    if (pThis->umax > pThis->emax)
        pThis->umax = pThis->emax;
}

#if 0
static int OnEudwKeyQuery(
    THIS_DATA*              pThis,
    int                     key)
/*
 * Query if given key is acceptable.
 *
 * Return non-zero if the key is acceptable.
 * Return zero otherwise.
 */
{
    int                     is_acceptable;

    is_acceptable = 0;

    switch (key)
    {
        case IIME_VK_0:
        case IIME_VK_1:
        case IIME_VK_2:
        case IIME_VK_3:
        case IIME_VK_4:
        case IIME_VK_5:
        case IIME_VK_6:
        case IIME_VK_7:
        case IIME_VK_8:
        case IIME_VK_9:
        case IIME_VK_10:
        case IIME_VK_11:
        case IIME_VK_12:
        case IIME_VK_13:
        case IIME_VK_14:
        case IIME_VK_15:
        case IIME_VK_BACKSPACE:
        case IIME_VK_LONGBACKSPACE:
        case IIME_VK_SELECT:
        case IIME_VK_SPACE:
        case IIME_VK_AUX:
        case IIME_VK_STAR:
        case IIME_VK_POUND:
        case IIME_VK_LEFT:
        case IIME_VK_RIGHT:
        case IIME_VK_UP:
        case IIME_VK_DOWN:
            is_acceptable = 1;
            break;
    }

    return is_acceptable;
}
#endif

static int OnEudwKeyEvent(
    THIS_DATA*              pThis,
    int                     key)
/*
 * Process given key event.
 *
 * Return non-zero if the event is handled.
 * Return zero otherwise.
 */
{
    int                     is_handled;

    is_handled = 0;

    switch (key)
    {
        case IIME_VK_0:
        case IIME_VK_1:
        case IIME_VK_2:
        case IIME_VK_3:
        case IIME_VK_4:
        case IIME_VK_5:
        case IIME_VK_6:
        case IIME_VK_7:
        case IIME_VK_8:
        case IIME_VK_9:
        case IIME_VK_10:
        case IIME_VK_11:
        case IIME_VK_12:
        case IIME_VK_13:
        case IIME_VK_14:
        case IIME_VK_15:
            is_handled = 1;
            OnEudwMultitapKey(pThis, key);
            break;

        case IIME_VK_BACKSPACE:
            is_handled = 1;
            OnEudwMultitapStop(pThis);
            if (pThis->ulen > 0)
                OnEudwDelete(pThis, 0);
            else
                OnEudwCancel(pThis);
            break;

        case IIME_VK_LONGBACKSPACE:
            is_handled = 1;
            OnEudwMultitapStop(pThis);
            if (pThis->ulen > 0)
                OnEudwDelete(pThis, 1);
            else
                OnEudwCancel(pThis);
            break;

        case IIME_VK_SELECT:
            is_handled = 1;
            OnEudwMultitapStop(pThis);
            OnEudwCommit(pThis);
            break;

        case IIME_VK_AUX:
            is_handled = 1;
            OnEudwAuxKey(pThis);
            break;

        case IIME_VK_SPACE:
            is_handled = 1;
            if (IS_FLAG_SET(pThis->options, OPTION_ENABLE_KEY0_CHAR_MAP))
                OnEudwMultitapKey(pThis, IIME_VK_0);
            break;

        case IIME_VK_STAR:
            if (IS_FLAG_SET(pThis->options, OPTION_ENABLE_KEY_STAR_CHAR_MAP))
            {
                is_handled = 1;
                OnEudwMultitapKey(pThis, IIME_VK_STAR);
            }
            else if (pThis->ecnt > 0)
                is_handled = 1;
            break;            
        case IIME_VK_POUND:
            if (IS_FLAG_SET(pThis->options, OPTION_ENABLE_KEY_POUND_CHAR_MAP))
            {
                is_handled = 1;
                OnEudwMultitapKey(pThis, IIME_VK_POUND);
            }
            else            
                is_handled = 1;
            /* Do nothing by design */
            break;

        case IIME_VK_LEFT:
            if (IS_FLAG_SET(pThis->options, OPTION_COMPLEX_RENDERING_LANGUAGE))
            {
                is_handled = 1;
                /*
                 * Do nothing by design.
                 *
                 * Disable caret moving for complex rendering language.
                 */
            }
            else
            {
                if (IS_FLAG_SET(pThis->options, OPTION_RIGHT_TO_LEFT_LANGUAGE))
                {
                    /*
                     * Right-To-Left language:
                     *      Left key moves caret to next position.
                     */
                    is_handled = 1;
                    OnEudwMultitapStop(pThis);
                    OnEudwCaretForward(pThis);
                }
                else
                {
                    /*
                     * Left-To-Right language:
                     *      Left key moves caret to previous position.
                     */
                    is_handled = 1;
                    OnEudwMultitapStop(pThis);
                    OnEudwCaretBackward(pThis);
                }
            }
            break;

        case IIME_VK_RIGHT:
            if (IS_FLAG_SET(pThis->options, OPTION_COMPLEX_RENDERING_LANGUAGE))
            {
                is_handled = 1;
                /*
                 * Do nothing by design.
                 *
                 * Disable caret moving for complex rendering language.
                 */
            }
            else
            {
                if (IS_FLAG_SET(pThis->options, OPTION_RIGHT_TO_LEFT_LANGUAGE))
                {
                    /*
                     * Right-To-Left language:
                     *      Right key moves caret to previous position.
                     */
                    is_handled = 1;
                    OnEudwMultitapStop(pThis);
                    OnEudwCaretBackward(pThis);
                }
                else
                {
                    /*
                     * Left-To-Right language:
                     *      Right key moves caret to next position.
                     */
                    is_handled = 1;
                    OnEudwMultitapStop(pThis);
                    OnEudwCaretForward(pThis);
                }
            }
            break;

        case IIME_VK_UP:
        case IIME_VK_DOWN:
            is_handled = 1;
            /* Do nothing by design */
            break;
    }

    return is_handled;
}

static void OnEudwTimerEvent(
    THIS_DATA*              pThis)
/*
 * Process timer event.
 */
{
    OnEudwMultitapStop(pThis);
}

static void OnEudwMultitapKey(
    THIS_DATA*              pThis,
    int                     key)
/*
 * Process a multitap key input.
 */
{
    if (IS_FLAG_SET(pThis->flags, FLAG_MULTITAP))
    {
        IIME_ASSERT(pThis->ulen > 0);
        /* When multitap is already actived, check the key against
           privious key. If keys match, continue scroll multitap,
           otherwise start a new multitap loop. */
        if (key == pThis->umtk)
        {
            OnEudwMultitapScroll(pThis);
        }
        else
        {
            OnEudwMultitapStop(pThis);
            OnEudwMultitapStart(pThis, key);
        }
    }
    else
    {
        /* Start multitap loop if multitap is not actived */
        OnEudwMultitapStart(pThis, key);
    }
}

static void OnEudwMultitapStart(
    THIS_DATA*              pThis,
    int                     key)
/*
 * Start multitap for specified key.
 */
{
    int                     f;
    IIME_WCHAR                c;

    IIME_ASSERT(IS_FLAG_CLR(pThis->flags, FLAG_MULTITAP));

    /* Check maximum length limit */

    if (pThis->ulen >= pThis->umax)
        return;

    /* Get multitap info from database engine */
    
    if (key == IIME_VK_STAR && IS_FLAG_SET(pThis->options, OPTION_ENABLE_KEY_STAR_CHAR_MAP))
        c = (IIME_WCHAR) (LDB_KEY_STAR);
    else if (key == IIME_VK_POUND && IS_FLAG_SET(pThis->options, OPTION_ENABLE_KEY_POUND_CHAR_MAP))
        c = (IIME_WCHAR) (LDB_KEY_POUND);    
    else
        c = (IIME_WCHAR) (LDB_KEY_0 + (key - IIME_VK_0));

    if (IS_FLAG_SET(pThis->options, OPTION_CASE_SENSITIVE_LANGUAGE))
    {
        if (IS_FLAG_SET(pThis->options, OPTION_UPPER_CASE_ALWAYS))
            f = LDB_UPPER_CASE;
        else if (IS_FLAG_SET(pThis->options, OPTION_LOWER_CASE_ALWAYS))
            f = LDB_LOWER_CASE;
        else if (IS_FLAG_SET(pThis->flags, FLAG_LOWER_CASE))
            f = LDB_LOWER_CASE;
        else if (IS_FLAG_SET(pThis->flags, FLAG_UPPER_CASE))
            f = LDB_UPPER_CASE;
        else /* IS_FLAG_SET(pThis->flags, FLAG_SMART_CASE) */
            f = LDB_LOWER_CASE | LDB_UPPER_CASE;
    }
    else
    {
        f = 0;
    }

    if (pThis->language == IIME_LANG_THAI)
    {
        IIME_WCHAR            prev_char;
        const IIME_WCHAR*     multitap_list;
        int                 num_multitap_chars;

        if (pThis->upos > 0)
            prev_char = pThis->ustr[pThis->upos-1];
        else
            prev_char = 0;

        multitap_list = ldbapi4_get_multitype(pThis->dbobj, c, f, &num_multitap_chars);

        pThis->umtl = 0;
        if (multitap_list && num_multitap_chars > 0)
        {
            int i;

            for (i = 0; i < num_multitap_chars; i++)
            {
                if (ldbapi4_is_valid_thai_follow_char(prev_char, multitap_list[i]))
                {
                    pThis->umts[pThis->umtl] = multitap_list[i];
                    pThis->umtl++;
                    if (pThis->umtl == MAX_MULTITAP_LENGTH)
                        break;
                }
            }
        }
    }
    else if (pThis->language == IIME_LANG_HINDI)
    {
        IIME_WCHAR            prev_char;
        const IIME_WCHAR*     multitap_list;
        int                 num_multitap_chars;

        if (pThis->upos > 0)
            prev_char = pThis->ustr[pThis->upos-1];
        else
            prev_char = 0;

        multitap_list = ldbapi4_get_multitype(pThis->dbobj, c, f, &num_multitap_chars);

        pThis->umtl = 0;
        if (multitap_list && num_multitap_chars > 0)
        {
            int i;

            for (i = 0; i < num_multitap_chars; i++)
            {
                if (ldbapi4_is_valid_hindi_follow_char(prev_char, multitap_list[i]))
                {
                    pThis->umts[pThis->umtl] = multitap_list[i];
                    pThis->umtl++;
                    if (pThis->umtl == MAX_MULTITAP_LENGTH)
                        break;
                }
            }
        }
    }
	else if (pThis->language == IIME_LANG_NEPALI)
    {
        IIME_WCHAR            prev_char;
        const IIME_WCHAR*     multitap_list;
        int                 num_multitap_chars;

        if (pThis->upos > 0)
            prev_char = pThis->ustr[pThis->upos-1];
        else
            prev_char = 0;

        multitap_list = ldbapi4_get_multitype(pThis->dbobj, c, f, &num_multitap_chars);

        pThis->umtl = 0;
        if (multitap_list && num_multitap_chars > 0)
        {
            int i;

            for (i = 0; i < num_multitap_chars; i++)
            {
                if (ldbapi4_is_valid_nepali_follow_char(prev_char, multitap_list[i]))
                {
                    pThis->umts[pThis->umtl] = multitap_list[i];
                    pThis->umtl++;
                    if (pThis->umtl == MAX_MULTITAP_LENGTH)
                        break;
                }
            }
        }
    }
	else if (pThis->language == IIME_LANG_SINHALA)
    {
        IIME_WCHAR            prev_char;
        const IIME_WCHAR*     multitap_list;
        int                 num_multitap_chars;

        if (pThis->upos > 0)
            prev_char = pThis->ustr[pThis->upos-1];
        else
            prev_char = 0;

        multitap_list = ldbapi4_get_multitype(pThis->dbobj, c, f, &num_multitap_chars);

        pThis->umtl = 0;
        if (multitap_list && num_multitap_chars > 0)
        {
            int i;

            for (i = 0; i < num_multitap_chars; i++)
            {
                if (ldbapi4_is_valid_sinhala_follow_char(prev_char, multitap_list[i]))
                {
                    pThis->umts[pThis->umtl] = multitap_list[i];
                    pThis->umtl++;
                    if (pThis->umtl == MAX_MULTITAP_LENGTH)
                        break;
                }
            }
        }
    }
	//daodejing
	else if (pThis->language == IIME_LANG_BODO)
    {
        IIME_WCHAR            prev_char;
        const IIME_WCHAR*     multitap_list;
        int                 num_multitap_chars;

        if (pThis->upos > 0)
            prev_char = pThis->ustr[pThis->upos-1];
        else
            prev_char = 0;

        multitap_list = ldbapi4_get_multitype(pThis->dbobj, c, f, &num_multitap_chars);

        pThis->umtl = 0;
        if (multitap_list && num_multitap_chars > 0)
        {
            int i;

            for (i = 0; i < num_multitap_chars; i++)
            {
                if (ldbapi4_is_valid_bodo_follow_char(prev_char, multitap_list[i]))
                {
                    pThis->umts[pThis->umtl] = multitap_list[i];
                    pThis->umtl++;
                    if (pThis->umtl == MAX_MULTITAP_LENGTH)
                        break;
                }
            }
        }
    }
	else if (pThis->language == IIME_LANG_DOGRI)
    {
        IIME_WCHAR            prev_char;
        const IIME_WCHAR*     multitap_list;
        int                 num_multitap_chars;

        if (pThis->upos > 0)
            prev_char = pThis->ustr[pThis->upos-1];
        else
            prev_char = 0;

        multitap_list = ldbapi4_get_multitype(pThis->dbobj, c, f, &num_multitap_chars);

        pThis->umtl = 0;
        if (multitap_list && num_multitap_chars > 0)
        {
            int i;

            for (i = 0; i < num_multitap_chars; i++)
            {
                if (ldbapi4_is_valid_dogri_follow_char(prev_char, multitap_list[i]))
                {
                    pThis->umts[pThis->umtl] = multitap_list[i];
                    pThis->umtl++;
                    if (pThis->umtl == MAX_MULTITAP_LENGTH)
                        break;
                }
            }
        }
    }	
	else if (pThis->language == IIME_LANG_SANSKRIT)
    {
        IIME_WCHAR            prev_char;
        const IIME_WCHAR*     multitap_list;
        int                 num_multitap_chars;

        if (pThis->upos > 0)
            prev_char = pThis->ustr[pThis->upos-1];
        else
            prev_char = 0;

        multitap_list = ldbapi4_get_multitype(pThis->dbobj, c, f, &num_multitap_chars);

        pThis->umtl = 0;
        if (multitap_list && num_multitap_chars > 0)
        {
            int i;

            for (i = 0; i < num_multitap_chars; i++)
            {
                if (ldbapi4_is_valid_sanskrit_follow_char(prev_char, multitap_list[i]))
                {
                    pThis->umts[pThis->umtl] = multitap_list[i];
                    pThis->umtl++;
                    if (pThis->umtl == MAX_MULTITAP_LENGTH)
                        break;
                }
            }
        }
    }	
	else if (pThis->language == IIME_LANG_KASHMIRI)
    {
        IIME_WCHAR            prev_char;
        const IIME_WCHAR*     multitap_list;
        int                 num_multitap_chars;

        if (pThis->upos > 0)
            prev_char = pThis->ustr[pThis->upos-1];
        else
            prev_char = 0;

        multitap_list = ldbapi4_get_multitype(pThis->dbobj, c, f, &num_multitap_chars);

        pThis->umtl = 0;
        if (multitap_list && num_multitap_chars > 0)
        {
            int i;

            for (i = 0; i < num_multitap_chars; i++)
            {
                if (ldbapi4_is_valid_kashmiri_follow_char(prev_char, multitap_list[i]))
                {
                    pThis->umts[pThis->umtl] = multitap_list[i];
                    pThis->umtl++;
                    if (pThis->umtl == MAX_MULTITAP_LENGTH)
                        break;
                }
            }
        }
    }	
	else if (pThis->language == IIME_LANG_KONKANI)
    {
        IIME_WCHAR            prev_char;
        const IIME_WCHAR*     multitap_list;
        int                 num_multitap_chars;

        if (pThis->upos > 0)
            prev_char = pThis->ustr[pThis->upos-1];
        else
            prev_char = 0;

        multitap_list = ldbapi4_get_multitype(pThis->dbobj, c, f, &num_multitap_chars);

        pThis->umtl = 0;
        if (multitap_list && num_multitap_chars > 0)
        {
            int i;

            for (i = 0; i < num_multitap_chars; i++)
            {
                if (ldbapi4_is_valid_konkani_follow_char(prev_char, multitap_list[i]))
                {
                    pThis->umts[pThis->umtl] = multitap_list[i];
                    pThis->umtl++;
                    if (pThis->umtl == MAX_MULTITAP_LENGTH)
                        break;
                }
            }
        }
    }	
	else if (pThis->language == IIME_LANG_MAITHILI)
    {
        IIME_WCHAR            prev_char;
        const IIME_WCHAR*     multitap_list;
        int                 num_multitap_chars;

        if (pThis->upos > 0)
            prev_char = pThis->ustr[pThis->upos-1];
        else
            prev_char = 0;

        multitap_list = ldbapi4_get_multitype(pThis->dbobj, c, f, &num_multitap_chars);

        pThis->umtl = 0;
        if (multitap_list && num_multitap_chars > 0)
        {
            int i;

            for (i = 0; i < num_multitap_chars; i++)
            {
                if (ldbapi4_is_valid_maithili_follow_char(prev_char, multitap_list[i]))
                {
                    pThis->umts[pThis->umtl] = multitap_list[i];
                    pThis->umtl++;
                    if (pThis->umtl == MAX_MULTITAP_LENGTH)
                        break;
                }
            }
        }
    }	
	else if (pThis->language == IIME_LANG_MANIPURI)
    {
        IIME_WCHAR            prev_char;
        const IIME_WCHAR*     multitap_list;
        int                 num_multitap_chars;

        if (pThis->upos > 0)
            prev_char = pThis->ustr[pThis->upos-1];
        else
            prev_char = 0;

        multitap_list = ldbapi4_get_multitype(pThis->dbobj, c, f, &num_multitap_chars);

        pThis->umtl = 0;
        if (multitap_list && num_multitap_chars > 0)
        {
            int i;

            for (i = 0; i < num_multitap_chars; i++)
            {
                if (ldbapi4_is_valid_manipuri_follow_char(prev_char, multitap_list[i]))
                {
                    pThis->umts[pThis->umtl] = multitap_list[i];
                    pThis->umtl++;
                    if (pThis->umtl == MAX_MULTITAP_LENGTH)
                        break;
                }
            }
        }
    }	
	else if (pThis->language == IIME_LANG_SANTALI)
    {
        IIME_WCHAR            prev_char;
        const IIME_WCHAR*     multitap_list;
        int                 num_multitap_chars;

        if (pThis->upos > 0)
            prev_char = pThis->ustr[pThis->upos-1];
        else
            prev_char = 0;

        multitap_list = ldbapi4_get_multitype(pThis->dbobj, c, f, &num_multitap_chars);

        pThis->umtl = 0;
        if (multitap_list && num_multitap_chars > 0)
        {
            int i;

            for (i = 0; i < num_multitap_chars; i++)
            {
                if (ldbapi4_is_valid_santali_follow_char(prev_char, multitap_list[i]))
                {
                    pThis->umts[pThis->umtl] = multitap_list[i];
                    pThis->umtl++;
                    if (pThis->umtl == MAX_MULTITAP_LENGTH)
                        break;
                }
            }
        }
    }	
	else if (pThis->language == IIME_LANG_SOMALI)
    {
        IIME_WCHAR            prev_char;
        const IIME_WCHAR*     multitap_list;
        int                 num_multitap_chars;

        if (pThis->upos > 0)
            prev_char = pThis->ustr[pThis->upos-1];
        else
            prev_char = 0;

        multitap_list = ldbapi4_get_multitype(pThis->dbobj, c, f, &num_multitap_chars);

        pThis->umtl = 0;
        if (multitap_list && num_multitap_chars > 0)
        {
            int i;

            for (i = 0; i < num_multitap_chars; i++)
            {
                if (ldbapi4_is_valid_somali_follow_char(prev_char, multitap_list[i]))
                {
                    pThis->umts[pThis->umtl] = multitap_list[i];
                    pThis->umtl++;
                    if (pThis->umtl == MAX_MULTITAP_LENGTH)
                        break;
                }
            }
        }
    }	
	else if (pThis->language == IIME_LANG_SINDHI)
    {
        IIME_WCHAR            prev_char;
        const IIME_WCHAR*     multitap_list;
        int                 num_multitap_chars;

        if (pThis->upos > 0)
            prev_char = pThis->ustr[pThis->upos-1];
        else
            prev_char = 0;

        multitap_list = ldbapi4_get_multitype(pThis->dbobj, c, f, &num_multitap_chars);

        pThis->umtl = 0;
        if (multitap_list && num_multitap_chars > 0)
        {
            int i;

            for (i = 0; i < num_multitap_chars; i++)
            {
                if (ldbapi4_is_valid_sindhi_follow_char(prev_char, multitap_list[i]))
                {
                    pThis->umts[pThis->umtl] = multitap_list[i];
                    pThis->umtl++;
                    if (pThis->umtl == MAX_MULTITAP_LENGTH)
                        break;
                }
            }
        }
    }	
    else if (pThis->language == IIME_LANG_BENGALI)
    {
        IIME_WCHAR            prev_char;
        const IIME_WCHAR*     multitap_list;
        int                 num_multitap_chars;

        if (pThis->upos > 0)
            prev_char = pThis->ustr[pThis->upos-1];
        else
            prev_char = 0;

        multitap_list = ldbapi4_get_multitype(pThis->dbobj, c, f, &num_multitap_chars);

        pThis->umtl = 0;
        if (multitap_list && num_multitap_chars > 0)
        {
            int i;

            for (i = 0; i < num_multitap_chars; i++)
            {
                if (ldbapi4_is_valid_bengali_follow_char(prev_char, multitap_list[i]))
                {
                    pThis->umts[pThis->umtl] = multitap_list[i];
                    pThis->umtl++;
                    if (pThis->umtl == MAX_MULTITAP_LENGTH)
                        break;
                }
            }
        }
    }
    else
    {
        const IIME_WCHAR*     multitap_list;
        int                 num_multitap_chars;

        multitap_list = ldbapi4_get_multitype(pThis->dbobj, c, f, &num_multitap_chars);

        if (multitap_list && num_multitap_chars > 0)
        {
            if (num_multitap_chars > MAX_MULTITAP_LENGTH)
                num_multitap_chars = MAX_MULTITAP_LENGTH;
            memcpy(pThis->umts, multitap_list, sizeof(IIME_WCHAR) * num_multitap_chars);
            pThis->umtl = num_multitap_chars;
        }
        else
        {
            pThis->umtl = 0;
        }
    }

    if (pThis->umtl <= 0)
        return;

    /* Insert multitap char */

    memmove(
        pThis->ustr + pThis->upos + 1,
        pThis->ustr + pThis->upos,
        STR_BYTES(pThis->ulen - pThis->upos));
    pThis->ustr[pThis->upos] = pThis->umts[0];

    pThis->ulen++;
    pThis->upos++;

    /* Start multiple loop if at least two items exist */

    if (pThis->umtl >= 2)
    {
        pThis->umtk = key;
        pThis->umtp = 0;
        SET_FLAG(pThis->flags, FLAG_MULTITAP);
    }

    SET_FLAG(pThis->flags, FLAG_EUDW_DIRTY);
}

static void OnEudwMultitapScroll(
    THIS_DATA*              pThis)
/*
 * Scroll multitap to next char.
 */
{
    IIME_WCHAR*               p;

    /* Multitap char is the one before caret */

    IIME_ASSERT(pThis->upos > 0);

    p = pThis->ustr + (pThis->upos - 1);

    /* Wrap scroll multitap char */

    IIME_ASSERT( (pThis->umtp >= 0) && (pThis->umtp < pThis->umtl) );

    pThis->umtp++;
    if (pThis->umtp >= pThis->umtl)
        pThis->umtp = 0;

    *p = pThis->umts[pThis->umtp];

    SET_FLAG(pThis->flags, FLAG_EUDW_DIRTY);
}

static void OnEudwMultitapStop(
    THIS_DATA*              pThis)
/*
 * Stop multitap if it's actived.
 */
{
    if (IS_FLAG_SET(pThis->flags, FLAG_MULTITAP))
    {
        CLR_FLAG(pThis->flags, FLAG_MULTITAP);
        SET_FLAG(pThis->flags, FLAG_EUDW_DIRTY);
    }
}

static void OnEudwAuxKey(
    THIS_DATA*              pThis)
/*
 * Process star key.
 */
{
    static const struct
    {
        IIME_WCHAR chr;
        IIME_WCHAR map;
    } VIET_TONE_TAB[144] =
    {
        { 0x0041, 0x00C0 }, { 0x0045, 0x00C8 }, { 0x0049, 0x00CC }, { 0x004F, 0x00D2 },
        { 0x0055, 0x00D9 }, { 0x0059, 0x1EF2 }, { 0x0061, 0x00E0 }, { 0x0065, 0x00E8 },
        { 0x0069, 0x00EC }, { 0x006F, 0x00F2 }, { 0x0075, 0x00F9 }, { 0x0079, 0x1EF3 },
        { 0x00C0, 0x00C1 }, { 0x00C1, 0x1EA2 }, { 0x00C2, 0x1EA6 }, { 0x00C3, 0x1EA0 },
        { 0x00C8, 0x00C9 }, { 0x00C9, 0x1EBA }, { 0x00CA, 0x1EC0 }, { 0x00CC, 0x00CD },
        { 0x00CD, 0x1EC8 }, { 0x00D2, 0x00D3 }, { 0x00D3, 0x1ECE }, { 0x00D4, 0x1ED2 },
        { 0x00D5, 0x1ECC }, { 0x00D9, 0x00DA }, { 0x00DA, 0x1EE6 }, { 0x00DD, 0x1EF6 },
        { 0x00E0, 0x00E1 }, { 0x00E1, 0x1EA3 }, { 0x00E2, 0x1EA7 }, { 0x00E3, 0x1EA1 },
        { 0x00E8, 0x00E9 }, { 0x00E9, 0x1EBB }, { 0x00EA, 0x1EC1 }, { 0x00EC, 0x00ED },
        { 0x00ED, 0x1EC9 }, { 0x00F2, 0x00F3 }, { 0x00F3, 0x1ECF }, { 0x00F4, 0x1ED3 },
        { 0x00F5, 0x1ECD }, { 0x00F9, 0x00FA }, { 0x00FA, 0x1EE7 }, { 0x00FD, 0x1EF7 },
        { 0x0102, 0x1EB0 }, { 0x0103, 0x1EB1 }, { 0x0128, 0x1ECA }, { 0x0129, 0x1ECB },
        { 0x0168, 0x1EE4 }, { 0x0169, 0x1EE5 }, { 0x01A0, 0x1EDC }, { 0x01A1, 0x1EDD },
        { 0x01AF, 0x1EEA }, { 0x01B0, 0x1EEB }, { 0x1EA0, 0x0041 }, { 0x1EA1, 0x0061 },
        { 0x1EA2, 0x00C3 }, { 0x1EA3, 0x00E3 }, { 0x1EA4, 0x1EA8 }, { 0x1EA5, 0x1EA9 },
        { 0x1EA6, 0x1EA4 }, { 0x1EA7, 0x1EA5 }, { 0x1EA8, 0x1EAA }, { 0x1EA9, 0x1EAB },
        { 0x1EAA, 0x1EAC }, { 0x1EAB, 0x1EAD }, { 0x1EAC, 0x00C2 }, { 0x1EAD, 0x00E2 },
        { 0x1EAE, 0x1EB2 }, { 0x1EAF, 0x1EB3 }, { 0x1EB0, 0x1EAE }, { 0x1EB1, 0x1EAF },
        { 0x1EB2, 0x1EB4 }, { 0x1EB3, 0x1EB5 }, { 0x1EB4, 0x1EB6 }, { 0x1EB5, 0x1EB7 },
        { 0x1EB6, 0x0102 }, { 0x1EB7, 0x0103 }, { 0x1EB8, 0x0045 }, { 0x1EB9, 0x0065 },
        { 0x1EBA, 0x1EBC }, { 0x1EBB, 0x1EBD }, { 0x1EBC, 0x1EB8 }, { 0x1EBD, 0x1EB9 },
        { 0x1EBE, 0x1EC2 }, { 0x1EBF, 0x1EC3 }, { 0x1EC0, 0x1EBE }, { 0x1EC1, 0x1EBF },
        { 0x1EC2, 0x1EC4 }, { 0x1EC3, 0x1EC5 }, { 0x1EC4, 0x1EC6 }, { 0x1EC5, 0x1EC7 },
        { 0x1EC6, 0x00CA }, { 0x1EC7, 0x00EA }, { 0x1EC8, 0x0128 }, { 0x1EC9, 0x0129 },
        { 0x1ECA, 0x0049 }, { 0x1ECB, 0x0069 }, { 0x1ECC, 0x004F }, { 0x1ECD, 0x006F },
        { 0x1ECE, 0x00D5 }, { 0x1ECF, 0x00F5 }, { 0x1ED0, 0x1ED4 }, { 0x1ED1, 0x1ED5 },
        { 0x1ED2, 0x1ED0 }, { 0x1ED3, 0x1ED1 }, { 0x1ED4, 0x1ED6 }, { 0x1ED5, 0x1ED7 },
        { 0x1ED6, 0x1ED8 }, { 0x1ED7, 0x1ED9 }, { 0x1ED8, 0x00D4 }, { 0x1ED9, 0x00F4 },
        { 0x1EDA, 0x1EDE }, { 0x1EDB, 0x1EDF }, { 0x1EDC, 0x1EDA }, { 0x1EDD, 0x1EDB },
        { 0x1EDE, 0x1EE0 }, { 0x1EDF, 0x1EE1 }, { 0x1EE0, 0x1EE2 }, { 0x1EE1, 0x1EE3 },
        { 0x1EE2, 0x01A0 }, { 0x1EE3, 0x01A1 }, { 0x1EE4, 0x0055 }, { 0x1EE5, 0x0075 },
        { 0x1EE6, 0x0168 }, { 0x1EE7, 0x0169 }, { 0x1EE8, 0x1EEC }, { 0x1EE9, 0x1EED },
        { 0x1EEA, 0x1EE8 }, { 0x1EEB, 0x1EE9 }, { 0x1EEC, 0x1EEE }, { 0x1EED, 0x1EEF },
        { 0x1EEE, 0x1EF0 }, { 0x1EEF, 0x1EF1 }, { 0x1EF0, 0x01AF }, { 0x1EF1, 0x01B0 },
        { 0x1EF2, 0x00DD }, { 0x1EF3, 0x00FD }, { 0x1EF4, 0x0059 }, { 0x1EF5, 0x0079 },
        { 0x1EF6, 0x1EF8 }, { 0x1EF7, 0x1EF9 }, { 0x1EF8, 0x1EF4 }, { 0x1EF9, 0x1EF5 }
    };

    IIME_WCHAR*               p;
    int                     s;
    int                     e;
    int                     m;

    OnEudwMultitapStop(pThis);

    /* Handle Vietnamese tone scroll */

    if ( (pThis->language == IIME_LANG_VIETNAMESE) && (pThis->upos > 0) )
    {
        p = pThis->ustr + pThis->upos - 1;

        s = 0;
        e = 143;
        while (s <= e)
        {
            m = (s + e) / 2;
            if (*p < VIET_TONE_TAB[m].chr)
            {
                e = m - 1;
            }
            else if (*p > VIET_TONE_TAB[m].chr)
            {
                s = m + 1;
            }
            else /* *p == VIET_TONE_TAB[m].chr */
            {
                *p = VIET_TONE_TAB[m].map;
                SET_FLAG(pThis->flags, FLAG_EUDW_DIRTY);
                break;
            }
        }
    }
}

static void OnEudwDelete(
    THIS_DATA*              pThis,
    int                     all)
/*
 * Delete the char before caret.
 */
{
    if (all)
    {
        pThis->ulen = 0;
        pThis->upos = 0;
    }
    else if (pThis->upos > 0)
    {
        memmove(
            pThis->ustr + pThis->upos - 1,
            pThis->ustr + pThis->upos,
            STR_BYTES(pThis->ulen - pThis->upos));
        pThis->upos--;
        pThis->ulen--;
        SET_FLAG(pThis->flags, FLAG_EUDW_DIRTY);
    }
}

static void OnEudwCaretBackward(
    THIS_DATA*              pThis)
/*
 * Move caret backward one position.
 */
{
    IIME_ASSERT( (pThis->upos >= 0) && (pThis->upos <= pThis->ulen) );

    if (pThis->upos > 0)
    {
        pThis->upos--;
        SET_FLAG(pThis->flags, FLAG_EUDW_DIRTY);
    }
    else
    {
        #ifdef OPT_EUDW_WRAP
            /* Wrap to last position */
            pThis->upos = pThis->ulen;
            SET_FLAG(pThis->flags, FLAG_EUDW_DIRTY);
        #else
            /* Do nothing */
        #endif
    }
}

static void OnEudwCaretForward(
    THIS_DATA*              pThis)
/*
 * Move caret forward one position.
 */
{
    IIME_ASSERT( (pThis->upos >= 0) && (pThis->upos <= pThis->ulen) );

    if (pThis->upos < pThis->ulen)
    {
        pThis->upos++;
        SET_FLAG(pThis->flags, FLAG_EUDW_DIRTY);
    }
    else
    {
        #ifdef OPT_EUDW_WRAP
            /* Wrap to first position */
            pThis->upos = 0;
            SET_FLAG(pThis->flags, FLAG_EUDW_DIRTY);
        #else
            /* Do nothing */
        #endif
    }
}

static void OnEudwCommit(
    THIS_DATA*              pThis)
/*
 * End EUDW state. Set content back to edit item.
 */
{
    EDIT*                   e;
    int                     f;

    /* Notify eudw word to database engine */

    if (pThis->ulen > 0)
        ldbapi4_update_user_data(pThis->dbobj, pThis->ustr, pThis->ulen, LDB_EUDW);

    /* Update edit item */

    if (pThis->ulen > 0)
    {
        OnEditReplace(pThis, pThis->ustr, pThis->ulen);

        e = pThis->edit + pThis->ecur;
        CLR_FLAG(e->flags, FLAG_MATCH_KEY);

        if (IS_FLAG_SET(pThis->options, OPTION_CASE_SENSITIVE_LANGUAGE))
        {
            if (IS_FLAG_SET(pThis->flags, FLAG_SMART_CASE))
            {
                CLR_FLAG(e->flags, FLAG_INITIAL_CASE);
                CLR_FLAG(e->flags, FLAG_UPPER_CASE);
                CLR_FLAG(e->flags, FLAG_LOWER_CASE);
                f = ldbapi4_check_case(pThis->dbobj, pThis->edit_str + e->pos, e->len);
                if (f == LDB_INITIAL_CASE)
                    SET_FLAG(e->flags, FLAG_INITIAL_CASE);
                else if (f == LDB_UPPER_CASE)
                    SET_FLAG(e->flags, FLAG_UPPER_CASE);
                else
                    SET_FLAG(e->flags, FLAG_LOWER_CASE);
            }
        }
    }
    else
    {
        OnEditDeleteItem(pThis);
    }

    SwitchEditState(pThis);

    if (IS_FLAG_SET(pThis->options, OPTION_SINGLE_WORD_EDIT))
    {
        if (pThis->ecnt > 0)
            OnEditCommit(pThis);
    }

    GenerateCandidates(pThis);
}

static void OnEudwCancel(
    THIS_DATA*              pThis)
/*
 * Cancel EUDW state, back to EDIT state.
 */
{
    SwitchEditState(pThis);
    GenerateCandidates(pThis);
}

/****************************************************************************
 *
 * Implementation of MISC functions.
 *
 */

static int GetEditFreeSpace(
    THIS_DATA*              pThis)
/*
 * Return the free size of edit content at current time.
 *
 * This is the maximum number of extra characters can be inserted
 * into edit content by the upper limit.
 *
 * This value will never be negative.
 *
 * A value of zero means we have meet the limit, and no more
 * character can be add to edit content.
 */
{
    int                     c;
    int                     i;
    int                     limit;

    /* Get the lowest limit */

    limit = (COMMIT_LENGTH_LIMIT <= CNT_MAX_EDIT) ? COMMIT_LENGTH_LIMIT : CNT_MAX_EDIT;

    #ifdef OPT_EXTRA_SPACE
        limit--;
    #endif

    /* Count current edit content size */

    c = 0;
    for (i = 0; i < pThis->ecnt; i++)
    {
        c += pThis->edit[i].len + 1;
        #ifdef OPT_EXTRA_SPACE
            if ( (i + 1 < pThis->ecnt) && (pThis->edit[i+1].len == 0) )
                c++;
        #endif
    }

    IIME_ASSERT(c <= limit);

    return limit - c;
}

static int GetHistory(
    THIS_DATA*              pThis,
    IIME_WCHAR*               buffer,
    int                     max_chars)
/*
 * Return history string for current edit item.
 */
{
    EDIT*                   e;
    int                     i;
    int                     c;

    /* Include the first non-empty previous edit item in history */

    i = pThis->ecur - 1;
    while (i >= 0)
    {
        e = pThis->edit + i;
        if (e->len > 0)
            break;
        i--;
    }
    if (i < 0)
        return 0;           /* Item not exist */

    e = pThis->edit + i;
    if (e->len > max_chars)
        return 0;           /* Buffer length limit exceed */

    memcpy(buffer, pThis->edit_str + e->pos, STR_BYTES(e->len));

    c = e->len;

    /* Include next non-empty previous edit item in history */

    i--;
    while (i >= 0)
    {
        e = pThis->edit + i;
        if (e->len > 0)
            break;
        i--;
    }
    if (i < 0)
        return c;           /* Item not exist */

    e = pThis->edit + i;
    if (c + e->len + 1 > max_chars)
        return c;           /* Buffer length limit exceed */

    memmove(buffer + e->len + 1, buffer, STR_BYTES(c));
    memcpy(buffer, pThis->edit_str + e->pos, STR_BYTES(e->len));
    buffer[e->len] = CHAR_SPACE;
    c += e->len + 1;

    return c;
}
static void GenerateCandidates(
    THIS_DATA*              pThis)
/*
 * Generate candidates list by match edit item in database engine.
 */
{
    IIME_WCHAR                his[COMMIT_BUFFER_SIZE];
    int                     his_len;
    int                     len_limit;

    EDIT*                   e;
    CAND*                   d;
    int                     c;
    const IIME_WCHAR*         p;
    int                     i;
    int                     len;
    int                     cflags;     /* Case flags in matching */

    SET_FLAG(pThis->flags, FLAG_CAND_DIRTY);

    /* No candidate will be generated if current item is empty */

    if (pThis->ecnt <= 0)
    {
        pThis->ccnt = 0;
        pThis->cpfi = 0;
        pThis->cpct = 0;
        pThis->cpcu = 0;
        return;
    }

    e = pThis->edit + pThis->ecur;
    IIME_ASSERT( (e->len >= 0) && (e->len <= pThis->emax) );

    if (e->len == 0)
    {
        pThis->ccnt = 0;
        pThis->cpfi = 0;
        pThis->cpct = 0;
        pThis->cpcu = 0;
        return;
    }

    /* Generate history for matching */

    his_len = GetHistory(pThis, his, COMMIT_BUFFER_SIZE);

    /* Match current item in database engine */

    cflags = 0;
    if (IS_FLAG_SET(pThis->options, OPTION_CASE_SENSITIVE_LANGUAGE))
    {
        if (IS_FLAG_SET(pThis->options, OPTION_UPPER_CASE_ALWAYS))
            cflags = LDB_UPPER_CASE;
        else if (IS_FLAG_SET(pThis->options, OPTION_LOWER_CASE_ALWAYS))
            cflags = LDB_LOWER_CASE;
        else if (IS_FLAG_SET(pThis->options, OPTION_INITIAL_CASE_ALWAYS))
            cflags = LDB_INITIAL_CASE;
        else if (IS_FLAG_SET(e->flags, FLAG_INITIAL_CASE))
            cflags = LDB_INITIAL_CASE;
        else if (IS_FLAG_SET(e->flags, FLAG_UPPER_CASE))
            cflags = LDB_UPPER_CASE;
        else /* IS_FLAG_SET(e->flags, FLAG_LOWER_CASE) */
            cflags = LDB_LOWER_CASE;
    }

    p = IS_FLAG_SET(e->flags, FLAG_MATCH_KEY) ? (pThis->edit_key + e->pos) : (pThis->edit_str + e->pos);

#if defined(SPREADTRUM_PRODUCT)
	if (IS_FLAG_SET(pThis->options, OPTION_ARABIC_WITHOUT_SORT) && pThis->ecnt == 1 && e->len == 1 && *p >= LDB_KEY_1 && *p <= LDB_KEY_9)
	{
		static const LDB_CHAR* ARABIC_FIXED_CAND[9] = 
		{
			L"\x002E\x061F\x060C\x0021\x061B\x0022\x0040\x003A\x0031\x003B\x0028\x0029\x002A\x002D",
			L"\x062C\x062D\x062E\x0032",
			L"\x0627\x0623\x0625\x0622\x0628\x062A\x062B\x0033",
			L"\x0637\x0638\x0639\x063A\x0034",
			L"\x0633\x0634\x0635\x0636\x0035",
			L"\x062F\x0630\x0631\x0632\x0036",
			L"\x0648\x0624\x0649\x064A\x0626\x0621\x0037",
			L"\x0645\x0646\x0647\x0629\x0038",
			L"\x0641\x0642\x0643\x0644\x0039",
		};

		const LDB_CHAR* cand;

		cand = ARABIC_FIXED_CAND[*p - LDB_KEY_1];

		c = IIMEString_Length(cand);

		IIME_ASSERT(c > 0);

		len_limit = e->len + GetEditFreeSpace(pThis);

		d = pThis->cand;
		pThis->ccnt = 0;
		for (i = 0; i < c; i++)
		{
			p = cand + i;
			len = 1;
			if (len > len_limit)
				continue;
			d->len = len;
			d->str = p;
			d->width = pThis->label_text_width + GetCandStrWidth(pThis, p, len);
			d->flags = 0;
			d++;
			pThis->ccnt++;
		}
	}
	else
#endif
#if defined(HISENSE_PRODUCT)
    if (pThis->language == IIME_LANG_ENGLISH && pThis->ecnt == 1 && e->len == 1 && *p >= LDB_KEY_2 && *p <= LDB_KEY_9)
    {
        static const LDB_CHAR* L_ENG_FIXED_CAND[8] =
        {
            L"abc2", L"def3", L"ghi4", L"jkl5",
            L"mno6", L"pqrs7", L"tuv8", L"wxyz9"
        };

        static const LDB_CHAR* U_ENG_FIXED_CAND[8] =
        {
            L"ABC2", L"DEF3", L"GHI4", L"JKL5",
            L"MNO6", L"PQRS7", L"TUV8", L"WXYZ9"
        };

        const LDB_CHAR* cand;
        if (cflags == LDB_LOWER_CASE)
            cand = L_ENG_FIXED_CAND[*p - LDB_KEY_2];
        else
            cand = U_ENG_FIXED_CAND[*p - LDB_KEY_2];

        c = IIMEString_Length(cand);

        IIME_ASSERT(c > 0);

        /* Check each match result, those within length limit
           are saved as candidate. */

        len_limit = e->len + GetEditFreeSpace(pThis);

        d = pThis->cand;
        pThis->ccnt = 0;
        for (i = 0; i < c; i++)
        {
            p = cand + i;
            len = 1;
            if (len > len_limit)
                continue;
            d->len = len;
            d->str = p;
            d->width = pThis->label_text_width + GetCandStrWidth(pThis, p, len);
            d->flags = 0;
            d++;
            pThis->ccnt++;
        }
    }
    else
#endif
#ifdef KMX_THI_MT_CANDIDATE_BOX
    if (IIME_ime_is_thai_mt()&&pThis->ecnt == 1&& ((*p >= LDB_KEY_0 && *p <= LDB_KEY_9)))
    {
    	static const LDB_CHAR* THAI_FIXED_CAND[10] = 
    	{
    		L"\x0020\x0E48\x0E49\x0E4A\x0E4B\x0E47\x0E4C\x0E24\x0E26\x0E2F\x0E3F\x0030",
              L"\x002C\x002E\x003A\x003B\x002D\x0025\x0028\x0029\x0021\x0031",
    		L"\x0E01\x0E02\x0E04\x0E06\x0E07\x0E08\x0E09\x0032",
    		L"\x0E0A\x0E0B\x0E0C\x0E0D\x0E0E\x0E0F\x0E10\x0033",
    		L"\x0E11\x0E12\x0E13\x0E14\x0E15\x0E16\x0E17\x0034",
    		L"\x0E18\x0E19\x0E1A\x0E1B\x0E1C\x0E1D\x0E1E\x0035",
    		L"\x0E1F\x0E20\x0E21\x0E22\x0E23\x0E25\x0E27\x0036",
    		L"\x0E28\x0E29\x0E2A\x0E2B\x0E2C\x0E2D\x0E2E\x0037",
    		L"\x0E31\x0E34\x0E35\x0E36\x0E37\x0E38\x0E39\x0E30\x0038",
    		L"\x0E32\x0E33\x0E40\x0E41\x0E42\x0E43\x0E44\x0E46\x0039",
    	};
        const LDB_CHAR* cand;
       cand = THAI_FIXED_CAND[*p - LDB_KEY_0];
    	c = IIMEString_Length(cand);

    	IIME_ASSERT(c > 0);

    	len_limit = e->len + GetEditFreeSpace(pThis);

    	d = pThis->cand;
    	pThis->ccnt = 0;
    	for (i = 0; i < c; i++)
    	{
    		p = cand + i;
    		len = 1;
    		if (len > len_limit)
    			continue;
    		d->len = len;
    		d->str = p;
    		d->width = pThis->label_text_width + GetCandStrWidth(pThis, p, len);
    		d->flags = 0;
    		d++;
    		pThis->ccnt++;
    	}
    }
    else if (IIME_ime_is_en_mt()&&pThis->ecnt == 1&& ((*p >= LDB_KEY_0 && *p <= LDB_KEY_9)))
    {
    	static const LDB_CHAR* L_ENG_FIXED_CAND[10] = 
    	{
    		L" 0",
    		L"\x0020\x002e\x002c\x003f\x0021\x0031\x003a\x003b\x0027\x002d\x005c\x0040\x002f\x0025\x0026\x0028\x0029\x005b\x005d\x007b\x007d\x003c\x003e\x0022\x0024\x002b\x003d\x0023\x002a\x005e\x005f\x007e\x0060\x007c",
              L"abc2", L"def3", L"ghi4", L"jkl5",
            	L"mno6", L"pqrs7", L"tuv8", L"wxyz9"
    	};
	static const LDB_CHAR* U_ENG_FIXED_CAND[10] = 
    	{
    		L" 0",
    		L"\x0020\x002e\x002c\x003f\x0021\x0031\x003a\x003b\x0027\x002d\x005c\x0040\x002f\x0025\x0026\x0028\x0029\x005b\x005d\x007b\x007d\x003c\x003e\x0022\x0024\x002b\x003d\x0023\x002a\x005e\x005f\x007e\x0060\x007c",
              L"ABC2", L"DEF3", L"GHI4", L"JKL5",
            	L"MNO6", L"PQRS7", L"TUV8", L"WXYZ9"
    	};
        const LDB_CHAR* cand;
	
	if(IIME_ime_is_en_Abc_mt())
	{
		if(pThis->m_wPreChar ==' '||pThis->m_wPreChar =='\n'||pThis->m_wPreChar == 0)
		{
			cand = U_ENG_FIXED_CAND[*p - LDB_KEY_0];
		}
		else
		{
			cand = L_ENG_FIXED_CAND[*p - LDB_KEY_0];
		}
	}
	else
	{
		if (IIME_ime_is_en_abc_mt())
			cand = L_ENG_FIXED_CAND[*p - LDB_KEY_0];
		else
			cand = U_ENG_FIXED_CAND[*p - LDB_KEY_0];
	}
    	c = IIMEString_Length(cand);

    	IIME_ASSERT(c > 0);

    	len_limit = e->len + GetEditFreeSpace(pThis);

    	d = pThis->cand;
    	pThis->ccnt = 0;
    	for (i = 0; i < c; i++)
    	{
    		p = cand + i;
    		len = 1;
    		if (len > len_limit)
    			continue;
    		d->len = len;
    		d->str = p;
    		d->width = pThis->label_text_width + GetCandStrWidth(pThis, p, len);
    		d->flags = 0;
    		d++;
    		pThis->ccnt++;
    	}
    }
    else
#endif
    {
        c = ldbapi4_match(pThis->dbobj, his, his_len, 0, 0,
            p, e->len, cflags, CAND_LIST_SIZE);

        IIME_ASSERT(c > 0);

        /* Check each match result, those within length limit
           are saved as candidate. */

        len_limit = e->len + GetEditFreeSpace(pThis);

        d = pThis->cand;
        pThis->ccnt = 0;
        for (i = 0; i < c; i++)
        {
            p = ldbapi4_get_match(pThis->dbobj, i, &len);
            if (len > len_limit)
                continue;
            if (IS_FLAG_SET(pThis->flags, FLAG_STAR_AS_ONE) && *p == '1')
                continue;
#ifdef SPREADTONE_SPECIAL
            if (pThis->language == IIME_LANG_VIETNAMESE && *p == '1' && len == 1)
                continue;
#endif
            d->len = len;
            d->str = p;
            d->width = pThis->label_text_width + GetCandStrWidth(pThis, p, len);
            d->flags = 0;
            d++;
            pThis->ccnt++;
        }
    }

    IIME_ASSERT(pThis->ccnt > 0);

    /* Count for EUDW candidate at the end */

    if (IS_FLAG_SET(pThis->flags, FLAG_EUDW_DISABLED))
    {
        /* Do nothing */
    }
    else
    {
        d->len = pThis->eudwp_len;
        d->str = pThis->eudwp;
        d->width = pThis->label_text_width + GetCandStrWidth(pThis, d->str, d->len);
        d->flags = FLAG_CAND_EUDW;
        pThis->ccnt++;
    }

    /* Focus first candidate by default */

    SET_FLAG(pThis->cand[0].flags, FLAG_PAGE_START);
    OnCandLocateFocus(pThis, 0);

    OnCandSync(pThis);
}

static int IsEudwCandidate(
    THIS_DATA*              pThis,
    int                     index)
/*
 * Check if specified candidate is EUDW candidate.
 *
 * Return non-zero if the candidate is EUDW candidate.
 * Return zero otherwise.
 */
{
    /*#ifdef NO_EUDW*/
    if (IS_FLAG_SET(pThis->flags, FLAG_EUDW_DISABLED))
    {
        return 0;
    }
    else
    {
        IIME_ASSERT(pThis->ccnt > 0);
        IIME_ASSERT( (index >= 0) && (index < pThis->ccnt) );

        return IS_FLAG_SET(pThis->cand[index].flags, FLAG_CAND_EUDW) ? 1 : 0;
    }
    /*#endif*/
}

static void GetContent(
    THIS_DATA*              pThis,
    PIIME_IMM_CONTENT         pContent)
/*
 * Return UI content at current state.
 */
{
    int                     i;
    int                     c;
    int                     c2;
    IIME_WCHAR*               p;
    const IIME_WCHAR*         p2;

    memset(pContent, 0, sizeof(IIME_IMM_CONTENT));

    /* Fill commit content */

    if (IS_FLAG_SET(pThis->flags, FLAG_COMMIT))
    {
        IIME_ASSERT(pThis->commit_len >= 0);
        IIME_ASSERT(pThis->commit_len <= COMMIT_LENGTH_LIMIT);
        memcpy(pContent->pCommit, pThis->commit, STR_BYTES(pThis->commit_len));
        pContent->pCommit[pThis->commit_len] = 0;
    }

    /* Fill edit content */

    if (pThis->state == STATE_EUDW)
    {
        if (pThis->eudwt_len <= CNT_MAX_EDIT - 1)
            c = pThis->eudwt_len;
        else
            c = CNT_MAX_EDIT - 1;
        memcpy((IIME_WCHAR*)(pContent->pPreBuffer), pThis->eudwt, STR_BYTES(c));
    }
    else
    {
        if (IS_FLAG_SET(pThis->flags, FLAG_WORD_INPUT))
        {
            /* Ignore edit content in word input mode */
        }
        else 
        {
            /* Set edit content in phrase input mode */
            pContent->nPreEdit = (unsigned short) pThis->ecnt;
            if (pThis->ecnt > 0)
            {
                pContent->iHotPreEdit = (unsigned short) pThis->ecur;
                p = (IIME_WCHAR*) (pContent->pPreBuffer);
                c = 0;
                for (i = 0; i < pThis->ecnt; i++)
                {
                    memcpy(p, pThis->edit_str + pThis->edit[i].pos, STR_BYTES(pThis->edit[i].len));
                    p += pThis->edit[i].len;
                    #ifdef OPT_EXTRA_SPACE
                        if ( (i + 1 < pThis->ecnt) && (pThis->edit[i+1].len == 0) )
                        {
                            *p = CHAR_SPACE;
                            p++;
                            c++;
                        }
                    #endif
                    *p = 0;
                    p++;
                    c += pThis->edit[i].len + 1;
                }
                IIME_ASSERT(c <= CNT_MAX_EDIT);
            }
            else
            {
                pContent->iHotPreEdit = 0;
            }
        }
    }

    /* Fill candidate content */

    pContent->nCand = (unsigned short) pThis->cpct;
    if (pThis->cpct > 0)
    {
        pContent->iHotCand = (unsigned short) pThis->cpcu;
        p = (IIME_WCHAR*) (pContent->pCandBuffer);
        c = 0;
        for (i = 0; i < pThis->cpct; i++)
        {
            c2 = pThis->cand[pThis->cpfi + i].len;
            p2 = pThis->cand[pThis->cpfi + i].str;
            if (c2 > CNT_MAX_CAND - c - 1)
                c2 = CNT_MAX_CAND - c - 1;      /* Chopped by buffer limit */
            memcpy(p, p2, STR_BYTES(c2));
            p += c2;
            *p = 0;
            p++;
            c += c2 + 1;
            if (c >= CNT_MAX_CAND)
                break;
        }
        IIME_ASSERT(c <= CNT_MAX_CAND);
    }
    else
    {
        pContent->iHotCand = 0;
    }

    /* Fill eudw content */

    if (pThis->state == STATE_EUDW)
    {
        pContent->nUserWord = (unsigned short) (pThis->ulen + 1);  /* ??? Report 1 extra */
        pContent->iHotUserWord = (unsigned short) pThis->upos;
        memcpy(pContent->pUserWordBuffer, pThis->ustr, STR_BYTES(pThis->ulen));
        pContent->pUserWordBuffer[pThis->ulen] = 0;
    }

    /* Fill flags */

    if (pThis->cpfi > 0)
        pContent->flagPageCand |= IIME_FLAG_PAGEDOWN;

    if (pThis->cpfi + pThis->cpct < pThis->ccnt)
        pContent->flagPageCand |= IIME_FLAG_PAGEUP;

    switch (pThis->state)
    {
        case STATE_CAND:
            pContent->flagState = IIME_FLAG_STATE_CAND;
            break;
        case STATE_EUDW:
            pContent->flagState = IIME_FLAG_STATE_USER;
            break;
        default: /* STATE_EDIT */
            pContent->flagState = IIME_FLAG_STATE_EDIT;
            break;
    }
}

static void InitWidthParams(
    THIS_DATA*              pThis,
    const IIME_IMM_PARAMS*    imm_params)
{
    unsigned short          c;

    IIME_ASSERT(imm_params);

    pThis->pGetTextWidthFunc = imm_params->pGetTextWidthFunc;

    pThis->label_text_width = imm_params->nLableWidth;
    if (pThis->label_text_width < 0)
        pThis->label_text_width = 0;

    c = 0x57;
    pThis->min_candidate_width = pThis->pGetTextWidthFunc(&c, 1);
    if (pThis->min_candidate_width <= 0)
        pThis->min_candidate_width = 1;

    pThis->total_window_width = imm_params->nWinWidth;
    if (pThis->total_window_width <= 0)
        pThis->total_window_width = (pThis->label_text_width + pThis->min_candidate_width) * 9;
}

static int GetCandStrWidth(
    THIS_DATA*              pThis,
    const IIME_WCHAR*         str,
    int                     len)
{
    int                     w;

    w = pThis->pGetTextWidthFunc(str, len);

    if (w < pThis->min_candidate_width)
        w = pThis->min_candidate_width;

    return w;
}
