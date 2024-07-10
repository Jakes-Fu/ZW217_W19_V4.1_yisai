/****************************************************************************
** File Name:     mmibt_devicelist.c                                        *
** Author:         yuantao.xu                                            *
** Date:           7/11/2012                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the bluetooth device paired*
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 7/11/2012    yuantao.xu           Create                                  *
**                                                                         *
****************************************************************************/
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                     *
**---------------------------------------------------------------------------*/
#define _MMIBT_DEVICELIST_C_
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                     *
**---------------------------------------------------------------------------*/
#include "mmi_app_bt_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_timer.h"
#include "mmibt_func.h"
#include "mmipub.h"
#include "guilabel.h"
#include "mmifmm_export.h"   //bt_abs.h中用到ffs.h中的结构
#include "bt_abs.h"
#include "bt_cfg.h"
#include "mmibt_text.h"
#include "mmi_default.h"
#include "guitext.h"
#include "mmivcard_export.h"
#include "mmieng_export.h"
#include "mmicl_export.h"
#include "mmiphone_export.h"
#include "mmibt_a2dp.h"
#include "mmiudisk_export.h"
#include "mmisd_export.h"
#include "mmibt_nv.h"
#include "mmi_mainmenu_export.h"
#include "mmifmm_text.h"
#include "guimenu.h"
#include "guires.h"
#include "guiedit.h"
#include "mmiidle_export.h"
#include "mmibt_devicelist.h"
#include "mmibt_headset.h"
#include "mmi_appmsg.h"
#ifdef BLUETOOTH_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef struct mmibt_device_list_param_tag
{
    MMIBT_TRANSFER_FILE_INFO_T    transfer_file_info;    
    MMIBT_OPERTATION_TYPE_E     root_opertion;
    uint32              current_service;
}MMIBT_DEVICE_LIST_PARAM_T;
/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription: Handle blue tooth device list win msg  
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E      HandleDeviceListWinMsg(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param 
                                               );
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void DeviceList_CreateList(
                    MMI_WIN_ID_T             win_id,                   
                    MMI_CTRL_ID_T            ctrl_id, 
                    uint32                   service
                    );
/*****************************************************************************/
//  Discription: Open the blue tooth device list  win   
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenDeviceListWin(MMIBT_DEVICE_LIST_PARAM_T *param_ptr);
/*-----------------------------------------------------------------------------*/
//device list win table
WINDOW_TABLE(MMIBT_DEVICE_LIST_WIN_TAB) = 
{ 
    //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC((uint32) HandleDeviceListWinMsg ),    
        WIN_ID(MMIBT_DEVICE_LIST_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
        WIN_STATUSBAR,
#endif
        WIN_TITLE(TXT_BT_DEVICE_LIST),
        
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN    
        
};
/*-------------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription: This function is handle Device list win msg 
//  Global resource dependence: None
//  Author: Wenming Feng 
//  Note : 
//  Return:
/*****************************************************************************/
LOCAL MMI_RESULT_E     HandleDeviceListWinMsg(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param 
                                              )
{
    uint16                  cur_select_item  = 0;
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_STRING_T            text_str1 = {0};
    MMI_STRING_T            text_str2 = {0};
    MMI_STRING_T            s_title_str = {0};
    BT_DEVICE_INFO          current_device_info = {0};
#ifdef MMI_PDA_SUPPORT
    uint32                  current_service = 0;
    GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = PNULL;
#endif
    uint8   paired_device_index[MMIBT_MAX_DEV_ITEMS] = {0};
    MMIBT_DEVICE_LIST_PARAM_T  *win_param_ptr = (MMIBT_DEVICE_LIST_PARAM_T *)MMK_GetWinUserData(win_id);
    
    SCI_TRACE_LOW("[MMIBT] HandleDeviceListWinMsg msg_id =0x%x",msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:

#ifdef MMI_PDA_SUPPORT
        if (BT_GetPairedDeviceCount(MMIBT_GetCurrentSevice()) > 0)
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_DELALL, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, MMIBT_DEVICELIST_LIST_CTRL_ID);
        }
#endif
        MMK_SetAtvCtrl( win_id, MMIBT_DEVICELIST_LIST_CTRL_ID);     
        break; 
    case MSG_BT_DEVICE_LIST_UPDATE:

        //Update device list
        cur_select_item = GUILIST_GetCurItemIndex(MMIBT_DEVICELIST_LIST_CTRL_ID);

        //delete device list
        MMK_DestroyDynaCtrl(MMIBT_DEVICELIST_LIST_CTRL_ID);
        //create new device list
        DeviceList_CreateList(win_id, MMIBT_DEVICELIST_LIST_CTRL_ID,  win_param_ptr->current_service);      
        GUILIST_ValidCurItemPos(MMIBT_DEVICELIST_LIST_CTRL_ID);
        //set current selected item
        if(cur_select_item <= GUILIST_GetTotalItemNum(MMIBT_DEVICELIST_LIST_CTRL_ID))
        {
           GUILIST_SetCurItemIndex(MMIBT_DEVICELIST_LIST_CTRL_ID, cur_select_item);
        }
        MMK_SetAtvCtrl( win_id, MMIBT_DEVICELIST_LIST_CTRL_ID ); 

        SCI_TRACE_LOW("[MMIBT] MSG_BT_DEVICE_LIST_UPDATE MMK_IsFocusWin(MMIBT_DEVICE_LIST_WIN_ID) = %d",  MMK_IsFocusWin(MMIBT_DEVICE_LIST_WIN_ID));
        if(MMK_IsFocusWin(MMIBT_DEVICE_LIST_WIN_ID))
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;

    case MSG_CTL_LIST_LONGOK:
        
#ifdef MMI_PDA_SUPPORT                   
        item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;
        cur_select_item = item_content_ptr->item_index; 
        if(0 != cur_select_item)
        {
            const GUILIST_ITEM_T *cur_list_ptr = {0};
            cur_list_ptr = GUILIST_GetItemPtrByIndex(MMIBT_DEVICELIST_LIST_CTRL_ID, cur_select_item);
            if(PNULL != cur_list_ptr)
            {
                s_title_str.wstr_len = cur_list_ptr->item_data_ptr->item_content[1].item_data.text_buffer.wstr_len;
                s_title_str.wstr_ptr = cur_list_ptr->item_data_ptr->item_content[1].item_data.text_buffer.wstr_ptr;
             }
            SCI_MEMSET(paired_device_index,0,sizeof(paired_device_index));
            MMIBT_GetPairedDevList(win_param_ptr->current_service, paired_device_index);
            BT_GetPairedDeviceInfo(win_param_ptr->current_service, paired_device_index[cur_select_item -2], &current_device_info);            
            MMIBT_SetCurBtInfo(&current_device_info);
            if(MMIBT_OPC_MAIN == win_param_ptr->root_opertion)
            {
                BOOLEAN                is_connect = FALSE;
                uint16                 dev_type = 0;
                MMI_MENU_GROUP_ID_T group_id = MENU_BT_OTHER_DEVICE_DETAILS;
                
                //option menu
                BT_GetDeviceType(&current_device_info, &dev_type);
                if(dev_type & BT_DEVICE_CLASS_AUDIO)
                {
                    //headset
                    is_connect = MMIBT_DeviceIsConnectedHeadset(&current_device_info.addr);                    
                    {
                        //open or close headset permit
                        if(is_connect)
                        {
                            //current headset not connected
                            MMIBT_EnterDeviceDetailsWin(&current_device_info,&s_title_str,HEADSET_DISCONNECTED);
                        }
                        else
                        {
                            //current headset already connected
                            MMIBT_EnterDeviceDetailsWin(&current_device_info,&s_title_str,HEADSET_CONNECTED);
                        }
                    }
                }
                else
                {
                	current_service = BT_GetPairedDeviceService(&current_device_info.addr);

                    MMIBT_EnterDeviceDetailsWin(&current_device_info,&s_title_str,NOT_HEADSET);                
                    if((BT_SERVICE_HANDFREE|BT_SERVICE_HANDSET|BT_SERVICE_A2DP) & current_service)
                    {
                        is_connect = MMIBT_DeviceIsConnectedHeadset(&current_device_info.addr);                    
                        {
                            //open or close headset permit
                            if(is_connect)
                            {
                                //current headset not connected
                                GUIMENU_SetItemVisible(MMIBT_DEVICE_DETAILS_MENU_CTRL_ID, group_id, ID_BT_DEVICE_CONNECT, FALSE);
                            }
                            else
                            {
                                //current headset already connected
                                GUIMENU_SetItemVisible(MMIBT_DEVICE_DETAILS_MENU_CTRL_ID, group_id, ID_BT_DEVICE_DISCONNECT, FALSE);
                            }
                        }
                    }
                    else
                    {
                        GUIMENU_SetItemVisible(MMIBT_DEVICE_DETAILS_MENU_CTRL_ID, group_id, ID_BT_DEVICE_CONNECT, FALSE);
                        GUIMENU_SetItemVisible(MMIBT_DEVICE_DETAILS_MENU_CTRL_ID, group_id, ID_BT_DEVICE_DISCONNECT, FALSE);                    
                    }
                }
            }
            else
            {
                //selected this device to do operation
                DoOperation(win_param_ptr->root_opertion,TRUE);
            }
        }
#endif
        break;
#ifdef MMI_PDA_SUPPORT
    case MSG_GET_FOCUS:
        if (BT_GetPairedDeviceCount(MMIBT_GetCurrentSevice()) > 0)
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }
        break;

    case MSG_NOTIFY_TITLE_MENU_ICON:
        {
            MMI_STRING_T wait_text = {0};
            //blue tooth is power on
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_DEVICE_DELETE_ALL);
            //if(BT_GetState())
            {
                MMI_GetLabelTextByLang(TXT_DELETE_ALL_QUERY, &wait_text);
                OpenQueryWin(MMI_BT_QUERY_WIN_ID, &wait_text, PNULL, MMIBT_OPC_DEVICE_DELETE_ALL);
            }
        }
        break;

    case MSG_APP_MENU:
        cur_select_item = GUILIST_GetCurItemIndex(MMIBT_DEVICELIST_LIST_CTRL_ID);
        SCI_MEMSET(paired_device_index,0,sizeof(paired_device_index));
        MMIBT_GetPairedDevList(win_param_ptr->current_service, paired_device_index);
        SCI_MEMSET(&current_device_info, 0x00, sizeof(current_device_info));
#ifdef MMI_PDA_SUPPORT
        BT_GetPairedDeviceInfo(win_param_ptr->current_service, paired_device_index[cur_select_item -2], &current_device_info);            
#else
        BT_GetPairedDeviceInfo(win_param_ptr->current_service, paired_device_index[cur_select_item -1], &current_device_info);        
#endif
        MMIBT_EnterDeviceDetailsWin(&current_device_info,PNULL,PDA_PAIRED_OPT); 
        break;
#endif
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:

        cur_select_item = GUILIST_GetCurItemIndex(MMIBT_DEVICELIST_LIST_CTRL_ID);
        SCI_MEMSET(paired_device_index,0,sizeof(paired_device_index));
        MMIBT_GetPairedDevList(win_param_ptr->current_service, paired_device_index);
        SCI_TRACE_LOW("HandleDeviceListWinMsg cur_select_item =%d",cur_select_item);
        //"Add New Device"
        if(0 == cur_select_item) 
        { 
            SCI_MEMSET(&current_device_info, 0x00, sizeof(current_device_info));      
            //blue tooth is power on
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_SEARCH);
            if(BT_GetState())
            {
                DoOperation(MMIBT_OPC_SEARCH, FALSE);               
            }
            else
            {
                MMI_STRING_T display_str = {0};

                MMI_GetLabelTextByLang(TXT_TURN_ON_OR_NOT, &display_str);
                //blue tooth is power off
                OpenQueryWin(MMI_BT_QUERY_WIN_ID, &display_str, PNULL, MMIBT_OPC_SEARCH);
            }
        }
