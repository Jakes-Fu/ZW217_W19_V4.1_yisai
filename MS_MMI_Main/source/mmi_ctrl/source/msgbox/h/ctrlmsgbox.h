/*****************************************************************************
** File Name:      ctrlmsgbox.h                                              *
** Author:                                                                   *
** Date:           16/04/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe message box control         *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2004       panda.luo          Create                                   *
******************************************************************************/

#ifndef _CTRLMSGBOX_H_
#define _CTRLMSGBOX_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guicommon.h"
#include "mmk_type.h"
#include "guianim.h"
#include "guisoftkey.h"
#include "guimsgbox.h"
#include "guictrl.h"

#include "ctrlbase.h"
#include "ctrlmsgbox_export.h"

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
// msgbox type info
#define CTRL_MSGBOX_TYPE     (MSGBOX_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

// msgbox init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;

    GUIMSGBOX_STYLE_T   style;/*!<类型 */
    uint8               lcd_id;/*!<lcd的id */
    GUI_RECT_T			*rect_ptr;/*!<显示矩形*/
} CTRLMSGBOX_INIT_PARAM_T;

/*
GUIMSGBOX_TEXT_INFO_T定义了消息框里的文本内容的属性。
消息框没有滚动条，因此，如果内容超过显示区域，则自动滚动显示。
*/
typedef struct
{
    MMI_STRING_T			string;         //需要显示的字符串
    GUI_RECT_T				text_rect;      //显示字符串的区域
    GUI_COLOR_T				back_color;     //显示文本区域的背景色
    uint8					move_line;      //每次滚动行数
    uint8					space_line;     //行间距
    uint8					space_char;     //字间距    
    GUI_FONT_T				font;			//字符串字体
    GUI_COLOR_T				font_color;		//字体颜色
    uint8					char_perline;   //每行可以显示的字符数
    uint8					max_lines;      //控件可以显示的最大文本行
    uint8					total_lines;    //需要显示的文本的行数
    uint16					first_line;     //显示在lcd上的第一个字符的位置
    BOOLEAN					is_timer_need;  //是否需要启动定时器
    GUI_TIMER_ID_T			text_timer;     //文本滚动定时器
    GUI_TIMER_DURATION_T    text_timeout;   //定时向下滚动文本
}GUIMSGBOX_TEXT_INFO_T;


//GUI_MESSAGEBOX_T定义了消息框控件的数据结构
typedef struct _msgbox_obj_tag
{
    CTRLBASE_OBJ_T              base_ctrl;

    GUIMSGBOX_STYLE_T       style;              //消息框的风格
    BOOLEAN                 is_active;          //控件是否处于Active
    BOOLEAN                 is_focus;           //控件所在的窗口是否处于Focus
    GUI_COLOR_T             back_color;         //控件背景颜色
    MMI_IMAGE_ID_T  		back_image_id;		//控件背景图片
    GUIMSGBOX_TEXT_INFO_T   text_info;          //显示的提示信息
    MMI_TEXT_ID_T           title_text_id;      //title text id
    uint8                   title_height;       //title height
    GUI_COLOR_T             title_color;        //标题背景颜色    
    GUIMSGBOX_IMAGE_E       image_type;         //图片类型
    GUI_RECT_T              image_rect;         //图片显示区域
    void                    *animate_ptr;        //动画指针
    MMI_IMAGE_ID_T          image_id;           //图片id
    MMI_ANIM_ID_T           anim_id;            //动画id    
    GUI_TIMER_DURATION_T    ani_timeout;        //动画播放频率
    uint8                   img_vspace;         //图到边框上边和下边的距离
    uint8                   img_hspace;          //图到边框左边和右边的距离
    MMI_TEXT_ID_T           softkey_id[GUISOFTKEY_BUTTON_NUM];     
    GUI_RECT_T              sk_rect;
    GUI_COLOR_T             sk_bg_color;	
	MMI_WIN_ID_T			win_id;
} CTRLMSGBOX_OBJ_T;    //消息框控件的数据结构

// msgbox class for public or protected method
typedef struct _msgbox_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

} CTRLMSGBOX_VTBL_T;


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get msgbox type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T MSGBOX_GetType(void);



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif




