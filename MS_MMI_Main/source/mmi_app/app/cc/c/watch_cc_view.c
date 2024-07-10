/*****************************************************************************
** File Name:      watch_cc_view.c                                           *
** Author:         yuming.yang                                               *
** Date:           02/12/2020                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:    This file is used to draw cc module interface             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                     NAME             DESCRIPTION                     *
** 02/2020               yuming.yang            Create                       *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "window_parse.h"
#include "mmi_appmsg.h"
#include "mmi_resource.h"
#include "guitext.h"
#include "guilabel.h"
#include "guibutton.h"
#include "guistring.h"
#include "mmidisplay_data.h"
#include "mmicc_export.h"
#include "mmicc_id.h"
#include "mmicc_internal.h"
#include "mmicc_text.h"
#include "mmicc_image.h"
#include "mmicc_anim.h"
#include "mmicc_audio.h"
#include "mmicom_string.h"
#include "mmipb_common.h"
#include "mmicl_export.h"
#include "mmiset_export.h"
#include "mmicom_trace.h"
#include "mmipb_datalist.h"
#include "watch_common_btn.h"
#include "watch_commonwin_export.h"
#include "watch_common.h"
#include "watch_cc_view.h"
#include "mmismsapp_interface.h"
#include "mmisms_export.h"
#ifdef MMI_RECORD_SUPPORT
#include "watch_record_api.h"
#endif
#include "mmidata_share.h"
//#ifdef MMI_ETWS_SUPPORT //pclint bug 2099193
#include "mmisms_export.h"
//#endif
#include "watch_sms_presetlist.h"
#include "mmieng_export.h"
#include "guires.h"

#include "zdt_app.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

#ifdef SCREEN_SHAPE_CIRCULAR
#define WATCH_MO_PHOTO_START_POINT          DP2PX_POINT(84,24)
#define WATCH_MT_PHOTO_START_POINT          DP2PX_POINT(84, 48)
#define WATCH_MO_NAME_DISP_RECT             DP2PX_RECT(36, 108, 204, 132)
#define WATCH_MT_NAME_DISP_RECT             DP2PX_RECT(36, 132, 204, 156)
#define WATCH_CALL_STATE_DISP_RECT          DP2PX_RECT(36, 140, 204, 156)
#define WATCH_RECORD_STATE_DISP_RECT        {168,12,228,28}
#define WATCH_PULL_DOWN_START_POINT_LEFT    {52,0}
#define WATCH_PULL_DOWN_START_POINT_RIGHT   {164,0}
#define WATCH_MO_QUERY_STR_DISP_RECT        DP2PX_RECT(36, 36, 204, 150)
#define WATCH_PULL_DOWN_START_POINT         DP2PX_POINT(108, 0)
#define WATCH_SMS_REJECT_DISP_RECT          DP2PX_RECT(36, 24, 204, 40)
#define WATCH_SMS_REJECT_TP_VAILD_RECT      DP2PX_RECT(52,0,188,24)
#define WATCH_CALL_TIME_DISP_RECT           DP2PX_RECT(36, 140, 204, 156)
#define WATCH_RECORDING_BG_DISP_RECT        {0,0,239,239}
#define WATCH_CALLING_TEXT_DISP_RECT        DP2PX_RECT(36, 24, 204, 40)

#define WATCH_STRING_FONT_BIG               DP_FONT_22
#else
#define WATCH_MO_PHOTO_START_POINT          {84,16}
#define WATCH_MT_PHOTO_START_POINT          {84,36}
#define WATCH_MO_QUERY_STR_DISP_RECT        {12,0,228,152}
#define WATCH_MO_NAME_DISP_RECT             {12,100,228,124}
#define WATCH_MT_NAME_DISP_RECT             {12,124,228,148}
#define WATCH_CALL_STATE_DISP_RECT          {12,136,228,156}
#define WATCH_RECORD_STATE_DISP_RECT        {168,12,228,28}
#define WATCH_PULL_DOWN_START_POINT_LEFT    {52,0}
#define WATCH_PULL_DOWN_START_POINT_RIGHT   {164,0}
#define WATCH_SMS_REJECT_DISP_RECT          {88,0,152,24}
#define WATCH_SMS_REJECT_TP_VAILD_RECT      {52,0,188,24}
#define WATCH_CALL_TIME_DISP_RECT           {12,136,228,152}
#define WATCH_RECORDING_BG_DISP_RECT        {0,0,239,239}

#define WATCH_STRING_FONT_BIG               SONG_FONT_30
#endif

#define WATCH_STRING_FONT_MIDDLE            DP_FONT_24
#define WATCH_STRING_FONT_SMALL             DP_FONT_16
#define WATCH_NOTE_LEN_FOR_1LINE            215

#define CC_WATCH_WAITING_CALL_PERIOD        MMI_2SECONDS    //每次播放间隔时间
#define CC_WATCH_WAITING_CALL_PLAY_TIMER    6   //呼叫等待时候，tone播放的次数
#define CC_WATCH_DTMF_WAITING_TIME          MMI_3SECONDS

#ifdef MMI_VOLTE_SUPPORT
#define WATCH_VOLTE_ICON_START_POINT        {6,6}
#endif

/**--------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN  s_cc_reject_call_by_sms = FALSE;
LOCAL uint8    s_cc_waiting_call_ring_timer = 0; //Bug 2194799
LOCAL uint8    s_cc_call_time_count_timer = 0;
//拒接陌生电话挂断电话标记
PUBLIC BOOLEAN s_cc_reject_call_by_unkown_call = FALSE;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle voice call query window messages
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleCcMoCallQueryWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          );
/*****************************************************************************/
//  Description : handle voice calling window messages
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleCcMoCallingWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          );
/*****************************************************************************/
//  Description : handle voice call connected window messages
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleCcVoiceCallConnectedWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          );
/*****************************************************************************/
//  Description : handle voice call incoming window messages
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleCcMtCallingWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          );

/*****************************************************************************/
//	Description : the process message function of query win
//	Global resource dependence :
//  Author:feiyue.ji
//	Note:2020.7.23
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSendDTMFQueryWinMsg(
                                                MMI_WIN_ID_T win_id,
                                                MMI_MESSAGE_ID_E msg_id,
                                                DPARAM param);

/*****************************************************************************/
//	Description : the process message function of the sending dtmf
//	Global resource dependence :
//  Author:feiyue.ji
//	Note:2020.7.23
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSendingDTMFNoteWinMsg(
                                                 MMI_WIN_ID_T win_id,
                                                 MMI_MESSAGE_ID_E msg_id,
                                                 DPARAM param);

/*****************************************************************************/
//  Description : handle call failed
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CC_HandleMoCallFailed(MMICC_MO_CALL_ERROR_TYPE_E type);

/*****************************************************************************/
//  Description : convert time count to str
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void  CC_TimeCountToStr(MMI_STRING_T* time_str_ptr, uint32 time_count);

/*****************************************************************************/
//  Description : open the window when mo call
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void  CC_OpenWatchMoCallWin(void);
/*****************************************************************************/
//  Description : open the window when mt call
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void  CC_OpenWatchMtCallWin(void);

/*****************************************************************************/
//  Description : update mo call status(button and time)
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void  CC_UpdateWatchMoCallStatus(void);

/*****************************************************************************/
//  Description : update mt call status(button and time)
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void  CC_UpdateWatchMtCallStatus(void);

/*****************************************************************************/
//  Description : update disconnected call status
//  Global resource dependence : none
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
LOCAL void  CC_UpdateWatchDisconnectedCallStatus(void);

#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
//  Description : 开始录音或停止录音
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CC_HandleRecordButton(MMI_WIN_ID_T win_id);
#endif

/*****************************************************************************/
//	Description : send DTMF when a call come into active state
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_SendStoredDtmf(void);

/*****************************************************************************/
//	Description : send DTMF note win
//	Global resource dependence :
//  Author:feiyue.ji
//	Note:2020.7.23
/*****************************************************************************/
LOCAL void CC_OpenSendDtmfPromptWin( void );

/*****************************************************************************/
//description :
//lobal resource dependence :
//Author: feiyue.ji
//date:2020.07.21
/*****************************************************************************/
//PUBLIC void  CC_StartDtmfWaitingTimer(void);

/*---------------------------------------------------------------------------*/
/*                          WINDOW TABLE DEFINITION                          */
/*---------------------------------------------------------------------------*/
#define WATCH_WIN_TAB 1
#if WATCH_WIN_TAB

WINDOW_TABLE(WATCHCC_QUERYMOCALL_WIN_TAB) =
{
    WIN_ID(WATCHCC_QUERYMOCALL_WIN_ID),
    WIN_FUNC((uint32)HandleCcMoCallQueryWinMsg),
    WIN_HIDE_STATUS,
    WIN_STYLE(WS_DISABLE_RETURN_WIN),
    CREATE_BUTTON_CTRL(IAMGE_CALL_BACK, WATCHCC_BACK_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IAMGE_CALL_DIAL, WATCHCC_DIAL_BUTTON_CTRL_ID),
    WIN_SOFTKEY(STXT_RETURN, TXT_NULL, TXT_CALL),
    END_WIN
};//mo call query win

WINDOW_TABLE(WATCHCC_MOCALLING_WIN_TAB) =
{
    WIN_ID(WATCHCC_MOCALLING_WIN_ID),
    WIN_FUNC((uint32)HandleCcMoCallingWinMsg),
    WIN_HIDE_STATUS,
    WIN_STYLE(WS_DISABLE_RETURN_WIN),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,  WATCHCC_CALLSTATE_LABEL_CTRL_ID),
    CREATE_BUTTON_CTRL(IAMGE_CALL_HANGUP, WATCHCC_HANGUP_BUTTON_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_CC_HANG_OFF, TXT_NULL),
    END_WIN
};//mo calling win

WINDOW_TABLE(WATCHCC_CONNECTED_WIN_TAB) =
{
    WIN_ID(WATCHCC_CONNECTED_WIN_ID),
    WIN_FUNC((uint32)HandleCcVoiceCallConnectedWinMsg),
    WIN_HIDE_STATUS,
    WIN_STYLE(WS_DISABLE_RETURN_WIN),
#ifdef CALL_RECORD_SUPPORT
    CREATE_BUTTON_CTRL(IAMGE_CALL_RECORDOFF, WATCHCC_RECORDOFF_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IAMGE_CALL_RECORDON, WATCHCC_RECORDON_BUTTON_CTRL_ID),
#endif
    CREATE_BUTTON_CTRL(IAMGE_CALL_HANGUP, WATCHCC_HANGUP_BUTTON_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,  WATCHCC_CALLTIME_LABEL_CTRL_ID),
#ifdef CALL_RECORD_SUPPORT
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_LEFT,  WATCHCC_CALLRECORD_LABEL_CTRL_ID),
#endif
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    WIN_SOFTKEY(TXT_RECORDER, TXT_NULL, TXT_CC_HANG_OFF),
    END_WIN
};//mo or mt connected

