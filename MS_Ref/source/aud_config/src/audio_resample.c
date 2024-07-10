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

#include "os_api.h"
#include "audio_resample.h"

//init must be call once before a new music stream.
//inRate:input pcm sample rate.
//outRate:output pcm sample rate.
//chl: channel,1 or 2. output same as input.
//return :0 success,other fail.
int32 audioResampleInitParam(audResamCtx_t *ptTransamObj, uint32 inRate, uint32 outRate, uint16 chl)
{
    if (!ptTransamObj)
        return -1;
    if (inRate == 0 || outRate == 0)
    {
        ptTransamObj->playrate = 0; //reuse for init juage.
        return -1;
    }
    //Init Global variables
    ptTransamObj->playrate = outRate;
    ptTransamObj->playrateStep = outRate / 25; //* 40/1000;
    if (ptTransamObj->playrateStep == 0)
    {
        ptTransamObj->playrate = 0; //reuse for init juage.
        return -1;
    }
    ptTransamObj->playScale = 16384 / ptTransamObj->playrateStep;
    ptTransamObj->dataratestep = inRate / 25; // 40/1000;
    if (ptTransamObj->dataratestep == 0)
    {
        ptTransamObj->playrate = 0; //reuse for init juage.
        return -1;
    }
    ptTransamObj->intpoint = 0;
    ptTransamObj->floatpoint = 0;
    ptTransamObj->blocklength = 0;

    ptTransamObj->historyData_l = 0;
    ptTransamObj->historyData_r = 0;
    if (chl != 1 && chl != 2)
    {
        ptTransamObj->playrate = 0; //reuse for init juage.
        return -1;
    }
    ptTransamObj->chl = chl;

    return 0;
}

