/*****************************************************************************
** File Name:      ctrltimeedit.c                                               *
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

#include "ctrltimeedit.h"
#include "ctrltimeedit_export.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define GUIEDIT_TIME_MAX_LEN            8   //xx:xx PM
#define GUIEDIT_TIME_SUFFIX_LEN         3   // PM

#define GUIEDIT_TIME_SUFFIX_SEPARATOR   ' '

#define GUIEDIT_TIME_SUFFIX_A           'A'
#define GUIEDIT_TIME_SUFFIX_P           'P'
#define GUIEDIT_TIME_SUFFIX_M           'M'

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

//edit time bit
typedef enum
{
    GUIEDIT_TIME_BIT_NULL,
    GUIEDIT_TIME_BIT_H_TEN, //时-十位
    GUIEDIT_TIME_BIT_H_ONE, //时-个位
    GUIEDIT_TIME_BIT_M_TEN, //分-十位
    GUIEDIT_TIME_BIT_M_ONE, //分-个位
    GUIEDIT_TIME_BIT_S_TEN, //秒-十位
    GUIEDIT_TIME_BIT_S_ONE, //秒-个位
    GUIEDIT_TIME_BIT_AM_PM, //AM or PM
    GUIEDIT_TIME_BIT_MAX
} GUIEDIT_TIME_BIT_E;

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
//  Description : init timeedit class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TimeEditCtrlInitVtbl (
    CTRLTIMEEDIT_VTBL_T        *timeedit_vtbl_ptr
);

/*****************************************************************************/
//  Description : timeedit construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TimeEditCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLTIMEEDIT_INIT_PARAM_T   *timeedit_param_ptr
);

/*****************************************************************************/
//  Description : timeedit destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TimeEditCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TimeEditCtrlPackInitParam (
    GUIEDIT_INIT_DATA_T         *timeedit_init_ptr,   //in
    CTRLTIMEEDIT_INIT_PARAM_T   *timeedit_param_ptr   //out
);

/*****************************************************************************/
//  Description : handle timeedit msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E TimeEditCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : get timeedit pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLTIMEEDIT_OBJ_T* GetTimeEditPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN TimeEditTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
// 	Description : get align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E TimeEditCtrlGetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr   //in:
);

/*****************************************************************************/
// 	Description : set align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void TimeEditCtrlSetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr,  //in:
    GUI_ALIGN_E             align               //in
);

/*****************************************************************************/
// 	Description : get im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void TimeEditCtrlGetImInfo (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr, //in:
    BOOLEAN                *is_forbid_ptr,     //in/out:
    GUIIM_TYPE_T           *init_im_ptr,       //in/out:
    GUIIM_TYPE_T           *allow_im_ptr,      //in/out:
    GUIIM_INPUT_TYPE_T     *init_input_ptr,    //in/out:
    GUIIM_INPUT_TYPE_T     *allow_input_ptr,   //in/out:
    GUIIM_CAPITAL_MODE_T   *capital_mode_ptr,  //in/out:
    GUIIM_TAG_T            *tag_ptr            //in/out:
);

/*****************************************************************************/
// 	Description : set im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void TimeEditCtrlSetImInfo (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr, //in:
    BOOLEAN                *is_forbid_ptr,     //in:may PNULL
    GUIIM_TYPE_T           *init_im_ptr,       //in:may PNULL
    GUIIM_TYPE_T           *allow_im_ptr,      //in:may PNULL
    GUIIM_INPUT_TYPE_T     *init_type_ptr,     //in:may PNULL
    GUIIM_INPUT_TYPE_T     *allow_type_ptr,    //in:may PNULL
    GUIIM_CAPITAL_MODE_T   *capital_mode_ptr,  //in:may PNULL
    GUIIM_TAG_T            *tag_ptr
);

/*****************************************************************************/
//  Description : 设置横屏时的新edit
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TimeEditCtrlSetLandscapeEdit (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    MMI_HANDLE_T        landscape_edit          // [in] the dst landscape edit
);

/*****************************************************************************/
//  Description : 获取指定偏移的字符是否有效
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 该方法用于在TP消息点击后，判断是否点在有效的字符上
/*****************************************************************************/
LOCAL BOOLEAN TimeEditCtrlIsValidOffset (           // [ret] TRUE if char offset is valid
    CTRLBASEFIXED_OBJ_T     *basefixed_ctrl_ptr,    // [in] ctrl ptr
    uint16                  *char_offset_ptr,       // [in/out] char offset, and may change the offset if valid
    uint16                  *highlight_len_ptr      // [out] highlight length if char valid
);

/*****************************************************************************/
//  Description : get time string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void TimeEditCtrlConvertToStr (
    CTRLBASEFIXED_OBJ_T     *basefixed_ctrl_ptr
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
//  Description : handle time edit up/down key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTimeEditUpDownKey (
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr,
    BOOLEAN             is_add
);

/*****************************************************************************/
//  Description : handle time edit up/down/number key,get new time and time string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNewTime (//up/down:is change;number:is valid
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr,
    BOOLEAN            is_num,     //TRUE:number key:FALSE:up/down key
    BOOLEAN            is_add,
    uint8              num_value   //from 0 to 9
);

