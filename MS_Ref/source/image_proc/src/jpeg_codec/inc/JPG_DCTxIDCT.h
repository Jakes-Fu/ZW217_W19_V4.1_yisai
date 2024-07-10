/******************************************************************************
 ** File Name:      transform.h                                                   *
 ** Author:         Yi.Wang                                                *
 ** Date:           2007/04/28                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    define the DCT/IDCT transform function*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2007/04/28     Yi.Wang          Create.                                   *
 ******************************************************************************/
#ifndef TRANSFORM_H
#define TRANSFORM_H

#define W1 2841 /* 2048*sqrt(2)*cos(1*pi/16) */
#define W2 2676 /* 2048*sqrt(2)*cos(2*pi/16) */
#define W3 2408 /* 2048*sqrt(2)*cos(3*pi/16) */
#define W5 1609 /* 2048*sqrt(2)*cos(5*pi/16) */
#define W6 1108 /* 2048*sqrt(2)*cos(6*pi/16) */
#define W7 565  /* 2048*sqrt(2)*cos(7*pi/16) */

#include "sci_types.h"

void JPEG_SoftDCT(uint8 * block, int32 * rgiDst);
void JPEG_SoftIDCT(int32 * block, uint8 * pixelData, uint16* pQuant);
void Initialize_Clip();

extern uint8	iclip[];
extern uint8	*pClip_table;
#endif