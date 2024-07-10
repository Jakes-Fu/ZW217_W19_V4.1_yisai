/*****************************************************************************
** File Name:      mmiap_appmain.c                                           *
** Author:                                                                   *
** Date:           28/08/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to audio player                         *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2012       robert.wang       Create                                    *
******************************************************************************/

#define _MMIAP_MAIN_C_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmipub.h"
#include "mmi_text.h"
#include "mmi_appmsg.h"
#include "mmi_applet_table.h"

//other module
#include "mmicc_export.h"
#include "mmiacc_text.h"

#include "mmiudisk_export.h"
#include "mmisd_export.h"
#include "mmieng_export.h"


//local module
#include "mmiapwin_main.h"
#include "mmimp3_id.h"
#include "mmimp3_export.h"
#include "mmiap_play.h"
#include "mmimp3_menutable.h"
#include "mmimp3_nv.h"
#include "mmiap_applet.h"

#ifdef MMIAP_LYRIC_SUPPORT
#include "mmiap_lyric_internal.h"
#endif

#ifdef MMIAP_SPECTRUM_SUPPORT
#include "mmiap_spectrum.h"
#endif

#ifdef MMIAP_F_HISTORY_PLAYLIST
#include "mmiap_lastinfo_ctrl.h"
#endif

#include "mmiap_appmain.h"
#include "mmi_applet_table.h"


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init all ap info
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InitAllAPInfo(
    void
);

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
LOCAL MMIAP_INFO_T *s_mmiap_info_ptr = PNULL;
LOCAL BOOLEAN s_ap_is_bg_start = FALSE;

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get ap info ptr
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIAP_INFO_T *GetApInfoPtr(void)
{
    return s_mmiap_info_ptr;
}

/*****************************************************************************/
//  Description : get ap open player info ptr
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIAP_OPEN_PLAYER_INFO_T *GetApOpenPlayerInfoPtr(void)
{
    MMIAP_INFO_T *info_ptr = GetApInfoPtr();
    MMIAP_OPEN_PLAYER_INFO_T *open_ptr = PNULL;


    if (PNULL != info_ptr)
    {
        open_ptr = info_ptr->open_player_info_ptr;
    }

    return open_ptr;
}

/*****************************************************************************/
//  Description : create ap info
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateAPInfo(void)
{
    BOOLEAN ret = FALSE;
    MMIAP_INFO_T *info_ptr = GetApInfoPtr();
    uint32 info_size = sizeof(*info_ptr);


    if (PNULL == info_ptr)
    {
        info_ptr = SCI_ALLOCA(info_size);
    }

    if (PNULL != info_ptr)
    {
        s_mmiap_info_ptr = info_ptr;

        SCI_MEMSET(info_ptr, 0, info_size);

        ret = TRUE;
    }

    return ret;
}

/*****************************************************************************/
//  Description : destroy ap info
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DestroyAPInfo(void)
{

    MMIAP_DestroyAPPlayCtrlInfo();

#ifdef MMIAP_LYRIC_SUPPORT
    MMIAP_DestroyLyricInfo();
#endif

#ifdef MMIAP_SPECTRUM_SUPPORT
    MMIAP_DestroySpectrumInfo();
#endif


    if (PNULL != s_mmiap_info_ptr)
    {
        if (PNULL != s_mmiap_info_ptr->open_player_info_ptr)
        {
            SCI_FREE(s_mmiap_info_ptr->open_player_info_ptr);
        }

        SCI_FREE(s_mmiap_info_ptr);
    }


    MMI_CheckAllocatedMemInfo();
}

/*****************************************************************************/
//  Description : check environment
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckEnvironment(
    void
)
{
    BOOLEAN ret = FALSE;


    do
    {
        if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
        {
            MMIPUB_OpenAlertWarningWin(TXT_CALL_USING);
            break;
        }
        else if(MMIAPIUDISK_UdiskIsRun())
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
            break;
        }
        else if(MMIAPISD_IsCardLogOn())
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_CARDLOG_USING);
            break;
        }
        else if(MMIAPIENG_GetIQModeStatus())
        {
            MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
            break;
        }

        ret = TRUE;
    }
    while(0);



    SCI_TRACE_LOW("[MMIAP] CheckEnvironment ret=%d",\
                  ret);

    return ret;
}

