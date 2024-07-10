/*****************************************************************************
** File Name:      watch_notifycenter_shortcutwin.c                          *
** Author:         longwei.qiao                                              *
** Date:           02/20/2020                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe shortcut function           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 1/2021      longwei.qiao          Create                                  *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmi_common.h"
#include "mmiphone_export.h"
#include "mmiset_text.h"
#include "watch_notifycenter_text.h"
#include "watch_notifycenter_image.h"
#include "watch_notifycenter_position.h"
#include "watch_notifycenter_id.h"
#include "watch_notifycenter_shortcutwin.h"
#include "watch_slidewin.h"
#include "mmiset_export.h"
#include "mmiconnection_export.h"
#include "guiwin.h"
#include "watch_commonwin_export.h"
#ifdef  BLUETOOTH_SUPPORT
#include "mmibt_func.h"
#include "ual_bt.h"
#endif
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#ifdef MMI_GPS_SUPPORT
#include "mmigps_api.h"
#endif
#ifdef ADULT_WATCH_SUPPORT
#include "watch_flashlight.h"
#endif
#include "mmiidle_statusbar.h"

#ifdef BBM_ROLE_SUPPORT
#include "bbm_export.h"
#endif

#ifdef UAL_BT_HFU_SUPPORT
#include "ual_bt_hfu.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define WATCH_STATUSBAR_RECT      DP2PX_RECT(24, 76, 219, 95) //  Bug 1911410

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    SHORTCUT_ITEM_GPRS,
    SHORTCUT_ITEM_WIFI,
    SHORTCUT_ITEM_FLY,
    SHORTCUT_ITEM_GPS,
    SHORTCUT_ITEM_TORCH,
    SHORTCUT_ITEM_BT,
    SHORTCUT_ITEM_SEARCH,
    SHORTCUT_ITEM_MAX
}WATCH_SHORTCUT_ITEM_E;

typedef struct
{
    MMI_IMAGE_ID_T        open_image_id;
    MMI_IMAGE_ID_T        close_image_id;
    GUI_RECT_T            rect;
    BOOLEAN               is_open;
    BOOLEAN               is_wait;//是否需要等待单次操作的完成
    BOOLEAN               is_closed_by_fly;
    BOOLEAN               is_active_operate_in_fly;
    BOOLEAN(*item_handle_func)(void);
    void(*get_state_func)(void);
}WATCH_SHORTCUT_ITEM_T;

typedef struct
{
    GUI_RECT_T  rect;
    GUI_COLOR_T bg_color;
    WATCH_SHORTCUT_ITEM_T item[SHORTCUT_ITEM_MAX];
}WATCH_SHORTCUT_T;
/**--------------------------------------------------------------------------*
**                          LOCAL VARIABLES                                  *
**---------------------------------------------------------------------------*/
LOCAL WATCH_SHORTCUT_T s_notifycenter_shortcut_data = {0};
LOCAL GUI_POINT_T      s_shortcut_down_point = {0};
/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/

LOCAL GUI_RECT_T Shortcut_CalcItemRect(uint16 index)
{
    GUI_RECT_T total_rect   = SHORCUTWIN_SHORTCUT_RECT;
    uint16     margin_width = 0;
    uint16     split_width  = SHORTCUT_SPLIT_WIDTH;
    GUI_RECT_T item_rect = {0};
    if(index >= SHORTCUT_1ROW_MAX_ICONNUM + SHORTCUT_2ROW_MAX_ICONNUM)
    {
        //error index
        return item_rect;
    }
    else if(index > SHORTCUT_1ROW_MAX_ICONNUM - 1)//secend row
    {
        margin_width = SHORTCUT_2ROW_MARGIN;
        item_rect.left = margin_width + ((index - SHORTCUT_1ROW_MAX_ICONNUM) % SHORTCUT_2ROW_MAX_ICONNUM) * (SHORTCUT_ICON_WIDTH + split_width);
        item_rect.top = total_rect.bottom - SHORTCUT_ICON_HEIGHT + 1;
    }
    else//first row
    {
        margin_width = SHORTCUT_1ROW_MARGIN;
        item_rect.left = margin_width + (index % SHORTCUT_1ROW_MAX_ICONNUM) * (SHORTCUT_ICON_WIDTH + split_width);
        item_rect.top = total_rect.top;
    }
    item_rect.right = item_rect.left + SHORTCUT_ICON_WIDTH - 1;
    item_rect.bottom = item_rect.top + SHORTCUT_ICON_HEIGHT - 1;
    if(GUI_IsInvalidRect(item_rect))
    {
        SCI_MEMSET(&item_rect,0,sizeof(item_rect));
    }
    return item_rect;
}
LOCAL void Shortcut_GetShortcutItemState(void)
{
    uint8 i = 0;
    for(i = 0; i< SHORTCUT_ITEM_MAX; i++)
    {
        //get state
        if(PNULL != s_notifycenter_shortcut_data.item[i].get_state_func)
        {
            s_notifycenter_shortcut_data.item[i].get_state_func();
        }
    }
}
LOCAL void  Shortcut_HandleShortCutFun(
                        MMI_WIN_ID_T        win_id,
                        GUI_POINT_T        *tp_point_ptr
                        )
{
    uint8    i = 0;
    BOOLEAN  result = FALSE;
    BOOLEAN  is_fly_mode_on = MMIAPISET_GetFlyMode();
    for(i = 0; i< SHORTCUT_ITEM_MAX; i++)
    {
        if(GUI_PointIsInRect(*tp_point_ptr, s_notifycenter_shortcut_data.item[i].rect))
        {
            if(PNULL != s_notifycenter_shortcut_data.item[i].item_handle_func)
            {
                result = s_notifycenter_shortcut_data.item[i].item_handle_func();
            }

            if(is_fly_mode_on && result)
            {
                s_notifycenter_shortcut_data.item[i].is_active_operate_in_fly = TRUE;
            }
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL );
            return;
        }
    }
}
LOCAL BOOLEAN Shortcut_OpenOrCloseGPRS(void)
{
    BOOLEAN is_available = MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1);
    if(FALSE == is_available)
    {
        SCI_TRACE_LOW("Shortcut_OpenOrCloseGPRS :sim not available");
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_GPRS].close_image_id = res_aw_ic_qs_data_disable;
        return FALSE;
    }
