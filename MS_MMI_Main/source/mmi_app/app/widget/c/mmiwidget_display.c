/*****************************************************************************
** File Name:      mmiwidget_display.c                                        *
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

#define _MMIWIDGET_DISPLAY_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"

#if (defined MMIWIDGET_SUPPORT)

#include "mmk_kbd.h"
#include "guicommon.h"
#include "mmidisplay_data.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "mmi_theme.h"
#include "mmiwidget.h"
#include "mmi_nv.h"
#include "mmiwidget_nv.h"
#include "mmiwidget_image.h"
#include "mmiwidget_menutable.h"
#include "guires.h"
#include "mmi_modu_main.h"
#include "mmi_id.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
//#include "mmiset_display.h"
//#include "mmi_wallpaper_export.h"
//#include "guictrl_api.h"


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIWIDGET_HOR_PRE_NETWORK_Y     47
#define MMIWIDGET_HOR_PRE_NETWORK_X_GAP 6

#define MMIWIDGET_VER_PRE_NETWORK_Y     50
#define MMIWIDGET_VER_PRE_NETWORK_X_GAP 5

#define MMIWIDGET_HOR_PRE_CLOCK_Y       47
#define MMIWIDGET_VER_PRE_CLOCK_Y_GAP   20
#define MMIWIDGET_HOR_PRE_CLOCK_X       35

#define MMIWIDGET_VER_PRE_PB_Y          50    
#define MMIWIDGET_HOR_PRE_PB_Y          30

#define MMIWIDGET_PRELOAD_PAGE_0        0
#define MMIWIDGET_PRELOAD_PAGE_1        1
#define MMIWIDGET_PRELOAD_PAGE_2        2

#define MMIWIDGET_DEFAULT_PAGE_INDEX    MMIWIDGET_PRELOAD_PAGE_1
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

extern const MMIWIDGET_ITEM_INFO_T g_mp3_widget;
extern const MMIWIDGET_ITEM_INFO_T g_pb_widget;
#ifndef MMI_WIDGET_MEMO_LOW_MEMORY
extern const MMIWIDGET_ITEM_INFO_T g_memo_widget;
#endif
#ifndef MMI_WIDGET_SHORTCUT_MENU_LOW_MEMORY
extern const MMIWIDGET_ITEM_INFO_T g_shortcut_menu_widget;
#endif
extern const MMIWIDGET_ITEM_INFO_T g_calendar_widget;
#ifdef SCH_ASSISTANT_WIDGET
extern const MMIWIDGET_ITEM_INFO_T g_schedule_widget;
#endif
#ifndef MMI_WIDGET_NETWORK_LOW_MEMORY
extern const MMIWIDGET_ITEM_INFO_T g_network_widget;
#endif

#ifdef PDA_TIME_ANIM_SUPPORT
extern const MMIWIDGET_ITEM_INFO_T g_time_widget;
#else
#ifdef MMI_WIDGET_DUAL_CLOCK
extern const MMIWIDGET_ITEM_INFO_T g_clock_widget;
#endif
#endif

#ifdef SNS_SUPPORT
extern const MMIWIDGET_ITEM_INFO_T g_sns_widget;
#endif

extern const MMIWIDGET_ITEM_INFO_T g_custom_word_widget;
extern const MMIWIDGET_ITEM_INFO_T g_newsms_widget;
#if defined(FM_SUPPORT)
#ifdef MMI_WIDGET_FM
extern const MMIWIDGET_ITEM_INFO_T g_fm_widget;
#endif
#endif
extern const MMIWIDGET_ITEM_INFO_T g_calllog_widget;
#ifdef ASP_SUPPORT
extern const MMIWIDGET_ITEM_INFO_T g_autoanswer_log_widget;
#endif
extern const MMIWIDGET_ITEM_INFO_T g_alarm_widget;
#ifndef MMI_WIDGET_SHORTCUT_SET_LOW_MEMORY
extern const MMIWIDGET_ITEM_INFO_T g_shortcut_set_widget;
#endif
extern const MMIWIDGET_ITEM_INFO_T g_elve_widget;
extern const MMIWIDGET_ITEM_INFO_T g_aclock_widget;
extern const MMIWIDGET_ITEM_INFO_T g_search_widget;
#ifdef MMI_WIDGET_WEATHER1
extern MMIWIDGET_ITEM_INFO_T       g_weather_widget;
#endif
#ifdef MMI_WIDGET_WEATHER2 
extern MMIWIDGET_ITEM_INFO_T       g_weather_widget2;
#endif

#ifndef MMI_WIDGET_DUALCLOCK_LOW_MEMORY
extern const MMIWIDGET_ITEM_INFO_T g_dual_clock_widget;
#endif
#ifndef MMI_WIDGET_DUAL_ACLOCK_LOW_MEMORY
extern const MMIWIDGET_ITEM_INFO_T g_dual_aclock_widget;
#endif
#if defined(MMI_ENABLE_DCD)
extern const MMIWIDGET_ITEM_INFO_T g_dcd_widget;
#endif

//所有widget应用的表
LOCAL const MMIWIDGET_ITEM_INFO_T *s_all_widget_table[] =
{
#ifdef MMI_WIDGET_WEATHER1
    &g_weather_widget,
#endif
#ifdef MMI_WIDGET_WEATHER2 
    &g_weather_widget2;
#endif
#ifdef SNS_SUPPORT    
    &g_sns_widget
#endif 
    &g_mp3_widget,
    &g_pb_widget,
#ifndef MMI_WIDGET_MEMO_LOW_MEMORY    
    &g_memo_widget,
#endif
#ifndef MMI_WIDGET_SHORTCUT_MENU_LOW_MEMORY    
    &g_shortcut_menu_widget,
    &g_calendar_widget,
#ifdef SCH_ASSISTANT_WIDGET
    &g_schedule_widget,
#endif
#endif
#ifndef MMI_WIDGET_NETWORK_LOW_MEMORY
    &g_network_widget,
#endif

#ifdef PDA_TIME_ANIM_SUPPORT
    &g_time_widget,
#else
#ifdef MMI_WIDGET_DUAL_CLOCK
    &g_clock_widget,
    #endif
#endif

    &g_custom_word_widget,
    &g_newsms_widget,
#if defined(FM_SUPPORT)
#ifdef MMI_WIDGET_FM
    &g_fm_widget,
#endif
#endif
    &g_calllog_widget,
#ifdef ASP_SUPPORT    
    &g_autoanswer_log_widget,
#endif    
    &g_alarm_widget,
#ifndef MMI_WIDGET_SHORTCUT_SET_LOW_MEMORY
    &g_shortcut_set_widget,
#endif
    &g_aclock_widget,    
    &g_elve_widget,
    &g_search_widget,
#if defined(MMI_ENABLE_DCD)
    &g_dcd_widget,
#endif
#ifndef MMI_WIDGET_DUALCLOCK_LOW_MEMORY
    &g_dual_clock_widget,
#endif
#ifndef MMI_WIDGET_DUAL_ACLOCK_LOW_MEMORY
    &g_dual_aclock_widget
#endif
};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : creat idle widget
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void DestroyIdleWidget(void);

/*****************************************************************************/
//  Description : creat idle widget
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void CreateIdleWidget(void);

