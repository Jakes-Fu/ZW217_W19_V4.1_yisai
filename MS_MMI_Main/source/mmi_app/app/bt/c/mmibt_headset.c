/****************************************************************************
** File Name:     mmibt_headset.c                                     *
** Author:         yuantao.xu                                            *
** Date:           8/22/2012                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the bluetooth device paired*
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 8/22/2012    yuantao.xu           Create                                  *
**                                                                         *
****************************************************************************/
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                     *
**---------------------------------------------------------------------------*/
#define _MMIBT_HEADSET_C_
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                     *
**---------------------------------------------------------------------------*/
#include "mmi_app_bt_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmibt_func.h"
#include "mmipub.h"
#include "guilabel.h"
#include "mmifmm_export.h"   //bt_abs.h中用到ffs.h中的结构
#include "bt_abs.h"
#include "bt_cfg.h"
#include "mmibt_text.h"
#include "mmi_default.h"
#include "guitext.h"
#include "mmibt_a2dp.h"
#include "mmibt_app.h"
#include "mmibt_nv.h"
#include "guires.h"
#include "guiedit.h"
#include "guictrl_api.h"
#include "mmibt_headset.h"
#include "mmi_appmsg.h"
#include "ual_bt.h"
#ifdef ADULT_WATCH_SUPPORT
#include "adultwatch_bt_win.h"
#include "watch_cc_view.h"//Bug 1918144
#endif
#ifdef BLUETOOTH_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription:check headset all service is close
//  Global resource dependence: none 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN HeadsetIsClosed(BT_ADDRESS * in_addr, BT_ADDRESS *out_addr);
/*****************************************************************************/
//  Description :  HandleConnectCnfAlertWin
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandleConnectCnfAlertWin(
                         MMI_WIN_ID_T       win_id,     
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         );
/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                            LOCAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : close headset
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS CloseHeadset(const BT_ADDRESS *src_addr)
{
    uint32 dev_service = BT_GetPairedDeviceService(src_addr);
    BT_STATUS  return_value = BT_ERROR;
    
    //SCI_TRACE_LOW:"CloseHeadset enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10694_112_2_18_2_8_22_342,(uint8*)"");
    if(BT_SERVICE_A2DP & dev_service && MMIBT_GetCurrentA2dpStatus(src_addr))/*lint !e605 */
    {
        //A2DP need to close
        return_value = MMIBT_CloseA2dpHeadset();
        if(TRUE == return_value)
        {
            return_value = BT_PENDING;
        }
        else
        {
            return_value = BT_ERROR;
        }
        
    }
    else if((BT_SERVICE_HANDSET |BT_SERVICE_HANDFREE) & dev_service && MMIBT_IsDeviceActivedEarphone(src_addr))
    {
        //HFG need to close
        return_value =  BT_DeactivateHS();
    }
    else
    {
        return_value = BT_SUCCESS;
    }

    return return_value;    
}
/*****************************************************************************/
//  Discription:check headset all service is close
//  Global resource dependence: none 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN HeadsetIsClosed(BT_ADDRESS * in_addr, BT_ADDRESS *out_addr)
{
    BOOLEAN ret = TRUE;
    BOOLEAN is_active = FALSE;
    uint32      service_type = 0;
    BT_DEVICE_INFO dev_info = {0};
    BT_STATUS    status = 0;
    uint32      device_num = 0;
    uint32       i = 0;
    
    if(PNULL != in_addr)
    {
        //only check in_addr is connected headset or not
        //get current earphone status, if is active do not allow to delete device
        service_type = BT_GetPairedDeviceService(in_addr);
        if ((BT_SERVICE_HANDSET |BT_SERVICE_HANDFREE) & service_type)
        {
            is_active = MMIBT_IsDeviceActivedEarphone(in_addr);//MMIBT_GetCurrentEarphoneStatus(in_addr);
        }
    //#ifdef  BT_A2DP_SUPPORT  
        if (BT_SERVICE_A2DP & service_type)
        {
            is_active |= MMIBT_GetCurrentA2dpStatus(in_addr);
        }
    //#endif
        if(is_active)
        {
            if(PNULL != out_addr)
            {
              SCI_MEMCPY(out_addr, in_addr, sizeof(BT_ADDRESS));
            }
        }
    }
    else
    {
        //check all paired device to find exist Connected Headset
        device_num = BT_GetPairedDeviceCount(BT_SERVICE_HANDSET);   
        for(i = 1; i <= device_num; i++)
        {
             status = BT_GetPairedDeviceInfo(BT_SERVICE_HANDSET, i, &dev_info);
             if(BT_SUCCESS == status)
             {
                service_type = BT_GetPairedDeviceService(&dev_info.addr);
                if (BT_SERVICE_HANDSET & service_type)
                {
                    is_active = MMIBT_IsDeviceActivedEarphone(&dev_info.addr);//MMIBT_GetCurrentEarphoneStatus(&dev_info.addr);
                }
           // #ifdef  BT_A2DP_SUPPORT  
                if (BT_SERVICE_A2DP & service_type)
                {
                    is_active |= MMIBT_GetCurrentA2dpStatus(&dev_info.addr);
                }
            //#endif
                if(is_active)
                {
                    if(PNULL != out_addr)
                    {
                      SCI_MEMCPY(out_addr, &dev_info.addr, sizeof(BT_ADDRESS));
                    }                   
                    break;
                }
             }//if(BT_SUCCESS == status)
        }//for(i = 1; i <= device_num; i++)
    }
    ret = !is_active;
    //SCI_TRACE_LOW:"HeadsetIsClosed ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9149_112_2_18_2_8_19_316,(uint8*)"d",ret);
    return ret;
}
/*****************************************************************************/
//  Description : handle headset close result
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void handleHeadsetCloseResult(BT_STATUS bt_status, BT_ADDRESS *headset_addr_ptr)
{
    MMI_STRING_T wait_text = {0};
    MMI_WIN_ID_T alert_id = MMI_BT_REMOTE_ALERT_WIN_ID;
    BT_ADDRESS   connect_addr = {0};
    MMIBT_OPERATION_INFO_T operation_info = MMIBT_GetOpertor();

    //wait_text.wstr_ptr = wait_text_buf;    
    if(BT_SUCCESS == bt_status)
    {
        //close finished
        if(MMIBT_OPC_OPEN_HEADSET != operation_info.current_opertion)
        {
            //xingdong_bt
            //no HFG and A2DP need to active, show "Alerady Closed"
            if(HeadsetIsClosed(PNULL, PNULL))
            {
                MMIDEFAULT_SetBackLight(TRUE);
				MMI_GetLabelTextByLang(TXT_BT_HEADSET_CLOSED, &wait_text);

                MMIPUB_OpenAlertWinByTextPtr(PNULL,&wait_text,PNULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
#ifdef BT_DIALER_SUPPORT
                MMIBT_GetBtStatusForIcon();
#endif
            }
            //SetBackgroupRun(FALSE);
        }
        else
        { 
            //current operation not close headset, need continue to do current_opertion
            //xingdong_bt
            //SetBackgroupRun(FALSE);
            DoOperation(operation_info.current_opertion, FALSE);
        }
    }
    else if (BT_PENDING == bt_status)
    { //Closing...
        //xingdong_bt

        if(HeadsetIsClosed(PNULL, &connect_addr) || (!memcmp(headset_addr_ptr, &connect_addr, sizeof(BT_ADDRESS))))
        {
            if(!MMK_IsOpenWin(MMIBT_EARCONN_WAIT_WIN_ID))// && (!GetIsBackgroupRun()))
            {
                if(MMIBT_OPC_OPEN_HEADSET == operation_info.current_opertion)
                {
                    MMI_GetLabelTextByLang(TXT_BT_ACTIVATING, &wait_text);
                }
                else
                {
                    //Dective
                    MMI_GetLabelTextByLang(TXT_BT_DISCONNECTING, &wait_text);
                }
                MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_EARCONN_WAIT_WIN_ID,IMAGE_NULL,
                    ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePubwinWaitingMsg);           
            }
        }
    }
    else
    {
        //xingdong_bt
        //close error
        //if(!GetIsBackgroupRun())
        {
            MMIDEFAULT_SetBackLight(TRUE);
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_CANNOT_FIND_DEVICE,TXT_NULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
        }
        //SetBackgroupRun(FALSE);
    } 
}
/*****************************************************************************/
//  Description : open headset
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void OpenHeadset(const BT_ADDRESS *src_addr)
{
    uint32 dev_service = 0;
    BT_STATUS return_value = BT_ERROR;
    MMI_STRING_T wait_text = {0};
    MMI_WIN_ID_T alert_id = MMI_BT_REMOTE_ALERT_WIN_ID;
    MMI_WIN_ID_T connect_alert_id = MMI_BT_CONNECT_CNF_WIN_ID;
    BT_ADDRESS  address = {0};
    //uint16 		dev_name[BT_DEVICE_NAME_SIZE] = {0}; 
#ifdef BT_DIALER_SUPPORT
    BT_ADDRESS   dev_addr = {0};
#endif

#if (defined BT_HL_PATCH_SUPPORT) || (defined BLUETOOTH_SUPPORT_SPRD_BT)				
    if(MMIBT_IsOnTransferStatus())
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_TRANSFER_FILE_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return;
    }
