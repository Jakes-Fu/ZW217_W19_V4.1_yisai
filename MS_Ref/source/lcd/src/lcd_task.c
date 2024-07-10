/******************************************************************************
 ** File Name:       lcd_task.c                                               *
 ** Author:                                                                   *
 ** DATE:            04/25/2011                                               *
 ** Copyright:       2011 Spreatrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE              NAME             DESCRIPTION                            *
 ******************************************************************************/
#include "ms_ref_lcd_trc.h"
#include "lcd_task.h"

#include "dal_lcd.h"
#include "os_api.h"
#include "chip.h"
#include "lcm_prod.h"
#include "lcdc_app.h"
#include "lcd_cfg.h"
#include "sc_reg.h"
#include "display.h"
#include "lcm_prod.h"
#include "priority_system.h"
#if defined(_ARM_SIMULATOR_)
//no task-related functions and so on...
PUBLIC BOOLEAN LCD_isTaskActive(void)
{
    return SCI_FALSE;
}

PUBLIC uint32 LCD_Task_Open(uint32 priority)
{
    return SCI_SUCCESS;
}

PUBLIC uint32 LCD_Task_Close(void)
{
    return SCI_SUCCESS;
}

#else
/////////////////////////////////////////////////////////////////////////////
#define LCD_STACK_SIZE               4000
#define LCD_QUEUE_NUM                10
#define LCD_TASK_PRIORITY            PRI_LCD_TASK  //  27


/////////////////////////////////////////////////////////////////////////////
/*event-related resources*/
LOCAL SCI_EVENT_GROUP_PTR   s_lcd_event_ptr  = SCI_NULL;
LOCAL SCI_SEMAPHORE_PTR     s_lcd_api_sm_ptr = SCI_NULL;
/*task-related resources*/
LOCAL uint32                s_lcd_task_id     = SCI_INVALID_BLOCK_ID;
DISPLAY_TASK_CONTEXT_T      s_lcd_task_context = {0};
#ifdef MOCOR_SUPPORT_ARGB888_MODE //&& !defined(WIN32)
LOCAL BOOLEAN s_used_argb888 = FALSE;
#endif
/////////////////////////////////////////////////////////////////////////////
LOCAL void LCD_Task_Entry(uint32 argc, void *argv);
LOCAL uint32 LCD_Task_Open(uint32 priority);

LOCAL uint32 LCD_EvalLCDTaskPrority()
{
    char thread_name[SCI_MAX_NAME_SIZE];
    char queue_name[SCI_MAX_NAME_SIZE];
    uint32 state = 0;
    uint32 preempt = 0;
    uint32 priority = LCD_TASK_PRIORITY;

    BLOCK_ID thread_id = SCI_IdentifyThread();

    if(SCI_INVALID_BLOCK_ID != thread_id )
    {
        if(SCI_SUCCESS != SCI_GetThreadInfo(thread_id, thread_name, queue_name, &state,  &priority, &preempt))
        {
            SCI_PASSERT(0,("get thread info error!"));    /*assert verified*/
        }
        priority -- ;
    }
    return priority;
}

/////////////////////////////////////////////////////////////////////////////

/*****************************************************************************/
//  Description: get current task context
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC DISPLAY_TASK_CONTEXT_T *LCD_GetTaskContext()
{
   return &s_lcd_task_context;
}
/*****************************************************************************/
//  Description: wait the DONE event and then clear it
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void LCD_WaitAndClearDoneStatus()
{
    uint32 i_event_flag=0;
    SCI_GetEvent(s_lcd_event_ptr,LCD_EVENT_DONE,SCI_AND_CLEAR, &i_event_flag,
                                                        SCI_WAIT_FOREVER);
    //DISPLAY_TRACE:"lcd_task : LCD_WaitAndClearDoneStatus() "
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_TASK_99_112_2_18_1_45_26_118,(uint8*)"");
}

/*****************************************************************************/
//  Description: wait blend done event
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void LCD_WaitBlendDoneStatus()
{
    uint32 i_event_flag=0;
    SCI_GetEvent(s_lcd_event_ptr,LCD_EVENT_BLEND_OK,SCI_AND, &i_event_flag,
                                                        SCI_WAIT_FOREVER);
    //DISPLAY_TRACE:"lcd_task : LCD_WaitBlendDoneStatus() "
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_TASK_111_112_2_18_1_45_26_119,(uint8*)"");
}

