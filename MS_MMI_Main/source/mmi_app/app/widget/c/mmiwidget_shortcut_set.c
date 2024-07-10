/*****************************************************************************
** File Name:      mmiwidget_shortcut_set.c                                        *
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
  
#define _MMIWIDGET_SHORTCUT_SET_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"

#ifdef MMI_WIDGET_SHORTCUT_SET
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
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MAX_MMIWIDGET_SHORTCUT_SET_ITEM              4 

//item id 
 typedef enum
 {
#ifdef MMI_WIFI_SUPPORT
     MMIWIDGET_SHORTCUT_SET_WIFI,
#endif
         
#ifdef BLUETOOTH_SUPPORT
     MMIWIDGET_SHORTCUT_SET_BT,
#endif
         
     MMIWIDGET_SHORTCUT_SET_BACKLIGHT,
     MMIWIDGET_SHORTCUT_SET_SILENT,  
     
     MMIWIDGET_SHORTCUT_SET_HIGH_PRIO_BOUND, 

     MMIWIDGET_SHORTCUT_SET_FLYMODE = MMIWIDGET_SHORTCUT_SET_HIGH_PRIO_BOUND,
     MMIWIDGET_SHORTCUT_SET_AUTO_LOCK_SCREEN,
     MMIWIDGET_SHORTCUT_SET_REDKEYLOCK,
         
     MMIWIDGET_SHORTCUT_SET_ITEM_MAX
 }WIDGETPB_SHORTCUT_SET_ITEM_E/*lint !e751*/;


//item info
typedef struct
{    
	WIDGETPB_SHORTCUT_SET_ITEM_E 	item_id;
	MMI_IMAGE_ID_T   				image_id;  
	MMI_IMAGE_ID_T					bottom_image_id;
    	GUI_RECT_T	     					rect;	
    	BOOLEAN         					is_open;
    	BOOLEAN         					is_focus;
    	BOOLEAN         					is_wait;
	void(*item_handle_func)(void);
	void(*get_state_func)(void);
}MMIWIDGET_SHORTCUT_SET_ITEM_T;

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
//     Description : Handle Widget Shortcut Set Win Msg
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetShortcutSetWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef BLUETOOTH_SUPPORT
/*****************************************************************************/
//     Description : Handle Widget Shortcut set Bt state
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void MMIWIDGET_Shortcutset_SetBtState(void);
#endif

/*****************************************************************************/
//     Description : Handle Widget Shortcut set backlight state
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void MMIWIDGET_Shortcutset_SetBackLightState(void);
/*****************************************************************************/
//     Description : Handle Widget Shortcut set Silent state
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void MMIWIDGET_Shortcutset_SetSilentState(void);

/*****************************************************************************/
//     Description : Handle Widget Shortcut set flymode
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void MMIWIDGET_Shortcutset_SetFlymode(void);
LOCAL void MMIWIDGET_Shortcutset_SetAutoKeylock(void);
/*****************************************************************************/
//     Description : Handle Widget Shortcut set redkeylock
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void MMIWIDGET_Shortcutset_SetRedkeylock(void);

LOCAL void SetShortcutRect (MMI_WIN_ID_T win_id);

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//    Author:
//    Note: 
/*****************************************************************************/
LOCAL void MMIWIDGET_InitShortcutSet(
                                    MMI_WIN_ID_T win_id
                                );

/*****************************************************************************/
//  Description : display Shortcut Set
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayShortcutSet(
                       MMI_WIN_ID_T win_id
                       );

