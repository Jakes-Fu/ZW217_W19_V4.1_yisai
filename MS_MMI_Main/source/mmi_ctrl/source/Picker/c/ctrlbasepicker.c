/*****************************************************************************
** File Name:      ctrlbasepicker.c                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012      hua.fang              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "mmk_tp.h"
#include "guires.h"
#include "mmi_theme.h"

#include "ctrlbasepicker.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define BASEPICKER_TIPS_SPACE           2
#define BASEPICKER_TIPS_OFFSET          15
#define BASEPICKER_MAX_DISP_COUNT       10              // 一个域里，最大显示个数
#define BASEPICKER_TP_MOVE_DIS          5               // 判断是否移动的偏移量

#define BASEPICKER_FIELD_NONE           (-1)

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
typedef struct
{
    GUI_RECT_T      disp_rect;      // 显示区域，这里用绝对位置
    uint16          value;          // 显示的值，AMPM自己处理
    GUI_FONT_ALL_T  font;           // 显示的字体
} BASEPICKER_FIELD_INFO_T;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : init basepicker class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlInitVtbl (
    CTRLBASEPICKER_VTBL_T        *basepicker_vtbl_ptr
);

/*****************************************************************************/
//  Description : basepicker construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlConstruct (
    CTRLBASE_OBJ_T              *base_ctrl_ptr,
    CTRLBASEPICKER_INIT_PARAM_T *basepicker_param_ptr
);

/*****************************************************************************/
//  Description : basepicker destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : handle basepicker msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E BasePickerCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : get basepicker pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLBASEPICKER_OBJ_T* GetBasePickerPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN BasePickerTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlGetHeightByWidth (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
    uint16              width,          //in:
    uint16              *height_ptr     //in/out:
);

/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlSetDisplayRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
);

/*****************************************************************************/
// 	Description : set ctrl rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlSetRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr       //in:
);

/*****************************************************************************/
//  Description : 获取开始域与结束域(不包含该值)
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerCtrlGetFieldRange (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    int32                  *start_field_ptr,       // [out]
    int32                  *end_field_ptr          // [out]
);

/*****************************************************************************/
//  Description : 获取指定域的最值
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerCtrlGetValueRange (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,// [in]
    CTRLBASEPICKER_FIELD_T      field,               // [in]
    uint16                      *min_value_ptr,      // [out]
    uint16                      *max_value_ptr       // [out]
);

/*****************************************************************************/
//  Description : 获取指定域的相对显示位置
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T BasePickerCtrlGetFieldRect (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,   // [in]
    int32                   field                   // [in]
);

/*****************************************************************************/
//  Description : 获取域中的文字
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerCtrlGetFieldText (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T      field,                  // [in]
    uint16                      cur_index,              // [in]
    MMI_STRING_T                *dis_string_ptr         // [out]
);

/*****************************************************************************/
//  Description : 获取提示文本
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T BasePickerCtrlGetFieldTips (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,     // [in]
    int32                  field               // [in]
);

/*****************************************************************************/
//  Description : 处理TP down消息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BasePickerHandleTpDown (
    CTRLBASEPICKER_OBJ_T  *basepicker_ctrl_ptr,   // [in]
    DPARAM                param                   // [in]
);

/*****************************************************************************/
//  Description : 处理TP move消息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BasePickerHandleTpMove (
    CTRLBASEPICKER_OBJ_T  *basepicker_ctrl_ptr,    // [in]
    DPARAM                param              // [in]
);

/*****************************************************************************/
//  Description : 处理TP up消息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BasePickerHandleTpUp (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,   // [in]
    DPARAM                  param                   // [in]
);

/*****************************************************************************/
//  Description : 开启scroll状态的定时器
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStartTpScrollTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr  // [in]
);

/*****************************************************************************/
//  Description : 关闭scroll状态的定时器
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStopTpScrollTimer (
    CTRLBASEPICKER_OBJ_T          *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T        field                   // [in] 指定哪个域的，如果还有其他域在用，就不会停止
);

/*****************************************************************************/
//  Description : 处理scroll状态的定时器
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//   scroll定时器，不用判断每个域，因为只有可能一个域进入scroll状态
/*****************************************************************************/
LOCAL void BasePickerHandleTpScrollTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr  // [in]
);

/*****************************************************************************/
//  Description : 开启fling状态的定时器
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStartFlingTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr     // [in]
);

/*****************************************************************************/
//  Description : 关闭fling状态的定时器
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStopFlingTimer (
    CTRLBASEPICKER_OBJ_T         *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T       field                   // [in] 指定哪个域的，如果还有其他域在用，就不会停止
);

/*****************************************************************************/
//  Description : 处理fling状态的定时器
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerHandleFlingTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr     // [in]
);

/*****************************************************************************/
//  Description : 开启recover状态的定时器
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStartRecoverTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr       // [in]
);

/*****************************************************************************/
//  Description : 关闭recover状态的定时器
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStopRecoverTimer (
    CTRLBASEPICKER_OBJ_T           *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T         field                   // [in] 指定哪个域的，如果还有其他域在用，就不会停止
);

/*****************************************************************************/
//  Description : 处理恢复状态的定时器
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerHandleRecoverTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr   // [in]
);

/*****************************************************************************/
//  Description : 根据某个域的内容
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayField (
    CTRLBASEPICKER_OBJ_T          *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T        field,                  // [in]
    GUI_RECT_T                    *dis_rect_ptr           // [in] 显示区域
);

/*****************************************************************************/
//  Description : 绘制背景
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayBg (
    CTRLBASEPICKER_OBJ_T      *basepicker_ctrl_ptr,     // [in]
    GUI_RECT_T                *dis_rect_ptr,
    GUI_RECT_T                *img_rect_ptr
);

/*****************************************************************************/
//  Description : 绘制边框
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayBorder (
    CTRLBASEPICKER_OBJ_T  *basepicker_ctrl_ptr,   // [in]
    GUI_RECT_T            *client_rect_ptr,       // [in]
    GUI_RECT_T            *img_rect_ptr           // [in]
);

/*****************************************************************************/
//  Description : 显示文本
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayText (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T      field,                  // [in]
    GUI_RECT_T                  *disp_rect_ptr          // [in]
);

/*****************************************************************************/
//  Description : 绘制分隔条
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplaySeparator (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    GUI_RECT_T             *separator_rect_ptr,    // [in]
    GUI_RECT_T             *relative_rect_ptr,     // [in]
    GUI_POINT_T            *start_pt_ptr,          // [in]
    MMI_IMAGE_ID_T         separator_image_id      // [in]
);

/*****************************************************************************/
//  Description : 绘制蒙板
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayMask (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,     // [in]
    GUI_RECT_T              *dis_rect_ptr
);

/*****************************************************************************/
//  Description : 绘制提示文本
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayTips (
    CTRLBASEPICKER_OBJ_T            *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T          field,                  // [in]
    GUI_RECT_T                      *dis_rect_ptr,          // [in]
    GUI_RECT_T                      *clip_rect_ptr          // [in]
);

/*****************************************************************************/
//  Description : 设置偏移位置，并自动更新界面
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetBasePickerOffset (
    CTRLBASEPICKER_OBJ_T           *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T         field,                  // [in]
    int32                          offset                  // [in] 与当前值的偏移量
);

/*****************************************************************************/
//  Description : 检查域里面的值是否合法
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void CheckValid (
    CTRLBASEPICKER_OBJ_T          *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T        field                   // [in]
);

/*****************************************************************************/
//  Description : 根据对齐方式，获取显示的起点位置
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetStartPoint (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,   // [in]
    GUI_POINT_T             *start_point_ptr        // [out]
);

/*****************************************************************************/
//  Description : 设置宽度
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void SetBasePickerSize (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,         // [in]
    uint16                 width,                  // [in]
    uint16                 height                  // [in]
);

/*****************************************************************************/
//  Description : 获取宽度
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void GetBasePickerSize (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    uint16                 *width_ptr,             // [out]
    uint16                 *height_ptr             // [out]
);

/*****************************************************************************/
//  Description : 获取行距
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TranslateRect (
    GUI_RECT_T     *rect_ptr,      // [in/out]
    int16          x,              // [in]
    int16          y               // [in]
);

/*****************************************************************************/
//  Description : 获取位置和内容信息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void GetBasePickerInfo (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,     // [in]
    CTRLBASEPICKER_FIELD_T      field,              // [in]
    BASEPICKER_FIELD_INFO_T     *field_info_ptr,    // [out] 返回的区域是绝对区域
    int32                       field_info_count    // [in]
);

/*****************************************************************************/
//  Description : 根据当前值，获取上一个，或下一个值
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//  返回是否成功，如果是单向的，到头之后返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN GetNextValue (
    CTRLBASEPICKER_OBJ_T         *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T       field,                  // [in]
    uint16                       cur_date_value,         // [in]
    BOOLEAN                      is_next,                // [in]
    uint16                       *next_value_ptr         // [out]
);

/*****************************************************************************/
//  Description : 发送选中消息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
__inline LOCAL void PostPickerSelectedMsg (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr     // [in]
);

/*****************************************************************************/
//  Description : 获取item的高度
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
__inline LOCAL uint16 GetItemHeight (
    CTRLBASEPICKER_OBJ_T *basepicker_ctrl_ptr  // [in]
);

/*****************************************************************************/
//  Description : get align
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
__inline LOCAL GUI_ALIGN_E GetAlign (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr
);

/*****************************************************************************/
//  Description : 根据DOWN时的点，判断是否移动了
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
__inline LOCAL BOOLEAN IsTpMoved (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    GUI_POINT_T            *point_ptr              // [in]
);
#endif

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get basepicker type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T BASEPICKER_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASE_TYPE,                                       // parent class type
                       "basepicker",                                         // control name
                       sizeof (CTRLBASEPICKER_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) BasePickerCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) BasePickerCtrlDestruct,      // control destructor function
                       sizeof (CTRLBASEPICKER_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) PNULL,                           // convert init param
                       sizeof (CTRLBASEPICKER_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) BasePickerCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init basepicker class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlInitVtbl (
    CTRLBASEPICKER_VTBL_T        *basepicker_vtbl_ptr
)
{
#ifdef TOUCH_PANEL_SUPPORT
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) basepicker_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = BasePickerCtrlHandleMsg;

    // 以下接口用于实现内嵌form
    base_ctrl_vtbl_ptr->GetHeightByWidth = BasePickerCtrlGetHeightByWidth;
    base_ctrl_vtbl_ptr->SetDisplayRect = BasePickerCtrlSetDisplayRect;
    base_ctrl_vtbl_ptr->SetRect = BasePickerCtrlSetRect;

    // virtual method for base picker ctrl
    basepicker_vtbl_ptr->GetFieldRange = BasePickerCtrlGetFieldRange;
    basepicker_vtbl_ptr->GetValueRange = BasePickerCtrlGetValueRange;
    basepicker_vtbl_ptr->GetFieldRect = BasePickerCtrlGetFieldRect;
    basepicker_vtbl_ptr->GetFieldText = BasePickerCtrlGetFieldText;
    basepicker_vtbl_ptr->GetFieldTips = BasePickerCtrlGetFieldTips;
