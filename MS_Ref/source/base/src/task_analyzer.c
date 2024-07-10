/******************************************************************************
 ** File Name:      task_analyse.c                                            *
 ** Author:         Johnson.sun                                               *
 ** DATE:           02/18/2008                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 all abstract task analyzer.                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 02/18/2008     Johnson.sun      Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "task_analyzer.h"
//#include "os_apiext.h"
#include "sc_reg.h"
#include "threadx_os.h"

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

#ifdef TASK_ANALYZER
#define STATIC_TASK_NODE_NUM   52 //(SCI_MAX_STATIC_THREAD_NUM+2), static, rtos manager, system timer
#define IDLE_NUM               1
#define IRQ_NODE_NUM           32
#define FIQ_NODE_NUM           32
#define DYNAMIC_TASK_NODE_NUM  39
#define USER_MARK_NODE_NUM     100 
 
#define IDLE_BASE           (STATIC_TASK_NODE_NUM) 
#define IRQ_BASE            (IDLE_BASE + 1)
#define FIQ_BASE            (IRQ_BASE + IRQ_NODE_NUM)
#define DYNAMIC_TASK_BASE   (FIQ_BASE + FIQ_NODE_NUM)
#define USER_MARK_BASE      (DYNAMIC_TASK_BASE + DYNAMIC_TASK_NODE_NUM)

#define MAX_TASK_ANALYSE_NUM  (USER_MARK_BASE + USER_MARK_NODE_NUM) //total 256 node
       

/* struct size: 10*4=40bytes. */ 
typedef struct TASK_ANALYZER_NODE_tag
{
    uint32 id;               //task analyzer node id or thread id..
    TASK_TYPE_E task_type;   //task type: static, dynamic, irq, fiq, user mark.
    char   task_name[16];    //task name
    uint32 enter_time;       //current enter task time each time
    uint32 total_time_l;     //total time low part(us) : 0~0xFFFFFFFE
    uint32 total_time_h;     //total time high part(us): multiple of 0xFFFFFFFE
    uint32 is_used;          //the flag whether the task ran. 
}TASK_ANALYZER_NODE_T; 


typedef struct TASK_ANALYZER_tag
{
    uint32 total_run_time_l;
    uint32 total_run_time_h;
    
    TASK_ANALYZER_NODE_T node_table[MAX_TASK_ANALYSE_NUM];
}TASK_ANALYZER_T;

TASK_ANALYZER_T  s_logical_task_analyzer;


LOCAL TASK_ANALYZER_NODE_T  *s_test_node_ptr = &s_logical_task_analyzer.node_table[0];

LOCAL BOOLEAN s_is_first = SCI_FALSE;  //record point flag for the first.
LOCAL uint32  s_cur_task_id = 0;


LOCAL char int_map[32][10] =
{    
    /*   0           1           2        3          4          5         6      7  */
    "Uartsleep", "Soft_int", "COMMRX", "COMMTX", "INTCNT1", "INTCNT2", "GPIO", "RTC",
    /* 8     9       10        11           12        13     14       15            */
    "kPD", "I2C" , "Sim0", "Ser_port0", "Ser_port1", "DSP", "ADC", "Gea_poll",
    /* 16      17          18         19     20     21      22      23              */
    "Sys", "Ser_port3", "Ser_port2", "Spk", "DMA", "VBC", "Mon0", "Mon1",
    /* 24    25      26      27    28      29     30      31                        */
    "Mon2", "USB", "DCAM", "NLC", "DCT", "Sim1", "WDG", "PBINT"
};

#endif 

/*****************************************************************************/
// Description : Init task analyzer value (time value clear zero).
// Global resource dependence :
// Return value: None
// Author      : Johnson.sun 
// Note :           
/*****************************************************************************/
void TA_InitTaskAnalyse(void)
{
#ifdef TASK_ANALYZER
    uint32 i;
    uint32 temp;
    
    s_logical_task_analyzer.total_run_time_l = 0;
    s_logical_task_analyzer.total_run_time_h = 0;

    s_test_node_ptr = &s_logical_task_analyzer.node_table[0];

    /* Register static task, thread_manage task, idle */
    for (i=0; i<IRQ_BASE-1; i++)
    {
        s_test_node_ptr[i].id = 0;
        SCI_MEMSET(&s_test_node_ptr[i].task_name, 0, 16);
        s_test_node_ptr[i].enter_time = 0;
        s_test_node_ptr[i].total_time_l = 0;
        s_test_node_ptr[i].total_time_h = 0;
        s_test_node_ptr[i].is_used = SCI_TRUE;
    }
    
    /* System Timer Thread */
    s_test_node_ptr[51].id = 51; 
    strncpy(s_test_node_ptr[51].task_name, "System Timer Thread", 15);
    s_test_node_ptr[51].task_name[15] = 0;
    s_test_node_ptr[51].enter_time = 0;
    s_test_node_ptr[51].total_time_l = 0;
    s_test_node_ptr[51].total_time_h = 0;
    s_test_node_ptr[51].is_used = SCI_TRUE;
    
    /* Register IRQ */
    for (i=IRQ_BASE; i<FIQ_BASE; i++)
    {
        s_test_node_ptr[i].id = i;
        temp = strlen("IRQ_");
        strcpy(&s_test_node_ptr[i].task_name[0], "IRQ_");
        strcat(&s_test_node_ptr[i].task_name[temp], int_map[i-IRQ_BASE]);
        s_test_node_ptr[i].enter_time = 0;
        s_test_node_ptr[i].total_time_l = 0;
        s_test_node_ptr[i].total_time_h = 0;
        s_test_node_ptr[i].is_used = SCI_TRUE;
    }
    
    /* Register FIQ */ 
    for (i=FIQ_BASE; i<DYNAMIC_TASK_BASE; i++)
    {
        s_test_node_ptr[i].id = i;
        temp = strlen("FIQ_");
        strcpy(&s_test_node_ptr[i].task_name[0], "FIQ_");
        strcat(&s_test_node_ptr[i].task_name[temp], int_map[i-FIQ_BASE]);
        s_test_node_ptr[i].enter_time = 0;
        s_test_node_ptr[i].total_time_l = 0;
        s_test_node_ptr[i].total_time_h = 0;
        s_test_node_ptr[i].is_used = SCI_TRUE;
    }
  
    /* Init Dynamic task and user mark region */
    for (i=DYNAMIC_TASK_BASE; i<MAX_TASK_ANALYSE_NUM; i++)
    {
        s_test_node_ptr[i].id = 0;
        SCI_MEMSET(&s_test_node_ptr[i].task_name, 0, 16);
        s_test_node_ptr[i].enter_time = 0;
        s_test_node_ptr[i].total_time_l = 0;
        s_test_node_ptr[i].total_time_h = 0;
        s_test_node_ptr[i].is_used = SCI_FALSE;
    }      
    
    s_is_first = SCI_TRUE;
    s_cur_task_id = STATIC_TASK_NODE_NUM - 1;
#endif
}


/*****************************************************************************/
// Description : Reset task analyzer value (time value clear zero).
// Global resource dependence :
// Return value: None
// Author      : Johnson.sun 
// Note :           
/*****************************************************************************/
void TA_ResetTaskAnalyseRequest(void)
{
#ifdef TASK_ANALYZER
    uint32 i;
    
    s_logical_task_analyzer.total_run_time_l = 0;
    s_logical_task_analyzer.total_run_time_h = 0;
    
    for (i=0; i<MAX_TASK_ANALYSE_NUM; i++)
    {
        s_test_node_ptr[i].enter_time = 0;
        s_test_node_ptr[i].total_time_l = 0;
        s_test_node_ptr[i].total_time_h = 0;
    }
    
    s_is_first = SCI_TRUE;
    s_cur_task_id = STATIC_TASK_NODE_NUM - 2; //Manage thread.
#endif
}

