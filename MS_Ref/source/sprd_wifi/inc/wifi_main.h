/*
* Copyright (c) 2020, Unisoc Communications Inc. All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
* the Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
* IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef __WIFI_MAIN_H__
#define __WIFI_MAIN_H__

#include "os_api.h"
#include "wifi_api.h"

typedef struct WIFI_SCAN_CMD_T
{
    uint8 scan_flag;// 1:start; 0:stop 
    uint16 nChNum;
    uint16 nMaxNodeNum;
    uint16 nCh[13];
}WIFI_SCAN_CMD_T;


void wifi_main_init(UAL_WIFI_CALLBACK wifi_callback);

void wifi_main(uint32 argc, void *argv);

#endif /* __WIFI_API_H__ */
