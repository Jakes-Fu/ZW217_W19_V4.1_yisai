/****************************************************************************
** File Name:      mmiwww.c                                            *
** Author:                                                                    *
** Date:           06/15/2009                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe the WWW Browser            *
*****************************************************************************
**                         Important Edit History                            *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 06/2009        wancan.you        Create
** 
****************************************************************************/
#define _MMIWWW_C_  

/**-------------------------------------------------------------------------*
**                         Dependencies                                     *
**--------------------------------------------------------------------------*/
#include "mmi_app_www_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"
#include "mmk_timer.h"
#include "in_message.h"
#include "mmi_applet_table.h"
#include "mmi_signal_ext.h"
#include "mmiwww_internal.h"
#include "mmiwww_netfront.h"
#include "mmiwww_export.h"
#include "mmiwww_menutab.h"
#include "mmiphone_export.h"
#include "mn_api.h"
#include "mmipdp_export.h"
#include "mmiwww_func.h"
#include "mmiwww_id.h"
#include "mmipub.h"
#include "mmi_image.h"
#include "mmiudisk_export.h"
#include "mmi_appmsg.h"
#include "mmimtv_export.h"
#include "mmivt_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#ifdef DL_SUPPORT
#include "mmidl_export.h"
#endif
#include "freq_cfg.h"

#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif

#include "mmicl_export.h"

#ifdef BROWSER_SUPPORT_NETFRONT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                          EXTERN DECLARE                                *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
double _HUGE = 0x7FFFFFFF;
#endif

/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
MMI_APPLICATION_T g_mmiwww_app;         //www的实体

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL WWW_APPLET_T *s_applet_instance = PNULL; /* Temp*/
#ifndef WIN32
LOCAL uint32 s_mmiwww_chng_freq_req_handler = CHNG_FREQ_REQ_HANDLER_NULL;
#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : the process message function of the WWW
//  Global resource dependence : 
//  Author:jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E WWWApplet_HandleEvent(IAPPLET_T* iapplet_ptr, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : Initialize netfront
//  Global resource dependence : 
//  Author:jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIWWW_Initialize(MMIWWW_INSTANCE_T *self, DPARAM param);

/*****************************************************************************/
//  Description : Finalize netfront
//  Global resource dependence : 
//  Author:jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIWWW_Finalize(MMIWWW_INSTANCE_T *self, DPARAM param);

/*****************************************************************************/
// 	Description : handle www ps msg
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWPsMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

/*****************************************************************************/
// 	Description : process pdp signal
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void HandleWWWPDPMsg(MMIPDP_CNF_INFO_T *msg_ptr);

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
//applet的数组
LOCAL const CAF_STATIC_APPLET_T s_mmiwww_applet_array[] = 
{
    { GUID_NAME_DEF(SPRD_WWW_APPLET_ID), sizeof(WWW_APPLET_T), WWWApplet_HandleEvent, IMAGE_MAINMENU_ICON_BROWSER, TXT_BROWSER },
};

//applet的信息
LOCAL const CAF_STATIC_APPLET_INFO_T s_mmiwww_applet_info = 
{
    (CAF_STATIC_APPLET_T*)s_mmiwww_applet_array,
    ARR_SIZE(s_mmiwww_applet_array)
};

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register www module applet info
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void RegAppletInfo(void)
{
    MMI_RegAppletInfo( MMI_MODULE_WWW, &s_mmiwww_applet_info );
}