#ifdef BBM_ROLE_SUPPORT
     //check watch role
     if(!BbmCurrentRoleIsBtPhone())
     {
         SCI_TRACE_LOW("Shortcut_GetGPRSState :not phone");
         s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_GPRS].is_open = FALSE;
         s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_GPRS].close_image_id = res_aw_ic_qs_data_disable;
         return FALSE;
     }
#endif
    if (MMIAPICONNECTION_GetGPRSSwitchStatus() == MMICONNECTION_SETTING_GPRS_SWITCH_ON)
    {
        MMIAPICONNECTION_SetGPRSSwitchStatus(MMICONNECTION_SETTING_GPRS_SWITCH_OFF);
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_GPRS].is_open = FALSE;
    }
    else
    {
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_GPRS].is_open = TRUE;
        MMIAPICONNECTION_SetGPRSSwitchStatus(MMICONNECTION_SETTING_GPRS_SWITCH_ON);
    }
    return TRUE;
}
LOCAL BOOLEAN Shortcut_OpenOrCloseWIFI(void)
{
#ifdef MMI_WIFI_SUPPORT
    if(FALSE == s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_WIFI].is_open)
    {
        if(FALSE == s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_WIFI].is_wait)
        {
            s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_WIFI].is_wait = TRUE;
            MMIAPIWIFI_On(FALSE);
             SCI_TRACE_LOW("Shortcut_OpenOrCloseWIFI :open");
        }
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_WIFI].is_open = TRUE;
    }
    else
    {
        if(FALSE == s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_WIFI].is_wait)
        {
            s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_WIFI].is_wait = TRUE;
            MMIAPIWIFI_Off();
            SCI_TRACE_LOW("Shortcut_OpenOrCloseWIFI :close");
        }
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_WIFI].is_open = FALSE;
    }
#endif
    return TRUE;
}
LOCAL BOOLEAN Shortcut_OpenOrCloseBT(void)
{
#ifdef BBM_ROLE_SUPPORT
    BOOLEAN is_bt_on = Bbm_GetBTOn();
    ual_bt_status_e result = UAL_BT_STATUS_SUCCESS;
    if (!is_bt_on)
    {
        BbmOpenBt();
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_BT].is_open = TRUE;
    }
    else
    {
        BbmCloseBt();
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_BT].is_open = FALSE;
    }

#endif
    return TRUE;
}
LOCAL BOOLEAN Shortcut_HandleBTMsg(ual_cms_msg_t param)
{
    BOOLEAN ret = TRUE;
    SCI_TRACE_LOW("Shortcut_HandleBTMsg msg_id =%x",param.msg_id);
    switch(param.msg_id)
    {
#ifdef UAL_BT_HFU_SUPPORT
        case UAL_BT_SLC_CONNECTED_IND:
            MAIN_SetIdleBlueToothState(ON_AND_CONNECT);
            MMK_SendMsg(WATCH_SHORTCUT_WIN_ID,MSG_FULL_PAINT,PNULL);
            break;
        case UAL_BT_SLC_DISCONNECTED_IND:
            MAIN_SetIdleBlueToothState(ON_AND_DISCONNECT);
            MMK_SendMsg(WATCH_SHORTCUT_WIN_ID,MSG_FULL_PAINT,PNULL);
            break;
#endif
        default:
            ret = FALSE;
            SCI_TRACE_LOW("Shortcut_HandleBTMsg default");
            break;
    }
    return ret;
}
LOCAL BOOLEAN Shortcut_HandleBBMMsg(uint32 msg_id, void* param)
{
    BOOLEAN ret = TRUE;
    SCI_TRACE_LOW("Shortcut_HandleBBMMsg msg_id =%x",msg_id);
    switch(msg_id)
    {
#ifdef BBM_ROLE_SUPPORT
        case BBM_MSG_BT_OPEN_SUCESS:
            //update blue tooth idle icon
            MMIBT_UpdateBtIdleIcon(TRUE,TRUE);
            SCI_TRACE_LOW("Shortcut_HandleBBMMsg opened");
            MMK_SendMsg(WATCH_SHORTCUT_WIN_ID,MSG_FULL_PAINT,PNULL);
            break;
        case BBM_MSG_BT_CLOSE_SUCESS:
             //update blue tooth idle icon
             MMIBT_UpdateBtIdleIcon(FALSE,FALSE);
             SCI_TRACE_LOW("Shortcut_HandleBBMMsg closed");
             MMK_SendMsg(WATCH_SHORTCUT_WIN_ID,MSG_FULL_PAINT,PNULL);
             break;
        case BBM_MSG_BT_OPEN_FAIL:
        case BBM_MSG_BT_CLOSE_FAIL:
        if(MMK_IsOpenWin(WATCH_SHORTCUT_WIN_ID))
        {
            MMI_STRING_T              tip_content  = {0};
            MMI_GetLabelTextByLang(TXT_ERROR, &tip_content);
            Adult_WatchCOM_NoteWin_1Line_Toast_Enter(NOTIFYCENTER_NOTE_WIN_ID,&tip_content,0,PNULL);
        }
        break;
#endif
        default:
            ret = FALSE;
            SCI_TRACE_LOW("Shortcut_HandleBBMMsg default");
            break;
    }
    return ret;
}

PUBLIC void NotifyCenter_RegisterBTService(void)
{
    uint32 p_handle = 0;
#ifdef BBM_ROLE_SUPPORT
    BbmRegister("NotifyCenter",Shortcut_HandleBBMMsg);
#endif
#ifdef UAL_BT_HFU_SUPPORT
    ual_bt_hfu_register(Shortcut_HandleBTMsg,&p_handle);
#endif
}
#ifdef MMI_WIFI_SUPPORT
LOCAL void Shortcut_PowerOnWifiCallback(MMIWIFI_NOTIFY_EVENT_E event_id,MMIWIFI_NOTIFY_PARAM_T* param_ptr)
{
    MMIWIFI_STATUS_E wifi_state =  MMIAPIWIFI_GetStatus();
    if(TRUE == s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_WIFI].is_wait)
    {
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_WIFI].is_wait = FALSE;
        if ((MMIWIFI_NOTIFY_EVENT_ON == event_id||MMIWIFI_NOTIFY_EVENT_OFF == event_id)
            && WIFISUPP_RESULT_SUCC != param_ptr->result)
        {
            if(WATCH_SHORTCUT_WIN_ID == MMK_GetFocusWinId())
            {
                MMI_STRING_T              tip_content  = {0};
                MMI_GetLabelTextByLang(TXT_ERROR, &tip_content);
                Adult_WatchCOM_NoteWin_1Line_Toast_Enter(NOTIFYCENTER_NOTE_WIN_ID,&tip_content,0,PNULL);
            }
        }
       
        if((TRUE == s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_WIFI].is_open)
            && (MMIWIFI_STATUS_OFF ==  wifi_state))
        {
            MMIAPIWIFI_On(FALSE);
            SCI_TRACE_LOW("Shortcut_PowerOnWifiCallback :need open");
        }
        else if((FALSE == s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_WIFI].is_open)
            && (MMIWIFI_STATUS_OFF !=  wifi_state))
        {
            MMIAPIWIFI_Off();
            SCI_TRACE_LOW("Shortcut_PowerOnWifiCallback :need close");
        }
        else
        {
            //do nothing
            SCI_TRACE_LOW("Shortcut_PowerOnWifiCallback :do nothing");
        }
    }
}
#endif

PUBLIC void NotifyCenter_RegisterWIFIService(void)
{
#ifdef MMI_WIFI_SUPPORT
    MMIWIFI_NOTIFY_INFO_T notify_info = {0};
    notify_info.module_id = WATCH_MODULE_NOTIFYCENTER;
    notify_info.notify_func = Shortcut_PowerOnWifiCallback;
    MMIAPIWIFI_RegNotifyInfo(&notify_info);
#endif
}
LOCAL BOOLEAN Shortcut_OpenOrCloseGPS(void)
{
#ifdef MMI_GPS_SUPPORT
    BOOLEAN is_open = FALSE;
    is_open = MMIGPS_OnOffState_Get();
    if(FALSE == is_open)
    {
        MMIGPS_Open();
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_GPS].is_open = TRUE;
    }
    else
    {
        MMIGPS_Close();
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_GPS].is_open = FALSE;
    }
#endif
    return TRUE;
}
LOCAL BOOLEAN Shortcut_OpenOrCloseTorch(void)
{
#ifdef ADULT_WATCH_SUPPORT
    AdultWatchFlashLight_MainWin_Enter();
    s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_TORCH].is_open = TRUE;
