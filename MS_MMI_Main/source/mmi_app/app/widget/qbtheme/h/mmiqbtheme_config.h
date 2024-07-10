/*****************************************************************************
** File Name:      mmiqbtheme_config.h                                       *
** Author:                                                                   *
** Date:           08/10/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe phonebook                   *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2011        YING.XU      Create
******************************************************************************/

#ifndef _MMIQBTHEME_CONFIG_H_
#define _MMIQBTHEME_CONFIG_H_ 

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

//widget 拖入dockbar后缩放的大小
#define MMIQBTHEME_THUMB_WIDGET_THUMB_SIZE (MMI_MAINSCREEN_WIDTH / 4)

//max item num in dockbar page, not including special item
#define MMIQBTHEME_DOCKBAR_PAGE_MAX_ITEM (9)
#define MMIQBTHEME_DOCKBAR_PAGE_MAX_DISPLAY_ITEM (6)

#define MMIQBTHEME_MAX_DOCKBAR_PAGE_NUM 3

#define MMIQBTHEME_SLIDING_DELEY_TICKS (0) //20ms per frame


#if defined(MAINLCD_SIZE_320X480)
//widget页面的行列数
#define MMIQBTHEME_GRID_COL_NUM                    4
#define MMIQBTHEME_GRID_ROW_NUM                    4

#define MMIQBTHEME_DOCKBAR_PAGE2_MAX_ITEM   4

//page tilte height, area  displaying page1-9 icon
#define MMIQBTHEME_PAGE_TITLE_HEIGHT   18

#define MMIQBTHEME_DOCKBAR_HEIGHT   85
#define MMIQBTHEME_DOCKBAR_ITEM_HEIGHT   MMIQBTHEME_DOCKBAR_HEIGHT
#define MMIQBTHEME_DOCKBAR_ITEM_WIDTH  MMITHEME_MENU_ITEM_WIDTH   // MMI_MAINSCREEN_WIDTH/MMIQBTHEME_DOCKBAR_PAGE2_MAX_ITEM

//dockbar中菜单相关的位置定义
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_HEIGHT  MMITHEME_MENU_ITEM_HEIGHT 
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_WIDTH   MMITHEME_MENU_ITEM_WIDTH
//菜单项icon的大小
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_ICON_SIZE   MENUICON_HEIGHT
//菜单项text的大小
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_TEXT_FONT  MMITHEME_MAINMENU_TEXT_FONT
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_TEXT_HEIGHT  MENUTEXT_HEIGHT 
//widget page预览项的大小数据
#define MMIQBTHEME_DOCKBAR_PREVIEW_ITEM_WIDTH  48   // MMI_MAINSCREEN_WIDTH/MMIQBTHEME_DOCKBAR_PAGE_MAX_DISPLAY_ITEM
#define MMIQBTHEME_DOCKBAR_PREVIEW_ITEM_HEIGHT  72

#elif defined(MAINLCD_SIZE_240X320)
//widget页面的行列数
#define MMIQBTHEME_GRID_ROW_NUM            3
#define MMIQBTHEME_GRID_COL_NUM            3

#define MMIQBTHEME_DOCKBAR_PAGE2_MAX_ITEM  3

//page tilte height, area  displaying page1-9 icon
#define MMIQBTHEME_PAGE_TITLE_HEIGHT   16

#define MMIQBTHEME_DOCKBAR_HEIGHT         75     // 44 + 16
#define MMIQBTHEME_DOCKBAR_ITEM_HEIGHT   MMIQBTHEME_DOCKBAR_HEIGHT 
#define MMIQBTHEME_DOCKBAR_ITEM_WIDTH    MMITHEME_MENU_ITEM_WIDTH   // MMI_MAINSCREEN_WIDTH/MMIQBTHEME_DOCKBAR_PAGE2_MAX_ITEM

//dockbar中菜单相关的位置定义
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_HEIGHT  MMITHEME_MENU_ITEM_HEIGHT 
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_WIDTH   MMITHEME_MENU_ITEM_WIDTH
//菜单项icon的大小
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_ICON_SIZE   MENUICON_HEIGHT
//菜单项text的大小
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_TEXT_FONT  MMITHEME_MAINMENU_TEXT_FONT
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_TEXT_HEIGHT  MENUTEXT_HEIGHT 


//widget page预览项的大小数据
#define MMIQBTHEME_DOCKBAR_PREVIEW_ITEM_WIDTH   36  // MMI_MAINSCREEN_WIDTH/MMIQBTHEME_DOCKBAR_PAGE_MAX_DISPLAY_ITEM
#define MMIQBTHEME_DOCKBAR_PREVIEW_ITEM_HEIGHT  48  //36/240*320

#elif defined(MAINLCD_SIZE_240X400)
//widget页面的行列数
#define MMIQBTHEME_GRID_COL_NUM            3
#define MMIQBTHEME_GRID_ROW_NUM            3

#define MMIQBTHEME_DOCKBAR_PAGE2_MAX_ITEM  3

//page tilte height, area  displaying page1-9 icon
#define MMIQBTHEME_PAGE_TITLE_HEIGHT   20


