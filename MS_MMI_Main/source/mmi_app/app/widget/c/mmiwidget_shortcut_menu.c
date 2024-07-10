/*****************************************************************************
** File Name:      mmiwidget_shortcut_menu.c                                        *
** Author:                                                                   *
** Date:           10/23/2009                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/23/2009        bin.ji              Creat
******************************************************************************/

#define _MMIWIDGET_SHORTCUT_MENU_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "std_header.h"

#ifdef MMI_WIDGET_SHORTCUT_MENU
#include "window_parse.h"
//#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiwidget_image.h"
#include "mmiwidget.h"
#include "mmiwidget_id.h"
#include "guires.h"
#include "mmitheme_widget.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiidle_export.h"
#include "mmi_appmsg.h"
#include "mmiwidget_position.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIWIDEGT_SHORTCUT_MENU_TOTAL           MMISET_IDLE_SHORTCUT_MAX_NUM /*5*/
#define MMIWIDGET_SHORTCUT_FIX_ITEMS            1
#define MMIWIDGET_SHORTCUT_ITEMS_PER_PAGE       3

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
// shortcut menu info
typedef struct 
{	
    MMISET_SHORTCUT_TYPE_E	shortcut_type;  
    GUI_RECT_T              tp_rect;	
}MMIWIDGET_SHORTCUT_MENU_ITEM_T;

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
//点中区域的标识
typedef enum
{
    MMIWIDGET_SHORTCUT_MENU_HIT_NONE,        
        MMIWIDGET_SHORTCUT_MENU_HIT_LAST_PAGE_ON,       
        MMIWIDGET_SHORTCUT_MENU_HIT_LAST_PAGE_OFF,    
        MMIWIDGET_SHORTCUT_MENU_HIT_NEXT_PAGE_ON,       
        MMIWIDGET_SHORTCUT_MENU_HIT_NEXT_PAGE_OFF,          
        MMIWIDGET_SHORTCUT_MENU_HIT_MAX
}MMIWIDGET_SHORTCUT_MENU_HIT_TYPE_E;
/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//记录当前处在g_shortcut_menu_all_array的索引
LOCAL uint16 s_shortcut_menu_current_page = 0;

//点中状态
LOCAL uint32 s_widget_shortcut_menu_hit_type = MMIWIDGET_SHORTCUT_MENU_HIT_NONE;

LOCAL MMIWIDGET_SHORTCUT_MENU_ITEM_T s_shortcut_menu_items_arr[MMIWIDGET_SHORTCUT_FIX_ITEMS \
                                                            + MMIWIDEGT_SHORTCUT_MENU_TOTAL] = {0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

LOCAL MMI_RESULT_E  HandleShortcutMenuTpPressDown(
                                                  MMI_WIN_ID_T        win_id,        //IN:
                                                  DPARAM            param        //IN:
                                                  );

LOCAL MMI_RESULT_E  HandleWidgetShortcutMenuWinMsg(
                                                   MMI_WIN_ID_T        win_id,        //IN:
                                                   MMI_MESSAGE_ID_E    msg_id,        //IN:
                                                   DPARAM            param        //IN:
                                                   );
LOCAL void MMIWIDGET_ShortcutMenu_DisplayShortcutMenu(MMI_WIN_ID_T     win_id);
LOCAL MMI_RESULT_E  HandleShortcutMenuTpPressUp(MMI_WIN_ID_T   win_id,  DPARAM    param );

LOCAL void ResetWidgetShortcut();
/**--------------------------------------------------------------------------*/

PUBLIC const MMIWIDGET_ITEM_INFO_T g_shortcut_menu_widget =
{
    MMIWIDGET_SHORTCUT_MENU_ID, 
        HandleWidgetShortcutMenuWinMsg, 
        PNULL,
        ResetWidgetShortcut,
		PNULL,        
        WIDGET_SHORTCUT_MENU_WIN_ID, 
        IMAGE_WIDGET_SHORTCUT_MENU_SHORTCUT, 
        IMAGE_WIDGET_SHORTCUT_MENU_BG,
        TXT_WIDGET_SHORTCUT_MENU,
        50,//    75, 
        50,//    50
        FALSE,
};

LOCAL void ResetWidgetShortcut()
{
    if(MMIWIDGET_SHORTCUT_MENU_HIT_NEXT_PAGE_ON == s_widget_shortcut_menu_hit_type )
        s_widget_shortcut_menu_hit_type = MMIWIDGET_SHORTCUT_MENU_HIT_NEXT_PAGE_OFF;
    else if(MMIWIDGET_SHORTCUT_MENU_HIT_LAST_PAGE_ON == s_widget_shortcut_menu_hit_type)
        s_widget_shortcut_menu_hit_type = MMIWIDGET_SHORTCUT_MENU_HIT_LAST_PAGE_OFF;
}

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

LOCAL void UpdateShortcutMenuItems()
{
    int i = 0;
    MMISET_IDLE_SHORTCUT_T      shortcut_items = {0};/*lint !e64*/
    
    shortcut_items = MMIAPISET_GetIdleShortcut();
    
    s_shortcut_menu_items_arr[0].shortcut_type = MMISET_SHORTCUT_MAINMENU;
    
    for (i = 0; i < MMIWIDEGT_SHORTCUT_MENU_TOTAL; ++i)
    {
        s_shortcut_menu_items_arr[MMIWIDGET_SHORTCUT_FIX_ITEMS + i].shortcut_type = \
            shortcut_items.idle_shortcut[i];
    }
}

LOCAL MMI_RESULT_E  HandleWidgetShortcutMenuWinMsg(
                                                   MMI_WIN_ID_T        win_id,       
                                                   MMI_MESSAGE_ID_E    msg_id,     
                                                   DPARAM            param       
                                                   )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;	
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {     
    case MSG_OPEN_WINDOW:
    case MSG_GET_FOCUS:	
        UpdateShortcutMenuItems();
        break;
        
    case MSG_LCD_SWITCH:
        ResetWidgetShortcut();
        break;
        
    case MSG_WIDGET_TP_PRESS_DOWN:
        recode =HandleShortcutMenuTpPressDown(win_id,param);
        break;
    case MSG_WIDGET_TP_PRESS_UP:
        recode = HandleShortcutMenuTpPressUp(win_id,param);		  	 
        break;
        
    case MSG_FULL_PAINT:			
        MMIWIDGET_ShortcutMenu_DisplayShortcutMenu(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    MMI_CheckAllocatedMemInfo();
    return recode;        
}

LOCAL void MMIWIDGET_ShortcutMenu_DisplayShortcutMenu(MMI_WIN_ID_T     win_id)
{
    GUI_RECT_T win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T page_rect = MMIWIDGET_SHORTCUT_MENU_PAGE_BUTTON;
    GUI_POINT_T     point={0};
    uint8  i=0;	
    uint16 image_width=0;
    uint16 image_height=0;	
    MMI_IMAGE_ID_T img_id = 0;
    MMISET_SHORTCUT_TYPE_E shortcut_type = MMISET_SHORTCUT_MAX;
    uint16 cur_start_index = 0;	
    
    MMK_GetWinRect(win_id, &win_rect);	
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);
    if (UILAYER_IsLayerActive(&lcd_dev_info))
    {
        //显示背景图
        point.x = win_rect.left;
        point.y = win_rect.top;
        
        GUIRES_DisplayImg(
            &point,
            PNULL,
            PNULL,
            win_id,
            IMAGE_WIDGET_SHORTCUT_MENU_BG,
            &lcd_dev_info
            );
        point.x  += MMIWIDGET_SHORTCUT_MENU_ICON_START_X;
        point.y  += MMIWIDGET_SHORTCUT_MENU_ICON_START_Y;
        
        for (i = 0; i< MMIWIDGET_SHORTCUT_ITEMS_PER_PAGE; ++i)
        {
            cur_start_index = s_shortcut_menu_current_page * MMIWIDGET_SHORTCUT_ITEMS_PER_PAGE + i;
            shortcut_type = s_shortcut_menu_items_arr[cur_start_index].shortcut_type;

#if defined MMI_SMART_IDLE_SUPPORT_LARGE_ICON || defined MMI_SMART_IDLE_SUPPORT_SMALL_ICON
            img_id = MMIAPIIDLE_GetShortcutIcon(shortcut_type);
#else
            img_id = 0;
#endif
            
            GUIRES_GetImgWidthHeight(&image_width, &image_height,img_id, win_id);
            GUIRES_DisplayImg(&point, PNULL, PNULL, win_id, img_id, &lcd_dev_info);
            
            s_shortcut_menu_items_arr[cur_start_index].tp_rect.left = point.x;
            s_shortcut_menu_items_arr[cur_start_index].tp_rect.top = point.y;
            s_shortcut_menu_items_arr[cur_start_index].tp_rect.right = point.x + image_width - 1;
            s_shortcut_menu_items_arr[cur_start_index].tp_rect.bottom = point.y + image_height - 1;
            
            s_shortcut_menu_items_arr[cur_start_index].tp_rect = MMI_ConvertWinRect(MMICOM_WINPOS_DISP2WIN,\
                win_id, s_shortcut_menu_items_arr[cur_start_index].tp_rect);
            
            
            point.x += (image_width + MMIWIDGET_SHORTCUT_MENU_ICON_INTERVAL) ;
        }
        
        page_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, page_rect);
        switch (s_widget_shortcut_menu_hit_type)
        {
        case MMIWIDGET_SHORTCUT_MENU_HIT_NEXT_PAGE_ON:
            GUIRES_DisplayImg(
                PNULL,
                &page_rect,
                PNULL,
                win_id,
                IMAGE_WIDGET_SHORTCUT_MENU_NEXT_PAGE_ON,
                &lcd_dev_info
                );
            break;
        case MMIWIDGET_SHORTCUT_MENU_HIT_LAST_PAGE_ON:
            GUIRES_DisplayImg(
                PNULL,
                &page_rect,
                PNULL,
                win_id,
                IMAGE_WIDGET_SHORTCUT_MENU_LAST_PAGE_ON,
                &lcd_dev_info
                );
            break;
        case MMIWIDGET_SHORTCUT_MENU_HIT_LAST_PAGE_OFF:
            GUIRES_DisplayImg(
                PNULL,
                &page_rect,
                PNULL,
                win_id,
                IMAGE_WIDGET_SHORTCUT_MENU_LAST_PAGE_OFF,
                &lcd_dev_info
                );
            break;	
        case MMIWIDGET_SHORTCUT_MENU_HIT_NEXT_PAGE_OFF:
            GUIRES_DisplayImg(
                PNULL,
                &page_rect,
                PNULL,
                win_id,
                IMAGE_WIDGET_SHORTCUT_MENU_NEXT_PAGE_OFF,
                &lcd_dev_info
                );
            break;				
        default:
            break;			
        }		
    }
}

