/******************************************************************************
 ** File Name:      cyclic_computer.c                                                     *
 ** Author:         Richard Yang                                              *
 ** DATE:           22/08/2001                                                *
 ** Copyright:      2001 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 22/08/2001     Richard.Yang     Create.                                   *
 ******************************************************************************/
#include "sci_types.h"
#include "ref_common.h"
 
 /**
  * crc16 - compute the CRC-16 for the data buffer
  * @crc:        previous CRC value
  * @buffer:     data pointer
  * @len:        number of bytes in the buffer
  * note:        calculate the CRC16  polynomial is 0x8005
  * Returns the updated CRC value.
  */

 uint16 crc16(uint16 crc, uint8 const *buffer, uint32 len)
 {
    uint16 crc2 = 0;
    uint16 c    = 0;
    uint16 j    = 0;

    while (len--)
    {
        crc2 = 0;
        c = (crc^ (*buffer++)) &0xff;

        for (j=0; j<8; j++)
        {

            if ( (crc2 ^ c) & 0x0001)
            {
                crc2 = (crc2 >> 1) ^ 0xA001;
            }
            else
            {
                crc2 =   crc2 >> 1;
            }

            c = c >> 1;
        }

        crc = (uint16) ( (crc >> 8) ^crc2);
    }

    return crc;
 }


uint16 REF_Crc16(uint16 crc, uint8 const *buffer, uint32 len)
{
	return  crc16(crc, buffer, len);
}
 