/*****************************************************************************
** File Name:      ctrlphonenumedit.c                                               *
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

#include "ctrlphonenumedit.h"

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
//  Description : init phonenumedit class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PhoneNumEditCtrlInitVtbl (
    CTRLPHONENUMEDIT_VTBL_T        *phonenumedit_vtbl_ptr
);

/*****************************************************************************/
//  Description : phonenumedit construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PhoneNumEditCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLPHONENUMEDIT_INIT_PARAM_T   *phonenumedit_param_ptr
);

/*****************************************************************************/
//  Description : phonenumedit destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PhoneNumEditCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void PhoneNumEditCtrlPackInitParam (
    GUIEDIT_INIT_DATA_T             *phonenumedit_init_ptr,   //in
    CTRLPHONENUMEDIT_INIT_PARAM_T   *phonenumedit_param_ptr   //out
);

/*****************************************************************************/
//  Description : handle phonenumedit msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E PhoneNumEditCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : get phonenumedit pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLPHONENUMEDIT_OBJ_T* GetPhoneNumEditPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN PhoneNumEditTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
// 	Description : get align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E PhoneNumEditCtrlGetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr   //in:
);

/*****************************************************************************/
// 	Description : set align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void PhoneNumEditCtrlSetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr,  //in:
    GUI_ALIGN_E             align               //in
);

/*****************************************************************************/
// 	Description : get im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void PhoneNumEditCtrlGetImInfo (
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
LOCAL void PhoneNumEditCtrlSetImInfo (
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
LOCAL void PhoneNumEditCtrlConfigImKey (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : 设置横屏时的新edit
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void PhoneNumEditCtrlSetLandscapeEdit (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    MMI_HANDLE_T        landscape_edit          // [in] the dst landscape edit
);

/*****************************************************************************/
//  Description : 获取总行数
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL uint16 PhoneNumEditCtrlGetLineNum (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    uint16              display_width           // [in] width
);

/*****************************************************************************/
//  Description : 检测字符串是否合法
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PhoneNumEditCtrlIsStringValid (     // [ret] TRUE if char offset is valid
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    wchar                   *str_ptr,
    uint16                  str_len
);

/*****************************************************************************/
//  Description : set line info,include start position,total line number and top line index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PhoneNumEditCtrlParseText (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,     //in:
    uint16              start_line_index        //in:
);

/*****************************************************************************/
//  Description : check edit font
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void CheckEditFont (
    CTRLPHONENUMEDIT_OBJ_T  *phonenumedit_ctrl_ptr      //in:
);

/*****************************************************************************/
//  Description : judge which font is perfect
//  Global resource dependence :
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL GUI_FONT_T GetPerfectFont (
    CTRLPHONENUMEDIT_OBJ_T  *phonenumedit_ctrl_ptr
);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get edit phone number string,截掉了非首位的+号以及后面的字符
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPNEDIT_GetPhoneNumString (
    MMI_CTRL_ID_T     ctrl_id,    //in:
    MMI_STRING_T      *str_ptr    //in:/out
)
{
    CTRLPHONENUMEDIT_OBJ_T* phonenumedit_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T* baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    phonenumedit_ctrl_ptr = GetPhoneNumEditPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)phonenumedit_ctrl_ptr;

    if ((PNULL != phonenumedit_ctrl_ptr) && (PNULL != str_ptr))
    {
        //set string
        str_ptr->wstr_ptr = baseedit_ctrl_ptr->str_ptr;
        str_ptr->wstr_len = baseedit_ctrl_ptr->str_len;

        //截掉了非首位的+号以及后面的字符
        str_ptr->wstr_len = BASEFLEX_InterceptPhoneNum (str_ptr->wstr_ptr, str_ptr->wstr_len);
    }
}

/*****************************************************************************/
//  Description :设置phone number edit当输入满,再输入字符,是否删除第一个字符
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPNEDIT_SetPhoneNumStyle (
    BOOLEAN        is_delete,
    MMI_CTRL_ID_T  ctrl_id
)
{
    CTRLPHONENUMEDIT_OBJ_T* phonenumedit_ctrl_ptr = PNULL;
    CTRLBASEFLEX_OBJ_T* baseflex_ctrl_ptr = PNULL;

    //get edit pointer by control id
    phonenumedit_ctrl_ptr = GetPhoneNumEditPtr (ctrl_id);
    baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*)phonenumedit_ctrl_ptr;

    if (PNULL != phonenumedit_ctrl_ptr)
    {
        baseflex_ctrl_ptr->is_del_after_full = is_delete;
    }
}