#endif

    return TRUE;
}

/*****************************************************************************/
//  Description : basepicker construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlConstruct (
    CTRLBASE_OBJ_T              *base_ctrl_ptr,
    CTRLBASEPICKER_INIT_PARAM_T *basepicker_param_ptr
)
{
    BOOLEAN                 result = TRUE;

#ifdef TOUCH_PANEL_SUPPORT
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = (CTRLBASEPICKER_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr || PNULL == basepicker_param_ptr)
    {
        return FALSE;
    }

    MMITHEME_GetBasePickerTheme (&basepicker_ctrl_ptr->theme);

    base_ctrl_ptr->bg = basepicker_ctrl_ptr->theme.bg;

    BASEPICKER_Init(basepicker_ctrl_ptr);

#endif

    return (result);
}

/*****************************************************************************/
//  Description : basepicker destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN                 result = TRUE;

#ifdef TOUCH_PANEL_SUPPORT
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = (CTRLBASEPICKER_OBJ_T*) base_ctrl_ptr;

    if (PNULL == basepicker_ctrl_ptr)
    {
        return FALSE;
    }

    BASEPICKER_Destroy(basepicker_ctrl_ptr);

#endif

    return (result);
}

/*****************************************************************************/
//  Description : 设置提示文字的位置
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEPICKER_SetTipsPos (
    MMI_CTRL_ID_T                 ctrl_id,
    CTRLBASEPICKER_TIPS_POS_E     tips_pos
)
{
    BOOLEAN         result = FALSE;

#ifdef TOUCH_PANEL_SUPPORT
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = PNULL;

    //get ctrl pointer by control id
    basepicker_ctrl_ptr = GetBasePickerPtr (ctrl_id);

    if (PNULL != basepicker_ctrl_ptr)
    {
        BASEPICKER_SetTipsPos (basepicker_ctrl_ptr, tips_pos);

        result = TRUE;
    }

#endif

    return result;
}

/*****************************************************************************/
//  Description : 是否在设置值的时候做动画
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEPICKER_EnableAnim (
    MMI_CTRL_ID_T         ctrl_id,
    BOOLEAN               enable_anim
)
{
    BOOLEAN         result = FALSE;

#ifdef TOUCH_PANEL_SUPPORT
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = PNULL;

    //get ctrl pointer by control id
    basepicker_ctrl_ptr = GetBasePickerPtr (ctrl_id);

    if (PNULL != basepicker_ctrl_ptr)
    {
        basepicker_ctrl_ptr->enable_anim = enable_anim;
        result = TRUE;
    }

#endif

    return result;
}

/*****************************************************************************/
//  Description : 设置picker风格的高度
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEPICKER_SetHeight (
    MMI_CTRL_ID_T        ctrl_id,
    uint16               height
)
{
    BOOLEAN         result = FALSE;

#ifdef TOUCH_PANEL_SUPPORT
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = PNULL;

    //get ctrl pointer by control id
    basepicker_ctrl_ptr = GetBasePickerPtr (ctrl_id);

    if (PNULL != basepicker_ctrl_ptr && 0 != height)
    {
        basepicker_ctrl_ptr->theme.client_height = height;
        result = TRUE;
    }

#endif

    return result;
}

/*****************************************************************************/
//  Description : 设置picker风格的行高
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEPICKER_SetLineHeight (
    MMI_CTRL_ID_T    ctrl_id,
    uint16           line_height
)
{
    BOOLEAN                 result = FALSE;

#ifdef TOUCH_PANEL_SUPPORT
    uint16                  value[MMITHEME_BASEPICKER_MAX_FIELD_COUNT] = {0};
    int32                   i = 0;
    int32                   start_field = 0;
    int32                   end_field = 0;
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = PNULL;

    //get ctrl pointer by control id
    basepicker_ctrl_ptr = GetBasePickerPtr (ctrl_id);

    if (PNULL != basepicker_ctrl_ptr)
    {
        VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

        // store the value of each field, because it is calculated by line height
        for (i = start_field; i < end_field; i++)
        {
            value[i-start_field] = BASEPICKER_GetValue (basepicker_ctrl_ptr, i);
        }

        basepicker_ctrl_ptr->theme.line_height = line_height;

        // set the value again by new line height
        for (i = start_field; i < end_field; i++)
        {
            BASEPICKER_SetValue (basepicker_ctrl_ptr, i, value[i-start_field]);
        }

        result = TRUE;
    }

#endif

    return result;
}

/*****************************************************************************/
//  Description : 设置picker风格的文字字体
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEPICKER_SetFont (
    MMI_CTRL_ID_T      ctrl_id,
    GUI_FONT_ALL_T     *normal_font_ptr,
    GUI_FONT_ALL_T     *active_font_ptr
)
{
    BOOLEAN         result = FALSE;

#ifdef TOUCH_PANEL_SUPPORT
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = PNULL;

    //get ctrl pointer by control id
    basepicker_ctrl_ptr = GetBasePickerPtr (ctrl_id);

    if (PNULL != basepicker_ctrl_ptr)
    {
        if (PNULL != normal_font_ptr)
        {
            basepicker_ctrl_ptr->theme.content_font = *normal_font_ptr;
        }

        if (PNULL != active_font_ptr)
        {
            basepicker_ctrl_ptr->theme.cur_content_font = *active_font_ptr;
        }

        result = TRUE;
    }

#endif

    return result;
}

#ifdef TOUCH_PANEL_SUPPORT

/*****************************************************************************/
//  Description : init
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEPICKER_Init (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr    // [in]
)
{
    int32                   i = 0;
    int32                   start_field = 0;
    int32                   end_field   = 0;

    basepicker_ctrl_ptr->tp_down_field = BASEPICKER_FIELD_NONE;

    // 获取当前风格下，域的开始和结束范围，所有风格作统一处理
    VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

    for (i = start_field; i < end_field; i++)
    {
        // 归零，方法里面会设置成最小值
        BASEPICKER_SetValue (basepicker_ctrl_ptr, i, 0);

        SCI_ASSERT (PNULL == basepicker_ctrl_ptr->state[i].tracker_ptr);   /*assert verified*/
        basepicker_ctrl_ptr->state[i].tracker_ptr = MMK_CreateTracker();   // 创建加速器
        basepicker_ctrl_ptr->state[i].is_loop = TRUE;                      // 默认都要循环处理
    }
}

/*****************************************************************************/
//  Description : destroy
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEPICKER_Destroy (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr    // [in]
)
{
    int32 i = 0;

    BASEPICKER_Reset (basepicker_ctrl_ptr);

    for (i = 0; i < MMITHEME_BASEPICKER_MAX_FIELD_COUNT; i++)
    {
        if (PNULL != basepicker_ctrl_ptr->state[i].tracker_ptr)
        {
            MMK_DestroyTracker (basepicker_ctrl_ptr->state[i].tracker_ptr);
            basepicker_ctrl_ptr->state[i].tracker_ptr = PNULL;
        }
    }
}

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
)
{
    BOOLEAN result = TRUE;
    uint16  item_height = 0;
    uint16  min_value = 0;
    uint16  max_value = 0;

    VTLBASEPICKER_GetValueRange (basepicker_ctrl_ptr, field, &min_value, &max_value);

    // 超出最值，变为最值
    if (value < min_value)
    {
        value = min_value;
    }
    else if (value > max_value)
    {
        value = max_value;
    }

    // height of a item
    item_height = GetItemHeight (basepicker_ctrl_ptr);

    basepicker_ctrl_ptr->state[field].cur_offset = (value - min_value) * item_height;

    CheckValid (basepicker_ctrl_ptr, field);

    return result;
}

/*****************************************************************************/
//  Description : 获取域里的值
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEPICKER_GetValue (
    CTRLBASEPICKER_OBJ_T         *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T       field                   // [in]
)
{
    uint16  min_value = 0;
    uint16  max_value = 0;
    int32   cur_index = 0;
    uint16  item_height = 0;

    VTLBASEPICKER_GetValueRange (basepicker_ctrl_ptr, field, &min_value, &max_value);

    // height of a item
    item_height = GetItemHeight (basepicker_ctrl_ptr);

    cur_index = basepicker_ctrl_ptr->state[field].cur_offset / item_height;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(cur_index >= 0);     /*assert verified*/
    if (cur_index >= 0)
    {
        min_value += cur_index;
    }

    return min_value;
}

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
)
{
    uint16  min_value = 0;
    uint16  max_value = 0;
    uint16  item_height = 0;

    // height of a item
    item_height = GetItemHeight (basepicker_ctrl_ptr);

    VTLBASEPICKER_GetValueRange (basepicker_ctrl_ptr, field, &min_value, &max_value);

    basepicker_ctrl_ptr->state[field].recover_cur_offset = basepicker_ctrl_ptr->state[field].cur_offset;
    basepicker_ctrl_ptr->state[field].recover_dst_offset = item_height * (value - min_value);

    if (basepicker_ctrl_ptr->state[field].recover_cur_offset == basepicker_ctrl_ptr->state[field].recover_dst_offset)
    {
        basepicker_ctrl_ptr->state[field].recover_move_step = 0;
    }
    else if (is_up)
    {
        // recover_dst_offset的值可以超过总长度
        if (basepicker_ctrl_ptr->state[field].recover_dst_offset < basepicker_ctrl_ptr->state[field].recover_cur_offset)
        {
            basepicker_ctrl_ptr->state[field].recover_dst_offset += item_height * (max_value - min_value + 1);
        }

        // 步长加倍
        basepicker_ctrl_ptr->state[field].recover_move_step = (2 * MMITHEME_BASEPICKER_RECOVER_DISTANCE);
    }
    else
    {
        // recover_dst_offset的值可以<0
        if (basepicker_ctrl_ptr->state[field].recover_dst_offset > basepicker_ctrl_ptr->state[field].recover_cur_offset)
        {
            basepicker_ctrl_ptr->state[field].recover_dst_offset -= item_height * (max_value - min_value + 1);
        }

        // 步长加倍
        basepicker_ctrl_ptr->state[field].recover_move_step = - (2 * MMITHEME_BASEPICKER_RECOVER_DISTANCE);
    }

    basepicker_ctrl_ptr->state[field].slide_state = MMK_TP_SLIDE_RECOVER;
    BasePickerStartRecoverTimer (basepicker_ctrl_ptr);
}

