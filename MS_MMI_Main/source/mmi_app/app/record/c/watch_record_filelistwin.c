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
#include "mmifmm_export.h"
#include "watch_record_api.h"
#include "watch_commonwin_export.h"

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
MMIFMM_DATA_INFO_T s_fmm_list_data = {0};
uint32 s_cur_index = 0;
FILEARRAY s_locallist_array_ptr = PNULL;
LOCAL uint32 s_list_num = 0;
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL void WatchRec_FileList_FULL_PAINT(void);
LOCAL void WatchRec_FileList_APP_WEB(void);
LOCAL void WatchRec_FileList_OpenWindow(MMI_WIN_ID_T      winId, MMI_MESSAGE_ID_E msgId, DPARAM param);
LOCAL void WatchRec_FileList_CloseWindow(void);
LOCAL MMI_RESULT_E HandleRecordListWinMsg(
    MMI_WIN_ID_T      winId,
    MMI_MESSAGE_ID_E    msgId,
    DPARAM                param
);
LOCAL void WatchRec_FileList_Callback(void);
LOCAL void WatchRec_FileList_OpenList(void);
LOCAL void WatchRec_IntTime2Wstr(uint32 itime, wchar *ctime);
LOCAL void WatchRec_BeginFindFile(MMI_WIN_ID_T winId);
LOCAL BOOLEAN WatchRec_LocalList_GetInfo(
                                    int32               locallist_index,
                                    FILEARRAY_DATA_T    *f_array_data_ptr
                                    );
LOCAL uint32 WatchRec_LocalList_GetNum( void );
LOCAL void WatchRec_LocalList_Release(void);
LOCAL void WatchRec_LocalList_Reflash(void);


LOCAL void WatchRec_LocalList_Reflash(void)
{
    GUILIST_RemoveAllItems(WATCHRECORD_LIST_CTRL_ID);
    WatchRec_LocalList_Release();
    WatchRec_BeginFindFile(MMIRECORD_LIST_WIN_ID);
}
LOCAL void WatchRec_LocalList_Release(void)
{
    if(PNULL != s_locallist_array_ptr)
    {
        MMIAPIFILEARRAY_Destroy(&s_locallist_array_ptr);
        s_locallist_array_ptr = PNULL;
    }
}

LOCAL uint32 WatchRec_LocalList_GetNum( void )
{
    uint32  locallist_num = 0;

    if(PNULL != s_locallist_array_ptr)
    {
        locallist_num = MMIAPIFILEARRAY_GetArraySize(s_locallist_array_ptr);
    }

    return locallist_num;
}

LOCAL BOOLEAN WatchRec_LocalList_GetInfo(
                                    int32               locallist_index,
                                    FILEARRAY_DATA_T    *f_array_data_ptr
                                    )
{
    BOOLEAN    result = FALSE;

    if (PNULL != f_array_data_ptr && PNULL != s_locallist_array_ptr && locallist_index >= 0)
    {
        result = MMIAPIFILEARRAY_Read(s_locallist_array_ptr, locallist_index, f_array_data_ptr);
    }
    return result;
}


