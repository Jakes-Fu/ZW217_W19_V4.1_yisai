/*****************************************************************************
** File Name:      ctrlpswedit.c                                               *
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

#include "ctrlpswedit.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define GUIEDIT_PASSWORD_CHAR               0x2A    //*

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
//  Description : init pswedit class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PswEditCtrlInitVtbl (
    CTRLPSWEDIT_VTBL_T        *pswedit_vtbl_ptr
);

/*****************************************************************************/
//  Description : pswedit construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PswEditCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLPSWEDIT_INIT_PARAM_T   *pswedit_param_ptr
);

/*****************************************************************************/
//  Description : pswedit destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PswEditCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void PswEditCtrlPackInitParam (
    GUIEDIT_INIT_DATA_T         *pswedit_init_ptr,   //in
    CTRLPSWEDIT_INIT_PARAM_T    *pswedit_param_ptr   //out
);

/*****************************************************************************/
//  Description : handle pswedit msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E PswEditCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : get pswedit pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLPSWEDIT_OBJ_T* GetPswEditPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN PswEditTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
// 	Description : get align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E PswEditCtrlGetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr   //in:
);

/*****************************************************************************/
// 	Description : set align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void PswEditCtrlSetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr,  //in:
    GUI_ALIGN_E             align               //in
);

