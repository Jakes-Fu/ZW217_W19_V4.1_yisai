
/****************************************************************************
 *
 * IIMEpvdmo.c
 *
 */

#include "IIMEstd.h"
#include "provider_com.h"
#include "chinese_symbol.h"

#include "IIMEpvdmo.h"
#include "ldbapi4.h"

#include "IIMEpvdlatin4.h"//ZHMCH	解决未定义!


#ifndef IIME_VK_10
    #define IIME_VK_10    0x3A
    #define IIME_VK_11    0x3B
    #define IIME_VK_12    0x3C
    #define IIME_VK_13    0x3D
    #define IIME_VK_14    0x3E
    #define IIME_VK_15    0x3F
#endif

#ifdef MAGICONE_PROMPT_CHINESE
static const unsigned short s_prompt_string[4] = {0x65B0, 0x5355, 0x8BCD, 0};
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

/*
 * To enable generating standby space output:
 *   #define OPT_STANDBY_SPACE
 */

#define OPT_STANDBY_SPACE

/*
 * To enable generating standby number zero output:
 *   #define OPT_STANDBY_NUM0
 */

#define OPT_STANDBY_NUM0

/*
 * To enable case switching by up key in edit state:
 *   #define OPT_UP_CASESWITCH
 */

#define OPT_UP_CASESWITCH

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
#define CAND_LIST_SIZE      80

/* EUDW buffer size */
#define EUDW_BUFFER_SIZE    EDIT_BUFFER_SIZE

/****************************************************************************
 *
 * Private data defines
 *
 */

#ifndef IIME_VK_LONGBACKSPACE
    #define IIME_VK_LONGBACKSPACE (IIME_VK_BACKSPACE | IIME_VK_LONGCLICK)
#endif

/* GB2312 Candidates list for initial state of magic mode */

/* Constants for THIS_DATA.state */

#define STATE_MAGIC         1
#define STATE_EDIT          2
#define STATE_CAND          3
#define STATE_EUDW          4

/* Constants for THIS_DATA.flags */

#define FLAG_MAGIC          0x0001  /* Set if magic mode */
#define FLAG_MULTITAP       0x0002  /* Set if multitap actived */
#define FLAG_ACTIVED        0x0004  /* Set if actived */
#define FLAG_HINT_ICASE     0x0010  /* Set if next phase start with initial case */

#define FLAG_EDIT_DIRTY     0x0100
#define FLAG_CAND_DIRTY     0x0200
#define FLAG_EUDW_DIRTY     0x0400
#define FLAG_COMMIT         0x0800

#define FLAG_MAGIC_QUIT     0x1000
#define FLAG_MAGIC_BACK     0x2000

#define FLAG_EUDW_DISABLED  0x10000

/* Constants for EDIT.flags */

#define FLAG_MATCH_KEY      0x0001
#define FLAG_INITIAL_CASE   0x0002
#define FLAG_UPPER_CASE     0x0004

/* Constants for CAND.flags */

#define FLAG_PAGE_START     0x0001
#define FLAG_CAND_EUDW      0x0002

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

typedef struct tagMO_THIS_DATA
{
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
    const IIME_WCHAR*         umts;   /* Multitap list */
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
    int                     is_sure_type;
    void*                   dbobj;

    /* Cached info from database engine */
    int                     emax;   /* Max edit item length set by database engine */
    const IIME_WCHAR*         eudwp;
    int                     eudwp_len;
    const IIME_WCHAR*         eudwt;
    int                     eudwt_len;

    const IIME_WCHAR*         mIIME;    /* magic candidates list */
    int                     mcc;    /* magic candidates count */
    
    IIME_WCHAR                magic_symbol_char[96];

    int                     py_mode;
} MO_THIS_DATA;

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
    MO_THIS_DATA*              pThis);

static void InitMagicMode(
    MO_THIS_DATA*              pThis);

/* MAGIC functions */

#if 0
static int OnMagicKeyQuery(
    MO_THIS_DATA*              pThis,
    int                     key);
#endif

static int OnMagicKeyEvent(
    MO_THIS_DATA*              pThis,
    int                     key);

static void OnMagicInit(
    MO_THIS_DATA*              pThis);

static void OnMagicExit(
    MO_THIS_DATA*              pThis);

/* EDIT functions */

static void SwitchEditState(
    MO_THIS_DATA*              pThis);

#if 0
static int OnEditKeyQuery(
    MO_THIS_DATA*              pThis,
    int                     key);
#endif

static int OnEditKeyEvent(
    MO_THIS_DATA*              pThis,
    int                     key);

static void OnEditNew(
    MO_THIS_DATA*              pThis);

static void OnEditAppend(
    MO_THIS_DATA*              pThis,
    int                     key);

static void OnEditDelete(
    MO_THIS_DATA*              pThis,
    int                     all);

static void OnEditReplace(
    MO_THIS_DATA*              pThis,
    const IIME_WCHAR*         buffer,
    int                     length);

static void OnEditSwitchCase(
    MO_THIS_DATA*              pThis);

static void OnEditPrev(
    MO_THIS_DATA*              pThis);

static void OnEditNext(
    MO_THIS_DATA*              pThis);

static void OnEditCommit(
    MO_THIS_DATA*              pThis);

/* CANDIDATE functions */

static void SwitchCandState(
    MO_THIS_DATA*              pThis);

#if 0
static int OnCandKeyQuery(
    MO_THIS_DATA*              pThis,
    int                     key);