/*****************************************************************************/
//  Description : set phone number edit display direction
//  Global resource dependence :
//  Author: Jassmine
//  Note: only for phone number multi line
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPNEDIT_SetPhoneNumDir (
    MMI_CTRL_ID_T             ctrl_id,    //in
    GUIEDIT_DISPLAY_DIR_E     display_dir //in
)
{
    BOOLEAN             result = FALSE;
    CTRLPHONENUMEDIT_OBJ_T* phonenumedit_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T* baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    phonenumedit_ctrl_ptr = GetPhoneNumEditPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)phonenumedit_ctrl_ptr;

    if ((PNULL != phonenumedit_ctrl_ptr) &&
        ((GUIEDIT_STYLE_MULTI == baseedit_ctrl_ptr->style) || (GUIEDIT_STYLE_ADAPTIVE == baseedit_ctrl_ptr->style)))
    {
        //set
        baseedit_ctrl_ptr->display_dir = display_dir;

        //set align
        switch (display_dir)
        {
        case GUIEDIT_DISPLAY_DIR_RB:
            phonenumedit_ctrl_ptr->theme.align = ALIGN_RVMIDDLE;
            break;

        default:
            phonenumedit_ctrl_ptr->theme.align = ALIGN_LVMIDDLE;
            break;
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set dtmf num for phonenum
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPNEDIT_SetDtmfNum (
    MMI_CTRL_ID_T             ctrl_id,    //in
    BOOLEAN                   is_dtmf_num //in
)
{
    BOOLEAN             result = FALSE;
    CTRLPHONENUMEDIT_OBJ_T* phonenumedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    phonenumedit_ctrl_ptr = GetPhoneNumEditPtr (ctrl_id);

    if (PNULL != phonenumedit_ctrl_ptr)
    {
        phonenumedit_ctrl_ptr->is_dtmf_num = is_dtmf_num;
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : set window intercept edit control msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPNEDIT_SetWinInterceptCtrlMsg (
    MMI_CTRL_ID_T                    ctrl_id,
    GUIEDIT_WIN_INTERCEPT_CTRL_MSG   func
)
{
    CTRLPHONENUMEDIT_OBJ_T* phonenumedit_ctrl_ptr = PNULL;
    CTRLBASEFLEX_OBJ_T* baseflex_ctrl_ptr = PNULL;

    //get edit pointer by control id
    phonenumedit_ctrl_ptr = GetPhoneNumEditPtr (ctrl_id);
    baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*)phonenumedit_ctrl_ptr;

    if (PNULL != phonenumedit_ctrl_ptr)
    {
        //set function
        baseflex_ctrl_ptr->WinInterceptCtrlMsgFunc = func;
    }
}

/*****************************************************************************/
//  Description : set edit font extend
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPNEDIT_SetFontEx (
    MMI_CTRL_ID_T   ctrl_id,      //in
    GUIEDIT_FONT_T  *font_ptr     //in
)
{
    CTRLPHONENUMEDIT_OBJ_T  *phonenumedit_ctrl_ptr = PNULL;
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T      *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    phonenumedit_ctrl_ptr = GetPhoneNumEditPtr (ctrl_id);
    baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*)phonenumedit_ctrl_ptr;
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)phonenumedit_ctrl_ptr;

    //set font
    if (PNULL != phonenumedit_ctrl_ptr && PNULL != font_ptr)
    {
        phonenumedit_ctrl_ptr->edit_font = *font_ptr;
        baseedit_ctrl_ptr->common_theme.font.font = font_ptr->big_font;

        //re-parse edit text
        BASEFLEX_ReparseEditText (baseflex_ctrl_ptr);

        //notify edit change
        BASEEDIT_NotifyLineMsg (baseedit_ctrl_ptr, 0);
    }
}

/*****************************************************************************/
//  Description : set edit line is adaptive or not
//  Global resource dependence :
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPNEDIT_SetDialEditAdaptive (
    MMI_CTRL_ID_T   ctrl_id,                //in
    BOOLEAN         is_font_adaptive,       //in
    BOOLEAN         is_line_adaptive        //in
)
{
    CTRLPHONENUMEDIT_OBJ_T* phonenumedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    phonenumedit_ctrl_ptr = GetPhoneNumEditPtr (ctrl_id);

    //set font
    if (PNULL != phonenumedit_ctrl_ptr)
    {
        phonenumedit_ctrl_ptr->is_dial_font_adaptive = is_font_adaptive;
        phonenumedit_ctrl_ptr->is_dial_line_adaptive = is_line_adaptive;
    }
}

/*****************************************************************************/
//  Description : get phonenumedit type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T PHONENUMEDIT_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASEFLEX_TYPE,                                     // parent class type
                       "phonenumedit",                                         // control name
                       sizeof (CTRLPHONENUMEDIT_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) PhoneNumEditCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) PhoneNumEditCtrlDestruct,      // control destructor function
                       sizeof (CTRLPHONENUMEDIT_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) PhoneNumEditCtrlPackInitParam,     // convert init param
                       sizeof (CTRLPHONENUMEDIT_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) PhoneNumEditCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init phonenumedit class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PhoneNumEditCtrlInitVtbl (
    CTRLPHONENUMEDIT_VTBL_T        *phonenumedit_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) phonenumedit_vtbl_ptr;
    CTRLBASEEDIT_VTBL_T *baseedit_ctrl_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*) phonenumedit_vtbl_ptr;
    CTRLBASEFLEX_VTBL_T *baseflex_ctrl_vtbl_ptr = (CTRLBASEFLEX_VTBL_T*) phonenumedit_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = PhoneNumEditCtrlHandleMsg;

    baseedit_ctrl_vtbl_ptr->GetAlign = PhoneNumEditCtrlGetAlign;
    baseedit_ctrl_vtbl_ptr->SetAlign = PhoneNumEditCtrlSetAlign;
    baseedit_ctrl_vtbl_ptr->GetImInfo = PhoneNumEditCtrlGetImInfo;
    baseedit_ctrl_vtbl_ptr->SetImInfo = PhoneNumEditCtrlSetImInfo;
    baseedit_ctrl_vtbl_ptr->ConfigImKey = PhoneNumEditCtrlConfigImKey;
    baseedit_ctrl_vtbl_ptr->SetLandscapeEdit = PhoneNumEditCtrlSetLandscapeEdit;
    baseedit_ctrl_vtbl_ptr->GetLineNum = PhoneNumEditCtrlGetLineNum;

    baseflex_ctrl_vtbl_ptr->IsStringValid = PhoneNumEditCtrlIsStringValid;
    baseflex_ctrl_vtbl_ptr->ParseText = PhoneNumEditCtrlParseText;

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
LOCAL void PhoneNumEditCtrlPackInitParam (
    GUIEDIT_INIT_DATA_T             *phonenumedit_init_ptr,   //in
    CTRLPHONENUMEDIT_INIT_PARAM_T   *phonenumedit_param_ptr   //out
)
{
    CTRLBASE_INIT_PARAM_T       *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) phonenumedit_param_ptr;
    CTRLBASEEDIT_INIT_PARAM_T   *baseedit_ctrl_param_ptr = (CTRLBASEEDIT_INIT_PARAM_T*) phonenumedit_param_ptr;

    if (PNULL == phonenumedit_init_ptr || PNULL == phonenumedit_param_ptr)
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = phonenumedit_init_ptr->both_rect;

    baseedit_ctrl_param_ptr->is_rect_adaptive = phonenumedit_init_ptr->is_rect_adaptive;
    baseedit_ctrl_param_ptr->str_max_len = phonenumedit_init_ptr->str_max_len;
    baseedit_ctrl_param_ptr->type = phonenumedit_init_ptr->type;
}