LOCAL void WatchRec_FileList_Callback(void)
{
    uint16 idx = 0;

    // get current item index
    idx = GUILIST_GetCurItemIndex(WATCHRECORD_LIST_CTRL_ID);
    s_cur_index = idx;
    //MMK_CloseWin(MMIRECORD_LIST_WIN_ID);
    WATCHRECORD_OpenPlayWin();
}
LOCAL void WatchRec_BeginFindFile(MMI_WIN_ID_T winId)
{
    BOOLEAN                 load_result             = FALSE;
    MMIFMM_FILTER_T         filter                  = {0};
    wchar path_name[MMIFMM_PATHNAME_LEN + 1] = {0};


    if (PNULL == s_locallist_array_ptr)
    {
       s_locallist_array_ptr = MMIAPIFILEARRAY_Create();
    }

    if(PNULL == s_locallist_array_ptr)
    {
        return;
    }
    MMI_MEMCPY(filter.filter_str,17,"*.wav *.mp3 *.amr",17,sizeof(uint8)*17);

    if (MMISD_IsSDExistEx())
    {
	SCI_MEMCPY(path_name, L"E:\\Audio", sizeof(wchar)*8);
    }
    else
    {
    	SCI_MEMCPY(path_name, L"D:\\Audio", sizeof(wchar)*8);
    }

    // 1389172: 在搜索时指定排序方式
    MMIAPIFMM_SearchFileInPathAndSort(
        path_name,
        MMIAPICOM_Wstrlen(path_name),
        &filter,
        FALSE,
        FUNC_FIND_FILE,
        s_locallist_array_ptr,
        winId,
        MSG_WATCHRECORD_LOAD_FINISH,
        FILEARRAY_SORT_TIME_DESCEND);
}
LOCAL void WatchRec_FileList_OpenWindow(MMI_WIN_ID_T      winId, MMI_MESSAGE_ID_E msgId, DPARAM param)
{
    GUI_RECT_T rect = {0};
    MMI_STRING_T textLeft = {0};
    MMI_STRING_T textMid = {0};
    MMI_STRING_T textRight = {0};
    wchar backText[] = L"Back";
    MMI_STRING_T strTitle = {0};
    MMI_STRING_T text_str = {0};
    WATCH_SOFTKEY_TEXT_ID_T softkey_text = {NULL ,NULL ,NULL};
    uint32 i = 0;
    MMI_STRING_T str1 = {0};
    MMI_STRING_T str2 = {0};

    s_list_num = WatchRec_LocalList_GetNum();

    SCI_TRACE_LOW("WatchRec_FileList_OpenList() s_list_num = %d.",  s_list_num);

    if(0 == s_list_num)
    {
        MMI_GetLabelTextByLang(TXT_RECORD_NORECORD, &strTitle);
        WatchCOM_NoteWin_FullScreen_Enter(MMIRECORD_WORK_QUERY_WIN_ID,&text_str,&strTitle,res_recorder_ic_rec,0,softkey_text,PNULL);
        MMK_CloseWin(winId);
        return;
    }

    rect.left = 0;
    rect.top = 0;
    rect.right = RECORD_BACKGROUND_RIGHT;
    rect.bottom = RECORD_BACKGROUND_BOTTOM;
    GUILABEL_SetRect(MMIRECORD_BK_LABEL_CTRL_ID, &rect, FALSE);
    GUILABEL_SetBackgroundColor(MMIRECORD_BK_LABEL_CTRL_ID, 0x0040);
    MMK_SetAtvCtrl(winId,WATCHRECORD_LIST_CTRL_ID);
    GUILIST_SetMaxItem(WATCHRECORD_LIST_CTRL_ID, s_list_num, TRUE);//must before append list
    for(i=0;i<s_list_num;i++)
    {
        WatchCOM_ListItem_Draw_2Str(WATCHRECORD_LIST_CTRL_ID, str1, str2);
    }

    textRight.wstr_ptr = backText;
    textRight.wstr_len = MMIAPICOM_Wstrlen(backText);
    GUIWIN_SetSoftkeyButtonTextPtr(winId, textRight, 2, TRUE);
}
LOCAL void WatchRec_FileList_CloseWindow(void)
{
    WatchRec_LocalList_Release();
}
LOCAL void WatchRec_FileList_OpenList(void)
{
    int16 i = 0;
    wchar ctime[9] = {0};
    MMI_STRING_T str1 = {0};
    MMI_STRING_T str2 = {0};
    uint32 user_data = 0;
    MMI_STRING_T strTitle = {0};
    MMI_STRING_T text_str = {0};
    WATCH_SOFTKEY_TEXT_ID_T softkey_text = {NULL ,NULL ,NULL};
    int16 display_num = 0;

    s_list_num = WatchRec_LocalList_GetNum();
    SCI_TRACE_LOW("WatchRec_FileList_OpenList() s_list_num = %d.",  s_list_num); 
	
    if(0 == s_list_num)
    {
        MMI_GetLabelTextByLang(TXT_RECORD_NORECORD, &strTitle);
        WatchCOM_NoteWin_FullScreen_Enter(MMIRECORD_WORK_QUERY_WIN_ID,&text_str,&strTitle,res_recorder_ic_rec,0,softkey_text,PNULL);
        MMK_CloseWin(MMIRECORD_LIST_WIN_ID);
        return;
    }
    user_data = (uint32)(WatchRec_FileList_Callback);
    if(s_list_num > 255)
    {
        display_num = 255;
    }
    else
    {
        display_num = s_list_num;
    }
    for(i=0;i<display_num;i++)
    {
        FILEARRAY_DATA_T f_array_data_ptr = {0};
        wchar fileName[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
        uint16 fileNameLen = 0;
        MMISRVAUD_CONTENT_INFO_T info = {0};

        WatchRec_LocalList_GetInfo(i,&f_array_data_ptr);
        MMIAPIFMM_SplitFullPath(
                    f_array_data_ptr.filename,
                    MMIAPICOM_Wstrlen(f_array_data_ptr.filename),
                    PNULL,
                    PNULL,
                    PNULL,
                    PNULL,
                    fileName,
                    &fileNameLen);

        str1.wstr_len = fileNameLen;
        str1.wstr_ptr = fileName;
        MMISRVAUD_GetFileContentInfo(f_array_data_ptr.filename,MMIAPICOM_Wstrlen(f_array_data_ptr.filename),&info);
        WatchRec_IntTime2Wstr(info.total_time, ctime);
        str2.wstr_ptr = ctime;
        str2.wstr_len = 8;
        WatchCOM_ListItem_Draw_2Str(WATCHRECORD_LIST_CTRL_ID,str1,str2);
        GUILIST_SetItemUserData(WATCHRECORD_LIST_CTRL_ID, i, &user_data);
    }
    
}
LOCAL void WatchRec_FileList_APP_WEB(void)
{
    WatchCOM_List_Item_CallBack(WATCHRECORD_LIST_CTRL_ID);
    //MMK_CloseWin(MMIRECORD_LIST_WIN_ID);
}
LOCAL void WatchRec_FileList_FULL_PAINT(void)
{
    // remove list
    GUILIST_RemoveAllItems(WATCHRECORD_LIST_CTRL_ID);

    // append list
    WatchRec_FileList_OpenWindow(MMIRECORD_LIST_WIN_ID, 0, PNULL);
}

/**--------------------------------------------------------------------------*
**                          WINDOW  DEFINITION                               *
**--------------------------------------------------------------------------*/
WINDOW_TABLE(MMIRECORD_LIST_WIN_TAB) =
{
       WIN_HIDE_STATUS,
       CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIRECORD_BK_LABEL_CTRL_ID),
	CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, WATCHRECORD_LIST_CTRL_ID),
	WIN_FUNC((uint32)HandleRecordListWinMsg),
	WIN_ID(MMIRECORD_LIST_WIN_ID),
       WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
    END_WIN
};

LOCAL MMI_RESULT_E  Handle_WatchRecWaitingCallback(
                                                    MMI_WIN_ID_T win_id,
                                                    MMI_MESSAGE_ID_E msg_id,
                                                    DPARAM param
                                                    )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    static BOOLEAN isStopByUser = FALSE;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            isStopByUser = FALSE;
            break;
        }

        case MSG_WATCHRECORD_LOAD_START:
        {
            WatchRec_BeginFindFile(win_id);
            break;
        }

        case MSG_WATCHRECORD_LOAD_FINISH:
        {
            // 当用户没有主动按下退出时，关闭wating窗口并进行排序
            if (FALSE == isStopByUser)
            {
                if (TRUE == MMK_IsFocusWin(win_id))
                {
                    // 1389172: no need to sort
                    //MMIAPIFILEARRAY_Sort(s_locallist_array_ptr, FILEARRAY_SORT_TIME_DESCEND);

                    // open file list window
                    if (TRUE == MMK_IsOpenWin(MMIRECORD_LIST_WIN_ID))
                    {
                        MMK_CloseWin(MMIRECORD_LIST_WIN_ID);
                    }
                    MMK_CreateWin((uint32 *)MMIRECORD_LIST_WIN_TAB, PNULL);

                    // close waiting window
                    MMK_CloseWin(win_id);
                }
            }
            break;
        }

        case MSG_APP_RED:
        {
            isStopByUser = TRUE;

            // stop finding
            MMIAPIFMM_SearchFileStop();

            recode = MMI_RESULT_FALSE;
            break;
        }

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        {
            isStopByUser = TRUE;

            // stop finding
            MMIAPIFMM_SearchFileStop();

            MMK_CloseWin(win_id);
            break;
        }
        case MSG_CLOSE_WINDOW:
        {
            isStopByUser = FALSE;
            break;
        }

        default:
            break;
    }
}

