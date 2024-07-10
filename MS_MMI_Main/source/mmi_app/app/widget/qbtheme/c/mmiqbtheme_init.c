/*****************************************************************************
** File Name:      mmiqbtheme_init.c                                         *
** Author:                                                                   *
** Date:           08/10/2011                                                *
** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to init widget page                     *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/23/2009     ying.xu              Creat
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_widget_trc.h"
#include "mmk_kbd.h"
#include "guicommon.h"
#include "mmidisplay_data.h"
#include "mmi_theme.h"
#include "mmiwidget.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_kbd.h"
#include "guilcd.h"
#include "window_parse.h"
#include "guimenu.h"
#include "mmitheme_menu.h"
#include "mmitheme_mainmenu.h"
#include "mmiwidget_export.h"
#include "mmi_mainmenu_export.h"
#include "mmi_menutable.h"
#include "mmiwidget_nv.h"
#include "mmiwidget_internal.h"

#include "mmiwidget_item.h"
#include "mmiwidget_grid.h"
#include "mmiwidget_page.h"
#include "mmiwidget_virtualwin.h"
#include "mmiwidget_wallpaper.h"

#include "mmiqbtheme_export.h"

#include "mmiqbtheme_config.h"
#include "mmiqbtheme_item.h"
#include "mmiqbtheme_grid.h"
#include "mmiqbtheme_page.h"
#include "mmiqbtheme_virtualwin.h"
#include "mmiqbtheme_dockbar.h"

#include "mmiqbtheme_idlewin.h"
#include "mmiqbtheme_idleexport.h"
#include "mmiqbtheme_menuwin.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 
#define MMIWIDGET_VER_PRE_NETWORK_Y     0
#define MMIWIDGET_VER_PRE_NETWORK_X     26

#define MMIWIDGET_VER_PRE_CLOCK_X       82
#define MMIWIDGET_VER_PRE_CLOCK_Y       266

#define MMIWIDGET_VER_PRE_ALARM_X       0
#define MMIWIDGET_VER_PRE_ALARM_Y       110

#define MMIWIDGET_VER_PRE_PB_X          0    
#define MMIWIDGET_VER_PRE_PB_Y          110

#define MMIWIDGET_PRELOAD_PAGE_0        0
#define MMIWIDGET_PRELOAD_PAGE_1        1
#define MMIWIDGET_PRELOAD_PAGE_2        2


#ifndef ARRAY_SIZE
#define ARRAY_SIZE( a )     ( sizeof (a) / sizeof (a)[0] )
#endif

//#define GET_SHORTCUT_NODE(page, idx)   ((uint16)(((page & 0x00FF) << 8) | ((idx & 0x00FF))))

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
 
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/


LOCAL const MMIGRID_WIDGET_PRELOAD_POSITION_T s_grid_idle_position[] =
{                                                   
    {MMIWIDGET_SHORTCUT_SET_ID,         0 , 0 , 0 },
    {MMIWIDGET_FM_ID,                   0 , 0 , 1 },
#ifdef MMI_WIDGET_WEATHER2
    {MMIWIDGET_WEATHER2_ID,              0 , 0 , 2 },
#endif
    {MMIWIDGET_TIME_ID,                 0 , 0 , 3 },
    {MMIWIDGET_ALARM_ID,                0 , 0 , 4 },    
    {MMIWIDGET_MP3_ID,                   0 , 0 , 5 },
    {MMIWIDGET_CALENDAR_ID,              0 , 0 , 6 },
};

LOCAL CONST MMIGRID_SHOTRCUT_PRELOAD_POSITION_T s_shortcut_init_info[] = 
{
#if defined MAINLCD_SIZE_320X480
#ifdef MMI_PDA_SUPPORT
    {ID_MAINMENU_IDLE_STYLE,            0 , 3 , 3},/*menu_id,col_index,row_index,preload_page*/
