/*****************************************************************************
** File Name:      mmiidle_statusbar.c                                       *
** Author:                                                                   *
** Date:           11/07/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2011        jian.ma              Creat
******************************************************************************/

#define _MMIIDLE_STATUSBAR_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_idle_trc.h"
#include "mmiidle_statusbar.h"
#include "mmialarm_export.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#ifdef QQ_SUPPORT
#include "mmiqq_export.h"
#endif
#include "mmialarm_id.h"
#include "mms_image.h"
#include "mmisms_id.h"
#include "mmisms_image.h"
#include "mmicl_export.h"
#include "mmicc_id.h"

#include "mmiset_id.h"
#ifdef MCARE_V31_SUPPORT
#include "McfInterface.h"
#include "Mcare_Interface.h"
#endif
#include "mmiidle_func.h"
#include "mmidisplay_data.h"
#include "mmi_id.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#ifdef MMI_OTA_SUPPORT
#include "mmiota_image.h"
#endif
#include "mmiidle_cstyle.h"

#ifdef CMMB_SUPPORT
#include "mmimtv_export.h"
#endif
#ifdef ATV_SUPPORT
#include "mmiatv_export.h"
#endif
#include "mmimms_export.h"
#include "mmiidle_subwintab.h"
//#ifdef MMI_DUAL_BATTERY_SUPPORT
#include "dualbat_drvapi.h"
//#endif
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmidm_export.h"
#include "guistatusbar_scrollmsg.h"
#include "mmicl_internal.h"
#include "mmisms_read.h"
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#include "mmiwlan_image.h"
#endif
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#ifdef ASP_SUPPORT
#include "mmiasp_export.h"
#endif
#ifndef WIN32
#include "gpio_prod_api.h"
#endif
#include "mmipub.h"
#ifdef MMI_IDLE_FDN_ICON_SUPPORT
#include "mmifdn_export.h"
#endif

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
#include "mmisms_export.h"
#include "mmiota_export.h"
#endif
#ifdef MET_MEX_SUPPORT
#include "Met_mex_image.h"
#include "Met_mex_export.h"
#endif
#ifdef WRE_SUPPORT
#include "mmiwre_other.h"
#endif

#if defined(MMI_SMSCB_SUPPORT)
#include "mmisms_export.h"
#endif

#include "mmiconnection_export.h"

#ifdef SNS_PULLING_SUPPORT
#include "mmiidle_statusbar.h"
#include "mmisns_srv.h"
#endif //SNS_PULLING_SUPPORT
#include "mmi_event_api.h"
#include "mmisms_receive.h"
#ifdef DROPDOWN_NOTIFY_SHORTCUT
#include "watch_notifycenter.h"
#endif
#ifdef BBM_ROLE_SUPPORT
#include "bbm_export.h"
#endif

#ifdef SCREEN_SHAPE_CIRCULAR
#include "ual_tele_radio.h"
#endif
#include "ual_tele_sim.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifdef PDA_UI_DROPDOWN_WIN
#define MMIIDLE_DATE_LEN 12
#endif
#define MMIIDLE_BATTERY_PERCENT_LEN  4
#define ACTIVE_ICON_MAX (0x01 << MMI_DUAL_SYS_MAX)  
#define MMI_SIGNAL_IDLE_MODE_TIMEOUT 2000 //信号跳变平滑显示Timer 2000ms
#define MMI_SIGNAL_DEDICATED_MODE_TIMEOUT 1000 //信号跳变平滑显示Timer 1000
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifdef PRODUCT_DM
LOCAL BOOLEAN s_is_gprs_attach[MMI_DUAL_SYS_MAX] = {0}; 
#endif
#if defined CMCC_UI_STYLE || defined NET_ICON_UI_STYLE
LOCAL MMIPDP_NOTIFY_EVENT_E s_pdp_event[MMI_DUAL_SYS_MAX] = {0}; 
#endif
LOCAL uint8 s_signal_bound_timer[MMI_DUAL_SYS_MAX] = {0};

LOCAL const MMI_RATICON_G_PLMN_T s_raticon_table[] = {0};
#define MAX_RATICON_NUM (ARR_SIZE(s_raticon_table))

local uint32 s_tele_sim_register_handle = 0; //监听sim的handle

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
extern DBAT_STATE_INFO_T s_dbat_state_info = {0};
extern DBAT_BAT_INFO_T  dbat_info1 = {0};
extern DBAT_BAT_INFO_T  dbat_info2 = {0};
extern BOOLEAN MMI_GetEarphoneFlag(void);
#endif
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
/*****************************************************************************/
//  Description : 用于新短信，未接来电等事件的回调函数
//  Global resource dependence : none
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void NewEventCallback(MMIEVENT_INFO_T *event_info_ptr);
#endif

/*****************************************************************************/
// Description : 显示图标
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL void SetStatusBarInfo(
                            uint32 item_index
                            );
#ifdef MMI_CSP_SUPPORT
/*****************************************************************************/
// Description : 显示设置线路
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL BOOLEAN DisplaySetLineInfoFunc(void);
#endif
/*****************************************************************************/
// Description : 显示蓝牙
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL BOOLEAN DisplaySetBluetoothFunc(void);

/*****************************************************************************/
// Description : 显示sim1飞行模式
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL BOOLEAN DisplaySetSysmodeFunc(void);
#ifdef MMI_WIFI_SUPPORT
/*****************************************************************************/
// Description : open wifi win
// Global resource dependence : 
// Author: li.li
// Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayWifiMainFunc(void);
#endif
#ifdef PDA_UI_DROPDOWN_WIN
/*****************************************************************************/
// Description : 获得日期字符串
// Global resource dependence : 
// Author: paul.huang
// Note:
/*****************************************************************************/
LOCAL void StatusbarGetSysDate(
                               wchar *date_wstr, 
                               uint16 wlen
                               );
#endif
/*****************************************************************************/
// Description : 显示漫游图标
// Global resource dependence : 
// Author: wancan.you
// Note:
/*****************************************************************************/
LOCAL void SetRoamingInfo(uint32 item_index, BOOLEAN is_append);

/*****************************************************************************/
// Description : 显示运营商名称
//  Parameter: [In] dual_sys
//             [In] is_append
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
// Note:
/*****************************************************************************/
LOCAL void SetNetworkNameInfo( MN_DUAL_SYS_E dual_sys, BOOLEAN is_append);// Bug-1334911 显示运营商名称

#ifdef MMI_CSP_SUPPORT
/*****************************************************************************/
// Description : 获取线路设置的图标
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetLineRoadIconId(void);
#endif

/*****************************************************************************/
// Description : 获取环境设置的图标
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetEvnSetIconId(void);


/*****************************************************************************/
// Description : 获取线路设置的图标
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetLineCallForwardIconId(void);

/*****************************************************************************/
// Description : 设置GPS图标
// Global resource dependence :
// Author: Qi.liu1
// Note:2020-6-16
/*****************************************************************************/
LOCAL void SetStatusBarGpsInfo(BOOLEAN is_enable,BOOLEAN is_append);

/*****************************************************************************/
//  Description : 判断是否时号码快速查询
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetMessageDispImageId(void);

/*****************************************************************************/
// Description : 显示图标
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL void SetGprsInfo(
                                                MN_DUAL_SYS_E dual_sys,
                                                MMIPDP_NOTIFY_EVENT_E pdp_event,
                                                BOOLEAN is_append
                                                );
#ifdef PRODUCT_DM
/*****************************************************************************/
// Description : get ps active icon
// Global resource dependence : 
// Author: wancan.you
// Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetActiveIconId(MMIPHONE_DATA_SERVICE_TYPE_E data_service_type);

/*****************************************************************************/
// Description : get ps dating icon
// Global resource dependence : 
// Author: wancan.you
// Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetDatingIconId(MN_DUAL_SYS_E dual_sys, MMIPHONE_DATA_SERVICE_TYPE_E data_service_type);
#endif //PRODUCT_DM

/*****************************************************************************/
//  Description : handle rxlevel bound timer
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StartRxLevelTimer(
                                MN_DUAL_SYS_E dual_sys, 
                                BOOLEAN is_need_bound
                                );

#ifdef SNS_PULLING_SUPPORT
/*****************************************************************************
*  Description : set the icon display state of the idle window
*  Global resource dependence : 
*  Author: David.Chen
*  Note:
******************************************************************************/
LOCAL void MAIN_UpdateIdleState(uint32 item_index, BOOLEAN isDisplay);
#endif //SNS_PULLING_SUPPORT

#ifdef ASP_SUPPORT
/*****************************************************************************/
//  Description :InitIdleASPState
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
LOCAL  void InitIdleASPState(void);
#endif

#ifdef MMI_MULTISIM_COLOR_SUPPORT_STATUSICON                
/*****************************************************************************/
//  Description : Init satatus bar sim color
//  Global resource dependence : none
//  Author:  sam.hua
//  Note: 
/*****************************************************************************/
LOCAL void MMIMAIN_InitStatusBarOfSimColor(void);
#endif

#ifdef JAVA_SUPPORT
/*****************************************************************************/
//  Description : handle java
//  Global resource dependence : none
//  Author: Paul.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_JavaCallBack(void)
{
#ifdef JAVA_SUPPORT_MYRIAD
    MMIAPIJAVA_OpenTaskMgr();
#else
    MMIAPIJAVA_OpenMainMenu();
#endif

    return TRUE;
}
#endif

#ifdef MMI_IDLE_FDN_ICON_SUPPORT
/*****************************************************************************/
//  Description : get fdn display image
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetFDNDispImageId(void);
#ifdef FDN_SUPPORT
/*****************************************************************************/
// Description : display fdn func
// Global resource dependence : 
// Author: wancan.you
// Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayFDNFunc(void);
#endif
#endif

#ifdef QQ_SUPPORT
/*****************************************************************************/
//  Description : handle qq
//  Global resource dependence : none
//  Author: Paul.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIN_QqCallBack(void)
{
    return MMIAPIQQ_OpenMainWin();
}
#endif


/*****************************************************************************/
// Description : 显示sim1飞行模式
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL BOOLEAN DisplaySetSysmodeFunc(void)
{
    //单卡项目统一进入飞行模式的设置
#ifdef MCARE_V31_SUPPORT
	if(MCareV31_Exit_Confirm())
	{
		return FALSE;		
	}
#endif
#ifdef MMI_MULTI_SIM_SYS_SINGLE
    MMIAPISET_OpenSetFlyMode();
#else
    if(MMIAPISET_GetFlyMode())
    {
        MMIAPISET_OpenSetFlyMode();
    }
    else
    {
        MMIAPISET_OpenMultiSimSettingsWindow();
    }
#endif

    return TRUE;
}
#ifdef MMI_CSP_SUPPORT
/*****************************************************************************/
// Description : 显示设置线路
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL BOOLEAN DisplaySetLineInfoFunc(void)
{
    MMIAPISET_SetLineInfo();

    return TRUE;
}
#endif
#ifdef BLUETOOTH_SUPPORT
/*****************************************************************************/
// Description : 显示蓝牙
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL BOOLEAN DisplaySetBluetoothFunc(void)
{
    MMIAPIBT_SetBluetoothMode();

    return TRUE;
}
#endif
#ifdef MMI_WIFI_SUPPORT
/*****************************************************************************/
// Description : open wifi win
// Global resource dependence : 
// Author: li.li
// Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayWifiMainFunc(void)
{
    MMIAPIWIFI_OpenListWin();

    return TRUE;
}
#endif

#ifdef MMI_VOLTE_SUPPORT
LOCAL void StatusAreaSetVolteInfo(MN_DUAL_SYS_E dual_sys,uint32 item_index, BOOLEAN is_append);
#endif

/*****************************************************************************/
// Description : 显示闹钟列表
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL BOOLEAN DisplayAlarmFunc(void)
{
    BOOLEAN     result  =   FALSE;
    //打开闹钟列表
    MMIAPIALM_OpenMainWin();
    if (MMK_IsOpenWin(MMIALM_MAINWIN_ID))
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// Description : 显示短信、彩信或者push信箱
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL BOOLEAN DisplayMsgInboxFunc(void)
{
    BOOLEAN         result      =    FALSE;
    MMI_IMAGE_ID_T  image_id    =    GetMessageDispImageId();
#ifdef CMMB_SUPPORT
    if(IMAGE_IDLE_TITLE_ICON_EB == image_id)
    {
        //打开手机电视紧急广播列表
        MMIAPIMTV_OpenEBList();
    }
    else
#endif        
        if(IMAGE_NULL != image_id && IMAGE_IDLE_ICON_NEW_VM != image_id
        && !(IMAGE_IDLE_TITLE_ICON_MMS == image_id && MMISMS_FOLDER_SECURITY == MMIAPIMMS_GetDownloadingMMSFolderType())
        )
    {
       // 打开SMS列表窗口；
       
//#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
#ifdef MMI_SMS_CHAT_SUPPORT  
        MMIAPISMS_ReadNewSms(MMISMS_BOX_CHAT);
        return TRUE;
//#endif
#else
        MMIAPISMS_ReadNewSms(MMISMS_BOX_MT);

       if (MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID))
       {
           result = TRUE;
       }
#endif       
    }

   return result;
}

/*****************************************************************************/
// Description : 显示未接来电
// Global resource dependence : 
// Author: samboo.shen
// Note:2009-4-13
/*****************************************************************************/
LOCAL BOOLEAN DisplayMissedCallFunc(void)
{
    BOOLEAN     result  =   FALSE;


   // 打开未接来电窗口；
   WatchCL_ListWin_Enter();
#if 0
   MMIAPICL_OpenCallsListWindow(MMICL_CALL_MISSED);
   if(MMK_IsOpenWin(MMICL_MISSED_CHILD_WIN_ID))
   {
       //成功打开未接来电窗口，清空未接来电数目
       MMIAPICC_ClearMissedCallNum();
       result = TRUE;
   }
#endif
   return result;
}

/*****************************************************************************/
// Description : 显示呼叫转移菜单
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL BOOLEAN DisplayCallForwardMenuFunc(void)
{
    BOOLEAN     result  =   FALSE;

    MMIAPISET_OpenDivertSettingsWindow();
    if(MMK_IsOpenWin(MMISET_SET_CALL_WIN_ID))
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
// Description : 设置情景模式
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL BOOLEAN SetEnvModeFunc(void)
{
    BOOLEAN     result  =   FALSE;

    MMIENVSET_OpenMainMenuWindow();
    if(MMK_IsOpenWin(MMIENVSET_MAIN_MENU_WIN_ID))
    {
        result = TRUE;
    }

    return result;
}
/*****************************************************************************/
// Description : 显示电池充电界面
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
LOCAL BOOLEAN OpenBtryCapWinFunc(void)
{
    BOOLEAN     result  =   FALSE;

    //显示电池充电界面
    MMIAPIPHONE_OpenBtryCapWin();
    if (MMK_IsOpenWin(PHONE_STARTUP_BTRY_CAP_WIN_ID))
    {
        result = TRUE;
    }
    
    return result;
}

/*****************************************************************************/
// Description : 显示图标
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL void SetStatusBarInfo(
                            uint32 item_index
                            )
{
    MMI_IMAGE_ID_T          image_id        =   IMAGE_NULL;
    GUI_LCD_DEV_INFO        lcd_dev_info    =   {GUI_LCD_0, GUI_BLOCK_0};
    GUIWIN_STBDATA_ITEM_DATA_T  item_data   =   {0};

    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;

    item_data.icon_info.icon_num = 1;
    item_data.icon_info.icon_type = GUIWIN_STBDATA_ICON_NORMAL;
    item_data.icon_info.is_display = TRUE;
    item_data.cur_frame = 0;
    item_data.is_handle_callback = TRUE;

    switch(item_index)
    {
    case MMI_WIN_ICON_SIGNAL:
        MMIIDLE_SetSimStatusBarInfo(MN_DUAL_SYS_1, MMI_WIN_ICON_SIGNAL, TRUE);
        GUIWIN_SetStbItemValid(MMI_WIN_ICON_SIGNAL,TRUE);
        break;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    case MMI_WIN_ICON_SIGNAL_2:
        MMIIDLE_SetSimStatusBarInfo(MN_DUAL_SYS_2, MMI_WIN_ICON_SIGNAL_2, TRUE);        //MAIN LCD sim2信号量提示
        break;

#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
    case MMI_WIN_ICON_SIGNAL_3:
        MMIIDLE_SetSimStatusBarInfo(MN_DUAL_SYS_3, MMI_WIN_ICON_SIGNAL_3, TRUE);        //MAIN LCD sim2信号量提示
        break;
#endif

#if defined(MMI_MULTI_SIM_SYS_QUAD)
    case MMI_WIN_ICON_SIGNAL_4:
        MMIIDLE_SetSimStatusBarInfo(MN_DUAL_SYS_4, MMI_WIN_ICON_SIGNAL_4, TRUE);        //MAIN LCD sim2信号量提示
        break;
#endif
#endif
    case MMI_WIN_NETWORK_NAME:// Bug-1334911 显示运营商名称
        {
            uint32 i = 0;
            for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
            {
                SetNetworkNameInfo((MN_DUAL_SYS_E)i,TRUE);
            }
#if (!defined DROPDOWN_NOTIFY_SHORTCUT) && ((defined ADULT_WATCH_SUPPORT) || (!defined SCREEN_SHAPE_CIRCULAR))
            GUIWIN_SetStbItemValid(MMI_WIN_NETWORK_NAME,TRUE);
#endif
            break;
        }
	case MMI_WIN_ICON_GPS:
        {
            SetStatusBarGpsInfo(FALSE,TRUE);
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_GPS, TRUE);
            GUIWIN_SetStbItemValid(MMI_WIN_ICON_GPS,TRUE);
            break;
	    }
    case MMI_WIN_ICON_ROAMING:
        SetRoamingInfo(MMI_WIN_ICON_ROAMING, TRUE);
#if (defined ADULT_WATCH_SUPPORT) || (!defined SCREEN_SHAPE_CIRCULAR)
        GUIWIN_SetStbItemValid(MMI_WIN_ICON_ROAMING,TRUE);  //bug1958833
#endif
        break;

#ifdef PRODUCT_DM
#if !defined CMCC_UI_STYLE && !defined NET_ICON_UI_STYLE
    case MMI_WIN_ICON_GPRS:
        SetGprsInfo(MN_DUAL_SYS_1, MMIPDP_NOTIFY_EVENT_DETTACH, TRUE);
        break;
#endif
#endif

#if defined(CMMB_SUPPORT) || defined(ATV_SUPPORT)
    case MMI_WIN_ICON_CMMB_SIGNAL:
        {
            uint8          signal_level = 0;
            
            if (MMIAPIMTV_GetTVSignalLevel(&signal_level))
            {
                image_id = MMIAPIMTV_GetTVStatusImageID(signal_level);
            }
            item_data.is_handle_callback = FALSE;
            item_data.app_handle_func = PNULL;
            item_data.icon_info.icon_array[0] = image_id;
            GUIWIN_AppendStbItemData(&item_data);
            
            if (image_id != IMAGE_NULL)
            {
                GUIWIN_SetStbItemVisible(MMI_WIN_ICON_CMMB_SIGNAL, TRUE);
            }
            else
            {
                GUIWIN_SetStbItemVisible(MMI_WIN_ICON_CMMB_SIGNAL, FALSE);
            }
#if defined MMI_ISTYLE_SUPPORT                
            if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
            {
                GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_CMMB_SIGNAL, TRUE);
            }
            else
#endif				
            {
                GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_CMMB_SIGNAL, FALSE);
            }   
        }
        break;
#endif
#ifdef JAVA_SUPPORT
    case MMI_WIN_JAVA: //JAVA
        item_data.icon_info.icon_array[0] = IMAGE_IDLE_TITLE_ICON_JAVA;
        item_data.app_handle_func = MMIIDLE_JavaCallBack;
        GUIWIN_AppendStbItemData(&item_data);
        #ifdef JAVA_SUPPORT_IA
        if(MMIAPIJAVA_IsJavaBackgroundRuning())
        #else
        if(MMIAPIJAVA_IsJavaRuning())
        #endif
        {
            MAIN_UpdateIdleJavaState(TRUE);
        }
        else
        {
            MAIN_UpdateIdleJavaState(FALSE);
        }
        break; 
#endif

#ifdef SNS_PULLING_SUPPORT
    case MMI_WIN_ICON_SNS_FB:
        item_data.icon_info.icon_array[0] = IMAGE_IDLE_TITLE_ICON_SNS_FB;
        item_data.app_handle_func = PNULL;
        GUIWIN_AppendStbItemData(&item_data);
        break;
        
    case MMI_WIN_ICON_SNS_TWIT:
        item_data.icon_info.icon_array[0] = IMAGE_IDLE_TITLE_ICON_SNS_TWIT;
        item_data.app_handle_func = PNULL;
        GUIWIN_AppendStbItemData(&item_data);
        break;
#endif //SNS_PULLING_SUPPORT

    case MMI_WIN_ICON_CALLING:  
        item_data.icon_info.icon_array[0] = IMAGE_IDLE_TITLE_ICON_CALLING;
        item_data.app_handle_func = PNULL;
        GUIWIN_AppendStbItemData(&item_data);
        
        if((TRUE==MMIIDLE_GetIdleWinInfo()->is_calling))
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_CALLING, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_CALLING, FALSE);
        }
#if defined MMI_ISTYLE_SUPPORT            
        if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_CALLING, TRUE);
        }
        else
#endif			
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_CALLING, FALSE);
        }   

        break;
        
    case MMI_WIN_ICON_MISSED_CALL:
        item_data.icon_info.icon_array[0] = IMAGE_IDLE_TITLE_ICON_MISSED_CALL;
        item_data.app_handle_func = DisplayMissedCallFunc;
        GUIWIN_AppendStbItemData(&item_data);
        
        if((TRUE==MMIIDLE_GetIdleWinInfo()->is_missed_call))
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_MISSED_CALL, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_MISSED_CALL, FALSE);
        }
#if defined MMI_ISTYLE_SUPPORT
        if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_MISSED_CALL, TRUE);
        }
        else
#endif			
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_MISSED_CALL, FALSE);
        }   

        break;
    case MMI_WIN_ICON_ALARM:  // 闹钟 2
        image_id = IMAGE_IDLE_TITLE_ICON_CLOCK;
        item_data.icon_info.icon_array[0] = image_id;
        item_data.app_handle_func = DisplayAlarmFunc;
        GUIWIN_AppendStbItemData(&item_data);
        
        if((TRUE==MMIIDLE_GetIdleWinInfo()->is_alarm))
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_ALARM, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_ALARM, FALSE);
        }
 #if defined MMI_ISTYLE_SUPPORT                      
        if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
           GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_ALARM, TRUE);
        }
        else
#endif			
        {
           GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_ALARM, TRUE);
        }
#ifdef DROPDOWN_NOTIFY_SHORTCUT
        GUIWIN_SetStbItemValid(MMI_WIN_ICON_ALARM,TRUE);
#endif

        break;

    case MMI_WIN_ICON_MESSAGE: // 消息
        image_id = GetMessageDispImageId();
        item_data.icon_info.icon_array[0] = image_id;
        item_data.app_handle_func = DisplayMsgInboxFunc;
        GUIWIN_AppendStbItemData(&item_data);
        
        if (image_id != IMAGE_NULL)
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_MESSAGE, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_MESSAGE, FALSE);
        }           
 #if defined MMI_ISTYLE_SUPPORT           
        if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_MESSAGE, TRUE);
        }
        else
#endif			
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_MESSAGE, FALSE);
        }   
        break;

#ifdef MMI_IDLE_FDN_ICON_SUPPORT
#ifdef FDN_SUPPORT       
    case MMI_WIN_ICON_FDN: // FDN
        image_id = GetFDNDispImageId();
        item_data.icon_info.icon_array[0] = image_id;
        item_data.app_handle_func = DisplayFDNFunc;
        GUIWIN_AppendStbItemData(&item_data);
        if (image_id != IMAGE_NULL)
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_FDN, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_FDN, FALSE);
        }
        break;
#endif
#endif

    case MMI_WIN_ICON_CALL_FORWARD: // 呼叫转移
        image_id = GetLineCallForwardIconId();
        item_data.icon_info.icon_array[0] = image_id;
        item_data.app_handle_func = DisplayCallForwardMenuFunc;
        GUIWIN_AppendStbItemData(&item_data);
        
        if (image_id != IMAGE_NULL)
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_CALL_FORWARD, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_CALL_FORWARD, FALSE);
        } 
#if defined MMI_ISTYLE_SUPPORT            
        if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_CALL_FORWARD, TRUE);
        }
        else
#endif			
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_CALL_FORWARD, FALSE);
        }   

        break;
#ifdef MMI_CSP_SUPPORT
    case MMI_WIN_ICON_LINE_INFO: // 线路
        image_id = GetLineRoadIconId();
        item_data.icon_info.icon_array[0] = image_id;
        item_data.app_handle_func = DisplaySetLineInfoFunc;
        GUIWIN_AppendStbItemData(&item_data);
        
        if (image_id != IMAGE_NULL)
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_LINE_INFO, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_LINE_INFO, FALSE);
        }
 #if defined MMI_ISTYLE_SUPPORT       
        if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_LINE_INFO, TRUE);
        }
        else
#endif			
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_LINE_INFO, FALSE);
        }   

        break;
#endif //MMI_CSP_SUPPORT
        
#ifdef BLUETOOTH_SUPPORT

    case MMI_WIN_ICON_BLUE_TOOTH: //  蓝牙
#ifdef BT_DIALER_SUPPORT
        if ( ON_AND_DISCONNECT == MMIIDLE_GetIdleWinInfo()->bluetooth_state)
        {
            image_id = (MMI_IMAGE_ID_T)IMAGE_IDLE_TITLE_ICON_BT_HIDDEN; 
            //blue tooth is power on but disconnect 
        }
        else if ( ON_AND_CONNECT == MMIIDLE_GetIdleWinInfo()->bluetooth_state)
        {
            image_id = (MMI_IMAGE_ID_T)IMAGE_IDLE_TITLE_ICON_BT_VISIBLE; 
            //blue tooth is power on and connect 
        }
#else
        if ( ON_AND_VISIBLE == MMIIDLE_GetIdleWinInfo()->bluetooth_state)
        {
            image_id = (MMI_IMAGE_ID_T)res_stat_bluetooth; 
            //blue tooth is power on and visible
        }
        else if ( ON_AND_HIDDEN == MMIIDLE_GetIdleWinInfo()->bluetooth_state)
        {
            image_id = (MMI_IMAGE_ID_T)IMAGE_IDLE_TITLE_ICON_BT_HIDDEN; 
            //blue tooth is power on and hidden 
        }
#endif
        else
        {
            image_id = (MMI_IMAGE_ID_T)IMAGE_NULL;
        }

        item_data.icon_info.icon_array[0] = image_id;
        item_data.app_handle_func = DisplaySetBluetoothFunc;
        
        GUIWIN_AppendStbItemData(&item_data);
        
        if (image_id != IMAGE_NULL)
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_BLUE_TOOTH, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_BLUE_TOOTH, FALSE);
        }
#if defined MMI_ISTYLE_SUPPORT           
        if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_BLUE_TOOTH, TRUE);
        }
        else
#endif			
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_BLUE_TOOTH, TRUE);
        }   
        GUIWIN_SetStbItemValid(MMI_WIN_ICON_BLUE_TOOTH,TRUE);
        break;
