/****************************************************************************
 *
 * IIMEpvdamharicfull_2.h
 *
 */

#ifndef IIMEPVDAMHARICFULL_H
#define IIMEPVDAMHARICFULL_H

extern Provider_Com* IIMEAmharicFullProvider2_Create(
    PIIME_IMM_PARAMS          pImmParams,
    PIIME_USER_PARAMS         pUserParam1,
    PIIME_USER_PARAMS         pUserParam2,
    const void*             pdbMisc,
	const void*				pdbMO,
    const IIME_MULTI_STRING_TABLE* pMulti_string_table,
    IIME_INT                  idMethod);

extern void IIMEAmharicFullProvider2_Delete(
    Provider_Com*           pProvider);

#endif /* IIMEPVDAMHARICFULL_H */
