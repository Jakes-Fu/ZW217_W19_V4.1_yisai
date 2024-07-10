/*****************************************************************************
** File Name:      watch_charge_win.c                                           *
** Author:                                                                      *
** Date:           03/16/2020                                                   *
** Copyright:      2020 Spreadtrum, Incoporated. All Rights Reserved.         ..*
** Description:    watch charge win
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/16/2020  zhikun.lv        Create
******************************************************************************/

#include "mmi_default.h"

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
#include "mmiphone_charge.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#ifndef RGB888_TO_RGB565
#define RGB888_TO_RGB565(_r, _g, _b)  (((((uint32)_r )<< 8) & 0xf800) |((((uint32)_g )<< 3) & 0x07e0) |((((uint32)_b) >> 3) & 0x1f))
#endif

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/
//charge update timer
LOCAL uint8 s_charge_update_timer = 0;

#ifdef SCREEN_SHAPE_CIRCULAR
LOCAL uint8 s_charge_tips_timer = 0;
LOCAL uint8 s_cur_anim_index = 0;
LOCAL MMI_IMAGE_ID_T s_charge_amin_ids[] =
{
    res_battery_ic_charge_10,
    res_battery_ic_charge_20,
    res_battery_ic_charge_30,
    res_battery_ic_charge_40,
    res_battery_ic_charge_50,
    res_battery_ic_charge_60,
    res_battery_ic_charge_70,
    res_battery_ic_charge_80,
    res_battery_ic_charge_90,
    res_battery_ic_charge_100,
};
#endif
LOCAL uint8 s_charge_tips_during_timer = 0;

/**--------------------------------------------------------------------------*
**                         LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/
LOCAL uint32 GetCurrentChgValue()
{
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
    return p_chgmng_info->bat_remain_cap;
}

//judge is battery is full
LOCAL BOOLEAN isChargeFull()
{
    return (GetCurrentChgValue() >= BATTERY_CHARGE_FULL_CAP);
}

//start timer for charing update
LOCAL void StartChgTimer()
{
    if (s_charge_update_timer == 0)
    {
        s_charge_update_timer = MMK_CreateWinTimer(WATCH_LAUNCHER_CHARGE_WIN_ID, 500, TRUE);
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

#ifdef SCREEN_SHAPE_CIRCULAR
//start timer for charing tips
LOCAL void StartChgTipsTimer()
{
    if (s_charge_tips_timer == 0)
    {
        s_charge_tips_timer = MMK_CreateWinTimer(WATCH_LAUNCHER_CHARGETIP_WIN_ID, 500, TRUE);
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

LOCAL void DrawChgValue(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T rect = LAUNCHER_CHARGE_VALUE_RECT;
    wchar wStrBattery[16] = {0};
    char strBattery[16] = {0};
    MMI_STRING_T textStr ={0};
    GUISTR_STYLE_T text_style = {0};
    GUISTR_STATE_T state = {0};
    uint32 value = GetCurrentChgValue();
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T win_rect = {0};

    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    MMK_GetWinRect(win_id, &win_rect);
    rect.left += win_rect.left;
    rect.top += win_rect.top;
    rect.right += win_rect.left;
    rect.bottom += win_rect.top;

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
    GUI_RECT_T anim_rect = LAUNCHER_CHARGE_ANIM_RECT;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T win_rect = {0};

    MMK_GetWinRect(win_id, &win_rect);

    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    GUI_FillRect(&lcd_dev_info, win_rect, LAUNCHER_CHARGE_BACKGROUND_COLOR);

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
                       s_charge_amin_ids[s_cur_anim_index],
                       &lcd_dev_info);
    }
    else
    {
        GUIRES_DisplayImg(PNULL,
                       &anim_rect,
                       PNULL,
                       win_id,
                       res_battery_ic_charge_100,
                       &lcd_dev_info);
    }

    //draw 100% text
#ifdef POWER_OFF_CHARGE_TEXT_SHOW
    DrawChgValue(win_id);
#endif
}
#else

//create label.which show "charging" or "charged full"
LOCAL void CreateChgLabel(MMI_CTRL_ID_T ctrl_id, BOOLEAN b_init)
{
    GUI_RECT_T rect = LAUNCHER_CHARGE_STATE_RECT;
    MMI_STRING_T text = {0};
    if (!isChargeFull())
    {
        MMI_GetLabelTextByLang(TXT_CHARGE_STATE, &text);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_CHARGE_FINISH, &text);
    }

    if (b_init)
    {
        GUILABEL_SetRect(ctrl_id, &rect, TRUE);
    }
    GUILABEL_SetText(ctrl_id, &text, TRUE);
    GUILABEL_SetFont(ctrl_id, LAUNCHER_CHARGE_DEFAULT_FONT_SIZE, MMI_WHITE_COLOR);
}

//label for display current battery value
LOCAL void CreateChgValue(MMI_CTRL_ID_T ctrl_id, BOOLEAN b_init)
{
    GUI_RECT_T rect = LAUNCHER_CHARGE_VALUE_RECT;
    MMI_STRING_T text = {0};
    wchar wStrBattery[16] = {0};
    char strBattery[16] = {0};
    uint16 color = RGB888_TO_RGB565(0x25, 0xa8, 0x00);
    GUI_BG_T bg = {0};
    uint32 value = GetCurrentChgValue();
    bg.bg_type = GUI_BG_COLOR;
    bg.color = MMI_BLACK_COLOR;

    sprintf(strBattery, "%d", value);
    MMIAPICOM_StrToWstr(strBattery, wStrBattery);
    text.wstr_ptr = wStrBattery;
    text.wstr_len = MMIAPICOM_Wstrlen(wStrBattery);

    if (b_init)
    {
        GUILABEL_SetRect(ctrl_id, &rect, TRUE);
        GUILABEL_SetFont(ctrl_id, LAUNCHER_CHARGE_DEFAULT_FONT_SIZE, color);
        GUILABEL_SetBg(ctrl_id, &bg);
    }
    else
    {
        CTRLLABEL_FlushBg(ctrl_id);
    }

    GUILABEL_SetText(ctrl_id, &text, TRUE);
}

//display charging animation
LOCAL void CreateChgAnim(MMI_CTRL_ID_T ctrl_id)
{
    GUI_RECT_T rect = LAUNCHER_CHARGE_IMAGE_RECT;
    GUIANIM_CTRL_INFO_T control_info = {0};
    GUIANIM_DATA_INFO_T data_info = {0};
    GUIANIM_DISPLAY_INFO_T display_info = {0};
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = ctrl_id;
    data_info.anim_id = WATCH_INDICATOR_ANIM;

    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.bg.bg_type = GUI_BG_COLOR;
    display_info.bg.color = LAUNCHER_CHARGE_BACKGROUND_COLOR;
    CTRLANIM_SetCtrlRect(ctrl_id, &rect);
    GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
    GUIANIM_SetVisible(ctrl_id, TRUE, TRUE);
    GUIANIM_PlayAnim(ctrl_id);
}

//display charged full image
LOCAL void CreateChgAnimFull(MMI_CTRL_ID_T ctrl_id)
{
    GUI_RECT_T charge_image_rect = LAUNCHER_CHARGE_IMAGE_RECT;
    GUI_BG_T bg = {0};
    bg.bg_type = GUI_BG_COLOR;
    bg.color = MMI_BLACK_COLOR;

    GUILABEL_SetRect(ctrl_id, &charge_image_rect,TRUE);
    GUILABEL_SetIcon(ctrl_id, res_battery_ic_charge_100);
    GUILABEL_SetBg(ctrl_id, &bg);
    GUILABEL_SetVisible(ctrl_id, FALSE, TRUE);
}

//acording to current battery value, update animation area
LOCAL void UpdateAnimArea(MMI_CTRL_ID_T ctrl_id, MMI_CTRL_ID_T ctrl_id2)
{
    if (GetCurrentChgValue() >= BATTERY_CHARGE_FULL_CAP) //·ÀÖ¹Ò»Ö±³ä²»Âú
    {
        GUIANIM_SetVisible(ctrl_id, FALSE, FALSE);
        GUILABEL_SetVisible(ctrl_id2, TRUE, FALSE);
    }
    else
    {
        GUIANIM_SetVisible(ctrl_id, TRUE, FALSE);
        GUILABEL_SetVisible(ctrl_id2, FALSE, FALSE);
    }
}

//update all controls
LOCAL void UpdateAll(MMI_WIN_ID_T win_id)
{
    CreateChgLabel(LAUNCHER_CHARGE_STATE_CTRL_ID, FALSE);
    #ifdef POWER_OFF_CHARGE_TEXT_SHOW
    CreateChgValue(LAUNCHER_CHARGE_VALUE_CTRL_ID, FALSE);
    #endif
    UpdateAnimArea(LAUNCHER_CHARGE_IMAGE_CTRL_ID, LAUNCHER_CHARGE_IMAGE_FULL_CTRL_ID);
    MMK_SendMsg(win_id, MSG_FULL_PAINT, 0);
}
#endif

//msg handler for charging win
LOCAL MMI_RESULT_E HandleLauncherChargeWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,    //IN:
                                      DPARAM            param                //IN:
                                      )
{
    LOCAL uint32 last_charge_val = 0;
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    PHONE_STARTUP_CONDITION_E restart_condition = STARTUP_NONE;
    restart_condition = MMIAPIPHONE_GetStartUpCondition();
    SCI_TRACE_LOW("HandleLauncherChargeWinMsg() restart_condition = %d.", restart_condition);

    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            last_charge_val = GetCurrentChgValue();
#ifdef SCREEN_SHAPE_CIRCULAR
            s_cur_anim_index = 0;
#else
            CreateChgLabel(LAUNCHER_CHARGE_STATE_CTRL_ID, TRUE);
            #ifdef POWER_OFF_CHARGE_TEXT_SHOW
            CreateChgValue(LAUNCHER_CHARGE_VALUE_CTRL_ID, TRUE);
            #endif
            CreateChgAnim(LAUNCHER_CHARGE_IMAGE_CTRL_ID);
            CreateChgAnimFull(LAUNCHER_CHARGE_IMAGE_FULL_CTRL_ID);
            UpdateAnimArea(LAUNCHER_CHARGE_IMAGE_CTRL_ID, LAUNCHER_CHARGE_IMAGE_FULL_CTRL_ID);
#endif
            StartChgTimer();
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            StopChgTimer();
            break;
        }

#ifndef SCREEN_SHAPE_CIRCULAR
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
#ifdef SCREEN_SHAPE_CIRCULAR
            DrawChargeInfo(win_id);
#else
            if (restart_condition == STARTUP_NORMAL)
            {
                GUI_RECT_T win_rect = {0};
                GUI_LCD_DEV_INFO lcd_dev_info = {0};
                MMK_GetWinRect(win_id, &win_rect);
                MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
                GUI_FillRect(&lcd_dev_info, win_rect, LAUNCHER_CHARGE_BACKGROUND_COLOR);
            }
            else if (restart_condition == STARTUP_CHARGE)
            {
                GUI_RECT_T win_rect = {0};
                GUI_LCD_DEV_INFO lcd_dev_info = {0};
                uint16 lcd_width = 0;
                uint16 lcd_height = 0;
                GUILCD_GetLogicWidthHeight(0, &lcd_width, &lcd_height);
                win_rect.right = lcd_width - 1;
                win_rect.bottom = lcd_height - 1;
                GUI_FillRect(&lcd_dev_info, win_rect, LAUNCHER_CHARGE_BACKGROUND_COLOR);
            }
#endif
            break;
        }

        case MSG_GET_FOCUS:
        {
            StartChgTimer();
#ifdef SCREEN_SHAPE_CIRCULAR
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, PNULL);
#else
            MMK_SendMsg(LAUNCHER_CHARGE_IMAGE_CTRL_ID, MSG_CTL_GET_FOCUS, NULL);
            GUIANIM_PlayAnim(LAUNCHER_CHARGE_IMAGE_CTRL_ID);
#endif
            break;
        }

        case MSG_LOSE_FOCUS:
        {
            if (restart_condition != STARTUP_CHARGE)
            {
                StopChgTimer();
#ifndef SCREEN_SHAPE_CIRCULAR
                GUIANIM_PauseAnim(LAUNCHER_CHARGE_IMAGE_CTRL_ID);
#endif
            }

            break;
        }
#ifdef SCREEN_SHAPE_CIRCULAR
        case MSG_KEYDOWN_RED:
        {
            SCI_TRACE_LOW("[HandleLauncherChargeWinMsg]:receive MSG_KEYDOWN_RED");
            if(TRUE == MMIDEFAULT_IsBacklightOn())
            {
                MMIDEFAULT_TurnOffBackLight();
                MMIDEFAULT_CloseAllLight_Watch();//close LCD
            }else
            {
                MMIDEFAULT_TurnOnBackLight();
            }
            break;
        }
#endif
        case MSG_TIMER:
        {
            uint8 timer_id = *(uint8 *)param;
#ifdef SCREEN_SHAPE_CIRCULAR
            if (s_charge_update_timer == timer_id && MMK_IsFocusWin(win_id))
            {
                //refresh charging image id by timer.
                if(s_cur_anim_index < sizeof(s_charge_amin_ids)/sizeof(MMI_IMAGE_ID_T)-1)
                {
                    s_cur_anim_index++;
                }
                else
                {
                    s_cur_anim_index = 0;
                }

                if((FALSE == isChargeFull()) || (100 != last_charge_val))
                {
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
                }
                last_charge_val = GetCurrentChgValue();


            }

#else
            if (s_charge_update_timer == timer_id)
            {
                if (last_charge_val != BATTERY_CHARGE_FULL_CAP && GetCurrentChgValue() >= BATTERY_CHARGE_FULL_CAP)
                {
                    SCI_TRACE_LOW("HandleLauncherChargeWinMsg() nofull to full!.");
                    UpdateAll(win_id);
                }
                else if (last_charge_val == BATTERY_CHARGE_FULL_CAP && GetCurrentChgValue() <= BATTERY_CHARGE_FULL_CAP)
                {
                    SCI_TRACE_LOW("HandleLauncherChargeWinMsg() full to notfull!.");
                    UpdateAll(win_id);
                }
                else
                {
                    #ifdef POWER_OFF_CHARGE_TEXT_SHOW
                    CreateChgValue(LAUNCHER_CHARGE_VALUE_CTRL_ID, FALSE);
                    MMK_SendMsg(LAUNCHER_CHARGE_VALUE_CTRL_ID, MSG_CTL_PAINT, 0);
                    #endif
                }

                last_charge_val = GetCurrentChgValue();
            }
#endif
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    //process default msg
    if (!recode)
    {
        if (restart_condition == STARTUP_NORMAL)
        {
            recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
        }
        else if (restart_condition == STARTUP_CHARGE)
        {
            if (msg_id == MSG_KEYDOWN_8 || msg_id == MSG_APP_RED)
            {
                 recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
            }
        }
    }

    MMI_CheckAllocatedMemInfo();
    return recode;
}

LOCAL void HandleLauncherChargeTipTimerOut(
                               uint8  timer_id,
                               uint32 param
                               )
{
    WatchLAUNCHER_ChargeTip_Exist();
}

LOCAL MMI_RESULT_E HandleLauncherChargeTipWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,    //IN:
                                      DPARAM            param                //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
#ifdef SCREEN_SHAPE_CIRCULAR
            StartChgTipsTimer();
            s_cur_anim_index = 0;
#else
            CreateChgLabel(LAUNCHER_CHARGETIP_STATE_CTRL_ID, TRUE);
            CreateChgValue(LAUNCHER_CHARGETIP_VALUE_CTRL_ID, TRUE);
            CreateChgAnim(LAUNCHER_CHARGETIP_IMAGE_CTRL_ID);
            CreateChgAnimFull(LAUNCHER_CHARGETIP_IMAGE_FULL_CTRL_ID);
            UpdateAnimArea(LAUNCHER_CHARGETIP_IMAGE_CTRL_ID,LAUNCHER_CHARGETIP_IMAGE_FULL_CTRL_ID);
#endif
            if (0 == s_charge_tips_during_timer)
            {
                s_charge_tips_during_timer = MMK_CreateTimerCallback(LAUNCHER_CHARGE_TIP_DURATION, HandleLauncherChargeTipTimerOut, NULL, FALSE);
            }
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
#ifdef SCREEN_SHAPE_CIRCULAR
            StopChgTipsTimer();
            if (0 != s_charge_tips_during_timer)
            {
                MMK_StopTimer(s_charge_tips_during_timer);
                s_charge_tips_during_timer = 0;
            }
#endif
            break;
        }

        case MSG_FULL_PAINT:
        {
#ifdef SCREEN_SHAPE_CIRCULAR
            DrawChargeInfo(win_id);
#else
            GUI_RECT_T win_rect = {0};
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            GUI_FillRect(&lcd_dev_info, win_rect, LAUNCHER_CHARGE_BACKGROUND_COLOR);
#endif
            break;
        }

#ifdef SCREEN_SHAPE_CIRCULAR
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
                    if(s_cur_anim_index < sizeof(s_charge_amin_ids)/sizeof(MMI_IMAGE_ID_T)-1)
                    {
                        s_cur_anim_index++;
                    }
                    else
                    {
                        s_cur_anim_index = 0;
                    }
                }
            }
            break;
        }
#endif
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();
    return recode;
}

LOCAL void HandleLauncherBatteryLowTipTimerOut( 
                               uint8  timer_id,
                               uint32 param
                               )
{
    if (MMK_IsOpenWin(WATCH_LAUNCHER_BATTERYLOW_WIN_ID))
    {
        MMK_CloseWin(WATCH_LAUNCHER_BATTERYLOW_WIN_ID);
    }
}

LOCAL MMI_RESULT_E HandleLauncherBatteryLowWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMI_STRING_T text = {0};
            MMI_STRING_T comfirm_text = {0};
            GUI_RECT_T win_rect = {0};
            GUI_RECT_T image_rect = LAUNCHER_BATTERYLOW_IMAGE_RECT;
            GUI_RECT_T tip_rect = LAUNCHER_BATTERYLOW_TIP_RECT;
            GUI_RECT_T confirm_rect = LAUNCHER_BATTERYLOW_CONFIRM_RECT;
            GUI_LCD_DEV_INFO lcd_dev_info = {0};

            GUI_FONT_ALL_T font_type = {LAUNCHER_CHARGE_DEFAULT_FONT_SIZE, MMI_WHITE_COLOR};
            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            MMI_GetLabelTextByLang(TXT_LOW_BATTERY, &text);
            MMI_GetLabelTextByLang(TXT_LOW_BATTERY_CONFIRM, &comfirm_text);

            GUILABEL_SetRect(LAUNCHER_BATTERYLOW_IMAGE_CTRL_ID, &image_rect, TRUE);
            GUILABEL_SetIcon(LAUNCHER_BATTERYLOW_IMAGE_CTRL_ID, res_battery_ic_low);

            GUILABEL_SetRect(LAUNCHER_BATTERYLOW_TIP_CTRL_ID, &tip_rect, TRUE);
            GUILABEL_SetText(LAUNCHER_BATTERYLOW_TIP_CTRL_ID, &text, TRUE);

            GUILABEL_SetFont(LAUNCHER_BATTERYLOW_TIP_CTRL_ID, LAUNCHER_CHARGE_DEFAULT_FONT_SIZE, MMI_WHITE_COLOR);

#ifdef SCREEN_SHAPE_CIRCULAR
            MMK_CreateTimerCallback(LAUNCHER_CHARGE_TIP_DURATION, HandleLauncherBatteryLowTipTimerOut, NULL, FALSE);
#else
            GUIBUTTON_SetRect(LAUNCHER_BATTERYLOW_CONFIRM_CTRL_ID, &confirm_rect);
            GUIBUTTON_SetText(LAUNCHER_BATTERYLOW_CONFIRM_CTRL_ID, comfirm_text.wstr_ptr, comfirm_text.wstr_len);
            GUIBUTTON_SetFont(LAUNCHER_BATTERYLOW_CONFIRM_CTRL_ID, &font_type);

            MMK_SetActiveCtrl(LAUNCHER_BATTERYLOW_CONFIRM_CTRL_ID, TRUE);
#endif
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            break;
        }

        case MSG_APP_OK:
        case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT 
        case MSG_NOTIFY_PENOK:
#endif
        case MSG_NOTIFY_MIDSK:
        {
            MMK_CloseWin(win_id);
            break;
        }

        case MSG_FULL_PAINT:
        {
            GUI_RECT_T win_rect = {0};
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            GUI_FillRect(&lcd_dev_info, win_rect, LAUNCHER_CHARGE_BACKGROUND_COLOR);
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();
    return recode;
}

WINDOW_TABLE(WATCH_LAUNCHER_CHARGE_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherChargeWinMsg ),
    WIN_ID(WATCH_LAUNCHER_CHARGE_WIN_ID ),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
#ifndef SCREEN_SHAPE_CIRCULAR
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, LAUNCHER_CHARGE_STATE_CTRL_ID),
#ifdef POWER_OFF_CHARGE_TEXT_SHOW
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, LAUNCHER_CHARGE_VALUE_CTRL_ID),
#endif
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, LAUNCHER_CHARGE_IMAGE_FULL_CTRL_ID),
    CREATE_ANIM_CTRL(LAUNCHER_CHARGE_IMAGE_CTRL_ID, WATCH_LAUNCHER_CHARGE_WIN_ID),
#endif
    END_WIN
};

WINDOW_TABLE(WATCH_LAUNCHER_CHARGETIP_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherChargeTipWinMsg),
    WIN_ID(WATCH_LAUNCHER_CHARGETIP_WIN_ID ),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
#ifndef SCREEN_SHAPE_CIRCULAR
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, LAUNCHER_CHARGETIP_STATE_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, LAUNCHER_CHARGETIP_VALUE_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, LAUNCHER_CHARGETIP_IMAGE_FULL_CTRL_ID),
    CREATE_ANIM_CTRL(LAUNCHER_CHARGETIP_IMAGE_CTRL_ID, WATCH_LAUNCHER_CHARGETIP_WIN_ID),
#endif
    END_WIN
};

WINDOW_TABLE(WATCH_LAUNCHER_BATTERYLOW_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherBatteryLowWinMsg),
    WIN_ID(WATCH_LAUNCHER_BATTERYLOW_WIN_ID ),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, LAUNCHER_BATTERYLOW_IMAGE_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, LAUNCHER_BATTERYLOW_TIP_CTRL_ID),
#ifndef SCREEN_SHAPE_CIRCULAR
    CREATE_BUTTON_CTRL(res_sys_bg_lowbattery, LAUNCHER_BATTERYLOW_CONFIRM_CTRL_ID),
#endif
    END_WIN
};


/**--------------------------------------------------------------------------*
**                         PUBLIC FUNCTION                                   *
**---------------------------------------------------------------------------*/

/*****************************************************************************
//  Description : open charge win
//  Author: zhikun.lv
//  Param:N/A
//  Return:
//  Note:
*****************************************************************************/
PUBLIC void WatchLAUNCHER_Charge_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_CHARGE_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_CHARGE_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}


/*****************************************************************************
//  Description : open charge tip win
//  Author: zhikun.lv
//  Param:N/A
//  Return:
//  Note:
*****************************************************************************/
PUBLIC void WatchLAUNCHER_ChargeTip_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_CHARGETIP_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_CHARGETIP_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}

/*****************************************************************************
//  Description : close charge tip win
//  Author:
//  Param:N/A
//  Return:
//  Note:
*****************************************************************************/
PUBLIC void WatchLAUNCHER_ChargeTip_Exist()
{
    if (MMK_IsOpenWin(WATCH_LAUNCHER_CHARGETIP_WIN_ID))
    {
        MMK_CloseWin(WATCH_LAUNCHER_CHARGETIP_WIN_ID);
    }
}

/*****************************************************************************
//  Description : open low battery tip win
//  Author: zhikun.lv
//  Param:N/A
//  Return:
//  Note:
*****************************************************************************/
PUBLIC void WatchLAUNCHER_BatteryLow_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_BATTERYLOW_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_BATTERYLOW_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}
