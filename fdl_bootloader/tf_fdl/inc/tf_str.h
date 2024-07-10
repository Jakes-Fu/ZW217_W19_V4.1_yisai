/******************************************************************************
 ** File Name:      TF_main.h                                                *
 ** Author:           tao.feng                                                 *
 ** DATE:            20/12/2010                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the interfaces of TF download process  *
            *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE               NAME             DESCRIPTION                               *
 ** 20/12/2010     tao.feng         Create.                                   *
 **                                                                           *
 ******************************************************************************/


#ifndef _TF_STR__H_
#define _TF_STR__H_


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "integer.h"
#include "ff.h"
#include "cmddef.h"


/**---------------------------------------------------------------------------*
 **                         Macro Defination                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         	Export Func                                   *
 **---------------------------------------------------------------------------*/



/*****************************************************************************/
//  Discription:TF  load string compare
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC int TF_Load_wstrcmp(
                             const TCHAR * string1,
                             const TCHAR * string2
                             );
                             
/*****************************************************************************/
//  Discription:TF  load defined count string compare
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC int32 TF_Load_Wstrncmp(const TCHAR* pString1, const TCHAR* pString2, size_t nCount);


/*****************************************************************************/
//  Discription:TF  load string copy
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC TCHAR* TF_Load_Wstrcpy( 
                                TCHAR* dst,
                                const TCHAR* src
                                );
                                
/*****************************************************************************/
//  Discription:TF  load cal string length
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC size_t TF_Load_Wstrlen( const TCHAR* str );


/*****************************************************************************/
//  Discription:TF  find string first not equal position
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC TCHAR* TF_Load_Wstrstr(const TCHAR* pString1, const TCHAR* pString2);

/*****************************************************************************/
//  Discription:TF  find string append
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC TCHAR* TF_Load_Wstrcat(TCHAR* pDest, const TCHAR* pSource);

/*****************************************************************************/
//  Discription:TF_Load_Wstrtolower
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void TF_Load_Wstrtolower(TCHAR *pSrc);


/*****************************************************************************/
//  Discription:memcpy
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void  TF_Memcpy(void *dst, const void *src, int size);

/*****************************************************************************/
//  Discription:memset
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void   TF_Memset(void *dst, int padding, int size);



#endif //_TF_STR__H_