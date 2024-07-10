/*****************************************************************************
** File Name:      mmifmmsrv_api.c                                           *
** Author:                                                                   *
** Date:             06/04/2007                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe file manager module         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2007       Robert.Lu          Create                                   *
******************************************************************************/

#define _MMIFMMSRV_API_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_fmm_trc.h"
#include "mmi_osbridge.h"
#include "mmipub.h"

#include "mmifmm.h"
#include "mmifmm_id.h"
#include "mmimms_export.h"
#include "mmimms_text.h"
#include "mmisd_export.h"
#include "mmifmm_text.h"
#include "mmieng_export.h"
#include "mmiudisk_export.h"

#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#include "mmivp_text.h"
#endif
#include "mmifmm_interface.h"

#include "mmicc_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif

#if defined(DRM_SUPPORT)
#include "mmidrm_export.h"
#include "mmi_drmfilemgr.h"
#endif
#include "mmifmm_drm.h"
#include "mmifmm_app.h"
#include "mmifmm_pic.h"
//#include "mmifmm_srv.h"
#include "mmifmmsrv_api.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifdef HERO_ENGINE_SUPPORT 
#define HERO_ENGINE_FILE_SUFFIX_NUM               1
#endif 
#define MMIFMM_PLAYPER_TIME      300

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/

typedef struct  
{
    uint32                      max_size;       //下载铃声最大值
    MMI_WIN_ID_T                win_id;         //发送消息的目的窗口
    wchar                       ticked_file[MMIFILE_FULL_PATH_MAX_LEN+1];  //file need to be ticked on
    uint16                      ticked_file_len; //file name len
    int32                       ring_vol;
#ifdef DRM_SUPPORT
    MMIFMM_DRM_LIMIT_TYPE_T     drm_limit;      //用于设置是否次数受限或者是否转发类型受限等
#endif
}MMIFMM_SELECT_WIN_PARAM_T;

typedef enum
{
    MMIFMM_MUSIC_INFO_NONE,

    MMIFMM_MUSIC_INFO_UNKNOWN,
    MMIFMM_MUSIC_INFO_NOT_SUPPORT,
    MMIFMM_MUSIC_INFO_SUPPORT,
	MMIFMM_MUSIC_INFO_DRM_FAIL,
	MMIFMM_MUSIC_INFO_IN_CALL_STATE

    //MMIFMM_MUSIC_INFO_MAX
} MMIFMM_MUSIC_INFO_STATUS_E;

typedef enum
{
    MMIFMM_SELECT_FAIL,             //select file fail
    MMIFMM_SELECT_SUCCESS,          //select file success
    MMIFMM_SELECT_DRM_NOCOPYRIGHTS //select file is drm have no copyrights
} MMIFMM_SELECT_FILE_RESULT_E;

typedef enum
{
    MMIFMM_SELECT_RING,
    MMIFMM_SELECT_MOVIE,
    MMIFMM_SELECT_PIC,
    MMIFMM_SELECT_FONT,
    MMIFMM_SELECT_ALL,
    MMIFMM_SELECT_DEFAULT = MMIFMM_SELECT_ALL
} MMIFMM_SELECT_FILE_TYPE_E;


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

const wchar g_mmifmm_ellipsis_wstr[] = { '.', '.', '.', 0 };
const wchar g_mmifmm_ellipsis_3_wstr[] = { '.', '.', '.', 0 };

#ifdef CMCC_UI_STYLE
wchar g_mmimultim_picture_dir_wstr[] = { 'P', 'i', 'c', 't', 'u', 'r', 'e', 0 }/*lint !e785*/;
wchar g_mmimultim_photos_dir_wstr[] = { 'P', 'h', 'o', 't', 'o', 's', 0 }/*lint !e785*/;
#else
wchar g_mmimultim_picture_dir_wstr[] = { 'P', 'h', 'o', 't', 'o', 's', 0 }/*lint !e785*/;
#endif

#ifdef BAIDU_DRIVE_SUPPORT
wchar g_mmimultim_gallary_baidudrv_thumbnail_dir_wstr[] = { 'P', 'h','o','t','o','s', '\\','B','a','i','d','u','D', 'r', 'v','\\','T','h','u','m','b','n','a','i','l','s',0};
wchar g_mmimultim_gallary_baidudrv_download_dir_wstr[] = { 'P', 'h','o','t','o','s', '\\','B','a','i','d','u','D', 'r', 'v','\\','D','o','w','n','l','o','a','d',0};
wchar g_mmimultim_gallary_baidudrv_upload_dir_wstr[] = { 'P', 'h','o','t','o','s', '\\','B','a','i','d','u','D', 'r', 'v','\\','U','p','l','o','a','d',0};
#endif

wchar g_mmimultim_music_dir_wstr[]   = { 'A', 'u', 'd', 'i', 'o', 0 }/*lint !e785*/;
wchar g_mmimultim_movie_dir_wstr[]   = { 'V', 'i', 'd', 'e', 'o' , 0 }/*lint !e785*/;
#ifdef MMI_KING_MOVIE_SUPPORT
wchar g_mmimultim_kingmovie_dir_wstr[]   = { 'K','i','n','g','M', 'o', 'v', 'i', 'e' , 0 }/*lint !e785*/;
#endif
wchar g_mmimultim_ebook_dir_wstr[]   = { 'E', 'b', 'o', 'o', 'k' , 0 }/*lint !e785*/;
wchar g_mmimultim_java_dir_wstr[]   = { 'J', 'A', 'V', 'A',  0 }/*lint !e785*/;
#if defined MMI_VCARD_SUPPORT
wchar g_mmimultim_vcard_dir_wstr[]   = { 'v', 'C', 'a', 'r', 'd' , 0 }/*lint !e785*/;
#endif
#ifdef MMI_VCALENDAR_SUPPORT
wchar g_mmimultim_vcalendar_dir_wstr[]   = { 'v', 'C', 'a', 'l', 'e' ,'n' ,'d' ,'a' ,'r' , 0 }/*lint !e785*/;
#endif
#ifdef MCARE_V31_SUPPORT
wchar g_tencentmcare_tmz_dir_wstr[]   = { 'Q', 'Q', 'D', 'o', 'w', 'n', 'L', 'o', 'a', 'd' , 0 }/*lint !e785*/;      
#endif

wchar g_mmimultim_pb_dir_wstr[]      = { 'P', 'B' , 0 };
wchar g_other_dir_default[] = {'O', 't', 'h', 'e', 'r','s', 0}/*lint !e785*/;
wchar g_mmimultim_dir_system[] = {MMIMULTIM_DIR_SYSTEM_CHAR, 0};
wchar g_mmimultim_alarm_dir_wstr[]      = { 'A', 'l' ,'a','r','m', 0 };
wchar g_mmimultim_font_dir_wstr[]   = { 'F', 'o', 'n', 't', 0 };

#ifdef MMI_SSL_CERT_SUPPORT
wchar g_mmimultim_ssl_dir_wstr[]   = { 'S', 'S', 'L', '_', 'C', 'E', 'R', 'T', 0 };/*lint !e785*/
#endif

wchar g_file_array_dir_wstr[]   = { 'F', 'i', 'l', 'e','a','r','r','a','y',0 };

LOCAL BOOLEAN s_is_music_preview = FALSE;
LOCAL MMIFMM_SELECT_WIN_PARAM_T     *s_select_win_param_ptr = PNULL;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : select a file from fmm
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN OpenSelectWin(
                               MMIFMM_SELECT_INFO_T *select_info,
                               MMIFMM_SELECT_FILE_TYPE_E  select_type,
                               PROCESSMSG_FUNC       handle_func
                               );
/*****************************************************************************/
//  Description : handle get icon from icon list preview win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectPicWinMsg(
                                       MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param
                                       );

/*****************************************************************************/
//  Description : handle get select all file
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectAllFileWinMsg(
                                       MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param
                                       );

/*****************************************************************************/
//  Description : handle select ring list win
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectMusicWinMsg(
                                  MMI_WIN_ID_T          win_id,     
                                  MMI_MESSAGE_ID_E      msg_id, 
                                  DPARAM                param
                                  );

