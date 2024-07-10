/****************************************************************************
** File Name:     mmibt_mainwin.c                                        *
** Author:         yuantao.xu                                            *
** Date:           8/3/2012                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the cryp display menu     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 8/3/2012    yuantao.xu     Create                                  *
**                                                                         *
****************************************************************************/
#define _MMIBT_MAINWIN_C_
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                     *
**---------------------------------------------------------------------------*/
#include "mmi_app_bt_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_timer.h"
#include "mmibt_func.h"
#include "mmibt_mainwin.h"
#include "mmibt_app.h"
#include "mmipub.h"
#include "guilabel.h"
#include "mmifmm_export.h"
#include "bt_abs.h"
#include "bt_cfg.h"
#include "mmibt_text.h"
#include "mmi_default.h"
#include "mmiset_export.h"
#include "guitext.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "mmivcard_export.h"
#include "mmieng_export.h"
#include "mmicl_export.h"
#include "mmiphone_export.h"
#include "mmibt_a2dp.h"
#include "mmiudisk_export.h"
#include "mmisd_export.h"
#include "mmibt_nv.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmipicview_export.h"
#include "mmialarm_export.h"
#include "mmi_mainmenu_export.h"
#include "mmifmm_text.h"
#include "guimenu.h"
#include "guires.h"
#include "mmidc_export.h"
#include "guiedit.h"
#include "mmiidle_export.h"
#include "mmimtv_export.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#include "mmi_autotest.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#include "mmieng_id.h"
#include "guictrl_api.h"
#include "mmieng_text.h"
#include "mmienvset_export.h"
#include "mime_type.h"
#include "mmisms_export.h"
#ifdef DRM_SUPPORT
#include "mmidrm.h"
#endif
#include "mmikl_export.h"
#ifdef WRE_SUPPORT
#include "mmiwre_export.h"
#endif
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#include "mmiidle_statusbar.h"
#include "mmimultim_image.h"
#include "mmisrvaud_api.h"

#ifdef BT_PBAP_SUPPORT
#include "mmipb_export.h"
#include "mmipb_app.h"
#include "mmipb_common.h"
#include "mmipb_text.h"
#include "mmifilearray_export.h"
//#include "mmipb_task.h"
#endif

#ifdef BLUETOOTH_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#ifdef BT_PBAP_SUPPORT 
#define BT_PBAP_TIMER_OUT            (30*1000)       //同步联系人请求响应过时保护时长
LOCAL uint8 s_bt_connect_timeout_timer = 0;
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                */
/*---------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
#ifdef BT_HID_SUPPORT
static BOOLEAN s_bt_keyboard_enable = FALSE;
#endif

/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription: Handle blue tooth main menu win msg  
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleMainMenuWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          );

#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Discription: create list menu by current operation
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return: 
/*****************************************************************************/
LOCAL void MMIBT_CreateMainMenuList(
                    MMI_WIN_ID_T             win_id, //IN: list menu's window id
                    MMI_CTRL_ID_T            ctrl_id, //IN: list menu's id
                    MMI_TEXT_ID_T            text_id //IN: list menu titles's text id
                               );
#endif

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
//  Description : MMIBT_UpdateBtSettingWin
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
LOCAL void  MMIBT_UpdateBtSettingWin(BOOLEAN is_update);

/*****************************************************************************/
//  Discription: Handle Bt setting win msg  
//  Global resource dependence: none 
//  Author: yang.yang 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBtSettingWinMsg(
										 MMI_WIN_ID_T win_id, 
										 MMI_MESSAGE_ID_E msg_id, 
										 DPARAM param
										 );

/*****************************************************************************/
//  Discription: Handle BtPhone main win msg  
//  Global resource dependence: none 
//  Author: yang.yang 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleBtPhoneWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          );

#endif

#ifdef BT_PBAP_SUPPORT
/*****************************************************************************/
//  Description : HandleBtContactSyncQueryWin
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBtContactSyncQueryWin(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIBT_HandlePubwinConnectWaitingMsg(
                                          MMI_WIN_ID_T         win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubwinSyncWaitingMsg(
                                          MMI_WIN_ID_T         win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );

/*****************************************************************************/
//  Description : open query win
//  Global resource dependence : 
//  Author: yanyan.an
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_OpenQueryWin(MMI_TEXT_ID_T text_id, MMIPUB_HANDLE_FUNC func);

/*****************************************************************************/
//  Description : MMIBT_CreateTimer
//  Global resource dependence : 
//  Author: yanyan.an
//  Note: 创建超时定时器
/*****************************************************************************/
LOCAL void MMIBT_CreateTimer(void);

/******************************************************************************/
// Description: MMIBT_ProcessTimerOver
// Global resource dependence: 
// Author: yanyan.an
// Note: 超时保护机制
/******************************************************************************/
LOCAL void MMIBT_ProcessTimerOver(uint8 timer_id, uint32 param);

#endif

#if defined(MMI_SMS_SYN_SUPPORT) && defined(BT_SPP_SUPPORT)
/*****************************************************************************/
//  Description : HandleBtContactSyncQueryWin
//  Global resource dependence :                                
//  Author: yang.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBtSMSSyncQueryWin(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
#endif

#if defined(BT_MAP_SUPPORT)
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
//void MMISMS_SMSMapParserCallback(MMISMS_MAP_PARSER_RESULT_T *result);
#endif

#ifdef BT_HID_SUPPORT
/*****************************************************************************/
//  Description : HandleBTHIDCamMainMenuWinMsg
//  Global resource dependence : 
//  Author:wuding.yang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleBTHIDCamMainMenuWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          );

/*****************************************************************************/
//  Description : HandleBtCameraPanelWinMsg
//  Global resource dependence : 
//  Author:wuding.yang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleBtCameraPanelWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          );
#endif

/*******************************************************************************/
//window table define
WINDOW_TABLE(MMIBT_MAIN_MENU_WIN_TAB) = 
{ 
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
        WIN_STATUSBAR,
#endif
        WIN_FUNC((uint32) HandleMainMenuWinMsg ),    
        WIN_ID(MMIBT_MAIN_MENU_WIN_ID),
        WIN_TITLE(TXT_BLUETOOTH),
#ifndef MMI_GUI_STYLE_TYPICAL
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIBT_MAIN_LIST_CTRL_ID),
#endif
#ifndef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
        END_WIN    
        
};

#ifdef BT_DIALER_SUPPORT
#ifdef BT_DIALER2_SUPPORT
WINDOW_TABLE(MMIBT_BTPHONE_MAIN_WIN_TAB) = 
{ 
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
	WIN_STATUSBAR,
#endif
        WIN_FUNC((uint32) HandleBtPhoneMainWinMsg ),    
        WIN_ID(MMIBT_BTPHONE_MAIN_WIN_ID),
        WIN_TITLE(TXT_TOOLS_BTPHONE),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIBT_BTPHONE_MAIN_LIST_CTRL_ID),
#ifndef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
        END_WIN    
        
};
#endif

//BtPhone main window
WINDOW_TABLE(MMIBT_BTPHONE_WIN_TAB) = 
{ 
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
        WIN_STATUSBAR,
#endif
        WIN_FUNC((uint32) HandleBtPhoneWinMsg ),    
        WIN_ID(MMIBT_BTPHONE_WIN_ID),
        WIN_TITLE(TXT_TOOLS_BTPHONE),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIBT_BTPHONE_LIST_CTRL_ID),
#ifndef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
        END_WIN    
        
};

//bluetooth setting window
WINDOW_TABLE(MMIBT_SETTING_WIN_TAB) = 
{
	    WIN_FUNC((uint32)HandleBtSettingWinMsg),
	    WIN_ID(MMIBT_SETTING_WIN_ID),
	    WIN_TITLE(STXT_MAIN_SETTING),
	    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIBT_SETTING_LIST_CTRL_ID),
	    END_WIN
};

