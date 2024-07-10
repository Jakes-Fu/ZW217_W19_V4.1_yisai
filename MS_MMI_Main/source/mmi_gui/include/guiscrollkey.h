/*****************************************************************************
** File Name:      guiscrollkey.h                                            *
** Author:                                                                   *
** Date:           05/08/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/08/2004     jibin       		Create
******************************************************************************/

/*!
 *  \addtogroup gui_scrollkey_group_label
 *  @{
 */
 
/*! 
 *  \file 	guiscrollkey.h
 *  \author	jibin
 *  \date	May,2004
 *  \brief 	This file is used to describe main functions of scrollkey.
*/

#ifndef _GUI_SCROLLKEY_H_
#define _GUI_SCROLLKEY_H_ 

/*--------------------------------------------------------------------------*/
/*                         Include Files                                    */
/*--------------------------------------------------------------------------*/

#ifdef GUIF_SCROLLKEY

#include "ctrlscrollkey_export.h"

/*--------------------------------------------------------------------------*/
/*                         Compiler Flag                                    */
/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/*--------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                 */
/*--------------------------------------------------------------------------*/
#define GUISCROLLKEY_SetTextId              CTRLSCROLLKEY_SetTextId       
#define GUISCROLLKEY_SetButtonState         CTRLSCROLLKEY_SetButtonState  
#define GUISCROLLKEY_GetTextId              CTRLSCROLLKEY_GetTextId       
#define GUISCROLLKEY_SetStyle               CTRLSCROLLKEY_SetStyle        
#define GUISCROLLKEY_SetBgType              CTRLSCROLLKEY_SetBgType       
#define GUISCROLLKEY_SetConfirmAnim         CTRLSCROLLKEY_SetConfirmAnim  
#define GUISCROLLKEY_Update                 CTRLSCROLLKEY_Update          
#define GUISCROLLKEY_IsTPEnable             CTRLSCROLLKEY_IsTPEnable      
#define GUISCROLLKEY_SetTPEnable            CTRLSCROLLKEY_SetTPEnable     
#define GUISCROLLKEY_SetVisible             CTRLSCROLLKEY_SetVisible      
#define GUISCROLLKEY_SetButtonIconId        CTRLSCROLLKEY_SetButtonIconId 
#define GUISCROLLKEY_SetButtonTextId        CTRLSCROLLKEY_SetButtonTextId 
#define GUISCROLLKEY_Setfont                CTRLSCROLLKEY_Setfont         
#define GUISCROLLKEY_SetButtonTextPtr       CTRLSCROLLKEY_SetButtonTextPtr

/*****************************************************************************/
//  Description : 动态创建状态栏控件
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUISCROLLKEY_CreateCtrlDyn(
                                        MMI_HANDLE_T win_handle,
                                        MMI_CTRL_ID_T ctrl_id, 
                                        MMI_TEXT_ID_T left_text_id,
                                        MMI_TEXT_ID_T right_text_id
                                        );



/*--------------------------------------------------------------------------*/
/*                         TYPE AND STRUCT                                  */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                              */
/*--------------------------------------------------------------------------*/
///*****************************************************************************/
//// 	Description : 设置scrollkey的文本ID,并告知控件是否立即刷新
////	Global resource dependence : 
////  Author:Great.Tian
////	Note:
///*****************************************************************************/
//PUBLIC BOOLEAN GUISCROLLKEY_SetTextId(
//                                    MMI_HANDLE_T    win_handle,
//                                    MMI_CTRL_ID_T   ctrl_id,
//                                    MMI_TEXT_ID_T   leftsoft_id,   // the left scrollkey id
//                                    MMI_TEXT_ID_T   rightsoft_id,  // the right scrollkey id
//                                    BOOLEAN         is_need_update       // whether update
//                                    );

///*****************************************************************************/
////  Description : set  the valid state of the scrollkey bar button
////  Global resource dependence : 
////  Author:Robert Lu
////  Note:
///*****************************************************************************/
//PUBLIC BOOLEAN  GUISCROLLKEY_SetButtonState(
//                                          MMI_HANDLE_T	 win_handle,
//                                          MMI_CTRL_ID_T   ctrl_id,
//                                          GUISCROLLKEY_INDEX   button_index,
//                                          BOOLEAN         is_gray,
//                                          BOOLEAN		 is_long_press
//                                          );

///*****************************************************************************/
//// 	Description : set and display the sofkey if the window is focus
////	Global resource dependence : 
////  Author:Great.Tian
////	Note:
///*****************************************************************************/
//PUBLIC BOOLEAN GUISCROLLKEY_GetTextId(
//                                    MMI_HANDLE_T    win_handle,     //in:
//                                    MMI_CTRL_ID_T   ctrl_id,        //in:
//                                    MMI_TEXT_ID_T   *left_id_ptr,   //in:may PNULL
//                                    MMI_TEXT_ID_T   *right_id_ptr   //in:may PNULL
//                                    );

///*****************************************************************************/
////  Description : 画scrollkey
////  Global resource dependence : 
////  Author: Jibin
////  Note:
///*****************************************************************************/
//PUBLIC void GUISCROLLKEY_SetStyle(
//								MMI_HANDLE_T    win_handle,
//								MMI_CTRL_ID_T   ctrl_id,
//								GUI_COLOR_T		font_color,
//								uint32			bg_type,
//								GUI_COLOR_T		bg_color,
//								MMI_IMAGE_ID_T	bg_img
//								);

///*****************************************************************************/
//// Description : 设置scrollkey的背景类型
//// Global resource dependence : 
//// Author: Jibin
//// Note:
///*****************************************************************************/
//PUBLIC void GUISCROLLKEY_SetBgType(
//                                 MMI_HANDLE_T    win_handle,
//                                 MMI_CTRL_ID_T   ctrl_id,
//                                 GUI_SCROLLKEY_BG_TYPE_T  bg_type
//                                 );

//PUBLIC void GUISCROLLKEY_SetConfirmAnim(
//                                 MMI_HANDLE_T       win_handle,
//                                 MMI_CTRL_ID_T      ctrl_id,
//                                 GUISCROLLKEY_INDEX index,
//                                 BOOLEAN            is_enable                                 
//								 );

///*****************************************************************************/
////  Description : 画scrollkey
////  Global resource dependence : 
////  Author: Jibin
////  Note:
///*****************************************************************************/
//PUBLIC void GUISCROLLKEY_Update(
//                              MMI_HANDLE_T    win_handle,
//                              MMI_CTRL_ID_T   ctrl_id
//                              );

///*****************************************************************************/
////  Description : whether scrollkey response to tp msg
////  Global resource dependence : 
////  Author: Jibin
////  Note:
///*****************************************************************************/
//PUBLIC BOOLEAN GUISCROLLKEY_IsTPEnable(
//                                     MMI_HANDLE_T    win_handle,
//                                     MMI_CTRL_ID_T   ctrl_id
//                                     );

///*****************************************************************************/
////  Description : 
////  Global resource dependence : 
////  Author: Jibin
////  Note:
///*****************************************************************************/
//PUBLIC BOOLEAN GUISCROLLKEY_SetTPEnable(
//                                      MMI_HANDLE_T    win_handle,
//                                      MMI_CTRL_ID_T   ctrl_id,
//                                      BOOLEAN         tp_enable
//                                      );

///*****************************************************************************/
////  Description : 设置是否可见
////  Global resource dependence : 
////  Author: xiaoqing.lu
////  Note:
///*****************************************************************************/
//PUBLIC BOOLEAN GUISCROLLKEY_SetVisible(
//                                     MMI_HANDLE_T    win_handle,
//                                     MMI_CTRL_ID_T   ctrl_id,
//                                     BOOLEAN         is_visible
//                                     );

///*****************************************************************************/
//// 	Description : 设置scrollkey button上的Icon图片ID,并告知控件是否立即刷新
////	Global resource dependence : 
////  Author: xiaoqing.lu
////	Note:
///*****************************************************************************/
//PUBLIC BOOLEAN GUISCROLLKEY_SetButtonIconId(
//                                          MMI_HANDLE_T        win_handle,
//                                          MMI_CTRL_ID_T       ctrl_id,
//                                          MMI_IMAGE_ID_T      icon_id,
//                                          MMI_TEXT_ID_T       text_id,
//                                          GUISCROLLSKIN_INDEX skin_idx,
//                                          GUISCROLLKEY_INDEX  button_num,
//                                          BOOLEAN             is_need_update       // whether update
//                                          );

///*****************************************************************************/
//// 	Description : 设置scrollkey button的文本ID,并告知控件是否立即刷新
////	Global resource dependence : 
////  Author:Great.Tian
////	Note:
///*****************************************************************************/
//PUBLIC BOOLEAN GUISCROLLKEY_SetButtonTextId(
//                                          MMI_HANDLE_T    win_handle,
//                                          MMI_CTRL_ID_T   ctrl_id,
//                                          MMI_TEXT_ID_T   text_id,   // the left scrollkey id
//                                          GUISCROLLKEY_INDEX button_num,
//                                          BOOLEAN         is_need_update       // whether update
//                                          );

///*****************************************************************************/
//// Description : 设置字体和颜色
//// Global resource dependence : 
//// Author: xiaoqing.lu
//// Note:
///*****************************************************************************/
//PUBLIC void GUISCROLLKEY_Setfont(
//                                MMI_HANDLE_T    win_handle,
//                                MMI_CTRL_ID_T   ctrl_id,
//                                GUI_COLOR_T		*color_ptr,
//                                GUI_FONT_T      *font_ptr
//                                );

///*****************************************************************************/
////  Description : 动态创建状态栏控件
////  Global resource dependence : 
////  Author:xiaoqing.lu
////  Note:
///*****************************************************************************/
//PUBLIC BOOLEAN GUISCROLLKEY_CreateCtrlDyn(
//                                        MMI_HANDLE_T win_handle,
//                                        MMI_CTRL_ID_T ctrl_id, 
//                                        MMI_TEXT_ID_T left_text_id,
//                                        MMI_TEXT_ID_T right_text_id
//                                        );

///*****************************************************************************/
//// 	Description : 设置scrollkey button的文本ID,并告知控件是否立即刷新
////	Global resource dependence : 
////  Author:Great.Tian
////	Note:
///*****************************************************************************/
//PUBLIC BOOLEAN GUISCROLLKEY_SetButtonTextPtr(
//                                           MMI_HANDLE_T    win_handle,
//                                           MMI_CTRL_ID_T   ctrl_id,
//                                           MMI_STRING_T    str_info,   // the left scrollkey id
//                                           GUISCROLLKEY_INDEX button_num,
//                                           BOOLEAN         is_need_update       // whether update
//                                           );

#endif // GUIF_SCROLLKEY
/*--------------------------------------------------------------------------*/
/*                         Compiler Flag                                    */
/*--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*! @} */