#endif
        
#ifdef MMI_WIFI_SUPPORT
    case MMI_WIN_ICON_WIFI:
#if 0
        if (MMIAPIWIFI_GetStatus() != MMIWIFI_STATUS_OFF)
        {
            image_id = IMAGE_WIFI_ON_ICON;
        }
        else
        {
            image_id = IMAGE_NULL;
        }
        
        item_data.icon_info.icon_array[0] = image_id;
        item_data.app_handle_func = DisplayWifiMainFunc;
        GUIWIN_AppendStbItemData(&item_data);
        
        if (image_id != IMAGE_NULL)
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_WIFI, TRUE);          
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_WIFI, FALSE);
        }
#if defined MMI_ISTYLE_SUPPORT                    
        if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_WIFI, TRUE);
        }
        else
#endif			
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_WIFI, FALSE);
        }   
#endif
        {
            uint32  iconIndex                =  0;
            uint32  wifi_icon_id[5] =        {res_stat_wifi_0,
                                                              res_stat_wifi_1,
                                                              res_stat_wifi_2,
                                                              res_stat_wifi_3,
                                                              res_stat_wifi_4};

            for (iconIndex = 0; iconIndex < 5; iconIndex++)
            {
                item_data.icon_info.icon_array[iconIndex] = wifi_icon_id[iconIndex];
            }

            item_data.app_handle_func = DisplayWifiMainFunc;
            item_data.icon_info.icon_num = 5;
            item_data.icon_info.icon_type = GUIWIN_STBDATA_ICON_NORMAL;

            GUIWIN_AppendStbItemData(&item_data);
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_WIFI, TRUE);
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_WIFI, TRUE);
            if (MMIAPIWIFI_GetStatus() != MMIWIFI_STATUS_OFF)
            {
                GUIWIN_SetStbItemVisible(MMI_WIN_ICON_WIFI, TRUE);
            }
            else
            {
                GUIWIN_SetStbItemVisible(MMI_WIN_ICON_WIFI, FALSE);
            }
            GUIWIN_SetStbItemValid(MMI_WIN_ICON_WIFI,TRUE);
            break;
        }
#endif

    case MMI_WIN_ICON_ENVSET:     // 情景模式

        image_id = GetEvnSetIconId();

        item_data.icon_info.icon_array[0] = image_id;
        item_data.app_handle_func = SetEnvModeFunc;
        GUIWIN_AppendStbItemData(&item_data);
        
        if (image_id != IMAGE_NULL)
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_ENVSET, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_ENVSET, FALSE);
        }
#if defined MMI_ISTYLE_SUPPORT                    
        if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_ENVSET, TRUE);
        }
        else
#endif			
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_ENVSET, FALSE);
        }   

        break;

    case MMI_WIN_ICON_EARPHONE: //耳机
        item_data.icon_info.icon_array[0] = IMAGE_IDLE_TITLE_ICON_EARPHONE;
        item_data.app_handle_func = PNULL;
        GUIWIN_AppendStbItemData(&item_data);
        
        if(GPIO_CheckHeadsetStatus())
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_EARPHONE, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_EARPHONE, FALSE);
        }
#if defined MMI_ISTYLE_SUPPORT       
        if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_EARPHONE, TRUE);
        }
        else
#endif			
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_EARPHONE, FALSE);
        }   
            
        break;
#ifndef MMI_IDLE_MINI_SUPPORT
    case MMI_WIN_TEXT_TIME:
        {
            uint16 w_str[MMIIDLE_TIME_LEN] = {0};
            // uint16 wstr_len = 0;

            MAIN_StatusbarGetSysTime(w_str, MMIIDLE_TIME_LEN);
            item_data.icon_info.is_display = FALSE;
            item_data.text_info.is_display = TRUE;
            item_data.app_handle_func = (GUIWIN_STBDATA_APP_HANDLE_FUNC )MMIAPISET_SetTime;
            item_data.text_info.wstr_len = (uint32)MMIAPICOM_Wstrlen(w_str);
            
            // 取最小值，防止越界
            item_data.text_info.wstr_len = MIN(item_data.text_info.wstr_len, MMIIDLE_TIME_LEN);
            item_data.text_info.wstr_len = MIN(item_data.text_info.wstr_len, MMISTATUSBAR_ITEM_TEXT_LEN);

            // cp字符串
            SCI_MEMCPY(item_data.text_info.wstr_ptr, w_str, sizeof(wchar)*item_data.text_info.wstr_len);

            item_data.text_info.font_color = MMI_WHITE_COLOR;//MMI_GRAY_WHITE_COLOR; CRNEWMS00203710
            item_data.text_info.align = ALIGN_HVMIDDLE; //居中 edit for CRNEWMS00135505
            item_data.text_info.font_type = MMI_STATUS_FONT_TYPE;
            
            GUIWIN_AppendStbItemData(&item_data);

            GUIWIN_SetStbItemVisible(MMI_WIN_TEXT_TIME, TRUE);
            //GUIWIN_SetStbItemCenterAlign(MMI_WIN_TEXT_TIME, FALSE);
            GUIWIN_SetStbItemRightAlign(MMI_WIN_TEXT_TIME, TRUE);
#if defined MMI_ISTYLE_SUPPORT               
            if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
            {
                GUIWIN_SetStbItemVisible(MMI_WIN_TEXT_TIME, TRUE);
                GUIWIN_SetStbItemCenterAlign(MMI_WIN_TEXT_TIME, TRUE);
            }    
#endif			
        }
        break;
#endif 

#ifdef MMI_VOLTE_SUPPORT
    case MMI_WIN_ICON_VOLTE:
        StatusAreaSetVolteInfo(MN_DUAL_SYS_1, MMI_WIN_ICON_VOLTE, TRUE);

#ifdef SCREEN_SHAPE_CIRCULAR
        GUIWIN_SetStbItemValid(MMI_WIN_ICON_VOLTE,TRUE);
#endif
#if (defined MMI_ISTYLE_SUPPORT)
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_VOLTE, TRUE);
#endif
        break;
        
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    case MMI_WIN_ICON_VOLTE_2:
        StatusAreaSetVolteInfo(MN_DUAL_SYS_2, MMI_WIN_ICON_VOLTE_2, TRUE);
#if defined MMI_ISTYLE_SUPPORT               
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_VOLTE_2, TRUE);
#endif
        break;
#endif
#endif

#ifdef PDA_UI_DROPDOWN_WIN
    case MMI_WIN_TEXT_DATE:
        {
            uint16 w_str[MMIIDLE_DATE_LEN] = {0};

            StatusbarGetSysDate(w_str, MMIIDLE_DATE_LEN);
            item_data.icon_info.is_display = FALSE;
            item_data.text_info.is_display = TRUE;
            item_data.app_handle_func = (GUIWIN_STBDATA_APP_HANDLE_FUNC )MMIAPISET_SetDate;
            item_data.text_info.wstr_len = (uint32)MMIAPICOM_Wstrlen(w_str);
            
            // 取最小值，防止越界
            item_data.text_info.wstr_len = MIN(item_data.text_info.wstr_len, MMIIDLE_DATE_LEN);
            item_data.text_info.wstr_len = MIN(item_data.text_info.wstr_len, MMISTATUSBAR_ITEM_TEXT_LEN);

            // cp字符串
            SCI_MEMCPY(item_data.text_info.wstr_ptr, w_str, sizeof(wchar)*item_data.text_info.wstr_len);

            item_data.text_info.font_color = MMI_WHITE_COLOR;//MMI_GRAY_WHITE_COLOR; CRNEWMS00203710
            item_data.text_info.align = ALIGN_HVMIDDLE; //居中 edit for CRNEWMS00135505
            item_data.text_info.font_type = MMI_STATUS_FONT_TYPE;
            
            GUIWIN_AppendStbItemData(&item_data);
            GUIWIN_SetStbItemVisible(MMI_WIN_TEXT_DATE, FALSE);
            GUIWIN_SetStbItemRightAlign(MMI_WIN_TEXT_DATE, FALSE);
        }
        break;
#endif

//==========MEX START ==================

#ifdef MET_MEX_QQ_SUPPORT  
    case MEX_MASTER_QQ_STATUS:      
    case MEX_SLAVE_QQ_STATUS: //QQ
        {
            uint32 icon_index = 0;
            item_data.icon_info.icon_array[icon_index] = MexInterface_GetStbIcon(item_index,STB_IMAGE);
            item_data.app_handle_func = (GUIWIN_STBDATA_APP_HANDLE_FUNC )MetMexQQOpenWin;
            item_data.icon_info.icon_num = 1;
            item_data.cur_frame = 0;
            
            GUIWIN_AppendStbItemData(&item_data);
            
            if(1 == MexInterface_GetStbIcon(item_index,STB_ISSHOW))
            {
                GUIWIN_SetStbItemVisible(item_index,TRUE);
            }
            else
            {
                GUIWIN_SetStbItemVisible(item_index,FALSE);           
            }    
#if defined MMI_ISTYLE_SUPPORT			
            if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
            {
                GUIWIN_SetStbItemRightAlign(item_index, TRUE);
            }
            else
#endif				
            {
                GUIWIN_SetStbItemRightAlign(item_index, FALSE);
            }   
        }
        break;
#endif
#ifdef MET_MEX_DOUBLE_QQ_SUPPORT
  case MEX_MASTER_DOUBLE_QQ_STATUS:		
    	{    		
    		uint32 icon_index = 0;
            item_data.icon_info.icon_array[icon_index] = MexInterface_GetStbIcon(item_index,STB_IMAGE);
    		item_data.app_handle_func = (GUIWIN_STBDATA_APP_HANDLE_FUNC )MetMexQQ1OpenWin;
    		item_data.icon_info.icon_num = 1;
    		item_data.cur_frame = 0;
            GUIWIN_AppendStbItemData(&item_data);
            
            if(1 == MexInterface_GetStbIcon(item_index,STB_ISSHOW))
            {
                GUIWIN_SetStbItemVisible(item_index,TRUE);
            }
            else
            {
                GUIWIN_SetStbItemVisible(item_index,FALSE);
		    }
#if defined MMI_ISTYLE_SUPPORT			
            if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
            {
                GUIWIN_SetStbItemRightAlign(item_index, TRUE);
            }
            else
#endif				
            {
                GUIWIN_SetStbItemRightAlign(item_index, FALSE);
            }              
        }            
	break;
	case MEX_SLAVE_DOUBLE_QQ_STATUS: //QQ  
		{			
			uint32 icon_index = 0;
            item_data.icon_info.icon_array[icon_index] = MexInterface_GetStbIcon(item_index,STB_IMAGE);
			item_data.app_handle_func = (GUIWIN_STBDATA_APP_HANDLE_FUNC )MetMexQQ2OpenWin;
			item_data.icon_info.icon_num = 1;
			item_data.cur_frame = 0;
			GUIWIN_AppendStbItemData(&item_data);
		
            if(1 == MexInterface_GetStbIcon(item_index,STB_ISSHOW))
			{
				GUIWIN_SetStbItemVisible(item_index, TRUE);
			}
			else
			{
				GUIWIN_SetStbItemVisible(item_index, FALSE);
			}
#if defined MMI_ISTYLE_SUPPORT			
            if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
            {
                GUIWIN_SetStbItemRightAlign(item_index, TRUE);
            }
            else
#endif				
            {
                GUIWIN_SetStbItemRightAlign(item_index, FALSE);
            }   
		}
        break;

#endif
#ifdef MET_MEX_MSN_SUPPORT
	   	case MEX_MSN_STATUS: //MSN
        {            
            uint32 icon_index = 0;
            item_data.icon_info.icon_array[icon_index] = MexInterface_GetStbIcon(item_index,STB_IMAGE);
            item_data.app_handle_func = (GUIWIN_STBDATA_APP_HANDLE_FUNC )MetMexMSNOpenWin;
            item_data.icon_info.icon_num = 1;
            item_data.cur_frame = 0;
            GUIWIN_AppendStbItemData(&item_data);

            if(1 == MexInterface_GetStbIcon(item_index,STB_ISSHOW))
            {
               GUIWIN_SetStbItemVisible(item_index, TRUE);
            }
            else
            {
               GUIWIN_SetStbItemVisible(item_index, FALSE);
            }
#if defined MMI_ISTYLE_SUPPORT			
            if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
            {
                GUIWIN_SetStbItemRightAlign(item_index, TRUE);
            }
            else
#endif				
            {
                GUIWIN_SetStbItemRightAlign(item_index, FALSE);
            }   
        }

	   	break;
#endif
#ifdef MET_MEX_FETION_SUPPORT
	   	case MEX_FETION_STATUS: //FETION
        {
            uint32 icon_index = 0;
            item_data.icon_info.icon_array[icon_index] = MexInterface_GetStbIcon(item_index,STB_ICON);
            item_data.app_handle_func = (GUIWIN_STBDATA_APP_HANDLE_FUNC )MetMexFETIONOpenWin;
            item_data.icon_info.icon_num = 1;
            item_data.cur_frame = 0;
            GUIWIN_AppendStbItemData(&item_data);

            if(1 == MexInterface_GetStbIcon(item_index,STB_ISSHOW))
            {
               GUIWIN_SetStbItemVisible(item_index, TRUE);
            }
            else
            {
               GUIWIN_SetStbItemVisible(item_index, FALSE);
            }
#if defined MMI_ISTYLE_SUPPORT			
            if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
            {
                GUIWIN_SetStbItemRightAlign(item_index, TRUE);
            }
            else
#endif				
            {
                GUIWIN_SetStbItemRightAlign(item_index, FALSE);
            }   
        }
         
	   	break;
#endif


//==========MEX END==================

#ifdef MRAPP_SUPPORT
    case MMI_WIN_ICON_MRAPP: //冒泡
        item_data.icon_info.icon_array[0] = IMG_MRAPP_ONLINE;
        item_data.app_handle_func = MMIMRAPP_activeMrapp;
        GUIWIN_AppendStbItemData(&item_data);
        if(MMIMRAPP_GetVMState() == VM_BACK_RUN)
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_MRAPP, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_MRAPP, FALSE);
        }
#ifdef PDA_UI_SUPPORT
#if defined MMI_ISTYLE_SUPPORT
		if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    	{
        	GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_MRAPP, TRUE);
    	}
    	else
#endif			
    	{
        	GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_MRAPP, FALSE);
    	}
#endif
        break;
#endif
#ifdef HERO_ENGINE_SUPPORT 
    case MMI_WIN_ICON_HERO:
#if 0//ndef HERO_SLIM_OPEN
        item_data.icon_info.icon_array[0] = IMG_HERO_APP_ONLINE;
#else
		item_data.icon_info.icon_array[0] = IMAGE_NULL;
#endif

        item_data.app_handle_func = MMIHEROAPP_activeMrapp;

        GUIWIN_AppendStbItemData(&item_data);
        if(MMIHEROAPP_GetVMState() == VM_BACK_RUN)
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_HERO, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_HERO, FALSE);
        }
#ifdef PDA_UI_SUPPORT
		if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    	{
        	GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_HERO, TRUE);
    	}
    	else
    	{
        	GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_HERO, FALSE);
    	}
#endif
        break;

#endif

#ifdef MMI_DUAL_BATTERY_SUPPORT
        case MMI_WIN_ICON_BATTERY:  // 电池电量
        //SCI_TRACE_LOW:"SetStatusBarInfo MMI_WIN_ICON_BATTERY"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_1174_112_2_18_2_24_13_70,(uint8*)"");
        MMIAPIIDLE_SetStbBatteryInfo(TRUE);
        break;

        case MMI_WIN_ICON_BATTERY_AUX:  // 电池电量
        //SCI_TRACE_LOW:"SetStatusBarInfo MMI_WIN_ICON_BATTERY_AUX"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_1179_112_2_18_2_24_13_71,(uint8*)"");
        MMIAPIIDLE_SetStbBatteryInfo(FALSE);
        break;
#else
    case MMI_WIN_ICON_BATTERY:  // 电池电量
        {
            uint32  icon_index       =  0;
#if (!defined ADULT_WATCH_SUPPORT) && (defined SCREEN_SHAPE_CIRCULAR)
            uint32  charge_icon_id[12] =       {res_stat_battery_0,
                                                                    res_stat_battery_10,
                                                                    res_stat_battery_20,
                                                                    res_stat_battery_30,
                                                                    res_stat_battery_40,
                                                                    res_stat_battery_50,
                                                                    res_stat_battery_60,
                                                                    res_stat_battery_70,
                                                                    res_stat_battery_80,
                                                                    res_stat_battery_90,
                                                                    res_stat_battery_100,
                                                                    res_stat_battery_charing
                                                                    };

#else
            uint32  charge_icon_id[11] =       {res_stat_battery_0,
                                                                    res_stat_battery_10,
                                                                    res_stat_battery_20,
                                                                    res_stat_battery_30,
                                                                    res_stat_battery_40,
                                                                    res_stat_battery_50,
                                                                    res_stat_battery_60,
                                                                    res_stat_battery_70,
                                                                    res_stat_battery_80,
                                                                    res_stat_battery_90,
                                                                    res_stat_battery_100,
                                                                    };
#endif
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
            MMISET_POWER_SETTING_ITEM_T  power_sav = MMIAPISET_GetPowerSavingSetting();  
            
            if(power_sav.is_active)
            {
                charge_icon_id[0] = IMAGE_IDLE_TITLE_ICON_PS_BATTERY1;
                charge_icon_id[1] = IMAGE_IDLE_TITLE_ICON_PS_BATTERY2;
                charge_icon_id[2] = IMAGE_IDLE_TITLE_ICON_PS_BATTERY3;
                charge_icon_id[3] = IMAGE_IDLE_TITLE_ICON_PS_BATTERY4;
                charge_icon_id[4] = IMAGE_IDLE_TITLE_ICON_PS_BATTERY5;
                charge_icon_id[5] = IMAGE_IDLE_TITLE_ICON_PS_BATTERY6;
            }
#endif
            MMIIDLE_GetIdleWinInfo()->batlevel = MMIAPIPHONE_GetBatCapacity(); // 获取当前的电量

            for (icon_index = 0; icon_index < MMISTATUSBAR_ITEM_ICON_FRAME; icon_index++)
            {
                item_data.icon_info.icon_array[icon_index] = charge_icon_id[icon_index];
            }
#if (!defined ADULT_WATCH_SUPPORT) && (defined SCREEN_SHAPE_CIRCULAR)
            item_data.app_handle_func = OpenBtryCapWinFunc;
            item_data.icon_info.icon_type = GUIWIN_STBDATA_ICON_NORMAL;
            item_data.icon_info.icon_num = MMISTATUSBAR_ITEM_ICON_FRAME;

            if (MMIIDLE_GetIdleWinInfo()->is_charge)
            {
                item_data.cur_frame = 11;
            }
            else
            {
                item_data.cur_frame = MMIIDLE_GetIdleWinInfo()->batlevel;
            }

#else
            item_data.app_handle_func = OpenBtryCapWinFunc;
            item_data.icon_info.icon_num = MMISTATUSBAR_ITEM_ICON_FRAME;
            item_data.cur_frame = MMIIDLE_GetIdleWinInfo()->batlevel;

            if (MMIIDLE_GetIdleWinInfo()->is_charge)
            {
                item_data.icon_info.icon_type = GUIWIN_STBDATA_ICON_ANIM;
            }
            else
            {
                item_data.icon_info.icon_type = GUIWIN_STBDATA_ICON_NORMAL;
            }
#endif
            GUIWIN_AppendStbItemData(&item_data);
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_BATTERY, TRUE);
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_BATTERY, TRUE);
            GUIWIN_SetStbItemValid(MMI_WIN_ICON_BATTERY,TRUE);
        }
        break;

#endif
case MMI_WIN_ICON_HD_CALL:
        item_data.icon_info.icon_array[0] = IMAGE_IDLE_ICON_HDCL;
        item_data.app_handle_func = PNULL;
        GUIWIN_AppendStbItemData(&item_data);
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_HD_CALL, FALSE);
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_HD_CALL, TRUE);
        break;
#ifdef QQ_SUPPORT
    case MMI_WIN_ICON_SPREADTRUM_QQ:
        InitIdleQQState();
        break;
#endif

#ifdef ASP_SUPPORT
    case MMI_WIN_ICON_ASP:
        InitIdleASPState();
        break;
#endif
#ifdef MCARE_V31_SUPPORT
	 case MMI_WIN_ICON_MCARE_APP:
		{
#ifndef WIN32
		    McfIF_SetStatuBarInfo();
#else
			GUIWIN_AppendStbItemData(&item_data);
#endif
#if defined MMI_ISTYLE_SUPPORT
            if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
            {
                GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_MCARE_APP, TRUE);
            }
            else
#endif				
            {
                GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_MCARE_APP, FALSE);
            }
		}
		break;
#endif
#ifdef WRE_SUPPORT //add for wre_background by heng.xiao    
    case MMI_WIN_WRE:
    case MMI_WIN_WRE+1:
    case MMI_WIN_WRE+2:
    case MMI_WIN_WRE+3:
    case MMI_WIN_WRE+4:
    case MMI_WIN_WRE+WRE_ICON_MAX-1:
        {
            //lend IMAGE_IDLE_TITLE_ICON_CALLING to calculate width
            item_data.icon_info.icon_array[0] = IMAGE_IDLE_TITLE_ICON_CALLING;  
            //item_data.icon_info.icon_array[1] = 0;
            item_data.icon_info.icon_num = 1;
            item_data.icon_info.icon_type = GUIWIN_STBDATA_ICON_WRE;
            MMIWRE_SetStatusBarValue(item_index-MMI_WIN_WRE,&item_data);
            
            GUIWIN_AppendStbItemData(&item_data);
            if (MMIWRE_HAVE_BG(item_index-MMI_WIN_WRE))
            {
                MAIN_UpdateIdleWREState(item_index-MMI_WIN_WRE,TRUE,FALSE);
            }
            else
            {
                MAIN_UpdateIdleWREState(item_index-MMI_WIN_WRE,FALSE,FALSE);
            }            
        }
        break;
#endif

#ifdef MMI_BATTERY_PERCENT_IN_STATUSBAR 
    case MMI_WIN_ICON_BATTERY_PERCENT:
        {
            BOOLEAN is_percent_dis = MMIAPISET_GetBatteryPercentDisSwitch();
            item_data.icon_info.is_display = FALSE;
            item_data.text_info = MMIAPIIDLE_GetBatteryPercentStbTxt();
            
            GUIWIN_AppendStbItemData(&item_data);
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_BATTERY_PERCENT, is_percent_dis);
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_BATTERY_PERCENT, TRUE);
            GUIWIN_SetStbItemValid(MMI_WIN_ICON_BATTERY_PERCENT,TRUE);
        }
        break;
#endif

    default:
        break;
    }
}
LOCAL boolean handleTeleSimMsg(ual_cms_msg_t msg)
{
    SCI_TRACE_LOW("msg.msg_id = %d.", msg.msg_id);
    switch (msg.msg_id)
    {
        case MSG_UAL_TELE_SIM_STATUS_IND:
        {
            SCI_TRACE_LOW("MSG_UAL_TELE_SIM_STATUS_IND");
            MAIN_UpdateIdleSignalState();
            break;
        }
        default:
            SCI_TRACE_LOW("recive default! msg_id = %d", msg.msg_id);
            break;
    }
    return TRUE;
}
/*****************************************************************************/
// Description : 显示图标
// Global resource dependence :
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
PUBLIC void MMIDILE_StatusBarInit(void)
{
    uint32      i   =   0;

#if defined CMCC_UI_STYLE || defined NET_ICON_UI_STYLE
    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        s_pdp_event[i] = MMIPDP_NOTIFY_EVENT_MAX; 
    }
#endif

    for(i = 0; i < MMI_WIN_ICON_MAX; i++)
    {
        SetStatusBarInfo(i);
    }

#ifdef MMI_MULTISIM_COLOR_SUPPORT_STATUSICON
    MMIMAIN_InitStatusBarOfSimColor();
#endif
#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
	MMIFW_SetEventListener(SMS_NEWSMS_EVENT, NewEventCallback, TRUE); 
#endif

    ual_tele_sim_register(handleTeleSimMsg, &s_tele_sim_register_handle);
}

PUBLIC BOOLEAN MMIAPICOM_IsExistInRatIconTable(MN_PLMN_T * cur_plmn_ptr, BOOLEAN * isGmust, BOOLEAN * isLTEmust)
{
    uint16 i = 0;
    BOOLEAN isRaticonable = FALSE;
    
    for (i = 0 ; i < MAX_RATICON_NUM ; i++)
    {
        if ((s_raticon_table[i].mnc == cur_plmn_ptr->mnc)
            && (s_raticon_table[i].mcc == cur_plmn_ptr->mcc))
        {
            *isGmust = s_raticon_table[i].isGmust;
            *isLTEmust = s_raticon_table[i].isLTEmust;
            isRaticonable = TRUE;
            
            break;
        }
    }

    return isRaticonable;
}

/*****************************************************************************/
//  Description : handle miss call
//  Global resource dependence : none
//  Author: Paul.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIN_MissCallCallBack(void)
{
    return DisplayMissedCallFunc();
}

/*****************************************************************************/
//  Description : handle miss message
//  Global resource dependence : none
//  Author: Paul.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIN_MissMessageCallBack(void)
{
    return DisplayMsgInboxFunc();
}

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
/*****************************************************************************/
//  Description : handle miss message
//  Global resource dependence : none
//  Author: Paul.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIN_ReadPushCallBack(void)
{
    uint32 push_num = 0;
    MMISMS_ORDER_ID_T first_order_id = PNULL;

    push_num = MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_WAP_PUSH) + MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_WAP_OTA);

    MMIAPISMS_OpenPushBox();
    if(1 == push_num)
    {
        first_order_id = MMIAPISMS_GetFirstUnreadPushMsgId();
        if(PNULL != first_order_id)
        {
            if(first_order_id->flag.msg_type == MMISMS_TYPE_WAP_PUSH)
            {
#ifdef MMI_OTA_SUPPORT
                MMIAPIOTA_WapPushEnter(first_order_id->record_id);
#endif
            }
            else if(first_order_id->flag.msg_type == MMISMS_TYPE_WAP_OTA)
            {
#ifdef MMI_OTA_SUPPORT
                MMIAPIOTA_OTAEnter(first_order_id->record_id);
#endif
            }
        }
    }
    return TRUE;
}
#endif


