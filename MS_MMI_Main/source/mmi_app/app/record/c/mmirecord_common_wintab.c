/*****************************************************************************
** File Name:      mmirecord_common_wintab.c                                 *
** Author:         bin.ji                                                    *
** Date:           3/1/2005                                                  *
** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe recorder                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 3/1/2005       bin.ji           Create                                    *
** 8/11/2006      haiyang.hu       Modify                                    *
******************************************************************************/

#define _MMIRECORD_COMMON_WINTAB_C_

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

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
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

/*****************************************************************************/
//  Description : HandleRecordCallbackMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void HandleRecordCallbackMsg(MMI_HANDLE_T win_handle,
                                   DPARAM     param);

/*****************************************************************************/
//  Description : HandlePlayCallbackMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void HandlePlayCallbackMsg(MMI_HANDLE_T win_handle,
                                 DPARAM     param);

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

/*****************************************************************************/
//  Description : OpenOptionMenuWin
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void OpenOptionMenuWin(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : start record
//  Global resource dependence : none
//  Author: haiyanghu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StartRecord(BOOLEAN is_reRecord, MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : PauseRecord
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void PauseRecord(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : ResumeRecord
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void ResumeRecord(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : ResumePlay
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void ResumePlay(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : PausePlay
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void PausePlay(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : StopRecord
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void StopRecord(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : StopPlay
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void StopPlay(MMI_HANDLE_T win_handlen);

/*****************************************************************************/
//  Description : StartPlay
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void StartPlay(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : start record timer
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void StartDisplayProgressTimer(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : stop record timer
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void StopDisplayProgressTimer(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : StopRecordOrPlay
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void StopRecordOrPlay(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : HandleTimerMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTimerMsg(MMI_HANDLE_T win_handle,
                                  DPARAM     param);

/*****************************************************************************/
//  Description : handle get focus msg of record
//  Global resource dependence : none
//  Author: xiaoming.ren
//  Note:
/*****************************************************************************/
LOCAL void HandleRecordGetFocus(MMI_HANDLE_T win_handle);
/*****************************************************************************/
//  Description : handle lose focus msg of record
//  Global resource dependence : none
//  Author: xiaoming.ren
//  Note:
/*****************************************************************************/
LOCAL void HandleRecordLoseFocus(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : HandleOKKeyMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOKKeyMsg(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : handle pub menu win init handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePubMenuInitHandler(MMIPUB_MENU_NOTIFY_T *param);

/*****************************************************************************/
//  Description : handle pub menu win ok handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePubMenuOKHandler(MMIPUB_MENU_NOTIFY_T *param);

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

/*****************************************************************************/
//  Description : set recordfile pah and name
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void UpdateRecordFilePath(RECORD_WIN_DATA_T * record_win_data_ptr);

/**--------------------------------------------------------------------------*
**                          WINDOW  DEFINITION                               *
**--------------------------------------------------------------------------*/
WINDOW_TABLE(MMIRECORD_MAINPLAY_WIN_TAB) =
{
#ifndef MMI_RECORD_MINI_SUPPORT
    WIN_STATUSBAR,
#else
    WIN_TITLE(TXT_RECORDER),
#endif
    WIN_FUNC((uint32)HandleRecordMainPlayWinMsg),
    WIN_ID(MMIRECORD_MAINPLAY_WIN_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    END_WIN
};


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

    if(PNULL == record_win_data_ptr)
    {
        return result;
    }
    
    SCI_TRACE_LOW("[RECORD] HandleWebKeyMsg, entry!");
            
#if !defined (KEYPAD_TYPE_COMPACT_KEYPAD) && !defined (KEYPAD_TYPE_FOUR_KEY_UNIVERSE)
    if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY])
    {
        MMK_PostMsg(win_handle, MSG_RECORD_START_PLAY, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD]
        && MMIRECORD_BUTTON_STATE_NORMAL != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
    {
        MMK_PostMsg(win_handle, MSG_START_RECORD, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE])
    {
        MMK_PostMsg(win_handle, MSG_PAUSE_RECORD, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE])
    {
        MMK_PostMsg(win_handle, MSG_RECORD_PAUSE_PLAY, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
    {
        MMK_PostMsg(win_handle, MSG_RESUME_RECORD, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP])
    {
        MMK_PostMsg(win_handle, MSG_RECORD_RESUME_PLAY, PNULL, PNULL);
    }
#endif
 
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

    if(MMIRECORD_BUTTON_STATE_NORMAL != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]
            && MMIRECORD_BUTTON_STATE_FOCUS != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]
            && MMIRECORD_BUTTON_STATE_NORMAL != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]
            && MMIRECORD_BUTTON_STATE_FOCUS != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP])
    {
        MMK_PostMsg(win_handle, MSG_OPEN_OPTION_WIN, PNULL, PNULL);
    }

#if !defined (KEYPAD_TYPE_COMPACT_KEYPAD) && !defined (KEYPAD_TYPE_FOUR_KEY_UNIVERSE)
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE])
    {
        MMK_PostMsg(win_handle, MSG_PAUSE_RECORD, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE])
    {
        MMK_PostMsg(win_handle, MSG_RECORD_PAUSE_PLAY, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
    {
        MMK_PostMsg(win_handle, MSG_RESUME_RECORD, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP])
    {
        MMK_PostMsg(win_handle, MSG_RECORD_RESUME_PLAY, PNULL, PNULL);
    }

#endif
    return result;
}

/*****************************************************************************/
//  Description : HandleRecordCallbackMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void HandleRecordCallbackMsg(MMI_HANDLE_T win_handle,
                                   DPARAM     param)
{
    MMIRECORD_RESULT_E result = FALSE;

    if(PNULL == param)
    {
        return;
    }

    result = *(MMIRECORD_RESULT_E *)param;
    SCI_TRACE_LOW("[RECORD] HandleRecordCallbackMsg result = %d", result);
    MMIDEFAULT_TurnOnBackLight();
    StopDisplayProgressTimer(win_handle);

    if(MMIRECORD_RESULT_ERROR == result)
    {
        MMIPUB_CloseAlertWin();
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_FAIL, PNULL, PNULL,
                                    MMIPUB_SOFTKEY_ONE, PNULL);
        MMIRECORD_UpdateButtonStatusReady(win_handle);
    }
    else
    {
        if(MMIRECORD_RESULT_NO_SPACE == result)
        {
            MMIRECORD_ShowErrorMsg(SFS_ERROR_NO_SPACE);
        }

        MMIRECORD_UpdateButtonStatusStopped(win_handle);
    }

    MMIRECORD_UpdateScreen(win_handle, FALSE);
}

/*****************************************************************************/
//  Description : HandlePlayCallbackMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void HandlePlayCallbackMsg(MMI_HANDLE_T win_handle,
                                 DPARAM     param)
{
    BOOLEAN is_succ = FALSE;

    if(PNULL == param)
    {
        return;
    }

    is_succ = *(BOOLEAN *)param;

    SCI_TRACE_LOW("[RECORD] HandlePlayCallbackMsg is_succ = %d", is_succ);
    //playing end
    MMIDEFAULT_TurnOnBackLight();

    if(!is_succ)
    {
        MMIPUB_OpenAlertFailWin(TXT_COMMON_NO_SUPPORT);
    }
    else
    {
        MMIRECORD_UpdateScreen(win_handle, FALSE);
        MMITHEME_UpdateRect();
    }

    StopDisplayProgressTimer(win_handle);
    MMIRECORD_UpdateButtonStatusStopped(win_handle);
    MMIRECORD_UpdateScreen(win_handle, FALSE);

}

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
                StopRecord(win_handle);
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
        MMIRECORD_RequestVirtualHandle();

        if(!MMIRECORD_Constructor())
        {
            MMK_CloseWin(win_id);
            break;
        }

        UpdateRecordFilePath(record_win_data_ptr);
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
        
    case MSG_GET_FOCUS:
        HandleRecordGetFocus(record_win_data_ptr->win_handle);
        break;

    case MSG_APP_UPSIDE:
    case MSG_APP_UP:  //增大音量，显示音量调节杆
        if(MMIRECORD_BUTTON_STATE_FOCUS ==  record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME]
                || MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME])
        {
            MMIRECORD_TurnVol(MMICOM_PANEL_OPEN_INC);
        }

        break;

    case MSG_APP_DOWNSIDE:
    case MSG_APP_DOWN:  //减小音量，显示音量调节杆
        if(MMIRECORD_BUTTON_STATE_FOCUS ==  record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME]
                || MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME])
        {
            MMIRECORD_TurnVol(MMICOM_PANEL_OPEN_DEC);
        }

        break;

    case MSG_APP_HASH:
        if(MMIRECORD_BUTTON_STATE_FOCUS ==  record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME]
                || MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME])
        {
            MMIRECORD_SetMute();
        }

        break;

    case MSG_LOSE_FOCUS:
        HandleRecordLoseFocus(record_win_data_ptr->win_handle);
        break;

    case MSG_RECORD_CALL_BACK:
        HandleRecordCallbackMsg(record_win_data_ptr->win_handle, param);
        break;

    case MSG_RECORD_PLAY_CALL_BACK:
        HandlePlayCallbackMsg(record_win_data_ptr->win_handle, param);
        break;

    case MSG_STOP_RECORD_PLAY:
        StopRecordOrPlay(record_win_data_ptr->win_handle);
        break;

    case MSG_RECORD_START_PLAY:
        StartPlay(record_win_data_ptr->win_handle);
        break;

    case MSG_START_RECORD:
        if(PNULL != param)
        {
            StartRecord(*(BOOLEAN *)param, record_win_data_ptr->win_handle);
        }
        else
        {
            StartRecord(FALSE, record_win_data_ptr->win_handle);
        }

        break;

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
       	    StopRecord(record_win_data_ptr->win_handle);
	 	  }
		}
        break;

    case MSG_RECORD_STOP_PLAY:
        StopPlay(record_win_data_ptr->win_handle);
        break;

    case MSG_RECORD_PAUSE_PLAY:
        PausePlay(record_win_data_ptr->win_handle);
        break;

    case MSG_PAUSE_RECORD:
        PauseRecord(record_win_data_ptr->win_handle);
        break;

    case MSG_RECORD_RESUME_PLAY:
        ResumePlay(record_win_data_ptr->win_handle);
        break;

    case MSG_RESUME_RECORD:
        ResumeRecord(record_win_data_ptr->win_handle);
        break;

    case MSG_INSERT_RECORD:
        if(0 != record_win_data_ptr->record_file_name_len
                && MMIRECORD_InsertRecord(record_win_data_ptr->record_file_full_path, record_win_data_ptr->record_file_full_path_len))
        {
            MMK_CloseWin(win_id);
        }

        break;

    case MSG_OPEN_OPTION_WIN:
        OpenOptionMenuWin(win_id);
        break;

    case MSG_TIMER:
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
        break;

    /*case MSG_CLOSE_WINDOW:
		record_win_data_ptr->is_insert_record = FALSE;
        StopRecordOrPlay(record_win_data_ptr->win_handle);
        FreeMainWinUserData(record_win_data_ptr->win_handle);
        MMIRECORD_Destructor();
        MMIRECORD_FreeVirtualHandle();
        MMK_CloseApplet(SPRD_RECORD_APPLET_ID);
        break;*/ //bug-72892;

    case MSG_PROMPTWIN_OK:
        if(PNULL != param)
        {
            MMI_WIN_ID_T            query_win_id = *(MMI_WIN_ID_T*)param;
            MMK_CloseWin(query_win_id);

            if(MMIRECORD_RESULT_NO_EXIST == MMIRECORD_DeleteRecordFile())
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COM_FILE_NO_EXIST, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL,
                                            MMIPUB_SOFTKEY_ONE, PNULL);
            }

            UpdateRecordFilePath(record_win_data_ptr);

        }

        break;

    case MSG_PROMPTWIN_CANCEL:
        if(PNULL != param)
        {
            MMI_WIN_ID_T            query_win_id = *(MMI_WIN_ID_T*)param;
            MMK_CloseWin(query_win_id);
        }

        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    recode = MMIRECORD_HandleRecordMainPlayWinMsg(win_id,msg_id,param);

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
        StopRecordOrPlay(record_win_data_ptr->win_handle);  
		FreeMainWinUserData(record_win_data_ptr->win_handle);
		MMIRECORD_Destructor();
        MMIRECORD_FreeVirtualHandle();   
		MMK_CloseApplet(SPRD_RECORD_APPLET_ID);
    }

    return recode;
}

