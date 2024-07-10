/*****************************************************************************
** File Name:      ctrldatepicker.c                                               *
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

#include "ctrldatepicker.h"

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
//  Description : init datepicker class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DatepickerCtrlInitVtbl (
    CTRLDATEPICKER_VTBL_T        *datepicker_vtbl_ptr
);

/*****************************************************************************/
//  Description : datepicker construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DatepickerCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLDATEPICKER_INIT_PARAM_T   *datepicker_param_ptr
);

/*****************************************************************************/
//  Description : datepicker destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DatepickerCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DatepickerCtrlPackInitParam (
    GUIDATEPICKER_INIT_DATA_T     *datepicker_init_ptr,   //in
    CTRLDATEPICKER_INIT_PARAM_T   *datepicker_param_ptr   //out
);

#ifdef TOUCH_PANEL_SUPPORT

/*****************************************************************************/
//  Description : handle timepicker msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DatePickerCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : ��ȡ��ʼ���������(��������ֵ)
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DatePickerCtrlGetFieldRange (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    int32                  *start_field_ptr,       // [out]
    int32                  *end_field_ptr          // [out]
);

/*****************************************************************************/
//  Description : ��ȡָ�������ֵ
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DatePickerCtrlGetValueRange (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,// [in]
    CTRLBASEPICKER_FIELD_T      field,               // [in]
    uint16                      *min_value_ptr,      // [out]
    uint16                      *max_value_ptr       // [out]
);

/*****************************************************************************/
//  Description : ��ȡָ����������ʾλ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T DatePickerCtrlGetFieldRect (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,   // [in]
    int32                   field                   // [in]
);

/*****************************************************************************/
//  Description : ��ȡ���е�����
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DatePickerCtrlGetFieldText (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T      field,                  // [in]
    uint16                      cur_index,              // [in]
    MMI_STRING_T                *dis_string_ptr         // [out]
);

/*****************************************************************************/
//  Description : ��ȡ��ʾ�ı�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T DatePickerCtrlGetFieldTips (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    int32                  field                   // [in]
);

/*****************************************************************************/
//  Description : ��������Ƿ�Ϸ�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DatePickerCtrlCheckDate (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    BOOLEAN                is_need_anim            // [in] �Ƿ���Ҫ���ƶ���
);

/*****************************************************************************/
//  Description : adjust day by year and month
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint8 CheckDayValue (
    uint16	year,
    uint8	month,
    uint8	day
);

/*****************************************************************************/
//  Description : get datepicker pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLDATEPICKER_OBJ_T* GetDatePickerPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN DatePickerTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);
#endif

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get datepicker type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T DATEPICKER_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASEPICKER_TYPE,                                 // parent class type
                       "datepicker",                                         // control name
                       sizeof (CTRLDATEPICKER_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) DatepickerCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) DatepickerCtrlDestruct,      // control destructor function
                       sizeof (CTRLDATEPICKER_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) DatepickerCtrlPackInitParam,     // convert init param
                       sizeof (CTRLDATEPICKER_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) DatepickerCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init datepicker class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DatepickerCtrlInitVtbl (
    CTRLDATEPICKER_VTBL_T        *datepicker_vtbl_ptr
)
{
#ifdef TOUCH_PANEL_SUPPORT
    CTRLBASE_VTBL_T         *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) datepicker_vtbl_ptr;
    CTRLBASEPICKER_VTBL_T   *basepicker_vtbl_ptr = (CTRLBASEPICKER_VTBL_T*) datepicker_vtbl_ptr;

    if (PNULL == basepicker_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = DatePickerCtrlHandleMsg;

    basepicker_vtbl_ptr->GetFieldRange = DatePickerCtrlGetFieldRange;
    basepicker_vtbl_ptr->GetValueRange = DatePickerCtrlGetValueRange;
    basepicker_vtbl_ptr->GetFieldRect = DatePickerCtrlGetFieldRect;
    basepicker_vtbl_ptr->GetFieldText = DatePickerCtrlGetFieldText;
    basepicker_vtbl_ptr->GetFieldTips = DatePickerCtrlGetFieldTips;
    basepicker_vtbl_ptr->CheckValue = DatePickerCtrlCheckDate;
#endif

    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
// �÷������ڽ��ؼ��Ķ����ʼ�������ṹת��Ϊ�ڲ��ĳ�ʼ�������ṹ
// ���ڿؼ��м̳й�ϵ���ؼ���ʼ��ʱ�ִ��в����������ڲ��ĳ�ʼ������Ҳ�����м̳й�ϵ
/*****************************************************************************/
LOCAL void DatepickerCtrlPackInitParam (
    GUIDATEPICKER_INIT_DATA_T     *datepicker_init_ptr,   //in
    CTRLDATEPICKER_INIT_PARAM_T   *datepicker_param_ptr   //out
)
{
#ifdef TOUCH_PANEL_SUPPORT
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) datepicker_param_ptr;

    if (PNULL == datepicker_init_ptr || PNULL == datepicker_param_ptr)
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = datepicker_init_ptr->both_rect;
#endif
}

/*****************************************************************************/
//  Description : datepicker construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DatepickerCtrlConstruct (
    CTRLBASE_OBJ_T                  *base_ctrl_ptr,
    CTRLDATEPICKER_INIT_PARAM_T     *datepicker_param_ptr
)
{
#ifdef TOUCH_PANEL_SUPPORT
    CTRLDATEPICKER_OBJ_T    *datepicker_ctrl_ptr = (CTRLDATEPICKER_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr || PNULL == datepicker_param_ptr)
    {
        return FALSE;
    }

    MMITHEME_GetDatePickerTheme (&datepicker_ctrl_ptr->theme);
#endif

    return TRUE;
}

/*****************************************************************************/
//  Description : datepicker destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DatepickerCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    return TRUE;
}

/*****************************************************************************/
//  Description : set date
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLDATEPICKER_SetDate (
    MMI_CTRL_ID_T   ctrl_id,            // [in]
    uint16          year,               // [in]
    uint8           month,              // [in]
    uint8           day                 // [in]
)
{
#ifdef TOUCH_PANEL_SUPPORT
    int32                   i = 0;
    int32                   start_field = 0;
    int32                   end_field   = 0;
    uint16                  value[3] = {0};
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = PNULL;

    basepicker_ctrl_ptr = (CTRLBASEPICKER_OBJ_T*) GetDatePickerPtr (ctrl_id);

    if (PNULL != basepicker_ctrl_ptr)
    {
        value[0] = year;
        value[1] = month;
        value[2] = day;

        // ��ȡ��ǰ����£���Ŀ�ʼ�ͽ�����Χ�����з����ͳһ����
        VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

        for (i = start_field; i < end_field; i++)
        {
            if (basepicker_ctrl_ptr->enable_anim)
            {
                BASEPICKER_SetRecoverValue (basepicker_ctrl_ptr, i, value[i - start_field], FALSE);
            }
            else
            {
                BASEPICKER_SetValue (basepicker_ctrl_ptr, i, value[i - start_field]);
            }
        }

        VTLBASEPICKER_CheckValue (basepicker_ctrl_ptr, FALSE);
    }

#endif
}

/*****************************************************************************/
//  Description : get date
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLDATEPICKER_GetDate (
    MMI_CTRL_ID_T   ctrl_id,        // [in]
    uint16          *year_ptr,      // [out] may PNULL
    uint8           *month_ptr,     // [out] may PNULL
    uint8           *day_ptr        // [out] may PNULL
)
{
#ifdef TOUCH_PANEL_SUPPORT
    uint16                  year = 0;
    uint16                  month = 0;
    uint16                  day = 0;
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = PNULL;

    basepicker_ctrl_ptr = (CTRLBASEPICKER_OBJ_T*) GetDatePickerPtr (ctrl_id);

    if (PNULL != basepicker_ctrl_ptr)
    {
        year = BASEPICKER_GetValue (basepicker_ctrl_ptr, MMITHEME_DATEPICKER_FIELD_YEAR);

        if (PNULL != year_ptr)
        {
            *year_ptr = year;
        }

        month = BASEPICKER_GetValue (basepicker_ctrl_ptr, MMITHEME_DATEPICKER_FIELD_MONTH);

        if (PNULL != month_ptr)
        {
            *month_ptr = (uint8) month;
        }

        if (PNULL != day_ptr)
        {
            day = BASEPICKER_GetValue (basepicker_ctrl_ptr, MMITHEME_DATEPICKER_FIELD_DAY);
            day = CheckDayValue (year, month, day);

            *day_ptr = (uint8) day;
        }
    }

#endif
}