/*****************************************************************************/
//  Description : handle movie select win
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectMovWinMsg(
                                      MMI_WIN_ID_T          win_id,     
                                      MMI_MESSAGE_ID_E      msg_id, 
                                      DPARAM                param
                                      );

/*****************************************************************************/
//  Description : handle select ring list win call back
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN OpenSelectMusicWinCbk(
                                FILEARRAY_DATA_T    *file_data_ptr
                                );

/*****************************************************************************/
//  Description : handle select ring list win call back
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL MMIFMM_SELECT_FILE_RESULT_E OpenSelectMovWinCbk(
                                FILEARRAY_DATA_T    *file_data_ptr
                                );

/*****************************************************************************/
//  Description : handle select movie list from mms win call back
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN OpenSelectMovWinMMSCbk(
                                     FILEARRAY_DATA_T    *file_data_ptr
                                     );

LOCAL BOOLEAN OpenSelectPicWinCbk(
                                FILEARRAY_DATA_T    *file_data_ptr
                                );

/*****************************************************************************/
//  Description : handle select file call back
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN OpenSelectAllFileWinCbk(
                                FILEARRAY_DATA_T    *file_data_ptr
                                );
#if defined FONT_TYPE_SUPPORT_VECTOR && defined FILE_FONT_SUPPORT
/*****************************************************************************/
//  Description : handle select font win msg
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectFontWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          );
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : select a pic from fmm
//  Global resource dependence : 
//  Author: robert.lu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_OpenSelectPictureWin(MMIFMM_SELECT_INFO_T *select_info)
{
    return OpenSelectWin(select_info,MMIFMM_SELECT_PIC,HandleSelectPicWinMsg);
}

/*****************************************************************************/
//  Description : select a pic from fmm
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_OpenSelectAllFileWin(MMIFMM_SELECT_INFO_T *select_info)
{
    return OpenSelectWin(select_info,MMIFMM_SELECT_ALL,HandleSelectAllFileWinMsg);
}


/*****************************************************************************/
//  Description : select a music from fmm
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_OpenSelectMusicWin(MMIFMM_SELECT_INFO_T *select_info)
{
    return OpenSelectWin(select_info,MMIFMM_SELECT_RING,HandleSelectMusicWinMsg);
}

/*****************************************************************************/
//  Description : select a movie from fmm
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_OpenSelectMovieWin(MMIFMM_SELECT_INFO_T *select_info)
{
    return OpenSelectWin(select_info,MMIFMM_SELECT_MOVIE,HandleSelectMovWinMsg);
}

#if defined FONT_TYPE_SUPPORT_VECTOR && defined FILE_FONT_SUPPORT
/*****************************************************************************/
//  Description : select a font from fmm
//  Global resource dependence : 
//  Author: james.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_OpenSelectFontWin(MMIFMM_SELECT_INFO_T *select_info)
{
    return OpenSelectWin(select_info,MMIFMM_SELECT_FONT,HandleSelectFontWinMsg);
}
#endif

/*****************************************************************************/
//  Description : handle select ring list win call back
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN OpenSelectMusicWinCbk(
                                FILEARRAY_DATA_T    *file_data_ptr
                                )
{
    BOOLEAN                         result = FALSE;
    MMIFMM_SELECT_RETURN_T          selected_info = {0};

    //SCI_ASSERT(NULL != file_data_ptr);
    if(PNULL == file_data_ptr)
    {
        //SCI_TRACE_LOW:"OpenSelectMusicWinCbk param error:PNULL == file_data_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFMM_2727_112_2_18_2_20_11_235,(uint8*)"");
        return FALSE;
    }

#ifdef DRM_SUPPORT
    if (PNULL != s_select_win_param_ptr
        && ((0 != s_select_win_param_ptr->drm_limit.cons_mode_limit)
        || (0 != s_select_win_param_ptr->drm_limit.method_level_limit)))
    {
        //如果用户设置了限制类型，则检查此文件是否支持用户的要求
        if (MMIAPIFMM_IsSelDrmConstraintFile(file_data_ptr->filename, 
            file_data_ptr->name_len, s_select_win_param_ptr->drm_limit))
        {
            return FALSE;
        }
    }
#endif

    selected_info.file_size = file_data_ptr->size;
    selected_info.file_type = MMIFMM_FILE_TYPE_MUSIC;
    selected_info.music_type = MMIAPICOM_GetMusicType(file_data_ptr->filename, file_data_ptr->name_len);

    MMIAPICOM_Wstrncpy(selected_info.file_name_ptr, file_data_ptr->filename, file_data_ptr->name_len);
    selected_info.file_name_len = file_data_ptr->name_len;

    selected_info.data_buf_ptr = NULL;
    MMK_SendMsg(s_select_win_param_ptr->win_id, MSG_MULTIM_SELECTED_RETURN, (void*)&selected_info);

    result = TRUE;

	return result;

}

/*****************************************************************************/
//  Description : handle select ring list win
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectMusicWinMsg(
                                  MMI_WIN_ID_T          win_id,     
                                  MMI_MESSAGE_ID_E      msg_id, 
                                  DPARAM                param
                                  )
{
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    static uint8        s_splay_timer = 0;
    static MMIFMM_MUSIC_INFO_STATUS_E     s_music_info_status = MMIFMM_MUSIC_INFO_NONE;
    FILEARRAY_DATA_T    file_data = {0};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(0 != s_splay_timer)
        {
            MMK_StopTimer(s_splay_timer);
            s_splay_timer = 0;
        }
        s_music_info_status = MMIFMM_MUSIC_INFO_UNKNOWN;
#ifndef MMI_PDA_SUPPORT
        s_splay_timer =  MMK_CreateWinTimer(win_id, MMIFMM_PLAYPER_TIME, FALSE);
#endif

#ifdef MMI_PDA_SUPPORT
        MMIFMM_SetIsHavePreviewIcon(TRUE);
#endif
        result = MMI_RESULT_TRUE;
        /* Pause Back ground sound */
        MMISRVAUD_ReqVirtualHandle("FMM SELECT RING", MMISRVAUD_PRI_NORMAL);
        break;

    case MSG_FMM_OPENFILEWIN_RELOAD:    
        if(MMIFMM_MUSIC_INFO_NONE != s_music_info_status)
        {
#ifndef MMI_PDA_SUPPORT
            s_splay_timer =  MMK_CreateWinTimer(win_id, MMIFMM_PLAYPER_TIME, FALSE);
#endif
            s_music_info_status = MMIFMM_MUSIC_INFO_UNKNOWN;
        }
        break;
    case MSG_APP_UPSIDE:
    case MSG_APP_DOWNSIDE:
    case MSG_KEYREPEAT_UPSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:
        result = MMI_RESULT_TRUE;
        break;        

    case MSG_APP_DOWN:
    case MSG_APP_UP:
    case MSG_CTL_LIST_MOVEDOWN:
    case MSG_CTL_LIST_MOVEUP: 
#ifndef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_MOVEBOTTOM:
    case MSG_CTL_LIST_MOVETOP:
    case MSG_CTL_LIST_NXTPAGE:
    case MSG_CTL_LIST_PREPAGE:
#endif
#ifdef MMI_PDA_SUPPORT      
   case MSG_NOTIFY_LIST_CHECK_CONTENT:
#endif
        MMIAPISET_StopAppRing();

        if(0 != s_splay_timer)
        {
            MMK_StopTimer(s_splay_timer);
            s_splay_timer = 0;
        }
        s_splay_timer =  MMK_CreateWinTimer(win_id, MMIFMM_PLAYPER_TIME, FALSE);
        s_music_info_status = MMIFMM_MUSIC_INFO_UNKNOWN;
        result = MMI_RESULT_TRUE;
        break;

    case MSG_TIMER:
        if (0 != s_splay_timer && s_splay_timer ==  *( GUI_TIMER_ID_T*)param)
        {
            //start play file
            s_is_music_preview = TRUE;
            if(MMIAPIFMM_GetOpenFileWinCurFile(&file_data))
            {
                if ( -1 == s_select_win_param_ptr->ring_vol)
                {
                    s_select_win_param_ptr->ring_vol = MMIFMM_PLAY_RING_DEFAULT_VOL;
                }
                if (FILEARRAY_TYPE_FILE == file_data.type)
                {
                    if (0 == file_data.size)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
                        s_music_info_status = MMIFMM_MUSIC_INFO_NOT_SUPPORT;
                    }
                    else
                    {
#ifdef DRM_SUPPORT
                        DRMFILE_PRE_CHECK_STATUS_E pre_check_drmfile_status =
                            MMIAPIDRM_PreCheckFileStatus(file_data.filename, DRM_PERMISSION_PLAY);

                        if (DRMFILE_PRE_CHECK_NORMAL != pre_check_drmfile_status)
                        {
                            if (DRMFILE_PRE_CHECK_NO_RIGHTS == pre_check_drmfile_status)
                            {
                                MMIAPIFMM_UpdateSelectListIconData(file_data.filename, file_data.name_len);
                            }

                            s_music_info_status = MMIFMM_MUSIC_INFO_DRM_FAIL;
                            break;
                        }
#endif
                        if (MMIAPIFMM_PlayMusicFile(MMISET_RING_TYPE_OTHER, FALSE,
                            file_data.filename, file_data.name_len,
                            1, 0, s_select_win_param_ptr->ring_vol, NULL))
                        {
                            s_music_info_status = MMIFMM_MUSIC_INFO_SUPPORT;
                        }
                        else
                        {
                            if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
                            {
                                s_music_info_status = MMIFMM_MUSIC_INFO_SUPPORT;
                            }
                            else
                            {
                                MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
                                s_music_info_status = MMIFMM_MUSIC_INFO_NOT_SUPPORT;
                            }
                        }
                    }
                }
            }
        }
        result = MMI_RESULT_TRUE;
        break;
