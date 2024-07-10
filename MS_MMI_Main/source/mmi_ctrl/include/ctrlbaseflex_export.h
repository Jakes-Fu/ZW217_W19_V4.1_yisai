/*****************************************************************************
** File Name:      ctrlbaseflex_export.h                                        *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Create
******************************************************************************/

#ifndef _CTRLBASEFLEX_EXPORT_H_
#define _CTRLBASEFLEX_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbaseedit_export.h"
#include "mmitheme_edit.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// TODO:
// 这个结构应该放到ctrllistedit_export.h里，现在由于baseflex用到，所以暂时放这里
/*! @enum GUIEDIT_STYLE_E
@brief 短信编码方式
*/
typedef enum
{
    GUIEDIT_SMS_ENCODE_AUTO,	/*!<默认*/
    GUIEDIT_SMS_ENCODE_7BIT,	/*!<7bit编码*/
    GUIEDIT_SMS_ENCODE_8BIT,    /*!<8bit编码*/
	GUIEDIT_SMS_ENCODE_UCS2,    /*!<ucs2编码*/
    GUIEDIT_SMS_ENCODE_MAX		/*!<保留位*/
} GUIEDIT_SMS_ENCODE_E;

typedef enum
{
    EDIT_BUTTON_TEXT_ID,     /*!< 显示文本 */ 
	EDIT_BUTTON_IMAGE_ID,    /*!< 显示icon */  
		
	EDIT_BUTTON_MAX,
}GUIEDIT_BUTTON_TYPE_T;

/*! @typedef GUIEDIT_WIN_INTERCEPT_CTRL_MSG
@brief 窗口是否截获控件消息,目前只能截获Cancel消息
*/
typedef BOOLEAN (*GUIEDIT_WIN_INTERCEPT_CTRL_MSG)(//是否需要ctrl处理
                                                  MMI_WIN_ID_T      win_id,
                                                  MMI_MESSAGE_ID_E  msg_id,
                                                  wchar*            str_ptr,
                                                  uint16            str_len
                                                  );

// TODO:
// 这个结构应该放到ctrllistedit_export.h里，现在由于baseflex用到，所以暂时放这里
/*! @struct guiedit_list_item_tag
@brief LIST的文本数据
*/
typedef struct guiedit_list_item_tag
{
    BOOLEAN             is_valid;           /*!<item是否有效*/
    BOOLEAN             is_object;          /*!<item是否是文本对象*/
    wchar               *display_str_ptr;   /*!<实际显示的文本*/
    uint16              display_str_len;    /*!<实际显示文本的长度*/
    wchar               *user_str_ptr;      /*!<文本对象对应的内容。is_object为FALSE，代表一个普通的文本，此值被忽略；如果is_object为TRUE，但此值为空，也将作为普通文本处理*/
    uint16              user_str_len;       /*!<文本对象对应内容的长度*/
} GUIEDIT_LIST_ITEM_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : set editbox whether to respond to tp msg
//  Global resource dependence :
//  Author: yuming.yang
//  Note: default handle tp msg
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_SetHandleTpMsg (
    MMI_CTRL_ID_T    ctrl_id,
    BOOLEAN          is_handle
);

#endif

