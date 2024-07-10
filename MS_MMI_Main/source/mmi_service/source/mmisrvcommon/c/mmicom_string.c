/****************************************************************************
** File Name:      mmicom_string.c                                         *
** Author:         jian.ma                                                   *
** Date:           03/04/2008                                                *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 4/2008        jian.ma            Create
** 
****************************************************************************/
#define MMICOM_STRING_C


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_app_common_trc.h"
//#ifdef WIN32
//#include "std_header.h"
//#endif
#include "mmicom_string.h"
#include "mmk_type.h"
//#include "guifont.h"
//#include "mmi_modu_main.h"
//#include "mmi_position.h"
//#include "mmifmm_export.h"
#include "mmi_common.h"
//#include "gui_ucs2b_converter.h"
//#include "sci_codec.h"
//#include "guicommon.h"
#include "mmisrv_adapt.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
typedef enum _MMI_ALPHA_TYPE_E
{
    MMI_ALPHA_TYPE_DEFAULT,
    MMI_ALPHA_TYPE_80,
    MMI_ALPHA_TYPE_81,
    MMI_ALPHA_TYPE_82,
    MMI_ALPHA_TYPE_MAX        
} MMI_ALPHA_TYPE_E;
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//the table is for changing from 03.38 alphabet table to ASCII alphabet table  
//add by sunsome.ju
const unsigned char default_to_ascii_table[128] =
{
    0x40,0xA3,0x24,0xA5,0xE8,0xE9,0xF9,0xEC,0xF2,0xC7,0x0A,0xD8,0xF8,0x0D,0xC5,0xE5,
    0x20,0x5F,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0xC6,0xE6,0xDF,0xC9,
    0x20,0x21,0x22,0x23,0xA4,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
    0xA1,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0xC4,0xD6,0xD1,0xDC,0xA7,
    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0xE4,0xF6,0xF1,0xFC,0xE0
};

const unsigned char default_to_ascii_extension_table[128] =
{
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x5e,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x7B,0x7D,0x20,0x20,0x20,0x20,0x20,0x5c,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x5B,0x7e,0x5D,0x20,
    0x7c,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20
};

//the table is for changing from ASCII alphabet table to 03.38 alphabet table  
//add by sunsome.ju
const unsigned char ascii_to_default_table[128] =
{
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0A,0x20,0x20,0x0D,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x21,0x22,0x23,0x02,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
    0x00,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x20,0x20,0x20,0x20,0x11,
    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x20,0x20,0x20,0x20,0x20
};

//add by fengming.huang
const wchar sms_default_to_ascii_table[128] =
{
    0x0040,0x00A3,0x0024,0x00A5,0x00E8,0x00E9,0x00F9,0x00EC,0x00F2,0x00C7,0x000A,0x00D8,0x00F8,0x000D,0x00C5,0x00E5,
    0x0394,0x005F,0x03A6,0x0393,0x039B,0x03A9,0x03A0,0x03A8,0x03A3,0x0398,0x039E,0x001B,0x00C6,0x00E6,0x00DF,0x00C9,
    0x0020,0x0021,0x0022,0x0023,0x00A4,0x0025,0x0026,0x0027,0x0028,0x0029,0x002A,0x002B,0x002C,0x002D,0x002E,0x002F,
    0x0030,0x0031,0x0032,0x0033,0x0034,0x0035,0x0036,0x0037,0x0038,0x0039,0x003A,0x003B,0x003C,0x003D,0x003E,0x003F,
    0x00A1,0x0041,0x0042,0x0043,0x0044,0x0045,0x0046,0x0047,0x0048,0x0049,0x004A,0x004B,0x004C,0x004D,0x004E,0x004F,
    0x0050,0x0051,0x0052,0x0053,0x0054,0x0055,0x0056,0x0057,0x0058,0x0059,0x005A,0x00C4,0x00D6,0x00D1,0x00DC,0x00A7,
    0x00BF,0x0061,0x0062,0x0063,0x0064,0x0065,0x0066,0x0067,0x0068,0x0069,0x006A,0x006B,0x006C,0x006D,0x006E,0x006F,
    0x0070,0x0071,0x0072,0x0073,0x0074,0x0075,0x0076,0x0077,0x0078,0x0079,0x007A,0x00E4,0x00F6,0x00F1,0x00FC,0x00E0
};

const wchar sms_default_to_ascii_extension_table[128] =
{
    0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x0020,0x005E,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x007B,0x007D,0x0020,0x0020,0x0020,0x0020,0x0020,0x005C,
    0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x005B,0x007E,0x005D,0x0020,
    0x007C,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x0020,0x0020,0x20AC,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020
};

//the table is for changing from ASCII alphabet table to 23.38 alphabet table  
//add by fengming.huang
const uint8 sms_ascii_to_default_table[256] =
{
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0A,0x20,0x20,0x0D,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x21,0x22,0x23,0x02,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
    0x00,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x20,0x20,0x20,0x20,0x11,
    0x20,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x40,0x20,0x01,0x24,0x03,0x20,0x5F,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x60,
    0x20,0x20,0x20,0x20,0x5B,0x0E,0x1C,0x09,0x20,0x1F,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x5D,0x20,0x20,0x20,0x20,0x5C,0x20,0x0B,0x20,0x20,0x20,0x5E,0x20,0x20,0x1E,
    0x7F,0x20,0x20,0x20,0x7B,0x0F,0x1D,0x20,0x04,0x05,0x20,0x20,0x07,0x20,0x20,0x20,
    0x20,0x7D,0x08,0x20,0x20,0x20,0x7C,0x20,0x0C,0x06,0x20,0x20,0x7E,0x20,0x20,0x20
};
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         LOCAL Function                                    *
 **---------------------------------------------------------------------------*/
/*****************************************************************************
Description : 81 coding decode
Global resource dependence : none
Author: baokun.yin
Note:
	   If the first octet of the alpha string is set to '81', 
	   then the second octet contains the number of characters in the string, 
	   the third octet contains an 8 bit number defines bits 15 to 8 of a 16 bit base pointer.
	   The fourth and subsequent octets in the string offset value(bit 8 sets 1), or contain a ASCCII(bit 8 sets 0),

*****************************************************************************/
BOOLEAN Decode81String(  			
					   uint16						input_len,//IN
					   uint8	   					*input_ptr,//IN
					   uint16                      output_max_len,//IN
					   uint16						*output_len,//OUT
					   uint8						*output_ptr//OUT
					   );

/*****************************************************************************
Description : 82 coding decode
Global resource dependence : none
Author: baokun.yin
Note:
		If the first octet of the alpha string is set to '82'
		the second octet contains a value indicating the number of characters in the string
		the third and fourth octets contain a 16 bit number defines the complete 16 bit base pointer
		The fifth and subsequent octets is GSM Default Alpha(bit 8 is 0), offset value by base pointer(bit 8 is 1)
*****************************************************************************/
BOOLEAN Decode82String(  			
					   uint16						input_len,//IN
					   uint8	   					*input_ptr,//IN
					   uint16                      output_max_len,//IN
					   uint16						*output_len,//OUT
					   uint8						*output_ptr//OUT
					   );

/*****************************************************************************/
// 	Description : encode Alpha Fields for UCS2
//	Global resource dependence : none
//  Author: Tracy Zhang
//	Note: GSM 11.11 Annex B
/*****************************************************************************/
LOCAL BOOLEAN EncodeUnicode2GSMAlphabet(                       //RETURN:
                                 uint8	input_len,      //IN:
                                 uint8	*input_ptr,     //IN:
                                 uint16	*output_len_ptr,//OUT:
                                 uint8	*output_ptr,     //OUT:
                                 uint16 max_out_len
                                 );

/*****************************************************************************/
// 	Description : get alphabet encode type
//	Global resource dependence : none
//  Author: rui.xu
//	Note: GSM 11.11 Annex B
/*****************************************************************************/
LOCAL MMI_ALPHA_TYPE_E GetAlphabetEncodeType(                        //RETURN:
                                            uint16  *base_addr_ptr, //OUT:
                                            uint16  *defaul_char_num_ptr,//OUT:
                                            uint8   ucs2_len,       //IN:
                                            uint16	*ucs2_ptr       //IN:
                                            );

