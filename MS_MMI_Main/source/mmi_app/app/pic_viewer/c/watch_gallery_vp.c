/*****************************************************************************
** File Name:      watch_gallery_vp.c                                        *
** Author:                                                                   *
** Date:           2020/2/25                                                 *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe gallery vp UI and logic     *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2020/2/25    longwei.qiao       Create                                    *
******************************************************************************/
#ifdef PIC_VIEWER_SUPPORT
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmivp_export.h"
#include "mmivp_internal.h"
#include "window_parse.h"

#include "mmidisplay_data.h"
#include "mmivp_internal.h"
#include "mmivp_text.h"
#include "mmi_resource.h"
#include "mmifmm_export.h"
#include "mmipicview_id.h"

#include "watch_gallery_internal.h"
#include "mmicom_trace.h"
#include "mmipicview_text.h"
#include "mmipicview_image.h"
#include "guictrl_api.h"
#include "watch_commonwin_export.h"
#ifdef WATCH_LEFT_RIGHT_WEAR_SUPPORT
#include "dal_display.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
#define   GALLERY_VP_DISPLAY_RECT           DP2PX_RECT(36,36,203,203)
#define   GALLERY_VP_DEL_BUTTON_RECT        DP2PX_RECT(60,168,107,215)
#define   GALLERY_VP_PLAY_BUTTON_RECT       DP2PX_RECT(132,168,179,215)
#define   GALLERY_VP_PAUSE_BUTTON_RECT      DP2PX_RECT(96,168,143,215)
#else
#define   GALLERY_VP_DISPLAY_RECT           {0,0,239,239}
#define   GALLERY_VP_DEL_BUTTON_RECT        {125,179,227,227}
#define   GALLERY_VP_PLAY_BUTTON_RECT       {11,179,113,227}

#endif
#define   GALLERY_VP_DEL_FOCUS_BORDER       {125-5,179-5,227+5,227+5}
#define   GALLERY_VP_PLAY_FOCUS_BORDER      {11-5,179-5,113+5,227+5}

