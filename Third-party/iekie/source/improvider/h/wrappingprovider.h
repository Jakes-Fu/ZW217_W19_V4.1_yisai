/**************************************************************************
 wrappingprovider.h
 
     Description:
	 
     Author:      ke wen (kew@IIME.com.cn)
     Date Time:   11/30/2005 10:57:06 AM

     Copyright (C) 2000-2005 Beijing Chinese Star Cyber Technology Ltd.
     All rights reserved.

 **************************************************************************/

#ifndef __WRAPPINGPROVIDER_H__
#define __WRAPPINGPROVIDER_H__

typedef struct tagWrappingProvider
{
	DECLARE_PROVIDER_INTERFACE()

	/* property*/
	IIME_INT	m_iState;	
	const void*	m_pdbMisc;		/* basic db */
	const void*	m_pdbMO;		/* magic-one misc db*/
	IIME_INT	m_idMethod;		/* stroke or pinyin*/

	IIME_IMM_OPTIONS	*m_pOption;
	IIME_IMM_PARAMS	*m_pImmParam;
	IIME_USER_PARAMS	m_UserParam1;	/* used for stroke or pinyin*/
	IIME_USER_PARAMS	m_UserParam2;	/* used for magic-one*/

	
	Provider_Com *m_pNestProvider;

} WrappingProvider;

Provider_Com* WrappingProvider_Create(PIIME_IMM_PARAMS  pImmParams,
									  PIIME_USER_PARAMS pUserParam1,
									  PIIME_USER_PARAMS pUserParam2,
									  const void* pdbMisc,
									  const void* pdbMO,
                                      const IIME_MULTI_STRING_TABLE* pMulti_string_table,
									  IIME_INT idMethod);

void WrappingProvider_Delete(Provider_Com* pThis);

#endif /*__WRAPPINGPROVIDER_H__*/
