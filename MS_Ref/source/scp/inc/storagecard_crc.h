/******************************************************************************
 ** File Name:      StorageCard_crc.h                                                 *
 ** Author:         Zhemin.Lin                                                *
 ** DATE:           11/11/2003                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    define crc16 and crc7 operation interface                 *
 ******************************************************************************/

#ifndef _SC_CRC_H_
#define _SC_CRC_H_

PUBLIC uint8 SC_GetCrc7(uint8 *pdata, uint32 len);
PUBLIC uint16 SC_GetCrc16(uint8* pdata, uint32 len);

#endif