/******************************************************************************
 ** File Name:      freq_hal.c                                                    *
 ** Author:         Zhengjiang.Lei                                              *
 ** DATE:           01/29/2007                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file realize the frequency-changing function of the platform.          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/29/2007     Zhengjiang.Lei     Create.                                   *
  ** 08/17/2010     Yuhua.Shii     Modify.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "tx_api_thumb.h"
#include "chip_plf_export.h"
#include "freq_phy.h"
#include "priority_system.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
 **                         MACRO variables                                  *
 **---------------------------------------------------------------------------*/
//#define CHNG_FREQ_DEBUG_TRACE

#ifdef CHNG_FREQ_DEBUG_TRACE
#define CHNG_FREQ_TRACE_LOW  SCI_TRACE_LOW
#else
#define CHNG_FREQ_TRACE_LOW(...)
#endif

#define ACQUIRE_MUTEX(m)   \
    SCI_ASSERT(NULL != m);/*assert verified*/  \
    SCI_GetMutex(m, \
                 SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SCI_WAIT_FOREVER : 0)

#define RELEASE_MUTEX(m)   \
    SCI_ASSERT(NULL != m);/*assert verified*/  \
    SCI_PutMutex(m)

//note , more than 32 is forbidden. ***
#define MAX_SUPPORT_HANDLER_NUM          32               //Only support 32 handler running at the same clk.
#define VALID_HANDLE_ID_BASE             0x46524551       //ASIIC code of "FREQ"
#define INVALID_HANLER_ID                (MAX_SUPPORT_HANDLER_NUM)
#define CHNG_FREQ_REQ_HANDLER_VALID         SCI_TRUE
#define CHNG_FREQ_REQ_HANDLER_INVALID       SCI_FALSE

//Chng Freq task info
#define CHNG_FREQ_TASK_NAME             "T_P_CHNG_FREQ"
#define CHNG_FREQ_TASK_QNAME                    "Chng Freq Queue"
#define CHNG_FREQ_TASK_STACK_SIZE           (2048)
#define CHNG_FREQ_TASK_QNUM             MAX_SUPPORT_HANDLER_NUM
#define CHNG_FREQ_TASK_PRI              PRI_CHNG_FREQ_TASK   //5
#define CHNG_FREQ_TASK_PREEMPTIVE       SCI_PREEMPT
#define CHNG_FREQ_TASK_START_FLAG       SCI_AUTO_START


/**---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                 *
 **---------------------------------------------------------------------------*/
typedef struct _handler_clk_t
{
    uint32 handler_id;
    char *handler_name;             //application name,like mp3, mp4, tv_out...
    uint32 handler_clk_level;
    struct _handler_clk_t *pPrev;
    struct _handler_clk_t *pNext;
} CHNG_FREQ_HANDLER_CLK_T,*P_CHNG_FREQ_HANDLER_CLK_LIST;

typedef struct _thread_clk_mem_t
{
    CHNG_FREQ_HANDLER_CLK_T stHandlerClk;
    uint32                  valid_flag;
} CHNG_FREQ_HANDLER_T;

//define the CHNG FREQ signal data struct
typedef struct _chng_freq_sig
{
    xSignalHeaderRec     sig;       //the signal
    uint32 event_flag_index;
    CHNG_FREQ_MESSAGE_T  param; //parameter
} CHNG_FREQ_SIGNAL_T;

/**---------------------------------------------------------------------------*
 **                         Extern variables                                  *
 **---------------------------------------------------------------------------*/
extern unsigned long _tx_thread_system_state;

/**---------------------------------------------------------------------------*
 **                         Local variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL CHNG_FREQ_HANDLER_T s_chng_freq_handler_tbl[MAX_SUPPORT_HANDLER_NUM];

LOCAL CHNG_FREQ_HANDLER_CLK_T s_chng_freq_req_list =
{
    0,
    NULL,
    0,
    NULL,
    NULL
};
LOCAL SCI_MUTEX_PTR g_Mutex_handle_ptr = PNULL;

LOCAL SCI_EVENT_GROUP_PTR  s_freq_chng_event     = NULL;
LOCAL SCI_EVENT_GROUP_PTR  s_freq_chng_end_event = NULL;
/* Holds the task ID for Chng_Freq task */
LOCAL BLOCK_ID             s_chng_freq_task_id   = SCI_INVALID_BLOCK_ID;
LOCAL uint32               current_handler = NULL;