/*****************************************************************************/
//  Description : check is default ext char
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN IsDefaultExtChar(wchar    ascii_char);

/*****************************************************************************/
//  Description : Opera escape '!' '~' '*' '\'' '(' ')' 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsUnreservedChar(uint8 c);

/*****************************************************************************/
//  Description : Escape URL
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint32 URL_EscapeUrl(uint8 *dst_ptr, uint32 dst_len, const uint8 *url_ptr);

/**---------------------------------------------------------------------------*
 **                         Functions                                         *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : check is default ext char
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: add '�' as a default extension character
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN IsDefaultExtChar(wchar    ascii_char)
{
    BOOLEAN     result = FALSE;

    switch (ascii_char) 
    {
    case '^':
         result = TRUE;
        break;

    case '{':
        result = TRUE;
        break;

    case '}':
        result = TRUE;
        break;

    case '\\':
        result = TRUE;
        break;

    case '[':
        result = TRUE;
        break;

    case '~':
        result = TRUE;
        break;

    case ']':
        result = TRUE;
        break;
        
    case '|':
        result = TRUE;
        break;

    case '�':
        result = TRUE;
        break;
        
    default:
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : operate the ascii character as the character in default ext table 
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_AsciiAsDefaultExt(
                                           uint8    ascii_char,         //IN:
                                           uint8    *default_char_ptr   //OUT:
                                           )
{
    BOOLEAN     result = TRUE;

    switch (ascii_char) 
    {
    case '^':
        *default_char_ptr = 0x14;
        break;

    case '{':
        *default_char_ptr = 0x28;
        break;

    case '}':
        *default_char_ptr = 0x29;
        break;

    case '\\':
        *default_char_ptr = 0x2f;
        break;

    case '[':
        *default_char_ptr = 0x3c;
        break;

    case '~':
        *default_char_ptr = 0x3d;
        break;

    case ']':
        *default_char_ptr = 0x3e;
        break;
        
    case '|':
        *default_char_ptr = 0x40;
        break;
        
    default:
        *default_char_ptr = 0x20;
        result = FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : Change from ASCII alphabet table to 03.38 alphabet table
//  Global resource dependence : none
//  Author: sunsome.ju
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_Ascii2default(                          //RETURN: the length of default string
                         uint8      *ascii_ptr,     //IN:
                         uint8      *default_ptr,   //OUT:
                         uint16     str_len         //IN:
                         )
{
    uint16  default_len = 0;
    int32   i           = 0;
    uint8   temp_char   = 0;
    
    if (PNULL == ascii_ptr || PNULL == default_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_Ascii2default PNULL == ascii_ptr || PNULL == default_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_368_112_2_18_1_59_12_90,(uint8*)"");
        return 0;
    }
    
    for (i=0; i<str_len; i++)
    {
        if (ascii_ptr[i] >= 0x80)
        {
            break;
        }
        else
        {
            // according to GSM 7bit default alphabet extension table
            if (MMIAPICOM_AsciiAsDefaultExt(ascii_ptr[i], &temp_char))
            {
                default_ptr[default_len] = 0x1b;
                default_len++;
                default_ptr[default_len] = temp_char;
                default_len++;
            }
            else
            {
                default_ptr[default_len] = ascii_to_default_table[ascii_ptr[i]];
                default_len++;
            }
        }
    }

    return (default_len);
}


/*****************************************************************************/
//  Description : Change from 03.38 alphabet table to ASCII alphabet table
//  Global resource dependence : none
//  Author: sunsome.ju
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_Default2Ascii(                          //RETURN:
                         uint8      *default_ptr,   //IN:
                         uint8      *ascii_ptr,     //OUT:
                         uint16     str_len         //IN:
                         )
{
    uint16 ascii_len = 0;
    int32 i      = 0;
    
    if (PNULL == ascii_ptr || PNULL == default_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_Default2Ascii PNULL == ascii_ptr || PNULL == default_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_416_112_2_18_1_59_12_91,(uint8*)"");
        return 0;
    }

    for (i=0; i<str_len; i++)
    {
        if (default_ptr[i] >= 0x80)
        {
            break;
        }
        else
        {
            if (default_ptr[i] == 0x1b)
            {
                i++;
                if (default_ptr[i] >= 0x80)
                {
                    break;
                }
                else
                {
                    ascii_ptr[ascii_len] = default_to_ascii_extension_table[default_ptr[i]];
                    ascii_len++;
                }
            }
            else
            {
                 ascii_ptr[ascii_len] = default_to_ascii_table[default_ptr[i]];
                 ascii_len++;
            }
        }
    }

    return (ascii_len);
}


/*****************************************************************************/
//  Description : Count default extension character
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_CountDefaultExt(                            //RETURN: the num of default extension character
                                        wchar   *ascii_ptr,     //IN:
                                        uint16  str_len         //IN:
                                        )
{
    uint16  ext_count   = 0;
    int32   i           = 0;
    uint8   temp_char   = 0;

    if (PNULL == ascii_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_CountDefaultExt PNULL == ascii_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1142_112_2_18_1_59_13_104,(uint8*)"");
        return 0;
    }
    
    for (i=0; i<str_len; i++)
    {
#if 0
        if (ascii_ptr[i] >= 0x80 && EURO_UNICODE != ascii_ptr[i])
        {
            continue;
        }
        else
        {
            // according to GSM 7bit default alphabet extension table
            if (MMIAPICOM_WcharAsDefaultExt(ascii_ptr[i], &temp_char))
            {
                ext_count++;
            }
        }
#else
        // according to GSM 7bit default alphabet extension table
        if (MMIAPICOM_WcharAsDefaultExt(ascii_ptr[i], &temp_char))
        {
            ext_count++;
        }
#endif
    }

    return (ext_count);
}


/*****************************************************************************/
// Description : when the digit is "xx." ,change it to be "xx"
// Global resource dependence : 
// Author:   zhaohui
// Note:   
/*****************************************************************************/
PUBLIC void MMIAPICOM_CheckDecimalPoint(uint8 *buf)
{
    int16 len = 0;
    
    len = SCI_STRLEN((char*)buf);
    
    if (buf[len-1] == '.')
    {
        buf[len-1] = 0;
    }
}

