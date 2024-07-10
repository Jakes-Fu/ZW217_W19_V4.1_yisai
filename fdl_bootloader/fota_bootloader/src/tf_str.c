/******************************************************************************
 ** File Name:    TF_load.c                                               *
 ** Author:         tao.feng                                              *
 ** DATE:           21/01/2011                                                *
 ** Copyright:     2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE               NAME              DESCRIPTION                               *
 ** 21/01/2011     tao.feng          Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
 
 /**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "tf_str.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Macro  Definition                                     *
 **---------------------------------------------------------------------------*/

	  
/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Global  Variables                                  *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Extern  Func                                  *
 **---------------------------------------------------------------------------*/                                     



/**---------------------------------------------------------------------------*
 **                          Functions    Definition                            *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription:TF  load string compare
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
 int TF_Load_wstrcmp(
                             const TCHAR * string1,
                             const TCHAR * string2
                             )
{
	int ret = 0 ;

	if( string1 == PNULL && string2 == PNULL )
	{
		return 0;
	}
	else if( string1 == PNULL && string2 != PNULL )
	{
		return -1;
	}
	else if( string1 != PNULL && string2 == PNULL )
	{
		return 1;
	}
	else
	{
		/*lint -save -e613 */
		ret = *(TCHAR *)string1 - *(TCHAR *)string2;/*lint !e613 */

		while( (!ret) && *string2 && *string1)/*lint !e613 */
		{
			++string1;/*lint !e613 */
			++string2;
			ret = *(TCHAR *)string1 - *(TCHAR *)string2;
		}
		/*lint -restore */
		return ( ret > 0 ? 1 : ( ret < 0 ? -1 : 0 ) ); 
	}
}


/*****************************************************************************/
//  Discription:TF  load defined count string compare
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
 int32 TF_Load_Wstrncmp(const TCHAR* pString1, const TCHAR* pString2, size_t nCount)
{
        if(!nCount)
        {
            return 0;   // return 0 if pString1 or pString2 == NULL.
        }

        if(!pString1 || !pString2)
        {
            return 1;
        }

        while( --nCount && *pString1 && *pString1 == *pString2)
        {
            pString1++;
            pString2++;
        }

        return ((int32)(*pString1 - *pString2));

}


/*****************************************************************************/
//  Discription:TF  load string copy
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
 TCHAR* TF_Load_Wstrcpy( 
                                TCHAR* dst,
                                const TCHAR* src
                                )
{
    TCHAR* cp = dst;

    if( PNULL == src || PNULL == dst )
    {
        return dst;
    }

    while(PNULL != (*cp++ = *src++) )
        ; /* copy src to dst */

    return( dst );

}

/*****************************************************************************/
//  Discription:TF  load cal string length
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
 size_t TF_Load_Wstrlen( const TCHAR* str )
{
	const TCHAR* ustr = str;

	if( PNULL == str )
	{
		return 0;
	}

	while( *ustr++ );

	return( (size_t)(ustr - str - 1) );
}


/*****************************************************************************/
//  Discription:TF  find string first not equal position
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
 TCHAR* TF_Load_Wstrstr(const TCHAR* pString1, const TCHAR* pString2)
{
	TCHAR *pPos = (TCHAR *) pString1; 
	TCHAR *pStr2 = (TCHAR *) pString2; 
	uint32 nLen = 0;

	nLen = TF_Load_Wstrlen(pString2);
	if(0 == nLen)
	{
		return((TCHAR *)pString1);
	}

	while (*pPos)
	{
		if(0 == TF_Load_Wstrncmp(pPos, pStr2, nLen))
		{
			return(pPos);
		}
		pPos++; 
	}

	return (NULL);
}


/*****************************************************************************/
//  Discription:TF  find string append
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
 TCHAR* TF_Load_Wstrcat(TCHAR* pDest, const TCHAR* pSource)
{
	TCHAR* pCp = pDest;
	// return 0 if pDest or pSource == NULL.

	if(!pDest || !pSource)
	{
		return NULL;    //find the end of pDest
	}
	while( *pCp )
	{
		pCp++;    //copy pSource to end of pDest
	}
	while( *pSource )
	{
		*pCp++ = *pSource++;
	}

	*pCp = 0; 
	return pDest;
}


/*****************************************************************************/
//  Discription:TF_Load_Wstrtolower
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
 void TF_Load_Wstrtolower(TCHAR *pSrc)
{
	if(pSrc)
	{
		while(*pSrc)
		{
			if(*pSrc >= 'A' && *pSrc <= 'Z')
				*pSrc += 'a'-'A';
			pSrc++;
		}
	}
}


/*****************************************************************************/
//  Discription:memcpy
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
/*PUBLIC void  TF_Memcpy(void *dst, const void *src, int size)
{
    register char *memcpy_src;
    register char *memcpy_dst;    
    register int  memcpy_size;
    
    memcpy_dst   = (char *)dst;
    memcpy_src   = (char *)src;
    memcpy_size  = size;
    
    while(0 < memcpy_size)
    {
        *(unsigned long *)memcpy_dst  = *(unsigned long *)memcpy_src;
        memcpy_dst   += 4;
        memcpy_src   += 4;
        memcpy_size  -= 4;
    } 
    
}*/

/*****************************************************************************/
//  Discription:memset
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
/*PUBLIC void TF_Memset(void *dst, int padding, int size)
{
    register char *memset_dst;
    register int  memset_size;
    register unsigned long memset_pad;    
   
    memset_dst   = dst;
    memset_pad   = padding;
    memset_size  = size;
    memset_pad = (memset_pad << 8)  | memset_pad;
    memset_pad = (memset_pad << 16) | memset_pad;

    while (0 < memset_size)
    {
        *(unsigned long *)memset_dst  = memset_pad;
        memset_size  -= 4;
        memset_dst   += 4;
    }
}*/


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 
