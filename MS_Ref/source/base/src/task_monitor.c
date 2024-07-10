/******************************************************************************
 ** File Name:      task_monitor.c                                            *
 ** Author:         Richard Yang                                              *
 ** DATE:           03/04/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 task switch.                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/04/2003     Richard.Yang     Create.                                   *

 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"


#include "task_monitor.h"
#include "fiq_drv.h"
#include "isr_drvapi.h"
#include "os_param.h"

#include "mem_dump.h" //kevin.wang@0719

#include "os_import.h"
#include "sio.h"
#include "deep_sleep.h"
#include "threadx_assert.h"
#include "os_import.h"
#include "ref_outport.h"
#include "timer_drvapi.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
#define TM_TASK_MONITOR

#ifdef TM_TASK_MONITOR
#define DUMP_MEM_PACKET_SIZE    (16 * 1024)
//extern uint32 sleep_start_time;

typedef struct TASK_TEST_POINTtag
{
	uint16  id;
	uint16  acuracy_time;
	void  * pointer;
	void  * return_addr;
	uint32  time;
}TASK_TEST_POINT_T;


#ifdef SRAM_SIZE_16MBIT
#define MAX_SWITCH_NUM 			80
#elif defined(SRAM_SIZE_32MBIT)
#define MAX_SWITCH_NUM 			320
#else
#define MAX_SWITCH_NUM 			570  //6121E is 128MBIT
#endif

#define IRQ_NUM					32
#define FIQ_NUM					32
#define MEM_REGION_NUM			6

typedef struct _TM_HEAD
{
	uint8  magic[16];
	uint16 nrof_irq;
	uint16 irq_tab_offset;
	uint16 nrof_mem_region;
	uint16 mem_tab_offset;
	uint16 nrof_point;
	uint16 point_tab_offset;
} TM_HEAD;

typedef struct _TM_INFO
{
	/* head */
	TM_HEAD 	head;
	
	/* irq table */
	char		irq_tab[IRQ_NUM][16];
	
	/* memory region table */
	struct
	{
		uint32 start_addr;
		uint32 size;
		uint32 offset;
	} mem_tab[MEM_REGION_NUM];
	
	/* test point table */
	TASK_TEST_POINT_T point_tab[MAX_SWITCH_NUM + 2];
} TM_INFO;


#if defined(PLATFORM_UWS6121E)
typedef struct _TM_TASK_NAME
{
	/* head */
	unsigned long 	taskId;
	
	/* test point table */
	char            name[22]; //原32，ram裁剪
} TM_TASK_NAME;

__align(4)  static TM_TASK_NAME tm_taskName[60];

static uint16 task_name_offset = 0;
#endif

__align(4)  static TM_INFO tm_info;
static TASK_TEST_POINT_T * test_point = &tm_info.point_tab[0];
static uint32 test_point_offset = 0;

#endif	

