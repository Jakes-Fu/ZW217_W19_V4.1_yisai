/*****************************************************************************
** File Name:      gui_ucs2gb.c                                              *
** Author:                                                                   *
** Date:           06/22/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe conversion function         *
**                 from UCS2 string to GB2312 string                         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/22/2004    Jim.Zhang           Create                                  *
******************************************************************************/

#ifndef _GUI_UCS2GB_C_
#define _GUI_UCS2GB_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "gui_unicode.h"
#include "caf.h"
#include "sci_codec.h"

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


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

//extern const unsigned short number_of_ucs2_4E00_9FFF;
// extern const unsigned char sci_ucs2_mirror_4E00_9FFF[ 0xA0 - 0x4E ][256 >> 3];
// extern const unsigned short sci_ucs2_index_table_4E00_9FFF[0xA0 - 0x4E][4];
// extern const unsigned char sci_number_of_bit_1[256];

#ifdef GBK_SUPPORT 
extern uint32 GUI_WstrToGBK( // return the length of gbk_ptr
                            uint8        *gbk_ptr,   // store the gbk string code
                            const wchar  *wstr_ptr,  // the unicode stringcode that need to be converted.
                            uint32       wstr_len    //the unicode length
                            );
#endif

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/*****************************************************************************/
// 	Description : get an ucs2 code 's offset in the conversion table..
//	Global resource dependence : none
//  Author: Jim.zhang
//	Note: 
/*****************************************************************************/
PUBLIC int16 GUI_GetUCSOffset(			//return the offset in GB2312 table.
                              uint16 ucs2_code	//an ucs2 code. 0x4E00 <= ucs2 < 0xA000 
                              ) 
{
	uint8 *mirror_ptr = PNULL;
	uint8 chr = 0;
	uint16 offset = 0;
	uint16 page = 0;
	uint16 tmp = 0;

	page = (uint16)((ucs2_code >> 8) - 0x4E);
	ucs2_code &= 0xFF;

	tmp        = (uint16)(ucs2_code >> 6); // now 0 <= tmp < 4 
	offset     = sci_ucs2_index_table_4E00_9FFF[page][ tmp ];  
	mirror_ptr = (uint8 *)&sci_ucs2_mirror_4E00_9FFF[page][ tmp << 3 ]; //[0, 8, 16, 24]

	tmp = (uint16)(ucs2_code & 0x3F); 		// mod 64  

	while( tmp >= 8 )
	{
		offset += sci_number_of_bit_1[ *mirror_ptr ];
		mirror_ptr++;
		tmp -= 8;
	}

	chr = *mirror_ptr;
	if (chr & (0x1 << tmp)) // Ok,this ucs2 can be covert to GB2312.
	{	
		while( tmp ) 
		{ 
			if (chr & 0x1)
			{
				offset++;
			}
			chr >>= 1;
			tmp--;
		}
		
		return offset;
	}
	else	//It can be conerted to GB2312 code.
	{
		return (-1);
	}
}

/*****************************************************************************/
// 	Description : wstr to gb
//	Global resource dependence : none
//  Author: Jim.zhang
//	Note: 
/*****************************************************************************/
PUBLIC uint32 GUI_WstrToGB(						// return the length of gb2312_ptr
                           uint8  *gb_ptr,			// store the gb2312 string code
                           const wchar  *wstr_ptr,  // the ucs2 stringcode that need to be converted.
                           uint32   wstr_len         //the unicode length
                           )     
{ 
#ifdef GBK_SUPPORT  
    return GUI_WstrToGBK( gb_ptr, wstr_ptr, wstr_len );
#else

    int16    offset = 0;
    uint16   ucs2_code = 0;
    uint16   gb_code = 0xA3BF; 
    uint32   gb_length = 0;
    uint32   i = 0;

    if ( PNULL == gb_ptr || PNULL == wstr_ptr || 0 == wstr_len )
    {
        return 0;
    }
  	
    for ( ; i < wstr_len; i++ )
    {
        gb_code = 0xA3BF; 

        ucs2_code = wstr_ptr[i];
  
        if (GUI_IsAsciiCode(ucs2_code))
        {
            *gb_ptr++ = (uint8)ucs2_code;
            gb_length++;
        }
        /*
        else if (GUI_IsLatinUnicode(ucs2))// || GUI_IsCyrillicUnicode(ucs2))//other latin unicode
        {
            *gb_ptr++ = (uint8)((ucs2 & 0xFF00) >> 8);
            *gb_ptr++ = (uint8)(ucs2 & 0x00FF);

            gb_length += 2;
        }*/
       
        else
        {
#if defined (IM_SIMP_CHINESE_SUPPORT) || defined (MMI_DISPLAY_SIMP_CHINESE_SUPPORT)
            if( (0x4E00 <= ucs2_code) && (0xA000 > ucs2_code) )
            {
                offset = GUI_GetUCSOffset( ucs2_code );
                if (offset >= 0)
                {
                    gb_code = sci_ucs2_to_gb2312_table[ offset ];
                }
            }
            else
            {
                // Is chinese symbol ? try search another table
                uint16 gb2312_table_size = 94 * 16;
                
                for( offset = 0; offset < gb2312_table_size; offset++ )
                {
                    if( ucs2_code == sci_gb2312_to_ucs2_table[ offset ] )
                    {
                        gb_code = (uint16)(offset / 94 + 0xA0); 
                        gb_code = (uint16)((gb_code << 8) + (offset % 94 + 0xA1));
                        break;
                    }
                }
            }
#endif  
            *gb_ptr++ = (uint8)( gb_code >> 8);
            *gb_ptr++ = (uint8)( gb_code );
            gb_length +=  2;
        }
      
    }
  
    return gb_length;
#endif
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


#endif	//_GUI_UCS2GB_C_
