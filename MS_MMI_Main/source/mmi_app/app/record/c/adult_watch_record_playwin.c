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
#include "mmidisplay_data.h"
#include "mmimp3_text.h"
#include "mmiidle_statusbar.h"
//#include "mmidropdownwin_export.h"

//#include "mmiap_appmain.h "

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define RECORD_PLAY_CURTIME_LEFT DP2PX_VALUE(20)
#define RECORD_PLAY_CURTIME_TOP DP2PX_VALUE(94)
#define RECORD_PLAY_CURTIME_BOTTOM DP2PX_VALUE(130)
#define RECORD_PLAY_CURTIME_RIGHT DP2PX_VALUE(220)

#define RECORD_PLAY_TOLTIME_TOP DP2PX_VALUE(122)
#define RECORD_PLAY_TOLTIME_BOTTOM DP2PX_VALUE(142)
#define RECORD_PLAY_TOLTIME_RIGHT DP2PX_VALUE(240)

#ifdef ADULT_WATCH_SUPPORT
#define RECORD_PLAY_TITLE_TOP DP2PX_VALUE(24)
#define RECORD_PLAY_TITLE_BOTTOM DP2PX_VALUE(44)
#define RECORD_PLAY_TITLE_RIGHT DP2PX_VALUE(240)
#else
#define RECORD_PLAY_TITLE_TOP DP2PX_VALUE(48)
#define RECORD_PLAY_TITLE_BOTTOM DP2PX_VALUE(72)
#define RECORD_PLAY_TITLE_RIGHT DP2PX_VALUE(240)
#ifdef SCREEN_SHAPE_CIRCULAR
#define RECORD_PLAY_NAME_MAX_LEN 30 //手表项目中RECODER文件的名称是写死的，REC.001.WAV-REC.999.WAV之间
#endif
#endif
#ifdef WIN32
#define RECORD_PLAY_TIMER_TIMEOUT 1000
#else
#define RECORD_PLAY_TIMER_TIMEOUT 300
#endif
#define PROGRESS_BAR_THICK 5
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef void(*WATCH_REC_PLAY_CALLBACK_PFUNC)(MMISRVAUD_REPORT_RESULT_E result, DPARAM param);

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL WATCH_RECORD_PLAY_STATE_E s_play_state = WATCH_RECORD_PLAY_STATE_NORMAL;
extern FILEARRAY s_locallist_array_ptr;
LOCAL uint8  s_play_timer_id = 0;  //play timer
LOCAL uint32 s_play_cur_time = 0;
LOCAL uint32 s_play_tol_time = 0;
//LOCAL uint32 s_cur_play_index = 0;
LOCAL MMISRV_HANDLE_T s_record_play_handle = 0;
#if defined(SCREEN_SHAPE_CIRCULAR) && !defined(ADULT_WATCH_SUPPORT)
LOCAL wchar s_cur_play_name[RECORD_PLAY_NAME_MAX_LEN + 1] = {0};
LOCAL wchar s_input_name[RECORD_PLAY_NAME_MAX_LEN + 1] = {0};
#else
LOCAL wchar s_cur_play_name[MMIFMM_FILE_FILE_NAME_MAX_LEN + 1] = {0};
LOCAL wchar s_input_name[MMIFMM_FILE_FILE_NAME_MAX_LEN + 1] = {0};
#endif
LOCAL uint16 s_cur_play_name_len = 0;
LOCAL BOOLEAN s_enter_from_ex = FALSE;//′óía2???è?2￥·??÷
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL void WatchRec_PlayWin_FULL_PAINT(void);
LOCAL void WatchRec_PlayWin_APP_OK(void);
LOCAL void WatchRec_PlayWin_OpenWindow(void);
LOCAL MMI_RESULT_E HandleRecordPlayWinMsg(
    MMI_WIN_ID_T      winId,
    MMI_MESSAGE_ID_E    msgId,
    DPARAM                param
);
LOCAL void WatchRec_PlayWin_Callback(void);
LOCAL MMI_RESULT_E HandleRecordPlayWinDeleteOkMsg(
    MMI_WIN_ID_T      winId,
    MMI_MESSAGE_ID_E    msgId,
    DPARAM                param
);
LOCAL void WatchRec_IntTime2Wstr(uint32 itime, wchar *ctime);
LOCAL void WatchRec_PlayWin_Delete(void);
LOCAL void WatchRec_PlayWin_StartDelete(MMI_WIN_ID_T winId);
LOCAL MMI_RESULT_E WatchRec_PlayWin_Query_Callback(MMI_WIN_ID_T winId, MMI_MESSAGE_ID_E msgId, DPARAM param);
LOCAL BOOLEAN WatchRec_LocalList_GetInfo(
                                    int32               locallist_index,
                                    FILEARRAY_DATA_T    *f_array_data_ptr
                                    );
LOCAL void WatchRec_StartPlayTimer(void);
LOCAL void WatchRec_StopPlayTimer(void);
LOCAL BOOLEAN WatchRec_PlayRecordFile(void);
LOCAL void WatchRec_PlayWin_TimerCallBack(void);
LOCAL void WatchRec_GetPlayTime(MMIRECORD_TIME_T* timPtr);
LOCAL void WatchRec_ResetPlayTimer(void);
LOCAL void WatchRec_StopPlay(void);
LOCAL uint16 GetPlayTime(MMISET_ALL_RING_TYPE_E type, wchar *name_ptr, uint32 name_len);
LOCAL void Watch_StopPlayFile(void);
LOCAL BOOLEAN Watch_PlayRecordFile(
                        MMISET_ALL_RING_TYPE_E         ring_type,
                        BOOLEAN                        is_a2dp,
                        BOOLEAN                        is_consume,
                        const wchar                    *full_path_ptr,
                        uint16                         full_path_len,
                        uint32                         play_times,
                        uint32                         play_offset,
                        uint32                         vol,
                        MMISET_RING_CALLBACK_PFUNC    call_back
                        );
