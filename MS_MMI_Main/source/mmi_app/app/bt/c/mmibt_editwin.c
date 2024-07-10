/****************************************************************************
** File Name:     mmibt_editwin.c                                        *
** Author:         yuantao.xu                                            *
** Date:           8/3/2012                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the cryp display menu     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 8/7/2012    yuantao.xu     Create                                  *
**                                                                         *
****************************************************************************/
//#ifndef _MMIBT_WINTAB_C_
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
#include "mmibt_editwin.h"
#include "mmipub.h"
#include "guilabel.h"
#include "mmifmm_export.h"   //bt_abs.h中用到ffs.h中的结构
#include "bt_abs.h"
#include "bt_cfg.h"
#include "mmibt_text.h"
#include "mmi_default.h"
#include "mmiset_export.h"
#include "guitext.h"
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
#include "guictrl_api.h"
#include "mmiidle_statusbar.h"
#ifdef BLUETOOTH_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
 typedef struct mmibt_editwin_param_tag
{
    BOOLEAN is_local_name;
    BT_DEVICE_INFO edit_device_info;
}MMIBT_EDITWIN_PARAM_T;
/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                */
/*---------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription: Handle blue tooth edit device name win msg
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleEditDeviceNameMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             );
 /*****************************************************************************/
//  Discription: Create blue tooth edit device name win
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E CreateEditWin(MMIBT_EDITWIN_PARAM_T *editwin_info);
/*--------------------------------------------------------------------------------*/
//the window for edit blue tooth device name win table
WINDOW_TABLE(MMIBT_EDIT_DEVICE_NAME_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleEditDeviceNameMsg),    
    WIN_ID(MMIBT_EDIT_DEVICE_NAME_WIN_ID),
    WIN_TITLE(TXT_NULL),
    CREATE_EDIT_TEXT_CTRL(0,MMIBT_EDIT_DEVICE_NAME_EDITBOX_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),  
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),  
#endif    
    END_WIN
};

