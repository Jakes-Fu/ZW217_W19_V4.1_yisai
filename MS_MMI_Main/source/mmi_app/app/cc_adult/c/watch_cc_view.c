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
#include "guires.h"
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
#include "mmieng_export.h"
#include "watch_record_api.h"
#include "mmidata_share.h"
#include "mmi_adapt_screen.h"
#include "ual_bt.h"
#ifdef UAL_BT_HFU_SUPPORT
#include "ual_bt_hfu.h"
#endif
#ifdef BBM_ROLE_SUPPORT
#include "bbm_export.h"
#endif
#ifdef MMI_ETWS_SUPPORT
#include "mmisms_export.h"
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define CC_INSTANCE         (MMICC_GetAppletInstance()->cc_instance)

#define WATCH_CALL_MODE_START_POINT         DP2PX_POINT(112,220)         //通话模式图标显示
#define WATCH_CALL_AUDIO_ROUTE_RECT         DP2PX_RECT(104,8,136,40)     //音频通路状态显示区域
#define WATCH_MO_QUERY_STR_DISP_RECT        DP2PX_RECT(36,36,204,150)    //询问窗字符串显示区域
#define WATCH_MO_NAME_DISP_RECT             DP2PX_RECT(36,56,204,76)     //去电窗口名字显示
#define WATCH_MT_NAME_DISP_RECT             DP2PX_RECT(36,56,204,76)     //来电窗口名字显示
#define WATCH_CALL_STATE_DISP_RECT          DP2PX_RECT(36,88,204,104)    //通话状态显示
#define WATCH_CALL_TIME_DISP_RECT           DP2PX_RECT(36,88,204,104)    //通话时长显示

#define WATCH_BTN_LEFT_RECT                 DP2PX_RECT(48,124,80,156)     //静音按钮
#define WATCH_BTN_MIDDLE_RECT               DP2PX_RECT(104,124,136,156)   //音量调节
#define WATCH_BTN_RIGHT_RECT                DP2PX_RECT(160,124,192,156)   //拨号盘
#define WATCH_BTN_SINGLE_RECT               DP2PX_RECT(96,168,144,216)    //挂断键-单按键
#define WATCH_BTN_TWO_LEFT_RECT             DP2PX_RECT(60,168,108,216)    //挂断键-双按键
#define WATCH_BTN_TWO_RIGHT_RECT            DP2PX_RECT(132,168,180,216)   //接听键-双按键

#define WATCH_STRING_FONT_BIG               DP2PX_FONT(SONG_FONT_30)
#define WATCH_STRING_FONT_MIDDLE            DP2PX_FONT(SONG_FONT_20)
#define WATCH_STRING_FONT_SMALL             DP2PX_FONT(SONG_FONT_16)
#define WATCH_NOTE_LEN_FOR_1LINE            DP2PX_VALUE(215)

#define CC_WATCH_WAITING_CALL_PERIOD        MMI_2SECONDS                  //每次播放间隔时间
#define CC_WATCH_WAITING_CALL_PLAY_TIMER    6                             //呼叫等待时候，tone播放的次数
#define CC_WATCH_DTMF_WAITING_TIME          MMI_3SECONDS
#define CC_WATCH_VOL_CLOSE_TIME             MMI_2SECONDS                  //音量设置界面自动关闭计时

#ifdef MMI_VOLTE_SUPPORT
#define WATCH_VOLTE_ICON_START_POINT        DP2PX_POINT(104,40)           //VOLTE图标
#endif


/**--------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL  uint32                   g_call_time_count = 0;
LOCAL  uint8                    g_call_volume_end_timer = 0;
LOCAL  BOOLEAN                  g_call_vol_auto_end = FALSE;
LOCAL  MMICC_WANT_MO_CALL_T     g_want_mo_call_info = {0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle set volume window messages
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleCcSetVolumeWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          );

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

#ifdef BBM_ROLE_SUPPORT
/*****************************************************************************/
//  Description : make call flow when Bbm support
//      BBM控制下的呼出流程: 先判断当前的设备形态，如果是Phone形态，则可以直接
//      呼出MN电话；如果是Dialer形态，先判断当前Dialer是否为已连接状态，如果已
//      连接，直接呼出BT电话；如果未连接，说明当前为BLE连接，需要调用Switch接口
//      连接Dialer，Dialer连接成功的事件是UAL_BT_SLC_CONNECTED_IND，收到该消息
//      后，再呼出BT电话
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CC_MakeCall_ForBbm(MN_DUAL_SYS_E dual_sys,
                                       uint8 *tele_num,
                                       uint8  tele_len
                                       );
#endif
/*****************************************************************************/
//  Description : make call for fixed mode: Dialer or Phone
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CC_MakeCall_ForNormal(MN_DUAL_SYS_E dual_sys,
                                       uint8 *tele_num,
                                       uint8  tele_len
                                       );

/*****************************************************************************/
//  Description : get current product role
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  CC_CurrentRoleIsPhone(void);

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
//  Description : update call status(button and time)
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void  CC_UpdateWatchCallStatus(void);

/*****************************************************************************/
//  Description : get mo call error text by type
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T CC_GetErrorTextByType(MMICC_MO_CALL_ERROR_TYPE_E type);

/*****************************************************************************/
//  Description : get mo call error image by type
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T CC_GetErrorImageByType(MMICC_MO_CALL_ERROR_TYPE_E type);