LOCAL uint32 WatchRec_GetPlayCurTime(void);
LOCAL BOOLEAN WatchRec_HandleAudioCallBack(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param);
LOCAL void WatchRec_PlaywinDrawBG(void);
LOCAL void WatchRec_ResumePlay(void);
//LOCAL void WatchRec_PlayWin_DrawProgressBar(uint16 percentage);
LOCAL uint32 WatchRec_GetCurSecondTime(void);
LOCAL void WatchRec_PausePlay(void);
LOCAL void WatchRec_PausePlayTimer(void);
LOCAL void WatchRec_ResumePlayTimer(void);
LOCAL void WatchRec_PlayWin_APP_Web(void);
LOCAL void Handle_TOUCH_PANEL_SUPPORT( DPARAM param);

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

LOCAL void WatchRec_PlayWin_Delete(void)
{
    MMI_STRING_T strTitle = {0};
    if((WATCH_RECORD_PLAY_STATE_PLAYING == s_play_state) ||(WATCH_RECORD_PLAY_STATE_PAUSE == s_play_state))
    {
        MMI_GetLabelTextByLang(TXT_MP3_DEL_OPENED_MUSIC_FAIL, &strTitle);
#ifdef ADULT_WATCH_SUPPORT
        Adult_WatchCOM_NoteWin_2Line_Toast_Enter(
                                        MMIRECORD_WORK_QUERY_WIN_ID,
                                        &strTitle,
                                        0,
                                        PNULL
                                     );
#endif
        SCI_TRACE_LOW("WatchRec_PlayWin_Delete delete Fail!");
        return;
    }
#ifdef ADULT_WATCH_SUPPORT
    AdultWatchCOM_QueryWin_1Btn_Enter(MMIRECORD_WORK_QUERY_WIN_ID,0,res_aw_recorder_ic_delete,WatchRec_PlayWin_Query_Callback);
#else
    {
        WATCH_SOFTKEY_TEXT_ID_T softkey = {0};
        MMI_STRING_T strTitle = {0};
        MMI_GetLabelTextByLang(TXT_RECORD_DELETE_OR_NOT, &strTitle);
        WatchCOM_QueryWin_2Btn_Enter(MMIRECORD_WORK_QUERY_WIN_ID,&strTitle,0,0,res_aw_recorder_ic_back,res_aw_recorder_ic_delete,softkey,WatchRec_PlayWin_Query_Callback);
    }
#endif
}
LOCAL void WatchRec_PlayWin_StartDelete(MMI_WIN_ID_T winId)
{
    FILEARRAY_DATA_T f_array_data_ptr = {0};
    uint32 index = 0;
    MMI_STRING_T strTitle = {0};
	
    index = WATCHRECORD_GetFileListCurIndex();
    WatchRec_LocalList_GetInfo(index,&f_array_data_ptr);
    if(MMIAPIFMM_DeleteFileAsyn(f_array_data_ptr.filename,MMIAPICOM_Wstrlen(f_array_data_ptr.filename),winId,MSG_VR_DELETE_BEGIN))
    {
        MMK_CloseWin(winId);

        MMIVIRTUALARRAY_Delete(WATCHRECORD_GetFileArray(), index);
        SCI_TRACE_LOW("MMIAPIFMM_DeleteFileAsyn return true!");
        MMI_GetLabelTextByLang(TXT_RECORD_FILE_DELETED, &strTitle);
#ifdef ADULT_WATCH_SUPPORT
        Adult_WatchCOM_NoteWin_1Line_Toast_Enter(
                                                        MMIRECORD_PLAYING_NOTE_WIN_ID,
                                                        &strTitle,
                                                        0,
                                                        HandleRecordPlayWinDeleteOkMsg//bug1345876 
                                                     );
#endif
    }
    else
    {
        MMK_CloseWin(winId);
        SCI_TRACE_LOW("MMIAPIFMM_DeleteFileAsyn return false!");
        MMI_GetLabelTextByLang(TXT_COMMON_DEL_FAIL, &strTitle);
#ifdef ADULT_WATCH_SUPPORT
        Adult_WatchCOM_NoteWin_1Line_Toast_Enter(
                                                        MMIRECORD_PLAYING_NOTE_WIN_ID,
                                                        &strTitle,
                                                        0,
                                                        PNULL
                                                     );
#endif
    }
}
LOCAL void WatchRec_PlayWin_Callback(void)
{
    MMK_CloseWin(MMIRECORD_LIST_WIN_ID);
}
LOCAL MMI_RESULT_E WatchRec_PlayWin_Query_Callback(MMI_WIN_ID_T winId, MMI_MESSAGE_ID_E msgId, DPARAM param)
{
       MMI_RESULT_E recode =  MMI_RESULT_TRUE;

    switch(msgId)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        //case MSG_NOTIFY_CANCEL:
            //WatchRec_PlayWin_StartDelete();
        //break;
	 //case MSG_NOTIFY_OK:
            //MMK_CloseWin(winId);
        //break;
#endif
#ifdef ADULT_WATCH_SUPPORT
       case MSG_CTL_OK:
       case MSG_APP_OK:
       case MSG_NOTIFY_MIDSK:
#else
       case MSG_CTL_CANCEL:
       case MSG_APP_CANCEL:
#endif
       {
            MMK_CloseWin(MMIRECORD_PLAY_WIN_ID);
            WatchRec_PlayWin_StartDelete(winId);
            break;
       }
#ifdef ADULT_WATCH_SUPPORT
       case MSG_CTL_CANCEL:
       case MSG_APP_CANCEL:
#else
       case MSG_CTL_OK:
       case MSG_APP_OK:
       case MSG_NOTIFY_MIDSK:
#endif
       {
            MMK_CloseWin(winId);
            break;
       }
       default:
           recode = MMI_RESULT_FALSE;
           break;
    }
    return recode;
	//MMK_CloseWin(MMIRECORD_LIST_WIN_ID);
}

