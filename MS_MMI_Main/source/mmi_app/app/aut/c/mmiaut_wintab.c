/****************************************************************************
** File Name:      mmiaut_wintab.c
** Author:                                                                  
** Date:           2012/01/20
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is mmi nettime wintab file.AUT is short for auto update time
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 2012/01/20        dave.ruan             Create
** 
****************************************************************************/

#include "mmi_app_aut_trc.h"
#ifdef  MMI_UPDATE_TIME_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmipub.h"
#if !defined(MMI_MULTI_SIM_SYS_SINGLE) && defined(MMI_SNTP_SUPPORT) 
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#endif
#include "mmiaut_export.h"
#include "mmiaut_internal.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN   s_is_need_auto_update_time = FALSE;   //remember is need auto update time flag temply,when manual updatetime
#ifdef MMI_WIFI_SUPPORT
LOCAL BOOLEAN s_aut_is_use_wifi = FALSE;
#endif
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         LOCAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 等待窗口处理
//	Global resource dependence : 
//  Author:dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleManualUpdateSystemTimeWaitWin(
                                                 MMI_WIN_ID_T win_id,
                                                 MMI_MESSAGE_ID_E msg_id,
                                                 DPARAM param
                                                 );

#ifdef MMI_SNTP_SUPPORT
/*****************************************************************************/
// 	Description : Handle Deactive PDP Prompt WinMsg
//	Global resource dependence : 
//  Author: dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeactivePDPPromptWinMsg(
                                            MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param
                                            );


#if !defined(MMI_MULTI_SIM_SYS_SINGLE) 
/*****************************************************************************/
//  Description : append item to listbox for WIFI or SIM card select
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL void AutAppendSelNetworkListItem(MMI_CTRL_ID_T ctrl_id,uint8 item_num,MMICONNECTION_SETTING_DETAIL_ITEM_T* detail_item);

/*****************************************************************************/
// 	Description : 选择可用的直连账户
//	Global resource dependence : 
//    Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAutoUpdateTimeSelAccountWindow(
                                                     MMI_WIN_ID_T    win_id, 
                                                     MMI_MESSAGE_ID_E   msg_id, 
                                                     DPARAM             param
                                               );

WINDOW_TABLE(MMISET_AUTO_UPDATE_TIME_SELECT_ACCOUNT_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleAutoUpdateTimeSelAccountWindow),
        WIN_ID(MMISET_UPDATE_TIME_SEL_ITEM_WIN_ID),
        WIN_TITLE(TXT_SIM_SEL),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_AUTO_UPDATE_TIM_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
#endif

/*****************************************************************************/
// 	Description : Handle Deactive PDP Prompt WinMsg
//	Global resource dependence : 
//  Author: dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeactivePDPPromptWinMsg(
                                            MMI_WIN_ID_T     win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM           param
                                            )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
            {
                MMIPUB_INFO_T *win_info_ptr = PNULL;
                MMICONNECTION_SETTING_DETAIL_ITEM_T* detail_item_data_ptr = PNULL;
                MMICONNECTION_SETTING_DETAIL_ITEM_T item_detail = {0};
                
                win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
                if (PNULL != win_info_ptr)
                {
                    SCI_MEMSET(&item_detail,0x00,sizeof(MMICONNECTION_SETTING_DETAIL_ITEM_T));
                    detail_item_data_ptr = (MMICONNECTION_SETTING_DETAIL_ITEM_T *)win_info_ptr->user_data;

                    //set sntp link account detail,and use it after dactive pdp successfully
                    item_detail.dual_sys = detail_item_data_ptr->dual_sys;
                    item_detail.item_index = detail_item_data_ptr->item_index;
                    MMIAUT_SetSntpLinkDetailItem(item_detail);
                }

                MMIAPIPDP_ReleaseAll(MMIAUT_ReleaseAllPDPCallback);
                MMIAUT_OpenManualUpdateWaitWin();
                MMK_CloseWin(win_id);
            }
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        case MSG_CLOSE_WINDOW:
            {
                MMIPUB_INFO_T *win_info_ptr = PNULL;
                MMICONNECTION_SETTING_DETAIL_ITEM_T* detail_item_data_ptr = PNULL;

                win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
                if (PNULL != win_info_ptr)
                {
                    detail_item_data_ptr = (MMICONNECTION_SETTING_DETAIL_ITEM_T *)win_info_ptr->user_data;
                    SCI_FREE(detail_item_data_ptr);
                }
            }
            break;

        default:
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;
    }

    return (result);    
}

