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
 * @file        cms_license.h
 *
 * @brief       Provides the license common struture and macron macro.
 *
 * @revision
 * Date         Author          Notes
 * 2020-02-20   OneOS Team      First Version
 ***********************************************************************************************************************
 */

#ifndef __CMS_LICENSE_H__
#define __CMS_LICENSE_H__

#include <stdint.h>
#include <string.h>
#include "os_util.h"

#define DBG_TAG   "[cms.license]"
//#define LOG_E(tag,fmt, ...)                  printf(fmt)
#define DEBUG
#if defined(DEBUG)
#define LOG_I(DBG_TAG, fmt, ...)          os_kprintf("I/ %s" fmt " at %s@Line%u\n", DBG_TAG, ##__VA_ARGS__, __PRETTY_FUNCTION__, __LINE__)
#define LOG_D(DBG_TAG, fmt, ...)          os_kprintf("D/ %s" fmt " at %s@Line%u\n", DBG_TAG, ##__VA_ARGS__, __PRETTY_FUNCTION__, __LINE__)
#define LOG_E(DBG_TAG, fmt, ...)          os_kprintf("E/ %s" fmt " at %s@Line%u\n", DBG_TAG, ##__VA_ARGS__, __PRETTY_FUNCTION__, __LINE__)
#define LOG_W(DBG_TAG, fmt, ...)          os_kprintf("W/ %s" fmt " at %s@Line%u\n", DBG_TAG, ##__VA_ARGS__, __PRETTY_FUNCTION__, __LINE__)
#else
#define LOGI(TAG, fmt, ...)
#define LOGD(TAG, fmt, ...)
#define LOGE(TAG, fmt, ...)
#define LOGW(TAG, fmt, ...)
#endif

#ifndef KEY_PARA_MAX_LEN
#define KEY_PARA_MAX_LEN    94
#endif

#define COMPONENT_RTC  1003

typedef enum
{
    CMS_LICENSE_CMD_NON             = 0x00U,    /**/
    CMS_LICENSE_CMD_HID             = 0x01U,    /**/
    CMS_LICENSE_CMD_WRITE           = 0x02U,    /**/
    CMS_LICENSE_CMD_ASK             = 0x03U,    /**/
} CMS_LICENSE_CMD;

typedef enum
{
    CMS_LICENSE_TYPE_REQUEST        = 0x00U,    /**/
    CMS_LICENSE_TYPE_NON            = 0x40U,    /**/
    CMS_LICENSE_TYPE_RESOPND        = 0x80U,    /**/
    CMS_LICENSE_TYPE_RST            = 0xC0U,
} CMS_LICENSE_CMD_TYPE;

typedef struct license
{
    char flash_PART_name[16];
    uint32_t offset;
    uint16_t componet_id;
    uint16_t license_len;
} license_t;

typedef struct payload
{
    uint16_t command;
    uint16_t length;
    uint16_t componet_id;
    uint8_t  *buff;
} payload_t;

#endif