/*****************************************************************************/
//  Description : set date range
//  Global resource dependence :
//  Author: hua.fang
//  Note: default is CTRLDATEPICKER_YEAR_RANGE_SET
/*****************************************************************************/
PUBLIC void CTRLDATEPICKER_SetDateRange (
    MMI_CTRL_ID_T                 ctrl_id,           // [in]
    CTRLDATEPICKER_YEAR_RANGE_E   year_range_type    // [in]
)
{
#ifdef TOUCH_PANEL_SUPPORT
    CTRLDATEPICKER_OBJ_T    *datepicker_ctrl_ptr = PNULL;

    datepicker_ctrl_ptr = GetDatePickerPtr (ctrl_id);

    if (PNULL != datepicker_ctrl_ptr)
    {
        datepicker_ctrl_ptr->theme.year_range = year_range_type;
    }

#endif
}

/*****************************************************************************/
//  Description : get date range
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC CTRLDATEPICKER_YEAR_RANGE_E CTRLDATEPICKER_GetDateRange (
    MMI_CTRL_ID_T     ctrl_id  // [in]
)
{
    CTRLDATEPICKER_YEAR_RANGE_E year_range = CTRLDATEPICKER_YEAR_RANGE_NULL;

#ifdef TOUCH_PANEL_SUPPORT
    CTRLDATEPICKER_OBJ_T        *datepicker_ctrl_ptr = PNULL;

    datepicker_ctrl_ptr = GetDatePickerPtr (ctrl_id);

    if (PNULL != datepicker_ctrl_ptr)
    {
        year_range = datepicker_ctrl_ptr->theme.year_range;
    }

#endif

    return year_range;
}

/*****************************************************************************/
//  Description : set date style
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLDATEPICKER_SetDateStyle (
    MMI_CTRL_ID_T           ctrl_id,
    CTRLDATEPICKER_STYLE_E  *date_style_ptr,//may PNULL
    BOOLEAN                 is_update
)
{
#ifdef TOUCH_PANEL_SUPPORT
    CTRLDATEPICKER_OBJ_T    *datepicker_ctrl_ptr = PNULL;

    datepicker_ctrl_ptr = GetDatePickerPtr (ctrl_id);

    if (PNULL != datepicker_ctrl_ptr)
    {
        if (PNULL != date_style_ptr)
        {
            datepicker_ctrl_ptr->theme.date_style = *date_style_ptr;
        }

        if (is_update)
        {
            BASEPICKER_Display ( (CTRLBASEPICKER_OBJ_T*) datepicker_ctrl_ptr);
        }
    }

#endif
}

#ifdef TOUCH_PANEL_SUPPORT

/*****************************************************************************/
//  Description : handle datepicker msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DatePickerCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    CTRLBASE_VTBL_T         *parent_vtbl_ptr = PNULL;

    parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_DATEPICKER_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);

    return parent_vtbl_ptr->HandleEvent (base_ctrl_ptr, msg_id, param);
}

/*****************************************************************************/
//  Description : ��ȡ��ʼ���������(��������ֵ)
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DatePickerCtrlGetFieldRange (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    int32                  *start_field_ptr,       // [out]
    int32                  *end_field_ptr          // [out]
)
{
    *start_field_ptr = MMITHEME_DATEPICKER_FIELD_START;
    *end_field_ptr = MMITHEME_DATEPICKER_FIELD_END;

    SCI_ASSERT (*start_field_ptr <= MMITHEME_BASEPICKER_MAX_FIELD_COUNT     /*assert verified*/
                && *end_field_ptr <= MMITHEME_BASEPICKER_MAX_FIELD_COUNT);
}

/*****************************************************************************/
//  Description : ��ȡָ�������ֵ
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DatePickerCtrlGetValueRange (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,// [in]
    CTRLBASEPICKER_FIELD_T      field,               // [in]
    uint16                      *min_value_ptr,      // [out]
    uint16                      *max_value_ptr       // [out]
)
{
    CTRLDATEPICKER_OBJ_T    *datepicker_ctrl_ptr = (CTRLDATEPICKER_OBJ_T*) basepicker_ctrl_ptr;

    switch (field)
    {
    case MMITHEME_DATEPICKER_FIELD_YEAR:
        MMITHEME_GetDatePickerYearRange (
            datepicker_ctrl_ptr->theme.year_range,
            min_value_ptr, max_value_ptr);
        break;

    case MMITHEME_DATEPICKER_FIELD_MONTH:
        *min_value_ptr = 1;
        *max_value_ptr = 12;
        break;

    case MMITHEME_DATEPICKER_FIELD_DAY:
        *min_value_ptr = 1;
        *max_value_ptr = 31;
        break;

    default:
        SCI_ASSERT (FALSE);     /*assert verified*/
        break;
    }
}

/*****************************************************************************/
//  Description : ��ȡָ����������ʾλ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T DatePickerCtrlGetFieldRect (
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
    CTRLDATEPICKER_OBJ_T    *datepicker_ctrl_ptr = (CTRLDATEPICKER_OBJ_T*) basepicker_ctrl_ptr;

    // ��������¶�Ӧ����ֵ
    int32 range[CTRLDATEPICKER_STYLE_MAX][3] =        // ������CTRLDATEPICKER_STYLE_Eһ��
    {
        {0, 0, 0},
        {MMITHEME_DATEPICKER_FIELD_YEAR, MMITHEME_DATEPICKER_FIELD_MONTH, MMITHEME_DATEPICKER_FIELD_DAY},
        {MMITHEME_DATEPICKER_FIELD_YEAR, MMITHEME_DATEPICKER_FIELD_DAY, MMITHEME_DATEPICKER_FIELD_MONTH},
        {MMITHEME_DATEPICKER_FIELD_MONTH, MMITHEME_DATEPICKER_FIELD_YEAR, MMITHEME_DATEPICKER_FIELD_DAY},
        {MMITHEME_DATEPICKER_FIELD_MONTH, MMITHEME_DATEPICKER_FIELD_DAY, MMITHEME_DATEPICKER_FIELD_YEAR},
        {MMITHEME_DATEPICKER_FIELD_DAY, MMITHEME_DATEPICKER_FIELD_YEAR, MMITHEME_DATEPICKER_FIELD_MONTH},
        {MMITHEME_DATEPICKER_FIELD_DAY, MMITHEME_DATEPICKER_FIELD_MONTH, MMITHEME_DATEPICKER_FIELD_YEAR},
    };

    field_rect = BASEPICKER_GetTextRect (basepicker_ctrl_ptr);

    width = basepicker_ctrl_ptr->theme.client_width;

    // ���� + �ָ���(������������ҵ�)
    VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);
    width -= (basepicker_ctrl_ptr->theme.border_width * 2 + (end_field - start_field + 1) * basepicker_ctrl_ptr->theme.separator_width);

    // �ҵ���Ӧ����ֵ������֮ǰ�Ŀ��
    for (i = 0; i < 3; i++)
    {
        cur_field = range[datepicker_ctrl_ptr->theme.date_style][i];
        cur_percent += datepicker_ctrl_ptr->theme.field_width[cur_field];

        if (cur_field == field)
        {
            break;
        }

        pre_percent += datepicker_ctrl_ptr->theme.field_width[cur_field];
    }

    field_rect.left += width * pre_percent / 100 + i * basepicker_ctrl_ptr->theme.separator_width;
    field_rect.right = field_rect.left + width * cur_percent / 100 - width * pre_percent / 100 - 1;     // ���ܺϲ������������

    return field_rect;
}