#ifdef BT_HID_SUPPORT//bluetooth CameraPanel
WINDOW_TABLE(MMIBT_CAMERA_PANEL_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleBtCameraPanelWinMsg),
    WIN_ID(MMIBT_CAMERA_PANEL_WIN_ID),
    WIN_TITLE(TXT_BT_CAM_PANEL),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIBT_BT_CAMERA_PANEL_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIBT_HID_CAMERA_WIN_TAB) = 
{ 
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
        WIN_STATUSBAR,
#endif
        WIN_FUNC((uint32) HandleBTHIDCamMainMenuWinMsg),    
        WIN_ID(MMIBT_CAMERA_HID_WIN_ID),
#ifndef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
        END_WIN
};

#endif

#endif

/*****************************************************************************/
//  Discription: Get the show string of lock status 
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleMainMenuWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          )
{
#ifdef MMI_GUI_STYLE_TYPICAL
    MMI_MENU_ID_T               menu_id = 0;
#endif
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
#ifndef MMI_GUI_STYLE_TYPICAL
    uint16                     cur_index = 0;
#endif
    BT_ADDRESS                  dev_addr = {0};
   
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_GUI_STYLE_TYPICAL
        GUILIST_SetMaxItem(MMIBT_MAIN_LIST_CTRL_ID,MMIBT_MAIN_MEUN_NUM, FALSE);
        MMIBT_UpdateMainMenu(FALSE);
        MMK_SetAtvCtrl(win_id, MMIBT_MAIN_LIST_CTRL_ID);        
#else
        MMK_SetAtvCtrl(win_id, MMIBT_MAIN_MENU_CTRL_ID);
#endif
        //Register Event
        break;

#ifndef MMI_GUI_STYLE_TYPICAL
     case MSG_CTL_LIST_CHECK_CONTENT:
        cur_index =  GUILIST_GetCurItemIndex(MMIBT_MAIN_LIST_CTRL_ID);
        switch(cur_index)
        {
        case BT_INDEX_OPEN:
        case BT_INDEX_VISIBLE:
        case BT_INDEX_HELP:
            MMK_SendMsg(win_id, MSG_APP_OK,PNULL);
            break;
            
        default:
            break;
        }
         break;
#endif
        
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifndef MMI_GUI_STYLE_TYPICAL
    cur_index =  GUILIST_GetCurItemIndex(MMIBT_MAIN_LIST_CTRL_ID);
    switch(cur_index)
    {
        case BT_INDEX_OPEN:
            if (BT_GetState())
            {
#ifdef BT_DIALER_SUPPORT           
                if(MMIBT_IsConnectDevice(&dev_addr))
                {
                    MMIPUB_OpenAlertWarningWin(TXT_BT_DEVICE_CONNECT_ALERT);
                }
                else
#endif
                {
//lzk mark
#ifdef BT_DIALER_SUPPORT
                    MMIBT_SetOpertor(MMIBT_OPC_MAIN, MMIBT_OPC_POWER_ONOFF);  //MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_POWER_ONOFF);
#else
                    MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_POWER_ONOFF);
#endif
                     MMIBT_OpenBluetooth(FALSE, TRUE);
                }
            }
            else
            {
                MMIBT_SetOpertor(MMIBT_OPC_MAIN, MMIBT_OPC_POWER_ONOFF);
                MMIBT_OpenBluetooth(TRUE, TRUE);
            }            
            break;

        case BT_INDEX_VISIBLE:
            if(MMIBT_GetIsDeviceVisible())
            {
                DoOperation(MMIBT_OPC_HIDE,FALSE);
            }
            else
            {
                DoOperation(MMIBT_OPC_VISIBLE,FALSE);
            }
            break;

        case BT_INDEX_PAIRES:
            {
                MMIBT_SetOpertor(MMIBT_OPC_MAIN, MMIBT_OPC_LIST_PAIRED);
                MMIBT_EnterDeviceListWin(PNULL,MMIBT_GetOpertor().root_opertion,BT_SERVICE_ALL);
            }
            break;

#ifndef PDA_UI_DROPDOWN_WIN
		case BT_INDEX_FILETRANS:
            {
         		MMIBT_OpenTransferWin();
            }
            break;
#endif

#ifdef MMIBT_FTS_SUPPORT 
        case BT_INDEX_SHARED:
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_SHARED_FILE_SETTING);
            DoOperation(MMIBT_OPC_SHARED_FILE_SETTING, FALSE);
            break;
#endif

#ifdef BT_HID_SUPPORT
            case BT_INDEX_CAMERA:
            {
                MMK_CreateWin((uint32 *)MMIBT_CAMERA_PANEL_WIN_TAB, (ADD_DATA)PNULL);
            }
            break;
#endif

        case BT_INDEX_NAME:
            MMIBT_EnterEditWin(PNULL,TRUE);
            break;

        case BT_INDEX_LOCATION:
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_SAVE_LOCATION);
            MMIBT_EnterStorageSelectWin();
            break;

        case BT_INDEX_HELP:
            MMIBT_EnterTextBoxWin(PNULL,MMIBT_OPC_HELP);
            break;
        default:
            break;
    }
    
#else
        menu_id = GUIMENU_GetCurNodeId(MMIBT_MAIN_MENU_CTRL_ID);
        switch (menu_id)
        {
        case ID_SET_BT_ON:
            //Open BT 
            {
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_POWER_ONOFF);
                MMIBT_OpenBluetooth(TRUE, TRUE);
            }
            break;
        case ID_SET_BT_OFF:
            //Close BT
            {
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_POWER_ONOFF);
                MMIBT_OpenBluetooth(FALSE, TRUE);
            }
            break;
        case ID_MY_BT_VISIBILITY:
            //"BT Visibility"
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_VISIBILITY);
            if (BT_GetState())
            {
                MMIBT_EnterSetVisibilityWin();
            }
            else
            {
                MMI_STRING_T display_str = {0};
                MMI_GetLabelTextByLang(TXT_TURN_ON_OR_NOT, &display_str);
                OpenQueryWin(MMI_BT_QUERY_WIN_ID,&display_str, PNULL, MMIBT_OPC_VISIBILITY);               
            }
            break;
        case ID_BT_DEVICE_LIST:
            //"Paired Device"
            {
                //set opeartion info
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_LIST_PAIRED);
                MMIBT_EnterDeviceListWin(PNULL,MMIBT_GetOpertor().root_opertion,BT_SERVICE_ALL);
            }
            break;
        case ID_BT_SHARED_FILE_SETTING:
            //"Set Shared File"
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_SHARED_FILE_SETTING);
            if (BT_GetState())
            {
                DoOperation(MMIBT_OPC_SHARED_FILE_SETTING, FALSE);
            }
            else
            {
                MMI_STRING_T display_str = {0};

                MMI_GetLabelTextByLang(TXT_TURN_ON_OR_NOT, &display_str);
                OpenQueryWin(MMI_BT_QUERY_WIN_ID, &display_str, PNULL, MMIBT_OPC_SHARED_FILE_SETTING);
            }
            break;
        case ID_MY_BT_DEVICE_NAME:
            //"Device Name"
            break;
        case ID_BT_SAVE_FILE_LOCATION:
            //"Save File Location"
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_SAVE_LOCATION);
            MMIBT_EnterStorageSelectWin();
            break;
        case ID_BT_HELP://device list 
            //"Help"
            MMIBT_EnterTextBoxWin(PNULL,MMIBT_OPC_HELP);
            break;
        default:
            break;
        }
#endif
        break;
        
        case MSG_GET_FOCUS: 
            break;
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            //UnRegister event
            MMK_DestroyDynaCtrl(MMIBT_MAIN_MENU_CTRL_ID);
            break;
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}
#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: maryxiao
//  Note:
//
/*****************************************************************************/

