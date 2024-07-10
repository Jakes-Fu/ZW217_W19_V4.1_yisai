/*****************************************************************************
** File Name:    watch_launcher_editclock.c                                                                       *
** Author:        zhikun.lv                                                                                                 *
** Date:           3/2/2020                                                                                                *
** Copyright:    2020 Spreatrum, Incoporated. All Rights Reserved.                                       *
** Description:  This file is used to describe punctuation                                                      *
******************************************************************************
**                         Important Edit History                                                                        *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                                                    *
** 3/2/2020     zhikun.lv              Create
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "watch_launcher_common.h"
#include "watch_launcher_editclockwin.h"
#include "watch_launcher_main.h"
#include "watch_slidepage.h"
#include "mmicom_trace.h"

#include "mmiset_export.h"
#include "watch_launcher_multe_function_dial.h"

/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/
typedef struct tEditClockWinData
{
    GUI_RECT_T display_rect;
    BOOLEAN is_tp_pressed;
    BOOLEAN is_tp_moved;
    BOOLEAN is_tp_released;
    MMISET_CLOCK_DISPLAY_STYLE_E type;

}tEditClockWinData;

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#ifndef TRACE_APP_LAUNCHER
#define TRACE_APP_LAUNCHER SCI_TRACE_LOW
#endif

#define TRACE_APP_EDITCLOCK TRACE_APP_LAUNCHER
#define TRACE_APP_EDITCLOCK_ENTER TRACE_APP_LAUNCHER("enter")
#define TRACE_APP_EDITCLOCK_EXIT TRACE_APP_LAUNCHER("exit")

#define WATCH_EDIT_ELEM_NUM 3

MMI_HANDLE_T winhandle;

/**--------------------------------------------------------------------------*
**                         LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/
LOCAL tEditClockWinData* CreateInitData()
{
    tEditClockWinData *data = NULL;
    GUI_RECT_T rect = LAUNCHER_EDITCLOCKWIN_RECT;
    if((data = SCI_ALLOC_APPZ(sizeof(tEditClockWinData))) == NULL)
    {
        SCI_ASSERT(0);
    }
    data->display_rect = rect;
    TRACE_APP_EDITCLOCK("alloc memory addr = %0X.", data);
    return data;
}

LOCAL void DrawClockImage(MMI_WIN_ID_T win_id, MMI_IMAGE_ID_T image_id, GUI_RECT_T rect)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T dis_rect = {0};
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    dis_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, rect);
    GUIRES_DisplayImg(PNULL,
                       &dis_rect,
                       PNULL,
                       win_id,
                       image_id,
                       &lcd_dev_info
                       );
}

LOCAL MMI_RESULT_E HandleLauncherEditClockWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    tEditClockWinData *pInfo = MMK_GetWinAddDataPtr(win_id);
    TRACE_APP_EDITCLOCK("add_data addr = %0X.", pInfo);
    TRACE_APP_EDITCLOCK("win_id = %0X. msg_id = %0X", win_id, msg_id);

    if (pInfo == NULL)
    {
        return recode;
    }

    MMI_CheckAllocatedMemInfo();

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            TRACE_APP_EDITCLOCK("MSG_OPEN_WINDOW");
            //MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
            break;
        }

        case MSG_KEYDOWN_CANCEL:
        {
            MMK_CloseWin(win_id);
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            TRACE_APP_EDITCLOCK("MSG_CLOSE_WINDOW");
            WatchSLIDEPAGE_DestoryHandle(winhandle);

            if (pInfo != NULL)
            {
                TRACE_APP_EDITCLOCK("free add_data. addr = %0X.", pInfo);
                MMK_FreeWinAddData(win_id);
            }
            break;
        }

        case MSG_FULL_PAINT:
        {
            GUI_RECT_T win_rect = {0};
            GUI_RECT_T clockpanel_rect = DP2PX_RECT(0, 0, 240, 240);
            GUI_RECT_T dis_rect = DP2PX_RECT(45, 45, 195, 195);
            GUI_RECT_T left_rect = DP2PX_RECT(0, 108, 24, 132);
            GUI_RECT_T right_rect = DP2PX_RECT(216, 108, 240, 132);
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            TRACE_APP_EDITCLOCK("MSG_FULL_PAINT");

            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

            //fill background color
            GUI_FillRect(&lcd_dev_info, win_rect, MMI_BLACK_COLOR);
            clockpanel_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, clockpanel_rect);
            GUIRES_DisplayImg(NULL,
                            &clockpanel_rect,
                            PNULL,
                            win_id,
                            res_aw_edit_clock_mask,
                            &lcd_dev_info);
			
            //draw clock panel by clock type
            if (pInfo->type == MMISET_CLOCK_STYLE_ANALOG)
            {
                DrawClockImage(win_id, res_aw_edit_clock_pointer, dis_rect);
            }
            else if (pInfo->type == MMISET_CLOCK_STYLE_DIGIT) 
            {
                DrawClockImage(win_id, res_aw_edit_clock_number, dis_rect);
            }
			else if  (pInfo->type == MMISET_CLOCK_STYLE_MULITIFUNCTION) 
			{
    			GUI_RECT_T img_rect = DP2PX_RECT(108, 206, 132, 230);
                DrawClockImage(win_id, res_aw_edit_clock_function, dis_rect);
			    GUIRES_DisplayImg(PNULL,
			                       &img_rect,
			                       PNULL,
			                       win_id,
			                       res_aw_edit_ic_edit,
			                       &lcd_dev_info
			                       );
			}
            //draw left && right arrow btn
            left_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, left_rect);
            right_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, right_rect);
            GUIRES_DisplayImg(NULL, &left_rect, NULL, win_id, res_aw_edit_ic_left, &lcd_dev_info);
            GUIRES_DisplayImg(NULL, &right_rect, NULL, win_id, res_aw_edit_ic_right, &lcd_dev_info);
            break;
        }

        case MSG_APP_WEB:
        {
            TRACE_APP_EDITCLOCK("MSG_APP_WEB");
            TRACE_APP_EDITCLOCK("edit selected clock type = %d.", pInfo->type);

            MMIAPISET_SetClockDisplayStyle(pInfo->type);
            MMK_CloseWin(win_id);
            break;
        }
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_TP_PRESS_DOWN:
        {
            TRACE_APP_EDITCLOCK("MSG_TP_PRESS_DOWN");
            pInfo->is_tp_pressed = TRUE;
            pInfo->is_tp_moved = FALSE;
            pInfo->is_tp_released = FALSE;
            break;
        }

        case MSG_TP_PRESS_MOVE:
        {
            TRACE_APP_EDITCLOCK("MSG_TP_PRESS_MOVE");
            pInfo->is_tp_moved = TRUE;
            break;
        }

        case MSG_TP_PRESS_UP:
        {
            TRACE_APP_EDITCLOCK("MSG_TP_PRESS_UP");
            TRACE_APP_EDITCLOCK("edit selected clock type = %d.", pInfo->type);
            if (pInfo->is_tp_pressed/* && !pInfo->is_tp_moved*/)
            {
                BOOLEAN pointresult = TRUE;
                GUI_POINT_T  point = {0};
                point.x = MMK_GET_TP_X(param);
                point.y = MMK_GET_TP_Y(param);
                if  (pInfo->type == MMISET_CLOCK_STYLE_MULITIFUNCTION) 
                {
                    pointresult = OnClickInEditMTFDialWin(point);
                    if(TRUE == pointresult)
                    {
                        return MMI_RESULT_TRUE;
                    }
                }
                pointresult = OnClickInEditDialWin(point);
                if(TRUE == pointresult)
                {
                    return MMI_RESULT_TRUE;
                }
                MMIAPISET_SetClockDisplayStyle(pInfo->type);
	            MMK_CloseWin(win_id);

            }
            break;
        }
#endif
        case MSG_KEYDOWN_1:
        {
            WatchLAUNCHER_SetChargePageVisible(TRUE);
            break;
        }

        case MSG_KEYDOWN_2:
        {
            WatchLAUNCHER_SetChargePageVisible(FALSE);
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    MMI_CheckAllocatedMemInfo();
    TRACE_APP_EDITCLOCK_EXIT;
    return recode;
}

/*define edit analog clock wintab*/
WINDOW_TABLE(WATCH_LAUNCHER_EDITANALOGCLOCK_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherEditClockWinMsg),
    WIN_ID(WATCH_LAUNCHER_EDITANALOGCLOCK_WIN_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_FLING_CLOSE_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    //WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
    END_WIN
};

WINDOW_TABLE(WATCH_LAUNCHER_EDITDIGITALCLOCK_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherEditClockWinMsg),
    WIN_ID(WATCH_LAUNCHER_EDITDIGITALCLOCK_WIN_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_FLING_CLOSE_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    //WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
    END_WIN
};
WINDOW_TABLE(WATCH_LAUNCHER_EDITMULITIFUNCTIONCLOCK_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleLauncherEditClockWinMsg),
    WIN_ID(WATCH_LAUNCHER_EDITMULITIFUNCTIONCLOCK_WIN_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_FLING_CLOSE_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    //WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
    END_WIN
};


LOCAL void WatchLAUNCHER_EditAnalogClockWin1_Enter()
{
    tEditClockWinData* data = CreateInitData();
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_EDITANALOGCLOCK_WIN_ID;
    TRACE_APP_EDITCLOCK_ENTER;
    data->type = MMISET_CLOCK_STYLE_ANALOG;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    MMK_CreateWin(WATCH_LAUNCHER_EDITANALOGCLOCK_WINTAB, data);
    TRACE_APP_EDITCLOCK_EXIT;
}

LOCAL void WatchLAUNCHER_EditDigitalClockWin1_Enter()
{
    tEditClockWinData* data = CreateInitData();
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_EDITDIGITALCLOCK_WIN_ID;
    TRACE_APP_EDITCLOCK_ENTER;
    data->type = MMISET_CLOCK_STYLE_DIGIT;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }
    MMK_CreateWin(WATCH_LAUNCHER_EDITDIGITALCLOCK_WINTAB, data);
    TRACE_APP_EDITCLOCK_ENTER;
}
LOCAL void WatchLAUNCHER_EditMultiFunctionClockWin1_Enter()
{
    tEditClockWinData* data = CreateInitData();
    MMI_WIN_ID_T win_id = WATCH_LAUNCHER_EDITMULITIFUNCTIONCLOCK_WIN_ID;
    TRACE_APP_EDITCLOCK_ENTER;
    data->type = MMISET_CLOCK_STYLE_MULITIFUNCTION;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }
    MMK_CreateWin(WATCH_LAUNCHER_EDITMULITIFUNCTIONCLOCK_WINTAB, data);
    TRACE_APP_EDITCLOCK_ENTER;
}



LOCAL MMI_RESULT_E WatchEditClock_HandleCb(
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
            int test = 0;
            break;
        }

        case MSG_SLIDEPAGE_GETFOCUS:
        {
            int test = 0;
            break;
        }

        case MSG_SLIDEPAGE_LOSEFOCUS:
        {
            int test = 0;
            //MMK_CloseWin(WATCH_LAUNCHER_EDITANALOGCLOCK_WIN_ID);

            break;
        }

        case MSG_SLIDEPAGE_PAGECHANGED:
        {
            int test = 0;
            break;
        }
        default:
            break;
    }
    return recode;
}

/**--------------------------------------------------------------------------*
**                         PUBLIC FUNCTION                                   *
**---------------------------------------------------------------------------*/
/*****************************************************************************
//  Description : open selectclocktype window
//  Author: zhikun.lv
//  Return: 
//  Note:
*****************************************************************************/
PUBLIC void WatchLAUNCHER_EditClockWin_Enter()
{
    tWatchSlidePageItem elem[LAUNCHER_EDITCLOCKWIN_NUM] = {0};
    uint8 elem_num = WATCH_EDIT_ELEM_NUM;
    TRACE_APP_EDITCLOCK_ENTER;
    winhandle = WatchSLIDEPAGE_CreateHandle();

    elem[0].fun_enter_win = WatchLAUNCHER_EditAnalogClockWin1_Enter;
    elem[0].win_id = WATCH_LAUNCHER_EDITANALOGCLOCK_WIN_ID;
    elem[1].fun_enter_win = WatchLAUNCHER_EditDigitalClockWin1_Enter;
    elem[1].win_id = WATCH_LAUNCHER_EDITDIGITALCLOCK_WIN_ID;
    elem[2].fun_enter_win = WatchLAUNCHER_EditMultiFunctionClockWin1_Enter;
    elem[2].win_id = WATCH_LAUNCHER_EDITMULITIFUNCTIONCLOCK_WIN_ID;
    if (MMIAPISET_GetClockDisplayStyle() == MMISET_CLOCK_STYLE_ANALOG)
    {
        WatchSLIDEPAGE_Open(winhandle, elem, elem_num, 0, TRUE, WatchEditClock_HandleCb);
    }
    else if (MMIAPISET_GetClockDisplayStyle() == MMISET_CLOCK_STYLE_DIGIT)
    {
        WatchSLIDEPAGE_Open(winhandle, elem, elem_num, 1, TRUE, WatchEditClock_HandleCb);
    }
    else if (MMIAPISET_GetClockDisplayStyle() == MMISET_CLOCK_STYLE_MULITIFUNCTION)
    {
        WatchSLIDEPAGE_Open(winhandle, elem, elem_num, 2, TRUE, WatchEditClock_HandleCb);
    }

    TRACE_APP_EDITCLOCK_EXIT;
}
