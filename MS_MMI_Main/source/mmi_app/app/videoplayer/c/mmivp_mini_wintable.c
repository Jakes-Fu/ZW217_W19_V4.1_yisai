/*****************************************************************************
** File Name:      mmiVP_wintab.c                                         *
** Author:                                                                   *
** Date:           2010/3/26                                                 *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010/3/26      apple.zhang        Create                                    *
******************************************************************************/
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
#include "mmi_app_video_player_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmivp_export.h"
#include "mmiudisk_export.h"
#include "mmisd_export.h"
//#include "mmieng.h"
#include "mmipub.h"
#include "mmi_default.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmi_appmsg.h"
#include "mmivp_internal.h"
#include "mmivp_id.h"
#include "window_parse.h"
#include "guires.h"
#include "mmidisplay_data.h"
#include "mmivp_internal.h"
#include "mmk_app.h"
#include "mmi_resource.h"
#include "mmiset_display.h"
#include "mmiphone_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "guiform.h"
#include "guistatusbar.h"
#include "guiblock.h"
#include "guitab.h"
#include "guilcd.h"
#include "mmimultim_text.h"
#include "mmifmm_export.h"
#ifdef MMI_KING_MOVIE_SUPPORT
#include "mmikm_export.h"
#endif
#include "mmimultim_image.h"
#include "mmicc_export.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif


/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define MMIVP_PLAYER_WIN_ID  MMIVP_MAIN_PLAY_WIN_ID //播放器主窗口id
#define VP_VOLUME_MIN MMISRVAUD_VOLUME_LEVEL_0
#define VP_VOLUME_MAX  MMISRVAUD_VOLUME_LEVEL_MAX
LOCAL BOOLEAN  s_is_complusive_pause = FALSE;
LOCAL wchar    s_vp_full_path_name[VP_MAX_NAME_LEN +1]={0}; 
/*---------------------------------------------------------------------------*/
/*                         GLOBAL DEFINITION                                 */
/*---------------------------------------------------------------------------*/
// the window for preview video 
//  Description : handle preview video win msg
//  Global resource dependence : 
//  Author: murphy.xie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVideoPlayMiniWinMsg(
                                       MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param
                                       );
/*****************************************************************************/
// 	Description : HandleFmmVideoPreviewCallback
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void HandleVideoPlayMiniCallback(VP_CALLBACK_T *msg_ptr);
//*****************************************************************************/
//  Description : 暂停
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void VideoPlayerCompulsivePause(void);
/*****************************************************************************/
//  Description : 恢复
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void VideoPlayerCompulsiveResume(void);
/*****************************************************************************/
//  Description : 音量调节
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void AdjustVolCallBack(MMICOM_PANEL_CALLBACK_T *para_ptr) ;

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/
WINDOW_TABLE(MMIVP_VIDEO_PLAY_MINI_WIN_TAB) = 
{
	//CLEAR_LCD,
    WIN_FUNC( (uint32)HandleVideoPlayMiniWinMsg ),  
    WIN_ID( MMIVP_MAIN_PLAY_WIN_ID ),
//WIN_MOVE_STYLE(MOVE_FORBIDDEN),
//    WIN_SUPPORT_ANGLE( WIN_SUPPORT_ANGLE_INIT ),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    WIN_SOFTKEY(TXT_NULL/*TXT_COMMON_OK*/, TXT_NULL, STXT_RETURN),
	END_WIN
};
/*****************************************************************************/
// 	Description : HandleFmmVideoPreviewCallback
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void HandleVideoPlayMiniCallback(VP_CALLBACK_T *msg_ptr)
{
    VP_PLAY_END_IND_T  *play_end_ind_ptr = PNULL;
    VP_PLAY_END_RESULT_E play_end_result = 0;
    
    if (PNULL == msg_ptr)
    {
        return;
    }
    
   SCI_TRACE_LOW("[VP] HandleVideoPlayMiniCallback msg_ptr->msg_id = %d");
    switch(msg_ptr->msg_id) 
    {
    case VP_PLAY_END_IND:
        play_end_ind_ptr = (VP_PLAY_END_IND_T *)msg_ptr->para;        
        play_end_result = play_end_ind_ptr->result;        
    	break;
        
    case VP_STOP_IND: 
        play_end_result = VP_PLAY_END_STOPPED_BY_USER;           
        break;

    case VP_PLAY_PROCESS_IND:
        //MMITHEME_UpdateRect();
        return;

    default:
        return;
    }
    MMK_PostMsg(MMIVP_PLAYER_WIN_ID,MSG_MPEG4_END_CALLBACK,&play_end_result,sizeof(VP_PLAY_END_RESULT_E));

}

/*****************************************************************************/
// 	Description : to handle the screen saver window message
//	Global resource dependence : 
//  Author:murphy.xie
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleVideoPlayMiniWinMsg(
                                           MMI_WIN_ID_T         win_id, 
                                           MMI_MESSAGE_ID_E     msg_id, 
                                           DPARAM               param
                                           )
{
    MMI_RESULT_E                        result = MMI_RESULT_TRUE;
#ifdef VIDEO_PLAYER_SUPPORT
    VP_PLAY_PARAM_T                     play_param  = {0};
    GUI_RECT_T                          full_rect = {0};
#endif
    wchar*                              full_path_name_ptr = (uint32)MMK_GetWinAddDataPtr(win_id);/*lint !e64 */
    uint16                              full_path_len = 0;
    IGUICTRL_T* ctrl_handle=PNULL;

    VP_STRACE_LOW("[VP]: HandleVideoPlayMiniWinMsg(), msg_id = %x");
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //MMIDEFAULT_StopAutoKeylockTimer();
        //MMIDEFAULT_StopLcdBackLightTimer();
        //set anim param
        if(full_path_name_ptr != PNULL)
        {
            full_path_len = MMIAPICOM_Wstrlen(full_path_name_ptr);
        }

        //设置状态栏不可见
        ctrl_handle = MMK_GetCtrlPtrByWin(MMK_GetWinHandle(MMK_GetFirstAppletHandle(), win_id), MMITHEME_GetStatusBarCtrlId());
        if(0 != ctrl_handle)
        {
            IGUICTRL_SetState(ctrl_handle, GUICTRL_STATE_DISABLE_ACTIVE | GUICTRL_STATE_INVISIBLE | GUICTRL_STATE_DISABLE_TP, TRUE);
        }
            play_param.dual_sys = MN_DUAL_SYS_1;
            play_param.full_path_len = full_path_len;
            play_param.full_path_ptr = full_path_name_ptr;
            play_param.is_random_play = FALSE;
            play_param.is_scal_up = TRUE;
            play_param.mode = VP_REPEAT_PLAY_MODE_ONE;
            play_param.play_type = VP_PLAY_FROM_FILE;
            play_param.url_ptr = PNULL;
            play_param.video_buf_info.type = 0;
            play_param.video_buf_info.video_buf_len = 0;
            play_param.video_buf_info.video_buf_ptr = PNULL;
            play_param.display_param.audioSupport = 1;
            play_param.display_param.videoSupport = 1;

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

            MMIDEFAULT_TurnOnBackLight();
            MMIDEFAULT_AllowTurnOffBackLight(FALSE); 
            if(!MMIAPIVP_Play(&play_param, HandleVideoPlayMiniCallback, 0))
            {
                MMIDEFAULT_AllowTurnOffBackLight(TRUE);
                MMK_CloseWin(win_id);

            }

        break;

    case MSG_MPEG4_END_CALLBACK:
        MMK_CloseWin(win_id);

        break;
    case MSG_LOSE_FOCUS:
        GUIBLOCK_ResetMainType();
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        VideoPlayerCompulsivePause();
        break;
    case MSG_GET_FOCUS:
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        GUIBLOCK_SetType(255,MMIAPIVP_GetTransparenceColor(),GUIBLOCK_TYPE_OSD,(GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev());
        VideoPlayerCompulsiveResume();
        break;
    case MSG_LCD_SWITCH:               
        if(MMIAPICOM_IsPanelOpened())
        {
            MMIAPICOM_ClosePanelChildWin();
            GUI_FillRect(MMITHEME_GetDefaultLcdDev(), MMITHEME_GetFullScreenRect(), MMIAPIVP_GetTransparenceColor());
        }
        break;
    case MSG_FULL_PAINT:           
        {           
            GUI_FillRect(MMITHEME_GetDefaultLcdDev(), MMITHEME_GetFullScreenRect(), MMIAPIVP_GetTransparenceColor());
        }
        break;
    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        MMK_FreeWinAddData(win_id);		
        MMIAPIVP_ExitPlay();
        SCI_MEMSET(s_vp_full_path_name,0x00,(VP_MAX_NAME_LEN +1)*sizeof(wchar));

        //MMIDEFAULT_StartAutoKeylockTimer();
        //MMIDEFAULT_StartSettingLCDBackLightTimer();
        break;

    case MSG_KEYDOWN_VOL_UP:
    case MSG_KEYDOWN_UP:        
    case MSG_APP_UPSIDE:
        MMIAPIVP_TurnVol(MMICOM_PANEL_OPEN_INC);
        break;
    case MSG_KEYDOWN_DOWN:         
    case MSG_APP_DOWNSIDE:
    case MSG_KEYDOWN_VOL_DOWN:
        MMIAPIVP_TurnVol(MMICOM_PANEL_OPEN_DEC);
        break;
    case MSG_APP_CANCEL:    //停止播放      
    case MSG_CTL_CANCEL: 
        {
            MMIAPIVP_ExitPlay();
            MMK_CloseWin(win_id);
        }
    break;
    case MSG_APP_RED:
        {
            MMIAPIVP_ExitPlay();
            MMK_CloseWin(win_id);
            result = MMI_RESULT_FALSE;
        }
    default:
        {
            result = MMI_RESULT_FALSE;
        }
        break;
    }

    return (result);
}


/*****************************************************************************/
//  Description : 音量调节
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void AdjustVolCallBack(MMICOM_PANEL_CALLBACK_T *para_ptr) 
{
    if(PNULL != para_ptr)
    {
        VP_STRACE_LOW("[VP] AdjustVolCallBack volue = %d");
        MMIAPISET_SetMultimVolume(para_ptr->cur_value);
        MMIAPIVP_FSMUpdateCurrentVol(VP_GetCurrentFSMHandle(), para_ptr->cur_value);
    }  
}

/*****************************************************************************/
//  Description : 调节音量窗口
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_TurnVol(MMICOM_PANEL_OPEN_TYPE_E turn_vol_type)
{
    MMICOM_PANEL_INFO_T panel_info = {0};
    
    panel_info.cur_value = MMIAPISET_GetMultimVolume();
    panel_info.last_value = MMIAPISET_GetLastMultimVolume();
    panel_info.min_value = VP_VOLUME_MIN;   
    panel_info.max_value = VP_VOLUME_MAX;
    panel_info.panel_type = MMICOM_PANEL_VOLUME;
    panel_info.open_type = turn_vol_type;
    panel_info.set_callback = AdjustVolCallBack; 
#ifndef UI_MULTILAYER_SUPPORT
    panel_info.createwin_style = MMICOM_PLAYER_PANEL_CREATE_WIN;
#endif
    panel_info.x = -1;
    panel_info.y = -1;
    MMIAPICOM_OpenPanelChildWin(MMK_GetFocusWinHandle(),&panel_info); 
}

