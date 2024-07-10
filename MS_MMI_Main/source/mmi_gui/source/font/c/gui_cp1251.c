#ifdef IM_RUSSIAN_SUPPORT
/*****************************************************************************
** File Name:      gui_cp1251.c                                              *
** Author:                                                                   *
** Date:           06/07/2011                                                *
** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe conversion function         *
**                 from CP1251 to unicode                                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/07/2011    James.Zhang         Create                                  *
******************************************************************************/

#ifndef _GUI_CP1251_C_
#define _GUI_CP1251_C_

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
 * CP1251
 */
LOCAL const uint16 s_cp1251_2uni[128] = 
{
  /* 0x80 */
  0x0402, 0x0403, 0x201a, 0x0453, 0x201e, 0x2026, 0x2020, 0x2021,
  0x20ac, 0x2030, 0x0409, 0x2039, 0x040a, 0x040c, 0x040b, 0x040f,
  /* 0x90 */
  0x0452, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014,
  0xfffd, 0x2122, 0x0459, 0x203a, 0x045a, 0x045c, 0x045b, 0x045f,
  /* 0xa0 */
  0x00a0, 0x040e, 0x045e, 0x0408, 0x00a4, 0x0490, 0x00a6, 0x00a7,
  0x0401, 0x00a9, 0x0404, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x0407,
  /* 0xb0 */
  0x00b0, 0x00b1, 0x0406, 0x0456, 0x0491, 0x00b5, 0x00b6, 0x00b7,
  0x0451, 0x2116, 0x0454, 0x00bb, 0x0458, 0x0405, 0x0455, 0x0457,
  /* 0xc0 */
  0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417,
  0x0418, 0x0419, 0x041a, 0x041b, 0x041c, 0x041d, 0x041e, 0x041f,
  /* 0xd0 */
  0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427,
  0x0428, 0x0429, 0x042a, 0x042b, 0x042c, 0x042d, 0x042e, 0x042f,
  /* 0xe0 */
  0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437,
  0x0438, 0x0439, 0x043a, 0x043b, 0x043c, 0x043d, 0x043e, 0x043f,
  /* 0xf0 */
  0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447,
  0x0448, 0x0449, 0x044a, 0x044b, 0x044c, 0x044d, 0x044e, 0x044f,
};

LOCAL const uint8 s_cp1251_page00[32] = 
{
  0xa0, 0x00, 0x00, 0x00, 0xa4, 0x00, 0xa6, 0xa7, /* 0xa0-0xa7 */
  0x00, 0xa9, 0x00, 0xab, 0xac, 0xad, 0xae, 0x00, /* 0xa8-0xaf */
  0xb0, 0xb1, 0x00, 0x00, 0x00, 0xb5, 0xb6, 0xb7, /* 0xb0-0xb7 */
  0x00, 0x00, 0x00, 0xbb, 0x00, 0x00, 0x00, 0x00, /* 0xb8-0xbf */
};
LOCAL const uint8 s_cp1251_page04[152] = 
{
  0x00, 0xa8, 0x80, 0x81, 0xaa, 0xbd, 0xb2, 0xaf, /* 0x00-0x07 */
  0xa3, 0x8a, 0x8c, 0x8e, 0x8d, 0x00, 0xa1, 0x8f, /* 0x08-0x0f */
  0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, /* 0x10-0x17 */
  0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, /* 0x18-0x1f */
  0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, /* 0x20-0x27 */
  0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, /* 0x28-0x2f */
  0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, /* 0x30-0x37 */
  0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, /* 0x38-0x3f */
  0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, /* 0x40-0x47 */
  0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff, /* 0x48-0x4f */
  0x00, 0xb8, 0x90, 0x83, 0xba, 0xbe, 0xb3, 0xbf, /* 0x50-0x57 */
  0xbc, 0x9a, 0x9c, 0x9e, 0x9d, 0x00, 0xa2, 0x9f, /* 0x58-0x5f */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x60-0x67 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x68-0x6f */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x70-0x77 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x78-0x7f */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x80-0x87 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x88-0x8f */
  0xa5, 0xb4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x90-0x97 */
};
LOCAL const uint8 s_cp1251_page20[48] = 
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
// 	Description : cp1251 to wchar
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CP1251ToWchar(
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
        wchar w_char = s_cp1251_2uni[ch-0x80];
        if ( w_char != 0xfffd )
        {
            *wch_ptr = w_char;
            return TRUE;
        }
    }

    return FALSE;
}


/*****************************************************************************/
// 	Description : wchar to cp1251 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN WcharToCP1251(
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
    else if (w_char >= 0x00a0 && w_char < 0x00c0)
    {
        ch = s_cp1251_page00[w_char-0x00a0];
    }
    else if (w_char >= 0x0400 && w_char < 0x0498)
    {
        ch = s_cp1251_page04[w_char-0x0400];
    }
    else if (w_char >= 0x2010 && w_char < 0x2040)
    {
        ch = s_cp1251_page20[w_char-0x2010];
    }
    else if (w_char == 0x20ac)
    {
        ch = 0x88;
    }
    else if (w_char == 0x2116)
    {
        ch = 0xb9;
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
// 	Description : cp1251 to wstr
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
PUBLIC uint32 GUI_CP1251ToWstr(//return the number of UCS2 code that have been converted.
                               wchar       *wstr_ptr, 	   //destination: string pointer to UCS2 string.
                               const uint8 *cp1251_ptr,    //source: string pointer to cp1251 string
                               uint32      str_len         //the length to convert from cp1251 to UCS2.
                               )
{
    uint32 i = 0;
    uint32 len = str_len;
    
    if ( PNULL == cp1251_ptr || PNULL == wstr_ptr || 0 == str_len )
    {
        return 0;
    }

    for ( i = 0; i < str_len; i++ )
    {
        if ( !CP1251ToWchar( cp1251_ptr[i], &wstr_ptr[i]) )
        {
            len = i;
            break;
        }
    }

    return len;
}

/*****************************************************************************/
// 	Description : wstr to cp1251 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
PUBLIC uint32 GUI_WstrToCP1251(//return the number of UCS2 code that have been converted.
                               uint8 *cp1251_ptr,        //destination: string pointer to cp1251 string
                               const wchar *wstr_ptr,    //source: string pointer to UCS2 string.
                               uint32 wstr_len           //the length to convert from UCS2 to cp1251.
                               )
{
    uint32 i = 0;
    uint32 len = wstr_len;
    
    if ( PNULL == cp1251_ptr || PNULL == wstr_ptr || 0 == wstr_len )
    {
        return 0;
    }
   
    for ( i = 0; i < wstr_len; i++ )
    {
        if ( !WcharToCP1251( &cp1251_ptr[i], wstr_ptr[i] ) )
        {
            len = i;
            break;
        }
    }
    
    return len;
}

PUBLIC uint32 GUI_CP1251ToWstrWithEndPos(//return the number of UCS2 code that have been converted.
									  wchar *wstr_ptr, 	     //destination: string pointer to UCS2 string.
									  const uint8 *cp1251_ptr,  //source: string pointer to CP1251 string
									  uint32 str_len,        //the length to convert from CP1251 to UCS2.
									  uint32 *src_end_pos    //the end pos of src string
									  )
{
    uint32 ret = 0;
    if (PNULL == wstr_ptr || PNULL == cp1251_ptr || 0 == str_len) {
        return 0;
    }

    ret = GUI_CP1251ToWstr(wstr_ptr, cp1251_ptr, str_len);
    if (NULL != src_end_pos) {
        *src_end_pos = (uint32)cp1251_ptr + ret;
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