/*****************************************************************************/
//  Description : get edit current cursor position
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLBASEFLEX_GetCursorCurPos (
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description : set edit current cursor position
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_SetCursorCurPos (
    MMI_CTRL_ID_T    ctrl_id,    //in:
    uint16           cur_pos     //in:from 0
);

/*****************************************************************************/
//  Description : set the hide state
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_SetCursorHideState (
    MMI_CTRL_ID_T    ctrl_id,
    BOOLEAN          is_hide
);

/*****************************************************************************/
//  Description : get edit string,include text,digital,phone number etc.
//  Global resource dependence :
//  Author: Jassmine
//  Note:  对于GUIEDIT_TYPE_PHONENUM类型的editbox，如果输入的电话号码首位为“+”，
//     则该位不计算在已输入字符个数之内。那么在editbox内部会多开辟一个宽字节的内存来存储“+”。
//     此时用户输入的字符数长度可能会超过给定最大值；以应用给定的字符串最大值截取editbox前面的部分。
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_GetString (
    MMI_CTRL_ID_T     ctrl_id,    //in:
    MMI_STRING_T      *str_ptr    //in:/out
);

/*****************************************************************************/
//  Description : get edit string,include text,digital,phone number etc.
//  Global resource dependence :
//  Author: Hongjun Jia
//  Note:  该函数获取editbox中字符串的所有信息；仅供GUIEDIT_TYPE_PHONE
//         类型的editbox使用。一般用户请使用CTRLBASEFLEX_GetString()
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_GetStringEx (
									  MMI_CTRL_ID_T     ctrl_id,    //in:
									  MMI_STRING_T      *str_ptr    //in:/out
									);

/*****************************************************************************/
//  Description : set edit string,include text,digital,phone number etc.
//  Global resource dependence :
//  Author: Jassmine
//  Note:超出最大范围则进行截取
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_SetString (
    MMI_CTRL_ID_T      ctrl_id,    //in:
    wchar              *str_ptr,   //in:
    uint16             str_len     //in:
);

/*****************************************************************************/
//  Description : insert text string before current cursor,include text,digital,phone number etc.
//  Global resource dependence :
//  Author: Jassmine
//  Note: 超出最大范围则返回FALSE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_InsertString (
    MMI_CTRL_ID_T      ctrl_id,    //in:
    wchar              *str_ptr,   //in:
    uint16             str_len     //in:
);

/*****************************************************************************/
//  Description : clear edit all string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_ClearAllStr (
    MMI_CTRL_ID_T    ctrl_id
);

/*****************************************************************************/
//  Description : clear edit all string
//  Global resource dependence :
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_ClearAllStrEx (
    MMI_CTRL_ID_T       ctrl_id,
    BOOLEAN             is_update
);

/*****************************************************************************/
//  Description : set edit max length
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_SetMaxLen (
    MMI_CTRL_ID_T     ctrl_id,
    uint16            str_max_len
);

/*****************************************************************************/
//  Description : config edit disable input char
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_ConfigDisableChar (
    MMI_CTRL_ID_T     ctrl_id,    //in:
    wchar             *value_ptr, //in:
    uint16            value_count //in:
);

/*****************************************************************************/
//  Description : config edit add enable input char
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_ConfigAddEnableChar (
    MMI_CTRL_ID_T	ctrl_id,    //in:
    wchar			*value_ptr, //in:
    uint16			value_count //in:
);

/*****************************************************************************/
//  Description : set edit is display dividing line
//  Global resource dependence :
//  Author: Jassmine
//  Note: default don't display
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_SetDividingLine (
    MMI_CTRL_ID_T   ctrl_id,    //in:
    uint16          line_width, //in:0 don't display dividing line
    GUI_COLOR_T     line_color  //in:
);

/*****************************************************************************/
//  Description : set edit is handle red key,default handle,close current window
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_SetHandleRedKey (
    BOOLEAN         is_handle,
    MMI_CTRL_ID_T   ctrl_id
);

/*****************************************************************************/
//  Description : set edit long cancel is delete all chars or delete one by one
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_SetLongCancelDelAll (
    BOOLEAN         is_del_all,
    MMI_CTRL_ID_T   ctrl_id
);

/*****************************************************************************/
//  Description : get cursor rect,include multi line
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_GetPosInfo (
    MMI_CTRL_ID_T     ctrl_id,            //in:
    uint16            pos,                //in:
    GUI_RECT_T        *cursor_rect_ptr    //in/out:
);

/*****************************************************************************/
//  Description : get cursor rect,include multi line
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_GetHighlightInfo (
    MMI_CTRL_ID_T   ctrl_id,
    uint16          *highlight_min_ptr,
    uint16          *highlight_max_ptr
);

/*****************************************************************************/
//  Description : get cursor rect,include multi line
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_GetHighlightData (
    MMI_CTRL_ID_T   ctrl_id,
    GUIEDIT_HIGHLIGHT_T *highlight_data_ptr
);

/*****************************************************************************/
//  Description : 设置无内容时，显示的默认文字
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_SetDefaultString (
    MMI_CTRL_ID_T       ctrl_id,                //in:
    wchar               *default_string_ptr,    //in:
    uint16              default_string_len      //in:
);

/*****************************************************************************/
//  Description : 设置无内容时，显示的默认文字
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_SetDefaultStringById (
    MMI_CTRL_ID_T       ctrl_id,            //in:
    MMI_TEXT_ID_T       default_string_id   //in:
);

/***************************************************************************//*!
@brief 设置是否支持剪贴板
@author hua.fang
@param ctrl_id [in] 控件id
@param is_support  [in] 是否支持剪贴板，默认为支持
@note 必须在平台支持剪贴板的情况下才有效
*******************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_SetClipboardEnabled (
    MMI_CTRL_ID_T    ctrl_id,
    BOOLEAN          is_support
);

/*****************************************************************************/
//  Description : set edit softkey,left,middle,right button
//  Global resource dependence :
//  Author: Jassmine
//  Note:
// 如果id为0，不修改原内容
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_SetSoftkey (
    MMI_CTRL_ID_T                    ctrl_id,    //in:
    uint16                           index,      //in:softkey index
    uint16                           str_len,    //in:0,不变
    MMI_TEXT_ID_T                    text_1,     //in:<str_len display text_1
    MMI_TEXT_ID_T                    text_2,     //in:>=str_len display text_2
    MMITHEME_EDIT_SET_BUTTON_PFUNC   setsoftkey  //in:优先通过函数设置softkey[in:字符串指针in:字符串长度,out:softkey值]
);

/*****************************************************************************/
//  Description : set edit softkey,left,middle,right button
//  Global resource dependence : 
//  Author: hongjun.jia
//  Note: 对GUIEDIT_SetSoftkey扩展，支持editbox 的softkey显示图片
//  如果id为0，不修改原内容
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_SetSoftkeyEx(
    MMI_CTRL_ID_T                    ctrl_id,    //in:
    uint16                           index,      //in:softkey index
    uint16                           str_len,    //in:0,不变
    MMI_TEXT_ID_T                    text_1,     //in:<str_len display text_1
    MMI_TEXT_ID_T                    text_2,     //in:>=str_len display text_2
    MMI_IMAGE_ID_T                   image_id,
    GUIEDIT_BUTTON_TYPE_T            style,
    MMITHEME_EDIT_SET_BUTTON_PFUNC   setsoftkey  //in:优先通过函数设置softkey[in:字符串指针in:字符串长度,out:softkey值]
);

/*****************************************************************************/
//  Description : set edit style,single line,multi line or adaptive
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_SetStyle (
    MMI_CTRL_ID_T      ctrl_id,
    GUIEDIT_STYLE_E    style
);

/*****************************************************************************/
//  Description : set edit is display im icon and number info
//  Global resource dependence :
//  Author: Jassmine
//  Note: Default:text and list edit display,others not display
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_SetDispLeftNum (
    MMI_CTRL_ID_T     ctrl_id,                //in:
    BOOLEAN           is_disp_num_info        //in:
);

/*****************************************************************************/
//  Description : del highlight chars
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_DelHighlightChars(
                                        MMI_CTRL_ID_T     ctrl_id    //in:
                                        );

/*****************************************************************************/
//  Description : display title bar
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_DisplayTitleInfo(
                                        MMI_CTRL_ID_T     ctrl_id    //in:
                                        );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
