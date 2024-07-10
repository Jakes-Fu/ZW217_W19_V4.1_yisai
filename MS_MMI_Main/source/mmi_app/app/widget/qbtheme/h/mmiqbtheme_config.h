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

//widget ����dockbar�����ŵĴ�С
#define MMIQBTHEME_THUMB_WIDGET_THUMB_SIZE (MMI_MAINSCREEN_WIDTH / 4)

//max item num in dockbar page, not including special item
#define MMIQBTHEME_DOCKBAR_PAGE_MAX_ITEM (9)
#define MMIQBTHEME_DOCKBAR_PAGE_MAX_DISPLAY_ITEM (6)

#define MMIQBTHEME_MAX_DOCKBAR_PAGE_NUM 3

#define MMIQBTHEME_SLIDING_DELEY_TICKS (0) //20ms per frame


#if defined(MAINLCD_SIZE_320X480)
//widgetҳ���������
#define MMIQBTHEME_GRID_COL_NUM                    4
#define MMIQBTHEME_GRID_ROW_NUM                    4

#define MMIQBTHEME_DOCKBAR_PAGE2_MAX_ITEM   4

//page tilte height, area  displaying page1-9 icon
#define MMIQBTHEME_PAGE_TITLE_HEIGHT   18

#define MMIQBTHEME_DOCKBAR_HEIGHT   85
#define MMIQBTHEME_DOCKBAR_ITEM_HEIGHT   MMIQBTHEME_DOCKBAR_HEIGHT
#define MMIQBTHEME_DOCKBAR_ITEM_WIDTH  MMITHEME_MENU_ITEM_WIDTH   // MMI_MAINSCREEN_WIDTH/MMIQBTHEME_DOCKBAR_PAGE2_MAX_ITEM

//dockbar�в˵���ص�λ�ö���
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_HEIGHT  MMITHEME_MENU_ITEM_HEIGHT 
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_WIDTH   MMITHEME_MENU_ITEM_WIDTH
//�˵���icon�Ĵ�С
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_ICON_SIZE   MENUICON_HEIGHT
//�˵���text�Ĵ�С
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_TEXT_FONT  MMITHEME_MAINMENU_TEXT_FONT
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_TEXT_HEIGHT  MENUTEXT_HEIGHT 
//widget pageԤ����Ĵ�С����
#define MMIQBTHEME_DOCKBAR_PREVIEW_ITEM_WIDTH  48   // MMI_MAINSCREEN_WIDTH/MMIQBTHEME_DOCKBAR_PAGE_MAX_DISPLAY_ITEM
#define MMIQBTHEME_DOCKBAR_PREVIEW_ITEM_HEIGHT  72

#elif defined(MAINLCD_SIZE_240X320)
//widgetҳ���������
#define MMIQBTHEME_GRID_ROW_NUM            3
#define MMIQBTHEME_GRID_COL_NUM            3

#define MMIQBTHEME_DOCKBAR_PAGE2_MAX_ITEM  3

//page tilte height, area  displaying page1-9 icon
#define MMIQBTHEME_PAGE_TITLE_HEIGHT   16

#define MMIQBTHEME_DOCKBAR_HEIGHT         75     // 44 + 16
#define MMIQBTHEME_DOCKBAR_ITEM_HEIGHT   MMIQBTHEME_DOCKBAR_HEIGHT 
#define MMIQBTHEME_DOCKBAR_ITEM_WIDTH    MMITHEME_MENU_ITEM_WIDTH   // MMI_MAINSCREEN_WIDTH/MMIQBTHEME_DOCKBAR_PAGE2_MAX_ITEM

//dockbar�в˵���ص�λ�ö���
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_HEIGHT  MMITHEME_MENU_ITEM_HEIGHT 
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_WIDTH   MMITHEME_MENU_ITEM_WIDTH
//�˵���icon�Ĵ�С
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_ICON_SIZE   MENUICON_HEIGHT
//�˵���text�Ĵ�С
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_TEXT_FONT  MMITHEME_MAINMENU_TEXT_FONT
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_TEXT_HEIGHT  MENUTEXT_HEIGHT 


//widget pageԤ����Ĵ�С����
#define MMIQBTHEME_DOCKBAR_PREVIEW_ITEM_WIDTH   36  // MMI_MAINSCREEN_WIDTH/MMIQBTHEME_DOCKBAR_PAGE_MAX_DISPLAY_ITEM
#define MMIQBTHEME_DOCKBAR_PREVIEW_ITEM_HEIGHT  48  //36/240*320

#elif defined(MAINLCD_SIZE_240X400)
//widgetҳ���������
#define MMIQBTHEME_GRID_COL_NUM            3
#define MMIQBTHEME_GRID_ROW_NUM            3

#define MMIQBTHEME_DOCKBAR_PAGE2_MAX_ITEM  3

//page tilte height, area  displaying page1-9 icon
#define MMIQBTHEME_PAGE_TITLE_HEIGHT   20


#define MMIQBTHEME_DOCKBAR_HEIGHT        75     // 44 + 16
#define MMIQBTHEME_DOCKBAR_ITEM_HEIGHT   MMIQBTHEME_DOCKBAR_HEIGHT 
#define MMIQBTHEME_DOCKBAR_ITEM_WIDTH    MMITHEME_MENU_ITEM_WIDTH   // MMI_MAINSCREEN_WIDTH/MMIQBTHEME_DOCKBAR_PAGE2_MAX_ITEM

//dockbar�в˵���ص�λ�ö���
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_HEIGHT  MMITHEME_MENU_ITEM_HEIGHT 
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_WIDTH   MMITHEME_MENU_ITEM_WIDTH
//�˵���icon�Ĵ�С
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_ICON_SIZE   MENUICON_HEIGHT
//�˵���text�Ĵ�С
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_TEXT_FONT  MMITHEME_MAINMENU_TEXT_FONT
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_TEXT_HEIGHT  MENUTEXT_HEIGHT 


//widget pageԤ����Ĵ�С����
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

//dockbar�в˵���ص�λ�ö���
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_HEIGHT  MMITHEME_MENU_ITEM_HEIGHT 
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_WIDTH   MMITHEME_MENU_ITEM_WIDTH
//�˵���icon�Ĵ�С
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_ICON_SIZE   MENUICON_HEIGHT
//�˵���text�Ĵ�С
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_TEXT_FONT  MMITHEME_MAINMENU_TEXT_FONT
#define MMIQBTHEME_DOCKBAR_MENU_ITEM_TEXT_HEIGHT  MENUTEXT_HEIGHT 


//widget pageԤ����Ĵ�С����
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
    MMIQBTHEME_ITEM_MOVING = 4 ,     //��page_slideing״̬����ͬʱ���ڣ����Ը���1bit��ʾ1��״̬
}MMIQBTHEME_FSM_STATE_E;
*/
//ƽ̨��ʾ��صĽṹ����
typedef struct
{    
    MMI_WIN_ID_T      win_id;
    GUI_LCD_DEV_INFO lcd_dev_info; //lcd dev info  
    //��������£����2��������ͬ�����ǻ���ʱ��ͬ
    GUI_RECT_T      display_rect;       //display rect  ��ʾʱ�����򣬱��绬���϶�ʱ����layout_rect��ֵ�ͻ᲻ͬ
    GUI_RECT_T      layout_rect;        //�ռ����ʱ������ rect ���ж�TP�������Ƿ���item��Χ��ʱ����display_rect

}MMIQBTHEME_DISPLAY_T;

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
