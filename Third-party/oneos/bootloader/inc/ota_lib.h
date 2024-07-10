/**
***********************************************************************************************************************
* Copyright (c) 2020, China Mobile Communications Group Co.,Ltd.
*
* @file        ota_lib.h
*
* @brief
*
* @revision
* Date         Author       Notes
* 2021-06-16   gongsui      First Version
***********************************************************************************************************************
*/

#ifndef _OTA_LIB_H_
#define _OTA_LIB_H_

#include <stdlib.h>
#include "ota_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef FAL_NAME_MAX
#define FAL_NAME_MAX   24
#endif

typedef enum
{
    FAL_UPGRADE_PART   = 0x00,  /* Upgrade partition  */
    FAL_DOWNLOAD_PART  = 0x01,  /* Download partition */
    FAL_BACKUP_PART    = 0x02,  /* Backup partition   */
} fal_part_type_e;

typedef struct ota_fal_flash_ops
{
    ota_int32(*init)(void);
    ota_int32(*read)(ota_uint32 addr, ota_uint8 *buf, ota_size_t size);         /* Read operation. Success: Return the read data length. Failed: return -1 */
    ota_int32(*write)(ota_uint32 addr, const ota_uint8 *buf, ota_size_t size);  /* write operation. Success: Return the read data length. Failed: return -1 */
    ota_int32(*erase)(ota_uint32 addr, ota_size_t size);                        /* erase operation. Success: return 0. Failed: return -1 */
} ota_fal_flash_ops_t;

typedef struct ota_fal_flash
{
    ota_int8   name[FAL_NAME_MAX];        /* flash name */
    ota_size_t size;                      /* flash size */
    ota_size_t erase_size;                /* Flash minimum erase unit*/
    ota_fal_flash_ops_t ops;              /* Flash read/write/erase operation function */
} ota_fal_flash_t;

typedef struct ota_fal_part_info
{
    ota_int8   name[FAL_NAME_MAX];        /* Partition name */
    ota_int8   flash_name[FAL_NAME_MAX];  /* Flash name of the partition */
    ota_uint32 address;                   /* Partition address */
    ota_uint32 size;                      /* Partition size */
    ota_uint32 part_type;                 /* Partition type */
} ota_fal_part_info_t;

typedef struct ota_fal_map
{
    ota_uint32  logic_addr;     /* Partition logic address */
    ota_uint32  phy_addr;       /* Partition physical address */
} ota_fal_map_t;

/**
 **********************************************************************************************************************
 * @brief           This function for output data. Set it as an empty function when the print is not required
 * @param[in]       fmt     The data format
 * @param[in]       ...     Variable parameter 
 *
 * @retval          void
 **********************************************************************************************************************
 */
void ota_printf(const ota_int8 *fmt, ...);

/**
 **********************************************************************************************************************
 * @brief           This function will get upgrade progress information and customize the display format
 * @param[in]       percentage     Upgrade percentage (0-100]
 *
 * @retval          void
 **********************************************************************************************************************
 */
void ota_update_proc_bar(ota_int32 percentage);

/**
 **********************************************************************************************************************
 * @brief           This function for heap space initialization
 * @param[in]       buf            Heap buffer
 * @param[in]       buf_size       Heap buffer size  (byte)
 * @param[in]       block_size     Minimum unit of heap allocation at one time  (byte)
 *
 * @retval          ota_int32      Success: 0. Failed: -1
 **********************************************************************************************************************
 */
ota_int32 ota_heap_init(ota_uint8 *buf, ota_size_t buf_size, ota_size_t block_size);

/**
 **********************************************************************************************************************
 * @brief           This function for flash partition initialization. Used before the upgrade starts
 * @param[in]       flash_tab          flash configuration table
 * @param[in]       flash_tab_size     flash configuration table size
 * @param[in]       part_tab           Partition configuration table
 * @param[in]       part_tab_size      Partition configuration table size
 *
 * @retval          ota_int32          Success: 0. Failed: -1
 **********************************************************************************************************************
 */
ota_int32 ota_fal_init(ota_fal_flash_t *flash_tab, ota_size_t flash_tab_size, ota_fal_part_info_t *part_tab, ota_size_t part_tab_size);

/**
 **********************************************************************************************************************
 * @brief           This function for fal address mapping initialization, if logic address is used
 * @param[in]       map_table          logic-physical address correspondence table
 * @param[in]       map_table_size     logic-physical address correspondence table size
 *
 * @retval          ota_int32          Success: 0. Failed: -1
 **********************************************************************************************************************
 */
ota_int32 ota_fal_map_init(ota_fal_map_t *map_table, ota_size_t map_table_size);

/**
**********************************************************************************************************************
* @brief           This function for mapping between virtual flash and fal
* @param[in]       vir_flash          the start address of the virtual flash
* @param[in]       size               size of the virtual flash
*
* @retval          ota_int32          Success: 0. Failed: -1
**********************************************************************************************************************
*/
#ifdef CMIOT_VIRTUAL_FILE_SYS
ota_int32 ota_fal_vir_flash_init(ota_uint8 *vir_flash, ota_size_t size);
#endif

/**
 **********************************************************************************************************************
 * @brief           This function will start the upgrade
 * @param[in]       try_times         Number of upgrade retries. At least one retry is advised   
 *
 * @retval          ota_int32         Upgrade status code
 **********************************************************************************************************************
 */
ota_int32 ota_start_up(ota_uint32 try_times);

#ifdef __cplusplus
}
#endif

#endif
