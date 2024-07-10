/*****************************************************************************
** File Name:      mmirecord_optlistwin.c                                 *
** Author:         jiaxin.sun                                                    *
** Date:           2020.3.4                                                  *
** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe recorder                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*

******************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_type.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "mmi_appmsg.h"
#include "mmk_msg.h"
#include "mmi_default.h"
#include "mmipub.h"
#include "mmicom_panel.h"
#include "mmk_timer.h"
#include "mmi_image.h"
#include "mmi_pubmenuwin.h"
#include "mmitheme_update.h"

#include "mmirecord_wintablel.h"
#include "mmirecord_appcontrol.h"
#include "mmirecord_main.h"
#include "mmirecord_image.h"
#include "mmirecord_menutable.h"
#include "mmirecord_id.h"
#include "mmirecord_text.h"
#include "mmi_common.h"
#include "watch_common_btn.h"
#include "watch_common_list.h"
#include "watch_record_api.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST  list_item_wav   = {TXT_WATCH_RECORD_FORMATWAV};
WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST  list_item_mp3 = {TXT_WATCH_RECORD_FORMATMP3};
WATCHCOM_LIST_ITEM_STYLE_1STR_RADIO_ST  list_item_amr   = {TXT_WATCH_RECORD_FORMATAMR};
LOCAL uint8 s_current_index = 0;
LOCAL WATCHCOM_LIST_ITEM__ST s_demo_radio_list_data[] =
{
    {WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR_RADIO,  &list_item_wav,   PNULL},
    {WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR_RADIO,  &list_item_mp3,  PNULL},
    {WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1STR_RADIO,  &list_item_amr,   PNULL},
};
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL void WatchRec_OptList_FULL_PAINT(void);
LOCAL void WatchRec_OptList_APP_WEB(void);
LOCAL void WatchRec_OptList_OpenWindow(MMI_WIN_ID_T      winId, MMI_MESSAGE_ID_E msgId, DPARAM param);
LOCAL MMI_RESULT_E HandleRecordOptWinMsg(
    MMI_WIN_ID_T      winId,
    MMI_MESSAGE_ID_E    msgId,
    DPARAM                param
);

LOCAL void WatchRec_OptList_OpenWindow(MMI_WIN_ID_T      winId, MMI_MESSAGE_ID_E msgId, DPARAM param)
{
    GUI_RECT_T rect = {0};
    MMIRECORD_WORK_FORMAT_E format = MMIRECORD_GetRecordFileFormatType();
    uint8 curIndex = 0;
    MMI_STRING_T textLeft = {0};
    MMI_STRING_T textMid = {0};
    MMI_STRING_T textRight = {0};

    rect.left = 0;
    rect.top = 0;
    rect.right = RECORD_BACKGROUND_RIGHT;
    rect.bottom = RECORD_BACKGROUND_BOTTOM;
    GUILABEL_SetRect(MMIRECORD_BK_LABEL_CTRL_ID, &rect, FALSE);
    GUILABEL_SetBackgroundColor(MMIRECORD_BK_LABEL_CTRL_ID, 0x0040);
    MMK_SetAtvCtrl(MMIRECORD_STARTSTATUS_OPTIONMENU_WIN_ID,MMIRECORD_OPT_LIST_CTRL_ID);
    if(MMIRECORD_WORK_FORMAT_WAV == format)
    {
        s_current_index = 0;
	 GUILIST_SetCurItemIndex(MMIRECORD_OPT_LIST_CTRL_ID, s_current_index);
        GUILIST_SetSelectedItem(MMIRECORD_OPT_LIST_CTRL_ID,s_current_index,TRUE);
    }
    else if(MMIRECORD_WORK_FORMAT_MP3 == format)
    {
        s_current_index = 1;
        GUILIST_SetCurItemIndex(MMIRECORD_OPT_LIST_CTRL_ID, s_current_index);
        GUILIST_SetSelectedItem(MMIRECORD_OPT_LIST_CTRL_ID,s_current_index,TRUE);
    }
    else if(MMIRECORD_WORK_FORMAT_AMR == format)
    {
        s_current_index = 2;
        GUILIST_SetCurItemIndex(MMIRECORD_OPT_LIST_CTRL_ID, s_current_index);
        GUILIST_SetSelectedItem(MMIRECORD_OPT_LIST_CTRL_ID,s_current_index,TRUE);
    }
    else
    {
        MMIRECORD_SetRecordFileFormatType(MMIRECORD_WORK_FORMAT_WAV);
        s_current_index = 0;
	 GUILIST_SetCurItemIndex(MMIRECORD_OPT_LIST_CTRL_ID, s_current_index);
        GUILIST_SetSelectedItem(MMIRECORD_OPT_LIST_CTRL_ID,s_current_index,TRUE);
    }

    CTRLLIST_ChangeRadioListDefSoftkeyID(MMIRECORD_OPT_LIST_CTRL_ID,0,TXT_WATCH_RECORD_BACK_SOFT_KEY);
    
}

LOCAL void WatchRec_OptList_APP_WEB(void)
{
    uint16 idx = 0;
    // get current item index
    idx = GUILIST_GetCurItemIndex(MMIRECORD_OPT_LIST_CTRL_ID);
    if(0 == idx)
    {
        MMIRECORD_SetRecordFileFormatType(MMIRECORD_WORK_FORMAT_WAV);
    }
    else if(1 == idx)
    {
        MMIRECORD_SetRecordFileFormatType(MMIRECORD_WORK_FORMAT_MP3);
    }
    else
    {
        MMIRECORD_SetRecordFileFormatType(MMIRECORD_WORK_FORMAT_AMR);
    }
    MMK_CloseWin(MMIRECORD_STARTSTATUS_OPTIONMENU_WIN_ID);
}
LOCAL void WatchRec_OptList_FULL_PAINT(void)
{
    uint16 curSelection = 0;
    int32 listOffset = 0;
    uint16 listNum=0;
    curSelection = s_current_index;
    // get top item offset
    GUILIST_GetTopItemOffset(MMIRECORD_OPT_LIST_CTRL_ID, &listOffset);

    /* append list items*/
