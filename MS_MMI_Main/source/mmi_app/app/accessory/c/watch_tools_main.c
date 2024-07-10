/*****************************************************************************
** File Name:      watch_set_main.c                                          *
** Author:         bin.wang1                                                 *
** Date:           02/17/2020                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 02/17/2020           bin.wang1             Create                         *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/

#include "guilistbox.h"
#include "watch_common_list.h"
#include "os_api.h"
#include "sci_types.h"
#include "window_parse.h"

#include "mmiset_image.h"
#include "mmiset_text.h"
#include "mmiset_id.h"
////#include "watch_set_sound.h"
////#include "watch_set_brightness.h"
////#include "watch_set_alarmlist.h"
////#include "watch_set_more.h"
////#include "watch_set_clock.h"
////#include "watch_set_about.h"
////#include "watch_set_about.h"
#include "watch_commonwin_export.h"
//#ifdef BLUETOOTH_SUPPORT
////#include "mmibt_export.h"
////#include "watch_set_backlight.h"
#include "mmicc_export.h" //for dialpad
////#include "watch_launcher_editclockwin.h"
//#endif
#ifdef ADULT_WATCH_SUPPORT
#include "watch_set_display.h"
#include "watch_set_wear.h"
#include "watch_set_system.h"
#include "mmiwlan_text.h"
#include "mmiwifi_export.h"
#include "adultwatch_bt_win.h"
#include "watch_set_bt_ble_manage.h"
#include "adultwatch_bt_view.h"
#endif
#ifdef SCREEN_SHAPE_CIRCULAR
#include "watch_alarm_main.h"
#endif
#include "watch_common.h"
#include "mmiset_menutable.h"
#include "mmidisplay_data.h"


#if defined(ZDT_TOOLS_MENU_SUPPORT)// wuxx add.
//主菜单不够放置的功能,可放在TOOLS 下展示

#include "watch_tools_main.h"

#include "mmi_default.h"
#include "mmiacc_id.h"
#include "mmiacc_position.h"
#include "mmialarm_text.h"
#include "mmialarm_image.h"
#include "mmiset_export.h"
#include "mmiacc_nv.h"
#include "guistring.h"


#ifndef TIP_NULL
#define TIP_NULL 0
#endif

extern PUBLIC void MMIAPICALC_OpenMainWin(void);

extern PUBLIC void MMIZDT_OpenWeatherWin(void);

extern PUBLIC void WatchStopWatch_MainWin_Enter(void);

extern PUBLIC void MMIZDT_OpenPedometerWin(void);

extern PUBLIC void MMIZDT_OpenScheduleWin(void);

extern PUBLIC void WatchCL_ListWin_Enter(void);

extern PUBLIC void MMIZDT_OpenManualWin(void);


#if defined(HERO_ENGINE_SUPPORT)
#if defined(HERO_APP_WSTORE_OPEN)
extern PUBLIC void launcher_hero_appstore(void);
extern PUBLIC void launcher_hero_appstore_manager(void);
#endif
#endif



/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifdef ADULT_WATCH_SUPPORT
LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_wlan         = { TXT_SET_LIST_WLAN,                   IMAGE_SET_MAIN_AW_WLAN };
LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_bluetooth    = { TXT_BLUETOOTH,                       IMAGE_SET_MAIN_AW_BLUETOOTH };


LOCAL WATCHCOM_LIST_ITEM__ST s_watch_tools_main_text_list_data[] =
{
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_wlan,       WatchWIFI_MainWin_Enter },//todo
#ifndef BBM_ROLE_SUPPORT
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_bluetooth,  AdultBT_MainWin_Enter },
#else
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_bt1,        AdultWatchBT_MainWin_Enter },
#endif

};
#else

// ADD TOOLS MENU HERE. 

//计算器
LOCAL WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST   list_item_calculator    = { IMAGE_CALCULATOR_ICON,     TXT_TOOLS_CALCULATOR };
//天气
LOCAL WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST   list_item_weather    = { IMAGE_WEATHER_SEC_MENU_ICON,     TXT_WEATHER_FORECAST };
//闹钟
LOCAL WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST   list_item_alarm_clock    = { IMAGE_ALARM_ICON,     TXT_ALARM_CLOCK };
//秒表
LOCAL WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST   list_item_stopwatch    = { IMAGE_STOPWATCH_SEC_MENU_ICON,     TXT_ACC_STOPWATCH };
//计步
LOCAL WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST   list_item_pedometer    = { IMAGE_PEDOMETER_SEC_MENU_ICON,     STXT_TOOLS_PEDOMETER };//TXT_ZDT_MAINMENU_STEP TXT_ZDT_MAINMENU_JB TXT_ZDT_MAINMENU_PEDO
//课程表
LOCAL WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST   list_item_schedule    = { IMAGE_SCHEDULE_SEC_MENU_ICON,     STXT_TOOLS_SCHEDULE };
//通话记录
LOCAL WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST   list_item_call_log   = { IMAGE_CALL_LOG_SEC_MENU_ICON,     STXT_MAIN_CL };
//说明书
LOCAL WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST   list_item_manual_readme   = { IMAGE_MANUAL_README_SEC_MENU_ICON,     STXT_TOOLS_MANUAL_README };



#if defined(HERO_ENGINE_SUPPORT)
#if defined(HERO_APP_WSTORE_OPEN)
LOCAL WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST   list_item_app_store    = { IMAGE_APP_STORE_SEC_MENU_ICON,     TXT_APP_STORE };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST   list_item_app_center    = { IMAGE_APP_CENTER_SEC_MENU_ICON,     TXT_APP_CENTER };
#endif
#endif

LOCAL WATCHCOM_LIST_ITEM__ST s_watch_tools_main_text_list_data[] =
{
	{ WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR, &list_item_calculator, MMIAPICALC_OpenMainWin},

	{ WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR, &list_item_weather, MMIZDT_OpenWeatherWin},

	{ WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR, &list_item_alarm_clock, WatchAlarm_MainWin_Enter},

	{ WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR, &list_item_stopwatch, WatchStopWatch_MainWin_Enter},

	{ WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR, &list_item_pedometer, MMIZDT_OpenPedometerWin},

	{ WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR, &list_item_schedule, MMIZDT_OpenScheduleWin},

	{ WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR, &list_item_call_log, WatchCL_ListWin_Enter},

	{ WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR, &list_item_manual_readme, MMIZDT_OpenManualWin},

    
    #if defined(HERO_ENGINE_SUPPORT)
    #if defined(HERO_APP_WSTORE_OPEN)
        { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR, &list_item_app_store, launcher_hero_appstore},
        { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR, &list_item_app_center, launcher_hero_appstore_manager},
    #endif
    #endif


};



#endif
/**---------------------------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DEFINITION                                           *
 **---------------------------------------------------------------------------------------------*/