#endif
    return TRUE;
}
LOCAL BOOLEAN Shortcut_OpenOrCloseSearchPhone(void)
{
    return TRUE;
}
LOCAL void handleShortcutStateWhenCloseFly(void)
{
    uint16       i              = 0;
    s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_FLY].is_open = FALSE;
    //关飞行模式允许打开数据开关
    if(TRUE == MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1))
    {
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_GPRS].close_image_id = res_aw_ic_qs_data_off;
    }
    //打开被飞行模式关闭的应用
    for(i = 0; i< SHORTCUT_ITEM_MAX; i++)
    {
        if(!s_notifycenter_shortcut_data.item[i].is_active_operate_in_fly 
            && s_notifycenter_shortcut_data.item[i].is_closed_by_fly
            && (PNULL != s_notifycenter_shortcut_data.item[i].item_handle_func))
        {
            s_notifycenter_shortcut_data.item[i].item_handle_func();
            //clear fly flag
            s_notifycenter_shortcut_data.item[i].is_closed_by_fly = FALSE;
        }
        else
        {
            //clear fly flag
            s_notifycenter_shortcut_data.item[i].is_active_operate_in_fly = FALSE;
            s_notifycenter_shortcut_data.item[i].is_closed_by_fly = FALSE;
        }
    }
}
LOCAL void handleShortcutStateWhenOpenFly(void)
{
    s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_FLY].is_open = TRUE;
    //开飞行模式蓝牙会自动关闭，所以此处需要设置flag
    if(TRUE == s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_BT].is_open)
    {
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_BT].is_open = FALSE;
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_BT].is_closed_by_fly = TRUE;
    }
    //close wifi
    if(TRUE == s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_WIFI].is_open)
    {
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_WIFI].is_open = FALSE;
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_WIFI].is_closed_by_fly = TRUE;
    }
    //开飞行模式需要关闭数据开关，并且不允许打开数据开关
    if(TRUE == s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_GPRS].is_open)
    {
        MMIAPICONNECTION_SetGPRSSwitchStatus(MMICONNECTION_SETTING_GPRS_SWITCH_OFF);
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_GPRS].is_open = FALSE;
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_GPRS].is_closed_by_fly = TRUE;
    }
    s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_GPRS].close_image_id = res_aw_ic_qs_data_disable;
}
LOCAL void Shortcut_OpenPs(void)
{
#ifdef BBM_ROLE_SUPPORT
    //check watch role
    if(BbmCurrentRoleIsBtPhone())//power on ps only phone mode
#endif
    {
        // 激活协议栈
        MMIAPIPHONE_StartUpPsAccordingToSetting(FALSE, TRUE);
    }
}
LOCAL void Shortcut_ClosePs(void)
{
     uint32 dual_sys =0;
     BOOLEAN is_ps_ready = TRUE;
     MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
     if ((e_dualsys_setting & (0x01 << MN_DUAL_SYS_1))
       && MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
     {
         is_ps_ready = MMIAPIPHONE_GetIsPsReady(MN_DUAL_SYS_1);
         SCI_TRACE_LOW("Shortcut is_ps_ready %d",is_ps_ready);
     }
     if (is_ps_ready)
     {
        SCI_TRACE_LOW("Shortcut SetFlyMode is_ps_ready is TRUE");
        MMIAPIPHONE_DeactivePsAccordingToSetting();
     }
     else
     {
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_FLY].is_wait = FALSE;
        SCI_TRACE_LOW("Shortcut SetFlyMode is_ps_ready is FALSE");
     }
}
LOCAL BOOLEAN Shortcut_OpenOrCloseFlyMode(void)
{
    MMI_WIN_ID_T win_id         = WATCH_SHORTCUT_WIN_ID;
    BOOLEAN      is_fly_mode_on = MMIAPISET_GetFlyMode();
    if (!is_fly_mode_on)
    {
        //open
        is_fly_mode_on = TRUE;
        MMIAPISET_SetFlyMode(is_fly_mode_on);
        if(FALSE == s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_FLY].is_wait)//用户快速点击飞行模式的处理，单次开关PS完成后才进行下一次的开关PS操作
        {
            s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_FLY].is_wait = TRUE;
            SCI_TRACE_LOW("Shortcut open fly and close ps");
            Shortcut_ClosePs();
        }
        handleShortcutStateWhenOpenFly();
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_FLY].is_open = TRUE;
    }
    else
    {
         //close
         is_fly_mode_on = FALSE;
         MMIAPISET_SetFlyMode(is_fly_mode_on);
        if(FALSE == s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_FLY].is_wait)//用户快速点击飞行模式的处理，单次开关PS完成后才进行下一次的开关PS操作
        {
            s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_FLY].is_wait = TRUE;
            SCI_TRACE_LOW("Shortcut close fly and open ps");
            Shortcut_OpenPs();
        }
        handleShortcutStateWhenCloseFly();
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_FLY].is_open = FALSE;
    }
    return TRUE;
}
LOCAL BOOLEAN OpenOrCloseFlyMode(void)
{
    MMI_WIN_ID_T win_id         = WATCH_SHORTCUT_WIN_ID;
    BOOLEAN      is_fly_mode_on = MMIAPISET_GetFlyMode();
    if (!is_fly_mode_on)
    {
        //open
        is_fly_mode_on = TRUE;
        MMIAPISET_SetFlyMode(is_fly_mode_on);
        SCI_TRACE_LOW("Shortcut open fly and close ps---other");
        Shortcut_ClosePs();
        handleShortcutStateWhenOpenFly();
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_FLY].is_open = TRUE;
    }
    else
    {
         //close
        is_fly_mode_on = FALSE;
        MMIAPISET_SetFlyMode(is_fly_mode_on);
        SCI_TRACE_LOW("Shortcut close fly and open ps---other");
        Shortcut_OpenPs();
        handleShortcutStateWhenCloseFly();
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_FLY].is_open = FALSE;
    }
    return TRUE;
}

PUBLIC BOOLEAN NotifyCenter_OpenOrCloseFlyMode(void)
{
    return OpenOrCloseFlyMode();//系统其他地方调用开关飞行模式接口，不一定存在快速开关的情况，需要区分开
}
PUBLIC void NotifyCenter_ClearWaitStateForPs(void)
{
     SCI_TRACE_LOW("Shortcut clear ps wait state");
    s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_FLY].is_wait = FALSE;
}

LOCAL void Shortcut_GetGPRSState(void)
{
    BOOLEAN is_available = MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1);
    if(FALSE == is_available)
    {
        SCI_TRACE_LOW("Shortcut_OpenOrCloseGPRS :sim not available");
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_GPRS].is_open = FALSE;
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_GPRS].close_image_id = res_aw_ic_qs_data_disable;
        return;
    }
#ifdef BBM_ROLE_SUPPORT
     //check watch role
     if(!BbmCurrentRoleIsBtPhone())
     {
         SCI_TRACE_LOW("Shortcut_GetGPRSState :not phone");
         s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_GPRS].is_open = FALSE;
         s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_GPRS].close_image_id = res_aw_ic_qs_data_disable;
         return;
     }
#endif
    if (MMIAPICONNECTION_GetGPRSSwitchStatus() == MMICONNECTION_SETTING_GPRS_SWITCH_ON)
    {
         s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_GPRS].is_open = TRUE;
    }
    else
    {
         s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_GPRS].is_open = FALSE;
    }
}
LOCAL void Shortcut_GetWIFIState(void)
{
#ifdef MMI_WIFI_SUPPORT
    MMIWIFI_STATUS_E wifi_state =  MMIAPIWIFI_GetStatus();
    if(wifi_state == MMIWIFI_STATUS_OFF)
    {
           s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_WIFI].is_open = FALSE;
    }
    else
    {
           s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_WIFI].is_open = TRUE;
    }
#endif
}
LOCAL void Shortcut_GetBTState(void)
{
#ifdef  BBM_ROLE_SUPPORT
     if(TRUE == Bbm_GetBTOn())
     {
           s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_BT].is_open = TRUE;
     }
     else
     {
           s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_BT].is_open = FALSE;
     }
#endif
}
LOCAL void Shortcut_GetGPSState(void)
{
#ifdef MMI_GPS_SUPPORT
    BOOLEAN is_open = FALSE;
    is_open = MMIGPS_OnOffState_Get();
    if(TRUE == is_open)
    {
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_GPS].is_open = TRUE;
    }
    else
    {
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_GPS].is_open = FALSE;
    }
#endif
}
LOCAL void Shortcut_GetTorchState(void)
{
   s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_TORCH].is_open = FALSE;
}
LOCAL void Shortcut_GetSearchPhoneState(void)
{
   s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_SEARCH].is_open = FALSE;
}
LOCAL void Shortcut_GetFlyModeState(void)
{
    BOOLEAN is_fly_mode_on = MMIAPISET_GetFlyMode();
    if(FALSE == is_fly_mode_on)
    {
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_FLY].is_open = FALSE;
    }
    else
    {
        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_FLY].is_open = TRUE;
    }
}

