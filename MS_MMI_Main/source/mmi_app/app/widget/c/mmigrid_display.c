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

#include "mmi_app_widget_trc.h"
#ifdef MMI_GRID_IDLE_SUPPORT
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
#include "mmi_modu_main.h"
#include "mmi_id.h"
#include "mmiset_export.h"
#include "mmiset_display.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_kbd.h"
#include "guilcd.h"
#include "window_parse.h"
#include "guimenu.h"
#include "mmitheme_menu.h"
#include "mmitheme_mainmenu.h"
#include "mmiset_wintab.h"
#include "Mmiwidget_text.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
#include "mmiwidget_item.h"
#include "mmiwidget_page.h"
#include "mmiwidget_virtualwin.h"

#include "mmi_mainmenu_export.h"
#ifdef MMI_TASK_MANAGER
#include "mmi_applet_manager.h"
#endif
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#ifdef SEARCH_SUPPORT 
#include "mmisearch_export.h"
#endif
#include "mmi_menutable.h"
#include "mmiset_text.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
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

#define WIDGET_NODE_MAX_NUM            0x040
//#define GET_SHORTCUT_NODE(page, idx)   ((uint16)(((page & 0x00FF) << 8) | ((idx & 0x00FF))))

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
 
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

LOCAL const MMIGRID_WIDGET_PRELOAD_POSITION_T s_grid_idle_position[] =
{                                                   
#ifdef BAR_PHONE_WIDGET 

#ifdef MMI_WIDGET_WEATHER1
    {MMIWIDGET_WEATHER_ID,              0 , 0 , 0 },
#endif

    {MMIWIDGET_MP3_ID,                  0 , 0 , 1 },

#ifdef MMI_WIDGET_SPEED_DIAL
    {MMIWIDGET_SPEED_DIAL_ID,           0 , 0 , 2 },
#endif   

    {MMIWIDGET_FM_ID,                   0 , 0 , 3},
    {MMIWIDGET_IMG_VIEWER_ID,           0 , 0 , 4 },	
    {MMIWIDGET_CALENDAR_ID,             0 , 0 , 5 },
#ifdef MMI_WIDGET_ACLOCK
    {MMIWIDGET_ACLOCK_ID,               0 , 0 , 6 },
#endif

    
#else   //#ifdef BAR_PHONE_WIDGET
		
	{MMIWIDGET_SHORTCUT_SET_ID,         0 , 0 , 0 },
    {MMIWIDGET_FM_ID,                   0 , 0 , 1 },
#ifdef MMI_WIDGET_WEATHER2
    {MMIWIDGET_WEATHER2_ID,              0 , 0 , 2 },
#endif
    {MMIWIDGET_TIME_ID,                 0 , 0 , 3 },
    {MMIWIDGET_ALARM_ID,                0 , 0 , 4 },	
    {MMIWIDGET_MP3_ID,                   0 , 0 , 5 },
    {MMIWIDGET_CALENDAR_ID,              0 , 0 , 6 },
#endif    
};