/*****************************************************************************/
//  Description : phonenumedit construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PhoneNumEditCtrlConstruct (
    CTRLBASE_OBJ_T                  *base_ctrl_ptr,
    CTRLPHONENUMEDIT_INIT_PARAM_T   *phonenumedit_param_ptr
)
{
    BOOLEAN                 result = TRUE;
    CTRLPHONENUMEDIT_OBJ_T  *phonenumedit_ctrl_ptr = (CTRLPHONENUMEDIT_OBJ_T*) base_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T      *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) base_ctrl_ptr;
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr || PNULL == phonenumedit_param_ptr)
    {
        return FALSE;
    }

    MMITHEME_GetPhoneNumEditTheme (&phonenumedit_ctrl_ptr->theme);

    //set display im icon and number info
    baseedit_ctrl_ptr->is_disp_im_icon  = TRUE;
    baseflex_ctrl_ptr->is_disp_num_info = TRUE;

    BASEEDIT_Init(baseedit_ctrl_ptr);

    return (result);
}

/*****************************************************************************/
//  Description : phonenumedit destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PhoneNumEditCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN                 result = TRUE;
    CTRLPHONENUMEDIT_OBJ_T  *phonenumedit_ctrl_ptr = (CTRLPHONENUMEDIT_OBJ_T*) base_ctrl_ptr;

    if (PNULL == phonenumedit_ctrl_ptr)
    {
        return FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle phonenumedit msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E PhoneNumEditCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    CTRLBASE_VTBL_T         *parent_vtbl_ptr = PNULL;
    CTRLPHONENUMEDIT_OBJ_T  *phonenumedit_ctrl_ptr = (CTRLPHONENUMEDIT_OBJ_T*) base_ctrl_ptr;

    if (PNULL == phonenumedit_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_PHONENUMEDIT_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    result = parent_vtbl_ptr->HandleEvent (base_ctrl_ptr, msg_id, param);

    return (result);
}

/*****************************************************************************/
//  Description : get phonenumedit pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLPHONENUMEDIT_OBJ_T* GetPhoneNumEditPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        if (!PhoneNumEditTypeOf (ctrl_ptr))
        {
            SCI_TRACE_LOW("== PhoneNumEditTypeOf error == 0x%x", ctrl_ptr);
            ctrl_ptr = PNULL;
        }
    }

    return (CTRLPHONENUMEDIT_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN PhoneNumEditTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf ( (OBJECT_TYPE_PTR) ctrl_ptr, CTRL_PHONENUMEDIT_TYPE);
}

