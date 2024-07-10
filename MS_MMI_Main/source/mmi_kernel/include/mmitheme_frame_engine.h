/***************************************************************************
** File Name:      MySample_image.h                                        *
** Author:         hua.fang                                                *
** Date:           07/16/2011                                              *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/2010        hua.fang         Create                                  *
**																		   *
****************************************************************************/
#ifndef _MMITHEME_ANIMATION_ENGINE_H_
#define _MMITHEME_ANIMATION_ENGINE_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "sci_types.h"
#include "cafcontrol.h"
#include "mmk_app.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef _cplusplus
	extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
typedef void* ANIM_HANDLE_T;                                    // 动画句柄

// 动画类型
typedef enum
{
    MMIANIM_TYPE_BLANK  = 0,
    MMIANIM_TYPE_SIZE,
    MMIANIM_TYPE_MAX,
} MMIANIM_TYPE_E;

// 动画信息
typedef struct
{
    BOOLEAN                         is_stop_after_end_frame;    // 当动画结束后，是否需要从动画引擎中摘除
    uint16                          start_frame_index;          // 起始帧
    uint16                          end_frame_index;            // 结束帧
} MMIANIM_INFO_T;

// blank anim param
typedef struct
{
    uint32                          user_data;                  // 用户数据
} BLANK_ANIM_CALLBACK_PARAM_T;

// size anim param
typedef struct
{
    GUI_SIZE_T                      start_size;                 // 起始位置大小
    GUI_SIZE_T                      end_size;                   // 结束位置大小
    MMK_DAMP_PLAY_TYPE_E            play_type;                  // 变化类型
} SIZE_ANIM_PARAM_T;

typedef struct
{
    uint32                          user_data;                  // 用户数据
    GUI_SIZE_T                      cur_size;                   // 当前的尺寸
} SIZE_ANIM_CALLBACK_PARAM_T;

// param in MMIFRAME_AddAnim
typedef union
{
    SIZE_ANIM_PARAM_T               size_anim;                  // 尺寸动画的参数
} MMIANIM_ADD_PARAM_U;

// param in DO_ANIM_FUNC
typedef union
{
    BLANK_ANIM_CALLBACK_PARAM_T     blank_anim;                 // 空白动画的回调参数
    SIZE_ANIM_CALLBACK_PARAM_T      size_anim;                  // 尺寸动画的回调参数
} MMIANIM_CALLBACK_PARAM_U;

// callback
typedef void (*DO_ANIM_FUNC)(ANIM_HANDLE_T anim_handle, uint16 frame_index, MMIANIM_CALLBACK_PARAM_U *callback_param);  // 绘制一帧的动画回调
typedef void (*ANIM_DONE_FUNC)(ANIM_HANDLE_T anim_handle, uint32 user_data);                                            // 动画结束的回调
typedef void (*ANIM_RESET_FUNC)(ANIM_HANDLE_T anim_handle);                                                             // 动画重置的回调

// 动画所需的回调函数
typedef struct
{
    DO_ANIM_FUNC                    DoAnimFunc;                 // 做动画
    ANIM_DONE_FUNC                  AnimDoneFunc;               // 动画结束
    ANIM_RESET_FUNC                 AnimResetFunc;              // 重置
} MMIANIM_FUNC_T;

// 动画参数结构
typedef struct
{
    MMIANIM_TYPE_E                  anim_type;                  // 动画类型
    uint32                          user_data;                  // 用户数据

    MMIANIM_FUNC_T                  anim_func;                  // 根据不同的类型而不同
    MMIANIM_ADD_PARAM_U             add_param;                  // 针对各个动画的参数
} MMIANIM_PARAM_T;

// 开始动画的参数
typedef struct
{
    BOOLEAN                         is_async;                   // 是否异步
    uint16                          frame_count;                // 总帧数
    uint32                          period;                     // 每帧之间的间隔(单位:ms)
} MMIANIM_START_PARAM_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : reset frame engine
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMIFRAME_Reset(void);

/*****************************************************************************/
//  Description : start animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFRAME_Start(
                              MMIANIM_START_PARAM_T     *start_param_ptr
                              );

/*****************************************************************************/
//  Description : stop animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMIFRAME_Stop(void);

/*****************************************************************************/
//  Description : remove animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMIFRAME_RemoveAnim(
                                ANIM_HANDLE_T       anim_handle
                                );

/*****************************************************************************/
//  Description : add animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC ANIM_HANDLE_T MMIFRAME_AddAnim(
                                      MMIANIM_PARAM_T       *anim_param_ptr
                                      );

/*****************************************************************************/
//  Description : set animation param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFRAME_SetAnimParam(
                                     ANIM_HANDLE_T          anim_handle,
                                     MMIANIM_PARAM_T        *anim_param_ptr
                                     );

/*****************************************************************************/
//  Description : get animation param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC MMIANIM_PARAM_T MMIFRAME_GetAnimParam(
                                             ANIM_HANDLE_T          anim_handle
                                             );

/*****************************************************************************/
//  Description : create start and end frame index
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMIANIM_SetInfo(
                            ANIM_HANDLE_T       anim_handle,
                            MMIANIM_INFO_T      *anim_info_ptr
                            );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif  // _MMITHEME_ANIMATION_ENGINE_H_
