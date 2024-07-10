/*****************************************************************************
** File Name:      mmitheme_widget.c                                         *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      bin.ji              Creat
******************************************************************************/

#define _MMITHEME_WIDGET_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "mmitheme_widget.h"
#include "mmi_image.h"
#include "mmidisplay_data.h"
#include "mmi_position.h"
#include "mmi_theme.h"
#include "mmiwidget_image.h"
#include "mmi_id.h"
#include "guires.h"
#include "mmiwidget_position.h"
#include "mmitheme_menu.h"
#include "mmitheme_mainmenu.h"
#include "mmi_common.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/



//每处理一次tp_move消息隔tp_move_next_interval个消息再处理下一个
#define MMIWIDGET_TP_MOVE_NEXT_INTERVAL_NUM     2

//滑页效果参数
#ifdef MAINLCD_SIZE_320X480
#define MMIWIDGET_SLIDEPAGE_RUN_STEPS           30 
#else
#define MMIWIDGET_SLIDEPAGE_RUN_STEPS           20 
#endif

#define MMIWIDGET_SLIDEPAGE_DELAY_TICKS         0
#define MMIWIDGET_SLIDEPAGE_EDGE                20

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get public windows theme,alert or query windows
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetWidgetTheme(
                          MMITHEME_WIDGET_T     *theme_ptr
                          )
{
#ifdef MMIWIDGET_SUPPORT  
    GUI_RECT_T shortcut_bar_rect = {0};//MMIWIDGET_SHORTCUTBAR_RECT;  
    GUI_RECT_T shortcut_bar_switch_rect = {0};//MMIWIDGET_SHORTCUTSWITCH_RECT;    
    uint16 bar_width = 0;
    uint16 bar_height = 0;
    uint16 switch_width = 0;
    uint16 switch_height = 0;
    MMI_IMAGE_ID_T shortcut_bar_bg_icon = 0;
    GUI_RECT_T shortcut_bar_bg_rect = {0};
#endif    
    GUI_RECT_T title_display_rect = MMITHEME_WIDGET_TITLE_ICON_RECT;
    int16 title_h_item_space = MMITHEME_WIDGET_TITLE_ICON_H_SPC;
    GUI_RECT_T h_title_display_rect = MMITHEME_H_WIDGET_TITLE_ICON_RECT;
    int16 h_title_h_item_space = MMITHEME_H_WIDGET_TITLE_ICON_H_SPC;
    
    //SCI_ASSERT(PNULL != theme_ptr); /*assert verified*/
    if (PNULL == theme_ptr)
    {
        return;
    }

#ifdef MMIWIDGET_SUPPORT
    //初始化widget shortcut数据
    theme_ptr->shortcut_data.disp_interval = MMIWIDGET_SHORTCUT_INTERVAL;

    //shortcut_bar_data
    if (MMITHEME_IsMainScreenLandscape())
    {
        shortcut_bar_bg_icon = IMAGE_WIDGET_H_BAR_BG;
    }
    else
    {
        shortcut_bar_bg_icon = IMAGE_WIDGET_BAR_BG;
    }
    GUIRES_GetImgWidthHeight(&bar_width, &bar_height, shortcut_bar_bg_icon, MAIN_IDLE_WIN_ID);
    shortcut_bar_rect.top = MMIWIDGET_SHORTCUT_TOP_GAP;
    shortcut_bar_rect.left = 0;
    shortcut_bar_rect.right = shortcut_bar_rect.left + bar_width - 1;
    shortcut_bar_rect.bottom = shortcut_bar_rect.top + bar_height -1;

    shortcut_bar_bg_rect = shortcut_bar_rect;
    shortcut_bar_rect.top +=MMIWIDGET_SHORTCUT_BG_ITEM_SPACE;
    shortcut_bar_rect.bottom -=MMIWIDGET_SHORTCUT_BG_ITEM_SPACE;
    theme_ptr->shortcut_data.bar_data.shortcut_bar_bg_rect = shortcut_bar_bg_rect;
    theme_ptr->shortcut_data.bar_data.shortcut_bar_rect = shortcut_bar_rect;
    theme_ptr->shortcut_data.bar_data.shortcut_bar_color = MMI_GRAY_COLOR;//MMI_DARK_GREEN_COLOR;
    theme_ptr->shortcut_data.bar_data.shortcut_bar_bg_icon = shortcut_bar_bg_icon;
    
    //shortcut_bar_switch_data
    GUIRES_GetImgWidthHeight(&switch_width, &switch_height, IMAGE_WIDGET_BAR_BUTTON_CLOSE_OFF, MAIN_IDLE_WIN_ID);
    shortcut_bar_switch_rect.left = shortcut_bar_bg_rect.right + 1;
    shortcut_bar_switch_rect.right = shortcut_bar_switch_rect.left + switch_width - 1;
    shortcut_bar_switch_rect.top =  shortcut_bar_bg_rect.top;
    shortcut_bar_switch_rect.bottom = shortcut_bar_bg_rect.top + switch_height -1;
    theme_ptr->shortcut_data.switch_data.shortcut_bar_switch_rect = shortcut_bar_switch_rect;
    theme_ptr->shortcut_data.switch_data.shortcut_bar_switch_color = MMI_YELLOW_COLOR;//MMI_DARK_GREEN_COLOR;
    theme_ptr->shortcut_data.switch_data.shortcut_bar_switch_close_off_icon = IMAGE_WIDGET_BAR_BUTTON_CLOSE_OFF;
    theme_ptr->shortcut_data.switch_data.shortcut_bar_switch_close_on_icon = IMAGE_WIDGET_BAR_BUTTON_CLOSE_ON;
    theme_ptr->shortcut_data.switch_data.shortcut_bar_switch_open_off_icon = IMAGE_WIDGET_BAR_BUTTON_OPEN_OFF;
    theme_ptr->shortcut_data.switch_data.shortcut_bar_switch_open_on_icon = IMAGE_WIDGET_BAR_BUTTON_OPEN_ON;    
#endif
    
    theme_ptr->page_data.unsel_bg_image = IMAGE_SLIDE_MAINMENU_TITLE_OFF;
    theme_ptr->page_data.sel_bg_image = IMAGE_SLIDE_MAINMENU_TITLE_ON;
    //计算page icon位置

    if (MMITHEME_IsMainScreenLandscape())
    {
        title_display_rect = h_title_display_rect;
        title_h_item_space = h_title_h_item_space;
    }
    GUI_CreateMatrixRect(&title_display_rect,
        0, 
        0, 
        title_h_item_space, 
        0, 
        1, 
        MMITHEME_WIDGET_PAGE_NUM,
        theme_ptr->page_data.img_disp_rect
        );

    theme_ptr->slidepage_run_steps = MMIWIDGET_SLIDEPAGE_RUN_STEPS;
    theme_ptr->slidepage_delay_ticks = MMIWIDGET_SLIDEPAGE_DELAY_TICKS;
    theme_ptr->slide_edge = MMIWIDGET_SLIDEPAGE_EDGE;

    theme_ptr->shortcut_text_font = MMITHEME_MAINMENU_TEXT_FONT;
    theme_ptr->shortcut_text_color= MMI_WHITE_COLOR;//MMITHEME_GetCurThemeFontColor(MMI_THEME_MAINMENU_FONT);
#ifdef MMI_MAINMENU_MINI_SUPPORT
    theme_ptr->shortcut_text_bg = IMAGE_NULL; 
#else
	theme_ptr->shortcut_text_bg = IMAGE_MAINMENU_ITEM_TEXT_BG;
#endif
}

