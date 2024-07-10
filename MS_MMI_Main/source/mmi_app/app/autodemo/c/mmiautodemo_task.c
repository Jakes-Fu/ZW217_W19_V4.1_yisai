/*****************************************************************************
** File Name:      mmiautodemo_task.c                                              *
** Author:                                                                   *
** Date:           3/16/2012                                                 *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                     DESCRIPTION                 *
** 3/2012        yongsheng.wang                Create                        *
******************************************************************************/
#define _MMIAUTODEMO_TASK_C_  
/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmiautodemo_task.h"
#include "tasks_id.h" 
#include "priority_app.h"
#include "os_api.h"
#include "mmi_osbridge.h"
#include "dal_keypad.h"
#include "mmk_msg.h"
#include "mmiautodemo_api.h"
#include "mmiautodemo_internal.h"
#include "mmiautodemo_export.h"
#include "mmiautodemo_text.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
// auto demo stack and queue define
#define   MMIAD_TASK_STACK_SIZE         (1024 * 8) //auto demo task　stack size
#define   MMIAD_TASK_QUEUE_NUM          8    //auto demo task queue num
#define   MMIAD_TASK_PRIORITY           PRI_AUTODEMO_TASK //auto demo task优先级
#define   MMIAD_SEND_SIGNAL_INTERVAL    10

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : AutoDemo task 入口函数
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL void ADTask_Entry(
                        uint32 argc, 
                        void * argv
                        );

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/   
LOCAL BLOCK_ID             g_mmiad_ad_task_id = 0;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description : Auto demo task 入口函数
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL void ADTask_Entry(
                        uint32 argc, 
                        void * argv
                        )
{
    uint16                     pause_time       = 0;
    GUI_POINT_T                start_point      = {0};
    GUI_POINT_T                end_point        = {0};
    uint32                     keycode          = 0;
    BOOLEAN                    exist_next_flag  = FALSE;//是否还有下一行
    MMIAD_MSG_TYPE_E           cmd_type         = MMIAD_MSG_NONE;
    MMI_AUTODEMO_INFO_T*       cmd_info         = MMIAD_GetAUTODEMO_Info(); 
    uint8                      cur_pos_str[CMD_STR_LINE_MAX_NUM] = {'\0'};
    uint32                     valid_parse_time = 0;//记录执行过的有效指令次数
    
    cmd_info->cmd_buf_ptr = SCI_ALLOC_APPZ(MMI_AUTO_DEMO_CMD_TOREAD_SIZE);
    //SCI_ASSERT(PNULL != cmd_info->cmd_buf_ptr);
    if(PNULL == cmd_info->cmd_buf_ptr)
    {
        SCI_TRACE_LOW("ADTask_Entry in mmiautodemo_task.c, PNULL == cmd_info->cmd_buf_ptr");
        MMIAD_OpenAutoDemoErrorWin(TXT_AUTODEMO_FORMAT_ERROR);
        MMIAPIAD_Stop();
    }
    MMIAPIAUTODEMO_LoadCmd(cmd_info->cmd_buf_ptr , &(cmd_info->cmd_buf_len));
    if(0 != cmd_info->cmd_buf_len)
    {
	    while(1)/*lint !e716*/
	    {	
            exist_next_flag = MMIAD_GetNextCmdLine(cur_pos_str, cmd_info->cmd_buf_ptr, &(cmd_info->cmd_buf_len));

            if(*cur_pos_str != '\0')
            {    
                cmd_type = MMIAD_ParseCmdByLine(cur_pos_str, &pause_time, &start_point, &end_point, &keycode );
 
                SCI_TRACE_LOW("ADTask_Entry handle msg type, cmd_type = %d", cmd_type);

                switch(cmd_type)
                {
                case MMIAD_SHORT_PRESS:
                case MMIAD_LONG_PRESS:                             
                   MMIAD_Send_KPD_SignalToMMITask(keycode, cmd_type, pause_time);
                   valid_parse_time++;
                    break;

                case MMIAD_TP_MOVE:
                case MMIAD_TP_CLICK:
                    MMIAD_Send_TP_SignalToMMITask(cmd_type, start_point, end_point, pause_time);
                    valid_parse_time++;
                    break;
                default:
                    SCI_TRACE_LOW("[warning]:ADTask_Entry handle default msg type");
                    //SCI_Sleep(MMIAD_SEND_SIGNAL_INTERVAL );
                }
            }
        //脚本读取结束跳出循环，准备结束task
            if(!exist_next_flag) 
            {
                break;
            }
        }
	}
    else
    {
        MMIAD_OpenAutoDemoErrorWin(TXT_AUTODEMO_FORMAT_ERROR);
    }
    MMIAPIAUTODEMO_OpenAutodemoWindow();
    if(valid_parse_time > 0)
    {
        MMIAD_OpenAutoDemoFinishedWin();
    }
    else
    {
        MMIAD_OpenAutoDemoErrorWin(TXT_AUTODEMO_ERROR_NONE_SIGNAL);
    }
    MMIAPIAD_Stop();
}

