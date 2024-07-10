/****************************************************************************
** File Name:      mmitheme_tips.c                                          *
** Author:         Xiaoqing.lu                                              *
** Date:           2/20/2009                                                *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the mmi theme.             *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 2/2009         Xiaoqing.lu      Create                                   *
**                                                                          *
****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmitheme_tips.h"
#include "mmi_id.h"
#include "mmi_image.h"
#include "guifont.h"
#include "mmidisplay_data.h"
#include "mmi_theme.h"
#include "guistring.h"
#include "mmi_color.h"
#include "mmi_resource.h"
#include "mmitheme_combox.h"
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
#define MMI_DEFAULT_COMBOX_SHOWITEM_NUM 5
/*----------------------------------------------------------------------------*/
/*                         TYPE AND STRUCT                                    */
/*----------------------------------------------------------------------------*/

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
									   )
{
	if (PNULL == style_ptr)
	{
		return FALSE;
	}

	//border
    style_ptr->border.color = MMI_WHITE_COLOR;
    style_ptr->border.width = 1;
    style_ptr->border.type  = GUI_BORDER_ROUNDED;

	//font
    style_ptr->font = MMI_DEFAULT_TEXT_FONT;
    style_ptr->font_color = MMI_BLACK_COLOR;
    
	//item num
    style_ptr->item_height      = MMI_LIST_ITEM_HEIGHT;
    style_ptr->default_item_num = MMI_DEFAULT_COMBOX_SHOWITEM_NUM;

	//other
    style_ptr->left_space           = MMI_DNLIST_TEXT_LEFT_SPACE;
    style_ptr->unpressed_arrow_icon = IMAGE_DROPDOWNLIST_ARROW_UN;
    style_ptr->pressed_arrow_icon   = IMAGE_DROPDOWNLIST_ARROW_PR;
    
	//bg
    style_ptr->bg_info.bg_type = GUI_BG_COLOR;    
    style_ptr->bg_info.color   = MMI_WHITE_COLOR;
    style_ptr->bg_info.img_id  = IMAGE_COMMON_BG;
    style_ptr->bg_info.shape   = GUI_SHAPE_ROUNDED_RECT;

	style_ptr->has_icon = FALSE;
	style_ptr->has_expand_button   = TRUE;
	style_ptr->has_function_button = FALSE;
	style_ptr->is_list_expand = FALSE;

	return TRUE;
}                                             