/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
LOCAL P_CHNG_FREQ_HANDLER_CLK_LIST _CHNG_FREQ_List_Search_ID (P_CHNG_FREQ_HANDLER_CLK_LIST pTCL,uint32 handler);
LOCAL uint32 _CHNG_FREQ_List_Search_MaxClk (P_CHNG_FREQ_HANDLER_CLK_LIST pTCL);
LOCAL P_CHNG_FREQ_HANDLER_CLK_LIST _CHNG_FREQ_MallocMem (uint32 handler);
LOCAL void _CHNG_FREQ_List_Add (P_CHNG_FREQ_HANDLER_CLK_LIST pTCL, uint32 handler, uint32 handler_clk_level);
LOCAL void _CHNG_FREQ_List_Delete (P_CHNG_FREQ_HANDLER_CLK_LIST pTCL);
LOCAL uint32 _CHNG_FREQ_Event_Set_All (void);
LOCAL uint32 _CHNG_FREQ_End_Event_Set_All (void);
LOCAL uint32 _CHNG_FREQ_End_Event_Set (uint32 event_id);
LOCAL uint32 _CHNG_FREQ_End_Event_Clr (uint32 event_id);
LOCAL uint32 _CHNG_FREQ_End_Event_Wait (uint32 event_id);
LOCAL uint32 _CHNG_FREQ_End_ReqFreeEvtFlagIndex (uint32 handler);
LOCAL BOOLEAN _CHNG_FREQ_SendReq (uint32 handler, CHNG_FREQ_MESSAGE_T *freq_param);
LOCAL void _CHNG_FREQ_ProcessReq (CHNG_FREQ_MESSAGE_T *freq_param);
LOCAL void _CHNG_FREQ_TaskRoutine (uint32 argc,    void *argv);
LOCAL void _CHNG_FREQ_SetArmAhbClk (uint32 handler,uint32 arm_ahb_clk_level);
LOCAL void _CHNG_FREQ_RestoreArmAhbClk (uint32 handler);

/**---------------------------------------------------------------------------*
 **                         Function defines                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    This function is used to check if the handler is in the freq-chng request list or not.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :
/*****************************************************************************/
LOCAL P_CHNG_FREQ_HANDLER_CLK_LIST _CHNG_FREQ_List_Search_ID (P_CHNG_FREQ_HANDLER_CLK_LIST pTCL,uint32 handler)
{
    P_CHNG_FREQ_HANDLER_CLK_LIST pList = pTCL;
    ACQUIRE_MUTEX (g_Mutex_handle_ptr);

    while (pList->handler_id!= handler)
    {
        pList = pList->pNext;

        if (pList == pTCL)
        {
            RELEASE_MUTEX (g_Mutex_handle_ptr);
            return NULL;
        }
    }

    RELEASE_MUTEX (g_Mutex_handle_ptr);
    return pList;
}

/*****************************************************************************/
// Description :    This function is used to search the max clk level in the freq-chng requst list.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :           Search max ahb_clk,the higher clk level, the higher ahb_clk is.
/*****************************************************************************/
LOCAL uint32 _CHNG_FREQ_List_Search_MaxClk (P_CHNG_FREQ_HANDLER_CLK_LIST pTCL)
{
    P_CHNG_FREQ_HANDLER_CLK_LIST pList = pTCL;
    uint32 max_clk_level = pTCL->handler_clk_level;
    ACQUIRE_MUTEX (g_Mutex_handle_ptr);

    do
    {
        pList = pList->pNext;

        if (max_clk_level < pList->handler_clk_level)
        {
            max_clk_level = pList->handler_clk_level;
        }

    }
    while (pTCL!=pList);

    RELEASE_MUTEX (g_Mutex_handle_ptr);
    return max_clk_level;
}

/*****************************************************************************/
// Description :    This function is used to get the handler structure pointer.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :
/*****************************************************************************/
LOCAL P_CHNG_FREQ_HANDLER_CLK_LIST _CHNG_FREQ_MallocMem (uint32 handler)
{
    uint32 index = handler - VALID_HANDLE_ID_BASE;

    if (CHNG_FREQ_REQ_HANDLER_VALID == s_chng_freq_handler_tbl[index].valid_flag)
    {
        return & (s_chng_freq_handler_tbl[index].stHandlerClk);
    }
    else
    {
        return NULL;
    }
}

/*****************************************************************************/
// Description :    This function is used to add a node to the freq-chng requst list.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :
/*****************************************************************************/
LOCAL void _CHNG_FREQ_List_Add (P_CHNG_FREQ_HANDLER_CLK_LIST pTCL, uint32 handler, uint32 handler_clk_level)
{
    P_CHNG_FREQ_HANDLER_CLK_LIST pList = pTCL;
    P_CHNG_FREQ_HANDLER_CLK_LIST pNewList = NULL;

    ACQUIRE_MUTEX (g_Mutex_handle_ptr);
    //Alloc a unit of CHNG_FREQ_HANDLER_CLK_T
    pNewList = _CHNG_FREQ_MallocMem (handler);
    SCI_ASSERT (pNewList != NULL);/*assert verified*/

    pList->pNext->pPrev = pNewList;
    pNewList->pNext = pList->pNext;
    pList->pNext = pNewList;
    pNewList->pPrev = pList;
    pNewList->handler_id = handler;
    pNewList->handler_clk_level = handler_clk_level;

    RELEASE_MUTEX (g_Mutex_handle_ptr);
}