LOCAL void Shortcut_InitShortcutData(void)
{
    GUI_RECT_T shortcut_rect = SHORCUTWIN_SHORTCUT_RECT;
    WATCH_SHORTCUT_T shortcut_info = {0};
    SCI_MEMCPY(&shortcut_info, &s_notifycenter_shortcut_data, sizeof(WATCH_SHORTCUT_T));
	shortcut_info.rect = shortcut_rect;
    shortcut_info.bg_color = MMI_BLACK_COLOR;
    //gprs
    shortcut_info.item[SHORTCUT_ITEM_GPRS].close_image_id = res_aw_ic_qs_data_off;
    shortcut_info.item[SHORTCUT_ITEM_GPRS].open_image_id = res_aw_ic_qs_data_on;
    shortcut_info.item[SHORTCUT_ITEM_GPRS].rect = Shortcut_CalcItemRect(SHORTCUT_ITEM_GPRS);
    shortcut_info.item[SHORTCUT_ITEM_GPRS].item_handle_func = Shortcut_OpenOrCloseGPRS;
    shortcut_info.item[SHORTCUT_ITEM_GPRS].get_state_func = Shortcut_GetGPRSState;
    //wifi
    shortcut_info.item[SHORTCUT_ITEM_WIFI].close_image_id = res_aw_ic_qs_wifi_off;
    shortcut_info.item[SHORTCUT_ITEM_WIFI].open_image_id = res_aw_ic_qs_wifi_on;
    shortcut_info.item[SHORTCUT_ITEM_WIFI].is_wait = FALSE;
    shortcut_info.item[SHORTCUT_ITEM_WIFI].rect = Shortcut_CalcItemRect(SHORTCUT_ITEM_WIFI);
    shortcut_info.item[SHORTCUT_ITEM_WIFI].item_handle_func = Shortcut_OpenOrCloseWIFI;
    shortcut_info.item[SHORTCUT_ITEM_WIFI].get_state_func = Shortcut_GetWIFIState;
    //fly
    shortcut_info.item[SHORTCUT_ITEM_FLY].close_image_id = res_aw_ic_qs_fly_off;
    shortcut_info.item[SHORTCUT_ITEM_FLY].open_image_id = res_aw_ic_qs_fly_on;
    shortcut_info.item[SHORTCUT_ITEM_FLY].is_wait = FALSE;
    shortcut_info.item[SHORTCUT_ITEM_FLY].rect = Shortcut_CalcItemRect(SHORTCUT_ITEM_FLY);
    shortcut_info.item[SHORTCUT_ITEM_FLY].item_handle_func = Shortcut_OpenOrCloseFlyMode;
    shortcut_info.item[SHORTCUT_ITEM_FLY].get_state_func = Shortcut_GetFlyModeState;
    //bt
    shortcut_info.item[SHORTCUT_ITEM_BT].close_image_id = res_aw_ic_qs_bluetooth_off;
    shortcut_info.item[SHORTCUT_ITEM_BT].open_image_id = res_aw_ic_qs_bluetooth_on;
    shortcut_info.item[SHORTCUT_ITEM_BT].rect = Shortcut_CalcItemRect(SHORTCUT_ITEM_BT);
    shortcut_info.item[SHORTCUT_ITEM_BT].item_handle_func = Shortcut_OpenOrCloseBT;
    shortcut_info.item[SHORTCUT_ITEM_BT].get_state_func = Shortcut_GetBTState;
    //gps
    shortcut_info.item[SHORTCUT_ITEM_GPS].close_image_id = res_aw_ic_qs_gps_off;
    shortcut_info.item[SHORTCUT_ITEM_GPS].open_image_id = res_aw_ic_qs_gps_on;
    shortcut_info.item[SHORTCUT_ITEM_GPS].rect = Shortcut_CalcItemRect(SHORTCUT_ITEM_GPS);
    shortcut_info.item[SHORTCUT_ITEM_GPS].item_handle_func = Shortcut_OpenOrCloseGPS;
    shortcut_info.item[SHORTCUT_ITEM_GPS].get_state_func = Shortcut_GetGPSState;
    //torch
    shortcut_info.item[SHORTCUT_ITEM_TORCH].close_image_id = res_aw_ic_qs_flashlight_off;
    shortcut_info.item[SHORTCUT_ITEM_TORCH].open_image_id = res_aw_ic_qs_flashlight_on;
    shortcut_info.item[SHORTCUT_ITEM_TORCH].rect = Shortcut_CalcItemRect(SHORTCUT_ITEM_TORCH);
    shortcut_info.item[SHORTCUT_ITEM_TORCH].item_handle_func = Shortcut_OpenOrCloseTorch;
    shortcut_info.item[SHORTCUT_ITEM_TORCH].get_state_func = Shortcut_GetTorchState;
    //search phone
    shortcut_info.item[SHORTCUT_ITEM_SEARCH].close_image_id = res_aw_ic_qs_findphone_disable;
    shortcut_info.item[SHORTCUT_ITEM_SEARCH].open_image_id = res_aw_ic_qs_findphone_disable;
    shortcut_info.item[SHORTCUT_ITEM_SEARCH].rect = Shortcut_CalcItemRect(SHORTCUT_ITEM_SEARCH);
    shortcut_info.item[SHORTCUT_ITEM_SEARCH].item_handle_func = Shortcut_OpenOrCloseSearchPhone;
    shortcut_info.item[SHORTCUT_ITEM_SEARCH].get_state_func = Shortcut_GetSearchPhoneState;

    SCI_MEMCPY(&s_notifycenter_shortcut_data, &shortcut_info, sizeof(WATCH_SHORTCUT_T));

}
LOCAL void Shortcut_DisplayBackground(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T       rect         = SHORCUTWIN_BG_RECT;
    GUI_LCD_DEV_INFO lcd_dev_info = {0,0};
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
}
LOCAL void Shortcut_DisplayDateInfo(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T                  date_rect           = SHORCUTWIN_DATE_RECT;
    uint8                       datestr[16]         = {0};
    wchar                       datewstr[16]        = {0};
    GUI_LCD_DEV_INFO            lcd_dev_info        = {0,0};
    GUISTR_STYLE_T              text_style          = {0};
    GUISTR_STATE_T              text_state          = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS;
    MMI_STRING_T                text                = {0};
    SCI_DATE_T                  date                = {0};
    //load date info
    TM_GetSysDate(&date);
    sprintf(datestr, "%d/%d", date.mon, date.mday);
    MMIAPICOM_StrToWstr(datestr,datewstr);
    text.wstr_len = MMIAPICOM_Wstrlen(datewstr);
    text.wstr_ptr = datewstr;
    //set display param
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font  = DP_FONT_32;
    text_style.font_color = MMI_WHITE_COLOR;
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    //draw
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *) &date_rect,       //the fixed display area
        (const GUI_RECT_T      *) &date_rect,       //用户要剪切的实际区域
        (const MMI_STRING_T     *)&text,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
         );
}
LOCAL void Shortcut_DisplayNetworkNameAndWeek(MMI_WIN_ID_T win_id)
{
    MMI_STRING_T            network_str     = {0};
    MMI_STRING_T            week_str        = {0};
    MMI_STRING_T            display_str     = {0};
    MMI_STRING_T            temp_str        = {0};
    GUI_LCD_DEV_INFO        lcd_dev_info    = {0,0};
    BOOLEAN                 is_fly_mode_on  = FALSE;
    GUISTR_STYLE_T          text_style      = {0};
    GUISTR_STATE_T          text_state      = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_SINGLE_LINE;
    GUI_RECT_T              disply_rect     = SHORCUTWIN_NETWORK_AND_WEEK_RECT;
    wchar                   opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN] = {0};
    wchar                   separative = ' ';
    MMI_STRING_T            temp1_str  = {0};
    SCI_DATE_T          date           = {0};
    MMI_TEXT_ID_T       week_table[]   = {
                                            TXT_CLOCK_SUNDAY,
                                            TXT_CLOCK_MONDAY,
                                            TXT_CLOCK_TUESDAY,
                                            TXT_CLOCK_WEDNESDAY,
                                            TXT_CLOCK_THURSDAY,
                                            TXT_CLOCK_FRIDAY,
                                            TXT_CLOCK_SATURDAY
									     };
#ifdef BBM_ROLE_SUPPORT
    //check watch role
    if(FALSE == BbmCurrentRoleIsBtPhone())
    {
        //load bt connection status info
        if(ual_bt_hfu_get_connect_status())
        {
            MMI_GetLabelTextByLang(TXT_SHORTCUT_PHONE_CONNECTED, &network_str);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_SHORTCUT_PHONE_DISCONNECTED, &network_str);
        }

    }
    else
#endif
    {
        //load network info
        is_fly_mode_on = MMIAPISET_GetFlyMode();
        if (is_fly_mode_on)
        {
            MMI_GetLabelTextByLang(TXT_SET_FLY_MODE, &network_str);
        }
        else if (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
        {
            SCI_MEMSET(&network_str, 0x00, sizeof(network_str));
            network_str.wstr_ptr = (wchar *)opn_spn_name;
            SCI_MEMSET(network_str.wstr_ptr, 0x00, sizeof(opn_spn_name));
            MMIAPIPHONE_GetSimAndServiceString(MN_DUAL_SYS_1, &network_str);
        }
        else
        {
           MMI_GetLabelTextByLang(TXT_SHORTCUT_NOSIM, &network_str);
        }

    }

    //load week info
    TM_GetSysDate(&date);
    MMI_GetLabelTextByLang(week_table[date.wday],&week_str);
    temp1_str.wstr_ptr = &separative;
    temp1_str.wstr_len = 1;
    //get display string
    MMIAPICOM_CatTwoString(&temp_str, &network_str, &temp1_str);
    MMIAPICOM_CatTwoString(&display_str, &temp_str, &week_str);
    MMIAPICOM_DestroyString(temp_str);
    //set display param
    text_style.align = ALIGN_HVMIDDLE;
    text_style.font  = DP_FONT_18;
    text_style.font_color = MMI_WHITE_COLOR;
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    //draw
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *) &disply_rect,       //the fixed display area
        (const GUI_RECT_T      *) &disply_rect,       //用户要剪切的实际区域
        (const MMI_STRING_T     *)&display_str,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
         );
    //free memory
     MMIAPICOM_DestroyString(display_str);
}
LOCAL void Shortcut_DisplayShortcutIcon(MMI_WIN_ID_T win_id)
{
    uint8                   i             = 0;
    GUI_LCD_DEV_INFO        lcd_dev_info  = {0,0};
    MMI_IMAGE_ID_T          icon_image_id = 0;
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    for(i=0; i < SHORTCUT_ITEM_MAX; i++)
    {
        //get image
        if (s_notifycenter_shortcut_data.item[i].is_open)
        {
            icon_image_id = s_notifycenter_shortcut_data.item[i].open_image_id;
        }
        else
        {
            icon_image_id = s_notifycenter_shortcut_data.item[i].close_image_id;
        }
        //draw icon
        GUIRES_DisplayImg(
                            PNULL,
                            &(s_notifycenter_shortcut_data.item[i].rect),
                            PNULL,
                            win_id,
                            icon_image_id,
                            &lcd_dev_info
                            );
    }
}
LOCAL MMI_RESULT_E HandleShortcutWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_RESULT_E      result    = MMI_RESULT_TRUE;
    GUI_BOTH_RECT_T   both_rect = MMITHEME_GetStatusBarBothRect();
    GUI_RECT_T        rect = WATCH_STATUSBAR_RECT;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            //init
            Shortcut_InitShortcutData();
            //get cur state
            Shortcut_GetShortcutItemState();
            //set statusbar rect
            both_rect.v_rect = rect;
            GUIWIN_SetStbBothRect(win_id, &both_rect);
        }
        break;

        case MSG_FULL_PAINT:
        {
#ifdef BBM_ROLE_SUPPORT
            //check watch role
            if(BbmCurrentRoleIsBtPhone())
            {
                //phone
                GUIWIN_SetStbItemVisible(MMI_WIN_ICON_SIGNAL, TRUE);
                GUIWIN_SetStbItemDisable(MMI_WIN_ICON_SIGNAL, FALSE);
            }
            else
            {
                //not phone ,don't need signal
                GUIWIN_SetStbItemVisible(MMI_WIN_ICON_SIGNAL, FALSE);
                GUIWIN_SetStbItemDisable(MMI_WIN_ICON_SIGNAL, TRUE);
            }
#endif
            //draw bg
            Shortcut_DisplayBackground(win_id);
            //draw fg
            Shortcut_DisplayDateInfo(win_id);
            Shortcut_DisplayNetworkNameAndWeek(win_id);
            Shortcut_DisplayShortcutIcon(win_id);
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T      point = {0};
            BOOLEAN          x_is_valid = FALSE;
            BOOLEAN          y_is_valid = FALSE;
            if(PNULL != param)
            {
                point.x = MMK_GET_TP_X(param);
                point.y = MMK_GET_TP_Y(param);
                x_is_valid = (abs(s_shortcut_down_point.x - point.x) < SHORTCUT_ICON_WIDTH ? TRUE:FALSE);
                y_is_valid = (abs(s_shortcut_down_point.y - point.y) < SHORTCUT_ICON_HEIGHT ? TRUE:FALSE);
                if(x_is_valid && y_is_valid)
                {
                    Shortcut_HandleShortCutFun(win_id,&point);
                }
            }
        }
        break;
        case MSG_TP_PRESS_DOWN:
        {
            if(PNULL != param)
            {
                s_shortcut_down_point.x = MMK_GET_TP_X(param);
                s_shortcut_down_point.y = MMK_GET_TP_Y(param);
            }
        }
        break;
