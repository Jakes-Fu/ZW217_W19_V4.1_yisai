/*****************************************************************************
** File Name:      mmidropdownwin_main.c                                   *
** Author:                                                                   *
** Date:           05/29/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2011      Paul.Huang              Creat
******************************************************************************/

#define _MMI_DROPDOWNWIN_WINTAB_C_

#include "mmi_app_dropdownwin_trc.h"
#ifdef PDA_UI_DROPDOWN_WIN

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "ui_layer.h"
#include "mmi_position.h"
#include "window_parse.h"
#include "guilistbox.h"
#include "guistatusbar.h"
#include "guires.h"
#include "mmidisplay_data.h"
#include "mmidropdownwin_nv.h"
#include "mmidropdownwin_image.h"
#include "mmidropdownwin_export.h"
#include "mmidropdown_position.h"
#include "mmidropdownwin_id.h"
#include "watch_sms_message.h"
#include "mmiset_export.h"

#include "mmimms_export.h"
#include "mmiset_display.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT) || (defined WEATHER_SUPPORT)
#include "mmiwidget_image.h"
#endif
#include "mmi_text.h"
#include "mmiset_text.h"
#include "mmicc_text.h"
#include "mmicc_image.h"
#include "mmiset_func.h"
#include "mmisms_text.h"
#include "mmisms_export.h"
#include "mmimp3_text.h"
#ifdef JAVA_SUPPORT
#include "java_mmi_text.h"
#endif
#include "mmiidle_export.h" 
#include "mmicl_internal.h"    
#include "mmisms_app.h"
#include "mmisms_read.h"
#include "mmk_tp.h"
#include "mmimp3_export.h"
#include "mmidropdownwin_text.h"
#include "mmiacc_text.h"
#include "mmicountedtime_export.h"
#include "mmipub.h"
#include "mmiudisk_export.h"
#include "mmisd_export.h"
#include "mmifmm_id.h"
#include "guictrl_api.h"

#ifdef MMI_PDA_SUPPORT
#include "mmi_timer_export.h"
#endif
#include "mmiidle_func.h"
#include "mmistk_export.h"

#include "mmimp3_id.h"
#include "mmicc_image.h"

#ifdef MMI_GPRS_SWITCH_SUPPORT
#include "mmiconnection_export.h"
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT 
#include "mmiset_videowallpaper.h"
#endif

#include "mmisms_receive.h"
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
#include "mmiset_id.h"
#endif

#include "mmialarm_text.h"
#include "mmialarm_image.h"
#include "mmiacc_event.h"
#include "mmischedule_export.h"

#include "mmialarm_export.h"
#include "mmi_event_api.h"

#include "mmiset_wintab.h"
#include "mmidropdownwin_export.h"
#include "zdt_app.h"

#include "ual_tele_radio.h"
#include "ual_tele_sim.h"

#ifndef ZTE_WATCH
#define DROPDOWN_USE_ZDT_SUPPORT 1
#else
#define DROPDOWN_USE_ZDT_SUPPORT 0
#endif

//#define DROPDOWN_USE_ZDT_SUPPORT 1

#if !DROPDOWN_USE_ZDT_SUPPORT 
#define DROPDOWN_BRIGHTNESS_RECT DP2PX_RECT(12,121, 228, 169)
#define DROPDOWN_VOLUME_RECT DP2PX_RECT(12,180, 228, 228)
#define DROPDOWN_PROGRESSBAR_TOTAL_LENGTH    216
#define DROPDOWN_PROGRESSBAR_BRIGHT_LENGTH 31//27
#define DROPDOWN_PROGRESSBAR_VOLUME_LENGTH 24//22
#define DROPDOWN_PROGRESSBAR_BRIGTH_LEVEL_MAX 7
#define DROPDOWN_PROGBAR_SEL_BRIGHT 1
#define DROPDOWN_PROGBAR_SEL_VOLUME 2

#define DP_BRIGHT_ICON_X	30
#define DP_BRIGHT_ICON_Y	121//129
#define DP_VOLUME_ICON_X	30
#define DP_VOLUME_ICON_Y	180//188

extern uint8 cur_ring_volume_img_id;
extern uint8 cur_bright_img_id;
extern int16 current_percent_brightness;
extern uint8 progress_down_flag;

#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
PUBLIC void MMIROPDOWN_CreateFlasglightWin();

/*****************************************************************************/
// Description : 显示信号图标
// Global resource dependence : 
// Author: wancan.you
// Note:
/*****************************************************************************/


LOCAL MMI_IMAGE_ID_T Shortcut_GetSignalInfo(char * string_str)
{
    MN_PHONE_PLMN_STATUS_E plmn_status = PLMN_NO_SERVICE;
    const MMI_IMAGE_ID_T signal_img_id[6] = {res_stat_signal_0, res_stat_signal_1, res_stat_signal_2,res_stat_signal_3, res_stat_signal_4,res_stat_signal_5};
    BOOLEAN is_fly_mode_on = FALSE;
    MN_PLMN_T cur_plmn = {0};
    BOOLEAN isGmust = FALSE;
    BOOLEAN isLTEmust = FALSE;
    BOOLEAN isRaticonable = FALSE;
    MMI_IMAGE_ID_T image_id = NULL;
    MMIPHONE_NET_SERVICE_TYPE_E  service_type = NET_SERVICE_TYPE_MAX;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    MMI_IMAGE_ID_T signal_start_id = res_stat_signal_0;

    if (is_fly_mode_on)
    {
        image_id = IMAGE_IDLE_TITLE_ICON_FLYMODE;
        sprintf(string_str, "%s", "");
    }
    else if(MMIAPIPHONE_IsSimOk(dual_sys))
    {
    #ifndef UAL_TELE_SUPPORT
        MMI_GMMREG_RAT_E network_status_rat = MMI_GMMREG_RAT_GPRS;
        network_status_rat = MMIAPIPHONE_GetTDOrGsm(dual_sys);
    #else
        ual_tele_radio_current_network_type_e network_status_rat = UAL_TELE_RADIO_CURRENT_NETWORK_TYPE_2G;
        network_status_rat = ual_tele_radio_get_current_network_type(dual_sys);
    #endif
        SCI_TRACE_LOW("Shortcut_GetSignalInfo: network_status_rat = %d", network_status_rat);
        //network_status_rat = MMI_GMMREG_RAT_3G;
        switch (network_status_rat) 
        {
            case UAL_TELE_RADIO_CURRENT_NETWORK_TYPE_2G:
            {
                sprintf(string_str, "%s", "4G");
            }
                break;
            case UAL_TELE_RADIO_CURRENT_NETWORK_TYPE_3G:
            {
                sprintf(string_str, "%s", "3G");
            }
                break;    
            case MMI_GMMREG_RAT_LTE:
            case UAL_TELE_RADIO_CURRENT_NETWORK_TYPE_4G:
            {
                sprintf(string_str, "%s", "4G");
            }
                break; 
            case MMI_GMMREG_RAT_UNKNOWN:
            case UAL_TELE_RADIO_CURRENT_NETWORK_TYPE_UNKNOWN:
            {
                sprintf(string_str, "%s", "E");
            }
               break;
            default:
            {
                sprintf(string_str, "%s", "");
            }
              break;
        }
        plmn_status = MMIAPIPHONE_GetPlmnStatus(dual_sys);
        if (PLMN_NO_SERVICE == plmn_status
            || PLMN_EMERGENCY_ONLY == plmn_status 
            || PLMN_EMERGENCY_SIM_INVALID == plmn_status
            || PLMN_EMERGENCY_GPRS_ONLY == plmn_status 
            || PLMN_EMERGENCY_SIM_INVALID_GPRS_ONLY == plmn_status
            || PLMN_REGISTER_SERVICE == plmn_status
            || PLMN_REGISTER_GPRS_ONLY == plmn_status)
        {
             signal_start_id = IMAGE_IDLE_TITLE_ICON_SIGNAL1;    
        }
        if (MMIAPIIDLE_NeedShowIconAccordingToDualSysSetting(dual_sys))
        {
            if (SIM_STATUS_PIN_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys)
                || SIM_STATUS_PUK_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys))
            {
                image_id = signal_start_id;
            }
            else
            {
                image_id = signal_img_id[MMIIDLE_GetIdleWinInfo()->rxlevel_step[dual_sys]];
            }
        }
        else
        {
            image_id = signal_start_id;
        }
    }
    else
    {
        image_id = res_stat_nosim;
        sprintf(string_str, "%s", "");
        SCI_TRACE_LOW("Shortcut_GetSignalInfo: no sim");
    }

    return image_id;
}

LOCAL MMI_IMAGE_ID_T Shortcut_GetVolteInfo()
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    MN_PHONE_PLMN_STATUS_E plmn_status = PLMN_NO_SERVICE;
    BOOLEAN is_emergency_service = FALSE;
    BOOLEAN is_ps_ready = FALSE;
    BOOLEAN is_fly_mode_on = FALSE;
    BOOLEAN is_volte_icon_on =FALSE;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

    is_fly_mode_on = MMIAPISET_GetFlyMode();
    if (is_fly_mode_on)
    {
        image_id = IMAGE_NULL;
    }
    else
    {
        if (MMIAPIPHONE_GetVolteIconDisplayStyle(dual_sys) == 1)
        {
            image_id = IMAGE_PRIMO_STATUS_VOLTE;
        }
        else
        {
            image_id = IMAGE_PRIMO_STATUS_HD;
        }
    }
#ifdef UAL_TELE_SUPPORT
    plmn_status = ual_tele_radio_get_current_plmn_state(dual_sys);
#else
    plmn_status = MMIAPIPHONE_GetPlmnStatus(dual_sys);
    is_ps_ready = MMIAPIPHONE_GetIsPsReady(dual_sys);
#endif

#ifndef UAL_TELE_SUPPORT
    if(PLMN_NO_SERVICE == plmn_status
            || PLMN_EMERGENCY_ONLY == plmn_status
            || PLMN_EMERGENCY_SIM_INVALID == plmn_status
            || PLMN_EMERGENCY_GPRS_ONLY == plmn_status
            || PLMN_EMERGENCY_SIM_INVALID_GPRS_ONLY == plmn_status
            || PLMN_REGISTER_SERVICE == plmn_status
            || PLMN_REGISTER_GPRS_ONLY == plmn_status
            || !is_ps_ready)
#else
    if(UAL_TELE_RADIO_PLMN_STATE_NORMAL != plmn_status)
#endif
    {
        is_emergency_service = TRUE;
        SCI_TRACE_LOW("[%s] is_emergency_service = %d",__FUNCTION__, is_emergency_service);
    }

    is_volte_icon_on = MMIAPIPHONE_GetVolteStatus(dual_sys);//bug2138051
    if (SIM_STATUS_PIN_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys)
            || SIM_STATUS_PUK_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys)
            || SIM_STATUS_NO_SIM == MMIAPIPHONE_GetSimStatus(dual_sys)
            || !is_volte_icon_on
            || is_emergency_service)
    {
            image_id = IMAGE_NULL;
    }
    
    if (image_id == IMAGE_NULL)
    {
        SCI_TraceLow("[%s] image_id is NULL",__FUNCTION__);
    }
    return image_id;
}

PUBLIC void ZDT_DisplaySingal(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO lcd_dev_info)
{
    MMI_IMAGE_ID_T          signal_icon_id =IMAGE_NULL;
    MMI_IMAGE_ID_T          volte_icon_id =IMAGE_NULL;
    GUI_RECT_T              signal_str_rect     = {30,4,65,40};
    GUI_RECT_T              volte_rect     = {12,10,29,40};
    GUI_RECT_T            text_str_rect     = {27,2,65,40};
    char                 * str_char = "";
    wchar                str_wchar[10] = {0};
    MMI_STRING_T        txt_string = {0};
    str_char = (char *)SCI_ALLOC_APPZ(10);
    signal_icon_id = Shortcut_GetSignalInfo(str_char);
    volte_icon_id = Shortcut_GetVolteInfo();
    if(signal_icon_id != IMAGE_NULL)
    {
        signal_str_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, signal_str_rect);
        GUIRES_DisplayImg(PNULL,&signal_str_rect,PNULL,win_id,signal_icon_id,&lcd_dev_info);
    }
    if(volte_icon_id != IMAGE_NULL)
    {
        volte_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, volte_rect);
        GUIRES_DisplayImg(PNULL,&volte_rect,PNULL,win_id,volte_icon_id,&lcd_dev_info);
    }
    
    if(0 != strcmp(str_char, ""))
    {
        GUISTR_STATE_T txt_state = GUISTR_STATE_ALIGN;
        GUISTR_STYLE_T txt_style = {0};
        
        txt_style.align = ALIGN_LEFT;
        if(0 == strcmp(str_char, "GPRS")){
            txt_style.font = SONG_FONT_10;
        }else{
            txt_style.font = SONG_FONT_12;
        }
        txt_style.font_color = MMI_WHITE_COLOR;
        text_str_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, text_str_rect);
        GUI_GBToWstr(str_wchar, str_char, strlen(str_char));
        txt_string.wstr_ptr = str_wchar;
        txt_string.wstr_len = MMIAPICOM_Wstrlen(str_wchar);
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &text_str_rect,
            &text_str_rect,
            &txt_string,
            &txt_style,
            txt_state,
            GUISTR_TEXT_DIR_AUTO
            );
        SCI_FREE(str_char);
    }
    else{
        SCI_FREE(str_char);
    }
    
}