/*****************************************************************************/
// 	Description : get align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E PhoneNumEditCtrlGetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr   //in:
)
{
    CTRLPHONENUMEDIT_OBJ_T* phonenumedit_ctrl_ptr = (CTRLPHONENUMEDIT_OBJ_T*) baseedit_ctrl_ptr;

    return phonenumedit_ctrl_ptr->theme.align;
}

/*****************************************************************************/
// 	Description : set align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void PhoneNumEditCtrlSetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr,  //in:
    GUI_ALIGN_E             align               //in
)
{
    CTRLPHONENUMEDIT_OBJ_T* phonenumedit_ctrl_ptr = (CTRLPHONENUMEDIT_OBJ_T*) baseedit_ctrl_ptr;

    phonenumedit_ctrl_ptr->theme.align = align;
}

/*****************************************************************************/
// 	Description : get im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void PhoneNumEditCtrlGetImInfo (
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
    CTRLPHONENUMEDIT_OBJ_T* phonenumedit_ctrl_ptr = (CTRLPHONENUMEDIT_OBJ_T*) baseedit_ctrl_ptr;

    *is_forbid_ptr    = phonenumedit_ctrl_ptr->theme.im.is_forbid_symbol;
    *init_input_ptr   = phonenumedit_ctrl_ptr->theme.im.init_input_type;
    *allow_input_ptr  = phonenumedit_ctrl_ptr->theme.im.allow_input_type;
    *init_im_ptr      = phonenumedit_ctrl_ptr->theme.im.init_im;
    *allow_im_ptr     = phonenumedit_ctrl_ptr->theme.im.allow_im;
    *capital_mode_ptr = phonenumedit_ctrl_ptr->theme.im.capital_mode;
    *tag_ptr          = phonenumedit_ctrl_ptr->theme.im.tag;
}