/*****************************************************************************/
// 	Description : get im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void PswEditCtrlGetImInfo (
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
LOCAL void PswEditCtrlSetImInfo (
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
LOCAL void PswEditCtrlConfigImKey (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : 设置横屏时的新edit
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void PswEditCtrlSetLandscapeEdit (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    MMI_HANDLE_T        landscape_edit          // [in] the dst landscape edit
);

/*****************************************************************************/
//  Description : 获取edit的字符串内容
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL wchar* PswEditCtrlGetDispStr (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    uint16              start_pos,
    uint16              str_len
);

/*****************************************************************************/
//  Description : 获取总行数
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL uint16 PswEditCtrlGetLineNum (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    uint16              display_width           // [in] width
);

/*****************************************************************************/
//  Description : 检测字符串是否合法
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PswEditCtrlIsStringValid (     // [ret] TRUE if char offset is valid
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    wchar                   *str_ptr,
    uint16                  str_len
);

/*****************************************************************************/
//  Description : 最大长度改变后的通知回调
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void PswEditCtrlOnMaxLenChanged (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : 当添加文本后的通知回调
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void PswEditCtrlOnTextAdded(
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    GUIEDIT_ADD_STR_T       *add_str_ptr
);

///*****************************************************************************/
////  Description : 获取指定宽度内能容纳的字数和象素值
////  Global resource dependence : 
////  Author: hua.fang
////  Param:
///*****************************************************************************/
//PUBLIC BOOLEAN PswEditCtrlGetStringInfo(
//    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,     //in:
//    int8                dir,                    //in:
//    uint16              start_pos,              //in:
//    uint16              str_len,                //in:
//    uint16              line_width,             //in:
//    BOOLEAN             is_byword,              //in:
//    uint16              *char_num_ptr,          //out:
//    uint16              *string_pixel_ptr       //out:
//);

/*****************************************************************************/
//  Description : handle password timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandlePasswordTimer (
    CTRLPSWEDIT_OBJ_T* pswedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : get password string
//  Global resource dependence :
//  Author: Jassmine
//  Note: alloc memory,need free
/*****************************************************************************/
PUBLIC wchar* GetPswString (
    CTRLPSWEDIT_OBJ_T   *pswedit_ctrl_ptr,
    uint16              start_pos,      //in:
    uint16              str_len         //in:
);

/*****************************************************************************/
//  Description : start password timer id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartPswTimer (
    CTRLPSWEDIT_OBJ_T   *pswedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : stop password timer id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StopPswTimer (
    CTRLPSWEDIT_OBJ_T   *pswedit_ctrl_ptr
);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : set edit password style
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPSWEDIT_SetPasswordStyle (
    MMI_CTRL_ID_T              ctrl_id,
    GUIEDIT_PASSWORD_STYLE_E   style
)
{
    CTRLPSWEDIT_OBJ_T  *pswedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    pswedit_ctrl_ptr = GetPswEditPtr (ctrl_id);

    if (PNULL != pswedit_ctrl_ptr)
    {
        //set style
        pswedit_ctrl_ptr->psw_style = style;
    }
}

/*****************************************************************************/
//  Description : get pswedit type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T PSWEDIT_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASEFLEX_TYPE,                                // parent class type
                       "pswedit",                                         // control name
                       sizeof (CTRLPSWEDIT_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) PswEditCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) PswEditCtrlDestruct,      // control destructor function
                       sizeof (CTRLPSWEDIT_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) PswEditCtrlPackInitParam,     // convert init param
                       sizeof (CTRLPSWEDIT_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) PswEditCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init pswedit class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PswEditCtrlInitVtbl (
    CTRLPSWEDIT_VTBL_T        *pswedit_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) pswedit_vtbl_ptr;
    CTRLBASEEDIT_VTBL_T *baseedit_ctrl_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*) pswedit_vtbl_ptr;
    CTRLBASEFLEX_VTBL_T *baseflex_ctrl_vtbl_ptr = (CTRLBASEFLEX_VTBL_T*) pswedit_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = PswEditCtrlHandleMsg;

    baseedit_ctrl_vtbl_ptr->GetAlign = PswEditCtrlGetAlign;
    baseedit_ctrl_vtbl_ptr->SetAlign = PswEditCtrlSetAlign;
    baseedit_ctrl_vtbl_ptr->GetImInfo = PswEditCtrlGetImInfo;
    baseedit_ctrl_vtbl_ptr->SetImInfo = PswEditCtrlSetImInfo;
    baseedit_ctrl_vtbl_ptr->ConfigImKey = PswEditCtrlConfigImKey;
    baseedit_ctrl_vtbl_ptr->SetLandscapeEdit = PswEditCtrlSetLandscapeEdit;
    baseedit_ctrl_vtbl_ptr->GetDispStr = PswEditCtrlGetDispStr;
    baseedit_ctrl_vtbl_ptr->GetLineNum = PswEditCtrlGetLineNum;

    baseflex_ctrl_vtbl_ptr->IsStringValid = PswEditCtrlIsStringValid;
    baseflex_ctrl_vtbl_ptr->OnMaxLenChanged = PswEditCtrlOnMaxLenChanged;
    baseflex_ctrl_vtbl_ptr->OnTextAdded = PswEditCtrlOnTextAdded;

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
LOCAL void PswEditCtrlPackInitParam (
    GUIEDIT_INIT_DATA_T         *pswedit_init_ptr,   //in
    CTRLPSWEDIT_INIT_PARAM_T    *pswedit_param_ptr   //out
)
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) pswedit_param_ptr;
    CTRLBASEEDIT_INIT_PARAM_T   *baseedit_ctrl_param_ptr = (CTRLBASEEDIT_INIT_PARAM_T*) pswedit_param_ptr;

    if (PNULL == pswedit_init_ptr || PNULL == pswedit_param_ptr)
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = pswedit_init_ptr->both_rect;

    baseedit_ctrl_param_ptr->is_rect_adaptive = pswedit_init_ptr->is_rect_adaptive;
    baseedit_ctrl_param_ptr->str_max_len = pswedit_init_ptr->str_max_len;
    baseedit_ctrl_param_ptr->type = pswedit_init_ptr->type;
}

/*****************************************************************************/
//  Description : pswedit construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PswEditCtrlConstruct (
    CTRLBASE_OBJ_T              *base_ctrl_ptr,
    CTRLPSWEDIT_INIT_PARAM_T    *pswedit_param_ptr
)
{
    BOOLEAN             result = TRUE;
    MMITHEME_EDIT_PSW_DISPLAY_T  display_theme = {0};
    CTRLPSWEDIT_OBJ_T   *pswedit_ctrl_ptr = (CTRLPSWEDIT_OBJ_T*) base_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) base_ctrl_ptr;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr || PNULL == pswedit_param_ptr)
    {
        return FALSE;
    }

    MMITHEME_GetPswEditTheme (&pswedit_ctrl_ptr->theme);

    //get password display info
    MMITHEME_GetPwDisplayTheme (&display_theme);

    //set border,bg etc.
    baseedit_ctrl_ptr->common_theme.border = display_theme.border;
    baseedit_ctrl_ptr->common_theme.bg     = display_theme.bg;
    baseedit_ctrl_ptr->common_theme.font   = display_theme.font;

    //set edit password default is single
    baseedit_ctrl_ptr->style = GUIEDIT_STYLE_SINGLE;

    //set default password style
    pswedit_ctrl_ptr->psw_style = GUIEDIT_PASSWORD_STYLE_DIGITAL;

    //set display im icon and number info
    baseedit_ctrl_ptr->is_disp_im_icon  = TRUE;
    baseflex_ctrl_ptr->is_disp_num_info = TRUE;

    baseflex_ctrl_ptr->is_disable_cutcopy = TRUE;

    pswedit_ctrl_ptr->password_ptr = BASEEDIT_AllocStrMemory (baseedit_ctrl_ptr->str_max_len);

    BASEEDIT_Init(baseedit_ctrl_ptr);

    return (result);
}