#ifdef MMI_PDA_SUPPORT
        else if(cur_select_item > 1)
        {            
            BT_GetPairedDeviceInfo(win_param_ptr->current_service, paired_device_index[cur_select_item -2], &current_device_info);
            SCI_TRACE_LOW("[&&&]devicelist address = %x, %x, %x, %x, %x, %x",
                        current_device_info.addr.addr[0],current_device_info.addr.addr[1],current_device_info.addr.addr[2],current_device_info.addr.addr[3],
                        current_device_info.addr.addr[4],current_device_info.addr.addr[5]);
            MMIBT_SetCurBtInfo(&current_device_info);
            SCI_TRACE_LOW("[&&&]devicelist cur_select_item = %d",cur_select_item);
            if(MMIBT_OPC_MAIN != win_param_ptr->root_opertion)
            {
                 DoOperation(win_param_ptr->root_opertion,TRUE);
            }
        }
#else

        //select or option
        else 
        {
            BT_GetPairedDeviceInfo(win_param_ptr->current_service, paired_device_index[cur_select_item -1], &current_device_info);
            MMIBT_SetCurBtInfo(&current_device_info);
            SCI_TRACE_LOW("HandleDeviceListWinMsg win_param_ptr->root_opertion =%d",win_param_ptr->root_opertion);

            if(MMIBT_OPC_MAIN == win_param_ptr->root_opertion)
            {
                BOOLEAN                is_connect = FALSE;
                uint16                 device_type = 0;
                MMI_MENU_GROUP_ID_T    group_id = MENU_BT_OTHER_DEVICE_DETAILS;
                
                //option menu
                BT_GetDeviceType(&current_device_info, &device_type);
#ifdef BT_DIALER_SUPPORT
                if(device_type & BT_DEVICE_CLASS_PHONE)
                {
                    //headset
                    is_connect = MMIBT_DeviceIsConnectedHandsetGateway(&current_device_info.addr);
                    {
                        //open or close headset permit
                        if(is_connect)
                        {
							MMIBT_EnterDeviceDetailsWin(&current_device_info,&s_title_str,NOT_HEADSET);
                            //current headset not connected
                            GUIMENU_SetItemVisible(MMIBT_DEVICE_DETAILS_MENU_CTRL_ID, group_id, ID_BT_DEVICE_CONNECT, FALSE);
                        }
                        else
                        {
                            //headset
                            is_connect = MMIBT_DeviceIsConnectedHeadset(&current_device_info.addr);

                            if((device_type & BT_DEVICE_CLASS_AUDIO) && (TRUE == is_connect))
                            {
                                //open or close headset permit
                                //current headset not connected
                                MMIBT_EnterDeviceDetailsWin(&current_device_info,&s_title_str,HEADSET_DISCONNECTED);
                                GUIMENU_SetItemVisible(MMIBT_DEVICE_DETAILS_MENU_CTRL_ID, group_id, ID_BT_DEVICE_CONNECT, FALSE);
                            }
                            else
                            {
                                MMIBT_EnterDeviceDetailsWin(&current_device_info,&s_title_str,HEADSET_CONNECTED);

                                //current headset already connected
                                GUIMENU_SetItemVisible(MMIBT_DEVICE_DETAILS_MENU_CTRL_ID, group_id, ID_BT_DEVICE_DISCONNECT, FALSE); 
                            }
                        }
                    }
                }
                else
#endif
                if(device_type & BT_DEVICE_CLASS_AUDIO)
                {
                    //headset
                    is_connect = MMIBT_DeviceIsConnectedHeadset(&current_device_info.addr); 
                    //open or close headset permit
                    if(is_connect)
                    {
                        //current headset not connected
                       MMIBT_EnterDeviceDetailsWin(&current_device_info,&s_title_str,HEADSET_DISCONNECTED);
                    }
                    else
                    {
                        //current headset already connected
                       MMIBT_EnterDeviceDetailsWin(&current_device_info,&s_title_str,HEADSET_CONNECTED);    
                    }
                }
                else
                {
                    //not headset
                    MMIBT_EnterDeviceDetailsWin(&current_device_info,&s_title_str,NOT_HEADSET);
                    GUIMENU_SetItemVisible(MMIBT_DEVICE_DETAILS_MENU_CTRL_ID, group_id, ID_BT_DEVICE_CONNECT, FALSE);
#ifdef BT_DIALER_SUPPORT
#ifndef WIN32
                    if(BT_HfuGetConnStatus(&current_device_info.addr))
                    {
                    
                    }
                    else
#endif
#endif
                    {
                        GUIMENU_SetItemVisible(MMIBT_DEVICE_DETAILS_MENU_CTRL_ID, group_id, ID_BT_DEVICE_DISCONNECT, FALSE);                    
                    }
                }
            }
            else
            {
                //selected this device to do operation                
                DoOperation(win_param_ptr->root_opertion,TRUE);
            }
        }