void TM_SendInitRequest(void)
{
#ifdef TM_TASK_MONITOR
    int i;
    const char **fiq_irq_name= CHIP_GetNameOfFiqIrq();
//the dumped data related with register contains three part:the number of registers group,which is 4 bytes;
//all registers base address,which is 4*CHIP_GetNumCtlRegGrp() bytes;all registers length,
//which is 4*CHIP_GetNumCtlRegGrp() bytes;all registers data,which is CHIP_GetAllCtlRegLen() bytes.    
	uint32 dump_reg_size = 4 + (4 + 4) * CHIP_GetNumCtlRegGrp() + CHIP_GetAllCtlRegLen();
	
	/* head */
	SCI_MEMCPY(tm_info.head.magic, "TASKMONITORBEGIN", sizeof(tm_info.head.magic));
	tm_info.head.nrof_irq = NUM_FIQ_IRQ;
	tm_info.head.irq_tab_offset = (uint32)(&((TM_INFO *)0)->irq_tab[0]);  /*lint !e413 */
	tm_info.head.nrof_mem_region = sizeof(tm_info.mem_tab) / sizeof(tm_info.mem_tab[0]);
	tm_info.head.mem_tab_offset = (uint32)(&((TM_INFO *)0)->mem_tab[0]);  /*lint !e413 */
	tm_info.head.nrof_point = MAX_SWITCH_NUM; 
	tm_info.head.point_tab_offset = (uint32)(&((TM_INFO *)0)->point_tab[0]);	/*lint !e413 */
	
	/* irq table */

	for (i = 0; i < tm_info.head.nrof_irq; ++i) {
		strcpy(tm_info.irq_tab[i], fiq_irq_name[i]);
	} 
	
	/* memory region table */
	/* SRAM / SDRAM */
	tm_info.mem_tab[0].start_addr = MEM_GetDumpBaseAddr(); //g_mem_dump_start;
	//tm_info.mem_tab[0].size = (MEM_GetDumpLen() / DUMP_MEM_PACKET_SIZE) * DUMP_MEM_PACKET_SIZE;
	tm_info.mem_tab[0].size = MEM_GetDumpLen();
	tm_info.mem_tab[0].offset = 0;
	
	/* task information */
	tm_info.mem_tab[1].start_addr = (uint32)&tm_taskName[0]; /* invalid */
	tm_info.mem_tab[1].size = sizeof(TM_TASK_NAME) * 60;
	tm_info.mem_tab[1].offset = (uint32)&tm_taskName[0] - MEM_GetDumpBaseAddr();
	
	/* shared memory */
	tm_info.mem_tab[2].start_addr = CHIP_GetSharedMemBase();
	tm_info.mem_tab[2].size = CHIP_GetSharedMemLen();
	tm_info.mem_tab[2].offset = tm_info.mem_tab[0].offset + tm_info.mem_tab[0].size + DUMP_MEM_PACKET_SIZE;   // offset the same as before

	/* internal ram */
	tm_info.mem_tab[3].start_addr = CHIP_GetInterRamBase();
	tm_info.mem_tab[3].size = CHIP_GetInterRamLen();
	tm_info.mem_tab[3].offset = tm_info.mem_tab[2].offset + tm_info.mem_tab[2].size;
	
	/* register */
	tm_info.mem_tab[4].start_addr = CHIP_GetFirstRegBaseAddr();
	tm_info.mem_tab[4].size = dump_reg_size; 
	tm_info.mem_tab[4].offset = tm_info.mem_tab[3].offset + tm_info.mem_tab[3].size;
	
	/* DSP memory */
	tm_info.mem_tab[5].start_addr = CHIP_GetDspMemBaseAddr(); 
	tm_info.mem_tab[5].size = CHIP_GetDspMemLen();
	tm_info.mem_tab[5].offset = tm_info.mem_tab[4].offset + tm_info.mem_tab[4].size;
	
	/* point table */
	test_point = &tm_info.point_tab[0];
#endif /* TM_TASK_MONITOR */	
}

LOCAL void prv_TM_SendRequest(uint8 id, uint32 param1, uint32 param2)
{
    #ifdef TM_TASK_MONITOR
        uint8 assert_mode;
		uint32 i = test_point_offset;
        OSPARAM_GetAssertMode(&assert_mode);
        if(SCI_ASSERT_DEBUG_MODE == assert_mode)
        {	
        	test_point_offset++;
	    
	        if (test_point_offset >= MAX_SWITCH_NUM)
		        test_point_offset = 0;
	
        	if ( i < MAX_SWITCH_NUM)
	        {	
    	        test_point[i].id          = id;
                test_point[i].time         = SLEEP_GetCur32kNum();
                test_point[i].acuracy_time = 0; 
            	test_point[i].pointer     = (void *) param1;
        	    test_point[i].return_addr = (void *) param2;
            }
        }
    #endif
}


void TM_SendIdleRequest(void)
{
	uint32 irq_ie_state = INT_HAL_IRQGetIEState();

	prv_TM_SendRequest(TM_IDLE_REQ,irq_ie_state,0);

}

