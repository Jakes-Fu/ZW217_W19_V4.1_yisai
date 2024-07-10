/*****************************************************************************
** File Name:      ctrldigitaledit.c                                               *
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

#include "ctrldigitaledit.h"

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
//  Description : init digitaledit class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DigitalEditCtrlInitVtbl(
    CTRLDIGITALEDIT_VTBL_T        *digitaledit_vtbl_ptr
);

/*****************************************************************************/
//  Description : digitaledit construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DigitalEditCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLDIGITALEDIT_INIT_PARAM_T   *digitaledit_param_ptr
);

/*****************************************************************************/
//  Description : digitaledit destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DigitalEditCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DigitalEditCtrlPackInitParam (
    GUIEDIT_INIT_DATA_T             *digitaledit_init_ptr,   //in
    CTRLDIGITALEDIT_INIT_PARAM_T    *digitaledit_param_ptr   //out
);

/*****************************************************************************/
//  Description : handle digitaledit msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DigitalEditCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : get digitaledit pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLDIGITALEDIT_OBJ_T* GetDigitalEditPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN DigitalEditTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
// 	Description : get align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E DigitalEditCtrlGetAlign(
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr   //in:
);

/*****************************************************************************/
// 	Description : set align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void DigitalEditCtrlSetAlign(
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr,  //in:
    GUI_ALIGN_E             align               //in
);

/*****************************************************************************/
// 	Description : get im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void DigitalEditCtrlGetImInfo(
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
LOCAL void DigitalEditCtrlSetImInfo(
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
// 	Description : config im default key
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void DigitalEditCtrlConfigImKey(
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : 设置横屏时的新edit
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DigitalEditCtrlSetLandscapeEdit(
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    MMI_HANDLE_T        landscape_edit          // [in] the dst landscape edit
);

/*****************************************************************************/
//  Description : 检测字符串是否合法
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DigitalEditCtrlIsStringValid(      // [ret] TRUE if char offset is valid
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    wchar                   *str_ptr,
    uint16                  str_len
);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get digitaledit type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T DIGITALEDIT_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASEFLEX_TYPE,                                    // parent class type
                       "digitaledit",                                         // control name
                       sizeof (CTRLDIGITALEDIT_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) DigitalEditCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) DigitalEditCtrlDestruct,      // control destructor function
                       sizeof (CTRLDIGITALEDIT_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) DigitalEditCtrlPackInitParam,     // convert init param
                       sizeof (CTRLDIGITALEDIT_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) DigitalEditCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init digitaledit class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DigitalEditCtrlInitVtbl (
    CTRLDIGITALEDIT_VTBL_T        *digitaledit_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) digitaledit_vtbl_ptr;
    CTRLBASEEDIT_VTBL_T *baseedit_ctrl_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*) digitaledit_vtbl_ptr;
    CTRLBASEFLEX_VTBL_T *baseflex_ctrl_vtbl_ptr = (CTRLBASEFLEX_VTBL_T*) digitaledit_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = DigitalEditCtrlHandleMsg;

    baseedit_ctrl_vtbl_ptr->GetAlign = DigitalEditCtrlGetAlign;
    baseedit_ctrl_vtbl_ptr->SetAlign = DigitalEditCtrlSetAlign;
    baseedit_ctrl_vtbl_ptr->GetImInfo = DigitalEditCtrlGetImInfo;
    baseedit_ctrl_vtbl_ptr->SetImInfo = DigitalEditCtrlSetImInfo;
    baseedit_ctrl_vtbl_ptr->ConfigImKey = DigitalEditCtrlConfigImKey;
    baseedit_ctrl_vtbl_ptr->SetLandscapeEdit = DigitalEditCtrlSetLandscapeEdit;

    baseflex_ctrl_vtbl_ptr->IsStringValid = DigitalEditCtrlIsStringValid;

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
LOCAL void DigitalEditCtrlPackInitParam (
    GUIEDIT_INIT_DATA_T             *digitaledit_init_ptr,   //in
    CTRLDIGITALEDIT_INIT_PARAM_T    *digitaledit_param_ptr   //out
)
{
    CTRLBASE_INIT_PARAM_T       *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) digitaledit_param_ptr;
    CTRLBASEEDIT_INIT_PARAM_T   *baseedit_ctrl_param_ptr = (CTRLBASEEDIT_INIT_PARAM_T*) digitaledit_param_ptr;

    if (PNULL == digitaledit_init_ptr || PNULL == digitaledit_param_ptr)
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = digitaledit_init_ptr->both_rect;

    baseedit_ctrl_param_ptr->is_rect_adaptive = digitaledit_init_ptr->is_rect_adaptive;
    baseedit_ctrl_param_ptr->str_max_len = digitaledit_init_ptr->str_max_len;
    baseedit_ctrl_param_ptr->type = digitaledit_init_ptr->type;
}