#if !defined(MMI_MULTI_SIM_SYS_SINGLE) 
/*****************************************************************************/
//  Description : append item to listbox for WIFI or SIM card select
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
LOCAL void AutAppendSelNetworkListItem(MMI_CTRL_ID_T ctrl_id,uint8 item_num,MMICONNECTION_SETTING_DETAIL_ITEM_T* detail_item)
{

    GUILIST_SetMaxItem(ctrl_id, MMI_DUAL_SYS_MAX, FALSE );
    
    if(0 == item_num)
    {
        MMIAPISET_AppendMultiSimName(ctrl_id, TXT_COMMON_OK, MMI_DUAL_SYS_MAX, MMISET_APPEND_SYS_OK);
    }
    else
    {
        uint8 index = 0;
        MMISET_SIM_NAME_T   sim_str     = {0};
        wchar opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN] = {0};
        MMI_STRING_T network_str = {0};
        MMI_IMAGE_ID_T sim_img_id[MMI_DUAL_SYS_MAX] ={
#ifdef MMI_MULTI_SIM_SYS_SINGLE
            IMAGE_IDLE_PROMPT_SIM
#else
            IMAGE_IDLE_PROMPT_SIM1,
            IMAGE_IDLE_PROMPT_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_PROMPT_SIM3,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_PROMPT_SIM4
#endif
#endif
        };
        MMI_IMAGE_ID_T no_sim_img_id[MMI_DUAL_SYS_MAX] = {
#ifdef MMI_MULTI_SIM_SYS_SINGLE
            IMAGE_IDLE_PROMPT_NO_SIM
#else
            IMAGE_IDLE_PROMPT_NO_SIM1,
            IMAGE_IDLE_PROMPT_NO_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_PROMPT_NO_SIM3,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_PROMPT_NO_SIM4
#endif
#endif
        };
        MMI_IMAGE_ID_T gray_sim_img_id[MMI_DUAL_SYS_MAX] = {
#ifdef MMI_MULTI_SIM_SYS_SINGLE
            IMAGE_IDLE_PROMPT_GRAY_SIM
#else
            IMAGE_IDLE_PROMPT_GRAY_SIM1,
            IMAGE_IDLE_PROMPT_GRAY_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_PROMPT_GRAY_SIM3,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            IMAGE_IDLE_PROMPT_GRAY_SIM4
#endif
#endif
        };
        MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
        MN_DUAL_SYS_E pre_dual_sys = MMI_DUAL_SYS_MAX;//remember the SIM index
        
        for(index =0; index < item_num; index++)
        {
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};
            MMI_IMAGE_ID_T image_id = IMAGE_NULL;
            
            if(detail_item[index].dual_sys == pre_dual_sys)
            {
                //the same card only append once
                continue;
            }
            else
            {
                pre_dual_sys =detail_item[index].dual_sys;
            }
            
            item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT;
            item_t.item_data_ptr = &item_data;
            item_t.user_data = index;//indicate which item  is appended !!
            if (e_dualsys_setting & (0x01 << detail_item[index].dual_sys))
            {
                if (MMIAPIPHONE_GetSimExistedStatus(detail_item[index].dual_sys))
                {
                    image_id = sim_img_id[detail_item[index].dual_sys];
                }
                else
                {
                    image_id = no_sim_img_id[detail_item[index].dual_sys];
                }
            }
            else
            {
                if (MMIAPIPHONE_GetSimExistedStatus(detail_item[index].dual_sys))
                {
                    image_id = gray_sim_img_id[detail_item[index].dual_sys];
                }
                else
                {
                    image_id = no_sim_img_id[detail_item[index].dual_sys];
                }
            }

            item_data.item_content[AUT_LIST_ITEM_SIM_ICON_INDEX].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[AUT_LIST_ITEM_SIM_ICON_INDEX].item_data.image_id = image_id;

            sim_str = MMIAPISET_GetSimName(detail_item[index].dual_sys);

            //display SIM Card Display string!!
            item_data.item_content[AUT_LIST_ITEM_SIM_ID_INDEX].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[AUT_LIST_ITEM_SIM_ID_INDEX].item_data.text_buffer.wstr_len = sim_str.wstr_len;
            
            item_data.item_content[AUT_LIST_ITEM_SIM_ID_INDEX].item_data.text_buffer.wstr_ptr = sim_str.wstr_arr;

            //get opn_spn information for display in sim selection page!!
            SCI_MEMSET(&network_str, 0x00, sizeof(network_str));
            network_str.wstr_ptr = (wchar *)opn_spn_name;
            SCI_MEMSET(network_str.wstr_ptr, 0x00, sizeof(opn_spn_name));
            
            MMIAPISET_GetSimServiceString((MN_DUAL_SYS_E)detail_item[index].dual_sys, &network_str);
    
            item_data.item_content[AUT_LIST_ITEM_SIM_SERVICE_NAME_INDEX].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[AUT_LIST_ITEM_SIM_SERVICE_NAME_INDEX].item_data.text_buffer.wstr_len = network_str.wstr_len;
            item_data.item_content[AUT_LIST_ITEM_SIM_SERVICE_NAME_INDEX].item_data.text_buffer.wstr_ptr = network_str.wstr_ptr;

            //set softkey
            item_data.softkey_id[0] = TXT_COMMON_OK;
            item_data.softkey_id[1] = TXT_NULL;
            item_data.softkey_id[2] = STXT_RETURN;
            
            GUILIST_AppendItem(ctrl_id, &item_t);      
        }
    }
