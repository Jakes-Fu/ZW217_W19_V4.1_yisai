/*****************************************************************************
** File Name:      ctrltitle.h                                               *
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

#ifndef _CTRLTITLE_H_
#define _CTRLTITLE_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrltitle_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
// title type info
#define CTRL_TITLE_TYPE     (TITLE_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// title init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;
  	MMI_TEXT_ID_T		        text_id;         /*!< text_id */ 
    // self inif param ...
} CTRLTITLE_INIT_PARAM_T;

//title control
typedef struct _title_obj_tag
{
    CTRLBASE_OBJ_T              base_ctrl;
    
    //�����ı�
    GUITITLE_TEXT_DATA_E        text_type;
    MMI_TEXT_ID_T               text_id;
    MMI_STRING_T                title_str;          /*!< title string */ 
    GUISTR_STYLE_T              str_style;          /*!< �ı���� */ 

    uint16                      title_text_margin;  /*!< ��Ե */ 
    uint16                      icon_space;         /*!< icon��� */ 

    //theme
    BOOLEAN                     is_need_bg;         /*!< �Ƿ��б��� */ 
    MMI_IMAGE_ID_T              bg_image;           /*!< ����ͼƬ */ 
    GUI_COLOR_T                 bg_color;           /*!< ������ɫ */ 

    //Сͼ��
    MMI_IMAGE_ID_T              icon_id;            /*!< icon_id */ 

    //��ͷ
    BOOLEAN                     is_need_button;      /*!< �Ƿ���Ҫ��ͷ */ 
    GUITITLE_BUTTON_STATE_E     button_state[GUITITLE_BUTTON_NUM]; /*!< ��ͷ��Ϣ */ 
    GUI_RECT_T                  button_rect[GUITITLE_BUTTON_NUM];  /*!< ��ͷ���� */ 
    GUITITLE_BUTTON_INFO_T      button_info[GUITITLE_BUTTON_NUM];
    MMI_MESSAGE_ID_E            button_msg[GUITITLE_BUTTON_NUM];
    MMI_CTRL_ID_T               handle_msg_ctrl_id[GUITITLE_BUTTON_NUM];

    //���ַ���
    BOOLEAN                     is_need_sub_text;               /*!< �Ƿ������ַ��� */ 
    // sub text info
    GUI_RECT_T                  sub_text_rect;                  /*!< ���ַ������� */ 
    GUI_FONT_T                  sub_font;                       /*!< ���ַ������� */ 
    wchar                       sub_str[GUITITLE_SUBTEXT_MAXLEN+1]; /*!< ���ַ����ı� */ 
    uint16                      sub_str_len;                        /*!< ���ַ������� */ 
    GUI_ALIGN_E                 sub_align;                          /*!< ���ַ��������ʽ */ 

    //���ı�����
    GUITITLE_SCROLL_TYPE_E      scroll_type;        /*!< ������ģʽ */ 
    BOOLEAN                     is_text_overstep;   /*!< ���ı��Ŀ�ȳ��������ʱ�򣬸�ֵΪTRUE */ 
    uint8                       scroll_timer_id;    /*!< ������timer_id */ 
    uint32                      scroll_time;        /*!< ѭ������ʱ�� */ 
    uint32                      scroll_pixel_step;  /*!< ÿ�ι����������� */ 
    uint16                      scroll_char_offset; /*!< �ַ�offset */  //record the roll num of chars (roll one char each time  
    uint16                      scroll_pixel_offset;/*!< ����offset */  //title string pixel offset
    
    MMI_HANDLE_T                win_handle;         /*!< ����handle */ 

    // self member ...
} CTRLTITLE_OBJ_T;