PUBLIC void ZDT_DisplayBattery(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO lcd_dev_info)
{

    uint32    icon_index = 0;
    BOOLEAN is_charge = 0;
    uint32    battery = 0;
    uint32    charge_icon_id[11] ={
                                    res_stat_battery_0,
                                    res_stat_battery_10,
                                    res_stat_battery_20,
                                    res_stat_battery_30,
                                    res_stat_battery_40,
                                    res_stat_battery_50,
                                    res_stat_battery_60,
                                    res_stat_battery_70,
                                    res_stat_battery_80,
                                    res_stat_battery_90,
                                    res_stat_battery_100};
    
    MMI_IMAGE_ID_T          bat_icon_id =IMAGE_NULL;
    GUI_RECT_T              bat_icon_rect     = {185,7,215,30};
    GUI_RECT_T              bat_str_rect     = {185,7,215,30};
    GUI_RECT_T              charging_str_rect     = {168,8,190,40};
    uint8 battery_level = MMIAPIPHONE_GetBatCapacity();
    
    is_charge = MMIIDLE_GetIdleWinInfo()->is_charge;
    bat_icon_id = charge_icon_id[battery_level];
    SCI_TRACE_LOW("DisplayBattry: is_charge = %d", is_charge);
    //is_charge = TRUE;
    if(!is_charge)
    {
        if(bat_icon_id != IMAGE_NULL)
        {
            GUIRES_DisplayImg(PNULL,&bat_icon_rect,PNULL,win_id,bat_icon_id, &lcd_dev_info);
        }
    }
    else
    {
        if(bat_icon_id != IMAGE_NULL) 
        {
           GUIRES_DisplayImg( PNULL, &bat_icon_rect, PNULL, win_id, res_stat_battery_charing,&lcd_dev_info);
        }
        
    }  
}

extern YX_WEATHER_INFO_T g_yx_wt_info;
LOCAL void DisplayWeather(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO  lcd_dev_info = {0};
    GUI_POINT_T point = {0};
    const uint16 img_width = 82;
    const uint16 img_height = 82;
    MMI_STRING_T    display_string    = {0};
    wchar    wstr_buf[MMIPHONE_MAX_OPER_NAME_LEN]    = {0};
    MMI_IMAGE_ID_T weather_img_id = IMAGE_WEATHER_UNKNOWN;

    uint16 qing_wstr[2] = {0x6674, 0};//qing
    uint16 yin_wstr[2] = {0x9634, 0};//yin
    uint16 yu_wstr[2] = {0x96E8, 0};//yu
    uint16 xue_wstr[2] = {0x96EA, 0};//xue


    GUI_RECT_T    wt_rect={150,70,240,100};
    GUI_RECT_T    temperature_rect={125,110,(MMI_MAINSCREEN_WIDTH -1),160};   
    GUI_RECT_T    city_rect={150,40,(MMI_MAINSCREEN_WIDTH -20),70};


    GUI_RECT_T    text_rect={0};
    MMI_STRING_T tmp_string = {0};
    MMI_STRING_T        content_str = {0};    
    wchar               content_text[100 + 1] = {0};
    wchar               content_text_1[100 + 1] = {0};    
    uint16 uint16_str_len = 0;
    GUISTR_STYLE_T      text_style = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE| GUISTR_STATE_ELLIPSIS;
    MMI_TEXT_ID_T weather_string_id;
    char*           temp_ptr = PNULL;
    uint16 temp_len = 0;

    uint8 temp_str[32] = {0};
    uint8 temp_wstr[32] = {0};

    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    
    //location
    point.x = 120;
    point.y = 40 ;
    GUIRES_DisplayImg(&point,PNULL,PNULL,win_id, IMAGE_WEATHER_LOCATION,&lcd_dev_info);
    //city
    tmp_string.wstr_ptr = g_yx_wt_info.city_name;
    tmp_string.wstr_len = (uint16) MMIAPICOM_Wstrlen (tmp_string.wstr_ptr);
    text_style.align = ALIGN_LEFT;
    text_style.font = SONG_FONT_24;
    text_style.font_color = MMI_WHITE_COLOR;
    GUISTR_DrawTextToLCDInRect( 
                &lcd_dev_info,
                (const GUI_RECT_T      *)&city_rect,       //the fixed display area
                (const GUI_RECT_T      *)&city_rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&tmp_string,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );    

    //weather icon  
    switch(g_yx_wt_info.num)
    {
        case 1:
            weather_img_id = IMAGE_BIG_WEATHER_SUNNY;
            weather_string_id=TXT_WEATHER_SUNNY;
            break;
        case 2:
            weather_img_id = IMAGE_BIG_WEATHER_CLOUD;
            weather_string_id=TXT_WEATHER_RAIN;            
            break;
        case 3:
            weather_img_id = IMAGE_BIG_WEATHER_RAIN;
            weather_string_id=TXT_WEATHER_CLOUD;                    
            break;
        case 4:
            weather_img_id = IMAGE_BIG_WEATHER_SNOW;
            weather_string_id=TXT_WEATHER_SNOW;                    
            break;
        default:
            weather_img_id = IMAGE_BIG_WEATHER_UNKNOW;
            weather_string_id=TXT_WEATHER_UNKNOW;                    
            break;                
    }
 
    point.x = 30;
    point.y = 40 ;
    GUIRES_DisplayImg(&point,PNULL,PNULL,win_id, weather_img_id,&lcd_dev_info); 
    

    //weather text
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)weather_string_id,&tmp_string);
    text_style.align = ALIGN_LEFT;
    text_style.font_color = MMI_WHITE_COLOR;
    text_style.font = SONG_FONT_32;
    GUISTR_DrawTextToLCDInRect( 
                &lcd_dev_info,
                (const GUI_RECT_T      *)&wt_rect,       //the fixed display area
                (const GUI_RECT_T      *)&wt_rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&tmp_string,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );    
    
    //temperature
    memset(temp_str,0,sizeof(temp_str));
    sprintf((char*) temp_str, "%d", g_yx_wt_info.cur_degree);
    MMIAPICOM_StrcatFromStrToUCS2(content_text, &uint16_str_len, temp_str, strlen(temp_str));
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_WEATHER_TEMPERATURE_DU,&tmp_string);
    MMIAPICOM_StrcatFromSTRINGToUCS2(content_text, &uint16_str_len, &tmp_string);
    text_style.font = SONG_FONT_40;    
    text_style.align = ALIGN_LEFT;
    text_style.font_color = MMI_YELLOW_COLOR;
    content_str.wstr_ptr = content_text;
    content_str.wstr_len = MMIAPICOM_Wstrlen(content_text);
    GUISTR_DrawTextToLCDInRect( 
                &lcd_dev_info,
                (const GUI_RECT_T      *)&temperature_rect,       //the fixed display area
                (const GUI_RECT_T      *)&temperature_rect,       //用户要剪切的实际区域
                (const MMI_STRING_T    *)&content_str,
                &text_style,
                state,
                GUISTR_TEXT_DIR_AUTO
                );
}

PUBLIC void ZDT_DisplayLoginIcon(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO lcd_dev_info)
{
#ifdef ZDT_PLAT_YX_SUPPORT
    if(TRUE == YX_Net_Is_Land())
    {
        GUI_POINT_T point = {0};
        point.x = 68;
        point.y = 10;
        GUIRES_DisplayImg(&point,PNULL,PNULL,win_id, IMAGE_IDLE_TITLE_ICON_YX,&lcd_dev_info); 
    }
#endif
}

PUBLIC void ZDT_DisplayDataInOutIcon(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO lcd_dev_info)
{
    if(MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_1))
    {
        GUI_POINT_T point = {0};
        point.x = 48;
        point.y = 4;
        GUIRES_DisplayImg(&point,PNULL,PNULL,win_id, res_stat_date_inout,&lcd_dev_info);
    }
}

PUBLIC void ZDT_DisplayStatusbar(MMI_WIN_ID_T win_id,GUI_LCD_DEV_INFO lcd_dev_info)
{
    ZDT_DisplayBattery(win_id,lcd_dev_info);
    ZDT_DisplaySingal(win_id,lcd_dev_info);
    ZDT_DisplayDataInOutIcon(win_id, lcd_dev_info);
    ZDT_DisplayLoginIcon(win_id, lcd_dev_info);
}

LOCAL void DisplayBottomButton(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T tools_rect = DROPDOWN_TOOLS_RECT;
    GUI_RECT_T sound_rect = DROPDOWN_SOUND_RECT;
    GUI_RECT_T settings_rect = DROPDOWN_SETTINGS_RECT;
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    GUIRES_DisplayImg(PNULL,&tools_rect,PNULL,win_id, IMAGE_DROPDOWN_TOOL,&lcd_dev_info);
    GUIRES_DisplayImg(PNULL,&sound_rect,PNULL,win_id, IMAGE_DROPDOWN_SOUND_OPEN,&lcd_dev_info);
    GUIRES_DisplayImg(PNULL,&settings_rect,PNULL,win_id, IMAGE_DROPDOWN_SETTINGS,&lcd_dev_info);
}

/* 
add by fys 2023/10/04
*/
#if !DROPDOWN_USE_ZDT_SUPPORT 
LOCAL void DisplayTopButton(MMI_WIN_ID_T win_id)
{
	GUI_LCD_DEV_INFO lcd_dev_info = {0};
    uint8 i = 0;
    MMI_IMAGE_ID_T img_id[] = {statusbar_flashlight,statusbar_ring_and_vibrate,statusbar_long_battery};

	GUI_RECT_T btn_rect = {DROPDOWN_TOP_TOOLS_BG_MARGIN_LEFT, DROPDOWN_TOP_TOOLS_BG_MARGIN_TOP
        , DROPDOWN_TOP_TOOLS_BG_MARGIN_LEFT+DROPDOWN_TOP_TOOLS_BG_WIDTH, DROPDOWN_TOP_TOOLS_BG_MARGIN_TOP+DROPDOWN_TOP_TOOLS_BG_HEIGHT};
	GUI_RECT_T btn_img_rect = {DROPDOWN_TOP_TOOLS_IMG_MARGIN_LEFT, DROPDOWN_TOP_TOOLS_IMG_MARGIN_TOP
        , DROPDOWN_TOP_TOOLS_IMG_MARGIN_LEFT+DROPDOWN_TOP_TOOLS_IMG_WIDTH, DROPDOWN_TOP_TOOLS_IMG_MARGIN_TOP+DROPDOWN_TOP_TOOLS_BG_HEIGHT};

	MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
	
    for(i; i<3; i++)
    {
        LCD_FillRoundedRect(&lcd_dev_info, btn_rect, btn_rect, GUI_RGB2RGB565(0x24, 0x24, 0x24));
	    GUIRES_DisplayImg(PNULL, &btn_img_rect, PNULL, win_id, img_id[i],&lcd_dev_info);
        btn_rect.left += DROPDOWN_TOP_TOOLS_BG_WIDTH + DROPDOWN_TOP_TOOLS_BG_OFFSET;
	    btn_rect.right += DROPDOWN_TOP_TOOLS_BG_OFFSET + DROPDOWN_TOP_TOOLS_BG_WIDTH;
	    btn_img_rect.left += DROPDOWN_TOP_TOOLS_IMG_WIDTH + DROPDOWN_TOP_TOOLS_OFFSET;
	    btn_img_rect.right += DROPDOWN_TOP_TOOLS_OFFSET + DROPDOWN_TOP_TOOLS_IMG_WIDTH;
    }	
}

LOCAL void DisplayBrightnessBar(MMI_WIN_ID_T win_id)
{
	GUI_LCD_DEV_INFO lcd_dev_info = {0};
	GUI_RECT_T left_bar_rect = {12, 120, 32, 168};
	GUI_RECT_T right_bar_rect = {208, 120, 228, 168};
	GUI_RECT_T mid_bar_rect = {36, 120, 204, 168};
	uint8 cur_value = 0;

	MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

	LCD_FillArc(&lcd_dev_info, &left_bar_rect, 36, 144, 24, 90, 270, GUI_RGB2RGB565(0x24,0x24,0x24));
	GUI_FillRect(&lcd_dev_info, mid_bar_rect, GUI_RGB2RGB565(0x24, 0x24, 0x24));
	LCD_FillArc(&lcd_dev_info, &right_bar_rect, 204, 144, 24, 270, 90, GUI_RGB2RGB565(0x24,0x24,0x24));

	cur_value = MMIAPISET_GetCurrentContrast();
	SCI_TRACE_LOW("%s: cur_value = %d", __FUNCTION__, cur_value);
	if(cur_value == 1)
	{
		LCD_FillArc(&lcd_dev_info, &left_bar_rect, 36, 144, 24, 90, 270, GUI_RGB2RGB565(0x0b,0x84,0xff));
	}
	else if(cur_value > 6)
	{
		LCD_FillArc(&lcd_dev_info, &left_bar_rect, 36, 144, 24, 90, 270, GUI_RGB2RGB565(0x0b,0x84,0xff));
		GUI_FillRect(&lcd_dev_info, mid_bar_rect, GUI_RGB2RGB565(0x0b,0x84,0xff));
		LCD_FillArc(&lcd_dev_info, &right_bar_rect, 204, 144, 24, 270, 90, GUI_RGB2RGB565(0x0b,0x84,0xff));
	}
	else
	{
		LCD_FillArc(&lcd_dev_info, &left_bar_rect, 36, 144, 24, 90, 270, GUI_RGB2RGB565(0x0b,0x84,0xff));
		mid_bar_rect.right = 36 + 33 * (cur_value - 1);
		GUI_FillRect(&lcd_dev_info, mid_bar_rect, GUI_RGB2RGB565(0x0b,0x84,0xff));
	}
}

LOCAL void DisplayVolumeBar(MMI_WIN_ID_T win_id)
{
	GUI_LCD_DEV_INFO lcd_dev_info = {0};
	GUI_RECT_T left_bar_rect = {12, 180, 32, 228};
	GUI_RECT_T right_bar_rect = {208, 180, 228, 228};
	GUI_RECT_T mid_bar_rect = {36, 180, 204, 228};
	uint8 cur_value = 0;

	MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
	
	LCD_FillArc(&lcd_dev_info, &left_bar_rect, 36, 204, 24, 90, 270, GUI_RGB2RGB565(0x24,0x24,0x24));
	GUI_FillRect(&lcd_dev_info, mid_bar_rect, GUI_RGB2RGB565(0x24, 0x24, 0x24));
	LCD_FillArc(&lcd_dev_info, &right_bar_rect, 204, 204, 24, 270, 90, GUI_RGB2RGB565(0x24,0x24,0x24));

	cur_value = MMIAPISET_GetCallRingVolume();
	SCI_TRACE_LOW("%s: cur_value = %d", __FUNCTION__, cur_value);
	if(cur_value == 0)
	{

	}
	else if(cur_value < 9)
	{
		LCD_FillArc(&lcd_dev_info, &left_bar_rect, 36, 204, 24, 90, 270, GUI_RGB2RGB565(0x0b,0x84,0xff));
		mid_bar_rect.right = 36 + 21 * (cur_value - 1);
		GUI_FillRect(&lcd_dev_info, mid_bar_rect, GUI_RGB2RGB565(0x0b,0x84,0xff));
	}
	else
	{
		LCD_FillArc(&lcd_dev_info, &left_bar_rect, 36, 204, 24, 90, 270, GUI_RGB2RGB565(0x0b,0x84,0xff));
		GUI_FillRect(&lcd_dev_info, mid_bar_rect, GUI_RGB2RGB565(0x0b,0x84,0xff));
		LCD_FillArc(&lcd_dev_info, &right_bar_rect, 204, 204, 24, 270, 90, GUI_RGB2RGB565(0x0b,0x84,0xff));
	}
}
#endif
LOCAL void ZDT_DropDownWin_DisplayBg(MMI_WIN_ID_T        win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T win_rect = MMIDROPDOWNWIN_RECT;
    GUI_RECT_T weather_rect = {15,28,225,144};
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    GUI_FillRect(&lcd_dev_info, win_rect, MMIDROPDOWNWIN_BGCOLOR);
	#if DROPDOWN_USE_ZDT_SUPPORT != 0
    LCD_FillRoundedRect(&lcd_dev_info, weather_rect,weather_rect, 0x1082);
    #endif
}

LOCAL void ZDT_DropDownWin_DisplayFg(MMI_WIN_ID_T        win_id)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

	GUI_RECT_T progressrect = {12,121, 228, 169};
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    ZDT_DisplayStatusbar(win_id, lcd_dev_info);
#if DROPDOWN_USE_ZDT_SUPPORT != 0
    DisplayWeather(win_id);
    DisplayBottomButton(win_id);
#else
	DisplayTopButton(win_id);
	//DisplayBrightnessBar(win_id);
	//WATCHCOM_ProgressBar(win_id, progressrect,30,res_progress_bg,res_progress_fg,NULL);
	//DisplayVolumeBar(win_id);	
#endif
}

LOCAL void HandleTpPressUp(MMI_WIN_ID_T win_id, GUI_POINT_T point)
{
	GUI_RECT_T btn_img_rect = {DROPDOWN_TOP_TOOLS_IMG_MARGIN_LEFT, DROPDOWN_TOP_TOOLS_IMG_MARGIN_TOP
        , DROPDOWN_TOP_TOOLS_IMG_MARGIN_LEFT+DROPDOWN_TOP_TOOLS_IMG_WIDTH, DROPDOWN_TOP_TOOLS_IMG_MARGIN_TOP+DROPDOWN_TOP_TOOLS_BG_HEIGHT};
    uint8 i=0;
    for(i; i<3; i++)
    {
        if(GUI_PointIsInRect(point, btn_img_rect))
        {
            break;
        }
        btn_img_rect.left += DROPDOWN_TOP_TOOLS_IMG_WIDTH + DROPDOWN_TOP_TOOLS_OFFSET;
	    btn_img_rect.right += DROPDOWN_TOP_TOOLS_OFFSET + DROPDOWN_TOP_TOOLS_IMG_WIDTH;
    }
    switch(i)
    {
        case 0:
            MMIROPDOWN_CreateFlasglightWin();
            break;
        case 1:
            Settings_NoteTypeSelectWin_Enter();
            break;
        case 2:
            Watch_LongRangeMode_switch();
            break;
    }
}

LOCAL void CheckTouchUp(GUI_POINT_T point)
{
    GUI_RECT_T tools_rect = DROPDOWN_TOOLS_RECT;
    GUI_RECT_T sound_rect = DROPDOWN_SOUND_RECT;
    GUI_RECT_T settings_rect = DROPDOWN_SETTINGS_RECT;
    if (GUI_PointIsInRect(point, tools_rect))
    {
        MMIAPISET_ZdtBrightnessWin();
    }
    else if(GUI_PointIsInRect(point, sound_rect))
    {
        MMIAPISET_ZdtVolumeWin();
    }
    else if(GUI_PointIsInRect(point, settings_rect))
    {
        WatchSET_MainWin_Enter();
    }
}