LOCAL void WatchTools_Main_APP_OK( void )
{
    WatchCOM_List_Item_CallBack( MMI_WAHCH_TOOLS_MAIN_LIST_CTRL_ID );
    #if 0 
      MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
    GUIMENU_GetId(MMISET_MAIN_LIST_CTRL_ID,&group_id,&menu_id);
	    switch (menu_id)
	    {
		case ID_SETTINGS_CALLRING_SELECT:
			Settings_ChargeRingSelectWin_Enter();
			break;
		case ID_SETTINGS_BACKLIGHT_TIMEOUT:
			Settings_BackLight_Enter();
			break;
		case ID_SETTINGS_BRIGHTNESS:
			MMIAPISET_ZdtBrightnessWin();
			break;
		case ID_SETTINGS_VOLUME:
			MMIAPISET_ZdtVolumeWin();
			break;
		case ID_SETTINGS_RESTORE:
			MMIAPISET_ZdtPowerRestoreWin();
			break;
		case ID_SETTINGS_POWEROFF:
			MMIAPISET_ZdtPowerOffWin();
			break;
		case ID_SETTINGS_POWER_RESTART:
			MMIAPISET_ZdtPowerRestartWin();
			break;
		case ID_SETTINGS_WATCH_INFO:
			/*Settings_RegardWin_Enter();*/MMIAPISET_ZdtWatchInfoWin();
			break;
#ifdef FOTA_SUPPORT_ABUP
		case ID_SETTINGS_PHONE_ABUP_FOTA:
			break;
#endif
		default:
			break;
		}
	#endif
}

LOCAL void WatchTools_Main_APP_CANCEL( void )
{
    MMK_CloseWin( MMI_WATCH_TOOLS_MAIN_WIN_ID );
}

