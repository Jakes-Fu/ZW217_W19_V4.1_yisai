/*****************************************************************************
** File Name:      ctrlbasepicker.h                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Create
******************************************************************************/

#ifndef _CTRLBASEPICKER_H_
#define _CTRLBASEPICKER_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "mmitheme_picker.h"
#include "ctrlbasepicker_export.h"
#include "mmi_string.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define CTRLBASEPICKER_MAX_STR_LEN          10              // 文本最大显示长度

// basepicker type info
#define CTRL_BASEPICKER_TYPE     (BASEPICKER_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
// field
typedef int32 CTRLBASEPICKER_FIELD_T;

// basepicker init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;

    // self inif param ...
} CTRLBASEPICKER_INIT_PARAM_T;

// 域的属性
typedef struct
{
    BOOLEAN                     is_loop;                // 数值是否要循环
    MMK_TP_SLIDE_E              slide_state;            // 当前的状态
    int32                       cur_offset;             // 当前的偏移值，同时用于计算当前的时间日期值
    void                        *tracker_ptr;           // 加速器
    float                       fling_velocity;         // fling状态时的加速度值
    int32                       recover_move_step;      // recover状态时的每次缩短量, >0表示往上移动，<0表示往下移动，=0表示位置正好
    int32                       recover_dst_offset;     // recover状态时的最终目标偏移值.(该值有可能会超过offset的最值，会在SetOffset函数里修正)
    int32                       recover_cur_offset;     // recover状态时的当前偏移值
} CTRLBASEPICKER_ITEM_T;

//basepicker control
typedef struct _basepicker_obj_tag
{
    CTRLBASE_OBJ_T              base_ctrl;

    CTRLBASEPICKER_FIELD_T      tp_down_field;          // 触笔按下的域
    GUI_POINT_T                 tp_start_point;         // TP DOWN时的点
    GUI_POINT_T                 tp_pre_point;           // 上一个TP点
    uint8                       tpscroll_timer_id;      // 拖动时的定时器ID
    uint8                       fling_timer_id;         // 滑动时的定时器ID
    uint8                       recover_timer_id;       // 恢复原位时的定时器ID
    CTRLBASEPICKER_TIPS_POS_E   tips_pos;               // 提示符的位置，默认在顶部
    BOOLEAN                     enable_anim;            // 调用接口设置值时，是否需要动画

    CTRLBASEPICKER_ITEM_T       state[MMITHEME_BASEPICKER_MAX_FIELD_COUNT]; // 各个域的状态

    MMITHEME_BASEPICKER_T       theme;

    BOOLEAN                     is_tp_moved;
    // self member ...
} CTRLBASEPICKER_OBJ_T;

// basepicker class for public or protected method
typedef struct _basepicker_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

    /**************************************************************************/
    //  Description : 获取开始域与结束域(不包含该值)
    /**************************************************************************/
    void (*GetFieldRange)(
                       CTRLBASEPICKER_OBJ_T *basepicker_ctrl_ptr,   // [in]
                       int32                *start_field_ptr,       // [out]
                       int32                *end_field_ptr          // [out]
                       );

    /**************************************************************************/
    //  Description : 获取指定域的最值
    /**************************************************************************/
    void (*GetValueRange)(
                       CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,// [in]
                       CTRLBASEPICKER_FIELD_T      field,               // [in]
                       uint16                      *min_value_ptr,      // [out]
                       uint16                      *max_value_ptr       // [out]
                       );

    /*****************************************************************************/
    //  Description : 获取指定域的相对显示位置
    /*****************************************************************************/
    GUI_RECT_T (*GetFieldRect)(
                            CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,   // [in]
                            int32                   field                   // [in]
                            );

    /**************************************************************************/
    //  Description : 获取域中的文字
    /**************************************************************************/
    void (*GetFieldText)(
                      CTRLBASEPICKER_OBJ_T          *basepicker_ctrl_ptr,   // [in]
                      CTRLBASEPICKER_FIELD_T        field,                  // [in]
                      uint16                        cur_index,              // [in]
                      MMI_STRING_T                  *dis_string_ptr         // [out]
                      );

    /*****************************************************************************/
    //  Description : 获取提示文本
    /*****************************************************************************/
    MMI_TEXT_ID_T (*GetFieldTips)(
                               CTRLBASEPICKER_OBJ_T *basepicker_ctrl_ptr,   // [in]
                               int32                field                   // [in]
                               );

    /*****************************************************************************/
    //  Description : 检查值是否合法，如果不合法，将其置为合法值
    /*****************************************************************************/
    void (*CheckValue)(
                     CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
                     BOOLEAN                is_need_anim            // [in] 是否需要绘制动画
                     );

} CTRLBASEPICKER_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get basepicker type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T BASEPICKER_GetType(void);

/*****************************************************************************/
//  Description : init
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEPICKER_Init (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr    // [in]
);

/*****************************************************************************/
//  Description : destroy
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEPICKER_Destroy (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr    // [in]
);

/*****************************************************************************/
//  Description : 设置域里的值，如果超过最值，自动变最值
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEPICKER_SetValue (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T      field,                  // [in]
    uint16                      value                   // [in]
);

/*****************************************************************************/
//  Description : 获取域里的值
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEPICKER_GetValue (
    CTRLBASEPICKER_OBJ_T         *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T       field                   // [in]
);

