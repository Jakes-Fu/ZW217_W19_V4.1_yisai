/*****************************************************************************
** File Name:      adult_watch_charge_win.c                                           *
** Author:                                                                      *
** Date:           01/21/2021                                                   *
** Copyright:      2020 Spreadtrum, Incoporated. All Rights Reserved.         ..*
** Description:    watch charge win
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/21/2021  weipeng.wang        Create
******************************************************************************/


#include "watch_charge_win.h"
#include "watch_launcher_main.h"

#include "dualbat_drvapi.h"
#include "guilabel.h"
#include "guibutton.h"
#include "mmi_id.h"
#include "mmi_text.h"
#include "mmi_anim.h"

#include "mmicom_trace.h"
#include "mmiphone_export.h"
#include "mmiacc_id.h"
#include "watch_commonwin_export.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/
//charge update timer
LOCAL uint8 s_charge_update_timer = 0;
LOCAL uint8 s_charge_tips_timer = 0;
LOCAL uint8 s_cur_index = 0;

LOCAL MMI_IMAGE_ID_T s_charge_image_ids[] =
{
    res_aw_ic_charge_1,
    res_aw_ic_charge_2,
    res_aw_ic_charge_3,
    res_aw_ic_charge_4,
    res_aw_ic_charge_5,
    res_aw_ic_charge_6,
    res_aw_ic_charge_7,
    res_aw_ic_charge_8,
    res_aw_ic_charge_9,
    res_aw_ic_charge_10,
    res_aw_ic_charge_11,
    res_aw_ic_charge_12,
    res_aw_ic_charge_13,
};

