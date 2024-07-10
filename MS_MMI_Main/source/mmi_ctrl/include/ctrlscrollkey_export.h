/*****************************************************************************
** File Name:      ctrlscrollkey_export.h                                        *
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

#ifndef _CTRLSCROLLKEY_EXPORT_H_
#define _CTRLSCROLLKEY_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase_export.h"
#include "guicommon.h"
#include "guilcd.h"
#include "guictrl.h"
#include "guianim.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#ifdef GUIF_SCROLLKEY

/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define GUISCROLLKEY_BUTTON_NUM  	2 // scrollkey一共二个button，左右
// button's id
#define LEFTSCROLL_BUTTON     		0 // 左button的索引
#define RIGHTSCROLL_BUTTON    		1 // 右button的索引

typedef uint32  GUI_SCROLLKEY_BG_TYPE_T;
// backgroun stype
#define GUI_SCROLLKEY_BG_NULL         0
#define GUI_SCROLLKEY_BG_IMAGE_ONLY   1
#define GUI_SCROLLKEY_BG_COLOR_ONLY   2

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef uint16  GUISCROLLSKIN_INDEX;
typedef uint16  GUISCROLLKEY_INDEX;

typedef enum
{
    GUISCROLLKEY_SKIN_GREEN_ID,
	GUISCROLLKEY_SKIN_GRAY_ID,
    GUISCROLLKEY_SKIN_YELLOW_ID,
#ifndef MMI_LOW_MEMORY_RESOURCE    
    GUISCROLLKEY_SKIN_RED_ID,    
#endif
	GUISCROLLKEY_SKIN_MAX,
}GUISCROLLKEY_SKIN_E;

typedef struct
{
    GUI_BOTH_RECT_T     both_rect;  /*!<横竖屏区域*/

	MMI_IMAGE_ID_T		left_scroll_key_id;
	MMI_IMAGE_ID_T		right_scroll_key_id;
	MMI_TEXT_ID_T		left_scroll_text_id;
	MMI_TEXT_ID_T		right_scroll_text_id;
	GUISCROLLSKIN_INDEX	left_scroll_skin_idx;
	GUISCROLLSKIN_INDEX	right_scroll_skin_idx;
} GUISCROLLKEY_INIT_DATA_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : 设置scrollkey的文本ID,并告知控件是否立即刷新
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLLKEY_SetTextId(
                                    MMI_HANDLE_T    win_handle,
                                    MMI_CTRL_ID_T   ctrl_id,
                                    MMI_TEXT_ID_T   leftsoft_id,   // the left scrollkey id
                                    MMI_TEXT_ID_T   rightsoft_id,  // the right scrollkey id
                                    BOOLEAN         is_need_update       // whether update
                                    );

/*****************************************************************************/
//  Description : set  the valid state of the scrollkey bar button
//  Global resource dependence : 
//  Author:Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  CTRLSCROLLKEY_SetButtonState(
                                          MMI_HANDLE_T	 win_handle,
                                          MMI_CTRL_ID_T   ctrl_id,
                                          GUISCROLLKEY_INDEX   button_index,
                                          BOOLEAN         is_gray,
                                          BOOLEAN		 is_long_press
                                          );

/*****************************************************************************/
// 	Description : set and display the sofkey if the window is focus
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLLKEY_GetTextId(
                                    MMI_HANDLE_T    win_handle,     //in:
                                    MMI_CTRL_ID_T   ctrl_id,        //in:
                                    MMI_TEXT_ID_T   *left_id_ptr,   //in:may PNULL
                                    MMI_TEXT_ID_T   *right_id_ptr   //in:may PNULL
                                    );

/*****************************************************************************/
//  Description : 画scrollkey
//  Global resource dependence : 
//  Author: Jibin
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSCROLLKEY_SetStyle(
								MMI_HANDLE_T    win_handle,
								MMI_CTRL_ID_T   ctrl_id,
								GUI_COLOR_T		font_color,
								uint32			bg_type,
								GUI_COLOR_T		bg_color,
								MMI_IMAGE_ID_T	bg_img
								);

/*****************************************************************************/
// Description : 设置scrollkey的背景类型
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
PUBLIC void CTRLSCROLLKEY_SetBgType(
                                 MMI_HANDLE_T    win_handle,
                                 MMI_CTRL_ID_T   ctrl_id,
                                 GUI_SCROLLKEY_BG_TYPE_T  bg_type
                                 );

PUBLIC void CTRLSCROLLKEY_SetConfirmAnim(
                                 MMI_HANDLE_T       win_handle,
                                 MMI_CTRL_ID_T      ctrl_id,
                                 GUISCROLLKEY_INDEX index,
                                 BOOLEAN            is_enable                                 
								 );

/*****************************************************************************/
//  Description : 画scrollkey
//  Global resource dependence : 
//  Author: Jibin
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSCROLLKEY_Update(
                              MMI_HANDLE_T    win_handle,
                              MMI_CTRL_ID_T   ctrl_id
                              );

/*****************************************************************************/
//  Description : whether scrollkey response to tp msg
//  Global resource dependence : 
//  Author: Jibin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLLKEY_IsTPEnable(
                                     MMI_HANDLE_T    win_handle,
                                     MMI_CTRL_ID_T   ctrl_id
                                     );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: Jibin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLLKEY_SetTPEnable(
                                      MMI_HANDLE_T    win_handle,
                                      MMI_CTRL_ID_T   ctrl_id,
                                      BOOLEAN         tp_enable
                                      );

/*****************************************************************************/
//  Description : 设置是否可见
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLLKEY_SetVisible(
                                     MMI_HANDLE_T    win_handle,
                                     MMI_CTRL_ID_T   ctrl_id,
                                     BOOLEAN         is_visible
                                     );

/*****************************************************************************/
// 	Description : 设置scrollkey button上的Icon图片ID,并告知控件是否立即刷新
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLLKEY_SetButtonIconId(
                                          MMI_HANDLE_T        win_handle,
                                          MMI_CTRL_ID_T       ctrl_id,
                                          MMI_IMAGE_ID_T      icon_id,
                                          MMI_TEXT_ID_T       text_id,
                                          GUISCROLLSKIN_INDEX skin_idx,
                                          GUISCROLLKEY_INDEX  button_num,
                                          BOOLEAN             is_need_update       // whether update
                                          );

/*****************************************************************************/
// 	Description : 设置scrollkey button的文本ID,并告知控件是否立即刷新
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLLKEY_SetButtonTextId(
                                          MMI_HANDLE_T    win_handle,
                                          MMI_CTRL_ID_T   ctrl_id,
                                          MMI_TEXT_ID_T   text_id,   // the left scrollkey id
                                          GUISCROLLKEY_INDEX button_num,
                                          BOOLEAN         is_need_update       // whether update
                                          );

/*****************************************************************************/
// Description : 设置字体和颜色
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC void CTRLSCROLLKEY_Setfont(
                                MMI_HANDLE_T    win_handle,
                                MMI_CTRL_ID_T   ctrl_id,
                                GUI_COLOR_T		*color_ptr,
                                GUI_FONT_T      *font_ptr
                                );

/*****************************************************************************/
// 	Description : 设置scrollkey button的文本ID,并告知控件是否立即刷新
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSCROLLKEY_SetButtonTextPtr(
                                           MMI_HANDLE_T    win_handle,
                                           MMI_CTRL_ID_T   ctrl_id,
                                           MMI_STRING_T    str_info,   // the left scrollkey id
                                           GUISCROLLKEY_INDEX button_num,
                                           BOOLEAN         is_need_update       // whether update
                                           );

#endif  // GUIF_SCROLLKEY

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