/*****************************************************************************/
// Description : 显示信号图标
// Global resource dependence : 
// Author: wancan.you
// Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_SetSimStatusBarInfo(
                                MN_DUAL_SYS_E dual_sys,
                                uint32 item_index,
                                BOOLEAN is_append
                                )
{
#if defined CMCC_UI_STYLE
    const MMI_IMAGE_ID_T service_image[NET_SERVICE_TYPE_MAX][MMIPDP_NOTIFY_EVENT_MAX + 1]=
    {
        {IMAGE_IDLE_TITLE_ICON_G_DATAING_1,IMAGE_IDLE_TITLE_ICON_G_1,IMAGE_IDLE_TITLE_ICON_G_1,IMAGE_IDLE_TITLE_ICON_G_1, IMAGE_IDLE_TITLE_ICON_G_1},
        {IMAGE_IDLE_TITLE_ICON_E_DATAING_1,IMAGE_IDLE_TITLE_ICON_E_1,IMAGE_IDLE_TITLE_ICON_E_1,IMAGE_IDLE_TITLE_ICON_E_1, IMAGE_IDLE_TITLE_ICON_E_1},
        {IMAGE_IDLE_TITLE_ICON_3G_DATAING_1,IMAGE_IDLE_TITLE_ICON_3G_1,IMAGE_IDLE_TITLE_ICON_3G_1,IMAGE_IDLE_TITLE_ICON_3G_1, IMAGE_IDLE_TITLE_ICON_3G_1},
        {IMAGE_PRIMO_STATUS_4G_bar0_connect, IMAGE_PRIMO_STATUS_4G_bar0_normal, IMAGE_PRIMO_STATUS_4G_bar0_normal, IMAGE_PRIMO_STATUS_4G_bar0_normal, IMAGE_PRIMO_STATUS_4G_bar0_normal},
        {IMAGE_PRIMO_STATUS_LTE_bar0_connect, IMAGE_PRIMO_STATUS_LTE_bar0_normal, IMAGE_PRIMO_STATUS_LTE_bar0_normal, IMAGE_PRIMO_STATUS_LTE_bar0_normal, IMAGE_PRIMO_STATUS_LTE_bar0_normal},
    };
#endif

#if defined NET_ICON_UI_STYLE
    MN_PHONE_PLMN_STATUS_E plmn_status = PLMN_NO_SERVICE;
#ifdef MMI_MULTI_SIM_SYS_SINGLE
    const MMI_IMAGE_ID_T emergency_service_start_id = IMAGE_IDLE_TITLE_ICON_X_SIM_1;

    const MMI_IMAGE_ID_T service_image[NET_SERVICE_TYPE_MAX][MMIPDP_NOTIFY_EVENT_MAX + 1]=
    {
    #ifdef ZDT_NET_2G_SUPPORT
        {IMAGE_IDLE_ICON_2G_DATAING_1,IMAGE_IDLE_ICON_2G_1,IMAGE_IDLE_ICON_2G_1,IMAGE_IDLE_ICON_2G_1, IMAGE_IDLE_ICON_2G_1},
        {IMAGE_IDLE_ICON_2G_DATAING_1,IMAGE_IDLE_ICON_2G_1,IMAGE_IDLE_ICON_2G_1,IMAGE_IDLE_ICON_2G_1, IMAGE_IDLE_ICON_2G_1},
    #endif
    #ifdef ZDT_NET_3G_SUPPORT
        {IMAGE_IDLE_ICON_3G_DATAING_1,IMAGE_IDLE_ICON_3G_1,IMAGE_IDLE_ICON_3G_1,IMAGE_IDLE_ICON_3G_1, IMAGE_IDLE_ICON_3G_1},
    #endif
        {IMAGE_IDLE_ICON_4G_DATAING_1, IMAGE_IDLE_ICON_4G_1, IMAGE_IDLE_ICON_4G_1, IMAGE_IDLE_ICON_4G_1, IMAGE_IDLE_ICON_4G_1},
    };
#elif defined MULTI_SIM_SYS_DUAL	
    const MMI_IMAGE_ID_T emergency_service_start_id[MMI_DUAL_SYS_MAX]  = {IMAGE_IDLE_TITLE_ICON_X_SIM1_1, IMAGE_IDLE_TITLE_ICON_X_SIM2_1};

    const MMI_IMAGE_ID_T service_image_sim1[NET_SERVICE_TYPE_MAX][MMIPDP_NOTIFY_EVENT_MAX + 1]=
    {
        {IMAGE_IDLE_TITLE_ICON_G_DATAING_SIM1_1,IMAGE_IDLE_TITLE_ICON_G_SIM1_1,IMAGE_IDLE_TITLE_ICON_G_SIM1_1,IMAGE_IDLE_TITLE_ICON_G_SIM1_1, IMAGE_IDLE_TITLE_ICON_G_SIM1_1},
        {IMAGE_IDLE_TITLE_ICON_E_DATAING_SIM1_1,IMAGE_IDLE_TITLE_ICON_E_SIM1_1,IMAGE_IDLE_TITLE_ICON_E_SIM1_1,IMAGE_IDLE_TITLE_ICON_E_SIM1_1, IMAGE_IDLE_TITLE_ICON_E_SIM1_1},
        {IMAGE_IDLE_TITLE_ICON_3G_DATAING_SIM1_1,IMAGE_IDLE_TITLE_ICON_3G_SIM1_1,IMAGE_IDLE_TITLE_ICON_3G_SIM1_1,IMAGE_IDLE_TITLE_ICON_3G_SIM1_1, IMAGE_IDLE_TITLE_ICON_3G_SIM1_1},
        {IMAGE_PRIMO_STATUS_4G_bar0_connect, IMAGE_PRIMO_STATUS_4G_bar0_normal, IMAGE_PRIMO_STATUS_4G_bar0_normal, IMAGE_PRIMO_STATUS_4G_bar0_normal, IMAGE_PRIMO_STATUS_4G_bar0_normal},
        //{IMAGE_PRIMO_STATUS_LTE_bar0_connect, IMAGE_PRIMO_STATUS_LTE_bar0_normal, IMAGE_PRIMO_STATUS_LTE_bar0_normal, IMAGE_PRIMO_STATUS_LTE_bar0_normal, IMAGE_PRIMO_STATUS_LTE_bar0_normal},
    };

    const MMI_IMAGE_ID_T service_image_sim2[NET_SERVICE_TYPE_MAX][MMIPDP_NOTIFY_EVENT_MAX + 1]=
    {
        {IMAGE_IDLE_TITLE_ICON_G_DATAING_SIM2_1,IMAGE_IDLE_TITLE_ICON_G_SIM2_1,IMAGE_IDLE_TITLE_ICON_G_SIM2_1,IMAGE_IDLE_TITLE_ICON_G_SIM2_1, IMAGE_IDLE_TITLE_ICON_G_SIM2_1},
        {IMAGE_IDLE_TITLE_ICON_E_DATAING_SIM2_1,IMAGE_IDLE_TITLE_ICON_E_SIM2_1,IMAGE_IDLE_TITLE_ICON_E_SIM2_1,IMAGE_IDLE_TITLE_ICON_E_SIM2_1, IMAGE_IDLE_TITLE_ICON_E_SIM2_1},
        {IMAGE_IDLE_TITLE_ICON_3G_DATAING_SIM2_1,IMAGE_IDLE_TITLE_ICON_3G_SIM2_1,IMAGE_IDLE_TITLE_ICON_3G_SIM2_1,IMAGE_IDLE_TITLE_ICON_3G_SIM2_1, IMAGE_IDLE_TITLE_ICON_3G_SIM2_1},
        {IMAGE_PRIMO_STATUS_4G_bar0_connect, IMAGE_PRIMO_STATUS_4G_bar0_normal, IMAGE_PRIMO_STATUS_4G_bar0_normal, IMAGE_PRIMO_STATUS_4G_bar0_normal, IMAGE_PRIMO_STATUS_4G_bar0_normal},
        //{IMAGE_PRIMO_STATUS_LTE_bar0_connect, IMAGE_PRIMO_STATUS_LTE_bar0_normal, IMAGE_PRIMO_STATUS_LTE_bar0_normal, IMAGE_PRIMO_STATUS_LTE_bar0_normal, IMAGE_PRIMO_STATUS_LTE_bar0_normal},
    };
#endif
#endif

#ifdef MMI_MULTISIM_COLOR_SUPPORT_STATUSICON
        GUISTBDATA_ICON_T icon_info = {0};
#else
        const MMI_IMAGE_ID_T signal_gsm_start_id[MMI_DUAL_SYS_MAX] = 
        {
    #ifdef MMI_MULTI_SIM_SYS_SINGLE
            IMAGE_IDLE_TITLE_ICON_SIGNAL1
    #else
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_SIM1,
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_SIM2,
        #if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_SIM3,
        #endif
        #if defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_SIM4
        #endif
    #endif
        };
        
#ifdef MMI_GPRS_SUPPORT     
        const MMI_IMAGE_ID_T signal_gprs_start_id[MMI_DUAL_SYS_MAX] = {
    #ifdef MMI_MULTI_SIM_SYS_SINGLE
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_GPRS
    #else
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_GPRS_SIM1,
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_GPRS_SIM2,
        #if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_GPRS_SIM3,
        #endif
        #if defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_GPRS_SIM4
        #endif
    #endif
        };
#endif    
    
#ifdef PRODUCT_DM  /* lint_lai */
        const MMI_IMAGE_ID_T signal_td_start_id[MMI_DUAL_SYS_MAX] = {
    #ifdef MMI_MULTI_SIM_SYS_SINGLE
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_T
    #else
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_T_SIM1,
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_T_SIM2,
        #if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_T_SIM3,
        #endif
        #if defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_T_SIM4
        #endif
    #endif
        };   
#endif    

#endif /* end of MMI_MULTISIM_COLOR_SUPPORT_STATUSICON */

    BOOLEAN is_fly_mode_on = FALSE;
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    GUIWIN_STBDATA_ITEM_DATA_T item_data = {0};
    MMI_IMAGE_ID_T signal_start_id = IMAGE_NULL;
    MN_PLMN_T cur_plmn = {0};
    BOOLEAN isGmust = FALSE;
    BOOLEAN isLTEmust = FALSE;
    BOOLEAN isRaticonable = FALSE;
    MMIPHONE_NET_SERVICE_TYPE_E  service_type = NET_SERVICE_TYPE_MAX;
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    item_data.icon_info.icon_num = 1;
    item_data.icon_info.icon_type = GUIWIN_STBDATA_ICON_NORMAL;
    item_data.icon_info.is_display = TRUE;
    item_data.cur_frame = 0;
    item_data.is_handle_callback = TRUE;
    
    is_fly_mode_on = MMIAPISET_GetFlyMode();

#ifdef SCREEN_SHAPE_CIRCULAR
    if (is_fly_mode_on)
    {
        if (MN_DUAL_SYS_1 == dual_sys)//飞行模式只显示一个图标
        {
            image_id = IMAGE_IDLE_TITLE_ICON_FLYMODE;
        }
        else
        {
            image_id = IMAGE_NULL;
        }
    }
    else if (MMIAPIPHONE_GetSimExistedStatus(dual_sys))
    {
#ifndef UAL_TELE_SUPPORT
        MMI_GMMREG_RAT_E network_status_rat = MMI_GMMREG_RAT_GPRS;
        network_status_rat = MMIAPIPHONE_GetTDOrGsm(dual_sys);
#else
        ual_tele_radio_current_network_type_e network_status_rat = UAL_TELE_RADIO_CURRENT_NETWORK_TYPE_2G;
        network_status_rat = ual_tele_radio_get_current_network_type(dual_sys);
#endif
        isRaticonable = MMIAPICOM_IsExistInRatIconTable(&cur_plmn, &isGmust, &isLTEmust);
        SCI_TRACE_LOW("MMIIDLE_SetSimStatusBarInfo dual_sys:%d ,network_status_rat:%d,data_service_type:%d", dual_sys, network_status_rat, MMIAPIPHONE_GetDataServiceType(dual_sys));

#ifdef MMI_MULTISIM_COLOR_SUPPORT_STATUSICON
        switch (network_status_rat) 
        {
        case MMI_GMMREG_RAT_GPRS:
            #ifdef MMI_GPRS_SUPPORT            
            signal_start_id = IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_GPRS;
            #else            
            signal_start_id = IMAGE_IDLE_TITLE_ICON_SIGNAL1;
            #endif            
            break;
            
        case MMI_GMMREG_RAT_3G:
            signal_start_id = IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_T;
            break;
        case MMI_GMMREG_RAT_LTE:            
        case MMI_GMMREG_RAT_UNKNOWN:
            signal_start_id = IMAGE_IDLE_TITLE_ICON_SIGNAL1;
            break;
            
        default:
            signal_start_id = IMAGE_IDLE_TITLE_ICON_SIGNAL1;
            break;
        }
#else
        switch (network_status_rat) 
        {
#ifndef UAL_TELE_SUPPORT
        case MMI_GMMREG_RAT_GPRS:
#else
        case UAL_TELE_RADIO_CURRENT_NETWORK_TYPE_2G:
#endif
    #if defined CMCC_UI_STYLE
            signal_start_id = service_image[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
    #elif defined NET_ICON_UI_STYLE
        #ifdef MMI_MULTI_SIM_SYS_SINGLE
            signal_start_id = service_image[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
        #elif defined MULTI_SIM_SYS_DUAL	
            if (MN_DUAL_SYS_1 == dual_sys)
            {
                signal_start_id = service_image_sim1[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
            }
            else if (MN_DUAL_SYS_2 == dual_sys)
            {
                signal_start_id = service_image_sim2[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
            }			
        #endif
    #else
            signal_start_id = signal_gprs_start_id[dual_sys];
    #endif
            break;
            
#ifndef UAL_TELE_SUPPORT
        case MMI_GMMREG_RAT_3G:
#else
        case UAL_TELE_RADIO_CURRENT_NETWORK_TYPE_3G:
#endif
    #if defined CMCC_UI_STYLE
            signal_start_id = service_image[NET_SERVICE_TYPE_3G][s_pdp_event[dual_sys]];
    #elif defined NET_ICON_UI_STYLE
        #ifdef MMI_MULTI_SIM_SYS_SINGLE
            signal_start_id = service_image[NET_SERVICE_TYPE_3G][s_pdp_event[dual_sys]];
        #elif defined MULTI_SIM_SYS_DUAL	
            if (MN_DUAL_SYS_1 == dual_sys)
            {
                signal_start_id = service_image_sim1[NET_SERVICE_TYPE_3G][s_pdp_event[dual_sys]];
            }
            else if (MN_DUAL_SYS_2 == dual_sys)
            {
                signal_start_id = service_image_sim2[NET_SERVICE_TYPE_3G][s_pdp_event[dual_sys]];
            }			
        #endif
    #else
            signal_start_id = signal_td_start_id[dual_sys];
    #endif
            break;
            
#ifndef UAL_TELE_SUPPORT
        case MMI_GMMREG_RAT_LTE:
#else
        case UAL_TELE_RADIO_CURRENT_NETWORK_TYPE_4G:
#endif
            service_type = NET_SERVICE_TYPE_4G;
    #if defined CMCC_UI_STYLE
                    signal_start_id = service_image[service_type][s_pdp_event[dual_sys]];
    #elif defined NET_ICON_UI_STYLE
        #ifdef MMI_MULTI_SIM_SYS_SINGLE
                    SCI_TRACE_LOW("[MMIIDLE_SetSimStatusBarInfo] s_pdp_event[%d] = %d", dual_sys, s_pdp_event[dual_sys]);
                    signal_start_id = service_image[service_type][s_pdp_event[dual_sys]];
        #elif defined MULTI_SIM_SYS_DUAL    
                    if (MN_DUAL_SYS_1 == dual_sys)
                    {
                        signal_start_id = service_image_sim1[service_type][s_pdp_event[dual_sys]];
                    }
                    else if (MN_DUAL_SYS_2 == dual_sys)
                    {
                        signal_start_id = service_image_sim2[service_type][s_pdp_event[dual_sys]];
                    }           
        #endif
    #else
                    signal_start_id = signal_gprs_start_id[dual_sys];
    #endif
            break;
            
#ifndef UAL_TELE_SUPPORT
        case MMI_GMMREG_RAT_UNKNOWN:
#else
        case UAL_TELE_RADIO_CURRENT_NETWORK_TYPE_UNKNOWN:
#endif
    #if defined CMCC_UI_STYLE
            signal_start_id = service_image[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
    #elif defined NET_ICON_UI_STYLE
        #ifdef MMI_MULTI_SIM_SYS_SINGLE
            signal_start_id = service_image[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
        #elif defined MULTI_SIM_SYS_DUAL	
            if(MN_DUAL_SYS_1 == dual_sys)
            {
                signal_start_id = service_image_sim1[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
            }
            else if(MN_DUAL_SYS_2 == dual_sys)
            {
                signal_start_id = service_image_sim2[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
            }			
        #endif
    #else
            signal_start_id = signal_gsm_start_id[dual_sys];
    #endif
            break;
            
        default:
    #if defined CMCC_UI_STYLE
            signal_start_id = service_image[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
    #elif defined NET_ICON_UI_STYLE
        #ifdef MMI_MULTI_SIM_SYS_SINGLE
            signal_start_id = service_image[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
        #elif defined MULTI_SIM_SYS_DUAL	
            if(MN_DUAL_SYS_1 == dual_sys)
            {
                signal_start_id = service_image_sim1[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
            }
            else if(MN_DUAL_SYS_2 == dual_sys)
            {
                signal_start_id = service_image_sim2[NET_SERVICE_TYPE_G][s_pdp_event[dual_sys]];
            }			
        #endif
    #else
            signal_start_id = signal_gsm_start_id[dual_sys];
    #endif
            break;
        }
#endif /* end of MMI_MULTISIM_COLOR_SUPPORT_STATUSICON */

#if defined CMCC_UI_STYLE
        if (EDGE_DATA_SERVICE_TYPE == MMIAPIPHONE_GetDataServiceType(dual_sys))
        {
            signal_start_id = service_image[NET_SERVICE_TYPE_E][s_pdp_event[dual_sys]];
        }
#endif

#if defined NET_ICON_UI_STYLE
        if (EDGE_DATA_SERVICE_TYPE == MMIAPIPHONE_GetDataServiceType(dual_sys))
        {
    #ifdef MMI_MULTI_SIM_SYS_SINGLE
            signal_start_id = service_image[NET_SERVICE_TYPE_E][s_pdp_event[dual_sys]];
    #elif defined MULTI_SIM_SYS_DUAL	
            if(MN_DUAL_SYS_1 == dual_sys)
            {
                signal_start_id = service_image_sim1[NET_SERVICE_TYPE_E][s_pdp_event[dual_sys]];
            }
            else if(MN_DUAL_SYS_2 == dual_sys)
            {
                signal_start_id = service_image_sim2[NET_SERVICE_TYPE_E][s_pdp_event[dual_sys]];
            }	
    #endif	
        }
        plmn_status = MMIAPIPHONE_GetPlmnStatus(dual_sys);
        if (PLMN_NO_SERVICE == plmn_status
            || PLMN_EMERGENCY_ONLY == plmn_status 
            || PLMN_EMERGENCY_SIM_INVALID == plmn_status
            || PLMN_EMERGENCY_GPRS_ONLY == plmn_status 
            || PLMN_EMERGENCY_SIM_INVALID_GPRS_ONLY == plmn_status
            || PLMN_REGISTER_SERVICE == plmn_status
            || PLMN_REGISTER_GPRS_ONLY == plmn_status)
        {
    #ifdef MMI_MULTI_SIM_SYS_SINGLE
            SCI_TRACE_LOW("[MMIIDLE_SetSimStatusBarInfo] emergency_service_start_id");
            signal_start_id = emergency_service_start_id;
    #elif defined MULTI_SIM_SYS_DUAL	
            signal_start_id = emergency_service_start_id[dual_sys];
    #endif        
        }            
#endif /* end of NET_ICON_UI_STYLE */

        if (MMIAPIIDLE_NeedShowIconAccordingToDualSysSetting(dual_sys))
        {
            if (SIM_STATUS_PIN_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys)
                || SIM_STATUS_PUK_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys))
            {
                image_id = signal_start_id;
            }
            else
            {
                image_id = MMIIDLE_GetIdleWinInfo()->rxlevel_step[dual_sys] + signal_start_id;
            }
        }
        else
        {
            image_id = signal_start_id;
        }
    }
    else
    {
        image_id = res_stat_nosim;
    }

#else
    if (is_fly_mode_on)
    {
        if (MN_DUAL_SYS_1 == dual_sys)//飞行模式只显示一个图标
        {
            image_id = IMAGE_IDLE_TITLE_ICON_FLYMODE;
        }
        else
        {
            image_id = IMAGE_NULL;
        }
    }
//    else if( 0 == MMIAPIPHONE_GetSimExistedNum(PNULL, 0))
//   {
        //如果没有插入sim卡，只需要显示1个没有卡的图标就可以了。
//        if(MN_DUAL_SYS_1 == dual_sys)
//        {
//            image_id = IMAGE_IDLE_ICON_NO_SIM;
//        }
//        else
//        {
//            image_id = IMAGE_NULL;
//        }
//    }
    else if(SIM_STATUS_NO_SIM == MMIAPIPHONE_GetSimStatus(dual_sys))
	{
        image_id = IMAGE_IDLE_ICON_NO_SIM;
	}
    else
    {
        if (MMIAPIPHONE_GetSimExistedStatus(dual_sys))
        {
        #ifdef MMI_MULTISIM_COLOR_SUPPORT_STATUSICON
            if (MMIAPIIDLE_NeedShowIconAccordingToDualSysSetting(dual_sys))
            {
                if (SIM_STATUS_PIN_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys)
                    || SIM_STATUS_PUK_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys))
                {
                    image_id = IMAGE_IDLE_TITLE_ICON_SIGNAL1;
                }
                else
                {
                    if (MMIIDLE_GetIdleWinInfo()->is_gprs[dual_sys])
                    {
                        if (MMIAPICONNECTION_isGPRSSwitchedOff())
                        {
                            signal_start_id = IMAGE_IDLE_TITLE_ICON_SIGNAL1;
                        }
            #ifdef MMI_GPRS_SUPPORT                        
                        else
                        {
                            signal_start_id = IMAGE_IDLE_TITLE_ICON_SIGNAL1_AND_GPRS;
                        }
            #endif                        

                    }
                    else
                    {
                        signal_start_id = IMAGE_IDLE_TITLE_ICON_SIGNAL1;
                    }

                    image_id = MMIIDLE_GetIdleWinInfo()->rxlevel_step[dual_sys] + signal_start_id;
                }
            }
            else
            {
                image_id = IMAGE_IDLE_TITLE_ICON_SIGNAL1;
            }
        #else /* MMI_MULTISIM_COLOR_SUPPORT_STATUSICON */
            if (MMIAPIIDLE_NeedShowIconAccordingToDualSysSetting(dual_sys))
            {
                if (SIM_STATUS_PIN_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys)
                    || SIM_STATUS_PUK_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys))
                {
                    image_id = signal_gsm_start_id[dual_sys];
                }
                else
                {
                    if (MMIIDLE_GetIdleWinInfo()->is_gprs[dual_sys])
                    {
                        if (MMIAPICONNECTION_isGPRSSwitchedOff())
                        {
                            signal_start_id = signal_gsm_start_id[dual_sys];
                        }
            #ifdef MMI_GPRS_SUPPORT 
                        else
                        {
                            signal_start_id = signal_gprs_start_id[dual_sys];
                        }
            #endif
                    }
                    else
                    {
                        signal_start_id = signal_gsm_start_id[dual_sys];
                    }

                    image_id = MMIIDLE_GetIdleWinInfo()->rxlevel_step[dual_sys] + signal_start_id;
                }
            }
            else
            {
                image_id = signal_gsm_start_id[dual_sys];
            }
        #endif /* end of MMI_MULTISIM_COLOR_SUPPORT_STATUSICON */
        }
        else
        {
            image_id = IMAGE_NULL;
        }
    }
#endif /* end of PRODUCT_DM */

#ifdef MMI_VOLTE_SUPPORT
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (MN_DUAL_SYS_2 == dual_sys)
    {
        item_index = MMI_WIN_ICON_SIGNAL_2;
    }
#endif
#endif

#ifdef MMI_MULTISIM_COLOR_SUPPORT_STATUSICON
    if ((image_id == IMAGE_IDLE_TITLE_ICON_FLYMODE ) || (image_id == IMAGE_IDLE_ICON_NO_SIM ) || (image_id == IMAGE_NULL))
    {
        GUISTBDATA_GetItemIcon(item_index, &icon_info);
        icon_info.is_color_exchange	= FALSE;
        GUISTBDATA_SetItemIcon(item_index, &icon_info);
    }
    else
    {
        GUISTBDATA_GetItemIcon(item_index, &icon_info);
        icon_info.is_color_exchange = TRUE;
        GUISTBDATA_SetItemIcon(item_index, &icon_info);
    }
#endif    
    item_data.icon_info.icon_array[0] = image_id;
    item_data.app_handle_func = DisplaySetSysmodeFunc;

    if (is_append)
    {
        GUIWIN_AppendStbItemData(&item_data);
    }
    else
    {
        GUIWIN_SetStbItemIconId(item_index, image_id);
    }
#ifdef ADULT_WATCH_SUPPORT
    //check watch role
#ifdef BBM_ROLE_SUPPORT
    if(!BbmCurrentRoleIsBtPhone())
    {
        //not phone ,don't need signal
        image_id = IMAGE_NULL;
    }
#endif
#endif
    if (image_id == IMAGE_NULL)
    {
        GUIWIN_SetStbItemVisible(item_index, FALSE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(item_index, TRUE);
    }

    GUIWIN_SetStbItemRightAlign(item_index, FALSE);

    return;
}

#ifdef PDA_UI_DROPDOWN_WIN
/*****************************************************************************/
// Description : 获得日期字符串
// Global resource dependence : 
// Author: paul.huang
// Note:
/*****************************************************************************/
LOCAL void StatusbarGetSysDate(
                               wchar *date_wstr, 
                               uint16 wlen
                               )
{
    uint8 date_str[MMIIDLE_DATE_LEN]  = {0};
    uint16 date_wstr_len = 0;
    SCI_DATE_T  sys_date = {0};

    if (PNULL == date_wstr)
    {
        //MMI_ASSERT_WARNING("PNULL != date_wstr");
        //SCI_TRACE_LOW:"StatusbarGetSysDate ERROR! PNULL == date_wstr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_1581_112_2_18_2_24_14_73,(uint8*)"");
        return;
    }

    TM_GetSysDate(&sys_date);

    MMIAPISET_FormatDateStrByDateStyle(sys_date.year, sys_date.mon, sys_date.mday,'-',(uint8*)date_str,MMIIDLE_DATE_LEN);
 
    date_wstr_len = strlen((char*)date_str);
    date_wstr_len = MIN(date_wstr_len, wlen);
    //convert string
    MMI_STRNTOWSTR(date_wstr,MMIIDLE_DATE_LEN, date_str,MMIIDLE_DATE_LEN, date_wstr_len);
    
    return;
}
#endif

/*****************************************************************************/
//  Description :  设置文本内容
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 文本内容为时间
/*****************************************************************************/
LOCAL BOOLEAN StatusbarSetDispTime(void)
{
    uint16 w_str[MMIIDLE_TIME_LEN] = {0};
    GUIWIN_STBDATA_TEXT_T text_info = {0};
#ifdef PDA_UI_DROPDOWN_WIN
    uint16 w_str2[MMIIDLE_DATE_LEN] = {0};
    GUIWIN_STBDATA_TEXT_T text_info2 = {0};
#endif
    
    MAIN_StatusbarGetSysTime(w_str, MMIIDLE_TIME_LEN);
    text_info.is_display = TRUE;
    text_info.wstr_len = MMIAPICOM_Wstrlen(w_str);//MMIIDLE_TIME_LEN;
    
    SCI_MEMCPY(text_info.wstr_ptr, w_str, 
        sizeof(wchar)*MIN(text_info.wstr_len, MMIIDLE_TIME_LEN));
    text_info.font_color = MMI_WHITE_COLOR;//MMI_GRAY_WHITE_COLOR; CRNEWMS00203710
    text_info.align = ALIGN_HVMIDDLE;//ALIGN_LEFT; 
    text_info.font_type = MMI_STATUS_FONT_TYPE;
    
    GUIWIN_SetStbItemText(MMI_WIN_TEXT_TIME, &text_info);

#ifdef PDA_UI_DROPDOWN_WIN
    StatusbarGetSysDate(w_str2, MMIIDLE_DATE_LEN);
    text_info2.is_display = TRUE;
    text_info2.wstr_len = MMIAPICOM_Wstrlen(w_str2);
    
    SCI_MEMCPY(text_info2.wstr_ptr, w_str2, 
        sizeof(wchar)*MIN(text_info2.wstr_len, MMIIDLE_DATE_LEN));
    
    text_info2.font_color = MMI_WHITE_COLOR;//MMI_GRAY_WHITE_COLOR; CRNEWMS00203710
    text_info2.align = ALIGN_HVMIDDLE;//ALIGN_BOTTOM; //juan.zhao statusbar中 时间显示应该从下对齐
    text_info2.font_type = MMI_STATUS_FONT_TYPE;
    
    GUIWIN_SetStbItemText(MMI_WIN_TEXT_DATE, &text_info2);
#endif
    return TRUE;        
}
/*****************************************************************************/
//  Description :  设置文本内容
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 文本内容为时间
/*****************************************************************************/
PUBLIC void MAIN_SetStbDispTime(void)
{
    if (StatusbarSetDispTime())
    {
        //SCI_TRACE_LOW:"MAIN_SetStbDispTime Set Time OK!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_1647_112_2_18_2_24_14_74,(uint8*)"");
        GUIWIN_UpdateStb();
    }
}

/*****************************************************************************/
//  Description :  获取状态栏timer是否可见
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 文本内容为时间
/*****************************************************************************/
PUBLIC BOOLEAN MAIN_IsStbDispTime(void)
{
    return GUIWIN_IsStbItemVisible(MMI_WIN_TEXT_TIME);
}

/*****************************************************************************/
//  Description :  设置状态栏是否显示timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 文本内容为时间
/*****************************************************************************/
PUBLIC void MAIN_SetStbTimeVisible(
                                   BOOLEAN is_true
                                   )
{
    GUIWIN_SetStbItemVisible(MMI_WIN_TEXT_TIME, is_true);
}


/*****************************************************************************/
//  Description :  set status bar time info
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MAIN_SetStbDispTimeinfo(wchar *time_buf,uint16 time_len)
{
    BOOLEAN result = FALSE;
    if (PNULL!=time_buf && 0 != time_len)
    {
        GUIWIN_STBDATA_TEXT_T text_info = {0};
        text_info.is_display = TRUE;
        text_info.wstr_len = time_len;//MMIIDLE_TIME_LEN;
    
        SCI_MEMCPY(text_info.wstr_ptr, time_buf, 
            sizeof(wchar)*MIN(text_info.wstr_len, 10));
        text_info.font_color = MMI_WHITE_COLOR;//MMI_GRAY_WHITE_COLOR; CRNEWMS00203710
        text_info.align = ALIGN_HVMIDDLE;//ALIGN_LEFT; 
        text_info.font_type = MMI_STATUS_FONT_TYPE;
    
        GUIWIN_SetStbItemText(MMI_WIN_TEXT_TIME, &text_info);
        result = TRUE;
    }
    return result;
}
/*****************************************************************************/
// Description : 获得时间字符串
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC void MAIN_StatusbarGetSysTime(
                               wchar *time_wstr, 
                               uint16 wlen
                               )
{
    uint8 time_str[MMIIDLE_TIME_LEN]  = {0};
    uint16 time_wstr_len = 0;
    SCI_TIME_T  sys_time = {0};
    // BOOLEAN                     is_12hour       = FALSE;
    // BOOLEAN                     is_pm           = FALSE;

    if (PNULL == time_wstr)
    {
        //MMI_ASSERT_WARNING("PNULL != time_wstr");
        //SCI_TRACE_LOW:"MAIN_StatusbarGetSysTime ERROR! PNULL == time_wstr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_1715_112_2_18_2_24_14_75,(uint8*)"");
        return;
    }
    //get system time
    TM_GetSysTime(&sys_time);

    MMIAPISET_FormatTimeStrByTime(sys_time.hour, sys_time.min,(uint8*)time_str,MMIIDLE_TIME_LEN);
 
    time_wstr_len = strlen((char*)time_str);
    time_wstr_len = MIN(time_wstr_len, wlen);
    //convert string
    MMI_STRNTOWSTR(time_wstr,MMIIDLE_TIME_LEN, time_str,MMIIDLE_TIME_LEN, time_wstr_len);
    
    return;
}

#ifdef MMI_CSP_SUPPORT
/*****************************************************************************/
//  Description : update the state of line
//  Global resource dependence :  
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_UpdateLineState(void)
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    
    image_id = GetLineRoadIconId();
    GUIWIN_SetStbItemIconId(MMI_WIN_ICON_LINE_INFO, image_id);
    if (image_id == IMAGE_NULL)
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_LINE_INFO, FALSE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_LINE_INFO, TRUE);
    }

    // update
    GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE    
    //刷新小屏的线路显示状态
    MMISUB_UpdateDisplaySubLcd();
#endif
}

/*****************************************************************************/
//  Description : get the state of line
//  Global resource dependence :  
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleLineState(
                                  uint16 line_state,
                                  MN_DUAL_SYS_E dual_sys
                                  )
{    
    if (MMI_DUAL_SYS_MAX <= dual_sys)
    {
        //MMI_ASSERT_WARNING("dual_sys < MMI_DUAL_SYS_MAX");
        //SCI_TRACE_LOW:"MAIN_SetIdleLineState ERROR! dual_sys < MMI_DUAL_SYS_MAX,dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_1335_112_2_18_2_24_14_72,(uint8*)"d",dual_sys);
        return;
    }
    
    if (line_state != MMIIDLE_GetIdleWinInfo()->line_state[dual_sys])
    {
        MMIIDLE_GetIdleWinInfo()->line_state[dual_sys] = line_state;

        MAIN_UpdateLineState();
    }
}


/*****************************************************************************/
// Description : ?????????(?????2????)
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetLineRoadIconId(void)
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    uint32 i = 0;
    uint16 line2_index = 0;
    BOOLEAN is_display = FALSE;
    
    const MMI_IMAGE_ID_T line2_icon[] = {
        IMAGE_NULL,
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        IMAGE_IDLE_ICON_LINE2
#else
        //--------------------------SIM4 SIM3 SIM2 SIM1
        IMAGE_IDLE_ICON_LINE2_SIM1,//0x01------0001
        IMAGE_IDLE_ICON_LINE2_SIM2,//0x02------0010
        IMAGE_IDLE_ICON_LINE2_SIM1_SIM2,//0x03------0011

#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_ICON_LINE2_SIM3,//0x04------0100
        IMAGE_IDLE_ICON_LINE2_SIM1_SIM3,//0x05------0101
        IMAGE_IDLE_ICON_LINE2_SIM2_SIM3,//0x06------0110
        IMAGE_IDLE_ICON_LINE2_SIM1_SIM2_SIM3,//0x07------0111
#endif

#if defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_ICON_LINE2_SIM4,//0x08------1000
        IMAGE_IDLE_ICON_LINE2_SIM1_SIM4,//0x09------1001
        IMAGE_IDLE_ICON_LINE2_SIM2_SIM4,//0x0a------1010
        IMAGE_IDLE_ICON_LINE2_SIM1_SIM2_SIM4,//0x0b------1011
        IMAGE_IDLE_ICON_LINE2_SIM3_SIM4,//0x0c------1100
        IMAGE_IDLE_ICON_LINE2_SIM1_SIM3_SIM4,//0x0d------1101
        IMAGE_IDLE_ICON_LINE2_SIM2_SIM3_SIM4,//0x0e------1110
        IMAGE_IDLE_ICON_LINE2_SIM1_SIM2_SIM3_SIM4//0x0f------1111
#endif
#endif
    };

    if( !MMIAPISET_GetFlyMode())
    {
        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            //获取线路2的图标(只有在线路2的时候才显示)
            if ( (1 == MMIIDLE_GetIdleWinInfo()->line_state[i]) 
               &&(MMIAPIPHONE_IsSimAvailable((MN_DUAL_SYS_E)i)))
            {
                line2_index |= (0x01 << i);
                is_display = TRUE;
            }
        }
    }   
    
    //SCI_TRACE_LOW:"GetLineRoadIconId is_display=%d, line2_index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_1913_112_2_18_2_24_15_77,(uint8*)"dd", is_display, line2_index);
    
    if (is_display && line2_index < sizeof(line2_icon)/sizeof(line2_icon[0]))
    {
        image_id = line2_icon[line2_index];
    }
    else
    {
        image_id = IMAGE_NULL;
    }

    return image_id;
}

#endif //MMI_CSP_SUPPORT

/*****************************************************************************/
// Description : 显示漫游图标
// Global resource dependence : 
// Author: wancan.you
// Note:
/*****************************************************************************/
LOCAL void SetRoamingInfo(uint32 item_index, BOOLEAN is_append)
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    GUIWIN_STBDATA_ITEM_DATA_T item_data = {0};
    uint32 i = 0;
    uint16 roaming_index = 0;
    BOOLEAN is_display = FALSE;
    
    const MMI_IMAGE_ID_T roaming_icon[] = {
        IMAGE_NULL,
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        IMAGE_IDLE_ROAM
#else
        //--------------------SIM4 SIM3 SIM2 SIM1
        IMAGE_IDLE_ROAM_SIM1,//0x01------0001
        IMAGE_IDLE_ROAM_SIM2,//0x02------0010
        IMAGE_IDLE_ROAM_SIM1_SIM2,//0x03------0011

#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_ROAM_SIM3,//0x04------0100
        IMAGE_IDLE_ROAM_SIM1_SIM3,//0x05------0101
        IMAGE_IDLE_ROAM_SIM2_SIM3,//0x06------0110
        IMAGE_IDLE_ROAM_SIM1_SIM2_SIM3,//0x07------0111
#endif

#if defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_ROAM_SIM4,//0x08------1000
        IMAGE_IDLE_ROAM_SIM1_SIM4,//0x09------1001
        IMAGE_IDLE_ROAM_SIM2_SIM4,//0x0a------1010
        IMAGE_IDLE_ROAM_SIM1_SIM2_SIM4,//0x0b------1011
        IMAGE_IDLE_ROAM_SIM3_SIM4,//0x0c------1100
        IMAGE_IDLE_ROAM_SIM1_SIM3_SIM4,//0x0d------1101
        IMAGE_IDLE_ROAM_SIM2_SIM3_SIM4,//0x0e------1110
        IMAGE_IDLE_ROAM_SIM1_SIM2_SIM3_SIM4//0x0f------1111
#endif
#endif
    };

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (MMIPHONE_ROAMING_STATUS_NO_ROMING != MMIAPIPHONE_GetRoamingStatus(i)
            && MMIIDLE_GetIdleWinInfo()->is_roaming[i]
            && MMIAPIIDLE_NeedShowIconAccordingToDualSysSetting(i))
        {
            roaming_index |= (0x01 << i);
            is_display = TRUE;
        }
    }

    //SCI_TRACE_LOW:"SetRoamingInfo is_display=%d, roaming_index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_1810_112_2_18_2_24_14_76,(uint8*)"dd", is_display, roaming_index);

    if (is_display && roaming_index < sizeof(roaming_icon)/sizeof(roaming_icon[0]))
    {
        image_id = roaming_icon[roaming_index];
    }
    else
    {
        image_id = IMAGE_NULL;
    }

    item_data.icon_info.icon_num = 1;
    item_data.icon_info.icon_type = GUIWIN_STBDATA_ICON_NORMAL;
    item_data.icon_info.is_display = TRUE;
    item_data.cur_frame = 0;

    item_data.icon_info.icon_array[0] = image_id;
    item_data.app_handle_func = PNULL;
    item_data.is_handle_callback = FALSE;
    if (is_append)
    {
        GUIWIN_AppendStbItemData(&item_data);
    }
    else
    {
        GUIWIN_SetStbItemIconId(item_index, image_id);
    }

    if (IMAGE_NULL != image_id)
    {
        GUIWIN_SetStbItemVisible(item_index, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(item_index, FALSE);
    }
#if defined MMI_ISTYLE_SUPPORT   
    if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        GUIWIN_SetStbItemRightAlign(item_index, TRUE);
    }
    else
