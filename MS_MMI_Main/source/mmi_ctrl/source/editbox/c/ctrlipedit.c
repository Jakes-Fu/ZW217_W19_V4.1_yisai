/*****************************************************************************
** File Name:      ctrlipedit.c                                               *
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
#include "ctrlipedit.h"
#include "ctrlipedit_export.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define GUIEDIT_IP_MAX_LEN      15  //000.000.000.000

#define GUIEDIT_IP_SEPARATOR            '.'

//edit IP bit
typedef enum
{
    GUIEDIT_IP_BIT_NULL,
    GUIEDIT_IP_BIT_HUNDRED, //千位
    GUIEDIT_IP_BIT_TEN,     //十位
    GUIEDIT_IP_BIT_ONE,     //个位
    GUIEDIT_IP_BIT_MAX
} GUIEDIT_IP_BIT_E;

//edit IP bit
typedef struct
{
    uint8               num_pos;//from 1,from left to right
    GUIEDIT_IP_BIT_E    bit;    //IP bit
} GUIEDIT_IP_BIT_T;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//IP对应的位数
LOCAL const GUIEDIT_IP_BIT_T s_guiedit_ip_bit[] =
{
    1, GUIEDIT_IP_BIT_HUNDRED, //1.千位
    1, GUIEDIT_IP_BIT_TEN,    //1.百位
    1, GUIEDIT_IP_BIT_ONE,    //1.十位

    0, GUIEDIT_IP_BIT_NULL,   //separator

    2, GUIEDIT_IP_BIT_HUNDRED, //2.千位
    2, GUIEDIT_IP_BIT_TEN,    //2.百位
    2, GUIEDIT_IP_BIT_ONE,    //2.十位

    0, GUIEDIT_IP_BIT_NULL,   //separator

    3, GUIEDIT_IP_BIT_HUNDRED, //3.千位
    3, GUIEDIT_IP_BIT_TEN,    //3.百位
    3, GUIEDIT_IP_BIT_ONE,    //3.十位

    0, GUIEDIT_IP_BIT_NULL,   //separator

    4, GUIEDIT_IP_BIT_HUNDRED, //4.千位
    4, GUIEDIT_IP_BIT_TEN,    //4.百位
    4, GUIEDIT_IP_BIT_ONE,    //4.十位
};

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
//  Description : init ipedit class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IPEditCtrlInitVtbl (
    CTRLIPEDIT_VTBL_T        *ipedit_vtbl_ptr
);

/*****************************************************************************/
//  Description : ipedit construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IPEditCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLIPEDIT_INIT_PARAM_T   *ipedit_param_ptr
);

/*****************************************************************************/
//  Description : ipedit destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IPEditCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void IPEditCtrlPackInitParam (
    GUIEDIT_INIT_DATA_T         *ipedit_init_ptr,   //in
    CTRLIPEDIT_INIT_PARAM_T     *ipedit_param_ptr   //out
);

/*****************************************************************************/
//  Description : handle ipedit msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E IPEditCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : get ipedit pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLIPEDIT_OBJ_T* GetIPEditPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN IPEditTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
// 	Description : get align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E IPEditCtrlGetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr   //in:
);

/*****************************************************************************/
// 	Description : set align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void IPEditCtrlSetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr,  //in:
    GUI_ALIGN_E             align               //in
);

