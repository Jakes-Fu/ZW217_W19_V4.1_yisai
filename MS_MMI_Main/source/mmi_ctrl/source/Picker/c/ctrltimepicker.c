/*****************************************************************************
** File Name:      ctrltimepicker.c                                               *
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
#include "guires.h"
#include "mmi_theme.h"

#include "ctrltimepicker.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

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
//  Description : init timepicker class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TimepickerCtrlInitVtbl (
    CTRLTIMEPICKER_VTBL_T        *timepicker_vtbl_ptr
);

/*****************************************************************************/
//  Description : timepicker construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TimepickerCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLTIMEPICKER_INIT_PARAM_T   *timepicker_param_ptr
);

/*****************************************************************************/
//  Description : timepicker destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TimepickerCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TimepickerCtrlPackInitParam (
    GUITIMEPICKER_INIT_DATA_T     *timepicker_init_ptr,   //in
    CTRLTIMEPICKER_INIT_PARAM_T   *timepicker_param_ptr   //out
);

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : get timepicker pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLTIMEPICKER_OBJ_T* GetTimePickerPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN TimePickerTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
//  Description : handle timepicker msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E TimePickerCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : 获取开始域与结束域(不包含该值)
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TimePickerCtrlGetFieldRange (
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
LOCAL void TimePickerCtrlGetValueRange (
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
LOCAL GUI_RECT_T TimePickerCtrlGetFieldRect (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,   // [in]
    int32                   field                   // [in]
);

/*****************************************************************************/
//  Description : 获取域中的文字
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TimePickerCtrlGetFieldText (
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
LOCAL MMI_TEXT_ID_T TimePickerCtrlGetFieldTips (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    int32                  field                   // [in]
);

/*****************************************************************************/
//  Description : convert 24 hour to 12 hour
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Conver24HourTo12Hour (//is am
    uint8    *hour_ptr   //in/out
);

/*****************************************************************************/
//  Description : convert 12 hour to 24 hour
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint8 Conver12HourTo24Hour (
    BOOLEAN    is_am,  //in
    uint8      hour  //in
);

/*****************************************************************************/
//  Description : set time
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TimePickerSetTime (
    CTRLTIMEPICKER_OBJ_T    *timepicker_ctrl_ptr,       // [in]
    uint8                   hour,                       // [in]
    uint8                   minute,                     // [in]
    uint8                   second                      // [in]
);

/*****************************************************************************/
//  Description : get time
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TimePickerGetTime (
    CTRLTIMEPICKER_OBJ_T    *timepicker_ctrl_ptr,       // [in]
    uint8                   *hour_ptr,                  // [out] may PNULL
    uint8                   *minute_ptr,                // [out] may PNULL
    uint8                   *second_ptr                 // [out] may PNULL
);
#endif

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get timepicker type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T TIMEPICKER_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASEPICKER_TYPE,                                 // parent class type
                       "timepicker",                                         // control name
                       sizeof (CTRLTIMEPICKER_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) TimepickerCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) TimepickerCtrlDestruct,      // control destructor function
                       sizeof (CTRLTIMEPICKER_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) TimepickerCtrlPackInitParam,     // convert init param
                       sizeof (CTRLTIMEPICKER_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) TimepickerCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init timepicker class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TimepickerCtrlInitVtbl (
    CTRLTIMEPICKER_VTBL_T        *timepicker_vtbl_ptr
)
{
#ifdef TOUCH_PANEL_SUPPORT
    CTRLBASE_VTBL_T         *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) timepicker_vtbl_ptr;
    CTRLBASEPICKER_VTBL_T   *basepicker_vtbl_ptr = (CTRLBASEPICKER_VTBL_T*) timepicker_vtbl_ptr;

    if (PNULL == basepicker_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = TimePickerCtrlHandleMsg;

    basepicker_vtbl_ptr->GetFieldRange = TimePickerCtrlGetFieldRange;
    basepicker_vtbl_ptr->GetValueRange = TimePickerCtrlGetValueRange;
    basepicker_vtbl_ptr->GetFieldRect = TimePickerCtrlGetFieldRect;
    basepicker_vtbl_ptr->GetFieldText = TimePickerCtrlGetFieldText;
    basepicker_vtbl_ptr->GetFieldTips = TimePickerCtrlGetFieldTips;
#endif

    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 该方法用于将控件的对外初始化参数结构转换为内部的初始化参数结构
// 由于控件有继承关系，控件初始化时又带有参数，所以内部的初始化参数也必须有继承关系
/*****************************************************************************/
LOCAL void TimepickerCtrlPackInitParam (
    GUITIMEPICKER_INIT_DATA_T     *timepicker_init_ptr,   //in
    CTRLTIMEPICKER_INIT_PARAM_T   *timepicker_param_ptr   //out
)
{
#ifdef TOUCH_PANEL_SUPPORT
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) timepicker_param_ptr;

    if (PNULL == timepicker_init_ptr || PNULL == timepicker_param_ptr)
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = timepicker_init_ptr->both_rect;
#endif
}

/*****************************************************************************/
//  Description : timepicker construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TimepickerCtrlConstruct (
    CTRLBASE_OBJ_T                  *base_ctrl_ptr,
    CTRLTIMEPICKER_INIT_PARAM_T     *timepicker_param_ptr
)
{
    BOOLEAN                 result = TRUE;

#ifdef TOUCH_PANEL_SUPPORT
    CTRLTIMEPICKER_OBJ_T    *timepicker_ctrl_ptr = (CTRLTIMEPICKER_OBJ_T*) base_ctrl_ptr;
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = (CTRLBASEPICKER_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr || PNULL == timepicker_param_ptr)
    {
        return FALSE;
    }

    MMITHEME_GetTimePickerTheme (&timepicker_ctrl_ptr->theme);

    // 时间风格下的12小时制，AMPM不需要循环
    if (CTRLTIMEPICKER_STYLE_12 == timepicker_ctrl_ptr->theme.time_style)
    {
        basepicker_ctrl_ptr->state[MMITHEME_TIMEPICKER_FIELD_12H_AMPM].is_loop = FALSE;
    }

#endif

    return (result);
}

/*****************************************************************************/
//  Description : timepicker destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TimepickerCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN             result = TRUE;

    return (result);
}

