/*****************************************************************************
** File Name:      ctrlscrollkey.h                                               *
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

#ifndef _CTRLSCROLLKEY_H_
#define _CTRLSCROLLKEY_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrlscrollkey_export.h"

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
// scrollkey type info
#define CTRL_SCROLLKEY_TYPE     (SCROLLKEY_GetType())

#ifdef GUIF_SCROLLKEY

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// scrollkey的每一项，目前只显示下列中的一种，不支持两种同时显示
typedef enum
{
    GUISCROLLKEY_DATA_TEXT_ID,     // 显示文本
	GUISCROLLKEY_DATA_TEXT_BUFFER, // 显示文本
    GUISCROLLKEY_DATA_IMAGE_ID,    // 显示icon
	GUISCROLLKEY_DATA_MAX,
}GUISCROLLKEY_DATA_TYPE_E;

// 动画数据信息
typedef struct
{
    GUIANIM_DATA_INFO_T    anim_data; // text type: text id or buffer
    int16				   comp_image_w; // 图片显示的压缩宽度
	int16				   comp_image_h; // 图片显示的压缩高度
} GUISCROLLKEY_ANIM_DATA_INFO_T;

// 动画文件数据信息
typedef struct
{
    GUIANIM_FILE_INFO_T    anim_path;
    int16				   comp_image_w; // 图片显示的压缩宽度
	int16				   comp_image_h; // 图片显示的压缩高度
} GUISCROLLKEY_ANIM_FILE_INFO_T;

// 只显示其中一种数据
typedef union
{
    MMI_TEXT_ID_T               text_id;
    MMI_STRING_T                text_buffer;
    MMI_IMAGE_ID_T              image_id;
    GUISCROLLKEY_ANIM_DATA_INFO_T      anim_data; // anim data
    GUISCROLLKEY_ANIM_FILE_INFO_T      anim_path; // anim path
}GUISCROLLKEY_CONTENT_DATA_U;

// 控件的内容数据信息
typedef struct
{
    GUISCROLLKEY_DATA_TYPE_E data_type;
    GUISCROLLKEY_CONTENT_DATA_U data_u_main;
    GUISCROLLKEY_CONTENT_DATA_U data_u_secondary;	
} GUISCROLLKEY_CONTENT_T;

// scrollkey 的 button数据
typedef struct
{
    GUISCROLLKEY_CONTENT_T      content;    		   	/*!<character */
    MMI_IMAGE_ID_T              icon_down_id;
    MMI_IMAGE_ID_T              icon_up_id;
	MMI_IMAGE_ID_T              icon_confirm_id;
	MMI_IMAGE_ID_T              confirm_point_id;
    BOOLEAN                     is_pressed;
    BOOLEAN                     is_gray;             // TRUE - show;
	BOOLEAN	                    is_long_press;		 //是否支持long press
	BOOLEAN                     is_confirmed;
	BOOLEAN                     is_animated;
	GUI_RECT_T                  rect;
	uint16                      previous_x;
    void                        *param;
}GUI_SCROLLKEY_ITEM_T;

// scrollkey init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;

	MMI_IMAGE_ID_T		        left_scroll_key_id;
	MMI_IMAGE_ID_T		        right_scroll_key_id;
	MMI_TEXT_ID_T		        left_scroll_text_id;
	MMI_TEXT_ID_T		        right_scroll_text_id;
	GUISCROLLSKIN_INDEX	        left_scroll_skin_idx;
	GUISCROLLSKIN_INDEX	        right_scroll_skin_idx;
} CTRLSCROLLKEY_INIT_PARAM_T;

//scrollkey control
typedef struct
{
    CTRLBASE_OBJ_T              base_ctrl;

	BOOLEAN						tp_disable;			/*是否不响应tp消息*/
	BOOLEAN						is_visible;			/*是否可见，如果不可见，则不响应消息*/
	BOOLEAN                     is_scrollkey_pressed; /*是否scrollkey被按下*/

    MMI_IMAGE_ID_T             	bg_image;           /*!<background image */
    GUI_COLOR_T                	bg_color;           /*!<background color */
    GUI_COLOR_T                	font_color;         /*!<font color */
    GUI_FONT_T      		   	font;               /*!<font */
    uint16            		   	char_space;         /*!<character space */
	uint32             		   	bg_type;    	    /*!<background stype */
    uint8	        		   	frameType;	        /*!<border style:rectangle,3Drectangle,image,no border */
    int8                        cur_obj_index;      // 如果使用了动画控件，记录当前解析的索引

    GUI_SCROLLKEY_ITEM_T        button_arr[GUISCROLLKEY_BUTTON_NUM];    /*!<button array */

    GUI_LCD_DEV_INFO            scrollkey_layer;

	MMI_HANDLE_T				win_handle;

    // self member ...
} CTRLSCROLLKEY_OBJ_T;

// scrollkey class for public or protected method
typedef struct
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

    // self vtbl ...
} CTRLSCROLLKEY_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get scrollkey type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T SCROLLKEY_GetType(void);

#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

