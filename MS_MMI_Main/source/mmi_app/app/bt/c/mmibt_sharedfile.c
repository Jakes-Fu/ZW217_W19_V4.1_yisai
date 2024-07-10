/****************************************************************************
** File Name:     mmibt_setvisibility.c                                     *
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
#define _MMIBT_SETVISIBILITY_C_
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
#include "mmibt_app.h"
#include "mmibt_nv.h"
#include "guimenu.h"
#include "guires.h"
#include "guiedit.h"
#include "mmibt_setvisibility.h"
#include "mmiudisk_export.h"
#ifdef BLUETOOTH_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
typedef enum
{
    MMIBT_SHARE_SETTING_AUTHORITY,
    MMIBT_SHARE_SETTING_LOCATION 
}MMIBT_SHARE_SETTING_INDEX/*lint !e751*/;
#ifdef MMIBT_FTS_SUPPORT
#define MMIBT_SHARED_AUTHORITY_NUM      2
#endif
#define MMIBT_SHARED_DIRECTORY_NUM      (MMI_DEVICE_NUM)
#ifdef MMIBT_FTS_SUPPORT
//Shared Dir Authority Items
const uint8* const s_bt_list_shared_authority[MMIBT_SHARED_AUTHORITY_NUM] = 
{
    MMIBT_FTP_RW_MODE,/*lint !e64*/
    MMIBT_FTP_RO_MODE    /*lint !e64*/
}; /*lint !e64*/
#endif
/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription:  handle list menu action
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleListMenuWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          );
#ifdef MMIBT_FTS_SUPPORT
/*****************************************************************************/
//  Discription:  handle list box select
//  Global resource dependence: none 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleSharedListBoxWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          );
#endif
/*****************************************************************************/
//  Description : set ftp shared directory
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMIBT_SETTING_RET Set_FTP_Shared_Directory(uint16 cur_select_item);
/*****************************************************************************/
//  Description : open direct and get user selected dir
//  Global resource dependence : 
//  Author: 
//  Note: after user selected a dir, it will be set to shaired directory
/*****************************************************************************/
LOCAL void MMIBT_OpenFileCallBack( BOOLEAN     is_success,
                                    FILEARRAY   file_array);
#ifdef MMIBT_FTS_SUPPORT
/*****************************************************************************/
//  Description : set ftp shared authority
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMIBT_SETTING_RET Set_FTP_Shared_Authority(uint16 cur_select_item);
#endif//MMIBT_FTS_SUPPORT
#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Discription:  get sharesetting authority 
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
MMI_TEXT_ID_T MMIBT_GetShareSettingAuthority(void);
/*****************************************************************************/
//  Discription:  get sharesetting location
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
MMI_TEXT_ID_T MMIBT_GetShareSettingLocation(void);
#endif
/*****************************************************************************/
//  Discription: 根据current_opt和service的不同，创建蓝牙中所不同的list
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void CreateSharedListBox(
                    MMI_WIN_ID_T             win_id,                   
                    MMI_CTRL_ID_T            ctrl_id,       
                    MMIBT_OPERTATION_TYPE_E  current_opt,
                    uint32                      service
                    );
#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Discription: create list menu by current operation
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return: 
/*****************************************************************************/
LOCAL void CreateSharedFileSettingMenu(
                    MMI_WIN_ID_T             win_id, //IN: list menu's window id
                    MMI_CTRL_ID_T            ctrl_id, //IN: list menu's id
                    MMI_TEXT_ID_T            text_id //IN: list menu titles's text id
                               );
#endif
/*****************************************************************************/
#ifdef MMIBT_FTS_SUPPORT
//sharing file setting window
WINDOW_TABLE(MMIBT_SHARED_FILE_WIN_TAB) = 
{ 
        WIN_STATUSBAR,
        WIN_FUNC((uint32) HandleListMenuWinMsg),    
        WIN_ID(MMIBT_SHARED_FILE_WIN_ID),
        WIN_TITLE(TXT_BT_SET_SHARED_FILE),
#ifndef MMI_GUI_STYLE_TYPICAL
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIBT_SHARE_SETTING_LIST_CTRL_ID),
#endif
#ifndef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
        END_WIN    
        
};

