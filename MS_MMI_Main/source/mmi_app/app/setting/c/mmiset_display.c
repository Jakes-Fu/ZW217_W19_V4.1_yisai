/*****************************************************************************
** File Name:      mmiset_display.c                                          *
** Author:                                                                   *
** Date:           03/06/2004                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2004       Jassmine.Meng       Create
******************************************************************************/

#define _MMISET_DISPLAY_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "guilcd.h"
#include "guianim.h"
#include "guilistbox.h"
#include "mmidisplay_data.h"
#include "mmi_textfun.h"
#include "mmi_image.h"
#include "mmiidle_export.h"
#include "mmiset_display.h"
#include "mmiset_nv.h"
#include "mmiset_id.h"
#include "mmi_nv.h"
#include "mmiset_text.h"
#include "mmiset_wintab.h"
#include "mmiset_image.h"
#include "mmiset_func.h"
#include "mmiset_internal.h"
#include "mmi_wallpaper_export.h"
#include "mmiudisk_export.h"
//#include "guires.h"
#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT) || (defined QBTHEME_SUPPORT)
#include "mmiwidget.h"
#include "mmiwidget_export.h"
#endif

#if (defined QBTHEME_SUPPORT)
#include "mmiqbtheme_idleexport.h"
#endif

#include "mmi_resource.h"
#include "guibutton.h" //for pclint warning
#include "guisetlist.h"
#if defined(MMI_WIFI_SUPPORT) && defined(CMCC_UI_STYLE)
#include "mmiwlan_text.h"
#endif
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmi_livewallpaper_text.h"
#include "mmilivewallpaper_export.h"
#endif
#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
//#include "mmiset_lock_effect.h"
#include "mmiset_lock_effect_internal.h"
#endif
#ifdef PDA_UI_SUPPORT_MANIMENU_GO
#include "mmi_mainmenu_export.h"
#endif
#ifdef PDA_UI_SUPPORT_U_IDLE
#include "mmigrid_uidle_se.h"
#endif
#include "mmitheme_mainmenu.h"

#ifdef ADULT_WATCH_SUPPORT
#include "watch_launcher_multe_function_dial.h"
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMISET_LCD_CONTRAST_SPAN            14 //屏幕亮度调节的step
#define MMISET_LCD_CONTRSAT_MIN             2  /*lint !e793*/ //2允许的最小屏幕亮度值
#define MMISET_LCD_CONTRSAT_MAX             100//允许的最大的屏幕亮度值

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
// extern const MMISET_FIXED_ANIM_WALLPAPER_T g_fixed_anim_wallpaper[];
extern void LCD_SetBackLightBrightness( uint32  brightless );
//extern uint16 g_power_animate_index;

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
uint16                              g_cur_wallpaper_index       =   0;
// uint16                              g_cur_anim_wallpaper_page   =   0;

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
static uint8                        s_cur_contrast      =   0;			//current lcd contrast
static MMISET_BACKLIGHT_SET_T      g_backlight_info     =   {0};		//back light info
static uint32                       s_real_contrast     =   0;	//real lcd contrast
static MMISET_IDLE_DISPLAY_T       s_idle_display_info  =   {0};	//idle display info

LOCAL MMISET_IDLE_STYLE_E       s_mmiset_idle_style     =   MMISET_IDLE_STYLE_SMART;
LOCAL MMISET_IDLE_SUBSTYLE_E       s_mmiset_idle_substyle     =   MMISET_IDLE_SUBSTYLE_DEFAULT;
LOCAL MMIMAINMENU_TYPE_E           s_mainmenu_style = MMIMAINMENU_MATRIX_E;
LOCAL MMISET_3D_STYLE_T         s_mmi_3d_style = {0};
#ifdef PDA_UI_SUPPORT_MANIMENU_GO
LOCAL GO_MENU_STYLE_E           s_go_menu_style = GO_MENU_STYLE_RANDOM;
#endif
LOCAL MMISET_TIME_DISPLAY_TYPE_E s_time_display_type = MMISET_TIME_24HOURS;
LOCAL MMISET_TIME_DISPLAY_TYPE_E s_clock_display_style = MMISET_CLOCK_STYLE_ANALOG;
#ifdef PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER
LOCAL MMISET_EFFECT_RENDER_SETTING  s_render_setting = {0};
#endif
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef SET_3D_STYLE_SUPPORT
/*****************************************************************************/
// 	Description : 判断是否有效NV 3D style（在NV被异常破坏的情况下，恢复默认值）
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsValid3DStyle(
                             MMISET_3D_STYLE_T     *mmi_3d_style_ptr
                             );

/*****************************************************************************/
// 	Description : 得到NV 3D style
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL MMISET_3D_STYLE_T* Get3DStyle(void);

/*****************************************************************************/
// 	Description : 设置NV 3D style
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void Set3DStyle(
                      MMISET_3D_STYLE_T *mmi_3d_style_ptr
                      );
/*****************************************************************************/
// 	Description : init 3d style
//	Global resource dependence : 
//  Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL void  Init3DStyle(void);
/*****************************************************************************/
// 	Description : 切换3D其他特效的开启/关闭情况
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void Set3DStyleList(uint16    cur_sel);
#endif
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
/*****************************************************************************/
// 	Description : 设置3D style mainmenu的类型
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void Set3DStyleMainmenu(
                              uint16    cur_sel
                              );
#endif
/*****************************************************************************/
// 	Description : set idle win display mode
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
//LOCAL void SetIdleDisplaytMode(
//                               MMISET_IDLE_DISPLAY_T    mode_set
//                                );

/*****************************************************************************/
// 	Description : init the information about wallpaper set
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL void InitBacklightInfo(void);

/*****************************************************************************/
// 	Description : init the time display type
//	Global resource dependence : none
//  Author: SCV
//	Note:
/*****************************************************************************/
LOCAL void InitTimeDisplayType(void);

LOCAL void InitClockDisplayStyle( void );

/*****************************************************************************/
// 	Description : init idle display style
//	Global resource dependence : none
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void InitIdleStyle(void);

/*****************************************************************************/
// 	Description : init idle display mode
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL void InitIdleDisplayMode(void);


/*****************************************************************************/
// 	Description : get contrast of sub LCD
//	Global resource dependence : none
//  Author: scv
//	Note:
/*****************************************************************************/
// LOCAL uint8 GetSubLCDContrast(void);

/*****************************************************************************/
// 	Description : update contrast of LCD
//	Global resource dependence : none
//  Author: scv
//	Note:
/*****************************************************************************/
//LOCAL uint16 GetSubLCDContrastValue(
//                                    uint8  lcd_id,     //the id of lcd
//                                    uint8  lcd_contrast//the contrast of lcd
//                                     );

/*****************************************************************************/
// 	Description : set time display type 
//	Global resource dependence : none
//  Author: SCV
//	Note:
/*****************************************************************************/
LOCAL void SetTimeDisplayType(
                              MMISET_TIME_DISPLAY_TYPE_E    time_display_type
                              );

/*****************************************************************************/
// 	Description : init the information about wallpaper set
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL void InitWallpaperInfo(void);

#ifdef LCD_SLIDE_SUPPORT

/*****************************************************************************/
// 	Description : set slide open type 
//	Global resource dependence : none
//  Author: jianhui.luo
//	Note:
/*****************************************************************************/
LOCAL void SetSlideOpenType(
                              MMISET_SLIDE_OPEN_TYPE_E slide_open_type
                              );

/*****************************************************************************/
// 	Description : set slide close type 
//	Global resource dependence : none
//  Author: jianhui.luo
//	Note:
/*****************************************************************************/
LOCAL void SetSlideCloseType(
                              MMISET_SLIDE_CLOSE_TYPE_E slide_close_type
                              );

/*****************************************************************************/
// 	Description : set keylock switch type 
//	Global resource dependence : none
//  Author: jianhui.luo
//	Note:
/*****************************************************************************/
LOCAL void SetKeylockSwitchType(
                              MMISET_KEYLOCK_SWITCH_TYPE_E keylock_switch_type
                              );

#endif
#ifdef MMI_RES_ORIENT_BOTH
/*****************************************************************************/
// 	Description : 设置g_sensor切换特效
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void SetGsensorSwitchAnimStyle(
                                     uint32 g_sensor_anim
                                     );
#endif
#if defined(PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER) 
/*****************************************************************************/
// 	Description : init the render effect 
//	Global resource dependence : 
//  Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL void InitLightShadowRenderSetting(void);
#endif

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : init display set
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_DisplayInit(void)
{
    uint8                           mainlcd_contrast        =   MMISET_CONTRAST_FIVE;  
#ifdef TTS_SUPPORT
    BOOLEAN                         is_tts_on               =   TRUE;  	
#endif
	
	MMIMAINMENU_TYPE_E                 mainmenu_style          =   MMISET_DEFAULT_MENU_STYLE_E;	

    MN_RETURN_RESULT_E              return_value            =   MN_RETURN_FAILURE;
    MMISET_TVOUT_TYPE_E             tvout_mode              =   MMISET_PAL_LINE_BY_LINE;
#ifdef SCREENSAVER_SUPPORT    
    MMISET_SCREENSAVER_INFO_T       ssaver_info             =   {0};
#endif
    MMISET_POWER_ON_OFF_ANIM_INFO_T power_on_off_anim_info  =   {0};  //@xingdong.li
    
    //init main menu style
    MMINV_READ(MMINV_SET_MAIN_MENU_STYLE,&mainmenu_style,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_SET_MAIN_MENU_STYLE,&mainmenu_style);
    }
#ifdef ADULT_WATCH_SUPPORT
    s_mainmenu_style = MMIMAINMENU_LIST_E;
#else
    s_mainmenu_style = mainmenu_style;
#endif
    //初始化时间显示格式
    InitTimeDisplayType();

    //init clock style
    InitClockDisplayStyle();

#ifdef ADULT_WATCH_SUPPORT
    //init clock style
	InitMTFDisplayItemType();
#endif
    MMISET_InitDateDisplayType();
    #ifdef SET_3D_STYLE_SUPPORT
    //init 3D style
    Init3DStyle();
    #endif
    
    //init idle style
    InitIdleStyle(); 
    
    //init the information about wallpaper set
    InitWallpaperInfo();
    
    //init the information about backlight
    InitBacklightInfo();
    
    //init the contrast of mainlcd
    MMINV_READ(MMINV_SET_MAINLCD_CONTRAST,&mainlcd_contrast,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_SET_MAINLCD_CONTRAST,&mainlcd_contrast);
    }
    //set gloable variable
    s_cur_contrast = mainlcd_contrast;
    if(RESTART_BY_ALARM != POWER_GetRestartCondition())
    {
        //set contrast
        //MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID,mainlcd_contrast);
    }
    else //cr57279
    {
        s_real_contrast = mainlcd_contrast*MMISET_LCD_CONTRAST_SPAN + MMISET_LCD_CONTRSAT_MIN;
    }

    //init idle win display mode
#ifdef MMISET_IDLE_DISPLAY_SUPPORT
    InitIdleDisplayMode();
#endif

#ifdef SCREENSAVER_SUPPORT    
    //init screen saver info
    MMINV_READ(MMINV_SCREENSAVER_SETTING_INFO, &ssaver_info,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        //@xingdong.li
        ssaver_info.is_open           = FALSE;
        ssaver_info.ssaver_type       = MMISET_SCREENSAVER_CLOSED;
        
        MMINV_WRITE(MMINV_SCREENSAVER_SETTING_INFO, &ssaver_info);
    }
#endif   
    //init power on off animation info  //@xingdong.li
    MMINV_READ(MMINV_POWER_ON_ANIM_SETTING_INFO, &power_on_off_anim_info, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        power_on_off_anim_info.is_open      = TRUE;
        power_on_off_anim_info.anim_type    = MMISET_POWER_ON_OFF_ANIM_DEFAULT; 
        MMINV_WRITE(MMINV_POWER_ON_ANIM_SETTING_INFO, &power_on_off_anim_info);
    }
    MMINV_READ(MMINV_POWER_OFF_ANIM_SETTING_INFO, &power_on_off_anim_info, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        power_on_off_anim_info.is_open      = TRUE;
        power_on_off_anim_info.anim_type    = MMISET_POWER_ON_OFF_ANIM_DEFAULT; 
        MMINV_WRITE(MMINV_POWER_OFF_ANIM_SETTING_INFO, &power_on_off_anim_info);
    }   

#ifdef TTS_SUPPORT
    //init tts status barry.peng
    MMINV_READ(MMINV_SET_TTS_ON_OFF,&is_tts_on,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_tts_on = TRUE;
        MMINV_WRITE(MMINV_SET_TTS_ON_OFF,&is_tts_on);
    }
#endif

    //init tv out mode 
    MMINV_READ(MMINV_SET_TV_OUT_MODE,&tvout_mode,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        tvout_mode = MMISET_PAL_LINE_BY_LINE;
        MMINV_WRITE(MMINV_SET_TV_OUT_MODE,&tvout_mode);
    }

#ifdef PDA_UI_SUPPORT_MANIMENU_GO
    //init GO menu style from nv
    MMINV_READ(MMINV_SET_GO_MENU_STYLE,&s_go_menu_style,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        s_go_menu_style = GO_MENU_STYLE_RANDOM;
        MMINV_WRITE(MMINV_SET_GO_MENU_STYLE,&s_go_menu_style);
    }
#endif
#if defined(PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER) 
    InitLightShadowRenderSetting();
#endif
}
/*****************************************************************************/
// 	Description : factory reset the idle display mode 
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_ResetIdleDisplayMode(void)
{
    //set initial value
    s_idle_display_info.visible_flag.item_flag.is_show_time_and_date = TRUE;
    s_idle_display_info.visible_flag.item_flag.is_show_network_name = TRUE;
#if defined MMI_MULTI_SIM_SYS_TRI ||  defined MMI_MULTI_SIM_SYS_QUAD ||  defined MMI_MULTI_SIM_SYS_DUAL                                                             
    s_idle_display_info.visible_flag.item_flag.is_show_network_name2 = TRUE;
#endif        
#if defined MMI_MULTI_SIM_SYS_TRI ||  defined MMI_MULTI_SIM_SYS_QUAD                                                           
    s_idle_display_info.visible_flag.item_flag.is_show_network_name3 = TRUE;
#endif        
#if defined MMI_MULTI_SIM_SYS_QUAD                                                           
    s_idle_display_info.visible_flag.item_flag.is_show_network_name4 = TRUE;
#endif        
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
#if !(defined(MMI_DISPLAYSET_MINI_SUPPORT) && defined(MMI_ENABLE_DCD)) 
    s_idle_display_info.visible_flag.item_flag.is_show_idle_words = FALSE;		
#endif
#endif
    #if defined(MMI_WIFI_SUPPORT) && defined(CMCC_UI_STYLE)
    s_idle_display_info.item_flag.is_show_wlan_option = TRUE;
    #endif
    s_idle_display_info.visible_flag.item_flag.is_show_today_schedule = TRUE;
#ifdef COUNTEDTIME_SUPPORT
	s_idle_display_info.visible_flag.item_flag.is_show_countdown = FALSE;
#endif
#ifdef MMI_AUDIO_PLAYER_SUPPORT
	s_idle_display_info.visible_flag.item_flag.is_show_mp3 = FALSE;
#endif
#ifdef FM_SUPPORT
	s_idle_display_info.visible_flag.item_flag.is_show_fm = FALSE;
#endif
#ifdef MMI_IDLE_STKTEXT_SUPPORT
	s_idle_display_info.visible_flag.item_flag.is_show_stk_idle_text = FALSE;
#endif
#ifdef DCD_SUPPORT
	s_idle_display_info.visible_flag.item_flag.is_show_dcd  = FALSE;
#endif
#ifdef MMI_WIFI_SUPPORT
	s_idle_display_info.visible_flag.item_flag.is_show_wlan = FALSE;
#endif
    SCI_MEMSET(s_idle_display_info.idle_words, 0X00, sizeof(s_idle_display_info.idle_words));
    s_idle_display_info.wstr_len = 0;
    MMINV_WRITE(MMINV_SET_IDLEDISPLAY_INFO,&s_idle_display_info);    

}
/*****************************************************************************/
// 	Description : set the selected type of time display 
//	Global resource dependence : none
//  Author: SCV
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetSelectTimeDisplyType(
                                           MMI_CTRL_ID_T    ctrl_id
                                           )
{
    uint16      cur_selection   =   0;
    MMISET_TIME_DISPLAY_TYPE_E time_display_type = MMISET_TIME_12HOURS;
    
#ifdef DPHONE_SUPPORT 
    GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,sizeof(cur_selection));
#else    
    cur_selection = GUISETLIST_GetCurIndex(ctrl_id);
#endif 
     if (0 == cur_selection)
    {
        time_display_type = MMISET_TIME_12HOURS;
    }
    else
    {
        time_display_type = MMISET_TIME_24HOURS;
    }
    SetTimeDisplayType(time_display_type);
}
 
/*****************************************************************************/
// 	Description : get the type of time display type
//	Global resource dependence : none
//  Author: SCV
//	Note:
/*****************************************************************************/
PUBLIC MMISET_TIME_DISPLAY_TYPE_E MMIAPISET_GetTimeDisplayType(void)
{
    SCI_TRACE_LOW("MMIAPISET_GetTimeDisplayType %d",s_time_display_type);
    return s_time_display_type;
}
/*****************************************************************************/
// 	Description : set time display type 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetTimeDisplayType(MMISET_TIME_DISPLAY_TYPE_E time_display_type)
{
    SetTimeDisplayType(time_display_type);
}

/*****************************************************************************/
//  Description : get the type of clock display type
//  Global resource dependence : none
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC MMISET_CLOCK_DISPLAY_STYLE_E MMIAPISET_GetClockDisplayStyle( void )
{
    SCI_TRACE_LOW("MMIAPISET_GetClockDisplayType %d",s_clock_display_style);
    return s_clock_display_style;
}