/*******************************************************************************************/
//  Description : parse sim string
//  Global resource dependence :
//  Author: wancan.you
//  Note:   
/*********************************************************************************************/
PUBLIC void MMIAPICOM_ParseSimStr(uint8* sim_str,//IN:
                                            uint32 sim_str_len,//IN:
                                            MMI_STRING_T* mmi_str,//OUT:
                                            uint32 mmi_buf_len//IN:
                                            )
{
    uint32 i = 0, dest_pos= 0;
    uint16 valid_len = 0, ucs2_base = 0;
    uint16 wide_str = 0;
    uint8* valid_ptr = PNULL;
    uint8* temp_str  = PNULL;

    if (PNULL == sim_str || PNULL == mmi_str->wstr_ptr || mmi_buf_len == 0 )
    {
        //SCI_TRACE_LOW:"MMIAPICOM_ParseSimStr PNULL == sim_str || PNULL == mmi_str->wstr_ptr || mmi_buf_len == 0 "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1603_112_2_18_1_59_14_115,(uint8*)"");
        return;
    }
    SCI_MEMSET(mmi_str->wstr_ptr, 0, (mmi_buf_len * sizeof(wchar)));

    //SCI_TRACE_LOW:"[MMICOM] sim_str_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1608_112_2_18_1_59_14_116,(uint8*)"d", sim_str_len);

    temp_str = (uint8*)mmi_str->wstr_ptr;

    mmi_str->wstr_len = 0;
    mmi_buf_len = mmi_buf_len * sizeof(wchar);

    if (0 == sim_str_len)
    {
        return;
    }

    if ((uint8)0x80 == sim_str[0])      // the valid characters begin from char[1]
    {
        uint8 len_invalid = 0;

        if (sim_str_len > 1)
        {
            mmi_str->wstr_len = sim_str_len - 1;
        }
        else
        {
            return;
        }

        if(mmi_str->wstr_len % 2)
        {
            mmi_str->wstr_len--;
        }
        for (i = mmi_str->wstr_len; i > 0; i-=2)
        {
            if(((sim_str[i] == 0x00) && (sim_str[i-1] == 0x00)) || ((sim_str[i] == 0xff) && (sim_str[i-1] == 0xff)) )
            {
                len_invalid += 2;
            }
            else
            {
                break;
            }
        }
        mmi_str->wstr_len -= len_invalid;

        if (mmi_str->wstr_len > mmi_buf_len)
        {
            mmi_str->wstr_len = mmi_buf_len;
        }

        if (mmi_str->wstr_len > sim_str_len)
        {
            mmi_str->wstr_len = sim_str_len;
        }
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
        MMI_MEMCPY(temp_str, mmi_buf_len, &(sim_str[1]), sim_str_len, mmi_str->wstr_len);
#else
        GUI_UCS2L2UCS2B(temp_str, mmi_str->wstr_len, &(sim_str[1]), mmi_str->wstr_len);
#endif

        mmi_str->wstr_len = mmi_str->wstr_len/sizeof(wchar);
    }
    else if ((uint8)0x81 == sim_str[0])     // each UNICODE charater was composed of two bytes
    {
        valid_len = sim_str[1];
        if (valid_len > mmi_buf_len)
        {
            valid_len = mmi_buf_len;
        }
        mmi_str->wstr_len = valid_len;
        // one byte is base byte, saved in char[2], it is the UNICODE character 's high byte
        ucs2_base = sim_str[2];     
        ucs2_base = (uint16)(ucs2_base << 7);
        // another byte lies in character array, and them are the UNICODE character 's low byte.
        valid_ptr = &sim_str[3];        
        dest_pos = 0;                       
        for (i = 0; i < valid_len; i++) 
        {
            if ((valid_ptr[i] >= (uint8)0x80)) 
            {
                wide_str = (uint16)(ucs2_base + (valid_ptr[i] - 0x80));
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
                temp_str[dest_pos] = (uint8)((wide_str & 0xFF00) >> 8);
                temp_str[dest_pos + 1] = (uint8)(wide_str & 0x00FF);
#else
                temp_str[dest_pos + 1] = (uint8)((wide_str & 0xFF00) >> 8);
                temp_str[dest_pos] = (uint8)(wide_str & 0x00FF);
#endif
            }
            else 
            {   // if the character is not extended ASCII, the high byte should be 0x00.
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
                temp_str[dest_pos] = 0;
                temp_str[dest_pos + 1] = valid_ptr[i];
#else
                temp_str[dest_pos + 1] = 0;
                temp_str[dest_pos] = valid_ptr[i];
#endif
            }
            dest_pos += 2;
            if (dest_pos >= mmi_buf_len)
            {
                break;
            }
        }
    }
    else if ((uint8)0x82 == sim_str[0])     // each UNICODE charater was composed of two bytes
    {
        //mmi_str->is_ucs2 = TRUE;
        valid_len = sim_str[1];
        if (valid_len > mmi_buf_len)
        {
            valid_len = mmi_buf_len;
        }
        mmi_str->wstr_len = valid_len;
        // each wide byte is base byte, saved in char[2] & char[3]
        ucs2_base = sim_str[2];
        ucs2_base = (uint16)((ucs2_base << 8) + sim_str[3]);
        // another byte lies in character array, and them are the UNICODE character 's low byte.
        valid_ptr = &sim_str[4];
        dest_pos = 0; 
        for (i = 0; i < valid_len; i++) 
        {
            if (valid_ptr[i] >= 0x80) 
            {
                wide_str = (uint16)(ucs2_base + (valid_ptr[i] - 0x80));
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
                temp_str[dest_pos] = (uint8)((wide_str & 0xFF00) >> 8);
                temp_str[dest_pos + 1] = (uint8)(wide_str & 0x00FF);
#else
                temp_str[dest_pos + 1] = (uint8)((wide_str & 0xFF00) >> 8);
                temp_str[dest_pos] = (uint8)(wide_str & 0x00FF);
#endif
            }
            else 
            {   // if the character is not extended ASCII, the high byte should be 0x00.
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
                temp_str[dest_pos] = 0;
                temp_str[dest_pos + 1] = valid_ptr[i];
#else
                temp_str[dest_pos + 1] = 0;
                temp_str[dest_pos] = valid_ptr[i];
#endif
            }
            dest_pos += 2;
            if (dest_pos >= mmi_buf_len)
            {
                break;
            }
        }
    }
    else                                        // alphabet
    {
        mmi_str->wstr_len = sim_str_len;
        if (mmi_str->wstr_len > mmi_buf_len)
        {
            mmi_str->wstr_len = mmi_buf_len;
        }

        mmi_str->wstr_len = MMIAPICOM_Default2Wchar(&(sim_str[0]),
                                                                                    mmi_str->wstr_ptr,
                                                                                    mmi_str->wstr_len);
    }
    //SCI_TRACE_LOW:"[MMICOM] mmi_str->length = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1768_112_2_18_1_59_14_117,(uint8*)"d", mmi_str->wstr_len);

    return;
}

/*****************************************************************************/
// 	Description : convert SIM str to the real phone entry alpha
//	Global resource dependence : none
//    Author: dave.ruan
//	Note: OUT len and data, mainly for outputing SIM str len
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_ConvSIMStr2PhoneAlpha(
										   uint8	*alpha_ptr,		//IN/OUT:
										   uint8	*alpha_len_ptr,	//IN/OUT:
										   uint8	max_len	//IN/OUT:
										   ) 
{
    BOOLEAN result = TRUE;
    uint8 byte_num = 0;
    uint8 ucs2_num = 0;
    int32 i = 0;

    if(PNULL == alpha_ptr || PNULL == alpha_len_ptr)
    {
        return FALSE;
    }
    
    if ((0 < *alpha_len_ptr)/* && (PNULL != *alpha_ptr)*/)//Bug 350354
    {
    	byte_num = MIN(*alpha_len_ptr, PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN);

        if(byte_num > max_len)
        {
            *alpha_len_ptr = max_len;
            return FALSE;
        }
	
    	switch (alpha_ptr[0])
    	{
    	case 0x80:
    		byte_num = byte_num - ((byte_num + 1) % 2);	//odd
    		ucs2_num = (byte_num + 1) / 2;
    		for (i=0; i < ucs2_num; i++)
    		{
    			if (((0xff == alpha_ptr[i*2 + 1]) && (0xff == alpha_ptr[i*2 + 2]))
    				|| ((0x00 == alpha_ptr[i*2 + 1]) && (0x00 == alpha_ptr[i*2 + 2])))
    			{
    				byte_num = i*2 + 1;
    				break;
    			}
    		}
    		*alpha_len_ptr = byte_num;
    		break;

    	case 0x81:
    		alpha_ptr[1]   = MIN(alpha_ptr[1], PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN - 3);
    		*alpha_len_ptr = alpha_ptr[1] + 3;
    		break;
    		
    	case 0x82:
    		alpha_ptr[1]   = MIN(alpha_ptr[1], PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN - 4);
    		*alpha_len_ptr = alpha_ptr[1] + 4;
    		break;
    		
    	default:
    		for (i=0; i<byte_num; i++)
    		{
    			if (0xff == alpha_ptr[i])
    			{
    				*alpha_len_ptr = (uint8)i;
    				break;
    			}
    		}
      		*alpha_len_ptr = i;
  		break;
    	}
    }
    else
    {
        *alpha_len_ptr = 0;
    	result = FALSE;
    }
    return (result);
}

/*****************************************************************************/
// 	Description : encode alphabet under sim standard
//	Global resource dependence : none
//  Author:baokun
//	Note:
/*****************************************************************************/
BOOLEAN MMIAPICOM_EncodeGSMAlphabet(
	PHONEBOOK_ALPHABET_TYPE_E	alpha_type,
	uint8						input_len,
    uint8	   					*input_ptr,
    uint16						*output_len,
    uint8						*output_ptr,
    uint16                      max_out_len
)
{
	BOOLEAN result  = TRUE;
	
	if((input_ptr == PNULL)||(output_len == PNULL)||(output_ptr == PNULL) || (input_len == 0))
	{
		return FALSE;
	}
	
// 	if(input_len > max_out_len)
// 	{
// 		return FALSE;
// 	}
	
	switch(alpha_type)
	{
	case PHONEBOOK_ALPHABET_ASCII:
	    // ascii string,need not to encode
		*output_len = MIN(input_len,max_out_len);
        if (*output_len>PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN)
        {
            *output_len = PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN;
        }
		SCI_MEMCPY(output_ptr, input_ptr, *output_len);
		break;

	case PHONEBOOK_ALPHABET_UCS2:
	    // ucs2 string, encode it
        result = EncodeUnicode2GSMAlphabet(input_len, input_ptr, output_len, output_ptr,max_out_len);
		break;

	default:
		//SCI_TRACE_LOW:"MMIAPICOM_EncodeGSMAlphabet alpha_type  %d !"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1938_112_2_18_1_59_15_124,(uint8*)"d", alpha_type);
		break;
	}
	return result;
}

/*****************************************************************************
Description : encode alphabet under sim standard
Global resource dependence : none
Author: baokun.yin
Note:
*****************************************************************************/
BOOLEAN MMIAPICOM_DecodeGSMAlphabet(
									PHONEBOOK_ALPHABET_TYPE_E	*alpha_type,//OUT
									uint16						input_len,//IN
									uint8	   					*input_ptr,//IN
									uint16                      output_max_len,//IN
									uint16						*output_len,//OUT
									uint8						*output_ptr//OUT
									)
{

    // uint16	i=0;
    // uint8   base_char =0;
    // BOOLEAN is_extend_default = FALSE;
    // uint16  base_unicode = 0;
    // uint8   offset=0;

   if((alpha_type == PNULL)||(input_ptr == PNULL)||(output_len == PNULL)||(output_ptr == PNULL) || (input_len == 0))
	{
	    //SCI_TRACE_LOW:"MMIAPICOM_DecodeGSMAlphabet invalid parameter !"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_1967_112_2_18_1_59_15_125,(uint8*)"");
		return FALSE;
	}
	
	switch(input_ptr[0])
	{
    case 0x80:
		//If the first octet in the alpha string is '80', 
		//then the remaining octets are 16 bit UCS2 characters,
        *alpha_type = PHONEBOOK_ALPHABET_UCS2;
        *output_len = input_len - 1;
		if(*output_len > 0)
		{
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
			SCI_MEMCPY(output_ptr, &input_ptr[1], MIN(*output_len, output_max_len));
#else
            GUI_UCS2B2UCS2L(output_ptr,
				*output_len,
				&input_ptr[1],
				*output_len);
#endif
		}
        break;
    case 0x81:
		//'81' decode
        *alpha_type = PHONEBOOK_ALPHABET_UCS2;
        Decode81String(input_len, input_ptr, output_max_len, output_len, output_ptr);
        break;
    case 0x82:
		// '82' decode
		Decode82String(input_len, input_ptr, output_max_len, output_len, output_ptr);
        *alpha_type = PHONEBOOK_ALPHABET_UCS2;

        break;
    default:
        *alpha_type = PHONEBOOK_ALPHABET_ASCII;
        *output_len = input_len;
        SCI_MEMCPY(output_ptr, input_ptr, input_len);
        break;
    }
    if(input_len > 0 && *output_len == 0)
    {
        //need correct 
        *output_len = input_len*2;
        *output_ptr = *input_ptr;
    }
    return TRUE ;
}

/*****************************************************************************
Description : 81 coding decode
Global resource dependence : none
Author: baokun.yin
Note:
	   If the first octet of the alpha string is set to '81', 
	   then the second octet contains the number of characters in the string, 
	   the third octet contains an 8 bit number defines bits 15 to 8 of a 16 bit base pointer.
	   The fourth and subsequent octets in the string offset value(bit 8 sets 1), or contain a ASCCII(bit 8 sets 0),

*****************************************************************************/
BOOLEAN Decode81String(  			
					   uint16						input_len,//IN
					   uint8	   					*input_ptr,//IN
					   uint16                      output_max_len,//IN
					   uint16						*output_len,//OUT
					   uint8						*output_ptr//OUT
					   )
{
	uint16	i=0;
    wchar  *wchar_ptr = PNULL;
    uint8   base_char =0;
    BOOLEAN is_extend_default = FALSE;
    uint8   offset=0;

	if(input_len <= MMICOM_81STR_OFFSET)
	{
		//string no value
		*output_len = 0;
		return FALSE;
	}
	
	*output_len = 2 * input_ptr[1];
	if(*output_len > 0 )
	{
		if(*output_len > output_max_len)
		{
			//set output len less max len
			input_ptr[1] = output_max_len / 2;
			*output_len = output_max_len;
		}
		SCI_MEMSET((void*)output_ptr,0,*output_len);
		//get charater count
		base_char = (input_ptr[2] >> 1) & 0x7F;
		
		for(i = 0; i < input_ptr[1]; i++)
		{
			//decode string
			if(!(input_ptr[i + MMICOM_81STR_OFFSET] & 0x80))
			{
				//if bit 8 is 0, the remaining 7 bits of the octet contain a GSM Default Alphabet character(ASCII),
				if(is_extend_default) 
				{
					//some extend alphabet like �(0x20ac) need parse to a wchar rather than a char
                    uint16 ext_alphabet = sms_default_to_ascii_extension_table[input_ptr[i+ MMICOM_81STR_OFFSET]];

#if defined(__BIG_ENDIAN) || defined(__BigEndian)
					output_ptr[offset++] = ((ext_alphabet>>8) & 0xFF);
					output_ptr[offset++] = (ext_alphabet & 0xFF);
					
#else
					output_ptr[offset++] = (ext_alphabet & 0xFF);
					output_ptr[offset++] = ((ext_alphabet>>8) & 0xFF);
#endif
					is_extend_default = FALSE;
					(*output_len) -= 2;
				}
				else
				{
					if(0x1b == input_ptr[i+ MMICOM_81STR_OFFSET]) 
					{
						//the next is default string
						is_extend_default = TRUE;
						continue;
					}
					else
					{
						
						//SCI_TransDefault2Unicode(input_ptr[i+MMICOM_81STR_OFFSET],&(output_ptr[offset++]));
						//MMIAPICOM_Default2Ascii(&input_ptr[i+MMICOM_81STR_OFFSET],&(output_ptr[offset++]), 1);
                        wchar_ptr = &output_ptr[offset++]; /*lint !e64*/
                        *wchar_ptr = sms_default_to_ascii_table[input_ptr[i+MMICOM_81STR_OFFSET]];
// #if (defined(__BIG_ENDIAN) || defined(__BigEndian))
// 						output_ptr[offset-1]^=output_ptr[offset]^=output_ptr[offset-1]^=output_ptr[offset];
// 						
// #endif 
						offset++;
						
					}
				}
				
			}
			else
			{
				//bit 8 is 1, this string is offset value, need add base char
#if defined(__BIG_ENDIAN) || defined(__BigEndian)		
				output_ptr[offset++] = base_char;
				output_ptr[offset++] = (input_ptr[i+ MMICOM_81STR_OFFSET] & 0x7F) | ((input_ptr[2] << 7) & 0x80);
#else
				output_ptr[offset++] = (input_ptr[i+MMICOM_81STR_OFFSET] & 0x7F) | ((input_ptr[2] << 7) & 0x80);
				output_ptr[offset++] = base_char;
#endif
			}
			
			is_extend_default = FALSE;
		}
	}
	return TRUE;
}


