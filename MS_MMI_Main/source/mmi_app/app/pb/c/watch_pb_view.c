/*****************************************************************************
** File Name:      watch_pb_view.c                                           *
** Author:         yuming.yang                                               *
** Date:           04/05/2020                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:    This file is used to draw pb list                         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                     NAME             DESCRIPTION                     *
** 04/2020               yuming.yang            Create                       *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "window_parse.h"
#include "mmi_appmsg.h"
#include "mmi_resource.h"
#include "mmidisplay_data.h"
#include "watch_common_list.h"
#include "mmicom_string.h"
#include "mmipb_text.h"
#include "mmipb_image.h"
#include "mmipb_datalist.h"
#include "mmipb_id.h"
#include "watch_pb_view.h"
#include "watch_common.h"
#include "mmicl_export.h"
#include "mmicc_export.h"
#include "mmicom_trace.h"
#include "mmicc_image.h"
#include "mmipb_export.h"
#include "watch_cc_view.h"

//for SMS
#include "mmisms_id.h"
#include "mmisms_export.h"
#include "mmismsapp_interface.h"
#include "watch_commonwin_export.h"
#include "mmisms_image.h"
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : handle pb main window messages
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandlePbMainMenuWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          );


/*---------------------------------------------------------------------------*/
/*                          WINDOW TABLE DEFINITION                          */
/*---------------------------------------------------------------------------*/

WINDOW_TABLE(WATCHPB_MAINMENU_WIN_TAB) =
{
    WIN_ID(WATCHPB_MAINMENU_WIN_ID),
    WIN_FUNC((uint32)HandlePbMainMenuWinMsg),
    WIN_HIDE_STATUS,
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, WATCHPB_MAINMENU_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SELECT, STXT_RETURN),
    END_WIN
};


/*****************************************************************************/
//  Discription: draw pb list items
//  Global resource dependence:
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void UpdatePbMainMenuList(MMIPB_CONTACTLISTINFO_T* contact_info)
{
    MMI_CTRL_ID_T   ctrlId = WATCHPB_MAINMENU_LIST_CTRL_ID;
    MMI_STRING_T    callLogStr = {0};
    uint32          i = 0;
    MMI_STRING_T    nameStr = {0};

    GUILIST_RemoveAllItems(ctrlId);
    // append list items
    MMI_GetLabelTextByLang(TXT_PB_CALL_LOG, &callLogStr);
    WatchCOM_ListItem_Draw_1Icon_1Str(ctrlId, IAMGE_CALLLOG_LIST, callLogStr);
    if(contact_info != PNULL)
    {
        for (i = 0; i < contact_info->count; i++)
        {
            nameStr.wstr_ptr = &(contact_info->contactArr[i].name.wstr);
            nameStr.wstr_len = contact_info->contactArr[i].name.wstr_len;
            WatchCOM_ListItem_Draw_1Icon_1Str(ctrlId, WatchPB_GetImageIdByFlag(contact_info->contactArr[i].photo_id_flag, FALSE), nameStr);
        }
    }
}

/*****************************************************************************/
//  Description : handle pb main window messages
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandlePbMainMenuWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          )
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMIPB_CONTACTLISTINFO_T*    userData = (MMIPB_CONTACTLISTINFO_T *)MMK_GetWinAddDataPtr(win_id);
#ifdef SCREEN_SHAPE_CIRCULAR
    GUI_RECT_T                  list_rect = WATCHLIST_RECT;
#endif

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMK_SetAtvCtrl(win_id, WATCHPB_MAINMENU_LIST_CTRL_ID);
#ifdef SCREEN_SHAPE_CIRCULAR
            GUILIST_SetRect(WATCHPB_MAINMENU_LIST_CTRL_ID, &list_rect);
