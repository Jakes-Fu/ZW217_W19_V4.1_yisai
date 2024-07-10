/****************************************************************************
** File Name:      mmiflash_wintab.c                                                *
** Author:                                                                 *
** Date:           27/05/2011                                              *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2011       jesse.yu         Create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_app_flash_win_trc.h"
#include "mmiflash_export.h"
#include "ui_layer.h"
#include "dal_time.h"
#include "mmi_position.h"
#include "Mn_type.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_id.h"
#include "mmiset_text.h"
#include "mmimp3_export.h"
#include "mmidisplay_data.h"
#include "mmi_custom_define.h"
#include "mmiacc_event.h"
#include "flashtask_msg.h"
#include "flashtask_main.h"
#include "mmi_appmsg.h"
#include "guistatusbar.h"
#include "mmiidle_export.h"
#include "mmi_mainmenu_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmisms_export.h"
#include "mmicl_export.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmifm_export.h"
#include "mmipub.h"
#include "mmischedule_export.h"
#include "Mmk_gsensor.h"
#include "Mmistk.h"

#ifdef JAVA_SUPPORT
#include "Mmijava_export.h"
#endif

//#include "Mmiwifi_api.h"
#ifdef MMI_KEY_LOCK_SUPPORT
#include "mmikl_export.h"
#endif
#include "mmiidle_subwintab.h"
#include "Mmidm_export.h"
#include "MAL_phone_func.h"
#include "Mmiudisk_export.h"
#include "mmicalendar_export.h"
#include "mmipb_internal.h"
#include "Mmidc.h"
#include "Mmiebook.h"
#include "Mmiatv_export.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "Mmitv_out.h"
#include "Mmienvset.h"
#include "Mmigame.h"
#include "mmi_wallpaper_export.h"
#include "Mmiset_display.h"
#include "Mmiset_func.h"
#include "Guires.h"

#ifdef MMIEMAIL_SUPPORT
#include "mmiemail_export.h"
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
#define MMI_FLASH_IDLE_VIBRA_DEFAULT_TIME 1500 

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
static BOOLEAN              sFlashVisiable      = 0;
static GUI_LCD_DEV_INFO     stb_layer_handle    = {0, UILAYER_NULL_HANDLE};
static GUI_LCD_DEV_INFO     flash_layer_handle  = {0, UILAYER_NULL_HANDLE};
static GUI_LCD_DEV_INFO     wallpaper_layer_handle  = {0, UILAYER_NULL_HANDLE};
static FLASH_IDLE_ITEM_T    sflash_idle_item[FLASH_IDLE_ITEM_MAX] = {0};

/**---------------------------------------------------------------------------*
 **                         Functions                                         *
 **---------------------------------------------------------------------------*/

LOCAL void MMIFlash_CreateStbLayer(MMI_WIN_ID_T win_id)
{
    uint16 width = 0;
    uint16 height = 0;
    UILAYER_CREATE_T create_info = {0};  
    GUI_RECT_T main_rect = MMITHEME_GetFullScreenRect();    

    width  = main_rect.right - main_rect.left + 1;
    height = MMI_STATUSBAR_HEIGHT;
      
    create_info.lcd_id          = LCD_ID_0;
    create_info.owner_handle    = win_id;
    create_info.offset_x        = 0;
    create_info.offset_y        = 0;
    create_info.width           = width;
    create_info.height          = height;
    create_info.is_bg_layer     = FALSE;
    create_info.is_static_layer = FALSE;   
    
    UILAYER_CreateLayer(&create_info, &stb_layer_handle);
    UILAYER_Clear(&stb_layer_handle);
}

LOCAL void MMIFlash_CreateFlashLayer(MMI_WIN_ID_T win_id)
{
    uint16 width = 0;
    uint16 height = 0;
    UILAYER_CREATE_T create_info = {0};  
    GUI_RECT_T main_rect = MMITHEME_GetFullScreenRect();    

    width  = main_rect.right  - main_rect.left + 1;
    height = main_rect.bottom - main_rect.top  + 1;
        
    create_info.lcd_id          = LCD_ID_0;
    create_info.owner_handle    = win_id;
    create_info.offset_x        = 0;
    create_info.offset_y        = 0;
    create_info.width           = width;
    create_info.height          = height;
    create_info.is_bg_layer     = TRUE;
    create_info.is_static_layer = TRUE;   
    
    UILAYER_CreateLayer(&create_info, &flash_layer_handle);
    UILAYER_Clear(&flash_layer_handle); 
}

LOCAL void MMIFlash_CreateWallpaperLayer(MMI_WIN_ID_T win_id)
{
    uint16 width = 0;
    uint16 height = 0;
    UILAYER_CREATE_T create_info = {0};  
    GUI_RECT_T main_rect = MMITHEME_GetFullScreenRect();    

    width  = main_rect.right  - main_rect.left + 1;
    height = main_rect.bottom - main_rect.top  + 1;
        
    create_info.lcd_id          = LCD_ID_0;
    create_info.owner_handle    = win_id;
    create_info.offset_x        = 0;
    create_info.offset_y        = 0;
    create_info.width           = width;
    create_info.height          = height;
    create_info.is_bg_layer     = TRUE;
    create_info.is_static_layer = FALSE;   
    
    UILAYER_CreateLayer(&create_info, &wallpaper_layer_handle);
    UILAYER_SetLayerColorKey(&wallpaper_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
    
    if (UILAYER_IsLayerActive(&wallpaper_layer_handle))
    {
        UILAYER_Clear(&wallpaper_layer_handle);
        //初始黑色背景
        LCD_FillRect(&wallpaper_layer_handle, main_rect, MMI_BLACK_COLOR);
        MMK_SetCtrlLcdDevInfo(MMIIDLE_WALLPAPER_ANIM_CTRL_ID, &wallpaper_layer_handle);
    }
}

LOCAL void MMIFlash_SetBltLayer(void)
{
    UILAYER_APPEND_BLT_T append_layer = {0};

    append_layer.lcd_dev_info = flash_layer_handle;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);   

    append_layer.lcd_dev_info = stb_layer_handle;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);    
}

LOCAL void MMIFlash_RemoveBltLayer(void)
{
    UILAYER_RemoveBltLayer(&flash_layer_handle);
    UILAYER_RemoveBltLayer(&stb_layer_handle);
}

LOCAL void MMIFlash_InitIdleItem(void)
{
    uint32 i = 0;
    for(i = 0; i < FLASH_IDLE_ITEM_MAX; i++)
    {
        sflash_idle_item[i].wstr_ptr = SCI_ALLOCA((FLASH_IDLE_ITEM_STRING_LEN_MAX + 1)*sizeof(wchar));

		if(PNULL == sflash_idle_item[i].wstr_ptr)
		{
		   return;
		}
		
        sflash_idle_item[i].wstr_len = 0;
    }
}

LOCAL void MMIFlash_IdleGetDateTimeStr(void)
{
    SCI_DATE_T  sys_date    = {0};
    SCI_TIME_T  sys_time = {0};
    char        u8_dt_str[FLASH_IDLE_ITEM_STRING_LEN_MAX+1] = {0};

    TM_GetSysDate(&sys_date);  
    TM_GetSysTime(&sys_time);

    if(sys_date.wday == 0)
    {
        sys_date.wday = 6;
    }
    else
    {
        sys_date.wday--;
    }
    sprintf((char*) u8_dt_str, "%04d%02d%02d%01d%02d%02d", sys_date.year, sys_date.mon, sys_date.mday, 
        sys_date.wday, sys_time.hour, sys_time.min);
    
    MMIAPICOM_StrToWstr((const uint8*)u8_dt_str, sflash_idle_item[FLASH_IDLE_ITEM_DATETIME].wstr_ptr);	
    sflash_idle_item[FLASH_IDLE_ITEM_DATETIME].wstr_len= MMIAPICOM_Wstrlen(sflash_idle_item[FLASH_IDLE_ITEM_DATETIME].wstr_ptr);
    sflash_idle_item[FLASH_IDLE_ITEM_DATETIME].is_display = TRUE;
}

LOCAL void MMIFlash_IdleGetNetStr(void)
{
    MMI_STRING_T        network_str     = {0};
    BOOLEAN             is_fly_mode_on  = FALSE;
    uint                i  = 0;
    wchar 			    opn_spn_name[FLASH_IDLE_ITEM_STRING_LEN_MAX+1] = {0};
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    wchar *sim_deactivated_str = PNULL;
#endif
    
    is_fly_mode_on = MMIAPISET_GetFlyMode();
    
    for (i = 0; i <= MN_DUAL_SYS_2; i++)
    {
        sflash_idle_item[FLASH_IDLE_ITEM_NET1+i].is_display = TRUE;
        if(is_fly_mode_on)
        {
            MMI_GetLabelTextByLang(TXT_SET_FLY_MODE, &network_str);
            
            sflash_idle_item[FLASH_IDLE_ITEM_NET1+i].wstr_len = MIN(network_str.wstr_len, FLASH_IDLE_ITEM_STRING_LEN_MAX);
            MMIAPICOM_Wstrncpy(sflash_idle_item[FLASH_IDLE_ITEM_NET1+i].wstr_ptr, 
                network_str.wstr_ptr, sflash_idle_item[FLASH_IDLE_ITEM_NET1+i].wstr_len);
        }
        else if (MMIAPIPHONE_GetSimExistedStatus(i))
        {
            if (MMIAPIIDLE_NeedShowIconAccordingToDualSysSetting((MN_DUAL_SYS_E)i))
            {
                SCI_MEMSET(&network_str, 0x00, sizeof(network_str));
                network_str.wstr_ptr = (wchar *)opn_spn_name;
                SCI_MEMSET(network_str.wstr_ptr, 0x00, sizeof(opn_spn_name));
                
                MMIAPIPHONE_GetSimAndServiceString((MN_DUAL_SYS_E)i, &network_str);

                sflash_idle_item[FLASH_IDLE_ITEM_NET1+i].wstr_len = MIN(network_str.wstr_len, FLASH_IDLE_ITEM_STRING_LEN_MAX);
                MMIAPICOM_Wstrncpy(sflash_idle_item[FLASH_IDLE_ITEM_NET1+i].wstr_ptr, 
                    network_str.wstr_ptr, sflash_idle_item[FLASH_IDLE_ITEM_NET1+i].wstr_len);
            }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            else
            {
                MMISET_SIM_NAME_T sim_name = {0};
                MMI_STRING_T sim_deactivated = {0};

                sim_name = MMIAPISET_GetSimName(i);

                MMI_GetLabelTextByLang(TXT_SIM_DEACTIVATED, &sim_deactivated);

                //此处不判断是否需要加入空格，统一都多分配一个空格的存储空间(2bytes)    
                sim_deactivated_str = (wchar *)SCI_ALLOC_APP(((sim_deactivated.wstr_len + sim_name.wstr_len + 2)*sizeof(wchar)));
                SCI_MEMSET(sim_deactivated_str, 0x00, ((sim_deactivated.wstr_len + sim_name.wstr_len + 2)*sizeof(wchar)));
                                    
                MMI_WSTRNCPY(sim_deactivated_str,
                                            (sim_deactivated.wstr_len + sim_name.wstr_len + 1),
                                            sim_name.wstr_arr,
                                            sim_name.wstr_len,
                                            sim_name.wstr_len);
                
                // ASCII 码数据，两个字符串中间追加一个空格。
                if (MMIAPICOM_IsASCIIString(sim_name.wstr_arr,sim_name.wstr_len)
                    && MMIAPICOM_IsASCIIString(sim_deactivated.wstr_ptr ,sim_name.wstr_len))
                {
                    MMIAPICOM_Wstrcat(sim_deactivated_str, L" "); 
                    MMI_WSTRNCPY(sim_deactivated_str + sim_name.wstr_len + 1,
                                            (sim_deactivated.wstr_len + 1),
                                            sim_deactivated.wstr_ptr,
                                            sim_deactivated.wstr_len,
                                            sim_deactivated.wstr_len);
                    
                    network_str.wstr_ptr = (wchar *)sim_deactivated_str;
                    network_str.wstr_len = sim_deactivated.wstr_len + sim_name.wstr_len + 1 ;

                    sflash_idle_item[FLASH_IDLE_ITEM_NET1+i].wstr_len = MIN(network_str.wstr_len, FLASH_IDLE_ITEM_STRING_LEN_MAX);
                    MMIAPICOM_Wstrncpy(sflash_idle_item[FLASH_IDLE_ITEM_NET1+i].wstr_ptr, 
                        network_str.wstr_ptr, sflash_idle_item[FLASH_IDLE_ITEM_NET1+i].wstr_len);
                }
                else
                {
                    MMI_WSTRNCPY(sim_deactivated_str + sim_name.wstr_len ,
                                            (sim_deactivated.wstr_len + 1),
                                            sim_deactivated.wstr_ptr,
                                            sim_deactivated.wstr_len,
                                            sim_deactivated.wstr_len);
                    network_str.wstr_ptr = (wchar *)sim_deactivated_str;
                    network_str.wstr_len = sim_deactivated.wstr_len + sim_name.wstr_len ;

                    sflash_idle_item[FLASH_IDLE_ITEM_NET1+i].wstr_len = MIN(network_str.wstr_len, FLASH_IDLE_ITEM_STRING_LEN_MAX);
                    MMIAPICOM_Wstrncpy(sflash_idle_item[FLASH_IDLE_ITEM_NET1+i].wstr_ptr, 
                        network_str.wstr_ptr, sflash_idle_item[FLASH_IDLE_ITEM_NET1+i].wstr_len);
                }
            }
#endif
        }
        else
        {

#ifndef MMI_IDLE_MINI_SUPPORT
            MMI_GetLabelTextByLang(TXT_INSERT_SIM, &network_str);
#else
            MMI_GetLabelTextByLang(TXT_INSERT_SIM_MINI, &network_str);   
#endif                
            sflash_idle_item[FLASH_IDLE_ITEM_NET1+i].wstr_len = MIN(network_str.wstr_len, FLASH_IDLE_ITEM_STRING_LEN_MAX);
            MMIAPICOM_Wstrncpy(sflash_idle_item[FLASH_IDLE_ITEM_NET1+i].wstr_ptr, 
                network_str.wstr_ptr, sflash_idle_item[FLASH_IDLE_ITEM_NET1+i].wstr_len);
        }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
        if (PNULL != sim_deactivated_str)
        {
            SCI_FREE(sim_deactivated_str);
            sim_deactivated_str = PNULL;
        }
#endif
    }   
}

LOCAL void MMIFlash_IdleGetMp3Str(void)
{
    MMI_STRING_T mp3_str = {0};
	
    sflash_idle_item[FLASH_IDLE_ITEM_MP3].is_display = FALSE;
    if (MMIAPIMP3_IsPlayerActive())
    {
        //SCI_TRACE_LOW:"MMIFlash_IdleGetMp3Str"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFLASH_WINTAB_343_112_2_18_2_19_17_0,(uint8*)"");
        mp3_str.wstr_ptr = (wchar*)SCI_ALLOC_APP((MMIFMM_FILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar));
        SCI_MEMSET(mp3_str.wstr_ptr,0,((MMIFMM_FILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar)));     
#ifdef MMI_AUDIO_PLAYER_SUPPORT		
        MMIAPIMP3_GetCurMp3Name(&mp3_str, MMIMP3_NAME_MAX_LEN);
#endif
    	//display string
    	if(0 == mp3_str.wstr_len || (MMIMP3_PLAYER_STATE_NONE == MMIAPIMP3_GetPlayerState()))
    	{
            sflash_idle_item[FLASH_IDLE_ITEM_MP3].is_display = FALSE;
    	}
    	else
    	{
            sflash_idle_item[FLASH_IDLE_ITEM_MP3].is_display = TRUE;
            sflash_idle_item[FLASH_IDLE_ITEM_MP3].wstr_len= MIN(mp3_str.wstr_len, FLASH_IDLE_ITEM_STRING_LEN_MAX);
            MMIAPICOM_Wstrncpy(sflash_idle_item[FLASH_IDLE_ITEM_MP3].wstr_ptr, 
                mp3_str.wstr_ptr, sflash_idle_item[FLASH_IDLE_ITEM_MP3].wstr_len);
    	}
        //free memory
        SCI_FREE(mp3_str.wstr_ptr);
    } 
	
}

LOCAL void MMIFlash_IdleGetCalendarStr(void)
{
    uint8                   time_str[FLASH_IDLE_ITEM_STRING_LEN_MAX+1] = {0};
    uint16                  time_wstr[FLASH_IDLE_ITEM_STRING_LEN_MAX+1] = {0};
    uint16                  time_str_len = 0;   
    MMI_STRING_T            schedule_str = {0};
    MMIACC_SMART_EVENT_T*   schedule_event_ptr = PNULL;
    uint16                  event_id = 0;
#ifdef CALENDAR_SUPPORT
    //get schedule
    if (!MMIAPISCH_OpenEventToday(&event_id) || (MMIAPISCH_IsAllPastEventToday(&event_id)))
    {
        sflash_idle_item[FLASH_IDLE_ITEM_CALENDAR].is_display = FALSE;
    }
    else
    {
        schedule_event_ptr = (MMIACC_SMART_EVENT_T*)SCI_ALLOCA(sizeof(MMIACC_SMART_EVENT_T));

		if(PNULL == schedule_event_ptr)
		{
		   return;		
		}
		
        SCI_MEMSET(schedule_event_ptr, 0, sizeof(MMIACC_SMART_EVENT_T) );
        MMIAPIALM_FormInteEventInfo(event_id,schedule_event_ptr);

        //timer
        sprintf((char*)time_str,"%02d:%02d", schedule_event_ptr->event_fast_info.hour, schedule_event_ptr->event_fast_info.minute);
		
		/*lint -e666*/
        time_str_len = (uint16)MIN((strlen((char *)time_str)), (FLASH_IDLE_ITEM_STRING_LEN_MAX));
        MMI_STRNTOWSTR(time_wstr,time_str_len,
            time_str,time_str_len,
            time_str_len);

        //get schedule string len
        schedule_str.wstr_len = time_str_len + schedule_event_ptr->event_content_info.w_length+1;

        //copy schedule string
        schedule_str.wstr_ptr = (wchar*)SCI_ALLOCA((schedule_str.wstr_len+1)*sizeof(wchar));

		if(PNULL == schedule_str.wstr_ptr)
		{
		   SCI_FREE(schedule_str.wstr_ptr);
		   return;		
		}
		
        SCI_MEMSET(schedule_str.wstr_ptr,0,((schedule_str.wstr_len+1)*sizeof(wchar)));

        //copy time
        MMI_WSTRNCPY(schedule_str.wstr_ptr, schedule_str.wstr_len, time_wstr, time_str_len, time_str_len);
        MMI_WSTRNCPY(schedule_str.wstr_ptr+time_str_len,schedule_str.wstr_len-time_str_len,L" ",1,1);

        //copy event
        MMI_WSTRNCPY(schedule_str.wstr_ptr+time_str_len+1,schedule_str.wstr_len-time_str_len-1,
            schedule_event_ptr->event_content_info.w_content,schedule_event_ptr->event_content_info.w_length,
            schedule_event_ptr->event_content_info.w_length);

        sflash_idle_item[FLASH_IDLE_ITEM_CALENDAR].is_display = TRUE;
        sflash_idle_item[FLASH_IDLE_ITEM_CALENDAR].wstr_len= MIN(schedule_str.wstr_len, FLASH_IDLE_ITEM_STRING_LEN_MAX);
        MMIAPICOM_Wstrncpy(sflash_idle_item[FLASH_IDLE_ITEM_CALENDAR].wstr_ptr, 
            schedule_str.wstr_ptr, sflash_idle_item[FLASH_IDLE_ITEM_CALENDAR].wstr_len);

        SCI_FREE(schedule_str.wstr_ptr);
        SCI_FREE(schedule_event_ptr);
    }
#endif
}

LOCAL void MMIFlash_IdleGetLanguage(void)
{
    MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;
    MMIAPISET_GetLanguageType(&lang_type);

    if(MMISET_LANGUAGE_SIMP_CHINESE == lang_type)
    {
        sflash_idle_item[FLASH_IDLE_ITEM_LANGUAGE].is_display = TRUE;
    }
    else
    {
        sflash_idle_item[FLASH_IDLE_ITEM_LANGUAGE].is_display = FALSE;
    }
    sflash_idle_item[FLASH_IDLE_ITEM_LANGUAGE].wstr_len = 0;
}

PUBLIC FLASH_IDLE_ITEM_T * MMIAPIFlash_GetFlashIdleItem(void)
{
	return sflash_idle_item;
}

PUBLIC void MMIAPIFlash_UpdateIdleItem(void)
{
    //SCI_TRACE_LOW:"MMIAPIFlash_UpdateIdleItem"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFLASH_WINTAB_457_112_2_18_2_19_18_1,(uint8*)"");
    MMIFlash_IdleGetDateTimeStr();
    MMIFlash_IdleGetNetStr();
#ifdef MMI_AUDIO_PLAYER_SUPPORT  
    MMIFlash_IdleGetMp3Str();
#endif
    MMIFlash_IdleGetCalendarStr();
    MMIFlash_IdleGetLanguage();
}

LOCAL void MMIFlash_DestroyIdleItem(void)
{
    uint32 i = 0;
    for(i = 0; i < FLASH_IDLE_ITEM_MAX; i++)
    {
        //SCI_ASSERT(sflash_idle_item[i].wstr_ptr != PNULL);
        if(sflash_idle_item[i].wstr_ptr != PNULL)
        {
        	SCI_FREE(sflash_idle_item[i].wstr_ptr);
        	sflash_idle_item[i].wstr_ptr = PNULL;
        }
        sflash_idle_item[i].is_display = FALSE;
    }
}


