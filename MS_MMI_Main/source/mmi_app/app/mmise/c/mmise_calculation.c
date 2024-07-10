/****************************************************************************
** File Name:      se_cacluation.c                                                	                    *
** Author:           jila                                                      		            *
** Date:           2011.03.15                                             	                            *
** Description:   												          *
****************************************************************************
**                         Important Edit History                         	 	*
** ------------------------------------------------------------------------*
** DATE           	        NAME             DESCRIPTION                       	*
** 2011.03.15        	jila	
** 
****************************************************************************/
#ifndef _MMISE_CALCULATION_C_
#define _MMISE_CALCULATION_C_
#include "mmi_app_mmise_trc.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "mmi_position.h"
#include "mmk_app.h"
#include "mmise.h"
#include "mmi_appmsg.h"
#include "tasks_id.h"
#include "priority_app.h"/**---------------------------------------------------------------------------*
 **                         	Compiler Flag                                     	*
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#define CALC_TASK_STACK_SIZE                  (8 * 1024) //任务栈大小
#define CALC_TASK_QUEUE_NUM                  64   //任务消息队列大小
//任务优先级必须低于MMI Task
#define CALC_TASK_PRIORITY                    PRI_CACL_TASK  //  (SCI_PRIORITY_IDLE-1)

LOCAL BLOCK_ID s_calc_task_id = SCI_INVALID_BLOCK_ID;

LOCAL BOOLEAN s_is_task_exit = FALSE;

//记录queue的数目，防止超出
LOCAL int16 s_calc_task_queue_num = 0;



typedef struct
{
    SIGNAL_VARS
    uint32 calc_type;
    calc_cb_func calc_func;
    void *add_data;
    uint32 add_data_size;
}MMISE_CALC_SIG_T;
/*****************************************************************************/
//  Description : CalculateTaskHandle
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void CalculateTaskHandle(
                          uint32 argc, 
                          void * argv
                          )
{

    MMISE_CALC_SIG_T *signal_ptr = PNULL;

    BOOLEAN is_task_exit = FALSE;

    while (!is_task_exit)
    {
        signal_ptr = (MMISE_CALC_SIG_T *)SCI_GetSignal(SCI_IdentifyThread());
        //SCI_ASSERT(signal_ptr != PNULL);/*assert verified*/
        if (PNULL == signal_ptr)
        {
            break;
        }

        s_calc_task_queue_num--;


        switch (signal_ptr->SignalCode)
        {

        case MMISE_CALC_REQ:
            {
                SCI_ASSERT(signal_ptr->calc_func != PNULL);/*assert verified*/
                signal_ptr->calc_func(signal_ptr->calc_type,(signal_ptr->add_data),signal_ptr->add_data_size);
         
                if(signal_ptr->add_data_size)
                {
                    SCI_Free(((signal_ptr->add_data)));
                }

                break;
            }
        case MMISE_CALC_EXIT:
            {
                is_task_exit = TRUE;
                break;
            }
        default:
            {
                //SCI_TRACE_LOW:"[CalulateTaskHandle] receive UNKNOWN Signal, Code is %x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISE_CALCULATION_99_112_2_18_2_37_28_38,(uint8*)"d",signal_ptr->SignalCode);
                break;
            }
        }



        SCI_FREE(signal_ptr);

        if (is_task_exit)
        {
            break;
        }
    }

    s_calc_task_id = SCI_INVALID_BLOCK_ID;
	s_calc_task_queue_num = 0;

    SCI_ThreadExit();
}

/*****************************************************************************/
//  Description : MMISE_CalculateTaskCreate
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMISE_CalculateTaskCreate(void)
{
    if( SCI_INVALID_BLOCK_ID == s_calc_task_id )
    {
        s_calc_task_id = SCI_CreateAppThread( 
            "T_SECALC", 
            "Q_SECALC", 
            CalculateTaskHandle, 
            0, 
            0, 
            CALC_TASK_STACK_SIZE, 
            CALC_TASK_QUEUE_NUM, 
            CALC_TASK_PRIORITY, 
            SCI_PREEMPT, 
            SCI_AUTO_START);
        
        // 创建失败，进行出错处理
        //SCI_ASSERT(SCI_INVALID_BLOCK_ID != s_calc_task_id);/*assert verified*/
        if (SCI_INVALID_BLOCK_ID != s_calc_task_id)
        {
            SCI_Sleep(5);
        }
    }
}

/*****************************************************************************/
//  Description : MMISE_CalculateTaskDestroy
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMISE_CalculateTaskDestroy()
{
    xSignalHeaderRec      *signal_ptr = PNULL;
    int32 i = 0;
    
//如果超出消息队列，
    while((s_calc_task_queue_num >= CALC_TASK_QUEUE_NUM)&&(i<100))
    {
        SCI_Sleep(10);
        i++;
    }
    
    if(s_calc_task_id == SCI_INVALID_BLOCK_ID)
    {
        return;
    }
    
    if (s_calc_task_queue_num <CALC_TASK_QUEUE_NUM)
    {
        s_calc_task_queue_num ++;       
        signal_ptr = (xSignalHeaderRec *)SCI_ALLOC_APP(sizeof(xSignalHeaderRec));

        SCI_ASSERT( PNULL != signal_ptr );/*assert verified*/

        SCI_MEMSET(signal_ptr, 0x00, sizeof(xSignalHeaderRec));

        signal_ptr->SignalCode = (uint16)MMISE_CALC_EXIT;
        signal_ptr->SignalSize = sizeof( xSignalHeaderRec );
        signal_ptr->Sender = SCI_IdentifyThread();;


        SCI_SendSignal((xSignalHeader)signal_ptr, s_calc_task_id);    
        while (s_calc_task_id != SCI_INVALID_BLOCK_ID)
        {
            SCI_Sleep(10);
        }
    }
}