#endif

static int OnCandKeyEvent(
    MO_THIS_DATA*              pThis,
    int                     key);

static void OnCandCommit(
    MO_THIS_DATA*              pThis,
    int                     index);

static void OnCandLocateFocus(
    MO_THIS_DATA*              pThis,
    int                     index);

static void OnCandSync(
    MO_THIS_DATA*              pThis);

static void OnCandFocusPrev(
    MO_THIS_DATA*              pThis);

static void OnCandFocusNext(
    MO_THIS_DATA*              pThis);

static void OnCandFocusPrevPage(
    MO_THIS_DATA*              pThis);

static void OnCandFocusNextPage(
    MO_THIS_DATA*              pThis);

/* EUDW functions */

static void SwitchEudwState(
    MO_THIS_DATA*              pThis);

#if 0
static int OnEudwKeyQuery(
    MO_THIS_DATA*              pThis,
    int                     key);
#endif

static int OnEudwKeyEvent(
    MO_THIS_DATA*              pThis,
    int                     key);

static void OnEudwTimerEvent(
    MO_THIS_DATA*              pThis);

static void OnEudwMultitapKey(
    MO_THIS_DATA*              pThis,
    int                     key);

static void OnEudwMultitapStart(
    MO_THIS_DATA*              pThis,
    int                     key);

static void OnEudwMultitapScroll(
    MO_THIS_DATA*              pThis);

static void OnEudwMultitapStop(
    MO_THIS_DATA*              pThis);

static void OnEudwDelete(
    MO_THIS_DATA*              pThis,
    int                     all);

static void OnEudwCaretBackward(
    MO_THIS_DATA*              pThis);

static void OnEudwCaretForward(
    MO_THIS_DATA*              pThis);

static void OnEudwCommit(
    MO_THIS_DATA*              pThis);

static void OnEudwCancel(
    MO_THIS_DATA*              pThis);

/* MISC functions */

static int IsNumString(
    const IIME_WCHAR*         string,
    int                     length);

static int GetEditFreeSpace(
    MO_THIS_DATA*              pThis);

static void Commit(
    MO_THIS_DATA*              pThis,
    const IIME_WCHAR*         string,
    int                     length);

static int GetHistory(
    MO_THIS_DATA*              pThis,
    IIME_WCHAR*               buffer,
    int                     max_chars);

static void GenerateCandidates(
    MO_THIS_DATA*              pThis);

static int IsEudwCandidate(
    MO_THIS_DATA*              pThis,
    int                     index);

static void GetContent(
    MO_THIS_DATA*              pThis,
    PIIME_IMM_CONTENT         pContent);

static void InitWidthParams(
    MO_THIS_DATA*              pThis,
    const IIME_IMM_PARAMS*    imm_params);

static int GetCandStrWidth(
    MO_THIS_DATA*              pThis,
    const IIME_WCHAR*         str,
    int                     len);

/****************************************************************************
 *
 * Implementation of interface functions
 *
 */

IIME_UINT IIMEMagicProvider_SetInputMethod(
    Provider_Com*           pIMP,
    IIME_UINT                 nNewInputMethod,
    PIIME_IMM_CONTENT         pContent)
{
    MO_THIS_DATA*              pThis;

    pThis = (MO_THIS_DATA*)(((IIMEMagicProvider*)pIMP)->m_pData);

    if (nNewInputMethod == IM_MAGIC_ONE)
    {
        InitMagicMode(pThis);
        if (pContent)
            GetContent(pThis, pContent);
        return nNewInputMethod;
    }
    else if (nNewInputMethod == IM_ENGLISH)
    {
        InitDefaultMode(pThis);
        if (pContent)
            GetContent(pThis, pContent);
        return nNewInputMethod;
    }
    else
    {
        if (pContent)
            memset(pContent, 0, sizeof(IIME_IMM_CONTENT));
        return IM_NONE;
    }
}

static IIME_RESULT IIMEMagicProvider_EventHandler(
    Provider_Com*           pIMP,
    IIME_UINT                 hEvent,
    IIME_UINT                 iMsg,
    PIIME_IMM_CONTENT         pContent)
{
    MO_THIS_DATA*              pThis;
    int                     event;
    int                     key;
    int                     is_handled;
    int                     pflags;

    pThis = (MO_THIS_DATA*)(((IIMEMagicProvider*)pIMP)->m_pData);

    event = hEvent;
    key = iMsg;

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
                case STATE_MAGIC:
                    is_handled = OnMagicKeyEvent(pThis, key);
                    break;

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

    if (IS_FLAG_SET(pThis->flags, FLAG_MAGIC_QUIT))
        return IIME_ERROR_RETURN_LAST_PROVIDER;
    else if (is_handled)
        return IIME_ERROR_USED;
    else
        return IIME_ERROR_UNUSED;
}

#if 0
static IIME_BOOL IIMEMagicProvider_IsWantedKey(
    Provider_Com*           pIMP,
    IIME_UINT                 iMsg)
{
    MO_THIS_DATA*              pThis;
    int                     is_acceptable;

    pThis = (MO_THIS_DATA*)(((IIMEMagicProvider*)pIMP)->m_pData);

    is_acceptable = 0;

    switch (pThis->state)
    {
        case STATE_MAGIC:
            is_acceptable = OnMagicKeyQuery(pThis, iMsg);
            break;

        case STATE_EDIT:
            is_acceptable = OnEditKeyQuery(pThis, iMsg);
            break;

        case STATE_CAND:
            is_acceptable = OnCandKeyQuery(pThis, iMsg);
            break;

        case STATE_EUDW:
            is_acceptable = OnEudwKeyQuery(pThis, iMsg);
            break;
    }

    return is_acceptable ? IIME_TRUE : IIME_FALSE;
}