//change to radio list
    CTRLLIST_ChangeDisplayType(MMIRECORD_OPT_LIST_CTRL_ID,GUILIST_RADIOLIST_E,TRUE);
    listNum = sizeof(s_demo_radio_list_data)/sizeof(WATCHCOM_LIST_ITEM__ST);
   WatchCOM_RadioList_Create(s_demo_radio_list_data,listNum,MMIRECORD_OPT_LIST_CTRL_ID);
   GUILIST_SetSelectedItem(MMIRECORD_OPT_LIST_CTRL_ID, curSelection, TRUE);

    //set current item
    GUILIST_SetCurItemIndex(MMIRECORD_OPT_LIST_CTRL_ID, curSelection);

    // set top item offset
    GUILIST_SetTopItemOffset(MMIRECORD_OPT_LIST_CTRL_ID, listOffset);
}

LOCAL MMI_RESULT_E HandleRecordOptWinMsg(
    MMI_WIN_ID_T      winId,
    MMI_MESSAGE_ID_E    msgId,
    DPARAM                param
)
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;

    switch(msgId)
    {
        case MSG_OPEN_WINDOW:
        {
	     WatchRec_OptList_OpenWindow(winId, msgId, param);
	     break;
        }
        case MSG_FULL_PAINT:
        {
           WatchRec_OptList_FULL_PAINT();
           break;
        }
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif
       case MSG_CTL_OK:
       case MSG_APP_OK:
       case MSG_APP_WEB:
       case MSG_CTL_MIDSK:
        {
            WatchRec_OptList_APP_WEB();
            break;
        }
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMK_CloseWin(MMIRECORD_STARTSTATUS_OPTIONMENU_WIN_ID);
            break;
        }
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
WINDOW_TABLE(MMIRECORD_OPT_WIN_TAB) =
{
       WIN_HIDE_STATUS,
       CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIRECORD_BK_LABEL_CTRL_ID),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIRECORD_OPT_LIST_CTRL_ID),
	WIN_FUNC((uint32)HandleRecordOptWinMsg),
	WIN_ID(MMIRECORD_STARTSTATUS_OPTIONMENU_WIN_ID),
#ifndef TOUCH_PANEL_SUPPORT
       WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
#endif
    END_WIN
};
PUBLIC void WATCHRECORD_OpenOptList(void)
{
    MMK_CreateWin((uint32 *)MMIRECORD_OPT_WIN_TAB, PNULL);
}