/*****************************************************************************/
//  Description : draw audio device icon
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CC_DrawAudioDevBtn(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : handle set audio device button
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CC_HandleSetAudioDevButton(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : play new call ring
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  CC_PlayNewCallRing();

/*****************************************************************************/
//  Description : handle mute button
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CC_HandleMuteButton(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : reset timer for auto close volume win
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CC_ResetVolumeWinTimer(MMI_WIN_ID_T win_id);

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
PUBLIC void  CC_StartDtmfWaitingTimer(void);

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
    //WIN_STYLE(WS_DISABLE_RETURN_WIN),
    CREATE_BUTTON_CTRL(IMAGE_ADULT_CALL_BACK, WATCHCC_BACK_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_ADULT_CALL_DIAL, WATCHCC_DIAL_BUTTON_CTRL_ID),
    END_WIN
};//mo call query win

WINDOW_TABLE(WATCHCC_MOCALLING_WIN_TAB) =
{
    WIN_ID(WATCHCC_MOCALLING_WIN_ID),
    WIN_FUNC((uint32)HandleCcMoCallingWinMsg),
    WIN_HIDE_STATUS,
    WIN_STYLE(WS_DISABLE_RETURN_WIN),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,  WATCHCC_CALLSTATE_LABEL_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_ADULT_CALL_ROUTE_EARPHONE, WATCHCC_ROUTE_EARPHONE_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_ADULT_CALL_ROUTE_WATCH, WATCHCC_ROUTE_WATCH_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_ADULT_CALL_ROUTE_PHONE, WATCHCC_ROUTE_PHONE_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_ADULT_CALL_MUTE_DISABLE, WATCHCC_MUTE_DISABLE_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_ADULT_CALL_VOLUME, WATCHCC_VOLUME_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_ADULT_CALL_DIALPAD, WATCHCC_DIALPAD_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_ADULT_CALL_HANGUP, WATCHCC_HANGUP_BUTTON_CTRL_ID),
    END_WIN
};//mo calling win

WINDOW_TABLE(WATCHCC_CONNECTED_WIN_TAB) =
{
    WIN_ID(WATCHCC_CONNECTED_WIN_ID),
    WIN_FUNC((uint32)HandleCcVoiceCallConnectedWinMsg),
    WIN_HIDE_STATUS,
    WIN_STYLE(WS_DISABLE_RETURN_WIN),
    CREATE_BUTTON_CTRL(IMAGE_ADULT_CALL_ROUTE_EARPHONE, WATCHCC_ROUTE_EARPHONE_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_ADULT_CALL_ROUTE_WATCH, WATCHCC_ROUTE_WATCH_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_ADULT_CALL_ROUTE_PHONE, WATCHCC_ROUTE_PHONE_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_ADULT_CALL_MUTE, WATCHCC_MUTE_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_ADULT_CALL_UNMUTE, WATCHCC_UNMUTE_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_ADULT_CALL_VOLUME, WATCHCC_VOLUME_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_ADULT_CALL_DIALPAD, WATCHCC_DIALPAD_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_ADULT_CALL_HANGUP, WATCHCC_HANGUP_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_ADULT_CALL_HANGUP_DISABLE, WATCHCC_HANGUP_DISABLE_BUTTON_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,  WATCHCC_CALLTIME_LABEL_CTRL_ID),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};//mo or mt connected

WINDOW_TABLE(WATCHCC_MTCALLING_WIN_TAB) =
{
    WIN_ID(WATCHCC_MTCALLING_WIN_ID),
    WIN_FUNC((uint32)HandleCcMtCallingWinMsg),
    WIN_HIDE_STATUS,
    WIN_STYLE(WS_DISABLE_RETURN_WIN),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,  WATCHCC_CALLSTATE_LABEL_CTRL_ID),
    //Dial means Answer Call here
    CREATE_BUTTON_CTRL(IMAGE_ADULT_CALL_DIAL, WATCHCC_DIAL_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_ADULT_CALL_HANGUP, WATCHCC_HANGUP_BUTTON_CTRL_ID),
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
    MMK_CreateWin((uint32 *)WATCHCC_QUERYMOCALL_WIN_TAB, param);
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
PUBLIC void WatchCC_ConnectedWin_Enter(void)
{
    MMK_CreateWin((uint32 *)WATCHCC_CONNECTED_WIN_TAB, PNULL);
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
//  Description : open set volume window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_SetVolumeWin_Enter(void)
{
    MMI_WIN_ID_T        winId = WATCHCC_SETVOLUME_WIN_ID;
    MMI_IMAGE_ID_T      volHigh = IMAGE_ADULT_CALL_VOLUME_HIGH;
    MMI_IMAGE_ID_T      volLow = IMAGE_ADULT_CALL_VOLUME_LOW;
    uint32              volMax = MMISET_VOL_MAX;
    uint32              volCur = MMIAPICC_GetCallVolume();
#ifdef BLUETOOTH_SUPPORT
    BT_ADDRESS          address = {0};
#endif

#ifdef BLUETOOTH_SUPPORT
    if(TRUE == ual_bt_dev_get_is_on())
    {
        if(TRUE == CC_CurrentRoleIsPhone())
        {
            if (MMIAPIBT_GetActivatedHS(&address))
            {
                if(MMICC_AUDIO_DEV_BT == MMICC_GetCurrentDev())
                {
                    volMax = MMISET_BT_VOL_MAX;//Bug 1918144
                    volCur = MMIAPICC_GetBtSpeakerVol();//Bug 1918144
                }
            }
        }
    }
#endif

    TRACE_APP_CALLCTRL("volMax = %d, volCur = %d", volMax, volCur);
    WatchCOM_ProgressWin_Enter(winId, volHigh, volLow, volMax, volCur, HandleCcSetVolumeWinMsg);
    WatchCOM_ProgressWin_SetConstraint(winId, volMax, MMISET_VOL_ONE); //Bug 1579276
}

/*****************************************************************************/
//  Description : Close set volume window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_SetVolumeWin_Exit(void)
{
    if(MMK_IsOpenWin(WATCHCC_SETVOLUME_WIN_ID))
    {
        MMK_CloseWin(WATCHCC_SETVOLUME_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : call tip window
//  Parameter: [In] text_id, image_id
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_NoteWin_Enter(MMI_TEXT_ID_T text_id, MMI_IMAGE_ID_T image_id)
{
    MMI_WIN_ID_T    winId   = WATCHCC_CALL_ENDED_WIN_ID;
    MMI_STRING_T    noteStr = {0};
    MMI_GetLabelTextByLang(text_id, &noteStr);
    Adult_WatchCOM_NoteWin_1Icon_1Str_Enter(winId, &noteStr, image_id, PNULL);
}

/*****************************************************************************/
//  Description : update current call audio device
//  Parameter: [In] text_id, image_id
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_UpdateCurrentCallDev(void)
{
    TRACE_APP_CALLCTRL("WatchCC_UpdateCurrentCallDev, Enter.");

    if(MMK_IsOpenWin(WATCHCC_MOCALLING_WIN_ID))
    {
        CC_DrawAudioDevBtn(WATCHCC_MOCALLING_WIN_ID);
    }
    if(MMK_IsOpenWin(WATCHCC_CONNECTED_WIN_ID))
    {
        CC_DrawAudioDevBtn(WATCHCC_CONNECTED_WIN_ID);
    }
}

#ifdef BLUETOOTH_SUPPORT
/*****************************************************************************/
//  Description : 收到蓝牙调节音量的消息，显示音量调节窗口
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_BTSpeakerVolChange(void)
{
    MMI_WIN_ID_T    winId = WATCHCC_SETVOLUME_WIN_ID;
    uint8           volBt = MMIAPICC_GetBtSpeakerVol();

    TRACE_APP_CALLCTRL("BT Speaker vol = %d", volBt);
    MMIDEFAULT_TurnOnBackLight();
    MMIAPICC_SetCallVolume(volBt);

    if(MMK_IsOpenWin(WATCHCC_MOCALLING_WIN_ID) || MMK_IsOpenWin(WATCHCC_CONNECTED_WIN_ID))
    {
        if(!MMK_IsOpenWin(winId))
        {
            g_call_vol_auto_end = TRUE;
            WatchCC_SetVolumeWin_Enter();
            WatchCOM_ProgressWin_SetProgress(winId, (uint32)volBt);
        }
        else
        {
            if(TRUE == g_call_vol_auto_end)
            {
                CC_ResetVolumeWinTimer(winId);
            }
            WatchCOM_ProgressWin_SetProgress(winId, (uint32)volBt);
        }
    }
}
#endif

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

    if((MMICC_MO_START_IND == type) || (MMICC_MT_START_IND == type))
    {
        //construct audio handle
        if(PNULL == MMICC_GetAppletInstance())
        {
            TRACE_APP_CALLCTRL("MMICC_StartCCApplet, type = %d", type);
            MMICC_StartCCApplet();
        }
    }

    switch(type)
    {
        case MMICC_MO_START_IND:
        {
            MMIDEFAULT_TurnOnBackLight();
            CC_OpenWatchMoCallWin();
            break;
        }
        case MMICC_MT_START_IND:
        {
            MMIDEFAULT_TurnOnBackLight();
            WATCHRECORD_StopRecordOrPlay();
            CC_OpenWatchMtCallWin();
            MMIUSBSHARE_Disconnect();
            break;
        }
        case MMICC_MO_CONNECTED_IND:
        {
            CC_UpdateWatchCallStatus();
            WatchCC_MoCallingWin_Exit();
            break;
        }
        case MMICC_MT_CONNECTED_IND:
        {
            CC_UpdateWatchCallStatus();
            WatchCC_MtCallingWin_Exit();
            break;
        }
        case MMICC_DISCONNECTED_IND:
        {
            CC_CALL_STATE_E  callState = MMICC_GetCallStatusByIndex(MMICC_GetCurrentCallIndex());
            if (MMIAPICC_IsInState(CC_MO_CONNECTING_STATE) || MMIAPICC_IsInState(CC_MT_CONNECTING_STATE))
            {
                MMICC_ReleaseCallReq(MN_DUAL_SYS_1, MMICC_GetCurrentCallIndex(), MN_CAUSE_NORMAL_CLEARING);
            }

            if((CC_INCOMING_CALL_STATE == callState || CC_WAITING_STATE == callState)
                && CC_RELEASE_OPER != MMICC_GetCallOperatedByIndex(MMICC_GetCurrentCallIndex()))
            {
                //来电时非主动挂断,挂断后不弹提示窗
                SCI_TRACE_LOW("[WatchCC] MMICC_UpdateCallStatusDisplay, No notewin when MT is not hangup by self");
            }
            else//主动挂断来电
            {
                WatchCC_NoteWin_Enter(TXT_CC_NOTE_CALL_END, IMAGE_CALL_NOTIFY_ENDED);
            }

            WatchCC_MoCallingWin_Exit();
            WatchCC_ConnectedWin_Exit();
            WatchCC_MtCallingWin_Exit();
#ifdef MMI_ETWS_SUPPORT
            if( (TRUE == MMIAPISMSCB_GetEMCallStatus()) || (TRUE == MMIAPISMSCB_GetIsNeedRedisplay()) )
            {
                MMIAPISMSCB_ShowNewETWSPrompt();
            }
#endif
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
//  Parameter: [In] tele_num, tele_len
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchAPICC_DialNumber(uint8 *tele_num, uint8 tele_len)
{
    MMICC_CALLINFO_T         *callInfo = NULL;

    if((PNULL == tele_num) || (0 == tele_len))
    {
        TRACE_APP_CALLCTRL("Number is null");
        return;
    }

    //先解析是否是暗码
    if(TRUE == MMIAPIPARSE_ParseString(tele_num, tele_len))
    {
        TRACE_APP_CALLCTRL("[CC] Parsed in ParseString.");
        return;
    }

    callInfo = (MMICC_CALLINFO_T *)SCI_ALLOC(sizeof(MMICC_CALLINFO_T));
    SCI_ASSERT(callInfo != NULL);
    SCI_MEMSET(callInfo, 0, sizeof(MMICC_CALLINFO_T));

    callInfo->tele_len = tele_len;
    MMI_MEMCPY(
        callInfo->tele_num,
        callInfo->tele_len,
        tele_num,
        tele_len,
        MIN(MMICC_PHONE_NUM_MAX_LEN, tele_len)); //Bug 1607002

    WatchCC_MoCallQueryWin_Enter(callInfo);
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
#ifdef BBM_ROLE_SUPPORT
    TRACE_APP_CALLCTRL("[CC] make call flow for bbm.");
    CC_MakeCall_ForBbm(dual_sys, tele_num, tele_len);
#else
    TRACE_APP_CALLCTRL("[CC] make call in fixed mode.");
    CC_MakeCall_ForNormal(dual_sys, tele_num, tele_len);
#endif
}

#endif //PUBLIC_FUNC

#ifdef BBM_ROLE_SUPPORT
/*****************************************************************************/
//  Description : make call flow when Bbm support
//      BBM控制下的呼出流程: 先判断当前的设备形态，如果是Phone形态，则可以直接
//      呼出MN电话；如果是Dialer形态，先判断当前Dialer是否为已连接状态，如果已
//      连接，直接呼出BT电话；如果未连接，说明当前为BLE连接，需要调用Switch接口
//      连接Dialer，Dialer连接成功的事件是UAL_BT_SLC_CONNECTED_IND，收到该消息
//      后，再呼出BT电话
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CC_MakeCall_ForBbm(MN_DUAL_SYS_E dual_sys,
                                       uint8 *tele_num,
                                       uint8  tele_len
                                       )
{
    //dialer mode
    if(FALSE == CC_CurrentRoleIsPhone())
    {
        //dialer mode && bt hfu is connected
#ifdef UAL_BT_HFU_SUPPORT
        TRACE_APP_CALLCTRL("[CC] hfu_connect_status = %d", ual_bt_hfu_get_connect_status());
        if(TRUE == ual_bt_hfu_get_connect_status())
        {
            CC_MakeCall_ForNormal(dual_sys, tele_num, tele_len);
        }
        //dialer mode && bt hfu is not connected
        else
#endif
        {
            //call bbm action, 切换到Dialer
            if(BBM_RETURN_SUCESS == BbmSwitchToBtDialer())
            {
                //记录want mo call信息
                g_want_mo_call_info.is_want_mo_call = TRUE;
                g_want_mo_call_info.tele_len = tele_len;
                MMI_MEMCPY(g_want_mo_call_info.tele_num, g_want_mo_call_info.tele_len,
                    tele_num, tele_len, MIN(MMICC_PHONE_NUM_MAX_LEN, tele_len));
                //打开呼叫中窗口等待，等SLC连接成功才能调用HFU接口make call
                MMIAPICC_SetCallVolume(MMIAPISET_GetCallVolume()); //Bug 1649299
                CC_OpenWatchMoCallWin();
            }
            else
            {
                WatchCC_NoteWin_Enter(TXT_CC_NOTE_BTCALL_FAIL, IMAGE_CALL_NOTIFY_CHEAK_BT);
            }
        }
    }
    //phone mode
    else
    {
        CC_MakeCall_ForNormal(dual_sys, tele_num, tele_len);
    }
}
#endif

/*****************************************************************************/
//  Description : make call for fixed mode: Dialer or Phone
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CC_MakeCall_ForNormal(MN_DUAL_SYS_E dual_sys,
                                       uint8 *tele_num,
                                       uint8  tele_len
                                       )
{
    CC_RESULT_E  result = CC_RESULT_ERROR;
    result = WatchCC_MakeCall(dual_sys, tele_num, tele_len);

    //make auto call failure
    if (result == CC_RESULT_ERROR)
    {
        //phone call
        if(TRUE == CC_CurrentRoleIsPhone())
        {
            MMI_TEXT_ID_T   textId = TXT_NULL;
            MMI_IMAGE_ID_T  imageId = IMAGE_NULL;
            //如果返回失败，获取当前的失败类型，通过类型弹出对应错误提示
            MMICC_MO_CALL_ERROR_TYPE_E  errorType = MMICC_GetMoCallErrorType();

            //print trace
            TRACE_APP_CALLCTRL("[CC] Make SIM Call Failure!");

            textId = CC_GetErrorTextByType(errorType);
            imageId = CC_GetErrorImageByType(errorType);
            WatchCC_NoteWin_Enter(textId, imageId);
        }
        //bt call
        else
        {
            //print trace
            TRACE_APP_CALLCTRL("[CC] Make BT Call Failure!");
            WatchCC_NoteWin_Enter(TXT_CC_NOTE_BTCALL_FAIL, IMAGE_CALL_NOTIFY_CHEAK_BT);
        }
    }
    //make auto call success
    else
    {
        //phone call
        if(TRUE == CC_CurrentRoleIsPhone())
        {
            //print trace
            TRACE_APP_CALLCTRL("[CC] Make SIM Call Success!");
        }
        //bt call
        else
        {
            uint8 volume = MMIAPISET_GetCallVolume();
            TRACE_APP_CALLCTRL("[CC] Make BT Call Success, set_volume = %d", volume);
            // HFU不直接存储和读取NV，音量值需要APP设置下去
            MMIAPICC_SetCallVolume(volume);
        }
    }
}


/*****************************************************************************/
//  Description : get current product role
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  CC_CurrentRoleIsPhone(void)
{
    //BBM存在时优先判断BBM状态，否则从工模中获取当前设备形态
#ifdef BBM_ROLE_SUPPORT
    TRACE_APP_CALLCTRL("[CC] IsBtPhone = %d", BbmCurrentRoleIsBtPhone());
    return BbmCurrentRoleIsBtPhone();
#elif defined(UAL_BT_HFU_SUPPORT)
    TRACE_APP_CALLCTRL("[CC] product_role = %d", ual_bt_get_product_role());
    if(1 == ual_bt_get_product_role())
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#else
    //若BBM 与HFU 都关闭，则只支持phone形态
    TRACE_APP_CALLCTRL("[CC] Only support phone product.");
    return TRUE;
#endif
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
    uint8			        telNum[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
    uint8			        telNumLen = 0;
    wchar                   dispWchar[MMICC_PHONE_NUM_MAX_LEN + 2]  ={0};
	MMICC_DISPINFO_T        dispInfo = {0};
    MMICC_DISPINFO_T        *addData = NULL;
    addData = (MMICC_DISPINFO_T *)SCI_ALLOC(sizeof(MMICC_DISPINFO_T));
	SCI_ASSERT(addData != NULL);

    if(TRUE == g_want_mo_call_info.is_want_mo_call)
    {
        telNumLen = g_want_mo_call_info.tele_len;
        MMI_MEMCPY(telNum, telNumLen, g_want_mo_call_info.tele_num,
            g_want_mo_call_info.tele_len, MIN(MMICC_PHONE_NUM_MAX_LEN, telNumLen));
    }
    else
    {
        MMICC_GetWantCallNumStr(telNum, &telNumLen);
    }

    WatchPB_GetDispInfoContent((uint8 *)telNum, telNumLen, &dispWchar, TRUE);
    dispInfo.disp_name_len = MMIAPICOM_Wstrlen(dispWchar);
    MMI_WSTRNCPY(dispInfo.disp_name_buf, MMICC_PHONE_NUM_MAX_LEN + 2,(const wchar*)dispWchar, dispInfo.disp_name_len, dispInfo.disp_name_len);

    *addData = dispInfo;
    if(!MMK_IsOpenWin(WATCHCC_MOCALLING_WIN_ID))
    {
        WatchCC_MoCallingWin_Enter(addData);
    }
    else
    {
        MMK_SendMsg(WATCHCC_MOCALLING_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
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
    uint8                   telNumLen = 0;
    wchar                   dispWchar[MMICC_PHONE_NUM_MAX_LEN + 2]  ={0};
    MMICC_DISPINFO_T        dispInfo = {0};
    MMICC_DISPINFO_T        *addData = NULL;
    uint8                   callIndex = 0;
    MMI_STRING_T            btMsg = {0};

    addData = (MMICC_DISPINFO_T *)SCI_ALLOC(sizeof(MMICC_DISPINFO_T));
    SCI_ASSERT(addData != NULL);

    //协议规定不能同时存在两个MT,如果之前有MO,MO没有接通时会把MO挂断，接通了不会提示MT
    if (MMK_IsOpenWin(WATCHCC_MOCALLING_WIN_ID))
    {
        MMK_CloseWin(WATCHCC_MOCALLING_WIN_ID);
    }

    MMICC_GetCallNumStrByIndex(telNum, &telNumLen, callIndex);
    //get info for display
    dispInfo.tel_num_len = telNumLen;
    WatchPB_GetDispInfoContent((uint8 *)telNum, telNumLen, &dispWchar, FALSE);
    dispInfo.disp_name_len = MMIAPICOM_Wstrlen(dispWchar);
    MMI_WSTRNCPY(dispInfo.disp_name_buf, MMICC_PHONE_NUM_MAX_LEN + 2,(const wchar*)dispWchar, dispInfo.disp_name_len, dispInfo.disp_name_len);

    *addData = dispInfo;
    if(!MMK_IsOpenWin(WATCHCC_MTCALLING_WIN_ID))
    {
        WatchCC_MtCallingWin_Enter(addData);
    }

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
        AdultWatchCOM_QueryWin_2Btn_Enter(dtmf_query_win_id, &promt_string, &button_left_str, &button_right_str, IMAGE_NULL, IMAGE_NULL, HandleSendDTMFQueryWinMsg);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_CC_SENDING_DTMF, &promt_str);
        MMIAPICOM_Wstrncat(promt_string.wstr_ptr, promt_str.wstr_ptr, promt_str.wstr_len);// fix bug1374557
        MMIAPICOM_Wstrncat(promt_string.wstr_ptr, alt, 1);
        MMIAPICOM_Wstrncat(promt_string.wstr_ptr, dtmf_str.wstr_ptr, dtmf_str.wstr_len);
        promt_string.wstr_len = MIN(dtmf_str.wstr_len+promt_str.wstr_len+1, 100);

        if (MMK_IsOpenWin(alert_win_id))
        {
            MMK_CloseWin(alert_win_id);
        }
        Adult_WatchCOM_NoteWin_2Line_Toast_Enter(alert_win_id, &promt_string, NULL, HandleSendingDTMFNoteWinMsg);
    }
}

/*****************************************************************************/
//  Description : update call status(button and time)
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void  CC_UpdateWatchCallStatus(void)
{
    MMI_WIN_ID_T    winId = WATCHCC_CONNECTED_WIN_ID;

    if(TRUE == CC_CurrentRoleIsPhone())
    {
        if(!MMICC_IsCurrentCallIDValid())
        {
            return;
        }
    }

    if(!MMK_IsOpenWin(winId) )
    {
        WatchCC_ConnectedWin_Enter();
    }
    else
    {
        MMK_SendMsg(winId, MSG_FULL_PAINT, PNULL);
    }
}

/*****************************************************************************/
//  Description : get mo call error text by type
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T CC_GetErrorTextByType(MMICC_MO_CALL_ERROR_TYPE_E type)
{
    MMI_TEXT_ID_T textId = TXT_CC_OPER_FAIL;

    if(type >= CC_MO_ERROR_TYPE_MAX)
    {
        return textId;
    }

    switch(type)
    {
        case CC_MO_DIAL_FAILED:
        {
            textId = TXT_DIAL_FAILED;
            break;
        }
        case CC_MO_EMPTY_NUM:
        {
            textId = TXT_CL_EMPTY_NUM;
            break;
        }
        case CC_MO_IN_FLY_MODE:
        {
            textId = TXT_CC_NOTE_FLYMODE;
            break;
        }
        case CC_MO_IS_BARRING_NUM:
        {
            textId = TXT_CC_CANNOT_CALL;
            break;
        }
        case CC_MO_NET_UNAVAIL:
        {
            textId = TXT_CC_NOTE_NET_FAIL;
            break;
        }
        case CC_MO_INVALID_NUM:
        {
            textId = TXT_CC_INVALID_NUM;
            break;
        }
        case CC_MO_NO_SIM:
        {
            textId = TXT_CC_NOTE_INSERT_SIM;
            break;
        }
        case CC_MO_FDN_ONLY:
        {
            textId = TXT_CC_FDN_ONLY;
            break;
        }
        case CC_MO_NO_IP_NUM:
        {
            textId = TXT_CC_NO_IP_NUM;
            break;
        }
        case CC_MO_CMD_NO_SUPPORT:
        {
            textId = TXT_COMMON_CMD_NO_SUPPORT;
            break;
        }
        default:
        {
            textId = TXT_CC_OPER_FAIL;
            break;
        }
    }
    return textId;
}

/*****************************************************************************/
//  Description : get mo call error image by type
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_IMAGE_ID_T CC_GetErrorImageByType(MMICC_MO_CALL_ERROR_TYPE_E type)
{
    MMI_IMAGE_ID_T imageId = IMAGE_CALL_NOTIFY_ERROR;

    if(type >= CC_MO_ERROR_TYPE_MAX)
    {
        return imageId;
    }

    switch(type)
    {
        case CC_MO_IN_FLY_MODE:
        {
            imageId = IMAGE_CALL_NOTIFY_FLYMODE;
            break;
        }
        case CC_MO_NET_UNAVAIL:
        {
            imageId = IMAGE_CALL_NOTIFY_CHEAK_NETWORK;
            break;
        }
        case CC_MO_NO_SIM:
        {
            imageId = IMAGE_CALL_NOTIFY_CHEAK_SIM;
            break;
        }
        case CC_MO_DIAL_FAILED:
        case CC_MO_EMPTY_NUM:
        case CC_MO_IS_BARRING_NUM:
        case CC_MO_INVALID_NUM:
        case CC_MO_FDN_ONLY:
        case CC_MO_NO_IP_NUM:
        case CC_MO_CMD_NO_SUPPORT:
        {
            imageId = IMAGE_CALL_NOTIFY_ERROR;
            break;
        }
        default:
        {
            imageId = IMAGE_CALL_NOTIFY_ERROR;
            break;
        }
    }
    return imageId;
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
            imageId = IMAGE_ADULT_CALL_STATUS_HD;
        }
        else
        {
            imageId = IMAGE_ADULT_CALL_STATUS_VOLTE;
        }
        GUIRES_DisplayImg(&startPoint, PNULL, PNULL, win_id, imageId, &devInfo);
    }
}
#endif

/*****************************************************************************/
//  Description : draw audio device icon
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CC_DrawAudioDevBtn(MMI_WIN_ID_T win_id)
{
#ifdef BLUETOOTH_SUPPORT
    BT_ADDRESS       address = {0};
    TRACE_APP_CALLCTRL("is_exist_bt_headset = %d, current_dev = %d", MMIAPIBT_GetActivatedHS(&address), MMICC_GetCurrentDev());

    if(FALSE == CC_CurrentRoleIsPhone())
    {
        // HFU连接上，再显示通路图标
#ifdef UAL_BT_HFU_SUPPORT
        if (TRUE == ual_bt_hfu_get_connect_status())
        {
            if(MMICC_AUDIO_DEV_BT == MMICC_GetCurrentDev())
            {
                GUIBUTTON_SetVisible(WATCHCC_ROUTE_PHONE_BUTTON_CTRL_ID, FALSE, FALSE);
                GUIBUTTON_SetVisible(WATCHCC_ROUTE_WATCH_BUTTON_CTRL_ID, TRUE, FALSE);
            }
            else
            {
                GUIBUTTON_SetVisible(WATCHCC_ROUTE_PHONE_BUTTON_CTRL_ID, TRUE, FALSE);
                GUIBUTTON_SetVisible(WATCHCC_ROUTE_WATCH_BUTTON_CTRL_ID, FALSE, FALSE);
            }
        }
        else
        {
            GUIBUTTON_SetVisible(WATCHCC_ROUTE_PHONE_BUTTON_CTRL_ID, FALSE, FALSE);
            GUIBUTTON_SetVisible(WATCHCC_ROUTE_WATCH_BUTTON_CTRL_ID, FALSE, FALSE);
        }
#endif
        GUIBUTTON_SetVisible(WATCHCC_ROUTE_EARPHONE_BUTTON_CTRL_ID, FALSE, FALSE);
    }
    else
    {
        if (MMIAPIBT_GetActivatedHS(&address))
        {
            if(MMICC_AUDIO_DEV_HF == MMICC_GetCurrentDev())
            {
                GUIBUTTON_SetVisible(WATCHCC_ROUTE_EARPHONE_BUTTON_CTRL_ID, FALSE, FALSE);
                GUIBUTTON_SetVisible(WATCHCC_ROUTE_WATCH_BUTTON_CTRL_ID, TRUE, FALSE);
            }
            else
            {
                GUIBUTTON_SetVisible(WATCHCC_ROUTE_EARPHONE_BUTTON_CTRL_ID, TRUE, FALSE);
                GUIBUTTON_SetVisible(WATCHCC_ROUTE_WATCH_BUTTON_CTRL_ID, FALSE, FALSE);
            }
        }
        else
        {
            GUIBUTTON_SetVisible(WATCHCC_ROUTE_EARPHONE_BUTTON_CTRL_ID, FALSE, FALSE);
            GUIBUTTON_SetVisible(WATCHCC_ROUTE_WATCH_BUTTON_CTRL_ID, FALSE, FALSE);
        }
        GUIBUTTON_SetVisible(WATCHCC_ROUTE_PHONE_BUTTON_CTRL_ID, FALSE, FALSE);
    }
#else
    GUIBUTTON_SetVisible(WATCHCC_ROUTE_EARPHONE_BUTTON_CTRL_ID, FALSE, FALSE);
    GUIBUTTON_SetVisible(WATCHCC_ROUTE_WATCH_BUTTON_CTRL_ID, FALSE, FALSE);
    GUIBUTTON_SetVisible(WATCHCC_ROUTE_PHONE_BUTTON_CTRL_ID, FALSE, FALSE);
#endif
    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
}

/*****************************************************************************/
//  Description : handle set audio device button
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CC_HandleSetAudioDevButton(MMI_WIN_ID_T win_id)
{
#ifdef BLUETOOTH_SUPPORT
    BT_ADDRESS      address = {0};

    TRACE_APP_CALLCTRL("is_exist_bt_headset = %d, current_dev = %d", MMIAPIBT_GetActivatedHS(&address), MMICC_GetCurrentDev());
    if(FALSE == CC_CurrentRoleIsPhone())
    {
        if(MMICC_AUDIO_DEV_BT == MMICC_GetCurrentDev())
        {
            GUIBUTTON_SetVisible(WATCHCC_ROUTE_WATCH_BUTTON_CTRL_ID, FALSE, FALSE);
            GUIBUTTON_SetVisible(WATCHCC_ROUTE_PHONE_BUTTON_CTRL_ID, TRUE, FALSE);
            MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_DISABLE, MMICC_AUDIO_DEV_BT);
        }
        else
        {
            GUIBUTTON_SetVisible(WATCHCC_ROUTE_WATCH_BUTTON_CTRL_ID, TRUE, FALSE);
            GUIBUTTON_SetVisible(WATCHCC_ROUTE_PHONE_BUTTON_CTRL_ID, FALSE, FALSE);
            MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_ENABLE, MMICC_AUDIO_DEV_BT);
        }
        GUIBUTTON_SetVisible(WATCHCC_ROUTE_EARPHONE_BUTTON_CTRL_ID, FALSE, FALSE);
    }
    else
    {
        if(MMIAPIBT_GetActivatedHS(&address))
        {
            if(MMICC_AUDIO_DEV_BT == MMICC_GetCurrentDev())
            {
                GUIBUTTON_SetVisible(WATCHCC_ROUTE_EARPHONE_BUTTON_CTRL_ID, FALSE, FALSE);
                GUIBUTTON_SetVisible(WATCHCC_ROUTE_WATCH_BUTTON_CTRL_ID, TRUE, FALSE);
                //MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_DISABLE, MMICC_AUDIO_DEV_BT);
                MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_ENABLE, MMICC_AUDIO_DEV_HF);
            }
            else
            {
                GUIBUTTON_SetVisible(WATCHCC_ROUTE_EARPHONE_BUTTON_CTRL_ID, TRUE, FALSE);
                GUIBUTTON_SetVisible(WATCHCC_ROUTE_WATCH_BUTTON_CTRL_ID, FALSE, FALSE);
                MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_ENABLE, MMICC_AUDIO_DEV_BT);
            }
        }
        GUIBUTTON_SetVisible(WATCHCC_ROUTE_PHONE_BUTTON_CTRL_ID, FALSE, FALSE);
    }
#endif
    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
}

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
        if (0 == CC_INSTANCE.waiting_call_ring_timer)
        {
            CC_INSTANCE.waiting_call_ring_timer = MMK_CreateWinTimer(WATCHCC_MTCALLING_WIN_ID, CC_WATCH_WAITING_CALL_PERIOD, FALSE);
        }
    }
    else
    {
        MMIAPICC_StartRingOrVibrate(FALSE);
        MMICC_SetMtPresetCodecRoute(0);
    }
    return TRUE;
}

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
    MMI_WIN_ID_T    alert_win_id = MMICC_SEND_DTMF_WIN_ID;
    MMI_STRING_T    alert_str = {0};
    wchar           temp_alert_str[CC_MAX_TELE_NUM_LEN + 1] = {0};
    MMI_STRING_T    promt_str = {0};
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
//  Description : handle mute button
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CC_HandleMuteButton(MMI_WIN_ID_T win_id)
{
    TRACE_APP_CALLCTRL("is_mute = %d", MMICC_IsCurrentCallMute());
    if(MMICC_IsCurrentCallMute())
    {
        //取消静音
        MMICC_SetCurrentCallMute(FALSE);
        GUIBUTTON_SetVisible(WATCHCC_UNMUTE_BUTTON_CTRL_ID, TRUE, FALSE);
        GUIBUTTON_SetVisible(WATCHCC_MUTE_BUTTON_CTRL_ID, FALSE, FALSE);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
    }
    else
    {
        //静音
        MMICC_SetCurrentCallMute(TRUE);
        GUIBUTTON_SetVisible(WATCHCC_MUTE_BUTTON_CTRL_ID, TRUE, FALSE);
        GUIBUTTON_SetVisible(WATCHCC_UNMUTE_BUTTON_CTRL_ID, FALSE, FALSE);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
    }
}

/*****************************************************************************/
//  Description : reset timer for auto close volume win
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CC_ResetVolumeWinTimer(MMI_WIN_ID_T win_id)
{
    if (g_call_volume_end_timer != 0)
    {
        MMK_StopTimer(g_call_volume_end_timer);
        g_call_volume_end_timer = 0;
    }
    g_call_volume_end_timer = MMK_CreateWinTimer(win_id, CC_WATCH_VOL_CLOSE_TIME, FALSE);
}

/*****************************************************************************/
//  Discription: draw query win
//  Global resource dependence:
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void DrawMoCallQueryWin(void)
{
    GUI_RECT_T          dispRect   = WATCH_MO_QUERY_STR_DISP_RECT;
    GUI_LCD_DEV_INFO    devInfo    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_STRING_T        strData    = {0};
    GUISTR_STYLE_T      styleInfo  = {0};
    GUISTR_STATE_T      state      = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK;
    MMI_TEXT_ID_T       callMode   = TXT_NULL;
    GUI_RECT_T          btnRectL    = WATCH_BTN_TWO_LEFT_RECT;
    GUI_RECT_T          btnRectR    = WATCH_BTN_TWO_RIGHT_RECT;

    styleInfo.align    = ALIGN_HVMIDDLE;
    styleInfo.font     = WATCH_STRING_FONT_MIDDLE;
    styleInfo.font_color = MMI_WHITE_COLOR ;

    if(FALSE == CC_CurrentRoleIsPhone())
    {
        callMode = TXT_CC_CALL_IN_BT_MODE;
    }
    else
    {
        callMode = TXT_CC_CALL_IN_SIM_MODE;
    }
    MMI_GetLabelTextByLang(callMode, &strData);
    GUISTR_DrawTextToLCDInRect(&devInfo, &dispRect, &dispRect, &strData, &styleInfo, state, GUISTR_TEXT_DIR_AUTO);
    GUIBUTTON_SetRect(WATCHCC_BACK_BUTTON_CTRL_ID, &btnRectL);
    GUIBUTTON_SetRect(WATCHCC_DIAL_BUTTON_CTRL_ID, &btnRectR);
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
    GUISTR_STATE_T      state       = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE;
    GUI_LCD_DEV_INFO    devInfo     = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T          nameDispRect   = WATCH_MO_NAME_DISP_RECT;
    GUISTR_STYLE_T      nameStyleInfo  = {0};
    GUI_RECT_T          stateRect   = WATCH_CALL_STATE_DISP_RECT;
    GUI_RECT_T          btnRectTop  = WATCH_CALL_AUDIO_ROUTE_RECT;
    GUI_RECT_T          btnRectL    = WATCH_BTN_LEFT_RECT;
    GUI_RECT_T          btnRectM    = WATCH_BTN_MIDDLE_RECT;
    GUI_RECT_T          btnRectR    = WATCH_BTN_RIGHT_RECT;
    GUI_RECT_T          btnRectD    = WATCH_BTN_SINGLE_RECT;
    MMI_IMAGE_ID_T      callModeImg = IMAGE_NULL;
    GUI_POINT_T         start_point = WATCH_CALL_MODE_START_POINT;

    WATCHCOM_DisplayBackground(winId);

    if(NULL != contact_info)
    {
        // 显示名称
        dispStr.wstr_ptr = &(contact_info->disp_name_buf);
        dispStr.wstr_len= contact_info->disp_name_len;
        nameStyleInfo.align    = ALIGN_HMIDDLE;
        nameStyleInfo.font     = WATCH_STRING_FONT_MIDDLE;
        nameStyleInfo.font_color = MMI_WHITE_COLOR ;
        if (GUISTR_GetStringWidth(&nameStyleInfo, &dispStr, state) > (nameDispRect.right - nameDispRect.left))
        {
            nameStyleInfo.font = WATCH_STRING_FONT_SMALL;
        }
        GUISTR_DrawTextToLCDInRect(&devInfo, &nameDispRect, &nameDispRect, &dispStr, &nameStyleInfo, state, GUISTR_TEXT_DIR_AUTO);
        // 显示当前通话模式
        if(FALSE == CC_CurrentRoleIsPhone())
        {
            callModeImg = IMAGE_ADULT_CALL_MODE_BT;
        }
        else
        {
            callModeImg = IMAGE_ADULT_CALL_MODE_SIM;
        }
        GUIRES_DisplayImg(&start_point, PNULL, PNULL, winId, callModeImg, &devInfo);
        // 显示呼叫状态
        MMI_GetLabelTextByLang(TXT_CC_CALL_OUTGOING, &stateStr);
        GUILABEL_SetRect(labelCtrlId, &stateRect, FALSE);
        GUILABEL_SetText(labelCtrlId, &stateStr, FALSE);
        GUILABEL_SetFont(labelCtrlId, WATCH_STRING_FONT_SMALL, MMI_WHITE_COLOR);
        // 显示button图片
        GUIBUTTON_SetRect(WATCHCC_ROUTE_EARPHONE_BUTTON_CTRL_ID, &btnRectTop);
        GUIBUTTON_SetRect(WATCHCC_ROUTE_WATCH_BUTTON_CTRL_ID, &btnRectTop);
        GUIBUTTON_SetRect(WATCHCC_ROUTE_PHONE_BUTTON_CTRL_ID, &btnRectTop);
        GUIBUTTON_SetRect(WATCHCC_MUTE_DISABLE_BUTTON_CTRL_ID, &btnRectL);
        GUIBUTTON_SetRect(WATCHCC_VOLUME_BUTTON_CTRL_ID, &btnRectM);
        GUIBUTTON_SetRect(WATCHCC_DIALPAD_BUTTON_CTRL_ID, &btnRectR);
        GUIBUTTON_SetRect(WATCHCC_HANGUP_BUTTON_CTRL_ID, &btnRectD);
    }
}