MMI_TEXT_ID_T MMIBT_GetLocation(void)
{
    MMIFILE_DEVICE_E dev    = MMI_DEVICE_UDISK;
    MMI_TEXT_ID_T    text_id = {0};
    MMIBT_USER_SETTING_U setting = {0};/*lint !e64*/

   MMIBT_GetFileUserSetting(MMINV_BT_FILE_LOCATION, &setting);

        do
        {
            if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(dev), MMIAPIFMM_GetDevicePathLen(dev)))
    		{
    			if(dev== setting.filedev)
    			{
    				text_id = MMIAPIFMM_GetDeviceName(dev);
    			}
    		}
        }while(++dev < MMI_DEVICE_NUM);
    
     
     return text_id;
} 
/*****************************************************************************/
//  Discription: when bt is off set function inavailable, else set function 
//  available
//  Global resource dependence: none 
//  Author: maryxiao
//  Note:
//
/*****************************************************************************/
LOCAL void MMIBT_SetFunctionGray(BOOLEAN isgray)
{
     int     i = 0;

     for(i=BT_INDEX_VISIBLE; i<BT_INDEX_HELP; i++)
     {
        GUILIST_SetItemGray(MMIBT_MAIN_LIST_CTRL_ID, i, isgray);
        GUILIST_SetItemInvalid(MMIBT_MAIN_LIST_CTRL_ID, i, isgray);
     }
	 
#ifndef PDA_UI_DROPDOWN_WIN
	 if(FALSE == isgray)
 	 {
		if(!MMIBT_IsOnTransferStatus())
		{
			GUILIST_SetItemGray(MMIBT_MAIN_LIST_CTRL_ID, BT_INDEX_FILETRANS, TRUE);
        	GUILIST_SetItemInvalid(MMIBT_MAIN_LIST_CTRL_ID, BT_INDEX_FILETRANS, TRUE);
		}
 	 }
#endif
}
/*****************************************************************************/
//  Discription: append one line one item
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
LOCAL void AppendOneLineTextListItem(	
                            MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id,
                            uint16           pos,
                            BOOLEAN          is_update
                            )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;

    item_t.item_data_ptr = &item_data;
    item_data.softkey_id[0] = TXT_COMMON_OK;
    item_data.softkey_id[2] = STXT_RETURN;
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;
    
    if( is_update )
    {
        GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
    }
    else
    {
        GUILIST_AppendItem( ctrl_id, &item_t );
    }
}
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_UpdateMainMenu(BOOLEAN is_init)
{
    MMI_IMAGE_ID_T              imgae_id = IMAGE_NULL;
    uint16                   device_name[BT_DEVICE_NAME_SIZE] = {0};
    MMI_STRING_T             str_info={0};
    wchar                    device_wname[BT_DEVICE_NAME_SIZE] = {0};
    MMI_IMAGE_ID_T              imgae_icon_id = IMAGE_NULL;
    BOOLEAN                     is_bt_on = FALSE;
    MMI_TEXT_ID_T               left_text_id = TXT_NULL;
    
    if (BT_GetState())
    {
        imgae_id = IMAGE_CHECK_SELECTED_ICON;
        imgae_icon_id = IMAGE_COMMON_RIGHT_ARROW;
        is_bt_on = TRUE;
        left_text_id = TXT_COMMON_CLOSE;
    }
    else
    {
         imgae_id = IMAGE_CHECK_UNSELECTED_ICON;
         imgae_icon_id = IMAGE_COMMON_RIGHT_ARROW;
         left_text_id = TXT_OPEN;
    }
    MMIBT_AppendOneLineTextIconListItem(MMIBT_MAIN_LIST_CTRL_ID,TXT_BLUETOOTH,imgae_id,BT_INDEX_OPEN,left_text_id,is_init);

    if(MMIBT_GetIsDeviceVisible())
    {    
        if(is_bt_on)
        {
            imgae_id = IMAGE_CHECK_SELECTED_ICON;   
        }
        else
        {
            imgae_id = IMAGE_CONTROL_LIST_CHECK_DISABLED;
        }
		left_text_id = TXT_COMMON_CLOSE;
    }
    else
    {
        if(is_bt_on)
        {
            imgae_id = IMAGE_CHECK_UNSELECTED_ICON;
        }
        else
        {
            imgae_id = IMAGE_CHECK_UNSELECTED_ICON_GRAY;
        }
		left_text_id = TXT_OPEN;
	}
    MMIBT_AppendOneLineTextIconListItem(MMIBT_MAIN_LIST_CTRL_ID,TXT_MY_BT_VISIBILITY,imgae_id,BT_INDEX_VISIBLE,left_text_id,is_init);

    AppendOneLineTextListItem(MMIBT_MAIN_LIST_CTRL_ID,TXT_BT_DEVICE_LIST,BT_INDEX_PAIRES,is_init);

#ifndef PDA_UI_DROPDOWN_WIN
	AppendOneLineTextListItem(MMIBT_MAIN_LIST_CTRL_ID,TXT_BT_TRANSFER_FILES,BT_INDEX_FILETRANS,is_init);
#endif

#ifdef MMIBT_FTS_SUPPORT
    AppendOneLineTextListItem(MMIBT_MAIN_LIST_CTRL_ID,TXT_BT_SET_SHARED_FILE,BT_INDEX_SHARED,is_init);
#endif

#ifdef BT_HID_SUPPORT
    //HID Camera
    AppendOneLineTextListItem(MMIBT_MAIN_LIST_CTRL_ID,TXT_BT_CAMERA,BT_INDEX_CAMERA,is_init);
#endif

    BT_GetLocalName(device_name);
    MMIAPICOM_StrToWstr(device_name, device_wname);/*lint !e64*/
    str_info.wstr_len = MMIAPICOM_Wstrlen(device_name);
    str_info.wstr_ptr = device_name;
    MMIBT_AppendTwoLineTextIconSmallTextListItem(MMIBT_MAIN_LIST_CTRL_ID,TXT_MY_BT_DEVICE_NAME,imgae_icon_id,str_info,BT_INDEX_NAME,is_init);

    str_info.wstr_len = 0;
    str_info.wstr_ptr = PNULL;
    MMI_GetLabelTextByLang(MMIBT_GetLocation(), &str_info);
    MMIBT_AppendTwoLineTextIconSmallTextListItem(MMIBT_MAIN_LIST_CTRL_ID,TXT_BT_SAVE_FILE_LOCATION,imgae_icon_id,str_info,BT_INDEX_LOCATION,is_init);

    AppendOneLineTextListItem(MMIBT_MAIN_LIST_CTRL_ID,TXT_HELP,BT_INDEX_HELP,is_init);

    if(is_bt_on)
    {
        MMIBT_SetFunctionGray(FALSE);
    }
    else
    {
        MMIBT_SetFunctionGray(TRUE);
    }

    if(is_init)
    {
        if (MMK_IsFocusWin(MMIBT_MAIN_MENU_WIN_ID))
        {
            MMK_SendMsg(MMIBT_MAIN_MENU_WIN_ID, MSG_FULL_PAINT, PNULL);
        }
    }
}
#endif
/*****************************************************************************/
//  Discription: Open the blue tooth main menu win  
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_OpenMainMenuWin(void)
{
    //设置蓝牙的主操作信息
    MMIBT_SetOpertor(MMIBT_OPC_MAIN, MMIBT_OPC_MAIN);   
    //创建主菜单窗口
    MMK_CreateWin((uint32 *)MMIBT_MAIN_MENU_WIN_TAB, NULL);
    //创建主窗口的菜单
#ifdef MMI_GUI_STYLE_TYPICAL
    MMIBT_CreateMainMenuList(MMIBT_MAIN_MENU_WIN_ID, MMIBT_MAIN_MENU_CTRL_ID, TXT_BLUETOOTH);
#endif
}

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
//  Discription: Open the BtPhone main win  
//  Global resource dependence: none 
//  Author: yang.yang 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_OpenBtPhoneWin(void)
{
	//设置蓝牙的主操作信息
    MMIBT_SetOpertor(MMIBT_OPC_MAIN, MMIBT_OPC_MAIN);   
    //创建主窗口
#ifdef BT_DIALER2_SUPPORT
    MMK_CreateWin((uint32 *)MMIBT_BTPHONE_MAIN_WIN_TAB, NULL);
#else
    MMK_CreateWin((uint32 *)MMIBT_BTPHONE_WIN_TAB, NULL);
#endif
}
#endif