/*****************************************************************************/
//  Description : pswedit destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PswEditCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLPSWEDIT_OBJ_T     *pswedit_ctrl_ptr = (CTRLPSWEDIT_OBJ_T*) base_ctrl_ptr;

    if (PNULL == pswedit_ctrl_ptr)
    {
        return FALSE;
    }

    //stop password timer
    StopPswTimer (pswedit_ctrl_ptr);

    if (PNULL != pswedit_ctrl_ptr->password_ptr)
    {
        SCI_FREE (pswedit_ctrl_ptr->password_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle pswedit msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E PswEditCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    BOOLEAN             need_parent_handle = TRUE;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = PNULL;
    CTRLPSWEDIT_OBJ_T   *pswedit_ctrl_ptr = (CTRLPSWEDIT_OBJ_T*) base_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) base_ctrl_ptr;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*) base_ctrl_ptr;

    if (PNULL == pswedit_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_CTL_GET_FOCUS:
        StartPswTimer (pswedit_ctrl_ptr);
        break;

    case MSG_CTL_LOSE_FOCUS:
        //stop password timer
        if (StopPswTimer (pswedit_ctrl_ptr))
        {
            if (baseedit_ctrl_ptr->highlight.is_underline)
            {
                //commit
                pswedit_ctrl_ptr->is_edit_commit = TRUE;
                GUIIM_CommitLast (baseedit_ctrl_ptr->im_ctrl_handle);
                pswedit_ctrl_ptr->is_edit_commit = FALSE;
            }
            else
            {
                //reparse text
                BASEFLEX_ReparseEditText (baseflex_ctrl_ptr);
            }

            //update display
            VTLBASEEDIT_DisplayAll (baseedit_ctrl_ptr, TRUE);
        }
        break;

    case MSG_CTL_LOSE_ACTIVE:
        //stop password timer
        StopPswTimer (pswedit_ctrl_ptr);
        break;

    case MSG_NOTIFY_LOSE_ACTIVE:
        if (!MMK_IsActiveCtrl (base_ctrl_ptr->handle))
        {
            //stop password timer
            StopPswTimer (pswedit_ctrl_ptr);
        }
        break;

    case MSG_TIMER:
        if (pswedit_ctrl_ptr->pw_timer_id == * (uint8 *) param)
        {
            HandlePasswordTimer (pswedit_ctrl_ptr);
            need_parent_handle = FALSE;
        }
        break;

    default:
        break;
    }

    if (need_parent_handle)
    {
        parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_PSWEDIT_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
        result = parent_vtbl_ptr->HandleEvent (base_ctrl_ptr, msg_id, param);
    }

    return (result);
}

/*****************************************************************************/
//  Description : get pswedit pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLPSWEDIT_OBJ_T* GetPswEditPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        if (!PswEditTypeOf (ctrl_ptr))
        {
            SCI_TRACE_LOW("== GetPswEditPtr error == 0x%x", ctrl_ptr);
            ctrl_ptr = PNULL;
        }
    }

    return (CTRLPSWEDIT_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN PswEditTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf ( (OBJECT_TYPE_PTR) ctrl_ptr, CTRL_PSWEDIT_TYPE);
}

/*****************************************************************************/
// 	Description : get align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E PswEditCtrlGetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr   //in:
)
{
    CTRLPSWEDIT_OBJ_T* pswedit_ctrl_ptr = (CTRLPSWEDIT_OBJ_T*) baseedit_ctrl_ptr;

    return pswedit_ctrl_ptr->theme.align;
}