#endif
#ifdef BT_DIALER_SUPPORT
    if(MMIBT_IsConnectDevice(&dev_addr))
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_HFU_CONNECT_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return;
    }
#endif
    
    //open headset
    //SCI_TRACE_LOW:"OpenHeadset enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10737_112_2_18_2_8_22_343,(uint8*)"");
    if(!HeadsetIsClosed(PNULL, &address) && (memcmp(src_addr, &address, sizeof(BT_ADDRESS))))
    {
        //exist not closed service, close it first
        return_value = CloseHeadset(&address);
        handleHeadsetCloseResult(return_value, &address);
        return;
    }
    //SCI_TRACE_LOW:"OpenHeadset address = %x, %x, %x, %x, %x, %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10745_112_2_18_2_8_22_344,(uint8*)"dddddd", address.addr[0],address.addr[1],address.addr[2],address.addr[3],address.addr[4],address.addr[5]);
    //SCI_TRACE_LOW:"OpenHeadset src_addr = %x, %x, %x, %x, %x, %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10746_112_2_18_2_8_22_345,(uint8*)"dddddd", src_addr->addr[0],src_addr->addr[1],src_addr->addr[2],src_addr->addr[3],src_addr->addr[4],src_addr->addr[5]);
    
    dev_service = BT_GetPairedDeviceService(src_addr);
    if((BT_SERVICE_HANDSET |BT_SERVICE_HANDFREE) & dev_service && (!MMIBT_IsDeviceActivedEarphone(src_addr)))
    {
        //HFG need to active
        return_value = BT_ActivateHS(src_addr);
    }
    else if(BT_SERVICE_A2DP & dev_service && (!MMIBT_GetCurrentA2dpStatus(src_addr)))/*lint !e605 */
    {
        //A2DP need to active
        return_value = MMIBT_OpenA2dpHeadset(*src_addr);
        if(TRUE == return_value)
        {
            return_value = BT_PENDING;
        }
		else
		{
			return_value = BT_ERROR;
		}
    }
    else
    {
        //SCI_TRACE_LOW:"MMIBT LOCAL void OpenHeadset enter, TXT_BT_HEADSET_OPENED"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10769_112_2_18_2_8_22_346,(uint8*)"");
		MMIDEFAULT_SetBackLight(TRUE);
		MMK_CloseWin(alert_id);
		MMI_GetLabelTextByLang(TXT_BT_HEADSET_OPENED, &wait_text);
        MMIPUB_OpenAlertWinByTextPtr(PNULL,&wait_text,PNULL,IMAGE_PUBWIN_SUCCESS,
            &connect_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,HandleConnectCnfAlertWin);
        MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
        MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_ACTIVE);
