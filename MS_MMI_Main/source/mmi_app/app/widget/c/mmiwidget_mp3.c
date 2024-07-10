/*****************************************************************************
** File Name:      mmiwidget_mp3.c                                        *
** Author:                                                                   *
** Date:           10/23/2009                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/23/2009        bin.ji              Creat
******************************************************************************/

#define _MMIWIDGET_MP3_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_widget_trc.h"
#if defined MMI_GRID_MP3
//#define MMIWIDGET_MP3_SE 
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_utility.h"
#include "guianim.h"
#include "mmiwidget_image.h"
#include "mmiwidget.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "mmimp3_export.h"
#include "guires.h"
#include "mmitheme_widget.h"
#include "mmi_appmsg.h"
#include "mmiwidget_position.h"
#include "mmimp3_text.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
#include "mmi_applet_table.h"
#include "mmiset_export.h"
#include "mmipub.h"
#include "mmidisplay_data.h"

#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif

#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_idlewin.h"
#endif
#include "mmicc_export.h"
#include "mmiudisk_export.h"
#include "mmieng_export.h"
#include "mmisd_export.h"

#if defined MMIWIDGET_MP3_SE 
#include "graphics_3d_rotate.h"
#include "mmiwidget_special_effect.h"
#include "mmiwidget_mp3_se.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/



#define   MMIWIDGET_MP3_PROGRESS_TIMEOUT          300

#define   MMIWIDGET_MP3_FORWARD_BACKWARD_PERIOD   1000

#define   MMIWIDGET_MP3_ADJUST_VOL_PERIOD          500

#define   MMIWIDGET_MP3_SWAP_STYLE_PERIOD          2000

#define   MMIWIDGET_MP3_PROGRESS_FILL_COLOR       0x07e0

#ifndef WIN32
#if (defined MAINLCD_SIZE_240X400||defined MAINLCD_SIZE_240X320)
#define   MMIWIDGET_MP3_PROGRESS_MARGIN            8
#else
#define   MMIWIDGET_MP3_PROGRESS_MARGIN            3
#endif
#endif
//点中区域的标识
typedef enum
{
    MMIWIDGET_MP3_HIT_NONE,         //无
    MMIWIDGET_MP3_HIT_CHANGE_PANEL,        //却换风格
    MMIWIDGET_MP3_HIT_PREV,         //前一首
    MMIWIDGET_MP3_HIT_NEXT,         //下一首    
    MMIWIDGET_MP3_HIT_PAUSE_ON,        //暂停按下
    MMIWIDGET_MP3_HIT_PAUSE_OFF,        //暂停激活    
    MMIWIDGET_MP3_HIT_PLAY_ON,         //播放
    MMIWIDGET_MP3_HIT_PLAY_OFF,         //播放  
    MMIWIDGET_MP3_HIT_ENTER_PLAYER,    
    MMIWIDGET_MP3_LONGHIT_PREV,         //快退
    MMIWIDGET_MP3_LONGHIT_NEXT,         //快进
    MMIWIDGET_MP3_HIT_MAX
}MMIWIDGET_MP3_HIT_TYPE_E;

//界面类型
typedef enum
{
    MMIWIDGET_MP3_PANEL_NONE,         //无
    MMIWIDGET_MP3_PANEL_PAUSE_STYLE,         
    MMIWIDGET_MP3_PANEL_PALY_STYLE,         
    MMIWIDGET_MP3_PANEL_VOL_STYLE,     
    MMIWIDGET_MP3_PANEL_MAX
}MMIWIDGET_MP3_PANEL_STYLE_E;


typedef struct 
{
    wchar path[MMIFILE_FULL_PATH_MAX_LEN + 1];
    uint32 path_length;
    MMIMP3_ALBUM_INFO_T     album_info;
    int32 valid_flag ;		//album pic buffer flag
}MMIWIDGET_MP3_INFO_T;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

//点中区域的标识
LOCAL uint32 s_widget_mp3_hit_type = MMIWIDGET_MP3_HIT_NONE;
LOCAL uint8  s_widget_mp3_progess_timer_id =0;
LOCAL uint8  s_widget_mp3_forward_backward_timer_id = 0;


LOCAL BOOLEAN s_widget_mp3_is_forward = FALSE;

LOCAL uint32 s_widget_mp3_panel_style = MMIWIDGET_MP3_PANEL_PALY_STYLE;

LOCAL GUIIMG_BITMAP_T s_cover_img = {0}; 

#if defined MMIWIDGET_MP3_SE 

static const  MMI_IMAGE_ID_T s_image_array[] = 
                                {
                                IMAGE_WIDGET_MP3_SE_SLIDER_LIGHT    ,
                                IMAGE_WIDGET_MP3_SE_SLIDER_BG   ,
                                //IMAGE_WIDGET_MP3_SE_SLIDER_POINT    ,
                                IMAGE_WIDGET_MP3_SE_POFU    ,
                                IMAGE_WIDGET_MP3_SE_POFD    ,
                                IMAGE_WIDGET_MP3_SE_POBU    ,
                                IMAGE_WIDGET_MP3_SE_POBD    ,
                                IMAGE_WIDGET_MP3_SE_PIB     ,
                                IMAGE_WIDGET_MP3_SE_PIF_PLAY    ,
                                IMAGE_WIDGET_MP3_SE_PIF_PAUSE   
                                //IMAGE_WIDGET_MP3_BG
                                };
                                
 static const  MMI_IMAGE_ID_T s_loading_image_array[] = 
                                {
                                IMAGE_WIDGET_MP3_SE_LOADING_BG    ,
                                IMAGE_WIDGET_MP3_SE_LOADING_ROT   ,
                                IMAGE_WIDGET_MP3_SE_LOADING_ROT   
                                };
                                
//这个数组的顺序和大小必须和 TURNTABLE_RES_T 保持一致
LOCAL MMI_TURNTABLE_RES_T s_mmi_resource_array = {0};  
LOCAL TURNTABLE_RES_T s_resource_array = {0};

LOCAL TURNTABLE_LOAD_RES_T s_loading_resource_array = {0};
LOCAL MMI_TURNTABLE_LOAD_RES_T s_mmi_loading_resource_array = {0};  

LOCAL MMIWIDGET_SPECIAL_EFFECT_T s_widget_se = {0};
typedef struct 
{
    MMIWIDGET_MP3_SE_ANIM_TYPE_E anim_type ;
    int32  switch_type ;                   // -1:back; 1: forward
}MMIWIDGET_MP3_SE_ANIM_PARA_T;
#define MP3_INFO_ARRAY_NUM (MP3_SE_DISPLAY_NUM + 1)

LOCAL MMIWIDGET_MP3_INFO_T  *sp_mp3_info[MP3_INFO_ARRAY_NUM]; 
LOCAL MMIWIDGET_MP3_SE_ANIM_PARA_T s_anim_para = {0};
LOCAL int32 CreateImageBuffer(MMI_WIN_ID_T    win_id, MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se);
LOCAL int32 DestroyImageBuffer(MMI_WIN_ID_T    win_id, MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se);
LOCAL int32 Display(MMI_WIN_ID_T  win_id, MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se);
LOCAL int32 UpdateImageBuffer(MMI_WIN_ID_T  win_id, MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se);
LOCAL int32 IsPlayAnimation(MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se);

LOCAL BOOLEAN CreatePlayList(MMIWIDGET_MP3_INFO_T *pp_mp3_info[]);
LOCAL BOOLEAN DestroyPlayList(MMIWIDGET_MP3_INFO_T *p_mp3_info[]);
LOCAL int32 UpdatePlayList(MMIWIDGET_MP3_INFO_T *p_mp3_info[]);
LOCAL int32 SwitchPlayList(MMIWIDGET_MP3_INFO_T *p_mp3_info[], int32 offset);
LOCAL BOOLEAN UpdatePlayListItem(MMIWIDGET_MP3_INFO_T *p_mp3_info, int32 i);
LOCAL BOOLEAN GetPlayListItemPath(wchar *p_path, uint32 *p_path_length, int32 offset);
LOCAL BOOLEAN DecodeAlbumPicture(MMIWIDGET_MP3_INFO_T  *p_mp3_info[], int32 index);
LOCAL int32 CreateAlbumPictureResource(MMI_WIN_ID_T  win_id, MMIWIDGET_MP3_INFO_T  *p_mp3_info, 
                                        int32 index, GUIIMG_BITMAP_T *p_img_bmp);
LOCAL int32 DestroyAlbumPictureResource(MMIWIDGET_MP3_INFO_T  *p_mp3_info,int32 index);
LOCAL int32 UpdateAlbumPictureResource(MMI_WIN_ID_T  win_id, MMIWIDGET_MP3_INFO_T  *p_mp3_info,
                                       int32 index, GUIIMG_BITMAP_T *p_img_bmp);

#endif


/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
              
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : Handle WidgetMp3 Win Msg
//    Global resource dependence : 
//  Author:bin.ji
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetMp3WinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

/*****************************************************************************/
//  Description : 获得mp3 widget上的文本风格
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void GetMp3WidgetTextStyle(
                            GUISTR_STYLE_T *style_ptr
                            );

/*****************************************************************************/
//  Description : display mp3 name
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void DisplayMp3Name(
                          MMI_WIN_ID_T     win_id,
                          GUI_LCD_DEV_INFO *lcd_dev_ptr,
                          GUI_RECT_T *rect_ptr                          
                          );

#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
/*****************************************************************************/
// Description : create image viewer image animation control.
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL void CreateImgAnimCtrl(MMI_WIN_ID_T  win_id);

/*****************************************************************************/
// Description : 显示网页加载完成动画的初始/完成状态
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetImgAnimData(MMIMP3_ALBUM_INFO_T *album_info );

/*****************************************************************************/
//  Description : display mp3 album info
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void DisplayMp3AlbumInfo(MMI_WIN_ID_T     win_id,
                                GUI_LCD_DEV_INFO *lcd_dev_ptr,
                                GUI_RECT_T *rect_ptr,
                                MMIMP3_ALBUM_INFO_T *album_info_ptr
                                                        );
#endif

/*****************************************************************************/
//  Description : display mp3 name
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void DisplayMp3Panel(
                           MMI_WIN_ID_T     win_id,
                           MMIWIDGET_MP3_PANEL_STYLE_E panel_style
                           );
#ifdef BAR_PHONE_WIDGET 
LOCAL void DisplayMp3Softkey(
                           MMI_WIN_ID_T     win_id,
                           BOOLEAN          is_update
                           );

#endif

/*****************************************************************************/
//     Description : Handle WidgetMp3 Win Msg
//    Global resource dependence : 
//  Author:bin.ji
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTpPressDown(MMI_WIN_ID_T win_id, DPARAM param);

LOCAL  void DisplayWigetMp3TimeInfo(MMI_WIN_ID_T win_id);

LOCAL  void DisplayWigetMp3Progress(MMI_WIN_ID_T win_id);

LOCAL BOOLEAN StartWidgetMp3Timer(MMI_WIN_ID_T win_id);

LOCAL BOOLEAN StopWidgetMp3Timer(void);

LOCAL  BOOLEAN HandleProgressBarTpKey(int16 tp_x, GUI_RECT_T progress_bar_rect);
                                             
LOCAL BOOLEAN GetMp3CurrentOffsetByTp(BOOLEAN  *is_forward_ptr,        //in/out:
                                      uint32    tp_x_offset,
                                      uint32   *mp3_total_offset_ptr,  //in/out:从Mp3 0位置算起
                                      uint32   *mp3_bitrate_ptr
                                      );
                                      
LOCAL  BOOLEAN ForwardOrBackwardMp3(BOOLEAN  is_tp_progress, //是否是Tp进度条
                                    BOOLEAN  is_forward,
                                    uint32   mp3_offset,     //按键:Mp3 pause位置算起,Tp进度条:Mp3 0位置算起
                                    MMIMP3_AUDIO_STATE_E cur_state
                                    );
                                    
LOCAL MMI_RESULT_E HandleWidgetMp3TpLongKey(MMI_WIN_ID_T win_id, DPARAM  param);
                                           
LOCAL void StartForwardOrBackwardTimer(MMI_WIN_ID_T win_id,  BOOLEAN  is_forward );

LOCAL void StopForwardOrBackwardTimer(void);

/*****************************************************************************/
//  Description : Handle WidgetMp3 pre or next btn
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void HandleWidgetMp3PreNextBtn(MMI_WIN_ID_T       win_id,
                                     BOOLEAN            is_pre);

/*****************************************************************************/
//     Description : reset child win, just for lose focus to init button status.
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/
LOCAL void ResetWidgetMp3(void);

/*****************************************************************************/
//  Description : check mp3 can run or not
//  Global resource dependence : 
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckMp3RunCondition(void);

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/

PUBLIC const MMIWIDGET_ITEM_INFO_T g_mp3_widget =
{
    MMIWIDGET_MP3_ID, 
    HandleWidgetMp3WinMsg, 
    PNULL,
    ResetWidgetMp3,
    PNULL,
    WIDGET_MP3_WIN_ID, 
    IMAGE_WIDGET_MP3_SHORTCUT, 
    IMAGE_WIDGET_MP3_BG,
    TXT_WIDGET_MP3,
    50,//    75, 
    50,//    50
    FALSE
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : reset child win, just for lose focus to init button status.
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/
LOCAL void ResetWidgetMp3()
{
    s_widget_mp3_hit_type     = MMIWIDGET_MP3_HIT_NONE;  
   // s_widget_mp3_hit_vol_type = MMIWIDGET_MP3_HIT_VOL_NONE;
    s_widget_mp3_panel_style  = MMIWIDGET_MP3_PANEL_PALY_STYLE;      
}

/*****************************************************************************/
//     Description : Handle WidgetMp3 Win Msg
//    Global resource dependence : 
//  Author:bin.ji
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTpPressDown(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};
    GUI_RECT_T button_prev_rect = MMIWIDGET_MP3_LBUTTON_RECT;
    GUI_RECT_T button_next_rect = MMIWIDGET_MP3_RBUTTON_RECT;    
    GUI_RECT_T button_play_rect = MMIWIDGET_MP3_ENTRYPLAY_RECT;
    GUI_RECT_T button_progress_rect = MMIWIDGET_MP3_PROGRESS_RECT;

#ifndef WIN32
    button_progress_rect.top -= MMIWIDGET_MP3_PROGRESS_MARGIN;
    button_progress_rect.bottom += MMIWIDGET_MP3_PROGRESS_MARGIN;
#endif
	point.x = MMK_GET_TP_X(param);
	point.y = MMK_GET_TP_Y(param);
    point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);
    
    if(MMIWIDGET_MP3_PANEL_PALY_STYLE == s_widget_mp3_panel_style)
    {
	    if (GUI_PointIsInRect(point, button_prev_rect))
	    {
	        //显示按钮
	        s_widget_mp3_hit_type = MMIWIDGET_MP3_HIT_PREV;
	    }

	    else if (GUI_PointIsInRect(point, button_next_rect))
	    {
	        s_widget_mp3_hit_type = MMIWIDGET_MP3_HIT_NEXT;
	    }
	    else if(GUI_PointIsInRect(point, button_play_rect))
        {
            switch(MMIAPIMP3_GetPlayerState())
            {
            case MMIMP3_PLAYER_STATE_NONE:
            case MMIMP3_PLAYER_SWITCH_PAUSED:
                s_widget_mp3_hit_type = MMIWIDGET_MP3_HIT_PLAY_ON;
                break;
            case MMIMP3_PLAYER_MUSIC_SWITCH:
                s_widget_mp3_hit_type = MMIWIDGET_MP3_HIT_PAUSE_ON;
                break;
            case MMIMP3_PLAYER_AUDIO_ACTIVE:
                switch(MMIAPIMP3_GetAudioState())
                {
                case MMIMP3_AUDIO_PLAYING:
                    s_widget_mp3_hit_type = MMIWIDGET_MP3_HIT_PAUSE_ON;
                    break;

                case MMIMP3_AUDIO_PAUSED:
                case MMIMP3_AUDIO_STOPPED:
                default:
                    s_widget_mp3_hit_type = MMIWIDGET_MP3_HIT_PLAY_ON;
                    break;
                }
                break;
                default:
                    break;
            }
        }

	    else if(GUI_PointIsInRect(point, button_progress_rect))
	    {
		     recode = HandleProgressBarTpKey(point.x,button_progress_rect);
	    }
	    else
	    {
	        s_widget_mp3_hit_type = MMIWIDGET_MP3_HIT_NONE;
	        recode = MMI_RESULT_FALSE;
	    }
     }
    return recode;
}

/*****************************************************************************/
//  Description : Handle WidgetMp3 pre btn after check
//  Global resource dependence : 
//  Author:paul.huang
//  Note: 
/*****************************************************************************/
LOCAL void HandlePreBtnAfterCheck(void)
{
    MMI_WIN_ID_T win_id = WIDGET_MP3_WIN_ID;
    
    if(MMIAPIMP3_IsLoadCurPlayInfo())
    {
        MMIAPIMP3_GoToPreOrNextMp3(TRUE);   
#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
        MMIAPIMP3_UpdateAllAlbumInfo(TRUE);
#endif        
        //SetImgAnimData(win_id);
        StartWidgetMp3Timer(win_id);

        #ifdef MMIAP_F_RELEASE_BG_AUDIO
        MMIAPIAP_ReleaseAudioHandle();
        #endif
    }
    else
    {
        //打开mp3播放器
        MMIAPIMP3_OpenMp3Player();
    }
}

/*****************************************************************************/
//  Description : Handle WidgetMp3 next btn after check
//  Global resource dependence : 
//  Author:paul.huang
//  Note: 
/*****************************************************************************/
LOCAL void HandleNextBtnAfterCheck(void)
{
    MMI_WIN_ID_T win_id = WIDGET_MP3_WIN_ID;
    
    if(MMIAPIMP3_IsLoadCurPlayInfo())
    {
        MMIAPIMP3_GoToPreOrNextMp3(FALSE); 
#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
        MMIAPIMP3_UpdateAllAlbumInfo(FALSE);
#endif        
        //SetImgAnimData(win_id);
        StartWidgetMp3Timer(win_id);

        #ifdef MMIAP_F_RELEASE_BG_AUDIO
        MMIAPIAP_ReleaseAudioHandle();
        #endif
    }
    else
    {
        //打开mp3播放器
        MMIAPIMP3_OpenMp3Player();
    }
}

/*****************************************************************************/
//  Description : Handle WidgetMp3 pre or next btn
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
LOCAL void HandleWidgetMp3PreNextBtn(MMI_WIN_ID_T       win_id,
                                     BOOLEAN            is_pre)
{   
#if defined MMIWIDGET_MP3_SE 
    if(IsPlayAnimation(&s_widget_se))
    {
        return;
    }
#endif  
    MMIWIDGET_DEBUG(LOG_STR("KEY_PRESS_UP time = %d"), SCI_GetTickCount());
    if(CheckMp3RunCondition())
    {    
        if(is_pre)
        {
            MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_AUDIO_PLAYER, HandlePreBtnAfterCheck);
        }
        else
        {
            MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_AUDIO_PLAYER, HandleNextBtnAfterCheck);
        }
    }
}

/*****************************************************************************/
//  Description : Handle WidgetMp3 play btn after check
//  Global resource dependence : 
//  Author:paul.huang
//  Note: 
/*****************************************************************************/
LOCAL void HandlePlayPauseBtnAfterCheck(void)
{
    MMI_WIN_ID_T win_id = WIDGET_MP3_WIN_ID;
    uint32 offset = 0;
    
    switch(MMIAPIMP3_GetPlayerState())
    {
    case MMIMP3_PLAYER_STATE_NONE:
        if(MMIAPIMP3_IsLoadCurPlayInfo())
        {

            MMI_APPLET_START_T start ={0};
            
            start.guid = SPRD_MP3_APPLET_ID;
            start.state = MMI_APPLET_STATE_BG_HIDE_WIN|MMI_APPLET_STATE_START_BG|MMI_APPLET_STATE_TASK_MANAGER;
            
            MMK_StartApplet( &start );

            offset = MMIAPIAP_GetCurPlayOffset(MMIMP3_ZERO_OFFSET);
            if (MMIAPIMP3_PlayAudioPlayer(offset))
            {    
                StartWidgetMp3Timer(win_id);
            }
            else
            {
                //MMIAUDIO_ResumeBgPlay(MMIBGPLAY_MODULE_MP3);
            }

#ifdef PDA_UI_DROPDOWN_WIN
            MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_RUNNING_PLAYER);
#endif
        }
        else
        {
            //打开mp3播放器
            MMIAPIMP3_OpenMp3Player();
        }
        break;
        
    case MMIMP3_PLAYER_AUDIO_ACTIVE:
        {
            switch(MMIAPIMP3_GetAudioState())
            { 
            case MMIMP3_AUDIO_PLAYING:
                MMIAPIMP3_PauseMp3(FALSE);
                StopWidgetMp3Timer();   
                s_widget_mp3_hit_type = MMIWIDGET_MP3_HIT_PLAY_OFF; 
                break;
                
            case MMIMP3_AUDIO_PAUSED:
                MMIAPIMP3_ResumeMp3(FALSE);
                StartWidgetMp3Timer(win_id);
                s_widget_mp3_hit_type = MMIWIDGET_MP3_HIT_PAUSE_OFF; 
#ifdef PDA_UI_DROPDOWN_WIN
                MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_RUNNING_PLAYER);
#endif
                break;
 
            default:
                break;
            }
        }
        break;
        
    case MMIMP3_PLAYER_MUSIC_SWITCH:
        MMIAPIMP3_PauseMp3(FALSE);
        StopWidgetMp3Timer();
        s_widget_mp3_hit_type = MMIWIDGET_MP3_HIT_PLAY_OFF;
        break;
        
    case MMIMP3_PLAYER_SWITCH_PAUSED:
        MMIAPIMP3_ResumeMp3(FALSE);
        StartWidgetMp3Timer(win_id);
#ifdef PDA_UI_DROPDOWN_WIN
                MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_RUNNING_PLAYER);
#endif
        s_widget_mp3_hit_type = MMIWIDGET_MP3_HIT_PAUSE_OFF;
        break;
        
    default:
        break;
    }



#ifdef MMIAP_F_RELEASE_BG_AUDIO
    MMIAPIAP_ReleaseAudioHandle();
#endif
}

/*****************************************************************************/
//  Description : Handle WidgetMp3 play or pause btn
//  Global resource dependence : 
//  Author:cheng.luo
//  Note: 
/*****************************************************************************/
LOCAL void HandleWidgetMp3PlayPauseBtn(MMI_WIN_ID_T win_id)
{    
#if defined MMIWIDGET_MP3_SE 
    if(IsPlayAnimation(&s_widget_se))
    {
        return;
    }
#endif  

    if(CheckMp3RunCondition())
    {
        MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_AUDIO_PLAYER, HandlePlayPauseBtnAfterCheck);
    }
}

/*****************************************************************************/
//     Description : Handle WidgetMp3 Win Msg
//    Global resource dependence : 
//  Author:bin.ji
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTpPressUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};
    GUI_RECT_T button_prev_rect = MMIWIDGET_MP3_LBUTTON_RECT;
    GUI_RECT_T button_next_rect = MMIWIDGET_MP3_RBUTTON_RECT;    
    GUI_RECT_T button_play_rect = MMIWIDGET_MP3_ENTRYPLAY_RECT;
    GUI_RECT_T button_player_rect = MMIWIDGET_MP3_ENTRYPLAYER_RECT;	
    GUI_RECT_T button_progress_rect = MMIWIDGET_MP3_PROGRESS_RECT;

#ifndef WIN32
    button_progress_rect.top -= MMIWIDGET_MP3_PROGRESS_MARGIN;
    button_progress_rect.bottom += MMIWIDGET_MP3_PROGRESS_MARGIN;
#endif    
	point.x = MMK_GET_TP_X(param);
	point.y = MMK_GET_TP_Y(param);
    point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);
    if(MMIWIDGET_MP3_PANEL_PALY_STYLE == s_widget_mp3_panel_style)
    {
       	if (GUI_PointIsInRect(point, button_prev_rect))
	    {
	        if(s_widget_mp3_hit_type == MMIWIDGET_MP3_HIT_PREV)
	        {
                HandleWidgetMp3PreNextBtn(win_id,TRUE);
            }
	    }
	    else if (GUI_PointIsInRect(point, button_next_rect))
	    {  
	        if(s_widget_mp3_hit_type == MMIWIDGET_MP3_HIT_NEXT)
	        {
	            HandleWidgetMp3PreNextBtn(win_id,FALSE);
	        }
	    }
	   else if(GUI_PointIsInRect(point, button_play_rect))
       { 
           HandleWidgetMp3PlayPauseBtn(win_id);
       }
	   else if(GUI_PointIsInRect(point, button_player_rect)
	    && !GUI_PointIsInRect(point, button_progress_rect))
	   {
		   //打开mp3播放器
             MMIAPIMP3_OpenMp3Player();
	   }
	   else 
	   {
		   recode = MMI_RESULT_FALSE;	   
	   }
    }
   s_widget_mp3_hit_type = MMIWIDGET_MP3_HIT_NONE;   
    return recode;
}

/*****************************************************************************/
//  Description : Handle WidgetMp3 Win Msg
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetMp3WinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T   parent_win_id = MMK_GetParentWinHandle( win_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#if defined  MMIWIDGET_MP3_SE
        CreateImgAnimCtrl(win_id);
        s_widget_se.CreateResourceBuffer = CreateImageBuffer;  /*lint !e64 */
        s_widget_se.DestroyResourceBuffer = DestroyImageBuffer;    /*lint !e64 */     
        s_widget_se.Display = Display;         /*lint !e64 */
        s_widget_se.user_data = &s_anim_para;        
#endif        
		break;

#ifdef TOUCH_PANEL_SUPPORT
    case MSG_WIDGET_TP_PRESS_DOWN:
        //点中显示图标按下效果
        recode = HandleTpPressDown(win_id, param);
	    DisplayMp3Panel(win_id,s_widget_mp3_panel_style);	
        break;

    case MSG_WIDGET_TP_PRESS_MOVE:
		recode = MMI_RESULT_FALSE;
		break;

    case MSG_WIDGET_TP_PRESS_UP:
        StopForwardOrBackwardTimer();
        recode = HandleTpPressUp(win_id, param);
        break;
        
    case MSG_WIDGET_TP_PRESS_LONG_DOWN:	
        recode = HandleWidgetMp3TpLongKey(win_id,param);
        break;  
#endif
        
#ifdef BAR_PHONE_WIDGET 
    case MSG_KEYLONG_DOWN:
        if (MMIAPIMP3_ForwardMp3())
        {
            s_widget_mp3_hit_type = MMIWIDGET_MP3_LONGHIT_NEXT;
            StartForwardOrBackwardTimer(win_id,TRUE);
        }
#if !defined MMIWIDGET_MP3_SE 
        DisplayMp3Panel(win_id,s_widget_mp3_panel_style);	
#endif        
        break; 
    
    case MSG_KEYLONG_UP:
        if (MMIAPIMP3_BackwardMp3())
        {
            s_widget_mp3_hit_type = MMIWIDGET_MP3_LONGHIT_PREV;
            StartForwardOrBackwardTimer(win_id,FALSE);
        }
#if !defined MMIWIDGET_MP3_SE 
        DisplayMp3Panel(win_id,s_widget_mp3_panel_style);	
#endif        
        break;   

    case MSG_KEYPRESSUP_DOWN:
    case MSG_KEYPRESSUP_UP:
        StopForwardOrBackwardTimer();        
        s_widget_mp3_hit_type = MMIWIDGET_MP3_HIT_NONE;
#if !defined MMIWIDGET_MP3_SE 
        DisplayMp3Panel(win_id,s_widget_mp3_panel_style);	
#endif 
        break;

    case MSG_KEYUP_UP:
        if(s_widget_mp3_hit_type == MMIWIDGET_MP3_HIT_PREV)
        { 
            HandleWidgetMp3PreNextBtn(win_id,TRUE);
        }
        s_widget_mp3_hit_type = MMIWIDGET_MP3_HIT_NONE;
#if !defined MMIWIDGET_MP3_SE 
        DisplayMp3Panel(win_id,s_widget_mp3_panel_style);	
#endif 
        break;   
        
    case MSG_KEYUP_DOWN:    
        if(s_widget_mp3_hit_type == MMIWIDGET_MP3_HIT_NEXT)
        {
            HandleWidgetMp3PreNextBtn(win_id,FALSE);
        }
        s_widget_mp3_hit_type = MMIWIDGET_MP3_HIT_NONE;
#if !defined MMIWIDGET_MP3_SE 
        DisplayMp3Panel(win_id,s_widget_mp3_panel_style);	
#endif 
        break;    
        
    case MSG_KEYDOWN_UP:
        s_widget_mp3_hit_type = MMIWIDGET_MP3_HIT_PREV; 
#if !defined MMIWIDGET_MP3_SE 
        DisplayMp3Panel(win_id,s_widget_mp3_panel_style);	
#endif 
        break;   
        
    case MSG_KEYDOWN_DOWN:    
        s_widget_mp3_hit_type = MMIWIDGET_MP3_HIT_NEXT; 
#if !defined MMIWIDGET_MP3_SE 
        DisplayMp3Panel(win_id,s_widget_mp3_panel_style);	
#endif 
        break;    
#endif
        
    case MSG_APP_WEB:
#ifdef BAR_PHONE_WIDGET 
        HandleWidgetMp3PlayPauseBtn(win_id);
#if !defined MMIWIDGET_MP3_SE 
        DisplayMp3Panel(win_id,s_widget_mp3_panel_style);	
#endif 
#else
        MMIAPIMP3_OpenMp3Player();
#endif        
        break;      

        
    case MSG_TIMER:

	     if(MMIWIDGET_MP3_PANEL_PALY_STYLE == s_widget_mp3_panel_style)	
         {
             if(*(uint8 *)param == s_widget_mp3_progess_timer_id)
             {
                 BOOLEAN  is_page_moving = FALSE;
                 if(!MMK_IsFocusWin(parent_win_id))
                 {
                    break;
                 }
#if defined QBTHEME_SUPPORT
                     if(MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
                     {
                         is_page_moving = MMIQBIDLE_IsForbidPaint(MMIWIDGET_MP3_ID);                     
                     }
#endif

#ifdef MMI_GRID_IDLE_SUPPORT
                 if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle()) 
                 {
                     is_page_moving = MMIGRID_IsForbidPaint(MMIWIDGET_MP3_ID);
                 }
#endif
#if !defined MMIWIDGET_MP3_SE 
                 if (!is_page_moving)
                 {
                     DisplayMp3Panel(win_id, MMIWIDGET_MP3_PANEL_PALY_STYLE);
                 }
#else                 
                UpdateImageBuffer(win_id, &s_widget_se);
#endif
             }
             else if (s_widget_mp3_forward_backward_timer_id == *((uint8 *)param))
             {
                 BOOLEAN seek_result = FALSE;
                 StopForwardOrBackwardTimer();
                 
                 if (s_widget_mp3_is_forward)
                 {
                     seek_result = MMIMP3_ForwardMp3();
                 }
                 else
                 {
                     seek_result = MMIMP3_BackwardMp3();
                 }
                 StartForwardOrBackwardTimer(win_id,s_widget_mp3_is_forward);
#if !defined MMIWIDGET_MP3_SE 
	       		 DisplayMp3Panel(win_id, MMIWIDGET_MP3_PANEL_PALY_STYLE);             
#else                 
                UpdateImageBuffer(win_id, &s_widget_se);
#endif
             }

         }
		break;
		
    case MSG_FULL_PAINT:
#if !defined MMIWIDGET_MP3_SE 
	     DisplayMp3Panel(win_id,s_widget_mp3_panel_style);	
#endif        
        break;

#ifdef BAR_PHONE_WIDGET 
    case MSG_WIDGET_UPDATE_SOFTKEY:        
        DisplayMp3Softkey(win_id,FALSE);
         break;
#endif
             
		//目前只有处于MP3背景播放后，widget mp3才播放
   case MSG_GET_FOCUS:
	    //SetImgAnimData(win_id);
        ResetWidgetMp3();
        
   		//if (MMIAPIMP3_IsPlayerPlaying())
        if (MMIAPIMP3_IsPlayerActive())
		{
		    StartWidgetMp3Timer(win_id);
		}
		else
		{
		    StopWidgetMp3Timer();
		}
		break;

   case MSG_CLOSE_WINDOW:
   case MSG_LOSE_FOCUS:
   	    StopWidgetMp3Timer();
	    StopForwardOrBackwardTimer();
        //StopAdjustVolTimer();
		break;

#if defined MMIWIDGET_MP3_SE 
   case MSG_NOTIFY_ANIM_DISPLAY_IND:
        {
            GUIANIM_DISPLAY_IND_T *anim_ind_ptr = (GUIANIM_DISPLAY_IND_T*)param;
            int32 i = 0;

            MMIWIDGET_DEBUG(LOG_STR("MSG_NOTIFY_ANIM_DISPLAY_IND result = %d, time = %"),
                            anim_ind_ptr->result, SCI_GetTickCount());                    
            
            //sp_mp3_info would be freed, if widget was not focused state
            if(0 == s_widget_se.focus_flag)
            {                
                break;
            }
            
            //DisplayMp3Panel(win_id, MMIWIDGET_MP3_PANEL_PALY_STYLE);
            for(i = 0; i < MP3_INFO_ARRAY_NUM; i++)
            {
                if(1 == sp_mp3_info[i]->valid_flag)
                {
                    break;
                }
            }       
            MMIWIDGET_DEBUG(LOG_STR(" index = %d"), i);             
            
            if(i< MP3_INFO_ARRAY_NUM)
            {
                GUIANIM_GetAnimDisplayBitmap(MMI_MP3_ANIM_CTRL_ID, &s_cover_img);  
                //release album pic raw data
                SCI_FREE(sp_mp3_info[i]->album_info.img_data_ptr);
                UpdateAlbumPictureResource(win_id, sp_mp3_info[i], i, &s_cover_img);    
                DecodeAlbumPicture(sp_mp3_info, i + 1);
            }                    
        }
        break;

    case MSG_WIDGET_MP3_SWITCH:
    {
        wchar full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
        uint16 full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    
        
        MMIWIDGET_DEBUG(LOG_STR("MSG_WIDGET_MP3_SWITCH time = %d"), SCI_GetTickCount());
        MMIAPIMP3_GetCurPlayFileFullPath(full_path_name,&full_path_len);
        if(0 == MMIAPICOM_Wstrcmp(sp_mp3_info[0]->path, full_path_name))
        {
            break;
        }        
      
        s_widget_se.current_frame = 0;
        s_anim_para.switch_type = (int32)param ? -1 : 1;

        if(-1 == s_anim_para.switch_type)
        {
            SwitchPlayList(sp_mp3_info, s_anim_para.switch_type);
        }
        if(MMIAPIMP3_IsPlayerPlaying())
        {                    
            s_anim_para.anim_type = MMIWIDGET_MP3_SE_ANIM_SWITCH_ON_PLAY;
        }
        else
        {
            s_anim_para.anim_type = MMIWIDGET_MP3_SE_ANIM_SWITCH_ON_PAUSE;
        }
      
        break;
    } 
#endif

#if !defined MMIWIDGET_MP3_SE 
    case MSG_IDLE_UPDATE_MP3:
        DisplayMp3Panel(win_id, s_widget_mp3_panel_style);
        break;
#endif
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
#if defined MMIWIDGET_MP3_SE 
    MMIWIDGET_HandleWidgetSpecialEffectWinMsg(&s_widget_se, win_id, msg_id, param);
#endif

    return recode;        
}

#if !defined MMIWIDGET_MP3_SE
/*****************************************************************************/
//  Description : display mp3 name
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void DisplayMp3Panel(
                           MMI_WIN_ID_T     win_id,
                           MMIWIDGET_MP3_PANEL_STYLE_E panel_style
                           )
{
    BOOLEAN    isPressPlayBtn = FALSE;
    GUI_RECT_T win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
    MMITHEME_WIDGET_MP3_T mp3_widget_theme = {0};
    GUI_POINT_T point = {0};
	GUI_RECT_T button_prev_rect = MMIWIDGET_MP3_LBUTTON_RECT;
    GUI_RECT_T button_next_rect = MMIWIDGET_MP3_RBUTTON_RECT;
    GUI_RECT_T button_play_rect = MMIWIDGET_MP3_ENTRYPLAY_RECT;
    GUI_RECT_T button_display_rect = {0};
    GUI_RECT_T album_rect = MMIWIDGET_MP3_MP3ALBUM_RECT;
    GUI_RECT_T name_rect = MMIWIDGET_MP3_MP3NAME_RECT;
    MMIMP3_ALBUM_INFO_T album_info  = {0};
    
    MMITHEME_GetMp3WidgetTheme(&mp3_widget_theme);    
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    
    if (!UILAYER_IsLayerActive(&lcd_dev_info))
    {
        return;
    }    

    //in case clear main layer
    if(UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&lcd_dev_info))
    {
        UILAYER_ClearRect(&lcd_dev_info, win_rect);
    }

    if(MMIWIDGET_MP3_PANEL_PALY_STYLE == panel_style)
    {	 
	        //显示背景图
	        point.x = win_rect.left;
	        point.y = win_rect.top;
	        
	        GUIRES_DisplayImg(&point,
              	              PNULL,
              	              PNULL,
              	              win_id,
              	              mp3_widget_theme.bg_img,
              	              &lcd_dev_info
              	              );
              	              
	            button_display_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, button_prev_rect);
	            GUIRES_DisplayImg(
	                PNULL,
	                &button_display_rect,
	                PNULL,
	                win_id,
	                IMAGE_WIDGET_MP3_PREV_OFF,
	                &lcd_dev_info
                       );

	            button_display_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, button_next_rect);
	            GUIRES_DisplayImg(
	                PNULL,
	                &button_display_rect,
	                PNULL,
	                win_id,
	                IMAGE_WIDGET_MP3_NEXT_OFF,
	                &lcd_dev_info
	                );
                DisplayMp3Name(win_id,  &lcd_dev_info, &name_rect); 