/*****************************************************************************/
//  Description : set time style and display style
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTIMEPICKER_SetTimeStyle (
    MMI_CTRL_ID_T                     ctrl_id,            // [in]
    CTRLTIMEPICKER_STYLE_E            *time_style_ptr,    // [in] may PNULL
    CTRLTIMEPICKER_DISPLAY_STYLE_E    *display_style_ptr, // [in] may PNULL
    BOOLEAN                           is_update           // [in]
)
{
#ifdef TOUCH_PANEL_SUPPORT
    uint8                   hour = 0;
    uint8                   minute = 0;
    uint8                   second = 0;
    CTRLTIMEPICKER_OBJ_T    *timepicker_ctrl_ptr = PNULL;
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = PNULL;

    timepicker_ctrl_ptr = GetTimePickerPtr (ctrl_id);

    if (PNULL != timepicker_ctrl_ptr)
    {
        basepicker_ctrl_ptr = (CTRLBASEPICKER_OBJ_T*) timepicker_ctrl_ptr;

        // 有可能会12/24小时制切换，所以先保存下之前的时间，后面再重新恢复
        TimePickerGetTime (timepicker_ctrl_ptr, &hour, &minute, &second);

        // 销毁之前的信息，后续会重新创建
        // 必须在改变风格前销毁，因为需要依据field
        BASEPICKER_Destroy (basepicker_ctrl_ptr);

        if (PNULL != time_style_ptr)
        {
            timepicker_ctrl_ptr->theme.time_style = *time_style_ptr;
        }

        if (PNULL != display_style_ptr)
        {
            timepicker_ctrl_ptr->theme.display_style = *display_style_ptr;
        }

        BASEPICKER_Init (basepicker_ctrl_ptr);

        // 时间风格下的12小时制，AMPM不需要循环
        if (CTRLTIMEPICKER_STYLE_12 == timepicker_ctrl_ptr->theme.time_style)
        {
            basepicker_ctrl_ptr->state[MMITHEME_TIMEPICKER_FIELD_12H_AMPM].is_loop = FALSE;
        }

        TimePickerSetTime (timepicker_ctrl_ptr, hour, minute, second);

        if (is_update)
        {
            BASEPICKER_Display (basepicker_ctrl_ptr);
        }
    }

#endif
}