void TA_RegisterTaskRequest(TASK_TYPE_E task_type, const char *thread_name, uint32 id)
{
#ifdef TASK_ANALYZER
    uint32 offset;
    
    if (DYNAMIC_TASK == task_type)
    {
       for(offset=DYNAMIC_TASK_BASE; offset<USER_MARK_BASE; offset++)
       {
           if (!s_test_node_ptr[offset].is_used)
           {
               s_test_node_ptr[offset].id = id;
               s_test_node_ptr[offset].task_type = DYNAMIC_TASK;
               if (strlen(thread_name) >= 15)
               {
                   SCI_MEMCPY(&s_test_node_ptr[offset].task_name, thread_name, 15);
               }
               else
               {
                   uint16 i;
                   strcpy((char *)s_test_node_ptr[offset].task_name, thread_name);
                   for (i=strlen(thread_name); i<15; i++)
                   {
                       s_test_node_ptr[offset].task_name[i] = ' ';
                   }
               }
               s_test_node_ptr[offset].task_name[15] = 0;
               s_test_node_ptr[offset].enter_time = 0;
               s_test_node_ptr[offset].total_time_l = 0;
               s_test_node_ptr[offset].total_time_h = 0;
               s_test_node_ptr[offset].is_used = SCI_TRUE;
               break;
           }
       }
    }
    else if (STATIC_TASK == task_type)
    {
        s_test_node_ptr[id].id = id;
        s_test_node_ptr[id].task_type = STATIC_TASK;
        if (strlen(thread_name) >= 15)
        {
            SCI_MEMCPY(s_test_node_ptr[id].task_name, thread_name, 15);
        }
        else
        {
            uint16 i;
            strcpy((char *)s_test_node_ptr[id].task_name, thread_name);
            for (i=strlen(thread_name); i<15; i++)
            {
                s_test_node_ptr[id].task_name[i] = ' ';
            }
        }
        s_test_node_ptr[id].task_name[15] = 0;
        s_test_node_ptr[id].enter_time = 0;
        s_test_node_ptr[id].total_time_l = 0;
        s_test_node_ptr[id].total_time_h = 0;
        s_test_node_ptr[id].is_used = SCI_TRUE;
    }
    else
    {
//        SCI_PASSERT(0,("task type = %d", task_type)); //error!!! /*assert verified*/
    }
#endif 
}

/*****************************************************************************/
// Description : Un-register task analyzer value (time value clear zero).
// Global resource dependence :
// Return value: None
// Author      : Johnson.sun 
// Note :           
/*****************************************************************************/
void TA_UnRegisterTaskRequest(TASK_TYPE_E task_type, uint32 id)
{
#ifdef TASK_ANALYZER
    uint32 offset;
    
    if (DYNAMIC_TASK == task_type)
    {
        for(offset=DYNAMIC_TASK_BASE; offset<USER_MARK_BASE; offset++)
        {
            if (id == s_test_node_ptr[offset].id)
            {
                SCI_MEMSET(s_test_node_ptr[offset].task_name, 0, 16);
                s_test_node_ptr[offset].enter_time = 0;
                s_test_node_ptr[offset].total_time_l = 0;
                s_test_node_ptr[offset].total_time_h = 0;
                s_test_node_ptr[offset].is_used = SCI_FALSE;
            }
        }
    }
    else
    {
//         SCI_PASSERT(0,("task_type=%d", task_type));//error!!!  /*assert verified*/
    }
#endif
}

/*****************************************************************************/
// Description : Enter Idle "task" and Stat analyzer value.
// Global resource dependence :
// Return value: None
// Author      : Johnson.sun 
// Note :           
/*****************************************************************************/
void TA_EnterIdleRequest(void)
{
 #ifdef TASK_ANALYZER
    uint32 time;
    uint32 time_delta;

    if (s_is_first)
    {
        s_is_first = SCI_FALSE;
        s_test_node_ptr[IDLE_BASE].enter_time = *(volatile uint32 *)WDG_VALUE;
        s_cur_task_id = IDLE_BASE;
    }
    else
    {
        if (s_cur_task_id == IDLE_BASE)
        {
            return; 
        }
   
        /* Record time through wdg, and count last task execute time */
        time = *(volatile uint32 *)WDG_VALUE;
        if (time >= s_test_node_ptr[s_cur_task_id].enter_time) //wdg reset
        {
            time_delta = 0xFFFFFFFF - time + 1 + s_test_node_ptr[s_cur_task_id].enter_time;
        }
        else
        {
            time_delta = s_test_node_ptr[s_cur_task_id].enter_time - time;
        }
        
        if (s_logical_task_analyzer.total_run_time_l <= s_logical_task_analyzer.total_run_time_l + time_delta)
        {
        	s_logical_task_analyzer.total_run_time_l += time_delta;
        }
        else
        {
            s_logical_task_analyzer.total_run_time_h += 1;
            s_logical_task_analyzer.total_run_time_l = s_logical_task_analyzer.total_run_time_l + time_delta - 0xFFFFFFFE;
        }
        
    	if (s_test_node_ptr[s_cur_task_id].total_time_l <= s_test_node_ptr[s_cur_task_id].total_time_l + time_delta)
        {
            s_test_node_ptr[s_cur_task_id].total_time_l += time_delta;
        }
        else
        {
            s_test_node_ptr[s_cur_task_id].total_time_h += 1;
            s_test_node_ptr[s_cur_task_id].total_time_l = s_test_node_ptr[s_cur_task_id].total_time_l + time_delta - 0xFFFFFFFE;
        }
        /* Record idle task enter time and refresh current task id */
        s_cur_task_id = IDLE_BASE;
        s_test_node_ptr[IDLE_BASE].enter_time = time;
    }
#endif
}

