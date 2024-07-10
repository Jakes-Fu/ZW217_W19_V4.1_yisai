/*****************************************************************************
** File Name:      ctrltextedit.c                                               *
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

#include "ctrltextedit.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define GUIEDIT_TEXT_MAX_LEN (4*1024)

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
//  Description : init textedit class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextEditCtrlInitVtbl (
    CTRLTEXTEDIT_VTBL_T        *textedit_vtbl_ptr
);

/*****************************************************************************/
//  Description : textedit construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextEditCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLTEXTEDIT_INIT_PARAM_T   *textedit_param_ptr
);

/*****************************************************************************/
//  Description : textedit destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextEditCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TextEditCtrlPackInitParam (
    GUIEDIT_INIT_DATA_T         *textedit_init_ptr,   //in
    CTRLTEXTEDIT_INIT_PARAM_T   *textedit_param_ptr   //out
);

/*****************************************************************************/
//  Description : handle textedit msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E TextEditCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : get textedit pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLTEXTEDIT_OBJ_T* GetTextEditPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN TextEditTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

/*****************************************************************************/
// 	Description : get align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E TextEditCtrlGetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr   //in:
);

/*****************************************************************************/
// 	Description : set align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void TextEditCtrlSetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr,  //in:
    GUI_ALIGN_E             align               //in
);

/*****************************************************************************/
// 	Description : get im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void TextEditCtrlGetImInfo (
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
LOCAL void TextEditCtrlSetImInfo (
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
LOCAL void TextEditCtrlSetLandscapeEdit (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    MMI_HANDLE_T        landscape_edit          // [in] the dst landscape edit
);

/*****************************************************************************/
//  Description : 检测字符串是否合法
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextEditCtrlIsStringValid (     // [ret] TRUE if char offset is valid
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    wchar                   *str_ptr,
    uint16                  str_len
);

/*****************************************************************************/
//  Description : 获取剩余字数信息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint16 TextEditCtrlGetLeftNumInfo ( // [ret] the left char num
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,     // [in]
    uint16                  str_max_len,            // [in] the max size of char
    wchar                   *str_ptr,               // [out] left char num string
    uint16                  *str_len_ptr            // [out] length of string
);

/*****************************************************************************/
//  Description : 在添加字符前做检查
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TextEditCtrlBeforeAddString(
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
    GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr         //in
);

/*****************************************************************************/
//  Description : 在添加字符后做检查
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TextEditCtrlAfterAddString(
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
    GUIEDIT_ADD_STR_T           *add_str_ptr,           //in/out:
    GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr         //in
);

/*****************************************************************************/
//  Description : 在删除字符前做检查
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TextEditCtrlBeforeDelString(
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr      //in:
);

/*****************************************************************************/
//  Description : 检查添加的字符
//  Global resource dependence :
//  Author: hua.fang
// TRUE: commit的字符已经拷贝到actual_str_ptr;
// FALSE: 不拷贝字符，需要后续处理
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextEditCtrlCheckCommitStr (
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
    GUIEDIT_ADD_STR_T           *add_str_ptr,           //in/out:
    GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr,        //out
    uint16                      *delete_len_ptr         //out:
);

/*****************************************************************************/
//  Description : 最大长度改变后的通知回调
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TextEditCtrlOnMaxLenChanged (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : init text
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitTextEdit (
    CTRLTEXTEDIT_OBJ_T     *textedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : get page index,for example sms number
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetPageIndex (//from 1
    CTRLTEXTEDIT_OBJ_T     *textedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : get left char number
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 GetLeftCharNum (
    CTRLTEXTEDIT_OBJ_T     *textedit_ctrl_ptr
);

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : get sms left char number
//  Global resource dependence :
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetSmsLeftCharNum (
    CTRLTEXTEDIT_OBJ_T     *textedit_ctrl_ptr
);
#endif

/*****************************************************************************/
//  Description : get remaining characters number string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetRemainNumStr (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    uint16                  left_char_num,
    uint16                  str_max_len,
    wchar                   *str_ptr,
    uint16                  *str_len_ptr
);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : set text max length
//  Global resource dependence :
//  Author: Jassmine
//  Note:当中英文输入字符个数不一致,或者控件创建没有设置时,需调用此函数
/*****************************************************************************/
PUBLIC void CTRLTEXTEDIT_SetTextMaxLen (
    MMI_CTRL_ID_T     ctrl_id,
    uint16            unicode_max_len,
    uint16            ascii_max_len
)
{
    uint16          str_max_len = 0;
    CTRLTEXTEDIT_OBJ_T* textedit_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T* baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    textedit_ctrl_ptr = GetTextEditPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)textedit_ctrl_ptr;

    if ((PNULL != textedit_ctrl_ptr) &&
        (ascii_max_len >= unicode_max_len))
    {
        if (baseedit_ctrl_ptr->str_len > 0)
        {
            CTRLBASEFLEX_ClearAllStrEx(ctrl_id, FALSE);
        }

        //set max len
        textedit_ctrl_ptr->max_len.ascii = ascii_max_len;
        textedit_ctrl_ptr->max_len.ucs2  = unicode_max_len;

        //get max len
        str_max_len = (uint16) MAX (unicode_max_len, ascii_max_len);

        if (str_max_len != baseedit_ctrl_ptr->str_max_len)
        {
            baseedit_ctrl_ptr->str_max_len = str_max_len;
            SCI_ASSERT (0 < baseedit_ctrl_ptr->str_max_len); /*assert verified*/

            //free sting memory and alloc
            if (PNULL != baseedit_ctrl_ptr->str_ptr)
            {
                SCI_FREE (baseedit_ctrl_ptr->str_ptr);
                baseedit_ctrl_ptr->str_len = 0;
            }

            baseedit_ctrl_ptr->str_ptr = BASEEDIT_AllocStrMemory (baseedit_ctrl_ptr->str_max_len);

            //free line info memory and alloc
            if (PNULL != baseedit_ctrl_ptr->line_info_ptr)
            {
                SCI_FREE (baseedit_ctrl_ptr->line_info_ptr);
                baseedit_ctrl_ptr->total_line_num = 1;
                BASEEDIT_GetTopLineOffsetRange(baseedit_ctrl_ptr, &baseedit_ctrl_ptr->top_line_offset, PNULL);
            }

            baseedit_ctrl_ptr->line_info_ptr = BASEEDIT_AllocLineMemory (baseedit_ctrl_ptr->str_max_len);

            VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, 0);
            VTLBASEEDIT_SetHighlight (baseedit_ctrl_ptr, 0, 0, SETHIGHLIGHT_BOTH);
        }
    }
}

/*****************************************************************************/
//  Description : set text max length
//  Global resource dependence :
//  Author: Jassmine
//  Note: only for sms
/*****************************************************************************/
PUBLIC void CTRLTEXTEDIT_SetTextMaxLenEx (
    MMI_CTRL_ID_T       ctrl_id,
    GUIEDIT_TEXT_MAX_T  *max_len_ptr
)
{
    uint16          str_max_len = 0;
    CTRLTEXTEDIT_OBJ_T* textedit_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T* baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    textedit_ctrl_ptr = GetTextEditPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)textedit_ctrl_ptr;

    if (PNULL != textedit_ctrl_ptr)
    {
        if (baseedit_ctrl_ptr->str_len > 0)
        {
            CTRLBASEFLEX_ClearAllStrEx(ctrl_id, FALSE);
        }

        //set max len
        textedit_ctrl_ptr->max_len = *max_len_ptr;

        //get max len
        if ((max_len_ptr->ascii > max_len_ptr->r8_1byte) &&
            (max_len_ptr->r8_1byte > max_len_ptr->r8_2byte) &&
            (max_len_ptr->r8_2byte > max_len_ptr->ucs2))
        {
            str_max_len = max_len_ptr->ascii;
        }
        else
        {
            SCI_PASSERT (FALSE, ("GUIEDIT_SetTextMaxLenEx: max len %d, %d ,%d, %d error!", max_len_ptr->ascii, max_len_ptr->r8_1byte, max_len_ptr->r8_2byte, max_len_ptr->ucs2)); /*assert verified*/
        }

        if (str_max_len != baseedit_ctrl_ptr->str_max_len)
        {
            baseedit_ctrl_ptr->str_max_len = str_max_len;
            SCI_ASSERT (0 < baseedit_ctrl_ptr->str_max_len); /*assert verified*/

            //free sting memory and alloc
            if (PNULL != baseedit_ctrl_ptr->str_ptr)
            {
                SCI_FREE (baseedit_ctrl_ptr->str_ptr);
                baseedit_ctrl_ptr->str_len = 0;
            }

            baseedit_ctrl_ptr->str_ptr = BASEEDIT_AllocStrMemory (baseedit_ctrl_ptr->str_max_len);

            //free line info memory and alloc
            if (PNULL != baseedit_ctrl_ptr->line_info_ptr)
            {
                SCI_FREE (baseedit_ctrl_ptr->line_info_ptr);
                baseedit_ctrl_ptr->total_line_num = 1;
                BASEEDIT_GetTopLineOffsetRange(baseedit_ctrl_ptr, &baseedit_ctrl_ptr->top_line_offset, PNULL);
            }

            baseedit_ctrl_ptr->line_info_ptr = BASEEDIT_AllocLineMemory (baseedit_ctrl_ptr->str_max_len);

            VTLBASEEDIT_SetCursorPos (baseedit_ctrl_ptr, 0);
            VTLBASEEDIT_SetHighlight (baseedit_ctrl_ptr, 0, 0, SETHIGHLIGHT_BOTH);
        }
    }
}

/*****************************************************************************/
//  Description : set text page info
//  Global resource dependence :
//  Author: Jassmine
//  Note:only for sms
/*****************************************************************************/
PUBLIC void CTRLTEXTEDIT_SetTextPage (
    MMI_CTRL_ID_T			ctrl_id,	//in:
    GUIEDIT_TEXT_PAGE_T		*page_ptr,	//in:may PNULL
    GUIEDIT_SMS_ENCODE_E	*encode_ptr	//in:may PNULL
)
{
    CTRLTEXTEDIT_OBJ_T* textedit_ctrl_ptr = PNULL;
    CTRLBASEEDIT_OBJ_T* baseedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    textedit_ctrl_ptr = GetTextEditPtr (ctrl_id);
    baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)textedit_ctrl_ptr;

    if (PNULL != textedit_ctrl_ptr)
    {
        textedit_ctrl_ptr->is_sms = TRUE;

        //set display page and page info
        if (PNULL != page_ptr)
        {
            textedit_ctrl_ptr->page = *page_ptr;
        }

        //set encode type
        if (PNULL != encode_ptr)
        {
            textedit_ctrl_ptr->sms_encode = *encode_ptr;
        }
    }
}

/*****************************************************************************/
//  Description : set edit is support sms encode
//  Global resource dependence :
//  Author: Jassmine
//  Note:only for sms
/*****************************************************************************/
PUBLIC void CTRLTEXTEDIT_SetSmsEncode (
    MMI_CTRL_ID_T			ctrl_id,	//in:
    BOOLEAN                 is_sms      //in
)
{
    CTRLTEXTEDIT_OBJ_T* textedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    textedit_ctrl_ptr = GetTextEditPtr (ctrl_id);

    if (PNULL != textedit_ctrl_ptr)
    {
        textedit_ctrl_ptr->is_sms = is_sms;
    }
}

/*****************************************************************************/
//  Description : set edit is support encode
//  Global resource dependence :
//  Author: Jassmine
//  Note:only for pb
/*****************************************************************************/
PUBLIC void CTRLTEXTEDIT_SetPbEncode (
    BOOLEAN         is_support,
    MMI_CTRL_ID_T   ctrl_id
)
{
    CTRLTEXTEDIT_OBJ_T* textedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    textedit_ctrl_ptr = GetTextEditPtr (ctrl_id);

    if (PNULL != textedit_ctrl_ptr)
    {
        //set pb name is need encode
        textedit_ctrl_ptr->is_pb_encode = is_support;
    }
}
/*****************************************************************************/
//  Description : judge whether editor support pb encode or not
//  Global resource dependence :
//  Author: Jassmine
//  Note:only for pb
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTEXTEDIT_GetPbEncode (
    MMI_CTRL_ID_T   ctrl_id
)
{
    CTRLTEXTEDIT_OBJ_T* textedit_ctrl_ptr = PNULL;
	BOOLEAN				is_support_encode = FALSE;

    //get edit pointer by control id
    textedit_ctrl_ptr = GetTextEditPtr (ctrl_id);

    if (PNULL != textedit_ctrl_ptr)
    {
        //set pb name is need encode
        is_support_encode = textedit_ctrl_ptr->is_pb_encode ? TRUE:FALSE;
    }

	return is_support_encode;
}

/*****************************************************************************/
// 	Description : get the left number in Text EditBox
//	Global resource dependence :
//  Author: jassmine
//	Note:
/*****************************************************************************/
PUBLIC uint16 CTRLTEXTEDIT_GetLeftNum (        //RETURN:
    MMI_CTRL_ID_T   ctrl_id,	    //IN:
    uint16          *sms_num		//out:
)
{
    uint16 left_char_num = 0;
    uint16 page_index = 0;
    CTRLTEXTEDIT_OBJ_T* textedit_ctrl_ptr = PNULL;

    //get edit pointer by control id
    textedit_ctrl_ptr = GetTextEditPtr (ctrl_id);

    if (PNULL != textedit_ctrl_ptr)
    {
        left_char_num = GetLeftCharNum (textedit_ctrl_ptr);

        if (textedit_ctrl_ptr->is_sms)
        {
            //get page index
            page_index = GetPageIndex (textedit_ctrl_ptr);
        }
    }

    if (PNULL != sms_num)
    {
        *sms_num = page_index;
    }

    return left_char_num;
}

/*****************************************************************************/
//  Description : get textedit type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T TEXTEDIT_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASEFLEX_TYPE,                                 // parent class type
                       "textedit",                                         // control name
                       sizeof (CTRLTEXTEDIT_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) TextEditCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) TextEditCtrlDestruct,      // control destructor function
                       sizeof (CTRLTEXTEDIT_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) TextEditCtrlPackInitParam,     // convert init param
                       sizeof (CTRLTEXTEDIT_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) TextEditCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init textedit class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextEditCtrlInitVtbl (
    CTRLTEXTEDIT_VTBL_T        *textedit_vtbl_ptr
)
{
    CTRLBASE_VTBL_T     *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) textedit_vtbl_ptr;
    CTRLBASEEDIT_VTBL_T *baseedit_ctrl_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*) textedit_vtbl_ptr;
    CTRLBASEFLEX_VTBL_T *baseflex_ctrl_vtbl_ptr = (CTRLBASEFLEX_VTBL_T*) textedit_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = TextEditCtrlHandleMsg;

    baseedit_ctrl_vtbl_ptr->GetAlign = TextEditCtrlGetAlign;
    baseedit_ctrl_vtbl_ptr->SetAlign = TextEditCtrlSetAlign;
    baseedit_ctrl_vtbl_ptr->GetImInfo = TextEditCtrlGetImInfo;
    baseedit_ctrl_vtbl_ptr->SetImInfo = TextEditCtrlSetImInfo;
    baseedit_ctrl_vtbl_ptr->SetLandscapeEdit = TextEditCtrlSetLandscapeEdit;

    baseflex_ctrl_vtbl_ptr->IsStringValid = TextEditCtrlIsStringValid;
    baseflex_ctrl_vtbl_ptr->GetLeftNumInfo = TextEditCtrlGetLeftNumInfo;
    baseflex_ctrl_vtbl_ptr->CheckCommitStr = TextEditCtrlCheckCommitStr;
    baseflex_ctrl_vtbl_ptr->BeforeAddString = TextEditCtrlBeforeAddString;
    baseflex_ctrl_vtbl_ptr->AfterAddString = TextEditCtrlAfterAddString;
    baseflex_ctrl_vtbl_ptr->BeforeDelString = TextEditCtrlBeforeDelString;
    baseflex_ctrl_vtbl_ptr->OnMaxLenChanged = TextEditCtrlOnMaxLenChanged;

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
LOCAL void TextEditCtrlPackInitParam (
    GUIEDIT_INIT_DATA_T         *textedit_init_ptr,   //in
    CTRLTEXTEDIT_INIT_PARAM_T   *textedit_param_ptr   //out
)
{
    CTRLBASE_INIT_PARAM_T       *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) textedit_param_ptr;
    CTRLBASEEDIT_INIT_PARAM_T   *baseedit_ctrl_param_ptr = (CTRLBASEEDIT_INIT_PARAM_T*) textedit_param_ptr;

    if (PNULL == textedit_init_ptr || PNULL == textedit_param_ptr)
    {
        return;
    }

    base_ctrl_param_ptr->both_rect = textedit_init_ptr->both_rect;

    baseedit_ctrl_param_ptr->is_rect_adaptive = textedit_init_ptr->is_rect_adaptive;
    baseedit_ctrl_param_ptr->str_max_len = textedit_init_ptr->str_max_len;
    baseedit_ctrl_param_ptr->type = textedit_init_ptr->type;
}

/*****************************************************************************/
//  Description : textedit construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextEditCtrlConstruct (
    CTRLBASE_OBJ_T              *base_ctrl_ptr,
    CTRLTEXTEDIT_INIT_PARAM_T   *textedit_param_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLTEXTEDIT_OBJ_T  *textedit_ctrl_ptr = (CTRLTEXTEDIT_OBJ_T*) base_ctrl_ptr;

    if (PNULL == base_ctrl_ptr || PNULL == textedit_param_ptr)
    {
        return FALSE;
    }

    MMITHEME_GetTextEditTheme (&textedit_ctrl_ptr->theme);

    InitTextEdit (textedit_ctrl_ptr);

    BASEEDIT_Init((CTRLBASEEDIT_OBJ_T*)textedit_ctrl_ptr);

    return (result);
}

/*****************************************************************************/
//  Description : textedit destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextEditCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLTEXTEDIT_OBJ_T  *textedit_ctrl_ptr = (CTRLTEXTEDIT_OBJ_T*) base_ctrl_ptr;

    if (PNULL == textedit_ctrl_ptr)
    {
        return FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle textedit msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E TextEditCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLTEXTEDIT_OBJ_T  *textedit_ctrl_ptr = (CTRLTEXTEDIT_OBJ_T*) base_ctrl_ptr;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = PNULL;

    if (PNULL == textedit_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    parent_vtbl_ptr = (CTRLBASE_VTBL_T*) TYPEMNG_GetParentVtbl (CTRL_TEXTEDIT_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    result = parent_vtbl_ptr->HandleEvent (base_ctrl_ptr, msg_id, param);

    return (result);
}

/*****************************************************************************/
//  Description : get textedit pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLTEXTEDIT_OBJ_T* GetTextEditPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*) MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        if (!TextEditTypeOf (ctrl_ptr))
        {
            SCI_TRACE_LOW("== GetTextEditPtr error == 0x%x", ctrl_ptr);
            ctrl_ptr = PNULL;
        }
    }

    return (CTRLTEXTEDIT_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN TextEditTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf ( (OBJECT_TYPE_PTR) ctrl_ptr, CTRL_TEXTEDIT_TYPE);
}

/*****************************************************************************/
// 	Description : get align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E TextEditCtrlGetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr   //in:
)
{
    CTRLTEXTEDIT_OBJ_T* textedit_ctrl_ptr = (CTRLTEXTEDIT_OBJ_T*) baseedit_ctrl_ptr;

    return textedit_ctrl_ptr->theme.align;
}

/*****************************************************************************/
// 	Description : set align
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void TextEditCtrlSetAlign (
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr,  //in:
    GUI_ALIGN_E             align               //in
)
{
    CTRLTEXTEDIT_OBJ_T* textedit_ctrl_ptr = (CTRLTEXTEDIT_OBJ_T*) baseedit_ctrl_ptr;

    textedit_ctrl_ptr->theme.align = align;
}

/*****************************************************************************/
// 	Description : get im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void TextEditCtrlGetImInfo (
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
    CTRLTEXTEDIT_OBJ_T* textedit_ctrl_ptr = (CTRLTEXTEDIT_OBJ_T*) baseedit_ctrl_ptr;

    *is_forbid_ptr    = textedit_ctrl_ptr->theme.im.is_forbid_symbol;
    *init_input_ptr   = textedit_ctrl_ptr->theme.im.init_input_type;
    *allow_input_ptr  = textedit_ctrl_ptr->theme.im.allow_input_type;
    *init_im_ptr      = textedit_ctrl_ptr->theme.im.init_im;
    *allow_im_ptr     = textedit_ctrl_ptr->theme.im.allow_im;
    *capital_mode_ptr = textedit_ctrl_ptr->theme.im.capital_mode;
    *tag_ptr          = textedit_ctrl_ptr->theme.im.tag;
}

/*****************************************************************************/
// 	Description : set im info
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL void TextEditCtrlSetImInfo (
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
    CTRLTEXTEDIT_OBJ_T* textedit_ctrl_ptr = (CTRLTEXTEDIT_OBJ_T*) baseedit_ctrl_ptr;

    if (PNULL != is_forbid_ptr)
    {
        textedit_ctrl_ptr->theme.im.is_forbid_symbol = *is_forbid_ptr;
    }

    if (PNULL != init_im_ptr)
    {
        textedit_ctrl_ptr->theme.im.init_im = *init_im_ptr;
    }

    if (PNULL != allow_im_ptr)
    {
        textedit_ctrl_ptr->theme.im.allow_im = *allow_im_ptr;
    }

    if (PNULL != init_type_ptr)
    {
        textedit_ctrl_ptr->theme.im.init_input_type = *init_type_ptr;
    }

    if (PNULL != allow_type_ptr)
    {
        textedit_ctrl_ptr->theme.im.allow_input_type = *allow_type_ptr;
    }

    if (PNULL != capital_mode_ptr)
    {
        textedit_ctrl_ptr->theme.im.capital_mode = *capital_mode_ptr;
    }

    if (PNULL != tag_ptr)
    {
        textedit_ctrl_ptr->theme.im.tag = *tag_ptr;
    }
}

/*****************************************************************************/
//  Description : 设置横屏时的新edit
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TextEditCtrlSetLandscapeEdit (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    MMI_HANDLE_T        landscape_edit          // [in] the dst landscape edit
)
{
    CTRLTEXTEDIT_OBJ_T  *new_edit_ctrl_ptr = GetTextEditPtr (landscape_edit);
    CTRLTEXTEDIT_OBJ_T  *textedit_ctrl_ptr = (CTRLTEXTEDIT_OBJ_T*) baseedit_ctrl_ptr;
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr = (CTRLTEXTEDIT_OBJ_T*) baseedit_ctrl_ptr;

    if (PNULL != new_edit_ctrl_ptr)
    {
        //set string max len
        if (textedit_ctrl_ptr->is_sms)
        {
            CTRLTEXTEDIT_SetTextPage (landscape_edit,
                                 &textedit_ctrl_ptr->page,
                                 &textedit_ctrl_ptr->sms_encode);

            CTRLTEXTEDIT_SetTextMaxLenEx (
                landscape_edit,
                &textedit_ctrl_ptr->max_len);
        }
        else
        {
            CTRLTEXTEDIT_SetTextMaxLen (
                landscape_edit,
                textedit_ctrl_ptr->max_len.ucs2,
                textedit_ctrl_ptr->max_len.ascii);
        }

        //set pb is encode
        CTRLTEXTEDIT_SetPbEncode (
            textedit_ctrl_ptr->is_pb_encode,
            landscape_edit);

        //set style
        CTRLBASEFLEX_SetStyle (landscape_edit, baseedit_ctrl_ptr->style);

        //set string
        CTRLBASEFLEX_SetString (landscape_edit, baseedit_ctrl_ptr->str_ptr, baseedit_ctrl_ptr->str_len);

        //set im
        new_edit_ctrl_ptr->theme.im = textedit_ctrl_ptr->theme.im;

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
//  Description : 检测字符串是否合法
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextEditCtrlIsStringValid (     // [ret] TRUE if char offset is valid
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    wchar                   *str_ptr,
    uint16                  str_len
)
{
    return BASEFLEX_CheckTextStr (baseflex_ctrl_ptr, str_ptr, str_len);
}

/*****************************************************************************/
//  Description : 获取剩余字数信息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint16 TextEditCtrlGetLeftNumInfo ( // [ret] the left char num
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,     // [in]
    uint16                  str_max_len,            // [in] the max size of char
    wchar                   *str_ptr,               // [out] left char num string
    uint16                  *str_len_ptr            // [out] length of string
)
{
    uint16  left_char_num = 0;
    CTRLTEXTEDIT_OBJ_T  *textedit_ctrl_ptr = (CTRLTEXTEDIT_OBJ_T*) baseflex_ctrl_ptr;

    left_char_num = GetLeftCharNum (textedit_ctrl_ptr);

    if (PNULL != str_ptr && PNULL != str_len_ptr && 0 != str_max_len)
    {
        GetRemainNumStr (
            textedit_ctrl_ptr,
            left_char_num, str_max_len,
            str_ptr, str_len_ptr);
    }

    return (left_char_num);
}

/*****************************************************************************/
//  Description : 在添加字符前做检查
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TextEditCtrlBeforeAddString(
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
    GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr         //in
)
{
    uint16              highlight_min_pos = 0;
    uint16              highlight_len = 0;
    CTRLTEXTEDIT_OBJ_T  *textedit_ctrl_ptr = (CTRLTEXTEDIT_OBJ_T*) baseflex_ctrl_ptr;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    highlight_len     = BASEEDIT_GetHighlightLen (baseedit_ctrl_ptr);

    //adjust current string ucs2 and default length
    if (0 < highlight_len)
    {
        highlight_min_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);

        //adjust not ascii len and default extension char number for text
        TEXTEDIT_AdjustSpecialCharNum (
                textedit_ctrl_ptr,
                FALSE,
                baseedit_ctrl_ptr->str_ptr + highlight_min_pos,
                highlight_len,
                highlight_min_pos);
    }
}

/*****************************************************************************/
//  Description : 在添加字符后做检查
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TextEditCtrlAfterAddString(
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
    GUIEDIT_ADD_STR_T           *add_str_ptr,           //in/out:
    GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr         //in
)
{
    CTRLTEXTEDIT_OBJ_T  *textedit_ctrl_ptr = (CTRLTEXTEDIT_OBJ_T*) baseflex_ctrl_ptr;

    //add actual string
    if (0 < actual_str_ptr->str_len)
    {
        //adjust not ascii len and default extension char number for text
        TEXTEDIT_AdjustSpecialCharNum (
                textedit_ctrl_ptr,
                TRUE,
                actual_str_ptr->str_ptr,
                actual_str_ptr->str_len,
                0);
    }
}

/*****************************************************************************/
//  Description : 在删除字符前做检查
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TextEditCtrlBeforeDelString(
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr      //in:
)
{
    wchar       *delete_str_ptr = PNULL;
    uint16      delete_str_len = 0;
    uint16      highlight_min_pos = 0;
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)baseflex_ctrl_ptr;

    highlight_min_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);
    delete_str_ptr = baseedit_ctrl_ptr->str_ptr + highlight_min_pos;
    delete_str_len = BASEEDIT_GetHighlightLen (baseedit_ctrl_ptr);

    //adjust not ascii len and default extension char number for text
    TEXTEDIT_AdjustSpecialCharNum (
            (CTRLTEXTEDIT_OBJ_T*)baseflex_ctrl_ptr,
            FALSE,
            delete_str_ptr,
            delete_str_len,
            highlight_min_pos);
}

/*****************************************************************************/
//  Description : 检查添加的字符
//  Global resource dependence :
//  Author: hua.fang
// TRUE: commit的字符已经拷贝到actual_str_ptr;
// FALSE: 不拷贝字符，需要后续处理
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TextEditCtrlCheckCommitStr (
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
    GUIEDIT_ADD_STR_T           *add_str_ptr,           //in/out:
    GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr,        //out
    uint16                      *delete_len_ptr         //out:
)
{
    BOOLEAN             result = FALSE;
    CTRLBASEFLEX_VTBL_T *baseflex_vtbl_ptr = PNULL;
    CTRLTEXTEDIT_OBJ_T  *textedit_ctrl_ptr = (CTRLTEXTEDIT_OBJ_T*) baseflex_ctrl_ptr;

    if (!TEXTEDIT_GetAddLenForDifferent (textedit_ctrl_ptr, add_str_ptr))
    {
        // call parent virtual method
        baseflex_vtbl_ptr = (CTRLBASEFLEX_VTBL_T*) TYPEMNG_GetParentVtbl (
                CTRL_TEXTEDIT_TYPE, (OBJECT_TYPE_PTR)baseflex_ctrl_ptr);

        result = baseflex_vtbl_ptr->CheckCommitStr (
                     baseflex_ctrl_ptr, add_str_ptr, actual_str_ptr, delete_len_ptr);
    }

    // 必须返回FALSE，因为这里不拷贝字符，需要后续处理
    return result;
}

/*****************************************************************************/
//  Description : 最大长度改变后的通知回调
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void TextEditCtrlOnMaxLenChanged (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
)
{
    CTRLTEXTEDIT_OBJ_T* textedit_ctrl_ptr = (CTRLTEXTEDIT_OBJ_T*) baseflex_ctrl_ptr;

    InitTextEdit (textedit_ctrl_ptr);
}

/*****************************************************************************/
//  Description : init text
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void InitTextEdit (
    CTRLTEXTEDIT_OBJ_T     *textedit_ctrl_ptr
)
{
    CTRLBASEEDIT_OBJ_T* baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)textedit_ctrl_ptr;
    CTRLBASEFLEX_OBJ_T* baseflex_ctrl_ptr = (CTRLBASEFLEX_OBJ_T*)textedit_ctrl_ptr;

    if (PNULL != textedit_ctrl_ptr)
    {
        if (baseedit_ctrl_ptr->str_max_len > GUIEDIT_TEXT_MAX_LEN)
        {
            baseedit_ctrl_ptr->str_max_len = GUIEDIT_TEXT_MAX_LEN;
        }

        //set text max length
        textedit_ctrl_ptr->max_len.ascii    = baseedit_ctrl_ptr->str_max_len;
        textedit_ctrl_ptr->max_len.ucs2     = baseedit_ctrl_ptr->str_max_len;
        textedit_ctrl_ptr->max_len.r8_1byte = baseedit_ctrl_ptr->str_max_len;
        textedit_ctrl_ptr->max_len.r8_2byte = baseedit_ctrl_ptr->str_max_len;

        //set text max and min ucs2
        textedit_ctrl_ptr->ucs2.max = 0;
        textedit_ctrl_ptr->ucs2.min = 0xFFFF;

        //set display im icon and number info
        baseedit_ctrl_ptr->is_disp_im_icon  = TRUE;
        baseflex_ctrl_ptr->is_disp_num_info = TRUE;

        baseflex_ctrl_ptr->is_byword = TRUE;
    }
}

/*****************************************************************************/
//  Description : get page index,for example sms number
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetPageIndex (//from 1
    CTRLTEXTEDIT_OBJ_T     *textedit_ctrl_ptr
)
{
    uint16      cur_str_len = 0;
    uint16      page_index = 0;
    uint16      single_len = 0;
    uint16      multi_len = 0;
    CTRLBASEEDIT_OBJ_T* baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)textedit_ctrl_ptr;

    //get current string len
    cur_str_len = baseedit_ctrl_ptr->str_len;

    //set single and multi max len
    if (TEXTEDIT_IsConvertUcs2 (
            textedit_ctrl_ptr->char_num.ucs2,
            &textedit_ctrl_ptr->lan_num))
    {
        //ucs2
        single_len = textedit_ctrl_ptr->page.single_ucs2_len;
        multi_len  = textedit_ctrl_ptr->page.multi_ucs2_len;
    }
    else
    {
        //adjust current string len
        cur_str_len = (uint16) (cur_str_len + textedit_ctrl_ptr->char_num.r8_2byte + textedit_ctrl_ptr->char_num.default_ext);

        if (0 == textedit_ctrl_ptr->char_num.r8_2byte)
        {
            if (0 == textedit_ctrl_ptr->char_num.r8_1byte)
            {
                //ascii
                single_len = textedit_ctrl_ptr->page.single_ascii_len;
                multi_len  = textedit_ctrl_ptr->page.multi_ascii_len;
            }
            else
            {
                //r8 1byte
                single_len = textedit_ctrl_ptr->page.single_r8_1byte_len;
                multi_len  = textedit_ctrl_ptr->page.multi_r8_1byte_len;
            }
        }
        else
        {
            //r8 2byte
            if (textedit_ctrl_ptr->lan_num.por > 0
                || textedit_ctrl_ptr->lan_num.tur > 0
                || textedit_ctrl_ptr->lan_num.tur_esp > 0)
            {
                single_len = textedit_ctrl_ptr->page.single_r8_2byte_len;
                multi_len  = textedit_ctrl_ptr->page.multi_r8_2byte_len;
            }
            else
            {
                single_len = textedit_ctrl_ptr->page.single_r8_1byte_len;
                multi_len  = textedit_ctrl_ptr->page.multi_r8_1byte_len;
            }
        }
    }

    //get page index
    if ((cur_str_len <= single_len) || (0 == multi_len))
    {
        page_index = 1;
    }
    else
    {
        page_index = (uint16) ( (cur_str_len - 1) / multi_len + 1);
    }

    return (page_index);
}