/*****************************************************************************/
//  Description : callback save info
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
//LOCAL void CallbackSaveInfo(
//                            MMIWIDGET_SAVE_INFO_T *save_info_ptr
//                            );

/*****************************************************************************/
//  Description : load all widget to control
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void LoadAllWidget(
                         MMIWIDGET_CTRL_E  ctrl_id,
                         MMIWIDGET_SAVE_INFO_T *save_info_ptr
                         );

/*****************************************************************************/
//  Description : preload widget on idle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void InitialWidgets(MMIWIDGET_SAVE_INFO_T *save_info_ptr);

/*****************************************************************************/
//  Description : preload pb widget on idle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void PreloadWidgetBar(MMIWIDGET_SAVE_INFO_T *save_info_ptr, MMIWIDGET_ITEM_INFO_T *item_ptr);

/*****************************************************************************/
//  Description : preload pb widget on idle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void PreloadNetworkOnIdle(MMIWIDGET_SAVE_INFO_T *save_info_ptr, MMIWIDGET_ITEM_INFO_T *item_ptr);

/*****************************************************************************/
//  Description : preload pb widget on idle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void PreloadClockOnIdle(MMIWIDGET_SAVE_INFO_T *save_info_ptr, MMIWIDGET_ITEM_INFO_T *item_ptr);

