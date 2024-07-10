/*****************************************************************************
** File Name:   watch_flashlight.c                                           *
** Author:      miao.xi                                                      *
** Date:        2021/1/27                                                    *
** Copyright:                                                                *
** Description: This file is used to process msg to main window of flashlight*
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
**   DATE                  NAME              DESCRIPTION                     *
** 01/27/2021             miao.xi              Create                        *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "dal_time.h"
#include "window_parse.h"
#include "mmk_timer.h"
#include "mmi_appmsg.h"
#include "mmidisplay_data.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_default.h"
#include "mmiacc_id.h"
#include "mmiacc_position.h"


#include "mmiset_export.h"
#include "watch_flashlight.h"

#include "watch_common.h"
#include "mmicom_trace.h"

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/
LOCAL uint8 BrightnessLevel = 0;
LOCAL MMISET_BACKLIGHT_SET_T backlightinfo = {0};
/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/
LOCAL void SettingsBrightness( uint8 level )
{
    SCI_TRACE_LOW(" enter level %d", level);

    MMIAPISET_SetCurrentContrastEx( level );
    MMIAPISET_SetMainLCDContrast();
    MMIAPISET_UpdateLCDContrast( GUI_MAIN_LCD_ID, level );
}


LOCAL MMI_RESULT_E HandleMsgFlashLightWindow( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E    recode      = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            uint8    Brightness_Max = MMISET_CONTRAST_SEVEN;
            MMISET_BACKLIGHT_SET_T baklightAlwaysOn = {0};
            baklightAlwaysOn.backlight_type = MMISET_BACKLIGHT_OPEN_FOREVER;

            //record brightness before
            BrightnessLevel = MMIAPISET_GetCurrentContrast();
            //set brightness max
            SettingsBrightness(Brightness_Max);

            //record backlightinfo
            MMIAPISET_GetBackLightInfo(&backlightinfo);
            //set backlight Always on
            MMIAPISET_SetBackLightInfo(baklightAlwaysOn);
        }
        break;
    case MSG_FULL_PAINT:
        {
            uint16               lcd_width      = 0;
            uint16               lcd_height     = 0;
            GUI_RECT_T           BackGroundRect = {0};
            GUI_LCD_DEV_INFO     lcd_dev_info   = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

            //draw background
            GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
            BackGroundRect.left   = 0;
            BackGroundRect.top    = 0;
            BackGroundRect.right  = lcd_width-1;
            BackGroundRect.bottom = lcd_height-1;
            LCD_FillRect(&lcd_dev_info, BackGroundRect, MMI_WHITE_COLOR);

        }
        break;
    case MSG_LOSE_FOCUS:
        SettingsBrightness(BrightnessLevel);
        MMIAPISET_SetBackLightInfo(backlightinfo);
        break;
    case MSG_GET_FOCUS:
        {
            uint8    Brightness_Max = MMISET_CONTRAST_SEVEN;
            MMISET_BACKLIGHT_SET_T baklightAlwaysOn = {0};
            baklightAlwaysOn.backlight_type = MMISET_BACKLIGHT_OPEN_FOREVER;

            SettingsBrightness(Brightness_Max);
            MMIAPISET_SetBackLightInfo(baklightAlwaysOn);
        }
        break;
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_CLOSE_WINDOW:
        {
            //set brightness before
            SettingsBrightness(BrightnessLevel);

            MMIAPISET_SetBackLightInfo(backlightinfo);

            //MMK_ReturnIdleWin();
            MMK_CloseWin(win_id);
        }
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

// window table of flashlight win
WINDOW_TABLE(WATCH_FLASHLITHT_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_ID(WATCH_FLASHLITHT_WIN_ID),
    WIN_FUNC((uint32)HandleMsgFlashLightWindow),
    END_WIN
};

/*****************************************************************************/
//  Description: enter flash light window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchFlashLight_MainWin_Enter( void )
{
    MMK_CreateWin((uint32 *)WATCH_FLASHLITHT_WIN_TAB,PNULL);
}

/*****************************************************************************/
//  Description: exist flash light window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchFlashLight_MainWin_Exit( void )
{
    MMK_CloseWin(WATCH_FLASHLITHT_WIN_ID);
}