#ifdef BT_BQB_SUPPORT
        MMIAPICC_IsCallingAndInfoBt();
		SCI_TRACE_LOW("MMIBT LOCAL void OpenHeadset, MMIAPICC_IsCallingAndInfoBt");
#endif
#ifdef BT_DIALER_SUPPORT
        MMIBT_GetBtStatusForIcon();
#endif
        return;
    }
    if (BT_PENDING == return_value)
    {
        //xingdong_bt
        //Opening...
        if(!MMK_IsOpenWin(MMIBT_EARCONN_WAIT_WIN_ID))// && (!GetIsBackgroupRun()))
        {
            MMI_GetLabelTextByLang(TXT_BT_ACTIVATING, &wait_text);
			MMIDEFAULT_SetBackLight(TRUE);
            MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_EARCONN_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePubwinWaitingMsg);
        }
    }
    //xingdong_bt
    else if(return_value != BT_SUCCESS)// && (!GetIsBackgroupRun()))
    {
        //Open or close error
        MMIDEFAULT_SetBackLight(TRUE);
        //MMIPUB_CloseAlertWin();
        MMK_CloseWin(alert_id);
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_CANNOT_FIND_DEVICE,TXT_NULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
    }
}
/*****************************************************************************/
//  Discription: get active earphone result confirm
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_OpenHeadsetCnf(BT_MSG_T* msg_body_ptr, BOOLEAN is_a2dp)
{
    BT_ADDRESS     address = {0};
    BT_ADDRESS     connected_address = {0};
    BT_DEVICE_INFO connect_dev_info = {0};
    //BT_STATUS     return_value = BT_SUCCESS;
    MMI_STRING_T wait_text = {0};
    MMI_WIN_ID_T alert_id = MMI_BT_ALERT_WIN_ID;
    MMI_WIN_ID_T connect_alert_id = MMI_BT_CONNECT_CNF_WIN_ID;
#ifdef BT_DIALER_SUPPORT
    BT_ADDRESS   dev_addr = {0};
#endif
    
    if (PNULL == msg_body_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_OpenHeadsetCnf PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6264_112_2_18_2_8_13_270,(uint8*)"");
        return;
    }