static IIME_RESULT IIMEMagicProvider_Active(
    Provider_Com*           pIMP,
    IIME_BOOL                 bActivite)
{
    MO_THIS_DATA*              pThis;

    pThis = (MO_THIS_DATA*)(((IIMEMagicProvider*)pIMP)->m_pData);

    if (bActivite)
    {
        if (IS_FLAG_SET(pThis->flags, FLAG_MAGIC))
            InitMagicMode(pThis);
        else
            InitDefaultMode(pThis);
        SET_FLAG(pThis->flags, FLAG_ACTIVED);
    }
    else
    {
        CLR_FLAG(pThis->flags, FLAG_ACTIVED);
    }

    return IIME_ERROR_SUCCESS;
}
#endif

static IIME_RESULT IIMEMagicProvider_SetOptions(
    Provider_Com*           pIMP,
    const IIME_IMM_OPTIONS*   pOption)
{
    MO_THIS_DATA*              pThis;

    pThis = (MO_THIS_DATA*)(((IIMEMagicProvider*)pIMP)->m_pData);

    if (pOption->dwPYFuzzy & IIME_EUDW_DISABLED){
        SET_FLAG(pThis->flags, FLAG_EUDW_DISABLED);
    }
    else{
        CLR_FLAG(pThis->flags, FLAG_EUDW_DISABLED);
    }

    pThis->py_mode = pOption->dwPinyinMode;

    if (pOption->pfnCheckValidHanzi)
    {
        CALLBACK_CHECK_VALID_HANZI pfnCheck = pOption->pfnCheckValidHanzi;
        int i, j;
        j = 0;
        for (i = 0; i < pThis->mcc; i++)
        {
            IIME_WCHAR chr = pThis->mIIME[i];
            if ((*pfnCheck)(chr) != 0)
                pThis->magic_symbol_char[j++] = chr;
        }

        pThis->mcc = j;
        pThis->mIIME = pThis->magic_symbol_char;
    }
    return IIME_ERROR_SUCCESS;
}

static IIME_RESULT IIMEMagicProvider_GetOptions(
    Provider_Com*           pIMP,
    IIME_IMM_OPTIONS*         pOption)
{
    /* No options provided currently.
       Do nothing by design. */

    return IIME_ERROR_SUCCESS;
}

static IIME_RESULT IIMEMagicProvider_GetProperties(
    Provider_Com*           pIMP,
    int*         pProperties)
{
    /* No options provided currently.
       Do nothing by design. */

    return IIME_ERROR_SUCCESS;
}
Provider_Com* IIMEMagicProvider_Create(
    PIIME_IMM_PARAMS          pImmParams,
    PIIME_USER_PARAMS         pUserParam1,
    PIIME_USER_PARAMS         pUserParam2,
    const void*             pdbMisc,
	const void*				pdbMO,
    const IIME_MULTI_STRING_TABLE* pMulti_string_table,
    IIME_INT                  idMethod)
{
    IIMEMagicProvider*        pProvider;
    MO_THIS_DATA*              pThis;
    void*                   dbobj;
    LDB_INFO                dbinfo;
    LDB_INIT                dbinit;

    int                     database_avail;
    int                     object_size;

    /* Query database */

    if (ldbapi4_get_db_info(pdbMisc, &dbinfo))
    {
        database_avail = 1;
        object_size = sizeof(IIMEMagicProvider) + sizeof(MO_THIS_DATA) + dbinfo.engine_object_size;
    }
    else
    {
        /* Database not found */
        database_avail = 0;
        object_size = sizeof(IIMEMagicProvider) + sizeof(MO_THIS_DATA);
    }

    /*
     * Alloc memory for:
     *   1. provider object
     *   2. provider object private data
     *   3. database object
     */

    pProvider = (IIMEMagicProvider*) IIME_MALLOC(object_size);

    if (!pProvider)
    {
        /* Error: not enough memory */
        return 0;
    }

    pThis = (MO_THIS_DATA*) ((char*)pProvider + sizeof(IIMEMagicProvider));

    /* Initialize provider object interface */

    memset(pProvider, 0, sizeof(IIMEMagicProvider));

    pProvider->m_pData = pThis;

    pProvider->m_pfnEventHandle           = IIMEMagicProvider_EventHandler;

    pProvider->m_pfnSetOptions            = IIMEMagicProvider_SetOptions;
    pProvider->m_pfnGetOptions            = IIMEMagicProvider_GetOptions;
    pProvider->m_pfnQueryProperties       = IIMEMagicProvider_GetProperties;

    /* Initialize database object */
    
    if (database_avail)
    {
        dbobj = (char*)pThis + sizeof(MO_THIS_DATA);

        memset(&dbinit, 0, sizeof(dbinit));
        dbinit.database = pdbMisc;
        dbinit.user_data = pUserParam1->pUserCache;
        dbinit.user_data_size = pUserParam1->iUserCacheSize;

        if (!ldbapi4_initialize(dbobj, &dbinit))
        {
            /* Error: invalid database */
            IIME_FREE(pProvider);
            return 0;
        }
    }
    else
    {
        dbobj = 0;
    }

    /* Initialize provider object */

    memset(pThis, 0, sizeof(MO_THIS_DATA));

    pThis->dbobj = dbobj;
    pThis->is_sure_type = dbinfo.is_sure_type;

    if (database_avail)
    {
        pThis->emax = dbinfo.max_input_length;
        if (pThis->emax > CNT_MAX_CAND - 1)
            pThis->emax = CNT_MAX_CAND - 1;

    #ifdef MAGICONE_PROMPT_CHINESE
        pThis->eudwp = s_prompt_string;
        pThis->eudwp_len = 3;
    #else
        pThis->eudwp = dbinfo.eudw_prompt;
        pThis->eudwp_len = dbinfo.eudw_prompt_length;
    #endif

        pThis->eudwt = dbinfo.eudw_title;
        pThis->eudwt_len = dbinfo.eudw_title_length;
    }

    InitWidthParams(pThis, pImmParams);

    InitDefaultMode(pThis);

    if ( (idMethod == IM_ZHUYIN) ||
         (idMethod == IM_ZHUYIN_MT) ||
         (idMethod == IM_STROKE_BIG5) )
    {
        pThis->mIIME = big5_magic_candidates;
        pThis->mcc = big5_magic_number;
    }
    else
    {
        pThis->mIIME = gb_magic_candidates;
        pThis->mcc = gb_magic_number;
    }

	return (Provider_Com*) pProvider;
}

void IIMEMagicProvider_Delete(
    Provider_Com*           pProvider)
{
    MO_THIS_DATA*              pThis;

    if (pProvider)
    {
        pThis = (MO_THIS_DATA*)(((IIMEMagicProvider*)pProvider)->m_pData);

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
    MO_THIS_DATA*              pThis)
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

static void InitMagicMode(
    MO_THIS_DATA*              pThis)
/*
 * Initialize to magic-one mode.
 */
{
    pThis->flags = FLAG_MAGIC;
    pThis->state = STATE_MAGIC;

    OnMagicInit(pThis);
}

/****************************************************************************
 *
 * Implementation of MAGIC functions.
 *
 */

#if 0
static int OnMagicKeyQuery(
    MO_THIS_DATA*              pThis,
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

        case IIME_VK_LEFT:
        case IIME_VK_RIGHT:
        case IIME_VK_UP:
        case IIME_VK_DOWN:

        case IIME_VK_BACKSPACE:
        case IIME_VK_LONGBACKSPACE:
        case IIME_VK_SELECT:
        case IIME_VK_SPACE:
        case IIME_VK_AUX:
        case IIME_VK_STAR:
        case IIME_VK_POUND:

            is_acceptable = 1;
            break;
    }

    return is_acceptable;
}
#endif

static int OnMagicKeyEvent(
    MO_THIS_DATA*              pThis,
    int                     key)
/*
 * Process given key event.
 *
 * Return non-zero if the event is handled.
 * Return zero otherwise.
 */
{
    int                     is_handled;
    CAND*                   d;

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
            if (pThis->dbobj)
            {
                /* Switch to EDIT state, continue process this key */
                pThis->ecnt = 0;
                SwitchEditState(pThis);
                OnEditKeyEvent(pThis, key);
            }
            else
            {
                /* Key ignored, do nothing by design */
            }
            break;

        case IIME_VK_LEFT:
            is_handled = 1;
            if (IS_FLAG_CLR(pThis->flags, FLAG_EUDW_DISABLED) || !pThis->dbobj)
            {
                /* Key ignored, do nothing by design */
            }
            else
            {
                pThis->edit[0].len = 0;
                SET_FLAG(pThis->flags, FLAG_MAGIC_BACK);
                SwitchEudwState(pThis);
            }
            break;

        case IIME_VK_RIGHT:
            is_handled = 1;
            SET_FLAG(pThis->flags, FLAG_MAGIC_BACK);
            SwitchCandState(pThis);
            OnCandFocusNext(pThis);
            break;

        case IIME_VK_UP:
            is_handled = 1;
            /* Do nothing by design */
            break;

        case IIME_VK_DOWN:
            is_handled = 1;
            SET_FLAG(pThis->flags, FLAG_MAGIC_BACK);
            SwitchCandState(pThis);
            break;

        case IIME_VK_BACKSPACE:
        case IIME_VK_LONGBACKSPACE:
            is_handled = 1;
            OnMagicExit(pThis);
            break;

        case IIME_VK_SELECT:
        case IIME_VK_SPACE:
            is_handled = 1;
            d = pThis->cand + (pThis->cpfi + pThis->cpcu);
            Commit(pThis, d->str, d->len);
            OnMagicExit(pThis);
            break;

        case IIME_VK_AUX:
        case IIME_VK_STAR:
        case IIME_VK_POUND:
            is_handled = 1;
            /* Do nothing by design */;
            break;

        default:
            /* Pass through */
            break;
    }

    return is_handled;
}

