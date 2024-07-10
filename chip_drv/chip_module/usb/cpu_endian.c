/******************************************************************************
 ** File Name:      cpu_endian.c                                                 *
 ** Author:         JiaYong.Yang                                              *
 ** DATE:           09/01/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "sci_types.h"


PUBLIC  uint32 cpu_to_be32 (uint32 dword)
{
#ifdef CHIP_ENDIAN_LITTLE
    return ( ( ( (dword) <<24) &0xff000000L) | ( ( (dword) <<8) &0x00ff0000L) | ( ( (dword) >>8) &0x0000ff00L) | ( ( (dword) >>24) &0x000000ffL));
#else
    return dword;
#endif
}
PUBLIC  uint32 cpu_to_le32 (uint32 dword)
{
#ifndef CHIP_ENDIAN_LITTLE
    return ( ( ( (dword) <<24) &0xff000000L) | ( ( (dword) <<8) &0x00ff0000L) | ( ( (dword) >>8) &0x0000ff00L) | ( ( (dword) >>24) &0x000000ffL));
#else
    return dword;
#endif
}
PUBLIC  uint32 le32_to_cpu (uint32 dword)
{
#ifndef CHIP_ENDIAN_LITTLE
    return ( ( ( (dword) <<24) &0xff000000L) | ( ( (dword) <<8) &0x00ff0000L) | ( ( (dword) >>8) &0x0000ff00L) | ( ( (dword) >>24) &0x000000ffL));
#else
    return dword;
#endif
}
PUBLIC  uint16 cpu_to_le16 (uint16 word)
{

#ifndef CHIP_ENDIAN_LITTLE
    return ( ( ( (word) <<8) &0xff00) | ( ( (word) >>8) &0x00ff));
#else
    return  word;
#endif

}
PUBLIC  uint16 le16_to_cpu (uint16 word)
{
#ifndef CHIP_ENDIAN_LITTLE

    return ( ( ( (word) <<8) &0xff00) | ( ( (word) >>8) &0x00ff));
#else
    return word;
#endif
}