LOCAL void WatchRec_AppendFileList(uint16 itemIdx, MMI_CTRL_ID_T ctrlId)
{
    FILEARRAY_DATA_T f_array_data_ptr = {0};
    wchar fileName[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16 fileNameLen = 0;
    MMISRVAUD_CONTENT_INFO_T info = {0};
    MMI_STRING_T str1 = {0};
    MMI_STRING_T str2 = {0};
    uint32 user_data = 0;
    wchar ctime[9] = {0};
    GUILIST_ITEM_DATA_T itemData = {0};

    WatchRec_LocalList_GetInfo(itemIdx, &f_array_data_ptr);
    MMIAPIFMM_SplitFullPath(
                f_array_data_ptr.filename,
                MMIAPICOM_Wstrlen(f_array_data_ptr.filename),
                PNULL,
                PNULL,
                PNULL,
                PNULL,
                fileName,
                &fileNameLen);

    str1.wstr_len = fileNameLen;
    str1.wstr_ptr = fileName;
    MMISRVAUD_GetFileContentInfo(f_array_data_ptr.filename,MMIAPICOM_Wstrlen(f_array_data_ptr.filename),&info);
    WatchRec_IntTime2Wstr(info.total_time, ctime);
    str2.wstr_ptr = ctime;
    str2.wstr_len = 8;
    user_data = (uint32)(WatchRec_FileList_Callback);
    itemData.softkey_id[LEFT_BUTTON] = TXT_NULL;
    itemData.softkey_id[MIDDLE_BUTTON] = TXT_NULL;
    itemData.softkey_id[RIGHT_BUTTON] = TXT_COMMON_COMMON_BACK;
    itemData.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    itemData.item_content[0].item_data.text_buffer = str1;
    itemData.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    itemData.item_content[1].item_data.text_buffer = str2;
    GUILIST_SetItemData(WATCHRECORD_LIST_CTRL_ID, &itemData, itemIdx);
    GUILIST_SetItemUserData(WATCHRECORD_LIST_CTRL_ID, itemIdx, &user_data);
}

LOCAL MMI_RESULT_E HandleRecordListWinMsg(
    MMI_WIN_ID_T      winId,
    MMI_MESSAGE_ID_E    msgId,
    DPARAM                param
)
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;
    GUILIST_NEED_ITEM_DATA_T *item_data_ptr = PNULL;
    MMI_CTRL_ID_T ctrlId = WATCHRECORD_LIST_CTRL_ID;

    switch(msgId)
    {
        case MSG_OPEN_WINDOW:
        {
            WatchRec_FileList_OpenWindow(winId, msgId, param);
            break;
        }
        case MSG_FULL_PAINT:
        {
            WatchRec_FileList_FULL_PAINT();
            break;
        }

        case MSG_CTL_LIST_NEED_ITEM_DATA: // Bug-1370448
        {
            item_data_ptr = (GUILIST_NEED_ITEM_DATA_T *)param;
            if (PNULL == item_data_ptr)
            {
                SCI_TRACE_LOW("HandleRecordListWinMsg: MSG_CTL_LIST_NEED_ITEM_DATA param is PNULL!");
                break;
            }

            WatchRec_AppendFileList(item_data_ptr->item_index, ctrlId);
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
            WatchRec_FileList_APP_WEB();
            break;
        }
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMK_CloseWin(MMIRECORD_LIST_WIN_ID);
            break;
        }
        case MSG_CLOSE_WINDOW:
            WatchRec_FileList_CloseWindow();
        break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

LOCAL void WatchRec_IntTime2Wstr(uint32 itime, wchar *ctime)
{
    uint32  totalHours = (itime / 3600);
    uint32  totalMinutes = ((itime % 3600) / 60);
    uint32  totalSeconds = ((itime % 3600) % 60);
    wchar text[] = {'0','0',':','0','0',':','0','0',0};
    wchar hour[] = {'0','1','2','3','4','5','6','7','8','9',0};
    wchar minH[] = {'0','1','2','3','4','5',0};
    wchar minL[] = {'0','1','2','3','4','5','6','7','8','9',0};
    wchar secH[] = {'0','1','2','3','4','5',0};
    wchar secL[] = {'0','1','2','3','4','5','6','7','8','9',0};
    uint8 second_low = (uint8)(totalSeconds % 10);
    uint8 second_high = (uint8)(totalSeconds / 10);
    uint8 minute_low = (uint8)(totalMinutes % 10);
    uint8 minute_high = (uint8)(totalMinutes / 10);
    uint8 hour_low = (uint8)(totalHours % 10);
    uint8 hour_high = (uint8)(totalHours / 10);

    text[0] = hour[hour_high];
    text[1] = hour[hour_low];
    text[3] = minH[minute_high];
    text[4] = minL[minute_low];
    text[6] = secH[second_high];
    text[7] = secL[second_low];
    MMIAPICOM_Wstrcpy(ctime,text);
}

/*****************************************************************************/
//  Description : open wating window before record list
//  Para : none
//  return : none
//  Author: yanli.yang
//  Note:
/*****************************************************************************/
PUBLIC void WATCHRECORD_OpenWaitingWin(void)
{
    MMI_STRING_T tipStr = {0};
    WATCH_SOFTKEY_TEXT_ID_T softKey = {0};

    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &tipStr);

    // open waiting window
    WatchCOM_WaitingWin_1Line_Enter(MMIRECORD_WAITING_WIN_ID, &tipStr, 0, softKey, Handle_WatchRecWaitingCallback);

    MMK_PostMsg(MMIRECORD_WAITING_WIN_ID, MSG_WATCHRECORD_LOAD_START, PNULL, 0);
}

PUBLIC void WATCHRECORD_OpenFileList(void)
{
    MMK_CreateWin((uint32 *)MMIRECORD_LIST_WIN_TAB, PNULL);
}
  /*****************************************************************************/
//  Description : get file list current index
//  Para : none
//  return : current index
//  Author: jiaxin.sun
//  Note:
/*****************************************************************************/
PUBLIC uint32 WATCHRECORD_GetFileListCurIndex(void)
{
    return s_cur_index;
}

  /*****************************************************************************/
//  Description : get file list total number
//  Para : none
//  return : totle number
//  Author: jiaxin.sun
//  Note:
/*****************************************************************************/
PUBLIC uint32 WATCHRECORD_GetFileListNum(void)
{
    return s_list_num;
}

/*****************************************************************************/
//  Description : get file list file array pointer
//  Para : none
//  return : file array pointer
//  Author: yanli.yang
//  Note:
/*****************************************************************************/
PUBLIC FILEARRAY WATCHRECORD_GetFileArray(void)
{
    return s_locallist_array_ptr;
}