/*****************************************************************************/
// 	Description : get im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void IPEditCtrlGetImInfo (
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
LOCAL void IPEditCtrlSetImInfo (
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
LOCAL void IPEditCtrlSetLandscapeEdit (
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
LOCAL BOOLEAN IPEditCtrlIsValidOffset (             // [ret] TRUE if char offset is valid
    CTRLBASEFIXED_OBJ_T     *basefixed_ctrl_ptr,    // [in] ctrl ptr
    uint16                  *char_offset_ptr,       // [in/out] char offset, and may change the offset if valid
    uint16                  *highlight_len_ptr      // [out] highlight length if char valid
);

/*****************************************************************************/
//  Description : get IP string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void IPEditCtrlConvertToStr (
    CTRLBASEFIXED_OBJ_T     *basefixed_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle IP edit up/down key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIPEditUpDownKey (
    CTRLIPEDIT_OBJ_T    *ipedit_ctrl_ptr,
    BOOLEAN             is_add
);

/*****************************************************************************/
//  Description : handle IP edit up/down/number key,get new IP and IP string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNewIP (//up/down:is change;number:is valid
    CTRLIPEDIT_OBJ_T    *ipedit_ctrl_ptr,
    BOOLEAN             is_num,     //TRUE:number key:FALSE:up/down key
    BOOLEAN             is_add,
    uint8               num_value   //from 0 to 9
);

/*****************************************************************************/
//  Description : get current IP bit
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIEDIT_IP_BIT_T GetCurIPBit (
    CTRLIPEDIT_OBJ_T    *ipedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle IP edit left key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIPEditLeftKey (
    CTRLIPEDIT_OBJ_T    *ipedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle IP edit right key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIPEditRightKey (
    CTRLIPEDIT_OBJ_T    *ipedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : move IP cursor to right
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MoveIPCursorToRight (
    CTRLIPEDIT_OBJ_T    *ipedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : IP handle im commit msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleIPImCommit (
    CTRLIPEDIT_OBJ_T    *ipedit_ctrl_ptr,
    GUIEDIT_ADD_STR_T   *add_str_ptr
);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get IP
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint32 CTRLIPEDIT_GetIP (
    MMI_CTRL_ID_T   ctrl_id
)
{
    uint32          ip_addr = 0;
    CTRLIPEDIT_OBJ_T    *ipedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    ipedit_ctrl_ptr = GetIPEditPtr (ctrl_id);

    if (PNULL != ipedit_ctrl_ptr)
    {
        ip_addr = ipedit_ctrl_ptr->ip_addr;
    }

    return (ip_addr);
}

/*****************************************************************************/
//  Description : set IP
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLIPEDIT_SetIP (
    MMI_CTRL_ID_T   ctrl_id,
    uint32          ip_addr
)
{
    CTRLIPEDIT_OBJ_T    *ipedit_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    ipedit_ctrl_ptr = GetIPEditPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) ipedit_ctrl_ptr;

    if (PNULL != ipedit_ctrl_ptr)
    {
        //set IP
        ipedit_ctrl_ptr->ip_addr = ip_addr;

        //reset length,get new IP string
        baseedit_ctrl_ptr->str_len = 0;
    }
}

/*****************************************************************************/
//  Description : get ipedit type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T IPEDIT_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASEFIXED_TYPE,                              // parent class type
                       "ipedit",                                         // control name
                       sizeof (CTRLIPEDIT_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) IPEditCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) IPEditCtrlDestruct,      // control destructor function
                       sizeof (CTRLIPEDIT_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) IPEditCtrlPackInitParam,     // convert init param
                       sizeof (CTRLIPEDIT_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) IPEditCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init ipedit class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IPEditCtrlInitVtbl (
    CTRLIPEDIT_VTBL_T        *ipedit_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) ipedit_vtbl_ptr;
    CTRLBASEEDIT_VTBL_T *baseedit_ctrl_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*) ipedit_vtbl_ptr;
    CTRLBASEFIXED_VTBL_T *basefixed_ctrl_vtbl_ptr = (CTRLBASEFIXED_VTBL_T*) ipedit_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = IPEditCtrlHandleMsg;

    baseedit_ctrl_vtbl_ptr->GetAlign = IPEditCtrlGetAlign;
    baseedit_ctrl_vtbl_ptr->SetAlign = IPEditCtrlSetAlign;
    baseedit_ctrl_vtbl_ptr->GetImInfo = IPEditCtrlGetImInfo;
    baseedit_ctrl_vtbl_ptr->SetImInfo = IPEditCtrlSetImInfo;
    baseedit_ctrl_vtbl_ptr->SetLandscapeEdit = IPEditCtrlSetLandscapeEdit;

    basefixed_ctrl_vtbl_ptr->IsValidOffset = IPEditCtrlIsValidOffset;
    basefixed_ctrl_vtbl_ptr->ConvertToStr = IPEditCtrlConvertToStr;

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
LOCAL void IPEditCtrlPackInitParam (
    GUIEDIT_INIT_DATA_T         *ipedit_init_ptr,   //in
    CTRLIPEDIT_INIT_PARAM_T     *ipedit_param_ptr   //out
)
{
    CTRLBASE_INIT_PARAM_T       *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) ipedit_param_ptr;
    CTRLBASEEDIT_INIT_PARAM_T   *baseedit_ctrl_param_ptr = (CTRLBASEEDIT_INIT_PARAM_T*) ipedit_param_ptr;

    if (PNULL == ipedit_init_ptr || PNULL == ipedit_param_ptr)
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = ipedit_init_ptr->both_rect;

    baseedit_ctrl_param_ptr->is_rect_adaptive = ipedit_init_ptr->is_rect_adaptive;
    baseedit_ctrl_param_ptr->str_max_len = ipedit_init_ptr->str_max_len;
    baseedit_ctrl_param_ptr->type = ipedit_init_ptr->type;
}

/*****************************************************************************/
//  Description : ipedit construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IPEditCtrlConstruct (
    CTRLBASE_OBJ_T              *base_ctrl_ptr,
    CTRLIPEDIT_INIT_PARAM_T     *ipedit_param_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLIPEDIT_OBJ_T    *ipedit_ctrl_ptr = (CTRLIPEDIT_OBJ_T*) base_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr || PNULL == ipedit_param_ptr)
    {
        return FALSE;
    }

    MMITHEME_GetIPEditTheme (&ipedit_ctrl_ptr->theme);

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
    baseedit_ctrl_ptr->str_max_len = GUIEDIT_IP_MAX_LEN;

    BASEEDIT_Init(baseedit_ctrl_ptr);

    return (result);
}

/*****************************************************************************/
//  Description : ipedit destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IPEditCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLIPEDIT_OBJ_T     *ipedit_ctrl_ptr = (CTRLIPEDIT_OBJ_T*) base_ctrl_ptr;

    if (PNULL == ipedit_ctrl_ptr)
    {
        return FALSE;
    }

    // TODO
    // ...

    return (result);
}

/*****************************************************************************/
//  Description : handle ipedit msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E IPEditCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLIPEDIT_OBJ_T    *ipedit_ctrl_ptr = (CTRLIPEDIT_OBJ_T*) base_ctrl_ptr;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = PNULL;

    if (PNULL == ipedit_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_APP_UP:
    case MSG_KEYREPEAT_UP:
        result = HandleIPEditUpDownKey (ipedit_ctrl_ptr, TRUE);
        break;

    case MSG_APP_DOWN:
    case MSG_KEYREPEAT_DOWN:
        result = HandleIPEditUpDownKey (ipedit_ctrl_ptr, FALSE);
        break;

    case MSG_APP_LEFT:
    case MSG_KEYREPEAT_LEFT:
        result = HandleIPEditLeftKey (ipedit_ctrl_ptr);
        break;

    case MSG_APP_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
        result = HandleIPEditRightKey (ipedit_ctrl_ptr);
        break;

    case MSG_NOTIFY_IM_COMMIT:
        {
            GUIEDIT_ADD_STR_T   add_str = {0};

            BASEEDIT_ConvertImCommitMsg(param, &add_str);

            //Date,Time,IP need handle number key msg
            HandleIPImCommit (ipedit_ctrl_ptr, &add_str);
        }
        break;

    default:
        parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_IPEDIT_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
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
LOCAL GUI_ALIGN_E IPEditCtrlGetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr   //in:
)
{
    CTRLIPEDIT_OBJ_T* ipedit_ctrl_ptr = (CTRLIPEDIT_OBJ_T*) baseedit_ctrl_ptr;

    return ipedit_ctrl_ptr->theme.align;
}

/*****************************************************************************/
// 	Description : set align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void IPEditCtrlSetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr,  //in:
    GUI_ALIGN_E             align               //in
)
{
    CTRLIPEDIT_OBJ_T* ipedit_ctrl_ptr = (CTRLIPEDIT_OBJ_T*) baseedit_ctrl_ptr;

    ipedit_ctrl_ptr->theme.align = align;
}

/*****************************************************************************/
// 	Description : get im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void IPEditCtrlGetImInfo (
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
    CTRLIPEDIT_OBJ_T* ipedit_ctrl_ptr = (CTRLIPEDIT_OBJ_T*) baseedit_ctrl_ptr;

    *is_forbid_ptr    = ipedit_ctrl_ptr->theme.im.is_forbid_symbol;
    *init_input_ptr   = ipedit_ctrl_ptr->theme.im.init_input_type;
    *allow_input_ptr  = ipedit_ctrl_ptr->theme.im.allow_input_type;
    *init_im_ptr      = ipedit_ctrl_ptr->theme.im.init_im;
    *allow_im_ptr     = ipedit_ctrl_ptr->theme.im.allow_im;
    *capital_mode_ptr = ipedit_ctrl_ptr->theme.im.capital_mode;
    *tag_ptr          = ipedit_ctrl_ptr->theme.im.tag;
}

/*****************************************************************************/
// 	Description : set im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void IPEditCtrlSetImInfo (
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
    CTRLIPEDIT_OBJ_T* ipedit_ctrl_ptr = (CTRLIPEDIT_OBJ_T*) baseedit_ctrl_ptr;

    if (PNULL != is_forbid_ptr)
    {
        ipedit_ctrl_ptr->theme.im.is_forbid_symbol = *is_forbid_ptr;
    }

    if (PNULL != init_im_ptr)
    {
        ipedit_ctrl_ptr->theme.im.init_im = *init_im_ptr;
    }

    if (PNULL != allow_im_ptr)
    {
        ipedit_ctrl_ptr->theme.im.allow_im = *allow_im_ptr;
    }

    if (PNULL != init_type_ptr)
    {
        ipedit_ctrl_ptr->theme.im.init_input_type = *init_type_ptr;
    }

    if (PNULL != allow_type_ptr)
    {
        ipedit_ctrl_ptr->theme.im.allow_input_type = *allow_type_ptr;
    }

    if (PNULL != capital_mode_ptr)
    {
        ipedit_ctrl_ptr->theme.im.capital_mode = *capital_mode_ptr;
    }

    if (PNULL != tag_ptr)
    {
        ipedit_ctrl_ptr->theme.im.tag = *tag_ptr;
    }
}

/*****************************************************************************/
//  Description : 设置横屏时的新edit
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void IPEditCtrlSetLandscapeEdit (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    MMI_HANDLE_T        landscape_edit          // [in] the dst landscape edit
)
{
    CTRLIPEDIT_OBJ_T    *ipedit_ctrl_ptr = (CTRLIPEDIT_OBJ_T*) baseedit_ctrl_ptr;
    CTRLIPEDIT_OBJ_T    *new_edit_ctrl_ptr = GetIPEditPtr (landscape_edit);
    CTRLBASEEDIT_OBJ_T  *new_baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) new_edit_ctrl_ptr;

    if (PNULL != new_edit_ctrl_ptr)
    {
        //set ip
        CTRLIPEDIT_SetIP (landscape_edit, ipedit_ctrl_ptr->ip_addr);

        //set im
        new_edit_ctrl_ptr->theme.im = ipedit_ctrl_ptr->theme.im;

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
LOCAL BOOLEAN IPEditCtrlIsValidOffset (             // [ret] TRUE if char offset is valid
    CTRLBASEFIXED_OBJ_T     *basefixed_ctrl_ptr,    // [in] ctrl ptr
    uint16                  *char_offset_ptr,       // [in/out] char offset, and may change the offset if valid
    uint16                  *highlight_len_ptr      // [out] highlight length if char valid
)
{
    BOOLEAN             result = TRUE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) basefixed_ctrl_ptr;

    if (GUIEDIT_IP_SEPARATOR == baseedit_ctrl_ptr->str_ptr[*char_offset_ptr])
    {
        result = FALSE;
    }

    *highlight_len_ptr = 1;

    return result;
}