/*****************************************************************************/
// 	Description : set align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void PswEditCtrlSetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr,  //in:
    GUI_ALIGN_E             align               //in
)
{
    CTRLPSWEDIT_OBJ_T* pswedit_ctrl_ptr = (CTRLPSWEDIT_OBJ_T*) baseedit_ctrl_ptr;

    pswedit_ctrl_ptr->theme.align = align;
}

/*****************************************************************************/
// 	Description : get im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void PswEditCtrlGetImInfo (
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
    CTRLPSWEDIT_OBJ_T* pswedit_ctrl_ptr = (CTRLPSWEDIT_OBJ_T*) baseedit_ctrl_ptr;

    *is_forbid_ptr    = pswedit_ctrl_ptr->theme.im.is_forbid_symbol;
    *init_input_ptr   = pswedit_ctrl_ptr->theme.im.init_input_type;
    *allow_input_ptr  = pswedit_ctrl_ptr->theme.im.allow_input_type;
    *init_im_ptr      = pswedit_ctrl_ptr->theme.im.init_im;
    *allow_im_ptr     = pswedit_ctrl_ptr->theme.im.allow_im;
    *capital_mode_ptr = pswedit_ctrl_ptr->theme.im.capital_mode;
    *tag_ptr          = pswedit_ctrl_ptr->theme.im.tag;
}

/*****************************************************************************/
// 	Description : set im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void PswEditCtrlSetImInfo (
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
    CTRLPSWEDIT_OBJ_T* pswedit_ctrl_ptr = (CTRLPSWEDIT_OBJ_T*) baseedit_ctrl_ptr;

    if (PNULL != is_forbid_ptr)
    {
        pswedit_ctrl_ptr->theme.im.is_forbid_symbol = *is_forbid_ptr;
    }

    if (PNULL != init_im_ptr)
    {
        pswedit_ctrl_ptr->theme.im.init_im = *init_im_ptr;
    }

    if (PNULL != allow_im_ptr)
    {
        pswedit_ctrl_ptr->theme.im.allow_im = *allow_im_ptr;

        //modify password style
        if (GUIIM_TYPE_DIGITAL != *allow_im_ptr)
        {
            //set style
            pswedit_ctrl_ptr->psw_style = GUIEDIT_PASSWORD_STYLE_ALL;
        }
    }

    if (PNULL != init_type_ptr)
    {
        pswedit_ctrl_ptr->theme.im.init_input_type = *init_type_ptr;
    }

    if (PNULL != allow_type_ptr)
    {
        pswedit_ctrl_ptr->theme.im.allow_input_type = *allow_type_ptr;
    }

    if (PNULL != capital_mode_ptr)
    {
        pswedit_ctrl_ptr->theme.im.capital_mode = *capital_mode_ptr;
    }

    if (PNULL != tag_ptr)
    {
        pswedit_ctrl_ptr->theme.im.tag = *tag_ptr;
    }
}

/*****************************************************************************/
// 	Description : config im default key
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void PswEditCtrlConfigImKey (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr
)
{
    BOOLEAN     is_config_star = FALSE;
    BOOLEAN     is_config_hash = FALSE;
    wchar       *star_value_ptr = PNULL;
    wchar       phonenum_star_array[4] = {'*', '+', 'P', 'W'};
    uint32      star_value_count = 0;
    wchar       hash_value_array[1] = {'#'};
    uint32      hash_value_count = 0;
    CTRLPSWEDIT_OBJ_T* pswedit_ctrl_ptr = (CTRLPSWEDIT_OBJ_T*) baseedit_ctrl_ptr;

    if (0 != baseedit_ctrl_ptr->im_ctrl_handle)
    {
        switch (pswedit_ctrl_ptr->psw_style)
        {
        case GUIEDIT_PASSWORD_STYLE_DIGITAL:
            //not handle star/hash
            is_config_star = TRUE;
            is_config_hash = TRUE;
            break;

        case GUIEDIT_PASSWORD_STYLE_DIGITAL_PIN:
            is_config_star = TRUE;
            is_config_hash = TRUE;

            //set hash value count
            hash_value_count = 1;
            break;

        case GUIEDIT_PASSWORD_STYLE_DIGITAL_PUK:
        case GUIEDIT_PASSWORD_STYLE_DIGITAL_EX:
            is_config_star = TRUE;
            is_config_hash = TRUE;

            //set start value and count
            star_value_ptr = phonenum_star_array;

            //set star value count
            star_value_count = 1;

            //set hash value count
            hash_value_count = 1;
            break;

        default:
            break;
        }

        if (is_config_star)
        {
            //config im *
            GUIIM_SetCustomKey (
                baseedit_ctrl_ptr->im_ctrl_handle,
                GUIIM_TYPE_DIGITAL,
                GUIIM_CUSTOM_KEY_STAR,
                star_value_ptr,
                star_value_count);
        }

        if (is_config_hash)
        {
            //config im #
            GUIIM_SetCustomKey (
                baseedit_ctrl_ptr->im_ctrl_handle,
                GUIIM_TYPE_DIGITAL,
                GUIIM_CUSTOM_KEY_HASH,
                hash_value_array,
                hash_value_count);
        }
    }
}

