/****************************************************************************
** File Name:     mmibt_savelocation.c                                     *
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
#define _MMIBT_SAVELOCATION_C_

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
#include "mmifmm_text.h"
#include "mmi_filemgr.h"
#include "guimenu.h"
#include "guires.h"
#include "guiedit.h"
#include "mmibt_savelocation.h"
#include "mmiudisk_export.h"
#ifdef BLUETOOTH_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIBT_SAVE_LOCATION_NUM         (MMI_DEVICE_NUM -1)
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription:  handle list box select
//  Global resource dependence: none 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleSaveLocationWinMsg(
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
LOCAL void CreateSaveLocationList(
                    MMI_WIN_ID_T             win_id,                   
                    MMI_CTRL_ID_T            ctrl_id,  
                    uint32                   service
                    );              
/*****************************************************************************/
//  Description : set opp file location
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMIBT_SETTING_RET Set_OPP_File_Location(uint16 cur_select_item);
/*****************************************************************************/
//the window for set file save location
WINDOW_TABLE(MMIBT_SET_SAVE_LOCATION_WIN_TAB) = 
{ 
    WIN_FUNC((uint32) HandleSaveLocationWinMsg ),
    WIN_ID(MMIBT_SET_SAVE_LOCATION_WIN_ID),
    WIN_TITLE(TXT_BT_SAVE_FILE_LOCATION),  
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIBT_SET_SAVE_LOCATION_CTRL_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN    
};

/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                            LOCAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription:  Handle Save Location Win Msg 
//  Global resource dependence: none 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleSaveLocationWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          )
{
    MMI_RESULT_E     result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id  = MMIBT_SET_SAVE_LOCATION_CTRL_ID;
    MMI_TEXT_ID_T prompt_text_id = TXT_NULL;
    MMI_IMAGE_ID_T promt_image_id = IMAGE_NULL;
    uint16        cur_select_item = 0;
    MMIBT_SETTING_RET    set_ret = MMIBT_SUCCESS;
    
    const MMI_TEXT_ID_T prompt_id_array[MMIBT_SETTING_NUM] = 
    {
        TXT_COMPLETE,
        TXT_COMMON_UDISK_USING,
        TXT_FOLDER_NOT_EXIST,
        TXT_UDISK_NO_EXIST,
        TXT_SD_NO_EXIST,
        TXT_UDISK_SD_NOT_EXIST,
    	TXT_COMMON_SD_FORMAT_ERROR,
        TXT_BT_FTP_BUSY,
        TXT_ERROR
    };
    
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
        //Select
            cur_select_item = GUILIST_GetCurItemIndex(ctrl_id);
            {
                //file location
                set_ret = Set_OPP_File_Location(cur_select_item);
            }
            if(set_ret < MMIBT_SETTING_NUM)
            {
                //need prompt user result
                prompt_text_id = prompt_id_array[set_ret];
                if(MMIBT_SUCCESS == set_ret)
                {
                    promt_image_id = IMAGE_PUBWIN_SUCCESS;
                }
                else
                {
                    promt_image_id = IMAGE_PUBWIN_WARNING;
                }
                if(TXT_COMPLETE != prompt_text_id)
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,prompt_text_id,TXT_NULL,promt_image_id,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);            
                }
                //Refresh list
#ifdef MMI_GUI_STYLE_TYPICAL
                MMK_DestroyDynaCtrl(ctrl_id);
                CreateSaveLocationList(win_id, ctrl_id, BT_SERVICE_ALL);   /*lint !e64*/ 
#endif                 
            }  
            MMK_CloseWin(win_id);
        break;

        case MSG_GET_FOCUS: 
            MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
        break;
        case MSG_CLOSE_WINDOW:
            MMK_DestroyDynaCtrl(ctrl_id);
        break;
        default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Discription: Open storage select win 
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_EnterStorageSelectWin(void)
{
    // BOOLEAN         exited_udisk = FALSE;
    MMIBT_OPERATION_INFO_T operation_info = MMIBT_GetOpertor();
    
    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return;
    }

    if (!MMIAPIUDISK_IsDeviceReady())
    {
        return;
    }
#ifdef MMI_GUI_STYLE_TYPICAL
    if (!BT_GetState())
    {   
        MMI_STRING_T display_str = {0};
        MMI_GetLabelTextByLang(TXT_TURN_ON_OR_NOT, &display_str);
        OpenQueryWin(MMI_BT_QUERY_WIN_ID, &display_str, PNULL, operation_info.current_opertion);             
    }
    else