/*****************************************************************************/
//     Description : Handle Widget Shortcut Set tp down Msg
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL MMI_RESULT_E  MMIWIDGET_ShortcutSet_HandleTpPressDown(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

/*****************************************************************************/
//     Description : Handle Widget Shortcut Set  tp up Msg
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL MMI_RESULT_E  MMIWIDGET_ShortcutSet_HandleTpPressUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

#ifdef MMI_WIFI_SUPPORT
/*****************************************************************************/
//     Description : Handle Widget Shortcut open wifi
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void MMIWIDGET_Shortcutset_OpenWifi(void);
#endif

#ifdef BLUETOOTH_SUPPORT
/*****************************************************************************/
//     Description : Handle Widget Shortcut open Bt
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void MMIWIDGET_Shortcutset_OpenBt(void);
#endif

/*****************************************************************************/
//     Description : Handle Widget Shortcut set  backlight
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void MMIWIDGET_Shortcutset_SetBacklight(void);

/*****************************************************************************/
//     Description : Handle Widget Shortcut switch flymode
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void MMIWIDGET_Shortcutset_SwitchFlymode(void);

LOCAL void MMIWIDGET_Shortcutset_OpenAutokeylock(void);

/*****************************************************************************/
//     Description : Handle Widget Shortcut open redkeylock
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void MMIWIDGET_Shortcutset_OpenRedkeylock(void);

/*****************************************************************************/
//     Description : Handle Widget Shortcut open Silent
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void MMIWIDGET_Shortcutset_OpenSilent(void);

/*****************************************************************************/
//     Description : reset shortcut set state
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void ResetWidgetShortcutSet();

#ifdef MMI_WIFI_SUPPORT
/*****************************************************************************/
//     Description : Callback function to power on wifi.
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void PowerOnWifiCallback(MMIWIFI_NOTIFY_EVENT_E event_id,MMIWIFI_NOTIFY_PARAM_T* param_ptr);
#endif

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/

PUBLIC const MMIWIDGET_ITEM_INFO_T g_shortcut_set_widget =
{
    MMIWIDGET_SHORTCUT_SET_ID, 
    HandleWidgetShortcutSetWinMsg, 
    PNULL, 
    ResetWidgetShortcutSet,
    PNULL,    
    WIDGET_SHORTCUT_SET_WIN_ID, 
    IMAGE_WIDGET_SHORTCUT_SET_SHORTCUT, 
    IMAGE_WIDGET_SHORTCUT_SET_BG,
    TXT_WIDGET_SHORTCUT_SET,
    50,//    75, 
    50,//    75
    FALSE
};

//
LOCAL MMIWIDGET_SHORTCUT_SET_ITEM_T s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_ITEM_MAX] = {0};
LOCAL uint32    s_shortcut_set_disp_arr[MAX_MMIWIDGET_SHORTCUT_SET_ITEM] = {0};   

//ǰһλģʽID
//LOCAL uint8               	s_noisy_pre_mode = MMIENVSET_STANDARD_MODE;
//LOCAL uint8                 s_silent_pre_mode = MMIENVSET_MEETING_MODE;

/*****************************************************************************/
//     Description : Reset shortcut set state
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void ResetWidgetShortcutSet()
{
    int i = 0;
    
    for(i = 0; i < MAX_MMIWIDGET_SHORTCUT_SET_ITEM; ++i)
    {
        s_shortcut_set_item_t[s_shortcut_set_disp_arr[i]].is_focus = FALSE;
    }

    //MMK_SendMsg(WIDGET_SHORTCUT_SET_WIN_ID, MSG_FULL_PAINT, 0);
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut Set Win Msg
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL MMI_RESULT_E HandleWidgetShortcutSetWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
    int i = 0;
       MMI_CheckAllocatedMemInfo();
    	switch (msg_id)
    	{
		
    	case MSG_OPEN_WINDOW:
			MMIWIDGET_InitShortcutSet(win_id);
            SetShortcutRect(win_id);
			break;

		case MSG_WIDGET_TP_PRESS_DOWN:
			recode = MMIWIDGET_ShortcutSet_HandleTpPressDown(win_id,param);
			break;
			
		case MSG_WIDGET_TP_PRESS_UP:
			recode = MMIWIDGET_ShortcutSet_HandleTpPressUp(win_id,param);		  	 
			break;
				
    	case MSG_FULL_PAINT:
			for (i = 0; i < MAX_MMIWIDGET_SHORTCUT_SET_ITEM ; i++)
			{
                s_shortcut_set_item_t[s_shortcut_set_disp_arr[i]].get_state_func();
			}
			         
       		DisplayShortcutSet(win_id);
        	break;

    	case MSG_GET_FOCUS:
            SetShortcutRect(win_id);
            break;

    		default:
        	recode = MMI_RESULT_FALSE;
        	break;
    }
    MMI_CheckAllocatedMemInfo();
    return recode;    	
	
}