/*****************************************************************************/
//  Description : 设置横屏时的新edit
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void PswEditCtrlSetLandscapeEdit (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    MMI_HANDLE_T        landscape_edit          // [in] the dst landscape edit
)
{
    CTRLPSWEDIT_OBJ_T   *new_edit_ctrl_ptr = GetPswEditPtr (landscape_edit);
    CTRLPSWEDIT_OBJ_T   *pswedit_ctrl_ptr = (CTRLPSWEDIT_OBJ_T*) baseedit_ctrl_ptr;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*) baseedit_ctrl_ptr;

    if (PNULL != new_edit_ctrl_ptr)
    {
        //set string max len
        CTRLBASEFLEX_SetMaxLen (landscape_edit, baseedit_ctrl_ptr->str_max_len);

        //set style
        CTRLBASEFLEX_SetStyle (landscape_edit, baseedit_ctrl_ptr->style);

        //set password style
        CTRLPSWEDIT_SetPasswordStyle (landscape_edit, pswedit_ctrl_ptr->psw_style);

        //set string
        CTRLBASEFLEX_SetString (landscape_edit, baseedit_ctrl_ptr->str_ptr, baseedit_ctrl_ptr->str_len);

        //set im
        new_edit_ctrl_ptr->theme.im = pswedit_ctrl_ptr->theme.im;

        //set cursor pos
        CTRLBASEFLEX_SetCursorCurPos (landscape_edit, baseedit_ctrl_ptr->cursor.cur_pos);

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
//  Description : 获取edit的字符串内容
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL wchar* PswEditCtrlGetDispStr (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    uint16              start_pos,
    uint16              str_len
)
{
    wchar *str_ptr = PNULL;

    //get password string
    str_ptr = GetPswString ((CTRLPSWEDIT_OBJ_T*)baseedit_ctrl_ptr, start_pos, str_len);

    return str_ptr;
}

/*****************************************************************************/
//  Description : 获取总行数
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL uint16 PswEditCtrlGetLineNum (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    uint16              display_width           // [in] width
)
{
    BOOLEAN     is_handle = TRUE;
    uint16      total_line_num = 0;
    wchar       *str_ptr = PNULL;
    wchar       str_len = baseedit_ctrl_ptr->str_len;
    CTRLPSWEDIT_OBJ_T   *pswedit_ctrl_ptr = (CTRLPSWEDIT_OBJ_T*) baseedit_ctrl_ptr;

    if (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)
    {
        total_line_num = 1;
    }
    else
    {
        //get total num
        if (0 == pswedit_ctrl_ptr->visible_len)
        {
            total_line_num = GUI_CalculateCharLinesByPixelNum (
                                 display_width,
                                 GUIEDIT_PASSWORD_CHAR,
                                 str_len,
                                 baseedit_ctrl_ptr->common_theme.font.font,
                                 (uint8) baseedit_ctrl_ptr->common_theme.char_space);

            is_handle = FALSE;
        }
        else
        {
            //get password string
            str_ptr = PswEditCtrlGetDispStr (baseedit_ctrl_ptr, 0, baseedit_ctrl_ptr->str_len);

            total_line_num = GUI_CalculateStringLinesByPixelNum (
                                 display_width,
                                 str_ptr,
                                 str_len,
                                 baseedit_ctrl_ptr->common_theme.font.font,
                                 (uint8) baseedit_ctrl_ptr->common_theme.char_space,
                                 FALSE);

            //is carriage return or line feed
            if (BASEEDIT_IsNewLineChar (baseedit_ctrl_ptr, str_len))
            {
                //cursor display the next line
                total_line_num++;
            }
        }

        //must one line
        if (0 == total_line_num)
        {
            total_line_num = 1;
        }
    }

    return (total_line_num);
}

/*****************************************************************************/
//  Description : 检测字符串是否合法
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PswEditCtrlIsStringValid (     // [ret] TRUE if char offset is valid
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    wchar                   *str_ptr,
    uint16                  str_len
)
{
    BOOLEAN         result = TRUE;
    uint16          i = 0;
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*) baseflex_ctrl_ptr;
    CTRLPSWEDIT_OBJ_T   *pswedit_ctrl_ptr = (CTRLPSWEDIT_OBJ_T*) baseflex_ctrl_ptr;

    switch (pswedit_ctrl_ptr->psw_style)
    {
    case GUIEDIT_PASSWORD_STYLE_DIGITAL:
        //check digital string
        result = BASEFLEX_CheckDigitalStr (str_ptr, str_len);
        break;

    case GUIEDIT_PASSWORD_STYLE_DIGITAL_PIN:
        //check password string
        for (i = 0; i < str_len; i++)
        {
            if ('#' == str_ptr[i])
            {
                MMK_SendMsg (base_ctrl_ptr->handle, MSG_APP_OK, PNULL);
                result = FALSE;
                break;
            }
            else if (!BASEFLEX_IsDigitalChar (str_ptr[i]))
            {
                result = FALSE;
                break;
            }
        }

        break;

    case GUIEDIT_PASSWORD_STYLE_DIGITAL_PUK:
        //check password string
        for (i = 0; i < str_len; i++)
        {
            if ('#' == str_ptr[i])
            {
                MMK_SendMsg (base_ctrl_ptr->handle, MSG_APP_OK, PNULL);
                result = FALSE;
                break;
            }
            else if ( (!BASEFLEX_IsDigitalChar (str_ptr[i])) &&
                      ('*' != str_ptr[i]))
            {
                result = FALSE;
                break;
            }
        }

        break;

    case GUIEDIT_PASSWORD_STYLE_DIGITAL_EX:
        //check password string
        for (i = 0; i < str_len; i++)
        {
            if ( (!BASEFLEX_IsDigitalChar (str_ptr[i])) &&
                    ('*' != str_ptr[i]) &&
                    ('#' != str_ptr[i]))
            {
                result = FALSE;
                break;
            }
        }

        break;

    case GUIEDIT_PASSWORD_STYLE_ALL:
        break;

    default:
        //SCI_TRACE_LOW:"CheckPasswordStr:password style %d is error!"
        SCI_TRACE_ID (TRACE_TOOL_CONVERT, GUIEDIT_5560_112_2_18_3_15_45_126, (uint8*) "d", pswedit_ctrl_ptr->psw_style);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : 最大长度改变后的通知回调
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void PswEditCtrlOnMaxLenChanged (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
)
{
    CTRLPSWEDIT_OBJ_T   *pswedit_ctrl_ptr = (CTRLPSWEDIT_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;

    if (PNULL != pswedit_ctrl_ptr->password_ptr)
    {
        SCI_FREE (pswedit_ctrl_ptr->password_ptr);
    }

    pswedit_ctrl_ptr->password_ptr = BASEEDIT_AllocStrMemory (baseedit_ctrl_ptr->str_max_len);
}

/*****************************************************************************/
//  Description : 当添加文本后的通知回调
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void PswEditCtrlOnTextAdded(
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    GUIEDIT_ADD_STR_T       *add_str_ptr
)
{
    CTRLPSWEDIT_OBJ_T   *pswedit_ctrl_ptr = (CTRLPSWEDIT_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;

    if ((add_str_ptr->is_im_commit) &&
        (!pswedit_ctrl_ptr->is_edit_commit))
    {
        //stop password timer
        StopPswTimer (pswedit_ctrl_ptr);

        //modify password visible char
        pswedit_ctrl_ptr->visible_len = (uint16) (add_str_ptr->commit_len + add_str_ptr->underline_len);
        pswedit_ctrl_ptr->visible_start   = (uint16) (baseedit_ctrl_ptr->cursor.cur_pos - pswedit_ctrl_ptr->visible_len);

        //creat password timer
        StartPswTimer (pswedit_ctrl_ptr);
    }
}

///*****************************************************************************/
////  Description : 获取指定宽度内能容纳的字数和象素值
////  Global resource dependence : 
////  Author: hua.fang
////  Param:
///*****************************************************************************/
//PUBLIC BOOLEAN PswEditCtrlGetStringInfo(
//    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,     //in:
//    int8                dir,                    //in:
//    uint16              start_pos,              //in:
//    uint16              str_len,                //in:
//    uint16              line_width,             //in:
//    BOOLEAN             is_byword,              //in:
//    uint16              *char_num_ptr,          //out:
//    uint16              *string_pixel_ptr       //out:
//)
//{
//    wchar               *password_str_ptr = PNULL;
//    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;
//    CTRLPSWEDIT_OBJ_T   *pswedit_ctrl_ptr = (CTRLPSWEDIT_OBJ_T*) baseflex_ctrl_ptr;

//    if ( (PNULL != char_num_ptr) && (PNULL != string_pixel_ptr))
//    {
//        if (0 == pswedit_ctrl_ptr->visible_len)
//        {
//            GUI_GetCharInfoInLine (
//                    GUIEDIT_PASSWORD_CHAR,
//                    str_len,
//                    baseedit_ctrl_ptr->common_theme.font.font,
//                    (uint8) baseedit_ctrl_ptr->common_theme.char_space,
//                    line_width,
//                    char_num_ptr,
//                    string_pixel_ptr);
//        }
//        else
//        {
//            //get password string
//            password_str_ptr = GetPswString (baseedit_ctrl_ptr, start_pos, str_len);

//            GUI_GetStringInfoInLineWithDir (
//                    TEXT_DIR_LTR,
//                    password_str_ptr,
//                    str_len,
//                    baseedit_ctrl_ptr->common_theme.font.font,
//                    (uint8) baseedit_ctrl_ptr->common_theme.char_space,
//                    line_width,
//                    char_num_ptr,
//                    string_pixel_ptr,
//                    FALSE);
//        }
//    }

//    return TRUE;
//}

/*****************************************************************************/
//  Description : handle password timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandlePasswordTimer (
    CTRLPSWEDIT_OBJ_T* pswedit_ctrl_ptr
)
{
    GUIEDIT_UPDATE_T    update_info = {0};
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) pswedit_ctrl_ptr;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*) pswedit_ctrl_ptr;

    //stop timer
    if (StopPswTimer (pswedit_ctrl_ptr))
    {
        if (baseedit_ctrl_ptr->highlight.is_underline)
        {
            //commit
            pswedit_ctrl_ptr->is_edit_commit = TRUE;
            GUIIM_CommitLast (baseedit_ctrl_ptr->im_ctrl_handle);
            pswedit_ctrl_ptr->is_edit_commit = FALSE;
        }
        else
        {
            //reparse text
            BASEFLEX_ReparseEditText (baseflex_ctrl_ptr);

            //update
            update_info.update_mode = GUIEDIT_UPDATE_ALL;
            VTLBASEEDIT_DisplayAll (baseedit_ctrl_ptr, TRUE);
        }
    }
}

/*****************************************************************************/
//  Description : get password string
//  Global resource dependence :
//  Author: Jassmine
//  Note: alloc memory,need free
/*****************************************************************************/
PUBLIC wchar* GetPswString (
    CTRLPSWEDIT_OBJ_T   *pswedit_ctrl_ptr,
    uint16              start_pos,      //in:
    uint16              str_len         //in:
)
{
    wchar       *password_str_ptr = PNULL;
    uint16      i = 0;
    uint16      pw_visible_start = 0;
    uint16      pw_visible_end = 0;
    uint16      end_pos = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) pswedit_ctrl_ptr;

    //SCI_ASSERT (PNULL != pswedit_ctrl_ptr->password_ptr);
    if(PNULL != pswedit_ctrl_ptr->password_ptr)
	{
		password_str_ptr = pswedit_ctrl_ptr->password_ptr;

	    //set string
	    for (i = 0; i < str_len; i++)
	    {
	        password_str_ptr[i] = GUIEDIT_PASSWORD_CHAR;
	    }

	    password_str_ptr[i] = 0;

	    //highlight display char
	    if (0 < pswedit_ctrl_ptr->visible_len)
	    {
	        //get string end position
	        end_pos = (uint16) (start_pos + str_len);

	        //get visible password string start and end pos
	        pw_visible_start = pswedit_ctrl_ptr->visible_start;
	        pw_visible_end   = (uint16) (pw_visible_start + pswedit_ctrl_ptr->visible_len);

	        //get visible password string
	        pw_visible_start = (uint16) MAX (pw_visible_start, start_pos);
	        pw_visible_end = (uint16) MIN (pw_visible_end, end_pos);

	        //kevin.lou modified:delete assert
	        //set highlight char
	        if ( (pw_visible_end > pw_visible_start) && (pw_visible_start >= start_pos))
	        {
	            //SCI_ASSERT(pw_visible_start >= start_pos); /*assert verified*/
	            for (i = pw_visible_start; i < pw_visible_end; i++)
	            {
	                password_str_ptr[i-start_pos] = baseedit_ctrl_ptr->str_ptr[i];
	            }
	        }
	    }
			
	}

    return (password_str_ptr);
}