/*****************************************************************************/
//  Description : StopRecordOrPlay
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void StopRecordOrPlay(MMI_HANDLE_T win_handle)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
    {
        StopRecord(win_handle);
    }
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP])
    {
        StopPlay(win_handle);
    }
}

/*****************************************************************************/
//  Description : handle pub menu win ok handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePubMenuOKHandler(MMIPUB_MENU_NOTIFY_T *param)
{
    MMI_MENU_GROUP_ID_T     group_id    = MENU_RECORD_START_OPTION;
    MMI_MENU_ID_T           menu_id     = ID_RECORD_STORIGE_POS;
    BOOLEAN                 is_Rerecord  = FALSE;
    MMI_WIN_ID_T            query_win_id = MMIRECORD_QUERY_WIN_ID;


    if(PNULL != param)
    {
        RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(param->dst_handle);
        GUIMENU_GetId(param->ctrl_id, &group_id, &menu_id);

        switch(menu_id)
        {
        case ID_RECORD_START:
            MMK_PostMsg(param->dst_handle, MSG_START_RECORD, PNULL, PNULL);
            MMK_CloseWin(param->win_id);
            break;

        case ID_RECORD_LIST:
            if(PNULL != record_win_data_ptr)
            {
                MMIRECORD_OpenRecordFileList();
            }

            MMK_CloseWin(param->win_id);
            break;

        case ID_RECORD_STOPPED_PLAY:
            MMK_CloseWin(param->win_id);
            MMK_PostMsg(param->dst_handle , MSG_RECORD_START_PLAY, PNULL, PNULL);
            break;

        case ID_RECORD_AGAIN:
            is_Rerecord = TRUE;
            MMK_PostMsg(param->dst_handle, MSG_START_RECORD, &is_Rerecord, sizeof(BOOLEAN));
            MMK_CloseWin(param->win_id);
            break;

        case ID_RECORD_DELETE:
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE, IMAGE_PUBWIN_QUERY, &query_win_id, PNULL);
            break;

        case ID_RECORD_INSERT://recordwork
            MMK_PostMsg(param->dst_handle, MSG_INSERT_RECORD, PNULL, PNULL);
            MMK_CloseWin(param->win_id);
            break;

        case ID_RECORD_SEND_BY_MMS:
            if(PNULL != record_win_data_ptr
                    && 0 != record_win_data_ptr->record_file_name_len)
            {
                MMIRECORD_SendFileViaMMS(record_win_data_ptr->record_file_full_path, record_win_data_ptr->record_file_full_path_len);
            }

            MMK_CloseWin(param->win_id);
            break;

        case ID_RECORD_SEND_BY_BT:
            if(PNULL != record_win_data_ptr
                    && 0 != record_win_data_ptr->record_file_name_len)
            {
                MMIRECORD_SendFileViaBT(record_win_data_ptr->record_file_full_path, record_win_data_ptr->record_file_full_path_len);
            }

            MMK_CloseWin(param->win_id);
            break;

        case ID_RECORD_STORIGE_FORMAT_WAV:
            MMIRECORD_SetRecordFileFormatType(MMIRECORD_WORK_FORMAT_WAV);
            MMK_CloseWin(param->win_id);
            break;

        case ID_RECORD_STORIGE_FORMAT_MP3://andy.he_record
            //set storage file format type  menu
            MMIRECORD_SetRecordFileFormatType(MMIRECORD_WORK_FORMAT_MP3);
            MMK_CloseWin(param->win_id);
            break;

        case ID_RECORD_STORIGE_FORMAT_AMR://andy.he_record
            //set storage file format type  menu
            MMIRECORD_SetRecordFileFormatType(MMIRECORD_WORK_FORMAT_AMR);
            MMK_CloseWin(param->win_id);
            break;

        default:
            if(menu_id >= ID_RECORD_UDISK && menu_id <= ID_RECORD_UDISK_MAX)
            {
                //set storage menu
                MMIRECORD_SetRecordStorage(MMI_DEVICE_UDISK + menu_id - ID_RECORD_UDISK);
                MMK_CloseWin(param->win_id);
            }

            break;
        }
    }
}