/*****************************************************************************
Description : 82 coding decode
Global resource dependence : none
Author: baokun.yin
Note:
		If the first octet of the alpha string is set to '82'
		the second octet contains a value indicating the number of characters in the string
		the third and fourth octets contain a 16 bit number defines the complete 16 bit base pointer
		The fifth and subsequent octets is GSM Default Alpha(bit 8 is 0), offset value by base pointer(bit 8 is 1)
*****************************************************************************/
BOOLEAN Decode82String(  			
					   uint16						input_len,//IN
					   uint8	   					*input_ptr,//IN
					   uint16                      output_max_len,//IN
					   uint16						*output_len,//OUT
					   uint8						*output_ptr//OUT
					   )
{
	uint16	i=0;
    wchar  *wchar_ptr = PNULL;
    uint16   base_unicode =0;
    BOOLEAN is_extend_default = FALSE;
    uint8   offset=0;
	
	if(input_len <= MMICOM_82STR_OFFSET)
	{
		*output_len = 0;
		return FALSE;
	}
	*output_len = 2 * input_ptr[1];
	if(*output_len > output_max_len)
	{
		input_ptr[1] = output_max_len >> 1;
		*output_len = output_max_len;
	}
	SCI_MEMSET((void*)output_ptr,0,*output_len);
	for(i = 0; i < input_ptr[1]; i++)
	{
		if((input_ptr[i+ MMICOM_82STR_OFFSET] & 0x80) == 0x00)
		{
			//bit 8 is 0, it is GSM Default Alpha
			if(is_extend_default) 
			{
				//extend table value
                uint16 ext_alphabet = sms_default_to_ascii_extension_table[input_ptr[i+ MMICOM_82STR_OFFSET]];

#if defined(__BIG_ENDIAN) || defined(__BigEndian)
					output_ptr[offset++] = ((ext_alphabet>>8) & 0xFF);
					output_ptr[offset++] = (ext_alphabet & 0xFF);
					
#else
					output_ptr[offset++] = (ext_alphabet & 0xFF);
					output_ptr[offset++] = ((ext_alphabet>>8) & 0xFF);
#endif
				
				is_extend_default = FALSE;
				continue;
			}
			else
			{
				if(0x1b == input_ptr[i+4]) 
				{
					is_extend_default = TRUE;
					(*output_len) -=2;
					continue;
				}
				else
				{
					//SCI_TransDefault2Unicode (input_ptr[i+MMICOM_82STR_OFFSET ],&(output_ptr[offset++]));
					//MMIAPICOM_Default2Ascii(&input_ptr[i+MMICOM_82STR_OFFSET],&(output_ptr[offset++]), 1);
                    wchar_ptr = &output_ptr[offset++]; /*lint !e64*/
                    *wchar_ptr = sms_default_to_ascii_table[input_ptr[i+MMICOM_82STR_OFFSET]];
// #if (defined(__BIG_ENDIAN) || defined(__BigEndian))
// 					output_ptr[offset-1]^=output_ptr[offset]^=output_ptr[offset-1]^=output_ptr[offset];
// #endif
					
					offset++;
				}
			}
			
		}
		else
		{
			
			base_unicode = (input_ptr[2] << 8) + input_ptr[3];
			base_unicode += (input_ptr[i+MMICOM_82STR_OFFSET] & 0x7F);
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
			output_ptr[offset++] = (uint8)((base_unicode & 0xFF00) >> 8);
			output_ptr[offset++] = (uint8)(base_unicode & 0x00FF);
#else
			output_ptr[offset++] = (uint8)(base_unicode & 0x00FF);
			output_ptr[offset++] = (uint8)((base_unicode & 0xFF00) >> 8);
#endif
			
			
		}
		is_extend_default = FALSE;
		
	}
	return TRUE;
}

/*****************************************************************************/
// 	Description : get alphabet encode type
//	Global resource dependence : none
//  Author: rui.xu
//	Note: GSM 11.11 Annex B
/*****************************************************************************/
LOCAL MMI_ALPHA_TYPE_E GetAlphabetEncodeType(                        //RETURN:
                                            uint16  *base_addr_ptr, //OUT:
                                            uint16  *defaul_char_num_ptr,//OUT:
                                            uint8   ucs2_len,       //IN:
                                            uint16	*ucs2_ptr       //IN:
                                            )
{
    MMI_ALPHA_TYPE_E alpha_type = MMI_ALPHA_TYPE_DEFAULT;
    uint8   base_addr = 0;
    uint16  max_ucs2  = 0;
    uint16  min_ucs2  = 0XFFFF; // exclude default alphabet
    uint8   i         = 0;

    *base_addr_ptr = 0;
    for (i=0; i<ucs2_len; i++)
    {
        max_ucs2 = MAX(ucs2_ptr[i], max_ucs2);
        if (ucs2_ptr[i] >= 0x80)
        {
            min_ucs2 = MIN(ucs2_ptr[i], min_ucs2);
        }
        else if(defaul_char_num_ptr != PNULL)
        {
            if(IsDefaultExtChar(ucs2_ptr[i]))(*defaul_char_num_ptr)++;
        }
    }

    if (0XFFFF == min_ucs2)
    {
        alpha_type = MMI_ALPHA_TYPE_DEFAULT;
    }
    else
    {
        // the range of ucs2, not include default alphabet, is out of "half page"
        if ((max_ucs2 - min_ucs2 >= 0X80))
        {
            alpha_type = MMI_ALPHA_TYPE_80;
        }
        else
        {
            base_addr = (min_ucs2 >> 7);
            if ((max_ucs2 >> 7) == base_addr)
            {
                // is in the same half page
                *base_addr_ptr = base_addr;
                alpha_type     = MMI_ALPHA_TYPE_81;
            }
            else
            {
                *base_addr_ptr = min_ucs2;
                alpha_type     = MMI_ALPHA_TYPE_82;
            }
        }
    }
     return (alpha_type);
}
/*****************************************************************************/
// 	Description : encode Alpha Fields for UCS2
//	Global resource dependence : none
//  Author: Tracy Zhang
//	Note: GSM 11.11 Annex B
/*****************************************************************************/
LOCAL BOOLEAN EncodeUnicode2GSMAlphabet(                       //RETURN:
                                 uint8	input_len,      //IN:
                                 uint8	*input_ptr,     //IN:
                                 uint16	*output_len_ptr,//OUT:
                                 uint8	*output_ptr,     //OUT:
                                 uint16 max_out_len
                                 )
{
    MMI_ALPHA_TYPE_E encode_type=MMI_ALPHA_TYPE_DEFAULT; 
    uint8   ext_value = 0x20;
    uint16  i=0,j=0;
    uint16  base_addr = 0;
    uint8   ucs2_count = 0;
    uint8   offset_value  = 0;
    uint16  *ucs2_str_ptr = PNULL;
    BOOLEAN result=TRUE;
   
	ucs2_str_ptr = SCI_ALLOC_APP((input_len)+ 2);

	if(ucs2_str_ptr == PNULL)
	{
		return FALSE;
	}
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
    for (i=0; i<(input_len >> 1); i++)
    {
        ucs2_str_ptr[i] = (input_ptr[i*2] << 8) + input_ptr[i*2+1];
        ucs2_count++;
     }

#else
    for (i=0; i<(input_len >> 1); i++)
    {
        ucs2_str_ptr[i] = input_ptr[i*2] +( input_ptr[i*2+1]<< 8);
        ucs2_count++;
     }
#endif
     encode_type = GetAlphabetEncodeType(&base_addr, PNULL, ucs2_count, ucs2_str_ptr);

     switch(encode_type)
     {
     case MMI_ALPHA_TYPE_80:
		 //80, add 80 in head, then copy it to dest
         output_ptr[0] = 0x80;
		 if(max_out_len > 1)
		 {
             input_len=MIN(input_len,(max_out_len - 1));
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
			SCI_MEMCPY(&output_ptr[1], input_ptr, input_len);
#else
			//80 是大端的存储格式，所以要把小端转换为大端
			//16 bit UCS2 characters, with the more
			//significant octet (MSO) of the UCS2 character coded 
			//in the lower numbered octet of the alpha field, and the less
			//significant octet (LSO) of the UCS2 character is coded 
			//in the higher numbered alpha field octet
            GUI_UCS2L2UCS2B(&output_ptr[1],
				input_len,
				input_ptr,
				input_len);
#endif			 
			 //SCI_MEMCPY(&output_ptr[1], input_ptr, input_len);

			 *output_len_ptr =  input_len + 1;
		 }
		 else
		 {
			 *output_len_ptr = 0;
		 }
         break;
         
     case MMI_ALPHA_TYPE_81:
		 //encode to 81
         if(max_out_len > MMICOM_81STR_OFFSET)
		 {
			 output_ptr[0] = 0x81;
			 output_ptr[1] = ucs2_count;
			 output_ptr[2] = (uint8)(base_addr);
			 *output_len_ptr = MMICOM_81STR_OFFSET;
			 for (i=0,j=0; j < ucs2_count && (*output_len_ptr < max_out_len); i++,j++)
			 {
				 //encode
				 //get its base char
				 offset_value = (uint8)(ucs2_str_ptr[j] & 0x7f);

                 if (ucs2_str_ptr[j] >= 0x80 && !IsDefaultExtChar(ucs2_str_ptr[j]))
                 {
                     if(MMIAPICOM_WcharAsDefaultGreek(ucs2_str_ptr[j], &ext_value))
                     {
                         //greek GSM alpha
                         output_ptr[i + MMICOM_81STR_OFFSET] = ext_value;
                         (*output_len_ptr)++;
                         
                     }
                     else
                     {
                         //get this unicode's offset
                         offset_value += 0x80;
                         output_ptr[i+3] = offset_value;
                         (*output_len_ptr) ++;
                     }
                 }
				 else if(MMIAPICOM_WcharAsDefaultExt(ucs2_str_ptr[j], &ext_value)) 
				 {
					 //extern default GSM alpha
					 if(((*output_len_ptr)+2) > max_out_len)
					 {
						 break;
					 }
					 output_ptr[i+++MMICOM_81STR_OFFSET] = 0x1b;
					 output_ptr[i+MMICOM_81STR_OFFSET] = ext_value;
					 output_ptr[1]++;
					 (*output_len_ptr) +=2;
					 
				 }
				 else
				 {
					 //default GSM alpha
					 output_ptr[i + MMICOM_81STR_OFFSET] = sms_ascii_to_default_table[ucs2_str_ptr[j]];
					 (*output_len_ptr)++;
				 } 
				 
			 }
		 }
         
         break;
         
     case MMI_ALPHA_TYPE_82:
         if(max_out_len > MMICOM_82STR_OFFSET)
		 {
			 *output_len_ptr= MMICOM_82STR_OFFSET ;
			 output_ptr[0] = 0x82;
			 output_ptr[1] = ucs2_count;
			 output_ptr[2] = (uint8)(base_addr >> 8);//base point
			 output_ptr[3] = (uint8)(base_addr & 0xff);//base point
			 for (i=0,j=0; j< ucs2_count; i++,j++)
			 {
				 if (ucs2_str_ptr[j] >= 0x80  && !IsDefaultExtChar(ucs2_str_ptr[j]))
				 {
                     if(MMIAPICOM_WcharAsDefaultGreek(ucs2_str_ptr[j], &ext_value))
                     {
                         output_ptr[i+ MMICOM_82STR_OFFSET] =ext_value; //(uint8)(ucs2_str[i] & 0x7f);
                         (*output_len_ptr) ++;
                     }
                     else
                     {
                         //get unicode's offset	 
                         output_ptr[i+4] = (uint8)((ucs2_str_ptr[j] - base_addr) + 0x80);
                         (*output_len_ptr)++;
                     }
				 }
				 else if(MMIAPICOM_WcharAsDefaultExt(ucs2_str_ptr[j], &ext_value)) 
				 {
					 //extern default GSM alpha
					 if(((*output_len_ptr)+2)> max_out_len)
					 {
						 break;
					 }
					 output_ptr[i+++ MMICOM_82STR_OFFSET] = 0x1b;
					 output_ptr[i+ MMICOM_82STR_OFFSET] = ext_value;
					 output_ptr[1]++;
					 (*output_len_ptr) +=2;
					 
				 }
				 else
				 {
					 ////default GSM alpha
					 output_ptr[i+ MMICOM_82STR_OFFSET] =sms_ascii_to_default_table[ucs2_str_ptr[j]]; //(uint8)(ucs2_str[i] & 0x7f);
					 (*output_len_ptr) ++;
				 }
			 }
         }
         break;	
         
         
     default:
         
         result=FALSE;
         break;
     }

	 SCI_FREE(ucs2_str_ptr);
     return result;
     
}