/*****************************************************************************/
//  Description : set time
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTIMEPICKER_SetTime (
    MMI_CTRL_ID_T      ctrl_id,            // [in]
    uint8              hour,               // [in]
    uint8              minute,             // [in]
    uint8              second              // [in]
)
{
#ifdef TOUCH_PANEL_SUPPORT
    CTRLTIMEPICKER_OBJ_T    *timepicker_ctrl_ptr = PNULL;

    timepicker_ctrl_ptr = GetTimePickerPtr (ctrl_id);

    if (PNULL != timepicker_ctrl_ptr)
    {
        TimePickerSetTime(timepicker_ctrl_ptr, hour, minute, second);
    }

#endif
}

/*****************************************************************************/
//  Description : get time
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTIMEPICKER_GetTime (
    MMI_CTRL_ID_T      ctrl_id,            // [in]
    uint8              *hour_ptr,          // [out] may PNULL
    uint8              *minute_ptr,        // [out] may PNULL
    uint8              *second_ptr         // [out] may PNULL
)
{
#ifdef TOUCH_PANEL_SUPPORT
    CTRLTIMEPICKER_OBJ_T    *timepicker_ctrl_ptr = PNULL;

    timepicker_ctrl_ptr = GetTimePickerPtr (ctrl_id);

    if (PNULL != timepicker_ctrl_ptr)
    {
        TimePickerGetTime(timepicker_ctrl_ptr, hour_ptr, minute_ptr, second_ptr);
    }

#endif
}

#ifdef TOUCH_PANEL_SUPPORT

/*****************************************************************************/
//  Description : handle timepicker msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E TimePickerCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    CTRLBASE_VTBL_T         *parent_vtbl_ptr = PNULL;

    parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl(CTRL_TIMEPICKER_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);

    return parent_vtbl_ptr->HandleEvent (base_ctrl_ptr, msg_id, param);
}

/*****************************************************************************/
//  Description : 获取开始域与结束域(不包含该值)
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TimePickerCtrlGetFieldRange (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    int32                  *start_field_ptr,       // [out]
    int32                  *end_field_ptr          // [out]
)
{
    CTRLTIMEPICKER_OBJ_T    *timepicker_ctrl_ptr = (CTRLTIMEPICKER_OBJ_T*) basepicker_ctrl_ptr;

    if (CTRLTIMEPICKER_STYLE_12 == timepicker_ctrl_ptr->theme.time_style)
    {
        *start_field_ptr = MMITHEME_TIMEPICKER_FIELD_12H_START;
        *end_field_ptr = MMITHEME_TIMEPICKER_FIELD_12H_END;
    }
    else if (CTRLTIMEPICKER_STYLE_TIMER == timepicker_ctrl_ptr->theme.time_style)
    {
        if (CTRLTIMEPICKER_DISPLAY_STYLE_HMS == timepicker_ctrl_ptr->theme.display_style)
        {
            *start_field_ptr = MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_START;
            *end_field_ptr = MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_END;
        }
        else
        {
            *start_field_ptr = MMITHEME_TIMEPICKER_FIELD_TIMER_START;
            *end_field_ptr = MMITHEME_TIMEPICKER_FIELD_TIMER_END;
        }
    }
    else
    {
        *start_field_ptr = MMITHEME_TIMEPICKER_FIELD_24H_START;
        *end_field_ptr = MMITHEME_TIMEPICKER_FIELD_24H_END;
    }

    SCI_ASSERT (*start_field_ptr <= MMITHEME_BASEPICKER_MAX_FIELD_COUNT     /*assert verified*/
                && *end_field_ptr <= MMITHEME_BASEPICKER_MAX_FIELD_COUNT);
}