#endif
        break;
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:

        //midile softkey
        cur_select_item = GUILIST_GetCurItemIndex(MMIBT_DEVICELIST_LIST_CTRL_ID);
        SCI_MEMSET(paired_device_index,0,sizeof(paired_device_index));
        MMIBT_GetPairedDevList(win_param_ptr->current_service, paired_device_index);
        if((MMIBT_OPC_MAIN !=win_param_ptr->root_opertion) 
            ||0 == cur_select_item
          )
        {
            //Search Device
            MMK_SendMsg(win_id, MSG_CTL_OK, NULL);
        }
        else
        {              
            BOOLEAN                 is_connect = FALSE;
            uint32                  service_type = 0;
            int                     index = 0;
            MMIBT_OPERTATION_TYPE_E opc = MMIBT_OPC_NOT_CHANGE;

            cur_select_item = GUILIST_GetCurItemIndex(MMIBT_DEVICELIST_LIST_CTRL_ID);
#ifdef MMI_PDA_SUPPORT
            index = paired_device_index[cur_select_item -2];
#else
            index = paired_device_index[cur_select_item -1];
#endif
            BT_GetPairedDeviceInfo(win_param_ptr->current_service, index, &current_device_info);
            MMIBT_SetCurBtInfo(&current_device_info);
            service_type =  BT_GetPairedDeviceService(&current_device_info.addr);
            if(BT_SERVICE_A2DP & service_type || (BT_SERVICE_HANDSET|BT_SERVICE_HANDFREE) & service_type)
            {
                is_connect = MMIBT_DeviceIsConnectedHeadset(&current_device_info.addr);
                 //connect or disconnect headset
                if(is_connect)
				{
                    //if exist HFG service and HFG  active need to Close
					//or if exist A2DP service and A2DP active need to Close too
                    opc = MMIBT_OPC_CLOSE_HEADSET;
				}
				else
				{
#ifdef BT_DIALER_SUPPORT
                    //connect or disconnect headset
                    is_connect = MMIBT_DeviceIsConnectedHandsetGateway(&current_device_info.addr);
                    if(((BT_SERVICE_HEADSET_GATEWAY|BT_SERVICE_HANDFREE_GATEWAY) & service_type) && (TRUE == is_connect))
                    {
                        //if exist HFG service and HFG  active need to Close
                        //or if exist A2DP service and A2DP active need to Close too
                        opc = MMIBT_OPC_CLOSE_HANDSET_GATEWAY;
                    }
                    else
#endif
                    {
                        opc = MMIBT_OPC_OPEN_HEADSET;
                    }
                }
            }
#ifdef BT_DIALER_SUPPORT
            else if((BT_SERVICE_HEADSET_GATEWAY|BT_SERVICE_HANDFREE_GATEWAY) & service_type)
            {
                //connect or disconnect headset
                is_connect = MMIBT_DeviceIsConnectedHandsetGateway(&current_device_info.addr);
                if(is_connect)
                {
                    //if exist HFG service and HFG  active need to Close
                    //or if exist A2DP service and A2DP active need to Close too
                    opc = MMIBT_OPC_CLOSE_HANDSET_GATEWAY;
                }
                else
                {
                    opc = MMIBT_OPC_OPEN_HANDSET_GATEWAY;
                }
            }
#endif
            else 
            {
                //authority
                opc = MMIBT_OPC_DEVICE_DETAILS;
            }
			MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, opc);
			//connect headset
			if(BT_GetState() || MMIBT_OPC_DEVICE_DETAILS == opc)
			{
				
				if((MMIBT_OPC_CLOSE_HEADSET == opc)
#ifdef BT_DIALER_SUPPORT
                    || (MMIBT_OPC_CLOSE_HANDSET_GATEWAY == opc)
#endif             
				)
				{
					//open or close headset query
					MMI_GetLabelTextByLang(TXT_BT_WHETHER_DISCONNECT, &text_str1);
        			text_str2.wstr_ptr = current_device_info.name;
 					text_str2.wstr_len = MMIAPICOM_Wstrlen(current_device_info.name);
					//BT is ON, do operation
					OpenQueryWin(MMI_BT_QUERY_WIN_ID, &text_str1,  & text_str2, opc);
				}
				else
				{
					DoOperation(opc, FALSE);
				}
			}   
			else
			{
				MMI_GetLabelTextByLang(TXT_TURN_ON_OR_NOT, &text_str1);
				OpenQueryWin(MMI_BT_QUERY_WIN_ID, &text_str1, PNULL, opc);  
			}
        }
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:

        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:

		if(MMIBT_OPC_OPP_SEND == win_param_ptr->root_opertion)
		{
			if(win_param_ptr->transfer_file_info.is_temp_file)
			{
				MMIAPIFMM_DeleteFile(win_param_ptr->transfer_file_info.transfer_file_name, PNULL);
			}
		}
        SCI_FREEIF(win_param_ptr);
        MMK_DestroyDynaCtrl(MMIBT_DEVICELIST_LIST_CTRL_ID);
        break;

    default:
        result =MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