/*****************************************************************************/
//  Description : get left char number
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 GetLeftCharNum (
    CTRLTEXTEDIT_OBJ_T     *textedit_ctrl_ptr
)
{
    uint16  left_char_num = 0;
#ifdef MMI_GUI_STYLE_TYPICAL
    uint16  r8_2byte_num = 0;
#endif
    uint16  default_num = 0;
    CTRLBASEEDIT_OBJ_T* baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)textedit_ctrl_ptr;

    //get the number of remaining characters
    if (baseedit_ctrl_ptr->str_max_len > baseedit_ctrl_ptr->str_len)
    {
        left_char_num = (uint16) (baseedit_ctrl_ptr->str_max_len - baseedit_ctrl_ptr->str_len);
    }
    else
    {
        left_char_num = 0;
    }

    if (textedit_ctrl_ptr->is_sms)    //is sms
    {
#ifdef MMI_GUI_STYLE_TYPICAL
        if ( (GUIEDIT_SMS_ENCODE_UCS2 == textedit_ctrl_ptr->sms_encode) &&
                (0 == baseedit_ctrl_ptr->str_len))
        {
            left_char_num = (uint16) (textedit_ctrl_ptr->max_len.ucs2);
        }
        else if (!TEXTEDIT_IsConvertUcs2 (textedit_ctrl_ptr->char_num.ucs2, &textedit_ctrl_ptr->lan_num))  //no ucs2
        {
            //reduce r8 2byte,default ext
            r8_2byte_num = textedit_ctrl_ptr->char_num.r8_2byte;
            default_num  = textedit_ctrl_ptr->char_num.default_ext;

            if (left_char_num > (r8_2byte_num + default_num))
            {
                left_char_num = (uint16) (left_char_num - r8_2byte_num - default_num);
            }
            else
            {
                left_char_num = 0;
            }
        }
#else
        left_char_num = GetSmsLeftCharNum (textedit_ctrl_ptr);
#endif
    }

    if ( (textedit_ctrl_ptr->is_pb_encode) && //is pb encode
            (MMITHEME_PB_ENCODE_80 != MMITHEME_GetEncodeTypeForPb (textedit_ctrl_ptr->ucs2.max, textedit_ctrl_ptr->ucs2.min)))  //not 80 encode
    {
        //reduce default ext
        default_num  = textedit_ctrl_ptr->char_num.default_ext;

        if (left_char_num > default_num)
        {
            left_char_num = (uint16) (left_char_num - default_num);
        }
        else
        {
            left_char_num = 0;
        }
    }

    return (left_char_num);
}

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : get sms left char number
//  Global resource dependence :
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetSmsLeftCharNum (
    CTRLTEXTEDIT_OBJ_T     *textedit_ctrl_ptr
)
{
    uint16      left_char_num = 0;
    uint16      cur_str_len = 0;
    uint16      single_len = 0;
    uint16      multi_len = 0;
    CTRLBASEEDIT_OBJ_T* baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)textedit_ctrl_ptr;

    //get current string len
    cur_str_len = baseedit_ctrl_ptr->str_len;

    //set single and multi max len
    if (TEXTEDIT_IsConvertUcs2 (textedit_ctrl_ptr->char_num.ucs2, &textedit_ctrl_ptr->lan_num))
    {
        //ucs2
        single_len = textedit_ctrl_ptr->page.single_ucs2_len;
        multi_len  = textedit_ctrl_ptr->page.multi_ucs2_len;
    }
    else
    {
        //adjust current string len
        cur_str_len = (uint16) (cur_str_len + textedit_ctrl_ptr->char_num.r8_2byte + textedit_ctrl_ptr->char_num.default_ext);

        if (0 == textedit_ctrl_ptr->char_num.r8_2byte)
        {
            if (0 == textedit_ctrl_ptr->char_num.r8_1byte)
            {
                //ascii
                single_len = textedit_ctrl_ptr->page.single_ascii_len;
                multi_len  = textedit_ctrl_ptr->page.multi_ascii_len;
            }
            else
            {
                //r8 1byte
                single_len = textedit_ctrl_ptr->page.single_r8_1byte_len;
                multi_len  = textedit_ctrl_ptr->page.multi_r8_1byte_len;
            }
        }
        else
        {
            //r8 2byte
#if 1
			single_len = textedit_ctrl_ptr->page.single_r8_2byte_len;
            multi_len  = textedit_ctrl_ptr->page.multi_r8_2byte_len;
#else
		   if (textedit_ctrl_ptr->lan_num.por > 0
                || textedit_ctrl_ptr->lan_num.tur > 0
                || textedit_ctrl_ptr->lan_num.tur_esp > 0)
            {
                single_len = textedit_ctrl_ptr->page.single_r8_2byte_len;
                multi_len  = textedit_ctrl_ptr->page.multi_r8_2byte_len;
            }
            else
            {
                single_len = textedit_ctrl_ptr->page.single_r8_1byte_len;
                multi_len  = textedit_ctrl_ptr->page.multi_r8_1byte_len;
            }
#endif
        }
    }

    //get page index
    if ( (cur_str_len <= single_len) ||
            (0 == multi_len))
    {
        left_char_num =  single_len - cur_str_len;
    }
    else
    {
        left_char_num = (0 == cur_str_len % multi_len) ? 0 : (multi_len - cur_str_len % multi_len);
    }

    return (left_char_num);
}
#endif

/*****************************************************************************/
//  Description : get remaining characters number string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void GetRemainNumStr (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    uint16                  left_char_num,
    uint16                  str_max_len,
    wchar                   *str_ptr,
    uint16                  *str_len_ptr
)
{
    uint8       remain_num[GUIEDIT_NUM_INFO_MAX_LEN] = {0};
    uint16      page_index = 0;
    CTRLTEXTEDIT_OBJ_T* textedit_ctrl_ptr = (CTRLTEXTEDIT_OBJ_T*)baseflex_ctrl_ptr;

    if (textedit_ctrl_ptr->is_sms)
    {
        //get page index
        page_index = GetPageIndex (textedit_ctrl_ptr);

        *str_len_ptr = (uint16) sprintf ( (char *) remain_num, "%d/%d", left_char_num, page_index);
    }
    else
    {
        *str_len_ptr = (uint16) sprintf ( (char *) remain_num, "%d", left_char_num);
    }

    SCI_ASSERT (*str_len_ptr < GUIEDIT_NUM_INFO_MAX_LEN); /*assert verified*/

    //convert wchar
    MMI_STRNTOWSTR (str_ptr, str_max_len,
                    (const uint8 *) remain_num, GUIEDIT_NUM_INFO_MAX_LEN,
                    *str_len_ptr);
}


