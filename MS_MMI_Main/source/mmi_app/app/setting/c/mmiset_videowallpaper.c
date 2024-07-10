/****************************************************************************
** File Name:      mmiset_videowallpaper.c                                 *
** Author:         aoke.hu                                                 *
** Date:           08/02/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2011        aoke.hu          create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#include "mmiset_videowallpaper.h"
#include "window_parse.h"
#include "guilistbox.h"
#include "mmifilearray_export.h"
#include "mmifmm_export.h"
#include "mmifmm_comfunc.h"
#include "mmipub.h"
#include "mmiset_id.h"
#include "mmiset_menutable.h"
#include "mmiset_text.h"
#include "mmiset_nv.h"
#include "mmiset_display.h"
#include "mmivp_text.h"
#include "mmivp_internal.h"
#include "mmiudisk_export.h"
#include "mmicom_panel.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmi_wallpaper_export.h"
 #ifdef  VIDEOTHUMBNAIL_SUPPORT  
 #include "mmifmm_interface.h"
 #include "img_dec_interface.h"
 #endif
#include "mmi_nv.h"
/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION    							  *
 **---------------------------------------------------------------------------*/
#define MMISET_VIDEO_SYSFILE_DIR            s_video_sysfile_dir_wstr
#define MMISET_VIDEO_SYSFILE_DIR_LEN        MMIAPICOM_Wstrlen(s_video_sysfile_dir_wstr) //15
#define MMISET_VIDEO_SYSFILE_NAME           s_video_sysfile_name_wstr
#define MMISET_VIDEO_SYSFILE_NAME_LEN       MMIAPICOM_Wstrlen(s_video_sysfile_name_wstr) //14
#define FILE_SUFFIX_MAXLEN     10

#ifndef PDA_VIDEOPLAYER_WALLPAPER_SUPPORT_LOW_MEMORY
#define MMISET_VIDEO_MAX_VIDEO_SIZE         (800*600)
#else
#define MMISET_VIDEO_MAX_VIDEO_MIDDLE_SIZE         (320*240)
#define MMISET_VIDEO_MAX_VIDEO_SMALL_SIZE         (220*176)

#define MMISET_H264_MAX_SIZE_FOR_1300KB_AVAILNUM (176*144)
#define MMISET_MPEG4_MAX_SIZE_FOR_1300KB_AVAILNUM  (220*176)
#define MMISET_MJPGE_MAX_SIZE_FOR_1300KB_AVAILNUM (480*320)

#define MMISET_H264_MAX_SIZE_FOR_2100KB_AVAILNUM (320*240)
#define MMISET_MPEG4_MAX_SIZE_FOR_2100KB_AVAILNUM  (352*288)
#define MMISET_MJPGE_MAX_SIZE_FOR_2100KB_AVAILNUM (640*480)

#define MMISET_H264_MAX_SIZE_FOR_2400KB_AVAILNUM (320*240)
#define MMISET_MPEG4_MAX_SIZE_FOR_2400KB_AVAILNUM  (352*288)
#define MMISET_MJPGE_MAX_SIZE_FOR_2400KB_AVAILNUM (640*480)

#define MMISET_H264_MAX_SIZE_FOR_2900KB_AVAILNUM (320*240)
#define MMISET_MPEG4_MAX_SIZE_FOR_2900KB_AVAILNUM  (352*288)
#define MMISET_MJPGE_MAX_SIZE_FOR_2900KB_AVAILNUM (640*480)
#endif

 /**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT    							  *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
static BOOLEAN s_osd = FALSE;   //两种视频墙纸切换：
//true,MMI替换主层使用888 osd buffer,保持并MP4由REF刷新的机制不变
//false，ref把视频数据给MMI，统一由MMI来触发刷屏。
LOCAL MMISET_VIDEO_WALLPAPER_LIST_T s_video_wallpaper_info = {0};
LOCAL MMISET_VIDEO_WALLPAPER_NV_T s_video_setting_info = {0};
LOCAL uint16 s_video_wallpaper_cur_index =0xFF; 
LOCAL BOOLEAN s_is_video_layer_enable = FALSE ;
// 视频墙纸文件路径
LOCAL const wchar s_video_sysfile_dir_wstr[] = L"video_wallpaper";
// 视频墙纸文件名
LOCAL const wchar s_video_sysfile_name_wstr[] = L"video_list.dat";

 #ifdef  VIDEOTHUMBNAIL_SUPPORT  
    LOCAL wchar s_wp_video_getthumbnail_name[MMIFMM_FULL_FILENAME_LEN+1]={0};
 #endif
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : PlayNextVideoWallpaper
//	Global resource dependence : 
//  Author:aoke.hu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN PlayNextVideoWallpaper(void);

/*****************************************************************************/
// 	Description : SwitchToNextWallpaper
//	Global resource dependence : 
//  Author:aoke.hu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN SwitchToNextWallpaper(void);

/*****************************************************************************/
// Description : 停止播放VIDEO WALLPAPER 
// Global resource dependence : 
// Author:aoke.hu
// Note: 
/*****************************************************************************/
LOCAL void StopVideoWallpaper(void);

/*****************************************************************************/
//  Description : 获取下一个视频
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL MMISET_VIDEO_LIST_T* GetNextVideo(DPLAYER_MEDIA_INFO_T* media_info_ptr);

/*****************************************************************************/
//  Description : VIDEO WALLPAPER列表重置
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void VideolistReset(void);

/*****************************************************************************/
//  Description : append video wallpaper set list
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void AppendVideoWallpaperSetList(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : to handle the VIDEO WALLPAPER window
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVideoWallpaperWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM            param
                                               );

/*****************************************************************************/
//  Description : append video list
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void AppendVideoList(MMI_CTRL_ID_T ctrl_id,
                           uint32 total_num);

/*****************************************************************************/
//  Description : update video list
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void UpdateVideoList(void);

/*****************************************************************************/
//  Description : to handle the VIDEO list window
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVideoListWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM            param
                                          );

/*****************************************************************************/
//  Description : to handle the video list window
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVideolListOptMenuWinMsg(
                                                  MMI_WIN_ID_T        win_id, 
                                                  MMI_MESSAGE_ID_E    msg_id, 
                                                  DPARAM            param
                                                  );

/*****************************************************************************/
//  Description : to handle the video list long ok window
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVideolListLongOKOptMenuWinMsg(
                                                        MMI_WIN_ID_T        win_id, 
                                                        MMI_MESSAGE_ID_E    msg_id, 
                                                        DPARAM            param
                                                        );

/*****************************************************************************/
//  Description : 从文件管理器中添加视频文件到列表中
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void VideoList_AddItem(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void AddFileToVideoListCallBack(
                                      BOOLEAN     is_success,
                                      FILEARRAY   file_array
                                      );

/*****************************************************************************/
//  Description : VideoListWriteFile
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void VideoListWriteFile(void);

/*****************************************************************************/
//  Description : VideoListWriteFile
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN VideoListReadFile(void);

/*****************************************************************************/
//  Description : FreeVideoList
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void FreeVideoList(void);

/*****************************************************************************/
//  Description : CheckVideoListVer
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckVideoListVer(uint8 *version_ptr);

/*****************************************************************************/
//  Description : AddItemToVideoList
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void AddItemToVideoList(MMISET_VIDEO_LIST_T * item_ptr);

/*****************************************************************************/
//  Description : DelItemFromVideoList
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void DelItemFromVideoList(uint32 index);

/*****************************************************************************/
//  Description : Get Video List Total Num
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetVideoListTotalNum(void);

/*****************************************************************************/
//  Description : Get Video item by index
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL MMISET_VIDEO_LIST_T* GetVideoItemByIndex(uint32 index);

/*****************************************************************************/
//  Description : reset setting info
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void ResetSettingInfo(void);

/*****************************************************************************/
//  Description : video setting init
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void VideoSettingInit(void);

/*****************************************************************************/
//  Description : get video setting
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetVideoRandomSetting(void);

/*****************************************************************************/
//  Description : Set video setting
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void SetVideoRandomSetting(BOOLEAN is_random);

/*****************************************************************************/
//  Description : Get video audiosupport setting
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetVideoAudioSetting(void);

/*****************************************************************************/
//  Description : Set video audiosupport setting
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void SetVideoAudioSetting(BOOLEAN is_audio_support);
 
/*****************************************************************************/
// 	Description : HandleVideoScreenSaverMsg
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void HandleDynWallpaperPlayMsg(VP_CALLBACK_T *msg_ptr);

/*****************************************************************************/
//  Description : 音量调节
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void AdjustVolCallBack(MMICOM_PANEL_CALLBACK_T *para_ptr);
/*****************************************************************************/
//  Description : 有些时候视频DPLAYEROPEN 成功，但之后解码器在解码中会失败 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckAllVideoIsInValid(void);
/*****************************************************************************/
//  Description : 打开调节窗口
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void OpenVolumePanel(MMI_WIN_ID_T win_id);
#ifdef PDA_VIDEOPLAYER_WALLPAPER_SUPPORT_LOW_MEMORY
/*****************************************************************************/
//  Description : CheckVideoIsLowMemorySupport
//  Global resource dependence : 
//  Author: andy.he
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  CheckVideoIsLowMemorySupport(DPLAYER_MEDIA_INFO_T *media_info);
#endif

/*****************************************************************************/
// 	Description : PlayNextVideoWallpaper
//	Global resource dependence : 
//  Author:aoke.hu
//	Note: 
/*****************************************************************************/
LOCAL VP_DISPLAY_PARAM_T GetDisplayParam(DPLAYER_MEDIA_INFO_T *media_info_ptr);


 #ifdef  VIDEOTHUMBNAIL_SUPPORT  
 
/*****************************************************************************/
//  Description : get image width and height by image path
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WP_GetImgInfoByPath(
                                 wchar* full_path_wstr_ptr,     //in:
                                 GUIIMG_INFO_T  *img_info_ptr   //in/out:
                                 );
/*****************************************************************************/
//  Description :WP_DisplayThumbnail  显示视频的对应缩略图
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL  BOOLEAN  WP_DisplayThumbnail(wchar* video_fullpathname);
/*****************************************************************************/
//  Description : 获取bmp_fullpathname 的图片img数据
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL  BOOLEAN  WP_DecoderThumbnailBmpImage(wchar* bmp_fullpathname ,GUIIMG_DEC_OUT_T *img_output);
/*****************************************************************************/
//  Description :WP_GetThumbnailPathnameForWriteEx
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL  BOOLEAN  WP_GetThumbnailPathnameForWriteEx(wchar* thumbnai_path,wchar* videopath);
/*****************************************************************************/
//  Description :WP_GetThumbnailPathnameForReadEx
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL  BOOLEAN  WP_GetThumbnailPathnameForReadEx(wchar* thumbnai_path,wchar* videopath);
/*****************************************************************************/
//  Description : WP_VideoThumbnailCallback
//  Global resource dependence : 
//  Author:  
//  Note:  
/*****************************************************************************/

LOCAL  BOOLEAN  WP_VideoThumbnailCallback(DPLAYER_IMAGE_DATA_T *image_data);
/*****************************************************************************/
//  Description : WP_GetVideothumnail
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN WP_GetVideothumnail(wchar *video_fullpath_name,uint16 video_paht_len);
#endif

/*****************************************************************************/
//  Description : CanbeAddToVideoList
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CanbeAddToVideoList(uint16             total_mark_num,
                                  FILEARRAY_DATA_T   * file_data_ptr);
/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/
WINDOW_TABLE( MMISET_SET_VIDEO_WALLPAPER_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleVideoWallpaperWinMsg),
        WIN_ID(MMISET_VIDEO_WALLPAPER_WIN_ID),
        //WIN_STYLE( WS_HAS_TRANSPARENT),
        WIN_TITLE( TXT_SET_WALLPAPER_VIDEO),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_VIDEO_WALLPAPER_CTRL_ID),
        END_WIN
};

WINDOW_TABLE( MMISET_SET_VIDEO_LIST_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleVideoListWinMsg),
        WIN_ID(MMISET_VIDEO_LIST_WIN_ID),
        //WIN_STYLE( WS_HAS_TRANSPARENT),
        WIN_TITLE( TXT_SET_WALLPAPER_VIDEO_LIST),
        WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMISET_VIDEO_LIST_CTRL_ID),
        END_WIN
};

WINDOW_TABLE( MMISET_VIDEO_LIST_POPMENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleVideolListOptMenuWinMsg),
        WIN_ID(MMISET_VIDEO_LIST_OPT_MENU_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        CREATE_POPMENU_CTRL(MENU_SET_VIDEO_LIST_OPT,MMISET_VIDEO_LIST_OPT_MENU_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
        END_WIN
};

WINDOW_TABLE( MMISET_VIDEO_LIST_LONGOK_POPMENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleVideolListLongOKOptMenuWinMsg),
        WIN_ID(MMISET_VIDEO_LIST_LONGOK_OPT_MENU_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        CREATE_POPMENU_CTRL(MENU_SET_VIDEO_LIST_LONGOK_OPT,MMISET_VIDEO_LIST_LONGOK_OPT_MENU_CTRL_ID),
#ifndef MMI_PDA_SUPPORT
		WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
#endif
        END_WIN
};

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : MMISETVIDEO_OpenWallpaperWin
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISETVWP_OpenMainWin(void)
{
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
    MMISET_POWER_SETTING_ITEM_T power_saving_setting = {0};
#endif    

	if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return;
    }

#ifdef MMI_POWER_SAVING_MODE_SUPPORT
    //NEWMS00186826
    power_saving_setting =  MMIAPISET_GetPowerSavingSetting();   
    if(power_saving_setting.is_fb_anim_wallpaper && power_saving_setting.is_active) 
    {
        if(MMIAPISETVWP_IsOpen())
        {
            MMIAPISET_ResetDefaultWallpaper();
        }
        MMIPUB_OpenAlertWarningWin(TXT_VP_VIDEO_WALLPAPER_POWERSAVING_ON);
        return ;
    }
#endif
    MMK_CreateWin((uint32*)MMISET_SET_VIDEO_WALLPAPER_WIN_TAB,PNULL);
}