/*****************************************************************************/
//  Description : get current time bit
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIEDIT_TIME_BIT_E GetCurTimeBit (
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle time edit up/down/number key,get new time hour
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNewTimeHour (//up/down:is change;number:is valid
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr,
    BOOLEAN             is_num,     //TRUE:number key:FALSE:up/down key
    BOOLEAN             is_add,
    uint8               num_value,  //from 0 to 9
    GUIEDIT_TIME_BIT_E  time_bit
);

/*****************************************************************************/
//  Description : adjust minute or second when time is timer,must > 0
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AdjustMinOrSec (
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle time edit up/down/number key,get new time minute
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNewTimeMinute (//up/down:is change;number:is valid
    CTRLTIMEEDIT_OBJ_T      *timeedit_ctrl_ptr,
    BOOLEAN                 is_num,     //TRUE:number key:FALSE:up/down key
    BOOLEAN                 is_add,
    uint8                   num_value,  //from 0 to 9
    GUIEDIT_TIME_BIT_E      time_bit
);

/*****************************************************************************/
//  Description : handle time edit up/down/number key,get new time second
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNewTimeSecond (//up/down:is change;number:is valid
    CTRLTIMEEDIT_OBJ_T      *timeedit_ctrl_ptr,
    BOOLEAN                 is_num,     //TRUE:number key:FALSE:up/down key
    BOOLEAN                 is_add,
    uint8                   num_value,  //from 0 to 9
    GUIEDIT_TIME_BIT_E      time_bit
);

/*****************************************************************************/
//  Description : get new time suffix
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNewTimeSuffix (
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle time edit left key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTimeEditLeftKey (
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle time edit right key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTimeEditRightKey (
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : move time cursor to right
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MoveTimeCursorToRight (
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : time handle im commit msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleTimeImCommit (
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr,
    GUIEDIT_ADD_STR_T   *add_str_ptr
);

/*****************************************************************************/
//  Description : time handle am/pm
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTimeAmPm (
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr
);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get time
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTIMEEDIT_GetTime (
    MMI_CTRL_ID_T   ctrl_id,    //in
    uint8           *hour_ptr,  //in/out:may PNULL
    uint8           *minute_ptr,//in/out:may PNULL
    uint8           *second_ptr //in/out:may PNULL
)
{
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    timeedit_ctrl_ptr = GetTimeEditPtr (ctrl_id);

    if (PNULL != timeedit_ctrl_ptr)
    {
        //set year
        if (PNULL != hour_ptr)
        {
            *hour_ptr = timeedit_ctrl_ptr->hour;
        }

        //set month
        if (PNULL != minute_ptr)
        {
            *minute_ptr = timeedit_ctrl_ptr->minute;
        }

        //set day
        if (PNULL != second_ptr)
        {
            *second_ptr = timeedit_ctrl_ptr->second;
        }
    }
}

/*****************************************************************************/
//  Description : set time
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTIMEEDIT_SetTime (
    MMI_CTRL_ID_T   ctrl_id,
    uint8           hour,
    uint8           minute,
    uint8           second
)
{
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    timeedit_ctrl_ptr = GetTimeEditPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) timeedit_ctrl_ptr;

    if (PNULL != timeedit_ctrl_ptr)
    {
        //set time
        timeedit_ctrl_ptr->hour   = hour;
        timeedit_ctrl_ptr->minute = minute;
        timeedit_ctrl_ptr->second = second;

        //reset length,get new time string
        baseedit_ctrl_ptr->str_len = 0;
    }
}

/*****************************************************************************/
//  Description : set time style and display style
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLTIMEEDIT_SetTimeStyle (
    MMI_CTRL_ID_T                  ctrl_id,
    GUIEDIT_SEPARATOR_E            *separator_ptr,     //may PNULL
    GUIEDIT_TIME_STYLE_E           *time_style_ptr,    //may PNULL
    GUIEDIT_TIME_DISPLAY_STYLE_E   *display_style_ptr, //may PNULL
    BOOLEAN                        is_update
)
{
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    timeedit_ctrl_ptr = GetTimeEditPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) timeedit_ctrl_ptr;

    if (PNULL != timeedit_ctrl_ptr)
    {
        //set separator
        if (PNULL != separator_ptr)
        {
            timeedit_ctrl_ptr->theme.separator_style = *separator_ptr;
        }

        //set time style
        if (PNULL != time_style_ptr)
        {
            timeedit_ctrl_ptr->theme.time_style = *time_style_ptr;
        }

        if (PNULL != display_style_ptr)
        {
            timeedit_ctrl_ptr->theme.display_style = *display_style_ptr;
        }

        //reset length,get new date string
        SCI_MEMSET(baseedit_ctrl_ptr->str_ptr,0,((baseedit_ctrl_ptr->str_max_len +1)*sizeof(wchar)));
        baseedit_ctrl_ptr->str_len = 0;

        if (is_update)
        {
            //set cursor
            baseedit_ctrl_ptr->cursor.cur_pos      = 0;
            baseedit_ctrl_ptr->cursor.x_coordinate = GUIEDIT_CURSOR_INVALID_COORDINATE;

            //set highlight
            VTLBASEEDIT_SetHighlight (baseedit_ctrl_ptr, 0, 1, SETHIGHLIGHT_BOTH);

            //update
            VTLBASEEDIT_DisplayAll (baseedit_ctrl_ptr, FALSE);
        }
    }
}

/*****************************************************************************/
//  Description : get timeedit type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T TIMEEDIT_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASEFIXED_TYPE,                                // parent class type
                       "timeedit",                                         // control name
                       sizeof (CTRLTIMEEDIT_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) TimeEditCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) TimeEditCtrlDestruct,      // control destructor function
                       sizeof (CTRLTIMEEDIT_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) TimeEditCtrlPackInitParam,     // convert init param
                       sizeof (CTRLTIMEEDIT_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) TimeEditCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init timeedit class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TimeEditCtrlInitVtbl (
    CTRLTIMEEDIT_VTBL_T        *timeedit_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) timeedit_vtbl_ptr;
    CTRLBASEEDIT_VTBL_T *baseedit_ctrl_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*) timeedit_vtbl_ptr;
    CTRLBASEFIXED_VTBL_T *basefixed_ctrl_vtbl_ptr = (CTRLBASEFIXED_VTBL_T*) timeedit_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = TimeEditCtrlHandleMsg;

    baseedit_ctrl_vtbl_ptr->GetAlign = TimeEditCtrlGetAlign;
    baseedit_ctrl_vtbl_ptr->SetAlign = TimeEditCtrlSetAlign;
    baseedit_ctrl_vtbl_ptr->GetImInfo = TimeEditCtrlGetImInfo;
    baseedit_ctrl_vtbl_ptr->SetImInfo = TimeEditCtrlSetImInfo;
    baseedit_ctrl_vtbl_ptr->SetLandscapeEdit = TimeEditCtrlSetLandscapeEdit;

    basefixed_ctrl_vtbl_ptr->IsValidOffset = TimeEditCtrlIsValidOffset;
    basefixed_ctrl_vtbl_ptr->ConvertToStr = TimeEditCtrlConvertToStr;

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
LOCAL void TimeEditCtrlPackInitParam (
    GUIEDIT_INIT_DATA_T         *timeedit_init_ptr,   //in
    CTRLTIMEEDIT_INIT_PARAM_T   *timeedit_param_ptr   //out
)
{
    CTRLBASE_INIT_PARAM_T       *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) timeedit_param_ptr;
    CTRLBASEEDIT_INIT_PARAM_T   *baseedit_ctrl_param_ptr = (CTRLBASEEDIT_INIT_PARAM_T*) timeedit_param_ptr;

    if (PNULL == timeedit_init_ptr || PNULL == timeedit_param_ptr)
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = timeedit_init_ptr->both_rect;

    baseedit_ctrl_param_ptr->is_rect_adaptive = timeedit_init_ptr->is_rect_adaptive;
    baseedit_ctrl_param_ptr->str_max_len = timeedit_init_ptr->str_max_len;
    baseedit_ctrl_param_ptr->type = timeedit_init_ptr->type;
}

/*****************************************************************************/
//  Description : timeedit construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TimeEditCtrlConstruct (
    CTRLBASE_OBJ_T              *base_ctrl_ptr,
    CTRLTIMEEDIT_INIT_PARAM_T   *timeedit_param_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr = (CTRLTIMEEDIT_OBJ_T*) base_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr || PNULL == timeedit_param_ptr)
    {
        return FALSE;
    }

    MMITHEME_GetTimeEditTheme (&timeedit_ctrl_ptr->theme);

    //set edit time default is single
    baseedit_ctrl_ptr->style = GUIEDIT_STYLE_SINGLE;

    //set highlight
    VTLBASEEDIT_SetHighlight (
        baseedit_ctrl_ptr, 0,
        (uint16) (baseedit_ctrl_ptr->highlight.start_pos + 1),
        SETHIGHLIGHT_END);

    //hide cursor
    baseedit_ctrl_ptr->cursor.is_hide = TRUE;

    //set max string length
    baseedit_ctrl_ptr->str_max_len = GUIEDIT_TIME_MAX_LEN;

    BASEEDIT_Init(baseedit_ctrl_ptr);

    return (result);
}

