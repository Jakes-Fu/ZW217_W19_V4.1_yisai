/*****************************************************************************
** File Name:      ctrlpicker.c                                               *
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

#include "ctrlpicker.h"

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
//  Description : init picker class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PickerCtrlInitVtbl (
    CTRLPICKER_VTBL_T        *picker_vtbl_ptr
);

/*****************************************************************************/
//  Description : picker construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PickerCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLPICKER_INIT_PARAM_T   *picker_param_ptr
);

/*****************************************************************************/
//  Description : picker destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PickerCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void PickerCtrlPackInitParam (
    GUIPICKER_INIT_DATA_T     *picker_init_ptr,   //in
    CTRLPICKER_INIT_PARAM_T   *picker_param_ptr   //out
);

#ifdef TOUCH_PANEL_SUPPORT

/*****************************************************************************/
//  Description : handle picker msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E PickerCtrlHandleMsg (
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
LOCAL void PickerCtrlGetFieldRange (
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
LOCAL void PickerCtrlGetValueRange (
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
LOCAL GUI_RECT_T PickerCtrlGetFieldRect (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,   // [in]
    int32                   field                   // [in]
);

/*****************************************************************************/
//  Description : 获取域中的文字
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void PickerCtrlGetFieldText (
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
LOCAL MMI_TEXT_ID_T PickerCtrlGetFieldTips (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    int32                  field                   // [in]
);

/*****************************************************************************/
//  Description : 释放picker风格分配的资源
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void FreePickerContent (
    CTRLPICKER_OBJ_T    *picker_ctrl_ptr
);

/*****************************************************************************/
//  Description : get picker pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLPICKER_OBJ_T* GetPickerPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN PickerTypeOf (
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
//  Description : get picker type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T PICKER_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASEPICKER_TYPE,                             // parent class type
                       "picker",                                         // control name
                       sizeof (CTRLPICKER_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) PickerCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) PickerCtrlDestruct,      // control destructor function
                       sizeof (CTRLPICKER_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) PickerCtrlPackInitParam,     // convert init param
                       sizeof (CTRLPICKER_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) PickerCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init picker class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PickerCtrlInitVtbl (
    CTRLPICKER_VTBL_T        *picker_vtbl_ptr
)
{
#ifdef TOUCH_PANEL_SUPPORT
    CTRLBASE_VTBL_T         *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) picker_vtbl_ptr;
    CTRLBASEPICKER_VTBL_T   *basepicker_vtbl_ptr = (CTRLBASEPICKER_VTBL_T*) picker_vtbl_ptr;

    if (PNULL == basepicker_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = PickerCtrlHandleMsg;

    basepicker_vtbl_ptr->GetFieldRange = PickerCtrlGetFieldRange;
    basepicker_vtbl_ptr->GetValueRange = PickerCtrlGetValueRange;
    basepicker_vtbl_ptr->GetFieldRect = PickerCtrlGetFieldRect;
    basepicker_vtbl_ptr->GetFieldText = PickerCtrlGetFieldText;
    basepicker_vtbl_ptr->GetFieldTips = PickerCtrlGetFieldTips;
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
LOCAL void PickerCtrlPackInitParam (
    GUIPICKER_INIT_DATA_T     *picker_init_ptr,   //in
    CTRLPICKER_INIT_PARAM_T   *picker_param_ptr   //out
)
{
#ifdef TOUCH_PANEL_SUPPORT
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) picker_param_ptr;

    if (PNULL == picker_init_ptr || PNULL == picker_param_ptr)
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = picker_init_ptr->both_rect;
#endif
}

/*****************************************************************************/
//  Description : picker construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PickerCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLPICKER_INIT_PARAM_T  *picker_param_ptr
)
{
    BOOLEAN                 result = TRUE;

#ifdef TOUCH_PANEL_SUPPORT
    CTRLPICKER_OBJ_T        *picker_ctrl_ptr = (CTRLPICKER_OBJ_T*) base_ctrl_ptr;
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = (CTRLBASEPICKER_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr || PNULL == picker_param_ptr)
    {
        return FALSE;
    }

    MMITHEME_GetPickerTheme (&picker_ctrl_ptr->theme);

    // PICKER不需要提示文字
    BASEPICKER_SetTipsPos (basepicker_ctrl_ptr, CTRLBASEPICKER_TIPS_POS_NONE);
#endif

    return (result);
}

/*****************************************************************************/
//  Description : picker destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PickerCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN             result = TRUE;

#ifdef TOUCH_PANEL_SUPPORT
    CTRLPICKER_OBJ_T    *picker_ctrl_ptr = (CTRLPICKER_OBJ_T*) base_ctrl_ptr;

    if (PNULL == picker_ctrl_ptr)
    {
        return FALSE;
    }

    FreePickerContent (picker_ctrl_ptr);
#endif

    return (result);
}