#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
    if(MMIBT_IsOnTransferStatus())
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_TRANSFER_FILE_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return;
    }
#endif
#ifdef BT_DIALER_SUPPORT
    if(MMIBT_IsConnectDevice(&dev_addr))
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_HFU_CONNECT_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return;
    }
#endif
    //xingdong_bt
    //SetBackgroupRun(FALSE);
    if(is_a2dp)
    {
        //set a2dp flag and status
        MMIBT_A2dpLinkConnected(msg_body_ptr);
        address = MMIBT_GetA2dAddress(msg_body_ptr);
    }
    else
    {
        address = *(BT_ADDRESS *)msg_body_ptr->body_ptr;
        //address = remote_dev_info->addr;
        MMIBT_SetHfuStatus();
    }    
    //SCI_TRACE_LOW:"MMIBT_OpenHeadsetCnf address = %x, %x, %x, %x, %x, %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6281_112_2_18_2_8_13_271,(uint8*)"dddddd", address.addr[0],address.addr[1],address.addr[2],address.addr[3],address.addr[4],address.addr[5]);

    BT_GetPairedDeviceName(&address, connect_dev_info.name);
	if(BT_SUCCESS != msg_body_ptr->status)
	{
		//SCI_TRACE_LOW:"MMIBT_OpenHeadsetCnf msg_body_ptr->status %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6286_112_2_18_2_8_13_272,(uint8*)"d", msg_body_ptr->status);
		MMIDEFAULT_SetBackLight(TRUE);
		ClosePrmAndOpenDoneWin(
			MMIBT_EARCONN_WAIT_WIN_ID,
			TXT_BT_CANNOT_FIND_DEVICE,  
			IMAGE_PUBWIN_FAIL,
			MMI_2SECONDS
			);
		return;
	}
    //只有当窗口打开时才处理该消息
    if(MMK_IsOpenWin(MMIBT_EARCONN_WAIT_WIN_ID))
    {
        if (BT_SUCCESS == msg_body_ptr->status)
        {
            OpenHeadset(&address);
        }
    }
    else
    {
        //remote open 
        //CR 228983
        if(!HeadsetIsClosed(PNULL, &connected_address) && (memcmp(&connected_address, &address, sizeof(BT_ADDRESS))))
        {
            //another headset already open, must close this headset
            //SetBackgroupRun(TRUE);
            handleHeadsetCloseResult(CloseHeadset(&(address)), &address);
        }
        else
        {
            if(MMIBT_DeviceIsConnectedHeadset(&address))
            {
                 //opened finished 
                //this headset is opened
                //xingdong_bt
                //if(!GetIsBackgroupRun() && MMIBT_OpenedHeadsetIsExist(&connect_dev_info))
                if(MMIBT_OpenedHeadsetIsExist(&connect_dev_info))
                {
                    //SCI_TRACE_LOW:"MMIBT MMIBT_OpenHeadsetCnf enter, TXT_BT_HEADSET_OPENED"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6324_112_2_18_2_8_13_273,(uint8*)"");
                    MMIDEFAULT_SetBackLight(TRUE);
					MMK_CloseWin(alert_id);
                    //wait_text.wstr_ptr = wait_text_buf;
					MMI_GetLabelTextByLang(TXT_BT_HEADSET_OPENED, &wait_text);
                    MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_ACTIVE);
                    MMIPUB_OpenAlertWinByTextPtr(PNULL,&wait_text,PNULL,IMAGE_PUBWIN_SUCCESS,
                        &connect_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,HandleConnectCnfAlertWin);
#ifdef BT_DIALER_SUPPORT
                    MMIBT_GetBtStatusForIcon();
#endif
                }
#ifdef BT_BQB_SUPPORT
                MMIAPICC_IsCallingAndInfoBt();
#endif                 
            }
            else
            {
                OpenHeadset(&address); 
            }
        } 
        //remote open
    }
    if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID) && MMIBT_DeviceIsConnectedHeadset(&address))
    {
        //Update Device list
        MMK_PostMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, PNULL, PNULL);
    }
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
    MMIAPIENVSET_SetBluetoothMode();
#endif

#ifdef ADULT_WATCH_SUPPORT
    WatchCC_SetVolumeWin_Exit();//Bug 1918144
#endif

}

