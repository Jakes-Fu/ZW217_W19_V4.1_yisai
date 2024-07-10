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
#include "crc.h" 




/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     uint16 crc_16_l_calc()                                                  *  
 **                                                                           *	 
 ** DESCRIPTION                                                               *
 **     Cacualte the CRC value.                                               *
 **                                                                           *
 ** INPUT                                                                     *
 **     src:    Pointer of the input buffer.                                  *
 **     size:   Size of the input buffer by uint8.                             *
 **                                                                           *
 ** OUTPUT                                                                    *
 **     None                                                                  *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     16 bit CRC value.                                                     *
 **                                                                           *	 
 ** DEPENDENCIES                                                              *
 **     None                                                                  *
 **---------------------------------------------------------------------------*/

uint16 crc_16_l_calc(const uint8 *src, uint16 size)
{
  unsigned int    i;
    unsigned short  crc = 0;

    while (size--)
    {
        for (i = CRC_16_L_SEED; i != 0 ; i >>= 1)
        {
            if (crc & CRC_16_L_POLYNOMIAL)
            {
                crc <<= 1;
                crc ^= CRC_16_POLYNOMIAL;
            }
            else
            {
                crc <<= 1;
            }

            if (*src & i)
            {
                crc ^= CRC_16_POLYNOMIAL;
            }
        }

        ++src;
    }

    return (crc);
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif /* End crc.h*/