/*****************************************************************************/
// Description :    This function is used to delete a node from the freq-chng req list.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :
/*****************************************************************************/
LOCAL void _CHNG_FREQ_List_Delete (P_CHNG_FREQ_HANDLER_CLK_LIST pTCL)
{
    P_CHNG_FREQ_HANDLER_CLK_LIST pList = pTCL;

    ACQUIRE_MUTEX (g_Mutex_handle_ptr);
    pList->pPrev->pNext = pList->pNext;
    pList->pNext->pPrev = pList->pPrev;

    RELEASE_MUTEX (g_Mutex_handle_ptr);
}


/*****************************************************************************/
// Description :    This function is used to get a handler which specify the application
//                  that request the frequency-changing.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :           before an application request to change frequency, get a handler first.
/*****************************************************************************/
PUBLIC uint32 CHNG_FREQ_GetReqHandler (const char *handler_name)
{
    uint32 i;

    SCI_ASSERT (PNULL != handler_name);/*assert verified*/

    /*
        if an application has got a handler already, is this application request a handler
        agian when it didn't delete this handler, assert it.
    */

    for (i=0; i<MAX_SUPPORT_HANDLER_NUM; i++)
    {
        if (CHNG_FREQ_REQ_HANDLER_VALID == s_chng_freq_handler_tbl[i].valid_flag)
        {
            if (! (strcmp (s_chng_freq_handler_tbl[i].stHandlerClk.handler_name,handler_name)))
            {
                SCI_PASSERT (0, ("%s get chng freq handler error !", *handler_name));/*assert to do*/
            }
        }
    }

    for (i=0; i<MAX_SUPPORT_HANDLER_NUM; i++)
    {
        if (CHNG_FREQ_REQ_HANDLER_INVALID == s_chng_freq_handler_tbl[i].valid_flag)
        {
            s_chng_freq_handler_tbl[i].valid_flag               = CHNG_FREQ_REQ_HANDLER_VALID;
            s_chng_freq_handler_tbl[i].stHandlerClk.handler_name = (char *) handler_name;
            return (i + VALID_HANDLE_ID_BASE);
        }
    }

    return CHNG_FREQ_REQ_HANDLER_NULL;
}

/*****************************************************************************/
// Description :    This function is used to delete the handler which request to change frequency
//                  before .
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :           .
/*****************************************************************************/
PUBLIC uint32 CHNG_FREQ_DeleteReqHandler (uint32 handler)
{
    uint32 index;

    index = handler - VALID_HANDLE_ID_BASE;

    SCI_ASSERT (index < MAX_SUPPORT_HANDLER_NUM);/*assert verified*/
    SCI_ASSERT (CHNG_FREQ_REQ_HANDLER_VALID == s_chng_freq_handler_tbl[index].valid_flag);/*assert verified*/

    SCI_ASSERT (PNULL != s_chng_freq_handler_tbl[index].stHandlerClk.handler_name);/*assert verified*/

    s_chng_freq_handler_tbl[index].valid_flag = CHNG_FREQ_REQ_HANDLER_INVALID;
    s_chng_freq_handler_tbl[index].stHandlerClk.handler_name = PNULL;

    return 0;

}

