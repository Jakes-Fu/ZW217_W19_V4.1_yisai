/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#ifndef _AUD_RESAMPLE_H
#define _AUD_RESAMPLE_H

#include "os_api.h"
//#include <string.h>
//
#ifdef __cplusplus
extern "C" {
#endif

typedef struct audResamCtx
{
    uint32 playrate;     //output sample rate
    int16 playrateStep;  //output step
    int16 playScale;     //scale factor for calculate
    int16 intpoint;      //input sample index
    int16 floatpoint;    //resample float index
    int16 dataratestep;  //input step
    int16 blocklength;   //input sample number
    int16 historyData_l; //history L sample
    int16 historyData_r; //history R sample
    uint16 chl;          //channel. 1 or 2. output same as input.
} audResamCtx_t;

int32 audioResampleInitParam(audResamCtx_t *ptTransamObj, uint32 inRate, uint32 outRate, uint16 chl);
uint32 audioResample(audResamCtx_t *ptTransamObj, int16 *inBuf, int16 inum, int16 *outBuf);

#ifdef __cplusplus
}
#endif

#endif