/*****************************************************************************/
//  Description : handle pub menu win init handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePubMenuInitHandler(MMIPUB_MENU_NOTIFY_T *param)
{
    if(PNULL != param)
    {
        MMIRECORD_LoadRecordMenu(param->dst_handle);
        MMK_SetAtvCtrl(param->win_id, param->ctrl_id);
    }

}


/*****************************************************************************/
//  Description : OpenOptionMenuWin
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void OpenOptionMenuWin(MMI_HANDLE_T win_handle)
{
    MMIPUB_MENU_PARAM_T menuparam = {0};

    MMIAPICOM_ClosePanelChildWin();

    menuparam.group_id = (MMI_MENU_GROUP_ID_T)MENU_RECORD_START_OPTION;
    menuparam.win_id = (MMI_WIN_ID_T)MMIRECORD_STARTSTATUS_OPTIONMENU_WIN_ID;
    menuparam.ctrl_id = (MMI_CTRL_ID_T)MMIRECORD_STARTSTATUS_OPTIONMENU_CTRL_ID;
    menuparam.applet_handle = SPRD_RECORD_APPLET_ID;

    menuparam.dst_handle = win_handle;
    menuparam.func.OnMenuOk = HandlePubMenuOKHandler;
    menuparam.func.OnMenuInit = HandlePubMenuInitHandler;
    MMIPUB_OpenPubMenuWin(&menuparam);



    //MMK_CreateWin((uint32 *)MMIRECORD_STARTSTATUS_OPTIONMENU_WIN_TAB,(ADD_DATA)win_handle);
}
/*****************************************************************************/
//  Description : PauseRecord
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void PauseRecord(MMI_HANDLE_T win_handle)
{
    if(MMIRECORD_PauseRecord())
    {
        MMIRECORD_UpdateButtonStatusRecordPaused(win_handle);
        StopDisplayProgressTimer(win_handle);
        MMIRECORD_UpdateScreen(win_handle, FALSE);
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
        StartDisplayProgressTimer(win_handle);
        MMIRECORD_UpdateScreen(win_handle, FALSE);
    }
}