//inBuf:input pcm buf
//inum:input sample count.must be multiple of 2.
//outBuf:output pcm buf.user must ensure buffer size enough for up/down resample.
//return :output sample count.maybe odd or even.
uint32 audioResample(audResamCtx_t *ptTransamObj, int16 *inBuf, int16 inum, int16 *outBuf)
{
    int32 ltemp, rtemp;
    int16 op = 0;

    if (!ptTransamObj || !inBuf || inum <= 0 || !outBuf)
        return 0;
    //playrate reuse for init juage.
    if (ptTransamObj->playrate == 0)
    { //init fail.
        return 0;
    }

    ptTransamObj->blocklength = inum;

    if (ptTransamObj->chl == 2)
    {
        while (ptTransamObj->intpoint < ptTransamObj->blocklength)
        {
            //OSI_LOGI(0xffffffff, "chl2 intpoint=%d blocklen=%d",ptTransamObj->intpoint,ptTransamObj->blocklength);
            //left
            if (0 == ptTransamObj->intpoint)
            {
                ltemp = (ptTransamObj->historyData_l * (ptTransamObj->playrateStep - ptTransamObj->floatpoint) + inBuf[ptTransamObj->intpoint * 2] * ptTransamObj->floatpoint) * ptTransamObj->playScale / 16384;
            }
            else
            {
                ltemp = (inBuf[(ptTransamObj->intpoint - 1) * 2] * (ptTransamObj->playrateStep - ptTransamObj->floatpoint) + inBuf[ptTransamObj->intpoint * 2] * ptTransamObj->floatpoint) * ptTransamObj->playScale / 16384;
            }
            //right
            if (0 == ptTransamObj->intpoint)
            {
                rtemp = (ptTransamObj->historyData_r * (ptTransamObj->playrateStep - ptTransamObj->floatpoint) + inBuf[ptTransamObj->intpoint * 2 + 1] * ptTransamObj->floatpoint) * ptTransamObj->playScale / 16384;
            }
            else
            {
                rtemp = (inBuf[(ptTransamObj->intpoint - 1) * 2 + 1] * (ptTransamObj->playrateStep - ptTransamObj->floatpoint) + inBuf[ptTransamObj->intpoint * 2 + 1] * ptTransamObj->floatpoint) * ptTransamObj->playScale / 16384;
            }
            //------------------------------------------------------------------------
            outBuf[op * 2] = ltemp;
            outBuf[op * 2 + 1] = rtemp;
            op = op + 1;

            //
            ptTransamObj->floatpoint = ptTransamObj->floatpoint + ptTransamObj->dataratestep;
            while (ptTransamObj->floatpoint >= ptTransamObj->playrateStep)
            {
                //OSI_LOGI(0xffffffff, "chl2 floatpoint=%d playstep=%d",ptTransamObj->floatpoint,ptTransamObj->playrateStep);
                ptTransamObj->floatpoint = ptTransamObj->floatpoint - ptTransamObj->playrateStep;
                ptTransamObj->intpoint = ptTransamObj->intpoint + 1;
            }
        }

        //save the old data for next time
        ptTransamObj->historyData_l = inBuf[(ptTransamObj->blocklength - 1) * 2];
        ptTransamObj->historyData_r = inBuf[(ptTransamObj->blocklength - 1) * 2 + 1];

        //OSI_LOGI(0xffffffff, "chl2 intpoint=%d blocklen=%d op=%d l=0x%x r=0x%x",ptTransamObj->intpoint,ptTransamObj->blocklength,op,
        //    ptTransamObj->historyData_l,ptTransamObj->historyData_r);
        //OSI_LOGI(0xffffffff, "chl2 datastep=%d playstep=%d",ptTransamObj->dataratestep,ptTransamObj->playrateStep);
    }
    else if (ptTransamObj->chl == 1)
    { //Mono
        while (ptTransamObj->intpoint < ptTransamObj->blocklength)
        {
            //OSI_LOGI(0xffffffff, "chl1 intpoint=%d blocklen=%d",ptTransamObj->intpoint,ptTransamObj->blocklength);
            //Mono
            if (0 == ptTransamObj->intpoint)
            {
                ltemp = (ptTransamObj->historyData_l * (ptTransamObj->playrateStep - ptTransamObj->floatpoint) + inBuf[ptTransamObj->intpoint] * ptTransamObj->floatpoint) * ptTransamObj->playScale / 16384;
            }
            else
            {
                ltemp = (inBuf[ptTransamObj->intpoint - 1] * (ptTransamObj->playrateStep - ptTransamObj->floatpoint) + inBuf[ptTransamObj->intpoint] * ptTransamObj->floatpoint) * ptTransamObj->playScale / 16384;
            }
            //------------------------------------------------------------------------
            outBuf[op] = ltemp;
            op = op + 1;

            //
            ptTransamObj->floatpoint = ptTransamObj->floatpoint + ptTransamObj->dataratestep;
            while (ptTransamObj->floatpoint >= ptTransamObj->playrateStep)
            {
                //OSI_LOGI(0xffffffff, "chl1 floatpoint=%d playstep=%d",ptTransamObj->floatpoint,ptTransamObj->playrateStep);
                ptTransamObj->floatpoint = ptTransamObj->floatpoint - ptTransamObj->playrateStep;
                ptTransamObj->intpoint = ptTransamObj->intpoint + 1;
            }
        }
        //save the old data for next time
        ptTransamObj->historyData_l = inBuf[ptTransamObj->blocklength - 1];
        //OSI_LOGI(0xffffffff, "chl1 intpoint=%d blocklen=%d op=%d l=0x%x r=0x%x",ptTransamObj->intpoint,ptTransamObj->blocklength,op,
        //    ptTransamObj->historyData_l,ptTransamObj->historyData_r);
        //OSI_LOGI(0xffffffff, "chl1 datastep=%d playstep=%d",ptTransamObj->dataratestep,ptTransamObj->playrateStep);
    }
    //adjust the intpoint
    ptTransamObj->intpoint = ptTransamObj->intpoint - ptTransamObj->blocklength;

    return op;
}