/*****************************************************************************/
//  Description : Start WWW browser
//  Global resource dependence : 
//  Author:jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWWW_StartWWW(const MMIBROWSER_ENTRY_PARAM * entry_param_ptr)
{
    MMI_APPLET_START_T start ={0};
    WWW_APPLET_T *www_applet_ptr = MMIWWW_APPLETINSTANCE();
    MMIWWW_CUSTOMIZED_CONNECTION_T customized_connection = {0};
    BOOLEAN is_applet_opened = FALSE;
    uint16 sim_ok = MN_DUAL_SYS_1;
    uint16 sim_num = 0;
#ifdef MMI_WIFI_SUPPORT    
    BOOLEAN is_wifi_on = FALSE;    
#endif

    if (PNULL == entry_param_ptr)
    {
        return;
    }

    //SCI_TRACE_LOW:"MMIAPIWWW_StartWWW start type=%d,is_connection_customized=%d,connection_index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_156_112_2_18_3_11_27_0,(uint8*)"ddd", entry_param_ptr->type, entry_param_ptr->is_connection_customized, entry_param_ptr->connection_index);

    if (PNULL == www_applet_ptr)
    {
        customized_connection.dual_sys = entry_param_ptr->dual_sys;
        customized_connection.is_connection_customized = entry_param_ptr->is_connection_customized;
        customized_connection.connection_index = entry_param_ptr->connection_index;

        start.guid = SPRD_WWW_APPLET_ID;
#ifdef MMIWWW_APPLET_RUN_WITHOUT_WIN
        start.state = MMI_APPLET_STATE_BG_HIDE_WIN;
#endif /* MMIWWW_APPLET_NO_WIN */
        start.param_ptr = &customized_connection;
        start.size_of_param = sizeof(customized_connection);

        MMIWWW_SetInitUA(entry_param_ptr->user_agent_ptr);
        
        MMK_StartApplet(&start);
    }
    else
    {
        is_applet_opened = TRUE;

#ifndef MMIWWW_APPLET_RUN_WITHOUT_WIN
        MMK_SetAppletState(MMIWWW_HANDLE(www_applet_ptr), MMI_APPLET_STATE_BG_HIDE_WIN, FALSE);
#endif

        MMK_GrabWinFocusByApplet(MMIWWW_HANDLE(www_applet_ptr));
    }

    if (MMIBROWSER_ACCESS_MAINMENU == entry_param_ptr->type || MMIBROWSER_ACCESS_URL == entry_param_ptr->type)
    {
        MMIWWW_TidyExistedWin();
    }

    www_applet_ptr = MMIWWW_APPLETINSTANCE();

    if (PNULL == www_applet_ptr) /* Fail to Create applet */
    {
        return;
    }

    www_applet_ptr->www_instance.enter_type = entry_param_ptr->type;

    if (!is_applet_opened)
    {
        sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok, 1);
#ifdef MMI_WIFI_SUPPORT
        is_wifi_on = MMIAPIWIFI_GetIsWlanOn();
        is_wifi_on = is_wifi_on&&MMIAPIWIFI_APIsAvailable();
#endif
        //if dual sys is defined, use GPRS
        if (customized_connection.dual_sys < MMI_DUAL_SYS_MAX)
        {
            customized_connection.bearer_type = MMIPDP_INTERFACE_GPRS;
        }
		//chinamobile own service，use GPRS
        else if ((MMIBROWSER_ACCESS_MAINMENU == entry_param_ptr->type
            || MMIBROWSER_ACCESS_URL == entry_param_ptr->type)
            && (entry_param_ptr->is_connection_customized))
        {
    	    customized_connection.bearer_type = MMIPDP_INTERFACE_GPRS;
    	    if(1 == sim_num)
    	    {
    		    customized_connection.dual_sys = (MN_DUAL_SYS_E)sim_ok;
    	    }
    	    else if(1 < sim_num)
    	    {
                MMIWWW_SaveUrl(BROWSER_START_NORMAL, (char *)entry_param_ptr->url_ptr);
                MMIWWW_SelectNetworkType(FALSE);  
                return;
    	    }
    	    else //no sim valid
    	    {
                MMIAPIPHONE_AlertSimNorOKStatus();

                if (MMIWWW_IsNeedStopApplet())
                {
                    MMIAPIWWW_StopWWW();
                }

                return;
    	    }
        }
#ifdef MMI_WIFI_SUPPORT        
        else if(is_wifi_on) //wifi or wlan is available
        {
            if (0 == sim_num)
            {
                customized_connection.bearer_type = MMIPDP_INTERFACE_WIFI;
            }
            else //sim_num > 1
            {
                //select sim or wifi
                MMIWWW_SaveUrl(BROWSER_START_NORMAL, (char *)entry_param_ptr->url_ptr);
                MMIWWW_SelectNetworkType(TRUE);  
                return;                
            }
        }
#endif        
        else //no wifi 
        {
    	    customized_connection.bearer_type = MMIPDP_INTERFACE_GPRS;
    	    if(1 == sim_num)
    	    {
    		    customized_connection.dual_sys = sim_ok;
    	    }
    	    else if(1 < sim_num)
    	    {
                MMIWWW_SaveUrl(BROWSER_START_NORMAL, (char *)entry_param_ptr->url_ptr);
                MMIWWW_SelectNetworkType(FALSE);  
                return;
    	    }
    	    else //no sim valid
    	    {
                MMIAPIPHONE_AlertSimNorOKStatus();

                if (MMIWWW_IsNeedStopApplet())
                {
                    MMIAPIWWW_StopWWW();
                }

                return;
    	    }
        }


        MMIWWW_SetCustomizedConnection(&customized_connection);
    }

    switch (entry_param_ptr->type)
    {
    case MMIBROWSER_ACCESS_MAINMENU:
        if (!is_applet_opened)
        {
            MMIWWW_OpenWWWMainMenuWin(MMIWWW_CTRL_ADDRESS_LABEL);

#ifdef MMI_WIFI_SUPPORT
#ifdef MMIWWW_PDP_APPLET_CONNECT
            MMIWWW_NETCONNECT();
#endif
#endif
        }
        break;

    case MMIBROWSER_ACCESS_URL:
        if (!MMIWWW_OpenWWWBrowserWin(BROWSER_START_BY_URL, (char *)entry_param_ptr->url_ptr))
        {
            if (MMIWWW_IsNeedStopApplet())
            {
                MMIAPIWWW_StopWWW();
            }
        }
        else if (!is_applet_opened)
        {
#ifdef MMI_WIFI_SUPPORT
#ifdef MMIWWW_PDP_APPLET_CONNECT
            MMIWWW_NETCONNECT();
#endif
#endif
        }
        break;

    case MMIBROWSER_ACCESS_DOWNLOAD_BY_URL:
        {
            MMIDL_DOWNLOAD_INFO_T download_info = {0};
        
            MMIWWW_OpenWWWBrowserWin(BROWSER_START_BY_URL,  MMIWWW_ABOUT_BLANK);
            download_info.mime_type = MIME_EXTEND_APP_JAR;
            download_info.url = (uint8 *)entry_param_ptr->url_ptr;
        
            MMIAPIDL_StartDownload(MMIDL_CREATE_NEW, &download_info);
        }
        break;

    case MMIBROWSER_ADD_BOOKMARK:
        {
            uint16 str_len = 0;
            uint8 *added_url = PNULL;
            uint8 *added_title = PNULL;
            str_len = SCI_STRLEN(entry_param_ptr->url_ptr);
            str_len = (uint16)MIN(str_len, MAX_WEB_URL_LEN);

            added_url = SCI_ALLOCA((str_len + 1) * sizeof(uint8));
            if (PNULL == added_url)
            {
                return;
            }
            SCI_MEMSET(added_url, 0x00, ((str_len + 1) * sizeof(uint8)));
            strncpy((char*)added_url, entry_param_ptr->url_ptr,  str_len);
            MMIWWW_NetfrontSetAddedUrl(added_url);
            if (PNULL != entry_param_ptr->url_title_ptr)
            {
                str_len = SCI_STRLEN(entry_param_ptr->url_title_ptr);
                str_len = (uint16)MIN(str_len, MAX_WEB_TITLE_LEN);
                added_title = SCI_ALLOCA((str_len + 1) * sizeof(uint8));
                if (PNULL == added_title)
                {
                    SCI_FREE(added_url);
                    return;
                }
                SCI_MEMSET(added_title, 0x00, ((str_len + 1) * sizeof(uint8)));
                strncpy((char*)added_title, entry_param_ptr->url_title_ptr,  str_len);
                MMIWWW_NetfrontSetAddedTitle(added_title);
            }

            MMIWWW_ChangeBookmarkWin(MMIWWW_BM_CHANGE_ADD_URL);
        }
        break;

    case MMIBROWSER_ACCESS_FLOWRATE:
        MMIAPICL_OpenGprsFlowRateWin();
        break;

    default:
        MMIAPIWWW_StopWWW();
        break;
    }
}

 /*****************************************************************************/