/*****************************************************************************/
//  Discription: get deactive earphone confirm
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_CloseHeadsetCnf(BT_MSG_T* msg_body_ptr, BOOLEAN is_a2dp)
{
    MMI_WIN_ID_T alert_id = MMI_BT_ALERT_WIN_ID;
    BT_ADDRESS   address = {0};
    BT_STATUS    return_value = BT_SUCCESS;
    BT_DEVICE_INFO *device = PNULL;
    BT_DEVICE_INFO  device_info = {0};
    MMI_STRING_T wait_text = {0};

    if ((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        //SCI_TRACE_LOW:"MMIBT_CloseHeadsetCnf PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6371_112_2_18_2_8_13_274,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"MMIBT_CloseHeadsetCnf is_a2dp %d msg_body_ptr->status %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6374_112_2_18_2_8_13_275,(uint8*)"dd",is_a2dp, msg_body_ptr->status);
    device = (BT_DEVICE_INFO *)msg_body_ptr->body_ptr;
//lzk mark
#ifdef BT_DIALER_SUPPORT	
    MMIBT_A2dpLinkDisconnect(msg_body_ptr);
#endif
    
	if(is_a2dp)
    {
	    //MMIBT_HandleAppOperation(MMIBT_A2DP_OPR_FUNC_DISCONNECT);
        //set a2dp flag and status
//lzk mark
#ifdef BT_DIALER_SUPPORT
//         MMIBT_A2dpLinkDisconnect(msg_body_ptr);
#else
           MMIBT_A2dpLinkDisconnect(msg_body_ptr);
#endif
        address = MMIBT_GetA2dAddress(msg_body_ptr);
    }
    else
    {
        address = device->addr;
    }
    BT_GetPairedDeviceName(&address, device_info.name);
    //SCI_TRACE_LOW:"MMIBT_CloseHeadsetCnf address = %x, %x, %x, %x, %x, %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6387_112_2_18_2_8_13_276,(uint8*)"dddddd", address.addr[0],address.addr[1],address.addr[2],address.addr[3],address.addr[4],address.addr[5]);
    //xingdong_bt
    if(HeadsetIsClosed(PNULL, PNULL))
    {
        //closed headset is playing headset or no active headset exist, need stop music
//         MMIBT_RecoveryBtStatus(MMIBT_MODULE_A2DP);
        //xingdong_bt
        MMIBT_HandleAppOperation(MMIBT_A2DP_OPR_FUNC_DISCONNECT);

        MMIBT_StopDevice(MMIBT_GetA2dpAppType());
        //recovery device status
        MMIBT_Init_ActionQueue();
        MMIBT_SetDeviceStatus(MMIBT_HEADSET_STATUS_STOP); 
    }
    if (BT_SUCCESS == msg_body_ptr->status)
    {   
        //check need close other service or not
        return_value = CloseHeadset(&address); 
        if (MMK_IsOpenWin(MMIBT_EARCONN_WAIT_WIN_ID))
        {
            handleHeadsetCloseResult(return_value, &address);   
        }
        if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
        {
            //Update Device list
            MMK_SendMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, PNULL);
        }
    }
    else
    {
        MMIDEFAULT_SetBackLight(TRUE);
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_CANNOT_FIND_DEVICE,TXT_NULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
    }

    if (MMK_IsOpenWin(MMIBT_EARCONN_WAIT_WIN_ID))
    {
         MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_DEACTIVE);
        //xingdong_bt
        //SetBackgroupRun(FALSE);
    }
    else if(MMIBT_IsPhoneReady())
    {
        //phone ready and not bt power off action

        //wait_text.wstr_ptr = wait_text_buf;
		MMI_GetLabelTextByLang(TXT_BT_HEADSET_CLOSED, &wait_text);
        if(HeadsetIsClosed(PNULL, PNULL))
        {
            //xingdong_bt
            //close all service 
            //if(!GetIsBackgroupRun())
            {
				MMIDEFAULT_SetBackLight(TRUE);
                MMIPUB_OpenAlertWinByTextPtr(PNULL, &wait_text,PNULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                //Update device list
                MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_DEACTIVE);
                if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
                {
                    MMK_SendMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, NULL);
                }
#ifdef BT_DIALER_SUPPORT
                MMIBT_GetBtStatusForIcon();
#endif
            }
            //SetBackgroupRun(FALSE);
        }
        MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
    }
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
    MMIAPIENVSET_UnSetBluetoothMode();
#endif

#ifdef ADULT_WATCH_SUPPORT
    WatchCC_SetVolumeWin_Exit();//Bug 1918144
#endif

}
/*****************************************************************************/
//  Discription: get active earphone result confirm
//  Global resource dependence: None
//  Author:
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_OpenHeadsetConfirmIND(BT_MSG_T* msg_body_ptr, BOOLEAN is_a2dp)
{
    BT_ADDRESS     address = {0};
    BT_DEVICE_INFO connect_dev_info = {0};
    MMI_STRING_T wait_text = {0};
    MMI_WIN_ID_T alert_id = MMI_BT_ALERT_WIN_ID;
    MMI_WIN_ID_T connect_alert_id = MMI_BT_CONNECT_CNF_WIN_ID;
#ifdef BT_DIALER_SUPPORT
    BT_ADDRESS   dev_addr = {0};
#endif
    
    if(PNULL == msg_body_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_OpenHeadsetConfirmIND msg_body_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_11086_112_2_18_2_8_23_354,(uint8*)"");
        return;
    }
    if(is_a2dp)
    {
        //set a2dp flag and status
        //MMIBT_A2dpLinkConnected(msg_body_ptr);
        address = MMIBT_GetA2dAddress(msg_body_ptr);
    }
    else
    {
        address = *(BT_ADDRESS *)msg_body_ptr->body_ptr;
        //MMIBT_SetHfuStatus();
        MMIBT_StopOpenWaitTimer();
    }    
	 //added tonny.chen