LOCAL void MMIFlash_ResetWallpaperLayer(void)
{
    if (UILAYER_IsLayerActive(&wallpaper_layer_handle))
    {
        MMK_SetCtrlLcdDevInfo(MMIIDLE_WALLPAPER_ANIM_CTRL_ID, &wallpaper_layer_handle);
    }
    else
    {
        MMK_SetCtrlLcdDevInfo(MMIIDLE_WALLPAPER_ANIM_CTRL_ID, (GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev());
    }
}

PUBLIC MMI_RESULT_E MMIAPIFlashWin_HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E            recode              = MMI_RESULT_TRUE;
    static uint8            s_vibra_timer_id    = 0;
    MMISET_IDLE_DISPLAY_T   *idle_mode          = PNULL;
    LOCAL BOOLEAN          s_is_first_power_on  = TRUE;
    LOCAL BOOLEAN          s_is_timer_on        = FALSE;
    LOCAL BOOLEAN           s_is_down_red_key   = FALSE;  //是否按了red key
    IGUICTRL_T              *stb_ctr_ptr        = PNULL;
 	
    idle_mode = MMIAPISET_GetIdleDisplayMode();

	//SCI_TRACE_LOW:"[idle] MMIAPIFlashWin_HandleMsg msg_id 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFLASH_WINTAB_505_112_2_18_2_19_18_2,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_OpenGsensor( win_id );

        s_is_down_red_key = FALSE;

        MMIDEFAULT_StartLcdBackLightTimer();
        MMIDEFAULT_StartAutoKeylockTimer();
#ifndef TOUCH_PANEL_HWICON_SUPPORT_NONE
        MMIAPIIDLE_StartHdShortCutBarIcon();
#endif
        MMIAPISTK_CnfIdleScrAva();
        MMIAPISTK_CnfSimUserAct();
        //creat wallpaper ctrl
#if defined MMI_WALLPAPER_SUPPORT
        MMIAPIIDLE_CreatWallpaperCtrl();
#endif
        //flash 
        MMISET_SetIsSensorOpenFlag(0);
        MMISET_SetIdleStyle(MMISET_IDLE_STYLE_COMMON);
        MMIAPISET_SetMainmenuStyle(MMIMAINMENU_MATRIX_E);
        {
            GUI_POINT_T         dis_point = {0};
            
            GUIWIN_CreateStbDyn(MAIN_IDLE_WIN_ID); // 动态创建状态栏
            GUIWIN_SetStbState(MAIN_IDLE_WIN_ID, GUISTATUSBAR_STATE_EVENT, TRUE);
            GUIWIN_SetStbBgIsDesplay(MAIN_IDLE_WIN_ID, FALSE);
            MIAPIIDLE_MAIN_SetStbTimeVisible(FALSE);
    		MMIFlash_CreateStbLayer(win_id);
            MMIFlash_CreateFlashLayer(win_id);
            //remove main layer.
            UILAYER_RemoveMainLayer();
            stb_ctr_ptr = (IGUICTRL_T*)MMK_GetCtrlPtrByWin(win_id, MMITHEME_GetStatusBarCtrlId());
            GUICTRL_SetLcdDevInfo(stb_ctr_ptr, &stb_layer_handle);
            MMIFlash_SetBltLayer();
            MMIFlash_ResetWallpaperLayer();
    		sFlashVisiable = TRUE;
    		MMIFlash_InitIdleItem();
    		FlashTask_Play(FLASH_UI_ENTRY_PATH);
            // display background
            GUIRES_DisplayImg(&dis_point, PNULL, PNULL, win_id, IMAGE_FLASH_BG, &flash_layer_handle);
        }
        //MMIDLE_RestartSsTimer(win_id);
        MMK_SetAtvCtrl(win_id,IDLEWIN_STATUSBAR_CTRL_ID);

#ifdef JAVA_SUPPORT_IA
    	MMIAPIJAVA_JBlendiaAjmsInit();
#endif

//多卡的情况下，将PS的启动延后至Ilde Win Open的时候
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        // TODO: 改为调用MMIAPIPHONE_PowerOnPsFull
        MMIAPIPHONE_PowerOnPsEx();
#endif

/* BEGIN: Modified by George.Liu 01526, 2010/8/2   PN:MergeToMw1029forCustom */
#ifdef MMI_WIFI_SUPPORT
        MMIAPIWIFI_Init();
#endif
/* END:   Modified by George.Liu 01526, 2010/8/2   PN:MergeToMw1029forCustom */

#ifdef MMIEMAIL_SUPPORT
//  MMIAPIEMAIL_SyncEmail();
#endif

        break;

        //失去焦点
    case MSG_BACKLIGHT_TURN_OFF:
    case MSG_LOSE_FOCUS:
        //flash
        {
            //restore main layer.
            UILAYER_RestoreMainLayer();
            UILAYER_Clear(&stb_layer_handle);
            UILAYER_RemoveBltLayer(&stb_layer_handle);
            UILAYER_RemoveBltLayer(&flash_layer_handle);
            MMIFlash_ResetWallpaperLayer();
#if 0//def WIN32
            FlashTask_Stop();
#else
    		FlashTask_SuspendUI();
    		while(FlashTask_GetMode() == FLASHTASK_MODE_PLAY)
    			SCI_Sleep(10);
#endif
    		sFlashVisiable = FALSE;
        }
        MIAPIIDLE_MAIN_SetStbTimeVisible(TRUE);
        s_is_down_red_key = FALSE;
        MAIN_SetAllStbAppCallbackValid(FALSE);
//        MMIAPIIDLESS_StopTimer();
        //StopAnimTimer();
        break;
    
    //重获焦点
    case MSG_BACKLIGHT_TURN_ON:
    case MSG_GET_FOCUS:
        MIAPIIDLE_MAIN_SetStbTimeVisible(FALSE);
        MAIN_SetAllStbAppCallbackValid(TRUE);
        MMIAPISMS_CheckSMSState(FALSE);

        //MMIDLE_RestartSsTimer(win_id);
#ifdef MMI_KEY_LOCK_SUPPORT
        if (MMIAPIKL_IsPhoneLocked())
        {
            MMIAPIKL_LockPhone();
        }
#endif
        if (MMIDEFAULT_GetFlipStatus())
        {
            // 如果Flip处于Open状态才StartTimer
            //StartAnimTimer();
        }
        else
        {   
            //StopAnimTimer();
        }
        MMIAPISTK_CnfIdleScrAva();
        MMIAPISTK_CnfSimUserAct();
        {
            //remove main layer.
            UILAYER_RemoveMainLayer();

            stb_ctr_ptr = (IGUICTRL_T*)MMK_GetCtrlPtrByWin(win_id, MMITHEME_GetStatusBarCtrlId());
            GUICTRL_SetLcdDevInfo(stb_ctr_ptr, &stb_layer_handle);
            UILAYER_Clear(&stb_layer_handle);
        	MMIFlash_SetBltLayer();
        	MMIFlash_ResetWallpaperLayer();
#if 0//def WIN32
        	FlashTask_Play(FLASH_UI_ENTRY_PATH);
#else
    		FlashTask_ResumeUI();
    		while(FlashTask_GetMode() == FLASHTASK_MODE_PAUSE)
    			SCI_Sleep(10);
            Mal_Phone_STB_Update();//flash
#endif
    		sFlashVisiable = TRUE;
        }
        break;

        
#ifdef DCD_SUPPORT
	case MSG_NOTIFY_TAB_NEED_ITEM_CONTENT:
	case MSG_NOTIFY_TAB_SWITCH: 		
	case MSG_DCD_TAB_UPDATE:
        MMIAPIDCD_HandleIdleWinMessage(win_id,msg_id,param);
        break;
#endif

    case MSG_FULL_PAINT:
        if(s_is_first_power_on)
        {
            s_is_first_power_on = FALSE;
            if(MMIAPIENG_GetChipTestMode() == MMIENG_CHIP_TEST_SETTING_ON)
            {
                MMIENGAPI_StartChipTest();
            }
        }       
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏的显示，和主屏同步,主要是彩信
        MMISUB_UpdateDisplaySubLcd();  
#endif
        if (MMIAPISET_IsNeedResetDateTime()) //判断是否需要设置时间和日期
        { 
            //在MSG_FULL_PAINT中打开新窗口，需要清除上次刷新
            MMITHEME_InitMainLcdStortUpdateRect();
            MMIAPISET_SetDate(); 
        }
        //回到Idle后判断是否要还插着USB
        if (MMIAPIUdisk_IsUSBDelay()&&!MMIAPIDM_GetHandsetLock())
        {
            MMK_PostMsg(win_id,MSG_IDLE_USB,PNULL,PNULL); //判断是否有延时的usb
        }
        MMIAPIDM_HandleDmIdleOption();
        //flash 
        {
            if(sFlashVisiable)
            {
				GUI_RECT_T rt;
				//SCI_TRACE_LOW:"flash_printf:@@@@@Screen update"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFLASH_WINTAB_679_112_2_18_2_19_18_3,(uint8*)"");
				amuse_get_flash_update_rect( &rt );
                MMITHEME_StoreUpdateRect(&flash_layer_handle, rt);
    	    }
    	}
        break;

    case MSG_NOTIFY_STATUSBAR_PAINT:
        if (MMK_IsFocusWin(win_id))
        {
            UILAYER_Clear(&stb_layer_handle);
            s_is_timer_on = MAIN_IsStbDispTime();
            MIAPIIDLE_MAIN_SetStbTimeVisible(FALSE);
        }
        break;

    case MSG_NOTIFY_STATUSBAR_PAINT_END:
        if (MMK_IsFocusWin(win_id))
        {
            MIAPIIDLE_MAIN_SetStbTimeVisible(s_is_timer_on);
            s_is_timer_on = FALSE;
        }
        break;

    case MSG_IDLE_USB:  
        if (MMIAPIUdisk_IsUSBDelay())
        {        
            MMIAPIUdisk_OpenUsbOperWindow();
            MMIAPIUdisk_SetUSBDelay(FALSE);     
        }
        break;
        
    case MSG_CTL_ANIM_DISPLAY_IND:
        {
            GUIANIM_DISPLAY_IND_T   display_ind = *(GUIANIM_DISPLAY_IND_T *)param;

            //close waiting win and prompt set wallpaper
#if defined MMI_WALLPAPER_SUPPORT
            MMIAPIFMM_CloseWaitSetWallpaper(display_ind.result);
#endif
        }
        if (MMK_IsFocusWin(win_id))
        {
            //MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
        }
        break;
        
    case MSG_CTL_ANIM_GIF_UPDATE:     
    //分钟中断
    case MSG_IDLE_UPDATE_DATETIME:
    //刷新运营商名
    case MSG_IDLE_UPDATE_NETWORK:
    //刷新倒计时时间
    case MSG_IDLE_UPDATE_COUNTEDTIME:
    case MSG_IDLE_UPDATE_MP3:
        if(MSG_IDLE_UPDATE_MP3 == msg_id)
        {
            //SCI_TRACE_LOW:"[idle] MMIAPIFlashWin_HandleMsg MSG_IDLE_UPDATE_MP3"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFLASH_WINTAB_734_112_2_18_2_19_18_4,(uint8*)"");
        }
        if (MMK_IsFocusWin(win_id))
        {
            Mal_Phone_STB_Update();//flash
            //MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
        }
        break;
    //刷新小图标 
    case MSG_IDLE_UPDATE_ICON:
        if ( (MMIDEFAULT_IsBacklightOn()) && (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId()) )
        {          
            MMK_SendMsg( IDLEWIN_STATUSBAR_CTRL_ID,MSG_CTL_PAINT,PNULL);
        }
        break;
    //震动启动处理
    case MSG_IDLE_VIBRA:
        MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_OTHER);
        if (s_vibra_timer_id != 0)
        {
            MMK_StopTimer(s_vibra_timer_id);
            s_vibra_timer_id = 0;
        }
        s_vibra_timer_id = MMK_CreateWinTimer(MAIN_IDLE_WIN_ID, MMI_FLASH_IDLE_VIBRA_DEFAULT_TIME, FALSE);
        break;

    case MSG_KEYREPEAT_UP:
    case MSG_KEYREPEAT_DOWN:
    case MSG_KEYREPEAT_LEFT:
    case MSG_KEYREPEAT_RIGHT:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG:        
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        break;

    //flash
    case MSG_APP_WEB:
	case MSG_APP_OK:
	case MSG_APP_CANCEL:
	case MSG_APP_UP:
	case MSG_APP_DOWN:
	case MSG_APP_LEFT:
	case MSG_APP_RIGHT:
    case MSG_KEYUP_WEB:
	case MSG_KEYUP_OK:
	case MSG_KEYUP_CANCEL:
	case MSG_KEYUP_UP:
	case MSG_KEYUP_DOWN:
	case MSG_KEYUP_LEFT:
	case MSG_KEYUP_RIGHT:
	    //MMIDLE_RestartSsTimer(win_id);
		FlashTask_HandleKeyMsg(msg_id);
		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_DOWN:	
	case MSG_TP_PRESS_UP:	
	case MSG_TP_PRESS_MOVE:	
	    //MMIDLE_RestartSsTimer(win_id);
		FlashTask_HandleTPMsg(msg_id, (uint32)param);
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/04
    case MSG_APP_SMS:
		MMIAPISMS_OpenMainMenuWin();			
		break;
#endif 
    //一键上网
    case MSG_APP_AT:
#ifdef BROWSER_SUPPORT
        {
            MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/

            entry_param.type = MMIBROWSER_ACCESS_MAINMENU;
            entry_param.dual_sys = MN_DUAL_SYS_MAX;
            entry_param.url_ptr = PNULL;
            entry_param.user_agent_ptr = PNULL;

            MMIAPIBROWSER_Entry(&entry_param);
        }
#endif
        break;
    
    //绿键已拨电话
    case MSG_APP_GREEN:        
    case MSG_APP_CALL2:
    case MSG_APP_CALL3:
    case MSG_APP_CALL4:       
        MMIAPICL_OpenCallsListWindow(MMICL_CALL_DIALED);
        break;  

    case MSG_KEYDOWN_TV:
#ifdef CMMB_SUPPORT
		MMIAPIMTV_OpenMTVPlayer();	
#else
        MMIAPIATV_OpenATV();
#endif			
        break;
        
    case MSG_APP_CAMERA:
#ifdef  CAMERA_SUPPORT    
        if(MMIDEFAULT_GetFlipStatus())
        {
            MMIAPIDC_OpenPhotoWin();
        }
        else
#endif
        {
            MMIDEFAULT_TurnOnBackLight();
            //MMIAPIMP3_HandleCameraKey();
        }
        break;

