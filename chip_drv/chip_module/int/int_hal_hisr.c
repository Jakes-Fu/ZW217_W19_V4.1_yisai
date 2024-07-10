/******************************************************************************
 ** File Name:      int_hal_hisr.c
 ** Author:         steve.zhan
 ** DATE:           06/29/2010
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.
 ** Description:    This file defines the related driver of interrupt process.
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/29/2010     steve.zhan        Create                                    *
 *****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *

**---------------------------------------------------------------------------*/

#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "os_import.h"
#include "chip_plf_export.h"

#include "task_monitor.h"

#define TM_POINT  TM_SendTestPointRequest
//#define HISR_INT_MONITOR_DEBUG//Only used for Debug

#if defined(PLATFORM_SC6800H)
#undef HISR_INT_MONITOR_DEBUG//Only used for Debug
#endif

#ifdef  HISR_INT_MONITOR_DEBUG
#include "tx_api_thumb.h"
#endif

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


/*lint -e656 confirmed by xuepeng*/
/******************************************************************************
**
**                          Date Structures
**
******************************************************************************/
LOCAL SCI_SEMAPHORE_PTR semaphorePtr[ (CHIPDRV_HISR_LAST_ID - CHIPDRV_HISR_START_ID -1) ] = {0};
LOCAL uint8 HisrQNameBuf[ (CHIPDRV_HISR_LAST_ID - CHIPDRV_HISR_START_ID -1) ][32 + 4] = {0};
LOCAL uint8 HisrSemaBuf[ (CHIPDRV_HISR_LAST_ID - CHIPDRV_HISR_START_ID -1) ][32 + 4] = {0};

LOCAL Hisr_T *volatile hisr_header0;        // List of pending hisrs
LOCAL Hisr_T *volatile hisr_header1;        // List of pending hisrs
LOCAL Hisr_T *volatile hisr_header2;        // List of pending hisrs


LOCAL void call_hisrs (Hisr_T **volatile hisr_list);
LOCAL void  Hisr0 (uint32 i, void *d);
LOCAL void  Hisr1 (uint32 i, void *d);
LOCAL void  Hisr2 (uint32 i, void *d);

/******************************************************************************
**
**                          Global Variables
**
******************************************************************************/
Hisr_T HISR_list[MAX_ISR_NUM];               //will related with interrupt array

/******************************************************************************
**
**                          Function Definitions
**
******************************************************************************/