#endif
            GUILIST_SetMaxItem(WATCHPB_MAINMENU_LIST_CTRL_ID, WATCHPB_CONTACT_NUM_MAX+1, FALSE);
            break;
        }
        case MSG_CLOSE_WINDOW:
        {
            if (userData != NULL)
            {
                SCI_FREE(userData);
            }
            break;
        }
        case MSG_FULL_PAINT:
        {
#ifdef SCREEN_SHAPE_CIRCULAR
            WATCHCOM_DisplayBackground(win_id);
#endif
            UpdatePbMainMenuList(userData);
            break;
        }
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        {
            WatchPB_MainMenuWin_Exit();
            break;
        }

        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        case MSG_CTL_PENOK:
        {
            uint32 idx = GUILIST_GetCurItemIndex(WATCHPB_MAINMENU_LIST_CTRL_ID);
            if(idx == 0)
            {
                //calllog
                if(0 == MMICL_GetAllRecordsNum())
                {
                    WatchCL_EmptyList_NoteWin();
                }
                else
                {
                    WatchCL_ListWin_Enter();
                }
            }
            else
            {
                if (userData != NULL)
                {
                    WatchCC_MoCallQueryWin_Enter(&userData->contactArr[idx -1]);
                }
            }
            break;
        }

        case MSG_APP_GREEN:
        {
            uint32 idx = GUILIST_GetCurItemIndex(WATCHPB_MAINMENU_LIST_CTRL_ID);
            if(idx == 0)
            {
                //do nothing
            }
            else
            {
                if (userData != NULL)
                {
                    uint8     numBuf[MMIPB_NUMBER_MAX_STRING_LEN+2] = {0};
                    uint8     numLen = 0;
                    MMIPB_BCDNumberToUTF8(userData->contactArr[idx -1].number, MMIPB_NUMBER_MAX_STRING_LEN, numBuf);
                    numLen = MMIPB_GetBCDNumberStrlen(userData->contactArr[idx -1].number);
                    //make call
                    WatchAPICC_MakeCall(MN_DUAL_SYS_1, &numBuf, numLen, PNULL, PNULL, CC_SIM1_CALL, CC_CALL_NORMAL_CALL, PNULL);
                }
            }
            break;
        }
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}



/*---------------------------------------------------------------------------*/
/*                             PUBLIC FUNCTION                               */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description: Open pb main menu window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchPB_MainMenuWin_Enter(void)
{
    MMIPB_CONTACT_T contactInfo = {0};
    uint32          contactsNum = WatchPB_SearchPresetContact();
    uint32          arrayIdx[WATCHPB_CONTACT_NUM_MAX] = {0};
    uint32          i = 0;
    MMIPB_CONTACTLISTINFO_T *viewData = NULL;

    WatchPB_GetPresetContactArray(arrayIdx);

    viewData = SCI_ALLOC_APPZ(sizeof(MMIPB_CONTACTLISTINFO_T));
    SCI_ASSERT(viewData != NULL);
    SCI_ASSERT(contactsNum <= WATCHPB_CONTACT_NUM_MAX);

    for (i = 0; i < contactsNum; i++)
    {
        SCI_MEMSET(&contactInfo, 0x00, sizeof(MMIPB_CONTACT_T));
        WatchPB_GetPresetContactInfoByIndex(arrayIdx[i], &contactInfo);
        viewData->contactArr[i] = contactInfo;
    }
    viewData->count = contactsNum;
    MMK_CreateWin((uint32 *)WATCHPB_MAINMENU_WIN_TAB, viewData);
}

/*****************************************************************************/
//  Description : Close pb main menu window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchPB_MainMenuWin_Exit(void)
{
    if(MMK_IsOpenWin(WATCHPB_MAINMENU_WIN_ID))
    {
        MMK_CloseWin(WATCHPB_MAINMENU_WIN_ID);
    }
}


#define SMSCONTACT
#ifdef SMSCONTACT
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description : Draw Sms Main List Item
//  Parameter: [In] ctrl Id
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL void DrawSmsContactListItem( MMIPB_CONTACTLISTINFO_T* contact_info )
{
    uint32          i = 0;
    MMI_CTRL_ID_T ctrlId = WATCHPB_CONTACT_LIST_CTRL_ID;
    MMI_STRING_T    nameStr = {0};
    
    GUILIST_RemoveAllItems(ctrlId);
    
    if(contact_info != PNULL)
    {
        for (i = 0; i < contact_info->count; i++)
        {
            nameStr.wstr_ptr = &(contact_info->contactArr[i].name.wstr);
            nameStr.wstr_len = contact_info->contactArr[i].name.wstr_len;
            WatchCOM_ListItem_Draw_1Icon_1Str(ctrlId, WatchPB_GetImageIdByFlag(contact_info->contactArr[i].photo_id_flag, FALSE), nameStr);
        }
    }
}

LOCAL void SMS_Contact_OPEN_WINDOW( void)
{
    DrawSmsContactListItem(WATCHPB_CONTACT_LIST_CTRL_ID);
    MMK_SetAtvCtrl(WATCHPB_CONTACT_WIN_ID, WATCHPB_CONTACT_LIST_CTRL_ID);
}