/*****************************************************************************/
//  Description : 设置PICKER风格是否需要循环操作
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPICKER_SetLoop (
    MMI_CTRL_ID_T          ctrl_id,
    BOOLEAN                is_loop
)
{
    BOOLEAN                 result = FALSE;

#ifdef TOUCH_PANEL_SUPPORT
    int32                   start_field = 0;
    int32                   end_field   = 0;
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = PNULL;

    //get ctrl pointer by control id
    basepicker_ctrl_ptr = (CTRLBASEPICKER_OBJ_T*) GetPickerPtr (ctrl_id);

    if (PNULL != basepicker_ctrl_ptr)
    {
        // 获取当前风格下，域的开始和结束范围，所有风格作统一处理
        VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

        basepicker_ctrl_ptr->state[start_field].is_loop = is_loop;

        result = TRUE;
    }

#endif

    return result;
}

/*****************************************************************************/
//  Description : 设置PICKER风格的文字
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPICKER_SetContent (
    MMI_CTRL_ID_T       ctrl_id,
    MMI_STRING_T        *content_array_ptr,
    uint16              content_array_size
)
{
    BOOLEAN             result = FALSE;

#ifdef TOUCH_PANEL_SUPPORT
    uint16              i = 0;
    CTRLPICKER_OBJ_T    *picker_ctrl_ptr = PNULL;

    if (PNULL == content_array_ptr || 0 == content_array_size)
    {
        return result;
    }

    picker_ctrl_ptr = GetPickerPtr (ctrl_id);

    if (PNULL != picker_ctrl_ptr)
    {
        FreePickerContent (picker_ctrl_ptr);

        picker_ctrl_ptr->content_size = content_array_size;
        picker_ctrl_ptr->content_ptr = SCI_ALLOC_APP (content_array_size * sizeof (MMI_STRING_T));
        SCI_MEMSET (picker_ctrl_ptr->content_ptr, 0, content_array_size*sizeof (MMI_STRING_T));

        for (i = 0; i < content_array_size; i++)
        {
            picker_ctrl_ptr->content_ptr[i].wstr_len = content_array_ptr[i].wstr_len;
            picker_ctrl_ptr->content_ptr[i].wstr_ptr = SCI_ALLOC_APP ( (content_array_ptr[i].wstr_len + 1) * sizeof (wchar_t));
            SCI_MEMSET (
                picker_ctrl_ptr->content_ptr[i].wstr_ptr, 0,
                (content_array_ptr[i].wstr_len + 1) * sizeof (wchar_t));

            SCI_MEMCPY (
                picker_ctrl_ptr->content_ptr[i].wstr_ptr,
                content_array_ptr[i].wstr_ptr,
                content_array_ptr[i].wstr_len * sizeof (wchar_t));
        }

        result = TRUE;
    }

#endif

    return result;
}

/*****************************************************************************/
//  Description : 设置PICKER风格的选中项
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPICKER_SetSelectedItem (
    MMI_CTRL_ID_T  ctrl_id,
    uint16         index
)
{
    BOOLEAN                 result = FALSE;

#ifdef TOUCH_PANEL_SUPPORT
    int32                   start_field = 0;
    int32                   end_field   = 0;
    CTRLPICKER_OBJ_T        *picker_ctrl_ptr = PNULL;
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = PNULL;

    //get ctrl pointer by control id
    picker_ctrl_ptr = GetPickerPtr (ctrl_id);

    if (PNULL != picker_ctrl_ptr
            && index < picker_ctrl_ptr->content_size)
    {
        basepicker_ctrl_ptr = (CTRLBASEPICKER_OBJ_T*) picker_ctrl_ptr;

        // 获取当前风格下，域的开始和结束范围，所有风格作统一处理
        VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

        if (basepicker_ctrl_ptr->enable_anim)
        {
            BASEPICKER_SetRecoverValue (basepicker_ctrl_ptr, start_field, index, FALSE);
        }
        else
        {
            BASEPICKER_SetValue (basepicker_ctrl_ptr, start_field, index);
        }

        result = TRUE;
    }

#endif

    return result;
}

/*****************************************************************************/
//  Description : 获取PICKER风格的选中项
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLPICKER_GetSelectedItem (
    MMI_CTRL_ID_T   ctrl_id
)
{
    uint16                  index = 0;

#ifdef TOUCH_PANEL_SUPPORT
    int32                   start_field = 0;
    int32                   end_field   = 0;
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr = PNULL;

    //get ctrl pointer by control id
    basepicker_ctrl_ptr = (CTRLBASEPICKER_OBJ_T*) GetPickerPtr (ctrl_id);

    if (PNULL != basepicker_ctrl_ptr)
    {
        // 获取当前风格下，域的开始和结束范围，所有风格作统一处理
        VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);

        index = BASEPICKER_GetValue (basepicker_ctrl_ptr, start_field);
    }

#endif

    return index;
}

#ifdef TOUCH_PANEL_SUPPORT

/*****************************************************************************/
//  Description : handle picker msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E PickerCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    CTRLBASE_VTBL_T         *parent_vtbl_ptr = PNULL;

    parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl(CTRL_PICKER_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);

    return parent_vtbl_ptr->HandleEvent (base_ctrl_ptr, msg_id, param);
}