/*****************************************************************************/
/*****************************************************************************/
// Description :    This function is used to create HISR
// Global resource dependence :
// Author : steve.zhan
// Note :
/*****************************************************************************/
PUBLIC void CHIPDRVHISR_Create (const uint8 *HisrName, CHIPDRV_HISR_ID_E ThreadId, void (*entry) (uint32, void *),
                                CHIPDRV_HISR_PRI_E Priority, int queueNum)
{
    uint32  preempt = 1;

    SCI_PASSERT ( (ThreadId > CHIPDRV_HISR_START_ID &&  ThreadId < CHIPDRV_HISR_LAST_ID),  /*assert verified*/
                  ("%s, %d, threadId: %d", __MODULE__, __LINE__, ThreadId));
    SCI_PASSERT ( (entry != NULL), ("%s, %d, entry == NULL: %d", __MODULE__, __LINE__));/*assert verified*/

    semaphorePtr[ThreadId -CHIPDRV_HISR_START_ID -1] = NULL;

    if (queueNum <= 0)
    {
        queueNum = 1;
    }

    HisrQNameBuf[ThreadId -CHIPDRV_HISR_START_ID -1][0] = 'h';
    HisrQNameBuf[ThreadId -CHIPDRV_HISR_START_ID -1][1] = 'i';
    HisrQNameBuf[ThreadId -CHIPDRV_HISR_START_ID -1][2] = 's';
    HisrQNameBuf[ThreadId -CHIPDRV_HISR_START_ID -1][3] = 'r';

    sprintf ( (char *) (& (HisrQNameBuf[ThreadId -CHIPDRV_HISR_START_ID -1][4])), "0x%x", (uint32) ThreadId);/*lint !e515 -e515*/

    if (NULL == semaphorePtr[ThreadId -CHIPDRV_HISR_START_ID -1])
    {

        HisrSemaBuf[ThreadId -CHIPDRV_HISR_START_ID -1][0] = 'h';
        HisrSemaBuf[ThreadId -CHIPDRV_HISR_START_ID -1][1] = 'i';
        HisrSemaBuf[ThreadId -CHIPDRV_HISR_START_ID -1][2] = 's';
        HisrSemaBuf[ThreadId -CHIPDRV_HISR_START_ID -1][3] = 'r';

        sprintf ( (char *) (& (HisrSemaBuf[ThreadId -CHIPDRV_HISR_START_ID -1][4])), "0x%x", (uint32) ThreadId);
        semaphorePtr[ThreadId -CHIPDRV_HISR_START_ID -1] = SCI_CreateSemaphore (\
                (const char *) & (HisrSemaBuf[ThreadId -CHIPDRV_HISR_START_ID -1]), 0);

    }

    if (Priority == 0)
    {
        preempt  = 0;
    }

    SCI_CreateThread ( (const char *) HisrName, (const char *) & (HisrQNameBuf[ThreadId -CHIPDRV_HISR_START_ID -1]) , \
                       entry, 0, 0, 2 * 1024, queueNum, Priority, preempt, TRUE);

    return;
}

/*****************************************************************************/
// Description :    This function is used in task to get semaphore.
// Global resource dependence :
// Author : steve.zhan
// Note :
/*****************************************************************************/
PUBLIC uint32  CHIPDRVHISR_WaitActive (CHIPDRV_HISR_ID_E ThreadId)
{

    SCI_PASSERT ( (ThreadId > CHIPDRV_HISR_START_ID &&  ThreadId < CHIPDRV_HISR_LAST_ID),  /*assert verified*/
                  ("%s, %d, threadId: %d", __MODULE__, __LINE__, ThreadId));

    if (NULL == semaphorePtr[ThreadId -CHIPDRV_HISR_START_ID -1])
    {
        SCI_PASSERT ( (0), ("%s, %d, threadId: %d", __MODULE__, __LINE__, ThreadId));/*assert verified*/
    }

    if (SCI_ERROR == SCI_GetSemaphore (semaphorePtr[ThreadId -CHIPDRV_HISR_START_ID -1], SCI_WAIT_FOREVER))
    {
        //SCI_TRACE_LOW:"CHIPDRVHISR_WaitActive:Get Semaphore Fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,INT_HAL_HISR_149_112_2_17_23_4_57_1035,(uint8*)"");
    }

    return 0;
}


/*****************************************************************************/
// Description :    This function is used to activate the HISR to process some things.
// Global resource dependence :
// Author : steve.zhan
// Note :
/*****************************************************************************/
PUBLIC void CHIPDRVHISR_Activate (CHIPDRV_HISR_ID_E ThreadId)
{
    SCI_PASSERT ( (ThreadId > CHIPDRV_HISR_START_ID &&  ThreadId < CHIPDRV_HISR_LAST_ID),  /*assert verified*/
                  ("%s, %d, threadId: %d", __MODULE__, __LINE__, ThreadId));

    if (NULL == semaphorePtr[ThreadId -CHIPDRV_HISR_START_ID -1])
    {
        SCI_PASSERT ( (0), ("%s, %d, threadId: %d", __MODULE__, __LINE__, ThreadId));/*assert verified*/
    }

    if (SCI_ERROR == SCI_PutSemaphore (semaphorePtr[ThreadId -CHIPDRV_HISR_START_ID -1]))
    {
        //SCI_TRACE_LOW:"CHIPDRVHISR_Activate:put Semaphore Fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,INT_HAL_HISR_172_112_2_17_23_4_57_1036,(uint8*)"");
    }

    return;

}