//init wifi
#ifdef MMI_WIFI_SUPPORT
/*****************************************************************************/
//     Description : Handle Widget Shortcut set WIFI state
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
PUBLIC void MMIAPIWIDGET_Shortcutset_SetWifiState(void)
{
	if (MMIWIFI_STATUS_OFF != MMIAPIWIFI_GetStatus()) 
	{
		s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_WIFI].is_open = TRUE;
		s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_WIFI].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_ON;
		s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_WIFI].is_wait = FALSE;
	}
	else
	{
		s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_WIFI].is_open = FALSE;
		s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_WIFI].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_OFF;
	}
	DisplayShortcutSet(WIDGET_SHORTCUT_SET_WIN_ID);
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut reset WIFI state
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
PUBLIC void MMIAPIWIDGET_Shortcutset_ResetWifiState()
{
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
PUBLIC void MMIWIDGET_Shortcutset_SetBtState()
{	
	if(MMIAPIBT_IsBtOn())
	{
		s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BT].is_open = TRUE;
		s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BT].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_ON;
	}
	else
	{
		s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BT].is_open = FALSE;
		s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BT].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_OFF;
	}
	
}
#endif

//init backlight	
/*****************************************************************************/
//     Description : Handle Widget Shortcut set backlight state
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
PUBLIC void MMIWIDGET_Shortcutset_SetBackLightState()
{
	uint32 backlight = 0;
	backlight = MMIAPISET_GetCurrentContrast();
	if(backlight < 3)
	{
		s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BACKLIGHT].image_id = IMAGE_WIDGET_SHORTCUT_SET_BACKLIGHT_1;
	}
	else if((3 <= backlight ) && (backlight< 6))
	{
		s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BACKLIGHT].image_id = IMAGE_WIDGET_SHORTCUT_SET_BACKLIGHT_2;
	}
	else
	{
		s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BACKLIGHT].image_id = IMAGE_WIDGET_SHORTCUT_SET_BACKLIGHT_3;
	}
	
}

//init silent
/*****************************************************************************/
//     Description : Handle Widget Shortcut set Silent state
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
PUBLIC void MMIWIDGET_Shortcutset_SetSilentState()
{
    
    uint8           envset_mode     = 0;
    
    envset_mode = MMIAPIENVSET_GetActiveModeId();
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
    if (envset_mode == MMIENVSET_EARPHONE_MODE)
    {
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_SILENT].is_open = TRUE;
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_SILENT].image_id = IMAGE_WIDGET_SHORTCUT_SET_EARPHONE;
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_SILENT].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_ON;
        return;
    }
    else if (envset_mode == MMIENVSET_BLUETOOTH_MODE)
    {
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_SILENT].is_open = TRUE;
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_SILENT].image_id = IMAGE_WIDGET_SHORTCUT_SET_BT_EARPHONE;
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_SILENT].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_ON;
        return;
    }
    else 
#endif 	
    if((envset_mode== MMIENVSET_MEETING_MODE) || (envset_mode== MMIENVSET_SILENT_MODE))
    {
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_SILENT].is_open = TRUE;
        //s_silent_pre_mode = envset_mode;
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_SILENT].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_OFF;
    }
    else
    {
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_SILENT].is_open = FALSE;
        //s_noisy_pre_mode = envset_mode;
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_SILENT].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_ON;
    }
    s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_SILENT].image_id = IMAGE_WIDGET_SHORTCUT_SET_VOL;
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut set flymode
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
PUBLIC void MMIWIDGET_Shortcutset_SetFlymode()
{
    if(MMIAPISET_GetFlyMode())
    {
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_FLYMODE].is_open = TRUE;
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_FLYMODE].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_ON;
    }
    else
    {
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_FLYMODE].is_open = FALSE;
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_FLYMODE].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_OFF;
    }
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut set redkeylock
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void MMIWIDGET_Shortcutset_SetAutoKeylock()
{
    if(MMISET_KEYLOCK_CLOSE != MMIAPISET_GetAutoBoardKey())
    {
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_AUTO_LOCK_SCREEN].is_open = TRUE;
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_AUTO_LOCK_SCREEN].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_ON;
    }
    else
    {
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_AUTO_LOCK_SCREEN].is_open = FALSE;
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_AUTO_LOCK_SCREEN].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_OFF;
    }
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut set redkeylock
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void MMIWIDGET_Shortcutset_SetRedkeylock()
{
    if(MMIAPISET_GetRedKeyLock())
    {
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_REDKEYLOCK].is_open = TRUE;
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_REDKEYLOCK].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_ON;
    }
    else
    {
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_REDKEYLOCK].is_open = FALSE;
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_REDKEYLOCK].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_OFF;
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
	GUI_RECT_T rect[4] = {0};
    uint16 bg_height = 0;
    uint16 bg_width = 0;
	GUI_RECT_T bg_rect = {0};


    GUIRES_GetImgWidthHeight(&bg_width, &bg_height, IMAGE_WIDGET_SHORTCUT_SET_BG, win_id);
    bg_rect.right = bg_width -1 ;
    bg_rect.bottom = bg_height - 1 ;
    #if (defined MAINLCD_SIZE_240X320||defined MAINLCD_SIZE_240X400)
    bg_rect.left = 8 ;//NEWMS00196750 according to pic ,cut the two side white space 
    bg_rect.right = bg_rect.right -8 ;
    #endif
    #if (defined MAINLCD_SIZE_320X480)
    bg_rect.left = 6 ;//NEWMS00196750 according to pic ,cut the two side white space 
    bg_rect.right = bg_rect.right -6 ;
    #endif
    GUI_CreateMatrixRect(&bg_rect, 0, 0, 0, 0,  1, 4, rect);
    // init rect bottom_image_id
    s_shortcut_set_item_t[s_shortcut_set_disp_arr[0]].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_ON;
    s_shortcut_set_item_t[s_shortcut_set_disp_arr[0]].rect = rect[0];
    
    s_shortcut_set_item_t[s_shortcut_set_disp_arr[1]].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_ON;
    s_shortcut_set_item_t[s_shortcut_set_disp_arr[1]].rect = rect[1]; 
    
    s_shortcut_set_item_t[s_shortcut_set_disp_arr[2]].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_ON;
    s_shortcut_set_item_t[s_shortcut_set_disp_arr[2]].rect = rect[2];
    
    s_shortcut_set_item_t[s_shortcut_set_disp_arr[3]].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_ON;
    s_shortcut_set_item_t[s_shortcut_set_disp_arr[3]].rect = rect[3];
}
/*****************************************************************************/
//     Description : Handle Widget Shortcut Set Win Msg
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void MMIWIDGET_InitShortcutSet (MMI_WIN_ID_T win_id)
{
	//init wifi
	#ifdef MMI_WIFI_SUPPORT
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_WIFI].image_id = IMAGE_WIDGET_SHORTCUT_SET_WIFI;
	//s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_WIFI].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_ON_L;
	//s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_WIFI].rect = wifi_rect;	
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_WIFI].item_handle_func = MMIWIDGET_Shortcutset_OpenWifi;
    s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_WIFI].get_state_func = MMIAPIWIDGET_Shortcutset_SetWifiState;
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_WIFI].is_wait = FALSE;
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_WIFI].is_focus = FALSE;
	#endif

	//init bt
	#ifdef BLUETOOTH_SUPPORT
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BT].image_id = IMAGE_WIDGET_SHORTCUT_SET_BLUETOOTH;
	//s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BT].rect = bt_rect;
	//s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BT].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_ON_M;
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BT].item_handle_func = MMIWIDGET_Shortcutset_OpenBt;
    s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BT].get_state_func = MMIWIDGET_Shortcutset_SetBtState;
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BT].is_wait = FALSE;
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BT].is_focus = FALSE;
	#endif

	//init backlight
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BACKLIGHT].image_id = IMAGE_WIDGET_SHORTCUT_SET_BACKLIGHT_1;
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BACKLIGHT].is_open= TRUE;
	//s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BACKLIGHT].rect = backlight_rect;	
	//s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BACKLIGHT].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_ON_M;
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BACKLIGHT].is_focus = FALSE;
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BACKLIGHT].item_handle_func = MMIWIDGET_Shortcutset_SetBacklight;
    s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BACKLIGHT].get_state_func = MMIWIDGET_Shortcutset_SetBackLightState;
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BACKLIGHT].is_wait = FALSE;

	//init silent
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_SILENT].image_id = IMAGE_WIDGET_SHORTCUT_SET_VOL;
	//s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_SILENT].rect = silent_rect;	
	//s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_SILENT].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_ON_R;
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_SILENT].is_focus = FALSE;
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_SILENT].item_handle_func = MMIWIDGET_Shortcutset_OpenSilent;
    s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_SILENT].get_state_func = MMIWIDGET_Shortcutset_SetSilentState;
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_SILENT].is_wait = FALSE;
	
    // init flymode
    s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_FLYMODE].image_id = IMAGE_WIDGET_SHORTCUT_SET_FLYMODE;
    s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_FLYMODE].item_handle_func = MMIWIDGET_Shortcutset_SwitchFlymode;
    s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_FLYMODE].get_state_func = MMIWIDGET_Shortcutset_SetFlymode;
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_FLYMODE].is_wait = FALSE;
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_FLYMODE].is_focus = FALSE;

    // init AUTO keylock
    s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_AUTO_LOCK_SCREEN].image_id = IMAGE_WIDGET_SHORTCUT_SET_REDKEYLOCK;
    s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_AUTO_LOCK_SCREEN].item_handle_func = MMIWIDGET_Shortcutset_OpenAutokeylock;
    s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_AUTO_LOCK_SCREEN].get_state_func = MMIWIDGET_Shortcutset_SetAutoKeylock;
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_AUTO_LOCK_SCREEN].is_wait = FALSE;
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_AUTO_LOCK_SCREEN].is_focus = FALSE;

    // init redkeylock
    s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_REDKEYLOCK].image_id = IMAGE_WIDGET_SHORTCUT_SET_REDKEYLOCK;
    s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_REDKEYLOCK].item_handle_func = MMIWIDGET_Shortcutset_OpenRedkeylock;
    s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_REDKEYLOCK].get_state_func = MMIWIDGET_Shortcutset_SetRedkeylock;
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_REDKEYLOCK].is_wait = FALSE;
	s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_REDKEYLOCK].is_focus = FALSE;

    // init s_shortcut_set_disp_arr
#ifdef MMI_WIFI_SUPPORT
    s_shortcut_set_disp_arr[0] = MMIWIDGET_SHORTCUT_SET_WIFI;
#else
    s_shortcut_set_disp_arr[0] = MMIWIDGET_SHORTCUT_SET_FLYMODE;
#endif

#ifdef BLUETOOTH_SUPPORT
    s_shortcut_set_disp_arr[1] = MMIWIDGET_SHORTCUT_SET_BT;
#else
    #ifdef MMI_WIFI_SUPPORT
    s_shortcut_set_disp_arr[1] = MMIWIDGET_SHORTCUT_SET_FLYMODE;
    #else
    s_shortcut_set_disp_arr[1] = MMIWIDGET_SHORTCUT_SET_AUTO_LOCK_SCREEN;
    //s_shortcut_set_disp_arr[1] = MMIWIDGET_SHORTCUT_SET_REDKEYLOCK;
    #endif
#endif
    
     s_shortcut_set_disp_arr[2] = MMIWIDGET_SHORTCUT_SET_BACKLIGHT;
     s_shortcut_set_disp_arr[3] = MMIWIDGET_SHORTCUT_SET_SILENT;
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut Set Win Msg
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void DisplayShortcutSet(MMI_WIN_ID_T  win_id)
{
    GUI_RECT_T win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T item_img_rect = {0};
    GUI_POINT_T bottom_img_point = {0};
    uint32 i = 0;
    uint32 item_num = MAX_MMIWIDGET_SHORTCUT_SET_ITEM;
    MMI_IMAGE_ID_T   	show_image_id = 0;  
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
        IMAGE_WIDGET_SHORTCUT_SET_BG,
        &lcd_dev_info
        );
    
        
        for(i = 0 ; i < item_num ;i++)
        {            
            item_img_rect = s_shortcut_set_item_t[s_shortcut_set_disp_arr[i]].rect;
            
            //show item image 		
            show_image_id= s_shortcut_set_item_t[s_shortcut_set_disp_arr[i]].image_id;
            
            GUIRES_GetImgWidthHeight(&img_width, &img_height, show_image_id, win_id);

            item_img_rect = GUI_GetCenterRect(item_img_rect,  img_width,  img_height);
            
            item_img_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, item_img_rect);
            GUIRES_DisplayImg(
                PNULL,
                &item_img_rect,
                PNULL,
                win_id,
                show_image_id,
                &lcd_dev_info
                );

            //show item image 		
            show_image_id= s_shortcut_set_item_t[s_shortcut_set_disp_arr[i]].bottom_image_id;
            
            GUIRES_GetImgWidthHeight(&img_width, &img_height, show_image_id, win_id);

            bottom_img_point.x = item_img_rect.left +  (item_img_rect.right 
                        - item_img_rect.left - img_width)/2;
            bottom_img_point.y = item_img_rect.bottom + 1;
            //bottom_img_point = MMI_ConvertWinPoint(MMICOM_WINPOS_WIN2DISP, win_id, bottom_img_point);
            GUIRES_DisplayImg(
                &bottom_img_point,
                PNULL,
                PNULL,
                win_id,
                s_shortcut_set_item_t[s_shortcut_set_disp_arr[i]].bottom_image_id,
                &lcd_dev_info
                );		
        }
}

#ifdef MMI_WIFI_SUPPORT
/*****************************************************************************/
//     Description : Handle Widget Shortcut open wifi
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void MMIWIDGET_Shortcutset_OpenWifi()
{
    MMIWIFI_NOTIFY_INFO_T notify_info = {0};
    
	if(s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_WIFI].is_open)
	{
        MMIAPIWIFI_SetInfo(FALSE);
		MMIAPIWIFI_Off();
	
	}
	else
	{
	    notify_info.module_id = MMI_MODULE_WIDGET;
	    notify_info.notify_func = PowerOnWifiCallback;
  		MMIAPIWIFI_RegNotifyInfo(&notify_info);
  		
		if (MMIAPIWIFI_On(TRUE))
		{
		    s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_WIFI].is_wait = TRUE;
		}
	}

}
#endif

#ifdef BLUETOOTH_SUPPORT
/*****************************************************************************/
//     Description : Handle Widget Shortcut open Bt
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void MMIWIDGET_Shortcutset_OpenBt()
{
	if(s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BT].is_open)
	{
		//MMIAPIBT_OpenBlueTooth(FALSE);
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_POWER_ONOFF);
                MMIBT_OpenBluetooth(FALSE, TRUE);

	}
	else
	{
		//MMIAPIBT_OpenBlueTooth(TRUE);

		MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_POWER_ONOFF);
            	MMIBT_OpenBluetooth(TRUE, TRUE);

	}

}
#endif

/*****************************************************************************/
//     Description : Handle Widget Shortcut set  backlight
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void MMIWIDGET_Shortcutset_SetBacklight()
{
	uint32                       real_contrast     =   0;
		
	if(s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BACKLIGHT].image_id == IMAGE_WIDGET_SHORTCUT_SET_BACKLIGHT_1)
	{
	     real_contrast = 4;
	}
	else if(s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_BACKLIGHT].image_id == IMAGE_WIDGET_SHORTCUT_SET_BACKLIGHT_2)
	{
	     real_contrast = 7;
	}
	else
	{
	     real_contrast = 0;
	}

	MMIAPISET_SetCurrentContrast(real_contrast);      
	MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID, real_contrast);
	MMIAPISET_SetMainLCDContrast();
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut open Silent
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void MMIWIDGET_Shortcutset_OpenSilent()
{
#if defined(MMI_WIDGET_SHORTCUT_SET) || defined(PDA_UI_DROPDOWN_WIN)       
    if(s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_SILENT].is_open)
    {
        MMIAPIENVSET_ChangeToNoiseMode();
    }
 
    else
    {
        MMIAPIENVSET_ChangeToSilentMode();
    }
#endif    
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut Switch Flymode
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void MMIWIDGET_Shortcutset_SwitchFlymode()
{
	if(s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_FLYMODE].is_open)
	{
		MMIAPISET_SetFlyModeWholeFlow(FALSE);
	}
	else
	{
		MMIAPISET_SetFlyModeWholeFlow(TRUE);
	}
}

LOCAL void MMIWIDGET_Shortcutset_OpenAutokeylock()
{
	if(s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_AUTO_LOCK_SCREEN].is_open)
	{
        MMISET_SetAutoBoardKey(MMISET_KEYLOCK_CLOSE);
    }
    else
    {
        MMISET_SetAutoBoardKey(MMISET_KEYLOCK_30_SEC);
    }
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut Open Redkeylock
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void MMIWIDGET_Shortcutset_OpenRedkeylock()
{
	if(s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_REDKEYLOCK].is_open)
	{
		MMIAPISET_SetRedKeyLock(FALSE);
	}
	else
	{
		MMIAPISET_SetRedKeyLock(TRUE);
	}
}

#ifdef MMI_WIFI_SUPPORT
/*****************************************************************************/
//     Description : Callback function to power on wifi.
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL void PowerOnWifiCallback(MMIWIFI_NOTIFY_EVENT_E event_id,MMIWIFI_NOTIFY_PARAM_T* param_ptr)
{ 
    if ((MMIWIFI_NOTIFY_EVENT_ON == event_id)
        && WIFISUPP_RESULT_SUCC != param_ptr->result)
    {
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_WIFI].bottom_image_id = IMAGE_WIDGET_SHORTCUT_SET_OFF;  
        s_shortcut_set_item_t[MMIWIDGET_SHORTCUT_SET_WIFI].is_wait = FALSE;
    }
}
#endif

/*****************************************************************************/
//     Description : Handle Widget Shortcut Set tp down Msg
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL MMI_RESULT_E  MMIWIDGET_ShortcutSet_HandleTpPressDown(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_FALSE;
   	GUI_POINT_T point = {0};
    // GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
    uint32 i = 0;
    uint32 item_num = 0;
    GUI_RECT_T hit_rect = {0};
    uint16 bg_width = 0;
    uint16 bg_height = 0;
    MMI_CheckAllocatedMemInfo();
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);
    
    item_num = 	MAX_MMIWIDGET_SHORTCUT_SET_ITEM;

    GUIRES_GetImgWidthHeight(&bg_width, &bg_height,IMAGE_WIDGET_SHORTCUT_SET_BG, win_id);
    for(i = 0; i< item_num; i++)
    {
		hit_rect.left = (bg_width/MAX_MMIWIDGET_SHORTCUT_SET_ITEM) * i;
		hit_rect.right = bg_width/MAX_MMIWIDGET_SHORTCUT_SET_ITEM + hit_rect.left;
		hit_rect.top = 0;
		hit_rect.bottom = bg_height;
        if(GUI_PointIsInRect(point, hit_rect))
        {
            s_shortcut_set_item_t[s_shortcut_set_disp_arr[i]].is_focus = TRUE;
        }
    }
    MMI_CheckAllocatedMemInfo();
    return recode;
}

/*****************************************************************************/
//     Description : Handle Widget Shortcut Set  tp up Msg
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/ 
LOCAL MMI_RESULT_E  MMIWIDGET_ShortcutSet_HandleTpPressUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
   	GUI_POINT_T point = {0};
    // GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
    uint32 i = 0;
    uint32 item_num = 0;
    GUI_RECT_T hit_rect = {0};
    uint16 bg_width = 0;
    uint16 bg_height = 0;
    MMI_CheckAllocatedMemInfo();
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);
    
    item_num = 	MAX_MMIWIDGET_SHORTCUT_SET_ITEM;
    GUIRES_GetImgWidthHeight(&bg_width, &bg_height,IMAGE_WIDGET_SHORTCUT_SET_BG, win_id);
    for(i = 0; i< item_num; i++)
    {
        s_shortcut_set_item_t[s_shortcut_set_disp_arr[i]].is_focus = FALSE;

		hit_rect.left = (bg_width/MAX_MMIWIDGET_SHORTCUT_SET_ITEM) * i;
		hit_rect.right = bg_width/MAX_MMIWIDGET_SHORTCUT_SET_ITEM + hit_rect.left;
		hit_rect.top = 0;
		hit_rect.bottom = bg_height;
        if(GUI_PointIsInRect(point, hit_rect))
        {
            s_shortcut_set_item_t[s_shortcut_set_disp_arr[i]].item_handle_func();
            UILAYER_SetDirectDraw( FALSE );            
        }
    }
    MMI_CheckAllocatedMemInfo();
    return recode;
    
}
#endif
