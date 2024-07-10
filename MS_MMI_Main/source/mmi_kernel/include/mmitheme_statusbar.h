/******************************************************************************
** File Name:      guistatusbar.h                                             *
** Author:         xiaoqing.lu                                                *
** Date:           12/23/2008                                                 *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.          *
** Description:    This file is used to describe the data struct of           *
**                              guistatusbar control                          *
*******************************************************************************
**                         Important Edit History                             *
** ---------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 12/23/2008     Xiaoqing.lu      Create			                          *
******************************************************************************/

/*!
 *  \addtogroup gui_statusbar_group_label
 *  @{
 */
 
/*! 
 *  \file 	guistatusbar.h 
 *  \author	jibin
 *  \date	May,2004
 *  \brief 	This file is used to describe the data struct of guistatusbar control
 */

#ifndef _MMITHEME_STATUSBAR_H_
#define _MMITHEME_STATUSBAR_H_ 

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "mmk_type.h"
#include "guicommon.h"
#include "guiblock.h"
#include "guifont.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern   "C"
    {
#endif
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*! \brief timer out */
#ifdef MAINLCD_SIZE_128X64
#define MMISTATUSBAR_ICONANIM_TIMEOUT                400 // 动画启动时的时间间隔
#else
#define MMISTATUSBAR_ICONANIM_TIMEOUT                200 // 动画启动时的时间间隔
#endif
#define MMISTATUSBAR_SCROLLMSG_TIMEOUT               200 // 滚动的时间间隔
#define MMISTATUSBAR_SCROLLMSG_PAUSE_TIMEOUT         1000// 滚动最后一帧停留的时间间隔

#define MMISTATUSBAR_ITEM_WIDTH                      DP2PX_VALUE(20)  // item的最大宽度
#define MMISTATUSBAR_ITEM_HEIGHT                     DP2PX_VALUE(30)  // item的最大高度
#define MMISTATUSBAR_ITEM_SPACE                      DP2PX_VALUE(2)   // item之间的具体

#if (!defined ADULT_WATCH_SUPPORT) && (defined SCREEN_SHAPE_CIRCULAR)
#define MMISTATUSBAR_ITEM_ICON_FRAME                 12   // 动画图标的最大数
#else
#define MMISTATUSBAR_ITEM_ICON_FRAME                 11   // 动画图标的最大数
#endif
#define MMISTATUSBAR_ITEM_TEXT_LEN                   14  // 文本的长度

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
/************************************************************************/
/*        MMI_STATUSBAR_STYLE_T                                         */
/************************************************************************/

typedef struct
{
    uint32              item_space;  // item space 
    uint32              item_width;  // item default width
    uint32              item_height; // item default height
    GUI_FONT_T          font_type;   // font type
    GUI_COLOR_T         font_color;  // font color
    GUI_ALIGN_E         align;       // font align

	uint8               layer_alpha;

    GUI_BG_T            bg_info;
    uint32              timer_out;
    uint32              scrollmsg_timer_out;
    uint32              scrollmsg_pause_timer_out;    
    BOOLEAN             is_transparent;
}MMI_STATUSBAR_STYLE_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get the status bar style, is called by tab control
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetStatusBarStyle(
                                          MMI_STATUSBAR_STYLE_T  *style_ptr
                                          );

/*****************************************************************************/
// 	Description : get the status bar rect
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMITHEME_GetStatusBarCtrlId(void);

/*****************************************************************************/
// 	Description : get the height of statusbar
//	Global resource dependence : 
//  Author:hua.fang
//	Note:
/*****************************************************************************/
PUBLIC uint32  MMITHEME_GetStatusBarHeight(void);

/*****************************************************************************/
// Description : 状态栏的TP消息自定义处理
// Global resource dependence : 
// Author: bin.ji
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMITHEME_HandleStatusbarTPMsg(
                                                  MMI_HANDLE_T win_handle, //窗口句柄
                                                  MMI_MESSAGE_ID_E  msg_id,   //事件类型
                                                  DPARAM            param     //附带的处理参数
                                                  );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // _MMITHEME_STATUSBAR_H_
/*! @} */