//  Description : Stop WWW browser
//  Global resource dependence : 
//  Author:jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWWW_StopWWWEx(BOOLEAN is_minimize)
{
    WWW_APPLET_T* www_applet_ptr = MMIWWW_APPLETINSTANCE();
    if (PNULL != www_applet_ptr)
    {
        MMI_HANDLE_T applet_handle = MMIWWW_HANDLE(www_applet_ptr);

        if (is_minimize)
        {
#ifndef MMIWWW_APPLET_RUN_WITHOUT_WIN
            MMK_SetAppletState(applet_handle, MMI_APPLET_STATE_BG_HIDE_WIN, TRUE);
#endif

            MMK_HideWinByApplet(applet_handle);
        }
        else
        {
            MMK_CloseApplet(applet_handle);
        }
    }
}

/*****************************************************************************/
//  Description : Init WWW
//  Global resource dependence : 
//  Author:jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWWW_InitWWW(void)
{
    g_mmiwww_app.ProcessMsg = HandleWWWPsMsg;

    MMIWWW_RegMenuGroup();

#ifndef WIN32
    if (CHNG_FREQ_REQ_HANDLER_NULL == s_mmiwww_chng_freq_req_handler)
    {
        s_mmiwww_chng_freq_req_handler = CHNG_FREQ_GetReqHandler("mmiwww");
    }   
#endif
}

/*****************************************************************************/
//  Description : Set systerm frequency for mmi www
//  Global resource dependence :                     
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIWWW_SetFreq(void)
{
#ifndef WIN32
    if (CHNG_FREQ_REQ_HANDLER_NULL != s_mmiwww_chng_freq_req_handler)
    {
        CHNG_FREQ_SetArmClk(s_mmiwww_chng_freq_req_handler, (CHNG_FREQ_INDEX_E)FREQ_INDEX_WWW);       
    }
#endif
    //SCI_TRACE_LOW:"\"\"  MMIWWW_SetFreq   \"#"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_433_112_2_18_3_11_27_1,(uint8*)"");
    
    return 1;
}
 
/*****************************************************************************/
//  Description : Get systerm frequency for mmi www
//  Global resource dependence :                     
//  Author: wancan.you
//  Note: 
/*****************************************************************************/ 
PUBLIC uint32 MMIWWW_RestoreFreq(void)
{
#ifndef WIN32
     if (CHNG_FREQ_REQ_HANDLER_NULL != s_mmiwww_chng_freq_req_handler)
    {
        CHNG_FREQ_RestoreARMClk(s_mmiwww_chng_freq_req_handler);
    }       
#endif
    //SCI_TRACE_LOW:"\"\"  MMIWWW_SetFreq   \"#"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_450_112_2_18_3_11_27_2,(uint8*)"");

    return 1;
}