/*****************************************************************************/
//  Description : create Auto demo task
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAD_CreateADTask(void)
{
    if (0 != g_mmiad_ad_task_id)
    {
        //之前创建的task还没有关闭!
        SCI_TRACE_LOW("MMIAD_CreateTask, autodemo task hasn't been closed!");
        return;
    }
    
    g_mmiad_ad_task_id = SCI_CreateAppThread( 
                                            "MMIAD_AD_TASK", 
                                            "Q_MMIAD_AD_TASK", 
                                            ADTask_Entry, 
                                            0, 
                                            0, 
                                            MMIAD_TASK_STACK_SIZE, 
                                            MMIAD_TASK_QUEUE_NUM, 
                                            MMIAD_TASK_PRIORITY, 
                                            SCI_PREEMPT, 
                                            SCI_AUTO_START);

    SCI_TRACE_LOW("MMIAD_CreateTask, g_mmiad_ad_task_id = %d", g_mmiad_ad_task_id);
}

/*****************************************************************************/
//  Description : delete Autodemo task
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAD_DeleteADTask(void)
{
	BLOCK_ID tmp_task_id = g_mmiad_ad_task_id;

    if(0 != g_mmiad_ad_task_id)
	{
        g_mmiad_ad_task_id = 0;
		SCI_DeleteThread(tmp_task_id);	
	}
	else
    {
        SCI_TRACE_LOW("MMIAD_DeleteADTask, autodemo task hasn't been created!");
        return;
    }
	
}

/*****************************************************************************/
//  Description : send kpd signal to mmi task
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIAD_Send_KPD_SignalToMMITask( uint32 keycode, MMIAD_MSG_TYPE_E msg_type, uint16 pause_time)
{
    MmiKeyPress* signal_ptr=PNULL;
    uint16      signal_code = 0;

    /*稍后应重构代码*/
	switch(msg_type)
	{
    case MMIAD_SHORT_PRESS:
		//按下signal
		signal_code = KPD_DOWN;
		MmiCreateSignal(signal_code, sizeof(MmiKeyPress), (MmiSignalS**)&signal_ptr);
		signal_ptr->keyCode = keycode;
		signal_ptr->Sender = KPDSVR;
		MmiSendSignal(TASK_FL_ID, (MmiSignalS*)signal_ptr);

		//抬起signal
		signal_code = KPD_UP;
		MmiCreateSignal(signal_code, sizeof(MmiKeyPress), (MmiSignalS**)&signal_ptr);
		signal_ptr->keyCode = keycode;
		signal_ptr->Sender = KPDSVR;
		MmiSendSignal(TASK_FL_ID, (MmiSignalS*)signal_ptr);
        SCI_Sleep(pause_time);
        break;

    case MMIAD_LONG_PRESS:
		//按下signal
		signal_code = KPD_DOWN;
		MmiCreateSignal(signal_code, sizeof(MmiKeyPress), (MmiSignalS**)&signal_ptr);
		signal_ptr->keyCode = keycode;
		signal_ptr->Sender = KPDSVR;
		MmiSendSignal(TASK_FL_ID, (MmiSignalS*)signal_ptr);
		
		//长按pause_time时间
		SCI_Sleep(pause_time);
	
		//抬起signal
		signal_code = KPD_UP;
		MmiCreateSignal(signal_code, sizeof(MmiKeyPress), (MmiSignalS**)&signal_ptr);
		signal_ptr->keyCode = keycode;
		signal_ptr->Sender = KPDSVR;
		MmiSendSignal(TASK_FL_ID, (MmiSignalS*)signal_ptr);
        SCI_Sleep(pause_time * 4);
        break;
    default:
        SCI_TRACE_LOW("MMIAD_Send_KPD_SignalToMMITask, no this operation!");
		break;
	}
}