/*****************************************************************************/
//  Description : open mp3 player
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_OpenMp3Player(
    void
)
{
    BOOLEAN ret = FALSE;
    MMIAP_INFO_T *info_ptr = GetApInfoPtr();



    SCI_TRACE_LOW("[MMIAP] MMIAPIMP3_OpenMp3Player");


    //check environment
    ret = CheckEnvironment();
    if (!ret)
    {
        return;
    }


    //检查是否已经打开
    if(PNULL != info_ptr) //resume the mp3 applet
    {
        //关闭已经打开的所有窗口
        MMK_CloseMiddleWinByApplet(SPRD_MP3_APPLET_ID, MMIMP3_MAIN_PLAY_WIN_ID + 1,MMIMP3_MAX_WIN_ID);


        MMIAPIMP3_OpenAudioPlayerAfterCheck();
        return;
    }

    ret = InitAllAPInfo();
    if (!ret)
    {
        //销毁ap info
        MMIAP_DestroyAPInfo();

        return;
    }

    //entry audio player
    MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_AUDIO_PLAYER, MMIAPIMP3_OpenAudioPlayerAfterCheck);
}

/*****************************************************************************/
//  Description : reg win info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_RegWinInfo
(
    MMIAP_WIN_REQ_PARAM_T *param_ptr //out
)
{
    BOOLEAN ret = FALSE;
    MMIAP_INFO_T *info_ptr = GetApInfoPtr();
    MMIAP_WIN_TYPE_E win_type = 0;


    if (PNULL == param_ptr
            || PNULL == info_ptr
       )
    {
        return ret;
    }

    win_type = param_ptr->win_type;
    if (MMIAP_WIN_TYPE_MAX <= win_type)
    {
        return ret;
    }

    info_ptr->wins_info[win_type].win_ptr = param_ptr->win_ptr;
    info_ptr->wins_info[win_type].win_callback_func = param_ptr->win_callback_func;


    SCI_TRACE_LOW("[MMIAP] MMIAP_RegWinInfo win_ptr=0x%X, callback_func=0x%X",\
                  param_ptr->win_ptr, param_ptr->win_callback_func);

    ret = TRUE;

    return ret;
}

/*****************************************************************************/
//  Description : do win callback
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIAP_WIN_CALLBACK_RET_E MMIAP_DoWinCallback
(
    MMIAP_WIN_TYPE_E which_win, //in
    MMIAP_WIN_FLAG_T flag,//in
    void* param_ptr, //in
    uint32 size_of_param //in
)
{
    MMIAP_WIN_CALLBACK_RET_E ret = MMIAP_WIN_CALLBACK_RET_CONTINUE;

    MMIAP_INFO_T *info_ptr = GetApInfoPtr();
    uint32 *win_ptr = PNULL;
    MMIAP_WIN_CALLBACK cb_func = PNULL;
    MMIAP_WIN_CALLBACK_PARAM_T cb_info = {0};
    uint32 size_of_cb_info = 0;




    if (PNULL == info_ptr)
    {
        return ret;
    }

    SCI_TRACE_LOW("[MMIAP] MMIAP_DoWinCallback which_win=%d",\
                  which_win);


    if (MMIAP_WIN_TYPE_MAX <= which_win)
    {
        return ret;
    }

    cb_func = info_ptr->wins_info[which_win].win_callback_func;
    if (PNULL != cb_func)
    {
        win_ptr = info_ptr->wins_info[which_win].win_ptr;

        size_of_cb_info = sizeof(cb_info);
        cb_info.win_ptr = win_ptr;
        cb_info.win_flag = flag;
        cb_info.param_ptr = param_ptr;
        cb_info.size_of_param = size_of_param;

        ret = cb_func(&cb_info, size_of_cb_info);
    }

    SCI_TRACE_LOW("[MMIAP] MMIAP_DoWinCallback win_ptr=0x%X, callback_func=0x%X",\
                  win_ptr, cb_func);

    SCI_TRACE_LOW("[MMIAP] MMIAP_DoWinCallback ret=%d",\
                  ret);

    return ret;
}

/*****************************************************************************/
//  Description : alloc player info
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AllocPlayerInfo(
    MMIAP_OPEN_PLAYER_INFO_T **open_info_pptr //in
)
{
    BOOLEAN ret = FALSE;

    MMIAP_INFO_T *ap_info_ptr = GetApInfoPtr();
    MMIAP_OPEN_PLAYER_INFO_T *info_ptr = PNULL;
    uint32 size = 0;


    if (PNULL == ap_info_ptr
            || PNULL == open_info_pptr
       )
    {
        return ret;
    }

    info_ptr = ap_info_ptr->open_player_info_ptr;
    size = sizeof(*info_ptr);

    if (PNULL == info_ptr)
    {
        info_ptr = SCI_ALLOCA(size);
    }
    if (PNULL != info_ptr)
    {
        ret = TRUE;

        SCI_MEMSET(info_ptr, 0, size);
        ap_info_ptr->open_player_info_ptr = info_ptr;

        *open_info_pptr = info_ptr;
    }


    return ret;
}

/*****************************************************************************/
//  Description : init player info
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_InitPlayerInfo(
    MMIAP_OPEN_PLAYER_PARAM_T *open_param_ptr //in
)
{
    BOOLEAN ret = FALSE;
    MMIAP_OPEN_PLAYER_INFO_T *info_ptr = PNULL;


    ret = AllocPlayerInfo(&info_ptr);
    if (!ret)
    {
        return ret;
    }

    if (MMIMP3_OPEN_INFO_FMM_DATA == open_param_ptr->open_type)
    {
        info_ptr->open_type = open_param_ptr->open_type;
        info_ptr->file_info_u.fmm_data_info.file_info_ptr = open_param_ptr->file_info_u.fmm_data_info.file_info_ptr;
        info_ptr->file_info_u.fmm_data_info.file_num = open_param_ptr->file_info_u.fmm_data_info.file_num;
        info_ptr->file_info_u.fmm_data_info.file_index = open_param_ptr->file_info_u.fmm_data_info.file_index;
        info_ptr->file_info_u.fmm_data_info.dir_path_ptr = open_param_ptr->file_info_u.fmm_data_info.dir_path_ptr;
        info_ptr->file_info_u.fmm_data_info.dir_path_len = open_param_ptr->file_info_u.fmm_data_info.dir_path_len;
    }
    else
    {
        uint32 file_name_len = 0;

        file_name_len = open_param_ptr->file_info_u.file_data_info.file_name_len;

        info_ptr->open_type =  open_param_ptr->open_type;
        MMI_WSTRNCPY(info_ptr->file_info_u.file_data_info.file_name,
                     FMM_SEARCH_FILENAME_MAXLEN,
                     open_param_ptr->file_info_u.file_data_info.file_name,
                     file_name_len,
                     file_name_len);
        info_ptr->file_info_u.file_data_info.file_name_len = file_name_len;
    }


    ret = TRUE;

    SCI_TRACE_LOW("[MMIAP] MMIAP_InitPlayerInfo open_type=%d",
                  open_param_ptr->open_type);

    return ret;
}