/*****************************************************************************/
//  Description : 获取指定域的最值
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TimePickerCtrlGetValueRange (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,// [in]
    CTRLBASEPICKER_FIELD_T      field,               // [in]
    uint16                      *min_value_ptr,      // [out]
    uint16                      *max_value_ptr       // [out]
)
{
    switch (field)
    {
    case MMITHEME_TIMEPICKER_FIELD_24H_HOUR:
        *min_value_ptr = 0;
        *max_value_ptr = 23;
        break;

    case MMITHEME_TIMEPICKER_FIELD_24H_MINUTE:
        *min_value_ptr = 0;
        *max_value_ptr = 59;
        break;

    case MMITHEME_TIMEPICKER_FIELD_12H_HOUR:
        *min_value_ptr = 1;
        *max_value_ptr = 12;
        break;

    case MMITHEME_TIMEPICKER_FIELD_12H_MINUTE:
        *min_value_ptr = 0;
        *max_value_ptr = 59;
        break;

    case MMITHEME_TIMEPICKER_FIELD_12H_AMPM:
        *min_value_ptr = 0;
        *max_value_ptr = 1;
        break;

    case MMITHEME_TIMEPICKER_FIELD_TIMER_HOUR:
        *min_value_ptr = 0;
        *max_value_ptr = 23;
        break;

    case MMITHEME_TIMEPICKER_FIELD_TIMER_MINUTE:
        *min_value_ptr = 0;
        *max_value_ptr = 59;
        break;

    case MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_HOUR:
        *min_value_ptr = 0;
        *max_value_ptr = 23;
        break;

    case MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_MINUTE:
        *min_value_ptr = 0;
        *max_value_ptr = 59;
        break;

    case MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_SECOND:
        *min_value_ptr = 0;
        *max_value_ptr = 59;
        break;

    default:
        SCI_ASSERT (FALSE);     /*assert verified*/
        break;
    }
}

/*****************************************************************************/
//  Description : 获取指定域的相对显示位置
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T TimePickerCtrlGetFieldRect (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,   // [in]
    int32                   field                   // [in]
)
{
    uint16                  width = 0;
    GUI_RECT_T              field_rect = {0};
    int32                   i = 0;
    uint16                  pre_percent = 0;
    uint16                  cur_percent = 0;
    int32                   cur_field = 0;
    int32                   start_field = 0;
    int32                   end_field = 0;
    CTRLTIMEPICKER_OBJ_T    *timepicker_ctrl_ptr = (CTRLTIMEPICKER_OBJ_T*) basepicker_ctrl_ptr;
    int32 range[CTRLTIMEPICKER_STYLE_MAX][3] =        // 个数与CTRLTIMEPICKER_STYLE_E一致
    {
        {0, 0, 0},
        {MMITHEME_TIMEPICKER_FIELD_12H_HOUR, MMITHEME_TIMEPICKER_FIELD_12H_MINUTE, MMITHEME_TIMEPICKER_FIELD_12H_AMPM},
        {MMITHEME_TIMEPICKER_FIELD_24H_HOUR, MMITHEME_TIMEPICKER_FIELD_24H_MINUTE, 0},
        {MMITHEME_TIMEPICKER_FIELD_TIMER_HOUR, MMITHEME_TIMEPICKER_FIELD_TIMER_MINUTE, 0},
    };

    field_rect = BASEPICKER_GetTextRect (basepicker_ctrl_ptr);

    width = basepicker_ctrl_ptr->theme.client_width;

    // 边线 + 分隔条(加上最左和最右的)
    VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);
    width -= (basepicker_ctrl_ptr->theme.border_width * 2 + (end_field - start_field + 1) * basepicker_ctrl_ptr->theme.separator_width);

    if (CTRLTIMEPICKER_DISPLAY_STYLE_HMS == timepicker_ctrl_ptr->theme.display_style)
    {
        range[3][0] = MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_HOUR;
        range[3][1] = MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_MINUTE;
        range[3][2] = MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_SECOND;
    }

    for (i = 0; i < 3; i++)
    {
        cur_field = range[timepicker_ctrl_ptr->theme.time_style][i];

        cur_percent += timepicker_ctrl_ptr->theme.field_width[cur_field];

        if (cur_field == field)
        {
            break;
        }

        pre_percent += timepicker_ctrl_ptr->theme.field_width[cur_field];
    }

    field_rect.left += width * pre_percent / 100 + i * basepicker_ctrl_ptr->theme.separator_width;
    field_rect.right = field_rect.left + width * cur_percent / 100 - width * pre_percent / 100 - 1;     // 不能合并，否则有误差

    return field_rect;
}