/*****************************************************************************/
//  Description : MMIAPIVP_IsLiving
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_IsLiving(void)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : MMIAPIVP_IsLiving
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_IsPlayWinFocus(void)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : MMIAPIVP_GetTransparenceColor
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
PUBLIC GUI_COLOR_T MMIAPIVP_GetTransparenceColor(void)
{
   // if(MMISET_THEME_DEFAULT_TYPE == MMIAPISET_GetCurrentThemeType())
   // {
        return MMI_BLACK_COLOR;
   // }
   // else
   // {
    //    return MMI_WHITE_COLOR;
    //}    
}

/*****************************************************************************/
//  Description : 填充透明色
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_FillRectWithTransparenceColor(
                                           uint32 *width_ptr, //[IN/OUT]
                                           uint32 *height_ptr, //[IN/OUT]
                                           uint32 *x_offset_ptr, //[IN/OUT]
                                           uint32 * y_offset_ptr, //[IN/OUT]
                                           uint32 RotateAngle,
                                           BOOLEAN is_need_fill                                  
                                           )
{
    GUI_RECT_T lcd_rect = {0};
    GUI_LCD_DEV_INFO const* lcd_dev_info_ptr = (const GUI_LCD_DEV_INFO*)MMITHEME_GetDefaultLcdDev();
    GUILCD_COORDINATE_PARAM_T logic_coordinate = {0};
    GUILCD_COORDINATE_PARAM_T physical_coordinate = {0};
    uint16 temp = 0;
    LCD_ANGLE_E  logic_angle = GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID);
   
    if (PNULL == width_ptr || PNULL == height_ptr || PNULL == x_offset_ptr || PNULL == y_offset_ptr)
    {
        return FALSE;
    }

    //VP_STRACE_LOW:"[VP] MMIAPIVP_FillRectWithTransparenceColor before x= %d, y=%d,width = %d, height = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13482_112_2_18_3_5_56_538,(uint8*)"dddd",*x_offset_ptr,*y_offset_ptr,*width_ptr,*height_ptr);
        
    *width_ptr = *width_ptr - MMIVP_GetAlign4Offset(*x_offset_ptr);        
    *width_ptr = *width_ptr - (*width_ptr % 4);
    *x_offset_ptr = *x_offset_ptr + MMIVP_GetAlign4Offset(*x_offset_ptr);
    *height_ptr = *height_ptr - MMIVP_GetAlign4Offset(*y_offset_ptr);
    *height_ptr = *height_ptr - (*height_ptr % 4);    
    *y_offset_ptr = *y_offset_ptr + MMIVP_GetAlign4Offset(*y_offset_ptr);
        
	//VP_STRACE_LOW:"[VP] MMIAPIVP_FillRectWithTransparenceColor after x= %d, y=%d,width = %d, height = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13495_112_2_18_3_5_56_539,(uint8*)"dddd",*x_offset_ptr,*y_offset_ptr,*width_ptr,*height_ptr);
    
    if (is_need_fill)
    {
        if (RotateAngle != logic_angle
             && !((LCD_ANGLE_0 == RotateAngle && LCD_ANGLE_180 == logic_angle)
            ||(LCD_ANGLE_180 == RotateAngle && LCD_ANGLE_0 ==logic_angle)))
        {
            lcd_rect.left = *y_offset_ptr;
            lcd_rect.top = *x_offset_ptr;        
            lcd_rect.right = *y_offset_ptr + *height_ptr;
            lcd_rect.bottom = *x_offset_ptr + *width_ptr; 
            
        }
        else
        {
            lcd_rect.left = *x_offset_ptr;
            lcd_rect.top = *y_offset_ptr;        
            lcd_rect.right = *x_offset_ptr + *width_ptr;
            lcd_rect.bottom = *y_offset_ptr + *height_ptr; 
            
        }
        GUI_FillRect(lcd_dev_info_ptr, lcd_rect, MMIAPIVP_GetTransparenceColor()); 
    }
    
    logic_coordinate.angle = RotateAngle;
    logic_coordinate.lcd_height = *height_ptr;
    logic_coordinate.lcd_width = *width_ptr;
    logic_coordinate.rect.left = *x_offset_ptr;
    logic_coordinate.rect.right = *x_offset_ptr + *width_ptr-1;
    logic_coordinate.rect.top = *y_offset_ptr;
    logic_coordinate.rect.bottom = *y_offset_ptr + *height_ptr-1;  
    
    GUILCD_GetLogicWidthHeight(lcd_dev_info_ptr->lcd_id,&(logic_coordinate.lcd_width),&(logic_coordinate.lcd_height));
    
    if(RotateAngle != logic_angle
        && !((LCD_ANGLE_0 == RotateAngle && LCD_ANGLE_180 == logic_angle)
            ||(LCD_ANGLE_180 == RotateAngle && LCD_ANGLE_0 ==logic_angle))
            )
    {
        temp = logic_coordinate.lcd_width;
        logic_coordinate.lcd_width = logic_coordinate.lcd_height;
        logic_coordinate.lcd_height = temp;
    }

    GUILCD_ConvertLogicToPhysicalCoordinate(lcd_dev_info_ptr->lcd_id,
        &logic_coordinate,
        &physical_coordinate);

    *width_ptr = physical_coordinate.rect.right - physical_coordinate.rect.left + 1;
    *height_ptr = physical_coordinate.rect.bottom - physical_coordinate.rect.top + 1;
    *x_offset_ptr = physical_coordinate.rect.left;
    *y_offset_ptr = physical_coordinate.rect.top;
  

    //VP_STRACE_LOW:"[VP] MMIAPIVP_FillRectWithTransparenceColor after x= %d, y=%d,width = %d, height = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_13554_112_2_18_3_5_57_540,(uint8*)"dddd",*x_offset_ptr,*y_offset_ptr,*width_ptr,*height_ptr);

    return TRUE;
}

/*****************************************************************************/
//  Description : 填充透明色
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_TranslateDisplayParamLogicToPhysical(VP_DISPLAY_PARAM_T *disp_para, BOOLEAN is_need_fill)
{
    GUI_LCD_DEV_INFO const* lcd_dev_info_ptr = (const GUI_LCD_DEV_INFO*)MMITHEME_GetDefaultLcdDev();
    GUILCD_COORDINATE_PARAM_T logic_coordinate = {0};
    GUILCD_COORDINATE_PARAM_T physical_coordinate = {0};

    logic_coordinate.angle = disp_para->RotateAngle;
    logic_coordinate.lcd_height = disp_para->disp_rect.dy;
    logic_coordinate.lcd_width = disp_para->disp_rect.dx;

    MMIAPIVP_FillRectWithTransparenceColor(&(disp_para->disp_rect.dx), &(disp_para->disp_rect.dy), &(disp_para->disp_rect.x), &(disp_para->disp_rect.y), disp_para->RotateAngle, is_need_fill);
    MMIAPIVP_FillRectWithTransparenceColor(&(disp_para->target_rect.dx), &(disp_para->target_rect.dy), &(disp_para->target_rect.x), &(disp_para->target_rect.y), disp_para->RotateAngle, is_need_fill);
    GUILCD_ConvertLogicToPhysicalCoordinate(lcd_dev_info_ptr->lcd_id,
        &logic_coordinate,
        &physical_coordinate);
    
    disp_para->RotateAngle = physical_coordinate.angle;
    return TRUE;

}
/*****************************************************************************/
//  Description : 暂停
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void VideoPlayerCompulsivePause(void)
{
    if (!s_is_complusive_pause)
    {
        s_is_complusive_pause = TRUE;
        MMIAPIVP_FSMCompulsivePause(VP_GetCurrentFSMHandle());
    }    
}

/*****************************************************************************/
//  Description : 恢复
//  Global resource dependence : 
//  Author: applezhang
//  Note: 
/*****************************************************************************/
LOCAL void VideoPlayerCompulsiveResume(void)
{
    s_is_complusive_pause = FALSE;
    MMIAPIVP_FSMCompulsiveResume(VP_GetCurrentFSMHandle());    //状态改变，显示应该位于该函数之下
}

/*****************************************************************************/
//  Description : 播放视频文件
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_MiniFunction_PlayVideo(wchar *full_path_name_ptr,
                                                                                            uint16 full_path_name_len)
{  

    
    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return FALSE;
    }
    else if(MMIAPISD_IsCardLogOn())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_CARDLOG_USING);
        return FALSE;
    }
    else if (MMIAPIENG_GetIQModeStatus())
    {
        MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
        return FALSE;
    }
    else if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        MMIPUB_OpenAlertWarningWin(TXT_CALL_USING);
        return FALSE;
    }
    
    if (PNULL == full_path_name_ptr )
    {
        MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_NO_EXIST);
        return FALSE;
    }
    
        //JAVA 后台运行,内存不足，提示退出
#ifdef JAVA_SUPPORT
    //VP_STRACE_LOW:"MMIAPIVP_PlayVideoFromVideoPlayer java running =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_WINTABLE_3045_112_2_18_3_5_34_431,(uint8*)"d", MMIAPIJAVA_IsJavaRuning());
    if (MMIAPIJAVA_IsJavaRuning())
    {
        MMIPUB_OpenAlertWarningWin( TXT_EXIT_BACKGROUND_JAVA);
        return FALSE;
    }
#endif

#ifdef MMI_KING_MOVIE_SUPPORT
    MMIAPIKM_CkeckSupportfile(full_path_name_ptr);
#endif

    if (!MMK_IsOpenWin(MMIVP_PLAYER_WIN_ID))
    {        
        wchar  *filename_ptr = PNULL;
        filename_ptr = (wchar *)SCI_ALLOCA((MMIFILE_FILE_NAME_MAX_LEN +1)* sizeof(wchar));
        if(filename_ptr != PNULL)
        {
         SCI_MEMSET(filename_ptr, 0x00, ((MMIFILE_FILE_NAME_MAX_LEN +1)* sizeof(wchar)));
         MMIAPICOM_Wstrcpy(filename_ptr, full_path_name_ptr);
         MMK_CreateWin((uint32*)MMIVP_VIDEO_PLAY_MINI_WIN_TAB, (ADD_DATA)filename_ptr);
         MMIAPICOM_Wstrncpy( s_vp_full_path_name,
                                                full_path_name_ptr,MMIAPICOM_Wstrlen(full_path_name_ptr)) ;
        }
    }
    else  //播放器窗口已经打开，设置获得focus
    {        
        MMK_WinGrabFocus(MMIVP_PLAYER_WIN_ID);
    }

    return TRUE;
    
}
/*****************************************************************************/
//  Description : 播放视频文件
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_MiniFunction_ExitVideoPlayer(void)
{
    VP_STRACE_LOW("[VP] MMIAPIVP_MiniFunction_ExitPlay s_current_fsm_ptr = %x");
    MMIAPIVP_ExitPlay();
    MMK_CloseWin(MMIVP_PLAYER_WIN_ID);
}
//add CR233033
/*****************************************************************************/
//  Description : get full path name of playing file
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_GetPlayingFilename(wchar * full_path_name_ptr)
{
    if(MMIAPIVP_IsOpenPlayerWin())
    {
        MMIAPICOM_Wstrncpy( full_path_name_ptr,
                                                s_vp_full_path_name,MMIAPICOM_Wstrlen(s_vp_full_path_name)) ;
    }

}
/*****************************************************************************/
//  Description : MMIAPIVP_IsOpenPlayerWin
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVP_IsOpenPlayerWin(void)
{
    return MMK_IsOpenWin(MMIVP_PLAYER_WIN_ID);

}
#endif