/*****************************************************************************/
//  Description: get lcd API control semaphor
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void LCD_GetAPISemaphore()
{
    if(!SCI_InThreadContext())
    {
        return;
    }

    if(s_lcd_task_id == SCI_INVALID_BLOCK_ID)
    {
        LCD_Task_Open(LCD_EvalLCDTaskPrority());
    }

    SCI_ASSERT( PNULL != s_lcd_api_sm_ptr );    /*assert verified*/
    if( SCI_ERROR == SCI_GetSemaphore(s_lcd_api_sm_ptr, SCI_WAIT_FOREVER) )
    {
        //DISPLAY_TRACE:"lcd_task:Get s_lcd_api_sm_ptr Semaphore Fail!--"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_TASK_133_112_2_18_1_45_26_120,(uint8*)"");
    }
    //DISPLAY_TRACE:"lcd_task : LCD_GetAPISemaphore() "
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_TASK_135_112_2_18_1_45_26_121,(uint8*)"");
}
/*****************************************************************************/
//  Description: put lcd task API semaphor
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void LCD_PutAPISemaphore()
{
    uint32 ret = 0;
    if(!SCI_InThreadContext())
    {
        return;
    }

    SCI_ASSERT( PNULL != s_lcd_api_sm_ptr );    /*assert verified*/

    ret = SCI_PutSemaphore( s_lcd_api_sm_ptr);
    if(ret != SCI_SUCCESS)
    {
        //DISPLAY_TRACE:"lcd_task:Put s_lcd_api_sm_ptr Semaphore Fail!--"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_TASK_154_112_2_18_1_45_27_122,(uint8*)"");
    }
    //DISPLAY_TRACE:"lcd_task : LCD_PutAPISemaphore() "
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_TASK_156_112_2_18_1_45_27_123,(uint8*)"");
}

/*****************************************************************************/
//  Description: get lcd task control semaphor
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void LCD_GetTaskSemaphore(LCD_ID_E lcd_id)
{
    DISPLAY_TASK_CONTEXT_T *context = LCD_GetTaskContext();
    if(!SCI_InThreadContext())
    {
        return;
    }

    SCI_PASSERT(( PNULL != context->lcd_task_sm_ptr ),("lcd_task_sm_ptr is PNULL, %d!,lcd_id"));/*assert verified*/
    if( SCI_ERROR == SCI_GetSemaphore(context->lcd_task_sm_ptr, SCI_WAIT_FOREVER) )
    {
        //DISPLAY_TRACE:"lcd_task:Get lcd_task_sm_ptr Semaphore Fail!--"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_TASK_174_112_2_18_1_45_27_124,(uint8*)"");
    }
    //DISPLAY_TRACE:"lcd_task: LCD_GetTaskSemaphore() "
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_TASK_176_112_2_18_1_45_27_125,(uint8*)"");
}
/*****************************************************************************/
//  Description: put lcd task control semaphor
//    Global resource dependence:
//  Author: Jianping.wang
//    Note:
/*****************************************************************************/
PUBLIC void LCD_PutTaskSemaphore(LCD_ID_E lcd_id)
{
    uint32 ret = 0;
    DISPLAY_TASK_CONTEXT_T *context = LCD_GetTaskContext();
    if(!SCI_InThreadContext())
    {
        return;
    }

    SCI_PASSERT(( PNULL != context->lcd_task_sm_ptr ),("lcd_task_sm_ptr  is PNULL, %d!,lcd_id"));/*assert verified*/

    ret = SCI_PutSemaphore( context->lcd_task_sm_ptr );
    if(ret != SCI_SUCCESS)
    {
        //DISPLAY_TRACE:"lcd_task:Put lcd_task_sm_ptr  Semaphore Fail!--"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_TASK_196_112_2_18_1_45_27_126,(uint8*)"");
    }
    //DISPLAY_TRACE:"lcd_task: LCD_PutTaskSemaphore() "
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_TASK_198_112_2_18_1_45_27_127,(uint8*)"");
}