/*****************************************************************************/
//  Description : reset picker
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEPICKER_Reset (
    CTRLBASEPICKER_OBJ_T *basepicker_ctrl_ptr
)
{
    int32           i = 0;
    int32           start_field = 0;
    int32           end_field = 0;
    uint16          value = 0;

    // reset the value
    VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

    for (i = start_field; i < end_field; i++)
    {
        basepicker_ctrl_ptr->state[i].slide_state = MMK_TP_SLIDE_NONE;

        value = BASEPICKER_GetValue (basepicker_ctrl_ptr, i);
        BASEPICKER_SetValue (basepicker_ctrl_ptr, i, value);
    }

    basepicker_ctrl_ptr->tp_down_field = BASEPICKER_FIELD_NONE;

    // reset all timers
    if (0 != basepicker_ctrl_ptr->tpscroll_timer_id)
    {
        MMK_StopTimer (basepicker_ctrl_ptr->tpscroll_timer_id);
        basepicker_ctrl_ptr->tpscroll_timer_id = 0;
    }

    if (0 != basepicker_ctrl_ptr->fling_timer_id)
    {
        MMK_StopTimer (basepicker_ctrl_ptr->fling_timer_id);
        basepicker_ctrl_ptr->fling_timer_id = 0;
    }

    if (0 != basepicker_ctrl_ptr->recover_timer_id)
    {
        MMK_StopTimer (basepicker_ctrl_ptr->recover_timer_id);
        basepicker_ctrl_ptr->recover_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : 根据区域显示picker内容
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEPICKER_Display (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr    // [in]
)
{
    BOOLEAN             result = FALSE;
    uint16              client_width = 0;
    uint16              client_height = 0;
    int32               i = 0;
    int32               start_field = 0;
    int32               end_field = 0;
    GUI_RECT_T          cross_rect = {0};
    GUI_RECT_T          client_rect = {0};              // 图片的显示区域(绝对区域)
    GUI_RECT_T          relative_dis_rect = {0};        // 显示区域的相对区域(相对于图片区域而言)
    GUI_RECT_T          text_rect = {0};                // 用于判断文本是否在可视区内
    GUI_RECT_T          tips_clip_rect = {0};
    GUI_RECT_T          field_rect = {0};
    GUI_RECT_T          separator_rect = {0};
    GUI_POINT_T         start_point = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    if (!GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, base_ctrl_ptr->rect))
    {
        return;
    }

    // 获取显示的起点位置，及图片的宽高信息
    result = GetStartPoint (basepicker_ctrl_ptr, &start_point);

    if (!result)
    {
        return;
    }

    GetBasePickerSize (basepicker_ctrl_ptr, &client_width, &client_height);

    // 图片的区域(转化为绝对位置)
    client_rect.left = start_point.x;
    client_rect.right = client_rect.left + client_width - 1;
    client_rect.top = start_point.y;
    client_rect.bottom = client_rect.top + client_height - 1;

    // 能够显示的情况
    if (GUI_IntersectRect (&client_rect, client_rect, cross_rect))
    {
        // 需要绘制的相对区域
        relative_dis_rect = client_rect;
        TranslateRect (&relative_dis_rect, -start_point.x, -start_point.y);

        // 画背景图
        BasePickerDisplayBg (basepicker_ctrl_ptr, &client_rect, &relative_dis_rect);

        // 画蒙层
        // picker风格，蒙板放在文字底下
        if (!basepicker_ctrl_ptr->theme.is_mask_on_top)
        {
            BasePickerDisplayMask (basepicker_ctrl_ptr, &cross_rect);
        }

        // 画文字, 提示文字，分隔条
        {
            VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

            separator_rect.top = 0;
            separator_rect.bottom = separator_rect.top + client_height - 1;

            // 查各个域是否在显示区域内
            for (i = start_field; i < end_field; i++)
            {
                field_rect = VTLBASEPICKER_GetFieldRect (basepicker_ctrl_ptr, i);

                // 画内容
                if (GUI_IntersectRect (&text_rect, field_rect, relative_dis_rect))
                {
                    TranslateRect (&text_rect, start_point.x, start_point.y);
                    BasePickerDisplayText (basepicker_ctrl_ptr, i, &text_rect);
                }

                // 画提示文本
                if (CTRLBASEPICKER_TIPS_POS_TOP == basepicker_ctrl_ptr->tips_pos)
                {
                    text_rect = field_rect;
                    text_rect.top = 0;
                    text_rect.bottom = field_rect.top - basepicker_ctrl_ptr->theme.border_width - 1;

                    if (GUI_IntersectRect (&tips_clip_rect, text_rect, relative_dis_rect))
                    {
                        TranslateRect (&text_rect, start_point.x, start_point.y);
                        TranslateRect (&tips_clip_rect, start_point.x, start_point.y);

                        BasePickerDisplayTips (basepicker_ctrl_ptr, i, &text_rect, &tips_clip_rect);
                    }
                }

                // 画分隔条
                if (basepicker_ctrl_ptr->theme.separator_width > 0)
                {
                    if (i == start_field)           // 如果第1项，画下最左边的分隔条
                    {
                        separator_rect.right = field_rect.left - 1;
                        separator_rect.left = separator_rect.right - basepicker_ctrl_ptr->theme.separator_width + 1;

                        BasePickerDisplaySeparator (
                            basepicker_ctrl_ptr, &separator_rect, &relative_dis_rect,
                            &start_point, basepicker_ctrl_ptr->theme.separator_image_id);
                    }

                    separator_rect.left = field_rect.right + 1;
                    separator_rect.right = separator_rect.left + basepicker_ctrl_ptr->theme.separator_width - 1;

                    BasePickerDisplaySeparator (
                        basepicker_ctrl_ptr, &separator_rect, &relative_dis_rect,
                        &start_point, basepicker_ctrl_ptr->theme.separator_image_id);
                }
            }
        }

        // 画蒙层
        // 非picker风格，蒙板在文字上面
        if (basepicker_ctrl_ptr->theme.is_mask_on_top)
        {
            BasePickerDisplayMask (basepicker_ctrl_ptr, &cross_rect);
        }

        // 画边框
        if (basepicker_ctrl_ptr->theme.border_width > 0)
        {
            BasePickerDisplayBorder (basepicker_ctrl_ptr, &client_rect, &relative_dis_rect);
        }
    }
}

/*****************************************************************************/
//  Description : 设置提示文字的位置
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEPICKER_SetTipsPos (
    CTRLBASEPICKER_OBJ_T          *basepicker_ctrl_ptr,
    CTRLBASEPICKER_TIPS_POS_E     tips_pos
)
{
    basepicker_ctrl_ptr->tips_pos = tips_pos;

    if (CTRLBASEPICKER_TIPS_POS_TOP != tips_pos)
    {
        basepicker_ctrl_ptr->theme.top_margin = basepicker_ctrl_ptr->theme.bottom_margin;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : 获取整个文本的区域，(扣除提示文字)
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T BASEPICKER_GetTextRect (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr
)
{
    GUI_RECT_T  text_rect = {0};

    text_rect.top = basepicker_ctrl_ptr->theme.top_margin;
    text_rect.bottom = basepicker_ctrl_ptr->theme.client_height - basepicker_ctrl_ptr->theme.bottom_margin - 1;
    text_rect.left = basepicker_ctrl_ptr->theme.separator_width + basepicker_ctrl_ptr->theme.border_width;
    text_rect.right = basepicker_ctrl_ptr->theme.client_width - (basepicker_ctrl_ptr->theme.separator_width + basepicker_ctrl_ptr->theme.border_width) - 1;

    return text_rect;
}

/*****************************************************************************/
//  Description : handle basepicker msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E BasePickerCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    CTRLBASE_VTBL_T         *parent_vtbl_ptr = PNULL;
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = (CTRLBASEPICKER_OBJ_T*) base_ctrl_ptr;

    if (PNULL == basepicker_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_CTL_PAINT:
        if (VTLBASE_GetVisible (base_ctrl_ptr))
        {
            BASEPICKER_Display (basepicker_ctrl_ptr);
        }

        break;

    case MSG_CTL_GET_ACTIVE:
        break;

    case MSG_CTL_LOSE_ACTIVE:
    case MSG_CTL_CLOSE:
        break;

    case MSG_CTL_LOSE_FOCUS:
        BASEPICKER_Reset (basepicker_ctrl_ptr);
        break;

    case MSG_NOTIFY_LOSE_ACTIVE:
        break;

    case MSG_TIMER:

        if (basepicker_ctrl_ptr->tpscroll_timer_id == * (uint8 *) param)
        {
            BasePickerHandleTpScrollTimer (basepicker_ctrl_ptr);
        }
        else if (basepicker_ctrl_ptr->fling_timer_id == * (uint8 *) param)
        {
            BasePickerHandleFlingTimer (basepicker_ctrl_ptr);
        }
        else if (basepicker_ctrl_ptr->recover_timer_id == * (uint8 *) param)
        {
            BasePickerHandleRecoverTimer (basepicker_ctrl_ptr);
        }

        break;

    case MSG_TP_PRESS_DOWN:
        BasePickerHandleTpDown (basepicker_ctrl_ptr, param);
        break;

    case MSG_TP_PRESS_MOVE:
        BasePickerHandleTpMove (basepicker_ctrl_ptr, param);
        break;

    case MSG_TP_PRESS_UP:
        BasePickerHandleTpUp (basepicker_ctrl_ptr, param);
        break;

    default:
        parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_BASEPICKER_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
        result = parent_vtbl_ptr->HandleEvent (base_ctrl_ptr, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlGetHeightByWidth (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
    uint16              width,          //in:
    uint16              *height_ptr     //in/out:
)
{
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr = (CTRLBASEPICKER_OBJ_T*) base_ctrl_ptr;

    SetBasePickerSize (basepicker_ctrl_ptr, width, 0);
    GetBasePickerSize (basepicker_ctrl_ptr, PNULL, height_ptr);

    return TRUE;
}

/*****************************************************************************/
// 	Description : set ctrl rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlSetRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr       //in:
)
{
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr = (CTRLBASEPICKER_OBJ_T*) base_ctrl_ptr;
    CTRLBASE_VTBL_T        *parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_BASEPICKER_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);

    if ( (!GUI_IsRectEmpty (*rect_ptr)) &&
            (!GUI_EqualRect (base_ctrl_ptr->rect, *rect_ptr)))
    {
        parent_vtbl_ptr->SetRect (base_ctrl_ptr, rect_ptr);

        GUI_SetVOrHRect( base_ctrl_ptr->handle, (GUI_RECT_T *)rect_ptr,&(base_ctrl_ptr->both_rect));

        SetBasePickerSize (
            basepicker_ctrl_ptr,
            rect_ptr->right - rect_ptr->left + 1,
            rect_ptr->bottom - rect_ptr->top + 1);
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : set ctrl display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN BasePickerCtrlSetDisplayRect (
    CTRLBASE_OBJ_T      *base_ctrl_ptr, //in:
    const GUI_RECT_T    *rect_ptr,      //in:
    BOOLEAN             is_update       //in:
)
{
    BOOLEAN     result = FALSE;
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr = (CTRLBASEPICKER_OBJ_T*) base_ctrl_ptr;
    CTRLBASE_VTBL_T        *parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_BASEPICKER_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);

    result = parent_vtbl_ptr->SetDisplayRect (base_ctrl_ptr, rect_ptr, is_update);

    if (is_update)
    {
        BASEPICKER_Display (basepicker_ctrl_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : 获取开始域与结束域(不包含该值)
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerCtrlGetFieldRange (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    int32                  *start_field_ptr,       // [out]
    int32                  *end_field_ptr          // [out]
)
{
    SCI_ASSERT (0); /*assert verified*/
}

/*****************************************************************************/
//  Description : 获取指定域的最值
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerCtrlGetValueRange (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,// [in]
    CTRLBASEPICKER_FIELD_T      field,               // [in]
    uint16                      *min_value_ptr,      // [out]
    uint16                      *max_value_ptr       // [out]
)
{
    SCI_ASSERT (0); /*assert verified*/
}

/*****************************************************************************/
//  Description : 获取指定域的相对显示位置
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T BasePickerCtrlGetFieldRect (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,   // [in]
    int32                   field                   // [in]
)
{
    GUI_RECT_T temp = {0};

    SCI_ASSERT (0); /*assert verified*/
    return temp;
}

/*****************************************************************************/
//  Description : 获取域中的文字
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerCtrlGetFieldText (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T      field,                  // [in]
    uint16                      cur_index,              // [in]
    MMI_STRING_T                *dis_string_ptr         // [out]
)
{
    SCI_ASSERT (0); /*assert verified*/
}

/*****************************************************************************/
//  Description : 获取提示文本
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T BasePickerCtrlGetFieldTips (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,     // [in]
    int32                  field               // [in]
)
{
    MMI_TEXT_ID_T temp = 0;
    SCI_ASSERT (0); /*assert verified*/
    return temp;
}

/*****************************************************************************/
//  Description : 处理TP down消息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BasePickerHandleTpDown (
    CTRLBASEPICKER_OBJ_T  *basepicker_ctrl_ptr,   // [in]
    DPARAM                param                   // [in]
)
{
    int32           i = 0;
    int32           start_field = 0;
    int32           end_field = 0;
    GUI_POINT_T     tp_point = {0};             // TP点的位置
    GUI_POINT_T     relative_tp_point = {0};    // TP相对于控件的位置
    GUI_POINT_T     start_point = {0};          // 绘制的起始位置

    //set tp press point
    tp_point.x = MMK_GET_TP_X (param);
    tp_point.y = MMK_GET_TP_Y (param);

    basepicker_ctrl_ptr->tp_down_field = BASEPICKER_FIELD_NONE;

    VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

    GetStartPoint (basepicker_ctrl_ptr, &start_point);

    for (i = start_field; i < end_field; i++)
    {
        // 计算TP相对位置
        relative_tp_point = tp_point;
        relative_tp_point.x -= start_point.x;
        relative_tp_point.y -= start_point.y;

        // 是否点击在这个域里
        if (GUI_PointIsInRect (relative_tp_point, VTLBASEPICKER_GetFieldRect (basepicker_ctrl_ptr, i)))
        {
            // 只可能是MMK_TP_SLIDE_FLING,MMK_TP_SLIDE_RECOVER,MMK_TP_SLIDE_NONE
            if (MMK_TP_SLIDE_TP_SCROLL == basepicker_ctrl_ptr->state[i].slide_state)
            {
                //SCI_TRACE_LOW:"slide_state error: %d"
                SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_TOUCH_645_112_2_18_3_16_22_138, (uint8*) "d", basepicker_ctrl_ptr->state[i].slide_state);
                break;
            }

            basepicker_ctrl_ptr->is_tp_moved = FALSE;

            basepicker_ctrl_ptr->tp_down_field = i;

            // 记录起始点
            basepicker_ctrl_ptr->tp_start_point = tp_point;

            // 记录前一个点
            basepicker_ctrl_ptr->tp_pre_point = tp_point;

            MMK_ResetVelocityItemByTracker (
                basepicker_ctrl_ptr->state[basepicker_ctrl_ptr->tp_down_field].tracker_ptr);

            MMK_AddVelocityItemByTracker (
                basepicker_ctrl_ptr->state[i].tracker_ptr,
                tp_point.x, tp_point.y);

            // 在拖动或恢复过程中，TP点下去的点，算做移动，后续的TP UP就不会将点击的值移动
            if (MMK_TP_SLIDE_FLING == basepicker_ctrl_ptr->state[i].slide_state)
            {
                basepicker_ctrl_ptr->is_tp_moved = TRUE;
                BasePickerStopFlingTimer (basepicker_ctrl_ptr, i);
            }
            else if (MMK_TP_SLIDE_RECOVER == basepicker_ctrl_ptr->state[i].slide_state)
            {
                basepicker_ctrl_ptr->is_tp_moved = TRUE;
                BasePickerStopRecoverTimer (basepicker_ctrl_ptr, i);
            }

            basepicker_ctrl_ptr->state[i].slide_state = MMK_TP_SLIDE_NONE;

            break;
        }
    }
}

/*****************************************************************************/
//  Description : 处理TP move消息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BasePickerHandleTpMove (
    CTRLBASEPICKER_OBJ_T  *basepicker_ctrl_ptr,    // [in]
    DPARAM                param              // [in]
)
{
    GUI_POINT_T             tp_point = {0};
    CTRLBASEPICKER_FIELD_T  field = basepicker_ctrl_ptr->tp_down_field;    // tp_down_field后面用的太多了，这里用临时变量存一下

    if (BASEPICKER_FIELD_NONE != field)
    {
        if (MMK_TP_SLIDE_NONE != basepicker_ctrl_ptr->state[field].slide_state
                && MMK_TP_SLIDE_TP_SCROLL != basepicker_ctrl_ptr->state[field].slide_state)
        {
            //SCI_TRACE_LOW:"slide_state error: %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_TOUCH_702_112_2_18_3_16_22_139, (uint8*) "d", basepicker_ctrl_ptr->state[field].slide_state);
            return;
        }

        basepicker_ctrl_ptr->is_tp_moved = TRUE;

        //set tp press point
        tp_point.x = MMK_GET_TP_X (param);
        tp_point.y = MMK_GET_TP_Y (param);

        // 增加速度点
        MMK_AddVelocityItemByTracker (
            basepicker_ctrl_ptr->state[field].tracker_ptr,
            tp_point.x, tp_point.y);

        if (MMK_TP_SLIDE_TP_SCROLL != basepicker_ctrl_ptr->state[field].slide_state
                && IsTpMoved (basepicker_ctrl_ptr, &tp_point))
        {
            basepicker_ctrl_ptr->state[field].slide_state = MMK_TP_SLIDE_TP_SCROLL;
            BasePickerStartTpScrollTimer (basepicker_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : 处理TP up消息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BasePickerHandleTpUp (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,   // [in]
    DPARAM                  param                   // [in]
)
{
    int32           i = 0;
    uint16          item_height = 0;
    int32           item_offset = 0;
    GUI_POINT_T     tp_point = {0};
    CTRLBASEPICKER_FIELD_T      field = basepicker_ctrl_ptr->tp_down_field;
    BASEPICKER_FIELD_INFO_T     field_info[BASEPICKER_MAX_DISP_COUNT] = {0};     // 域里面值的显示信息(包括位置和值)

    if (BASEPICKER_FIELD_NONE != field)
    {
        if (MMK_TP_SLIDE_NONE != basepicker_ctrl_ptr->state[field].slide_state
                && MMK_TP_SLIDE_TP_SCROLL != basepicker_ctrl_ptr->state[field].slide_state)
        {
            //SCI_TRACE_LOW:"slide_state error: %d"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_TOUCH_747_112_2_18_3_16_22_140, (uint8*) "d", basepicker_ctrl_ptr->state[field].slide_state);
            return;
        }

        BasePickerStopTpScrollTimer (basepicker_ctrl_ptr, field);

        //set tp press point
        tp_point.x = MMK_GET_TP_X (param);
        tp_point.y = MMK_GET_TP_Y (param);

        // 增加速度点
        MMK_AddVelocityItemByTracker (
            basepicker_ctrl_ptr->state[field].tracker_ptr,
            tp_point.x, tp_point.y);

        MMK_ComputeCurrentVelocityByTracker (
            basepicker_ctrl_ptr->state[field].tracker_ptr,
            (float *) PNULL,
            &basepicker_ctrl_ptr->state[field].fling_velocity,
            0.f,
            FLING_MAX_VELOCITY);

        //// 这里降下速，不然太快了
        //basepicker_ctrl_ptr->state[field].fling_velocity /= 2;

        if (FLING_MIN_VELOCITY <= abs ( (int32) basepicker_ctrl_ptr->state[field].fling_velocity))  // 达到一定速度，需要滑动
        {
            // 进入滑动状态，起动滑动定时器
            basepicker_ctrl_ptr->state[field].slide_state = MMK_TP_SLIDE_FLING;
            BasePickerStartFlingTimer (basepicker_ctrl_ptr);
        }
        else    // 恢复原位
        {
            // 如果没有移动过，说明是直接点击某个区域的，自动跳到那个值
            if (!basepicker_ctrl_ptr->is_tp_moved)
            {
                // 获取显示信息
                GetBasePickerInfo (basepicker_ctrl_ptr, field, field_info, BASEPICKER_MAX_DISP_COUNT);

                for (i = 0; i < BASEPICKER_MAX_DISP_COUNT; i++)
                {
                    if (GUI_PointIsInRect (tp_point, field_info[i].disp_rect))
                    {
                        if (field_info[i].disp_rect.top > field_info[0].disp_rect.top)  // 点在下方，向上移动
                        {
                            BASEPICKER_SetRecoverValue (basepicker_ctrl_ptr, field, field_info[i].value, TRUE);
                        }
                        else
                        {
                            BASEPICKER_SetRecoverValue (basepicker_ctrl_ptr, field, field_info[i].value, FALSE);
                        }

                        break;
                    }
                }

                // 手抬起，并且没有移动的情况下，发消息通知应用
                PostPickerSelectedMsg (basepicker_ctrl_ptr);
            }
            else
            {
                // height of a item
                item_height = GetItemHeight (basepicker_ctrl_ptr);

                // 这个item离它的起点的偏移量
                item_offset = basepicker_ctrl_ptr->state[field].cur_offset % item_height;

                // 设置目标位置，及当前的位置
                basepicker_ctrl_ptr->state[field].recover_cur_offset = basepicker_ctrl_ptr->state[field].cur_offset;
                basepicker_ctrl_ptr->state[field].recover_dst_offset = basepicker_ctrl_ptr->state[field].cur_offset - item_offset;

                // 确定移动步长
                if (0 == item_offset)   // 正好停在当前值的位置，起动recover timer，在timer到时检查时间日期值是否正确
                {
                    basepicker_ctrl_ptr->state[field].recover_move_step = 0;
                }
                else if (item_offset > item_height / 2)       // 如果当前值已经拖动超过了一半，就往上跳到下一个值
                {
                    basepicker_ctrl_ptr->state[field].recover_move_step = MMITHEME_BASEPICKER_RECOVER_DISTANCE;
                    basepicker_ctrl_ptr->state[field].recover_dst_offset += item_height;
                }
                else    // 否则，恢复到当前值
                {
                    basepicker_ctrl_ptr->state[field].recover_move_step = -MMITHEME_BASEPICKER_RECOVER_DISTANCE;
                }

                basepicker_ctrl_ptr->state[field].slide_state = MMK_TP_SLIDE_RECOVER;
                BasePickerStartRecoverTimer (basepicker_ctrl_ptr);
            }
        }

        basepicker_ctrl_ptr->tp_down_field = BASEPICKER_FIELD_NONE;
    }
}

/*****************************************************************************/
//  Description : 开启scroll状态的定时器
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStartTpScrollTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr  // [in]
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    if (PNULL != basepicker_ctrl_ptr && 0 == basepicker_ctrl_ptr->tpscroll_timer_id)
    {
        basepicker_ctrl_ptr->tpscroll_timer_id = MMK_CreateWinTimer (
                    base_ctrl_ptr->handle,
                    MMITHEME_BASEPICKER_TPSCROLL_TIME,
                    TRUE);
    }
}

/*****************************************************************************/
//  Description : 关闭scroll状态的定时器
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStopTpScrollTimer (
    CTRLBASEPICKER_OBJ_T          *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T        field                   // [in] 指定哪个域的，如果还有其他域在用，就不会停止
)
{
    BOOLEAN can_stop = TRUE;
    int32   i = 0;
    int32   start_field = 0;
    int32   end_field = 0;

    if (PNULL != basepicker_ctrl_ptr && 0 != basepicker_ctrl_ptr->tpscroll_timer_id)
    {
        VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

        // 因为有可能会有多个域同时共用了这个定时器，所以要判断各个域是否有用，
        // 再决定是否要停掉
        for (i = start_field; i < end_field; i++)
        {
            if (MMK_TP_SLIDE_TP_SCROLL == basepicker_ctrl_ptr->state[i].slide_state && i != field)
            {
                can_stop = FALSE;
                break;
            }
        }

        if (can_stop)
        {
            MMK_StopTimer (basepicker_ctrl_ptr->tpscroll_timer_id);
            basepicker_ctrl_ptr->tpscroll_timer_id = 0;
        }
    }
}

/*****************************************************************************/
//  Description : 处理scroll状态的定时器
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//   scroll定时器，不用判断每个域，因为只有可能一个域进入scroll状态
/*****************************************************************************/
LOCAL void BasePickerHandleTpScrollTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr  // [in]
)
{
    int32               tp_move_offset = 0;
    GUI_POINT_T         point   = {0};
    MMI_TP_STATUS_E     state   = MMI_TP_NONE;

    if (BASEPICKER_FIELD_NONE == basepicker_ctrl_ptr->tp_down_field)
    {
        //SCI_TRACE_LOW:"slide_state error: %d"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_TOUCH_955_112_2_18_3_16_23_141, (uint8*) "d", basepicker_ctrl_ptr->tp_down_field);
        return;
    }

    // restart tp move timer
    BasePickerStopTpScrollTimer (basepicker_ctrl_ptr, basepicker_ctrl_ptr->tp_down_field);
    BasePickerStartTpScrollTimer (basepicker_ctrl_ptr);

    MMK_GetLogicTPMsg (&state, &point);

    tp_move_offset = basepicker_ctrl_ptr->tp_pre_point.y - point.y;

    SetBasePickerOffset (basepicker_ctrl_ptr, basepicker_ctrl_ptr->tp_down_field, tp_move_offset);

    // store tp point
    basepicker_ctrl_ptr->tp_pre_point = point;
}

/*****************************************************************************/
//  Description : 开启fling状态的定时器
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStartFlingTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr     // [in]
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    if (PNULL != basepicker_ctrl_ptr && 0 == basepicker_ctrl_ptr->fling_timer_id)
    {
        basepicker_ctrl_ptr->fling_timer_id = MMK_CreateWinTimer (
                                                  base_ctrl_ptr->handle,
                                                  MMITHEME_BASEPICKER_FLING_TIME,
                                                  TRUE);
    }
}

/*****************************************************************************/
//  Description : 关闭fling状态的定时器
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStopFlingTimer (
    CTRLBASEPICKER_OBJ_T         *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T       field                   // [in] 指定哪个域的，如果还有其他域在用，就不会停止
)
{
    BOOLEAN can_stop = TRUE;
    int32   i = 0;
    int32   start_field = 0;
    int32   end_field = 0;

    if (PNULL != basepicker_ctrl_ptr && 0 != basepicker_ctrl_ptr->fling_timer_id)
    {
        VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

        // 因为有可能会有多个域同时共用了这个定时器，所以要判断各个域是否有用，
        // 再决定是否要停掉
        for (i = start_field; i < end_field; i++)
        {
            if (MMK_TP_SLIDE_FLING == basepicker_ctrl_ptr->state[i].slide_state && i != field)
            {
                can_stop = FALSE;
                break;
            }
        }

        if (can_stop)
        {
            MMK_StopTimer (basepicker_ctrl_ptr->fling_timer_id);
            basepicker_ctrl_ptr->fling_timer_id = 0;
        }
    }
}

/*****************************************************************************/
//  Description : 处理fling状态的定时器
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerHandleFlingTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr     // [in]
)
{
    BOOLEAN is_fling_state = FALSE;
    uint16  item_height = 0;
    int32   i = 0;
    int32   old_pos = 0;        // 原来未修改之前的位置
    int32   cur_pos = 0;        // 加过偏移量后的位置
    int32   item_offset = 0;
    int32   start_field = 0;
    int32   end_field = 0;

    VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

    for (i = start_field; i < end_field; i++)
    {
        if (MMK_TP_SLIDE_FLING == basepicker_ctrl_ptr->state[i].slide_state)
        {
            BasePickerStopFlingTimer (basepicker_ctrl_ptr, i);

            old_pos = basepicker_ctrl_ptr->state[i].cur_offset;

            // 重算加速度
            cur_pos = MMK_GetFlingOffset (
                          old_pos, basepicker_ctrl_ptr->state[i].fling_velocity,
                          (float) FLING_TIME, &basepicker_ctrl_ptr->state[i].fling_velocity);

            is_fling_state = FALSE;

            if (FLING_RUN_MIN_VELOCITY <= abs ( (int32) basepicker_ctrl_ptr->state[i].fling_velocity)) // 还可以加速运动
            {
                // 没改变的情况下，走RECOVER状态
                if (SetBasePickerOffset (basepicker_ctrl_ptr, i, old_pos - cur_pos))
                {
                    BasePickerStartFlingTimer (basepicker_ctrl_ptr);
                    is_fling_state = TRUE;
                }
            }

            if (!is_fling_state)    // 小于最小加速度，进入恢复状态
            {
                item_height = GetItemHeight (basepicker_ctrl_ptr);
                item_offset = basepicker_ctrl_ptr->state[i].cur_offset % item_height;

                basepicker_ctrl_ptr->state[i].recover_cur_offset = basepicker_ctrl_ptr->state[i].cur_offset;
                basepicker_ctrl_ptr->state[i].recover_dst_offset = basepicker_ctrl_ptr->state[i].cur_offset - item_offset;

                if ( ( (int32) basepicker_ctrl_ptr->state[i].fling_velocity) > 0)   // 往下拖动的情况，需要往下走
                {
                    basepicker_ctrl_ptr->state[i].recover_move_step = -MMITHEME_BASEPICKER_RECOVER_DISTANCE;
                }
                else                                                    // 往上拖动的情况
                {
                    basepicker_ctrl_ptr->state[i].recover_move_step = MMITHEME_BASEPICKER_RECOVER_DISTANCE;

                    if (basepicker_ctrl_ptr->state[i].is_loop)
                    {
                        basepicker_ctrl_ptr->state[i].recover_dst_offset += item_height;
                    }
                }

                basepicker_ctrl_ptr->state[i].slide_state = MMK_TP_SLIDE_RECOVER;
                BasePickerStartRecoverTimer (basepicker_ctrl_ptr);
            }
        }
    }
}

/*****************************************************************************/
//  Description : 开启recover状态的定时器
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStartRecoverTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr       // [in]
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    if (PNULL != basepicker_ctrl_ptr && 0 == basepicker_ctrl_ptr->recover_timer_id)
    {
        basepicker_ctrl_ptr->recover_timer_id = MMK_CreateWinTimer (
                                                    base_ctrl_ptr->handle,
                                                    MMITHEME_BASEPICKER_RECOVER_TIME,
                                                    TRUE);
    }
}

/*****************************************************************************/
//  Description : 关闭recover状态的定时器
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerStopRecoverTimer (
    CTRLBASEPICKER_OBJ_T           *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T         field                   // [in] 指定哪个域的，如果还有其他域在用，就不会停止
)
{
    BOOLEAN can_stop = TRUE;
    int32   i = 0;
    int32   start_field = 0;
    int32   end_field = 0;

    if (PNULL != basepicker_ctrl_ptr && 0 != basepicker_ctrl_ptr->recover_timer_id)
    {
        VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

        // 因为有可能会有多个域同时共用了这个定时器，所以要判断各个域是否有用，
        // 再决定是否要停掉
        for (i = start_field; i < end_field; i++)
        {
            if (MMK_TP_SLIDE_RECOVER == basepicker_ctrl_ptr->state[i].slide_state && i != field)
            {
                can_stop = FALSE;
                break;
            }
        }

        if (can_stop)
        {
            MMK_StopTimer (basepicker_ctrl_ptr->recover_timer_id);
            basepicker_ctrl_ptr->recover_timer_id = 0;
        }
    }
}

/*****************************************************************************/
//  Description : 处理恢复状态的定时器
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerHandleRecoverTimer (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr   // [in]
)
{
    BOOLEAN is_need_check_value = TRUE;     // 是否需要检查时间
    int32   i = 0;
    int32   start_field = 0;
    int32   end_field = 0;
    int32   recover_old_offset = 0;

    VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

    for (i = start_field; i < end_field; i++)
    {
        if (MMK_TP_SLIDE_RECOVER == basepicker_ctrl_ptr->state[i].slide_state)
        {
            BasePickerStopRecoverTimer (basepicker_ctrl_ptr, i);

            recover_old_offset = basepicker_ctrl_ptr->state[i].recover_cur_offset;

            basepicker_ctrl_ptr->state[i].recover_cur_offset += basepicker_ctrl_ptr->state[i].recover_move_step;

            // 已经恢复到目标位置，或是滑过头的情况，改变状态
            if (0 == basepicker_ctrl_ptr->state[i].recover_move_step     // 抬起时正好在原位
                    || (basepicker_ctrl_ptr->state[i].recover_cur_offset <= basepicker_ctrl_ptr->state[i].recover_dst_offset    // 减过头了
                        && basepicker_ctrl_ptr->state[i].recover_move_step < 0)
                    || (basepicker_ctrl_ptr->state[i].recover_cur_offset >= basepicker_ctrl_ptr->state[i].recover_dst_offset    // 加过头了
                        && basepicker_ctrl_ptr->state[i].recover_move_step > 0))
            {
                // 结束scroll状态，进入none状态
                basepicker_ctrl_ptr->state[i].recover_cur_offset = basepicker_ctrl_ptr->state[i].recover_dst_offset;
                basepicker_ctrl_ptr->state[i].slide_state = MMK_TP_SLIDE_NONE;

                // 滑动完后，通知应用
                PostPickerSelectedMsg (basepicker_ctrl_ptr);
            }
            else
            {
                // 重新scroll
                BasePickerStartRecoverTimer (basepicker_ctrl_ptr);
            }

            SetBasePickerOffset (basepicker_ctrl_ptr, i, basepicker_ctrl_ptr->state[i].recover_cur_offset - recover_old_offset);
        }

        // 如果其他还有人scroll, 那不需要检查值
        if (MMK_TP_SLIDE_NONE != basepicker_ctrl_ptr->state[i].slide_state)
        {
            is_need_check_value = FALSE;
        }
    }

    // 如果是按下的情况，也不需要检查
    if (is_need_check_value && BASEPICKER_FIELD_NONE == basepicker_ctrl_ptr->tp_down_field)
    {
        VTLBASEPICKER_CheckValue (basepicker_ctrl_ptr, TRUE);
    }
}

/*****************************************************************************/
//  Description : 根据某个域的内容
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayField (
    CTRLBASEPICKER_OBJ_T          *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T        field,                  // [in]
    GUI_RECT_T                    *dis_rect_ptr           // [in] 显示区域
)
{
    BOOLEAN     result = FALSE;
    uint16      client_width = 0;
    uint16      client_height = 0;
    GUI_RECT_T  img_rect = {0};                 // 图片的显示区域(绝对区域)
    GUI_RECT_T  relative_dis_rect = {0};        // 显示区域的相对区域(相对于图片区域而言)
    GUI_RECT_T  text_rect = {0};                // 用于判断文本是否在可视区内
    GUI_POINT_T start_point = {0};

    // 获取显示的起点位置，及图片的宽高信息
    result = GetStartPoint (basepicker_ctrl_ptr, &start_point);

    if (!result)
    {
        return;
    }

    GetBasePickerSize (basepicker_ctrl_ptr, &client_width, &client_height);

    // 图片的区域(转化为绝对位置)
    img_rect.left = start_point.x;
    img_rect.right = img_rect.left + client_width - 1;
    img_rect.top = start_point.y;
    img_rect.bottom = img_rect.top + client_height - 1;

    // 能够显示的情况
    if (GUI_IntersectRect (&img_rect, img_rect, *dis_rect_ptr))
    {
        // 需要绘制的相对区域
        relative_dis_rect = img_rect;
        TranslateRect (&relative_dis_rect, -start_point.x, -start_point.y);

        // 画背景图
        BasePickerDisplayBg (basepicker_ctrl_ptr, &img_rect, &relative_dis_rect);

        // 画蒙层
        // picker风格，蒙板放在文字底下
        if (!basepicker_ctrl_ptr->theme.is_mask_on_top)
        {
            BasePickerDisplayMask (basepicker_ctrl_ptr, dis_rect_ptr);
        }

        // 画文字
        if (GUI_IntersectRect (&text_rect, VTLBASEPICKER_GetFieldRect (basepicker_ctrl_ptr, field), relative_dis_rect))
        {
            BasePickerDisplayText (basepicker_ctrl_ptr, field, &img_rect);
        }

        // 画蒙层
        // 非picker风格，蒙板在文字上面
        if (basepicker_ctrl_ptr->theme.is_mask_on_top)
        {
            BasePickerDisplayMask (basepicker_ctrl_ptr, dis_rect_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : 绘制背景
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayBg (
    CTRLBASEPICKER_OBJ_T      *basepicker_ctrl_ptr,     // [in]
    GUI_RECT_T                *dis_rect_ptr,
    GUI_RECT_T                *img_rect_ptr
)
{
    GUI_RECT_T          img_rect = {0};
    GUI_BG_DISPLAY_T    bg_display = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    //set image rect
    img_rect.left   = (int16) (dis_rect_ptr->left - base_ctrl_ptr->rect.left);
    img_rect.top    = (int16) (dis_rect_ptr->top - base_ctrl_ptr->rect.top);
    img_rect.right  = (int16) (img_rect.left + dis_rect_ptr->right - dis_rect_ptr->left);
    img_rect.bottom = (int16) (img_rect.top + dis_rect_ptr->bottom - dis_rect_ptr->top);

    //set bg display info
    bg_display.is_transparent = FALSE;
    bg_display.rect           = base_ctrl_ptr->rect;
    bg_display.display_rect   = *dis_rect_ptr;
    bg_display.img_rect       = img_rect;
    bg_display.ctrl_handle    = base_ctrl_ptr->handle;
    bg_display.win_handle     = base_ctrl_ptr->win_handle;

    //display bg
    GUI_DisplayBg (&base_ctrl_ptr->bg,
                   &bg_display,
                   &base_ctrl_ptr->lcd_dev_info);
}

/*****************************************************************************/
//  Description : 绘制边框
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayBorder (
    CTRLBASEPICKER_OBJ_T  *basepicker_ctrl_ptr,   // [in]
    GUI_RECT_T            *client_rect_ptr,       // [in]
    GUI_RECT_T            *img_rect_ptr           // [in]
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    GUIRES_DisplayImg (
        PNULL,
        client_rect_ptr,
        img_rect_ptr,
        base_ctrl_ptr->win_handle,
        basepicker_ctrl_ptr->theme.border_image_id,
        (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info);
}

/*****************************************************************************/
//  Description : 显示文本
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayText (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T      field,                  // [in]
    GUI_RECT_T                  *disp_rect_ptr          // [in]
)
{
    uint16          string_width = 0;
    int32           i = 0;
    GUI_POINT_T     start_point = {0};
    GUI_RECT_T      field_rect = {0};
    GUI_RECT_T      tips_rect = {0};
    GUISTR_STYLE_T  str_style = {0};
    GUISTR_STATE_T  str_state = 0;
    BASEPICKER_FIELD_INFO_T  field_info[BASEPICKER_MAX_DISP_COUNT] = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    // 获取显示信息
    GetBasePickerInfo (basepicker_ctrl_ptr, field, field_info, BASEPICKER_MAX_DISP_COUNT);

    //set string style
    str_style.align      = ALIGN_HVMIDDLE;
    str_style.char_space = basepicker_ctrl_ptr->theme.char_space;

    str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE;

    for (i = 0; i < BASEPICKER_MAX_DISP_COUNT; i++)
    {
        // 显示的字符串，放在这里，用于每次都初始化
        wchar           display_str[CTRLBASEPICKER_MAX_STR_LEN] = {0};
        MMI_STRING_T    dis_string = {0};

        dis_string.wstr_ptr = (wchar*) display_str;

        VTLBASEPICKER_GetFieldText (basepicker_ctrl_ptr, field, field_info[i].value, &dis_string);

        str_style.font_color = field_info[i].font.color;
        str_style.font = field_info[i].font.font;

        field_rect = field_info[i].disp_rect;

        if (CTRLBASEPICKER_TIPS_POS_MIDDLE == basepicker_ctrl_ptr->tips_pos)
        {
            field_rect.left -= BASEPICKER_TIPS_OFFSET;
            field_rect.right -= BASEPICKER_TIPS_OFFSET;
            //field_rect.right = (field_rect.right + field_rect.left) / 2 - BASEPICKER_TIPS_SPACE;
        }

        GUISTR_DrawTextToLCDInRect (
            (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info,
            (const GUI_RECT_T*) &field_rect,
            (const GUI_RECT_T*) disp_rect_ptr,
            &dis_string,
            &str_style,
            str_state,
            GUISTR_TEXT_DIR_AUTO);

        if (0 == string_width
                && CTRLBASEPICKER_TIPS_POS_MIDDLE == basepicker_ctrl_ptr->tips_pos)
        {
            string_width = GUI_GetStringWidthEx (
                               basepicker_ctrl_ptr->theme.cur_content_font.font,
                               dis_string.wstr_ptr, dis_string.wstr_len,
                               str_style.char_space);
        }
    }

    // middle tips
    if (CTRLBASEPICKER_TIPS_POS_MIDDLE == basepicker_ctrl_ptr->tips_pos)
    {
        //set rect
        GetStartPoint (basepicker_ctrl_ptr, &start_point);

        tips_rect = VTLBASEPICKER_GetFieldRect (basepicker_ctrl_ptr, field);
        TranslateRect (&tips_rect, start_point.x, start_point.y);

        tips_rect.left = (tips_rect.left + tips_rect.right + string_width) / 2 + BASEPICKER_TIPS_SPACE - BASEPICKER_TIPS_OFFSET;

        BasePickerDisplayTips (basepicker_ctrl_ptr, field, &tips_rect, disp_rect_ptr);
    }
}

/*****************************************************************************/
//  Description : 绘制分隔条
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplaySeparator (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    GUI_RECT_T             *separator_rect_ptr,    // [in]
    GUI_RECT_T             *relative_rect_ptr,     // [in]
    GUI_POINT_T            *start_pt_ptr,          // [in]
    MMI_IMAGE_ID_T         separator_image_id      // [in]
)
{
    GUI_RECT_T      img_rect = {0};         // 显示的位置
    GUI_RECT_T      sep_img_rect = {0};     // 图片中的位置
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    if (GUI_IntersectRect (&img_rect, *separator_rect_ptr, *relative_rect_ptr))
    {
        sep_img_rect = img_rect;

        TranslateRect (&img_rect, start_pt_ptr->x, start_pt_ptr->y);
        TranslateRect (&sep_img_rect, -separator_rect_ptr->left, -separator_rect_ptr->top);

        GUIRES_DisplayImg (
            PNULL,
            &img_rect,
            &sep_img_rect,
            base_ctrl_ptr->win_handle,
            separator_image_id,
            (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info);
    }
}

/*****************************************************************************/
//  Description : 绘制蒙板
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayMask (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,     // [in]
    GUI_RECT_T              *dis_rect_ptr
)
{
    BOOLEAN     result = FALSE;
    uint16      image_height = 0;
    GUI_RECT_T  mask_rect = {0};
    GUI_RECT_T  dst_mask_rect = {0};
    GUI_RECT_T  relative_dis_rect = {0};
    GUI_POINT_T start_point = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    // 获取显示的起点位置，及图片的宽高信息
    result = GetStartPoint (basepicker_ctrl_ptr, &start_point);

    if (!result)
    {
        return;
    }

    mask_rect = BASEPICKER_GetTextRect (basepicker_ctrl_ptr);

    // PICKER风格用item的高度，其他用图片
    if (GUI_BG_IMG == basepicker_ctrl_ptr->theme.mask_bg.bg_type
            && MMITHEME_CheckImageID (basepicker_ctrl_ptr->theme.mask_bg.img_id))
    {
        GUIRES_GetImgWidthHeight (
            PNULL, &image_height,
            basepicker_ctrl_ptr->theme.mask_bg.img_id,
            base_ctrl_ptr->win_handle);
    }
    else
    {
        image_height = GetItemHeight (basepicker_ctrl_ptr);
    }

    mask_rect.top = (mask_rect.top + mask_rect.bottom - image_height + 1) / 2;
    mask_rect.bottom = mask_rect.top + image_height - 1;

    // 图片的区域(转化为绝对位置)
    mask_rect.left += start_point.x;
    mask_rect.right += start_point.x;
    mask_rect.top += start_point.y;
    mask_rect.bottom += start_point.y;

    dst_mask_rect = mask_rect;

    // 能够显示的情况
    if (GUI_IntersectRect (&dst_mask_rect, dst_mask_rect, *dis_rect_ptr))
    {
        if (GUI_BG_IMG == basepicker_ctrl_ptr->theme.mask_bg.bg_type
                && MMITHEME_CheckImageID (basepicker_ctrl_ptr->theme.mask_bg.img_id))
        {
            relative_dis_rect.left = dst_mask_rect.left - mask_rect.left;
            relative_dis_rect.right = dst_mask_rect.right - mask_rect.left;
            relative_dis_rect.top = dst_mask_rect.top - mask_rect.top;
            relative_dis_rect.bottom = dst_mask_rect.bottom - mask_rect.top;

            // 画上面的蒙层
            GUIRES_DisplayImg (
                PNULL,
                &dst_mask_rect,
                &relative_dis_rect,
                base_ctrl_ptr->win_handle,
                basepicker_ctrl_ptr->theme.mask_bg.img_id,
                (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info);
        }
        else
        {
            GUI_FillRect (
                (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info,
                dst_mask_rect,
                basepicker_ctrl_ptr->theme.mask_bg.color);
        }
    }
}

/*****************************************************************************/
//  Description : 绘制提示文本
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void BasePickerDisplayTips (
    CTRLBASEPICKER_OBJ_T            *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T          field,                  // [in]
    GUI_RECT_T                      *dis_rect_ptr,          // [in]
    GUI_RECT_T                      *clip_rect_ptr          // [in]
)
{
    GUISTR_STYLE_T  str_style = {0};
    GUISTR_STATE_T  str_state = 0;
    MMI_TEXT_ID_T   tips_text_id = {0};
    MMI_STRING_T    tips_string = {0};
    GUI_RECT_T      img_rect = {0};
    uint16          img_height = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    // 提示文字背景
    if (CTRLBASEPICKER_TIPS_POS_TOP == basepicker_ctrl_ptr->tips_pos)
    {
        img_rect.right = clip_rect_ptr->right - clip_rect_ptr->left;
        img_rect.bottom = clip_rect_ptr->bottom - clip_rect_ptr->top;

        GUIRES_DisplayImg (
            PNULL,
            clip_rect_ptr,
            &img_rect,
            base_ctrl_ptr->win_handle,
            basepicker_ctrl_ptr->theme.tips_image_id,
            (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info);
    }

    tips_text_id = VTLBASEPICKER_GetFieldTips (basepicker_ctrl_ptr, field);

    if (0 != tips_text_id)
    {
        if (CTRLBASEPICKER_TIPS_POS_TOP == basepicker_ctrl_ptr->tips_pos)
        {
            // 根据背景的高度
            GUIRES_GetImgWidthHeight (
                PNULL, &img_height,
                basepicker_ctrl_ptr->theme.tips_image_id,
                base_ctrl_ptr->win_handle);

            dis_rect_ptr->bottom = dis_rect_ptr->top + img_height - 1;

            str_style.align      = ALIGN_HVMIDDLE;
            str_style.font_color = basepicker_ctrl_ptr->theme.tips_font.color;
            str_style.font       = basepicker_ctrl_ptr->theme.tips_font.font;
        }
        else
        {
            str_style.align      = ALIGN_LVMIDDLE;
            str_style.font_color = basepicker_ctrl_ptr->theme.tips_font_middle.color;
            str_style.font       = basepicker_ctrl_ptr->theme.tips_font_middle.font;
        }

        str_style.char_space = basepicker_ctrl_ptr->theme.char_space;

        str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN_BY_DIR;

        MMITHEME_GetLabelTextByLang (tips_text_id, &tips_string);

        GUISTR_DrawTextToLCDInRect (
            (const GUI_LCD_DEV_INFO*) &base_ctrl_ptr->lcd_dev_info,
            (const GUI_RECT_T*) dis_rect_ptr,
            (const GUI_RECT_T*) clip_rect_ptr,
            &tips_string,
            &str_style,
            str_state,
            GUISTR_TEXT_DIR_AUTO);
    }
}

/*****************************************************************************/
//  Description : 设置偏移位置，并自动更新界面
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetBasePickerOffset (
    CTRLBASEPICKER_OBJ_T           *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T         field,                  // [in]
    int32                          offset                  // [in] 与当前值的偏移量
)
{
    BOOLEAN         is_handle = TRUE;
    uint16          min_value = 0;
    uint16          max_value = 0;
    uint16          item_height = 0;
    int32           max_offset = 0;
    int32           old_offset = 0;
    GUI_POINT_T     start_point = {0};
    GUI_RECT_T      cross_rect = {0};
    GUI_RECT_T      field_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    if (0 == offset)
    {
        return FALSE;
    }

    item_height = GetItemHeight (basepicker_ctrl_ptr);

    VTLBASEPICKER_GetValueRange (basepicker_ctrl_ptr, field, &min_value, &max_value);

    // 最大的长度
    max_offset = (max_value - min_value + 1) * item_height;

    // 设置偏移量
    old_offset = basepicker_ctrl_ptr->state[field].cur_offset;
    basepicker_ctrl_ptr->state[field].cur_offset += offset;

    // 分循环与单向两种情况
    //  循环时，超过最值，变为另一端
    //  单向时，超过最值，变为最值
    if (basepicker_ctrl_ptr->state[field].is_loop)
    {
        if (basepicker_ctrl_ptr->state[field].cur_offset < 0)
        {
            basepicker_ctrl_ptr->state[field].cur_offset += max_offset;
        }
        else if (basepicker_ctrl_ptr->state[field].cur_offset >= max_offset)
        {
            basepicker_ctrl_ptr->state[field].cur_offset -= max_offset;
        }

        if (basepicker_ctrl_ptr->state[field].cur_offset < 0
                || basepicker_ctrl_ptr->state[field].cur_offset >= max_offset)
        {
            basepicker_ctrl_ptr->state[field].cur_offset = 0;
        }
    }
    else
    {
        if (basepicker_ctrl_ptr->state[field].cur_offset < 0)
        {
            basepicker_ctrl_ptr->state[field].cur_offset = 0;
            is_handle = FALSE;
        }
        else if (basepicker_ctrl_ptr->state[field].cur_offset >= max_offset - item_height)
        {
            basepicker_ctrl_ptr->state[field].cur_offset = max_offset - item_height;
            is_handle = FALSE;
        }
    }

    // 检查值的有效性
    CheckValid (basepicker_ctrl_ptr, field);

    // 绘制
    if (old_offset != basepicker_ctrl_ptr->state[field].cur_offset
            && GUI_IntersectRect (&cross_rect, base_ctrl_ptr->display_rect, base_ctrl_ptr->rect))
    {
        GetStartPoint (basepicker_ctrl_ptr, &start_point);

        field_rect = VTLBASEPICKER_GetFieldRect (basepicker_ctrl_ptr, field);
        TranslateRect (&field_rect, start_point.x, start_point.y);

        if (GUI_IntersectRect (&field_rect, cross_rect, field_rect))
        {
            BasePickerDisplayField (basepicker_ctrl_ptr, field, &field_rect);
        }
    }

    return is_handle;
}

/*****************************************************************************/
//  Description : 检查域里面的值是否合法
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void CheckValid (
    CTRLBASEPICKER_OBJ_T          *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T        field                   // [in]
)
{
    uint16  cur_value = 0;
    uint16  min_value = 0;
    uint16  max_value = 0;

    VTLBASEPICKER_GetValueRange (basepicker_ctrl_ptr, field, &min_value, &max_value);

    cur_value = BASEPICKER_GetValue (basepicker_ctrl_ptr, field);

    SCI_ASSERT (cur_value >= min_value && cur_value <= max_value);      /*assert verified*/
}

/*****************************************************************************/
//  Description : 根据对齐方式，获取显示的起点位置
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetStartPoint (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,   // [in]
    GUI_POINT_T             *start_point_ptr        // [out]
)
{
    uint16          client_width = 0;
    uint16          client_height = 0;
    GUI_ALIGN_E     align = ALIGN_HVMIDDLE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != start_point_ptr);       /*assert verified*/
    if (PNULL == start_point_ptr)
    {
        return FALSE;
    }

    GetBasePickerSize (basepicker_ctrl_ptr, &client_width, &client_height);

    align = GetAlign (basepicker_ctrl_ptr);

    if (ALIGN_LVMIDDLE == align)
    {
        start_point_ptr->x = base_ctrl_ptr->rect.left;
    }
    else if (ALIGN_RVMIDDLE == align)
    {
        start_point_ptr->x = base_ctrl_ptr->rect.right - (int16) client_width;
    }
    else
    {
        start_point_ptr->x = (base_ctrl_ptr->rect.left + base_ctrl_ptr->rect.right + 1 - (int16) client_width) / 2;
    }

    start_point_ptr->y = (base_ctrl_ptr->rect.top + base_ctrl_ptr->rect.bottom + 1 - (int16) client_height) / 2;

    return TRUE;
}

/*****************************************************************************/
//  Description : 设置宽度
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void SetBasePickerSize (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,         // [in]
    uint16                 width,                  // [in]
    uint16                 height                  // [in]
)
{
    if (0 != width)
    {
        basepicker_ctrl_ptr->theme.client_width = width;
    }

    if (0 != height)
    {
        basepicker_ctrl_ptr->theme.client_height = height;
    }
}

/*****************************************************************************/
//  Description : 获取宽度
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void GetBasePickerSize (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    uint16                 *width_ptr,             // [out]
    uint16                 *height_ptr             // [out]
)
{
    if (PNULL != width_ptr)
    {
        *width_ptr = basepicker_ctrl_ptr->theme.client_width;
    }

    // 高度为所给背景图的高度
    if (PNULL != height_ptr)
    {
        *height_ptr = basepicker_ctrl_ptr->theme.client_height;
    }
}

/*****************************************************************************/
//  Description : 获取行距
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TranslateRect (
    GUI_RECT_T     *rect_ptr,      // [in/out]
    int16          x,              // [in]
    int16          y               // [in]
)
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != rect_ptr);      /*assert verified*/
    if (PNULL != rect_ptr)
    {
        rect_ptr->left += x;
        rect_ptr->right += x;
        rect_ptr->top += y;
        rect_ptr->bottom += y;
    }
}

/*****************************************************************************/
//  Description : 获取位置和内容信息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void GetBasePickerInfo (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,     // [in]
    CTRLBASEPICKER_FIELD_T      field,              // [in]
    BASEPICKER_FIELD_INFO_T     *field_info_ptr,    // [out] 返回的区域是绝对区域
    int32                       field_info_count    // [in]
)
{
    uint16      item_height = 0;        // 每个item高度
    uint16      date_value = 0;         // 当前正在计算的item的值
    uint16      cur_date_value = 0;     // 中间item的值
    int32       i = 0;
    int32       start_pos = 0;          // 中间item的起始位置
    int32       cur_pos = 0;            // 当前正在计算的item的位置
    int32       middle_pos = 0;         // 中间位置
    int32       cur_index = 0;          // 在field_info_ptr的偏移
    GUI_POINT_T start_point = {0};
    GUI_RECT_T  field_rect = {0};       // 显示区域

    //set rect
    GetStartPoint (basepicker_ctrl_ptr, &start_point);

    field_rect = VTLBASEPICKER_GetFieldRect (basepicker_ctrl_ptr, field);
    TranslateRect (&field_rect, start_point.x, start_point.y);

    // get current date value
    cur_date_value = BASEPICKER_GetValue (basepicker_ctrl_ptr, field);
    date_value = cur_date_value;

    // height of a item
    item_height = GetItemHeight (basepicker_ctrl_ptr);

    // start position of the current item
    middle_pos = (field_rect.bottom + field_rect.top + 1) / 2;
    start_pos = (field_rect.bottom + field_rect.top + 1 - item_height) / 2 + 1 - basepicker_ctrl_ptr->state[field].cur_offset % item_height;
    cur_pos = start_pos;

    //SCI_TRACE_LOW:"== GetBasePickerInfo == y: %d, value: %d"
    SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_TOUCH_1820_112_2_18_3_16_25_142, (uint8*) "dd", start_pos, cur_date_value);

    // 找当前值下面的
    for (i = 0; i < field_info_count; i++)
    {
        if (cur_pos > field_rect.bottom)
        {
            break;
        }
        else
        {
            // 该值的显示区域
            field_info_ptr[cur_index].disp_rect = field_rect;
            field_info_ptr[cur_index].disp_rect.top = cur_pos;
            field_info_ptr[cur_index].disp_rect.bottom = cur_pos + item_height - 1;

            // 值
            field_info_ptr[cur_index].value = date_value;

            // 获取颜色信息
            if (field_info_ptr[cur_index].disp_rect.top <= middle_pos
                    && field_info_ptr[cur_index].disp_rect.bottom > middle_pos)
            {
                field_info_ptr[cur_index].font = basepicker_ctrl_ptr->theme.cur_content_font;
            }
            else
            {
                field_info_ptr[cur_index].font = basepicker_ctrl_ptr->theme.content_font;
            }

            cur_index++;
            cur_pos += item_height;

            if (!GetNextValue (basepicker_ctrl_ptr, field, date_value, TRUE, &date_value))
            {
                break;
            }
        }
    }

    // 找当前值上面的
    if (GetNextValue (basepicker_ctrl_ptr, field, cur_date_value, FALSE, &date_value))
    {
        cur_pos = start_pos - 1;

        for (i = cur_index; i < field_info_count; i++)
        {
            if (cur_pos < field_rect.top)
            {
                break;
            }
            else
            {
                field_info_ptr[cur_index].disp_rect = field_rect;
                field_info_ptr[cur_index].disp_rect.bottom = cur_pos;
                field_info_ptr[cur_index].disp_rect.top = cur_pos - item_height + 1;

                field_info_ptr[cur_index].value = date_value;

                // 获取颜色信息
                if (field_info_ptr[cur_index].disp_rect.top <= middle_pos
                        && field_info_ptr[cur_index].disp_rect.bottom > middle_pos)
                {
                    field_info_ptr[cur_index].font = basepicker_ctrl_ptr->theme.cur_content_font;
                }
                else
                {
                    field_info_ptr[cur_index].font = basepicker_ctrl_ptr->theme.content_font;
                }

                cur_index++;
                cur_pos -= item_height;

                if (!GetNextValue (basepicker_ctrl_ptr, field, date_value, FALSE, &date_value))
                {
                    break;
                }
            }
        }
    }
}

/*****************************************************************************/
//  Description : 根据当前值，获取上一个，或下一个值
//  Global resource dependence :
//  Author: hua.fang
//  Note:
//  返回是否成功，如果是单向的，到头之后返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN GetNextValue (
    CTRLBASEPICKER_OBJ_T         *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T       field,                  // [in]
    uint16                       cur_date_value,         // [in]
    BOOLEAN                      is_next,                // [in]
    uint16                       *next_value_ptr         // [out]
)
{
    BOOLEAN is_succeeded = TRUE;
    uint16 min_value = 0;
    uint16 max_value = 0;

    VTLBASEPICKER_GetValueRange (basepicker_ctrl_ptr, field, &min_value, &max_value);

    if (is_next)
    {
        if (cur_date_value >= max_value)
        {
            if (basepicker_ctrl_ptr->state[field].is_loop)
            {
                *next_value_ptr = min_value;
            }
            else
            {
                is_succeeded = FALSE;
            }
        }
        else
        {
            *next_value_ptr = cur_date_value + 1;
        }
    }
    else
    {
        if (cur_date_value <= min_value)
        {
            if (basepicker_ctrl_ptr->state[field].is_loop)
            {
                *next_value_ptr = max_value;
            }
            else
            {
                is_succeeded = FALSE;
            }
        }
        else
        {
            *next_value_ptr = cur_date_value - 1;
        }
    }

    return is_succeeded;
}

/*****************************************************************************/
//  Description : 发送选中消息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
__inline LOCAL void PostPickerSelectedMsg (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr     // [in]
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) basepicker_ctrl_ptr;

    // picker风格，且已经停止的情况下发消息
    if (0 == basepicker_ctrl_ptr->recover_timer_id
            && 0 == basepicker_ctrl_ptr->fling_timer_id
            && 0 == basepicker_ctrl_ptr->tpscroll_timer_id)
    {
        CTRLMSG_PostNotify (base_ctrl_ptr->handle, MSG_NOTIFY_PICKER_SELECTED);
    }
}

/*****************************************************************************/
//  Description : 获取item的高度
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
__inline LOCAL uint16 GetItemHeight (
    CTRLBASEPICKER_OBJ_T *basepicker_ctrl_ptr  // [in]
)
{
    return basepicker_ctrl_ptr->theme.line_height;
}

/*****************************************************************************/
//  Description : get align
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
__inline LOCAL GUI_ALIGN_E GetAlign (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr
)
{
    return basepicker_ctrl_ptr->theme.align;
}

/*****************************************************************************/
//  Description : 根据DOWN时的点，判断是否移动了
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
__inline LOCAL BOOLEAN IsTpMoved (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    GUI_POINT_T            *point_ptr              // [in]
)
{
    if (abs (point_ptr->x - basepicker_ctrl_ptr->tp_start_point.x) > BASEPICKER_TP_MOVE_DIS
            || abs (point_ptr->y - basepicker_ctrl_ptr->tp_start_point.y) > BASEPICKER_TP_MOVE_DIS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : get basepicker pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLBASEPICKER_OBJ_T* GetBasePickerPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT (BasePickerTypeOf (ctrl_ptr)); /*assert verified*/
    }

    return (CTRLBASEPICKER_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN BasePickerTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf ( (OBJECT_TYPE_PTR) ctrl_ptr, CTRL_BASEPICKER_TYPE);
}

#endif