static void OnMagicInit(
    MO_THIS_DATA*              pThis)
{
    int                     i;

    /* Set edit content */

    pThis->edit[0].pos = 0;
    pThis->edit[0].len = 1;
    pThis->edit_str[0] = pThis->mIIME[0];
    if (pThis->dbobj)
        pThis->edit_key[0] = ldbapi4_char_to_key(pThis->dbobj, pThis->mIIME[0]);
    else
        pThis->edit_key[0] = LDB_KEY_1;
    pThis->edit[0].flags = 0;

    pThis->ecur = 0;
    pThis->ecnt = 1;

    SET_FLAG(pThis->flags, FLAG_EDIT_DIRTY);

    /* Set candidate content */

    pThis->ccnt = pThis->mcc;

    for (i = 0; i < pThis->ccnt; i++)
    {
        pThis->cand[i].str = pThis->mIIME + i;
        pThis->cand[i].len = 1;
        pThis->cand[i].flags = 0;
        pThis->cand[i].width = pThis->label_text_width + pThis->min_candidate_width;
    }

    SET_FLAG(pThis->cand[0].flags, FLAG_PAGE_START);

    SET_FLAG(pThis->flags, FLAG_CAND_DIRTY);

    /* Select first candidate by default */

    OnCandLocateFocus(pThis, 0);
}

static void OnMagicExit(
    MO_THIS_DATA*              pThis)
{
    SET_FLAG(pThis->flags, FLAG_MAGIC_QUIT);

    pThis->ecnt = 0;
    pThis->ecur = 0;

    pThis->ccnt = 0;
    pThis->cpfi = 0;
    pThis->cpct = 0;
    pThis->cpcu = 0;

    pThis->state = STATE_MAGIC;
}

/****************************************************************************
 *
 * Implementation of EDIT functions.
 *
 */

static void SwitchEditState(
    MO_THIS_DATA*              pThis)
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
    MO_THIS_DATA*              pThis,
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
            is_acceptable = 1;
            break;

        case IIME_VK_SPACE:
            #ifdef OPT_STANDBY_SPACE
                is_acceptable = 1;
            #else
                is_acceptable = (pThis->ecnt > 0) ? 1 : 0;
            #endif
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

static int OnEditKeyEvent(
    MO_THIS_DATA*              pThis,
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
            is_handled = 1;
            #ifdef OPT_STANDBY_SPACE
                if ( (pThis->ecnt == 0) && IS_FLAG_CLR(pThis->flags, FLAG_MAGIC) )
                {
                    is_handled = 1;
                    pThis->commit[0] = CHAR_NUM0;
                    pThis->commit[1] = 0;
                    pThis->commit_len = 1;
                    SET_FLAG(pThis->flags, FLAG_COMMIT);
                    break;
                }
            #endif
            OnEditAppend(pThis, key);
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
            OnEditAppend(pThis, key);
            break;

        case IIME_VK_BACKSPACE:
            if (pThis->ecnt > 0)
            {
                is_handled = 1;
                OnEditDelete(pThis, 0);
            }
            else
            {
                /* Pass through */
            }
            break;

        case IIME_VK_LONGBACKSPACE:
            if (pThis->ecnt > 0)
            {
                is_handled = 1;
                OnEditDelete(pThis, 1);
            }
            else
            {
                /* Pass through */
            }
            break;

        case IIME_VK_SELECT:
            if (pThis->ecnt > 0)
            {
                is_handled = 1;
                OnEditCommit(pThis);
            }
            else
            {
                /* Pass through */
            }
            break;

        case IIME_VK_SPACE:
            if (pThis->ecnt > 0)
            {
                is_handled = 1;
                if (IS_FLAG_SET(pThis->flags, FLAG_MAGIC))
                    OnEditCommit(pThis);
                else
                    OnEditNew(pThis);
            }
            else
            {
                #ifdef OPT_STANDBY_SPACE
                    is_handled = 1;
                    pThis->commit[0] = CHAR_SPACE;
                    pThis->commit[1] = 0;
                    pThis->commit_len = 1;
                    SET_FLAG(pThis->flags, FLAG_COMMIT);
                #else
                    /* Pass through */
                #endif
            }
            break;

        case IIME_VK_STAR:
        case IIME_VK_POUND:
            if (pThis->ecnt > 0)
                is_handled = 1;
            break;

        case IIME_VK_AUX:
            if (pThis->ecnt > 0)
            {
                is_handled = 1;
                OnEditSwitchCase(pThis);
            }
            else
            {
                /* Pass through */
            }
            break;

        case IIME_VK_LEFT:
            if (pThis->ecnt > 0)
            {
                is_handled = 1;
                /*#ifdef NO_EUDW*/
                if (IS_FLAG_SET(pThis->flags, FLAG_EUDW_DISABLED)){
                    if (IS_FLAG_SET(pThis->flags, FLAG_MAGIC))
                        ;
                    else
                        OnEditPrev(pThis);
                /*#else*/
                }
                else{
                    if (IS_FLAG_SET(pThis->flags, FLAG_MAGIC))
                        SwitchEudwState(pThis);
                    else
                        OnEditPrev(pThis);
                /*#endif*/
                }
            }
            else
            {
                /* Pass through */
            }
            break;

        case IIME_VK_RIGHT:
            if (pThis->ecnt > 0)
            {
                is_handled = 1;
                if (IS_FLAG_SET(pThis->flags, FLAG_MAGIC))
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
            else
            {
                /* Pass through */
            }
            break;

        case IIME_VK_UP:
            if (pThis->ecnt > 0)
            {
                is_handled = 1;
                OnEditSwitchCase(pThis);
            }
            else
            {
                /* Pass through */
            }
            break;

        case IIME_VK_DOWN:
            if (pThis->ecnt > 0)
            {
                is_handled = 1;
                if (pThis->ccnt > 0)
                    SwitchCandState(pThis);
            }
            else
            {
                /* Pass through */
            }
            break;

        default:
            /* Pass through */
            break;
    }

    return is_handled;
}

static void OnEditNew(
    MO_THIS_DATA*              pThis)
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
    MO_THIS_DATA*              pThis,
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
        if (IS_FLAG_SET(pThis->flags, FLAG_HINT_ICASE))
            SET_FLAG(e->flags, FLAG_INITIAL_CASE);
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
    pThis->edit_str[i] = (IIME_WCHAR)(LDB_KEY_0 + (key - IIME_VK_0));
    pThis->edit_key[i] = (IIME_WCHAR)(LDB_KEY_0 + (key - IIME_VK_0));
    e->len++;
    SET_FLAG(e->flags, FLAG_MATCH_KEY);

    /* Mark edit content changed */

    SET_FLAG(pThis->flags, FLAG_EDIT_DIRTY);

    /* Generate new candidates */

    GenerateCandidates(pThis);
}

static void OnEditDelete(
    MO_THIS_DATA*              pThis,
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

    if ( IS_FLAG_SET(pThis->flags, FLAG_MAGIC) && (pThis->ecnt == 0) )
        OnMagicExit(pThis);
}

static void OnEditReplace(
    MO_THIS_DATA*              pThis,
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
    {
        if (pThis->dbobj)
            pThis->edit_key[e->pos+i] = ldbapi4_char_to_key(pThis->dbobj, buffer[i]);
        else
            pThis->edit_key[e->pos+i] = LDB_KEY_1;
    }
    e->len = length;

    SET_FLAG(pThis->flags, FLAG_EDIT_DIRTY);
}

static void OnEditSwitchCase(
    MO_THIS_DATA*              pThis)
{
    EDIT*                   e;
    int                     i;
    int                     skip_initial_case;

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
        if (skip_initial_case)
        {
            CLR_FLAG(e->flags, FLAG_INITIAL_CASE);
        }
        else
        {
            CLR_FLAG(e->flags, FLAG_INITIAL_CASE);
            SET_FLAG(e->flags, FLAG_UPPER_CASE);
        }
    }
    else if (IS_FLAG_SET(e->flags, FLAG_UPPER_CASE))
    {
        CLR_FLAG(e->flags, FLAG_UPPER_CASE);
    }
    else
    {
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
    MO_THIS_DATA*              pThis)
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
    MO_THIS_DATA*              pThis)
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
    MO_THIS_DATA*              pThis)
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

    if (IS_FLAG_SET(pThis->flags, FLAG_MAGIC))
        ldbapi4_update_user_data(pThis->dbobj, 0, 0, 0);

    ldbapi4_update_user_data(pThis->dbobj, pThis->commit, pThis->commit_len, 0);

    if (IS_FLAG_SET(pThis->flags, FLAG_MAGIC))
        ldbapi4_update_user_data(pThis->dbobj, 0, 0, 0);

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

    if (IS_FLAG_SET(pThis->flags, FLAG_MAGIC))
        OnMagicExit(pThis);
}

/****************************************************************************
 *
 * Implementation of CANDIDATE functions.
 *
 */

static void SwitchCandState(
    MO_THIS_DATA*              pThis)
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
    MO_THIS_DATA*              pThis,
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

static int OnCandKeyEvent(
    MO_THIS_DATA*              pThis,
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

    switch (key)
    {
        case IIME_VK_0:
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
            if (IS_FLAG_SET(pThis->flags, FLAG_MAGIC_BACK))
            {
                OnMagicExit(pThis);
            }
            else
            {
                OnCandLocateFocus(pThis, 0);
                OnCandSync(pThis);
                SwitchEditState(pThis);
            }
            break;

        case IIME_VK_SELECT:
            is_handled = 1;
            i = pThis->cpfi + pThis->cpcu;
            if (IsEudwCandidate(pThis, i))
                SwitchEudwState(pThis);
            else
                OnCandCommit(pThis, i);
            break;

        case IIME_VK_SPACE:
            is_handled = 1;
            i = pThis->cpfi + pThis->cpcu;
            if (IsEudwCandidate(pThis, i))
            {
                /* Do nothing by design */
            }
            else if (IS_FLAG_SET(pThis->flags, FLAG_MAGIC))
            {
                OnCandCommit(pThis, i);
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
            is_handled = 1;
            OnCandFocusPrev(pThis);
            break;

        case IIME_VK_RIGHT:
            is_handled = 1;
            OnCandFocusNext(pThis);
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
    MO_THIS_DATA*              pThis,
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

    if (IS_FLAG_SET(pThis->flags, FLAG_MAGIC))
    {
        Commit(pThis, pThis->edit_str + e->pos, e->len);
        if (pThis->dbobj)
        {
            ldbapi4_update_user_data(pThis->dbobj, 0, 0, 0);
            ldbapi4_update_user_data(pThis->dbobj, pThis->commit, pThis->commit_len, 0);
            ldbapi4_update_user_data(pThis->dbobj, 0, 0, 0);
        }
        OnMagicExit(pThis);
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
    MO_THIS_DATA*              pThis,
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
    MO_THIS_DATA*              pThis)
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
    MO_THIS_DATA*              pThis)
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
    MO_THIS_DATA*              pThis)
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
}

static void OnCandFocusPrevPage(
    MO_THIS_DATA*              pThis)