/*****************************************************************************/
//  Description : 获取开始域与结束域(不包含该值)
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void PickerCtrlGetFieldRange (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    int32                  *start_field_ptr,       // [out]
    int32                  *end_field_ptr          // [out]
)
{
    *start_field_ptr = MMITHEME_PICKER_FIELD_START;
    *end_field_ptr = MMITHEME_PICKER_FIELD_END;

    SCI_ASSERT (*start_field_ptr <= MMITHEME_BASEPICKER_MAX_FIELD_COUNT     /*assert verified*/
                && *end_field_ptr <= MMITHEME_BASEPICKER_MAX_FIELD_COUNT);
}

/*****************************************************************************/
//  Description : 获取指定域的最值
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void PickerCtrlGetValueRange (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,// [in]
    CTRLBASEPICKER_FIELD_T      field,               // [in]
    uint16                      *min_value_ptr,      // [out]
    uint16                      *max_value_ptr       // [out]
)
{
    CTRLPICKER_OBJ_T    *picker_ctrl_ptr = (CTRLPICKER_OBJ_T*) basepicker_ctrl_ptr;

    *min_value_ptr = 0;

    if (0 != picker_ctrl_ptr->content_size)
    {
        *max_value_ptr = picker_ctrl_ptr->content_size - 1;
    }
    else
    {
        *max_value_ptr = 0;
    }
}

/*****************************************************************************/
//  Description : 获取指定域的相对显示位置
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T PickerCtrlGetFieldRect (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,   // [in]
    int32                   field                   // [in]
)
{
    uint16              width = 0;
    GUI_RECT_T          field_rect = {0};
    int32               start_field = 0;
    int32               end_field = 0;
    CTRLPICKER_OBJ_T    *picker_ctrl_ptr = (CTRLPICKER_OBJ_T*) basepicker_ctrl_ptr;

    field_rect = BASEPICKER_GetTextRect (basepicker_ctrl_ptr);

    width = basepicker_ctrl_ptr->theme.client_width;

    // 边线 + 分隔条(加上最左和最右的)
    VTLBASEPICKER_GetFieldRange (basepicker_ctrl_ptr, &start_field, &end_field);
    width -= (basepicker_ctrl_ptr->theme.border_width * 2 + (end_field - start_field + 1) * basepicker_ctrl_ptr->theme.separator_width);

    field_rect.right = field_rect.left + width * picker_ctrl_ptr->theme.field_width[field] / 100 - 1;

    return field_rect;
}

/*****************************************************************************/
//  Description : 获取域中的文字
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void PickerCtrlGetFieldText (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T      field,                  // [in]
    uint16                      cur_index,              // [in]
    MMI_STRING_T                *dis_string_ptr         // [out]
)
{
    CTRLPICKER_OBJ_T    *picker_ctrl_ptr = (CTRLPICKER_OBJ_T*) basepicker_ctrl_ptr;

    if (PNULL != picker_ctrl_ptr->content_ptr
            && picker_ctrl_ptr->content_size > 0)
    {
        if (cur_index <= picker_ctrl_ptr->content_size)
        {
            MMIAPICOM_Wstrncpy (
                dis_string_ptr->wstr_ptr,
                picker_ctrl_ptr->content_ptr[cur_index].wstr_ptr,
                picker_ctrl_ptr->content_ptr[cur_index].wstr_len);

            dis_string_ptr->wstr_len = picker_ctrl_ptr->content_ptr[cur_index].wstr_len;
        }
    }
}

/*****************************************************************************/
//  Description : 获取提示文本
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T PickerCtrlGetFieldTips (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
    int32                  field                   // [in]
)
{
    MMI_TEXT_ID_T       text_id = 0;
    CTRLPICKER_OBJ_T    *picker_ctrl_ptr = (CTRLPICKER_OBJ_T*) basepicker_ctrl_ptr;

    if (MMITHEME_PICKER_FIELD_COUNT > field)
    {
        text_id = picker_ctrl_ptr->theme.field_tips[field];
    }

    return text_id;
}

/*****************************************************************************/
//  Description : 释放picker风格分配的资源
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void FreePickerContent (
    CTRLPICKER_OBJ_T    *picker_ctrl_ptr
)
{
    uint16  i = 0;

    if (PNULL != picker_ctrl_ptr->content_ptr)
    {
        for (i = 0; i < picker_ctrl_ptr->content_size; i++)
        {
            if (PNULL != picker_ctrl_ptr->content_ptr[i].wstr_ptr)
            {
                SCI_FREE (picker_ctrl_ptr->content_ptr[i].wstr_ptr);
            }
        }

        SCI_FREE (picker_ctrl_ptr->content_ptr);
        picker_ctrl_ptr->content_size = 0;
    }
}

/*****************************************************************************/
//  Description : get picker pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLPICKER_OBJ_T* GetPickerPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT (PickerTypeOf (ctrl_ptr)); /*assert verified*/
    }

    return (CTRLPICKER_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN PickerTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf ( (OBJECT_TYPE_PTR) ctrl_ptr, CTRL_PICKER_TYPE);
}
#endif