///*****************************************************************************/
////  Description : get password string info in line
////  Global resource dependence :
////  Author: Jassmine
////  Note:
///*****************************************************************************/
//PUBLIC BOOLEAN GUIEDIT_GetPwStrInfoInLine (//is password
//    CTRLPSWEDIT_OBJ_T   *pswedit_ctrl_ptr,
//    uint16              start_pos,          //in:
//    uint16              str_len,            //in:
//    uint16              line_width,         //in:
//    uint16              *char_num_ptr,      //in/out:
//    uint16              *string_pixel_ptr   //in/out:
//)
//{
//    BOOLEAN             result = FALSE;
//    wchar               *password_str_ptr = PNULL;
//    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) pswedit_ctrl_ptr;

//    if ( (PNULL != char_num_ptr) &&
//            (PNULL != string_pixel_ptr) &&
//            (GUIEDIT_TYPE_PASSWORD == baseedit_ctrl_ptr->type))
//    {
//        if (0 == pswedit_ctrl_ptr->visible_len)
//        {
//            GUI_GetCharInfoInLine (
//                    GUIEDIT_PASSWORD_CHAR,
//                    str_len,
//                    baseedit_ctrl_ptr->common_theme.font.font,
//                    (uint8) baseedit_ctrl_ptr->common_theme.char_space,
//                    line_width,
//                    char_num_ptr,
//                    string_pixel_ptr);
//        }
//        else
//        {
//            //get password string
//            password_str_ptr = GetPswString (baseedit_ctrl_ptr, start_pos, str_len);