/*****************************************************************************/
//  Discription: draw call connected win
//  Global resource dependence:
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void DrawVoiceCallConnectedWin(void)
{
    MMI_WIN_ID_T        winId = WATCHCC_CONNECTED_WIN_ID;
    MMI_STRING_T        dispStr    = {0};
    GUISTR_STATE_T      state       = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE;
    GUI_LCD_DEV_INFO    devInfo     = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T          nameDispRect   = WATCH_MO_NAME_DISP_RECT;
    GUISTR_STYLE_T      nameStyleInfo  = {0};
    GUI_RECT_T          btnRectTop  = WATCH_CALL_AUDIO_ROUTE_RECT;
    GUI_RECT_T          btnRectL    = WATCH_BTN_LEFT_RECT;
    GUI_RECT_T          btnRectM    = WATCH_BTN_MIDDLE_RECT;
    GUI_RECT_T          btnRectR    = WATCH_BTN_RIGHT_RECT;
    GUI_RECT_T          btnRectD    = WATCH_BTN_SINGLE_RECT;
    MMI_IMAGE_ID_T      callModeImg = IMAGE_NULL;
    GUI_POINT_T         start_point = WATCH_CALL_MODE_START_POINT;
    uint8               telNum[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
    uint8               telNumLen = 0;
    uint8               callIndex = 0;
    wchar               dispWchar[MMICC_PHONE_NUM_MAX_LEN + 2]  ={0};

    WATCHCOM_DisplayBackground(winId);
    //current mpty status
    if(TRUE == MMICC_IsExistMPTY())
    {
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CC_MULTICALL, &dispStr);
    }
    else
    {
        MMICC_GetCallNumStrByIndex(telNum, &telNumLen, callIndex); 
        WatchPB_GetDispInfoContent((uint8 *)telNum, telNumLen, &dispWchar, TRUE);
        dispStr.wstr_ptr = dispWchar;
        dispStr.wstr_len= MMIAPICOM_Wstrlen(dispWchar);
        MMI_WSTRNCPY(dispStr.wstr_ptr, MMICC_PHONE_NUM_MAX_LEN + 2, (const wchar*)dispWchar, dispStr.wstr_len, dispStr.wstr_len);
    }
    //current hold status
    if(MMICC_IsExistHoldCall())
    {
        GUIBUTTON_SetVisible(WATCHCC_HANGUP_BUTTON_CTRL_ID, FALSE, TRUE);
        GUIBUTTON_SetVisible(WATCHCC_HANGUP_DISABLE_BUTTON_CTRL_ID, TRUE, TRUE);
    }
    else
    {
        GUIBUTTON_SetVisible(WATCHCC_HANGUP_BUTTON_CTRL_ID, TRUE, TRUE);
        GUIBUTTON_SetVisible(WATCHCC_HANGUP_DISABLE_BUTTON_CTRL_ID, FALSE, TRUE);
    }
    //current mute status
    if(MMICC_IsCurrentCallMute())
    {
        GUIBUTTON_SetVisible(WATCHCC_UNMUTE_BUTTON_CTRL_ID, FALSE, TRUE);
        GUIBUTTON_SetVisible(WATCHCC_MUTE_BUTTON_CTRL_ID, TRUE, TRUE);
    }
    else
    {
        GUIBUTTON_SetVisible(WATCHCC_UNMUTE_BUTTON_CTRL_ID, TRUE, TRUE);
        GUIBUTTON_SetVisible(WATCHCC_MUTE_BUTTON_CTRL_ID, FALSE, TRUE);
    }
    // 显示名称
    nameStyleInfo.align    = ALIGN_HMIDDLE;
    nameStyleInfo.font     = WATCH_STRING_FONT_MIDDLE;
    nameStyleInfo.font_color = MMI_WHITE_COLOR ;
    if (GUISTR_GetStringWidth(&nameStyleInfo, &dispStr, state) > (nameDispRect.right - nameDispRect.left))
    {
        nameStyleInfo.font = WATCH_STRING_FONT_SMALL;
    }
    GUISTR_DrawTextToLCDInRect(&devInfo, &nameDispRect, &nameDispRect, &dispStr, &nameStyleInfo, state, GUISTR_TEXT_DIR_AUTO);
    // 显示当前通话模式
    if(FALSE == CC_CurrentRoleIsPhone())
    {
        callModeImg = IMAGE_ADULT_CALL_MODE_BT;
    }
    else
    {
        callModeImg = IMAGE_ADULT_CALL_MODE_SIM;
    }
    GUIRES_DisplayImg(&start_point, PNULL, PNULL, winId, callModeImg, &devInfo);
    // 显示BUTTON图片
    GUIBUTTON_SetRect(WATCHCC_ROUTE_EARPHONE_BUTTON_CTRL_ID, &btnRectTop);
    GUIBUTTON_SetRect(WATCHCC_ROUTE_WATCH_BUTTON_CTRL_ID, &btnRectTop);
    GUIBUTTON_SetRect(WATCHCC_ROUTE_PHONE_BUTTON_CTRL_ID, &btnRectTop);
    GUIBUTTON_SetRect(WATCHCC_MUTE_BUTTON_CTRL_ID, &btnRectL);
    GUIBUTTON_SetRect(WATCHCC_UNMUTE_BUTTON_CTRL_ID, &btnRectL);
    GUIBUTTON_SetRect(WATCHCC_VOLUME_BUTTON_CTRL_ID, &btnRectM);
    GUIBUTTON_SetRect(WATCHCC_DIALPAD_BUTTON_CTRL_ID, &btnRectR);
    GUIBUTTON_SetRect(WATCHCC_HANGUP_BUTTON_CTRL_ID, &btnRectD);
    GUIBUTTON_SetRect(WATCHCC_HANGUP_DISABLE_BUTTON_CTRL_ID, &btnRectD);
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
    MMI_CTRL_ID_T       labelCtrlId = WATCHCC_CALLSTATE_LABEL_CTRL_ID;
    GUI_LCD_DEV_INFO    devInfo    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUISTR_STATE_T      state      = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE;
    GUISTR_STYLE_T      styleInfo  = {0};
    GUISTR_STYLE_T      nameStyleInfo  = {0};
    MMI_STRING_T        strMt      = {0};
    MMI_STRING_T        dispName   = {0};
    GUI_RECT_T          nameDispRect = WATCH_MT_NAME_DISP_RECT;
    GUI_RECT_T          strMtRect    = WATCH_CALL_STATE_DISP_RECT;
    GUI_RECT_T          btnRectL    = WATCH_BTN_TWO_LEFT_RECT;
    GUI_RECT_T          btnRectR    = WATCH_BTN_TWO_RIGHT_RECT;
    MMI_IMAGE_ID_T      callModeImg = IMAGE_NULL;
    GUI_POINT_T         start_point = WATCH_CALL_MODE_START_POINT;

    WATCHCOM_DisplayBackground(winId);

    if(NULL != contact_info)
    {
        // 显示姓名或号码
        dispName.wstr_ptr = &(contact_info->disp_name_buf);
        dispName.wstr_len= contact_info->disp_name_len;
        MMI_WSTRNCPY(dispName.wstr_ptr, MMICC_PHONE_NUM_MAX_LEN + 2, (const wchar*)contact_info->disp_name_buf, dispName.wstr_len, dispName.wstr_len);
        nameStyleInfo.align    = ALIGN_HMIDDLE;
        nameStyleInfo.font     = WATCH_STRING_FONT_MIDDLE;
        nameStyleInfo.font_color = MMI_WHITE_COLOR ;
        if (GUISTR_GetStringWidth(&nameStyleInfo, &dispName, state) > (nameDispRect.right - nameDispRect.left))
        {
            nameStyleInfo.font = WATCH_STRING_FONT_SMALL;
        }
        GUISTR_DrawTextToLCDInRect(&devInfo, &nameDispRect, &nameDispRect, &dispName, &nameStyleInfo, state, GUISTR_TEXT_DIR_AUTO);
        // 显示当前通话模式
        if(FALSE == CC_CurrentRoleIsPhone())
        {
            callModeImg = IMAGE_ADULT_CALL_MODE_BT;
        }
        else
        {
            callModeImg = IMAGE_ADULT_CALL_MODE_SIM;
        }
        GUIRES_DisplayImg(&start_point, PNULL, PNULL, winId, callModeImg, &devInfo);
        // 显示来电中
        MMI_GetLabelTextByLang(TXT_CC_CALL_INCOMING, &strMt);
        GUILABEL_SetRect(labelCtrlId, &strMtRect, FALSE);
        GUILABEL_SetText(labelCtrlId, &strMt, FALSE);
        GUILABEL_SetFont(labelCtrlId, WATCH_STRING_FONT_SMALL, MMI_WHITE_COLOR);
        // 显示BUTTON图片
        GUIBUTTON_SetRect(WATCHCC_HANGUP_BUTTON_CTRL_ID, &btnRectL);
        GUIBUTTON_SetRect(WATCHCC_DIAL_BUTTON_CTRL_ID, &btnRectR);
    }
}