#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
                MMIAPIMP3_GetCurMp3AlbumInfo(&album_info);
                DisplayMp3AlbumInfo(win_id,  &lcd_dev_info, &album_rect, &album_info);
#endif

	        switch (s_widget_mp3_hit_type)
	        {
	        case MMIWIDGET_MP3_HIT_PREV:	        
	        case MMIWIDGET_MP3_LONGHIT_PREV:
			//widget在mp3未播放的时候不应响应	
		       if(MMIAPIMP3_IsPlayerActive())
		      {
		            //前进按钮
		            button_display_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, button_prev_rect);
		            GUIRES_DisplayImg(
		                PNULL,
		                &button_display_rect,
		                PNULL,
		                win_id,
		                IMAGE_WIDGET_MP3_PREV_ON,//mp3_widget_theme.entry_on_img,
		                &lcd_dev_info
		                );
		       }
	            break;
	        case MMIWIDGET_MP3_HIT_NEXT:
	        case MMIWIDGET_MP3_LONGHIT_NEXT:
			//widget在mp3未播放的时候不应响应
		       if(MMIAPIMP3_IsPlayerActive())
		       {
		            //后退按钮
		            button_display_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, button_next_rect);
		            GUIRES_DisplayImg(
		                PNULL,
		                &button_display_rect,
		                PNULL,
		                win_id,
		                IMAGE_WIDGET_MP3_NEXT_ON,
		                &lcd_dev_info
		                );
		       }
	            break;
	        case MMIWIDGET_MP3_HIT_PLAY_ON:
		     //播放  
			//widget在mp3未播放的时候不应响应
		       if(MMIAPIMP3_IsPlayerActive())
		        {
   		            isPressPlayBtn = TRUE;
		                button_display_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, button_play_rect);
		                GUIRES_DisplayImg(
		                    PNULL,
		                    &button_display_rect,
		                    PNULL,
		                    win_id,
		                    IMAGE_WIDGET_MP3_PLAY_ON,
		                    &lcd_dev_info
		                    );
	              }
	              break;
				
	        case MMIWIDGET_MP3_HIT_PLAY_OFF:
			//widget在mp3未播放的时候不应响应
		       if(MMIAPIMP3_IsPlayerActive())
		       {
   		           isPressPlayBtn = TRUE;		       
		                button_display_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, button_play_rect);
		                GUIRES_DisplayImg(
		                    PNULL,
		                    &button_display_rect,
		                    PNULL,
		                    win_id,
		                    IMAGE_WIDGET_MP3_PLAY_OFF,
		                    &lcd_dev_info
		                    );
	             }
	            break;
	           
	        case MMIWIDGET_MP3_HIT_PAUSE_ON:
		     //暂停
			//widget在mp3未播放的时候不应响应
		       if(MMIAPIMP3_IsPlayerActive())
		      {	     
    		      isPressPlayBtn = TRUE;
		                button_display_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, button_play_rect);
		                GUIRES_DisplayImg(
		                    PNULL,
		                    &button_display_rect,
		                    PNULL,
		                    win_id,
		                    IMAGE_WIDGET_MP3_PAUSE_ON,
		                    &lcd_dev_info
		                    );
	              }
	            break;
		
	        case MMIWIDGET_MP3_HIT_PAUSE_OFF:
			//widget在mp3未播放的时候不应响应
		       if(MMIAPIMP3_IsPlayerActive())
			   {
 		           isPressPlayBtn = TRUE;
			    	button_display_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, button_play_rect);	
	                GUIRES_DisplayImg(
	                    PNULL,
	                    &button_display_rect,
	                    PNULL,
	                    win_id,
	                    IMAGE_WIDGET_MP3_PAUSE_OFF,//mp3_widget_theme.entry_on_img,
	                    &lcd_dev_info
	                    );
	            }
	            break;
	        default:
	            break;
	        }
	   

        if (!isPressPlayBtn)
        {
            MMI_IMAGE_ID_T  img_id = 0;
            
            switch(MMIAPIMP3_GetPlayerState())
            {
            case MMIMP3_PLAYER_STATE_NONE:
            case MMIMP3_PLAYER_SWITCH_PAUSED:
                img_id = IMAGE_WIDGET_MP3_PLAY_OFF;                
                break;
            case MMIMP3_PLAYER_MUSIC_SWITCH:
                img_id = IMAGE_WIDGET_MP3_PAUSE_OFF;
                break;
            case MMIMP3_PLAYER_AUDIO_ACTIVE:
                if (MMIMP3_AUDIO_PLAYING == MMIAPIMP3_GetAudioState())
                {
                    img_id = IMAGE_WIDGET_MP3_PAUSE_OFF;
                }
                else
                {
                    img_id = IMAGE_WIDGET_MP3_PLAY_OFF;
                }
                break;
            default:
                break;
            }
            
            button_display_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, button_play_rect);
            GUIRES_DisplayImg(
                PNULL,
                &button_display_rect,
                PNULL,
                win_id,
                img_id,
                &lcd_dev_info
                );
        }  
    }
     if(MMIWIDGET_MP3_PANEL_PALY_STYLE == s_widget_mp3_panel_style)
     {
         DisplayWigetMp3TimeInfo(win_id);
         DisplayWigetMp3Progress(win_id);
     }
#ifdef BAR_PHONE_WIDGET 
     DisplayMp3Softkey(win_id, TRUE);
#endif
}
#endif

/*****************************************************************************/
//  Description : display mp3 softkey
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
#ifdef BAR_PHONE_WIDGET 
LOCAL void DisplayMp3Softkey(
                           MMI_WIN_ID_T     win_id,                           
                           BOOLEAN          is_update
                           )
{
    MMI_TEXT_ID_T    text_id;
    
    if((MMIAPIMP3_IsPlayerActive()) && (MMIMP3_AUDIO_PLAYING == MMIMP3_GetAudioState()))
    {
        text_id = TXT_PAUSE;
    }
    else
    {
        text_id = TXT_COMMON_PLAY;
    }
    MMIWIDGET_SoftkeySetTextId(win_id,text_id,is_update);
}
#endif



/*****************************************************************************/
//  Description : display mp3 name
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void DisplayMp3Name(
                          MMI_WIN_ID_T     win_id,
                            GUI_LCD_DEV_INFO *lcd_dev_ptr,
                            GUI_RECT_T* rect_ptr                          
                          )
{
    GUISTR_STYLE_T text_style = {0};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;
    MMI_STRING_T mp3_str = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = *lcd_dev_ptr;
    GUI_RECT_T text_rect = MMIWIDGET_MP3_MP3NAME_RECT;
    MMI_STRING_T     tempStr = {0};
    uint16      font_width = 0;
    uint16      mp3_text_len = 0;
    int         i = 0;
    GUI_FONT_T font = SONG_FONT_16;
    
    text_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, *rect_ptr);
    
    text_style.align = ALIGN_LVMIDDLE;
    text_style.font = font;
    text_style.font_color = MMI_BLACK_COLOR;
    
    //get mp3 name
    mp3_str.wstr_ptr = (wchar*)SCI_ALLOC_APP((MMIFMM_FILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar));
    SCI_MEMSET(mp3_str.wstr_ptr,0,((MMIFMM_FILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar)));        
    MMIAPIMP3_GetCurMp3Name(&mp3_str, MMIMP3_NAME_MAX_LEN);
    //采用label控件会有背景刷新问题
    //GetMp3WidgetTextStyle(&text_style);
   // font_width =  GUI_GetFontWidth(font, 'A');
    mp3_text_len = GUI_GetStringWidth(font, mp3_str.wstr_ptr, mp3_str.wstr_len);

    if(mp3_text_len >= (text_rect.right - text_rect.left))
    {
        for(i=0; i<mp3_str.wstr_len; i++)
        {
            font_width += GUI_GetStringWidth(font, &mp3_str.wstr_ptr[i], 1);
            if(font_width> (text_rect.right - text_rect.left))
            {
                break;
            }
        }
        mp3_str.wstr_len = i-3;
        mp3_str.wstr_ptr[mp3_str.wstr_len] = 0;
        MMIAPICOM_Wstrcat(mp3_str.wstr_ptr, L"...");
        mp3_str.wstr_len+=3;
    }
    //display string
    if(0 == mp3_str.wstr_len )  //|| (MMIMP3_PLAYER_STATE_NONE == MMIAPIMP3_GetPlayerState()))
    {
        MMI_GetLabelTextByLang(TXT_AUDIO, &tempStr);
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &text_rect,
            &text_rect,
            &tempStr,	   
            &text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO
            );
    }
    else
    {
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &text_rect,
            &text_rect,
            &mp3_str,	   
            &text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO
            );
    }
    //free memory
    SCI_FREE(mp3_str.wstr_ptr);

    
}

#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
/*****************************************************************************/
//  Description : display mp3 album info
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void DisplayMp3AlbumInfo(MMI_WIN_ID_T     win_id,
                               GUI_LCD_DEV_INFO *lcd_dev_ptr,
                               GUI_RECT_T *rect_ptr  ,
                               MMIMP3_ALBUM_INFO_T *album_info_ptr
                                                            )
{
    GUISTR_STYLE_T text_style = {0};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;
    MMI_STRING_T mp3_str = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = *lcd_dev_ptr;
    GUI_RECT_T text_rect = MMIWIDGET_MP3_MP3ALBUM_RECT;
    //MMIMP3_ALBUM_INFO_T album_info  = {0};
    uint32 temp_length = 0;
    uint16 mp3_text_len = 0;
    int32 i = 0;
    uint16 font_width = 0;
    GUI_FONT_T font = SONG_FONT_14;
    
    text_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, *rect_ptr);
    
    text_style.align = ALIGN_LVMIDDLE;
    text_style.font = font;
    text_style.font_color = MMI_BLACK_COLOR;
    

    //get mp3 name
    mp3_str.wstr_ptr = (wchar*)SCI_ALLOC_APP((MMIFMM_FILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar));
    SCI_MEMSET(mp3_str.wstr_ptr,0,((MMIFMM_FILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar)));        
    mp3_str.wstr_len = 0;
    
    //display string
    // MMIAPIMP3_GetCurMp3AlbumInfo(&album_info);
     
     temp_length = MMIAPICOM_Wstrlen(album_info_ptr->artist);         
     if(temp_length != 0)
     {
        MMIAPICOM_Wstrcat(mp3_str.wstr_ptr, album_info_ptr->artist);        
        mp3_str.wstr_len = temp_length;
     }     
     
     temp_length = MMIAPICOM_Wstrlen(album_info_ptr->album);
     if(temp_length != 0)
     {
         mp3_str.wstr_ptr[mp3_str.wstr_len] = L'-';
         mp3_str.wstr_len++;
         MMIAPICOM_Wstrcat(mp3_str.wstr_ptr, album_info_ptr->album);
         mp3_str.wstr_len += temp_length;
     }  
     
    mp3_text_len = GUI_GetStringWidth(font, mp3_str.wstr_ptr, mp3_str.wstr_len);

    if(mp3_text_len >= (text_rect.right - text_rect.left))
    {
        for(i=0; i<mp3_str.wstr_len; i++)
        {
            font_width += GUI_GetStringWidth(font, &mp3_str.wstr_ptr[i], 1);
            if(font_width> (text_rect.right - text_rect.left))
            {
                break;
            }
        }
        mp3_str.wstr_len = i-3;
        mp3_str.wstr_ptr[mp3_str.wstr_len] = 0;
        MMIAPICOM_Wstrcat(mp3_str.wstr_ptr, L"...");
        mp3_str.wstr_len+=3;
    }

    if(0==mp3_str.wstr_len&&MMIMP3_PLAYER_AUDIO_ACTIVE==MMIAPIMP3_GetPlayerState())   
    {
        MMI_STRING_T str_t = {0};
        MMI_GetLabelTextByLang(TXT_NO_ALBUM,&str_t);
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &text_rect,
            &text_rect,
            &str_t,	   
            &text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO
            );
    }
    else
    {
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &text_rect,
            &text_rect,
            &mp3_str,	   
            &text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO
            );
    }
    //free memory
    SCI_FREE(mp3_str.wstr_ptr);
}
#endif
/*****************************************************************************/
//  Description : 获得mp3 widget上的文本风格
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void GetMp3WidgetTextStyle(
								 GUISTR_STYLE_T *style_ptr
								 )
{
    MMITHEME_WIDGET_MP3_T mp3_widget_theme = {0};
	
    MMITHEME_GetMp3WidgetTheme(&mp3_widget_theme);
	
    //SCI_ASSERT(PNULL != style_ptr);

    if (PNULL == style_ptr)
    {
        //SCI_TRACE_LOW:"GetMp3WidgetTextStyle PNULL == style_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_MP3_1548_112_2_18_3_8_18_207,(uint8*)"");
        return;
    }
	
    style_ptr->effect = FONT_EFFECT_NONE; 
    style_ptr->angle = ANGLE_0;
    style_ptr->align = ALIGN_HVMIDDLE;
    style_ptr->font = mp3_widget_theme.mp3name_font;
    style_ptr->font_color = mp3_widget_theme.mp3name_color;
}

#if !defined MMIWIDGET_MP3_SE
/*****************************************************************************/
//  Description : display mp3 time info
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL  void   DisplayWigetMp3TimeInfo(MMI_WIN_ID_T win_id)
{
    GUISTR_STYLE_T text_style = {0};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;
    MMI_STRING_T    cur_str_t   = {0};
    MMI_STRING_T    total_str_t = {0};
    wchar        cur_time_wstr[32]   = {0}; 
    wchar        total_time_wstr[32] = {0}; 
    char         cur_time_str[32] = {0};
    char         total_time_str[32] = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T text_rect = MMIWIDGET_MP3_MP3TIME_RECT;
	MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
	if (!UILAYER_IsLayerActive(&lcd_dev_info))
	{
	    return;
	}
    text_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, text_rect);
    
    text_style.align = ALIGN_LVMIDDLE;
    text_style.font = 12;
    text_style.font_color = MMI_BLACK_COLOR;
    
    if (MMIAPIMP3_IsPlayerActive()
        || MMIAPIAP_IsDisplayForPlayStateNone(FALSE, TRUE)
       )
    {        
        MMIAPIMP3_GetPlayingDynInfo(&dyn_info,FALSE);
        if(dyn_info.total_time/60 < 60)
        {
            cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.2ld:%.2ld", dyn_info.cur_time/60, dyn_info.cur_time%60);
            total_str_t.wstr_len = sprintf((char*)total_time_str, "%.2ld:%.2ld", dyn_info.total_time/60, dyn_info.total_time%60);
        }
        else
        {
            cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.2ld:%.2ld:%.2ld", (dyn_info.cur_time/60)/60, 
                (dyn_info.cur_time/60)%60, 
                (dyn_info.cur_time%60));
            total_str_t.wstr_len = sprintf((char*)total_time_str, "%.2ld:%.2ld:%.2ld", (dyn_info.total_time/60)/60,
                (dyn_info.total_time/60)%60, 
                dyn_info.total_time%60);
        }
        cur_str_t.wstr_ptr = cur_time_wstr;
        total_str_t.wstr_ptr = total_time_wstr;
        MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)cur_time_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
        MMI_STRNTOWSTR(total_str_t.wstr_ptr, total_str_t.wstr_len, (uint8*)total_time_str, total_str_t.wstr_len, total_str_t.wstr_len);
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &text_rect,
            &text_rect,
            &cur_str_t,	   
            &text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO
            );

        text_style.align = ALIGN_RVMIDDLE;
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            &text_rect,
            &text_rect,
            &total_str_t,	   
            &text_style,
            text_state,
            GUISTR_TEXT_DIR_AUTO
            );        
    } 
}

/*****************************************************************************/
//  Description : display mp3 progress bar
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL  void   DisplayWigetMp3Progress(MMI_WIN_ID_T win_id)
{
	GUI_RECT_T          progress_win_rect = MMIWIDGET_MP3_PROGRESS_RECT;
	GUI_RECT_T          progress_lcd_rect = {0};
	GUI_LCD_DEV_INFO    lcd_dev_info = {0};
	MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};
	MMITHEME_WIDGET_MP3_T mp3_widget_theme = {0};
	MMITHEME_GetMp3WidgetTheme(&mp3_widget_theme);   
	//获取窗口的层
	MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);	
	if (!UILAYER_IsLayerActive(&lcd_dev_info))
	{
	    return;
	}
		progress_lcd_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id,progress_win_rect);

        if (MMIAPIMP3_IsPlayerActive()
            || MMIAPIAP_IsDisplayForPlayStateNone(FALSE, FALSE)
           )
		{
			//获取播放信息	
			MMIAPIMP3_GetPlayingDynInfo(&dyn_info,FALSE);
            if(dyn_info.total_length != 0)
            {
                progress_lcd_rect.right = progress_lcd_rect.left 
                    + (uint32)(MMIWIDGET_MP3_PROGRESS_WIDTH*((uint64_t)dyn_info.cur_offset)/dyn_info.total_length);/*lint !e737*/
            }
            else
            {
                progress_lcd_rect.right = progress_lcd_rect.left;     
            }
				  GUIRES_DisplayImg(
					  PNULL,
                      &progress_lcd_rect,
					  PNULL,
					  win_id,
					  IMAGE_WIDGET_MP3_SCROLLBAR_BOLL,
					  &lcd_dev_info
					  );			   
		}

	
}
#endif

LOCAL BOOLEAN StartWidgetMp3Timer(MMI_WIN_ID_T win_id)
{
    BOOLEAN     result  =   FALSE;
	
	
	if ((0 == s_widget_mp3_progess_timer_id) )
		
	{
		s_widget_mp3_progess_timer_id = MMK_CreateWinTimer(win_id, MMIWIDGET_MP3_PROGRESS_TIMEOUT, TRUE);
		result = TRUE;
	}
	
	
    return (result);
}


LOCAL BOOLEAN StopWidgetMp3Timer(void)
{
    BOOLEAN     result  =   FALSE;
	
    if (0 != s_widget_mp3_progess_timer_id)
    {
        MMK_StopTimer(s_widget_mp3_progess_timer_id);
        s_widget_mp3_progess_timer_id = 0;
        result = TRUE;
    }
	
    return (result);
}
LOCAL  BOOLEAN HandleProgressBarTpKey(
													int16          tp_x,
													GUI_RECT_T     progress_bar_rect
													)
{
    BOOLEAN     result = FALSE;
    BOOLEAN     is_forward = FALSE;
    uint32      tp_x_offet = 0;
    uint32      mp3_total_offset = 0;
    uint32      mp3_bitrate = 0;
    uint32      review_start_offset = 0;
    uint32      review_end_offset = 0;

    if(!(progress_bar_rect.left <= tp_x) && (tp_x <= progress_bar_rect.right))
    {
        //MMI_ASSERT_WARNING("[MMIMP3]:yaohua MMIMP3_HandleProgressBarTpKey, tp_x=%d error!",tp_x);
        //SCI_TRACE_LOW:"HandleProgressBarTpKey  tp_x=%d error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_MP3_1723_112_2_18_3_8_19_208,(uint8*)"d", tp_x);
        return result;
    }
	
    if ((MMIMP3_AUDIO_PLAYING == MMIAPIMP3_GetAudioState()) ||
        (MMIMP3_AUDIO_PAUSED  == MMIAPIMP3_GetAudioState()))
    {
        //get forward or backward length and time offset
        tp_x_offet = tp_x - progress_bar_rect.left;
        //SCI_TRACE_LOW:"MMIMP3_HandleProgressBarTpKey:tp_x_offet = %d,tp_x = %d,progress_bar_rect.left = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_MP3_1732_112_2_18_3_8_19_209,(uint8*)"ddd",tp_x_offet,tp_x,progress_bar_rect.left);
        result = GetMp3CurrentOffsetByTp(&is_forward,tp_x_offet,&mp3_total_offset,&mp3_bitrate);
        
        if(MMIMP3_REVIEW_STATE_PLAY == MMIAPIMP3_GetReviewStatus() &&
            MMIAPIMP3_GetReviewStartPoint(&review_start_offset)     &&
            MMIAPIMP3_GetReviewEndPoint(&review_end_offset))
        {          
            if(mp3_total_offset >= review_end_offset ||
                mp3_total_offset <= review_start_offset)
            {
                return TRUE;
            }
        }
        
        if (result)
        {
            result = ForwardOrBackwardMp3(TRUE,is_forward,mp3_total_offset,MMIAPIMP3_GetAudioState());
        }
        else
        {
            //SCI_TRACE_LOW:"MMIMP3_HandleProgressBarTpKey:yaohua get mp3 current offset result %d is error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_MP3_1752_112_2_18_3_8_19_210,(uint8*)"d",result);
        }
        
    }
    else
    {
        //SCI_TRACE_LOW:"MMIMP3_HandleProgressBarTpKey:cur_state %d don't handle!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_MP3_1758_112_2_18_3_8_19_211,(uint8*)"d",MMIAPIMP3_GetAudioState());
    }
	
    return (result);
}
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
	
    if(PNULL == is_forward_ptr ||
		PNULL == mp3_total_offset_ptr ||
		PNULL == mp3_bitrate_ptr)
    {
        //MMI_ASSERT_WARNING("[MMIMP3]:yaohua GetMp3CurrentOffsetByTp, is_forward_ptr = %x,mp3_total_offset_ptr = %x,mp3_bitrate_ptr = %x!",is_forward_ptr,mp3_total_offset_ptr,mp3_bitrate_ptr);
        //SCI_TRACE_LOW:"GetMp3CurrentOffsetByTp, Error: is_forward_ptr = %x,mp3_total_offset_ptr = %x,mp3_bitrate_ptr = %x!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_MP3_1781_112_2_18_3_8_19_212,(uint8*)"ddd",is_forward_ptr,mp3_total_offset_ptr,mp3_bitrate_ptr);
        return FALSE;
    }
	
    MMIAPIMP3_GetPlayingDynInfo(&dyn_info,FALSE);
	
    if (0 != dyn_info.total_length)
    {
        //get current progress slide x
        cur_slide_x_offset = (uint32)(MMIWIDGET_MP3_PROGRESS_WIDTH*((uint64_t)dyn_info.cur_offset)/dyn_info.total_length);
        //SCI_TRACE_LOW:"GetMp3CurrentOffsetByTp:cur_slide_x_offset = %d,tp_x_offset = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_MP3_1791_112_2_18_3_8_19_213,(uint8*)"dd",cur_slide_x_offset,tp_x_offset);
		
        //if forward
        if (cur_slide_x_offset < tp_x_offset)
        {
            is_forward = TRUE;
        }
        else if (cur_slide_x_offset > tp_x_offset)
        {
            is_forward = FALSE;
        }
        else
        {
            result = FALSE;
        }
		
        if (result)
        {
            //get mp3 total offset
            mp3_total_offset = tp_x_offset*(dyn_info.total_length/MMIWIDGET_MP3_PROGRESS_WIDTH);
			
            //SCI_TRACE_LOW:"GetMp3CurrentOffsetByTp:is_forward = %d,mp3_total_offset = %ld"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_MP3_1812_112_2_18_3_8_19_214,(uint8*)"dd",is_forward,mp3_total_offset);
			
            *is_forward_ptr = is_forward;
            *mp3_total_offset_ptr = mp3_total_offset;
            *mp3_bitrate_ptr = dyn_info.bit_rate;
        }
    }
	
    return (result);
}

LOCAL  BOOLEAN ForwardOrBackwardMp3(BOOLEAN              is_tp_progress, //是否是Tp进度条
									BOOLEAN              is_forward,
									uint32               mp3_offset,     //按键:Mp3 pause位置算起,Tp进度条:Mp3 0位置算起
									MMIMP3_AUDIO_STATE_E cur_state)
{
    BOOLEAN             result = FALSE;
    uint32              mp3_total_offset = 0;
	
    //SCI_TRACE_LOW:"[MMIMP3] ForwardOrBackwardMp3 mp3_offset = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_MP3_1831_112_2_18_3_8_19_215,(uint8*)"d", mp3_offset);
    //MMI_PASSERT_WARNING((MMIMP3_AUDIO_PLAYING == cur_state) || (MMIMP3_AUDIO_PAUSED == cur_state),("[MMIMP3]:ForwardOrBackwardMp3, cur_state = %d error!",cur_state));
    if(!(MMIMP3_AUDIO_PLAYING == cur_state || MMIMP3_AUDIO_PAUSED == cur_state))
    {
        //MMI_ASSERT_WARNING("[MMIMP3]:ForwardOrBackwardMp3, cur_state = %d error!",cur_state);
        //SCI_TRACE_LOW:"ForwardOrBackwardMp3, cur_state = %d error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_MP3_1836_112_2_18_3_8_19_216,(uint8*)"d",cur_state);
        return result;
    }
		
    //get total mp3 offset,from start
    if (is_tp_progress)
    {
        mp3_total_offset = mp3_offset;
    }
	
    switch (cur_state)
    {
    case MMIMP3_AUDIO_PLAYING:
		result = MMISRVAUD_Seek(MMIAPIMP3_GetBGPlayerHandle(),mp3_total_offset);
        
        if (result)
        {
            //seek mp3 lyric
#ifdef MMIAP_LYRIC_SUPPORT            
            MMIAPIMP3_SeekLyric(mp3_total_offset,TRUE);
#endif        
        }
        else
        {
            //SCI_TRACE_LOW:"ForwardOrBackwardMp3:audio seek result = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_MP3_1863_112_2_18_3_8_19_217,(uint8*)"d",result);
        }
        break;
		
    case MMIMP3_AUDIO_PAUSED:
        result = MMIAPIMP3_ResumeMp3(FALSE);

        if(result)
        {
            MMISRVAUD_Seek(MMIAPIMP3_GetBGPlayerHandle(),mp3_total_offset);
            
#ifdef MMIAP_LYRIC_SUPPORT
            MMIAPIMP3_SeekLyric(mp3_total_offset, TRUE);
#endif              
        }
       break;
        
    default:
        break;
    }
	
    return (result);
}
LOCAL MMI_RESULT_E HandleWidgetMp3TpLongKey(
														  MMI_WIN_ID_T win_id,
														  DPARAM     param
														  )
{
    GUI_RECT_T button_prev_rect = MMIWIDGET_MP3_LBUTTON_RECT;
    GUI_RECT_T button_next_rect = MMIWIDGET_MP3_RBUTTON_RECT;
    GUI_POINT_T     tp_point = {0};
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    //get tp point
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);
    tp_point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, tp_point);	
    if(MMIWIDGET_MP3_PANEL_PALY_STYLE == s_widget_mp3_panel_style)
    {	
		if(!MMIAPIMP3_IsPlayerActive())
		{
			return MMI_RESULT_FALSE;
		}
		if (GUI_PointIsInRect(tp_point,button_prev_rect))
		{
			//显示按钮
			s_widget_mp3_hit_type = MMIWIDGET_MP3_LONGHIT_PREV;
			
			if(MMIAPIMP3_BackwardMp3())
			{
				//start forward or backward timer
				StartForwardOrBackwardTimer(win_id,FALSE);
			}
		}
		else if (GUI_PointIsInRect(tp_point,button_next_rect))
		{
			//显示按钮
			s_widget_mp3_hit_type = MMIWIDGET_MP3_LONGHIT_NEXT;
			if (MMIAPIMP3_ForwardMp3())
			{
				StartForwardOrBackwardTimer(win_id,TRUE);
			}
		}
		else
		{
			result = MMI_RESULT_FALSE;
		}
		
	}
    return (result);
}
LOCAL void StartForwardOrBackwardTimer(MMI_WIN_ID_T win_id,  BOOLEAN  is_forward )
{
    if (0 == s_widget_mp3_forward_backward_timer_id)
    {
        s_widget_mp3_forward_backward_timer_id = MMK_CreateWinTimer(win_id,MMIWIDGET_MP3_FORWARD_BACKWARD_PERIOD,FALSE);
        s_widget_mp3_is_forward = is_forward;
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
    if (0 < s_widget_mp3_forward_backward_timer_id)
    {
        MMK_StopTimer(s_widget_mp3_forward_backward_timer_id);
        s_widget_mp3_forward_backward_timer_id = 0;
    }

}

/*****************************************************************************/
//  Description : check mp3 can run or not
//  Global resource dependence : 
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckMp3RunCondition(void)
{
    BOOLEAN result = TRUE;
    
    if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        MMIPUB_OpenAlertWarningWin(TXT_CALL_USING);
        result = FALSE;
    }
    else if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        result = FALSE;
    }
    else if(MMIAPISD_IsCardLogOn())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_CARDLOG_USING);
        result = FALSE;
    }
    else if (MMIAPIENG_GetIQModeStatus())
    {
        MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
        result = FALSE;
    }

    return result;
}

#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
/*****************************************************************************/
// Description : create image viewer image animation control.
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL void CreateImgAnimCtrl(MMI_WIN_ID_T  win_id)
{
    GUIANIM_INIT_DATA_T init_data = {0};
    MMI_CONTROL_CREATE_T create   = {0};           
    MMI_CTRL_ID_T ctrl_id = 0;
    void    *anim_ptr = PNULL;
    GUI_RECT_T disp_rect = {0,0,117,117};
  
    ctrl_id = MMI_MP3_ANIM_CTRL_ID;

    disp_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, disp_rect);

    init_data.both_rect.h_rect = init_data.both_rect.v_rect = disp_rect;
    
    create.ctrl_id = ctrl_id;
    create.guid = SPRD_GUI_ANIM_ID;
    create.parent_win_handle = win_id;
    create.parent_ctrl_handle = 0;
    create.init_data_ptr = &init_data;
    
    anim_ptr = MMK_CreateControl(&create);

    GUIANIM_SetVisible(ctrl_id, FALSE, FALSE);
    
    //set anim ctrl rect
    GUIANIM_SetCtrlRectByPtr(anim_ptr,&disp_rect);

    //set image display rect
    GUIANIM_SetDisplayRectByPtr(anim_ptr,&disp_rect,FALSE);
}
/*****************************************************************************/
// Description : 显示网页加载完成动画的初始/完成状态
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetImgAnimData(MMIMP3_ALBUM_INFO_T *album_info )
{
	GUIANIM_CTRL_INFO_T     ctrl_info   = {0};
	MMI_CTRL_ID_T ctrl_id               = 0;
	GUIANIM_DISPLAY_INFO_T display_info = {0};
    GUIANIM_DATA_INFO_T     data_info   = {0};
	//MMIMP3_ALBUM_INFO_T  album_info     =  {0};
    //GUI_BG_T                err_bg      = {0};
	ctrl_id = MMI_MP3_ANIM_CTRL_ID;

    ctrl_info.is_wallpaper_ctrl = FALSE;
	ctrl_info.is_ctrl_id        = TRUE;
	ctrl_info.ctrl_id           = ctrl_id;
	ctrl_info.ctrl_ptr          = PNULL;

	display_info.is_update         = FALSE;
	display_info.align_style       = GUIANIM_ALIGN_HVMIDDLE;
	display_info.is_zoom           = TRUE;

	display_info.bg.bg_type    = GUI_BG_COLOR;
    display_info.bg.color      = MMI_BLACK_COLOR;
	display_info.bg.shape      = GUI_SHAPE_RECT;
	
	//MMIAPIMP3_GetCurMp3AlbumInfo(&album_info);	
	
    //if(PNULL != album_info->img_data_ptr && album_info->data_size > 0)
	{  
        data_info.data_ptr  = album_info->img_data_ptr;
        data_info.data_size = album_info->data_size;
        //图片格式可能不支持或数据已损坏，使用默认专辑图片作为错误背景
//        err_bg.bg_type  = GUI_BG_IMG;
//        err_bg.img_id   = IMAGE_WIDGET_MP3_COVERBG;
//        GUIANIM_SetErrorBg(ctrl_id, &err_bg);
        GUIANIM_SetParam(&ctrl_info,
            &data_info,
            PNULL,
            &display_info);
    }
    return TRUE;
}
#endif

#if defined MMIWIDGET_MP3_SE 
LOCAL BOOLEAN CreatePlayList(MMIWIDGET_MP3_INFO_T *pp_mp3_info[])
{
    int32 i = 0;
    for(i = 0; i < MP3_INFO_ARRAY_NUM; i++)
    {
        *(pp_mp3_info + i) = SCI_ALLOC_APP(sizeof(MMIWIDGET_MP3_INFO_T));
        SCI_MEMSET(*(pp_mp3_info + i), 0, sizeof(MMIWIDGET_MP3_INFO_T)) ;
    }
    return TRUE;
}

LOCAL BOOLEAN DestroyPlayList(MMIWIDGET_MP3_INFO_T *p_mp3_info[])
{    
    int32 i = 0;
    for(i = 0; i < MP3_INFO_ARRAY_NUM; i++)
    {
        DestroyAlbumPictureResource(p_mp3_info[i], i);
        SCI_FREE(p_mp3_info[i]); 
    }
    return TRUE;
}

LOCAL int32 UpdatePlayList(MMIWIDGET_MP3_INFO_T *p_mp3_info[])
{
    int32 result = 0;
    int32 i = 0;
    int32 total_num = MMIMP3_GetCurPlayListFileNum();
    if(0 == total_num)
    {
        return total_num;
    }
    total_num = total_num < MP3_INFO_ARRAY_NUM - 1?total_num : MP3_INFO_ARRAY_NUM - 1;
    for(i = 0; i < total_num; i++)
    {
        result = UpdatePlayListItem(p_mp3_info[i], i);    
    }
    UpdatePlayListItem(p_mp3_info[MP3_INFO_ARRAY_NUM - 1], - 1);  
    
    //DecodeAlbumPicture(p_mp3_info, 0);
    
    return total_num;
}

LOCAL int32 SwitchPlayList(MMIWIDGET_MP3_INFO_T *p_mp3_info[], int32 offset)
{
    uint32 i = 0;
    uint32 total_num = MMIMP3_GetCurPlayListFileNum();
    MMIWIDGET_MP3_INFO_T temp_mp3_info = {0};
    OBJECT_T temp_object = {0};
    if(0 == total_num)
    {
        return total_num;
    }
    MMIWIDGET_DEBUG(LOG_STR_ENTER("offset = %d, total_num = %d,  time = %d"),  
                        offset, total_num, SCI_GetTickCount());
    total_num = total_num < MP3_INFO_ARRAY_NUM - 1?total_num : MP3_INFO_ARRAY_NUM - 1;
    
    if(offset > 0)
    {   
        //DestroyAlbumPictureResource(p_mp3_info[MP3_INFO_ARRAY_NUM - 1], MP3_INFO_ARRAY_NUM - 1);        
        
        SCI_MEMCPY(&temp_mp3_info, p_mp3_info[MP3_INFO_ARRAY_NUM - 1], sizeof(MMIWIDGET_MP3_INFO_T));
        temp_object = s_mmi_resource_array.CoverImg[MP3_INFO_ARRAY_NUM - 1];

        SCI_MEMCPY(p_mp3_info[MP3_INFO_ARRAY_NUM - 1], p_mp3_info[0], sizeof(MMIWIDGET_MP3_INFO_T));   
        s_mmi_resource_array.CoverImg[MP3_INFO_ARRAY_NUM - 1] = s_mmi_resource_array.CoverImg[0];
        
        for(i = 0; i < total_num - 1; i++)
        {
             SCI_MEMCPY(p_mp3_info[i], p_mp3_info[i + 1], sizeof(MMIWIDGET_MP3_INFO_T));              
             s_mmi_resource_array.CoverImg[i] = s_mmi_resource_array.CoverImg[i + 1];
        }
        
        SCI_MEMCPY(p_mp3_info[i], &temp_mp3_info, sizeof(MMIWIDGET_MP3_INFO_T));
        s_mmi_resource_array.CoverImg[i] = temp_object;
        
        UpdatePlayListItem(p_mp3_info[i], i);  
        
        DestroyAlbumPictureResource( p_mp3_info[i], i);
        CreateAlbumPictureResource( MAIN_IDLE_WIN_ID,  p_mp3_info[i], i, 0);

    }
    else
    {        
        //DestroyAlbumPictureResource(p_mp3_info[total_num - 1], total_num - 1);
        //p_mp3_info[total_num - 1]->valid_flag = 0;      

        SCI_MEMCPY(&temp_mp3_info, p_mp3_info[total_num - 1], sizeof(MMIWIDGET_MP3_INFO_T));
        temp_object = s_mmi_resource_array.CoverImg[total_num - 1];

        for(i = total_num - 1;i >= 1; i--)
        {
             SCI_MEMCPY(p_mp3_info[i], p_mp3_info[i - 1], sizeof(MMIWIDGET_MP3_INFO_T)); 
             s_mmi_resource_array.CoverImg[i] = s_mmi_resource_array.CoverImg[i - 1];
        }
        SCI_MEMCPY(p_mp3_info[0], p_mp3_info[MP3_INFO_ARRAY_NUM - 1], sizeof(MMIWIDGET_MP3_INFO_T)); 
        s_mmi_resource_array.CoverImg[0] = s_mmi_resource_array.CoverImg[MP3_INFO_ARRAY_NUM - 1];
      
        SCI_MEMCPY(p_mp3_info[MP3_INFO_ARRAY_NUM - 1], &temp_mp3_info, sizeof(MMIWIDGET_MP3_INFO_T)); 
        s_mmi_resource_array.CoverImg[MP3_INFO_ARRAY_NUM - 1] = temp_object;

        UpdatePlayListItem(p_mp3_info[MP3_INFO_ARRAY_NUM - 1], - 1);  
    }
    
    DecodeAlbumPicture(p_mp3_info, 0);
    MMIWIDGET_DEBUG(LOG_STR_LEAVE("time = %d"),   SCI_GetTickCount());
    return 0;
}


