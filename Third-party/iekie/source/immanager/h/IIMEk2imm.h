
/**************************************************************************
 csk2imm.h
 
     Description: IIME_IMM definition
				  this file only be include in csk2imm.c, don't inlcude it 
				  in any other files!!!!
	 
     Author:      ke wen (kew@cstar.com.cn)
     Date Time:   11/25/2005 10:16:12 PM

     Copyright (C) 2000-2005 Beijing Chinese Star Cyber Technology Ltd.
     All rights reserved.

 **************************************************************************/

#ifndef __IIMEK2IMM_H__
#define __IIMEK2IMM_H__


typedef Provider_Com* (*PFN_PROVIDER_CREATE)(PIIME_IMM_PARAMS  pImmParams,
										     PIIME_USER_PARAMS pUserParam1,
										     PIIME_USER_PARAMS pUserParam2,
										     const void * pdbMisc,
											 const void * pdbMO,
                                             const IIME_MULTI_STRING_TABLE* pMulti_string_table,
										     IIME_INT idMethod);

typedef void (*PFN_PROVIDER_DELETE)(Provider_Com *p);

typedef struct tagProviderAttribute
{
	IIME_INT	m_idIM;
	IIME_INT	m_idBaseIM;

    PFN_PROVIDER_CREATE m_pfnCreate;
    PFN_PROVIDER_DELETE m_pfnDelete;

} ProviderAttribute;


#endif /*__IIMEK2IMM_H__*/