LOCAL MMI_RESULT_E  HandleShortcutMenuTpPressDown( MMI_WIN_ID_T win_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;	
    GUI_POINT_T point = {0};
    GUI_RECT_T page_rect = MMIWIDGET_SHORTCUT_MENU_PAGE_BUTTON;
    MMI_CheckAllocatedMemInfo();
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);
    
    if (GUI_PointIsInRect(point, page_rect))
    {
        if(0 == s_shortcut_menu_current_page)			  
            s_widget_shortcut_menu_hit_type = MMIWIDGET_SHORTCUT_MENU_HIT_NEXT_PAGE_ON;
        else if(1 == s_shortcut_menu_current_page)	 	
            s_widget_shortcut_menu_hit_type = MMIWIDGET_SHORTCUT_MENU_HIT_LAST_PAGE_ON;
    }
    else
    {
        recode = MMI_RESULT_FALSE;
    }	
    MMI_CheckAllocatedMemInfo();
    return recode;
}

LOCAL MMI_RESULT_E  HandleShortcutMenuTpPressUp(  MMI_WIN_ID_T   win_id,  DPARAM    param )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};
    uint8 i = 0;       
    uint16 cur_start_index = 0;
    MMISET_SHORTCUT_TYPE_E shortcut_type = MMISET_SHORTCUT_MAX;
    uint8 page_count = 0;
    GUI_RECT_T page_rect = MMIWIDGET_SHORTCUT_MENU_PAGE_BUTTON;