#if defined TOUCH_PANEL_SUPPORT
#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            if(src_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
            {
                break;
            }
            else if(src_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
            {
                break;
            }
            else if(src_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
            {
                break;
            }
            else
            {
                result = MMI_RESULT_TRUE;
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            }
        }
        break;      
#else
    case MSG_CTL_PENOK:
#endif
#endif
    case MSG_APP_OK:    
    case MSG_CTL_OK:
    case MSG_APP_WEB:   
    case MSG_CTL_MIDSK:
        MMIAPISET_StopAppRing();
        if(!MMIAPIFMM_GetOpenFileWinCurFile(&file_data))
		{
			break;
		}
        if (FILEARRAY_TYPE_FILE == file_data.type)
        {
            if(0 == file_data.size)
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
            }
            else if(!MMIAPIMMS_IsEditMMS() && 0 != s_select_win_param_ptr->max_size && s_select_win_param_ptr->max_size < file_data.size)
            {
                MMIPUB_OpenAlertWarningWin(TXT_FILESIZE_TOOBIG);
            }
            else
            {                
#ifdef DRM_SUPPORT
                DRMFILE_PRE_CHECK_STATUS_E pre_check_drmfile_status = DRMFILE_PRE_CHECK_NORMAL;

                if (MMIAPIMMS_IsEditMMS())
                {
                    pre_check_drmfile_status = MMIAPIDRM_PreCheckFileStatusExForMMS(file_data.filename, DRM_PERMISSION_PLAY, FALSE);
                }
                else
                {
                    pre_check_drmfile_status = MMIAPIDRM_PreCheckFileStatus(file_data.filename, DRM_PERMISSION_PLAY);
                }                    
                
                if (DRMFILE_PRE_CHECK_NORMAL != pre_check_drmfile_status)
                {
                    if (DRMFILE_PRE_CHECK_NO_RIGHTS == pre_check_drmfile_status)
                    {
                        MMIAPIFMM_UpdateSelectListIconData(file_data.filename, file_data.name_len);
                    }
                    
                    s_music_info_status = MMIFMM_MUSIC_INFO_DRM_FAIL;
                }
                else
#endif	
                {
                    if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
                    {
                        s_music_info_status = MMIFMM_MUSIC_INFO_IN_CALL_STATE;
                    }
                    else if(!MMIAPIFMM_PlayMusicFileEx(MMISET_RING_TYPE_OTHER,FALSE,FALSE,
                        file_data.filename, file_data.name_len,
                        1, 0, s_select_win_param_ptr->ring_vol, NULL))
                    {
                        s_music_info_status = MMIFMM_MUSIC_INFO_NOT_SUPPORT;
                    }
                    MMIAPISET_StopAppRing();
                }
           
                if (MMIAPIMMS_IsEditMMS())
                {
                    if (!OpenSelectMusicWinCbk(&file_data))
                    {
#ifdef DRM_SUPPORT                        
                        MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
#endif
                    }
                    
                    //MMK_CloseWin(win_id);
                    return result;
                }
                else
                {
                    if (MMIFMM_MUSIC_INFO_NOT_SUPPORT == s_music_info_status)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
                    }
#ifdef DRM_SUPPORT
                    else if (MMIFMM_MUSIC_INFO_DRM_FAIL == s_music_info_status)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
                    }
#endif
                    else if(MMIFMM_MUSIC_INFO_IN_CALL_STATE == s_music_info_status)
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_FMM_CC_IN_CALL_STATE_CANNOT_CHOOSE_MUSIC);
                    }
                    else
                    {
                        if (!OpenSelectMusicWinCbk(&file_data))
                        {
#ifdef DRM_SUPPORT                    	
                            MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
#endif
                            return result;
                        }
                        //MMK_CloseWin(win_id);
                        return result;
                    }                    
                }                  
            }            
            result = MMI_RESULT_TRUE;
        }
        break;

    case MSG_LOSE_FOCUS:
        if(0 != s_splay_timer)
        {
            MMK_StopTimer(s_splay_timer);
            s_splay_timer = 0;
        }
        MMIAPISET_StopAppRing();
        s_is_music_preview = FALSE;
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_CLOSE_WINDOW:
        s_is_music_preview = FALSE;
        if(0 != s_splay_timer)
        {
            MMK_StopTimer(s_splay_timer);
            s_splay_timer = 0;
        }
        s_music_info_status = MMIFMM_MUSIC_INFO_NONE;
        MMIAPISET_StopAppRing();

        if (PNULL != s_select_win_param_ptr)
        {
           SCI_FREE(s_select_win_param_ptr);
           s_select_win_param_ptr = PNULL;
        }

#ifdef MMI_PDA_SUPPORT
        MMIFMM_SetIsHavePreviewIcon(FALSE);
#endif
        /* resume background sound */
        MMISRVAUD_FreeVirtualHandle("FMM SELECT RING");
        result = MMI_RESULT_TRUE;
        break;

    default:
        break;
    }
    return result;
}