LOCAL void  Hisr0 (uint32 i, void *d)
{
    for (;;)
    {
        TM_POINT (0xa5001230,      0);
        CHIPDRVHISR_WaitActive (CHIPDRV_HISR_0);
        TM_POINT (0xa5001230,      1);
        call_hisrs ( (Hisr_T **volatile) &hisr_header0);
        TM_POINT (0xa5001230,      2);
    }

}
LOCAL void  Hisr1 (uint32 i, void *d)
{
    for (;;)
    {
        TM_POINT (0xa5001231,      0);
        CHIPDRVHISR_WaitActive (CHIPDRV_HISR_1);
        TM_POINT (0xa5001231,      2);
        call_hisrs ( (Hisr_T **volatile) &hisr_header1);
        TM_POINT (0xa5001231,      3);
    }

}
LOCAL void  Hisr2 (uint32 i, void *d)
{
    for (;;)
    {
        TM_POINT (0xa5001232,      0);
        CHIPDRVHISR_WaitActive (CHIPDRV_HISR_2);
        TM_POINT (0xa5001232,      1);
        call_hisrs ( (Hisr_T **volatile) &hisr_header2);
        TM_POINT (0xa5001232,      2);
    }
}


LOCAL void call_hisrs (Hisr_T **volatile phisr_list)
{
    while (*phisr_list != NULL)
    {
        Hisr_T *intr;
        uint32 count;

        SCI_DisableIRQ();//Disable irq only

        intr = *phisr_list;
        *phisr_list = intr->next_hisr;
        count = intr->hisr_count;
        intr->hisr_count = 0;

        SCI_RestoreIRQ();

        if (intr->hisr_func == NULL)
        {
            SCI_PASSERT ( 0, ("%s, %d", __MODULE__, __LINE__));/*assert verified*/
        }
        else
        {
            intr->hisr_func (count, intr->data);
        }
    }
}


/*****************************************************************************/
// Description :    This function Init Hisr data struct.
// Global resource dependence :
// Author : steve.zhan
// Note :
/*****************************************************************************/
PUBLIC void Hisr_init(void)
{
    uint32 i = 0;

    hisr_header0 = NULL;
    hisr_header1 = NULL;
    hisr_header2 = NULL;

    for (i = 0; i< (uint32) MAX_ISR_NUM; ++i)
    {
        HISR_list[i].hisr_func = NULL;
        HISR_list[i].data = NULL;
        HISR_list[i].next_hisr = NULL;
        HISR_list[i].hisr_count = 0;
    }

    CHIPDRVHISR_Create ( (const uint8 *) "hisr0", CHIPDRV_HISR_0, Hisr0 , CHIPDRV_HISR_PRIO_0, 0);
    CHIPDRVHISR_Create ( (const uint8 *) "hisr1", CHIPDRV_HISR_1, Hisr1 , CHIPDRV_HISR_PRIO_1, 0);
    CHIPDRVHISR_Create ( (const uint8 *) "hisr2", CHIPDRV_HISR_2, Hisr2 , CHIPDRV_HISR_PRIO_2, 0);

}