/*****************************************************************************/
//  Description : get IP string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void IPEditCtrlConvertToStr (
    CTRLBASEFIXED_OBJ_T     *basefixed_ctrl_ptr
)
{
    uint8       ip_num = 0;
    uint8       ip_hundred = 0;
    uint8       ip_ten = 0;
    uint8       ip_one = 0;
    uint16      i = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) basefixed_ctrl_ptr;
    CTRLIPEDIT_OBJ_T    *ipedit_ctrl_ptr = (CTRLIPEDIT_OBJ_T*) basefixed_ctrl_ptr;

    if (0 == baseedit_ctrl_ptr->str_len)
    {
        //get the first ip num
        ip_num     = (uint8) (ipedit_ctrl_ptr->ip_addr >> 24);
        ip_hundred = (uint8) (ip_num / 100);
        ip_ten     = (uint8) ( (ip_num % 100) / 10);
        ip_one     = (uint8) (ip_num % 10);

        baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (ip_hundred + '0');
        baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (ip_ten + '0');
        baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (ip_one + '0');
        baseedit_ctrl_ptr->str_ptr[i++] = GUIEDIT_IP_SEPARATOR;

        //get the second ip num
        ip_num     = (uint8) ( (ipedit_ctrl_ptr->ip_addr & 0xFF0000) >> 16);
        ip_hundred = (uint8) (ip_num / 100);
        ip_ten     = (uint8) ( (ip_num % 100) / 10);
        ip_one     = (uint8) (ip_num % 10);

        baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (ip_hundred + '0');
        baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (ip_ten + '0');
        baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (ip_one + '0');
        baseedit_ctrl_ptr->str_ptr[i++] = GUIEDIT_IP_SEPARATOR;

        //get the third ip num
        ip_num     = (uint8) ( (ipedit_ctrl_ptr->ip_addr & 0xFF00) >> 8);
        ip_hundred = (uint8) (ip_num / 100);
        ip_ten     = (uint8) ( (ip_num % 100) / 10);
        ip_one     = (uint8) (ip_num % 10);

        baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (ip_hundred + '0');
        baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (ip_ten + '0');
        baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (ip_one + '0');
        baseedit_ctrl_ptr->str_ptr[i++] = GUIEDIT_IP_SEPARATOR;

        //get the fourth ip num
        ip_num     = (uint8) (ipedit_ctrl_ptr->ip_addr & 0xFF);
        ip_hundred = (uint8) (ip_num / 100);
        ip_ten     = (uint8) ( (ip_num % 100) / 10);
        ip_one     = (uint8) (ip_num % 10);

        baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (ip_hundred + '0');
        baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (ip_ten + '0');
        baseedit_ctrl_ptr->str_ptr[i++] = (wchar) (ip_one + '0');

        baseedit_ctrl_ptr->str_len = i;
        SCI_ASSERT (i == baseedit_ctrl_ptr->str_max_len); /*assert verified*/
    }
}

/*****************************************************************************/
//  Description : get ipedit pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLIPEDIT_OBJ_T* GetIPEditPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        if (!IPEditTypeOf (ctrl_ptr))
        {
            SCI_TRACE_LOW("== GetIPEditPtr error == 0x%x", ctrl_ptr);
            ctrl_ptr = PNULL;
        }
    }

    return (CTRLIPEDIT_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN IPEditTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf ( (OBJECT_TYPE_PTR) ctrl_ptr, CTRL_IPEDIT_TYPE);
}