#if defined FONT_TYPE_SUPPORT_VECTOR && defined FILE_FONT_SUPPORT
/*****************************************************************************/
//  Description : handle select font win msg
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectFontWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    FILEARRAY_DATA_T    file_data = {0};

    switch(msg_id)
    {
#if defined TOUCH_PANEL_SUPPORT 
#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            if(src_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
            {
                break;
            }
            else if(src_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
            {
                break;
            }
            else if(src_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
            {
                break;
            }
            else
            {
                result = MMI_RESULT_TRUE;
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            }
        }
        break;       
#else
    case MSG_CTL_PENOK:
#endif
#endif
    case MSG_APP_WEB:   
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:    
    case MSG_CTL_OK:
        if(!MMIAPIFMM_GetOpenFileWinCurFile(&file_data))
		{
			break;
		}
        
        if (FILEARRAY_TYPE_FILE == file_data.type)
        {
            MMIFMM_SELECT_RETURN_T selected_info = {0};
            
            selected_info.file_size = file_data.size;
            selected_info.file_type = MMIFMM_FILE_TYPE_FONT;
            
            MMIAPICOM_Wstrncpy(selected_info.file_name_ptr, file_data.filename, file_data.name_len);
            selected_info.file_name_len = file_data.name_len;

            MMK_SendMsg(s_select_win_param_ptr->win_id, MSG_MULTIM_SELECTED_RETURN, (void*)&selected_info);

            //MMK_CloseWin(win_id);
            result = MMI_RESULT_TRUE;
        }
        break;

    case MSG_CLOSE_WINDOW:
        if (PNULL != s_select_win_param_ptr)
        {
           SCI_FREE(s_select_win_param_ptr);
           s_select_win_param_ptr = PNULL;
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

#endif

/*****************************************************************************/
//  Description : handle select ring list win call back
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL MMIFMM_SELECT_FILE_RESULT_E OpenSelectMovWinCbk(
                                FILEARRAY_DATA_T    *file_data_ptr
                                )
{
    MMIFMM_SELECT_FILE_RESULT_E result = MMIFMM_SELECT_FAIL;
    
#ifdef VIDEO_PLAYER_SUPPORT
    //modified by liyan.zhu for c++ test
    //MMIFMM_SELECT_RETURN_T      selected_info = {0};
    VP_SUPPORTED_STATUS_TYPE_E supported_type = VP_SUPPORTED_STATUS_TYPE_MAX;


    //SCI_ASSERT(NULL != file_data_ptr);
#ifdef DRM_SUPPORT
    if (PNULL != s_select_win_param_ptr
        && ((0 != s_select_win_param_ptr->drm_limit.cons_mode_limit)
        || (0 != s_select_win_param_ptr->drm_limit.method_level_limit)))
    {
        //如果用户设置了限制类型，则检查此文件是否支持用户的要求
        if (MMIAPIFMM_IsSelDrmConstraintFile(file_data_ptr->filename, 
            file_data_ptr->name_len, s_select_win_param_ptr->drm_limit))
        {
            return MMIFMM_SELECT_DRM_NOCOPYRIGHTS;
        }
    }
#endif
    
    if(PNULL == file_data_ptr)
    {
        //SCI_TRACE_LOW:"OpenSelectMovWinCbk param error:PNULL == file_data_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFMM_3498_112_2_18_2_20_13_238,(uint8*)"");
        return MMIFMM_SELECT_FAIL;
    }
    

    supported_type = MMIAPIVP_GetSupportedStatus(file_data_ptr->filename,file_data_ptr->name_len);
    if (VP_SUPPORTED_BOTH       == supported_type ||
        VP_SUPPORTED_VIDEO_ONLY == supported_type)

    {
        MMIFMM_SELECT_RETURN_T      selected_info = {0};
        selected_info.file_size = file_data_ptr->size;
        selected_info.file_type = MMIFMM_FILE_TYPE_MOVIE;
        selected_info.movie_type = MMIAPICOM_GetMovieType(file_data_ptr->filename, file_data_ptr->name_len);

        MMIAPICOM_Wstrncpy(selected_info.file_name_ptr, file_data_ptr->filename, file_data_ptr->name_len);
        selected_info.file_name_len = file_data_ptr->name_len;

        selected_info.data_buf_ptr = NULL;
        MMK_SendMsg(s_select_win_param_ptr->win_id, MSG_MULTIM_SELECTED_RETURN, (void*)&selected_info);

        result = MMIFMM_SELECT_SUCCESS;
    }
    else

    {
        //SCI_TRACE_LOW:"OpenSelectMovWinCbk, supported_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFMM_3523_112_2_18_2_20_13_239,(uint8*)"d",supported_type);
    }
    
#endif    
    return result;
}

/*****************************************************************************/
//  Description : handle select movie list from mms win call back
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN OpenSelectMovWinMMSCbk(
                                     FILEARRAY_DATA_T    *file_data_ptr
                                     )
{
    BOOLEAN                     result = TRUE;
    MMIFMM_SELECT_RETURN_T      selected_info = {0};

    
    //SCI_ASSERT(NULL != file_data_ptr);
    if(PNULL == file_data_ptr)
    {
        //SCI_TRACE_LOW:"OpenSelectMovWinMMSCbk param error:PNULL == file_data_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFMM_3545_112_2_18_2_20_13_240,(uint8*)"");
        return FALSE;
    }

#ifdef DRM_SUPPORT
    if (PNULL != s_select_win_param_ptr
        && ((0 != s_select_win_param_ptr->drm_limit.cons_mode_limit)
        || (0 != s_select_win_param_ptr->drm_limit.method_level_limit)))
    {
        //如果用户设置了限制类型，则检查此文件是否支持用户的要求
        if (MMIAPIFMM_IsSelDrmConstraintFile(file_data_ptr->filename, 
            file_data_ptr->name_len, s_select_win_param_ptr->drm_limit))
        {
            return FALSE;
        }
    }
#endif
        
    selected_info.file_size = file_data_ptr->size;
    selected_info.file_type = MMIFMM_FILE_TYPE_MOVIE;
    selected_info.movie_type = MMIAPICOM_GetMovieType(file_data_ptr->filename, file_data_ptr->name_len);        
    MMIAPICOM_Wstrncpy(selected_info.file_name_ptr, file_data_ptr->filename, file_data_ptr->name_len);
    selected_info.file_name_len = file_data_ptr->name_len;        
    selected_info.data_buf_ptr = NULL;
    MMK_SendMsg(s_select_win_param_ptr->win_id, MSG_MULTIM_SELECTED_RETURN, (void*)&selected_info);
    
    return result;
}

/*****************************************************************************/
//  Description : handle movie select win
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectMovWinMsg(
                                        MMI_WIN_ID_T            win_id,
                                        MMI_MESSAGE_ID_E        msg_id,
                                        DPARAM                  param
                                        )
{
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
#ifdef VIDEO_PLAYER_SUPPORT 
    FILEARRAY_DATA_T    file_data = {0};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if ( !MMIAPIMMS_IsBlockConflict(BLOCK_MEM_POOL_MP4ENC) )
        {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT            
			MMIAPIFMM_SetOpenFileWinSoftkey(STXT_SELECT, IMAGE_COMMON_SOFTKEY_START_ICON, STXT_RETURN);
#else
			MMIAPIFMM_SetOpenFileWinSoftkey(STXT_SELECT, STXT_PREVIEW, STXT_RETURN);
#endif
        }
        else
        {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT 
            MMIAPIFMM_SetOpenFileWinSoftkey(STXT_SELECT, IMAGE_NULL, STXT_RETURN);
#else
			MMIAPIFMM_SetOpenFileWinSoftkey(STXT_SELECT, TXT_NULL, STXT_RETURN);
#endif
        }
        #ifdef MMI_PDA_SUPPORT
        if(!MMIAPIMMS_IsEditMMS())//MMS not allow preview
        {
            MMIFMM_SetIsHavePreviewIcon(TRUE);
        }
        #endif
        result = MMI_RESULT_TRUE;

        /* Pause Back ground sound */
        MMISRVAUD_ReqVirtualHandle("FMM SELECT MOVIE", MMISRVAUD_PRI_NORMAL);        
    break;


    case MSG_APP_OK:
    case MSG_CTL_OK:
        if(!MMIAPIFMM_GetOpenFileWinCurFile(&file_data))
		{
			break;
		}
        if (FILEARRAY_TYPE_FILE == file_data.type)
        {
            if(0 == file_data.size)
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
            }
            else if(!MMIAPIMMS_IsEditMMS() && 0 != s_select_win_param_ptr->max_size && s_select_win_param_ptr->max_size < file_data.size)
            {                
                MMIPUB_OpenAlertWarningWin(TXT_FILESIZE_TOOBIG);
            }
            else
            {
                if (MMIAPIMMS_IsEditMMS())
                {
                    if(!OpenSelectMovWinMMSCbk(&file_data))
                    {
#ifdef DRM_SUPPORT                        
                		MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
#endif
                    }
                    //MMK_CloseWin(win_id);
                    return result;
                }
                else
                {
                    MMIFMM_SELECT_FILE_RESULT_E sel_result = OpenSelectMovWinCbk(&file_data);
                    if(MMIFMM_SELECT_SUCCESS == sel_result )
                    {
                        //MMK_CloseWin(win_id);
                        return MMI_RESULT_TRUE;
                    }
#ifdef DRM_SUPPORT 
                    else if (MMIFMM_SELECT_DRM_NOCOPYRIGHTS == sel_result)
                    {                        
                        MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
                    }
#endif
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
                    }
                }                
            }

            result = MMI_RESULT_TRUE;
        }
    break;
#if defined TOUCH_PANEL_SUPPORT
#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            if(src_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
            {
                break;
            }
            else if(src_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
            {
                break;
            }
            else if(src_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
            {
                break;
            }
            else
            {
                result = MMI_RESULT_TRUE;
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            }
        }
        break;       
#else
    case MSG_CTL_PENOK:
#endif
#endif
    case MSG_APP_WEB:   
    case MSG_CTL_MIDSK:
#ifdef MMI_PDA_SUPPORT      
   case MSG_NOTIFY_LIST_CHECK_CONTENT:
#endif
        if(!MMIAPIFMM_GetOpenFileWinCurFile(&file_data))
		{
            break;
		}
        if (FILEARRAY_TYPE_FILE == file_data.type)
        {
            if (!MMIAPIMMS_IsBlockConflict(BLOCK_MEM_POOL_MP4ENC))
            {
                if(0 == file_data.size)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
                }
                else
                {
                    uint16              dev_name[MMIFILE_DEVICE_NAME_MAX_LEN+1] = {0};
                    uint16              dev_len = 0;
                    MMICOM_VIDEO_TYPE_E       movie_type = MMICOM_VIDEO_TYPE_3GP;    
                    movie_type = MMIAPICOM_GetMovieType(file_data.filename, file_data.name_len);
                    MMIAPIFMM_SplitFullPath(file_data.filename, file_data.name_len,
                                          dev_name, &dev_len, NULL, NULL, NULL, NULL);
                   
#ifdef BROWSER_SUPPORT
                    if(MMIAPIBROWSER_IsRunning())
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_BROWSER_VEDIO_NOT_SUPPORT);
                    }
                    else
#endif
                    {
#ifdef DRM_SUPPORT
                        {

                            DRMFILE_PRE_CHECK_STATUS_E pre_check_drmfile_status =
                            MMIAPIDRM_PreCheckFileStatus(file_data.filename, DRM_PERMISSION_PLAY);

                            if (DRMFILE_PRE_CHECK_NORMAL != pre_check_drmfile_status)
                            {
                                if (DRMFILE_PRE_CHECK_NO_RIGHTS == pre_check_drmfile_status)
                                {
                                    MMIAPIFMM_UpdateSelectListIconData(file_data.filename, file_data.name_len);
                                }
                                break;
                            }
                        }
#endif
                        //support check before preview
                        { //CR120028
							VP_SUPPORTED_STATUS_TYPE_E supported_type = VP_SUPPORTED_STATUS_TYPE_MAX;

							supported_type = MMIAPIVP_GetSupportedStatus(file_data.filename,file_data.name_len);
							if (VP_SUPPORTED_BOTH       == supported_type ||
								VP_SUPPORTED_VIDEO_ONLY == supported_type)
                            {
						        //MMIAPIVP_PlayVideoFromVideoPlayerEx(VP_ENTRY_FROM_FILE, file_data.filename, file_data.name_len, PNULL, NULL, PNULL);
                                MMIAPIFMM_PreviewVideo(file_data.filename);
                            }
                            else
                            {
								MMI_WIN_ID_T alert_win_id=MMIFMM_ALERT_VIDEO_PREVIEW_WIN_ID;
								MMIPUB_OpenAlertWinByTextId(PNULL,
									TXT_COMMON_NO_SUPPORT,
									TXT_NULL,
									IMAGE_NULL,
									&alert_win_id,
									PNULL,
									MMIPUB_SOFTKEY_ONE,//MMIPUB_SOFTKEY_ONE
                                    PNULL); 
                            }
                        }
                    }
                }
            }
            else
            {
                if(0 == file_data.size)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
                }
                else if(0 != s_select_win_param_ptr->max_size && s_select_win_param_ptr->max_size < file_data.size)
                {
                    if (MMIAPIMMS_IsEditMMS())
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_MMS_MULTIFILE_TOO_BIG);
                        if(MMK_IsOpenWin(MMIFMM_OPEN_FILE_WIN_ID))
                        {
                            MMK_CloseParentWin(MMIFMM_OPEN_FILE_WIN_ID); 
                        }
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_FILESIZE_TOOBIG);
                    } 
                }
                else
                {
                    if (MMIAPIMMS_IsEditMMS())
                    {
                        if(!OpenSelectMovWinMMSCbk(&file_data))
                        {
#ifdef DRM_SUPPORT                        
                        	MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
#endif
                        }                    

                        //MMK_CloseWin(win_id);
                        return result;
                    }
                    else
                    {
                        MMIFMM_SELECT_FILE_RESULT_E sel_result = OpenSelectMovWinCbk(&file_data);
                        if(MMIFMM_SELECT_SUCCESS == sel_result)
                        {
                           // MMK_CloseWin(win_id);
                            return MMI_RESULT_TRUE;
                        }
#ifdef DRM_SUPPORT 
                        else if (MMIFMM_SELECT_DRM_NOCOPYRIGHTS == sel_result)
                        {                        
                            MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
                        }
#endif
                        else
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
                        }
                    }                    
                }
            }

            result = MMI_RESULT_TRUE;
        }
        break;

    case MSG_CLOSE_WINDOW:
        if (PNULL != s_select_win_param_ptr)
        {
            SCI_FREE(s_select_win_param_ptr);
            s_select_win_param_ptr = PNULL;
        }
        
        #ifdef MMI_PDA_SUPPORT
        if(!MMIAPIMMS_IsEditMMS())//MMS not allow preview
        {
            MMIFMM_SetIsHavePreviewIcon(FALSE);
        }
        #endif
        result = MMI_RESULT_TRUE;

        /* Resume Back ground sound */
        MMISRVAUD_FreeVirtualHandle("FMM SELECT MOVIE");
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
#endif
    return result;
}




/*****************************************************************************/
//  Description : handle select ring list win call back
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN OpenSelectAllFileWinCbk(
                                FILEARRAY_DATA_T    *file_data_ptr
                                )
{
    BOOLEAN                         result = FALSE;
    MMIFMM_SELECT_RETURN_T          selected_info = {0};

    //SCI_ASSERT(NULL != file_data_ptr);
#ifdef DRM_SUPPORT
    if (PNULL != s_select_win_param_ptr
        && ((0 != s_select_win_param_ptr->drm_limit.cons_mode_limit)
        || (0 != s_select_win_param_ptr->drm_limit.method_level_limit)))
    {
        //如果用户设置了限制类型，则检查此文件是否支持用户的要求
        if (MMIAPIFMM_IsSelDrmConstraintFile(file_data_ptr->filename, 
            file_data_ptr->name_len, s_select_win_param_ptr->drm_limit))
        {
            return FALSE;
        }
    }
#endif
    if(PNULL == file_data_ptr)
    {
        //SCI_TRACE_LOW:"OpenSelectAllFileWinCbk param error:PNULL == file_data_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFMM_4131_112_2_18_2_20_14_242,(uint8*)"");
        return TRUE;
    }
    selected_info.file_size = file_data_ptr->size;
    selected_info.file_type = MMIAPIFMM_GetFileTypeByFileName(file_data_ptr->filename, file_data_ptr->name_len);
    //selected_info.pic_type = MMIAPICOM_GetImgaeType(file_data_ptr->filename, file_data_ptr->name_len);

    MMIAPICOM_Wstrncpy(selected_info.file_name_ptr, file_data_ptr->filename, file_data_ptr->name_len);
    selected_info.file_name_len = file_data_ptr->name_len;

    selected_info.data_buf_ptr = NULL;
    MMK_SendMsg(s_select_win_param_ptr->win_id, MSG_MULTIM_SELECTED_RETURN, (void*)&selected_info);

    result = TRUE;
    return result;
}

