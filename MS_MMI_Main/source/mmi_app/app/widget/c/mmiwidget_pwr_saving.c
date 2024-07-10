/*****************************************************************************
** File Name:      mmiwidget_prw_saving.c                                        *
** Author:                                                                   *
** Date:           10/23/2009                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**  Creat
******************************************************************************/
  
#define _MMIWIDGET_PWR_SAVING_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"
#ifdef MMI_SAM_THEME
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
#include "window_parse.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiwidget.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "guires.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmitheme_widget.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiset_func.h"
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#ifdef BLUETOOTH_SUPPORT
#include "mmibt_export.h"
#include "mmibt_func.h"   
#endif
//#include "mmiset_display.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
#include "mmiwidget_position.h"
#include "mmiset_text.h"
#include "guiwin.h"
#include "mmiidle_statusbar.h"
#include "mmiidle_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIWIDGET_PWR_SAVING_MAX_ITEM           8 

//item id 
typedef enum
{
#ifdef MMI_WIFI_SUPPORT     
     MMIWIDGET_SHORTCUT_PWRSAV_WIFI,
#else
     MMIWIDGET_SHORTCUT_PWRSAV_BATTERY,
#endif     
#ifdef BLUETOOTH_SUPPORT
     MMIWIDGET_SHORTCUT_PWRSAV_BT,
#endif     
     MMIWIDGET_SHORTCUT_PWRSAV_BGDATA,
     MMIWIDGET_SHORTCUT_PWRSAV_SS,
     MMIWIDGET_SHORTCUT_PWRSAV_BRIGHT,
     MMIWIDGET_SHORTCUT_PWRSAV_BACKLIGHT,
     MMIWIDGET_SHORTCUT_PWRSAV_ALERT,
     MMIWIDGET_SHORTCUT_PWRSAV_SET,
     MMIWIDGET_SHORTCUT_PWRSAV_MAX     
}WIDGETPB_PWR_SAVING_ITEM_E/*lint !e751*/;

//item info
typedef struct
{    
	WIDGETPB_PWR_SAVING_ITEM_E 	     item_id;
	MMI_IMAGE_ID_T   				 image_id;  
	MMI_IMAGE_ID_T					 bottom_image_id;
    MMI_TEXT_ID_T                    str_name;
  	GUI_RECT_T	     				 img_rect;	
  	GUI_RECT_T	     				 name_rect; 
  	GUI_RECT_T	     				 hl_rect;   	
   	BOOLEAN         				 is_open;
   	BOOLEAN         				 is_focus;
   	BOOLEAN         				 is_wait;
	void(*item_handle_func)(void);
	void(*get_state_func)(void);
}MMIWIDGET_PWR_SAVING_ITEM_T;

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
/*****************************************************************************/
//     Description : Handle Widget Power Saving Win Msg
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetPwrSavingWinMsg(
                                      MMI_WIN_ID_T       win_id,        //IN:
                                      MMI_MESSAGE_ID_E   msg_id,        //IN:
                                      DPARAM             param        //IN:
                                      );

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : Handle Widget Shortcut set redkeylock
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
//LOCAL void MMIWIDGET_Shortcutset_SetRedkeylock(void);

LOCAL void SetShortcutRect (MMI_WIN_ID_T win_id);

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL void InitPwrSaving(
                                    MMI_WIN_ID_T win_id
                                  );

/*****************************************************************************/
//     Description : Handle Widget Shortcut Set tp down Msg
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL MMI_RESULT_E  HandlePwrSavTpDown(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

/*****************************************************************************/
//     Description : Handle Widget Shortcut Set  tp up Msg
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL MMI_RESULT_E  HandlePwrSavTpUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

/*****************************************************************************/
//     Description : reset shortcut set state
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void ResetWidgetPwrSaving();

/*****************************************************************************/
//  Description : get short cut set widget image.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
LOCAL void GetPwrSavingWidgetImage(MMI_IMAGE_ID_T *bg_img, MMI_IMAGE_ID_T *sc_img);

/*****************************************************************************/
//     Description : Display power saving.
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void DisplayPwrSaving(MMI_WIN_ID_T  win_id);
/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/

PUBLIC const MMIWIDGET_ITEM_INFO_T g_pwr_saving_widget =
{
    MMIWIDGET_PWRSAVING_ID, 
    HandleWidgetPwrSavingWinMsg, 
    PNULL, 
    ResetWidgetPwrSaving,
    GetPwrSavingWidgetImage,    
    WIDGET_PWRSAVING_WIN_ID, 
    IMAGE_WIDGET_SHORTCUT_SET_SHORTCUT, 
    IMAGE_WIDGET_PWRSAV_BG,
    TXT_SET_POWER_SAVING_MODE,
    50,//    75, 
    50,//    75
    FALSE
};

//
LOCAL MMIWIDGET_PWR_SAVING_ITEM_T s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_MAX]   = {0};
LOCAL uint32                      s_pwr_saving_disp_arr[MMIWIDGET_PWR_SAVING_MAX_ITEM] = {0};
LOCAL MMISET_POWER_SETTING_ITEM_T s_pwr_saving_set = {0};

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get short cut set widget image.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
LOCAL void GetPwrSavingWidgetImage(MMI_IMAGE_ID_T *bg_img, MMI_IMAGE_ID_T *sc_img)
{
    if (PNULL != bg_img)
    {
        *bg_img = IMAGE_WIDGET_PWRSAV_BG;
    }

    if (PNULL != sc_img)    
    {
        *sc_img = IMAGE_WIDGET_SHORTCUT_SET_BG;
    }  
}

/*****************************************************************************/
//     Description : Reset shortcut set state
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void ResetWidgetPwrSaving()
{
    int i = 0;

    for(i = 0; i < MMIWIDGET_PWR_SAVING_MAX_ITEM; ++i)
    {
        s_pwr_saving_item_t[s_pwr_saving_disp_arr[i]].is_focus = FALSE;
    }
}

/*****************************************************************************/
//  Description : Handle Widget Shortcut Set Win Msg
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
LOCAL MMI_RESULT_E HandleWidgetPwrSavingWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
    int i = 0;

    	switch (msg_id)
    	{		
    	case MSG_OPEN_WINDOW:
			InitPwrSaving(win_id);
            SetShortcutRect(win_id);
			break;

		case MSG_WIDGET_TP_PRESS_DOWN:
			recode = HandlePwrSavTpDown(win_id,param);
			break;
			
		case MSG_WIDGET_TP_PRESS_UP:
			recode = HandlePwrSavTpUp(win_id,param);		  	 
			break;
				
    	case MSG_FULL_PAINT:
			for (i = 0; i < MMIWIDGET_PWR_SAVING_MAX_ITEM ; i++)
			{
                s_pwr_saving_item_t[s_pwr_saving_disp_arr[i]].get_state_func();
			}
			         
       		DisplayPwrSaving(win_id);
        	break;

    	case MSG_GET_FOCUS:
            SetShortcutRect(win_id);
            break;

        default:
        	recode = MMI_RESULT_FALSE;
        	break;
    }
    
    return recode;    		
}


#ifdef MMI_WIFI_SUPPORT
/*****************************************************************************/
//     Description : Handle Widget Shortcut open wifi
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void SetPwrSavWifi()
{
    s_pwr_saving_set.is_fb_wlan = !s_pwr_saving_set.is_fb_wlan;
    MMIAPISET_SetPowerSavingSetting(s_pwr_saving_set);
}
#else
/*****************************************************************************/
//     Description : Handle Widget Shortcut open wifi
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void SetPwrSavBattery()
{
#ifdef MMI_BATTERY_PERCENT_IN_STATUSBAR
    BOOLEAN is_disp_bat = FALSE;
    GUIWIN_STBDATA_TEXT_T  text_info = {0};

    text_info = MMIAPIIDLE_GetBatteryPercentStbTxt();
    is_disp_bat = MMIAPISET_GetBatteryPercentDisSwitch();

    is_disp_bat = !is_disp_bat;
    
    MMIAPISET_SetBatteryPercentDisSwitch(is_disp_bat);

    MMIAPISET_SetBatteryPercentDisSwitch(is_disp_bat);
    
    if(is_disp_bat)
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_BATTERY_PERCENT, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_BATTERY_PERCENT, FALSE);
    }
    
    GUIWIN_SetStbItemText(MMI_WIN_ICON_BATTERY_PERCENT,&text_info);
    GUIWIN_UpdateStb();
#endif    
}
#endif

#ifdef BLUETOOTH_SUPPORT
/*****************************************************************************/
//     Description : Handle Widget Shortcut open wifi
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void SetPwrSavBt()
{
    s_pwr_saving_set.is_fb_bluetooth = !s_pwr_saving_set.is_fb_bluetooth;
    MMIAPISET_SetPowerSavingSetting(s_pwr_saving_set);
}
#endif

/*****************************************************************************/
//     Description : Handle Widget Shortcut open wifi
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void SetPwrSavBgData()
{
    s_pwr_saving_set.is_fb_bg_data = !s_pwr_saving_set.is_fb_bg_data;
    MMIAPISET_SetPowerSavingSetting(s_pwr_saving_set);
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut open wifi
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void SetPwrSavSs()
{
    s_pwr_saving_set.is_fb_ss = !s_pwr_saving_set.is_fb_ss;
    MMIAPISET_SetPowerSavingSetting(s_pwr_saving_set);
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut open wifi
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void SetPwrSavBright()
{
    if (s_pwr_saving_set.lcd_contrast < MMISET_CONTRAST_THREE)
    {
        s_pwr_saving_set.lcd_contrast = MMISET_CONTRAST_FIVE;
    }
    else
    if (s_pwr_saving_set.lcd_contrast < MMISET_CONTRAST_SIX) 
    {
        s_pwr_saving_set.lcd_contrast = MMISET_CONTRAST_SEVEN;
    }
    else
    {
        s_pwr_saving_set.lcd_contrast = MMISET_CONTRAST_TWO;    
    }
    
    MMIAPISET_SetPowerSavingSetting(s_pwr_saving_set);
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut open wifi
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void SetPwrSavKeyLight()
{
    s_pwr_saving_set.is_fb_keypad_light = !s_pwr_saving_set.is_fb_keypad_light;
    MMIAPISET_SetPowerSavingSetting(s_pwr_saving_set);
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut open wifi
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void SetPwrSavAlert()
{
    s_pwr_saving_set.is_fb_alert_ring = !s_pwr_saving_set.is_fb_alert_ring;
    MMIAPISET_SetPowerSavingSetting(s_pwr_saving_set);
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut open wifi
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void SetPwrSavSet()
{
    MMIAPISET_OpenPowerSavingSetting();
}

//init wifi
#ifdef MMI_WIFI_SUPPORT
/*****************************************************************************/
//     Description : Handle Widget Shortcut set WIFI state
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void SetWifiState(void)
{
	if (!s_pwr_saving_set.is_fb_wlan) 
	{
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_WIFI].is_open = TRUE;
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_WIFI].image_id = IMAGE_WIDGET_PWRSAV_WIFI_ON;
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_WIFI].bottom_image_id = IMAGE_WIDGET_PWRSAV_ITEM_ON;		
	}
	else
	{
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_WIFI].is_open = FALSE;
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_WIFI].image_id = IMAGE_WIDGET_PWRSAV_WIFI_OFF;		
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_WIFI].bottom_image_id = IMAGE_WIDGET_PWRSAV_ITEM_OFF;
	}
}
#else
/*****************************************************************************/
//     Description : Handle Widget Shortcut set WIFI state
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void SetBatteryState(void)
{
#ifdef MMI_BATTERY_PERCENT_IN_STATUSBAR
	if (MMIAPISET_GetBatteryPercentDisSwitch()) 
	{
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BATTERY].is_open = TRUE;
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BATTERY].image_id = IMAGE_WIDGET_PWRSAV_BAT_ON;
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BATTERY].bottom_image_id = IMAGE_WIDGET_PWRSAV_ITEM_ON;		
	}
	else
#endif	
	{
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BATTERY].is_open = FALSE;
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BATTERY].image_id = IMAGE_WIDGET_PWRSAV_BAT_OFF;		
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BATTERY].bottom_image_id = IMAGE_WIDGET_PWRSAV_ITEM_OFF;
	}
}
#endif

//init bt
#ifdef BLUETOOTH_SUPPORT
/*****************************************************************************/
//     Description : Handle Widget Shortcut set Bt state
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
PUBLIC void SetBtState()
{	
	if(!s_pwr_saving_set.is_fb_bluetooth)
	{
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BT].is_open = TRUE;
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BT].image_id = IMAGE_WIDGET_PWRSAV_BT_ON;		
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BT].bottom_image_id = IMAGE_WIDGET_PWRSAV_ITEM_ON;
	}
	else
	{
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BT].is_open = FALSE;
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BT].image_id = IMAGE_WIDGET_PWRSAV_BT_OFF;
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BT].bottom_image_id = IMAGE_WIDGET_PWRSAV_ITEM_OFF;		
	}	
}
#endif

/*****************************************************************************/
//     Description : Handle Widget Shortcut set bg data
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
PUBLIC void SetBgDataState()
{	
	if(!s_pwr_saving_set.is_fb_bg_data)
	{
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BGDATA].is_open = TRUE;
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BGDATA].image_id = IMAGE_WIDGET_PWRSAV_BGDATA_ON;		
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BGDATA].bottom_image_id = IMAGE_WIDGET_PWRSAV_ITEM_ON;
	}
	else
	{
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BGDATA].is_open = FALSE;
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BGDATA].image_id = IMAGE_WIDGET_PWRSAV_BGDATA_OFF;
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BGDATA].bottom_image_id = IMAGE_WIDGET_PWRSAV_ITEM_OFF;		
	}	
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut set bg data
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
PUBLIC void SetScreenSaveState()
{	
	if(!s_pwr_saving_set.is_fb_ss)
	{
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_SS].is_open = TRUE;
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_SS].image_id = IMAGE_WIDGET_PWRSAV_SS_ON;		
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_SS].bottom_image_id = IMAGE_WIDGET_PWRSAV_ITEM_ON;
	}
	else
	{
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_SS].is_open = FALSE;
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_SS].image_id = IMAGE_WIDGET_PWRSAV_SS_OFF;
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_SS].bottom_image_id = IMAGE_WIDGET_PWRSAV_ITEM_OFF;		
	}	
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut set bg data
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
PUBLIC void SetBackLightState()
{	
	if(!s_pwr_saving_set.is_fb_keypad_light)
	{
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BACKLIGHT].is_open = TRUE;
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BACKLIGHT].image_id = IMAGE_WIDGET_PWRSAV_BL_ON;		
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BACKLIGHT].bottom_image_id = IMAGE_WIDGET_PWRSAV_ITEM_ON;
	}
	else
	{
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BACKLIGHT].is_open = FALSE;
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BACKLIGHT].image_id = IMAGE_WIDGET_PWRSAV_BL_OFF;
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BACKLIGHT].bottom_image_id = IMAGE_WIDGET_PWRSAV_ITEM_OFF;		
	}	
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut set bg data
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
PUBLIC void SetAlertState()
{	
	if(!s_pwr_saving_set.is_fb_alert_ring)
	{
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_ALERT].is_open = TRUE;
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_ALERT].image_id = IMAGE_WIDGET_PWRSAV_ALERT_ON;		
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_ALERT].bottom_image_id = IMAGE_WIDGET_PWRSAV_ITEM_ON;
	}
	else
	{
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_ALERT].is_open = FALSE;
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_ALERT].image_id = IMAGE_WIDGET_PWRSAV_ALERT_OFF;
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_ALERT].bottom_image_id = IMAGE_WIDGET_PWRSAV_ITEM_OFF;		
	}	
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut set bg data
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
PUBLIC void SetSetState()
{	
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_SET].is_open = TRUE;
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_SET].image_id = IMAGE_WIDGET_PWRSAV_SET_ON;		
}

/*****************************************************************************/
//  Description : Handle Widget Shortcut set bright state
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
PUBLIC void SetBrightState()
{
	uint32 bright = s_pwr_saving_set.lcd_contrast;
	
	if(bright < 3)
	{
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BRIGHT].image_id = IMAGE_WIDGET_PWRSAV_BRIGHT_1;
	}
	else if((3 <= bright ) && (bright< 6))
	{
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BRIGHT].image_id = IMAGE_WIDGET_PWRSAV_BRIGHT_2;
	}
	else
	{
		s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BRIGHT].image_id = IMAGE_WIDGET_PWRSAV_BRIGHT_3;
	}	
}

/*****************************************************************************/
//     Description : SetShortcutRect
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void SetShortcutRect (MMI_WIN_ID_T win_id)
{
    uint16 height = 0;
    uint16 width = 0;
    uint16 bg_height = 0;
    uint16 bg_width = 0;
    MMI_IMAGE_ID_T bg_img = 0;

    s_pwr_saving_set = MMIAPISET_GetPowerSavingSetting();
    
    GetPwrSavingWidgetImage(&bg_img, PNULL);
    GUIRES_GetImgWidthHeight(&width, &height, s_pwr_saving_item_t[0].image_id, win_id);
    GUIRES_GetImgWidthHeight(&bg_width, &bg_height, bg_img, win_id);
    
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[0]].img_rect.left   = MMIWIDGET_PWR_SAV_11_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[0]].img_rect.top   = MMIWIDGET_PWR_SAV_11_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[0]].img_rect.right  = MMIWIDGET_PWR_SAV_11_LEFT + width - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[0]].img_rect.bottom = MMIWIDGET_PWR_SAV_11_TOP + height - 1;
    
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[0]].name_rect.left   = MMIWIDGET_PWR_SAV_11_NAME_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[0]].name_rect.top   = MMIWIDGET_PWR_SAV_11_NAME_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[0]].name_rect.right  = MMIWIDGET_PWR_SAV_11_NAME_LEFT + bg_width/4 - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[0]].name_rect.bottom = MMIWIDGET_PWR_SAV_11_NAME_TOP + 14 - 1; 

    s_pwr_saving_item_t[s_pwr_saving_disp_arr[0]].hl_rect.left   = MMIWIDGET_PWR_SAV_11_HL_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[0]].hl_rect.top   = MMIWIDGET_PWR_SAV_11_HL_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[0]].hl_rect.right  = MMIWIDGET_PWR_SAV_11_HL_LEFT + bg_width/4 - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[0]].hl_rect.bottom = MMIWIDGET_PWR_SAV_11_HL_TOP + 14 - 1; 
//======================================================================================================
    
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[1]].img_rect.left   = MMIWIDGET_PWR_SAV_12_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[1]].img_rect.top   = MMIWIDGET_PWR_SAV_12_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[1]].img_rect.right  = MMIWIDGET_PWR_SAV_12_LEFT + width - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[1]].img_rect.bottom = MMIWIDGET_PWR_SAV_12_TOP + height - 1;     

    s_pwr_saving_item_t[s_pwr_saving_disp_arr[1]].name_rect.left   = MMIWIDGET_PWR_SAV_12_NAME_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[1]].name_rect.top   = MMIWIDGET_PWR_SAV_12_NAME_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[1]].name_rect.right  = MMIWIDGET_PWR_SAV_12_NAME_LEFT + bg_width/4 - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[1]].name_rect.bottom = MMIWIDGET_PWR_SAV_12_NAME_TOP + 14 - 1; 

    s_pwr_saving_item_t[s_pwr_saving_disp_arr[1]].hl_rect.left   = MMIWIDGET_PWR_SAV_12_HL_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[1]].hl_rect.top   = MMIWIDGET_PWR_SAV_12_HL_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[1]].hl_rect.right  = MMIWIDGET_PWR_SAV_12_HL_LEFT + bg_width/4 - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[1]].hl_rect.bottom = MMIWIDGET_PWR_SAV_12_HL_TOP + 14 - 1; 

//=======================================================================================================
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[2]].img_rect.left   = MMIWIDGET_PWR_SAV_13_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[2]].img_rect.top   = MMIWIDGET_PWR_SAV_13_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[2]].img_rect.right  = MMIWIDGET_PWR_SAV_13_LEFT + width - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[2]].img_rect.bottom = MMIWIDGET_PWR_SAV_13_TOP + height - 1;  

    s_pwr_saving_item_t[s_pwr_saving_disp_arr[2]].name_rect.left   = MMIWIDGET_PWR_SAV_13_NAME_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[2]].name_rect.top   = MMIWIDGET_PWR_SAV_13_NAME_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[2]].name_rect.right  = MMIWIDGET_PWR_SAV_13_NAME_LEFT + bg_width/4 - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[2]].name_rect.bottom = MMIWIDGET_PWR_SAV_13_NAME_TOP + 14 - 1;     

    s_pwr_saving_item_t[s_pwr_saving_disp_arr[2]].hl_rect.left   = MMIWIDGET_PWR_SAV_13_HL_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[2]].hl_rect.top   = MMIWIDGET_PWR_SAV_13_HL_TOP;    
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[2]].hl_rect.right  = MMIWIDGET_PWR_SAV_13_HL_LEFT + bg_width/4 - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[2]].hl_rect.bottom = MMIWIDGET_PWR_SAV_13_HL_TOP + 14 - 1; 
    
//========================================================================================================    
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[3]].img_rect.left   = MMIWIDGET_PWR_SAV_14_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[3]].img_rect.top   = MMIWIDGET_PWR_SAV_14_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[3]].img_rect.right  = MMIWIDGET_PWR_SAV_14_LEFT + width - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[3]].img_rect.bottom = MMIWIDGET_PWR_SAV_14_TOP + height - 1;  

    s_pwr_saving_item_t[s_pwr_saving_disp_arr[3]].name_rect.left   = MMIWIDGET_PWR_SAV_14_NAME_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[3]].name_rect.top   = MMIWIDGET_PWR_SAV_14_NAME_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[3]].name_rect.right  = MMIWIDGET_PWR_SAV_14_NAME_LEFT + bg_width/4 - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[3]].name_rect.bottom = MMIWIDGET_PWR_SAV_14_NAME_TOP + 14 - 1; 

    s_pwr_saving_item_t[s_pwr_saving_disp_arr[3]].hl_rect.left   = MMIWIDGET_PWR_SAV_14_HL_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[3]].hl_rect.top   = MMIWIDGET_PWR_SAV_14_HL_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[3]].hl_rect.right  = MMIWIDGET_PWR_SAV_14_HL_LEFT + bg_width/4 - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[3]].hl_rect.bottom = MMIWIDGET_PWR_SAV_14_HL_TOP + 14 - 1; 

//=========================================================================================================
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[4]].img_rect.left   = MMIWIDGET_PWR_SAV_21_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[4]].img_rect.top   = MMIWIDGET_PWR_SAV_21_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[4]].img_rect.right  = MMIWIDGET_PWR_SAV_21_LEFT + width - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[4]].img_rect.bottom = MMIWIDGET_PWR_SAV_21_TOP + height - 1;    

    s_pwr_saving_item_t[s_pwr_saving_disp_arr[4]].name_rect.left   = MMIWIDGET_PWR_SAV_21_NAME_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[4]].name_rect.top   = MMIWIDGET_PWR_SAV_21_NAME_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[4]].name_rect.right  = MMIWIDGET_PWR_SAV_21_NAME_LEFT + bg_width/4 - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[4]].name_rect.bottom = MMIWIDGET_PWR_SAV_21_NAME_TOP + 14 - 1; 

    s_pwr_saving_item_t[s_pwr_saving_disp_arr[4]].hl_rect.left   = MMIWIDGET_PWR_SAV_21_HL_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[4]].hl_rect.top   = MMIWIDGET_PWR_SAV_21_HL_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[4]].hl_rect.right  = MMIWIDGET_PWR_SAV_21_HL_LEFT + bg_width/4 - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[4]].hl_rect.bottom = MMIWIDGET_PWR_SAV_21_HL_TOP + 14 - 1; 
//==========================================================================================================   

    s_pwr_saving_item_t[s_pwr_saving_disp_arr[5]].img_rect.left   = MMIWIDGET_PWR_SAV_22_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[5]].img_rect.top   = MMIWIDGET_PWR_SAV_22_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[5]].img_rect.right  = MMIWIDGET_PWR_SAV_22_LEFT + width - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[5]].img_rect.bottom = MMIWIDGET_PWR_SAV_22_TOP + height - 1;  

    s_pwr_saving_item_t[s_pwr_saving_disp_arr[5]].name_rect.left   = MMIWIDGET_PWR_SAV_22_NAME_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[5]].name_rect.top   = MMIWIDGET_PWR_SAV_22_NAME_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[5]].name_rect.right  = MMIWIDGET_PWR_SAV_22_NAME_LEFT + bg_width/4 - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[5]].name_rect.bottom = MMIWIDGET_PWR_SAV_22_NAME_TOP + 14 - 1;   

    s_pwr_saving_item_t[s_pwr_saving_disp_arr[5]].hl_rect.left   = MMIWIDGET_PWR_SAV_22_HL_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[5]].hl_rect.top   = MMIWIDGET_PWR_SAV_22_HL_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[5]].hl_rect.right  = MMIWIDGET_PWR_SAV_22_HL_LEFT + bg_width/4 - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[5]].hl_rect.bottom = MMIWIDGET_PWR_SAV_22_HL_TOP + 14 - 1;     
//==========================================================================================================
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[6]].img_rect.left   = MMIWIDGET_PWR_SAV_23_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[6]].img_rect.top   = MMIWIDGET_PWR_SAV_23_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[6]].img_rect.right  = MMIWIDGET_PWR_SAV_23_LEFT + width - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[6]].img_rect.bottom = MMIWIDGET_PWR_SAV_23_TOP + height - 1;  
    
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[6]].name_rect.left   = MMIWIDGET_PWR_SAV_23_NAME_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[6]].name_rect.top   = MMIWIDGET_PWR_SAV_23_NAME_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[6]].name_rect.right  = MMIWIDGET_PWR_SAV_23_NAME_LEFT + bg_width/4 - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[6]].name_rect.bottom = MMIWIDGET_PWR_SAV_23_NAME_TOP + 14 - 1;  

    s_pwr_saving_item_t[s_pwr_saving_disp_arr[6]].hl_rect.left   = MMIWIDGET_PWR_SAV_23_HL_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[6]].hl_rect.top   = MMIWIDGET_PWR_SAV_23_HL_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[6]].hl_rect.right  = MMIWIDGET_PWR_SAV_23_HL_LEFT + bg_width/4 - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[6]].hl_rect.bottom = MMIWIDGET_PWR_SAV_23_HL_TOP + 14 - 1;    
//==========================================================================================================

    s_pwr_saving_item_t[s_pwr_saving_disp_arr[7]].img_rect.left   = MMIWIDGET_PWR_SAV_24_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[7]].img_rect.top   = MMIWIDGET_PWR_SAV_24_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[7]].img_rect.right  = MMIWIDGET_PWR_SAV_24_LEFT + width - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[7]].img_rect.bottom = MMIWIDGET_PWR_SAV_24_TOP + height - 1;    

    s_pwr_saving_item_t[s_pwr_saving_disp_arr[7]].name_rect.left   = MMIWIDGET_PWR_SAV_24_NAME_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[7]].name_rect.top   = MMIWIDGET_PWR_SAV_24_NAME_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[7]].name_rect.right  = MMIWIDGET_PWR_SAV_24_NAME_LEFT + bg_width/4 - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[7]].name_rect.bottom = MMIWIDGET_PWR_SAV_24_NAME_TOP + 14 - 1;    

    s_pwr_saving_item_t[s_pwr_saving_disp_arr[7]].hl_rect.left   = MMIWIDGET_PWR_SAV_24_HL_LEFT;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[7]].hl_rect.top   = MMIWIDGET_PWR_SAV_24_HL_TOP;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[7]].hl_rect.right  = MMIWIDGET_PWR_SAV_24_HL_LEFT + bg_width/4 - 1;
    s_pwr_saving_item_t[s_pwr_saving_disp_arr[7]].hl_rect.bottom = MMIWIDGET_PWR_SAV_24_HL_TOP + 14 - 1;        
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut Set Win Msg
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void InitPwrSaving (MMI_WIN_ID_T win_id)
{  
#ifdef MMI_WIFI_SUPPORT
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_WIFI].image_id = IMAGE_WIDGET_PWRSAV_WIFI_ON;
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_WIFI].item_handle_func = SetPwrSavWifi;
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_WIFI].get_state_func = SetWifiState;
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_WIFI].str_name = TXT_WIDGET_PWRSAV_WIFI;
	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_WIFI].is_wait = FALSE;
	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_WIFI].is_focus = FALSE;
#else
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BATTERY].image_id = IMAGE_WIDGET_PWRSAV_BAT_ON;
	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BATTERY].item_handle_func = SetPwrSavBattery;
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BATTERY].get_state_func = SetBatteryState;
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BATTERY].str_name = TXT_WIDGET_PWRSAV_BATTERY;    
	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BATTERY].is_wait = FALSE;
	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BATTERY].is_focus = FALSE;       
#endif

	//init bt
#ifdef BLUETOOTH_SUPPORT
	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BT].item_handle_func = SetPwrSavBt;
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BT].get_state_func = SetBtState;
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BT].str_name = TXT_WIDGET_PWRSAV_BT;    
	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BT].is_wait = FALSE;
	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BT].is_focus = FALSE;
#endif

	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BGDATA].item_handle_func = SetPwrSavBgData;
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BGDATA].get_state_func = SetBgDataState;
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BGDATA].str_name = TXT_WIDGET_PWRSAV_BGDATA;    
	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BGDATA].is_wait = FALSE;
	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BGDATA].is_focus = FALSE;

	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_SS].item_handle_func = SetPwrSavSs;
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_SS].get_state_func = SetScreenSaveState;
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_SS].str_name = TXT_WIDGET_PWRSAV_SS;    
	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_SS].is_wait = FALSE;
	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_SS].is_focus = FALSE;    	

	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BRIGHT].item_handle_func = SetPwrSavBright;
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BRIGHT].get_state_func = SetBrightState;
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BRIGHT].str_name = TXT_WIDGET_PWRSAV_BRIGHT;    
	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BRIGHT].is_wait = FALSE;
	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BRIGHT].is_focus = FALSE; 

	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BACKLIGHT].item_handle_func = SetPwrSavKeyLight;
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BACKLIGHT].get_state_func = SetBackLightState;
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BACKLIGHT].str_name = TXT_WIDGET_PWRSAV_KEYLIGHT;    
	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BACKLIGHT].is_wait = FALSE;
	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_BACKLIGHT].is_focus = FALSE; 

	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_ALERT].item_handle_func = SetPwrSavAlert;
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_ALERT].get_state_func = SetAlertState;
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_ALERT].str_name = TXT_WIDGET_PWRSAV_ALERT_SETTING;    
	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_ALERT].is_wait = FALSE;
	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_ALERT].is_focus = FALSE; 

	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_SET].item_handle_func = SetPwrSavSet;
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_SET].get_state_func = SetSetState;
    s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_SET].str_name = STXT_MAIN_SETTING;    
	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_SET].is_wait = FALSE;
	s_pwr_saving_item_t[MMIWIDGET_SHORTCUT_PWRSAV_SET].is_focus = FALSE; 

    // init s_shortcut_set_disp_arr
#ifdef MMI_WIFI_SUPPORT
    s_pwr_saving_disp_arr[MMIWIDGET_SHORTCUT_PWRSAV_WIFI]      = MMIWIDGET_SHORTCUT_PWRSAV_WIFI;
#else
    s_pwr_saving_disp_arr[MMIWIDGET_SHORTCUT_PWRSAV_BATTERY]   = MMIWIDGET_SHORTCUT_PWRSAV_BATTERY;
#endif

#ifdef BLUETOOTH_SUPPORT
    s_pwr_saving_disp_arr[MMIWIDGET_SHORTCUT_PWRSAV_BT]        = MMIWIDGET_SHORTCUT_PWRSAV_BT;
#endif        
    s_pwr_saving_disp_arr[MMIWIDGET_SHORTCUT_PWRSAV_BGDATA]    = MMIWIDGET_SHORTCUT_PWRSAV_BGDATA;
    s_pwr_saving_disp_arr[MMIWIDGET_SHORTCUT_PWRSAV_SS]        = MMIWIDGET_SHORTCUT_PWRSAV_SS;    
    s_pwr_saving_disp_arr[MMIWIDGET_SHORTCUT_PWRSAV_BRIGHT]    = MMIWIDGET_SHORTCUT_PWRSAV_BRIGHT;
    s_pwr_saving_disp_arr[MMIWIDGET_SHORTCUT_PWRSAV_BACKLIGHT] = MMIWIDGET_SHORTCUT_PWRSAV_BACKLIGHT;    
    s_pwr_saving_disp_arr[MMIWIDGET_SHORTCUT_PWRSAV_ALERT]     = MMIWIDGET_SHORTCUT_PWRSAV_ALERT;
    s_pwr_saving_disp_arr[MMIWIDGET_SHORTCUT_PWRSAV_SET]       = MMIWIDGET_SHORTCUT_PWRSAV_SET;             
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut Set Win Msg
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void DisplayPwrSaving(MMI_WIN_ID_T  win_id)
{
    GUI_RECT_T win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T item_img_rect = {0};
    GUI_RECT_T rect = {0};
    GUI_POINT_T bottom_img_point = {0};
    uint32 i = 0;
    uint32 item_num = MMIWIDGET_PWR_SAVING_MAX_ITEM;
    GUISTR_STYLE_T    str_style = {0};
    GUISTR_STATE_T    text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_EFFECT;
    MMI_IMAGE_ID_T    show_image_id = 0;  
    MMI_TEXT_ID_T     str_name = 0; 
    MMI_STRING_T      name_str = {0};
    uint16  img_height = 0;
    uint16  img_width = 0;
    
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    if (!UILAYER_IsLayerActive(&lcd_dev_info))
    {
        return;
    }
    
    GUIRES_DisplayImg(
                      PNULL,
                      &win_rect,
                      PNULL,
                      win_id,
                      IMAGE_WIDGET_PWRSAV_BG,
                      &lcd_dev_info
                      );
            
    for(i = 0 ; i < item_num ;i++)
    {                    
        //show item image 		
        show_image_id= s_pwr_saving_item_t[s_pwr_saving_disp_arr[i]].image_id;
        
        GUIRES_GetImgWidthHeight(&img_width, &img_height, show_image_id, win_id);

        rect = s_pwr_saving_item_t[s_pwr_saving_disp_arr[i]].img_rect;
        
        item_img_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, rect);
        GUIRES_DisplayImg(
            PNULL,
            &item_img_rect,
            PNULL,
            win_id,
            show_image_id,
            &lcd_dev_info
            );

        str_style.font       = MMI_DEFAULT_SMALL_FONT;
        str_style.font_color = MMI_WHITE_COLOR;
        str_style.align      = ALIGN_HMIDDLE;
        rect = s_pwr_saving_item_t[s_pwr_saving_disp_arr[i]].name_rect;
        str_name = s_pwr_saving_item_t[s_pwr_saving_disp_arr[i]].str_name;

        rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, rect);

        MMI_GetLabelTextByLang(str_name,&name_str);
        
        GUISTR_DrawTextToLCDInRect( 
                            	   (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                            	   &rect,
                            	   &rect,
                                   (const MMI_STRING_T     *)&name_str,
                                   &str_style,
                                   text_state,
                                   GUISTR_TEXT_DIR_AUTO);


        //show item image 		
        show_image_id= s_pwr_saving_item_t[s_pwr_saving_disp_arr[i]].bottom_image_id;

        if (0 == show_image_id)
        {
            continue;
        }
        
        GUIRES_GetImgWidthHeight(&img_width, &img_height, show_image_id, win_id);

        bottom_img_point.x = s_pwr_saving_item_t[s_pwr_saving_disp_arr[i]].hl_rect.left;
        bottom_img_point.y = s_pwr_saving_item_t[s_pwr_saving_disp_arr[i]].hl_rect.top;
        
        bottom_img_point = MMI_ConvertWinPoint(MMICOM_WINPOS_WIN2DISP, win_id, bottom_img_point);
        
        GUIRES_DisplayImg(
            &bottom_img_point,
            PNULL,
            PNULL,
            win_id,
            show_image_id,
            &lcd_dev_info
            );		
    }
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut Set tp down Msg
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL MMI_RESULT_E  HandlePwrSavTpDown(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_FALSE;
   	GUI_POINT_T point = {0};
    uint32 i = 0;
    uint32 item_num = 0;
    GUI_RECT_T rect = {0};
    uint16 bg_width = 0;
    uint16 bg_height = 0;  
    MMI_IMAGE_ID_T bg_img = 0;
    
    GetPwrSavingWidgetImage(&bg_img, PNULL);
    GUIRES_GetImgWidthHeight(&bg_width, &bg_height, bg_img, win_id);    
    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);
    
    item_num = 	MMIWIDGET_PWR_SAVING_MAX_ITEM;

    for(i = 0; i< item_num; i++)
    {
        rect.left   = i%(MMIWIDGET_PWR_SAVING_MAX_ITEM/2)*bg_width/4;
        rect.right  = rect.left + bg_width/4 - 1;
        rect.top    = i/(MMIWIDGET_PWR_SAVING_MAX_ITEM/2)*bg_height/2;
        rect.bottom = rect.top + bg_height/2 - 1;
        
        if(GUI_PointIsInRect(point, rect))
        {
            s_pwr_saving_item_t[s_pwr_saving_disp_arr[i]].is_focus = TRUE;
        }
    }
    
    return recode;
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut Set  tp up Msg
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL MMI_RESULT_E  HandlePwrSavTpUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
   	GUI_POINT_T point = {0};
    // GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
    uint32 i = 0;
    uint32 item_num = 0;
    GUI_RECT_T rect = {0};
    uint16 bg_width = 0;
    uint16 bg_height = 0;
    MMI_IMAGE_ID_T bg_img = 0;
    
    GetPwrSavingWidgetImage(&bg_img, PNULL);
    GUIRES_GetImgWidthHeight(&bg_width, &bg_height, bg_img, win_id);      

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);
    
    item_num = 	MMIWIDGET_PWR_SAVING_MAX_ITEM;
    
    for(i = 0; i< item_num; i++)
    {
        s_pwr_saving_item_t[s_pwr_saving_disp_arr[i]].is_focus = FALSE;

        rect.left   = i%(MMIWIDGET_PWR_SAVING_MAX_ITEM/2)*bg_width/4;
        rect.right  = rect.left + bg_width/4 - 1;
        rect.top    = i/(MMIWIDGET_PWR_SAVING_MAX_ITEM/2)*bg_height/2;
        rect.bottom = rect.top + bg_height/2 - 1;
        
        if(GUI_PointIsInRect(point, rect))
        {
            if (PNULL != s_pwr_saving_item_t[s_pwr_saving_disp_arr[i]].item_handle_func)
            {
                s_pwr_saving_item_t[s_pwr_saving_disp_arr[i]].item_handle_func();
            }
            
            UILAYER_SetDirectDraw( FALSE );            
        }
    }
    
    return recode;
    
}
#endif
#endif