#ifdef MMIWIDGET_SUPPORT
/*****************************************************************************/
//  Description : get public windows theme,alert or query windows
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetPBWidgetTheme(
                          MMITHEME_WIDGET_PB_T     *theme_ptr
                          )
{
    //SCI_ASSERT(PNULL != theme_ptr); /*assert verified*/
    if (PNULL == theme_ptr)
    {
        return;
    }
    
    theme_ptr->rect_color = MMI_BLUE_COLOR;
    theme_ptr->bg_img = IMAGE_WIDGET_PB_BG;
    theme_ptr->default_img = IMAGE_WIDGET_PB_ANIM_DEFAULT;
    theme_ptr->error_img = IMAGE_WIDGET_PB_ANIM_EMPTY;
}
#endif

#ifdef MMI_GRID_MP3
/*****************************************************************************/
//  Description : get public windows theme,alert or query windows
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetMp3WidgetTheme(
                          MMITHEME_WIDGET_MP3_T     *theme_ptr
                          )
{
    //SCI_ASSERT(PNULL != theme_ptr); /*assert verified*/
    if (PNULL == theme_ptr)
    {
        return;
    }
    
    theme_ptr->bg_img = IMAGE_WIDGET_MP3_BG;
   // theme_ptr->entry_on_img = IMAGE_WIDGET_MP3_ENTRY_ON;
    theme_ptr->mp3name_font = SONG_FONT_16;
    theme_ptr->mp3name_color = MMITHEME_GetDefaultWidgetThemeColor();//MMI_GRAY_WHITE_COLOR;
}
#endif