/*****************************************************************************/
//  Discription: Open the blue tooth device list  win   
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_EnterDeviceListWin(MMIBT_TRANSFER_FILE_INFO_T *file_info_ptr,
                                    MMIBT_OPERTATION_TYPE_E	root_opertion,
                                    uint32         service)
{
    MMIBT_DEVICE_LIST_PARAM_T device_list_param = {0}; 

    device_list_param.root_opertion = root_opertion;
    device_list_param.current_service = service;
    if(PNULL != file_info_ptr)
    {
        SCI_MEMCPY(device_list_param.transfer_file_info.transfer_file_name,file_info_ptr->transfer_file_name,sizeof(device_list_param.transfer_file_info.transfer_file_name));
        device_list_param.transfer_file_info.is_temp_file = file_info_ptr->is_temp_file;
    }
    OpenDeviceListWin(&device_list_param);
}
/*****************************************************************************/
//  Discription: Open the blue tooth device list  win   
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E OpenDeviceListWin(MMIBT_DEVICE_LIST_PARAM_T *param_ptr)
{
    MMIBT_DEVICE_LIST_PARAM_T *win_param_ptr = PNULL;
    MMI_HANDLE_T                win_handle = 0;
    BOOLEAN                     res = MMI_RESULT_FALSE;  
    
    if(PNULL == param_ptr)
        return res;
    
    if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
    {
        MMK_CloseWin(MMIBT_DEVICE_LIST_WIN_ID);
    }

    win_param_ptr = (MMIBT_DEVICE_LIST_PARAM_T *)SCI_ALLOC_APPZ(sizeof(MMIBT_DEVICE_LIST_PARAM_T));
    if(PNULL != win_param_ptr)
    {
        win_param_ptr->current_service = param_ptr->current_service;
        win_param_ptr->root_opertion = param_ptr->root_opertion;
        SCI_MEMSET(win_param_ptr->transfer_file_info.transfer_file_name,0,sizeof(win_param_ptr->transfer_file_info.transfer_file_name));
        SCI_MEMCPY(win_param_ptr->transfer_file_info.transfer_file_name, param_ptr->transfer_file_info.transfer_file_name, 
                        sizeof(win_param_ptr->transfer_file_info.transfer_file_name));
        win_param_ptr->transfer_file_info.is_temp_file = param_ptr->transfer_file_info.is_temp_file;

        win_handle = MMK_CreateWin((uint32 *)MMIBT_DEVICE_LIST_WIN_TAB, (ADD_DATA)win_param_ptr);
        if (!win_handle || (MMI_INVALID_ID == win_handle))
        {
            SCI_FREEIF(win_param_ptr);
        }
        else
        {
            MMK_SetWinUserData(MMIBT_DEVICE_LIST_WIN_ID, (void *)win_param_ptr);
            DeviceList_CreateList(MMIBT_DEVICE_LIST_WIN_ID, MMIBT_DEVICELIST_LIST_CTRL_ID,  win_param_ptr->current_service);
            res = MMI_RESULT_TRUE;
        }        
    }
    else
    {
        SCI_TRACE_LOW("OpenDeviceListWin alloc fail");
    }

    return res;
}
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void DeviceList_CreateList(
                    MMI_WIN_ID_T             win_id,                   
                    MMI_CTRL_ID_T            ctrl_id,  
                    uint32                   service
                    )
{
    GUILIST_INIT_DATA_T init_data = {0};
    MMI_TEXT_ID_T left_softkey_id   = TXT_NULL;
    MMI_TEXT_ID_T middle_softkey_id = TXT_NULL;
    MMI_TEXT_ID_T right_softkey_id  = STXT_RETURN;
    uint16          list_num = 0;
    int32          i = 0;
    uint32        paired_service = 0;
    BT_DEVICE_INFO dev_info = {0};
    uint8     paired_device_index[MMIBT_MAX_DEV_ITEMS] = {0}; 
    GUILIST_TYPE_E  type = GUILIST_TEXTLIST_E;
    MMIBT_OPERATION_INFO_T operation_info = MMIBT_GetOpertor();

    MMIBT_SetCurrentSevice(service);
    if(!MMK_IsOpenWin(win_id))
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8621_112_2_18_2_8_18_306,(uint8*)"");
        return;
    }
    