#endif
        case MSG_SLIDEWIN_END:
        {
            GUIWIN_SetStbDropDownState(win_id, FALSE);
        }
        break;
        case MSG_SLIDEWIN_BEGIN:
        {
            GUIWIN_SetStbDropDownState(win_id, TRUE);
        }
        break;

        case MSG_LOSE_FOCUS:
        {
            MMK_CloseWin(win_id);
        }
        break;

        //bug1904633
        case APP_MN_PS_READY_IND:
        case APP_MN_PS_POWER_ON_CNF:
        case APP_MN_PS_POWER_OFF_CNF:
        {
            SCI_TRACE_LOW("Shortcut recive ps message fly wait = %d", s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_FLY].is_wait);
            if(TRUE == s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_FLY].is_wait)//Y用户快速点击时，由于ps是异步的，需要判断当前的fly状态，是否需要开关ps
            {
                s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_FLY].is_wait  = FALSE;
                if(TRUE == s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_FLY].is_open)
                {
                    if(TRUE == MMIAPIPHONE_GetIsPsReady(MN_DUAL_SYS_1))
                    {
                        SCI_TRACE_LOW("Shortcut: fly is on ,will close ps");
                        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_FLY].is_wait = TRUE;
                        Shortcut_ClosePs();
                    }
                }
                else
                {
                    if(TRUE == MMIAPIPHONE_GetIsPsDeactiveCnf(MN_DUAL_SYS_1))
                    {
                        SCI_TRACE_LOW("Shortcut: fly  is off,will open ps");
                        s_notifycenter_shortcut_data.item[SHORTCUT_ITEM_FLY].is_wait = TRUE;
                        Shortcut_OpenPs();
                    }
                }
            }
        }
        break;

        default:
        {
            result = MMI_RESULT_FALSE;
        }
        break;
    }
    return result;
}
 // window table of shortcut win
WINDOW_TABLE(WATCH_SHORTCUT_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleShortcutWinMsg),
    WIN_ID(WATCH_SHORTCUT_WIN_ID),
	WIN_STATUSBAR,
    END_WIN
};
/*****************************************************************************/
//  Description : enter Shortcut win
//  Paramter:
//          [In]:none
//          [Out]:none
//          [Return]:none
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void NotifyCenter_EnterShortcutWin(void)
{
    GUI_RECT_T roiRect = SHORTCUTWIN_ROIREGION;

    MMI_WIN_ID_T win_id = WATCH_SHORTCUT_WIN_ID;
    MMI_WIN_ID_T old_id = MMK_GetFocusWinId();

    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }

    MMK_CreateWin(WATCH_SHORTCUT_WIN_TAB, PNULL);

    do
    {
        uint32 handle = 0;
        tSlideWinInitData init = {0};
        init.win_id = win_id;
        init.win_before_id = old_id;
        init.direction = SLIDEWIN_DIRECTION_VERT;
        init.mode = SLIDEWIN_STARTUPMODE_TP;
        init.style = SLIDEWIN_STYLE_ENABLE_ROI | SLIDEWIN_STYLE_ENABLE_RANGE | SLIDEWIN_STYLE_ENABLE_FREE_WHEN_WINCLOSE;
        init.init_pos = SHORTCUTWIN_INIT_POS;
        init.stop_pos = SHORTCUTWIN_STOP_POS;
        init.close_pos = SHORTCUTWIN_END_POS;
        init.range_min = SHORTCUTWIN_INIT_POS;
        init.range_max = 0;
        init.roi_region = roiRect;
        handle = WatchSLIDEWIN_Create(&init);

        WatchSLIDEWIN_Start(handle);
    } while (0);
}
/*****************************************************************************/
//  Description : close Shortcut win
//  Paramter:
//          [In]:none
//          [Out]:none
//          [Return]:none
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void NotifyCenter_CloseShortcutWin(void)
{
    MMK_CloseWin(WATCH_SHORTCUT_WIN_ID);
}
/*****************************************************************************/
//  Description : Send Msg To Shortcut Win
//  Paramter:
//          [In]:none
//          [Out]:none
//          [Return]:none
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN NotifyCenter_SendMsgToShortcutWin(MMI_MESSAGE_ID_E msg_id ,DPARAM param_ptr)
{
    if (MMK_IsOpenWin(WATCH_SHORTCUT_WIN_ID) && WATCH_SHORTCUT_WIN_ID == MMK_GetFocusWinId())
    {
        return MMK_SendMsg(WATCH_SHORTCUT_WIN_ID, msg_id, param_ptr);
    }
    else
    {
        return (FALSE);
    }
}