#endif
    {ID_MAINMENU_QBTHEMEAPP,            1 , 3 , 3},
    {ID_MAINMENU_QBTHEMEMORE,           2 , 3 , 3},
    {ID_COMMUNICATION_BROWSER,          3 , 3 , 3},
#endif

#if defined MAINLCD_SIZE_240X320 || defined MAINLCD_SIZE_240X400 || defined MAINLCD_SIZE_176X220
#ifdef MMI_PDA_SUPPORT
    {ID_MAINMENU_IDLE_STYLE,            0 , 2 , 3},/*menu_id,col_index,row_index,preload_page*/
#endif
    {ID_MAINMENU_QBTHEMEAPP,            1 , 2 , 3},
    {ID_MAINMENU_QBTHEMEMORE,           2 , 2 , 3},
    {ID_COMMUNICATION_BROWSER,          3 , 2 , 3},
#endif

#if 0
#ifdef  CAMERA_SUPPORT
    {ID_ENTERTAIMENT_CAMERA,            3 , 4 , 3},
#endif
    {ID_ENTERTAIMENT_MP3,               0 , 3 , 2},
    {ID_ENTERTAIMENT_VIDEO,             0 , 3 , 2},
    {ID_ENTERTAIMENT_FAVORITE,          3 , 3 , 2},
    {ID_COMMUNICATION_CL,               3 , 4 , 2},
    {ID_PIC_VIEWER,                     0 , 4 , 2},
    {ID_TOOLS_CALCULATOR,               1 , 4 , 2},
    {ID_TOOLS_ALARM,                    2 , 4 , 2},
#endif
};

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

#ifdef MMI_GRID_MP3
extern const MMIWIDGET_ITEM_INFO_T g_mp3_widget;
#endif

#ifdef MMI_WIDGET_MEMO
extern const MMIWIDGET_ITEM_INFO_T g_memo_add_widget;
extern const MMIWIDGET_ITEM_INFO_T g_memo_widget[];
#endif

#ifdef MMI_WIDGET_SHORTCUT_MENU
extern const MMIWIDGET_ITEM_INFO_T g_shortcut_menu_widget;
#endif

#ifdef MMI_GRID_CALENDAR
extern const MMIWIDGET_ITEM_INFO_T g_calendar_widget;
#endif

#ifdef SCH_ASSISTANT_WIDGET
extern const MMIWIDGET_ITEM_INFO_T g_schedule_widget;
#endif
#ifdef MMI_WIDGET_NETWORK
extern const MMIWIDGET_ITEM_INFO_T g_network_widget;
#endif

#ifdef MMI_WIDGET_TIME
extern const MMIWIDGET_ITEM_INFO_T g_time_widget;
#else
#ifdef MMI_WIDGET_DUAL_CLOCK
extern const MMIWIDGET_ITEM_INFO_T g_clock_widget;
#endif
#endif
//extern const MMIWIDGET_ITEM_INFO_T g_custom_word_widget;

#ifdef WEATHER_SUPPORT
 
//extern const MMIWIDGET_ITEM_INFO_T g_weather_widget;
#ifdef MMI_WIDGET_WEATHER2
extern const MMIWIDGET_ITEM_INFO_T g_weather_widget2;
#endif
#endif

#if defined(FM_SUPPORT)
#ifdef MMI_WIDGET_FM
extern const MMIWIDGET_ITEM_INFO_T g_fm_widget;
#endif
#endif
#ifdef MMI_WIDGET_ALARM
extern const MMIWIDGET_ITEM_INFO_T g_alarm_grid;
#endif
#ifdef MMI_WIDGET_SHORTCUT_SET
extern const MMIWIDGET_ITEM_INFO_T g_shortcut_set_widget;
#endif
#ifdef MMI_WIDGET_ACLOCK
extern const MMIWIDGET_ITEM_INFO_T g_aclock_widget;
extern const MMIWIDGET_ITEM_INFO_T g_aclock_widget2;
extern const MMIWIDGET_ITEM_INFO_T g_aclock_widget3;
#endif

