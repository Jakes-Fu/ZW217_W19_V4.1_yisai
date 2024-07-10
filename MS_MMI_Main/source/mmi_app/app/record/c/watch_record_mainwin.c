/*****************************************************************************
** File Name:      mmirecord_mainwin.c                                 *
** Author:         jiaxin.sun                                                    *
** Date:           2020.3.4                                                 *
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
#include "watch_commonwin_export.h"
#include "mmidisplay_data.h"

//#include "mmirecord_internal.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                                                             *
**--------------------------------------------------------------------------*/
#define RECORD_RECORD_TIME_TOP DP2PX_VALUE(72)
#define RECORD_RECORD_TIME_RIGHT DP2PX_VALUE(240)
#define RECORD_RECORD_TIME_BOTTOM DP2PX_VALUE(112)
#ifdef WIN32
#define RECORD_TIMER_TIMEOUT 1000
#else
#define RECORD_TIMER_TIMEOUT 300
#endif
#define RECORD_JUMP_TIMER_TIMEOUT 500
#define MMIRECORD_MIN_NEED_SPACE        20000
/**--------------------------------------------------------------------------*
**                         LOCAL DEFINITION                                                                               *
**--------------------------------------------------------------------------*/
LOCAL MMIVOICE_RECORD_STATE_E s_recorder_state = MMIVOICE_RECORD_STATE_NORMAL;
LOCAL uint8  s_timer_id = 0;  //record timer
LOCAL uint32 s_record_cur_timer_elapsed = 0;
LOCAL uint8 s_jump_timer_id = 0;
LOCAL BOOLEAN s_time_display = TRUE;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                                                                 */
/*---------------------------------------------------------------------------*/
LOCAL void WatchRec_StartTimer(void);
LOCAL void WatchRec_StopTimer(void);
LOCAL void WatchRec_GetRecordTime(MMIRECORD_TIME_T* timePtr);
LOCAL void WatchRec_MainWin_TimerCallBack(void);
LOCAL void WatchRec_MainWin_FullPaint(MMI_WIN_ID_T winId, MMI_MESSAGE_ID_E msgId, DPARAM param);
LOCAL void WatchRec_PauseRecord(MMI_HANDLE_T winHandle);
LOCAL void WatchRec_ResumeRecord(MMI_HANDLE_T winHandle);
LOCAL void WatchRec_MainWin_OpenWindow(
    MMI_WIN_ID_T      winId,
    MMI_MESSAGE_ID_E    msgId,
    DPARAM                param);
LOCAL MMI_RESULT_E HandleRecordMainWinTpKeyDown(MMI_HANDLE_T win_handle, DPARAM     param);
LOCAL void WatchRec_MainwinDrawBG(void);
/*****************************************************************************/
//  Description : set recordfile pah and name
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void UpdateRecordFilePath(RECORD_WIN_DATA_T * record_win_data_ptr);
LOCAL void WatchRec_StartJumpTimer(void);
LOCAL void WatchRec_StopJumpTimer(void);
LOCAL void WatchRec_MainWin_JumpTimerCallBack(void);
LOCAL void WatchRec_StopJumpText(void);
LOCAL BOOLEAN WatchRec_MainWin_IsSpaceEnough(void);
#if 0
/*****************************************************************************/
//  Description :是否插入录音询问窗口
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRecordWorkQueryWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
);//recordwork
#endif

/*****************************************************************************/
//  Description : Recorder Main Play win
//  Global resource dependence : none
//  Author: haiyanghu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRecordMainPlayWinMsg(
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM                param
);

