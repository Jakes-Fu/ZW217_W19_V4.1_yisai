/***************************************************************************
 ** File Name:      sci_codec.c                                            *
 ** Author:         Xiangxin.Gu                                            *
 ** Date:           19/06/2004                                             *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved       *
 ** Description:    Declare all code and dec functions for at and sms      *
****************************************************************************
 
****************************************************************************
 **                        Edit History                                    *
 ** -----------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                            *
 ** 19/06/2002     Xiangxin Gu        Create.                              *
****************************************************************************/

#ifndef SCI_CODEC_H
#define SCI_CODEC_H

#include "sci_types.h"

/**------------------------------------------------------------------------*
 **                         Compiler Flag                                  *
 **------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

//extern const unsigned short number_of_ucs2_4E00_9FFF;
extern const unsigned char sci_ucs2_mirror_4E00_9FFF[ 0xA0 - 0x4E ][256 >> 3];
extern const unsigned short sci_ucs2_index_table_4E00_9FFF[0xA0 - 0x4E][4];
extern const unsigned char sci_number_of_bit_1[256];
//#ifdef IM_SIMP_CHINESE_SUPPORT
extern const unsigned short sci_gb2312_to_ucs2_table[];
extern const unsigned short sci_ucs2_to_gb2312_table[];
//#endif
/**********************************************************************
    Description: Trans a UCS2 format string to a GB2312 one
    Global resource dependence: ucs2_to_gb2312_table[], gb2312_to_ucs2_table[]
    return: SCI_SUCCESS...
    Author: @Xiangxin
    Note: if you want to trans a UCS2 code to GB2312 code, value ucs2_str_len 2
          ***please pay attention to that the unit of ucs2_str_len is byte
***********************************************************************/
extern uint32 SCI_TransUCS2StrToGB2312Str(
        uint8 *ucs2_str_ptr, // in: UCS2 string
        int16 ucs2_str_len, // in: bytes indicate the length of UCS2 string
        uint8 *gb2312_str_ptr, // in/out: buffer for GB2312 string
        int16 *gb2312_str_len // in: max bytes of the buffer for gb2312 string
        					   // out: bytes indicate the length of GB2312 string
        );

/**********************************************************************
    Description: Trans a GB2312 format string to a UCS2 one
    Global resource dependence: gb2312_to_ucs2_table[]
    return: SCI_SUCCESS...
    Author: @Xiangxin
    Note: 
***********************************************************************/
extern uint32 SCI_TransGB2312StrToUCS2Str(
        uint8 *gb2312_str_ptr, // in: GB2312 string
        int16 gb2312_str_len, // in: bytes indicate the length of GB2312 string
        uint8 *ucs2_str_ptr, // in/out: buffer for UCS2 string
        int16 *ucs2_str_len  // in: max bytes of the buffer for ucs2_data
        					 // out: bytes indicate the length of UCS2 string
        );



/**********************************************************************
    Description: Pack 8bits to 7bits
    Global resource dependence: none
    return: SCI_SUCCESS...
    Author: @Xiangxin
    Note: seven_bits_data_offset: (0-7), bit offset means
          how many bits from the highest bit in a byte
***********************************************************************/

extern uint32 SCI_Pack8bitsTo7bits(
	    uint8 *eight_bits_data_ptr, // in: data to be packed, which is eight_bits_data
	    int16 eight_bits_data_len, // in: bytes indicate the length of source data to be packed
	    
	    uint8 *seven_bits_data_ptr, // in/out: buffer for packed data, which is seven_bits_data
	    int16 *seven_bits_data_len, // in: max bytes of the buffer for seven_bits_data
	                               // out: bytes indicate the length of seven_bits_data
	    uint8 *seven_bits_data_offset // in: bit offset for begin
	    							  // out: bit offset for end
        );

/*********************************************************************************************************
    Description: Unpack some 7bits to 8bits
    Global resource dependence: SCI_UnpackSome7bitsTo8bits()
    return: SCI_SUCCESS...
    Author: @Xiangxin
    Note: how_many_7bits means how many 7bits in the data pointed by seven_bits_data_ptr
**********************************************************************************************************/
		
