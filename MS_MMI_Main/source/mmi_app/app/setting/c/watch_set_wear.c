/*****************************************************************************
** File Name:      watch_set_wear.c                                          *
** Author:         xiaoju.cheng                                              *
** Date:           01/06/2021                                                *
** Copyright:      All Rights Reserved.                                      *
** Description: Display wear settings items                                  *
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
#ifdef WATCH_LEFT_RIGHT_WEAR_SUPPORT
#include "dal_display.h"
#endif
#ifdef ADULT_WATCH_SUPPORT
#include "watch_sensor.h"
#endif

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
#define SETTINGS_WEAR_HAND_ITEM_MAX                (2)

LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_wear_left_hand  = { TXT_SET_WEAR_LEFT_HAND };
LOCAL WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST s_wear_right_hand = { TXT_SET_WEAR_RIGHT_HAND };

LOCAL WATCHCOM_LIST_ITEM__ST s_wear_hand_list[] =
{
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,   &s_wear_left_hand,      PNULL },
    { WatchCOM_List_Item_Visible_Default,   GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,   &s_wear_right_hand,     PNULL },
};

LOCAL WATCHCOM_RADIOLIST_WIN_INFO_ST s_wear_hand_win             = { MMISET_WEAR_WIN_ID,
                                                                        MMISET_WEAR_LIST_CTRL_ID,
                                                                        s_wear_hand_list,
                                                                        PNULL,
                                                                        0,
                                                                        SETTINGS_WEAR_HAND_ITEM_MAX,
                                                                        TXT_SET_LIST_WEAR};



/**---------------------------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DEFINITION                                           *
 **---------------------------------------------------------------------------------------------*/
LOCAL void Settings_Wear_Select(void)
{
    uint32	cur_selection	=	0;
    DISPLAY_ANGLE_E display_angle = DISPLAY_ANGLE_0;//DISPLAY_ANGLE_270 + 1 pclint
    GUI_LCD_DEV_INFO dev_info_x = {0};

	//get the selected item
	cur_selection = GUILIST_GetCurItemIndex(MMISET_WEAR_LIST_CTRL_ID);
	
	SCI_TRACE_LOW("Settings_Wear_Select:cur_selection=%d",cur_selection);

	switch (cur_selection)
	{
    case 0:
            display_angle = DISPLAY_ANGLE_0;
            break;

	case 1:
            display_angle = DISPLAY_ANGLE_180;
            break;

	default:
		break;
	}
#ifdef WATCH_LEFT_RIGHT_WEAR_SUPPORT
    LCD_SetWearType(display_angle);
#endif
#ifdef ADULT_WATCH_SUPPORT
    if(TRUE == WATCHAPISET_GetRaiseWrist())
    {
        AdultWatch_DeactiveRaiseWristSensor();
        AdultWatch_UnregesterRaiseWristSensor();
        AdultWatch_RegisterRaiseWristSensor();
        AdultWatch_ActiveRaiseWristSensor();
    }
#endif
    MMITHEME_StoreUpdateRect(&dev_info_x,MMITHEME_GetFullScreenRect());
    //function todo.
}

LOCAL MMI_RESULT_E _Callback_WearSelectWin( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E    recode =  MMI_RESULT_TRUE;

    switch(msg_id)
    {

        case MSG_NOTIFY_LIST_SET_SELECT:
        {
            Settings_Wear_Select();
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

PUBLIC void Settings_WearWin_Enter( void )
{
    uint16 curIndex = 0;

#ifdef WATCH_LEFT_RIGHT_WEAR_SUPPORT
    if (DISPLAY_ANGLE_0 == LCD_GetWearType())
    {
        curIndex = 0;
    }
    else
    {
        curIndex = 1;
    }
#endif
    s_wear_hand_win.listCurIdx = curIndex;
    s_wear_hand_win.pCallback  = _Callback_WearSelectWin;
    AdultWatchCOM_RadioListWin_Create( &s_wear_hand_win );
}
