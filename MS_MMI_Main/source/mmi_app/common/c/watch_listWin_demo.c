/*****************************************************************************
** File Name:      watch_listWin_demo.c                                        *
** Author:           xiuyun.wang                                             *
** Date:             02/11/2020                                              *
** Copyright:                                                                *
** Description:    This file is used as a demo code for list window          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 02/11/2020           xiuyun.wang           Create                         *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include <stdio.h>
#include "guilistbox.h"
#include "watch_common_list.h"
#include "os_api.h"
#include "sci_types.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmi_id.h"
#include "window_parse.h"
#include "mmiphone_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
PUBLIC void MMIDEMO_BrightnessSettings_EnterWin(void)
{
}
PUBLIC void MMIDEMO_ClockSettings_EnterWin(void)
{
}
PUBLIC void MMIDEMO_AlarmSettings_EnterWin(void)
{
}
PUBLIC void MMIDEMO_BtSettings_EnterWin(void)
{
}

/*****************************************************************************/
//  Description : get list item visible sim existed flag
//  Parameter: [In] None
//             [Out] None
//             [Return] the visible value, WATCHCOM_LIST_ITEM_VISIBLE_TRUE & WATCHCOM_LIST_ITEM_VISIBLE_FALSE
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDEMO_Item_Visible_Sim(void)
{
    if ((MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1) && (SIM_STATUS_PUK_BLOCKED != MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1)))
#if defined MMI_MULTI_SIM_SYS_DUAL
        || (MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2) && (SIM_STATUS_PUK_BLOCKED != MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_2)))
#endif
        )
    {
        return TRUE;
    }

    return FALSE;
}
LOCAL uint32 MMIDEMO_Item_GetMarkerNum(void)
{
    return 123;
}
LOCAL uint32 MMIDEMO_Item_SwitchOnOff(void)
{
    return TRUE;
}

/******************************demo for text list data******************************************/
WATCHCOM_LIST_ITEM_STYLE_2STR_ST  list_item_set_brightness     = {TXT_COMMON_COMMON_BRIGHTNESS,  TXT_COMMON_COMMON_BRIGHTNESS};
WATCHCOM_LIST_ITEM_STYLE_1STR_SWITCH_ST  list_item_set_clock   = {TXT_TOOLS_CLOCK,               MMIDEMO_Item_SwitchOnOff};
WATCHCOM_LIST_ITEM_STYLE_1ICON_1STR_ST   list_item_set_alarm   = {IMAGE_SECMENU_ICON_SET_PHONE,  TXT_ALARM_CLOCK};
WATCHCOM_LIST_ITEM_STYLE_2STR_1MARKER_1STR_ST list_item_set_bt = {TXT_BLUETOOTH,                 TXT_BLUETOOTH,       TXT_BLUETOOTH, MMIDEMO_Item_GetMarkerNum};

LOCAL WATCHCOM_LIST_ITEM__ST s_demo_text_list_data[] =
{
    {WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR,  &list_item_set_alarm,       MMIDEMO_AlarmSettings_EnterWin},
    {WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_2STR,        &list_item_set_brightness,  MMIDEMO_BrightnessSettings_EnterWin},
    {WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR_SWITCH, &list_item_set_clock,       MMIDEMO_ClockSettings_EnterWin},
    {MMIDEMO_Item_Visible_Sim,         GUIITEM_STYLE_3STR_1MARKER,&list_item_set_bt,          MMIDEMO_BtSettings_EnterWin},
};
/******************************demo for check list data******************************************/
WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST  list_item_speed_fast   = {TXT_COMMON_FAST};
WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST  list_item_speed_normal = {TXT_NORMAL};
WATCHCOM_LIST_ITEM_STYLE_1STR_CHECK_ST  list_item_speed_slow   = {TXT_COMMON_SLOW};

LOCAL WATCHCOM_LIST_ITEM__ST s_demo_check_list_data[] =
{
    {WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR_CHECK,  &list_item_speed_fast,   PNULL},
    {WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR_CHECK,  &list_item_speed_normal, PNULL},
    {MMIDEMO_Item_Visible_Sim,         GUIITEM_STYLE_1STR_CHECK,  &list_item_speed_slow,   PNULL},
};
/******************************demo for check list data******************************************/
WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST  list_item_fast   = {TXT_COMMON_FAST};
WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST  list_item_normal = {TXT_NORMAL};
WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST  list_item_slow   = {TXT_COMMON_SLOW};
LOCAL WATCHCOM_LIST_ITEM__ST s_demo_radio_list_data[] =
{
    {WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR_RADIO,  &list_item_fast,   PNULL},
    {WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR_RADIO,  &list_item_normal, PNULL},
    {MMIDEMO_Item_Visible_Sim,         GUIITEM_STYLE_1STR_RADIO,  &list_item_slow,   PNULL},
};

