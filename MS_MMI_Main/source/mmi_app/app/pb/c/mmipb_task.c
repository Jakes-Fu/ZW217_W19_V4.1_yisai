/****************************************************************************
** File Name:      mmipb_task.c                                            *
** Author:                                                                 *
** Date:           11/09/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file  define task of phonebook.
**                 这个文件PB task中的消息接受，处理和管理                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2011        baokun yin         Create
** 
****************************************************************************/
#define _MMIPB_TASK_C_  

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_app_pb_trc.h"
#include "mmipb_task.h"
#include "tasks_id.h"
#include "mmipb_interface.h"
#include "mmi_appmsg.h"
#include "os_api.h"
#include "priority_app.h"
#include "mmipb_app.h"
#if defined(SEARCH_SUPPORT) && defined(MMIPB_SEARCH_CONTACT_CONTENT) 
#include "mmipb_search.h"
#endif
#ifdef BT_PBAP_SUPPORT
#include "mmicl_export.h"
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
// pb stack and queue define
// #ifdef PB_SUPPORT_LOW_MEMORY
// #define   MMIPB_TASK_STACK_SIZE         (1024*4) //pb task　stack size
// #else
#define   MMIPB_TASK_STACK_SIZE         (1024*8) //pb task　stack size
// #endif
#define   MMIPB_TASK_QUEUE_NUM          8    //pb task queue num
#define   MMIPB_TASK_PRIORITY           PRI_APP_PB_TASK //pb task优先级
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : PB task的入口函数
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void PBTask_Entry(
                        uint32 argc, 
                        void * argv
                        );

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/   
LOCAL BLOCK_ID g_mmipb_pb_task_id = 0;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : PB task的入口函数
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void PBTask_Entry(
                        uint32 argc, 
                        void * argv
                        )
{
    xSignalHeaderRec    *psig = NULL;
    MMIPB_SIGNAL_T      *pb_signal = PNULL;
    MMIPB_HANDLE_NODE_T handle_node = {0};
    MMIPB_ERROR_E       cmd_ret = MMIPB_ERROR_ERROR;
    MMIPB_CNF_MSG_T     cnf_msg = {0};

    while (1)/*lint !e716*/
    {
        MMI_RECEIVE_SIGNAL(psig, SCI_IdentifyThread());/*lint !e63*/
        pb_signal = (MMIPB_SIGNAL_T *)psig;
        //psig = s_cur_sig;
		SCI_TRACE_LOW("[MMIPB]PBTask_Entry psig->SignalCode = 0x%x", psig->SignalCode);/*lint !e613*/
        switch(psig->SignalCode) /*lint !e613*/
        {
        case MMIPB_SIG_SORT:
            //排序
            MMIPB_InitEntryList();
            //排序结束，通知MMI
            if(pb_signal != PNULL)
            {
                cnf_msg.handle = pb_signal->handle;
            }
            cnf_msg.ret = MMIPB_ERROR_SUCCESS;
            MMIPB_SendSignalToMMITask(MMI_PB_SORT_CNF, &cnf_msg);
            break;
        case MMIPB_SIG_READ:
            //读请求
            if(pb_signal != PNULL)
            {
                handle_node = pb_signal->handle_node;
                cmd_ret = MMIPB_Read(
                    handle_node.access_type,
                    handle_node.access_point,
                    handle_node.data_len,
                    handle_node.data_ptr
                    );
                
                cnf_msg.handle = pb_signal->handle;
                cnf_msg.handle_node = handle_node;
                cnf_msg.ret = cmd_ret;
                MMIPB_SendSignalToMMITask(MMI_PB_READ_CNF, &cnf_msg);
            }
            break;
        case MMIPB_SIG_WRITE:
            //写请求
            if(pb_signal != PNULL)
            {
                handle_node = pb_signal->handle_node;
                {
                    cmd_ret = MMIPB_Write(
                               handle_node.access_type,
                               handle_node.access_point,
                               handle_node.data_len,
                               handle_node.data_ptr
                              );
                    cnf_msg.handle = pb_signal->handle;
                }
                cnf_msg.ret = cmd_ret;
                cnf_msg.handle_node = handle_node;
                MMIPB_SendSignalToMMITask(MMI_PB_WRITE_CNF, &cnf_msg);
            }
	         break;
        case MMIPB_SIG_DELETE:
            //删除请求  
            if(pb_signal != PNULL)
            {
                handle_node = pb_signal->handle_node;
                {
                    cmd_ret = MMIPB_Delete(
                               handle_node.access_type,
                               handle_node.access_point
                              );
                    cnf_msg.handle = pb_signal->handle;
                    cnf_msg.ret = cmd_ret;
                    cnf_msg.handle_node = handle_node;
                    MMIPB_SendSignalToMMITask(MMI_PB_DELETE_CNF, &cnf_msg);
                }
            }
            break;
#ifdef  SNS_SUPPORT            
        case MMIPB_SIG_SNS_UPDATE:
            SCI_TRACE_LOW("[MMIPB]PBTask_Entry MMIPB_SIG_SNS_UPDATE "); 
            MMIPB_LoadSnsContact();
            if(pb_signal != PNULL)
            {
                cnf_msg.handle = pb_signal->handle;
            }
            cnf_msg.ret = MMIPB_ERROR_SUCCESS;
            MMIPB_SendSignalToMMITask(MMI_PB_SORT_CNF, &cnf_msg);
            break;
        case MMIPB_SIG_SNS_READ_ACTIVE:
            SCI_TRACE_LOW("[MMIPB]PBTask_Entry MMIPB_SIG_SNS_READ_ACTIVE "); 
            if(pb_signal != PNULL)
            {
                cnf_msg.handle = pb_signal->handle;
            }
            cnf_msg.ret = cmd_ret;
            MMIPB_SendSignalToMMITask(MMI_PB_UPDATE_SNS_ACTIVITY_CNF, &cnf_msg);
            break;
#endif            
#ifdef SEARCH_SUPPORT
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
        case MMIPB_SIG_SEARCH:
            if(pb_signal != PNULL)
            {
                handle_node = pb_signal->handle_node;
                {
                    MMI_STRING_T   search_str = {0};
					uint16 num = 0;
                    search_str.wstr_ptr = ((MMIPB_SEARCH_DATA_T*)(handle_node.data_ptr))->search_str;
                    search_str.wstr_len = MMIAPICOM_Wstrlen(((MMIPB_SEARCH_DATA_T*)(handle_node.data_ptr))->search_str);
                    num = MMIPB_MultiSearch(&search_str, MMIPB_ALL_GROUP_MASK, 0, MMIPB_SEARCH_NAME+MMIPB_SEARCH_NUMBER+MMIPB_SEARCH_OTHER);/*lint !e605*/   
                    cnf_msg.handle = pb_signal->handle;
                    cnf_msg.handle_node = handle_node;
					((MMIPB_SEARCH_DATA_T*)(cnf_msg.handle_node.data_ptr))->find_num = num;
                    MMIPB_SendSignalToMMITask(MMI_PB_SEARCH_CNF, &cnf_msg);
                }
            }
            
            break;
#endif 
#endif

#ifdef BT_PBAP_SUPPORT  
        case MMIPB_SIG_PBAP_CONTACT_READ:
            SCI_TRACE_LOW("[MMIPB]PBTask_Entry MMIPB_SIG_PBAP_CONTACT_READ");
            cnf_msg.ret = MMIPB_ParsePbapData();
            MMIPB_SendSignalToMMITask(MMI_PB_PBAP_CONTACT_READ_CNF, &cnf_msg);
            break;

        case MMIPB_SIG_PBAP_CALLLOG_READ:
            SCI_TRACE_LOW("[MMIPB]PBTask_Entry MMIPB_SIG_PBAP_CALLLOG_READ");
            cnf_msg.ret = MMICL_ParsePbapData();
            MMIPB_SendSignalToMMITask(MMI_PB_PBAP_CALLLOG_READ_CNF, &cnf_msg);
            break;
#endif

        default:
            //SCI_TRACE_LOW:"[MMIPB]PBTask_Entry: SigCode unknown"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_TASK_174_112_2_18_2_44_11_302,(uint8*)"");
            break;
        }

        MMI_FREE_SIGNAL(psig);
    }
}