void * TM_SendTaskRequest(void *thread_ptr)
{
	prv_TM_SendRequest(TM_TASK_REQ,thread_ptr,0);
  
    return thread_ptr;
}

extern uint32 _tx_pc_val_before_irq;
void TM_SendIrqRequest(uint8 id, uint32 irq_status)
{ 
	prv_TM_SendRequest(id,irq_status,_tx_pc_val_before_irq);

}

void TM_SendSleepRequest(uint8 id)
{
	prv_TM_SendRequest(id,0,0);

}

void TM_SendTestPointRequest(uint32 param1, uint32 param2)
{
	if(SCI_IsAssert())
	{
		return;
	}

	prv_TM_SendRequest(TM_TEST_POINT_REQ,param1,param2);

}

void TM_setTaskName(unsigned long taskid, char *name)
{
    #if defined TM_TASK_MONITOR && defined(PLATFORM_UWS6121E)
	uint8      v_taskNameLen;
    uint32     i;
	
	v_taskNameLen = ((strlen(name)) > 31 ? 31 : (strlen(name)));

    for(i = 0; i < task_name_offset; i++)
    {
         if(!memcmp(tm_taskName[i].name, name, v_taskNameLen))
         {
             // task name has been saved, update the task id
             tm_taskName[i].taskId = taskid;
             return;
		 }
	}

	if(task_name_offset < 60)
	{
         i = task_name_offset++;
  
         tm_taskName[i].taskId = taskid;
		 strncpy(&tm_taskName[i].name[0], (char *)name, v_taskNameLen);
	}
    #endif   
}

extern uint8 dump_mem_data[8];
extern const uint8 dump_mem_end[8];

void DumpSwitchMemory(void)
{

#ifndef WIN32
    uint32  i;
    uint32  num;
    char    *mem_ptr = (char *)(((uint32)&tm_info - 32) & (uint32)0xFFFFFFF0);
    uint32  packet_num;

    SCI_PrintAssertString("\nDumping memory to a file, please wait ......");

	packet_num = sizeof(tm_info);
    packet_num = packet_num / DUMP_MEM_PACKET_SIZE + 1;
    
    // Dump memory data.
    for (num = 0; num < packet_num; num++)
    {
        OS_DebugPutChar(0x7E);
        
        // Head.
        for (i = 0; i < sizeof(dump_mem_data); i++)
        {
            if ((dump_mem_data[i] == 0x7E) || (dump_mem_data[i] == 0x7D))
            {
                OS_DebugPutChar(0x7D);
                OS_DebugPutChar((dump_mem_data[i] ^ 0x20));
            }
            else
            {
                OS_DebugPutChar(dump_mem_data[i]);
            }
        }
        
        // Data.
        for (i = 0; i < DUMP_MEM_PACKET_SIZE; i++)
        {
            if ((*mem_ptr == 0x7E) || (*mem_ptr == 0x7D))
            {
                OS_DebugPutChar(0x7D);
                OS_DebugPutChar((*mem_ptr ^ 0x20));
            }
            else
            {
                OS_DebugPutChar(*mem_ptr);
            }

      	    mem_ptr++;
        }

        OS_DebugPutChar(0x7E);
    }

    // Dump end packet.
    OS_DebugPutChar(0x7E);
    for (i = 0; i < sizeof(dump_mem_end); i++)
    {
        if ((dump_mem_end[i] == 0x7E) || (dump_mem_end[i] == 0x7D))
        {
            OS_DebugPutChar(0x7D);
            OS_DebugPutChar((dump_mem_end[i] ^ 0x20));
        }
        else
        {
            OS_DebugPutChar(dump_mem_end[i]);
        }
    }
    OS_DebugPutChar(0x9);
    OS_DebugPutChar(0x7E);
    
    SCI_PrintAssertString( "\nDumping switch memory completed !!!\n");

#endif

}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif //
