/*****************************************************************************
** File Name:      ctrldateedit.c                                               *
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
#include "guifont.h"
#include "mmi_theme.h"

#include "ctrldateedit.h"
#include "ctrldateedit_export.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define GUIEDIT_DATE_MAX_LEN    10  //xxxx.xx.xx

//edit date bit
typedef enum
{
    GUIEDIT_DATE_BIT_NULL,
    GUIEDIT_DATE_BIT_Y_THOUSAND,//年-千位
    GUIEDIT_DATE_BIT_Y_HUNDRED, //年-百位
    GUIEDIT_DATE_BIT_Y_TEN,     //年-十位
    GUIEDIT_DATE_BIT_Y_ONE,     //年-个位
    GUIEDIT_DATE_BIT_M_TEN,     //月-十位
    GUIEDIT_DATE_BIT_M_ONE,     //月-个位
    GUIEDIT_DATE_BIT_D_TEN,     //天-十位
    GUIEDIT_DATE_BIT_D_ONE,     //天-个位
    GUIEDIT_DATE_BIT_MAX
} GUIEDIT_DATE_BIT_E;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//年月日对应的位数
LOCAL const GUIEDIT_DATE_BIT_E s_guiedit_date_ymd_bit[] =
{
    GUIEDIT_DATE_BIT_Y_THOUSAND,//年-千位
    GUIEDIT_DATE_BIT_Y_HUNDRED, //年-百位
    GUIEDIT_DATE_BIT_Y_TEN,     //年-十位
    GUIEDIT_DATE_BIT_Y_ONE,     //年-个位

    GUIEDIT_DATE_BIT_NULL,      //separator

    GUIEDIT_DATE_BIT_M_TEN,     //月-十位
    GUIEDIT_DATE_BIT_M_ONE,     //月-个位

    GUIEDIT_DATE_BIT_NULL,      //separator

    GUIEDIT_DATE_BIT_D_TEN,     //天-十位
    GUIEDIT_DATE_BIT_D_ONE,     //天-个位
};

//年日月对应的位数
LOCAL const GUIEDIT_DATE_BIT_E s_guiedit_date_ydm_bit[] =
{
    GUIEDIT_DATE_BIT_Y_THOUSAND,//年-千位
    GUIEDIT_DATE_BIT_Y_HUNDRED, //年-百位
    GUIEDIT_DATE_BIT_Y_TEN,     //年-十位
    GUIEDIT_DATE_BIT_Y_ONE,     //年-个位

    GUIEDIT_DATE_BIT_NULL,      //separator

    GUIEDIT_DATE_BIT_D_TEN,     //天-十位
    GUIEDIT_DATE_BIT_D_ONE,     //天-个位

    GUIEDIT_DATE_BIT_NULL,      //separator

    GUIEDIT_DATE_BIT_M_TEN,     //月-十位
    GUIEDIT_DATE_BIT_M_ONE,     //月-个位
};

//月年日对应的位数
LOCAL const GUIEDIT_DATE_BIT_E s_guiedit_date_myd_bit[] =
{
    GUIEDIT_DATE_BIT_M_TEN,     //月-十位
    GUIEDIT_DATE_BIT_M_ONE,     //月-个位

    GUIEDIT_DATE_BIT_NULL,      //separator

    GUIEDIT_DATE_BIT_Y_THOUSAND,//年-千位
    GUIEDIT_DATE_BIT_Y_HUNDRED, //年-百位
    GUIEDIT_DATE_BIT_Y_TEN,     //年-十位
    GUIEDIT_DATE_BIT_Y_ONE,     //年-个位

    GUIEDIT_DATE_BIT_NULL,      //separator

    GUIEDIT_DATE_BIT_D_TEN,     //天-十位
    GUIEDIT_DATE_BIT_D_ONE,     //天-个位
};

//月日年对应的位数
LOCAL const GUIEDIT_DATE_BIT_E s_guiedit_date_mdy_bit[] =
{
    GUIEDIT_DATE_BIT_M_TEN,     //月-十位
    GUIEDIT_DATE_BIT_M_ONE,     //月-个位

    GUIEDIT_DATE_BIT_NULL,      //separator

    GUIEDIT_DATE_BIT_D_TEN,     //天-十位
    GUIEDIT_DATE_BIT_D_ONE,     //天-个位

    GUIEDIT_DATE_BIT_NULL,      //separator

    GUIEDIT_DATE_BIT_Y_THOUSAND,//年-千位
    GUIEDIT_DATE_BIT_Y_HUNDRED, //年-百位
    GUIEDIT_DATE_BIT_Y_TEN,     //年-十位
    GUIEDIT_DATE_BIT_Y_ONE,     //年-个位
};

//日年月对应的位数
LOCAL const GUIEDIT_DATE_BIT_E s_guiedit_date_dym_bit[] =
{
    GUIEDIT_DATE_BIT_D_TEN,     //天-十位
    GUIEDIT_DATE_BIT_D_ONE,     //天-个位

    GUIEDIT_DATE_BIT_NULL,      //separator

    GUIEDIT_DATE_BIT_Y_THOUSAND,//年-千位
    GUIEDIT_DATE_BIT_Y_HUNDRED, //年-百位
    GUIEDIT_DATE_BIT_Y_TEN,     //年-十位
    GUIEDIT_DATE_BIT_Y_ONE,     //年-个位

    GUIEDIT_DATE_BIT_NULL,      //separator

    GUIEDIT_DATE_BIT_M_TEN,     //月-十位
    GUIEDIT_DATE_BIT_M_ONE,     //月-个位
};

//日月年对应的位数
LOCAL const GUIEDIT_DATE_BIT_E s_guiedit_date_dmy_bit[] =
{
    GUIEDIT_DATE_BIT_D_TEN,     //天-十位
    GUIEDIT_DATE_BIT_D_ONE,     //天-个位

    GUIEDIT_DATE_BIT_NULL,      //separator

    GUIEDIT_DATE_BIT_M_TEN,     //月-十位
    GUIEDIT_DATE_BIT_M_ONE,     //月-个位

    GUIEDIT_DATE_BIT_NULL,      //separator

    GUIEDIT_DATE_BIT_Y_THOUSAND,//年-千位
    GUIEDIT_DATE_BIT_Y_HUNDRED, //年-百位
    GUIEDIT_DATE_BIT_Y_TEN,     //年-十位
    GUIEDIT_DATE_BIT_Y_ONE,     //年-个位
};

//每个月对应的天数
LOCAL const uint8 s_guiedit_date_month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
LOCAL const uint8 s_guiedit_date_leap_month_days[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

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
//  Description : init dateedit class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DateEditCtrlInitVtbl (
    CTRLDATEEDIT_VTBL_T        *dateedit_vtbl_ptr
);

/*****************************************************************************/
//  Description : dateedit construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DateEditCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLDATEEDIT_INIT_PARAM_T   *dateedit_param_ptr
);

/*****************************************************************************/
//  Description : dateedit destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DateEditCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DateEditCtrlPackInitParam (
    GUIEDIT_INIT_DATA_T         *dateedit_init_ptr,   //in
    CTRLDATEEDIT_INIT_PARAM_T   *dateedit_param_ptr   //out
);

/*****************************************************************************/
//  Description : handle dateedit msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DateEditCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : get dateedit pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLDATEEDIT_OBJ_T* GetDateEditPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN DateEditTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
// 	Description : get align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E DateEditCtrlGetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr   //in:
);

/*****************************************************************************/
// 	Description : set align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void DateEditCtrlSetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr,  //in:
    GUI_ALIGN_E             align               //in
);

/*****************************************************************************/
// 	Description : get im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void DateEditCtrlGetImInfo (
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
LOCAL void DateEditCtrlSetImInfo (
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
LOCAL void DateEditCtrlSetLandscapeEdit (
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
LOCAL BOOLEAN DateEditCtrlIsValidOffset (           // [ret] TRUE if char offset is valid
    CTRLBASEFIXED_OBJ_T     *basefixed_ctrl_ptr,    // [in] ctrl ptr
    uint16                  *char_offset_ptr,       // [in/out] char offset, and may change the offset if valid
    uint16                  *highlight_len_ptr      // [out] highlight length if char valid
);

/*****************************************************************************/
//  Description : get date string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DateEditCtrlConvertToStr (
    CTRLBASEFIXED_OBJ_T     *basefixed_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle date edit up/down key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDateEditUpDownKey (
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr,
    BOOLEAN             is_add
);

/*****************************************************************************/
//  Description : handle date edit up/down/number key,get new date and date string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNewDate (//up/down:is change;number:is valid
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr,
    BOOLEAN            is_num,     //TRUE:number key:FALSE:up/down key
    BOOLEAN            is_add,
    uint8              num_value   //from 0 to 9
);

/*****************************************************************************/
//  Description : get current date bit
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIEDIT_DATE_BIT_E GetCurDateBit (
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle date edit up/down/number key,get new date year
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNewDateYear (//up/down:is change;number:is valid
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr,
    BOOLEAN             is_num,     //TRUE:number key:FALSE:up/down key
    BOOLEAN             is_add,
    uint8               num_value,  //from 0 to 9
    GUIEDIT_DATE_BIT_E  date_bit
);

/*****************************************************************************/
//  Description : adjust day by year and month
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint8 AdjustDateDay (
    uint16	year,
    uint8	month,
    uint8	day
);

/*****************************************************************************/
//  Description : handle date edit up/down/number key,get new date month
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNewDateMonth (//up/down:is change;number:is valid
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr,
    BOOLEAN             is_num,     //TRUE:number key:FALSE:up/down key
    BOOLEAN             is_add,
    uint8               num_value,  //from 0 to 9
    GUIEDIT_DATE_BIT_E  date_bit
);

/*****************************************************************************/
//  Description : handle date edit up/down key,get new date day
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNewDateDay (//up/down:is change;number:is valid
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr,
    BOOLEAN             is_num,     //TRUE:number key:FALSE:up/down key
    BOOLEAN             is_add,
    uint8               num_value,  //from 0 to 9
    GUIEDIT_DATE_BIT_E  date_bit
);

/*****************************************************************************/
//  Description : handle date edit left key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDateEditLeftKey (
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle date edit right key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDateEditRightKey (
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : move date cursor to right
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MoveDateCursorToRight (
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : date handle im commit msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleDateImCommit (
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr,
    GUIEDIT_ADD_STR_T   *add_str_ptr
);

/*****************************************************************************/
//  Description : check date valid
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CheckDateValid (
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr
);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get date
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLDATEEDIT_GetDate (
    MMI_CTRL_ID_T   ctrl_id,    //in
    uint16          *year_ptr,  //in/out:may PNULL
    uint8           *month_ptr, //in/out:may PNULL
    uint8           *day_ptr    //in/out:may PNULL
)
{
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    dateedit_ctrl_ptr = GetDateEditPtr (ctrl_id);

    if (PNULL != dateedit_ctrl_ptr)
    {
        //set year
        if (PNULL != year_ptr)
        {
            *year_ptr = dateedit_ctrl_ptr->year;
        }

        //set month
        if (PNULL != month_ptr)
        {
            *month_ptr = dateedit_ctrl_ptr->month;
        }

        //set day
        if (PNULL != day_ptr)
        {
            *day_ptr = dateedit_ctrl_ptr->day;
        }
    }
}

/*****************************************************************************/
//  Description : set date
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLDATEEDIT_SetDate (
    MMI_CTRL_ID_T   ctrl_id,
    uint16          year,
    uint8           month,
    uint8           day
)
{
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    dateedit_ctrl_ptr = GetDateEditPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)dateedit_ctrl_ptr;

    if (PNULL != dateedit_ctrl_ptr)
    {
        //set date
        dateedit_ctrl_ptr->year  = year;
        dateedit_ctrl_ptr->month = month;
        dateedit_ctrl_ptr->day   = day;

        CheckDateValid (dateedit_ctrl_ptr);

        //reset length,get new date string
        baseedit_ctrl_ptr->str_len = 0;
    }
}

/*****************************************************************************/
//  Description : set date style and separator
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLDATEEDIT_SetDateStyle (
    MMI_CTRL_ID_T          ctrl_id,
    GUIEDIT_SEPARATOR_E    *separator_ptr, //may PNULL
    GUIEDIT_DATE_STYLE_E   *date_style_ptr,//may PNULL
    BOOLEAN                is_update
)
{
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    dateedit_ctrl_ptr = GetDateEditPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)dateedit_ctrl_ptr;

    if (PNULL != dateedit_ctrl_ptr)
    {
        //set date type and separator
        if (PNULL != separator_ptr)
        {
            dateedit_ctrl_ptr->theme.separator_style = *separator_ptr;
        }

        if (PNULL != date_style_ptr)
        {
            dateedit_ctrl_ptr->theme.date_style = *date_style_ptr;
        }

        //reset length,get new date string
        baseedit_ctrl_ptr->str_len = 0;

        if (is_update)
        {
            //set cursor
            VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, 0);
            baseedit_ctrl_ptr->cursor.x_coordinate = GUIEDIT_CURSOR_INVALID_COORDINATE;

            //set highlight
            VTLBASEEDIT_SetHighlight (baseedit_ctrl_ptr, 0, 1, SETHIGHLIGHT_BOTH);

            //update
            VTLBASEEDIT_DisplayAll (baseedit_ctrl_ptr, FALSE);
        }
    }
}

/*****************************************************************************/
//  Description : get date range
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUIEDIT_DATE_YEAR_RANGE_E CTRLDATEEDIT_GetDateRange (
    MMI_CTRL_ID_T     ctrl_id
)
{
    CTRLDATEEDIT_OBJ_T          *dateedit_ctrl_ptr = PNULL;
    GUIEDIT_DATE_YEAR_RANGE_E   year_range = GUIEDIT_DATE_YEAR_RANGE_NULL;

    //get edit pointer by control id
    dateedit_ctrl_ptr = GetDateEditPtr (ctrl_id);

    if (PNULL != dateedit_ctrl_ptr)
    {
        year_range = dateedit_ctrl_ptr->year_range;
    }

    return (year_range);
}

/*****************************************************************************/
//  Description : set date range
//  Global resource dependence :
//  Author: Jassmine
//  Note: default is GUIEDIT_DATE_YEAR_RANGE_SET
/*****************************************************************************/
PUBLIC void CTRLDATEEDIT_SetDateRange (
    MMI_CTRL_ID_T              ctrl_id,
    GUIEDIT_DATE_YEAR_RANGE_E  year_range_type
)
{
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    dateedit_ctrl_ptr = GetDateEditPtr (ctrl_id);

    if (PNULL != dateedit_ctrl_ptr)
    {
        dateedit_ctrl_ptr->year_range = year_range_type;
    }
}

/*****************************************************************************/
//  Description : get dateedit type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T DATEEDIT_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASEFIXED_TYPE,                                // parent class type
                       "dateedit",                                         // control name
                       sizeof (CTRLDATEEDIT_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) DateEditCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) DateEditCtrlDestruct,      // control destructor function
                       sizeof (CTRLDATEEDIT_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) DateEditCtrlPackInitParam,     // convert init param
                       sizeof (CTRLDATEEDIT_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) DateEditCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init dateedit class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DateEditCtrlInitVtbl (
    CTRLDATEEDIT_VTBL_T        *dateedit_vtbl_ptr
)
{
    CTRLBASE_VTBL_T         *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) dateedit_vtbl_ptr;
    CTRLBASEEDIT_VTBL_T     *baseedit_ctrl_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*) dateedit_vtbl_ptr;
    CTRLBASEFIXED_VTBL_T    *basefixed_ctrl_vtbl_ptr = (CTRLBASEFIXED_VTBL_T*) dateedit_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = DateEditCtrlHandleMsg;

    baseedit_ctrl_vtbl_ptr->GetAlign = DateEditCtrlGetAlign;
    baseedit_ctrl_vtbl_ptr->SetAlign = DateEditCtrlSetAlign;
    baseedit_ctrl_vtbl_ptr->GetImInfo = DateEditCtrlGetImInfo;
    baseedit_ctrl_vtbl_ptr->SetImInfo = DateEditCtrlSetImInfo;
    baseedit_ctrl_vtbl_ptr->SetLandscapeEdit = DateEditCtrlSetLandscapeEdit;

    basefixed_ctrl_vtbl_ptr->IsValidOffset = DateEditCtrlIsValidOffset;
    basefixed_ctrl_vtbl_ptr->ConvertToStr = DateEditCtrlConvertToStr;

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
LOCAL void DateEditCtrlPackInitParam (
    GUIEDIT_INIT_DATA_T         *dateedit_init_ptr,   //in
    CTRLDATEEDIT_INIT_PARAM_T   *dateedit_param_ptr   //out
)
{
    CTRLBASE_INIT_PARAM_T       *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) dateedit_param_ptr;
    CTRLBASEEDIT_INIT_PARAM_T   *baseedit_ctrl_param_ptr = (CTRLBASEEDIT_INIT_PARAM_T*) dateedit_param_ptr;

    if (PNULL == dateedit_init_ptr || PNULL == dateedit_param_ptr)
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = dateedit_init_ptr->both_rect;

    baseedit_ctrl_param_ptr->is_rect_adaptive = dateedit_init_ptr->is_rect_adaptive;
    baseedit_ctrl_param_ptr->str_max_len = dateedit_init_ptr->str_max_len;
    baseedit_ctrl_param_ptr->type = dateedit_init_ptr->type;
}

/*****************************************************************************/
//  Description : dateedit construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DateEditCtrlConstruct (
    CTRLBASE_OBJ_T              *base_ctrl_ptr,
    CTRLDATEEDIT_INIT_PARAM_T   *dateedit_param_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr = (CTRLDATEEDIT_OBJ_T*) base_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr || PNULL == dateedit_param_ptr)
    {
        return FALSE;
    }

    MMITHEME_GetDateEditTheme (&dateedit_ctrl_ptr->theme);

    //set edit date default is single
    baseedit_ctrl_ptr->style = GUIEDIT_STYLE_SINGLE;

    //set highlight
    VTLBASEEDIT_SetHighlight (
        baseedit_ctrl_ptr, 0,
        (uint16) (baseedit_ctrl_ptr->highlight.start_pos + 1), SETHIGHLIGHT_END);

    //hide cursor
    baseedit_ctrl_ptr->cursor.is_hide = TRUE;

    //set max string length
    baseedit_ctrl_ptr->str_max_len = GUIEDIT_DATE_MAX_LEN;

    //set date is set type
    dateedit_ctrl_ptr->year_range = GUIEDIT_DATE_YEAR_RANGE_SET;

    CheckDateValid (dateedit_ctrl_ptr);

    BASEEDIT_Init(baseedit_ctrl_ptr);

    return (result);
}

/*****************************************************************************/
//  Description : dateedit destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DateEditCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLDATEEDIT_OBJ_T     *dateedit_ctrl_ptr = (CTRLDATEEDIT_OBJ_T*) base_ctrl_ptr;

    if (PNULL == dateedit_ctrl_ptr)
    {
        return FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle dateedit msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DateEditCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = PNULL;
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr = (CTRLDATEEDIT_OBJ_T*) base_ctrl_ptr;

    if (PNULL == dateedit_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_APP_UP:
    case MSG_KEYREPEAT_UP:
        result = HandleDateEditUpDownKey (dateedit_ctrl_ptr, TRUE);
        break;

    case MSG_APP_DOWN:
    case MSG_KEYREPEAT_DOWN:
        result = HandleDateEditUpDownKey (dateedit_ctrl_ptr, FALSE);
        break;

    case MSG_APP_LEFT:
    case MSG_KEYREPEAT_LEFT:
        result = HandleDateEditLeftKey (dateedit_ctrl_ptr);
        break;

    case MSG_APP_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
        result = HandleDateEditRightKey (dateedit_ctrl_ptr);
        break;

    case MSG_NOTIFY_IM_COMMIT:
        {
            GUIEDIT_ADD_STR_T   add_str = {0};

            BASEEDIT_ConvertImCommitMsg(param, &add_str);

            //Date,Time,IP need handle number key msg
            HandleDateImCommit (dateedit_ctrl_ptr, &add_str);
        }
        break;

    default:
        parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_DATEEDIT_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
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
LOCAL GUI_ALIGN_E DateEditCtrlGetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr   //in:
)
{
    CTRLDATEEDIT_OBJ_T* dateedit_ctrl_ptr = (CTRLDATEEDIT_OBJ_T*) baseedit_ctrl_ptr;

    return dateedit_ctrl_ptr->theme.align;
}

/*****************************************************************************/
// 	Description : set align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void DateEditCtrlSetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr,  //in:
    GUI_ALIGN_E             align               //in
)
{
    CTRLDATEEDIT_OBJ_T* dateedit_ctrl_ptr = (CTRLDATEEDIT_OBJ_T*) baseedit_ctrl_ptr;

    dateedit_ctrl_ptr->theme.align = align;
}

/*****************************************************************************/
// 	Description : get im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void DateEditCtrlGetImInfo (
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
    CTRLDATEEDIT_OBJ_T* dateedit_ctrl_ptr = (CTRLDATEEDIT_OBJ_T*) baseedit_ctrl_ptr;

    *is_forbid_ptr    = dateedit_ctrl_ptr->theme.im.is_forbid_symbol;
    *init_input_ptr   = dateedit_ctrl_ptr->theme.im.init_input_type;
    *allow_input_ptr  = dateedit_ctrl_ptr->theme.im.allow_input_type;
    *init_im_ptr      = dateedit_ctrl_ptr->theme.im.init_im;
    *allow_im_ptr     = dateedit_ctrl_ptr->theme.im.allow_im;
    *capital_mode_ptr = dateedit_ctrl_ptr->theme.im.capital_mode;
    *tag_ptr          = dateedit_ctrl_ptr->theme.im.tag;
}

/*****************************************************************************/
// 	Description : set im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void DateEditCtrlSetImInfo (
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
    CTRLDATEEDIT_OBJ_T* dateedit_ctrl_ptr = (CTRLDATEEDIT_OBJ_T*) baseedit_ctrl_ptr;

    if (PNULL != is_forbid_ptr)
    {
        dateedit_ctrl_ptr->theme.im.is_forbid_symbol = *is_forbid_ptr;
    }

    if (PNULL != init_im_ptr)
    {
        dateedit_ctrl_ptr->theme.im.init_im = *init_im_ptr;
    }

    if (PNULL != allow_im_ptr)
    {
        dateedit_ctrl_ptr->theme.im.allow_im = *allow_im_ptr;
    }

    if (PNULL != init_type_ptr)
    {
        dateedit_ctrl_ptr->theme.im.init_input_type = *init_type_ptr;
    }

    if (PNULL != allow_type_ptr)
    {
        dateedit_ctrl_ptr->theme.im.allow_input_type = *allow_type_ptr;
    }

    if (PNULL != capital_mode_ptr)
    {
        dateedit_ctrl_ptr->theme.im.capital_mode = *capital_mode_ptr;
    }

    if (PNULL != tag_ptr)
    {
        dateedit_ctrl_ptr->theme.im.tag = *tag_ptr;
    }
}

/*****************************************************************************/
//  Description : 设置横屏时的新edit
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DateEditCtrlSetLandscapeEdit (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    MMI_HANDLE_T        landscape_edit          // [in] the dst landscape edit
)
{
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr = (CTRLDATEEDIT_OBJ_T*) baseedit_ctrl_ptr;
    CTRLDATEEDIT_OBJ_T  *new_edit_ctrl_ptr = GetDateEditPtr (landscape_edit);
    CTRLBASEEDIT_OBJ_T  *new_baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)new_edit_ctrl_ptr;

    if (PNULL != new_edit_ctrl_ptr)
    {
        //set date
        CTRLDATEEDIT_SetDate (
                landscape_edit,
                dateedit_ctrl_ptr->year,
                dateedit_ctrl_ptr->month,
                dateedit_ctrl_ptr->day);

        //set date style
        CTRLDATEEDIT_SetDateStyle (
                landscape_edit,
                (GUIEDIT_SEPARATOR_E *) &dateedit_ctrl_ptr->theme.separator_style,
                (GUIEDIT_DATE_STYLE_E *) &dateedit_ctrl_ptr->theme.date_style,
                FALSE);

        //set range
        CTRLDATEEDIT_SetDateRange (landscape_edit, dateedit_ctrl_ptr->year_range);

        //set im
        new_edit_ctrl_ptr->theme.im = dateedit_ctrl_ptr->theme.im;

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
LOCAL BOOLEAN DateEditCtrlIsValidOffset (           // [ret] TRUE if char offset is valid
    CTRLBASEFIXED_OBJ_T     *basefixed_ctrl_ptr,    // [in] ctrl ptr
    uint16                  *char_offset_ptr,       // [in/out] char offset, and may change the offset if valid
    uint16                  *highlight_len_ptr      // [out] highlight length if char valid
)
{
    BOOLEAN             result = TRUE;
    wchar               separator_char = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) basefixed_ctrl_ptr;
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr = (CTRLDATEEDIT_OBJ_T*) basefixed_ctrl_ptr;

    separator_char = BASEEDIT_GetSeparatorChar ( (GUIEDIT_SEPARATOR_E) dateedit_ctrl_ptr->theme.separator_style);

    if (separator_char == baseedit_ctrl_ptr->str_ptr[*char_offset_ptr])
    {
        result = FALSE;
    }

    *highlight_len_ptr = 1;

    return result;
}