/*****************************************************************************/
//  Description : Play Fmm music.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_PlayFmmMusic(
    MMIFMM_DATA_INFO_T  *file_info_ptr,
    uint16              file_num,
    uint16              file_index,
    wchar               *dir_path_ptr,
    uint16              dir_path_len
)
{
    BOOLEAN result = TRUE;
    BOOLEAN is_need_init = FALSE;
    BOOLEAN is_playfile = TRUE;
    MMIAP_OPEN_PLAYER_PARAM_T open_param = {0};
    MMIAP_INFO_T *info_ptr = GetApInfoPtr();



    SCI_TRACE_LOW("[MMIAP] MMIAPIMP3_PlayFmmMusic");


    if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        MMIPUB_OpenAlertWarningWin(TXT_CALL_USING);
        return FALSE;
    }



    //检查是否已经打开
    if(PNULL == info_ptr)
    {
        is_need_init = TRUE;

        result = InitAllAPInfo();
        if (!result)
        {
            //销毁ap info
            MMIAP_DestroyAPInfo();

            MMIPUB_OpenAlertWarningWin(TXT_COMM_OPEN_FAIL);
            return result;
        }
    }
    else
    {
        MMIAPIMP3_StopAudioPlayer();
    }

    open_param.open_type = MMIMP3_OPEN_INFO_FMM_DATA;
    open_param.file_info_u.fmm_data_info.file_info_ptr = file_info_ptr;
    open_param.file_info_u.fmm_data_info.file_num = file_num;
    open_param.file_info_u.fmm_data_info.file_index = file_index;
    open_param.file_info_u.fmm_data_info.dir_path_ptr = dir_path_ptr;
    open_param.file_info_u.fmm_data_info.dir_path_len = dir_path_len;
    result = MMIAP_InitPlayerInfo(&open_param);
    if (!result)
    {
        //销毁ap info
        MMIAP_DestroyAPInfo();

        MMIPUB_OpenAlertWarningWin(TXT_COMM_OPEN_FAIL);
        return result;
    }


    MMIAP_OpenMainWin(is_playfile);


    if(is_need_init)
    {
        MMIAP_SetNeedInitPlayer(TRUE);
    }
    else
    {
        //关闭已经打开的所有窗口
        MMK_CloseMiddleWinByApplet(SPRD_MP3_APPLET_ID, MMIMP3_MAIN_PLAY_WIN_ID + 1,MMIMP3_MAX_WIN_ID);

        MMIAP_SetNeedInitPlayer(FALSE);
        MMK_SendMsg(MMIMP3_MAIN_PLAY_WIN_ID, MSG_MP3_INIT_WAIT, (DPARAM)&is_playfile);
    }

    return result;
}

/*****************************************************************************/
//  Description : Play Fmm music file
//  Global resource dependence :
//  Author:aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_PlayFile(const wchar *file_name_ptr,
                                  const uint32 filename_len
                                 )
{
    BOOLEAN result = TRUE;
    BOOLEAN is_need_init = FALSE;
    BOOLEAN is_playfile = TRUE;
    MMIAP_OPEN_PLAYER_PARAM_T open_param = {0};
    MMIAP_INFO_T *info_ptr = GetApInfoPtr();



    if(PNULL == file_name_ptr
            || 0 == filename_len)
    {
        return FALSE;
    }


    SCI_TRACE_LOW("[MMIAP] MMIAPIMP3_PlayFile");


    //检查是否已经打开
    if(PNULL == info_ptr)
    {
        is_need_init = TRUE;
        result = InitAllAPInfo();
        if (!result)
        {
            //释放AP
            MMIAP_DestroyAPInfo();

            MMIPUB_OpenAlertWarningWin(TXT_COMM_OPEN_FAIL);
            return result;
        }
    }
    else
    {
        MMIAPIMP3_StopAudioPlayer();
    }

    open_param.open_type = MMIMP3_OPEN_INFO_SINGLE_FILE;
    MMI_WSTRNCPY(open_param.file_info_u.file_data_info.file_name,
                 FMM_SEARCH_FILENAME_MAXLEN,
                 file_name_ptr,
                 filename_len,
                 filename_len);
    open_param.file_info_u.file_data_info.file_name_len = filename_len;

    result = MMIAP_InitPlayerInfo(&open_param);
    if (!result)
    {
        //释放AP
        MMIAP_DestroyAPInfo();

        MMIPUB_OpenAlertWarningWin(TXT_COMM_OPEN_FAIL);
        return result;
    }



    MMIAP_OpenMainWin(is_playfile);


    if(is_need_init)
    {
        MMIAP_SetNeedInitPlayer(TRUE);
    }
    else
    {
        //关闭已经打开的所有窗口
        MMK_CloseMiddleWinByApplet(SPRD_MP3_APPLET_ID, MMIMP3_MAIN_PLAY_WIN_ID + 1,MMIMP3_MAX_WIN_ID);

        MMIAP_SetNeedInitPlayer(FALSE);
        MMK_SendMsg(MMIMP3_MAIN_PLAY_WIN_ID, MSG_MP3_INIT_WAIT, (DPARAM)&is_playfile);
    }

    return result;
}