#ifdef MMI_WIDGET_SEARCH
extern const MMIWIDGET_ITEM_INFO_T g_search_widget;
#endif
#ifdef MMI_WIDGET_DUAL_CLOCK
extern const MMIWIDGET_ITEM_INFO_T g_dual_clock_widget;
#endif
#ifdef MMI_WIDGET_DUAL_ACLOCK
extern const MMIWIDGET_ITEM_INFO_T g_dual_aclock_widget;
#endif

#ifdef MMI_GRID_COUNTEDTIME
extern const MMIWIDGET_ITEM_INFO_T g_countedtime_widget;
#endif

#ifdef MMI_GRID_CONTACT
extern const MMIWIDGET_ITEM_INFO_T g_contact_add_widget;
extern const MMIWIDGET_ITEM_INFO_T g_contact_widget[];
#endif

#ifdef BROWSER_SUPPORT_DORADO
#ifdef MMI_WIDGET_NETSEARCH
extern const MMIWIDGET_ITEM_INFO_T g_netsearch_widget;
#endif
#endif
#if defined(MMI_ENABLE_DCD)
extern const MMIWIDGET_ITEM_INFO_T g_dcd_widget;
#endif
#ifdef MMI_WIDGET_SPEED_DIAL
extern const MMIWIDGET_ITEM_INFO_T       g_speed_dial_widget;
#endif  

#ifdef MMI_GRID_IMG_VIEWER
extern const MMIWIDGET_ITEM_INFO_T g_imgviewer_widget;
#endif

#ifdef SNS_SUPPORT
#ifdef WRE_WEIBO_SUPPORT
extern const MMIWIDGET_ITEM_INFO_T g_sina_widget;
#endif
#ifdef WRE_FACEBOOK_SUPPORT
extern const MMIWIDGET_ITEM_INFO_T g_facebook_widget;
#endif
#ifdef WRE_TWITTER_SUPPORT
extern const MMIWIDGET_ITEM_INFO_T g_twitter_widget;
#endif

#endif

//所有widget应用的表
LOCAL const MMIWIDGET_ITEM_INFO_T *s_all_widget_table[] =
{
#ifdef WEATHER_SUPPORT
     
    //  &g_weather_widget,
#ifdef MMI_WIDGET_WEATHER2
        &g_weather_widget2,
#endif
    
#endif
#ifdef BROWSER_SUPPORT_DORADO
#ifdef MMI_WIDGET_NETSEARCH
        &g_netsearch_widget,
#endif
#endif
#ifdef SNS_SUPPORT
#ifdef WRE_WEIBO_SUPPORT
        &g_sina_widget,
#endif
#ifdef WRE_FACEBOOK_SUPPORT
        &g_facebook_widget,
#endif
#ifdef WRE_TWITTER_SUPPORT
        &g_twitter_widget,
#endif        
#endif
#ifdef MMI_GRID_MP3
        &g_mp3_widget,
#endif    
    
#ifdef MMI_WIDGET_MEMO   
        &g_memo_add_widget,
        &g_memo_widget[0],
        &g_memo_widget[1],
        &g_memo_widget[2],
        &g_memo_widget[3],
        &g_memo_widget[4],    
#endif
    
#ifdef MMI_WIDGET_SHORTCUT_MENU    
        &g_shortcut_menu_widget,
#endif
    
#ifdef MMI_GRID_CALENDAR
        &g_calendar_widget,
#endif
#ifdef SCH_ASSISTANT_WIDGET
        &g_schedule_widget,
#endif
    
#ifdef MMI_WIDGET_NETWORK
        &g_network_widget,
#endif
#ifdef MMI_WIDGET_TIME
        &g_time_widget,
#else
#ifdef MMI_WIDGET_DUAL_CLOCK
        &g_clock_widget,
        #endif
#endif
    
#if defined(FM_SUPPORT) 
#ifdef MMI_WIDGET_FM
        &g_fm_widget,
#endif
#endif
#ifdef MMI_WIDGET_ALARM
        &g_alarm_grid,
#endif
#ifdef MMI_WIDGET_SHORTCUT_SET
        &g_shortcut_set_widget,
#endif
#ifdef MMI_WIDGET_ACLOCK
        &g_aclock_widget,    
        &g_aclock_widget2,
        &g_aclock_widget3,
#endif
#ifdef MMI_WIDGET_SEARCH
        &g_search_widget,
#endif
#if defined(MMI_ENABLE_DCD)
        &g_dcd_widget,
#endif
#ifdef MMI_WIDGET_DUAL_CLOCK
        &g_dual_clock_widget,
#endif
#ifdef MMI_WIDGET_DUAL_ACLOCK
        &g_dual_aclock_widget,
#endif
#ifdef MMI_GRID_IMG_VIEWER
        &g_imgviewer_widget,
#endif
#ifdef MMI_GRID_COUNTEDTIME
        &g_countedtime_widget,
#endif
#ifdef MMI_GRID_CONTACT
        &g_contact_add_widget,
        &g_contact_widget[0],    
        &g_contact_widget[1],
        &g_contact_widget[2],
        &g_contact_widget[3],    
        &g_contact_widget[4],    
        &g_contact_widget[5],
        &g_contact_widget[6],
        &g_contact_widget[7],    
        &g_contact_widget[8],
        &g_contact_widget[9],
#endif
#ifdef MMI_WIDGET_SPEED_DIAL
        &g_speed_dial_widget,
#endif    
    0
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
LOCAL void CreateIdleGrid(void);

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
//  Description : preload widget on idle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void InitialGrids(MMI_IDLE_GRID_DATA_T *ctrl_ptr);

/*****************************************************************************/
//  Description : preload pb widget on idle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void PreloadWidgetBar(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, MMIWIDGET_ITEM_INFO_T *item_ptr);


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create idle widget
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIQBTHEME_CreateIdle(void)
{
    if (MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
    {
        CreateIdleGrid();		
    }
}

/*****************************************************************************/
//  Description : destroy idle widget
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIQBTHEME_DestroyIdle(void)
{
    DestroyIdleWidget();
}

/*****************************************************************************/
//  Description : creat idle widget
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void DestroyIdleWidget(void)
{
    MMIQBIDLE_Destroy(MMIWIDGET_CTRL_IDLE_ID);
}

/*****************************************************************************/
//  Description : creat idle widget
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void CreateIdleGrid(void)
{
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetFullScreenBothRect();
    BOOLEAN result = FALSE;
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = 0;
    
    result = MMIQBIDLE_Create(MAIN_IDLE_WIN_ID, MMIWIDGET_CTRL_IDLE_ID, &both_rect);
    idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr();
    MMIWIDGET_LoadAllWidget(idle_grid_ptr, s_all_widget_table, ARRAY_SIZE(s_all_widget_table));        
    MMIQBIDLE_SetCurPageIndex(idle_grid_ptr->cur_page_index);    
}

/*****************************************************************************/
//  Description : preload pb widget on idle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void PreloadWidgetBar(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, MMIWIDGET_ITEM_INFO_T *item_ptr)
{
    uint32 index = 0;
    uint32 widget_id = 0;
	
    if(PNULL == idle_grid_ptr)
    {
	   return ;	
	}
	
    index = idle_grid_ptr->shortcut.shortcut_num;	
    if (PNULL != item_ptr)
    {
        widget_id = item_ptr->widget_id;
        idle_grid_ptr->shortcut.shortcut_item[index].widget_id = widget_id;
        idle_grid_ptr->shortcut.shortcut_item_pos[index] = item_ptr->start_pos;
        //save_info_ptr->widget_shortcut_list.item_info[index].save_pos.y = 0;
        index++;
    } 
    idle_grid_ptr->shortcut.shortcut_num = index;
}

/*****************************************************************************/
//  Description : preload widget on idle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void InitialGrids(MMI_IDLE_GRID_DATA_T *ctrl_ptr)
{
    uint32 i = 0;
    uint32 j = 0;
    uint32 widget_id = 0;
    MMIWIDGET_ITEM_INFO_T *item_ptr = PNULL;
     
    ctrl_ptr->cur_page_index = MMIWIDGET_DEFAULT_PAGE_INDEX;    
    ctrl_ptr->new_page_index = MMIWIDGET_DEFAULT_PAGE_INDEX;
    ctrl_ptr->page_num = MMI_GRID_PAGE_NUM;
	//init widgets
    for (i = 0; i < ARR_SIZE(s_all_widget_table); i++)
    {
		BOOLEAN is_init_to_idle = FALSE;
        item_ptr = s_all_widget_table[i]; /*lint !e605*/

        if (PNULL == item_ptr)
        {
            continue;
        }
        
        widget_id = item_ptr->widget_id;

		for (j = 0; j < ARRAY_SIZE(s_grid_idle_position); j++)
        {
			if (widget_id == s_grid_idle_position[j].widget_id)
			{
				MMIWIDGET_InitWidgetsPosition(ctrl_ptr, item_ptr, s_grid_idle_position[j]);
                is_init_to_idle = TRUE;
				break;
			}
		}
		if (!is_init_to_idle)
		{
		    PreloadWidgetBar(ctrl_ptr, item_ptr);
		}
    }

	//init shortcuts
    //if(PNULL != save_info_ptr->icon_item_info)
	//{
        for(i = 0; i < ARRAY_SIZE(s_shortcut_init_info); i++)
		{
            MMIWIDGET_InitShortcutsPosition(ctrl_ptr,s_shortcut_init_info[i]);
		}
	//}
}

/*****************************************************************************/
//  Description : read nv
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIQBIDLE_ReadWidgetNV(
                        MMI_GRID_SAVE_INFO_T *save_info_ptr
                        )
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MMI_IDLE_GRID_DATA_T  *idle_grid_ptr = PNULL;
    uint32 i = 0;
    uint32 widget_id = 0;
    
    idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr();

    if (PNULL == idle_grid_ptr)
    {
        return;
    }

    if (PNULL != save_info_ptr)
    {
        MMINV_READ(MMI_QBTHEME_IDLE_SAVE_INFO, save_info_ptr, return_value);        
        
        if (MN_RETURN_SUCCESS != return_value || FALSE == save_info_ptr->is_nv_set)
        {
            InitialGrids(idle_grid_ptr);
            //MMINV_WRITE(MMI_QBTHEME_IDLE_SAVE_INFO, save_info_ptr);   
            MMIQBIDLE_SaveWidgetInfoToNV(idle_grid_ptr);
            return;
        }
        else
        {
            idle_grid_ptr->shortcut.shortcut_num = save_info_ptr->widget_shortcut_list.item_num ;
    
            for (i = 0; i < idle_grid_ptr->shortcut.shortcut_num; i++)
            {
                widget_id = save_info_ptr->widget_shortcut_list.item_info[i].widget_id;
                if (MMIWIDGET_IsValidWidgetId(widget_id))
                {            
                    idle_grid_ptr->shortcut.shortcut_item[i].widget_id = widget_id;
                }
            }
            idle_grid_ptr->page_num = save_info_ptr->page_num;
            idle_grid_ptr->cur_page_index = save_info_ptr->cur_page_index;            
            idle_grid_ptr->new_page_index = save_info_ptr->cur_page_index;
            SCI_MEMCPY(idle_grid_ptr->icon_ptr, save_info_ptr->icon_item_info, 
                MMIQBTHEME_MAX_WIDGET_PAGE_NUM*MMI_IDLE_GRID_PAGE_CELL_NUM * sizeof(MMI_IDLE_SC_ICON_T));
            SCI_MEMCPY(idle_grid_ptr->grid_data_tab, save_info_ptr->sc_grid_data_tab, 
                MMIQBTHEME_MAX_WIDGET_PAGE_NUM*MMI_IDLE_GRID_PAGE_CELL_NUM * sizeof(uint16));   
        }
    }
}

/*****************************************************************************/
//  Description : save widget info to NV
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIQBIDLE_SaveWidgetInfoToNV(
                                          MMI_IDLE_GRID_DATA_T *idle_grid_ptr
                                          )
{
    MMI_GRID_SAVE_INFO_T *save_info = PNULL;
    uint32 i = 0;
    uint32 widget_id = MMIWIDGET_INVALID_ID;
    uint32 index = 0;
    
    if (PNULL == idle_grid_ptr)
    {
        //SCI_TRACE_LOW:"SaveWidgetInfoToNV() PNULL == idle_grid_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIQBTHEME_INIT_899_112_2_18_3_9_16_309,(uint8*)"");
        return FALSE;
    }
    
    save_info = (MMI_GRID_SAVE_INFO_T*)SCI_ALLOC_APP(sizeof(MMI_GRID_SAVE_INFO_T));
    
    if (PNULL == save_info)
    {
        return FALSE;
    }
    
    save_info->widget_shortcut_list.item_num = idle_grid_ptr->shortcut.shortcut_num;
    
    for (i = 0; i < idle_grid_ptr->shortcut.shortcut_num; i++)
    {
        widget_id = idle_grid_ptr->shortcut.shortcut_item[i].widget_id;
        if (MMIWIDGET_IsValidWidgetId(widget_id))
        {
            save_info->widget_shortcut_list.item_info[index++].widget_id = widget_id;
        }
    }
    
    save_info->page_num = idle_grid_ptr->page_num;
    
    save_info->cur_page_index = idle_grid_ptr->cur_page_index;
    save_info->is_nv_set = TRUE;
    SCI_MEMCPY(save_info->icon_item_info, idle_grid_ptr->icon_ptr,
                MMIQBTHEME_MAX_WIDGET_PAGE_NUM*MMI_IDLE_GRID_PAGE_CELL_NUM * sizeof(MMI_IDLE_SC_ICON_T));
    SCI_MEMCPY(save_info->sc_grid_data_tab, idle_grid_ptr->grid_data_tab, 
                MMIQBTHEME_MAX_WIDGET_PAGE_NUM*MMI_IDLE_GRID_PAGE_CELL_NUM * sizeof(uint16));   
    
    MMINV_WRITE(MMI_QBTHEME_IDLE_SAVE_INFO, save_info);
    
    SCI_FREE(save_info);  
    save_info = PNULL;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : Reset Factory Setting
//  Global resource dependence : none
//  Author: nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIQBIDLE_ResetFactorySetting(void)
{
    MMI_GRID_SAVE_INFO_T  set_nv = {0};
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = MMIAPIQBIDLE_GetGridPtr();
    if(0 != idle_grid_ptr)
    {
        MMIWIDGET_UnLoadAllWidget(idle_grid_ptr);
        MMIAPIQBTHEME_DestroyIdle();
    }
    
    SCI_MEMSET(&set_nv, 0, sizeof(MMI_GRID_SAVE_INFO_T));
    MMINV_WRITE(MMI_QBTHEME_IDLE_SAVE_INFO, &set_nv);
        
#ifdef MMI_WIDGET_MEMO 
    MMIWIDGET_MEMO_ResetFactorySettings();    
#endif
#if defined MMI_GRID_CONTACT
    MMIWIDGET_PB_ResetFactorySettings();
#endif
}