/*****************************************************************************/
// Description :    This function is used to set all event which specify permits
//                  to change frequency.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :           not permit to call SCI_SetEvent() in system init.****
/*****************************************************************************/
LOCAL uint32 _CHNG_FREQ_Event_Set_All (void)
{
    uint32 status_E;

    if (SCI_NULL == s_freq_chng_event)
    {
        return TX_NO_EVENTS;
    }

    status_E = SCI_SetEvent (s_freq_chng_event, FREQ_CHNG_EVENT_ALL, SCI_OR);

    CHNG_FREQ_TRACE_LOW ("_CHNG_FREQ_Event_Set_All(),after set,event flag: 0x%x.", ( (TX_EVENT_FLAGS_GROUP *) s_freq_chng_event)->tx_event_flags_current);

    if (TX_SUCCESS != status_E)
    {
        SCI_ASSERT (0);/*assert to do*/
    }

    return status_E;
}
/*****************************************************************************/
// Description :    This function is used to set an event which specify this event
//                  permits to change frequency. after this event finished accessing
//                  sdram, set the event.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :           sys_mode: when you call this function in an isr handler,
//                          use SYS_MODE_INT.
/*****************************************************************************/
PUBLIC uint32 CHNG_FREQ_Event_Set (FREQ_CHNG_EVENT_E id, SYS_MODE_E sys_mode)
{
    uint32 status_E = 0;

    if (SCI_NULL == s_freq_chng_event)
    {
        return TX_NO_EVENTS;
    }

    //not permit to call SCI_SetEvent() in system init.
    if ( (0 == _tx_thread_system_state) || (sys_mode == SYS_MODE_INT))
    {
        status_E = SCI_SetEvent (s_freq_chng_event, (1<<id), SCI_OR);

        CHNG_FREQ_TRACE_LOW ("CHNG_FREQ_Event_Set(), id:0x%x,after set,event flag: 0x%x.",id, ( (TX_EVENT_FLAGS_GROUP *) s_freq_chng_event)->tx_event_flags_current);

        if (TX_SUCCESS != status_E)
        {
            SCI_ASSERT (0);/*assert to do*/
        }
    }

    return status_E;
}
/*****************************************************************************/
// Description :    This function is used to clear an event which specify this event
//                  forbids to change frequency. before this event will begin to access
//                  sdram, clear the event.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :           sys_mode: when you call this function in an isr handler,
//                          use SYS_MODE_INT.
/*****************************************************************************/
PUBLIC uint32 CHNG_FREQ_Event_Clr (FREQ_CHNG_EVENT_E id, SYS_MODE_E sys_mode)
{
    uint32 status_E;

    status_E = 0;

    if (SCI_NULL == s_freq_chng_event)
    {
        return TX_NO_EVENTS;
    }

    //not permit to call SCI_SetEvent() in system init.
    if ( (0 == _tx_thread_system_state) || (sys_mode == SYS_MODE_INT))
    {
        status_E = SCI_SetEvent (s_freq_chng_event, (~ (1UL<<id)), SCI_AND);

        CHNG_FREQ_TRACE_LOW ("CHNG_FREQ_Event_Clr(), id:0x%x,after clear,event flag: 0x%x.",id, ( (TX_EVENT_FLAGS_GROUP *) s_freq_chng_event)->tx_event_flags_current);

        if (TX_SUCCESS != status_E)
        {
            SCI_ASSERT (0);/*assert to do*/
        }
    }

    return status_E;
}

/*****************************************************************************/
// Description :    This function is used to set all event which specify permits
//                  to change frequency.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :           not permit to call SCI_SetEvent() in system init.****
/*****************************************************************************/
LOCAL uint32 _CHNG_FREQ_End_Event_Set_All (void)
{
    uint32 status_E;
    if (SCI_NULL == s_freq_chng_end_event)
    {
        return TX_NO_EVENTS;
    }

    status_E = SCI_SetEvent (s_freq_chng_end_event, (1<<(MAX_SUPPORT_HANDLER_NUM)) - 1, SCI_OR);/*lint !e648 !e778 confirmed by yuhua*/

    CHNG_FREQ_TRACE_LOW ("_CHNG_FREQ_End_Event_Set_All(),after set,event flag: 0x%x.", ( (TX_EVENT_FLAGS_GROUP *) s_freq_chng_end_event)->tx_event_flags_current);

    if (TX_SUCCESS != status_E)
    {
        SCI_ASSERT (0);/*assert to do*/
    }

    return status_E;
}

/*****************************************************************************/
// Description :    This function is used to set an event which specify an application
//                  of freq-chng can can go down.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :
/*****************************************************************************/
LOCAL uint32 _CHNG_FREQ_End_Event_Set (uint32 event_id)
{
    uint32 status_E;

    SCI_ASSERT (event_id <= MAX_SUPPORT_HANDLER_NUM);/*assert verified*/

    if (SCI_NULL == s_freq_chng_end_event)
    {
        return TX_NO_EVENTS;
    }

    status_E = SCI_SetEvent (s_freq_chng_end_event, (1<<event_id), SCI_OR);

    CHNG_FREQ_TRACE_LOW ("_CHNG_FREQ_End_Event_Set(), event_id:0x%x,after set,event flag: 0x%x.",event_id, ( (TX_EVENT_FLAGS_GROUP *) s_freq_chng_end_event)->tx_event_flags_current);

    if (TX_SUCCESS != status_E)
    {
        SCI_ASSERT (0);/*assert to do*/
    }

    return status_E;
}