/*****************************************************************************/
// 	Description :  alphabet under sim standard encode len
//	Global resource dependence : none
//  Author:baokun
//	Note:
/*****************************************************************************/
uint16 MMIAPICOM_GetUCS2MaxEncodeLen(
	PHONEBOOK_ALPHABET_TYPE_E	alpha_type,
	uint8                       alpha_max_len,
	uint8						wchar_len,
    wchar	   					*wchar_ptr
 )
{
	uint16 base_addr = 0;
	uint16 max_ucs2_len = 0;
    uint16 ext_char_num = 0;
	uint32  i = 0;
	BOOLEAN is_max = FALSE;
	MMI_ALPHA_TYPE_E encode_type = GetAlphabetEncodeType(&base_addr, PNULL, wchar_len, wchar_ptr);

	if(PHONEBOOK_ALPHABET_ASCII == alpha_type)
	{
		max_ucs2_len = alpha_max_len;
	}
	else
	{
		for(i = 0; i < wchar_len; i++)
		{
			base_addr = 0;
            ext_char_num = 0;
			encode_type = GetAlphabetEncodeType(&base_addr, &ext_char_num, i +1, wchar_ptr);
			switch(encode_type)
			{
			case MMI_ALPHA_TYPE_80:
				if((((i+1 )<< 1) + 1) > alpha_max_len)
				{
                    is_max = TRUE;
				}
				break;
			case MMI_ALPHA_TYPE_81:
				if(i+1 + MMICOM_81STR_OFFSET +ext_char_num > alpha_max_len)
				{
					is_max = TRUE;
				}		 
				break;
			case MMI_ALPHA_TYPE_82:
				if(i+1 + MMICOM_82STR_OFFSET +ext_char_num> alpha_max_len)
				{
					is_max = TRUE;
				}	
				break;
			default:
				if(i+1 > alpha_max_len)
				{
                    is_max = TRUE;
				}
				break;
			}
			if(is_max)
			{
				break;
			}
			else
			{
				max_ucs2_len = i + 1;
			}
		}
	}
	max_ucs2_len = MIN(max_ucs2_len, wchar_len);
    //SCI_TRACE_LOW:"MMIAPICOM_GetGSMAlphabetLen encode_type %d wchar_len %d alpha_len %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_2513_112_2_18_1_59_16_126,(uint8*)"ddd",encode_type, wchar_len, alpha_max_len);
	return max_ucs2_len;

}

/*****************************************************************************/
//  Description : operate the wchar as the character in default ext table 
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_WcharAsDefaultExt(
                                           wchar    data_in,         //IN:
                                           uint8    *default_char_ptr   //OUT:
                                           )
{
    BOOLEAN     result = TRUE;

    switch (data_in) 
    {
    case 0x005E://^
        *default_char_ptr = 0x14;
        break;

    case 0x007B://{
        *default_char_ptr = 0x28;
        break;

    case 0x007D://}
        *default_char_ptr = 0x29;
        break;

    case 0x005C://\   /*!e427*/
        *default_char_ptr = 0x2f;
        break;

    case 0x005B://[
        *default_char_ptr = 0x3c;
        break;

    case 0x007E://~
        *default_char_ptr = 0x3d;
        break;

    case 0x005D://]
        *default_char_ptr = 0x3e;
        break;
        
    case 0x007C://|
        *default_char_ptr = 0x40;
        break;

    case EURO_UNICODE://�
        *default_char_ptr = 0x65;
        break;
        
    default:
        *default_char_ptr = 0x20;
        result = FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : operate the wchar as the character in default greek 
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_WcharAsDefaultGreek(
                                           wchar    data_in,         //IN:
                                           uint8    *default_char_ptr   //OUT:
                                           )
{
    BOOLEAN     result = TRUE;

    switch (data_in) 
    {
    case 0x0394://Δ
        *default_char_ptr = 0x10;
        break;

    case 0x03A6://Φ
        *default_char_ptr = 0x12;
        break;

    case 0x0393://Γ
        *default_char_ptr = 0x13;
        break;

    case 0x039B://Λ
        *default_char_ptr = 0x14;
        break;

    case 0x03A9://Ω
        *default_char_ptr = 0x15;
        break;

    case 0x03A0://Π
        *default_char_ptr = 0x16;
        break;

    case 0x03A8://Ψ
        *default_char_ptr = 0x17;
        break;
        
    case 0x03A3://Σ
        *default_char_ptr = 0x18;
        break;

    case 0x0398://Θ
        *default_char_ptr = 0x19;
        break;

    case 0x039E://Ξ
        *default_char_ptr = 0x1A;
        break;
        
    default:
        *default_char_ptr = 0x20;
        result = FALSE;
        break;
    }

    return (result);
}


/*****************************************************************************/
//  Description : Change from 03.38 alphabet table to ASCII alphabet table
//  Global resource dependence : none
//  Author: sunsome.ju
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_DefaultToWstr(                          //RETURN:
                                      uint8     *default_ptr,   //IN:
                                      wchar      *ascii_ptr,        //OUT:
                                      uint16     str_len            //IN:
                                      )
{
    uint16 ascii_len = 0;
    int32 i      = 0;

    if (PNULL == ascii_ptr || PNULL == default_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_DefaultToWstr PNULL == ascii_ptr || PNULL == default_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_468_112_2_18_1_59_12_92,(uint8*)"");
        return 0;
    }

    for (i=0; i<str_len; i++)
    {
        if (default_ptr[i] >= 0x80)
        {
            break;
        }
        else
        {
            if (default_ptr[i] == 0x1b)
            {
                i++;
                if (default_ptr[i] >= 0x80)
                {
                    break;
                }
                else
                {
                    ascii_ptr[ascii_len] = default_to_ascii_extension_table[default_ptr[i]];
                    ascii_len++;
                }
            }
            else
            {
                 ascii_ptr[ascii_len] = default_to_ascii_table[default_ptr[i]];
                 ascii_len++;
            }
        }
    }

    return (ascii_len);
}
/*****************************************************************************/
//  Description : Change from ASCII alphabet table to 23.38 alphabet table
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_Wchar2default(                          //RETURN: the length of default string
                         wchar      *wchar_ptr,     //IN:
                         uint8      *default_ptr,   //OUT:
                         uint16     str_len         //IN:
                         )
{
    uint16  default_len = 0;
    int32   i           = 0;
    uint8   temp_char   = 0;

    if (PNULL == wchar_ptr || PNULL == default_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_Wchar2default PNULL == wchar_ptr || PNULL == default_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_2657_112_2_18_1_59_16_127,(uint8*)"");
        return 0;
    }
    
    for (i=0; i<str_len; i++)
    {
        // according to GSM 7bit default alphabet extension table
        if (MMIAPICOM_WcharAsDefaultExt(wchar_ptr[i], &temp_char))
        {
            default_ptr[default_len] = 0x1b;
            default_len++;
            default_ptr[default_len] = temp_char;
            default_len++;
        }
        else if(MMIAPICOM_WcharAsDefaultGreek(wchar_ptr[i], &temp_char))
        {
            default_ptr[default_len] = temp_char;
            default_len++;
        }
        else
        {
            default_ptr[default_len] = sms_ascii_to_default_table[wchar_ptr[i]];
            default_len++;
        }
    }

    return (default_len);
}

/*****************************************************************************/
//  Description : 判断wstring是否小于 0XFF的ASCII 字符串
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPICOM_IsASCIIStringForPB(
                                        const wchar* wstr_ptr, 
                                        uint16 wstr_len
                                        )
{
    BOOLEAN is_ascii = TRUE;
    uint32  i = 0;
    
    if( PNULL != wstr_ptr )
    {
        for ( ; i < wstr_len; i++ )
        {
            if(wstr_ptr[i] >= 0x7F)
            {
                is_ascii = FALSE;
                
                break;
            }
        }
    }

    return is_ascii;
}
/*****************************************************************************/
//  Description : 判断wstring是否为 default 字符串
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPICOM_IsDefaultString(
                                        const wchar* wstr_ptr, 
                                        uint16 wstr_len
                                        )
{
    BOOLEAN is_ascii = TRUE;
    uint32  i = 0;
    
    if( PNULL != wstr_ptr )
    {
        for ( ; i < wstr_len; i++ )
        {
            if( !MMIAPICOM_IsDefaultChar( wstr_ptr[i] ) &&
                !MMIAPICOM_IsDefaultExtChar( wstr_ptr[i] ) )
            {
                is_ascii = FALSE;
                
                break;
            }
        }
    }

    return is_ascii;
}

/*****************************************************************************/
//  Description : 判断是否为 default 字符
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPICOM_IsDefaultChar(uint16 ch)
{
    BOOLEAN is_default = FALSE;
    int32 i = 0;
    uint16 table_len = sizeof(sms_default_to_ascii_table)/sizeof(sms_default_to_ascii_table[0]);

    for(i=0;i<table_len;i++)
    {
        if(sms_default_to_ascii_table[i] == ch) 
        {
            is_default = TRUE;
            break;
        }
    }

    return is_default;
}

/*****************************************************************************/
//  Description : 判断是否为 default ext 字符
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPICOM_IsDefaultExtChar(uint16 ch)
{
    BOOLEAN is_default = FALSE;
    int32 i = 0;
    uint16 table_len = sizeof(sms_default_to_ascii_table)/sizeof(sms_default_to_ascii_table[0]);

    for(i=0;i<table_len;i++)
    {
        if(sms_default_to_ascii_extension_table[i] == ch) 
        {
            is_default = TRUE;
            break;
        }
    }

    return is_default;
}

/*****************************************************************************/
//  Description : Change from 23.38 alphabet table to ASCII alphabet table
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_Default2Wchar(                          //RETURN:
                                      uint8      *default_ptr,   //IN:
                                      wchar      *wchar_ptr,     //OUT:
                                      uint16     str_len         //IN:
                                      )
{
    uint16 ascii_len = 0;
    int32 i      = 0;

    if (PNULL == wchar_ptr || PNULL == default_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_Default2Wchar PNULL == wchar_ptr || PNULL == default_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICOM_STRING_2775_112_2_18_1_59_16_128,(uint8*)"");
        return 0;
    }
    
    for (i=0; i<str_len; i++)
    {
        if (default_ptr[i] >= 0x80)
        {
            break;
        }
        else
        {
            if (default_ptr[i] == 0x1b)
            {
                i++;
                if (default_ptr[i] >= 0x80)
                {
                    break;
                }
                else
                {
                    wchar_ptr[ascii_len] = sms_default_to_ascii_extension_table[default_ptr[i]];
                    ascii_len++;
                }
            }
            else
            {
                 wchar_ptr[ascii_len] = sms_default_to_ascii_table[default_ptr[i]];
                 ascii_len++;
            }
        }
    }

    return (ascii_len);
}

/*****************************************************************************/
//  Description : Opera escape '!' '~' '*' '\'' '(' ')' 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsUnreservedChar(uint8 c)
{
    if( (c>='0'&&c<='9') ||
        (c>='a'&&c<='z') ||
        (c>='A'&&c<='Z') ||
        c=='-' || c=='_' || c=='.'/*|| c=='!' || c=='~' || c=='*' || c=='\''  || c=='(' || c==')'*/) {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : Escape URL
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint32 URL_EscapeUrl(uint8 *dst_ptr, uint32 dst_len, const uint8 *url_ptr)
{
    uint8 c = 0;
    uint32 ret_len = 0;

    if(PNULL == dst_ptr || PNULL == url_ptr)
    {
        return 0;
    }

    while (0 != (c = *url_ptr++))
    {
        if (IsUnreservedChar(c))
        {
            if (ret_len + 1 <= dst_len)
            {
                dst_ptr[ret_len ++] = c;
            }
            else
            {
                break;
            }
        }
        else if (' ' == c)
        {
            if (ret_len + 1 <= dst_len)
            {
                dst_ptr[ret_len ++] = '+';
            }
            else
            {
                break;
            }
        }
        else
        {
            char str[10] = {0};
            uint16 len = 0;

            sprintf(str, "%02X", c);
            len = strlen(str);

            if (ret_len + len <= dst_len)
            {
                dst_ptr[ret_len ++] = '%';

                SCI_MEMCPY(dst_ptr+ ret_len, str, len);

                ret_len += len;
            }
            else
            {
                break;
            }
        }
    }

    return ret_len;
}

/*****************************************************************************/
//  Description : Normalize Url
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_NormalizeUrl(uint8 *in_url, uint8* out_url, uint16 out_max_len, BOOLEAN is_escape)
{
    uint16 url_len = 0;
    uint16 in_url_len = 0;
    uint8 *temp_str_ptr = PNULL;
    BOOLEAN is_scheme_header = FALSE;
    BOOLEAN result = TRUE;
    uint32 i = 0;
    const char * scheme_header[] = {MMICOM_URL_WTAI_TEL_HEADER,
                                                        MMICOM_URL_WTAI_WP_MC_EMPTY_HEADER,
                                                        MMICOM_URL_WTAI_WP_MC_HEADER,
                                                        MMICOM_URL_WTAI_WP_SD_HEADER,
                                                        MMICOM_URL_WTAI_WP_AP_EMPTY_HEADER,
                                                        MMICOM_URL_WTAI_WP_AP_HEADER,
                                                        MMICOM_URL_WTAI_SMS_HEADER,
                                                        MMICOM_URL_WTAI_SMS_TO_HEADER,
                                                        MMICOM_URL_WTAI_MMS_HEADER,
                                                        MMICOM_URL_WTAI_MMS_TO_HEADER,
                                                        MMICOM_URL_WTAI_MAIL_HEADER,
                                                        MMICOM_URL_WTAI_MAIL_TO_HEADER,
                                                        MMICOM_URL_HTTP_HEADER,
                                                        MMICOM_URL_HTTPS_HEADER,
                                                        MMICOM_URL_RTSP_HEADER,
                                                        MMICOM_URL_MMS_HEADER,
                                                        MMICOM_URL_FTP_HEADER,
                                                        MMICOM_URL_TELENT_HEADER,
                                                        MMICOM_URL_FILE_HEADER,
                                                        MMICOM_URL_ABOUT_BLANK
                                                        };
    uint16 scheme_header_len = ARR_SIZE(scheme_header);
    uint16 http_header_len = 0;
    uint16 chr_cnt = 0;
    uint8 *cpy_start = PNULL;

    if (PNULL == in_url || PNULL == out_url)
    {
        return FALSE;
    }

    SCI_TRACE_LOW("MMIAPICOM_NormalizeUrl in_url=%s", in_url);

    while(' ' == *in_url)
    {
        in_url++;
    }

    in_url_len = SCI_STRLEN((char *)in_url);

    if (0 == in_url_len)
    {
        SCI_TRACE_LOW("MMIAPICOM_NormalizeUrl: in_url empty");

        return FALSE;
    }

    for (i = 0; i < in_url_len; i++)
    {
        if (0x7f < *((uint8 *)(in_url + i)))
        {
            chr_cnt++;
        }
    }

    url_len = in_url_len + strlen(MMICOM_URL_HTTP_HEADER) + chr_cnt * 2; /* 非ASCII字符需要转义*/

    temp_str_ptr = SCI_ALLOCAZ(url_len + 1);

    if (PNULL == temp_str_ptr)
    {
        return FALSE;
    }

    SCI_MEMCPY(temp_str_ptr, in_url, in_url_len);
    temp_str_ptr = (uint8 *)MMIAPICOM_Strlower((char *)temp_str_ptr);

    for (i = 0; i < scheme_header_len; i++)
    {
        if (0 == strncmp((char *)temp_str_ptr, (char *)scheme_header[i], strlen((char *)scheme_header[i])))
        {
            is_scheme_header = TRUE;
            break;
        }
    }

    cpy_start = temp_str_ptr;
    SCI_MEMSET(temp_str_ptr, 0x00, (url_len + 1));

    if (!is_scheme_header)
    {
        http_header_len = SCI_STRLEN(MMICOM_URL_HTTP_HEADER);
        SCI_MEMCPY(temp_str_ptr, MMICOM_URL_HTTP_HEADER, http_header_len);
        cpy_start += strlen(MMICOM_URL_HTTP_HEADER);
    }

    if (is_escape)
    {
        URL_EscapeUrl(cpy_start, url_len, in_url);
    }
    else
    {
        SCI_MEMCPY(cpy_start, in_url, in_url_len);
    }

    url_len = SCI_STRLEN((char *)temp_str_ptr);
    url_len = MIN(url_len, out_max_len);

    SCI_MEMCPY(out_url, temp_str_ptr, url_len);

    SCI_FREE(temp_str_ptr);

    return result;
}




/*****************************************************************************/
//     Description : 
//     Global resource dependence :
//  Author:
//     Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPICOM_PrimoGenerateReplaceStr
(
const MMI_STRING_T *src_str,
const wchar* find_str,
const wchar* replace_str,
uint16 result_max_len,
MMI_STRING_T *result_str//out
)
{
       uint16 array_index = 0;
       wchar* str1 = PNULL;
       wchar* find_pos = PNULL;
       int src_str_remain_len = 0;
       int src_str_len = 0;
       int find_str_len = 0;
       int replace_str_len = 0;
       uint16 remain_len = result_max_len;
       uint16 min_len = 0;
       if(PNULL == src_str 
              || PNULL == src_str->wstr_ptr
              || PNULL == result_str 
              || PNULL == result_str->wstr_ptr
              || PNULL == find_str
              || PNULL == replace_str
              )
       {
              return FALSE;
       }
              
       src_str_remain_len = src_str_len = src_str->wstr_len;
       find_str_len = MMIAPICOM_Wstrlen(find_str);
       replace_str_len = MMIAPICOM_Wstrlen(replace_str);
       
       if(0 == src_str_len || 0 == find_str_len)
       {
              return FALSE;
       }
       
       str1 = src_str->wstr_ptr;
       find_pos = MMIAPICOM_Wstrstr(str1, find_str);
       if(PNULL == find_pos)
       {
              result_str->wstr_len = MIN(remain_len, src_str_len);
              MMIAPICOM_Wstrncpy(result_str->wstr_ptr, src_str->wstr_ptr, result_str->wstr_len);
              return FALSE;
       }
       str1 = src_str->wstr_ptr; 
       for(; *str1; str1++)
       {
              array_index++;
              if(str1 == find_pos) break;       
       }
       
       if(array_index > src_str_len)
       {
              result_str->wstr_len = MIN(remain_len, src_str_len);
              MMIAPICOM_Wstrncpy(result_str->wstr_ptr, src_str->wstr_ptr, result_str->wstr_len);
              return FALSE;
       }
       
       result_str->wstr_len = MIN(remain_len, array_index-1);
       MMIAPICOM_Wstrncpy(result_str->wstr_ptr, src_str->wstr_ptr, result_str->wstr_len);
       src_str_remain_len -= array_index-1;

       remain_len = remain_len - result_str->wstr_len;
       min_len = MIN(remain_len, replace_str_len);
       MMIAPICOM_Wstrncat(result_str->wstr_ptr, replace_str, min_len);
       result_str->wstr_len += min_len;
       src_str_remain_len -= find_str_len;
       
       str1 += find_str_len;
       remain_len = remain_len - min_len;
       min_len = MIN(remain_len, src_str_remain_len);
       MMIAPICOM_Wstrncat(result_str->wstr_ptr, str1, min_len);
       result_str->wstr_len += min_len;
       
       return TRUE;
       
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPICOM_PrimoGenerateReplaceStrByTextId
(
MMI_TEXT_ID_T text_id,
const wchar* find_str,
const wchar* replace_str,
uint16 result_max_len,
MMI_STRING_T *result_str//out
)
{
       MMI_STRING_T    src_str = {0};
       MMI_GetLabelTextByLang(text_id, &src_str);
       return MMIAPICOM_PrimoGenerateReplaceStr(&src_str, find_str, replace_str, result_max_len, result_str);
}