LOCAL void WatchRec_PlayWin_OpenWindow(void)
{
    GUI_RECT_T rect = {0};
    wchar titleText[20] = {0};
    LOCAL wchar curText[] = {'0','0',':','0','0',':','0','0',0};
    MMI_STRING_T strCurTime = {0};
    MMI_STRING_T strTolTime = {0};
    MMI_STRING_T strTitle = {0};
    uint32 index = 0;
    MMISRVAUD_CONTENT_INFO_T info = {0};
    wchar ctime[9] = {0};
    wchar pathName[MMIFMM_PATHNAME_LEN + 1] = {0};
    FILEARRAY_DATA_T f_array_data_ptr = {0};
    wchar fileName[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16 fileNameLen = 0;
    wchar *tmp_name = PNULL;

    WatchRec_PlaywinDrawBG();

    rect.left = 0;
    rect.top = RECORD_PLAY_TITLE_TOP;
    rect.right = RECORD_PLAY_TITLE_RIGHT;
    rect.bottom = RECORD_PLAY_TITLE_BOTTOM;
    GUILABEL_SetRect(MMIRECORD_PLAY_TITLE_LABEL_CTRL_ID, &rect, FALSE);

    rect.left = RECORD_PLAY_CURTIME_LEFT;
    rect.top = RECORD_PLAY_CURTIME_TOP;
    rect.right = RECORD_PLAY_CURTIME_RIGHT;
    rect.bottom = RECORD_PLAY_CURTIME_BOTTOM;
    GUILABEL_SetRect(MMIRECORD_PLAY_CURTIME_LABEL_CTRL_ID, &rect, FALSE);

    if(s_enter_from_ex)
    {
        tmp_name = s_input_name;
    }
    else
    {
        index = WATCHRECORD_GetFileListCurIndex();
        WatchRec_LocalList_GetInfo(index,&f_array_data_ptr);
        tmp_name = f_array_data_ptr.filename;
    }
    MMIAPIFMM_SplitFullPath(
            tmp_name,
            MMIAPICOM_Wstrlen(tmp_name),
            PNULL,
            PNULL,
            PNULL,
            PNULL,
            fileName,
            &fileNameLen);

    strTitle.wstr_ptr = fileName;
    strTitle.wstr_len = fileNameLen;
    GUILABEL_SetText(MMIRECORD_PLAY_TITLE_LABEL_CTRL_ID,&strTitle,TRUE);
    GUILABEL_SetFont(MMIRECORD_PLAY_TITLE_LABEL_CTRL_ID, DP_FONT_22, 0xffff);

    strCurTime.wstr_ptr = curText;
    strCurTime.wstr_len = 8;
    GUILABEL_SetText(MMIRECORD_PLAY_CURTIME_LABEL_CTRL_ID,&strCurTime,TRUE);
    GUILABEL_SetFont(MMIRECORD_PLAY_CURTIME_LABEL_CTRL_ID, DP_FONT_48, 0xffff);

    MMISRVAUD_GetFileContentInfo(tmp_name,MMIAPICOM_Wstrlen(tmp_name),&info);
    WatchRec_IntTime2Wstr(info.total_time, ctime);
    s_play_tol_time = info.total_time;
    strTolTime.wstr_ptr = ctime;
    strTolTime.wstr_len = 8;

    GUIBUTTON_SetVisible(MMIRECORD_PAUSE_BUTTON_CTRL_ID,FALSE,FALSE);
    GUIBUTTON_SetVisible(MMIRECORD_STOP_BUTTON_CTRL_ID,FALSE,FALSE);
#ifdef ADULT_WATCH_SUPPORT
	AdultWatchCOM_Btn_SetPos_2Btn(MMIRECORD_DELETE_BUTTON_CTRL_ID, MMIRECORD_PLAY_BUTTON_CTRL_ID);
#else
    WatchCOM_Btn_SetPos_2Btn(MMIRECORD_PLAY_BUTTON_CTRL_ID, MMIRECORD_DELETE_BUTTON_CTRL_ID);
#endif
    if(WATCH_RECORD_PLAY_STATE_PLAYING == s_play_state)
    {
        SCI_TRACE_LOW("[RECORD] WatchRec_PlayWin_OpenWindow WATCH_RECORD_PLAY_STATE_PLAYING == s_play_state.s_play_timer_id = %d",s_play_timer_id);
        if(0 == MMIAPICOM_Wstrncmp(tmp_name,s_cur_play_name,s_cur_play_name_len))
        {
            SCI_TRACE_LOW("index == s_cur_play_index");
            WatchRec_ResumePlay();
        }
        else
        {
            SCI_TRACE_LOW("stop play");
            WatchRec_StopPlay();
        }

    }
    MMIAPICOM_Wstrncpy(s_cur_play_name,tmp_name,MMIAPICOM_Wstrlen(tmp_name));
    s_cur_play_name_len = MMIAPICOM_Wstrlen(tmp_name);
    MMK_PostMsg(MMIRECORD_PLAY_WIN_ID, MSG_FULL_PAINT, PNULL, PNULL);
    //s_play_state = WATCH_RECORD_PLAY_STATE_NORMAL;
}
LOCAL void WatchRec_PlayWin_APP_Web(void)
{
    MMIFMM_PATH_INFO_T currentPath = {0};
    uint32 index = 0;
    wchar pathName[MMIFMM_PATHNAME_LEN + 1] = {0};
    uint16 pathLen = 0;
    MMISRVAUD_CONTENT_INFO_T info = {0};
    FILEARRAY_DATA_T f_array_data_ptr = {0};
    BOOLEAN result = FALSE;

    if(WATCH_RECORD_PLAY_STATE_NORMAL == s_play_state)
    {
		s_play_tol_time = 18;
        if(0 == s_play_tol_time)
        {
            return;
        }
        index = WATCHRECORD_GetFileListCurIndex();
        WatchRec_LocalList_GetInfo(index,&f_array_data_ptr);
        pathLen = MMIAPICOM_Wstrlen(f_array_data_ptr.filename);

        result = WatchRec_PlayRecordFile();
        if(result)
        {
            s_play_state = WATCH_RECORD_PLAY_STATE_PLAYING;
            MMK_PostMsg(MMIRECORD_PLAY_WIN_ID, MSG_FULL_PAINT, PNULL, PNULL);
	        WatchRec_StartPlayTimer();
        }
    }
    else if(WATCH_RECORD_PLAY_STATE_PLAYING == s_play_state)
    {
        //WatchRec_StopPlay();
#ifdef ADULT_WATCH_SUPPORT
        WatchRec_PausePlay();
#else
        WatchRec_StopPlay();
#endif

    }
    else if(WATCH_RECORD_PLAY_STATE_PAUSE == s_play_state)
    {
        WatchRec_ResumePlay();
    }
}

LOCAL void WatchRec_PlayWin_APP_OK(void)
{
#if 0
    MMIFMM_PATH_INFO_T currentPath = {0};
    uint32 index = 0;
    wchar pathName[MMIFMM_PATHNAME_LEN + 1] = {0};
    uint16 pathLen = 0;
    MMISRVAUD_CONTENT_INFO_T info = {0};
    FILEARRAY_DATA_T f_array_data_ptr = {0};
    BOOLEAN result = FALSE;
    MMISRVAUD_RING_FMT_E         audio_type  = MMISRVAUD_RING_FMT_MAX;

    if(WATCH_RECORD_PLAY_STATE_NORMAL == s_play_state)
    {
		s_play_tol_time = 18;
        if(0 == s_play_tol_time)
        {
            return;
        }
        index = WATCHRECORD_GetFileListCurIndex();
        WatchRec_LocalList_GetInfo(index,&f_array_data_ptr);
        pathLen = MMIAPICOM_Wstrlen(f_array_data_ptr.filename);

        result = WatchRec_PlayRecordFile();
        if(result)
        {
            s_play_state = WATCH_RECORD_PLAY_STATE_PLAYING;
            MMK_PostMsg(MMIRECORD_PLAY_WIN_ID, MSG_FULL_PAINT, PNULL, PNULL);
	        WatchRec_StartPlayTimer();
        }
    }
    else if(WATCH_RECORD_PLAY_STATE_PLAYING == s_play_state)
    {
        //WatchRec_StopPlay();
        WatchRec_PausePlay();

    }
    else if(WATCH_RECORD_PLAY_STATE_PAUSE == s_play_state)
    {
        WatchRec_ResumePlay();
    }
#endif
	WatchRec_PlayWin_Delete();
}
LOCAL void WatchRec_PlayWin_FULL_PAINT(void)
{
    WatchRec_PlaywinDrawBG();
    if(WATCH_RECORD_PLAY_STATE_NORMAL == s_play_state)
    {
#ifdef ADULT_WATCH_SUPPORT
        AdultWatchCOM_Btn_SetPos_2Btn(MMIRECORD_DELETE_BUTTON_CTRL_ID, MMIRECORD_PLAY_BUTTON_CTRL_ID);
#else
        WatchCOM_Btn_SetPos_2Btn(MMIRECORD_PLAY_BUTTON_CTRL_ID, MMIRECORD_DELETE_BUTTON_CTRL_ID);
#endif
        GUIBUTTON_SetVisible(MMIRECORD_PAUSE_BUTTON_CTRL_ID,FALSE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_STOP_BUTTON_CTRL_ID,FALSE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_PLAY_BUTTON_CTRL_ID,TRUE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_DELETE_BUTTON_CTRL_ID,TRUE,TRUE);
    }
    else if(WATCH_RECORD_PLAY_STATE_PLAYING== s_play_state)
    {
#ifdef ADULT_WATCH_SUPPORT
        AdultWatchCOM_Btn_SetPos_2Btn(MMIRECORD_DELETE_BUTTON_CTRL_ID, MMIRECORD_PAUSE_BUTTON_CTRL_ID);
        GUIBUTTON_SetVisible(MMIRECORD_DELETE_BUTTON_CTRL_ID,TRUE,TRUE);
#else
        WatchCOM_Btn_SetPos_1Btn(MMIRECORD_PAUSE_BUTTON_CTRL_ID);
        GUIBUTTON_SetVisible(MMIRECORD_DELETE_BUTTON_CTRL_ID,FALSE,TRUE);
#endif
        GUIBUTTON_SetVisible(MMIRECORD_PAUSE_BUTTON_CTRL_ID,TRUE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_STOP_BUTTON_CTRL_ID,FALSE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_PLAY_BUTTON_CTRL_ID,FALSE,TRUE);
    }
	else if(WATCH_RECORD_PLAY_STATE_PAUSE== s_play_state)
    {
        GUIBUTTON_SetVisible(MMIRECORD_PAUSE_BUTTON_CTRL_ID,FALSE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_STOP_BUTTON_CTRL_ID,FALSE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_PLAY_BUTTON_CTRL_ID,TRUE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_DELETE_BUTTON_CTRL_ID,TRUE,TRUE);
#ifdef ADULT_WATCH_SUPPORT
        AdultWatchCOM_Btn_SetPos_2Btn(MMIRECORD_DELETE_BUTTON_CTRL_ID, MMIRECORD_PLAY_BUTTON_CTRL_ID);
#else
        WatchCOM_Btn_SetPos_2Btn(MMIRECORD_PLAY_BUTTON_CTRL_ID, MMIRECORD_DELETE_BUTTON_CTRL_ID);
#endif
    }
}


/**--------------------------------------------------------------------------*
**                          WINDOW  DEFINITION                               *
**--------------------------------------------------------------------------*/
WINDOW_TABLE(MMIRECORD_PLAY_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIRECORD_PLAY_TITLE_LABEL_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIRECORD_PLAY_CURTIME_LABEL_CTRL_ID),
    CREATE_BUTTON_CTRL(res_aw_recorder_ic_play, MMIRECORD_PLAY_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(res_aw_recorder_ic_delete, MMIRECORD_DELETE_BUTTON_CTRL_ID),
#ifdef ADULT_WATCH_SUPPORT
    CREATE_BUTTON_CTRL(res_aw_recorder_ic_pause, MMIRECORD_PAUSE_BUTTON_CTRL_ID),
#else
    CREATE_BUTTON_CTRL(res_recorder_ic_stop, MMIRECORD_PAUSE_BUTTON_CTRL_ID),
#endif
    CREATE_BUTTON_CTRL(res_aw_recorder_ic_done, MMIRECORD_STOP_BUTTON_CTRL_ID),
    WIN_FUNC((uint32)HandleRecordPlayWinMsg),
    WIN_ID(MMIRECORD_PLAY_WIN_ID),
#ifndef TOUCH_PANEL_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
#endif
    END_WIN
};

LOCAL void Handle_TOUCH_PANEL_SUPPORT( DPARAM param)
{
    if((MMIRECORD_PLAY_BUTTON_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id) || (MMIRECORD_PAUSE_BUTTON_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id) ||(MMIRECORD_RESUME_BUTTON_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
    {
        WatchRec_PlayWin_APP_Web();
    }
    else if(MMIRECORD_DELETE_BUTTON_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
    {
        WatchRec_PlayWin_Delete();
    }
}
LOCAL MMI_RESULT_E HandleRecordPlayWinMsg(
    MMI_WIN_ID_T      winId,
    MMI_MESSAGE_ID_E    msgId,
    DPARAM                param
)
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;
    FILEARRAY_DATA_T f_array_data_ptr = {0};
    uint16 pathLen = 0;
    switch(msgId)
    {
        case MSG_OPEN_WINDOW:
        {
	     WatchRec_PlayWin_OpenWindow();
	     break;
        }
        case MSG_FULL_PAINT:
        {
           if(MMK_IsFocusWin(winId))
           {
               WatchRec_PlayWin_FULL_PAINT();
           }
           break;
        }
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
            Handle_TOUCH_PANEL_SUPPORT(param);
         break;
#endif
       case MSG_CTL_OK:
       case MSG_APP_OK:
       {
           WatchRec_PlayWin_APP_OK();
           break;
       }	   	
       case MSG_APP_WEB:
       case MSG_CTL_MIDSK:
           WatchRec_PlayWin_APP_Web();
           break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
             if(WATCH_RECORD_PLAY_STATE_PLAYING == s_play_state)
             {
                 WatchRec_StopPlay();
             }
             MMK_CloseWin(winId);

            break;
        }
		case MSG_CLOSE_WINDOW:
        {
		    if((WATCH_RECORD_PLAY_STATE_PLAYING == s_play_state) || (WATCH_RECORD_PLAY_STATE_PAUSE == s_play_state))
            {
                WatchRec_StopPlay();
            }
            break;
		}
	    case MSG_APP_RED:
             // if(WATCH_RECORD_PLAY_STATE_PLAYING == s_play_state)
              //{
                  //MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_RECORD_PLAY);
              //}
              WatchRec_StopPlay();
              recode = MMI_RESULT_FALSE;
		break;
        case MSG_TIMER:
		if(s_play_timer_id == *((uint8*)param))
		{
		    WatchRec_PlayWin_TimerCallBack();
		}
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

LOCAL void WatchRec_StartPlayTimer(void)
{
    uint32  timeOut = RECORD_PLAY_TIMER_TIMEOUT;

    if(0 != s_play_timer_id)
    {
        MMK_StopTimer(s_play_timer_id);
        s_play_timer_id = 0;
    }
    s_play_timer_id = MMK_CreateWinTimer(MMIRECORD_PLAY_WIN_ID, timeOut, TRUE);
}
LOCAL void WatchRec_StopPlayTimer(void)
{
    if(0 != s_play_timer_id)
    {
        MMK_StopTimer(s_play_timer_id);
        s_play_timer_id = 0;
    }
}
LOCAL BOOLEAN WatchRec_PlayRecordFile(void)
{
#ifndef WIN32
    uint32 index = 0;
    FILEARRAY_DATA_T f_array_data_ptr = {0};
    wchar *tmp_name = PNULL;
    if(s_enter_from_ex)
    {
        tmp_name = s_input_name;
    }
    else
    {
        index = WATCHRECORD_GetFileListCurIndex();
        WatchRec_LocalList_GetInfo(index,&f_array_data_ptr);
        tmp_name = f_array_data_ptr.filename;
    }
    return Watch_PlayRecordFile(MMISRVAUD_TYPE_RING_FILE,TRUE,TRUE,tmp_name,MMIAPICOM_Wstrlen(tmp_name),1,0,MMIAPISET_GetMultimVolume(),PNULL);
#else
    return TRUE;
#endif
}
LOCAL void WatchRec_PlayWin_TimerCallBack(void)
{
    wchar text[] = {'0','0',':','0','0',':','0','0',0};
    wchar hour[] = {'0','1','2','3','4','5','6','7','8','9',0};
    wchar minH[] = {'0','1','2','3','4','5',0};
    wchar minL[] = {'0','1','2','3','4','5','6','7','8','9',0};
    wchar secH[] = {'0','1','2','3','4','5',0};
    wchar secL[] = {'0','1','2','3','4','5','6','7','8','9',0};
    MMI_STRING_T strDone = {0};
    MMIRECORD_TIME_T time = {0};
    MMI_STRING_T strBig = {0};
    uint32 curSecondTime = 0;
    float curtime = 0.0;
    float toltime = 0.0;

    s_play_cur_time++;
#ifdef WIN32
    if(s_play_cur_time > s_play_tol_time)
    {
        WatchRec_StopPlay();
        //return;
    }
#endif
    WatchRec_GetPlayTime(&time);
    text[0] = hour[time.hour_high];
    text[1] = hour[time.hour_low];
    text[3] = minH[time.minute_high];
    text[4] = minL[time.minute_low];
    text[6] = secH[time.second_high];
    text[7] = secL[time.second_low];
    strBig.wstr_ptr = text;
    strBig.wstr_len = 8;

    curSecondTime = WatchRec_GetCurSecondTime();
    curtime = curSecondTime;
    toltime = s_play_tol_time;

	SCI_TRACE_LOW("[RECORD] WatchRec_PlayWin_TimerCallBack curtime = %f,toltime = %f", curtime,toltime);
    //WatchRec_PlayWin_DrawProgressBar(curtime /toltime * 100);
    GUILABEL_SetText(MMIRECORD_PLAY_CURTIME_LABEL_CTRL_ID,&strBig,TRUE);


}
LOCAL uint32 WatchRec_GetPlayCurTime(void)
{
    MMISRVAUD_PLAY_INFO_T playing_info_ptr = {0};
    if(0 != s_record_play_handle)
    {
         MMISRVAUD_GetPlayingInfo(s_record_play_handle, &playing_info_ptr);
         SCI_TRACE_LOW("[RECORD] WatchRec_GetPlayCurTime cur_time = %d", playing_info_ptr.cur_time);
         return playing_info_ptr.cur_time;
    }
    return 0;

}
LOCAL void WatchRec_GetPlayTime(MMIRECORD_TIME_T* timPtr) //[out] calculate time from total_time
{

    uint32 time = 0;
    uint32  totalHours = 0;
    uint32  totalMinutes = 0;
    uint32  totalSeconds = 0;
#ifdef WIN32
    time = s_play_cur_time;
#else
   time = WatchRec_GetPlayCurTime();
#endif
    SCI_TRACE_LOW("[RECORD] WatchRec_GetPlayTime time = %d", time);
    totalHours = (time / 3600);
    totalMinutes = ((time % 3600) / 60);
    totalSeconds = ((time % 3600) % 60);
    if(PNULL == timPtr)
    {
        return;
    }
    timPtr->second_low = (uint8)(totalSeconds % 10);
    timPtr->second_high = (uint8)(totalSeconds / 10);
    timPtr->minute_low = (uint8)(totalMinutes % 10);
    timPtr->minute_high = (uint8)(totalMinutes / 10);
    timPtr->hour_low = (uint8)(totalHours % 10);
    timPtr->hour_high = (uint8)(totalHours / 10);
}
LOCAL void WatchRec_StopPlay(void)
{
    SCI_TRACE_LOW("[RECORD]WatchRec_StopPlay start");
#ifndef WIN32
    Watch_StopPlayFile();
//#else

#endif
    WatchRec_ResetPlayTimer();
    s_play_state = WATCH_RECORD_PLAY_STATE_NORMAL;
    //WatchRec_PlayWin_DrawProgressBar(0);
    if(MMK_IsFocusWin(MMIRECORD_PLAY_WIN_ID))
    {
        MMK_PostMsg(MMIRECORD_PLAY_WIN_ID, MSG_FULL_PAINT, PNULL, PNULL);
    }
    SCI_TRACE_LOW("[RECORD]WatchRec_StopPlay end");
}
LOCAL void WatchRec_PausePlay(void)
{
    SCI_TRACE_LOW("[RECORD]WatchRec_PausePlay start");
#ifndef WIN32
    //MMISRVMGR_Suspend(s_record_play_handle);
    if(!MMISRVAUD_Pause(s_record_play_handle))
        return;
//#else

#endif
    WatchRec_PausePlayTimer();
    s_play_state = WATCH_RECORD_PLAY_STATE_PAUSE;
    //WatchRec_PlayWin_DrawProgressBar(0);
    if(MMK_IsFocusWin(MMIRECORD_PLAY_WIN_ID))
    {
        MMK_PostMsg(MMIRECORD_PLAY_WIN_ID, MSG_FULL_PAINT, PNULL, PNULL);
    }
    SCI_TRACE_LOW("[RECORD]WatchRec_PausePlay end");
}
LOCAL void WatchRec_PausePlayTimer(void)
{
    WatchRec_StopPlayTimer();
}
LOCAL void WatchRec_ResumePlayTimer(void)
{
    WatchRec_StartPlayTimer();
}
LOCAL void WatchRec_ResetPlayTimer(void)
{
    wchar text[] = {'0','0',':','0','0',':','0','0',0};
    MMI_STRING_T strTime = {0};
    strTime.wstr_ptr = text;
    strTime.wstr_len = 8;
    GUILABEL_SetText(MMIRECORD_PLAY_CURTIME_LABEL_CTRL_ID,&strTime,TRUE);
    s_play_cur_time = 0;
    WatchRec_StopPlayTimer();
}
LOCAL uint16 GetPlayTime(MMISET_ALL_RING_TYPE_E type, wchar *name_ptr, uint32 name_len)
{   
    uint time = 0;
    MMISRVAUD_CONTENT_INFO_T content = {0};
    if(PNULL == name_ptr || 0 == name_len)
    {
        return 0;
    }
    switch(type)
    {
    case MMISET_RING_TYPE_MSG:
        /* if file total time is longer than the max value, return the max value, else return 0 to play once */
        MMISRVAUD_GetFileContentInfo(name_ptr, name_len, &content);
        if(content.total_time > 60000)
        {
            time = 60000;
        }
        break;
    default:
        break;
    }
    return time;
}


LOCAL BOOLEAN WatchRec_HandleAudioCallBack(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    SCI_TRACE_LOW("[RECORD]WatchRec_HandleAudioCallBack entry");
    if(param != PNULL && handle > 0)
    {
        switch(param->event)
        {
        case MMISRVMGR_NOTIFY_SUSPEND:
            s_play_state = WATCH_RECORD_PLAY_STATE_PAUSE;
            WatchRec_StopPlayTimer();
            if(MMK_IsFocusWin(MMIRECORD_PLAY_WIN_ID))//bug1629773
            {
                MMK_PostMsg(MMIRECORD_PLAY_WIN_ID, MSG_FULL_PAINT, PNULL, PNULL);
            }
            break;
        case MMISRVMGR_NOTIFY_RESUME:
            s_play_state = WATCH_RECORD_PLAY_STATE_PLAYING;
            WatchRec_StartPlayTimer();
            if(MMK_IsFocusWin(MMIRECORD_PLAY_WIN_ID))
            {
                MMK_PostMsg(MMIRECORD_PLAY_WIN_ID, MSG_FULL_PAINT, PNULL, PNULL);
            }
            break;
        case MMISRVMGR_NOTIFY_EXT:
            report_ptr = (MMISRVAUD_REPORT_T *)param->data;
            if(PNULL != report_ptr)
            {
                switch(report_ptr->report)
                {
                case MMISRVAUD_REPORT_END:
                    WatchRec_StopPlay();
                    break;

                default:
                    break;
                }
            }
            break;
        default:
            break;
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

LOCAL BOOLEAN Watch_PlayRecordFile(
                        MMISET_ALL_RING_TYPE_E         ring_type,
                        BOOLEAN                        is_a2dp,
                        BOOLEAN                        is_consume,
                        const wchar                    *full_path_ptr,
                        uint16                         full_path_len,
                        uint32                         play_times,
                        uint32                         play_offset,
                        uint32                         vol,
                        MMISET_RING_CALLBACK_PFUNC    call_back
                        )
{
    BOOLEAN             result = FALSE;
    MMISRVAUD_RING_FMT_E     music_type = 0;
    //AUDIO_FORMATINFO_T  audio_info = {0};
    uint16              file_name[MMIFILE_FILE_NAME_MAX_LEN+2] = {0};
    uint16              file_name_len = 0;

#ifdef DRM_SUPPORT
    BOOLEAN                 is_audio_drm_file = FALSE;
    BOOLEAN                 is_drm_valid = FALSE;
    MMISRVAUD_RING_FMT_E         drm_audio_type = MMISRVAUD_RING_FMT_MAX;
#endif


    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;

    if((PNULL == full_path_ptr) || (0 == full_path_len) || !MMIAPISET_IsPermitPlayRing(ring_type))
    {

        if(PNULL != call_back)
        {
            call_back(MMISRVAUD_REPORT_RESULT_NOT_SUPPORT, PNULL);
        }        
        return FALSE;
    }

    Watch_StopPlayFile();

    MMIAPIFMM_SplitFullPath(full_path_ptr, full_path_len, 
                          PNULL, PNULL, PNULL, PNULL,
                          file_name, &file_name_len);
    
    music_type = MMIAPICOM_GetMusicType( file_name, file_name_len );

#ifdef DRM_SUPPORT
    if(MMIAPIMP3_IsDrmFile(
            full_path_ptr, full_path_len,
            &drm_audio_type,&is_audio_drm_file,&is_drm_valid
            ))
    {
        if(is_audio_drm_file && is_drm_valid)
        {
            music_type = drm_audio_type;
        }
    }
#endif

    req.is_auto_free = FALSE;
    req.notify = WatchRec_HandleAudioCallBack;
    req.pri = MMISRVAUD_PRI_NORMAL;
    audio_srv.duation = GetPlayTime(ring_type, full_path_ptr, full_path_len);
    audio_srv.play_times = play_times;
    audio_srv.volume = vol;
    audio_srv.info.ring_file.type = MMISRVAUD_TYPE_RING_FILE;
    audio_srv.info.ring_file.fmt = music_type;
    audio_srv.info.ring_file.name = full_path_ptr;
    audio_srv.info.ring_file.name_len = full_path_len;
    if(!GPIO_CheckHeadsetStatus())
    {
        audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARFREE | MMISRVAUD_ROUTE_BLUETOOTH;//bug1575387 
    }
    else
    {
        audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE;
    }


    handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    if (handle > 0)
    {
        s_record_play_handle = handle;

        if(MMISRVAUD_Play(s_record_play_handle, 0))
        {

            s_record_play_handle = handle;
            #ifdef DRM_SUPPORT
            if(is_audio_drm_file && is_consume)
            {
                MMIAPIDRM_ConsumeRights(SFS_INVALID_HANDLE, full_path_ptr);
            }
            #endif
            
            result = TRUE;
        }
        else
        {
            MMISRVMGR_Free(s_record_play_handle);
            s_record_play_handle = 0;
            if(PNULL != call_back)
            {
                call_back(MMISRVAUD_REPORT_RESULT_ERROR, PNULL);
            }
        }
    }
    else
    {
        if(PNULL != call_back)
        {
            call_back(MMISRVAUD_REPORT_RESULT_NOT_SUPPORT, PNULL);
        }
    }
    

    return result;
}
LOCAL void Watch_StopPlayFile(void)
{
    if(s_record_play_handle != 0)
    {
        MMISRVMGR_Free(s_record_play_handle);
        s_record_play_handle = 0;
    }
    
}
LOCAL void WatchRec_PlaywinDrawBG(void)
{
    GUI_LCD_DEV_INFO  lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T rect = {0};

    rect.left = 0;
    rect.top = 0;
    rect.right = RECORD_BACKGROUND_RIGHT;
    rect.bottom = RECORD_BACKGROUND_BOTTOM;
    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
}
/*LOCAL void WatchRec_PlayWin_DrawProgressBar(uint16 percentage)
{
    GUI_LCD_DEV_INFO  lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint16 x1 = 0;
    uint16 y1 = 0;
    uint16 x2 = 0;
    uint16 y2 = 0;

    x2 = RECORD_BACKGROUND_RIGHT * percentage / 100;
	SCI_TRACE_LOW("[RECORD] WatchRec_PlayWin_DrawProgressBar percentage = %d,x2 = %d", percentage,x2);
    LCD_DrawThickLine(&lcd_dev_info,x1,y1,x2,y2,MMI_RED_COLOR,PROGRESS_BAR_THICK);
}*/
LOCAL uint32 WatchRec_GetCurSecondTime(void)
{
#ifdef WIN32
    return s_play_cur_time;
#else
   return WatchRec_GetPlayCurTime();
#endif
}
LOCAL MMI_RESULT_E HandleRecordPlayWinDeleteOkMsg(
    MMI_WIN_ID_T      winId,
    MMI_MESSAGE_ID_E    msgId,
    DPARAM                param
)
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;
    switch(msgId)
    {
        case MSG_CLOSE_WINDOW:
            MMK_CloseWin(MMIRECORD_PLAY_WIN_ID);
            break;
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}
PUBLIC void WATCHRECORD_OpenPlayWin(void)
{
    s_enter_from_ex = FALSE;
    MMK_CreateWin((uint32 *)MMIRECORD_PLAY_WIN_TAB, PNULL);
}
PUBLIC void WATCHRECORD_OpenPlayWinEx(wchar *name)
{
    s_enter_from_ex = TRUE;
#if defined(SCREEN_SHAPE_CIRCULAR) && !defined(ADULT_WATCH_SUPPORT)
    SCI_MEMSET(s_input_name,0x0,sizeof(wchar)*RECORD_PLAY_NAME_MAX_LEN);
#else
    SCI_MEMSET(s_input_name,0x0,sizeof(wchar)*MMIFMM_FILE_FILE_NAME_MAX_LEN);
#endif
    MMIAPICOM_Wstrncpy(s_input_name, name, MMIAPICOM_Wstrlen(name));
    MMK_CreateWin((uint32 *)MMIRECORD_PLAY_WIN_TAB, PNULL);
}
PUBLIC void WATCHRECORD_StopPlay(void)
{
    SCI_TRACE_LOW("[RECORD]WATCHRECORD_StopPlay start");
    if(WATCH_RECORD_PLAY_STATE_PLAYING == s_play_state)
    {
        WatchRec_StopPlay();
        MMK_CloseWin(MMIRECORD_PLAY_WIN_ID);
        MMK_CloseWin(MMIRECORD_LIST_WIN_ID);
        MMK_CloseWin(MMIRECORD_MAINPLAY_WIN_ID);
    }
}
PUBLIC WATCH_RECORD_PLAY_STATE_E WATCHRECORD_GetPlayStatus(void)
{
    SCI_TRACE_LOW("[RECORD]WATCHRECORD_GetPlayStatus state:%d",s_play_state);
    return s_play_state;
}
PUBLIC BOOLEAN WATCHRECORD_GetCurPlayName(wchar *name)
{
    if(WATCH_RECORD_PLAY_STATE_PLAYING != s_play_state)
    {
        SCI_TRACE_LOW("[RECORD]WATCHRECORD_GetCurPlayName WATCH_RECORD_PLAY_STATE_PLAYING != s_play_state");
        return FALSE;
    }
    if(PNULL == name)
    {
        SCI_TRACE_LOW("[RECORD]WATCHRECORD_GetCurPlayName PNULL == name");
        return FALSE;
    }
    MMIAPICOM_Wstrncpy(name, s_cur_play_name, s_cur_play_name_len);
    SCI_TRACE_LOW("[RECORD]WATCHRECORD_GetCurPlayName name:%s",name);
    return TRUE;
}
LOCAL void WatchRec_ResumePlay(void)
{
       SCI_TRACE_LOW("[RECORD]WatchRec_ResumePlay start");
#ifndef WIN32
    //MMISRVMGR_Resume(s_record_play_handle);
    if(!MMISRVAUD_Resume(s_record_play_handle))
		return;
//#else

#endif
    WatchRec_StartPlayTimer();
    s_play_state = WATCH_RECORD_PLAY_STATE_PLAYING;
    //WatchRec_PlayWin_DrawProgressBar(0);
    if(MMK_IsFocusWin(MMIRECORD_PLAY_WIN_ID))
    {
        MMK_PostMsg(MMIRECORD_PLAY_WIN_ID, MSG_FULL_PAINT, PNULL, PNULL);
    }
    SCI_TRACE_LOW("[RECORD]WatchRec_ResumePlay end");
}