/*****************************************************************************/
//  Description : VIDEO WALLPAPER重置
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISETVWP_Reset(void)
{
    VideolistReset();
    ResetSettingInfo();
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_IsExistNextVWP(void)
{
	SCI_TRACE_LOW("MMIAPISETVWP_IsExistNextVWP s_video_wallpaper_cur_index = %d,total_num=%d",s_video_wallpaper_cur_index,s_video_wallpaper_info.total_num);
	if (s_video_wallpaper_cur_index + 1 < s_video_wallpaper_info.total_num)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_IsExistVWP(void)
{
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
    MMISET_POWER_SETTING_ITEM_T power_saving_setting = {0};
#endif    

	if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {        
        return FALSE;
    }

#ifdef MMI_POWER_SAVING_MODE_SUPPORT
    //NEWMS00186826
    power_saving_setting =  MMIAPISET_GetPowerSavingSetting();   
	SCI_TRACE_LOW("MMIAPISETVWP_IsExistVWP is_fb_anim_wallpaper = %d, is_active = %d",power_saving_setting.is_fb_anim_wallpaper,power_saving_setting.is_active);
    if(power_saving_setting.is_fb_anim_wallpaper && power_saving_setting.is_active) 
    {       
        return FALSE;
    }
#endif

	if (s_video_wallpaper_info.total_num > 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
/*****************************************************************************/
//  Description : 获取下一个视频
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL MMISET_VIDEO_LIST_T* GetNextVideo(DPLAYER_MEDIA_INFO_T* media_info_ptr)
{
    BOOLEAN result = FALSE;
    MMISET_VIDEO_LIST_T* item_ptr = PNULL;
    uint16 index =0;
    DPLAYER_MEDIA_INFO_T media_info = {0};
    
    s_video_wallpaper_cur_index++;
    if(s_video_wallpaper_info.total_num > MMISET_VIDEO_WALLPAPER_MAX_NUM)
    {
        //SCI_TRACE_LOW:"[MMISETVWP]:GetNextVideo total_num ERROR!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_383_112_2_18_2_52_15_347,(uint8*)"");
        s_video_wallpaper_info.total_num = MMISET_VIDEO_WALLPAPER_MAX_NUM;
    }
    if(s_video_wallpaper_cur_index >= s_video_wallpaper_info.total_num)
    {
        s_video_wallpaper_cur_index = 0;
    }
    item_ptr = GetVideoItemByIndex(s_video_wallpaper_cur_index);
    if(PNULL != item_ptr && !s_video_wallpaper_info.is_invalid[s_video_wallpaper_cur_index])
    {
        if(MMIAPIVP_GetInfoFromFullPathName(item_ptr->full_path_name,item_ptr->full_path_len,&media_info))
        {
#ifndef PDA_VIDEOPLAYER_WALLPAPER_SUPPORT_LOW_MEMORY
            if(media_info.video_info.height * media_info.video_info.width <= MMISET_VIDEO_MAX_VIDEO_SIZE
                && 1 == media_info.video_info.video_support)
#else
            if(CheckVideoIsLowMemorySupport(&media_info))
#endif
            {
                result = TRUE;
            }
            else
            {
                //SCI_TRACE_LOW:"[MMISETVWP]:GetNextVideo h=%d,w=%d,v_support=%d,video_type=%d,index=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_407_112_2_18_2_52_15_348,(uint8*)"ddddd",media_info.video_info.height,media_info.video_info.width,media_info.video_info.video_support,media_info.video_info.video_type, s_video_wallpaper_cur_index);
            }
        }
    }
    //跳过有问题的视频
    if(!result)
    {
        s_video_wallpaper_info.is_invalid[s_video_wallpaper_cur_index] = TRUE;
        index = (uint16)(s_video_wallpaper_cur_index + 1);
        if(index >= s_video_wallpaper_info.total_num)
        {
            index = 0;
        }
        while(index != s_video_wallpaper_cur_index)
        {
            item_ptr = GetVideoItemByIndex(index);
            if(PNULL != item_ptr && !s_video_wallpaper_info.is_invalid[index])
            {
                if(MMIAPIVP_GetInfoFromFullPathName(item_ptr->full_path_name,item_ptr->full_path_len,&media_info))
                {
#ifndef PDA_VIDEOPLAYER_WALLPAPER_SUPPORT_LOW_MEMORY
                    if(media_info.video_info.height * media_info.video_info.width <= MMISET_VIDEO_MAX_VIDEO_SIZE
                        && 1 == media_info.video_info.video_support)
#else
                    if(CheckVideoIsLowMemorySupport(&media_info))
#endif
                    {
                        result = TRUE;
                        s_video_wallpaper_cur_index = index;
                        break;
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"[MMISETVWP]:GetNextVideo h=%d,w=%d,v_support=%d,video_type=%d,index=%d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_441_112_2_18_2_52_15_349,(uint8*)"ddddd",media_info.video_info.height,media_info.video_info.width,media_info.video_info.video_support,media_info.video_info.video_type,index);
                    }
                }
            }
            s_video_wallpaper_info.is_invalid[index] = TRUE;
            index++;
            if(index >= s_video_wallpaper_info.total_num)
            {
                index = 0;
            }
        }
    }
    if(result)
    {
        if(PNULL != media_info_ptr)
        {
            SCI_MEMCPY(media_info_ptr,&media_info,sizeof(DPLAYER_MEDIA_INFO_T));
        }
        return item_ptr;
    }
    else
    {
        //SCI_TRACE_LOW:"[MMISETVWP]:GetNextVideo no support video!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_463_112_2_18_2_52_15_350,(uint8*)"");
        return PNULL;
    }
}

/*****************************************************************************/
//  Description : VIDEO WALLPAPER列表重置
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void VideolistReset(void)
{
    wchar               path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16              path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMIFMM_FILTER_T     filter      = {0};
    FILEARRAY           file_array = NULL;
    uint32              file_num = 0;
    uint32              i = 0;
    FILEARRAY_DATA_T    file_array_data = {0};
    MMISET_VIDEO_LIST_T video_item = {0};
    MMIFILE_DEVICE_E device_type = MMI_DEVICE_SYSTEM;
    
    FreeVideoList();
    MMIAPIFMM_GetSysFileFullPathForReadEx(
        TRUE,
        TRUE,
        MMISET_VIDEO_SYSFILE_DIR,
        MMISET_VIDEO_SYSFILE_DIR_LEN,
        PNULL,
        PNULL,
        path_name,
        &path_len
        );
    if(0 == path_len)
    {
        VideoListWriteFile();
        return;
    }
    
    //默认文件夹中搜索支持的视频文件
    MMIAPIVP_GetFilerInfo(&filter);
    device_type = MMIAPIFMM_GetDeviceTypeByPath(path_name,MMIFILE_DEVICE_LEN);
    while(device_type<MMI_DEVICE_NUM
        &&MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(device_type), MMIAPIFMM_GetDevicePathLen(device_type)))
    {
        file_array = MMIAPIFILEARRAY_Create();
        MMIAPIFMM_SearchFileInPath(path_name, 
            path_len, 
            &filter, 
            TRUE, 
            FUNC_FIND_FILE, 
            file_array, 
            0, 
            0);
        file_num = MMIAPIFILEARRAY_GetArraySize(file_array);
        file_num = MIN(file_num,(MMISET_VIDEO_WALLPAPER_MAX_NUM - s_video_wallpaper_info.total_num));
        for(i=0; i<file_num; i++)
        {
            //读取下一条要写的数据
            if(MMIAPIFILEARRAY_Read(file_array, i, &file_array_data))
            {
                SCI_MEMSET(&video_item,0,sizeof(MMISET_VIDEO_LIST_T));
                MMIAPICOM_Wstrncpy(video_item.full_path_name,file_array_data.filename,file_array_data.name_len);
                video_item.full_path_len = file_array_data.name_len;
                video_item.file_size = file_array_data.size;
                AddItemToVideoList(&video_item);
            }
            else
            {
                break;
            }
        }
        MMIAPIFILEARRAY_Destroy(&file_array);
        device_type++;
        MMIAPICOM_Wstrncpy(path_name,MMIAPIFMM_GetDevicePath(device_type),MMIFILE_DEVICE_LEN);
    }
    VideoListWriteFile();
}

/*****************************************************************************/
//  Description : VIDEO WALLPAPER初始化
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISETVWP_Init(void)
{
    VideoSettingInit();
    if(!VideoListReadFile())
    {
        VideolistReset();
    }
}

/*****************************************************************************/
//  Description : append video wallpaper set list
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void AppendVideoWallpaperSetList(MMI_CTRL_ID_T ctrl_id)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    
    GUILIST_SetMaxItem(ctrl_id, MMISET_VIDEO_WALLPAPER_MAX, FALSE);
    
    //MMISET_VIDEO_WALLPAPER_SWITCH
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
    item_t.item_data_ptr = &item_data;
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = TXT_SET_WALLPAPER_VIDEO_SWITCH;
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
    if(MMIAPISETVWP_IsOpen())
    {
        item_data.item_content[1].item_data.image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {
        item_data.item_content[1].item_data.image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }
    //
    GUILIST_AppendItem( ctrl_id, &item_t);
    
    //MMISET_VIDEO_WALLPAPER_LIST
    SCI_MEMSET(&item_t, 0x00, sizeof(item_t));
    SCI_MEMSET(&item_data, 0x00, sizeof(item_data));
#ifdef GUIF_LIST_PDASTYLE
	item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;
#else	
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_DEFAULT_FONT; //NEWMS00185675
#endif
    item_t.item_data_ptr = &item_data;
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = TXT_VP_CUSTOMIZED_LIST;
    GUILIST_AppendItem( ctrl_id, &item_t);
    
    //MMISET_VIDEO_WALLPAPER_RANDOM
    /*
    SCI_MEMSET(&item_t, 0x00, sizeof(item_t));
    SCI_MEMSET(&item_data, 0x00, sizeof(item_data));
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
    item_t.item_data_ptr = &item_data;
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = TXT_SET_SHUFFLE;
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
    if(GetVideoRandomSetting())
    {
    item_data.item_content[1].item_data.image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    else
    {
    item_data.item_content[1].item_data.image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }
    GUILIST_AppendItem( ctrl_id, &item_t);
    */
    //MMISET_VIDEO_WALLPAPER_AUDIOSUPPORT
    SCI_MEMSET(&item_t, 0x00, sizeof(item_t));
    SCI_MEMSET(&item_data, 0x00, sizeof(item_data));
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
    item_t.item_data_ptr = &item_data;
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = TXT_TURNON_MUTE;
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
    if(GetVideoAudioSetting())
    {
        item_data.item_content[1].item_data.image_id = IMAGE_CHECK_UNSELECTED_ICON;
    }
    else
    {
        item_data.item_content[1].item_data.image_id = IMAGE_CHECK_SELECTED_ICON;
    }
    GUILIST_AppendItem( ctrl_id, &item_t);
    
    GUILIST_SetCurItemIndex( ctrl_id, 0);
}

#ifdef PDA_UI_SUPPORT_U_IDLE
/*****************************************************************************/
// 	Description : handle video wallpaper set query window
//	Global resource dependence : none
//  Author: hongbo.lan
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVWSetQueryWinMsg(
                                              MMI_WIN_ID_T win_id,
                                              MMI_MESSAGE_ID_E msg_id,
                                              DPARAM param
                                              )
{
    MMI_RESULT_E   result       = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        MMK_PostMsg(MMK_GetPrevWinId(win_id), MSG_GRID_UIDLE_UPDATE_VW, PNULL, NULL);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }
    
    return result;
}
#endif

/*****************************************************************************/
//  Description : to handle the VIDEO WALLPAPER window
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVideoWallpaperWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM            param
                                               )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMISET_VIDEO_WALLPAPER_CTRL_ID;
    uint16          item_index = 0;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendVideoWallpaperSetList(ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        
        break;       
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:       
        item_index = GUILIST_GetCurItemIndex(ctrl_id);   
        switch(item_index) 
        {
        case MMISET_VIDEO_WALLPAPER_SWITCH:
            if(MMIAPISETVWP_IsOpen())
            {
                MMIAPISET_ResetDefaultWallpaper();
            }
            else
            {

#ifdef PDA_UI_SUPPORT_U_IDLE
            if (0 != MMIAPISET_GetUIdleStyle())
            {
                MMIPUB_OpenQueryWinByTextIdEx(MMK_GetFirstAppletHandle(),
                                              TXT_SET_QUERY_CLOSE_UIDLE_EFFECT, 
                                              IMAGE_PUBWIN_QUERY,
                                              PNULL, 
                                              HandleVWSetQueryWinMsg,
                                              MMISET_VIDEO_WALLPAPER_SWITCH);

                break;                              
                                              
            }
#endif
                MMISET_SetVideoWallpaper(MMISET_SET_WALLPAPER_CTRL_ID);
            }
            GUILIST_RemoveAllItems(ctrl_id);
            AppendVideoWallpaperSetList(ctrl_id);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);
            //GUILIST_ReplaceItem( ctrl_id, &item_t, pos );
            break;
        case MMISET_VIDEO_WALLPAPER_LIST:
            MMK_CreateWin((uint32*)MMISET_SET_VIDEO_LIST_WIN_TAB,PNULL);
            break;
            /*
            case MMISET_VIDEO_WALLPAPER_RANDOM:
            SetVideoRandomSetting(!s_video_setting_info.is_random);
            GUILIST_RemoveAllItems(ctrl_id);
            AppendVideoWallpaperSetList(ctrl_id);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);
            break;
            */
        case MMISET_VIDEO_WALLPAPER_AUDIO_SUPPORT:
            s_video_setting_info.is_audio_support = !s_video_setting_info.is_audio_support;
            SetVideoAudioSetting(s_video_setting_info.is_audio_support);
            GUILIST_RemoveAllItems(ctrl_id);
            AppendVideoWallpaperSetList(ctrl_id);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);
            break;
        default:
            break;
        }
        GUILIST_SetCurItemIndex( ctrl_id,  item_index); //NEWMS00173461 
        break;
        
#ifdef PDA_UI_SUPPORT_U_IDLE
        case MSG_GRID_UIDLE_UPDATE_VW:
            MMISET_SetUIdleStyle(0);
            MMISET_SetVideoWallpaper(MMISET_SET_WALLPAPER_CTRL_ID);
            GUILIST_RemoveAllItems(ctrl_id);
            AppendVideoWallpaperSetList(ctrl_id);
            GUILIST_SetCurItemIndex( ctrl_id,  0); 
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, NULL);
            break;
#endif
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : append video list
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void AppendVideoList(MMI_CTRL_ID_T ctrl_id,
                           uint32 total_num)
{
    uint32              i = 0;
    GUILIST_ITEM_T      item = {0};
    
    GUILIST_SetMaxItem(ctrl_id, total_num, TRUE);
    item.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT; 
    for (i = 0; i < total_num; i++ )
    {
        GUILIST_AppendItem( ctrl_id, &item);
    }
    GUILIST_SetCurItemIndex(ctrl_id, 0);
}

/*****************************************************************************/
//  Description : update video list
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void UpdateVideoList(void)
{
    MMI_CTRL_ID_T ctrl_id = MMISET_VIDEO_LIST_CTRL_ID;
    
    if(GUILIST_GetTotalItemNum(ctrl_id) > 0)
    {
        GUILIST_RemoveAllItems(ctrl_id);
    }
    AppendVideoList(ctrl_id,s_video_wallpaper_info.total_num);
}

/*****************************************************************************/
//  Description : to handle the video list window
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVideoListWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMISET_VIDEO_LIST_CTRL_ID;
    uint32          total_num = 0;
    GUILIST_NEED_ITEM_DATA_T* need_item_data_ptr = PNULL;
    MMISET_VIDEO_LIST_T*        item_ptr = PNULL;
    GUILIST_ITEM_T          item_t = {0};
    GUILIST_ITEM_DATA_T     item_data = {0};
    wchar file_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16 file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    wchar full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 full_path_len = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_PDA_SUPPORT
	if(MMISET_IDLE_ISTYLE != MMIAPISET_GetIdleStyle())
        {
            GUIWIN_SetTitleNeedButton(win_id, TRUE);
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_LEFT, TITLE_BUTTON_INVISIBLE, FALSE);
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonImage(win_id, GUITITLE_BUTTON_RIGHT, IMAGE_CONTROL_TITLE_ADD, IMAGE_CONTROL_TITLE_ADD, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_NULL, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, ctrl_id);
            GUIWIN_SetTitleIsVisible(win_id, TRUE, TRUE);
        }
	else
	{
		GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
	}
#endif

        total_num = GetVideoListTotalNum();
        AppendVideoList(ctrl_id,total_num);

        MMK_SetAtvCtrl(win_id,ctrl_id);   
        break;   
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:        
        need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
        if (PNULL != need_item_data_ptr)
        {
            item_ptr = GetVideoItemByIndex(need_item_data_ptr->item_index);
            if(PNULL != item_ptr)
            {
                full_path_len = item_ptr->full_path_len;
                SCI_MEMCPY(full_path_name, item_ptr->full_path_name, full_path_len*sizeof(wchar));
                MMIAPIFMM_SplitLastName(full_path_name, &full_path_len, 
                     file_name, &file_name_len);                     
                item_t.item_data_ptr = &item_data;     
                item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[0].item_data.image_id = IMAGE_COMMON_MOVIE_UDISK_ICON;
                item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;        
                item_data.item_content[1].item_data.text_buffer.wstr_len = file_name_len;
                item_data.item_content[1].item_data.text_buffer.wstr_ptr = file_name;
                item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;   
                MMIAPIFMM_GetFileSizeString(item_ptr->file_size, temp_wstr,  MMIFMM_SIZE_STR_LEN, TRUE);    
                item_data.item_content[2].item_data.text_buffer.wstr_ptr = temp_wstr;
                item_data.item_content[2].item_data.text_buffer.wstr_len = (uint16)MMIAPICOM_Wstrlen( temp_wstr );
                GUILIST_SetItemData(ctrl_id, &item_data, need_item_data_ptr->item_index);
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[MMISETVWP]: HandleVideoListWinMsg need_item_data_ptr is null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_815_112_2_18_2_52_16_351,(uint8*)"");
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_APP_MENU:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
//#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    //case MSG_CTL_PENOK:
//#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:       
        MMK_CreateWin((uint32*)MMISET_VIDEO_LIST_POPMENU_WIN_TAB,PNULL);
        break;
        
#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_LONGOK:
        //长按List弹出context menu菜单
        MMK_CreateWin((uint32*)MMISET_VIDEO_LIST_LONGOK_POPMENU_WIN_TAB,PNULL);
        break;
    case MSG_NOTIFY_TITLE_MENU_ICON:
        if (MMIAPIUDISK_UdiskIsRun())
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        }
        else
        {
            VideoList_AddItem();
        }
    break;
#endif
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : to handle the video list window
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVideolListOptMenuWinMsg(
                                                  MMI_WIN_ID_T        win_id, 
                                                  MMI_MESSAGE_ID_E    msg_id, 
                                                  DPARAM            param
                                                  )
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id  = MMISET_VIDEO_LIST_OPT_MENU_CTRL_ID;
    MMI_MENU_ID_T               menu_id  = 0;
    MMI_MENU_GROUP_ID_T         group_id = MENU_SET_VIDEO_LIST_OPT;
    uint16                      item_index = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_PDA_SUPPORT
        menu_id = ID_ENVSET_VIDEO_DELETE;
        GUIMENU_SetItemVisible(ctrl_id,group_id,menu_id,FALSE);
#else //NEWMS00171221
        menu_id = ID_ENVSET_VIDEO_DELETE;
        if(s_video_wallpaper_info.total_num == 0)
		{
            GUIMENU_SetItemVisible(ctrl_id,group_id,menu_id,FALSE);   
		}    
#endif
        menu_id = ID_ENVSET_VIDEO_ADD;
        if(s_video_wallpaper_info.total_num >= MMISET_VIDEO_WALLPAPER_MAX_NUM)
        {
            GUIMENU_SetItemVisible(ctrl_id,group_id,menu_id,FALSE);
        }
        else
        {
            GUIMENU_SetItemVisible(ctrl_id,group_id,menu_id,TRUE);
        }
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK: 
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        switch(menu_id) 
        {
        case ID_ENVSET_VIDEO_ADD:
            if (MMIAPIUDISK_UdiskIsRun())
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
            }
            else
            {
                VideoList_AddItem();
            }
            MMK_CloseWin(win_id);
            break;
            
        case ID_ENVSET_VIDEO_DELETE:
            item_index = GUILIST_GetCurItemIndex(MMISET_VIDEO_LIST_CTRL_ID);
            DelItemFromVideoList(item_index);
            VideoListWriteFile();
            UpdateVideoList();
            MMK_CloseWin(win_id);
            break;
        case ID_ENVSET_VIDEO_RESET:
            MMIPUB_OpenQueryWinByTextId(TXT_RESET_QUERY,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            //           MMIPUB_OpenAlertFailWin(TXT_COMPLETE);
            break;
        default:
            break;
        }
        break;
        
        case MSG_PROMPTWIN_OK:
            MMIPUB_CloseQuerytWin(PNULL);
            MMIAPISETVWP_Reset();
            UpdateVideoList();
            /* Modify by michael on 2/23/2012 for  NEWMS00171610:删除"成功"提示框 */
            //MMIPUB_OpenAlertSuccessWin( TXT_SUCCESS);
            /************************Modify over*************************/
            MMK_CloseWin(win_id);
            break;
            
        case MSG_PROMPTWIN_CANCEL:
            MMIPUB_CloseQuerytWin(PNULL);
            break;
            
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    
    return result;
}


/*****************************************************************************/
//  Description : to handle the video list long ok window
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleVideolListLongOKOptMenuWinMsg(
                                                        MMI_WIN_ID_T        win_id, 
                                                        MMI_MESSAGE_ID_E    msg_id, 
                                                        DPARAM            param
                                                        )
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id  = MMISET_VIDEO_LIST_LONGOK_OPT_MENU_CTRL_ID;
    MMI_MENU_ID_T               menu_id  = 0;
    MMI_MENU_GROUP_ID_T         group_id = MENU_SET_VIDEO_LIST_LONGOK_OPT;
    uint16                      item_index = 0;
    GUIMENU_BUTTON_INFO_T       button_info = {0};