/*****************************************************************************/
// 	Description : set im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void PhoneNumEditCtrlSetImInfo (
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
    CTRLPHONENUMEDIT_OBJ_T* phonenumedit_ctrl_ptr = (CTRLPHONENUMEDIT_OBJ_T*) baseedit_ctrl_ptr;

    if (PNULL != is_forbid_ptr)
    {
        phonenumedit_ctrl_ptr->theme.im.is_forbid_symbol = *is_forbid_ptr;
    }

    if (PNULL != init_im_ptr)
    {
        phonenumedit_ctrl_ptr->theme.im.init_im = *init_im_ptr;
    }

    if (PNULL != allow_im_ptr)
    {
        phonenumedit_ctrl_ptr->theme.im.allow_im = *allow_im_ptr;
    }

    if (PNULL != init_type_ptr)
    {
        phonenumedit_ctrl_ptr->theme.im.init_input_type = *init_type_ptr;
    }

    if (PNULL != allow_type_ptr)
    {
        phonenumedit_ctrl_ptr->theme.im.allow_input_type = *allow_type_ptr;
    }

    if (PNULL != capital_mode_ptr)
    {
        phonenumedit_ctrl_ptr->theme.im.capital_mode = *capital_mode_ptr;
    }

    if (PNULL != tag_ptr)
    {
        phonenumedit_ctrl_ptr->theme.im.tag = *tag_ptr;
    }
}