#endif		
    {
        GUIWIN_SetStbItemRightAlign(item_index, FALSE);
    }

}

/*****************************************************************************/
// Description : 显示运营商名称
//  Parameter: [In] dual_sys
//             [In] is_append
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
// Note:
/*****************************************************************************/
LOCAL void SetNetworkNameInfo( MN_DUAL_SYS_E dual_sys, BOOLEAN is_append) // Bug-1334911 显示运营商名称
{
    GUIWIN_STBDATA_ITEM_DATA_T  item_data   =   {0};
    MMI_STRING_T network = {0};
    wchar network_name[MMIPHONE_MAX_OPER_NAME_LEN] = {0};
    uint8 dst[MMIPHONE_MAX_OPER_NAME_LEN + 1] = {0};

    SCI_TRACE_LOW(" SetNetworkNameInfo enter");

    SCI_MEMSET(&network, 0x00, sizeof(network));
    network.wstr_ptr = (wchar *)network_name;
    SCI_MEMSET(network.wstr_ptr, 0x00, sizeof(network_name));
    // TODO:
    //如果是XTC版本，仅显示NOSIM/NO SERVICE/正常运营商 3种类型
#ifdef PRODUCT_CONFIG_UWS6131_XTC_I23
        MMIAPIPHONE_GetSimAndServiceStringForXtc(dual_sys, &network);
#else
        MMIAPIPHONE_GetSimAndServiceString(dual_sys, &network);
#endif

    item_data.cur_frame = 0;
    item_data.is_handle_callback = FALSE;
    item_data.icon_info.is_display = FALSE;
    item_data.text_info.is_display = TRUE;
    item_data.app_handle_func = 0;
    item_data.text_info.wstr_len = (uint32)network.wstr_len;
    item_data.text_info.font_color = MMI_WHITE_COLOR;
    item_data.text_info.align = ALIGN_LVMIDDLE;
    item_data.text_info.font_type = MMI_STATUS_FONT_TYPE;

    // 取最小值，防止越界
    item_data.text_info.wstr_len = MIN(item_data.text_info.wstr_len, MMISTATUSBAR_ITEM_TEXT_LEN);

    // cp字符串
    SCI_MEMCPY(item_data.text_info.wstr_ptr, network.wstr_ptr, sizeof(wchar)*item_data.text_info.wstr_len);

    if(is_append)
    {
        GUIWIN_AppendStbItemData(&item_data);
    }
    else
    {
        GUIWIN_SetStbItemText(MMI_WIN_NETWORK_NAME, &item_data.text_info);
    }

    GUIWIN_SetStbItemVisible(MMI_WIN_NETWORK_NAME, TRUE);
    GUIWIN_SetStbItemRightAlign(MMI_WIN_NETWORK_NAME, FALSE);
    SCI_TRACE_LOW(" SetNetworkNameInfo exit, network.wstr_len = %d, network.name = %s",network.wstr_len, MMIAPICOM_WstrToStr(network.wstr_ptr, dst));
}

/*****************************************************************************/
// Description : 显示GPS图标
//  Parameter: [In] dual_sys
//             [In] is_append
//             [Out] None
//             [Return] None
//  Author: qi.liu1
// Note:
/*****************************************************************************/
LOCAL void SetStatusBarGpsInfo(BOOLEAN is_enable,BOOLEAN is_append)
{
    GUIWIN_STBDATA_ITEM_DATA_T  item_data   =   {0};
	MMI_IMAGE_ID_T image_id = IMAGE_NULL;

    if(is_enable)
    {
        image_id = res_stat_gps;
        SCI_TRACE_LOW("SetStatusBarGpsInfo is_enable");
    }
    else
    {
        image_id = IMAGE_NULL;
    }

    item_data.cur_frame = 0;
    item_data.is_handle_callback = FALSE;
    item_data.app_handle_func = 0;
    item_data.icon_info.icon_num = 1;
    item_data.icon_info.icon_type = GUIWIN_STBDATA_ICON_NORMAL;
    item_data.icon_info.is_display = TRUE;
    item_data.cur_frame = 0;

    item_data.icon_info.icon_array[0] = image_id;
    item_data.app_handle_func = PNULL;
    item_data.is_handle_callback = FALSE;

    if(is_append)
    {
        GUIWIN_AppendStbItemData(&item_data);
    }
    else
    {
        GUIWIN_SetStbItemIconId(MMI_WIN_ICON_GPS, image_id);
    }

    if (IMAGE_NULL != image_id)
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_GPS, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_GPS, FALSE);
    }
}

/*****************************************************************************/
// Description : 获取线路设置的图标
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetLineCallForwardIconId(void)
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    uint32 i = 0;
    uint16 forward_index = 0;
    BOOLEAN is_display = FALSE;
    
    const MMI_IMAGE_ID_T forward_icon[] = {
        IMAGE_NULL,
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        IMAGE_IDLE_TITLE_ICON_FORWARD
#else
        //---------------------------------SIM4 SIM3 SIM2 SIM1
        IMAGE_IDLE_TITLE_ICON_FORWARD_SIM1,//0x01------0001
        IMAGE_IDLE_TITLE_ICON_FORWARD_SIM2,//0x02------0010
        IMAGE_IDLE_TITLE_ICON_FORWARD_SIM1_SIM2,//0x03------0011

#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_TITLE_ICON_FORWARD_SIM3,//0x04------0100
        IMAGE_IDLE_TITLE_ICON_FORWARD_SIM1_SIM3,//0x05------0101
        IMAGE_IDLE_TITLE_ICON_FORWARD_SIM2_SIM3,//0x06------0110
        IMAGE_IDLE_TITLE_ICON_FORWARD_SIM1_SIM2_SIM3,//0x07------0111
#endif

#if defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_IDLE_TITLE_ICON_FORWARD_SIM4,//0x08------1000
        IMAGE_IDLE_TITLE_ICON_FORWARD_SIM1_SIM4,//0x09------1001
        IMAGE_IDLE_TITLE_ICON_FORWARD_SIM2_SIM4,//0x0a------1010
        IMAGE_IDLE_TITLE_ICON_FORWARD_SIM1_SIM2_SIM4,//0x0b------1011
        IMAGE_IDLE_TITLE_ICON_FORWARD_SIM3_SIM4,//0x0c------1100
        IMAGE_IDLE_TITLE_ICON_FORWARD_SIM1_SIM3_SIM4,//0x0d------1101
        IMAGE_IDLE_TITLE_ICON_FORWARD_SIM2_SIM3_SIM4,//0x0e------1110
        IMAGE_IDLE_TITLE_ICON_FORWARD_SIM1_SIM2_SIM3_SIM4//0x0f------1111
#endif
#endif
    };

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (1 == MMIIDLE_GetIdleWinInfo()->is_forward[i])//只有在线路2下在显示
        {
            forward_index |= (0x01 << i);
            is_display = TRUE;
        }
    }

    //SCI_TRACE_LOW:"GetLineCallForwardIconId is_display=%d, forward_index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_1977_112_2_18_2_24_15_78,(uint8*)"dd", is_display, forward_index);

    if (is_display && forward_index < sizeof(forward_icon)/sizeof(forward_icon[0]))
    {
        image_id = forward_icon[forward_index];
    }
    else
    {
        image_id = IMAGE_NULL;
    }

    return image_id;
}

/*****************************************************************************/
// Description : 获取环境设置的图标
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetEvnSetIconId(void)
{
    MMI_IMAGE_ID_T  image_id        = IMAGE_NULL;
    uint8           envset_mode     = 0;

    envset_mode = MMIAPIENVSET_GetActiveModeId();
    switch(envset_mode)
    {    
    case MMIENVSET_STANDARD_MODE:  // 普通环境（常规）
        image_id = (MMI_IMAGE_ID_T)IMAGE_NULL;
        break;
        
    case MMIENVSET_SILENT_MODE:     // 安静环境（静音）
        image_id = (MMI_IMAGE_ID_T)IMAGE_IDLE_TITLE_ICON_SILENT;
        break;
        
    case MMIENVSET_MEETING_MODE:     // 会议环境（震动）
        image_id = (MMI_IMAGE_ID_T)IMAGE_IDLE_TITLE_ICON_VIBRATION;
        break;
        
    case MMIENVSET_INSIDE_MODE:     // 室内环境（低音）
        image_id = (MMI_IMAGE_ID_T)IMAGE_IDLE_TITLE_ICON_LOW_RING;
        break;
        
    case MMIENVSET_NOISY_MODE:       //户外环境（高音震动）
        image_id = (MMI_IMAGE_ID_T)IMAGE_IDLE_TITLE_ICON_RING_VIBRA;
        break;

    default:
        image_id = (MMI_IMAGE_ID_T)IMAGE_NULL;
        break;
    };
    
    return image_id;
}


/*****************************************************************************/
//  Description : set the roaming state of the idle window
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleRoamingState(
                                   MN_DUAL_SYS_E    e_dual_sys,
                                   BOOLEAN is_roaming
                                   )
{

    if (MMI_DUAL_SYS_MAX <= e_dual_sys)
    {
        //MMI_ASSERT_WARNING("e_dual_sys < MMI_DUAL_SYS_MAX");
        //SCI_TRACE_LOW:"MAIN_SetIdleRoamingState ERROR! MMI_DUAL_SYS_MAX <= e_dual_sys,e_dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_2045_112_2_18_2_24_15_79,(uint8*)"d",e_dual_sys);
        return;
    }
    //SCI_TRACE_LOW:"MAIN_SetIdleRoamingState: e_dual_sys=%d, is_roaming = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_2048_112_2_18_2_24_15_80,(uint8*)"dd", e_dual_sys, is_roaming);

    if(is_roaming != MMIIDLE_GetIdleWinInfo()->is_roaming[e_dual_sys])
    {
        MMIIDLE_GetIdleWinInfo()->is_roaming[e_dual_sys] = is_roaming;

        SetRoamingInfo(MMI_WIN_ICON_ROAMING, FALSE);

        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE        
        //刷新小屏的显示，Roaming状态
        MMISUB_UpdateDisplaySubLcd(); 
#endif
    }
}

PUBLIC void MAIN_SetIdleGpsState(BOOLEAN is_enable)
{
    SCI_TRACE_LOW("MAIN_SetIdleGpsState Enter");
    SetStatusBarGpsInfo(is_enable,FALSE);
    // update
    GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
    //刷新小屏的显示
    MMISUB_UpdateDisplaySubLcd();
#endif
}

/*****************************************************************************/
//  Description : set the Alarm state of the idle window
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_UpdateIdleEnvsetState(void)
{
    MMI_IMAGE_ID_T      image_id    = IMAGE_NULL;

    image_id = GetEvnSetIconId();

    GUIWIN_SetStbItemIconId(MMI_WIN_ICON_ENVSET, image_id);
    GUIWIN_SetStbItemIconCurFrame(MMI_WIN_ICON_ENVSET, 0);



    if (image_id != IMAGE_NULL)
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_ENVSET, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_ENVSET, FALSE);
    }
#if defined MMI_ISTYLE_SUPPORT    
    if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_ENVSET, TRUE);
    }
    else
#endif		
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_ENVSET, FALSE);
    }
    
    // update
    GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE    
    //刷新小屏的显示，短信状态
    MMISUB_UpdateDisplaySubLcd();
#endif
}


/*****************************************************************************/
//  Description : set the call forward state of the idle window
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleForwardState(
                                     MN_DUAL_SYS_E dual_sys,
                                     BOOLEAN forward_state
                                     )
{
    BOOLEAN             is_update   = FALSE;
    MMI_IMAGE_ID_T      image_id    = IMAGE_NULL;
    
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        if (forward_state != MMIIDLE_GetIdleWinInfo()->is_forward[dual_sys])
        {
            MMIIDLE_GetIdleWinInfo()->is_forward[dual_sys] = forward_state;
            is_update = TRUE;
        }
    }

    // 更改图标
    image_id = GetLineCallForwardIconId();
    GUIWIN_SetStbItemIconId(MMI_WIN_ICON_CALL_FORWARD, image_id);
    // 设置item可见性
    
    if (image_id != IMAGE_NULL)
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_CALL_FORWARD, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_CALL_FORWARD, FALSE);
    }
#if defined MMI_ISTYLE_SUPPORT    
    if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_CALL_FORWARD, TRUE);
    }
    else
#endif		
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_CALL_FORWARD, FALSE);
    }

    if (is_update)
    {
        // 刷新控件
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE        
        //刷新小屏的显示，闹钟状态
        MMISUB_UpdateDisplaySubLcd();   
#endif
    }
}

/*****************************************************************************/
//  Description : 判断是否时号码快速查询
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetMessageDispImageId(void)
{
    MMI_IMAGE_ID_T          image_id    =   IMAGE_NULL;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPISMS_IsOrderOk())
    {
        image_id = IMAGE_NULL;
    }
    else if (MMIAPISMS_IsSMSFull()) //短信满
    {
        image_id = IMAGE_IDLE_TITLE_ICON_FULL_MESSAGE;
    }
//     else if (MMIAPIMMS_IsMMSFull()) //彩信满
//     {
//         image_id = IMAGE_IDLE_TITLE_ICON_FULL_MMS_DUA;
//     }
#ifndef MONOCHROME_LCD_SUPPORT
    else if (MMIAPISMS_IsSendingSMS())
    {
        image_id = IMAGE_IDLE_TITLE_ICON_MESSAGE_SEND;
    }
#endif	
    else if(MMIAPIMMS_IsSendingMMS())//彩信下载中
    {
        image_id = IMAGE_IDLE_TITLE_ICON_MMSSEND;
    }
    else if(MMIAPIMMS_IsDownloadingMMS())//彩信下载中
    {
        image_id = IMAGE_IDLE_TITLE_ICON_MMS;
    }
    else if (0 < MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_SMS)) //未读短信
    {
        image_id = IMAGE_IDLE_TITLE_ICON_MESSAGE;
    }
    else if (0 < MMIAPISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_MAX))//未下载彩信
    {
        image_id = IMAGE_IDLE_TITLE_ICON_MMS_DUA;
    }
    else if (0 < MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_MMS)) //未读彩信
    {
        image_id = IMAGE_IDLE_TITLE_ICON_MMS_DUA;
    }
    //else if ( MMICMSBRW_IsPushFull()) //push信箱满
    //{
//        image_id = IMAGE_CMSBRW_PUSH_FULL;
    //}
    else if (0 < MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_WAP_PUSH)) //未读push消息
    {
        image_id = IMAGE_IDLE_TITLE_ICON_NEW_PUSH_ICON;
    }
    else if (0 < MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_WAP_OTA)) //未读WAP OTA
    {
        image_id = IMAGE_IDLE_TITLE_ICON_OTA;
    }
#ifdef CMMB_SUPPORT    
    else if(MMIAPIMTV_IsHaveTobeReadEBMsg())  //未读紧急广播
    {
        image_id = IMAGE_IDLE_TITLE_ICON_EB;  
    }