#define   GALLERY_VP_HALF_TRANSPARENCE     200
/**--------------------------------------------------------------------------*
**                          LOCAL VARIABLES                                  *
**---------------------------------------------------------------------------*/
LOCAL VP_INTERFACE_PLAYING_INFO_T s_gallery_vp_control_info = {0};
LOCAL GUI_LCD_DEV_INFO            s_vp_ctrl_layer           = {0};
LOCAL BOOLEAN                     s_vp_delbtn_is_focus      = FALSE;//记录删除控件获取焦点
LOCAL BOOLEAN                     s_vp_play_end             = FALSE;//播放结束或用户停止播放
/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/
LOCAL void GalleryVpSetDelBtnFocus(BOOLEAN is_focus)
{
    s_vp_delbtn_is_focus=is_focus;
}
LOCAL BOOLEAN GalleryVpDelBtnIsFocus(void)
{
    return s_vp_delbtn_is_focus;
}
#ifdef  VIDEOTHUMBNAIL_SUPPORT
/*****************************************************************************/
//  Description  : 获取缩略图成功的UI 回调函数，主要是显示得到的缩略图
//  Paramter:
//          [In]:image_data
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GalleryDisplayThumbnail(DPLAYER_IMAGE_DATA_T *image_data)
{
    UILAYER_COPYBUF_PARAM_T   copy_param   = {0};
    UILAYER_COPYBUF_T         copy_buf     = {0};
    int16                     image_width  = 0;
    int16                     image_height = 0;
    BOOLEAN                   result       = FALSE;
    GUI_RECT_T                display_rect = GALLERY_VP_DISPLAY_RECT;
    TRACE_APP_GALLERY("[VP]:DisplayThumbnail");
    if(image_data == PNULL ||image_data->coordinate.trim_w==0 ||image_data->coordinate.trim_h==0)
    {
       MMIAPIVP_ThumbnailClose();//获取异常
       return FALSE;
    }
    if(!MMK_IsFocusWin(WATCH_GALLERY_MAIN_WIN_ID))
    {
        MMIAPIVP_ThumbnailClose();
        return FALSE;
    }
    if(VP_STATE_READY != MMIAPIVP_FSMGetState(s_gallery_vp_control_info.vp_handle))
    {
        MMIAPIVP_ThumbnailClose();
        return FALSE;
    }
    MMIAPIVP_ThumbnailClose();
    TRACE_APP_GALLERY("image_data->coordinate.w:%d",image_data->coordinate.w);
    TRACE_APP_GALLERY("image_data->coordinate.h:%d",image_data->coordinate.h);
    TRACE_APP_GALLERY("image_data->coordinate.trim_x:%d",image_data->coordinate.trim_x);
    TRACE_APP_GALLERY("image_data->coordinate.trim_y:%d",image_data->coordinate.trim_y);
    TRACE_APP_GALLERY("image_data->coordinate.trim_w:%d",image_data->coordinate.trim_w);
    TRACE_APP_GALLERY("image_data->coordinate.trim_h:%d",image_data->coordinate.trim_h);

    image_width        = image_data->coordinate.trim_w;
    image_height       = image_data->coordinate.trim_h;
    copy_param.rect    = display_rect;
    copy_buf.data_type = DATA_TYPE_RGB565;
    copy_buf.width     = image_width;
    copy_buf.height    = image_height;
    copy_buf.buf_ptr   = image_data->buf.oneplane.p_data;

    if((copy_param.rect.right-copy_param.rect.left+1)>=image_width && (copy_param.rect.bottom-copy_param.rect.top+1)>=image_height)
    {
            copy_param.rect.left   += (copy_param.rect.right -copy_param.rect.left+1-image_width)/2;
            copy_param.rect.top    += (copy_param.rect.bottom-copy_param.rect.top+1-image_height)/2;
            copy_param.rect.right  =  (copy_param.rect.left + image_width-1);
            copy_param.rect.bottom =  (copy_param.rect.top + image_height-1);

            UILAYER_CopyBufferToLayer((GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev(),&copy_param,&copy_buf);
            MMITHEME_StoreUpdateRect(MMITHEME_GetDefaultLcdDev(),display_rect);
            MMITHEME_UpdateRect();
            result=TRUE;
    }
    return result;
}
/*****************************************************************************/
//  Description  : 视频播放器载入显示祯功能，准备参数并开始缩略图获取
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GalleryNotifyDPLAYERGetTumbnail(void)
{
    VP_DISPLAY_PARAM_T  display_param = {0};
    GUI_RECT_T          display_rect  = GALLERY_VP_DISPLAY_RECT;
#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
    GUI_RECT_T          bg_rect  = GALLERY_BG_RECT;
#else
    GUI_RECT_T          bg_rect  = GALLERY_VP_DISPLAY_RECT;
#endif
    if(!MMK_IsFocusWin(WATCH_GALLERY_MAIN_WIN_ID))
    {
        MMIAPIVP_ThumbnailClose();
        TRACE_APP_GALLERY("not focus win");
        return FALSE;
    }
    display_param.audioSupport   = 0;
    display_param.videoSupport   = 1;
    display_param.b_support_seek = 1;
    display_param.disp_mode      = DPLAYER_DISP_FULLVIEW;
    display_param.RotateAngle    = 0;
    display_param.disp_data_op   = DPLAYER_DISP_DATA_GET;
    display_param.b_no_osd       = 1;

    display_param.disp_rect.dy = display_rect.bottom - display_rect.top + 1;
    display_param.disp_rect.dx = display_rect.right - display_rect.left + 1;
    display_param.disp_rect.x  = display_rect.left;
    display_param.disp_rect.y  = display_rect.top;

    display_param.target_rect.x  = display_param.disp_rect.x;
    display_param.target_rect.y  = display_param.disp_rect.y;
    display_param.target_rect.dx = display_param.disp_rect.dx;
    display_param.target_rect.dy = display_param.disp_rect.dy;

    MMIAPIVP_ThumbnailClose();

    MMIAPIVP_TranslateDisplayParamLogicToPhysical(&display_param, FALSE);
    GUI_FillRect(MMITHEME_GetDefaultLcdDev(),bg_rect,MMI_BLACK_COLOR); //清除 遗留的字符串
    if(MMIAPIVP_ThumbnailOpen(s_gallery_vp_control_info.full_path_name, s_gallery_vp_control_info.full_path_len,MMIAPIVP_FSMGetTimeOffset(s_gallery_vp_control_info.vp_handle),GalleryDisplayThumbnail, &display_param,FALSE))
    {
        TRACE_APP_GALLERY("gallery_vp:NotifyDPLAYERGetTumbnail success !");
        return TRUE;
    }
    else
    {
        TRACE_APP_GALLERY("gallery_vp:NotifyDPLAYERGetTumbnail fail !");
        return FALSE;
    }
}
#endif
#ifdef UI_MULTILAYER_SUPPORT
LOCAL  void GalleryAppendLayerToBlt(void)
{
    UILAYER_APPEND_BLT_T append_layer = {0};
    if(!UILAYER_IsBltLayer(&s_vp_ctrl_layer))//加到图层队列中
    {
       append_layer.layer_level  = UILAYER_LEVEL_NORMAL;
       append_layer.lcd_dev_info = s_vp_ctrl_layer;
       UILAYER_AppendBltLayer(&append_layer);
    }
}
/*****************************************************************************/
//  Description  :create vp ctrl panel layer
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
LOCAL void GalleryCreateVpCtrlPanelLayer(void)
{
    UILAYER_CREATE_T     create_info  = {0};
    GUI_RECT_T           win_rect   = {0};
    MMI_WIN_ID_T         win_id = WATCH_GALLERY_MAIN_WIN_ID;
    MMK_GetWinRect(win_id, &win_rect);

    Gallery_Vp_DestoryLayer();//如果图层已经创建需要先释放

    if (UILAYER_IsMultiLayerEnable())//创建图层
    {
        create_info.lcd_id          = GUI_MAIN_LCD_ID;
        create_info.owner_handle    = win_id;
        create_info.offset_x        = 0;
        create_info.offset_y        = 0;
        create_info.width           = win_rect.right - win_rect.left + 1;
        create_info.height          = win_rect.bottom- win_rect.top + 1;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;
        UILAYER_CreateLayer(&create_info, &s_vp_ctrl_layer);
    }
}
LOCAL void GalleryClearVpCtrlPanelLayer(void)
{
      if(UILAYER_IsBltLayer(&s_vp_ctrl_layer))
      {
         UILAYER_Clear(&s_vp_ctrl_layer);
      }
}
/*****************************************************************************/
//  Description  :release vp layer
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void Gallery_Vp_DestoryLayer(void)
{
    if(UILAYER_IsBltLayer(&s_vp_ctrl_layer))
    {
       UILAYER_RELEASELAYER(&s_vp_ctrl_layer);
    }
}
#endif
#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
LOCAL void GallerySetVpCtrlParam(MMI_WIN_ID_T win_id)
{
    //ctrl id
    MMI_CTRL_ID_T         del_button_id      = MMIGALLERY_VP_DEL_BUTTON_CTRL_ID;
    MMI_CTRL_ID_T         play_button_id     = MMIGALLERY_VP_PLAY_BUTTON_CTRL_ID;
    MMI_CTRL_ID_T         pause_button_id    = MMIGALLERY_VP_PAUSE_BUTTON_CTRL_ID;
    //rect
    GUI_RECT_T            play_button_rect   = GALLERY_VP_PLAY_BUTTON_RECT;
    GUI_RECT_T            del_button_rect    = GALLERY_VP_DEL_BUTTON_RECT;
    GUI_RECT_T            pause_button_rect  = GALLERY_VP_PAUSE_BUTTON_RECT;
    //ctrl handle
//    MMI_HANDLE_T          del_button_handle  = MMK_GetCtrlHandleByWin(win_id,del_button_id); //CID 610567
//    MMI_HANDLE_T          play_button_handle = MMK_GetCtrlHandleByWin(win_id,play_button_id);
//    MMI_HANDLE_T          pause_button_handle = MMK_GetCtrlHandleByWin(win_id,pause_button_id);
    //set lcd info
    GUIAPICTRL_SetLcdDevInfo(play_button_id, &s_vp_ctrl_layer);
    GUIAPICTRL_SetLcdDevInfo(del_button_id,&s_vp_ctrl_layer);
    GUIAPICTRL_SetLcdDevInfo(pause_button_id,&s_vp_ctrl_layer);
    //set layout
    GUIBUTTON_SetRect(play_button_id, &play_button_rect);
    GUIBUTTON_SetRect(del_button_id, &del_button_rect);
    GUIBUTTON_SetRect(pause_button_id, &pause_button_rect);
    //forbid paint button
    GUIBUTTON_SetForbidPaint(del_button_id, TRUE);
    GUIBUTTON_SetForbidPaint(play_button_id, TRUE);
    GUIBUTTON_SetForbidPaint(pause_button_id, TRUE);
}
LOCAL void GalleryDrawVpCtrlPanelLayer(void)
{
//    MMI_WIN_ID_T         win_id = WATCH_GALLERY_MAIN_WIN_ID;
    MMI_CTRL_ID_T         del_button_id   = MMIGALLERY_VP_DEL_BUTTON_CTRL_ID;
    MMI_CTRL_ID_T         play_button_id  = MMIGALLERY_VP_PLAY_BUTTON_CTRL_ID;
    MMI_CTRL_ID_T         pause_button_id = MMIGALLERY_VP_PAUSE_BUTTON_CTRL_ID;
    UILAYER_Clear(&s_vp_ctrl_layer);
    //is visible
    if(s_gallery_vp_control_info.state == VP_STATE_PLAYING)
    {
        GUIBUTTON_SetVisible(pause_button_id,TRUE,TRUE);
        GUIBUTTON_SetVisible(del_button_id,FALSE,FALSE);
        GUIBUTTON_SetVisible(play_button_id,FALSE,FALSE);
    }
    else
    {
        GUIBUTTON_SetVisible(del_button_id,TRUE,TRUE);
        GUIBUTTON_SetVisible(play_button_id,TRUE,TRUE);
        GUIBUTTON_SetVisible(pause_button_id,FALSE,FALSE);
    }
}
#else

/*****************************************************************************/
//  Description : Set Vp Ctrl Param
//  Paramter:
//          [In]:win_id
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
LOCAL void GallerySetVpCtrlParam(MMI_WIN_ID_T win_id)
{
    GUI_BG_T              bg_info            = {0};
    MMI_STRING_T          del_text           = {0};
    MMI_STRING_T          play_text          = {0};
    GUI_FONT_ALL_T        font_info          = {0};
    //ctrl id
    MMI_CTRL_ID_T         del_button_id      = MMIGALLERY_VP_DEL_BUTTON_CTRL_ID;
    MMI_CTRL_ID_T         play_button_id     = MMIGALLERY_VP_PLAY_BUTTON_CTRL_ID;
    //text id
    MMI_TEXT_ID_T         play_text_id       = TXT_GALLERY_PLAY_BUTTON;
    MMI_TEXT_ID_T         del_text_id        = TXT_GALLERY_DEL_BUTTON;
    //rect
    GUI_RECT_T            play_button_rect   = GALLERY_VP_PLAY_BUTTON_RECT;
    GUI_RECT_T            del_button_rect    = GALLERY_VP_DEL_BUTTON_RECT;
    //ctrl handle
    MMI_HANDLE_T          del_button_handle  = MMK_GetCtrlHandleByWin(win_id,del_button_id);
    MMI_HANDLE_T          play_button_handle = MMK_GetCtrlHandleByWin(win_id,play_button_id);

    GUIAPICTRL_SetLcdDevInfo(play_button_handle,&s_vp_ctrl_layer);
    GUIAPICTRL_SetLcdDevInfo(del_button_id,&s_vp_ctrl_layer);
    //set layout
    GUIBUTTON_SetRect(play_button_id, &play_button_rect);
    GUIBUTTON_SetRect(del_button_id, &del_button_rect);
    //forbid paint button
    GUIBUTTON_SetForbidPaint(del_button_id, TRUE);
    GUIBUTTON_SetForbidPaint(play_button_id, TRUE);
    //set draw param
    if(s_gallery_vp_control_info.state == VP_STATE_PLAYING)
    {
        play_text_id = TXT_GALLERY_PAUSE_BUTTON;
    }
    MMI_GetLabelTextByLang(play_text_id, &play_text);
    MMI_GetLabelTextByLang(del_text_id, &del_text);
    font_info.font  = WATCH_DEFAULT_NORMAL_FONT;
    font_info.color = MMI_WHITE_COLOR;
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id  = res_gallery_vp_button_bg;
    //PLAY button
    GUIBUTTON_SetBg(play_button_id, &bg_info);
    GUIBUTTON_SetTextAlign(play_button_id,ALIGN_HVMIDDLE);
    GUIBUTTON_SetText(play_button_id,play_text.wstr_ptr,play_text.wstr_len);
    GUIBUTTON_SetFont(play_button_id,&font_info);
    //del button
    GUIBUTTON_SetBg(del_button_id, &bg_info);
    GUIBUTTON_SetTextAlign(del_button_id,ALIGN_HVMIDDLE);
    GUIBUTTON_SetText(del_button_id,del_text.wstr_ptr,del_text.wstr_len);
    GUIBUTTON_SetFont(del_button_id,&font_info);
}
LOCAL void GalleryDrawVpCtrlPanelLayer(void)
{
    MMI_CTRL_ID_T           del_button_id  = MMIGALLERY_VP_DEL_BUTTON_CTRL_ID;
    MMI_CTRL_ID_T           play_button_id = MMIGALLERY_VP_PLAY_BUTTON_CTRL_ID;
    MMI_STRING_T            play_text      = {0};
    //UILAYER_Clear(&s_vp_ctrl_layer);//画之前先清除
    //is visible
    MMI_TEXT_ID_T           play_text_id   = TXT_GALLERY_PLAY_BUTTON;
    if(s_gallery_vp_control_info.state == VP_STATE_PLAYING)
    {
        play_text_id = TXT_GALLERY_PAUSE_BUTTON;
    }
    MMI_GetLabelTextByLang(play_text_id, &play_text);
    GUIBUTTON_SetText(play_button_id,play_text.wstr_ptr,play_text.wstr_len);

    GUIBUTTON_SetVisible(del_button_id,TRUE,TRUE);
    GUIBUTTON_SetVisible(play_button_id,TRUE,TRUE);
}

#endif
LOCAL void GalleryDrawFocusCtrlRect(BOOLEAN is_focus)
{
    GUI_RECT_T   play_focus_rect = GALLERY_VP_PLAY_FOCUS_BORDER;
    GUI_RECT_T	 del_focus_rect  = GALLERY_VP_DEL_FOCUS_BORDER;
    UILAYER_Clear(&s_vp_ctrl_layer);
    if(!is_focus)
    {
       LCD_DrawRect(&s_vp_ctrl_layer,play_focus_rect, GALLERY_FOCUS_BORDER_COLOR);
    }
    else
    {
       LCD_DrawRect(&s_vp_ctrl_layer,del_focus_rect, GALLERY_FOCUS_BORDER_COLOR);
    }

}
/*****************************************************************************/
//  Description : display vp osd
//  Paramter:
//          [In]:is_get_tumbnail,del_btn_is_focus
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
LOCAL void GalleryVpDisplayOSD(BOOLEAN is_get_tumbnail,BOOLEAN del_btn_is_focus)
{
#ifdef  VIDEOTHUMBNAIL_SUPPORT
    if(is_get_tumbnail)
    {
       GalleryNotifyDPLAYERGetTumbnail();
    }
#endif

#ifdef ADULT_WATCH_SUPPORT
    Gallery_Main_DrawIndicator(TRUE);
#else
#if defined(SCREEN_SHAPE_CIRCULAR)
    //Gallery_main_ClearScreen();
#endif
    GalleryVpSetDelBtnFocus(del_btn_is_focus);
    GalleryDrawFocusCtrlRect(del_btn_is_focus);
#endif
    GalleryDrawVpCtrlPanelLayer();
}
LOCAL void GalleryVpDisappearOSD(void)
{
    MMI_CTRL_ID_T           vp_del_ctrl_id  = MMIGALLERY_VP_DEL_BUTTON_CTRL_ID;
    MMI_CTRL_ID_T           vp_play_ctrl_id = MMIGALLERY_VP_PLAY_BUTTON_CTRL_ID;
#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
    MMI_CTRL_ID_T         pause_button_id = MMIGALLERY_VP_PAUSE_BUTTON_CTRL_ID;
    GUIBUTTON_SetVisible(pause_button_id,FALSE,FALSE);
#endif
    GUIBUTTON_SetVisible(vp_del_ctrl_id,FALSE,FALSE);
    GUIBUTTON_SetVisible(vp_play_ctrl_id,FALSE,FALSE);

#ifdef ADULT_WATCH_SUPPORT
    Gallery_Main_DrawIndicator(FALSE);
#endif
    GalleryClearVpCtrlPanelLayer();
}
/*****************************************************************************/
//  Description  :stop vp play and clear display
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:清空图层显示界面，如果当前视频播放并未结束，需要停止
/*****************************************************************************/
PUBLIC BOOLEAN Gallery_Vp_StopPlayAndClearDis(void)
{
   BOOLEAN     result = FALSE;
   GalleryClearVpCtrlPanelLayer();
   if(VP_STATE_READY != MMIAPIVP_FSMGetState(s_gallery_vp_control_info.vp_handle))
   {
        result = MMIAPIVP_FSMStop(s_gallery_vp_control_info.vp_handle);
   }
   return result;
}
/*****************************************************************************/
//  Description : set video button is visible
//  Paramter:
//          [In]:is_visible
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void Gallery_Vp_SetPanelVisible(BOOLEAN is_visible)
{
    if(is_visible)
    {
       GalleryVpDisplayOSD(FALSE,FALSE);
    }
    else
    {
       GalleryVpDisappearOSD();
    }
}
/*****************************************************************************/
//  Description : get video file full Path and total time
//  Paramter:
//          [In]:file_info_ptr
//          [Out]:s_gallery_vp_control_info.full_path_name
//          [Out]:s_gallery_vp_control_info.total_time
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
LOCAL void GalleryGetFilePathAndTime(FILEARRAY_DATA_T  *file_info_ptr)
{
    DPLAYER_MEDIA_INFO_T media_info = {0};
    //get play file full path
    SCI_MEMSET(s_gallery_vp_control_info.full_path_name,0,((VP_MAX_NAME_LEN +1)*sizeof(wchar)));
    s_gallery_vp_control_info.full_path_len = (uint16)MIN(file_info_ptr->name_len,VP_MAX_NAME_LEN);
    TRACE_APP_GALLERY("cur file full path len is %d",s_gallery_vp_control_info.full_path_len);
    MMIAPICOM_Wstrncpy(s_gallery_vp_control_info.full_path_name,file_info_ptr->filename,s_gallery_vp_control_info.full_path_len);
    //get play file total time
    MMIAPIVP_GetInfoFromFullPathName(s_gallery_vp_control_info.full_path_name, s_gallery_vp_control_info.full_path_len, &media_info);
    s_gallery_vp_control_info.total_time = media_info.media_length;
}

/*****************************************************************************/
//  Description : start paly video file
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E GalleryStartPlayVideo(void)
{
    VP_PLAY_PARAM_T*    play_param_ptr = PNULL;
    MMI_RESULT_E        result         = MMI_RESULT_TRUE;
    GUI_RECT_T          display_rect   = GALLERY_VP_DISPLAY_RECT;

    play_param_ptr = (VP_PLAY_PARAM_T*)SCI_ALLOCA(sizeof(VP_PLAY_PARAM_T));
    if (PNULL == play_param_ptr)
    {
        TRACE_APP_GALLERY("GalleryStartPlayVideo:ALLOCA error");
        return MMI_RESULT_FALSE;
    }
    SCI_MEMSET(play_param_ptr,0,sizeof(VP_PLAY_PARAM_T));

    s_gallery_vp_control_info.state = VP_STATE_PLAYING;
    play_param_ptr->dual_sys = 0;
    play_param_ptr->display_param.audioSupport = 1;
    play_param_ptr->display_param.videoSupport = 1;
    play_param_ptr->display_param.b_support_seek = 1;
    play_param_ptr->display_param.disp_mode = DPLAYER_DISP_FULLVIEW;
    play_param_ptr->display_param.RotateAngle = 0;//Bug 1583376
    play_param_ptr->is_scal_up = FALSE;

    play_param_ptr->display_param.disp_rect.dy = display_rect.bottom - display_rect.top + 1;
    play_param_ptr->display_param.disp_rect.dx = display_rect.right - display_rect.left + 1;
    play_param_ptr->display_param.disp_rect.x  = display_rect.left;
    play_param_ptr->display_param.disp_rect.y  = display_rect.top;

    play_param_ptr->display_param.target_rect.x  = play_param_ptr->display_param.disp_rect.x;
    play_param_ptr->display_param.target_rect.y  = play_param_ptr->display_param.disp_rect.y;
    play_param_ptr->display_param.target_rect.dx = play_param_ptr->display_param.disp_rect.dx;
    play_param_ptr->display_param.target_rect.dy = play_param_ptr->display_param.disp_rect.dy;

    play_param_ptr->video_buf_info.type = 0;
    play_param_ptr->video_buf_info.video_buf_len = 0;
    play_param_ptr->video_buf_info.video_buf_ptr = PNULL;
    play_param_ptr->play_type = s_gallery_vp_control_info.play_type;

    play_param_ptr->full_path_len = s_gallery_vp_control_info.full_path_len;
    play_param_ptr->full_path_ptr = s_gallery_vp_control_info.full_path_name;
    play_param_ptr->url_ptr = PNULL;
    play_param_ptr->mode = 0;
    play_param_ptr->is_random_play = FALSE;

    play_param_ptr->vol = MMIAPISET_GetMultimVolume();
    result = MMIAPIVP_FSMPlay(s_gallery_vp_control_info.vp_handle,play_param_ptr);
    TRACE_APP_GALLERY("GalleryStartPlayVideo:result is %d",result);
    SCI_FREE(play_param_ptr);
    play_param_ptr = PNULL;

    return result;
}
/*****************************************************************************/
//  Description : play or pause video file
//  Paramter:
//          [In]:
//          [Out]:
//          [Return]:
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
LOCAL void GalleryVpPlayOrPause(void)
{
    VP_HANDLE     vp_handle = s_gallery_vp_control_info.vp_handle;
    MMI_RESULT_E  result    = MMI_RESULT_TRUE;
    TRACE_APP_GALLERY("enter vp play or pause fun");
    if (VP_STATE_READY == MMIAPIVP_FSMGetState(vp_handle)
        && 0 != s_gallery_vp_control_info.full_path_len)
    {
        //set transparence effect
#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)// bug 2222027
        GUIBLOCK_SetType(255, MMIAPIVP_GetTransparenceColor(),GUIBLOCK_TYPE_OSD, (GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev());
#else
        GUIBLOCK_SetType(GALLERY_VP_HALF_TRANSPARENCE, MMIAPIVP_GetTransparenceColor(),GUIBLOCK_TYPE_OSD, (GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev());
#endif
#ifdef  VIDEOTHUMBNAIL_SUPPORT
        MMIAPIVP_ThumbnailClose();
#endif
        GalleryStartPlayVideo();
        TRACE_APP_GALLERY("start play file");
    }
    else if (VP_STATE_PAUSED == MMIAPIVP_FSMGetState(vp_handle))
    {
        MMIAPIVP_FSMResume(vp_handle);
        TRACE_APP_GALLERY("resume play file");
    }
    else if (VP_STATE_PLAYING == MMIAPIVP_FSMGetState(vp_handle))
    {
        MMIAPIVP_FSMPause(vp_handle);
        TRACE_APP_GALLERY("pause play file");
    }
}

LOCAL void GalleryVpDisplayErrorCode(VP_ERROR_E error_code)
{
    MMI_TEXT_ID_T text_id = TXT_NULL;

    switch(error_code)
    {
        case VP_ERROR_NOT_SUPPORT:
        {
            text_id = TXT_COMMON_NO_SUPPORT;
        }
        break;
        case VP_ERROR_INTERNAL_SERVER_ERROR:
        {
            text_id = TXT_VP_INTERNAL_SERVER_ERROR;
        }
        break;
        case VP_ERROR_BAD_REQUEST:
        {
            text_id = TXT_COMM_BAD_REQUEST;
        }
        break;
        case VP_ERROR_UNAUTHORIZED:
        {
            text_id = TXT_UNAUTHORIZED;
        }
        break;
        case VP_ERROR_NOT_FOUND:
        {
            text_id = TXT_COM_FILE_NO_EXIST;
        }
        break;
        case VP_ERROR_SERVER_CLOSE_IND:
        {
            text_id = TXT_VP_SERVER_CLOSE_IND;
        }
        break;
        case VP_ERROR_DATA_TIMEOUT:
        {
            text_id = TXT_VP_DATA_TIMEOUT;
        }
        break;
        case VP_ERROR_CONNECT_TIMEOUT:
        {
            text_id = TXT_VP_CONNECT_TIMEOUT;
        }
        break;
        case VP_ERROR_NULL_FILE:
        {
            text_id = TXT_EMPTY_FILE;
        }
        break;
        case VP_ERROR_PDP_ACTIVE_ERROR:
        {
            text_id = TXT_VP_PDP_ACTIVE_ERROR;
        }
        break;
        case VP_ERROR_PDP_DEACTIVE_IND:
        {
            text_id = TXT_VP_PDP_DEACTIVE_IND;
        }
        break;
#ifdef DRM_SUPPORT
        case VP_ERROR_DRM_RIGHT_INVALID:
        {
            text_id = TXT_DRM_COPYRIGHTS_INVLIAD_NOT_OPERATE;
        }
        break;
        case VP_ERROR_DRM_NO_SYS_NITZ:
        {
            text_id = TXT_DRM_NOT_SYS_NITZ;
        }
        break;
#endif
        default:
        {
            text_id = TXT_COMM_PLAY_ERROR;
        }
        break;
    }
    {
       MMI_STRING_T              tip_content  = {0};
       WATCH_SOFTKEY_TEXT_ID_T   softket_test = {TXT_NULL,TXT_NULL,TXT_NULL};
       MMI_GetLabelTextByLang(text_id, &tip_content);
#ifndef ADULT_WATCH_SUPPORT
       WatchCOM_NoteWin_2Line_Enter(WATCH_GALLERY_NOTE_WIN_ID,&tip_content,0,softket_test,PNULL);
#else
       Adult_WatchCOM_NoteWin_1Line_Toast_Enter(WATCH_GALLERY_NOTE_WIN_ID,&tip_content,0,PNULL);
#endif
    }
}

/*****************************************************************************/
//  Description :vp handle fsm msg
//  Paramter:
//          [In]:msg_ptr
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note: 处理状态机给ui发送的消息
/*****************************************************************************/
LOCAL void GalleryVpHandleFsmMsg(VP_CALLBACK_T *msg_ptr)
{
    MMI_WIN_ID_T  win_id    = WATCH_GALLERY_MAIN_WIN_ID;
    if (PNULL == msg_ptr || PNULL == msg_ptr->para)
    {
        TRACE_APP_GALLERY("GalleryVpHandleFsmMsg:msg_ptr=PNULL,or msg_ptr->para=PNULL");
        return;
    }
    if(!MMK_IsFocusWin(win_id))
    {
        TRACE_APP_GALLERY("cur win losed focus");
        return;
    }
    if(MMIGALLERY_FILE_TYPE_VIDEO != Gallery_GetCurPreviewFileType())//bug1637782
    {
        TRACE_APP_GALLERY("cur file not video");
        return;
    }
    switch(msg_ptr->msg_id)
    {
        case VP_PLAY_CNF://开始播放
        {
             VP_PLAY_CNF_T play_result = *(VP_PLAY_CNF_T *)msg_ptr->para;
             if(VP_ERROR_NONE == play_result.error_code)
             {
                 MMIDEFAULT_AllowTurnOffBackLight(FALSE);
                 GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, STXT_OK, STXT_STOP, TRUE);
                 TRACE_APP_GALLERY("play success");
                 Gallery_Vp_SetBtnVisible(FALSE);
                 s_gallery_vp_control_info.state = VP_STATE_PLAYING;
             }
             else
             {
                 GUIBLOCK_ResetMainType();
                 TRACE_APP_GALLERY("play error and error code is %d",play_result.error_code);
                 s_gallery_vp_control_info.state = VP_STATE_READY;
                 GalleryVpDisplayErrorCode(play_result.error_code);
             }
        }
        break;
        case VP_PAUSE_CNF://暂停
        {
             VP_PAUSE_CNF_T pause_result = *(VP_PAUSE_CNF_T *)msg_ptr->para;
             if (VP_ERROR_NONE == pause_result.error_code)
             {
                MMIDEFAULT_AllowTurnOffBackLight(TRUE);
                TRACE_APP_GALLERY("pause success");
                s_gallery_vp_control_info.state = VP_STATE_PAUSED;
                Gallery_Vp_SetBtnVisible(TRUE);
             }
             else
             {
                TRACE_APP_GALLERY("pause error and error code is %d",pause_result.error_code);
             }
        }
        break;
        case VP_PLAY_END_IND:
        {
             TRACE_APP_GALLERY("play end confirm");
             s_vp_play_end=TRUE;
        }
        break;
        case VP_STOP_IND: //全部停止播放
        {
             TRACE_APP_GALLERY("stop confirm");
             GUIBLOCK_ResetMainType();
             MMIDEFAULT_AllowTurnOffBackLight(TRUE);
             s_gallery_vp_control_info.state = VP_STATE_READY;
             GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, STXT_OK, STXT_RETURN, TRUE);
             if(s_vp_play_end)//播放正常停止或者由用户主动停止,需要返回初始状态界面.
             {
                Gallery_Main_SetDisplayOsd(win_id,TRUE);
                GalleryVpDisplayOSD(TRUE,FALSE);
             }
             s_vp_play_end=FALSE;
        }
        break;
        default:
        {
             //do nothing
        }
        break;
    }
}
/*****************************************************************************/
//  Description  :create vp fsm and init
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void Gallery_Vp_CreateFsmAndInit(void)
{
    SCI_MEMSET(&s_gallery_vp_control_info,0,sizeof(VP_INTERFACE_PLAYING_INFO_T));
    s_gallery_vp_control_info.state        = VP_STATE_READY;
    s_gallery_vp_control_info.play_type    = VP_PLAY_FROM_FILE;
    s_gallery_vp_control_info.vp_handle    = MMIAPIVP_FSMInit(GalleryVpHandleFsmMsg);
#ifdef UI_MULTILAYER_SUPPORT
    GalleryCreateVpCtrlPanelLayer();
#endif
}
/*****************************************************************************/
//  Description  :release vp fsm
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void Gallery_Vp_FSMRelease(void)
{
    MMIAPIVP_FSMRelease(s_gallery_vp_control_info.vp_handle);
}
/*****************************************************************************/
//  Description : set video button is visible
//  Paramter:
//          [In]:is_visible,is_update
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void Gallery_Vp_SetBtnVisible(BOOLEAN is_visible)
{
    if(is_visible)
    {
       GalleryVpDisplayOSD(FALSE,FALSE);
    }
    else
    {
       GalleryVpDisappearOSD();
    }
}
/*****************************************************************************/
//  Description : pause for CC
//  Paramter:
//          [In]:
//          [Out]:
//          [Return]:
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void WatchGallery_Vp_PauseForCC(void)
{
    if(MMK_IsOpenWin(WATCH_GALLERY_MAIN_WIN_ID))
    {
        if (VP_STATE_PLAYING == MMIAPIVP_FSMGetState(s_gallery_vp_control_info.vp_handle))
        {
            MMIAPIVP_FSMPause(s_gallery_vp_control_info.vp_handle);
            TRACE_APP_GALLERY("pause play file for cc");
            s_gallery_vp_control_info.state = VP_STATE_PAUSED;
        }
    }
}
LOCAL void VP_HandleLeftKeyMsg(MMI_WIN_ID_T        win_id)
{
     if(Gallery_Main_IsDisplayOsd())
     {
        if(GalleryVpDelBtnIsFocus())//转换焦点
        {
            GalleryVpDisplayOSD(FALSE,FALSE);
        }
     }
     else
     {
         GalleryVpDisplayOSD(FALSE,FALSE);
     }
     Gallery_Main_SetDisplayOsd(win_id,TRUE);
}
LOCAL void VP_HandleRightKeyMsg(MMI_WIN_ID_T   win_id)
{
    if(Gallery_Main_IsDisplayOsd())
    {
        if(!GalleryVpDelBtnIsFocus())
        {
           GalleryVpDisplayOSD(FALSE,TRUE);
        }
    }
    else
    {
        GalleryVpDisplayOSD(FALSE,FALSE);
    }
    Gallery_Main_SetDisplayOsd(win_id,TRUE);
}
LOCAL void VP_HandlePENOKMsg(MMI_WIN_ID_T  win_id,DPARAM  param)
{
    if(PNULL != param)
    {
      switch(((MMI_NOTIFY_T*) param)->src_id)
       {
           case MMIGALLERY_VP_DEL_BUTTON_CTRL_ID:
           {
                Gallery_Main_SetDisplayOsd(win_id,FALSE);
                Gallery_Main_OpenDelQueryWin();
           }
           break;
           case MMIGALLERY_VP_PLAY_BUTTON_CTRL_ID:
           {
                GalleryVpPlayOrPause();
                if(VP_STATE_PLAYING==MMIAPIVP_FSMGetState(s_gallery_vp_control_info.vp_handle))
                {
                   Gallery_Main_SetDisplayOsd(win_id,FALSE);
                }
                else
                {
                   Gallery_Main_SetDisplayOsd(win_id,TRUE);
                }
           }
           break;
           #if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
           case MMIGALLERY_VP_PAUSE_BUTTON_CTRL_ID:
           {
                GalleryVpPlayOrPause();
                if(VP_STATE_PLAYING==MMIAPIVP_FSMGetState(s_gallery_vp_control_info.vp_handle))
                {
                   Gallery_Main_SetDisplayOsd(win_id,FALSE);
                }
                else
                {
                   Gallery_Main_SetDisplayOsd(win_id,TRUE);
                }
           }
           break;
           #endif
           default:
           {
               TRACE_APP_GALLERY("VP:((MMI_NOTIFY_T*) param)->src_id is error");
           }
       }
    }
}
LOCAL void VP_HandleWebKeyMsg(MMI_WIN_ID_T   win_id)
{
     if(Gallery_Main_IsDisplayOsd())
     {
        if(GalleryVpDelBtnIsFocus())
        {
            Gallery_Main_SetDisplayOsd(win_id,FALSE);
            Gallery_Main_OpenDelQueryWin();
        }
        else
        {
             GalleryVpPlayOrPause();
             if(VP_STATE_PLAYING==MMIAPIVP_FSMGetState(s_gallery_vp_control_info.vp_handle))
             {
                Gallery_Main_SetDisplayOsd(win_id,FALSE);
             }
             else
             {
                Gallery_Main_SetDisplayOsd(win_id,TRUE);
             }
        }
     }
     else
     {
         Gallery_Main_SetDisplayOsd(win_id,TRUE);
         GalleryVpDisplayOSD(FALSE,FALSE);
     }
}
LOCAL void VP_HandleLoseFocusMsg(MMI_WIN_ID_T   win_id)
{
     MMIDEFAULT_EnableKeyRing(GALLERY_MODULE_NAME, TRUE);//1352322
     GUIBLOCK_ResetMainType();
     Gallery_Main_SetDisplayOsd(win_id,FALSE);
     GalleryVpDisappearOSD();//隐藏OSD显示
     if(VP_STATE_PLAYING==MMIAPIVP_FSMGetState(s_gallery_vp_control_info.vp_handle))
     {
        MMIAPIVP_FSMPause(s_gallery_vp_control_info.vp_handle);
        s_gallery_vp_control_info.state = VP_STATE_PAUSED;//bug 1335466
     }
}
LOCAL void VP_HandleGetFocusMsg(MMI_WIN_ID_T   win_id)
{
#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
    GUI_RECT_T          bg_rect = GALLERY_BG_RECT;
#else
    GUI_RECT_T          bg_rect = GALLERY_VP_DISPLAY_RECT;
#endif
     MMIDEFAULT_EnableKeyRing(GALLERY_MODULE_NAME, FALSE);//1352322
     GalleryAppendLayerToBlt();
     if(VP_STATE_READY != MMIAPIVP_FSMGetState(s_gallery_vp_control_info.vp_handle))
     {
        //set transparence effect
#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR) // bug 2222027
        GUIBLOCK_SetType(255, MMIAPIVP_GetTransparenceColor(),GUIBLOCK_TYPE_OSD, (GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev());
#else
        GUIBLOCK_SetType(GALLERY_VP_HALF_TRANSPARENCE, MMIAPIVP_GetTransparenceColor(),GUIBLOCK_TYPE_OSD, (GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev());
#endif
        //暂停状态直接填充透明色,清除遗留的字串
        GUI_FillRect(MMITHEME_GetDefaultLcdDev(),bg_rect, MMIAPIVP_GetTransparenceColor());//清除主层显示
        GalleryVpDisplayOSD(FALSE,GalleryVpDelBtnIsFocus());
     }
     else
     {
        GalleryVpDisplayOSD(TRUE,GalleryVpDelBtnIsFocus());
     }
     Gallery_Main_SetDisplayOsd(win_id,TRUE);
}
/*****************************************************************************/
//  Description : handle gallery Vp msg
//  Paramter:
//          [In]:win_id,msg_id,param
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E Gallery_Vp_HandleMsg(
                                                 MMI_WIN_ID_T        win_id,
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                               )
{
    MMI_RESULT_E        result       = MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        {
             VP_HandleWebKeyMsg(win_id);
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
        {
             VP_HandlePENOKMsg(win_id, param);
        }
        break;
#endif
        case MSG_LOSE_FOCUS:
        {
             VP_HandleLoseFocusMsg(win_id);
        }
        break;
        case MSG_GET_FOCUS:
        {
             VP_HandleGetFocusMsg(win_id);
        }
        break;
        case MSG_APP_LEFT:
        {
             VP_HandleLeftKeyMsg(win_id);
        }
        break;
        case MSG_APP_RIGHT:
        {
             VP_HandleRightKeyMsg(win_id);
        }
        break;
        case MSG_APP_OK:
        {
            Gallery_Main_SetDisplayOsd(win_id,FALSE);
            GalleryVpDisappearOSD();//隐藏OSD显示
        }
        break;
        case MSG_APP_CANCEL:
        case MSG_APP_RED:
        {
             if(VP_STATE_READY!=MMIAPIVP_FSMGetState(s_gallery_vp_control_info.vp_handle))
             {
                s_vp_play_end=TRUE;
                TRACE_APP_GALLERY("VP:users stop play");
                MMIAPIVP_FSMStop(s_gallery_vp_control_info.vp_handle);
                break;
             }
             MMK_CloseWin(win_id);
        }
        break;
        default:
        {
             result = MMI_RESULT_FALSE;
        }
        break;
    }
    return result;
}
#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
//ctrl table of preview video file W307
WINDOW_TABLE(WATCH_GALLERY_CTRL_TAB_VP) =
{
    CREATE_BUTTON_CTRL(res_gallery_ic_play,MMIGALLERY_VP_PLAY_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(res_gallery_ic_delete,MMIGALLERY_VP_DEL_BUTTON_CTRL_ID),

    CREATE_BUTTON_CTRL(res_gallery_ic_pause,MMIGALLERY_VP_PAUSE_BUTTON_CTRL_ID),
#ifndef TOUCH_PANEL_SUPPORT
    WIN_SOFTKEY(TXT_NULL, STXT_OK, STXT_RETURN),
#endif
    END_WIN
};
#else
//ctrl table of preview video file W317
WINDOW_TABLE(WATCH_GALLERY_CTRL_TAB_VP) =
{
    CREATE_BUTTON_CTRL(NULL,MMIGALLERY_VP_PLAY_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(NULL,MMIGALLERY_VP_DEL_BUTTON_CTRL_ID),
#ifndef TOUCH_PANEL_SUPPORT
    WIN_SOFTKEY(TXT_NULL, STXT_OK, STXT_RETURN),
#endif
    END_WIN
};
#endif
/*****************************************************************************/
//  Description : display video file preview
//  Paramter:
//          [In]:file_info
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void Gallery_Vp_VideoFilePreview(MMI_WIN_ID_T win_id,FILEARRAY_DATA_T* file_info)
{
    VP_HANDLE  vp_handle = s_gallery_vp_control_info.vp_handle;
    SCI_MEMSET(&s_gallery_vp_control_info,0,sizeof(VP_INTERFACE_PLAYING_INFO_T));
    s_gallery_vp_control_info.state = VP_STATE_READY;
    s_gallery_vp_control_info.vp_handle=vp_handle;
    s_gallery_vp_control_info.play_type    = VP_PLAY_FROM_FILE;
    MMK_CreateCtrlByWinTabEx(win_id,(uint32 *)WATCH_GALLERY_CTRL_TAB_VP);
    GalleryAppendLayerToBlt();
    GallerySetVpCtrlParam(win_id);
    GalleryGetFilePathAndTime(file_info);
    GalleryVpDisplayOSD(TRUE,FALSE);
}
#endif

