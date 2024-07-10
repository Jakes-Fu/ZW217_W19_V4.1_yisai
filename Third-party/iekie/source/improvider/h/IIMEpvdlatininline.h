/****************************************************************************
 *
 * IIMEpvdlatininline.h
 *
 */

#ifndef IIMEPVDLATININLINE_H
#define IIMEPVDLATININLINE_H

/* Constructor & destructor */

Provider_Com* IIMELatinInlineProvider_Create(
    PIIME_IMM_PARAMS          pImmParams,
    PIIME_USER_PARAMS         pUserParam1,
    PIIME_USER_PARAMS         pUserParam2,
    const void*             pdbMisc,
	const void*				pdbMO,
    const IIME_MULTI_STRING_TABLE* pMulti_string_table,
    IIME_INT                  idMethod);

void IIMELatinInlineProvider_Delete(
    Provider_Com*           pProvider);


#endif /* IIMEPVDLATINFULL_H */
