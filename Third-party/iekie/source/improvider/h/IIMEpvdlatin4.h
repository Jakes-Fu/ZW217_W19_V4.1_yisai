
/****************************************************************************
 *
 * IIMEpvdlatin4.h
 *
 */

#ifndef IIMEPVDLATIN4_H
#define IIMEPVDLATIN4_H
//ZHMCH	×¢ÊÍstar
/*
#ifndef IIME_PID_VERMAJOR
#define IIME_PID_VERMAJOR	1
#endif
#ifndef IIME_PID_NAME0
#define IIME_PID_NAME0	1
#endif
#ifndef IIME_PID_NAME1
#define IIME_PID_NAME1	1
#endif
#ifndef IIME_PID_NAME2
#define IIME_PID_NAME2	1
#endif
#ifndef IIME_PID_NAME3
#define IIME_PID_NAME3	1
#endif
#ifndef IIME_PID_NAME4
#define IIME_PID_NAME4	1
#endif
#ifndef IIME_PID_NAME5
#define IIME_PID_NAME5	1
#endif

#ifndef IIME_PID_VERMINOR
#define IIME_PID_VERMINOR 00
#endif
*/
//ZHMCH	×¢ÊÍend
typedef struct tagIIMELatin4Provider
{
	DECLARE_PROVIDER_INTERFACE()
    void*   m_pData;
} IIMELatin4Provider;

/* Constructor & destructor */

Provider_Com* IIMELatin4Provider_Create(
    PIIME_IMM_PARAMS          pImmParams,
    PIIME_USER_PARAMS         pUserParam1,
    PIIME_USER_PARAMS         pUserParam2,
    const void*             pdbMisc,
	const void*				pdbMO,
    const IIME_MULTI_STRING_TABLE* pMulti_string_table,
    IIME_INT                  idMethod);

void IIMELatin4Provider_Delete(
    Provider_Com*           pProvider);


#endif /* IIMEPVDLATIN4_H */