//sharing file authority window
WINDOW_TABLE(MMIBT_SHARED_AUTHORITY_WIN_TAB) = 
{ 
        WIN_FUNC((uint32) HandleSharedListBoxWinMsg),    
        WIN_ID(MMIBT_SHARED_AUTHORITY_WIN_ID),
        WIN_TITLE(TXT_BT_SET_SHARING_AUTHORITY),
#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif 
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIBT_LIST_BOX_SHARED_AUTHORITY_ID),
#else        
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
        END_WIN    
        
};
//sharing file directory window
WINDOW_TABLE(MMIBT_SHARED_DIRECTORY_WIN_TAB) = 
{ 
        WIN_FUNC((uint32) HandleSharedListBoxWinMsg),    
        WIN_ID(MMIBT_SHARED_DIRECTORY_WIN_ID),
        WIN_TITLE(TXT_BT_SET_SHARING_DIRECTORY),
#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif       
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIBT_LIST_BOX_SHARED_DIRECTORY_ID),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
        END_WIN
};
#endif//MMIBT_FTS_SUPPORT
/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                            LOCAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/
#ifdef MMIBT_FTS_SUPPORT
/*****************************************************************************/
//  Discription:  handle list menu action
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleListMenuWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          )
{
//    MMI_MENU_GROUP_ID_T         group_id = 0;
    MMI_MENU_ID_T               menu_id = 0;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
#ifndef MMI_GUI_STYLE_TYPICAL
    MMI_STRING_T                display_str={0};
    uint16                      cur_index = 0;
#endif
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_GUI_STYLE_TYPICAL
        GUILIST_SetMaxItem(MMIBT_SHARE_SETTING_LIST_CTRL_ID,2, FALSE);
        MMI_GetLabelTextByLang(MMIBT_GetShareSettingAuthority(), &display_str);
        MMIBT_AppendTwoLineTextIconSmallTextListItem(MMIBT_SHARE_SETTING_LIST_CTRL_ID,TXT_BT_SET_SHARING_AUTHORITY,IMAGE_COMMON_RIGHT_ARROW,display_str,MMIBT_SHARE_SETTING_AUTHORITY,FALSE);
        MMI_GetLabelTextByLang(MMIBT_GetShareSettingLocation(), &display_str);
        MMIBT_AppendTwoLineTextIconSmallTextListItem(MMIBT_SHARE_SETTING_LIST_CTRL_ID,TXT_BT_SET_SHARING_DIRECTORY,IMAGE_COMMON_RIGHT_ARROW,display_str,MMIBT_SHARE_SETTING_LOCATION,FALSE);
        MMK_SetAtvCtrl(win_id, MMIBT_SHARE_SETTING_LIST_CTRL_ID);
#else
        MMK_SetAtvCtrl(win_id, MMIBT_LIST_MENU_SHARED_ID);
#endif
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
#ifndef MMI_GUI_STYLE_TYPICAL
    cur_index =  GUILIST_GetCurItemIndex(MMIBT_SHARE_SETTING_LIST_CTRL_ID);
    switch(cur_index)
    {
        case MMIBT_SHARE_SETTING_AUTHORITY:
            {
                MMIBT_OPERTATION_TYPE_E type = MMIBT_OPC_SHARED_AUTHORITY;
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_SHARED_AUTHORITY);
               // MMK_CreateWin((uint32 *)MMIBT_SHARED_AUTHORITY_WIN_TAB, (ADD_DATA)type);
                MMK_CreatePubListWin((uint32 *)MMIBT_SHARED_AUTHORITY_WIN_TAB, (ADD_DATA)type);
                CreateSharedListBox(MMIBT_SHARED_AUTHORITY_WIN_ID, MMIBT_LIST_BOX_SHARED_AUTHORITY_ID,
                           MMIBT_OPC_SHARED_AUTHORITY, BT_SERVICE_ALL);
            }
            break;
        case MMIBT_SHARE_SETTING_LOCATION:
            {
                MMIBT_OPERTATION_TYPE_E type = MMIBT_OPC_SHARED_DIRECTORY;
                if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                    break;
                }
                else
                {
                    //设置蓝牙的操作信息
                    MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_SHARED_DIRECTORY);
                    MMK_CreatePubListWin((uint32 *)MMIBT_SHARED_DIRECTORY_WIN_TAB, (ADD_DATA)type);
                    CreateSharedListBox(MMIBT_SHARED_DIRECTORY_WIN_ID, MMIBT_LIST_BOX_SHARED_DIRECTORY_ID,
                        MMIBT_OPC_SHARED_DIRECTORY, BT_SERVICE_ALL);

                }
            }
            break;

        default:
            break;
    }