/*****************************************************************************/
//  Description : digitaledit construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DigitalEditCtrlConstruct (
    CTRLBASE_OBJ_T                  *base_ctrl_ptr,
    CTRLDIGITALEDIT_INIT_PARAM_T    *digitaledit_param_ptr
)
{
    BOOLEAN                 result = TRUE;
    CTRLDIGITALEDIT_OBJ_T   *digitaledit_ctrl_ptr = (CTRLDIGITALEDIT_OBJ_T*) base_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T      *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) base_ctrl_ptr;
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr || PNULL == digitaledit_param_ptr)
    {
        return FALSE;
    }

    MMITHEME_GetDigitalEditTheme (&digitaledit_ctrl_ptr->theme);

    //set display im icon and number info
    baseedit_ctrl_ptr->is_disp_im_icon  = TRUE;
    baseflex_ctrl_ptr->is_disp_num_info = TRUE;

    BASEEDIT_Init(baseedit_ctrl_ptr);

    return (result);
}

/*****************************************************************************/
//  Description : digitaledit destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DigitalEditCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN                 result = TRUE;
    CTRLDIGITALEDIT_OBJ_T   *digitaledit_ctrl_ptr = (CTRLDIGITALEDIT_OBJ_T*) base_ctrl_ptr;

    if (PNULL == digitaledit_ctrl_ptr)
    {
        return FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle digitaledit msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DigitalEditCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    CTRLDIGITALEDIT_OBJ_T   *digitaledit_ctrl_ptr = (CTRLDIGITALEDIT_OBJ_T*) base_ctrl_ptr;
    CTRLBASE_VTBL_T         *parent_vtbl_ptr = PNULL;

    if (PNULL == digitaledit_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    parent_vtbl_ptr = (CTRLBASE_VTBL_T*)TYPEMNG_GetParentVtbl(CTRL_DIGITALEDIT_TYPE, (OBJECT_TYPE_PTR)base_ctrl_ptr);
    result = parent_vtbl_ptr->HandleEvent(base_ctrl_ptr, msg_id, param);

    return (result);
}

/*****************************************************************************/
//  Description : get digitaledit pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLDIGITALEDIT_OBJ_T* GetDigitalEditPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        if (!DigitalEditTypeOf (ctrl_ptr))
        {
            SCI_TRACE_LOW("== GetDigitalEditPtr error == 0x%x", ctrl_ptr);
            ctrl_ptr = PNULL;
        }
    }

    return (CTRLDIGITALEDIT_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN DigitalEditTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)ctrl_ptr, CTRL_DIGITALEDIT_TYPE);
}

/*****************************************************************************/
// 	Description : get align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E DigitalEditCtrlGetAlign(
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr   //in:
)
{
    CTRLDIGITALEDIT_OBJ_T* digitaledit_ctrl_ptr = (CTRLDIGITALEDIT_OBJ_T*)baseedit_ctrl_ptr;

    return digitaledit_ctrl_ptr->theme.align;
}

/*****************************************************************************/
// 	Description : set align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void DigitalEditCtrlSetAlign(
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr,  //in:
    GUI_ALIGN_E             align               //in
)
{
    CTRLDIGITALEDIT_OBJ_T* digitaledit_ctrl_ptr = (CTRLDIGITALEDIT_OBJ_T*)baseedit_ctrl_ptr;

    digitaledit_ctrl_ptr->theme.align = align;
}

/*****************************************************************************/
// 	Description : get im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void DigitalEditCtrlGetImInfo(
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
    CTRLDIGITALEDIT_OBJ_T* digitaledit_ctrl_ptr = (CTRLDIGITALEDIT_OBJ_T*)baseedit_ctrl_ptr;

    *is_forbid_ptr    = digitaledit_ctrl_ptr->theme.im.is_forbid_symbol;
    *init_input_ptr   = digitaledit_ctrl_ptr->theme.im.init_input_type;
    *allow_input_ptr  = digitaledit_ctrl_ptr->theme.im.allow_input_type;
    *init_im_ptr      = digitaledit_ctrl_ptr->theme.im.init_im;
    *allow_im_ptr     = digitaledit_ctrl_ptr->theme.im.allow_im;
    *capital_mode_ptr = digitaledit_ctrl_ptr->theme.im.capital_mode;
    *tag_ptr          = digitaledit_ctrl_ptr->theme.im.tag;
}