/*****************************************************************************/
//  Description : ResumePlay
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void ResumePlay(MMI_HANDLE_T win_handle)
{
    if(MMIRECORD_ResumePlay())
    {
        MMIRECORD_UpdateButtonStatusPlaying(win_handle);
        StartDisplayProgressTimer(win_handle);
        MMIRECORD_UpdateScreen(win_handle, FALSE);
    }
}

/*****************************************************************************/
//  Description : PausePlay
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void PausePlay(MMI_HANDLE_T win_handle)
{
    if(MMIRECORD_PausePlay())
    {
        MMIRECORD_UpdateButtonStatusPlayPause(win_handle);
        StopDisplayProgressTimer(win_handle);
        MMIRECORD_UpdateScreen(win_handle, FALSE);
    }
}

/*****************************************************************************/
//  Description : StopPlay
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void StopPlay(MMI_HANDLE_T win_handle)
{
    if(MMIRECORD_StopPlay())
    {
        MMIRECORD_UpdateButtonStatusStopped(win_handle);
        StopDisplayProgressTimer(win_handle);
        MMIRECORD_UpdateScreen(win_handle, FALSE);
    }

}

/*****************************************************************************/
//  Description : StartPlay
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void StartPlay(MMI_HANDLE_T win_handle)
{

    MMIRECORD_RESULT_E  result = MMIRECORD_RESULT_SUCCESS;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);


    result = MMIRECORD_PlayCurrent();

    if(MMIRECORD_RESULT_SUCCESS == result)
    {
        MMIRECORD_UpdateButtonStatusPlaying(win_handle);
        StartDisplayProgressTimer(win_handle);
    }
    else if(MMIRECORD_RESULT_NO_EXIST == result)
    {
        UpdateRecordFilePath(record_win_data_ptr);
        MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_NO_EXIST);
        MMIRECORD_UpdateButtonStatusReady(win_handle);
    }
    else
    {
        MMIPUB_OpenAlertFailWin(TXT_COMMON_NO_SUPPORT);
    }

    MMIRECORD_UpdateScreen(win_handle, FALSE);
}
/*****************************************************************************/
//  Description : StopRecord
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void StopRecord(MMI_HANDLE_T win_handle)
{
    MMI_WIN_ID_T    query_win_id = MMIRECORD_WORK_QUERY_WIN_ID;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    if(MMIRECORD_RESULT_SUCCESS == MMIRECORD_StopRecord())
    {
        if(record_win_data_ptr->is_insert_record)//recordwork
        {
            MMIPUB_OpenQueryWinByTextIdEx(SPRD_RECORD_APPLET_ID,
                                          TXT_RECORD_SAVEAINSERTRECORD,
                                          IMAGE_PUBWIN_QUERY,
                                          &query_win_id,
                                          HandleRecordWorkQueryWinMsg,
                                          win_handle);
        }
        else
        {
            MMIPUB_OpenAlertSuccessWin(TXT_COMM_SAVE_SUCCESS);
        }

        MMIRECORD_UpdateButtonStatusStopped(win_handle);
        StopDisplayProgressTimer(win_handle);
    }

    MMIRECORD_UpdateScreen(win_handle, FALSE);

}
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
        MMIAPICOM_ClosePanelChildWin();
        MMIRECORD_UpdateButtonStatusRecording(win_handle);
        StartDisplayProgressTimer(win_handle);
        MMIRECORD_UpdateScreen(win_handle, FALSE);
        break;

    default:
        MMIRECORD_UpdateButtonStatusReady(win_handle);
        MMIRECORD_ShowErrorMsg(fs_error);
        error_code = FALSE;
        break;
    }

    return error_code;
}


