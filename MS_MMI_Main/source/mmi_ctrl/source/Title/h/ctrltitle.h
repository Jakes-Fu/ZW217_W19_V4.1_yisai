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
    
    //标题文本
    GUITITLE_TEXT_DATA_E        text_type;
    MMI_TEXT_ID_T               text_id;
    MMI_STRING_T                title_str;          /*!< title string */ 
    GUISTR_STYLE_T              str_style;          /*!< 文本风格 */ 

    uint16                      title_text_margin;  /*!< 边缘 */ 
    uint16                      icon_space;         /*!< icon间隔 */ 

    //theme
    BOOLEAN                     is_need_bg;         /*!< 是否有背景 */ 
    MMI_IMAGE_ID_T              bg_image;           /*!< 背景图片 */ 
    GUI_COLOR_T                 bg_color;           /*!< 背景颜色 */ 

    //小图标
    MMI_IMAGE_ID_T              icon_id;            /*!< icon_id */ 

    //箭头
    BOOLEAN                     is_need_button;      /*!< 是否需要箭头 */ 
    GUITITLE_BUTTON_STATE_E     button_state[GUITITLE_BUTTON_NUM]; /*!< 箭头信息 */ 
    GUI_RECT_T                  button_rect[GUITITLE_BUTTON_NUM];  /*!< 箭头区域 */ 
    GUITITLE_BUTTON_INFO_T      button_info[GUITITLE_BUTTON_NUM];
    MMI_MESSAGE_ID_E            button_msg[GUITITLE_BUTTON_NUM];
    MMI_CTRL_ID_T               handle_msg_ctrl_id[GUITITLE_BUTTON_NUM];

    //子字符串
    BOOLEAN                     is_need_sub_text;               /*!< 是否有子字符串 */ 
    // sub text info
    GUI_RECT_T                  sub_text_rect;                  /*!< 子字符串区域 */ 
    GUI_FONT_T                  sub_font;                       /*!< 子字符串字体 */ 
    wchar                       sub_str[GUITITLE_SUBTEXT_MAXLEN+1]; /*!< 子字符串文本 */ 
    uint16                      sub_str_len;                        /*!< 子字符串长度 */ 
    GUI_ALIGN_E                 sub_align;                          /*!< 子字符串对齐格式 */ 

    //长文本滚动
    GUITITLE_SCROLL_TYPE_E      scroll_type;        /*!< 滚动条模式 */ 
    BOOLEAN                     is_text_overstep;   /*!< 当文本的宽度超过区域的时候，该值为TRUE */ 
    uint8                       scroll_timer_id;    /*!< 滚动条timer_id */ 
    uint32                      scroll_time;        /*!< 循环滚动时间 */ 
    uint32                      scroll_pixel_step;  /*!< 每次滚动的像素数 */ 
    uint16                      scroll_char_offset; /*!< 字符offset */  //record the roll num of chars (roll one char each time  
    uint16                      scroll_pixel_offset;/*!< 像素offset */  //title string pixel offset
    
    MMI_HANDLE_T                win_handle;         /*!< 窗口handle */ 

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
@brief 设置title文本
@author Liqing.peng
@param title_ptr            [in]    title指针
@param wstr_ptr             [in]    文本指针
@param wstr_len             [in]    文本长度
@param is_need_update       [in]    是否需要刷新
@return  设置成功返回TRUE，失败返回FALSE
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetTextByPtr(
                                     CTRLTITLE_OBJ_T    *title_ptr,
                                     const wchar        *wstr_ptr, //[IN] string pointer
                                     uint16	            wstr_len, //[IN] string length, <=16
                                     BOOLEAN            is_need_update //[IN] is flash
                                     );

/***************************************************************************//*!
@brief 设置title右下角索引信息文本
@author Liqing.peng
@param title_ptr            [in]    title指针
@param is_need_sub_text     [in]    是否需要索引文本
@param wstr_ptr             [in]    文本指针
@param wstr_len             [in]    文本长度
@param is_need_update       [in]    是否需要刷新
@return  设置成功返回TRUE，失败返回FALSE
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetSubTextByPtr(    
                                        CTRLTITLE_OBJ_T  *title_ptr,       // 控件指针
                                        BOOLEAN          is_need_sub_text, // 是否需要sub文本
                                        const wchar      *wstr_ptr,		   // [IN] string pointer
                                        uint16           wstr_len,		   // [IN] string length
                                        BOOLEAN          is_need_update    // 是否立即显示
                                        );


/***************************************************************************//*!
@brief 设置title右下角列表索引信息
@author Liqing.peng
@param title_ptr            [in]    控件指针
@param text_rect_ptr        [in]    子字符串的区域
@param font_ptr             [in]    子字符串的字体
@param align_ptr            [in]    对齐方式
@return  设置成功返回TRUE，失败返回FALSE
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLTITLE_SetSubTextParamByPtr(    
                                             CTRLTITLE_OBJ_T  *title_ptr,       // 控件指针
                                             GUI_RECT_T       *text_rect_ptr,   // 子字符串的区域
                                             GUI_FONT_T       *font_ptr,        // 子字符串字体
                                             GUI_ALIGN_E      *align_ptr        // align type
                                             );

/***************************************************************************//*!
@brief 设置title颜色
@author Liqing.peng
@param title_ptr            [in]    控件指针
@param font_color           [in]    字体颜色
@return  设置成功返回TRUE，失败返回FALSE
@note 
*******************************************************************************/
PUBLIC void CTRLTITLE_SetFontColorByPtr(
                                       CTRLTITLE_OBJ_T *title_ptr,
                                       GUI_COLOR_T       font_color
                                       );

/***************************************************************************//*!
@brief 设置title区域
@author Liqing.peng
@param title_ptr            [in]    控件指针
@param rect                 [in]    控件区域
@return  设置成功返回TRUE，失败返回FALSE
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
