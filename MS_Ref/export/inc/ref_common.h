/******************************************************************************
 ** File Name:      ref_param.h                                               *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           09/03/2003                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file contain some Ref parameter.                     *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 09/03/2003     Xueliang.Wang    Create.                                   *
 ******************************************************************************/
#ifndef __REF_COMMON_H__
#define __REF_COMMON_H__

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h" 

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**
  * crc16 - compute the CRC-16 for the data buffer
  * @crc:        previous CRC value
  * @buffer:     data pointer
  * @len:        number of bytes in the buffer
  * note:        calculate the CRC16  polynomial is 0x8005
  * Returns the updated CRC value.
  */

 uint16 REF_Crc16(uint16 crc, uint8 const *buffer, uint32 len);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  // __REF_COMMON_H__