/*****************************************************************************/
//  Description : timeedit destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TimeEditCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr = (CTRLTIMEEDIT_OBJ_T*) base_ctrl_ptr;

    if (PNULL == timeedit_ctrl_ptr)
    {
        return FALSE;
    }

    // TODO
    // ...

    return (result);
}

/*****************************************************************************/
//  Description : handle timeedit msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E TimeEditCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr = (CTRLTIMEEDIT_OBJ_T*) base_ctrl_ptr;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = PNULL;

    if (PNULL == timeedit_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_APP_UP:
    case MSG_KEYREPEAT_UP:
        result = HandleTimeEditUpDownKey (timeedit_ctrl_ptr, TRUE);
        break;

    case MSG_APP_DOWN:
    case MSG_KEYREPEAT_DOWN:
        result = HandleTimeEditUpDownKey (timeedit_ctrl_ptr, FALSE);
        break;

    case MSG_APP_LEFT:
    case MSG_KEYREPEAT_LEFT:
        result = HandleTimeEditLeftKey (timeedit_ctrl_ptr);
        break;

    case MSG_APP_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
        result = HandleTimeEditRightKey (timeedit_ctrl_ptr);
        break;

    case MSG_NOTIFY_IM_COMMIT:
        {
            GUIEDIT_ADD_STR_T   add_str = {0};

            BASEEDIT_ConvertImCommitMsg(param, &add_str);

            //Date,Time,IP need handle number key msg
            HandleTimeImCommit (timeedit_ctrl_ptr, &add_str);
        }
        break;

    case MSG_APP_WEB:
        result = HandleTimeAmPm (timeedit_ctrl_ptr);

        // 如果没处理，需要让父亲去处理，发消息给应用
		if (MMI_RESULT_FALSE == result)
		{
            parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_TIMEEDIT_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
            parent_vtbl_ptr->HandleEvent (base_ctrl_ptr, msg_id, param);
		}
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        if (!MMK_GET_TP_SLIDE (param))
        {
            HandleTimeAmPm (timeedit_ctrl_ptr);
        }

        parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_TIMEEDIT_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
        parent_vtbl_ptr->HandleEvent (base_ctrl_ptr, msg_id, param);
        break;
#endif

    default:
        parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_TIMEEDIT_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
        result = parent_vtbl_ptr->HandleEvent (base_ctrl_ptr, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : get align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E TimeEditCtrlGetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr   //in:
)
{
    CTRLTIMEEDIT_OBJ_T* timeedit_ctrl_ptr = (CTRLTIMEEDIT_OBJ_T*) baseedit_ctrl_ptr;

    return timeedit_ctrl_ptr->theme.align;
}

/*****************************************************************************/
// 	Description : set align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void TimeEditCtrlSetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr,  //in:
    GUI_ALIGN_E             align               //in
)
{
    CTRLTIMEEDIT_OBJ_T* timeedit_ctrl_ptr = (CTRLTIMEEDIT_OBJ_T*) baseedit_ctrl_ptr;

    timeedit_ctrl_ptr->theme.align = align;
}

/*****************************************************************************/
// 	Description : get im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void TimeEditCtrlGetImInfo (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr, //in:
    BOOLEAN                *is_forbid_ptr,     //in/out:
    GUIIM_TYPE_T           *init_im_ptr,       //in/out:
    GUIIM_TYPE_T           *allow_im_ptr,      //in/out:
    GUIIM_INPUT_TYPE_T     *init_input_ptr,    //in/out:
    GUIIM_INPUT_TYPE_T     *allow_input_ptr,   //in/out:
    GUIIM_CAPITAL_MODE_T   *capital_mode_ptr,  //in/out:
    GUIIM_TAG_T            *tag_ptr            //in/out:
)
{
    CTRLTIMEEDIT_OBJ_T* timeedit_ctrl_ptr = (CTRLTIMEEDIT_OBJ_T*) baseedit_ctrl_ptr;

    *is_forbid_ptr    = timeedit_ctrl_ptr->theme.im.is_forbid_symbol;
    *init_input_ptr   = timeedit_ctrl_ptr->theme.im.init_input_type;
    *allow_input_ptr  = timeedit_ctrl_ptr->theme.im.allow_input_type;
    *init_im_ptr      = timeedit_ctrl_ptr->theme.im.init_im;
    *allow_im_ptr     = timeedit_ctrl_ptr->theme.im.allow_im;
    *capital_mode_ptr = timeedit_ctrl_ptr->theme.im.capital_mode;
    *tag_ptr          = timeedit_ctrl_ptr->theme.im.tag;
}