LOCAL void WatchTools_Main_OPEN_WINDOW(MMI_WIN_ID_T win_id)
{
    uint32 listNum = 0;
#ifdef SCREEN_SHAPE_CIRCULAR
    GUI_RECT_T          list_rect = WATCHLIST_RECT;
#endif
	GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
	GUI_RECT_T          rect         = MMITHEME_GetFullScreenRectEx(win_id);
	GUI_RECT_T          text_rect={42,0,240,MMI_SPECIAL_TITLE_HEIGHT};
	GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
	MMI_STRING_T        string = {0};
	GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;

    listNum = sizeof(s_watch_tools_main_text_list_data)/sizeof(WATCHCOM_LIST_ITEM__ST);
#ifdef SCREEN_SHAPE_CIRCULAR
    GUILIST_SetRect(MMI_WAHCH_TOOLS_MAIN_LIST_CTRL_ID,&list_rect);
#endif
    WatchCOM_TextList_Create( s_watch_tools_main_text_list_data, listNum, MMI_WAHCH_TOOLS_MAIN_LIST_CTRL_ID );
    MMK_SetAtvCtrl( MMI_WATCH_TOOLS_MAIN_WIN_ID, MMI_WAHCH_TOOLS_MAIN_LIST_CTRL_ID );

	 #if defined(ADD_TITLE_IN_ALL_SETTING_MENU)		
		//draw TITLE bg		
			GUIRES_DisplayImg(PNULL,
			    &rect,
			    PNULL,
			    win_id,
			    IMAGE_THEME_BLACK_BG_SPEC, 
			    &lcd_dev_info); 

		///////draw title
		text_style.align = ALIGN_LVMIDDLE;
		text_style.font = SONG_FONT_28;
		text_style.font_color = MMI_WHITE_COLOR;
		text_style.char_space = 0;
	
    	MMI_GetLabelTextByLang(STXT_MAIN_TOOLS, &string);
		GUISTR_DrawTextToLCDInRect( 
		    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		    (const GUI_RECT_T      *)&text_rect,       //the fixed display area
		    (const GUI_RECT_T      *)&text_rect,       //用户要剪切的实际区域
		    (const MMI_STRING_T    *)&string,
		    &text_style,
		    state,
		    GUISTR_TEXT_DIR_AUTO
		    );
#endif	
	#if 0
    MMI_MENU_GROUP_ID_T         group_id        =   0;
    MMI_MENU_ID_T               menu_id         =   0;
#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)		
    GUI_RECT_T          content_rect={0,MMI_SPECIAL_TITLE_HEIGHT,240,240}; ////xiongkai ADD_TITLE_IN_ALL_SETTING_MENU	
    GUI_RECT_T          title_rect={0,0,240,MMI_SPECIAL_TITLE_HEIGHT}; 	
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
    GUI_RECT_T          rect         = MMITHEME_GetFullScreenRectEx(win_id);	
    GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    MMI_STRING_T        string = {0};	
    GUI_RECT_T          text_rect={42,0,240,MMI_SPECIAL_TITLE_HEIGHT}; 
#endif

    #if defined(ADD_TITLE_IN_ALL_SETTING_MENU)		
		//draw TITLE bg		
			GUIRES_DisplayImg(PNULL,
			    &rect,
			    PNULL,
			    win_id,
			    IMAGE_THEME_BLACK_BG_SPEC, 
			    &lcd_dev_info); 

		///////draw title
		text_style.align = ALIGN_LVMIDDLE;
		text_style.font = SONG_FONT_28;
		text_style.font_color = MMI_WHITE_COLOR;
		text_style.char_space = 0;
	
    		MMI_GetLabelTextByLang(STXT_MAIN_SETTING, &string);
		GUISTR_DrawTextToLCDInRect( 
		    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		    (const GUI_RECT_T      *)&text_rect,       //the fixed display area
		    (const GUI_RECT_T      *)&text_rect,       //用户要剪切的实际区域
		    (const MMI_STRING_T    *)&string,
		    &text_style,
		    state,
		    GUISTR_TEXT_DIR_AUTO
		    );
#endif		
	     GUIMENU_SetRect(MMISET_MAIN_LIST_CTRL_ID,&content_rect);		 
	     GUIMENU_GetId(MMISET_MAIN_LIST_CTRL_ID,&group_id,&menu_id);
         MMK_SetAtvCtrl(win_id,MMISET_MAIN_LIST_CTRL_ID);	
		 CTRLMENU_SetSecMenuItemHeight(MMISET_SET_VOLUME_CTRL_ID,48,48);
		 CTRLMENU_SetFontColor(MMISET_MAIN_LIST_CTRL_ID,MMI_WHITE_COLOR);
		 #endif
}