#ifdef MMI_PDA_SUPPORT
	MMISET_VIDEO_LIST_T         *item_ptr  = PNULL;
	wchar file_name [MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
	uint16 file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
	wchar full_path_name [MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
	uint16 full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
	MMI_STRING_T        title_string = {0};
#endif
    
    switch(msg_id)
    { 
    case MSG_OPEN_WINDOW:
        //set context menu button style
        button_info.is_static   = TRUE;
        button_info.group_id    = group_id;
#ifdef MMI_PDA_SUPPORT
		button_info.button_style = GUIMENU_BUTTON_STYLE_NONE;	
		item_ptr = GetVideoItemByIndex(GUILIST_GetCurItemIndex(MMISET_VIDEO_LIST_CTRL_ID));
         if(PNULL != item_ptr)
         {               
			 full_path_len = item_ptr->full_path_len;
             SCI_MEMCPY(full_path_name, item_ptr->full_path_name, full_path_len*sizeof(wchar));
             MMIAPIFMM_SplitLastName(full_path_name, &full_path_len, 
                     file_name, &file_name_len);
			 title_string.wstr_len = file_name_len;
			 title_string.wstr_ptr = file_name;
			GUIMENU_SetMenuTitle(&title_string,MMISET_VIDEO_LIST_LONGOK_OPT_MENU_CTRL_ID);
		}
		
		
#else
        button_info.button_style = GUIMENU_BUTTON_STYLE_CANCEL;
#endif
        GUIMENU_SetButtonStyle(ctrl_id, &button_info);
        break;
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK: 
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        switch(menu_id) 
        {  
        case ID_ENVSET_VIDEO_DELETE:
            item_index = GUILIST_GetCurItemIndex(MMISET_VIDEO_LIST_CTRL_ID);
            DelItemFromVideoList(item_index);
            VideoListWriteFile();
            UpdateVideoList();
            MMK_CloseWin(win_id);
            break;
        default:
            break;
        }
        break;
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : 从文件管理器中添加视频文件到列表中
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void VideoList_AddItem(void)
{
    FILEARRAY               ret_array   = NULL;
    MMIFMM_FILTER_T         filter      = {0};
    wchar                   abs_default_path[MMIFMM_FULL_FILENAME_LEN+1]={0};
    uint16                  abs_default_path_length = MMIFMM_FULL_FILENAME_LEN;
	MMIFMM_OPEN_LAYER_WIN_INFO_T open_layer_info = {0};
    
    //SCI_TRACE_LOW:"[MMISETVWP]: VideoList_AddItem"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_1024_112_2_18_2_52_16_352,(uint8*)"");
    
    ret_array = MMIAPIFILEARRAY_Create();
    
    //获得视频文件过滤信息
    MMIAPIVP_GetFilerInfo(&filter);
    
    //打开资源管理器
    MMIAPIVP_GetDefaultFolderPath(abs_default_path,&abs_default_path_length);
	open_layer_info.callback = AddFileToVideoListCallBack;
	open_layer_info.filter_ptr = &filter;
	open_layer_info.func_type = FUNC_FIND_FILE_AND_ALLFOLER;
	open_layer_info.handle_func = PNULL;
	open_layer_info.is_multi_sel = TRUE;
	open_layer_info.is_sel_folder = FALSE;
	open_layer_info.path_len = abs_default_path_length;
	open_layer_info.path_ptr = abs_default_path;
	open_layer_info.ret_array = ret_array;
	open_layer_info.select_cbk = CanbeAddToVideoList;
    if(FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayerEx(&open_layer_info))
    {
        if (PNULL != ret_array)
        {
            MMIAPIFILEARRAY_Destroy(&ret_array);   
            ret_array = PNULL;
        }
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
    }
}

#ifdef PDA_VIDEOPLAYER_WALLPAPER_SUPPORT_LOW_MEMORY
/*****************************************************************************/
//  Description : CheckVideoIsLowMemorySupport
//  Global resource dependence : 
//  Author: andy.he
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  CheckVideoIsLowMemorySupport(DPLAYER_MEDIA_INFO_T *media_info)
{
    BOOLEAN result=FALSE;

    if(media_info == PNULL)
    {
        return FALSE;
    }
    if(0 == media_info->video_info.video_support )
    {
         return FALSE;
    }

#if defined(VIDEOPLAYER_WALLPAPER_FOR_1300KB_AVAILNUM_IN_IDLESCREEN) //HVGA temporarily have 1300KB available memory in idlescreen for videoplayer .
     if(((media_info->video_info.height * media_info->video_info.width <= MMISET_H264_MAX_SIZE_FOR_1300KB_AVAILNUM) /*lint !e775*/
    &&(VIDEO_TYPE_H264 == media_info->video_info.video_type) )
     ||((media_info->video_info.height * media_info->video_info.width <= MMISET_MPEG4_MAX_SIZE_FOR_1300KB_AVAILNUM)
    &&(VIDEO_TYPE_MP4V == media_info->video_info.video_type) )
    ||((media_info->video_info.height * media_info->video_info.width <= MMISET_MPEG4_MAX_SIZE_FOR_1300KB_AVAILNUM)
    &&(VIDEO_TYPE_H263 == media_info->video_info.video_type) )
    || ((media_info->video_info.height * media_info->video_info.width <= MMISET_MJPGE_MAX_SIZE_FOR_1300KB_AVAILNUM) 
    && (VIDEO_TYPE_MJPEG == media_info->video_info.video_type)))/*lint !e775*/
        {
            result=TRUE;
        }

#elif  defined(VIDEOPLAYER_WALLPAPER_FOR_2100KB_AVAILNUM_IN_IDLESCREEN) //WQVGA temporarily have 2100KB available memory in idlescreen for videoplayer .
     if(((media_info->video_info.height * media_info->video_info.width <= MMISET_H264_MAX_SIZE_FOR_2100KB_AVAILNUM)
    &&(VIDEO_TYPE_H264 == media_info->video_info.video_type) )
     ||((media_info->video_info.height * media_info->video_info.width <= MMISET_MPEG4_MAX_SIZE_FOR_2100KB_AVAILNUM)
    &&(VIDEO_TYPE_MP4V == media_info->video_info.video_type) )
    ||((media_info->video_info.height * media_info->video_info.width <= MMISET_MPEG4_MAX_SIZE_FOR_2100KB_AVAILNUM)
    &&(VIDEO_TYPE_H263 == media_info->video_info.video_type) )
    || ((media_info->video_info.height * media_info->video_info.width <= MMISET_MJPGE_MAX_SIZE_FOR_2100KB_AVAILNUM) 
    && (VIDEO_TYPE_MJPEG == media_info->video_info.video_type)))
        {
            result=TRUE;
        }
#elif   defined(VIDEOPLAYER_WALLPAPER_FOR_2400KB_AVAILNUM_IN_IDLESCREEN) //QVGA temporarily have 2400KB available memory in idlescreen for videoplayer .
     if(((media_info->video_info.height * media_info->video_info.width <= MMISET_H264_MAX_SIZE_FOR_2400KB_AVAILNUM)
    &&(VIDEO_TYPE_H264 == media_info->video_info.video_type) )
     ||((media_info->video_info.height * media_info->video_info.width <= MMISET_MPEG4_MAX_SIZE_FOR_2400KB_AVAILNUM)
    &&(VIDEO_TYPE_MP4V == media_info->video_info.video_type) )
    ||((media_info->video_info.height * media_info->video_info.width <= MMISET_MPEG4_MAX_SIZE_FOR_2400KB_AVAILNUM)
    &&(VIDEO_TYPE_H263 == media_info->video_info.video_type) )
    || ((media_info->video_info.height * media_info->video_info.width <= MMISET_MJPGE_MAX_SIZE_FOR_2400KB_AVAILNUM) 
    && (VIDEO_TYPE_MJPEG == media_info->video_info.video_type)))
        {
            result=TRUE;
        }
#elif   defined(VIDEOPLAYER_WALLPAPER_FOR_2900KB_AVAILNUM_IN_IDLESCREEN) //KICF temporarily have 2900KB available memory in idlescreen for videoplayer .
     if(((media_info->video_info.height * media_info->video_info.width <= MMISET_H264_MAX_SIZE_FOR_2900KB_AVAILNUM)
    &&(VIDEO_TYPE_H264 == media_info->video_info.video_type) )
     ||((media_info->video_info.height * media_info->video_info.width <= MMISET_MPEG4_MAX_SIZE_FOR_2900KB_AVAILNUM)
    &&(VIDEO_TYPE_MP4V == media_info->video_info.video_type) )
    ||((media_info->video_info.height * media_info->video_info.width <= MMISET_MPEG4_MAX_SIZE_FOR_2900KB_AVAILNUM)
    &&(VIDEO_TYPE_H263 == media_info->video_info.video_type) )
    || ((media_info->video_info.height * media_info->video_info.width <= MMISET_MJPGE_MAX_SIZE_FOR_2900KB_AVAILNUM) 
    && (VIDEO_TYPE_MJPEG == media_info->video_info.video_type)))
        {
            result=TRUE;
        }
#else
     if(((media_info->video_info.height * media_info->video_info.width <= MMISET_H264_MAX_SIZE_FOR_1300KB_AVAILNUM)
    &&(VIDEO_TYPE_H264 == media_info->video_info.video_type) )
     ||((media_info->video_info.height * media_info->video_info.width <= MMISET_MPEG4_MAX_SIZE_FOR_1300KB_AVAILNUM)
    &&(VIDEO_TYPE_MP4V == media_info->video_info.video_type) )
    ||((media_info->video_info.height * media_info->video_info.width <= MMISET_MPEG4_MAX_SIZE_FOR_1300KB_AVAILNUM)
    &&(VIDEO_TYPE_H263 == media_info->video_info.video_type) )
    || ((media_info->video_info.height * media_info->video_info.width <= MMISET_MJPGE_MAX_SIZE_FOR_1300KB_AVAILNUM) 
    && (VIDEO_TYPE_MJPEG == media_info->video_info.video_type)))
        {
            result=TRUE;
        }
#endif
    return result;
}
#endif

 #ifdef  VIDEOTHUMBNAIL_SUPPORT  
/*****************************************************************************/
//  Description : get image width and height by image path
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WP_GetImgInfoByPath(
                                 wchar* full_path_wstr_ptr,     //in:
                                 GUIIMG_INFO_T  *img_info_ptr   //in/out:
                                 )
{
    BOOLEAN             result = TRUE;
    IMG_DEC_RET_E       get_result = IMG_DEC_RET_SUCCESS;
    IMG_DEC_SRC_T       dec_src = {0};
    IMG_DEC_INFO_T      dec_info = {0}; /*lint !e64*/
    
    if(PNULL == img_info_ptr || PNULL == full_path_wstr_ptr)
    {
        return FALSE;
    }
    
    //set decode src info
    dec_src.file_name_ptr = full_path_wstr_ptr;  /*lint !e413*/

    //get image info
    get_result = IMG_DEC_GetInfo(&dec_src,&dec_info);


    if (IMG_DEC_RET_SUCCESS == get_result)
    {
        //set image info
        switch (dec_info.img_type)
        {
        case IMG_DEC_TYPE_BMP:
            img_info_ptr->image_width  = dec_info.img_detail_info.bmp_info.img_width;
            img_info_ptr->image_height = dec_info.img_detail_info.bmp_info.img_height;
            break;

        case IMG_DEC_TYPE_WBMP:
            img_info_ptr->image_width  = dec_info.img_detail_info.wbmp_info.img_width;
            img_info_ptr->image_height = dec_info.img_detail_info.wbmp_info.img_height;
            break;

        case IMG_DEC_TYPE_JPEG:
            img_info_ptr->image_width  = dec_info.img_detail_info.jpeg_info.img_width;
            img_info_ptr->image_height = dec_info.img_detail_info.jpeg_info.img_heigth;
            break;

        case IMG_DEC_TYPE_PNG:
            img_info_ptr->image_width  = dec_info.img_detail_info.png_info.img_width;
            img_info_ptr->image_height = dec_info.img_detail_info.png_info.img_height;
            break;

        case IMG_DEC_TYPE_GIF:
            img_info_ptr->image_width  = dec_info.img_detail_info.gif_info.img_width;
            img_info_ptr->image_height = dec_info.img_detail_info.gif_info.img_height;
            break;

        case IMG_DEC_TYPE_UNKNOWN:
            break;
        
        default:
            result = FALSE;
            break;
        }
    }
    else
    {
        result = FALSE;
    }
    
    return (result);
}


/*****************************************************************************/
//  Description :WP_DisplayThumbnail  显示视频的对应缩略图
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL  BOOLEAN  WP_DisplayThumbnail(wchar* video_fullpathname)
{
    BOOLEAN             result = FALSE;
    wchar bmp_fullpathname[MMIFMM_FULL_FILENAME_LEN+1]={0};
    //wchar bmp_fullpathname[]=L"E:\\1.bmp";
    GUIIMG_DEC_OUT_T img_output={0};
    GUI_LCD_DEV_INFO* s_mainlcd_layer=MMITHEME_GetDefaultLcdDev();/*lint !e605*/  
    UILAYER_INFO_T  layer_info={0};
 
    if(PNULL == video_fullpathname)
    {
        SCI_TRACE_LOW("[MMISETVWP] WP_DisplayThumbnail video_fullpathname is PNULL !!!");
        
        return result;
    }
    WP_GetThumbnailPathnameForReadEx(bmp_fullpathname, video_fullpathname);

    if(!MMIFILE_IsFileExist(bmp_fullpathname, MMIAPICOM_Wstrlen(bmp_fullpathname)))
    {
        SCI_TRACE_LOW("[MMISETVWP] WP_DisplayThumbnail bmp_fullpathname is not exist !!!");
        return result;
    }
    
    result=WP_DecoderThumbnailBmpImage(bmp_fullpathname,& img_output);

    
    if(result)
    {
            UILAYER_COPYBUF_PARAM_T copy_param={0};
            UILAYER_COPYBUF_T copy_buf={0};
            uint16              width=0;          
            uint16              height=0; 
            height = img_output.actual_height;
            width = img_output.actual_width;
               
            UILAYER_GetLayerInfo(s_mainlcd_layer, &layer_info);

            SCI_TRACE_LOW("[MMISETVWP] WP_DisplayThumbnail bmp_fullpathname height:%d,width:%d",height,width);
            SCI_TRACE_LOW("[MMISETVWP] WP_DisplayThumbnail mainlayer height:%d,width:%d",layer_info.layer_height,layer_info.layer_width);

            if((layer_info.layer_height >= layer_info.layer_width 
                && height>=width) 
                ||(layer_info.layer_height <layer_info.layer_width 
                && height<width) )
            {
                if(layer_info.layer_height < height
                    ||layer_info.layer_width < width)
                {
                    SCI_TRACE_LOW("[MMISETVWP] WP_DisplayThumbnail ERROR BUFFER1");

                    //free vpbits
                    if (PNULL != img_output.decode_data_ptr)
                    {
                        SCI_FREE(img_output.decode_data_ptr);
                        img_output.decode_data_ptr = PNULL;
                    }
                    
                    return FALSE;
                }  
                GUI_FillRect(s_mainlcd_layer, UILAYER_GetLayerRect(s_mainlcd_layer), 0x0000);
                copy_param.rect = UILAYER_GetLayerRect(s_mainlcd_layer);
                copy_param.rect.left += (layer_info.layer_width-width)/2;
                copy_param.rect.top += (layer_info.layer_height-height)/2;                    
                copy_param.rect.right = (copy_param.rect.left + width-1);
                copy_param.rect.bottom = (copy_param.rect.top + height-1);
                
                copy_buf.buf_ptr = (uint8*)img_output.decode_data_ptr;
                copy_buf.data_type = DATA_TYPE_RGB565;
                copy_buf.width = width;
                copy_buf.height = height;
                UILAYER_CopyBufferToLayer(s_mainlcd_layer, &copy_param, &copy_buf);
            } 
            else
            {
                uint8       *layer_buf_ptr = PNULL;
                uint16           *dest_buf_ptr = PNULL;
                uint16           *src_buf_ptr = PNULL;
                uint16 i=0,j=0,dx=0,dy=0;
					
                layer_buf_ptr = UILAYER_GetLayerBufferPtr(s_mainlcd_layer);
                if(PNULL == layer_buf_ptr
                    ||layer_info.layer_height < width
                    ||layer_info.layer_width < height)
                {
                    SCI_TRACE_LOW("[MMISETVWP] WP_DisplayThumbnail ERROR BUFFER2");
                    
                    //free vpbits
                    if (PNULL != img_output.decode_data_ptr)
                    {
                        SCI_FREE(img_output.decode_data_ptr);
                        img_output.decode_data_ptr = PNULL;
                    }
                    
                    return FALSE;
                }

                dx= (layer_info.layer_width - height) /2;
                dy=(layer_info.layer_height - width)/2;
                GUI_FillRect(s_mainlcd_layer, UILAYER_GetLayerRect(s_mainlcd_layer), 0x0000);
                
                dest_buf_ptr = (uint16*)layer_buf_ptr;
                src_buf_ptr = (uint16*)img_output.decode_data_ptr;
                for(i=0;i<width;i++)
                {
                    for(j=0;j<height;j++)
                    {
                        *(dest_buf_ptr + (i+dy)*layer_info.layer_width + layer_info.layer_width-1-(j+dx)) = *(src_buf_ptr + j*width + i);
                    }
                }
            }
            //MMITHEME_StoreUpdateRect(MMITHEME_GetDefaultLcdDev(),MMITHEME_GetFullScreenRect());
            //MMITHEME_UpdateRect();
        }

        //free vpbits
    if (PNULL != img_output.decode_data_ptr)
    {
        SCI_FREE(img_output.decode_data_ptr);
        img_output.decode_data_ptr = PNULL;
    }

    SCI_TRACE_LOW("[MMISETVWP] WP_DisplayThumbnail  result:%d",result);
    return result;
  
}

/*****************************************************************************/
//  Description :MMIAPISETVWP_DisplayThumbnail  显示视频的对应缩略图
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC  void  MMIAPISETVWP_DisplayThumbnail(void)
{
            MMISET_VIDEO_LIST_T* item_ptr = PNULL;
            uint16 cur_index= s_video_wallpaper_cur_index+1;

            if(s_video_wallpaper_info.total_num > MMISET_VIDEO_WALLPAPER_MAX_NUM)
            {
            //SCI_TRACE_LOW:"[MMISETVWP]:GetNextVideo total_num ERROR!!!!!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_383_112_2_18_2_52_15_347,(uint8*)"");
            s_video_wallpaper_info.total_num = MMISET_VIDEO_WALLPAPER_MAX_NUM;
            }
            if(cur_index >= s_video_wallpaper_info.total_num)
            {
                cur_index = 0;
            }
            item_ptr = GetVideoItemByIndex(cur_index);
            if((!GUIMAINMENU_IsMoveState(MAINMENUWIN_MENU_CTRL_ID) && !CheckAllVideoIsInValid()) && PNULL != item_ptr && !s_video_wallpaper_info.is_invalid[cur_index])
            {
                 WP_DisplayThumbnail(item_ptr->full_path_name);
            }
            else
            {
                GUI_RECT_T bg_rect=MMITHEME_GetFullScreenRect();
                MMI_IMAGE_ID_T bg_img_id = MMIAPISET_GetWallpaperByIndex(MMIAPISET_GetDefaultWallpaperIDByStyle());
                
                GUIRES_DisplayImg(PNULL,
                &bg_rect,
                &bg_rect,
                MAIN_IDLE_WIN_ID,
                bg_img_id,
                MMITHEME_GetDefaultLcdDev());
            
            }
}
/*****************************************************************************/
//  Description : 获取bmp_fullpathname 的图片img数据
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL  BOOLEAN  WP_DecoderThumbnailBmpImage(wchar* bmp_fullpathname ,GUIIMG_DEC_OUT_T *img_output)
{
    BOOLEAN             result = FALSE;
    GUIIMG_SRC_T        img_src = {0};
    GUIIMG_DISPLAY_T    img_display = {0};
    GUIIMG_INFO_T     img_info_ptr={0};	

    if(PNULL == bmp_fullpathname || PNULL == img_output)
    {
        SCI_TRACE_LOW("[MMISETVWP] WP_DecoderThumbnailBmpImage bmp_fullpathname or img_output  is PNULL !!!");
        
        return result;
    }
    
    //set image source 
    img_src.is_file=TRUE;
    img_src.full_path_ptr=bmp_fullpathname;

    //get image source width and height
    WP_GetImgInfoByPath(bmp_fullpathname, &img_info_ptr);


    //set image dest width and height
    img_display.dest_width = img_info_ptr.image_width;
    img_display.dest_height = img_info_ptr.image_height;
    img_output->decode_data_size = img_info_ptr.image_height * img_info_ptr.image_width *sizeof(GUI_COLOR_T);


    //set image decode output data buffer
    img_output->decode_data_ptr = (uint8*)SCI_ALLOCA( img_output->decode_data_size);
    if (PNULL == img_output->decode_data_ptr)
    {
        SCI_TRACE_LOW("[MMISETVWP] WP_DecoderThumbnailBmpImage img_output.decode_data_ptr is PNULL !!!");
        return result;
    }
 
    //decode jpg
    result = GUIIMG_Decode(&img_src,
                &img_display,
                img_output);
    
    SCI_TRACE_LOW("[MMISETVWP] WP_DecoderThumbnailBmpImage  result:%d",result);
    return (result);
}


/*****************************************************************************/
//  Description :WP_GetThumbnailPathnameForWriteEx
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL  BOOLEAN  WP_GetThumbnailPathnameForWriteEx(wchar* thumbnai_path,wchar* videopath)
{
	BOOLEAN result=TRUE;
	int32 filename_len=0;
	int32 list_name_len=0;
	uint16  index = 0;
	wchar  list_file_name [MMIFMM_FULL_FILENAME_LEN+1]= {0};    
	uint16  pathlen=MMIFMM_FULL_FILENAME_LEN;
		
	
    if(thumbnai_path == PNULL || videopath==PNULL || 0 == MMIAPICOM_Wstrlen(videopath))
    {
        return FALSE;
    }

        filename_len=MMIAPICOM_Wstrlen(videopath);
        index = MMIFMM_GetFinallySlashIndex(videopath,(filename_len -1));  
        list_name_len = (uint16)(filename_len - 1 - index);
        MMIAPICOM_Wstrncpy( list_file_name,  videopath+index + 1, list_name_len);
        MMIAPICOM_Wstrncpy( list_file_name+list_name_len,  L".bmp", 4);

    result = MMIAPIFMM_GetSysFileFullPathForWriteEx(
        TRUE,
        FALSE,
        MMISET_VIDEO_SYSFILE_DIR,
        MMISET_VIDEO_SYSFILE_DIR_LEN,
        list_file_name,
        MMIAPICOM_Wstrlen(list_file_name),
        0,
        thumbnai_path,
        &pathlen
        );
    return result;
}


/*****************************************************************************/
//  Description :WP_GetThumbnailPathnameForReadEx
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL  BOOLEAN  WP_GetThumbnailPathnameForReadEx(wchar* thumbnai_path,wchar* videopath)
{
	BOOLEAN result=TRUE;
	int32 filename_len=0;
	int32 list_name_len=0;
	uint16  index = 0;
	wchar  list_file_name [MMIFMM_FULL_FILENAME_LEN+1]= {0};    
	uint16  pathlen=MMIFMM_FULL_FILENAME_LEN;
		
	
        if(thumbnai_path == PNULL || videopath==PNULL || 0 == MMIAPICOM_Wstrlen(videopath))
        {
            return FALSE;
        }

        filename_len=MMIAPICOM_Wstrlen(videopath);
        index = MMIFMM_GetFinallySlashIndex(videopath,(filename_len -1));  
        list_name_len = (uint16)(filename_len - 1 - index);
        MMIAPICOM_Wstrncpy( list_file_name,  videopath+index + 1, list_name_len);
        MMIAPICOM_Wstrncpy( list_file_name+list_name_len,  L".bmp", 4);

         result = MMIAPIFMM_GetSysFileFullPathForReadEx(
        TRUE,
        FALSE,
        MMISET_VIDEO_SYSFILE_DIR,
        MMISET_VIDEO_SYSFILE_DIR_LEN,
        list_file_name,
        MMIAPICOM_Wstrlen(list_file_name),
        thumbnai_path,
        &pathlen
        );
           
    return result;
}


/*****************************************************************************/
//  Description : WP_VideoThumbnailCallback
//  Global resource dependence : 
//  Author:  
//  Note:  
/*****************************************************************************/

LOCAL  BOOLEAN  WP_VideoThumbnailCallback(DPLAYER_IMAGE_DATA_T *image_data)
{
	wchar  thumbnail_file_name [MMIFMM_FULL_FILENAME_LEN+1]= {0};   
	uint32  image_dataheight=0;
	uint32  image_datawidth=0;
	BOOLEAN result=FALSE;
	if(image_data == PNULL )
        {
            return FALSE;
        }
    SCI_TRACE_LOW("[MMISETVWP] WP_VideoThumbnailCallback entry !!!");

#if 0
	static int i=0;
	char test_thumbnail_fullpath[MMIFMM_FULL_FILENAME_LEN+1]={0};
	wchar uni_test_thumbnail_fullpath[MMIFMM_FULL_FILENAME_LEN+1]={0};
	sprintf(test_thumbnail_fullpath, "E:\\wp\\%d.bmp",i);
	MMIAPICOM_StrToWstr(test_thumbnail_fullpath, uni_test_thumbnail_fullpath);
#endif
	image_dataheight =image_data->coordinate.trim_h;
	image_datawidth = image_data->coordinate.trim_w;

	WP_GetThumbnailPathnameForWriteEx(thumbnail_file_name,s_wp_video_getthumbnail_name);
	result=MMIAPICOM_WriteBMPFile(thumbnail_file_name,image_datawidth, image_dataheight, (uint8*) image_data->buf.oneplane.p_data, 16);

#if 0
	result=MMIAPICOM_WriteBMPFile(uni_test_thumbnail_fullpath,image_datawidth, image_dataheight, (uint8*) image_data->buf.oneplane.p_data, 16);
	i++;
#endif    
	SCI_MEMSET((void*)s_wp_video_getthumbnail_name,0x00,sizeof(s_wp_video_getthumbnail_name)/sizeof(wchar)); 
    SCI_TRACE_LOW("[MMISETVWP] WP_VideoThumbnailCallback exit !!!");
    return result;
}


/*****************************************************************************/
//  Description : WP_GetVideothumnail
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN WP_GetVideothumnail(wchar *video_fullpath_name,uint16 video_paht_len)
{
	VP_DISPLAY_PARAM_T  display_param={0};
	DPLAYER_MEDIA_INFO_T media_info={0};
	BOOLEAN result=FALSE;

	if(video_fullpath_name == PNULL  || 0 == MMIAPICOM_Wstrlen(video_fullpath_name))
        {
            return FALSE;
        }

	MMIAPIVP_GetInfoFromFullPathName(video_fullpath_name,video_paht_len,&media_info);
	display_param = GetDisplayParam(&media_info);
	display_param.audioSupport =0;
	display_param.videoSupport = 1;
	display_param.b_support_seek = 1;
	display_param.RotateAngle = 0;//GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID);//s_video_player_control_info.cur_angle; 
	display_param.disp_data_op = DPLAYER_DISP_DATA_GET;
        display_param.b_no_osd = 1;
	
     MMIAPIVP_TranslateDisplayParamLogicToPhysical(&display_param, FALSE);
     SCI_MEMSET((void*)s_wp_video_getthumbnail_name,0x00,sizeof(s_wp_video_getthumbnail_name)/sizeof(wchar));
    
    if(MMIAPIVP_ThumbnailOpen(video_fullpath_name, MMIAPICOM_Wstrlen(video_fullpath_name) ,0,WP_VideoThumbnailCallback,&display_param,FALSE))
    {
	SCI_MEMCPY((void*)s_wp_video_getthumbnail_name,(void*)video_fullpath_name,video_paht_len*sizeof(wchar));
	result=TRUE;
    }
    return result;
}
#endif
/*****************************************************************************/
//  Description : CanbeAddToVideoList
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CanbeAddToVideoList(uint16             total_mark_num,
                                  FILEARRAY_DATA_T   * file_data_ptr)
{
	MMISET_VIDEO_LIST_T video_item = {0};
    DPLAYER_MEDIA_INFO_T media_info = {0};

	SCI_TRACE_LOW("CanbeAddToVideoList total_mark_num = %d",total_mark_num);
	
    if (PNULL == file_data_ptr)
    {
		return FALSE;
    }    
	else if (total_mark_num > (MMISET_VIDEO_WALLPAPER_MAX_NUM - s_video_wallpaper_info.total_num))
	{	
		MMIPUB_OpenAlertWarningWin(TXT_SET_WALLPAPER_VIDEO_LIST_FULL);
		return FALSE;
    }
	else
	{	

		SCI_MEMSET(&video_item,0,sizeof(MMISET_VIDEO_LIST_T));
		MMIAPICOM_Wstrncpy(video_item.full_path_name,file_data_ptr->filename,file_data_ptr->name_len);
		video_item.full_path_len = file_data_ptr->name_len;
		video_item.file_size = file_data_ptr->size;                  
#ifdef DRM_SUPPORT//NEWMS00184327
		if (!MMIAPIDRM_CanDrmFileSetToWSPC(video_item.full_path_name, video_item.full_path_len, DRMFILE_SETTING_SCREENSAVER))        
		{
			//不可以设置此DRM文件
			MMIPUB_OpenAlertWarningWin(TXT_DRM_NOT_SET_VIDEOWALLPAPER);
			return FALSE;
		}
#endif 
#ifdef VIDEOTHUMBNAIL_SUPPORT
        if(MMIAPIVP_IsThumbnailOpen())
        {
            MMIPUB_OpenAlertWarningWin(TXT_VP_INSTANCE_WATING);
            return FALSE;
        }
		else if(!MMIAPIVP_GetInfoFromFullPathName(video_item.full_path_name,video_item.full_path_len,&media_info))
#else
       if(!MMIAPIVP_GetInfoFromFullPathName(video_item.full_path_name,video_item.full_path_len,&media_info))		
#endif
		{
			MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
			return FALSE;
		}
#ifndef PDA_VIDEOPLAYER_WALLPAPER_SUPPORT_LOW_MEMORY
		if(media_info.video_info.height * media_info.video_info.width <= MMISET_VIDEO_MAX_VIDEO_SIZE
			&& 1 == media_info.video_info.video_support)
#else
		if(CheckVideoIsLowMemorySupport(&media_info))
#endif 
		{
#ifdef  VIDEOTHUMBNAIL_SUPPORT  
                WP_GetVideothumnail(video_item.full_path_name,video_item.full_path_len); 		 
#endif
			return TRUE;			
		}
		else 
		{
			MMIPUB_OpenAlertWarningWin(TXT_VP_VIDEO_WALLPAPER_UNSUPPORTED);           
			return FALSE;
		}
	}
	
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void AddFileToVideoListCallBack(
                                      BOOLEAN     is_success,
                                      FILEARRAY   file_array
                                      )
{
	uint32          i = 0;
    uint32          file_num = 0;
    FILEARRAY_DATA_T file_array_data = {0};
    MMISET_VIDEO_LIST_T video_item = {0};   
	
	file_num = MMIAPIFILEARRAY_GetArraySize(file_array);

	SCI_TRACE_LOW("AddFileToVideoListCallBack file_num = %d",file_num);

	if (file_num > MMISET_VIDEO_WALLPAPER_MAX_NUM - s_video_wallpaper_info.total_num)
	{
		MMIPUB_OpenAlertWarningWin(TXT_SET_WALLPAPER_VIDEO_LIST_FULL);
		file_num = MMISET_VIDEO_WALLPAPER_MAX_NUM - s_video_wallpaper_info.total_num;
	}

	if(is_success && 0 < file_num)
	{	
		
		for(i=0; i<file_num; i++)
		{
			if(MMIAPIFILEARRAY_Read(file_array, i, &file_array_data))
            {
                SCI_MEMSET(&video_item,0,sizeof(MMISET_VIDEO_LIST_T));
                MMIAPICOM_Wstrncpy(video_item.full_path_name,file_array_data.filename,file_array_data.name_len);
                video_item.full_path_len = file_array_data.name_len;
                video_item.file_size = file_array_data.size;                  
				AddItemToVideoList(&video_item);                
            }
            else
            {
				SCI_TRACE_LOW("AddFileToVideoListCallBack read error");
                continue;
            }
        }
        VideoListWriteFile();
        UpdateVideoList();
		
	}
    MMIAPIFILEARRAY_Destroy(&file_array);
 }

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
///*****************************************************************************/
//LOCAL void AddFileToVideoListCallBack(
//                                      BOOLEAN     is_success,
//                                      FILEARRAY   file_array
//                                      )
//{    
//    uint32          i = 0;
//	uint32          index = 0;
//    uint32          file_num = 0;
//    FILEARRAY_DATA_T file_array_data = {0};
//    MMISET_VIDEO_LIST_T video_item = {0};
//
//    DPLAYER_MEDIA_INFO_T media_info = {0};
//    uint32          limited_file_num = 0;
//	uint32          max_add_item = 0;
//    
//    file_num = MMIAPIFILEARRAY_GetArraySize(file_array);
//    if(is_success && 0 < file_num)
//    {
//		i = 0;
//		index = 0;
//		max_add_item = MIN(file_num, (MMISET_VIDEO_WALLPAPER_MAX_NUM - s_video_wallpaper_info.total_num));
//        while (i< max_add_item	&& index < file_num)
//        {
//			//读取下一条要写的数据
//            if(MMIAPIFILEARRAY_Read(file_array, index, &file_array_data))
//            {
//                SCI_MEMSET(&video_item,0,sizeof(MMISET_VIDEO_LIST_T));
//                MMIAPICOM_Wstrncpy(video_item.full_path_name,file_array_data.filename,file_array_data.name_len);
//                video_item.full_path_len = file_array_data.name_len;
//                video_item.file_size = file_array_data.size;                  
//
//                if (MMIAPIVP_GetInfoFromFullPathName(video_item.full_path_name,video_item.full_path_len,&media_info)
//#ifdef DRM_SUPPORT
//					&& MMIAPIDRM_CanDrmFileSetToWSPC(video_item.full_path_name, video_item.full_path_len, DRMFILE_SETTING_SCREENSAVER)      
//#endif
//#ifndef PDA_VIDEOPLAYER_WALLPAPER_SUPPORT_LOW_MEMORY
//					&& media_info.video_info.height * media_info.video_info.width <= MMISET_VIDEO_MAX_VIDEO_SIZE
//                    && 1 == media_info.video_info.video_support
//#else
//					&& CheckVideoIsLowMemorySupport(&media_info)
//#endif
//					)
//				{
//                    AddItemToVideoList(&video_item);
//					i++;
//                } 
//				else
//				{
//					limited_file_num++;
//				}
//			}
//			index++;			
//        }
//
//		SCI_TRACE_LOW("AddFileToVideoListCallBack i = %d, index = %d, limited_file_num = %d, file_num = %d, max_add_item = %d", i, index,limited_file_num,file_num, max_add_item);
//
//        VideoListWriteFile();
//        UpdateVideoList();
//    }
//          
//	MMIAPIFILEARRAY_Destroy(&file_array);	
//	
//    if(0 < limited_file_num)
//    {
//        MMIPUB_OpenAlertWarningWin(TXT_VIDEO_WALLPAPER_NOT_ALL_SUPPORT);
//        limited_file_num = 0;
//    }
//	else if(index < file_num)
//    {
//        MMIPUB_OpenAlertWarningWin(TXT_SET_WALLPAPER_VIDEO_LIST_FULL);
//    }
//
// }

/*****************************************************************************/
//  Description : VideoListWriteFile
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void VideoListWriteFile(void)
{
    BOOLEAN     result = FALSE;
    BOOLEAN     write_result = TRUE;
    wchar       full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16      full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMIFILE_HANDLE  file_handle = SFS_INVALID_HANDLE; 
    MMISET_VIDEO_LIST_HEAD_T list_head = {0};
    uint32 write_size = 0;
    uint8* buffer_ptr = PNULL;
    uint32 load_size = 0;
    uint32 write_num = 0;
    uint32 list_len = 0;
    MMISET_VIDEO_LIST_T* cursor_ptr = PNULL;
    uint32 i=0;
    
    SCI_MEMSET(&s_video_wallpaper_info.is_invalid,0,MMISET_VIDEO_WALLPAPER_MAX_NUM*sizeof(BOOLEAN));
    s_video_wallpaper_cur_index = 0xFF;
    result = MMIAPIFMM_GetSysFileFullPathForWriteEx(
        TRUE,
        TRUE,
        MMISET_VIDEO_SYSFILE_DIR,
        MMISET_VIDEO_SYSFILE_DIR_LEN,
        MMISET_VIDEO_SYSFILE_NAME,
        MMISET_VIDEO_SYSFILE_NAME_LEN,
        0,
        full_path_name,
        &full_path_len
        );
    file_handle = MMIAPIFMM_CreateFile(full_path_name, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);/*lint !e655*/
    if(SFS_INVALID_HANDLE != file_handle)
    {
        SCI_MEMCPY(list_head.version,MMISET_VIDEO_WALLPAPER_LIST_VER,MMISET_VIDEO_WALLPAPER_LIST_VER_LEN);
        list_head.total_num = s_video_wallpaper_info.total_num;
        MMIAPIFMM_WriteFile(file_handle, &list_head, sizeof(MMISET_VIDEO_LIST_HEAD_T), &write_size, PNULL);
        if(list_head.total_num > 0)
        {
            list_len = sizeof(MMISET_VIDEO_LIST_T);
            write_num = list_head.total_num;
            load_size = write_num * list_len;
            buffer_ptr = SCI_ALLOC_APP(load_size);
            cursor_ptr= s_video_wallpaper_info.list_head_ptr;
            for(i=0; i<write_num && PNULL != cursor_ptr; i++)
            {
                SCI_MEMCPY((buffer_ptr+i*list_len),cursor_ptr,list_len);
                cursor_ptr = cursor_ptr->next_ptr;
            }
            if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, buffer_ptr, load_size, &write_size, PNULL))
            {
                //链表写出错
                //SCI_TRACE_LOW:"[MMISETVWP]:VideoListWriteFile FAIL, fetal error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_1272_112_2_18_2_52_17_353,(uint8*)"");
                write_result = FALSE;
            }  
            SCI_FREE(buffer_ptr);
        }
    }
    if(SFS_INVALID_HANDLE != file_handle)
    {
        MMIAPIFMM_CloseFile(file_handle);
    }
    if(!write_result)
    {
        //SCI_TRACE_LOW:"[MMISETVWP]:VideoListWriteFile ERROR!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_1284_112_2_18_2_52_17_354,(uint8*)"");
        MMIAPIFMM_DeleteFileSyn(full_path_name,full_path_len);
        FreeVideoList();
    }
}

/*****************************************************************************/
//  Description : VideoListWriteFile
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN VideoListReadFile(void)
{
    BOOLEAN     read_result = FALSE;
    wchar       full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16      full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMIFILE_HANDLE  file_handle = SFS_INVALID_HANDLE; 
    MMISET_VIDEO_LIST_HEAD_T list_head = {0};
    uint32 read_size = 0;
    uint8* buffer_ptr = PNULL;
    uint32 load_size = 0;
    uint32 load_num = 0;
    uint32 list_len = 0;
    MMISET_VIDEO_LIST_T list_item = {0};
    uint32 i=0;
    
    MMIAPIFMM_GetSysFileFullPathForReadEx(
        TRUE,
        TRUE,
        MMISET_VIDEO_SYSFILE_DIR,
        MMISET_VIDEO_SYSFILE_DIR_LEN,
        MMISET_VIDEO_SYSFILE_NAME,
        MMISET_VIDEO_SYSFILE_NAME_LEN,
        full_path_name,
        &full_path_len
        );
    file_handle = MMIAPIFMM_CreateFile(full_path_name, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
    if(SFS_INVALID_HANDLE != file_handle)
    {
        if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(
            file_handle, 
            &list_head, 
            sizeof(MMISET_VIDEO_LIST_HEAD_T), 
            &read_size, 
            NULL
            ))
        {
            if(CheckVideoListVer(list_head.version))
            {
                read_result = TRUE;
            }
        }
    }
    if(read_result)
    {
        s_video_wallpaper_info.total_num = list_head.total_num;
        load_num = MIN(MMISET_VIDEO_WALLPAPER_MAX_NUM, list_head.total_num);
        if(load_num > 0)
        {
            list_len = sizeof(MMISET_VIDEO_LIST_T);
            load_size = load_num * list_len;
            buffer_ptr = SCI_ALLOC_APP(load_size);
            if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(file_handle, buffer_ptr, load_size, &read_size, NULL))
            {
                for(i=0; i<load_num; i++)
                {
                    SCI_MEMCPY(&list_item,(buffer_ptr+i*list_len),list_len);
                    AddItemToVideoList(&list_item);
                } 
            }
            else
            {
                //链表出错，清空文件夹
                //SCI_TRACE_LOW:"[MMISETVWP]:VideoListReadFile ERROR!!!!!!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_1356_112_2_18_2_52_17_355,(uint8*)"");
                read_result = FALSE;
            }
            SCI_FREE(buffer_ptr);
        }
        else
        {
            //SCI_TRACE_LOW:"[MMISETVWP]:VideoListReadFile empty!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_1363_112_2_18_2_52_17_356,(uint8*)"");
            read_result = FALSE;
        }
    }
    if(SFS_INVALID_HANDLE != file_handle)
    {
        MMIAPIFMM_CloseFile(file_handle);
    }
    if(!read_result)
    {
        MMIAPIFMM_DeleteFileSyn(full_path_name,full_path_len);
        FreeVideoList();
    }
    
    return read_result;
}

/*****************************************************************************/
//  Description : FreeVideoList
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void FreeVideoList(void)
{
    MMISET_VIDEO_LIST_T *cur_ptr = s_video_wallpaper_info.list_head_ptr;
    
    while(PNULL != s_video_wallpaper_info.list_head_ptr)
    {
        cur_ptr = s_video_wallpaper_info.list_head_ptr;
        s_video_wallpaper_info.list_head_ptr = s_video_wallpaper_info.list_head_ptr->next_ptr;
        SCI_FREE(cur_ptr);
    }
    SCI_MEMSET(&s_video_wallpaper_info,0,sizeof(MMISET_VIDEO_WALLPAPER_LIST_T));
}

/*****************************************************************************/
//  Description : CheckVideoListVer
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckVideoListVer(uint8 *version_ptr)
{
    if(PNULL != version_ptr 
        && 0 == MMIAPICOM_Stricmp(version_ptr, (uint8 *)MMISET_VIDEO_WALLPAPER_LIST_VER))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : AddItemToVideoList
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void AddItemToVideoList(MMISET_VIDEO_LIST_T * item_ptr)
{
    MMISET_VIDEO_LIST_T *cur_ptr = s_video_wallpaper_info.list_head_ptr;
    MMISET_VIDEO_LIST_T *list_item_ptr = PNULL;
    
    if(PNULL != item_ptr)
    {
        list_item_ptr = SCI_ALLOC_APP(sizeof(MMISET_VIDEO_LIST_T));
        if(PNULL == list_item_ptr)
        {
            return;
        }
        SCI_MEMCPY(list_item_ptr, item_ptr, sizeof(MMISET_VIDEO_LIST_T));
        list_item_ptr->next_ptr = PNULL;
        if(PNULL == cur_ptr)
        {
            s_video_wallpaper_info.list_head_ptr = list_item_ptr;
            s_video_wallpaper_info.total_num = 1;
        }
        else
        {
            while(PNULL != cur_ptr->next_ptr)
            {
                cur_ptr = cur_ptr->next_ptr;
            }
            cur_ptr->next_ptr = list_item_ptr;
            s_video_wallpaper_info.total_num++;
        }
    }
}

/*****************************************************************************/
//  Description : DelItemFromVideoList
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void DelItemFromVideoList(uint32 index)
{
    MMISET_VIDEO_LIST_T *cur_ptr = s_video_wallpaper_info.list_head_ptr;
    uint32 i = 0;
    MMISET_VIDEO_LIST_T *list_item_ptr = PNULL;
    
    if(PNULL != cur_ptr)
    {
        if(0 == index)
        {
            list_item_ptr = s_video_wallpaper_info.list_head_ptr;
            s_video_wallpaper_info.list_head_ptr = list_item_ptr->next_ptr;
            SCI_FREE(list_item_ptr);
            s_video_wallpaper_info.total_num--;
        }
        else
        {
            while(PNULL != cur_ptr)
            {
                if(++i == index)
                {
                    break;
                }
                else
                {
                    cur_ptr = cur_ptr->next_ptr;
                }
            }
            if(PNULL != cur_ptr
                &&PNULL != cur_ptr->next_ptr)
            {
                list_item_ptr = cur_ptr->next_ptr;
                cur_ptr->next_ptr = list_item_ptr->next_ptr;
                SCI_FREE(list_item_ptr);
                s_video_wallpaper_info.total_num--;
            }
        }
    }
}

/*****************************************************************************/
//  Description : Get Video List Total Num
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetVideoListTotalNum(void)
{
    return s_video_wallpaper_info.total_num;
}

/*****************************************************************************/
//  Description : Get Video item by index
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL MMISET_VIDEO_LIST_T* GetVideoItemByIndex(uint32 index)
{
    MMISET_VIDEO_LIST_T *cur_ptr = s_video_wallpaper_info.list_head_ptr;
    
    while(0 != index --)
    {
        if(PNULL != cur_ptr)
        {
            cur_ptr = cur_ptr->next_ptr;
        }
        else
        {
            //SCI_TRACE_LOW:"[MMISETVWP]: GetVideoItemByIndex fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_1518_112_2_18_2_52_17_357,(uint8*)"");
            return PNULL;
        }
    }
    
    return cur_ptr;
}

/*****************************************************************************/
//  Description : reset setting info
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void ResetSettingInfo(void)
{
    s_video_setting_info.is_audio_support  = TRUE;
    s_video_setting_info.is_random  = FALSE;
    MMINV_WRITE(MMINV_SET_VIDEO_WALLPAPER, &s_video_setting_info);
}

/*****************************************************************************/
//  Description : video setting init
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void VideoSettingInit(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_SUCCESS;
    
    MMINV_READ(MMINV_SET_VIDEO_WALLPAPER, &s_video_setting_info, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {           
        ResetSettingInfo();
    }
}

/*****************************************************************************/
//  Description : Get video setting
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetVideoRandomSetting(void)
{
    return s_video_setting_info.is_random;
}

/*****************************************************************************/
//  Description : Set video random setting
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void SetVideoRandomSetting(BOOLEAN is_random)
{
    //SCI_TRACE_LOW:"[MMISETVWP]:SetVideoRandomSetting is_random=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_1567_112_2_18_2_52_17_358,(uint8*)"d",is_random);
    s_video_setting_info.is_random = is_random;
    MMINV_WRITE(MMINV_SET_VIDEO_WALLPAPER, &s_video_setting_info);
}

/*****************************************************************************/
//  Description : Get video audiosupport setting
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetVideoAudioSetting(void)
{
    //SCI_TRACE_LOW:"[MMISETVWP]:GetVideoAudioSetting is_audio_support=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_1578_112_2_18_2_52_17_359,(uint8*)"d",s_video_setting_info.is_audio_support);
    return s_video_setting_info.is_audio_support;
}

/*****************************************************************************/
//  Description : Set video audiosupport setting
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void SetVideoAudioSetting(BOOLEAN is_audio_support)
{
    //SCI_TRACE_LOW:"[MMISETVWP]:SetVideoAudioSetting is_audio_support=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_1588_112_2_18_2_52_17_360,(uint8*)"d",is_audio_support);
    s_video_setting_info.is_audio_support = is_audio_support;
    MMINV_WRITE(MMINV_SET_VIDEO_WALLPAPER, &s_video_setting_info);
}
/*****************************************************************************/
//  Description : 有些时候视频DPLAYEROPEN 成功，但之后解码器在解码中会失败 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckAllVideoIsInValid(void)
{
    uint8 index=0;
    BOOLEAN result=TRUE;
    //SCI_TRACE_LOW:"[MMISETVWP]:CheckAllVideoIsInValid entry"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_1600_112_2_18_2_52_17_361,(uint8*)"");
    for(index=0;index<s_video_wallpaper_info.total_num;index++)
    {
        if(!s_video_wallpaper_info.is_invalid[index])
        {
            result=FALSE;
            break;
        }
    }
    SCI_TRACE_LOW("[MMISETVWP]:CheckAllVideoIsInValid result:%d",result);
    return result;
}

/*****************************************************************************/
//  Description : ResetAllVideoIsValid
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_ResetAllVideoIsValid(void)
{
    uint8 index=0;
    BOOLEAN result=TRUE;
    SCI_TRACE_LOW("[MMISETVWP]:ResetAllVideoIsValid");
    for(index=0;index<s_video_wallpaper_info.total_num;index++)
    {
        s_video_wallpaper_info.is_invalid[index] = FALSE;

    }

    return result;
}

/*****************************************************************************/
// 	Description : HandleVideoScreenSaverMsg
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void HandleDynWallpaperPlayMsg(VP_CALLBACK_T *msg_ptr)
{
    VP_PLAY_END_IND_T  *play_end_ind_ptr = PNULL;
    VP_PLAY_END_RESULT_E play_end_result = 0;
    static BOOLEAN       s_is_play_end = FALSE;
    
    if (PNULL == msg_ptr)
    {
        return;
    }
    
    //SCI_TRACE_LOW:"[MMISETVWP] HandleDynWallpaperPlayMsg msg_ptr->msg_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_1627_112_2_18_2_52_17_362,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id) 
    {
    case VP_PLAY_END_IND:
        play_end_ind_ptr = (VP_PLAY_END_IND_T *)msg_ptr->para;        
        play_end_result = play_end_ind_ptr->result;  
        //SCI_TRACE_LOW:"[MMISETVWP] HandleDynWallpaperPlayMsg play_end_result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_1633_112_2_18_2_52_17_363,(uint8*)"d",play_end_result);
        if(play_end_result == VP_PLAY_END_ERROR ||play_end_result == VP_PLAY_END_NOT_SUPPORT)
        {
            s_video_wallpaper_info.is_invalid[s_video_wallpaper_cur_index]=TRUE;
        }
        s_is_play_end = TRUE;
        break;
        
    case VP_STOP_IND: 
        if(CheckAllVideoIsInValid())
        {
            MMIAPISETVWP_StopVideoWallpaper();
            MMIAPISET_ResetDefaultWallpaper(); 
            MMIAPISETVWP_ResetAllVideoIsValid();
        }
        else
        {
            play_end_result = VP_PLAY_END_STOPPED_BY_USER;   
            if(s_is_play_end)
            {
                MMIAPISETVWP_SwitchToNext();
                s_is_play_end = FALSE;
            }
        }
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : PlayNextVideoWallpaper
//	Global resource dependence : 
//  Author:aoke.hu
//	Note: 
/*****************************************************************************/
LOCAL VP_DISPLAY_PARAM_T GetDisplayParam(DPLAYER_MEDIA_INFO_T *media_info_ptr)
{
    GUI_RECT_T full_rect = {0};
    VP_DISPLAY_PARAM_T display_param = {0};

    if(PNULL == media_info_ptr)
    {
        return display_param;
    }
    
    full_rect = MMITHEME_GetFullScreenRect(); 
    display_param.disp_rect.dx = full_rect.right - full_rect.left + 1;           
    display_param.disp_rect.dy = full_rect.bottom - full_rect.top + 1;

    display_param.target_rect = display_param.disp_rect;
    display_param.disp_mode = DPLAYER_DISP_CUSTOMIZE;    
//#ifdef MMI_VIDEOWALLPAPER_SUPPORT_LOW_MEMORY
    display_param.RotateAngle = LCD_ANGLE_0;
    if((display_param.disp_rect.dy >=display_param.disp_rect.dx 
        && media_info_ptr->video_info.height>=media_info_ptr->video_info.width) 
        ||(display_param.disp_rect.dy <display_param.disp_rect.dx 
        && media_info_ptr->video_info.height<media_info_ptr->video_info.width) ) //屏幕与片源比例类似
    {
        //SCI_TRACE_MID:"[MMISETVWP]:GetDisplayParam KEEP WIDTH AND HEIGHT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_1693_112_2_18_2_52_17_364,(uint8*)"");
    }
    else
    {
        //SCI_TRACE_MID:"[MMISETVWP]:GetDisplayParam CHANGE dy=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_1697_112_2_18_2_52_17_365,(uint8*)"d",display_param.disp_rect.dy);
        display_param.disp_rect.dy = display_param.disp_rect.dx + display_param.disp_rect.dy;
        display_param.disp_rect.dx = display_param.disp_rect.dy - display_param.disp_rect.dx;
        display_param.disp_rect.dy = display_param.disp_rect.dy - display_param.disp_rect.dx;
    }
//#else
#if 0
    if((display_param.disp_rect.dy >=display_param.disp_rect.dx 
        && media_info_ptr->video_info.height>=media_info_ptr->video_info.width) 
        ||(display_param.disp_rect.dy <display_param.disp_rect.dx 
        && media_info_ptr->video_info.height<media_info_ptr->video_info.width) ) //屏幕与片源比例类似
    {
        //SCI_TRACE_LOW:"[MMISETVWP]:GetDisplayParam LCD_ANGLE_0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_1709_112_2_18_2_52_17_366,(uint8*)"");
        display_param.RotateAngle = LCD_ANGLE_0;
    }
    else
    {
        //SCI_TRACE_LOW:"[MMISETVWP]:GetDisplayParam LCD_ANGLE_90"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_1714_112_2_18_2_52_17_367,(uint8*)"");
        display_param.RotateAngle = LCD_ANGLE_90;
    }
#endif 
    SCI_MEMCPY(&display_param.target_rect,&display_param.disp_rect,sizeof(DPLAYER_DISP_RECT_T));

    return display_param;
}

/*****************************************************************************/
// 	Description : PlayNextVideoWallpaper
//	Global resource dependence : 
//  Author:aoke.hu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN PlayNextVideoWallpaper(void)
{
#ifdef VIDEO_PLAYER_SUPPORT
    VP_PLAY_PARAM_T         play_param  = {0};
    BOOLEAN                 ret_value = FALSE;
    MMISET_VIDEO_LIST_T*    video_item_ptr = PNULL;
    DPLAYER_MEDIA_INFO_T    media_info = {0};
    VP_DISPLAY_PARAM_T      display_param = {0};

    if(MMIAPIUDISK_UdiskIsRun())
    {
        //SCI_TRACE_LOW:"[MMISETVWP]:PlayNextVideoWallpaper() UdiskIsRun!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_1738_112_2_18_2_52_17_368,(uint8*)"");
    }
    else
    {
        video_item_ptr = GetNextVideo(&media_info);
    }

    // check the exist of the file name.
    if(PNULL != video_item_ptr)
    {
        play_param.dual_sys = MN_DUAL_SYS_1;
        play_param.full_path_len = video_item_ptr->full_path_len;
        play_param.full_path_ptr = video_item_ptr->full_path_name;
        play_param.is_random_play = FALSE;
        play_param.is_scal_up = TRUE;
        play_param.mode = VP_REPEAT_PLAY_MODE_CLOSE;//VP_REPEAT_PLAY_MODE_ONE;
        play_param.play_type = VP_PLAY_FROM_FILE;
        play_param.url_ptr = PNULL;
        play_param.video_buf_info.type = 0;
        play_param.video_buf_info.video_buf_len = 0;
        play_param.video_buf_info.video_buf_ptr = PNULL;
        display_param = GetDisplayParam(&media_info);
        SCI_MEMCPY(&play_param.display_param,&display_param,sizeof(VP_DISPLAY_PARAM_T));
        play_param.display_param.videoSupport = 1;
        if(GetVideoAudioSetting())
        {
            play_param.display_param.audioSupport = 1;
            //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_VIDEO_WALLPAPER, FALSE);
            MMIAPISET_StopAllRing(FALSE); 
        }
        else
        {
            play_param.display_param.audioSupport = 0;
        }

        play_param.vol = MMIAPISET_GetMultimVolume();//MMIAUDIO_GetCurVolume();
        if(!s_osd)
        {
            play_param.is_cr_video_layer = TRUE;
        }
        play_param.is_audio_coexist = TRUE;
        //SCI_TRACE_LOW:"[MMISETVWP]:PlayNextVideoWallpaper() MMIAPIVP_Play!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_1779_112_2_18_2_52_18_369,(uint8*)"");
        if(!MMIAPIVP_Play(&play_param, HandleDynWallpaperPlayMsg, 0))
        {
            //SCI_TRACE_LOW:"[MMISETVWP]:PlayNextVideoWallpaper() MMIAPIVP_Play fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_1782_112_2_18_2_52_18_370,(uint8*)"");
            //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_VIDEO_WALLPAPER, TRUE);
            ret_value = FALSE;
        }
        else
        {           
            //SCI_TRACE_LOW:"[MMISETVWP]:PlayNextVideoWallpaper() MMIAPIVP_Play TRUE!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_1788_112_2_18_2_52_18_371,(uint8*)"");
            if(GetVideoAudioSetting())
            {
                MMIDEFAULT_EnableKeyRing("VIDEO WALLPAPER", FALSE);
                MMIDEFAULT_EnableTpRing("VIDEO WALLPAPER", FALSE);
            }
            s_is_video_layer_enable = TRUE ;
            ret_value = TRUE;
            if(s_osd)
            {
                UILAYER_EnableOsdLayer(TRUE);
            }
            else
            {
                #if 0 //NEWMS00177259
                 MMIAPIVP_CreateVideoLayer();
                #endif
            }    
            //开启主层透明色，并清除
            //            UILAYER_SetLayerColorKey(MMITHEME_GetDefaultLcdDev(), TRUE, UILAYER_TRANSPARENT_COLOR);
            //            UILAYER_Clear(MMITHEME_GetDefaultLcdDev());
            //            UILAYER_RemoveMainLayer();
            //            UILAYER_SetDirectDraw(TRUE);
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[MMISETVWP]:PlayNextVideoWallpaper() not found!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_1815_112_2_18_2_52_18_372,(uint8*)"");
        ret_value = FALSE ;
    }
    return ret_value;
#else
    return FALSE ;
#endif
    
}

/*****************************************************************************/
// Description : 切换到下一个VIDEO WALLPAPER 
// Global resource dependence : 
// Author:aoke.hu
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPISETVWP_SwitchToNext(void)
{
    if (MMIAPISETVWP_IsOpen() && !CheckAllVideoIsInValid()) 
    {
        //MMIAUDIO_PauseBgPlay(MMIBGPLAY_MODULE_VIDEO_WALLPAPER);
        
        BLKMEM_GarbageClean();
        if(SwitchToNextWallpaper())
        {
             IGUICTRL_T* ctrl_ptr=PNULL;
             MMI_HANDLE_T ctrl_handle=PNULL;
             ctrl_ptr = MMK_GetCtrlPtrByWin(MAIN_IDLE_WIN_ID, MMIIDLE_WALLPAPER_ANIM_CTRL_ID);
            if(ctrl_ptr != PNULL)
            {
                ctrl_handle = MMK_GetCtrlHandleByWin(MAIN_IDLE_WIN_ID, MMIIDLE_WALLPAPER_ANIM_CTRL_ID);
                MMK_DestroyControl(ctrl_handle);
            }
         #if 0//NEWMS00177259
            UILAYER_RemoveMainLayer();
         #endif   
        } 
        
    } 
}

/*****************************************************************************/
// 	Description : SwitchToNextWallpaper
//	Global resource dependence : 
//  Author:aoke.hu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN SwitchToNextWallpaper(void)
{
    BOOLEAN ret_value = TRUE ;
    
    MMIAPIVP_ExitPlay();
    ret_value = PlayNextVideoWallpaper();
    
    return ret_value ;
    
}

/*****************************************************************************/
// Description : 停止播放VIDEO WALLPAPER 
// Global resource dependence : 
// Author:aoke.hu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_StopVideoWallpaper(void)
{
    if(s_is_video_layer_enable)
    {
        StopVideoWallpaper() ;
#if 0//NEWMS00177259
        UILAYER_RestoreMainLayer();
#endif
    }

    return TRUE;
}

/*****************************************************************************/
// Description : 停止播放VIDEO WALLPAPER 
// Global resource dependence : 
// Author:aoke.hu
// Note: 
/*****************************************************************************/
LOCAL void StopVideoWallpaper(void)
{
     IGUICTRL_T* ctrl_ptr=PNULL;
    // MMI_HANDLE_T ctrl_handle=PNULL;
    GUIANIM_DATA_INFO_T             data_info           = {0};  
    GUIANIM_CTRL_INFO_T             control_info        = {0};
    GUIANIM_DISPLAY_INFO_T          display_info        = {0};
    BOOLEAN         is_save_bitmap=TRUE;
    
    MMIAPIVP_ExitPlay();
    s_is_video_layer_enable = FALSE ;
    if(s_osd)
    {
        UILAYER_EnableOsdLayer(FALSE);
    }
    else
    {
        MMIAPIVP_DestroyVideoLayer();
    } 
    if(GetVideoAudioSetting())
    {
        MMIDEFAULT_EnableKeyRing("VIDEO WALLPAPER", TRUE);
        MMIDEFAULT_EnableTpRing("VIDEO WALLPAPER",TRUE);
    }

    ctrl_ptr = MMK_GetCtrlPtrByWin(MAIN_IDLE_WIN_ID, MMIIDLE_WALLPAPER_ANIM_CTRL_ID);
    if(ctrl_ptr == PNULL)
    {
        MMIAPIIDLE_CreatAnimCtrl(MAIN_IDLE_WIN_ID, MMIIDLE_WALLPAPER_ANIM_CTRL_ID, FALSE);
    }
        //set parameter
    control_info.is_wallpaper_ctrl = TRUE;
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = MMIIDLE_WALLPAPER_ANIM_CTRL_ID;

    display_info.is_syn_decode = (BOOLEAN)(!is_save_bitmap);
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_save_wallpaper = is_save_bitmap;
    display_info.bg.bg_type = GUI_BG_COLOR;
    display_info.bg.color   = 0xffff;
    
    data_info.img_id = MMIAPISET_GetWallpaperByIndex(MMIAPISET_GetDefaultWallpaperIDByStyle());
    GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
    
}

/*****************************************************************************/
// Description : 视频buffer层是否已开启
// Global resource dependence : 
// Author:aoke.hu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_IsVideoLayerEnable(void)
{
    return s_is_video_layer_enable;
}

/*****************************************************************************/
// Description : 是否存在视频墙纸声音
// Global resource dependence : 
// Author:aoke.hu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_IsAudioUsing(void)
{
    return s_is_video_layer_enable && GetVideoAudioSetting();
}

/*****************************************************************************/
// Description : VIDEO WALLPAPER 是否开启
// Global resource dependence : 
// Author:aoke.hu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_IsOpen(void)
{
    MMISET_WALLPAPER_SET_T      wallpaper_info = {0};
    
    MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
    if (MMISET_WALLPAPER_VIDEO == wallpaper_info.wallpaper_type)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// Description : 
// Global resource dependence : 
// Author:aoke.hu
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPISETVWP_LcdSwitch(void)
{
    if(s_is_video_layer_enable && !s_osd)
    {
        MMIAPIVP_DestroyVideoLayer();
        MMIAPIVP_CreateVideoLayer();
        MMIAPIVP_CurrentVideoLcdSwitch();
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISETVWP_HandleVideoWallPaper(MMI_WIN_ID_T win_id, 
                                                      MMI_MESSAGE_ID_E msg_id, 
                                                      DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    static BOOLEAN is_idle_openwin = FALSE;
	SCI_TRACE_LOW("MMIAPISETVWP_HandleVideoWallPaper msg_id = 0x%x",msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(MAIN_IDLE_WIN_ID == win_id)
        {
            is_idle_openwin = TRUE;
        }
        else
        {
            MMIAPISETVWP_SwitchToNext(); 
        }
        break;

    case MSG_FULL_PAINT:
		SCI_TRACE_LOW("MMIAPISETVWP_HandleVideoWallPaper is_idle_openwin = 0x%x",is_idle_openwin);
        if(is_idle_openwin && MMK_IsFocusWin(win_id))
        {
            is_idle_openwin = FALSE;
#ifdef  VIDEOTHUMBNAIL_SUPPORT            
        if( MMIAPISETVWP_IsOpen())
        {
            MMIAPISETVWP_DisplayThumbnail();
        }
#endif        
            MMIAPISETVWP_SwitchToNext(); 
        }
        break;

#ifdef FLIP_PHONE_SUPPORT        
    case MSG_KEYDOWN_FLIP:
#endif
    case MSG_BACKLIGHT_TURN_OFF:
    case MSG_LOSE_FOCUS:
    case MSG_CLOSE_WINDOW:
        MMIAPISETVWP_StopVideoWallpaper();
        break;
        
    
#ifdef FLIP_PHONE_SUPPORT        
    case MSG_KEYUP_FLIP:
#endif
	case MSG_BACKLIGHT_TURN_ON:
    case MSG_GET_FOCUS:
        if(is_idle_openwin)
        {
            is_idle_openwin = FALSE;
        }
        if( MMIAPISETVWP_IsOpen())
        {
#ifdef  VIDEOTHUMBNAIL_SUPPORT  
            MMIAPISETVWP_DisplayThumbnail();
#else     
            GUI_RECT_T bg_rect=MMITHEME_GetFullScreenRect();
            MMI_IMAGE_ID_T bg_img_id = MMIAPISET_GetWallpaperByIndex(MMIAPISET_GetDefaultWallpaperIDByStyle());
            
            GUIRES_DisplayImg(PNULL,
            &bg_rect,
            &bg_rect,
            MAIN_IDLE_WIN_ID,
            bg_img_id,
            MMITHEME_GetDefaultLcdDev());
#endif   
            if(!GUIMAINMENU_IsMoveState(MAINMENUWIN_MENU_CTRL_ID) && !CheckAllVideoIsInValid())
            {
                MMIAPISETVWP_SwitchToNext();  
            }

        }
        break;

    case MSG_LCD_SWITCH:
        //SCI_TRACE_LOW:"[MMIAPISETWP]: LCD SWITCH"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_2038_112_2_18_2_52_18_373,(uint8*)"");
        MMIAPISETVWP_LcdSwitch();
        break;

    case MSG_APP_UPSIDE:
    case MSG_APP_DOWNSIDE:
        if(MMIAPISETVWP_IsAudioUsing()
            && !MMIAPICOM_IsPanelOpened())
        {
            OpenVolumePanel(win_id);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}

/*****************************************************************************/
// Description : 实现两种视频墙纸事项方式的切换
// Global resource dependence : 
// Author:aoke.hu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_Test(void)
{
    s_osd = !s_osd;
    if(s_osd)
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_OK);
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(STXT_CANCEL);
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : 音量调节
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void AdjustVolCallBack(MMICOM_PANEL_CALLBACK_T *para_ptr) 
{
    if(PNULL != para_ptr)
    {
        //SCI_TRACE_LOW:"[MMIAPISETWP] AdjustVolCallBack volue = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_VIDEOWALLPAPER_2089_112_2_18_2_52_18_374,(uint8*)"d", para_ptr->cur_value);
        MMIAPISET_SetMultimVolume(para_ptr->cur_value);
        MMIAPIVP_FSMUpdateCurrentVol(VP_GetCurrentFSMHandle(), para_ptr->cur_value);
    }  
}

/*****************************************************************************/
//  Description : 打开调节窗口
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void OpenVolumePanel(MMI_WIN_ID_T win_id)
{  
    MMICOM_PANEL_INFO_T panel_info = {0};

    panel_info.x = -1;
    panel_info.y = -1;
    panel_info.cur_value = MMIAPISET_GetMultimVolume();
    panel_info.last_value = MMIAPISET_GetLastMultimVolume();
    panel_info.min_value = MMISET_VOL_ZERO;   
    panel_info.max_value = MMISET_VOL_MAX;
    panel_info.panel_type = MMICOM_PANEL_VOLUME;
    panel_info.set_callback = AdjustVolCallBack;    
    
    MMIAPICOM_OpenPanelChildWin(win_id,&panel_info);
     
}

/*****************************************************************************/
//  Description : 加入视频墙纸列表
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_AddToVideoWallpaperList(wchar *full_name_ptr, 
                                                    uint16 full_name_len, 
                                                    uint32 file_size)
{
    BOOLEAN result = FALSE;
    MMISET_VIDEO_LIST_T video_item = {0};
    DPLAYER_MEDIA_INFO_T media_info = {0};

    if(PNULL == full_name_ptr || 0 == full_name_len)
    {
        return result;
    }

    if(MMISET_VIDEO_WALLPAPER_MAX_NUM > s_video_wallpaper_info.total_num)
    {
        SCI_MEMSET(&video_item,0,sizeof(MMISET_VIDEO_LIST_T));
        MMIAPICOM_Wstrncpy(video_item.full_path_name,full_name_ptr,full_name_len);
        video_item.full_path_len = full_name_len;
        video_item.file_size = file_size;

        if(!MMIAPIVP_GetInfoFromFullPathName(video_item.full_path_name,video_item.full_path_len,&media_info))
        {
            result = FALSE; 
        } 
        
#ifndef PDA_VIDEOPLAYER_WALLPAPER_SUPPORT_LOW_MEMORY
        if(media_info.video_info.height * media_info.video_info.width <= MMISET_VIDEO_MAX_VIDEO_SIZE
                    && 1 == media_info.video_info.video_support)
#else
        if(CheckVideoIsLowMemorySupport(&media_info))
#endif
        {
            AddItemToVideoList(&video_item);
            VideoListWriteFile();
            result = TRUE;
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_VP_VIDEO_WALLPAPER_UNSUPPORTED);
            result = FALSE; 
        }
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_SET_SETAS_WALLPAPER_VIDEO_LIST_FULL);
        result = FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description : 暂停视频播放
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_Pause(void)
{
    if(s_is_video_layer_enable)
    {
        return MMIAPIVP_Pause(0);
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : 恢复视频播放
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_Resume(void)
{
    if(MMIAPIVP_Resume(0))
    {
        return TRUE;
    }
    else
    {
        MMIAPISETVWP_SwitchToNext();
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_SetBgVideoAudioSupport(BOOLEAN is_audio_support)
{
    if(GetVideoAudioSetting())
    {
        MMIAPIVP_SetBgVideoAudioSupport(is_audio_support);
    }
    else
    {
        MMIAPIVP_SetBgVideoAudioSupport(FALSE);
    }
    return TRUE;
}


#endif //MMI_VIDEOWALLPAPER_SUPPORT


/*Edit by script, ignore 4 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