LOCAL void WatchRec_StartRecord(BOOLEAN is_reRecord, MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : ResumeRecord
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void ResumeRecord(MMI_HANDLE_T win_handle);

LOCAL void WatchRec_StopRecord(MMI_HANDLE_T winHandle);

/*****************************************************************************/
//  Description : HandleTimerMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
//LOCAL MMI_RESULT_E HandleTimerMsg(MMI_HANDLE_T win_handle,
//                                  DPARAM     param);

/*****************************************************************************/
//  Description : HandleOKKeyMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOKKeyMsg(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description :CreateMainWinUserData
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateMainWinUserData(MMIRECORD_WORK_PARAM_T *work_param_ptr,
                                    MMI_HANDLE_T      win_handle);

/*****************************************************************************/
//  Description :CreateMainWinUserData
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void FreeMainWinUserData(MMI_HANDLE_T      win_handle);



/**--------------------------------------------------------------------------*
**                          WINDOW  DEFINITION                               *
**--------------------------------------------------------------------------*/
WINDOW_TABLE(MMIRECORD_MAINPLAY_WIN_TAB) =
{
    WIN_HIDE_STATUS,
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIRECORD_TIME_FORM_CTRL_ID),
	CREATE_BUTTON_CTRL(res_camera_ic_setting, MMIRECORD_OPT_BUTTON_CTRL_ID),
	CREATE_BUTTON_CTRL(res_recorder_ic_start, MMIRECORD_RECORD_BUTTON_CTRL_ID),
	CREATE_BUTTON_CTRL(res_recorder_ic_menu_list, MMIRECORD_LIST_BUTTON_CTRL_ID),
	CREATE_BUTTON_CTRL(res_recorder_ic_done, MMIRECORD_STOP_BUTTON_CTRL_ID),
	CREATE_BUTTON_CTRL(res_recorder_ic_pause, MMIRECORD_PAUSE_BUTTON_CTRL_ID),
	CREATE_BUTTON_CTRL(res_recorder_ic_play, MMIRECORD_RESUME_BUTTON_CTRL_ID),
	WIN_FUNC((uint32)HandleRecordMainPlayWinMsg),
	WIN_ID(MMIRECORD_MAINPLAY_WIN_ID),
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_NULL),
    END_WIN
};
LOCAL MMI_RESULT_E HandleRecordMainWinTpKeyDown(MMI_HANDLE_T win_handle, DPARAM     param)
{
    return 0;
}
LOCAL void WatchRec_MainWin_OpenWindow(
    MMI_WIN_ID_T      winId,
    MMI_MESSAGE_ID_E    msgId,
    DPARAM                param)
{
    GUI_RECT_T rect = {0};
    GUI_RECT_T rectTime = {0};
    LOCAL wchar bigText[] = {'0','0',':','0','0',':','0','0',0};
    MMI_STRING_T strBig = {0};
    RECORD_WIN_DATA_T       *recordWinDataPtr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(winId);

    WatchRec_MainwinDrawBG();

    s_recorder_state = MMIVOICE_RECORD_STATE_NORMAL;

     rectTime.left = 0;
     rectTime.top = RECORD_RECORD_TIME_TOP;
     rectTime.right = RECORD_RECORD_TIME_RIGHT;
     rectTime.bottom = RECORD_RECORD_TIME_BOTTOM;
     GUILABEL_SetRect(MMIRECORD_TIME_FORM_CTRL_ID, &rectTime, FALSE);
     strBig.wstr_ptr = bigText;
     strBig.wstr_len = 8;
     GUILABEL_SetText(MMIRECORD_TIME_FORM_CTRL_ID,&strBig,TRUE);
     GUILABEL_SetFont(MMIRECORD_TIME_FORM_CTRL_ID, DP2PX_FONT(SONG_FONT_48), 0xffff);
     if(!MMIRECORD_Constructor())
     {
         MMK_CloseWin(winId);
         return;
     }

     UpdateRecordFilePath(recordWinDataPtr);
}
LOCAL void WatchRec_MainWin_FullPaint(MMI_WIN_ID_T winId, MMI_MESSAGE_ID_E msgId, DPARAM param)
{
    MMI_STRING_T textLeft = {0};
    MMI_STRING_T textMid = {0};
    MMI_STRING_T textRight = {0};

    WatchRec_MainwinDrawBG();
    if(MMIVOICE_RECORD_STATE_NORMAL == s_recorder_state)
    {
        WatchCOM_Btn_SetPos_3Btn(MMIRECORD_OPT_BUTTON_CTRL_ID,MMIRECORD_RECORD_BUTTON_CTRL_ID,MMIRECORD_LIST_BUTTON_CTRL_ID);
        GUIBUTTON_SetVisible(MMIRECORD_STOP_BUTTON_CTRL_ID,FALSE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_PAUSE_BUTTON_CTRL_ID,FALSE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_LIST_BUTTON_CTRL_ID,TRUE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_RECORD_BUTTON_CTRL_ID,TRUE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_OPT_BUTTON_CTRL_ID,TRUE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_RESUME_BUTTON_CTRL_ID,FALSE,TRUE);

        GUIWIN_SetSoftkeyTextId(winId,STXT_OPTION,TXT_WATCH_RECORD_RECORD,TXT_COMMON_LIST,TRUE);
    }
    else if(MMIVOICE_RECORD_STATE_RECORDING == s_recorder_state)
    {
        WatchCOM_Btn_SetPos_2Btn(MMIRECORD_STOP_BUTTON_CTRL_ID,MMIRECORD_PAUSE_BUTTON_CTRL_ID);
        GUIBUTTON_SetVisible(MMIRECORD_STOP_BUTTON_CTRL_ID,TRUE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_PAUSE_BUTTON_CTRL_ID,TRUE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_LIST_BUTTON_CTRL_ID,FALSE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_RECORD_BUTTON_CTRL_ID,FALSE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_OPT_BUTTON_CTRL_ID,FALSE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_RESUME_BUTTON_CTRL_ID,FALSE,TRUE);

        GUIWIN_SetSoftkeyTextId(winId,STXT_STOP,TXT_PAUSE,0,TRUE);
    }
    else if(MMIVOICE_RECORD_STATE_PAUSE == s_recorder_state)
    {
        WatchCOM_Btn_SetPos_2Btn(MMIRECORD_STOP_BUTTON_CTRL_ID,MMIRECORD_RESUME_BUTTON_CTRL_ID);
        GUIBUTTON_SetVisible(MMIRECORD_STOP_BUTTON_CTRL_ID,TRUE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_PAUSE_BUTTON_CTRL_ID,FALSE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_LIST_BUTTON_CTRL_ID,FALSE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_RECORD_BUTTON_CTRL_ID,FALSE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_OPT_BUTTON_CTRL_ID,FALSE,TRUE);
        GUIBUTTON_SetVisible(MMIRECORD_RESUME_BUTTON_CTRL_ID,TRUE,TRUE);

		GUIWIN_SetSoftkeyTextId(winId,STXT_STOP,TXT_WATCH_RECORD_RESUME,0,TRUE);
    }
}

LOCAL void WatchRec_GetRecordTime(MMIRECORD_TIME_T* timPtr) //[out] calculate time from total_time
{

    uint32 time = 0;
	uint32  totalHours = 0;
	uint32  totalMinutes = 0;
	uint32  totalSeconds = 0;
#ifdef WIN32
    time = s_record_cur_timer_elapsed;
#else
    time = MMIRECORD_GetRecordSecondTime();
#endif
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
LOCAL void WatchRec_MainWin_TimerCallBack(void)
{
    wchar text[] = {'0','0',':','0','0',':','0','0',0};
    wchar hour[] = {'0','1','2','3','4','5','6','7','8','9',0};
    wchar minH[] = {'0','1','2','3','4','5',0};
    wchar minL[] = {'0','1','2','3','4','5','6','7','8','9',0};
    wchar secH[] = {'0','1','2','3','4','5',0};
    wchar secL[] = {'0','1','2','3','4','5','6','7','8','9',0};
    GUI_BG_T  bg={0};
    MMI_STRING_T strDone = {0};
    MMIRECORD_TIME_T time = {0};
    MMI_STRING_T strBig = {0};
    MMI_STRING_T strTitle = {0};//TXT_NO_SPACE
    WATCH_SOFTKEY_TEXT_ID_T softkey_text = {NULL ,NULL ,NULL};
    if(!WatchRec_MainWin_IsSpaceEnough())
    {

        WatchRec_StopRecord(MMIRECORD_MAINPLAY_WIN_ID);
        return;
    }
    s_record_cur_timer_elapsed++;
    WatchRec_GetRecordTime(&time);
    text[0] = hour[time.hour_high];
    text[1] = hour[time.hour_low];
    text[3] = minH[time.minute_high];
    text[4] = minL[time.minute_low];
    text[6] = secH[time.second_high];
    text[7] = secL[time.second_low];
    strBig.wstr_ptr = text;
    strBig.wstr_len = 8;


    GUILABEL_SetText(MMIRECORD_TIME_FORM_CTRL_ID,&strBig,TRUE);


}
LOCAL void WatchRec_MainWin_JumpTimerCallBack(void)
{
    if(MMIVOICE_RECORD_STATE_PAUSE == s_recorder_state)
    {
        if(s_time_display)
        {
            GUILABEL_SetVisible(MMIRECORD_TIME_FORM_CTRL_ID,TRUE,TRUE);
        }
        else
        {
            GUILABEL_SetVisible(MMIRECORD_TIME_FORM_CTRL_ID,FALSE,TRUE);
        }
        MMK_PostMsg(MMIRECORD_MAINPLAY_WIN_ID, MSG_FULL_PAINT, PNULL, PNULL);
        s_time_display = !s_time_display;
    }
}
LOCAL void WatchRec_ResetTimer(void)
{
    wchar text[] = {'0','0',':','0','0',':','0','0',0};
    MMI_STRING_T strTime = {0};
    strTime.wstr_ptr = text;
    strTime.wstr_len = 8;
    GUILABEL_SetText(MMIRECORD_TIME_FORM_CTRL_ID,&strTime,TRUE);
    s_record_cur_timer_elapsed = 0;
    WatchRec_StopTimer();
    //MMK_StopTimer(s_timer_id);
}
/*****************************************************************************/
//  Description : HandleWebKeyMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWebKeyMsg(MMI_HANDLE_T win_handle)
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);
    MMI_STRING_T strTitle = {0};//TXT_NO_SPACE
    WATCH_SOFTKEY_TEXT_ID_T softkey_text = {NULL ,NULL ,NULL};
    if(PNULL == record_win_data_ptr)
    {
        return result;
    }

    SCI_TRACE_LOW("[RECORD] HandleWebKeyMsg, entry!");

    if(MMIVOICE_RECORD_STATE_NORMAL == s_recorder_state)
    {
        if(!WatchRec_MainWin_IsSpaceEnough())
        {
             MMI_GetLabelTextByLang(TXT_NO_SPACE, &strTitle);
#ifdef ADULT_WATCH_SUPPORT
             WatchCOM_NoteWin_2Line_Enter(
                                                        MMIRECORD_WORK_QUERY_WIN_ID,
                                                        &strTitle,
                                                        0,
                                                        softkey_text,
                                                        PNULL
                                                     );
#else
            WatchPUB_Alert_Note_Win(MMIRECORD_WORK_QUERY_WIN_ID,TXT_NO_SPACE);
#endif
            return result;
        }
        WatchRec_StartRecord(FALSE, win_handle);
        //MMK_PostMsg(win_handle, MSG_START_RECORD, PNULL, PNULL);
        
    }
    else if(MMIVOICE_RECORD_STATE_RECORDING == s_recorder_state)
    {
        WatchRec_PauseRecord(win_handle);
        //MMK_PostMsg(win_handle, MSG_PAUSE_RECORD, PNULL, PNULL);

    }
    else if(MMIVOICE_RECORD_STATE_PAUSE == s_recorder_state)
    {
        WatchRec_ResumeRecord(win_handle);
        //MMK_PostMsg(win_handle, MSG_RESUME_RECORD, PNULL, PNULL);

    }


    return result;
}