/*****************************************************************************/
//  Description : get public windows theme,alert or query windows
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetClockWidgetTheme(
                          MMITHEME_WIDGET_CLOCK_T     *theme_ptr
                          )
{
    //SCI_ASSERT(PNULL != theme_ptr); /*assert verified*/
    if (PNULL == theme_ptr)
    {
        return;
    }

    theme_ptr->time_font = SONG_FONT_32;
    theme_ptr->date_font = SONG_FONT_14;
    theme_ptr->week_font = SONG_FONT_14;
    theme_ptr->clock_font_color = MMI_WHITE_COLOR;
    theme_ptr->date_font_color = MMITHEME_GetDefaultWidgetThemeColor();//MMI_GRAY_WHITE_COLOR;
}

/*****************************************************************************/
//  Description : get widget dual clock(digital & analog) theme.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetDualClockWidgetTheme(
                          MMITHEME_WIDGET_CLOCK_T     *theme_ptr
                          )
{
    //SCI_ASSERT(PNULL != theme_ptr); /*assert verified*/
    if (PNULL == theme_ptr)
    {
        return;
    }
    
    theme_ptr->time_font = MMI_DEFAULT_NORMAL_FONT;//MMI_DEFAULT_BIG_FONT;
    theme_ptr->date_font = MMI_DEFAULT_NORMAL_FONT;
    theme_ptr->week_font = MMI_DEFAULT_NORMAL_FONT;
    theme_ptr->city_font = MMI_DEFAULT_NORMAL_FONT;//MMI_DEFAULT_BIG_FONT;      
    theme_ptr->clock_font_color = MMI_BLACK_COLOR;   
    theme_ptr->city_font_color = MMI_DARK_GRAY_COLOR;
}

/*****************************************************************************/
//  Description : get widget dual clock(digital & analog) theme.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetADualClockWidgetTheme(
                          MMITHEME_WIDGET_CLOCK_T     *theme_ptr
                          )
{
    //SCI_ASSERT(PNULL != theme_ptr); /*assert verified*/
    if (PNULL == theme_ptr)
    {
        return;
    }
    
    theme_ptr->city_font = SONG_FONT_20;      
    theme_ptr->city_font_color = MMI_WHITE_COLOR;
}

/*****************************************************************************/
//  Description : get default widget theme color.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T MMITHEME_GetDefaultWidgetThemeColor(void)
{
//@for light function modify
#if (defined (MAINLCD_SIZE_240X320) ) && (!defined(MMI_GRID_IDLE_SUPPORT))
    return MMITHEME_GetCurThemeFontColor(MMI_THEME_PUBWIN_SOFTKEY_FONT);
#else
    return MMI_WHITE_COLOR;
#endif
}

#if defined(FM_SUPPORT)
/*****************************************************************************/
//  Description : get widget fm theme.
//  Global resource dependence : 
//  Author: paul.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetFMWidgetTheme(
                          MMITHEME_WIDGET_FM_T     *theme_ptr
                          )
{
    //SCI_ASSERT(PNULL != theme_ptr); /*assert verified*/
    if (PNULL == theme_ptr)
    {
        return;
    }


    theme_ptr->name_font = MMI_DEFAULT_NORMAL_FONT;//MMI_DEFAULT_BIG_FONT;
    theme_ptr->name_color = MMI_WHITE_COLOR;
}
#endif