/*****************************************************************************/
//  Description : Initialize netfront
//  Global resource dependence : 
//  Author:jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIWWW_Initialize(MMIWWW_INSTANCE_T *self, DPARAM param)
{
    //SCI_TRACE_LOW:"MMIWWW_Initialize"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_461_112_2_18_3_11_27_3,(uint8*)"");

    MMIWWW_SetCustomizedConnection(param);

    if (!MMIWWW_CreateMemory())
    {
        MMIAPIWWW_StopWWW();
        MMIWWW_OpenWWWAlertWin(PNULL, TXT_COMMON_NO_MEMORY, PNULL, IMAGE_PUBWIN_WARNING);
        return FALSE;
    }

    if (MMIWWW_GetFirstEntry())
    {
        MMIWWW_CleanUserInfo();
    }

    MMIWWW_Netfront_Initialize(self);
    if (MMIWWW_GetFirstEntry()) /* Set after NF init */
    {
        MMIWWW_SetDefaultUserData(self);
        MMIWWW_SetFirstEntry(FALSE);
    }

#ifndef MMI_WIFI_SUPPORT
#ifdef MMIWWW_PDP_APPLET_CONNECT
    MMIWWW_NETCONNECT();
#endif

    MMIWWW_SetHTTPProxy(PNULL);
#endif

    return TRUE;
}

/*****************************************************************************/
//  Description : Finalize netfront
//  Global resource dependence : 
//  Author:jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIWWW_Finalize(MMIWWW_INSTANCE_T *self, DPARAM param)
{
    //SCI_TRACE_LOW:"MMIWWW_Finalize"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_501_112_2_18_3_11_27_4,(uint8*)"");

#ifdef MMIWWW_PDP_APPLET_CONNECT
    MMIWWW_NETDISCONNECT();
#endif

    MMIWWW_Netfront_Finalize(self);

    MMIWWW_ResetConnectionSettings();

    MMIWWW_DestroyMemory();

    return TRUE;
}