/*****************************************************************************/
//  Description : handle select ring list win call back
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN OpenSelectPicWinCbk(
                                FILEARRAY_DATA_T    *file_data_ptr
                                )
{
    BOOLEAN                         result = FALSE;
    MMIFMM_SELECT_RETURN_T          selected_info = {0};

    //SCI_ASSERT(NULL != file_data_ptr);
    if(PNULL == file_data_ptr)
    {
        //SCI_TRACE_LOW:"OpenSelectPicWinCbk param error:PNULL == file_data_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFMM_4162_112_2_18_2_20_14_243,(uint8*)"");
        return FALSE;
    }

#ifdef DRM_SUPPORT
    if (PNULL != s_select_win_param_ptr
        && ((0 != s_select_win_param_ptr->drm_limit.cons_mode_limit)
        || (0 != s_select_win_param_ptr->drm_limit.method_level_limit)))
    {
        //如果用户设置了限制类型，则检查此文件是否支持用户的要求
        if (MMIAPIFMM_IsSelDrmConstraintFile(file_data_ptr->filename, 
            file_data_ptr->name_len, s_select_win_param_ptr->drm_limit))
        {
            return FALSE;
        }
    }
#endif

    selected_info.file_size = file_data_ptr->size;
    selected_info.file_type = MMIFMM_FILE_TYPE_PICTURE;
    selected_info.pic_type = MMIAPICOM_GetImgaeType(file_data_ptr->filename, file_data_ptr->name_len);

    MMIAPICOM_Wstrncpy(selected_info.file_name_ptr, file_data_ptr->filename, file_data_ptr->name_len);
    selected_info.file_name_len = file_data_ptr->name_len;

    selected_info.data_buf_ptr = NULL;
#if defined MMI_WALLPAPER_SUPPORT
    if(MMIAPISET_IsWallpaperCall()
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
        ||MMIAPISET_IsPoweronCall()||MMIAPISET_IsPoweroffCall()
#endif
#ifdef SCREENSAVER_SUPPORT
        ||MMIAPISET_IsScreensaverCall()
#endif
    )
    {
        MMIAPIFMM_PreviewPicture(file_data_ptr->filename);
    }
    else
#endif
    {
        MMK_SendMsg(s_select_win_param_ptr->win_id, MSG_MULTIM_SELECTED_RETURN, (void*)&selected_info);
        if(MMK_IsOpenWin(MMIFMM_OPEN_FILE_WIN_ID))
        {
            MMK_CloseParentWin(MMIFMM_OPEN_FILE_WIN_ID); 
        }
    }
    result = TRUE;
    return result;
}

/*****************************************************************************/
//  Description : handle get icon from icon list preview win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectPicWinMsg(
                                       MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param
                                       )
{
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    FILEARRAY_DATA_T    file_data = {0};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifndef MMI_PDA_SUPPORT
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT            
		MMIAPIFMM_SetOpenFileWinSoftkey(STXT_SELECT, IMAGE_COMMON_SOFTKEY_START_ICON, STXT_RETURN);
#else
		MMIAPIFMM_SetOpenFileWinSoftkey(STXT_SELECT, STXT_PREVIEW, STXT_RETURN);
#endif
#endif
#ifdef MMI_PDA_SUPPORT
        if((!MMIAPISET_IsWallpaperCall())
#ifdef MMI_PDA_PREVIEW_BUTTON_SUPPORT
       &&(!MMIAPISET_IsPoweronCall())&&(!MMIAPISET_IsPoweroffCall())
#endif
#ifdef SCREENSAVER_SUPPORT
&&(!MMIAPISET_IsScreensaverCall())
#endif
         )
        {
            MMIFMM_SetIsHavePreviewIcon(TRUE);
        }
        result = MMI_RESULT_TRUE;
#endif
        break;
		
    case MSG_APP_WEB:   
    case MSG_CTL_MIDSK:
#ifdef MMI_PDA_SUPPORT      
   case MSG_NOTIFY_LIST_CHECK_CONTENT:
#endif
        if(!MMIAPIFMM_GetOpenFileWinCurFile(&file_data))
        {
            break;
        }
        if (FILEARRAY_TYPE_FILE == file_data.type)
        {
            result = MMI_RESULT_TRUE;
#ifdef DRM_SUPPORT
            {
                DRMFILE_PRE_CHECK_STATUS_E pre_check_drmfile_status =
                MMIAPIDRM_PreCheckFileStatus(file_data.filename, DRM_PERMISSION_DISPLAY);

                if (DRMFILE_PRE_CHECK_NORMAL != pre_check_drmfile_status)
                {
                    if (DRMFILE_PRE_CHECK_NO_RIGHTS == pre_check_drmfile_status)
                    {
                        MMIAPIFMM_UpdateSelectListIconData(file_data.filename, file_data.name_len);
                    }
                    break;
                }
            }
#endif
            //OpenSelectPicWinCbk(&file_data);
            MMIAPIFMM_PreviewPicture(file_data.filename);
        }
        result = MMI_RESULT_TRUE;
        break;
#if defined TOUCH_PANEL_SUPPORT
#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            if(src_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
            {
                break;
            }
            else if(src_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
            {
                break;
            }
            else if(src_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
            {
                break;
            }
            else
            {
                result = MMI_RESULT_TRUE;
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            }
        }
        break;        
#else
    case MSG_CTL_PENOK:
#endif
#endif
    case MSG_APP_OK:    
    case MSG_CTL_OK:
    //case MSG_APP_WEB:   
    //case MSG_CTL_MIDSK:
        if(!MMIAPIFMM_GetOpenFileWinCurFile(&file_data))
		{
			break;
		}
        if (FILEARRAY_TYPE_FILE == file_data.type)
        {
            if(0 == file_data.size)
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
            }
			else if (!MMIAPIFMM_IsPicSupportFromPath(file_data.filename,file_data.name_len))
			{
				MMIPUB_OpenAlertFailWin(TXT_ERROR);
			}
            else if(!MMIAPIMMS_IsEditMMS() && 0 != s_select_win_param_ptr->max_size && s_select_win_param_ptr->max_size < file_data.size)
            {
                MMIPUB_OpenAlertWarningWin(TXT_FILESIZE_TOOBIG);            
            }
            else
            {
                if (!OpenSelectPicWinCbk(&file_data))
                {
#ifdef DRM_SUPPORT                	
    				MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
#endif    				
                    return result;    
                }
                //MMK_CloseWin(win_id);
                return result;
            }

            result = MMI_RESULT_TRUE;
        }
        break;

    case MSG_CLOSE_WINDOW:
        if (PNULL != s_select_win_param_ptr)
        {
           SCI_FREE(s_select_win_param_ptr);
           s_select_win_param_ptr = PNULL;
        }
#ifdef  MMI_PDA_SUPPORT
        if((!MMIAPISET_IsWallpaperCall())
#ifdef MMI_PDA_PREVIEW_BUTTON_SUPPORT
       &&(!MMIAPISET_IsPoweronCall())&&(!MMIAPISET_IsPoweroffCall())
#endif
#ifdef SCREENSAVER_SUPPORT
&&(!MMIAPISET_IsScreensaverCall())
#endif
        )
        {
            MMIFMM_SetIsHavePreviewIcon(FALSE);
        }
#endif
        result = MMI_RESULT_TRUE;
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : handle get icon from icon list preview win msg
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSelectAllFileWinMsg(
                                       MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param
                                       )
{
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    FILEARRAY_DATA_T    file_data = {0};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT 
		MMIAPIFMM_SetOpenFileWinSoftkey(STXT_SELECT, IMAGE_NULL, STXT_RETURN);
#else
		MMIAPIFMM_SetOpenFileWinSoftkey(STXT_SELECT, TXT_NULL, STXT_RETURN);
#endif
        break;
#if defined TOUCH_PANEL_SUPPORT
#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            if(src_id == MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID)
            {
                break;
            }
            else if(src_id == MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID)
            {
                break;
            }
            else if(src_id == MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID)
            {
                break;
            }
            else
            {
                result = MMI_RESULT_TRUE;
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            }
        }
        break;        
#else
    case MSG_CTL_PENOK:
#endif
#endif
    case MSG_APP_OK:    
    case MSG_CTL_OK:
    case MSG_APP_WEB:   
    case MSG_CTL_MIDSK:		
    //case MSG_APP_WEB:   
    //case MSG_CTL_MIDSK:
        if(!MMIAPIFMM_GetOpenFileWinCurFile(&file_data))
        {
            break;
        }
        if (FILEARRAY_TYPE_FILE == file_data.type)
        {
            if(0 == file_data.size)
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
            }
            else if(!MMIAPIMMS_IsEditMMS() && 0 != s_select_win_param_ptr->max_size && s_select_win_param_ptr->max_size < file_data.size)
            {
                MMIPUB_OpenAlertWarningWin(TXT_FILESIZE_TOOBIG);            
            }
            else
            {
                if (!OpenSelectAllFileWinCbk(&file_data))
                {
#ifdef DRM_SUPPORT                	
    				MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
#endif    				
                    return result;
                }                
                //MMK_CloseWin(win_id);
                return result;
            }

            result = MMI_RESULT_TRUE;
        }
        break;

    case MSG_APP_CAMERA:
        //MMK_CloseWin(win_id);
        result = MMI_RESULT_TRUE;
        break;

    case MSG_CLOSE_WINDOW:
        if (PNULL != s_select_win_param_ptr)
        {
           SCI_FREE(s_select_win_param_ptr);
           s_select_win_param_ptr = PNULL;
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get ticked file name in open file window
//  Global resource dependence : 
//  Author: Jassmine 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_GetOpenFileWinTickedFile(
                                               wchar   *file_name_ptr, //in:
                                               uint16  *name_len_ptr   //in/out:in传入file name max len
                                               )
{
    BOOLEAN     result = FALSE;

    if((PNULL == file_name_ptr) || (PNULL == name_len_ptr))
    {
        //SCI_TRACE_LOW:"MMIFMM_GetOpenFileWinTickedFile param error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFMM_5277_112_2_18_2_20_16_258,(uint8*)"");
        return FALSE;
    }

    if ((PNULL != s_select_win_param_ptr) &&
        (0 < s_select_win_param_ptr->ticked_file_len))
    {
        MMI_WSTRNCPY(file_name_ptr,*name_len_ptr,
            s_select_win_param_ptr->ticked_file,s_select_win_param_ptr->ticked_file_len,
            s_select_win_param_ptr->ticked_file_len);
        *name_len_ptr = s_select_win_param_ptr->ticked_file_len;

        result = TRUE;
    }

    return (result);
}

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Description : open music file
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_OpenMusicFileList(void)
{
    wchar save_path[MMIFMM_FILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16 save_path_len = MMIFMM_FILE_FULL_PATH_MAX_LEN+1;
    MMIFILE_DEVICE_E devie = MMI_DEVICE_SDCARD;
    
	devie = MMIAPIFMM_GetFirstValidDevice();
	if(devie < MMI_DEVICE_NUM)
	{
        MMIAPIFMM_CombineFullPath(
                    MMIAPIFMM_GetDevicePath(devie), MMIAPIFMM_GetDevicePathLen(devie), 
                    MMIMULTIM_DIR_MUSIC, MMIMULTIM_DIR_MUSIC_LEN, PNULL, NULL, 
                    save_path, &save_path_len
                    );
        MMIAPIFMM_OpenExplorerExt(save_path, save_path_len, PNULL, 
            0, TRUE, MMIFMM_MUSIC_ALL);
	}
    
}

/*****************************************************************************/
//  Description : open movie file
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_OpenMovieFileList(void)
{
    wchar save_path[MMIFMM_FILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16 save_path_len = MMIFMM_FILE_FULL_PATH_MAX_LEN+1;
    MMIFILE_DEVICE_E devie = MMI_DEVICE_SDCARD;
    
	devie = MMIAPIFMM_GetFirstValidDevice();
	if(devie < MMI_DEVICE_NUM)
	{
        MMIAPIFMM_CombineFullPath(
                    MMIAPIFMM_GetDevicePath(devie), MMIAPIFMM_GetDevicePathLen(devie), 
                    MMIMULTIM_DIR_MOVIE, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MOVIE), 
					PNULL, NULL, 
                    save_path, &save_path_len
                    );
        MMIAPIFMM_OpenExplorerExt(save_path, save_path_len, PNULL, 
            0, TRUE, MMIFMM_MOVIE_ALL);
	}    
}

#endif//#ifdef CMCC_UI_STYLE


#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : set the select drm file limit type
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 由用户首先设置一个查找文件窗口的后缀形式，
//  然后针对DRM如果还有一些限制，则会在选中后根据用户的相关设置进行提示
//  注意本函数跟在选择窗口的后面才会起作用，如MMIAPIFMM_OpenSelectMovieWin等
//  这函数只针对DRM起作用 值设置成:MMIFMM_DRMFILE_LIMIT_COUNT，
//  MMIFMM_DRMFILE_LIMIT_INTERVAL, MMIFMM_DRMFILE_LIMIT_INTERVAL and so on
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SetSelDrmLimitValue(MMIFMM_DRM_LIMIT_TYPE_T limit_value)
{    
    if (PNULL == s_select_win_param_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDRM] MMIAPIFMM_SetSelDrmLimitValue: invalid param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFMM_6033_112_2_18_2_20_18_265,(uint8*)"");
        return FALSE;
    }

    s_select_win_param_ptr->drm_limit.cons_mode_limit 
        = limit_value.cons_mode_limit;
    s_select_win_param_ptr->drm_limit.method_level_limit 
        = limit_value.method_level_limit;    
	s_select_win_param_ptr->drm_limit.is_rights_valid
        = limit_value.is_rights_valid;

    //SCI_TRACE_LOW:"[MMIDRM] MMIAPIFMM_SetSelDrmLimitValue: cons_mode_limit = %x, method_level_limit = %x, is_rights_valid=%d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFMM_6047_112_2_18_2_20_18_266,(uint8*)"ddd", s_select_win_param_ptr->drm_limit.cons_mode_limit, s_select_win_param_ptr->drm_limit.method_level_limit,s_select_win_param_ptr->drm_limit.is_rights_valid);

    return TRUE;
}
#endif