/*****************************************************************************/
//  Description : create pb task
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_CreateTask(void)
{
    if (0 != g_mmipb_pb_task_id)
    {
        //之前创建的task还没有关闭!
        //SCI_TRACE_LOW:"MMIPB_CreateTask, pb task hasn't been closed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_TASK_191_112_2_18_2_44_11_303,(uint8*)"");
        return;
    }
    
    g_mmipb_pb_task_id = SCI_CreateAppThread( 
                        "MMIPB_PB_TASK", 
                        "Q_MMIPB_PB_TASK", 
                        PBTask_Entry, 
                        0, 
                        0, 
                        MMIPB_TASK_STACK_SIZE, 
                        MMIPB_TASK_QUEUE_NUM, 
                        MMIPB_TASK_PRIORITY, 
                        SCI_PREEMPT, 
                        SCI_AUTO_START);

    //SCI_TRACE_LOW:"MMIPB_CreateTask, g_mmipb_pb_task_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_TASK_207_112_2_18_2_44_11_304,(uint8*)"d", g_mmipb_pb_task_id);
}

/*****************************************************************************/
//  Description : send signal to pb task
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIPB_SendSignalToPBTask(uint16 sig_id, uint32 handle)
{ 
    MMIPB_SIGNAL_T *sig_ptr = PNULL;
    MMIPB_HANDLE_NODE_T handle_node = {0};
    
    MMI_CREATE_SIGNAL(sig_ptr,
        sig_id,
        sizeof(MMIPB_SIGNAL_T),
        SCI_IdentifyThread()
        );
    sig_ptr->handle = handle;  
    if(MMIPB_GetHandleContent(handle, &handle_node))
    {
        sig_ptr->handle_node = handle_node;
    }
    MMI_SEND_SIGNAL(sig_ptr, g_mmipb_pb_task_id);
}


/*****************************************************************************/
//  Description : send signal to mmi task
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIPB_SendSignalToMMITask(uint16 sig_id, MMIPB_CNF_MSG_T *msg_ptr)
{ 
    MMIPB_SIGNAL_T *sig_ptr = PNULL;
    
    MMI_CREATE_SIGNAL(sig_ptr,
        sig_id,
        sizeof(MMIPB_SIGNAL_T),
        SCI_IdentifyThread()
        );
    sig_ptr->handle = msg_ptr->handle;
    sig_ptr->ret = msg_ptr->ret;
    sig_ptr->handle_node = msg_ptr->handle_node;
    MMI_SEND_SIGNAL(sig_ptr, P_APP);
}