#define MMIQBTHEME_DOCKBAR_HEIGHT        75     // 44 + 16
#define MMIQBTHEME_DOCKBAR_ITEM_HEIGHT   MMIQBTHEME_DOCKBAR_HEIGHT 
#define MMIQBTHEME_DOCKBAR_ITEM_WIDTH    MMITHEME_MENU_ITEM_WIDTH   // MMI_MAINSCREEN_WIDTH/MMIQBTHEME_DOCKBAR_PAGE2_MAX_ITEM

//dockbar中菜单相关的位置定义
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_HEIGHT  MMITHEME_MENU_ITEM_HEIGHT 
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_WIDTH   MMITHEME_MENU_ITEM_WIDTH
//菜单项icon的大小
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_ICON_SIZE   MENUICON_HEIGHT
//菜单项text的大小
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_TEXT_FONT  MMITHEME_MAINMENU_TEXT_FONT
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_TEXT_HEIGHT  MENUTEXT_HEIGHT 


//widget page预览项的大小数据
#define MMIQBTHEME_DOCKBAR_PREVIEW_ITEM_WIDTH   36  // MMI_MAINSCREEN_WIDTH/MMIQBTHEME_DOCKBAR_PAGE_MAX_DISPLAY_ITEM
#define MMIQBTHEME_DOCKBAR_PREVIEW_ITEM_HEIGHT  60  

#else
#define MMIQBTHEME_GRID_COL_NUM            4
#define MMIQBTHEME_GRID_ROW_NUM            4

#define MMIQBTHEME_DOCKBAR_PAGE2_MAX_ITEM   4

//page tilte height, area  displaying page1-9 icon
#define MMIQBTHEME_PAGE_TITLE_HEIGHT   16

#define MMIQBTHEME_DOCKBAR_HEIGHT   60
#define MMIQBTHEME_DOCKBAR_ITEM_HEIGHT   MMIQBTHEME_DOCKBAR_HEIGHT 

#define MMIQBTHEME_DOCKBAR_ITEM_WIDTH  MMITHEME_MENU_ITEM_WIDTH   // MMI_MAINSCREEN_WIDTH/MMIQBTHEME_DOCKBAR_PAGE2_MAX_ITEM

//dockbar中菜单相关的位置定义
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_HEIGHT  MMITHEME_MENU_ITEM_HEIGHT 
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_WIDTH   MMITHEME_MENU_ITEM_WIDTH
//菜单项icon的大小
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_ICON_SIZE   MENUICON_HEIGHT
//菜单项text的大小
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_TEXT_FONT  MMITHEME_MAINMENU_TEXT_FONT
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_TEXT_HEIGHT  MENUTEXT_HEIGHT 


//widget page预览项的大小数据
#define MMIQBTHEME_DOCKBAR_PREVIEW_ITEM_WIDTH  36   // MMI_MAINSCREEN_WIDTH/MMIQBTHEME_DOCKBAR_PAGE3_MAX_ITEM
#define MMIQBTHEME_DOCKBAR_PREVIEW_ITEM_HEIGHT  (MMIQBTHEME_DOCKBAR_PREVIEW_ITEM_WIDTH*1.5)
#endif

#define MMIQBTHEME_GRID_PAGE_CELL_NUM    (MMIQBTHEME_GRID_ROW_NUM*MMIQBTHEME_GRID_COL_NUM)
#define MMIQBTHEME_SHORTCUT_ITEM_MAX     (MMIQBTHEME_GRID_PAGE_CELL_NUM * MMIQBTHEME_MAX_WIDGET_PAGE_NUM)
#define MMIQBTHEME_WIDGET_PAGE_MAX_ITEM  (MMIQBTHEME_GRID_ROW_NUM*MMIQBTHEME_GRID_COL_NUM)



// FSM state 
typedef enum
{
    MMIQBTHEME_NORMAL,
    MMIQBTHEME_EDITING,
    MMIQBTHEME_PAGE_SLIDING,
    MMIQBTHEME_ITEM_MOVING,
}MMIQBTHEME_FSM_STATE_E;
/*
typedef enum
{
    MMIQBTHEME_NORMAL = 0,
    MMIQBTHEME_EDITING = 1,
    MMIQBTHEME_PAGE_SLIDING = 2 ,
    MMIQBTHEME_ITEM_MOVING = 4 ,     //和page_slideing状态可能同时存在，所以改用1bit表示1个状态
}MMIQBTHEME_FSM_STATE_E;
*/
//平台显示相关的结构描述
typedef struct
{    
    MMI_WIN_ID_T      win_id;
    GUI_LCD_DEV_INFO lcd_dev_info; //lcd dev info  
    //正常情况下，这个2个区域相同，但是滑动时不同
    GUI_RECT_T      display_rect;       //display rect  显示时的区域，比如滑动拖动时，和layout_rect的值就会不同
    GUI_RECT_T      layout_rect;        //空间分配时的区域 rect ，判断TP的坐标是否在item范围内时，用display_rect

}MMIQBTHEME_DISPLAY_T;

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