/*****************************************************************************/
// Description : 给选中的文件打勾
// Global resource dependence : 
// Author: juan.wu
// Note: 
/*****************************************************************************/
PUBLIC void MMIFMM_DrawTickedOnFile(uint16 tick_file_index)
{
    uint16 suffix_wstr[MMIFMM_FILE_SUFFIX_MAX_LEN + 1]        = {0};
    uint16 suffix_len                                = MMIFMM_FILE_SUFFIX_MAX_LEN;
    MMIFMM_FILE_TYPE_E file_type                     = MMIFMM_FILE_TYPE_NORMAL;

    if(PNULL == s_select_win_param_ptr)
    {
        return;
    }
    
	//fixed by liyan.zhu for bugzilla 2760 Change Code
    if (MMIAPIFMM_SplitFileName(s_select_win_param_ptr->ticked_file, s_select_win_param_ptr->ticked_file_len, PNULL, PNULL, suffix_wstr, &suffix_len))
    {
		file_type = MMIAPIFMM_ConvertFileType(suffix_wstr, suffix_len);
		if(MMIFMM_FILE_TYPE_PICTURE == file_type)
		{
			return;
		}
		else if(0 < s_select_win_param_ptr->ticked_file_len)
		{
            FILEARRAY_DATA_T            file_data = {0};
            uint16                      item_index = 0;
            GUILIST_ITEM_DATA_T         item_data = {0};/*lint !e64*/
            wchar           file_name[FILEARRAY_FILENAME_MAX_LEN+1] = {0};
			wchar           size_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
            uint16          file_name_len = 0;
			
            item_index = tick_file_index;
			
            item_data.softkey_id[0] = STXT_SELECT;
            item_data.softkey_id[2] = STXT_RETURN;
			
			if(MMIFMM_GetOpenFileWinFile(item_index, &file_data))
			{
				if (0 == MMIAPICOM_CompareTwoWstr(s_select_win_param_ptr->ticked_file, 
					s_select_win_param_ptr->ticked_file_len, 
					file_data.filename, file_data.name_len))
				{
					item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
					item_data.item_content[0].item_data.image_id = IMAGE_CHECK_SELECTED_ICON;
					
					MMIAPIFMM_SplitFullPath(file_data.filename, file_data.name_len,
						NULL, NULL, NULL, NULL,
						file_name, &file_name_len);
					
					item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
					item_data.item_content[1].item_data.text_buffer.wstr_len = file_name_len;
					
					item_data.item_content[1].item_data.text_buffer.wstr_ptr = file_name;
					if(FILEARRAY_TYPE_FILE == file_data.type )
					{
						item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
						MMIAPIFMM_GetFileSizeString(file_data.size, 
							size_wstr,
							MMIFMM_SIZE_STR_LEN,
							TRUE);
						item_data.item_content[2].item_data.text_buffer.wstr_ptr = size_wstr;
						item_data.item_content[2].item_data.text_buffer.wstr_len = (uint16)MMIAPICOM_Wstrlen(size_wstr);
                        //增加预览 icon
#ifdef MMI_PDA_SUPPORT
                        item_data.item_content[3].item_data_type = GUIITEM_DATA_IMAGE_ID;
                        item_data.item_content[3].item_data.image_id = IMAGE_LIST_TITLE_ARROW_RIGHT_UP;
#endif
                    }
					
					GUILIST_SetItemData(MMIFMM_OPEN_FILE_BYLAYER_LIST_CTRL_ID, &item_data, item_index );
				}
			}
			//    }
        }
    }
}
/*****************************************************************************/
// Description :是否正在预览铃声
// Global resource dependence : 
// Author: juan.wu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsPreviewMusic(void)
{
    return s_is_music_preview;
}

