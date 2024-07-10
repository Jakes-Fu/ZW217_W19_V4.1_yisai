/****************************************************************************
** File Name:      mmitheme_setlist.c                                       *
** Author:         Xiaoqing.lu                                              *
** Date:           11/04/2009                                               *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the mmi theme.             *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 04/2009        Xiaoqing.lu       Create
** 
****************************************************************************/

#ifndef _MMITHEME_SETLIST_C_
#define _MMITHEME_SETLIST_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmidisplay_data.h"
//#include "mmi_id.h"
#include "mmi_image.h"
#include "guifont.h"
#include "mmiset_display.h"
#include "mmitheme_setlist.h"
#include "mmi_setlist_win.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
LOCAL MMI_IMAGE_ID_T s_item_def_icon_id = IMAGE_NULL;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get the status bar style, is called by tab control
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetSetlistStyle(
                                        MMI_GUISETLIST_STYLE_T  *style_ptr
                                        )
{

    //SCI_PASSERT(PNULL != style_ptr, ("MMITHEME_GetSetlistStyle: the param is PNULL!")); /*assert verified*/
    if (PNULL == style_ptr)
    {
        return FALSE;
    }

    style_ptr->char_space = 0;
    style_ptr->font_type = MMI_DEFAULT_TEXT_FONT;
    style_ptr->font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_LABEL_FONT);
    style_ptr->align = ALIGN_HVMIDDLE;

    // 背景设置
    style_ptr->bg_info.bg_type = GUI_BG_IMG;
    style_ptr->bg_info.img_id = IMAGE_COMMON_BG;
    style_ptr->bg_info.color = MMI_DARK_GRAY_COLOR;
    style_ptr->bg_info.shape = GUI_SHAPE_ROUNDED_RECT;

    // 状态
    style_ptr->str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ALIGN_BY_DIR;
    style_ptr->effect = FONT_EFFECT_NONE;
    style_ptr->timer_out = MMISETLIST_ICONANIM_TIMEOUT;

    // 图片显示在文本的前面
    style_ptr->layout = GUISETLIST_LAYOUT_TEXT;

    style_ptr->left_margin = MMISETLIST_TEXT_MARGIN;
    style_ptr->right_margin = MMISETLIST_TEXT_MARGIN;
    
    // 边框信息
    style_ptr->border.color = MMI_BACKGROUND_COLOR;
    style_ptr->border.width = 1;
    style_ptr->border.type = GUI_BORDER_ROUNDED;

    // 箭头图片信息
#ifdef TOUCH_PANEL_SUPPORT
    style_ptr->left_arrow_icon_down = IMAGE_SETLIST_ARROW_LEFT_DOWN;
	style_ptr->left_arrow_icon_up = IMAGE_SETLIST_ARROW_LEFT_UP;
    style_ptr->right_arrow_icon_down = IMAGE_SETLIST_ARROW_RIGHT_DOWN;
	style_ptr->right_arrow_icon_up = IMAGE_SETLIST_ARROW_RIGHT_UP;
#else
    #ifndef MMI_RES_LOW_COST
        style_ptr->left_arrow_icon_down = IMAGE_SETLIST_ARROW_LEFT_UP;
    	style_ptr->left_arrow_icon_up = IMAGE_SETLIST_ARROW_LEFT_UP;
        style_ptr->right_arrow_icon_down = IMAGE_SETLIST_ARROW_RIGHT_UP;
    	style_ptr->right_arrow_icon_up = IMAGE_SETLIST_ARROW_RIGHT_UP;
    #endif
#endif

    s_item_def_icon_id = IMAGE_NULL;

    return TRUE;
} 

/*****************************************************************************/
//  Description : 设置默认的最大item数值
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMITHEME_GetSetMaxItemNum(void)
{
    return MMISETLIST_ITEM_MAX;
} 

/*****************************************************************************/
//  Description : 当显示动画的时候,获取动画显示的默认图片
//  Global resource dependence : 
//  Author: 
//  Note: 当动画解析错误或者解析之前显示的图片
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMITHEME_GetDefaultItemIcon(void)
{
    return s_item_def_icon_id;
} 

/*****************************************************************************/
//  Description : 当显示动画的时候,获取动画显示的默认图片
//  Global resource dependence : 
//  Author: 
//  Note: 当动画解析错误或者解析之前显示的图片
/*****************************************************************************/
PUBLIC void MMITHEME_SetDefaultItemIcon(
                                        MMI_IMAGE_ID_T icon_id
                                        )
{
    s_item_def_icon_id = icon_id;
} 

/*****************************************************************************/
//  Description : 打开setlist窗口
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_CreateSetlistChildWin(
                                           MMI_OPEN_SETLIST_WIN_T *data_ptr
                                           )
{
    MMISETLIST_CreateWin(data_ptr);
}

#endif // _MMITHEME_SETLIST_C_