/*****************************************************************************/
//  Description : handle set volume window messages
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleCcSetVolumeWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            if(TRUE == g_call_vol_auto_end)
            {
                g_call_volume_end_timer = MMK_CreateWinTimer(win_id, CC_WATCH_VOL_CLOSE_TIME, FALSE);
            }
            break;
        }
        case MSG_CLOSE_WINDOW:
        {
            g_call_vol_auto_end = FALSE;
            if (g_call_volume_end_timer != 0)
            {
                MMK_StopTimer(g_call_volume_end_timer);
                g_call_volume_end_timer = 0;
            }
            break;
        }
        case MSG_NOTIFY_PRGBOX_CHANGE:
        {
            uint32 volume = *(uint32*)param; //bug 1629191
            TRACE_APP_CALLCTRL("volume = %d", volume);
            MMIAPICC_SetCallVolume(volume);
            break;
        }
        case MSG_TIMER:
        {
            if((g_call_volume_end_timer == *(uint8*)param) && (TRUE == g_call_vol_auto_end))
            {
                MMK_CloseWin(win_id);
            }
            break;
        }
#if 1
        case MSG_APP_UP:
        {
            uint8 volume = MMIAPICC_GetCallVolume();
            volume += 1;
            if(MMISET_VOL_MAX >= volume && MMISET_VOL_ONE <= volume)
            {
                MMIAPICC_SetCallVolume(volume);
                WatchCOM_ProgressWin_SetProgress(win_id, (uint32)volume);
            }
            break;
        }
        case MSG_APP_DOWN:
        {
            uint8 volume = MMIAPICC_GetCallVolume();
            volume -= 1;
            if(MMISET_VOL_MAX >= volume && MMISET_VOL_ONE <= volume)
            {
                MMIAPICC_SetCallVolume(volume);
                WatchCOM_ProgressWin_SetProgress(win_id, (uint32)volume);
            }
            break;
        }
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        {
            MMK_CloseWin(win_id);
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
    MMICC_CALLINFO_T*   contactInfo = (MMICC_CALLINFO_T *)MMK_GetWinAddDataPtr(win_id);

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            break;
        }
        case MSG_CLOSE_WINDOW:
        {
            if (contactInfo != NULL)
            {
                SCI_FREE(contactInfo);
            }
            break;
        }
        case MSG_FULL_PAINT:
        {
            WATCHCOM_DisplayBackground(win_id);
            DrawMoCallQueryWin();
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
                        WatchAPICC_MakeCall(MN_DUAL_SYS_1, contactInfo->tele_num, contactInfo->tele_len,
                                            PNULL, PNULL, CC_SIM1_CALL, CC_CALL_NORMAL_CALL, PNULL);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
            break;
        }