#ifdef MMI_PDA_SUPPORT
    if(win_id == MMIBT_DEVICE_LIST_WIN_ID)
    {
        init_data.both_rect = MMITHEME_GetWinClientBothRect(win_id);
        init_data.type = type;
        GUILIST_CreateListBox(win_id, 0, ctrl_id, &init_data);
    }
 #else
    init_data.both_rect = MMITHEME_GetWinClientBothRect(win_id);
    init_data.type = type;
    GUILIST_CreateListBox(win_id, 0, ctrl_id, &init_data);
#endif

        //list paired device
#ifdef MMI_PDA_SUPPORT
    GUILIST_SetMaxItem(ctrl_id,MMIBT_MAX_DEV_ITEMS + 2, FALSE);//max item 20
#else
    GUILIST_SetMaxItem(ctrl_id,MMIBT_MAX_DEV_ITEMS + 1, FALSE);//max item 20
    if(MMIBT_OPC_MAIN == operation_info.root_opertion)
    {
        left_softkey_id = STXT_OPTION;
    }
    else
    {
        left_softkey_id = STXT_SELECT;
    }
    
    GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
#endif
    //set "Add New Device" item first
#ifdef MMI_PDA_SUPPORT
	//fixed by hongjun.jia for CR177517
    MMIBT_AppendOneLineIconTextListItem(ctrl_id, IMAGE_CONTROL_LIST_ADD, TXT_BT_ADD_DEVICE, 0, FALSE);
    MMIBT_AppendSplitListItem(ctrl_id, TXT_BT_DEVICE_LIST, 1, FALSE);
