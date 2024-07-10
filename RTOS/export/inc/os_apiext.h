/******************************************************************************
 ** File Name:      os_apiext.h                                               *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           11/14/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic Application Interface (API)   *
 **                 to the high-performance RTOS.                             *
 **                 Some service prototypes and some data structure for       *
 **                 internal are defined in this file.                        *
 ******************************************************************************
 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/14/2002     Xueliang.Wang    Create.                                   *
 ******************************************************************************/
#ifndef _OS_APIEXT_H
#define _OS_APIEXT_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

// Indentification of a thread.
#define SCI_STATIC_THREAD           0x20021201    // Static thread
#define SCI_DYNAMIC_THREAD          0x20021202    // Dynamic thread 
#define KAL_STATIC_THREAD           0x20021204
#define KAL_DYNAMIC_THREAD          0x20021208
#define KAL_INVALID_THREAD          0x0
#define SCI_INVALID_THREAD          0x0

// Used to generate dynamic thread id.
// Dynamic thread id is the control block address plus SCI_MIN_DYNAMIC_ID.
#define SCI_MIN_DYNAMIC_THREAD_ID   0x1000

// We use this value to fullfill our stack, so that we can check our stack
// usage information when run the program.
#define MEM_INIT_VALUE              0x5A

// IRQ max nest.
#define SCI_MAX_IRQ_NESTING         10
#define SCI_MAX_FIQ_NESTING         10


// Send option on queue.
#define SCI_PRI_NORMAL              0
#define SCI_PRI_FRONT               1

// Macro for queue item size.
#define SCI_1_ULONG                 0
#define SCI_2_ULONG                 1
#define SCI_4_ULONG                 2
#define SCI_8_ULONG                 3
#define SCI_16_ULONG                4

/**---------------------------------------------------------------------------*/
// Type define.
/**---------------------------------------------------------------------------*/
typedef struct RETURN_VALUE_tag
{
    uint32  kernel_value;
    uint32  api_value;
    char    value_str[SCI_MAX_NAME_SIZE];
} RETURN_VALUE_T;

/*
 * Added by Eddie.Li for support realtime debug on OS.
 */
#define MEM_INFO_CONTINUE    0
#define MEM_INFO_END         1

#define MAX_MEM_INFO_NUM  (100)

/* Define for the realtime to get memory info. */
typedef enum mem_info_getting_status
{
    MEM_INFO_GETTING_INVALID    = 0,
    MEM_INFO_GETTING_CONTINUOUS,
    MEM_INFO_GETTING_COMPLETE
}mem_info_getting_status_t;


#define  VERSION_2G   0x47534D00UL
#define  VERSION_3G   0x54440000UL

#define  IS_3G_VERSION(_X_)  (VERSION_3G == (_X_)->version)

#define MAX_MEM_FILE_NAME_LEN 23

#define ENTRY_BYTE_HEAP   0x42595445UL

typedef struct MEM_INFO_2G_Tag
{
	uint8  file_name[MAX_MEM_FILE_NAME_LEN + 1];
	uint32 line;
	uint32 size;
	uint32 block_num;	
}MEM_INFO_2G_T;

#define MAX_ENTRY_NAME_LEN 15

typedef struct MEM_INFO_3G_Tag
{
	uint32 entry_type;
	uint32 block_num;
	uint32 size;
	uint32 file_code;
	uint32 line;
	uint8  entry_name[MAX_ENTRY_NAME_LEN + 1];
	uint8  file_name[MAX_MEM_FILE_NAME_LEN + 1];
}MEM_INFO_3G_T;

typedef struct MEM_PACK_Tag
{
	uint32  version;
	uint32  fregment;   	
	union MEM_PACK_BODY_Tag
	{
		MEM_INFO_2G_T mem_info_2G;
		MEM_INFO_3G_T mem_info_3G;
	}pack_body;
}MEM_PACK_T;

/*----------------------------------------------------------------------------*
 *  NAME
 *      SCI_MemInfoGet
 *
 *  DESCRIPTION
 *      Get the memory info from RTOS.
 *
 *  RETURNS
 *      Status: 
 *
 *----------------------------------------------------------------------------*/
PUBLIC int SCI_MemInfoGet(uint8 *mem_info_ptr, uint32  mem_info_len,  uint32  item_offset);
PUBLIC  uint32 SCI_GetMemInfoCount(void);
/*
 * End!!!
 * Added by Eddie.Li for support realtime debug on OS. 
 */

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function is used to initialize the IRQ stack. 
//	Global resource dependence: 
//  Author:         lin.liu
//	Note:           
/*****************************************************************************/
PUBLIC void SCI_InitIRQStack(void);

/**---------------------------------------------------------------------------*
 ** MEMORY
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The InitMem function is used to create 6 diffrent 
//                  memory pool. 
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_InitMem(void);

/*****************************************************************************/
//  Description:    The SCI_PrintAllocatedMemInfo function is used to print 
//                  allocated memory infomation.
//	Global resource dependence: 
//  Author: 		Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_PrintAllocatedMemInfo(void);
// Added this temporarily for beijing test. It will be deleted later.
#define SCI_ExtDumpAllocMem     SCI_PrintAllocatedMemInfo

