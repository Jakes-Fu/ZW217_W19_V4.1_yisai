/**************************************************************************
 IIMEmtprovider.h
 
     Description:
     
     Author:      ke wen (kew@IIME.com.cn)
     Date Time:   12/4/2005 12:33:03 PM

     Copyright (C) 2000-2005 Beijing Chinese Star Cyber Technology Ltd.
     All rights reserved.

 **************************************************************************/

#ifndef __IIMEMTPROVIDER_H__
#define __IIMEMTPROVIDER_H__

typedef struct tagIIMEMTProvider
{
    DECLARE_PROVIDER_INTERFACE()

    IIME_INT  m_idMethod;
    const void*  pdbMisc;

    IIME_IMM_CONTENT m_immContent;

    int      m_MagicOne;
    IIME_UINT  m_nPreKey;		/* the low 16 bit is normal key value, the hight 16 bit is vowel without tone*/
    IIME_INT   m_nCount;
	IIME_BOOL  m_bFirstChar;
	IIME_BOOL  m_bCapsLock;
	#ifdef KMX_LDB_HANGUL
    IIME_WCHAR m_wPreComp;
	#endif
	/* output*/
	IIME_DWORD	m_dwUpdateFlag;

	IIME_WCHAR	m_wUnderline;
	IIME_WCHAR	m_wCommit;

	IIME_WCHAR	m_wPreChar;

    CALLBACK_GET_CONTEXT    m_pfnGetContext;

    void*       dbobj;
    int         did;
}IIMEMTProvider;

Provider_Com * IIMEMTProvider_Create(PIIME_IMM_PARAMS  pImmParams,
								   PIIME_USER_PARAMS pUserParam1,
								   PIIME_USER_PARAMS pUserParam2,
							       const void*	  pdbMisc,
						           const void*	  pdbMO,
                                   const IIME_MULTI_STRING_TABLE* pMulti_string_table,
								   IIME_INT idMethod);

void IIMEMTProvider_Delete(Provider_Com *pThis);

#endif /*__IIMEMTPROVIDER_H__*/
