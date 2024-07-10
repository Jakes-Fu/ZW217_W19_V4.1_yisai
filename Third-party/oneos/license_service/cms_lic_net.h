/**
 ***********************************************************************************************************************
 * Copyright (c) 2020, China Mobile Communications Group Co.,Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with 
 * the License. You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
 * an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations under the License.
 *
 * @file        cms_lic_online.h
 *
 * @brief       The header file for cms_lic_online.c
 *
 * @revision
 * Date         Author          Notes
 * 2021-12-1    OneOS Team      First Version
 ***********************************************************************************************************************
 */
 
#ifndef __CMS_LIC_NET_H__
#define __CMS_LIC_NET_H__

#include "cms_license.h"


int lic_online_apply_license(uint16_t component_id);

typedef struct rtc_lic{
    uint8_t *lic;
    uint16_t len;
}RTC_LIC_ST;

extern RTC_LIC_ST g_license;

#endif
