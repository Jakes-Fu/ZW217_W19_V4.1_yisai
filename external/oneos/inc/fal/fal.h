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
 * @file        fal.h
 *
 * @brief       fal
 *
 * @revision
 * Date         Author          Notes
 * 2020-02-20   OneOS Team      First Version
 ***********************************************************************************************************************
 */

#ifndef __FAL_H__
#define __FAL_H__

#include "os_types.h"

#ifndef FAL_DEV_NAME_MAX
#define FAL_DEV_NAME_MAX 16
#endif

typedef struct fal_flash fal_flash_t;

struct fal_flash_ops
{
    int (*read_page)(fal_flash_t *flash, os_uint32_t page_addr, os_uint8_t *buff, os_uint32_t page_nr);
    int (*write_page)(fal_flash_t *flash, os_uint32_t page_addr, const os_uint8_t *buff, os_uint32_t page_nr);
    int (*erase_block)(fal_flash_t *flash, os_uint32_t page_addr, os_uint32_t page_nr);
};

/**
 ***********************************************************************************************************************
 * @struct      os_list_node
 *
 * @brief       Double List structure
 ***********************************************************************************************************************
 */
struct os_list_node
{
    struct os_list_node *next;      /* Point to next node */
    struct os_list_node *prev;      /* point to previous node */
};
typedef struct os_list_node os_list_node_t;

struct fal_flash
{
    char name[FAL_DEV_NAME_MAX];  /* the name of the flash */

    os_uint32_t page_size;      /* the min of read/write size measured in bytes */
    os_uint32_t block_size;     /* the min of flash erase size measured in bytes */
    os_uint32_t capacity;       /* flash capacity measured in bytes */

    struct fal_flash_ops ops;

    os_list_node_t list;

    void *priv;
};

// struct onchip_flash_info
// {
//     os_uint32_t start_addr;
//     os_uint32_t capacity;
//     os_uint32_t block_size;
//     os_uint32_t page_size;
// };

// int fal_flash_register(fal_flash_t *flash);
// fal_flash_t *fal_flash_find(const char *name);
// os_mtd_device_t *fal_mtd_device_create(const char *parition_name);
// os_blk_device_t *fal_blk_device_create(const char *parition_name);

#define fal_block_shift(flash_dev)   (os_ffs(flash_dev->block_size)-1)
#define fal_page_shift(flash_dev)    (os_ffs(flash_dev->page_size)-1)

#endif    /* __FAL_H__ */
