/*****************************************************************************
** File Name:      watch_launcher_main.c                                                                          *
** Author:                                                                                                                     *
** Date:             02/03/2020                                                                                          *
** Copyright:      2020 Spreadtrum, Incoporated. All Rights Reserved.                                   *
** Description:    Watch Launcher Win
******************************************************************************
**                         Important Edit History                                                                         *
** --------------------------------------------------------------------------*
** DATE            NAME             DESCRIPTION                                                                    *
** 02/03/2020  zhikun.lv        Create
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_timer.h"
#include "guistring.h"
#include "ui_layer.h"
#include "graphics_draw.h"

#include "watch_slidepage.h"
#include "watch_launcher_main.h"

#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif

#include "watch_pb_view.h"
#include "watch_stopwatch.h"
#include "watch_gallery_export.h"
#include "watch_charge_win.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_main.h"
#endif
#include "watch_launcher_editclockwin.h"
#include "watch_sms_main.h"
#include "mmicc_export.h" //for dialpad
#include "mmicom_trace.h"
#include "mmi_default.h"
#ifdef DROPDOWN_NOTIFY_SHORTCUT
#include "watch_notifycenter.h"
#endif
#ifdef ADULT_WATCH_SUPPORT
#include "watch_steps.h"
#include "watch_heartrate.h"
#include "watch_sms_message.h"
#include "mmieng_uitestwin.h"

#endif
#ifdef DYNAMIC_WATCHFACE_SUPPORT
#include "watchface_view.h"
#endif

#include "watch_extra_export.h"

#ifdef BAIDU_DRIVE_SUPPORT
#include "mmibaidu_export.h"
#endif

#ifdef TULING_AI_SUPPORT //图灵机器人
extern void MMIAPIMENU_EnterAiChat(void);
#endif

#ifdef ZDT_ZFB_SUPPORT
extern void MMIZFB_OpenMainWin(void);
#endif

#ifdef LEBAO_MUSIC_SUPPORT //乐宝音乐
#include "mmi_lebao.h"
#endif

#if defined(ZDT_VIDEOCHAT_SUPPORT) && defined(VIDEO_CALL_AGORA_SUPPORT) //声网视频通话
#include "video_call_text.h"
#include "video_call_export.h"
#endif

#include "Mmidc_export.h"
#include "mmicalc_export.h"
#include "watch_alarm_main.h"
#include "zdt_win_export.h"
#include "zdt_app.h"
#include "watch_set_alarmlist.h"
#ifdef SETUP_WIZARD_SUPPORT
#include "setup_wizard.h"
#endif

#include "mmieng_win.h"
#include "watch_common.h"

#include "guiiconlist.h"

#ifdef ZTE_MENU_SUPPORT
#include "zte_menu_win.h"
#include "zte_menu_id.h"
#endif


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#ifndef TRACE_APP_LAUNCHER
#define TRACE_APP_LAUNCHER
#endif

#define WATCH_LAUNCHER_CLOCK_UPDATE_INTERVAL 500

#define WATCH_LAUNCHER_CLOCK_DIGITAL_UPDATE_INTERVAL (30 * 1000)// W307 notify update
#ifdef DROPDOWN_NOTIFY_SHORTCUT
#define WATCH_LAUNCHER_NOTIFY_TIMER_INTERVAL 1000
#endif
#define WATCH_LAUNCHER_INDICATOR_OFFSET 18

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/
//handle of slidepage
LOCAL MMI_HANDLE_T s_handle = 0;

//type of clock page win

//clock update timer
LOCAL uint8 s_clock_update_timer = 0;
#ifdef DROPDOWN_NOTIFY_SHORTCUT
LOCAL uint8 s_clock_notify_timer = 0;
#endif
#ifdef ENG_SUPPORT
LOCAL uint8 s_eng_tp_down_count = 0;
#endif

LOCAL GUI_LCD_DEV_INFO s_indi_layer = {0};

LOCAL uint8 s_current_menu_style = 0;

/**--------------------------------------------------------------------------*
**                         LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/
LOCAL BOOLEAN IsPressedStatusBarRect(GUI_POINT_T point);

//diplay background
LOCAL void DisplayWinPanelBg(MMI_WIN_ID_T win_id);

//draw foreground base
LOCAL void DisplayWinPanelFgBase(MMI_WIN_ID_T win_id,
                                               MMI_IMAGE_ID_T image_id,
                                               GUI_RECT_T client_rect,
                                               MMI_TEXT_ID_T title_id,
                                               GUI_RECT_T title_rect);

//handle clock win message
LOCAL MMI_RESULT_E HandleLauncherClockWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,    //IN:
                                      DPARAM            param                //IN:
                                      );

//handle call win message
LOCAL MMI_RESULT_E HandleLauncherCallWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

#ifdef STOPWATCH_SUPPORT
//handle stopwatch win message
LOCAL MMI_RESULT_E HandleLauncherStopWatchWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );
#endif

//handle camera win message
LOCAL MMI_RESULT_E HandleLauncherCameraWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

//handle gallery win message
LOCAL MMI_RESULT_E HandleLauncherGalleryWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

#ifdef MMI_RECORD_SUPPORT
//handle voicerecord win message
LOCAL MMI_RESULT_E HandleLauncherVoiceRecordWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );
#endif

//handle sms win message
LOCAL MMI_RESULT_E HandleLauncherSmsWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

////handle setting win message
LOCAL MMI_RESULT_E HandleLauncherSettingWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

#if defined(LEBAO_MUSIC_SUPPORT)// wuxx add
LOCAL MMI_RESULT_E HandleLauncherLebaoWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );
#endif

#ifndef ADULT_WATCH_SUPPORT
#if defined(JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT) || defined(APP_STORE_SUPPORT) || defined(DYNAMIC_WATCHFACE_SUPPORT)
LOCAL MMI_RESULT_E HandleLauncherExtraWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );
#endif
#endif

#ifdef ADULT_WATCH_SUPPORT
//handle sport win message
LOCAL MMI_RESULT_E HandleLauncherSportWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

//handle moving object win message
LOCAL MMI_RESULT_E HandleLauncherMovingObjectWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

//handle heart rate win message
LOCAL MMI_RESULT_E HandleLauncherHeartRateWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

//handle pay win message
LOCAL MMI_RESULT_E HandleLauncherPayWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );
#endif

PUBLIC void MMIZDT_DropUp_EnterWin(eSlideWinStartUpMode launch_mode);

/*****************************************************************/
//judge if point is contained by status bar rect
LOCAL BOOLEAN IsPressedStatusBarRect(GUI_POINT_T point)
{
    GUI_RECT_T status_bar_rect = LAUNCHER_NOTIFY_AREA;
    if (GUI_PointIsInRect(point, status_bar_rect))
    {
        return TRUE;
    }
    return FALSE;
}

LOCAL BOOLEAN IsPressedSoftkeyBarRect(GUI_POINT_T point)
{
    GUI_RECT_T softkey_rect = LAUNCHER_SOFTKEY_AREA;
    if (GUI_PointIsInRect(point, softkey_rect))
    {
        return TRUE;
    }
    return FALSE;
}
//diplay background
LOCAL void DisplayWinPanelBg(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T clientRect = LAUNCHER_CLIENT_RECT;
    GUI_RECT_T win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    win_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, clientRect);
    GUI_FillRect(&lcd_dev_info, win_rect, MMI_BLACK_COLOR);
}
LOCAL void DisplayWinPanelFgAddUnreadInfoNum(MMI_WIN_ID_T win_id,uint8 unread_num)
{
    GUI_RECT_T         disp_rect           = {0};
    GUI_RECT_T         max_rect            = LAUNCHER_NOTIFY_UNREAD_MESSAGE_MAX_RECT;
    GUI_RECT_T         normal_rect         = LAUNCHER_NOTIFY_UNREAD_MESSAGE_RECT;
    GUI_LCD_DEV_INFO   lcd_dev_info        = {0};
    char               str_unread_num[LAUNCHER_NOTIFICATION_NUM_MAX_LEN + 1] = {0};
    uint8              str_unread_num_len  = 0;
    wchar              temp[LAUNCHER_NOTIFICATION_NUM_MAX_LEN + 1]           = {0};
    MMI_STRING_T       dis_num_str         = {0};
    MMI_IMAGE_ID_T     image_id            = NULL;
    if(unread_num == 0)
    {
         return;
    }
    else if(unread_num > LAUNCHER_NOTIFICATION_MAX_NUM)
    {
        unread_num = LAUNCHER_NOTIFICATION_MAX_NUM;
        disp_rect  = max_rect;
        image_id   = res_launcher_notification_2;
        sprintf(str_unread_num, "%d+", unread_num);
    }
    else
    {
        disp_rect = normal_rect;
        image_id  = res_launcher_notification_1;
        sprintf(str_unread_num, "%d", unread_num);
    }
    str_unread_num_len = strlen(str_unread_num);
    MMI_STRNTOWSTR(temp, LAUNCHER_NOTIFICATION_NUM_MAX_LEN + 1, (const uint8 *)str_unread_num , str_unread_num_len,  str_unread_num_len);
    dis_num_str.wstr_ptr = temp;
    dis_num_str.wstr_len = str_unread_num_len;
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);

    //Draw unread  num txt
    do
    {
        GUISTR_STYLE_T      text_style = {0};
        GUISTR_STATE_T      text_state = GUISTR_STATE_ALIGN;
        text_style.align      = ALIGN_HVMIDDLE;
        text_style.font       = WATCH_DEFAULT_SMALL_FONT;
        text_style.font_color = MMI_WHITE_COLOR;

        //转换窗口坐标到显示坐标
        disp_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, disp_rect);
        GUIRES_DisplayImg(PNULL,
                           &disp_rect,
                           PNULL,
                           win_id,
                           image_id,
                           &lcd_dev_info
                           );
        GUISTR_DrawTextToLCDInRect(
                          (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                          &disp_rect,
                          &disp_rect,
                          &dis_num_str,
                          &text_style,
                          text_state,
                          GUISTR_TEXT_DIR_AUTO
                          );
    }while (0);
}

//draw foreground
LOCAL void DisplayWinPanelFgBase(MMI_WIN_ID_T win_id,
                                               MMI_IMAGE_ID_T image_id,
                                               GUI_RECT_T client_rect,
                                               MMI_TEXT_ID_T title_id,
                                               GUI_RECT_T title_rect)
{
    uint16 img_width = 0;
    uint16 img_height = 0;
    GUI_RECT_T img_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);
    GUIRES_GetImgWidthHeight(&img_width, &img_height,  image_id, win_id);

    img_rect = GUI_GetCenterRect(client_rect, img_width, img_height);
    GUIRES_DisplayImg(PNULL,
                       &img_rect,
                       PNULL,
                       win_id,
                       image_id,
                       &lcd_dev_info
                       );

    //Draw title txt
    do
    {
        MMI_STRING_T widgetStr = {0};
        GUI_RECT_T disp_rect = LAUNCHER_TITLE_RECT;
        GUISTR_STYLE_T      text_style = {0};
        GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN;
        text_style.align    = ALIGN_HVMIDDLE;
        text_style.font     = DP_FONT_20;
        text_style.font_color= MMI_WHITE_COLOR;

        //转换窗口坐标到显示坐标
        disp_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, disp_rect);
        MMI_GetLabelTextByLang(title_id, &widgetStr);
        GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &disp_rect,
        &disp_rect,
        &widgetStr,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
        );
    }while (0);
}

LOCAL void CreateIndicatorLayer(MMI_WIN_ID_T win_id,
                                                                         uint32 width,
                                                                         uint32 height,
                                                                         GUI_LCD_DEV_INFO *lcd_dev_ptr)
{
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_CREATE_T create_info = {0};
    if (PNULL == lcd_dev_ptr)
    {
        return;
    }

    lcd_dev_ptr->lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_ptr->block_id = UILAYER_NULL_HANDLE;

    // 创建层
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = win_id;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = width;
    create_info.height = height;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = TRUE;

    UILAYER_CreateLayer(
        &create_info,
        lcd_dev_ptr
        );

    UILAYER_SetLayerColorKey(lcd_dev_ptr, TRUE, UILAYER_TRANSPARENT_COLOR);
    UILAYER_Clear(lcd_dev_ptr);
#endif
}


//process the common action of launcher
PUBLIC MMI_RESULT_E WatchLAUNCHER_HandleCommonWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    LOCAL BOOLEAN b_tp_pressed = FALSE;
    LOCAL BOOLEAN b_tp_moved = FALSE;
    LOCAL GUI_POINT_T tp_point = {0};
    LOCAL BOOLEAN b_drop_from_top = FALSE;
    LOCAL BOOLEAN b_drop_from_bottom = FALSE;
    MMI_CheckAllocatedMemInfo();
    TRACE_APP_LAUNCHER("win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);
    switch (msg_id)
    {
        case MSG_KEYLONG_HASH:
        {
            TRACE_APP_LAUNCHER("MSG_KEYLONG_HASH");
#ifdef PDA_UI_DROPDOWN_WIN
            //MMINotifyWin_EnterWin(SLIDEWIN_STARTUPMODE_AUTO);
            MMIZDT_DropDown_EnterWin(SLIDEWIN_STARTUPMODE_AUTO);
#endif
            break;
        }

#ifdef TOUCH_PANEL_SUPPORT
        case MSG_TP_PRESS_DOWN:
        {
            tp_point.x = MMK_GET_TP_X(param);
            tp_point.y = MMK_GET_TP_Y(param);
#ifndef DROPDOWN_NOTIFY_SHORTCUT
#ifdef PDA_UI_DROPDOWN_WIN
            if (IsPressedStatusBarRect(tp_point))
            {
                //StartUp DropWin
                TRACE_APP_LAUNCHER("IsPressedStatusBarRect() = true.");

                b_drop_from_top = TRUE;
                //MMINotifyWin_EnterWin(SLIDEWIN_STARTUPMODE_TP);
                //MMIZDT_DropDown_EnterWin(SLIDEWIN_STARTUPMODE_AUTO);
            }
            else if(IsPressedSoftkeyBarRect(tp_point))
            {
                b_drop_from_bottom = TRUE;
            }
#endif
#else
            if (IsPressedStatusBarRect(tp_point))
            {
                //StartUp DropWin
                TRACE_APP_LAUNCHER("IsPressedStatusBarRect() = true.");
#ifdef DYNAMIC_WATCHFACE_SUPPORT
                if(MMK_IsFocusWin(WATCHFACE_WIN_ID))
#else
                if(MMK_IsFocusWin(WATCH_LAUNCHER_CLOCK_WIN_ID))
#endif
                {
                    NotifyCenter_EnterShortcutWin();
                }
            }
            else if(IsPressedSoftkeyBarRect(tp_point))
 			{
#ifdef DYNAMIC_WATCHFACE_SUPPORT

                if(MMK_IsFocusWin(WATCHFACE_WIN_ID))
#else
                if(MMK_IsFocusWin(WATCH_LAUNCHER_CLOCK_WIN_ID))
#endif
                {
					NotifyCenter_EnterListWin();
                }
			}
#endif
            b_tp_pressed = TRUE;
            break;
        }

        case MSG_TP_PRESS_MOVE:
        {
            TRACE_APP_LAUNCHER("MSG_TP_PRESS_MOVE");
            b_tp_moved = TRUE;
            if(abs(MMK_GET_TP_Y(param) - tp_point.y) > 10)
            {
                if(b_drop_from_top)
                {
                    MMIZDT_DropDown_EnterWin(SLIDEWIN_STARTUPMODE_AUTO);
                    b_drop_from_top = FALSE;
                }
                else if(b_drop_from_bottom)
                {
				#ifdef ZTE_WATCH
                    MMIZDT_DropUp_EnterWin(SLIDEWIN_STARTUPMODE_AUTO);
				#endif
                    b_drop_from_bottom = FALSE;
                }
            }
            break;
        }

        case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T tp_up = {0};
            BOOLEAN bClick = FALSE;
            TRACE_APP_LAUNCHER("MSG_TP_PRESS_UP");
            tp_up.x = MMK_GET_TP_X(param);
            tp_up.y = MMK_GET_TP_Y(param);
            bClick = (abs(tp_up.x - tp_point.x) < 3) && (abs(tp_up.y - tp_point.y) < 3);
            if (b_tp_pressed && bClick)
            {
                //post tp click msg to win.
                //MMK_PostMsg(win_id, MSG_APP_WEB, NULL, NULL);
				MMK_PostMsg(win_id, MSG_APP_WEB, &tp_up, sizeof(GUI_POINT_T));
            }
            b_tp_pressed = FALSE;
            b_tp_moved = FALSE;
            b_drop_from_top = FALSE;
            b_drop_from_bottom = FALSE;
            break;
        }
#endif
        case MSG_KEYDOWN_0:
            if(!MMIENG_IsMonkeyTestMode())
            {
                MMIAPIMENU_CreatMainMenu();
            }
            break;
        case MSG_NOTIFY_OK:
        case MSG_APP_OK:
        {
            SCI_TRACE_LOW("WatchCC_DialpadWin_Enter");
            WatchCC_DialpadWin_Enter();
            break;
        }

        //case MSG_KEYUP_RED:
        //    WatchSLIDEAGE_SetCurrentPageIndex(1);
        //    break;

        case MSG_NOTIFY_CANCEL:
        case MSG_APP_CANCEL:
            if(!MMIENG_IsMonkeyTestMode())
            {
               // MMIAPIENG_StartEngineerMenu();
            }
            break;
        case MSG_KEYDOWN_8:
            // TODO:
#ifdef PRODUCT_CONFIG_UWS6131_XTC_I23
        case MSG_APP_RED:
            SCI_TRACE_LOW("WatchLAUNCHER_HandleCommonWinMsg() msg_id = %d.", msg_id);
#endif
            if(TRUE == MMIDEFAULT_IsBacklightOn())
            {
                MMIDEFAULT_TurnOffBackLight();
                MMIDEFAULT_CloseAllLight_Watch();//close LCD
            }else
            {
                MMIDEFAULT_TurnOnBackLight();
            }
            break;
        default:
            recode = MMI_RESULT_FALSE;
    }
    MMI_CheckAllocatedMemInfo();
    TRACE_APP_LAUNCHER("exit");;
    return recode;
}

PUBLIC MMI_RESULT_E WatchLAUNCHER_HandleCb(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    switch (msg_id)
    {
        case MSG_SLIDEPAGE_OPENED:
        {
#ifndef ADULT_WATCH_SUPPORT
#ifndef SCREEN_SHAPE_CIRCULAR
            GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
            CreateIndicatorLayer(win_id, 240, 240, &s_indi_layer);
            if (!UILAYER_IsBltLayer(&s_indi_layer))
            {
                UILAYER_APPEND_BLT_T append;
                append.layer_level = UILAYER_LEVEL_NORMAL;
                append.lcd_dev_info = s_indi_layer;
                UILAYER_AppendBltLayer(&append);
            }
#endif
#endif
            break;
        }

        case MSG_SLIDEPAGE_GETFOCUS:
        {
#ifndef ADULT_WATCH_SUPPORT
#ifndef SCREEN_SHAPE_CIRCULAR

            if (!UILAYER_IsBltLayer(&s_indi_layer))
            {
                UILAYER_APPEND_BLT_T append;
                append.layer_level = UILAYER_LEVEL_NORMAL;
                append.lcd_dev_info = s_indi_layer;
                UILAYER_AppendBltLayer(&append);
            }
#endif
#endif
            break;
        }

        case MSG_SLIDEPAGE_LOSEFOCUS:
        {
#ifndef ADULT_WATCH_SUPPORT
#ifndef SCREEN_SHAPE_CIRCULAR
            UILAYER_RemoveBltLayer(&s_indi_layer);
#endif
#endif
            break;
        }

        case MSG_SLIDEPAGE_PAGECHANGED:
        {
#ifndef ADULT_WATCH_SUPPORT
#ifndef SCREEN_SHAPE_CIRCULAR
            WatchLAUNCHER_DisplayIndicator(win_id);
#endif
#endif
            break;
        }
        case MSG_SLIDEPAGE_END:
        {
            MMK_SendMsg(win_id, MSG_SLIDEPAGE_END, PNULL);
            break;
        }
        default:
            break;
    }
    return recode;
}

#ifndef DYNAMIC_WATCHFACE_SUPPORT
//handle clock win message
LOCAL MMI_RESULT_E HandleLauncherClockWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,    //IN:
                                      DPARAM            param                //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
#ifdef ADULT_WATCH_SUPPORT
    LOCAL BOOLEAN bRedKeyDown = FALSE;
#endif
    TRACE_APP_LAUNCHER("win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);
    MMI_CheckAllocatedMemInfo();

    switch (msg_id)
    {
    #ifdef WIN32
        case MSG_APP_9:
            MMIAPIPHONE_StartUpPsAccordingToSetting(TRUE, TRUE);
            break;
    #endif
        case MSG_OPEN_WINDOW:
        {
            TRACE_APP_LAUNCHER("MSG_OPEN_WINDOW");
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            TRACE_APP_LAUNCHER("MSG_CLOSE_WINDOW");
            MMK_StopTimer(s_clock_update_timer);
            s_clock_update_timer = 0;
#ifdef DROPDOWN_NOTIFY_SHORTCUT
            MMK_StopTimer(s_clock_notify_timer);
            s_clock_notify_timer = 0;
#endif
            break;
        }

        case MSG_FULL_PAINT:
        {
            GUI_RECT_T win_rect = {0};
            GUI_RECT_T dis_rect = LAUNCHER_CLIENT_RECT;
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            TRACE_APP_LAUNCHER("MSG_FULL_PAINT");

            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            GUI_FillRect(&lcd_dev_info, win_rect, MMI_BLACK_COLOR);

            dis_rect.left += win_rect.left;
            dis_rect.right += win_rect.left;
            if (MMIAPISET_GetClockDisplayStyle() == MMISET_CLOCK_STYLE_ANALOG)
            {
                TRACE_APP_LAUNCHER("clockType= LAUNCHER_CLOCKTYPE_ANALOG.");
                WatchLAUNCHER_DrawAnalogClock(win_id, dis_rect);
            }
            else if (MMIAPISET_GetClockDisplayStyle() == MMISET_CLOCK_STYLE_DIGIT)
            {
                TRACE_APP_LAUNCHER("clockType= LAUNCHER_CLOCKTYPE_DIGITAL_12HOUR.");
                if (MMIAPISET_GetTimeDisplayType() == MMISET_TIME_12HOURS)
                {
                    WatchLAUNCHER_DrawDigitalClock(win_id, dis_rect, TRUE);
                }
                else
                {
                    WatchLAUNCHER_DrawDigitalClock(win_id, dis_rect, FALSE);
                }
            }
#ifdef ADULT_WATCH_SUPPORT
            else if (MMIAPISET_GetClockDisplayStyle() == MMISET_CLOCK_STYLE_MULITIFUNCTION)
            {
                TRACE_APP_LAUNCHER("clockType= MMISET_CLOCK_STYLE_MULITIFUNCTION.");
				WatchLAUNCHER_DrawMTFClock(win_id, dis_rect);
            }
#endif
#ifdef DROPDOWN_NOTIFY_SHORTCUT
            if(NotifyCenter_IsExist() == TRUE)
#else
            if(MMIAPISMS_GetAllUnreadMsgCount()>0 || MMIAPICC_GetMissedCallNumber() > 0)//1347205
#endif
            {
#ifndef DYNAMIC_WATCHFACE_SUPPORT
#ifdef ADULT_WATCH_SUPPORT
                GUI_RECT_T disp_rect = LAUNCHER_CLOCK_NOTIFICATION_RECT;
                disp_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, disp_rect);
                GUIRES_DisplayImg(PNULL,
                                   &disp_rect,
                                   PNULL,
                                   win_id,
                                   res_clock_ic_notification,
                                   &lcd_dev_info
                                   );
#else
            GUI_RECT_T disp_rect1 = LAUNCHER_CLOCK_NOTIFICATION_RECT1;
            disp_rect1 = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, disp_rect1);
            GUIRES_DisplayImg(PNULL,
                               &disp_rect1,
                               PNULL,
                               win_id,
                               res_clock_ic_notification,
                               &lcd_dev_info
                               );
#endif
#endif
            }
            break;
        }

        case MSG_GET_FOCUS:
        {
            uint32 timer_val = 0;
            TRACE_APP_LAUNCHER("MSG_GET_FOCUS");
            MMK_StopTimer(s_clock_update_timer);
            s_clock_update_timer = 0;
#ifdef DROPDOWN_NOTIFY_SHORTCUT
            MMK_StopTimer(s_clock_notify_timer);
            s_clock_notify_timer = 0;
#endif
            MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);

            timer_val = (MMIAPISET_GetClockDisplayStyle() == MMISET_CLOCK_STYLE_ANALOG ||
				        MMIAPISET_GetClockDisplayStyle() == MMISET_CLOCK_STYLE_MULITIFUNCTION) ? 
                                        WATCH_LAUNCHER_CLOCK_UPDATE_INTERVAL : 
                                        WATCH_LAUNCHER_CLOCK_DIGITAL_UPDATE_INTERVAL;

            s_clock_update_timer = MMK_CreateWinTimer(win_id, timer_val, TRUE);
#ifdef DROPDOWN_NOTIFY_SHORTCUT
            s_clock_notify_timer = MMK_CreateWinTimer(win_id, WATCH_LAUNCHER_NOTIFY_TIMER_INTERVAL, TRUE);
#endif
            break;
        }

        case MSG_LOSE_FOCUS:
        {
            TRACE_APP_LAUNCHER("MSG_LOSE_FOCUS");
            MMK_StopTimer(s_clock_update_timer);
            s_clock_update_timer = 0;
#ifdef DROPDOWN_NOTIFY_SHORTCUT
            MMK_StopTimer(s_clock_notify_timer);
            s_clock_notify_timer = 0;
#endif
            break;
        }

        case MSG_TIMER:
        {
            uint8 timer_id = *(uint8 *)param;
            TRACE_APP_LAUNCHER("MSG_TIMER");
            if (timer_id == s_clock_update_timer)
            {
                MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
            }
#ifdef DROPDOWN_NOTIFY_SHORTCUT
            else if (timer_id == s_clock_notify_timer)
            {
                static uint32 old_notity_cnt = 0;
                uint32 new_notity_cnt = NotifyCenter_GetTotalNum();
                if(old_notity_cnt != new_notity_cnt)
                {
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
                    old_notity_cnt = new_notity_cnt;
                }
                
            }
#endif
            break;
        }

#ifdef TOUCH_PANEL_SUPPORT
        //enter to edit clock win by tp long pressed
        case MSG_TP_PRESS_LONG:
        {
            WatchLAUNCHER_EditClockWin_Enter();
            break;
        }
#endif

#ifdef ADULT_WATCH_SUPPORT
        case MSG_APP_WEB:
        {
            MMIAPISET_SetMainmenuStyle(MMIMAINMENU_LIST_E);
            MMIAPIMENU_CreatMainMenu();
            break;
        }

        case MSG_APP_RED:
        {
            bRedKeyDown = TRUE;
            break;
        }

        case MSG_KEYDOWN_USER_FUNC:
        {
            AdultWatchSteps_MainWin_Enter();
            break;
        }

        case MSG_KEYUP_RED:
        {
            if (bRedKeyDown)
            {
                MMIAPISET_SetMainmenuStyle(MMIMAINMENU_LIST_E);
                MMIAPIMENU_CreatMainMenu();
            }
            bRedKeyDown = FALSE;
            break;
        }

        case MSG_KEYLONG_RED:
        {
            bRedKeyDown = FALSE;
            recode = FALSE;
            break;
        }
#endif
        case MSG_KEYLONG_WEB:
        {
            //WatchLAUNCHER_BatteryLow_Enter();
            WatchLAUNCHER_EditClockWin_Enter();
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();

    if (!recode)
    {
        recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
    }
    TRACE_APP_LAUNCHER("exit");;
    return recode;
}
#endif

#ifdef LAUNCHER_ONE_APP_IN_PAGE

LOCAL MMI_RESULT_E HandleLauncherCallWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    TRACE_APP_LAUNCHER("win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_GET_FOCUS:
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
            break;
        case MSG_FULL_PAINT:
        {
            GUI_RECT_T win_rect = {0};
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            TRACE_APP_LAUNCHER("MSG_FULL_PAINT");
            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            DisplayWinPanelBg(win_id);
            DisplayWinPanelFgBase(win_id, res_launcher_ic_call, win_rect, TXT_LAUNCHER_CC_TITLE, win_rect);
            DisplayWinPanelFgAddUnreadInfoNum(win_id, MMIAPICC_GetMissedCallNumber());
            break;
        }

        case MSG_APP_WEB:
        {
            TRACE_APP_LAUNCHER("MSG_APP_WEB");
            WatchPB_MainMenuWin_Enter();
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();

    if (!recode)
    {
        recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
    }
    TRACE_APP_LAUNCHER("exit");;
    return recode;
}

#ifdef STOPWATCH_SUPPORT
LOCAL MMI_RESULT_E HandleLauncherStopWatchWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    TRACE_APP_LAUNCHER("win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_GET_FOCUS:
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
            break;
        case MSG_FULL_PAINT:
        {
            GUI_RECT_T win_rect = {0};
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            TRACE_APP_LAUNCHER("MSG_FULL_PAINT");
            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            DisplayWinPanelBg(win_id);
            DisplayWinPanelFgBase(win_id, res_launcher_ic_timer, win_rect, TXT_LAUNCHER_STOPWATCH_TITLE, win_rect);
            break;
        }

        case MSG_APP_WEB:
        {
            TRACE_APP_LAUNCHER("MSG_APP_WEB");
            WatchStopWatch_MainWin_Enter();
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    if (!recode)
    {
        recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
    }

    MMI_CheckAllocatedMemInfo();
    TRACE_APP_LAUNCHER("exit");;
    return recode;
}
#endif

LOCAL MMI_RESULT_E HandleLauncherCameraWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    TRACE_APP_LAUNCHER("win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_GET_FOCUS:
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
            break;
        case MSG_FULL_PAINT:
        {
            GUI_RECT_T win_rect = {0};
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            DisplayWinPanelBg(win_id);
            DisplayWinPanelFgBase(win_id, res_launcher_ic_camera, win_rect, TXT_LAUNCHER_CAMERA_TITLE, win_rect);
            break;
        }

        case MSG_APP_WEB:
        {
            //to do -- invoke camera
#ifdef BAIDU_DRIVE_SUPPORT
	if(MMIAPIBAIDU_IsAccountExist() ||
	    (0 == MMIAPIPHONE_GetSimAvailableNum(PNULL, 0)) // no sim insert
	  )
	{
            MMIAPIDC_OpenPhotoWin();
	}
	else
	{
		MMIAPIBAIDU_DRV_OpenQRImgWindow();
	}
#else
    MMIAPIDC_OpenPhotoWin();
#endif
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();
    if (!recode)
    {
        recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
    }
    return recode;
}

LOCAL MMI_RESULT_E HandleLauncherGalleryWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    TRACE_APP_LAUNCHER("win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_GET_FOCUS:
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
            break;
        case MSG_FULL_PAINT:
        {
            GUI_RECT_T win_rect = {0};
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            DisplayWinPanelBg(win_id);
            DisplayWinPanelFgBase(win_id, res_launcher_ic_photo, win_rect, TXT_LAUNCHER_GALLERY_TITLE, win_rect);
            break;
        }

        case MSG_APP_WEB:
        {
            WatchGallery_MainWin_Enter();
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();
    if (!recode)
    {
        recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
    }
    return recode;
}

#ifdef MMI_RECORD_SUPPORT
LOCAL MMI_RESULT_E HandleLauncherVoiceRecordWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    TRACE_APP_LAUNCHER("win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_GET_FOCUS:
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
            break;
        case MSG_FULL_PAINT:
        {
            GUI_RECT_T win_rect = {0};
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            DisplayWinPanelBg(win_id);
            DisplayWinPanelFgBase(win_id, res_launcher_ic_soundrecorder, win_rect, TXT_LAUNCHER_VOICERECORDER_TITLE, win_rect);
            break;
        }

        case MSG_APP_WEB:
        {
            //to do --- invoke voice recorder.
            MMIRECORD_OpenMainWin(PNULL);
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();
    if (!recode)
    {
        recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
    }
    return recode;
}
#endif

LOCAL MMI_RESULT_E HandleLauncherSmsWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    TRACE_APP_LAUNCHER("win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_GET_FOCUS:
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
            break;
        case MSG_FULL_PAINT:
        {
            GUI_RECT_T win_rect = {0};
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            DisplayWinPanelBg(win_id);
            DisplayWinPanelFgBase(win_id, res_launcher_ic_message, win_rect, TXT_LAUNCHER_SMS_TITLE, win_rect);
            DisplayWinPanelFgAddUnreadInfoNum(win_id,MMIAPISMS_GetAllUnreadMsgCount());
            break;
        }

        case MSG_APP_WEB:
        {
#ifdef SCREEN_SHAPE_CIRCULAR
            WatchSMS_MessageWin_Enter();
#else
            WatchSMS_MainMenuWin_Enter();
#endif
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();
    if (!recode)
    {
        recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
    }
    return recode;
}

LOCAL MMI_RESULT_E HandleLauncherSettingWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    TRACE_APP_LAUNCHER("win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_GET_FOCUS:
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
            break;
        case MSG_FULL_PAINT:
        {
            GUI_RECT_T win_rect = {0};
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            DisplayWinPanelBg(win_id);
            DisplayWinPanelFgBase(win_id, res_launcher_ic_settings, win_rect, TXT_LAUNCHER_SETTINGS_TITLE, win_rect);
            break;
        }

        case MSG_APP_WEB:
        {
            WatchSET_MainWin_Enter();
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();
    if (!recode)
    {
        recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
    }
    return recode;
}

#if defined(LEBAO_MUSIC_SUPPORT)// wuxx add
LOCAL MMI_RESULT_E HandleLauncherLebaoWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    TRACE_APP_LAUNCHER("win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_FULL_PAINT:
        {
            GUI_RECT_T win_rect = {0};
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            DisplayWinPanelBg(win_id);
            DisplayWinPanelFgBase(win_id, res_launcher_ic_migu, win_rect, TXT_LAUNCHER_MIGU_TITLE, win_rect);
            break;
        }

        case MSG_APP_WEB:
        {
            MMIAPIMENU_EnterLebao();
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();
    if (!recode)
    {
        recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
    }
    return recode;
}							  
#endif                                    

#ifndef ADULT_WATCH_SUPPORT
#if defined(JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT) || defined(APP_STORE_SUPPORT) || defined(DYNAMIC_WATCHFACE_SUPPORT)
LOCAL MMI_RESULT_E HandleLauncherExtraWinMsg(
                                      MMI_WIN_ID_T        win_id,       //IN:
                                      MMI_MESSAGE_ID_E    msg_id,       //IN:
                                      DPARAM              param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    TRACE_APP_LAUNCHER("win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_GET_FOCUS:
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
            break;
        case MSG_FULL_PAINT:
        {
            GUI_RECT_T win_rect = {0};
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            TRACE_APP_LAUNCHER("MSG_FULL_PAINT");
            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            DisplayWinPanelBg(win_id);
            DisplayWinPanelFgBase(win_id, res_launcher_ic_extra, win_rect, TXT_EXTRA_LAUNCHER, win_rect);
            break;
        }

        case MSG_APP_WEB:
        {
            TRACE_APP_LAUNCHER("MSG_APP_WEB");
            WatchEXTRA_MainWin_Enter();
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    if (!recode)
    {
        recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
    }

    MMI_CheckAllocatedMemInfo();
    TRACE_APP_LAUNCHER("exit");;
    return recode;
}
#endif
#endif

#ifdef ADULT_WATCH_SUPPORT
LOCAL MMI_RESULT_E HandleLauncherSportWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    TRACE_APP_LAUNCHER("win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_GET_FOCUS:
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
            break;
        case MSG_FULL_PAINT:
        {
            GUI_RECT_T win_rect = {0};
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            DisplayWinPanelBg(win_id);
            DisplayWinPanelFgBase(win_id, res_launcher_ic_sport, win_rect, NULL, win_rect);
            break;
        }

        case MSG_APP_WEB:
        {
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();
    if (!recode)
    {
        recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
    }
    return recode;
}

LOCAL MMI_RESULT_E HandleLauncherMovingObjectWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    TRACE_APP_LAUNCHER("win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_GET_FOCUS:
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
            break;
        case MSG_FULL_PAINT:
        {
            GUI_RECT_T win_rect = {0};
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            DisplayWinPanelBg(win_id);
            DisplayWinPanelFgBase(win_id, res_launcher_ic_moving_object, win_rect, NULL, win_rect);
            AdultWatchSteps_DrawStepsInfo(win_id);
            break;
        }

        case MSG_APP_WEB:
        {
            break;
        }
        case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T tp_point = {0};
            tp_point.x = MMK_GET_TP_X(param);
            tp_point.y = MMK_GET_TP_Y(param);
            AdultWatchSteps_ClickEvent(tp_point);
            break;
        }
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();
    if (!recode)
    {
        recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
    }
    return recode;
}

LOCAL MMI_RESULT_E HandleLauncherHeartRateWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    TRACE_APP_LAUNCHER("win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_FULL_PAINT:
        {
            GUI_RECT_T win_rect = {0};
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            DisplayWinPanelBg(win_id);
            DisplayWinPanelFgBase(win_id, res_launcher_ic_heart_rate, win_rect, TXT_NULL, win_rect);
            AdultWatchHeartRate_DrawHeartRateInfo(win_id);
            break;
        }

        case MSG_APP_WEB:
        {
            break;
        }

        case MSG_SLIDEPAGE_END:
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            break;
        }

        case MSG_GET_FOCUS:
            if(MMK_IsFocusWin(win_id))
            {
                MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
                MMIDEFAULT_AllowTurnOffBackLight(FALSE);
                AdultWatchHeartRate_ActiveSensor();
            }
            break;

        case MSG_LOSE_FOCUS:
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            AdultWatchHeartRate_DeactiveSensor();
            AdultWatchHeartRate_SetMeasuredStatus(FALSE);
        break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();
    if (!recode)
    {
        recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
    }
    return recode;
}

LOCAL MMI_RESULT_E HandleLauncherPayWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    TRACE_APP_LAUNCHER("win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_GET_FOCUS:
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, PNULL);
            break;
        case MSG_FULL_PAINT:
        {
            GUI_RECT_T win_rect = {0};
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            DisplayWinPanelBg(win_id);
            DisplayWinPanelFgBase(win_id, res_launcher_ic_pay, win_rect, NULL, win_rect);
            break;
        }

        case MSG_APP_WEB:
        {
            break;
        }
#ifdef ENG_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_TP_PRESS_DOWN:
        {
            s_eng_tp_down_count++;
            break;
        }
        case MSG_TP_PRESS_UP:
        {
            if(s_eng_tp_down_count == 5)
            {
                s_eng_tp_down_count = 0;
                if(!MMIENG_IsMonkeyTestMode())
                {
                UITESTWIN_OpenPhoneTestWin();
            }
            }
            break;
        }
#endif
        case MSG_SLIDEPAGE_END:
        {
            s_eng_tp_down_count = 0;
            break;
        }
#endif

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();
    if (!recode)
    {
        recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
    }
    return recode;
}
#endif
#ifndef DYNAMIC_WATCHFACE_SUPPORT
/*win table begin*/
WINDOW_TABLE(WATCH_LAUNCHER_CLOCK_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherClockWinMsg),
    WIN_ID(WATCH_LAUNCHER_CLOCK_WIN_ID ),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};
#endif
WINDOW_TABLE(WATCH_LAUNCHER_CALL_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherCallWinMsg),
    WIN_ID(WATCH_LAUNCHER_CALL_WIN_ID ),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};

#ifdef STOPWATCH_SUPPORT
WINDOW_TABLE(WATCH_LAUNCHER_STOPWATCH_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherStopWatchWinMsg),
    WIN_ID(WATCH_LAUNCHER_STOPWATCH_WIN_ID ),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};
#endif

WINDOW_TABLE(WATCH_LAUNCHER_CAMERA_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherCameraWinMsg),
    WIN_ID(WATCH_LAUNCHER_CAMERA_WIN_ID ),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};

WINDOW_TABLE(WATCH_LAUNCHER_GALLERY_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherGalleryWinMsg),
    WIN_ID(WATCH_LAUNCHER_GALLERY_WIN_ID ),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};

#ifdef MMI_RECORD_SUPPORT
WINDOW_TABLE(WATCH_LAUNCHER_VOICERECORDER_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherVoiceRecordWinMsg),
    WIN_ID(WATCH_LAUNCHER_VOICERECORDER_WIN_ID ),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};
#endif

WINDOW_TABLE(WATCH_LAUNCHER_SMS_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherSmsWinMsg),
    WIN_ID(WATCH_LAUNCHER_SMS_WIN_ID ),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};

WINDOW_TABLE(WATCH_LAUNCHER_SETTING_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherSettingWinMsg),
    WIN_ID(WATCH_LAUNCHER_SETTINGS_WIN_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};
#if defined(LEBAO_MUSIC_SUPPORT)// wuxx add
WINDOW_TABLE(WATCH_LAUNCHER_LEBAO_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherLebaoWinMsg),
    WIN_ID(WATCH_LAUNCHER_LEBAO_WIN_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};
#endif
    
#ifndef ADULT_WATCH_SUPPORT
#if defined(JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT) || defined(APP_STORE_SUPPORT) || defined(DYNAMIC_WATCHFACE_SUPPORT)
WINDOW_TABLE(WATCH_LAUNCHER_EXTRA_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherExtraWinMsg),
    WIN_ID(WATCH_LAUNCHER_EXTRA_WIN_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};
#endif
#endif
#ifdef ADULT_WATCH_SUPPORT
WINDOW_TABLE(WATCH_LAUNCHER_SPORT_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)HandleLauncherSportWinMsg),
    WIN_ID(WATCH_LAUNCHER_SPORT_WIN_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};

WINDOW_TABLE(WATCH_LAUNCHER_MOVING_OBJECT_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)HandleLauncherMovingObjectWinMsg),
    WIN_ID(WATCH_LAUNCHER_MOVING_OBJECT_WIN_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};

WINDOW_TABLE(WATCH_LAUNCHER_HEART_RATE_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)HandleLauncherHeartRateWinMsg),
    WIN_ID(WATCH_LAUNCHER_HEART_RATE_WIN_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};

WINDOW_TABLE(WATCH_LAUNCHER_PAY_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)HandleLauncherPayWinMsg),
    WIN_ID(WATCH_LAUNCHER_PAY_WIN_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN | WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};
#endif
/*win table end*/


/**--------------------------------------------------------------------------*
**                         PUBLIC FUNCTION                                   *
**---------------------------------------------------------------------------*/
#ifndef DYNAMIC_WATCHFACE_SUPPORT
PUBLIC void WatchLAUNCHER_ClockWin_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_CLOCK_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_CLOCK_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}
#endif
//open call window page
PUBLIC void WatchLAUNCHER_CallWin_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_CALL_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_CALL_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}

#ifdef STOPWATCH_SUPPORT
//open stopwatch window page
PUBLIC void WatchLAUNCHER_StopWatchWin_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_STOPWATCH_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_STOPWATCH_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}
#endif

//open camera window page
PUBLIC void WatchLAUNCHER_CameraWin_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_CAMERA_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_CAMERA_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}

//open gallery window page
PUBLIC void WatchLAUNCHER_GalleryWin_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_GALLERY_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_GALLERY_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}

#ifdef MMI_RECORD_SUPPORT
//open voice recorder window page
PUBLIC void WatchLAUNCHER_VoiceRecordWin_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_VOICERECORDER_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_VOICERECORDER_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}
#endif

//open sms window page
PUBLIC void WatchLAUNCHER_SmsWin_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_SMS_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_SMS_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}

//open setting window page
PUBLIC void WatchLAUNCHER_SettingWin_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_SETTINGS_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_SETTING_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}

#if defined(LEBAO_MUSIC_SUPPORT)// wuxx add
//open migu music window page LEBAO_MUSIC_SUPPORT
PUBLIC void WatchLAUNCHER_LebaoWin_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_LEBAO_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_LEBAO_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}
#endif

#ifndef ADULT_WATCH_SUPPORT
#if defined(JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT) || defined(APP_STORE_SUPPORT) || defined(DYNAMIC_WATCHFACE_SUPPORT)
PUBLIC void WatchLAUNCHER_ExtraWin_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_EXTRA_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_EXTRA_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}
#endif
#endif

#ifdef ADULT_WATCH_SUPPORT
PUBLIC void WatchLAUNCHER_SportWin_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_SPORT_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_SPORT_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}

//open moving object card
PUBLIC void WatchLAUNCHER_MovingObjectWin_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_MOVING_OBJECT_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_MOVING_OBJECT_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}

//open heart rate card
PUBLIC void WatchLAUNCHER_HeartRateWin_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_HEART_RATE_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_HEART_RATE_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}

//open pay card
PUBLIC void WatchLAUNCHER_PayWin_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_PAY_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_PAY_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}
#endif

LOCAL void Launcher_OneApp_Page()
{
    tWatchSlidePageItem elem[LAUNCHER_ELEM_COUNT] = {0};
    MMI_HANDLE_T handle = WatchSLIDEPAGE_CreateHandle();
    uint8 i = 0;

#ifdef ADULT_WATCH_SUPPORT
    elem[i].fun_enter_win = WatchLAUNCHER_Charge_Enter;
    elem[i].win_id = WATCH_LAUNCHER_CHARGE_WIN_ID;
    if(FALSE == CHGMNG_IsChargeConnect())//bug1577388
    {
    elem[i].is_disabled = TRUE;
    }
    i++;
#ifdef DYNAMIC_WATCHFACE_SUPPORT
    elem[i].fun_enter_win = WATCHFACE_Win_Enter;
    elem[i].win_id = WATCHFACE_WIN_ID;
#else
    elem[i].fun_enter_win = WatchLAUNCHER_ClockWin_Enter;
    elem[i].win_id = WATCH_LAUNCHER_CLOCK_WIN_ID;
#endif
    i++;

    elem[i].fun_enter_win = WatchLAUNCHER_MovingObjectWin_Enter;
    elem[i].win_id = WATCH_LAUNCHER_MOVING_OBJECT_WIN_ID;
    i++;

    elem[i].fun_enter_win = WatchLAUNCHER_HeartRateWin_Enter;
    elem[i].win_id = WATCH_LAUNCHER_HEART_RATE_WIN_ID;
    i++;
    elem[i].fun_enter_win = WatchLAUNCHER_SportWin_Enter;
    elem[i].win_id = WATCH_LAUNCHER_SPORT_WIN_ID;

    i++;

    elem[i].fun_enter_win = WatchLAUNCHER_PayWin_Enter;
    elem[i].win_id = WATCH_LAUNCHER_PAY_WIN_ID;
    i++;
    WatchSLIDEPAGE_Open(handle, elem, i, 1, TRUE, WatchLAUNCHER_HandleCb);
#else
    elem[i].fun_enter_win = WatchLAUNCHER_Charge_Enter;
    elem[i].win_id = WATCH_LAUNCHER_CHARGE_WIN_ID;
    if(FALSE == CHGMNG_IsChargeConnect())//bug1577388
    {
    elem[i].is_disabled = TRUE;
    }
    i++;
#ifdef DYNAMIC_WATCHFACE_SUPPORT
    elem[i].fun_enter_win = WATCHFACE_Win_Enter;
    elem[i].win_id = WATCHFACE_WIN_ID;
#else
    elem[i].fun_enter_win = WatchLAUNCHER_ClockWin_Enter;
    elem[i].win_id = WATCH_LAUNCHER_CLOCK_WIN_ID;
#endif
    i++;

    elem[i].fun_enter_win = WatchLAUNCHER_CallWin_Enter;
    elem[i].win_id = WATCH_LAUNCHER_CALL_WIN_ID;
    i++;

#ifdef STOPWATCH_SUPPORT
    elem[i].fun_enter_win = WatchLAUNCHER_StopWatchWin_Enter;// temp move to first
    elem[i].win_id = WATCH_LAUNCHER_STOPWATCH_WIN_ID;
    i++;
#endif

    elem[i].fun_enter_win = WatchLAUNCHER_CameraWin_Enter;
    elem[i].win_id = WATCH_LAUNCHER_CAMERA_WIN_ID;
    i++;

    elem[i].fun_enter_win = WatchLAUNCHER_GalleryWin_Enter;
    elem[i].win_id = WATCH_LAUNCHER_GALLERY_WIN_ID;
    i++;

#ifdef MMI_RECORD_SUPPORT
    elem[i].fun_enter_win = WatchLAUNCHER_VoiceRecordWin_Enter;
    elem[i].win_id = WATCH_LAUNCHER_VOICERECORDER_WIN_ID;
    i++;
#endif

    elem[i].fun_enter_win = WatchLAUNCHER_SmsWin_Enter;
    elem[i].win_id = WATCH_LAUNCHER_SMS_WIN_ID;
    i++;

    elem[i].fun_enter_win = WatchLAUNCHER_SettingWin_Enter;
    elem[i].win_id = WATCH_LAUNCHER_SETTINGS_WIN_ID;
    i++;

#if defined(LEBAO_MUSIC_SUPPORT)// wuxx add
    elem[i].fun_enter_win = WatchLAUNCHER_LebaoWin_Enter; // LEBAO_MUSIC_SUPPORT
    elem[i].win_id = WATCH_LAUNCHER_LEBAO_WIN_ID;
    i++;	
#endif

#if defined(JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT) || defined(APP_STORE_SUPPORT) || defined(DYNAMIC_WATCHFACE_SUPPORT)
    elem[i].fun_enter_win = WatchLAUNCHER_ExtraWin_Enter;
    elem[i].win_id = WATCH_LAUNCHER_EXTRA_WIN_ID;
    i++;
#endif

    WatchSLIDEPAGE_Open(handle, elem, i, 1, FALSE, WatchLAUNCHER_HandleCb);
#endif
    s_handle = handle;

}

#endif

#ifdef LAUNCHER_FOUR_APP_IN_PAGE

PUBLIC MMI_RESULT_E WatchLAUNCHER_FourAppHandleCommonWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    LOCAL BOOLEAN b_tp_pressed = FALSE;
    LOCAL BOOLEAN b_tp_moved = FALSE;
    LOCAL GUI_POINT_T tp_point = {0};
    MMI_CheckAllocatedMemInfo();
    TRACE_APP_LAUNCHER("win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);
    switch (msg_id)
    {
        case MSG_KEYLONG_HASH:
        {
            TRACE_APP_LAUNCHER("MSG_KEYLONG_HASH");
#ifdef PDA_UI_DROPDOWN_WIN
            MMINotifyWin_EnterWin(SLIDEWIN_STARTUPMODE_AUTO);
            //MMIZDT_DropDown_EnterWin(SLIDEWIN_STARTUPMODE_AUTO);
#endif
            break;
        }

#ifdef TOUCH_PANEL_SUPPORT
        case MSG_TP_PRESS_DOWN:
        {
            tp_point.x = MMK_GET_TP_X(param);
            tp_point.y = MMK_GET_TP_Y(param);
#ifndef DROPDOWN_NOTIFY_SHORTCUT
#ifdef PDA_UI_DROPDOWN_WIN
            if (IsPressedStatusBarRect(tp_point))
            {
                //StartUp DropWin
                TRACE_APP_LAUNCHER("IsPressedStatusBarRect() = true.");

                MMINotifyWin_EnterWin(SLIDEWIN_STARTUPMODE_TP);
                //MMIZDT_DropDown_EnterWin(SLIDEWIN_STARTUPMODE_AUTO);
            }
#endif
#else
            if (IsPressedStatusBarRect(tp_point))
            {
                //StartUp DropWin
                TRACE_APP_LAUNCHER("IsPressedStatusBarRect() = true.");
#ifdef DYNAMIC_WATCHFACE_SUPPORT
                if(MMK_IsFocusWin(WATCHFACE_WIN_ID))
#else
                if(MMK_IsFocusWin(WATCH_LAUNCHER_CLOCK_WIN_ID))
#endif
                {
                    NotifyCenter_EnterShortcutWin();
                }
            }
            else if(IsPressedSoftkeyBarRect(tp_point))
 			{
#ifdef DYNAMIC_WATCHFACE_SUPPORT

                if(MMK_IsFocusWin(WATCHFACE_WIN_ID))
#else
                if(MMK_IsFocusWin(WATCH_LAUNCHER_CLOCK_WIN_ID))
#endif
                {
					NotifyCenter_EnterListWin();
                }
			}
#endif
            b_tp_pressed = TRUE;
            break;
        }

        case MSG_TP_PRESS_MOVE:
        {
            TRACE_APP_LAUNCHER("MSG_TP_PRESS_MOVE");
            b_tp_moved = TRUE;
            break;
        }

        case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T tp_up = {0};
            BOOLEAN bClick = FALSE;
            TRACE_APP_LAUNCHER("MSG_TP_PRESS_UP");
            tp_up.x = MMK_GET_TP_X(param);
            tp_up.y = MMK_GET_TP_Y(param);
            bClick = (abs(tp_up.x - tp_point.x) < 3) && (abs(tp_up.y - tp_point.y) < 3);
            if (b_tp_pressed && bClick)
            {
                //post tp click msg to win.
                MMK_PostMsg(win_id, MSG_APP_WEB, &tp_up, sizeof(GUI_POINT_T));
            }
            b_tp_pressed = FALSE;
            b_tp_moved = FALSE;
            break;
        }
#endif
        case MSG_KEYDOWN_0:
            if(!MMIENG_IsMonkeyTestMode())
            {
                MMIAPIMENU_CreatMainMenu();
            }
            break;
        case MSG_NOTIFY_OK:
        case MSG_APP_OK:
        {
            SCI_TRACE_LOW("WatchCC_DialpadWin_Enter");
            WatchCC_DialpadWin_Enter();
            break;
        }

        case MSG_NOTIFY_CANCEL:
        case MSG_APP_CANCEL:
            if(!MMIENG_IsMonkeyTestMode())
            {
                MMIAPIENG_StartEngineerMenu();
            }
            break;
        case MSG_KEYDOWN_8:
            // TODO:
#ifdef PRODUCT_CONFIG_UWS6131_XTC_I23
        case MSG_APP_RED:
            SCI_TRACE_LOW("WatchLAUNCHER_HandleCommonWinMsg() msg_id = %d.", msg_id);
#endif
            if(TRUE == MMIDEFAULT_IsBacklightOn())
            {
                MMIDEFAULT_TurnOffBackLight();
                MMIDEFAULT_CloseAllLight_Watch();//close LCD
            }else
            {
                MMIDEFAULT_TurnOnBackLight();
            }
            break;
        default:
            recode = MMI_RESULT_FALSE;
    }
    MMI_CheckAllocatedMemInfo();
    TRACE_APP_LAUNCHER("exit");
    return recode;
}

typedef void (*STARTAPPHANDLE)(void);
#define APP_MAX_SIZE 24     //最多24个应用
#define APP_COUNT_IN_PAGE 4 //每页4个应用

typedef enum{
    LAUNCHER_APP_PAGE_ONE = 0,
    LAUNCHER_APP_PAGE_TWO,
    LAUNCHER_APP_PAGE_THREE,
    LAUNCHER_APP_PAGE_FOUR,
}LAUNCHER_APP_PAGE_TYPE;

typedef struct
{   uint8 check_sim;
    GUI_RECT_T click_rect;
    STARTAPPHANDLE start_handle;
}LAUNCHER_APP_ITEM;

static const LAUNCHER_APP_ITEM s_all_apps[APP_MAX_SIZE] = 
{
    //第一页
    MEUN_PHONEBOOK,//通讯录
    MEUN_DIAL,//拨号
#ifdef TULING_AI_SUPPORT
   MEUN_AICHART,//机器人
#endif
#if defined(ZDT_VIDEOCHAT_SUPPORT) && defined(VIDEO_CALL_AGORA_SUPPORT) //声网视频通话
    MEUN_VIDEO_CALL,//MEUN_VIDEO_CALL,MEUN_VIDEO_INCOMING_TEST//视频通话
#endif
    //第二页
    MEUN_WECHART,//微聊
    MEUN_CAMERA,//相机
    MEUN_GALLERY,//相册
    MEUN_BIND_DEVICE,//绑定流程
    //第三页
    MEUN_SETTINGS, //设置
    MEUN_FIND_FRIEND,//秒表
    #if defined(ZDT_TOOLS_MENU_SUPPORT)// wuxx add.
    MEUN_WATCH_TOOLS,//工具箱
    #else
    MEUN_CALCULATOR,//计算器
    #endif
    MEUN_PEDOMETER,//闹钟
    //第四页
#if 0//defined(HERO_ENGINE_SUPPORT)
  #if defined(HERO_APP_WSTORE_OPEN)
  MEUN_HERO_APPSTORE,//应用商城,应用市场
  //MEUN_HERO_APPSTORE_MANAGER,//应用管理,应用中心
  #endif
#else
    MEUN_SCHEDULE, //课程表
    //MEUN_WEATHER, //天气
    //MEUN_PEDOMETER,//计步
#endif

#ifdef LEBAO_MUSIC_SUPPORT
    MEUN_MUISE,//乐宝音乐
#endif

#ifdef ZDT_ZFB_SUPPORT
    MEUN_ALIPAY,//支付宝
#else
    MEUN_MANUAL,//说明书
#endif
    {NULL,NULL},
};

LOCAL void Launcher_App_Start(GUI_POINT_T *point, LAUNCHER_APP_PAGE_TYPE page_index)
{
    if(point != NULL)
    {
        uint8 i=page_index*APP_COUNT_IN_PAGE;
        for(;i<APP_MAX_SIZE;i++)
        {
            if(s_all_apps[i].start_handle == NULL || i >= (page_index+1)*APP_COUNT_IN_PAGE)
            {
                TRACE_APP_LAUNCHER("there is not app rect");
                return;
            }
            else if(GUI_PointIsInRect(*point,s_all_apps[i].click_rect))
            {
                if(s_all_apps[i].check_sim == 1 && MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1) != SIM_STATUS_OK)
                {
                    //MMIZDT_OpenNoSimOrDataWin();
                    MMIPUB_OpenAlertWarningWin(TXT_NO_SIM_OR_DATA);
                }
                else
                {
                    s_all_apps[i].start_handle();
                }
                return;
            }	
        }
    }
}

PUBLIC MMI_RESULT_E WatchLAUNCHER_FourApp_HandleCb(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    TRACE_APP_LAUNCHER("%s, win_id = 0x%0x, msg_id = 0x%0x.", __FUNCTION__,win_id, msg_id);
    switch (msg_id)
    {
        case MSG_SLIDEPAGE_OPENED:
        {
            break;
        }

        case MSG_SLIDEPAGE_GETFOCUS:
        {
            break;
        }

        case MSG_SLIDEPAGE_LOSEFOCUS:
        {
            break;
        }

        case MSG_SLIDEPAGE_PAGECHANGED:
        {
            break;
        }
        case MSG_SLIDEPAGE_END:
        {
            MMK_SendMsg(win_id, MSG_SLIDEPAGE_END, PNULL);
            break;
        }
        default:
            break;
    }
    return recode;
}

LOCAL void DisplayAppPage(MMI_WIN_ID_T win_id, MMI_IMAGE_ID_T image_id)
{
    GUI_RECT_T win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    MMK_GetWinRect(win_id, &win_rect);

    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);
    GUIRES_DisplayImg(PNULL,
                       &win_rect,
                       PNULL,
                       win_id,
                       image_id,
                       &lcd_dev_info
                       );
}


LOCAL MMI_RESULT_E HandleLauncherAppPageOneWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    TRACE_APP_LAUNCHER("win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_GET_FOCUS:
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
            break;
        case MSG_FULL_PAINT:
        {
            DisplayAppPage(win_id, res_app_page1);
            break;
        }

        case MSG_APP_WEB:
        {
            Launcher_App_Start((GUI_POINT_T*)param, LAUNCHER_APP_PAGE_ONE);
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();
    if (!recode)
    {
        recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
    }
    return recode;
}


LOCAL MMI_RESULT_E HandleLauncherAppPageTwoWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    TRACE_APP_LAUNCHER("win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_GET_FOCUS:
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
            break;
        case MSG_FULL_PAINT:
        {
            DisplayAppPage(win_id, res_app_page2);
            break;
        }

        case MSG_APP_WEB:
        {
            Launcher_App_Start((GUI_POINT_T*)param, LAUNCHER_APP_PAGE_TWO);
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();
    if (!recode)
    {
        recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
    }
    return recode;
}

LOCAL MMI_RESULT_E HandleLauncherAppPageThreeWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    TRACE_APP_LAUNCHER("win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_GET_FOCUS:
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
            break;
        case MSG_FULL_PAINT:
        {
            DisplayAppPage(win_id, res_app_page3);
            break;
        }

        case MSG_APP_WEB:
        {
            Launcher_App_Start((GUI_POINT_T*)param, LAUNCHER_APP_PAGE_THREE);
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();
    if (!recode)
    {
        recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
    }
    return recode;
}

LOCAL MMI_RESULT_E HandleLauncherAppPageFourWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    TRACE_APP_LAUNCHER("win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_GET_FOCUS:
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
            break;
        case MSG_FULL_PAINT:
        {
            DisplayAppPage(win_id, res_app_page4);
            break;
        }

        case MSG_APP_WEB:
        {
            Launcher_App_Start((GUI_POINT_T*)param, LAUNCHER_APP_PAGE_FOUR);
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();
    if (!recode)
    {
        recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
    }
    return recode;
}

WINDOW_TABLE(WATCH_LAUNCHER_APP_PAGE1_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherAppPageOneWinMsg),
    WIN_ID(WATCH_LAUNCHER_APP_PAGE1_WIN_ID ),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};

WINDOW_TABLE(WATCH_LAUNCHER_APP_PAGE2_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherAppPageTwoWinMsg),
    WIN_ID(WATCH_LAUNCHER_APP_PAGE2_WIN_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};

WINDOW_TABLE(WATCH_LAUNCHER_APP_PAGE3_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherAppPageThreeWinMsg),
    WIN_ID(WATCH_LAUNCHER_APP_PAGE3_WIN_ID ),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};

WINDOW_TABLE(WATCH_LAUNCHER_APP_PAGE4_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherAppPageFourWinMsg),
    WIN_ID(WATCH_LAUNCHER_APP_PAGE4_WIN_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};


PUBLIC void WatchLAUNCHER_PageOneWin_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_APP_PAGE1_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_APP_PAGE1_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}

PUBLIC void WatchLAUNCHER_PageTwoWin_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_APP_PAGE2_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_APP_PAGE2_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}

PUBLIC void WatchLAUNCHER_PageThreeWin_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_APP_PAGE3_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_APP_PAGE3_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}

PUBLIC void WatchLAUNCHER_PageFourWin_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_APP_PAGE4_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_APP_PAGE4_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}

LOCAL void Launcher_FourApp_page()
{
    tWatchSlidePageItem elem[LAUNCHER_ELEM_COUNT] = {0};
    MMI_HANDLE_T handle = WatchSLIDEPAGE_CreateHandle();
    uint8 i = 0;

#ifdef DYNAMIC_WATCHFACE_SUPPORT
    //elem[i].fun_enter_win = WATCHFACE_Win_Enter;
    //elem[i].win_id = WATCHFACE_WIN_ID;
    elem[i].fun_enter_win = MMIWatchPanel_Open;
    elem[i].win_id = WATCH_LAUNCHER_PANEL_WIN_ID;
#else
    elem[i].fun_enter_win = WatchLAUNCHER_ClockWin_Enter;
    elem[i].win_id = WATCH_LAUNCHER_CLOCK_WIN_ID;
#endif
    i++;

    elem[i].fun_enter_win = WatchLAUNCHER_PageOneWin_Enter;
    elem[i].win_id = WATCH_LAUNCHER_APP_PAGE1_WIN_ID;
    i++;

    elem[i].fun_enter_win = WatchLAUNCHER_PageTwoWin_Enter;
    elem[i].win_id = WATCH_LAUNCHER_APP_PAGE2_WIN_ID;
    i++;

    elem[i].fun_enter_win = WatchLAUNCHER_PageThreeWin_Enter;
    elem[i].win_id = WATCH_LAUNCHER_APP_PAGE3_WIN_ID;
    i++;
    elem[i].fun_enter_win = WatchLAUNCHER_PageFourWin_Enter;
    elem[i].win_id = WATCH_LAUNCHER_APP_PAGE4_WIN_ID;
    i++;

    //elem[i].fun_enter_win = WatchLAUNCHER_AllAppPage_Enter;
    //elem[i].win_id = WATCH_LAUNCHER_ALL_APP_PAGE_WIN_ID;
    //

    //i++;
    WatchSLIDEPAGE_Open(handle, elem, i, 0, TRUE, WatchLAUNCHER_FourApp_HandleCb);
    s_handle = handle;
}

#endif


#ifdef LAUNCHER_ALL_APP_IN_PAGE
typedef void(*STARTAPPHANDLE)(void);
typedef struct 
{
    uint8 check_sim;
#ifdef APP_ITEM_NAME //是否显示菜单名称
    MMI_TEXT_ID_T   text_id;//the title of the item
#endif
    MMI_IMAGE_ID_T  img_id;//the image of the item
    STARTAPPHANDLE start_handle;
}APP_LIST_ITEM_T;

const APP_LIST_ITEM_T  g_app_list_info[] = 
{
    MEUN_PHONEBOOK,//通讯录
    MEUN_DIAL,//拨号
#if defined(ZDT_VIDEOCHAT_SUPPORT) && defined(VIDEO_CALL_AGORA_SUPPORT) //声网视频通话
    #ifndef WIN32//视频通话
    MEUN_VIDEO_CALL,
    #else
    MEUN_VIDEO_INCOMING_TEST,
    #endif
#endif
    MEUN_WECHART,//微聊
//    MEUN_CALLLOG,
    MEUN_CAMERA,//相机
    MEUN_GALLERY,//相册
    MEUN_FIND_FRIEND,// 交友
    MEUN_QRCODE,
    MEUN_SETTINGS, //设置
    MEUN_STOPWATCH,//秒表
    MEUN_WEATHER, //天气
//    MEUN_CALCULATOR,//计算器
    MEUN_ALARM,//闹钟
#ifdef ZDT_ZFB_SUPPORT
    MEUN_ALIPAY, //支付宝
#endif
#ifdef TULING_AI_SUPPORT
    MEUN_AI_CHAT,//图灵AI
#endif
#ifdef XIAODU_SUPPORT
    MEUN_XIAODU,
#endif
#ifdef LEBAO_MUSIC_SUPPORT
    MEUN_MUSIC,//咪咕音乐
#endif
#if defined(XYSDK_SUPPORT)|| defined(XYSDK_SRC_SUPPORT)
    MEUN_XMLY,//喜马拉雅
#endif
#ifdef MAINMENU_STYLE_SUPPORT
    MEUN_STYLE,//主菜单风格切换
#endif
#ifndef ZTE_WATCH
    MEUN_SCHEDULE,
    MEUN_PANEL_SWITCH,//表盘切换
    MEUN_BRIGHTNESS, //亮度调节
    MEUN_DEVICE_ABOUT,//关于设备  
    MEUN_FACTORY_RESET, //恢复出厂
    MEUN_POWER_OFF, //关机
    MEUN_POWER_RESTART, //重启
    MEUN_RING,//铃声
    MEUN_VOLUME,//音量
    MEUN_SCREEN_TIMEOUT, //息屏时间
#endif
#ifdef FOTA_SUPPORT // bao add. wuxx add 20231031
    MEUN_FOTA,
#endif
 //   MEUN_PEDOMETER,//计步
};

LOCAL uint8 APP_MENU_SIZE = sizeof(g_app_list_info)/sizeof(APP_LIST_ITEM_T);

#ifdef APP_ITEM_NAME //是否显示菜单名称
LOCAL void Append_App_Title(MMI_CTRL_ID_T ctrl_id, DPARAM param)
{
    GUIICONLIST_DISP_T  icon_list = {0};
    uint16  icon_index = *((uint16 *)param);
    MMI_GetLabelTextByLang(g_app_list_info[icon_index].text_id,&icon_list.name_str);//title
    GUIICONLIST_AppendText(icon_index,ctrl_id,&icon_list);
}
#endif

LOCAL void Append_App_Icon(MMI_CTRL_ID_T ctrl_id, DPARAM param)
{
    GUIANIM_DATA_INFO_T  data_info = {0};
    uint16 icon_index = *((uint16 *)param);
    data_info.img_id = g_app_list_info[icon_index].img_id;
    GUIICONLIST_AppendIcon(icon_index,ctrl_id,&data_info,PNULL);
}

LOCAL void App_List_Start_App(uint8 index)
{
    if (index < APP_MENU_SIZE)
    {
        if(g_app_list_info[index].check_sim == 1 && MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1) != SIM_STATUS_OK)
        {
            MMIPUB_OpenAlertWarningWin(TXT_NO_SIM_OR_DATA);
        }
        else if(g_app_list_info[index].start_handle != NULL)
        {
            g_app_list_info[index].start_handle();
        }
    }
}

LOCAL void Create_App_IconList(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUI_BORDER_T   select_border = {0};
    GUI_RECT_T list_rect = APP_LIST_RECT;
    GUI_FONT_ALL_T font_all = {0};
    GUI_BG_T bg = {0};
    GUIICONLIST_MARGINSPACE_INFO_T margin_space = APP_LIST_ITEM_MARGIN_SPACE;
    select_border.type = GUI_BORDER_NONE;
    select_border.width = 0;
    GUIICONLIST_SetHideSlideBar(ctrl_id);
    GUIICONLIST_SetItemBorderStyle(ctrl_id, FALSE, &select_border);//不需要边框
    GUIICONLIST_SetItemBorderStyle(ctrl_id, TRUE, &select_border);//不需要选中边框
    GUIICONLIST_SetTotalIcon(ctrl_id,APP_MENU_SIZE);
    GUIICONLIST_SetRect(ctrl_id,&list_rect);
    GUIAPICTRL_SetBothRect(ctrl_id, &list_rect);
    GUIICONLIST_SetIconItemSpace(ctrl_id, margin_space);
    GUIICONLIST_SetStyle(ctrl_id, GUIICONLIST_STYLE_ICON_UIDT);
    CTRLICONLIST_SetLayoutStyle(ctrl_id,GUIICONLIST_LAYOUT_V);
    GUIICONLIST_SetCurIconIndex(0, ctrl_id);
    GUIICONLIST_SetIconWidthHeight(ctrl_id, APP_LIST_ITEM_ICON_WIDTH, APP_LIST_ITEM_ICON_HEIGHT); 
    GUIICONLIST_SetLoadType(ctrl_id, GUIICONLIST_LOAD_ALL);
    font_all.color = MMI_WHITE_COLOR;
    font_all.font = SONG_FONT_20;
    GUIICONLIST_SetIconListTextInfo(ctrl_id, font_all);
    bg.bg_type = GUI_BG_COLOR;
    bg.color = MMI_BLACK_COLOR;
    GUIICONLIST_SetBg(ctrl_id, &bg); 
    MMK_SetAtvCtrl(win_id,ctrl_id);
}

LOCAL MMI_RESULT_E HandleAppIconListMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = LAUNCHER_APP_LIST_CTRL_ID;
    GUIICONLIST_DELIMITER_INFO_T group_info = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T full_rect = MMITHEME_GetFullScreenRectEx(win_id);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    switch( msg_id)
    {
    case MSG_OPEN_WINDOW:
        //SCI_TRACE_LOW("[MEM] %d KB-- %d KB U=%d KB",SCI_GetSystemSpaceTotalAvalidMem()/1024, SCI_GetHeapTotalSpace(DYNAMIC_HEAP_APP_MEMORY)/1024,ZDT_File_GetMemFree(FALSE));
        Create_App_IconList(win_id, ctrl_id);
        break;

    case MSG_GET_FOCUS:
        //SCI_TRACE_LOW("[MEM] %d KB-- %d KB U=%d KB",SCI_GetSystemSpaceTotalAvalidMem()/1024, SCI_GetHeapTotalSpace(DYNAMIC_HEAP_APP_MEMORY)/1024,ZDT_File_GetMemFree(FALSE));
        break;
    case MSG_FULL_PAINT:
        if(MMK_GetFocusWinId() == win_id)
        {
            GUIRES_DisplayImg(PNULL, &full_rect, PNULL, win_id,IMAGE_THEME_BLACK_BG, &lcd_dev_info); 
        }
        break;
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        {
            uint16 cur_index = GUIICONLIST_GetCurIconIndex(ctrl_id);      
            App_List_Start_App(cur_index);
        }
        break;
#ifdef APP_ITEM_NAME //是否显示菜单名称
    case MSG_CTL_ICONLIST_APPEND_TEXT:
	    Append_App_Title(ctrl_id, param);
        break;
#endif
    case MSG_CTL_ICONLIST_APPEND_ICON:
        Append_App_Icon(ctrl_id, param);
        break;

    case MSG_KEYUP_RED:
        if(MMK_GetFocusWinId() == win_id)
        {
            //WatchSLIDEAGE_SetCurrentPageIndex(1);
        }
        break;
    case MSG_KEYDOWN_RED:
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    if (!result)
    {
        result = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
    }
    return (result);
}

LOCAL MMI_RESULT_E HandleAllAppPageWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
#if defined(MAINMENU_STYLE_SUPPORT) || defined(MAINMENU_STYLE_SUPPORT)
    uint8 style = MMISET_GetWatchMenuStyle();
    if(msg_id == MSG_KEYUP_RED)
    {
        if(MMK_GetFocusWinId() == win_id)
        {
            WatchSLIDEAGE_SetCurrentPageIndex(1);
            return MMI_RESULT_TRUE;
        }
    }
    if(style == 0)
    {
//        Create_App_IconList(win_id, LAUNCHER_APP_LIST_CTRL_ID);
        if(MSG_OPEN_WINDOW != msg_id && CTRLICONLIST_GetTotalIcon(LAUNCHER_APP_LIST_CTRL_ID) == 0)
        {
             Create_App_IconList(win_id, LAUNCHER_APP_LIST_CTRL_ID);
        }
        else
        {
            GUI_RECT_T list_rect = APP_LIST_RECT;
            GUIICONLIST_SetRect(LAUNCHER_APP_LIST_CTRL_ID,&list_rect);
        }
        return HandleAppIconListMsg(win_id,msg_id,param);
    }
    else if(style == 1)
    {
        if(MMK_GetCtrlHandleByWin(win_id,LAUNCHER_APP_LIST_CTRL_ID) != NULL)
        {
            GUI_RECT_T list_rect = {0};
            GUIICONLIST_SetRect(LAUNCHER_APP_LIST_CTRL_ID,&list_rect);
        }
        return HandleMsgMenuSemicircleWindow(win_id,msg_id,param);
    }
#else
    return HandleAppIconListMsg(win_id,msg_id,param);
#endif
}

WINDOW_TABLE(LAUNCHER_ALL_APP_PAGE_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleAllAppPageWinMsg),
    WIN_ID(WATCH_LAUNCHER_ALL_APP_PAGE_WIN_ID),
#ifdef ZTE_WATCH
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
#else
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
#endif
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    CREATE_ICONLIST_CTRL(LAUNCHER_APP_LIST_CTRL_ID),
    END_WIN
};

PUBLIC BOOLEAN WatchLAUNCHER_AllAppPage_IsFocusOpen()
{
    BOOLEAN ret = FALSE;
    if(MMK_IsOpenWin(WATCH_LAUNCHER_ALL_APP_PAGE_WIN_ID))
    {
        ret = TRUE;
    }
    return ret;
}


PUBLIC void WatchLAUNCHER_AllApp_Win(void)
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_ALL_APP_PAGE_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(LAUNCHER_ALL_APP_PAGE_WIN_TAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}

BOOLEAN WatchLAUNCHER_AllAppPage_IsFocusWin()
{
    return MMK_IsFocusWin(WATCH_LAUNCHER_ALL_APP_PAGE_WIN_ID);
}

PUBLIC MMI_RESULT_E WatchLAUNCHER_AppList_HandleCb(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    TRACE_APP_LAUNCHER("%s, win_id = 0x%0x, msg_id = 0x%0x.", __FUNCTION__,win_id, msg_id);
    switch (msg_id)
    {
        case MSG_SLIDEPAGE_OPENED:
        {
            break;
        }

        case MSG_SLIDEPAGE_GETFOCUS:
        {
            break;
        }

        case MSG_SLIDEPAGE_LOSEFOCUS:
        {
            break;
        }

        case MSG_SLIDEPAGE_PAGECHANGED:
        {
            break;
        }
        case MSG_SLIDEPAGE_END:
        {
            MMK_SendMsg(win_id, MSG_SLIDEPAGE_END, PNULL);
            break;
        }
        case MSG_FULL_PAINT:
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            break;
        }
        default:
            break;
    }
    return recode;
}

LOCAL void WatchLAUNCHER_AllAppPage_Enter()
{
    tWatchSlidePageItem elem[LAUNCHER_ELEM_COUNT] = {0};
    MMI_HANDLE_T handle;
    uint8 i = 0;
    if(s_handle != NULL)
    {
        WatchSLIDEPAGE_DestoryHandle(s_handle);
        s_handle = NULL;
    }
    handle= WatchSLIDEPAGE_CreateHandle();  

    elem[i].fun_enter_win = WatchLAUNCHER_AllApp_Win;
    elem[i].win_id = WATCH_LAUNCHER_ALL_APP_PAGE_WIN_ID;
    i++;

    elem[i].fun_enter_win = MMIWatchPanel_Open;
    elem[i].win_id = WATCH_LAUNCHER_PANEL_WIN_ID;
    i++;

    WatchSLIDEPAGE_Open(handle, elem, i, 1, TRUE, WatchLAUNCHER_AppList_HandleCb);
    s_handle = handle;
}

#ifdef ZTE_WATCH

#define LAUNCHER_ELEM_COUNT 3

LOCAL void Init_PhoneBook_And_Dialer_Button(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T phonebook_ctrl_rect = LAUNCHER_PHONEBOOK_LABEL_RECT;
    GUI_RECT_T dialer_ctrl_rect = LAUNCHER_DIALER_LABEL_RECT;
    GUI_FONT_T font = SONG_FONT_28;
    GUI_BG_T item_bg = {0};
    item_bg.bg_type = GUI_BG_IMG;
    item_bg.img_id = image_watch_list_bg;

    //MMI_GetLabelTextByLang(TXT_CONTACT, &text);
    WATCHCOM_DisplayBackground(win_id);

    phonebook_ctrl_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, phonebook_ctrl_rect);
    GUILABEL_SetRect(LAUNCHER_PHONEBOOK_CTRL_ID, &phonebook_ctrl_rect, TRUE);
    GUILABEL_SetIcon(LAUNCHER_PHONEBOOK_CTRL_ID, res_contact_icon);
    GUILABEL_SetTextById(LAUNCHER_PHONEBOOK_CTRL_ID, TXT_CONTACT, FALSE);
    GUILABEL_SetFont(LAUNCHER_PHONEBOOK_CTRL_ID, font, MMI_WHITE_COLOR);
    GUILABEL_SetTextMargin(LAUNCHER_PHONEBOOK_CTRL_ID,LAUNCHER_PHONEBOOK_AND_DIALER_LABEL_TEXT_SPACE);
    CTRLLABEL_SetMargin(LAUNCHER_PHONEBOOK_CTRL_ID,LAUNCHER_PHONEBOOK_AND_DIALER_LABEL_TEXT_SPACE);
    CTRLLABEL_SetBg(LAUNCHER_PHONEBOOK_CTRL_ID,&item_bg);

    dialer_ctrl_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, dialer_ctrl_rect);
    GUILABEL_SetRect(LAUNCHER_DIALER_CTRL_ID, &dialer_ctrl_rect, TRUE);
    GUILABEL_SetIcon(LAUNCHER_DIALER_CTRL_ID, res_dialer_icon);
    GUILABEL_SetTextById(LAUNCHER_DIALER_CTRL_ID, TXT_COMM_DIALPAD, FALSE);
    GUILABEL_SetFont(LAUNCHER_DIALER_CTRL_ID, font, MMI_WHITE_COLOR);
    GUILABEL_SetTextMargin(LAUNCHER_DIALER_CTRL_ID,LAUNCHER_PHONEBOOK_AND_DIALER_LABEL_TEXT_SPACE);
    GUILABEL_SetMargin(LAUNCHER_DIALER_CTRL_ID,LAUNCHER_PHONEBOOK_AND_DIALER_LABEL_TEXT_SPACE);
    CTRLLABEL_SetBg(LAUNCHER_DIALER_CTRL_ID,&item_bg);

}

LOCAL void HandleNotifyPenokMsg(DPARAM param)
{
    MMI_NOTIFY_T notify = *(MMI_NOTIFY_T*)param;
    if (notify.src_id == LAUNCHER_PHONEBOOK_CTRL_ID)
    {
        MMIZDT_OpenPBWin();
    }
    else if(notify.src_id == LAUNCHER_DIALER_CTRL_ID)
    {
        MMIZDT_OpenDialWin();
    }
}

LOCAL MMI_RESULT_E HandleLauncherPhoneBookAndDialerWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    TRACE_APP_LAUNCHER("win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            Init_PhoneBook_And_Dialer_Button(win_id);
            break;
        case MSG_GET_FOCUS:
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
            break;
        case MSG_FULL_PAINT:
        {
            WATCHCOM_DisplayBackground(win_id);
            break;
        }

        case MSG_NOTIFY_PENOK:
        case MSG_APP_WEB:
        case MSG_APP_OK:
        {
            HandleNotifyPenokMsg(param);
            break;
        }
        case MSG_KEYDOWN_RED:
            break;
        case MSG_KEYUP_RED:
            WatchSLIDEAGE_SetCurrentPageIndex(1);
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();
    if (!recode)
    {
        recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
    }
    return recode;
}

WINDOW_TABLE(WATCH_LAUNCHER_PHONEBOOK_AND_DIALER_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherPhoneBookAndDialerWinMsg),
    WIN_ID(WATCH_LAUNCHER_PHONEBOOK_AND_DIALER_WIN_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, LAUNCHER_PHONEBOOK_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT, LAUNCHER_DIALER_CTRL_ID),
    END_WIN
};


PUBLIC void WatchLAUNCHER_PhoneBook_And_DialerWin_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_PHONEBOOK_AND_DIALER_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_PHONEBOOK_AND_DIALER_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}

LOCAL MMI_RESULT_E HandleLauncherWechatContactListWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    TRACE_APP_LAUNCHER("win_id = 0x%0x, msg_id = 0x%0x.", win_id, msg_id);
    MMI_CheckAllocatedMemInfo();
    switch (msg_id)
    {
        case MSG_GET_FOCUS:
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,PNULL);
            break;
        case MSG_KEYDOWN_RED:
            break;
        case MSG_KEYUP_RED:
            WatchSLIDEAGE_SetCurrentPageIndex(1);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();
    if (!recode)
    {
        recode = HandleLauncher_ChatContactListWinMsg(win_id, msg_id, param);
    }
    return recode;
}

WINDOW_TABLE(WATCH_LAUNCHER_WECHAT_CONTACT_LIST_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherWechatContactListWinMsg),
    WIN_ID(WATCH_LAUNCHER_WECHAT_CONTACT_LIST_WIN_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};



PUBLIC void WatchLAUNCHER_WechatContactListWin_Enter()
{
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_WECHAT_CONTACT_LIST_WIN_ID;
    MMI_HANDLE_T win_handle = 0;
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    win_handle = MMK_CreateWin(WATCH_LAUNCHER_WECHAT_CONTACT_LIST_WINTAB, NULL);
    MMK_SetWinRect(win_handle, &rect);
}

LOCAL void WatchLAUNCHER_ZTE_Enter()
{
    tWatchSlidePageItem elem[LAUNCHER_ELEM_COUNT] = {0};
    MMI_HANDLE_T handle;
    uint8 i = 0;
    if(s_handle != NULL)
    {
        WatchSLIDEPAGE_DestoryHandle(s_handle);
        s_handle = NULL;
    }
    handle= WatchSLIDEPAGE_CreateHandle();  

    elem[i].fun_enter_win = WatchLAUNCHER_PhoneBook_And_DialerWin_Enter;
    elem[i].win_id = WATCH_LAUNCHER_PHONEBOOK_AND_DIALER_WIN_ID;
    i++;

    elem[i].fun_enter_win = MMIWatchPanel_Open;
    elem[i].win_id = WATCH_LAUNCHER_PANEL_WIN_ID;
    i++;

    elem[i].fun_enter_win = WatchLAUNCHER_WechatContactListWin_Enter;
    elem[i].win_id = WATCH_LAUNCHER_WECHAT_CONTACT_LIST_WIN_ID;
    i++;

    WatchSLIDEPAGE_Open(handle, elem, i, 1, FALSE, WatchLAUNCHER_AppList_HandleCb);
    s_handle = handle;
}
#endif

#endif

/*****************************************************************************/
//  Discription: open watch launcher window
//  Global resource dependence:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
PUBLIC void WatchLAUNCHER_Enter(void)
{
#ifndef RELEASE_INFO
    MMIENG_SetUSBLog(TRUE);  // wuxx for test
    SCI_SetArmLogFlag(TRUE); // wuxx for test
#endif
    if(SIM_STATUS_NO_SIM == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1))
    {
        MMIZDT_OpenInsertSimWin();
    }
#ifdef SETUP_WIZARD_SUPPORT
    else if(MMIZDT_Get_Setup_Wizard_Status() == 0)
    {
        Setup_Wizard_MainWin_Enter();
    }
#endif
    else
    {
        WatchOpen_IdleWin();
    }
    MMIZDT_CheckOpenClassModeWin();
}

PUBLIC void WatchOpen_IdleWin(void)
{
    SCI_TRACE_LOW("[%s]: enter", __FUNCTION__);
#ifdef LAUNCHER_ONE_APP_IN_PAGE
    Launcher_OneApp_Page();
#elif LAUNCHER_FOUR_APP_IN_PAGE
    Launcher_FourApp_page();
#elif LAUNCHER_ALL_APP_IN_PAGE
    #ifdef ZTE_WATCH
        WatchLAUNCHER_ZTE_Enter();
    #else
        WatchLAUNCHER_AllAppPage_Enter();
    #endif
#endif
#ifdef SETUP_WIZARD_SUPPORT
    //MMIZDT_NV_SetFirstPWON(0);
#endif
}

PUBLIC void WatchLAUNCHER_MainMenuSwitch(MMI_WIN_ID_T win_id)
{
#if defined(MAINMENU_STYLE_SUPPORT) || defined(MAINMENU_STYLE_SUPPORT)
    //uint8 style = MMISET_GetWatchMenuStyle();
    //if(s_current_menu_style != style)
    //{
    //    if(style == 0)
    //    {
    //        WatchSLIDEAGE_SetPageEnable(ZTE_MENU_SEMICIRCLE_WIN_ID, FALSE);
    //        WatchSLIDEAGE_SetPageEnable(WATCH_LAUNCHER_ALL_APP_PAGE_WIN_ID, TRUE);
    //        //MMK_SendMsg(WATCH_LAUNCHER_ALL_APP_PAGE_WIN_ID, MSG_FULL_PAINT,PNULL);
    //    }
    //    else if(style == 1)
    //    {
    //        WatchSLIDEAGE_SetPageEnable(ZTE_MENU_SEMICIRCLE_WIN_ID, TRUE);
    //        WatchSLIDEAGE_SetPageEnable(WATCH_LAUNCHER_ALL_APP_PAGE_WIN_ID, FALSE);
    //        //MMK_SendMsg(ZTE_MENU_SEMICIRCLE_WIN_ID, MSG_FULL_PAINT,PNULL);
    //    }        
    //    s_current_menu_style = style;
    //    //WatchSLIDEAGE_SetCurrentPageIndex(0);
    //    //MMK_CloseWin(win_id);
    //    //MMK_CloseWin(WATCH_LAUNCHER_PANEL_WIN_ID);
    //    //WatchOpen_IdleWin();
    //}      
#endif
}


/*****************************************************************************/
//  Discription: display indicator images
//  Global resource dependence:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
PUBLIC void WatchLAUNCHER_DisplayIndicator(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T title_rect = LAUNCHER_INDICATOR_RECT;
    tWatchSlidePage*entity = WatchSLIDEPAGE_GetSlideInfoByHandle(s_handle);
    MMI_IMAGE_ID_T img_id = 0;
    GUI_LCD_DEV_INFO lcd_dev_info =s_indi_layer;//{0, 0};
    uint8 i = 0;
    GUI_POINT_T start_point = {0};
    start_point.x = title_rect.left;
    start_point.y = title_rect.top;

    GUI_FillRect(&lcd_dev_info, title_rect, MMI_BLACK_COLOR);
    if (entity != NULL)
    {
        uint8 cur_page_index = entity->cur_page_index;
 
        for (i = 0; i < entity->page_cnt; i++)
        {
            if (entity->pages[i].is_disabled == TRUE)
                continue;
            img_id = res_launcher_ic_point_off;
            if (i == cur_page_index)
            {
                img_id = res_launcher_ic_point_on;
            }
            start_point.x += WATCH_LAUNCHER_INDICATOR_OFFSET;
            GUIRES_DisplayImg(&start_point,
                PNULL,
                PNULL,
                win_id,
                img_id,
                &lcd_dev_info);
        }
    }

}

/*****************************************************************************/
//  Discription: set charge page visible
//  Global resource dependence:
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
PUBLIC void WatchLAUNCHER_SetChargePageVisible(BOOLEAN bVisible)
{
    if (bVisible)
    {
        WatchSLIDEAGE_SetPageEnable(WATCH_LAUNCHER_CHARGE_WIN_ID, TRUE);
    }
    else
    {
        WatchSLIDEAGE_SetPageEnable(WATCH_LAUNCHER_CHARGE_WIN_ID, FALSE);
    }
}

#ifdef ZTE_WATCH
PUBLIC void MMIZDT_DropUp_EnterWin(eSlideWinStartUpMode launch_mode)
{
    GUI_RECT_T roiRect = MMIDROPUPWIN_ROIREGION;
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_ALL_APP_PAGE_WIN_ID;
    MMI_WIN_ID_T old_id = MMK_GetFocusWinId();
    if (MMK_IsOpenWin(win_id) && MMK_GetFocusWinId() == win_id)
    {
        return;
    }
    WatchLAUNCHER_AllApp_Win();
    do
    {
        uint32 handle = 0;
        tSlideWinInitData init = {0};
        init.win_id = win_id;
        init.win_before_id = old_id;
        init.direction = SLIDEWIN_DIRECTION_VERT;
        init.mode = launch_mode;
        init.style = SLIDEWIN_STYLE_ENABLE_ROI | SLIDEWIN_STYLE_ENABLE_RANGE| SLIDEWIN_STYLE_ENABLE_FREE_WHEN_WINCLOSE;
        init.init_pos = MMIDROPUPWIN_INIT_POS;
        init.stop_pos = MMIDROPUPWIN_STOP_POS;
        init.close_pos = MMIDROPUPWIN_END_POS;
        init.range_min = MMIDROPUPWIN_INIT_POS;
        init.range_max = 0;
        init.roi_region = roiRect;
        handle = WatchSLIDEWIN_Create(&init);

        WatchSLIDEWIN_Start(handle);
    }while (0);
}
#endif

