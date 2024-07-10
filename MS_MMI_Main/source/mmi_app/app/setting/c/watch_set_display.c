/*****************************************************************************
** File Name:      watch_set_display.c                                       *
** Author:         xiaoju.cheng                                              *
** Date:           01/06/2021                                                *
** Copyright:      All Rights Reserved.                                      *
** Description: Display display settings items                               *
******************************************************************************
**                         Important Edit History
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 01/06/2020           xiaoju.cheng             Create                      *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/

#include "guilistbox.h"
#include "watch_common_list.h"
#include "os_api.h"
#include "sci_types.h"
#include "window_parse.h"
#include "mmicom_trace.h"

#include "mmiset_image.h"
#include "mmiset_text.h"
#include "mmiset_id.h"
#include "mmiset_export.h"
#include "watch_common_radiolist_win.h"
#include "watch_commonwin_export.h"
#ifdef ADULT_WATCH_SUPPORT
#include "watch_sensor.h"
#endif
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
typedef enum
{
    SETTINGS_DISPLAY_RAISE_WRIST,
    SETTINGS_DISPLAY_BRIGHTNESS,
    SETTINGS_DISPLAY_BACKLIGHT,
    SETTINGS_DISPLAY_ITEM_MAX
}
SETTINGS_DISPLAY_LIST_ITEM_E;

#define SETTINGS_AUTO_BACKLIGHT_ITEM_MAX                (7)

LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_auto_backlight_5s =  { TXT_SET_TIME_5S };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_auto_backlight_10s = { TXT_SET_TIME_10S };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_auto_backlight_15s = { TXT_SET_TIME_15S };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_auto_backlight_20s = { TXT_SET_TIME_20S };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_auto_backlight_25s = { TXT_SET_TIME_25S };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_auto_backlight_30s = { TXT_SET_TIME_30S };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_auto_backlight_always_open = { TXT_SET_TIME_OPEN_FOREVER };

LOCAL WATCHCOM_LIST_ITEM__ST s_auto_backlight_list[] =
{
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,   &s_auto_backlight_5s,              PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,   &s_auto_backlight_10s,             PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,   &s_auto_backlight_15s,             PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,   &s_auto_backlight_20s,             PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,   &s_auto_backlight_25s,             PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,   &s_auto_backlight_30s,             PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,   &s_auto_backlight_always_open,     PNULL },
};

LOCAL WATCHCOM_RADIOLIST_WIN_INFO_ST s_auto_backlight_win = { MMISET_AUTO_BACKLIGHT_WIN_ID,
                                                                MMISET_AUTO_BACKLIGHT_CTRL_ID,
                                                                s_auto_backlight_list,
                                                                PNULL,
                                                                0,
                                                                SETTINGS_AUTO_BACKLIGHT_ITEM_MAX,
                                                                TXT_SET_DISPLAY_BACKLIGHT};

/**---------------------------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DEFINITION                                           *
 **---------------------------------------------------------------------------------------------*/

LOCAL void Settings_BackLight_Select(void)
{
	uint32	cur_selection	=	0;
    MMISET_BACKLIGHT_SET_T backlightinfo = {0};

	//get the selected item
	cur_selection = GUILIST_GetCurItemIndex(MMISET_AUTO_BACKLIGHT_CTRL_ID);
	
	TRACE_APP_SETTINGS("Settings_BackLight_Select:cur_selection=%d",cur_selection);

	switch (cur_selection)
	{
	case 0:
		backlightinfo.backlight_type = MMISET_BACKLIGHT_OPEN;
		backlightinfo.backlight_time = MMISET_TIME_5;
		break;

	case 1:
		backlightinfo.backlight_type = MMISET_BACKLIGHT_OPEN;
		backlightinfo.backlight_time = MMISET_TIME_10;
		break;

	case 2:
		backlightinfo.backlight_type = MMISET_BACKLIGHT_OPEN;
		backlightinfo.backlight_time = MMISET_TIME_15;
		break;

	case 3:
		backlightinfo.backlight_type = MMISET_BACKLIGHT_OPEN;
		backlightinfo.backlight_time = MMISET_TIME_20;
		break;

	case 4:
		backlightinfo.backlight_type = MMISET_BACKLIGHT_OPEN;
		backlightinfo.backlight_time = MMISET_TIME_25;
		break;

	case 5:
		backlightinfo.backlight_type = MMISET_BACKLIGHT_OPEN;
		backlightinfo.backlight_time = MMISET_TIME_30;
		break;

	case 6:
		backlightinfo.backlight_type = MMISET_BACKLIGHT_OPEN_FOREVER;
		break;

	default:
		break;
	}
	MMIAPISET_SetBackLightInfo(backlightinfo);
    MMIDEFAULT_StartLcdBackLightTimer();//bug1587326, 1579831
}

LOCAL MMI_RESULT_E Settings_Auto_Backlight_Callback( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E    recode =  MMI_RESULT_TRUE;

    switch(msg_id)
    {

        case MSG_NOTIFY_LIST_SET_SELECT:
        {
            Settings_BackLight_Select();
            MMK_CloseWin( win_id );
            break;
        }

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMK_CloseWin( win_id );
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

 /**--------------------------------------------------------------------------*
  **						 FUNCTION DEFINITION							  *
  **--------------------------------------------------------------------------*/
 LOCAL uint16 Settings_Auto_BackLight_Get_CurSelection( void)
{
    MMISET_BACKLIGHT_SET_T backlightinfo = {0};
    uint16 curSelection = SETTINGS_AUTO_BACKLIGHT_ITEM_MAX;
    backlightinfo = WATCHAPISET_GetBackLightinfo();

    //select backlight type
    TRACE_APP_SETTINGS("watchset:Settings_Auto_BackLight_Enter backlight_type = %d",backlightinfo.backlight_type);
    switch (backlightinfo.backlight_type)
    {
        case MMISET_BACKLIGHT_OPEN:
            switch (backlightinfo.backlight_time)
            {
                case MMISET_TIME_5:
                    curSelection = 0;
                    break;

                 case MMISET_TIME_10:
                    curSelection = 1;
                    break;

                 case MMISET_TIME_15:
                    curSelection = 2;
                    break;

                 case MMISET_TIME_20:
                    curSelection = 3;
                    break;

                 case MMISET_TIME_25:
                    curSelection = 4;
                    break;

                 case MMISET_TIME_30:
                    curSelection = 5;
                    break;

                 default:
                    SCI_TRACE_LOW("watchset:Settings_Auto_BackLight_Enter default");
                 break;
           }
           break;

           //³£¿ª
       case MMISET_BACKLIGHT_OPEN_FOREVER:
           curSelection = 6;
           break;

       case MMISET_BACKLIGHT_CLOSE:
           TRACE_APP_SETTINGS("watchset:Settings_Auto_BackLight_Enter MMISET_BACKLIGHT_CLOSE");
           break;

       default:
           break;
    }
    return curSelection;
}

 LOCAL void Settings_Auto_BackLight_Enter( void )
{
    s_auto_backlight_win.listCurIdx    = Settings_Auto_BackLight_Get_CurSelection();
    s_auto_backlight_win.pCallback     = Settings_Auto_Backlight_Callback;
    AdultWatchCOM_RadioListWin_Create( &s_auto_backlight_win );
}
LOCAL void Settings_Display_OPEN_WINDOW( MMI_WIN_ID_T win_id )
{
    MMI_CTRL_ID_T   listCtrlId =  MMISET_DISPLAY_LIST_CTRL_ID;

    GUILIST_SetMaxItem( listCtrlId, SETTINGS_DISPLAY_ITEM_MAX,  FALSE );
    MMK_SetAtvCtrl( win_id, listCtrlId );
}

LOCAL void Settings_RaiseWristSelect( MMI_WIN_ID_T win_id )
{
    MMI_IMAGE_ID_T      imageId = IMAGE_SWITCH_OFF_SELECTED;
    BOOLEAN             is_raise_wrist_on = FALSE;
    MMI_STRING_T        itemStr1 = { 0 };
    uint32              userData = 0;
    MMI_CTRL_ID_T       listCtrlId =  MMISET_DISPLAY_LIST_CTRL_ID;

    is_raise_wrist_on = WATCHAPISET_GetRaiseWrist();

    // status image
    if( TRUE == is_raise_wrist_on )
    {
        is_raise_wrist_on = FALSE;
        imageId = IMAGE_SWITCH_OFF_SELECTED;
#ifdef ADULT_WATCH_SUPPORT
        AdultWatch_DeactiveRaiseWristSensor();
        AdultWatch_UnregesterRaiseWristSensor();
#endif
    }
    else
    {
        is_raise_wrist_on = TRUE;
        imageId = IMAGE_SWITCH_ON_SELECTED;
#ifdef ADULT_WATCH_SUPPORT
        AdultWatch_RegisterRaiseWristSensor();
        AdultWatch_ActiveRaiseWristSensor();
#endif
    }
    WATCHAPISET_SetRaiseWrist(is_raise_wrist_on);
    MMI_GetLabelTextByLang( TXT_SET_DISPLAY_RAISE_WIRST, &itemStr1 );
    AdultWatchCOM_ListItem_Draw_1Str_1Icon_1line( listCtrlId, itemStr1, imageId );
    MMK_SendMsg( win_id, MSG_FULL_PAINT, 0 );
    //function todo.
}

LOCAL MMI_RESULT_E _Callback_Progress_BrightnessWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_NOTIFY_PRGBOX_CHANGE:
        {
            uint8 level = *(uint8*)param;

            TRACE_APP_SETTINGS(" _Callback_Progress_BrightnessWin level is %d", level);

            MMIAPISET_SetCurrentContrastEx( level );
            MMIAPISET_SetMainLCDContrast();
            MMIAPISET_UpdateLCDContrast( GUI_MAIN_LCD_ID, level );
            break;
        }
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

LOCAL void Settings_Display_APP_OK( MMI_WIN_ID_T win_id )
{
    uint16 index = GUILIST_GetCurItemIndex( MMISET_DISPLAY_LIST_CTRL_ID );

    switch (index)
    {
        case SETTINGS_DISPLAY_RAISE_WRIST:
        {
            Settings_RaiseWristSelect(win_id);
            break;
        }

        case SETTINGS_DISPLAY_BRIGHTNESS:
        {
            uint8 brightnessLevel = MMIAPISET_GetCurrentContrast();

            TRACE_APP_SETTINGS("Settings_Display_APP_OK,brightnessLevel is %d",brightnessLevel);
            WatchCOM_ProgressWin_Enter(MMISET_BRIGHTNESS_WIN_ID,res_aw_brightness_high,res_aw_brightness_low,MMISET_CONTRAST_SEVEN,brightnessLevel,_Callback_Progress_BrightnessWin);
            break;
        }

        case SETTINGS_DISPLAY_BACKLIGHT:
        {
            Settings_Auto_BackLight_Enter();
            break;
        }

        default:
        {
            TRACE_APP_SETTINGS(" index error %d", index);
            break;
        }
    }
}

LOCAL void Settings_Display_APP_CANCEL( MMI_WIN_ID_T win_id )
{
    MMK_CloseWin( win_id );
}

LOCAL void Settings_Display_FULL_PAINT( MMI_WIN_ID_T win_id )
{
    uint16          curSelection = 0;
    int32           listOffset = 0;
    uint16          ringId = 0;
    uint16          modeIndex = 0;
    MN_DUAL_SYS_E   dualSys = MMIAPISET_GetActiveSim();
    MMI_TEXT_ID_T*  pStr2TextId = TXT_NULL;
    MMI_STRING_T    itemStr = { 0 };
    MMI_STRING_T    itemStr2 = { 0 };
    MMI_CTRL_ID_T   listCtrlId =  MMISET_DISPLAY_LIST_CTRL_ID;
    WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST* pStStr2Text;
    BOOLEAN             is_raise_wrist_on = FALSE;
    MMI_IMAGE_ID_T      imageId = IMAGE_SWITCH_OFF_SELECTED;

    // get top item offset
    GUILIST_GetTopItemOffset( listCtrlId, &listOffset );
    // get current select item index
    curSelection = GUILIST_GetCurItemIndex( listCtrlId );
    GUILIST_RemoveAllItems( listCtrlId );

    //raise the wrist,and light up the screen.
    is_raise_wrist_on = WATCHAPISET_GetRaiseWrist();
    // status image
    if( TRUE == is_raise_wrist_on )
    {
        imageId = IMAGE_SWITCH_ON_SELECTED;
    }
    MMI_GetLabelTextByLang( TXT_SET_DISPLAY_RAISE_WIRST, &itemStr );
    AdultWatchCOM_ListItem_Draw_1Str_1Icon_1line( listCtrlId, itemStr, imageId );

    //brightness
    MMI_GetLabelTextByLang( TXT_SET_DISPLAY_BRIGHTNESS, &itemStr );
    AdultWatchCOM_ListItem_Draw_1StrLeftAlign( listCtrlId, itemStr );

    //backlight
    MMI_GetLabelTextByLang( TXT_SET_DISPLAY_BACKLIGHT,  &itemStr );
    curSelection = Settings_Auto_BackLight_Get_CurSelection(); 
    if( curSelection < SETTINGS_AUTO_BACKLIGHT_ITEM_MAX )
    {
        pStStr2Text = (WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST*)s_auto_backlight_list[curSelection].data_ptr;
        if( pStStr2Text != PNULL )
        {
            MMI_GetLabelTextByLang( pStStr2Text->text_id,  &itemStr2 );
        }
        else
        {
            TRACE_APP_SETTINGS("Settings_Display_FULL_PAINT,pStStr2Text is PNULL!!");
        }
    }
    else
    {
        TRACE_APP_SETTINGS("Settings_Display_FULL_PAINT curSelection error %d", curSelection);
    }
    AdultWatchCOM_ListItem_Draw_2Str_2line( listCtrlId, itemStr, itemStr2 );

    //set current item
    GUILIST_SetCurItemIndex( listCtrlId, curSelection );
    //set top item offset
    GUILIST_SetTopItemOffset( listCtrlId, listOffset );
}

/*****************************************************************************/
//  Description : to handle Settings window message
//  Global resource dependence :
//  Author:bin.wang1
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingsDisplayWindow(
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
            Settings_Display_OPEN_WINDOW( win_id );
            break;
        }
        case MSG_FULL_PAINT:
        {
            Settings_Display_FULL_PAINT( win_id );
            break;
        }
        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        {
            Settings_Display_APP_OK( win_id );
            break;
        }
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            Settings_Display_APP_CANCEL( win_id );
            break;
        }
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
WINDOW_TABLE( MMISET_DISPLAY_WIN_TAB ) =
{
    WIN_FUNC((uint32)HandleSettingsDisplayWindow ),
    WIN_ID( MMISET_DISPLAY_WIN_ID ),
    WIN_HIDE_STATUS,
    WIN_TITLE(TXT_SET_LIST_DISPLAY_BRIGHTNESS),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_DISPLAY_LIST_CTRL_ID),
    END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
// idle -> settings -> display
PUBLIC void Settings_DisplayWin_Enter( void )
{
    MMK_CreateWin( (uint32 *)MMISET_DISPLAY_WIN_TAB, PNULL );
}