/*****************************************************************************/
// 	Description : set im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void TimeEditCtrlSetImInfo (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr, //in:
    BOOLEAN                *is_forbid_ptr,     //in:may PNULL
    GUIIM_TYPE_T           *init_im_ptr,       //in:may PNULL
    GUIIM_TYPE_T           *allow_im_ptr,      //in:may PNULL
    GUIIM_INPUT_TYPE_T     *init_type_ptr,     //in:may PNULL
    GUIIM_INPUT_TYPE_T     *allow_type_ptr,    //in:may PNULL
    GUIIM_CAPITAL_MODE_T   *capital_mode_ptr,  //in:may PNULL
    GUIIM_TAG_T            *tag_ptr
)
{
    CTRLTIMEEDIT_OBJ_T* timeedit_ctrl_ptr = (CTRLTIMEEDIT_OBJ_T*) baseedit_ctrl_ptr;

    if (PNULL != is_forbid_ptr)
    {
        timeedit_ctrl_ptr->theme.im.is_forbid_symbol = *is_forbid_ptr;
    }

    if (PNULL != init_im_ptr)
    {
        timeedit_ctrl_ptr->theme.im.init_im = *init_im_ptr;
    }

    if (PNULL != allow_im_ptr)
    {
        timeedit_ctrl_ptr->theme.im.allow_im = *allow_im_ptr;
    }

    if (PNULL != init_type_ptr)
    {
        timeedit_ctrl_ptr->theme.im.init_input_type = *init_type_ptr;
    }

    if (PNULL != allow_type_ptr)
    {
        timeedit_ctrl_ptr->theme.im.allow_input_type = *allow_type_ptr;
    }

    if (PNULL != capital_mode_ptr)
    {
        timeedit_ctrl_ptr->theme.im.capital_mode = *capital_mode_ptr;
    }

    if (PNULL != tag_ptr)
    {
        timeedit_ctrl_ptr->theme.im.tag = *tag_ptr;
    }
}

/*****************************************************************************/
//  Description : 设置横屏时的新edit
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TimeEditCtrlSetLandscapeEdit (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    MMI_HANDLE_T        landscape_edit          // [in] the dst landscape edit
)
{
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr = (CTRLTIMEEDIT_OBJ_T*) baseedit_ctrl_ptr;
    CTRLTIMEEDIT_OBJ_T  *new_edit_ctrl_ptr = GetTimeEditPtr (landscape_edit);
    CTRLBASEEDIT_OBJ_T  *new_baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) new_edit_ctrl_ptr;

    if (PNULL != new_edit_ctrl_ptr)
    {
        //set time
        CTRLTIMEEDIT_SetTime (
                landscape_edit,
                timeedit_ctrl_ptr->hour,
                timeedit_ctrl_ptr->minute,
                timeedit_ctrl_ptr->second);

        //set time style
        CTRLTIMEEDIT_SetTimeStyle (
                landscape_edit,
                (GUIEDIT_SEPARATOR_E *) &timeedit_ctrl_ptr->theme.separator_style,
                (GUIEDIT_TIME_STYLE_E *) &timeedit_ctrl_ptr->theme.time_style,
                (GUIEDIT_TIME_DISPLAY_STYLE_E *) &timeedit_ctrl_ptr->theme.display_style,
                FALSE);

        //set im
        new_edit_ctrl_ptr->theme.im = timeedit_ctrl_ptr->theme.im;

        //set cursor pos and highlight
        VTLBASEEDIT_SetCursorPos (new_baseedit_ctrl_ptr, baseedit_ctrl_ptr->cursor.cur_pos);
        new_baseedit_ctrl_ptr->highlight = baseedit_ctrl_ptr->highlight;
    }
}

/*****************************************************************************/
//  Description : 获取指定偏移的字符是否有效
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// 该方法用于在TP消息点击后，判断是否点在有效的字符上
/*****************************************************************************/
LOCAL BOOLEAN TimeEditCtrlIsValidOffset (           // [ret] TRUE if char offset is valid
    CTRLBASEFIXED_OBJ_T     *basefixed_ctrl_ptr,    // [in] ctrl ptr
    uint16                  *char_offset_ptr,       // [in/out] char offset, and may change the offset if valid
    uint16                  *highlight_len_ptr      // [out] highlight length if char valid
)
{
    BOOLEAN             result = TRUE;
    uint16              char_offset = *char_offset_ptr;
    wchar               separator_char = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) basefixed_ctrl_ptr;
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr = (CTRLTIMEEDIT_OBJ_T*) basefixed_ctrl_ptr;

    separator_char = BASEEDIT_GetSeparatorChar ( (GUIEDIT_SEPARATOR_E) timeedit_ctrl_ptr->theme.separator_style);

    if (separator_char == baseedit_ctrl_ptr->str_ptr[char_offset])
    {
        result = FALSE;
    }
    else
    {
        //set highlight len
        *highlight_len_ptr = 1;

        if (GUIEDIT_TIME_STYLE_12 == timeedit_ctrl_ptr->theme.time_style)
        {
            if (GUIEDIT_TIME_SUFFIX_SEPARATOR == baseedit_ctrl_ptr->str_ptr[char_offset])
            {
                result = FALSE;
            }
            else if ( (GUIEDIT_TIME_SUFFIX_A == baseedit_ctrl_ptr->str_ptr[char_offset]) ||
                      (GUIEDIT_TIME_SUFFIX_P == baseedit_ctrl_ptr->str_ptr[char_offset]))
            {
                *highlight_len_ptr = 2;
            }
            else if (GUIEDIT_TIME_SUFFIX_M == baseedit_ctrl_ptr->str_ptr[char_offset])
            {
                char_offset--;
                *highlight_len_ptr = 2;
            }
        }
    }

    *char_offset_ptr = char_offset;

    return result;
}