/*****************************************************************************/
// Description :    This function is used to clear an event which specify an application
//                  of freq-chng.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :
/*****************************************************************************/
LOCAL uint32 _CHNG_FREQ_End_Event_Clr (uint32 event_id)
{
    uint32 status_E;

    SCI_ASSERT (event_id <= MAX_SUPPORT_HANDLER_NUM);/*assert verified*/

    if (SCI_NULL == s_freq_chng_end_event)
    {
        return TX_NO_EVENTS;
    }

    status_E = SCI_SetEvent (s_freq_chng_end_event, (~ (1UL<<event_id)), SCI_AND);

    CHNG_FREQ_TRACE_LOW ("_CHNG_FREQ_End_Event_Clr(), event_id:0x%x,after clear,event flag: 0x%x.",event_id, ( (TX_EVENT_FLAGS_GROUP *) s_freq_chng_end_event)->tx_event_flags_current);

    if (TX_SUCCESS != status_E)
    {
        SCI_ASSERT (0);/*assert to do*/
    }

    return status_E;
}

/*****************************************************************************/
// Description :    This function is used to clear an event which specify an application
//                  of freq-chng.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :
/*****************************************************************************/
LOCAL uint32 _CHNG_FREQ_End_Event_Wait (uint32 event_id)
{
    uint32 actual_event,status;

    SCI_ASSERT (event_id <= MAX_SUPPORT_HANDLER_NUM);/*assert verified*/

    if (SCI_NULL == s_freq_chng_end_event)
    {
        SCI_ASSERT (0);/*assert verified*/
    }

    //get the event to specify changing frequency end.
    CHNG_FREQ_TRACE_LOW ("befor get %x ,current chng_freq_end event flag: 0x%x.",event_id, ( (TX_EVENT_FLAGS_GROUP *) s_freq_chng_end_event)->tx_event_flags_current);
    status = SCI_GetEvent (s_freq_chng_end_event,
                           (1 << event_id),
                           SCI_AND,
                           &actual_event,
                           SCI_WAIT_FOREVER);
    return 0;

}
/*****************************************************************************/
// Description :    This function is used to request an event flag id from the CHNG_FREQ_END_EVT group.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :           when an application want to chang frequency, it should allocate an event
//                  of CHNG_FREQ_END to specify whether it's freq-chnging is finished or not.
//return:           return an event flag index of the CHNG_FREQ_END event group which is not uesd now.
/*****************************************************************************/
LOCAL uint32 _CHNG_FREQ_End_ReqFreeEvtFlagIndex (uint32 handler)
{
    uint32 i,current_evt_flag;
    uint32 index = handler - VALID_HANDLE_ID_BASE;

    SCI_ASSERT (index < MAX_SUPPORT_HANDLER_NUM);/*assert verified*/

    if (SCI_NULL == s_freq_chng_end_event)
    {
        return INVALID_HANLER_ID;
    }

    //  SCI_DisableIRQ();  // ????
    current_evt_flag = ( (TX_EVENT_FLAGS_GROUP *) s_freq_chng_end_event)->tx_event_flags_current;
    CHNG_FREQ_TRACE_LOW ("befor get %x ,current chng_freq_end event flag: 0x%x.",index, ( (TX_EVENT_FLAGS_GROUP *) s_freq_chng_end_event)->tx_event_flags_current);

    for (i=0; i< MAX_SUPPORT_HANDLER_NUM; i++)
    {
        // are there any event is free now in the CHNG_FREQ_END_EVENT group? if get one, return index.
        if (current_evt_flag & (1 << i))
        {
            return i;
        }
    }

    //  SCI_RestoreIRQ();

    return INVALID_HANLER_ID;   
}