/*****************************************************************************/
//  Description : set clock display type
//  Global resource dependence : none
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetClockDisplayStyle( MMISET_CLOCK_DISPLAY_STYLE_E clock_display_style )
{
    s_clock_display_style = clock_display_style;
    MMINV_WRITE(MMINV_SET_CLOCK_DISPLAY_STYLE, &s_clock_display_style);
}

/*****************************************************************************/
// 	Description : set time display type 
//	Global resource dependence : none
//  Author: SCV
//	Note:
/*****************************************************************************/
LOCAL void SetTimeDisplayType(
                              MMISET_TIME_DISPLAY_TYPE_E time_display_type
                              )
{
    s_time_display_type = time_display_type;
    MMINV_WRITE(MMINV_SET_TIME_DISPLAY_TYPE, &s_time_display_type);
}
/*****************************************************************************/
// 	Description : init the time display type
//	Global resource dependence : none
//  Author: SCV
//	Note:
/*****************************************************************************/
LOCAL void InitTimeDisplayType(void)
{
    MN_RETURN_RESULT_E          return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_TIME_DISPLAY_TYPE, &s_time_display_type, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        s_time_display_type= MMISET_TIME_24HOURS;
        MMINV_WRITE(MMINV_SET_TIME_DISPLAY_TYPE, &s_time_display_type);
    }
}

LOCAL void InitClockDisplayStyle( void )
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_CLOCK_DISPLAY_STYLE, &s_clock_display_style, return_value);
    if ( MN_RETURN_SUCCESS != return_value )
    {
        s_clock_display_style = MMISET_CLOCK_STYLE_DIGIT;
        MMINV_WRITE(MMINV_SET_CLOCK_DISPLAY_STYLE, &s_clock_display_style);
    }
}

/*****************************************************************************/
// 	Description : init idle display style
//	Global resource dependence : none
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void InitIdleStyle(void)
{
    MN_RETURN_RESULT_E      return_value    =   MN_RETURN_FAILURE;
    MMISET_IDLE_STYLE_E     idle_style      =   MMISET_IDLE_STYLE_SMART;
    MMISET_IDLE_SUBSTYLE_E     idle_substyle      =   MMISET_IDLE_SUBSTYLE_DEFAULT;
    
#ifdef MMIWIDGET_SUPPORT
    idle_style      =   MMISET_IDLE_STYLE_WIDGET;
#endif
    
#if !defined(MMI_ANDROID_SUPPORT) && defined(MMI_ISTYLE_SUPPORT)
    idle_style = MMISET_IDLE_ISTYLE;
#elif defined MMI_GRID_IDLE_SUPPORT
#ifdef BAR_PHONE_WIDGET 
    #if defined MMI_SMART_IDLE_SUPPORT_LARGE_ICON || defined MMI_SMART_IDLE_SUPPORT_SMALL_ICON
        idle_style  = MMISET_IDLE_STYLE_SMART;
    #elif defined MMI_COMMON_IDLE_SUPPORT
        idle_style  = MMISET_IDLE_STYLE_COMMON;
    #endif
#else
        idle_style  = MMISET_IDLE_STYLE_GRID;
#endif
#endif
    
    MMINV_READ(MMINV_SET_IDLE_STYLE, &idle_style, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
#if defined(DPHONE_SUPPORT)
        idle_style = MMISET_IDLE_STYLE_COMMON;
#else
        
#if !defined(MMI_ANDROID_SUPPORT) && defined(MMI_ISTYLE_SUPPORT)
        idle_style = MMISET_IDLE_ISTYLE;
#elif defined MMI_GRID_IDLE_SUPPORT
#ifdef BAR_PHONE_WIDGET 
    #if defined MMI_SMART_IDLE_SUPPORT_LARGE_ICON || defined MMI_SMART_IDLE_SUPPORT_SMALL_ICON
        idle_style  = MMISET_IDLE_STYLE_SMART;
    #elif defined MMI_COMMON_IDLE_SUPPORT
        idle_style  = MMISET_IDLE_STYLE_COMMON;
    #endif
#else
      idle_style  = MMISET_IDLE_STYLE_GRID;
#endif
#elif defined MMIWIDGET_SUPPORT
        idle_style = MMISET_IDLE_STYLE_WIDGET; 
#elif defined (MMI_COMMON_IDLE_SUPPORT) 
        idle_style = MMISET_IDLE_STYLE_COMMON;
#else
        idle_style = MMISET_IDLE_STYLE_SMART;
#endif        
#endif
#ifdef CMCC_UI_STYLE
        idle_style = MMISET_IDLE_STYLE_SMART;
#endif
        MMINV_WRITE(MMINV_SET_IDLE_STYLE,&idle_style);
    }


#ifdef BAR_PHONE_WIDGET 
    {
    #if defined MMI_SMART_IDLE_SUPPORT_LARGE_ICON || defined MMI_SMART_IDLE_SUPPORT_SMALL_ICON
        MMISET_IDLE_STYLE_E pre_idle_style  = MMISET_IDLE_STYLE_SMART;
    #elif defined MMI_COMMON_IDLE_SUPPORT
        MMISET_IDLE_STYLE_E pre_idle_style  = MMISET_IDLE_STYLE_COMMON;
    #endif

        MMINV_READ(MMINV_SET_IDLE_FORMER_STYLE, &pre_idle_style, return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            MMINV_WRITE(MMINV_SET_IDLE_FORMER_STYLE,&pre_idle_style);
        }
    }
#endif

    s_mmiset_idle_style = idle_style;

    //substyle
    MMINV_READ(MMINV_SET_IDLE_SUBSTYLE, &idle_substyle, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        idle_substyle = MMISET_IDLE_SUBSTYLE_DEFAULT;
        MMINV_WRITE(MMINV_SET_IDLE_SUBSTYLE,&idle_substyle);       
    }
    s_mmiset_idle_substyle = idle_substyle;	
}

/*****************************************************************************/
// 	Description : init idle display mode
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL void InitIdleDisplayMode(void)
{
    MN_RETURN_RESULT_E          return_value        =   MN_RETURN_FAILURE;
    
    //get idle display info from nv
    MMINV_READ(MMINV_SET_IDLEDISPLAY_INFO,&s_idle_display_info,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        //set initial value
        s_idle_display_info.visible_flag.item_flag.is_show_time_and_date = TRUE;
        s_idle_display_info.visible_flag.item_flag.is_show_network_name = TRUE;
#if defined MMI_MULTI_SIM_SYS_TRI ||  defined MMI_MULTI_SIM_SYS_QUAD ||  defined MMI_MULTI_SIM_SYS_DUAL                                                             
        s_idle_display_info.visible_flag.item_flag.is_show_network_name2 = TRUE;
#endif        
#if defined MMI_MULTI_SIM_SYS_TRI ||  defined MMI_MULTI_SIM_SYS_QUAD                                                           
        s_idle_display_info.visible_flag.item_flag.is_show_network_name3 = TRUE;
#endif        
#if defined MMI_MULTI_SIM_SYS_QUAD                                                           
        s_idle_display_info.visible_flag.item_flag.is_show_network_name4 = TRUE;
#endif      
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
#if !(defined(MMI_DISPLAYSET_MINI_SUPPORT) && defined(MMI_ENABLE_DCD)) 
        s_idle_display_info.visible_flag.item_flag.is_show_idle_words = FALSE;	
#endif
#endif
        s_idle_display_info.visible_flag.item_flag.is_show_today_schedule = TRUE;
#ifdef MMI_AUDIO_PLAYER_SUPPORT
        s_idle_display_info.visible_flag.item_flag.is_show_mp3 = FALSE;
#endif
#ifdef FM_SUPPORT
        s_idle_display_info.visible_flag.item_flag.is_show_fm = FALSE;
#endif
#if defined(MMI_WIFI_SUPPORT) && defined(CMCC_UI_STYLE)
        s_idle_display_info.visible_flag.item_flag.is_show_wlan_option = TRUE;
 #endif
#ifdef COUNTEDTIME_SUPPORT
	s_idle_display_info.visible_flag.item_flag.is_show_countdown = FALSE;
#endif
#ifdef MMI_IDLE_STKTEXT_SUPPORT
	s_idle_display_info.visible_flag.item_flag.is_show_stk_idle_text = FALSE;
#endif
#ifdef DCD_SUPPORT
	s_idle_display_info.visible_flag.item_flag.is_show_dcd  = FALSE;
#endif
#ifdef MMI_WIFI_SUPPORT
	s_idle_display_info.visible_flag.item_flag.is_show_wlan = FALSE;
#endif
        SCI_MEMSET(s_idle_display_info.idle_words, 0X00, sizeof(s_idle_display_info.idle_words));
        s_idle_display_info.wstr_len = 0;
        MMINV_WRITE(MMINV_SET_IDLEDISPLAY_INFO,&s_idle_display_info);    
    }
}

/*****************************************************************************/
// 	Description : init the information about wallpaper set
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL void InitBacklightInfo(void)
{
    MN_RETURN_RESULT_E          return_value    =   MN_RETURN_FAILURE;
    MMISET_BACKLIGHT_SET_T      backlight_info  =   {0};
    
    MMINV_READ(MMINV_SET_BACKLIGHT_INFO,&backlight_info,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        //set initial value
        backlight_info.backlight_type = MMISET_BACKLIGHT_OPEN;
        backlight_info.backlight_time = MMISET_BACKLIGHT_DEFAULT_TIME;
        
        MMINV_WRITE(MMINV_SET_BACKLIGHT_INFO,&backlight_info);
    }
    g_backlight_info.backlight_type = backlight_info.backlight_type;
    g_backlight_info.backlight_time = backlight_info.backlight_time;//temp modify
}

PUBLIC MMISET_BACKLIGHT_SET_T WATCHAPISET_GetBackLightinfo()
{
    return g_backlight_info;
}
/*****************************************************************************/
// 	Description : init the information about wallpaper set
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
LOCAL void InitWallpaperInfo(void)
{
    MMISET_WALLPAPER_SET_T      wallpaper_info  =   {0};
           
    //init the information about wallpaper set
    //MMINV_READ(MMINV_SET_WALLPAPER_INFO,&wallpaper_info,return_value);
    if(!MMIAPISET_GetWallpaperNVByStyle(&wallpaper_info))
    {
        //set initial value
        wallpaper_info.wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;
#ifdef MMI_GRID_IDLE_SUPPORT
        if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
        {
            wallpaper_info.wallpaper_id = MMISET_WALLPAPER_DEFAULT_ID ; 
        }
        else
#endif
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT 
        if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
            wallpaper_info.wallpaper_id = MMISET_WALLPAPER_DEFAULT_ID + 1;
        }
        else
#endif
        {
            wallpaper_info.wallpaper_id = MMISET_WALLPAPER_DEFAULT_ID;
        }

        //MMINV_WRITE(MMINV_SET_WALLPAPER_INFO,&wallpaper_info);
        if(!MMISET_SetWallpaperNVByStyle(&wallpaper_info))
        {
            return;
        }
    }
} 

/*****************************************************************************/
// 	Description : get information about wallpaper set
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC MMISET_IDLE_DISPLAY_T * MMIAPISET_GetIdleDisplayMode( void)
{
    return &s_idle_display_info;
}

/*****************************************************************************/
// 	Description : get information about wallpaper set
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_GetWallpaperSetInfo(
                                          MMISET_WALLPAPER_SET_T    *wallpaper_ptr
                                          )
{

    BOOLEAN return_value = FALSE;
    MMISET_WALLPAPER_SET_T  wallpaper_info  =   {0};
    
    //SCI_ASSERT(PNULL != wallpaper_ptr);
    if(PNULL == wallpaper_ptr)
   {
   	//SCI_TRACE_LOW:"mmisetting_assert MMIAPISET_GetWallpaperSetInfo"
   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_644_112_2_18_2_50_52_138,(uint8*)"");
	return;
   }
       
    //get the information about wallpaper set from NV
    //MMINV_READ(MMINV_SET_WALLPAPER_INFO,wallpaper_ptr,return_value);
    return_value = MMIAPISET_GetWallpaperNVByStyle(wallpaper_ptr);
    
    if (wallpaper_ptr->wallpaper_type >= MMISET_WALLPAPER_MAX || !return_value )
    {
        //set initial value
        wallpaper_ptr->wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;
#ifdef MMI_GRID_IDLE_SUPPORT
        if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
        {
            wallpaper_ptr->wallpaper_id = MMISET_WALLPAPER_DEFAULT_ID; 
        }
        else
#endif
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT 
        if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
            wallpaper_ptr->wallpaper_id = MMISET_WALLPAPER_DEFAULT_ID + 1;
        }
        else
#endif
        {
            wallpaper_ptr->wallpaper_id = MMISET_WALLPAPER_DEFAULT_ID;   
        }
     
        //MMINV_WRITE(MMINV_SET_WALLPAPER_INFO,wallpaper_ptr);
        if(!MMISET_SetWallpaperNVByStyle(&wallpaper_info))
        {
            return;
        }
    }

     
}

/*****************************************************************************/
// 	Description : set information about wallpaper set
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetWallpaperSetInfo(
                                          MMISET_WALLPAPER_SET_T  wallpaper_info//the info abput wallpaper
                                          )
{  
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
    //无论如何先销毁旧动态墙纸;
    if(MMIAPIIDLE_IsLiveWallpaper())
    {
        MMIAPILIVEWALLPAPER_Stop();
        MMIAPILIVEWALLPAPER_Destroy(TRUE);
    }
#endif

    //write information about wallpaper set into NV
    //MMINV_WRITE(MMINV_SET_WALLPAPER_INFO,&wallpaper_info);
    if(!MMISET_SetWallpaperNVByStyle(&wallpaper_info))
    {
        return;
    }
    
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
    //然后创建新动态墙纸;
    if(MMIAPIIDLE_IsLiveWallpaper())
    {
        MMIAPILIVEWALLPAPER_Init();
    }
#endif

    //update wallpaper
#if defined MMI_WALLPAPER_SUPPORT
    MMIAPIIDLE_SetWallpaper(TRUE);
#endif
}

/*****************************************************************************/
// 	Description : reset default wallpaper
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ResetDefaultWallpaper(void)
{
    MMISET_WALLPAPER_SET_T  wallpaper_info  =   {0};
    //reset wallpaper
    if (!MMIAPIUDISK_UdiskIsRun())
    {
        wallpaper_info.wallpaper_id     =   MMIAPISET_GetDefaultWallpaperIDByStyle();
        wallpaper_info.wallpaper_type   =   MMISET_WALLPAPER_FIXED_PIC;
    }    
    else
    {
        MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
    }
    MMIAPISET_SetWallpaperSetInfo(wallpaper_info);
}

/*****************************************************************************/
// 	Description : get information about backlight set
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_GetBackLightInfo(
                                       MMISET_BACKLIGHT_SET_T     *backlight_ptr
                                       )
{
    //SCI_ASSERT(PNULL != backlight_ptr);
    if(PNULL == backlight_ptr)
   {
   	//SCI_TRACE_LOW:"mmisetting_assert MMIAPISET_GetBackLightInfo"
   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_742_112_2_18_2_50_52_139,(uint8*)"");
	return;
   }
      
    //get the information about back light info
    *backlight_ptr = g_backlight_info;
}

/*****************************************************************************/
// 	Description : set information about backlight set
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetBackLightInfo(
                                    MMISET_BACKLIGHT_SET_T     backlight_info//the info about backlight
                                    )
{
    //write information about backlight set into NV
    MMINV_WRITE(MMINV_SET_BACKLIGHT_INFO,&backlight_info);
    
    //set gloable variable
    g_backlight_info = backlight_info;
}

/*****************************************************************************/
// 	Description : get contrast of main LCD
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetMainLCDContrast(void)
{
    uint8                   mainlcd_contrast    =   MMISET_CONTRAST_FIVE;
    MN_RETURN_RESULT_E      return_value        =   MN_RETURN_FAILURE;
    
    //get the contrast of mainlcd from NV
    MMINV_READ(MMINV_SET_MAINLCD_CONTRAST,&mainlcd_contrast,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_SET_MAINLCD_CONTRAST,&mainlcd_contrast);
    }
    
    return (mainlcd_contrast);
}

/*****************************************************************************/
// 	Description : set contrast of main LCD
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetMainLCDContrast(void)
{
    //write the contrast of mainlcd into NV
    MMINV_WRITE(MMINV_SET_MAINLCD_CONTRAST,&s_cur_contrast);
}


/*****************************************************************************/
// 	Description : update contrast of LCD
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_UpdateLCDContrast(
                                        uint8  lcd_id,     //the id of lcd
                                        uint8  lcd_contrast//the contrast of lcd
                                        )
{
    uint32      real_contrast    =   0;
    
    real_contrast = lcd_contrast*MMISET_LCD_CONTRAST_SPAN + MMISET_LCD_CONTRSAT_MIN;
    
    s_real_contrast = real_contrast;
    
    if (MMISET_LCD_CONTRSAT_MAX < s_real_contrast)
    {
        //SCI_TRACE_LOW:"MMIAPISET_UpdateLCDContrast:s_real_contrast = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_812_112_2_18_2_50_52_140,(uint8*)"d",s_real_contrast);
        s_real_contrast = MMISET_LCD_CONTRSAT_MAX;
    }
    
#ifndef WIN32
    LCD_SetBackLightBrightness(s_real_contrast);
#endif
}
/*****************************************************************************/
// 	Description : 24小时制显示时间
//	Global resource dependence : none
//  Author: SCV
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_GetTimeStr24(uint8    * timestr)
{
    SCI_TIME_T      time    =   {0};
    if (PNULL == timestr)
    {
        MMI_ASSERT_WARNING("PNULL != timestr");/*assert verified*/
        return;
    }
    
    TM_GetSysTime(&time);
    sprintf((char*)timestr,"%02d:%02d",
        time.hour,
        time.min
        );
}

/*****************************************************************************/
// 	Description : 12小时制显示时间
//	Global resource dependence : none
//  Author: SCV
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_GetTimeStr12(uint8    * timestr)
{
    SCI_TIME_T      time    =   {0};
    
    if (PNULL == timestr)
    {
        MMI_ASSERT_WARNING("PNULL != timestr");/*assert verified*/
        return;
    }
    
    TM_GetSysTime(&time);
    if ((MMISET_TIME_PM_HOUR > time.hour))//AM
    {
        if ((MMISET_TIME_1_HOUR> time.hour))//00:00-1:00
        {
            sprintf((char*)timestr,"%02d:%02d%s",(time.hour + MMISET_TIME_PM_HOUR),time.min,"AM");
        }
        else
        {
            sprintf((char*)timestr,"%02d:%02d%s",time.hour,time.min,"AM");
        }
    }
    else//PM
    {
        if ((MMISET_TIME_PM_HOUR <= time.hour) && (MMISET_TIME_13_HOUR > time.hour))//12:00-13:00
        {
            sprintf((char*)timestr, "%02d:%02d%s", time.hour, time.min, "PM");
        }
        else
        {
            sprintf((char*)timestr, "%02d:%02d%s", (time.hour - MMISET_TIME_PM_HOUR), time.min, "PM");
        }
    }
}

/*****************************************************************************/
// 	Description : 添加时间显示格式选择的radio list的item
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendTimeDisplayTypeListItem(
                                                 MMI_CTRL_ID_T	    ctrl_id
                                                 )
{
    uint16                      cur_selection       =   0;
    MMISET_TIME_DISPLAY_TYPE_E  time_display_type   =   MMISET_TIME_12HOURS;
    
    //add items to listbox
    MMIAPISET_AppendListItemByTextId( TXT_COMM_12_HOURS, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_COMM_24_HOURS, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    time_display_type = MMIAPISET_GetTimeDisplayType();
    
    cur_selection=(uint16)time_display_type;
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}


/*****************************************************************************/
// 	Description : 预览开关机动画返回列表后更新打勾情况
//	Global resource dependence : 
//  Author:scv
//	Note: 
/*****************************************************************************/
/*  //@xingdong.li, commented
//PUBLIC void MMISET_UpdateSelectedAnimateItem(
//MMI_CTRL_ID_T ctrl_id	
//)
//{
//MMISET_ANIMATE_SET_T        animate_info = {0};
//uint16 				        old_selection=0;	
//
//  //去掉原来选中项前的勾勾     
//  old_selection = (uint16)(animate_info.animate_id- 1);	
//  GUILIST_ChangeIconId(ctrl_id, old_selection, IMAGE_CHECK_UNSELECTED_ICON, IMAGE_NULL);
//  
//    //给选中项打勾勾
//    g_power_animate_index = GUILIST_GetCurItemIndex(MMISET_SET_POWER_ON_OFF_ANIMATION_CTRL_ID);
//    GUILIST_ChangeIconId(ctrl_id,g_power_animate_index,IMAGE_CHECK_SELECTED_ICON,IMAGE_NULL);
//    
//       }
*/
/*****************************************************************************/
// 	Description : 添加固定的开关机动画到text list的item
//	Global resource dependence : 
//  Author: scv
//	Note:
/*****************************************************************************/
/*      //@xingdong.li, commented
PUBLIC void MMISET_AppendOpenCloseAnimationListItem(
MMI_CTRL_ID_T	    ctrl_id
)
{
uint8						i               = 0;
uint16                      cur_selection   = 0;
MMISET_ANIMATE_SET_T        animate_info    = {0};

  SCI_MEMSET(&animate_info, 0, sizeof(MMISET_ANIMATE_SET_T));
  
    //add items to listbox
    MMIAPISET_AppendListItemByTextId(TXT_SET_ANIMATION_1,STXT_OK,ctrl_id);
    MMIAPISET_AppendListItemByTextId(TXT_SET_ANIMATION_2,STXT_OK,ctrl_id);
    MMIAPISET_AppendListItemByTextId(TXT_SET_ANIMATION_3,STXT_OK,ctrl_id);
    
      
        //get open animate info     
        cur_selection = (uint16)(animate_info.animate_id- 1);
        
          //未选中项前不打勾
          for(i=0;i<MMISET_FIXED_POWER_ON_OFF_ANIMATE_MAX_NUM;i++)
          {
          if(i!=cur_selection)
          {
          GUILIST_ChangeIconId(ctrl_id,i,IMAGE_CHECK_UNSELECTED_ICON,IMAGE_NULL);
          }
          }
          
            //set selected item
            GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
            
              //给选中项前面打勾
              GUILIST_ChangeIconId(ctrl_id, cur_selection, IMAGE_CHECK_SELECTED_ICON, IMAGE_NULL);
              
                //set current item
                GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
                
                  GUILIST_UpdateListBox(ctrl_id);
                  }
*/

/*****************************************************************************/
// 	Description : set the mode of TV out
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetTVOutMmode(
                                    MMI_CTRL_ID_T       ctrl_id
                                    )
{
    uint16                  cur_selection   =   0;
    MMISET_TVOUT_TYPE_E     tvout_mode      =   MMISET_PAL_LINE_BY_LINE;

    //get the selected item
    GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
    //SCI_ASSERT((MMISET_TV_OUT_MODE_MAX_NUM > cur_selection));
    if(MMISET_TV_OUT_MODE_MAX_NUM <= cur_selection)
    {
        //SCI_TRACE_LOW:"mmisetting_assert MMIAPISET_SetTVOutMmode cur_selection %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_990_112_2_18_2_50_53_141,(uint8*)"d", cur_selection);
        return;
    }

    tvout_mode = (MMISET_TVOUT_TYPE_E)cur_selection;

    //write the status of main menu animation into NV
    MMINV_WRITE(MMINV_SET_TV_OUT_MODE,&tvout_mode);
}

#ifdef TTS_SUPPORT
/*****************************************************************************/
// 	Description : get the info of TTS setting status
//	Global resource dependence : 
//  Author: barry.peng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetTTSInfo( void )
{
    BOOLEAN					is_tts_open = FALSE;    
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    //get TTS info from NV
    MMINV_READ(MMINV_SET_TTS_ON_OFF,&is_tts_open,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_tts_open =TRUE;
        MMINV_WRITE(MMINV_SET_TTS_ON_OFF,&is_tts_open);
    }
    
    return (is_tts_open);
}
/*****************************************************************************/
// 	Description : set the info of TTS setting status
//	Global resource dependence : 
//  Author: tao.xue
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetTTS(
                              BOOLEAN	    is_tts_open
                              )
{
    //write the status of TTS into NV
    MMINV_WRITE(MMINV_SET_TTS_ON_OFF,&is_tts_open);
}
/*****************************************************************************/
// 	Description : set the info of TTS setting status
//	Global resource dependence : 
//  Author: barry.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetTTSInfo(
                              MMI_CTRL_ID_T	    ctrl_id
                              )
{
    uint16      cur_selection = 0;
    BOOLEAN     is_tts_open = FALSE;
    
    //get the selected item
    GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
    
    // set the global virable
    if ( 0 == cur_selection)
    {
        is_tts_open = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPISET_SetTTSInfo cur_selection = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_1126_112_2_18_2_50_53_142,(uint8*)"d",cur_selection);
        is_tts_open = FALSE;
    }
    
    //write the status of TTS into NV
    MMINV_WRITE(MMINV_SET_TTS_ON_OFF,&is_tts_open);
}
/*****************************************************************************/
// 	Description : add set tts list item
//	Global resource dependence : 
//  Author: barry.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendTTSListItem(
                                     MMI_CTRL_ID_T	    ctrl_id
                                     )
{
    uint16                      cur_selection = 0;
    BOOLEAN                     is_tts_open = FALSE;
    
    MMIAPISET_AppendListItemByTextId( TXT_OPEN, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_CLOSE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );	
    //get current tts setting info
    is_tts_open = MMIAPISET_GetTTSInfo();
    //the status is open
    if (is_tts_open)
    {
        cur_selection = 0;
    }
    else
    {
        cur_selection = 1;
    }
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);    
}
#endif

/*****************************************************************************/
// 	Description : get main menu style
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC MMIMAINMENU_TYPE_E MMIAPISET_GetMainmenuStyle(void)
{
    SCI_ASSERT(s_mainmenu_style <= MMIMAINMENU_TYPE_MAX);/*assert verified*/
    SCI_TRACE_LOW("MMIAPISET_GetMainmenuStyle s_mainmenu_style=%d",s_mainmenu_style);
    return s_mainmenu_style;
}
 
/*****************************************************************************/
// 	Description : set main menu style
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetMainmenuStyle(
                                       MMIMAINMENU_TYPE_E     mainmenu_style
                                       )
{
    SCI_ASSERT(mainmenu_style <= MMIMAINMENU_TYPE_MAX);/*assert verified*/
    
    //set main menu style
    MMINV_WRITE(MMINV_SET_MAIN_MENU_STYLE,&mainmenu_style);
    s_mainmenu_style = mainmenu_style;
}
 
/*****************************************************************************/
// 	Description : get the type of tv out mode
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC MMISET_TVOUT_TYPE_E  MMIAPISET_GetTVOutMode( void )
{
    MMISET_TVOUT_TYPE_E     tvout_mode      =   MMISET_PAL_LINE_BY_LINE;
    MN_RETURN_RESULT_E      return_value    =   MN_RETURN_FAILURE;

    //get tv out mode from NV
    MMINV_READ(MMINV_SET_TV_OUT_MODE,&tvout_mode,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        tvout_mode = MMISET_PAL_LINE_BY_LINE;
        MMINV_WRITE(MMINV_SET_TV_OUT_MODE,&tvout_mode);
    }

    return (tvout_mode);
}
 
/*****************************************************************************/
// 	Description : add TVout mode list item
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_AppendTVOutModeListItem(
                                             MMI_CTRL_ID_T      ctrl_id
                                             )
{
    uint16                      cur_selection   =   0;

    MMIAPISET_AppendListItemByTextId( TXT_SET_PAL_LINE_BY_LINE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_SET_NTSC_LINE_BY_LINE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );

#ifdef MMISET_TVOUT_INTERLEAVING_MODE
    MMIAPISET_AppendListItemByTextId( TXT_SET_PAL_INTERLEAVING_LINE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_SET_NTSC_INTERLEAVING_LINE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
#endif

    //get current tv out mode
    cur_selection = MMIAPISET_GetTVOutMode();

    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);    
}
 
 /*****************************************************************************/
 // 	Description : add set main menu animation list item
 //	Global resource dependence : 
 //  Author: kelly.li
 //	Note:
 /*****************************************************************************/
// PUBLIC void MMISET_AppendMenuAnimationListItem(
//     MMI_CTRL_ID_T	    ctrl_id
//     )
// {
//     uint16                      cur_selection = 0;
//     BOOLEAN                     is_menu_animation_open = FALSE;
//     
//     MMIAPISET_AppendListItemByTextId( TXT_OPEN, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
//     MMIAPISET_AppendListItemByTextId( TXT_CLOSE, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );	
//     //get current wallpaper info
//     is_menu_animation_open = MMIAPISET_GetMenuAnimationInfo();
//     
//     //the status is open
//     if (is_menu_animation_open)
//     {
//         cur_selection = 0;
//     }
//     else
//     {
//         cur_selection = 1;
//     }
//     GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
//     GUILIST_SetCurItemIndex(ctrl_id,cur_selection);    
// }
 
/*****************************************************************************/
// 	Description : 添加固定图片的墙纸到radio list的item
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
 #if 0
PUBLIC void MMISET_AppendFixedWallpaperListItem(
                                                 MMI_CTRL_ID_T      ctrl_id
                                                 )
{
     
    uint16                      cur_selection   =   0;
    MMISET_WALLPAPER_SET_T      wallpaper_info  =   {0};
    MMI_TEXT_ID_T               item_text[5]    =   {TXT_SET_WALLPAPER_1,TXT_SET_WALLPAPER_2,TXT_SET_WALLPAPER_3,TXT_SET_WALLPAPER_4,TXT_SET_WALLPAPER_5};
    uint32                      i               =   0;


    SCI_MEMSET(&wallpaper_info, 0, sizeof(MMISET_WALLPAPER_SET_T));     
    //MMIAPISET_AppendListItemByTextId(TXT_EXPORT_LOC_DEFAULT, STXT_PREVIEW, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);      
    MMIAPISET_AppendListItemByTextIdExt(TXT_EXPORT_LOC_DEFAULT,STXT_OK, STXT_PREVIEW, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO  );
    for (i = 0; i < MMISET_WALLPAPER_FIXED_PIC_MAX_NUM-1; i++ )
    {
        MMIAPISET_AppendListItemByTextIdExt( item_text[i],TXT_COMMON_OK, STXT_PREVIEW, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO  );
        
        //MMIAPISET_AppendListItemByTextId(item_text[i], STXT_PREVIEW, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    }
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_MORE,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO  );
    //MMIAPISET_AppendListItemByTextId(TXT_SET_WALLPAPER_MORE, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO); 


    //get current wallpaper info
    MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);

    if (MMIAPIUDISK_UdiskIsRun() && (MMISET_WALLPAPER_DOWNLOAD == wallpaper_info.wallpaper_type))
    {
        wallpaper_info.wallpaper_id = MMISET_WALLPAPER_DEFAULT_ID;
        wallpaper_info.wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;        
    }
    //当前墙纸是固定图片
    if (MMISET_WALLPAPER_FIXED_PIC == wallpaper_info.wallpaper_type)
    {
        cur_selection = (uint16)(wallpaper_info.wallpaper_id - 1);
    }
    else
    {
        cur_selection = MMISET_WALLPAPER_FIXED_PIC_MAX_NUM;
    }
    GUILIST_SetSelectedItem( ctrl_id, cur_selection, TRUE );
    GUILIST_SetCurItemIndex( ctrl_id,cur_selection );
}
#else
PUBLIC void MMISET_AppendFixedWallpaperListItem(
                                                 MMI_CTRL_ID_T      ctrl_id
                                                 )
{
    #define LIST_STR_LEN    20 
    uint16                      cur_selection   =   0;
    MMISET_WALLPAPER_SET_T      wallpaper_info  =   {0};

    SCI_MEMSET(&wallpaper_info, 0, sizeof(MMISET_WALLPAPER_SET_T));     
    MMIAPISET_AppendListItemByTextIdExt(TXT_EXPORT_LOC_DEFAULT,STXT_OK, STXT_PREVIEW, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendFixedItems(
                                    TXT_SET_WALLPAPER_LIST_PREFIX,
                                    MMISET_WALLPAPER_FIXED_PIC_MAX_NUM - 1,
                                    TXT_COMMON_OK,
                                    STXT_PREVIEW,
                                    STXT_RETURN,
                                    ctrl_id,
                                    GUIITEM_STYLE_ONE_LINE_RADIO
                                    );
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT            
#ifdef LIVE_WALLPAPER_SUPPORT_LINEAR
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_LINEAR,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_POP
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_POP,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_BEE
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_BEE,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_GALAXY
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_GALAXY,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_SAKURA
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_SAKURA,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_SEAWORLD
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_SEAWORLD,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_LIGHTGRID
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_LIGHTGRID,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_RIPPLE
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_RIPPLE,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_COLLISION
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_COLLISION,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_LITTLECAT
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_LITTLECAT,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_SWEETTREE
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_SWEETTREE,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_COLLISION_BALLOON
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_COLLISION_BALLOON,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_COLLISION_MENU
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_COLLISION_MENU,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_WINDMILL
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_WINDMILL,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_RAINBOWCITY
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_RAINBOWCITY,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_PAPERTOWN
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_PAPERTOWN,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_AQUARIUM
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_AQUARIUM,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_GIRLSWING
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_GIRLSWING,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_SPRING
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_SPRING,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_NEST
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_NEST,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
#endif

#ifdef MMI_VIDEOWALLPAPER_SUPPORT
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_VIDEO,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_MORE,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    //展开anim的list
    //MMISET_AppendTwoLineAnimListItem(ctrl_id,STXT_OK, STXT_PREVIEW, STXT_RETURN);

    //get current wallpaper info
    MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);

    if (MMIAPIUDISK_UdiskIsRun() && (MMISET_WALLPAPER_DOWNLOAD == wallpaper_info.wallpaper_type))
    {
        wallpaper_info.wallpaper_id = MMISET_WALLPAPER_DEFAULT_ID;
        wallpaper_info.wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;        
    }
    ////当前墙纸是固定图片
    if (MMISET_WALLPAPER_FIXED_PIC == wallpaper_info.wallpaper_type)
    {
        cur_selection = (uint16)(wallpaper_info.wallpaper_id);
    }
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT    
    else if (MMISET_WALLPAPER_LIVE == wallpaper_info.wallpaper_type)
    {
        cur_selection = MMISET_WALLPAPER_FIXED_PIC_MAX_NUM + wallpaper_info.wallpaper_id;
    }
#endif    
#ifdef MMI_VIDEOWALLPAPER_SUPPORT    
    else if (MMISET_WALLPAPER_VIDEO == wallpaper_info.wallpaper_type)
    {
        cur_selection = MMISET_WALLPAPER_FIXED_PIC_MAX_NUM + MMISET_WALLPAPER_SPECIAL_ANIM_NUM;
    }
#endif     
    else
    {
        cur_selection = MMISET_WALLPAPER_FIXED_PIC_MAX_NUM + MMISET_WALLPAPER_SPECIAL_ANIM_NUM +1;//
    }

    GUILIST_SetSelectedItem( ctrl_id, cur_selection, TRUE );
    GUILIST_SetCurItemIndex( ctrl_id,cur_selection );
}
#endif
/*****************************************************************************/
// 	Description : 设定固定图片为墙纸
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetFixedWallpaper(
                                     MMI_CTRL_ID_T      ctrl_id
                                     )
{
    uint16                      sel_item_index  =   0;
    MMISET_WALLPAPER_SET_T      wallpaper_info  =   {0};

    SCI_MEMSET(&wallpaper_info, 0, sizeof(MMISET_WALLPAPER_SET_T));

    //get the selected item
    sel_item_index = g_cur_wallpaper_index;

    wallpaper_info.wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;
    wallpaper_info.wallpaper_id = (uint16)(sel_item_index);

    MMIAPISET_SetWallpaperSetInfo(wallpaper_info);

    MMISET_AppendFixedWallpaperListItem(ctrl_id);

    //free multimedia space
    //MMIAPIFMM_InitWallPaper();
}

 #ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
/*****************************************************************************/
// 	Description : 设定动态墙纸
//	Global resource dependence : 
//  Author: Gaily.Wang
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetLiveWallpaper(
                                    MMI_CTRL_ID_T      ctrl_id,
                                    uint32 id
                                    )
{
    MMISET_WALLPAPER_SET_T      wallpaper_info  =   {0};

    SCI_MEMSET(&wallpaper_info, 0, sizeof(MMISET_WALLPAPER_SET_T));

    wallpaper_info.wallpaper_type = MMISET_WALLPAPER_LIVE;
    wallpaper_info.wallpaper_id = (uint16)(id);

    MMIAPISET_SetWallpaperSetInfo(wallpaper_info);
#ifndef MMI_PDA_SUPPORT
   // MMISET_AppendFixedWallpaperListItem(ctrl_id);  
#endif
    //free multimedia space
#if defined MMI_WALLPAPER_SUPPORT
    MMIAPIFMM_InitWallPaper();
#endif
}
#endif//LIVE_WALLPAPER_FRAMEWORK_SUPPORT
 

#ifdef MMI_VIDEOWALLPAPER_SUPPORT
/*****************************************************************************/
// 	Description : 设定视频墙纸
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetVideoWallpaper(
                                    MMI_CTRL_ID_T      ctrl_id
                                    )
{
//    uint16                      sel_item_index  =   0;
    MMISET_WALLPAPER_SET_T      wallpaper_info  =   {0};

    SCI_MEMSET(&wallpaper_info, 0, sizeof(MMISET_WALLPAPER_SET_T));

    //get the selected item
    //sel_item_index = g_cur_wallpaper_index;

    wallpaper_info.wallpaper_type = MMISET_WALLPAPER_VIDEO;
    wallpaper_info.wallpaper_id = (uint16)(MMISET_WALLPAPER_LIST_VIDEO + 1);

    MMIAPISET_SetWallpaperSetInfo(wallpaper_info);

    MMISET_AppendFixedWallpaperListItem(ctrl_id);
//    GUILABEL_SetTextById(MMISET_DISPLAY_WALLPAPER_LABEL_EDIT_CTRL_ID,TXT_SET_WALLPAPER_VIDEO, FALSE);

    //free multimedia space
//    MMIAPIFMM_InitWallPaper();
}
#endif
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
/*****************************************************************************/
// 	Description :  set idle words content
//	Global resource dependence : 
//  Author:kelly.li
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetIdleWordsContent( 
                                            MMI_STRING_T    *content_info
                                            )
{
    BOOLEAN     result                                          =   FALSE;
    wchar       ucs_content[MMISET_IDLE_WORDS_UCS_MAX_LEN + 1]  =   {0};
    uint16      len                                             =   0;

    MMI_WSTRNCPY( ucs_content, MMISET_IDLE_WORDS_UCS_MAX_LEN, content_info->wstr_ptr, content_info->wstr_len, content_info->wstr_len );
    len = content_info->wstr_len;    


    //init display idle words
    SCI_MEMSET(s_idle_display_info.idle_words, 0x00, sizeof(s_idle_display_info.idle_words));
    s_idle_display_info.wstr_len = 0;

    //SCI_ASSERT(len <= MMISET_IDLE_WORDS_UCS_MAX_LEN);


  // widget idle 个性词语可以设置为空

	//if(len >0)
	//{
        	MMI_WSTRNCPY( s_idle_display_info.idle_words, MMISET_IDLE_WORDS_UCS_MAX_LEN, ucs_content, len, len );
        	s_idle_display_info.wstr_len = len;
        	result = TRUE;
    	//}

    return (result);
}
 #endif
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
/*****************************************************************************/
// 	Description : add idle display list to radio list 
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendIdleDisplayListItem(
                                            MMI_CTRL_ID_T   ctrl_id
                                            )
{
    GUILIST_ITEM_T          item_t          =       {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data       =       {0};/*lint !e64*/
    MMI_IMAGE_ID_T          image_id        =      IMAGE_NULL;
    const MMI_TEXT_ID_T           idle_text_id[MMISET_IDLE_DISPLAY_MAX_ITEM] =
                                                                {TXT_SET_TIME_AND_DATE,
#if defined MMI_MULTI_SIM_SYS_TRI ||  defined MMI_MULTI_SIM_SYS_QUAD ||  defined MMI_MULTI_SIM_SYS_DUAL                                                             
                                                                TXT_SIM_SEL_SIM1,  
#else                                                           
                                                                TXT_SIM_SEL,
#endif                                                                
#if defined MMI_MULTI_SIM_SYS_TRI ||  defined MMI_MULTI_SIM_SYS_QUAD ||  defined MMI_MULTI_SIM_SYS_DUAL                                                             
                                                                TXT_SIM_SEL_SIM2,
#endif         
#if defined MMI_MULTI_SIM_SYS_TRI ||  defined MMI_MULTI_SIM_SYS_QUAD   
                                                                TXT_SIM_SEL_SIM3,
#endif                                                                
#if defined MMI_MULTI_SIM_SYS_QUAD   
                                                                TXT_SIM_SEL_SIM4,
#endif                                                                
                                                                TXT_SET_TODAY_SCHEDULE,
#ifdef COUNTEDTIME_SUPPORT
                                                                TXT_TOOLS_COUNTEDTIME,
#endif     
#ifdef MMI_AUDIO_PLAYER_SUPPORT
                                                                TXT_SET_SHORTCUT_MP3,
#endif
#ifdef FM_SUPPORT

                                                                TXT_FM,
#endif
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
#if !(defined(MMI_DISPLAYSET_MINI_SUPPORT) && defined(MMI_ENABLE_DCD)) 

                                                                TXT_SET_CUSTOM_WORD, 
#endif  
#endif
#ifdef MMI_IDLE_STKTEXT_SUPPORT

                                                                TXT_COMMON_MAIN_STK,
#endif
                                         
#ifdef MMI_WIFI_SUPPORT
                                                                TXT_WIFI ,
#endif
                                                                };
    uint32                  i               =       0;   
    uint16                  cur_selection   =       0;
    uint32      num1 = MMISET_IDLE_DISPLAY_MAX_ITEM;
    
    if(num1 != sizeof(idle_text_id)/sizeof(idle_text_id[0]))    /*lint !e774 */
    {
        return ;
    }
    
    GUILIST_SetMaxItem(MMISET_SET_IDLE_DISPLAY_CTRL_ID,MMISET_IDLE_DISPLAY_MAX_ITEM, FALSE );//max item 4
    
   
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &item_data;
    
    item_data.softkey_id[LEFT_BUTTON]   = (MMI_TEXT_ID_T)STXT_SELECT;
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
	item_data.softkey_id[MIDDLE_BUTTON] = (MMI_IMAGE_ID_T)IMAGE_NULL;
#else
	item_data.softkey_id[MIDDLE_BUTTON] = (MMI_TEXT_ID_T)TXT_NULL;
#endif
    item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)STXT_RETURN;
    
    for (i = 0; i < MMISET_IDLE_DISPLAY_MAX_ITEM; i++)
    {    
            if(s_idle_display_info.visible_flag.flag_array[i])
            {
                image_id = IMAGE_CHECK_SELECTED_ICON;
                item_data.softkey_id[LEFT_BUTTON]   = (MMI_TEXT_ID_T)STXT_CANCEL;
            }
            else
            {
                image_id = IMAGE_CHECK_UNSELECTED_ICON;
                item_data.softkey_id[LEFT_BUTTON]   = (MMI_TEXT_ID_T)STXT_SELECT;
            }


        
        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = image_id;
        
        
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[1].item_data.text_id = idle_text_id[i];
        
        
        GUILIST_AppendItem(ctrl_id,&item_t);
    }
    
    cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}
#endif
#endif
/*****************************************************************************/
// 	Description : 设定固定动画为墙纸
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
//PUBLIC void MMISET_SetFixedAnimWallpaper(
//                                        MMI_CTRL_ID_T   ctrl_id
//                                        )
//{
//    uint16                      sel_item_index  =   0;
//    MMISET_WALLPAPER_SET_T      wallpaper_info  =   {0};
//
//    SCI_MEMSET(&wallpaper_info, 0, sizeof(MMISET_WALLPAPER_SET_T));
//
//    //get the selected item
//    sel_item_index = g_cur_wallpaper_index;
//
//    wallpaper_info.wallpaper_type = MMISET_WALLPAPER_FIXED_ANIM;
//    wallpaper_info.wallpaper_id = (uint16)(sel_item_index + 1);
//
//    MMIAPISET_SetWallpaperSetInfo(wallpaper_info);
//
//    MMISET_AppendFixedAnimWallpaper(ctrl_id);
//
//    //free multimedia space
//    MMIAPIFMM_InitWallPaper();
//}
// 
/*****************************************************************************/
// 	Description : 添加固定的动画墙纸到text list的item
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
//PUBLIC void MMISET_AppendFixedAnimWallpaper(
//                                            MMI_CTRL_ID_T   ctrl_id
//                                            )
//{
//    uint16                      cur_selection   =   0;
//    MMISET_WALLPAPER_SET_T      wallpaper_info  =   {0};
//
//    SCI_MEMSET(&wallpaper_info, 0, sizeof(MMISET_WALLPAPER_SET_T));
//
//    //add items to listbox
//    MMIAPISET_AppendListItemByTextId( TXT_SET_ANIMATION_1, STXT_PREVIEW, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
//    MMIAPISET_AppendListItemByTextId( TXT_SET_ANIMATION_2, STXT_PREVIEW, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
//    MMIAPISET_AppendListItemByTextId( TXT_SET_ANIMATION_3, STXT_PREVIEW, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
//
//
//    //get current wallpaper info
//    MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
//
//    //当前墙纸是固定图片
//    if (MMISET_WALLPAPER_FIXED_ANIM == wallpaper_info.wallpaper_type)
//    {
//        cur_selection = (uint16)(wallpaper_info.wallpaper_id - 1);
//
//        GUILIST_SetSelectedItem( ctrl_id, cur_selection, TRUE );
//        GUILIST_SetCurItemIndex( ctrl_id,cur_selection );
//    }
//}
 
/*****************************************************************************/
// 	Description : 播放动画墙纸的第n帧
//	Global resource dependence : 
//  Author:Jassmine
//	Note:
/*****************************************************************************/
//PUBLIC void  MMIAPISET_DisplayAnimWallPaper(
//                                            BOOLEAN         is_first_page,
//                                            uint16          wallpaper_id,
//                                            MMI_WIN_ID_T    win_id
//                                            )
//{
//    uint32		        bmp_id          =   0;
//    GUI_POINT_T         dis_point       =   {0};
//    GUI_LCD_DEV_INFO	lcd_dev_info    =   {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
//
//    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
//    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
//
//    if (is_first_page)
//    {
//        g_cur_anim_wallpaper_page = 0;
//    }
//    else
//    {
//        g_cur_anim_wallpaper_page++;
//        if (g_cur_anim_wallpaper_page >= g_fixed_anim_wallpaper[wallpaper_id-1].page_num)
//        {
//            g_cur_anim_wallpaper_page = 0;
//        }
//    }
//    bmp_id = (uint32)g_fixed_anim_wallpaper[wallpaper_id-1].page[g_cur_anim_wallpaper_page];
//
//    GUIRES_DisplayImg(&dis_point,
//                        PNULL,
//                        PNULL,
//                        win_id,
//                        bmp_id,
//                        &lcd_dev_info);
//}
#if defined DPHONE_SUPPORT
/*****************************************************************************/
// 	Description : 添加背光灯到radio list的item
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendBacklightListItem(
                                            MMI_CTRL_ID_T   ctrl_id
                                            )
{
    uint16                      cur_selection   =   0;

    //add items to listbox
    MMIAPISET_AppendListItemByTextId( TXT_SET_TIME_OPEN_FOREVER, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_SET_TIME_30S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_SET_TIME_20S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_SET_TIME_15S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_COMM_10SECONDS, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_SET_TIME_5S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );

    //get current wallpaper info
    switch (g_backlight_info.backlight_type)
    {
    case MMISET_BACKLIGHT_OPEN:
        switch (g_backlight_info.backlight_time)
        {
        case MMISET_TIME_30:
            cur_selection = 1;
            break;

        case MMISET_TIME_20:
            cur_selection = 2;
            break;

        case MMISET_TIME_15:
            cur_selection = 3;
            break;

        case MMISET_TIME_10:
            cur_selection = 4;
            break;

        case MMISET_TIME_5:
            cur_selection = 5;
            break;

        default:
            //SCI_TRACE_LOW:"MMISET_AppendBacklightListItem:backlight time is %d error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_1891_112_2_18_2_50_55_144,(uint8*)"d",g_backlight_info.backlight_time);
            break;
        }
        break;

    case MMISET_BACKLIGHT_OPEN_FOREVER:
        cur_selection = 0;
        break;

    case MMISET_BACKLIGHT_CLOSE:
        //SCI_TRACE_LOW("MMISET_AppendBacklightListItem:backlight type is close!");
        break;

    default:
        //SCI_TRACE_LOW:"MMISET_AppendBacklightListItem:backlight type is %d error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_1905_112_2_18_2_50_55_145,(uint8*)"d",g_backlight_info.backlight_type);
        break;
    }

    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);

    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}
#endif
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
/*****************************************************************************/
// 	Description : set idle win display mode
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetIdleDisplayMode(
                                        uint16  cur_index
                                        )
{
     s_idle_display_info.visible_flag.flag_array[cur_index] 
        =  !(s_idle_display_info.visible_flag.flag_array[cur_index] );
}
#if  0//pclint
/*****************************************************************************/
// 	Description : set idle win display mode
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetDisplaytIdleMode(void)
{
    //write information about idle win display set into NV
    MMINV_WRITE(MMINV_SET_IDLEDISPLAY_INFO,&s_idle_display_info);
}
#endif
 #endif
 #endif 
 #if defined (MMISET_BACKLIGHT_TIME_SUPPORT) || defined(DPHONE_SUPPORT)
/*****************************************************************************/
// 	Description : 设定背光灯时间
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetBackLight(
                                MMI_CTRL_ID_T   ctrl_id
                                )
{
    uint16                      cur_selection   =   0;
    MMISET_BACKLIGHT_SET_T      backlight_info  =   {0};

    SCI_MEMSET(&backlight_info, 0, sizeof(MMISET_BACKLIGHT_SET_T));

    //get the selected item
#ifdef DPHONE_SUPPORT 
    GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
#else
    cur_selection = GUISETLIST_GetCurIndex(ctrl_id);
#endif 
    //SCI_ASSERT( (MMISET_BACKLIGHT_MAX_ITEM > cur_selection));
   if(MMISET_BACKLIGHT_MAX_ITEM <= cur_selection)
   {
   	//SCI_TRACE_LOW:"mmisetting_assert MMISET_SetBackLight cur_selection %d"
   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_2003_112_2_18_2_50_55_147,(uint8*)"d", cur_selection);
	return;
   }
    
    switch (cur_selection)
    {
        case 0:
            backlight_info.backlight_type = MMISET_BACKLIGHT_OPEN_FOREVER;
            break;

        case 1:
            backlight_info.backlight_type = MMISET_BACKLIGHT_OPEN;
            backlight_info.backlight_time = MMISET_TIME_30;
            break;

        case 2:
            backlight_info.backlight_type = MMISET_BACKLIGHT_OPEN;
            backlight_info.backlight_time = MMISET_TIME_20;
            break;

        case 3:
            backlight_info.backlight_type = MMISET_BACKLIGHT_OPEN;
            backlight_info.backlight_time = MMISET_TIME_15;
            break;

        case 4:
            backlight_info.backlight_type = MMISET_BACKLIGHT_OPEN;
            backlight_info.backlight_time = MMISET_TIME_10;
            break;

        case 5:
            backlight_info.backlight_type = MMISET_BACKLIGHT_OPEN;
            backlight_info.backlight_time = MMISET_TIME_5;
            break;

        default:
            break;
    }
    MMIAPISET_SetBackLightInfo(backlight_info);
}
 #endif
#if defined DPHONE_SUPPORT
/*****************************************************************************/
// 	Description : 显示对比度的初始图片
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_DisplayInitContrast(MMI_WIN_ID_T     win_id)
{
    uint8               mainlcd_contrast    =   0;
    GUI_BG_T  img_bg = {0};
	
    mainlcd_contrast = MMIAPISET_GetMainLCDContrast();     

    if (MMISET_CONTRAST_SEVEN < mainlcd_contrast)
    {
        mainlcd_contrast = MMISET_CONTRAST_SEVEN;
    }

    //set gloable variable
    s_cur_contrast = mainlcd_contrast; 
  
     img_bg.bg_type = GUI_BG_IMG;
     img_bg.img_id = IMAGE_SET_CONTRAST_DOWN_RELEASED_DEC_GREY;
     GUIBUTTON_SetGrayed(MMISET_CONTRAST_DOWN_BUTTON_CTRL_ID,FALSE,&img_bg,FALSE);   
      img_bg.img_id = IMAGE_SET_CONTRAST_UP_RELEASED_DEC_GREY;
     GUIBUTTON_SetGrayed(MMISET_CONTRAST_UP_BUTTON_CTRL_ID,FALSE,&img_bg,FALSE); 
	 
    if(MMISET_CONTRAST_ONE > s_cur_contrast)
    {
 	GUIBUTTON_SetGrayed(MMISET_CONTRAST_DOWN_BUTTON_CTRL_ID, TRUE, NULL,TRUE);
    }
  
    if(MMISET_CONTRAST_SEVEN == s_cur_contrast)
    {
 	GUIBUTTON_SetGrayed(MMISET_CONTRAST_UP_BUTTON_CTRL_ID, TRUE,NULL, TRUE);
    }
}
#endif 
/*****************************************************************************/
// 	Description : get current lcd contrast
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetCurrentContrast(void)
{
    return s_cur_contrast;
}
 
/*****************************************************************************/
// 	Description : set current lcd contrast
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetCurrentContrast(uint8     lcd_contrast)
{
    // SCI_ASSERT(MMISET_CONTRAST_ONE <= lcd_contrast && lcd_contrast <= MMISET_CONTRAST_SEVEN);	
    if (MMISET_CONTRAST_SEVEN < lcd_contrast)
    {
        lcd_contrast = MMISET_CONTRAST_SEVEN;
    }
    s_cur_contrast = lcd_contrast;
	
    if(MMISET_CONTRAST_ONE > s_cur_contrast)
    {
  	 GUIBUTTON_SetGrayed(MMISET_CONTRAST_DOWN_BUTTON_CTRL_ID, TRUE, NULL,TRUE);
	 GUIBUTTON_SetGrayed(MMISET_CONTRAST_UP_BUTTON_CTRL_ID, FALSE,NULL, FALSE);	 
    }    
    else if(MMISET_CONTRAST_SEVEN == s_cur_contrast)
     {
  	  GUIBUTTON_SetGrayed(MMISET_CONTRAST_DOWN_BUTTON_CTRL_ID, FALSE,NULL,FALSE);
	  GUIBUTTON_SetGrayed(MMISET_CONTRAST_UP_BUTTON_CTRL_ID, TRUE,NULL, TRUE);
     }
     else  if(MMISET_CONTRAST_SEVEN > s_cur_contrast && MMISET_CONTRAST_ONE <= s_cur_contrast)
     {
	  GUIBUTTON_SetGrayed(MMISET_CONTRAST_UP_BUTTON_CTRL_ID, FALSE,NULL, FALSE);
  	  GUIBUTTON_SetGrayed(MMISET_CONTRAST_DOWN_BUTTON_CTRL_ID, FALSE, NULL,FALSE);
     }
}

// this API not related UI only change data
PUBLIC void MMIAPISET_SetCurrentContrastEx( uint8 lcd_contrast )
{
    if( MMISET_CONTRAST_FIVE < lcd_contrast )
    {
        lcd_contrast = MMISET_CONTRAST_FIVE;
    }
    s_cur_contrast = lcd_contrast;
}

/*****************************************************************************/
// 	Description : 调解对比度的显示
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_TuneUpConstrast(
                                    MMI_MESSAGE_ID_E   msg_id
                                    )
{
    //get current volume
    switch (msg_id)
    {
    case MSG_APP_LEFT:
        if (MMISET_CONTRAST_ONE <= s_cur_contrast) // modify by mary FOR CR 221484
        {
            s_cur_contrast--;
            //tune up contrast
            MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID,s_cur_contrast);
        }
		
        if(MMISET_CONTRAST_ONE > s_cur_contrast)
        {
            GUIBUTTON_SetGrayed(MMISET_CONTRAST_DOWN_BUTTON_CTRL_ID, TRUE, NULL,TRUE);
        }
        if(MMISET_CONTRAST_SIX == s_cur_contrast)
        {
            GUIBUTTON_SetGrayed(MMISET_CONTRAST_UP_BUTTON_CTRL_ID, FALSE,NULL,FALSE);
        }
        break;

    case MSG_APP_RIGHT:
        if (MMISET_CONTRAST_SEVEN > s_cur_contrast)
        {
            s_cur_contrast++;
            //tune up contrast
            MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID,s_cur_contrast);
        }
		
        if(MMISET_CONTRAST_ONE == s_cur_contrast)
        {
            GUIBUTTON_SetGrayed(MMISET_CONTRAST_DOWN_BUTTON_CTRL_ID, FALSE,NULL, FALSE);
        }
        if(MMISET_CONTRAST_SEVEN == s_cur_contrast)
        {
            GUIBUTTON_SetGrayed(MMISET_CONTRAST_UP_BUTTON_CTRL_ID, TRUE,NULL, TRUE);
        }
        break;

    default:
        //SCI_TRACE_LOW:"MMISET_TuneUpConstrast:msg_id %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_2167_112_2_18_2_50_55_148,(uint8*)"d",msg_id);
        break;
    }
    //SCI_TRACE_LOW:"MMISET_TuneUpConstrast:s_cur_contrast is %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_2170_112_2_18_2_50_55_149,(uint8*)"d",s_cur_contrast);

}
 
/*****************************************************************************/
// 	Description : 恢复屏幕的对比度
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_ResetContrast(
                                    uint8   lcd_id
                                    )
{
    uint8   mainlcd_contrast    =   0;

    mainlcd_contrast = MMIAPISET_GetMainLCDContrast();

    if (MMISET_CONTRAST_SEVEN < mainlcd_contrast)
    {
        mainlcd_contrast = MMISET_CONTRAST_FIVE;
    }

    //set gloable variable
    s_cur_contrast = mainlcd_contrast;

    //reset constrast
    MMIAPISET_UpdateLCDContrast(lcd_id,s_cur_contrast);
}
 
/*****************************************************************************/
// 	Description : get the backlight value
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPISET_GetRealBacklightValue(void)
{
    return s_real_contrast;
}
#ifdef SCREENSAVER_SUPPORT
/*****************************************************************************/
// 	Description : get screen saver info
//	Global resource dependence : 
//  Author:Jassmine
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_GetScreensaverInfo(
                                        MMISET_SCREENSAVER_INFO_T   *ssaver_info_ptr    //in/out:
                                        )
{
    MN_RETURN_RESULT_E      return_value    =   MN_RETURN_FAILURE;
    BOOLEAN                 is_need_reset   =   FALSE;

   // SCI_ASSERT(PNULL != ssaver_info_ptr);
   if(PNULL == ssaver_info_ptr)
   {
   	//SCI_TRACE_LOW:"mmisetting_assert MMIAPISET_GetScreensaverInfo"
   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_2221_112_2_18_2_50_55_150,(uint8*)"");
	return;
   }
    
    MMINV_READ(
                MMINV_SCREENSAVER_SETTING_INFO, 
                ssaver_info_ptr,
                return_value
                );

    if (MN_RETURN_SUCCESS != return_value)
    {
        is_need_reset = TRUE;
    }


    if (MMISET_SCREENSAVER_MORE_PICTURE == ssaver_info_ptr->ssaver_type)    //@xingdong.li
    {
        if (is_need_reset || !MMIAPIFMM_IsFileExist(
                                ssaver_info_ptr->more_pic_saver_info.name_wstr,
                                ssaver_info_ptr->more_pic_saver_info.name_wstr_len
                                ))
        {
            ssaver_info_ptr->is_open        = FALSE;
            ssaver_info_ptr->ssaver_type    = MMISET_SCREENSAVER_CLOSED; 
            SCI_MEMSET(
                        &ssaver_info_ptr->more_pic_saver_info, 
                        0, 
                        sizeof(MMISET_MORE_SCREENSAVER_T)
                        ); 
            if (!MMIAPIUDISK_UdiskIsRun())
            {
                MMINV_WRITE(
                            MMINV_SCREENSAVER_SETTING_INFO, 
                            ssaver_info_ptr
                            );
            }
        }
    }
#ifdef VIDEO_PLAYER_SUPPORT
    else if (MMISET_SCREENSAVER_MORE_ANIMATION == ssaver_info_ptr->ssaver_type)    //@xingdong.li
    {
        if (is_need_reset ||!MMIAPIFMM_IsFileExist(
                                ssaver_info_ptr->more_ani_saver_info.name_wstr,
                                ssaver_info_ptr->more_ani_saver_info.name_wstr_len
                                ))
        {

            ssaver_info_ptr->is_open        = FALSE;
            ssaver_info_ptr->ssaver_type    = MMISET_SCREENSAVER_CLOSED; 
            SCI_MEMSET(
                        &ssaver_info_ptr->more_ani_saver_info, 
                        0, 
                        sizeof(MMISET_MORE_SCREENSAVER_T)
                        ); 
            if (!MMIAPIUDISK_UdiskIsRun())
            {
                MMINV_WRITE(
                            MMINV_SCREENSAVER_SETTING_INFO, 
                            ssaver_info_ptr
                            );
            }

        }
    } 
#endif
}
#endif
/*****************************************************************************/
// 	Description : get first day of week
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: 
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetFirstDayOfWeek( void )
{
    uint8                   first_day_of_week   =   0;
    MMISET_LANGUAGE_TYPE_E  language_type       =   MMISET_MAX_LANGUAGE;

    MMIAPISET_GetLanguageType( &language_type );

#ifdef IM_ARABIC_SUPPORT
    if( MMISET_LANGUAGE_ARABIC == language_type )
    {
        first_day_of_week = 6;
    }
#endif
    return first_day_of_week;
}
 
/*****************************************************************************/
// 	Description : get week day offset
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: 
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetWeekDayByOffset( uint8 offset )
{
    return ( offset + MMIAPISET_GetFirstDayOfWeek() ) % WEEKDAY_NUM;
}


/*****************************************************************************/
// 	Description : get offset by week day
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: 根据周几获得第几格
/*****************************************************************************/
PUBLIC uint8 MMIAPISET_GetOffsetByWeekDay( uint8 week_day )
{
    return ( WEEKDAY_NUM + week_day - MMIAPISET_GetFirstDayOfWeek() ) % WEEKDAY_NUM;
}


/*****************************************************************************/
// 	Description : is display idle words
//	Global resource dependence : 
//  Author:apple.zhang
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsDisplayIdleWords(void)
{
#ifndef MMI_IDLE_CUSTOM_WORD_SUPPORT
#if !(defined(MMI_DISPLAYSET_MINI_SUPPORT) && defined(MMI_ENABLE_DCD)) 
    return s_idle_display_info.visible_flag.item_flag.is_show_idle_words;
#endif
#endif
}