/*****************************************************************************/
// 	Description : config im default key
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void PhoneNumEditCtrlConfigImKey (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr
)
{
    uint16      highlight_min_pos = 0;
    wchar       *star_value_ptr = PNULL;
    wchar       phonenum_star_array[4] = {'*', '+', 'P', 'W'};
    uint32      star_value_count = 0;
    wchar       hash_value_array[1] = {'#'};
    uint32      hash_value_count = 0;
    CTRLPHONENUMEDIT_OBJ_T* phonenumedit_ctrl_ptr = (CTRLPHONENUMEDIT_OBJ_T*) baseedit_ctrl_ptr;

    if (0 != baseedit_ctrl_ptr->im_ctrl_handle)
    {
        if (phonenumedit_ctrl_ptr->is_dtmf_num)
        {
            //set start value and count
            star_value_ptr = phonenum_star_array;

            //set star value count
            star_value_count = 1;
        }
        else
        {
            //get highlight min pos
            highlight_min_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);

            //set start value and count
            star_value_ptr = phonenum_star_array;

            if (0 == highlight_min_pos)
            {
                star_value_count = 2;
            }
            else
            {
                star_value_count = 4;
            }
        }

        //set hash value count
        hash_value_count = 1;

        //config im *
        GUIIM_SetCustomKey (
                baseedit_ctrl_ptr->im_ctrl_handle,
                GUIIM_TYPE_DIGITAL,
                GUIIM_CUSTOM_KEY_STAR,
                star_value_ptr,
                star_value_count);

        //config im #
        GUIIM_SetCustomKey (
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
LOCAL void PhoneNumEditCtrlSetLandscapeEdit (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    MMI_HANDLE_T        landscape_edit          // [in] the dst landscape edit
)
{
    CTRLPHONENUMEDIT_OBJ_T  *new_edit_ctrl_ptr = GetPhoneNumEditPtr (landscape_edit);
    CTRLPHONENUMEDIT_OBJ_T  *phonenumedit_ctrl_ptr = (CTRLPHONENUMEDIT_OBJ_T*) baseedit_ctrl_ptr;
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*) baseedit_ctrl_ptr;

    if (PNULL != new_edit_ctrl_ptr)
    {
        //set string max len
        CTRLBASEFLEX_SetMaxLen (landscape_edit, baseedit_ctrl_ptr->str_max_len);

        //set style
        CTRLBASEFLEX_SetStyle (landscape_edit, baseedit_ctrl_ptr->style);

        //set phone number style
        CTRLPNEDIT_SetPhoneNumStyle (baseflex_ctrl_ptr->is_del_after_full, landscape_edit);

        //set dir
        CTRLPNEDIT_SetPhoneNumDir (landscape_edit, baseedit_ctrl_ptr->display_dir);

        //set string
        CTRLBASEFLEX_SetString (landscape_edit, baseedit_ctrl_ptr->str_ptr, baseedit_ctrl_ptr->str_len);

        //set im
        new_edit_ctrl_ptr->theme.im = phonenumedit_ctrl_ptr->theme.im;

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
//  Description : 获取总行数
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL uint16 PhoneNumEditCtrlGetLineNum (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    uint16              display_width           // [in] width
)
{
    uint16      total_line_num = 0;
    wchar       *str_ptr = baseedit_ctrl_ptr->str_ptr;
    wchar       str_len = baseedit_ctrl_ptr->str_len;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*) baseedit_ctrl_ptr;

    if (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)
    {
        total_line_num = 1;
    }
    else
    {
        if (GUIEDIT_DISPLAY_DIR_RB == baseedit_ctrl_ptr->display_dir)
        {
            //reset string
            str_ptr = PNULL;
            str_len = 0;

            //get reverse string
            BASEFLEX_GetReverseString (baseflex_ctrl_ptr, &str_ptr, &str_len);
        }

        total_line_num = GUI_CalculateStringLinesByPixelNum (
                             display_width,
                             str_ptr,
                             str_len,
                             baseedit_ctrl_ptr->common_theme.font.font,
                             (uint8) baseedit_ctrl_ptr->common_theme.char_space,
                             baseflex_ctrl_ptr->is_byword);

        //is carriage return or line feed
        if (BASEEDIT_IsNewLineChar (baseedit_ctrl_ptr, str_len))
        {
            //cursor display the next line
            total_line_num++;
        }

        if (GUIEDIT_DISPLAY_DIR_RB == baseedit_ctrl_ptr->display_dir)
        {
            //free reverse string
            BASEFLEX_FreeReverseString (&str_ptr);
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
LOCAL BOOLEAN PhoneNumEditCtrlIsStringValid (     // [ret] TRUE if char offset is valid
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    wchar                   *str_ptr,
    uint16                  str_len
)
{
    BOOLEAN     result = TRUE;
    uint16      i = 0;
    uint16      start_pos = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) baseflex_ctrl_ptr;

    if (0 < str_len)
    {
        //check the first char,digital,*,+
        if (0 == baseedit_ctrl_ptr->cursor.cur_pos)
        {
            if ( (BASEFLEX_IsDigitalChar (str_ptr[0])) ||
                    ('*' == str_ptr[0]) ||
                    ('+' == str_ptr[0]) ||
                    ('#' == str_ptr[0]))
            {
                start_pos = 1;
            }
            else
            {
                result = FALSE;
            }
        }

        if (result)
        {
            //check phone number string
            for (i = start_pos; i < str_len; i++)
            {
                if ( (!BASEFLEX_IsDigitalChar (str_ptr[i])) &&
                        ('*' != str_ptr[i]) &&
                        ('+' != str_ptr[i]) &&
                        ('#' != str_ptr[i]) &&
                        ('P' != str_ptr[i]) &&
                        ('W' != str_ptr[i]))
                {
                    result = FALSE;
                    break;
                }
            }
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : set line info,include start position,total line number and top line index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN PhoneNumEditCtrlParseText (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,     //in:
    uint16              start_line_index        //in:
)
{
    BOOLEAN         result = FALSE;
    CTRLBASEFLEX_VTBL_T *baseflex_vtbl_ptr = (CTRLBASEFLEX_VTBL_T*)TYPEMNG_GetParentVtbl(
            CTRL_PHONENUMEDIT_TYPE, (OBJECT_TYPE_PTR)baseflex_ctrl_ptr);

    if (PNULL != baseflex_vtbl_ptr && PNULL != baseflex_vtbl_ptr->ParseText)
    {
        result = baseflex_vtbl_ptr->ParseText(baseflex_ctrl_ptr, start_line_index);

        if (result)
        {
            // 检查大小字体时，是否需要换字体
            CheckEditFont ((CTRLPHONENUMEDIT_OBJ_T*)baseflex_ctrl_ptr);
        }
        //set top line index
        BASEFLEX_MakeCursorVisible(baseflex_ctrl_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : check edit font
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void CheckEditFont (
    CTRLPHONENUMEDIT_OBJ_T  *phonenumedit_ctrl_ptr      //in:
)
{
    GUI_FONT_T      cur_font = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) phonenumedit_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)phonenumedit_ctrl_ptr;

    if (phonenumedit_ctrl_ptr->edit_font.is_valid && !phonenumedit_ctrl_ptr->is_font_checked)
    {
        // 防止递归调用，这里置标志
        phonenumedit_ctrl_ptr->is_font_checked = TRUE;

        if (GUIEDIT_STYLE_SINGLE == baseedit_ctrl_ptr->style)
        {
            if (phonenumedit_ctrl_ptr->is_dial_font_adaptive)
            {
                cur_font = GetPerfectFont (phonenumedit_ctrl_ptr);

                if (phonenumedit_ctrl_ptr->is_dial_line_adaptive && cur_font == phonenumedit_ctrl_ptr->edit_font.small_font)
                {
                    CTRLBASEFLEX_SetStyle (base_ctrl_ptr->handle, GUIEDIT_STYLE_MULTI);
                }
            }
            // 单行检查显示的起始位置
            else if (baseedit_ctrl_ptr->line_info_ptr[0].start_pos != 0
                     || baseedit_ctrl_ptr->line_info_ptr[1].start_pos != baseedit_ctrl_ptr->str_len)
            {
                cur_font = phonenumedit_ctrl_ptr->edit_font.small_font;
            }
            else
            {
                cur_font = phonenumedit_ctrl_ptr->edit_font.big_font;
            }
        }
        else
        {
            if (phonenumedit_ctrl_ptr->is_dial_font_adaptive)
            {
                cur_font = GetPerfectFont (phonenumedit_ctrl_ptr);

                if (phonenumedit_ctrl_ptr->is_dial_line_adaptive && cur_font != phonenumedit_ctrl_ptr->edit_font.small_font)
                {
                    CTRLBASEFLEX_SetStyle (base_ctrl_ptr->handle, GUIEDIT_STYLE_SINGLE);
                }
            }
            // 多行检查行数与一页的行数
            else if (BASEEDIT_GetTotalHeight(baseedit_ctrl_ptr) > BASEEDIT_GetDispHeight(baseedit_ctrl_ptr))
            {
                cur_font = phonenumedit_ctrl_ptr->edit_font.small_font;
            }
            else
            {
                cur_font = phonenumedit_ctrl_ptr->edit_font.big_font;
            }
        }

        // 与当前字体不一致时，重新设置
        if (cur_font != baseedit_ctrl_ptr->common_theme.font.font)
        {
            GUIEDIT_SetFont (base_ctrl_ptr->handle, cur_font);

            // 当切换到大字体时，有可能是小字体能显示下，大字体显示不下
            // 所以这里重新检查一遍
            if (cur_font == phonenumedit_ctrl_ptr->edit_font.big_font)
            {
                phonenumedit_ctrl_ptr->is_font_checked = FALSE;

                CheckEditFont (phonenumedit_ctrl_ptr);
            }
        }

        phonenumedit_ctrl_ptr->is_font_checked = FALSE;
    }
}

/*****************************************************************************/
//  Description : judge which font is perfect
//  Global resource dependence :
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL GUI_FONT_T GetPerfectFont (
    CTRLPHONENUMEDIT_OBJ_T  *phonenumedit_ctrl_ptr
)
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*) phonenumedit_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)phonenumedit_ctrl_ptr;

    int16 edit_width = base_ctrl_ptr->rect.right - base_ctrl_ptr->rect.left + 1 - 2 * baseedit_ctrl_ptr->common_theme.char_space;

    if (edit_width > GUI_GetStringWidthEx (phonenumedit_ctrl_ptr->edit_font.big_font, baseedit_ctrl_ptr->str_ptr, baseedit_ctrl_ptr->str_len, baseedit_ctrl_ptr->common_theme.char_space))
    {
        return  phonenumedit_ctrl_ptr->edit_font.big_font;
    }
    else if (edit_width > GUI_GetStringWidthEx (phonenumedit_ctrl_ptr->edit_font.mid_font, baseedit_ctrl_ptr->str_ptr, baseedit_ctrl_ptr->str_len, baseedit_ctrl_ptr->common_theme.char_space) + 2)
    {
        return  phonenumedit_ctrl_ptr->edit_font.mid_font;
    }
    else
    {
        return phonenumedit_ctrl_ptr->edit_font.small_font;
    }
}

