/***************************************************************************
 ** File Name:      ps_char_pack.h                                         *
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

#ifndef PS_CHAR_PACK_H
#define PS_CHAR_PACK_H

#include "sci_types.h"

/**------------------------------------------------------------------------*
 **                         Compiler Flag                                  *
 **------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*********************************************************************************************************
    Description: Unpack 7bits to 8bits
    Global resource dependence: none
    return: SCI_SUCCESS...
    Author: @Xiangxin
    Note: 1) seven_bits_data_offset: (0-7), bit offset means
             how many bits from the lowest bit in a byte
          2) while there are (8n - 1 + offset) pieces of 7bits to be upacked,
             for example, offset == 0, for 7 pieces of 7bits, param seven_bits_data_len is 8
                                       for 8 pieces of 7bits, param seven_bits_data_len is 8 too
             so the function must be telled if the last 7bits needed to be trans to a byte.
             the way is: if the last piece of 7bits equal to 0x0d, there is no need to be trans to a byte
                         if the last piece of 7bits not equal to 0x0d, it is needed to be trans to a byte
**********************************************************************************************************/
uint32 PS_Unpack7bitsTo8bits(
                              uint8 *seven_bits_data_ptr,   // in: data to be unpacked, which is seven_bits_data
                              int16 seven_bits_data_len,    // in: bytes indicate the length of seven_bits_data
                              uint8 seven_bits_data_offset, // in: bit offset for begin
                              uint8 *eight_bits_data_ptr,   // in/out: buffer for unpacked data, which is eight_bits_data
                              int16 *eight_bits_data_len    // in: max bytes of the buffer for eight_bits_data
                                                            // out: bytes indicate the length of eight_bits_data
                              );

/*********************************************************************************************************
    Description: Unpack some 7bits to 8bits
    Global resource dependence: 
    return: SCI_SUCCESS...
    Author: @Xiangxin
    Note: how_many_7bits means how many 7bits in the data pointed by seven_bits_data_ptr
**********************************************************************************************************/
uint32 PS_UnpackSome7bitsTo8bits(
                                  uint8 *seven_bits_data_ptr,   // in: data to be unpacked, which is seven_bits_data
                                  int16 how_many_7bits,         // in: how many 7bits in the data pointed by seven_bits_data_ptr
                                  uint8 seven_bits_data_offset, // in: bit offset for begin
                                  uint8 *eight_bits_data_ptr,   // in/out: buffer for unpacked data, which is eight_bits_data
                                  int16 *eight_bits_data_len    // in: max bytes of the buffer for eight_bits_data
                                                                // out: bytes indicate the length of eight_bits_data
                                  );

/**********************************************************************
    Description: trans string to BCD
    Global resource dependence: none
    return: SCI_SUCCESS...
    Author: @Xiangxin
    Note: 
***********************************************************************/
uint32 PS_TransStrToBCD(
                         char  *bcd_str_ptr,    // in: bcd string
                         uint8 *bcd_data_ptr,   // in/out: buffer for bcd data
                         int16 *bcd_data_len    // in: max bytes of the buffer for bcd data
                                                // out: bytes indicate the length of bcd data
                         );

/**********************************************************************
    Description: Pack 8bits to 7bits
    Global resource dependence: none
    return: SCI_SUCCESS...
    Author: @Xiangxin
    Note: 1) seven_bits_data_offset: (0-7), bit offset means
             how many bits from the lowest bit in a byte
          2) After return from this function, 
             caller can add 0x0d while seven_bits_data_offset(out) equal to 1 if needed
***********************************************************************/
uint32 PS_Pack8bitsTo7bits(
                            uint8 *eight_bits_data_ptr,     // in: data to be packed, which is eight_bits_data
                            int16 eight_bits_data_len,      // in: bytes indicate the length of source data to be packed
                            uint8 *seven_bits_data_ptr,     // in/out: buffer for packed data, which is seven_bits_data
                            int16 *seven_bits_data_len,     // in: max bytes of the buffer for seven_bits_data
                                                            // out: bytes indicate the length of seven_bits_data
                            uint8 *seven_bits_data_offset   // in: bit offset for begin
                                                            // out: bit offset for end
                            );

/**********************************************************************
    Description: trans BCD to string
    Global resource dependence: none
    return: SCI_SUCCESS...
    Author: @Xiangxin
    Note: 
***********************************************************************/
uint32 PS_TransBCDToStr(
                         uint8  *bcd_data_ptr,  // in: bcd data
                         int16  bcd_data_len,   // in: length of bcd data
                         char   *bcd_str_ptr,   // in/out: buffer for bcd string
                         int16  max_bcd_str_len // in: max bytes of the buffer for bcd string
                         );

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  /* PS_CHAR_PACK_H */