PUBLIC void MMIZDT_DropDown_CloseWin()
{
    MMK_CloseWin(MMIZDT_DROPDOWN_WIN_ID);
}

#if !DROPDOWN_USE_ZDT_SUPPORT 
LOCAL uint8 Get_DropDownProg_CurrentIndex(uint16 current_percent,uint16 progress_length,uint16 per_length, uint8 maxlevel)
{
	uint8 level_index = (current_percent * progress_length)/100/per_length;
	if(current_percent == 0)
		level_index = 0;
	else
		level_index++;
	if(level_index >maxlevel)
		level_index =maxlevel;
	return level_index;
}
#endif


#ifndef ROUND
#define ROUND(f) (int)((f)>0.0? (f+0.5):(f-0.5))
#endif

LOCAL MMI_RESULT_E HandleDropDownWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        )
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;

	#if !DROPDOWN_USE_ZDT_SUPPORT     
    GUI_RECT_T progressrect = DROPDOWN_BRIGHTNESS_RECT;
	GUI_RECT_T progress_volume_rect = DROPDOWN_VOLUME_RECT;
	uint8 cur_value = 0;
	MMI_IMAGE_ID_T         res_progress_bg = IMAGE_ZTE_DROP_PROGRESS_BG;
	MMI_IMAGE_ID_T         res_progress_fg= IMAGE_ZTE_DROP_PROGRESS;
	MMI_IMAGE_ID_T         res_progress_hand = NULL;
	GUI_POINT_T   display_point = {0};
	GUI_LCD_DEV_INFO lcd_dev_info = {0};
	uint32      cur_item_index = 0;
	uint32  volume_item_total = MMISET_VOL_MAX+1;
	MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
	#endif 
    SCI_TRACE_LOW("NotifyWin_HandleWinMsg(), msg_id = %0X.", msg_id);
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
#if !DROPDOWN_USE_ZDT_SUPPORT 
			current_percent_brightness = 0;
			progress_down_flag = 0;
			cur_bright_img_id = MMIAPISET_GetCurrentContrast();
			if (MMISET_CONTRAST_SEVEN < cur_bright_img_id)
				cur_bright_img_id = MMISET_CONTRAST_SEVEN;
			WATCHCOM_ProgressBarByIndex(win_id, progressrect, cur_bright_img_id,MMISET_CONTRAST_SEVEN,res_progress_bg, res_progress_fg, res_progress_hand);
			/*if(cur_bright_img_id ==0)
			 current_percent_brightness = 0;
		 else
			 current_percent_brightness = (cur_bright_img_id)*DROPDOWN_PROGRESSBAR_BRIGHT_LENGTH*100/DROPDOWN_PROGRESSBAR_TOTAL_LENGTH;
		 WATCHCOM_ProgressBar(win_id, progressrect, current_percent_brightness,res_progress_bg, res_progress_fg, res_progress_hand);*/
			cur_ring_volume_img_id = MMIAPISET_GetCallRingVolume();

			if(MMISET_VOL_MAX < cur_ring_volume_img_id)
				cur_ring_volume_img_id = MMISET_VOL_MAX;
			WATCHCOM_ProgressBarByIndex(win_id, progress_volume_rect, cur_ring_volume_img_id,volume_item_total,res_progress_bg, res_progress_fg, res_progress_hand);
			/* if(cur_ring_volume_img_id ==0)
			 current_percent_brightness = 0;
		 else
			 current_percent_brightness = (cur_ring_volume_img_id)*DROPDOWN_PROGRESSBAR_VOLUME_LENGTH*100/DROPDOWN_PROGRESSBAR_TOTAL_LENGTH;
	
		WATCHCOM_ProgressBar(win_id, progress_volume_rect,current_percent_brightness,res_progress_bg,res_progress_fg,res_progress_hand);*/
#endif 
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            break;
        }

        case MSG_FULL_PAINT:
        {
           ZDT_DropDownWin_DisplayBg(win_id);
            ZDT_DropDownWin_DisplayFg(win_id);
		//WATCHCOM_ProgressBar(win_id, progressrect, current_percent_brightness,res_progress_bg, res_progress_fg, res_progress_hand);
#if !DROPDOWN_USE_ZDT_SUPPORT 
		/*if(cur_bright_img_id ==0)
			 current_percent_brightness = 0;
		 else
			 current_percent_brightness = (cur_bright_img_id)*DROPDOWN_PROGRESSBAR_BRIGHT_LENGTH*100/DROPDOWN_PROGRESSBAR_TOTAL_LENGTH;
		 WATCHCOM_ProgressBar(win_id, progressrect, current_percent_brightness,res_progress_bg, res_progress_fg, res_progress_hand);*/
		 WATCHCOM_ProgressBarByIndex(win_id, progressrect, cur_bright_img_id,MMISET_CONTRAST_SEVEN,res_progress_bg, res_progress_fg, res_progress_hand);
		 display_point.x = DP_BRIGHT_ICON_X; // 43*BIG_SCALE;//81*SCALE;//client_rect.v_rect.left;
		display_point.y = DP_BRIGHT_ICON_Y; //  14*BIG_SCALE;//25*SCALE;///client_rect.v_rect.top;	

		GUIRES_DisplayImg(&display_point,
		    PNULL,
		    PNULL,
		    win_id,
		    statusbar_brightness,
		    &lcd_dev_info);
		 
		//cur_ring_volume_img_id = MMIAPISET_GetCallRingVolume();
		 /*if(cur_ring_volume_img_id ==0)
			 current_percent_brightness = 0;
		 else
			 current_percent_brightness = (cur_ring_volume_img_id)*DROPDOWN_PROGRESSBAR_VOLUME_LENGTH*100/DROPDOWN_PROGRESSBAR_TOTAL_LENGTH;
	
		WATCHCOM_ProgressBar(win_id, progress_volume_rect,current_percent_brightness,res_progress_bg,res_progress_fg,res_progress_hand);*/
		WATCHCOM_ProgressBarByIndex(win_id, progress_volume_rect, cur_ring_volume_img_id,volume_item_total,res_progress_bg, res_progress_fg, res_progress_hand);
		display_point.x = DP_VOLUME_ICON_X; // 43*BIG_SCALE;//81*SCALE;//client_rect.v_rect.left;
		display_point.y = DP_VOLUME_ICON_Y; //  14*BIG_SCALE;//25*SCALE;///client_rect.v_rect.top;	

		GUIRES_DisplayImg(&display_point,
		    PNULL,
		    PNULL,
		    win_id,
		    statusbar_volume,
		    &lcd_dev_info);
#endif
            break;
        }

        case MSG_CTL_MIDSK:
        case MSG_CTL_PENOK:
        {
            break;
        }