/*****************************************************************************/
// Description : Enter thread "task" and Stat analyzer value.
// Global resource dependence :
// Return value: None
// Author      : Johnson.sun 
// Note :           
/*****************************************************************************/
void *TA_EnerTaskRequest(void *thread_ptr)
{
 #ifdef TASK_ANALYZER 	
    uint32 time;
    uint32 time_delta;
    uint32 block_type;
    uint32 thread_id;
    int  i;
    
    SCI_ASSERT(thread_ptr != SCI_NULL);/*assert verified*/
 
    /* Record time through wdg, and count last task execute time */
    time = *(volatile uint32 *)WDG_VALUE;
    if (time >= s_test_node_ptr[s_cur_task_id].enter_time) //wdg reset
    {
        time_delta = 0xFFFFFFFF - time + 1 + s_test_node_ptr[s_cur_task_id].enter_time;
    }
    else
    {
        time_delta = s_test_node_ptr[s_cur_task_id].enter_time - time;
    }
        
    if (s_logical_task_analyzer.total_run_time_l <= s_logical_task_analyzer.total_run_time_l + time_delta)
    {
        s_logical_task_analyzer.total_run_time_l += time_delta;
    }
    else
    {
        s_logical_task_analyzer.total_run_time_h += 1;
        s_logical_task_analyzer.total_run_time_l = s_logical_task_analyzer.total_run_time_l + time_delta - 0xFFFFFFFE;
    }
        
    if (s_test_node_ptr[s_cur_task_id].total_time_l <= s_test_node_ptr[s_cur_task_id].total_time_l + time_delta)
    {
        s_test_node_ptr[s_cur_task_id].total_time_l += time_delta;
    }
    else
    {
        s_test_node_ptr[s_cur_task_id].total_time_h += 1;
        s_test_node_ptr[s_cur_task_id].total_time_l = s_test_node_ptr[s_cur_task_id].total_time_l + time_delta - 0xFFFFFFFE;
    }
    
    /* System Timer Thread */
    if (0 == strcmp("System Timer Thread", ((TX_THREAD *)thread_ptr)->tx_thread_name))
    {
        /* Record idle task enter time and refresh current task id */
    	s_cur_task_id = 51;
    	s_test_node_ptr[51].enter_time = time;
    	return thread_ptr;
    }
    
    /* Static Thread and Dynamic Thread */
    block_type = *((uint32 *)thread_ptr - 2);
    thread_id = *((uint32 *)thread_ptr - 1);
    if (block_type == SCI_STATIC_THREAD)
    {
        SCI_ASSERT(thread_id < IDLE_BASE);/*assert verified*/
        /* Record idle task enter time and refresh current task id */
    	s_cur_task_id = thread_id;
    	s_test_node_ptr[thread_id].enter_time = time;
    }
    else if (block_type == SCI_DYNAMIC_THREAD)
    {
        for (i=DYNAMIC_TASK_BASE; i<USER_MARK_BASE; i++)
        {
            if (thread_id == s_test_node_ptr[i].id)
            {
                break;
            }
        }
        
        if (i < USER_MARK_BASE)
        {
            /* Record idle task enter time and refresh current task id */
    		s_cur_task_id = i;
    		s_test_node_ptr[s_cur_task_id].enter_time = time;
        }
        else
        {
 //           SCI_PASSERT(0,("thread_name=%s", ((TX_THREAD *)thread_ptr)->tx_thread_name));//error!!! no task record.  /*assert verified*/
        }
    }
    else
    {
 //       SCI_PASSERT(0,("thread_name=%s", ((TX_THREAD *)thread_ptr)->tx_thread_name));//error!!! no task record.   /*assert verified*/
    }
    	
#endif   
    return thread_ptr;
}