/*****************************************************************************/
//  Description : preload pb widget on idle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void PreloadAlarmOnIdle(MMIWIDGET_SAVE_INFO_T *save_info_ptr, MMIWIDGET_ITEM_INFO_T *item_ptr);

/*****************************************************************************/
//  Description : preload pb widget on idle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void PreloadPbOnIdle(MMIWIDGET_SAVE_INFO_T *save_info_ptr, MMIWIDGET_ITEM_INFO_T *item_ptr);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create idle widget
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWIDGET_CreateIdleWidget(void)
{
    if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle())
    {
        CreateIdleWidget();

        if(MMIAPISET_IsDisplayIdleWords())
    	{		
        	MMIAPIWIDGET_AddDynamicItem(MMIWIDGET_CTRL_IDLE_ID, MMIWIDGET_CUSTOM_WORD_ID, FALSE, FALSE);    		
    	}
    	else
    	{
    		MMIAPIWIDGET_RemoveDynamicItem(MMIWIDGET_CTRL_IDLE_ID,MMIWIDGET_CUSTOM_WORD_ID);
    	}

 #ifdef MMI_ENABLE_DCD
		if(MMIAPIDCD_DcdIsOpen())
		{
			MMIAPIWIDGET_AddDynamicItem(MMIWIDGET_CTRL_IDLE_ID, MMIWIDGET_DCD_ID, FALSE, FALSE);
		}
		else
		{
			MMIAPIWIDGET_RemoveDynamicItem(MMIWIDGET_CTRL_IDLE_ID, MMIWIDGET_DCD_ID);
		} 
#endif		
    }
}

/*****************************************************************************/
//  Description : destroy idle widget
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWIDGET_DestroyIdleWidget(void)
{
    if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle())
    {
        DestroyIdleWidget();
    }
}

/*****************************************************************************/
//  Description : find widget item info
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC MMIWIDGET_ITEM_INFO_T * MMIWIDGET_FindWidgetItemInfo(
                                          uint32 widget_id
                                          )
{
    uint32 i = 0;

    for (i = 0; i < ARR_SIZE(s_all_widget_table); i++)
    {
        if ((PNULL != s_all_widget_table[i]) && (widget_id == s_all_widget_table[i]->widget_id))
        {
            return s_all_widget_table[i]; /*lint !e605*/           
        }
    }
  
    return PNULL;
}

/*****************************************************************************/
//  Description : creat idle widget
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void DestroyIdleWidget(void)
{
    MMIWIDGET_Destroy(MMIWIDGET_CTRL_IDLE_ID);
}

/*****************************************************************************/
//  Description : creat idle widget
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void CreateIdleWidget(void)
{
    MMIWIDGET_CTRL_E  ctrl_id = MMIWIDGET_CTRL_IDLE_ID;
    MMIWIDGET_SAVE_INFO_T save_info = {0};
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetFullScreenBothRect();
    BOOLEAN result = FALSE;

    result = MMIWIDGET_Create(MAIN_IDLE_WIN_ID, MMIWIDGET_CTRL_IDLE_ID, &both_rect);
    if (result)
    {
        //读NV
        MMIWIDGET_ReadWidgetNV(&save_info);

        //初始化widget shortcut数据
        MMIWIDGET_SetMaxItemNum(ctrl_id, MMIWIDGET_MAX_ITEM_NUM);
        //MMIWIDGET_SetCallbackSaveFunc(ctrl_id, CallbackSaveInfo);

        LoadAllWidget(ctrl_id, &save_info);

        //update and save current nvram.
        MMIWIDGET_SaveWidgetInfoToNV(MMIAPIWIDGET_GetWidgetPtr(ctrl_id));    
//    MMK_DelayUpdateScreen();
//    MMK_UpdateScreen();
    }
}

