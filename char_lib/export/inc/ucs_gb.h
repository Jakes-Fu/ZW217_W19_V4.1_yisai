/***************************************************************************
 ** File Name:      ucs_gb.h                                               *
 ** Author:         Fancier.Fan                                            *
 ** Date:           22/03/2006                                             *
 ** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved       *
 ** Description:    ucs2gb, gb2ucs convert API
****************************************************************************
 
****************************************************************************
 **                        Edit History                                    *
 ** -----------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                            *
 ** 19/06/2002     Xiangxin Gu        Create.                              *
****************************************************************************/

#ifndef UCS_GB_H
#define UCS_GB_H

#include "sci_types.h"

/**------------------------------------------------------------------------*
 **                         Compiler Flag                                  *
 **------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

unsigned short ucs2str_to_gb2312str(// return the length of gb2312_ptr
    unsigned char  *ucs_ptr,        // the ucs2 stringcode that need to be converted.
    unsigned short   length,         //the unicode length      //@steven  cr8219 2004 03 29 , see cr8040
    unsigned char  *gb2312_ptr);     // store the gb2312 string code
    
#ifdef ZDT_APP_SUPPORT
unsigned short ucs2str_to_gb2312strExt(// return the length of gb2312_ptr
    unsigned char  *ucs_ptr,        // the ucs2 stringcode that need to be converted.
    unsigned short   length,         //the unicode length      //@steven  cr8219 2004 03 29 , see cr8040
    unsigned char  *gb2312_ptr,
    unsigned char is_bigendi);     // store the gb2312 string code
#endif

// Input Param:
//      gb2312      : the string that need to be converted
//      gb2312_size : the size of the gb2312 string.
//      max_ucs2    : the max buffer of ucs2.
// Output Param     :
//      ucs2        : store the convert data
// Return :
//      the count we converted.
unsigned long  gb2312_to_ucs2(              // return the UCS2 count
    const unsigned  char *  gb2312,         // the pointer that contain the gb2312 string
    unsigned int            gb2312_size,    // the bytes count that the gb2312 string length
    unsigned short       *  ucs2,           // the buffer that store the converted ucs2
    unsigned long           max_ucs2);       // max ucs2 buffer size

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  /* UCS_GB_H */