/*****************************************************************************/
//  Description : HandleOKKeyMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOKKeyMsg(MMI_HANDLE_T win_handle)
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return result;
    }

    if(MMIVOICE_RECORD_STATE_NORMAL == s_recorder_state)
    {
        WATCHRECORD_OpenOptList();
    }
    else if((MMIVOICE_RECORD_STATE_RECORDING == s_recorder_state) || (MMIVOICE_RECORD_STATE_PAUSE == s_recorder_state))
    {
        WatchRec_StopRecord(win_handle);
    }

    return result;
}
#if 0
/*****************************************************************************/
//  Description : HandleTimerMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTimerMsg(MMI_HANDLE_T win_handle,
                                  DPARAM     param)
{
    MMI_RESULT_E            recode  = MMI_RESULT_FALSE;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL != record_win_data_ptr
            && record_win_data_ptr->timer_id == *((uint8 *) param))
    {
        StopDisplayProgressTimer(win_handle);

        if(MMK_IsFocusWin(win_handle))
        {
            if(MMIRECORD_IsReachMax(record_win_data_ptr->max_file_time, record_win_data_ptr->max_file_size))
            {
                WatchRec_StopRecord(win_handle);
            }
            else
            {
                MMIRECORD_UpdateScreen(win_handle, FALSE);
                StartDisplayProgressTimer(win_handle);
            }
        }

        recode  = MMI_RESULT_TRUE;
    }

    return recode;
}
#endif
/*****************************************************************************/
//  Description : set recordfile pah and name
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void UpdateRecordFilePath(RECORD_WIN_DATA_T * record_win_data_ptr)
{
    wchar *record_file_path_ptr = MMIRECORD_GetRecordFilePath();
    wchar *record_file_name_ptr = MMIRECORD_GetRecordFileName();
    uint16  str_len = 0;

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    SCI_MEMSET(record_win_data_ptr->record_file_full_path, 0, MMIRECORD_SRV_MAX_FILE_NAME_LEN);
    record_win_data_ptr->record_file_full_path_len = 0;

    SCI_MEMSET(record_win_data_ptr->record_file_name, 0, MMIRECORD_SRV_MAX_FILE_NAME_LEN);
    record_win_data_ptr->record_file_name_len = 0;

    str_len = MMIAPICOM_Wstrlen(record_file_path_ptr);

    if(PNULL != record_file_path_ptr)
    {
        record_win_data_ptr->record_file_full_path_len = MIN(MMIRECORD_SRV_MAX_FILE_NAME_LEN, str_len);
        MMIAPICOM_Wstrncpy(record_win_data_ptr->record_file_full_path, record_file_path_ptr, record_win_data_ptr->record_file_full_path_len);

    }

    if(PNULL != record_file_name_ptr)
    {
        record_win_data_ptr->record_file_name_len = MIN(MMIRECORD_SRV_MAX_FILE_NAME_LEN, str_len);
        MMIAPICOM_Wstrncpy(record_win_data_ptr->record_file_name, record_file_name_ptr, record_win_data_ptr->record_file_name_len);
    }

}
/*****************************************************************************/
//  Description : Recorder Main Play win
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRecordMainPlayWinMsg(
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM                param
)
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_id);
    LOCAL uint8 delay_time_id = 0;
    LOCAL BOOLEAN is_dalayed = FALSE;

    if(PNULL == record_win_data_ptr)
    {
        return MMI_RESULT_TRUE;
    }

    if(MSG_TIMER != msg_id)
    {
        SCI_TRACE_LOW("[RECORD] HandleRecordMainPlayWinMsg msg_id = 0x%x", msg_id);
    }

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        WatchRec_MainWin_OpenWindow(win_id,msg_id,param);
        break;
    case MSG_FULL_PAINT:
        WatchRec_MainWin_FullPaint(win_id,msg_id,param);
    break;
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        HandleOKKeyMsg(record_win_data_ptr->win_handle);
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        HandleWebKeyMsg(record_win_data_ptr->win_handle);
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        if(MMIVOICE_RECORD_STATE_NORMAL == s_recorder_state)
        {
            WATCHRECORD_OpenWaitingWin();
            //MMIRECORD_OpenRecordFileList();
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            if(MMIRECORD_OPT_BUTTON_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//
            {
                WATCHRECORD_OpenOptList();
            }
            else if(MMIRECORD_RECORD_BUTTON_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//
            {
                WatchRec_StartRecord(FALSE, record_win_data_ptr->win_handle);
            }
            else if(MMIRECORD_LIST_BUTTON_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//
            {
                WATCHRECORD_OpenWaitingWin();
            }
            else if(MMIRECORD_STOP_BUTTON_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//
            {
                WatchRec_StopRecord(record_win_data_ptr->win_handle);
            }
            else if(MMIRECORD_PAUSE_BUTTON_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//
            {
                WatchRec_PauseRecord(record_win_data_ptr->win_handle);
            }
            else if(MMIRECORD_RESUME_BUTTON_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)//
            {
                WatchRec_ResumeRecord(record_win_data_ptr->win_handle);
            }
        }
        break;

#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_STOP_RECORD:
        {
    	      uint32 cur_time = MMIRECORD_GetRecordSecondTime();
             if(!is_dalayed && cur_time <= 1)
             {
                 is_dalayed = TRUE;
                 delay_time_id = MMK_CreateWinTimer(record_win_data_ptr->win_handle, 300, FALSE);
             }
             else
             {
                 is_dalayed = FALSE;
		   WatchRec_StopRecord(record_win_data_ptr->win_handle);
             }
        }
        break;



    case MSG_TIMER:
		if(s_timer_id == *((uint8*)param))
		{
		    WatchRec_MainWin_TimerCallBack();
		}
        else if(s_jump_timer_id == *((uint8*)param))
		{
		    WatchRec_MainWin_JumpTimerCallBack();
		}
		#if 0
		if(PNULL != param)
		{
		  if(delay_time_id == *((uint8*)param))
		  {
		    MMK_StopTimer(delay_time_id);
		    delay_time_id = 0;
		    MMK_PostMsg((record_win_data_ptr->win_handle), MSG_STOP_RECORD, PNULL, PNULL);
		  }
        recode = HandleTimerMsg(record_win_data_ptr->win_handle, param);
		}
		#endif
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

   // recode = MMIRECORD_HandleRecordMainPlayWinMsg(win_id,msg_id,param);

    //bug-72892;
    if(MSG_CLOSE_WINDOW == msg_id)
    {
        is_dalayed = FALSE;
        if(delay_time_id)
        {
            MMK_StopTimer(delay_time_id);
            delay_time_id = 0;
        }
        record_win_data_ptr->is_insert_record = FALSE;
        if(MMIVOICE_RECORD_STATE_RECORDING == s_recorder_state || MMIVOICE_RECORD_STATE_PAUSE == s_recorder_state)
        {
            WatchRec_StopRecord(record_win_data_ptr->win_handle);
        }  
        FreeMainWinUserData(record_win_data_ptr->win_handle);
        MMIRECORD_Destructor();
        MMK_CloseApplet(SPRD_RECORD_APPLET_ID);
    }

    return recode;
}

LOCAL void WatchRec_PauseRecord(MMI_HANDLE_T winHandle)
{
    wchar text[] = {'0','0',':','0','0',':','0','0',0};
    wchar hour[] = {'0','1','2','3','4','5','6','7','8','9',0};
    wchar minH[] = {'0','1','2','3','4','5',0};
    wchar minL[] = {'0','1','2','3','4','5','6','7','8','9',0};
    wchar secH[] = {'0','1','2','3','4','5',0};
    wchar secL[] = {'0','1','2','3','4','5','6','7','8','9',0};
    MMIRECORD_TIME_T time = {0};
    MMI_STRING_T strBig = {0};

    if(MMIRECORD_PauseRecord())
    {
        s_recorder_state = MMIVOICE_RECORD_STATE_PAUSE;
        WatchRec_StopTimer();
//Bug 1812489按下暂停键时先从audio获取当前录音时间并显示到屏幕
        WatchRec_GetRecordTime(&time);
        text[0] = hour[time.hour_high];
        text[1] = hour[time.hour_low];
        text[3] = minH[time.minute_high];
        text[4] = minL[time.minute_low];
        text[6] = secH[time.second_high];
        text[7] = secL[time.second_low];
        strBig.wstr_ptr = text;
        strBig.wstr_len = 8;
        GUILABEL_SetText(MMIRECORD_TIME_FORM_CTRL_ID,&strBig,TRUE);
///////
        WatchRec_StartJumpTimer();
        //WatchCOM_Btn_SetPos_2Btn(MMIRECORD_STOP_BUTTON_CTRL_ID,MMIRECORD_RESUME_BUTTON_CTRL_ID);
        MMK_PostMsg(winHandle, MSG_FULL_PAINT, PNULL, PNULL);
    }
}
/*****************************************************************************/
//  Description : ResumeRecord
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void ResumeRecord(MMI_HANDLE_T win_handle)
{
    if(MMIRECORD_ResumeRecord())
    {
        MMIRECORD_UpdateButtonStatusRecording(win_handle);
        MMIRECORD_UpdateScreen(win_handle, FALSE);
    }
}
LOCAL void WatchRec_ResumeRecord(MMI_HANDLE_T winHandle)
{
    if(MMIRECORD_ResumeRecord())
    {
        s_recorder_state = MMIVOICE_RECORD_STATE_RECORDING;
        WatchRec_StartTimer();
        WatchRec_StopJumpText();
        MMK_PostMsg(winHandle, MSG_FULL_PAINT, PNULL, PNULL);
    }
}

LOCAL void WatchRec_StopRecord(MMI_HANDLE_T winHandle)
{
    MMI_WIN_ID_T    queryWinId = MMIRECORD_WORK_QUERY_WIN_ID;
    RECORD_WIN_DATA_T       *recordWinDataPtr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(winHandle);
    //wchar text[] = L"Saved!";
    MMI_STRING_T strTitle = {0};
    //MMI_STRING_T text_str = {0};
    WATCH_SOFTKEY_TEXT_ID_T softkey_text = {NULL ,NULL ,NULL};

	SCI_TRACE_LOW("[RECORD]WatchRec_StopRecord start");

    if(MMIRECORD_RESULT_SUCCESS == MMIRECORD_StopRecord())
    {
        SCI_TRACE_LOW("[RECORD]WatchRec_StopRecord MMIRECORD_RESULT_SUCCESS");
        s_recorder_state = MMIVOICE_RECORD_STATE_NORMAL;
        WatchRec_StopJumpText();
        WatchRec_ResetTimer();
        MMK_PostMsg(winHandle, MSG_FULL_PAINT, PNULL, PNULL);
        MMI_GetLabelTextByLang(TXT_COMMON_SAVED, &strTitle);
        //strTitle.wstr_len = MMIAPICOM_Wstrlen(text);
        //strTitle.wstr_ptr = text;
        WatchCOM_NoteWin_1Line_Enter(MMIRECORD_WORK_QUERY_WIN_ID,
                                     &strTitle,
                                     0,
                                     softkey_text,
                                     PNULL);
    }
	else if(MMIRECORD_STATE_STOPPED == MMIRECORD_GetRecordState())
    {
        SCI_TRACE_LOW("[RECORD]WatchRec_StopRecord stoped");
        s_recorder_state = MMIVOICE_RECORD_STATE_NORMAL;
        WatchRec_StopJumpText();
        WatchRec_ResetTimer();
        MMK_PostMsg(winHandle, MSG_FULL_PAINT, PNULL, PNULL);
		MMI_GetLabelTextByLang(TXT_COMMON_SAVED, &strTitle);
        //strTitle.wstr_len = MMIAPICOM_Wstrlen(text);
        //strTitle.wstr_ptr = text;
        WatchCOM_NoteWin_1Line_Enter(MMIRECORD_WORK_QUERY_WIN_ID,
                                     &strTitle,
                                     0,
                                     softkey_text,
                                     PNULL);
    }
    //MMIRECORD_UpdateScreen(win_handle, FALSE);

}
LOCAL void WatchRec_StartTimer(void)
{
    uint32  timeOut = RECORD_TIMER_TIMEOUT;

    if(0 != s_timer_id)
    {
        MMK_StopTimer(s_timer_id);
        s_timer_id = 0;
    }
    s_timer_id = MMK_CreateWinTimer(MMIRECORD_MAINPLAY_WIN_ID, timeOut, TRUE);
}
LOCAL void WatchRec_StopTimer(void)
{
    if(0 != s_timer_id)
    {
        MMK_StopTimer(s_timer_id);
        s_timer_id = 0;
    }
}
LOCAL void WatchRec_StartJumpTimer(void)
{
    uint32  timeOut = RECORD_JUMP_TIMER_TIMEOUT;

    if(0 != s_jump_timer_id)
    {
        MMK_StopTimer(s_jump_timer_id);
        s_jump_timer_id = 0;
    }
    s_jump_timer_id = MMK_CreateWinTimer(MMIRECORD_MAINPLAY_WIN_ID, timeOut, TRUE);
}
LOCAL void WatchRec_StopJumpTimer(void)
{
    if(0 != s_jump_timer_id)
    {
        MMK_StopTimer(s_jump_timer_id);
        s_jump_timer_id = 0;
    }
}
#if 0
/*****************************************************************************/
//  Description : start record
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StartRecord(BOOLEAN is_reRecord, MMI_HANDLE_T win_handle)
{
    BOOLEAN error_code = TRUE;
    MMIFILE_ERROR_E   fs_error = SFS_ERROR_NO_SPACE;
    uint32 record_id = MMIRECORD_GetRecordId();
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return FALSE;
    }

    SCI_TRACE_LOW("[RECORD] StartRecord is_reRecord = %d", is_reRecord);

    if(!is_reRecord)
            //&& !record_win_data_ptr->is_insert_record)
    {
        record_win_data_ptr->record_support_type = MMIRECORD_GetRecordFileFormatType();
    }

    fs_error = MMIRECORD_StartRecordNormal(is_reRecord, record_win_data_ptr->record_support_type, &record_id, MMIRECORD_GetRecordStorage());

    UpdateRecordFilePath(record_win_data_ptr);

    switch(fs_error)
    {
    case SFS_ERROR_NONE:
        //WatchCOM_Btn_SetPos_2Btn(MMIRECORD_STOP_BUTTON_CTRL_ID,MMIRECORD_PAUSE_BUTTON_CTRL_ID);
        MMK_PostMsg(win_handle, MSG_FULL_PAINT, PNULL, PNULL);
        WatchRec_StartTimer();
        /*MMIAPICOM_ClosePanelChildWin();
        MMIRECORD_UpdateButtonStatusRecording(win_handle);
        StartDisplayProgressTimer(win_handle);
        MMIRECORD_UpdateScreen(win_handle, FALSE);*/
        break;

    default:
        SCI_TRACE_LOW("[RECORD]start record error");
        //MMIRECORD_UpdateButtonStatusReady(win_handle);
        //MMIRECORD_ShowErrorMsg(fs_error);
        error_code = FALSE;
        break;
    }

    return error_code;
}
#endif
LOCAL void WatchRec_StartRecord(BOOLEAN is_reRecord, MMI_HANDLE_T win_handle)
{
    MMIFILE_DEVICE_E dev_type = MMI_DEVICE_UDISK;
    BOOLEAN error_code = TRUE;
    MMIFILE_ERROR_E   fs_error = SFS_ERROR_NO_SPACE;
    uint32 record_id = MMIRECORD_GetRecordId();
    MMI_STRING_T strTitle = {0};
    WATCH_SOFTKEY_TEXT_ID_T softkey = { PNULL, PNULL, PNULL };
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(WATCH_RECORD_PLAY_STATE_PLAYING == WATCHRECORD_GetPlayStatus())
    {
        MMI_GetLabelTextByLang(TXT_WATCH_RECORD_PLAYING,&strTitle);
#ifdef ADULT_WATCH_SUPPORT
        WatchCOM_NoteWin_2Line_Enter(
                                        MMIRECORD_PLAYING_NOTE_WIN_ID,
                                        &strTitle,
                                        0,
                                        softkey,
                                        PNULL
                                     );
#else
        WatchPUB_Alert_Note_Win(MMIRECORD_PLAYING_NOTE_WIN_ID,TXT_WATCH_RECORD_PLAYING);
#endif
        return;
    }
    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    SCI_TRACE_LOW("[RECORD] StartRecord is_reRecord = %d", is_reRecord);

    if(!is_reRecord)
            //&& !record_win_data_ptr->is_insert_record)
    {
        record_win_data_ptr->record_support_type = MMIRECORD_GetRecordFileFormatType();
    }
	
    if (MMISD_IsSDExistEx())
    {
        SCI_TRACE_LOW("WatchRec_StartRecord().dev_type = MMI_DEVICE_SDCARD.");
	    dev_type = MMI_DEVICE_SDCARD;
    }
 
    fs_error = MMIRECORD_StartRecordNormal(is_reRecord, record_win_data_ptr->record_support_type, 
    &record_id, dev_type);
    //fs_error = MMIRECORD_StartRecordNormal(is_reRecord);

    UpdateRecordFilePath(record_win_data_ptr);

    switch(fs_error)
    {
    case SFS_ERROR_NONE:
        //WatchCOM_Btn_SetPos_2Btn(MMIRECORD_STOP_BUTTON_CTRL_ID,MMIRECORD_PAUSE_BUTTON_CTRL_ID);        
        WatchRec_StartTimer();
        s_recorder_state = MMIVOICE_RECORD_STATE_RECORDING;
        MMK_PostMsg(win_handle, MSG_FULL_PAINT, PNULL, PNULL);
        /*MMIAPICOM_ClosePanelChildWin();
        MMIRECORD_UpdateButtonStatusRecording(win_handle);
        StartDisplayProgressTimer(win_handle);
        MMIRECORD_UpdateScreen(win_handle, FALSE);*/
        break;

    default:
        SCI_TRACE_LOW("[RECORD]start record error:%d",fs_error);
        //MMIRECORD_UpdateButtonStatusReady(win_handle);
        //MMIRECORD_ShowErrorMsg(fs_error);
        //error_code = FALSE;
        break;
    }

    return;
}
/*****************************************************************************/
//  Description :CreateMainWinUserData
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void FreeMainWinUserData(MMI_HANDLE_T      win_handle)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL != record_win_data_ptr)
    {
        SCI_FREE(record_win_data_ptr);
        record_win_data_ptr = PNULL;
    }

}
/*****************************************************************************/
//  Description :CreateMainWinUserData
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateMainWinUserData(MMIRECORD_WORK_PARAM_T *work_param_ptr,
                                    MMI_HANDLE_T      win_handle)
{
    RECORD_WIN_DATA_T  *record_win_data_ptr = PNULL;

    record_win_data_ptr = SCI_ALLOCA(sizeof(RECORD_WIN_DATA_T));

    if(PNULL != record_win_data_ptr)
    {
        SCI_MEMSET(record_win_data_ptr, 0, sizeof(RECORD_WIN_DATA_T));
        record_win_data_ptr->timer_id = 0;
        record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD] = MMIRECORD_BUTTON_STATE_FOCUS;
        record_win_data_ptr->is_record_work_paused = FALSE;

        record_win_data_ptr->record_play_anim.cur_frame  = 0;

        record_win_data_ptr->record_play_anim.total_frame = RECORD_ANIMAL_TOTAL_FRAME_NUM;
#ifdef MMI_RECORD_MINI_SUPPORT
        record_win_data_ptr->record_play_anim.bg.bg_type = GUI_BG_IMG;
        record_win_data_ptr->record_play_anim.bg.img_id = IMAGE_NULL;
#endif

        if(PNULL != work_param_ptr)
        {
            record_win_data_ptr->max_file_size = work_param_ptr->max_file_size;
            record_win_data_ptr->max_file_time = work_param_ptr->max_file_time;
            record_win_data_ptr->record_all_support_type = work_param_ptr->record_support_type;
            record_win_data_ptr->record_support_type = MMIRECORD_DecideRecordWorkFormat(work_param_ptr->record_support_type);
            record_win_data_ptr->is_insert_record = TRUE;
            if(record_win_data_ptr->record_support_type != MMIRECORD_GetRecordFileFormatType())
            {
                MMIRECORD_SetRecordFileFormatType(record_win_data_ptr->record_support_type);
            }
        }
        else
        {
            record_win_data_ptr->record_support_type = MMIRECORD_GetRecordFileFormatType();
        }

        record_win_data_ptr->record_play_anim.win_id  = win_handle;
        record_win_data_ptr->win_handle = win_handle;
        MMK_SetWinUserData(win_handle, record_win_data_ptr);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
LOCAL void WatchRec_MainwinDrawBG(void)
{
    GUI_LCD_DEV_INFO  lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T rect = {0};

    rect.left = 0;
    rect.top = 0;
    rect.right = RECORD_BACKGROUND_RIGHT;
    rect.bottom = RECORD_BACKGROUND_BOTTOM;
    LCD_FillRect(&lcd_dev_info, rect, MMI_BLACK_COLOR);
}
LOCAL void WatchRec_StopJumpText(void)
{
    WatchRec_StopJumpTimer();
    GUILABEL_SetVisible(MMIRECORD_TIME_FORM_CTRL_ID,TRUE,TRUE);
}
LOCAL BOOLEAN WatchRec_MainWin_IsSpaceEnough(void)
{
    uint32 freeHigh = 0;
    uint32 freeLow = 0;
    uint32 tolFreeSpace = 0;
    wchar path_name[2] = {0};

	SCI_TRACE_LOW("[RECORD]WatchRec_MainWin_IsSpaceEnough enter");
    if (MMISD_IsSDExistEx())
    {
        SCI_TRACE_LOW("[RECORD]WatchRec_MainWin_IsSpaceEnough e");
	    SCI_MEMCPY(path_name, L"E", sizeof(wchar)*1);
		SCI_TRACE_LOW("[RECORD]WatchRec_MainWin_IsSpaceEnough eSCI_MEMCPY");
    }
    else
    {
        SCI_TRACE_LOW("[RECORD]WatchRec_MainWin_IsSpaceEnough d");
    	SCI_MEMCPY(path_name, L"D", sizeof(wchar)*1);
    }
    if(!MMIAPIFMM_GetDeviceFreeSpace(path_name,MMIAPICOM_Wstrlen(path_name) , &freeHigh, &freeLow))
    {
        SCI_TRACE_LOW("WatchRec_MainWin_IsSpaceEnough get space err!");
        return FALSE;
    }
    tolFreeSpace = (freeHigh << 22) | (freeLow >> 10);
    if(tolFreeSpace <= MMIRECORD_MIN_NEED_SPACE/1000)
    {
        SCI_TRACE_LOW("WatchRec_MainWin_IsSpaceEnough no enough space!");
        return FALSE;
    }
    return TRUE;
}
/*****************************************************************************/
//  Description :MMIRECORD_OpenRecordMainWin
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC  MMI_HANDLE_T  MMIRECORD_OpenRecordMainWin(MMIRECORD_WORK_PARAM_T *work_param_ptr)
{
    MMI_HANDLE_T      win_handle = 0;

    win_handle = MMK_CreateWinByApplet(SPRD_RECORD_APPLET_ID, (uint32*)MMIRECORD_MAINPLAY_WIN_TAB, PNULL);

    if(0 != win_handle)
    {
        CreateMainWinUserData(work_param_ptr, win_handle);
    }

    return win_handle;
}

/*****************************************************************************/
//  Description : Close Menu
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_CloseMenu(void)
{
    MMK_CloseWin(MMIRECORD_STARTSTATUS_OPTIONMENU_WIN_ID);
}
/*****************************************************************************/
//  Description : Stop Record
//  [in]:isExit -TRUE:exit record window
//                     FALSE:no exit
//  Author:Jiaxin.sun
//  Note:
/*****************************************************************************/
PUBLIC void WATCHRECORD_StopRecord(BOOLEAN isExit)
{
    SCI_TRACE_LOW("[RECORD]WATCHRECORD_StopRecord start");
    if(MMIVOICE_RECORD_STATE_RECORDING == s_recorder_state)
    {
        WatchRec_StopRecord(MMIRECORD_MAINPLAY_WIN_ID);
		if(isExit)
        {
            MMK_CloseWin(MMIRECORD_MAINPLAY_WIN_ID);
        }		
    }
}
/*****************************************************************************/
//  Description : Stop Record Or Play
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void WATCHRECORD_StopRecordOrPlay(void)
{
    WATCHRECORD_StopRecord(TRUE);
    WATCHRECORD_StopPlay();
}
/*****************************************************************************/
//  Description : clock start
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void WATCHRECORD_ClockStart(void)
{
    WATCHRECORD_StopRecord(FALSE);
    //WATCHRECORD_PausePlay();
}