/*****************************************************************************/
//  Description : get default path by type
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
LOCAL wchar* GetDefaultPathSelectType(MMIFMM_SELECT_FILE_TYPE_E select_type)
{
    wchar* path_ptr = PNULL;

    switch(select_type)
    {
    case MMIFMM_SELECT_RING:
        path_ptr = MMIMULTIM_DIR_MUSIC;
    	break;
    case MMIFMM_SELECT_MOVIE:
        path_ptr = MMIMULTIM_DIR_MOVIE;
    	break;
    case MMIFMM_SELECT_PIC:
        path_ptr = MMIMULTIM_DIR_PICTURE;
    	break;
    case MMIFMM_SELECT_FONT:
        path_ptr = MMIMULTIM_DIR_FONT;
    	break;
    default:
        path_ptr = MMIMULTIM_OTHER_DEFAULT_DIR;
        break;
    }
    return path_ptr;
}
/*****************************************************************************/
//  Description : select a file from fmm
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN OpenSelectWin(
                           MMIFMM_SELECT_INFO_T *select_info,
                           MMIFMM_SELECT_FILE_TYPE_E  select_type,
                           PROCESSMSG_FUNC       handle_func
                           )
{
    uint16                  find_path[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16                  find_path_len = 0;
    MMIFMM_FILTER_T         filter = {0};
    MMIFMM_FUNC_TYPE_E      func_type = 0;/*lint !e64*/
    
    int32   i = 0;

    if ( PNULL == select_info)
    {
        return FALSE;
    }
    MMIAPIFMM_CloseSelectFileWin();
    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return FALSE;
    }
    else if (MMIAPIENG_GetIQModeStatus())
    {
        MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
        return FALSE;
    }

    if (!MMIAPIUDISK_IsDeviceReady())
    {
        MMIPUB_OpenAlertWarningWin(TXT_NO_SD_CARD_ALERT);
        return FALSE;
    }
    if (PNULL!=s_select_win_param_ptr)
    {
        SCI_FREE(s_select_win_param_ptr);
    }
    s_select_win_param_ptr = (MMIFMM_SELECT_WIN_PARAM_T *)SCI_ALLOC_APPZ(sizeof(MMIFMM_SELECT_WIN_PARAM_T));
    //保存参数
    s_select_win_param_ptr->win_id = select_info->win_id;
    s_select_win_param_ptr->max_size = select_info->max_size;
    if(NULL != select_info->ticked_file_wstr && 0 < select_info->ticked_file_len)
    {
        MMI_WSTRNCPY(s_select_win_param_ptr->ticked_file,MMIFILE_FULL_PATH_MAX_LEN,
            select_info->ticked_file_wstr, select_info->ticked_file_len, select_info->ticked_file_len);
        s_select_win_param_ptr->ticked_file_len = select_info->ticked_file_len;
    }
    else
    {
        SCI_MEMSET(s_select_win_param_ptr->ticked_file, 0, (MMIFILE_FULL_PATH_MAX_LEN*sizeof(wchar)));
        s_select_win_param_ptr->ticked_file_len = 0;
    }

    //计算打开的目录
    if (NULL != select_info->ticked_file_wstr && 0 < select_info->ticked_file_len)
    {
        uint16  dev[MMIFILE_DEVICE_NAME_MAX_LEN+1] = {0};
        uint16  dir[MMIFILE_DIR_NAME_MAX_LEN+1] = {0};

        uint16 dev_len = MMIFILE_DEVICE_NAME_MAX_LEN;
        uint16 dir_len = MMIFILE_DIR_NAME_MAX_LEN;
        find_path_len = MMIFILE_FULL_PATH_MAX_LEN;
        MMIAPIFMM_SplitFullPath(select_info->ticked_file_wstr, select_info->ticked_file_len,dev, &dev_len, dir, &dir_len, NULL, NULL);
        MMIAPIFMM_CombineFullPath(dev, dev_len, dir, dir_len, NULL, 0, find_path, &find_path_len);
    }
    else 
	{
        MMIFILE_DEVICE_E      device = MMI_DEVICE_SDCARD;
        device = MMIAPIFMM_GetFirstValidDevice();
        if(MMI_DEVICE_NUM > device)
        {
            wchar  *full_path_ptr = GetDefaultPathSelectType(select_type);
			find_path_len = MMIFILE_FULL_PATH_MAX_LEN;
			if (!MMIAPIFMM_CombineFullPath(MMIAPIFMM_GetDevicePath(device), MMIAPIFMM_GetDevicePathLen(device),
				full_path_ptr, MMIAPICOM_Wstrlen(full_path_ptr),
				PNULL, 0,
				find_path, &find_path_len))
			{
				if (PNULL != s_select_win_param_ptr)
				{
					SCI_FREE(s_select_win_param_ptr);
				}
				return FALSE;
			}
        }
        else
        {
			//no valid device
			if (PNULL != s_select_win_param_ptr)
			{
				SCI_FREE(s_select_win_param_ptr);
			}
			return FALSE;
        }
	}

    //把搜索类型转化成字符串形式
    for (i = 0; i<ARR_SIZE(select_info->select_file);i++)/*lint !e574 !e737*/
    {
        MMIAPIFMM_GetFilerInfo(&filter,select_info->select_file[i]);
    }
    if (MMIFMM_SELECT_RING == select_type)
    {
        s_select_win_param_ptr->ring_vol = select_info->ring_vol;
    }
    func_type = FUNC_FIND_FILE_AND_ALLFOLER;
    {
        MMIFMM_OPEN_LAYER_WIN_INFO_T open_layer_info = {0};
        
        open_layer_info.path_ptr = find_path;
        open_layer_info.path_len = find_path_len;
        open_layer_info.filter_ptr = &filter;
        open_layer_info.func_type = func_type;
        open_layer_info.is_multi_sel = FALSE;
        open_layer_info.handle_func = handle_func;
        open_layer_info.ret_array = NULL;
        open_layer_info.callback = NULL;
        open_layer_info.is_internal_call =FALSE;
        open_layer_info.is_sel_folder = FALSE;
        
        if (MMIFMM_SELECT_PIC == select_type)
        {
#ifdef MMI_FMM_PICTHUMBNAIL_SUPPORT //添加PICTHUMBNAIL_SUPPORT控制	
            open_layer_info.disp_type = WIN_DISP_2LINE;
#else
			open_layer_info.disp_type = WIN_DISP_1LINE;
#endif
        }
        else 
        {
            open_layer_info.disp_type = WIN_DISP_1LINE;
        }
        
        MMIAPIFMM_OpenFileWinByLayerEx(&open_layer_info);
    }
//    MMIFMM_OpenFileWinByLayerInternal(find_path, find_path_len,
//        &filter, func_type, FALSE,       handle_func, NULL, NULL, FALSE, FALSE);
    
    return TRUE;

}

/*****************************************************************************/
//  Description : close select file window
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_CloseSelectFileWin(void)
{
    //SCI_TRACE_LOW:"MMIAPIFMM_CloseSelectFileWin, exit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFMM_3458_112_2_18_2_20_12_237,(uint8*)"");
    MMK_CloseWin(MMIFMM_OPEN_FILE_BYLAYER_WAIT_WIN_ID);
    if(MMK_IsOpenWin(MMIFMM_OPEN_FILE_WIN_ID))
    {
        MMK_CloseParentWin(MMIFMM_OPEN_FILE_WIN_ID);
    }
    MMK_CloseWin(MMIFMM_SEL_PICTURE_PREVIEW_WIN_ID);
}