/*****************************************************************************/
//  Description : get time string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void TimeEditCtrlConvertToStr (
    CTRLBASEFIXED_OBJ_T     *basefixed_ctrl_ptr
)
{
    BOOLEAN     is_am = FALSE;
    char        separator_char = 0;
    uint8       hour = 0;
    uint8       hour_ten = 0;
    uint8       hour_one = 0;
    uint8       minute_ten = 0;
    uint8       minute_one = 0;
    uint8       second_ten = 0;
    uint8       second_one = 0;
    uint16      i = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) basefixed_ctrl_ptr;
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr = (CTRLTIMEEDIT_OBJ_T*) basefixed_ctrl_ptr;

    if (0 == baseedit_ctrl_ptr->str_len)
    {
        //get separator
        separator_char = BASEEDIT_GetSeparatorChar ( (GUIEDIT_SEPARATOR_E) timeedit_ctrl_ptr->theme.separator_style);

        hour = timeedit_ctrl_ptr->hour;

        //12 hour(01:00-12:59)
        if (GUIEDIT_TIME_STYLE_12 == timeedit_ctrl_ptr->theme.time_style)
        {
            //convert 24 hour to 12 hour
            is_am = Conver24HourTo12Hour (&hour);
        }

        //set hour,minute,second ten and one bit
        hour_ten   = (uint8) (hour / 10);
        hour_one   = (uint8) (hour % 10);
        minute_ten = (uint8) (timeedit_ctrl_ptr->minute / 10);
        minute_one = (uint8) (timeedit_ctrl_ptr->minute % 10);
        second_ten = (uint8) (timeedit_ctrl_ptr->second / 10);
        second_one = (uint8) (timeedit_ctrl_ptr->second % 10);

        switch (timeedit_ctrl_ptr->theme.display_style)
        {
        case GUIEDIT_TIME_DISPLAY_STYLE_HM:
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (hour_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (hour_one + '0');

            baseedit_ctrl_ptr->str_ptr[i++] = separator_char;

            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (minute_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (minute_one + '0');
            break;

        case GUIEDIT_TIME_DISPLAY_STYLE_HMS:
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (hour_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (hour_one + '0');

            baseedit_ctrl_ptr->str_ptr[i++] = separator_char;

            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (minute_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (minute_one + '0');

            baseedit_ctrl_ptr->str_ptr[i++] = ':';

            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (second_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (second_one + '0');
            break;

        default:
            SCI_PASSERT (FALSE, ("GUIEDIT_GetTimeString:display style %d is error!", timeedit_ctrl_ptr->theme.display_style)); /*assert verified*/
            break;
        }

        //add AM or PM
        if (GUIEDIT_TIME_STYLE_12 == timeedit_ctrl_ptr->theme.time_style)
        {
            baseedit_ctrl_ptr->str_ptr[i++] = GUIEDIT_TIME_SUFFIX_SEPARATOR;

            if (is_am)
            {
                baseedit_ctrl_ptr->str_ptr[i++] = GUIEDIT_TIME_SUFFIX_A;
                baseedit_ctrl_ptr->str_ptr[i++] = GUIEDIT_TIME_SUFFIX_M;
            }
            else
            {
                baseedit_ctrl_ptr->str_ptr[i++] = GUIEDIT_TIME_SUFFIX_P;
                baseedit_ctrl_ptr->str_ptr[i++] = GUIEDIT_TIME_SUFFIX_M;
            }
        }

        baseedit_ctrl_ptr->str_len = i;
        SCI_ASSERT (i <= baseedit_ctrl_ptr->str_max_len); /*assert verified*/
    }
}

/*****************************************************************************/
//  Description : get timeedit pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLTIMEEDIT_OBJ_T* GetTimeEditPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        if (!TimeEditTypeOf (ctrl_ptr))
        {
            SCI_TRACE_LOW("== GetTimeEditPtr error == 0x%x", ctrl_ptr);
            ctrl_ptr = PNULL;
        }
    }

    return (CTRLTIMEEDIT_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN TimeEditTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf ( (OBJECT_TYPE_PTR) ctrl_ptr, CTRL_TIMEEDIT_TYPE);
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
//  Description : handle time edit up/down key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTimeEditUpDownKey (
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr,
    BOOLEAN             is_add
)
{
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) timeedit_ctrl_ptr;

    if (baseedit_ctrl_ptr->common_theme.is_cir_handle_ud)
    {
        //get new time
        if (GetNewTime (timeedit_ctrl_ptr, FALSE, is_add, 0))
        {
            //reset length,get new date string
            baseedit_ctrl_ptr->str_len = 0;

            //update time display
            VTLBASEEDIT_DisplayAll (baseedit_ctrl_ptr, FALSE);
        }

        result = MMI_RESULT_TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle time edit up/down/number key,get new time and time string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNewTime (//up/down:is change;number:is valid
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr,
    BOOLEAN            is_num,     //TRUE:number key:FALSE:up/down key
    BOOLEAN            is_add,
    uint8              num_value   //from 0 to 9
)
{
    BOOLEAN             result = FALSE;
    GUIEDIT_TIME_BIT_E  time_bit = GUIEDIT_TIME_BIT_NULL;

    //get cursor position corresponding bit
    time_bit = GetCurTimeBit (timeedit_ctrl_ptr);

    //kevin.lou modified:delete assert
    //SCI_ASSERT(GUIEDIT_TIME_BIT_NULL != time_bit); /*assert verified*/
    if (GUIEDIT_TIME_BIT_NULL == time_bit)
    {
        return result;
    }

    switch (time_bit)
    {
    case GUIEDIT_TIME_BIT_H_TEN:
    case GUIEDIT_TIME_BIT_H_ONE:
        //get new hour
        if (GetNewTimeHour (timeedit_ctrl_ptr, is_num, is_add, num_value, time_bit))
        {
            //adjust minute or second
            AdjustMinOrSec (timeedit_ctrl_ptr);

            result = TRUE;
        }

        break;

    case GUIEDIT_TIME_BIT_M_TEN:
    case GUIEDIT_TIME_BIT_M_ONE:
        //get new minute
        if (GetNewTimeMinute (timeedit_ctrl_ptr, is_num, is_add, num_value, time_bit))
        {
            //adjust minute or second
            AdjustMinOrSec (timeedit_ctrl_ptr);

            result = TRUE;
        }

        break;

    case GUIEDIT_TIME_BIT_S_TEN:
    case GUIEDIT_TIME_BIT_S_ONE:
        //get new second
        result = GetNewTimeSecond (timeedit_ctrl_ptr, is_num, is_add, num_value, time_bit);
        break;

    case GUIEDIT_TIME_BIT_AM_PM:
        result = GetNewTimeSuffix (timeedit_ctrl_ptr);
        break;

    default:
        //SCI_TRACE_LOW:"GetNewTime: time bit %d is error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_TIME_619_112_2_18_3_16_19_135, (uint8*) "d", time_bit);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get current time bit
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIEDIT_TIME_BIT_E GetCurTimeBit (
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr
)
{
    GUIEDIT_TIME_BIT_E  time_bit = GUIEDIT_TIME_BIT_NULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) timeedit_ctrl_ptr;

    switch (baseedit_ctrl_ptr->cursor.cur_pos)
    {
    case 0:
        time_bit = GUIEDIT_TIME_BIT_H_TEN;
        break;

    case 1:
        time_bit = GUIEDIT_TIME_BIT_H_ONE;
        break;

    case 3:
        time_bit = GUIEDIT_TIME_BIT_M_TEN;
        break;

    case 4:
        time_bit = GUIEDIT_TIME_BIT_M_ONE;
        break;

    case 6:

        switch (timeedit_ctrl_ptr->theme.display_style)
        {
        case GUIEDIT_TIME_DISPLAY_STYLE_HM:
            time_bit = GUIEDIT_TIME_BIT_AM_PM;
            break;

        case GUIEDIT_TIME_DISPLAY_STYLE_HMS:
            time_bit = GUIEDIT_TIME_BIT_S_TEN;
            break;

        default:
            SCI_PASSERT (FALSE, ("GetCurTimeBit:display style %d is error!", timeedit_ctrl_ptr->theme.display_style)); /*assert verified*/
            break;
        }

        break;

    case 7:
        SCI_ASSERT (GUIEDIT_TIME_DISPLAY_STYLE_HMS == timeedit_ctrl_ptr->theme.display_style); /*assert verified*/
        time_bit = GUIEDIT_TIME_BIT_S_ONE;
        break;

    case 9:
        SCI_ASSERT (GUIEDIT_TIME_DISPLAY_STYLE_HMS == timeedit_ctrl_ptr->theme.display_style); /*assert verified*/
        time_bit = GUIEDIT_TIME_BIT_AM_PM;
        break;

    default:
        //SCI_TRACE_LOW:"GetCurTimeBit: current cursor position %d is error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_TIME_682_112_2_18_3_16_19_136, (uint8*) "d", baseedit_ctrl_ptr->cursor.cur_pos);
        break;
    }

    return (time_bit);
}

/*****************************************************************************/
//  Description : handle time edit up/down/number key,get new time hour
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNewTimeHour (//up/down:is change;number:is valid
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr,
    BOOLEAN             is_num,     //TRUE:number key:FALSE:up/down key
    BOOLEAN             is_add,
    uint8               num_value,  //from 0 to 9
    GUIEDIT_TIME_BIT_E  time_bit
)
{
    BOOLEAN     result = FALSE;
    BOOLEAN     is_am = FALSE;
    uint8       new_hour = 0;
    uint8       hour_ten = 0;
    uint8       bit_value = 0;
    uint8       bit_min = 0;
    uint8       bit_max = 0;
    uint8       unit_value = 0;
    uint8       min_hour = 0;
    uint8       max_hour = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) timeedit_ctrl_ptr;

    //get hour ten bit
    hour_ten = (uint8) (baseedit_ctrl_ptr->str_ptr[0] - '0');

    //set bit value
    bit_value = (uint8) (baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos] - '0');

    switch (time_bit)
    {
    case GUIEDIT_TIME_BIT_H_TEN:
        //set unit and new hour
        unit_value = 10;
        new_hour   = (uint8) (baseedit_ctrl_ptr->str_ptr[1] - '0');

        //set bit min and max
        bit_min = 0;

        if (GUIEDIT_TIME_STYLE_12 == timeedit_ctrl_ptr->theme.time_style)
        {
            bit_max = 1;
        }
        else
        {
            bit_max = 2;
        }

        break;

    case GUIEDIT_TIME_BIT_H_ONE:
        //set unit and new hour
        unit_value = 1;
        new_hour   = (uint8) (hour_ten * 10);

        //set bit min and max
        bit_min = 0;

        if (GUIEDIT_TIME_STYLE_12 == timeedit_ctrl_ptr->theme.time_style)
        {
            if (0 == hour_ten)
            {
                bit_min = 1;
                bit_max = 9;
            }
            else
            {
                SCI_ASSERT (1 == hour_ten); /*assert verified*/
                bit_max = 2;
            }
        }
        else
        {
            if ( (0 == hour_ten) ||
                    (1 == hour_ten))
            {
                bit_max = 9;
            }
            else
            {
                SCI_ASSERT (2 == hour_ten); /*assert verified*/
                bit_max = 3;
            }
        }

        break;

    default:
        SCI_PASSERT (FALSE, ("GetNewTimeHour: time bit %d is error!", time_bit)); /*assert verified*/
        break;
    }

    //get bit value
    if (is_num)
    {
        if ( (num_value >= bit_min) && (num_value <= bit_max))
        {
            bit_value = num_value;
            result = TRUE;
        }
    }
    else
    {
        bit_value = BASEFIXED_CalculateBitValue (is_add, bit_value, bit_min, bit_max);
    }

    //get new hour
    new_hour = (uint8) (new_hour + bit_value * unit_value);

    //set max hour
    if (GUIEDIT_TIME_STYLE_12 == timeedit_ctrl_ptr->theme.time_style)
    {
        min_hour = 1;
        max_hour = 12;
    }
    else
    {
        min_hour = 0;
        max_hour = 23;
    }

    //adjust hour
    if (new_hour < min_hour)
    {
        new_hour = min_hour;
    }

    if (new_hour > max_hour)
    {
        new_hour = max_hour;
    }

    //12 hour(01:00-12:59)
    if (GUIEDIT_TIME_STYLE_12 == timeedit_ctrl_ptr->theme.time_style)
    {
        if (GUIEDIT_TIME_SUFFIX_A == baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->str_len - GUIEDIT_TIME_SUFFIX_LEN + 1])
        {
            is_am = TRUE;
        }

        //convert 24 hour to 12 hour
        new_hour = Conver12HourTo24Hour (is_am, new_hour);
    }

    //set new hour
    if (new_hour != timeedit_ctrl_ptr->hour)
    {
        timeedit_ctrl_ptr->hour = new_hour;

        if (!is_num)
        {
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : adjust minute or second when time is timer,must > 0
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AdjustMinOrSec (
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr
)
{
    if (GUIEDIT_TIME_STYLE_TIMER == timeedit_ctrl_ptr->theme.time_style)
    {
        switch (timeedit_ctrl_ptr->theme.display_style)
        {
        case GUIEDIT_TIME_DISPLAY_STYLE_HM:
            //adjust minute
            if ((0 == timeedit_ctrl_ptr->hour) &&
                (0 == timeedit_ctrl_ptr->minute))
            {
                timeedit_ctrl_ptr->minute = 1;
            }

            break;

        case GUIEDIT_TIME_DISPLAY_STYLE_HMS:
            //adjust second
            if ((0 == timeedit_ctrl_ptr->hour) &&
                (0 == timeedit_ctrl_ptr->minute) &&
                (0 == timeedit_ctrl_ptr->second))
            {
                timeedit_ctrl_ptr->second = 1;
            }

            break;

        default:
            //SCI_TRACE_LOW:"AdjustMinOrSec:display style %d is error!"
            SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_TIME_872_112_2_18_3_16_19_137, (uint8*) "d", timeedit_ctrl_ptr->theme.display_style);
            break;
        }
    }
}

/*****************************************************************************/
//  Description : handle time edit up/down/number key,get new time minute
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNewTimeMinute (//up/down:is change;number:is valid
    CTRLTIMEEDIT_OBJ_T      *timeedit_ctrl_ptr,
    BOOLEAN                 is_num,     //TRUE:number key:FALSE:up/down key
    BOOLEAN                 is_add,
    uint8                   num_value,  //from 0 to 9
    GUIEDIT_TIME_BIT_E      time_bit
)
{
    BOOLEAN     result = FALSE;
    uint8       new_minute = 0;
    uint8       minute_ten = 0;
    uint8       bit_value = 0;
    uint8       bit_min = 0;
    uint8       bit_max = 0;
    uint8       unit_value = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) timeedit_ctrl_ptr;

    //set minute ten
    minute_ten = (uint8) (timeedit_ctrl_ptr->minute / 10);

    //set bit value
    bit_value = (uint8) (baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos] - '0');

    switch (time_bit)
    {
    case GUIEDIT_TIME_BIT_M_TEN:
        //set unit and new minute
        unit_value = 10;
        new_minute = (uint8) (timeedit_ctrl_ptr->minute % 10);

        //set bit min and max
        bit_min = 0;
        bit_max = 5;
        break;

    case GUIEDIT_TIME_BIT_M_ONE:
        //set unit and new minute
        unit_value = 1;
        new_minute = (uint8) (minute_ten * 10);

        //set bit min and max
        if ( (GUIEDIT_TIME_STYLE_TIMER == timeedit_ctrl_ptr->theme.time_style) &&
                (GUIEDIT_TIME_DISPLAY_STYLE_HM == timeedit_ctrl_ptr->theme.display_style) &&
                (0 == timeedit_ctrl_ptr->hour) &&
                (0 == minute_ten))
        {
            bit_min = 1;
        }
        else
        {
            bit_min = 0;
        }

        bit_max = 9;
        break;

    default:
        SCI_PASSERT (FALSE, ("GetNewTimeMinute: time bit %d is error!", time_bit)); /*assert verified*/
        break;
    }

    //get bit value
    if (is_num)
    {
        if ( (num_value >= bit_min) &&
                (num_value <= bit_max))
        {
            bit_value = num_value;
            result = TRUE;
        }
    }
    else
    {
        bit_value = BASEFIXED_CalculateBitValue (is_add, bit_value, bit_min, bit_max);
    }

    //get new minute
    new_minute = (uint8) (new_minute + bit_value * unit_value);

    //set new minute
    if (new_minute != timeedit_ctrl_ptr->minute)
    {
        timeedit_ctrl_ptr->minute = new_minute;

        if (!is_num)
        {
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle time edit up/down/number key,get new time second
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNewTimeSecond (//up/down:is change;number:is valid
    CTRLTIMEEDIT_OBJ_T      *timeedit_ctrl_ptr,
    BOOLEAN                 is_num,     //TRUE:number key:FALSE:up/down key
    BOOLEAN                 is_add,
    uint8                   num_value,  //from 0 to 9
    GUIEDIT_TIME_BIT_E      time_bit
)
{
    BOOLEAN     result = FALSE;
    uint8       second_ten = 0;
    uint8       new_second = 0;
    uint8       bit_value = 0;
    uint8       bit_min = 0;
    uint8       bit_max = 0;
    uint8       unit_value = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) timeedit_ctrl_ptr;

    SCI_ASSERT (GUIEDIT_TIME_DISPLAY_STYLE_HMS == timeedit_ctrl_ptr->theme.display_style); /*assert verified*/

    //set second ten
    second_ten = (uint8) (timeedit_ctrl_ptr->second / 10);

    //set bit value
    bit_value = (uint8) (baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos] - '0');

    switch (time_bit)
    {
    case GUIEDIT_TIME_BIT_S_TEN:
        //set unit and new minute
        unit_value = 10;
        new_second = (uint8) (timeedit_ctrl_ptr->second % 10);

        //set bit min and max
        bit_min = 0;
        bit_max = 5;
        break;

    case GUIEDIT_TIME_BIT_S_ONE:
        //set unit and new minute
        unit_value = 1;
        new_second = (uint8) (second_ten * 10);

        //set bit min and max
        if ( (GUIEDIT_TIME_STYLE_TIMER == timeedit_ctrl_ptr->theme.time_style) &&
                (GUIEDIT_TIME_DISPLAY_STYLE_HMS == timeedit_ctrl_ptr->theme.display_style) &&
                (0 == timeedit_ctrl_ptr->hour) &&
                (0 == timeedit_ctrl_ptr->minute) &&
                (0 == second_ten))
        {
            bit_min = 1;
        }
        else
        {
            bit_min = 0;
        }

        bit_max = 9;
        break;

    default:
        SCI_PASSERT (FALSE, ("GetNewTimeSecond: time bit %d is error!", time_bit)); /*assert verified*/
        break;
    }

    //get bit value
    if (is_num)
    {
        if ( (num_value >= bit_min) &&
                (num_value <= bit_max))
        {
            bit_value = num_value;
            result = TRUE;
        }
    }
    else
    {
        bit_value = BASEFIXED_CalculateBitValue (is_add, bit_value, bit_min, bit_max);
    }

    //get new second
    new_second = (uint8) (new_second + bit_value * unit_value);

    //set new second
    if (new_second != timeedit_ctrl_ptr->second)
    {
        timeedit_ctrl_ptr->second = new_second;

        if (!is_num)
        {
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get new time suffix
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNewTimeSuffix (
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr
)
{
    BOOLEAN     result = TRUE;
    BOOLEAN     is_am = FALSE;
    uint8       new_hour = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) timeedit_ctrl_ptr;

    SCI_ASSERT (GUIEDIT_TIME_STYLE_12 == timeedit_ctrl_ptr->theme.time_style); /*assert verified*/

    //switch AM and PM
    if (GUIEDIT_TIME_SUFFIX_A == baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->str_len - GUIEDIT_TIME_SUFFIX_LEN + 1])
    {
        is_am = TRUE;
    }

    //get current hour
    new_hour = timeedit_ctrl_ptr->hour;

    //set new hour
    if (is_am)
    {
        new_hour = (uint8) (new_hour + 12);
    }
    else
    {
        new_hour = (uint8) (new_hour - 12);
    }

    //set new hour
    timeedit_ctrl_ptr->hour = new_hour;

    return (result);
}

/*****************************************************************************/
//  Description : handle time edit left key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTimeEditLeftKey (
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr
)
{
    uint16          highlight_len = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) timeedit_ctrl_ptr;

    if ( (0 == baseedit_ctrl_ptr->cursor.cur_pos) &&
            (!baseedit_ctrl_ptr->common_theme.is_cir_handle_lf))
    {
        result = MMI_RESULT_FALSE;
    }
    else
    {
        //reset highlight not display
        BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, FALSE);

        //set current cursor position
        if (0 < baseedit_ctrl_ptr->cursor.cur_pos)
        {
            VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, (uint16) (baseedit_ctrl_ptr->cursor.cur_pos - 1));
            highlight_len = 1;

            //if is separator
            if ((BASEEDIT_GetSeparatorChar ( (GUIEDIT_SEPARATOR_E) timeedit_ctrl_ptr->theme.separator_style) == baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos]) ||
                (GUIEDIT_TIME_SUFFIX_SEPARATOR == baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos]))
            {
                VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, (uint16) (baseedit_ctrl_ptr->cursor.cur_pos - 1));
            }
        }
        else
        {
            if (GUIEDIT_TIME_STYLE_12 == timeedit_ctrl_ptr->theme.time_style)
            {
                VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, (uint16) (baseedit_ctrl_ptr->str_len - GUIEDIT_TIME_SUFFIX_LEN + 1));
                highlight_len = (uint16) (GUIEDIT_TIME_SUFFIX_LEN - 1);
            }
            else
            {
                VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, (uint16) (baseedit_ctrl_ptr->str_len - 1));
                highlight_len = 1;
            }
        }

        //set highlight start position
        VTLBASEEDIT_SetHighlight (
                baseedit_ctrl_ptr,
                baseedit_ctrl_ptr->cursor.cur_pos,
                (uint16) (baseedit_ctrl_ptr->cursor.cur_pos + highlight_len),
                SETHIGHLIGHT_BOTH);

        //display highlight
        BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, TRUE);

        //set display cursor
        baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle time edit right key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTimeEditRightKey (
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr
)
{
    uint16          time_max_len = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) timeedit_ctrl_ptr;

    if (GUIEDIT_TIME_STYLE_12 == timeedit_ctrl_ptr->theme.time_style)
    {
        //AM or PM is one
        time_max_len = (uint16) (baseedit_ctrl_ptr->str_len - 1);
    }
    else
    {
        time_max_len = baseedit_ctrl_ptr->str_len;
    }

    if ( (baseedit_ctrl_ptr->cursor.cur_pos == (time_max_len - 1)) &&
            (!baseedit_ctrl_ptr->common_theme.is_cir_handle_lf))
    {
        result = MMI_RESULT_FALSE;
    }
    else
    {
        //reset highlight not display
        BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, FALSE);

        //set current cursor position
        MoveTimeCursorToRight (timeedit_ctrl_ptr);

        //display highlight
        BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, TRUE);

        //set display cursor
        baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : move time cursor to right
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MoveTimeCursorToRight (
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr
)
{
    uint16  time_max_len = 0;
    uint16  highlight_len = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) timeedit_ctrl_ptr;

    if (GUIEDIT_TIME_STYLE_12 == timeedit_ctrl_ptr->theme.time_style)
    {
        //AM or PM is one
        time_max_len = (uint16) (baseedit_ctrl_ptr->str_len - 1);
    }
    else
    {
        time_max_len = baseedit_ctrl_ptr->str_len;
    }

    highlight_len = 1;

    if (baseedit_ctrl_ptr->cursor.cur_pos < (time_max_len - 1))
    {
        VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, (uint16) (baseedit_ctrl_ptr->cursor.cur_pos + 1));

        //if is separator
        if ((BASEEDIT_GetSeparatorChar ( (GUIEDIT_SEPARATOR_E) timeedit_ctrl_ptr->theme.separator_style) == baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos]) ||
            (GUIEDIT_TIME_SUFFIX_SEPARATOR == baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos]))
        {
            VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, (uint16) (baseedit_ctrl_ptr->cursor.cur_pos + 1));
        }

        //AM or PM is one
        if ((GUIEDIT_TIME_SUFFIX_A == baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos]) ||
            (GUIEDIT_TIME_SUFFIX_P == baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos]))
        {
            highlight_len = (uint16) (GUIEDIT_TIME_SUFFIX_LEN - 1);
        }
    }
    else
    {
        VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, 0);
    }

    //set highlight start position
    VTLBASEEDIT_SetHighlight (
            baseedit_ctrl_ptr,
            baseedit_ctrl_ptr->cursor.cur_pos,
            (uint16) (baseedit_ctrl_ptr->cursor.cur_pos + highlight_len),
            SETHIGHLIGHT_BOTH);
}