LOCAL CONST MMIGRID_SHOTRCUT_PRELOAD_POSITION_T s_shortcut_init_info[] = 
{
#ifdef BAR_PHONE_WIDGET 
0
#else
    #if defined MAINLCD_SIZE_240X400 || defined MAINLCD_SIZE_320X480    
#ifdef MMIDC_F_U_CAMERA
        {ID_ENTERTAIMENT_UCAMERA,          0 , 3 , 3},
#else
#ifdef  CAMERA_SUPPORT
        {ID_ENTERTAIMENT_CAMERA,          0 , 3 , 3},
#endif		
#endif        
#ifdef MMIUZONE_SUPPORT
        {ID_ENTERTAIMENT_UZONE,             1 , 3 , 3},/*menu_id,col_index,row_index,preload_page*/
#else
        {ID_ENTERTAIMENT_MP3,          1 , 3 , 3},
#endif
        {ID_COMMUNICATION_BROWSER,          2 , 3 , 3},
#ifdef MMI_PDA_SUPPORT
        {ID_MAINMENU_IDLE_STYLE,            3 , 3 , 3},
#endif        
    #endif    

    #if defined MAINLCD_SIZE_240X320
#ifdef MMIDC_F_U_CAMERA
        {ID_ENTERTAIMENT_UCAMERA,             0 , 2 , 3},/*menu_id,col_index,row_index,preload_page*/
#else        
        {ID_ENTERTAIMENT_FAVORITE,             0 , 2 , 3},/*menu_id,col_index,row_index,preload_page*/
#endif        
#ifdef MMIUZONE_SUPPORT
        {ID_ENTERTAIMENT_UZONE,         	  1 , 2 , 3},
#else
        {ID_ENTERTAIMENT_MP3,          1 , 2 , 3},
#endif
        {ID_COMMUNICATION_BROWSER,        	  2 , 2 , 3},
#ifdef MMI_PDA_SUPPORT
        {ID_MAINMENU_IDLE_STYLE,              3 , 2 , 3},
#endif        
    #endif    
#endif

#if 1
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

#ifdef MMI_SAM_THEME
LOCAL const MMIGRID_WIDGET_PRELOAD_POSITION_T s_sam_grid_idle_position[] =
{                                                   
	{MMIWIDGET_TIME_ID,         0 , 0 , 0 },
	{MMIWIDGET_SEARCH_ID,       0 , 2 , 0 },	
	{MMIWIDGET_PWRSAVING_ID,    0 , 0 , 1 },
    {MMIWIDGET_IMG_VIEWER_ID,   0 , 0 , 2 },
    {MMIWIDGET_DUAL_ACLOCK_ID,  0 , 0 , 3 },
    {MMIWIDGET_DUAL_CLOCK_ID,   1 , 0 , 4 },      
};

LOCAL CONST MMIGRID_SHOTRCUT_PRELOAD_POSITION_T s_sam_shortcut_init_info[] = 
{        
        {ID_CLOCK,                0 , 3 , 0},
#ifdef FM_SUPPORT
        {ID_ENTERTAIMENT_FM,      1 , 3 , 0},
#endif
#ifdef CAMERA_SUPPORT
        {ID_ENTERTAIMENT_CAMERA,  2 , 3 , 0},
#endif        
        {ID_SETTINGS_PHONE,       3 , 3 , 0},
};
#endif
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
#ifdef MMI_WIDGET_WEATHER1
extern const MMIWIDGET_ITEM_INFO_T g_weather_widget;
#endif 	 
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
#ifndef BAR_PHONE_WIDGET 
extern const MMIWIDGET_ITEM_INFO_T g_aclock_widget2;
extern const MMIWIDGET_ITEM_INFO_T g_aclock_widget3;
#endif
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

#ifdef MMI_SAM_THEME
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
extern const MMIWIDGET_ITEM_INFO_T g_pwr_saving_widget;
#endif
#endif

//所有widget应用的表
LOCAL const MMIWIDGET_ITEM_INFO_T *s_all_widget_table[] =
{
#ifdef WEATHER_SUPPORT
#ifdef MMI_WIDGET_WEATHER1
 	&g_weather_widget,
#endif 	
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
    &g_memo_add_widget,/*lint !e64*/
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
#ifndef BAR_PHONE_WIDGET 
    &g_aclock_widget2,
    &g_aclock_widget3,
#endif
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
#ifdef MMI_SAM_THEME
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
    &g_pwr_saving_widget,
#endif    
#endif
    PNULL
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
//  Description : preload widget on idle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void InitialGrids(MMI_IDLE_GRID_DATA_T *idle_grid_ptr);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create idle widget
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIGRID_CreateIdle(void)
{
    if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
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
PUBLIC void MMIAPIGRID_DestroyIdle(void)
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
   MMIGRID_Destroy(MMIWIDGET_CTRL_IDLE_ID);
}

/*****************************************************************************/
//  Description : creat idle widget
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void CreateIdleGrid(void)
{
    MMIWIDGET_CTRL_E  ctrl_id = MMIWIDGET_CTRL_IDLE_ID;
    MMI_GRID_SAVE_INFO_T save_info = {0};
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetFullScreenBothRect();
    BOOLEAN result = FALSE;
    MMI_IDLE_GRID_DATA_T *idle_grid_ptr = 0;
    result = MMIGRID_Create(MAIN_IDLE_WIN_ID, MMIWIDGET_CTRL_IDLE_ID, &both_rect);
    if (result)
    {
        //初始化widget shortcut数据
        MMIGRID_SetMaxItemNum(ctrl_id, MMIWIDGET_MAX_ITEM_NUM);
        //读NV
        MMIGRID_ReadWidgetNV(&save_info);
        
        idle_grid_ptr = (MMI_IDLE_GRID_DATA_T *)MMIAPIGRID_GetGridPtr();
        MMIWIDGET_LoadAllWidget(idle_grid_ptr, s_all_widget_table, ARRAY_SIZE(s_all_widget_table));        
        MMIGRID_SetCurPageIndex(idle_grid_ptr->cur_page_index);

    }
}

/*****************************************************************************/
//  Description : preload pb widget on idle.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void PreloadWidgetBar(MMI_IDLE_GRID_DATA_T *idle_grid_ptr, MMIWIDGET_ITEM_INFO_T *item_ptr)
{
    uint32 index = idle_grid_ptr->shortcut.shortcut_num;
    uint32 widget_id = item_ptr->widget_id;
    
    if (PNULL != item_ptr)
    {
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
LOCAL void InitialGrids(MMI_IDLE_GRID_DATA_T *idle_grid_ptr)
{
    uint32 i = 0;
    uint32 j = 0;
    uint32 widget_id = 0;
    MMIWIDGET_ITEM_INFO_T *item_ptr = PNULL;
    MMI_IDLE_GRID_DATA_T *ctrl_ptr = PNULL;
    MMIGRID_SHOTRCUT_PRELOAD_POSITION_T *pre_icon_list   = PNULL;
    uint32 pre_icon_size   = 0;
    uint32 pre_widget_size = 0;
    MMIGRID_WIDGET_PRELOAD_POSITION_T   *pre_widget_list = PNULL;
    
    ctrl_ptr = MMIAPIGRID_GetGridPtr();

    if (PNULL == ctrl_ptr)
    {
        return;
    }

#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        ctrl_ptr->cur_page_index = MMIWIDGET_SAM_DEFAULT_PAGE_INDEX;     
    }
    else
#endif  
    {
        ctrl_ptr->cur_page_index = MMIWIDGET_DEFAULT_PAGE_INDEX;    
    }
    
    ctrl_ptr->page_num = MMI_GRID_PAGE_NUM;

#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        pre_icon_size   = ARRAY_SIZE(s_sam_shortcut_init_info);
        pre_widget_size = ARRAY_SIZE(s_sam_grid_idle_position);       
        pre_icon_list   = s_sam_shortcut_init_info;
        pre_widget_list = s_sam_grid_idle_position; 
    }
    else
#endif
    {
        pre_icon_size   = ARRAY_SIZE(s_shortcut_init_info);
        pre_widget_size = ARRAY_SIZE(s_grid_idle_position);      
        pre_icon_list   = s_shortcut_init_info;
        pre_widget_list = s_grid_idle_position;     
    }
    
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

		for (j = 0; j < pre_widget_size; j++)
        {
			if (widget_id == pre_widget_list[j].widget_id)
			{
				MMIWIDGET_InitWidgetsPosition(idle_grid_ptr, item_ptr, pre_widget_list[j]);
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
    //if(PNULL != save_info_ptr->icon_item_info)//clean pclint @juan.wu
	{
        for(i = 0; i < pre_icon_size; i++)
		{
            MMIWIDGET_InitShortcutsPosition(idle_grid_ptr,pre_icon_list[i]);
		}
	}
}

/*****************************************************************************/
//  Description : read nv
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIGRID_ReadWidgetNV(
                        MMI_GRID_SAVE_INFO_T *save_info_ptr
                        )
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MMI_IDLE_GRID_DATA_T  *ctrl_ptr = MMIAPIGRID_GetGridPtr();
    uint32 all_page_grid_num = 0;
    uint32 i = 0;
    uint32 widget_id = 0;
    
    if (PNULL == ctrl_ptr)
    {
        return;
    }

    if(!MMK_IsWindowLandscape( ctrl_ptr->win_id))   
    {   
        all_page_grid_num = MMI_IDLE_SC_ITEM_MAX;
    }
    else
    {
        all_page_grid_num = MMI_IDLE_SC_ITEM_MAX_LANDSCAPE;
    }

    if (PNULL != save_info_ptr)
    {
#ifdef MMI_SAM_THEME	
        if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
        {    
            MMINV_READ(MMI_SAM_SAVE_INFO, save_info_ptr, return_value);
        }
        else
#endif
        {
            MMINV_READ(MMI_GRID_SAVE_INFO, save_info_ptr, return_value);    
        }

        if (MN_RETURN_SUCCESS != return_value || FALSE == save_info_ptr->is_nv_set)
        {
            InitialGrids(ctrl_ptr);
            //MMINV_WRITE(MMI_GRID_SAVE_INFO, save_info_ptr);        
            MMIGRID_SaveWidgetInfoToNV(ctrl_ptr);
        }
        else
        {            
            ctrl_ptr->cur_page_index = save_info_ptr->cur_page_index;
            ctrl_ptr->page_num = MMI_GRID_PAGE_NUM ;    // save_info_ptr->page_num;
            ctrl_ptr->shortcut.shortcut_num = save_info_ptr->widget_shortcut_list.item_num ;
    
            for (i = 0; i < ctrl_ptr->shortcut.shortcut_num; i++)
            {
                widget_id = save_info_ptr->widget_shortcut_list.item_info[i].widget_id;
                if (MMIWIDGET_IsValidWidgetId(widget_id))
                {            
                    ctrl_ptr->shortcut.shortcut_item[i].widget_id = widget_id;
                }
            }
            SCI_MEMCPY(ctrl_ptr->icon_ptr, save_info_ptr->icon_item_info, all_page_grid_num*sizeof(MMI_IDLE_SC_ICON_NV_T));
            SCI_MEMCPY(ctrl_ptr->grid_data_tab, save_info_ptr->sc_grid_data_tab,  all_page_grid_num*sizeof(uint16));   
        }
    }
}

#endif


/*Edit by script, ignore 2 case. Fri Apr 27 09:38:56 2012*/ //IGNORE9527
