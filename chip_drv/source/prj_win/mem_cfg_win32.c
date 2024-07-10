/******************************************************************************
 ** File Name:      mem_cfg_sp7100.c                                          *
 ** Author:         Lin.liu                                                   *
 ** DATE:           2004-04-19                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the memory config of SP7100             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/19/2004     Lin.liu          Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "threadx_os.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#ifdef NEW_MEMORY_MM

#include "new_mem_cfg_win32.c"

#else

#define APP_MEM_POOL_BUF_SIZE   32*180 //5*180:MAX_APP_MEM_COUNT*sizeof(APP_MEM_T)
CONST uint32 g_app_mem_pool_buf_size = APP_MEM_POOL_BUF_SIZE;
PUBLIC uint32 g_app_mem_pool_buf[APP_MEM_POOL_BUF_SIZE/4] = {NULL};

#define FLASH_AMD_64M
//#define SRAM_2M
#include "mem_cfg_template.h"

const uint32 g_file_system_file_num         = 128;
const uint32 g_file_system_data_block_num   = 512;

const uint32 g_nvitem_cache_enabled = 0;

//===============================
// Be careful, the array below must be 4 bytes alignment.
// Byte heap size

/*Define the memory threshod size in the byte heap, when the allocation memory size less than
the threshod, it will be allocated from the heap low address, or it will be allocated from the
heap high address*/
#define SCI_STATIC_HEAP_SIZE_THRESHOD     0x14000   //80KB
CONST uint32  g_static_heap_size_threshod = SCI_STATIC_HEAP_SIZE_THRESHOD;

#define SCI_BASE_HEAP_SIZE_THRESHOD     0x1400   //5KB
CONST uint32  g_base_heap_size_threshod = SCI_BASE_HEAP_SIZE_THRESHOD;

#define SCI_APP_HEAP_SIZE_THRESHOD     0x14000   //80KB
CONST uint32  g_app_heap_size_threshod = SCI_APP_HEAP_SIZE_THRESHOD;


#define    MAX_STATIC_HEAP_SIZE     (560*1024)
#define    MAX_DYNAMIC_BASE_HEAP_SIZE  (50*1024)
#define    MAX_DYNAMIC_APP_HEAP_SIZE  (50000 * 1024)
#define BYTE_HEAP_SIZE    (MAX_STATIC_HEAP_SIZE + MAX_DYNAMIC_BASE_HEAP_SIZE + MAX_DYNAMIC_APP_HEAP_SIZE)

//@Daniel.Luo for pool optimization
//Be careful the pool size sequence must be ordered in ascending
#ifndef ATEST_SUPPORT
POOL_INFO_T pool_cfg[] =
{
    {32,   1500},
    {56,   900},
    {116,  300},
    {196,  300},
    {320,  300},
    {600,  300},
    {2080, 1000},
    {5156, 50 }
};
#else
POOL_INFO_T pool_cfg[] =
{
    {32,   1500},
    {56,   900},
    {116,  3000},
    {196,  3000},
    {320,  3000},
    {600,  3000},
    {2080, 1000},
    {5156, 5000}
};
#endif//ATEST_SUPPORT

#define POOL_TYPE_NUM   (sizeof(pool_cfg)/sizeof(POOL_INFO_T))
PUBLIC const uint32 pool_type_num = POOL_TYPE_NUM;
PUBLIC SCI_BLOCK_POOL_T pool[POOL_TYPE_NUM];
PUBLIC char pool_name[POOL_TYPE_NUM][32] = {0};
const uint32 static_byte_heap_size     =  MAX_STATIC_HEAP_SIZE;
const uint32 dynamic_byte_base_heap_size     =  MAX_DYNAMIC_BASE_HEAP_SIZE;
const uint32 dynamic_byte_app_heap_size     =  MAX_DYNAMIC_APP_HEAP_SIZE;
const uint32 byte_head_size     =  BYTE_HEAP_SIZE;
uint32    BYTE_HEAP_ADDR[ BYTE_HEAP_SIZE >> 2 ];

#ifdef SCI_MEMORY_DEBUG
#define POOL_XX_SIZE(size, num)  ((num * (size + sizeof(MEM_HEADER_T) + sizeof(void *))) >> 2)


#define POOL_COUNT_STEP     32
#define POOL_COUNTER_NUM    162     // the max pool
PUBLIC uint32 pool_counter [POOL_COUNTER_NUM] = {0};
PUBLIC uint32 step_released[POOL_COUNTER_NUM] = {0};
PUBLIC uint32 step_max_used[POOL_COUNTER_NUM] = {0};
const uint32 pool_count_step  = POOL_COUNT_STEP;
const uint32 pool_counter_num = POOL_COUNTER_NUM;

uint32 pool_max_used[POOL_TYPE_NUM];
#else
#define POOL_XX_SIZE(size, num)  ((num * (size + sizeof(void *))) >> 2)
#endif  // End of SCI_MEMORY_DEBUG


//following array sequence must be consist with pool_cfg
#ifndef ATEST_SUPPORT
LOCAL uint32 pool_1_addr[POOL_XX_SIZE (32,   1500) ];
LOCAL uint32 pool_2_addr[POOL_XX_SIZE (56,   900) ];
LOCAL uint32 pool_3_addr[POOL_XX_SIZE (116,  300) ];
LOCAL uint32 pool_4_addr[POOL_XX_SIZE (196,  300) ];
LOCAL uint32 pool_5_addr[POOL_XX_SIZE (320,  300) ];
LOCAL uint32 pool_6_addr[POOL_XX_SIZE (600,  300) ];
LOCAL uint32 pool_7_addr[POOL_XX_SIZE (2080, 1000) ];
LOCAL uint32 pool_8_addr[POOL_XX_SIZE (5156, 50) ];
#else
LOCAL uint32 pool_1_addr[POOL_XX_SIZE (32,   1500) ];
LOCAL uint32 pool_2_addr[POOL_XX_SIZE (56,   900) ];
LOCAL uint32 pool_3_addr[POOL_XX_SIZE (116,  3000) ];
LOCAL uint32 pool_4_addr[POOL_XX_SIZE (196,  3000) ];
LOCAL uint32 pool_5_addr[POOL_XX_SIZE (320,  3000) ];
LOCAL uint32 pool_6_addr[POOL_XX_SIZE (600,  3000) ];
LOCAL uint32 pool_7_addr[POOL_XX_SIZE (2080, 1000) ];
LOCAL uint32 pool_8_addr[POOL_XX_SIZE (5156, 5000) ];
#endif//ATEST_SUPPORT

uint32 *pool_addr[POOL_TYPE_NUM] =
{
    pool_1_addr,
    pool_2_addr,
    pool_3_addr,
    pool_4_addr,
    pool_5_addr,
    pool_6_addr,
    pool_7_addr,
    pool_8_addr
};


#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