/*****************************************************************************/
//  Description:    This function is used to send the signal to CHNG FREQ task
//  Author:         Zhengjiang.Lei
//  Note:
//****************************************************************************/
LOCAL BOOLEAN _CHNG_FREQ_SendReq (uint32 handler, CHNG_FREQ_MESSAGE_T *freq_param)
{
    uint32 event_flag_id;
    CHNG_FREQ_SIGNAL_T *chng_freq_sig_ptr = SCI_NULL;
    uint32 index = handler - VALID_HANDLE_ID_BASE;

    SCI_ASSERT (index < MAX_SUPPORT_HANDLER_NUM);/*assert verified*/
    SCI_ASSERT (SCI_NULL != s_chng_freq_task_id);/*assert verified*/

    chng_freq_sig_ptr = (CHNG_FREQ_SIGNAL_T *) SCI_ALLOC (sizeof (CHNG_FREQ_SIGNAL_T));
    SCI_ASSERT (SCI_NULL != chng_freq_sig_ptr);/*assert verified*/
    SCI_MEMSET (chng_freq_sig_ptr, 0, sizeof (CHNG_FREQ_SIGNAL_T));

    /*
        request an event flag first for the application which request to change frequency.
        the application is specified by handler. if get an free event flag, clear it first.
    */
    //CHNG_FREQ_TRACE_LOW("***before req evt index,current evt flag: 0x%x.",((TX_EVENT_FLAGS_GROUP*)s_freq_chng_end_event)->tx_event_flags_current);//temp **********************

    event_flag_id = _CHNG_FREQ_End_ReqFreeEvtFlagIndex (handler);

    if (INVALID_HANLER_ID == event_flag_id)
    {
        SCI_PASSERT (0, ("No free evt flag in CHNG_FREQ_END evt!"));/*assert to do*/
    }

    _CHNG_FREQ_End_Event_Clr (event_flag_id);

    //CHNG_FREQ_TRACE_LOW("***get evt flg index: %d,clr it,current evt flag: 0x%x.",event_flag_id,((TX_EVENT_FLAGS_GROUP*)s_freq_chng_end_event)->tx_event_flags_current);//temp **********************

    //init the signal
    chng_freq_sig_ptr->sig.SignalCode = CHNG_FREQ_REQ_CMD;
    chng_freq_sig_ptr->sig.SignalSize = sizeof (CHNG_FREQ_SIGNAL_T);
    chng_freq_sig_ptr->sig.Sender     = handler;
    chng_freq_sig_ptr->param = *freq_param;
    chng_freq_sig_ptr->event_flag_index = event_flag_id;

    if (SCI_SUCCESS == SCI_SendSignal ( (xSignalHeader) chng_freq_sig_ptr, s_chng_freq_task_id))
    {
        _CHNG_FREQ_End_Event_Wait (event_flag_id);
        return TRUE;
    }
    else
    {
        SCI_ASSERT (FALSE);/*assert to do*/
        return FALSE; /*lint !e527 comfirmed by xuepeng*/
    }
}

/*********************************************************************************************/
//Description:   This function is used to change ARM_CLK and AHB_CLK. By now,for any MPLL_CLK,
//               we can suport 10 level arm/ahb clock choice. And,we can choose a MPLL_CLK when
//               system initial,and never change it when running.
//Author:        Nick.Zhao
//Note:          Zhengjiang.Lei update for PL6800.
//Parameter:     arm_ahb_div:  the arm/ahb clk dividor to be set. like 0x0100,0x0502 etc.
//               sdram_timing: the sdram timing value to be set.
/*********************************************************************************************/
LOCAL void _CHNG_FREQ_ProcessReq (CHNG_FREQ_MESSAGE_T *freq_param)
{

    uint32 actual_event,status;

    TX_INTERRUPT_SAVE_AREA

    //get the event to change the arm_clk and ahb_clk really.
    CHNG_FREQ_TRACE_LOW ("befor get,current event flag: 0x%x.", ( (TX_EVENT_FLAGS_GROUP *) s_freq_chng_event)->tx_event_flags_current);

    while (freq_param->freq_param4)
    {
        status = SCI_GetEvent (s_freq_chng_event,
                               (FREQ_CHNG_EVENT_ALL),
                               SCI_AND,
                               &actual_event,
                               SCI_WAIT_FOREVER);

        TX_DISABLE_FIQ_IRQ

        if ( ( (TX_EVENT_FLAGS_GROUP *) s_freq_chng_event)->tx_event_flags_current == FREQ_CHNG_EVENT_ALL)
        {
            TX_RESTORE
            break;
        }

        TX_RESTORE

    }


    FREQ_PHY_ChangeClk (freq_param);

}

/*****************************************************************************/
// Description :    Chng Freq task routine.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :
/*****************************************************************************/
LOCAL void _CHNG_FREQ_TaskRoutine
(
    uint32 argc,
    void *argv
)
{
    //  BOOLEAN bTaskRun = SCI_TRUE;
    CHNG_FREQ_SIGNAL_T *signal;

    CHNG_FREQ_TRACE_LOW ("CHNG_FREQ task entry !");

    for (;;)
    {
        signal = (CHNG_FREQ_SIGNAL_T *) SCI_GetSignal (s_chng_freq_task_id);
        SCI_ASSERT (PNULL != signal);/*assert verified*/

        switch (signal->sig.SignalCode)
        {
            case CHNG_FREQ_REQ_CMD:
                _CHNG_FREQ_ProcessReq (& (signal->param));

                CHNG_FREQ_TRACE_LOW ("***before set evt flg id: %d,current evt flag: 0x%x.",signal->event_flag_index, ( (TX_EVENT_FLAGS_GROUP *) s_freq_chng_end_event)->tx_event_flags_current); //temp **********************
                _CHNG_FREQ_End_Event_Set (signal->event_flag_index);
                CHNG_FREQ_TRACE_LOW ("***after set evt flg id: %d,current evt flag: 0x%x.",signal->event_flag_index, ( (TX_EVENT_FLAGS_GROUP *) s_freq_chng_end_event)->tx_event_flags_current); //temp **********************

                break;
            default:
                SCI_PASSERT (0, ("SIG_CODE:%d, sender id: 0x%x.",signal->sig.SignalCode,signal->sig.Sender));/*assert verified*/
                break;
        }

        SCI_FREE (signal);
    }
}