#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Discription: create list menu by current operation
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return: 
/*****************************************************************************/
LOCAL void MMIBT_CreateMainMenuList(
                    MMI_WIN_ID_T             win_id, //IN: list menu's window id
                    MMI_CTRL_ID_T            ctrl_id, //IN: list menu's id
                    MMI_TEXT_ID_T            text_id //IN: list menu titles's text id
                               )
{
	GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    MMI_STRING_T    kstring = {0};
    uint32          node_id = 0;
    uint16           i = 0;
    uint32 s_bt_menu_main[MMIBT_MAIN_MEUN_NUM][2] ={
                                                    {ID_SET_BT_ON,              TXT_BT_SET_ON},//Set "BT ON" items
                                                    {ID_MY_BT_VISIBILITY,       TXT_MY_BT_VISIBILITY},//Set "BT Visibility" items
                                                    {ID_BT_DEVICE_LIST,         TXT_BT_DEVICE_LIST},//set "Paired Device" item
#ifdef MMIBT_FTS_SUPPORT
                                                    {ID_BT_SHARED_FILE_SETTING, TXT_BT_SET_SHARED_FILE},//set "Shared file Setting" item
#endif
                                                    {ID_MY_BT_DEVICE_NAME,      TXT_MY_BT_DEVICE_NAME},//set "BT Name" item
                                                    {ID_BT_SAVE_FILE_LOCATION,  TXT_BT_SAVE_FILE_LOCATION},//set "BT save file location" item
                                                    {ID_BT_HELP,                TXT_HELP}//set "Help" item
                                                    };
    GUIMENU_DYNA_ITEM_T     node_item = {0};

    if(!MMK_IsOpenWin(win_id))
    {
        SCI_TRACE_LOW("MMIBT_CreateMainMenuList window already opened !");
        return;
    }

    //creat dynamic menu
    GUIMENU_CreatDynamic(&both_rect,win_id,ctrl_id,GUIMENU_STYLE_THIRD);
    //GUIAPICTRL_SetBothRect(ctrl_id, &both_rect);
    GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
	
    MMK_SetAtvCtrl(win_id, ctrl_id);
    
    //set menu title
    MMI_GetLabelTextByLang(text_id, &kstring);
    GUIMENU_SetMenuTitle(&kstring,ctrl_id);
    
        //set bt main menu items
    for(i = 0; i < MMIBT_MAIN_MEUN_NUM; i++)
    {
        if((ID_SET_BT_ON == s_bt_menu_main[i][MMIBT_MEUN_ID_INDEX]) && BT_GetState())
        {
            //bt is on need change item info to {ID_SET_BT_ON, TXT_BT_SET_ON}
            text_id = TXT_SET_BT_OFF;
            node_id = ID_SET_BT_OFF;
        }
        else
        {
            //get item info from s_bt_menu_main
            node_id = s_bt_menu_main[i][MMIBT_MEUN_ID_INDEX];
            text_id = s_bt_menu_main[i][MMIBT_MEUN_TEXT_INDEX];
        }
        MMI_GetLabelTextByLang(text_id, &kstring);
        node_item.item_text_ptr = &kstring;
        GUIMENU_InsertNode(i, node_id, 0, &node_item, ctrl_id);
    }
 }
#endif
/*****************************************************************************/
// Description : 蓝牙开关设置页面
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-6
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIBT_SetBluetoothMode (void)
{
   //建立蓝牙开关设置窗口
   MMIBT_OpenMainMenuWin();  
   //返回窗口ID;
   return MMIBT_MAIN_MENU_WIN_ID;
}

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
//  Discription: when bt is off set function inavailable, else set function 
//  available
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void MMIBT_SetItemGray(BOOLEAN is_gray)
{
	int     i = 0;
    
    for(i=BT_PHONE_INDEX_PAIRES; i<BT_PHONE_INDEX_MAX; i++)
    {
        GUILIST_SetItemGray(MMIBT_BTPHONE_LIST_CTRL_ID, i, is_gray);
        GUILIST_SetItemInvalid(MMIBT_BTPHONE_LIST_CTRL_ID, i, is_gray);
    }
}

#ifdef BT_DIALER2_SUPPORT
/*****************************************************************************/
//  Description : MMIBT_UpdateBtPhoneMainWin
//  Global resource dependence : 
//  Author:Cheney.Wu
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIBT_UpdateBtPhoneMainWin(BOOLEAN is_update)
{
    MMI_CTRL_ID_T       ctrl_id = MMIBT_BTPHONE_MAIN_LIST_CTRL_ID;
    MMI_IMAGE_ID_T      imgae_id = IMAGE_NULL;
    uint16              device_name[BT_DEVICE_NAME_SIZE] = {0};
    MMI_STRING_T        str_info={0};
    MMI_IMAGE_ID_T      imgae_icon_id = IMAGE_COMMON_RIGHT_ARROW;
    BOOLEAN             is_bt_on = FALSE;
    MMI_TEXT_ID_T       left_text_id = TXT_NULL;
    BT_ADDRESS          dev_addr = {0};
    
    AppendOneLineTextListItem(ctrl_id,TXT_BLUETOOTH,BT_PHONE_MAIN_INDEX_SETTINGS,is_update);
    AppendOneLineTextListItem(ctrl_id,TXT_BT_DEVICE_LIST,BT_PHONE_MAIN_INDEX_PAIRES,is_update);
}

/*****************************************************************************/
//  Discription: when bt is off set function inavailable, else set function 
//  available
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void MMIBT_SetSettingItemGray(BOOLEAN is_gray)
{
    int     i = 0;
    
    for(i=BT_SETTING_INDEX_VISIBLE; i<BT_SETTING_INDEX_MAX; i++)
    {
        GUILIST_SetItemGray(MMIBT_SETTING_LIST_CTRL_ID, i, is_gray);
        GUILIST_SetItemInvalid(MMIBT_SETTING_LIST_CTRL_ID, i, is_gray);
    }
}

/*****************************************************************************/
//  Discription: Handle BtPhone main win msg  
//  Global resource dependence: none 
//  Author: Cheney.Wu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleBtPhoneMainWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          )
{
	MMI_RESULT_E                result = MMI_RESULT_TRUE;
	uint8                       cur_index = 0;
	BT_ADDRESS                  dev_addr = {0};
#ifdef BT_PBAP_SUPPORT
	MMI_WIN_ID_T                query_win_id = 0;
    BOOLEAN                     is_dev_connect = FALSE;
    BT_STATUS                   register_ret = BT_ERROR;
    BT_STATUS                   connect_ret = BT_ERROR;
	MMI_STRING_T                wait_text = {0};
#endif

	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMIBT_BTPHONE_MAIN_LIST_CTRL_ID, 2, FALSE);
        MMIBT_UpdateBtPhoneMainWin(FALSE);
        MMK_SetAtvCtrl(win_id, MMIBT_BTPHONE_MAIN_LIST_CTRL_ID);        
#ifdef MMI_GUI_STYLE_MINISCREEN
        GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, FALSE);
#endif
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_FULL_PAINT:
        break;	
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        cur_index = GUILIST_GetCurItemIndex(MMIBT_BTPHONE_MAIN_LIST_CTRL_ID);
        switch(cur_index)
        {
        case BT_PHONE_MAIN_INDEX_SETTINGS:
            MMK_CreateWin((uint32*)MMIBT_SETTING_WIN_TAB, PNULL);	
            
            break;
        case BT_PHONE_MAIN_INDEX_PAIRES:
            {
                MMIBT_INQUIRE_INFO_T inq_info = {0};
                inq_info.service_type = BT_SERVICE_ALL;
                SetInquireInfo(inq_info);
                //set opeartion info
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_LIST_PAIRED);
                OpenDeviceListWin();
            }
            break;
        default:
            result = MMI_RESULT_FALSE;
            break;
            
        }
        break;
        
        case MSG_LOSE_FOCUS:
            break;
            
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        case MSG_CLOSE_WINDOW:
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    
    return result;
}