/**--------------------------------------------------------------------------*
**                         LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/
//get current charge value
LOCAL uint32 GetCurrentChgValue()
{
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
    return p_chgmng_info->bat_remain_cap;
}

//judge is battery is full
LOCAL BOOLEAN isChargeFull()
{
    return (GetCurrentChgValue() == 100);
}

//start timer for charing update
LOCAL void StartChgTimer()
{
    if (s_charge_update_timer == 0)
    {
        s_charge_update_timer = MMK_CreateWinTimer(WATCH_LAUNCHER_CHARGE_WIN_ID, LAUNCHER_CHARGE_ANIM_DURATION, TRUE);
    }
}

//stop timer for charging update
LOCAL void StopChgTimer()
{
    if (s_charge_update_timer != 0)
    {
        MMK_StopTimer(s_charge_update_timer);
        s_charge_update_timer = 0;
    }
}

LOCAL void DrawChgValue(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T rect = LAUNCHER_CHARGE_VALUE_RECT;
    GUI_RECT_T win_rect = {0};
    wchar wStrBattery[16] = {0};
    char strBattery[16] = {0};
    MMI_STRING_T textStr ={0};
    GUISTR_STYLE_T text_style = {0};
    GUISTR_STATE_T state = {0};
    uint32 value = GetCurrentChgValue();
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    MMK_GetWinRect(win_id, &win_rect);
    rect.left += win_rect.left;
    rect.top += win_rect.top;
    rect.right += win_rect.left;
    rect.bottom += win_rect.top;

    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    sprintf(strBattery, "%d%s", value, "%");
    MMIAPICOM_StrToWstr(strBattery, wStrBattery);
    textStr.wstr_ptr = wStrBattery;
    textStr.wstr_len = MMIAPICOM_Wstrlen(wStrBattery);

    text_style.font_color = MMI_WHITE_COLOR;
    text_style.font = LAUNCHER_CHARGE_DEFAULT_FONT_SIZE;
    text_style.align = ALIGN_HVMIDDLE;
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T       *)&rect,
        (const GUI_RECT_T       *)&rect,
        (const MMI_STRING_T     *)&textStr,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO);
}

LOCAL void DrawChargeInfo(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T icon_rect = LAUNCHER_CHARGE_ICON_RECT;
    GUI_RECT_T anim_rect = LAUNCHER_CHARGE_ANIM_RECT;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T win_rect = {0};

    MMK_GetWinRect(win_id, &win_rect);

    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    GUI_FillRect(&lcd_dev_info, win_rect, LAUNCHER_CHARGE_BACKGROUND_COLOR);

    //draw 100% text
    DrawChgValue(win_id);

    //draw charge icon
    icon_rect.left += win_rect.left;
    icon_rect.top += win_rect.top;
    icon_rect.right += win_rect.left;
    icon_rect.bottom += win_rect.top;

    GUIRES_DisplayImg(PNULL,
                   &icon_rect,
                   PNULL,
                   win_id,
                   res_sys_charge_icon,
                   &lcd_dev_info);

    //draw charge animation
    anim_rect.left += win_rect.left;
    anim_rect.top += win_rect.top;
    anim_rect.right += win_rect.left;
    anim_rect.bottom += win_rect.top;
    if(FALSE == isChargeFull())
    {
        GUIRES_DisplayImg(PNULL,
                       &anim_rect,
                       PNULL,
                       win_id,
                       s_charge_image_ids[s_cur_index],
                       &lcd_dev_info);
    }
    else
    {
        GUIRES_DisplayImg(PNULL,
                       &anim_rect,
                       PNULL,
                       win_id,
                       res_aw_ic_charge_13,
                       &lcd_dev_info);
    }
}

//msg handler for charging win
LOCAL MMI_RESULT_E HandleLauncherChargeWinMsg(
                                      MMI_WIN_ID_T        win_id,    //IN:
                                      MMI_MESSAGE_ID_E    msg_id,    //IN:
                                      DPARAM              param      //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    static uint32 last_charge_val = 0;
    PHONE_STARTUP_CONDITION_E restart_condition = STARTUP_NONE;

    restart_condition = MMIAPIPHONE_GetStartUpCondition();
    SCI_TRACE_LOW("HandleLauncherChargeWinMsg() restart_condition = %d.", restart_condition);

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            StartChgTimer();
            s_cur_index = 0;
            last_charge_val = GetCurrentChgValue();
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            StopChgTimer();
            break;
        }

        case MSG_GET_FOCUS:
        {
            StartChgTimer();
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, PNULL);
            break;
        }

        case MSG_LOSE_FOCUS:
        {
            StopChgTimer();
            break;
        }
#if 0
        case MSG_USER_CHANGE_WINRECT:
        {
            GUI_BOTH_RECT_T both_rect = {0};
            GUI_RECT_T win_rect = {0};
            MMK_GetWinRect(win_id, &win_rect);
            both_rect.v_rect = win_rect;
            MMK_SetBothRectNotifyCtrl(win_id, &both_rect);
            break;
        }
#endif
        case MSG_FULL_PAINT:
        {
            GUI_RECT_T win_rect = LAUNCHER_CHARGE_WIN_RECT;
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            if (restart_condition == STARTUP_NORMAL)
            {
                MMK_GetWinRect(win_id, &win_rect);
            }
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            DrawChargeInfo(win_id);
            break;
        }

        case MSG_TIMER:
        {
            uint8 timer_id = *(uint8 *)param;
            if (s_charge_update_timer == timer_id && MMK_IsFocusWin(win_id))
            {
                if((FALSE == isChargeFull()) || (100 != last_charge_val))
                {
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
                }
                last_charge_val = GetCurrentChgValue();

                //refresh charging image id by timer.
                if(s_cur_index < sizeof(s_charge_image_ids)/sizeof(MMI_IMAGE_ID_T)-1)
                {
                    s_cur_index++;
                }
                else
                {
                    s_cur_index = 0;
                }
            }
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    //process default msg
    if (!recode)
    {
        recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
    }

    return recode;
}

//start timer for charing tips
LOCAL void StartChgTipsTimer()
{
    if (s_charge_tips_timer == 0)
    {
        s_charge_tips_timer = MMK_CreateWinTimer(WATCH_LAUNCHER_CHARGETIP_WIN_ID, LAUNCHER_CHARGE_ANIM_DURATION, TRUE);
    }

}

//stop timer for charging tips
LOCAL void StopChgTipsTimer()
{
    if (s_charge_tips_timer != 0)
    {
        MMK_StopTimer(s_charge_tips_timer);
        s_charge_tips_timer = 0;
    }
}

LOCAL void HandleLauncherChargeTipTimerOut(uint8  timer_id, uint32 param)
{
    if(MMK_IsOpenWin(WATCH_LAUNCHER_CHARGETIP_WIN_ID))
    {
        MMK_CloseWin(WATCH_LAUNCHER_CHARGETIP_WIN_ID);
    }
}

LOCAL MMI_RESULT_E HandleLauncherChargeTipWinMsg(
                                      MMI_WIN_ID_T        win_id,    //IN:
                                      MMI_MESSAGE_ID_E    msg_id,    //IN:
                                      DPARAM              param      //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMK_CreateTimerCallback((LAUNCHER_CHARGE_TIP_DURATION+LAUNCHER_CHARGE_ANIM_DURATION), HandleLauncherChargeTipTimerOut, NULL, FALSE);
            StartChgTipsTimer();
            s_cur_index = 0;
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            StopChgTipsTimer();
            break;
        }

        case MSG_FULL_PAINT:
        {
            GUI_RECT_T win_rect = {0};
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            DrawChargeInfo(win_id);
            break;
        }

        case MSG_TIMER:
        {
            uint8 timer_id = *(uint8 *)param;
            if(TRUE == MMK_IsOpenWin(MMIUDISK_USB_OPER_SELECT_WIN_ID))
            {
                MMK_CloseWin(win_id);
            }
            else
            {
                if(s_charge_tips_timer == timer_id)
                 {
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
                    //refresh charging image id by timer.
                    if(s_cur_index < sizeof(s_charge_image_ids)/sizeof(MMI_IMAGE_ID_T)-1)
                    {
                        s_cur_index++;
                    }
                    else
                    {
                        s_cur_index = 0;
                    }
                 }
            }
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

WINDOW_TABLE(WATCH_LAUNCHER_CHARGE_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherChargeWinMsg ),
    WIN_ID(WATCH_LAUNCHER_CHARGE_WIN_ID ),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};

WINDOW_TABLE(WATCH_LAUNCHER_CHARGETIP_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherChargeTipWinMsg),
    WIN_ID(WATCH_LAUNCHER_CHARGETIP_WIN_ID ),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};

/**--------------------------------------------------------------------------*
**                         PUBLIC FUNCTION                                   *
**---------------------------------------------------------------------------*/

/*****************************************************************************
//  Description : open charge win
//  Author: weipeng.wang
//  Param:N/A
//  Return:
//  Note:
*****************************************************************************/
PUBLIC void WatchLAUNCHER_Charge_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_CHARGE_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CHARGE_WIN_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_CHARGE_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}

/*****************************************************************************
//  Description : open charge tip win
//  Author: weipeng.wang
//  Param:N/A
//  Return:
//  Note:
*****************************************************************************/
PUBLIC void WatchLAUNCHER_ChargeTip_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_CHARGETIP_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CHARGE_WIN_RECT;

    if(MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_CHARGETIP_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}

/*****************************************************************************
//  Description : open low battery tip win
//  Author: weipeng.wang
//  Param:N/A
//  Return:
//  Note:
*****************************************************************************/
PUBLIC void WatchLAUNCHER_BatteryLow_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_BATTERYLOW_WIN_ID;
    MMI_STRING_T text_str = {0};

    SCI_TRACE_LOW("WatchLAUNCHER_BatteryLow_Enter: Enter"); 
    MMI_GetLabelTextByLang(TXT_LOW_BATTERY_WARNING,&text_str);
    Adult_WatchCOM_NoteWin_1Icon_1Str_Enter(win_id, &text_str, res_battery_ic_low, PNULL);
}