#if !DROPDOWN_USE_ZDT_SUPPORT 
	case MSG_TP_PRESS_DOWN:
	{
	        GUI_POINT_T   point = {0};
		
	        point.x = MMK_GET_TP_X(param);
	        point.y = MMK_GET_TP_Y(param);
		/* if (GUI_PointIsInRect(point, progressrect))
		{
			//uint8 percent = (point.x - rect.left)*100/(rect.right-rect.left);
			progress_down_flag = DROPDOWN_PROGBAR_SEL_BRIGHT;	
			current_percent_brightness = (point.x - progressrect.left)*100/(progressrect.right-progressrect.left);
			WATCHCOM_ProgressBar(win_id, progressrect, current_percent_brightness,res_progress_bg, res_progress_fg, res_progress_hand);
			cur_bright_img_id = Get_DropDownProg_CurrentIndex(current_percent_brightness,DROPDOWN_PROGRESSBAR_TOTAL_LENGTH,DROPDOWN_PROGRESSBAR_BRIGHT_LENGTH,MMISET_CONTRAST_SEVEN);
			MMIAPISET_SetCurrentContrast(cur_bright_img_id);      
			MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID, cur_bright_img_id);
			MMIAPISET_SetMainLCDContrast();
			MMK_UpdateScreen();			
		} 
		 else if(GUI_PointIsInRect(point,progress_volume_rect))
		 {
		 	progress_down_flag = DROPDOWN_PROGBAR_SEL_VOLUME;	
			current_percent_brightness = (point.x - progress_volume_rect.left)*100/(progress_volume_rect.right-progress_volume_rect.left);
			WATCHCOM_ProgressBar(win_id, progressrect, current_percent_brightness,res_progress_bg, res_progress_fg, res_progress_hand);
			cur_ring_volume_img_id = Get_DropDownProg_CurrentIndex(current_percent_brightness,DROPDOWN_PROGRESSBAR_TOTAL_LENGTH,DROPDOWN_PROGRESSBAR_VOLUME_LENGTH,MMISET_VOL_MAX);
			
			MMK_UpdateScreen();
		 }*/
		 if (GUI_PointIsInRect(point, progressrect))
		 {
			 progress_down_flag = DROPDOWN_PROGBAR_SEL_BRIGHT;	
			cur_item_index =  ROUND((float)(MMISET_CONTRAST_SEVEN)
			                      *(point.x - progressrect.left)
			                      /(progressrect.right-progressrect.left)); //0 is not in total num, eg total=100, progressbar display 0~100
			if(cur_item_index != cur_bright_img_id)
			{
				cur_bright_img_id = cur_item_index;
				WATCHCOM_ProgressBarByIndex(win_id, progressrect, cur_bright_img_id,MMISET_CONTRAST_SEVEN,res_progress_bg, res_progress_fg, res_progress_hand);
				MMIAPISET_SetCurrentContrast(cur_bright_img_id);      
				MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID, cur_bright_img_id);
				MMIAPISET_SetMainLCDContrast();
				MMK_UpdateScreen();
			}
		 }
		 else if(GUI_PointIsInRect(point,progress_volume_rect))
		 {
			 progress_down_flag = DROPDOWN_PROGBAR_SEL_VOLUME;
			cur_item_index =  ROUND((float)(volume_item_total)
			                      *(point.x - progress_volume_rect.left)
			                      /(progress_volume_rect.right-progress_volume_rect.left)); //0 is not in total num, eg total=100, progressbar display 0~100
			if(cur_item_index != cur_ring_volume_img_id)
			{
				cur_ring_volume_img_id = cur_item_index;
				WATCHCOM_ProgressBarByIndex(win_id, progress_volume_rect, cur_ring_volume_img_id,volume_item_total,res_progress_bg, res_progress_fg, res_progress_hand);
				MMK_UpdateScreen();
			}
		 }
			
	    }	
	      break;	

	  case MSG_TP_PRESS_MOVE:
		{			
			GUI_POINT_T   point = {0};              
			point.x = MMK_GET_TP_X(param);
			point.y = MMK_GET_TP_Y(param);
			if (DROPDOWN_PROGBAR_SEL_BRIGHT == progress_down_flag /*&& GUI_PointIsInRect(point, progressrect)*/)
			{
				//uint8 percent = (point.x - rect.left)*100/(rect.right-rect.left);
				/*current_percent_brightness= (point.x - progressrect.left)*100/(progressrect.right-progressrect.left);
				WATCHCOM_ProgressBar(win_id, progressrect, current_percent_brightness,res_progress_bg, res_progress_fg, NULL);
				cur_bright_img_id = Get_DropDownProg_CurrentIndex(current_percent_brightness,DROPDOWN_PROGRESSBAR_TOTAL_LENGTH,DROPDOWN_PROGRESSBAR_BRIGHT_LENGTH,MMISET_CONTRAST_SEVEN);*/
				cur_item_index =  ROUND((float)(MMISET_CONTRAST_SEVEN)
                                      *(point.x - progressrect.left)
                                      /(progressrect.right-progressrect.left)); //0 is not in total num, eg total=100, progressbar display 0~100
	               	if(cur_item_index != cur_bright_img_id)
	               	{
				   	cur_bright_img_id = cur_item_index;
					WATCHCOM_ProgressBarByIndex(win_id, progressrect, cur_bright_img_id,MMISET_CONTRAST_SEVEN,res_progress_bg, res_progress_fg, res_progress_hand);
					MMIAPISET_SetCurrentContrast(cur_bright_img_id);      
					MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID, cur_bright_img_id);
					MMIAPISET_SetMainLCDContrast();
					MMK_UpdateScreen();
	               	}				
			}
			else if (DROPDOWN_PROGBAR_SEL_VOLUME == progress_down_flag /*&& GUI_PointIsInRect(point, progressrect)*/)
			{
				//uint8 percent = (point.x - rect.left)*100/(rect.right-rect.left);
				/*current_percent_brightness = (point.x - progress_volume_rect.left)*100/(progress_volume_rect.right-progress_volume_rect.left);
				WATCHCOM_ProgressBar(win_id, progressrect, current_percent_brightness,res_progress_bg, res_progress_fg, NULL);
				cur_ring_volume_img_id = Get_DropDownProg_CurrentIndex(current_percent_brightness,DROPDOWN_PROGRESSBAR_TOTAL_LENGTH,DROPDOWN_PROGRESSBAR_VOLUME_LENGTH,MMISET_VOL_MAX);
				MMK_UpdateScreen();*/
				cur_item_index =  ROUND((float)(volume_item_total)
                                      *(point.x - progress_volume_rect.left)
                                      /(progress_volume_rect.right-progress_volume_rect.left)); //0 is not in total num, eg total=100, progressbar display 0~100
				if(cur_item_index != cur_ring_volume_img_id)
				{
					cur_ring_volume_img_id = cur_item_index;
					WATCHCOM_ProgressBarByIndex(win_id, progress_volume_rect, cur_ring_volume_img_id,volume_item_total,res_progress_bg, res_progress_fg, res_progress_hand);
					MMK_UpdateScreen();
				}				
			}
			else
				progress_down_flag = 0;
			}
			break;