WINDOW_TABLE(WATCHCC_MTCALLING_WIN_TAB) =
{
    WIN_ID(WATCHCC_MTCALLING_WIN_ID),
    WIN_FUNC((uint32)HandleCcMtCallingWinMsg),
    WIN_HIDE_STATUS,
    WIN_STYLE(WS_DISABLE_RETURN_WIN),
    CREATE_BUTTON_CTRL(IAMGE_CALL_DIAL, WATCHCC_DIAL_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IAMGE_CALL_HANGUP, WATCHCC_HANGUP_BUTTON_CTRL_ID),
    WIN_SOFTKEY(TXT_CC_HANG_OFF, TXT_NULL, TXT_COMMON_ANSWER_CALL),
    END_WIN
};//mt incoming win

#endif


/*****************************************************************************/
//  Description : Open mo call query window
//  Parameter: [In] contact info
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_MoCallQueryWin_Enter(void* param)
{
    MMIPB_CONTACT_T*   pInfo = PNULL;

    pInfo = (MMIPB_CONTACT_T *)SCI_ALLOC_APPZ(sizeof(MMIPB_CONTACT_T)); //Bug 2200562
    SCI_ASSERT(pInfo != PNULL);
    SCI_MEMCPY(pInfo, (MMIPB_CONTACT_T *)param, sizeof(MMIPB_CONTACT_T));
    MMK_CreateWin((uint32 *)WATCHCC_QUERYMOCALL_WIN_TAB, pInfo);
}

/*****************************************************************************/
//  Description : Close mo call query window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_MoCallQueryWin_Exit(void)
{
    if(MMK_IsOpenWin(WATCHCC_QUERYMOCALL_WIN_ID))
    {
        MMK_CloseWin(WATCHCC_QUERYMOCALL_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : Open mo calling window
//  Parameter: [In] contact info
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_MoCallingWin_Enter(void* param)
{
    MMK_CreateWin((uint32 *)WATCHCC_MOCALLING_WIN_TAB, param);
}

/*****************************************************************************/
//  Description : Close mo calling window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_MoCallingWin_Exit(void)
{
    if(MMK_IsOpenWin(WATCHCC_MOCALLING_WIN_ID))
    {
        MMK_CloseWin(WATCHCC_MOCALLING_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : Open mo or mt connected window
//  Parameter: [In] contact info
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_ConnectedWin_Enter(void* param)
{
    MMK_CreateWin((uint32 *)WATCHCC_CONNECTED_WIN_TAB, param);
}

/*****************************************************************************/
//  Description : Close mo or mt connected window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_ConnectedWin_Exit(void)
{
    if(MMK_IsOpenWin(WATCHCC_CONNECTED_WIN_ID))
    {
        MMK_CloseWin(WATCHCC_CONNECTED_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : Open mt incoming window
//  Parameter: [In] contact info
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_MtCallingWin_Enter(void* param)
{
    MMK_CreateWin((uint32 *)WATCHCC_MTCALLING_WIN_TAB, param);
}

/*****************************************************************************/
//  Description : Close mt incoming window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_MtCallingWin_Exit(void)
{
    if(MMK_IsOpenWin(WATCHCC_MTCALLING_WIN_ID))
    {
        MMK_CloseWin(WATCHCC_MTCALLING_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : open call ended tip window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_CallEnded_NoteWin_Enter(void)
{
    MMI_WIN_ID_T    winId   = WATCHCC_CALL_ENDED_WIN_ID;
    MMI_STRING_T    noteStr = {0};
    WATCH_SOFTKEY_TEXT_ID_T softKey = {0};
    MMI_GetLabelTextByLang(TXT_CC_CALLENDED, &noteStr);
    WatchCOM_NoteWin_FullScreen_Enter(winId, NULL, &noteStr, IAMGE_CALL_NOTIFY_ENDED, NULL, softKey, PNULL);
}

/*****************************************************************************/
//  Description : record saved tip window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_RecordSaved_NoteWin_Enter(void)
{
    MMI_WIN_ID_T    winId   = WATCHCC_RECORD_SAVED_WIN_ID;
    MMI_STRING_T    noteStr = {0};
    WATCH_SOFTKEY_TEXT_ID_T softKey = {0};
    MMI_GetLabelTextByLang(TXT_CC_RECOEDSAVED, &noteStr);
    WatchCOM_NoteWin_1Line_Enter(winId, &noteStr, NULL, softKey, NULL);
}

/*****************************************************************************/
//  Description : open call ended and record saved tip window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_EndedAndSaved_NoteWin_Enter(void)
{
    MMI_WIN_ID_T    winId   = WATCHCC_ENDED_AND_SAVED_WIN_ID;
    MMI_STRING_T    noteStr = {0};
    WATCH_SOFTKEY_TEXT_ID_T softKey = {0};
    MMI_GetLabelTextByLang(TXT_CC_ENDEDANDSAVED, &noteStr);
    WatchCOM_NoteWin_FullScreen_Enter(winId, NULL, &noteStr, IAMGE_CALL_NOTIFY_SAVED, NULL, softKey, PNULL);
}

/*****************************************************************************/
//  Description : open sms send and call reject tip window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_SentAndRejected_NoteWin_Enter(void)
{
    MMI_WIN_ID_T    winId   = WATCHCC_SENT_AND_REJECTED_WIN_ID;
    MMI_STRING_T    noteStr = {0};
    WATCH_SOFTKEY_TEXT_ID_T softKey = {0};
    MMI_GetLabelTextByLang(TXT_CC_SENTANDREJECT, &noteStr);
    WatchCOM_NoteWin_FullScreen_Enter(winId, NULL, &noteStr, IAMGE_CALL_NOTIFY_REJECTED, NULL, softKey, PNULL);
}

/*****************************************************************************/
//  Description : mo call error tip window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_MoCallError_NoteWin_Enter(MMI_TEXT_ID_T text_id)
{
    MMI_WIN_ID_T    winId   = WATCHCC_MOCALL_ERROR_WIN_ID;
    MMI_STRING_T    noteStr = {0};
    WATCH_SOFTKEY_TEXT_ID_T softKey = {0};
    GUISTR_STYLE_T  noteStyle = {0};
    GUISTR_STATE_T  state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE;
    noteStyle.align = ALIGN_LEFT;
    noteStyle.font = WATCH_STRING_FONT_MIDDLE;
    MMI_GetLabelTextByLang(text_id, &noteStr);
    if(GUISTR_GetStringWidth(&noteStyle, &noteStr, state) < WATCH_NOTE_LEN_FOR_1LINE) //bug 1321428
    {
        WatchCOM_NoteWin_1Line_Enter(winId, &noteStr, NULL, softKey, NULL);
    }
    else
    {
        WatchCOM_NoteWin_2Line_Enter(winId, &noteStr, NULL, softKey, NULL);
    }
}

/*****************************************************************************/
//  Description : memory is full tip window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_MemoryFull_NoteWin_Enter(void)
{
    MMI_WIN_ID_T    winId   = WATCHCC_MEMORY_FULL_WIN_ID;
    MMI_STRING_T    noteStr = {0};
    WATCH_SOFTKEY_TEXT_ID_T softKey = {0};
    MMI_GetLabelTextByLang(TXT_CC_MEMORYISFULL, &noteStr);
    WatchCOM_NoteWin_1Line_Enter(winId, &noteStr, NULL, softKey, NULL);
}


/*****************************************************************************/
//  Description : Reject Mt call By Sms
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_RejectMtBySms(void)
{
    if(CC_RESULT_SUCCESS == MMICC_ReleaseCurrentCallReq(MN_CAUSE_USER_BUSY))
    {
        s_cc_reject_call_by_sms = TRUE;
    }
}

extern BOOLEAN g_is_inVideo ; //视频通话中

#define PUBLIC_FUNC 1
#if PUBLIC_FUNC
/*****************************************************************************/
//  Description : update call state
//  Parameter: [In] cc state type
//             [Out] None
//             [Return] success or not
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_UpdateCallStatusDisplay(MMICC_UPDATESTATUS_TYPE_E type)
{
    BOOLEAN result = TRUE;

    if(type >= MMICC_UPDATESTATUS_TYPE_MAX)
    {
        return FALSE;
    }

    switch(type)
    {
        case MMICC_MO_START_IND:
        {
            CC_OpenWatchMoCallWin();
            break;
        }
        case MMICC_MT_START_IND:
        {
		#ifdef ZDT_VIDEOCHAT_SUPPORT//xxadd_0117 视频通话和来电冲突
				SCI_TRACE_LOW("11111  MMICC_UpdateCallStatusDisplay  g_is_inVideo=%d",g_is_inVideo);
				if(g_is_inVideo)
				{
					//MMIAPICC_ReleaseCallByRedkey();
					//MMICC_StopRingOrVibrateBeforeCCing();
					Video_Call_Remote_Hangup();
					 YX_Net_Send_Reply_DOWNAPPHANGUP(); 	
				}
		#endif 
					 
				if(YX_DB_TIMER_IsValid_Slient())
				{
					MMIAPICC_ReleaseCallByRedkey();
					MMICC_StopRingOrVibrateBeforeCCing();
					return result;
				}

			
             //拒接陌生电
            if(ZDT_Reject_UnknownCall())
            {
                uint8 telNum[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};    
                uint8 telNumLen = 0;
                MMICC_GetCallNumStrByIndex(telNum, &telNumLen, MMICC_GetCurrentCallIndex());
                if(ZDT_Reject_Call(&telNum))
                {
                    MMIAPICC_ReleaseCallByRedkey();
                    MMICC_StopRingOrVibrateBeforeCCing();
                    s_cc_reject_call_by_unkown_call = TRUE;
                    return result;
                }
            }
            
#ifdef LEBAO_MUSIC_SUPPORT
            lebao_exit();
#endif    
            MMIDEFAULT_TurnOnBackLight();
#ifdef MMI_RECORD_SUPPORT
            WATCHRECORD_StopRecordOrPlay();
#endif
            CC_OpenWatchMtCallWin();
            MMIUSBSHARE_Disconnect();
            break;
        }
        case MMICC_MO_CONNECTED_IND:
        {
            CC_UpdateWatchMoCallStatus();
            WatchCC_MoCallingWin_Exit();
            ZDT_SOS_Call_Success();
            break;
        }
        case MMICC_MT_CONNECTED_IND:
        {
            CC_UpdateWatchMtCallStatus();
            WatchCC_MtCallingWin_Exit();
            break;
        }
        case MMICC_DISCONNECTED_IND:
        {
            CC_UpdateWatchDisconnectedCallStatus();
            break;
        }
#ifdef MMI_VOLTE_SUPPORT
        case MMICC_VOLTE_CALL_IND:
        {
            if(MMK_IsFocusWin(WATCHCC_MOCALLING_WIN_ID)
                || MMK_IsFocusWin(WATCHCC_CONNECTED_WIN_ID)
                || MMK_IsFocusWin(WATCHCC_MTCALLING_WIN_ID))
            {
                MMK_PostMsg(WATCHCC_MOCALLING_WIN_ID, MSG_FULL_PAINT, PNULL,0);
                MMK_PostMsg(WATCHCC_CONNECTED_WIN_ID, MSG_FULL_PAINT, PNULL,0);
                MMK_PostMsg(WATCHCC_MTCALLING_WIN_ID, MSG_FULL_PAINT, PNULL,0);
            }
            break;
        }
#endif
        default:
            break;
    }
    return result;
}

/*****************************************************************************/
//  Description : make call api for watch
//  Parameter: [In] call info
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchAPICC_MakeCall(MN_DUAL_SYS_E dual_sys,
                                       uint8 *tele_num,
                                       uint8  tele_len,
                                       MMI_STRING_T* disp_ptr,
                                       MMI_STRING_T* name_ptr,
                                       CC_CALL_SIM_TYPE_E call_sim_type,
                                       uint32 call_type,
                                       MN_SUBADDR_T* sub_addr
                                       )
{
    if(CC_RESULT_ERROR == MMIAPICC_MakeCall(dual_sys, tele_num, tele_len,
                                        disp_ptr, name_ptr, call_sim_type, call_type, sub_addr))
    {
        MMICC_MO_CALL_ERROR_TYPE_E  errorType = MMICC_GetMoCallErrorType();
        CC_HandleMoCallFailed(errorType);
    }
    else
    {
        //make call success, do nothing
    }
}

#endif

/*****************************************************************************/
//  Description : handle call failed
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CC_HandleMoCallFailed(MMICC_MO_CALL_ERROR_TYPE_E type)
{
    BOOLEAN result = TRUE;

    if(type >= CC_MO_ERROR_TYPE_MAX)
    {
        return FALSE;
    }

    switch(type)
    {
        case CC_MO_DIAL_FAILED:
        {
            WatchCC_MoCallError_NoteWin_Enter(TXT_DIAL_FAILED);
            break;
        }
        case CC_MO_EMPTY_NUM:
        {
            WatchCC_MoCallError_NoteWin_Enter(TXT_CL_EMPTY_NUM);
            break;
        }
        case CC_MO_IN_FLY_MODE:
        {
            WatchCC_MoCallError_NoteWin_Enter(TXT_CLOSE_FLY_MODE_FIRST);
            break;
        }
        case CC_MO_IS_BARRING_NUM:
        {
            WatchCC_MoCallError_NoteWin_Enter(TXT_CC_CANNOT_CALL);
            break;
        }
        case CC_MO_NET_UNAVAIL:
        {
            WatchCC_MoCallError_NoteWin_Enter(TXT_CC_NET_UNAVAIL);
            break;
        }
        case CC_MO_INVALID_NUM:
        {
            WatchCC_MoCallError_NoteWin_Enter(TXT_CC_INVALID_NUM);
            break;
        }
        case CC_MO_NO_SIM:
        {
            WatchCC_MoCallError_NoteWin_Enter(TXT_CC_INSERTSIMCARD);
            break;
        }
        case CC_MO_FDN_ONLY:
        {
            WatchCC_MoCallError_NoteWin_Enter(TXT_CC_FDN_ONLY);
            break;
        }
        case CC_MO_NO_IP_NUM:
        {
            WatchCC_MoCallError_NoteWin_Enter(TXT_CC_NO_IP_NUM);
            break;
        }
		case CC_MO_CMD_NO_SUPPORT:
		{
            WatchCC_MoCallError_NoteWin_Enter(TXT_COMMON_CMD_NO_SUPPORT);
            break;
		}
        case CC_MO_NO_VOLTE_CALL:
        {
            WatchCC_MoCallError_NoteWin_Enter(TXT_CC_NO_VOLTE_CALL);
            break;
        }
        case CC_MO_ECC_CALL:
        {
            WatchCC_MoCallError_NoteWin_Enter(TXT_CC_ECC_CALL);
            break;
        }
        default:
        {
            WatchCC_MoCallError_NoteWin_Enter(TXT_CC_OPER_FAIL);
            break;
        }
    }
    return result;
}

/*****************************************************************************/
//  Description : convert time count to str
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void  CC_TimeCountToStr(MMI_STRING_T* time_str_ptr, uint32 time_count)
{
    uint8   timeStr[CC_DISPLAY_TIME_LEN + 2] = {0};

    if (PNULL != time_str_ptr && PNULL != time_str_ptr->wstr_ptr)
    {
        sprintf((char*)timeStr,
            "%02d:%02d:%02d",
            (int)(time_count / CC_HOUR_TO_SECOND),
            (int)((time_count % CC_HOUR_TO_SECOND) / CC_MINUTE_TO_SECOND),
            (int)(time_count % CC_MINUTE_TO_SECOND)
            );

        time_str_ptr->wstr_len = MIN(time_str_ptr->wstr_len, strlen((char*)timeStr));
        //prompt_str.wstr_ptr = time_str_arr;
        MMI_STRNTOWSTR(time_str_ptr->wstr_ptr,
            time_str_ptr->wstr_len,
            (uint8 *)timeStr,
            time_str_ptr->wstr_len,
            time_str_ptr->wstr_len);
    }
}

/*****************************************************************************/
//  Description : open the window when mo call
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void  CC_OpenWatchMoCallWin(void)
{
    uint8                   telNum[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
    uint8                   telNumLen = 0;
    wchar                   dispWchar[MMICC_PHONE_NUM_MAX_LEN + 2]  ={0};
    uint8                   dispImageFlag = 0;
    MMICC_DISPINFO_T        dispInfo = {0};
    MMICC_DISPINFO_T        *addData = NULL;
    addData = (MMICC_DISPINFO_T *)SCI_ALLOC_APPZ(sizeof(MMICC_DISPINFO_T));
    SCI_ASSERT(addData != NULL);

    if(PNULL == MMICC_GetAppletInstance())
    {
        MMICC_StartCCApplet();
    }

    MMICC_GetWantCallNumStr(telNum, &telNumLen);

#ifdef _SW_ZDT_PRODUCT_
    dispInfo.incall_portrait = ZDT_PB_GetDispInfoContent((uint8 *)telNum, telNumLen, &dispWchar, FALSE);
#else
    dispImageFlag = WatchPB_GetDispInfoContent((uint8 *)telNum, telNumLen, &dispWchar, FALSE);
    dispInfo.incall_portrait = WatchPB_GetImageIdByFlag(dispImageFlag, TRUE);
#endif
    dispInfo.disp_name_len = MMIAPICOM_Wstrlen(dispWchar);
    MMI_WSTRNCPY(dispInfo.disp_name_buf, MMICC_PHONE_NUM_MAX_LEN + 2,(const wchar*)dispWchar, dispInfo.disp_name_len, dispInfo.disp_name_len);

    *addData = dispInfo;
    WatchCC_MoCallingWin_Enter(addData);
    //close qurey win
    WatchCC_MoCallQueryWin_Exit();
}

/*****************************************************************************/
//  Description : open the window when mt call
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CC_OpenWatchMtCallWin(void)
{
    uint8                   telNum[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
    uint                    telNumLen = 0;
    wchar                   dispWchar[MMICC_PHONE_NUM_MAX_LEN + 2]  ={0};
    uint8                   dispImageFlag = 0;
    MMICC_DISPINFO_T        dispInfo = {0};
    MMICC_DISPINFO_T        *addData = NULL;
#ifdef BLUETOOTH_SUPPORT
    MMI_STRING_T            btMsg = {0};
#endif
    addData = (MMICC_DISPINFO_T *)SCI_ALLOC_APPZ(sizeof(MMICC_DISPINFO_T));
    SCI_ASSERT(addData != NULL);

    //协议规定不能同时存在两个MT,如果之前有MO,MO没有接通时会把MO挂断，接通了不会提示MT
    if (MMK_IsOpenWin(WATCHCC_MOCALLING_WIN_ID))
    {
        MMK_CloseWin(WATCHCC_MOCALLING_WIN_ID);
    }

    if(PNULL == MMICC_GetAppletInstance())
    {
        MMICC_StartCCApplet();
    }

    MMICC_GetCallNumStrByIndex(telNum, &telNumLen, MMICC_GetCurrentCallIndex());
    //get info for display
    dispInfo.tel_num_len = telNumLen;
#ifdef _SW_ZDT_PRODUCT_
    dispInfo.incall_portrait = ZDT_PB_GetDispInfoContent((uint8 *)telNum, telNumLen, &dispWchar, FALSE);
#else
    dispImageFlag = WatchPB_GetDispInfoContent((uint8 *)telNum, telNumLen, &dispWchar, FALSE);
    dispInfo.incall_portrait = WatchPB_GetImageIdByFlag(dispImageFlag, TRUE);
#endif

    dispInfo.disp_name_len = MMIAPICOM_Wstrlen(dispWchar);
    MMI_WSTRNCPY(dispInfo.disp_name_buf, MMICC_PHONE_NUM_MAX_LEN + 2,(const wchar*)dispWchar, dispInfo.disp_name_len, dispInfo.disp_name_len);

    *addData = dispInfo;
    WatchCC_MtCallingWin_Enter(addData);


#ifdef BLUETOOTH_SUPPORT
        btMsg.wstr_ptr = dispWchar;
        btMsg.wstr_len = MMIAPICOM_Wstrlen(dispWchar);
        MMIAPIBT_IncomingCall(btMsg);
#endif
}

/*****************************************************************************/
//  Description : dtmf note win
//  Author: feiyue.ji
//  Note:2020.7.23
/*****************************************************************************/
LOCAL void CC_OpenSendDtmfPromptWin( void )
{
    MMI_WIN_ID_T    alert_win_id = WATCHCC_SEND_DTMF_NOTE_WIN_ID;
    MMI_WIN_ID_T    dtmf_query_win_id = WATCHCC_SEND_DTMF_QUERY_WIN_ID;
    MMI_STRING_T    dtmf_str = {0};
    wchar           temp_alert_wchar[CC_MAX_TELE_NUM_LEN + 1] = {0};
    wchar           temp_wchar[100] = {0};
    MMI_STRING_T    promt_string = {0};
    MMI_STRING_T    promt_str = {0};
    MMI_STRING_T    button_left_str = {0};
    MMI_STRING_T    button_right_str = {0};
    WATCH_SOFTKEY_TEXT_ID_T softkey_text = {TXT_SEND, TXT_NULL, STXT_CANCEL};
    wchar alt[2] = {'\n','\0'};

    CC_DTMF_STATE_T dtmf_info = *MMICC_GetStoredDtmfInfo();
    dtmf_str.wstr_ptr = temp_alert_wchar;
    dtmf_str.wstr_len = dtmf_info.dtmf_len - 1;
    MMI_STRNTOWSTR(dtmf_str.wstr_ptr, dtmf_str.wstr_len, (uint8 *)&(dtmf_info.dtmf_str[1]), dtmf_str.wstr_len, dtmf_str.wstr_len);
    promt_string.wstr_ptr = temp_wchar;

    if (('W' == dtmf_info.dtmf_str[0] ||  'w' == dtmf_info.dtmf_str[0])&& dtmf_info.dtmf_len != 1)
    {
        MMI_GetLabelTextByLang(TXT_CC_SEND_DTMF, &promt_str);
        MMI_GetLabelTextByLang(TXT_SEND, &button_left_str);
        MMI_GetLabelTextByLang(STXT_RETURN, &button_right_str);
        MMIAPICOM_Wstrncat(promt_string.wstr_ptr, promt_str.wstr_ptr, promt_str.wstr_len);// fix bug1374557
        //由于MMIAPICOM_Wstrncat的第二个入参为const类型，因此在传参时需要注意参数类型 ，直接传入空格" "，会造成指针越界，且强转类型不可行
        MMIAPICOM_Wstrncat(promt_string.wstr_ptr, alt, 1);
        MMIAPICOM_Wstrncat(promt_string.wstr_ptr, dtmf_str.wstr_ptr, dtmf_str.wstr_len);
        promt_string.wstr_len = MIN(dtmf_str.wstr_len+promt_str.wstr_len+1, 100);

        if (MMK_IsOpenWin(dtmf_query_win_id))
        {
            MMK_CloseWin(dtmf_query_win_id);
        }
        WatchCOM_QueryWin_2Btn_Enter(dtmf_query_win_id, &promt_string, &button_left_str, &button_right_str, IMAGE_NULL, IMAGE_NULL, softkey_text, HandleSendDTMFQueryWinMsg);
    }
    else
    {
        softkey_text.softkeyLeft =TXT_NULL;
        softkey_text.softkeyMiddle =TXT_NULL;
        softkey_text.softkeyRight =TXT_NULL;
        MMI_GetLabelTextByLang(TXT_CC_SENDING_DTMF, &promt_str);
        MMIAPICOM_Wstrncat(promt_string.wstr_ptr, promt_str.wstr_ptr, promt_str.wstr_len);// fix bug1374557
        MMIAPICOM_Wstrncat(promt_string.wstr_ptr, alt, 1);
        MMIAPICOM_Wstrncat(promt_string.wstr_ptr, dtmf_str.wstr_ptr, dtmf_str.wstr_len);
        promt_string.wstr_len = MIN(dtmf_str.wstr_len+promt_str.wstr_len+1, 100);

        if (MMK_IsOpenWin(alert_win_id))
        {
            MMK_CloseWin(alert_win_id);
        }
        WatchCOM_NoteWin_2Line_Enter(alert_win_id, &promt_string, NULL, softkey_text,  HandleSendingDTMFNoteWinMsg);
    }
}

/*****************************************************************************/
//  Description : update mo call status(button and time)
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void  CC_UpdateWatchMoCallStatus(void)
{
    MMI_WIN_ID_T            winId = WATCHCC_CONNECTED_WIN_ID;
    uint8                   telNum[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
    uint8                   telNumLen = 0;
    wchar                   dispWchar[MMICC_PHONE_NUM_MAX_LEN + 2]  ={0};
    uint8                   dispImageFlag = 0;
    MMICC_DISPINFO_T        dispInfo = {0};
    MMICC_DISPINFO_T        *addData = NULL;
    addData = (MMICC_DISPINFO_T *)SCI_ALLOC_APPZ(sizeof(MMICC_DISPINFO_T));
    SCI_ASSERT(addData != NULL);

    MMICC_GetCallNumStrByIndex(telNum, &telNumLen, MMICC_GetCurrentCallIndex());
    //get info for display
#ifdef _SW_ZDT_PRODUCT_
    dispInfo.incall_portrait = ZDT_PB_GetDispInfoContent((uint8 *)telNum, telNumLen, &dispWchar, FALSE);
#else
    dispImageFlag = WatchPB_GetDispInfoContent((uint8 *)telNum, telNumLen, &dispWchar, FALSE);
    dispInfo.incall_portrait = WatchPB_GetImageIdByFlag(dispImageFlag, TRUE);
#endif
    dispInfo.disp_name_len = MMIAPICOM_Wstrlen(dispWchar);
    MMI_WSTRNCPY(dispInfo.disp_name_buf, MMICC_PHONE_NUM_MAX_LEN + 2,(const wchar*)dispWchar, dispInfo.disp_name_len, dispInfo.disp_name_len);

    *addData = dispInfo;
    if(!MMK_IsOpenWin(winId) )
    {
        WatchCC_ConnectedWin_Enter(addData);
    }
}

/*****************************************************************************/
//  Description : update mt call status(button and time)
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void  CC_UpdateWatchMtCallStatus(void)
{
    MMI_WIN_ID_T    winId = WATCHCC_CONNECTED_WIN_ID;
    uint8           telNum[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
    uint8           telNumLen = 0;
    wchar                   dispWchar[MMICC_PHONE_NUM_MAX_LEN + 2]  ={0};
    uint8                   dispImageFlag = 0;
    MMICC_DISPINFO_T        dispInfo = {0};
    MMICC_DISPINFO_T        *addData = NULL;
    addData = (MMICC_DISPINFO_T *)SCI_ALLOC_APPZ(sizeof(MMICC_DISPINFO_T));
    SCI_ASSERT(addData != NULL);

    if(!MMICC_IsCurrentCallIDValid())
    {
        return;
    }

    MMICC_GetCallNumStrByIndex(telNum, &telNumLen, MMICC_GetCurrentCallIndex());
    //get info for display
#ifdef _SW_ZDT_PRODUCT_
    dispInfo.incall_portrait = ZDT_PB_GetDispInfoContent((uint8 *)telNum, telNumLen, &dispWchar, FALSE);
#else
    dispImageFlag = WatchPB_GetDispInfoContent((uint8 *)telNum, telNumLen, &dispWchar, FALSE);
    dispInfo.incall_portrait = WatchPB_GetImageIdByFlag(dispImageFlag, TRUE);
#endif
    dispInfo.disp_name_len = MMIAPICOM_Wstrlen(dispWchar);
    MMI_WSTRNCPY(dispInfo.disp_name_buf, MMICC_PHONE_NUM_MAX_LEN + 2,(const wchar*)dispWchar, dispInfo.disp_name_len, dispInfo.disp_name_len);

    *addData = dispInfo;
    if(!MMK_IsOpenWin(winId) )
    {
        WatchCC_ConnectedWin_Enter(addData);
    }
}

/*****************************************************************************/
//  Description : update disconnected call status
//  Global resource dependence : none
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
LOCAL void  CC_UpdateWatchDisconnectedCallStatus(void)
{
    SCI_TRACE_LOW("[WatchCC] MMICC_UpdateCallStatusDisplay, call_num = %d", MMICC_GetCallNum());
#ifdef MMI_RECORD_SUPPORT
    if(!MMICC_IsRecording())
#endif
    {
        if((MMICL_CALL_MISSED == MMICC_GetCallLogType()) || (0 < MMICC_GetCallNum()))
        {
            //来电时非主动挂断,挂断后不弹提示窗
            SCI_TRACE_LOW("[WatchCC] MMICC_UpdateCallStatusDisplay, No notewin when MT is not hangup by self");
            //WatchCC_CallEnded_NoteWin_Enter();
        }
        else//主动挂断来电
        {
            SCI_TRACE_LOW("[WatchCC] MMICC_UpdateCallStatusDisplay s_cc_reject_call_by_sms = %d", s_cc_reject_call_by_sms);
            if(s_cc_reject_call_by_sms)
            {
                WatchCC_SentAndRejected_NoteWin_Enter();
                s_cc_reject_call_by_sms = FALSE;
            }
            else
            {
                if(MMIZDT_ShouldStayInClassModeWin())
                {
                    //上课禁用中自动挂断电话就不显示通话结束的窗口了
                }
                else if(s_cc_reject_call_by_unkown_call)
                {
                    s_cc_reject_call_by_unkown_call = FALSE;
                    //拒接模生电话中自动挂断电话就不显示通话结束的窗口了
                }
                else
                {
                    WatchCC_CallEnded_NoteWin_Enter();
                }

            }
        }
    }
#ifdef MMI_RECORD_SUPPORT
    else
    {
        MMICC_StopRecordInCall();
        WatchCC_EndedAndSaved_NoteWin_Enter();
    }
#endif

    if (FALSE == MMIAPICC_IsInState(CC_MO_CONNECTING_STATE))
    {
        WatchCC_MoCallingWin_Exit();
        ZDT_SOS_Call_Fail();
    }
    if (FALSE == MMIAPICC_IsInState(CC_MT_CONNECTING_STATE))
    {
        WatchCC_MtCallingWin_Exit();
    }
    if (FALSE == MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE))
    {
        WatchCC_ConnectedWin_Exit();
        MMIZDT_Call_SOS_Over();
    }
#ifdef MMI_ETWS_SUPPORT
    if( (TRUE == MMIAPISMSCB_GetEMCallStatus()) || (TRUE == MMIAPISMSCB_GetIsNeedRedisplay()) )
    {
        MMIAPISMSCB_ShowNewETWSPrompt();
    }
#endif
}

LOCAL void CC_MoCall_DrawHeadPhotoAndName(MMI_WIN_ID_T win_id, MMI_IMAGE_ID_T head_photo, MMI_STRING_T str_data)
{
    GUI_RECT_T          nameDispRect   = WATCH_MO_NAME_DISP_RECT;
    GUISTR_STYLE_T      nameStyleInfo  = {0};
    GUISTR_STATE_T      state       = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK;
    GUI_LCD_DEV_INFO    devInfo     = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_POINT_T         start_point = WATCH_MO_PHOTO_START_POINT;
    uint16 img_width = 0;
    uint16 img_height = 0;

    if(GUIRES_GetImgWidthHeight(&img_width, &img_height, head_photo, win_id))
    {
        start_point.x = (MMI_MAINSCREEN_WIDTH-img_width)/2;
    }

    GUIRES_DisplayImg(&start_point, PNULL, PNULL, win_id, head_photo, &devInfo);

    nameStyleInfo.align    = ALIGN_HMIDDLE;
    nameStyleInfo.font     = WATCH_STRING_FONT_MIDDLE;
    nameStyleInfo.font_color = MMI_WHITE_COLOR ;
    GUISTR_DrawTextToLCDInRect(&devInfo, &nameDispRect, &nameDispRect, &str_data, &nameStyleInfo, state, GUISTR_TEXT_DIR_AUTO);
}

LOCAL void CC_MtCall_DrawHeadPhotoAndName(MMI_WIN_ID_T win_id, MMI_IMAGE_ID_T head_photo, MMI_STRING_T str_data)
{
    GUI_RECT_T          nameDispRect   = WATCH_MT_NAME_DISP_RECT;
    GUISTR_STYLE_T      nameStyleInfo  = {0};
    GUISTR_STATE_T      state       = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK;
    GUI_LCD_DEV_INFO    devInfo     = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_POINT_T         start_point = WATCH_MT_PHOTO_START_POINT;
    uint16 img_width = 0;
    uint16 img_height = 0;
    if(GUIRES_GetImgWidthHeight(&img_width, &img_height, head_photo, win_id))
    {
        start_point.x = (MMI_MAINSCREEN_WIDTH-img_width)/2;
    }
    GUIRES_DisplayImg(&start_point, PNULL, PNULL, win_id, head_photo, &devInfo);

    nameStyleInfo.align    = ALIGN_HMIDDLE;
    nameStyleInfo.font     = WATCH_STRING_FONT_MIDDLE;
    nameStyleInfo.font_color =MMI_WHITE_COLOR ;
    GUISTR_DrawTextToLCDInRect(&devInfo, &nameDispRect, &nameDispRect, &str_data, &nameStyleInfo, state, GUISTR_TEXT_DIR_AUTO);
}

#ifdef MMI_VOLTE_SUPPORT
LOCAL void CC_DrawVolteIcon(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO    devInfo = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_POINT_T         startPoint = WATCH_VOLTE_ICON_START_POINT;
    MMI_IMAGE_ID_T      imageId = IMAGE_NULL;
    MN_DUAL_SYS_E       dualSys = MN_DUAL_SYS_1;

    SCI_TRACE_LOW("volte %s, volte status =%d", __FUNCTION__,MMICC_IsVolteNetworkStatus());
    if(MMICC_IsVolteNetworkStatus())
    {
        if(MMIAPIPHONE_GetVolteIconDisplayStyle(dualSys) == MMIPHONE_VOLTE_ICON_HD)
        {
            imageId = IMAGE_CALL_STATUS_HD;
        }
        else
        {
            imageId = IMAGE_CALL_STATUS_VOLTE;
        }
        GUIRES_DisplayImg(&startPoint, PNULL, PNULL, win_id, imageId, &devInfo);
    }
}
#endif

/*****************************************************************************/
//  Description : play new call ring
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  CC_PlayNewCallRing()
{
    MN_DUAL_SYS_E   dualSys = MN_DUAL_SYS_1;
    //play ring or vibrate
    if (MMICC_IsExistActiveCall()||MMICC_IsExistHoldCall() || MMICC_IsExistOperatedCall())
    {
        MMIAPISET_PlayRing(dualSys, FALSE,0,1,MMISET_RING_TYPE_CALL_IN_CALL,PNULL);
        if (0 == s_cc_waiting_call_ring_timer)
        {
            s_cc_waiting_call_ring_timer = MMK_CreateWinTimer(WATCHCC_MTCALLING_WIN_ID, CC_WATCH_WAITING_CALL_PERIOD, FALSE);
        }
    }
    else
    {
        MMIAPICC_StartRingOrVibrate(FALSE);
        MMICC_SetMtPresetCodecRoute(0);
    }
    return TRUE;
}

#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
//  Description : 开始录音或停止录音
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CC_HandleRecordButton(MMI_WIN_ID_T win_id)
{
    MMIRECORD_SRV_RESULT_E errorCode = MMIRECORD_SRV_RESULT_SUCCESS;

    if (MMICC_IsRecording())
    {
        //停止录音
        MMICC_StopRecordInCall();
        GUIBUTTON_SetVisible(WATCHCC_RECORDON_BUTTON_CTRL_ID, FALSE, FALSE);
        GUIBUTTON_SetVisible(WATCHCC_RECORDOFF_BUTTON_CTRL_ID, TRUE, FALSE);
        GUILABEL_SetVisible(WATCHCC_CALLRECORD_LABEL_CTRL_ID, FALSE, FALSE);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
        WatchCC_RecordSaved_NoteWin_Enter();
	}
	else
	{
        //开始录音
        errorCode = MMICC_StartRecordInCall();
        if(MMIRECORD_SRV_RESULT_SUCCESS != errorCode)
        {
            MMICC_StopRecordInCall();
        }
        switch (errorCode)
        {
            case MMIRECORD_SRV_RESULT_SUCCESS:
            {
                GUIBUTTON_SetVisible(WATCHCC_RECORDOFF_BUTTON_CTRL_ID, FALSE, FALSE);
                GUIBUTTON_SetVisible(WATCHCC_RECORDON_BUTTON_CTRL_ID, TRUE, FALSE);
                GUILABEL_SetVisible(WATCHCC_CALLRECORD_LABEL_CTRL_ID, TRUE, FALSE);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
                break;
            }
            case MMIRECORD_SRV_RESULT_NO_SPACE:
            {
                //内存不足
                WatchCC_MemoryFull_NoteWin_Enter();
                break;
            }
            case MMIRECORD_SRV_RESULT_FOLDER_FULL:
                break;
            default:
                break;
        }
    }
}
#endif

/*****************************************************************************/
//  Description :to answer a incoming call automatically, assoicated with eng
//  Global resource dependence : none
//  Author:yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CC_AutoAnswerIncomingCall(void)
{
    if(MMIAPIENG_GetAutoAnswerCallSet()
        &&(CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus()))
    {
        SCI_TRACE_LOW("[WatchCC] CC_AutoAnswerIncomingCall");
        //MMK_PostMsg(WATCHCC_MTCALLING_WIN_ID, MSG_APP_CANCEL, PNULL, 0);
        MMICC_AnswerCall();
    }
}
#if 0
/*****************************************************************************/
//  Description : CC_RejectMtBySms_CallBack
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CC_RejectMtBySms_CallBack(SMSSRV_CALLBACK_STRUCT_T* callback_data)
{
    SCI_TRACE_LOW("CC_RejectMtBySms_CallBack Enter");
    if(PNULL != callback_data)
    {
        SCI_TRACE_LOW("CC_RejectMtBySms_CallBack result = %d", callback_data->result);
        if(callback_data->result)
        {
            if(CC_RESULT_SUCCESS == MMICC_ReleaseCurrentCallReq(MN_CAUSE_USER_BUSY))
            {
                s_cc_reject_call_by_sms = TRUE;
            }
        }
    }
}
#endif
#if 0
/*****************************************************************************/
//description :
//lobal resource dependence :
//Author: feiyue.ji
//date:2020.07.21
/*****************************************************************************/
PUBLIC void  CC_StartDtmfWaitingTimer(void)
{
    if (MMI_NONE_TIMER_ID != CC_INSTANCE.dtmf_timer_id)
    {
        MMK_StopTimer(CC_INSTANCE.dtmf_sending_timer);
    }
    CC_INSTANCE.dtmf_timer_id = MMK_CreateWinTimer(WATCHCC_CONNECTED_WIN_ID, CC_WATCH_DTMF_WAITING_TIME, FALSE);// fix bug1374595
}
#endif

/*****************************************************************************/
//	Description : send DTMF when a call come into active state
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_SendStoredDtmf(void)
{
    uint32          i = 0;
    uint32          dtmf_len = 0;
    uint32          remain_len = 0;
    uint8           copy_len = 0;
    uint8           dtmf_str[CC_MAX_TELE_NUM_LEN+1] = {0};
    BOOLEAN         is_end = FALSE;
    MMI_STRING_T    alert_str = {0};
    wchar           temp_alert_str[CC_MAX_TELE_NUM_LEN + 1] = {0};
    CC_RESULT_E     is_dtmf = CC_RESULT_SUCCESS;
    BOOLEAN         is_valid_dtmf = FALSE;
    CC_DTMF_STATE_T* dtmf_info_ptr = MMICC_GetStoredDtmfInfo();

    //check the param
    if( !MMICC_IsCurrentCallIDValid())
    {
        return CC_RESULT_ERROR;
    }
    alert_str.wstr_ptr = temp_alert_str;
    MMI_MEMCPY(
        (char*)dtmf_info_ptr->dtmf_str,
        dtmf_info_ptr->dtmf_len,
        &(dtmf_info_ptr->dtmf_str[1]),
        (dtmf_info_ptr->dtmf_len - 1),
        (dtmf_info_ptr->dtmf_len - 1)
        );
    dtmf_info_ptr->dtmf_len--;
    SCI_MEMSET((char*)(&dtmf_info_ptr->dtmf_str[dtmf_info_ptr->dtmf_len]),0, 1);

    //if the extension number exist
    if(dtmf_info_ptr->is_dtmf)
    {
        //从有效的DTMF位置开始检查dtmf字符串
        for(i = 0; i < dtmf_info_ptr->dtmf_len; i++)
        {
            is_valid_dtmf = ('P' == dtmf_info_ptr->dtmf_str[i] || 'W' == dtmf_info_ptr->dtmf_str[i] ||
                'p' == dtmf_info_ptr->dtmf_str[i] || 'w' == dtmf_info_ptr->dtmf_str[i]);

            if(is_valid_dtmf)
            {
                dtmf_len = i;
                if(dtmf_len > 0)
                {
                    dtmf_info_ptr->is_dtmf = TRUE;
                    copy_len = ( dtmf_len > CC_MAX_TELE_NUM_LEN ) ? CC_MAX_TELE_NUM_LEN : dtmf_len;

                    MMI_MEMCPY(
                        (char*)dtmf_str,
                        dtmf_len,
                        dtmf_info_ptr->dtmf_str,
                        dtmf_info_ptr->dtmf_len,
                        copy_len
                        );

                    //发送有效的DTMF
                    MMICC_SendDTMFStr(dtmf_str, dtmf_len);
                }
                is_end = FALSE;

                break;
            }
        }

        //如果字符串全部监测完毕没有发现w,p则发送后面所有的DTMF
        if (i >= dtmf_info_ptr->dtmf_len)
        {
            is_end = TRUE;
        }

        //如果字符串中不再包含p,w,n则直接呼出
        if (is_end)
        {
            //alert_str.wstr_ptr = s_call_context.dtmf_state.dtmf_str;
            MMI_STRNTOWSTR(alert_str.wstr_ptr,
                dtmf_info_ptr->dtmf_len,
                (uint8 *)dtmf_info_ptr->dtmf_str,
                dtmf_info_ptr->dtmf_len,
                dtmf_info_ptr->dtmf_len);
            alert_str.wstr_len = dtmf_info_ptr->dtmf_len;

            //DTMF字符串第一个元素为字母需要去掉字母发送
            MMICC_SendDTMFStr(dtmf_info_ptr->dtmf_str, dtmf_info_ptr->dtmf_len);

            //clear dtmf infomation in s_call_context
            MMICC_ClearStoredDtmf();

            MMK_CloseWin(WATCHCC_SEND_DTMF_NOTE_WIN_ID);
            MMK_CloseWin(WATCHCC_SEND_DTMF_QUERY_WIN_ID);
        }
        else
        {
            //获取下一个有效的DTMF字符串，去掉已经发送的DTMF和第一个字符p或w
            dtmf_info_ptr->dtmf_len = dtmf_info_ptr->dtmf_len - copy_len;
            remain_len = ( dtmf_info_ptr->dtmf_len > CC_MAX_TELE_NUM_LEN ) ? CC_MAX_TELE_NUM_LEN : dtmf_info_ptr->dtmf_len;

            if(copy_len < CC_MAX_TELE_NUM_LEN)
            {
                MMI_MEMCPY(
                    (char*)dtmf_str,
                    remain_len,
                    (dtmf_info_ptr->dtmf_str + copy_len),
                    dtmf_info_ptr->dtmf_len,
                    remain_len
                );
            }

            //去除DTMF已经发送的部分，重新设置dtmf字符串
            dtmf_info_ptr->dtmf_len = remain_len;
            SCI_MEMSET(dtmf_info_ptr->dtmf_str, NULL, sizeof(dtmf_info_ptr->dtmf_str));
            MMI_MEMCPY(
                (char*)dtmf_info_ptr->dtmf_str,
                sizeof(dtmf_info_ptr->dtmf_str),
                dtmf_str,
                remain_len,
                remain_len
                );

            //判断是否已经只有字母存在，如果是则DTMF已经无效
            is_dtmf = MMICC_IsDTMF(dtmf_info_ptr->dtmf_str, dtmf_info_ptr->dtmf_len);
            if (is_dtmf)
            {
                return CC_RESULT_SUCCESS;
            }
            CC_OpenSendDtmfPromptWin();
        }
    }
    return CC_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Discription: draw query win
//  Global resource dependence:
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void DrawMoCallQueryWin(MMIPB_CONTACT_T *contact_info)
{
    GUI_RECT_T          dispRect   = WATCH_MO_QUERY_STR_DISP_RECT;
    GUI_LCD_DEV_INFO    devInfo    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_STRING_T        strData    = {0};
    GUISTR_STYLE_T      styleInfo  = {0};
    GUISTR_STATE_T      state      = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK;
    wchar               dstBuf[MMICC_PHONE_NUM_MAX_LEN + 2 + 6] = {0}; //"Call %U?", LEN+6
    strData.wstr_ptr = dstBuf;

    if(NULL != contact_info)
    {
        styleInfo.align    = ALIGN_HVMIDDLE;
        styleInfo.font     = WATCH_STRING_FONT_BIG;
        styleInfo.font_color = MMI_WHITE_COLOR ;
        MMIAPICOM_PrimoGenerateReplaceStrByTextId(TXT_CC_CALL,
                                                  L"%U",
                                                  &(contact_info->name.wstr),
                                                  MMICC_PHONE_NUM_MAX_LEN + 2 + 6,
                                                  &strData);
        GUISTR_DrawTextToLCDInRect(&devInfo, &dispRect, &dispRect, &strData, &styleInfo, state, GUISTR_TEXT_DIR_AUTO);
        WatchCOM_Btn_SetPos_2Btn(WATCHCC_BACK_BUTTON_CTRL_ID, WATCHCC_DIAL_BUTTON_CTRL_ID);
    }
}

/*****************************************************************************/
//  Discription: draw mo calling win
//  Global resource dependence:
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void DrawMoCallingWin(MMICC_DISPINFO_T *contact_info)
{
    MMI_WIN_ID_T        winId       = WATCHCC_MOCALLING_WIN_ID;
    MMI_CTRL_ID_T       labelCtrlId = WATCHCC_CALLSTATE_LABEL_CTRL_ID;
    MMI_STRING_T        stateStr    = {0};
    MMI_STRING_T        dispStr     = {0};
    GUI_RECT_T          stateRect   = WATCH_CALL_STATE_DISP_RECT;

    if(NULL != contact_info)
    {
        // 显示头像和名称
        dispStr.wstr_ptr = &(contact_info->disp_name_buf);
		dispStr.wstr_len= contact_info->disp_name_len;
        CC_MoCall_DrawHeadPhotoAndName(winId, contact_info->incall_portrait, dispStr);
        // 显示呼叫状态
        MMI_GetLabelTextByLang(TXT_CC_CALLING, &stateStr);
        GUILABEL_SetRect(labelCtrlId, &stateRect, FALSE);
        GUILABEL_SetText(labelCtrlId, &stateStr, FALSE);
        GUILABEL_SetFont(labelCtrlId, WATCH_STRING_FONT_SMALL, MMI_WHITE_COLOR);
        // 显示图片
        WatchCOM_Btn_SetPos_1Btn(WATCHCC_HANGUP_BUTTON_CTRL_ID);
    }
}


/*****************************************************************************/
//  Discription: draw call connected win
//  Global resource dependence:
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void DrawVoiceCallConnectedWin(MMICC_DISPINFO_T *contact_info)
{
    MMI_WIN_ID_T        winId = WATCHCC_CONNECTED_WIN_ID;
#ifdef CALL_RECORD_SUPPORT
    MMI_STRING_T        recordStr = {0};
    GUI_RECT_T          recordRect = WATCH_RECORD_STATE_DISP_RECT;
    MMI_CTRL_ID_T       recordLabelId = WATCHCC_CALLRECORD_LABEL_CTRL_ID;
#endif
    MMI_STRING_T        dispStr    = {0};

    if(NULL != contact_info)
    {
        // 显示头像和名称
        dispStr.wstr_ptr = &(contact_info->disp_name_buf);
		dispStr.wstr_len= contact_info->disp_name_len;
        MMI_WSTRNCPY(dispStr.wstr_ptr, MMICC_PHONE_NUM_MAX_LEN + 2, (const wchar*)contact_info->disp_name_buf, dispStr.wstr_len, dispStr.wstr_len);
        CC_MoCall_DrawHeadPhotoAndName(winId, contact_info->incall_portrait, dispStr);
        // 显示BUTTON图片
#ifdef CALL_RECORD_SUPPORT
        WatchCOM_Btn_SetPos_2Btn(WATCHCC_RECORDOFF_BUTTON_CTRL_ID,WATCHCC_HANGUP_BUTTON_CTRL_ID);
        WatchCOM_Btn_SetPos_2Btn(WATCHCC_RECORDON_BUTTON_CTRL_ID,WATCHCC_HANGUP_BUTTON_CTRL_ID);
#else
        WatchCOM_Btn_SetPos_1Btn(WATCHCC_HANGUP_BUTTON_CTRL_ID);
#endif
    }
    //显示录音状态
#ifdef CALL_RECORD_SUPPORT
    MMI_GetLabelTextByLang(TXT_CC_RECORDING, &recordStr);
    GUILABEL_SetRect(recordLabelId, &recordRect, FALSE);
    GUILABEL_SetText(recordLabelId, &recordStr, TRUE);
    GUILABEL_SetFont(recordLabelId, WATCH_STRING_FONT_SMALL, MMI_WHITE_COLOR);
    if(MMICC_IsRecording()) //bug1555926
    {
        GUILABEL_SetVisible(recordLabelId, TRUE, FALSE);
    }
    else
    {
        GUILABEL_SetVisible(recordLabelId, FALSE, FALSE);
    }
#endif
}

/*****************************************************************************/
//绘制短信拒接
/*****************************************************************************/
LOCAL void DrawMtCallingSmsReject()
{
    MMI_WIN_ID_T        winId      = WATCHCC_MTCALLING_WIN_ID;
    GUI_LCD_DEV_INFO    devInfo     = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUISTR_STATE_T      state       = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK;
    GUI_RECT_T          smsRejectRect = WATCH_SMS_REJECT_DISP_RECT;
    GUISTR_STYLE_T      styleInfo  = {0};
    MMI_STRING_T        strData    = {0};
#ifdef SCREEN_SHAPE_CIRCULAR
    GUI_POINT_T         StartPoint = WATCH_PULL_DOWN_START_POINT;
    GUI_RECT_T          callingTextRect = WATCH_CALLING_TEXT_DISP_RECT;
#else
    GUI_POINT_T         leftStartPoint = WATCH_PULL_DOWN_START_POINT_LEFT;
    GUI_POINT_T         rightStartPoint = WATCH_PULL_DOWN_START_POINT_RIGHT;
#endif
#ifdef SCREEN_SHAPE_CIRCULAR
    GUIRES_DisplayImg(&StartPoint, PNULL, PNULL, winId, IAMGE_CALL_SMSREJECT, &devInfo);
#else
    GUIRES_DisplayImg(&leftStartPoint, PNULL, PNULL, winId, IAMGE_CALL_SMSREJECT, &devInfo);
    GUIRES_DisplayImg(&rightStartPoint, PNULL, PNULL, winId, IAMGE_CALL_SMSREJECT, &devInfo);
#endif
    styleInfo.align    = ALIGN_HVMIDDLE;
    styleInfo.font     = WATCH_STRING_FONT_SMALL; //bug 1321445
    styleInfo.font_color = MMI_WHITE_COLOR ;
    MMI_GetLabelTextByLang(TXT_CC_REJECTBYSMS, &strData);
    GUISTR_DrawTextToLCDInRect(&devInfo, &smsRejectRect, &smsRejectRect, &strData, &styleInfo, state, GUISTR_TEXT_DIR_AUTO);

}


/*****************************************************************************/
//  Discription: draw mt calling win
//  Global resource dependence:
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void DrawMtCallingWin(MMICC_DISPINFO_T *contact_info)
{
    MMI_WIN_ID_T        winId      = WATCHCC_MTCALLING_WIN_ID;
    GUI_LCD_DEV_INFO    devInfo     = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUISTR_STATE_T      state       = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK;
#ifdef SCREEN_SHAPE_CIRCULAR
    GUI_POINT_T         StartPoint = WATCH_PULL_DOWN_START_POINT;
    GUI_RECT_T          callingTextRect = WATCH_CALLING_TEXT_DISP_RECT;
#endif
    GUISTR_STYLE_T      styleInfo  = {0};
    MMI_STRING_T        strData    = {0};
    MMI_STRING_T        dispStr    = {0};

    if(NULL != contact_info)
    {
        // 显示头像和名称
        dispStr.wstr_ptr = &(contact_info->disp_name_buf);
        dispStr.wstr_len= contact_info->disp_name_len;
        MMI_WSTRNCPY(dispStr.wstr_ptr, MMICC_PHONE_NUM_MAX_LEN + 2, (const wchar*)contact_info->disp_name_buf, dispStr.wstr_len, dispStr.wstr_len);
        CC_MtCall_DrawHeadPhotoAndName(winId, contact_info->incall_portrait, dispStr);
        // 显示BUTTON图片
        WatchCOM_Btn_SetPos_2Btn(WATCHCC_HANGUP_BUTTON_CTRL_ID,WATCHCC_DIAL_BUTTON_CTRL_ID);

        if(0 != contact_info->tel_num_len) //bug 1366954
        {
#ifdef SCREEN_SHAPE_CIRCULAR
            if((TRUE == MMICC_IsExistActiveCall()) || (TRUE == MMICC_IsExistHoldCall()))
            {
                GUIRES_DisplayImg(&StartPoint, PNULL, PNULL, winId, IAMGE_CALL_CALLING, &devInfo);
                styleInfo.align    = ALIGN_HVMIDDLE;
                styleInfo.font     = WATCH_STRING_FONT_SMALL;
                styleInfo.font_color = MMI_GREEN_COLOR ;
                MMI_GetLabelTextByLang(TXT_CC_ON_CALL, &strData);
                GUISTR_DrawTextToLCDInRect(&devInfo, &callingTextRect, &callingTextRect, &strData, &styleInfo, state, GUISTR_TEXT_DIR_AUTO);
            }
            else
#endif
            {

            }
        }
    }
}

/*****************************************************************************/
//  Description : handle voice call query window messages
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleCcMoCallQueryWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          )
{
    MMI_RESULT_E       result = MMI_RESULT_TRUE;
    MMIPB_CONTACT_T*   contactInfo = (MMIPB_CONTACT_T *)MMK_GetWinAddDataPtr(win_id);

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            break;
        }
        case MSG_CLOSE_WINDOW:
        {
            MMK_FreeWinAddData(win_id);
            break;
        }
        case MSG_FULL_PAINT:
        {
            WATCHCOM_DisplayBackground(win_id);
            DrawMoCallQueryWin(contactInfo);
            break;
        }
        case MSG_KEYDOWN_RED:
        case MSG_APP_OK:
        {
            WatchCC_MoCallQueryWin_Exit();
            break;
        }
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        {
            //make call
            uint8  numBuf[MMIPB_NUMBER_MAX_STRING_LEN+2] = {0};
            uint8  numLen = MMIPB_GetBCDNumberStrlen(&contactInfo->number);
            MMIPB_BCDNumberToUTF8(contactInfo->number, MMIPB_NUMBER_MAX_STRING_LEN, numBuf);
            WatchAPICC_MakeCall(MN_DUAL_SYS_1, &numBuf, numLen,
                                PNULL, PNULL, CC_SIM1_CALL, CC_CALL_NORMAL_CALL, PNULL);
            break;
        }
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
        {
            if(PNULL != param)
            {
                switch(((MMI_NOTIFY_T*) param)->src_id)
                {
                    case WATCHCC_BACK_BUTTON_CTRL_ID:
                    {
                        WatchCC_MoCallQueryWin_Exit();
                        break;
                    }
                    case WATCHCC_DIAL_BUTTON_CTRL_ID:
                    {
                        //make call
                        uint8  numBuf[MMIPB_NUMBER_MAX_STRING_LEN+2] = {0};
                        uint8  numLen = MMIPB_GetBCDNumberStrlen(&contactInfo->number);
                        MMIPB_BCDNumberToUTF8(contactInfo->number, MMIPB_NUMBER_MAX_STRING_LEN, numBuf);
                        WatchAPICC_MakeCall(MN_DUAL_SYS_1, &numBuf, numLen,
                                            PNULL, PNULL, CC_SIM1_CALL, CC_CALL_NORMAL_CALL, PNULL);
                        break;
                    }
                }
            }
            break;
        }
#endif
        default:
        {
            result = MMI_RESULT_FALSE;
            break;
        }
    }
    return result;
}

/*****************************************************************************/
//  Description : handle mo calling window messages
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleCcMoCallingWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          )
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMICC_DISPINFO_T *pInfo = (MMICC_DISPINFO_T *)MMK_GetWinAddDataPtr(win_id);

    TRACE_APP_CALLCTRL("enter cc calling win, msg_id = 0x%x",msg_id);

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            break;
        }
        case MSG_CLOSE_WINDOW:
        {
            MMK_FreeWinAddData(win_id);
            break;
        }
        case MSG_FULL_PAINT:
        {
            WATCHCOM_DisplayBackground(win_id);
            DrawMoCallingWin(pInfo);
#ifdef PLATFORM_ANTISW3
#ifdef MMI_VOLTE_SUPPORT
            CC_DrawVolteIcon(win_id);
#endif
#endif
            break;
        }
        case MSG_APP_OK:
        {
            MMK_CloseWin(win_id);
            break;
        }

        case MSG_BT_CANCEL_OUTGOING_CALL:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        {
            if(CC_RESULT_SUCCESS == MMICC_ReleaseCallReq(MMIAPICC_GetCallDualSys(), MMICC_GetCurrentCallIndex(), MN_CAUSE_NORMAL_CLEARING))
            {
                //WatchCC_CallEnded_NoteWin_Enter();
                //WatchCC_MoCallingWin_Exit();
                ZDT_SOS_Call_Success();
            }
            break;
        }
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
        {
#ifdef MMI_ETWS_SUPPORT
            if( MMK_IsOpenWin(MMISMS_ETWS_NEWMSG_WIN_ID) )
            {
                MMIAPISMSCB_SetIsNeedRedisplay(TRUE);
            }

            if( MMIAPISMSCB_IsETWSAlert() )
            {
                MMIAPISET_StopSOSVibrate();
            }
#endif
            if(PNULL != param)
            {
                switch(((MMI_NOTIFY_T*) param)->src_id)
                {
                    case WATCHCC_HANGUP_BUTTON_CTRL_ID:
                    {
                        if(CC_RESULT_SUCCESS == MMICC_ReleaseCallReq(MMIAPICC_GetCallDualSys(), MMICC_GetCurrentCallIndex(), MN_CAUSE_NORMAL_CLEARING))
                        {
                            //WatchCC_CallEnded_NoteWin_Enter(); //Bug 2083731
                            //WatchCC_MoCallingWin_Exit();
                        }
                        ZDT_SOS_Call_Success();
                        break;
                    }
                }
            }
            break;
        }
#endif
        default:
        {
            result = MMI_RESULT_FALSE;
            break;
        }
    }
    return result;
}


/*****************************************************************************/
//  Description : handle voice call connected window messages
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleCcVoiceCallConnectedWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          )
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMICC_DISPINFO_T            *pInfo = (MMICC_DISPINFO_T *)MMK_GetWinAddDataPtr(win_id);

    GUI_RECT_T                  bgDispRect = WATCH_RECORDING_BG_DISP_RECT;
    GUI_LCD_DEV_INFO            lcdDevInfo = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    uint32                      curTimeCount = 0;
    wchar                       tempWchar[CC_DISPLAY_TIME_LEN + 2] = {0};
    MMI_STRING_T                timeStr = {0};
    GUI_RECT_T                  timeRect = WATCH_CALL_TIME_DISP_RECT;
    MMI_CTRL_ID_T               timeLabelId = WATCHCC_CALLTIME_LABEL_CTRL_ID;
    timeStr.wstr_len = sizeof(tempWchar)/sizeof(wchar);
    timeStr.wstr_ptr = tempWchar;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            if(0 == s_cc_call_time_count_timer)
            {
                s_cc_call_time_count_timer = MMK_CreateWinTimer(win_id, CC_TIMER_OUT, TRUE );
            }
            // 显示通话时长
            CC_TimeCountToStr(&timeStr, curTimeCount);
            GUILABEL_SetRect(timeLabelId, &timeRect, FALSE);
            GUILABEL_SetText(timeLabelId, &timeStr, FALSE);
            GUILABEL_SetFont(timeLabelId, WATCH_STRING_FONT_SMALL, MMI_WHITE_COLOR);
#ifdef CALL_RECORD_SUPPORT
            GUILABEL_SetVisible(WATCHCC_CALLRECORD_LABEL_CTRL_ID, FALSE, TRUE);
            GUIBUTTON_SetVisible(WATCHCC_RECORDON_BUTTON_CTRL_ID, FALSE, TRUE);
#endif
            break;
        }
        case MSG_CC_CONNECT_OPEN_WINDOW:
            break;
        case MSG_CLOSE_WINDOW:
        {
            MMK_FreeWinAddData(win_id);
            if (s_cc_call_time_count_timer != 0)
            {
                MMK_StopTimer(s_cc_call_time_count_timer);
                s_cc_call_time_count_timer = 0;
            }
            break;
        }
        case MSG_FULL_PAINT:
        {
#ifdef MMI_RECORD_SUPPORT
            if(!MMICC_IsRecording())
#endif
            {
                WATCHCOM_DisplayBackground(win_id);
                CTRLLABEL_FlushBg(timeLabelId);
            }
#ifdef MMI_RECORD_SUPPORT
            else
            {
                GUIRES_DisplayImg(PNULL, &bgDispRect, PNULL, win_id, IAMGE_CALL_RECORDONBG, &lcdDevInfo);
                CTRLLABEL_FlushBg(timeLabelId);
            }
#endif
            DrawVoiceCallConnectedWin(pInfo);
#ifdef PLATFORM_ANTISW3
#ifdef MMI_VOLTE_SUPPORT
            CC_DrawVolteIcon(win_id);
#endif
#endif
            break;
        }
        case MSG_TIMER:
        {
            if(s_cc_call_time_count_timer == *(uint8*)param)
            {
                //MMICC_SetCallTimeCount(MMICC_GetCallTimeCount() + 1);//s_call_context.call_time_count++;
                if ((!MMICC_IsExistOperatedCall()) && (CC_CALL_STATE == MMICC_GetCurrentCallStatus()))
                {
                    curTimeCount = MMICC_GetCurrentCallTime();
                    if (MMK_IsFocusWin(win_id))
                    {
                        CC_TimeCountToStr(&timeStr, curTimeCount);
                        //fresh list time
                        GUILABEL_SetText(timeLabelId, &timeStr, TRUE);
                    }
               }
            }
            break;
        }
        case MSG_APP_OK:
        case MSG_CTL_OK:
        {
#ifdef MMI_RECORD_SUPPORT
            CC_HandleRecordButton(win_id);
#endif
            break;
        }
        case MSG_BT_CANCEL_CALL:
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        {
            MMICC_ReleaseCallReq(MMIAPICC_GetCallDualSys(), MMICC_GetCurrentCallIndex(), MN_CAUSE_NORMAL_CLEARING);
            break;
        }
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
        {
            if(PNULL != param)
            {
                switch(((MMI_NOTIFY_T*) param)->src_id)
                {
#ifdef CALL_RECORD_SUPPORT
                    case WATCHCC_RECORDOFF_BUTTON_CTRL_ID:
                    case WATCHCC_RECORDON_BUTTON_CTRL_ID:
                    {
                        CC_HandleRecordButton(win_id);
                        break;
                    }
#endif
                    case WATCHCC_HANGUP_BUTTON_CTRL_ID:
                    {
                        MMICC_ReleaseCallReq(MMIAPICC_GetCallDualSys(), MMICC_GetCurrentCallIndex(), MN_CAUSE_NORMAL_CLEARING);
                        break;
                    }
                }
            }
            break;
        }
#endif

        case MSG_GET_FOCUS:
        {
            TRACE_APP_CALLCTRL("MSG_GET_FOCUS");
            break;
        }
        default:
        {
            result = MMI_RESULT_FALSE;
            break;
        }
    }
    return result;
}

/*****************************************************************************/
//  Description : handle voice call incoming Window messages
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleCcMtCallingWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          )
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMICC_DISPINFO_T            *pInfo = (MMICC_DISPINFO_T *)MMK_GetWinAddDataPtr(win_id);
    GUI_RECT_T                  vaildRect = WATCH_SMS_REJECT_TP_VAILD_RECT;
    LOCAL BOOLEAN               needPlayRing = FALSE;
    LOCAL uint32                callRingTimes = 0;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            if (MMICC_IsExistIncommingCall())
            {
                needPlayRing = TRUE;
            }

#ifdef MMI_ETWS_SUPPORT
            if (MMICC_IsExistIncommingCall() && needPlayRing && ( FALSE == MMIAPISMSCB_IsETWSAlert()))
#else
            if (MMICC_IsExistIncommingCall() && needPlayRing )
#endif
            {
                needPlayRing = FALSE;
                CC_PlayNewCallRing();
                callRingTimes ++;
            }
            MMK_WinGrabFocus(MMK_ConvertIdToHandle(win_id));
            break;
        }
        case MSG_CLOSE_WINDOW:
        {
            MMK_FreeWinAddData(win_id);
            if (s_cc_waiting_call_ring_timer != 0)
            {
                MMISRVAUD_StopTone(MMISRVAUD_TYPE_TONE_GENERIC);
                MMK_StopTimer(s_cc_waiting_call_ring_timer);
                s_cc_waiting_call_ring_timer = 0;
                callRingTimes = 0;
            }
            MMIAPICC_StopRingOrVibrate();
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            break;
        }
        case MSG_FULL_PAINT:
        {

#ifdef MMI_ETWS_SUPPORT
            if (MMICC_IsExistIncommingCall() && needPlayRing && (!MMK_IsOpenWin(MMISMS_ETWS_NEWMSG_WIN_ID)))
            {
                needPlayRing = FALSE;
                CC_PlayNewCallRing();
                callRingTimes ++;
            }
#endif
            WATCHCOM_DisplayBackground(win_id);
            DrawMtCallingWin(pInfo);
#ifdef PLATFORM_ANTISW3
#ifdef MMI_VOLTE_SUPPORT
            CC_DrawVolteIcon(win_id);
#endif
#endif
            //自动接听功能，工程模式里面需要进行设置
            CC_AutoAnswerIncomingCall();
            break;
        }
        case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T  tp_point = {0};
            tp_point.x = MMK_GET_TP_X(param);
            tp_point.y = MMK_GET_TP_Y(param);
#ifdef SCREEN_SHAPE_CIRCULAR
            if((FALSE == MMICC_IsExistActiveCall()) && (FALSE == MMICC_IsExistHoldCall()))
#endif
            {
                if((GUI_PointIsInRect(tp_point, vaildRect)) && (0 != pInfo->tel_num_len))
                {
                    //短信拒接
                    uint8   telNum[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
                    uint8   telNumLen = 0;
                    MMISMS_DEST_ADDR_INFO_T numberInfo = {0}; //保存号码信息
                    MMISMS_SetPresetFrom( MMISMS_PRESETFROM_CALL );
                    MMICC_GetCallNumStrByIndex(telNum, &telNumLen, MMICC_GetCurrentCallIndex());
                    MMISMS_SetAddressToMessage((uint8 *)telNum, telNumLen, &numberInfo.dest_list);
                    //save phone number
                    MMISMS_SetNumberForSend(numberInfo);
                    WatchSMS_PresetListWin_Enter(PNULL);
                }
            }
            break;
        }
        case MSG_TIMER:
        {
            if (s_cc_waiting_call_ring_timer == *(uint8*)param)
            {
                if (callRingTimes <= CC_WATCH_WAITING_CALL_PLAY_TIMER)
                {
                    MMIAPISET_PlayRing(MMIAPICC_GetCallDualSys(), FALSE,0,1,MMISET_RING_TYPE_CALL_IN_CALL,PNULL);
                    MMK_StartWinTimer( win_id, s_cc_waiting_call_ring_timer, CC_WATCH_WAITING_CALL_PERIOD, FALSE);
                    callRingTimes ++;
                }
                else
                {
                    MMK_StopTimer(s_cc_waiting_call_ring_timer);
                    s_cc_waiting_call_ring_timer = 0;
                    callRingTimes = 0;
                }
            }
            break;
        }

        case MSG_APP_UP:
        {
            if(0 != pInfo->tel_num_len)
            {
                uint8	telNum[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
                uint8	telNumLen = 0;
                MMISMS_DEST_ADDR_INFO_T numberInfo = {0}; //保存号码信息
                MMISMS_SetPresetFrom( MMISMS_PRESETFROM_CALL );
                MMICC_GetCallNumStrByIndex(telNum, &telNumLen, MMICC_GetCurrentCallIndex());
                MMISMS_SetAddressToMessage((uint8 *)telNum, telNumLen, &numberInfo.dest_list);
                //save phone number
                MMISMS_SetNumberForSend(numberInfo);
                WatchSMS_PresetListWin_Enter(PNULL);
            }
            break;
        }
        case MSG_APP_DOWN:
        {
            //拨出情况调解音量
            if (CC_CALLING_STATE == MMICC_GetCurrentCallStatus())
            {
                //OpenVolumePanel(win_id, msg_id);
            }
            else if(CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus())
            {
                MMIAPICC_StopRingOrVibrate();
            }
            break;
        }
        case MSG_BT_REJECT_INCOMING_CALL:
        case MSG_APP_OK:
        {
            MMICC_ReleaseCurrentCallReq(MN_CAUSE_USER_BUSY);
            break;
        }
        case MSG_BT_ACCEPT_INCOMING_CALL:
        case MSG_APP_CANCEL:
        {
            if(!MMICC_AnswerCall())
            {
                TRACE_APP_CALLCTRL("bt answer call fail");
            }
            break;
        }
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
        {
#ifdef MMI_ETWS_SUPPORT
            if( MMK_IsOpenWin(MMISMS_ETWS_NEWMSG_WIN_ID) )
            {
                MMIAPISMSCB_SetIsNeedRedisplay(TRUE);
            }

            if( MMIAPISMSCB_IsETWSAlert() )
            {
                MMIAPISET_StopSOSVibrate();
            }
#endif
            if(PNULL != param)
            {
                switch(((MMI_NOTIFY_T*) param)->src_id)
                {
                    case WATCHCC_DIAL_BUTTON_CTRL_ID:
                    {
                        MMICC_AnswerCall();
                        break;
                    }
                    case WATCHCC_HANGUP_BUTTON_CTRL_ID:
                    {
                        MMICC_ReleaseCurrentCallReq(MN_CAUSE_USER_BUSY);
                        break;
                    }
                }
            }
            break;
        }
#endif
        default:
        {
            result = MMI_RESULT_FALSE;
            break;
        }
    }
    return result;
}

/*****************************************************************************/
//	Description : the process message function of the sending dtmf
//	Global resource dependence :
//  Author:feiyue.ji
//	Note:2020.7.23
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSendingDTMFNoteWinMsg(
                                                 MMI_WIN_ID_T win_id,
                                                 MMI_MESSAGE_ID_E msg_id,
                                                 DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch(msg_id) {
    case MSG_OPEN_WINDOW:
    {
        //CC_SendStoredDtmf();
        break;
    }

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
//	Description : the process message function of query win
//	Global resource dependence :
//  Author:feiyue.ji
//	Note:2020.7.23
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSendDTMFQueryWinMsg(
                                                MMI_WIN_ID_T win_id,
                                                MMI_MESSAGE_ID_E msg_id,
                                                DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            break;
        }

        case MSG_APP_OK:
        case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
            {
                MMICC_SendStringDtmf();
                MMK_CloseWin(win_id);
                break;
            }
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL://fix bug1374587
        {
            MMICC_CancelStringDtmf();
            MMK_CloseWin(WATCHCC_SEND_DTMF_NOTE_WIN_ID);
            MMK_CloseWin(win_id);
            break;
        }
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : send dtmf query window
//  Parameter: [In] dtmf_info
//             [Out] None
//             [Return] None
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_SendDtmf_QueryWin_Enter(uint8* p_dtmf_str,uint8 dtmf_len)
{
    MMI_WIN_ID_T    dtmf_query_win_id = WATCHCC_SEND_DTMF_QUERY_WIN_ID;
    MMI_STRING_T    dtmf_str = {0};
    wchar           temp_alert_wchar[CC_MAX_TELE_NUM_LEN + 1] = {0};
    wchar           temp_wchar[100] = {0};
    MMI_STRING_T    promt_string = {0};
    MMI_STRING_T    promt_str = {0};
    MMI_STRING_T    button_left_str = {0};
    MMI_STRING_T    button_right_str = {0};
    WATCH_SOFTKEY_TEXT_ID_T softkey_text = {TXT_SEND, TXT_NULL, STXT_CANCEL};
    wchar alt[2] = {'\n','\0'};

    dtmf_str.wstr_ptr = temp_alert_wchar;
    dtmf_str.wstr_len = dtmf_len;
    MMI_STRNTOWSTR(dtmf_str.wstr_ptr, dtmf_str.wstr_len, p_dtmf_str,dtmf_len, dtmf_str.wstr_len);
    promt_string.wstr_ptr = temp_wchar;

    MMI_GetLabelTextByLang(TXT_CC_SEND_DTMF, &promt_str);
    MMI_GetLabelTextByLang(TXT_SEND, &button_left_str);
    MMI_GetLabelTextByLang(STXT_RETURN, &button_right_str);
    MMIAPICOM_Wstrncat(promt_string.wstr_ptr, promt_str.wstr_ptr, promt_str.wstr_len);// fix bug1374557
    //由于MMIAPICOM_Wstrncat的第二个入参为const类型，因此在传参时需要注意参数类型 ，直接传入空格" "，会造成指针越界，且强转类型不可行
    MMIAPICOM_Wstrncat(promt_string.wstr_ptr, alt, 1);
    MMIAPICOM_Wstrncat(promt_string.wstr_ptr, dtmf_str.wstr_ptr, dtmf_str.wstr_len);
    promt_string.wstr_len = MIN(dtmf_str.wstr_len+promt_str.wstr_len+1, 100);

    if (MMK_IsOpenWin(dtmf_query_win_id))
    {
        MMK_CloseWin(dtmf_query_win_id);
    }
    WatchCOM_QueryWin_2Btn_Enter(dtmf_query_win_id, &promt_string, &button_left_str, &button_right_str, IMAGE_NULL, IMAGE_NULL, softkey_text, HandleSendDTMFQueryWinMsg);
}

/*****************************************************************************/
//  Description : sending dtmf tip window
//  Parameter: [In] dtmf_info
//             [Out] None
//             [Return] None
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_SendDtmf_NoteWin_Enter(uint8* p_dtmf_str,uint8 dtmf_len)
{
    MMI_WIN_ID_T    alert_win_id = WATCHCC_SEND_DTMF_NOTE_WIN_ID;
    MMI_STRING_T    dtmf_str = {0};
    wchar           temp_alert_wchar[CC_MAX_TELE_NUM_LEN + 1] = {0};
    wchar           temp_wchar[100] = {0};
    MMI_STRING_T    promt_string = {0};
    MMI_STRING_T    promt_str = {0};
    WATCH_SOFTKEY_TEXT_ID_T softkey_text = {TXT_SEND, TXT_NULL, STXT_CANCEL};
    wchar alt[2] = {'\n','\0'};

    dtmf_str.wstr_ptr = temp_alert_wchar;
    dtmf_str.wstr_len = dtmf_len;
    MMI_STRNTOWSTR(dtmf_str.wstr_ptr, dtmf_str.wstr_len,p_dtmf_str,dtmf_len, dtmf_str.wstr_len);
    promt_string.wstr_ptr = temp_wchar;

    softkey_text.softkeyLeft =TXT_NULL;
    softkey_text.softkeyMiddle =TXT_NULL;
    softkey_text.softkeyRight =TXT_NULL;
    MMI_GetLabelTextByLang(TXT_CC_SENDING_DTMF, &promt_str);
    MMIAPICOM_Wstrncat(promt_string.wstr_ptr, promt_str.wstr_ptr, promt_str.wstr_len);// fix bug1374557
    MMIAPICOM_Wstrncat(promt_string.wstr_ptr, alt, 1);
    MMIAPICOM_Wstrncat(promt_string.wstr_ptr, dtmf_str.wstr_ptr, dtmf_str.wstr_len);
    promt_string.wstr_len = MIN(dtmf_str.wstr_len+promt_str.wstr_len+1, 100);

    if (MMK_IsOpenWin(alert_win_id))
    {
        MMK_CloseWin(alert_win_id);
    }
    TRACE_APP_CALLCTRL("promt_string:%s",promt_string);
    WatchCOM_NoteWin_2Line_Enter(alert_win_id, &promt_string, NULL, softkey_text,  HandleSendingDTMFNoteWinMsg);

}

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//  Description : play new call ring
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPICC_PlayNewCallRing(MN_DUAL_SYS_E dual_sys)
{
    CC_PlayNewCallRing(dual_sys);
}
#endif

/*****************************************************************************/
//  Description : thermal close business note win
//  Parameter: [In] NONE
//             [Out]NONE
//             [Return]NONE
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_Thermal_Win_Enter(void)
{
    MMI_STRING_T            operating_str = {0};
    WATCH_SOFTKEY_TEXT_ID_T softkey_text  = {NULL, NULL, NULL};
    TRACE_APP_CALLCTRL("enter WatchCC_Thermal_NoteWin");
    MMI_GetLabelTextByLang(TXT_THERMAL_CLOSE_BUSINESS, &operating_str);
    WatchCOM_WaitingWin_1Line_Enter(WATCHCC_THERMAL_WIN_ID,&operating_str,0,softkey_text,PNULL);
}

