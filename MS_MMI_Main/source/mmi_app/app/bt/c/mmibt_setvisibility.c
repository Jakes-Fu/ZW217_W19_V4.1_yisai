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
//  Discription:
//  Global resource dependence: none 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleSetVisibilityWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          );
/*****************************************************************************/
//  Discription: 根据current_opt和service的不同，创建蓝牙中所不同的list
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void CreateVisibilityList(
                    MMI_WIN_ID_T             win_id,                   
                    MMI_CTRL_ID_T            ctrl_id,  
                    uint32                      service
                    );
/*****************************************************************************/
//blue tooth set visibility win table
WINDOW_TABLE(MMIBT_SET_VISIBILITY_WIN_TAB) = 
{ 
    WIN_FUNC((uint32) HandleSetVisibilityWinMsg),    
    WIN_ID(MMIBT_SET_VISIBILITY_WIN_ID),
    WIN_TITLE(TXT_MY_BT_VISIBILITY),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN        
};
/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                            LOCAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription:
//  Global resource dependence: none 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleSetVisibilityWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          )
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id  = MMIBT_SET_VISIBILITY_CTRL_ID;
    uint16        cur_select_item = 0;
    
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
        //get the selected item
        GUILIST_GetSelectedItemIndex(ctrl_id,&cur_select_item,1);
        if(0 == cur_select_item)
        {
            //set visibile
            DoOperation(MMIBT_OPC_VISIBLE,FALSE);
        }
        else
        {
            //set invisibile
            DoOperation(MMIBT_OPC_HIDE,FALSE);
        }
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
//  Discription: Open the bluetooth set visibility  win   
//  Global resource dependence: none 
//  Author: yuantao.xu 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_EnterSetVisibilityWin(void)
{
    MMK_CreateWin((uint32 *)MMIBT_SET_VISIBILITY_WIN_TAB,PNULL); 
    CreateVisibilityList(MMIBT_SET_VISIBILITY_WIN_ID, MMIBT_SET_VISIBILITY_CTRL_ID,  0);
}
/*****************************************************************************/
//  Discription: 根据current_opt和service的不同，创建蓝牙中所不同的list
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void CreateVisibilityList(
                    MMI_WIN_ID_T             win_id,                   
                    MMI_CTRL_ID_T            ctrl_id,  
                    uint32                      service
                    )
{
    GUILIST_INIT_DATA_T init_data = {0};
    MMI_TEXT_ID_T left_softkey_id   = TXT_NULL;
    MMIBT_USER_SETTING_U setting = {0};/*lint !e64*/
    uint16        selected_index = 0;
    MMI_HANDLE_T  ctrl_handle = 0;
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
    
    //Visibility list
    GUILIST_SetMaxItem(ctrl_id,MMIBT_SET_MAX_ITEM_NUM, FALSE);//max item 2
    //"Visible"
    MMIBT_AppendStringListItem(
        TXT_BT_VISIBILITY,
        GUIITEM_STYLE_ONE_LINE_RADIO,
        STXT_SELECT,
        TXT_NULL,
        STXT_RETURN,
        ctrl_id
        );
    //"Hide"
    MMIBT_AppendStringListItem(
        TXT_BT_HIDDEN,
        GUIITEM_STYLE_ONE_LINE_RADIO,
        STXT_SELECT,
        TXT_NULL,
        STXT_RETURN,
        ctrl_id
        );
    //set current selected item
    if(MMIBT_GetIsDeviceVisible())
    {
        //current is visibile, high light visibile item
        selected_index = 0;
    }
    else
    {
        //current is hide, high light hide item
        selected_index = 1;
    }
    
    GUILIST_SetCurItemIndex(ctrl_id,selected_index);
    GUILIST_SetSelectedItem(ctrl_id, selected_index, TRUE);
}
#endif