// title class for public or protected method
typedef struct _title_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

    // self vtbl ...
} CTRLTITLE_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get title type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T TITLE_GetType(void);
/***************************************************************************//*!
@brief ����title�ı�
@author Liqing.peng
@param title_ptr            [in]    titleָ��
@param wstr_ptr             [in]    �ı�ָ��
@param wstr_len             [in]    �ı�����
@param is_need_update       [in]    �Ƿ���Ҫˢ��
@return  ���óɹ�����TRUE��ʧ�ܷ���FALSE
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetTextByPtr(
                                     CTRLTITLE_OBJ_T    *title_ptr,
                                     const wchar        *wstr_ptr, //[IN] string pointer
                                     uint16	            wstr_len, //[IN] string length, <=16
                                     BOOLEAN            is_need_update //[IN] is flash
                                     );

/***************************************************************************//*!
@brief ����title���½�������Ϣ�ı�
@author Liqing.peng
@param title_ptr            [in]    titleָ��
@param is_need_sub_text     [in]    �Ƿ���Ҫ�����ı�
@param wstr_ptr             [in]    �ı�ָ��
@param wstr_len             [in]    �ı�����
@param is_need_update       [in]    �Ƿ���Ҫˢ��
@return  ���óɹ�����TRUE��ʧ�ܷ���FALSE
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetSubTextByPtr(    
                                        CTRLTITLE_OBJ_T  *title_ptr,       // �ؼ�ָ��
                                        BOOLEAN          is_need_sub_text, // �Ƿ���Ҫsub�ı�
                                        const wchar      *wstr_ptr,		   // [IN] string pointer
                                        uint16           wstr_len,		   // [IN] string length
                                        BOOLEAN          is_need_update    // �Ƿ�������ʾ
                                        );


/***************************************************************************//*!
@brief ����title���½��б�������Ϣ
@author Liqing.peng
@param title_ptr            [in]    �ؼ�ָ��
@param text_rect_ptr        [in]    ���ַ���������
@param font_ptr             [in]    ���ַ���������
@param align_ptr            [in]    ���뷽ʽ
@return  ���óɹ�����TRUE��ʧ�ܷ���FALSE
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetSubTextParamByPtr(    
                                             CTRLTITLE_OBJ_T  *title_ptr,       // �ؼ�ָ��
                                             GUI_RECT_T       *text_rect_ptr,   // ���ַ���������
                                             GUI_FONT_T       *font_ptr,        // ���ַ�������
                                             GUI_ALIGN_E      *align_ptr        // align type
                                             );

/***************************************************************************//*!
@brief ����title��ɫ
@author Liqing.peng
@param title_ptr            [in]    �ؼ�ָ��
@param font_color           [in]    ������ɫ
@return  ���óɹ�����TRUE��ʧ�ܷ���FALSE
@note 
*******************************************************************************/
PUBLIC void CTRLTITLE_SetFontColorByPtr(
                                       CTRLTITLE_OBJ_T *title_ptr,
                                       GUI_COLOR_T       font_color
                                       );

/***************************************************************************//*!
@brief ����title����
@author Liqing.peng
@param title_ptr            [in]    �ؼ�ָ��
@param rect                 [in]    �ؼ�����
@return  ���óɹ�����TRUE��ʧ�ܷ���FALSE
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetRectByPtr(    
                                     CTRLTITLE_OBJ_T *title_ptr,
                                     GUI_RECT_T	    rect
                                     );

/*****************************************************************************/
// 	Description : Set Title Visible
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetIsVisible(    
                                     CTRLTITLE_OBJ_T *title_ptr,
                                     BOOLEAN is_visible,
                                     BOOLEAN is_need_update
                                     );

/*****************************************************************************/
// Description : get title control ptr.
// Global resource dependence : 
// Author:  xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC CTRLTITLE_OBJ_T *CTRLTITLE_GetTitlePtr(
                                             MMI_HANDLE_T  win_handle,
                                             MMI_CTRL_ID_T ctrl_id
                                             );

/*****************************************************************************/
// 	Description : Set Istyle Icon state
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetButtonState(    
                                       CTRLTITLE_OBJ_T *title_ptr,
                                       GUITITLE_BUTTON_INDEX_T icon_index,
                                       GUITITLE_BUTTON_STATE_E icon_state,
                                       BOOLEAN is_need_update
                                       );

/*****************************************************************************/
// 	Description : Set Istyle Icon Text
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetButtonTextId(    
                                        CTRLTITLE_OBJ_T *title_ptr,
                                        GUITITLE_BUTTON_INDEX_T icon_index,
                                        MMI_TEXT_ID_T text_id,
                                        BOOLEAN is_need_update
                                        );

/*****************************************************************************/
// 	Description : Set Istyle Icon Text By Ptr
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetButtonTextByPtr(    
                                           CTRLTITLE_OBJ_T *title_ptr,
                                           GUITITLE_BUTTON_INDEX_T icon_index,
                                           MMI_STRING_T *str_ptr,
                                           BOOLEAN is_need_update
                                           );

/*****************************************************************************/
// 	Description : Set Istyle Icon Image
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetButtonImage(    
                                       CTRLTITLE_OBJ_T *title_ptr,
                                       GUITITLE_BUTTON_INDEX_T icon_index,
                                       MMI_IMAGE_ID_T image_id,
                                       MMI_IMAGE_ID_T press_image_id,
                                       BOOLEAN is_need_update
                                       );

/*****************************************************************************/
// 	Description : Set Istyle Icon Msg
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetButtonHandleMsgInfo(    
                                               CTRLTITLE_OBJ_T *title_ptr,
                                               GUITITLE_BUTTON_INDEX_T icon_index,
                                               MMI_MESSAGE_ID_E button_msg_id,
                                               MMI_CTRL_ID_T handle_msg_ctrl_id
                                               );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
