
/****************************************************************************
 *
 * IIMEpvdmo.h
 *
 */

#ifndef IIMEPVDMO_H
#define IIMEPVDMO_H

typedef struct tagIIMEMagicProvider
{
	DECLARE_PROVIDER_INTERFACE()
    void*   m_pData;
} IIMEMagicProvider;

/* Constructor & destructor */

Provider_Com* IIMEMagicProvider_Create(
    PIIME_IMM_PARAMS          pImmParams,
    PIIME_USER_PARAMS         pUserParam1,
    PIIME_USER_PARAMS         pUserParam2,
    const void*				pdbMisc,
	const void*				pdbMO,
    const IIME_MULTI_STRING_TABLE* pMulti_string_table,
    IIME_INT                  idMethod);

void IIMEMagicProvider_Delete(
    Provider_Com*           pProvider);

IIME_UINT IIMEMagicProvider_SetInputMethod(
    Provider_Com*           pIMP,
    IIME_UINT                 nNewInputMethod,
    PIIME_IMM_CONTENT         pContent);

#endif /* IIMEPVDMO_H */