#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
/*****************************************************************************/
// 	Description : 添加3D style mainmenu
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_Append3DStyleMainmenuListItem(
                                               MMI_CTRL_ID_T    ctrl_id
                                               )
{
    uint16      cur_selection = 0;

    //add items to listbox
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_COMMON_NORMAL, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId(TXT_MMISET_3D_STYLE_MAINMENU_MAGICMOVE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId(TXT_MMISET_3D_STYLE_MAINMENU_ROTATE3D, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId(TXT_MMISET_3D_STYLE_MAINMENU_SLIDEMOVE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId(TXT_MMISET_3D_STYLE_MAINMENU_ROTATEMOVE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId(TXT_MMISET_3D_STYLE_MAINMENU_LUCKYBOARD, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId(TXT_MMISET_3D_STYLE_MAINMENU_WRAPBAND, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId(TXT_MMISET_3D_STYLE_MAINMENU_DISTORTMOVE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    //get current keyboard light
    cur_selection = MMISET_Get3DStyleMainmenu();
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}
#endif
#ifdef SET_3D_STYLE_SUPPORT
/*****************************************************************************/
// 	Description : 添加窗口的3D特效
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_Append3DStyleScreenListItem(
                                               MMI_CTRL_ID_T    ctrl_id
                                               )
{
    uint16      cur_selection = 0;

    //add items to listbox
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_COMMON_NORMAL, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId(TXT_SET_3D_STYLE_SCREEN_DEFAULT, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId(TXT_AUTO, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId(TXT_SET_3D_STYLE_SCREEN_SLIDE_MOVE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId(TXT_SET_3D_STYLE_SCREEN_SLIDE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId(TXT_SET_3D_STYLE_SCREEN_ZOOM, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId(TXT_COMM_FADE_INOUT, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId(TXT_SET_3D_STYLE_SCREEN_ROLL, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    //get current keyboard light
    cur_selection = MMISET_Get3DStyleScreen();
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}
#endif
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
/*****************************************************************************/
// 	Description : 设置3D style mainmenu的结果
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_Set3DStyleMainmenu(
                                    MMI_CTRL_ID_T   ctrl_id
                                    )
{
    uint16      cur_selection    =   0;
    
    //get the selected item
    //GUILIST_GetSelectedItemIndex(ctrl_id, &cur_selection, 1);
    cur_selection = GUISETLIST_GetCurIndex(ctrl_id);
        
    //SCI_ASSERT((MMISET_3D_STYLE_MAINMENU_MAX > cur_selection));
   if(MMISET_3D_STYLE_MAINMENU_MAX <= cur_selection)
   {
   	//SCI_TRACE_LOW:"mmisetting_assert MMISET_Set3DStyleMainmenu cur_selection %d"
   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_2414_112_2_18_2_50_56_151,(uint8*)"d", cur_selection);
	return;
   }
        
    Set3DStyleMainmenu(cur_selection);
}
#endif
/*****************************************************************************/
// 	Description : 设置3窗口特效类型
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void Set3DStyleScreen(
                            uint16 cur_sel
                            )
{
#ifdef SET_3D_STYLE_SUPPORT   
    MMISET_3D_STYLE_T*   mmi_3d_style_ptr    =   PNULL;
    mmi_3d_style_ptr = Get3DStyle();
    mmi_3d_style_ptr->screen_3d_style = cur_sel;/*lint !e64*/
    Set3DStyle(mmi_3d_style_ptr);
#endif
}
#ifdef SET_3D_STYLE_SUPPORT
/*****************************************************************************/
// 	Description : 设置窗口的3D特效
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_Set3DStyleScreen(
                                    MMI_CTRL_ID_T   ctrl_id
                                    )
{
    uint16      cur_selection    =   0;
    
    //get the selected item
    //GUILIST_GetSelectedItemIndex(ctrl_id, &cur_selection, 1);
    cur_selection = GUISETLIST_GetCurIndex(ctrl_id);
        
    //SCI_ASSERT((MMISET_3D_STYLE_SCREEN_MAX > cur_selection));
    if(MMISET_3D_STYLE_SCREEN_MAX <= cur_selection)
    {
        //SCI_TRACE_LOW:"mmisetting_assert MMISET_Set3DStyleScreen cur_selection %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_2455_112_2_18_2_50_56_152,(uint8*)"d", cur_selection);
        return;
    }

    Set3DStyleScreen(cur_selection);
    return;
}
#endif
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
/*****************************************************************************/
// 	Description : 得到3D style mainmenu的类型
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMISET_Get3DStyleMainmenu(void)
{
    return (uint16)s_mmi_3d_style.mainmenu_3d_style;
}
#endif
/*****************************************************************************/
// 	Description : 得到窗口的3D特效类型
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMISET_Get3DStyleScreen(void)
{
    return (uint16)s_mmi_3d_style.screen_3d_style;
}

#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
/*****************************************************************************/
// 	Description : 设置3D style mainmenu的类型
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void Set3DStyleMainmenu(
                              uint16 cur_sel
                              )
{
    MMISET_3D_STYLE_T   *mmi_3d_style_ptr    =  PNULL;
#ifdef SET_3D_STYLE_SUPPORT    
    mmi_3d_style_ptr = Get3DStyle();

    mmi_3d_style_ptr->mainmenu_3d_style = cur_sel;/*lint !e64*/
    Set3DStyle(mmi_3d_style_ptr);
#endif
}
#endif

#ifdef EFFECT_3D_LIST_ROLL_SUPPORT
/*****************************************************************************/
// 	Description : 设置list 特效的开启/关闭情况
//	Global resource dependence : 
//  Author: juan.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_Set3DStyleList3DStyle(MMI_CTRL_ID_T	    ctrl_id)
{
    uint16      cur_selection    =   0;
    
    cur_selection = GUISETLIST_GetCurIndex(ctrl_id);

    Set3DStyleList(cur_selection);
}
#endif
/*****************************************************************************/
// 	Description : 得到3D其他特效的开启/关闭情况
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetList3DStyle(void)
{
    return s_mmi_3d_style.list_3d_style;
}

/*****************************************************************************/
// 	Description : 得到g-sensor切换特效
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPISET_GetGsensorSwitchStyle(void)
{
    if (s_mmi_3d_style.g_sensor_anim >= MMISET_G_SENSOR_SWITCH_MAX)
    {
        s_mmi_3d_style.g_sensor_anim = MMISET_G_SENSOR_SWITCH_DEFAULT;
    }
    return s_mmi_3d_style.g_sensor_anim;
}

/*****************************************************************************/
// 	Description : 设置g-sensor切换特效
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetGsensorSwitchStyle(
                                         uint32 g_sensor_anim
                                         )
{
    //SCI_ASSERT((MMISET_G_SENSOR_SWITCH_MAX > g_sensor_anim));
    if(MMISET_G_SENSOR_SWITCH_MAX <= g_sensor_anim)
   {
   	//SCI_TRACE_LOW:"mmisetting_assert MMISET_SetGsensorSwitchStyle g_sensor_anim %d"
   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_2558_112_2_18_2_50_56_153,(uint8*)"d", g_sensor_anim);
	return;
   }
#ifdef MMI_RES_ORIENT_BOTH     
    SetGsensorSwitchAnimStyle(g_sensor_anim);
#endif
    return;
}
#ifdef MMI_RES_ORIENT_BOTH
/*****************************************************************************/
// 	Description : 设置g_sensor切换特效
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void SetGsensorSwitchAnimStyle(
                                     uint32 g_sensor_anim
                                     )
{
    MMISET_3D_STYLE_T*   mmi_3d_style_ptr    =   PNULL;
#ifdef SET_3D_STYLE_SUPPORT    
    mmi_3d_style_ptr = Get3DStyle();
    mmi_3d_style_ptr->g_sensor_anim = g_sensor_anim;/*lint !e64*/
    Set3DStyle(mmi_3d_style_ptr);
#endif
}
#endif
#ifdef SET_3D_STYLE_SUPPORT
/*****************************************************************************/
// 	Description : 设置3D其他特效
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void Set3DStyleList(uint16    cur_sel)
{
    MMISET_3D_STYLE_T*   mmi_3d_style_ptr    =   PNULL;
    
    mmi_3d_style_ptr = Get3DStyle();
    mmi_3d_style_ptr->list_3d_style = cur_sel;
    Set3DStyle(mmi_3d_style_ptr);
}

/*****************************************************************************/
// 	Description : 得到NV 3D style
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL MMISET_3D_STYLE_T* Get3DStyle(void)
{
    return &s_mmi_3d_style;
}


/*****************************************************************************/
// 	Description : 设置NV 3D style
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void Set3DStyle(
                      MMISET_3D_STYLE_T     *mmi_3d_style_ptr
                      )
{
    if(PNULL == mmi_3d_style_ptr)
    {
   	    //SCI_TRACE_LOW:"mmisetting_assert Set3DStyle"
   	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_2638_112_2_18_2_50_56_155,(uint8*)"");
        return;
    }
    s_mmi_3d_style.mainmenu_3d_style = mmi_3d_style_ptr->mainmenu_3d_style;
    s_mmi_3d_style.screen_3d_style = mmi_3d_style_ptr->screen_3d_style;
    s_mmi_3d_style.list_3d_style = mmi_3d_style_ptr->list_3d_style;
    s_mmi_3d_style.g_sensor_anim = mmi_3d_style_ptr->g_sensor_anim;

    MMINV_WRITE(MMINV_SET_3D_STYLE, &s_mmi_3d_style);
}
/*****************************************************************************/
// 	Description : init 3d style
//	Global resource dependence : 
//  Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL void  Init3DStyle(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    SCI_MEMSET(&s_mmi_3d_style,0x00,sizeof(MMISET_3D_STYLE_T));

    MMINV_READ(MMINV_SET_3D_STYLE, &s_mmi_3d_style, return_value);

    if (MN_RETURN_SUCCESS != return_value || !IsValid3DStyle(&s_mmi_3d_style))
    {
        MMISET_Reset3DStyle();
    }
}

/*****************************************************************************/
// 	Description : 恢复默认值NV 3D style
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_Reset3DStyle(void)
{
    //s_mmi_3d_style.mainmenu_3d_style = MMISET_3D_STYLE_MAINMENU_ROTATE3D;
    //s_mmi_3d_style.screen_3d_style = MMISET_3D_STYLE_SCREEN_DEFAULT;
    //s_mmi_3d_style.list_3d_style = TRUE;
    //s_mmi_3d_style.g_sensor_anim = MMISET_G_SENSOR_SWITCH_DEFAULT;

	s_mmi_3d_style.mainmenu_3d_style = MMISET_3D_STYLE_MAINMENU_NONE;
    s_mmi_3d_style.screen_3d_style = MMISET_3D_STYLE_SCREEN_DEFAULT;
    s_mmi_3d_style.list_3d_style = FALSE;
    s_mmi_3d_style.g_sensor_anim = MMISET_G_SENSOR_SWITCH_DEFAULT;

    MMINV_WRITE(MMINV_SET_3D_STYLE, &s_mmi_3d_style);
}
#endif
/*****************************************************************************/
// 	Description : get idle display substyle
//	Global resource dependence : none
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC MMISET_IDLE_SUBSTYLE_E MMIAPISET_GetIdleSubStyle(void)
{
    return (s_mmiset_idle_substyle);
}

/*****************************************************************************/
// 	Description : Set idle display substyle
//	Global resource dependence : none
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void SetIdleSubStyle(
						   MMISET_IDLE_SUBSTYLE_E     idle_substyle
						   )
{
    //write nv
    MMINV_WRITE(MMINV_SET_IDLE_SUBSTYLE,&idle_substyle);

    s_mmiset_idle_substyle = idle_substyle;
}

/*****************************************************************************/
// 	Description : get idle display style
//	Global resource dependence : none
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC MMISET_IDLE_STYLE_E MMIAPISET_GetIdleStyle(void)
{
    return (s_mmiset_idle_style);
}

/*****************************************************************************/
// 	Description : Set idle display style
//	Global resource dependence : none
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetIdleStyle(
                                MMISET_IDLE_STYLE_E     idle_style
                                )
{
	MMISET_SetIdleAndSubStyle(idle_style, MMISET_IDLE_SUBSTYLE_DEFAULT);
}

/*****************************************************************************/
//Description : Set default system's wallpaperNV informatioin;
//Global resource dependence : 
//Author: Glen.Li
//Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetDefaultWallpaperNV(void)
{
    uint32 i = 0;
    MMISET_WALLPAPER_SET_T wallpaper_info[MMISET_IDLE_STYLE_MAX+1] = {0};

    for(i = MMISET_IDLE_STYLE_COMMON; i < (MMISET_IDLE_STYLE_MAX+1); i++)
    {  
        SCI_MEMSET(&wallpaper_info[i], 0x00, sizeof(MMISET_WALLPAPER_SET_T));

        wallpaper_info[i].wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;
        wallpaper_info[i].wallpaper_id = MMIAPISET_GetDefaultWallpaperIDByStyle();
    }

    MMINV_WRITE(MMINV_SET_WALLPAPER_INFO, wallpaper_info);
}

/*****************************************************************************/
//Description : Get wallpaperNV informatioin for different systems;
//Global resource dependence : 
//Author: Glen.Li
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetWallpaperNVByStyle(MMISET_WALLPAPER_SET_T * wallpaper_info_ptr)
{
    BOOLEAN return_value = FALSE;
    MMISET_IDLE_STYLE_E  idle_style = MMIAPISET_GetIdleStyle();
    MMISET_IDLE_SUBSTYLE_E idle_substyle = MMIAPISET_GetIdleSubStyle();

    SCI_TRACE_LOW("MMIAPISET_GetWallpaperNVByStyle: idle_style = %d,idle_substyle = %d,wallpaper_type = %d,wallpaper_id = %d",idle_style,idle_substyle,wallpaper_info_ptr->wallpaper_type,wallpaper_info_ptr->wallpaper_id);
    return_value = MMIAPISET_GetWallpaperNVByStyleEx(idle_style,idle_substyle,wallpaper_info_ptr);

    return return_value;
}

/*****************************************************************************/
//Description : Get wallpaperNV informatioin for different systems;
//Global resource dependence : 
//Author: Glen.Li
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetWallpaperNVByStyleEx(MMISET_IDLE_STYLE_E idle_style, MMISET_IDLE_SUBSTYLE_E idle_substyle, MMISET_WALLPAPER_SET_T * wallpaper_info_ptr)
{
    MMISET_WALLPAPER_SET_T wallpaper_info[MMISET_IDLE_STYLE_MAX+1] = {0};
    uint32 return_value = MN_RETURN_FAILURE;

    if(idle_style >= (MMISET_IDLE_STYLE_MAX+1))
    {
        SCI_TRACE_LOW("MMIAPISET_GetWallpaperNVByStyleEx error idle_style = %d",idle_style);
        return FALSE;
    }

    MMINV_READ(MMINV_SET_WALLPAPER_INFO, wallpaper_info, return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        MMIAPISET_SetDefaultWallpaperNV();

        MMINV_READ(MMINV_SET_WALLPAPER_INFO, wallpaper_info, return_value);
    }
    
    if ((idle_style < MMISET_IDLE_STYLE_MAX) 
#ifdef MMI_SAM_THEME    
        && (MMISET_IDLE_SUBSTYLE_SAM != idle_substyle)
#endif
        )
    {
        wallpaper_info_ptr->wallpaper_type = wallpaper_info[idle_style].wallpaper_type;
        wallpaper_info_ptr->wallpaper_id = wallpaper_info[idle_style].wallpaper_id;
    }
#ifdef MMI_SAM_THEME
    else if ((MMISET_IDLE_STYLE_GRID == idle_style) && (MMISET_IDLE_SUBSTYLE_SAM == idle_substyle))
    {  
        wallpaper_info_ptr->wallpaper_type = wallpaper_info[MMISET_IDLE_STYLE_MAX].wallpaper_type;
        wallpaper_info_ptr->wallpaper_id = wallpaper_info[MMISET_IDLE_STYLE_MAX].wallpaper_id;

    }
#endif
    
    return TRUE;
}

/*****************************************************************************/
//Description : Set wallpaperNV informatioin for different systems;
//Global resource dependence : 
//Author: Glen.Li
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetWallpaperNVByStyle(MMISET_WALLPAPER_SET_T * wallpaper_info_ptr)
{
    BOOLEAN return_value = FALSE;
    MMISET_IDLE_STYLE_E  idle_style = MMIAPISET_GetIdleStyle();
    MMISET_IDLE_SUBSTYLE_E idle_substyle = MMIAPISET_GetIdleSubStyle();

    SCI_TRACE_LOW("MMISET_SetWallpaperNVByStyle: idle_style = %d,idle_substyle = %d,wallpaper_type = %d,wallpaper_id = %d",idle_style,idle_substyle,wallpaper_info_ptr->wallpaper_type,wallpaper_info_ptr->wallpaper_id);
    return_value = MMISET_SetWallpaperNVByStyleEx(idle_style,idle_substyle,wallpaper_info_ptr);

    return return_value; 
}

/*****************************************************************************/
//Description : Set wallpaperNV informatioin for different systems;
//Global resource dependence : 
//Author: Glen.Li
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetWallpaperNVByStyleEx(MMISET_IDLE_STYLE_E idle_style, MMISET_IDLE_SUBSTYLE_E idle_substyle, MMISET_WALLPAPER_SET_T * wallpaper_info_ptr)
{
    uint32 return_value = MN_RETURN_FAILURE;
    MMISET_WALLPAPER_SET_T wallpaper_info[MMISET_IDLE_STYLE_MAX+1] = {0};

    SCI_TRACE_LOW("MMISET_SetWallpaperNVByStyleEx- idle_style = %d",idle_style);

    if (PNULL == wallpaper_info_ptr
        || idle_style >= (MMISET_IDLE_STYLE_MAX+1))
    {
        return FALSE;
    }

    MMINV_READ(MMINV_SET_WALLPAPER_INFO, wallpaper_info, return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        MMIAPISET_SetDefaultWallpaperNV();

        MMINV_READ(MMINV_SET_WALLPAPER_INFO, wallpaper_info, return_value);
    }

    if ((idle_style < MMISET_IDLE_STYLE_MAX) 
#ifdef MMI_SAM_THEME    
        && (MMISET_IDLE_SUBSTYLE_SAM != idle_substyle)
#endif
        )
    {
        SCI_MEMSET(&wallpaper_info[idle_style], 0x00, sizeof(MMISET_WALLPAPER_SET_T));
        wallpaper_info[idle_style].wallpaper_type = wallpaper_info_ptr->wallpaper_type;
        wallpaper_info[idle_style].wallpaper_id = wallpaper_info_ptr->wallpaper_id;
    }
#ifdef MMI_SAM_THEME
    else if ((MMISET_IDLE_STYLE_GRID == idle_style) && (MMISET_IDLE_SUBSTYLE_SAM == idle_substyle))
    {  
        SCI_MEMSET(&wallpaper_info[MMISET_IDLE_STYLE_MAX], 0x00, sizeof(MMISET_WALLPAPER_SET_T));
        wallpaper_info[MMISET_IDLE_STYLE_MAX].wallpaper_type = wallpaper_info_ptr->wallpaper_type;
        wallpaper_info[MMISET_IDLE_STYLE_MAX].wallpaper_id = wallpaper_info_ptr->wallpaper_id;
    }
#endif

    MMINV_WRITE(MMINV_SET_WALLPAPER_INFO, wallpaper_info);

    return TRUE;
}

/*****************************************************************************/
// 	Description : Set idle display style
//	Global resource dependence : none
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetIdleAndSubStyle(
                                MMISET_IDLE_STYLE_E     idle_style,
                                MMISET_IDLE_SUBSTYLE_E     idle_substyle
                                )
{

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
    static MMISET_KEY_LOCK_EFFECT_E s_pre_lock_effect = MMISET_KEY_LOCK_EFFECT_MAX; 
#endif

    MMISET_WALLPAPER_SET_T      wallpaper_info  =   {0};

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
    //keep old effect 
    s_pre_lock_effect = MMIAPISET_GetKeyLockEffect(); 
#endif
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
    //Destroy livewallpaper before switching system, otherwise MMISET_SetWallpaperNVByStyleEx() in this function will set nv to fixed wallpaper,we can't stop livewallpaper.
    if(MMIAPIIDLE_IsLiveWallpaper())
    {
        MMIAPILIVEWALLPAPER_Stop();
        MMIAPILIVEWALLPAPER_Destroy(TRUE);
    }
#endif
    // SCI_ASSERT((MMISET_IDLE_STYLE_COMMON <= idle_style) && (MMISET_IDLE_STYLE_MAX > idle_style));
    if((MMISET_IDLE_STYLE_COMMON > idle_style) || (MMISET_IDLE_STYLE_MAX <= idle_style))
    {
        //SCI_TRACE_LOW:"mmisetting_assert MMISET_SetIdleStyle idle_style %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_2682_112_2_18_2_50_56_156,(uint8*)"d", idle_style);
        return;
    }


#ifdef BAR_PHONE_WIDGET 
    {
        MMISET_IDLE_STYLE_E     idle_style_temp;
        idle_style_temp = MMIAPISET_GetIdleStyle();
        MMINV_WRITE(MMINV_SET_IDLE_FORMER_STYLE,&idle_style_temp);
    }
#endif


    //write nv
    MMINV_WRITE(MMINV_SET_IDLE_STYLE,&idle_style);

#ifdef MMIWIDGET_SUPPORT
    if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle() && MMISET_IDLE_STYLE_WIDGET != idle_style)
    {
        MMIAPIWIDGET_DestroyIdleWidget();
    }

    if (MMISET_IDLE_STYLE_WIDGET != MMIAPISET_GetIdleStyle() && MMISET_IDLE_STYLE_WIDGET == idle_style)
    {
        MMIAPIWIDGET_SetIdleWidgetStyle(TRUE);
    }
#endif   

#ifdef MMI_GRID_IDLE_SUPPORT
    if ((MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle() && MMISET_IDLE_STYLE_GRID != idle_style)
        || (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle() && MMISET_IDLE_STYLE_GRID == idle_style && MMIAPISET_GetIdleSubStyle() != idle_substyle))
    {
        MMIAPIGRID_DestroyIdle();
    }

    if(MMISET_IDLE_STYLE_GRID == idle_style&&MMISET_IDLE_SUBSTYLE_DEFAULT==idle_substyle)
    {
        #ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
        //reset to old lock effect 
       // #if 1//def MMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT
        MMIAPISET_SetKeyLockEffect(MMISET_KEY_LOCK_DEFAULT);
       // #else
       // MMIAPISET_SetKeyLockEffect(s_pre_lock_effect);
       // #endif
        #endif
    }
#endif 

#ifdef QBTHEME_SUPPORT
    if (MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle() && MMISET_IDLE_QBTHEME != idle_style)
    {
        MMIAPIQBTHEME_DestroyIdle();
    }

    if (MMISET_IDLE_QBTHEME != MMIAPISET_GetIdleStyle() && MMISET_IDLE_QBTHEME == idle_style)
    {
#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
#ifdef MMI_PDA_SUPPORT		
        //set keylock
#ifdef MMI_KEY_LOCK_DOUBLE_CIRCLE_SUPPORT
        MMIAPISET_SetKeyLockEffect( MMISET_KEY_LOCK_DOUBLE_CIRCLE );
#else
        MMIAPISET_SetKeyLockEffect( MMISET_KEY_LOCK_EFFECT1 );
#endif

#endif
#endif
    }
#endif 


#ifdef MMI_PDA_SUPPORT
    //wallpaper restoring between systems;-start;
    
    //set current styel's wallpaper      
#if defined MMI_ISTYLE_SUPPORT     
    if(MMISET_IDLE_ISTYLE == idle_style)
    {
        wallpaper_info.wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;
        wallpaper_info.wallpaper_id = MMISET_WALLPAPER_DEFAULT_ID + 1; 

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
        //set keylock
        MMIAPISET_SetKeyLockEffect( MMISET_KEY_LOCK_ISTYLE );
#endif
    }
    else
#endif		
    {
        MMISET_WALLPAPER_SET_T      wallpaper_info_store  =   {0};
        if(!MMIAPISET_GetWallpaperNVByStyleEx(idle_style,idle_substyle,&wallpaper_info_store))
        {
            return;
        }
        
        wallpaper_info.wallpaper_type = wallpaper_info_store.wallpaper_type;
        
        if(MMISET_WALLPAPER_FIXED_PIC == wallpaper_info_store.wallpaper_type
#if defined LIVE_WALLPAPER_FRAMEWORK_SUPPORT 
                || MMISET_WALLPAPER_LIVE == wallpaper_info_store.wallpaper_type
#endif
            )
        {        
            wallpaper_info.wallpaper_id = wallpaper_info_store.wallpaper_id;
        }
        else
        {
            wallpaper_info.wallpaper_id = MMISET_WALLPAPER_DEFAULT_ID;
        }
    }

    //Set new wallpaper's id and type to nv;
    //MMIAPISET_SetWallpaperSetInfo(wallpaper_info);
    if(!MMISET_SetWallpaperNVByStyleEx(idle_style,idle_substyle,&wallpaper_info))
    {
        return;
    }

	//wallpaper restoring between systems;-end;
#endif

    s_mmiset_idle_style = idle_style;
	SetIdleSubStyle(idle_substyle);	

    //after switching system, then set wallpaper. 
    MMIAPISET_SetWallpaperSetInfo(wallpaper_info);
    
#ifdef MMI_SAM_THEME
	if ((MMISET_IDLE_STYLE_GRID == idle_style) && (MMISET_IDLE_SUBSTYLE_SAM == idle_substyle))
	{
		MMISET_SetCurrentTheme(1, PNULL);

        //set keylock
#ifdef MMI_KEY_LOCK_SPEC_DOUBLE_CIRCLE_SUPPORT
        MMIAPISET_SetKeyLockEffect( MMISET_KEY_LOCK_DOUBLE_CIRCLE_SPEC );
#else
        MMIAPISET_SetKeyLockEffect( s_pre_lock_effect );
#endif
	}
	else
	{
		MMISET_SetCurrentTheme(0, PNULL);
	}
#endif
}

/*****************************************************************************/
// 	Description : reset idle display style
//	Global resource dependence : none
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_ResetIdleStyle(void)
{
#ifdef DPHONE_SUPPORT 
    MMISET_SetIdleStyle(MMISET_IDLE_STYLE_COMMON);
#else
    #ifdef BAR_PHONE_WIDGET
        #ifdef MMI_COMMON_IDLE_SUPPORT
            MMISET_SetIdleStyle(MMISET_IDLE_STYLE_COMMON);//NEWMS00202216
        #else
            MMISET_SetIdleStyle(MMISET_IDLE_STYLE_SMART);
        #endif
    #else
        #ifdef MMI_GRID_IDLE_SUPPORT
            MMISET_SetIdleStyle(MMISET_IDLE_STYLE_GRID);
        #elif defined MMIWIDGET_SUPPORT
            MMISET_SetIdleStyle(MMISET_IDLE_STYLE_WIDGET);
        #elif defined(MMI_COMMON_IDLE_SUPPORT)
            MMISET_SetIdleStyle(MMISET_IDLE_STYLE_COMMON);
        #else
            MMISET_SetIdleStyle(MMISET_IDLE_STYLE_SMART);
        #endif    
    #endif
#endif   
#ifdef CMCC_UI_STYLE
    MMISET_SetIdleStyle(MMISET_IDLE_STYLE_SMART);
#endif
}

/*****************************************************************************/
//  Description : update wallpaper
//  Global resource dependence : none
//  Author: jianhui.luo
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_UpdateWallpaper(void)
{
    BOOLEAN                         result = FALSE;
    uint32                          file_size = 0;
    SFS_DATE_T			            modify_date = {0};
    SFS_TIME_T			            modify_time = {0};
    MMIMULTIM_AS_WALLPAPER_INFO     wallpaper_file_ptr = {0};
    
    //get file name info
#if defined MMI_WALLPAPER_SUPPORT
    MMIAPIFMM_GetWallPaperInfo(&wallpaper_file_ptr);
#endif
    //墙纸文件是否被修改
    if (MMIAPIFMM_GetFileInfo(wallpaper_file_ptr.full_path_name,
            wallpaper_file_ptr.full_path_len,
            &file_size,
            &modify_date,
            &modify_time))
    {
        if ((modify_date.mday != wallpaper_file_ptr.wallpaper_modify_date.mday) || 
            (modify_date.mon != wallpaper_file_ptr.wallpaper_modify_date.mon) || 
            (modify_date.year != wallpaper_file_ptr.wallpaper_modify_date.year) || 
            (modify_time.hour != wallpaper_file_ptr.wallpaper_modify_time.hour) || 
            (modify_time.min !=  wallpaper_file_ptr.wallpaper_modify_time.min) || 
            (modify_time.sec !=  wallpaper_file_ptr.wallpaper_modify_time.sec))
        {
            //modify wallpaper date,time etc.
            wallpaper_file_ptr.wallpaper_modify_date.mday = modify_date.mday;
            wallpaper_file_ptr.wallpaper_modify_date.mon  = modify_date.mon;
            wallpaper_file_ptr.wallpaper_modify_date.year = modify_date.year;
            wallpaper_file_ptr.wallpaper_modify_time.hour = modify_time.hour;
            wallpaper_file_ptr.wallpaper_modify_time.min  = modify_time.min;
            wallpaper_file_ptr.wallpaper_modify_time.sec  = modify_time.sec;
#if defined MMI_WALLPAPER_SUPPORT
            MMIAPIFMM_WriteWallPaperNV(&wallpaper_file_ptr);

            //update wallpaper
            MMIAPIIDLE_SetWallpaper(TRUE);
#endif

            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get date string by date style
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_FormatDateStrByDateStyle(
                                            uint16 year, //in
                                            uint8 month, //in
                                            uint8 date, //in
                                            char symbol, //in
                                            uint8* date_str,//in/out
                                            uint16 date_str_max_len//in
                                            )
{
    MMISET_DATE_DISPLAY_TYPE_E  date_display_type   = MMISET_DATE_YMD;
    BOOLEAN result = FALSE;
    char sym_str[2] = {0};
    sym_str[0] = symbol;

    if(!(1 <= month && 12 >= month))
    {
        //SCI_TRACE_LOW:"Waring, MMIAPISET_FormatDateStrByDateStyle: month = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_2873_112_2_18_2_50_57_157,(uint8*)"d", month);
        //return result;
    }
    if(!(1 <= date && 31 >= date))
    {
        //SCI_TRACE_LOW:"Waring,MMIAPISET_FormatDateStrByDateStyle: date = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_2878_112_2_18_2_50_57_158,(uint8*)"d",date);
        //return result;
    }
    if (PNULL ==date_str|| date_str_max_len < MMISET_DATE_STR_LEN)
    {
        //SCI_TRACE_LOW:"Waring,MMIAPISET_FormatDateStrByDateStyle: in len = %d,min len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_2883_112_2_18_2_50_57_159,(uint8*)"dd",date_str_max_len,MMISET_DATE_STR_LEN);
        return result;
    }
    date_display_type = MMIAPISET_GetDateDisplayType();
    SCI_MEMSET(date_str,0,date_str_max_len);
    switch(date_display_type)
    {
        case MMISET_DATE_YMD:
            sprintf((char*) date_str, "%04d%s%02d%s%02d", year, sym_str,month, sym_str, date);
            result = TRUE;
            break;
        case MMISET_DATE_MDY:
            sprintf((char*) date_str, "%02d%s%02d%s%04d", month, sym_str, date, sym_str, year);
            result = TRUE;
            break;
        case MMISET_DATE_DMY:
            sprintf((char*) date_str, "%02d%s%02d%s%04d", date, sym_str, month, sym_str, year);
            result = TRUE;
            break;
        default:
            break;
    }
    return result;
}

/*****************************************************************************/
//  Description : get date string by date style
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_FormatDateStrByDateEx(
                                            uint16 year, //in
                                            uint8 month, //in
                                            uint8 date, //in
                                            char symbol, //in
                                            uint8* date_str,//in/out
                                            uint16 date_str_max_len,//in
                                            BOOLEAN is_display_year
                                            )
{
    MMISET_DATE_DISPLAY_TYPE_E  date_display_type   = MMISET_DATE_YMD;
    BOOLEAN result = FALSE;
    char sym_str[2] = {0};
    sym_str[0] = symbol;

    if(!(1 <= month && 12 >= month))
    {
        //SCI_TRACE_LOW:"Waring, MMIAPISET_FormatDateStrByDateEx: month = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_2929_112_2_18_2_50_57_160,(uint8*)"d", month);
        //return result;
    }
    if(!(1 <= date && 31 >= date))
    {
        //SCI_TRACE_LOW:"Waring,MMIAPISET_FormatDateStrByDateEx: date = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_2934_112_2_18_2_50_57_161,(uint8*)"d",date);
        //return result;
    }
    if (PNULL ==date_str|| date_str_max_len < MMISET_DATE_STR_LEN)
    {
        //SCI_TRACE_LOW:"Waring,MMIAPISET_FormatDateStrByDateEx: in len = %d,min len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_2939_112_2_18_2_50_57_162,(uint8*)"dd",date_str_max_len,MMISET_DATE_STR_LEN);
        return result;
    }
    date_display_type = MMIAPISET_GetDateDisplayType();
    SCI_MEMSET(date_str,0,date_str_max_len);
    switch(date_display_type)
    {
    case MMISET_DATE_YMD:
        if (is_display_year)
        {
            sprintf((char*) date_str, "%04d%s%02d%s%02d", year, sym_str,month, sym_str, date);
        }
        else
        {
            sprintf((char*) date_str, "%02d%s%02d", month, sym_str, date);
        }
        result = TRUE;
        break;
    case MMISET_DATE_MDY:
        if (is_display_year)
        {
            sprintf((char*) date_str, "%02d%s%02d%s%04d", month, sym_str, date, sym_str, year);
        }
        else
        {
            sprintf((char*) date_str, "%02d%s%02d", month, sym_str, date);
        }
        result = TRUE;
        break;
    case MMISET_DATE_DMY:
        if (is_display_year)
        {
            sprintf((char*) date_str, "%02d%s%02d%s%04d", date, sym_str, month, sym_str, year);
        }
        else
        {
            sprintf((char*) date_str, "%02d%s%02d", date, sym_str, month);
        }
        result = TRUE;
        break;
    default:
        break;
    }
    return result;
}

/*****************************************************************************/
// 	Description : format time string by time
//	Global resource dependence : 
//  Author:    jian.ma
//	Note:    
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_FormatTimeStrByTime(
					  uint8 hour,//in
					  uint8 min,//in
					  uint8* timestr,//in/out
					  uint16 time_str_max_len//in
					  )
{
	MMISET_TIME_DISPLAY_TYPE_E time_type = MMISET_TIME_12HOURS;
    BOOLEAN result = FALSE;

    if( PNULL == timestr ||time_str_max_len < MMISET_TIME_STR_12HOURS_LEN + 1 )
    {
        return result;
    }
    time_type = MMIAPISET_GetTimeDisplayType();

    SCI_MEMSET(timestr,0,time_str_max_len);
    
    switch (time_type)
    {
    case MMISET_TIME_12HOURS:
        {
            if ((MMISET_TIME_PM_HOUR > hour))//AM
            {
                if ((MMISET_TIME_1_HOUR> hour))//00:00-1:00
                {
                    sprintf((char*)timestr,"%02d:%02d%s",(hour + MMISET_TIME_PM_HOUR),min,"AM");
                }
                else
                {
                    sprintf((char*)timestr,"%02d:%02d%s",hour,min,"AM");
                }
            }
            else//PM
            {
                if ((MMISET_TIME_PM_HOUR <= hour) && (MMISET_TIME_13_HOUR > hour))//12:00-13:00
                {
                    sprintf((char*)timestr, "%02d:%02d%s", hour, min, "PM");
                }
                else
                {
                    sprintf((char*)timestr, "%02d:%02d%s", (hour - MMISET_TIME_PM_HOUR), min, "PM");
                }
            }
            result = TRUE;
        }
        break;

    case MMISET_TIME_24HOURS:
        {
            sprintf((char*)timestr,"%02d:%02d", hour, min);
        }
        result = TRUE;
        break;

    default:
        break;
    }
    return result;
}
/*****************************************************************************/
// 	Description : data display string
//	Global resource dependence : none
//  Author: SCV
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_GetCurrentDateStr(
                                        uint8* datestr,//in/out
                                        char symbol, //in
                                        uint16 datestr_len //in
                                        )
{
	SCI_DATE_T                  sys_date = {0};
    
    /*if (MMIAPISET_IsNeedResetDateTime())
    {
        sys_date.mday = 1;
        sys_date.mon = 1;
        sys_date.wday = 2;
        sys_date.year = 1980;
    }
	else*/
	{
		TM_GetSysDate(&sys_date);
	}

    MMIAPISET_FormatDateStrByDateStyle(sys_date.year,sys_date.mon,sys_date.mday,symbol,datestr,datestr_len);
}

#ifdef LCD_SLIDE_SUPPORT

/*****************************************************************************/
// 	Description : add slide open radio list item
//	Global resource dependence : 
//  Author: jianhui.luo
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendSlideOpenTypeListItem(
                                                 MMI_CTRL_ID_T	    ctrl_id
                                                 )
{
    uint16						cur_selection = 0;
    MMISET_SLIDE_OPEN_TYPE_E	slide_open_type = MMISET_RECEIVE_PHONE_TYPE;
    
    //add items to listbox
    MMIAPISET_AppendListItemByTextId( TXT_SET_RECEIVE_PHONE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_SET_REJECT_PHONE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    slide_open_type = MMIAPISET_GetSlideOpenType();
    
    cur_selection=(uint16)slide_open_type;
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}

/*****************************************************************************/
//  Description : get the type of slide open type
//  Global resource dependence : none
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC MMISET_SLIDE_OPEN_TYPE_E MMIAPISET_GetSlideOpenType(void)
{
    MN_RETURN_RESULT_E				return_value = MN_RETURN_FAILURE;
    MMISET_SLIDE_OPEN_TYPE_E		slide_open_type = MMISET_RECEIVE_PHONE_TYPE;
    
    MMINV_READ(MMINV_SET_SLIDE_OPEN_TYPE, &slide_open_type, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        slide_open_type= MMISET_RECEIVE_PHONE_TYPE;
        MMINV_WRITE(MMINV_SET_SLIDE_OPEN_TYPE, &slide_open_type);
    }
    
    return slide_open_type;
}

/*****************************************************************************/
// 	Description : set slide open type 
//	Global resource dependence : none
//  Author: jianhui.luo
//	Note:
/*****************************************************************************/
LOCAL void SetSlideOpenType(
                              MMISET_SLIDE_OPEN_TYPE_E slide_open_type
                              )
{
    //SCI_TRACE_LOW:"SetSlideOpenType slide_open_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_3131_112_2_18_2_50_57_163,(uint8*)"d",slide_open_type);
    MMINV_WRITE(MMINV_SET_SLIDE_OPEN_TYPE, &slide_open_type);
}

/*****************************************************************************/
// 	Description : add slide close radio list item
//	Global resource dependence : 
//  Author: jianhui.luo
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendSlideCloseTypeListItem(
                                                 MMI_CTRL_ID_T	    ctrl_id
                                                 )
{
    uint16						cur_selection = 0;
    MMISET_SLIDE_CLOSE_TYPE_E	slide_close_type = MMISET_STOP_OPERATION_TYPE;
    
    //add items to listbox
    MMIAPISET_AppendListItemByTextId( TXT_SET_STOP_OPERATION, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_SET_CONTINUE_OPERATION, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    slide_close_type = MMIAPISET_GetSlideCloseType();
    
    cur_selection=(uint16)slide_close_type;
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}

/*****************************************************************************/
//  Description : get the type of slide close type
//  Global resource dependence : none
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC MMISET_SLIDE_CLOSE_TYPE_E MMIAPISET_GetSlideCloseType(void)
{
    MN_RETURN_RESULT_E				return_value = MN_RETURN_FAILURE;
    MMISET_SLIDE_CLOSE_TYPE_E		slide_close_type = MMISET_STOP_OPERATION_TYPE;
    
    MMINV_READ(MMINV_SET_SLIDE_CLOSE_TYPE, &slide_close_type, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        slide_close_type= MMISET_STOP_OPERATION_TYPE;
        MMINV_WRITE(MMINV_SET_SLIDE_CLOSE_TYPE, &slide_close_type);
    }
    
    return slide_close_type;
}

/*****************************************************************************/
// 	Description : set slide close type 
//	Global resource dependence : none
//  Author: jianhui.luo
//	Note:
/*****************************************************************************/
LOCAL void SetSlideCloseType(
                              MMISET_SLIDE_CLOSE_TYPE_E slide_close_type
                              )
{
    //SCI_TRACE_LOW:"SetSlideCloseType slide_close_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_3188_112_2_18_2_50_57_164,(uint8*)"d",slide_close_type);
    MMINV_WRITE(MMINV_SET_SLIDE_CLOSE_TYPE, &slide_close_type);
}

/*****************************************************************************/
// 	Description : add keylock switch radio list item
//	Global resource dependence : 
//  Author: jianhui.luo
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendKeylockSwitchTypeListItem(
                                                 MMI_CTRL_ID_T	    ctrl_id
                                                 )
{
    uint16						cur_selection = 0;
    MMISET_KEYLOCK_SWITCH_TYPE_E	keylock_switch_type = MMISET_KEYLOCK_SWITCH_ON_TYPE;
    
    //add items to listbox
    MMIAPISET_AppendListItemByTextId( TXT_SET_KEYLOCK_SWITCH_ON, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_SET_KEYLOCK_SWITCH_OFF, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    
    keylock_switch_type = MMIAPISET_GetKeylockSwitchType();
    
    cur_selection=(uint16)keylock_switch_type;
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}

/*****************************************************************************/
//  Description : get the type of keylock switch type
//  Global resource dependence : none
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC MMISET_KEYLOCK_SWITCH_TYPE_E MMIAPISET_GetKeylockSwitchType(void)
{
    MN_RETURN_RESULT_E				return_value = MN_RETURN_FAILURE;
    MMISET_KEYLOCK_SWITCH_TYPE_E    keylock_switch_type = MMISET_KEYLOCK_SWITCH_ON_TYPE;
    
    MMINV_READ(MMINV_SET_KEYLOCK_SWITCH_TYPE, &keylock_switch_type, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        keylock_switch_type= MMISET_KEYLOCK_SWITCH_ON_TYPE;
        MMINV_WRITE(MMINV_SET_KEYLOCK_SWITCH_TYPE, &keylock_switch_type);
    }
    
    return keylock_switch_type;
}

/*****************************************************************************/
// 	Description : set keylock switch type 
//	Global resource dependence : none
//  Author: jianhui.luo
//	Note:
/*****************************************************************************/
LOCAL void SetKeylockSwitchType(
                              MMISET_KEYLOCK_SWITCH_TYPE_E keylock_switch_type
                              )
{
    //SCI_TRACE_LOW:"SetKeylockSwitchType keylock_switch_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_3245_112_2_18_2_50_57_165,(uint8*)"d",keylock_switch_type);
    MMINV_WRITE(MMINV_SET_KEYLOCK_SWITCH_TYPE, &keylock_switch_type);
}

/*****************************************************************************/
// 	Description : set the selected type of slide open 
//	Global resource dependence : none
//  Author: jianhui.luo
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetSelectSlideOpenType(
                                           MMISET_SLIDE_OPEN_TYPE_E slide_open_type
                                           )
{
    SetSlideOpenType(slide_open_type);
}

/*****************************************************************************/
// 	Description : set the selected type of slide close 
//	Global resource dependence : none
//  Author: jianhui.luo
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetSelectSlideCloseType(
                                           MMISET_SLIDE_CLOSE_TYPE_E slide_close_type
                                           )
{
    SetSlideCloseType(slide_close_type);
}

/*****************************************************************************/
// 	Description : set the selected type of keylock switch 
//	Global resource dependence : none
//  Author: SCV
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetSelectKeylockSwitchType(
                                           MMISET_KEYLOCK_SWITCH_TYPE_E keylock_switch_type
                                           )
{
    SetKeylockSwitchType(keylock_switch_type);
}

#endif
#ifdef SET_3D_STYLE_SUPPORT //#ifdef MCARE_V31_SUPPORT ||EFFECT_3D_MAIN_MENU_SUPPORT ||!MMI_DISPLAYSET_MINI_SUPPORT
/*****************************************************************************/
// 	Description : 判断是否有效NV 3D style（在NV被异常破坏的情况下，恢复默认值）
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsValid3DStyle(
                             MMISET_3D_STYLE_T     *mmi_3d_style_ptr
                             )
{    
    BOOLEAN result = FALSE;

    //SCI_ASSERT(mmi_3d_style_ptr);
    if(PNULL == mmi_3d_style_ptr)
    {
        //SCI_TRACE_LOW:"mmisetting_assert IsValid3DStyle"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_3297_112_2_18_2_50_57_166,(uint8*)"");
        return FALSE;
    }

    if (mmi_3d_style_ptr->mainmenu_3d_style < MMISET_3D_STYLE_MAINMENU_MAX)
    {
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"IsValid3DStyle() NV abnormal!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_3307_112_2_18_2_50_58_167,(uint8*)"");
    }
    return result;
}
#endif
/*****************************************************************************/
// 	Description : 展开GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT的list
//	Global resource dependence : 
//  Author: juan.wu
//	Note:
/*****************************************************************************/
/*PUBLIC void MMISET_AppendTwoLineAnimListItem(  
                            MMI_CTRL_ID_T	ctrl_id,
                            MMI_TEXT_ID_T        left_softkey_id,
                            MMI_TEXT_ID_T        middle_softkey_id,
                            MMI_TEXT_ID_T        right_softkey_id
                            )
{
    GUILIST_ITEM_T  item_t ={0};
    GUILIST_ITEM_DATA_T     item_data= {0};
    GUIANIM_DATA_INFO_T        anim_data = {0};
	uint16 i = 0;
    MMI_IMAGE_ID_T  image_id[5]=
    {
        IMAGE_THEME_BG,
        IMAGE_WALLPAPER_1,
        IMAGE_WALLPAPER_2,
        IMAGE_WALLPAPER_3,
        IMAGE_WALLPAPER_3
    };
    MMI_TEXT_ID_T   text_id[5]=
    {
        TXT_EXPORT_LOC_DEFAULT,
        TXT_SET_WALLPAPER_1,
        TXT_SET_WALLPAPER_2,
        TXT_SET_WALLPAPER_3,
        TXT_SET_WALLPAPER_MORE
    };
    item_t.item_data_ptr = &item_data;
    item_t.item_data_ptr->item_content->item_data.anim_data_ptr = &anim_data;
    for(i=0;i<5;i++)
    {
        item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT;

        item_t.item_data_ptr->item_content[0].item_data_type = GUIITEM_DATA_ANIM_DATA;
        item_t.item_data_ptr->item_content[0].item_data.anim_data_ptr->img_id= image_id[i];
        
        item_t.item_data_ptr->item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_t.item_data_ptr->item_content[1].item_data.text_id= text_id[i];

		GUILIST_AppendItem( ctrl_id, &item_t );  
    }
    
    item_t.item_data_ptr->softkey_id[0] = left_softkey_id;
    item_t.item_data_ptr->softkey_id[1] = middle_softkey_id;
    item_t.item_data_ptr->softkey_id[2] = right_softkey_id;
}*/

#ifdef MCARE_V31_SUPPORT
/*****************************************************************************/
// 	Description : 设置3窗口特效类型
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MCAREV31_Set3DStyleScreen(uint16 screen_3d_style)
{
    MMISET_3D_STYLE_T*   mmi_3d_style_ptr    =   PNULL;
#ifdef SET_3D_STYLE_SUPPORT    
    mmi_3d_style_ptr = Get3DStyle();
    mmi_3d_style_ptr->screen_3d_style = (MMISET_3D_STYLE_SCREEN_E)screen_3d_style;
    Set3DStyle(mmi_3d_style_ptr);
#endif
}
#endif

/*****************************************************************************/
//  Description :MMISET_CloseAllAnimSet
//  Global resource dependence : 
//  Author: juan.wu 
//  Note:关闭所有动画效果，供省电模式使用
/*****************************************************************************/
PUBLIC void MMISET_CloseAllAnimSet(void)
{
#ifdef SET_3D_STYLE_SUPPORT
    Set3DStyleScreen(MMISET_3D_STYLE_SCREEN_NONE);
#endif
#ifndef FLASH_SUPPORT
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
    Set3DStyleMainmenu(MMISET_3D_STYLE_MAINMENU_NONE);
#endif
#endif
#ifdef EFFECT_3D_LIST_ROLL_SUPPORT
    Set3DStyleList(0);
#endif
#ifdef MMI_RES_ORIENT_BOTH // 说明支持横竖屏切换
    SetGsensorSwitchAnimStyle(MMISET_G_SENSOR_SWITCH_NONE);
#endif
}

#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
/*****************************************************************************/
// 	Description : get main menu select style
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMENU_SELECT_EFFECT_STYLE_E MMIAPISET_GetMainmenuSelectStyle(void)
{
    MMIMENU_SELECT_EFFECT_STYLE_E     mainmenu_select_style = SELECT_EFFECT_ROUNDLIGHT;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    
    //get main menu Select style from nv
    MMINV_READ(MMINV_SET_MAIN_MENU_SELECT_STYLE,&mainmenu_select_style,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_SET_MAIN_MENU_SELECT_STYLE,&mainmenu_select_style);
    }

    return (mainmenu_select_style);    
}
/*****************************************************************************/
// 	Description : set main menu select style
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetMainmenuSelectStyle(
 MMIMENU_SELECT_EFFECT_STYLE_E  mainmenu_select_style
 )
{
     if (mainmenu_select_style >= SELECT_EFFECT_MAX)
     {
        return;
     }

     //set main menu style
     MMINV_WRITE(MMINV_SET_MAIN_MENU_SELECT_STYLE,&mainmenu_select_style);
}
#endif

#ifdef PDA_UI_SUPPORT_MANIMENU_GO
/*****************************************************************************/
// 	Description : get GO main menu style
//	Global resource dependence : 
//  Author: KYLE.JIN
//	Note:
/*****************************************************************************/
PUBLIC GO_MENU_STYLE_E MMIAPISET_GetGOmenuStyle(void)
{
    SCI_TRACE_LOW("MMIAPISET_GetGOmenuStyle %d",s_go_menu_style);
      
#if defined GO_FLASH_MAIN_MENU_SUPPORT && defined(MMI_ISTYLE_SUPPORT)     
    //iphone不支持上下滑动，改为classic
    if ( GO_MENU_STYLE_FLASH == s_go_menu_style && MMITHEME_IsIstyle())
    {
        return GO_MENU_STYLE_CLASSIC;
    }
#endif
    
    return s_go_menu_style;
}
 
/*****************************************************************************/
// 	Description : set main menu style
//	Global resource dependence : 
//  Author: kyle.jin
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetGOmenuStyle(
                                    GO_MENU_STYLE_E     go_menu_style
                                    )
{
    //set GO menu style
    s_go_menu_style = go_menu_style;
    MMINV_WRITE(MMINV_SET_GO_MENU_STYLE,&s_go_menu_style);
}
#endif

#ifdef PDA_UI_SUPPORT_U_IDLE
/*****************************************************************************/
// 	Description : get u idle effect style
//	Global resource dependence : 
//  Author: hongbo.lan
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISET_GetUIdleStyle(void)
{
    UIDLE_STYLE_E     uidle_style       =   UIDLE_STYLE_RANDOM;
    MN_RETURN_RESULT_E  return_value    =   MN_RETURN_FAILURE;

    //get GO menu style from nv
    MMINV_READ(MMINV_SET_UIDLE_STYLE, &uidle_style, return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_SET_UIDLE_STYLE, &uidle_style);
    }

    return (uint16)uidle_style;

}
 
/*****************************************************************************/
// 	Description : set u idle effect style
//	Global resource dependence : 
//  Author: hongbo.lan
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetUIdleStyle(uint16 uidle_style)
{
    if (uidle_style < UIDLE_STYLE_MAX)
    {
        //set uidle style
        MMINV_WRITE(MMINV_SET_UIDLE_STYLE, &uidle_style);        
    }
}
#endif

#if defined(PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER) 
/*****************************************************************************/
// 	Description : init the render effect 
//	Global resource dependence : 
//  Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL void InitLightShadowRenderSetting(void)
{
    MN_RETURN_RESULT_E      return_value    =   MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_RENDER_EFFECT,&s_render_setting,return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
#ifdef PDA_UI_SUPPORT_MANIMENU_GO
        s_render_setting.render_array[MMISET_EFFECT_RENDER_SETTING_GO_MENU]= FALSE;
#endif
        MMINV_WRITE(MMINV_SET_RENDER_EFFECT, &s_render_setting);
    }
}
/*****************************************************************************/
// 	Description : get the render effect 
//	Global resource dependence : 
//  Author: Arvin.wu
//	Note:
/*****************************************************************************/
PUBLIC MMISET_EFFECT_RENDER_SETTING* MMISET_GetLightShadowRenderSetting(void)
{
    return &s_render_setting;
}

/*****************************************************************************/
// 	Description : set the render effect 
//	Global resource dependence : 
//  Author: Arvin.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetLightShadowRenderSetting(MMISET_EFFECT_RENDER_SETTING  *render_setting_ptr)
{
    if(PNULL != render_setting_ptr)
    {
        SCI_MEMCPY(&s_render_setting,render_setting_ptr,sizeof(MMISET_EFFECT_RENDER_SETTING));
        MMINV_WRITE(MMINV_SET_RENDER_EFFECT, &s_render_setting);
    }
}
#endif

#ifdef BAR_PHONE_WIDGET 
PUBLIC MMISET_IDLE_STYLE_E MMIAPISET_GetPreIdleStyle(void)
{
    MN_RETURN_RESULT_E				return_value = MN_RETURN_FAILURE;
    MMISET_IDLE_STYLE_E     idle_style = MMISET_IDLE_STYLE_MAX;
    MMINV_READ(MMINV_SET_IDLE_FORMER_STYLE, &idle_style, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_SET_IDLE_FORMER_STYLE,&idle_style);
    }
    return idle_style;
}
#endif

/*****************************************************************************/
// 	Description : reset all styles' wallpaper at restoring Factory Setting.
//	Global resource dependence : 
//  Author: Glen.Li
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_ResetAllStylesWallpaperAtRestoreFactory(void)
{
    MMIAPISET_SetDefaultWallpaperNV();  
}

#ifdef ADULT_WATCH_SUPPORT
PUBLIC MMISET_WATCH_WEAR_E WATCHAPISET_GetWearType(void)
{
    MMISET_WATCH_WEAR_E wear_type = MMISET_WATCH_WEAR_TYPE_LEFT;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    
    //get wear type from nv
    MMINV_READ(MMINV_SET_WATCH_WEAR_TYPE,&wear_type,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_SET_WATCH_WEAR_TYPE,&wear_type);
    }

    return (wear_type);    
}

PUBLIC void WATCHAPISET_SetWearType(MMISET_WATCH_WEAR_E wear_type)
{
    MMINV_WRITE(MMINV_SET_WATCH_WEAR_TYPE,&wear_type);
}

PUBLIC BOOLEAN WATCHAPISET_GetRaiseWrist(void)
{
    BOOLEAN     is_raise_wrist_on = FALSE;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    
    //get wear type from nv
    MMINV_READ(MMINV_SET_WATCH_RAISE_WRIST,&is_raise_wrist_on,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_SET_WATCH_RAISE_WRIST,&is_raise_wrist_on);
    }

    return (is_raise_wrist_on);
}

PUBLIC void WATCHAPISET_SetRaiseWrist(BOOLEAN is_raise_wrist_on)
{
    MMINV_WRITE(MMINV_SET_WATCH_RAISE_WRIST,&is_raise_wrist_on);
}


#endif
//yangyu add begin
PUBLIC void MMISET_SetWatchPanelStyle(WATCH_PANEL_TYPE_T style)
{
    WATCH_PANEL_TYPE_T temp_style = style;
    MMINV_WRITE(MMINV_SET_WATCH_PANEL_STYLE, &temp_style);
}

PUBLIC WATCH_PANEL_TYPE_T MMISET_GetWatchPanelStyle()
{
#if !defined(ANALOG_CLK_SUPPORT)   //xiongkai WATCH_PANEL_BEAR_DOLL_SUPPORT
    WATCH_PANEL_TYPE_T temp_style = {0 , 7};
#else
    WATCH_PANEL_TYPE_T temp_style = {1 , 0};    //xiongkai  第一个元素  使用模拟还是数字时钟     第二个参数使用第几个模板
#endif
    MN_RETURN_RESULT_E  return_value    =   MN_RETURN_FAILURE;

    MMINV_READ(MMINV_SET_WATCH_PANEL_STYLE, &temp_style, return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_SET_WATCH_PANEL_STYLE, &temp_style);
    }
	SCI_TRACE_LOW("MMISET_GetWatchPanelStyle %d[%d] ",temp_style.is_anolog_panel,temp_style.watch_index);
	return temp_style;
}

PUBLIC BOOLEAN MMIAPISET_FormatMonDayDateStrByDateStyle(
                                            uint16 year, //in
                                            uint8 month, //in
                                            uint8 date, //in
                                            char symbol, //in
                                            uint8* date_str,//in/out
                                            uint16 date_str_max_len//in
                                            )
{
    MMISET_DATE_DISPLAY_TYPE_E  date_display_type   = MMISET_DATE_YMD;
    BOOLEAN result = FALSE;
    char sym_str[2] = {0};
    sym_str[0] = symbol;

    if(!(1 <= month && 12 >= month))
    {
        //SCI_TRACE_LOW:"Waring, MMIAPISET_FormatDateStrByDateStyle: month = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_2873_112_2_18_2_50_57_157,(uint8*)"d", month);
        //return result;
    }
    if(!(1 <= date && 31 >= date))
    {
        //SCI_TRACE_LOW:"Waring,MMIAPISET_FormatDateStrByDateStyle: date = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_2878_112_2_18_2_50_57_158,(uint8*)"d",date);
        //return result;
    }
    if (PNULL ==date_str|| date_str_max_len < MMISET_DATE_STR_LEN)
    {
        //SCI_TRACE_LOW:"Waring,MMIAPISET_FormatDateStrByDateStyle: in len = %d,min len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_DISPLAY_2883_112_2_18_2_50_57_159,(uint8*)"dd",date_str_max_len,MMISET_DATE_STR_LEN);
        return result;
    }
    date_display_type = MMIAPISET_GetDateDisplayType();
    SCI_MEMSET(date_str,0,date_str_max_len);
    switch(date_display_type)
    {
        case MMISET_DATE_YMD:
            sprintf((char*) date_str, "%02d%s%02d", month, sym_str, date);
            result = TRUE;
            break;
        case MMISET_DATE_MDY:
            sprintf((char*) date_str, "%02d%s%02d", month, sym_str, date);
            result = TRUE;
            break;
        case MMISET_DATE_DMY:
            sprintf((char*) date_str, "%02d%s%02d", date, sym_str, month);
            result = TRUE;
            break;
        default:
            break;
    }
    return result;
}

PUBLIC void MMIAPISET_GetCurrentDateMonDayStr(
                                        uint8* datestr,//in/out
                                        char symbol, //in
                                        uint16 datestr_len //in
                                        )
{
    SCI_DATE_T   sys_date = {0};
    TM_GetSysDate(&sys_date);
    MMIAPISET_FormatMonDayDateStrByDateStyle(sys_date.year,sys_date.mon,sys_date.mday,symbol,datestr,datestr_len);
}