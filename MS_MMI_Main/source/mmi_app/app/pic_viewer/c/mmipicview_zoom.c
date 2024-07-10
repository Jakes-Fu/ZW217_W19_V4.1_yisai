/*****************************************************************************
** File Name:      mmipicview_zoom.c                                         *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008      Jassmine              Creat
******************************************************************************/

#define _MMIPICVIEW_ZOOM_C_

#ifdef PICVIEW_VIEW_ZOOM
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_pic_viewer_trc.h"
#ifdef WIN32 
#include "std_header.h"
#endif

#ifdef PIC_VIEWER_SUPPORT
#include "mmi_appmsg.h"
#include "window_parse.h"
#include "mmk_type.h"
#include "mmk_timer.h"
#include "guianim.h"
#include "guires.h"
#include "guictrl_api.h"
#include "mmi_theme.h"
#include "mmitheme_pos.h"
#include "mmipicview_internal.h"
#include "mmipicview_id.h"
#include "mmipicview_image.h"
#include "mmidisplay_data.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIPICVIEW_SACCADE_BORDER_WIDTH 1       //扫视图边框宽度
#define MMIPICVIEW_SACCADE_RATIO        4       //扫视图是LCD宽,高的1/4
#define MMIPICVIEW_OSD_TIME             2000    //2s
#define MMIPICVIEW_MOVE_RATIO           4       //每次移动LCD宽,高的1/4

#define MMIPICVIEW_VIEW_ZOOM_SLEEP_TIME 300  //ms
#define MMIPICVIEW_VIEW_ZOOM_SLEEP_COUNTER_MAX 30
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL BOOLEAN               s_mmipicview_is_set_saccade = FALSE;
LOCAL BOOLEAN               s_mmipicview_is_display_osd = FALSE;
LOCAL BOOLEAN               s_mmipicview_is_tp_softkey = FALSE;
LOCAL uint8                 s_mmipicview_osd_timer_id = 0;
LOCAL GUI_POINT_T           s_mmipicview_tp_point = {0};
LOCAL GUIANIM_FILE_INFO_T   s_mmipicview_file = {0};

#if defined(PICVIEW_F_VIEW_ZOOM)
LOCAL uint32 s_mmipicview_zoom_out_counter = 0;
LOCAL uint32 s_mmipicview_sleep_counter = 0;
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
//  Description : handle picture zoom win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicZoomWinMsg(
                                       MMI_WIN_ID_T         win_id,
                                       MMI_MESSAGE_ID_E     msg_id,
                                       DPARAM               param
                                       );

/*****************************************************************************/
//  Description : set zoom anim param
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetZoomAnimParam(
                               wchar            *full_path_ptr, //in:
                               uint16           full_path_len,  //in:
                               MMI_CTRL_ID_T    ctrl_id         //in:
                               );

/*****************************************************************************/
//  Description : zoom anim owner draw call back function
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void ZoomAnimCallback(
                            GUIANIM_OWNER_DRAW_T    *owner_draw_ptr
                            );

/*****************************************************************************/
//  Description : set saccade anim param
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetSaccadeAnimParam(
                                  MMI_CTRL_ID_T     ctrl_id,    //in:
                                  GUIIMG_BITMAP_T   *bitmap_ptr //in:
                                  );

/*****************************************************************************/
//  Description : get saccade anim both rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T GetSaccadeAnimBothRect(void);

/*****************************************************************************/
//  Description : saccade anim owner draw call back function
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SaccadeAnimCallback(
                               GUIANIM_OWNER_DRAW_T     *owner_draw_ptr
                               );

/*****************************************************************************/
//  Description : set display zoom osd
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetDisplayZoomOsd(
                             BOOLEAN    is_display
                             );

/*****************************************************************************/
//  Description : display zoom osd
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void DisplayZoomOsd(
                          MMI_WIN_ID_T      win_id,     //in:
                          MMI_CTRL_ID_T     ctrl_id,    //in:
                          MMI_CTRL_ID_T     osd_ctrl_id //in:
                          );

/*****************************************************************************/
//  Description : display zoom osd softkey
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void DisplayOsdSoftkey(
                             MMI_WIN_ID_T   win_id,     //in:
                             MMI_CTRL_ID_T  ctrl_id     //in:zoom ctrl
                             );

/*****************************************************************************/
//  Description : start display zoom osd timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void StartZoomOsdTimer(
                             MMI_WIN_ID_T   win_id
                             );

/*****************************************************************************/
//  Description : stop display zoom osd timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void StopZoomOsdTimer(void);

/*****************************************************************************/
//  Description : handle zoom timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleZoomTimer(
                                   uint8            timer_id,   //in:
                                   MMI_WIN_ID_T     win_id,     //in:
                                   MMI_CTRL_ID_T    ctrl_id,    //in:
                                   MMI_CTRL_ID_T    osd_ctrl_id //in:
                                   );

/*****************************************************************************/
//  Description : handle display zoom osd
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleDisplayZoomOsd(
                                   MMI_WIN_ID_T     win_id,     //in:
                                   MMI_CTRL_ID_T    ctrl_id,    //in:
                                   MMI_CTRL_ID_T    osd_ctrl_id //in:
                                   );

/*****************************************************************************/
//  Description : handle picture zoom direction key
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleZoomKeyMove(
                                MMI_CTRL_ID_T       ctrl_id,
                                MMI_MESSAGE_ID_E    msg_id
                                );

/*****************************************************************************/
//  Description : handle picture zoom tp down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void HandleZoomTpDown(
                            MMI_WIN_ID_T    win_id,     //in:
                            MMI_CTRL_ID_T   ctrl_id,    //in:
                            MMI_CTRL_ID_T   osd_ctrl_id,//in:
                            DPARAM          param       //in:
                            );

/*****************************************************************************/
//  Description : handle picture zoom tp move msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void HandleZoomTpMove(
                            MMI_CTRL_ID_T   ctrl_id,    //in:
                            DPARAM          param       //in:
                            );

/*****************************************************************************/
//  Description : handle picture zoom tp up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void HandleZoomTpUp(
                          MMI_WIN_ID_T      win_id,     //in:
                          MMI_CTRL_ID_T     ctrl_id,    //in:
                          DPARAM            param       //in:
                          );
                          
/*****************************************************************************/
//  Description : process picture state
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL int32 ProcessPicState(MMIPICVIEW_PIC_STATE_E pic_state, //in:
                                MMI_WIN_ID_T     win_id,     //in:
                                MMI_CTRL_ID_T  ctrl_id     //in:zoom ctrl
                                );
                                
/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
//picture full screen preview
WINDOW_TABLE( MMIPICVIEW_ZOOM_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandlePicZoomWinMsg),
    WIN_ID(MMIPICVIEW_ZOOM_WIN_ID),
    WIN_STYLE(WS_HAS_ANIM_BG|WS_DISABLE_COMMON_BG),
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_CUR),
    CREATE_ANIM_CTRL(MMIPICVIEW_ZOOM_ANIM_CTRL_ID,MMIPICVIEW_ZOOM_WIN_ID),
    CREATE_ANIM_CTRL(MMIPICVIEW_ZOOM_OSD_ANIM_CTRL_ID,MMIPICVIEW_ZOOM_WIN_ID),
    WIN_SOFTKEY(TXT_NULL,TXT_NULL,TXT_NULL),
    END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : zoom picture
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_Zoom(
                            wchar   *full_path_ptr, //in:
                            uint16  full_path_len   //in:
                            )
{
    //alloc memory
    s_mmipicview_file.full_path_wstr_ptr = SCI_ALLOC_APP((full_path_len+1)*sizeof(wchar));
    
    if (PNULL == s_mmipicview_file.full_path_wstr_ptr)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW MMIPICVIEW_Zoom full_path_wstr_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_ZOOM_259_112_2_18_2_46_51_93,(uint8*)"");
        return ;
    }

    //set file path
    SCI_MEMSET(s_mmipicview_file.full_path_wstr_ptr,0,((full_path_len+1)*sizeof(wchar)));
    s_mmipicview_file.full_path_wstr_len = full_path_len;

    MMI_WSTRNCPY(s_mmipicview_file.full_path_wstr_ptr,full_path_len,
        full_path_ptr,full_path_len,
        full_path_len);

    MMK_CreateWin((uint32 *)MMIPICVIEW_ZOOM_WIN_TAB,PNULL);
}