/*    
    if(0 == (MMIWIDEGT_SHORTCUT_MENU_TOTAL + MMIWIDGET_SHORTCUT_FIX_ITEMS) % MMIWIDGET_SHORTCUT_ITEMS_PER_PAGE) 
    {
        page_count = (MMIWIDEGT_SHORTCUT_MENU_TOTAL + MMIWIDGET_SHORTCUT_FIX_ITEMS) / MMIWIDGET_SHORTCUT_ITEMS_PER_PAGE;
    }
    else
    {
        page_count = (MMIWIDEGT_SHORTCUT_MENU_TOTAL + MMIWIDGET_SHORTCUT_FIX_ITEMS) / MMIWIDGET_SHORTCUT_ITEMS_PER_PAGE + 1;
    }
*/
    page_count = ((MMIWIDEGT_SHORTCUT_MENU_TOTAL + MMIWIDGET_SHORTCUT_FIX_ITEMS) + (MMIWIDGET_SHORTCUT_ITEMS_PER_PAGE-1)) / MMIWIDGET_SHORTCUT_ITEMS_PER_PAGE;
    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    
    point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);
    if(GUI_PointIsInRect(point, page_rect))
    {
        ++s_shortcut_menu_current_page;
        s_shortcut_menu_current_page %= page_count; 
        if(0 == s_shortcut_menu_current_page )      
            s_widget_shortcut_menu_hit_type = MMIWIDGET_SHORTCUT_MENU_HIT_NEXT_PAGE_OFF;
        else if(1 == s_shortcut_menu_current_page)	 	
            s_widget_shortcut_menu_hit_type = MMIWIDGET_SHORTCUT_MENU_HIT_LAST_PAGE_OFF;		
    }
    else
    {
        //当点击在翻页的区域，而拖动到其他的地方释放时
        if(MMIWIDGET_SHORTCUT_MENU_HIT_NEXT_PAGE_ON == s_widget_shortcut_menu_hit_type )
            s_widget_shortcut_menu_hit_type = MMIWIDGET_SHORTCUT_MENU_HIT_NEXT_PAGE_OFF;
        else if(MMIWIDGET_SHORTCUT_MENU_HIT_LAST_PAGE_ON == s_widget_shortcut_menu_hit_type)
            s_widget_shortcut_menu_hit_type = MMIWIDGET_SHORTCUT_MENU_HIT_LAST_PAGE_OFF;
        
        for (i = 0; i< MMIWIDGET_SHORTCUT_ITEMS_PER_PAGE; ++i)
        {
            cur_start_index = s_shortcut_menu_current_page * MMIWIDGET_SHORTCUT_ITEMS_PER_PAGE + i;
            
            if(GUI_PointIsInRect(point, s_shortcut_menu_items_arr[cur_start_index].tp_rect))
            {
                shortcut_type = s_shortcut_menu_items_arr[cur_start_index].shortcut_type;
                MMIAPISET_OpenSelectedShortcutWin(MMK_GetParentWinHandle(win_id), shortcut_type);
                break;
            }
        }
    }
    
    return recode;
}
#endif