/*****************************************************************************/
//  Description : ��ȡ���е�����
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DatePickerCtrlGetFieldText (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T      field,                  // [in]
    uint16                      cur_index,              // [in]
    MMI_STRING_T                *dis_string_ptr         // [out]
)
{
    // ��ʾ���ַ����������������ÿ�ζ���ʼ��
    char    date_str[CTRLBASEPICKER_MAX_STR_LEN] = {0};

    // ��Ϊ2λ����ǰ�油0
    sprintf (date_str, "%02d", cur_index);

    dis_string_ptr->wstr_len = strlen (date_str);

    MMI_STRNTOWSTR (
        dis_string_ptr->wstr_ptr,
        CTRLBASEPICKER_MAX_STR_LEN - 1,
        (const uint8*) date_str,
        dis_string_ptr->wstr_len,
        dis_string_ptr->wstr_len);
}

/*****************************************************************************/
//  Description : ��ȡ��ʾ�ı�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T DatePickerCtrlGetFieldTips (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    int32                  field                   // [in]
)
{
    MMI_TEXT_ID_T           text_id = 0;
    CTRLDATEPICKER_OBJ_T    *datepicker_ctrl_ptr = (CTRLDATEPICKER_OBJ_T*) basepicker_ctrl_ptr;

    if (MMITHEME_DATEPICKER_FIELD_COUNT > field)
    {
        text_id = datepicker_ctrl_ptr->theme.field_tips[field];
    }

    return text_id;
}

/*****************************************************************************/
//  Description : ��������Ƿ�Ϸ�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DatePickerCtrlCheckDate (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    BOOLEAN                is_need_anim            // [in] �Ƿ���Ҫ���ƶ���
)
{
    uint16  year = 0;
    uint16  month = 0;
    uint16  day = 0;
    uint16  old_day = 0;

    year = BASEPICKER_GetValue (basepicker_ctrl_ptr, MMITHEME_DATEPICKER_FIELD_YEAR);
    month = BASEPICKER_GetValue (basepicker_ctrl_ptr, MMITHEME_DATEPICKER_FIELD_MONTH);
    day = BASEPICKER_GetValue (basepicker_ctrl_ptr, MMITHEME_DATEPICKER_FIELD_DAY);
    old_day = day;

    day = CheckDayValue (year, (uint8) month, (uint8) day);

    if (old_day != day)
    {
        if (is_need_anim)
        {
            BASEPICKER_SetRecoverValue (basepicker_ctrl_ptr, MMITHEME_DATEPICKER_FIELD_DAY, day, FALSE);
        }
        else
        {
            BASEPICKER_SetValue (basepicker_ctrl_ptr, MMITHEME_DATEPICKER_FIELD_DAY, day);
        }
    }
}

/*****************************************************************************/
//  Description : adjust day by year and month
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint8 CheckDayValue (
    uint16	year,
    uint8	month,
    uint8	day
)
{
    //ÿ���¶�Ӧ������
    uint8 date_month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    uint8 date_leap_month_days[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    SCI_ASSERT (0 < month && month < 13); /*assert verified*/

    //is leap year
    if (GUI_IsLeapYear ( (uint32) year))
    {
        if (day > date_leap_month_days[month - 1])
        {
            day = date_leap_month_days[month - 1];
        }
    }
    else
    {
        if (day > date_month_days[month - 1])
        {
            day = date_month_days[month - 1];
        }
    }

    return day;
}

/*****************************************************************************/
//  Description : get datepicker pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLDATEPICKER_OBJ_T* GetDatePickerPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT (DatePickerTypeOf (ctrl_ptr)); /*assert verified*/
    }

    return (CTRLDATEPICKER_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN DatePickerTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf ( (OBJECT_TYPE_PTR) ctrl_ptr, CTRL_DATEPICKER_TYPE);
}
#endif

