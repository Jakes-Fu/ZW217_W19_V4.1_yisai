/****************************************************************************
** File Name:     mmibt_devicedetails.c                                       *
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
#define _MMIBT_DEVICEDETAILS_C_
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                     *
**---------------------------------------------------------------------------*/
#include "mmi_app_bt_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmk_timer.h"
#include "mmibt_func.h"
#include "mmipub.h"
#include "guilabel.h"
#include "bt_abs.h"
#include "bt_cfg.h"
#include "mmibt_text.h"
#include "mmi_default.h"
#include "guitext.h"
#include "mmiphone_export.h"
#include "mmibt_a2dp.h"
#include "mmibt_nv.h"
#include "mmifmm_text.h"
#include "guimenu.h"
#include "guires.h"
#include "mmibt_devicedetails.h"
#include "mmisrvfmm_export.h"
#include "mmibt_app.h"
#ifdef BLUETOOTH_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
typedef struct mmibt_devicedetails_win_param_tag
{
    BT_DEVICE_INFO  device_info;
    MMI_STRING_T    title_string;
    MMIBT_DEVICEDETAILS_FUNC_TYPE_E func_type; 
}MMIBT_DEVICEDETAILS_WIN_PARAM_T;
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription: Handle inquired device details of have ftp function win msg  
//  Global resource dependence: none 
//  Author: Kelly.li 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePairedDeviceDetailsWinMsg(
                                                    MMI_WIN_ID_T win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param 
                                                    );
/*****************************************************************************/
//  Discription: Handle paired device details win msg  
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
LOCAL  MMI_RESULT_E  OpenDeviceDetailsWin(MMIBT_DEVICEDETAILS_WIN_PARAM_T *param_ptr);
/*----------------------------------------------------------------------------*/
WINDOW_TABLE( MMIBT_HEADSET_DISCONNECTED_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandlePairedDeviceDetailsWinMsg),
    WIN_ID(MMIBT_DEVICE_DETAILS_WIN_ID),
    //WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_BT_DEACTIVE_DEVICE_DETAILS,MMIBT_DEVICE_DETAILS_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MMIBT_HEADSET_CONNECTED_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandlePairedDeviceDetailsWinMsg),
    WIN_ID(MMIBT_DEVICE_DETAILS_WIN_ID),
    //WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_BT_DEVICE_DETAILS,MMIBT_DEVICE_DETAILS_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MMIBT_NOT_HEADSET_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandlePairedDeviceDetailsWinMsg),
    WIN_ID(MMIBT_DEVICE_DETAILS_WIN_ID),
    //WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_BT_OTHER_DEVICE_DETAILS,MMIBT_DEVICE_DETAILS_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE( MMIBT_PDA_PAIRED_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandlePairedDeviceDetailsWinMsg),
    WIN_ID(MMIBT_DEVICE_DETAILS_WIN_ID),
    //WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_BT_PDA_PAIRED_DETAILS,MMIBT_DEVICE_DETAILS_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif

/*****************************************************************************/
//  Discription: Handle paired device details win msg  
//  Global resource dependence: none 
//  Author: Kelly.li 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePairedDeviceDetailsWinMsg(
                                                    MMI_WIN_ID_T win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param 
                                                    )
{
    MMI_MENU_GROUP_ID_T     group_id = 0;
    MMI_MENU_ID_T           menu_id = 0;
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id = MMIBT_DEVICE_DETAILS_MENU_CTRL_ID;
    MMI_STRING_T            wait_text = {0};
    MMI_MENU_GROUP_ID_T     sort_group_id = MENU_BT_ACCREDIT_SETTINGS_OPT;
    GUIMENU_POP_SEL_INFO_T  pop_item_info = {0};
    BOOLEAN                 be_authorize = FALSE;
    
    MMIBT_DEVICEDETAILS_WIN_PARAM_T *win_param_ptr = (MMIBT_DEVICEDETAILS_WIN_PARAM_T *)MMK_GetWinUserData(win_id);
    uint16 cur_select_item = GUILIST_GetCurItemIndex(MMIBT_DEVICELIST_LIST_CTRL_ID);
#ifdef MMI_PDA_SUPPORT 
    GUIMENU_TITLE_INFO_T   sub_title_info = {0};
    MMI_STRING_T  sub_title_str = {0};  
#endif
#ifdef BT_DIALER_SUPPORT
    BT_ADDRESS    dev_addr = {0};
	uint16        dev_type = 0;
#endif

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_PDA_SUPPORT 
        GUIMENU_SetMenuTitle(&win_param_ptr->title_string, ctrl_id);
        sub_title_info.is_static = TRUE;
        sub_title_info.group_id = ID_BT_ACCREDIT_SETTINGS;
        MMI_GetLabelTextByLang(TXT_BT_ACCREDIT_SETTINGS, &sub_title_str);
        sub_title_info.sub_title_ptr = &sub_title_str;
        sub_title_info.title_icon_id = 0;
        GUIMENU_SetMenuTitleEx(MMIBT_DEVICE_DETAILS_MENU_CTRL_ID,&sub_title_info);
#endif
        BT_GetDeviceAuthorisation(&win_param_ptr->device_info.addr,&be_authorize);
        if(be_authorize)
        {
            menu_id = ID_BT_AUTO_CONNECT;
        }
        else
        {
            menu_id = ID_BT_ASK_EVERYTIME;
        }
        pop_item_info.is_static = TRUE;
        pop_item_info.ctrl_id = ctrl_id;
        pop_item_info.menu_id = menu_id;
        pop_item_info.group_id = sort_group_id;
        GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);   
        MMK_SetAtvCtrl(win_id,ctrl_id);     
        break;
    
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        switch(menu_id) 
        {
        case ID_BT_ASK_EVERYTIME:
            BT_SetDeviceAuthorisation(&win_param_ptr->device_info.addr,FALSE);
            break;
        case ID_BT_AUTO_CONNECT:
            BT_SetDeviceAuthorisation(&win_param_ptr->device_info.addr,TRUE);
            break;
        case ID_BT_DEVICE_RENAME: 
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_DEVICE_RENAME);
            MMIBT_EnterEditWin(&win_param_ptr->device_info,FALSE);
            break;
            
        case ID_BT_DEVICE_DELETE:      
            //blue tooth is power on
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_DEVICE_DELETE);
            //if(BT_GetState())
            {
                MMI_GetLabelTextByLang(TXT_DELETE_QUERY, &wait_text);
                OpenQueryWin(MMI_BT_QUERY_WIN_ID, &wait_text, PNULL, MMIBT_OPC_DEVICE_DELETE);
            }                                    
            break;
        case ID_BT_DEVICE_DELETE_ALL:      
            //blue tooth is power on
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_DEVICE_DELETE_ALL);
            //if(BT_GetState())
            {
                MMI_GetLabelTextByLang(TXT_DELETE_ALL_QUERY, &wait_text);
                OpenQueryWin(MMI_BT_QUERY_WIN_ID, &wait_text, PNULL, MMIBT_OPC_DEVICE_DELETE_ALL);
            }                                  
            break;
        
        case ID_BT_DEVICE_DETAIL:
            MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_DEVICE_DETAILS);
            //blue tooth is power on
            MMIBT_EnterTextBoxWin(&win_param_ptr->device_info.addr,MMIBT_OPC_DEVICE_DETAILS);
            break;
        case ID_BT_DEVICE_CONNECT:
            //headset connect
            //if the mp3 is playing or mp3 pause should not permit close or open headset
#ifdef BT_DIALER_SUPPORT
            BT_GetDeviceType(&win_param_ptr->device_info, &dev_type);
            if(dev_type & BT_DEVICE_CLASS_AUDIO)
#endif
            {
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_OPEN_HEADSET);
                if(BT_GetState())
                {              
				    DoOperation(MMIBT_OPC_OPEN_HEADSET, TRUE);
                }
                else
                {
                    //blue tooth is power off
                    MMI_GetLabelTextByLang(TXT_TURN_ON_OR_NOT, &wait_text);
                    OpenQueryWin(MMI_BT_QUERY_WIN_ID, &wait_text, PNULL, MMIBT_OPC_OPEN_HEADSET);
                }
            }
