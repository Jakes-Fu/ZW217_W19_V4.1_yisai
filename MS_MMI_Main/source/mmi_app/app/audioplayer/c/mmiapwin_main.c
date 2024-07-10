/*****************************************************************************
** File Name:      mmiapwin_main.c                                           *
** Author:                                                                   *
** Date:           2012/7/19                                                 *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe audioplayer main win        *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME              DESCRIPTION                              *
** 2012/7/19      aoke.hu           Create                                   *
******************************************************************************/

#define _MMIAPWIN_MAIN_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_audioplayer_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmipub.h"
#include "guilcd.h"
#include "mmi_appmsg.h"


//gui & mmk
#include "guilistbox.h"
#include "guirichtext.h"
#include "guitext.h"
#include "guiedit.h"
#include "guistring.h"
#include "guiref_scale.h"
#include "guires.h"
#include "guiownerdraw.h"
#include "guistatusbar.h"
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif


#include "mmimultim_image.h"


#include "mmi_text.h"
#include "mmi_default.h"
#include "mmk_timer.h"
#include "wma_adp.h"


//other module
#include "mmisrvaud_api.h"
#include "mmi_mainmenu_export.h"

#include "mmialarm_export.h"
#include "mmialarm_text.h"

#include "mmiset_export.h"
#include "mmiacc_text.h"
#include "mmiidle_export.h"

#include "mmifmm_internal.h"
#include "mmifmm_export.h"
#include "mmifmm_text.h"

#include "mmiidle_subwintab.h"
#include "mmicc_export.h"

#include "mmiudisk_export.h"
#include "mmisd_export.h"
#include "mmimms_export.h"
#include "mmieng_export.h"


#ifdef BLUETOOTH_SUPPORT
#include "mmibt_export.h"
#endif

#if defined(DRM_SUPPORT)
#include "drm_api.h"
#include "mmi_drmfilemgr.h"
#include "mmidrm_text.h"
#include "mmidrm_export.h"
#endif


//local module
#include "mmiap_position.h"
#include "mmimp3_image.h"
#include "mmimp3_export.h"
#include "mmimp3_text.h"
#include "mmimp3_anim.h"
#include "mmiap_common.h"
#include "mmiapwin_common.h"
#include "mmimp3_id.h"
#include "mmiap_control.h"
#include "mmiap_applet.h"
#include "mmiapwin_set.h"
#include "mmiapwin_list.h"
#include "mmiap_list.h"
#include "mmiap_list_control.h"
#include "mmiap_setting.h"
#include "mmiapwin_main.h"
#include "mmiap_appmain.h"
#include "mmiap_play.h"

#ifdef MMIAP_LYRIC_SUPPORT
#include "mmiap_lyric_internal.h"
#endif

#ifdef MMIAP_F_HISTORY_PLAYLIST
#include "mmiap_lastinfo_ctrl.h"
#endif

#ifdef MMIAP_SPECTRUM_SUPPORT
#include "mmiap_spectrum.h"
#endif

#ifdef MMIAP_MUSIC_LIST_SUPPORT
#include "mmiapwin_multilist.h"
#endif

#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
#include "mmiap_album.h"
#endif

#ifdef MMIAP_MUSICMARK_SUPPORT
#include "mmiap_musicmark.h"
#include "mmiapwin_musicmark.h"
#endif

#include "mmiapwin_common.h"
#include "mmiapwin_menu.h"

#ifdef BT_AV_CT_SUPPORT
#include "gui_ucs2b_converter.h"
#endif

/*lint -e730*/
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define MMIMP3_FORWARD_BACKWARD_PERIOD      1000        //1000ms
#define MMIMP3_TP_DRAG_PERIOD               500         //300ms
/**--------------------------------------------------------------------------*
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifdef MMIAP_F_RELEASE_BG_AUDIO
LOCAL BOOLEAN s_mp3_applet_suspend_by_key = FALSE;
#endif
#ifdef BT_AV_SNK_SUPPORT
LOCAL BOOLEAN s_mp3_is_bt_music = FALSE;
LOCAL BOOLEAN s_mp3_a2dp_sink_connected = FALSE;
#endif
#ifdef BT_AV_CT_SUPPORT
//LOCAL BOOLEAN s_mp3_is_avrcp_ct_connected = FALSE;/*lint !e551*/
LOCAL BOOLEAN s_mp3_is_resume_local_music = FALSE;
LOCAL BOOLEAN s_mp3_is_avrcp_right_long_key_down = FALSE;
LOCAL BOOLEAN s_mp3_is_avrcp_left_long_key_down = FALSE;
LOCAL BOOLEAN s_mp3_avrcp_lock_operation = FALSE;
LOCAL MMI_STRING_T s_mp3_avrcp_title_name={0};
LOCAL MMI_STRING_T s_mp3_avrcp_artist_name={0};
LOCAL MMI_STRING_T s_mp3_avrcp_album_name={0};
LOCAL MMI_STRING_T s_mp3_avrcp_track_info={0};
LOCAL AvrcpCtPlayStatus s_mp3_avrcp_play_status;
LOCAL BOOLEAN s_mp3_is_allocate_avrcp_string_memory = FALSE;
LOCAL uint16 s_mp3_avrcp_lock_operation_timer_id = 0;   
LOCAL uint16 s_mp3_avrcp_update_timer_id = 0; 
LOCAL uint16 s_mp3_avrcp_sync_timer_id = 0;   
#endif
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : handle mp3 play window message
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3PlayWinMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
);

/*****************************************************************************/
//  Description : handle mp3 main play window web key
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL void HandleMp3MainWinWebKey(
    BOOLEAN   is_key_down //是否是按键按下消息
);

/*****************************************************************************/
//  Description : handle mp3 main play window touch panel key
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3MainWinTpKey(
    DPARAM     param
);

/*****************************************************************************/
//  Description : handle mp3 seek timer
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMp3SeekTimer(
    uint8 timer_id
);

/*****************************************************************************/
//  Description : start mp3 forward or backward timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartForwardOrBackwardTimer(
    BOOLEAN  is_forward
);
/*****************************************************************************/
//  Description : stop mp3 forward or backward timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StopForwardOrBackwardTimer(void);

