/****************************************************************************
** File Name:      mmi_theme.c                                              *
** Author:         Xiaoqing.lu                                              *
** Date:           12/15/2008                                               *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the mmi theme.             *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 12/2008        Xiaoqing.lu       Create
** 
****************************************************************************/
#define _MMITHEME_TAB_C_
/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "std_header.h"
#include "mmitheme_tab.h"
#include "mmi_theme.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "guilcd.h"
#include "mmi_image.h"
#include "mmidisplay_data.h"
#include "mmi_text.h"
//#include "mmi_id.h"
#include "mmi_default.h"
#include "mmi_textfun.h"
#include "guifont.h"
#include "mmiidle_export.h"
#include "mmk_app.h"
//#include "mmi_wallpaper_export.h"
#include "mmiset_id.h"
#include "mmifmm_id.h"
#include "mmitheme_special_func.h"
//#include "mmiset_display.h"
#include "mmi_theme.h"
#include "guitab.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#ifdef MAINLCD_SIZE_320X480
#define MMITHEME_TAB_FONT	SONG_FONT_14
#else
#define MMITHEME_TAB_FONT	SONG_FONT_16
#endif

#ifdef GUITAB_PDASTYLE_SUPPORT
#define MMITHEME_TAB_ISTYLE_FONT	SONG_FONT_14
#endif


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
//  Description : get the tab style, is called by tab control
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetTabStyle(MMI_TAB_STYLE_T  *style_ptr)
{
	//SCI_PASSERT(PNULL != style_ptr, ("MMITHEME_GetTabStyle: the param is PNULL!")); /*assert verified*/
	if (PNULL == style_ptr)
	{
        return FALSE;
	}

#ifdef MONOCHROME_LCD_SUPPORT 
	style_ptr->bg_image_id = 0;
#else
	style_ptr->bg_image_id = IMAGE_TITLE_BAR2;
#endif//syy modify 2010.11.29 -end
//fixed by hongjun.jia for bugzilla 2451/2452
#if (!defined TOUCHPANEL_TYPE_NONE) &&(!defined PIC_GLIDE_SUPPORT)
	style_ptr->left_scroll_pressed_image_id = IMAGE_TAB_LEFT_PRESSED;
	style_ptr->left_scroll_released_image_id = IMAGE_TAB_LEFT_RELEASED;
	style_ptr->left_scroll_disabled_image_id = IMAGE_TAB_LEFT_DISABLED;
	style_ptr->right_scroll_pressed_image_id = IMAGE_TAB_RIGHT_PRESSED;
	style_ptr->right_scroll_released_image_id = IMAGE_TAB_RIGHT_RELEASED;
	style_ptr->right_scroll_disabled_image_id = IMAGE_TAB_RIGHT_DISABLED;
#endif	
#ifdef MONOCHROME_LCD_SUPPORT 
	style_ptr->bg_color = MMI_WHITE_COLOR;
#else
	style_ptr->bg_color = MMI_BACKGROUND_COLOR;
#endif
	style_ptr->sel_item_color = MMI_TAB_DEFAULT_FOCUS_COLOR;
	style_ptr->unsel_item_color = MMI_BLUE_COLOR;
	style_ptr->focus_rect_color = MMI_TAB_DEFAULT_FOCUS_COLOR;
	
	style_ptr->font_space =  0;
	style_ptr->item_default_width = MMITHEME_GetTabDefWidth();//MMITAB_DEFAULT_ITEM_WIDTH;
	style_ptr->item_default_height = MMITAB_DEFAULT_ITEM_HEIGHT;
	style_ptr->item_deficon_id = IMAGE_COMMON_PIC_ICON;
	style_ptr->item_error_id = IMAGE_COMMON_PIC_ERROR;
	style_ptr->scroll_card_def_width = MMI_TAB_CARD_DEFAULT_WIDTH;
	style_ptr->text_margin.left = 0;
	style_ptr->text_margin.top   = 0;
	style_ptr->text_margin.right = 0;
	style_ptr->text_margin.bottom = MMITAB_DEFAULT_ITEM_TEXT_BOTTOM_SPACE;
	
	style_ptr->height_bg_info.bg_type = GUI_BG_IMG;
	style_ptr->height_bg_info.img_id = IMAGE_TAB_SELECT1;
	style_ptr->height_bg_margin.left   = 0;
	style_ptr->height_bg_margin.top    = 0;
	style_ptr->height_bg_margin.right  = 0;
	style_ptr->height_bg_margin.bottom = 1 ;
#ifdef DPHONE_SUPPORT 
	style_ptr->is_show_cur_item_only = TRUE;
#else
	style_ptr->is_show_cur_item_only = FALSE;
#endif//syy modify 2010.11.29 -end
	
#ifdef GUITAB_PDASTYLE_SUPPORT
	style_ptr->prg_scroll_id  = IMAGE_TAB_SCROLL;
	
	style_ptr->sel_font_color = MMI_GRAY_WHITE_COLOR;
	
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
	if (MMITHEME_IsIstyle())
	{
		style_ptr->height_bg_info.img_id = IMAGE_ISTYLE_TAB_HIGHLIGHT_BG;
		style_ptr->bg_image_id = IMAGE_ISTYLE_TAB_BG;
		style_ptr->tp_sel_img_id  = IMAGE_ISTYLE_TAB_HIGHLIGHT_BG;
		style_ptr->dividing_line = IMAGE_NULL;
		style_ptr->font = MMITHEME_TAB_ISTYLE_FONT;
		style_ptr->font_color = MMI_WHITE_COLOR;
	}
	else
#endif
	{
		style_ptr->bg_image_id = IMAGE_TAB_BG;
		//style_ptr->tp_sel_img_id  = IMAGE_TAB_TP_SELECT1_PDA;
		style_ptr->tp_sel_img_id  = IMAGE_TAB_SELECT1;
		style_ptr->dividing_line  = IMAGE_TAB_LINE;
		style_ptr->font			  = MMITHEME_TAB_FONT;
		style_ptr->font_color	  = MMI_WEAK_DARK_COLOR;
	}
#ifdef MAINLCD_SIZE_320X480 
	style_ptr->item_state  = GUITAB_ITEMSTATE_UIMG_DTEXT;
#else
#if defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle())
    {
        style_ptr->item_state  = GUITAB_ITEMSTATE_UIMG_DTEXT;
    }
    else
#endif		
	{
    	style_ptr->item_state  = GUITAB_ITEMSTATE_SHOW_TEXT;
    }
#endif
	
	style_ptr->prg_height  = MMI_TAB_PRG_HEIGHT;
#else
	style_ptr->font			  = MMI_TITLE_TEXT_FONT;
	style_ptr->font_color	  = MMITHEME_GetCurThemeFontColor(MMI_THEME_TAB_FONT);
	style_ptr->sel_font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_TAB_FONT);
	
	style_ptr->item_state = GUITAB_ITEMSTATE_SHOW_IMG;
#endif
	
    style_ptr->default_item_num = MMITHEME_GetTabDefDispItemNum();
	
	return TRUE;
} 

/*****************************************************************************/
// 	Description : get the tab child win top
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetTabHeight(
                                    void
                                    )
{
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    uint16 tab_height = 0;

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

#ifdef MAINLCD_SIZE_128X64 
    if (0)	 
#else
    if (lcd_width > lcd_height)
#endif
    {
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
        if (MMITHEME_IsIstyle())
        {
            tab_height = MMI_ISTYLE_TAB_HEIGHT_H;
        }
        else
#endif
        {
            tab_height = MMI_TAB_HEIGHT_H;
        }
    }
    else 
    {
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
        if (MMITHEME_IsIstyle())
        {
            tab_height = MMI_ISTYLE_TAB_HEIGHT;
        }
        else
#endif
        {
            tab_height = MMI_TAB_HEIGHT;
        }
    }
    return tab_height;
}

/*****************************************************************************/
// 	Description : get the tab default width
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetTabDefWidth(void)
{
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    uint16 tab_width = 0;

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    if (lcd_width > lcd_height)
    {
        tab_width = MMI_TAB_ITEM_DEFAULT_WIDTH_H;
    }
    else 
    {
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
        if (MMITHEME_IsIstyle())
        {
            tab_width = MMI_TAB_ITEM_DEFAULT_WIDTH_ISTYLE;
        }
        else
#endif
        {
            tab_width = MMI_TAB_ITEM_DEFAULT_WIDTH;
        }
    }
    return tab_width;
}

/*****************************************************************************/
// 	Description : get the tab default display item num
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetTabDefDispItemNum(void)
{
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    uint16 item_num = 0;

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    if (lcd_width > lcd_height)
    {
        item_num = MMI_TAB_DISP_MAX_ITEM_NUM_H;
    }
    else 
    {
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
        if (MMITHEME_IsIstyle())
        {
            item_num = MMI_TAB_DISP_MAX_ITEM_NUM_ISTYLE;
        }
        else
#endif
        {
            item_num = MMI_TAB_DISP_MAX_ITEM_NUM;
        }
    }
    return item_num;
}

/*****************************************************************************/
// 	Description : get the tab child win top
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetTabChildWinTop(
                                         MMI_HANDLE_T win_handle
                                         )
{
    uint16 top = 0;

#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle())
    {
        if (MMK_IsIncludeStatusBar(win_handle))
        {
            top = MMI_STATUSBAR_HEIGHT;
        }

        //title
        top += MMI_TITLE_THEME_HEIGHT;
    }
    else
#endif
    {
        if (MMK_IsIncludeStatusBar(win_handle))
        {
            top = MMI_TAB_CHILD_WINDOW_TOP + MMI_STATUSBAR_HEIGHT;
        }
        else 
        {
            top = MMI_TAB_CHILD_WINDOW_TOP;
        }
    }

    return top;
}