#endif

/*****************************************************************************/
//  Description : MMIBT_UpdateBtPhoneWin
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIBT_UpdateBtPhoneWin(BOOLEAN is_update)
{
    MMI_CTRL_ID_T       ctrl_id = MMIBT_BTPHONE_LIST_CTRL_ID;
    MMI_IMAGE_ID_T      imgae_id = IMAGE_NULL;
    uint16              device_name[BT_DEVICE_NAME_SIZE] = {0};
    MMI_STRING_T        str_info={0};
    MMI_IMAGE_ID_T      imgae_icon_id = IMAGE_COMMON_RIGHT_ARROW;
    BOOLEAN             is_bt_on = FALSE;
    MMI_TEXT_ID_T       left_text_id = TXT_NULL;
    BT_ADDRESS          dev_addr = {0};
	
    if (BT_GetState())
    {
        imgae_id = IMAGE_CHECK_SELECTED_ICON;
        left_text_id = TXT_COMMON_CLOSE;
        is_bt_on = TRUE;
    }
    else
    {
        imgae_id = IMAGE_CHECK_UNSELECTED_ICON;
        left_text_id = TXT_OPEN;
        is_bt_on = FALSE;
    }
    MMIBT_AppendOneLineTextIconListItem(ctrl_id,TXT_BLUETOOTH,imgae_id,BT_PHONE_INDEX_ONOFF,left_text_id,is_update);
    AppendOneLineTextListItem(ctrl_id,TXT_BT_DEVICE_LIST,BT_PHONE_INDEX_PAIRES,is_update);

#if 0 //def BT_PBAP_SUPPORT
	AppendOneLineTextListItem(ctrl_id,TXT_BT_SYNC_CONTATCTS,BT_PHONE_INDEX_CONTACT,is_update);
#endif

#if (defined MMI_SMS_SYN_SUPPORT) && (defined BT_SPP_SUPPORT)
    AppendOneLineTextListItem(ctrl_id,TXT_BT_SYNC_SMS,BT_PHONE_INDEX_SMS,is_update);
#endif
    AppendOneLineTextListItem(ctrl_id,STXT_MAIN_SETTING,BT_PHONE_INDEX_SETTING,is_update);
    AppendOneLineTextListItem(ctrl_id,TXT_BT_ABOUT,BT_PHONE_INDEX_ABOUT,is_update);

    if(is_bt_on)
    {
        MMIBT_SetItemGray(FALSE);
    }
    else
    {
        MMIBT_SetItemGray(TRUE);
    }
}

/*****************************************************************************/
//  Discription: Handle BtPhone main win msg  
//  Global resource dependence: none 
//  Author: yang.yang 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleBtPhoneWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          )
{
	MMI_RESULT_E                result = MMI_RESULT_TRUE;
	uint8                       cur_index = 0;
	BT_ADDRESS                  dev_addr = {0};
#ifdef BT_PBAP_SUPPORT
	MMI_WIN_ID_T                query_win_id = 0;
    BOOLEAN                     is_dev_connect = FALSE;
    BT_STATUS                   register_ret = BT_ERROR;
    BT_STATUS                   connect_ret = BT_ERROR;
	MMI_STRING_T                wait_text = {0};
#endif

    switch(msg_id)
    {
	    case MSG_OPEN_WINDOW:
			GUILIST_SetMaxItem(MMIBT_BTPHONE_LIST_CTRL_ID, BT_PHONE_INDEX_MAX, FALSE);
			MMIBT_UpdateBtPhoneWin(FALSE);
			MMK_SetAtvCtrl(win_id, MMIBT_BTPHONE_LIST_CTRL_ID);        
#ifdef MMI_GUI_STYLE_MINISCREEN
			GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, FALSE);
#endif
	        break;

	    case MSG_GET_FOCUS:
			break;

		case MSG_FULL_PAINT:
			break;	

        case MSG_CTL_MIDSK:
	    case MSG_CTL_OK:
	    case MSG_APP_OK:
	    case MSG_APP_WEB:
		    cur_index = GUILIST_GetCurItemIndex(MMIBT_BTPHONE_LIST_CTRL_ID);
		    switch(cur_index)
		    {
			    case BT_PHONE_INDEX_ONOFF:
		            if (BT_GetState())
		            {
		                if(MMIBT_IsConnectDevice(&dev_addr))
		                {
							MMIPUB_OpenAlertWarningWin(TXT_BT_DEVICE_CONNECT_ALERT);
						}
						else
						{
			                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_POWER_ONOFF);
			                MMIBT_OpenBluetooth(FALSE, TRUE);
						}
		            }
		            else
		            {
		                MMIBT_SetOpertor(MMIBT_OPC_MAIN, MMIBT_OPC_POWER_ONOFF);
		                MMIBT_OpenBluetooth(TRUE, TRUE);
		            }
	            	break;

				case BT_PHONE_INDEX_PAIRES:
	                {

	                    MMIBT_SetOpertor(MMIBT_OPC_MAIN, MMIBT_OPC_LIST_PAIRED);
						MMIBT_EnterDeviceListWin(PNULL,MMIBT_GetOpertor().root_opertion,BT_SERVICE_ALL);
					}
					break;

#if 0 //def BT_PBAP_SUPPORT
				case BT_PHONE_INDEX_CONTACT:
					{
						if (!MMIBT_IsConnectDevice(&dev_addr))
						{
				            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_DEVICE_DISCONNECT_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
						}
						else
						{
							MMIBT_OpenQueryWin(TXT_BT_SYNC_QUERY, HandleBtContactSyncQueryWin);
						}
					}
					//MMIAPIPB_SetAllowSync(FALSE);
					break;
#endif

#if (defined MMI_SMS_SYN_SUPPORT) && (defined BT_SPP_SUPPORT)
                case BT_PHONE_INDEX_SMS:
					{
						if (!MMIBT_IsConnectDevice(&dev_addr))
						{
				            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_DEVICE_DISCONNECT_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
						}
						else
						{
						    MMIPUB_OpenQueryWinByTextId(TXT_BT_SYNC_SMS_QUERY, IMAGE_PUBWIN_QUERY,PNULL,(MMIPUB_HANDLE_FUNC)HandleBtSMSSyncQueryWin);
						}
					}
					break;
#endif

                case BT_PHONE_INDEX_SETTING:
                    {
						MMK_CreateWin((uint32*)MMIBT_SETTING_WIN_TAB, PNULL);	
					}
                    break;

#ifdef BT_HID_SUPPORT
				case BT_PHONE_INDEX_CAMERA:
				{
				    MMK_CreateWin((uint32 *)MMIBT_CAMERA_PANEL_WIN_TAB, (ADD_DATA)PNULL);
				}
				break;
#endif

                case BT_PHONE_INDEX_ABOUT:
                    {
                        MMIBT_EnterTextBoxWin(PNULL,MMIBT_OPC_ABOUT);
					}
                    break;

				default:
		        	result = MMI_RESULT_FALSE;
		        	break;
				
		    }
            break;