/*****************************************************************************/
// 	Description : set im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void DigitalEditCtrlSetImInfo(
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
    CTRLDIGITALEDIT_OBJ_T* digitaledit_ctrl_ptr = (CTRLDIGITALEDIT_OBJ_T*)baseedit_ctrl_ptr;

    if (PNULL != is_forbid_ptr)
    {
        digitaledit_ctrl_ptr->theme.im.is_forbid_symbol = *is_forbid_ptr;
    }
    if (PNULL != init_im_ptr)
    {
        digitaledit_ctrl_ptr->theme.im.init_im = *init_im_ptr;
    }
    if (PNULL != allow_im_ptr)
    {
        digitaledit_ctrl_ptr->theme.im.allow_im = *allow_im_ptr;
    }
    if (PNULL != init_type_ptr)
    {
        digitaledit_ctrl_ptr->theme.im.init_input_type = *init_type_ptr;
    }
    if (PNULL != allow_type_ptr)
    {
        digitaledit_ctrl_ptr->theme.im.allow_input_type = *allow_type_ptr;
    }
    if (PNULL != capital_mode_ptr)
    {
        digitaledit_ctrl_ptr->theme.im.capital_mode = *capital_mode_ptr;
    }
    if (PNULL != tag_ptr)
    {
        digitaledit_ctrl_ptr->theme.im.tag = *tag_ptr;
    }
}

/*****************************************************************************/
// 	Description : config im default key
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void DigitalEditCtrlConfigImKey(
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr
)
{
    wchar       *star_value_ptr = PNULL;
    uint32      star_value_count = 0;
    wchar       hash_value_array[1]= {'#'};
    uint32      hash_value_count = 0;

    if (0 != baseedit_ctrl_ptr->im_ctrl_handle)
    {
        //config im *
        GUIIM_SetCustomKey(
            baseedit_ctrl_ptr->im_ctrl_handle,
            GUIIM_TYPE_DIGITAL,
            GUIIM_CUSTOM_KEY_STAR,
            star_value_ptr,
            star_value_count);

        //config im #
        GUIIM_SetCustomKey(
            baseedit_ctrl_ptr->im_ctrl_handle,
            GUIIM_TYPE_DIGITAL,
            GUIIM_CUSTOM_KEY_HASH,
            hash_value_array,
            hash_value_count);
    }
}

/*****************************************************************************/
//  Description : 设置横屏时的新edit
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DigitalEditCtrlSetLandscapeEdit(
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    MMI_HANDLE_T        landscape_edit          // [in] the dst landscape edit
)
{
    CTRLDIGITALEDIT_OBJ_T   *new_edit_ctrl_ptr = GetDigitalEditPtr(landscape_edit);
    CTRLDIGITALEDIT_OBJ_T   *digitaledit_ctrl_ptr = (CTRLDIGITALEDIT_OBJ_T*)baseedit_ctrl_ptr;
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*) baseedit_ctrl_ptr;

    if (PNULL != new_edit_ctrl_ptr)
    {
        //set string max len
        CTRLBASEFLEX_SetMaxLen(landscape_edit,baseedit_ctrl_ptr->str_max_len);

        //set style
        CTRLBASEFLEX_SetStyle(landscape_edit,baseedit_ctrl_ptr->style);

        //set string
        CTRLBASEFLEX_SetString(landscape_edit,baseedit_ctrl_ptr->str_ptr,baseedit_ctrl_ptr->str_len);

        //set im
        new_edit_ctrl_ptr->theme.im = digitaledit_ctrl_ptr->theme.im;

        //set cursor pos
        CTRLBASEFLEX_SetCursorCurPos(landscape_edit,baseedit_ctrl_ptr->cursor.cur_pos);

        CTRLBASEFLEX_ConfigDisableChar (
            landscape_edit,
            baseflex_ctrl_ptr->disable_char_ptr,
            baseflex_ctrl_ptr->disable_char_len);

        CTRLBASEFLEX_ConfigAddEnableChar (
            landscape_edit,
            baseflex_ctrl_ptr->add_enable_char_ptr,
            baseflex_ctrl_ptr->add_enable_char_len);

        //set cursor hide
        CTRLBASEFLEX_SetCursorHideState (landscape_edit, baseedit_ctrl_ptr->cursor.is_hide);
    }
}

/*****************************************************************************/
//  Description : 检测字符串是否合法
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DigitalEditCtrlIsStringValid(      // [ret] TRUE if char offset is valid
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    wchar                   *str_ptr,
    uint16                  str_len
)
{
    return BASEFLEX_CheckDigitalStr(str_ptr,str_len);
}