/*****************************************************************************/
//  Description : the process message function of the WWW
//  Global resource dependence : 
//  Author:jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  WWWApplet_HandleEvent(    
                                                            IAPPLET_T*          iapplet_ptr,
                                                            MMI_MESSAGE_ID_E    msg_id, 
                                                            DPARAM              param
                                                            )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    WWW_APPLET_T* www_applet_ptr = (WWW_APPLET_T*)iapplet_ptr;

    //SCI_TRACE_LOW:"WWWApplet_HandleEvent msg_id = 0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_529_112_2_18_3_11_28_5,(uint8*)"d",msg_id);

    switch (msg_id)
    {
    case MSG_APPLET_START:
        s_applet_instance = www_applet_ptr;
        MMIWWW_Initialize(MMIWWW_NETFRONT(www_applet_ptr), param);
        break;

    case MSG_APPLET_STOP:
        MMIWWW_Finalize(MMIWWW_NETFRONT(www_applet_ptr), param);
        s_applet_instance = PNULL;
        break;

    case MSG_WWW_NOTIFY_UPLOAD:
        MMIWWW_Netfront_SuspendBrowser();//added by rui.li 20110128
        MMIWWW_UploadEnterExplorer((DPARAM)param);
        break;

    /*added by rui.li 20110128 begin*/
    case MSG_WWW_NOTIFY_UPLOAD_END:
        MMIWWW_Netfront_ResumeBrowser();
        break;
    /*added by rui.li 20110128 end*/

    case MSG_WWW_REQUEST_JSCONF:
        MMIWWW_OpenJSConfirmWin((DPARAM)param);
        break;

    case MSG_WWW_REQUEST_JSPROMPT:
        MMIWWW_OpenJSPromptWin();
        break;

    case MSG_WWW_REQUEST_WMLJSCONF:
        MMIWWW_OpenWMLSConfirmWin((DPARAM)param);
        break;

    case MSG_WWW_REQUEST_WMLJSPROMPT:
        MMIWWW_OpenWMLSPromptWin();
        break;

    case MSG_WWW_AUTH_MSG:
        MMIWWW_OpenAuthWin();
        break;

    case MSG_WWW_STOP_APPLET:
        MMIAPIWWW_StopWWW();
        break;

    case MSG_WWW_NO_MEMORY:
        if (SETTINGS_IMAGES_NOT_DISPLAY != MMIWWW_GetImageDisplayMode())
        {
            MMIWWW_Netfront_SetImageDisplayMode(SETTINGS_IMAGES_NOT_DISPLAY);
            MMIWWW_Netfront_SetScriptMode(SETTINGS_SCRIPT_DISABLED);

            if (MMIWWW_GetBrowserFocusFlag())
            {
                MMIWWW_OpenWWWAlertWin(PNULL, TXT_WWW_MEMORY_ALERT, PNULL, IMAGE_PUBWIN_WARNING);
            }
        }
        break;

    case MSG_WWW_QUERY_NETWORK:
        MMIPUB_OpenQueryWinByTextId(TXT_WLAN_QUERY, IMAGE_PUBWIN_QUERY, PNULL, MMIWWW_HandleSelectWLAN);
        break;

    case MSG_APPLET_SWITCH:
//         {
//             MMK_GrabWinFocusByApplet(www_applet_ptr->caf_applet.app_handle);
//         }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : Check Is Network Ready
//  Global resource dependence : 
//  Author:jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_NetworkIsReady(MMIWWW_INSTANCE_T *self)
{
    //SCI_TRACE_LOW:"MMIWWW_NetworkIsReady=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_615_112_2_18_3_11_28_6,(uint8*)"d",self->pdp_info.pdp_state);

    return (BOOLEAN)(MMIWWW_PDP_ACTIVE_OK == self->pdp_info.pdp_state);  
}

/*****************************************************************************/
//  Description : Get WWW Netfront Instance
//  Global resource dependence : 
//  Author:jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC MMIWWW_INSTANCE_T *MMIWWW_GetWWWInstance(void)
{
    WWW_APPLET_T* www_applet_ptr = PNULL;
    www_applet_ptr = MMIWWW_APPLETINSTANCE();

    if (PNULL != www_applet_ptr)
    {
        return &www_applet_ptr->www_instance;
    }

    return PNULL;
}

/*****************************************************************************/
//  Description : Get WWW Netfront Instance bookmark info
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC WWW_BOOKMARK_T *MMIWWW_GetWWWInstanceBM(void)
{
    MMIWWW_INSTANCE_T * www_instance = MMIWWW_INSTANCE();
    if(PNULL == www_instance)
    {
        return PNULL;
    }
    return &(www_instance->bookmark_info);
}

/*****************************************************************************/
//  Description : Get WWW Applet Instance
//  Global resource dependence : 
//  Author:jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC WWW_APPLET_T *MMIWWW_GetAppletInstance(void)
{
#if 0
        WWW_APPLET_T* www_applet_ptr = NULL;
        www_applet_ptr = (WWW_APPLET_T*)MMK_GetAppletPtr(SPRD_WWW_APPLET_ID);

        return www_applet_ptr;
#else
        return s_applet_instance;
#endif
}

/*****************************************************************************/
//  Description : Get LCD Info
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_GetLcdDevInfo(GUI_LCD_ID_E id, GUI_LCD_DEV_INFO* lcd_dev_info)
{
    switch(id)
    {
    case GUI_MAIN_LCD_ID:
        lcd_dev_info->lcd_id = GUI_MAIN_LCD_ID;
        lcd_dev_info->block_id = GUI_BLOCK_MAIN;
        break;

    case GUI_SUB_LCD_ID:
        lcd_dev_info->lcd_id = GUI_SUB_LCD_ID;
        lcd_dev_info->block_id = GUI_BLOCK_MAIN;
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : set PDP state
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_SetPDPState(MMIWWW_INSTANCE_T *self, slim_int in_state)
{
    //SCI_TRACE_LOW:"MMIWWW_SetPDPState:old %d-new %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_696_112_2_18_3_11_28_7,(uint8*)"dd",self->pdp_info.pdp_state, in_state);

    self->pdp_info.pdp_state = (MMIWWW_PDP_STATE_E)in_state;
}

/*****************************************************************************/
//  Description : Net ConnectEx
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL slim_int MMIWWW_NetConnectEx(MMIWWW_INSTANCE_T *self)
{
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    MMIPDP_ACTIVE_INFO_T app_info = {0};
    MMIWWW_PDP_INFO_T pdp_info = {0};

#ifdef MMI_WIFI_SUPPORT
    if (MMIPDP_INTERFACE_WIFI == self->customized_connection.bearer_type)
    {    
        app_info.app_handler = MMI_MODULE_WWW;
        app_info.handle_msg_callback = HandleWWWPDPMsg;
        app_info.ps_interface = MMIPDP_INTERFACE_WIFI;
    }
    else if (MMIPDP_INTERFACE_GPRS == self->customized_connection.bearer_type)
#endif
    {
        dual_sys = self->customized_connection.dual_sys;
        if (dual_sys >= MMI_DUAL_SYS_MAX)
        {
            //SCI_TRACE_LOW:"MMIWWW_NetConnectEx err dual_sys:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_724_112_2_18_3_11_28_8,(uint8*)"d", dual_sys);
            return SLIM_E_GENERIC;
        }

        if (!MMIAPIPHONE_IsSimAvailable(dual_sys)) //TODO
        {
            //SCI_TRACE_LOW:"MMIWWW_NetConnectEx:MMIAPIPHONE_IsSimOk not OK"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_730_112_2_18_3_11_28_9,(uint8*)"");
            MMIAPIPHONE_AlertSimNorOKStatus();
            MMIWWW_SetPDPState(self, MMIWWW_PDP_ACTIVE_FAILED);

            return SLIM_E_GENERIC;
        }

        MMIWWW_NetfrontGetSettingsPDPInfo(&pdp_info, dual_sys);
        MMIWWW_NetfrontSetPDPInfo(&pdp_info);
        
        app_info.app_handler = MMI_MODULE_WWW;
        app_info.auth_type = pdp_info.connect_setting.auth_type;
        app_info.apn_ptr = (char*)pdp_info.connect_setting.apn;
        app_info.user_name_ptr = (char*)pdp_info.connect_setting.username;
        app_info.psw_ptr = (char*)pdp_info.connect_setting.password;
        app_info.dual_sys = dual_sys;
        app_info.ps_service_type = BROWSER_E;
        app_info.ps_service_rat = MN_TD_PREFER;
        app_info.priority = 3;
        app_info.handle_msg_callback = HandleWWWPDPMsg;
        app_info.storage = MN_GPRS_STORAGE_ALL;
    }

    MMIWWW_SetPDPState(self, MMIWWW_PDP_ACTIVING);

    if (!MMIAPIPDP_Active(&app_info))
    {       
        //SCI_TRACE_LOW:"MMIWWW_NetConnectEx:MMIAPIPDP_Active fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_757_112_2_18_3_11_28_10,(uint8*)"");

        MMIWWW_SetPDPState(self, MMIWWW_PDP_ACTIVE_FAILED);
        
        return SLIM_E_GENERIC;
    }
    else
    {
        return SLIM_E_OK;
    }
}

/*****************************************************************************/
//  Description : Net Connect
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC int MMIWWW_NetConnect(MMIWWW_INSTANCE_T *self)
{
    //SCI_TRACE_LOW:"MMIWWW_NetConnect pdp_state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_775_112_2_18_3_11_28_11,(uint8*)"d",self->pdp_info.pdp_state);

    if ((MMIWWW_PDP_NONE == self->pdp_info.pdp_state)
        ||(MMIWWW_PDP_ACTIVE_FAILED == self->pdp_info.pdp_state)
        ||(MMIWWW_PDP_DEACTIVING == self->pdp_info.pdp_state)
        ||(MMIWWW_PDP_DEACTIVE_FAILED == self->pdp_info.pdp_state))
    {
        return MMIWWW_NetConnectEx(self);
    }
    else
    {
        //SCI_TRACE_LOW:"MMIWWW_NetConnect PDP Connecting now..."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_786_112_2_18_3_11_28_12,(uint8*)"");
    }

    return SLIM_E_OK;
}

/*****************************************************************************/
//  Description : Net Disconnect
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NetDisconnect(MMIWWW_INSTANCE_T *self)
{
    //SCI_TRACE_LOW:"MMIWWW_NetDisconnect directly"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_798_112_2_18_3_11_28_13,(uint8*)"");
    //SCI_TRACE_LOW:"MMIWWW_NetDisconnect is_ready = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_799_112_2_18_3_11_28_14,(uint8*)"d", MMIWWW_NetworkIsReady(self));
    /* 断开GPRS拨号 */
    if (MMIWWW_PDP_DEACTIVING != self->pdp_info.pdp_state)
    {
        MMIAPIPDP_Deactive(MMI_MODULE_WWW);
        MMIWWW_SetPDPState(self, MMIWWW_PDP_DEACTIVING);
    }

    //SCI_TRACE_LOW:"MMIWWW_NetDisconnect..."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_807_112_2_18_3_11_28_15,(uint8*)"");
}

/*****************************************************************************/
//  Description : Check Is Network Accessible
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWWW_NetworkIsAccessible(MMIWWW_INSTANCE_T *self)
{
    /* TODO: */
    return (BOOLEAN)(MMIWWW_PDP_ACTIVING == self->pdp_info.pdp_state);
}

/*****************************************************************************/
//  Description : handle www pdp active cnf
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void HandleWWWPDPActiveCnf(MMIWWW_INSTANCE_T *www_instance, MMIPDP_CNF_INFO_T *msg_ptr)
{
    if (PNULL == www_instance || PNULL == msg_ptr)
    {
        return;
    }

    //SCI_TRACE_LOW:"HandleWWWPDPActiveCnf() msg_ptr->result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_831_112_2_18_3_11_28_16,(uint8*)"d", msg_ptr->result);

    if (0 != www_instance->browser_instance)
    {
        MMIWWW_Netfront_Wakeup(www_instance->browser_instance);
    }

    if (MMIPDP_RESULT_SUCC == msg_ptr->result)
    {
        www_instance->pdp_info.net_id = msg_ptr->nsapi;
        MMIWWW_SetPDPState(www_instance, MMIWWW_PDP_ACTIVE_OK);
    }           
    else 
    {
        MMIWWW_SetPDPState(www_instance, MMIWWW_PDP_ACTIVE_FAILED);
    }
}

/*****************************************************************************/
//  Description : handle www pdp deactive cnf
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void HandleWWWPDPDeactiveCnf(MMIWWW_INSTANCE_T *www_instance, MMIPDP_CNF_INFO_T *msg_ptr)
{
    if (PNULL == www_instance)
    {
        return;
    }

    //SCI_TRACE_LOW:"HandleWWWPDPDeactiveCnf() msg_ptr->result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_860_112_2_18_3_11_28_17,(uint8*)"d", msg_ptr->result);

#if 0 //CNF is the confirmation of the last www_instance's disconnection ,do Not change the current www_instance's PDP state
    if (MMIPDP_RESULT_SUCC == msg_ptr->result)
    {
        MMIWWW_SetPDPState(www_instance, MMIWWW_PDP_DEACTIVE_OK);
    }           
    else 
    {
        MMIWWW_SetPDPState(www_instance, MMIWWW_PDP_DEACTIVE_FAILED);
    }
#endif
}

/*****************************************************************************/
//  Description : handle www pdp deactive Ind
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void HandleWWWPDPDeactiveInd(MMIWWW_INSTANCE_T *www_instance, MMIPDP_CNF_INFO_T *msg_ptr)
{
    if (PNULL == www_instance)
    {
        return;
    }

    //SCI_TRACE_LOW:"HandleWWWPDPDeactiveInd() msg_ptr->result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_885_112_2_18_3_11_28_18,(uint8*)"d", msg_ptr->result);

    if (MMIPDP_RESULT_SUCC == msg_ptr->result)
    {
        MMIWWW_SetPDPState(www_instance, MMIWWW_PDP_DEACTIVE_OK);
    }           
    else 
    {
        MMIWWW_SetPDPState(www_instance, MMIWWW_PDP_DEACTIVE_FAILED);
    }
}

/*****************************************************************************/
// 	Description : handle www ps msg
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWWWPsMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    WWW_APPLET_T *www_applet_ptr = MMIWWW_APPLETINSTANCE();
    MMIWWW_INSTANCE_T *www_instance = PNULL;

    if (PNULL == www_applet_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    www_instance = &www_applet_ptr->www_instance;

    switch (msg_id)
    {
    case SOCKET_ASYNC_GETHOSTBYNAME_CNF:
        //SCI_TRACE_LOW:"HandleWWWPsMsg receive SOCKET_ASYNC_GETHOSTBYNAME_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_917_112_2_18_3_11_28_19,(uint8*)"");
        result = MMIWWW_HandleDNSCnf(www_instance, param);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}

/*****************************************************************************/
// 	Description : process pdp signal
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void HandleWWWPDPMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    WWW_APPLET_T *www_applet_ptr = MMIWWW_APPLETINSTANCE();
    MMIWWW_INSTANCE_T *www_instance = PNULL;

    if (PNULL == msg_ptr || PNULL == www_applet_ptr)
    {
        return;
    }

    www_instance = &www_applet_ptr->www_instance;
    
    //check the param
    //SCI_TRACE_LOW:"HandleWWWPDPMsg() enter msg_id= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_946_112_2_18_3_11_28_20,(uint8*)"d", msg_ptr->msg_id);
    
    switch(msg_ptr->msg_id)
    {
    case MMIPDP_ACTIVE_CNF:
        {
            //SCI_TRACE_LOW:"HandleWWWPDPMsg: MMIPDP_ACTIVE_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_952_112_2_18_3_11_28_21,(uint8*)"");

            HandleWWWPDPActiveCnf(www_instance, msg_ptr);
        }
        break;
        
    case MMIPDP_DEACTIVE_CNF:
        {
            //SCI_TRACE_LOW:"HandleWWWPDPMsg: MMIPDP_DEACTIVE_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_960_112_2_18_3_11_28_22,(uint8*)"");

            HandleWWWPDPDeactiveCnf(www_instance, msg_ptr);
        }
        break;
        
    case MMIPDP_DEACTIVE_IND:
        {
            //SCI_TRACE_LOW:"HandleWWWPDPMsg: MMIPDP_DEACTIVE_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_968_112_2_18_3_11_28_23,(uint8*)"");

            HandleWWWPDPDeactiveInd(www_instance, msg_ptr);
        }
        break;

    default:
        break;
    }

    //SCI_FREE(msg_ptr);
}

/*****************************************************************************/
//  Description : register mmiwww module nv len and max item
//  Global resource dependence : none
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWWW_InitModule( void )
{
    MMIWWW_RegWinIdNameArr();    
    MMIWWW_RegNv();
    MMIWWW_InitConnectionSetting();
    RegAppletInfo();
}

/*****************************************************************************/
//  Description : handle www timer id
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWWW_HandleWWWTimer(uint8 timer_id, uint32 param)
{
    slim_int i = 0;
    NFBrowserTimer_TimerProc in_proc = PNULL;
    slim_opaque in_arg = PNULL;
    WWW_APPLET_T *www_applet_ptr = MMIWWW_APPLETINSTANCE();
    MMIWWW_INSTANCE_T *www_instance = PNULL;

    if (PNULL == www_applet_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPIWWW_HandleWWWTimer:FALSE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_1007_112_2_18_3_11_28_24,(uint8*)"");
        return;
    }

    www_instance = &www_applet_ptr->www_instance;
    
    for (i = 0; i < MMIWWW_MAX_TIMER_NUM; i ++)
    {
        if (timer_id == www_instance->timer_proc[i].timer_id)
        {
            in_proc = www_instance->timer_proc[i].in_proc;
            in_arg = www_instance->timer_proc[i].in_arg;

            www_instance->timer_proc[i].timer_id = 0;
            www_instance->timer_proc[i].in_proc = PNULL;
            www_instance->timer_proc[i].in_arg = PNULL;
            break;
        }
    }

    if (PNULL != in_proc)
    {
        in_proc(in_arg);
        //SCI_TRACE_LOW:"MMIAPIWWW_HandleWWWTimer:TRUE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_1030_112_2_18_3_11_29_25,(uint8*)"");
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPIWWW_HandleWWWTimer:FALSE 22"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWWW_1034_112_2_18_3_11_29_26,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : init browser
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWWW_UninitApp(void)
{
    MMIWWW_DestroyMemory();
}

/*****************************************************************************/
//  Description : check if browser is inited
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWWW_IsRunning(void)
{
    WWW_APPLET_T *www_applet_ptr = MMIWWW_APPLETINSTANCE();

    return (BOOLEAN)(PNULL != www_applet_ptr);
}

/*****************************************************************************/
//  Description : check if browser is the focused win
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWWW_IsActive(void)
{
    WWW_APPLET_T *www_applet_ptr = MMIWWW_APPLETINSTANCE();
    MMI_HANDLE_T applet_handle = PNULL;
    MMI_WIN_ID_T win_id = MMIWWW_WIN_ID_START;

    if (PNULL != www_applet_ptr )
    {
        applet_handle = MMIWWW_HANDLE(www_applet_ptr);
        if (PNULL != applet_handle)
        {
            for (win_id = MMIWWW_WIN_ID_START + 1; win_id <  MMIWWW_MAX_WIN_ID; win_id++)
            {
                if (MMK_IsFocusWin(MMK_GetWinHandle(applet_handle, win_id)))
                {
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : the entry of browser 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIAPIWWW_Entry(const MMIBROWSER_ENTRY_PARAM * entry_param_ptr)
{
    if (PNULL == entry_param_ptr)
    {
        return MMIBROWSER_ERROR_INPUT;
    }

    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return MMIBROWSER_FAIL;
    }

    if (MMIAPIMTV_IsMTVRunning())//cmmb
    {
        MMIPUB_OpenAlertWarningWin(TXT_EXIT_MOBILETV);
        return MMIBROWSER_FAIL;
    }

    MMIAPIWWW_StartWWW(entry_param_ptr);

    return MMIBROWSER_SUCC;
}

/*****************************************************************************/
//  Description : close browser 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIAPIWWW_Close(void)
{
    MMIAPIWWW_StopWWW();

    return MMIBROWSER_SUCC;
}


/*****************************************************************************/
//  Description : set net setting to default
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWWW_ResetFactorySetting(void)
{
    MMIWWW_ResetDefaultSettings();
}

/*****************************************************************************/
//  Description : reg mime func
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWWW_RegMimeFunc(MMIBROWSER_MIME_DOWNLOAD_PARAM_T* param_ptr)
{
#ifdef DL_SUPPORT
    MMIDL_MIME_DOWNLOAD_PARAM_T  mime_download_param = {0};

    if (PNULL == param_ptr)
    {
        return FALSE;
    }

    mime_download_param.mime_type = param_ptr->mime_type;
    mime_download_param.call_back = (MMIDL_MIME_CALLBACK)param_ptr->call_back;

    return MMIAPIDL_RegMimeCallback(&mime_download_param);
#else
    return FALSE;
#endif
}

/*****************************************************************************/
// Description : Send message to www applet
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_SendMsgToWWWApplet(MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_HANDLE_T applet_handle = PNULL;
    applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());

    MMK_SendMsg(applet_handle, msg_id, param);
}

/*****************************************************************************/
// Description : Post message to www applet
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_PostMsgToWWWApplet(MMI_MESSAGE_ID_E msg_id, DPARAM param, uint32 size_of_param)
{
    MMI_HANDLE_T applet_handle = PNULL;
    applet_handle = MMIWWW_HANDLE(MMIWWW_APPLETINSTANCE());

    MMK_PostMsg(applet_handle, msg_id, param, size_of_param);
}

#if 0
/*****************************************************************************/
//  Description : Set browser app active 
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_SetBrowserActive(void)
{
    WWW_APPLET_T* www_applet_ptr = PNULL;
    MMI_HANDLE_T applet_handle = PNULL;

    www_applet_ptr = MMIWWW_APPLETINSTANCE();
    if (PNULL != www_applet_ptr)
    {
        applet_handle = MMIWWW_HANDLE(www_applet_ptr);
        if (PNULL != applet_handle)
        {
            MMK_GrabWinFocusByApplet(applet_handle);
        }
    }
    else
    {
        MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/

        entry_param.type = MMIBROWSER_ACCESS_MAINMENU;
        entry_param.dual_sys = MN_DUAL_SYS_MAX;
        entry_param.url_ptr = PNULL;
        entry_param.user_agent_ptr = PNULL;

        MMIAPIBROWSER_Entry(&entry_param);
    }
    return TRUE;
}
#endif

/*****************************************************************************/
//  Description : Get browser Version 
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC char* MMIAPIWWW_GetVersion(void)
{
    return (char*)NFBrowser_GetVersion();/*lint !e746*/
}

/*****************************************************************************/
//  Description : Delete completed download task
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWWW_DeleteCompletedDownloadTask(uint32 task_index)
{
#ifdef DL_SUPPORT
    MMIAPIDL_DeleteCompletedDownloadTask(task_index);
#endif
}

/*****************************************************************************/
//  Description : Clean cahe
//  Global resource dependence : 
//  Author: minghu.mao
//  Note: 清除浏览器和http的cache
/*****************************************************************************/
PUBLIC void MMIWWW_CleanCache(uint8 * url_ptr, uint16 url_len)
{
    return;
}

/*****************************************************************************/
//  Description : Get Browser Instance
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC slim_int MMIWWW_GetBrowser(void)
{
    MMIWWW_INSTANCE_T * www_instance = MMIWWW_INSTANCE();

    if(PNULL == www_instance)
    {
        return 0;
    }

    return www_instance->browser_instance;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    Start of Atest  Function                                                                */
/* Description:  Hanlde at cmd for browser auto test cases                                                     */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

//need to do

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    end of Atest  Function                                                                 */
/* Description:  Hanlde at cmd for browser auto test cases                                                     */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/



#endif //#ifdef BROWSER_SUPPORT_NETFRONT
