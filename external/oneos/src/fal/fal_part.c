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
 * @file        fal_part.c
 *
 * @brief       fal_part
 *
 * @revision
 * Date         Author          Notes
 * 2020-02-20   OneOS Team      First Version
 ***********************************************************************************************************************
 */


#include "fal.h"
#include "fal_part.h"
#include "sci_types.h"
#include "flash_drvapi.h"

extern const  NOR_FLASH_CONFIG_T  s_platform_patitiion_config;
static fal_part_info_t g_app_info;
static fal_flash_t g_flash;
static fal_part_t g_fal_table;
static os_bool_t g_fal_init = OS_FALSE;

void fal_part_init()
{
    g_flash.block_size = s_platform_patitiion_config.sect_size;
    g_flash.page_size = 0;
    g_flash.capacity = s_platform_patitiion_config.flash_size;

    #ifdef FOTA_SUPPORT_CMIOT
    memcpy(g_app_info.name, "download", sizeof("download"));
    g_app_info.offset = s_platform_patitiion_config.fota_addr;
    g_app_info.size = s_platform_patitiion_config.fota_size;
    #else
    memset(g_app_info.name, 0, sizeof(g_app_info.name));
    g_app_info.offset = 0xFFFFFFFF;
    g_app_info.size = 0xFFFFFFFF;
    #endif

    g_fal_table.flash = &g_flash;
    g_fal_table.info = &g_app_info;

    g_fal_init = OS_TRUE;
}
/**
 ***********************************************************************************************************************
 * @brief           find the fal part info according the name of flash part
 *
 * @param[in]       name: the name of the flash part
 *
 * @return          fal_part_t *: see the structure description
 ***********************************************************************************************************************
 */
fal_part_t *fal_part_find(const char *name)
{
    if (g_fal_init == OS_FALSE)
    {
        fal_part_init();
    }
    if (strcmp(name, g_app_info.name))
    {
        return OS_NULL;
    }
    return &g_fal_table;
}

/**
 ***********************************************************************************************************************
 * @brief           read the data of part by offset address
 *
 * @param[in]       part: the name of the flash part
 *                  addr: offset address of the part
 *                  buf: save the data
 *                  size: read size
 *
 * @return          int
 ***********************************************************************************************************************
 */
int fal_part_read(fal_part_t *part, os_uint32_t addr, os_uint8_t *buf, os_size_t size)
{
    int len;
    os_uint32_t real_addr;
    if (part == OS_NULL)
    {
        return 0;
    }
    real_addr = addr + part->info->offset;
    if (FLASH_Read(real_addr, buf, size))
    {
        return size;
    }
    else
    {
        for(len=0; *(buf+len)!=NULL; len++);
        return len;
    }
}

/**
 ***********************************************************************************************************************
 * @brief           write the data in part by offset address
 *
 * @param[in]       part: the name of the flash part
 *                  addr: offset address of the part
 *                  buf: the data to be written
 *                  size: write data size
 *
 * @return          int
 ***********************************************************************************************************************
 */
int fal_part_write(fal_part_t *part, os_uint32_t addr, const os_uint8_t *buf, os_size_t size)
{
    os_uint32_t real_addr;
    if (part == OS_NULL)
    {
        return 0;
    }
    real_addr = addr + part->info->offset;
    if(FLASH_Write(real_addr, buf, size))
    {
        return size;
    }
    else
    {
        return 0;
    }
}

/**
 ***********************************************************************************************************************
 * @brief           erase the part by offset address and len
 *
 * @param[in]       part: the name of the flash part
 *                  addr: offset address of the part
 *                  size: erase size
 *
 * @return          int
 ***********************************************************************************************************************
 */
int fal_part_erase(fal_part_t *part, os_uint32_t addr, os_size_t size)
{
    os_uint32_t real_addr;
    os_uint32_t block_size;
    os_uint32_t block_num = 0;
    int index;
    if (part == OS_NULL)
    {
        return 0;
    }
    real_addr = addr + part->info->offset;
    block_size = part->flash->block_size;
    if (size == 0)
    {
        FLASH_Erase(real_addr);
    }
    else
    {
        block_num = (size % block_size) ? (size / block_size + 1) : (size / block_size);
        for (index = 0; index < block_num; index++)
        {
            if(!FLASH_Erase(real_addr + block_size * index)){
                // CMIOT_DEBUG("[CMIOT]", "erase failed on %#X", real_addr+block_size*index);
            }
        }
    }
    return 0;
}