/*****************************************************************************/
//  Description : get date string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void DateEditCtrlConvertToStr (
    CTRLBASEFIXED_OBJ_T     *basefixed_ctrl_ptr
)
{
    uint8       year_thousand = 0;
    uint8       year_hundred = 0;
    uint8       year_ten = 0;
    uint8       year_one = 0;
    uint8       month_ten = 0;
    uint8       month_one = 0;
    uint8       day_ten = 0;
    uint8       day_one = 0;
    char        separator_char = 0;
    uint16      i = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) basefixed_ctrl_ptr;
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr = (CTRLDATEEDIT_OBJ_T*) basefixed_ctrl_ptr;

    if ( (PNULL != basefixed_ctrl_ptr) &&
            (0 == baseedit_ctrl_ptr->str_len))
    {
        //get separator
        separator_char = BASEEDIT_GetSeparatorChar ( (GUIEDIT_SEPARATOR_E) dateedit_ctrl_ptr->theme.separator_style);

        CheckDateValid (dateedit_ctrl_ptr);

        //calculate one,ten,hundred bit etc.
        year_thousand   = (uint8) (dateedit_ctrl_ptr->year / 1000);
        year_hundred    = (uint8) ( (dateedit_ctrl_ptr->year % 1000) / 100);
        year_ten        = (uint8) ( (dateedit_ctrl_ptr->year % 100) / 10);
        year_one        = (uint8) (dateedit_ctrl_ptr->year % 10);
        month_ten       = (uint8) (dateedit_ctrl_ptr->month / 10);
        month_one       = (uint8) (dateedit_ctrl_ptr->month % 10);
        day_ten         = (uint8) (dateedit_ctrl_ptr->day / 10);
        day_one         = (uint8) (dateedit_ctrl_ptr->day % 10);

        //convert date to string
        switch (dateedit_ctrl_ptr->theme.date_style)
        {
        case GUIEDIT_DATE_STYLE_YMD:
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_thousand + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_hundred + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_one + '0');

            baseedit_ctrl_ptr->str_ptr[i++] = separator_char;

            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (month_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (month_one + '0');

            baseedit_ctrl_ptr->str_ptr[i++] = separator_char;

            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (day_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (day_one + '0');
            break;

        case GUIEDIT_DATE_STYLE_YDM:
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_thousand + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_hundred + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_one + '0');

            baseedit_ctrl_ptr->str_ptr[i++] = separator_char;

            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (day_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (day_one + '0');

            baseedit_ctrl_ptr->str_ptr[i++] = separator_char;

            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (month_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (month_one + '0');
            break;

        case GUIEDIT_DATE_STYLE_MYD:
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (month_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (month_one + '0');

            baseedit_ctrl_ptr->str_ptr[i++] = separator_char;

            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_thousand + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_hundred + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_one + '0');

            baseedit_ctrl_ptr->str_ptr[i++] = separator_char;

            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (day_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (day_one + '0');
            break;

        case GUIEDIT_DATE_STYLE_MDY:
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (month_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (month_one + '0');

            baseedit_ctrl_ptr->str_ptr[i++] = separator_char;

            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (day_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (day_one + '0');

            baseedit_ctrl_ptr->str_ptr[i++] = separator_char;

            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_thousand + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_hundred + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_one + '0');
            break;

        case GUIEDIT_DATE_STYLE_DYM:
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (day_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (day_one + '0');

            baseedit_ctrl_ptr->str_ptr[i++] = separator_char;

            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_thousand + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_hundred + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_one + '0');

            baseedit_ctrl_ptr->str_ptr[i++] = separator_char;

            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (month_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (month_one + '0');
            break;

        case GUIEDIT_DATE_STYLE_DMY:
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (day_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (day_one + '0');

            baseedit_ctrl_ptr->str_ptr[i++] = separator_char;

            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (month_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (month_one + '0');

            baseedit_ctrl_ptr->str_ptr[i++] = separator_char;

            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_thousand + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_hundred + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_ten + '0');
            baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (year_one + '0');
            break;

        default:
            SCI_PASSERT (FALSE, ("GUIEDIT_GetDateString:date style %d is error!", dateedit_ctrl_ptr->theme.date_style)); /*assert verified*/
            break;
        }

        baseedit_ctrl_ptr->str_len = i;
        SCI_ASSERT (i == baseedit_ctrl_ptr->str_max_len); /*assert verified*/
    }
}

/*****************************************************************************/
//  Description : get dateedit pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLDATEEDIT_OBJ_T* GetDateEditPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        if (!DateEditTypeOf (ctrl_ptr))
        {
            SCI_TRACE_LOW("== GetDateEditPtr error == 0x%x", ctrl_ptr);
            ctrl_ptr = PNULL;
        }
    }

    return (CTRLDATEEDIT_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN DateEditTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf ( (OBJECT_TYPE_PTR) ctrl_ptr, CTRL_DATEEDIT_TYPE);
}

/*****************************************************************************/
//  Description : handle date edit up/down key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDateEditUpDownKey (
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr,
    BOOLEAN             is_add
)
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)dateedit_ctrl_ptr;

    if (baseedit_ctrl_ptr->common_theme.is_cir_handle_ud)
    {
        //get new date
        if (GetNewDate (dateedit_ctrl_ptr, FALSE, is_add, 0))
        {
            //reset length,get new date string
            baseedit_ctrl_ptr->str_len = 0;

            //update date display
            VTLBASEEDIT_DisplayAll (baseedit_ctrl_ptr, FALSE);
        }

        result = MMI_RESULT_TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle date edit up/down/number key,get new date and date string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNewDate (//up/down:is change;number:is valid
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr,
    BOOLEAN            is_num,     //TRUE:number key:FALSE:up/down key
    BOOLEAN            is_add,
    uint8              num_value   //from 0 to 9
)
{
    BOOLEAN             result = FALSE;
    GUIEDIT_DATE_BIT_E  date_bit = GUIEDIT_DATE_BIT_NULL;

    //get cursor position corresponding bit
    date_bit = GetCurDateBit (dateedit_ctrl_ptr);

    //kevin.lou modified:delete assert
    //SCI_ASSERT(GUIEDIT_DATE_BIT_NULL != date_bit); /*assert verified*/
    if (GUIEDIT_DATE_BIT_NULL == date_bit)
    {
        return FALSE;
    }

    switch (date_bit)
    {
    case GUIEDIT_DATE_BIT_Y_THOUSAND:
    case GUIEDIT_DATE_BIT_Y_HUNDRED:
    case GUIEDIT_DATE_BIT_Y_TEN:
    case GUIEDIT_DATE_BIT_Y_ONE:
        //get new year
        if (GetNewDateYear (dateedit_ctrl_ptr, is_num, is_add, num_value, date_bit))
        {
            //adjust day
            dateedit_ctrl_ptr->day = AdjustDateDay (
                    dateedit_ctrl_ptr->year,
                    dateedit_ctrl_ptr->month,
                    dateedit_ctrl_ptr->day);

            result = TRUE;
        }
        break;

    case GUIEDIT_DATE_BIT_M_TEN:
    case GUIEDIT_DATE_BIT_M_ONE:
        //get new month
        if (GetNewDateMonth (dateedit_ctrl_ptr, is_num, is_add, num_value, date_bit))
        {
            //adjust day
            dateedit_ctrl_ptr->day = AdjustDateDay (
                    dateedit_ctrl_ptr->year,
                    dateedit_ctrl_ptr->month,
                    dateedit_ctrl_ptr->day);

            result = TRUE;
        }
        break;

    case GUIEDIT_DATE_BIT_D_TEN:
    case GUIEDIT_DATE_BIT_D_ONE:
        //get new day
        result = GetNewDateDay (dateedit_ctrl_ptr, is_num, is_add, num_value, date_bit);
        break;

    default:
        //SCI_TRACE_LOW:"GetNewDate: date bit %d is error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_DATE_758_112_2_18_3_16_1_129, (uint8*) "d", date_bit);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get current date bit
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIEDIT_DATE_BIT_E GetCurDateBit (
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr
)
{
    GUIEDIT_DATE_BIT_E  date_bit = GUIEDIT_DATE_BIT_NULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)dateedit_ctrl_ptr;

    switch (dateedit_ctrl_ptr->theme.date_style)
    {
    case GUIEDIT_DATE_STYLE_YMD:
        date_bit = s_guiedit_date_ymd_bit[baseedit_ctrl_ptr->cursor.cur_pos];
        break;

    case GUIEDIT_DATE_STYLE_YDM:
        date_bit = s_guiedit_date_ydm_bit[baseedit_ctrl_ptr->cursor.cur_pos];
        break;

    case GUIEDIT_DATE_STYLE_MYD:
        date_bit = s_guiedit_date_myd_bit[baseedit_ctrl_ptr->cursor.cur_pos];
        break;

    case GUIEDIT_DATE_STYLE_MDY:
        date_bit = s_guiedit_date_mdy_bit[baseedit_ctrl_ptr->cursor.cur_pos];
        break;

    case GUIEDIT_DATE_STYLE_DYM:
        date_bit = s_guiedit_date_dym_bit[baseedit_ctrl_ptr->cursor.cur_pos];
        break;

    case GUIEDIT_DATE_STYLE_DMY:
        date_bit = s_guiedit_date_dmy_bit[baseedit_ctrl_ptr->cursor.cur_pos];
        break;

    default:
        //SCI_TRACE_LOW:"GetCurDateBit:date style %d is error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_DATE_802_112_2_18_3_16_2_130, (uint8*) "d", dateedit_ctrl_ptr->theme.date_style);
        break;
    }

    return (date_bit);
}

/*****************************************************************************/
//  Description : handle date edit up/down/number key,get new date year
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNewDateYear (//up/down:is change;number:is valid
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr,
    BOOLEAN             is_num,     //TRUE:number key:FALSE:up/down key
    BOOLEAN             is_add,
    uint8               num_value,  //from 0 to 9
    GUIEDIT_DATE_BIT_E  date_bit
)
{
    BOOLEAN     result = FALSE;
    uint8       bit_value = 0;
    uint8       bit_min = 0;
    uint8       bit_max = 0;
    uint8       year_thousand = 0;
    uint8       year_hundred = 0;
    uint8       year_ten = 0;
    uint16      min_year = 0;
    uint16      max_year = 0;
    uint16      unit_value = 0;
    uint16      new_year = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)dateedit_ctrl_ptr;

    //get min and max year
    MMITHEME_GetEditDateRange (&min_year, &max_year, dateedit_ctrl_ptr->year_range);

    //set year thousand,hundred bit value
    year_thousand = (uint8) (dateedit_ctrl_ptr->year / 1000);
    year_hundred  = (uint8) ((dateedit_ctrl_ptr->year % 1000) / 100);
    year_ten      = (uint8) ((dateedit_ctrl_ptr->year % 100) / 10);

    //set bit value
    bit_value = (uint8) (baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos] - '0');

    switch (date_bit)
    {
    case GUIEDIT_DATE_BIT_Y_THOUSAND:
        //set unit and new year
        unit_value = 1000;
        new_year   = (uint16) (dateedit_ctrl_ptr->year % unit_value);

        //set bit min and max
        bit_min = (uint8) (min_year / unit_value);
        bit_max = (uint8) (max_year / unit_value);
        break;

    case GUIEDIT_DATE_BIT_Y_HUNDRED:
        //set unit and new year
        unit_value = 100;
        new_year   = (uint16) (year_thousand * 1000 + dateedit_ctrl_ptr->year % unit_value);

        //set bit min and max
        if (year_thousand > min_year / 1000)
        {
            bit_min = 0;
        }
        else
        {
            bit_min = (uint8) ( (min_year % 1000) / 100);
        }

        if (year_thousand < max_year / 1000)
        {
            bit_max = 9;
        }
        else
        {
            bit_max = (uint8) ( (max_year % 1000) / 100);
        }
        break;

    case GUIEDIT_DATE_BIT_Y_TEN:
        //set unit and new year
        unit_value = 10;
        new_year   = (uint16) (year_thousand * 1000 + year_hundred * 100 + dateedit_ctrl_ptr->year % unit_value);

        //set bit min and max
        if (year_thousand*10 + year_hundred > min_year / 100)
        {
            bit_min = 0;
        }
        else
        {
            bit_min = (uint8) ( (min_year % 100) / 10);
        }

        if (year_thousand*10 + year_hundred < max_year / 100)
        {
            bit_max = 9;
        }
        else
        {
            bit_max = (uint8) ( (max_year % 100) / 10);
        }
        break;

    case GUIEDIT_DATE_BIT_Y_ONE:
        //set unit and new year
        unit_value = 1;
        new_year   = (uint16) (dateedit_ctrl_ptr->year - dateedit_ctrl_ptr->year % 10);

        //set bit min and max
        if (year_thousand*100 + year_hundred*10 + year_ten > min_year / 10)
        {
            bit_min = 0;
        }
        else
        {
            bit_min = (uint8) (min_year % 10);
        }

        if (year_thousand*100 + year_hundred*10 + year_ten < max_year / 10)
        {
            bit_max = 9;
        }
        else
        {
            bit_max = (uint8) (max_year % 10);
        }
        break;

    default:
        SCI_PASSERT (FALSE, ("GetNewDateYear: date bit %d is error!", date_bit)); /*assert verified*/
        break;
    }

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
        //get bit value
        bit_value = BASEFIXED_CalculateBitValue (is_add, bit_value, bit_min, bit_max);
    }

    //get new year
    new_year = (uint16) (new_year + bit_value * unit_value);

    //adjust year
    if (new_year < min_year)
    {
        new_year = min_year;
    }

    if (new_year > max_year)
    {
        new_year = max_year;
    }

    //set new year
    if (new_year != dateedit_ctrl_ptr->year)
    {
        dateedit_ctrl_ptr->year = new_year;

        if (!is_num)
        {
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : adjust day by year and month
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint8 AdjustDateDay (
    uint16	year,
    uint8	month,
    uint8	day
)
{
    SCI_ASSERT (0 < month); /*assert verified*/

    //is leap year
    if (GUI_IsLeapYear ( (uint32) year))
    {
        if (30 == s_guiedit_date_leap_month_days[month - 1])
        {
            if (30 < day)
            {
                day = 30;
            }
        }
        else if (29 == s_guiedit_date_leap_month_days[month - 1])
        {
            if (29 < day)
            {
                day = 29;
            }
        }
    }
    else
    {
        if (30 == s_guiedit_date_month_days[month - 1])
        {
            if (30 < day)
            {
                day = 30;
            }
        }
        else if (28 == s_guiedit_date_month_days[month - 1])
        {
            if (28 < day)
            {
                day = 28;
            }
        }
    }

    return day;
}

/*****************************************************************************/
//  Description : handle date edit up/down/number key,get new date month
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNewDateMonth (//up/down:is change;number:is valid
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr,
    BOOLEAN             is_num,     //TRUE:number key:FALSE:up/down key
    BOOLEAN             is_add,
    uint8               num_value,  //from 0 to 9
    GUIEDIT_DATE_BIT_E  date_bit
)
{
    BOOLEAN     result = FALSE;
    uint8       month_ten = 0;
    uint8       month_one = 0;
    uint8       new_month = 0;
    uint8       bit_min = 0;
    uint8       bit_max = 0;
    uint8       bit_value = 0;
    uint8       unit_value = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)dateedit_ctrl_ptr;

    //get month ten and one
    month_ten = (uint8) (dateedit_ctrl_ptr->month / 10);
    month_one = (uint8) (dateedit_ctrl_ptr->month % 10);

    //set bit value
    bit_value = (uint8) (baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos] - '0');

    switch (date_bit)
    {
    case GUIEDIT_DATE_BIT_M_TEN:
        //set unit and new month
        unit_value = 10;
        new_month = month_one;

        //set bit min and max
        bit_min = 0;
        bit_max = 1;
        break;

    case GUIEDIT_DATE_BIT_M_ONE:
        //set unit and new month
        unit_value = 1;
        new_month = (uint8) (month_ten * 10);

        if (0 == month_ten)
        {
            //set bit min and max
            bit_min = 1;
            bit_max = 9;
        }
        else
        {
            SCI_ASSERT (1 == month_ten); /*assert verified*/
            //set bit min and max
            bit_min = 0;
            bit_max = 2;
        }

        break;

    default:
        SCI_PASSERT (FALSE, ("GetNewDateMonth: date bit %d is error!", date_bit)); /*assert verified*/
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

    //get new month
    new_month = (uint8) (new_month + bit_value * unit_value);

    //adjust year
    if (new_month < 1)
    {
        new_month = 1;
    }

    if (new_month > 12)
    {
        new_month = 12;
    }

    //set new month
    if (new_month != dateedit_ctrl_ptr->month)
    {
        dateedit_ctrl_ptr->month = new_month;

        if (!is_num)
        {
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle date edit up/down key,get new date day
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNewDateDay (//up/down:is change;number:is valid
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr,
    BOOLEAN             is_num,     //TRUE:number key:FALSE:up/down key
    BOOLEAN             is_add,
    uint8               num_value,  //from 0 to 9
    GUIEDIT_DATE_BIT_E  date_bit
)
{
    BOOLEAN     result = FALSE;
    uint8       day_ten = 0;
    uint8       day_one = 0;
    uint8       new_day = 0;
    uint8       max_day = 0;
    uint8       bit_min = 0;
    uint8       bit_max = 0;
    uint8       bit_value = 0;
    uint8       unit_value = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)dateedit_ctrl_ptr;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(0 < dateedit_ctrl_ptr->month); /*assert verified*/
    if (0 == dateedit_ctrl_ptr->month)
    {
        return FALSE;
    }

    //get day ten and one
    day_ten = (uint8) (dateedit_ctrl_ptr->day / 10);
    day_one = (uint8) (dateedit_ctrl_ptr->day % 10);

    //set bit value
    bit_value = (uint8) (baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos] - '0');

    switch (date_bit)
    {
    case GUIEDIT_DATE_BIT_D_TEN:
        //set unit and new month
        unit_value = 10;
        new_day = day_one;

        //set bit min and max
        if (2 == dateedit_ctrl_ptr->month)
        {
            bit_min = 0;
            bit_max = 2;
        }
        else
        {
            bit_min = 0;
            bit_max = 3;
        }
        break;

    case GUIEDIT_DATE_BIT_D_ONE:
        //set unit and new month
        unit_value = 1;
        new_day = (uint8) (day_ten * 10);

        //set bit min and max
        bit_min = 0;
        bit_max = 9;

        if (0 == day_ten)
        {
            bit_min = 1;
        }
        else if (2 == day_ten)
        {
            if ( (!GUI_IsLeapYear (dateedit_ctrl_ptr->year)) &&
                    (2 == dateedit_ctrl_ptr->month))
            {
                bit_max = 8;
            }
        }
        else if (3 == day_ten)
        {
            if (31 == s_guiedit_date_month_days[dateedit_ctrl_ptr->month - 1])
            {
                bit_max = 1;
            }
            else
            {
                bit_max = 0;
            }
        }
        break;

    default:
        SCI_PASSERT (FALSE, ("GetNewDateDay: date bit %d is error!", date_bit)); /*assert verified*/
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

    //get new month
    new_day = (uint8) (new_day + bit_value * unit_value);

    //adjust month
    if (new_day < 1)
    {
        new_day = 1;
    }

    if (GUI_IsLeapYear (dateedit_ctrl_ptr->year))
    {
        max_day = s_guiedit_date_leap_month_days[dateedit_ctrl_ptr->month - 1];
    }
    else
    {
        max_day = s_guiedit_date_month_days[dateedit_ctrl_ptr->month - 1];
    }

    if (new_day > max_day)
    {
        new_day = max_day;
    }

    //set new month
    if (new_day != dateedit_ctrl_ptr->day)
    {
        dateedit_ctrl_ptr->day = new_day;

        if (!is_num)
        {
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle date edit left key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDateEditLeftKey (
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)dateedit_ctrl_ptr;

    if ( (0 == baseedit_ctrl_ptr->cursor.cur_pos) &&
            (!baseedit_ctrl_ptr->common_theme.is_cir_handle_lf))
    {
        result = MMI_RESULT_FALSE;
    }
    else
    {
        //reset highlight not display
        BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, FALSE);

        //reset date cursor not display
        //BASEEDIT_ResetCursor (baseedit_ctrl_ptr);

        //set current cursor position
        if (0 < baseedit_ctrl_ptr->cursor.cur_pos)
        {
            VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, (uint16) (baseedit_ctrl_ptr->cursor.cur_pos - 1));

            //if is separator
            if (BASEEDIT_GetSeparatorChar ( (GUIEDIT_SEPARATOR_E) dateedit_ctrl_ptr->theme.separator_style) == baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos])
            {
                VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, (uint16) (baseedit_ctrl_ptr->cursor.cur_pos - 1));
            }
        }
        else
        {
            VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, (uint16) (baseedit_ctrl_ptr->str_len - 1));
        }

        //set highlight
        VTLBASEEDIT_SetHighlight (
            baseedit_ctrl_ptr,
            baseedit_ctrl_ptr->cursor.cur_pos,
            (uint16) (baseedit_ctrl_ptr->cursor.cur_pos + 1),
            SETHIGHLIGHT_BOTH);

        //display highlight
        BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, TRUE);

        //set display cursor
        baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle date edit right key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDateEditRightKey (
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)dateedit_ctrl_ptr;

    if ( (baseedit_ctrl_ptr->cursor.cur_pos == (baseedit_ctrl_ptr->str_len - 1)) &&
            (!baseedit_ctrl_ptr->common_theme.is_cir_handle_lf))
    {
        result = MMI_RESULT_FALSE;
    }
    else
    {
        //reset highlight not display
        BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, FALSE);

        //reset date cursor not display
        //BASEEDIT_ResetCursor (baseedit_ctrl_ptr);

        //set current cursor position
        MoveDateCursorToRight (dateedit_ctrl_ptr);

        //display highlight
        BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, TRUE);

        //set display cursor
        baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : move date cursor to right
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MoveDateCursorToRight (
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)dateedit_ctrl_ptr;

    if (baseedit_ctrl_ptr->cursor.cur_pos < (baseedit_ctrl_ptr->str_len - 1))
    {
        VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, (uint16) (baseedit_ctrl_ptr->cursor.cur_pos + 1));

        //if is separator
        if (BASEEDIT_GetSeparatorChar ( (GUIEDIT_SEPARATOR_E) dateedit_ctrl_ptr->theme.separator_style) == baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos])
        {
            VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, (uint16) (baseedit_ctrl_ptr->cursor.cur_pos + 1));
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
        (uint16) (baseedit_ctrl_ptr->cursor.cur_pos + 1),
        SETHIGHLIGHT_BOTH);
}