/*****************************************************************************/
//  Description : handle picture zoom win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicZoomWinMsg(
                                       MMI_WIN_ID_T         win_id,
                                       MMI_MESSAGE_ID_E     msg_id,
                                       DPARAM               param
                                       )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIPICVIEW_ZOOM_ANIM_CTRL_ID;
    MMI_CTRL_ID_T       osd_ctrl_id = MMIPICVIEW_ZOOM_OSD_ANIM_CTRL_ID;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //set saccade not visible
        GUIANIM_SetVisible(osd_ctrl_id,FALSE,FALSE);

        //set softkey not visible
        GUIWIN_SetSoftkeyVisible(win_id,FALSE);

        //set zoom anim
        s_mmipicview_is_set_saccade = FALSE;
        SetZoomAnimParam(s_mmipicview_file.full_path_wstr_ptr,
            s_mmipicview_file.full_path_wstr_len,
            ctrl_id);

        //set display osd
        SetDisplayZoomOsd(TRUE);

        MMK_SetAtvCtrl(win_id,ctrl_id);
        
        ProcessPicState(MMIPICVIEW_PIC_INIT, 
                        win_id,
                        ctrl_id 
                        );
        break;

    case MSG_GET_FOCUS:
        //set display osd
        SetDisplayZoomOsd(TRUE);
        break;
        
    case MSG_LOSE_FOCUS:
        //stop osd timer
        StopZoomOsdTimer();
        break;

    case MSG_TIMER:
        result = HandleZoomTimer(*(uint8*)param,win_id,ctrl_id,osd_ctrl_id);
        break;

    case MSG_APP_OK:
    case MSG_KEYREPEAT_OK:
        if (HandleDisplayZoomOsd(win_id,ctrl_id,osd_ctrl_id))
        {
            GUIANIM_Zoom(ctrl_id,FALSE,TRUE);
        }
        break;

    case MSG_APP_WEB:
    case MSG_KEYREPEAT_WEB:
        if (HandleDisplayZoomOsd(win_id,ctrl_id,osd_ctrl_id))
        {
            GUIANIM_Zoom(ctrl_id,TRUE,TRUE);
        }
        break;

#ifdef TOUCH_GESTURE_ENABLE
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_GESTURE:
    //SCI_TRACE_LOW:"MMIPICVIEW HandlePicZoomWinMsg MSG_TP_GESTURE type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_ZOOM_347_112_2_18_2_46_51_94,(uint8*)"d",MMK_GET_TP_GESTURE(param));
        if ( MMI_GESTURE_ZOOM_IN == MMK_GET_TP_GESTURE(param)
            || MMI_GESTURE_ZOOM_OUT == MMK_GET_TP_GESTURE(param) )
        {
            //Note:not call HandleDisplayZoomOsd,
            // because picture may be zoomed fail,
            //and do not display osd and soft key firstly
            //if (HandleDisplayZoomOsd(win_id,ctrl_id,osd_ctrl_id))
            {
                GUIANIM_Zoom(ctrl_id,(BOOLEAN)(MMI_GESTURE_ZOOM_IN == MMK_GET_TP_GESTURE(param)),TRUE);
            }
        }
        break;