#endif
	case MSG_TP_PRESS_UP:
		{
            GUI_POINT_T   point = {0};              
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
		#if !DROPDOWN_USE_ZDT_SUPPORT
			//CheckTouchUp(point);
			if(DROPDOWN_PROGBAR_SEL_VOLUME == progress_down_flag)
			{
				MMIAPISET_SetCallRingVolume(cur_ring_volume_img_id);     
				//Settings_Volume_SoundUpdate(cur_ring_volume_img_id);
			}
			else
				HandleTpPressUp(win_id,point);
			progress_down_flag = 0;
			//HandleTpPressUp(win_id,point);
		#else
			CheckTouchUp(point);
		#endif
			
            break;
        }
         	break;

        case MSG_LOSE_FOCUS:
        {
            MMK_CloseWin(win_id);
            break;
        }

        case MSG_KEYLONG_HASH:
        {
            MMIZDT_DropDown_CloseWin();
            break;
        }

        case MSG_KEYDOWN_5:
        {
            MMIZDT_DropDown_CloseWin();
            break;
        }
        //bug2143162
        case MSG_SLIDEWIN_END:
        {
            GUIWIN_SetStbDropDownState(win_id, FALSE);
        }
        break;
        case MSG_SLIDEWIN_BEGIN:
        {
            GUIWIN_SetStbDropDownState(win_id, TRUE);
        }
        break;
        case MSG_KEYDOWN_RED:
            break;      
        case MSG_KEYUP_RED:
        case MSG_KEYDOWN_WEB:
        case MSG_KEYDOWN_CANCEL:
            MMK_CloseWin(win_id);
        break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return result;
}

WINDOW_TABLE( MMIZDT_DROPDOWN_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleDropDownWinMsg),    
    WIN_ID( MMIZDT_DROPDOWN_WIN_ID),    
    WIN_HIDE_STATUS,
    WIN_MOVE_STYLE(MOVE_NOT_FULL_SCREEN_WINDOW),
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
    END_WIN
};

/*****************************************************************************
//  Description : start DropDown win
//  Param: launch_mode ------ auto start / tp start
//  Return: NONE
//  Note:
*****************************************************************************/
PUBLIC void MMIZDT_DropDown_EnterWin(eSlideWinStartUpMode launch_mode)
{
    GUI_RECT_T roiRect = MMIDROPDOWNWIN_ROIREGION;
    MMI_WIN_ID_T win_id = MMIZDT_DROPDOWN_WIN_ID;
    MMI_WIN_ID_T old_id = MMK_GetFocusWinId();

    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    MMK_CreateWin(MMIZDT_DROPDOWN_WIN_TAB, NULL);
#if 1
    do
    {
        uint32 handle = 0;
        tSlideWinInitData init = {0};
        init.win_id = win_id;
        init.win_before_id = old_id;
        init.direction = SLIDEWIN_DIRECTION_VERT;
        init.mode = launch_mode;
        init.style = SLIDEWIN_STYLE_ENABLE_ROI | SLIDEWIN_STYLE_ENABLE_RANGE | SLIDEWIN_STYLE_ENABLE_FREE_WHEN_WINCLOSE;
        init.init_pos = MMIDROPDOWNWIN_INIT_POS;
        init.stop_pos = MMIDROPDOWNWIN_STOP_POS;
        init.close_pos = MMIDROPDOWNWIN_END_POS;
        init.range_min = MMIDROPDOWNWIN_INIT_POS;
        init.range_max = 0;
        init.roi_region = roiRect;
        handle = WatchSLIDEWIN_Create(&init);

        WatchSLIDEWIN_Start(handle);
    }while (0);
#endif
}

LOCAL MMI_RESULT_E HandleFlashlightWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
	GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMIAPISET_SetCurrentContrast(9);      
			MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID, 9);
			MMIAPISET_SetMainLCDContrast();
        }
		break;
        case MSG_FULL_PAINT:
        {
            GUI_RECT_T win_rect = MMITHEME_GetFullScreenRect();
			GUI_FillRect(&lcd_dev_info, win_rect, MMI_WHITE_COLOR);
        }
		break;
		case MSG_APP_CANCEL:
		case MSG_CTL_CANCEL:
        case MSG_KEYUP_RED:
		{
			MMK_CloseWin(win_id);
		}
		break;
        case MSG_KEYDOWN_RED:
            break;
		case MSG_CLOSE_WINDOW:
		{
			extern int16 s_setting_brightness_value;
			MMIAPISET_SetCurrentContrast(s_setting_brightness_value);
			MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID, s_setting_brightness_value);
			MMIAPISET_SetMainLCDContrast();
		}
		break;
    }
	return result;
}

WINDOW_TABLE(MMI_DROPDOWN_FLASHLIGHT_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleFlashlightWinMsg), 
    WIN_ID( MMI_DROPDOWN_FLASHLIGHT_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

PUBLIC void MMIROPDOWN_CreateFlasglightWin()
{
	MMI_WIN_ID_T win_id = MMI_DROPDOWN_FLASHLIGHT_WIN_ID;
	MMI_HANDLE_T win_handle = 0;
	GUI_RECT_T rect = MMITHEME_GetFullScreenRect();
	if (MMK_IsOpenWin(win_id))
	{
		MMK_CloseWin(win_id);
	}

	win_handle = MMK_CreateWin((uint32*)MMI_DROPDOWN_FLASHLIGHT_WIN_TAB, NULL);
	MMK_SetWinRect(win_handle, &rect);
}

#endif //PDA_UI_DROPDOWN_WIN