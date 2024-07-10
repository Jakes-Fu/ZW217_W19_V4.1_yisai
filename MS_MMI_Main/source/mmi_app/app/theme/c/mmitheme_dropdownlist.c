/****************************************************************************
** File Name:      mmitheme_dropdownlist.c                                  *
** Author:         Xiaoqing.lu                                              *
** Date:           6/2/2009                                                 *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the mmi theme.             *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 02/2009        Xiaoqing.lu       Create                                  *
**                                                                          *
****************************************************************************/
#define _MMITHEME_DROPDOWNLIST_C_
/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmi_image.h"
#include "mmidisplay_data.h"
#include "guifont.h"
#include "mmitheme_dropdownlist.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define MMI_DEFAULT_DROPDOWNLIST_SHOWITEM_NUM   5

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get the dropdownlist style, is called by dropdownlist control
//  Global resource dependence : 
//  Author: yelianna
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetDropDownListStyle(
											 MMI_DROPDOWNLIST_STYLE_T  *style_ptr
											 )

{

   	//获得系统默认的风格，主要包括字体、背景的设置   
    //SCI_PASSERT(PNULL != style_ptr, ("MMITHEME_GetDropDownListStyle: the param is PNULL!")); /*assert verified*/
    if (PNULL == style_ptr)
    {
        return FALSE;
    }

    style_ptr->item_height = MMI_LIST_ITEM_HEIGHT;
    
    // 边框
    style_ptr->border.color = MMI_WHITE_COLOR;
    style_ptr->border.width = DP2PX_VALUE(1);
    style_ptr->border.type = GUI_BORDER_ROUNDED;

    // 背景
    style_ptr->bg_info.bg_type = GUI_BG_COLOR;    
    style_ptr->bg_info.color = MMI_WHITE_COLOR;
    style_ptr->bg_info.img_id = IMAGE_COMMON_BG;
    style_ptr->bg_info.shape = GUI_SHAPE_ROUNDED_RECT;

    // 默认打开list控件的背景
    style_ptr->list_bg_info = style_ptr->bg_info;

    // font
    style_ptr->font = MMI_DEFAULT_TEXT_FONT;
    style_ptr->font_space = 0;
    //@for light function modify
#if defined MAINLCD_SIZE_240X320 
    style_ptr->font_color = MMI_BACKGROUND_COLOR;
#else
    style_ptr->font_color = MMI_GRAY_WHITE_COLOR;
#endif

    // other
    style_ptr->left_space           = MMI_DNLIST_TEXT_LEFT_SPACE;
    style_ptr->unpressed_arrow_icon = IMAGE_DROPDOWNLIST_ARROW_UN;
    style_ptr->pressed_arrow_icon   = IMAGE_DROPDOWNLIST_ARROW_PR;

    style_ptr->default_item_num = MMI_DEFAULT_DROPDOWNLIST_SHOWITEM_NUM;
    return TRUE;
}

