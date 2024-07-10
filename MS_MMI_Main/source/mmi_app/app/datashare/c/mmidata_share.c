/****************************************************************************************
** File Name:    mmidata_share.c                                                         *
** Author:         liang.wu1                                                           *
** Date:           2020.01.14                                                            *
** Copyright:      2020 Spreadtrum, Incorporated. All Rights Reserved.                  *
** Description:     for usb data share, RNDIS module                                         *
** NOTE:                                                           *
*****************************************************************************************/
#include "window_parse.h"
#include "mmiset_export.h"
#include "mmipdp_export.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmipdp_internal.h"
#include "mmipdp_list.h"
#include "sig_code.h"
#include "mmi_signal.h"
#include "mmi_signal_ext.h"
#include "mn_type.h"
#include "mn_api.h"
#include "mmi_nv.h"
#include "mmk_timer.h"
//#include "mmipdp_export.h"
#include "mmiphone_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiidle_export.h"
#include "mmiconnection_export.h"
#include "mmipdp_export.h"
#include "sci_api.h"
#include "mmisms_export.h"
#include "mmiidle_statusbar.h"
#include "mmi_id.h"
#include "mmiacc_text.h"
#include "mmidisplay_data.h" 
#ifdef PLATFORM_ANTISW3
#include "usbshare_api.h"
#endif
#include "usbservice_api.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmipub.h"
#include "mmidata_share.h"

#define MMI_USBSHARE_RESTART_TIME_OUT   300
#define MMI_USBSHARE_PDP_RETRY_TIME   6000
static uint8  g_usbshare_restart_timer = 0;
static uint8  g_usbshare_restart_timer_cnt = 0;
static BOOLEAN g_usbshare_start = FALSE;
static uint32 g_usbshare_nsapi = 0;
LOCAL uint8  retry_timer_id = 0;
#ifdef ADULT_WATCH_SUPPORT
#include "watch_commonwin_export.h"
LOCAL void MMIUSBSHARE_WatchOpenErrPrompt(MMI_TEXT_ID_T alert_text_id,MMI_IMAGE_ID_T img_id )
{
    MMI_STRING_T commentString ;
    WATCH_SOFTKEY_TEXT_ID_T   softkey_test = {TXT_NULL,STXT_RETURN,TXT_NULL};
    MMI_GetLabelTextByLang(alert_text_id, &commentString);
    MMK_CloseWin( USB_SHARE_CONNECT_WIN_ID );
    Adult_WatchCOM_NoteWin_1Icon_1Str_Enter(USB_SHARE_CONNECT_WIN_ID, &commentString, img_id, PNULL );
}
#endif
LOCAL MMI_RESULT_E HandleUsbShareConnectWindow (
                                              MMI_WIN_ID_T       win_id,    // STARTUP CHARGE窗口的ID
                                              MMI_MESSAGE_ID_E      msg_id,     // 窗口的内部消息ID
                                              DPARAM                param       // 相应消息的参数
                                              );

// the window for usb share connect
WINDOW_TABLE( USB_SHARE_CONNECT_WIN_TAB ) = 
{
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_FUNC( (uint32)HandleUsbShareConnectWindow ),
    WIN_ID( USB_SHARE_CONNECT_WIN_ID ),
    WIN_TITLE(TXT_UDISK_VUART),
//1656763-adult watch no statusbar.
#ifdef ADULT_WATCH_SUPPORT
    WIN_HIDE_STATUS,
#else
    WIN_STATUSBAR,
#endif
//1656763-only phone board display softkey
#ifndef TOUCH_PANEL_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_EXIT),
#endif
    END_WIN
};

LOCAL MMI_RESULT_E HandleAlertExitWinMsg(
                                      MMI_WIN_ID_T win_id,
                                      MMI_MESSAGE_ID_E    msg_id, 
                                      DPARAM param
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_TIMER:
        break;
        
    case MSG_CLOSE_WINDOW:
        //exit usb share screen
        MMK_CloseWin(USB_SHARE_CONNECT_WIN_ID);
        break;

    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return recode;
}
LOCAL void UsbShareTimerHandler(uint8 timer_id, uint32 param)
{
    uint32 remote_nsapi = param;
    
    if (timer_id == g_usbshare_restart_timer)
    {
    #ifdef PLATFORM_ANTISW3
        if (UsbShare_start(remote_nsapi) == 0)
        {
            MMK_StopTimer(g_usbshare_restart_timer);
            g_usbshare_restart_timer = 0;
            g_usbshare_start = TRUE;
            
            /* start RNDIS driver here */
            SCI_TRACE_LOW("[USB SHARE] %s %d USB_StartService\n", __FUNCTION__, __LINE__);
            USB_StartService(USB_SERVICE_UCOM);
            g_usbshare_nsapi = param;
        }
    #endif

        if (g_usbshare_restart_timer_cnt++ > 10)
        {
            SCI_TRACE_LOW("[USB SHARE] %s %d USB_StartService FAIL \n", __FUNCTION__, __LINE__);
            MMK_StopTimer(g_usbshare_restart_timer);
            g_usbshare_restart_timer = 0;
            MMIUSBSHARE_Disconnect();
#ifdef ADULT_WATCH_SUPPORT
            MMIUSBSHARE_WatchOpenErrPrompt(TXT_SYS_WAITING_AND_TRY_LATER, pc_comm_connect_fail);
#else
            MMIPUB_OpenAlertWarningWin(TXT_SYS_WAITING_AND_TRY_LATER);
#endif
        }
    }
}