#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
    if(MMIBT_IsOnTransferStatus())
    {

        CloseHeadset(&address);
        MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_TRANSFER_FILE_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return;
    }
#endif
    //end added 
#ifdef BT_DIALER_SUPPORT
    if(MMIBT_IsConnectDevice(&dev_addr))
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_HFU_CONNECT_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return;
    }
#endif
    
    if(is_a2dp)
    {
        //set a2dp flag and status
        MMIBT_A2dpLinkConnected(msg_body_ptr);
        //address = MMIBT_GetA2dAddress(msg_body_ptr);
    }
    else
    {
        //address = *(BT_ADDRESS *)msg_body_ptr->body_ptr;
        MMIBT_SetHfuStatus();
    }    
    //SCI_TRACE_LOW:"MMIBT_OpenHeadsetConfirmIND address = %x, %x, %x, %x, %x, %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_11101_112_2_18_2_8_23_355,(uint8*)"dddddd", address.addr[0],address.addr[1],address.addr[2],address.addr[3],address.addr[4],address.addr[5]);
     
    BT_GetPairedDeviceName(&address, connect_dev_info.name);
	if(BT_SUCCESS != msg_body_ptr->status)
	{
		//SCI_TRACE_LOW:"MMIBT_OpenHeadsetConfirmIND msg_body_ptr->status %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_11106_112_2_18_2_8_23_356,(uint8*)"d", msg_body_ptr->status);
		MMIDEFAULT_SetBackLight(TRUE);
		ClosePrmAndOpenDoneWin(
			MMIBT_EARCONN_WAIT_WIN_ID,
			TXT_BT_CANNOT_FIND_DEVICE,  
			IMAGE_PUBWIN_FAIL,
			MMI_2SECONDS
			);
		return;
	}  
    if(MMIBT_DeviceIsConnectedHeadset(&address))
    {
        //opened finished 
        //this headset is opened
        if(MMIBT_OpenedHeadsetIsExist(&connect_dev_info))
        {
            //SCI_TRACE_LOW:"MMIBT MMIBT_OpenHeadsetConfirmIND enter, TXT_BT_HEADSET_OPENED"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_11122_112_2_18_2_8_23_357,(uint8*)"");
            MMIDEFAULT_SetBackLight(TRUE);
			MMK_CloseWin(alert_id);
            MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
			MMI_GetLabelTextByLang(TXT_BT_HEADSET_OPENED, &wait_text);
            MMIPUB_OpenAlertWinByTextPtr(PNULL,&wait_text,PNULL,IMAGE_PUBWIN_SUCCESS,
                &connect_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,HandleConnectCnfAlertWin);
            MMISRVAUD_HandleBtEvent(MMISRVAUD_BT_EVENT_ACTIVE);
#ifdef BT_DIALER_SUPPORT
            MMIBT_GetBtStatusForIcon();
#endif
        }
#ifdef BT_BQB_SUPPORT
        MMIAPICC_IsCallingAndInfoBt();
		SCI_TRACE_LOW("MMIBT LOCAL void OpenHeadset, MMIAPICC_IsCallingAndInfoBt");
#endif
    }
    if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID) && MMIBT_DeviceIsConnectedHeadset(&address))
    {
        //Update Device list
        MMK_PostMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, PNULL, PNULL);
    }
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
    MMIAPIENVSET_SetBluetoothMode();