/*****************************************************************************/
//  Description : 设置目标值，并进入recover状态
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEPICKER_SetRecoverValue (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T      field,                  // [in]
    uint16                      value,                  // [in],
    BOOLEAN                     is_up                   // [in] 方向是否向上
);

/*****************************************************************************/
//  Description : reset picker
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEPICKER_Reset (
    CTRLBASEPICKER_OBJ_T *basepicker_ctrl_ptr
);

/*****************************************************************************/
//  Description : 根据区域显示picker内容
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEPICKER_Display (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr    // [in]
);

/*****************************************************************************/
//  Description : 设置提示文字的位置
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEPICKER_SetTipsPos (
    CTRLBASEPICKER_OBJ_T          *basepicker_ctrl_ptr,
    CTRLBASEPICKER_TIPS_POS_E     tips_pos
);

/*****************************************************************************/
//  Description : 获取整个文本的区域，(扣除提示文字)
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T BASEPICKER_GetTextRect (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr
);

/*****************************************************************************/
//  Description : 获取开始域与结束域(不包含该值)
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
static __inline void VTLBASEPICKER_GetFieldRange(
                       CTRLBASEPICKER_OBJ_T *basepicker_ctrl_ptr,   // [in]
                       int32                *start_field_ptr,       // [out]
                       int32                *end_field_ptr          // [out]
                       )
{
    CTRLBASEPICKER_VTBL_T  *basepicker_vtbl_ptr = (CTRLBASEPICKER_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEPICKER_TYPE, basepicker_ctrl_ptr);

    if (PNULL != basepicker_vtbl_ptr->GetFieldRange)
    {
        basepicker_vtbl_ptr->GetFieldRange(basepicker_ctrl_ptr, start_field_ptr, end_field_ptr);
    }
}

/*****************************************************************************/
//  Description : 获取指定域的最值
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
static __inline void VTLBASEPICKER_GetValueRange(
                       CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,// [in]
                       CTRLBASEPICKER_FIELD_T       field,               // [in]
                       uint16                      *min_value_ptr,      // [out]
                       uint16                      *max_value_ptr       // [out]
                       )
{
    CTRLBASEPICKER_VTBL_T  *basepicker_vtbl_ptr = (CTRLBASEPICKER_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEPICKER_TYPE, basepicker_ctrl_ptr);

    if (PNULL != basepicker_vtbl_ptr->GetValueRange)
    {
        basepicker_vtbl_ptr->GetValueRange(basepicker_ctrl_ptr, field, min_value_ptr, max_value_ptr);
    }
}

/*****************************************************************************/
//  Description : 获取指定域的相对显示位置
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
static __inline GUI_RECT_T VTLBASEPICKER_GetFieldRect(
                            CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,   // [in]
                            int32                   field                   // [in]
                            )
{
    GUI_RECT_T  rect = {0};
    CTRLBASEPICKER_VTBL_T  *basepicker_vtbl_ptr = (CTRLBASEPICKER_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEPICKER_TYPE, basepicker_ctrl_ptr);

    if (PNULL != basepicker_vtbl_ptr->GetFieldRect)
    {
        rect = basepicker_vtbl_ptr->GetFieldRect(basepicker_ctrl_ptr, field);
    }

    return rect;
}

/*****************************************************************************/
//  Description : 获取域中的文字
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
static __inline void VTLBASEPICKER_GetFieldText(
                        CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,   // [in]
                        CTRLBASEPICKER_FIELD_T      field,                  // [in]
                        uint16                      cur_index,              // [in]
                        MMI_STRING_T                *dis_string_ptr         // [out]
                        )
{
    CTRLBASEPICKER_VTBL_T  *basepicker_vtbl_ptr = (CTRLBASEPICKER_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEPICKER_TYPE, basepicker_ctrl_ptr);

    if (PNULL != basepicker_vtbl_ptr->GetFieldText)
    {
        basepicker_vtbl_ptr->GetFieldText(basepicker_ctrl_ptr, field, cur_index, dis_string_ptr);
    }
}

/*****************************************************************************/
//  Description : 获取提示文本
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
static __inline MMI_TEXT_ID_T VTLBASEPICKER_GetFieldTips(
                                 CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
                                 int32                  field                   // [in]
                                 )
{
    MMI_TEXT_ID_T  text_id = {0};
    CTRLBASEPICKER_VTBL_T  *basepicker_vtbl_ptr = (CTRLBASEPICKER_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEPICKER_TYPE, basepicker_ctrl_ptr);

    if (PNULL != basepicker_vtbl_ptr->GetFieldTips)
    {
        text_id = basepicker_vtbl_ptr->GetFieldTips(basepicker_ctrl_ptr, field);
    }

    return text_id;
}

/*****************************************************************************/
//  Description : 检查值是否合法，如果不合法，将其置为合法值
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
static __inline void VTLBASEPICKER_CheckValue(
                 CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
                 BOOLEAN                is_need_anim            // [in] 是否需要绘制动画
                 )
{
    CTRLBASEPICKER_VTBL_T  *basepicker_vtbl_ptr = (CTRLBASEPICKER_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEPICKER_TYPE, basepicker_ctrl_ptr);

    if (PNULL != basepicker_vtbl_ptr->CheckValue)
    {
        basepicker_vtbl_ptr->CheckValue(basepicker_ctrl_ptr, is_need_anim);
    }
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif
