/*****************************************************************************
** File Name:      mmiidle_screensaver.c                                     *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2007      Jassmine              Creat
******************************************************************************/

#define _MMIIDLE_SCREENSAVER_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_screensaver_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmiss_export.h"
#include "mmiss_special.h"
#include "guianim.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmk_timer.h"
#include "mmi_default.h"
#include "mmi_anim.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#include "mmi_appmsg.h"
#include "mmimp3_export.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif

#ifdef SCREENSAVER_SUPPORT
//extern BOOLEAN      g_is_mp3_playing_bg;//是否有mp3背景播放
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIILDESS_TIME_OUT          12000   //screen saver time
//#define MMIILDESS_TIME_OUT  2000
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

 /**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL uint8               g_ssaver_time_id = 0;   //屏保定时器

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : to handle the screen saver window message
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleScreenSaverWinMsg(
                                           MMI_WIN_ID_T         win_id, 
                                           MMI_MESSAGE_ID_E     msg_id, 
                                           DPARAM               param
                                           );

/*****************************************************************************/
// 	Description : SetDefaultSecreenSaver
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void SetDefaultSecreenSaver(MMI_WIN_ID_T win_id , MMI_CTRL_ID_T ctrl_id);

#ifdef VIDEO_PLAYER_SUPPORT
/*****************************************************************************/
// 	Description : HandleVideoScreenSaverMsg
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void HandleVideoScreenSaverMsg(VP_CALLBACK_T *msg_ptr);
#endif

/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
// the window for screen saver 
WINDOW_TABLE(MMIIDLE_SCREENSAVER_WIN_TAB) = 
{
	//CLEAR_LCD,
	WIN_PRIO( WIN_LOWEST_LEVEL ),
	WIN_FUNC( (uint32)HandleScreenSaverWinMsg ),   //MS00209550 
	WIN_ID( MMIIDLE_SCREENSAVER_WIN_ID ),
//#ifdef MMI_PDA_SUPPORT
    WIN_HIDE_STATUS,
//#endif
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
	END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : start screen saver timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
// PUBLIC uint8 MMIAPIIDLESS_StartTimer(
//                               MMI_WIN_ID_T  win_id
//                               )
// {
//     uint8                       timer_id        = 0;
//     MMISET_SCREENSAVER_INFO_T   ssaver_info     = {0};
//     
//     //get screen saver info
//     MMIAPISET_GetScreensaverInfo(&ssaver_info);
// 
//     //screen saver is open
//     if (ssaver_info.is_open)
//     {
//         timer_id = MMK_CreateWinTimer(win_id,MMIILDESS_TIME_OUT,FALSE);
//     }
// 
//     return (timer_id);
// }

/*****************************************************************************/
//  Description : stop screen saver timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
// PUBLIC void MMIAPIIDLESS_StopTimer(void)
// {
//     if (0 != g_ssaver_time_id)
//     {
//         MMK_StopTimer(g_ssaver_time_id);
//         g_ssaver_time_id = 0;
//     }
// }


/*****************************************************************************/
//  Description : restart screen saver timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
// PUBLIC void MMIDLE_RestartSsTimer(
//                                   MMI_WIN_ID_T  win_id
//                                   )
// {
//     MMIAPIIDLESS_StopTimer();
//     g_ssaver_time_id = MMIAPIIDLESS_StartTimer(win_id);
// }

/*****************************************************************************/
//  Description : is screen saver timer id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
// PUBLIC BOOLEAN MMIAPIIDLESS_IsScreenSaverTimerId(DPARAM param)
// {
//     if (g_ssaver_time_id == *(uint8*)param)
//     {
//         return TRUE;
//     }
//     else
//     {
//         return FALSE;
//     }
// }
/*****************************************************************************/
// 	Description : open screen saver
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIDLESS_OpenScreenSaver(void)
{
    //if current back light is on
    if (MMIDEFAULT_IsBacklightOn()&&MMIDEFAULT_GetFlipStatus())/*开盖*/
    { 
        MMISET_SCREENSAVER_INFO_T  ssaver_info = {0};
        MMIAPISET_GetScreensaverInfo(&ssaver_info);

        if(ssaver_info.is_open 
//#if !defined MMI_PDA_SUPPORT
//            &&(!MMIAPIMP3_IsPlayerActive())
//#endif
            )
        {
            if (MMISET_SCREENSAVER_STAR <= ssaver_info.ssaver_type
                && MMISET_SCREENSAVER_SEED >= ssaver_info.ssaver_type)
            {
                //算法屏保
                MMIAPIIDLESS_OpenSpecialScreenSaver(MMIAPIIDLESS_ConvertSCRSaverType(ssaver_info.ssaver_type));
            }
            else
            {
                MMK_CreateWin((uint32*)MMIIDLE_SCREENSAVER_WIN_TAB, PNULL);
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPIIDLESS_OpenScreenSaver,open saver win fail! "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISS_SCREENSAVER_200_112_2_18_2_49_45_2,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPIIDLESS_OpenScreenSaver: backligt is close"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISS_SCREENSAVER_205_112_2_18_2_49_45_3,(uint8*)"");
    }
}

/*****************************************************************************/
// 	Description : close screen saver
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIDLESS_CloseScreenSaver(void)
{
    MMISET_SCREENSAVER_INFO_T   ssaver_info  = {0};

    if(MMK_IsOpenWin(MMIIDLE_SCREENSAVER_WIN_ID))
    { 
        MMIAPISET_GetScreensaverInfo(&ssaver_info);
#ifdef VIDEO_PLAYER_SUPPORT        
        if(MMISET_SCREENSAVER_MORE_ANIMATION == ssaver_info.ssaver_type)
        {
            MMIAPIVP_ExitPlay();
        }
#endif        
        MMK_CloseWin(MMIIDLE_SCREENSAVER_WIN_ID);
        MMK_SendMsg(MAIN_IDLE_WIN_ID, MSG_FULL_PAINT, PNULL);   
    }
}


/*****************************************************************************/
// 	Description : To judge whether the screensaver is mpeg4 type.
//	Global resource dependence : 
//  Author: xingdong.li
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLESS_IsScreenSaverMp4Type(void)
{

    BOOLEAN                     result      = FALSE;
    MMISET_SCREENSAVER_INFO_T   ssaver_info = {0};
    MMIAPISET_GetScreensaverInfo(&ssaver_info);
#ifdef VIDEO_PLAYER_SUPPORT        
    if (MMISET_SCREENSAVER_MORE_ANIMATION == ssaver_info.ssaver_type)
    {
        result = TRUE;
    }
#endif        
    return result;
}


/*****************************************************************************/
// 	Description : Reset ssaver to default.
//	Global resource dependence : 
//  Author: xingdong.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIDLESS_SetDefaultScreenSaver(void)
{
    // BOOLEAN                     result      = FALSE;
    MMISET_SCREENSAVER_INFO_T   ssaver_info = {0};
    
    //SCI_TRACE_LOW:"[MMIMPEG4]: MMIAPIIDLESS_SetDefaultScreenSaver, ENTERS!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISS_SCREENSAVER_261_112_2_18_2_49_45_4,(uint8*)"");

    ssaver_info.is_open = TRUE;
    ssaver_info.ssaver_type = MMISET_SCREENSAVER_ANIM1;

    MMIAPISET_SetScreenSaverInfo(&ssaver_info);
}
#ifdef VIDEO_PLAYER_SUPPORT
/*****************************************************************************/
// 	Description : HandleVideoScreenSaverMsg
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void HandleVideoScreenSaverMsg(VP_CALLBACK_T *msg_ptr)
{
    VP_PLAY_END_IND_T  *play_end_ind_ptr = PNULL;
    VP_PLAY_END_RESULT_E play_end_result = 0;
    GUI_RECT_T    screen_rect = MMITHEME_GetFullScreenRect();
    if (PNULL == msg_ptr)
    {
        return;
    }
    

    //SCI_TRACE_LOW:"[VP] HandleVideoScreenSaverMsg msg_ptr->msg_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISS_SCREENSAVER_284_112_2_18_2_49_45_5,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id) 
    {
    case VP_PLAY_END_IND:
        play_end_ind_ptr = (VP_PLAY_END_IND_T *)msg_ptr->para;        
        play_end_result = play_end_ind_ptr->result;        
        MMITHEME_StoreUpdateRect((GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev(),screen_rect);
    	break;
        
    case VP_STOP_IND: 
        play_end_result = VP_PLAY_END_STOPPED_BY_USER;           
        break;

    case VP_PLAY_PROCESS_IND:
        MMITHEME_UpdateRect();
        return;

    default:
        return;
    }
    MMK_PostMsg(MMIIDLE_SCREENSAVER_WIN_ID,MSG_MPEG4_END_CALLBACK,&play_end_result,sizeof(VP_PLAY_END_RESULT_E));

}
#endif

/*****************************************************************************/
// 	Description : SetDefaultSecreenSaver
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL void SetDefaultSecreenSaver(MMI_WIN_ID_T win_id , MMI_CTRL_ID_T ctrl_id)
{
    GUIANIM_CTRL_INFO_T                 ctrl_info = {0};
    GUIANIM_DISPLAY_INFO_T              display_info = {0};
    MMI_CONTROL_CREATE_T                anim_ctrl = {0};
    GUIANIM_INIT_DATA_T                 anim_init = {0};
    GUIANIM_DATA_INFO_T                 data_info = {0};
    
    GUI_BOTH_RECT_T anim_rect = MMITHEME_GetFullScreenBothRect();
    
    ctrl_info.is_ctrl_id = TRUE;
    ctrl_info.ctrl_id = ctrl_id;
    
    data_info.img_id = IMAGE_SCREEN_SAVER_1;
    
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.bg.bg_type  = GUI_BG_COLOR;
    display_info.bg.color    = MMI_WINDOW_BACKGROUND_COLOR;
    
    anim_init.both_rect = anim_rect;
    
    //creat anim
    anim_ctrl.ctrl_id           = ctrl_info.ctrl_id;
    anim_ctrl.guid              = SPRD_GUI_ANIM_ID;
    anim_ctrl.init_data_ptr     = &anim_init;
    anim_ctrl.parent_win_handle = win_id;
    MMK_CreateControl(&anim_ctrl);
    
    GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
}
/*****************************************************************************/
// 	Description : to handle the screen saver window message
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleScreenSaverWinMsg(
                                           MMI_WIN_ID_T         win_id, 
                                           MMI_MESSAGE_ID_E     msg_id, 
                                           DPARAM               param
                                           )
{
    MMI_RESULT_E                        result = MMI_RESULT_TRUE;
    GUIANIM_INIT_DATA_T                 anim_init = {0};
    MMI_CONTROL_CREATE_T                anim_ctrl = {0};
    GUIANIM_CTRL_INFO_T                 ctrl_info = {0};
    GUIANIM_FILE_INFO_T             	file_info = {0};
    GUIANIM_DISPLAY_INFO_T              display_info = {0};
    static MMISET_SCREENSAVER_INFO_T    ssaver_info = {0};
#ifdef VIDEO_PLAYER_SUPPORT
    VP_PLAY_PARAM_T                     play_param  = {0};
    GUI_RECT_T                          full_rect = {0};
#endif
   
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //get screen saver info
        MMIAPISET_GetScreensaverInfo(&ssaver_info);
        if(MMISET_SCREENSAVER_CLOSED == ssaver_info.ssaver_type)   //@xingdong.li
        {
            MMK_CloseWin(win_id);
			break;
        }
#ifdef VIDEO_PLAYER_SUPPORT
        else if(MMISET_SCREENSAVER_MORE_ANIMATION == ssaver_info.ssaver_type)       
        {
            MMIDEFAULT_EnableKeyRing("SCREEN SAVER", FALSE);
            MMIDEFAULT_EnableTpRing("SCREEN SAVER", FALSE);

#ifdef DRM_SUPPORT
            {
                if(MMIAPIDRM_IsDRMFile(SFS_INVALID_HANDLE ,ssaver_info.more_ani_saver_info.name_wstr))
                {
                    if (!MMIAPIDRM_IsRightsValid(SFS_INVALID_HANDLE, ssaver_info.more_ani_saver_info.name_wstr, TRUE, DRM_PERMISSION_PLAY))
                    {
                        {
                            ssaver_info.ssaver_type = MMISET_SCREENSAVER_ANIM1;
                            MMIAPISET_SetScreenSaverInfo(&ssaver_info);                               
                        }//mary fix cr114157
                        SetDefaultSecreenSaver(win_id , MMIIDLESS_ANIM_CTRL_ID);
                        break;
                    }
                }
            }
#endif
            play_param.dual_sys = MN_DUAL_SYS_1;
            play_param.full_path_len = ssaver_info.more_ani_saver_info.name_wstr_len;
            play_param.full_path_ptr = ssaver_info.more_ani_saver_info.name_wstr;
            play_param.is_random_play = FALSE;
            play_param.is_scal_up = TRUE;
            play_param.mode = VP_REPEAT_PLAY_MODE_ONE;
            play_param.play_type = VP_PLAY_FROM_FILE;
            play_param.url_ptr = PNULL;
            play_param.video_buf_info.type = 0;
            play_param.video_buf_info.video_buf_len = 0;
            play_param.video_buf_info.video_buf_ptr = PNULL;
            play_param.display_param.audioSupport = 0;
            play_param.display_param.videoSupport = 1;
            play_param.is_audio_coexist=TRUE;
          
            play_param.display_param.RotateAngle = GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID); 

            full_rect = MMITHEME_GetFullScreenRect(); 

            if (LCD_ANGLE_0 == play_param.display_param.RotateAngle)
            {
                play_param.display_param.RotateAngle = LCD_ANGLE_90;
                play_param.display_param.disp_rect.dy = full_rect.right - full_rect.left + 1;           
                play_param.display_param.disp_rect.dx = full_rect.bottom - full_rect.top + 1;
            }
            else if(LCD_ANGLE_180 == play_param.display_param.RotateAngle)
            {
                play_param.display_param.RotateAngle = LCD_ANGLE_270;
                play_param.display_param.disp_rect.dy = full_rect.right - full_rect.left + 1;           
                play_param.display_param.disp_rect.dx = full_rect.bottom - full_rect.top + 1;
            }
            else
            {
                play_param.display_param.disp_rect.dx = full_rect.right - full_rect.left + 1;           
                play_param.display_param.disp_rect.dy = full_rect.bottom - full_rect.top + 1;
            }
            
            play_param.display_param.target_rect.dx = play_param.display_param.disp_rect.dx;
            play_param.display_param.target_rect.dy = play_param.display_param.disp_rect.dy;
            play_param.display_param.target_rect.x = play_param.display_param.disp_rect.x;
            play_param.display_param.target_rect.y = play_param.display_param.disp_rect.y;
            play_param.display_param.disp_mode = DPLAYER_DISP_CUSTOMIZE;

            play_param.vol = MMIAPISET_GetMultimVolume();
            //play_param.vol = 0;
            MMIAPISET_StopAllRing(FALSE); 
            if(!MMIAPIVP_Play(&play_param, HandleVideoScreenSaverMsg, 0))
            {
                MMK_CloseWin(win_id);

            }
			break;			
        }        
#endif
		else if(MMISET_SCREENSAVER_MORE_PICTURE == ssaver_info.ssaver_type )
		{
            GUI_BOTH_RECT_T anim_rect = MMITHEME_GetFullScreenBothRect();
#ifdef DRM_SUPPORT	
            {
                if(MMIAPIDRM_IsDRMFile(SFS_INVALID_HANDLE ,ssaver_info.more_pic_saver_info.name_wstr ))
                {
                    if (!MMIAPIDRM_IsRightsValid(SFS_INVALID_HANDLE, ssaver_info.more_pic_saver_info.name_wstr, TRUE, DRM_PERMISSION_DISPLAY))
                    {
                        {
                            ssaver_info.ssaver_type = MMISET_SCREENSAVER_ANIM1;
                            MMIAPISET_SetScreenSaverInfo(&ssaver_info);    
                        }//mary fix cr114157
                        SetDefaultSecreenSaver(win_id , MMIIDLESS_ANIM_CTRL_ID);
                        break;
                    }
                }
            }
#endif        
            ctrl_info.is_ctrl_id = TRUE;
            ctrl_info.ctrl_id = MMIIDLESS_ANIM_CTRL_ID;

            display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
            display_info.bg.bg_type  = GUI_BG_COLOR;
            display_info.bg.color    = MMI_WINDOW_BACKGROUND_COLOR;

            anim_init.both_rect = anim_rect;

            //creat anim
            anim_ctrl.ctrl_id           = ctrl_info.ctrl_id;
            anim_ctrl.guid              = SPRD_GUI_ANIM_ID;
            anim_ctrl.init_data_ptr     = &anim_init;
            anim_ctrl.parent_win_handle = win_id;
            MMK_CreateControl(&anim_ctrl);

            file_info.full_path_wstr_ptr = ssaver_info.more_pic_saver_info.name_wstr;
            file_info.full_path_wstr_len = ssaver_info.more_pic_saver_info.name_wstr_len;

			GUIANIM_SetParam(&ctrl_info,PNULL,&file_info,&display_info);
			break;
		}
        else	//default setting
        {       
			SetDefaultSecreenSaver(win_id , MMIIDLESS_ANIM_CTRL_ID);
        }
        break;

    case MSG_MPEG4_END_CALLBACK:
#ifdef VIDEO_PLAYER_SUPPORT
        switch(*(VP_PLAY_END_RESULT_E *)(param))
        {          
        case VP_PLAY_END_STOPPED_BY_USER:
            MMK_CloseWin(win_id);
            break;
            
        case VP_PLAY_END_ERROR:
            MMIAPIIDLESS_CloseScreenSaver();
            if(MMIAPIIDLESS_IsScreenSaverMp4Type())
            {
                MMIAPIIDLESS_SetDefaultScreenSaver();
            }
            MMIDEFAULT_TurnOnLCDBackLight();
            break;
            
        default:
            break;
        }

#endif
        break;
    
    case MSG_BACKLIGHT_TURN_OFF:
    case MSG_LOSE_FOCUS:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
#ifdef VIDEO_PLAYER_SUPPORT
        if(MMISET_SCREENSAVER_MORE_ANIMATION == ssaver_info.ssaver_type)
        {

            MMIAPIVP_ExitPlay();
            MMIDEFAULT_EnableKeyRing("SCREEN SAVER", TRUE);
            MMIDEFAULT_EnableTpRing("SCREEN SAVER", TRUE);
        }
#endif
        break;

    default:
        if (((msg_id >= MSG_KEYDOWN_UP) && 
            (msg_id <= MSG_KEYDOWN_MIDDLE))
            ||(MSG_KEYUP_SLIDE == msg_id)
            ||(MSG_KEYDOWN_TV == msg_id)
            )
        {
#ifdef LCD_SLIDE_SUPPORT
            if (MSG_KEYUP_SLIDE == msg_id)
            {
                MMIDEFAULT_HandleSlideKey(MSG_KEYUP_SLIDE); 
            }
#endif
#ifdef VIDEO_PLAYER_SUPPORT
        if(MMISET_SCREENSAVER_MORE_ANIMATION == ssaver_info.ssaver_type)
            {
                MMIAPIVP_ExitPlay();
            }
#endif
            MMK_CloseWin(win_id);
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;
    }

    return (result);
}
#else
/*****************************************************************************/
//  Description : start screen saver timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
// PUBLIC uint8 MMIAPIIDLESS_StartTimer(MMI_WIN_ID_T  win_id)
// {
//     return 0;
// }

/*****************************************************************************/
//  Description : stop screen saver timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
// PUBLIC void MMIAPIIDLESS_StopTimer(void)
// {
// 
// }
/*****************************************************************************/
// 	Description : open screen saver
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIDLESS_OpenScreenSaver(void)
{
}

/*****************************************************************************/
// 	Description : close screen saver
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIDLESS_CloseScreenSaver(void)
{

}

/*****************************************************************************/
// 	Description : To judge whether the screensaver is mpeg4 type.
//	Global resource dependence : 
//  Author: xingdong.li
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLESS_IsScreenSaverMp4Type(void)
{
    return FALSE;
}

/*****************************************************************************/
// 	Description : Reset ssaver to default.
//	Global resource dependence : 
//  Author: xingdong.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIDLESS_SetDefaultScreenSaver(void)
{

}

/*****************************************************************************/
//  Description : restart screen saver timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
// PUBLIC void MMIDLE_RestartSsTimer(
//                                   MMI_WIN_ID_T  win_id
//                                   )
// {
// }

/*****************************************************************************/
//  Description : is screen saver timer id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
// PUBLIC BOOLEAN MMIAPIIDLESS_IsScreenSaverTimerId(DPARAM param)
// {
//     return FALSE;
// }
#endif

#if 0
/*****************************************************************************/
// 	Description : Reset ssaver to default.
//	Global resource dependence : 
//  Author: xingdong.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIIDLESS_OpenSSMpeg4(void)
{
	    static MMISET_SCREENSAVER_INFO_T    ssaver_info = {0};
#ifdef VIDEO_PLAYER_SUPPORT
	    VP_PLAY_PARAM_T                     play_param  = {0};
	    GUI_RECT_T                          full_rect = {0};
#endif

        MMIAPISET_GetScreensaverInfo(&ssaver_info);
	
            MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_SCREENSAVER_SET, FALSE);

            play_param.dual_sys = MN_DUAL_SYS_1;
            play_param.full_path_len = ssaver_info.more_saver_info.name_wstr_len;
            play_param.full_path_ptr = ssaver_info.more_saver_info.name_wstr;
            play_param.is_random_play = FALSE;
            play_param.is_scal_up = TRUE;
            play_param.mode = VP_REPEAT_PLAY_MODE_ONE;
            play_param.play_type = VP_PLAY_FROM_FILE;
            play_param.url_ptr = PNULL;
            play_param.video_buf_info.type = 0;
            play_param.video_buf_info.video_buf_len = 0;
            play_param.video_buf_info.video_buf_ptr = PNULL;
            play_param.display_param.audioSupport = 0;
            play_param.display_param.videoSupport = 1;

            play_param.display_param.RotateAngle = GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID); 

            full_rect = MMITHEME_GetFullScreenRect(); 
/*
            if (LCD_ANGLE_0 == play_param.display_param.RotateAngle)
            {
                play_param.display_param.RotateAngle = LCD_ANGLE_90; 
                play_param.display_param.disp_rect.dy = full_rect.right - full_rect.left + 1;           
                play_param.display_param.disp_rect.dx = full_rect.bottom - full_rect.top + 1;
            }
            else if(LCD_ANGLE_180 == play_param.display_param.RotateAngle)
            {
                play_param.display_param.RotateAngle = LCD_ANGLE_270;
                play_param.display_param.disp_rect.dy = full_rect.right - full_rect.left + 1;           
                play_param.display_param.disp_rect.dx = full_rect.bottom - full_rect.top + 1;
            }
            else
*/            
            {
                play_param.display_param.disp_rect.dx = full_rect.right - full_rect.left + 1;           
                play_param.display_param.disp_rect.dy = full_rect.bottom - full_rect.top + 1;
            }
            
            play_param.display_param.target_rect.dx = play_param.display_param.disp_rect.dx;
            play_param.display_param.target_rect.dy = play_param.display_param.disp_rect.dy;
            play_param.display_param.target_rect.x = play_param.display_param.disp_rect.x;
            play_param.display_param.target_rect.y = play_param.display_param.disp_rect.y;
            play_param.display_param.disp_mode = DPLAYER_DISP_CUSTOMIZE;
            
            play_param.vol = MMIAUDIO_GetCurVolume();//for cr220750 maryxiao

            if(!MMIAPIVP_Play(&play_param, HandleVideoScreenSaverMsg, 0))
            {

            }

}
#endif
 


/*Edit by script, ignore 1 case. Thu Apr 26 19:01:05 2012*/ //IGNORE9527