#ifdef  CAMERA_SUPPORT  
    case MSG_KEYLONG_CAMREA:
        MMIAPIDC_OpenPhotoWin();
        break;
#endif
    case MSG_APP_RED:
        s_is_down_red_key = TRUE;
        recode = MMI_RESULT_FALSE;
        break;

    case MSG_KEYUP_RED:
        if ((s_is_down_red_key) &&
        (MMIAPISET_GetRedKeyLock()))
        {
#ifdef LOCK_SCREEN_SPECIAL_EFFECT_SUPPORT        
            MMISEPC_CloseAllLight();
#else
            MMIDEFAULT_CloseAllLight();
#endif
        }
        else
        {
            //MMIDLE_RestartSsTimer(win_id);
            recode = MMI_RESULT_FALSE;
        }
        break;
        
    case MSG_APP_STAR:
        MMIAPIIDLE_OpenDialWin(msg_id,TRUE);
        break;

#ifdef FLIP_PHONE_SUPPORT    
    //关盖
    case MSG_KEYDOWN_FLIP:   
        //StopAnimTimer();
        recode = MMI_RESULT_FALSE;
        break;

    //开盖
    case MSG_KEYUP_FLIP:
        // 启动Timer，就是为了解决FlipDown状态下不启动TImer 
        //StartAnimTimer();
		recode = MMI_RESULT_FALSE;
        break;
#endif

    case MSG_KEYDOWN_HEADSET_DETECT:
    case MSG_KEYUP_HEADSET_DETECT:
        // 耳机插入和耳机拔出
        //SCI_TRACE_LOW:"IDLEWIN_HandleWinMsg: handle headset MSG!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFLASH_WINTAB_893_112_2_18_2_19_18_5,(uint8*)"");
        //MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        recode = MMI_RESULT_FALSE;
        break;

    //时钟事件
    case MSG_TIMER:
        //震动时钟
        if (s_vibra_timer_id == *((uint8 *) param))
        {
            //震动完成
            //SCI_TRACE_LOW:"IdleWin_HandleMsg:received timer msg!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFLASH_WINTAB_904_112_2_18_2_19_18_6,(uint8*)"");
            MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_OTHER);
            s_vibra_timer_id =0;
        }
//         else if (MMIAPIIDLESS_IsScreenSaverTimerId(param))
//         {
//             MMIAPIIDLESS_StopTimer();
//             if (MAIN_IDLE_WIN_ID == MMK_GetFocusWinId())
//             {         
//                if((!MMIAPIMP3_IsPlayerActive() && !MMIAPIKUR_IsPlayerActive() &&!MMIAPIFM_IsFmActive()))//for cr220750 maryxiao
//                 {                    
//                     if(MMIAPIIDLESS_IsScreenSaverMp4Type())
//                     {
//                         MMIAPISET_StopAllRing(FALSE);
//                     }
//                     MMIAPIIDLESS_OpenScreenSaver();
//                 }
//                 if (MMIDEFAULT_IsBacklightOn())
//                 {
//                     MMIDLE_RestartSsTimer(win_id);
//                 }
//             }
//         }
        break;
  
    case MSG_KEYDOWN_UPSIDE:
        if(MMIAPIKUR_IsPlayerPlaying())
        {
            MMIAPIKUR_SetKurVolume(TRUE);
        }

#ifdef FM_SUPPORT
        if(FM_PLAYING == MMIAPIFM_GetStatus())
        {
            MMIAPIFM_AdjustVolume(1);
        }
#endif
        break;

    case MSG_KEYDOWN_DOWNSIDE:
        //在mp3背景播放，不管开盖盒盖，短侧键调节mp3音量

        if(MMIAPIKUR_IsPlayerPlaying())
        {
            MMIAPIKUR_SetKurVolume(FALSE);
        }

#ifdef FM_SUPPORT
        if(FM_PLAYING == MMIAPIFM_GetStatus())
        {
            MMIAPIFM_AdjustVolume(-1);
        }
