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
// ����ṹӦ�÷ŵ�ctrllistedit_export.h���������baseflex�õ���������ʱ������
/*! @enum GUIEDIT_STYLE_E
@brief ���ű��뷽ʽ
*/
typedef enum
{
    GUIEDIT_SMS_ENCODE_AUTO,	/*!<Ĭ��*/
    GUIEDIT_SMS_ENCODE_7BIT,	/*!<7bit����*/
    GUIEDIT_SMS_ENCODE_8BIT,    /*!<8bit����*/
	GUIEDIT_SMS_ENCODE_UCS2,    /*!<ucs2����*/
    GUIEDIT_SMS_ENCODE_MAX		/*!<����λ*/
} GUIEDIT_SMS_ENCODE_E;

typedef enum
{
    EDIT_BUTTON_TEXT_ID,     /*!< ��ʾ�ı� */ 
	EDIT_BUTTON_IMAGE_ID,    /*!< ��ʾicon */  
		
	EDIT_BUTTON_MAX,
}GUIEDIT_BUTTON_TYPE_T;

/*! @typedef GUIEDIT_WIN_INTERCEPT_CTRL_MSG
@brief �����Ƿ�ػ�ؼ���Ϣ,Ŀǰֻ�ܽػ�Cancel��Ϣ
*/
typedef BOOLEAN (*GUIEDIT_WIN_INTERCEPT_CTRL_MSG)(//�Ƿ���Ҫctrl����
                                                  MMI_WIN_ID_T      win_id,
                                                  MMI_MESSAGE_ID_E  msg_id,
                                                  wchar*            str_ptr,
                                                  uint16            str_len
                                                  );

// TODO:
// ����ṹӦ�÷ŵ�ctrllistedit_export.h���������baseflex�õ���������ʱ������
/*! @struct guiedit_list_item_tag
@brief LIST���ı�����
*/
typedef struct guiedit_list_item_tag
{
    BOOLEAN             is_valid;           /*!<item�Ƿ���Ч*/
    BOOLEAN             is_object;          /*!<item�Ƿ����ı�����*/
    wchar               *display_str_ptr;   /*!<ʵ����ʾ���ı�*/
    uint16              display_str_len;    /*!<ʵ����ʾ�ı��ĳ���*/
    wchar               *user_str_ptr;      /*!<�ı������Ӧ�����ݡ�is_objectΪFALSE������һ����ͨ���ı�����ֵ�����ԣ����is_objectΪTRUE������ֵΪ�գ�Ҳ����Ϊ��ͨ�ı�����*/
    uint16              user_str_len;       /*!<�ı������Ӧ���ݵĳ���*/
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
//  Note:  ����GUIEDIT_TYPE_PHONENUM���͵�editbox���������ĵ绰������λΪ��+����
//     ���λ���������������ַ�����֮�ڡ���ô��editbox�ڲ���࿪��һ�����ֽڵ��ڴ����洢��+����
//     ��ʱ�û�������ַ������ȿ��ܻᳬ���������ֵ����Ӧ�ø������ַ������ֵ��ȡeditboxǰ��Ĳ��֡�
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_GetString (
    MMI_CTRL_ID_T     ctrl_id,    //in:
    MMI_STRING_T      *str_ptr    //in:/out
);

/*****************************************************************************/
//  Description : get edit string,include text,digital,phone number etc.
//  Global resource dependence :
//  Author: Hongjun Jia
//  Note:  �ú�����ȡeditbox���ַ�����������Ϣ������GUIEDIT_TYPE_PHONE
//         ���͵�editboxʹ�á�һ���û���ʹ��CTRLBASEFLEX_GetString()
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_GetStringEx (
									  MMI_CTRL_ID_T     ctrl_id,    //in:
									  MMI_STRING_T      *str_ptr    //in:/out
									);

/*****************************************************************************/
//  Description : set edit string,include text,digital,phone number etc.
//  Global resource dependence :
//  Author: Jassmine
//  Note:�������Χ����н�ȡ
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
//  Note: �������Χ�򷵻�FALSE
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
//  Description : ����������ʱ����ʾ��Ĭ������
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
//  Description : ����������ʱ����ʾ��Ĭ������
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEFLEX_SetDefaultStringById (
    MMI_CTRL_ID_T       ctrl_id,            //in:
    MMI_TEXT_ID_T       default_string_id   //in:
);

/***************************************************************************//*!
@brief �����Ƿ�֧�ּ�����
@author hua.fang
@param ctrl_id [in] �ؼ�id
@param is_support  [in] �Ƿ�֧�ּ����壬Ĭ��Ϊ֧��
@note ������ƽ̨֧�ּ����������²���Ч
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
// ���idΪ0�����޸�ԭ����
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_SetSoftkey (
    MMI_CTRL_ID_T                    ctrl_id,    //in:
    uint16                           index,      //in:softkey index
    uint16                           str_len,    //in:0,����
    MMI_TEXT_ID_T                    text_1,     //in:<str_len display text_1
    MMI_TEXT_ID_T                    text_2,     //in:>=str_len display text_2
    MMITHEME_EDIT_SET_BUTTON_PFUNC   setsoftkey  //in:����ͨ����������softkey[in:�ַ���ָ��in:�ַ�������,out:softkeyֵ]
);

/*****************************************************************************/
//  Description : set edit softkey,left,middle,right button
//  Global resource dependence : 
//  Author: hongjun.jia
//  Note: ��GUIEDIT_SetSoftkey��չ��֧��editbox ��softkey��ʾͼƬ
//  ���idΪ0�����޸�ԭ����
/*****************************************************************************/
PUBLIC void CTRLBASEFLEX_SetSoftkeyEx(
    MMI_CTRL_ID_T                    ctrl_id,    //in:
    uint16                           index,      //in:softkey index
    uint16                           str_len,    //in:0,����
    MMI_TEXT_ID_T                    text_1,     //in:<str_len display text_1
    MMI_TEXT_ID_T                    text_2,     //in:>=str_len display text_2
    MMI_IMAGE_ID_T                   image_id,
    GUIEDIT_BUTTON_TYPE_T            style,
    MMITHEME_EDIT_SET_BUTTON_PFUNC   setsoftkey  //in:����ͨ����������softkey[in:�ַ���ָ��in:�ַ�������,out:softkeyֵ]
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