#else
    MMIAPISET_AppendListItemByTextId(TXT_BT_ADD_DEVICE, STXT_ADD, ctrl_id, GUIITEM_STYLE_ONE_LINE_TEXT );
#endif
    SCI_MEMSET(paired_device_index,0,sizeof(paired_device_index));
    
    //get the paired device and append to the list 
    MMIBT_GetPairedDevList(service, paired_device_index);  
    list_num = BT_GetPairedDeviceCount(service);
    
    for(i =0; i<list_num; i++ )
    {
        //set middle softkey
        SCI_MEMSET(&dev_info, 0x00, sizeof(dev_info));
        BT_GetPairedDeviceInfo(service, paired_device_index[i], &dev_info);
        paired_service = BT_GetPairedDeviceService(&(dev_info.addr));
		BT_GetPairedDeviceName(&(dev_info.addr), dev_info.name);

        if(MMIBT_OPC_MAIN == operation_info.root_opertion)
        {
#ifdef BT_DIALER_SUPPORT
            if(MMIBT_DeviceIsConnectedHandsetGateway(&(dev_info.addr)))
            {
                ////current device is headset and already actived middle softkey is "Disconnect"
#ifdef MMI_GUI_STYLE_MINISCREEN
                middle_softkey_id = IMAGE_COMMON_SOFTKEY_DISCONNECT_ICON;
#else
                middle_softkey_id = TXT_COMM_DISCONNECT;
#endif					
            }
            else
#endif
            if(MMIBT_DeviceIsConnectedHeadset(&(dev_info.addr)))
            {
                ////current device is headset and already actived middle softkey is "Disconnect"
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
				middle_softkey_id = IMAGE_COMMON_SOFTKEY_DISCONNECT_ICON;
#else
                middle_softkey_id = TXT_COMM_DISCONNECT;
#endif
            }
#ifdef BT_DIALER_SUPPORT
            else if((paired_service & (BT_SERVICE_MAP_SERVER |BT_SERVICE_PBAP_SERVER)) || (paired_service & (BT_SERVICE_HEADSET_GATEWAY|BT_SERVICE_HANDFREE_GATEWAY)))
            {
                //current device is headset and not active, middle softkey is "Connect"
#ifdef MMI_GUI_STYLE_MINISCREEN
                middle_softkey_id = IMAGE_COMMON_SOFTKEY_CONNECT_ICON;
#else
                middle_softkey_id = TXT_COMMON_CONNECT_COMN;
#endif
            }
#endif
            else if((paired_service & (BT_SERVICE_HANDSET |BT_SERVICE_HANDFREE)) || (paired_service & BT_SERVICE_A2DP))
            {
                //current device is headset and not active, middle softkey is "Connect"
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
				middle_softkey_id = IMAGE_COMMON_SOFTKEY_CONNECT_ICON;
#else
                middle_softkey_id = TXT_COMMON_CONNECT_COMN;
#endif
            }
            else
            {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
				middle_softkey_id = IMAGE_COMMON_SOFTKEY_DETAIL_ICON;
#else
                middle_softkey_id = STXT_DETAIL;
#endif

            }
        }
        MMIBT_AppendDeviceListItem(
            &(dev_info),
            GUIITEM_STYLE_ONE_LINE_ICON_TEXT,
            left_softkey_id,
            middle_softkey_id,
            right_softkey_id,
            ctrl_id
            ); 
    }
}
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void CreatePubWinCallBack(MMI_HANDLE_T win_handle,const wchar *str_ptr,int str_len)
{
    if (MMK_IsOpenWin(win_handle))
    {
    	MMK_SendMsg(win_handle, MSG_BT_AUTO_CLOSE_WIN, PNULL);
        MMK_CloseWin(win_handle);
    }
}
#endif





















