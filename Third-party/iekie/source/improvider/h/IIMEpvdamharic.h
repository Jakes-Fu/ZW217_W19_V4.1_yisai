/**************************************************************************
 IIMEpvdamharic.h
 
     Description:
     
     Author:      ke wen (kew@IIME.com.cn)
     Date Time:   12/4/2005 12:33:03 PM

     Copyright (C) 2000-2005 Beijing Chinese Star Cyber Technology Ltd.
     All rights reserved.

 **************************************************************************/

#ifndef __IIMEPVDAMHARIC_H__
#define __IIMEPVDAMHARIC_H__

/* Constructor & destructor */

Provider_Com* AmharicProvider_Create(
    PIIME_IMM_PARAMS          pImmParams,
    PIIME_USER_PARAMS         pUserParam1,
    PIIME_USER_PARAMS         pUserParam2,
    const void*             pdbMisc,
	const void*				pdbMO,
    const IIME_MULTI_STRING_TABLE* pMulti_string_table,
    IIME_INT                  idMethod);

void AmharicProvider_Delete(
    Provider_Com*           pProvider);

#endif /* __IIMEPVDAMHARIC_H__ */