#endif    
    else if (MMIIDLE_GetIdleWinInfo()->is_new_vm)
    {
        //(11)显示新的语音信息
        image_id = IMAGE_IDLE_ICON_NEW_VM;
    }
#else
    if (!MMIAPISMS_IsOrderOk())
    {
        image_id = IMAGE_NULL;
    }
    else if (MMIAPISMS_IsSMSFull())//短信满
    {
        image_id = IMAGE_IDLE_TITLE_ICON_FULL_MESSAGE;
    }
#ifndef MONOCHROME_LCD_SUPPORT
    else if (MMIAPISMS_IsSendingSMS())
    {
        image_id = IMAGE_IDLE_TITLE_ICON_MESSAGE_SEND;
    }
#endif
    else if(MMIAPIMMS_IsSendingMMS())//彩信下载中
    {
        image_id = IMAGE_IDLE_TITLE_ICON_MMSSEND;
    }
    else if(MMIAPIMMS_IsDownloadingMMS())//彩信下载中
    {
        image_id = IMAGE_IDLE_TITLE_ICON_MMS;
    }
    else if (0 < MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_1, MMISMS_TYPE_SMS)) //未读短信
    {
        image_id = IMAGE_IDLE_TITLE_ICON_MESSAGE;
    }
    else if (0 < MMIAPISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_1))//未下载彩信
    {
        image_id = IMAGE_IDLE_TITLE_ICON_MMS_DUA;
    }
    else if (0 < MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_1, MMISMS_TYPE_MMS)) //未读彩信
    {
        image_id = IMAGE_IDLE_TITLE_ICON_MMS_DUA;
    }
    //else if( MMICMSBRW_IsPushFull()) //push信箱满
    //{
//        image_id = IMAGE_CMSBRW_PUSH_FULL;
    //}
    else if (0 < MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_1, MMISMS_TYPE_WAP_PUSH)) //未读push消息
    {
        image_id = IMAGE_IDLE_TITLE_ICON_NEW_PUSH_ICON;
    }
    else if (0 < MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_1, MMISMS_TYPE_WAP_OTA)) //未读WAP OTA
    {
        image_id = IMAGE_IDLE_TITLE_ICON_OTA;
    }
#ifdef CMMB_SUPPORT    
    else if(MMIAPIMTV_IsHaveTobeReadEBMsg())  //未读紧急广播
    {
        image_id = IMAGE_IDLE_TITLE_ICON_EB;  
    }
#endif    
    else if (MMIIDLE_GetIdleWinInfo()->is_new_vm)
    {
        //(11)显示新的语音信息
        image_id = IMAGE_IDLE_ICON_NEW_VM;
    }
#endif
    
    return image_id;
}


/*****************************************************************************/
//  Description : set msg state
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetMsgState(BOOLEAN is_draw)
{
    MMI_IMAGE_ID_T      image_id    =   IMAGE_NULL;

    image_id = GetMessageDispImageId();

    GUIWIN_SetStbItemIconId(MMI_WIN_ICON_MESSAGE, image_id);
    GUIWIN_SetStbItemIconCurFrame(MMI_WIN_ICON_MESSAGE, 0);

    if (image_id != IMAGE_NULL)
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_MESSAGE, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_MESSAGE, FALSE);
    }
#if defined MMI_ISTYLE_SUPPORT    
    if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_MESSAGE, TRUE);
    }
    else
#endif		
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_MESSAGE, FALSE);
    }
    
    if (is_draw)
    {
        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE        
        //刷新小屏的显示，短信状态
        MMISUB_UpdateDisplaySubLcd();
#endif
    }
}

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
/*****************************************************************************/
//  Description : set msg state
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetBtMsgState(BOOLEAN is_draw)
{
    MMI_IMAGE_ID_T      image_id    =   IMAGE_NULL;

    image_id = GetMessageDispImageId();

    GUIWIN_SetStbItemIconId(MMI_WIN_ICON_MESSAGE, image_id);
    GUIWIN_SetStbItemIconCurFrame(MMI_WIN_ICON_MESSAGE, 0);

    if (image_id != IMAGE_NULL)
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_MESSAGE, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_MESSAGE, FALSE);
    }
    
    if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_MESSAGE, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_MESSAGE, FALSE);
    }
    
    if (is_draw)
    {
        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE        
        //刷新小屏的显示，短信状态
        MMISUB_UpdateDisplaySubLcd();
#endif
    }
}
#endif

/*****************************************************************************/
// Description : 显示图标
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_SetGprsInfo(
                                    MN_DUAL_SYS_E dual_sys,
                                    MMIPDP_NOTIFY_EVENT_E pdp_event,
                                    BOOLEAN is_append
                                    )
{
    SetGprsInfo(dual_sys, pdp_event,is_append);
}
    
/*****************************************************************************/
// Description : 显示图标
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
LOCAL void SetGprsInfo(
                                                MN_DUAL_SYS_E dual_sys,
                                                MMIPDP_NOTIFY_EVENT_E pdp_event,
                                                BOOLEAN is_append
                                                )
{
#if defined CMCC_UI_STYLE || defined NET_ICON_UI_STYLE
    uint32 i = 0;
    if (MMI_DUAL_SYS_MAX <= dual_sys)
    {
        //SCI_TRACE_LOW:"SetGprsInfo error dual_sys: %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_2371_112_2_18_2_24_16_81,(uint8*)"d", dual_sys);
        return;
    }
    SCI_TRACE_LOW("[SetGprsInfo] 1 gprs_state s_pdp_event[%d] = %d", dual_sys, s_pdp_event[dual_sys]);
    s_pdp_event[dual_sys] = (pdp_event <= MMIPDP_NOTIFY_EVENT_MAX) ? pdp_event : MMIPDP_NOTIFY_EVENT_MAX;
    SCI_TRACE_LOW("[SetGprsInfo] 2 gprs_state s_pdp_event[%d] = %d", dual_sys, s_pdp_event[dual_sys]);

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        MMIIDLE_SetSimStatusBarInfo((MN_DUAL_SYS_E)i, MMI_WIN_ICON_SIGNAL + i, FALSE);
    }
#else
#ifdef PRODUCT_DM
    BOOLEAN is_sim_ok = FALSE;
    MMI_IMAGE_ID_T image_id    =   IMAGE_NULL;
    GUIWIN_STBDATA_ITEM_DATA_T item_data   =   {0};
    MMIPHONE_DATA_SERVICE_TYPE_E data_service_type = MMIAPIPHONE_GetDataServiceType(dual_sys);/*@CR120265 modify 2011.09.07*/
    MMI_GMMREG_RAT_E    network_status_rat = MMI_GMMREG_RAT_GPRS;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    BOOLEAN is_sim_attached = FALSE;
    MN_DUAL_SYS_E index = MN_DUAL_SYS_1;  
#endif
    MN_PHONE_PLMN_STATUS_E plmn_status = PLMN_NO_SERVICE;

    const MMI_IMAGE_ID_T data_sevice_image[DATA_SERVICE_TYPE_MAX][MMIPDP_NOTIFY_EVENT_MAX]=
    {
        {IMAGE_NULL,IMAGE_NULL,IMAGE_NULL,IMAGE_NULL},
        {IMAGE_IDLE_TITLE_ICON_GPRS_DATAING, IMAGE_IDLE_TITLE_ICON_GPRS_ACTIVE,IMAGE_IDLE_TITLE_ICON_GPRS_ACTIVE, IMAGE_NULL},
        {IMAGE_IDLE_TITLE_ICON_EDGE_DATAING, IMAGE_IDLE_TITLE_ICON_EDGE_ACTIVE,IMAGE_IDLE_TITLE_ICON_EDGE_ACTIVE, IMAGE_NULL},
        {IMAGE_IDLE_TITLE_ICON_TDDATA_DATAING, IMAGE_IDLE_TITLE_ICON_TDDATA_ACTIVE,IMAGE_IDLE_TITLE_ICON_TDDATA_ACTIVE, IMAGE_NULL},
        {IMAGE_IDLE_TITLE_ICON_HSDPA_DATAING, IMAGE_IDLE_TITLE_ICON_HSDPA_ACTIVE,IMAGE_IDLE_TITLE_ICON_HSDPA_ACTIVE, IMAGE_NULL}
    };

    if (MMI_DUAL_SYS_MAX <= dual_sys)
    {
        //SCI_TRACE_LOW:"SetGprsInfo error dual_sys: %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_2371_112_2_18_2_24_16_81,(uint8*)"d", dual_sys);
        return;
    }    

    network_status_rat = MMIAPIPHONE_GetTDOrGsm(dual_sys);
    plmn_status = MMIAPIPHONE_GetPlmnStatus(dual_sys);

    if (MMIAPIPHONE_IsSimAvailable(dual_sys)
        && (PLMN_NORMAL_GPRS_ONLY == plmn_status
            || PLMN_NORMAL_GSM_GPRS_BOTH == plmn_status)
        && MMI_GMMREG_RAT_UNKNOWN != network_status_rat)//RAT_UNKNOWN时不显示GPRS图标
    {
        is_sim_ok = TRUE;
    }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    for (index = MN_DUAL_SYS_1; index < MMI_DUAL_SYS_MAX; index++)
    {
        if (s_is_gprs_attach[index])
        {
            SCI_TRACE_LOW(" exist_attach_sim index:%d", index);
            if(index != dual_sys)
            {
                return;
            }
        }
    }
#endif
	

    s_is_gprs_attach[dual_sys] = (pdp_event < MMIPDP_NOTIFY_EVENT_DETTACH);

    SCI_TRACE_LOW("SetGprsInfo dual_sys:%d type:%d event:%d,network_status_rat:%d, plmn_status:%d", dual_sys, data_service_type, pdp_event, network_status_rat, plmn_status);

    if((DATA_SERVICE_TYPE_MAX <= data_service_type) || (MMIPDP_NOTIFY_EVENT_MAX <= pdp_event))
    {
        //SCI_TRACE_LOW:"SetGprsInfo data error! data_service_type = %d,pdp_event = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_2390_112_2_18_2_24_16_83,(uint8*)"dd",data_service_type,pdp_event);
        return;
    }
    if (MMIDM_PDP_STATUS_NONE == MMIAPIDM_GetPdpActiveStatus())
    {
        item_data.icon_info.icon_num = 1;
        item_data.icon_info.icon_type = GUIWIN_STBDATA_ICON_NORMAL;
        item_data.icon_info.is_display = TRUE;
        item_data.cur_frame = 0;
        item_data.is_handle_callback = TRUE;

#ifdef MMI_MULTI_SIM_SYS_SINGLE
        image_id = data_sevice_image[data_service_type][pdp_event];
#else        
//         for (index = MN_DUAL_SYS_1; index < MMI_DUAL_SYS_MAX; index++)
//         {
            if (s_is_gprs_attach[dual_sys])
            {
                is_sim_attached = TRUE;
                SCI_TRACE_LOW("SetGprsInfo attach_index:%d", index);
               /* break;*/
            }
        /*}*/
        //set dating icon
        if(MMIPDP_NOTIFY_EVENT_ACTIVED == pdp_event)
        {
            image_id = GetDatingIconId(dual_sys, data_service_type);
        }
        //set active icon
        else if (is_sim_attached)
        {
            is_sim_ok = TRUE;
            image_id = GetActiveIconId(data_service_type);
        }
#endif

        item_data.icon_info.icon_array[0] = image_id;
        item_data.app_handle_func = PNULL;
        item_data.is_handle_callback = TRUE;
        if(is_append)
        {
            GUIWIN_AppendStbItemData(&item_data);
        }
        else
        {
            GUIWIN_SetStbItemIconId(MMI_WIN_ICON_GPRS, image_id);
        }
        
        if (is_sim_ok)
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_GPRS, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_GPRS, FALSE);
        }
#if defined MMI_ISTYLE_SUPPORT
        if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_GPRS, TRUE);
        }
        else
#endif			
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_GPRS, FALSE);
        }

        /*@CR255469 modify 2011.08.23*/
        MMIAPIPDP_SetPdpLastEvent(dual_sys, pdp_event);//记录最后一次消息，在rat切换的时候更新图标
    }
#else //PRODUCT_DM
    uint32 i = 0;

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        MMIIDLE_SetSimStatusBarInfo((MN_DUAL_SYS_E)i, MMI_WIN_ICON_SIGNAL + i, FALSE);
    }

#endif
#endif
}

/*****************************************************************************/
//  Description : set the gprs state of the idle window
//  Global resource dependence : 
//  Author: figo.feng
//  Note: DUAL_MODE下只做关闭图标用
/*****************************************************************************/
PUBLIC void MAIN_SetIdleGprsState(
                                   MN_DUAL_SYS_E    e_dual_sys,
                                   BOOLEAN          gprs_state
                                   )
{
#if defined(HERO_ENGINE_SUPPORT)

	{
		extern BOOL hero_gprs_is_connected();
		if(hero_gprs_is_connected())
		{
			return;
		}
	}

#endif

    if (MMI_DUAL_SYS_MAX <= e_dual_sys)
    {
        //MMI_ASSERT_WARNING("e_dual_sys < MMI_DUAL_SYS_MAX");
        //SCI_TRACE_LOW:"MAIN_SetIdleGprsState ERROR! MMI_DUAL_SYS_MAX <= e_dual_sys,e_dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_2481_112_2_18_2_24_16_84,(uint8*)"d",e_dual_sys);
        return;
    }
    //SCI_TRACE_LOW:"MAIN_SetIdleGprsState: e_dual_sys=%d, gprs_state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_2484_112_2_18_2_24_16_85,(uint8*)"dd", e_dual_sys, gprs_state);
#ifdef MODEM_SUPPORT_RDA
    if (gprs_state)
    {
        SCI_TRACE_LOW("[MAIN_SetIdleGprsState] gprs_state = %d", gprs_state);
        // Bug-1308884 merge from NQ2
        if(MMIAPIPDP_PsIsActivedPdpLinkExistEx(e_dual_sys) && (MMIPDP_NOTIFY_EVENT_ACTIVED == s_pdp_event[e_dual_sys]))  /*@CR255469 modify 2011.08.23*/
        {
            SCI_TRACE_LOW("[MAIN_SetIdleGprsState] MMIPDP_NOTIFY_EVENT_ACTIVED");
            SetGprsInfo(e_dual_sys, MMIPDP_NOTIFY_EVENT_ACTIVED, FALSE);
        }
        else
        {
            if(TRUE == ual_tele_data_get_is_active(e_dual_sys))
            {
                SCI_TRACE_LOW("[MAIN_SetIdleGprsState] MMIPDP_NOTIFY_EVENT_ACTIVED");
                SetGprsInfo(e_dual_sys, MMIPDP_NOTIFY_EVENT_ACTIVED, FALSE);
            }
            else
            {
                SCI_TRACE_LOW("[MAIN_SetIdleGprsState] MMIPDP_NOTIFY_EVENT_ATTACH");
                SetGprsInfo(e_dual_sys, MMIPDP_NOTIFY_EVENT_ATTACH, FALSE);
            }

        }
    }
    else
    {
        SCI_TRACE_LOW("[MAIN_SetIdleGprsState] MMIPDP_NOTIFY_EVENT_DETTACH");
        SetGprsInfo(e_dual_sys, MMIPDP_NOTIFY_EVENT_DETTACH, FALSE);
    }
    // update
    GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
    //刷新小屏的显示，Gprs状态
    MMISUB_UpdateDisplaySubLcd();
#endif

#else   //MODEM_SUPPORT_SPRD

#ifdef PRODUCT_DM
    if (gprs_state)
    {
        // Bug-1308884 merge from NQ2
        if(MMIAPIPDP_PsIsActivedPdpLinkExistEx(e_dual_sys) && (MMIPDP_NOTIFY_EVENT_ACTIVED == s_pdp_event[e_dual_sys]))  /*@CR255469 modify 2011.08.23*/
        {
            SetGprsInfo(e_dual_sys, MMIPDP_NOTIFY_EVENT_ACTIVED, FALSE);
        }
        else
        {
            SetGprsInfo(e_dual_sys, MMIPDP_NOTIFY_EVENT_ATTACH, FALSE);
        }
    }
    else
    {
        SetGprsInfo(e_dual_sys, MMIPDP_NOTIFY_EVENT_DETTACH, FALSE);
    }
    // update
    GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE    
    //刷新小屏的显示，Gprs状态
    MMISUB_UpdateDisplaySubLcd();  
#endif
#else
    if(gprs_state != MMIIDLE_GetIdleWinInfo()->is_gprs[e_dual_sys])
    {
        MMIIDLE_GetIdleWinInfo()->is_gprs[e_dual_sys] = gprs_state;
        if(!gprs_state)//只在gprs没有开启的情况下进行处理
        {
            SetGprsInfo(e_dual_sys, MMIPDP_NOTIFY_EVENT_DETTACH,FALSE);
            // update
            GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE            
            //刷新小屏的显示，Gprs状态
            MMISUB_UpdateDisplaySubLcd(); 
#endif
        }
    }
#endif
#endif

#ifdef MRAPP_SUPPORT
	MMIMRAPP_setPhoneState(E_MR_MD_GPRS, gprs_state);
#endif
#if defined(HERO_ENGINE_SUPPORT)
	MMIHEROAPP_setPhoneState(E_HERO_MD_GPRS, gprs_state);
#endif
}
#ifdef PRODUCT_DM
/*****************************************************************************/
// Description : get ps active icon
// Global resource dependence : 
// Author: wancan.you
// Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetActiveIconId(MMIPHONE_DATA_SERVICE_TYPE_E data_service_type)
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    uint32 i = 0;
    uint16 index = 0;
    
    const MMI_IMAGE_ID_T active_icon[DATA_SERVICE_TYPE_MAX][ACTIVE_ICON_MAX] = {
        //NO_DATA_SERVICE_TYPE
        {
            IMAGE_NULL
        },/*lint !e785*/
        //GPRS_DATA_SERVICE_TYPE
        {
            IMAGE_NULL,
#ifdef MMI_MULTI_SIM_SYS_SINGLE
            IMAGE_IDLE_TITLE_ICON_GPRS_ACTIVE
#else
            //--------------------------SIM4 SIM3 SIM2 SIM1
            IMAGE_IDLE_ICON_GPRS_ACTIVE_SIM1,//0x01------0001
            IMAGE_IDLE_ICON_GPRS_ACTIVE_SIM2,//0x02------0010
            IMAGE_IDLE_ICON_GPRS_ACTIVE_SIM1_SIM2,//0x03------0011

#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_ICON_GPRS_ACTIVE_SIM3,//0x04------0100
            IMAGE_IDLE_ICON_GPRS_ACTIVE_SIM1_SIM3,//0x05------0101
            IMAGE_IDLE_ICON_GPRS_ACTIVE_SIM2_SIM3,//0x06------0110
            IMAGE_IDLE_ICON_GPRS_ACTIVE_SIM1_SIM2_SIM3,//0x07------0111
#endif

#if defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_ICON_GPRS_ACTIVE_SIM4,//0x08------1000
            IMAGE_IDLE_ICON_GPRS_ACTIVE_SIM1_SIM4,//0x09------1001
            IMAGE_IDLE_ICON_GPRS_ACTIVE_SIM2_SIM4,//0x0a------1010
            IMAGE_IDLE_ICON_GPRS_ACTIVE_SIM1_SIM2_SIM4,//0x0b------1011
            IMAGE_IDLE_ICON_GPRS_ACTIVE_SIM3_SIM4,//0x0c------1100
            IMAGE_IDLE_ICON_GPRS_ACTIVE_SIM1_SIM3_SIM4,//0x0d------1101
            IMAGE_IDLE_ICON_GPRS_ACTIVE_SIM2_SIM3_SIM4,//0x0e------1110
            IMAGE_IDLE_ICON_GPRS_ACTIVE_SIM1_SIM2_SIM3_SIM4//0x0f------1111
#endif
#endif            
        },
        //EDGE_DATA_SERVICE_TYPE
        {
            IMAGE_NULL,
#ifdef MMI_MULTI_SIM_SYS_SINGLE
            IMAGE_IDLE_TITLE_ICON_EDGE_ACTIVE
#else
            //--------------------------SIM4 SIM3 SIM2 SIM1
            IMAGE_IDLE_ICON_EDGE_ACTIVE_SIM1,//0x01------0001
            IMAGE_IDLE_ICON_EDGE_ACTIVE_SIM2,//0x02------0010
            IMAGE_IDLE_ICON_EDGE_ACTIVE_SIM1_SIM2,//0x03------0011

#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_ICON_EDGE_ACTIVE_SIM3,//0x04------0100
            IMAGE_IDLE_ICON_EDGE_ACTIVE_SIM1_SIM3,//0x05------0101
            IMAGE_IDLE_ICON_EDGE_ACTIVE_SIM2_SIM3,//0x06------0110
            IMAGE_IDLE_ICON_EDGE_ACTIVE_SIM1_SIM2_SIM3,//0x07------0111
#endif

#if defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_ICON_EDGE_ACTIVE_SIM4,//0x08------1000
            IMAGE_IDLE_ICON_EDGE_ACTIVE_SIM1_SIM4,//0x09------1001
            IMAGE_IDLE_ICON_EDGE_ACTIVE_SIM2_SIM4,//0x0a------1010
            IMAGE_IDLE_ICON_EDGE_ACTIVE_SIM1_SIM2_SIM4,//0x0b------1011
            IMAGE_IDLE_ICON_EDGE_ACTIVE_SIM3_SIM4,//0x0c------1100
            IMAGE_IDLE_ICON_EDGE_ACTIVE_SIM1_SIM3_SIM4,//0x0d------1101
            IMAGE_IDLE_ICON_EDGE_ACTIVE_SIM2_SIM3_SIM4,//0x0e------1110
            IMAGE_IDLE_ICON_EDGE_ACTIVE_SIM1_SIM2_SIM3_SIM4//0x0f------1111
#endif
#endif            
        },
        //TD_DATA_SERVICE_TYPE
        {
            IMAGE_NULL,
#ifdef MMI_MULTI_SIM_SYS_SINGLE
            IMAGE_IDLE_TITLE_ICON_TDDATA_ACTIVE
#else
            //--------------------------SIM4 SIM3 SIM2 SIM1
            IMAGE_IDLE_ICON_TDDATA_ACTIVE_SIM1,//0x01------0001
            IMAGE_IDLE_ICON_TDDATA_ACTIVE_SIM2,//0x02------0010
            IMAGE_IDLE_ICON_TDDATA_ACTIVE_SIM1_SIM2,//0x03------0011

#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_ICON_TDDATA_ACTIVE_SIM3,//0x04------0100
            IMAGE_IDLE_ICON_TDDATA_ACTIVE_SIM1_SIM3,//0x05------0101
            IMAGE_IDLE_ICON_TDDATA_ACTIVE_SIM2_SIM3,//0x06------0110
            IMAGE_IDLE_ICON_TDDATA_ACTIVE_SIM1_SIM2_SIM3,//0x07------0111
#endif

#if defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_ICON_TDDATA_ACTIVE_SIM4,//0x08------1000
            IMAGE_IDLE_ICON_TDDATA_ACTIVE_SIM1_SIM4,//0x09------1001
            IMAGE_IDLE_ICON_TDDATA_ACTIVE_SIM2_SIM4,//0x0a------1010
            IMAGE_IDLE_ICON_TDDATA_ACTIVE_SIM1_SIM2_SIM4,//0x0b------1011
            IMAGE_IDLE_ICON_TDDATA_ACTIVE_SIM3_SIM4,//0x0c------1100
            IMAGE_IDLE_ICON_TDDATA_ACTIVE_SIM1_SIM3_SIM4,//0x0d------1101
            IMAGE_IDLE_ICON_TDDATA_ACTIVE_SIM2_SIM3_SIM4,//0x0e------1110
            IMAGE_IDLE_ICON_TDDATA_ACTIVE_SIM1_SIM2_SIM3_SIM4//0x0f------1111
#endif
#endif            
        },
        //HSDPA_DATA_SERVICE_TYPE
        {
            IMAGE_NULL,
#ifdef MMI_MULTI_SIM_SYS_SINGLE
            IMAGE_IDLE_TITLE_ICON_HSDPA_ACTIVE
#else
            //--------------------------SIM4 SIM3 SIM2 SIM1
            IMAGE_IDLE_ICON_HSDPA_ACTIVE_SIM1,//0x01------0001
            IMAGE_IDLE_ICON_HSDPA_ACTIVE_SIM2,//0x02------0010
            IMAGE_IDLE_ICON_HSDPA_ACTIVE_SIM1_SIM2,//0x03------0011

#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_ICON_HSDPA_ACTIVE_SIM3,//0x04------0100
            IMAGE_IDLE_ICON_HSDPA_ACTIVE_SIM1_SIM3,//0x05------0101
            IMAGE_IDLE_ICON_HSDPA_ACTIVE_SIM2_SIM3,//0x06------0110
            IMAGE_IDLE_ICON_HSDPA_ACTIVE_SIM1_SIM2_SIM3,//0x07------0111
#endif

#if defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_ICON_HSDPA_ACTIVE_SIM4,//0x08------1000
            IMAGE_IDLE_ICON_HSDPA_ACTIVE_SIM1_SIM4,//0x09------1001
            IMAGE_IDLE_ICON_HSDPA_ACTIVE_SIM2_SIM4,//0x0a------1010
            IMAGE_IDLE_ICON_HSDPA_ACTIVE_SIM1_SIM2_SIM4,//0x0b------1011
            IMAGE_IDLE_ICON_HSDPA_ACTIVE_SIM3_SIM4,//0x0c------1100
            IMAGE_IDLE_ICON_HSDPA_ACTIVE_SIM1_SIM3_SIM4,//0x0d------1101
            IMAGE_IDLE_ICON_HSDPA_ACTIVE_SIM2_SIM3_SIM4,//0x0e------1110
            IMAGE_IDLE_ICON_HSDPA_ACTIVE_SIM1_SIM2_SIM3_SIM4//0x0f------1111
#endif
#endif            
        }
    };


    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (s_is_gprs_attach[i])
        {
            index |= (0x01 << i);
        }
    }

    //SCI_TRACE_LOW:"GetActiveIconId data_service_type: %d, index: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_2672_112_2_18_2_24_16_86,(uint8*)"dd", data_service_type, index);
    
    if (index < sizeof(active_icon[1])/sizeof(active_icon[1][0]))
    {
        image_id = active_icon[data_service_type][index];
    }
    else
    {
        image_id = IMAGE_NULL;
    }

    return image_id;
}

