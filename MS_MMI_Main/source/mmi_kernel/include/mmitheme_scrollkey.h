/***************************************************************************
** File Name:      mmitheme_scrollkey.h                                    *
** Author:         hua.fang                                                *
** Date:           08/03/2011                                              *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2011        hua.fang         Create                                  *
**																		   *
****************************************************************************/
#ifndef _MMITHEME_SCROLLKEY_H_
#define _MMITHEME_SCROLLKEY_H_

#ifdef GUIF_SCROLLKEY
/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "ctrlscrollkey_export.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef _cplusplus
	extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

typedef struct 
{
	MMI_IMAGE_ID_T		scroll_bar_up_id;
	MMI_IMAGE_ID_T		scroll_bar_down_id;
	MMI_IMAGE_ID_T		scroll_bar_confirm_id;
	MMI_IMAGE_ID_T		scroll_point_id;
}GUISCROLLKEY_SKIN_T[GUISCROLLKEY_BUTTON_NUM][GUISCROLLKEY_SKIN_MAX];

typedef struct 
{
    uint32						bg_type;					/*!< background type */
    MMI_IMAGE_ID_T				bg_image;					/*!< background image */
    GUI_COLOR_T					bg_color;					/*!< background color */
    GUI_COLOR_T					font_color;					/*!< font color */
    GUI_COLOR_T					pressed_font_color; 		/*!< font color */
    GUI_FONT_T					font;						/*!< font */    // 这个参数暂时没用上
    uint8						frameType;					/*!< frame type */
    uint16						char_space;					/*!< space between chars */
    uint16                      first_icon_offset;          // 第一个图标的偏移量
    uint16                      second_icon_offset;         // 第二个图标的偏移量
    MMK_SCROLL_BUTTON_T         skb_button[GUISCROLLKEY_BUTTON_NUM];
} MMI_SCROLLKEY_STYLE_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : get softkey text font
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC GUI_FONT_T MMITHEME_GetScrollKeyTextFont(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC uint32  MMITHEME_GetScrollkeyOffsetFromBottom(void);

/*****************************************************************************/
// 	Description : get the scrollkey bar rect
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMITHEME_GetScrollkeyCtrlId(void);

/*****************************************************************************/
// 	Description : get the height of the scrollkey area, is called by the MMI Kernel
//	Global resource dependence : 
//  Author:Robert.Lu
//	Note:
/*****************************************************************************/
PUBLIC uint32  MMITHEME_GetScrollkeyHeight(void);

/******************************************************************************/
/*! \brief get the scrollkey style, is called by the scrollkey control
 *  \param[out]		scrollkey_style_ptr		pointer to scrollkey style
 *  \author  Great.Tian
 *  \return 
 *		- <i><c>TRUE</c></i>: 	get success
 * 		- <i><c>FALSE</c></i>:	get failed
 */
/******************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetScrollkeyStyle(
										MMI_SCROLLKEY_STYLE_T* scrollkey_style_ptr,
										GUISCROLLSKIN_INDEX    left_scroll_skin_idx,
										GUISCROLLSKIN_INDEX    right_scroll_skin_idx
										);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetScrollkeySkin(
                                      GUISCROLLKEY_SKIN_T skin_table
	                                  );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif

#endif  // _MMITHEME_SCROLLKEY_H_