#ifdef BT_PBAP_SUPPORT        
		case MSG_BT_CONTACT_SYNC_CNF:
            {
				/************************************************************************/
				/* whether Static Data List is empty or not                             */
				/************************************************************************/
				//MMIAPIPB_InitPBAPDataList();
                //MMIPB_ClearAllBtContactForPbap();

				/************************************************************************/
				/* whether phone stroage has free space or not                                          */
				/************************************************************************/
                /*if(MMIPB_ERROR_NO_SPACE == MMIAPIPB_GetPhoneStorageFreeSpace())
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PB_NV_FULL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                    break;
                }*/
                
                is_dev_connect = MMIBT_IsConnectDevice(&dev_addr);

                SCI_TRACE_LOW("[MMIBT] MSG_BT_CONTACT_SYNC_CNF is_dev_connect:%d", is_dev_connect);
               
				if(!is_dev_connect)
			    {
				    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_DEVICE_DISCONNECT_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                    break;
                }

				/************************************************************************/
				/*Registers the Phonebook Access Client                                  */
				/************************************************************************/
		        register_ret = MMIBT_PbapClientRegister();
                
                SCI_TRACE_LOW("[MMIBT] MSG_BT_CONTACT_SYNC_CNF register_ret:%d", register_ret);
                
		        if((BT_SUCCESS != register_ret) && (BT_PENDING != register_ret) && (BT_IN_USE != register_ret))
	            {
	                MMIBT_OpenAlertTipWin(TXT_BT_REGISTER_FAIL); 
	                break;
	            }
                
                /************************************************************************/
				/* whether Static Data List is empty or not                             */
				/************************************************************************/
 				MMIAPIPB_InitPBAPDataList();
                
				/************************************************************************/
				/* Create a transport connection to the specified device and issue an  
				OBEX Connect Request */
				/************************************************************************/
				if(!MMK_IsOpenWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID))
			    {
			        MMI_GetLabelTextByLang(TXT_BT_CONNECT_WAITING, &wait_text); 
				    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_PBAP_CONNECT_WAIT_WIN_ID,IMAGE_NULL,
			            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,MMIBT_HandlePubwinConnectWaitingMsg); 
                    MMIPUB_SetWinSoftkey(MMIBT_PBAP_CONNECT_WAIT_WIN_ID, TXT_NULL, STXT_CANCEL, TRUE);
                }

                MMIBT_CreatePbapSyncTimer();
                
                SCI_TRACE_LOW("dev_addr: %x %x %x %x", 
                    dev_addr.addr[0], dev_addr.addr[1], dev_addr.addr[2], dev_addr.addr[3]);
                
                connect_ret = MMIBT_PbapConnect(&dev_addr);
                
                SCI_TRACE_LOW("[MMIBT] MSG_BT_CONTACT_SYNC_CNF connect_ret:%d", connect_ret);

                if(BT_SUCCESS == connect_ret)
                {
                    MMK_SendMsg(win_id, MSG_BT_PBAP_TP_CONNECT_SUCCESS, PNULL);
                }
                else if(BT_PENDING != connect_ret)
                {
                    MMIBT_OpenAlertTipWin(TXT_BT_PBAP_CONNECT_FAIL);                    
                }
	                
		    }
			break;
		case MSG_BT_PBAP_TP_CONNECT_SUCCESS: 
            {
                BT_STATUS pull_ret = BT_ERROR;
				//BT_MSG_T  msg_body = {0};
				
#ifdef WIN32
                MMICL_SimuPBAPDataToList(0);
#else
                MMIBT_StopPbapSyncTimer();
#endif
                /************************************************************************/
                /* Initiates the OBEX "Get" operation to retrieve a phonebook object from 
				the remote Phonebook Access Server. */
                /************************************************************************/
                pull_ret = MMIBT_PbapPullPhonebook();
                
                SCI_TRACE_LOW("[MMIBT] MSG_BT_PBAP_TP_CONNECT_SUCCESS pull_ret:%d", pull_ret);

                if(MMK_IsOpenWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID))
                {
                    MMK_CloseWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID);
                }
                
				if (BT_PENDING != pull_ret)
                {
                	if(BT_PBAP_TYPE_SIM != MMIBT_GetPbapRequestType())
                    {
                    	MMIBT_OpenAlertTipWin(TXT_BT_PULL_PHONEBOOK_ERROR); 
                    	break;
                	}
                }
                
                if(!MMK_IsOpenWin(MMIBT_SYNC_WAIT_WIN_ID))
			    {
			        MMI_GetLabelTextByLang(TXT_BT_SYNC_WAITING, &wait_text); 
				    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_SYNC_WAIT_WIN_ID,IMAGE_NULL,
			            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,HandlePubwinSyncWaitingMsg); 
                    MMIPUB_SetWinSoftkey(MMIBT_SYNC_WAIT_WIN_ID, TXT_NULL, STXT_CANCEL, TRUE);
                }

#ifdef WIN32
                MMIBT_SimuNotifyPBStartParse();
#endif
				/*test +++*/
				//MMIBT_InitBtMsgT(&msg_body);
				//MMIBT_AddVcardToContact(&msg_body);
				//MMIBT_InitBtMsgT(&msg_body);
				//MMIBT_AddVcardToContact(&msg_body);
				//MMIBT_InitBtMsgT(&msg_body);
				//MMIBT_AddVcardToContact(&msg_body);
				//MMIAPIPB_SendNotifyToPB();
				//MMIBT_InitBtMsgT(&msg_body);
				//MMIBT_AddVcardToContact(&msg_body);				
				/*text ---*/
		    }
            break;
#endif
            
	    case MSG_LOSE_FOCUS:
			break;

		case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
			
		case MSG_CLOSE_WINDOW:
			break;

		default:
	        result = MMI_RESULT_FALSE;
	        break;
	}

	return result;
}

/*****************************************************************************/
//  Description : MMIBT_UpdateBtSettingWin
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
LOCAL void  MMIBT_UpdateBtSettingWin(BOOLEAN is_update)
{
    MMI_CTRL_ID_T       ctrl_id = MMIBT_SETTING_LIST_CTRL_ID;
	MMI_IMAGE_ID_T      imgae_id = IMAGE_NULL;
    uint16              device_name[BT_DEVICE_NAME_SIZE] = {0};
    wchar               device_wname[BT_DEVICE_NAME_SIZE] = {0};
    MMI_STRING_T        str_info={0};
    MMI_IMAGE_ID_T      imgae_icon_id = IMAGE_COMMON_RIGHT_ARROW;
    MMI_TEXT_ID_T       left_text_id = TXT_NULL;
#ifdef BT_DIALER2_SUPPORT
    BOOLEAN             is_bt_on = FALSE;
#endif

#ifdef BT_DIALER2_SUPPORT
    if (BT_GetState())
    {
        imgae_id = IMAGE_CHECK_SELECTED_ICON;
        left_text_id = TXT_COMMON_CLOSE;
        is_bt_on     = TRUE;
    }
    else
    {
        imgae_id = IMAGE_CHECK_UNSELECTED_ICON;
        left_text_id = TXT_OPEN;
        is_bt_on = FALSE;
    }
    AppendOneLineTextIconListItem(ctrl_id,TXT_BLUETOOTH,imgae_id,BT_SETTING_INDEX_ONOFF,left_text_id,is_update);
#endif

    if(MMIBT_GetIsDeviceVisible())
    {
        imgae_id = IMAGE_CHECK_SELECTED_ICON;
        left_text_id = TXT_COMMON_CLOSE;
    }
    else
    {
        imgae_id = IMAGE_CHECK_UNSELECTED_ICON;
        left_text_id = TXT_OPEN;
    }
    MMIBT_AppendOneLineTextIconListItem(ctrl_id,TXT_MY_BT_VISIBILITY,imgae_id,BT_SETTING_INDEX_VISIBLE,left_text_id,is_update);

    BT_GetLocalName(device_name);
    MMIAPICOM_StrToWstr(device_name, device_wname);/*lint !e64*/
    str_info.wstr_len = MMIAPICOM_Wstrlen(device_name);
    str_info.wstr_ptr = device_name;
    MMIBT_AppendTwoLineTextIconSmallTextListItem(ctrl_id,TXT_MY_BT_DEVICE_NAME,imgae_icon_id,str_info,BT_SETTING_INDEX_NAME,is_update);

    str_info.wstr_len = 0;
    str_info.wstr_ptr = PNULL;
    MMI_GetLabelTextByLang(MMIBT_GetLocation(), &str_info);
    MMIBT_AppendTwoLineTextIconSmallTextListItem(ctrl_id,TXT_BT_SAVE_FILE_LOCATION,imgae_icon_id,str_info,BT_SETTING_INDEX_LOCATION,is_update);

#ifdef MMIBT_FTS_SUPPORT
    AppendOneLineTextListItem(ctrl_id,TXT_BT_SET_SHARED_FILE,BT_SETTING_INDEX_SHARED,is_update);
#endif

#ifdef BT_DIALER2_SUPPORT
    if(is_bt_on)
    {
        MMIBT_SetSettingItemGray(FALSE);
    }
    else
    {
        MMIBT_SetSettingItemGray(TRUE);
    }
#endif
}

