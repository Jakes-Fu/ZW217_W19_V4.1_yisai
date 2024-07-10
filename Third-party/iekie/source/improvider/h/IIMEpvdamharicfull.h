/****************************************************************************
 *
 * IIMEpvdlatinfull.h
 *
 */

#ifndef IIMEPVDLATINFULL_H
#define IIMEPVDLATINFULL_H

#define MAX_MATCH 32

IIME_FORWARD_STRUCT(IIMEAmharicFullProvider);//IIMELatinFullProvider	ZHMCH
//IIMELatinFullProvider	ZHMCH
typedef void (*PFNGETMULTITYPE)(IIMEAmharicFullProvider *pThis, LDB_CHAR key, LDB_CHAR prev_char, LDB_CHAR* pMultiList, int* pCount);
typedef int (*PFNISVALIDATEINPUT)(IIMEAmharicFullProvider *pThis, LDB_CHAR c);

//IIMELatinFullProvider	ZHMCH
struct tagIIMEAmharicFullProvider
{
	DECLARE_PROVIDER_INTERFACE()

	KMX_IME_ENGINE	m_Engine;

	IIME_INT          m_idMethod;

    unsigned short  m_flagFinish;
    unsigned short  m_flagCase;
    unsigned short  m_flagRTL;
    unsigned short  m_bCapLock;
    unsigned short  m_bSym;

    int             m_language;

    const QWERTY_KEYMAP *m_keymap;

    const void*     pdbMisc;

	IIME_WCHAR        m_pInput[64];
	int				m_nInput;
	
	IIME_WCHAR		m_PreChar;		/* used for GetMultiList */

	IIME_WCHAR		m_UnderChar;	/* uncommit char */
	IIME_WCHAR		m_PreKey;		/* the key type before */
	unsigned short	m_nCount;		/* the times of the same key of m_PreKey */

	KMX_LATIN_CANDIDATE	m_candidate[MAX_MATCH + 1];
	int				m_nCandidate;

	int             m_pPageMark[32];
    int             m_nCurPage;
	int				m_iHotCandidate;


	IIME_IMM_CONTENT	m_immContent;

	/* imm metriIIME */
    int						m_nWinWidth;           /* [in] Candidate window width*/
    int						m_nLableWidth;         /* [in] Lable width*/
    IIME_GET_TEXT_WIDTH_FUNC	m_pGetTextWidthFunc;

	/* special functions */
	PFNGETMULTITYPE			m_pfnGetMultiType;
	PFNISVALIDATEINPUT		m_pfnIsValidateInput;

    /* call back for get context */
    CALLBACK_GET_CONTEXT    m_pfnGetContext;
};

/* Constructor & destructor */

Provider_Com* IIMEAmharicFullProvider_Create(
    PIIME_IMM_PARAMS          pImmParams,
    PIIME_USER_PARAMS         pUserParam1,
    PIIME_USER_PARAMS         pUserParam2,
    const void*             pdbMisc,
	const void*				pdbMO,
    const IIME_MULTI_STRING_TABLE* pMulti_string_table,
    IIME_INT                  idMethod);

void IIMELatinAmhaircProvider_Delete(
    Provider_Com*           pProvider);


#endif /* IIMEPVDLATINFULL_H */
