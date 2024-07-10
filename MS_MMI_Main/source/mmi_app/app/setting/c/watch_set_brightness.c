/*****************************************************************************
** File Name:      watch_set_brightness.c                                    *
** Author:         bin.wang1                                                 *
** Date:           02/18/2020                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:   Display brightness setting                                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2020      bin.wang1              Creat                                 *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "guilistbox.h"
#include "watch_common_list.h"
#include "os_api.h"
#include "sci_types.h"
#include "window_parse.h"
#include "guilabel.h"
#include "guibutton.h"
#include "mmicom_trace.h"
#include "mmidisplay_data.h"

#include "mmiset_image.h"
#include "mmiset_text.h"
#include "mmiset_id.h"
#include "mmiset_export.h"
#include "watch_set_brightness.h"

#include "watch_set_position.h"
#include "mmi_app_setting_trc.h"
#include "watch_common_btn.h"
#include "watch_common.h"

#define MMISET_MINI_CONTRAST            0

MMI_IMAGE_ID_T s_settings_brightness_icon[] = {
                                                IMAGE_SET_BRIGHTNESS_INTENSITY_00,
                                                IMAGE_SET_BRIGHTNESS_INTENSITY_01,
                                                IMAGE_SET_BRIGHTNESS_INTENSITY_02,
                                                IMAGE_SET_BRIGHTNESS_INTENSITY_03,
                                                IMAGE_SET_BRIGHTNESS_INTENSITY_04,
                                                IMAGE_SET_BRIGHTNESS_INTENSITY_05,
                                                IMAGE_SET_BRIGHTNESS_INTENSITY_06,
                                                IMAGE_SET_BRIGHTNESS_INTENSITY_07
                                              };


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

LOCAL void SettingsBrightnessCheckValid( uint8* level_ptr )
{
    TRACE_APP_SETTINGS(" enter");

    if( PNULL == level_ptr )
    {
        TRACE_APP_SETTINGS("level_ptr error!!");
        return;
    }

    if( (*level_ptr) > MMISET_CONTRAST_SEVEN )
    {
        TRACE_APP_SETTINGS("brightnessLevel error %d reset to defaut value", *level_ptr);

        *level_ptr = MMISET_CONTRAST_FIVE;
    }
}

LOCAL void SettingsBrightnessSetIconId( MMI_CTRL_ID_T ctrl_id )
{
    uint8 brightnessLevel = MMIAPISET_GetCurrentContrast();

    TRACE_APP_SETTINGS(" enter");

    SettingsBrightnessCheckValid( &brightnessLevel );

    GUILABEL_SetIcon( ctrl_id, s_settings_brightness_icon[brightnessLevel] );//level0~level7
}

LOCAL void SettingsBrightnessUpdate( uint8 level )
{
    TRACE_APP_SETTINGS(" enter level %d", level);

    MMIAPISET_SetCurrentContrastEx( level );
    MMIAPISET_SetMainLCDContrast();
    MMIAPISET_UpdateLCDContrast( GUI_MAIN_LCD_ID, level );

    MMK_SendMsg( MMISET_BRIGHTNESS_WIN_ID, MSG_FULL_PAINT, PNULL );
}

LOCAL MMI_RESULT_E HandleSettingsBrightnessBtnLeft( void )
{
    uint8 brightnessLevel = MMIAPISET_GetCurrentContrast();

    SettingsBrightnessCheckValid( &brightnessLevel );

    if( brightnessLevel > MMISET_MINI_CONTRAST )
    {
        brightnessLevel--;
        SettingsBrightnessUpdate( brightnessLevel );
    }

    return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E HandleSettingsBrightnessBtnRight( void )
{
    uint8 brightnessLevel = MMIAPISET_GetCurrentContrast();

    TRACE_APP_SETTINGS(" enter");

    SettingsBrightnessCheckValid( &brightnessLevel );

    if( brightnessLevel < MMISET_CONTRAST_SEVEN )
    {
        brightnessLevel++;
        SettingsBrightnessUpdate( brightnessLevel );
    }

    return MMI_RESULT_TRUE;
}

LOCAL void Settings_Brightness_OPEN_WINDOW( MMI_WIN_ID_T win_id )
{
    GUI_RECT_T labelRect = { WATCH_SET_DEFAULT_TITLE_X, WATCH_SET_DEFAULT_TITLE_Y, WATCH_SET_DEFAULT_TITLE_X+WATCH_SET_DEFAULT_TITLE_W, WATCH_SET_DEFAULT_TITLE_Y+WATCH_SET_DEFAULT_TITLE_H };
    MMI_CTRL_ID_T ctrlLabel = MMISET_BRIGHTNESS_TITLE_LABEL_CTRL_ID;

    GUI_RECT_T iconRect = { WATCH_SET_DEFAULT_ICON_X, WATCH_SET_DEFAULT_ICON_Y, WATCH_SET_DEFAULT_ICON_X+WATCH_SET_DEFAULT_ICON_W, WATCH_SET_DEFAULT_ICON_Y+WATCH_SET_DEFAULT_ICON_H };
    MMI_CTRL_ID_T ctrlIcon = MMISET_BRIGHTNESS_ICON_LABEL_CTRL_ID;

    MMI_CTRL_ID_T leftBtn = MMISET_BRIGHTNESS_L_BTN_CTRL_ID;
    MMI_CTRL_ID_T rightBtn = MMISET_BRIGHTNESS_R_BTN_CTRL_ID;

    BOOLEAN reCode = TRUE;

    TRACE_APP_SETTINGS(" enter");

    //title label
    GUILABEL_SetRect( ctrlLabel, &labelRect, FALSE );
#ifdef SCREEN_SHAPE_CIRCULAR
    GUILABEL_SetFont( ctrlLabel, WATCH_FONT_22, MMI_WHITE_COLOR );
#else
    GUILABEL_SetFont( ctrlLabel, WATCH_DEFAULT_BIG_FONT, MMI_WHITE_COLOR );
#endif
#ifdef ADULT_WATCH_SUPPORT    
    GUILABEL_SetTextById( ctrlLabel, TXT_SET_DISPLAY_BRIGHTNESS, FALSE );
#else
    GUILABEL_SetTextById( ctrlLabel, TXT_COMMON_COMMON_BRIGHTNESS, FALSE );
#endif

    //icon label
    GUILABEL_SetRect( ctrlIcon, &iconRect, FALSE );

    //button
    WatchCOM_Btn_SetPos_2Btn( leftBtn, rightBtn );
    reCode= GUIBUTTON_SetCallBackFunc( leftBtn, HandleSettingsBrightnessBtnLeft );
    if( TRUE != reCode )
    {
        TRACE_APP_SETTINGS("set left btn callback error");
    }
    reCode= GUIBUTTON_SetCallBackFunc( rightBtn, HandleSettingsBrightnessBtnRight );
    if( TRUE != reCode )
    {
        TRACE_APP_SETTINGS("set right btn callback error");
    }
}

LOCAL void Settings_Brightness_FULL_PAINT( MMI_WIN_ID_T win_id )
{
    TRACE_APP_SETTINGS(" enter");

    //bg color
    WATCHCOM_DisplayBackground( win_id );

    //icon label
    SettingsBrightnessSetIconId( MMISET_BRIGHTNESS_ICON_LABEL_CTRL_ID );

}

LOCAL void Settings_Brightness_APP_LEFT_RIGHT( MMI_MESSAGE_ID_E msg_id )
{
    TRACE_APP_SETTINGS(" enter");

    if( MSG_APP_LEFT == msg_id )
    {
        HandleSettingsBrightnessBtnLeft();
    }
    else
    {
        HandleSettingsBrightnessBtnRight();
    }
}

LOCAL void Settings_Brightness_APP_CANCEL( MMI_WIN_ID_T win_id )
{
    MMK_CloseWin( win_id );
}

LOCAL MMI_RESULT_E HandleSettingsBrightnessWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E reCode = MMI_RESULT_TRUE;

    TRACE_APP_SETTINGS(" enter %08x", msg_id);

    switch( msg_id )
    {
        case MSG_OPEN_WINDOW:
        {
            Settings_Brightness_OPEN_WINDOW( win_id );
            break;
        }

        case MSG_FULL_PAINT:
        {
            Settings_Brightness_FULL_PAINT( win_id );
            break;
        }

        case MSG_APP_LEFT:
        case MSG_APP_RIGHT:
        {
            Settings_Brightness_APP_LEFT_RIGHT( msg_id );
            break;
        }
        case MSG_KEYDOWN_RED:
            break;
        case MSG_KEYUP_RED:
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            Settings_Brightness_APP_CANCEL( win_id );
            break;
        }

        default:
            reCode = MMI_RESULT_FALSE;
            break;
    }

    return reCode;
}

//idle -> settings -> brightness
WINDOW_TABLE( MMISET_BRIGHTNESS_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleSettingsBrightnessWindow ),
    WIN_ID( MMISET_BRIGHTNESS_WIN_ID ),
    WIN_HIDE_STATUS,
    CREATE_LABEL_CTRL( GUILABEL_ALIGN_MIDDLE, MMISET_BRIGHTNESS_TITLE_LABEL_CTRL_ID ),
    CREATE_LABEL_CTRL( GUILABEL_ALIGN_MIDDLE, MMISET_BRIGHTNESS_ICON_LABEL_CTRL_ID ),
    CREATE_BUTTON_CTRL( IMAGE_SET_COM_BTN_DECREASE, MMISET_BRIGHTNESS_L_BTN_CTRL_ID ),
    CREATE_BUTTON_CTRL( IMAGE_SET_COM_BTN_INCREASE, MMISET_BRIGHTNESS_R_BTN_CTRL_ID ),
    WIN_SOFTKEY( TXT_NULL, TXT_NULL, STXT_RETURN ),
    END_WIN
};

PUBLIC void Settings_BrightnessWin_Enter( void )
{
    MMK_CreateWin( (uint32 *)MMISET_BRIGHTNESS_WIN_TAB, PNULL );
}