/*****************************************************************************/
//  Description : to handle Settings window message
//  Global resource dependence :
//  Author:bin.wang1
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWatchToolsMainWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            #ifdef SCREEN_SHAPE_CIRCULAR
            WATCHCOM_DisplayBackground(win_id);
            #endif
            WatchTools_Main_OPEN_WINDOW(win_id);
            break;
        }
//#ifdef SCREEN_SHAPE_CIRCULAR
        case MSG_FULL_PAINT:
        {
#if 1 
            MMI_MENU_GROUP_ID_T         group_id        =   0;
		    MMI_MENU_ID_T               menu_id         =   0;
           #if defined(ADD_TITLE_IN_ALL_SETTING_MENU)		
		    GUI_RECT_T          content_rect={0,MMI_SPECIAL_TITLE_HEIGHT,240,240}; ////xiongkai ADD_TITLE_IN_ALL_SETTING_MENU	
		    GUI_RECT_T          title_rect={0,0,240,MMI_SPECIAL_TITLE_HEIGHT}; 	
		    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
		    GUI_RECT_T          rect         = MMITHEME_GetFullScreenRectEx(win_id);	
		    GUISTR_STYLE_T      text_style      = {0};/*lint !e64*/
		    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
		    MMI_STRING_T        string = {0};	
		    GUI_RECT_T          text_rect={42,0,240,MMI_SPECIAL_TITLE_HEIGHT}; 
            #endif
            WATCHCOM_DisplayBackground(win_id);
			

		    #if defined(ADD_TITLE_IN_ALL_SETTING_MENU)		
				//draw TITLE bg		
					GUIRES_DisplayImg(PNULL,
					    &rect,
					    PNULL,
					    win_id,
					    IMAGE_THEME_BLACK_BG_SPEC, 
					    &lcd_dev_info); 

				///////draw title
				text_style.align = ALIGN_LVMIDDLE;
				text_style.font = SONG_FONT_28;
				text_style.font_color = MMI_WHITE_COLOR;
				text_style.char_space = 0;
			
		    	MMI_GetLabelTextByLang(STXT_MAIN_TOOLS, &string); // 工具箱
				GUISTR_DrawTextToLCDInRect( 
				    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
				    (const GUI_RECT_T      *)&text_rect,       //the fixed display area
				    (const GUI_RECT_T      *)&text_rect,       //用户要剪切的实际区域
				    (const MMI_STRING_T    *)&string,
				    &text_style,
				    state,
				    GUISTR_TEXT_DIR_AUTO
				    );
                     #endif		
#endif
            break;
        }    
//#endif
        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
        #ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
        #endif
        {
            WatchTools_Main_APP_OK();
            break;
        }
        case MSG_KEYDOWN_RED:
            break;
        case MSG_KEYUP_RED:
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            WatchTools_Main_APP_CANCEL();
            break;
        }
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
WINDOW_TABLE( MMI_WATCH_TOOLS_MAIN_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleWatchToolsMainWindow ),
    WIN_ID( MMI_WATCH_TOOLS_MAIN_WIN_ID ),
    WIN_HIDE_STATUS,
#ifdef ADULT_WATCH_SUPPORT
    WIN_TITLE(STXT_MAIN_TOOLS), // tools  STXT_MAIN_TOOLS
#endif
    CREATE_LISTBOX_CTRL( GUILIST_TEXTLIST_E, MMI_WAHCH_TOOLS_MAIN_LIST_CTRL_ID ),
    //CREATE_MENU_CTRL(WATCHTOOLS_MENULIST_ICON, MMI_WAHCH_TOOLS_MAIN_LIST_CTRL_ID),
#ifndef ADULT_WATCH_SUPPORT
    WIN_SOFTKEY( STXT_OK, TXT_NULL, STXT_RETURN ),
#endif
    END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
// 主菜单不展示的工具功能放在此展示
PUBLIC void WatchTools_MainWin_Enter( void )
{
    MMK_CreateWin( (uint32 *)MMI_WATCH_TOOLS_MAIN_WIN_TAB, PNULL );
}

// Common_mdu_def.h --str define 
//  RES_ADD_STRING(STXT_MAIN_TOOLS,"Tools")    


#endif