#endif

        break;

    //进输入号码窗口
    case MSG_KEYDOWN_0:
    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9:
    case MSG_KEYUP_HASH:
        MMIAPIIDLE_OpenDialWin(msg_id, TRUE);
        break;
        
    case MSG_KEYLONG_HASH:
        //MMIDLE_RestartSsTimer(win_id);
        
        if (MMIAPIENG_GetIQModeStatus())
        {
            MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
            break;
        }
        else if (!MMIAPITVOUT_GetStartingStatus())
        {
            uint8 active_mode = 0;
            switch (MMIAPIENVSET_GetActiveModeId())
            {
            case MMIENVSET_STANDARD_MODE:  // 普通环境
            case MMIENVSET_INSIDE_MODE:      // 车内环境
            case MMIENVSET_NOISY_MODE:       // 户外环境
                active_mode = MMIENVSET_SILENT_MODE;
                break;
                
            case MMIENVSET_SILENT_MODE:      // 安静环境
                active_mode = MMIENVSET_MEETING_MODE;
                //if (MMIAPIIDLE_IdleWinIsOpen())
                //{
                //    return MMK_SendMsg(MAIN_IDLE_WIN_ID, MSG_IDLE_VIBRA, PNULL);
                //}
                //在idle窗口的处理函数里判断idle是否打开,有必要么?
                MMK_SendMsg(MAIN_IDLE_WIN_ID, MSG_IDLE_VIBRA, PNULL);
                break;
                
            case MMIENVSET_MEETING_MODE:     // 会议环境
                active_mode = MMIENVSET_STANDARD_MODE;
                break;
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
            case MMIENVSET_EARPHONE_MODE:
			case MMIENVSET_BLUETOOTH_MODE:
				break;
#endif
            default:
                //SCI_TRACE_LOW:"MMIAPIFlashWin_HandleMsg MSG_KEYLONG_HASH!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFLASH_WINTAB_1013_112_2_18_2_19_19_7,(uint8*)"");
                break;
            }
			UILAYER_Clear(&stb_layer_handle);
			if(MMIAPIENVSET_GetActiveModeId()<= MMIENVSET_NOISY_MODE)
			{
	            MMIAPIENVSET_SetActiveModeForIdle(active_mode);
			}
            //MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        else
        {
            MMIAPITVOUT_Close(); 
        }
        break; 
        
    case MSG_CLOSE_WINDOW:
        //StopAnimTimer();
		#ifdef JAVA_SUPPORT_IA
		MMIAPIJAVA_JBlendiaAjmsFinalize();
		#endif

        MMK_CloseGsensor( win_id );
        break;
	case MMIFLASH_MSG_INVOKE_MCARE_LIST:
		MCareV30_Main_Entry();
		break;
    case MMIFLASH_MSG_ENTRY_SCREEN:
        switch(*(FLASH_SCREEN_ID_ENUM*)param)
        {
            case FLASH_SCREEN_ID_PB:
            case FLASH_SCREEN_ID_S_PB:
                MMIAPIPB_OpenListAllGroupWin();
                break;
            case FLASH_SCREEN_ID_MESSAGE:
            case FLASH_SCREEN_ID_S_MESSAGE:
                MMIAPISMS_OpenMessageMainMenuWin();
                break;
#ifdef CALENDAR_SUPPORT
            case FLASH_SCREEN_ID_S_CALENDAR:
                MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_CALENDAR, MMIAPICALEND_OpenMainWin);
                break;
#endif
            case FLASH_SCREEN_ID_CL:
                MMIAPICL_OpenMainMenuWindow();
                break;

            case FLASH_SCREEN_ID_BROWSER:
#ifdef BROWSER_SUPPORT
                {
                    MMIBROWSER_ENTRY_PARAM entry_param = {0};
                    
                    entry_param.type = MMIBROWSER_ACCESS_MAINMENU;
                    entry_param.dual_sys = MN_DUAL_SYS_MAX;
                    entry_param.url_ptr = PNULL;
                    entry_param.user_agent_ptr = PNULL;
                    
                    MMIAPIBROWSER_Entry(&entry_param);
                }
#endif
                break;

            case FLASH_SCREEN_ID_PLAYER:
                MMIAPIMENU_EnterPlayer();
                break;

            case FLASH_SCREEN_ID_TOOLS:
                MMIAPIMENU_EnterTools();
                break;

            case FLASH_SCREEN_ID_CAMERA:
#ifdef  CAMERA_SUPPORT
                MMK_SendMsg(win_id, MSG_LOSE_FOCUS, PNULL);
                MMIAPIDC_OpenPhotoWin();
#endif
                break;
                
            case FLASH_SCREEN_ID_FAVORATE:
                MMIAPIFMM_OpenExplorer();
                break;

            case FLASH_SCREEN_ID_TV:
#ifdef ATV_SUPPORT
                MMIAPIATV_OpenATV();
#endif
                break;

            case FLASH_SCREEN_ID_GAMES:
#ifdef GAME_SUPPORT
                MMIAPIGAME_OpenMainMenuWindow();
#endif
                break;

            case FLASH_SCREEN_ID_ENTERTAINMENT:
                MMIAPIMENU_EnterEntertainment();
                break;

            case FLASH_SCREEN_ID_SETTING:
                MMIAPIMENU_EnterSetting();
                break;

            case FLASH_SCREEN_ID_AUDIO:
#ifdef MMI_AUDIO_PLAYER_SUPPORT                   
                MMIAPIMP3_OpenMp3Player();
#endif
                break;
#ifdef CALENDAR_SUPPORT
            case FLASH_SCREEN_ID_CALENDAR:
                MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_CALENDAR, MMIAPICALEND_OpenMainWin);
                break;
#endif
            default:
                //SCI_TRACE_LOW:"MMIAPIFlashWin_HandleMsg id error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFLASH_WINTAB_1121_112_2_18_2_19_19_8,(uint8*)"");
                break;
        }
		break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return (recode);
}

PUBLIC BOOLEAN MMIAPIFlash_FlashIsVisiable(void)
{
	return (sFlashVisiable && (FlashTask_GetMode() == FLASHTASK_MODE_PLAY));
}

PUBLIC uint8* MMIAPIFlash_GetFlashLayerBufPtr(void)
{
    return UILAYER_GetLayerBufferPtr(&flash_layer_handle);
}





/*Edit by script, ignore 4 case. Thu Apr 26 19:00:54 2012*/ //IGNORE9527