/*****************************************************************************/
//  Description : start record timer
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void StartDisplayProgressTimer(MMI_HANDLE_T win_handle)
{
    uint32  time_out = MMI_RECORD_TIME_OUT;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }


    if(0 != record_win_data_ptr->timer_id)
    {
        MMK_StopTimer(record_win_data_ptr->timer_id);
        record_win_data_ptr->timer_id = 0;
    }

    record_win_data_ptr->timer_id = MMK_CreateWinTimer(win_handle, time_out, FALSE);

}

/*****************************************************************************/
//  Description : stop record timer
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void StopDisplayProgressTimer(MMI_HANDLE_T win_handle)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    if(0 != record_win_data_ptr->timer_id)
    {
        MMK_StopTimer(record_win_data_ptr->timer_id);
        record_win_data_ptr->timer_id = 0;
    }
}

/*****************************************************************************/
//  Description :HandleRecordWorkQueryWinMsg
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRecordWorkQueryWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
)//recordwork
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *info_ptr     = (MMIPUB_INFO_T*)MMK_GetWinAddDataEx(win_id);
    MMI_HANDLE_T  mainwin_handle  = 0;


    switch(msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        if(PNULL != info_ptr)
        {
            mainwin_handle  = info_ptr->user_data;
            MMK_PostMsg(mainwin_handle, MSG_INSERT_RECORD, PNULL, PNULL);
        }

        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : handle lose focus msg of record
//  Global resource dependence : none
//  Author: xiaoming.ren
//  Note:
/*****************************************************************************/
LOCAL void HandleRecordLoseFocus(MMI_HANDLE_T win_handle)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL != record_win_data_ptr
            && record_win_data_ptr->is_insert_record
            && (0 != record_win_data_ptr->max_file_time
                || 0 != record_win_data_ptr->max_file_size))
    {
        if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE]
                || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE])
        {
            SCI_TRACE_LOW("[MMIRECORD] Main Win Lose Focus, cur is work state, will pause record");
            PauseRecord(win_handle);
            record_win_data_ptr->is_record_work_paused = TRUE;
        }
    }
}

/*****************************************************************************/
//  Description : handle get focus msg of record
//  Global resource dependence : none
//  Author: xiaoming.ren
//  Note:
/*****************************************************************************/
LOCAL void HandleRecordGetFocus(MMI_HANDLE_T win_handle)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    if(record_win_data_ptr->is_insert_record
            && (0 != record_win_data_ptr->max_file_size
                || 0 != record_win_data_ptr->max_file_time))
    {
        if(record_win_data_ptr->is_record_work_paused)
        {
            SCI_TRACE_LOW("[MMIRECORD] Main Win Get Focus, cur is work state, will resume record");
            ResumeRecord(win_handle);
            record_win_data_ptr->is_record_work_paused = FALSE;
        }
    }

    if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]
            || MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
    {
        StartDisplayProgressTimer(win_handle);
    }
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
/*Edit by script, ignore 2 case. Thu Apr 26 19:01:05 2012*/ //IGNORE9527


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