/*****************************************************************************/
//  Discription: Handle blue tooth edit device name win msg
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleEditDeviceNameMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             )
{
    uint16                   device_name[BT_DEVICE_NAME_SIZE] = {0};
    uint16                   ucs_new_name_len = 0;
    MMI_RESULT_E             result = MMI_RESULT_TRUE;
    MMI_STRING_T   new_name = {0};
    wchar                    ucs_new_name[BT_DEVICE_NAME_SIZE] = {0};
    MMI_CTRL_ID_T            ctrl_id = MMIBT_EDIT_DEVICE_NAME_EDITBOX_CTRL_ID;
    BT_STATUS                return_value = BT_SUCCESS;
    MMIBT_EDITWIN_PARAM_T    *edit_win_info = (MMIBT_EDITWIN_PARAM_T *)MMK_GetWinUserData(win_id);

    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            MMI_TEXT_ID_T text_id = TXT_NULL;
            MMI_STRING_T  title = {0};

			GUIEDIT_SetTextMaxLen(ctrl_id,MMIBT_DEVICE_NAME_UCS2_MAX_LEN, MMIBT_DEVICE_NAME_ALPHA_MAX_LEN);
            SCI_MEMSET(device_name,0x00,(BT_DEVICE_NAME_SIZE*sizeof(uint16))); 
            if(edit_win_info->is_local_name)
            {
               text_id = TXT_MY_BT_DEVICE_NAME_EDIT;
               BT_GetLocalName(device_name);
               GUIEDIT_SetString(ctrl_id, device_name,MMIAPICOM_Wstrlen((wchar*)device_name) );       

            }
            else
            {
                text_id = TXT_BT_DEVICE_RENAME;
                GUIEDIT_SetString(ctrl_id, edit_win_info->edit_device_info.name,MMIAPICOM_Wstrlen((wchar*)edit_win_info->edit_device_info.name) );      
            }
			GUIEDIT_SetSoftkey(ctrl_id,	0, 1, TXT_NULL,	TXT_COMMON_OK, PNULL);
            MMI_GetLabelTextByLang(text_id, &title);
            GUIWIN_SetTitleText(win_id, title.wstr_ptr, title.wstr_len, FALSE);
            MMK_SetAtvCtrl(win_id,ctrl_id);
        }
        break;
        
    
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        SCI_MEMSET(&new_name,0,sizeof(MMI_STRING_T));
        GUIEDIT_GetString(ctrl_id,&new_name);
        ucs_new_name_len = MIN(MMIBT_DEVICE_NAME_ALPHA_MAX_LEN, new_name.wstr_len);
        MMIAPICOM_Wstrncpy( ucs_new_name, new_name.wstr_ptr, new_name.wstr_len);
        ucs_new_name[new_name.wstr_len] = 0;
        
        if ( 0 < new_name.wstr_len  && new_name.wstr_len <= MMIBT_DEVICE_NAME_ALPHA_MAX_LEN)
        {
            if(edit_win_info->is_local_name)
            {
                return_value = BT_SetLocalName(ucs_new_name);
            }
            else
            {
                return_value = BT_SetPairedDeviceName( &edit_win_info->edit_device_info.addr,(uint16*)ucs_new_name);
            }
            if (BT_SUCCESS == return_value)
            {                
                if(edit_win_info->is_local_name)
                {
#ifndef MMI_GUI_STYLE_TYPICAL
                 MMIBT_AppendTwoLineTextIconSmallTextListItem(MMIBT_MAIN_LIST_CTRL_ID,TXT_MY_BT_DEVICE_NAME,IMAGE_COMMON_RIGHT_ARROW,new_name,BT_INDEX_NAME,TRUE);
#endif
#ifdef BT_DIALER_SUPPORT
				 MMIBT_AppendTwoLineTextIconSmallTextListItem(MMIBT_SETTING_LIST_CTRL_ID,TXT_MY_BT_DEVICE_NAME,IMAGE_COMMON_RIGHT_ARROW,new_name,BT_SETTING_INDEX_NAME,TRUE);
#endif
                }
                
                MMK_CloseWin(win_id);
//#if 0                
                if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
                {   
                    MMK_SendMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, NULL);
                    MMK_CloseWin(MMIBT_DEVICE_DETAILS_WIN_ID);
                } 
//#endif                
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }                   
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_DC_NAMEINVALID);
        }
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if(PNULL != edit_win_info)
        {
            SCI_FREE(edit_win_info);
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}
/*****************************************************************************/
//  Discription: Create blue tooth edit device name win
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_EnterEditWin(BT_DEVICE_INFO *device_info_ptr,BOOLEAN is_local_name)
{  
    MMIBT_EDITWIN_PARAM_T editwin_info = {0};
    
    editwin_info.is_local_name = is_local_name;
    if(PNULL != device_info_ptr)
    {
        SCI_MEMCPY(&editwin_info.edit_device_info, device_info_ptr, sizeof(BT_DEVICE_INFO));
    }
    
    CreateEditWin(&editwin_info);
}
/*****************************************************************************/
//  Discription: Create blue tooth edit device name win
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E CreateEditWin(MMIBT_EDITWIN_PARAM_T *editwin_info)
{
    MMIBT_EDITWIN_PARAM_T *param_ptr = PNULL;
    BOOLEAN                result = TRUE;
    MMI_HANDLE_T           win_handle = 0;
    
    if(PNULL == editwin_info)
        return MMI_RESULT_FALSE;

    if(MMK_IsOpenWin(MMIBT_EDIT_DEVICE_NAME_WIN_ID))
        MMK_CloseWin(MMIBT_EDIT_DEVICE_NAME_WIN_ID);

    param_ptr = (MMIBT_EDITWIN_PARAM_T *)SCI_ALLOC_APPZ(sizeof(MMIBT_EDITWIN_PARAM_T));

    if(PNULL == param_ptr)
        return MMI_RESULT_FALSE;
    SCI_TRACE_LOW("CreateEditWin enter,is_local_name %d",editwin_info->is_local_name);

    param_ptr->is_local_name = editwin_info->is_local_name;
    SCI_MEMCPY(&param_ptr->edit_device_info, &editwin_info->edit_device_info, sizeof(BT_DEVICE_INFO));
    win_handle = MMK_CreatePubEditWin((uint32 *)MMIBT_EDIT_DEVICE_NAME_WIN_TAB, (ADD_DATA)param_ptr);

    if(!win_handle || (MMI_INVALID_ID == win_handle))
    {
        SCI_FREE(param_ptr);        
    }
    else
    {
        MMK_SetWinUserData(MMIBT_EDIT_DEVICE_NAME_WIN_ID, (void *)param_ptr);
        result = MMI_RESULT_TRUE;
    }
    SCI_TRACE_LOW("CreateEditWin res %d",result);

    return result;
}
#endif