/*****************************************************************************/
//  Description : load all widget to control
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void LoadAllWidget(
                         MMIWIDGET_CTRL_E  ctrl_id,
                         MMIWIDGET_SAVE_INFO_T *save_info_ptr
                         )
{
    MMIWIDGET_ITEM_T widget_item = {0};    
    uint32 i = 0;
    uint32 widget_id = MMIWIDGET_INVALID_ID;
    MMIWIDGET_ITEM_INFO_T *item_ptr = PNULL;
    MMIWIDGET_CTRL_T *widget_ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(ctrl_id);
    
    if (PNULL == widget_ctrl_ptr)
    {
        return;
    }

    widget_ctrl_ptr->zorder_item_ptr = (MMIWIDGET_ITEM_T*)SCI_ALLOC_APP(sizeof(MMIWIDGET_ITEM_T));

    if (PNULL == widget_ctrl_ptr->zorder_item_ptr)
    {
        return;
    }
    
    SCI_MEMSET(widget_ctrl_ptr->item_ptr, 0x00, sizeof(MMIWIDGET_ITEM_T));
    widget_ctrl_ptr->zorder_item_ptr->zorder_prev_ptr = PNULL;
    widget_ctrl_ptr->zorder_item_ptr->zorder_next_ptr = PNULL;
    MMIWIDGET_SetCurPageIndex(ctrl_id, save_info_ptr->cur_page_index);
    //添加shortcut中的widget
    for (i = 0; i < save_info_ptr->widget_shortcut_list.item_num; i++)
    {
        widget_id = save_info_ptr->widget_shortcut_list.item_info[i].widget_id;        
        item_ptr = MMIWIDGET_FindWidgetItemInfo(widget_id);
        if (PNULL != item_ptr)
        {
//            SetWidgetItemData(&widget_item, item_ptr);
            widget_item.item_info = *item_ptr;
            widget_item.is_active = FALSE;
            widget_item.zorder_next_ptr = PNULL;
            widget_item.zorder_prev_ptr = PNULL;            
            MMIWIDGET_AppendItem(ctrl_id, &widget_item);
        }
    }
    for (i = 0; i < save_info_ptr->widget_active_list.item_num; i++)
    {
        widget_id = save_info_ptr->widget_active_list.item_info[i].widget_id;
        item_ptr = MMIWIDGET_FindWidgetItemInfo(widget_id);
        if (PNULL != item_ptr)
        {
            widget_item.item_info = *item_ptr;
            widget_item.is_active = TRUE;
            widget_item.zorder_next_ptr = PNULL;
            widget_item.zorder_prev_ptr = PNULL;             
            widget_item.item_info.start_pos = save_info_ptr->widget_active_list.item_info[i].save_pos;
            MMIWIDGET_AppendItem(ctrl_id, &widget_item);
        }
    }
    
    MMIWIDGET_SetShortcutOffset(ctrl_id, save_info_ptr->shortcut_offset);    
}

/*****************************************************************************/
//  Description : preload pb widget on idle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void PreloadPbOnIdle(MMIWIDGET_SAVE_INFO_T *save_info_ptr, MMIWIDGET_ITEM_INFO_T *item_ptr)
{
    uint16 width  = 0;
    uint16 height = 0;
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();  
    uint16 lcd_width  = rect.right  - rect.left + 1; 
    MMIWIDGET_CTRL_T *ctrl_ptr = PNULL;

    ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(MMIWIDGET_CTRL_IDLE_ID);

    if (PNULL == ctrl_ptr)
    {
        return;
    }
    
    save_info_ptr->widget_active_list.item_info[1].widget_id  = MMIWIDGET_PB_ID;

    GUIRES_GetImgWidthHeight(&width, &height, item_ptr->large_bg_img, MMK_GetFocusWinId());

    save_info_ptr->widget_active_list.item_info[1].save_pos.x = lcd_width * (MMIWIDGET_PRELOAD_PAGE_0 - MMIWIDGET_DEFAULT_PAGE_INDEX) + (lcd_width - width)/2;    

    if (MMK_IsWindowLandscape(ctrl_ptr->win_id))
    {
        save_info_ptr->widget_active_list.item_info[1].save_pos.y = MMIWIDGET_HOR_PRE_PB_Y;
    }
    else
    {
        save_info_ptr->widget_active_list.item_info[1].save_pos.y = MMIWIDGET_VER_PRE_PB_Y;
    }

    save_info_ptr->widget_active_list.item_num++;    
}

/*****************************************************************************/
//  Description : preload pb widget on idle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void PreloadAlarmOnIdle(MMIWIDGET_SAVE_INFO_T *save_info_ptr, MMIWIDGET_ITEM_INFO_T *item_ptr)
{
    uint16 width  = 0;
    uint16 height = 0;
    GUI_RECT_T rect   = MMITHEME_GetFullScreenRect();
    uint16 lcd_width  = rect.right  - rect.left + 1;
    uint16 lcd_height = rect.bottom - rect.top + 1;

    save_info_ptr->widget_active_list.item_info[3].widget_id  = MMIWIDGET_ALARM_ID;

    GUIRES_GetImgWidthHeight(&width, &height, item_ptr->large_bg_img, MMK_GetFocusWinId());

    save_info_ptr->widget_active_list.item_info[3].save_pos.x = lcd_width * (MMIWIDGET_PRELOAD_PAGE_2 - MMIWIDGET_DEFAULT_PAGE_INDEX) + (lcd_width - width)/2;    
    save_info_ptr->widget_active_list.item_info[3].save_pos.y = (lcd_height - height)/2;

    save_info_ptr->widget_active_list.item_num++;       
}

/*****************************************************************************/
//  Description : preload pb widget on idle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void PreloadClockOnIdle(MMIWIDGET_SAVE_INFO_T *save_info_ptr, MMIWIDGET_ITEM_INFO_T *item_ptr)
{
    uint32 img_id = 0;
    uint16 width  = 0;
    uint16 height = 0;
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();    
    uint16 lcd_width  = rect.right  - rect.left + 1; 
    MMIWIDGET_CTRL_T *ctrl_ptr = PNULL;

    ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(MMIWIDGET_CTRL_IDLE_ID);

    if (PNULL == ctrl_ptr)
    {
        return;
    }

    save_info_ptr->widget_active_list.item_info[0].widget_id  = MMIWIDGET_CLOCK_ID;

#if defined(MMI_MULTI_SIM_SYS_TRI)
    img_id = IMAGE_WIDGET_NETWORK_NAME_BG_TRI;
#elif defined(MMI_MULTI_SIM_SYS_QUAD)
    img_id = IMAGE_WIDGET_NETWORK_NAME_BG_QUAD;
#elif defined(MMI_MULTI_SIM_SYS_SINGLE) 
    img_id = IMAGE_WIDGET_NETWORK_NAME_BG_SINGLE;
#else 
    img_id = IMAGE_WIDGET_NETWORK_NAME_BG_DUAL;
#endif

    GUIRES_GetImgWidthHeight(&width, &height, img_id, MMK_GetFocusWinId());

    if (MMK_IsWindowLandscape(ctrl_ptr->win_id))
    {
        save_info_ptr->widget_active_list.item_info[0].save_pos.x = MMIWIDGET_HOR_PRE_CLOCK_X;       
        save_info_ptr->widget_active_list.item_info[0].save_pos.y = MMIWIDGET_HOR_PRE_CLOCK_Y;
    }
    else 
    {
        save_info_ptr->widget_active_list.item_info[0].save_pos.y = MMIWIDGET_VER_PRE_NETWORK_Y + height + MMIWIDGET_VER_PRE_CLOCK_Y_GAP;
        GUIRES_GetImgWidthHeight(&width, &height, item_ptr->large_bg_img, MMK_GetFocusWinId());
        save_info_ptr->widget_active_list.item_info[0].save_pos.x = (lcd_width - width)/2;       
    }   

    save_info_ptr->widget_active_list.item_num++;       
}

/*****************************************************************************/
//  Description : preload pb widget on idle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void PreloadNetworkOnIdle(MMIWIDGET_SAVE_INFO_T *save_info_ptr, MMIWIDGET_ITEM_INFO_T *item_ptr)
{
    uint16 width  = 0;
    uint16 height = 0;
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();    
    uint16 lcd_width  = rect.right  - rect.left + 1;   

    save_info_ptr->widget_active_list.item_info[2].widget_id  = MMIWIDGET_NETWORK_NAME_ID;

    GUIRES_GetImgWidthHeight(&width, &height, item_ptr->large_bg_img, MMK_GetFocusWinId());

    if (MMITHEME_IsMainScreenLandscape())
    {
        save_info_ptr->widget_active_list.item_info[2].save_pos.x = lcd_width - MMIWIDGET_HOR_PRE_NETWORK_X_GAP - width;       
        save_info_ptr->widget_active_list.item_info[2].save_pos.y = MMIWIDGET_HOR_PRE_NETWORK_Y;
    }
    else
    {
        save_info_ptr->widget_active_list.item_info[2].save_pos.y = MMIWIDGET_VER_PRE_NETWORK_Y;
        save_info_ptr->widget_active_list.item_info[2].save_pos.x = lcd_width - MMIWIDGET_VER_PRE_NETWORK_X_GAP - width;       
    }

    save_info_ptr->widget_active_list.item_num++;       
}

