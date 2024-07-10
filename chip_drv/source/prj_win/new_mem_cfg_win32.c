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
#define SCI_STATIC_HEAP_SIZE_THRESHOD         0x14000   //80KB
#define     SCI_APP_HEAP_SIZE_THRESHOD        0x14000     //160KB


#define     SCI_SYSTEM_SPACE_ALLOC_THRESHOD   0x4000      //16KB
#define     SCI_SYSTEM_MMAP_THRESHOLD         0x10000     //64KB


#define  	MAX_STATIC_SPACE_SIZE             (5*1024*1024) //SFS decrease memory 300K, Alarm decrease memory 30K
#define    	MAX_SYSTEM_SPACE_SIZE  	          (50*1024 * 1024)
#define    	MAX_BASE_SPACE_SIZE  	          (10*1024 * 1024)
#define    	MAX_APP_SPACE_SIZE  	          (20*1024 * 1024)
#define 	BYTE_HEAP_SIZE    			      (MAX_STATIC_SPACE_SIZE  + MAX_SYSTEM_SPACE_SIZE + MAX_BASE_SPACE_SIZE + MAX_APP_SPACE_SIZE)


/*Define the memory threshod size in the byte heap, when the allocation memory size less than 
	  the threshod, it will be allocated from the heap low address, or it will be allocated from the
	  heap high address*/
CONST uint32  g_static_heap_size_threshod = SCI_STATIC_HEAP_SIZE_THRESHOD;
CONST uint32  g_app_heap_size_threshod    = SCI_APP_HEAP_SIZE_THRESHOD;
CONST uint32  g_system_space_alloc_threshod    = SCI_SYSTEM_SPACE_ALLOC_THRESHOD;
CONST uint32  g_system_mmap_threshod      = SCI_SYSTEM_MMAP_THRESHOLD;

CONST uint32 max_static_space_size  =  MAX_STATIC_SPACE_SIZE;
CONST uint32 max_system_space_size  =  MAX_SYSTEM_SPACE_SIZE;
CONST uint32 max_app_space_size     =  MAX_APP_SPACE_SIZE;
CONST uint32 byte_head_size     	=  BYTE_HEAP_SIZE;
CONST uint32 max_base_space_size    =  MAX_BASE_SPACE_SIZE;
uint32    BYTE_HEAP_ADDR[ BYTE_HEAP_SIZE >> 2 ];







/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