/*****************************************************************************/
//  Description : MMISE_StartCalculate
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMISE_StartCalculate(MMISE_CALC_PARAM_T* calc_param_ptr)
{
    MMISE_CALC_SIG_T *signal_ptr = PNULL;
    int32 i = 0;
    
//如果超出消息队列，则等待处理
    while((s_calc_task_queue_num >= CALC_TASK_QUEUE_NUM)&&(i<100))
    {
        SCI_Sleep(10);
        i++;
    }
    
    if(s_calc_task_id == SCI_INVALID_BLOCK_ID)
    {
        return;
    }  
    
    if (s_calc_task_queue_num <CALC_TASK_QUEUE_NUM)
    {
        s_calc_task_queue_num ++;
        signal_ptr = SCI_ALLOC_APP(sizeof(MMISE_CALC_SIG_T));

        if( PNULL != signal_ptr )
        {
            SCI_MEMSET(signal_ptr, 0x00, sizeof(MMISE_CALC_SIG_T));

            signal_ptr->SignalCode = (uint16)MMISE_CALC_REQ;
            signal_ptr->SignalSize = sizeof(MMISE_CALC_SIG_T);
            signal_ptr->Sender     = SCI_IdentifyThread();
            signal_ptr->Pre        = SCI_NULL;
            signal_ptr->Suc        = SCI_NULL;
            signal_ptr->calc_type = calc_param_ptr->calc_type;
            signal_ptr->calc_func = calc_param_ptr->calc_func;
            signal_ptr->add_data = calc_param_ptr->data_ptr;

            if(calc_param_ptr->data_size != 0)
            {
                signal_ptr->add_data = SCI_ALLOC_APP(calc_param_ptr->data_size);

                SCI_MEMCPY((signal_ptr->add_data), calc_param_ptr->data_ptr,calc_param_ptr->data_size);

                signal_ptr->add_data_size = calc_param_ptr->data_size;
            }

            SCI_SendSignal((xSignalHeader)signal_ptr, s_calc_task_id);
        }
    }
    
}
/*****************************************************************************/
//  Description : 算法TASK是否存在
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISE_IsCalculateTaskExist(void)
{
    return ( SCI_INVALID_BLOCK_ID != s_calc_task_id );
}
/*****************************************************************************/
//  Description : MMISE_SendMsgToMMI
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void  MMISE_SendMsgToMMI(uint16 MsgId,void* param_ptr,uint32 param_size)
{
    MmiOtherTaskMsgS *signal_ptr = PNULL;

    signal_ptr = SCI_ALLOC_APP(sizeof(MmiOtherTaskMsgS));

    SCI_MEMSET(signal_ptr, 0x00, sizeof(MmiOtherTaskMsgS));

    signal_ptr->SignalCode = (uint16)MMISE_CALC_NOTIFY;
    signal_ptr->SignalSize = sizeof(MmiOtherTaskMsgS);
    signal_ptr->Sender     = s_calc_task_id;
    signal_ptr->Pre        = SCI_NULL;
    signal_ptr->Suc        = SCI_NULL;

    signal_ptr->msg_id = MsgId;

    if(param_size != 0)
    {
        signal_ptr->param_ptr = SCI_ALLOC_APP(param_size);

        SCI_MEMCPY(signal_ptr->param_ptr, param_ptr,param_size);

        signal_ptr->size_of_param = param_size;
    }
    
    SCI_SendSignal((struct xSignalHeaderStruct *)signal_ptr, P_APP);    
}

PUBLIC void  MMISE_SendMsgToMMI_For_Livewallpaper(uint16 MsgId,void* param_ptr,uint32 param_size)
{
    MmiOtherTaskMsgS *signal_ptr = PNULL;

    signal_ptr = SCI_ALLOC_APP(sizeof(MmiOtherTaskMsgS));

    SCI_MEMSET(signal_ptr, 0x00, sizeof(MmiOtherTaskMsgS));
    
    signal_ptr->SignalCode = (uint16)MsgId;
    signal_ptr->SignalSize = sizeof(MmiOtherTaskMsgS);
    signal_ptr->Sender     = s_calc_task_id;
    signal_ptr->Pre        = SCI_NULL;
    signal_ptr->Suc = SCI_NULL;

    //signal_ptr->msg_id = MsgId;

    if(param_size != 0)
    {
        signal_ptr->param_ptr = SCI_ALLOC_APP(param_size);

        SCI_MEMCPY(signal_ptr->param_ptr, param_ptr,param_size);

        signal_ptr->size_of_param = param_size;
    }
    
    SCI_SendSignal((struct xSignalHeaderStruct *)signal_ptr, P_APP);    
}
#endif// _MMISE_CALCULATION_C_