/*****************************************************************************/
//  Description : preload pb widget on idle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void PreloadWidgetBar(MMIWIDGET_SAVE_INFO_T *save_info_ptr, MMIWIDGET_ITEM_INFO_T *item_ptr)
{
    uint32 index = save_info_ptr->widget_shortcut_list.item_num;
    uint32 widget_id = item_ptr->widget_id;
    
    if (   PNULL != item_ptr
        && (!item_ptr->is_dynamic 
        || (item_ptr->is_dynamic 
        && item_ptr->DisplayWidgetCondition())))
    {
        save_info_ptr->widget_shortcut_list.item_info[index].widget_id = widget_id;
        save_info_ptr->widget_shortcut_list.item_info[index].save_pos = item_ptr->start_pos;
        //save_info_ptr->widget_shortcut_list.item_info[index].save_pos.y = 0;
        index++;
    } 
    save_info_ptr->widget_shortcut_list.item_num = index;
}

/*****************************************************************************/
//  Description : preload widget on idle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void InitialWidgets(MMIWIDGET_SAVE_INFO_T *save_info_ptr)
{
    uint32 i = 0;
    uint32 widget_id = 0;
    MMIWIDGET_ITEM_INFO_T *item_ptr = PNULL;
    MMIWIDGET_CTRL_T *ctrl_ptr = PNULL;

    if (PNULL == save_info_ptr)
    {
        return;
    }

    ctrl_ptr = MMIAPIWIDGET_GetWidgetPtr(MMIWIDGET_CTRL_IDLE_ID);

    if (PNULL == ctrl_ptr)
    {
        return;
    }
    
    save_info_ptr->shortcut_offset = 0;  
    save_info_ptr->widget_shortcut_list.item_num = 0;
    save_info_ptr->cur_page_index = MMIWIDGET_DEFAULT_PAGE_INDEX;

    for (i = 0; i < ARR_SIZE(s_all_widget_table); i++)
    {
        item_ptr = s_all_widget_table[i]; /*lint !e605*/

        if (PNULL == item_ptr)
        {
            continue;
        }

        widget_id = item_ptr->widget_id;
        
        switch(widget_id)
        {
        case MMIWIDGET_PB_ID:
            PreloadPbOnIdle(save_info_ptr, item_ptr);
            break;

        case MMIWIDGET_ALARM_ID:
            PreloadAlarmOnIdle(save_info_ptr, item_ptr);
            break;

        case MMIWIDGET_CLOCK_ID:
            PreloadClockOnIdle(save_info_ptr, item_ptr);
            break;

        case MMIWIDGET_NETWORK_NAME_ID:
            PreloadNetworkOnIdle(save_info_ptr, item_ptr);
            break;   

        default:
            PreloadWidgetBar(save_info_ptr, item_ptr);
            break;
        }
    }    
}

/*****************************************************************************/
//  Description : read nv
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWIDGET_ReadWidgetNV(
                        MMIWIDGET_SAVE_INFO_T *save_info_ptr
                        )
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    if (PNULL != save_info_ptr)
    {
        MMINV_READ(MMIWIDGET_SAVE_INFO, save_info_ptr, return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            InitialWidgets(save_info_ptr);
            MMINV_WRITE(MMIWIDGET_SAVE_INFO, save_info_ptr);        
        }
    }
}

/*****************************************************************************/
//  Description : creat wallpaper anim control and set parameter
//  Global resource dependence : 
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWIDGET_SetWallpaperLcdDevInfo(
                                      GUI_LCD_DEV_INFO *lcd_dev_info_ptr
                                      )
{
    // uint16 page_index = 0;

    MMK_SetCtrlLcdDevInfo(MMIIDLE_WALLPAPER_ANIM_CTRL_ID, lcd_dev_info_ptr);
}

#endif  //MMIWIDGET_SUPPORT