#else
        menu_id = GUIMENU_GetCurNodeId(MMIBT_LIST_MENU_SHARED_ID);
        switch (menu_id)
        {
        case ID_BT_SHARED_FILE_AUTHORITY:
            //Setting Shared file authority
            {
                MMIBT_OPERTATION_TYPE_E type = MMIBT_OPC_SHARED_AUTHORITY;
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_SHARED_AUTHORITY);
                MMK_CreateWin((uint32 *)MMIBT_SHARED_AUTHORITY_WIN_TAB, (ADD_DATA)type);
                CreateSharedListBox(MMIBT_SHARED_AUTHORITY_WIN_ID, MMIBT_LIST_BOX_SHARED_AUTHORITY_ID,
                           MMIBT_OPC_SHARED_AUTHORITY, BT_SERVICE_ALL);
            }
            break;
        case ID_BT_SHARED_FILE_DIRECTORY:
            //setting shared directory
            {
                MMIBT_OPERTATION_TYPE_E type = MMIBT_OPC_SHARED_DIRECTORY;
                if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                    break;
                }
                else
                {
                    //设置蓝牙的操作信息
                    MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_SHARED_DIRECTORY);
                    MMK_CreateWin((uint32 *)MMIBT_SHARED_DIRECTORY_WIN_TAB, (ADD_DATA)type);
                    CreateSharedListBox(MMIBT_SHARED_DIRECTORY_WIN_ID, MMIBT_LIST_BOX_SHARED_DIRECTORY_ID,
                        MMIBT_OPC_SHARED_DIRECTORY, BT_SERVICE_ALL);
                }
            }
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
            MMK_DestroyDynaCtrl(MMIBT_LIST_MENU_SHARED_ID);
            break;
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}
/*****************************************************************************/
//  Discription:  handle list box select
//  Global resource dependence: none 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleSharedListBoxWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          )
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    uint32     win_opc = (uint32)(MMK_GetWinAddDataPtr(win_id));
    MMI_CTRL_ID_T ctrl_id  = 0;
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
    
#ifdef MMIBT_FTS_SUPPORT
    if(MMIBT_OPC_SHARED_AUTHORITY == win_opc)
    {
        ctrl_id = MMIBT_LIST_BOX_SHARED_AUTHORITY_ID;
    }
    else
#endif//MMIBT_FTS_SUPPORT
		if(MMIBT_OPC_SHARED_DIRECTORY == win_opc)
    {
        ctrl_id = MMIBT_LIST_BOX_SHARED_DIRECTORY_ID;
    }
        
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
       
#ifdef MMIBT_FTS_SUPPORT
        //Setting 
        if(MMIBT_OPC_SHARED_AUTHORITY == win_opc)
        {
            //shared authority
            set_ret = Set_FTP_Shared_Authority(cur_select_item);
        }
        else
