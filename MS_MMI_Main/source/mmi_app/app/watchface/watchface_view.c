/****************************************************************************
** File Name:      watchface_view.c                                         *
** Author:                                                                  *
** Date:           2021.8.23                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to do provide network access api       *
*****************************************************************************
**                         Important Edit History                           *
** --------------------------------------------------------------------------*
** DATE           NAME                         DESCRIPTION                  *
** 2021.8.23      xiaoju.cheng                 Create
**
****************************************************************************/

/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "os_param.h"
#include "watchface_flow.h"
#include "mmiset_display.h"
#include "mmi_mainmenu_export.h"
#ifdef ADULT_WATCH_SUPPORT
#include "watch_steps.h"
#endif
#include "window_parse.h"
#include "mmicom_trace.h"
#include "watchface_edit_win.h"
#include "mmidisplay_color.h"
#include "mmiudisk_export.h"

#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
#include "jee.h"
#endif

#define WATCHFACE_USB_TIPS_RECT        DP2PX_RECT(0,0,239,239)
/*---------------------------------------------------------------------------*/
/*                        Macro Declaration                                  */
/*---------------------------------------------------------------------------*/
LOCAL MMI_RESULT_E HandleWatchfaceWinMsg(
                                      MMI_WIN_ID_T        win_id,
                                      MMI_MESSAGE_ID_E    msg_id,
                                      DPARAM              param
                                      );

/*****************************************************************************/
//  Description : create watchface usb tips
//  Parameter: None
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL void WATCHFACE_CreateUSBTips(void);

/*****************************************************************************/
//  Description : destory watchface usb tips
//  Parameter: None
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL void WATCHFACE_DestoryUSBTips(void);


/*---------------------------------------------------------------------------*/
/*                        Function Declaration                               */
/*---------------------------------------------------------------------------*/

WINDOW_TABLE(WATCHFACE_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleWatchfaceWinMsg),
    WIN_ID(WATCHFACE_WIN_ID ),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN |WS_DISABLE_RETURN_WIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};
LOCAL void WatchfaceWinLoseFocus(MMI_WIN_ID_T win_id)
{
    if (0 != MMK_GetCtrlHandleByWin(win_id, WATCH_FACE_USB_TIPS_CTRL_ID))
    {
        WATCHFACE_DestoryUSBTips();
    }
    if (FALSE == MMIAPIUDISK_UdiskIsRun())
    {
        WATCHFACE_StopTimer();
#if defined(UI_MULTILAYER_SUPPORT) && !defined(WF_USE_SINGLELAYER)
        WATCHFACE_RemoveLayer();
#endif
    }
    {
        uint16 w, h;
        GUI_RECT_T rect={0,0,0,0};
        GUI_LCD_DEV_INFO dev_info ={0};
        MMK_GetWinLcdDevInfo(win_id, &dev_info);
        GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &w, &h);
        rect.right = w-1;
        rect.bottom = h-1;
        MMITHEME_StoreUpdateRect(&dev_info,rect);
    }
}

LOCAL MMI_RESULT_E HandleWatchfaceWinMsg(
                                      MMI_WIN_ID_T        win_id,
                                      MMI_MESSAGE_ID_E    msg_id,
                                      DPARAM              param
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    BOOLEAN bRedKeyDown = FALSE;

    TRACE_WATCHFACE("[watchface_view]:msg_id = 0x%x",msg_id);

    switch (msg_id)
    {
        case MSG_FULL_PAINT:
        {
            TRACE_WATCHFACE("[watchface_view]:receive MSG_FULL_PAINT");
            WATCHFACE_FullPaint();
            break;
        }
        case MSG_GET_FOCUS:
        {
            TRACE_WATCHFACE("[watchface_view]: MSG_GET_FOCUS");
            MMK_PostMsg( win_id, MSG_FULL_PAINT, PNULL, PNULL);
            if (TRUE == MMIAPIUDISK_UdiskIsRun())
            {
                WATCHFACE_CreateUSBTips();
            }
            else
            {
#if defined(UI_MULTILAYER_SUPPORT) && !defined(WF_USE_SINGLELAYER)
                WATCHFACE_AppendLayer();
#endif
            }
            break;
        }
        case MSG_LOSE_FOCUS:
        {
            TRACE_WATCHFACE("[watchface_view]: MSG_LOSE_FOCUS");
            WatchfaceWinLoseFocus(win_id);
            //GUILCD_Invalidate(GUI_MAIN_LCD_ID, 0);  //B1904999 watch face flash.
            break;
        }
        case MSG_KEYDOWN_RED:
        {
            TRACE_WATCHFACE("[watchface_view]:receive MSG_KEYDOWN_RED");
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
#ifdef ADULT_WATCH_SUPPORT
        case MSG_APP_WEB:
        {
            TRACE_WATCHFACE("[watchface_view]:receive MSG_APP_WEB");
            MMIAPISET_SetMainmenuStyle(MMIMAINMENU_LIST_E);
            MMIAPIMENU_CreatMainMenu();
            break;
        }
        case MSG_APP_RED:
        {
            TRACE_WATCHFACE("[watchface_view]:receive MSG_APP_RED");
            bRedKeyDown = TRUE;
            break;
        }

        case MSG_KEYDOWN_USER_FUNC:
        {
            TRACE_WATCHFACE("[watchface_view]:receive MSG_KEYDOWN_USER_FUNC");
            AdultWatchSteps_MainWin_Enter();
            break;
        }
        case MSG_KEYUP_RED:
        {
            TRACE_WATCHFACE("[watchface_view]:receive MSG_KEYUP_RED");
            bRedKeyDown = FALSE;
            break;
        }
        case MSG_KEYLONG_RED:
        {
            TRACE_WATCHFACE("[watchface_view]:receive MSG_KEYLONG_RED");
            bRedKeyDown = FALSE;
            recode = FALSE;
            break;
        }
#endif
#ifdef TOUCH_PANEL_SUPPORT
        //enter to edit clock win by tp long pressed
        case MSG_TP_PRESS_LONG:
        {
            if (FALSE == MMIAPIUDISK_UdiskIsRun())
            {
                WATCHFACE_Edit_Slidepage_Enter();
            }
            break;
        }
#endif
#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
        case MSG_APP_6:
            jee_start_app("com.unisoc.test.require",NULL);
            break;
        case MSG_APP_7:
            jee_start_app("com.unisoc.test.import",NULL);
            break;
#endif

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    if (!recode)
    {
        recode = WatchLAUNCHER_HandleCommonWinMsg(win_id, msg_id, param);
    }
    return recode;
}

/*****************************************************************************/
//  Description : enter watchface
//  Parameter:None
//  Author: xiaoju.cheng
//  Note:
/*****************************************************************************/
PUBLIC void WATCHFACE_Win_Enter(void)
{
    MMI_WIN_ID_T win_id = WATCHFACE_WIN_ID;

    TRACE_WATCHFACE("[watchface_view]:enter win");

    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    MMK_CreateWin(WATCHFACE_WINTAB, NULL);
}
/*****************************************************************************/
//  Description : UpdateDate
//  Parameter:None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_UpdateDate(void)
{
    if ((MMK_IsOpenWin(WATCHFACE_WIN_ID))&&(MMK_IsFocusWin(WATCHFACE_WIN_ID)))
    {
        return MMK_PostMsg( WATCHFACE_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : create watchface usb tips
//  Parameter: None
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL void WATCHFACE_CreateUSBTips(void)
{
    MMI_CONTROL_CREATE_T    tips_ctrl       = {0};
    GUIANIM_INIT_DATA_T     init_data       = {0};
    GUIANIM_CTRL_INFO_T     control_info    = {0};
    GUIANIM_DATA_INFO_T     data_info       = {0};
    GUIANIM_DISPLAY_INFO_T  display_info    = {0};
    CAF_RECT_T              both_rect       = WATCHFACE_USB_TIPS_RECT;

    TRACE_WATCHFACE("[watchface_view]enter");

    //init data
    init_data.is_form_bg     = FALSE;
    SCI_MEMCPY(&init_data.both_rect.v_rect, &both_rect, sizeof(CAF_RECT_T));

    //creat
    tips_ctrl.ctrl_id            = WATCH_FACE_USB_TIPS_CTRL_ID;
    tips_ctrl.guid               = SPRD_GUI_ANIM_ID;
    tips_ctrl.parent_win_handle  = WATCHFACE_WIN_ID;
    tips_ctrl.init_data_ptr      = &init_data;

    MMK_CreateControl(&tips_ctrl);

    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = WATCH_FACE_USB_TIPS_CTRL_ID;
#ifdef ADULT_WATCH_SUPPORT
    data_info.img_id = usb_storage_connect_success;
#else
    data_info.img_id = res_common_usb;
#endif
    display_info.bg.bg_type = GUI_BG_COLOR;
    display_info.bg.color = MMI_BLACK_COLOR;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;

    GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);

    TRACE_WATCHFACE("[watchface_view]exit");
}

/*****************************************************************************/
//  Description : destory watchface usb tips
//  Parameter: None
//  Author: yifan.yang
//  Note:
/*****************************************************************************/
LOCAL void WATCHFACE_DestoryUSBTips(void)
{
    MMI_WIN_ID_T win_id = WATCHFACE_WIN_ID;

    TRACE_WATCHFACE("[watchface_view]enter");

    if (0 != MMK_GetCtrlHandleByWin(win_id, WATCH_FACE_USB_TIPS_CTRL_ID))
    {
        MMK_DestroyControl(WATCH_FACE_USB_TIPS_CTRL_ID);
    }

    TRACE_WATCHFACE("[watchface_view]exit");
}