/*********************************************************************************************/
//Description:   This function is used to specify the clock ARM_CLK and AHB_CLK..
//Author:        Zhengjiang.Lei
//Note:
//Parameter:     handler: the handler which specify the application that request to change freq.
//               arm_ahb_clk_level: the parameter is use to specify the clock level of arm/ahb clock
//               that you want to config,
//               Note, use MACRO to config this parameter according the MPLL you
//               specified before.
/*********************************************************************************************/
LOCAL void _CHNG_FREQ_SetArmAhbClk (uint32 handler,uint32 arm_ahb_clk_level)
{
    uint32 index = handler - VALID_HANDLE_ID_BASE;
    char *pHandler_name = PNULL;
    P_CHNG_FREQ_HANDLER_CLK_LIST pList = NULL;
    uint32 max_clk_level = 0;

    SCI_ASSERT (index < MAX_SUPPORT_HANDLER_NUM);/*assert verified*/
    SCI_PASSERT (FREQ_PHY_DetectClkValidity (arm_ahb_clk_level), ("Invalid clock level value (%d)!", arm_ahb_clk_level));/*assert verified*/
    SCI_ASSERT (CHNG_FREQ_REQ_HANDLER_VALID == s_chng_freq_handler_tbl[index].valid_flag);/*assert verified*/

    pHandler_name = s_chng_freq_handler_tbl[index].stHandlerClk.handler_name;
    SCI_ASSERT (PNULL != pHandler_name);/*assert verified*/

    CHNG_FREQ_TRACE_LOW ("Enter _CHNG_FREQ_SetArmAhbClk(): %s,handler index=0x%x,clklevel=%d",pHandler_name,index,arm_ahb_clk_level);

    pList = _CHNG_FREQ_List_Search_ID (&s_chng_freq_req_list, handler);

    if (NULL == pList)
    {
        _CHNG_FREQ_List_Add (&s_chng_freq_req_list,handler,arm_ahb_clk_level);
        CHNG_FREQ_TRACE_LOW ("add a node");
    }
    else
    {
        pList->handler_clk_level = arm_ahb_clk_level;
    }

    ACQUIRE_MUTEX (g_Mutex_handle_ptr);
    max_clk_level = _CHNG_FREQ_List_Search_MaxClk (&s_chng_freq_req_list);

    if (arm_ahb_clk_level < max_clk_level)
    {
        CHNG_FREQ_TRACE_LOW ("set lower level than the max in list,return");
        RELEASE_MUTEX (g_Mutex_handle_ptr);
        return;
    }

    RELEASE_MUTEX (g_Mutex_handle_ptr);

    FREQ_PHY_BeforeChangeClk (handler, arm_ahb_clk_level, _CHNG_FREQ_SendReq);

}
/*****************************************************************************/
// Description :    This function is used to invalidate the freq-chng request of
//                  the current task which call _CHNG_FREQ_SetArmAhbClk() before.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :           you must call this function in the same task which call
//                  _CHNG_FREQ_SetArmAhbClk() before.
/*****************************************************************************/
LOCAL void _CHNG_FREQ_RestoreArmAhbClk (uint32 handler)
{
    uint32 cur_max_clk_level;
    uint32 index = handler - VALID_HANDLE_ID_BASE;
    char *pHandler_name = PNULL;
    P_CHNG_FREQ_HANDLER_CLK_LIST pList = NULL;

    SCI_ASSERT (index < MAX_SUPPORT_HANDLER_NUM);/*assert verified*/
    SCI_ASSERT (CHNG_FREQ_REQ_HANDLER_VALID == s_chng_freq_handler_tbl[index].valid_flag);/*assert verified*/

    pHandler_name = s_chng_freq_handler_tbl[index].stHandlerClk.handler_name;
    SCI_ASSERT (PNULL != pHandler_name);/*assert verified*/

    CHNG_FREQ_TRACE_LOW ("_CHNG_FREQ_RestoreArmAhbClk(),%s, handler index=0x%x",pHandler_name,index);

    pList = _CHNG_FREQ_List_Search_ID (&s_chng_freq_req_list,handler);

    if (NULL == pList)
    {
        CHNG_FREQ_TRACE_LOW ("pList==NULL,return.");
        return;
    }

    _CHNG_FREQ_List_Delete (pList);
    CHNG_FREQ_TRACE_LOW ("delete a node.");

    cur_max_clk_level = _CHNG_FREQ_List_Search_MaxClk (&s_chng_freq_req_list);

    FREQ_PHY_BeforeChangeClk (handler, cur_max_clk_level, _CHNG_FREQ_SendReq);
}