#endif
    {
#ifndef MMI_GUI_STYLE_TYPICAL
        MMK_CreatePubListWin((uint32 *)MMIBT_SET_SAVE_LOCATION_WIN_TAB, PNULL);
#else
        MMK_CreateWin((uint32 *)MMIBT_SET_SAVE_LOCATION_WIN_TAB, PNULL);
#endif        
        CreateSaveLocationList(MMIBT_SET_SAVE_LOCATION_WIN_ID, MMIBT_SET_SAVE_LOCATION_CTRL_ID,BT_SERVICE_ALL);
    }

    return;
}
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void CreateSaveLocationList(
                    MMI_WIN_ID_T             win_id,                   
                    MMI_CTRL_ID_T            ctrl_id,  
                    uint32                   service
                    )
{
    GUILIST_INIT_DATA_T init_data = {0};
    MMIBT_USER_SETTING_U setting = {0};/*lint !e64*/
    uint16        selected_index = 0;
	uint16        cur_index = 0;
    MMI_HANDLE_T  ctrl_handle = 0;
    MMIFILE_DEVICE_E dev = MMI_DEVICE_UDISK;
    //create dyna list box

    MMIBT_SetCurrentSevice(service);
    if(!MMK_IsOpenWin(win_id))
    {
        //SCI_TRACE_LOW:"CreateList window already opened !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8621_112_2_18_2_8_18_306,(uint8*)"");
        return;
    }
    init_data.both_rect = MMITHEME_GetWinClientBothRect(win_id);
    init_data.type = GUILIST_RADIOLIST_E;
    ctrl_handle = GUILIST_CreateListBox(win_id, 0, ctrl_id, &init_data);
    
    GUILIST_SetMaxItem(ctrl_id,MMIBT_SAVE_LOCATION_NUM, FALSE);
    MMIBT_GetFileUserSetting(MMINV_BT_FILE_LOCATION, &setting);
    
    do
    {
        if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(dev), MMIAPIFMM_GetDevicePathLen(dev)))
		{		    
			MMIBT_AppendStringListItem(
				MMIAPIFMM_GetDeviceName(dev),
				GUIITEM_STYLE_ONE_LINE_RADIO,
				STXT_SELECT,
				TXT_NULL,
				STXT_RETURN,
				ctrl_id
				);

			if(dev== setting.filedev)
			{
				selected_index = cur_index;
			}
			cur_index++;
		}
    }while(++dev < MMI_DEVICE_NUM);
    GUILIST_SetCurItemIndex(ctrl_id,selected_index);
    GUILIST_SetSelectedItem(ctrl_id, selected_index, TRUE);
}
/*****************************************************************************/
//  Description : set opp file location
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMIBT_SETTING_RET Set_OPP_File_Location(uint16 cur_select_item)
{
    MMIBT_SETTING_RET    result  = MMIBT_SUCCESS;
    MMIBT_USER_SETTING_U setting = {0};/*lint !e64*/
    //MMIFILE_DEVICE_E     dev     = MMI_DEVICE_UDISK;
#ifndef MMI_GUI_STYLE_TYPICAL
    MMI_STRING_T           display_str ={0};
#endif
    MMIFILE_DEVICE_E dev_ok_type[MMI_DEVICE_NUM + 1] = {0};
    uint16        cur_index = 0;
    MMIFILE_DEVICE_E dev    = MMI_DEVICE_UDISK;
    
    if(cur_select_item >= MMIBT_SAVE_LOCATION_NUM)
    {
        //SCI_TRACE_LOW:"MMIBT_OPC_SAVE_LOCATION cur_select_item %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10550_112_2_18_2_8_22_340,(uint8*)"d",cur_select_item);
        return MMIBT_ERROR;
    }
    if(MMIAPIUDISK_UdiskIsRun())
    {
        //usb runing
        return MMIBT_UDISK_RUNING;
    }

    do
    {
        if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(dev), MMIAPIFMM_GetDevicePathLen(dev)))
    	{
    	    dev_ok_type[cur_index] = dev;
    		cur_index++;
    	}
    }while(++dev < MMI_DEVICE_NUM);
    //"Select Dir"
    dev_ok_type[cur_index] = MMI_DEVICE_NUM;
    
    if(MMIAPIFMM_GetDeviceTypeStatus(dev_ok_type[cur_select_item]))
    {
        setting.filedev = dev_ok_type[cur_select_item];  
    }
    else
    {
        setting.filedev = MMIAPIFMM_GetFirstValidDevice();    
    }

    if(MMI_DEVICE_NUM <= setting.filedev)
    {
        result = MMIBT_ERROR;
    }
   
    //selected location exist,save it
    //SCI_MEMSET(&setting, 0X00, sizeof(setting));
    MMIBT_SaveUserSetting(MMINV_BT_FILE_LOCATION, &setting);
#ifndef MMI_GUI_STYLE_TYPICAL
    if(result != MMIBT_ERROR)
    {
        MMI_GetLabelTextByLang(MMIAPIFMM_GetDeviceName(setting.filedev), &display_str);
        MMIBT_AppendTwoLineTextIconSmallTextListItem(MMIBT_MAIN_LIST_CTRL_ID,TXT_BT_SAVE_FILE_LOCATION,IMAGE_COMMON_RIGHT_ARROW,display_str,BT_INDEX_LOCATION,TRUE);
#ifdef BT_DIALER_SUPPORT
        MMIBT_AppendTwoLineTextIconSmallTextListItem(MMIBT_SETTING_LIST_CTRL_ID,TXT_BT_SAVE_FILE_LOCATION,IMAGE_COMMON_RIGHT_ARROW,display_str,BT_SETTING_INDEX_LOCATION,TRUE);
#endif
    }
#endif
    return result;           
}
#endif