/*****************************************************************************/
// Description : get ps dating icon
// Global resource dependence : 
// Author: wancan.you
// Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetDatingIconId(MN_DUAL_SYS_E dual_sys, MMIPHONE_DATA_SERVICE_TYPE_E data_service_type)
{
    const MMI_IMAGE_ID_T dating_icon[DATA_SERVICE_TYPE_MAX][MMI_DUAL_SYS_MAX] = {
        //NO_DATA_SERVICE_TYPE
        {
            IMAGE_NULL
        },
        //GPRS_DATA_SERVICE_TYPE
        {
#ifdef MMI_MULTI_SIM_SYS_SINGLE
            IMAGE_IDLE_TITLE_ICON_GPRS_DATAING
#else
            IMAGE_IDLE_TITLE_ICON_GPRS_DATAING_SIM1,
            IMAGE_IDLE_TITLE_ICON_GPRS_DATAING_SIM2,

#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_TITLE_ICON_GPRS_DATAING_SIM3,
#endif

#if defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_TITLE_ICON_GPRS_DATAING_SIM4,

#endif
#endif            
        },
        //EDGE_DATA_SERVICE_TYPE
        {
#ifdef MMI_MULTI_SIM_SYS_SINGLE
            IMAGE_IDLE_TITLE_ICON_EDGE_DATAING
#else
            IMAGE_IDLE_TITLE_ICON_EDGE_DATAING_SIM1,
            IMAGE_IDLE_TITLE_ICON_EDGE_DATAING_SIM2,

#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_TITLE_ICON_EDGE_DATAING_SIM3,
#endif

#if defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_TITLE_ICON_EDGE_DATAING_SIM4,

#endif
#endif                 
        },
        //TD_DATA_SERVICE_TYPE
        {
#ifdef MMI_MULTI_SIM_SYS_SINGLE
            IMAGE_IDLE_TITLE_ICON_TDDATA_DATAING
#else
            IMAGE_IDLE_TITLE_ICON_TDDATA_DATAING_SIM1,
            IMAGE_IDLE_TITLE_ICON_TDDATA_DATAING_SIM2,

#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_TITLE_ICON_TDDATA_DATAING_SIM3,
#endif

#if defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_TITLE_ICON_TDDATA_DATAING_SIM4,

#endif
#endif                     
        },
        //HSDPA_DATA_SERVICE_TYPE
        {
#ifdef MMI_MULTI_SIM_SYS_SINGLE
            IMAGE_IDLE_TITLE_ICON_HSDPA_DATAING
#else
            IMAGE_IDLE_TITLE_ICON_HSDPA_DATAING_SIM1,
            IMAGE_IDLE_TITLE_ICON_HSDPA_DATAING_SIM2,

#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_TITLE_ICON_HSDPA_DATAING_SIM3,
#endif

#if defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_TITLE_ICON_HSDPA_DATAING_SIM4,

#endif
#endif
        }
    };

    return dating_icon[data_service_type][dual_sys];
}
#endif //PRODUCT_DM


#ifdef WIN32
PUBLIC DBAT_STATE_INFO_T* DBAT_GetState(void)
{
    dbat_info1.vol = 3700; 
    dbat_info1.remain_cap = 40;
    dbat_info2.vol  = 3700; 
    dbat_info2.remain_cap = 60;
    
    s_dbat_state_info.module_state = 1;
    s_dbat_state_info.cur_bat  =  DBAT_MAIN_BAT;
    s_dbat_state_info.chgmng_info.bat_statistic_vol = 3700;
    s_dbat_state_info.chgmng_info.bat_cur_vol  =  3700;
    s_dbat_state_info.chgmng_info.bat_remain_cap = 70;
    
    s_dbat_state_info.p_bat[DBAT_MAIN_BAT] = &dbat_info1;
    s_dbat_state_info.p_bat[DBAT_AUX_BAT] = &dbat_info2;
    return &s_dbat_state_info;
}
#endif

#ifdef MMI_DUAL_BATTERY_SUPPORT
/*****************************************************************************/
//  Description : set the charge state
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleChargeState(
                                    BOOLEAN is_charge
                                    )
{
    GUIWIN_STBDATA_ICON_STYLE_E icon_type = GUIWIN_STBDATA_ICON_NONE;
    DBAT_STATE_INFO_T* dbat_info=DBAT_GetState();

    DBAT_BAT_E cur_charge_bat=dbat_info->cur_bat;
    DBAT_BAT_E cur_charge_bat_other=0;
    uint8*      batlevel=&MMIIDLE_GetIdleWinInfo()->batlevel;
    MMI_WIN_ICON_E bat_icon_type=MMI_WIN_ICON_BATTERY;
    uint8*      batlevel_other=&MMIIDLE_GetIdleWinInfo()->batlevel;
    MMI_WIN_ICON_E bat_icon_type_other=MMI_WIN_ICON_BATTERY;

    if(DBAT_MAIN_BAT==cur_charge_bat)
    {
        batlevel=&MMIIDLE_GetIdleWinInfo()->batlevel;
        bat_icon_type=MMI_WIN_ICON_BATTERY;
        batlevel_other=&MMIIDLE_GetIdleWinInfo()->batlevel_aux;
        bat_icon_type_other=MMI_WIN_ICON_BATTERY_AUX;
        cur_charge_bat_other=DBAT_AUX_BAT;
    }
    else
    {
        batlevel=&MMIIDLE_GetIdleWinInfo()->batlevel_aux;
        bat_icon_type=MMI_WIN_ICON_BATTERY_AUX;
        batlevel_other=&MMIIDLE_GetIdleWinInfo()->batlevel;
        bat_icon_type_other=MMI_WIN_ICON_BATTERY;
        cur_charge_bat_other=DBAT_MAIN_BAT;
    }
   
    //SCI_TRACE_LOW:"MAIN_SetIdleChargeState batlevel=%d, bat_icon_type=%d, cur_charge_bat=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_2832_112_2_18_2_24_16_87,(uint8*)"ddd", batlevel, bat_icon_type, cur_charge_bat);

    // 这个函数需要处理多种情况：Idle是否处于Focus, Charge的变化情况
    // 根据当前Idle窗口是否处于Focus来显示相关的东东！
    //SCI_TRACE_LOW:"MAIN_SetIdleChargeState is_charge = %d,s_idlewin_info.is_charge = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_2836_112_2_18_2_24_16_88,(uint8*)"dd",is_charge,MMIIDLE_GetIdleWinInfo()->is_charge);
    //if (is_charge != MMIIDLE_GetIdleWinInfo()->is_charge)//NEWMS00195807
    {
        MMIIDLE_GetIdleWinInfo()->is_charge = is_charge;
        
        *batlevel = MMIAPIPHONE_GetBatLevel(dbat_info->chgmng_info.bat_remain_cap);//当前使用电池
        if (is_charge)
        {
            icon_type = GUIWIN_STBDATA_ICON_ANIM;
        }
        else
        {
            icon_type = GUIWIN_STBDATA_ICON_NORMAL;
        }
        //SCI_TRACE_LOW:"MAIN_SetIdleChargeState %d, %d, %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_2850_112_2_18_2_24_17_89,(uint8*)"ddd", *batlevel, MMIIDLE_GetIdleWinInfo()->is_charge, icon_type);

        
        GUIWIN_SetStbItemIconStyle(bat_icon_type, icon_type);
        GUIWIN_SetStbItemIconCurFrame(bat_icon_type, *batlevel);
        
        *batlevel_other = MMIAPIPHONE_GetBatLevel(dbat_info->p_bat[cur_charge_bat_other]->remain_cap);//空闲电池
        GUIWIN_SetStbItemIconStyle(bat_icon_type_other, GUIWIN_STBDATA_ICON_NORMAL);
        GUIWIN_SetStbItemIconCurFrame(bat_icon_type_other, *batlevel_other);
        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE        
        MMISUB_SetBatteryLevel();  
        //刷新小屏的显示，充电
        MMISUB_UpdateDisplaySubLcd();
#endif
    }
}
#else
/*****************************************************************************/
//  Description : set the charge state
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleChargeState(
                                    BOOLEAN is_charge
                                    )
{
    GUIWIN_STBDATA_ICON_STYLE_E icon_type = GUIWIN_STBDATA_ICON_NONE;

    // 这个函数需要处理多种情况：Idle是否处于Focus, Charge的变化情况
    // 根据当前Idle窗口是否处于Focus来显示相关的东东！
    //if (is_charge != MMIIDLE_GetIdleWinInfo()->is_charge)//NEWMS00195807
    {
        MMIIDLE_GetIdleWinInfo()->is_charge = is_charge;
        
        MMIIDLE_GetIdleWinInfo()->batlevel = MMIAPIPHONE_GetBatCapacity();
#if (!defined SCREEN_SHAPE_CIRCULAR) || (defined ADULT_WATCH_SUPPORT)
        if (is_charge && CHGMNG_GetModuleState()->bat_remain_cap != 100)
        {
            icon_type = GUIWIN_STBDATA_ICON_ANIM;
        }
        else
        {
            icon_type = GUIWIN_STBDATA_ICON_NORMAL;
        }
#endif
        //SCI_TRACE_LOW:"MAIN_SetIdleChargeState %d, %d, %d, %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_2892_112_2_18_2_24_17_90,(uint8*)"dddd", MMIIDLE_GetIdleWinInfo()->batlevel, MMIIDLE_GetIdleWinInfo()->is_charge, icon_type, MMIIDLE_GetIdleWinInfo()->cur_batlevel);
#if (!defined ADULT_WATCH_SUPPORT) && (defined SCREEN_SHAPE_CIRCULAR)
        if (is_charge && CHGMNG_GetModuleState()->bat_remain_cap != 100)
        {
            GUIWIN_SetStbItemIconCurFrame(MMI_WIN_ICON_BATTERY, 11);
        }
        else
        {
            GUIWIN_SetStbItemIconCurFrame(MMI_WIN_ICON_BATTERY, MMIIDLE_GetIdleWinInfo()->batlevel);
        }

#else
        GUIWIN_SetStbItemIconStyle(MMI_WIN_ICON_BATTERY, icon_type);
        GUIWIN_SetStbItemIconCurFrame(MMI_WIN_ICON_BATTERY, MMIIDLE_GetIdleWinInfo()->batlevel);
#endif
        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE         
        MMISUB_SetBatteryLevel();         
        //刷新小屏的显示，充电
        MMISUB_UpdateDisplaySubLcd();
#endif
    }
}
#endif


#ifdef MMI_DUAL_BATTERY_SUPPORT
/*****************************************************************************/
//  Description : set the battery level of the idle window
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleDbatLevel(DBAT_STATE_INFO_T* dbat_info)
{
    uint8 bat_level=0;
    BOOLEAN is_update=FALSE;
    //SCI_TRACE_LOW:"MAIN_SetIdleDbatLevel  dbat_info->p_bat[DBAT_MAIN_BAT]=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_2915_112_2_18_2_24_17_91,(uint8*)"d", dbat_info->p_bat[DBAT_MAIN_BAT]);
    if(PNULL!=dbat_info->p_bat[DBAT_MAIN_BAT])
    {
        if(DBAT_MAIN_BAT == dbat_info->cur_bat)
        {
            bat_level = MMIAPIPHONE_GetBatLevel(dbat_info->chgmng_info.bat_remain_cap);
        }
        else
        {
            bat_level = MMIAPIPHONE_GetBatLevel(dbat_info->p_bat[DBAT_MAIN_BAT]->remain_cap);
        }
        //SCI_TRACE_LOW:"MAIN_SetIdleDbatLevel  bat_level=%d,s_idlewin_info.batlevel=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_2926_112_2_18_2_24_17_92,(uint8*)"dd", bat_level,MMIIDLE_GetIdleWinInfo()->batlevel);
        if((bat_level != MMIIDLE_GetIdleWinInfo()->batlevel)
            || (!GUIWIN_IsStbItemVisible(MMI_WIN_ICON_BATTERY)))
        {
            MMIIDLE_GetIdleWinInfo()->batlevel = bat_level;
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_BATTERY,TRUE);
            GUIWIN_SetStbItemIconCurFrame(MMI_WIN_ICON_BATTERY, MMIIDLE_GetIdleWinInfo()->batlevel);
            is_update=TRUE;
        }
    }
    else
    {
       if(GUIWIN_IsStbItemVisible(MMI_WIN_ICON_BATTERY))
       {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_BATTERY,FALSE);
            is_update=TRUE;
       }
    }
    
    //SCI_TRACE_LOW:"MAIN_SetIdleDbatLevel  dbat_info->p_bat[DBAT_AUX_BAT]=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_2945_112_2_18_2_24_17_93,(uint8*)"d", dbat_info->p_bat[DBAT_AUX_BAT]);
    if(PNULL!=dbat_info->p_bat[DBAT_AUX_BAT])
    {
        if(DBAT_AUX_BAT == dbat_info->cur_bat)
        {
            bat_level=MMIAPIPHONE_GetBatLevel(dbat_info->chgmng_info.bat_remain_cap);
        }
        else
        {
            bat_level=MMIAPIPHONE_GetBatLevel(dbat_info->p_bat[DBAT_AUX_BAT]->remain_cap);
        }
        
        //SCI_TRACE_LOW:"MAIN_SetIdleDbatLevel  bat_level=%d,MMIIDLE_GetIdleWinInfo()->batlevel_aux=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_2957_112_2_18_2_24_17_94,(uint8*)"dd", bat_level,MMIIDLE_GetIdleWinInfo()->batlevel_aux);
        if((bat_level != MMIIDLE_GetIdleWinInfo()->batlevel_aux) 
            || (!GUIWIN_IsStbItemVisible(MMI_WIN_ICON_BATTERY_AUX)))
        {
            MMIIDLE_GetIdleWinInfo()->batlevel_aux = bat_level;
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_BATTERY_AUX,TRUE);
            GUIWIN_SetStbItemIconCurFrame(MMI_WIN_ICON_BATTERY_AUX, MMIIDLE_GetIdleWinInfo()->batlevel_aux);
            is_update=TRUE;
        }
    }
    else
    {
       if(GUIWIN_IsStbItemVisible(MMI_WIN_ICON_BATTERY_AUX))
       {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_BATTERY_AUX,FALSE);
            is_update=TRUE;
       }
    }
    //SCI_TRACE_LOW:"MAIN_SetIdleDbatLevel  is_update=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_2975_112_2_18_2_24_17_95,(uint8*)"d", is_update);
    if(is_update)
    {
        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE        
        //刷新小屏的显示，刷新电量
        MMISUB_UpdateDisplaySubLcd(); 
#endif
    }
}
/*****************************************************************************/
// Description : 
// Global resource dependence : 
// Author: renyi.hu
// Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_SetStbBatteryInfo(BOOLEAN is_main_battery)
{
    uint32  icon_index                                   =  0;
    GUIWIN_STBDATA_ITEM_DATA_T  item_data   =   {0};
    DBAT_BAT_E bat_type=DBAT_MAIN_BAT;
    MMI_WIN_ICON_E icon_type=MMI_WIN_ICON_BATTERY;
    uint8 *batlevel=&(MMIIDLE_GetIdleWinInfo()->batlevel);
    DBAT_BAT_E cur_use_bat=DBAT_MAIN_BAT;
    uint32  charge_icon_id[MMISTATUSBAR_ITEM_ICON_FRAME] =   {IMAGE_IDLE_TITLE_ICON_BATTERY1,
                                                            IMAGE_IDLE_TITLE_ICON_BATTERY2,
                                                            IMAGE_IDLE_TITLE_ICON_BATTERY3,
                                                            IMAGE_IDLE_TITLE_ICON_BATTERY4,
                                                            IMAGE_IDLE_TITLE_ICON_BATTERY5,
                                                            IMAGE_IDLE_TITLE_ICON_BATTERY6};
    
    DBAT_STATE_INFO_T* dbat_info=DBAT_GetState();
    uint8 bat_level=0;
    
#ifdef MMI_POWER_SAVING_MODE_SUPPORT    
    MMISET_POWER_SETTING_ITEM_T  power_sav = MMIAPISET_GetPowerSavingSetting(); 
    if(power_sav.is_active)
    {
        charge_icon_id.icon_array[0] = IMAGE_IDLE_TITLE_ICON_PS_BATTERY1;
        charge_icon_id.icon_array[1] = IMAGE_IDLE_TITLE_ICON_PS_BATTERY2;
        charge_icon_id.icon_array[2] = IMAGE_IDLE_TITLE_ICON_PS_BATTERY3;
        charge_icon_id.icon_array[3] = IMAGE_IDLE_TITLE_ICON_PS_BATTERY4;
        charge_icon_id.icon_array[4] = IMAGE_IDLE_TITLE_ICON_PS_BATTERY5;
        charge_icon_id.icon_array[5] = IMAGE_IDLE_TITLE_ICON_PS_BATTERY6;
    }
#endif

    cur_use_bat=dbat_info->cur_bat;
    //SCI_TRACE_LOW:"MMIAPIIDLE_SetStbBatteryInfo is_main_battery=%d,cur_use_bat=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_3007_112_2_18_2_24_17_96,(uint8*)"dd",is_main_battery,cur_use_bat);
    if(!is_main_battery)
    {
        bat_type=DBAT_AUX_BAT;
        icon_type=MMI_WIN_ICON_BATTERY_AUX;
        batlevel=&(MMIIDLE_GetIdleWinInfo()->batlevel_aux); 
    }

    if(PNULL!=dbat_info->p_bat[bat_type])
    {
        if(bat_type == cur_use_bat)
        {
            bat_level=MMIAPIPHONE_GetBatLevel(dbat_info->chgmng_info.bat_remain_cap);
        }
        else
        {
            bat_level=MMIAPIPHONE_GetBatLevel(dbat_info->p_bat[bat_type]->remain_cap);
        }
        //SCI_TRACE_LOW:"MMIAPIIDLE_SetStbBatteryInfo bat_level=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_3025_112_2_18_2_24_17_97,(uint8*)"d",bat_level);
       
    }
    
    *batlevel = bat_level; // 获取当前的电量

    for (icon_index = 0; icon_index < MMISTATUSBAR_ITEM_ICON_FRAME; icon_index++)
    {
        item_data.icon_info.icon_array[icon_index] = charge_icon_id[icon_index];
    }

    item_data.app_handle_func = OpenBtryCapWinFunc;
    item_data.icon_info.icon_num = MMISTATUSBAR_ITEM_ICON_FRAME;
    item_data.icon_info.is_display = TRUE;
    item_data.cur_frame = *batlevel;    
    
    if(MMIIDLE_GetIdleWinInfo()->is_charge && bat_type==cur_use_bat)
    {
        item_data.icon_info.icon_type = GUIWIN_STBDATA_ICON_ANIM;
    }
    else
    {
        item_data.icon_info.icon_type = GUIWIN_STBDATA_ICON_NORMAL;
    }
    GUIWIN_AppendStbItemData(&item_data);
    GUIWIN_SetStbItemRightAlign(icon_type, TRUE);
    
    if(PNULL!=dbat_info->p_bat[bat_type])
    {
        GUIWIN_SetStbItemVisible(icon_type, TRUE);        
    }
    else
    {
        GUIWIN_SetStbItemVisible(icon_type, FALSE);
    } 
}


/*****************************************************************************/
// Description : 
// Global resource dependence : 
// Author: renyi.hu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIIDLE_IsChangeChargeState(void)
{
    if(TRUE == MMIIDLE_GetIdleWinInfo()->is_charge )
    {
        MMIIDLE_GetIdleWinInfo()->is_charge = FALSE;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif


/*****************************************************************************/
//  Description : handle rxlevel bound timer
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StartRxLevelTimer(MN_DUAL_SYS_E dual_sys, BOOLEAN is_need_bound)
{
    BOOLEAN     is_need_start_timer     = FALSE;

    s_signal_bound_timer[dual_sys] = 0;

    if (MMIIDLE_GetIdleWinInfo()->rxlevel_step[dual_sys] < MMIIDLE_GetIdleWinInfo()->rxlevel[dual_sys])
    {
        is_need_start_timer = TRUE;

        if (is_need_bound)
        {
            MMIIDLE_GetIdleWinInfo()->rxlevel_step[dual_sys]++;
        }
    }
    else if (MMIIDLE_GetIdleWinInfo()->rxlevel_step[dual_sys] > MMIIDLE_GetIdleWinInfo()->rxlevel[dual_sys])
    {
        is_need_start_timer = TRUE;

        if (is_need_bound)
        {
            MMIIDLE_GetIdleWinInfo()->rxlevel_step[dual_sys]--;
        }
    }

    if (is_need_start_timer)
    {
        uint32 time_out = MMI_SIGNAL_IDLE_MODE_TIMEOUT;

        if (MMIAPICC_IsInState(CC_IN_CALL_STATE)
#ifdef BROWSER_SUPPORT
            || MMIAPIBROWSER_IsRunning()
#endif
            )
        {
            time_out = MMI_SIGNAL_DEDICATED_MODE_TIMEOUT;
        }
        else
        {
            time_out = MMI_SIGNAL_IDLE_MODE_TIMEOUT;
        }

        s_signal_bound_timer[dual_sys] = MMK_CreateTimerCallback(time_out, MAIN_HandleRxLevelTimer , NULL, FALSE);
    }

    return is_need_start_timer;
}

/*****************************************************************************/
//  Description : handle rxlevel bound timer
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_HandleRxLevelTimer(uint8 timer_id, uint32 param)
{
    BOOLEAN is_refresh = FALSE;
    uint32 i = 0;

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (timer_id == s_signal_bound_timer[i])
        {
            is_refresh = StartRxLevelTimer(i, TRUE);

            if (is_refresh)
            {
                MMIIDLE_SetSimStatusBarInfo((MN_DUAL_SYS_E)i, MMI_WIN_ICON_SIGNAL + i, FALSE);
            }
        }

    }

    if (is_refresh)
    {
        if (MMIDEFAULT_IsBacklightOn())
        {
            //update 
            GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE            
            //刷新小屏的显示，刷新信号
            MMISUB_UpdateDisplaySubLcd();
#endif
        }
    }    
}

/*****************************************************************************/
//  Description : get the rx level
//  
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIMAIN_GetRxLevel(MN_DUAL_SYS_E e_dual_sys)
{
    return MMIIDLE_GetIdleWinInfo()->rxlevel[e_dual_sys];
}

/*****************************************************************************/
//  Description : set the rx level of the idle window
//    is_direct:TRUE直接显示信号跳变；FALSE平滑显示信号跳变
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleRxLevel(MN_DUAL_SYS_E e_dual_sys, uint8 rx_level, BOOLEAN is_direct)
{
    if (MMI_DUAL_SYS_MAX <= e_dual_sys)
    {
        //MMI_ASSERT_WARNING("e_dual_sys < MMI_DUAL_SYS_MAX");
        //SCI_TRACE_LOW:"MAIN_SetIdleRxLevel ERROR! MMI_DUAL_SYS_MAX <= e_dual_sys,e_dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_3189_112_2_18_2_24_17_98,(uint8*)"d",e_dual_sys);
        return;
    }
    if (rx_level != MMIIDLE_GetIdleWinInfo()->rxlevel_step[e_dual_sys])
    {
        MMIIDLE_GetIdleWinInfo()->rxlevel[e_dual_sys] = rx_level;

        //SCI_TRACE_LOW:"MAIN_SetIdleRxLevel %d,%d,%d, %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_3196_112_2_18_2_24_17_99,(uint8*)"dddd", rx_level, MMIIDLE_GetIdleWinInfo()->rxlevel[e_dual_sys], MMIIDLE_GetIdleWinInfo()->rxlevel_step[e_dual_sys], is_direct);

//lzk mark 
#ifdef BT_DIALER_SUPPORT
        if (0 != s_signal_bound_timer[e_dual_sys])//stop timer
        {
            SCI_TRACE_LOW("MAIN_SetIdleRxLevel s_signal_bound_timer != 0");
            return;
//             MMK_StopTimer(s_signal_bound_timer[e_dual_sys]);
//             s_signal_bound_timer[e_dual_sys] = 0;
        }

        if (is_direct)
        {
            MMIIDLE_GetIdleWinInfo()->rxlevel_step[e_dual_sys] = MMIIDLE_GetIdleWinInfo()->rxlevel[e_dual_sys];
#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
            MMIAPIBT_Service_Indicator(FALSE);
#endif
        }
        else
        {
            StartRxLevelTimer(e_dual_sys, FALSE);
#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
            MMIAPIBT_Service_Indicator(TRUE);
#endif
        }
	//mark #ifdef BT_DIALER_SUPPORT
#else
        if (is_direct)
        {
            if (0 != s_signal_bound_timer[e_dual_sys])//stop timer
            {
                MMK_StopTimer(s_signal_bound_timer[e_dual_sys]);
                s_signal_bound_timer[e_dual_sys] = 0;
            }            
            MMIIDLE_GetIdleWinInfo()->rxlevel_step[e_dual_sys] = MMIIDLE_GetIdleWinInfo()->rxlevel[e_dual_sys];
        }
        else
        {
            if (0 == s_signal_bound_timer[e_dual_sys])
            {
                StartRxLevelTimer(e_dual_sys, FALSE);
            } 
            else
            {
                MMIAPIDM_HandDmRxChange(rx_level);
				return;
            }
        }
#endif

        MMIIDLE_SetSimStatusBarInfo(e_dual_sys, (uint32)MMI_WIN_ICON_SIGNAL + ((uint32)e_dual_sys - MN_DUAL_SYS_1), FALSE);


        if (MMIDEFAULT_IsBacklightOn())
        {
            //update
            GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE            
            //刷新小屏的显示，刷新信号
            MMISUB_UpdateDisplaySubLcd(); 
#endif
        }
    }
    else
    {
        if (0 != s_signal_bound_timer[e_dual_sys])
        {
            MMK_StopTimer(s_signal_bound_timer[e_dual_sys]);
            s_signal_bound_timer[e_dual_sys] = 0;
        }
    }
    MMIAPIDM_HandDmRxChange(rx_level);
}


#ifdef WIN32
/*****************************************************************************/
//  Description : dummy function of REF
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CHGMNG_IsChargeConnect(void)
{
    return FALSE;
}
/*****************************************************************************/
//  Description : dummy function of REF
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_CheckHeadsetStatus(void)
{
    return MMI_GetEarphoneFlag();
}
#endif

//cr00116148 renwei modify
#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT

/*****************************************************************************/
//  Description : 添加滚动信息
//  Global resource dependence : none
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMAIN_AddScrollItemNewMMS(
                                        MMISMS_READ_MSG_T        *read_msg_ptr
                                        )
{
}

/*****************************************************************************/
//  Description : 添加滚动信息
//  Global resource dependence : none
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMAIN_SetScrollItemNewSMS(void)
{
    GUISTBSCROLLMSG_SCROLL_ITEM_T scroll_item = {0};
    MMISMS_READMSG_CONTENT_T read_content = {0};
    MMI_STRING_T msg_arr[3] = {0};
    wchar colon_str[]=L": \n";
    uint32 total_len = 0;
    
    scroll_item.is_valid = TRUE;
    scroll_item.type = GUISTBSCROLLMSG_SCROLL_ITEM_NEW_MESSAGE;    
        
    //icon
    scroll_item.icon_id = IMAGE_IDLE_TITLE_ICON_MESSAGE;

    read_content.name_string.wstr_ptr = SCI_ALLOCA((MMISMS_PBNAME_MAX_LEN + 1)*sizeof(wchar));
    SCI_MEMSET(read_content.name_string.wstr_ptr ,0x00,(MMISMS_PBNAME_MAX_LEN + 1)*sizeof(wchar));
    read_content.name_string.wstr_len = MMISMS_PBNAME_MAX_LEN;
    read_content.time_string.wstr_ptr = SCI_ALLOCA((30 + 1)*sizeof(wchar));
    read_content.time_string.wstr_len = 30;
    SCI_MEMSET(read_content.time_string.wstr_ptr  ,0x00,(30 + 1)*sizeof(wchar));
    read_content.content_string.wstr_ptr = SCI_ALLOCAZ( ( MMISMS_R8_MESSAGE_LEN + 1 )*sizeof(wchar) );
    read_content.content_string.wstr_len = MMISMS_R8_MESSAGE_LEN;
    MMIAPISMS_GetReadMsgContent(&read_content);

    msg_arr[0] = read_content.name_string;
    msg_arr[1].wstr_ptr = colon_str;
    msg_arr[1].wstr_len = MMIAPICOM_Wstrlen(colon_str);
    msg_arr[2] = read_content.content_string;

    total_len = msg_arr[0].wstr_len+msg_arr[1].wstr_len+msg_arr[2].wstr_len;
    scroll_item.string_info.wstr_ptr = SCI_ALLOCAZ((total_len + 1)*sizeof(wchar));
    scroll_item.string_info.wstr_len = total_len;
    MMIAPICOM_ConnectSomeString(msg_arr, 3, &scroll_item.string_info);
    GUISTBSCROLLMSG_AppendScrollItem(&scroll_item);

    if (PNULL != read_content.name_string.wstr_ptr)
    {
        SCI_FREE(read_content.name_string.wstr_ptr);
    }
    if (PNULL != read_content.time_string.wstr_ptr)
    {
        SCI_FREE(read_content.time_string.wstr_ptr);
    }
    if (PNULL != read_content.content_string.wstr_ptr)
    {
        SCI_FREE(read_content.content_string.wstr_ptr);
    }
    if (PNULL != scroll_item.string_info.wstr_ptr)
    {
        SCI_FREE(scroll_item.string_info.wstr_ptr);
    }   
}

/*****************************************************************************/
//  Description : 添加滚动信息
//  Global resource dependence : none
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void AddNewMessageStbScrollItem(void)
{
    MMIMAIN_SetScrollItemNewSMS();
}

/*****************************************************************************/
//  Description : 添加滚动信息
//  Global resource dependence : none
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void AddAlarmStbScrollItem(void)
{
    MMI_STRING_T kstring = {0};
    GUISTBSCROLLMSG_SCROLL_ITEM_T scroll_item = {0};
    
    scroll_item.is_valid = TRUE;
    scroll_item.type = GUISTBSCROLLMSG_SCROLL_ITEM_ALARM;    
    //icon
    scroll_item.icon_id = IMAGE_IDLE_TITLE_ICON_CLOCK;

    //text
    MMI_GetLabelTextByLang(TXT_ALARM_CLOCK, &kstring);
    scroll_item.string_info.wstr_len = kstring.wstr_len;
    scroll_item.string_info.wstr_ptr = SCI_ALLOC_APP(kstring.wstr_len*sizeof(wchar));
    MMI_WSTRNCPY(scroll_item.string_info.wstr_ptr, kstring.wstr_len, kstring.wstr_ptr, kstring.wstr_len, kstring.wstr_len);

    GUISTBSCROLLMSG_AppendScrollItem(&scroll_item);

    if (PNULL != scroll_item.string_info.wstr_ptr)
    {
        SCI_FREE(scroll_item.string_info.wstr_ptr);
    }
}
/*****************************************************************************/
//  Description : 添加滚动信息
//  Global resource dependence : none
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void AddScheduleStbScrollItem(void)
{
    MMI_STRING_T kstring = {0};
    GUISTBSCROLLMSG_SCROLL_ITEM_T scroll_item = {0};
    
    scroll_item.is_valid = TRUE;
    scroll_item.type = GUISTBSCROLLMSG_SCROLL_ITEM_SCHEDULE;    
    //icon
    scroll_item.icon_id = IMAGE_IDLE_TITLE_ICON_CLOCK;

    //text
    MMI_GetLabelTextByLang(TXT_IDLE_CALENDAR, &kstring);
    scroll_item.string_info.wstr_len = kstring.wstr_len;
    scroll_item.string_info.wstr_ptr = SCI_ALLOC_APP(kstring.wstr_len*sizeof(wchar));
    MMI_WSTRNCPY(scroll_item.string_info.wstr_ptr, kstring.wstr_len, kstring.wstr_ptr, kstring.wstr_len, kstring.wstr_len);

    GUISTBSCROLLMSG_AppendScrollItem(&scroll_item);

    if (PNULL != scroll_item.string_info.wstr_ptr)
    {
        SCI_FREE(scroll_item.string_info.wstr_ptr);
    }
}
/*****************************************************************************/
//  Description : 添加滚动信息
//  Global resource dependence : none
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void AddMissCallStbScrollItem(void)
{
    //MN_RETURN_RESULT_E    return_value = MN_RETURN_FAILURE;
    BOOLEAN result = FALSE;
    //MMICL_CALL_ARRAY_BCDINFO_T* arraycall_info = PNULL;
    MMICL_CALL_BCDINFO_T *call_bcdinfo_ptr = PNULL;
    uint8   tele_num_len = 0;   
    uint8       tele_num[PHONE_NUM_MAX_LEN + 2] = {0};
    BOOLEAN     find_result =FALSE;
    GUISTBSCROLLMSG_SCROLL_ITEM_T scroll_item = {0};

    //arraycall_info = SCI_ALLOCA(sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    call_bcdinfo_ptr = SCI_ALLOCA(sizeof(MMICL_CALL_BCDINFO_T));

    //if(PNULL == arraycall_info)
    if (PNULL == call_bcdinfo_ptr)
    {
        //SCI_TRACE_LOW:"AddMissCallStbScrollItem call info alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_3389_112_2_18_2_24_18_100,(uint8*)"");
        return;
    }

    //SCI_MEMSET(arraycall_info, 0, sizeof(MMICL_CALL_ARRAY_BCDINFO_T));
    SCI_MEMSET(call_bcdinfo_ptr, 0, sizeof(MMICL_CALL_BCDINFO_T));

    scroll_item.is_valid = TRUE;
    scroll_item.type = GUISTBSCROLLMSG_SCROLL_ITEM_MISSED_CALL;    

    scroll_item.icon_id = IMAGE_IDLE_TITLE_ICON_MISSED_CALL;
//    if(1 < missed_call_num)
//    {
//        //多条是否要处理？
//    }
//    return_value = MMICL_ReadNV(MMICL_CALL_MISSED, arraycall_info);
    
    result = MMICL_ReadLastMissedCall(call_bcdinfo_ptr);

    //组装：En(Missed call from 021-508-02727), Ch(来自02150802727的未接电话)
//    if(MN_RETURN_SUCCESS == return_value)
    if (result)
    {
        tele_num_len = MMIAPICOM_GenDispNumber(call_bcdinfo_ptr->number_type,
            call_bcdinfo_ptr->number_len,
            call_bcdinfo_ptr->number, 
            tele_num,
            PHONE_NUM_MAX_LEN + 2);

        scroll_item.string_info.wstr_ptr = (wchar*)SCI_ALLOC_APP((PHONE_NUM_MAX_LEN+1)*sizeof(wchar));
        scroll_item.string_info.wstr_len = PHONE_NUM_MAX_LEN;
        SCI_MEMSET(scroll_item.string_info.wstr_ptr, 0x00, (PHONE_NUM_MAX_LEN+1)*sizeof(wchar));

        find_result = MMIAPISMS_GetNameByNumberFromPB(tele_num, &scroll_item.string_info, PHONE_NUM_MAX_LEN);
        if (!find_result) //没有在电话本中找到相应纪录则显示号码
        {
            scroll_item.string_info.wstr_len = PHONE_NUM_MAX_LEN;
            
            MMI_STRNTOWSTR(scroll_item.string_info.wstr_ptr,
                scroll_item.string_info.wstr_len,
                (const uint8*)tele_num,
                tele_num_len,
                tele_num_len);
            
            scroll_item.string_info.wstr_len =  (uint16)strlen((char*)tele_num);;
        }
    }

    SCI_FREE(call_bcdinfo_ptr);
    call_bcdinfo_ptr = PNULL;

    GUISTBSCROLLMSG_AppendScrollItem(&scroll_item);

    if (PNULL != scroll_item.string_info.wstr_ptr)
    {
        SCI_FREE(scroll_item.string_info.wstr_ptr);
    }
}

/*****************************************************************************/
//  Description : 添加滚动信息
//  Global resource dependence : none
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void AddNewMMSStbScrollItem(void)
{
    MMI_STRING_T kstring = {0};
    GUISTBSCROLLMSG_SCROLL_ITEM_T scroll_item = {0};
    
    scroll_item.is_valid = TRUE;
    scroll_item.type = GUISTBSCROLLMSG_SCROLL_ITEM_NEW_MMS;    
    //icon
    scroll_item.icon_id = IMAGE_IDLE_TITLE_ICON_MMS_DUA;//新彩信

    //text
    MMI_GetLabelTextByLang(TXT_MMS, &kstring);
    scroll_item.string_info.wstr_len = kstring.wstr_len;
    scroll_item.string_info.wstr_ptr = SCI_ALLOC_APP(kstring.wstr_len*sizeof(wchar));
    MMI_WSTRNCPY(scroll_item.string_info.wstr_ptr, kstring.wstr_len, kstring.wstr_ptr, kstring.wstr_len, kstring.wstr_len);

    GUISTBSCROLLMSG_AppendScrollItem(&scroll_item);

    if (PNULL != scroll_item.string_info.wstr_ptr)
    {
        SCI_FREE(scroll_item.string_info.wstr_ptr);
    }
}

/*****************************************************************************/
//  Description : 用于新短信，未接来电等事件的回调函数
//  Global resource dependence : none
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
LOCAL void NewEventCallback(MMIEVENT_INFO_T *event_info_ptr)
{
    if (PNULL == event_info_ptr)
	{
		return;
	}
    switch (event_info_ptr->event_type)
    {
    case SMS_NEWSMS_EVENT:
		{			
//			uint16 to_be_read_num = *(uint16*)(event_info_ptr->param_ptr);
#ifdef MMI_SMS_CHAT_SUPPORT  
            MMISMS_NEW_SMS_INFO_T *new_sms_info_ptr = PNULL;
            new_sms_info_ptr = (MMISMS_NEW_SMS_INFO_T *)event_info_ptr->param_ptr;
            if((new_sms_info_ptr != PNULL && new_sms_info_ptr->cur_chat_order_id != PNULL) && ((new_sms_info_ptr->cur_chat_order_id->flag.is_concatenate_sms && new_sms_info_ptr->cur_chat_order_id->flag.is_receive_all)
                || (!new_sms_info_ptr->cur_chat_order_id->flag.is_concatenate_sms))
            )
#endif      
            {          
			    AddNewMessageStbScrollItem();
            }
		}
		break;
		
	default:
		break;
	}	
}

/*****************************************************************************/
//  Description : 添加滚动信息
//  Global resource dependence : none
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIN_AppendStbScrollItem(
                                           GUISTBSCROLLMSG_SCROLL_ITEM_E item_type
                                           )
{
    BOOLEAN result = TRUE;

    //SCI_TRACE_LOW:"MMIMAIN_AppendStbScrollItem() item_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_3486_112_2_18_2_24_18_101,(uint8*)"d", item_type);

    switch (item_type)
    {
    case GUISTBSCROLLMSG_SCROLL_ITEM_NEW_MESSAGE:
        AddNewMessageStbScrollItem();
        break;

#if defined(MMI_SMSCB_SUPPORT) && defined(MMI_STATUSBAR_SCROLLMSG_SUPPORT)
    case GUISTBSCROLLMSG_SCROLL_ITEM_NEW_SMSCB:
        MMIAPISMSCB_SetScrollItemNewSMSCB();
        break;
#endif

    case GUISTBSCROLLMSG_SCROLL_ITEM_MISSED_CALL:
        AddMissCallStbScrollItem();
        break;

    case GUISTBSCROLLMSG_SCROLL_ITEM_ALARM:
        AddAlarmStbScrollItem();
        break;
    case GUISTBSCROLLMSG_SCROLL_ITEM_SCHEDULE:
        AddScheduleStbScrollItem();
        break;

    case GUISTBSCROLLMSG_SCROLL_ITEM_NEW_MMS:
        AddNewMMSStbScrollItem();
        break;

    default:
        result = FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : 开机时添加滚动信息
//  Global resource dependence : none
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMAIN_PowerOnAddStbScrollItem(void)
{
    //未接来电
    //if (MMIAPICC_GetMissedCallNumber() > 0)
    //{
    //    MMIMAIN_AppendStbScrollItem(GUISTBSCROLLMSG_SCROLL_ITEM_MISSED_CALL);
    //}
    //新短信@@@读取信息可能会发生冲突，此处不建议做
    //if (MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_SMS) > 0)
    //{
        //MMIMAIN_AppendStbScrollItem(GUISTBSCROLLMSG_SCROLL_ITEM_NEW_MESSAGE);
    //}
    //新彩信
    //if (MMIAPISMS_GetTobeReadMsgCount(MN_DUAL_SYS_MAX, MMISMS_TYPE_MMS) > 0)
    //{
    //    MMIMAIN_AppendStbScrollItem(GUISTBSCROLLMSG_SCROLL_ITEM_NEW_MMS);
    //}
}

#endif

#ifdef MMI_VOLTE_SUPPORT
BOOLEAN is_volte_icon_display =FALSE;

LOCAL void StatusAreaSetVolteInfo(MN_DUAL_SYS_E dual_sys,uint32 item_index, BOOLEAN is_append)
{
    GUIWIN_STBDATA_ITEM_DATA_T item_data = {0};
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    MN_PHONE_PLMN_STATUS_E plmn_status = PLMN_NO_SERVICE;
    BOOLEAN is_emergency_service = FALSE;
    BOOLEAN is_ps_ready = FALSE;
    BOOLEAN is_fly_mode_on = FALSE;
    BOOLEAN is_volte_icon_on =FALSE;

    SCI_TRACE_LOW("StatusAreaSetVolteInfo dual_sys=%d, index=%d", dual_sys, item_index);
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

#ifdef MMI_MULTI_SIM_SYS_DUAL	
    if (dual_sys == MN_DUAL_SYS_2)
    {
		item_index = MMI_WIN_ICON_VOLTE_2;
    }
#endif
    is_fly_mode_on = MMIAPISET_GetFlyMode();

    if (is_fly_mode_on)
    {
        image_id = IMAGE_NULL;
    }
    else
    {
        //if (MMIAPIPHONE_volte_icon_display_set(dual_sys) == 1)
        //{
            //if (MMIAPIPHONE_GetImsStatus(dual_sys))
            //{
                if (MMIAPIPHONE_GetVolteIconDisplayStyle(dual_sys) == 1)
                {
                    image_id = IMAGE_PRIMO_STATUS_VOLTE;
                }
                else
                {
                    image_id = IMAGE_PRIMO_STATUS_HD;
                }
            //}		
            //else
            //{
            //    image_id = IMAGE_NULL;
           // }
        //}
        //else
        //{
        //    image_id = IMAGE_NULL;
        //}
    }
#ifdef UAL_TELE_SUPPORT
    plmn_status = ual_tele_radio_get_current_plmn_state(dual_sys);
#else
    plmn_status = MMIAPIPHONE_GetPlmnStatus(dual_sys);
    is_ps_ready = MMIAPIPHONE_GetIsPsReady(dual_sys);
#endif
    SCI_TRACE_LOW("StatusAreaSetVolteInfo plmn_status =%d,is_ps_ready =%d,sim_status = %d ",plmn_status,is_ps_ready,MMIAPIPHONE_GetSimStatus(dual_sys));
#ifndef UAL_TELE_SUPPORT
    if(PLMN_NO_SERVICE == plmn_status
            || PLMN_EMERGENCY_ONLY == plmn_status
            || PLMN_EMERGENCY_SIM_INVALID == plmn_status
            || PLMN_EMERGENCY_GPRS_ONLY == plmn_status
            || PLMN_EMERGENCY_SIM_INVALID_GPRS_ONLY == plmn_status
            || PLMN_REGISTER_SERVICE == plmn_status
            || PLMN_REGISTER_GPRS_ONLY == plmn_status
            || !is_ps_ready)
#else
    if(UAL_TELE_RADIO_PLMN_STATE_NORMAL != plmn_status)
#endif
    {
        is_emergency_service = TRUE;
        SCI_TRACE_LOW("StatusAreaSetVolteInfo is_emergency_service = %d",is_emergency_service);
    }

    is_volte_icon_on = MMIAPIPHONE_GetVolteStatus(dual_sys);//bug2138051
    if (SIM_STATUS_PIN_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys)
            || SIM_STATUS_PUK_BLOCKED == MMIAPIPHONE_GetSimStatus(dual_sys)
            || SIM_STATUS_NO_SIM == MMIAPIPHONE_GetSimStatus(dual_sys)
            || !is_volte_icon_on
            || is_emergency_service)
    {
            image_id = IMAGE_NULL;
    }

    item_data.icon_info.icon_array[0] = image_id;
    item_data.icon_info.icon_num = 1;
    item_data.icon_info.icon_type = GUIWIN_STBDATA_ICON_NORMAL;
    item_data.icon_info.is_display = TRUE;
    item_data.cur_frame = 0;
    item_data.app_handle_func = PNULL;
    item_data.is_handle_callback = FALSE;
    
    if (image_id != IMAGE_NULL)	
    {
		is_volte_icon_display = TRUE;
			SCI_TraceLow(" StatusAreaSetVolteInfo  image_id=%x is_volte_icon_display=%d",image_id,is_volte_icon_display);
	}
    else
    {
		is_volte_icon_display = FALSE;
			SCI_TraceLow(" StatusAreaSetVolteInfo  image_id= NULL is_volte_icon_display=%d",is_volte_icon_display);
	}
	
    if (is_append)
    {
        GUIWIN_AppendStbItemData(&item_data);
        //GUIWIN_SetStbItemVisible(item_index, FALSE);
    }
    else
    {
        GUIWIN_SetStbItemIconId(item_index, image_id);

        if (image_id == IMAGE_NULL)
        {
            GUIWIN_SetStbItemVisible(item_index, FALSE);
        }
        else
        {
            //if (!s_is_lockscreen)
            {
                GUIWIN_SetStbItemVisible(item_index, TRUE);
            }
            //else
            //{
            //    GUIWIN_SetStbItemVisible(item_index, FALSE);
            //}
        }
    }
    GUIWIN_SetStbItemRightAlign(item_index, FALSE);

    return;
}

/*****************************************************************************/
//  Description : set the VOLTE state of the idle window
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetIdleVolteState(MN_DUAL_SYS_E dual_sys)
{
    StatusAreaSetVolteInfo(dual_sys, MMI_WIN_ICON_VOLTE, FALSE);
    GUIWIN_UpdateStb();

    return;
}
#endif

#ifdef MMI_WIFI_SUPPORT
/*****************************************************************************/
//  Description : set the wifi icon of the idle window
//  Global resource dependence : 
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleWifiIcon(
                                       MMI_IMAGE_ID_T image_id
                                       )
{
    GUIWIN_SetStbItemIconId(MMI_WIN_ICON_WIFI, image_id);
    if (image_id != IMAGE_NULL)
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_WIFI, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_WIFI, FALSE);
    }
#if defined MMI_ISTYLE_SUPPORT
    if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_WIFI, TRUE);
    }
    else
#endif		
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_WIFI, TRUE);
    }

    // update
    GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE    
    //刷新小屏的显示
    MMISUB_UpdateDisplaySubLcd(); 
#endif
    MMIAPIPHONE_UpdateNetworkStatusWin();
}
/*****************************************************************************/
//  Description : get the wifi icon of the idle window
//  Global resource dependence : 
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MAIN_GetIdleWifiIcon(void)
{
    GUISTBDATA_ICON_T wifi_icon_info = {0};
    
    GUIWIN_GetStbItemIcon(MMI_WIN_ICON_WIFI, &wifi_icon_info); /*lint !e64*/
    return wifi_icon_info.icon_array[0];
}

#endif


/*****************************************************************************/
//  Description : set the blue tooth state of when power on
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleBTStateWhenPowerOn(
                                           BLUETOOTH_STATE_TYPE  bluetooth_state
                                           )
{
    //SCI_TRACE_LOW:"MAIN_SetIdleBTStateWhenPowerOn %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_3597_112_2_18_2_24_18_102,(uint8*)"d", bluetooth_state);
    MAIN_SetIdleBlueToothState(bluetooth_state);
}

/*****************************************************************************/
//  Description : set the blue tooth state of the idle window
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleBlueToothState(
                                       BLUETOOTH_STATE_TYPE  bluetooth_state
                                       )
{
#ifdef  BLUETOOTH_SUPPORT /* lint_lai */
    MMI_IMAGE_ID_T      image_id = IMAGE_NULL;

    //SCI_TRACE_LOW:"MAIN_SetIdleBlueToothState %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_3612_112_2_18_2_24_18_103,(uint8*)"d", bluetooth_state);
    if (bluetooth_state != MMIIDLE_GetIdleWinInfo()->bluetooth_state)
    {
        MMIIDLE_GetIdleWinInfo()->bluetooth_state = bluetooth_state;
#if defined(BT_DIALER_SUPPORT)
        if ( ON_AND_DISCONNECT == MMIIDLE_GetIdleWinInfo()->bluetooth_state)
        {
            image_id = (MMI_IMAGE_ID_T)IMAGE_IDLE_TITLE_ICON_BT_HIDDEN; 
            //blue tooth is power on but disconnect 
        }
        else if ( ON_AND_CONNECT == MMIIDLE_GetIdleWinInfo()->bluetooth_state)
        {
            image_id = (MMI_IMAGE_ID_T)IMAGE_IDLE_TITLE_ICON_BT_VISIBLE; 
            //blue tooth is power on and connect 
        }
#elif defined(ADULT_WATCH_SUPPORT)
        if ( ON_AND_DISCONNECT == MMIIDLE_GetIdleWinInfo()->bluetooth_state)
        {
            image_id = (MMI_IMAGE_ID_T)res_stat_bluetooth; 
            //blue tooth is power on but disconnect 
        }
        else if ( ON_AND_CONNECT == MMIIDLE_GetIdleWinInfo()->bluetooth_state)
        {
            image_id = (MMI_IMAGE_ID_T)res_aw_stat_bluetooth_connect;
            //blue tooth is power on and connect 
        }
#else
        if ( ON_AND_VISIBLE == MMIIDLE_GetIdleWinInfo()->bluetooth_state)
        {
            image_id = (MMI_IMAGE_ID_T)res_stat_bluetooth; 
            //blue tooth is power on and visible
        }
        else if ( ON_AND_HIDDEN == MMIIDLE_GetIdleWinInfo()->bluetooth_state)
        {
            image_id = (MMI_IMAGE_ID_T)res_stat_bluetooth;
            //blue tooth is power on and hidden 
        }
#endif
        else
        {
            image_id = (MMI_IMAGE_ID_T)IMAGE_NULL;
        }
        GUIWIN_SetStbItemIconId(MMI_WIN_ICON_BLUE_TOOTH, image_id);
        
        if (image_id != IMAGE_NULL)
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_BLUE_TOOTH, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_BLUE_TOOTH, TRUE);
        }
#if defined MMI_ISTYLE_SUPPORT        
        if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_BLUE_TOOTH, TRUE);
        }
        else
#endif			
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_BLUE_TOOTH, TRUE);
        }
        
        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE        
        //刷新小屏的显示
        MMISUB_UpdateDisplaySubLcd();
#endif
    }
#endif

}
/*****************************************************************************/
//  Description : set the charge state
//  Global resource dependence :
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleNetWorkName(void)
{
    uint32 i = 0;

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        MMIIDLE_SetSimStatusBarInfo((MN_DUAL_SYS_E)i, MMI_WIN_ICON_SIGNAL + i, FALSE);
        SetNetworkNameInfo((MN_DUAL_SYS_E)i, FALSE); // Bug-1334911 显示运营商名称
    }

    GUIWIN_UpdateStb(); //bug1377050
    MMIIDLE_SendMsgToIdleWin(MSG_IDLE_UPDATE_NETWORK, PNULL);
#ifdef DROPDOWN_NOTIFY_SHORTCUT
    NotifyCenter_SendMsgToShortcutWin(MSG_FULL_PAINT ,PNULL);
#endif

}

/*****************************************************************************/
//  Description : set the charge state
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_UpdateIdleSignalState(void)
{
    uint32 i = 0;

    for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        MMIIDLE_SetSimStatusBarInfo((MN_DUAL_SYS_E)i, MMI_WIN_ICON_SIGNAL + i, FALSE);
#ifdef MMI_VOLTE_SUPPORT
        StatusAreaSetVolteInfo((MN_DUAL_SYS_E)i, MMI_WIN_ICON_VOLTE + i, FALSE);
#endif
    }

    // update
    GUIWIN_UpdateStb();
}


/*****************************************************************************/
//  Description : set state of tv 
//  Global resource dependence :  
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetTVSignalState(void)
{
#if defined(CMMB_SUPPORT) || defined(ATV_SUPPORT)
    uint8          signal_level = 0;
    MMI_IMAGE_ID_T image_id     = IMAGE_NULL;
    
    if(MMIAPIMTV_GetTVSignalLevel(&signal_level))
    {
        image_id = MMIAPIMTV_GetTVStatusImageID(signal_level);
    }
    else if(MMIAPIATV_GetTVSignalLevel(&signal_level))
    {
        image_id = MMIAPIATV_GetTVStatusImageID(signal_level);
    }
    
    GUIWIN_SetStbItemIconId(MMI_WIN_ICON_CMMB_SIGNAL, image_id);
    
    if (image_id == IMAGE_NULL)
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_CMMB_SIGNAL, FALSE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_CMMB_SIGNAL, TRUE);
    }
#if defined MMI_ISTYLE_SUPPORT    
    if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_CMMB_SIGNAL, TRUE);
    }
    else
#endif		
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_CMMB_SIGNAL, FALSE);
    }
    
    // update
    GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE    
    //刷新小屏的线路显示状态
    MMISUB_UpdateDisplaySubLcd();
#endif
#endif
}

/*****************************************************************************/
//  Description : set state of tv 
//  Global resource dependence :  
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetTVSignalFalse(void)
{
#if defined(CMMB_SUPPORT) || defined(ATV_SUPPORT)
    GUIWIN_SetStbItemVisible(MMI_WIN_ICON_CMMB_SIGNAL, FALSE);
#endif
}

/*****************************************************************************/
//  Description : set the battery level of the idle window
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleBatLevel(
                                 uint8 bat_level
                                 )
{
    GUIWIN_STBDATA_ICON_STYLE_E     icon_type   =    GUIWIN_STBDATA_ICON_NONE;

    //SCI_TRACE_LOW:"MAIN_SetIdleBatLevel bat_level = %d,idle level =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_3759_112_2_18_2_24_18_104,(uint8*)"dd",bat_level,MMIIDLE_GetIdleWinInfo()->batlevel);
    if (CHGMNG_GetModuleState()->bat_remain_cap == 100)
    {
        MMIIDLE_GetIdleWinInfo()->batlevel = bat_level;//bug id:2130162
        icon_type = GUIWIN_STBDATA_ICON_NORMAL;
        GUIWIN_SetStbItemIconStyle(MMI_WIN_ICON_BATTERY, icon_type);
        GUIWIN_SetStbItemIconCurFrame(MMI_WIN_ICON_BATTERY, MMIIDLE_GetIdleWinInfo()->batlevel);
        GUIWIN_UpdateStb();
        return;
    }

    if(bat_level != MMIIDLE_GetIdleWinInfo()->batlevel && !(MMIIDLE_GetIdleWinInfo()->is_charge))
    {
        MMIIDLE_GetIdleWinInfo()->batlevel = bat_level;
        
        GUIWIN_GetStbItemIconStyle(MMI_WIN_ICON_BATTERY, &icon_type);

        GUIWIN_SetStbItemIconCurFrame(MMI_WIN_ICON_BATTERY, MMIIDLE_GetIdleWinInfo()->batlevel);

        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE        
        //刷新小屏的显示，刷新电量
        MMISUB_UpdateDisplaySubLcd(); 
#endif
    }
}


/*****************************************************************************/
//  Description : set missed call state
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetCallingState(BOOLEAN is_calling)
{
    MMI_IMAGE_ID_T      image_id    =   IMAGE_IDLE_TITLE_ICON_CALLING;

    if (MMIAPICC_IsExistUncipheringCall())
    {
        image_id = IMAGE_IDLE_TITLE_ICON_UNCIPHERING_CALLING;
    }
    GUIWIN_SetStbItemIconId(MMI_WIN_ICON_CALLING, image_id);
    GUIWIN_SetStbItemIconCurFrame(MMI_WIN_ICON_CALLING, 0);
    
    MMIIDLE_GetIdleWinInfo()->is_calling = is_calling;

    if((MMIIDLE_GetIdleWinInfo()->is_calling))
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_CALLING, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_CALLING, FALSE);
    }
#if defined MMI_ISTYLE_SUPPORT    
    if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_CALLING, TRUE);
    }
    else
#endif		
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_CALLING, FALSE);
    }
    
    // update 
    GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE        
    //刷新小屏的显示，闹钟状态
    MMISUB_UpdateDisplaySubLcd();  
#endif
}

/*****************************************************************************/
//  Description : set missed call state
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetMissedCallState(BOOLEAN is_missed_call)
{
    MMI_IMAGE_ID_T      image_id    =   IMAGE_IDLE_TITLE_ICON_MISSED_CALL;
    GUIWIN_SetStbItemIconId(MMI_WIN_ICON_MISSED_CALL, image_id);
    GUIWIN_SetStbItemIconCurFrame(MMI_WIN_ICON_MISSED_CALL, 0);
    
    if(is_missed_call != MMIIDLE_GetIdleWinInfo()->is_missed_call)
    {
        MMIIDLE_GetIdleWinInfo()->is_missed_call = is_missed_call;


        
        if((MMIIDLE_GetIdleWinInfo()->is_missed_call))
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_MISSED_CALL, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_MISSED_CALL, FALSE);
        }
#if defined MMI_ISTYLE_SUPPORT         
        if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_MISSED_CALL, TRUE);
        }
        else
#endif			
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_MISSED_CALL, FALSE);
        }
        
        // update 
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE        
        //刷新小屏的显示，闹钟状态
        MMISUB_UpdateDisplaySubLcd(); 
#endif
    }
}

/*****************************************************************************/
//  Description : set the td state of the idle window
//  Global resource dependence : 
//  Author: maggie
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleTdState(void)
{
    //SCI_TRACE_LOW:"MAIN_SetIdleTdState"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_3864_112_2_18_2_24_19_105,(uint8*)"");
    // update
    GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE    
    //刷新小屏的显示，Gprs状态
    MMISUB_UpdateDisplaySubLcd();
#endif
}

/*****************************************************************************/
//  Description : set the unknown state of the idle window
//  Global resource dependence : 
//  Author: maggie
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleUnknownState(void)
{
    //SCI_TRACE_LOW:"MAIN_SetIdleUnknownState"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_3877_112_2_18_2_24_19_106,(uint8*)"");
    // update
    GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE    
    //刷新小屏的显示，Gprs状态
    MMISUB_UpdateDisplaySubLcd();
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetAllStbAppCallbackValid(
                                           BOOLEAN is_valid
                                           )
{
    uint32      i   = 0;

    for (i = 0; i < MMI_WIN_ICON_MAX; i++)
    {
        GUIWIN_SetItemHandleFuncValid(i, is_valid);
    }
}

/*****************************************************************************/
//  Description : set the Alarm state of the idle window
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleAlarmState(
                                   BOOLEAN alarm_state
                                   )
{
    if(alarm_state != MMIIDLE_GetIdleWinInfo()->is_alarm)
    {
        MMIIDLE_GetIdleWinInfo()->is_alarm = alarm_state;



        if((TRUE == MMIIDLE_GetIdleWinInfo()->is_alarm))
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_ALARM, TRUE);
        }
        else
        {
            GUIWIN_SetStbItemVisible(MMI_WIN_ICON_ALARM, FALSE);
        }
#if defined MMI_ISTYLE_SUPPORT         
        if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_ALARM, TRUE);
        }
        else
#endif			
        {
            GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_ALARM, TRUE);
        }
        
        // update 
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE        
        //刷新小屏的显示，闹钟状态
        MMISUB_UpdateDisplaySubLcd(); 
#endif
    }
}



/*****************************************************************************/
//  Description : set the java state of the idle window
//  Global resource dependence : 
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_UpdateIdleJavaState(BOOLEAN java_state)
{
#if !defined(JAVA_SUPPORT_TYPE_SVM) || (defined(JAVA_SUPPORT_TYPE_SVM) && defined(SUPPORT_BACKGROUND_RUN))
#if ( !defined(DPHONE_SUPPORT) && defined(JAVA_SUPPORT)) 
    if(java_state)
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_JAVA, TRUE); 
        
#ifdef PDA_UI_DROPDOWN_WIN
        MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_RUNNING_JAVA);//cr00115796 renwei modify
#endif
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_JAVA, FALSE);
#ifdef PDA_UI_DROPDOWN_WIN
        MMIDROPDOWNWIN_DelNotifyRecord(DROPDOWN_RUNNING_JAVA);//cr00115796 renwei modify
#endif
    }
#if defined MMI_ISTYLE_SUPPORT 
    if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_JAVA, TRUE);
    }
    else
#endif		
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_JAVA, FALSE);
    }
    // update
    //GUIWIN_UpdateStb();
    //刷新小屏的显示，短信状态
    //MMISUB_UpdateDisplaySubLcd();
#endif
#endif
}

/*****************************************************************************/
//  Description : set the Alarm state of the idle window
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_UpdateIdleEarPhoneState(void)
{


    if(GPIO_CheckHeadsetStatus())
    {   
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_EARPHONE, TRUE); 
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_EARPHONE, FALSE);
    }
#if defined MMI_ISTYLE_SUPPORT     
    if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_EARPHONE, TRUE);
    }
    else
#endif		
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_EARPHONE, FALSE);
    }

    // update
//    GUIWIN_UpdateStb();
    //刷新小屏的显示，短信状态
//    MMISUB_UpdateDisplaySubLcd();
}

/*****************************************************************************/
//  Description : set the vibrator state of the idle window
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleShakeState(
                                   BOOLEAN shake_state
                                   )
{
    MMIIDLE_GetIdleWinInfo()->is_shake = shake_state;
    
    //SCI_PASSERT((FALSE == s_idlewin_info.is_shake) || (FALSE == s_idlewin_info.is_silent), ("MAIN_SetIdleShakeState vibrator and silent are true!"));
    if((TRUE == MMIIDLE_GetIdleWinInfo()->is_shake) && (TRUE == MMIIDLE_GetIdleWinInfo()->is_silent))
    {
        //SCI_TRACE_LOW:"MAIN_SetIdleShakeState vibrator and silent are true!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_4024_112_2_18_2_24_19_107,(uint8*)"");
        return;
    }
    MAIN_UpdateIdleEnvsetState();    
}

/*****************************************************************************/
//  Description : if is silent of call ring in the idle window
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleSilentState(
                                    BOOLEAN silent_state
                                    )
{
    MMIIDLE_GetIdleWinInfo()->is_silent = silent_state;
    
    //SCI_PASSERT((FALSE == s_idlewin_info.is_shake) || (FALSE == s_idlewin_info.is_silent), ("MAIN_SetIdleSilentState vibrator and silent are true!"));
    if((TRUE == MMIIDLE_GetIdleWinInfo()->is_shake) && (TRUE == MMIIDLE_GetIdleWinInfo()->is_silent))
    {
        //SCI_TRACE_LOW:"MAIN_SetIdleSilentState vibrator and silent are true!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_4043_112_2_18_2_24_19_108,(uint8*)"");
        return;
    }
    MAIN_UpdateIdleEnvsetState();    
}

#ifdef MRAPP_SUPPORT
/*****************************************************************************/
// 	Description : set the mrapp state of the idle window
//	Global resource dependence : 
//  Author: zack
//	Note:modify by zack@20110726
/*****************************************************************************/
PUBLIC void MAIN_SetIdleMrappState(BOOLEAN show_icon)
{
    int32 img_id = 0;
    MRAPP_BACKRUN_INFO_T back_info = {0};

    MMIMRAPP_GetBackRunAPPInfo(&back_info);

    if(back_info.app_id == MRAPP_SDKQQ)
    {
        switch(back_info.icon_state)
        {
        case MRAPP_STATE_ONLINE:
            img_id = IMG_MRAPP_QQONLINE;
            break;
        case MRAPP_STATE_INVISIBLE:
            img_id = IMG_MRAPP_QQHIDE;
            break;
        case MRAPP_STATE_AWAY:
            img_id = IMG_MRAPP_QQAWAY;
            break;
        case MRAPP_STATE_OFFLINE:
            img_id = IMG_MRAPP_QQOFFLINE;
            break;
        default:
            break;
        }
    }
    else
    {
        switch(back_info.icon_state)
        {
        case MRAPP_STATE_ONLINE:
            img_id = IMG_MRAPP_ONLINE;
            break;
        case MRAPP_STATE_INVISIBLE:
            img_id = IMG_MRAPP_HIDE;
            break;
        case MRAPP_STATE_AWAY:
            img_id = IMG_MRAPP_AWAY;
            break;
        case MRAPP_STATE_OFFLINE:
            img_id = IMG_MRAPP_OFFLINE;
            break;
        default:
            break;
        }
    }
    GUIWIN_SetStbItemIconId(MMI_WIN_ICON_MRAPP, img_id);
		
    if(show_icon)
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_MRAPP, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_MRAPP, FALSE);
    }

#ifdef PDA_UI_DROPDOWN_WIN
    MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_RUNNING_MRAPP);
#endif
#if defined MMI_ISTYLE_SUPPORT 
	if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_MRAPP, TRUE);
    }
    else
#endif		
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_MRAPP, FALSE);
    }

    GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE    
	MMISUB_UpdateDisplaySubLcd();
#endif
}
#endif

#ifdef HERO_ENGINE_SUPPORT
PUBLIC void MAIN_SetIdleHeroappState(BOOLEAN show_icon)
{
    int32 img_id = 0;
    HERO_APP_BACKRUN_INFO_T back_info = {0};

    MMIHEROAPP_GetBackRunAPPInfo(&back_info);

    GUIWIN_SetStbItemIconId(MMI_WIN_ICON_HERO, img_id);

    if(show_icon)
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_HERO, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_HERO, FALSE);
    }

#ifdef PDA_UI_DROPDOWN_WIN
    MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_RUNNING_HERO_ENGINE);
#endif

	if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_HERO, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_HERO, FALSE);
    }

    GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE    
	MMISUB_UpdateDisplaySubLcd();
#endif
}
#endif
    
#ifdef QQ_SUPPORT
/*****************************************************************************/
// Description : 刷新idle下的QQ icon
// Global resource dependence : 
// Author: fen.xie
// Note:
/*****************************************************************************/
PUBLIC  void MMIAPIIDLE_UpdateQQState(void)
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    
    //SCI_TRACE_LOW:"MMIAPIIDLE_UpdateQQState "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_4140_112_2_18_2_24_19_109,(uint8*)"");
    image_id = MMIAPIQQ_GetIdleImageID();
    GUIWIN_SetStbItemIconId(MMI_WIN_ICON_SPREADTRUM_QQ, image_id);
    GUIWIN_SetStbItemIconCurFrame(MMI_WIN_ICON_SPREADTRUM_QQ, 0);
    
    if (image_id != IMAGE_NULL)
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_SPREADTRUM_QQ, TRUE);
#ifdef PDA_UI_DROPDOWN_WIN
        MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_QQ);
#endif
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_SPREADTRUM_QQ, FALSE);
#ifdef PDA_UI_DROPDOWN_WIN
        MMIDROPDOWNWIN_DelNotifyRecord(DROPDOWN_NOTIFY_QQ);
#endif
    }
#if defined MMI_ISTYLE_SUPPORT    
    if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_SPREADTRUM_QQ, TRUE);
    }
    else
#endif		
    {
        GUIWIN_SetStbItemRightAlign(MMI_WIN_ICON_SPREADTRUM_QQ, FALSE);
    }

    GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE    
    MMISUB_UpdateDisplaySubLcd();
#endif
}

/*****************************************************************************/
//  Description :InitIdleQQState
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL  void InitIdleQQState(void)
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    GUIWIN_STBDATA_ITEM_DATA_T  item_data = {0};
    uint32 winic_id = MMI_WIN_ICON_SPREADTRUM_QQ;

    image_id = MMIAPIQQ_GetIdleImageID();

    item_data.icon_info.icon_num = 1;
    item_data.icon_info.icon_type = GUIWIN_STBDATA_ICON_NORMAL;
    item_data.icon_info.is_display = TRUE;
    item_data.cur_frame = 0;
    item_data.is_handle_callback = TRUE;
    
    item_data.icon_info.icon_array[0] = image_id;
    item_data.app_handle_func = MMIAPIQQ_OpenMainWin;
    GUIWIN_AppendStbItemData(&item_data);

    if (image_id != IMAGE_NULL)
    {
         GUIWIN_SetStbItemVisible(winic_id, TRUE);
    }
    else
    {
         GUIWIN_SetStbItemVisible(winic_id, FALSE);
    }
#if defined MMI_ISTYLE_SUPPORT     
    if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        GUIWIN_SetStbItemRightAlign(winic_id, TRUE);
    }
    else
#endif		
    {
        GUIWIN_SetStbItemRightAlign(winic_id, FALSE);
    }
}
#endif

#ifdef SNS_PULLING_SUPPORT
/*****************************************************************************
*  Description : set the icon display state of the idle window
*  Global resource dependence : 
*  Author: David.Chen
*  Note:
******************************************************************************/
LOCAL void MAIN_UpdateIdleState(uint32 item_index, BOOLEAN isDisplay)
{
    GUIWIN_SetStbItemVisible(item_index, isDisplay);
#if defined MMI_ISTYLE_SUPPORT 
    if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        GUIWIN_SetStbItemRightAlign(item_index, TRUE);
    }
    else
#endif		
    {
        GUIWIN_SetStbItemRightAlign(item_index, FALSE);
    }   

    // 刷新控件
    GUIWIN_UpdateStb();
}

/*****************************************************************************
*  Description : set the icon display state of the idle window
*  Global resource dependence : 
*  Author: David.Chen
*  Note:
******************************************************************************/
PUBLIC void MAIN_UpdateIdleSnsState(BOOLEAN isDisplay, int widget_id)
{
    if (MMI_SNS_FACEBOOK == widget_id)
    {
        MAIN_UpdateIdleState(MMI_WIN_ICON_SNS_FB, isDisplay);
    }
    else if (MMI_SNS_TIWTTER == widget_id)
    {
        MAIN_UpdateIdleState(MMI_WIN_ICON_SNS_TWIT, isDisplay);
    }
}
#endif //SNS_PULLING_SUPPORT

#ifdef ASP_SUPPORT
/*****************************************************************************/
//  Description :InitIdleASPState
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
LOCAL  void InitIdleASPState(void)
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    GUIWIN_STBDATA_ITEM_DATA_T  item_data = {0};
    uint32 winic_id = MMI_WIN_ICON_ASP;

    image_id = MMIAPIASP_GetIdleImageID();

    item_data.icon_info.icon_num = 1;
    item_data.icon_info.icon_type = GUIWIN_STBDATA_ICON_NORMAL;
    item_data.icon_info.is_display = TRUE;
    item_data.cur_frame = 0;
    item_data.is_handle_callback = TRUE;
    
    item_data.icon_info.icon_array[0] = image_id;
    item_data.app_handle_func = MMIAPIASP_EnterAsp;
    GUIWIN_AppendStbItemData(&item_data);

    if(IMAGE_NULL != image_id)
    {
         GUIWIN_SetStbItemVisible(winic_id, TRUE);
    }
    else
    {
         GUIWIN_SetStbItemVisible(winic_id, FALSE);
    }
#if defined MMI_ISTYLE_SUPPORT     
    if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        GUIWIN_SetStbItemRightAlign(winic_id, TRUE);
    }
    else
#endif		
    {
        GUIWIN_SetStbItemRightAlign(winic_id, FALSE);
    }
}
/*****************************************************************************/
// Description : 刷新idle下的asp icon
// Global resource dependence : 
// Author: jun.hu
// Note:
/*****************************************************************************/
PUBLIC  void MMIAPIIDLE_UpdateAspState(void)
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    uint32 winic_id = MMI_WIN_ICON_ASP;

    //SCI_TRACE_LOW:"MMIAPIIDLE_UpdateAspState "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIDLE_STATUSBAR_4264_112_2_18_2_24_19_110,(uint8*)"");
    image_id = MMIAPIASP_GetIdleImageID();

    if (image_id != IMAGE_NULL)
    {
        GUIWIN_SetStbItemIconId(winic_id, image_id);
        GUIWIN_SetStbItemIconCurFrame(winic_id, 0);
        GUIWIN_SetStbItemVisible(winic_id, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(winic_id, FALSE);
    }
#if defined MMI_ISTYLE_SUPPORT 
    if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        GUIWIN_SetStbItemRightAlign(winic_id, TRUE);
    }
    else
#endif		
    {
        GUIWIN_SetStbItemRightAlign(winic_id, FALSE);
    }
    
    GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE    
    MMISUB_UpdateDisplaySubLcd();
#endif
}
#endif
/*****************************************************************************/
// Description : 设置状态栏耳机图标的显示 
// Global resource dependence : 
// Author: juan.wu
// Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_SetStbEpDisplay(BOOLEAN is_display)
{
    GUIWIN_SetStbItemVisible(MMI_WIN_ICON_EARPHONE, is_display);
    GUIWIN_UpdateStb();
}


/*****************************************************************************/
//  Description : 设置新语音消息图标
//  Global resource dependence :  
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleVMFlagState( BOOLEAN vm_status)
{
    if ( vm_status != MMIIDLE_GetIdleWinInfo()->is_new_vm )
    {
        MMIIDLE_GetIdleWinInfo()->is_new_vm = vm_status;
    }
    // update
    MAIN_SetMsgState(TRUE);
}

#ifdef MMI_IDLE_FDN_ICON_SUPPORT
/*****************************************************************************/
//  Description : get fdn display image
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T GetFDNDispImageId(void)
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    uint32 i = 0;

    MMIFDN_CONTEXT_T *fdn_context_ptr = PNULL;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        fdn_context_ptr = MMIAPIFDN_GetFDNContext(i);

        if (fdn_context_ptr->is_fdn_enable)
        {
            image_id = IMAGE_IDLE_TITLE_ICON_FDN;

            break;
        }
    }

    return image_id;
}
#ifdef FDN_SUPPORT
/*****************************************************************************/
// Description : display fdn func
// Global resource dependence : 
// Author: wancan.you
// Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayFDNFunc(void)
{
    MMIAPISET_OpenFDNWindow();

    return TRUE;
}
#endif
/*****************************************************************************/
//  Description : set FDN state
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetFDNState(BOOLEAN is_draw)
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;

    image_id = GetFDNDispImageId();

    GUIWIN_SetStbItemIconId(MMI_WIN_ICON_FDN, image_id);
    GUIWIN_SetStbItemIconCurFrame(MMI_WIN_ICON_FDN, 0);
    
    if (image_id != IMAGE_NULL)
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_FDN, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_FDN, FALSE);
    }

    if (is_draw)
    {
        // update
        GUIWIN_UpdateStb();
#ifndef SUBLCD_SIZE_NONE
        MMISUB_UpdateDisplaySubLcd();
#endif
    }
}
#endif
#ifdef MMI_MULTISIM_COLOR_SUPPORT_STATUSICON
/*
const  uint32 statusBarSignalItem[]=
{
      MMI_WIN_ICON_SIGNAL,        // MAIN LCD sim1信号量提示
#ifndef MMI_MULTI_SIM_SYS_SINGLE
      MMI_WIN_ICON_SIGNAL_2,        //MAIN LCD sim2信号量提示
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
      MMI_WIN_ICON_SIGNAL_3,        //MAIN LCD sim3信号量提示
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
      MMI_WIN_ICON_SIGNAL_4,        //MAIN LCD sim4信号量提示
#endif
#endif
};
*/
/*****************************************************************************/
//  Description : Init satatus bar sim color
//  Global resource dependence : none
//  Author:  sam.hua
//  Note: 
/*****************************************************************************/
LOCAL void MMIMAIN_InitStatusBarOfSimColor(void)
{
    GUI_BG_T std_bg = {0};
    MN_DUAL_SYS_E   dual_sys=MN_DUAL_SYS_1;
    GUISTBDATA_ICON_T icon_info = {0};
    GUI_COLOR_T color;
	
    std_bg.bg_type = GUI_BG_COLOR;
    std_bg.shape =GUI_SHAPE_ROUNDED_RECT;
    for( dual_sys=MN_DUAL_SYS_1;dual_sys<MMI_DUAL_SYS_MAX;dual_sys++)
    {
        //Read color
        //GUIWIN_SetStbItemBg(MMI_WIN_ICON_SIGNAL+dual_sys,&std_bg);
        color = MMIAPISET_GetSimColor(dual_sys);

        GUISTBDATA_GetItemIcon(MMI_WIN_ICON_SIGNAL+dual_sys, &icon_info);
        icon_info.color				= color;
        GUISTBDATA_SetItemIcon(MMI_WIN_ICON_SIGNAL+dual_sys,&icon_info);
        
    }
    return;
}

/*****************************************************************************/
//  Description : Set sim color for statusbar
//  Global resource dependence : none
//  Author: sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMAIN_UpdateStatusBarBySimColor(MN_DUAL_SYS_E dual_sys,GUI_COLOR_T color)
{
#if 0
    GUI_BG_T std_bg = {0};

    if (dual_sys>=MMI_DUAL_SYS_MAX)
    {
        dual_sys = MN_DUAL_SYS_1;
    }
    
    std_bg.bg_type  = GUI_BG_COLOR;
    std_bg.color    = color;
    std_bg.shape    = GUI_SHAPE_ROUNDED_RECT;
    GUIWIN_SetStbItemBg(MMI_WIN_ICON_SIGNAL+dual_sys,&std_bg);
#else
    GUISTBDATA_ICON_T icon_info = {0};

    if (dual_sys>=MMI_DUAL_SYS_MAX)
    {
        dual_sys = MN_DUAL_SYS_1;
    }
    GUISTBDATA_GetItemIcon(MMI_WIN_ICON_SIGNAL+dual_sys, &icon_info);
    icon_info.color					= color;
    GUISTBDATA_SetItemIcon(MMI_WIN_ICON_SIGNAL+dual_sys,&icon_info);
#endif
    return;
}


#endif

#ifdef MET_MEX_QQ_SUPPORT 
PUBLIC int32 GetMasterQQIconId(void)
{
    return MEX_MASTER_QQ_STATUS;
}

PUBLIC int32 GetSlaveQQIconId(void)
{
    return MEX_SLAVE_QQ_STATUS;
}
#endif
//2011.06.09 黄炳辉发现多
//#endif
#ifdef MET_MEX_DOUBLE_QQ_SUPPORT 
PUBLIC int32 GetMasterQQIconId(void)
{
    return MEX_MASTER_DOUBLE_QQ_STATUS;
}

PUBLIC int32 GetSlaveQQIconId(void)
{
    return MEX_SLAVE_DOUBLE_QQ_STATUS;
}
#endif
#ifdef MET_MEX_MSN_SUPPORT
PUBLIC int32 GetMSNIconId(void)
{
    return MEX_MSN_STATUS;
}
#endif
#ifdef MET_MEX_FETION_SUPPORT	
PUBLIC int32 GetFETIONIconId(void)
{
    return MEX_FETION_STATUS;
}
#endif

#ifdef MCARE_V31_SUPPORT
PUBLIC BOOLEAN MMIMAIN_MCARE_QQCallBack(void)
{
    if(McareV31_IsMcareRunningFront())
    {
        return TRUE;
    }
    McfIdle_HandleIconFunc_QQ(); /*lint !e718 !e746*/
    return TRUE;
}
PUBLIC BOOLEAN MMIMAIN_MCARE_QCCallBack(void)
{
    if(McareV31_IsMcareRunningFront())
    {
        return TRUE;
    }
    MCareV31_QQContact_Shortcut();
    return TRUE;
}

PUBLIC uint32 Mcare_idle_iconid(void)
{
	return MMI_WIN_ICON_MCARE_APP;
}
#endif//#ifdef MCARE_V31_SUPPORT
#ifdef MMI_BATTERY_PERCENT_IN_STATUSBAR
/*****************************************************************************/
//  Description :MMIAPIIDLE_GetBatteryPercentStbTxt
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 获得status bar上电池电量百分比的字串，同时也记录该百分比
/*****************************************************************************/
PUBLIC GUIWIN_STBDATA_TEXT_T  MMIAPIIDLE_GetBatteryPercentStbTxt(void)
{
    GUIWIN_STBDATA_TEXT_T stb_text_info = {0};
    uint32               bat_level   =  0;
    uint8           num_str[16] = {0};
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();

    if(PNULL != p_chgmng_info)
    {  
        bat_level = p_chgmng_info->bat_remain_cap;
        
        MMIAPIPHONE_SetPreBatteryCap(bat_level);

        SCI_MEMSET(num_str,0,sizeof(char)*16);
#ifdef SCREEN_SHAPE_CIRCULAR
        sprintf((char*)num_str, "%d%s", (int)bat_level, "%");
#else
        sprintf((char*)num_str, "%d", (int)bat_level);
#endif
        MMIAPICOM_StrToWstr(num_str, stb_text_info.wstr_ptr);
        stb_text_info.wstr_len = MMIAPICOM_Wstrlen(stb_text_info.wstr_ptr);
        stb_text_info.is_display = TRUE;

        // 取最小值，防止越界
        stb_text_info.wstr_len = MIN(stb_text_info.wstr_len, MMIIDLE_BATTERY_PERCENT_LEN);

        stb_text_info.font_color = MMI_WHITE_COLOR;//MMI_GRAY_WHITE_COLOR; CRNEWMS00203710
        stb_text_info.align = ALIGN_HVMIDDLE; 
        stb_text_info.font_type = MMI_STATUS_FONT_TYPE;
    }
    return stb_text_info;
}
#endif

PUBLIC BOOLEAN MMIAPI_isPdpAvailable(void)
{
#ifndef WIN32
     MN_DUAL_SYS_E dual_sys = 0;//MNGPRS_GetSwitchDataCard();
	 SCI_TraceLow("MMIAPI_isPdpAvailable event pdp=%d",s_pdp_event[dual_sys]);
    if(MMIPDP_NOTIFY_EVENT_ACTIVED == s_pdp_event[dual_sys])
     {
        return FALSE;
     }
#endif
    return TRUE;
}