//            GUI_GetStringInfoInLineWithDir (
//                    TEXT_DIR_LTR,
//                    password_str_ptr,
//                    str_len,
//                    baseedit_ctrl_ptr->common_theme.font.font,
//                    (uint8) baseedit_ctrl_ptr->common_theme.char_space,
//                    line_width,
//                    char_num_ptr,
//                    string_pixel_ptr,
//                    FALSE);
//        }

//        result = TRUE;
//    }

//    return (result);
//}

/*****************************************************************************/
//  Description : start password timer id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartPswTimer (
    CTRLPSWEDIT_OBJ_T   *pswedit_ctrl_ptr
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) pswedit_ctrl_ptr;
    //CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) pswedit_ctrl_ptr;

    if ( 
        //(GUIEDIT_TYPE_PASSWORD == baseedit_ctrl_ptr->type) &&
        (MMK_IsFocusWin (base_ctrl_ptr->win_handle)) &&
        (0 < pswedit_ctrl_ptr->visible_len) &&
        (0 == pswedit_ctrl_ptr->pw_timer_id))
    {
        pswedit_ctrl_ptr->pw_timer_id = MMK_CreateWinTimer (
                base_ctrl_ptr->handle,
                pswedit_ctrl_ptr->theme.period,
                FALSE);
    }
}

/*****************************************************************************/
//  Description : stop password timer id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StopPswTimer (
    CTRLPSWEDIT_OBJ_T   *pswedit_ctrl_ptr
)
{
    BOOLEAN     result = FALSE;

    if (0 < pswedit_ctrl_ptr->pw_timer_id)
    {
        //stop timer
        MMK_StopTimer (pswedit_ctrl_ptr->pw_timer_id);
        pswedit_ctrl_ptr->pw_timer_id = 0;

        //not visible string
        pswedit_ctrl_ptr->visible_len = 0;

        result = TRUE;
    }

    return (result);
}