/*****************************************************************************/
//  Description : 获取域中的文字
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TimePickerCtrlGetFieldText (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T      field,                  // [in]
    uint16                      cur_index,              // [in]
    MMI_STRING_T                *dis_string_ptr         // [out]
)
{
    char                    date_str[CTRLBASEPICKER_MAX_STR_LEN] = {0};
    wchar                   ampm[2][3] = {{'A', 'M', 0}, {'P', 'M', 0}};
    CTRLTIMEPICKER_OBJ_T    *timepicker_ctrl_ptr = (CTRLTIMEPICKER_OBJ_T*) basepicker_ctrl_ptr;

    //set string
    if (CTRLTIMEPICKER_STYLE_12 == timepicker_ctrl_ptr->theme.time_style
            && MMITHEME_TIMEPICKER_FIELD_12H_AMPM == field)
    {
        // 12小时制的AMPM字符串做特殊处理
        if (cur_index <= 1)
        {
            MMIAPICOM_Wstrncpy (dis_string_ptr->wstr_ptr, ampm[cur_index], 2);
            dis_string_ptr->wstr_len = 2;
        }
    }
    else
    {
        // 变为2位数，前面补0
        sprintf (date_str, "%02d", cur_index);

        dis_string_ptr->wstr_len = strlen (date_str);

        MMI_STRNTOWSTR (
            dis_string_ptr->wstr_ptr,
            CTRLBASEPICKER_MAX_STR_LEN - 1,
            (const uint8*) date_str,
            dis_string_ptr->wstr_len,
            dis_string_ptr->wstr_len);
    }
}

/*****************************************************************************/
//  Description : 获取提示文本
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T TimePickerCtrlGetFieldTips (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    int32                  field                   // [in]
)
{
    MMI_TEXT_ID_T           text_id = 0;
    CTRLTIMEPICKER_OBJ_T    *timepicker_ctrl_ptr = (CTRLTIMEPICKER_OBJ_T*) basepicker_ctrl_ptr;

    if (MMITHEME_TIMEPICKER_FIELD_COUNT > field)
    {
        text_id = timepicker_ctrl_ptr->theme.field_tips[field];
    }

    return text_id;
}