/*****************************************************************************/
//  Description:    The SCI_PrintPoolInfo function is used to print 
//                  all memory pool usage information. 
//	Global resource dependence: 
//  Author: 		Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_PrintPoolInfo(void);

/*****************************************************************************/
//  Description:    The SCI_DumpPoolInfo function is used to print 
//                  all memory pool usage information. 
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_DumpPoolInfo(void);

PUBLIC void SCI_DumpHeapInfo(void);

PUBLIC   void SCI_DumpUserMemPool(void);

/*****************************************************************************/
//  Description:    The SCI_PrintAllocatedMemInfo function is used to dump
//                  allocated memory infomation.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_DumpAllocatedMemInfo(void);


/*****************************************************************************/
//  Description:    The SCI_HandleUndefined function is used handle Prefetch.
//	Global resource dependence:
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_HandleUndefined(void);

/*****************************************************************************/
//  Description:    The SCI_HandleSwi function is used handle SWI.
//	Global resource dependence:
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_HandleSwi(void);

/*****************************************************************************/
//  Description:    The SCI_HandlePrefetch function is used handle Prefetch.
//	Global resource dependence:
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_HandlePrefetch(void);

/*****************************************************************************/
//  Description:    The SCI_HandleAbort function is used handle Prefetch.
//	Global resource dependence:
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_HandleAbort(void);

/*****************************************************************************/
//  Description:    The SCI_HandleReserved function is used handle Prefetch.
//	Global resource dependence:
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_HandleReserved(void);

/*****************************************************************************/
//  Description:    The SCI_HandleWatchDog function is used handle watch dog.
//	Global resource dependence:
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_HandleWatchDog(void);

/**---------------------------------------------------------------------------*
 ** THERAD
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Get pointer of thread control block from thread id.
//	Global resource dependence: 
//                              SCI_MAX_STATIC_THREAD_NUM
//                              SCI_MIN_DYNAMIC_THREAD_ID
//                              s_static_thread_table
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
__inline SCI_THREAD_PTR SCI_GetThreadBlockFromId(   // If thread id is valid,
                                // Return the pointer of thread control block.
                                // else return SCI_NULL;
    BLOCK_ID thread_id
    );

/*****************************************************************************/
//  Description:    Get thread id from pointer of thread control block.
//	Global resource dependence: 
//                              SCI_MAX_STATIC_THREAD_NUM
//                              SCI_MIN_DYNAMIC_THREAD_ID
//                              s_static_thread_table
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
__inline BLOCK_ID SCI_GetThreadIdFromBlock(   // If thread id is valid,
                                // Return the pointer of thread control block.
                                // else return SCI_INVALID_BLOCK_ID;
    SCI_THREAD_PTR  thread_block_ptr
    );

/*****************************************************************************/
//  Description:    The function gets the next thread block by the special threadblock.
//	Global resource dependence: 
//  Author:         Liangwen.Zhen
//	Note:           is_find_first == TRUE£¬the function would ingnor threadx_block_ptr
/*****************************************************************************/
PUBLIC SCI_THREAD_PTR SCI_GetNextThreadBlock(
				BOOLEAN 		is_find_first,
				SCI_THREAD_PTR 	threadx_block_ptr
	);

/*****************************************************************************/
//  Description:    The function gets next thread id of a specified thread.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC BLOCK_ID  SCI_GetNextThread( // ID of next thread of specified thread
    BLOCK_ID prev_id                // ID of specified thread
    );
    
/*****************************************************************************/
//  Description:    Reset RTOS .
//	Global resource dependence:
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC void SCI_ResetRtos(void);

/*****************************************************************************/
//  Description:    The function is used to get error information by kernal 
//                  error code.
//	Global resource dependence:
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC RETURN_VALUE_T *SCI_GetInfoByKernalErr(
    uint32 err_code
    );

/*****************************************************************************/
//  Description:    searches timer signal from save_queue and current
//                  pending queue.
//	Global resource dependence:
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC xSignalHeader SCI_SearchQueues(   // If timer signal is found, return the
                                // signal pointer; else return SCI_NULL.
    uint32      timer_id,       // SignalCode of the timer msg
    BLOCK_ID    self_id         // ID of the task which receives
                                // the timer up msg
    );
    
/*****************************************************************************/
//  Description:    searches timer signal from save_queue and current
//                  pending queue.
//	Global resource dependence:
//  Author: Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC xSignalHeader SCI_SearchQueues_WithoutDel(   // If timer signal is found, return the
                                // signal pointer; else return SCI_NULL.
    uint32      timer_id,       // SignalCode of the timer msg
    BLOCK_ID    self_id         // ID of the task which receives
                                // the timer up msg
    );    

/*****************************************************************************/
//  Description:    The function is used to get error information by kernel 
//                  error code.
//	Global resource dependence:
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC RETURN_VALUE_T *SCI_GetInfoByKernelErr(
    uint32 err_code
    );
        
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  // End of _OS_APIEXT_H