#ifdef BT_DIALER_SUPPORT
            else if(dev_type & BT_DEVICE_CLASS_PHONE)
            {
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_OPEN_HANDSET_GATEWAY);
                if(BT_GetState())
                {              
                    DoOperation(MMIBT_OPC_OPEN_HANDSET_GATEWAY, TRUE);
                }
                else
                {
                    //blue tooth is power off
                    MMI_GetLabelTextByLang(TXT_TURN_ON_OR_NOT, &wait_text);
                    OpenQueryWin(MMI_BT_QUERY_WIN_ID, &wait_text, PNULL, MMIBT_OPC_OPEN_HANDSET_GATEWAY);
                }
            }
#endif

            break;
        case ID_BT_DEVICE_DISCONNECT:
#ifdef BT_DIALER_SUPPORT
            BT_GetDeviceType(&win_param_ptr->device_info, &dev_type);
            if(dev_type & BT_DEVICE_CLASS_AUDIO)
#endif
            {
                //headset connect 
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_CLOSE_HEADSET);

                if(BT_GetState())
                {
                    BT_DEVICE_INFO bt_dev_info = {0};
                    MMI_STRING_T            name_str = {0};
                    uint8   paired_device_index[MMIBT_MAX_DEV_ITEMS]= {0};

                    MMI_GetLabelTextByLang(TXT_BT_WHETHER_DISCONNECT, &wait_text);
                    SCI_MEMSET(paired_device_index,0,sizeof(paired_device_index));
                    MMIBT_GetPairedDevList(MMIBT_GetCurrentSevice(), paired_device_index);
#ifdef MMI_PDA_SUPPORT
                    BT_GetPairedDeviceInfo(MMIBT_GetCurrentSevice(), paired_device_index[cur_select_item -2], &bt_dev_info);
#else
                    BT_GetPairedDeviceInfo(MMIBT_GetCurrentSevice(), paired_device_index[cur_select_item -1], &bt_dev_info);
#endif
                    name_str.wstr_ptr = bt_dev_info.name;
                    name_str.wstr_len = MMIAPICOM_Wstrlen(bt_dev_info.name);
                    OpenQueryWin(MMI_BT_QUERY_WIN_ID, &wait_text, &name_str, MMIBT_OPC_CLOSE_HEADSET);
                 }
                 else
                 {
                     //blue tooth is power off
                     MMI_GetLabelTextByLang(TXT_TURN_ON_OR_NOT, &wait_text);
                     OpenQueryWin(MMI_BT_QUERY_WIN_ID, &wait_text, PNULL, MMIBT_OPC_CLOSE_HEADSET);
                 }
            }
#ifdef BT_DIALER_SUPPORT
            else if(dev_type & BT_DEVICE_CLASS_PHONE)
            {
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_CLOSE_HANDSET_GATEWAY);
                if(BT_GetState())
                {
                	BT_DEVICE_INFO bt_dev_info = {0};
					MMI_STRING_T            name_str = {0};
                	uint8   paired_device_index[MMIBT_MAX_DEV_ITEMS]= {0};
					
                    //BT_DEVICE_INFO bt_dev_info = {0};
                    MMI_GetLabelTextByLang(TXT_BT_WHETHER_DISCONNECT, &wait_text);
					SCI_MEMSET(paired_device_index,0,sizeof(paired_device_index));
	                MMIBT_GetPairedDevList(MMIBT_GetCurrentSevice(), paired_device_index);
#ifdef MMI_PDA_SUPPORT
                    BT_GetPairedDeviceInfo(MMIBT_GetCurrentSevice(), paired_device_index[cur_select_item -2], &bt_dev_info);
#else
                    BT_GetPairedDeviceInfo(MMIBT_GetCurrentSevice(), paired_device_index[cur_select_item -1], &bt_dev_info);
#endif
                    name_str.wstr_ptr = bt_dev_info.name;
                    name_str.wstr_len = MMIAPICOM_Wstrlen(bt_dev_info.name);
                    OpenQueryWin(MMI_BT_QUERY_WIN_ID, &wait_text, &name_str, MMIBT_OPC_CLOSE_HANDSET_GATEWAY);
                }
                else
                {
                    //blue tooth is power off
                    MMI_GetLabelTextByLang(TXT_TURN_ON_OR_NOT, &wait_text);
                    OpenQueryWin(MMI_BT_QUERY_WIN_ID, &wait_text, PNULL, MMIBT_OPC_CLOSE_HANDSET_GATEWAY);
                }
            }