#endif
#if 1
        case MSG_APP_OK:
        {
            WatchCC_MoCallQueryWin_Exit();
            break;
        }
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        {
            //make call
            WatchAPICC_MakeCall(MN_DUAL_SYS_1, contactInfo->tele_num, contactInfo->tele_len,
                                            PNULL, PNULL, CC_SIM1_CALL, CC_CALL_NORMAL_CALL, PNULL);
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
//  Description : click on hangup button
//  Global resource dependence :
//  Author: zirui.li
//  Note:
/*****************************************************************************/
LOCAL void HandleCcMoCallingClickonHangupBtn (void)
{
    if(TRUE == MMIAPICC_IsInState(CC_MO_CONNECTING_STATE))
    {
        WatchAPICC_ReleaseCurrentCall();
    }
    else
    {
#ifdef BBM_ROLE_SUPPORT
        if(FALSE == CC_CurrentRoleIsPhone())
        {
            BbmSwitchToBle();
        }
#endif
        WatchCC_MoCallingWin_Exit();
    }
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
            CC_DrawAudioDevBtn(win_id);
            break;
        }
        case MSG_CLOSE_WINDOW:
        {
            WatchCC_SetVolumeWin_Exit();
            Adult_WatchCC_DialpadWinInCall_Exit();
            SCI_MEMSET(&g_want_mo_call_info, 0x00, sizeof(MMICC_WANT_MO_CALL_T));
            if (pInfo != NULL)
            {
                SCI_FREE(pInfo);
            }
            break;
        }
        case MSG_FULL_PAINT:
        {
            DrawMoCallingWin(pInfo);
#ifdef MMI_VOLTE_SUPPORT
        if(TRUE == CC_CurrentRoleIsPhone())
        {
            CC_DrawVolteIcon(win_id);
        }
#endif
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
                    case WATCHCC_ROUTE_EARPHONE_BUTTON_CTRL_ID:
                    case WATCHCC_ROUTE_WATCH_BUTTON_CTRL_ID:
                    case WATCHCC_ROUTE_PHONE_BUTTON_CTRL_ID:
                    {
                        //set audio route
                        CC_HandleSetAudioDevButton(win_id);
                        break;
                    }
                    case WATCHCC_HANGUP_BUTTON_CTRL_ID:
                    {
                        HandleCcMoCallingClickonHangupBtn();  //Bug1668319
                        break;
                    }
                    case WATCHCC_VOLUME_BUTTON_CTRL_ID:
                    {
                        //set volume
                        WatchCC_SetVolumeWin_Enter();
                        break;
                    }
                    case WATCHCC_DIALPAD_BUTTON_CTRL_ID:
                    {
                        Adult_WatchCC_DialpadWinInCall_Enter();
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
            break;
        }
#endif
#if 1
        case MSG_APP_UP:
        {
            //set audio route
            CC_HandleSetAudioDevButton(win_id);
            break;
        }
        case MSG_APP_DOWN:
        {
            WatchCC_SetVolumeWin_Enter();
            break;
        }
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        {
            Adult_WatchCC_DialpadWinInCall_Enter();
            break;
        }
        case MSG_BT_CANCEL_OUTGOING_CALL://bug1615646
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        {
            HandleCcMoCallingClickonHangupBtn(); //Bug1668319
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
    GUI_LCD_DEV_INFO            lcdDevInfo = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

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
            CC_DrawAudioDevBtn(win_id);
            if(0 ==  CC_INSTANCE.call_time_count_timer)
            {
                CC_INSTANCE.call_time_count_timer = MMK_CreateWinTimer(win_id, CC_TIMER_OUT, TRUE );
#ifndef WIN32
                SCI_MMI_EnableDeepSleep(FALSE);//bug1921538
#endif
            }
            // 显示通话时长
            CC_TimeCountToStr(&timeStr, g_call_time_count);
            GUILABEL_SetRect(timeLabelId, &timeRect, FALSE);
            GUILABEL_SetText(timeLabelId, &timeStr, FALSE);
            GUILABEL_SetFont(timeLabelId, WATCH_STRING_FONT_SMALL, MMI_WHITE_COLOR);
            if (MMICC_IsSendingStoredDtmf())
            {
                CC_OpenSendDtmfPromptWin();
            }
            break;
        }
        case MSG_CC_CONNECT_OPEN_WINDOW:
            if (MMICC_IsSendingStoredDtmf())
            {
                CC_OpenSendDtmfPromptWin();
            }
            break;
        case MSG_CLOSE_WINDOW:
        {
            WatchCC_SetVolumeWin_Exit();
            Adult_WatchCC_DialpadWinInCall_Exit();
            if (CC_INSTANCE.call_time_count_timer != 0)
            {
                MMK_StopTimer(CC_INSTANCE.call_time_count_timer);
                CC_INSTANCE.call_time_count_timer = 0;
#ifndef WIN32
                SCI_MMI_EnableDeepSleep(TRUE);
#endif
            }
            g_call_time_count = 0;
            break;
        }
        case MSG_FULL_PAINT:
        {
            DrawVoiceCallConnectedWin();
#ifdef MMI_VOLTE_SUPPORT
            if(TRUE == CC_CurrentRoleIsPhone())
            {
                CC_DrawVolteIcon(win_id);
            }
#endif
            break;
        }
        case MSG_TIMER:
        {
            if(CC_INSTANCE.call_time_count_timer == *(uint8*)param)
            {
                if(FALSE == CC_CurrentRoleIsPhone())
                {
                    uint32 start_time = 0;
                    uint32 cur_time = SCI_GetTickCount();
                    ual_bt_get_current_call_start_time(&start_time);
                    g_call_time_count = (cur_time - start_time)/1000;
                }
                else
                {
                    MMICC_SetCallTimeCount(MMICC_GetCallTimeCount() + 1);//s_call_context.call_time_count++;
                    if ((!MMICC_IsExistOperatedCall()) && (CC_CALL_STATE == MMICC_GetCurrentCallStatus()))
                    {
                        {
                            g_call_time_count = MMICC_GetCurrentCallTime();
                        }
                    }
                }
                if (MMK_IsFocusWin(win_id))
                {
                    CC_TimeCountToStr(&timeStr, g_call_time_count);
                    //fresh list time
                    GUILABEL_SetText(timeLabelId, &timeStr, TRUE);
                }
            }
            else if(CC_INSTANCE.dtmf_timer_id == *(uint8*)param)//fix bug1366932
            {
                //send dtmf if exist extension number
                TRACE_APP_CALLCTRL("send stored dtmf");
                if(CC_RESULT_SUCCESS != CC_SendStoredDtmf())
                {
                    //SCI_TRACE_LOW:"ccapp.c:CC_SetupCompleteCnf() send dtmf failure"
                    TRACE_APP_CALLCTRL("ccapp.c:CC_SetupCompleteCnf() send dtmf failure");
                }

                if (MMICC_IsSendingStoredDtmf())
                {
                    MMK_StopTimer(CC_INSTANCE.dtmf_timer_id);
                    CC_INSTANCE.dtmf_timer_id = MMI_NONE_TIMER_ID;
                    CC_StartDtmfWaitingTimer();
                }
            }
            break;
        }
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
        {
            if(PNULL != param)
            {
                switch(((MMI_NOTIFY_T*) param)->src_id)
                {
                    case WATCHCC_ROUTE_EARPHONE_BUTTON_CTRL_ID:
                    case WATCHCC_ROUTE_WATCH_BUTTON_CTRL_ID:
                    case WATCHCC_ROUTE_PHONE_BUTTON_CTRL_ID:
                    {
                        //set audio route
                        CC_HandleSetAudioDevButton(win_id);
                        break;
                    }
                    case WATCHCC_MUTE_BUTTON_CTRL_ID:
                    case WATCHCC_UNMUTE_BUTTON_CTRL_ID:
                    {
                        CC_HandleMuteButton(win_id);
                        break;
                    }
                    case WATCHCC_HANGUP_BUTTON_CTRL_ID:
                    {
                        WatchAPICC_ReleaseCurrentCall();
                        break;
                    }
                    case WATCHCC_VOLUME_BUTTON_CTRL_ID:
                    {
                        //set volume
                        WatchCC_SetVolumeWin_Enter();
                        break;
                    }
                    case WATCHCC_DIALPAD_BUTTON_CTRL_ID:
                    {
                        Adult_WatchCC_DialpadWinInCall_Enter();
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
            break;
        }
#endif
#if 1
        case MSG_APP_UP:
        {
            //set audio route
            CC_HandleSetAudioDevButton(win_id);
            break;
        }
        case MSG_APP_OK:
        {
            CC_HandleMuteButton(win_id);
            break;
        }
        case MSG_APP_DOWN:
        {
            WatchCC_SetVolumeWin_Enter();
            break;
        }
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        {
            Adult_WatchCC_DialpadWinInCall_Enter();
            break;
        }
        case MSG_BT_CANCEL_CALL:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        {
            WatchAPICC_ReleaseCurrentCall();
            break;
        }
#endif

        case MSG_GET_FOCUS:
        {
            TRACE_APP_CALLCTRL("MSG_GET_FOCUS");
            TRACE_APP_CALLCTRL("is dtmf:%d", MMICC_IsSendingStoredDtmf());
            if (MMICC_IsSendingStoredDtmf())
            {
                CC_OpenSendDtmfPromptWin();
            }

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
    LOCAL BOOLEAN               needPlayRing = FALSE;
    LOCAL uint32                callRingTimes = 0;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            uint8 is_phone = 1;

            is_phone = ual_bt_get_product_role();//fix bug1976694

            if (is_phone)
            {
               if(MMICC_IsExistIncommingCall())
               {
                   needPlayRing = TRUE;
               }
            }
#ifdef UAL_BT_HFU_SUPPORT
            else
            {
                if(1 == ual_bt_get_total_call_num()) //fix bug1937759
                {
                    needPlayRing = TRUE;
                }
            }
#endif
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
            break;
        }
        case MSG_CLOSE_WINDOW:
        {
            if (pInfo != NULL)
            {
                SCI_FREE(pInfo);
            }
            if (CC_INSTANCE.waiting_call_ring_timer != 0)
            {
                MMISRVAUD_StopTone(MMISRVAUD_TYPE_TONE_GENERIC);
                MMK_StopTimer(CC_INSTANCE.waiting_call_ring_timer);
                CC_INSTANCE.waiting_call_ring_timer = 0;
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
            DrawMtCallingWin(pInfo);
#ifdef MMI_VOLTE_SUPPORT
            if(TRUE == CC_CurrentRoleIsPhone())
            {
                CC_DrawVolteIcon(win_id);
            }
#endif
            //自动接听功能，工程模式里面需要进行设置
            CC_AutoAnswerIncomingCall();
            break;
        }
        case MSG_TIMER:
        {
            if (CC_INSTANCE.waiting_call_ring_timer == *(uint8*)param)
            {
                if (callRingTimes <= CC_WATCH_WAITING_CALL_PLAY_TIMER)
                {
                    MMIAPISET_PlayRing(MMIAPICC_GetCallDualSys(), FALSE,0,1,MMISET_RING_TYPE_CALL_IN_CALL,PNULL);
                    MMK_StartWinTimer( win_id, CC_INSTANCE.waiting_call_ring_timer, CC_WATCH_WAITING_CALL_PERIOD, FALSE);
                    callRingTimes ++;
                }
                else
                {
                    MMK_StopTimer(CC_INSTANCE.waiting_call_ring_timer);
                    CC_INSTANCE.waiting_call_ring_timer = 0;
                    callRingTimes = 0;
                }
            }
            break;
        }

        case MSG_BT_ACCEPT_INCOMING_CALL:
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
                        if(MMICC_AnswerCall())
                        {
                            TRACE_APP_CALLCTRL("answer call success!");
                        }
                        break;
                    }
                    case WATCHCC_HANGUP_BUTTON_CTRL_ID:
                    {
                        WatchAPICC_ReleaseCurrentCall();
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
            break;
        }
#endif
        case MSG_BT_REJECT_INCOMING_CALL:
        case MSG_APP_OK:
        {
            WatchAPICC_ReleaseCurrentCall();
            break;
        }
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
        {
            if(MMICC_AnswerCall())
            {
                TRACE_APP_CALLCTRL("answer call success!");
            }
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
        MMICC_ClearStoredDtmf();
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
    MMI_WIN_ID_T alert_win_id = WATCHCC_SEND_DTMF_NOTE_WIN_ID;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            MMK_StopTimer(CC_INSTANCE.dtmf_timer_id);
            CC_INSTANCE.dtmf_timer_id = MMI_NONE_TIMER_ID;
            break;
        }

        case MSG_APP_OK:
        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
            {
                WATCH_SOFTKEY_TEXT_ID_T softkey_text = {TXT_NULL, TXT_NULL, TXT_NULL};
                CC_DTMF_STATE_T dtmf_info = *MMICC_GetStoredDtmfInfo();
                MMI_STRING_T dtmf_str = {0};
                MMI_STRING_T promt_str = {0};
                MMI_STRING_T    promt_string = {0};
                wchar temp_alert_wchar[CC_MAX_TELE_NUM_LEN + 1] = {0};
                wchar           temp_wchar[100] = {0};
                wchar alt[2] = {'\n','\0'};

                dtmf_str.wstr_ptr = temp_alert_wchar;
                dtmf_str.wstr_len = dtmf_info.dtmf_len - 1;
                MMI_STRNTOWSTR(dtmf_str.wstr_ptr,dtmf_str.wstr_len, (uint8 *)&(dtmf_info.dtmf_str[1]), dtmf_str.wstr_len,dtmf_str.wstr_len);
                MMI_GetLabelTextByLang(TXT_CC_SENDING_DTMF, &promt_str);
                promt_string.wstr_ptr = temp_wchar;

                MMIAPICOM_Wstrncat(promt_string.wstr_ptr, promt_str.wstr_ptr, promt_str.wstr_len);
                MMIAPICOM_Wstrncat(promt_string.wstr_ptr, alt, 1);
                MMIAPICOM_Wstrncat(promt_string.wstr_ptr, dtmf_str.wstr_ptr, dtmf_str.wstr_len);
                promt_string.wstr_len = MIN(dtmf_str.wstr_len+promt_str.wstr_len+1, 100);

                //start dtmf timer
                if (CC_INSTANCE.dtmf_timer_id == MMI_NONE_TIMER_ID)
                {
                    CC_StartDtmfWaitingTimer();
                }

                //open dtmf note win
                MMI_GetLabelTextByLang(TXT_CC_SENDING_DTMF, &promt_str);
                if (MMK_IsOpenWin(alert_win_id))
                {
                    MMK_CloseWin(alert_win_id);
                }
                Adult_WatchCOM_NoteWin_2Line_Toast_Enter(alert_win_id, &promt_string, NULL, HandleSendingDTMFNoteWinMsg);

                //close cur win
                MMK_CloseWin(win_id);
                //send dtmf
                if(CC_RESULT_SUCCESS != CC_SendStoredDtmf())
                {
                    TRACE_APP_CALLCTRL("send dtmf failure!");
                }
                break;
            }
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL://fix bug1374587
        {
            MMICC_ClearStoredDtmf();
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

#define BT_DIALER_CONTROL 1
#ifdef BT_DIALER_CONTROL

uint32 g_btcall_handle = 0;

#ifdef UAL_BT_HFU_SUPPORT
/*****************************************************************************/
//  Description : handle bt call msg
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleBtCallMsg(ual_cms_msg_t msg)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    uint8           volume = 0;

    TRACE_APP_CALLCTRL("msg_id = %d", msg.msg_id);
    volume = MMIAPISET_GetCallVolume();

    if((msg.msg_id >= UAL_BT_AUDIO_CONNECTED_IND) && (msg.msg_id <= UAL_BT_BUILD_MPTY_IND)
        && (msg.msg_id != UAL_BT_DISCONNECT_IND) && (msg.msg_id != UAL_BT_AUDIO_DISCONNECTED_IND))
    {
        //construct audio handle
        if(PNULL == MMICC_GetAppletInstance())
        {
            TRACE_APP_CALLCTRL("MMICC_StartCCApplet, msg_id = %d", msg.msg_id);
            MMICC_StartCCApplet();
            MMIAPICC_SetCallVolume(volume);//Bug 1621372
        }
    }

    switch(msg.msg_id)
    {
        case UAL_BT_SLC_CONNECTED_IND:
        {
            TRACE_APP_CALLCTRL("UAL_BT_SLC_CONNECTED_IND");
            MMIDEFAULT_TurnOnBackLight();
            if(TRUE == g_want_mo_call_info.is_want_mo_call)
            {
                CC_MakeCall_ForNormal(MN_DUAL_SYS_MAX, g_want_mo_call_info.tele_num, g_want_mo_call_info.tele_len);
                SCI_MEMSET(&g_want_mo_call_info, 0x00, sizeof(MMICC_WANT_MO_CALL_T));
            }
            break;
        }
        case UAL_BT_SLC_DISCONNECTED_IND:
        {
            TRACE_APP_CALLCTRL("UAL_BT_SLC_DISCONNECTED_IND");
            WatchCC_MoCallingWin_Exit();
            WatchCC_ConnectedWin_Exit();
            WatchCC_MtCallingWin_Exit();
            //destory audio handle
            if(PNULL != MMICC_GetAppletInstance())
            {
                MMICC_DestroyAudioHandle();
                MMK_PostMsg(MMICC_GetAppletInstance()->caf_applet.app_handle, MSG_CC_STOP_APPLET, PNULL, 0 );
            }
#ifdef BBM_ROLE_SUPPORT
            if(FALSE == CC_CurrentRoleIsPhone())
            {
                BbmSwitchToBle();
            }
#endif
            break;
        }
        case UAL_BT_AUDIO_CONNECTED_IND:
        case UAL_BT_AUDIO_DISCONNECTED_IND:
        {
            TRACE_APP_CALLCTRL("UAL_BT_AUDIO_CONNECTED_IND or UAL_BT_AUDIO_DISCONNECTED_IND");
            MMIDEFAULT_TurnOnBackLight();
            WatchCC_UpdateCurrentCallDev();
            break;
        }
        case UAL_BT_MO_PRE_START_IND:
        {
            TRACE_APP_CALLCTRL("UAL_BT_MO_PRE_START_IND");
            break;
        }
        case UAL_BT_MO_START_IND:
        case UAL_BT_MO_ALERT_IND:
        {
            TRACE_APP_CALLCTRL("UAL_BT_MO_START_IND or UAL_BT_MO_ALERT_IND");
            MMIDEFAULT_TurnOnBackLight();
            CC_OpenWatchMoCallWin();
            break;
        }
        case UAL_BT_MT_START_IND:
        {
            TRACE_APP_CALLCTRL("UAL_BT_MT_START_IND");
            MMIDEFAULT_TurnOnBackLight();
            WATCHRECORD_StopRecordOrPlay();
            CC_OpenWatchMtCallWin();
            MMIUSBSHARE_Disconnect();
            break;
        }
        case UAL_BT_MO_CONNECTED_IND:
        {
            TRACE_APP_CALLCTRL("UAL_BT_MO_CONNECTED_IND");
            CC_UpdateWatchCallStatus();
            WatchCC_MoCallingWin_Exit();
            break;
        }
        case UAL_BT_MT_CONNECTED_IND:
        {
            TRACE_APP_CALLCTRL("UAL_BT_MT_CONNECTED_IND");
            CC_UpdateWatchCallStatus();
            WatchCC_MtCallingWin_Exit();
            break;
        }
        case UAL_BT_HOLD_IND:
        {
            TRACE_APP_CALLCTRL("UAL_BT_HOLD_IND");
            CC_UpdateWatchCallStatus();
            break;
        }
        case UAL_BT_BUILD_MPTY_IND:
        {
            TRACE_APP_CALLCTRL("UAL_BT_BUILD_MPTY_IND");
            CC_UpdateWatchCallStatus();
            break;
        }
        case UAL_BT_DISCONNECT_IND:
        {
            ual_bt_disconnect_call_cause_e cause = UAL_BT_CAUSE_MAX;
            ual_bt_disconnect_call_cause_e *p_body = (ual_bt_disconnect_call_cause_e *)(msg.p_body);
            if(PNULL == p_body)
            {
                TRACE_APP_CALLCTRL("UAL_BT_DISCONNECT_IND, p_body = PNULL");
                break;
            }
            cause = *p_body;
            TRACE_APP_CALLCTRL("UAL_BT_DISCONNECT_IND, cause = %d", cause);
            if(cause == UAL_BT_CAUSE_ERROR_NONE)
            {
                WatchCC_NoteWin_Enter(TXT_CC_NOTE_CALL_END, IMAGE_CALL_NOTIFY_ENDED);
            }
            WatchCC_MoCallingWin_Exit();
            WatchCC_ConnectedWin_Exit();
            WatchCC_MtCallingWin_Exit();
            //destory audio handle
            if(PNULL != MMICC_GetAppletInstance())
            {
                //stop cc applet
                MMICC_DestroyAudioHandle();
                MMK_PostMsg(MMICC_GetAppletInstance()->caf_applet.app_handle, MSG_CC_STOP_APPLET, PNULL, 0 );
            }
#ifdef BBM_ROLE_SUPPORT
            if(FALSE == CC_CurrentRoleIsPhone())
            {
                BbmSwitchToBle();
            }
#endif
            break;
        }
        default:
            break;
    }

    return result;
}
#endif

#ifdef BBM_ROLE_SUPPORT
/*****************************************************************************/
//  Description : handle BBM msg
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CC_HandleBbmMsg(uint32 msg_id, void *param)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    TRACE_APP_CALLCTRL("msg_id = %d", msg_id);

    switch(msg_id)
    {
        case BBM_MSG_BT_ROLE_SWITCHING:
        {
            TRACE_APP_CALLCTRL("BBM_MSG_BT_ROLE_SWITCHING");
            break;
        }
        case BBM_MSG_BT_SWITCH_FAIL:
        {
            TRACE_APP_CALLCTRL("BBM_MSG_BT_SWITCH_FAIL");
            WatchCC_NoteWin_Enter(TXT_CC_NOTE_BTCALL_FAIL, IMAGE_CALL_NOTIFY_CHEAK_BT);
            break;
        }
        case BBM_MSG_BT_SWITCH_SUCESS:
        {
            TRACE_APP_CALLCTRL("BBM_MSG_BT_SWITCH_SUCESS");
            break;
        }
        default:
            break;
    }

    return result;
}
#endif

/*****************************************************************************/
//  Description : register bt call callback
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_App_Init(void)
{
    char  userName[6] = {'C','C','A','P','P'};
#ifdef UAL_BT_HFU_SUPPORT
    if(UAL_BT_REGISTER_SUCC == ual_bt_hfu_register((_ual_cms_client_register_callback)CC_HandleBtCallMsg, &g_btcall_handle))
    {
        TRACE_APP_CALLCTRL("register bt call callback success!");
    }
#endif
#ifdef BBM_ROLE_SUPPORT
    if(BBM_RETURN_SUCESS == BbmRegister(userName, (Bbm_CallBack)CC_HandleBbmMsg))
    {
        TRACE_APP_CALLCTRL("register BBM callback success!");
    }
#endif
}

/*****************************************************************************/
//  Description : get bt call service handle
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC uint32 WatchCC_GetBtCallHandle(void)
{
    return g_btcall_handle;
}

#endif

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