/**---------------------------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DEFINITION                                           *
 **---------------------------------------------------------------------------------------------*/

LOCAL void Demo_APP_WEB(void)
{
    WatchCOM_List_Item_CallBack(MMIDEMO_LIST_CTRL_ID);
}

LOCAL void Demo_APP_CANCEL(void)
{
    MMK_CloseWin(MMIDEMO_LIST_WIN_ID);
}

LOCAL void Demo_FULL_PAINT(void)
{
    uint16 cur_selection = 0;
    int32 list_offset = 0;
    uint16 list_num=0;

    // get current select item index
    cur_selection = GUILIST_GetCurItemIndex(MMIDEMO_LIST_CTRL_ID);

    // get top item offset
    GUILIST_GetTopItemOffset(MMIDEMO_LIST_CTRL_ID, &list_offset);

    /* append list items*/
#if 1
    //text list
    list_num = sizeof(s_demo_text_list_data)/sizeof(WATCHCOM_LIST_ITEM__ST);
    WatchCOM_TextList_Create(s_demo_text_list_data,list_num,MMIDEMO_LIST_CTRL_ID);
 #endif
    //change to radio list
    CTRLLIST_ChangeDisplayType(MMIDEMO_LIST_CTRL_ID,GUILIST_RADIOLIST_E,TRUE);
    list_num = sizeof(s_demo_radio_list_data)/sizeof(WATCHCOM_LIST_ITEM__ST);
    WatchCOM_RadioList_Create(s_demo_radio_list_data,list_num,MMIDEMO_LIST_CTRL_ID);
#if 1
    //change to check list
    CTRLLIST_ChangeDisplayType(MMIDEMO_LIST_CTRL_ID,GUILIST_CHECKLIST_E,TRUE);
    list_num = sizeof(s_demo_check_list_data)/sizeof(WATCHCOM_LIST_ITEM__ST);
    WatchCOM_CheckList_Create(s_demo_check_list_data,list_num,MMIDEMO_LIST_CTRL_ID);
#endif
    //set selected item
    GUILIST_SetSelectedItem(MMIDEMO_LIST_CTRL_ID, cur_selection, TRUE);

    //set current item
    GUILIST_SetCurItemIndex(MMIDEMO_LIST_CTRL_ID, cur_selection);

    // set top item offset
    GUILIST_SetTopItemOffset(MMIDEMO_LIST_CTRL_ID, list_offset);
}

LOCAL void Demo_OPEN_WINDOW( void)
{
    MMK_SetAtvCtrl(MMIDEMO_LIST_WIN_ID,MMIDEMO_LIST_CTRL_ID);
}

/*****************************************************************************/
//  Description : to handle demo list window message
//  Global resource dependence : 
//  Author:xiuyun.wang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDemoListWindow(
                                         MMI_WIN_ID_T    win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            Demo_OPEN_WINDOW();
            break;
        }
        case MSG_FULL_PAINT:
        {
           Demo_FULL_PAINT();
           break;
        }
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif
       // case MSG_CTL_OK:
       // case MSG_APP_OK:
       // case MSG_APP_WEB:
       // case MSG_CTL_MIDSK:
        {
            Demo_APP_WEB();
            break;
        }
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            Demo_APP_CANCEL();
            break;
        }
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;  
}

WINDOW_TABLE( MMIDEMO_LIST_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleDemoListWindow ),    
    WIN_ID( MMIDEMO_LIST_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIDEMO_LIST_CTRL_ID),  //最好在此处设置list type:GUILIST_TEXTLIST_E 普通 text list
    END_WIN                                                         //GUILIST_RADIOLIST_E  单选(Radio)list
                                                                    //GUILIST_CHECKLIST_E  多选(check)list
};

PUBLIC void MMIDEMO_List_EnterWin(void)
{
    MMK_CreateWin((uint32 *)MMIDEMO_LIST_WIN_TAB, PNULL);
}