/*****************************************************************************/
//  Description : set need init player flag
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetNeedInitPlayer(
    BOOLEAN is_need //in
)
{
    MMIAP_OPEN_PLAYER_INFO_T *open_ptr = GetApOpenPlayerInfoPtr();


    if (PNULL != open_ptr)
    {
        open_ptr->is_need_init_player = is_need;
    }

    SCI_TRACE_LOW("[MMIAP] MMIAP_SetNeedInitPlayer is_need=%d",\
                  is_need);
}

/*****************************************************************************/
//  Description : get need init player flag
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsNeedInitPlayer(
    void
)
{
    BOOLEAN is_need = FALSE;
    MMIAP_OPEN_PLAYER_INFO_T *open_ptr = GetApOpenPlayerInfoPtr();


    if (PNULL != open_ptr)
    {
        is_need = open_ptr->is_need_init_player;
    }


    SCI_TRACE_LOW("[MMIAP] MMIAP_IsNeedInitPlayer is_need=%d",\
                  is_need);

    return is_need;
}
#if 0
/*****************************************************************************/
//  Description : set play file flag
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetPlayFile(
    BOOLEAN is_play_file //in
)
{
    MMIAP_INFO_T *info_ptr = GetApInfoPtr();


    if (PNULL != info_ptr)
    {
        info_ptr->is_play_file = is_play_file;
    }

    SCI_TRACE_LOW("[MMIAP] MMIAP_SetPlayFile is_play_file=%d",\
                  is_play_file);
}

/*****************************************************************************/
//  Description : get play file flag
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsPlayFile(
    void
)
{
    BOOLEAN is_play_file = FALSE;
    MMIAP_INFO_T *info_ptr = GetApInfoPtr();


    if (PNULL != info_ptr)
    {
        is_play_file = info_ptr->is_play_file;
    }


    SCI_TRACE_LOW("[MMIAP] MMIAP_IsPlayFile is_play_file=%d",\
                  is_play_file);

    return is_play_file;
}
#endif


/*****************************************************************************/
//  Description : get open player info param
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_GetOpenPlayerParam(
    MMIAP_OPEN_PLAYER_PARAM_T *open_param_ptr //out
)
{
    BOOLEAN ret = FALSE;
    MMIAP_OPEN_PLAYER_INFO_T *info_ptr = GetApOpenPlayerInfoPtr();


    if (PNULL == info_ptr)
    {
        return ret;
    }

    if (MMIMP3_OPEN_INFO_FMM_DATA == info_ptr->open_type)
    {
        open_param_ptr->open_type = info_ptr->open_type;
        open_param_ptr->file_info_u.fmm_data_info.file_info_ptr = info_ptr->file_info_u.fmm_data_info.file_info_ptr;
        open_param_ptr->file_info_u.fmm_data_info.file_num = info_ptr->file_info_u.fmm_data_info.file_num;
        open_param_ptr->file_info_u.fmm_data_info.file_index = info_ptr->file_info_u.fmm_data_info.file_index;
        open_param_ptr->file_info_u.fmm_data_info.dir_path_ptr = info_ptr->file_info_u.fmm_data_info.dir_path_ptr;
        open_param_ptr->file_info_u.fmm_data_info.dir_path_len = info_ptr->file_info_u.fmm_data_info.dir_path_len;
    }
    else
    {
        uint32 file_name_len = 0;

        file_name_len = info_ptr->file_info_u.file_data_info.file_name_len;

        open_param_ptr->open_type = info_ptr->open_type;
        MMI_WSTRNCPY(open_param_ptr->file_info_u.file_data_info.file_name,
                     FMM_SEARCH_FILENAME_MAXLEN,
                     info_ptr->file_info_u.file_data_info.file_name,
                     file_name_len,
                     file_name_len);
        open_param_ptr->file_info_u.file_data_info.file_name_len = file_name_len;
    }


    SCI_TRACE_LOW("[MMIAP] MMIAP_GetOpenPlayerParam open_type=%d",
                  open_param_ptr->open_type);

    ret = TRUE;

    return ret;
}