/*
 * Set focus to candidate in previous page.
 */
{
    if (pThis->cpfi > 0)
    {
        OnCandLocateFocus(pThis, pThis->cpfi - 1);
        OnCandSync(pThis);
    }
    else if (pThis->cpcu > 0)
    {
        OnCandLocateFocus(pThis, 0);
        OnCandSync(pThis);
    }
}

static void OnCandFocusNextPage(
    MO_THIS_DATA*              pThis)
/*
 * Set focus to candidate in next page.
 */
{
    int                     i;

    i = pThis->cpfi + pThis->cpct;

    if (i < pThis->ccnt)
    {
        OnCandLocateFocus(pThis, i);
        OnCandSync(pThis);
    }
    else if (pThis->cpcu < pThis->cpct - 1)
    {
        OnCandLocateFocus(pThis, pThis->cpfi + pThis->cpct - 1);
        OnCandSync(pThis);
    }
}

/****************************************************************************
 *
 * Implementation of EUDW functions.
 *
 */

static void SwitchEudwState(
    MO_THIS_DATA*              pThis)
/*
 * Switch to EUDW state.
 */
{
    EDIT*                   e;

    pThis->state = STATE_EUDW;
    SET_FLAG(pThis->flags, FLAG_EUDW_DIRTY);

    /* Multitap is not actived initialy */

    CLR_FLAG(pThis->flags, FLAG_MULTITAP);

    /* Set initial content */

    e = pThis->edit + pThis->ecur;
    if (match_pid_key(pThis->edit_key + e->pos, e->len))
    {
        /* set product ID info */
        setup_pid_string(pThis->ustr);
        pThis->ulen = e->len;
    }
    else
    {
        pThis->ustr[0] = 0;
        pThis->ulen = 0;
    }

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
    MO_THIS_DATA*              pThis,
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
    MO_THIS_DATA*              pThis,
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

        case IIME_VK_SPACE:
        case IIME_VK_AUX:
        case IIME_VK_STAR:
        case IIME_VK_POUND:
            is_handled = 1;
            /* Do nothing by design */
            break;

        case IIME_VK_LEFT:
            is_handled = 1;
            OnEudwMultitapStop(pThis);
            OnEudwCaretBackward(pThis);
            break;

        case IIME_VK_RIGHT:
            is_handled = 1;
            OnEudwMultitapStop(pThis);
            OnEudwCaretForward(pThis);
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
    MO_THIS_DATA*              pThis)
/*
 * Process timer event.
 */
{
    OnEudwMultitapStop(pThis);
}

static void OnEudwMultitapKey(
    MO_THIS_DATA*              pThis,
    int                     key)
/*
 * Process a multitap key input.
 */
{
    if (IS_FLAG_SET(pThis->flags, FLAG_MULTITAP))
    {
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
    MO_THIS_DATA*              pThis,
    int                     key)
/*
 * Start multitap for specified key.
 */
{
    IIME_WCHAR                c;

    IIME_ASSERT(IS_FLAG_CLR(pThis->flags, FLAG_MULTITAP));

    /* Check maximum length limit */

    if (pThis->ulen >= pThis->umax)
        return;

    /* Get multitap info from database engine */

    c = (IIME_WCHAR) (LDB_KEY_0 + (key - IIME_VK_0));

    pThis->umts = ldbapi4_get_multitype(pThis->dbobj, c,
        LDB_LOWER_CASE | LDB_UPPER_CASE, &(pThis->umtl));

    IIME_ASSERT(pThis->umtl > 0);
    IIME_ASSERT(pThis->umts != 0);

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
    MO_THIS_DATA*              pThis)
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
    MO_THIS_DATA*              pThis)
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

static void OnEudwDelete(
    MO_THIS_DATA*              pThis,
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
    MO_THIS_DATA*              pThis)
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
    MO_THIS_DATA*              pThis)
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
    MO_THIS_DATA*              pThis)
/*
 * End EUDW state. Set content back to edit item.
 */
{
    EDIT*                   e;
    int                     f;

    /* Notify eudw word to database engine */

    ldbapi4_update_user_data(pThis->dbobj, pThis->ustr, pThis->ulen, LDB_EUDW);

    if (IS_FLAG_SET(pThis->flags, FLAG_MAGIC))
    {
        Commit(pThis, pThis->ustr, pThis->ulen);
        OnMagicExit(pThis);
    }
    else
    {
        /* Update edit item */

        OnEditReplace(pThis, pThis->ustr, pThis->ulen);

        e = pThis->edit + pThis->ecur;
        CLR_FLAG(e->flags, FLAG_MATCH_KEY);

        CLR_FLAG(e->flags, FLAG_INITIAL_CASE);
        CLR_FLAG(e->flags, FLAG_UPPER_CASE);
        f = ldbapi4_check_case(pThis->dbobj, pThis->edit_str + e->pos, e->len);
        if (f == LDB_INITIAL_CASE)
            SET_FLAG(e->flags, FLAG_INITIAL_CASE);
        else if (f == LDB_UPPER_CASE)
            SET_FLAG(e->flags, FLAG_UPPER_CASE);

        SwitchEditState(pThis);
        GenerateCandidates(pThis);
    }
}

static void OnEudwCancel(
    MO_THIS_DATA*              pThis)