LOCAL void UsbShareTimer(BOOLEAN restart, uint32 remote_nsapi)
{
    if (g_usbshare_restart_timer != 0)
    {
        MMK_StopTimer(g_usbshare_restart_timer);
        g_usbshare_restart_timer = 0;
    }

    if (restart)
    {
        g_usbshare_restart_timer = MMK_CreateTimerCallback(MMI_USBSHARE_RESTART_TIME_OUT, UsbShareTimerHandler, remote_nsapi, TRUE);
    }
}

LOCAL void UsbShare_HandleRetryTimerMsg(uint8 timer_id, uint32 param)
{
    if (timer_id == retry_timer_id && 0 != retry_timer_id)
    {
        MMK_StopTimer(retry_timer_id);
        
        retry_timer_id = 0;	
        
        if(!MMIUSBSHARE_ConnectPDP())
        {
#ifdef ADULT_WATCH_SUPPORT
            MMIUSBSHARE_WatchOpenErrPrompt(TXT_COMMON_LINK_DISCONNECTED, pc_comm_connect_fail);
#else
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_LINK_DISCONNECTED,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandleAlertExitWinMsg);
#endif
        }  
    }
}

LOCAL void HandleUsbSharePdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{

    if (PNULL == msg_ptr)
    {
        return;
    }

    switch(msg_ptr->msg_id) 
    {
        case MMIPDP_ACTIVE_CNF:     //PDP激活成功
            SCI_TRACE_LOW("[USB share] MMIPDP_ACTIVE_CNF msg_ptr->result:%d\n", msg_ptr->result);
            if (MMIPDP_RESULT_SUCC == msg_ptr->result)
            {
                g_usbshare_restart_timer_cnt = 0;
                #ifdef PLATFORM_ANTISW3
                if (UsbShare_start(msg_ptr->nsapi) == 0)
                {
                    g_usbshare_start = TRUE;
                    /* start RNDIS driver here */
                    SCI_TRACE_LOW("[USB SHARE] %s %d USB_GetCurService():%d\n", __FUNCTION__, __LINE__, USB_GetCurService());
                    if(USB_SERVICE_UCOM != USB_GetCurService())
                    {
                        SCI_TRACE_LOW("[USB SHARE] %s %d USB_StartService\n", __FUNCTION__, __LINE__);
                        USB_StartService(USB_SERVICE_UCOM);
                    }
                    g_usbshare_nsapi = msg_ptr->nsapi;
                }
                else
                {
                    SCI_TRACE_LOW("[USB SHARE] %s %d UsbShareTimer\n", __FUNCTION__, __LINE__);
                    UsbShareTimer(TRUE, msg_ptr->nsapi);
                }
                #endif
            }
#if defined(MMI_GPRS_SUPPORT) || defined(MMI_WIFI_SUPPORT)
            else
            {
                SCI_TRACE_LOW("[USB SHARE] %s %d MMIAPIPDP_Deactive\n", __FUNCTION__, __LINE__);
                MMIAPIPDP_Deactive(MMI_MODULE_DATASHARE);
                MMIUSBSHARE_Disconnect();
#ifdef ADULT_WATCH_SUPPORT
                MMIUSBSHARE_WatchOpenErrPrompt(TXT_SYS_WAITING_AND_TRY_LATER, pc_comm_connect_fail);
#else
                MMIPUB_OpenAlertWarningWin(TXT_CALL_USING);
#endif

            }
#endif
            break;

        case MMIPDP_DEACTIVE_CNF:   //PDP去激活成功
            SCI_TRACE_LOW("[USB share] MMIPDP_DEACTIVE_CNF\n");
            UsbShareTimer(FALSE, 0);
            g_usbshare_start = FALSE;
            
            break;

        case MMIPDP_DEACTIVE_IND:   //PDP被网络端去激活
            SCI_TRACE_LOW("[USB share] MMIPDP_DEACTIVE_IND\n");
            UsbShareTimer(FALSE, 0);
            g_usbshare_start = FALSE;
            SCI_TRACE_LOW("[USB share] USB_StopService\n");
            USB_StopService(USB_SERVICE_UCOM);
            retry_timer_id = MMK_CreateTimerCallback(MMI_USBSHARE_PDP_RETRY_TIME, UsbShare_HandleRetryTimerMsg, NULL, FALSE);
            break;

        default:
            break;
    }
}

/*****************************************************************************/
// 	Description : active pdp 
//	Global resource dependence : 
//    Author: liang.wu1
//	Note:
/*****************************************************************************/
BOOLEAN MMIUSBSHARE_ConnectPDP(void)
{
#ifndef WIN32

#if defined(MMI_GPRS_SUPPORT) || defined(MMI_WIFI_SUPPORT)
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    MMIPDP_ACTIVE_INFO_T app_info = {0};
    MMICONNECTION_LINKSETTING_DETAIL_T* setting_item_ptr = PNULL;

    // fist close browser
 /*   if (MMIAPIBROWSER_IsRunningEx(MMIBROWSER_TYPE_DORADO)
        || MMIAPIBROWSER_IsRunningEx(MMIBROWSER_TYPE_OPERA_MINI))
    {
        MMIPUB_OpenAlertWarningWin(TXT_SYS_WAITING_AND_TRY_LATER);
        return FALSE;
    }
    */
    //setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(MNGPRS_GetSwitchDataCard() , 0);
    setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(0 , 0);
    if(setting_item_ptr != PNULL)
    {		 
    	app_info.app_handler = MMI_MODULE_DATASHARE;
    	app_info.apn_ptr = (char*)setting_item_ptr->apn;
    	app_info.user_name_ptr = (char*)setting_item_ptr->username;
    	app_info.psw_ptr = (char*)setting_item_ptr->password;
    	app_info.dual_sys = 0;//MNGPRS_GetSwitchDataCard();
    	app_info.priority = 1;
    	app_info.ps_service_rat = MN_TD_PREFER;
    	app_info.handle_msg_callback = HandleUsbSharePdpMsg;
    	app_info.ps_service_type = BROWSER_E;
    	app_info.storage = MN_GPRS_STORAGE_ALL;
        
#ifdef IPVERSION_SUPPORT_V4_V6
        app_info.ip_type = setting_item_ptr->ip_type;
#endif

    	if(!MMIAPIPDP_Active(&app_info))
    	{
            SCI_TRACE_LOW("[USB share] MMIUSBSHARE_ConnectPDP pdp active failed\n");
            return FALSE;
    	}
    }
    else
    {
        SCI_TRACE_LOW("[USB share] MMIUSBSHARE_ConnectPDP [APN IDX ERROR]\n");
        return FALSE;
    }

    SCI_TRACE_LOW("[USB share] MMIUSBSHARE_ConnectPDP [Net Setting SUCESS]\n");

   return TRUE;
#endif   
#endif

}

/*****************************************************************************/
// 	Description : MMIUSBSHARE_Disable
//	Global resource dependence : 
//    Author: liang.wu1
//	Note:
/*****************************************************************************/
PUBLIC void MMIUSBSHARE_Disable(void)
{
    SCI_TraceLow("[USB share] MMIUSBSHARE_Disable ");

    if (g_usbshare_nsapi)
    {
    #ifdef PLATFORM_ANTISW3
        /* Usbshare cleanup has been moved to TCPIP_DeregNetInterface */
        //UsbShare_stop(g_usbshare_nsapi);
        UsbShare_stop_flow_control(g_usbshare_nsapi);
    #endif
        g_usbshare_nsapi = 0;
    }
    
    MMIAPIPDP_Deactive(MMI_MODULE_DATASHARE);
}

/*****************************************************************************/
// 	Description : MMIUSBSHARE_IsRunning
//	Global resource dependence : 
//    Author: liang.wu1
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIUSBSHARE_IsRunning(void)
{
    SCI_TraceLow("[USB share] MMIUSBSHARE_IsRunning =%d",g_usbshare_start);

    return g_usbshare_start;
}

PUBLIC void MMIUSBSHARE_Disconnect(void)
{
    SCI_TraceLow("[USB share] MMIUSBSHARE_Disconnect");
    if(MMK_IsOpenWin(USB_SHARE_CONNECT_WIN_ID))
    {
        MMK_CloseWin(USB_SHARE_CONNECT_WIN_ID);
    }
}
PUBLIC BOOLEAN MMIUSBSHARE_EnterConnect(void)
{
	 if (MMIAPICONNECTION_isGPRSSwitchedOff() 
	 	||(MMICC_GetCallNum()>0)
        //bug 1955022 begin
#ifndef UAL_NETWORKMGR_SUPPORT
	 	||!(MMIAPI_isPdpAvailable())
#endif
        //bug 1955022 end
	 	//||(FALSE == MMIAPISET_GetIsQueryCfu())
	 	)
	{
		SCI_TraceLow("[USB share] usb share ==gprs =%d,callnum=%d,pdp=%d,ss=%d",MMIAPICONNECTION_isGPRSSwitchedOff(),
		MMICC_GetCallNum(),MMIAPI_isPdpAvailable(),MMIAPISET_GetIsQueryCfu());
#ifdef ADULT_WATCH_SUPPORT
        MMIUSBSHARE_WatchOpenErrPrompt(TXT_SYS_WAITING_AND_TRY_LATER, pc_comm_connect_fail);
#else
	    MMIPUB_OpenAlertWarningWin(TXT_SYS_WAITING_AND_TRY_LATER);
#endif
	    return FALSE;
	}
	 
	if(!MMIUSBSHARE_ConnectPDP())
	{
#ifdef ADULT_WATCH_SUPPORT
        MMIUSBSHARE_WatchOpenErrPrompt(TXT_SYS_WAITING_AND_TRY_LATER, pc_comm_connect_fail);
#else
        MMIPUB_OpenAlertWarningWin(TXT_SYS_WAITING_AND_TRY_LATER);
#endif
        return FALSE;
	}

	MMK_CreateWin((uint32*)USB_SHARE_CONNECT_WIN_TAB,PNULL);
    //MMIDEFAULT_AllowTurnOffBackLight(TRUE);
    return TRUE;
}

LOCAL MMI_RESULT_E HandleUsbShareConnectWindow (
                                              MMI_WIN_ID_T       win_id,    // STARTUP CHARGE窗口的ID
                                              MMI_MESSAGE_ID_E      msg_id,     // 窗口的内部消息ID
                                              DPARAM                param       // 相应消息的参数
                                              )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    GUIANIM_INIT_DATA_T                 anim_init = {0};
    MMI_CONTROL_CREATE_T                anim_ctrl = {0};
    GUIANIM_CTRL_INFO_T                 ctrl_info = {0};
    GUIANIM_DATA_INFO_T                 data_info = {0};
    GUIANIM_DISPLAY_INFO_T              display_info = {0};
    uint16 screen_w=0;
    uint16 screen_h=0;
    GUI_RECT_T rect={0};
    GUI_COLOR_T bg_color=0x0861;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T anim_rect = MMITHEME_GetFullScreenBothRect();

            anim_rect.v_rect.top = anim_rect.v_rect.top + MMI_STATUSBAR_HEIGHT+MMI_TITLE_HEIGHT;
            anim_rect.v_rect.bottom = anim_rect.v_rect.bottom -MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT;

	     ctrl_info.is_ctrl_id = TRUE;
            ctrl_info.ctrl_id = MMI_CHARGE_ANIM_CTRL_ID;
            data_info.img_id = IMAGE_PC_CONNECTTING;
            display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
            anim_init.both_rect = anim_rect;

            //creat anim
            anim_ctrl.ctrl_id           = ctrl_info.ctrl_id;
            anim_ctrl.guid              = SPRD_GUI_ANIM_ID;
            anim_ctrl.init_data_ptr     = &anim_init;
            anim_ctrl.parent_win_handle = win_id;
            MMK_CreateControl(&anim_ctrl);
            GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
        }
        break;
    case MSG_FULL_PAINT:
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&screen_w,&screen_h);
        rect.right=screen_w-1;
        rect.bottom=screen_h-1;
        bg_color = MMI_BLACK_COLOR;

        LCD_FillRect(&lcd_dev_info,rect,bg_color);

        break;
    case MSG_CTL_ANIM_DISPLAY_IND:
    case MSG_CTL_ANIM_GIF_UPDATE:     
                if (MMK_IsFocusWin(win_id))
                {
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                }
        break;

       case MSG_APP_CANCEL:       
        //关闭网络连接窗口
        MMK_CloseWin(win_id);
        break;
        
        case MSG_APP_RED:      
        //关闭网络连接窗口
        MMK_CloseWin(win_id);
        
        result = MMI_RESULT_FALSE;
        break;
        
    case MSG_APP_FLIP:
        //关闭网络连接窗口
        MMK_CloseWin(win_id);
        
        result = MMI_RESULT_FALSE;
        break;

    case MSG_CLOSE_WINDOW:
        /*  fix bug 1345846
        if (USB_SERVICE_UCOM ==  USB_GetCurService())
        {       
            USB_StopService(USB_SERVICE_UCOM);
        }
        */
        SCI_TraceLow("[USB share] HandleUsbShareConnectWindow MSG_CLOSE_WINDOW");

        MMIUSBSHARE_Disable();
        UsbShareTimer(FALSE, 0);		
        g_usbshare_start = FALSE;
        
        if (retry_timer_id != 0)
        {
            MMK_StopTimer(retry_timer_id);            
            retry_timer_id = 0;
        }
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
