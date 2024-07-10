/*****************************************************************************
** File Name:      gui_cp1252.c                                              *
** Author:                                                                   *
** Date:           10/15/2010                                                *
** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe conversion function         *
**                 from CP1252 to unicode                                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/15/2010    James.Zhang         Create                                  *
******************************************************************************/

#ifndef _GUI_CP1252_C_
#define _GUI_CP1252_C_

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
 * CP1252
 */
LOCAL const uint16 s_cp1252_2uni[32] = 
{
  /* 0x80 */
  0x20ac, 0xfffd, 0x201a, 0x0192, 0x201e, 0x2026, 0x2020, 0x2021,
  0x02c6, 0x2030, 0x0160, 0x2039, 0x0152, 0xfffd, 0x017d, 0xfffd,
  /* 0x90 */
  0xfffd, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014,
  0x02dc, 0x2122, 0x0161, 0x203a, 0x0153, 0xfffd, 0x017e, 0x0178,
};

LOCAL const uint8 s_cp1252_page01[72] = 
{
  0x00, 0x00, 0x8c, 0x9c, 0x00, 0x00, 0x00, 0x00, /* 0x50-0x57 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x58-0x5f */
  0x8a, 0x9a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x60-0x67 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x68-0x6f */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x70-0x77 */
  0x9f, 0x00, 0x00, 0x00, 0x00, 0x8e, 0x9e, 0x00, /* 0x78-0x7f */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x80-0x87 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x88-0x8f */
  0x00, 0x00, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x90-0x97 */
};

LOCAL const uint8 s_cp1252_page02[32] = 
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x00, /* 0xc0-0xc7 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xc8-0xcf */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xd0-0xd7 */
  0x00, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, /* 0xd8-0xdf */
};

LOCAL const uint8 s_cp1252_page20[48] = 
{
  0x00, 0x00, 0x00, 0x96, 0x97, 0x00, 0x00, 0x00, /* 0x10-0x17 */
  0x91, 0x92, 0x82, 0x00, 0x93, 0x94, 0x84, 0x00, /* 0x18-0x1f */
  0x86, 0x87, 0x95, 0x00, 0x00, 0x00, 0x85, 0x00, /* 0x20-0x27 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x28-0x2f */
  0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x30-0x37 */
  0x00, 0x8b, 0x9b, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x38-0x3f */
};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : cp1252 to wchar
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CP1252ToWchar(
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

    if ( ch < 0x80 
        || ch >= 0xa0 ) 
    {
        *wch_ptr = ch;
        return TRUE;
    }
    else 
    {
        wchar w_char = s_cp1252_2uni[ch-0x80];
        if ( w_char != 0xfffd )
        {
            *wch_ptr = w_char;
            return TRUE;
        }
    }

    return FALSE;
}


/*****************************************************************************/
// 	Description : wchar to cp1252 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN WcharToCP1252(
                            uint8 *ch_ptr, //out
                            wchar w_char
                            )
{
    uint8 ch = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT( PNULL != ch_ptr );  /*assert verified*/
    if ( PNULL == ch_ptr )
    {
        return FALSE;
    }

    if ( w_char < 0x80 ) 
    {
        *ch_ptr = w_char;
        return TRUE;
    }
    else if (w_char >= 0x00a0 && w_char < 0x0100)
    {
        ch = w_char;
    }
    else if (w_char >= 0x0150 && w_char < 0x0198)
    {
        ch = s_cp1252_page01[w_char-0x0150];
    }
    else if (w_char >= 0x02c0 && w_char < 0x02e0)
    {
        ch = s_cp1252_page02[w_char-0x02c0];
    }
    else if (w_char >= 0x2010 && w_char < 0x2040)
    {
        ch = s_cp1252_page20[w_char-0x2010];
    }
    else if (w_char == 0x20ac)
    {
        ch = 0x80;
    }
    else if (w_char == 0x2122)
    {
        ch = 0x99;
    }

    if (ch != 0) 
    {
        *ch_ptr = ch;
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
// 	Description : cp1252 to wstr
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
PUBLIC uint32 GUI_CP1252ToWstr(//return the number of UCS2 code that have been converted.
                               wchar       *wstr_ptr, 	   //destination: string pointer to UCS2 string.
                               const uint8 *cp1252_ptr,    //source: string pointer to cp1252 string
                               uint32      str_len         //the length to convert from cp1252 to UCS2.
                               )
{
    uint32 i = 0;
    uint32 len = str_len;
    
    if ( PNULL == cp1252_ptr || PNULL == wstr_ptr || 0 == str_len )
    {
        return 0;
    }


    for ( i = 0; i < str_len; i++ )
    {
        if ( !CP1252ToWchar( cp1252_ptr[i], &wstr_ptr[i]) )
        {
            len = i;
            break;
        }
    }

    return len;
}

/*****************************************************************************/
// 	Description : wstr to cp1252 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
PUBLIC uint32 GUI_WstrToCP1252(//return the number of UCS2 code that have been converted.
                               uint8 *cp1252_ptr,        //destination: string pointer to cp1252 string
                               const wchar *wstr_ptr,    //source: string pointer to UCS2 string.
                               uint32 wstr_len           //the length to convert from UCS2 to cp1252.
                               )
{
    uint32 i = 0;
    uint32 len = wstr_len;
    
    if ( PNULL == cp1252_ptr || PNULL == wstr_ptr || 0 == wstr_len )
    {
        return 0;
    }
   
    for ( i = 0; i < wstr_len; i++ )
    {
        if ( !WcharToCP1252( &cp1252_ptr[i], wstr_ptr[i] ) )
        {
            len = i;
            break;
        }
    }
    
    return len;
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


#endif
