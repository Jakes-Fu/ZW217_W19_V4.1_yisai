/*****************************************************************************
** File Name:      gui_clipboard.c                                           *
** Author:         hua.fang                                                  *
** Date:           07/16/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2011        hua.fang          Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "mmi_app_theme_trc.h"
#include "mmitheme_frame_engine.h"
#include "mmi_list.h"
#include "guilcd.h"
#include "os_api.h"
#include "sci_api.h"
#include "chng_freq.h"
#include "mmk_timer.h"
#include "mmi_modu_main.h"
#include "guicommon.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
#define _ANIMATION_ENGINE_TRACE_

// 打印的宏
#ifdef _ANIMATION_ENGINE_TRACE_
#define START_TRACE_ANIM()          GUI_StartTiming()
#define TRACE_ANIM_TIMING(X)        GUI_PrintTiming(X)
#else
#define START_TRACE_ANIM()
#define TRACE_ANIM_TIMING(X)
#endif

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
// 动画基类对象结构
// 此结构必须放在继承结构的第一个位置
typedef struct
{
    MMI_LIST_T                  list;                       // 动画链表

    MMIANIM_INFO_T              anim_info;                  // 动画信息
    MMIANIM_PARAM_T             anim_param;                 // 动画参数
} ANIMATION_OBJECT_T;

// 帧对象结构
typedef struct
{
    ANIMATION_OBJECT_T          anim_header;                // 动画表头

    BOOLEAN                     is_init;                    // 是否初始化
    uint8                       timer_id;                   // 异步时的定时器ID
    uint16                      frame_count;                // 总帧数
    uint16                      cur_frame_idx;              // 当前帧
} FRAME_OBJECT_T;

typedef void (*DO_ANIMATION_FUNC)(ANIMATION_OBJECT_T *anim_object_ptr, uint16 frame_index);   // 做动画，必须实现

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init frame engine
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void InitFrameEngine(void);

/*****************************************************************************/
//  Description : synchronize animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SynchronizeAnim(
                              uint32        period
                              );

/*****************************************************************************/
//  Description : asynchronize animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AsynchronizeAnim(
                               uint32        period
                               );

/*****************************************************************************/
//  Description : handle async anim timer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void HandleAsyncAnimTimer(
                                uint8       timer_id,
                                uint32      param
                                );

/*****************************************************************************/
//  Description : make one frame
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void MakeOneFrame(void);

/*****************************************************************************/
//  Description : animation finished
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AnimationFinished(void);

/*****************************************************************************/
//  Description : add animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AddAnimation(
                           ANIMATION_OBJECT_T    *animation_ptr
                           );

/*****************************************************************************/
//  Description : create anim object
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL ANIMATION_OBJECT_T* CreateAnimObj(
                                        MMIANIM_PARAM_T       *anim_param_ptr
                                        );

/*****************************************************************************/
//  Description : release anim object
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseAnimObj(
                          ANIMATION_OBJECT_T     *anim_obj_ptr
                          );

/*****************************************************************************/
//  Description : do size animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DoSizeAnimation(
                           ANIMATION_OBJECT_T   *anim_object_ptr,
                           uint16               frame_index
                           );

/*****************************************************************************/
//  Description : do blank animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DoBlankAnimation(
                            ANIMATION_OBJECT_T   *anim_object_ptr,
                            uint16               frame_index
                            );

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/
LOCAL FRAME_OBJECT_T    s_frame_object = {0};