LOCAL void SMS_Contact_APP_WEB( void )
{
#if 1
    MMISMS_PRESET_BUFF_T s_preset_word = {0};
    uint8 numbuf[MMIPB_NUMBER_MAX_STRING_LEN+2]={0};
    uint8 number_len = 0;//要发送号码的长度号码的长度
    MMISMS_DEST_ADDR_INFO_T contact_info = {0};
    uint32 idx = GUILIST_GetCurItemIndex(WATCHPB_CONTACT_LIST_CTRL_ID);
    MMIPB_CONTACTLISTINFO_T *win_data_ptr = PNULL;

    win_data_ptr = (MMIPB_CONTACTLISTINFO_T *)MMK_GetWinAddDataPtr(WATCHPB_CONTACT_WIN_ID);
    SCI_ASSERT(win_data_ptr != NULL);

    MMIPB_BCDNumberToUTF8(win_data_ptr->contactArr[idx].number, MMIPB_NUMBER_MAX_STRING_LEN, numbuf);
    number_len = (uint8)strlen((char *)numbuf);
    if(0 == number_len)//fix bug:1519620
    {
        TRACE_APP_MESSAGES("phone number empty!");
        MMISMS_OpenNoteWin( MMISMS_NOTE_WIN_ID,TXT_COMMON_EMPTY_NUM,0 );
        return;
    }
    //save phone num
    MMISMS_SetAddressToMessage(
            (uint8 *)numbuf, 
            (uint8)strlen((char *)numbuf),
            &contact_info.dest_list );
    
    //get preset word
    s_preset_word =MMISMS_GetPresetForSend();
    
    //send sms
    MMISMSAPP_SendSmsWithData(MN_DUAL_SYS_1,
            FALSE,
            &contact_info,//sms->addr
            s_preset_word.preset_buff,//freq->data
            s_preset_word.preset_len,
            PNULL
            );
#endif
}

/*****************************************************************************/
//  Description : to handle the SMS main menu list window message
//  Parameter: [In] win Id
//             [In] msg id
//             [In] param
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSmsContactListWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM                param
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMIPB_CONTACTLISTINFO_T*    userData = (MMIPB_CONTACTLISTINFO_T *)MMK_GetWinAddDataPtr(win_id);

    TRACE_APP_MESSAGES("HandleSmsContactListWinMsg msg_id = 0x%x", msg_id);
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMK_SetAtvCtrl(win_id, WATCHPB_CONTACT_LIST_CTRL_ID);
            GUILIST_SetMaxItem(WATCHPB_CONTACT_LIST_CTRL_ID, WATCHPB_CONTACT_NUM_MAX+1, FALSE);
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            if (userData != NULL)
            {
                SCI_FREE(userData);
            }
            break;
        }

        case MSG_FULL_PAINT:
        {
            DrawSmsContactListItem(userData);
            break;
        }

#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif 
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        {
            SMS_Contact_APP_WEB();
            MMK_CloseWin(win_id);
            break;
        }

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}


/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
// the window for SMS contact menu
WINDOW_TABLE( WATCHPB_CONTACT_WIN_TAB ) =
{
    WIN_FUNC((uint32) HandleSmsContactListWinMsg),
    WIN_HIDE_STATUS,
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,  WATCHPB_CONTACT_LIST_CTRL_ID),
    WIN_ID( WATCHPB_CONTACT_WIN_ID ),
    WIN_SOFTKEY(TXT_NULL, TXT_SEND, STXT_RETURN),
    END_WIN
};


/*****************************************************************************/
//  Description : enter SMS Contact list win
//  Parameter: [In] None
//             [Out] None
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void WatchPB_ContactWin_Enter(void)
{
    MMIPB_CONTACT_T contactInfo = {0};
    uint32          contactsNum = WatchPB_SearchPresetContact();
    uint32          arrayIdx[WATCHPB_CONTACT_NUM_MAX] = {0};
    uint32          i = 0;
    MMIPB_CONTACTLISTINFO_T *pWinData = NULL;

    WatchPB_GetPresetContactArray(arrayIdx);

    pWinData = SCI_ALLOC_APPZ(sizeof(MMIPB_CONTACTLISTINFO_T));
    SCI_ASSERT(pWinData != NULL);
    SCI_ASSERT(contactsNum <= WATCHPB_CONTACT_NUM_MAX);

    for (i = 0; i < contactsNum; i++)
    {
        SCI_MEMSET(&contactInfo, 0x00, sizeof(MMIPB_CONTACT_T));
        WatchPB_GetPresetContactInfoByIndex(arrayIdx[i], &contactInfo);
        pWinData->contactArr[i] = contactInfo;
    }
    pWinData->count = contactsNum;

    if(contactsNum)
    {
        MMK_CreateWin((uint32 *)WATCHPB_CONTACT_WIN_TAB, pWinData);
    }
    else
    {
        if (pWinData != NULL)
        {
            SCI_FREE(pWinData);
        }
        //tip empty note win 
        {
            MMI_WIN_ID_T noteId = WATCHPB_EMPTY_NOTE_WIN_ID;
            MMI_STRING_T text_str = {0};
            WATCH_SOFTKEY_TEXT_ID_T softkey_text = {NULL ,NULL ,NULL};
            MMI_GetLabelTextByLang(TXT_BLANK,&text_str);
            WatchCOM_NoteWin_FullScreen_Enter( noteId,0,&text_str,IMAGE_SMS_PRESET_NONE,0,softkey_text,PNULL );
        }
    }

}

#endif
