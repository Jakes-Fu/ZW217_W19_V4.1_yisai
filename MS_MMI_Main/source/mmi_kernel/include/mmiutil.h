/******************************************************************************
** File Name:      mmiutil.h                                                  *
** Author:                                                                    *
** Date:            		                                                  *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.          *
** Description:      								                          *
*******************************************************************************
**                         Important Edit History                             *
** ---------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
**                                       									  *
******************************************************************************/


/*!
 *  \addtogroup mmk_mmiutil_group_label
 *  @{
 */


/*! 
 *  \file 	mmiutil.h
 *  \author	
 *  \date	
 *  \brief 	
 */
 
#ifndef MMIUTIL_H
#define MMIUTIL_H

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern   "C"
    {
#endif
/*----------------------------------------------------------------------------*/
/*                         DEPENDANCY                                         */
/*----------------------------------------------------------------------------*/


#include "sci_types.h"
/* Bcd Values for Dial Number storage */
#define   DIALBCD_0             0x0
#define   DIALBCD_1             0x1
#define   DIALBCD_2             0x2
#define   DIALBCD_3             0x3
#define   DIALBCD_4             0x4
#define   DIALBCD_5             0x5
#define   DIALBCD_6             0x6
#define   DIALBCD_7             0x7
#define   DIALBCD_8             0x8
#define   DIALBCD_9             0x9
#define   DIALBCD_STAR          0xA
#define   DIALBCD_HASH          0xB
#define   DIALBCD_PAUSE         0xC
#define   DIALBCD_WILD          0xD
#define   DIALBCD_EXPANSION     0xE
#define   DIALBCD_FILLER        0xF


/*
 * BCD format - LSB first (1234 = 0x21 0x43), MSB first (1234 = 0x12, 0x34)
 * or unpacked (1 digit per byte).
 */
#define PACKED_LSB_FIRST  		0
#define PACKED_MSB_FIRST  		1
#define UNPACKED          		2



#ifdef  MMIUTIL_C
#define EXT
#else
#define EXT     extern
#endif
/*----------------------------------------------------------------------------*/
/*                         FUNCTIONS                                          */
/*----------------------------------------------------------------------------*/
EXT void        mmi_GetBcdLen(uint8 bcdFmt,uint8 *pBcd,uint8 *pLen);
EXT void        mmi_BcdToStr(uint8 bcdFmt,uint8 *pBcd,uint8 bcdLen,char *str);
EXT void        mmi_BcdLfToStr(uint8 *pBcd,uint8 bcdLen,char *str);
EXT void        mmi_BcdMfToStr(uint8 *pBcd,uint8 bcdLen,char *str);
EXT void        mmi_BcdUpToStr(uint8 *pBcd,uint8 bcdLen,char *str);
EXT void        mmi_StrToBcd(uint8 bcdFmt,char *pStr,uint8 *pBcd);
EXT void        mmi_StrToBcdLf(char *pStr,uint8 strLen,uint8 *pBcd);
EXT void        mmi_StrToBcdMf(char *pStr,uint8 strLen,uint8 *pBcd);
EXT void        mmi_StrToBcdUp(char *pStr,uint8 strLen,uint8 *pBcd);


#undef EXT

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus   
    }
#endif


#endif  /* FILENAME_H */
/*! @} */