// 必须要与MMIANIM_TYPE_E里值的顺序一致
LOCAL DO_ANIMATION_FUNC s_DoAnimation[MMIANIM_TYPE_MAX] =
{
    DoBlankAnimation,
    DoSizeAnimation,
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : reset frame engine
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMIFRAME_Reset(void)
{
    ANIMATION_OBJECT_T      *animation_obj_ptr = PNULL;

    InitFrameEngine();

    if (!MMILIST_IsEmpty(&s_frame_object.anim_header.list))
    {
        while (&s_frame_object.anim_header.list != s_frame_object.anim_header.list.next)
        {
            animation_obj_ptr = (ANIMATION_OBJECT_T*)s_frame_object.anim_header.list.next;

            if (PNULL != animation_obj_ptr->anim_param.anim_func.AnimResetFunc)
            {
                animation_obj_ptr->anim_param.anim_func.AnimResetFunc(animation_obj_ptr);
            }

            ReleaseAnimObj(animation_obj_ptr);
        }
    }

    s_frame_object.cur_frame_idx = 0;
    s_frame_object.frame_count = 0;
}

/*****************************************************************************/
//  Description : start animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFRAME_Start(
                              MMIANIM_START_PARAM_T     *start_param_ptr
                              )
{
    BOOLEAN     result = TRUE;

    if (PNULL != start_param_ptr && !MMILIST_IsEmpty(&s_frame_object.anim_header.list))
    {
        s_frame_object.frame_count = start_param_ptr->frame_count;

        if (start_param_ptr->is_async)
        {
            result = AsynchronizeAnim(start_param_ptr->period);
        }
        else
        {
            result = SynchronizeAnim(start_param_ptr->period);
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : stop animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMIFRAME_Stop(void)
{
    if (0 != s_frame_object.timer_id)
    {
        MMK_StopTimer(s_frame_object.timer_id);

        s_frame_object.timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : add animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC ANIM_HANDLE_T MMIFRAME_AddAnim(
                                      MMIANIM_PARAM_T       *anim_param_ptr
                                      )
{
    ANIMATION_OBJECT_T     *anim_obj_ptr = PNULL;

    if (PNULL != anim_param_ptr && PNULL != anim_param_ptr->anim_func.DoAnimFunc && anim_param_ptr->anim_type < MMIANIM_TYPE_MAX)
    {
        anim_obj_ptr = CreateAnimObj(anim_param_ptr);

        if (PNULL != anim_obj_ptr && !AddAnimation(anim_obj_ptr))
        {
            ReleaseAnimObj(anim_obj_ptr);
            anim_obj_ptr = PNULL;
        }
    }

    return (ANIM_HANDLE_T)anim_obj_ptr;
}

/*****************************************************************************/
//  Description : remove animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMIFRAME_RemoveAnim(
                                ANIM_HANDLE_T       anim_handle
                                )
{
    ANIMATION_OBJECT_T *anim_obj_ptr = (ANIMATION_OBJECT_T*)anim_handle;

    if (PNULL != anim_obj_ptr)
    {
        ReleaseAnimObj(anim_obj_ptr);
    }
}

/*****************************************************************************/
//  Description : set animation param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFRAME_SetAnimParam(
                                     ANIM_HANDLE_T          anim_handle,
                                     MMIANIM_PARAM_T        *anim_param_ptr
                                     )
{
    BOOLEAN     result = FALSE;
    ANIMATION_OBJECT_T  *anim_obj_ptr = (ANIMATION_OBJECT_T*)anim_handle;

    if (PNULL != anim_obj_ptr && PNULL != anim_param_ptr)
    {
        anim_obj_ptr->anim_param = *anim_param_ptr;

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : get animation param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC MMIANIM_PARAM_T MMIFRAME_GetAnimParam(
                                             ANIM_HANDLE_T          anim_handle
                                             )
{
    MMIANIM_PARAM_T     anim_param = {0};
    ANIMATION_OBJECT_T  *anim_obj_ptr = (ANIMATION_OBJECT_T*)anim_handle;

    if (PNULL != anim_obj_ptr)
    {
        anim_param = anim_obj_ptr->anim_param;
    }

    return anim_param;
}

/*****************************************************************************/
//  Description : create start and end frame index
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMIANIM_SetInfo(
                            ANIM_HANDLE_T       anim_handle,
                            MMIANIM_INFO_T      *anim_info_ptr
                            )
{
    ANIMATION_OBJECT_T  *anim_obj_ptr = (ANIMATION_OBJECT_T*)anim_handle;

    if (PNULL != anim_obj_ptr && PNULL != anim_info_ptr)
    {
        anim_obj_ptr->anim_info = *anim_info_ptr;
    }
}

/*****************************************************************************/
//  Description : init frame engine
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void InitFrameEngine(void)
{
    if (!s_frame_object.is_init)
    {
        s_frame_object.is_init = TRUE;

        MMILIST_Init(&s_frame_object.anim_header.list);
    }
}

/*****************************************************************************/
//  Description : synchronize animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SynchronizeAnim(
                              uint32        period
                              )
{
    uint32                  pre_tick_count = 0;
    uint32                  cur_tick_count = 0;
#ifdef WIN32
    static uint32           sleep_time = 50;
#endif

    // do animation
    for (s_frame_object.cur_frame_idx = 1; s_frame_object.cur_frame_idx <= s_frame_object.frame_count; s_frame_object.cur_frame_idx++)
    {
        pre_tick_count = SCI_GetTickCount();

        MakeOneFrame();

#ifdef WIN32
        SCI_SLEEP(sleep_time);
#endif

        cur_tick_count = SCI_GetTickCount();

        if (cur_tick_count - pre_tick_count < period)
        {
            SCI_SLEEP(period - cur_tick_count + pre_tick_count);
        }
    }

    // animation finished
    AnimationFinished();

    return TRUE;
}

/*****************************************************************************/
//  Description : asynchronize animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AsynchronizeAnim(
                               uint32        period
                               )
{
    s_frame_object.timer_id = MMK_CreateTimerCallback(
            period, HandleAsyncAnimTimer, period, FALSE);

    s_frame_object.cur_frame_idx = 1;

    return MMK_StartTimerCallback(
            s_frame_object.timer_id, period, HandleAsyncAnimTimer, period, FALSE);
}

/*****************************************************************************/
//  Description : handle async anim timer
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void HandleAsyncAnimTimer(
                                uint8       timer_id,
                                uint32      param
                                )
{
    MakeOneFrame();

    s_frame_object.cur_frame_idx++;

    if (s_frame_object.cur_frame_idx == s_frame_object.frame_count)
    {
        // animation finished
        AnimationFinished();

        MMK_StopTimer(s_frame_object.timer_id);

        s_frame_object.timer_id = 0;
    }
    else
    {
        MMK_StartTimerCallback(
                s_frame_object.timer_id, param,
                HandleAsyncAnimTimer, param, FALSE);
    }
}

/*****************************************************************************/
//  Description : make one frame
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void MakeOneFrame(void)
{
    ANIMATION_OBJECT_T      *animation_obj_ptr = PNULL;
#ifdef WIN32
    static uint32           sleep_time = 50;
#endif

    // 变频
#ifndef WIN32
    CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(), FREQ_INDEX_MP3);
#endif

    //SCI_TRACE_LOW:"== MMIFRAME_Start == frame : %d/%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_FRAME_ENGINE_450_112_2_18_3_2_0_14,(uint8*)"dd",s_frame_object.cur_frame_idx, s_frame_object.frame_count);

    START_TRACE_ANIM();

    animation_obj_ptr = (ANIMATION_OBJECT_T*)s_frame_object.anim_header.list.next;

    while (animation_obj_ptr != (ANIMATION_OBJECT_T*)&s_frame_object.anim_header.list)
    {
        if (s_frame_object.cur_frame_idx >= animation_obj_ptr->anim_info.start_frame_index
            && (s_frame_object.cur_frame_idx < animation_obj_ptr->anim_info.end_frame_index || !animation_obj_ptr->anim_info.is_stop_after_end_frame))
        {
            s_DoAnimation[animation_obj_ptr->anim_param.anim_type](animation_obj_ptr, s_frame_object.cur_frame_idx);
        }

        TRACE_ANIM_TIMING("DoAnimation");

        animation_obj_ptr = (ANIMATION_OBJECT_T*)animation_obj_ptr->list.next;
    }

    GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );

    TRACE_ANIM_TIMING("GUILCD Invalidate");

#ifndef WIN32
    CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif
}

/*****************************************************************************/
//  Description : animation finished
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AnimationFinished(void)
{
    ANIMATION_OBJECT_T      *animation_obj_ptr = PNULL;

    START_TRACE_ANIM();

    // animation finished
    animation_obj_ptr = (ANIMATION_OBJECT_T*)s_frame_object.anim_header.list.next;

    while (animation_obj_ptr != (ANIMATION_OBJECT_T*)&s_frame_object.anim_header.list)
    {
        if (PNULL != animation_obj_ptr->anim_param.anim_func.AnimDoneFunc)
        {
            animation_obj_ptr->anim_param.anim_func.AnimDoneFunc(animation_obj_ptr, animation_obj_ptr->anim_param.user_data);
        }

        TRACE_ANIM_TIMING("AnimDoneFunc");

        animation_obj_ptr = (ANIMATION_OBJECT_T*)animation_obj_ptr->list.next;
    }
}

/*****************************************************************************/
//  Description : add animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AddAnimation(
                           ANIMATION_OBJECT_T    *animation_ptr
                           )
{
    BOOLEAN result = FALSE;

    if (s_frame_object.is_init && PNULL != animation_ptr)
    {
        MMILIST_InsertLast(&s_frame_object.anim_header.list, &animation_ptr->list);
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : create anim object
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL ANIMATION_OBJECT_T* CreateAnimObj(
                                        MMIANIM_PARAM_T       *anim_param_ptr
                                        )
{
    ANIMATION_OBJECT_T *anim_obj_ptr = PNULL;

    anim_obj_ptr = SCI_ALLOC_APP(sizeof(ANIMATION_OBJECT_T));
    SCI_MEMSET(anim_obj_ptr, 0, sizeof(ANIMATION_OBJECT_T));

    MMILIST_Init(&anim_obj_ptr->list);

    anim_obj_ptr->anim_info.start_frame_index = 0;
    anim_obj_ptr->anim_info.end_frame_index = (uint16)-1;

    anim_obj_ptr->anim_param = *anim_param_ptr;

    return anim_obj_ptr;
}

/*****************************************************************************/
//  Description : release anim object
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseAnimObj(
                          ANIMATION_OBJECT_T     *anim_obj_ptr
                          )
{
    if (PNULL != anim_obj_ptr)
    {
        MMILIST_Detach(&anim_obj_ptr->list);

        SCI_FREE(anim_obj_ptr);
    }
}

/*****************************************************************************/
//  Description : do blank animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DoBlankAnimation(
                            ANIMATION_OBJECT_T   *anim_object_ptr,
                            uint16               frame_index
                            )
{
    MMIANIM_CALLBACK_PARAM_U    callback_param = {0};
    callback_param.blank_anim.user_data = anim_object_ptr->anim_param.user_data;

    anim_object_ptr->anim_param.anim_func.DoAnimFunc(anim_object_ptr, frame_index, &callback_param);
}

/*****************************************************************************/
//  Description : do size animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DoSizeAnimation(
                           ANIMATION_OBJECT_T   *anim_object_ptr,
                           uint16               frame_index
                           )
{
    uint16                  frame_count = 0;
    uint16                  cur_frame_index = 0;
    GUI_SIZE_T              cur_size = {0};
    GUI_SIZE_T              *start_size_ptr = &anim_object_ptr->anim_param.add_param.size_anim.start_size;
    GUI_SIZE_T              *end_size_ptr = &anim_object_ptr->anim_param.add_param.size_anim.end_size;
    MMK_DAMP_PLAY_TYPE_E    play_type = anim_object_ptr->anim_param.add_param.size_anim.play_type;
    MMIANIM_CALLBACK_PARAM_U    callback_param = {0};

    //SCI_ASSERT(frame_index >= anim_object_ptr->anim_info.start_frame_index);    /*assert verified*/
    if(frame_index < anim_object_ptr->anim_info.start_frame_index)
    {
        return;
    }

    if (frame_index <= anim_object_ptr->anim_info.end_frame_index)
    {
        if (anim_object_ptr->anim_info.end_frame_index > s_frame_object.frame_count)
        {
            frame_count = s_frame_object.frame_count - anim_object_ptr->anim_info.start_frame_index;
        }
        else
        {
            frame_count = anim_object_ptr->anim_info.end_frame_index - anim_object_ptr->anim_info.start_frame_index;
        }
        
        cur_frame_index = frame_index - anim_object_ptr->anim_info.start_frame_index;
        
        cur_size.x = start_size_ptr->x + MMK_ClcltDampPlayOffset(play_type, end_size_ptr->x - start_size_ptr->x, frame_count, cur_frame_index);
        cur_size.y = start_size_ptr->y + MMK_ClcltDampPlayOffset(play_type, end_size_ptr->y - start_size_ptr->y, frame_count, cur_frame_index);
        cur_size.w = start_size_ptr->w + MMK_ClcltDampPlayOffset(play_type, end_size_ptr->w - start_size_ptr->w, frame_count, cur_frame_index);
        cur_size.h = start_size_ptr->h + MMK_ClcltDampPlayOffset(play_type, end_size_ptr->h - start_size_ptr->h, frame_count, cur_frame_index);
    }
    else if (!anim_object_ptr->anim_info.is_stop_after_end_frame)
    {
        cur_size = *end_size_ptr;
    }
    else
    {
        SCI_ASSERT(0);  /*assert verified*/
    }
    
    callback_param.size_anim.user_data = anim_object_ptr->anim_param.user_data;
    callback_param.size_anim.cur_size = cur_size;
    
    anim_object_ptr->anim_param.anim_func.DoAnimFunc(anim_object_ptr, frame_index, &callback_param);
}