/*****************************************************************************/
//  Description : handle IP edit up/down key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIPEditUpDownKey (
    CTRLIPEDIT_OBJ_T    *ipedit_ctrl_ptr,
    BOOLEAN             is_add
)
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) ipedit_ctrl_ptr;

    if (baseedit_ctrl_ptr->common_theme.is_cir_handle_ud)
    {
        //get new IP
        if (GetNewIP (ipedit_ctrl_ptr, FALSE, is_add, 0))
        {
            //reset length,get new date string
            baseedit_ctrl_ptr->str_len = 0;

            //update IP display
            VTLBASEEDIT_DisplayAll (baseedit_ctrl_ptr, PNULL);
        }

        result = MMI_RESULT_TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle IP edit up/down/number key,get new IP and IP string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNewIP (//up/down:is change;number:is valid
    CTRLIPEDIT_OBJ_T    *ipedit_ctrl_ptr,
    BOOLEAN             is_num,     //TRUE:number key:FALSE:up/down key
    BOOLEAN             is_add,
    uint8               num_value   //from 0 to 9
)
{
    BOOLEAN             result = FALSE;
    uint8               bit_value = 0;
    uint8               bit_min = 0;
    uint8               bit_max = 0;
    uint8               ip_hundred = 0;
    uint8               ip_ten = 0;
    uint8               ip_one = 0;
    uint8               unit_value = 0;
    uint32              ip_num = 0;
    uint32              ip_addr = 0;
    GUIEDIT_IP_BIT_T    ip_bit = {0};
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) ipedit_ctrl_ptr;

    //get cursor position corresponding bit
    ip_bit = GetCurIPBit (ipedit_ctrl_ptr);

    //kevin.lou modified:delete assert
    //SCI_ASSERT(GUIEDIT_IP_BIT_NULL != ip_bit.bit); /*assert verified*/
    if (GUIEDIT_IP_BIT_NULL == ip_bit.bit)
    {
        return FALSE;
    }

    //set bit value
    bit_value = (uint8) (baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos] - '0');

    switch (ip_bit.bit)
    {
    case GUIEDIT_IP_BIT_HUNDRED:
        //get ip number ten and one bit
        ip_ten = (uint8) (baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos + 1] - '0');
        ip_one = (uint8) (baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos + 2] - '0');

        //set unit and new IP number
        unit_value = 100;
        ip_num = (uint32) (ip_ten * 10 + ip_one);

        //set bit min and max
        bit_min = 0;
        bit_max = 2;
        break;

    case GUIEDIT_IP_BIT_TEN:
        //get ip number hundred and one bit
        ip_hundred = (uint8) (baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos - 1] - '0');
        ip_one     = (uint8) (baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos + 1] - '0');

        //set unit and new IP number
        unit_value = 10;
        ip_num = (uint32) (ip_hundred * 100 + ip_one);

        //set bit min and max
        bit_min = 0;

        if (2 == ip_hundred)
        {
            bit_max = 5;
        }
        else
        {
            bit_max = 9;
        }

        break;

    case GUIEDIT_IP_BIT_ONE:
        //get ip number hundred and ten bit
        ip_hundred = (uint8) (baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos - 2] - '0');
        ip_ten     = (uint8) (baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos - 1] - '0');

        //set unit and new IP number
        unit_value = 1;
        ip_num = (uint32) (ip_hundred * 100 + ip_ten * 10);

        //set bit min and max
        bit_min = 0;

        if ( (2 == ip_hundred) &&
                (5 == ip_ten))
        {
            bit_max = 5;
        }
        else
        {
            bit_max = 9;
        }

        break;

    default:
        SCI_PASSERT (FALSE, ("GetNewIP: IP bit %d is error!", ip_bit.bit)); /*assert verified*/
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

    //get new IP number
    ip_num = (uint32) (ip_num + bit_value * unit_value);

    //adjust IP number
    if (ip_num > 255)
    {
        ip_num = 255;
    }

    //get new IP address
    switch (ip_bit.num_pos)
    {
    case 1:
        ip_addr = (ip_num << 24) | (ipedit_ctrl_ptr->ip_addr & 0x00FFFFFF);
        break;

    case 2:
        ip_addr = (ip_num << 16) | (ipedit_ctrl_ptr->ip_addr & 0xFF00FFFF);
        break;

    case 3:
        ip_addr = (ip_num << 8) | (ipedit_ctrl_ptr->ip_addr & 0xFFFF00FF);
        break;

    case 4:
        ip_addr = ip_num  | (ipedit_ctrl_ptr->ip_addr & 0xFFFFFF00);
        break;

    default:
        SCI_PASSERT (FALSE, ("GetNewIP: IP number position %d is error!", ip_bit.num_pos)); /*assert verified*/
        break;
    }

    //set new IP
    if (ip_addr != ipedit_ctrl_ptr->ip_addr)
    {
        ipedit_ctrl_ptr->ip_addr = ip_addr;

        if (!is_num)
        {
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get current IP bit
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL GUIEDIT_IP_BIT_T GetCurIPBit (
    CTRLIPEDIT_OBJ_T    *ipedit_ctrl_ptr
)
{
    GUIEDIT_IP_BIT_T    ip_bit = {0};
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) ipedit_ctrl_ptr;

    ip_bit = s_guiedit_ip_bit[baseedit_ctrl_ptr->cursor.cur_pos];

    return (ip_bit);
}

/*****************************************************************************/
//  Description : handle IP edit left key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIPEditLeftKey (
    CTRLIPEDIT_OBJ_T    *ipedit_ctrl_ptr
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) ipedit_ctrl_ptr;

    if ((0 == baseedit_ctrl_ptr->cursor.cur_pos) &&
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

            //if is separator
            if (GUIEDIT_IP_SEPARATOR == baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos])
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
//  Description : handle IP edit right key
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleIPEditRightKey (
    CTRLIPEDIT_OBJ_T    *ipedit_ctrl_ptr
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) ipedit_ctrl_ptr;

    if ((baseedit_ctrl_ptr->cursor.cur_pos == (baseedit_ctrl_ptr->str_len - 1)) &&
        (!baseedit_ctrl_ptr->common_theme.is_cir_handle_lf))
    {
        result = MMI_RESULT_FALSE;
    }
    else
    {
        //reset highlight not display
        BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, FALSE);

        //set current cursor position
        MoveIPCursorToRight (ipedit_ctrl_ptr);

        //display highlight
        BASEEDIT_DisplayHighlight (baseedit_ctrl_ptr, TRUE);

        //set display cursor
        baseedit_ctrl_ptr->cursor.is_rect_valid = FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : move IP cursor to right
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void MoveIPCursorToRight (
    CTRLIPEDIT_OBJ_T    *ipedit_ctrl_ptr
)
{
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) ipedit_ctrl_ptr;

    if (baseedit_ctrl_ptr->cursor.cur_pos < (baseedit_ctrl_ptr->str_len - 1))
    {
        VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, (uint16) (baseedit_ctrl_ptr->cursor.cur_pos + 1));

        //if is separator
        if (GUIEDIT_IP_SEPARATOR == baseedit_ctrl_ptr->str_ptr[baseedit_ctrl_ptr->cursor.cur_pos])
        {
            VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, (uint16) (baseedit_ctrl_ptr->cursor.cur_pos + 1));
        }
    }
    else
    {
        VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, 0);
    }

    //set highlight
    VTLBASEEDIT_SetHighlight (
        baseedit_ctrl_ptr,
        baseedit_ctrl_ptr->cursor.cur_pos,
        (uint16) (baseedit_ctrl_ptr->cursor.cur_pos + 1),
        SETHIGHLIGHT_BOTH);
}

/*****************************************************************************/
//  Description : IP handle im commit msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleIPImCommit (
    CTRLIPEDIT_OBJ_T    *ipedit_ctrl_ptr,
    GUIEDIT_ADD_STR_T   *add_str_ptr
)
{
    uint8   num_value = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*) ipedit_ctrl_ptr;

    //get num value
    if (BASEFIXED_GetNumValueByIm (&num_value, add_str_ptr))
    {
        //get new IP
        if (GetNewIP (ipedit_ctrl_ptr, TRUE, FALSE, num_value))
        {
            //set current cursor position
            MoveIPCursorToRight (ipedit_ctrl_ptr);

            //reset length,get new date string
            baseedit_ctrl_ptr->str_len = 0;

            //update IP display
            VTLBASEEDIT_DisplayAll (baseedit_ctrl_ptr, PNULL);
        }
    }
}

