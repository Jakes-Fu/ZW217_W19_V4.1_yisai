#ifdef IM_THAI_SUPPORT
/*****************************************************************************
** File Name:      gui_cp874.c                                              *
** Author:                                                                   *
** Date:           06/07/2011                                                *
** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe conversion function         *
**                 from CP874 to unicode                                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/07/2011    James.Zhang         Create                                  *
******************************************************************************/

#ifndef _GUI_CP874_C_
#define _GUI_CP874_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "caf.h"
#include "sci_codec.h"
#include "gui_ucs2b_converter.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
/*
 * CP874
 */
LOCAL const uint16 s_cp874_2uni[128] = 
{
  /* 0x80 */
  0x20ac, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0x2026, 0xfffd, 0xfffd,
  0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd,
  /* 0x90 */
  0xfffd, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014,
  0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0xfffd,
  /* 0xa0 */
  0x00a0, 0x0e01, 0x0e02, 0x0e03, 0x0e04, 0x0e05, 0x0e06, 0x0e07,
  0x0e08, 0x0e09, 0x0e0a, 0x0e0b, 0x0e0c, 0x0e0d, 0x0e0e, 0x0e0f,
  /* 0xb0 */
  0x0e10, 0x0e11, 0x0e12, 0x0e13, 0x0e14, 0x0e15, 0x0e16, 0x0e17,
  0x0e18, 0x0e19, 0x0e1a, 0x0e1b, 0x0e1c, 0x0e1d, 0x0e1e, 0x0e1f,
  /* 0xc0 */
  0x0e20, 0x0e21, 0x0e22, 0x0e23, 0x0e24, 0x0e25, 0x0e26, 0x0e27,
  0x0e28, 0x0e29, 0x0e2a, 0x0e2b, 0x0e2c, 0x0e2d, 0x0e2e, 0x0e2f,
  /* 0xd0 */
  0x0e30, 0x0e31, 0x0e32, 0x0e33, 0x0e34, 0x0e35, 0x0e36, 0x0e37,
  0x0e38, 0x0e39, 0x0e3a, 0xfffd, 0xfffd, 0xfffd, 0xfffd, 0x0e3f,
  /* 0xe0 */
  0x0e40, 0x0e41, 0x0e42, 0x0e43, 0x0e44, 0x0e45, 0x0e46, 0x0e47,
  0x0e48, 0x0e49, 0x0e4a, 0x0e4b, 0x0e4c, 0x0e4d, 0x0e4e, 0x0e4f,
  /* 0xf0 */
  0x0e50, 0x0e51, 0x0e52, 0x0e53, 0x0e54, 0x0e55, 0x0e56, 0x0e57,
  0x0e58, 0x0e59, 0x0e5a, 0x0e5b, 0xfffd, 0xfffd, 0xfffd, 0xfffd,
};

LOCAL const uint8 s_cp874_page0e[96] = 
{
  0x00, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, /* 0x00-0x07 */
  0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, /* 0x08-0x0f */
  0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, /* 0x10-0x17 */
  0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, /* 0x18-0x1f */
  0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, /* 0x20-0x27 */
  0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, /* 0x28-0x2f */
  0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, /* 0x30-0x37 */
  0xd8, 0xd9, 0xda, 0x00, 0x00, 0x00, 0x00, 0xdf, /* 0x38-0x3f */
  0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, /* 0x40-0x47 */
  0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, /* 0x48-0x4f */
  0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, /* 0x50-0x57 */
  0xf8, 0xf9, 0xfa, 0xfb, 0x00, 0x00, 0x00, 0x00, /* 0x58-0x5f */
};
LOCAL const uint8 s_cp874_page20[24] = 
{
  0x00, 0x00, 0x00, 0x96, 0x97, 0x00, 0x00, 0x00, /* 0x10-0x17 */
  0x91, 0x92, 0x00, 0x00, 0x93, 0x94, 0x00, 0x00, /* 0x18-0x1f */
  0x00, 0x00, 0x95, 0x00, 0x00, 0x00, 0x85, 0x00, /* 0x20-0x27 */
};
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : cp874 to wchar
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CP874ToWchar(
                            uint8 ch,
                            wchar* wch_ptr //out
                            )
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT( PNULL != wch_ptr ); /*assert verified*/
    if ( PNULL == wch_ptr )
    {
        return FALSE;
    }

    if ( ch < 0x80 ) 
    {
        *wch_ptr = ch;
        return TRUE;
    }
    else 
    {
        wchar w_char = s_cp874_2uni[ch-0x80];
        if ( w_char != 0xfffd )
        {
            *wch_ptr = w_char;
            return TRUE;
        }
    }

    return FALSE;
}