#endif  //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif

    case MSG_APP_UP:
    case MSG_APP_DOWN:
    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
        if (HandleDisplayZoomOsd(win_id,ctrl_id,osd_ctrl_id))
        {
            HandleZoomKeyMove(ctrl_id,msg_id);
        }
        break;
        
    case MSG_KEYREPEAT_UP:
    case MSG_KEYREPEAT_DOWN:
    case MSG_KEYREPEAT_LEFT:
    case MSG_KEYREPEAT_RIGHT:
        if (HandleDisplayZoomOsd(win_id,ctrl_id,osd_ctrl_id))
        {
            if (!HandleZoomKeyMove(ctrl_id,msg_id))
            {
                //stop osd timer
                StopZoomOsdTimer();

                //start display timer
                StartZoomOsdTimer(win_id);
            }
        }
        break;

    case MSG_APP_CANCEL:
        if (HandleDisplayZoomOsd(win_id,ctrl_id,osd_ctrl_id))
        {
            MMK_CloseWin(win_id);
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        HandleZoomTpDown(win_id,ctrl_id,osd_ctrl_id,param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        HandleZoomTpMove(ctrl_id,param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        HandleZoomTpUp(win_id,ctrl_id,param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_NOTIFY_UPDATE:
        GUIANIM_UpdateRect(*(GUI_RECT_T *)param,ctrl_id);
        break;
    case MSG_PICVIEWER_MIN_ZOOM_OUT:
        //SCI_TRACE_LOW:"MMIPICVIEW HandlePicZoomWinMsg MSG_PICVIEWER_MIN_ZOOM_OUT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_ZOOM_412_112_2_18_2_46_51_95,(uint8*)"");
        
//         #if defined(PICVIEW_F_VIEW_ZOOM)
//             MMK_CloseWin(win_id);
//         #endif    
        break;
    case MSG_PICVIEWER_PIC_STATE:
        #if defined(PICVIEW_F_VIEW_ZOOM)
            ProcessPicState(MMIPICVIEW_PIC_DECODING,
                            win_id,
                            ctrl_id
                            );
        #endif    
        break;
    case MSG_CLOSE_WINDOW:
        //stop osd timer
        StopZoomOsdTimer();

        //free memory
        if (PNULL != s_mmipicview_file.full_path_wstr_ptr)
        {
            SCI_FREE(s_mmipicview_file.full_path_wstr_ptr);
            s_mmipicview_file.full_path_wstr_len = 0;
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set zoom anim param
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetZoomAnimParam(
                               wchar            *full_path_ptr, //in:
                               uint16           full_path_len,  //in:
                               MMI_CTRL_ID_T    ctrl_id         //in:
                               )
{
    BOOLEAN                 result = FALSE;
    GUI_BOTH_RECT_T         both_rect = MMITHEME_GetFullScreenBothRect();
    GUIANIM_RESULT_E        anim_result = GUIANIM_RESULT_SUCC;
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_FILE_INFO_T     anim_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};

    GUIAPICTRL_SetBothRect(ctrl_id,&both_rect);

    //set anim param
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = ctrl_id;

    //set file
    anim_info.full_path_wstr_ptr = full_path_ptr;
    anim_info.full_path_wstr_len = full_path_len;

    //set display info
    display_info.is_zoom = TRUE;
    display_info.is_disp_one_frame = TRUE;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.bg.bg_type = GUI_BG_COLOR;
    display_info.bg.color   = MMI_WINDOW_BACKGROUND_COLOR;

    anim_result = GUIANIM_SetParam(&control_info,PNULL,&anim_info,&display_info);

    //set call back
    GUIANIM_SetOwnerDraw(ctrl_id,ZoomAnimCallback);

    if ((GUIANIM_RESULT_SUCC == anim_result) ||
        (GUIANIM_RESULT_WAIT == anim_result))
    {
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : zoom anim owner draw call back function
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void ZoomAnimCallback(
                            GUIANIM_OWNER_DRAW_T    *owner_draw_ptr
                            )
{
    MMI_CTRL_ID_T   osd_ctrl_id = MMIPICVIEW_ZOOM_OSD_ANIM_CTRL_ID;

    if ((!s_mmipicview_is_set_saccade) && 
        (GUIANIM_IsSupport(owner_draw_ptr->ctrl_handle)))
    {
        //set saccade anim
        SetSaccadeAnimParam(osd_ctrl_id,&owner_draw_ptr->bitmap_data);

        s_mmipicview_is_set_saccade = TRUE;
    }

    if (s_mmipicview_is_display_osd) 
    {
        //stop osd timer
        StopZoomOsdTimer();

        //display zoom osd
        DisplayZoomOsd(owner_draw_ptr->win_handle,
            owner_draw_ptr->ctrl_handle,
            osd_ctrl_id);

        if (MMK_IsFocusWin(owner_draw_ptr->win_handle))
        {
            //start display timer
            StartZoomOsdTimer(owner_draw_ptr->win_handle);
        }
    }
}

/*****************************************************************************/
//  Description : set saccade anim param
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetSaccadeAnimParam(
                                  MMI_CTRL_ID_T     ctrl_id,    //in:
                                  GUIIMG_BITMAP_T   *bitmap_ptr //in:
                                  )
{
    BOOLEAN                 result = FALSE;
    GUI_BG_T                error_bg = {0};
    GUI_BORDER_T            border = {0};
    GUI_BOTH_RECT_T         both_rect = {0};
    GUIANIM_RESULT_E        anim_result = GUIANIM_RESULT_SUCC;
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};

    //get saccade anim both rect
    both_rect = GetSaccadeAnimBothRect();

    //set both rect
    GUIAPICTRL_SetBothRect(ctrl_id,&both_rect);

    //set error bg
    error_bg.bg_type = GUI_BG_COLOR;
    error_bg.color   = MMI_DARK_GRAY_COLOR;
    GUIANIM_SetErrorBg(ctrl_id,&error_bg);

    //set border
    border.type  = GUI_BORDER_SOLID;
    border.color = MMI_DARK_GRAY_COLOR;
    border.width = MMIPICVIEW_SACCADE_BORDER_WIDTH;
    GUIANIM_SetBorder(ctrl_id,&border);

    //set anim param
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = ctrl_id;
    
    //set data info
    data_info.is_bitmap   = TRUE;
    data_info.bitmap_data = *bitmap_ptr;

    //set display info
    display_info.is_zoom = TRUE;
    display_info.is_disp_one_frame = TRUE;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.bg.bg_type = GUI_BG_COLOR;
    display_info.bg.color   = MMI_GRAY_WHITE_COLOR;

    anim_result = GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);

    //set call back
    GUIANIM_SetOwnerDraw(ctrl_id,SaccadeAnimCallback);

    if ((GUIANIM_RESULT_SUCC == anim_result) ||
        (GUIANIM_RESULT_WAIT == anim_result))
    {
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : get saccade anim both rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T GetSaccadeAnimBothRect(void)
{
    uint16              lcd_width = 0;
    uint16              lcd_height = 0;
    uint16              saccade_width = 0;
    uint16              saccade_height = 0;
    GUI_BOTH_RECT_T     rect = {0};

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,
        &lcd_width,
        &lcd_height);
    
    //set both rect
    if (lcd_width > lcd_height)
    {
        uint16      temp_width = lcd_width;

        lcd_width  = lcd_height;
        lcd_height = temp_width;
    }

    //get saccade win width and height
    saccade_width  = (uint16)(lcd_width/MMIPICVIEW_SACCADE_RATIO);
    saccade_height = (uint16)(lcd_height/MMIPICVIEW_SACCADE_RATIO);

    rect.v_rect.left   = 0;
    rect.v_rect.right  = (int16)(saccade_width + MMIPICVIEW_SACCADE_BORDER_WIDTH*2 - 1);
    rect.v_rect.top    = (int16)(lcd_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT - saccade_height - MMIPICVIEW_SACCADE_BORDER_WIDTH*2);
    rect.v_rect.bottom = (int16)(lcd_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT - 1);

    rect.h_rect.left   = 0;
    rect.h_rect.right  = (int16)(saccade_height + MMIPICVIEW_SACCADE_BORDER_WIDTH*2 - 1);
    rect.h_rect.top    = (int16)(lcd_width - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT - saccade_width - MMIPICVIEW_SACCADE_BORDER_WIDTH*2);
    rect.h_rect.bottom = (int16)(lcd_width - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT - 1);

    return (rect);
}

/*****************************************************************************/
//  Description : saccade anim owner draw call back function
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SaccadeAnimCallback(
                               GUIANIM_OWNER_DRAW_T     *owner_draw_ptr
                               )
{
    uint16                  img_width = 0;
    uint16                  img_height = 0;
    uint16                  cur_img_width = 0;
    uint16                  cur_img_height = 0;
    GUI_RECT_T              clip_rect = {0};
    GUI_RECT_T              rect = {0};
    MMI_CTRL_ID_T           ctrl_id = MMIPICVIEW_ZOOM_ANIM_CTRL_ID;
    MMI_CTRL_ID_T           osd_ctrl_id = owner_draw_ptr->ctrl_handle;
    const GUI_LCD_DEV_INFO  *lcd_dev_ptr = MMITHEME_GetDefaultLcdDev();

    //get imgae width
    if ((GUIANIM_GetImgWidthHeightEx(ctrl_id,&img_width,&img_height)) &&
        (0 < img_width))
    {
        //get clip rect
        GUIANIM_GetZoomClipRect(ctrl_id,&clip_rect);

        //set osd image width
        GUIANIM_GetImgWidthHeightEx(osd_ctrl_id,&cur_img_width,&cur_img_height);
        
        //get saccade rect,left
        if (0 == clip_rect.left)
        {
            rect.left = owner_draw_ptr->display_x;
        }
        else
        {
            rect.left = (int16)(owner_draw_ptr->display_x + clip_rect.left * cur_img_width / img_width);
        }

        //right
        if (img_width == clip_rect.right + 1)
        {
            rect.right = (int16)(owner_draw_ptr->display_x + cur_img_width - 1);
        }
        else
        {
            rect.right = (int16)(owner_draw_ptr->display_x + clip_rect.right * cur_img_width / img_width);
        }

        //top
        if (0 == clip_rect.top)
        {
            rect.top = owner_draw_ptr->display_y;
        }
        else
        {
            rect.top = (int16)(owner_draw_ptr->display_y + clip_rect.top * cur_img_width / img_width);
        }

        //bottom
        if (img_height == clip_rect.bottom + 1)
        {
            rect.bottom = (int16)(owner_draw_ptr->display_y + cur_img_height - 1);
        }
        else
        {
            rect.bottom = (int16)(owner_draw_ptr->display_y + clip_rect.bottom * cur_img_width / img_width);
        }
        
        //get crossed rect
        if (GUI_IntersectRect(&rect,rect,owner_draw_ptr->display_rect))
        {
            //display rect
            LCD_DrawRect(lcd_dev_ptr,
                rect,
                MMI_DARK_RED_COLOR);
        }
    }
}

/*****************************************************************************/
//  Description : set display zoom osd
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetDisplayZoomOsd(
                             BOOLEAN    is_display
                             )
{
    s_mmipicview_is_display_osd = is_display;
}

/*****************************************************************************/
//  Description : display zoom osd
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void DisplayZoomOsd(
                          MMI_WIN_ID_T      win_id,     //in:
                          MMI_CTRL_ID_T     ctrl_id,    //in:
                          MMI_CTRL_ID_T     osd_ctrl_id //in:
                          )
{
    if (GUIANIM_IsSupport(ctrl_id))
    {
        //set saccade visible and update
        GUIANIM_SetVisible(osd_ctrl_id,TRUE,TRUE);
    }

    //set softkey visible
    GUIWIN_SetSoftkeyVisible(win_id,TRUE);

    //display softkey
    DisplayOsdSoftkey(win_id,ctrl_id);
}

/*****************************************************************************/
//  Description : display zoom osd softkey
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void DisplayOsdSoftkey(
                             MMI_WIN_ID_T   win_id,     //in:
                             MMI_CTRL_ID_T  ctrl_id     //in:zoom ctrl
                             )
{
    BOOLEAN         is_gray = FALSE;
    uint16          width_min = 0;
    uint16          width_max = 0;
    uint16          img_width = 0;
    GUI_BG_T        common_bg = {0};
    MMI_IMAGE_ID_T  img_id = 0;

    //set common bg type
    common_bg.bg_type = GUI_BG_NONE;
    GUIWIN_SetSoftkeyCommonBg(win_id,&common_bg);

    //get zoom width range
    if (GUIANIM_GetZoomWidthRange(ctrl_id,&width_min,&width_max,PNULL,PNULL))
    {
        //get current image widht
        GUIANIM_GetImgWidthHeightEx(ctrl_id,
            &img_width,
            PNULL);

        //is zoom out
        if (img_width > width_min)
        {
            is_gray = FALSE;
            img_id  = IMAGE_PICVIEW_ZOOM_OUT;
        }
        else
        {
            is_gray = TRUE;
            img_id  = IMAGE_PICVIEW_ZOOM_OUT_GRAY;

            ProcessPicState(MMIPICVIEW_PIC_MIN_ZOOM_OUT,
                            win_id,
                            ctrl_id
                            );
        }

        //set left image
        GUIWIN_SeSoftkeytButtonIconId(win_id,
            img_id,
            0,
            FALSE);

        //set left state
        GUIWIN_SetSoftkeyBtnState(win_id,
                0,
                is_gray,
                FALSE);
    
        //is zoom in
        if (img_width < width_max)
        {
            is_gray = FALSE;
            img_id  = IMAGE_PICVIEW_ZOOM_IN;
        }
        else
        {
            is_gray = TRUE;
            img_id  = IMAGE_PICVIEW_ZOOM_IN_GRAY;
        }

        //set middle softkey
        GUIWIN_SeSoftkeytButtonIconId(win_id,
            img_id,
            1,
            FALSE);

        //set middle state
        GUIWIN_SetSoftkeyBtnState(win_id,
                1,
                is_gray,
                FALSE);
    }

    //set right softkey
    GUIWIN_SeSoftkeytButtonIconId(win_id,
        IMAGE_PICVIEW_RETURN,
        2,
        TRUE);
}

/*****************************************************************************/
//  Description : start display zoom osd timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void StartZoomOsdTimer(
                             MMI_WIN_ID_T   win_id
                             )
{
    if (0 == s_mmipicview_osd_timer_id)
    {
        s_mmipicview_osd_timer_id = MMK_CreateWinTimer(win_id,MMIPICVIEW_OSD_TIME,FALSE);
    }
    else
    {
        //SCI_TRACE_LOW:"StartZoomOsdTimer:don't creat display timer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_ZOOM_854_112_2_18_2_46_52_96,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : stop display zoom osd timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void StopZoomOsdTimer(void)
{
    if (0 < s_mmipicview_osd_timer_id)
    {
        MMK_StopTimer(s_mmipicview_osd_timer_id);
        s_mmipicview_osd_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : handle zoom timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleZoomTimer(
                                   uint8            timer_id,   //in:
                                   MMI_WIN_ID_T     win_id,     //in:
                                   MMI_CTRL_ID_T    ctrl_id,    //in:
                                   MMI_CTRL_ID_T    osd_ctrl_id //in:
                                   )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    if (s_mmipicview_osd_timer_id == timer_id)
    {
        //stop timer
        StopZoomOsdTimer();
        
        //set saccade not visible
        GUIANIM_SetVisible(osd_ctrl_id,FALSE,FALSE);

        //set softkey not visible
        GUIWIN_SetSoftkeyVisible(win_id,FALSE);
        
        //set not display osd
        SetDisplayZoomOsd(FALSE);

        if (MMK_IsFocusWin(win_id))
        {
            //display zoom anim
            MMK_SendMsg(ctrl_id,MSG_CTL_PAINT,PNULL);
        }
    }
    else
    {
        result = MMI_RESULT_FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle display zoom osd
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleDisplayZoomOsd(
                                   MMI_WIN_ID_T     win_id,     //in:
                                   MMI_CTRL_ID_T    ctrl_id,    //in:
                                   MMI_CTRL_ID_T    osd_ctrl_id //in:
                                   )
{
    BOOLEAN     result = FALSE;

    //is display zoom osd
    result = s_mmipicview_is_display_osd;
    if (!result)
    {
        //set display osd
        SetDisplayZoomOsd(TRUE);

        //display zoom osd
        DisplayZoomOsd(win_id,ctrl_id,osd_ctrl_id);

        //start display timer
        StartZoomOsdTimer(win_id);
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle picture zoom direction key
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleZoomKeyMove(
                                MMI_CTRL_ID_T       ctrl_id,
                                MMI_MESSAGE_ID_E    msg_id
                                )
{
    BOOLEAN         result = FALSE;
    int16           move_x = 0;
    int16           move_y = 0;
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;

    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,
        &lcd_width,
        &lcd_height);

    switch (msg_id)
    {
    case MSG_APP_UP:
    case MSG_KEYREPEAT_UP:
        move_x = 0;
        move_y = (int16)(-lcd_height/MMIPICVIEW_MOVE_RATIO);
        break;

    case MSG_APP_DOWN:
    case MSG_KEYREPEAT_DOWN:
        move_x = 0;
        move_y = (int16)(lcd_height/MMIPICVIEW_MOVE_RATIO);
        break;

    case MSG_APP_LEFT:
    case MSG_KEYREPEAT_LEFT:
        move_x = (int16)(-lcd_width/MMIPICVIEW_MOVE_RATIO);
        move_y = 0;
        break;

    case MSG_APP_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
        move_x = (int16)(lcd_width/MMIPICVIEW_MOVE_RATIO);
        move_y = 0;
        break;

    default:
        //SCI_TRACE_LOW:"MMIPICVIEW HandleZoomKeyMove:msg_id %d is error!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_ZOOM_988_112_2_18_2_46_53_97,(uint8*)"d",msg_id);
        break;
    }

    //move
    result = GUIANIM_ZoomMove(ctrl_id,move_x,move_y,TRUE);

    return (result);
}

/*****************************************************************************/
//  Description : handle picture zoom tp down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void HandleZoomTpDown(
                            MMI_WIN_ID_T    win_id,     //in:
                            MMI_CTRL_ID_T   ctrl_id,    //in:
                            MMI_CTRL_ID_T   osd_ctrl_id,//in:
                            DPARAM          param       //in:
                            )
{
    GUI_POINT_T     tp_point = {0};

    //get point
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);

    //display osd
    HandleDisplayZoomOsd(win_id,ctrl_id,osd_ctrl_id);

    //stop osd timer
    StopZoomOsdTimer();
    
    //is tp softkey
    s_mmipicview_is_tp_softkey = MMIPICVIEW_IsTpOsdSoftkey(&tp_point);
    if (!s_mmipicview_is_tp_softkey)
    {
        //get point
        s_mmipicview_tp_point = tp_point;
    }
}

/*****************************************************************************/
//  Description : handle picture zoom tp move msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void HandleZoomTpMove(
                            MMI_CTRL_ID_T   ctrl_id,    //in:
                            DPARAM          param       //in:
                            )
{
    int16           move_x = 0;
    int16           move_y = 0;
    GUI_POINT_T     tp_point = {0};

    if (!s_mmipicview_is_tp_softkey)
    {
        //get point
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

        //set move x and y
        move_x = (int16)(s_mmipicview_tp_point.x - tp_point.x);
        move_y = (int16)(s_mmipicview_tp_point.y - tp_point.y);

        if ((0 != move_x) || (0 != move_y))
        {
            //set tp point
            s_mmipicview_tp_point = tp_point;

            //move
            GUIANIM_ZoomMove(ctrl_id,move_x,move_y,TRUE);

            //stop osd timer
            StopZoomOsdTimer();
        }
    }
}

/*****************************************************************************/
//  Description : handle picture zoom tp up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void HandleZoomTpUp(
                          MMI_WIN_ID_T      win_id,     //in:
                          MMI_CTRL_ID_T     ctrl_id,    //in:
                          DPARAM            param       //in:
                          )
{
    //move
    HandleZoomTpMove(ctrl_id,param);

    if (MMK_IsFocusWin(win_id))
    {
        //start display timer
        StartZoomOsdTimer(win_id);
    }
}

/*****************************************************************************/
//  Description : process picture state
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
LOCAL int32 ProcessPicState(MMIPICVIEW_PIC_STATE_E pic_state, //in:
                                MMI_WIN_ID_T     win_id,     //in:
                                MMI_CTRL_ID_T  ctrl_id     //in:zoom ctrl
                                )
{
    int32 ret = 0;
    
#if defined(PICVIEW_F_VIEW_ZOOM)

    //SCI_TRACE_LOW:"MMIPICVIEW ProcessPicState pic_state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_ZOOM_1101_112_2_18_2_46_53_98,(uint8*)"d",pic_state);
    switch(pic_state)
    {
    case MMIPICVIEW_PIC_INIT:
        s_mmipicview_zoom_out_counter = 0;
        
        MMK_PostMsg(win_id, MSG_PICVIEWER_PIC_STATE, PNULL, 0);
        break;
     case MMIPICVIEW_PIC_DECODING:
        {
            BOOLEAN is_decoded = FALSE;
    
            is_decoded = GUIANIM_IsSupport(ctrl_id);
            //SCI_TRACE_LOW:"MMIPICVIEW ProcessPicState is_decoded=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_ZOOM_1115_112_2_18_2_46_53_99,(uint8*)"d",is_decoded);
            if (is_decoded)
            {
                //send web msg to zoom in     
                MMK_SendMsg(win_id, MSG_APP_WEB, PNULL);  
            }
            else
            {
                //sleep for decoding
                if (s_mmipicview_sleep_counter < MMIPICVIEW_VIEW_ZOOM_SLEEP_COUNTER_MAX)
                {
                    MMK_PostMsg(win_id, MSG_PICVIEWER_PIC_STATE, PNULL, 0);
                    
                    SCI_Sleep(MMIPICVIEW_VIEW_ZOOM_SLEEP_TIME);
                    s_mmipicview_sleep_counter++;
                }
            }  
        }    
        break;
     case MMIPICVIEW_PIC_MIN_ZOOM_OUT:
        //SCI_TRACE_LOW:"MMIPICVIEW ProcessPicState zoom_out_counter=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_ZOOM_1136_112_2_18_2_46_53_100,(uint8*)"d",s_mmipicview_zoom_out_counter);
        if (0 < s_mmipicview_zoom_out_counter)
        {
            MMK_PostMsg(win_id, MSG_PICVIEWER_MIN_ZOOM_OUT, PNULL, 0);
        }
        s_mmipicview_zoom_out_counter++;
        break;
     default:
        break;
    }
#endif

    return ret;
}

#endif //#ifdef PIC_VIEWER_SUPPORT
#endif



/*Edit by script, ignore 4 case. Thu Apr 26 19:01:03 2012*/ //IGNORE9527