/*****************************************************************************/
//  Description : send tp signal to mmi task
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIAD_Send_TP_SignalToMMITask( MMIAD_MSG_TYPE_E msg_type,  
										  GUI_POINT_T start_point , 
										  GUI_POINT_T end_point,
										  uint16 pause_time)
{
    MmiTPPress *signal_ptr = PNULL;
    uint16      signal_code = 0;

	switch(msg_type)
	{    
	case MMIAD_TP_CLICK:
		signal_code = MMI_TP_DOWN;
		MmiCreateSignal(signal_code, sizeof(MmiTPPress), (MmiSignalS**)&signal_ptr);

		signal_ptr->x  = start_point.x;
		signal_ptr->y  = start_point.y;
		signal_ptr->gesture_type = 0;
		signal_ptr->Sender = P_TPP;

		MmiSendSignal(TASK_FL_ID, (MmiSignalS*)signal_ptr);
	
// 		//SCI_Sleep(MMIAD_SEND_SIGNAL_INTERVAL);
        signal_code = MMI_TP_MOVE;
		MmiCreateSignal(signal_code, sizeof(MmiTPPress), (MmiSignalS**)&signal_ptr);
		signal_ptr->x  = start_point.x;
		signal_ptr->y  = start_point.y;
		signal_ptr->gesture_type = 0;
		signal_ptr->Sender = P_TPP;
		MmiSendSignal(TASK_FL_ID, (MmiSignalS*)signal_ptr);	

	
		signal_code = MMI_TP_UP;
		MmiCreateSignal(signal_code, sizeof(MmiTPPress), (MmiSignalS**)&signal_ptr);

		signal_ptr->x  = start_point.x;
		signal_ptr->y  = start_point.y;
		signal_ptr->gesture_type = 0;
		signal_ptr->Sender = P_TPP;

		MmiSendSignal(TASK_FL_ID, (MmiSignalS*)signal_ptr);
    SCI_Sleep(pause_time);
	    break;

	case MMIAD_TP_MOVE:
		signal_code = MMI_TP_DOWN;
		MmiCreateSignal(signal_code, sizeof(MmiTPPress), (MmiSignalS**)&signal_ptr);
                signal_ptr->x  = start_point.x;
		signal_ptr->y  = start_point.y;
		signal_ptr->gesture_type = 0;
		signal_ptr->Sender = P_TPP;     
		MmiSendSignal(TASK_FL_ID, (MmiSignalS*)signal_ptr);
                signal_code = MMI_TP_MOVE;
		MmiCreateSignal(signal_code, sizeof(MmiTPPress), (MmiSignalS**)&signal_ptr);
		signal_ptr->x  = start_point.x;
		signal_ptr->y  = start_point.y;
		signal_ptr->gesture_type = 0;
		signal_ptr->Sender = P_TPP;
		MmiSendSignal(TASK_FL_ID, (MmiSignalS*)signal_ptr);	
		
                SCI_Sleep(MMIAD_SEND_SIGNAL_INTERVAL * 4);
                signal_code = MMI_TP_MOVE;
		MmiCreateSignal(signal_code, sizeof(MmiTPPress), (MmiSignalS**)&signal_ptr);
		signal_ptr->x  = start_point.x + (end_point.x - start_point.x) / 2;
		signal_ptr->y  = start_point.y;
		signal_ptr->gesture_type = 0;
		signal_ptr->Sender = P_TPP;
		MmiSendSignal(TASK_FL_ID, (MmiSignalS*)signal_ptr);	

		SCI_Sleep(MMIAD_SEND_SIGNAL_INTERVAL * 4);
		signal_code = MMI_TP_MOVE;
		MmiCreateSignal(signal_code, sizeof(MmiTPPress), (MmiSignalS**)&signal_ptr);
		signal_ptr->x  = end_point.x + 20;
		signal_ptr->y  = end_point.y;
		signal_ptr->gesture_type = 0;
		signal_ptr->Sender = P_TPP;
		MmiSendSignal(TASK_FL_ID, (MmiSignalS*)signal_ptr);	


		SCI_Sleep(MMIAD_SEND_SIGNAL_INTERVAL);
		signal_code = MMI_TP_UP;
		MmiCreateSignal(signal_code, sizeof(MmiTPPress), (MmiSignalS**)&signal_ptr);
		signal_ptr->x  = end_point.x;
		signal_ptr->y  = end_point.y;
		signal_ptr->gesture_type = 0;
		signal_ptr->Sender = P_TPP;
		MmiSendSignal(TASK_FL_ID, (MmiSignalS*)signal_ptr);	
                SCI_Sleep(pause_time);
		break;
	default:
		SCI_TRACE_LOW("MMIAD_Send_TP_SignalToMMITask, no this operation!");
		break;
	}
}

/*****************************************************************************/
//  Description : free cmd buf
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIAD_FreeCmdBuf()
{
    MMI_AUTODEMO_INFO_T *cmd_info = MMIAD_GetAUTODEMO_Info(); 

    if(PNULL != cmd_info->cmd_buf_ptr)
    {
        SCI_Free(cmd_info->cmd_buf_ptr);
        cmd_info->cmd_buf_ptr = PNULL;
    }
}