/*****************************************************************************/
//  Description : init all ap info
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InitAllAPInfo(
    void
)
{
    BOOLEAN ret = FALSE;


    MMI_CheckAllocatedMemInfo();

    do
    {
        ret = CreateAPInfo();
        if (!ret)
        {
            break;
        }

        ret = MMIAP_InitPlayCtrlInfo();
        if (!ret)
        {
            break;
        }

#ifdef MMIAP_LYRIC_SUPPORT
        ret = MMIAP_InitLyricInfo();
        if (!ret)
        {
            break;
        }
#endif

#ifdef MMIAP_SPECTRUM_SUPPORT
        ret = MMIAP_InitSpectrumInfo();
        if (!ret)
        {
            break;
        }
#endif

        //初始化文件后缀名
        MMIAP_InitFileFilter();
    }
    while(0);


    SCI_TRACE_LOW("[MMIAP] InitAllAPInfo ret=%d",\
                  ret);

    return ret;
}

/*****************************************************************************/
//  Description : initialize mp3 module
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_InitModule(void)
{
    //register menu
    MMIMP3_RegMenuGroup();
    //register nv
    MMIMP3_RegNv();

    MMIMP3_RegWinIdNameArr();

    MMIMP3_RegAppletInfo();
}

/*****************************************************************************/
//  Description : initialize Mp3
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_Init(void)
{

}

/*****************************************************************************/
//  Description : start mp3 by background
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIAP_StartByBG(void)
{
#ifdef MMIAP_F_HISTORY_PLAYLIST

    SCI_TRACE_LOW("[MMIMP3] MMIAPIAP_StartByBG");

    MMIAP_SetBgStartFlag(TRUE);

    MMIAP_OpenMainWin(FALSE);
    MMIAPIMP3_StopApplet(TRUE);
#endif
}

/*****************************************************************************/
//  Description : destroy mp3 by background
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIAP_DestroyByBG(void)
{
    SCI_TRACE_LOW("[MMIMP3] MMIAPIAP_DestroyByBG");

    MMIAPIMP3_StopApplet(FALSE);
}

/*****************************************************************************/
//  Description : set bg start flag
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetBgStartFlag(BOOLEAN is_bg_start)
{
    s_ap_is_bg_start = is_bg_start;
    SCI_TRACE_LOW("[MMIMP3] SetBgStartFlag is_bg_start=%d", is_bg_start);
}

/*****************************************************************************/
//  Description : bg start or not
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsBgStart(void)
{
    SCI_TRACE_LOW("[MMIMP3] IsBgStart is_bg_start=%d", s_ap_is_bg_start);

    return s_ap_is_bg_start;
}

/*****************************************************************************/
//  Description : get music filter pointer
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_FILTER_T* MMIAP_GetMusicFilterPtr(void)
{
    MMIAP_INFO_T *info_ptr = GetApInfoPtr();
    MMIFMM_FILTER_T *filter_ptr = PNULL;


    if (PNULL != info_ptr)
    {
        filter_ptr = &info_ptr->music_filter;
    }

    return filter_ptr;
}