/*****************************************************************************/
// Description : Enter IRQ "task" and Stat analyzer value.
// Global resource dependence :
// Return value: None
// Author      : Johnson.sun 
// Note :           
/*****************************************************************************/
void TA_EnterIrqRequest(uint32 irq_num)
{
#ifdef TASK_ANALYZER
    uint32 time;
    uint32 time_delta;
    uint32 irq_task_id;
    
    /* Firt figure out "irq task" id */
    irq_task_id = irq_num + IRQ_BASE;

    /* Is First?  */
    if (s_is_first)
    {
        s_is_first = SCI_FALSE;
        s_test_node_ptr[irq_task_id].enter_time = *(volatile uint32 *)WDG_VALUE;
        s_cur_task_id = irq_task_id;
    }
    else
    {
        /* Record time through wdg, and count last task execute time */
        time = *(volatile uint32 *)WDG_VALUE;
        if (time >= s_test_node_ptr[s_cur_task_id].enter_time) //wdg reset
        {
            time_delta = 0xFFFFFFFF - time + 1 + s_test_node_ptr[s_cur_task_id].enter_time;
        }
        else
        {
            time_delta = s_test_node_ptr[s_cur_task_id].enter_time - time;
        }

        time_delta = s_test_node_ptr[s_cur_task_id].enter_time - time;
        
        if (s_logical_task_analyzer.total_run_time_l <= s_logical_task_analyzer.total_run_time_l + time_delta)
        {
        	s_logical_task_analyzer.total_run_time_l += time_delta;
        }
        else
        {
            s_logical_task_analyzer.total_run_time_h += 1;
            s_logical_task_analyzer.total_run_time_l = s_logical_task_analyzer.total_run_time_l + time_delta - 0xFFFFFFFE;
        }
        
    	if (s_test_node_ptr[s_cur_task_id].total_time_l <= s_test_node_ptr[s_cur_task_id].total_time_l + time_delta)
        {
            s_test_node_ptr[s_cur_task_id].total_time_l += time_delta;
        }
        else
        {
            s_test_node_ptr[s_cur_task_id].total_time_h += 1;
            s_test_node_ptr[s_cur_task_id].total_time_l = s_test_node_ptr[s_cur_task_id].total_time_l + time_delta - 0xFFFFFFFE;
        }
        /* Record idle task enter time and refresh current task id */
        s_cur_task_id = irq_task_id;
        s_test_node_ptr[s_cur_task_id].enter_time = time;
    }
#endif
}


/*****************************************************************************/
// Description : Enter FIQ "task" and Stat analyzer value.
// Global resource dependence :
// Return value: None
// Author      : Johnson.sun 
// Note :           
/*****************************************************************************/
void TA_EnterFiqRequest(uint32 fiq_num)
{
#ifdef TASK_ANALYZER
    uint32 time;
    uint32 time_delta;
    uint32 fiq_task_id;
    
    /* Firt figure out "irq task" id */
    fiq_task_id = fiq_num + IRQ_BASE;

    /* Is First?  */
    if (s_is_first)
    {
        s_is_first = SCI_FALSE;
        s_test_node_ptr[fiq_task_id].enter_time = *(volatile uint32 *)WDG_VALUE;
        s_cur_task_id = fiq_task_id;
    }
    else
    {
        /* Record time through wdg, and count last task execute time */
        time = *(volatile uint32 *)WDG_VALUE;
        time_delta = s_test_node_ptr[s_cur_task_id].enter_time - time;
        
        if (s_logical_task_analyzer.total_run_time_l <= s_logical_task_analyzer.total_run_time_l + time_delta)
        {
        	s_logical_task_analyzer.total_run_time_l += time_delta;
        }
        else
        {
            s_logical_task_analyzer.total_run_time_h += 1;
            s_logical_task_analyzer.total_run_time_l = s_logical_task_analyzer.total_run_time_l + time_delta - 0xFFFFFFFE;
        }
        
    	if (s_test_node_ptr[s_cur_task_id].total_time_l <= s_test_node_ptr[s_cur_task_id].total_time_l + time_delta)
        {
            s_test_node_ptr[s_cur_task_id].total_time_l += time_delta;
        }
        else
        {
            s_test_node_ptr[s_cur_task_id].total_time_h += 1;
            s_test_node_ptr[s_cur_task_id].total_time_l = s_test_node_ptr[s_cur_task_id].total_time_l + time_delta - 0xFFFFFFFE;
        }
        /* Record idle task enter time and refresh current task id */
        s_cur_task_id = fiq_task_id;
        s_test_node_ptr[s_cur_task_id].enter_time = time;
    }
#endif
}