LOCAL BOOLEAN UpdatePlayListItem(MMIWIDGET_MP3_INFO_T *p_mp3_info, int32 i)
{     
    BOOLEAN result = 0;    
    p_mp3_info->path_length = MMIFILE_FULL_PATH_MAX_LEN;
    result = GetPlayListItemPath(p_mp3_info->path, &p_mp3_info->path_length, i); 
    
    p_mp3_info->valid_flag = 0;  
    SCI_FREE(p_mp3_info->album_info.img_data_ptr);    
    return result;
}

LOCAL BOOLEAN GetPlayListItemPath(wchar *p_path, uint32 *p_path_length, int32 offset)
{
    BOOLEAN result = 0;
    uint32 current_index = MMIMP3_GetCurPlayFileIndex();
    uint32 total_num = MMIMP3_GetCurPlayListFileNum();
    wchar full_path[MMIFILE_FULL_PATH_MAX_LEN +1] = {0};
	uint16 full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint32 index = 0;

    index = (current_index + offset + total_num) % total_num; /*lint !e737 */
    result = MMIMP3_GetCurPlayListFileInfo(index, full_path, &full_path_len);
    SCI_MEMSET(p_path, 0 , *p_path_length*sizeof(wchar));

    if(result)
    {
        MMI_WSTRNCPY(p_path,*p_path_length,full_path,full_path_len,
            full_path_len);
        *p_path_length = full_path_len;
    }
    else
    {
        *p_path_length = 0;
    }

    return result;
}

LOCAL BOOLEAN DecodeAlbumPicture(MMIWIDGET_MP3_INFO_T  *p_mp3_info[], int32 index)
{   
    BOOLEAN result =  0 ;
    int32 i = 0 ;
    MMIMP3_ALBUM_INFO_T temp_album_info = {0};
    MMIWIDGET_DEBUG(LOG_STR("time = %d"),  SCI_GetTickCount());                    
    for(i = index; i < MP3_INFO_ARRAY_NUM ; i++)
    {
        SCI_ASSERT(1 != p_mp3_info[i]->valid_flag);     /*assert verified*/
        if(2 == p_mp3_info[i]->valid_flag)
        {
            continue;
        }
        MMIWIDGET_DEBUG(LOG_STR(" index = %d "),  i); 
        SCI_ASSERT(0 == p_mp3_info[i]->album_info.img_data_ptr);    /*assert verified*/
        if(MMIMP3_GetAlbumInfo(p_mp3_info[i]->path, p_mp3_info[i]->path_length, &p_mp3_info[i]->album_info))
        {
            if(0 != p_mp3_info[i]->album_info.img_data_ptr
                && 0 != p_mp3_info[i]->album_info.data_size )
            {
                result = SetImgAnimData(&p_mp3_info[i]->album_info);
                p_mp3_info[i]->valid_flag = 1;  //解码过程中
                break;
            } 
        }        
        //p_mp3_info[i]->valid_flag = 2;  //没有专辑图片，使用默认图片替代
    }
    if(MP3_INFO_ARRAY_NUM == i)
    {        
        SetImgAnimData(&temp_album_info);
    }
    return result;
}

LOCAL int32 CreateAlbumPictureResource(MMI_WIN_ID_T  win_id, MMIWIDGET_MP3_INFO_T  *p_mp3_info,
                                       int32 index, GUIIMG_BITMAP_T *p_img_bmp)
{   
    uint16 image_width = 0;
    uint16 image_height = 0;
    uint32* p_buffer = 0;
    
    GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_WIDGET_MP3_SE_COVERBG,win_id);    
    p_buffer = MMIWIDGET_CreatePictureResource(win_id, IMAGE_WIDGET_MP3_SE_COVERBG);
    
    //SET BMP OBJ
    SCI_ASSERT(0 == s_mmi_resource_array.CoverImg[index].srcARGB_Map);      /*assert verified*/
    s_mmi_resource_array.CoverImg[index].srcARGB_Map = p_buffer;
    s_mmi_resource_array.CoverImg[index].up_window.h = image_height;
    s_mmi_resource_array.CoverImg[index].up_window.w = image_width;         
    
    return 0;
}

LOCAL int32 DestroyAlbumPictureResource(MMIWIDGET_MP3_INFO_T  *p_mp3_info,int32 index)
{   
    SCI_FREE(s_mmi_resource_array.CoverImg[index].srcARGB_Map);    
    if(0 != p_mp3_info)
    {
        p_mp3_info->valid_flag = 0;        
        SCI_FREE(p_mp3_info->album_info.img_data_ptr);    
    }
    return 0;
}

LOCAL int32 UpdateAlbumPictureResource(MMI_WIN_ID_T  win_id, MMIWIDGET_MP3_INFO_T  *p_mp3_info,
                                       int32 index, GUIIMG_BITMAP_T *p_img_bmp)
{   
    uint16 image_width = 0;
    uint16 image_height = 0;
    UILAYER_CREATE_T create_info = {0};
    GUI_LCD_DEV_INFO layer_handle = {0};
    uint32* p_buffer = 0;
    uint8*      p_layer_buffer = 0;
    uint32      buffer_size = 0;
    GUI_RECT_T  rect = {0};    
    GUI_RECT_T  fill_rect = {0};
    GUI_POINT_T point = {0};    
    
    if(1 != p_mp3_info->valid_flag)
    {
        return -1;
    }
    
    GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_WIDGET_MP3_SE_COVERBG,win_id);    
    
    if(0 != p_img_bmp)
    {
        DestroyAlbumPictureResource(p_mp3_info, index);
        
        
        //create layer    
        create_info.lcd_id          = GUI_MAIN_LCD_ID;
        create_info.owner_handle    = MMK_GetParentWinHandle(win_id);
        create_info.offset_x        = 0; 
        create_info.offset_y        = 0;
        create_info.width           = image_width;
        create_info.height          = image_height;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;
        
        UILAYER_CreateLayer(&create_info, &layer_handle);
        p_layer_buffer = UILAYER_GetLayerBufferPtr(&layer_handle);
        if(0 == p_layer_buffer)
        {        
            UILAYER_RELEASELAYER(&layer_handle);
            return -2;
        }
        
        buffer_size = image_width*image_height*4 ;
        p_buffer = SCI_ALLOCA(buffer_size);
        if(0 == p_buffer)
        {
            //release layer        
            UILAYER_RELEASELAYER(&layer_handle);
            return -3;
        }
        
        //draw image
        UILAYER_Clear(&layer_handle);
        rect = GUI_CreateRect(0,0, image_width - 1, image_height - 1);
        fill_rect = GUI_GetCenterRect(rect, 118, 118);
        
        point.x = (rect.left + rect.right + 1 - p_img_bmp->img_width) / 2;
        point.y = (rect.top + rect.bottom + 1 - p_img_bmp->img_height) / 2;
        
        LCD_FillRect(&layer_handle, fill_rect, MMI_BLACK_COLOR);
        GUIIMG_DisplayBmp(FALSE, &rect, &point, p_img_bmp, &layer_handle); 
        //SCI_FREE(p_mp3_info->album_info.img_data_ptr);
        p_mp3_info->valid_flag = 2;            
        GUIRES_DisplayImg(0, &rect,  0,  win_id,  IMAGE_WIDGET_MP3_SE_COVERBG_FRAME, &layer_handle);
        
        //copy layer to new buffer
        SCI_MEMCPY(p_buffer, p_layer_buffer, buffer_size);            
        UILAYER_RELEASELAYER(&layer_handle);

        //tranform to PMA888
        MMI_ARGB888TOPMARGB888( (uint32*)p_buffer, buffer_size);
    }   
    else
    {
        WidgetMusicPlayer_PicLoadingAnimat(&s_loading_resource_array);
        p_buffer = s_mmi_resource_array.CoverImg[index].srcARGB_Map ;
        buffer_size = 4*s_mmi_resource_array.CoverImg[index].up_window.h * s_mmi_resource_array.CoverImg[index].up_window.h;
        SCI_MEMCPY(p_buffer, s_loading_resource_array.loading_pOutBuf->srcARGB_Map, buffer_size);
    }

    
    //SET BMP OBJ
    //SCI_FREE(s_mmi_resource_array.CoverImg[index].srcARGB_Map);
    s_mmi_resource_array.CoverImg[index].srcARGB_Map = p_buffer;
    s_mmi_resource_array.CoverImg[index].up_window.h = image_height;
    s_mmi_resource_array.CoverImg[index].up_window.w = image_width;         
    
    return 0;
}

/*
LOCAL int32 CreateAlbumInfoResource(MMIWIDGET_MP3_INFO_T  *p_mp3_info,
                                       int32 index)
{   
    uint16 image_width = 0;
    uint16 image_height = 0;
    UILAYER_CREATE_T create_info = {0};
    GUI_LCD_DEV_INFO layer_handle = {0};
    uint32* p_buffer = 0;
    uint8*      p_layer_buffer = 0;
    uint32      buffer_size = 0;
    MMI_WIN_ID_T win_id = MAIN_IDLE_WIN_ID;
    GUI_RECT_T  rect = {0};
    GUI_POINT_T point = {0};
        GUIRES_GetImgWidthHeight(&image_width, &image_height, IMAGE_WIDGET_MP3_BG,win_id);    
        image_width -= 20;
        image_height /= 3;
        //create layer    
        create_info.lcd_id          = GUI_MAIN_LCD_ID;
        create_info.owner_handle    = win_id;
        create_info.offset_x        = 0; 
        create_info.offset_y        = 0;
        create_info.width           = image_width;
        create_info.height          = image_height;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;
        
        UILAYER_CreateLayer(&create_info, &layer_handle);
        p_layer_buffer = UILAYER_GetLayerBufferPtr(&layer_handle);
        if(0 == p_layer_buffer)
        {        
            UILAYER_RELEASELAYER(&layer_handle);
            return -2;
        }

        buffer_size = image_width*image_height*4 ;
        p_buffer = SCI_ALLOCA(buffer_size);
        if(0 == p_buffer)
        {
            //release layer        
            UILAYER_RELEASELAYER(&layer_handle);
            return -3;
        }

        //draw image
        UILAYER_Clear(&layer_handle);
        rect = GUI_CreateRect(0,0, image_width - 1, image_height - 1);

        //draw text
        DisplayMp3AlbumInfo(win_id, &layer_handle, &rect, &p_mp3_info->album_info);
        
        //tranform to PMA888
        MMI_ARGB888TOPMARGB888( (uint32*)p_layer_buffer, buffer_size);

        
        //copy layer to new buffer
        SCI_MEMCPY(p_buffer, p_layer_buffer, buffer_size);
        
        //SET BMP OBJ
        //SCI_FREE(s_mmi_resource_array.CoverImg[index].srcARGB_Map);
        s_mmi_resource_array.CoverImgHeadline[index].srcARGB_Map = p_buffer;
        s_mmi_resource_array.CoverImgHeadline[index].up_window.h = image_height;
        s_mmi_resource_array.CoverImgHeadline[index].up_window.w = image_width;         
    
    UILAYER_RELEASELAYER(&layer_handle);
    return 0;
}

LOCAL int32 DestroyAlbumInfoResource(MMIWIDGET_MP3_INFO_T  *p_mp3_info,int32 index)
{   
    SCI_FREE(s_mmi_resource_array.CoverImgHeadline[index].srcARGB_Map);    
    return 0;
}
*/
LOCAL int32 UpdateImageBuffer(MMI_WIN_ID_T  win_id, MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se)
{   
    MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};	
    MMIAPIMP3_GetPlayingDynInfo(&dyn_info,FALSE);
    s_resource_array.IsPlaying = MMIAPIMP3_IsPlayerPlaying();
    if(0 == dyn_info.total_time)
    {
        s_resource_array.PlayPercentage = 0;        
    }
    else
    {
        s_resource_array.PlayPercentage = dyn_info.cur_time * 100 / dyn_info.total_time;
    }
    return 0;
}

LOCAL int32 CreateImageBuffer(MMI_WIN_ID_T    win_id, MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se)
{    
    int32 i = 0;
    int32 image_num = sizeof(s_image_array) / sizeof(s_image_array[0]);
    uint16 image_width = 0;
    uint16 image_height = 0;
    uint32* p_buffer = 0;
    OBJECT_T *p_bmp_obj = 0;    

    //SCI_ASSERT(image_num == sizeof(s_image_array) / sizeof(s_image_array[0]));  /*lint !e737 */
    for(i = 0; i < image_num; i++)
    {
        GUIRES_GetImgWidthHeight(&image_width, &image_height, s_image_array[i],win_id);    
        p_buffer = MMIWIDGET_CreatePictureResource(win_id, s_image_array[i]);
        
        //SET BMP OBJ
        p_bmp_obj = (OBJECT_T*)&s_mmi_resource_array + i;
        p_bmp_obj->srcARGB_Map = (uint32*)p_buffer;
        p_bmp_obj->up_window.w = image_width;
        p_bmp_obj->up_window.h = image_height;
        *((OBJECT_T**)&s_resource_array + i) = p_bmp_obj;       
    }    

    image_num = sizeof(s_loading_image_array) / sizeof(s_loading_image_array[0]);
    for(i = 0; i < image_num; i++)
    {
        GUIRES_GetImgWidthHeight(&image_width, &image_height, s_loading_image_array[i],win_id);    
        p_buffer = MMIWIDGET_CreatePictureResource(win_id, s_loading_image_array[i]);
        
        //SET BMP OBJ
        p_bmp_obj = (OBJECT_T*)&s_mmi_loading_resource_array + i;
        p_bmp_obj->srcARGB_Map = (uint32*)p_buffer;
        p_bmp_obj->up_window.w = image_width;
        p_bmp_obj->up_window.h = image_height;
        *((OBJECT_T**)&s_loading_resource_array + i) = p_bmp_obj;       
    } 
    GUIRES_GetImgWidthHeight(&image_width, &image_height, s_loading_image_array[0],win_id);    
    p_buffer = MMIWIDGET_CreatePictureResource(win_id, s_loading_image_array[0]);
    
    //SET BMP OBJ
    p_bmp_obj = (OBJECT_T*)&s_mmi_loading_resource_array + i;
    p_bmp_obj->srcARGB_Map = (uint32*)p_buffer;
    p_bmp_obj->up_window.w = image_width;
    p_bmp_obj->up_window.h = image_height;
    *((OBJECT_T**)&s_loading_resource_array + i) = p_bmp_obj;       
    

    CreatePlayList(sp_mp3_info);
    s_mmi_resource_array.CoverImgNum = UpdatePlayList(sp_mp3_info);
    if(0 == s_mmi_resource_array.CoverImgNum)
    {
        s_mmi_resource_array.CoverImgNum = MP3_INFO_ARRAY_NUM; 
    }
    for(i = 0; i < MP3_INFO_ARRAY_NUM; i++)   /* lint !e574 !e737 */
    {
        CreateAlbumPictureResource(win_id,  sp_mp3_info[i], i , 0);
    } 
  
    //s_mmi_resource_array.CoverImgNum = MP3_SE_DISPLAY_NUM;
    s_resource_array.CoverImgNum = s_mmi_resource_array.CoverImgNum;
    s_resource_array.CoverImg = s_mmi_resource_array.CoverImg;    
    s_resource_array.CoverImgHeadline = s_mmi_resource_array.CoverImgHeadline;    
    return 0;
}

LOCAL int32 DestroyImageBuffer(MMI_WIN_ID_T    win_id, MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se)
{
    int32 i = 0;
    int32 resource_num = sizeof(s_image_array) / sizeof(s_image_array[0]);
    OBJECT_T *p_buffer = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    
    for(i = 0; i < resource_num; i++)
    {
        p_buffer = (OBJECT_T*)&s_mmi_resource_array + i;
        SCI_FREE(p_buffer->srcARGB_Map);
    }
    
    resource_num = sizeof(s_mmi_loading_resource_array) / sizeof(s_mmi_loading_resource_array.LoadingBG);
    for(i = 0; i < resource_num; i++)
    {
        p_buffer = (OBJECT_T*)&s_mmi_loading_resource_array + i;
        SCI_FREE(p_buffer->srcARGB_Map);
    } 

    DestroyPlayList(sp_mp3_info);
    
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    UILAYER_SetLayerMemFormat(&lcd_dev_info, UILAYER_MEM_IMMUTABLE);
    return 0;
}

LOCAL int32 GetAnimationTotalFrame(MMIWIDGET_MP3_SE_ANIM_TYPE_E anim_type)
{    
    int32 total_frame = 10;
    switch(anim_type)
    {
        case MMIWIDGET_MP3_SE_ANIM_SHOW_IN:
            total_frame *= 3;
        break;        
        
        case MMIWIDGET_MP3_SE_ANIM_SWITCH_ON_PAUSE:
            total_frame *= 1;
        break;
        
        case MMIWIDGET_MP3_SE_ANIM_SWITCH_ON_PLAY:
            total_frame *= 2;
        break;
/*
        case MMIWIDGET_MP3_SE_ANIM_PAUSE2PLAY:
        case MMIWIDGET_MP3_SE_ANIM_PLAY2PAUSE:
        case MMIWIDGET_MP3_SE_ANIM_PLAY:
        break;
*/        
        default:
        break;
    }
    return total_frame;
}

LOCAL int32 IsPlayAnimation( MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se)
{
    MMIWIDGET_MP3_SE_ANIM_PARA_T *p_anim_para = (MMIWIDGET_MP3_SE_ANIM_PARA_T*)(p_widget_se->user_data); 
    
    if(-1 == p_widget_se->current_frame
        || 0 == p_widget_se->current_frame
        || p_widget_se->current_frame == GetAnimationTotalFrame(p_anim_para->anim_type))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

LOCAL int32 Display(MMI_WIN_ID_T  win_id, MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se)
{
    GUI_RECT_T win_rect = {0};    
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    MMIWIDGET_MP3_SE_ANIM_PARA_T *p_anim_para = (MMIWIDGET_MP3_SE_ANIM_PARA_T*)(p_widget_se->user_data);
    BOOLEAN is_goback = FALSE;
    IMGREF_SIZE_T window = {0};
    uint32 *pColor = 0;
    int32 total_frame = 10;
    int32 i =0;
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    UILAYER_SetLayerMemFormat(&lcd_dev_info, UILAYER_MEM_FORMAT_DOUBLE);
    UILAYER_SetPreMultiplyAlpha(&lcd_dev_info,  TRUE,  FALSE);
    UILAYER_SwitchDoubleMem( &lcd_dev_info, TRUE );
    UILAYER_SetDirectDraw(TRUE);  
    
    UILAYER_Clear(&lcd_dev_info);
    pColor =  (uint32*)UILAYER_GetLayerBufferPtr(&lcd_dev_info);

    if(-1 == p_widget_se->current_frame)
    {
        p_anim_para->anim_type = MMIWIDGET_MP3_SE_ANIM_SHOW_IN;
        p_widget_se->current_frame = 0;
    }

    if(-1 == p_anim_para->switch_type)
    {
        is_goback = TRUE;
    } 

    for(i = 0; i < MP3_INFO_ARRAY_NUM; i++)
    {
       UpdateAlbumPictureResource(win_id, sp_mp3_info[i], i, 0);
    }
    //TurnTableShow(window, pColor, &s_resource_array, p_anim_para->anim_type, is_goback);
    total_frame = GetAnimationTotalFrame(p_anim_para->anim_type);
    window.w = win_rect.right - win_rect.left + 1;
    window.h =  win_rect.bottom - win_rect.top + 1;

    switch(p_anim_para->anim_type)
    {
        case MMIWIDGET_MP3_SE_ANIM_SHOW_IN:   
            if(p_widget_se->current_frame == total_frame)
            {                
                if(1 == s_anim_para.switch_type)
                {
                    SwitchPlayList(sp_mp3_info, s_anim_para.switch_type);                     
                    s_anim_para.switch_type = 0;                    
                }
            }
            if(p_widget_se->current_frame == total_frame - 1)
            {
                DecodeAlbumPicture(sp_mp3_info, 0);
            }
            
            WidgetMusicPlayer_ShowIn(p_widget_se->current_frame,total_frame,window,pColor,&s_resource_array);
            if(p_widget_se->current_frame >= total_frame)
            {   
                if(MMIAPIMP3_IsPlayerPlaying())
                {           
                    p_widget_se->current_frame = 0;
                    p_anim_para->anim_type = MMIWIDGET_MP3_SE_ANIM_PAUSE2PLAY;
                }
            }
            else
            {                
                p_widget_se->current_frame++;
            }
         break;

         case MMIWIDGET_MP3_SE_ANIM_PAUSE2PLAY:
             WidgetMusicPlayer_Pause2Play(p_widget_se->current_frame,total_frame,window,pColor,&s_resource_array);             
             if(p_widget_se->current_frame >= total_frame)
             {                
                 if(MMIAPIMP3_IsPlayerPlaying())
                 {                    
                     p_widget_se->current_frame = 0;
                     p_anim_para->anim_type = MMIWIDGET_MP3_SE_ANIM_PLAY;
                 }
                 else
                 {
                     p_widget_se->current_frame = total_frame;
                     p_anim_para->anim_type = MMIWIDGET_MP3_SE_ANIM_PLAY2PAUSE;
                 }
             }
             else
             {
                 p_widget_se->current_frame++;             
             }
         break;   
         
         case MMIWIDGET_MP3_SE_ANIM_PLAY2PAUSE:
             WidgetMusicPlayer_Pause2Play(p_widget_se->current_frame,total_frame,window,pColor,&s_resource_array);             
             if(p_widget_se->current_frame == 0)
             {                
                 p_widget_se->current_frame = 0;
                 if(MMIAPIMP3_IsPlayerPlaying())
                 {                    
                     p_anim_para->anim_type = MMIWIDGET_MP3_SE_ANIM_PAUSE2PLAY;
                 }         
             }
             else
             {
                 p_widget_se->current_frame--;             
             }
         break; 
         
         case MMIWIDGET_MP3_SE_ANIM_PLAY:
             if(1 == s_anim_para.switch_type)
             {
                 SwitchPlayList(sp_mp3_info, s_anim_para.switch_type);                     
                 s_anim_para.switch_type = 0;                     
             }
             WidgetMusicPlayer_Play(window,pColor,&s_resource_array);               
             if(!MMIAPIMP3_IsPlayerPlaying())
             {
                 p_anim_para->anim_type = MMIWIDGET_MP3_SE_ANIM_PLAY2PAUSE;
                 p_widget_se->current_frame = total_frame;
             }
         break;
         
         case MMIWIDGET_MP3_SE_ANIM_SWITCH_ON_PAUSE:
             WidgetMusicPlayer_PauseCDChoose(p_widget_se->current_frame,total_frame,is_goback,window,pColor,&s_resource_array);
             if(p_widget_se->current_frame >= total_frame)
             {                
                 p_anim_para->anim_type = MMIWIDGET_MP3_SE_ANIM_SHOW_IN;
                 p_widget_se->current_frame =  GetAnimationTotalFrame(p_anim_para->anim_type);
             }
             else
             {
                 p_widget_se->current_frame++;             
             }
             break;
             
         case MMIWIDGET_MP3_SE_ANIM_SWITCH_ON_PLAY:
             WidgetMusicPlayer_PlayCDChoose(p_widget_se->current_frame,total_frame,is_goback,window,pColor,&s_resource_array);
             if(p_widget_se->current_frame >= total_frame)
             {                
                 if(MMIAPIMP3_IsPlayerPlaying())
                 {      
                     p_widget_se->current_frame = 0;
                     p_anim_para->anim_type = MMIWIDGET_MP3_SE_ANIM_PLAY;
                 }    
             }
             else
             {
                 p_widget_se->current_frame++;             
             }

             break;

        default:  
            WidgetMusicPlayer_Play(window,pColor,&s_resource_array);
        break;
    }
    
    return 0;
}


#endif



#endif