/*****************************************************************************/
//  Description : date handle im commit msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleDateImCommit (
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr,
    GUIEDIT_ADD_STR_T   *add_str_ptr
)
{
    uint8   num_value = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)dateedit_ctrl_ptr;

    //get num value
    if (BASEFIXED_GetNumValueByIm (&num_value, add_str_ptr))
    {
        //get new date
        if (GetNewDate (dateedit_ctrl_ptr, TRUE, FALSE, num_value))
        {
            //set current cursor position
            MoveDateCursorToRight (dateedit_ctrl_ptr);

            //reset length,get new date string
            baseedit_ctrl_ptr->str_len = 0;

            //update date display
            VTLBASEEDIT_DisplayAll (baseedit_ctrl_ptr, FALSE);
        }
    }
}

/*****************************************************************************/
//  Description : check date valid
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void CheckDateValid (
    CTRLDATEEDIT_OBJ_T  *dateedit_ctrl_ptr
)
{
    uint16          min_year = 0;
    uint16          max_year = 0;

    MMITHEME_GetEditDateRange (&min_year, &max_year, dateedit_ctrl_ptr->year_range);

    if (dateedit_ctrl_ptr->year < min_year)
    {
        dateedit_ctrl_ptr->year = min_year;
    }
    else if (dateedit_ctrl_ptr->year > max_year)
    {
        dateedit_ctrl_ptr->year = max_year;
    }

    if (dateedit_ctrl_ptr->month < 1)
    {
        dateedit_ctrl_ptr->month = 1;
    }
    else if (dateedit_ctrl_ptr->month > 12)
    {
        dateedit_ctrl_ptr->month = 12;
    }

    if (dateedit_ctrl_ptr->day < 1)
    {
        dateedit_ctrl_ptr->day = 1;
    }
    else
    {
        dateedit_ctrl_ptr->day = AdjustDateDay (
                dateedit_ctrl_ptr->year,
                dateedit_ctrl_ptr->month,
                dateedit_ctrl_ptr->day);
    }
}