/*****************************************************************************/
// Description : Register Mark node for run-time analyzer.
// Global resource dependence :
// Return value: None
// Author      : Johnson.sun 
// Note :           
/*****************************************************************************/
uint32 TA_RegisterMarkNode(uint32 param1, uint32 param2)
{
#ifdef TASK_ANALYZER
    uint32 offset;
    
    for(offset=USER_MARK_BASE; offset<MAX_TASK_ANALYSE_NUM; offset++)
    {
       if (!s_test_node_ptr[offset].is_used)
       {
           s_test_node_ptr[offset].id = offset;
           s_test_node_ptr[offset].task_type = USER_MARK;
           s_test_node_ptr[offset].enter_time = 0;
           s_test_node_ptr[offset].total_time_l = 0;
           s_test_node_ptr[offset].total_time_h = 0;
           s_test_node_ptr[offset].is_used = SCI_TRUE;
           break;
       }       
    }
    
    if (offset >= MAX_TASK_ANALYSE_NUM)
    {
        return offset;
    }
    else
    {
        return 0; //error!!! no space!!!
    }
#else
    return 0; //error!!! no space!!!
#endif
}

/*****************************************************************************/
// Description : Un-register Mark node from test node.
// Global resource dependence :
// Return value: None
// Author      : Johnson.sun 
// Note :           
/*****************************************************************************/
void TA_UnRegisterMarkNode(uint32 mark_id)
{
#ifdef TASK_ANALYZER
    if (s_test_node_ptr[mark_id].is_used)
    {
       s_test_node_ptr[mark_id].is_used = SCI_FALSE; 
       s_test_node_ptr[mark_id].total_time_l = 0;
       s_test_node_ptr[mark_id].total_time_h = 0;       
    }
#endif
}

/*****************************************************************************/
// Description : Set Mark node enter point and record enter-time.
// Global resource dependence :
// Return value: None
// Author      : Johnson.sun 
// Note :           
/*****************************************************************************/
void TA_EnterMarkRequest(uint32 mark_id)
{
#ifdef TASK_ANALYZER
    s_test_node_ptr[mark_id].enter_time = *(volatile uint32 *)WDG_VALUE;
#endif
}

/*****************************************************************************/
// Description : Leave Mark node and Stat run-time.
// Global resource dependence :
// Return value: one mark time
// Author      : Johnson.sun 
// Note :           
/*****************************************************************************/
uint32 TA_LeaveMarkRequest(uint32 mark_id)
{
#ifdef TASK_ANALYZER
    uint32 time;
    uint32 time_delta;
    
    time = *(volatile uint32 *)WDG_VALUE;
    if (time >= s_test_node_ptr[mark_id].enter_time) //wdg reset
    {
        time_delta = 0xFFFFFFFF - time + 1 + s_test_node_ptr[mark_id].enter_time;
    }
    else
    {
        time_delta = s_test_node_ptr[mark_id].enter_time - time;
    }

    if (s_test_node_ptr[mark_id].total_time_l + time_delta < s_test_node_ptr[mark_id].total_time_l)
    {
        s_test_node_ptr[mark_id].total_time_h += 1;
        s_test_node_ptr[mark_id].total_time_l = s_test_node_ptr[mark_id].total_time_l + time_delta - 0xFFFFFFFE;
    }
    else
    {
        s_test_node_ptr[mark_id].total_time_l += time_delta;
    }
    
    return time_delta;
#else
    return 0; 
#endif
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 