#endif
            break;
        default:
            break;
        }
        MMK_CloseWin(win_id);
    
        break;              
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_LOSE_FOCUS:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:        
        SCI_FREEIF(win_param_ptr->title_string.wstr_ptr);
        SCI_FREEIF(win_param_ptr);
        break;
       
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return(result);
}
/*****************************************************************************/
//  Discription: Open the blue tooth device details  win   
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_EnterDeviceDetailsWin(BT_DEVICE_INFO *device_ptr,
                                                  MMI_STRING_T *string_ptr,
                                                  MMIBT_DEVICEDETAILS_FUNC_TYPE_E func_type)
{
    MMIBT_DEVICEDETAILS_WIN_PARAM_T param_info = {0};

    if(PNULL != device_ptr)
    {
        SCI_MEMCPY(&param_info.device_info, device_ptr, sizeof(BT_DEVICE_INFO));
    }
    if(PNULL != string_ptr)
    {
        param_info.title_string.wstr_ptr = string_ptr->wstr_ptr;
        param_info.title_string.wstr_len = string_ptr->wstr_len;
    }
    param_info.func_type = func_type;
    OpenDeviceDetailsWin(&param_info);    
}
/*****************************************************************************/
//  Discription: Handle paired device details win msg  
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  OpenDeviceDetailsWin(MMIBT_DEVICEDETAILS_WIN_PARAM_T *param_ptr)
{
    BOOLEAN res = MMI_RESULT_FALSE;
    MMIBT_DEVICEDETAILS_WIN_PARAM_T *win_param_ptr = PNULL;    
    MMI_HANDLE_T    win_handle = 0;
    uint32*         win_table_ptr = MMIBT_NOT_HEADSET_WIN_TAB;

    if(PNULL == param_ptr)
        return res;      

    if(MMK_IsOpenWin(MMIBT_DEVICE_DETAILS_WIN_ID))
    {
        MMK_CloseWin(MMIBT_DEVICE_DETAILS_WIN_ID);
    }

    win_param_ptr = (MMIBT_DEVICEDETAILS_WIN_PARAM_T *)SCI_ALLOC_APPZ(sizeof(MMIBT_DEVICEDETAILS_WIN_PARAM_T));
    if(PNULL != win_param_ptr)
    {
        SCI_MEMCPY(&win_param_ptr->device_info,&param_ptr->device_info,sizeof(BT_DEVICE_INFO));
        win_param_ptr->func_type = param_ptr->func_type;
        
        win_param_ptr->title_string.wstr_ptr = SCI_ALLOC_APPZ((MMIFMM_FULL_FILENAME_LEN+1)*sizeof(wchar));
        if(PNULL != win_param_ptr->title_string.wstr_ptr)
        {
            SCI_MEMCPY(win_param_ptr->title_string.wstr_ptr,param_ptr->title_string.wstr_ptr,param_ptr->title_string.wstr_len*sizeof(wchar));
            win_param_ptr->title_string.wstr_len = MMIAPICOM_Wstrlen(win_param_ptr->title_string.wstr_ptr);
        }
        
        switch(win_param_ptr->func_type)
        {
            case HEADSET_DISCONNECTED:
                win_table_ptr = MMIBT_HEADSET_DISCONNECTED_WIN_TAB;
                break;
                
            case HEADSET_CONNECTED:
                win_table_ptr = MMIBT_HEADSET_CONNECTED_WIN_TAB;
                break;
                                
#ifdef MMI_PDA_SUPPORT                
            case PDA_PAIRED_OPT:
                win_table_ptr = MMIBT_PDA_PAIRED_OPT_WIN_TAB;
                break;
#endif

            case NOT_HEADSET:
            default:
                break;
        }
        win_handle = MMK_CreateWin(win_table_ptr, (ADD_DATA)win_param_ptr);
        if(!win_handle || MMI_INVALID_ID == win_handle)
        {
            SCI_TRACE_LOW("OpenDeviceDetailsWin open win fail!");
            SCI_FREEIF(win_param_ptr->title_string.wstr_ptr);
            SCI_FREEIF(win_param_ptr);            
        }
        else
        {
            MMK_SetWinUserData(MMIBT_DEVICE_DETAILS_WIN_ID,(void *)win_param_ptr);
            res = MMI_RESULT_TRUE;
        }
    }
    SCI_TRACE_LOW("OpenDeviceDetailsWin malloc memery fail,res %d",res);

    return res;
}

#endif