/*****************************************************************************/
//  Description : time handle im commit msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleTimeImCommit (
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr,
    GUIEDIT_ADD_STR_T   *add_str_ptr
)
{
    uint8   num_value = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) timeedit_ctrl_ptr;

    //get num value
    if (BASEFIXED_GetNumValueByIm (&num_value, add_str_ptr))
    {
        //get new time
        if (GetNewTime (timeedit_ctrl_ptr, TRUE, FALSE, num_value))
        {
            //set current cursor position
            MoveTimeCursorToRight (timeedit_ctrl_ptr);

            //reset length,get new date string
            baseedit_ctrl_ptr->str_len = 0;

            //update time display
            VTLBASEEDIT_DisplayAll (baseedit_ctrl_ptr, FALSE);
        }
    }
}

/*****************************************************************************/
//  Description : time handle am/pm
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTimeAmPm (
    CTRLTIMEEDIT_OBJ_T  *timeedit_ctrl_ptr
)
{
    MMI_RESULT_E		result = MMI_RESULT_FALSE;
    GUIEDIT_TIME_BIT_E  time_bit = GUIEDIT_TIME_BIT_NULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) timeedit_ctrl_ptr;

    //get cursor position corresponding bit
    time_bit = GetCurTimeBit (timeedit_ctrl_ptr);

    if (GUIEDIT_TIME_BIT_AM_PM == time_bit)
    {
        GetNewTimeSuffix (timeedit_ctrl_ptr);

        //reset length,get new date string
        baseedit_ctrl_ptr->str_len = 0;

        //update time display
        VTLBASEEDIT_DisplayAll (baseedit_ctrl_ptr, FALSE);

        result = MMI_RESULT_TRUE;
    }

    return (result);
}

