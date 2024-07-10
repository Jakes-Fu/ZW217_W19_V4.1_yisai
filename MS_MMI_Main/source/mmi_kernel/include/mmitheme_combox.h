/******************************************************************************
** File Name:      mmitheme_combox.h                                         *
** Author:         xiaoqing.lu                                                *
** Date:           11/04/2009                                                 *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.          *
** Description:    This file is used to describe the data struct of           *
**                              guistatusbar control                          *
*******************************************************************************
**                         Important Edit History                             *
** ---------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 04/2009        Xiaoqing.lu      Create			                          *
******************************************************************************/

#ifndef _MMITHEME_SETLIST_H_
#define _MMITHEME_SETLIST_H_ 

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "mmk_type.h"
#include "guicommon.h"
#include "guiblock.h"
#include "guifont.h"
#include "guistring.h"
#include "guilistbox.h"

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

/*----------------------------------------------------------------------------*/
/*                         TYPE AND STRUCT                                    */
/*----------------------------------------------------------------------------*/
//the dropdownlist display style 
typedef struct
{
	GUI_BORDER_T   border;
	GUI_COLOR_T    font_color;            //字体颜色
	GUI_FONT_T     font;                  //字体
	uint8          font_space;            //字间距
	BOOLEAN        is_ellipsis;

	uint16         item_height;           //每条Item的高度
	uint16         default_item_num;      //每条Item的高度       
	uint8          left_space;            //字符左间距
	MMI_IMAGE_ID_T    unpressed_arrow_icon;   //未按下的下拉箭头图标
	MMI_IMAGE_ID_T    pressed_arrow_icon;     //已按下的下拉箭头图标
	
	GUI_BG_T      bg_info;
	GUI_BG_T      list_bg_info;
	
	BOOLEAN       has_icon;
	BOOLEAN       has_expand_button;
	BOOLEAN       has_function_button;
	BOOLEAN       is_list_expand;
}MMI_COMBOX_STYLE_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Get ComBox Style
//  Global resource dependence : 
//  Author: nan & xiyuan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetComBoxStyle(
									   MMI_COMBOX_STYLE_T* style_ptr
									   );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif // _MMITHEME_SETLIST_H_
/*! @} */