/******************************************************************************
// Purpose:    create lcd task
// Author:    Allen.Mao
// Input:       None
// Output:      None
// Return:
// Note:
******************************************************************************/
LOCAL uint32 LCD_Task_Open(uint32 priority)
{

    if (PNULL == s_lcd_api_sm_ptr)
    {
        s_lcd_api_sm_ptr = SCI_CreateSemaphore("LCD API SEMAPHORE", 1);
    }

    if (PNULL == s_lcd_task_context.lcd_task_sm_ptr)
    {
        s_lcd_task_context.lcd_task_sm_ptr = SCI_CreateSemaphore("LCD TASK SEMAPHORE", 0);
    }

    if(SCI_NULL == s_lcd_event_ptr)
    {
        s_lcd_event_ptr = SCI_CreateEvent("LCD_SYNC");
        SCI_SetEvent(s_lcd_event_ptr,LCD_EVENT_BLEND_OK,SCI_OR);    //  set event £¬init
    }

    if(SCI_INVALID_BLOCK_ID == s_lcd_task_id)
    {
        s_lcd_task_id = SCI_CreateAppThread(
                                            "T_LCD",
                                            "Q_LCD",
                                            LCD_Task_Entry,
                                            0,
                                            0,
                                            LCD_STACK_SIZE,
                                            LCD_QUEUE_NUM,
                                            priority,
                                            SCI_PREEMPT,
                                            SCI_DONT_START
                                            );

        if (PNULL != s_lcd_task_id)
        {
            SCI_ResumeThread(s_lcd_task_id);
        }

        //DISPLAY_TRACE:"lcd_task, lcd_Task_Open, task_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_TASK_247_112_2_18_1_45_27_128,(uint8*)"d",s_lcd_task_id);
    }
    else
    {
        //DISPLAY_TRACE:"lcd_task, LCD TASK is existed,task_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_TASK_251_112_2_18_1_45_27_129,(uint8*)"d", s_lcd_task_id);
    }
    SCI_PASSERT(SCI_INVALID_BLOCK_ID != s_lcd_task_id, ("[LCD_Task_Open] open lcd task failed"));    /*assert verified*/

    return SCI_SUCCESS;
}

/******************************************************************************
// Purpose:    lcd task entry
// Author:
// Input:       None
// Output:      None
// Return:
// Note:
******************************************************************************/
LOCAL void LCD_Task_Entry(uint32 argc, void *argv)
{
    uint32 i = 0;
    DISPLAY_TASK_CONTEXT_T *context = LCD_GetTaskContext();

    while(SCI_INVALID_BLOCK_ID == s_lcd_task_id)
    {
        SCI_Sleep(20);
    }

    while(1)/*lint !e716*/
    {
        SCI_GetSemaphore(context->lcd_task_sm_ptr,SCI_WAIT_FOREVER);

        if (LCD_UPDATE_MODE_ASYNC == context->mode)
        {
            SCI_SetEvent(s_lcd_event_ptr,~LCD_EVENT_BLEND_OK,SCI_AND);    // clear event

            if((context->blend.layer_num > 0 && context->blend.layer_num <= DISPLAY_UPDATE_LAYER_NUM) && (context->invalidate.layer_num > 0)
                        &&(!context->blend.dst_layer.layer_info.is_enable)&& (DISPLAY_ANGLE_0 == context->invalidate.param.rot_angle))
            {
#ifndef WIN32
                DISPLAY_LAYER_INFO_T *layer[DISPLAY_UPDATE_LAYER_NUM];

                //SCI_TRACE_LOW("++++++blend to LCD,s_clear_layer_id=%d+++++++++++",s_clear_layer_id);
                for (i = 0; i < context->blend.layer_num; i++)
                {
                    layer[i] = &context->blend.layer_arr[i].layer_info;
                    layer[i]->layer_id = i;
                }

                //    SCI_TRACE_LOW("++++++blend to LCD 2,s_clear_layer_id=%d+++++++++++",s_clear_layer_id);
                DISPLAY_InvalidateRect(context->lcd_id, layer, context->blend.layer_num, &context->invalidate.param);
#else
                extern void LCD_SetDirectDraw( BOOLEAN is_direct );
                extern void* LCD_GetMSSIMBuffer( void );

                LCD_SetDirectDraw( TRUE );

                if (context->blend.layer_num > 0)
                {
                    DISPLAY_BLEND_LAYER_T *layer[LAYER_SUPPORT_NUM] = {0};
                    LCD_INFO_T lcd_info;
                    uint32 offset;
                    uint32 buf_addr;

                    for (i = 0; i < context->blend.layer_num ; i++)
                    {
                        layer[i] = &context->blend.layer_arr[i];
                    }

                    LCD_GetInfo( context->lcd_id , &lcd_info );

                    //change to simulator buffer
                    offset   = (uint32)context->blend.dst_layer.layer_info.buf_addr - (uint32)lcd_info.lcdbuff_ptr;
                    buf_addr = (uint32)LCD_GetMSSIMBuffer() + offset;
                    context->blend.dst_layer.layer_info.buf_addr = buf_addr;

                    DISPLAY_BlendLayer(context->lcd_id,
                        &context->blend.dst_layer,
                        layer,
                        context->blend.layer_num,
                        &context->blend.rect);
                }

                if (context->invalidate.layer_num > 0)
                {
                    DISPLAY_LAYER_INFO_T *layer[DISPLAY_UPDATE_LAYER_NUM];

                    for (i = 0; i < context->invalidate.layer_num; i++)
                    {
                        layer[i] = &context->invalidate.layer_arr[i];
                    }

                    DISPLAY_InvalidateRect(context->lcd_id,
                        layer,
                        context->invalidate.layer_num,
                        &context->invalidate.param);
                }

                LCD_SetDirectDraw( FALSE );
#endif
            }
            else
            {
                if(context->blend.layer_num > 0)
                {
                    DISPLAY_BLEND_LAYER_T *layer[LAYER_SUPPORT_NUM]={0};

                    for (i = 0; i < context->blend.layer_num ; i++)
                    {
                        layer[i] = &context->blend.layer_arr[i];
                    }

                    DISPLAY_BlendLayer(context->lcd_id, &context->blend.dst_layer, layer,
                        context->blend.layer_num, &context->blend.rect);
                }
                if (context->invalidate.layer_num > 0)
                {
                    DISPLAY_LAYER_INFO_T *layer[DISPLAY_UPDATE_LAYER_NUM];
                    for (i = 0; i < context->invalidate.layer_num; i++)
                    {
                        layer[i] = &context->invalidate.layer_arr[i];
                    }
                    DISPLAY_InvalidateRect(context->lcd_id, layer, context->invalidate.layer_num, &context->invalidate.param);
                }
            }

            //callback
            if ( PNULL != context->blend.callback )
            {
                context->blend.callback( context->blend.param );
                context->blend.callback = PNULL;
            }

            SCI_SetEvent(s_lcd_event_ptr,LCD_EVENT_BLEND_OK,SCI_OR);    //  set event
        }
        else
        {
            if(PNULL != context->p_blend)
            {
                DISPLAY_BLENDLAYER_PARM_T *blend = context->p_blend;
                DISPLAY_BlendLayer(context->lcd_id, blend->dst_layer_ptr, blend->layer_arr,
                                   blend->layer_num, blend->rect_ptr);
                context->p_blend = PNULL;
            }
            if (PNULL != context->p_invalidate)
            {
                DISPLAY_INVALIDATE_PARM_T *invalidate = context->p_invalidate ;
                DISPLAY_InvalidateRect(context->lcd_id,invalidate->layer_arr,
                                       invalidate->layer_num, invalidate->param_ptr);
                context->p_invalidate = PNULL;
            }

            //callback
            if ( PNULL != context->blend.callback )
            {
                context->blend.callback( context->blend.param );
                context->blend.callback = PNULL;
            }
        }

        SCI_MEMSET(&context->blend, 0, sizeof(context->blend)); /*lint !e516 -e516*/
        SCI_MEMSET(&context->invalidate, 0, sizeof(context->invalidate));

        SCI_SetEvent(s_lcd_event_ptr,LCD_EVENT_DONE,SCI_OR);    //  set event
    }
}

/******************************************************************************
// Purpose:    is lcd task exits
// Author:
// Input:
// Output:      None
// Return:
// Note:
******************************************************************************/
PUBLIC BOOLEAN LCD_IsTaskExist(void)
{
    if (SCI_INVALID_BLOCK_ID == s_lcd_task_id)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/******************************************************************************
// Purpose:    change the priority of lcd task
// Author:
// Input:
            priority    --priority of the lcd task
// Output:      None
// Return:      none
// Note:
******************************************************************************/
PUBLIC uint32 LCD_SetTaskPriority(uint32 new_priority, uint32 *old_priority_ptr)
{
    uint32     old_priority = 0;
    uint32    status          = 0;
    uint32    lcd_ret         = LCD_SUCCESS;

    if (SCI_INVALID_BLOCK_ID != s_lcd_task_id)
    {
        status = SCI_ChangeThreadPriority(s_lcd_task_id, new_priority, &old_priority);
        if (SCI_SUCCESS != status)
        {
            lcd_ret = LCD_FAILED;
            return lcd_ret;
        }

        if (PNULL != old_priority_ptr)
        {
            *old_priority_ptr = old_priority;
        }
    }

    return lcd_ret;
}
PUBLIC void LCD_SetARGB888Mode(BOOLEAN bArgb888Mode)
{
#ifdef MOCOR_SUPPORT_ARGB888_MODE //&& !defined(WIN32)
    DISPLAY_TASK_CONTEXT_T *context = LCD_GetTaskContext();
    SCI_TRACE_LOW("LCD_SetARGB888Mode() Enter. isopen = %d.", bArgb888Mode);
    LCD_GetAPISemaphore();
    s_used_argb888 = bArgb888Mode;
    LCD_PutAPISemaphore();
    SCI_TRACE_LOW("LCD_SetARGB888Mode() exit.");
#endif
}


PUBLIC BOOLEAN LCD_GetARGB888Mode(void)
{
#ifdef MOCOR_SUPPORT_ARGB888_MODE //&& !defined(WIN32)
    return s_used_argb888;
#else
    return TRUE;
#endif
}
#endif