#endif
}
/*****************************************************************************/
//  Discription:check the deviece is connected headset
//  Global resource dependence: none 
//  Author: 
//  Note: if addr == NULL, it will check all paired device to find exist Connected Headset,
//        the connected headset will return in  out_addr
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_DeviceIsConnectedHeadset(BT_ADDRESS * in_addr)
{
    BOOLEAN ret = FALSE;
    BOOLEAN is_single_service = TRUE;
    uint32      service_type = 0;

    if(PNULL != in_addr)
    {
        //only check in_addr is connected headset or not
        //get current earphone status, if is active do not allow to delete device
        service_type = BT_GetPairedDeviceService(in_addr);
        if ((BT_SERVICE_HANDSET |BT_SERVICE_HANDFREE) & service_type)
        {
            ret = MMIBT_IsDeviceActivedEarphone(in_addr);//MMIBT_IsDeviceActivedEarphone(in_addr);
            is_single_service = FALSE;
        }
    //#ifdef  BT_A2DP_SUPPORT  
        if (BT_SERVICE_A2DP & service_type)
        {
            if(is_single_service)
            {
                ret = MMIBT_GetCurrentA2dpStatus(in_addr);
            }
            else
            {
                ret &= MMIBT_GetCurrentA2dpStatus(in_addr);
            }
        }
    //#endif
    }
    //SCI_TRACE_LOW:"MMIBT_DeviceIsConnectedHeadset ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9016_112_2_18_2_8_18_312,(uint8*)"d",ret);
    return ret;
}
/*****************************************************************************/
//  Discription:check exist opened headset or not
//  Global resource dependence: none 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_OpenedHeadsetIsExist(BT_DEVICE_INFO *out_dev)
{
    uint32 device_num = BT_GetPairedDeviceCount(BT_SERVICE_HANDSET);    
    uint32 i = 0;
    BT_STATUS status = BT_SUCCESS;
    BT_DEVICE_INFO dev_info = {0};
    BOOLEAN   exist = FALSE;
    for(i = 1; i <= device_num; i++)
    {
        status = BT_GetPairedDeviceInfo(BT_SERVICE_HANDSET, i, &dev_info);
        if(BT_SUCCESS == status)
        {
            if(MMIBT_DeviceIsConnectedHeadset(&(dev_info.addr)))
            {
                exist = TRUE;
                if(PNULL != out_dev)
                {
                    SCI_MEMCPY(out_dev, &dev_info, sizeof(BT_DEVICE_INFO));
                }
                break;
            }
        }

    }//f
    //SCI_TRACE_LOW:"MMIBT_OpenedHeadsetIsExist exist %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9077_112_2_18_2_8_18_315,(uint8*)"d",exist);
    return exist;
}
/*****************************************************************************/
//  Discription:  MMIAPIBT_ Open Bt Headset List
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_OpenBtHeadsetList(void)
{
    MMIBT_SetOpertor(MMIBT_OPC_MAIN, MMIBT_OPC_LIST_PAIRED);
    MMIBT_EnterDeviceListWin(PNULL,MMIBT_OPC_MAIN,BT_SERVICE_HANDFREE);
}
/*****************************************************************************/
//  Description :  HandleConnectCnfAlertWin
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandleConnectCnfAlertWin(
                         MMI_WIN_ID_T       win_id,     
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_CLOSE_WINDOW:
        MMIBT_HandleAppOperation(MMIBT_A2DP_OPR_FUNC_CONNECT);
        MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return result;
}
#ifdef MMIBT_HEADSET_AUTHORIZE 
/*****************************************************************************/
//  Discription: Set bluetooth device authorize status   
//  Global resource dependence: none 
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_SetAuthorization(BT_MSG_T* msg_body_ptr)
{
    MMI_STRING_T    display_text = {0};
    MMI_STRING_T    device_name = {0};
    BT_ADDRESS     address = {0};
    BT_DEVICE_INFO connect_dev_info = {0};

    if(PNULL == msg_body_ptr)
    {
        SCI_TRACE_LOW("MMIBT_SetAuthorization msg_body_ptr is null");
        return;
    }
    
    address = *(BT_ADDRESS *)msg_body_ptr->body_ptr;
    SCI_TRACE_LOW("MMIBT_OpenHeadsetConfirmIND address = %x, %x, %x, %x, %x, %x",
            (uint8*)"dddddd", address.addr[0],address.addr[1],address.addr[2],address.addr[3],address.addr[4],address.addr[5]);

    MMI_GetLabelTextByLang(TXT_BT_AUDIO_CONNECT, &display_text);
     
    BT_GetPairedDeviceName(&address, connect_dev_info.name);   
    device_name.wstr_ptr = connect_dev_info.name;
    device_name.wstr_len = MMIAPICOM_Wstrlen(connect_dev_info.name);
    MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_HFG_AUDIO_CONNECT);
    OpenQueryWin(MMI_BT_QUERY_WIN_ID, &display_text, &device_name, MMIBT_HFG_AUDIO_CONNECT);
}
#endif

/*****************************************************************************/
//  Discription:close headset
//  Global resource dependence: none 
//  Author:chunjuan.liang
//  Note: ual中的蓝牙地址转换为协议栈蓝牙地址
/*****************************************************************************/
PUBLIC int32 CloseHeadsetEx(uint8 *p_bt_addr)
{
    int32          ret = 0;
    BT_ADDRESS bt_addr = {0};
    BT_STATUS  bt_ret  = BT_ERROR;

    if(PNULL == p_bt_addr)
    {
        SCI_TRACE_LOW("CloseHeadsetEx p_bt_addr is null");
        return -1;
    }

    SCI_MEMCPY(&(bt_addr.addr),p_bt_addr,sizeof(BT_ADDRESS));

    bt_ret = CloseHeadset(&bt_addr);
    ret = (int32)bt_ret;

    return ret;
}
/*****************************************************************************/
//  Discription:check has headset connected
//  Global resource dependence: none 
//  Author:chunjuan.liang
//  Note: ual中的蓝牙地址转换为协议栈蓝牙地址
/*****************************************************************************/