/*
 * Cancel EUDW state, back to EDIT state.
 */
{
    if (IS_FLAG_SET(pThis->flags, FLAG_MAGIC_BACK))
    {
        OnMagicExit(pThis);
    }
    else
    {
        SwitchEditState(pThis);
        GenerateCandidates(pThis);
    }
}

/****************************************************************************
 *
 * Implementation of MISC functions.
 *
 */

static int IsNumString(
    const IIME_WCHAR*         string,
    int                     length)
/*
 * Check if given string is a number string.
 *
 * Return non-zero if the string contains only number characters.
 * Return zero otherwise.
 */
{
    while (length > 0)
    {
        if ( (*string < CHAR_NUM0) || (*string > CHAR_NUM9) )
            return 0;
        string++;
        length--;
    }
    return 1;
}

static int GetEditFreeSpace(
    MO_THIS_DATA*              pThis)
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

static void Commit(
    MO_THIS_DATA*              pThis,
    const IIME_WCHAR*         string,
    int                     length)
{
    IIME_ASSERT(length >= 0);

    pThis->commit_len = length;
    memcpy(pThis->commit, string, STR_BYTES(length));

    SET_FLAG(pThis->flags, FLAG_COMMIT);
}

static int GetHistory(
    MO_THIS_DATA*              pThis,
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
    MO_THIS_DATA*              pThis)
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

    if (IS_FLAG_SET(e->flags, FLAG_INITIAL_CASE))
        cflags = LDB_INITIAL_CASE;
    else if (IS_FLAG_SET(e->flags, FLAG_UPPER_CASE))
        cflags = LDB_UPPER_CASE;
    else
        cflags = LDB_LOWER_CASE;

    p = IS_FLAG_SET(e->flags, FLAG_MATCH_KEY) ? (pThis->edit_key + e->pos) : (pThis->edit_str + e->pos);

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
        if ( IS_FLAG_SET(pThis->flags, FLAG_MAGIC) && IsNumString(p, len) )
        {
            IIME_ASSERT(len == e->len);
            memmove(pThis->cand + 1, pThis->cand, sizeof(CAND) * pThis->ccnt);
            pThis->cand[0].len = len;
            pThis->cand[0].str = p;
            pThis->cand[0].width = pThis->label_text_width + GetCandStrWidth(pThis, p, len);
            pThis->cand[0].flags = 0;
        }
        else
        {
            d->len = len;
            d->str = p;
            d->width = pThis->label_text_width + GetCandStrWidth(pThis, p, len);
            d->flags = 0;
        }
        d++;
        pThis->ccnt++;
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

#ifdef MAGICONE_PROMPT_CHINESE
        d->width = pThis->label_text_width + pThis->min_candidate_width * 3;
#else
        d->width = pThis->label_text_width + GetCandStrWidth(pThis, d->str, d->len);
#endif

        d->flags = FLAG_CAND_EUDW;
        pThis->ccnt++;
    }

    /* Focus first candidate by default */

    SET_FLAG(pThis->cand[0].flags, FLAG_PAGE_START);
    OnCandLocateFocus(pThis, 0);

    OnCandSync(pThis);
}

static int IsEudwCandidate(
    MO_THIS_DATA*              pThis,
    int                     index)
/*
 * Check if specified candidate is EUDW candidate.
 *
 * Return non-zero if the candidate is EUDW candidate.
 * Return zero otherwise.
 */
{
    /*#ifdef NO_EUDW*/
    if (IS_FLAG_SET(pThis->flags, FLAG_EUDW_DISABLED)){
        return 0;
    }
    /*#else*/
    else{
        IIME_ASSERT(pThis->ccnt > 0);
        IIME_ASSERT( (index >= 0) && (index < pThis->ccnt) );

        return IS_FLAG_SET(pThis->cand[index].flags, FLAG_CAND_EUDW) ? 1 : 0;
    }
    /*#endif*/
}

static void GetContent(
    MO_THIS_DATA*              pThis,
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
        default: /* STATE_MAGIC & STATE_EDIT */
            pContent->flagState = IIME_FLAG_STATE_EDIT;
            break;
    }

    if (pThis->py_mode == IIME_PINYIN_MODE_5)
    {
        pContent->nPreEdit = 0;
        memset(pContent->pPreBuffer, 0, sizeof(short) * 64);
    }
}

static void InitWidthParams(
    MO_THIS_DATA*              pThis,
    const IIME_IMM_PARAMS*    imm_params)
{
    unsigned short          c;

    IIME_ASSERT(imm_params);

    pThis->pGetTextWidthFunc = imm_params->pGetTextWidthFunc;

    pThis->label_text_width = imm_params->nLableWidth;
    if (pThis->label_text_width < 0)
        pThis->label_text_width = 0;

    /*
     * Using double avarage width of number characters as minimum candidate width.
     */

    c = 0x4E00;
    pThis->min_candidate_width = pThis->pGetTextWidthFunc(&c, 1);
    if (pThis->min_candidate_width <= 0)
        pThis->min_candidate_width = 1;

    pThis->total_window_width = imm_params->nWinWidth;
    if (pThis->total_window_width <= 0)
        pThis->total_window_width = (pThis->label_text_width + pThis->min_candidate_width) * 9;
}

static int GetCandStrWidth(
    MO_THIS_DATA*              pThis,
    const IIME_WCHAR*         str,
    int                     len)
{
    int                     w;

    w = pThis->pGetTextWidthFunc(str, len);

    if (w < pThis->min_candidate_width)
        w = pThis->min_candidate_width;

    return w;
}