/*****************************************************************************/
// Description :    Invoked by ISR.
// Global resource dependence :
// Author : steve.zhan
// Note :
/*****************************************************************************/
PUBLIC void Hisr_put (int Intpri)
{
    Hisr_T *intr = &HISR_list[Intpri];
    CHIPDRV_HISR_PRI_E  hisr_pri = HISR_list[Intpri].pri;
    
    if (HISR_list[Intpri].hisr_func == NULL)
    {
        SCI_PASSERT ( (0), ("hisr_func is NULL,%s, %d, Intpri=%d", __MODULE__, __LINE__, Intpri));/*assert verified*/
        return; /*lint !e527 comfirmed by xuepeng*/
    }

    if(CHIPDRV_HISR_PRIO_0 == hisr_pri)
    {
                if (intr->hisr_count++ == 0)
                {
                    intr->next_hisr = hisr_header0;
                    hisr_header0 = intr;
                }
				else
				{
                    if(NULL == hisr_header0)
                    {
                        SCI_PASSERT ( (0), ("hisr_header0 is NULL, %s, %d", __MODULE__, __LINE__));
					}
				}

                CHIPDRVHISR_Activate (CHIPDRV_HISR_0);
    }
    else if(CHIPDRV_HISR_PRIO_1 == hisr_pri)
     {
                if (intr->hisr_count++ == 0)
                {
                    intr->next_hisr = hisr_header1;
                    hisr_header1 = intr;
                }
				else
				{
                    if(NULL == hisr_header1)
                    {
                        SCI_PASSERT ( (0), ("hisr_header1 is NULL, %s, %d", __MODULE__, __LINE__));
					}
				}

                CHIPDRVHISR_Activate (CHIPDRV_HISR_1);
      }
      else if(CHIPDRV_HISR_PRIO_2 == hisr_pri)
      {
                if (intr->hisr_count++ == 0)
                {
                    intr->next_hisr = hisr_header2;
                    hisr_header2 = intr;
                }
				else
				{
                    if(NULL == hisr_header2)
                    {
                        SCI_PASSERT ( (0), ("hisr_header2 is NULL, %s, %d", __MODULE__, __LINE__));
					}
				}

                CHIPDRVHISR_Activate (CHIPDRV_HISR_2);
       }
       else
       {
            SCI_PASSERT ( (0), ("%s, %d, priority: %d", __MODULE__, __LINE__, HISR_list[Intpri].pri));/*assert verified*/
       }
}


/*****************************************************************************/
// Description :    Invokedy by interrupt module, register Hisr callback
//                  function and the priority to HISR.
// Global resource dependence :
// Author : steve.zhan
// Note :
/*****************************************************************************/
PUBLIC uint32 Hisr_RegHandler (
    uint32 priIndex,
    hisr_func_t *pHisrFunc,
    CHIPDRV_HISR_PRI_E hisrPri,
    void              *pHisrData
)
{

    if (NULL == pHisrFunc)
    {
        SCI_PASSERT ( (0), ("Hisr_RegHandler Fun is NULL,%s, %d", __MODULE__, __LINE__));/*assert verified*/
    }

    HISR_list[priIndex].hisr_func = pHisrFunc;
    HISR_list[priIndex].data = pHisrData;
    HISR_list[priIndex].pri = hisrPri;

    return 0;
}



//===========================Debug Region =================================
//==========================================================================
#ifdef HISR_INT_MONITOR_DEBUG

typedef enum
{
    sema_op = 0x1,
    mutex_op = 0x2,
    event_op = 0x3,
    signal_op = 0x4,
    sleep_op = 0x5,
} INT_HISR_CHECK_E;

extern unsigned long GetCxt (void *);
typedef struct
{
    unsigned int   CPSR;
    unsigned int   LR, SP;
    unsigned int   R0, R1, R2, R3, R4, R5, R6, R7;
    unsigned int   R8, R9, R10, R11, R12;
    unsigned int   PC;
} CXTINFO, *PCXTINFO;

LOCAL uint32 cpsr_cpsr = 0;

LOCAL void DumpCxt (PCXTINFO pCxt)
{
    cpsr_cpsr = pCxt->CPSR;
}

LOCAL CXTINFO info[2];
LOCAL int DumpCpu()
{
    PCXTINFO pCxt = info;
    GetCxt (pCxt+1);
    DumpCxt (pCxt);

    return  0;
}