#if 0 //def WIFI_SUPPORT
    SCI_MEMSET(&item_t, 0x00, sizeof(item_t));
    SCI_MEMSET(&item_data, 0x00, sizeof(item_data));

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr = &item_data;
    item_t.user_data = MMI_DUAL_SYS_MAX;//WIFI
    #ifdef WLAN_SUPPORT
        MMI_GetLabelTextByLang(TXT_WIFI,&sim_str);
    #else
        MMI_GetLabelTextByLang(TXT_WIFI,&sim_str);
    #endif
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = MAIN_GetIdleWifiIcon();
                                 

    item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer.wstr_len = sim_str.wstr_len;
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = sim_str.wstr_ptr;

    //set softkey
    item_data.softkey_id[0] = TXT_COMMON_OK;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;
    
    GUILIST_AppendItem(ctrl_id, &item_t);
#endif

    GUILIST_SetOwnSofterKey(ctrl_id, TRUE);
}

/*****************************************************************************/
// 	Description : 选择可用的直连账户
//	Global resource dependence : 
//    Author:dave.ruan
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAutoUpdateTimeSelAccountWindow(
                                                     MMI_WIN_ID_T    win_id, 
                                                     MMI_MESSAGE_ID_E   msg_id, 
                                                     DPARAM             param
                                               )
{
    MMI_RESULT_E  recode     = MMI_RESULT_FALSE;
    MMI_CTRL_ID_T ctrl_id    = MMISET_AUTO_UPDATE_TIM_CTRL_ID;
    uint16		  list_index = 0;
    uint32 item_data =0;
    uint8   item_num = 0;
    MMICONNECTION_SETTING_DETAIL_ITEM_T detail_item[MMIAUT_MAX_DIRECT_LINK_SETTING_NUM]= {0}; 

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            //GUITITLE_SetTextId(win_id,MMITHEME_GetTitleCtrlId(),TXT_NETWORK_SELECT,FALSE);
#if !defined(MMI_MULTI_SIM_SYS_SINGLE) 
            MMIAPIPHONE_SetNeedUpdateNetworkStatusWin(win_id);
#endif  
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;

    case MSG_FULL_PAINT:
        {
            uint16 cur_selection = 0;

#if defined MMI_GPRS_SUPPORT
            MMIAPICONNECTION_GetNoProxyAccountIndex(&item_num,detail_item,MMIAUT_MAX_DIRECT_LINK_SETTING_NUM);
#endif
            /*if(1 >= item_num)
            {
                //SCI_TRACE_LOW:"[MMIAUT]::HandleAutoUpdateTimeSelAccountWindow item_num=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_WINTAB_370_112_2_18_2_5_25_45,(uint8*)"d",item_num);
                return recode;
            }*/
            AutAppendSelNetworkListItem(ctrl_id,item_num, detail_item);
            GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
        }
        break;

#if !defined(MMI_MULTI_SIM_SYS_SINGLE) 
    case MSG_GET_FOCUS:
        MMIAPIPHONE_SetNeedUpdateNetworkStatusWin(win_id);
        break;
#endif          
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        {           
            list_index = GUILIST_GetCurItemIndex(ctrl_id);
            
            GUILIST_GetItemData(ctrl_id, list_index, &item_data);
            
            if(list_index < MMI_DUAL_SYS_MAX)//item_data may > MMI_DUAL_SYS_MAX if wifi on
            {
#if defined MMI_GPRS_SUPPORT
                MMIAPICONNECTION_GetNoProxyAccountIndex(&item_num,detail_item,MMIAUT_MAX_DIRECT_LINK_SETTING_NUM);
#endif
                //SCI_TRACE_LOW:"[MMIAUT]::HandleAutoUpdateTimeSelAccountWindow item_num=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_WINTAB_397_112_2_18_2_5_25_46,(uint8*)"d",item_num);
                if(0 == item_num)
                {
                    //set detail_item 0 as the PDP account To get NITZ
                    detail_item[0].dual_sys = (MN_DUAL_SYS_E)list_index;
#if defined BROWSER_SUPPORT
                    detail_item[0].item_index = MMIAPIBROWSER_GetNetSettingIndex((MN_DUAL_SYS_E)list_index);
#endif
                    MMIAUT_ActivePDPLink(detail_item[0]);
                    return recode;
                }

                //在选卡界面直接进入联网流程，不关注是否有PDP 连接
                //if other card PDP link exist, deal with MMIPDP_RESULT_DIFFER_DUALSYS msg
                //if(!MMIAPIPDP_PsIsPdpLinkExist())
                {
                    //set detail_item 0 to active pdp
                    MMIAUT_OpenManualUpdateWaitWin();
#ifdef WIN32
                    MMIAUT_SetSntpLinkDetailItem(detail_item[item_data]);
                    MMIAUT_GetSNTPTime(0);//s_sntp_server_addr_index = MAX_SNTP_SERVER_ADDR-1
#else
                    MMIAUT_ActivePDPLink(detail_item[item_data]);
#endif                
                }
#if 0                //remove 
                else //need release all PDP links
                {
                    MMICONNECTION_SETTING_DETAIL_ITEM_T* select_detail_item = SCI_ALLOC_APPZ(sizeof(MMICONNECTION_SETTING_DETAIL_ITEM_T));

                    if(PNULL != select_detail_item)
                    {
                        select_detail_item->dual_sys = detail_item[item_data].dual_sys;
                        select_detail_item->item_index = detail_item[item_data].item_index;
                        
                        MMIPUB_OpenQueryWinByTextIdEx(VIRTUAL_WIN_ID,TXT_AUT_DEACTIVE_ALL_PDP_LINK_QUERY,IMAGE_PUBWIN_QUERY,PNULL,HandleDeactivePDPPromptWinMsg,(uint32)select_detail_item);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_MEMORY_FULL);
                    }
                }
#endif                
            }
            MMK_CloseWin(win_id);            
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;
        
    default:		
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif/*MMI_MULTI_SIM_SYS_SINGLE */
#endif/*MMI_SNTP_SUPPORT*/
/*****************************************************************************/
// 	Description : 开始手动更新时间的接口
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUT_StartManualUpdateSystemTime(void)
{
    BOOLEAN        result      = TRUE;
#ifdef MMI_SNTP_SUPPORT
    uint8  item_num = 0;
    MMICONNECTION_SETTING_DETAIL_ITEM_T detail_item[MMIAUT_MAX_DIRECT_LINK_SETTING_NUM]= {0}; 

    //get NITZ time with tickcount
    if(MMIAPIPHONE_SetSysTimeWithNitzAndTickTime())
    {
        //popup success notification even if set_value is not true
        MMIAPISET_ShowUpdateTimeMsgWin(TRUE,TRUE);
    }
    else
    {
        //no PDP link exist
        if(!MMIAPIPDP_PsIsPdpLinkExist())
        {
            uint32 sim_num = 0;
            uint16 sim_ok = MMI_DUAL_SYS_MAX;

            sim_num = MMIAPIPHONE_GetSimOkNum(&sim_ok, 1);

            if(0 == sim_num)
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
                return FALSE;
            }

            //if donot  have PDP link exist, search all direct links and get useful account num item
#if defined MMI_GPRS_SUPPORT
            MMIAPICONNECTION_GetNoProxyAccountIndex(&item_num,detail_item,MMIAUT_MAX_DIRECT_LINK_SETTING_NUM);
#endif
#if !defined(MMI_MULTI_SIM_SYS_SINGLE) 
            if(1 == item_num ||1 ==sim_num)
#endif
            {
                if(/*!MMIAPICONNECTION_isGPRSNeedAutoAttach() ||*/(1 == item_num))
                {
                    MMIAUT_OpenManualUpdateWaitWin();
#ifdef WIN32
                    MMIAUT_SetSntpLinkDetailItem(detail_item[0]);
                    MMIAUT_GetSNTPTime(0);//s_sntp_server_addr_index = MAX_SNTP_SERVER_ADDR-1
#else
		            //set detail_item 0 to active pdp
                    if(1 == sim_num)
                    {
                        detail_item[0].dual_sys = (MN_DUAL_SYS_E)sim_ok;
                    }
                    MMIAUT_ActivePDPLink(detail_item[0]);
#endif            
                }
                else if(item_num == 0)//SIM num =1, item num = 0
                {
                    MMIPUB_OpenAlertWarningWin(TXT_AUT_CURRENT_SERVICE_NO_TIME);
                    // deactive PDP is fail when PDP active success
                    MMIAUT_DeactivePDPLink();
                }
                else
                {
                    MMIAUT_OpenManualUpdateWaitWin();
#ifdef WIN32
                    MMIAUT_SetSntpLinkDetailItem(detail_item[1]);
                    MMIAUT_GetSNTPTime(0);//s_sntp_server_addr_index = MAX_SNTP_SERVER_ADDR-1
#else
                    if(1 == sim_num)
                    {
                        detail_item[1].dual_sys = (MN_DUAL_SYS_E)sim_ok;
                    }
                    MMIAUT_ActivePDPLink(detail_item[1]);
#endif
                }
            }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE) 
            else    //item_num >1 ||item =0
            {
                //open nework select win
                MMK_CreateWin((uint32*)MMISET_AUTO_UPDATE_TIME_SELECT_ACCOUNT_WIN_TAB,PNULL);
            }
#endif        
        }
        else      //if PDP link exist,first serach the SIM card that PDP link exist
        {
            MN_DUAL_SYS_E dual_sys = MMIAPIPDP_GetPdpLinkSIM();
            uint32 sim_num = 0;
            uint16 sim_ok = MMI_DUAL_SYS_MAX;
            
            sim_num = MMIAPIPHONE_GetSimOkNum(&sim_ok, 1);

            if(0 == sim_num)
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
                return FALSE;
            }
            
#if defined MMI_GPRS_SUPPORT
            MMIAPICONNECTION_GetCurSIMNoProxyAccountIndex(dual_sys, &item_num, detail_item,MMIAUT_MAX_DIRECT_LINK_SETTING_NUM);
#endif
            
#if !defined(MMI_MULTI_SIM_SYS_SINGLE) 
            if(0 <  item_num ||1 ==sim_num)
#endif
            {
                //if aut module PDP is not actived
                if(MMIPDP_LINK_STATE_DEACTIVED == MMIAPIPDP_GetLinkState(MMIAUT_GetModuleID())
                ||MMIPDP_LINK_STATE_MAX == MMIAPIPDP_GetLinkState(MMIAUT_GetModuleID()))
                {
                    MMIAUT_OpenManualUpdateWaitWin();
                    MMIAUT_ActivePDPLink(detail_item[0]);
                }
                else
                {
                    //PDP module is busy,wait for PDP deactived and try later
                    MMIPUB_OpenAlertWarningWin(TXT_SYS_WAITING_AND_TRY_LATER);
                    //deactive PDP
                    MMIAUT_DeactivePDPLink();
                }
            }
#if !defined(MMI_MULTI_SIM_SYS_SINGLE) 
            else    //item_num = 0, no direct account, enter SELECT win
            {
                //enter select SIM win, select SIM first, and deactive PDP
                MMK_CreateWin((uint32*)MMISET_AUTO_UPDATE_TIME_SELECT_ACCOUNT_WIN_TAB,PNULL);
            }        
#endif
        }
    }