/*****************************************************************************/
//  Description : convert 24 hour to 12 hour
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Conver24HourTo12Hour (//is am
    uint8    *hour_ptr   //in/out
)
{
    BOOLEAN     result = FALSE;

    if ( (12 > *hour_ptr)) //AM
    {
        result = TRUE;

        if ( (1 > *hour_ptr)) //00:00-1:00
        {
            *hour_ptr = (uint8) (*hour_ptr + 12);
        }
    }
    else//PM
    {
        if ( (12 <= *hour_ptr) && (13 > *hour_ptr)) //12:00-13:00
        {
        }
        else
        {
            *hour_ptr = (uint8) (*hour_ptr - 12);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : convert 12 hour to 24 hour
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint8 Conver12HourTo24Hour (
    BOOLEAN    is_am,  //in
    uint8      hour  //in
)
{
    uint8   new_hour = 0;

    new_hour = hour;

    if (is_am)
    {
        if (12 <= new_hour)
        {
            new_hour = (uint8) (new_hour - 12);
        }
    }
    else
    {
        if (12 > new_hour)
        {
            new_hour = (uint8) (new_hour + 12);
        }
    }

    return (new_hour);
}

/*****************************************************************************/
//  Description : set time
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TimePickerSetTime (
    CTRLTIMEPICKER_OBJ_T    *timepicker_ctrl_ptr,       // [in]
    uint8                   hour,                       // [in]
    uint8                   minute,                     // [in]
    uint8                   second                      // [in]
)
{
    BOOLEAN                 is_am = FALSE;
    int32                   i = 0;
    int32                   start_field = 0;
    int32                   end_field   = 0;
    uint8                   value[3] = {0};
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = PNULL;

    if (PNULL != timepicker_ctrl_ptr)
    {
        basepicker_ctrl_ptr = (CTRLBASEPICKER_OBJ_T*) timepicker_ctrl_ptr;

        value[0] = hour;
        value[1] = minute;
        value[2] = second;

        if (CTRLTIMEPICKER_STYLE_12 == timepicker_ctrl_ptr->theme.time_style)
        {
            is_am = Conver24HourTo12Hour (&value[0]);

            value[2] = (is_am ? 0 : 1);
        }

        // 获取当前风格下，域的开始和结束范围，所有风格作统一处理
        VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

        for (i = start_field; i < end_field; i++)
        {
            if (basepicker_ctrl_ptr->enable_anim)
            {
                BASEPICKER_SetRecoverValue (basepicker_ctrl_ptr, i, value[i - start_field], FALSE);
            }
            else
            {
                BASEPICKER_Reset (basepicker_ctrl_ptr);
                BASEPICKER_SetValue (basepicker_ctrl_ptr, i, value[i - start_field]);
            }
        }
    }
}

/*****************************************************************************/
//  Description : get time
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TimePickerGetTime (
    CTRLTIMEPICKER_OBJ_T    *timepicker_ctrl_ptr,       // [in]
    uint8                   *hour_ptr,                  // [out] may PNULL
    uint8                   *minute_ptr,                // [out] may PNULL
    uint8                   *second_ptr                 // [out] may PNULL
)
{
    BOOLEAN                 is_am = FALSE;
    int32                   ampm_value = 0;
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = PNULL;

    if (PNULL != timepicker_ctrl_ptr)
    {
        basepicker_ctrl_ptr = (CTRLBASEPICKER_OBJ_T*) timepicker_ctrl_ptr;

        if (CTRLTIMEPICKER_STYLE_12 == timepicker_ctrl_ptr->theme.time_style)
        {
            // 12小时制的小时，需要看AMPM域
            if (PNULL != hour_ptr)
            {
                *hour_ptr = (uint8) BASEPICKER_GetValue (basepicker_ctrl_ptr, MMITHEME_TIMEPICKER_FIELD_12H_HOUR);
                ampm_value = BASEPICKER_GetValue (basepicker_ctrl_ptr, MMITHEME_TIMEPICKER_FIELD_12H_AMPM);

                if (0 == ampm_value)
                {
                    is_am = TRUE;
                }

                *hour_ptr = Conver12HourTo24Hour (is_am, *hour_ptr);
            }

            if (PNULL != minute_ptr)
            {
                *minute_ptr = (uint8) BASEPICKER_GetValue (basepicker_ctrl_ptr, MMITHEME_TIMEPICKER_FIELD_12H_MINUTE);
            }
        }
        else if (CTRLTIMEPICKER_STYLE_TIMER == timepicker_ctrl_ptr->theme.time_style)
        {
            if (CTRLTIMEPICKER_DISPLAY_STYLE_HMS == timepicker_ctrl_ptr->theme.display_style)
            {
                if (PNULL != hour_ptr)
                {
                    *hour_ptr = (uint8) BASEPICKER_GetValue (basepicker_ctrl_ptr, MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_HOUR);
                }

                if (PNULL != minute_ptr)
                {
                    *minute_ptr = (uint8) BASEPICKER_GetValue (basepicker_ctrl_ptr, MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_MINUTE);
                }

                if (PNULL != second_ptr)
                {
                    *second_ptr = (uint8) BASEPICKER_GetValue (basepicker_ctrl_ptr, MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_SECOND);
                }
            }
            else
            {
                if (PNULL != hour_ptr)
                {
                    *hour_ptr = (uint8) BASEPICKER_GetValue (basepicker_ctrl_ptr, MMITHEME_TIMEPICKER_FIELD_TIMER_HOUR);
                }

                if (PNULL != minute_ptr)
                {
                    *minute_ptr = (uint8) BASEPICKER_GetValue (basepicker_ctrl_ptr, MMITHEME_TIMEPICKER_FIELD_TIMER_MINUTE);
                }
            }
        }
        else
        {
            if (PNULL != hour_ptr)
            {
                *hour_ptr = (uint8) BASEPICKER_GetValue (basepicker_ctrl_ptr, MMITHEME_TIMEPICKER_FIELD_24H_HOUR);
            }

            if (PNULL != minute_ptr)
            {
                *minute_ptr = (uint8) BASEPICKER_GetValue (basepicker_ctrl_ptr, MMITHEME_TIMEPICKER_FIELD_24H_MINUTE);
            }
        }
    }
}

/*****************************************************************************/
//  Description : get timepicker pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLTIMEPICKER_OBJ_T* GetTimePickerPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT (TimePickerTypeOf (ctrl_ptr)); /*assert verified*/
    }

    return (CTRLTIMEPICKER_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN TimePickerTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf ( (OBJECT_TYPE_PTR) ctrl_ptr, CTRL_TIMEPICKER_TYPE);
}
#endif