extern uint32 SCI_UnpackSome7bitsTo8bits(
	    uint8 *seven_bits_data_ptr, // in: data to be unpacked, which is seven_bits_data
	    int16 how_many_7bits, // in: how many 7bits in the data pointed by seven_bits_data_ptr
	    uint8 seven_bits_data_offset, // in: bit offset for begin
	    
	    uint8 *eight_bits_data_ptr, // in/out: buffer for upacked data, which is eight_bits_data
	    int16 *eight_bits_data_len // in: max bytes of the buffer for eight_bits_data
	                              // out: bytes indicate the length of eight_bits_data
	    );


/**********************************************************************
    Description: Unpack 7bits to 8bits
    Global resource dependence: none
    return: SCI_SUCCESS...
    Author: @Xiangxin
    Note: seven_bits_data_offset: (0-7), bit offset means
          how many bits from the highest bit in a byte
***********************************************************************/
		
extern uint32 SCI_Unpack7bitsTo8bits(
	    uint8 *seven_bits_data_ptr, // in: data to be unpacked, which is seven_bits_data
	    int16 seven_bits_data_len, // in: bytes indicate the length of seven_bits_data
	    uint8 seven_bits_data_offset, // in: bit offset for begin
	    
	    uint8 *eight_bits_data_ptr, // in/out: buffer for upacked data, which is eight_bits_data
	    int16 *eight_bits_data_len // in: max bytes of the buffer for eight_bits_data
	                              // out: bytes indicate the length of eight_bits_data
	    );




/**********************************************************************
    Description: trans bin to hex string
    Global resource dependence: none
    return: SCI_SUCCESS...
    Author: @Xiangxin
    Note: 
***********************************************************************/

extern uint32 SCI_TransBinToHexStr(
        uint8 *bin_data_ptr, // in: binary data
        int16 bin_data_len, // in: bytes of binary data
        char *hex_str_ptr, // in/out: buffer for hex string
        int16 max_hex_str_len // in: max bytes of the buffer for hex string
        );

/**********************************************************************
    Description: trans hex string to bin
    Global resource dependence: none
    return: SCI_SUCCESS...
    Author: @Xiangxin
    Note: 
***********************************************************************/

extern uint32 SCI_TransHexStrToBin(
        char *hex_str_ptr, // in: hex string
        uint8 *bin_data_ptr, // in/out: buffer for binary data
        int16 *bin_data_len // in: max bytes of the buffer for binary data
        				    // out: bytes indicate the length of binary data
        );




/**********************************************************************
    Description: trans BCD to string
    Global resource dependence: none
    return: SCI_SUCCESS...
    Author: @Xiangxin
    Note: 
***********************************************************************/

extern uint32 SCI_TransBCDToStr(
        uint8 *bcd_data_ptr, // in: bcd data
        int16 bcd_data_len, // in: length of bcd data
		char *bcd_str_ptr, // in/out: buffer for bcd string
		int16 max_bcd_str_len // in: max bytes of the buffer for bcd string
        );

/**********************************************************************
    Description: trans string to BCD
    Global resource dependence: none
    return: SCI_SUCCESS...
    Author: @Xiangxin
    Note: 
***********************************************************************/

extern uint32 SCI_TransStrToBCD(
		char *bcd_str_ptr, // in: bcd string
        uint8 *bcd_data_ptr, // in/out: buffer for bcd data
        int16 *bcd_data_len // in: max bytes of the buffer for bcd data
        					// out: bytes indicate the length of bcd data
		);

/**********************************************************************
    Description: 此函数用于从03.38表中字符转换成UNICODE码
    Global resource dependence: none
    return: SCI_SUCCESS...
    Author: @Xiangxin
    Note: 
***********************************************************************/
extern uint32 SCI_TransDefault2Unicode (
	    uint8 gsm_ch,
	    uint8 *uni_ch
	    );

/**********************************************************************
    Description: Trans a UCS2 format to a GB2312 one
    Global resource dependence: ucs2_to_gb2312_table[], gb2312_to_ucs2_table[]
    return: gb2312_ptr
    Author: @Xiangxin
    Note: 
          ***please pay attention to that the unit of ucs2_str_len is byte
***********************************************************************/
extern uint8* SCI_TransUCS2ToGB2312(
                        uint16  ucs2,         // the ucs2 code that need to be converted.
                        uint8*  gb2312_ptr    // store the gb2312 code
                        );

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  /* SCI_CODEC_H */