#endif
        {
            //shared directory
            set_ret = Set_FTP_Shared_Directory(cur_select_item);
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
            CreateSharedListBox(win_id, ctrl_id, win_opc, BT_SERVICE_ALL);   /*lint !e64*/ 
#endif                 
        }
           MMK_CloseWin(win_id);
        break;
        
        case MSG_GET_FOCUS: 
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        //case MSG_KEYDOWN_CANCE
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
//  Discription: 根据current_opt和service的不同，创建蓝牙中所不同的list
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void CreateSharedListBox(
                    MMI_WIN_ID_T             win_id,                   
                    MMI_CTRL_ID_T            ctrl_id,       
                    MMIBT_OPERTATION_TYPE_E  current_opt,
                    uint32                      service
                    )
{
    GUILIST_INIT_DATA_T init_data = {0};
    MMIBT_USER_SETTING_U setting = {0};/*lint !e64*/
    uint32        len = 0;
    uint16        selected_index = 0;
	uint16        cur_index = 0;
    MMI_HANDLE_T  ctrl_handle = 0;

    MMIBT_SetCurrentSevice(service);
    if(!MMK_IsOpenWin(win_id))
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8621_112_2_18_2_8_18_306,(uint8*)"");
        return;
    }
    
    init_data.both_rect = MMITHEME_GetWinClientBothRect(win_id);
    init_data.type = GUILIST_RADIOLIST_E;
    ctrl_handle = GUILIST_CreateListBox(win_id, 0, ctrl_id, &init_data);
    
    //set menu item
    switch(current_opt)
    {    
#ifdef MMIBT_FTS_SUPPORT
        case MMIBT_OPC_SHARED_AUTHORITY:
        {
            //set authority items
            GUILIST_SetMaxItem(ctrl_id,MMIBT_SHARED_AUTHORITY_NUM, FALSE);//max item 2
            //"Free Read and Write"
            MMIBT_AppendStringListItem(
                TXT_BT_AUTHORITY_RW,
                GUIITEM_STYLE_ONE_LINE_RADIO,
                STXT_SELECT,
                TXT_NULL,
                STXT_RETURN,
                ctrl_id
                );
            //"Read Only"
            MMIBT_AppendStringListItem(
                TXT_BT_AUTHORITY_READ,
                GUIITEM_STYLE_ONE_LINE_RADIO,
                STXT_SELECT,
                TXT_NULL,
                STXT_RETURN,
                ctrl_id
                );
    		
            MMIBT_GetFileUserSetting(MMINV_BT_SHAIRED_AUTHORITY, &setting);
            if(!memcmp((uint8 *)MMIBT_FTP_RW_MODE, setting.authority.file_mode, strlen(MMIBT_FTP_RW_MODE)))
            {
                selected_index = 0;
            }
            else
            {
                selected_index = 1;
            }
        }
            break;
#endif//MMIBT_FTS_SUPPORT
        case MMIBT_OPC_SHARED_DIRECTORY:
        {
            MMIFILE_DEVICE_E dev    = MMI_DEVICE_UDISK;
            BOOLEAN          dev_ok = FALSE;
            
            //set directory items
    		MMIBT_GetFileUserSetting(MMINV_BT_SHAIRED_DIRECTORY, &setting);
    		len = MMIAPICOM_Wstrlen((wchar*)setting.dir.shaired_path_name);
    		cur_index = 0;
            GUILIST_SetMaxItem(ctrl_id,MMIBT_SHARED_DIRECTORY_NUM, FALSE);//max item 3

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

                    if(dev == MMIAPIFMM_GetDeviceTypeByPath(setting.dir.shaired_path_name, MMIAPICOM_Wstrlen(setting.dir.shaired_path_name)))
        			{
        				selected_index = cur_index;
        				dev_ok         = TRUE;
        			}
        			cur_index++;
        		}
            }while(++dev < MMI_DEVICE_NUM);
            

            MMIBT_AppendStringListItem(
                TXT_SELECT_FOLDER,
                GUIITEM_STYLE_ONE_LINE_RADIO,
                STXT_SELECT,
                TXT_NULL,
                STXT_RETURN,
                ctrl_id);

             if(!dev_ok)
    		 {


    			 selected_index = cur_index;
    		 }
        }          
            break;
        default:
         break;
    }
    GUILIST_SetCurItemIndex(ctrl_id,selected_index);
    GUILIST_SetSelectedItem(ctrl_id, selected_index, TRUE);
}
#ifdef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Discription: create list menu by current operation
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return: 
/*****************************************************************************/
LOCAL void CreateSharedFileSettingMenu(
                    MMI_WIN_ID_T             win_id, //IN: list menu's window id
                    MMI_CTRL_ID_T            ctrl_id, //IN: list menu's id
                    MMI_TEXT_ID_T            text_id //IN: list menu titles's text id
                               )
{
	GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    MMI_STRING_T    kstring = {0};
    uint32          node_id = 0;
    GUIMENU_DYNA_ITEM_T     node_item = {0};

    if(!MMK_IsOpenWin(win_id))
    {
        SCI_TRACE_LOW("CreateSharedFileSettingMenu window already opened !");
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
    
    //set shared setting menu
    //"Shared file authority"
    text_id = TXT_BT_SET_SHARING_AUTHORITY;
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_id = ID_BT_SHARED_FILE_AUTHORITY;
    
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(0, node_id, 0, &node_item, ctrl_id);
    //"Shared file directory"
    text_id = TXT_BT_SET_SHARING_DIRECTORY;
    node_id = ID_BT_SHARED_FILE_DIRECTORY;
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(1, node_id, 0, &node_item, ctrl_id);
 }
#endif
/*****************************************************************************/
//  Description : set ftp shared directory
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMIBT_SETTING_RET Set_FTP_Shared_Directory(uint16 cur_select_item)
{
    MMIFMM_FILTER_T filter = {0};
    uint32          path_len = 0;
    uint32          usc_len = 0;
    FILEARRAY       ret_array = NULL;
    MMIBT_USER_SETTING_U setting = {0};/*lint !e64*/
    BT_STATUS      bt_ret = BT_SUCCESS;
    MMIBT_SETTING_RET result = MMIBT_SUCCESS;
	wchar          *path_ptr = PNULL;
	MMIFILE_DEVICE_E device = MMI_DEVICE_UDISK;
    uint32          device_index = 0;
    MMIFILE_DEVICE_E dev    = MMI_DEVICE_UDISK;
    MMIFILE_DEVICE_E dev_ok_type[MMI_DEVICE_NUM + 1] = {0};
    uint16        cur_index = 0;
    
#ifndef MMI_GUI_STYLE_TYPICAL
            MMI_STRING_T display_str ={0};
#endif

    if(MMIAPIUDISK_UdiskIsRun())
    {
        result = MMIBT_UDISK_RUNING;
    }

    if((MMIAPIFMM_GetFirstValidDevice() >= MMI_DEVICE_NUM))
    {
        return MMIBT_UDISK_SDCARD_NOT_EXIST;
    }    
    if(cur_select_item >= MMIBT_SHARED_DIRECTORY_NUM)
	{
		return MMIBT_UDISK_SDCARD_NOT_EXIST;
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
    
	device = dev_ok_type[cur_select_item];
    
    //SD or Udisk exist
    SCI_MEMSET(&setting, 0X00, sizeof(setting));
    //shared directly not opened
    if(device < MMI_DEVICE_NUM)
	{
		if(!MMIAPIFMM_GetDeviceTypeStatus((MMIFILE_DEVICE_E)device))
		{
			device = MMIAPIFMM_GetFirstValidDevice();
		}
		
        path_ptr = MMIAPIFMM_GetDevicePath(device);
		if(NULL != path_ptr)
		{
			usc_len = MIN(MMIAPICOM_Wstrlen(path_ptr), MMIFILE_FULL_PATH_MAX_LEN);/*lint !e666*/
			
			//select sd or udisk
			MMIAPICOM_Wstrncpy(setting.dir.shaired_path_name, path_ptr, usc_len);
            if(usc_len + 2 <= MMIFILE_FULL_PATH_MAX_LEN)
            {
                setting.dir.shaired_path_name[usc_len] = MMIFILE_COLON;
                //usc_len++;
                //setting.dir.shaired_path_name[usc_len] = MMIFILE_SLASH;
            }
			bt_ret = MMIBT_SaveUserSetting(MMINV_BT_SHAIRED_DIRECTORY, &setting);
			if(BT_SUCCESS == bt_ret)
			{

				result = MMIBT_SUCCESS;
#ifndef MMI_GUI_STYLE_TYPICAL
            MMI_GetLabelTextByLang(MMIBT_GetShareSettingLocation(), &display_str);
            MMIBT_AppendTwoLineTextIconSmallTextListItem(MMIBT_SHARE_SETTING_LIST_CTRL_ID,TXT_BT_SET_SHARING_DIRECTORY,IMAGE_COMMON_RIGHT_ARROW,display_str,MMIBT_SHARE_SETTING_LOCATION,TRUE);
#endif
			}
			else
			{
				if(BT_BUSY == bt_ret)
				{
					//ftp busy
					result = MMIBT_FTP_BUSY;
				}
				else
				{
					//folder not exist
					result = MMIBT_FOLDER_NOT_EXIST;
				}
			} 
		}
	}
    else
    {
        //selected folder
        MMIBT_GetFileUserSetting(MMINV_BT_SHAIRED_DIRECTORY, &setting);
        path_len = MMIAPICOM_Wstrlen((wchar *)setting.dir.shaired_path_name);
        SCI_MEMCPY(filter.filter_str, "*", strlen("*"));/*lint !e666*/
        //get directory from FMM
        ret_array = MMIAPIFILEARRAY_Create();          
        if(FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(setting.dir.shaired_path_name, path_len, &filter, FUNC_FIND_FOLDER, FALSE, NULL, ret_array, MMIBT_OpenFileCallBack,FALSE))
        {
            MMIAPIFILEARRAY_Destroy(&ret_array);
        }
        result = MMIBT_SETTING_NUM;
    }
  
    return result;  
}
/*****************************************************************************/
//  Description : open direct and get user selected dir
//  Global resource dependence : 
//  Author: 
//  Note: after user selected a dir, it will be set to shaired directory
/*****************************************************************************/
LOCAL void MMIBT_OpenFileCallBack( BOOLEAN     is_success,
                                    FILEARRAY   file_array
                                    )
{
    uint32  file_num = 0;
    BT_STATUS bt_ret = BT_SUCCESS;
    FILEARRAY_DATA_T file_array_data = {0};
    MMIBT_USER_SETTING_U setting = {0};/*lint !e64*/
#ifndef MMI_GUI_STYLE_TYPICAL
    MMI_STRING_T        display_str = {0};
#endif
    
    //SCI_TRACE_LOW:"MMIBT_OpenFileCallBack, Enter,is_success = %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9500_112_2_18_2_8_19_319,(uint8*)"d",is_success);
    if(is_success)
    {
        file_num = MMIAPIFILEARRAY_GetArraySize(file_array);
        //SCI_TRACE_LOW:"MMIBT_OpenFileCallBack, file_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9504_112_2_18_2_8_19_320,(uint8*)"d", file_num);
        if(file_num > 0)
        {
            //read user selected dir
            if(MMIAPIFILEARRAY_Read(file_array, 0, &file_array_data))
			{	
				//set shaired directory
				MMIDEFAULT_SetBackLight(TRUE);
				MMIAPICOM_Wstrncpy(setting.dir.shaired_path_name, file_array_data.filename, MIN(file_array_data.name_len, MMIFILE_FULL_PATH_MAX_LEN ));
				bt_ret = MMIBT_SaveUserSetting(MMINV_BT_SHAIRED_DIRECTORY, &setting);
#ifndef MMI_GUI_STYLE_TYPICAL
            MMI_GetLabelTextByLang(MMIBT_GetShareSettingLocation(), &display_str);
            MMIBT_AppendTwoLineTextIconSmallTextListItem(MMIBT_SHARE_SETTING_LIST_CTRL_ID,TXT_BT_SET_SHARING_DIRECTORY,IMAGE_COMMON_RIGHT_ARROW,display_str,MMIBT_SHARE_SETTING_LOCATION,TRUE);
#endif

			}
			else
			{
				bt_ret = BT_ERROR;
			}
            if(BT_SUCCESS == bt_ret)
            {
                //sucess
                //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE); 
            }
            else if(BT_BUSY == bt_ret)
            {
                //ftp busy
                MMIPUB_OpenAlertWarningWin(TXT_BT_FTP_BUSY);
     
            }
            else
            {
                //folder not exist or other error
                MMIPUB_OpenAlertWarningWin(TXT_FOLDER_NOT_EXIST);
            }
        }
    }
    //refresh list
#ifdef MMI_GUI_STYLE_TYPICAL
    MMK_DestroyDynaCtrl(MMIBT_LIST_BOX_SHARED_DIRECTORY_ID);
    CreateSharedListBox(MMIBT_SHARED_DIRECTORY_WIN_ID, MMIBT_LIST_BOX_SHARED_DIRECTORY_ID, MMIBT_OPC_SHARED_DIRECTORY, BT_SERVICE_ALL);
#endif
    //释放列表
    MMIAPIFILEARRAY_Destroy(&file_array);
    file_array = PNULL;
}
/*****************************************************************************/
//  Discription: Open the bluetooth set visibility  win   
//  Global resource dependence: none 
//  Author: yuantao.xu 
//  Note:
//
/*****************************************************************************/
PUBLIC void  MMIBT_EnterSharedWin(void)
{
    MMK_CreateWin((uint32 *)MMIBT_SHARED_FILE_WIN_TAB,PNULL);
#ifdef MMI_GUI_STYLE_TYPICAL
    CreateSharedFileSettingMenu(MMIBT_SHARED_FILE_WIN_ID, MMIBT_LIST_MENU_SHARED_ID,
                     TXT_BT_SET_SHARED_FILE);
#endif
}
/*****************************************************************************/
//  Description : set ftp shared authority
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMIBT_SETTING_RET Set_FTP_Shared_Authority(uint16 cur_select_item)
{
    BT_STATUS      bt_ret = BT_SUCCESS;
    MMIBT_USER_SETTING_U setting = {0};/*lint !e64*/
    MMIBT_SETTING_RET result = MMIBT_SUCCESS;

    if(MMIAPIUDISK_UdiskIsRun())
    {
        return MMIBT_UDISK_RUNING;
    }
    
    
	if(MMIAPIFMM_GetFirstValidDevice() >= MMI_DEVICE_NUM)
	{
		return MMIBT_UDISK_SDCARD_NOT_EXIST;
	}
	
    //sd or udisk exist
    SCI_MEMSET(&setting, 0X00, sizeof(setting));
    //SCI_PASSERT(cur_select_item < MMIBT_SHARED_AUTHORITY_NUM, ("HandleListBoxWinMsg cur_select_item %d",cur_select_item));/*assert verified*/
    if(cur_select_item >= MMIBT_SHARED_AUTHORITY_NUM)
    {
        return MMIBT_ERROR;
    }
    
    SCI_MEMCPY(setting.authority.file_mode, s_bt_list_shared_authority[cur_select_item], strlen((char*)s_bt_list_shared_authority[cur_select_item]));/*lint !e666*/
    bt_ret = MMIBT_SaveUserSetting(MMINV_BT_SHAIRED_AUTHORITY, &setting);
    if(BT_SUCCESS == bt_ret)
    {
#ifndef MMI_GUI_STYLE_TYPICAL
        MMI_STRING_T display_str ={0};
        MMI_GetLabelTextByLang(MMIBT_GetShareSettingAuthority(), &display_str);
        MMIBT_AppendTwoLineTextIconSmallTextListItem(MMIBT_SHARE_SETTING_LIST_CTRL_ID,TXT_BT_SET_SHARING_AUTHORITY,IMAGE_COMMON_RIGHT_ARROW,display_str,MMIBT_SHARE_SETTING_AUTHORITY,TRUE);
#endif
        result = MMIBT_SUCCESS;

    }
    else
    {
        if(BT_BUSY == bt_ret)
        {
            //ftp busy
            result = MMIBT_FTP_BUSY;
        }
        else
        {
            //folder not exist
            result = MMIBT_FOLDER_NOT_EXIST;
        }
    } 
    
    return result;  
}
#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Discription:  get sharesetting authority 
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
MMI_TEXT_ID_T MMIBT_GetShareSettingAuthority(void)
{
    MMI_TEXT_ID_T    text_id = {0};
    MMIBT_USER_SETTING_U setting = {0};/*lint !e64*/
    
    MMIBT_GetFileUserSetting(MMINV_BT_SHAIRED_AUTHORITY, &setting);
    if(!memcmp((uint8 *)MMIBT_FTP_RW_MODE, setting.authority.file_mode, strlen(MMIBT_FTP_RW_MODE)))
    {
       text_id =  TXT_BT_AUTHORITY_RW;
    }
    else
    {
        text_id = TXT_BT_AUTHORITY_READ;
    }
    return text_id;
}
/*****************************************************************************/
//  Discription:  get sharesetting location
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
MMI_TEXT_ID_T MMIBT_GetShareSettingLocation(void)
{
    MMIFILE_DEVICE_E dev    = MMI_DEVICE_UDISK;
    BOOLEAN          dev_ok = FALSE;
    MMI_TEXT_ID_T    text_id = {0};
    MMIBT_USER_SETTING_U setting = {0};/*lint !e64*/

    MMIBT_GetFileUserSetting(MMINV_BT_SHAIRED_DIRECTORY, &setting);
    do
    {
        if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(dev), MMIAPIFMM_GetDevicePathLen(dev)))
	    {

            if(dev == MMIAPIFMM_GetDeviceTypeByPath(setting.dir.shaired_path_name, MMIAPICOM_Wstrlen(setting.dir.shaired_path_name)))
		    {
                text_id = MMIAPIFMM_GetDeviceName(dev);
			    dev_ok         = TRUE;
		    }
	    }
    }while(++dev < MMI_DEVICE_NUM);

     if(!dev_ok)
     {

	     text_id = TXT_SELECT_FOLDER;
     }
     return text_id;
}
#endif
#endif
#endif