#endif    
    
    return result;
}

/*****************************************************************************/
// 	Description : 等待窗口处理
//	Global resource dependence : 
//  Author:dave.ruan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleManualUpdateSystemTimeWaitWin(
                                                 MMI_WIN_ID_T win_id,
                                                 MMI_MESSAGE_ID_E msg_id,
                                                 DPARAM param
                                                 )
{
    MMI_RESULT_E        recode    = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        {
            /*用户手动取消后，相关变量reset，不弹出提示界面*/
            MMIAUT_SetUserCancelFlag(TRUE);

            /*关闭等待窗口*/
            MMIAPIAUT_CloseManualUpdateWaitWin();
            
            MMIAPIAUT_SetIsNeedUpdateTimeByNet(s_is_need_auto_update_time);

            //deactive PDP
            MMIAUT_DeactivePDPLink();
        }
        break;
        
    case MSG_LOSE_FOCUS:
    case MSG_GET_FOCUS:
        break;
        
    case MSG_CLOSE_WINDOW:
        {
            MMIAUT_SetManualUpdateTimeRunning(FALSE);

            //deactive PDP
            MMIAUT_DeactivePDPLink();
        }
        break;

    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : 打开等待窗口
//  Global resource dependence : 
//  Author:dave.ruan 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAUT_OpenManualUpdateWaitWin(void)
{
    MMI_STRING_T          str = {0};
    MMI_STRING_T          right_softkey_txt_str = {0};
    MMI_WIN_ID_T          wait_win_id          = MMIAUT_GetManualUpdateWaitWinID();
    
    if (!MMK_IsOpenWin(wait_win_id))
    {
        /*记录手动更新前update_time flag的状态，当用户取消更新时，需要恢复该状态*/
        s_is_need_auto_update_time = MMIAPIAUT_GetIsNeedUpdateTimeByNet();
        MMIAPIAUT_SetIsNeedUpdateTimeByNet(TRUE);
        MMIAUT_SetManualUpdateTimeRunning(TRUE);
        //SCI_TRACE_LOW:"[MMIAUT]: OpenManualUpdateWaitWin deactive pdp s_is_need_auto_update_time= %d,MMIAPIAUT_GetIsNeedUpdateTimeByNet =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIAUT_WINTAB_613_112_2_18_2_5_25_47,(uint8*)"dd", s_is_need_auto_update_time,MMIAPIAUT_GetIsNeedUpdateTimeByNet());
                
        /*弹出等待窗口*/
        MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &str);
        MMI_GetLabelTextByLang(STXT_CANCEL, &right_softkey_txt_str);
        MMIPUB_OpenWaitWin(1, &str, PNULL, &right_softkey_txt_str, wait_win_id, IMAGE_NULL,ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleManualUpdateSystemTimeWaitWin);
    }
}

/*****************************************************************************/
// 	Description : Is Need Auto Update Time
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAUT_GetIsNeedAutoUpdateTime(void)
{
    return s_is_need_auto_update_time;
}

/*****************************************************************************/
// 	Description : Set Is Need Auto Update Time
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIAUT_SetIsNeedAutoUpdateTime(BOOLEAN is_need_update)
{
    s_is_need_auto_update_time = is_need_update;
}

/*****************************************************************************/
//  Description : 获取等待窗口的id
//  Global resource dependence : 
//  Author:dave.ruan
//  Note: 
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAUT_GetManualUpdateWaitWinID(void)
{
    MMI_WIN_ID_T wait_win_id  = MMISET_UPDATE_SYSTEM_TIME_WAITING_WIN_ID;

    return wait_win_id;
}
#endif /*MMI_UPDATE_TIME_SUPPORT*/


/*Edit by script, ignore 2 case. Fri Apr 27 09:38:48 2012*/ //IGNORE9527