/*****************************************************************************/
// 	Description : wchar to cp874 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN WcharToCP874(
                            uint8 *ch_ptr, //out
                            wchar w_char
                            )
{
    uint8 ch = 0;

    SCI_ASSERT( PNULL != ch_ptr );  /*assert verified*/

    if ( w_char < 0x80 ) 
    {
        *ch_ptr = w_char;
        return TRUE;
    }
    else if (w_char == 0x00a0 )
    {
        ch =  0xa0;
    }
    else if (w_char >= 0x0e00 && w_char < 0x0e60)
    {
        ch = s_cp874_page0e[w_char-0x0e00];
    }
    else if (w_char >= 0x2010 && w_char < 0x2028)
    {
        ch = s_cp874_page20[w_char-0x2010];
    }
    else if (w_char == 0x20ac)
    {
        ch = 0x80;
    }
    

    if (ch != 0) 
    {
        *ch_ptr = ch;
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
// 	Description : cp874 to wstr
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
PUBLIC uint32 GUI_CP874ToWstr(//return the number of UCS2 code that have been converted.
                               wchar       *wstr_ptr, 	   //destination: string pointer to UCS2 string.
                               const uint8 *cp874_ptr,    //source: string pointer to cp874 string
                               uint32      str_len         //the length to convert from cp874 to UCS2.
                               )
{
    uint32 i = 0;
    uint32 len = str_len;
    
    if ( PNULL == cp874_ptr || PNULL == wstr_ptr || 0 == str_len )
    {
        return 0;
    }

    for ( i = 0; i < str_len; i++ )
    {
        if ( !CP874ToWchar( cp874_ptr[i], &wstr_ptr[i]) )
        {
            len = i;
            break;
        }
    }

    return len;
}

/*****************************************************************************/
// 	Description : wstr to cp874 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
PUBLIC uint32 GUI_WstrToCP874(//return the number of UCS2 code that have been converted.
                               uint8 *cp874_ptr,        //destination: string pointer to cp874 string
                               const wchar *wstr_ptr,    //source: string pointer to UCS2 string.
                               uint32 wstr_len           //the length to convert from UCS2 to cp874.
                               )
{
    uint32 i = 0;
    uint32 len = wstr_len;
    
    if ( PNULL == cp874_ptr || PNULL == wstr_ptr || 0 == wstr_len )
    {
        return 0;
    }
   
    for ( i = 0; i < wstr_len; i++ )
    {
        if ( !WcharToCP874( &cp874_ptr[i], wstr_ptr[i] ) )
        {
            len = i;
            break;
        }
    }
    
    return len;
}

PUBLIC uint32 GUI_CP874ToWstrWithEndPos(//return the number of UCS2 code that have been converted.
									  wchar *wstr_ptr, 	     //destination: string pointer to UCS2 string.
									  const uint8 *cp874_ptr,  //source: string pointer to CP874 string
									  uint32 str_len,        //the length to convert from CP874 to UCS2.
									  uint32 *src_end_pos    //the end pos of src string
									  )
{
    uint32 ret = 0;
    if (PNULL == wstr_ptr || PNULL == cp874_ptr || 0 == str_len) {
        return 0;
    }

    ret = GUI_CP874ToWstr(wstr_ptr, cp874_ptr, str_len);
    if (NULL != src_end_pos) {
        *src_end_pos = (uint32)cp874_ptr + ret;
    }

    return ret;
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


#endif
#endif