/*****************************************************************************/
//  Description :  Handle open waiting win
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3OpenWaitingWin(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

/*****************************************************************************/
//  Description : handle the MSG_MP3_PLAY_CUR_FILE. msg
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void HandlePlayCurFileMsg(
    uint32 play_offset
);

/*****************************************************************************/
//  Description : handle left key
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void HandleMp3LeftUpKey(void);

/*****************************************************************************/
//  Description : handle left key
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void HandleMp3LeftLongKey(void);

/*****************************************************************************/
//  Description : handle right key
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void HandleMp3RightUpKey(void);

/*****************************************************************************/
//  Description : handle right key
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void HandleMp3RightLongKey(void);

/*****************************************************************************/
//  Description : handle up key
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void HandleMp3UpKey(
    void
);

/*****************************************************************************/
//  Description : handle down key
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void HandleMp3DownKey(
    void
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void HandleMp3LeftKeyButton(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void HandleMp3ReturnKeyButton(void);

/*****************************************************************************/
//  Description : HandlePenOk
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void HandlePenOk(MMI_WIN_ID_T win_id, DPARAM param);

/*****************************************************************************/
//  Description : HandlePenLongOk
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void HandlePenLongOk(MMI_WIN_ID_T win_id, DPARAM param);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMp3TpDragTimer(
    uint8 timer_id
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void StartTpDragTimer(
    void
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void StopTpDragTimer(void);

/*****************************************************************************/
//  Description : handle album image glide tp press up msg
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void HandleAlbumImgGlideTpUp
(
    MMIAP_MAIN_WIN_INFO_T *win_ptr, //in
    DPARAM param
);

/*****************************************************************************/
//  Description : handle normal pda tp presss up msg
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void HandleNormalTpUp(DPARAM param);

/*****************************************************************************/
//  Description : ap main win callback
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIAP_WIN_CALLBACK_RET_E MMIAPMAINWIN_CtrlCallback
(
    MMIAP_WIN_CALLBACK_PARAM_T *param_ptr, //in
    uint32 size_of_param //in
);

/*****************************************************************************/
//  Description : virtual win callback
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIAP_WIN_CALLBACK_RET_E MMIVIRTUALWIN_CtrlCallback
(
    MMIAP_WIN_CALLBACK_PARAM_T *param_ptr, //in
    uint32 size_of_param //in
);

/*****************************************************************************/
//  Description : main idle win callback
//  Global resource dependence :
//  Author: robert.wang
//  Note: MAIN_IDLE_WIN_ID
/*****************************************************************************/
LOCAL MMIAP_WIN_CALLBACK_RET_E MMIIDLEWIN_CtrlCallback
(
    MMIAP_WIN_CALLBACK_PARAM_T *param_ptr, //in
    uint32 size_of_param //in
);

/*****************************************************************************/
//  Description : handle main win open msg
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainWinOpenMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
);

#ifdef BT_AV_CT_SUPPORT
/*****************************************************************************/
//  Description : Update AVRCP player time position
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
LOCAL void UpdateAvrcpPlayerTimePosition(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : Sync AVRCP player time position
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
LOCAL void SyncAvrcpPlayerTimePosition(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : Handle AVRCP play Pause down event
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
LOCAL void HandleAvrcpPlayPauseDown(void);

/*****************************************************************************/
//  Description : Lock AVRCP operation
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
LOCAL void LockAvrcpOperation(void);

/*****************************************************************************/
//  Description : Unlock AVRCP operation
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
LOCAL void UnlockAvrcpOperation(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : Stop AVRCP update timer
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
LOCAL void StopAvrcpUpdateTimer(void);

/*****************************************************************************/
//  Description : Stop AVRCP sync timer
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
LOCAL void StopAvrcpSyncTimer(void);

/*****************************************************************************/
//  Description : init avrcp variables
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
LOCAL void InitAvrcpVariables(void); 
#endif
/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/

LOCAL WINDOW_TABLE(MMIMP3_MAIN_PLAY_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleMp3PlayWinMsg),
    WIN_ID(MMIMP3_MAIN_PLAY_WIN_ID),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};


LOCAL MMIAP_MAIN_WIN_INFO_T *s_mmiap_main_win_info_ptr = PNULL;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/*****************************************************************************/
//  Description : get main win info ptr
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIAP_MAIN_WIN_INFO_T* GetMainWinInfoPtr(
    void
)
{
    return s_mmiap_main_win_info_ptr;
}

/*****************************************************************************/
//  Description : get main win func info ptr
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMIAP_WIN_FUNC_INFO_T* GetMainWinFuncInfoPtr(
    void
)
{
    MMIAP_WIN_FUNC_INFO_T *func_ptr = PNULL;
    MMIAP_MAIN_WIN_INFO_T *win_ptr = GetMainWinInfoPtr();


    if (PNULL != win_ptr)
    {
        func_ptr = &win_ptr->func_info;
    }


    return func_ptr;
}

/*****************************************************************************/
//  Description : get main win func info ptr
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void CreateMainWin(
    MMI_WIN_ID_T win_id //in
)
{
    MMIAP_WIN_FUNC_INFO_T *func_ptr = GetMainWinFuncInfoPtr();


    if (PNULL != func_ptr
            && PNULL != func_ptr->Func_CreateMainWin
       )
    {
        func_ptr->Func_CreateMainWin(win_id);
    }
}


/*****************************************************************************/
//  Description : create main win info
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateMainWinInfo(
    void
)
{
    BOOLEAN ret = FALSE;
    uint32 size_of_main_win = sizeof(*s_mmiap_main_win_info_ptr);


    if (PNULL == s_mmiap_main_win_info_ptr)
    {
        //创建并初始化
        s_mmiap_main_win_info_ptr = SCI_ALLOCA(size_of_main_win);
    }
    if (PNULL != s_mmiap_main_win_info_ptr)
    {
        ret = TRUE;

        SCI_MEMSET(s_mmiap_main_win_info_ptr, 0, size_of_main_win);
    }

    SCI_TRACE_LOW("[MMIAP] CreateMainWinInfo win_info_ptr=0x%X",\
                  s_mmiap_main_win_info_ptr);


    SCI_TRACE_LOW("[MMIAP] CreateMainWinInfo ret=%d",\
                  ret);

    return ret;
}

/*****************************************************************************/
//  Description :destroy main win info
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void DestroyMainWinInfo(
    void
)
{

    MMIAP_DestroyCommonWinInfo();

#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
    MMIAP_DestroyAlbumMngInfo();
#endif

    if (PNULL != s_mmiap_main_win_info_ptr)
    {
        SCI_FREE(s_mmiap_main_win_info_ptr);
    }

    SCI_TRACE_LOW("[MMIAP] DestroyMainWinInfo");
}

/*****************************************************************************/
//  Description : init sub win
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void InitMainFuncInfo(
    void
)
{
    MMIAP_WIN_FUNC_INFO_T *func_ptr = GetMainWinFuncInfoPtr();


    if (PNULL == func_ptr)
    {
        return;
    }

    func_ptr->Func_DefaultOpenWinMsg = HandleMainWinOpenMsg;
}

/*****************************************************************************/
//  Description : init wins info, include main win, common win
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InitWinsInfo(
    void
)
{
    BOOLEAN ret = FALSE;


    do
    {
        ret = CreateMainWinInfo();
        if (!ret)
        {
            break;
        }

        InitMainFuncInfo();

        ret = MMIAP_InitCommonWinInfo();
        if (!ret)
        {
            break;
        }
#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
        ret = MMIAP_InitAlbumMngInfo();
        if (!ret)
        {
            break;
        }
#endif
    }
    while(0);


    SCI_TRACE_LOW("[MMIAP] InitWinsInfo ret=%d",\
                  ret);

    return ret;
}

/*****************************************************************************/
//  Description : set tp x point
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SetApTpX(
    MMIAP_MAIN_WIN_INFO_T *info_ptr, //in
    uint16 tp_x //in
)
{
    if (PNULL == info_ptr)
    {
        return;
    }

    info_ptr->tp_x = tp_x;

    SCI_TRACE_LOW("[MMIAP] SetApTpX tp_x=%d",\
                  tp_x);
}

/*****************************************************************************/
//  Description : get tp x point
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL uint16 GetApTpX(
    MMIAP_MAIN_WIN_INFO_T *info_ptr //in

)
{
    uint16 tp_x = 0;

    if (PNULL == info_ptr)
    {
        return tp_x;
    }

    tp_x = info_ptr->tp_x;

    SCI_TRACE_LOW("[MMIAP] GetApTpX tp_x=%d",\
                  tp_x);

    return tp_x;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPMAINWIN_Enter(BOOLEAN is_playfile)
{
    MMIAP_MAIN_WIN_INFO_T  *win_ptr = PNULL;
    MMI_WIN_ID_T win_id = MMIMP3_MAIN_PLAY_WIN_ID;
    BOOLEAN ret = FALSE;


    //检查是否已经创建窗口
    win_ptr = GetMainWinInfoPtr();
    if (PNULL != win_ptr
            && PNULL != win_ptr->win_handle
       )
    {
        SCI_TRACE_LOW("[MMIAP]WINMAIN MMIAPMAINWIN_Enter error MMIMP3_MAIN_PLAY_WIN_ID already opened");
        return;
    }

    //alloc and init
    ret = InitWinsInfo();
    if (!ret)
    {
        DestroyMainWinInfo();

        MMIAPIMP3_StopApplet(FALSE);
        return;
    }
    //重新获得win pointer
    win_ptr = GetMainWinInfoPtr();

    //init sub win interface
    MMIAPMAINWIN_InitSubWin(&win_ptr->func_info);


    //创建窗口
    win_ptr->win_handle = MMIAPAPPLET_CreateWinTable((uint32*)MMIMP3_MAIN_PLAY_WIN_TAB, (ADD_DATA)is_playfile);
    if (PNULL == win_ptr->win_handle)
    {
        DestroyMainWinInfo();

        MMIAPIMP3_StopApplet(FALSE);
        return;
    }


    win_ptr->win_id = win_id;
    MMK_SetWinUserData(win_ptr->win_handle, (void*)win_ptr);

    //reg main win info to app
    {
        MMIAP_WIN_REQ_PARAM_T param = {0};

        param.win_type = MMIAP_WIN_TYPE_MAIN;
        param.win_ptr = (uint32*)win_ptr;
        param.win_callback_func = MMIAPMAINWIN_CtrlCallback;
        MMIAP_RegWinInfo(&param);
    }

    //reg virtual win info to app
    {
        MMIAP_WIN_REQ_PARAM_T param = {0};

        param.win_type = MMIAP_WIN_TYPE_VIRTUAL;
        param.win_ptr = (uint32*)VIRTUAL_WIN_ID;
        param.win_callback_func = MMIVIRTUALWIN_CtrlCallback;
        MMIAP_RegWinInfo(&param);
    }


    //reg idle win info to app
    {
        MMIAP_WIN_REQ_PARAM_T param = {0};

        param.win_type = MMIAP_WIN_TYPE_IDLE;
        param.win_ptr = (uint32*)MAIN_IDLE_WIN_ID;
        param.win_callback_func = MMIIDLEWIN_CtrlCallback;
        MMIAP_RegWinInfo(&param);
    }

    CreateMainWin(win_id);
}

/*****************************************************************************/
//  Description : get main win func info ptr
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void HandleAppletResume(
    MMI_WIN_ID_T win_id
)
{
    MMIAP_WIN_FUNC_INFO_T *func_ptr = GetMainWinFuncInfoPtr();


    if (PNULL != func_ptr
            && PNULL != func_ptr->Func_AppletResume
       )
    {
        func_ptr->Func_AppletResume(win_id);
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPMAINWIN_ResumeMainWin(void)
{
    if(MMK_IsFocusWin(MMIMP3_MAIN_PLAY_WIN_ID))
    {
        HandleAppletResume(MMIMP3_MAIN_PLAY_WIN_ID);

        MMIMP3_UpdateButtonValidStatus(TRUE);
        MMIMP3_DisplayButton(TRUE, FALSE);
    }
}

/*****************************************************************************/
//  Description : OpenMp3MainWin
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_OpenMainWin(BOOLEAN is_playfile)
{
    MMI_APPLET_START_T start = {0};

    //SCI_TRACE_LOW:"OpenMp3MainWin"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_2639_112_2_18_2_4_42_299, (uint8*)"");
    start.guid = SPRD_MP3_APPLET_ID;
    start.state = MMI_APPLET_STATE_BG_HIDE_WIN | MMI_APPLET_STATE_TASK_MANAGER;
    start.param_ptr = &is_playfile;
    start.size_of_param = sizeof(BOOLEAN);
    MMK_StartApplet(&start);
}

/*****************************************************************************/
//  Description : HandlePenOk
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void HandlePenOk(MMI_WIN_ID_T win_id, DPARAM param)
{
    MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;

    switch(ctrl_id)
    {
    case MMIMP3_PREV_BTN_CTRL_ID:
    case MMIMP3_NEXT_BTN_CTRL_ID:
        StopForwardOrBackwardTimer();
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : HandlePenLongOk
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void HandlePenLongOk(MMI_WIN_ID_T win_id, DPARAM param)
{
    //MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);
    MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;

    switch(ctrl_id)
    {
    case MMIMP3_PREV_BTN_CTRL_ID:
        if(MMIAPIMP3_BackwardMp3())
        {
            StartForwardOrBackwardTimer(FALSE);
        }

        break;

    case MMIMP3_NEXT_BTN_CTRL_ID:
        if(MMIAPIMP3_ForwardMp3())
        {
            StartForwardOrBackwardTimer(TRUE);
        }

        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : enter audio player after check
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_OpenAudioPlayerAfterCheck(void)
{
    MMIAP_OpenMainWin(FALSE);
}

/*****************************************************************************/
//  Description :  Handle open waiting win
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3OpenWaitingWin(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    BOOLEAN  is_lose_focus = FALSE;
    BOOLEAN  is_playfile = FALSE;
    MMIAP_OPEN_PLAYER_PARAM_T play_param = {0};
    MMIMP3_FMM_INFO_T fmm_file_info = {0};
    MMI_WIN_ID_T    alert_win_id = MMIMP3_ALERT_WIN_ID;
    MMIFILE_DEVICE_E file_dev = MMI_DEVICE_UDISK;

    MMIPUB_INFO_T *pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    MMIAP_WAIT_WIN_INFO_T *win_info_ptr = PNULL;



    //SCI_TRACE_LOW:"[MMIMP3] HandleMp3OpenWaitingWin: msg = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_3590_112_2_18_2_4_44_301, (uint8*)"d", msg_id);


    if (PNULL != pubwin_info_ptr)
    {
        win_info_ptr = (MMIAP_WAIT_WIN_INFO_T*)pubwin_info_ptr->user_data;
    }

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //MMIMP3_CheckModuleInfo();

#ifdef MMIAP_MUSICMARK_SUPPORT
        MMIMP3_LoadAllMusicMarkInfo();
#endif
//        MMIMP3_LoadLists();
		MMK_PostMsg(win_id, MSG_MP3_LOAD_LISTS, PNULL, NULL);//bug 131703

        MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);


#ifdef MMIAP_F_FIRST_SEARCH
        MMIAP_CheckSearchFlag();
#endif

        if (PNULL != win_info_ptr)
        {
            is_playfile = win_info_ptr->is_play_file;
        }

        if(is_playfile)
        {
            MMK_PostMsg(win_id, MSG_MP3_OPEN_PLAYER_WITH_INFO, PNULL, PNULL);
        }
        else
        {
            MMK_PostMsg(win_id, MSG_MP3_OPEN_PLAYER, PNULL, NULL);
        }
        break;

	case MSG_MP3_LOAD_LISTS:
        MMIMP3_LoadLists();
		break;

    case MSG_MP3_OPEN_PLAYER:
        file_dev = MMI_DEVICE_UDISK;
        if (PNULL != win_info_ptr)
        {
            win_info_ptr->file_dev = file_dev;
        }

#ifdef MMIAP_F_FIRST_SEARCH
        if(!MMIAP_IsFirstSearch())
        {
            MMK_PostMsg(win_id,MSG_MP3_INIT_PLAYER_FINISH,PNULL,PNULL);
        }
        else
#endif
        {
            MMIMP3_SearchAudioFilesInDefaultFolder(win_id,
                                                   MSG_MP3_LOAD_ALL_MUSIC_FINISH,
                                                   MSG_MP3_LOAD_ALL_LIST_FINISH,
                                                   file_dev
                                                  );
        }
        break;

    case MSG_MP3_OPEN_PLAYER_WITH_INFO:

        //MMIMP3_CheckReservedDir();


        MMIAP_GetOpenPlayerParam(&play_param);

        /*先删除上次的fmm 列表
        *否则，列表文件会叠加
        */
        MMIMP3_DeleteListFile(FALSE, APLAYER_LIST_TYPE_FMM);

        if(MMIMP3_OPEN_INFO_SINGLE_FILE == play_param.open_type)
        {
            MMIMP3_LoadAudioFile(play_param.file_info_u.file_data_info.file_name,
                                 play_param.file_info_u.file_data_info.file_name_len ,
                                 TRUE,
                                 MMIMP3_GetMp3PlayMode());

        }
        else if(MMIMP3_OPEN_INFO_FMM_DATA == play_param.open_type)
        {
            fmm_file_info = play_param.file_info_u.fmm_data_info;

            if(!MMIMP3_LoadFmmPlayList(
                        fmm_file_info.file_info_ptr,
                        fmm_file_info.file_num,
                        fmm_file_info.file_index,
                        fmm_file_info.dir_path_ptr,
                        fmm_file_info.dir_path_len,
                        MMIMP3_GetMp3PlayMode()
                    ))
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ERROR,
                                            TXT_NULL, IMAGE_PUBWIN_FAIL, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);

            }
        }

        if(MMIAP_IsNeedInitPlayer()) //resume the mp3 applet
        {
            MMIAP_SetNeedInitPlayer(FALSE);
            //MMIMP3_CheckModuleInfo();

            file_dev = MMI_DEVICE_UDISK;
            if (PNULL != win_info_ptr)
            {
                win_info_ptr->file_dev = file_dev;
            }


#ifdef MMIAP_F_FIRST_SEARCH
            if(!MMIAP_IsFirstSearch())
            {
                MMK_PostMsg(win_id,MSG_MP3_INIT_PLAYER_FINISH,PNULL,PNULL);
            }
            else
#endif
            {

                MMIMP3_SearchAudioFilesInDefaultFolder(win_id,
                                                       MSG_MP3_LOAD_ALL_MUSIC_FINISH,
                                                       MSG_MP3_LOAD_ALL_LIST_FINISH,
                                                       file_dev
                                                      );

            }
        }
        else
        {
            MMK_PostMsg(win_id, MSG_MP3_INIT_PLAYER_FINISH, PNULL, NULL);
        }

        break;

    case MSG_MP3_INIT_PLAYER_FINISH:
    {
        BOOLEAN    is_first_init = FALSE;
        if (PNULL != win_info_ptr)
        {
            is_playfile = win_info_ptr->is_play_file;
        }
        is_first_init = MMIMP3_IsFirstInit();
        SCI_TRACE_LOW("[MMIMP3]is_first_init:%d",is_first_init);
        if(is_playfile)
        {
            MMIMP3_SetFirstInit(FALSE);
        }
        else if(TRUE == is_first_init)   //开机后首次进入播放器时，is_first_init为TRUE
        {
#ifdef MMIAP_F_HISTORY_PLAYLIST
            if (!MMIAP_GetHistoryPlayListInfo())
#endif
            {
                MMIMP3_LoadCurPlayInfo();
            }
            MMIMP3_SetFirstInit(FALSE);
        }



        MMIMP3_GetCurFileContent();


        if(MMK_IsFocusWin(win_id))
        {
            MMK_CloseWin(win_id);

            if(is_playfile)
            {
                MMIAPIMP3_PlayAudioPlayer(MMIMP3_ZERO_OFFSET);
            }
        }
        else
        {
            if (PNULL != win_info_ptr)
            {
                win_info_ptr->is_lose_focus = TRUE;
            }
        }

        if(!MMK_IsFocusWin(win_id))
        {
            MMIMP3_ResizeAllListBufferSize(TRUE);
        }


        break;
    }
    case MSG_GET_FOCUS:
        if (PNULL != win_info_ptr)
        {
            is_lose_focus = win_info_ptr->is_lose_focus;
            is_playfile = win_info_ptr->is_play_file;
        }
        if(is_lose_focus)
        {
            if (PNULL != win_info_ptr)
            {
                win_info_ptr->is_lose_focus = FALSE;
            }

            MMK_CloseWin(win_id);

            if(is_playfile)
            {
                MMIAPIMP3_PlayAudioPlayer(MMIMP3_ZERO_OFFSET);
            }
        }

        break;

    case MSG_MP3_LOAD_ALL_LIST_FINISH:
        if (PNULL != win_info_ptr)
        {
            win_info_ptr->file_dev++;
            file_dev = win_info_ptr->file_dev;
        }

        MMIMP3_SearchAudioFilesInDefaultFolder(win_id,
                                               MSG_MP3_LOAD_ALL_MUSIC_FINISH,
                                               MSG_MP3_LOAD_ALL_LIST_FINISH,
                                               file_dev
                                              );
        break;

    case MSG_MP3_LOAD_ALL_MUSIC_FINISH:
        MMIMP3_SaveFilesToMyMusicList();
        MMK_PostMsg(win_id,MSG_MP3_INIT_PLAYER_FINISH,PNULL,PNULL);
        break;

    case MSG_APP_CANCEL:
        file_dev = MMI_DEVICE_NUM;
        if (PNULL != win_info_ptr)
        {
            win_info_ptr->file_dev = file_dev;
        }
        MMIMP3_StopLoadAudioFiles();
        break;

    case MSG_APP_RED:
        file_dev = MMI_DEVICE_NUM;
        if (PNULL != win_info_ptr)
        {
            win_info_ptr->file_dev = file_dev;
        }
        MMIMP3_StopLoadAudioFiles();
        result = MMI_RESULT_FALSE;
        break;

    case MSG_CLOSE_WINDOW:
        MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);

        if (PNULL != win_info_ptr)
        {
            SCI_FREE(win_info_ptr);
        }
        break;

    default:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : set right key down
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void InitMainWinKeyInfo(
    MMIAP_MAIN_WIN_INFO_T *win_info_ptr //in
)
{
    if (PNULL != win_info_ptr)
    {
        SCI_MEMSET(&win_info_ptr->key_info, 0, sizeof(win_info_ptr->key_info));
    }
}

/*****************************************************************************/
//  Description : handle main win open msg
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainWinOpenMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    BOOLEAN ret = FALSE;
    BOOLEAN playfile = FALSE;
    MMIAP_MAIN_WIN_INFO_T *win_info_ptr = (MMIAP_MAIN_WIN_INFO_T*)MMK_GetWinUserData(win_id);



    if (PNULL == win_info_ptr)
    {
        return ret;
    }

    //init param
    InitMainWinKeyInfo(win_info_ptr);


#ifdef MMIAP_F_HISTORY_PLAYLIST
    MMIAP_InitOldInfo();
#endif


    MMIMP3_UpdateButtonValidStatus(TRUE);

    //reset backlight
#ifdef MMIAP_LYRIC_SUPPORT
    MMIMP3_HandleLyricBackLight(TRUE);
#endif
    MMIMP3_InitTitleAnim(TRUE);
    MMIMP3_StopTitleAnim(TRUE);
#ifdef MMIAP_SPECTRUM_SUPPORT
    MMIMP3_StartSpectrum();
#endif
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
    MMIAP_UpdateReviewMenuStatus();
#endif
    MMIMP3_DisplayButton(TRUE, FALSE);
    MMIMP3_DisplayFileName(FALSE);  //ydsh@20120228
    MMIAP_HandleSoftBarMsg(FALSE);

#ifdef CMCC_UI_STYLE

    if(MMIAP_GetOpenMp3Entry() == MMIMP3_ENTRY_TYPE_FAVORATE)
    {
        MMK_PostMsg(MMIMP3_ALL_MP3_LIST_OPT_WIN_ID, MSG_MP3_OPEN_MAIN_PLAY_WIN_SUCCESS, NULL, NULL);
        MMK_PostMsg(MMIMP3_PLAY_LIST_LIST_OPT_WIN_ID, MSG_MP3_OPEN_MAIN_PLAY_WIN_SUCCESS, NULL, NULL);
    }

#endif

#ifdef PDA_UI_DROPDOWN_WIN
    MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_RUNNING_PLAYER);//@fen.xie
#endif
    MMIAP_DisplayBackgroudImg(win_info_ptr->win_id);

    playfile = (BOOLEAN)MMK_GetWinAddDataPtr(win_info_ptr->win_handle);/*lint !e507*/

    MMK_SendMsg(win_info_ptr->win_handle, MSG_MP3_INIT_WAIT, (DPARAM)&playfile);

    return ret;
}

/*****************************************************************************/
//  Description : web key down value
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsWebKeyDown(
    MMIAP_MAIN_WIN_INFO_T *win_ptr //in
)
{
    BOOLEAN is_down = FALSE;

    if (PNULL == win_ptr)
    {
        return is_down;
    }

    is_down = win_ptr->key_info.is_web_key_down;


    SCI_TRACE_LOW("[MMIAP] IsWebKeyDown is_down=%d",\
                  is_down);

    return is_down;
}

/*****************************************************************************/
//  Description : set web key down
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SetWebKeyDown(
    MMIAP_MAIN_WIN_INFO_T *win_ptr, //in
    BOOLEAN is_down
)
{
    if (PNULL == win_ptr)
    {
        return ;
    }

    win_ptr->key_info.is_web_key_down = is_down;

    SCI_TRACE_LOW("[MMIAP] SetWebKeyDown is_down=%d",\
                  is_down);
}

/*****************************************************************************/
//  Description : tp key down value
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpKeyDown(
    MMIAP_MAIN_WIN_INFO_T *win_ptr //in
)
{
    BOOLEAN is_down = FALSE;

    if (PNULL == win_ptr)
    {
        return is_down;
    }

    is_down = win_ptr->key_info.is_tp_key_down;


    SCI_TRACE_LOW("[MMIAP] IsTpKeyDown is_down=%d",\
                  is_down);

    return is_down;
}

/*****************************************************************************/
//  Description : set tp key down
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SetTpKeyDown(
    MMIAP_MAIN_WIN_INFO_T *win_ptr, //in
    BOOLEAN is_down
)
{
    if (PNULL == win_ptr)
    {
        return ;
    }

    win_ptr->key_info.is_tp_key_down = is_down;

    SCI_TRACE_LOW("[MMIAP] SetTpKeyDown is_down=%d",\
                  is_down);
}

/*****************************************************************************/
//  Description : up side key down value
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsUpSideKeyDown(
    MMIAP_MAIN_WIN_INFO_T *win_ptr //in
)
{
    BOOLEAN is_down = FALSE;

    if (PNULL == win_ptr)
    {
        return is_down;
    }

    is_down = win_ptr->key_info.is_up_side_key_down;


    SCI_TRACE_LOW("[MMIAP] IsUpSideKeyDown is_down=%d",\
                  is_down);

    return is_down;
}

/*****************************************************************************/
//  Description : set up side key down
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SetUpSideKeyDown(
    MMIAP_MAIN_WIN_INFO_T *win_ptr, //in
    BOOLEAN is_down
)
{
    if (PNULL == win_ptr)
    {
        return ;
    }

    win_ptr->key_info.is_up_side_key_down = is_down;

    SCI_TRACE_LOW("[MMIAP] SetUpSideKeyDown is_down=%d",\
                  is_down);
}

/*****************************************************************************/
//  Description : down side key down value
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDownSideKeyDown(
    MMIAP_MAIN_WIN_INFO_T *win_ptr //in
)
{
    BOOLEAN is_down = FALSE;

    if (PNULL == win_ptr)
    {
        return is_down;
    }

    is_down = win_ptr->key_info.is_down_side_key_down;


    SCI_TRACE_LOW("[MMIAP] IsDownSideKeyDown is_down=%d",\
                  is_down);

    return is_down;
}

/*****************************************************************************/
//  Description : set down side key down
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SetDownSideKeyDown(
    MMIAP_MAIN_WIN_INFO_T *win_ptr, //in
    BOOLEAN is_down
)
{
    if (PNULL == win_ptr)
    {
        return ;
    }

    win_ptr->key_info.is_down_side_key_down = is_down;

    SCI_TRACE_LOW("[MMIAP] SetDownSideKeyDown is_down=%d",\
                  is_down);
}

/*****************************************************************************/
//  Description : up key down value
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsUpKeyDown(
    MMIAP_MAIN_WIN_INFO_T *win_ptr //in
)
{
    BOOLEAN is_down = FALSE;

    if (PNULL == win_ptr)
    {
        return is_down;
    }

    is_down = win_ptr->key_info.is_up_key_down;


    SCI_TRACE_LOW("[MMIAP] IsUpKeyDown is_down=%d",\
                  is_down);

    return is_down;
}

/*****************************************************************************/
//  Description : set up key down
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SetUpKeyDown(
    MMIAP_MAIN_WIN_INFO_T *win_ptr, //in
    BOOLEAN is_down
)
{
    if (PNULL == win_ptr)
    {
        return ;
    }

    win_ptr->key_info.is_up_key_down = is_down;

    SCI_TRACE_LOW("[MMIAP] SetUpKeyDown is_down=%d",\
                  is_down);
}

/*****************************************************************************/
//  Description : down key down value
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDownKeyDown(
    MMIAP_MAIN_WIN_INFO_T *win_ptr //in
)
{
    BOOLEAN is_down = FALSE;

    if (PNULL == win_ptr)
    {
        return is_down;
    }

    is_down = win_ptr->key_info.is_down_key_down;


    SCI_TRACE_LOW("[MMIAP] IsDownKeyDown is_down=%d",\
                  is_down);

    return is_down;
}

/*****************************************************************************/
//  Description : set down key down
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SetDownKeyDown(
    MMIAP_MAIN_WIN_INFO_T *win_ptr, //in
    BOOLEAN is_down
)
{
    if (PNULL == win_ptr)
    {
        return ;
    }

    win_ptr->key_info.is_down_key_down = is_down;

    SCI_TRACE_LOW("[MMIAP] SetDownKeyDown is_down=%d",\
                  is_down);
}

/*****************************************************************************/
//  Description : left key down value
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsLeftKeyDown(
    MMIAP_MAIN_WIN_INFO_T *win_ptr //in
)
{
    BOOLEAN is_down = FALSE;

    if (PNULL == win_ptr)
    {
        return is_down;
    }

    is_down = win_ptr->key_info.is_left_key_down;


    SCI_TRACE_LOW("[MMIAP] IsLeftKeyDown is_down=%d",\
                  is_down);

    return is_down;
}

/*****************************************************************************/
//  Description : set left key down
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SetLeftKeyDown(
    MMIAP_MAIN_WIN_INFO_T *win_ptr, //in
    BOOLEAN is_down
)
{
    if (PNULL == win_ptr)
    {
        return ;
    }

    win_ptr->key_info.is_left_key_down = is_down;

    SCI_TRACE_LOW("[MMIAP] SetLeftKeyDown is_down=%d",\
                  is_down);
}

/*****************************************************************************/
//  Description : right key down value
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsRightKeyDown(
    MMIAP_MAIN_WIN_INFO_T *win_ptr //in
)
{
    BOOLEAN is_down = FALSE;

    if (PNULL == win_ptr)
    {
        return is_down;
    }

    is_down = win_ptr->key_info.is_right_key_down;


    SCI_TRACE_LOW("[MMIAP] IsRightKeyDown is_down=%d",\
                  is_down);

    return is_down;
}

/*****************************************************************************/
//  Description : set right key down
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SetRightKeyDown(
    MMIAP_MAIN_WIN_INFO_T *win_ptr, //in
    BOOLEAN is_down
)
{
    if (PNULL == win_ptr)
    {
        return ;
    }

    win_ptr->key_info.is_right_key_down = is_down;

    SCI_TRACE_LOW("[MMIAP] SetRightKeyDown is_down=%d",\
                  is_down);
}

/*****************************************************************************/
//  Description : notify update flag
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNotifyUpdate(
    MMIAP_MAIN_WIN_INFO_T *win_ptr //in
)
{
    BOOLEAN is_update = FALSE;


    if (PNULL == win_ptr)
    {
        return is_update;
    }

    is_update = win_ptr->is_notify_update;


    SCI_TRACE_LOW("[MMIAP] IsNotifyUpdate is_update=%d",\
                  is_update);

    return is_update;
}

/*****************************************************************************/
//  Description : set notify update
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SetNotifyUpdate(
    MMIAP_MAIN_WIN_INFO_T *win_ptr, //in
    BOOLEAN is_update //in
)
{
    if (PNULL == win_ptr)
    {
        return ;
    }

    win_ptr->is_notify_update = is_update;

    SCI_TRACE_LOW("[MMIAP] SetNotifyUpdate is_update=%d",\
                  is_update);
}

/*****************************************************************************/
//  Description : notify update flag
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsApForward(
    void
)
{
    MMIAP_MAIN_WIN_INFO_T *win_ptr = GetMainWinInfoPtr();
    BOOLEAN is_forward = FALSE;


    if (PNULL == win_ptr)
    {
        return is_forward;
    }

    is_forward = win_ptr->is_forward;


    SCI_TRACE_LOW("[MMIAP] IsApForward is_forward=%d",\
                  is_forward);

    return is_forward;
}

/*****************************************************************************/
//  Description : set forward flag
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SetApForward(
    BOOLEAN is_forward //in
)
{
    MMIAP_MAIN_WIN_INFO_T *win_ptr = GetMainWinInfoPtr();

    if (PNULL == win_ptr)
    {
        return ;
    }

    win_ptr->is_forward = is_forward;

    SCI_TRACE_LOW("[MMIAP] SetApForward is_forward=%d",\
                  is_forward);
}

/*****************************************************************************/
//  Description : get forward Timer id
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL uint8 GetForwardTimerID(
    void
)
{
    MMIAP_MAIN_WIN_INFO_T *win_ptr = GetMainWinInfoPtr();
    uint8 timer_id = 0;


    if (PNULL == win_ptr)
    {
        return timer_id;
    }

    timer_id = win_ptr->forward_timer_id;


    SCI_TRACE_LOW("[MMIAP] GetForwardTimerID timer_id=%d",\
                  timer_id);

    return timer_id;
}

/*****************************************************************************/
//  Description : set forward timer id
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SetForwardTimerID(
    uint8 timer_id //in
)
{
    MMIAP_MAIN_WIN_INFO_T *win_ptr = GetMainWinInfoPtr();

    if (PNULL == win_ptr)
    {
        return ;
    }

    win_ptr->forward_timer_id = timer_id;

    SCI_TRACE_LOW("[MMIAP] SetForwardTimerID timer_id=%d",\
                  timer_id);
}

/*****************************************************************************/
//  Description : get tp drag Timer id
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL uint8 GetTpDragTimerID(
    void
)
{
    MMIAP_MAIN_WIN_INFO_T *win_ptr = GetMainWinInfoPtr();
    uint8 timer_id = 0;


    if (PNULL == win_ptr)
    {
        return timer_id;
    }

    timer_id = win_ptr->tp_drag_timer_id;


    SCI_TRACE_LOW("[MMIAP] GetTpDragTimerID timer_id=%d",\
                  timer_id);

    return timer_id;
}

/*****************************************************************************/
//  Description : set tp drag timer id
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SetTpDragTimerID(
    uint8 timer_id //in
)
{
    MMIAP_MAIN_WIN_INFO_T *win_ptr = GetMainWinInfoPtr();

    if (PNULL == win_ptr)
    {
        return ;
    }

    win_ptr->tp_drag_timer_id = timer_id;

    SCI_TRACE_LOW("[MMIAP] SetTpDragTimerID timer_id=%d",\
                  timer_id);
}

/*****************************************************************************/
//  Description : handle app web key win msg
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAP_HandleAppWebKeyWinMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMIAP_MAIN_WIN_INFO_T *win_info_ptr = (MMIAP_MAIN_WIN_INFO_T*)MMK_GetWinUserData(win_id);


    MMIMP3_NoSupportNumClear();
    HandleMp3MainWinWebKey(TRUE);
    SetWebKeyDown(win_info_ptr, TRUE);

    return result;
}

/*****************************************************************************/
//  Description : handle app web key win msg
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAPDefaultWinMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    MMIAP_WIN_FUNC_INFO_T *func_ptr = GetMainWinFuncInfoPtr();


    if (PNULL != func_ptr
            && PNULL != func_ptr->Func_SubWinMsg
       )
    {
        result = func_ptr->Func_SubWinMsg(win_id, msg_id, param);
    }


    return result;
}

LOCAL BOOLEAN CreateApWaitWin(
    BOOLEAN is_playfile //in
)
{
    BOOLEAN ret = FALSE;

    MMI_WIN_ID_T waiting_win_id = MMIMP3_OPEN_PLAYER_WAIT_WIN_ID;

    uint32       waiting_time_out = 0;
    MMI_TEXT_ID_T waitwin_text_id = TXT_COMMON_SEARCHING;
    MMIAP_WAIT_WIN_INFO_T *win_info_ptr = PNULL;
    uint32 size = 0;



    size = sizeof(*win_info_ptr);
    win_info_ptr = (MMIAP_WAIT_WIN_INFO_T*)SCI_ALLOCA(size);
    if (PNULL == win_info_ptr)
    {
        return ret;
    }
    SCI_MEMSET(win_info_ptr, 0, size);
    win_info_ptr->is_play_file = is_playfile;


#ifdef MMIAP_F_FIRST_SEARCH
    waitwin_text_id = TXT_COMMON_WAITING;
#endif

    MMIPUB_OpenAlertWinByTextIdEx(SPRD_MP3_APPLET_ID,
                                  &waiting_time_out,
                                  waitwin_text_id,
                                  TXT_NULL,
                                  IMAGE_PUBWIN_WAIT,
                                  &waiting_win_id,
                                  PNULL,
                                  MMIPUB_SOFTKEY_NONE,
                                  HandleMp3OpenWaitingWin,
                                  (uint32)win_info_ptr
                                 );

    ret = TRUE;

    return ret;
}

/*****************************************************************************/
//  Description : stop timers
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPMAINWIN_StopTimers(
    void
)
{
    StopForwardOrBackwardTimer();
    MMIMP3_StopTpDrag();
}

/*****************************************************************************/
//  Description : update play surface
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPMAINWIN_UpdatePlaySurface(
    void
)
{
    MMIMP3_UpdateButtonValidStatus(TRUE);
    MMIMP3_DisplayButton(TRUE, FALSE);
    MMIMP3_DisplayFileName(FALSE);
    MMIAP_HandleSoftBarMsg(FALSE);
}

/*****************************************************************************/
//  Description : handle mp3 play window message
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
//   1.使用main/sub窗口的概念
//   2.先将消息派发给sub 窗口处理
//   3.sub窗口不处理的消息，main窗口再处理。
//   4.sub窗口主要处理有差异的部分
//   5.main窗口处理公共部分
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3PlayWinMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    BOOLEAN create_ret = FALSE;

    MMIAP_MAIN_WIN_INFO_T *win_info_ptr = (MMIAP_MAIN_WIN_INFO_T*)MMK_GetWinUserData(win_id);



    SCI_TRACE_LOW("[MMIMP3] HandleMp3PlayWinMsg: msg = 0x%x",msg_id);



    result = HandleAPDefaultWinMsg(win_id, msg_id, param);

    SCI_TRACE_LOW("[MMIAP] HandleMp3PlayWinMsg result=%d",\
                  result);

    if (!result)
    {
        result = MMI_RESULT_TRUE;


        switch(msg_id)
        {
        case MSG_OPEN_WINDOW:
            HandleMainWinOpenMsg(win_id, msg_id, param);
            break;

        case MSG_MP3_INIT_WAIT:
        {
            BOOLEAN is_playfile = FALSE;

            if(PNULL != (BOOLEAN*)param)
            {
                is_playfile = *(BOOLEAN*)param;
            }


            create_ret = CreateApWaitWin(is_playfile);
            if (!create_ret)
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMM_OPEN_FAIL);
                MMIAPIMP3_StopApplet(FALSE);
                break;
            }


#ifdef MMIAP_F_HISTORY_PLAYLIST

            if(MMIAP_IsBgStart()
                    && !is_playfile
              )
            {
                MMK_HideWinByApplet(SPRD_MP3_APPLET_ID);
            }

#endif
        }
        break;

        case MSG_MP3_BG_PLAY_MUSIC:
#ifdef MMIAP_F_HISTORY_PLAYLIST
            if(MMIAP_IsBgStart())
            {
                BOOLEAN is_play = FALSE;

                MMIMP3_PLAYER_STATE_E player_state = 0;
                MMIMP3_AUDIO_STATE_E audio_state = 0;
                MMIMP3_OLD_INFO_T *old_info_ptr = MMIAP_GetCurOldInfo();


                MMIAP_SetBgStartFlag(FALSE);


                if(PNULL != old_info_ptr)
                {
                    player_state = old_info_ptr->player_state;
                    audio_state = old_info_ptr->audio_state;

                    switch(player_state)
                    {
                    case MMIMP3_PLAYER_AUDIO_ACTIVE:
                        switch(audio_state)
                        {
                        case MMIMP3_AUDIO_PLAYING:
                            is_play = TRUE;
                            break;

                        default:
                            break;
                        }

                        break;

                    case MMIMP3_PLAYER_MUSIC_SWITCH:
                        is_play = TRUE;
                        break;

                    default:
                        break;
                    }

                    if(is_play)
                    {
                        MMIAP_HandleMp3PlayPauseButton();
                    }
                }

                SCI_TRACE_LOW("[MMIMP3] HandleMp3PlayWinMsg is_play=%d", \
                              is_play);
            }

#endif
            break;

        case MSG_FULL_PAINT:
        {
            MMIAP_DisplayAlertTips();

            MMIAP_DisplayBackgroudImg(win_id);
        }
        break;

        case MSG_NOTIFY_UPDATE:

            if(!IsNotifyUpdate(win_info_ptr))
            {
                SetNotifyUpdate(win_info_ptr, TRUE);
                GUIANIM_UpdateRect(*(GUI_RECT_T *)param, MMIAP_GetAnimCtrlID(MMIAP_DIRECT_MID));
                SetNotifyUpdate(win_info_ptr, FALSE);
            }

            break;

        case MSG_CTL_ANIM_DISPLAY_IND:
            if(MMK_IsFocusWin(win_id))
            {
                //ydsh@20120228
                GUIANIM_DISPLAY_IND_T   display_ind = *(GUIANIM_DISPLAY_IND_T *)param;
                MMI_CTRL_ID_T mid_anim_ctrl_id = MMIAP_GetAnimCtrlID(MMIAP_DIRECT_MID);

                if(display_ind.ctrl_handle == MMK_GetCtrlHandleByWin(win_id, mid_anim_ctrl_id)
                        && GUIANIM_TYPE_NONE != GUIANIM_GetAnimType(mid_anim_ctrl_id))
                {
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                }
            }

            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527

        case MSG_CTL_PENLONGOK:
            //SCI_TRACE_LOW:"[MMIMP3] HandlePenLongOk: MSG_CTL_PENLONGOK"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_3887_112_2_18_2_4_45_302, (uint8*)"");
            HandlePenLongOk(win_id, param);
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527

        case MSG_CTL_PENOK:
            HandlePenOk(win_id, param);
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#if 0
        case MSG_LCD_SWITCH:
            //停止快进快退，button 不支持旋转
            StopForwardOrBackwardTimer();
            MMIMP3_StopTpDrag();

            HandlePreloadLCDSwitchWinMsg(win_id, msg_id, param);

            MMIMP3_UpdateButtonValidStatus(TRUE);
            MMIMP3_DisplayButton(TRUE, FALSE);
            MMIMP3_DisplayFileName(FALSE);
            MMIAP_HandleSoftBarMsg(FALSE);
            break;
#endif
        case MSG_GET_FOCUS:
#ifdef MMIAP_F_RELEASE_BG_AUDIO
			s_mp3_applet_suspend_by_key = FALSE;
#endif
			MMIMP3_GetCurFileContent();  //Bug 132580 
#ifdef MMIAP_SPECTRUM_SUPPORT
            MMIMP3_StartSpectrum();
#endif
            MMIMP3_UpdateButtonValidStatus(TRUE);
            //reset backlight
#ifdef MMIAP_LYRIC_SUPPORT
            MMIMP3_HandleLyricBackLight(TRUE);
#endif
            MMIMP3_DisplayButton(TRUE, FALSE);
            MMIMP3_DisplayFileName(FALSE);
            MMIAP_HandleSoftBarMsg(FALSE);

            //update file tip info
            MMIAP_UpdateOsdFileTip(FALSE);  //NEWMS00168735   //ydsh@20120228

            MMIAP_UpdateAnimCtrlInfo(FALSE);  //NEWMS00168735   //ydsh@20120228 //update album info

            if(MMIAP_IsNeedReopenOsdPanel())
            {
                //open osd panel
                MMIAP_OpenMp3OsdPanel(FALSE);

                //reset flag
                MMIAP_SetReopenOsdPanelFlag(FALSE);
            }

            break;

        case MSG_LOSE_FOCUS:
            InitMainWinKeyInfo(win_info_ptr);

#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
            if (PNULL != win_info_ptr)
            {
                MMIAP_SetAlbumTpStart(FALSE);
                MMIAP_SetAlbumGlideMove(FALSE);
            }
#endif

#ifdef MMIAP_SPECTRUM_SUPPORT
            MMIMP3_StopSpectrum(FALSE);
#endif
            MMIMP3_ClearMainAnim(TRUE);
            MMIAP_CloseVolumePanel();

            StopForwardOrBackwardTimer();

            MMIMP3_StopTpDrag();

            //reset backlight
            //MMIMP3_HandleLyricBackLight(FALSE);
            //背景播放很多时候在非focus下也会操作，MMIMP3_HandleLyricBackLight 函数不能修改
#ifdef MMIAP_LYRIC_SUPPORT

            if(MMIMP3_DISPLAY_LYRIC_ON_WITH_BACKLIGHT == APSETTING_GetDisplayStyle()  //设置接口不能常调用 MMIDEFAULT_AllowTurnOffBackLight
                    && MMIMP3_IsLyricExist())
            {
                MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            }

#endif

#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
            MMIAP_ReleaseAllAlbumImgInfo();
#endif
            break;

        case MSG_TIMER:
            if(HandleMp3SeekTimer(*((uint8 *)param)))
            {
                //
            }
            else if(MMIAP_HandleMp3AlertTimer(*((uint8 *)param)))
            {
                //
            }
            else if(HandleMp3TpDragTimer(*((uint8 *)param)))
            {
                //
            }
            else
            {
                result = MMI_RESULT_FALSE;
            }

            break;

        case MSG_APP_MENU:
        case MSG_NOTIFY_OK:
        case MSG_APP_OK:
#ifndef TOUCH_PANEL_SUPPORT
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
            //if(!MMITHEME_IsMainScreenLandscape())
        {
            if(MMIAPIMP3_IsPlayerActive()
                    && MMIAP_IsReviewOpen()
              )
            {
                if(MMIMP3_REVIEW_STATE_NONE == MMIMP3_GetReviewStatus() ||
                        MMIMP3_REVIEW_STATE_SET  == MMIMP3_GetReviewStatus())
                {
                    MMIAP_HandleMp3ReviewButton();
                    break;
                }
            }
        }

#endif
#endif
        //default
        {
            MMIAP_OpenPlayerOptMenu(win_id);
        }
        break;

        case MSG_NOTIFY_CANCEL:
        case MSG_APP_CANCEL:
        {
#ifndef TOUCH_PANEL_SUPPORT
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
            //if(!MMITHEME_IsMainScreenLandscape())
            {
                if(MMIAPIMP3_IsPlayerActive()
                && MMIAP_IsReviewOpen()
                  )
                {
                    if(MMIMP3_REVIEW_STATE_NONE == MMIMP3_GetReviewStatus() ||
                    MMIMP3_REVIEW_STATE_SET  == MMIMP3_GetReviewStatus())
                    {
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
						MMIMP3_SetReviewState(FALSE);
						MMIAP_HandleMp3ReviewButton();
						MMIAP_HandleReviewMenu();
						MMIMP3_SetReviewState(TRUE);
#endif
                        break;
                    }
                }
            }
#endif
#endif

#ifndef MMI_PDA_SUPPORT //NEWMS00173394
#ifdef MMIAP_F_RELEASE_BG_AUDIO
			s_mp3_applet_suspend_by_key = TRUE;
#endif
            MMIAPIMP3_StopApplet(TRUE);
#else
            MMIAPIMP3_StopApplet(FALSE);
#endif
        }
        break;

#ifdef MMI_AUDIOPLAYER_MINI_SUPPORT
        case MSG_APP_WEB:
            MMIMP3_NoSupportNumClear();
            HandleMp3MainWinWebKey(TRUE);
//			s_mp3_is_web_key_down = TRUE;
            break;
#endif

        case MSG_KEYUP_WEB:
            if(IsWebKeyDown(win_info_ptr))
            {
                SetWebKeyDown(win_info_ptr, FALSE);
            }

            break;

        case MSG_KEYLONG_WEB:

            break;

        case MSG_APP_LEFT:
            SetLeftKeyDown(win_info_ptr, TRUE);
            break;

        case MSG_KEYUP_LEFT:
            if(IsLeftKeyDown(win_info_ptr))
            {
                HandleMp3LeftUpKey();
                SetLeftKeyDown(win_info_ptr, FALSE);
            }

            break;

        case MSG_KEYLONG_LEFT:
            if(IsLeftKeyDown(win_info_ptr))
            {
                HandleMp3LeftLongKey();
            }

            break;

        case MSG_APP_RIGHT:
            SetRightKeyDown(win_info_ptr, TRUE);
            break;

        case MSG_KEYUP_RIGHT:
            if(IsRightKeyDown(win_info_ptr))
            {
                HandleMp3RightUpKey();
                SetRightKeyDown(win_info_ptr, FALSE);
            }

            break;

        case MSG_KEYLONG_RIGHT:
            if(IsRightKeyDown(win_info_ptr))
            {
                HandleMp3RightLongKey();
            }

            break;

        case MSG_APP_UP:
            SetUpKeyDown(win_info_ptr, TRUE);
            HandleMp3UpKey();
            break;

        case MSG_KEYPRESSUP_UP:
        case MSG_KEYUP_UP:
            SetUpKeyDown(win_info_ptr, FALSE);
            break;

        case MSG_APP_DOWN:
            SetDownKeyDown(win_info_ptr, TRUE);
            HandleMp3DownKey();
            break;

        case MSG_KEYPRESSUP_DOWN:
        case MSG_KEYUP_DOWN:
            SetDownKeyDown(win_info_ptr, FALSE);
            break;

        case MSG_APP_UPSIDE:
            SetUpSideKeyDown(win_info_ptr, TRUE);
            if(!MMIAP_IsVolumePanelOpen())
            {
                MMIAPIMP3_TurnVol(MMICOM_PANEL_OPEN_INC);
            }

            break;

        case MSG_KEYPRESSUP_UPSIDE:
        case MSG_KEYUP_UPSIDE:
            SetUpSideKeyDown(win_info_ptr, FALSE);
            break;

        case MSG_APP_DOWNSIDE:
            SetDownSideKeyDown(win_info_ptr, TRUE);
            if(!MMIAP_IsVolumePanelOpen())
            {
                MMIAPIMP3_TurnVol(MMICOM_PANEL_OPEN_DEC);
            }

            break;

        case MSG_KEYPRESSUP_DOWNSIDE:
        case MSG_KEYUP_DOWNSIDE:
            SetDownSideKeyDown(win_info_ptr, FALSE);
            break;

        case MSG_KEYREPEAT_UPSIDE:
            break;

        case MSG_KEYREPEAT_UP:
            break;

        case MSG_KEYREPEAT_DOWNSIDE:
            break;

        case MSG_KEYREPEAT_DOWN:
            break;

        case MSG_KEYPRESSUP_WEB:
            SetWebKeyDown(win_info_ptr, FALSE);
            StopForwardOrBackwardTimer();
            break;

        case MSG_KEYPRESSUP_LEFT:
#ifdef BT_AV_CT_SUPPORT
			if(s_mp3_is_bt_music)
			{
				if(s_mp3_is_avrcp_left_long_key_down)
				{
					MMIAPIBT_SetAvrcpPanelKey(0x0048,FALSE,FALSE);
					s_mp3_is_avrcp_left_long_key_down = FALSE;
				}
			}
			else
			{
				StopForwardOrBackwardTimer();
				SetLeftKeyDown(win_info_ptr, FALSE);
			}
#else
			StopForwardOrBackwardTimer();
	        SetLeftKeyDown(win_info_ptr, FALSE);
#endif
            break;

        case MSG_KEYPRESSUP_RIGHT:
#ifdef BT_AV_CT_SUPPORT
			if(s_mp3_is_bt_music)
			{
				if(s_mp3_is_avrcp_right_long_key_down)
				{
					MMIAPIBT_SetAvrcpPanelKey(0x0049,FALSE,FALSE);
					s_mp3_is_avrcp_right_long_key_down = FALSE;
				}
			}
			else
			{
				StopForwardOrBackwardTimer();
				SetRightKeyDown(win_info_ptr, FALSE);
			}
#else
			StopForwardOrBackwardTimer();
	        SetRightKeyDown(win_info_ptr, FALSE);
#endif
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527

        case MSG_TP_PRESS_DOWN:
            MMIMP3_NoSupportNumClear();
            SetTpKeyDown(win_info_ptr, TRUE);


            if (PNULL == param)
            {
                break;
            }

            result = HandleMp3MainWinTpKey(param);
            SetApTpX(win_info_ptr, MMK_GET_TP_X(param));

#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
            MMIAP_HandleAlbumImgGlideTpDown(win_info_ptr, param);
#endif
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527

        case MSG_TP_PRESS_UP:
            SetTpKeyDown(win_info_ptr, FALSE);
            StopForwardOrBackwardTimer();
            MMIMP3_StopTpDrag();

            //#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
            HandleAlbumImgGlideTpUp(win_info_ptr,param);
            //#endif
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527

        case MSG_TP_PRESS_MOVE:
            if (PNULL == param)
            {
                break;
            }

            if(MMIMP3_IsTpDraging())
            {
                SetApTpX(win_info_ptr, MMK_GET_TP_X(param));
            }

#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
            MMIAP_HandleAlbumImgGlideTpMove(win_info_ptr, param);
#endif
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527

        case MSG_TP_PRESS_LONG:
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef MMIAP_LYRIC_SUPPORT

        case MSG_MP3_LYRIC_DISPLAY:
            if(MMK_IsFocusWin(win_id))
            {
                MMIMP3_DisplayLyricImmediately();
            }

            break;
#endif

        case MSG_MP3_SHOW_SOFT_BAR:
            MMIAP_HandleSoftBarMsg(TRUE);
            break;

        case MSG_MP3_PLAY_CUR_FILE:
            HandlePlayCurFileMsg(*(uint32 *)param);
            break;

        case MSG_CLOSE_WINDOW:
            SetWebKeyDown(win_info_ptr, FALSE);
            SetTpKeyDown(win_info_ptr, FALSE);

#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
            if (PNULL != win_info_ptr)
            {
                MMIAP_SetAlbumTpStart(FALSE);
                MMIAP_SetAlbumGlideMove(FALSE);
            }
#endif

            MMIMP3_ClearMainAnim(TRUE);
            MMIMP3_ClearMp3AlertInfo(TRUE);
#ifdef MMIAP_SPECTRUM_SUPPORT
            MMIMP3_StopSpectrum(FALSE);
#endif
            MMIAP_CloseVolumePanel();

            if(MMIAP_IsOpenOsdPanel())
            {
                MMIAP_CloseMp3OsdPanel(FALSE);
            }

            StopForwardOrBackwardTimer();

            MMIMP3_StopTpDrag();


            //reset backlight
#ifdef MMIAP_LYRIC_SUPPORT
            MMIMP3_HandleLyricBackLight(FALSE);
#endif
#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
            MMIAP_ReleaseAllAlbumInfo();
#endif

            //销毁main win info
            DestroyMainWinInfo();
            break;

        case MSG_APP_RED:
            //关闭volume panel win
            /*if (MMIAPICOM_IsPanelOpened())
            {
                MMIAPICOM_ClosePanelChildWin();
            }*/

            MMIAPMAINWIN_ReleaseAnimCtrlInfo();
            result = MMI_RESULT_FALSE;
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
        }

    }

    return (result);
}

/*****************************************************************************/
//  Description : display mp3 play window
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
void MMIMP3_DisplayMp3PlayWin(
    BOOLEAN           is_update,//is update name and lyric
    MMI_WIN_ID_T      win_id
)
{
    if(MMK_IsFocusWin(win_id))
    {
        MMIMP3_DisplayButton(TRUE, FALSE);
        MMIMP3_DisplayFileName(FALSE);
#ifdef MMI_AUDIOPLAYER_MINI_SUPPORT
        MMIAP_UpdateOsdFileTip(FALSE);
#endif
        MMIAP_HandleSoftBarMsg(FALSE);

        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void HandleMp3LeftKeyButton(void)
{
    MMIAP_OpenPlayerOptMenu(MMIMP3_MAIN_PLAY_WIN_ID);
    //MMK_PostMsg(MMIMP3_MAIN_PLAY_WIN_ID, MSG_APP_OK , PNULL, NULL);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void HandleMp3ReturnKeyButton(void)
{
    MMK_PostMsg(MMIMP3_MAIN_PLAY_WIN_ID, MSG_APP_CANCEL , PNULL, NULL);
}

/*****************************************************************************/
//  Description : handle mp3 main play window web key
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL void HandleMp3MainWinWebKey(
    BOOLEAN   is_key_down //是否是按键按下消息
)
{
    //SCI_TRACE_LOW:"[MMI MP3] HandleMp3MainWinWebKey"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_6173_112_2_18_2_4_49_311, (uint8*)"");

    if(MMIMP3_IsButtonValid(MMIMP3_BUTTON_PLAY_PAUSE))
    {
#ifdef BT_AV_CT_SUPPORT
		if(s_mp3_is_bt_music)
		{
			HandleAvrcpPlayPauseDown();
		}
		else
		{
			MMIAP_HandleMp3PlayPauseButton();
		}
#else
		MMIAP_HandleMp3PlayPauseButton();
#endif
    }
}

/*****************************************************************************/
//  Description : handle left key
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void HandleMp3LeftUpKey(void)
{
#ifdef BT_AV_CT_SUPPORT
	if(s_mp3_is_bt_music)
	{
        if(s_mp3_avrcp_lock_operation)
        {
            SCI_TRACE_LOW("BT_AV, HandleMp3LeftUpKey in lock status");
            return;
        }
        LockAvrcpOperation();	
		MMIAPIBT_SetAvrcpPanelKey(0x004c,TRUE,TRUE);
        s_mp3_avrcp_play_status.Position = 0;		
	}
	else
	{
		MMIAPIMP3_GoToPreOrNextMp3(TRUE);
    
		MMIAPIMP3_SwitchAnimCtrlInfo(FALSE, TRUE);
	}
#else
    MMIAPIMP3_GoToPreOrNextMp3(TRUE);

    MMIAPIMP3_SwitchAnimCtrlInfo(FALSE, TRUE);
#endif
}

/*****************************************************************************/
//  Description : handle left key
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void HandleMp3LeftLongKey(void)
{
#ifdef BT_AV_CT_SUPPORT
	if(s_mp3_is_bt_music)
	{
		MMIAPIBT_SetAvrcpPanelKey(0x0048,TRUE,FALSE);
		s_mp3_is_avrcp_left_long_key_down = TRUE;
	}
	else
	{
		if(MMIAPIMP3_BackwardMp3())
		{
			StartForwardOrBackwardTimer(FALSE);
		}
	}
#else
	if(MMIAPIMP3_BackwardMp3())
    {
        StartForwardOrBackwardTimer(FALSE);
    }
#endif
}

/*****************************************************************************/
//  Description : handle right key
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void HandleMp3RightUpKey(void)
{
#ifdef BT_AV_CT_SUPPORT
	if(s_mp3_is_bt_music)
	{
        if(s_mp3_avrcp_lock_operation)
        {
            SCI_TRACE_LOW("BT_AV, HandleMp3RightUpKey in lock status");
            return;
        }
        LockAvrcpOperation();	
		MMIAPIBT_SetAvrcpPanelKey(0x004b,TRUE,TRUE);
        s_mp3_avrcp_play_status.Position = 0;		
	}
	else
	{
		MMIAPIMP3_GoToPreOrNextMp3(FALSE);
    
		MMIAPIMP3_SwitchAnimCtrlInfo(FALSE, FALSE);
	}
#else
    MMIAPIMP3_GoToPreOrNextMp3(FALSE);

    MMIAPIMP3_SwitchAnimCtrlInfo(FALSE, FALSE);
#endif
}

/*****************************************************************************/
//  Description : handle right key
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void HandleMp3RightLongKey(void)
{
#ifdef BT_AV_CT_SUPPORT
	if(s_mp3_is_bt_music)
	{
		MMIAPIBT_SetAvrcpPanelKey(0x0049,TRUE,FALSE);
		s_mp3_is_avrcp_right_long_key_down = TRUE;
	}
	else
	{
		if (MMIAPIMP3_ForwardMp3())
		{
			StartForwardOrBackwardTimer(TRUE);
		}
	}
#else
	if (MMIAPIMP3_ForwardMp3())
    {
        StartForwardOrBackwardTimer(TRUE);
    }
#endif
}

/*****************************************************************************/
//  Description : handle up key
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void HandleMp3UpKey(
    void
)
{
    if(!MMIAP_IsVolumePanelOpen())
    {
        MMIAPIMP3_TurnVol(MMICOM_PANEL_OPEN_INC);
    }
}

/*****************************************************************************/
//  Description : handle down key
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void HandleMp3DownKey(
    void
)
{
    if(!MMIAP_IsVolumePanelOpen())
    {
        MMIAPIMP3_TurnVol(MMICOM_PANEL_OPEN_DEC);
    }
}

/*****************************************************************************/
//  Description : get mp3 current offset,play position by tp x value
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetMp3CurrentOffsetByTp(
    BOOLEAN       *is_forward_ptr,        //in/out:
    uint32         tp_x_offset,
    uint32        *mp3_total_offset_ptr,  //in/out:从Mp3 0位置算起
    uint32        *mp3_bitrate_ptr
)
{
    BOOLEAN             result = TRUE;
    BOOLEAN             is_forward = FALSE;
    uint32              cur_slide_x_offset = 0;
    uint32              mp3_total_offset = 0;
    MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};
    GUI_RECT_T          progress_rect = {0};
    uint16              progress_width = 0;
    uint16              progress_height = 0;

    MMIMP3_GetProgressRect(TRUE, &progress_rect);
    progress_width = progress_rect.right - progress_rect.left + 1;
    progress_height = progress_rect.bottom - progress_rect.top + 1;

    if(PNULL == is_forward_ptr ||
            PNULL == mp3_total_offset_ptr ||
            PNULL == mp3_bitrate_ptr)
    {
        SCI_TRACE_LOW("[MMIMP3]:GetMp3CurrentOffsetByTp, is_forward_ptr = %x,mp3_total_offset_ptr = %x,mp3_bitrate_ptr = %x!",\
                      is_forward_ptr, mp3_total_offset_ptr, mp3_bitrate_ptr);
        return FALSE;
    }

    MMIMP3_GetPlayingDynInfo(&dyn_info, FALSE);

    if(0 != dyn_info.total_length)
    {
        //get current progress slide x
        cur_slide_x_offset = (uint32)(progress_width * ((uint64_t)dyn_info.cur_offset) / dyn_info.total_length);
        SCI_TRACE_LOW("GetMp3CurrentOffsetByTp:cur_slide_x_offset = %d,tp_x_offset = %d",\
                      cur_slide_x_offset, tp_x_offset);

        //if forward
        if(cur_slide_x_offset < tp_x_offset)
        {
            is_forward = TRUE;
        }
        else if(cur_slide_x_offset > tp_x_offset)
        {
            is_forward = FALSE;
        }
        else
        {
            result = FALSE;
        }

        if(result)
        {
            //get mp3 total offset
            mp3_total_offset = tp_x_offset * (dyn_info.total_length / progress_width);

            SCI_TRACE_LOW("GetMp3CurrentOffsetByTp:is_forward = %d,mp3_total_offset = %ld",\
                          is_forward, mp3_total_offset);

            *is_forward_ptr = is_forward;
            *mp3_total_offset_ptr = mp3_total_offset;
            *mp3_bitrate_ptr = dyn_info.bit_rate;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle progress bar tp key,forward or backward
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleProgressBarTpKey(int16 tp_x)
{
    BOOLEAN     result = FALSE;
    BOOLEAN     is_forward = FALSE;
    uint32      tp_x_offet = 0;
    uint32      mp3_total_offset = 0;
    uint32      mp3_bitrate = 0;
    uint32      review_start_offset = 0;
    uint32      review_end_offset = 0;
    GUI_RECT_T  progress_bar_rect = {0};

    MMIMP3_GetProgressGrooveRect(TRUE, &progress_bar_rect);

    if(tp_x < progress_bar_rect.left)
    {
        tp_x = progress_bar_rect.left;
    }

    if(tp_x > progress_bar_rect.right)
    {
        tp_x = progress_bar_rect.right;
    }

    if((MMIMP3_AUDIO_PLAYING == MMIMP3_GetAudioState()) ||
            (MMIMP3_AUDIO_PAUSED  == MMIMP3_GetAudioState()))
    {
        //get forward or backward length and time offset
        tp_x_offet = tp_x - progress_bar_rect.left;
        //SCI_TRACE_LOW:"MMIMP3_HandleProgressBarTpKey:tp_x_offet = %d,tp_x = %d,progress_bar_rect.left = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_3735_112_2_18_2_3_55_58, (uint8*)"ddd", tp_x_offet, tp_x, progress_bar_rect.left);
        result = GetMp3CurrentOffsetByTp(&is_forward, tp_x_offet, &mp3_total_offset, &mp3_bitrate);

        if(MMIMP3_REVIEW_STATE_PLAY == MMIMP3_GetReviewStatus() &&
                MMIAPIMP3_GetReviewStartPoint(&review_start_offset)     &&
                MMIAPIMP3_GetReviewEndPoint(&review_end_offset))
        {
            if(mp3_total_offset >= review_end_offset ||
                    mp3_total_offset <= review_start_offset)
            {
                return(result = FALSE);
            }
        }

        if(result)
        {
            result = MMIAP_ForwardOrBackwardMp3(TRUE, is_forward, mp3_total_offset, MMIMP3_GetAudioState());

            if(result && !MMIMP3_IsTpDraging())
            {
                MMIMP3_StartTpDrag();
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MMIMP3_HandleProgressBarTpKey:get mp3 current offset result %d is error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_3759_112_2_18_2_3_55_59, (uint8*)"d", result);
        }

    }

    return (result);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMp3TpDragTimer(
    uint8 timer_id
)
{
    uint16 tp_x = 0;
    MMIAP_MAIN_WIN_INFO_T *info_ptr = GetMainWinInfoPtr();

    if(GetTpDragTimerID() == timer_id)
    {
        //SCI_TRACE_LOW:"[MMIMP3]: HandleMp3TpDragTimer enter"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_6301_112_2_18_2_4_50_312, (uint8*)"");

        if (PNULL != info_ptr)
        {
            tp_x = GetApTpX(info_ptr);
        }

        HandleProgressBarTpKey((int16)tp_x);

        StopTpDragTimer();

        StartTpDragTimer();

        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void StartTpDragTimer(
    void
)
{
    uint8 timer_id = 0;


    //SCI_TRACE_LOW:"[MMIMP3]: StartTpDragTimer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_6323_112_2_18_2_4_50_313, (uint8*)"");

    if(0 == GetTpDragTimerID())
    {
        timer_id = MMK_CreateWinTimer(MMIMP3_MAIN_PLAY_WIN_ID, MMIMP3_TP_DRAG_PERIOD, FALSE);
        SetTpDragTimerID(timer_id);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMP3]: StartTpDragTimer: the timer has started!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_6331_112_2_18_2_4_50_314, (uint8*)"");
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void StopTpDragTimer(void)
{
    uint8 timer_id = 0;


    timer_id = GetTpDragTimerID();
    if(0 < timer_id)
    {
        MMK_StopTimer(timer_id);
        SetTpDragTimerID(0);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMP3]: StopTpDragTimer: the timer has stop!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_6348_112_2_18_2_4_50_315, (uint8*)"");
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetTpDraging(
    BOOLEAN is_tp_draging //in
)
{
    MMIAP_MAIN_WIN_INFO_T *win_ptr = GetMainWinInfoPtr();


    if (PNULL == win_ptr)
    {
        return ;
    }
    win_ptr->is_tp_draging = is_tp_draging;


    SCI_TRACE_LOW("[MMIAP] SetTpDraging is_tp_draging=%d",\
                  is_tp_draging);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_StartTpDrag(void)
{
    StopTpDragTimer();

    StartTpDragTimer();

    SetTpDraging(TRUE);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_StopTpDrag(void)
{
    StopTpDragTimer();

    SetTpDraging(FALSE);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsTpDraging(void)
{
    MMIAP_MAIN_WIN_INFO_T *win_ptr = GetMainWinInfoPtr();
    BOOLEAN is_tp_draging = FALSE;


    if (PNULL == win_ptr)
    {
        return is_tp_draging;
    }
    is_tp_draging = win_ptr->is_tp_draging;


    SCI_TRACE_LOW("[MMIAP] MMIMP3_IsTpDraging is_tp_draging=%d",\
                  is_tp_draging);

    return is_tp_draging;
}

/*****************************************************************************/
//  Description : handle mp3 main play window touch panel key
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMp3MainWinTpKey(
    DPARAM     param
)
{
    GUI_RECT_T      progress_bar_rect   = {0};
    GUI_POINT_T     tp_point = {0};
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    if(!MMIAP_IsOpenOsdPanel())
    {
        return result;
    }

    //get tp point
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);

    //MMIMP3_GetProgressGrooveRect(TRUE,&progress_bar_rect);
    GUIOWNDRAW_GetDisplayRect(MMIMP3_PROGRESS_OWNER_DRAW_CTRL_ID, &progress_bar_rect);

    if(GUI_PointIsInRect(tp_point, progress_bar_rect) &&
            MMIMP3_IsCurFileSeekSupport())
    {
        HandleProgressBarTpKey(tp_point.x);

        result = MMI_RESULT_FALSE;
    }
    else
    {
        result = MMI_RESULT_FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle mp3 seek timer
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleMp3SeekTimer(
    uint8 timer_id
)
{
    BOOLEAN  seek_result = FALSE;
    BOOLEAN is_forward = FALSE;

    if(GetForwardTimerID() == timer_id)
    {
        //SCI_TRACE_LOW:"[MMIMP3]: HandleMp3SeekTimer enter"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_8019_112_2_18_2_4_53_321, (uint8*)"");

        StopForwardOrBackwardTimer();

        is_forward = IsApForward();
        if(is_forward)
        {
            seek_result = MMIAPIMP3_ForwardMp3();
        }
        else
        {
            seek_result = MMIAPIMP3_BackwardMp3();
        }

        //if(seek_result)
        {
            StartForwardOrBackwardTimer(is_forward);
        }

        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : start mp3 forward or backward timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartForwardOrBackwardTimer(
    BOOLEAN  is_forward
)
{
    uint8 timer_id = 0;

    //SCI_TRACE_LOW:"[MMI MP3]: StartForwardOrBackwardTimer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_8050_112_2_18_2_4_53_322, (uint8*)"");

    if(0 == GetForwardTimerID())
    {
        timer_id = MMK_CreateTimer(MMIMP3_FORWARD_BACKWARD_PERIOD, FALSE);
        SetForwardTimerID(timer_id);

        SetApForward(is_forward);
    }
    else
    {
        //SCI_TRACE_LOW:"StartForwardOrBackwardTimer: the key timer has started!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_8058_112_2_18_2_4_53_323, (uint8*)"");
    }
}

/*****************************************************************************/
//  Description : stop mp3 forward or backward timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StopForwardOrBackwardTimer(void)
{
    uint8 timer_id = 0;



    timer_id = GetForwardTimerID();

    //SCI_TRACE_LOW:"[MMIMP3]: StopForwardOrBackwardTimer s_mp3_forward_backward_timer_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_8068_112_2_18_2_4_53_324, (uint8*)"d", timer_id);


    if(0 < timer_id)
    {
        MMK_StopTimer(timer_id);
        SetForwardTimerID(0);
    }
    else
    {
        //SCI_TRACE_LOW:"StopForwardOrBackwardTimer: the key timer has stop!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_8076_112_2_18_2_4_53_325, (uint8*)"");
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsSeeking(void)
{
    if(0 < GetForwardTimerID())
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#ifdef MMIAP_F_RELEASE_BG_AUDIO
/*****************************************************************************/
//  Description :judge mp3 applet is suspend by kl
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsMp3AppletSuspendByKey(void)
{
	return s_mp3_applet_suspend_by_key;
}
#endif

/*****************************************************************************/
//  Description : stop mp3 forward or backward timer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_StopForwardOrBackwardTimer(void)
{
    StopForwardOrBackwardTimer();
}

/*****************************************************************************/
//  Description : Start play the cur file.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_StartPlayCurFile(
    uint32 play_offset  //default as MMIMP3_ZERO_OFFSET
)
{
    if(MMK_IsOpenWin(MMIMP3_MAIN_PLAY_WIN_ID))
    {
        MMK_PostMsg(MMIMP3_MAIN_PLAY_WIN_ID, MSG_MP3_PLAY_CUR_FILE, &play_offset, sizeof(play_offset));
    }
}

/*****************************************************************************/
//  Description : handle the MSG_MP3_PLAY_CUR_FILE. msg
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void HandlePlayCurFileMsg(
    uint32 play_offset
)
{
    MMIAPIMP3_PlayAudioPlayer(play_offset);
}

/*****************************************************************************/
//  Description : handle album image glide tp press up msg
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void HandleAlbumImgGlideTpUp
(
    MMIAP_MAIN_WIN_INFO_T *win_ptr, //in
    DPARAM param
)
{
#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
    BOOLEAN ret = FALSE;

    ret = MMIAP_HandleAlbumImgGlideTpUp(win_ptr, param);
    if (!ret)
#endif
    {
        //check to open or close osd panel
        HandleNormalTpUp(param);
    }

    //set active
    MMK_SetAtvCtrl(MMIMP3_MAIN_PLAY_WIN_ID, MMIAP_GetAnimCtrlID(MMIAP_DIRECT_MID));
}

/*****************************************************************************/
//  Description : handle normal pda tp presss up msg
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void HandleNormalTpUp(DPARAM param)
{
    GUI_POINT_T     tp_point = {0};
    GUI_RECT_T      ver_album_rect = MMIMP3_ALBUM_ANIM_RECT_V;
    GUI_RECT_T      hor_album_rect = MMIMP3_ALBUM_ANIM_RECT_H;
    GUI_RECT_T      rect = {0};
    uint16          up_osd_height = 0;
    uint16          down_osd_height = 0;

    //get point
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);

    if(MMITHEME_IsMainScreenLandscape())
    {
        rect = hor_album_rect;
    }
    else
    {
        rect = ver_album_rect;
    }

    if(MMIAP_IsOpenOsdPanel())
    {
#ifndef MMI_RES_LOW_COST
        GUIRES_GetImgWidthHeight(PNULL, &up_osd_height, IMAGE_MP3_OSD_BG_UP, MMIMP3_MAIN_PLAY_WIN_ID);
#endif
        GUIRES_GetImgWidthHeight(PNULL, &down_osd_height, IMAGE_MP3_OSD_BG_DOWN, MMIMP3_MAIN_PLAY_WIN_ID);

        rect.top    += up_osd_height;
        rect.bottom -= down_osd_height;
    }

    //check point in rect
    if(GUI_PointIsInRect(tp_point, rect))
    {
        if(MMIAP_IsOpenOsdPanel())
        {
            MMIAP_CloseMp3OsdPanel(TRUE);
        }
        else
        {
            MMIAP_OpenMp3OsdPanel(TRUE);
        }
    }
}

#ifdef MMIAP_LYRIC_SUPPORT
/*****************************************************************************/
//  Description : handle win lyric callback
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIAP_WIN_CALLBACK_RET_E HandleWinLyricCallback(
    MMI_HANDLE_T win_handle, //in
    MMIAP_LYRIC_WIN_CALLBACK_PARAM_T *req_param_ptr //in
)
{
    MMIAP_WIN_CALLBACK_RET_E ret = MMIAP_WIN_CALLBACK_RET_CONTINUE;

    BOOLEAN is_update_backlight = FALSE; //是否更新背光
    BOOLEAN is_light_on = FALSE; //是否背光常亮
    BOOLEAN is_update_lyric = FALSE; //是否更新歌词
    BOOLEAN is_update_lyric_now = FALSE; //是否立刻更新歌词



    //check focus win
    if (!MMK_IsFocusWin(win_handle))
    {
        return ret;
    }


    //若是req_param_ptr为NULL时，
    //使用默认值
    if (PNULL != req_param_ptr)
    {
        is_update_backlight = req_param_ptr->is_update_backlight;
        is_light_on = req_param_ptr->is_light_on;
        is_update_lyric = req_param_ptr->is_update_lyric;
        is_update_lyric_now = req_param_ptr->is_update_lyric_now;
    }

    if (is_update_backlight)
    {
        if (is_light_on)
        {
            MMIDEFAULT_TurnOnLCDBackLight();
            MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        }
        else
        {
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        }
    }

    if (is_update_lyric)
    {
        if (is_update_lyric_now)
        {
            MMK_SendMsg(win_handle, MSG_MP3_LYRIC_DISPLAY, PNULL);
        }
        else
        {
            MMK_PostMsg(win_handle, MSG_MP3_LYRIC_DISPLAY, PNULL, 0);
        }
    }

    SCI_TRACE_LOW("[MMIAP] HandleWinLyricCallback ret=%d",\
                  ret);

    return ret;
}
#endif //#ifdef MMIAP_LYRIC_SUPPORT

/*****************************************************************************/
//  Description : handle win play callback
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIAP_WIN_CALLBACK_RET_E HandleWinPlayCallback(
    MMI_HANDLE_T win_handle, //in
    MMI_WIN_ID_T win_id, //in
    MMIAP_PLAY_WIN_CALLBACK_PARAM_T *req_param_ptr //in
)
{
    MMIAP_WIN_CALLBACK_RET_E ret = MMIAP_WIN_CALLBACK_RET_CONTINUE;

    MMIAP_PLAY_WIN_FLAG_T play_flag = 0;



    if (PNULL == req_param_ptr)
    {
        return ret;
    }


    play_flag = req_param_ptr->flag;
    SCI_TRACE_LOW("[MMIAP] HandleWinPlayCallback flag=0x%X",\
                  play_flag);

    //focus win
    if (MMK_IsFocusWin(win_handle))
    {
        SCI_TRACE_LOW("[MMIAP] HandleWinPlayCallback focus win");

        if (play_flag & MMIAPWIN_PLAY_FLAG_ALL_PANEL)
        {
            MMIMP3_DisplayMp3PlayWin(TRUE, win_id);
        }

        if(play_flag & MMIAPWIN_PLAY_FLAG_START_SPECTRUM)
        {
#ifdef MMIAP_SPECTRUM_SUPPORT
            MMIMP3_StartSpectrum();
#endif
            MMIMP3_DisplayMp3PlayWin(TRUE, win_id);
        }

        if (play_flag & MMIAPWIN_PLAY_FLAG_BUTTON_PROGRESS)
        {
            MMIMP3_DisplayButton(TRUE, TRUE);
            MMIMP3_GetPlayingDynInfo(PNULL, TRUE);
            MMIMP3_DisplayProgressAndTimeInfo(TRUE);
        }

        if (play_flag & MMIAPWIN_PLAY_FLAG_ONLY_PROGRESS)
        {
            MMIMP3_DisplayProgressAndTimeInfo(TRUE);
        }

        if (play_flag & MMIAPWIN_PLAY_FLAG_SWITCH_ALBUM)
        {
            MMIAPIMP3_SwitchAnimCtrlInfo(TRUE, FALSE); //update album info
            ret = MMIAP_WIN_CALLBACK_RET_DONE_BY_CB;
        }

#ifdef MMIAP_LYRIC_SUPPORT
        if (play_flag & MMIAPWIN_PLAY_FLAG_SHOW_LYRIC_NOW)
        {
            MMIMP3_DisplayLyricImmediately();
        }
#endif


        if (play_flag & MMIAPWIN_PLAY_FLAG_MSG_SOFT_BAR)
        {
            MMIMP3_DisplayButton(TRUE, TRUE);
        }

        if (play_flag & MMIAPWIN_PLAY_FLAG_UPDATE_BUTTON_STATE)
        {
            MMIMP3_UpdateButtonValidStatus(TRUE);
        }

    }

    //opened win
    if (MMK_IsOpenWin(win_handle))
    {
        SCI_TRACE_LOW("[MMIAP] HandleWinPlayCallback opened win");


#ifdef DRM_SUPPORT
        if (play_flag & MMIAPWIN_PLAY_FLAG_PRECHECK_FILE)
        {
            if(MMIAPIDRM_PreCheckFileStatusEX(req_param_ptr->full_path_name_ptr,
                                              DRM_PERMISSION_PLAY,
                                              req_param_ptr->drm_status_ptr)
              )
            {
                MMIMP3_StopMp3();
                ret = MMIAP_WIN_CALLBACK_RET_BREAK;
            }
        }
#endif

        if (play_flag & MMIAPWIN_PLAY_FLAG_ALERT_MSG)
        {
            MMIMP3_ALERT_MSG_E alert_msg = 0;


            alert_msg = req_param_ptr->alert_msg;
            MMIMP3_HandleMp3AlertMsg(alert_msg, MMIAP_Mp3AlertCallback);

            ret = MMIAP_WIN_CALLBACK_RET_DONE_BY_CB;
        }

        if (play_flag & MMIAPWIN_PLAY_FLAG_WIN_SET_ALERT)
        {
            MMISRVAP_ERROR_E result = 0;
            MMISRVAUD_REPORT_RESULT_E auido_ret = 0;


            switch(result)
            {
            case MMISRVAP_ERROR_FILE_FORMAT_NOT_SUPPORT:
                auido_ret = MMISRVAUD_REPORT_RESULT_NOT_SUPPORT;
                MMIAPISET_AlertRingPlayResult(auido_ret, PNULL);
                break;
            case MMISRVAP_ERROR_UNKNOWN:
                break;
            default:
                auido_ret = MMISRVAUD_REPORT_RESULT_ERROR;
                MMIAPISET_AlertRingPlayResult(auido_ret, PNULL);
                break;
            }
        }

        if (play_flag & MMIAPWIN_PLAY_FLAG_MSG_SOFT_BAR)
        {
            MMK_PostMsg(win_handle, MSG_MP3_SHOW_SOFT_BAR, PNULL, NULL);
        }
    }
    else
    {
        SCI_TRACE_LOW("[MMIAP] HandleWinPlayCallback not opened win");

        if (play_flag & MMIAPWIN_PLAY_FLAG_ALERT_MSG)
        {
            MMI_WIN_ID_T     alert_win_id = MMIMP3_ALERT_WIN_ID;

            BOOLEAN is_need_alert = FALSE;


            if (PNULL != req_param_ptr->param_ptr)
            {
                is_need_alert = *(BOOLEAN*)req_param_ptr->param_ptr;
            }

            if (is_need_alert)
            {
                //需要open alert win

                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COMMON_NO_SUPPORT, TXT_NULL,
                                            IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
        }
    }

    SCI_TRACE_LOW("[MMIAP] HandleWinPlayCallback ret=%d",\
                  ret);

    return ret;
}

#ifdef MMIAP_SPECTRUM_SUPPORT
/*****************************************************************************/
//  Description : handle win spectrum callback
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIAP_WIN_CALLBACK_RET_E HandleWinSpectrumCallback(
    MMI_HANDLE_T win_handle, //in
    MMIAP_SPECTRUM_WIN_CALLBACK_PARAM_T *req_param_ptr //in
)
{
    MMIAP_WIN_CALLBACK_RET_E ret = MMIAP_WIN_CALLBACK_RET_BREAK;


    if (PNULL == req_param_ptr)
    {
        return ret;
    }


    if (MMK_IsFocusWin(win_handle))
    {

        if (req_param_ptr->is_check_win_focus)
        {
            ret = MMIAP_WIN_CALLBACK_RET_CONTINUE;
        }

        if (req_param_ptr->is_show_spectrum)
        {
            MMIMP3_DisplaySpectrum(MMIMP3_GetMainAnimCtrlId());
        }
    }

    SCI_TRACE_LOW("[MMIAP] HandleWinPlayCallback ret=%d",\
                  ret);

    return ret;
}
#endif //#ifdef MMIAP_SPECTRUM_SUPPORT

/*****************************************************************************/
//  Description : ap main win callback
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIAP_WIN_CALLBACK_RET_E MMIAPMAINWIN_CtrlCallback
(
    MMIAP_WIN_CALLBACK_PARAM_T *param_ptr, //in
    uint32 size_of_param //in
)
{
    MMIAP_WIN_CALLBACK_RET_E ret = MMIAP_WIN_CALLBACK_RET_CONTINUE;

    MMIAP_WIN_FLAG_T win_flag = 0;
    MMIAP_MAIN_WIN_INFO_T *win_ptr = PNULL;
    MMI_HANDLE_T win_handle = PNULL;
    MMI_WIN_ID_T win_id = 0;

    if (PNULL == param_ptr)
    {
        return ret;
    }

    win_flag = param_ptr->win_flag;

    win_ptr = (MMIAP_MAIN_WIN_INFO_T *)param_ptr->win_ptr;
    if (PNULL == win_ptr)
    {
        return ret;
    }


    SCI_TRACE_LOW("[MMIAP] MMIAPMAINWIN_CtrlCallback win_flag=0x%X",\
                  win_flag);

    win_handle = win_ptr->win_handle;
    win_id = win_ptr->win_id;



    /*
    * handle callback for opened win or focus win
    */

    if (win_flag & MMIAPWIN_FLAG_FUNC_PLAY)
    {
        //opened win or focus win
        ret = HandleWinPlayCallback(win_handle, win_id, param_ptr->param_ptr);
    }


#ifdef MMIAP_LYRIC_SUPPORT
    //歌词功能
    if (win_flag & MMIAPWIN_FLAG_FUNC_LYRIC)
    {
        ret =  HandleWinLyricCallback(win_handle, param_ptr->param_ptr);
    }
#endif


#ifdef MMIAP_SPECTRUM_SUPPORT
    //频谱功能
    if (win_flag & MMIAPWIN_FLAG_FUNC_SPECTRUM)
    {
        ret =  HandleWinSpectrumCallback(win_handle, param_ptr->param_ptr);
    }
#endif


    SCI_TRACE_LOW("[MMIAP] MMIVIRTUALWIN_CtrlCallback ret=%d",\
                  ret);
    return ret;
}

/*****************************************************************************/
//  Description : virtual win callback
//  Global resource dependence :
//  Author: robert.wang
//  Note:VIRTUAL_WIN_ID
/*****************************************************************************/
LOCAL MMIAP_WIN_CALLBACK_RET_E MMIVIRTUALWIN_CtrlCallback
(
    MMIAP_WIN_CALLBACK_PARAM_T *param_ptr, //in
    uint32 size_of_param //in
)
{
    MMIAP_WIN_CALLBACK_RET_E ret = MMIAP_WIN_CALLBACK_RET_CONTINUE;
    MMIAP_WIN_FLAG_T win_flag = 0;
    MMI_HANDLE_T win_handle = PNULL;


    if (PNULL == param_ptr)
    {
        return ret;
    }

    win_flag = param_ptr->win_flag;

    win_handle = (uint32)param_ptr->win_ptr; //this is a value


    SCI_TRACE_LOW("[MMIAP] MMIVIRTUALWIN_CtrlCallback win_flag=0x%X",\
                  win_flag);

#ifdef MMIAP_LYRIC_SUPPORT
    //更新歌词
    if (win_flag & MMIAPWIN_FLAG_FUNC_LYRIC)
    {
        BOOLEAN is_update_lyric = FALSE; //是否更新歌词

        MMIAP_LYRIC_WIN_CALLBACK_PARAM_T *req_param_ptr = PNULL;


        if (PNULL != param_ptr->param_ptr)
        {
            req_param_ptr = (MMIAP_LYRIC_WIN_CALLBACK_PARAM_T*)param_ptr->param_ptr;
            is_update_lyric = req_param_ptr->is_update_lyric;
        }

        if (is_update_lyric)
        {
            MMK_PostMsg(win_handle, MSG_MP3_START_LIRIC, PNULL, 0);
        }
    }
#endif

    //play
    if (win_flag & MMIAPWIN_FLAG_FUNC_PLAY)
    {
        MMIAP_PLAY_WIN_CALLBACK_PARAM_T *req_param_ptr = PNULL;
        MMIAP_PLAY_WIN_FLAG_T play_flag = 0;



        if (PNULL != param_ptr->param_ptr)
        {
            req_param_ptr = (MMIAP_PLAY_WIN_CALLBACK_PARAM_T*)param_ptr->param_ptr;

            play_flag = req_param_ptr->flag;

            SCI_TRACE_LOW("[MMIAP] MMIVIRTUALWIN_CtrlCallback play_flag=0x%X",\
                          play_flag);


            if (play_flag & MMIAPWIN_PLAY_FLAG_PLAY_CALLBACK)
            {
                MMK_PostMsg(win_handle, MSG_MP3_PLAY_CALL_BACK, PNULL, 0);
            }

            if (play_flag & MMIAPWIN_PLAY_FLAG_FATAL_ERROR)
            {
                MMK_PostMsg(win_handle, MSG_MP3_FATAL_ERROR, PNULL, 0);
            }
        }
    }

    SCI_TRACE_LOW("[MMIAP] MMIVIRTUALWIN_CtrlCallback ret=%d",\
                  ret);
    return ret;
}

/*****************************************************************************/
//  Description : main idle win callback
//  Global resource dependence :
//  Author: robert.wang
//  Note: MAIN_IDLE_WIN_ID
/*****************************************************************************/
LOCAL MMIAP_WIN_CALLBACK_RET_E MMIIDLEWIN_CtrlCallback
(
    MMIAP_WIN_CALLBACK_PARAM_T *param_ptr, //in
    uint32 size_of_param //in
)
{
    MMIAP_WIN_CALLBACK_RET_E ret = MMIAP_WIN_CALLBACK_RET_CONTINUE;
    MMIAP_WIN_FLAG_T win_flag = 0;
    MMI_HANDLE_T win_handle = PNULL;


    if (PNULL == param_ptr)
    {
        return ret;
    }

    win_flag = param_ptr->win_flag;

    win_handle = (uint32)param_ptr->win_ptr; //this is a value


    SCI_TRACE_LOW("[MMIAP] MMIMAINIDLEWIN_CtrlCallback win_flag=0x%X",\
                  win_flag);


    //play
    if (win_flag & MMIAPWIN_FLAG_FUNC_PLAY)
    {
        MMIAP_PLAY_WIN_CALLBACK_PARAM_T *req_param_ptr = PNULL;
        MMIAP_PLAY_WIN_FLAG_T play_flag = 0;



        if (PNULL != param_ptr->param_ptr)
        {
            req_param_ptr = (MMIAP_PLAY_WIN_CALLBACK_PARAM_T*)param_ptr->param_ptr;

            play_flag = req_param_ptr->flag;

            SCI_TRACE_LOW("[MMIAP] MMIMAINIDLEWIN_CtrlCallback play_flag=0x%X",\
                          play_flag);


            if(MMK_IsFocusWin(win_handle))
            {
                if (play_flag & MMIAPWIN_PLAY_FLAG_UPDATE_MP3)
                {
                    MMK_SendMsg(win_handle, MSG_IDLE_UPDATE_MP3, 0);
                }
            }

            if (play_flag & MMIAPWIN_PLAY_FLAG_SWITCH_MP3)
            {
                MMK_SendMsg(win_handle, MSG_WIDGET_MP3_SWITCH, req_param_ptr->param_ptr);
            }
        }
    }


    SCI_TRACE_LOW("[MMIAP] MMIMAINIDLEWIN_CtrlCallback ret=%d",\
                  ret);
    return ret;
}

#if defined BT_AV_CT_SUPPORT || defined BT_AV_SNK_SUPPORT
/*****************************************************************************/
//  Description : get is bt music
//  Global resource dependence :
//  Author: jinju.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMp3_IsBtMusic(void)
{
	return s_mp3_is_bt_music;
}
#endif

#ifdef BT_AV_CT_SUPPORT
/*****************************************************************************/
//  Description : get avrcp play status
//  Global resource dependence :
//  Author: jinju.ma
//  Note:
/*****************************************************************************/
PUBLIC AvrcpCtPlayStatus MMIAPIMp3_GetAvrcpPlayStatus(void)
{
	return s_mp3_avrcp_play_status;
}
/*****************************************************************************/
//  Description : get is alloc avrcp string memory
//  Global resource dependence :
//  Author: jinju.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMp3_IsAllocAvrcpStringMemory(void)
{
	return s_mp3_is_allocate_avrcp_string_memory;
}

/*****************************************************************************/
//  Description : init A2DP and AVRCP connect status
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
PUBLIC void InitA2dpAvrcpConnectionStatus(void)
{
    s_mp3_a2dp_sink_connected   = MMIAPIBT_IsA2dpSinkConnected();
//    s_mp3_is_avrcp_ct_connected = MMIAPIBT_IsAvrcpCtConnected();
    
#ifdef WIN32
    //for simulator
    s_mp3_avrcp_play_status.Length = 166;	
    s_mp3_avrcp_play_status.Position = 66;
#endif
}

/*****************************************************************************/
//  Description : Allocate AVRCP string memory
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
PUBLIC void AllocateAvrcpStringMemory(void)
{
	if(PNULL == s_mp3_avrcp_title_name.wstr_ptr)
	{
		s_mp3_avrcp_title_name.wstr_ptr = (wchar*)SCI_ALLOC_APP( ( MAX_TG_PLAYER_STRING_LEN + 1 ) * sizeof(wchar) );
		if(PNULL == s_mp3_avrcp_title_name.wstr_ptr)
		{
			SCI_TRACE_LOW("BT_AV, allocate memory error - s_mp3_avrcp_title_name.wstr_ptr");
			return;
		}
	}
	if(PNULL == s_mp3_avrcp_artist_name.wstr_ptr)
	{
		s_mp3_avrcp_artist_name.wstr_ptr = (wchar*)SCI_ALLOC_APP( ( MAX_TG_PLAYER_STRING_LEN + 1 ) * sizeof(wchar) );
		if(PNULL == s_mp3_avrcp_artist_name.wstr_ptr)
		{
			SCI_TRACE_LOW("BT_AV, allocate memory error - s_mp3_avrcp_artist_name.wstr_ptr");
			return;
		}
	}
	if(PNULL == s_mp3_avrcp_album_name.wstr_ptr)
	{
		s_mp3_avrcp_album_name.wstr_ptr = (wchar*)SCI_ALLOC_APP( ( MAX_TG_PLAYER_STRING_LEN + 1 ) * sizeof(wchar) );
		if(PNULL == s_mp3_avrcp_album_name.wstr_ptr)
		{
			SCI_TRACE_LOW("BT_AV, allocate memory error - s_mp3_avrcp_album_name.wstr_ptr");
			return;
		}
	}
	if(PNULL == s_mp3_avrcp_track_info.wstr_ptr)
	{
		s_mp3_avrcp_track_info.wstr_ptr = (wchar*)SCI_ALLOC_APP( ( MMIMP3_DISPLAY_TIME_LEN + 1 ) * sizeof(wchar) );
		if(PNULL == s_mp3_avrcp_track_info.wstr_ptr)
		{
			SCI_TRACE_LOW("BT_AV, allocate memory error - s_mp3_avrcp_track_info.wstr_ptr");
			return;
		}
	}
	s_mp3_is_allocate_avrcp_string_memory = TRUE;
}

/*****************************************************************************/
//  Description : release AVRCP string memory
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
PUBLIC void FreeAvrcpStringMemory(void)
{
	if(PNULL != s_mp3_avrcp_title_name.wstr_ptr)
	{
		SCI_FREE(s_mp3_avrcp_title_name.wstr_ptr);
		s_mp3_avrcp_title_name.wstr_ptr = PNULL;
	}
	if(PNULL != s_mp3_avrcp_artist_name.wstr_ptr)
	{
		SCI_FREE(s_mp3_avrcp_artist_name.wstr_ptr);
		s_mp3_avrcp_artist_name.wstr_ptr = PNULL;
	}
	if(PNULL != s_mp3_avrcp_album_name.wstr_ptr)
	{
		SCI_FREE(s_mp3_avrcp_album_name.wstr_ptr);
		s_mp3_avrcp_album_name.wstr_ptr = PNULL;
	}
	if(PNULL != s_mp3_avrcp_track_info.wstr_ptr)
	{
		SCI_FREE(s_mp3_avrcp_track_info.wstr_ptr);
		s_mp3_avrcp_track_info.wstr_ptr = PNULL;
	}

	s_mp3_is_allocate_avrcp_string_memory = FALSE;
	SCI_TRACE_LOW("lzk log. FreeAvrcpStringMemory() exit");
}

/*****************************************************************************/
//  Description : update avrcp player display
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
PUBLIC void UpdateAvrcpPlayerDisplay(void)
{
    //bug 918575
    if(!MMK_IsFocusWin(MMIMP3_MAIN_PLAY_WIN_ID))
    {
        SCI_TRACE_LOW("lzk log. UpdateAvrcpPlayerDisplay() exit. ap not active win.");
        return;
    }
    //bug 918575

	if(PNULL != s_mp3_avrcp_title_name.wstr_ptr)
	{
        #ifdef MMI_AUDIOPLAYER_MINI_SUPPORT		
        GUITITLE_SetText(MMIMP3_MAIN_PLAY_WIN_ID, MMITHEME_GetTitleCtrlId(), s_mp3_avrcp_title_name.wstr_ptr, s_mp3_avrcp_title_name.wstr_len, TRUE);    
        #else	
		GUILABEL_SetText(MMIMP3_TITLE_LABLE_CTRL_ID, &s_mp3_avrcp_title_name, TRUE);
        #endif		
	}
	if(PNULL != s_mp3_avrcp_artist_name.wstr_ptr)
	{
		GUILABEL_SetText(MMIMP3_ARTIST_LABLE_CTRL_ID, &s_mp3_avrcp_artist_name, TRUE);
	}
	if(PNULL != s_mp3_avrcp_album_name.wstr_ptr)
	{
		GUILABEL_SetText(MMIMP3_ALBUM_LABLE_CTRL_ID, &s_mp3_avrcp_album_name, TRUE);
	}
	if(PNULL != s_mp3_avrcp_track_info.wstr_ptr)
	{
#ifdef MMI_AUDIOPLAYER_MINI_SUPPORT
        GUITITLE_SetSubText(MMIMP3_MAIN_PLAY_WIN_ID, MMITHEME_GetTitleCtrlId(), TRUE, s_mp3_avrcp_track_info.wstr_ptr, s_mp3_avrcp_track_info.wstr_len, 1);
#else
        GUILABEL_SetText(MMIMP3_FILE_INFO_LABLE_CTRL_ID, &s_mp3_avrcp_track_info, TRUE);
#endif
	}	
}

/*****************************************************************************/
//  Description : Update AVRCP player button display
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
PUBLIC void UpdateAvrcpPlayerButton(void)
{		
	GUI_BG_T btn_bg = {0};
    MMIAP_BUTTON_INFO_T button_info = {0};
	
	MMI_IMAGE_ID_T      pre_image       = IMAGE_COMMON_BTN_PREV;	//IMAGE_COMMON_BTN_PREV,IMAGE_COMMON_BTN_PREV_GREY
	MMI_IMAGE_ID_T      next_image      = IMAGE_COMMON_BTN_NEXT;	//IMAGE_COMMON_BTN_NEXT,IMAGE_COMMON_BTN_NEXT_GREY
	MMI_IMAGE_ID_T      play_pause_image = IMAGE_COMMON_BTN_PLAY;

	btn_bg.bg_type = GUI_BG_IMG;
	btn_bg.img_id  = pre_image;
	//GUIBUTTON_SetPressedFg(MMIMP3_PREV_BTN_CTRL_ID,&pre_bg);
    GUIBUTTON_SetFg(MMIMP3_PREV_BTN_CTRL_ID,&btn_bg);

	btn_bg.img_id  = next_image;
	//GUIBUTTON_SetPressedFg(MMIMP3_NEXT_BTN_CTRL_ID,&pre_bg);
    GUIBUTTON_SetFg(MMIMP3_NEXT_BTN_CTRL_ID,&btn_bg);

	/*media 0x00 : STOPPED
	 *		0x01 : PLAYING
	 *		0x02 : PAUSED
	 *		0x03 : FWD_SEEK
	 *		0x04 : REV_SEEK
	 *		0xFF : ERROR
	 */

	SCI_TRACE_LOW("BT_AV, UpdateAvrcpPlayerButton length:%u, position:%u, status:%u",s_mp3_avrcp_play_status.Length,s_mp3_avrcp_play_status.Position,s_mp3_avrcp_play_status.mediaStatus);

	if(0x00 == s_mp3_avrcp_play_status.mediaStatus ||  0x02 == s_mp3_avrcp_play_status.mediaStatus)
	{
		//stopped or paused
		play_pause_image = IMAGE_COMMON_BTN_PLAY;
		StopAvrcpUpdateTimer();
		StopAvrcpSyncTimer();
	}
	else if(0x01 == s_mp3_avrcp_play_status.mediaStatus)
	{
		//playing
		play_pause_image = IMAGE_COMMON_BTN_PAUSE;
		UpdateAvrcpPlayerTimePosition(0, 0);
		SyncAvrcpPlayerTimePosition(0, 0);
	}

	btn_bg.img_id  = play_pause_image;
    GUIBUTTON_SetFg(MMIMP3_PLAY_BTN_CTRL_ID,&btn_bg);

	GetAPButtonInfo(&button_info);
	button_info.button_valid[MMIMP3_BUTTON_PLAY_PAUSE] = TRUE;
	button_info.button_valid[MMIMP3_BUTTON_PREV] = TRUE;
	button_info.button_valid[MMIMP3_BUTTON_NEXT] = TRUE;
    SetAPButtonInfo(&button_info);

	GUIBUTTON_SetGrayed(MMIMP3_PREV_BTN_CTRL_ID,FALSE,PNULL,FALSE);
	GUIBUTTON_SetGrayed(MMIMP3_NEXT_BTN_CTRL_ID,FALSE,PNULL,FALSE);
	GUIBUTTON_SetGrayed(MMIMP3_PLAY_BTN_CTRL_ID,FALSE,PNULL,FALSE);

	if(MMK_IsFocusWin(MMIMP3_MAIN_PLAY_WIN_ID))
	{
		GUIBUTTON_Update(MMIMP3_PREV_BTN_CTRL_ID);
		GUIBUTTON_Update(MMIMP3_NEXT_BTN_CTRL_ID);
		GUIBUTTON_Update(MMIMP3_PLAY_BTN_CTRL_ID);
	}
}

/*****************************************************************************/
//  Description : Handle AVRCP play Pause down event
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
LOCAL void HandleAvrcpPlayPauseDown(void)
{
	GUI_BG_T btn_bg = {0};
	MMI_IMAGE_ID_T      play_pause_image = IMAGE_COMMON_BTN_PLAY;
	BT_STATUS retVal;

	if(s_mp3_avrcp_lock_operation)
	{
		SCI_TRACE_LOW("BT_AV, HandleAvrcpPlayPauseDown in lock status");
		return;
	}
    LockAvrcpOperation();
    btn_bg.bg_type = GUI_BG_IMG;

    SCI_TRACE_LOW("BT_AV, HandleAvrcpPlayPauseDown mediaStatus:%u",s_mp3_avrcp_play_status.mediaStatus);

	if(0x00 == s_mp3_avrcp_play_status.mediaStatus || 0x02 == s_mp3_avrcp_play_status.mediaStatus)
	{
		//stopped or paused
		SCI_TRACE_LOW("BT_AV, HandleAvrcpPlayPauseDown play");
		retVal = MMIAPIBT_SetAvrcpPanelKey(0x0044,TRUE,TRUE);

		if(BT_SUCCESS != retVal)
		{
			SCI_TRACE_LOW("BT_AV, HandleAvrcpPlayPauseDown play failed");
			return;
		}

        play_pause_image = IMAGE_COMMON_BTN_PAUSE;
		s_mp3_avrcp_play_status.mediaStatus = 0x01;
		UpdateAvrcpPlayerTimePosition(0, 0);
		SyncAvrcpPlayerTimePosition(0, 0);
	}
	else if(0x01 == s_mp3_avrcp_play_status.mediaStatus)
	{
		//playing
		SCI_TRACE_LOW("BT_AV, HandleAvrcpPlayPauseDown pause");
		retVal = MMIAPIBT_SetAvrcpPanelKey(0x0046,TRUE,TRUE);

		if(BT_SUCCESS != retVal)
		{
			SCI_TRACE_LOW("BT_AV, HandleAvrcpPlayPauseDown pause failed");
			return;
		}
	
        play_pause_image = IMAGE_COMMON_BTN_PLAY;
		s_mp3_avrcp_play_status.mediaStatus = 0x02;
		StopAvrcpUpdateTimer();
		StopAvrcpSyncTimer();
	}

    btn_bg.img_id  = play_pause_image;
    GUIBUTTON_SetFg(MMIMP3_PLAY_BTN_CTRL_ID,&btn_bg);
	
	if(MMK_IsFocusWin(MMIMP3_MAIN_PLAY_WIN_ID))
	{
		GUIBUTTON_Update(MMIMP3_PLAY_BTN_CTRL_ID);
	}
}

/*****************************************************************************/
//  Description : Lock AVRCP operation
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
LOCAL void LockAvrcpOperation(void)
{
    if (0 != s_mp3_avrcp_lock_operation_timer_id)
    {
	    MMK_StopTimer(s_mp3_avrcp_lock_operation_timer_id);
	    s_mp3_avrcp_lock_operation_timer_id = 0;
    }
    SCI_TRACE_LOW("BT_AV, LockAvrcpOperation");
    s_mp3_avrcp_lock_operation = TRUE;
    s_mp3_avrcp_lock_operation_timer_id = MMK_CreateTimerCallback(500, UnlockAvrcpOperation, NULL, FALSE);
}

/*****************************************************************************/
//  Description : Unlock AVRCP operation
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
LOCAL void UnlockAvrcpOperation(uint8 timer_id, uint32 param)
{
    SCI_TRACE_LOW("BT_AV, UnlockAvrcpOperation");
	s_mp3_avrcp_lock_operation = FALSE;
}

/*****************************************************************************/
//  Description : Update AVRCP player time position
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
LOCAL void UpdateAvrcpPlayerTimePosition(uint8 timer_id, uint32 param)
{
    if(0x01 != s_mp3_avrcp_play_status.mediaStatus)
    {
        SCI_TRACE_LOW("BT_AV, is not playing");
        return;
    }
    
    StopAvrcpUpdateTimer();

    s_mp3_avrcp_update_timer_id = MMK_CreateTimerCallback(1000, UpdateAvrcpPlayerTimePosition, NULL, FALSE);

//bug926085
#if 0
    s_mp3_avrcp_play_status.Position++;
#endif
//bug926085

    if(s_mp3_avrcp_play_status.Position >= s_mp3_avrcp_play_status.Length)
    {
        s_mp3_avrcp_play_status.Position = s_mp3_avrcp_play_status.Length;
    }
    
    if(MMK_IsFocusWin(MMIMP3_MAIN_PLAY_WIN_ID))
    {
        SCI_TRACE_LOW("BT_AV, UpdateAvrcpPlayerTimePosition position:%u",s_mp3_avrcp_play_status.Position);
        GUIOWNDRAW_Update(MMIMP3_TIME_INFO_OWNER_DRAW_CTRL_ID);
        GUIOWNDRAW_Update(MMIMP3_PROGRESS_OWNER_DRAW_CTRL_ID);
    }
}

/*****************************************************************************/
//  Description : Sync AVRCP player time position
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
LOCAL void SyncAvrcpPlayerTimePosition(uint8 timer_id, uint32 param)
{
	StopAvrcpSyncTimer();
	SCI_TRACE_LOW("BT_AV, create avrcp sync timer");
    //bug926085
	s_mp3_avrcp_sync_timer_id = MMK_CreateTimerCallback(1000, SyncAvrcpPlayerTimePosition, NULL, FALSE);
    //bug926085
    MMIAPIBT_GetMdaPlayStatus();
}

/*****************************************************************************/
//  Description : Stop AVRCP update timer
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
LOCAL void StopAvrcpUpdateTimer(void)
{
	if (0 != s_mp3_avrcp_update_timer_id)
	{
		SCI_TRACE_LOW("BT_AV, StopAvrcpUpdateTimer");
		MMK_StopTimer(s_mp3_avrcp_update_timer_id);
		s_mp3_avrcp_update_timer_id = 0;
	}
}

/*****************************************************************************/
//  Description : Stop AVRCP sync timer
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
LOCAL void StopAvrcpSyncTimer(void)
{
	if (0 != s_mp3_avrcp_sync_timer_id)
	{
		SCI_TRACE_LOW("BT_AV, StopAvrcpSyncTimer");
		MMK_StopTimer(s_mp3_avrcp_sync_timer_id);
		s_mp3_avrcp_sync_timer_id = 0;
	}
}

/*****************************************************************************/
//  Description : init avrcp variables
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
LOCAL void InitAvrcpVariables(void)
{
    char    cstr_track[MMIMP3_DISPLAY_TIME_LEN +1] = {0};

    s_mp3_avrcp_play_status.mediaStatus = 0x00;
    s_mp3_avrcp_play_status.Length = 0;
    s_mp3_avrcp_play_status.Position = 0;

    if(PNULL != s_mp3_avrcp_title_name.wstr_ptr)
    {
        SCI_MEMSET(s_mp3_avrcp_title_name.wstr_ptr,0, ( MAX_TG_PLAYER_STRING_LEN + 1 ) * sizeof(wchar) );
        s_mp3_avrcp_title_name.wstr_len = 0;
    }
    if(PNULL != s_mp3_avrcp_artist_name.wstr_ptr)
    {
        SCI_MEMSET(s_mp3_avrcp_artist_name.wstr_ptr,0, ( MAX_TG_PLAYER_STRING_LEN + 1 ) * sizeof(wchar) );
        s_mp3_avrcp_artist_name.wstr_len = 0;
    }
    if(PNULL != s_mp3_avrcp_album_name.wstr_ptr)
    {
        SCI_MEMSET(s_mp3_avrcp_album_name.wstr_ptr,0, ( MAX_TG_PLAYER_STRING_LEN + 1 ) * sizeof(wchar) );
        s_mp3_avrcp_album_name.wstr_len = 0;
    }
    if(PNULL != s_mp3_avrcp_track_info.wstr_ptr)
    {
        SCI_MEMSET(s_mp3_avrcp_track_info.wstr_ptr,0, ( MMIMP3_DISPLAY_TIME_LEN + 1 ) * sizeof(wchar) );
        SCI_MEMSET(cstr_track,0,MMIMP3_DISPLAY_TIME_LEN +1);
        sprintf(cstr_track, "0/0");
        MMIAPICOM_StrToWstr((uint8*)cstr_track, s_mp3_avrcp_track_info.wstr_ptr);
        s_mp3_avrcp_track_info.wstr_len = strlen(cstr_track);
#ifdef MMI_AUDIOPLAYER_MINI_SUPPORT
        GUITITLE_SetSubText(MMIMP3_MAIN_PLAY_WIN_ID, MMITHEME_GetTitleCtrlId(), TRUE, s_mp3_avrcp_track_info.wstr_ptr, s_mp3_avrcp_track_info.wstr_len, 1);
#else
        GUILABEL_SetText(MMIMP3_FILE_INFO_LABLE_CTRL_ID, &s_mp3_avrcp_track_info, TRUE);
#endif
     }
}

/*****************************************************************************/
//  Description : Update A2DP and AVRCP connect status
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMp3_UpdateA2dpAvrcpConnectionStatus(BOOLEAN a2dp, BOOLEAN avrcp)
{
	s_mp3_a2dp_sink_connected = a2dp;
	//s_mp3_is_avrcp_ct_connected = avrcp;

	if(s_mp3_a2dp_sink_connected)
	{
		SCI_TRACE_LOW("BT_AV, a2dp connected!");
		
		s_mp3_is_bt_music = TRUE;
        InitAvrcpVariables();
		
		if(MMIMP3_PLAYER_AUDIO_ACTIVE == MMIAPIMP3_GetPlayerState() && MMIMP3_AUDIO_PLAYING == MMIMP3_GetAudioState())
		{
			SCI_TRACE_LOW("BT_AV, pause local music");
			MMIAPIMP3_PauseMp3(FALSE);
			s_mp3_is_resume_local_music = TRUE;
		}
	}
	else
	{
		SCI_TRACE_LOW("BT_AV, a2dp disconnected!");
	
		s_mp3_is_bt_music = FALSE;
        s_mp3_avrcp_lock_operation = FALSE;	
		
		if(MMIMP3_PLAYER_AUDIO_ACTIVE == MMIAPIMP3_GetPlayerState() && MMIMP3_AUDIO_PAUSED == MMIMP3_GetAudioState() && s_mp3_is_resume_local_music)
		{
			SCI_TRACE_LOW("BT_AV, resume local music");
			MMIAPIMP3_ResumeMp3(FALSE);
			s_mp3_is_resume_local_music = FALSE;
		}
		
		StopAvrcpUpdateTimer();
		StopAvrcpSyncTimer();
	}

	if (MMK_IsOpenWin(MMIMP3_PLAYER_OPT_MENU_WIN_ID))
    {
		SCI_TRACE_LOW("BT_AV, update options menu");
		MMIAP_OpenPlayerOptMenu(MMIMP3_MAIN_PLAY_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : Update MP3 player status
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMp3_UpdatePlayerStatus(AvrcpCtPlayStatus status)
{	
	BOOLEAN update = FALSE;
	uint32 length = status.Length;
	uint32 position = status.Position;
	uint8 mediaStatus = status.mediaStatus;

	SCI_TRACE_LOW("BT_AV, MMIAPIMp3_UpdatePlayerStatus length:%u, position:%u, mediaStatus:%u, s_mp3_avrcp_lock_operation:%u",length,position,mediaStatus,s_mp3_avrcp_lock_operation);

	//if(s_mp3_avrcp_lock_operation)
	//{
		//SCI_TRACE_LOW("BT_AV, MMIAPIMp3_UpdatePlayerStatus in lock status");
		//return;
	//}

	SCI_TRACE_LOW("BT_AV, MMIAPIMp3_UpdatePlayerStatus before length:%u, position:%u, mediaStatus:%u",s_mp3_avrcp_play_status.Length,s_mp3_avrcp_play_status.Position,s_mp3_avrcp_play_status.mediaStatus);

	if(s_mp3_avrcp_play_status.mediaStatus != mediaStatus)
		update = TRUE;

	if(length != 0xFFFFFFFF && length > 0)
	{
		s_mp3_avrcp_play_status.Length = length/1000;
	}
	if(position != 0xFFFFFFFF && position > 0)
	{
		s_mp3_avrcp_play_status.Position = position/1000;
	}
	
    //s_mp3_avrcp_play_status.mediaStatus = mediaStatus;
	//if(update)
	    //UpdateAvrcpPlayerButton();

	SCI_TRACE_LOW("BT_AV, MMIAPIMp3_UpdatePlayerStatus after length:%u, position:%u, mediaStatus:%u",s_mp3_avrcp_play_status.Length,s_mp3_avrcp_play_status.Position,s_mp3_avrcp_play_status.mediaStatus);

    if(MMK_IsFocusWin(MMIMP3_MAIN_PLAY_WIN_ID))
    {
        SCI_TRACE_LOW("BT_AV, MMIAPIMp3_UpdatePlayerStatus position update");
        GUIOWNDRAW_Update(MMIMP3_TIME_INFO_OWNER_DRAW_CTRL_ID);
        GUIOWNDRAW_Update(MMIMP3_PROGRESS_OWNER_DRAW_CTRL_ID);
    }
}

/*****************************************************************************/
//  Description : Update MP3 player attributes
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMp3_UpdatePlayerAttributes(AvrcpCtElementAttributes attr)
{
	/* attriId	0x1	Title
	 * 			0x2	Artist Name
	 * 			0x3	Album Name
	 * 			0x4	Track Number
	 * 			0x5	Total Nunmber of Tracks
	 * 			0x6	Genre
	 * 			0x7	Playing Time(milliseconds)
	 */
	AvrcpCtElementAttributes attributes = attr;

	char    cstr_track[MMIMP3_DISPLAY_TIME_LEN +1] = {0};
	uint16 avrcp_track_number = 0;
	uint16 avrcp_total_tracks = 0;

	int i=0;

	SCI_TRACE_LOW("MMIMP3, _UpdatePlayerAttributes numIds: %d",attributes.numIds);

	if(FALSE == s_mp3_is_allocate_avrcp_string_memory)
		AllocateAvrcpStringMemory();

	if(PNULL != s_mp3_avrcp_title_name.wstr_ptr)
	{
		SCI_MEMSET(s_mp3_avrcp_title_name.wstr_ptr,0, ( MAX_TG_PLAYER_STRING_LEN + 1 ) * sizeof(wchar) );
        s_mp3_avrcp_title_name.wstr_len = 0;		
	}
	if(PNULL != s_mp3_avrcp_artist_name.wstr_ptr)
	{
		SCI_MEMSET(s_mp3_avrcp_artist_name.wstr_ptr,0, ( MAX_TG_PLAYER_STRING_LEN + 1 ) * sizeof(wchar) );
        s_mp3_avrcp_artist_name.wstr_len = 0;		
	}
	if(PNULL != s_mp3_avrcp_album_name.wstr_ptr)
	{
		SCI_MEMSET(s_mp3_avrcp_album_name.wstr_ptr,0, ( MAX_TG_PLAYER_STRING_LEN + 1 ) * sizeof(wchar) );
        s_mp3_avrcp_album_name.wstr_len = 0;		
	}
	if(PNULL != s_mp3_avrcp_track_info.wstr_ptr)
	{
		SCI_MEMSET(s_mp3_avrcp_track_info.wstr_ptr,0, ( MMIMP3_DISPLAY_TIME_LEN + 1 ) * sizeof(wchar) );
        s_mp3_avrcp_track_info.wstr_len = 0;		
	}

    for(i=0; i<attributes.numIds; i++)
	{
		SCI_TRACE_LOW("BT_AV, attrId: %d",attributes.txt[i].attrId);
		SCI_TRACE_LOW("BT_AV, charSet: %d",attributes.txt[i].charSet);
		SCI_TRACE_LOW("BT_AV, length: %d",attributes.txt[i].length);
		SCI_TRACE_LOW("BT_AV, string: %s",attributes.txt[i].string);

        if(attributes.txt[i].length >= (MAX_TG_PLAYER_STRING_LEN * sizeof(wchar)))
        {
            attributes.txt[i].length = MAX_TG_PLAYER_STRING_LEN * sizeof(wchar);
        }

		switch(attributes.txt[i].attrId){
			case 0x01:
				//Title name
				GUI_UTF8ToWstr(s_mp3_avrcp_title_name.wstr_ptr, attributes.txt[i].length, (uint8*)attributes.txt[i].string, attributes.txt[i].length);
				s_mp3_avrcp_title_name.wstr_len = attributes.txt[i].length;
				break;
			case 0x02:
				//Artist name
				GUI_UTF8ToWstr(s_mp3_avrcp_artist_name.wstr_ptr, attributes.txt[i].length, (uint8*)attributes.txt[i].string, attributes.txt[i].length);
				s_mp3_avrcp_artist_name.wstr_len = attributes.txt[i].length;
				break;
			case 0x03:
				//Ablum name
				GUI_UTF8ToWstr(s_mp3_avrcp_album_name.wstr_ptr, attributes.txt[i].length, (uint8*)attributes.txt[i].string, attributes.txt[i].length);
				s_mp3_avrcp_album_name.wstr_len = attributes.txt[i].length;
				break;
			case 0x04:
				//Track number
				SCI_MEMCPY(cstr_track,attributes.txt[i].string,attributes.txt[i].length);
				if('-' == cstr_track[0] || '0' == cstr_track[0]) //negative number
					avrcp_track_number = 1;
				else
					avrcp_track_number = atoi(cstr_track);

				SCI_TRACE_LOW("BT_AV,  avrcp_track_number: %d",avrcp_track_number);
				break;
			case 0x05:
				//Total number of tracks
				SCI_MEMCPY(cstr_track,attributes.txt[i].string,attributes.txt[i].length);	
				if('-' == cstr_track[0] || '0' == cstr_track[0]) //negative number
					avrcp_total_tracks = 1;
				else
					avrcp_total_tracks = atoi(cstr_track);
				
				SCI_TRACE_LOW("BT_AV,  avrcp_total_tracks: %d",avrcp_total_tracks);
				SCI_MEMSET(cstr_track,0,MMIMP3_DISPLAY_TIME_LEN +1);
				sprintf(cstr_track, "%d/%d", avrcp_track_number, avrcp_total_tracks);
				SCI_TRACE_LOW("BT_AV,  cstr_track: %s",cstr_track);
				
				MMIAPICOM_StrToWstr((uint8*)cstr_track, s_mp3_avrcp_track_info.wstr_ptr);
				s_mp3_avrcp_track_info.wstr_len = strlen(cstr_track);
				break;
			default:
				break;
		}
	}

	UpdateAvrcpPlayerDisplay();
	UpdateAvrcpPlayerTimePosition(0, 0);
}

/*****************************************************************************/
//  Description : Update MP3 player playback status
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMp3_UpdatePlayback(uint8 status)
{
	//if(s_mp3_avrcp_lock_operation)
	//{
		//SCI_TRACE_LOW("BT_AV, MMIAPIMp3_UpdatePlayback in lock status");
		//return;
	//}
	s_mp3_avrcp_play_status.mediaStatus = status;
	SCI_TRACE_LOW("BT_AV, MMIAPIMp3_UpdatePlayback set status:%d",s_mp3_avrcp_play_status.mediaStatus);
	UpdateAvrcpPlayerButton();
}

/*****************************************************************************/
//  Description : Update MP3 player playing position
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMp3_UpdatePlayPosition(uint32 position)
{
	s_mp3_avrcp_play_status.Position = position/1000;
	SCI_TRACE_LOW("BT_AV, MMIAPIMp3_UpdatePlayPosition set position:%u",s_mp3_avrcp_play_status.Position);
}

/*****************************************************************************/
//  Description : AVRCP Track changed
//  Global resource dependence :
//  Author: kay.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMp3_AvrcpTrackChanged(void)
{

}
#endif

/*****************************************************************************/
//  Description :headset plug in or out
//  Global resource dependence :
//  Author: chunjuan.liang
//  Note:bug909351
/*****************************************************************************/
PUBLIC void MMIAPIMp3_HeadsetInd(MMI_MESSAGE_ID_E msg_id)
{
    if(MSG_KEYUP_HEADSET_DETECT == msg_id)
    {
        if(MMIAPIMP3_IsPlayerPlaying())
        {
            MMIAP_HandleMp3PlayPauseButton();
        }
    }
}

/*Edit by script, ignore 4 case. Thu Apr 26 19:00:49 2012*/ //IGNORE9527


/*Edit by script, ignore 26 case. Fri Apr 27 09:38:48 2012*/ //IGNORE9527