LOCAL BOOLEAN isInInterrupt(void)
{
    DumpCpu();
    cpsr_cpsr = cpsr_cpsr & 0xf;

    if (cpsr_cpsr == 0x2 || cpsr_cpsr == 0x1)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

extern uint32 IrqUnitRegValue[];

LOCAL int MyStrCmp (const char *s1, const char *s2, int maxlen)
{
    int i;

    for (i = 0; i < maxlen; i++)
    {
        if (s1[i] != s2[i])
        {
            return 0;
        }

        if (s1[i] == 0 || s2[i] == 0)
        {
            return 1;
        }
    }

    return 1;
}


#endif

//DEBUG API. pls refer to INT_HISR_CHECK_E.
PUBLIC void INT_ForDebugCheckMemOp (uint32 value)
{
#ifdef HISR_INT_MONITOR_DEBUG

    if (isInInterrupt())
    {
        if ( (! (IrqUnitRegValue[0] & 0x10000))
           )
        {
            SCI_PASSERT ( (0), ("ALLOC IN INT,0x%x, 0x%x, value =0x%x\n", /*assert verified*/  \
                                IrqUnitRegValue[0] , IrqUnitRegValue[1] , value));
        }
    }

#else
    return ;
#endif
}

//arg0, pls refer to INT_HISR_CHECK_E.
PUBLIC void ISR_ForDebugCheckDelayOp (uint32 value, SCI_SEMAPHORE_PTR sem_ptr)
{
#ifdef HISR_INT_MONITOR_DEBUG
    TX_THREAD        *cur_tx_thread = NULL;
    uint32 len = 0;

    if (isInInterrupt())
    {
        SCI_PASSERT ( (0), ("Get %d In INT,%s, %d, irqstatus =0x%x\n", value, __MODULE__, __LINE__,INT_HAL_IRQGetUint1Status()));/*assert verified*/
    }

    cur_tx_thread = tx_thread_identify();

    if (cur_tx_thread == NULL)
    {
        return;
    }

    len = strlen (cur_tx_thread->tx_thread_name);

    if (len > 4) // 4 is "hisr"length.
    {
        len = 4;
    }

    if (sem_ptr == NULL && value== sema_op)
    {
        SCI_PASSERT ( (0), ("Get sema,but is NULL,%s, %d\n", __MODULE__, __LINE__));/*assert verified*/
    }

    if (value== sema_op && sem_ptr != NULL)
    {
        if (semaphorePtr[CHIPDRV_HISR_0 -CHIPDRV_HISR_START_ID -1]  == sem_ptr)
        {
        }
        else if (semaphorePtr[CHIPDRV_HISR_1 -CHIPDRV_HISR_START_ID -1]  == sem_ptr)
        {
        }
        else if (semaphorePtr[CHIPDRV_HISR_2 -CHIPDRV_HISR_START_ID -1]  == sem_ptr)
        {
        }
        else if (semaphorePtr[CHIPDRV_HISR_3 -CHIPDRV_HISR_START_ID -1]  == sem_ptr)
        {
        }
        else
        {
            if (MyStrCmp (cur_tx_thread->tx_thread_name,  "hisr", len))
            {
                SCI_PASSERT ( (0), ("Get %d In hisr,%s, %d, cur_tx_thread->tx_thread_name =%s\n", /*assert verified*/  \
                                    value, __MODULE__, __LINE__, cur_tx_thread->tx_thread_name));
            }

        }
    }
    else
    {
        if (MyStrCmp (cur_tx_thread->tx_thread_name,  "hisr", len))
        {
            SCI_PASSERT ( (0), ("Get %d In hisr,%s, %d, cur_tx_thread->tx_thread_name =%s\n", /*assert verified*/  \
                                value, __MODULE__, __LINE__, cur_tx_thread->tx_thread_name));
        }
    }

#else
    return ;
#endif
}

#ifdef PLATFORM_UWS6121E
PUBLIC Hisr_T *INT_PHY_GetIrqHisrInfo(
    uint32                  priIndex
    )
{
    return (&HISR_list[priIndex]);    
}
#endif 

/*lint +e656 confirmed by xuepeng*/


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *

**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
//end of file