/*****************************************************************************/
//  Discription: Handle Bt setting win msg  
//  Global resource dependence: none 
//  Author: yang.yang 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBtSettingWinMsg(MMI_WIN_ID_T win_id, 
												MMI_MESSAGE_ID_E msg_id, 
												DPARAM param
												)
{
    uint8               cur_index = 0;
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
#ifdef BT_DIALER2_SUPPORT
    BT_ADDRESS    dev_addr = {0};
#endif

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:               
        GUILIST_SetMaxItem(MMIBT_SETTING_LIST_CTRL_ID, BT_SETTING_INDEX_MAX, FALSE);
		MMIBT_UpdateBtSettingWin(FALSE);
		MMK_SetAtvCtrl(win_id, MMIBT_SETTING_LIST_CTRL_ID); 
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
	case MSG_CTL_OK:
	case MSG_APP_OK:
	case MSG_APP_WEB:
	    cur_index = GUILIST_GetCurItemIndex(MMIBT_SETTING_LIST_CTRL_ID);
	    switch(cur_index)
	    {
#ifdef BT_DIALER2_SUPPORT
        	case BT_SETTING_INDEX_ONOFF:
	            if (BT_GetState())
	            {
	                if(MMIBT_IsConnectDevice(&dev_addr))
	                {
	                    MMIPUB_OpenAlertWarningWin(TXT_BT_DEVICE_CONNECT_ALERT);
	                }
	                else
	                {
	                    MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_POWER_ONOFF);
	                    MMIBT_OpenBluetooth(FALSE, TRUE);
	                }
	            }
	            else
	            {
	                MMIBT_SetOpertor(MMIBT_OPC_MAIN, MMIBT_OPC_POWER_ONOFF);
	                MMIBT_OpenBluetooth(TRUE, TRUE);
	            }
	            MMIBT_UpdateBtSettingWin(TRUE);
	            break;
#endif
		    case BT_SETTING_INDEX_VISIBLE:
	            if(MMIBT_GetIsDeviceVisible())
                {
                    DoOperation(MMIBT_OPC_HIDE,FALSE);
                }
                else
                {
                    DoOperation(MMIBT_OPC_VISIBLE,FALSE);
                }
            	break;

            case BT_SETTING_INDEX_NAME:
                MMIBT_EnterEditWin(PNULL,TRUE);
                break;
            
		    case BT_SETTING_INDEX_LOCATION:
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_SAVE_LOCATION);
                MMIBT_EnterStorageSelectWin();
                break;
                
#ifdef MMIBT_FTS_SUPPORT
            case BT_SETTING_INDEX_SHARED:
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_SHARED_FILE_SETTING);
                DoOperation(MMIBT_OPC_SHARED_FILE_SETTING, FALSE);
                break;
#endif

			default:
	        	result = MMI_RESULT_FALSE;
	        	break;
	    }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

#endif

#ifdef BT_PBAP_SUPPORT
/*****************************************************************************/
//  Description : HandleBtContactSyncQueryWin
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBtContactSyncQueryWin(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    uint32       dev_service = 0;
    BT_ADDRESS   dev_addr = {0};
    switch(msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            MMIAPIBT_GetHfuConnectedAddress(&dev_addr);
            if(BT_HfuGetConnStatus(&dev_addr))
            {
                dev_service = BT_GetPairedDeviceService(&dev_addr);
                if(BT_SERVICE_PBAP_SERVER & dev_service)
                {
            		MMIBT_SetPbapRequestType(BT_PBAP_TYPE_PB);
            		MMK_SendMsg(MMK_GetPrevWinId(win_id), MSG_BT_CONTACT_SYNC_CNF, PNULL);
                } 
				else
                {
                    MMIBT_OpenAlertTipWin(TXT_BT_PBAP_NOT_SUPPORT); 
                }
            }
            MMK_CloseWin(win_id);  
        }
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        break;
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
    return result;
}

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIBT_HandlePubwinConnectWaitingMsg(
                                          MMI_WIN_ID_T         win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
        MMIBT_StopPbapSyncTimer();
	    MMIBT_PbapClientTpDisconnect();
	    MMK_CloseWin(win_id);
	    break;
    
    case MSG_APP_RED:
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;  
}

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubwinSyncWaitingMsg(
                                          MMI_WIN_ID_T         win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMIBT_PbapAbortClient();
        MMIBT_SetPbapCancelValue(TRUE);
	    MMK_CloseWin(win_id);
        MMIPUB_OpenAlertSuccessWin(STXT_CANCEL);
        break;
     
    case MSG_APP_RED:
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;  
}

/*****************************************************************************/
//  Description : open query win
//  Global resource dependence : 
//  Author: yanyan.an
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_OpenQueryWin(MMI_TEXT_ID_T text_id, MMIPUB_HANDLE_FUNC    func)
{
	MMIPUB_OpenQueryWinByTextId(text_id, IMAGE_PUBWIN_QUERY,PNULL,(MMIPUB_HANDLE_FUNC)func);
}

/*****************************************************************************/
//  Description : MMIBT_CreateTimer
//  Global resource dependence : 
//  Author: yanyan.an
//  Note: 创建超时定时器
/*****************************************************************************/
PUBLIC void MMIBT_CreatePbapSyncTimer(void)
{
    SCI_TRACE_LOW("[MMIBT] MMIBT_CreatePbapSyncTimer enter");
    
    if(s_bt_connect_timeout_timer != 0) 
    {
        MMIBT_StopPbapSyncTimer();
    }
    
    s_bt_connect_timeout_timer = MMK_CreateTimerCallback(BT_PBAP_TIMER_OUT, MMIBT_ProcessTimerOver, 0, FALSE);
    
    SCI_TRACE_LOW("[MMIBT] MMIBT_CreatePbapSyncTimer exit, timer_id = %d", s_bt_connect_timeout_timer);
}

/******************************************************************************/
// Description: MMIBT_StopTimer
// Global resource dependence: 
// Author: yanyan.an
// Note: 停止超时定时器
/******************************************************************************/
PUBLIC void MMIBT_StopPbapSyncTimer(void)
{
    SCI_TRACE_LOW("[MMIBT] MMIBT_StopPbapSyncTimer enter");
    
    if(s_bt_connect_timeout_timer != 0)    
    {   
        MMK_StopTimer(s_bt_connect_timeout_timer);   
        s_bt_connect_timeout_timer = 0;    
    }   
    
    SCI_TRACE_LOW("[MMIBT] MMIBT_StopPbapSyncTimer exit, timer_id = %d", s_bt_connect_timeout_timer);
}

/******************************************************************************/
// Description: MMIBT_ProcessTimerOver
// Global resource dependence: 
// Author: yanyan.an
// Note: 超时保护机制
/******************************************************************************/
LOCAL void MMIBT_ProcessTimerOver(uint8 timer_id, uint32 param)
{
    SCI_TRACE_LOW("[MMIBT] MMIBT_ProcessTimerOver enter");

    MMIBT_StopPbapSyncTimer();
    MMK_CloseWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID);
    MMIPUB_OpenAlertWarningWin(TXT_BT_TIMEOUT_ALERT);
    MMIBT_PbapClientTpDisconnect();
}
#endif

#if defined(MMI_SMS_SYN_SUPPORT) && defined(BT_SPP_SUPPORT)
/*****************************************************************************/
//  Description : HandleBtContactSyncQueryWin
//  Global resource dependence :                                
//  Author: yang.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleBtSMSSyncQueryWin(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    switch(msg_id)
    {
		case MSG_CTL_OK:
		case MSG_APP_OK:
		case MSG_APP_WEB:
			{
                MMIBT_SMSSyncBegin();
                MMK_CloseWin(win_id);  
            }
			break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            break;
        default:
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;
    }
    return result;
}
#endif