PUBLIC BOOLEAN MMIBT_DeviceIsConnectedHeadsetEx(uint8 *p_bt_addr)
{
    BT_ADDRESS bt_addr = {0};
    if(PNULL == p_bt_addr)
    {
        SCI_TRACE_LOW("MMIBT_DeviceIsConnectedHeadsetEx p_bt_addr is null");
        return FALSE;
    }
    SCI_MEMCPY(&(bt_addr.addr),p_bt_addr,sizeof(BT_ADDRESS));

    return MMIBT_DeviceIsConnectedHeadset(&bt_addr);
}

/*****************************************************************************/
//  Discription:check headset permit to open
//  Global resource dependence: none 
//  Author:chunjuan.liang
//  Note: ual中的蓝牙地址转换为协议栈蓝牙地址
/*****************************************************************************/
PUBLIC BOOLEAN HeadsetOpenIsPermitEx(uint8 *p_bt_addr)
{
    BT_ADDRESS bt_addr = {0};
    if(PNULL == p_bt_addr)
    {
        SCI_TRACE_LOW("HeadsetOpenIsPermitEx p_bt_addr is null");
        return FALSE;
    }

    SCI_MEMCPY(&(bt_addr.addr),p_bt_addr,sizeof(BT_ADDRESS));

    return HeadsetOpenIsPermit(&(bt_addr ));
}

/*****************************************************************************/
//  Description : open headsetEx
//  Global resource dependence : none
//  Author:chunjuan.liang
//  Note: ual中的蓝牙地址转换为协议栈蓝牙地址
/*****************************************************************************/
PUBLIC void OpenHeadsetEx(uint8 *src_addr)
{
    BT_ADDRESS bt_addr = {0};
    if(PNULL == src_addr)
    {
        SCI_TRACE_LOW("HeadsetOpenIsPermitEx p_bt_addr is null");
        return;
    }

    SCI_MEMCPY(&(bt_addr.addr),src_addr,sizeof(BT_ADDRESS));

    OpenHeadset(&(bt_addr ));
}
#ifdef ADULT_WATCH_SUPPORT
PUBLIC BOOLEAN AdultWatch_RemoveHeadset(BT_MSG_T *msg_body,BOOLEAN is_a2dp)
{
    BT_STATUS del_ret = BT_SUCCESS;
    BT_ADDRESS   addr = {0};
    uint8   headset_addr[UAL_BT_DEV_ADDRESS_SIZE] = {0};
    if ((PNULL == msg_body) || (PNULL == msg_body->body_ptr) )
    {
        SCI_TRACE_LOW("msg_body_ptr is null");
        return FALSE;
    }
    if(TRUE == is_a2dp)//fix bug1492118
    {
        BT_A2DP_INFO_T a2dp_info = {0};

        a2dp_info = *(BT_A2DP_INFO_T *)msg_body->body_ptr;
        SCI_MEMCPY(&(addr.addr), a2dp_info.dev_addr.addr, BT_ADDRESS_SIZE);
    }
    else
    {
        BT_DEVICE_INFO  device = {0};

        device = *(BT_DEVICE_INFO *)msg_body->body_ptr;
        SCI_MEMCPY(&(addr.addr), device.addr.addr, BT_ADDRESS_SIZE);
    }
    AdultWatch_GetRemoveHeadsetAddr(headset_addr);
    SCI_TRACE_LOW("g_remove_paired_dev_infoaddr %d,%d,%d,%d,%d,%d",headset_addr[0],headset_addr[1],headset_addr[2],headset_addr[3],headset_addr[4],headset_addr[5]);
    SCI_TRACE_LOW("device address %d,%d,%d,%d,%d,%d",addr.addr[0],addr.addr[1],addr.addr[2],addr.addr[3],addr.addr[4],addr.addr[5]);

    if(HeadsetIsClosed(&addr,PNULL))  //bug1352192
    {
        if(!memcmp(headset_addr, &addr, sizeof(BT_ADDRESS)))
        {
            SCI_TRACE_LOW("delete BT headset");
            del_ret = BT_RemovePairedDevice(&addr);
            SCI_TRACE_LOW("del_ret = %d",del_ret);
        }
    }else
    {
        del_ret = CloseHeadset(&addr);
    }
}
#endif
#endif