/*****************************************************************************/
// Description :    This function is used to init the chng_freq module.
// Global resource dependence :
// Author :         Zhengjiang.Lei
// Note :           you must call this function after RTOS init.
/*****************************************************************************/
PUBLIC void CHNG_FREQ_Init (void)
{
    uint32 i;

    if ( (NULL==s_chng_freq_req_list.pPrev) && (NULL==s_chng_freq_req_list.pNext))
    {
        //Initialize the linklist
        s_chng_freq_req_list.handler_id = CHNG_FREQ_REQ_HANDLER_NULL;

        //here default set is for idle running freq, r/h/i should be min level
        s_chng_freq_req_list.handler_clk_level = FREQ_PHY_GetMinClkLevel();

        s_chng_freq_req_list.pPrev = &s_chng_freq_req_list;
        s_chng_freq_req_list.pNext = &s_chng_freq_req_list;

        //Initialize the s_chng_freq_handler_tbl
        for (i=0; i<MAX_SUPPORT_HANDLER_NUM; i++)
        {
            s_chng_freq_handler_tbl[i].stHandlerClk.handler_name = PNULL;
            s_chng_freq_handler_tbl[i].valid_flag = CHNG_FREQ_REQ_HANDLER_INVALID;
        }

        //Initialize the mutex
        g_Mutex_handle_ptr = SCI_CreateMutex ("CHIP_CLK_MUTEX", SCI_INHERIT);

        if (SCI_NULL == g_Mutex_handle_ptr)
        {
            SCI_ASSERT (0);/*assert verified*/
        }

        //create a event for the freq-changing application.
        s_freq_chng_event = SCI_CreateEvent ("FREQ_CHNG_EVENT");

        if (SCI_NULL == s_freq_chng_event)
        {
            SCI_ASSERT (0);/*assert verified*/
        }

        _CHNG_FREQ_Event_Set_All();

        //create a event for freq-change ending sync.
        s_freq_chng_end_event = SCI_CreateEvent ("FREQ_CHNG_END_EVENT");

        if (SCI_NULL == s_freq_chng_end_event)
        {
            SCI_ASSERT (0);/*assert verified*/
        }

        _CHNG_FREQ_End_Event_Set_All();

        s_chng_freq_task_id = SCI_CreateThread (CHNG_FREQ_TASK_NAME, CHNG_FREQ_TASK_QNAME, _CHNG_FREQ_TaskRoutine,
                                                0, NULL, CHNG_FREQ_TASK_STACK_SIZE, CHNG_FREQ_TASK_QNUM,
                                                CHNG_FREQ_TASK_PRI, CHNG_FREQ_TASK_PREEMPTIVE,
                                                CHNG_FREQ_TASK_START_FLAG);
        SCI_ASSERT (SCI_INVALID_BLOCK_ID != s_chng_freq_task_id);/*assert verified*/
    }
}

/*****************************************************************************/
// Description :    This function is used to set the customized freq config(base address)
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC void CHNG_FREQ_SetCustomFreqCfg (uint32 *p_customized_freq_ptr)
{
    SCI_ASSERT (p_customized_freq_ptr != PNULL);/*assert verified*/

    FREQ_PHY_SetCustomFreqCfg (p_customized_freq_ptr);
}



/*****************************************************************************/
// Description :    This function is used to change the Arm Clk(may contain the AHB Clk)
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC void CHNG_FREQ_SetArmClk (uint32 handler, CHNG_FREQ_INDEX_E freq_type_index)
{
    uint32 arm_ahb_clk_level = 0;
    //get the clk level according the input freq type index
    arm_ahb_clk_level = FREQ_PHY_GetClkLevel (freq_type_index);
    //SCI_TRACE_LOW:"chng_freq_c.c CHNG_FREQ_SetArmClk %d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FREQ_HAL_854_112_2_17_23_4_31_965,(uint8*)"d", arm_ahb_clk_level);
    //set the arm/ahb clk
    _CHNG_FREQ_SetArmAhbClk (handler,arm_ahb_clk_level);
}

/*****************************************************************************/
// Description :    This function is used to restore the original ARM_CLK/AHB_CLK
// Global resource dependence :
// Author :         Nick.Zhao
// Note :
/*****************************************************************************/
PUBLIC void CHNG_FREQ_RestoreARMClk (uint32 handler)
{
    //restore arm/ahb clk
    _CHNG_FREQ_RestoreArmAhbClk (handler);
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