/*****************************************************************************/
//  Description : MMIBT_HandleBTKey
//  Global resource dependence : 
//  Author:wuding.yang
//  Note: 
/*****************************************************************************/
extern BOOLEAN IPHONE_DEVICE;
void MMIBT_HandleBTKey(uint32 keys_status,uint32 key_code)
{
#ifndef WIN32
#ifdef BT_HID_SUPPORT
    uint32 change_key_code = key_code;
    uint32 change_key_status = keys_status;
    static uint8 takepic_key_count = 0;

    if (s_bt_keyboard_enable)//Only MMI enabled the HID/HOGP function, we will send the key value to the bluetooth
    {
        switch (key_code)//Kay Vaule from Hut1_12.pdf  10 Keyboard/Keypad Page(0x07)
        {
            case KEY_1:
            {
		  //I defined 0xF0 as the take-picture-key to our BT layer
                //The ATC task can send by BT_HIDSendBTKey.
                //This key value is defined by SPRD, NOT in the Hut1_12.pdf  10 Keyboard/Keypad Page(0x07)
                change_key_code = 0xF0;//KEY_SPRD_BT_HID_CAMREA;
            }
                break;
            case KEY_2:
                change_key_code = 0x1F;
                break;
            case KEY_3:
                change_key_code = 0x20;
                break;
            case KEY_4:
                change_key_code = 0x21;
                break;
            case KEY_5:
                change_key_code = 0x22;
                break;
            case KEY_6:
                change_key_code = 0x23;
                break;
            case KEY_7:
                change_key_code = 0x24;
                break;
            case KEY_8:
                change_key_code = 0x25;
                break;
            case KEY_9:
                change_key_code = 0x26;
                break;
            case KEY_0:
                change_key_code = 0x27;
            default:
                change_key_code = 0x27;
                break;
        }

        if (KEY_1 == key_code)//I will only send KEY_1 to BT to take picture in MMI(but NOT in AT), block other keys here to PASS the Monkey Test.
        {
            if (KEY_PRESSED == keys_status)
            {
            	  if(IPHONE_DEVICE)
            	  {
            	  	change_key_code = 0xF0;
                	BT_HIDSendBTKey(keys_status,change_key_code);
            	  }
		  else
		  {
                	change_key_code = 0x58;
                	BT_HIDSendBTKey(keys_status,change_key_code);
                	SCI_Sleep(10);
                	change_key_status = KEY_RELEASED;
                	BT_HIDSendBTKey(change_key_status,change_key_code);

                	SCI_Sleep(10);
                	change_key_code = 0x80;
                	BT_HIDSendBTKey(keys_status,change_key_code);
		  }
            }
            else if (KEY_RELEASED == keys_status)
            {
                if(IPHONE_DEVICE)
                {
                     change_key_code = 0xF0;
                }
		  else
		  {
                	change_key_code = 0x80;
		  }
		  BT_HIDSendBTKey(keys_status,change_key_code);
            }
        }
    }
#endif
#endif
}

#ifdef BT_HID_SUPPORT
/*****************************************************************************/
//  Description : HandleBTHIDCamMainMenuWinMsg
//  Global resource dependence : 
//  Author:wuding.yang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleBTHIDCamMainMenuWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          )
{
#ifdef MMI_GUI_STYLE_TYPICAL
    MMI_MENU_ID_T               menu_id = 0;
#endif
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
#ifndef MMI_GUI_STYLE_TYPICAL
    uint16                     cur_ctrl_list_index = 0;
#endif

    //SCI_TRACE_LOW("HandleBTKBDMainMenuWinMsg msg_id=0x%x",msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            s_bt_keyboard_enable = TRUE;  
         }
	break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        {
        }
        break;

        case MSG_GET_FOCUS: 
            break;

        case MSG_FULL_PAINT:
        //case MSG_PRE_FULL_PAINT:
        case MSG_END_FULL_PAINT:
        {
            BOOLEAN test_flag = FALSE;
            GUI_LCD_DEV_INFO lcd_dev_info = {0};
            MMI_IMAGE_ID_T img_id = IMAGE_MAINMENU_ICON_BTKBD_BIG;
            GUI_RECT_T bg_rect = MMITHEME_GetFullScreenRect();

            lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
            lcd_dev_info.block_id = GUI_BLOCK_MAIN;
            LCD_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR);

            test_flag = GUIRES_DisplayImg(NULL,&bg_rect,&bg_rect,MMIBT_CAMERA_HID_WIN_ID,img_id,&lcd_dev_info);

            //SCI_TRACE_LOW("HandleBTKBDMainMenuWinMsg MSG_FULL_PAINT %d ... %d %d %d %d",test_flag,bg_rect.left,bg_rect.top,bg_rect.right,bg_rect.bottom);
	}
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        case MSG_APP_RED:
            s_bt_keyboard_enable = FALSE;
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
            //MMK_DestroyDynaCtrl(MMIBT_HID_CAM_MAIN_LIST_CTRL_ID);
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return result;
}

/*****************************************************************************/
//  Description : UpdateBluetoothCamPanelWin
//  Global resource dependence : 
//  Author:wuding.yang
//  Note: 
/*****************************************************************************/
LOCAL void UpdateBluetoothCamPanelWin(void)
{
    uint8 display_index = 0;
    MMI_CTRL_ID_T ctrl_id = MMIBT_BT_CAMERA_PANEL_LIST_CTRL_ID;
	
    //AppendOneLineTextListItem(ctrl_id,TXT_BT_SPP_CAM,display_index,FALSE);
    //display_index++;

    AppendOneLineTextListItem(ctrl_id,TXT_BT_HID_CAM,display_index,FALSE);

    return;
}

/*****************************************************************************/
//  Description : HandleBtCameraPanelWinMsg
//  Global resource dependence : 
//  Author:wuding.yang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleBtCameraPanelWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
#ifndef MMI_GUI_STYLE_TYPICAL
    uint16 cur_ctrl_list_index = 0;
#endif

//#if defined (BT_SPP_SUPPORT) || defined (BT_HID_SUPPORT)
#ifdef BT_HID_SUPPORT
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMIBT_BT_CAMERA_PANEL_LIST_CTRL_ID,2,FALSE);//Two types  Camera: HID_Camera & SPP_Camera

        UpdateBluetoothCamPanelWin();

        MMK_SetAtvCtrl(win_id, MMIBT_BT_CAMERA_PANEL_LIST_CTRL_ID); 
        break;

#ifdef TOUCH_PANEL_SUPPORT
    case MSG_CTL_PENOK:
#endif
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    {
#ifndef MMI_GUI_STYLE_TYPICAL
        cur_ctrl_list_index =  GUILIST_GetCurItemIndex(MMIBT_BT_CAMERA_PANEL_LIST_CTRL_ID);
        if (cur_ctrl_list_index > 2)
        {
            cur_ctrl_list_index = 2;//double check, array index, only 2 type BT camera
        }
    
        switch(cur_ctrl_list_index)
        {
            case 0:
            {
#ifdef BT_HID_SUPPORT
                MMK_CreateWin((uint32 *)MMIBT_HID_CAMERA_WIN_TAB, (ADD_DATA)PNULL);
#endif
            }
            break;

#if 0
            case 1:
            {
#ifdef BT_SPP_SUPPORT
                MMK_CreateWin((uint32 *)MMIBT_SPP_CAMERA_WIN_TAB, (ADD_DATA)PNULL);
#endif
            }
            break;
#endif

            default:
            break;
        }
#endif
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
        MMK_CloseWin(win_id);
        break;
    
    case MSG_CLOSE_WINDOW:
        MMK_DestroyDynaCtrl(MMIBT_BT_CAMERA_PANEL_LIST_CTRL_ID);
        break;
    
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
#endif

    return result;
}
#endif

#endif
